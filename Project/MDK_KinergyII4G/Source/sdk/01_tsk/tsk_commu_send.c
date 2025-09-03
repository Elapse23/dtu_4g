/**
 * =====================================================================================
 * @file        tsk_commu_send.c
 * @brief       通信发送任务实现
 * @author      23Elapse & GitHub Copilot
 * @version     1.0
 * @date        2025-09-03
 * @note        处理ESP AT命令发送和RS485串口发送
 * =====================================================================================
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "bsp_uart_ring_buffer.h"
#include "bsp_log_manager.h"
#include <string.h>
#include <stdio.h>

/* 任务参数 */
#define SEND_TASK_STACK_SIZE        (configMINIMAL_STACK_SIZE * 2)
#define SEND_TASK_PRIORITY          (tskIDLE_PRIORITY + 3)
#define SEND_QUEUE_LENGTH           20
#define SEND_TIMEOUT_MS             1000
#define AT_RESPONSE_TIMEOUT_MS      5000

/* 发送消息类型 */
typedef enum {
    SEND_TYPE_ESP_AT = 0,       // ESP AT命令
    SEND_TYPE_RS485_DATA,       // RS485数据
    SEND_TYPE_HEARTBEAT,        // 心跳数据
    SEND_TYPE_QUERY            // 查询命令
} SendMessageType_t;

/* 发送消息结构 */
typedef struct {
    SendMessageType_t type;     // 消息类型
    UART_ID_t target_uart;      // 目标串口
    uint8_t data[256];          // 发送数据
    uint16_t length;            // 数据长度
    uint32_t timeout_ms;        // 超时时间
    bool wait_response;         // 是否等待响应
    char expected_response[64]; // 期望的响应
} SendMessage_t;

/* ESP AT命令状态 */
typedef enum {
    ESP_STATE_IDLE = 0,
    ESP_STATE_CONNECTING,
    ESP_STATE_CONNECTED,
    ESP_STATE_SENDING,
    ESP_STATE_ERROR
} EspState_t;

/* 静态变量 */
static QueueHandle_t s_send_queue = NULL;
static SemaphoreHandle_t s_send_mutex = NULL;
static EspState_t s_esp_state = ESP_STATE_IDLE;
static TaskHandle_t s_send_task_handle = NULL;

/* 私有函数声明 */
static void process_send_message(const SendMessage_t* message);
static bool send_esp_at_command(const char* command, const char* expected_response, uint32_t timeout_ms);
static bool send_rs485_data(const uint8_t* data, uint16_t length);
static bool wait_for_response(UART_ID_t uart_id, const char* expected, uint32_t timeout_ms);
static void send_heartbeat_data(void);
static void esp_init_sequence(void);

/**
 * @brief 通信发送任务主函数
 * @param pvParameters 任务参数（未使用）
 */
void vCommuSendTask(void* pvParameters)
{
    (void)pvParameters;
    SendMessage_t received_message;
    BaseType_t queue_result;
    
    LOG_INFO(LOG_MODULE_PROTOCOL, "Communication send task started");
    
    /* ESP模块初始化序列 */
    esp_init_sequence();
    
    /* 任务主循环 */
    while (1) {
        /* 等待发送队列中的消息 */
        queue_result = xQueueReceive(s_send_queue, &received_message, pdMS_TO_TICKS(1000));
        
        if (queue_result == pdTRUE) {
            /* 处理接收到的发送消息 */
            process_send_message(&received_message);
        } else {
            /* 超时处理，发送心跳数据 */
            send_heartbeat_data();
        }
    }
}

/**
 * @brief 初始化通信发送任务
 * @return BaseType_t 初始化结果
 */
BaseType_t CommuSend_Init(void)
{
    BaseType_t result = pdPASS;
    
    /* 创建发送队列 */
    s_send_queue = xQueueCreate(SEND_QUEUE_LENGTH, sizeof(SendMessage_t));
    if (s_send_queue == NULL) {
        LOG_ERROR(LOG_MODULE_PROTOCOL, "Failed to create send queue");
        return pdFAIL;
    }
    
    /* 创建互斥量 */
    s_send_mutex = xSemaphoreCreateMutex();
    if (s_send_mutex == NULL) {
        LOG_ERROR(LOG_MODULE_PROTOCOL, "Failed to create send mutex");
        vQueueDelete(s_send_queue);
        return pdFAIL;
    }
    
    /* 创建发送任务 */
    result = xTaskCreate(vCommuSendTask, "CommuSendTask", SEND_TASK_STACK_SIZE, 
                        NULL, SEND_TASK_PRIORITY, &s_send_task_handle);
    
    if (result != pdPASS) {
        LOG_ERROR(LOG_MODULE_PROTOCOL, "Failed to create send task");
        vQueueDelete(s_send_queue);
        vSemaphoreDelete(s_send_mutex);
        return pdFAIL;
    }
    
    LOG_INFO(LOG_MODULE_PROTOCOL, "Communication send task initialized");
    return pdPASS;
}

