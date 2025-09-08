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
#include "tsk_4g_init.h"

#include <string.h>
#include <stdio.h>

/* 任务参数 */
#define SEND_TASK_STACK_SIZE        (configMINIMAL_STACK_SIZE * 4)  // 增加栈大小
#define SEND_TASK_PRIORITY          (tskIDLE_PRIORITY + 3)
#define SEND_QUEUE_LENGTH           20
#define SEND_TIMEOUT_MS             1000
#define AT_RESPONSE_TIMEOUT_MS      5000

/* 发送消息类型 */
typedef enum {
    SEND_TYPE_4G_AT = 0,        // 4G AT命令
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

/* 4G模块状态 */
typedef enum {
    MODEM_STATE_IDLE = 0,
    MODEM_STATE_CONNECTING,
    MODEM_STATE_CONNECTED,
    MODEM_STATE_SENDING,
    MODEM_STATE_ERROR
} ModemState_t;

/* 静态变量 */
static QueueHandle_t s_send_queue = NULL;
static SemaphoreHandle_t s_send_mutex = NULL;
static ModemState_t s_modem_state = MODEM_STATE_IDLE;
static TaskHandle_t s_send_task_handle = NULL;

/* 私有函数声明 */
static void process_send_message(const SendMessage_t* message);
static bool send_4g_at_command(const char* command, const char* expected_response, uint32_t timeout_ms);
static bool send_rs485_data(const uint8_t* data, uint16_t length);
static bool wait_for_response(UART_ID_t uart_id, const char* expected, uint32_t timeout_ms);
static void send_heartbeat_data(void);

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
    
    /* 初始化消息接收缓冲区 */
    memset(&received_message, 0, sizeof(received_message));
    
    /* 等待4G模块初始化完成 */
    LOG_INFO(LOG_MODULE_PROTOCOL, "Waiting for 4G module initialization...");
    if (Quectel4G_WaitForReady(60000)) {  // 等待最多60秒
        LOG_INFO(LOG_MODULE_PROTOCOL, "4G module ready, send task operational");
        s_modem_state = MODEM_STATE_IDLE;
    } else {
        LOG_ERROR(LOG_MODULE_PROTOCOL, "4G module initialization timeout");
        s_modem_state = MODEM_STATE_ERROR;
    }
    
    /* 任务主循环 */
    while (1) {
        
        /* 清空消息缓冲区，防止垃圾数据 */
        memset(&received_message, 0, sizeof(received_message));
        
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
 * @brief 发送4G AT命令
 * @param cmd_config AT命令配置结构体
 * @return BaseType_t 发送结果
 */
BaseType_t CommuSend_4gAtCommand(const AT_Cmd_Config_t* cmd_config)
{
    SendMessage_t message;
    
    if (!cmd_config || !cmd_config->at_cmd || strlen(cmd_config->at_cmd) >= sizeof(message.data)) {
        return pdFAIL;
    }
    
    /* 初始化消息结构体，清除垃圾数据 */
    memset(&message, 0, sizeof(message));
    
    /* 构建发送消息 */
    message.type = SEND_TYPE_4G_AT;
    message.target_uart = UART_ID_LTE;  // 4G模块连接到LTE串口
    message.length = strlen(cmd_config->at_cmd);
    memcpy(message.data, cmd_config->at_cmd, message.length);
    message.data[message.length] = '\0';  // 确保字符串结束
    message.timeout_ms = cmd_config->timeout_ms;
    message.wait_response = (cmd_config->expected_resp != NULL);
    
    if (cmd_config->expected_resp) {
        strncpy(message.expected_response, cmd_config->expected_resp, sizeof(message.expected_response) - 1);
        message.expected_response[sizeof(message.expected_response) - 1] = '\0';
    }
    
    /* 发送到队列 */
    if (xQueueSend(s_send_queue, &message, pdMS_TO_TICKS(100)) != pdTRUE) {
        LOG_ERROR(LOG_MODULE_PROTOCOL, "Failed to queue 4G AT command");
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
    
    /* 初始化消息结构体，清除垃圾数据 */
    memset(&message, 0, sizeof(message));
    
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
 * @brief 统一的UART数据发送接口
 * @param uart_id 串口ID
 * @param data 要发送的数据
 * @param length 数据长度
 * @param timeout_ms 超时时间（毫秒）
 * @return BaseType_t 发送结果 (pdPASS/pdFAIL)
 */
BaseType_t CommuSend_UartData(UART_ID_t uart_id, const uint8_t* data, uint32_t length, uint32_t timeout_ms)
{
    UART_Status_t uart_result;
    
    if (uart_id == UART_ID_LTE) {
        // LTE串口直接发送，绕过LOG到LTE的转发机制
        // 临时禁用LOG到LTE转发，避免冲突
        bool log_forward_was_enabled = UART_RingBuffer_IsLogToLteForwardEnabled();
        if (log_forward_was_enabled) {
            UART_RingBuffer_SetLogToLteForward(false);
        }
        
        // 直接发送到LTE串口
        uart_result = UART_RingBuffer_Send(uart_id, data, length, timeout_ms);
        
        // 恢复LOG到LTE转发状态
        if (log_forward_was_enabled) {
            UART_RingBuffer_SetLogToLteForward(true);
        }
        
        LOG_DEBUG(LOG_MODULE_PROTOCOL, "LTE direct send result: %s, data: %.*s", 
                 uart_result == UART_OK ? "SUCCESS" : "FAILED", 
                 (int)length, (const char*)data);
    } else {
        // 其他串口正常发送，包括LOG→LTE转发功能
        uart_result = UART_RingBuffer_Send(uart_id, data, length, timeout_ms);
    }
    
    return (uart_result == UART_OK) ? pdPASS : pdFAIL;
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
        case SEND_TYPE_4G_AT:
            send_result = send_4g_at_command((const char*)message->data, 
                                            message->wait_response ? message->expected_response : NULL,
                                            message->timeout_ms);
            LOG_INFO(LOG_MODULE_PROTOCOL, "4G AT command: %s, Result: %s", 
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
 * @brief 发送4G AT命令
 * @param command AT命令
 * @param expected_response 期望响应
 * @param timeout_ms 超时时间
 * @return bool 发送结果
 */
static bool send_4g_at_command(const char* command, const char* expected_response, uint32_t timeout_ms)
{
    UART_Status_t uart_result;
    char at_command[256];
    bool result = false;
    
    /* 格式化AT命令（添加\r\n） */
    snprintf(at_command, sizeof(at_command), "%s\r\n", command);
    
    /* 发送AT命令 */
    uart_result = CommuSend_UartData(UART_ID_LTE, (const uint8_t*)at_command, strlen(at_command), timeout_ms) == pdPASS ? UART_OK : UART_ERR_TIMEOUT;
    
    if (uart_result == UART_OK) {
        LOG_DEBUG(LOG_MODULE_PROTOCOL, "4G AT command sent: %s", command);
        
        /* 如果需要等待响应 */
        if (expected_response) {
            result = wait_for_response(UART_ID_LTE, expected_response, timeout_ms);
        } else {
            result = true;
        }
    } else {
        LOG_ERROR(LOG_MODULE_PROTOCOL, "Failed to send 4G AT command: %s", command);
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
    /* 发送数据 */
    BaseType_t send_result = CommuSend_UartData(UART_ID_RS485, data, length, SEND_TIMEOUT_MS);
    
    if (send_result == pdPASS) {
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

    /* 每1000次心跳发送一次4G状态查询 */
    if ((heartbeat_counter % 1000) == 0) {
        /* 检查4G模块状态 */
        QuectelState_t state = Quectel4G_GetState();
        if (state >= QUECTEL_STATE_READY) {
            static const AT_Cmd_Config_t at_cmd = {"AT", "OK", 2000, 1, NULL, false, NULL};
            CommuSend_4gAtCommand(&at_cmd);
        }
    }
    
    heartbeat_counter++;
}

/**
 * @brief 获取4G模块状态
 * @return ModemState_t 4G模块当前状态
 */
ModemState_t CommuSend_GetModemState(void)
{
    return s_modem_state;
}

