/**
 * =====================================================================================
 * @file        tsk_4g_init.c
 * @brief       移远4G模块初始化任务实现
 * @author      23Elapse & GitHub Copilot
 * @version     1.0
 * @date        2025-09-03
 * @note        负责移远4G模块的初始化、配置和状态管理
 * 
 * @architecture_note
 * 数据接收架构说明：
 * 1. 4G数据接收任务在模块初始化时即创建，而不是等到TCP连接成功后
 * 2. 这样确保初始化期间的AT指令响应能够正确接收和处理
 * 3. LTE串口数据流：tsk_commu_receive → 4G数据队列 → 4G数据接收任务
 * 4. 数据接收任务持续运行，处理AT指令响应、Socket数据和模块事件
 * 5. TCP连接断开时不停止数据接收任务，保证重连时的通信畅通
 * =====================================================================================
 */

#include "tsk_4g_init.h"
#include "lte_module_init.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "bsp_uart_ring_buffer.h"
#include "bsp_log_manager.h"
#include "tsk_commu_receive.h"
#include "tsk_commu_send.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* ============================= 全局变量定义 ============================= */
/**
 * @brief 全局4G设备信息实例
 * @note 这是系统的核心数据结构，所有设备状态信息都存储在这里
 *       在简化的4G网络连接流程中，主要在初始化阶段使用
 */
Lte_Device_Info_t g_Lte_info = {0};

/* ============================= 静态变量定义 ============================= */

/* 任务句柄 */
static TaskHandle_t s_init_task_handle = NULL;         /**< 初始化任务句柄 */
static TaskHandle_t s_data_receive_task_handle = NULL; /**< 数据接收任务句柄 */

/* 状态和配置 */
static LteState_t s_current_state = LTE_STATE_UNKNOWN;  /**< 当前模块状态 */
static LteInitConfig_t s_init_config = {0};                 /**< 初始化配置参数 */
static bool s_init_completed = false;                           /**< 初始化完成标志 */

/* 重启计数和硬件状态 */
static uint8_t s_hard_reset_count = 0;                 /**< 硬件重启计数器 */
static uint8_t s_soft_reset_count = 0;                 /**< 软件重启计数器 */
static bool s_lte_module_hardware_initialized = false; /**< LTE模块硬件初始化标志 */

/* 队列句柄 */
static QueueHandle_t s_lte_receive_queue = NULL;        /**< 4G AT响应队列 */

/* 互斥量 */
static SemaphoreHandle_t s_lte_access_mutex = NULL;    /**< LTE串口访问互斥量，用于与commu_receiver任务互斥 */

/* ============================= 静态函数声明 ============================= */
static bool get_network_status(void);
static void at_callback_basic_test(const char* resp, AT_Result_t result);
static void at_callback_get_version(const char* resp, AT_Result_t result);
static void at_callback_get_imei(const char* resp, AT_Result_t result);
static void at_callback_get_iccid(const char* resp, AT_Result_t result);
static void at_callback_get_imsi(const char* resp, AT_Result_t result);
static void at_callback_check_sim(const char* resp, AT_Result_t result);
static void at_callback_signal_quality(const char* resp, AT_Result_t result);
static void at_callback_network_reg(const char* resp, AT_Result_t result);
static void at_callback_operator_info(const char* resp, AT_Result_t result);

/* ============================= 函数实现 ============================= */


/**
 * @brief 监控网络状态
 */
static void monitor_network_status(void)
{
    (void)get_network_status();
}

/**
 * @brief 获取网络状态信息
 * @return bool 是否成功获取
 */
static bool get_network_status(void)
{
    /* 定义网络状态查询指令序列 */
    const AT_Cmd_Config_t status_sequence[] = {
        /* 获取信号质量 */
        {
            .module_type = MODULE_TYPE_4G,
            .at_cmd = "AT+CSQ",
            .expected_resp = "OK",
            .timeout_ms = 3000,
            .retries = 1,
            .critical = false,
            .callback = at_callback_signal_quality
        },
        /* 获取运营商信息 */
        {
            .module_type = MODULE_TYPE_4G,
            .at_cmd = "AT+COPS?",
            .expected_resp = "OK",
            .timeout_ms = 5000,
            .retries = 1,
            .critical = false,
            .callback = at_callback_operator_info
        },
        /* 检查网络注册状态 */
        {
            .module_type = MODULE_TYPE_4G,
            .at_cmd = "AT+CREG?",
            .expected_resp = "OK",
            .timeout_ms = 3000,
            .retries = 1,
            .critical = false,
            .callback = at_callback_network_reg
        },
        /* 检查数据连接状态 */
        {
            .module_type = MODULE_TYPE_4G,
            .at_cmd = "AT+QIACT?",
            .expected_resp = "OK",
            .timeout_ms = 3000,
            .retries = 1,
            .critical = false,
            .callback = NULL
        }
    };
    
    const uint8_t status_cmd_count = sizeof(status_sequence) / sizeof(AT_Cmd_Config_t);
    
    /* 执行状态查询序列 */
    return Lte_ExecuteAtSequence(status_sequence, status_cmd_count);
}

/**
 * @brief 更新状态
 * @param new_state 新状态
 * @note 状态更新简化为直接赋值，因为枚举赋值在ARM架构下通常是原子操作
 */
static void update_state(LteState_t new_state)
{
    s_current_state = new_state;
    LOG_DEBUG(LOG_MODULE_NETWORK, "4G state updated to: %d", new_state);
}

/**
 * @brief 获取4G模块当前状态
 * @return LteState_t 当前状态
 * @note 状态读取简化为直接读取，因为枚举读取在ARM架构下通常是原子操作
 */
LteState_t Lte_GetState(void)
{
    return s_current_state;
}

/**
 * @brief 检查4G模块是否就绪
 * @return bool 是否就绪
 */
bool Lte_IsReady(void)
{
    return (Lte_GetState() >= LTE_STATE_READY);
}

/**
 * @brief 等待4G模块初始化完成
 * @param timeout_ms 等待超时时间
 * @return bool 是否在超时前完成初始化
 */