/**
 * @brief 发送ESP AT命令
 * @param command AT命令字符串
 * @param expected_response 期望的响应
 * @param timeout_ms 超时时间
 * @return BaseType_t 发送结果
 */
BaseType_t CommuSend_EspAtCommand(const char* command, const char* expected_response, uint32_t timeout_ms)
{
    SendMessage_t message;
    
    if (!command || strlen(command) >= sizeof(message.data)) {
        return pdFAIL;
    }
    
    /* 构建发送消息 */
    message.type = SEND_TYPE_ESP_AT;
    message.target_uart = UART_ID_LTE;  // ESP模块通常连接到LTE串口
    message.length = strlen(command);
    memcpy(message.data, command, message.length);
    message.timeout_ms = timeout_ms;
    message.wait_response = (expected_response != NULL);
    
    if (expected_response) {
        strncpy(message.expected_response, expected_response, sizeof(message.expected_response) - 1);
        message.expected_response[sizeof(message.expected_response) - 1] = '\0';
    }
    
    /* 发送到队列 */
    if (xQueueSend(s_send_queue, &message, pdMS_TO_TICKS(100)) != pdTRUE) {
        LOG_ERROR(LOG_MODULE_PROTOCOL, "Failed to queue ESP AT command");
        return pdFAIL;
    }
    
    return pdPASS;
}

/**
 * @brief 发送RS485数据
 * @param data 数据指针
 * @param length 数据长度
 * @return BaseType_t 发送结果
 */
BaseType_t CommuSend_Rs485Data(const uint8_t* data, uint16_t length)
{
    SendMessage_t message;
    
    if (!data || length == 0 || length > sizeof(message.data)) {
        return pdFAIL;
    }
    
    /* 构建发送消息 */
    message.type = SEND_TYPE_RS485_DATA;
    message.target_uart = UART_ID_RS485;
    message.length = length;
    memcpy(message.data, data, length);
    message.timeout_ms = SEND_TIMEOUT_MS;
    message.wait_response = false;
    
    /* 发送到队列 */
    if (xQueueSend(s_send_queue, &message, pdMS_TO_TICKS(100)) != pdTRUE) {
        LOG_ERROR(LOG_MODULE_PROTOCOL, "Failed to queue RS485 data");
        return pdFAIL;
    }
    
    return pdPASS;
}

/**
 * @brief 处理发送消息
 * @param message 发送消息指针
 */
