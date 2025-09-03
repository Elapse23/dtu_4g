/**
 * =====================================================================================
 * @file        tsk_commu_receive.c
 * @brief       通信接收任务实现
 * @author      23Elapse & GitHub Copilot
 * @version     1.0
 * @date        2025-09-03
 * @note        处理串口通信、数据解析和响应
 * =====================================================================================
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "bsp_uart_ring_buffer.h"
#include "bsp_log_manager.h"
#include "tsk_commu_send.h"
#include <string.h>
#include <stdio.h>

/* 任务参数 */
#define COMMU_TASK_STACK_SIZE       (configMINIMAL_STACK_SIZE * 2)
#define COMMU_TASK_PRIORITY         (tskIDLE_PRIORITY + 4)
#define COMMU_PROCESS_DELAY_MS      50

/* 接收缓冲区大小 */
#define RX_BUFFER_SIZE              256
#define MAX_MESSAGE_LENGTH          128

/* 通信处理状态 */
typedef enum {
    COMMU_STATE_IDLE = 0,
    COMMU_STATE_RECEIVING,
    COMMU_STATE_PROCESSING,
    COMMU_STATE_RESPONDING
} CommuState_t;

/* 消息类型定义 */
typedef struct {
    uint8_t data[MAX_MESSAGE_LENGTH];
    uint16_t length;
    UART_ID_t source_uart;
} CommuMessage_t;

/* 静态变量 */
static CommuState_t s_commu_state = COMMU_STATE_IDLE;
static uint8_t s_rx_buffer[RX_BUFFER_SIZE];
static CommuMessage_t s_current_message;
static TaskHandle_t s_receive_task_handle = NULL;

/* 私有函数声明 */
static void process_uart_data(UART_ID_t uart_id);
static void handle_received_message(CommuMessage_t* message);
static void send_response(UART_ID_t uart_id, const char* response);
static BaseType_t parse_command(const uint8_t* data, uint16_t length, char* response, uint16_t max_response_len);

/* 前向声明 */
void vCommuReceiveTask(void* pvParameters);

/**
 * @brief 初始化通信接收任务
 * @return BaseType_t 初始化结果
 */
BaseType_t CommuReceive_Init(void)
{
    BaseType_t result;
    
    /* 创建通信接收处理任务 */
    result = xTaskCreate(vCommuReceiveTask, "CommuReceiveTask", 
                        COMMU_TASK_STACK_SIZE, NULL, 
                        COMMU_TASK_PRIORITY, &s_receive_task_handle);
    
    if (result != pdPASS) {
        LOG_ERROR(LOG_MODULE_PROTOCOL, "Failed to create communication receive task");
        return pdFAIL;
    }
    
    LOG_INFO(LOG_MODULE_PROTOCOL, "Communication receive task initialized");
    return pdPASS;
}

/**
 * @brief 通信接收任务主函数
 * @param pvParameters 任务参数（未使用）
 */
void vCommuReceiveTask(void* pvParameters)
{
    (void)pvParameters;
    
    LOG_INFO(LOG_MODULE_PROTOCOL, "Communication receive task started");
    
    /* 任务主循环 */
    while (1) {
        /* 处理RS485串口数据 */
        process_uart_data(UART_ID_RS485);
        
        /* 处理LTE串口数据 */
        process_uart_data(UART_ID_LTE);
        
        /* 任务延时，避免占用过多CPU */
        vTaskDelay(pdMS_TO_TICKS(COMMU_PROCESS_DELAY_MS));
    }
}

/**
 * @brief 处理指定串口的接收数据
 * @param uart_id 串口ID
 */
static void process_uart_data(UART_ID_t uart_id)
{
    UART_Status_t result;
    uint32_t available_bytes;
    uint16_t bytes_to_read;
    
    /* 检查是否有可用数据 */
    available_bytes = UART_RingBuffer_GetAvailableBytes(uart_id);
    
    if (available_bytes > 0) {
        /* 限制读取长度 */
        bytes_to_read = (available_bytes > sizeof(s_rx_buffer)) ? sizeof(s_rx_buffer) : available_bytes;
        
        /* 接收数据 */
        result = UART_RingBuffer_Receive(uart_id, s_rx_buffer, bytes_to_read, 10);
        
        if (result == UART_OK) {
            /* 构建消息结构 */
            if (bytes_to_read <= MAX_MESSAGE_LENGTH) {
                memcpy(s_current_message.data, s_rx_buffer, bytes_to_read);
                s_current_message.length = bytes_to_read;
                s_current_message.source_uart = uart_id;
                
                /* 处理接收到的消息 */
                handle_received_message(&s_current_message);
                
                LOG_DEBUG(LOG_MODULE_PROTOCOL, "Processed %d bytes from UART%d", bytes_to_read, uart_id);
            } else {
                LOG_WARN(LOG_MODULE_PROTOCOL, "Message too long (%d bytes) from UART%d", bytes_to_read, uart_id);
            }
        }
    }
}

/**
 * @brief 处理接收到的消息
 * @param message 消息指针
 */