bool Lte_WaitForReady(uint32_t timeout_ms)
{
    uint32_t start_time = xTaskGetTickCount();
    uint32_t elapsed_time;
    
    while (1) {
        if (Lte_IsReady()) {
            return true;
        }
        
        elapsed_time = (xTaskGetTickCount() - start_time) * portTICK_PERIOD_MS;
        if (elapsed_time >= timeout_ms) {
            return false;
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

/**
 * @brief 获取默认初始化配置
 * @return 默认配置
 */
LteInitConfig_t Lte_GetDefaultConfig(void)
{
    LteInitConfig_t config = {
        .init_timeout_ms = 60000,           // 60秒初始化超时
        .at_response_timeout_ms = 5000,     // 5秒AT响应超时
        .max_retry_count = 3,               // 最多重试3次
        .auto_register = true,              // 自动注册网络
        .enable_echo = false,               // 禁用回显
        .apn = "3gnet",                     // 默认APN
        .username = "",                     // 无用户名
        .password = "",                     // 无密码
    };
    
    return config;
}

/**
 * @brief 强制重新初始化4G模块
 * @return BaseType_t 操作结果
 */
BaseType_t Lte_Reinitialize(void)
{
    s_init_completed = false;
    update_state(LTE_STATE_UNKNOWN);
    
    /* 触发重新初始化 */
    if (s_init_task_handle) {
        /* 通过设置错误状态来触发重新初始化 */
        update_state(LTE_STATE_ERROR);
        return pdPASS;
    }
    
    return pdFAIL;
}

/**
 * @brief 获取AT指令响应数据 - 直接从串口获取，与commu_receiver任务互斥
 * @details 使用互斥量确保与commu_receiver任务对串口的互斥访问，
 *          直接从串口缓冲区读取AT指令响应数据
 * 
 * @param response_buffer 响应数据缓冲区
 * @param buffer_size 缓冲区大小
 * @param timeout_ms 超时时间（毫秒）
 * @return bool 是否成功获取数据
 */
static bool get_response_data(char* response_buffer, size_t buffer_size, uint32_t timeout_ms)
{
    bool result = false;
    TickType_t start_tick = xTaskGetTickCount();
    TickType_t timeout_ticks = pdMS_TO_TICKS(timeout_ms);
    
    /* 参数有效性检查 */
    if (!response_buffer || buffer_size == 0) {
        LOG_ERROR(LOG_MODULE_NETWORK, "Invalid parameters for get_response_data");
        return false;
    }
    
    /* 检查互斥量是否已初始化 */
    if (!s_lte_access_mutex) {
        LOG_ERROR(LOG_MODULE_NETWORK, "LTE access mutex not initialized");
        return false;
    }
    
    /* 清空响应缓冲区 */
    memset(response_buffer, 0, buffer_size);
    
    /* 临时绕过互斥量检查，直接读取数据 */
    bool mutex_acquired = false;
    if (s_lte_access_mutex) {
        /* 获取LTE串口访问互斥量，确保与commu_receiver任务互斥 */
        if (xSemaphoreTake(s_lte_access_mutex, timeout_ticks) == pdTRUE) {
            mutex_acquired = true;
            LOG_DEBUG(LOG_MODULE_NETWORK, "Acquired LTE mutex, reading response from UART (timeout: %dms)", timeout_ms);
        } else {
            LOG_ERROR(LOG_MODULE_NETWORK, "Failed to acquire LTE access mutex for response reading");
        }
    } else {
        LOG_WARN(LOG_MODULE_NETWORK, "LTE access mutex not initialized, proceeding without mutex");
    }
    
    /* 主要读取循环 - 直接从串口读取数据 */
    while ((xTaskGetTickCount() - start_tick) < timeout_ticks) {
        
        /* 检查串口是否有可用数据 */
        uint32_t available_bytes = UART_RingBuffer_GetAvailableBytes(UART_ID_LTE);
        
        if (available_bytes > 0) {
            /* 限制读取长度 */
            uint16_t bytes_to_read = (available_bytes > (buffer_size - 1)) ? 
                                    (buffer_size - 1) : available_bytes;
            
            /* 直接从串口环形缓冲区读取数据 */
            UART_Status_t uart_result = UART_RingBuffer_Receive(UART_ID_LTE, 
                                                                (uint8_t*)response_buffer, 
                                                                bytes_to_read, 
                                                                100);
            
            if (uart_result == UART_OK && bytes_to_read > 0) {
                /* 确保字符串结束 */
                response_buffer[bytes_to_read] = '\0';
                
                LOG_INFO(LOG_MODULE_NETWORK, "LTE response received from UART: [%.*s] (%d bytes)", 
                        (int)bytes_to_read, response_buffer, bytes_to_read);
                
                result = true;
                break;
            }
        }
        
        /* 短暂延时避免过度消耗CPU */
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    
    /* 释放LTE串口访问互斥量 */
    if (mutex_acquired) {
        xSemaphoreGive(s_lte_access_mutex);
    }
    
    /* 记录结果 */
    uint32_t elapsed_ms = (xTaskGetTickCount() - start_tick) * portTICK_PERIOD_MS;
    if (result) {
        LOG_INFO(LOG_MODULE_NETWORK, "Successfully received LTE response in %dms", elapsed_ms);
    } else {
        LOG_WARN(LOG_MODULE_NETWORK, "LTE response timeout after %dms", elapsed_ms);
    }
    
    return result;
}

/**
 * @brief 获取LTE串口访问互斥量 - 供其他任务使用
 * @details 确保多个任务对LTE串口访问的同步，防止数据竞争
 * 
 * @param timeout_ms 获取超时时间（毫秒）
 * @return bool 是否成功获取互斥量
 */
bool AcquireLteAccessMutex(uint32_t timeout_ms)
{
    if (!s_lte_access_mutex) {
        LOG_ERROR(LOG_MODULE_NETWORK, "LTE access mutex not initialized");
        return false;
    }
    
    TickType_t timeout_ticks = (timeout_ms == portMAX_DELAY) ? 
                              portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    
    BaseType_t result = xSemaphoreTake(s_lte_access_mutex, timeout_ticks);
    return (result == pdTRUE);
}

/**
 * @brief 释放LTE串口访问互斥量
 */
void ReleaseLteAccessMutex(void)
{
    if (s_lte_access_mutex) {
        xSemaphoreGive(s_lte_access_mutex);
    }
}

/**
 * @brief 执行AT指令配置
 * @details 此函数支持其他模块调用，用于执行AT命令
 * 
 * @param cmd_config AT指令配置
 * @return AT_Result_t 执行结果
 */
AT_Result_t execute_at_command_with_config(const AT_Cmd_Config_t* cmd_config)
{
    char at_command[256];
    char response_buffer[512];
    AT_Result_t result = AT_RESULT_ERROR;
    uint8_t retry_count = 0;
    
    if (!cmd_config) {
        LOG_ERROR(LOG_MODULE_NETWORK, "AT command config is NULL");
        return AT_RESULT_ERROR;
    }
    /* 特殊处理：仅等待响应，不发送指令 */
    if (cmd_config->at_cmd == NULL || strlen(cmd_config->at_cmd) == 0) {
        LOG_DEBUG(LOG_MODULE_NETWORK, "Waiting for module response: %s (%s)", 
                 cmd_config->expected_resp ? cmd_config->expected_resp : "Any", 
                 cmd_config->description ? cmd_config->description : "No description");
        /* 仅等待响应，重试循环 */
        while (retry_count <= cmd_config->retries) {
            /* 获取响应 */
            if (get_response_data(response_buffer, sizeof(response_buffer), cmd_config->timeout_ms)) {
                LOG_INFO(LOG_MODULE_NETWORK, "Wait response received: [%s]", response_buffer);
                /* 检查期望响应 */
                if (cmd_config->expected_resp) {
                    if (strstr(response_buffer, cmd_config->expected_resp) != NULL) {
                        result = AT_RESULT_OK;
                        LOG_INFO(LOG_MODULE_NETWORK, "Successfully received expected response: %s", cmd_config->expected_resp);
                        
                        /* 特殊处理：RDY信号后需要额外延时让模块完全准备好 */
                        if (strstr(cmd_config->expected_resp, "RDY") != NULL) {
                            LOG_INFO(LOG_MODULE_NETWORK, "RDY signal received, waiting for module to be fully ready...");
                            vTaskDelay(pdMS_TO_TICKS(1000));  // RDY后延时1秒
                        }
                        
                        break;
                    } else {
                        result = AT_RESULT_TIMEOUT;
                        LOG_WARN(LOG_MODULE_NETWORK, "Unexpected response while waiting for: %s", cmd_config->expected_resp);
                    }
                } else {
                    /* 不检查响应内容，有数据即认为成功 */
                    result = AT_RESULT_OK;
                    break;
                }
            } else {
                result = AT_RESULT_TIMEOUT;
                LOG_WARN(LOG_MODULE_NETWORK, "Timeout waiting for response: %s", 
                        cmd_config->expected_resp ? cmd_config->expected_resp : "Any");
            }
            
            retry_count++;
            if (retry_count <= cmd_config->retries) {
                LOG_INFO(LOG_MODULE_NETWORK, "Retrying wait for response (%d/%d): %s", 
                        retry_count, cmd_config->retries, 
                        cmd_config->expected_resp ? cmd_config->expected_resp : "Any");
                vTaskDelay(pdMS_TO_TICKS(100));  // 重试前延时
            }
        }
        
        /* 如果重试次数用尽 */
        if (retry_count > cmd_config->retries && result != AT_RESULT_OK) {
            result = AT_RESULT_RETRY_EXHAUSTED;
            LOG_ERROR(LOG_MODULE_NETWORK, "Wait for response retry exhausted: %s", 
                     cmd_config->expected_resp ? cmd_config->expected_resp : "Any");
        }
        
        /* 调用回调函数 */
        if (cmd_config->callback) {
            cmd_config->callback(response_buffer, result);
        }
        
        return result;
    }
    
    LOG_INFO(LOG_MODULE_NETWORK, "Executing AT command: [%s] (%s)", 
            cmd_config->at_cmd, cmd_config->description ? cmd_config->description : "No description");
    
    /* 重试循环 */
    while (retry_count <= cmd_config->retries) {
        // snprintf(at_command, sizeof(at_command), "%s\r\n", cmd_config->at_cmd);
        /* 调试：检查AT指令内容是否正常 */
        size_t cmd_len = strlen(cmd_config->at_cmd);
        LOG_DEBUG(LOG_MODULE_NETWORK, "About to send AT command: [%s], len=%d", 
                cmd_config->at_cmd, cmd_len);
        
        if (cmd_len >= 2) {
            LOG_DEBUG(LOG_MODULE_NETWORK, "Last 2 bytes of AT command: 0x%02X 0x%02X", 
                    (unsigned char)cmd_config->at_cmd[cmd_len-2],
                    (unsigned char)cmd_config->at_cmd[cmd_len-1]);
        }
        
        /* 清空LTE发送缓冲区，避免重试时数据累积 */
        UART_RingBuffer_FlushTx(UART_ID_LTE);
        
        /* 清空LTE接收缓冲区，确保接收到的是当前命令的响应 */
        UART_RingBuffer_FlushRx(UART_ID_LTE);
        
        /* 发送命令 */
        BaseType_t send_result = CommuSend_UartData(UART_ID_LTE, (const uint8_t*)cmd_config->at_cmd, 
                                                    strlen(cmd_config->at_cmd), cmd_config->timeout_ms);        
        if (send_result != pdPASS) {
            LOG_ERROR(LOG_MODULE_NETWORK, "Failed to send AT command (result: %d): %s", send_result, cmd_config->at_cmd);
            retry_count++;
            continue;
        }
        
        LOG_DEBUG(LOG_MODULE_NETWORK, "AT command sent successfully, waiting for response...");
        
        /* 获取响应 */
        if (get_response_data(response_buffer, sizeof(response_buffer), cmd_config->timeout_ms)) {
            LOG_INFO(LOG_MODULE_NETWORK, "AT command response received: [%s]", response_buffer);
            
            /* 检查期望响应 */
            if (cmd_config->expected_resp) {
                if (strstr(response_buffer, cmd_config->expected_resp) != NULL) {
                    result = AT_RESULT_OK;
                    LOG_INFO(LOG_MODULE_NETWORK, "AT command successful: %s", cmd_config->at_cmd);
                    break;
                } else if (strstr(response_buffer, "ERROR") != NULL) {
                    result = AT_RESULT_ERROR;
                    LOG_WARN(LOG_MODULE_NETWORK, "AT command returned ERROR: %s", cmd_config->at_cmd);
                } else {
                    result = AT_RESULT_TIMEOUT;
                    LOG_WARN(LOG_MODULE_NETWORK, "AT command unexpected response: %s", cmd_config->at_cmd);
                }
            } else {
                /* 不检查响应，认为成功 */
                result = AT_RESULT_OK;
                LOG_INFO(LOG_MODULE_NETWORK, "AT command completed (no response check): %s", cmd_config->at_cmd);
                break;
            }
        } else {
            result = AT_RESULT_TIMEOUT;
            // LOG_WARN(LOG_MODULE_NETWORK, "AT command timeout (no response): %s", cmd_config->at_cmd);
            
            /* 添加紧急退出机制：如果是用户命令，不要无限重试 */
            if (strstr(cmd_config->description, "User") != NULL) {
                LOG_WARN(LOG_MODULE_NETWORK, "User command timeout, breaking retry loop");
                break;
            }
        }
        
        retry_count++;
        if (retry_count <= cmd_config->retries) {
            LOG_INFO(LOG_MODULE_NETWORK, "Retrying AT command (%d/%d): %s", 
                    retry_count, cmd_config->retries, cmd_config->at_cmd);
            vTaskDelay(pdMS_TO_TICKS(1000));  // 重试前延时
        }
    }
    
    /* 如果重试次数用尽 */
    if (retry_count > cmd_config->retries && result != AT_RESULT_OK) {
        result = AT_RESULT_RETRY_EXHAUSTED;
        // LOG_ERROR(LOG_MODULE_NETWORK, "AT command retry exhausted: %s", cmd_config->at_cmd);
    }
    
    // LOG_INFO(LOG_MODULE_NETWORK, "AT command execution completed: [%s], result: %d", cmd_config->at_cmd, result);
    
    /* 调用回调函数 */
    if (cmd_config->callback) {
        cmd_config->callback(response_buffer, result);
    }
    
    return result;
}

/**
 * @brief 执行单个AT指令
 * @param cmd_config AT指令配置
 * @return AT_Result_t 执行结果
 */
AT_Result_t Lte_ExecuteAtCommand(const AT_Cmd_Config_t* cmd_config)
{
    if (!cmd_config) {
        return AT_RESULT_ERROR;
    }

    return execute_at_command_with_config(cmd_config);
}

/**
 * @brief 执行AT指令序列
 * @details 按顺序执行一系列AT指令，支持关键指令和非关键指令的区分处理。
 *          关键指令失败时会立即停止执行并触发模块重启，
 *          非关键指令失败时会记录错误但继续执行后续指令。
 * 
 * @param cmd_sequence AT指令序列数组指针
 * @param count 指令数量
 * @return bool true-所有关键指令执行成功，false-存在关键指令执行失败
 * 
 * @note 函数会统计成功和失败的指令数量，便于调试和监控
 * @note 每个指令的执行结果都会记录到日志中
 */
bool Lte_ExecuteAtSequence(const AT_Cmd_Config_t* cmd_sequence, uint8_t count)
{
    uint8_t i;
    AT_Result_t result;
    bool all_success = true;
    
    if (!cmd_sequence || count == 0) {
        return false;
    }
    
    LOG_INFO(LOG_MODULE_NETWORK, "Executing AT command sequence (%d commands)", count);
    
    /* 逐个执行AT指令序列 */
    for (i = 0; i < count; i++) {
        const char* cmd_display = (cmd_sequence[i].at_cmd && strlen(cmd_sequence[i].at_cmd) > 0) ? cmd_sequence[i].at_cmd : 
                                (cmd_sequence[i].expected_resp && strlen(cmd_sequence[i].expected_resp) > 0) ? cmd_sequence[i].expected_resp : "Wait";
        LOG_DEBUG(LOG_MODULE_NETWORK, "Executing command %d/%d: %s", 
                i + 1, count, cmd_display);
        
        /* 直接使用本地函数执行 */
        result = execute_at_command_with_config(&cmd_sequence[i]);
        
        if (result != AT_RESULT_OK) {
            all_success = false;
            /* 检查是否为关键指令失败 */
            if (cmd_sequence[i].critical) {
                break;  /* 关键指令失败，立即停止序列执行 */
            }
            /* 非关键指令失败处理 */
            if (result == AT_RESULT_RETRY_EXHAUSTED) {
                LOG_WARN(LOG_MODULE_NETWORK, "Non-critical command retry exhausted, continuing with next command");
            }
        } else {
            LOG_DEBUG(LOG_MODULE_NETWORK, "Command executed successfully: %s", cmd_display);
        }
        /* 指令间适当延时，避免发送过快 */
        if (i < (count - 1)) {
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
    return all_success;
}

/* AT指令回调函数实现 */
/**
 * @brief 解析信号质量响应
 * @param response AT命令响应字符串
 * @param rssi 信号强度输出指针
 * @param ber 误码率输出指针
 * @return bool 解析是否成功
 */
static bool parse_signal_quality(const char* response, int8_t* rssi, uint8_t* ber)
{
    if (!response || !rssi || !ber) {
        return false;
    }
    
    /* 查找+CSQ:响应 */
    const char* csq_start = strstr(response, "+CSQ:");
    if (csq_start) {
        int rssi_raw, ber_raw;
        if (sscanf(csq_start, "+CSQ: %d,%d", &rssi_raw, &ber_raw) == 2) {
            /* 转换RSSI值：0-31有效，99表示未知 */
            if (rssi_raw == 99) {
                *rssi = -113;  // 未知信号强度
            } else if (rssi_raw >= 0 && rssi_raw <= 31) {
                *rssi = -113 + 2 * rssi_raw;  // 转换为dBm
            } else {
                *rssi = -113;  // 无效值
            }
            
            /* BER值：0-7有效，99表示未知 */
            *ber = (ber_raw == 99) ? 0 : (uint8_t)ber_raw;
            
            return true;
        }
    }
    
    return false;
}

/**
 * @brief 解析运营商信息响应
 * @param response AT命令响应字符串
 * @param operator_name 运营商名称输出缓冲区
 * @param name_size 缓冲区大小
 * @return bool 解析是否成功
 */
static bool parse_operator_info(const char* response, char* operator_name, size_t name_size)
{
    if (!response || !operator_name || name_size == 0) {
        return false;
    }
    
    /* 查找+COPS:响应 */
    const char* cops_start = strstr(response, "+COPS:");
    if (cops_start) {
        /* 解析格式：+COPS: <mode>,<format>,"<oper>"[,<AcT>] */
        const char* quote_start = strstr(cops_start, "\"");
        if (quote_start) {
            quote_start++;  // 跳过第一个引号
            const char* quote_end = strstr(quote_start, "\"");
            if (quote_end) {
                size_t len = quote_end - quote_start;
                if (len < name_size) {
                    strncpy(operator_name, quote_start, len);
                    operator_name[len] = '\0';
                    return true;
                }
            }
        }
    }
    
    /* 如果没找到引号格式，尝试直接解析数字格式 */
    if (strstr(response, "46000") || strstr(response, "46002") || strstr(response, "46004") || strstr(response, "46007") || strstr(response, "46008")) {
        strncpy(operator_name, "China Mobile", name_size - 1);
        operator_name[name_size - 1] = '\0';
        return true;
    } else if (strstr(response, "46001") || strstr(response, "46006") || strstr(response, "46009")) {
        strncpy(operator_name, "China Unicom", name_size - 1);
        operator_name[name_size - 1] = '\0';
        return true;
    } else if (strstr(response, "46003") || strstr(response, "46005") || strstr(response, "46011")) {
        strncpy(operator_name, "China Telecom", name_size - 1);
        operator_name[name_size - 1] = '\0';
        return true;
    }
    
    return false;
}

/**
 * @brief AT基本测试回调
 */
static void at_callback_basic_test(const char* resp, AT_Result_t result)
{
    if (result == AT_RESULT_OK) {
        LOG_INFO(LOG_MODULE_NETWORK, "4G module responds to AT commands");
    } else {
        // LOG_ERROR(LOG_MODULE_NETWORK, "4G module not responding to AT commands");
    }
}

/**
 * @brief 获取版本信息回调
 */
static void at_callback_get_version(const char* resp, AT_Result_t result)
{
    if (result == AT_RESULT_OK && resp) {
        /* 解析固件版本信息 */
        char* version_start = strstr(resp, "Revision: ");
        if (version_start) {
            version_start += 10;  // 跳过"Revision: "
            char* version_end = strstr(version_start, "\r");
            if (version_end) {
                size_t version_len = version_end - version_start;
                if (version_len < sizeof(g_Lte_info.firmware_version)) {
                    strncpy(g_Lte_info.firmware_version, version_start, version_len);
                    g_Lte_info.firmware_version[version_len] = '\0';
                }
            }
        }
        
        /* 解析模块型号 */
        char* model_start = strstr(resp, "Lte");
        if (model_start) {
            char* model_end = strstr(model_start, "\r");
            if (model_end) {
                size_t model_len = model_end - model_start;
                if (model_len < sizeof(g_Lte_info.module_model)) {
                    strncpy(g_Lte_info.module_model, model_start, model_len);
                    g_Lte_info.module_model[model_len] = '\0';
                }
            }
        }
        
        LOG_INFO(LOG_MODULE_NETWORK, "4G module: %s, Version: %s", 
                g_Lte_info.module_model, g_Lte_info.firmware_version);
    }
}

/**
 * @brief 获取IMEI回调
 */
static void at_callback_get_imei(const char* resp, AT_Result_t result)
{
    if (result == AT_RESULT_OK && resp) {
        /* 解析IMEI */
        char* imei_start = strstr(resp, "86");  // IMEI通常以86开头
        if (imei_start) {
            char* imei_end = strstr(imei_start, "\r");
            if (imei_end) {
                size_t imei_len = imei_end - imei_start;
                if (imei_len < sizeof(g_Lte_info.imei)) {
                    strncpy(g_Lte_info.imei, imei_start, imei_len);
                    g_Lte_info.imei[imei_len] = '\0';
                    
                    LOG_INFO(LOG_MODULE_NETWORK, "IMEI: %s", g_Lte_info.imei);
                }
            }
        }
    }
}

/**
 * @brief 获取ICCID回调
 */
static void at_callback_get_iccid(const char* resp, AT_Result_t result)
{
    if (result == AT_RESULT_OK && resp) {
        /* 解析ICCID */
        char* iccid_start = strstr(resp, "+QCCID: ");
        if (iccid_start) {
            iccid_start += 8;  // 跳过"+QCCID: "
            char* iccid_end = strstr(iccid_start, "\r");
            if (iccid_end) {
                size_t iccid_len = iccid_end - iccid_start;
                if (iccid_len < sizeof(g_Lte_info.iccid)) {
                    strncpy(g_Lte_info.iccid, iccid_start, iccid_len);
                    g_Lte_info.iccid[iccid_len] = '\0';
                    
                    LOG_INFO(LOG_MODULE_NETWORK, "ICCID: %s", g_Lte_info.iccid);
                }
            }
        }
    }
}

/**
 * @brief 获取IMSI回调
 */
static void at_callback_get_imsi(const char* resp, AT_Result_t result)
{
    if (result == AT_RESULT_OK && resp) {
        /* 解析IMSI */
        char* imsi_start = strstr(resp, "460");  // 中国移动、联通、电信IMSI都以460开头
        if (imsi_start) {
            char* imsi_end = strstr(imsi_start, "\r");
            if (imsi_end) {
                size_t imsi_len = imsi_end - imsi_start;
                if (imsi_len < sizeof(g_Lte_info.imsi)) {
                    strncpy(g_Lte_info.imsi, imsi_start, imsi_len);
                    g_Lte_info.imsi[imsi_len] = '\0';
                    
                    LOG_INFO(LOG_MODULE_NETWORK, "IMSI: %s", g_Lte_info.imsi);
                }
            }
        }
    }
}

/**
 * @brief 检查SIM卡回调
 */
static void at_callback_check_sim(const char* resp, AT_Result_t result)
{
    if (result == AT_RESULT_OK) {
        LOG_INFO(LOG_MODULE_NETWORK, "SIM card is ready");
    } else {
        LOG_ERROR(LOG_MODULE_NETWORK, "SIM card not ready or not inserted");
    }
}

/**
 * @brief 信号质量回调
 */
static void at_callback_signal_quality(const char* resp, AT_Result_t result)
{
    if (result == AT_RESULT_OK && resp) {
        int8_t rssi;
        uint8_t ber;
        if (parse_signal_quality(resp, &rssi, &ber)) {
            /* 更新新的全局设备信息 */
            g_Lte_info.signal_rssi = rssi;
            g_Lte_info.signal_ber = ber;
            
            /* 同时更新旧的网络信息结构 */
            LOG_INFO(LOG_MODULE_NETWORK, "Signal: %d dBm, BER: %d", rssi, ber);
        }
    }
}

/**
 * @brief 网络注册回调
 */
static void at_callback_network_reg(const char* resp, AT_Result_t result)
{
    if (result == AT_RESULT_OK) {
        LOG_INFO(LOG_MODULE_NETWORK, "Network registration successful");
    } else {
        LOG_WARN(LOG_MODULE_NETWORK, "Network registration failed");
    }
}

/**
 * @brief 运营商信息回调
 */
static void at_callback_operator_info(const char* resp, AT_Result_t result)
{
    if (result == AT_RESULT_OK && resp) {
        if (parse_operator_info(resp, g_Lte_info.operator_name, 
                               sizeof(g_Lte_info.operator_name))) {
            LOG_INFO(LOG_MODULE_NETWORK, "Operator: %s", g_Lte_info.operator_name);
        }
    }
}

/**
 * @brief 获取4G设备信息
 * @param info 设备信息结构指针
 * @return bool 获取是否成功
 * @note 简化为直接复制，因为在4G网络连接流程中不存在多任务竞争
 */
bool Lte_GetDeviceInfo(Lte_Device_Info_t* info)
{
    if (!info) {
        return false;
    }
    
    *info = g_Lte_info;
    return true;
}

/**
 * @brief 更新4G设备信息
 * @param info 设备信息结构指针
 * @return bool 更新是否成功
 * @note 简化为直接复制，因为在4G网络连接流程中不存在多任务竞争
 */
bool Lte_UpdateDeviceInfo(const Lte_Device_Info_t* info)
{
    if (!info) {
        return false;
    }
    
    g_Lte_info = *info;
    return true;
}

/**
 * @brief 执行模块重启（内部函数）
 * @details 根据参数执行软件重启或硬件重启。软件重启通过AT指令实现，
 *          硬件重启通过控制GPIO引脚实现。每种重启方式都有对应的计数器
 *          用于防止无限重启循环。
 * 
 * @param hard_reset true-硬件重启（GPIO控制），false-软件重启（AT指令）
 * @return bool true-重启操作成功，false-重启操作失败
 * 
 * @note 硬件重启需要根据具体硬件设计实现GPIO控制逻辑
 * @note 重启后会自动更新对应的计数器
 * @note 重启成功后会等待模块重新启动完成
 */
static bool perform_module_reset(bool hard_reset)
{
    bool reset_success = false;
    
    if (hard_reset) {
        LOG_INFO(LOG_MODULE_NETWORK, "Performing hardware reset of 4G module");
        s_hard_reset_count++;
        lte_4g_module_hardware_reset(); // 调用具体的硬件重置函数
        LOG_INFO(LOG_MODULE_NETWORK, "Hardware reset pulse sent, waiting for module restart");
        vTaskDelay(pdMS_TO_TICKS(MODULE_RESET_WAIT_MS));
        reset_success = true;  /* 假设硬件重启操作成功 */
        
    } else {
        LOG_INFO(LOG_MODULE_NETWORK, "Performing software reset of 4G module");
        s_soft_reset_count++;
        
        /* 软件重启：发送AT+CFUN=1,1指令重启射频功能 */
        AT_Cmd_Config_t reset_cmd = {
            .module_type = MODULE_TYPE_4G,
            .at_cmd = "AT+CFUN=1,1",
            .expected_resp = "OK",
            .timeout_ms = SOFT_RESET_TIMEOUT_MS,
            .retries = 2,
            .critical = false,
            .callback = NULL
        };
        
        /* 使用本地函数执行软件重启 */
        AT_Result_t result = execute_at_command_with_config(&reset_cmd);
        if (result == AT_RESULT_OK) {
            LOG_INFO(LOG_MODULE_NETWORK, "Software reset command sent successfully");
            vTaskDelay(pdMS_TO_TICKS(MODULE_RESET_WAIT_MS));  /* 等待模块重启完成 */
            reset_success = true;
        } else {
            LOG_ERROR(LOG_MODULE_NETWORK, "Software reset command failed");
            reset_success = false;
        }
    }
    
    /* 重启后重置状态 */
    if (reset_success) {
        update_state(LTE_STATE_UNKNOWN);
        s_init_completed = false;
        
        /* 清空设备信息 */
        memset(&g_Lte_info, 0, sizeof(g_Lte_info));
        
        LOG_INFO(LOG_MODULE_NETWORK, "Module reset completed successfully");
    }
    
    return reset_success;
}

/**
 * @brief 成功初始化后重置计数器
 */
static void reset_counters_on_success(void)
{
    s_hard_reset_count = 0;
    s_soft_reset_count = 0;
    LOG_DEBUG(LOG_MODULE_NETWORK, "Reset counters cleared on successful initialization");
}

/**
 * @brief 硬件重启4G模块
 * @return BaseType_t 操作结果
 */
BaseType_t Lte_HardReset(void)
{
    LOG_INFO(LOG_MODULE_NETWORK, "Manual hard reset requested");
    
    if (s_hard_reset_count >= MAX_HARD_RESET_COUNT) {
        LOG_ERROR(LOG_MODULE_NETWORK, "Hard reset count exceeded, reset denied");
        return pdFAIL;
    }
    
    if (perform_module_reset(true)) {
        return pdPASS;
    } else {
        return pdFAIL;
    }
}

/**
 * @brief 软件重启4G模块
 * @return BaseType_t 操作结果
 */
BaseType_t Lte_SoftReset(void)
{
    LOG_INFO(LOG_MODULE_NETWORK, "Manual soft reset requested");
    
    if (perform_module_reset(false)) {
        return pdPASS;
    } else {
        return pdFAIL;
    }
}

/**
 * @brief 检查网络注册状态序列
 * @return bool 是否成功注册网络
 */
static bool check_network_registration_sequence(void)
{
    update_state(LTE_STATE_REGISTERING);
    
    /* 定义网络注册检查序列 */
    const AT_Cmd_Config_t registration_sequence[] = {
        /* 检查网络注册状态 */
        {
            .module_type = MODULE_TYPE_4G,
            .at_cmd = "AT+CREG?\r\n",
            .expected_resp = "+CREG:",
            .timeout_ms = 5000,
            .retries = 30,
            .critical = true,
            .callback = at_callback_network_reg
        },
        /* 检查GPRS注册状态 */
        {
            .module_type = MODULE_TYPE_4G,
            .at_cmd = "AT+CGREG?\r\n",
            .expected_resp = "+CGREG:",
            .timeout_ms = 5000,
            .retries = 5,
            .critical = false,
            .callback = NULL
        }
    };
    
    const uint8_t reg_cmd_count = sizeof(registration_sequence) / sizeof(AT_Cmd_Config_t);
    
    LOG_INFO(LOG_MODULE_NETWORK, "Checking network registration...");
    return Lte_ExecuteAtSequence(registration_sequence, reg_cmd_count);
}

/**
 * @brief 建立数据连接序列
 * @return bool 是否成功建立连接
 */
static bool setup_data_connection_sequence(void)
{
    static char apn_command[128];  // 使用静态变量避免栈内存问题
    
    update_state(LTE_STATE_CONNECTING);
    
    /* 配置APN命令 */
    snprintf(apn_command, sizeof(apn_command), "AT+QICSGP=1,1,\"%s\",\"%s\",\"%s\",1\r\n", 
            s_init_config.apn, s_init_config.username, s_init_config.password);
    
    /* 定义数据连接建立序列 */
    AT_Cmd_Config_t connection_sequence[] = {
        /* 配置APN */
        {
            .module_type = MODULE_TYPE_4G,
            .at_cmd = apn_command, // 直接使用apn_command变量
            .expected_resp = "OK",
            .timeout_ms = 5000,
            .retries = 2,
            .critical = true,
            .callback = NULL
        },
        /* 激活PDP上下文 */
        {
            .module_type = MODULE_TYPE_4G,
            .at_cmd = "AT+QIACT=1\r\n",
            .expected_resp = "OK",
            .timeout_ms = 15000,
            .retries = 3,
            .critical = true,
            .callback = NULL
        },
        /* 查询IP地址 */
        {
            .module_type = MODULE_TYPE_4G,
            .at_cmd = "AT+QIACT?\r\n",
            .expected_resp = "+QIACT:",
            .timeout_ms = 3000,
            .retries = 1,
            .critical = false,
            .callback = NULL
        }
    };
    
    const uint8_t conn_cmd_count = sizeof(connection_sequence) / sizeof(AT_Cmd_Config_t);
    
    LOG_INFO(LOG_MODULE_NETWORK, "Setting up data connection...");
    return Lte_ExecuteAtSequence(connection_sequence, conn_cmd_count);
}

/**
 * @brief 执行初始化序列
 * @details 按照预定义的AT指令序列对4G模块进行初始化配置。
 *          该函数执行完整的初始化流程，包括基本通信测试、配置设置、
 *          网络注册和数据服务激活。支持智能重试和错误恢复机制。
 * 
 * @note 初始化过程中如果遇到关键指令失败，会自动触发模块重启
 * @note 非关键指令失败时会记录警告但继续执行后续指令
 * @note 整个初始化过程失败时会自动重试，超过最大重试次数后触发重启
 */
static void perform_initialization_sequence(void)
{
    uint8_t retry_count = 0;
    bool init_success = false;
    
    update_state(LTE_STATE_INITIALIZING);
    LOG_INFO(LOG_MODULE_NETWORK, "Starting Lte 4G initialization sequence");
    
    /* 定义初始化AT指令序列 */
    const AT_Cmd_Config_t init_sequence[] = {
        /* 等待模块准备就绪信号 */
        {
            .module_type = MODULE_TYPE_4G,
            .at_cmd = NULL,  // NULL表示仅等待响应，不发送指令
            .expected_resp = "RDY",
            .timeout_ms = 15000,
            .retries = 1,
            .critical = false,
            .callback = NULL
        },
        /* 基本通信测试 */
        {
            .module_type = MODULE_TYPE_4G,
            .at_cmd = "AT\r\n",
            .expected_resp = "OK",
            .timeout_ms = 3000,
            .retries = 3,
            .critical = true,
            .callback = at_callback_basic_test
        },
        /* 设置回显模式 */
        {
            .module_type = MODULE_TYPE_4G,
            .at_cmd = s_init_config.enable_echo ? "ATE1\r\n" : "ATE0\r\n",
            .expected_resp = "OK",
            .timeout_ms = 3000,
            .retries = 2,
            .critical = false,
            .callback = NULL
        },
        /* 设置全功能模式 */
        {
            .module_type = MODULE_TYPE_4G,
            .at_cmd = "AT+CFUN=1\r\n",
            .expected_resp = "OK",
            .timeout_ms = 10000,
            .retries = 2,
            .critical = true,
            .callback = NULL
        },
        /* 获取版本信息 */
        {
            .module_type = MODULE_TYPE_4G,
            .at_cmd = "ATI\r\n",
            .expected_resp = "OK",
            .timeout_ms = 5000,
            .retries = 1,
            .critical = false,
            .callback = at_callback_get_version
        },
        /* 获取IMEI */
        {
            .module_type = MODULE_TYPE_4G,
            .at_cmd = "AT+GSN\r\n",
            .expected_resp = "OK",
            .timeout_ms = 5000,
            .retries = 1,
            .critical = false,
            .callback = at_callback_get_imei
        },
        /* 获取ICCID */
        {
            .module_type = MODULE_TYPE_4G,
            .at_cmd = "AT+QCCID\r\n",
            .expected_resp = "OK",
            .timeout_ms = 5000,
            .retries = 1,
            .critical = false,
            .callback = at_callback_get_iccid
        },
        /* 获取IMSI */
        {
            .module_type = MODULE_TYPE_4G,
            .at_cmd = "AT+CIMI\r\n",
            .expected_resp = "OK",
            .timeout_ms = 5000,
            .retries = 1,
            .critical = false,
            .callback = at_callback_get_imsi
        },
        /* 检查SIM卡状态 */
        {
            .module_type = MODULE_TYPE_4G,
            .at_cmd = "AT+CPIN?\r\n",
            .expected_resp = "READY",
            .timeout_ms = 5000,
            .retries = 3,
            .critical = true,
            .callback = at_callback_check_sim
        }
    };
    
    const uint8_t init_cmd_count = sizeof(init_sequence) / sizeof(AT_Cmd_Config_t);
    
    /* 初始化重试循环 */
    while (retry_count < s_init_config.max_retry_count && !init_success) {
        /* 等待模块启动稳定 */
        // vTaskDelay(pdMS_TO_TICKS(3000));
        
        LOG_INFO(LOG_MODULE_NETWORK, "Initialization attempt %d/%d", 
                retry_count + 1, s_init_config.max_retry_count);
        
        /* 执行基础初始化序列 */
        if (Lte_ExecuteAtSequence(init_sequence, init_cmd_count)) {
            update_state(LTE_STATE_READY);
            
            /* 根据配置决定是否自动注册网络 */
            if (s_init_config.auto_register) {
                if (check_network_registration_sequence()) {
                    update_state(LTE_STATE_REGISTERED);
                    
                    /* 尝试建立数据连接 */
                    if (setup_data_connection_sequence()) {
                        update_state(LTE_STATE_CONNECTED);
                        init_success = true;  /* 完整初始化成功 */
                    }
                }
            } else {
                init_success = true;  /* 仅基础初始化成功即可 */
            }
        }
        
        /* 处理初始化失败的情况 */
        if (!init_success) {
            retry_count++;
            if (retry_count < s_init_config.max_retry_count) {
                /* 根据重试次数采用不同的重启策略 */
                if (retry_count >= 4) {  /* 前四次失败后尝试软件重启 */
                    LOG_WARN(LOG_MODULE_NETWORK, "Initialization failed %d times, attempting soft reset", retry_count);
                    if (!perform_module_reset(true)) {  /* TODO  执行软件重启  全都执行硬件重启,软件重启将true改为false即可*/
                        LOG_ERROR(LOG_MODULE_NETWORK, "Soft reset failed, will try hard reset next");
                    }
                } else {
                    // LOG_WARN(LOG_MODULE_NETWORK, "Initialization failed, retrying in %d ms", 
                    //         INIT_RETRY_DELAY_MS);
                }
                vTaskDelay(pdMS_TO_TICKS(INIT_RETRY_DELAY_MS));
            } else {
                /* 所有常规重试都失败，尝试硬件重启作为最后手段 */
                LOG_ERROR(LOG_MODULE_NETWORK, "All initialization attempts failed, attempting hard reset");
                if (s_hard_reset_count < MAX_HARD_RESET_COUNT) {
                    if (perform_module_reset(true)) {  /* 执行硬件重启 */
                        /* 硬件重启成功，重置重试计数器进行最后一次尝试 */
                        retry_count = 0;
                        LOG_INFO(LOG_MODULE_NETWORK, "Hard reset completed, restarting initialization");
                        vTaskDelay(pdMS_TO_TICKS(MODULE_RESET_WAIT_MS));
                    } else {
                        LOG_ERROR(LOG_MODULE_NETWORK, "Hard reset failed");
                    }
                } else {
                    LOG_ERROR(LOG_MODULE_NETWORK, "Maximum hard reset count exceeded, stopping attempts");
                }
            }
        }
    }
    
    if (init_success) {
        s_init_completed = true;
        reset_counters_on_success();  // 成功后重置计数器
        LOG_INFO(LOG_MODULE_NETWORK, "Lte 4G initialization completed successfully");
        
        /* 获取初始网络状态 */
        (void)get_network_status();
        
    } else {
        update_state(LTE_STATE_ERROR);
        LOG_ERROR(LOG_MODULE_NETWORK, "Lte 4G initialization failed after %d attempts and %d hard resets", 
                 s_init_config.max_retry_count, s_hard_reset_count);
    }
}


/* ============================= 任务函数实现 ============================= */
bool get_lte_initialization_status(void)
{
    return s_init_completed;
}
/**
 * @brief 4G模块初始化任务主函数
 * @param pvParameters 任务参数（未使用）
 * 
 * @details 这是4G模块管理的主要任务函数，负责：
 *          1. 执行完整的模块初始化序列
 *          2. 定期监控网络状态
 *          3. 处理模块错误状态，自动重新初始化
 *          4. 维护模块的正常运行状态
 * 
 * @note 该任务会一直运行，直到系统关闭
 */
static void vLteInitTask(void* pvParameters)
{
    (void)pvParameters;  /* 避免未使用参数警告 */
    TickType_t last_status_check = xTaskGetTickCount();
    
    LOG_INFO(LOG_MODULE_NETWORK, "Lte 4G initialization task started, free heap: %d", xPortGetFreeHeapSize());
    
    /* 首先执行LTE硬件初始化（在任务中执行，避免阻塞主线程） */
    if (!s_lte_module_hardware_initialized) {
        LOG_INFO(LOG_MODULE_NETWORK, "Initializing LTE module hardware...");
        lte_4g_module_hardware_reset();
        s_lte_module_hardware_initialized = true;
        LOG_INFO(LOG_MODULE_NETWORK, "LTE module hardware initialized successfully");
    }

    LOG_INFO(LOG_MODULE_NETWORK, "Starting LTE initialization sequence...");
    /* 执行初始化序列 */
    // perform_initialization_sequence();
    
    /* 任务主循环 - 状态监控和维护 */
    while (1) {
        // TickType_t current_time = xTaskGetTickCount();
        
        // /* 定期检查网络状态（仅在模块就绪后） */
        // if ((current_time - last_status_check) >= pdMS_TO_TICKS(STATUS_CHECK_INTERVAL_MS)) {
        //     if (s_current_state >= LTE_STATE_READY) {
        //         monitor_network_status();  /* 监控网络状态和信号质量 */
        //     }
        //     last_status_check = current_time;
        // }
        
        // /* 错误状态自动恢复机制 */
        // if (s_current_state == LTE_STATE_ERROR) {
        //     LOG_WARN(LOG_MODULE_NETWORK, "4G module in error state, attempting reinitialize");
        //     vTaskDelay(pdMS_TO_TICKS(INIT_RETRY_DELAY_MS));  /* 等待后重试 */
        //     perform_initialization_sequence();  /* 重新执行初始化 */
        // }
        /* 任务循环延时，避免占用过多CPU资源 */
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

/**
 * @brief 清理已创建的资源
 * @param cleanup_mask 清理位掩码（指定需要清理的资源）
 */
static void cleanup_resources(uint16_t cleanup_mask)
{
    /* 清理4G AT响应队列 */
    if ((cleanup_mask & CLEANUP_AT_QUEUE) && s_lte_receive_queue) {
        vQueueDelete(s_lte_receive_queue);
        s_lte_receive_queue = NULL;
    }
    
    /* 清理数据接收任务 */
    if ((cleanup_mask & CLEANUP_DATA_TASK) && s_data_receive_task_handle) {
        vTaskDelete(s_data_receive_task_handle);
        s_data_receive_task_handle = NULL;
    }
    
    /* 清理互斥量 */
    if (cleanup_mask & CLEANUP_MUTEXES) {
        if (s_lte_access_mutex) {
            vSemaphoreDelete(s_lte_access_mutex);
            s_lte_access_mutex = NULL;
        }
    }
}

/**
 * @brief 初始化4G模块初始化任务
 * @param config 初始化配置指针，NULL使用默认配置
 * @return BaseType_t 初始化结果
 */
BaseType_t Lte_init(const LteInitConfig_t* config)
{
    BaseType_t result;
    
    /* 使用默认配置或用户配置 */
    if (config) {
        s_init_config = *config;
    } else {
        s_init_config = Lte_GetDefaultConfig();
    }
    
    /* 初始化全局设备信息结构 */
    memset(&g_Lte_info, 0, sizeof(g_Lte_info));
    strncpy(g_Lte_info.apn, s_init_config.apn, sizeof(g_Lte_info.apn) - 1);
    g_Lte_info.context_id = 1;  // 默认使用上下文ID 1
    g_Lte_info.connection_type = 0;  // 默认TCP连接
    
    
    /* 创建AT响应队列 */
    s_lte_receive_queue = xQueueCreate(QUEUE_LENGTH, QUEUE_ITEM_SIZE);
    if (!s_lte_receive_queue) {
        LOG_ERROR(LOG_MODULE_NETWORK, "Failed to create 4G AT queue");
        cleanup_resources(CLEANUP_DATA_QUEUE);
        return pdFAIL;
    }

    /* 创建LTE访问互斥量 - 用于与commu_receiver任务互斥访问串口 */
    s_lte_access_mutex = xSemaphoreCreateMutex();
    if (!s_lte_access_mutex) {
        LOG_ERROR(LOG_MODULE_NETWORK, "Failed to create LTE access mutex");
        cleanup_resources(CLEANUP_DATA_QUEUE | CLEANUP_AT_QUEUE);
        return pdFAIL;
    }

    /* 创建初始化任务 */
    result = xTaskCreate(vLteInitTask, "LteInit", INIT_TASK_STACK_SIZE,
                        NULL, INIT_TASK_PRIORITY, &s_init_task_handle);
    
    if (result != pdPASS) {
        LOG_ERROR(LOG_MODULE_NETWORK, "Failed to create 4G init task");
        cleanup_resources(CLEANUP_ALL_RESOURCES);
        return pdFAIL;
    }
    
    LOG_INFO(LOG_MODULE_NETWORK, "Lte 4G simplified initialization completed");
    return pdPASS;
}


bool LTE_SendDataToMCU(ModuleType_t module_type, const uint8_t* data, uint32_t length)
{
    if (!data || length == 0) {
        LOG_ERROR(LOG_MODULE_NETWORK, "Invalid data parameters");
        return false;
    }
    
    if (!s_lte_receive_queue) {
        LOG_ERROR(LOG_MODULE_NETWORK, "4G AT queue not initialized");
        return false;
    }
    
    /* 创建队列消息 */
    LteToMcu_QueueMessage_t queue_message;
    memset(&queue_message, 0, sizeof(queue_message));
    queue_message.header.module_type = module_type;
    queue_message.header.timestamp = xTaskGetTickCount();
    queue_message.header.target_module = MODULE_TYPE_MCU;
    /* 限制数据长度 */
    uint32_t copy_length = (length > sizeof(queue_message.data)) ? sizeof(queue_message.data) : length;
    memcpy(queue_message.data, data, copy_length);
    queue_message.header.data_length = copy_length;
    
    /* 发送到队列 */
    BaseType_t result = xQueueSend(s_lte_receive_queue, &queue_message, pdMS_TO_TICKS(100)); //TODO 该队列在数据处理任务接收
    
    if (result == pdPASS) {
        LOG_DEBUG(LOG_MODULE_NETWORK, "Queued %d bytes to 4G data queue", copy_length);
        return true;
    } else {
        LOG_WARN(LOG_MODULE_NETWORK, "Failed to queue 4G data (%d bytes), queue might be full", copy_length);
        return false;
    }
}


/*
    通信接收任务 commu_receive      串口数据接收 lte、rs485
    通信发送任务 commu_send         串口数据发送 lte、rs485
    数据处理任务 data_process       lte接收数据处理，通过s_lte_receive_queue
    lte通信任务 lte_commu           进行模块联网
*/