static void process_send_message(const SendMessage_t* message)
{
    bool send_result = false;
    
    if (!message) {
        return;
    }
    
    /* 获取互斥量 */
    if (xSemaphoreTake(s_send_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        LOG_WARN(LOG_MODULE_PROTOCOL, "Failed to take send mutex");
        return;
    }
    
    switch (message->type) {
        case SEND_TYPE_ESP_AT:
            send_result = send_esp_at_command((const char*)message->data, 
                                            message->wait_response ? message->expected_response : NULL,
                                            message->timeout_ms);
            LOG_INFO(LOG_MODULE_PROTOCOL, "ESP AT command: %s, Result: %s", 
                    message->data, send_result ? "SUCCESS" : "FAILED");
            break;
            
        case SEND_TYPE_RS485_DATA:
            send_result = send_rs485_data(message->data, message->length);
            LOG_INFO(LOG_MODULE_PROTOCOL, "RS485 data sent (%d bytes), Result: %s", 
                    message->length, send_result ? "SUCCESS" : "FAILED");
            break;
            
        case SEND_TYPE_HEARTBEAT:
            send_heartbeat_data();
            send_result = true;
            break;
            
        default:
            LOG_WARN(LOG_MODULE_PROTOCOL, "Unknown send message type: %d", message->type);
            break;
    }
    
    /* 释放互斥量 */
    xSemaphoreGive(s_send_mutex);
}

/**
 * @brief 发送ESP AT命令
 * @param command AT命令
 * @param expected_response 期望响应
 * @param timeout_ms 超时时间
 * @return bool 发送结果
 */
static bool send_esp_at_command(const char* command, const char* expected_response, uint32_t timeout_ms)
{
    UART_Status_t uart_result;
    char at_command[256];
    bool result = false;
    
    /* 格式化AT命令（添加\r\n） */
    snprintf(at_command, sizeof(at_command), "%s\r\n", command);
    
    /* 发送AT命令 */
    uart_result = UART_RingBuffer_Send(UART_ID_LTE, (const uint8_t*)at_command, strlen(at_command), timeout_ms);
    
    if (uart_result == UART_OK) {
        LOG_DEBUG(LOG_MODULE_PROTOCOL, "AT command sent: %s", command);
        
        /* 如果需要等待响应 */
        if (expected_response) {
            result = wait_for_response(UART_ID_LTE, expected_response, timeout_ms);
        } else {
            result = true;
        }
    } else {
        LOG_ERROR(LOG_MODULE_PROTOCOL, "Failed to send AT command: %s", command);
    }
    
    return result;
}

/**
 * @brief 发送RS485数据
 * @param data 数据指针
 * @param length 数据长度
 * @return bool 发送结果
 */
static bool send_rs485_data(const uint8_t* data, uint16_t length)
{
    UART_Status_t result;
    
    /* 发送数据 */
    result = UART_RingBuffer_Send(UART_ID_RS485, data, length, SEND_TIMEOUT_MS);
    
    if (result == UART_OK) {
        LOG_DEBUG(LOG_MODULE_PROTOCOL, "RS485 data sent successfully");
        return true;
    } else {
        LOG_ERROR(LOG_MODULE_PROTOCOL, "Failed to send RS485 data");
        return false;
    }
}

/**
 * @brief 等待串口响应
 * @param uart_id 串口ID
 * @param expected 期望的响应字符串
 * @param timeout_ms 超时时间
 * @return bool 是否收到期望响应
 */
static bool wait_for_response(UART_ID_t uart_id, const char* expected, uint32_t timeout_ms)
{
    uint8_t rx_buffer[256];
    uint32_t start_time = xTaskGetTickCount();
    uint32_t elapsed_time;
    
    while (1) {
        /* 检查超时 */
        elapsed_time = (xTaskGetTickCount() - start_time) * portTICK_PERIOD_MS;
        if (elapsed_time >= timeout_ms) {
            LOG_WARN(LOG_MODULE_PROTOCOL, "Response timeout waiting for: %s", expected);
            return false;
        }
        
        /* 检查是否有可用数据 */
        uint32_t available = UART_RingBuffer_GetAvailableBytes(uart_id);
        if (available > 0) {
            uint16_t bytes_to_read = (available > sizeof(rx_buffer)) ? sizeof(rx_buffer) : available;
            
            UART_Status_t result = UART_RingBuffer_Receive(uart_id, rx_buffer, bytes_to_read, 100);
            if (result == UART_OK) {
                /* 添加字符串结束符 */
                rx_buffer[bytes_to_read] = '\0';
                
                /* 检查是否包含期望的响应 */
                if (strstr((const char*)rx_buffer, expected) != NULL) {
                    LOG_DEBUG(LOG_MODULE_PROTOCOL, "Received expected response: %s", expected);
                    return true;
                }
            }
        }
        
        /* 短暂延时避免占用太多CPU */
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

/**
 * @brief 发送心跳数据
 */
static void send_heartbeat_data(void)
{
    static uint32_t heartbeat_counter = 0;
    
    /* 每10次心跳发送一次ESP状态查询 */
    if ((heartbeat_counter % 10) == 0) {
        CommuSend_EspAtCommand("AT", "OK", 2000);
    }
    
    heartbeat_counter++;
}

/**
 * @brief ESP模块初始化序列
 */
static void esp_init_sequence(void)
{
    LOG_INFO(LOG_MODULE_PROTOCOL, "Starting ESP initialization sequence");
    
    /* 延时等待ESP模块启动 */
    vTaskDelay(pdMS_TO_TICKS(2000));
    
    /* 基本AT命令测试 */
    if (send_esp_at_command("AT", "OK", 3000)) {
        LOG_INFO(LOG_MODULE_PROTOCOL, "ESP module responded to AT command");
        s_esp_state = ESP_STATE_IDLE;
        
        /* 获取版本信息 */
        send_esp_at_command("AT+GMR", "OK", 3000);
        
        /* 设置为Station模式 */
        send_esp_at_command("AT+CWMODE=1", "OK", 3000);
        
    } else {
        LOG_ERROR(LOG_MODULE_PROTOCOL, "ESP module not responding");
        s_esp_state = ESP_STATE_ERROR;
    }
}

/**
 * @brief 获取ESP状态
 * @return EspState_t ESP当前状态
 */
EspState_t CommuSend_GetEspState(void)
{
    return s_esp_state;
}

/**
 * @brief 连接WiFi网络
 * @param ssid WiFi名称
 * @param password WiFi密码
 * @return BaseType_t 连接结果
 */
BaseType_t CommuSend_ConnectWiFi(const char* ssid, const char* password)
{
    char command[128];
    
    if (!ssid || !password) {
        return pdFAIL;
    }
    
    s_esp_state = ESP_STATE_CONNECTING;
    
    /* 格式化连接命令 */
    snprintf(command, sizeof(command), "AT+CWJAP=\"%s\",\"%s\"", ssid, password);
    
    /* 发送连接命令 */
    if (CommuSend_EspAtCommand(command, "WIFI CONNECTED", 10000) == pdPASS) {
        s_esp_state = ESP_STATE_CONNECTED;
        LOG_INFO(LOG_MODULE_PROTOCOL, "WiFi connected successfully");
        return pdPASS;
    } else {
        s_esp_state = ESP_STATE_ERROR;
        LOG_ERROR(LOG_MODULE_PROTOCOL, "WiFi connection failed");
        return pdFAIL;
    }
}
