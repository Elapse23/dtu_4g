/**
 * =====================================================================================
 * @file        tsk_commu_receive.c
 * @brief       通信接收任务实现
 * @author      23Elapse & GitHub Copilot
 * @version     1.0
 * @date        2025-09-03
 * @note        处理串口通信、数据解析和响应
 * 
 * 支持的调试命令 (可通过LOG串口或RS485串口发送):
 * - AT            : 基本AT测试命令
 * - STATUS        : 查询系统状态
 * - VERSION       : 查询版本信息
 * - RESET         : 软重置4G模块
 * - PING          : 连通性测试
 * - LTE_TEST      : 向LTE模块发送AT测试命令
 * - LTE_STATUS    : 查询LTE串口缓冲区状态
 * - 4G_CONNECT,ip,port : 连接4G TCP服务器
 * - 4G_CMD,AT+XXX : 转发4G AT命令
 * - RS485_SEND,data : 发送RS485数据
 * 
 * 注意：通过LOG串口发送命令时，响应会通过日志系统输出，格式为 [CMD_RESPONSE] xxx
 * =====================================================================================
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "bsp_uart_ring_buffer.h"
#include "bsp_log_manager.h"
#include "tsk_commu_send.h"
#include "tsk_4g_init.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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
static uint32_t s_lte_debug_counter = 0;  /* LTE调试计数器 */

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
        /* 喂狗 - 防止看门狗复位 */

        
        // /* 定期输出LTE串口状态调试信息 */
        // s_lte_debug_counter++;
        // if (s_lte_debug_counter >= 100) {  /* 每5秒输出一次状态 (50ms * 100) */
        //     s_lte_debug_counter = 0;
        //     uint32_t lte_available = UART_RingBuffer_GetAvailableBytes(UART_ID_LTE);
        //     LOG_INFO(LOG_MODULE_PROTOCOL, "LTE UART status check - Available bytes: %d", lte_available);
        // }
        
        /* 处理RS485串口数据 */
        process_uart_data(UART_ID_RS485);
        
        /* 处理LOG串口数据（用于调试命令） */
        process_uart_data(UART_ID_LOG);
        
        /* 处理LTE串口数据 */
        process_uart_data(UART_ID_LTE);
        
        /* 任务延时，避免占用过多CPU */
        vTaskDelay(pdMS_TO_TICKS(COMMU_PROCESS_DELAY_MS));
    }
}

/**
 * @brief 处理指定串口的接收数据
 * @details 根据串口类型进行不同的数据处理：
 *          - RS485: 直接处理命令和协议数据
 *          - LTE: 转发给4G模块数据处理队列
 * 
 * @param uart_id 串口ID
 */