static void handle_received_message(CommuMessage_t* message)
{
    char response[MAX_MESSAGE_LENGTH];
    BaseType_t command_handled = pdFALSE;
    
    if (!message || message->length == 0) {
        return;
    }
    
    s_commu_state = COMMU_STATE_PROCESSING;
    
    /* 尝试解析命令并生成响应 */
    command_handled = parse_command(message->data, message->length, response, sizeof(response));
    
    if (command_handled == pdTRUE) {
        s_commu_state = COMMU_STATE_RESPONDING;
        send_response(message->source_uart, response);
        LOG_INFO(LOG_MODULE_PROTOCOL, "Command processed and response sent via UART%d", message->source_uart);
    } else {
        /* 未识别的命令，记录日志 */
        LOG_WARN(LOG_MODULE_PROTOCOL, "Unknown command received from UART%d", message->source_uart);
    }
    
    s_commu_state = COMMU_STATE_IDLE;
}

/**
 * @brief 发送响应数据
 * @param uart_id 目标串口ID
 * @param response 响应字符串
 */
static void send_response(UART_ID_t uart_id, const char* response)
{
    UART_Status_t result;
    uint16_t response_len = strlen(response);
    
    if (response_len > 0) {
        result = UART_RingBuffer_Send(uart_id, (const uint8_t*)response, response_len, 100);
        
        if (result != UART_OK) {
            LOG_ERROR(LOG_MODULE_PROTOCOL, "Failed to send response via UART%d", uart_id);
        }
    }
}

/**
 * @brief 解析接收到的命令并生成响应
 * @param data 接收到的数据
 * @param length 数据长度
 * @param response 响应缓冲区
 * @param max_response_len 响应缓冲区最大长度
 * @return pdTRUE 如果命令被识别和处理，pdFALSE 否则
 */
static BaseType_t parse_command(const uint8_t* data, uint16_t length, char* response, uint16_t max_response_len)
{
    /* 确保数据以null结尾用于字符串操作 */
    char command_buffer[MAX_MESSAGE_LENGTH + 1];
    
    if (length >= sizeof(command_buffer)) {
        return pdFALSE;
    }
    
    memcpy(command_buffer, data, length);
    command_buffer[length] = '\0';
    
    /* 基本命令解析 */
    if (strncmp(command_buffer, "AT", 2) == 0) {
        /* AT命令响应 */
        snprintf(response, max_response_len, "OK\r\n");
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "STATUS", 6) == 0) {
        /* 状态查询命令 */
        EspState_t esp_state = CommuSend_GetEspState();
        snprintf(response, max_response_len, "STATUS:READY,STATE:%d,ESP_STATE:%d\r\n", 
                (int)s_commu_state, (int)esp_state);
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "VERSION", 7) == 0) {
        /* 版本查询命令 */
        snprintf(response, max_response_len, "VERSION:DTU_4G_V1.0\r\n");
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "RESET", 5) == 0) {
        /* 重置命令 */
        snprintf(response, max_response_len, "RESET:OK\r\n");
        /* 发送ESP重置命令 */
        CommuSend_EspAtCommand("AT+RST", "ready", 5000);
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "PING", 4) == 0) {
        /* PING命令 */
        snprintf(response, max_response_len, "PONG\r\n");
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "WIFI_CONNECT", 12) == 0) {
        /* WiFi连接命令格式: WIFI_CONNECT,ssid,password */
        char* ssid = strchr(command_buffer, ',');
        if (ssid) {
            ssid++; // 跳过逗号
            char* password = strchr(ssid, ',');
            if (password) {
                *password = '\0'; // 终止ssid字符串
                password++; // 跳过逗号
                
                /* 异步发送WiFi连接命令 */
                CommuSend_ConnectWiFi(ssid, password);
                snprintf(response, max_response_len, "WIFI_CONNECTING\r\n");
                return pdTRUE;
            }
        }
        snprintf(response, max_response_len, "WIFI_CONNECT_ERROR:INVALID_FORMAT\r\n");
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "ESP_CMD", 7) == 0) {
        /* ESP命令转发格式: ESP_CMD,AT+GMR */
        char* esp_cmd = strchr(command_buffer, ',');
        if (esp_cmd) {
            esp_cmd++; // 跳过逗号
            /* 转发ESP AT命令 */
            CommuSend_EspAtCommand(esp_cmd, "OK", 3000);
            snprintf(response, max_response_len, "ESP_CMD_SENT\r\n");
            return pdTRUE;
        }
        snprintf(response, max_response_len, "ESP_CMD_ERROR:INVALID_FORMAT\r\n");
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "RS485_SEND", 10) == 0) {
        /* RS485发送命令格式: RS485_SEND,data */
        char* rs485_data = strchr(command_buffer, ',');
        if (rs485_data) {
            rs485_data++; // 跳过逗号
            /* 发送RS485数据 */
            CommuSend_Rs485Data((const uint8_t*)rs485_data, strlen(rs485_data));
            snprintf(response, max_response_len, "RS485_SENT\r\n");
            return pdTRUE;
        }
        snprintf(response, max_response_len, "RS485_SEND_ERROR:INVALID_FORMAT\r\n");
        return pdTRUE;
    }
    
    /* 命令未识别 */
    return pdFALSE;
}

/**
 * @brief 获取通信接收任务当前状态
 * @return 当前状态
 */
CommuState_t CommuReceive_GetState(void)
{
    return s_commu_state;
}
