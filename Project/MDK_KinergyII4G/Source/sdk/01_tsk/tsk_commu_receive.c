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
#include "tsk_commu_receive.h"
#include "lte_module_init.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* 内部函数声明 */
static bool lte_receive_data_to_mcu(const uint8_t* data, size_t length);



/* 静态变量 */
static CommuState_t s_commu_state = COMMU_STATE_IDLE;
static uint8_t s_rx_buffer[RX_BUFFER_SIZE];
static CommuMessage_t s_current_message;
static TaskHandle_t s_receive_task_handle = NULL;

/* LTE数据监控标志 */
/* LTE数据监控标志 - 控制是否将LTE发送的数据镜像到LOG串口 */
bool s_lte_monitor_enabled = false;




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
    available_bytes = uart_get_available_bytes(uart_id);
    
    /* 启用LTE串口调试日志 */
    if (uart_id == UART_ID_LTE && available_bytes > 0) {
        LOG_DEBUG(LOG_MODULE_PROTOCOL, "LTE UART%d available bytes: %d", uart_id, available_bytes);
    }
    
    if (available_bytes > 0) {
        /* 限制读取长度 */
        bytes_to_read = (available_bytes > sizeof(s_rx_buffer)) ? sizeof(s_rx_buffer) : available_bytes;
        
        /* 接收数据 */
        result = uart_receive(uart_id, s_rx_buffer, bytes_to_read, 10);
        
        /* 对于LTE串口，进行特殊处理 */
        if (uart_id == UART_ID_LTE && result == UART_OK) {
            LOG_DEBUG(LOG_MODULE_PROTOCOL, "LTE shared data content: %.*s", bytes_to_read, s_rx_buffer);
            
            /* 使用统一接口进行数据路由 */
            bool routing_success = lte_receive_data_to_mcu(s_rx_buffer, bytes_to_read);
            
            if (!routing_success) {
                LOG_ERROR(LOG_MODULE_PROTOCOL, "Failed to route shared LTE data via unified interface (%d bytes)", bytes_to_read);
            }
        }
        
        if (result == UART_OK) {
            /* 根据串口类型进行不同处理 */
            if (uart_id == UART_ID_RS485 || uart_id == UART_ID_LOG) 
            {
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
    uint16_t response_len = strlen(response);
    
    if (response_len > 0) {
        UART_Status_t uart_result = uart_send(uart_id, (const uint8_t*)response, response_len, 100);
        
        if (uart_result != UART_OK) {
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
        LteState_t module_state = lte_get_state();
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
        lte_soft_reset();
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "4G_STATUS", 9) == 0) {
        /* 4G模块状态查询 */
        LteState_t state = lte_get_state();
        bool init_status = get_lte_initialization_status();
        snprintf(response, max_response_len, "4G_STATE:%d,INIT_STATUS:%s\r\n", 
                (int)state, init_status ? "TRUE" : "FALSE");
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "4G_INIT", 7) == 0) {
        /* 强制4G模块初始化 */
        BaseType_t result = lte_reinitialize();
        if (result == pdPASS) {
            snprintf(response, max_response_len, "4G_INIT_STARTED\r\n");
        } else {
            snprintf(response, max_response_len, "4G_INIT_FAILED\r\n");
        }
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "LTE_CLEAR", 9) == 0) {
        /* 清空LTE缓冲区和统计数据 */
        uart_flush_rx(UART_ID_LTE);
        uart_flush_tx(UART_ID_LTE);
        snprintf(response, max_response_len, "LTE_BUFFERS_CLEARED\r\n");
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "4G_RAWTEST", 10) == 0) {
        /* 原始数据测试 - 发送不同的测试数据 */
        const uint8_t test_patterns[][8] = {
            {0x41, 0x54, 0x0D, 0x0A, 0x00},           // "AT\r\n"
            {0x41, 0x54, 0x45, 0x30, 0x0D, 0x0A, 0x00}, // "ATE0\r\n"
            {0x0D, 0x0A, 0x41, 0x54, 0x0D, 0x0A, 0x00}, // "\r\nAT\r\n"
        };
        
        for (int i = 0; i < 3; i++) {
            LOG_INFO(LOG_MODULE_PROTOCOL, "Sending test pattern %d", i);
            uart_send(UART_ID_LTE, test_patterns[i], strlen((char*)test_patterns[i]), 1000);
            vTaskDelay(pdMS_TO_TICKS(1000)); // 等待1秒
        }
        snprintf(response, max_response_len, "4G_RAWTEST_SENT\r\n");
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "4G_BAUD_TEST", 12) == 0) {
        /* 测试不同波特率的可能性 */
        LOG_INFO(LOG_MODULE_PROTOCOL, "Current LTE UART baud rate should be 115200");
        LOG_INFO(LOG_MODULE_PROTOCOL, "If module responds at different baud rate, we need to reconfigure");
        
        // 发送同步字符
        const uint8_t sync_pattern[] = {0x55, 0x55, 0x55, 0x41, 0x54, 0x0D, 0x0A}; // 同步 + AT
        uart_send(UART_ID_LTE, sync_pattern, sizeof(sync_pattern), 1000);
        
        snprintf(response, max_response_len, "4G_BAUD_TEST_SENT\r\n");
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "4G_MONITOR", 10) == 0) {
        /* 监控4G模块是否发送任何数据 */
        LOG_INFO(LOG_MODULE_PROTOCOL, "Starting 10-second monitoring of LTE UART...");
        
        uint32_t start_available = uart_get_available_bytes(UART_ID_LTE);
        
        // 等待10秒并检查是否有新数据
        for (int i = 0; i < 10; i++) {
            vTaskDelay(pdMS_TO_TICKS(1000));
            uint32_t current_available = uart_get_available_bytes(UART_ID_LTE);
            
            if (current_available > start_available) {
                LOG_INFO(LOG_MODULE_PROTOCOL, "Activity detected at second %d: AVAIL=%d", 
                         i+1, current_available);
                break;
            }
        }
        
        uint32_t final_available = uart_get_available_bytes(UART_ID_LTE);
        
        snprintf(response, max_response_len, "4G_MONITOR_DONE:AVAIL_CHANGE=%d,FINAL_AVAIL=%d\r\n", 
                 final_available - start_available, final_available);
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "LTE_MIRROR", 10) == 0) {
        /* 启用/禁用LTE数据镜像到LOG串口 */
        char* param = strchr(command_buffer, ',');
        if (param) {
            param++; // 跳过逗号
            if (strncmp(param, "ON", 2) == 0) {
                s_lte_monitor_enabled = true;
                snprintf(response, max_response_len, "LTE_MIRROR_ENABLED\r\n");
            } else if (strncmp(param, "OFF", 3) == 0) {
                s_lte_monitor_enabled = false;
                snprintf(response, max_response_len, "LTE_MIRROR_DISABLED\r\n");
            } else {
                snprintf(response, max_response_len, "LTE_MIRROR_INVALID_PARAM\r\n");
            }
        } else {
            snprintf(response, max_response_len, "LTE_MIRROR_STATUS:%s\r\n", 
                    s_lte_monitor_enabled ? "ON" : "OFF");
        }
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "TASK_INFO", 9) == 0) {
        /* FreeRTOS任务状态详细信息 */
        TaskStatus_t *pxTaskStatusArray;
        volatile UBaseType_t uxArraySize, x;
        uint32_t ulTotalRunTime;

        // 获取任务数量
        uxArraySize = uxTaskGetNumberOfTasks();
        
        // 分配内存来存储任务状态
        pxTaskStatusArray = pvPortMalloc(uxArraySize * sizeof(TaskStatus_t));
        
        if (pxTaskStatusArray != NULL) {
            // 生成任务状态快照
            uxArraySize = uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, &ulTotalRunTime);
            
            // 查找LteInit任务
            for (x = 0; x < uxArraySize; x++) {
                if (strstr(pxTaskStatusArray[x].pcTaskName, "LteInit") != NULL) {
                    snprintf(response, max_response_len, 
                            "LteInit Task - State:%d, Priority:%d, StackWaterMark:%d\r\n",
                            (int)pxTaskStatusArray[x].eCurrentState, 
                            (int)pxTaskStatusArray[x].uxCurrentPriority,
                            (int)pxTaskStatusArray[x].usStackHighWaterMark);
                    vPortFree(pxTaskStatusArray);
                    return pdTRUE;
                }
            }
            
            snprintf(response, max_response_len, "LteInit task not found, Total tasks:%d\r\n", (int)uxArraySize);
            vPortFree(pxTaskStatusArray);
        } else {
            snprintf(response, max_response_len, "Memory allocation failed\r\n");
        }
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "LTE_STATUS", 10) == 0) {
        /* LTE串口状态查询 */
        uint32_t available = uart_get_available_bytes(UART_ID_LTE);
        bool is_tx_busy = uart_is_tx_busy(UART_ID_LTE);
        
        snprintf(response, max_response_len, "LTE_AVAIL:%d,TX_BUSY:%s\r\n", 
                available, is_tx_busy ? "YES" : "NO");
        
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "LTE_READ", 8) == 0) {
        /* 直接从LTE缓冲区读取数据进行测试 - 仅在4G任务不活跃时使用 */
        {
            uint8_t test_buffer[64];
            uint32_t available = uart_get_available_bytes(UART_ID_LTE);
            
            if (available > 0) {
                uint16_t bytes_to_read = (available > sizeof(test_buffer)) ? sizeof(test_buffer) : available;
                UART_Status_t result = uart_receive(UART_ID_LTE, test_buffer, bytes_to_read, 100);
                
                if (result == UART_OK) {
                    snprintf(response, max_response_len, "LTE_READ_SUCCESS:%d bytes,[", bytes_to_read);
                    
                    // 显示前8个字节的十六进制
                    for (uint16_t i = 0; i < bytes_to_read && i < 8; i++) {
                        char hex_str[4];
                        snprintf(hex_str, sizeof(hex_str), "%02X ", test_buffer[i]);
                        strcat(response, hex_str);
                    }
                    strcat(response, "] ASCII:[");
                    
                    // 显示ASCII
                    for (uint16_t i = 0; i < bytes_to_read && i < 8; i++) {
                        if (test_buffer[i] >= 32 && test_buffer[i] <= 126) {
                            char ascii_str[2] = {test_buffer[i], '\0'};
                            strcat(response, ascii_str);
                        } else {
                            strcat(response, ".");
                        }
                    }
                    strcat(response, "]\r\n");
                } else {
                    snprintf(response, max_response_len, "LTE_READ_FAILED:%d\r\n", result);
                }
            } else {
                snprintf(response, max_response_len, "LTE_READ_EMPTY\r\n");
            }

        } 
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "UART_STATUS", 11) == 0) {
        /* 检查所有UART状态 */
        LOG_INFO(LOG_MODULE_PROTOCOL, "Checking all UART status");
        
        uint32_t lte_available = uart_get_available_bytes(UART_ID_LTE);
        uint32_t rs485_available = uart_get_available_bytes(UART_ID_RS485);
        uint32_t log_available = uart_get_available_bytes(UART_ID_LOG);
        
        snprintf(response, max_response_len, "UART_STATUS:LTE=%d,RS485=%d,LOG=%d\r\n",
                 (int)lte_available, (int)rs485_available, (int)log_available);
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "BLE_CMD", 7) == 0) {
        /* BLE指令转发格式: BLE_CMD,AT+BLE... */
        char* ble_cmd = strchr(command_buffer, ',');
        if (ble_cmd) {
            ble_cmd++; // 跳过逗号
            LOG_INFO(LOG_MODULE_PROTOCOL, "Extracted BLE command: [%s]", ble_cmd);
            
            LOG_INFO(LOG_MODULE_PROTOCOL, "Executing BLE command: %s", ble_cmd);
            
            UART_Status_t result = uart_send(UART_ID_LTE, (const uint8_t*)ble_cmd, strlen(ble_cmd), 3000);
            
            if (result == UART_OK) {
                snprintf(response, max_response_len, "BLE_CMD_SENT_OK\r\n");
            } else {
                snprintf(response, max_response_len, "BLE_CMD_SEND_ERROR\r\n");
            }
            return pdTRUE;
        }
        snprintf(response, max_response_len, "BLE_CMD_ERROR:INVALID_FORMAT\r\n");
        return pdTRUE;
    }
    else if (strncmp(command_buffer, "4G_CMD", 6) == 0) {
        /* 4G命令转发格式: 4G_CMD,AT+GMR */
        LOG_INFO(LOG_MODULE_PROTOCOL, "Processing 4G_CMD command");
        char* at_cmd = strchr(command_buffer, ',');
        if (at_cmd) {
            at_cmd++; // 跳过逗号
            /* 构建完整的AT命令（添加\r\n） */
            static char full_at_cmd[256];
            if (strstr(at_cmd, "\r\n") == NULL) 
            {
                snprintf(full_at_cmd, sizeof(full_at_cmd), "%s\r\n", at_cmd);
            } 
            else 
            {
                strncpy(full_at_cmd, at_cmd, sizeof(full_at_cmd) - 1);
                full_at_cmd[sizeof(full_at_cmd) - 1] = '\0';
            }
            size_t full_cmd_len = strlen(full_at_cmd);

            AT_Cmd_Config_t cmd_config = {
                .module_type = MODULE_TYPE_4G,
                .at_cmd = full_at_cmd,  // 使用带结束符的完整命令
                .expected_resp = "OK",
                .timeout_ms = 2000,
                .retries = 0,
                .critical = false,
                .callback = NULL
            };
            
            LOG_INFO(LOG_MODULE_PROTOCOL, "Executing AT command via 4G module: %s (timeout: %d ms, len: %d)", 
                     full_at_cmd, cmd_config.timeout_ms, strlen(full_at_cmd));
            
            /* 记录开始时间 */
            TickType_t start_tick = xTaskGetTickCount();
            
            /* 执行4G AT命令 */
            AT_Result_t result = at_exec(&cmd_config);
            
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
            uart_send(UART_ID_RS485, (const uint8_t*)rs485_data, strlen(rs485_data), 1000);
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
CommuState_t commu_receive_get_state(void)
{
    return s_commu_state;
}



/**
 * @brief 检查是否为4G模块响应
 * @param data 数据缓冲区
 * @param length 数据长度
 * @return bool 是否为4G响应
 */
static bool is_4g_response(const uint8_t* data, size_t length)
{
    if (!data || length == 0) return false;
    
    const char* data_str = (const char*)data;
    
    /* 4G模块特有响应标识 */
    const char* g4_indicators[] = {
        "+QIURC:",      // Lte URC通知
        "+QIOPEN:",     // Socket连接状态
        "+QICLOSE:",    // Socket关闭
        "+QIRD:",       // 数据读取
        "+QISEND:",     // 数据发送状态
        "+CME ERROR:",  // GSM错误
        "+CMS ERROR:",  // SMS错误
        "+CREG:",       // 网络注册状态
        "+CGREG:",      // GPRS注册状态
        "+CSQ:",        // 信号质量
        "+COPS:",       // 运营商信息
        "+QCCID:",      // ICCID响应
        "RDY",          // 模块就绪
        "+CFUN:",       // 功能模式
        NULL
    };
    
    for (int i = 0; g4_indicators[i] != NULL; i++) {
        if (strstr(data_str, g4_indicators[i]) != NULL) {
            return true;
        }
    }
    
    return false;
}

/**
 * @brief 检查是否为BLE模块响应
 * @param data 数据缓冲区
 * @param length 数据长度
 * @return bool 是否为BLE响应
 */
static bool is_ble_response(const uint8_t* data, size_t length)
{
    if (!data || length == 0) return false;
    
    const char* data_str = (const char*)data;
    
    /* BLE模块特有响应标识 */
    const char* ble_indicators[] = {
        "+BLE",         // BLE相关响应
        "+WRITE:",      // BLE写入事件
        "+NOTIFY:",     // BLE通知事件
        "+CONN:",       // BLE连接事件
        "+DISC:",       // BLE断开事件
        "+CWMODE:",     // WiFi模式响应
        "+CWJAP:",      // WiFi连接响应
        "+CWLAP:",      // WiFi扫描响应
        "+CIPSTART:",   // TCP连接响应
        "+CIPSTATUS:",  // TCP状态响应
        "+IPD,",        // 接收到TCP数据
        "WIFI ",        // ESP模块WiFi状态
        "busy p...",    // ESP忙碌状态
        "SEND OK",      // ESP发送完成
        "SEND FAIL",    // ESP发送失败
        "ALREADY CONNECT", // 已连接
        NULL
    };
    
    for (int i = 0; ble_indicators[i] != NULL; i++) {
        if (strstr(data_str, ble_indicators[i]) != NULL) {
            return true;
        }
    }
    
    return false;
}

/**
 * @brief 识别数据来源（4G或BLE模块）
 * @param data 数据缓冲区
 * @param length 数据长度
 * @return Data_Source_t 数据来源
 */
static Data_Source_t identify_data_source(const uint8_t* data, size_t length)
{
    if (!data || length == 0) {
        return DATA_SOURCE_UNKNOWN;
    }
    const char* data_str = (const char*)data;
    
    /* 检查4G模块响应特征 */
    if (is_4g_response(data, length)) {
        LOG_DEBUG(LOG_MODULE_PROTOCOL, "Identified as 4G data: %.*s", (int)length, data_str);
        return DATA_SOURCE_4G;
    }

    /* 检查BLE模块响应特征 */
    if (is_ble_response(data, length)) {
        LOG_DEBUG(LOG_MODULE_PROTOCOL, "Identified as BLE data: %.*s", (int)length, data_str);
        return DATA_SOURCE_BLE;
    }

    return DATA_SOURCE_4G;  // 默认路由到4G模块

}

/**
 * @brief 改进的数据路由函数 - 使用统一接口
 * @param data 数据缓冲区
 * @param length 数据长度
 * @return bool 路由结果
 */
static bool lte_receive_data_to_mcu(const uint8_t* data, size_t length)
{
    bool result = false;
    /* 识别数据来源 */
    Data_Source_t data_source = identify_data_source(data, length);
    ModuleType_t module_type;
    
    /* 将Data_Source_t转换为ModuleType_t */
    switch (data_source) {
        case DATA_SOURCE_4G:
            module_type = MODULE_TYPE_4G;
            break;
        case DATA_SOURCE_BLE:
            module_type = MODULE_TYPE_BLE;
            break;
        default:
            module_type = MODULE_TYPE_4G;  // 默认为4G
            break;
    }
    
    switch (module_type) {
        case MODULE_TYPE_4G:
        case MODULE_TYPE_BLE:
            if (lte_send_data_to_mcu(module_type, data, length)) 
            {
                result = true;
            }
            break;            
        default:
            LOG_WARN(LOG_MODULE_PROTOCOL, "No handler for module type %d", module_type);
            break;
    }
    return result;

}

static bool s_lte_module_initialized = false; 

/**
 * @brief 通信接收任务主函数
 * @param pvParameters 任务参数（未使用）
 */
void vCommuReceiveTask(void* pvParameters)
{
    (void)pvParameters;
    LOG_INFO(LOG_MODULE_PROTOCOL, "Communication receive task started");
//     if (!s_lte_module_initialized) {
//         LOG_INFO(LOG_MODULE_NETWORK, "Initializing LTE module hardware...");
//         lte_4g_module_hardware_open();
// //        lte_4g_module_hardware_reset();
//         s_lte_module_initialized = true;
//         LOG_INFO(LOG_MODULE_NETWORK, "LTE module hardware initialized successfully");
//     }
    /* 任务主循环 */
    while (1) {
        /* 喂狗 - 防止看门狗复位 */
        
        /* 处理RS485串口数据 */
        // process_uart_data(UART_ID_RS485);
        
        // /* 处理LOG串口数据（用于调试命令） */
        process_uart_data(UART_ID_LOG);
        
        /* 处理LTE串口数据 */
        // if(get_lte_initialization_status())
        // {
        //     process_uart_data(UART_ID_LTE);
        // }

        
        /* 任务延时，避免占用过多CPU */
        vTaskDelay(pdMS_TO_TICKS(COMMU_PROCESS_DELAY_MS));
    }
}
/**
 * @brief 初始化通信接收任务
 * @return BaseType_t 初始化结果
 */
BaseType_t commu_receive_init(void)
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