static void process_uart_data(UART_ID_t uart_id)
{
    UART_Status_t result;
    uint32_t available_bytes;
    uint16_t bytes_to_read;
    
    /* 检查是否有可用数据 */
    available_bytes = UART_RingBuffer_GetAvailableBytes(uart_id);
    
    /* 启用LTE串口调试日志 */
    if (uart_id == UART_ID_LTE && available_bytes > 0) {
        LOG_DEBUG(LOG_MODULE_PROTOCOL, "LTE UART%d available bytes: %d", uart_id, available_bytes);
    }
    
    if (available_bytes > 0) {
        /* 限制读取长度 */
        bytes_to_read = (available_bytes > sizeof(s_rx_buffer)) ? sizeof(s_rx_buffer) : available_bytes;
        
        /* 接收数据 */
        result = UART_RingBuffer_Receive(uart_id, s_rx_buffer, bytes_to_read, 10);
        
        if (result == UART_OK) {
            /* 为调试命令显示接收到的内容（仅显示LOG和RS485串口的命令） */
            if (uart_id == UART_ID_LOG || uart_id == UART_ID_RS485) {
                LOG_INFO(LOG_MODULE_PROTOCOL, "CMD received from UART%d: [%.*s]", uart_id, bytes_to_read, s_rx_buffer);
            }
            
            /* 根据串口类型进行不同处理 */
            if (uart_id == UART_ID_LTE) {
                /* LTE串口数据：转发给4G数据处理队列 */
                LOG_DEBUG(LOG_MODULE_PROTOCOL, "LTE data content: %.*s", bytes_to_read, s_rx_buffer);
                
                if (!Quectel4G_SendDataToQueue(s_rx_buffer, bytes_to_read)) {
                    LOG_WARN(LOG_MODULE_PROTOCOL, "Failed to forward LTE data to 4G queue (%d bytes)", bytes_to_read);
                } else {
                    LOG_DEBUG(LOG_MODULE_PROTOCOL, "Forwarded %d bytes from LTE to 4G queue", bytes_to_read);
                }
            } else if (uart_id == UART_ID_RS485 || uart_id == UART_ID_LOG) {
                /* RS485和LOG串口：本地处理命令 */
                if (bytes_to_read <= MAX_MESSAGE_LENGTH) {
                    memcpy(s_current_message.data, s_rx_buffer, bytes_to_read);
                    s_current_message.length = bytes_to_read;
                    s_current_message.source_uart = uart_id;
                    
                    /* 处理接收到的消息 */
                    handle_received_message(&s_current_message);
                } else {
                    LOG_WARN(LOG_MODULE_PROTOCOL, "Message too long (%d bytes) from UART%d", bytes_to_read, uart_id);
                }
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
        LOG_ERROR(LOG_MODULE_PROTOCOL, "Invalid message received (NULL or empty)");
        return;
    }
    
    s_commu_state = COMMU_STATE_PROCESSING;
    
    LOG_INFO(LOG_MODULE_PROTOCOL, "Processing command from UART%d: [%.*s]", 
             message->source_uart, message->length, message->data);
    
    /* 尝试解析命令并生成响应 */
    command_handled = parse_command(message->data, message->length, response, sizeof(response));
    
    if (command_handled == pdTRUE) {
        s_commu_state = COMMU_STATE_RESPONDING;
        
        /* 根据来源串口选择响应方式 */
        if (message->source_uart == UART_ID_LOG) {
            /* LOG串口接收的命令：通过日志系统输出响应，避免与日志输出冲突 */
            LOG_INFO(LOG_MODULE_PROTOCOL, "[CMD_RESPONSE] %s", response);
        } else {
            /* 其他串口：正常发送响应 */
            send_response(message->source_uart, response);
        }
        
        LOG_INFO(LOG_MODULE_PROTOCOL, "Command processed successfully via UART%d", message->source_uart);
    } else {
        /* 未识别的命令，记录日志 */
        LOG_WARN(LOG_MODULE_PROTOCOL, "Unknown command received from UART%d: [%.*s]", 
                 message->source_uart, message->length, message->data);
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
    BaseType_t result;
    uint16_t response_len = strlen(response);
    
    if (response_len > 0) {
        result = CommuSend_UartData(uart_id, (const uint8_t*)response, response_len, 100);
        
        if (result != pdPASS) {
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
        QuectelState_t module_state = Quectel4G_GetState();
        snprintf(response, max_response_len, "STATUS:READY,STATE:%d,4G_STATE:%d\r\n", 
                (int)s_commu_state, (int)module_state);
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
        /* 发送4G模块软重置命令 */
        Quectel4G_SoftReset();
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "PING", 4) == 0) {
        /* PING命令 */
        snprintf(response, max_response_len, "PONG\r\n");
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "LTE_TEST", 8) == 0) {
        /* LTE串口测试命令 - 发送AT指令到LTE模块 */
        // 检查是否启用了LOG到LTE转发
        if (UART_RingBuffer_IsLogToLteForwardEnabled()) {
            snprintf(response, max_response_len, "LTE_TEST_INFO:FORWARD_MODE_ACTIVE,USE_AT_DIRECTLY\r\n");
            return pdTRUE;
        }
        
        const char* test_cmd = "AT";
        AT_Cmd_Config_t at_cfg = {test_cmd, NULL, 1000, 0, "LTE Test", false, NULL};
        BaseType_t result = CommuSend_4gAtCommand(&at_cfg);
        if (result == pdPASS) {
            snprintf(response, max_response_len, "LTE_TEST_SENT\r\n");
        } else {
            snprintf(response, max_response_len, "LTE_TEST_ERROR:%d\r\n", result);
        }
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "LTE_STATUS", 10) == 0) {
        /* LTE串口状态查询 */
        uint32_t available = UART_RingBuffer_GetAvailableBytes(UART_ID_LTE);
        snprintf(response, max_response_len, "LTE_AVAILABLE:%d\r\n", available);
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "UART_STATUS", 11) == 0) {
        /* 检查所有UART状态 */
        LOG_INFO(LOG_MODULE_PROTOCOL, "Checking all UART status");
        
        uint32_t lte_available = UART_RingBuffer_GetAvailableBytes(UART_ID_LTE);
        uint32_t rs485_available = UART_RingBuffer_GetAvailableBytes(UART_ID_RS485);
        uint32_t log_available = UART_RingBuffer_GetAvailableBytes(UART_ID_LOG);
        
        snprintf(response, max_response_len, "UART_STATUS:LTE=%d,RS485=%d,LOG=%d\r\n",
                 (int)lte_available, (int)rs485_available, (int)log_available);
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "FORWARD_ENABLE", 14) == 0) {
        /* 启用LOG<->LTE转发功能 */
        UART_RingBuffer_SetLogToLteForward(true);
        UART_RingBuffer_SetLteToLogForward(true);
        snprintf(response, max_response_len, "FORWARD_ENABLED\r\n");
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "FORWARD_DISABLE", 15) == 0) {
        /* 禁用LOG<->LTE转发功能 */
        UART_RingBuffer_SetLogToLteForward(false);
        UART_RingBuffer_SetLteToLogForward(false);
        snprintf(response, max_response_len, "FORWARD_DISABLED\r\n");
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "FORWARD_STATUS", 14) == 0) {
        /* 查询转发功能状态 */
        bool log_to_lte = UART_RingBuffer_IsLogToLteForwardEnabled();
        bool lte_to_log = UART_RingBuffer_IsLteToLogForwardEnabled();
        snprintf(response, max_response_len, "FORWARD_STATUS:LOG_TO_LTE=%s,LTE_TO_LOG=%s\r\n",
                 log_to_lte ? "ON" : "OFF", lte_to_log ? "ON" : "OFF");
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "CMD_CONFLICTS", 13) == 0) {
        /* 检查命令冲突状态 */
        bool forward_enabled = UART_RingBuffer_IsLogToLteForwardEnabled();
        snprintf(response, max_response_len, 
                 "CMD_CONFLICTS:FORWARD=%s,NOTE=%s\r\n",
                 forward_enabled ? "ON" : "OFF",
                 forward_enabled ? "USE_AT_DIRECTLY" : "USE_4G_CMD_OR_LTE_TEST");
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "4G_CONNECT", 10) == 0) {
        /* 4G连接命令格式: 4G_CONNECT,server_ip,server_port */
        char* server_ip = strchr(command_buffer, ',');
        if (server_ip) {
            server_ip++; // 跳过逗号
            char* server_port_str = strchr(server_ip, ',');
            if (server_port_str) {
                *server_port_str = '\0'; // 终止server_ip字符串
                server_port_str++; // 跳过逗号
                
                uint16_t server_port = (uint16_t)atoi(server_port_str);
                
                /* 连接4G TCP服务器 */
                QuectelDataResult_t result = Quectel4G_ConnectTCP(server_ip, server_port, 0);
                if (result == QUECTEL_DATA_OK) {
                    snprintf(response, max_response_len, "4G_CONNECTING\r\n");
                } else {
                    snprintf(response, max_response_len, "4G_CONNECT_ERROR:%d\r\n", result);
                }
                return pdTRUE;
            }
        }
        snprintf(response, max_response_len, "4G_CONNECT_ERROR:INVALID_FORMAT\r\n");
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "4G_CMD", 6) == 0) {
        /* 4G命令转发格式: 4G_CMD,AT+GMR */
        LOG_INFO(LOG_MODULE_PROTOCOL, "Processing 4G_CMD command");
        
        // 检查是否启用了LOG到LTE转发，如果启用则建议直接使用AT命令
        if (UART_RingBuffer_IsLogToLteForwardEnabled()) {
            char* at_cmd = strchr(command_buffer, ',');
            if (at_cmd) {
                at_cmd++; // 跳过逗号
                snprintf(response, max_response_len, "4G_CMD_INFO:FORWARD_MODE_ACTIVE,TRY:[%s]\r\n", at_cmd);
            } else {
                snprintf(response, max_response_len, "4G_CMD_INFO:FORWARD_MODE_ACTIVE,USE_AT_DIRECTLY\r\n");
            }
            return pdTRUE;
        }
        
        char* at_cmd = strchr(command_buffer, ',');
        if (at_cmd) {
            at_cmd++; // 跳过逗号
            LOG_INFO(LOG_MODULE_PROTOCOL, "Extracted AT command: [%s]", at_cmd);
            
            /* 检查是否是简单的非阻塞命令 */
            if (strncmp(at_cmd, "AT+GMR", 6) == 0 || strncmp(at_cmd, "AT+CIMI", 7) == 0) {
                /* 对于查询类命令，使用简化的发送方式 */
                LOG_INFO(LOG_MODULE_PROTOCOL, "Using simplified send for query command: %s", at_cmd);
                
                /* 直接发送命令，不等待响应 */
                AT_Cmd_Config_t at_cfg = {at_cmd, NULL, 1000, 0, "Query Command", false, NULL};
                BaseType_t uart_result = CommuSend_4gAtCommand(&at_cfg);
                
                if (uart_result == pdPASS) {
                    LOG_INFO(LOG_MODULE_PROTOCOL, "AT command sent successfully: %s", at_cmd);
                    snprintf(response, max_response_len, "4G_CMD_SENT_OK\r\n");
                } else {
                    LOG_ERROR(LOG_MODULE_PROTOCOL, "Failed to send AT command: %s", at_cmd);
                    snprintf(response, max_response_len, "4G_CMD_SEND_ERROR\r\n");
                }
                
                return pdTRUE;
            }
            
            /* 其他命令使用标准流程 */
            /* 构建AT命令配置，使用较短的超时时间防止死锁 */
            AT_Cmd_Config_t cmd_config = {at_cmd, "OK", 2000, 0, "User AT Command", false, NULL};
            
            LOG_INFO(LOG_MODULE_PROTOCOL, "Executing AT command via 4G module: %s (timeout: %d ms)", 
                     at_cmd, cmd_config.timeout_ms);
            
            /* 记录开始时间 */
            TickType_t start_tick = xTaskGetTickCount();
            
            /* 执行4G AT命令 */
            AT_Result_t result = Quectel4G_ExecuteAtCommand(&cmd_config);
            
            /* 计算执行时间 */
            TickType_t end_tick = xTaskGetTickCount();
            uint32_t execution_time = (end_tick - start_tick) * portTICK_PERIOD_MS;
            
            LOG_INFO(LOG_MODULE_PROTOCOL, "4G AT command completed: result=%d, time=%d ms", 
                     result, execution_time);
            
            if (result == AT_RESULT_OK) {
                snprintf(response, max_response_len, "4G_CMD_OK\r\n");
            } else {
                snprintf(response, max_response_len, "4G_CMD_ERROR:%d\r\n", result);
            }
            return pdTRUE;
        }
        LOG_WARN(LOG_MODULE_PROTOCOL, "4G_CMD: Invalid format, missing comma");
        snprintf(response, max_response_len, "4G_CMD_ERROR:INVALID_FORMAT\r\n");
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
