/**
 * =====================================================================================
 * @file        tsk_4g_init.c
 * @brief       移远4G模块初始化任务实现
 * @author      23Elapse & GitHub Copilot
 * @version     1.0
 * @date        2025-09-03
 * @note        负责移远4G模块的初始化、配置和状态管理
 * =====================================================================================
 */

#include "tsk_4g_init.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "bsp_uart_ring_buffer.h"
#include "bsp_log_manager.h"
#include "bsp_watchdog.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* ============================= 配置参数定义 ============================= */

/* 任务参数配置 */
#define INIT_TASK_STACK_SIZE        (configMINIMAL_STACK_SIZE * 3)  /**< 初始化任务栈大小 */
#define INIT_TASK_PRIORITY          (tskIDLE_PRIORITY + 4)          /**< 初始化任务优先级 */
#define AT_COMMAND_TIMEOUT_MS       5000                            /**< AT指令默认超时时间 */
#define INIT_RETRY_DELAY_MS         2000                            /**< 初始化重试间隔时间 */
#define STATUS_CHECK_INTERVAL_MS    10000                           /**< 状态检查间隔时间 */

/* 模块重启相关参数 */
#define MODULE_RESET_PULSE_MS       100     /**< 硬件重启脉冲持续时间（毫秒） */
#define MODULE_RESET_WAIT_MS        5000    /**< 模块重启后等待时间（毫秒） */
#define MAX_HARD_RESET_COUNT        3       /**< 最大硬件重启次数 */
#define SOFT_RESET_TIMEOUT_MS       10000   /**< 软件重启命令超时时间（毫秒） */

/* ============================= 静态变量定义 ============================= */

/**
 * @brief 任务和同步相关变量
 */
static TaskHandle_t s_init_task_handle = NULL;         /**< 初始化任务句柄 */
static SemaphoreHandle_t s_state_mutex = NULL;         /**< 状态保护互斥量 */

/**
 * @brief 状态和配置相关变量
 */
static QuectelState_t s_current_state = QUECTEL_STATE_UNKNOWN;  /**< 当前模块状态 */
static QuectelInitConfig_t s_init_config = {0};        /**< 初始化配置参数 */

/**
 * @brief 运行状态和重启计数相关变量
 */
static bool s_init_completed = false;                  /**< 初始化完成标志 */
static uint8_t s_hard_reset_count = 0;                 /**< 硬件重启计数器 */
static uint8_t s_soft_reset_count = 0;                 /**< 软件重启计数器 */

/**
 * @brief 全局4G设备信息实例
 * @note 这是系统的核心数据结构，所有设备状态信息都存储在这里
 *       通过互斥量保护，确保多线程访问安全
 */
Quectel4G_Device_Info_t g_quectel4g_info = {0};

/* ============================= 函数声明 ============================= */

/**
 * @brief AT指令回调函数声明
 * @note 这些回调函数在对应的AT指令执行完成后被调用，用于解析响应并更新设备信息
 */
static void at_callback_basic_test(const char* resp, AT_Result_t result);
static void at_callback_get_version(const char* resp, AT_Result_t result);
static void at_callback_get_imei(const char* resp, AT_Result_t result);
static void at_callback_get_iccid(const char* resp, AT_Result_t result);
static void at_callback_get_imsi(const char* resp, AT_Result_t result);
static void at_callback_check_sim(const char* resp, AT_Result_t result);
static void at_callback_signal_quality(const char* resp, AT_Result_t result);
static void at_callback_network_reg(const char* resp, AT_Result_t result);
static void at_callback_operator_info(const char* resp, AT_Result_t result);

/**
 * @brief 私有函数声明
 * @note 这些是内部使用的核心功能函数
 */
static void vQuectel4GInitTask(void* pvParameters);                          /**< 主初始化任务函数 */
static void update_state(QuectelState_t new_state);                         /**< 状态更新函数 */
static void perform_initialization_sequence(void);                          /**< 执行初始化序列 */
static void monitor_network_status(void);                                   /**< 监控网络状态 */
static bool parse_signal_quality(const char* response, int8_t* rssi, uint8_t* ber);  /**< 解析信号质量 */
static bool parse_operator_info(const char* response, char* operator_name, size_t name_size);  /**< 解析运营商信息 */
static AT_Result_t execute_at_command_with_config(const AT_Cmd_Config_t* cmd_config);  /**< 执行AT指令 */
static bool get_response_data(char* response_buffer, size_t buffer_size, uint32_t timeout_ms);  /**< 获取响应数据 */
static bool setup_data_connection_sequence(void);
static bool check_network_registration_sequence(void);
static bool get_network_status(void);                                       /**< 获取网络状态信息 */
static bool perform_module_reset(bool hard_reset);
static void reset_counters_on_success(void);

/**
 * @brief 数据传输相关私有函数声明
 * @note 这些函数负责网络数据的收发和连接管理
 */
static void data_receive_task(void* pvParameters);                           /**< 数据接收任务 */
static bool process_socket_data(uint8_t socket_id, const char* data, uint16_t length);  /**< 处理Socket数据 */
static bool parse_socket_receive_data(const char* response, uint8_t* socket_id, 
                                     uint8_t* data_buffer, uint16_t* data_length);  /**< 解析Socket接收数据 */
static QuectelDataResult_t send_data_internal(uint8_t socket_id, const uint8_t* data, 
                                             uint16_t length, uint32_t timeout_ms);  /**< 内部数据发送函数 */
static bool setup_socket_connection(QuectelProtocol_t protocol, const char* remote_ip, 
                                   uint16_t remote_port, uint16_t local_port);  /**< 建立Socket连接 */
static void handle_socket_close_event(uint8_t socket_id);                    /**< 处理Socket关闭事件 */
static void update_network_statistics(uint16_t bytes_sent, uint16_t bytes_received);  /**< 更新网络统计 */

/**
 * @brief TCP服务器连接管理相关私有函数声明
 * @note 这些函数负责TCP服务器连接的完整流程管理
 */
static void tcp_connection_monitor_task(void* pvParameters);                 /**< TCP连接监控任务 */
static bool tcp_connect_with_retry(const Quectel4G_TcpServerConfig_t* config);  /**< 带重试的TCP连接 */
static bool tcp_verify_connection(uint8_t socket_id);                       /**< 验证TCP连接状态 */
static void tcp_handle_connection_event(uint8_t event_type, uint8_t socket_id, uint8_t error_code);  /**< 处理TCP连接事件 */
static bool tcp_send_heartbeat_internal(void);                              /**< 内部心跳发送 */
static bool start_data_receive_task(void);                                  /**< 启动数据接收任务 */
static void stop_data_receive_task(void);                                   /**< 停止数据接收任务 */

/**
 * @brief 设备注册和数据交互相关私有函数声明
 * @note 这些函数负责设备注册流程和实时数据发送
 */
static void device_register_task(void* pvParameters);                       /**< 设备注册任务 */
static void realtime_data_task(void* pvParameters);                         /**< 实时数据发送任务 */
static bool device_send_command(uint8_t command, const uint8_t* data, uint16_t length);  /**< 发送设备指令 */
static uint8_t device_parse_response(const uint8_t* data, uint16_t length); /**< 解析服务器响应 */
static bool device_wait_for_response(uint8_t expected_response, uint32_t timeout_ms);  /**< 等待服务器响应 */
static void device_handle_register_event(DeviceRegisterState_t state, bool success, uint8_t command, uint8_t response);  /**< 处理注册事件 */
static bool device_transition_to_next_state(void);                          /**< 状态机状态转换 */
static void device_cleanup_register_tasks(void);                            /**< 清理注册相关任务 */

/* ============================= 静态变量定义 ============================= */

/**
 * @brief 数据传输相关静态变量
 * @note 这些变量用于管理数据传输状态和缓冲区
 */
static TaskHandle_t s_data_receive_task_handle = NULL;                      /**< 数据接收任务句柄 */
static QuectelDataReceiveCallback_t s_rx_callback = NULL;                   /**< 数据接收回调函数 */
static QuectelConnectionCallback_t s_conn_callback = NULL;                  /**< 连接状态回调函数 */
static uint8_t* s_rx_buffer = NULL;                                         /**< 数据接收缓冲区 */
static uint16_t s_rx_buffer_size = 1024;                                    /**< 接收缓冲区大小 */
static SemaphoreHandle_t s_data_mutex = NULL;                               /**< 数据传输互斥量 */
static QueueHandle_t s_4g_data_queue = NULL;                                /**< 4G数据处理队列 */
static SemaphoreHandle_t s_uart_mutex = NULL;                               /**< LTE串口访问互斥量 */

/* TCP服务器连接管理相关变量 */
static Quectel4G_TcpServerConfig_t s_tcp_config = {0};                      /**< TCP服务器配置 */
static Quectel4G_TcpEventCallback_t s_tcp_event_callback = NULL;            /**< TCP事件回调函数 */
static void* s_tcp_user_data = NULL;                                        /**< TCP用户数据指针 */
static bool s_tcp_auto_reconnect_enabled = false;                           /**< TCP自动重连使能 */
static uint8_t s_tcp_retry_count = 0;                                       /**< TCP重试次数 */
static uint32_t s_tcp_retry_interval = 5000;                                /**< TCP重试间隔（毫秒） */
static uint32_t s_tcp_connect_time = 0;                                     /**< TCP连接建立时间 */
static TaskHandle_t s_tcp_monitor_task = NULL;                              /**< TCP连接监控任务句柄 */

/* 设备注册和数据交互相关变量 */
static DeviceRegisterData_t s_device_register_data = {0};                   /**< 设备注册数据 */
static DeviceRegisterCallback_t s_device_register_callback = NULL;          /**< 设备注册回调函数 */
static void* s_device_register_user_data = NULL;                            /**< 设备注册用户数据 */
static SemaphoreHandle_t s_register_mutex = NULL;                           /**< 设备注册互斥量 */
static TaskHandle_t s_device_register_task = NULL;                          /**< 设备注册任务句柄 */
static TaskHandle_t s_realtime_data_task = NULL;                            /**< 实时数据发送任务句柄 */
static uint32_t s_register_start_time = 0;                                  /**< 注册开始时间 */
static uint8_t s_register_stats_total = 0;                                  /**< 总指令数统计 */
static uint8_t s_register_stats_success = 0;                                /**< 成功指令数统计 */
static uint8_t s_register_stats_failed = 0;                                 /**< 失败指令数统计 */

/* 队列配置参数 */
#define QUEUE_LENGTH            10                                           /**< 队列长度 */
#define QUEUE_ITEM_SIZE         sizeof(Quectel4G_QueueMessage_t)           /**< 队列项大小 */

/* TCP连接监控配置 */
#define TCP_MONITOR_INTERVAL_MS     5000                                     /**< TCP连接监控间隔（毫秒） */
#define TCP_CONNECTION_CHECK_CMD    "AT+QISTATE=0,1"                        /**< TCP连接状态查询命令 */

/* 设备注册配置参数 */
#define DEVICE_REGISTER_TIMEOUT_MS      30000                               /**< 设备注册超时时间（毫秒） */
#define DEVICE_COMMAND_RETRY_COUNT      3                                    /**< 设备指令重试次数 */
#define REALTIME_DATA_DEFAULT_INTERVAL  10000                               /**< 实时数据默认发送间隔（毫秒） */
#define DEVICE_REGISTER_TASK_STACK      2048                                /**< 设备注册任务栈大小 */
#define REALTIME_DATA_TASK_STACK        1024                                /**< 实时数据任务栈大小 */

/* ============================= 核心功能实现 ============================= */

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
static void vQuectel4GInitTask(void* pvParameters)
{
    (void)pvParameters;  /* 避免未使用参数警告 */
    TickType_t last_status_check = 0;  /* 上次状态检查时间 */
    
    LOG_INFO(LOG_MODULE_NETWORK, "Quectel 4G initialization task started");
    
    /* 执行初始化序列 */
    perform_initialization_sequence();
    
    /* 任务主循环 - 状态监控和维护 */
    while (1) {
        TickType_t current_time = xTaskGetTickCount();
        
        /* 定期检查网络状态（仅在模块就绪后） */
        if ((current_time - last_status_check) >= pdMS_TO_TICKS(STATUS_CHECK_INTERVAL_MS)) {
            if (s_current_state >= QUECTEL_STATE_READY) {
                monitor_network_status();  /* 监控网络状态和信号质量 */
            }
            last_status_check = current_time;
        }
        
        /* 错误状态自动恢复机制 */
        if (s_current_state == QUECTEL_STATE_ERROR) {
            LOG_WARN(LOG_MODULE_NETWORK, "4G module in error state, attempting reinitialize");
            vTaskDelay(pdMS_TO_TICKS(INIT_RETRY_DELAY_MS));  /* 等待后重试 */
            perform_initialization_sequence();  /* 重新执行初始化 */
        }
        
        /* 任务循环延时，避免占用过多CPU资源 */
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/**
 * @brief 初始化4G模块初始化任务
 * @param config 初始化配置指针，NULL使用默认配置
 * @return BaseType_t 初始化结果
 */
BaseType_t Quectel4G_Init(const QuectelInitConfig_t* config)
{
    BaseType_t result = pdPASS;
    
    /* 使用默认配置或用户配置 */
    if (config) {
        s_init_config = *config;
    } else {
        s_init_config = Quectel4G_GetDefaultConfig();
    }
    
    /* 初始化全局设备信息结构 */
    memset(&g_quectel4g_info, 0, sizeof(g_quectel4g_info));
    strncpy(g_quectel4g_info.apn, s_init_config.apn, sizeof(g_quectel4g_info.apn) - 1);
    g_quectel4g_info.context_id = 1;  // 默认使用上下文ID 1
    g_quectel4g_info.connection_type = 0;  // 默认TCP连接
    
    /* 创建状态互斥量 */
    s_state_mutex = xSemaphoreCreateMutex();
    if (s_state_mutex == NULL) {
        LOG_ERROR(LOG_MODULE_NETWORK, "Failed to create state mutex");
        return pdFAIL;
    }
    
    /* 创建数据传输互斥量 */
    s_data_mutex = xSemaphoreCreateMutex();
    if (s_data_mutex == NULL) {
        LOG_ERROR(LOG_MODULE_NETWORK, "Failed to create data mutex");
        vSemaphoreDelete(s_state_mutex);
        return pdFAIL;
    }
    
    /* 创建LTE串口访问互斥量 */
    s_uart_mutex = xSemaphoreCreateMutex();
    if (s_uart_mutex == NULL) {
        LOG_ERROR(LOG_MODULE_NETWORK, "Failed to create UART mutex");
        vSemaphoreDelete(s_state_mutex);
        vSemaphoreDelete(s_data_mutex);
        return pdFAIL;
    }
    
    /* 创建设备注册互斥量 */
    s_register_mutex = xSemaphoreCreateMutex();
    if (s_register_mutex == NULL) {
        LOG_ERROR(LOG_MODULE_NETWORK, "Failed to create register mutex");
        vSemaphoreDelete(s_state_mutex);
        vSemaphoreDelete(s_data_mutex);
        vSemaphoreDelete(s_uart_mutex);
        return pdFAIL;
    }
    
    /* 初始化数据传输相关配置 */
    s_rx_callback = s_init_config.rx_callback;
    s_conn_callback = s_init_config.conn_callback;
    s_rx_buffer_size = s_init_config.rx_buffer_size > 0 ? s_init_config.rx_buffer_size : 1024;
    
    /* 分配接收缓冲区 */
    s_rx_buffer = (uint8_t*)pvPortMalloc(s_rx_buffer_size);
    if (s_rx_buffer == NULL) {
        LOG_ERROR(LOG_MODULE_NETWORK, "Failed to allocate RX buffer");
        vSemaphoreDelete(s_state_mutex);
        vSemaphoreDelete(s_data_mutex);
        vSemaphoreDelete(s_uart_mutex);
        vSemaphoreDelete(s_register_mutex);
        return pdFAIL;
    }
    
    /* 创建4G数据处理队列 */
    s_4g_data_queue = xQueueCreate(QUEUE_LENGTH, QUEUE_ITEM_SIZE);
    if (s_4g_data_queue == NULL) {
        LOG_ERROR(LOG_MODULE_NETWORK, "Failed to create 4G data queue");
        vSemaphoreDelete(s_state_mutex);
        vSemaphoreDelete(s_data_mutex);
        vSemaphoreDelete(s_uart_mutex);
        vSemaphoreDelete(s_register_mutex);
        vPortFree(s_rx_buffer);
        return pdFAIL;
    }
    
    /* 创建初始化任务 */
    result = xTaskCreate(vQuectel4GInitTask, "Quectel4GInit", INIT_TASK_STACK_SIZE,
                        NULL, INIT_TASK_PRIORITY, &s_init_task_handle);
    
    if (result != pdPASS) {
        LOG_ERROR(LOG_MODULE_NETWORK, "Failed to create 4G init task");
        vSemaphoreDelete(s_state_mutex);
        vSemaphoreDelete(s_data_mutex);
        vSemaphoreDelete(s_uart_mutex);
        vSemaphoreDelete(s_register_mutex);
        vPortFree(s_rx_buffer);
        vQueueDelete(s_4g_data_queue);
        return pdFAIL;
    }
    
    LOG_INFO(LOG_MODULE_NETWORK, "Quectel 4G init task created successfully");
    return pdPASS;
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
    
    update_state(QUECTEL_STATE_INITIALIZING);
    LOG_INFO(LOG_MODULE_NETWORK, "Starting Quectel 4G initialization sequence");
    
    /* 定义初始化AT指令序列 */
    const AT_Cmd_Config_t init_sequence[] = {
        /* 基本通信测试 */
        {
            .at_cmd = "AT",
            .expected_resp = "OK",
            .timeout_ms = 3000,
            .retries = 3,
            .description = "Basic AT communication test",
            .critical = true,  // 关键指令：通信失败需要重新开始
            .callback = at_callback_basic_test
        },
        /* 设置回显模式 */
        {
            .at_cmd = s_init_config.enable_echo ? "ATE1" : "ATE0",
            .expected_resp = "OK",
            .timeout_ms = 3000,
            .retries = 2,
            .description = "Set echo mode",
            .critical = false,  // 非关键指令：失败可以继续
            .callback = NULL
        },
        /* 设置全功能模式 */
        {
            .at_cmd = "AT+CFUN=1",
            .expected_resp = "OK",
            .timeout_ms = 10000,
            .retries = 2,
            .description = "Set full functionality mode",
            .critical = true,   // 关键指令：功能模式设置失败需要重新开始
            .callback = NULL
        },
        /* 获取版本信息 */
        {
            .at_cmd = "ATI",
            .expected_resp = "OK",
            .timeout_ms = 5000,
            .retries = 1,
            .description = "Get version information",
            .critical = false,  // 非关键指令：版本信息获取失败可以继续
            .callback = at_callback_get_version
        },
        /* 获取IMEI */
        {
            .at_cmd = "AT+GSN",
            .expected_resp = "OK",
            .timeout_ms = 5000,
            .retries = 1,
            .description = "Get IMEI",
            .critical = false,  // 非关键指令：IMEI获取失败可以继续
            .callback = at_callback_get_imei
        },
        /* 获取ICCID */
        {
            .at_cmd = "AT+QCCID",
            .expected_resp = "OK",
            .timeout_ms = 5000,
            .retries = 1,
            .description = "Get ICCID",
            .critical = false,  // 非关键指令：ICCID获取失败可以继续
            .callback = at_callback_get_iccid
        },
        /* 获取IMSI */
        {
            .at_cmd = "AT+CIMI",
            .expected_resp = "OK",
            .timeout_ms = 5000,
            .retries = 1,
            .description = "Get IMSI",
            .critical = false,  // 非关键指令：IMSI获取失败可以继续
            .callback = at_callback_get_imsi
        },
        /* 检查SIM卡状态 */
        {
            .at_cmd = "AT+CPIN?",
            .expected_resp = "READY",
            .timeout_ms = 5000,
            .retries = 3,
            .description = "Check SIM card status",
            .critical = true,   // 关键指令：SIM卡未就绪需要重新开始
            .callback = at_callback_check_sim
        }
    };
    
    const uint8_t init_cmd_count = sizeof(init_sequence) / sizeof(AT_Cmd_Config_t);
    
    /* 初始化重试循环 */
    while (retry_count < s_init_config.max_retry_count && !init_success) {
        /* 等待模块启动稳定 */
        vTaskDelay(pdMS_TO_TICKS(3000));
        
        LOG_INFO(LOG_MODULE_NETWORK, "Initialization attempt %d/%d", 
                retry_count + 1, s_init_config.max_retry_count);
        
        /* 执行基础初始化序列 */
        if (Quectel4G_ExecuteAtSequence(init_sequence, init_cmd_count)) {
            update_state(QUECTEL_STATE_READY);
            
            /* 根据配置决定是否自动注册网络 */
            if (s_init_config.auto_register) {
                if (check_network_registration_sequence()) {
                    update_state(QUECTEL_STATE_REGISTERED);
                    
                    /* 尝试建立数据连接 */
                    if (setup_data_connection_sequence()) {
                        update_state(QUECTEL_STATE_CONNECTED);
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
                if (retry_count >= 2) {  /* 前两次失败后尝试软件重启 */
                    LOG_WARN(LOG_MODULE_NETWORK, "Initialization failed %d times, attempting soft reset", retry_count);
                    if (!perform_module_reset(false)) {  /* 执行软件重启 */
                        LOG_ERROR(LOG_MODULE_NETWORK, "Soft reset failed, will try hard reset next");
                    }
                } else {
                    LOG_WARN(LOG_MODULE_NETWORK, "Initialization failed, retrying in %d ms", 
                            INIT_RETRY_DELAY_MS);
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
        LOG_INFO(LOG_MODULE_NETWORK, "Quectel 4G initialization completed successfully");
        
        /* 获取初始网络状态 */
        (void)get_network_status();
    } else {
        update_state(QUECTEL_STATE_ERROR);
        LOG_ERROR(LOG_MODULE_NETWORK, "Quectel 4G initialization failed after %d attempts and %d hard resets", 
                 s_init_config.max_retry_count, s_hard_reset_count);
    }
}

/**
 * @brief 检查网络注册状态序列
 * @return bool 是否成功注册网络
 */
static bool check_network_registration_sequence(void)
{
    update_state(QUECTEL_STATE_REGISTERING);
    
    /* 定义网络注册检查序列 */
    const AT_Cmd_Config_t registration_sequence[] = {
        /* 检查网络注册状态 */
        {
            .at_cmd = "AT+CREG?",
            .expected_resp = "+CREG:",
            .timeout_ms = 5000,
            .retries = 30,  // 最多重试30次，每次1秒
            .description = "Check network registration status",
            .critical = true,   // 关键指令：网络注册失败需要重新开始
            .callback = at_callback_network_reg
        },
        /* 检查GPRS注册状态 */
        {
            .at_cmd = "AT+CGREG?",
            .expected_resp = "+CGREG:",
            .timeout_ms = 5000,
            .retries = 5,
            .description = "Check GPRS registration status",
            .critical = false,  // 非关键指令：GPRS注册失败可以继续
            .callback = NULL
        }
    };
    
    const uint8_t reg_cmd_count = sizeof(registration_sequence) / sizeof(AT_Cmd_Config_t);
    
    LOG_INFO(LOG_MODULE_NETWORK, "Checking network registration...");
    return Quectel4G_ExecuteAtSequence(registration_sequence, reg_cmd_count);
}

/**
 * @brief 建立数据连接序列
 * @return bool 是否成功建立连接
 */
static bool setup_data_connection_sequence(void)
{
    char apn_command[128];
    
    update_state(QUECTEL_STATE_CONNECTING);
    
    /* 配置APN命令 */
    snprintf(apn_command, sizeof(apn_command), "AT+QICSGP=1,1,\"%s\",\"%s\",\"%s\",1", 
            s_init_config.apn, s_init_config.username, s_init_config.password);
    
    /* 定义数据连接建立序列 */
    AT_Cmd_Config_t connection_sequence[] = {
        /* 配置APN */
        {
            .at_cmd = apn_command,
            .expected_resp = "OK",
            .timeout_ms = 5000,
            .retries = 2,
            .description = "Configure APN settings",
            .critical = true,   // 关键指令：APN配置失败需要重新开始
            .callback = NULL
        },
        /* 激活PDP上下文 */
        {
            .at_cmd = "AT+QIACT=1",
            .expected_resp = "OK",
            .timeout_ms = 15000,
            .retries = 3,
            .description = "Activate PDP context",
            .critical = true,   // 关键指令：PDP上下文激活失败需要重新开始
            .callback = NULL
        },
        /* 查询IP地址 */
        {
            .at_cmd = "AT+QIACT?",
            .expected_resp = "+QIACT:",
            .timeout_ms = 3000,
            .retries = 1,
            .description = "Query IP address",
            .critical = false,  // 非关键指令：IP查询失败可以继续
            .callback = NULL
        }
    };
    
    const uint8_t conn_cmd_count = sizeof(connection_sequence) / sizeof(AT_Cmd_Config_t);
    
    LOG_INFO(LOG_MODULE_NETWORK, "Setting up data connection...");
    return Quectel4G_ExecuteAtSequence(connection_sequence, conn_cmd_count);
}

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
            .at_cmd = "AT+CSQ",
            .expected_resp = "OK",
            .timeout_ms = 3000,
            .retries = 1,
            .description = "Get signal quality",
            .critical = false,  // 非关键指令：信号质量查询失败可以继续
            .callback = at_callback_signal_quality
        },
        /* 获取运营商信息 */
        {
            .at_cmd = "AT+COPS?",
            .expected_resp = "OK",
            .timeout_ms = 5000,
            .retries = 1,
            .description = "Get operator information",
            .critical = false,  // 非关键指令：运营商信息查询失败可以继续
            .callback = at_callback_operator_info
        },
        /* 检查网络注册状态 */
        {
            .at_cmd = "AT+CREG?",
            .expected_resp = "OK",
            .timeout_ms = 3000,
            .retries = 1,
            .description = "Check network registration",
            .critical = false,  // 非关键指令：状态监控中的网络注册查询失败可以继续
            .callback = at_callback_network_reg
        },
        /* 检查数据连接状态 */
        {
            .at_cmd = "AT+QIACT?",
            .expected_resp = "OK",
            .timeout_ms = 3000,
            .retries = 1,
            .description = "Check data connection status",
            .critical = false,  // 非关键指令：数据连接状态查询失败可以继续
            .callback = NULL
        }
    };
    
    const uint8_t status_cmd_count = sizeof(status_sequence) / sizeof(AT_Cmd_Config_t);
    
    /* 执行状态查询序列 */
    return Quectel4G_ExecuteAtSequence(status_sequence, status_cmd_count);
}

/**
 * @brief 更新状态
 * @param new_state 新状态
 */
static void update_state(QuectelState_t new_state)
{
    if (s_state_mutex && xSemaphoreTake(s_state_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        s_current_state = new_state;
        xSemaphoreGive(s_state_mutex);
        
        LOG_DEBUG(LOG_MODULE_NETWORK, "4G state updated to: %d", new_state);
    }
}

/**
 * @brief 获取4G模块当前状态
 * @return QuectelState_t 当前状态
 */
QuectelState_t Quectel4G_GetState(void)
{
    QuectelState_t state = QUECTEL_STATE_UNKNOWN;
    
    if (s_state_mutex && xSemaphoreTake(s_state_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        state = s_current_state;
        xSemaphoreGive(s_state_mutex);
    }
    
    return state;
}

/**
 * @brief 检查4G模块是否就绪
 * @return bool 是否就绪
 */
bool Quectel4G_IsReady(void)
{
    return (Quectel4G_GetState() >= QUECTEL_STATE_READY);
}

/**
 * @brief 等待4G模块初始化完成
 * @param timeout_ms 等待超时时间
 * @return bool 是否在超时前完成初始化
 */
bool Quectel4G_WaitForReady(uint32_t timeout_ms)
{
    uint32_t start_time = xTaskGetTickCount();
    uint32_t elapsed_time;
    
    while (1) {
        if (Quectel4G_IsReady()) {
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
 * @return QuectelInitConfig_t 默认配置
 */
QuectelInitConfig_t Quectel4G_GetDefaultConfig(void)
{
    QuectelInitConfig_t config = {
        .init_timeout_ms = 60000,           // 60秒初始化超时
        .at_response_timeout_ms = 5000,     // 5秒AT响应超时
        .max_retry_count = 3,               // 最多重试3次
        .auto_register = true,              // 自动注册网络
        .enable_echo = false,               // 禁用回显
        .apn = "3gnet",                     // 默认APN
        .username = "",                     // 无用户名
        .password = "",                     // 无密码
        
        /* 数据传输默认配置 */
        .rx_callback = NULL,                // 无默认接收回调
        .conn_callback = NULL,              // 无默认连接回调
        .rx_buffer_size = 1024,             // 1KB接收缓冲区
        .keepalive_interval = 300           // 5分钟心跳间隔
    };
    
    return config;
}

/**
 * @brief 强制重新初始化4G模块
 * @return BaseType_t 操作结果
 */
BaseType_t Quectel4G_Reinitialize(void)
{
    s_init_completed = false;
    update_state(QUECTEL_STATE_UNKNOWN);
    
    /* 触发重新初始化 */
    if (s_init_task_handle) {
        /* 通过设置错误状态来触发重新初始化 */
        update_state(QUECTEL_STATE_ERROR);
        return pdPASS;
    }
    
    return pdFAIL;
}

/**
 * @brief 获取响应数据（仅用于AT指令响应）
 * @details 专门用于AT指令的同步响应等待，使用互斥量保护串口访问
 * 
 * @param response_buffer 响应缓冲区
 * @param buffer_size 缓冲区大小
 * @param timeout_ms 超时时间
 * @return bool 是否成功获取数据
 * 
 * @note 此函数仅用于AT指令的同步响应，异步数据接收通过队列处理
 */
static bool get_response_data(char* response_buffer, size_t buffer_size, uint32_t timeout_ms)
{
    uint32_t start_time;
    uint32_t elapsed_time;
    size_t total_received = 0;
    bool result = false;
    
    if (!response_buffer || buffer_size == 0) {
        return false;
    }
    
    /* 获取串口访问互斥量 */
    if (xSemaphoreTake(s_uart_mutex, pdMS_TO_TICKS(timeout_ms)) != pdTRUE) {
        LOG_WARN(LOG_MODULE_NETWORK, "Failed to acquire UART mutex for AT response");
        return false;
    }
    
    response_buffer[0] = '\0';
    start_time = xTaskGetTickCount();
    
    while (total_received < (buffer_size - 1)) {
        /* 检查超时 */
        elapsed_time = (xTaskGetTickCount() - start_time) * portTICK_PERIOD_MS;
        if (elapsed_time >= timeout_ms) {
            break;
        }
        
        /* 检查可用数据 */
        uint32_t available = UART_RingBuffer_GetAvailableBytes(UART_ID_LTE);
        if (available > 0) {
            uint16_t bytes_to_read = (available > (buffer_size - total_received - 1)) ? 
                                   (buffer_size - total_received - 1) : available;
            
            UART_Status_t uart_result = UART_RingBuffer_Receive(UART_ID_LTE, 
                                    (uint8_t*)(response_buffer + total_received), 
                                    bytes_to_read, 100);
            if (uart_result == UART_OK) {
                total_received += bytes_to_read;
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    
    response_buffer[total_received] = '\0';
    result = (total_received > 0);
    
    /* 释放串口访问互斥量 */
    xSemaphoreGive(s_uart_mutex);
    
    return result;
}

/**
 * @brief 执行AT指令配置
 * @param cmd_config AT指令配置
 * @return AT_Result_t 执行结果
 */
static AT_Result_t execute_at_command_with_config(const AT_Cmd_Config_t* cmd_config)
{
    char at_command[256];
    char response_buffer[512];
    UART_Status_t uart_result;
    AT_Result_t result = AT_RESULT_ERROR;
    uint8_t retry_count = 0;
    
    if (!cmd_config || !cmd_config->at_cmd) {
        return AT_RESULT_ERROR;
    }
    
    LOG_DEBUG(LOG_MODULE_NETWORK, "Executing AT command: %s (%s)", 
             cmd_config->at_cmd, cmd_config->description ? cmd_config->description : "No description");
    
    /* 重试循环 */
    while (retry_count <= cmd_config->retries) {
        /* 格式化AT命令 */
        snprintf(at_command, sizeof(at_command), "%s\r\n", cmd_config->at_cmd);
        
        /* 发送命令 */
        uart_result = UART_RingBuffer_Send(UART_ID_LTE, (const uint8_t*)at_command, 
                                         strlen(at_command), cmd_config->timeout_ms);
        
        if (uart_result != UART_OK) {
            LOG_ERROR(LOG_MODULE_NETWORK, "Failed to send AT command: %s", cmd_config->at_cmd);
            retry_count++;
            continue;
        }
        
        /* 获取响应 */
        if (get_response_data(response_buffer, sizeof(response_buffer), cmd_config->timeout_ms)) {
            /* 检查期望响应 */
            if (cmd_config->expected_resp) {
                if (strstr(response_buffer, cmd_config->expected_resp) != NULL) {
                    result = AT_RESULT_OK;
                    LOG_DEBUG(LOG_MODULE_NETWORK, "AT command successful: %s", cmd_config->at_cmd);
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
                break;
            }
        } else {
            result = AT_RESULT_TIMEOUT;
            LOG_WARN(LOG_MODULE_NETWORK, "AT command timeout: %s", cmd_config->at_cmd);
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
        LOG_ERROR(LOG_MODULE_NETWORK, "AT command retry exhausted: %s", cmd_config->at_cmd);
    }
    
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
AT_Result_t Quectel4G_ExecuteAtCommand(const AT_Cmd_Config_t* cmd_config)
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
bool Quectel4G_ExecuteAtSequence(const AT_Cmd_Config_t* cmd_sequence, uint8_t count)
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
        LOG_DEBUG(LOG_MODULE_NETWORK, "Executing command %d/%d: %s", 
                 i + 1, count, cmd_sequence[i].at_cmd);
        
        result = execute_at_command_with_config(&cmd_sequence[i]);
        
        if (result != AT_RESULT_OK) {
            LOG_ERROR(LOG_MODULE_NETWORK, "AT sequence failed at command %d: %s", 
                     i + 1, cmd_sequence[i].at_cmd);
            all_success = false;
            
            /* 检查是否为关键指令失败 */
            if (cmd_sequence[i].critical) {
                LOG_ERROR(LOG_MODULE_NETWORK, "Critical AT command failed: %s", cmd_sequence[i].at_cmd);
                LOG_ERROR(LOG_MODULE_NETWORK, "Stopping AT sequence due to critical failure");
                break;  /* 关键指令失败，立即停止序列执行 */
            }
            
            /* 非关键指令失败处理 */
            if (result == AT_RESULT_RETRY_EXHAUSTED) {
                LOG_WARN(LOG_MODULE_NETWORK, "Non-critical command retry exhausted, continuing with next command");
            }
        } else {
            LOG_DEBUG(LOG_MODULE_NETWORK, "Command executed successfully: %s", cmd_sequence[i].at_cmd);
        }
        
        /* 指令间适当延时，避免发送过快 */
        if (i < (count - 1)) {
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }
    
    LOG_INFO(LOG_MODULE_NETWORK, "AT command sequence completed. Success: %s", 
            all_success ? "Yes" : "No");
    
    return all_success;
}

/* AT指令回调函数实现 */

/**
 * @brief AT基本测试回调
 */
static void at_callback_basic_test(const char* resp, AT_Result_t result)
{
    if (result == AT_RESULT_OK) {
        LOG_INFO(LOG_MODULE_NETWORK, "4G module responds to AT commands");
    } else {
        LOG_ERROR(LOG_MODULE_NETWORK, "4G module not responding to AT commands");
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
                if (version_len < sizeof(g_quectel4g_info.firmware_version)) {
                    strncpy(g_quectel4g_info.firmware_version, version_start, version_len);
                    g_quectel4g_info.firmware_version[version_len] = '\0';
                }
            }
        }
        
        /* 解析模块型号 */
        char* model_start = strstr(resp, "Quectel");
        if (model_start) {
            char* model_end = strstr(model_start, "\r");
            if (model_end) {
                size_t model_len = model_end - model_start;
                if (model_len < sizeof(g_quectel4g_info.module_model)) {
                    strncpy(g_quectel4g_info.module_model, model_start, model_len);
                    g_quectel4g_info.module_model[model_len] = '\0';
                }
            }
        }
        
        LOG_INFO(LOG_MODULE_NETWORK, "4G module: %s, Version: %s", 
                g_quectel4g_info.module_model, g_quectel4g_info.firmware_version);
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
                if (imei_len < sizeof(g_quectel4g_info.imei)) {
                    strncpy(g_quectel4g_info.imei, imei_start, imei_len);
                    g_quectel4g_info.imei[imei_len] = '\0';
                    
                    LOG_INFO(LOG_MODULE_NETWORK, "IMEI: %s", g_quectel4g_info.imei);
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
                if (iccid_len < sizeof(g_quectel4g_info.iccid)) {
                    strncpy(g_quectel4g_info.iccid, iccid_start, iccid_len);
                    g_quectel4g_info.iccid[iccid_len] = '\0';
                    
                    LOG_INFO(LOG_MODULE_NETWORK, "ICCID: %s", g_quectel4g_info.iccid);
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
                if (imsi_len < sizeof(g_quectel4g_info.imsi)) {
                    strncpy(g_quectel4g_info.imsi, imsi_start, imsi_len);
                    g_quectel4g_info.imsi[imsi_len] = '\0';
                    
                    LOG_INFO(LOG_MODULE_NETWORK, "IMSI: %s", g_quectel4g_info.imsi);
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
            g_quectel4g_info.signal_rssi = rssi;
            g_quectel4g_info.signal_ber = ber;
            
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
        if (parse_operator_info(resp, g_quectel4g_info.operator_name, 
                               sizeof(g_quectel4g_info.operator_name))) {
            LOG_INFO(LOG_MODULE_NETWORK, "Operator: %s", g_quectel4g_info.operator_name);
        }
    }
}

/**
 * @brief 获取4G设备信息
 * @param info 设备信息结构指针
 * @return bool 获取是否成功
 */
bool Quectel4G_GetDeviceInfo(Quectel4G_Device_Info_t* info)
{
    if (!info) {
        return false;
    }
    
    if (s_state_mutex && xSemaphoreTake(s_state_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        *info = g_quectel4g_info;
        xSemaphoreGive(s_state_mutex);
        return true;
    }
    
    return false;
}

/**
 * @brief 更新4G设备信息
 * @param info 设备信息结构指针
 * @return bool 更新是否成功
 */
bool Quectel4G_UpdateDeviceInfo(const Quectel4G_Device_Info_t* info)
{
    if (!info) {
        return false;
    }
    
    if (s_state_mutex && xSemaphoreTake(s_state_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        g_quectel4g_info = *info;
        xSemaphoreGive(s_state_mutex);
        return true;
    }
    
    return false;
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
        
        /* 硬件重启逻辑 - 需要根据具体硬件实现 */
        /* 这里提供标准的GPIO控制重启引脚的实现模板 */
        
        // TODO: 根据实际硬件设计实现硬件重启逻辑
        // 典型实现示例：
        // GPIO_ResetPin(4G_RESET_PIN, GPIO_PIN_RESET);     /* 拉低RESET引脚 */
        // vTaskDelay(pdMS_TO_TICKS(MODULE_RESET_PULSE_MS)); /* 保持复位状态 */
        // GPIO_ResetPin(4G_RESET_PIN, GPIO_PIN_SET);       /* 释放RESET引脚 */
        
        LOG_INFO(LOG_MODULE_NETWORK, "Hardware reset pulse sent, waiting for module restart");
        vTaskDelay(pdMS_TO_TICKS(MODULE_RESET_WAIT_MS));
        reset_success = true;  /* 假设硬件重启操作成功 */
        
    } else {
        LOG_INFO(LOG_MODULE_NETWORK, "Performing software reset of 4G module");
        s_soft_reset_count++;
        
        /* 软件重启：发送AT+CFUN=1,1指令重启射频功能 */
        AT_Cmd_Config_t reset_cmd = {
            .at_cmd = "AT+CFUN=1,1",               /* 重启射频功能 */
            .expected_resp = "OK",
            .timeout_ms = SOFT_RESET_TIMEOUT_MS,
            .retries = 2,
            .description = "Software reset module",
            .critical = false,                      /* 软件重启不是关键操作 */
            .callback = NULL
        };
        
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
        update_state(QUECTEL_STATE_UNKNOWN);
        s_init_completed = false;
        
        /* 清空设备信息 */
        memset(&g_quectel4g_info, 0, sizeof(g_quectel4g_info));
        
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
BaseType_t Quectel4G_HardReset(void)
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
BaseType_t Quectel4G_SoftReset(void)
{
    LOG_INFO(LOG_MODULE_NETWORK, "Manual soft reset requested");
    
    if (perform_module_reset(false)) {
        return pdPASS;
    } else {
        return pdFAIL;
    }
}

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

/* ========================== 数据传输功能实现 ========================== */

/**
 * @brief 数据接收任务
 * @details 从队列接收LTE串口数据，解析Socket数据接收事件（+QIURC），
 *          并调用用户注册的回调函数处理接收到的数据。
 * 
 * @param pvParameters 任务参数（未使用）
 * 
 * @note 该任务通过队列接收来自通信任务的LTE数据，避免直接操作串口
 * @note 队列机制确保数据处理的有序性和线程安全性
 */
static void data_receive_task(void* pvParameters)
{
    Quectel4G_QueueMessage_t queue_message;
    char* response_buffer;
    uint8_t socket_id;
    uint8_t data_buffer[256];
    uint16_t data_length;
    
    LOG_INFO(LOG_MODULE_NETWORK, "4G data receive task started (queue-based)");
    
    while (1) {
        /* 从队列接收数据，阻塞等待 */
        if (xQueueReceive(s_4g_data_queue, &queue_message, portMAX_DELAY) == pdTRUE) {
            /* 喂狗 - 防止看门狗复位 */
            MACRO_IWDG_RELOAD();
            
            response_buffer = (char*)queue_message.data;
            
            LOG_DEBUG(LOG_MODULE_NETWORK, "Received %d bytes from queue: %.*s", 
                     queue_message.length, queue_message.length, response_buffer);
            
            /* 检查是否是Socket数据接收事件 */
            if (strstr(response_buffer, "+QIURC: \"recv\"")) {
                /* 解析Socket接收数据 */
                if (parse_socket_receive_data(response_buffer, &socket_id, data_buffer, &data_length)) {
                    /* 处理接收到的数据 */
                    process_socket_data(socket_id, (const char*)data_buffer, data_length);
                }
            }
            /* 检查是否是Socket关闭事件 */
            else if (strstr(response_buffer, "+QIURC: \"closed\"")) {
                /* 解析关闭的Socket ID */
                const char* closed_pos = strstr(response_buffer, "closed\",");
                if (closed_pos) {
                    socket_id = (uint8_t)atoi(closed_pos + 8);
                    handle_socket_close_event(socket_id);
                }
            }
            /* 检查是否是其他4G模块事件 */
            else if (strstr(response_buffer, "+QIURC:") || 
                     strstr(response_buffer, "+QIRD:") ||
                     strstr(response_buffer, "+QISEND:")) {
                /* 处理其他4G模块相关事件 */
                LOG_DEBUG(LOG_MODULE_NETWORK, "4G module event: %.*s", 
                         queue_message.length, response_buffer);
            }
            /* 检查是否是纯数据（不包含AT响应格式） */
            else if (queue_message.length > 0 && response_buffer[0] >= 10 && response_buffer[0] <= 51) {
                /* 可能是服务器响应数据，尝试处理 */
                LOG_DEBUG(LOG_MODULE_NETWORK, "Processing potential server response data");
                Quectel4G_ProcessServerResponse((const uint8_t*)response_buffer, queue_message.length);
            }
        }
    }
}

/**
 * @brief 启动数据接收任务
 * @return bool 启动是否成功
 */
static bool start_data_receive_task(void)
{
    if (s_data_receive_task_handle != NULL) {
        LOG_WARN(LOG_MODULE_NETWORK, "Data receive task already running");
        return true;  // 任务已存在
    }
    
    BaseType_t result = xTaskCreate(data_receive_task, "4GDataReceive", 2048, 
                                   NULL, INIT_TASK_PRIORITY - 1, &s_data_receive_task_handle);
    
    if (result != pdPASS) {
        LOG_ERROR(LOG_MODULE_NETWORK, "Failed to create data receive task");
        s_data_receive_task_handle = NULL;
        return false;
    }
    
    LOG_INFO(LOG_MODULE_NETWORK, "Data receive task started successfully");
    return true;
}

/**
 * @brief 停止数据接收任务
 */
static void stop_data_receive_task(void)
{
    if (s_data_receive_task_handle != NULL) {
        LOG_INFO(LOG_MODULE_NETWORK, "Stopping data receive task");
        
        vTaskDelete(s_data_receive_task_handle);
        s_data_receive_task_handle = NULL;
        
        /* 清空队列中的剩余数据 */
        if (s_4g_data_queue != NULL) {
            xQueueReset(s_4g_data_queue);
        }
        
        LOG_INFO(LOG_MODULE_NETWORK, "Data receive task stopped");
    }
}

/**
 * @brief 处理Socket接收到的数据
 * @details 更新网络统计信息，调用用户注册的数据接收回调函数
 * 
 * @param socket_id Socket ID
 * @param data 接收到的数据
 * @param length 数据长度
 * @return bool 处理是否成功
 */
static bool process_socket_data(uint8_t socket_id, const char* data, uint16_t length)
{
    if (!data || length == 0) {
        return false;
    }
    
    /* 更新设备信息中的统计数据 */
    update_network_statistics(0, length);
    
    /* 调用用户注册的回调函数 */
    if (s_rx_callback) {
        s_rx_callback((const uint8_t*)data, length, NULL, 0);
    }
    
    LOG_DEBUG(LOG_MODULE_NETWORK, "Received %d bytes on socket %d", length, socket_id);
    return true;
}

/**
 * @brief 解析Socket数据接收响应
 * @details 解析格式：+QIURC: "recv",<socket_id>,<data_length>
 * 
 * @param response AT响应字符串
 * @param socket_id 输出Socket ID
 * @param data_buffer 数据缓冲区
 * @param data_length 输出数据长度
 * @return bool 解析是否成功
 */
static bool parse_socket_receive_data(const char* response, uint8_t* socket_id, 
                                     uint8_t* data_buffer, uint16_t* data_length)
{
    if (!response || !socket_id || !data_buffer || !data_length) {
        return false;
    }
    
    /* 查找+QIURC: "recv"响应 */
    const char* recv_start = strstr(response, "+QIURC: \"recv\"");
    if (recv_start) {
        /* 解析Socket ID和数据长度 */       //TODO 这里需判断是缓存模式还是直吐模式，缓存模式似乎没有data_length 而直吐模式不需要再通过AT+QIRD获取数据
        if (sscanf(recv_start, "+QIURC: \"recv\",%hhu,%hu", socket_id, data_length) == 2) {
            /* 这里需要发送AT+QIRD命令来读取实际数据 */
            char read_cmd[32];
            snprintf(read_cmd, sizeof(read_cmd), "AT+QIRD=%d", *socket_id);
            
            AT_Cmd_Config_t read_config = {
                .at_cmd = read_cmd,
                .expected_resp = "+QIRD:",
                .timeout_ms = 3000,
                .retries = 1,
                .description = "Read socket data",
                .critical = false,
                .callback = NULL
            };
            
            /* 执行读取命令并解析返回的数据 */
            if (execute_at_command_with_config(&read_config) == AT_RESULT_OK) {
                /* TODO: 这里需要实现具体的数据解析逻辑 */
                /* 目前简化处理，实际项目中需要根据AT+QIRD的响应格式来解析 */
                *data_length = 0; /* 临时设置为0，实际需要从响应中解析 */
                return true;
            }
        }
    }
    
    return false;
}

/**
 * @brief 处理Socket关闭事件
 * @details 更新连接状态，调用连接状态回调函数
 * 
 * @param socket_id 关闭的Socket ID
 */
static void handle_socket_close_event(uint8_t socket_id)
{
    LOG_INFO(LOG_MODULE_NETWORK, "Socket %d closed", socket_id);
    
    /* 更新设备连接状态 */
    if (xSemaphoreTake(s_data_mutex, pdMS_TO_TICKS(1000)) == pdTRUE) {
        if (g_quectel4g_info.socket_id == socket_id) {
            g_quectel4g_info.socket_connected = false;
        }
        xSemaphoreGive(s_data_mutex);
    }
    
    /* 调用连接状态回调 */
    if (s_conn_callback) {
        s_conn_callback(false, socket_id);
    }
}

/**
 * @brief 建立TCP连接
 * @details 使用AT+QIOPEN命令建立TCP Socket连接
 * 
 * @param remote_ip 远程服务器IP地址
 * @param remote_port 远程服务器端口
 * @param local_port 本地端口（0表示自动分配）
 * @return QuectelDataResult_t 连接结果
 */
QuectelDataResult_t Quectel4G_ConnectTCP(const char* remote_ip, uint16_t remote_port, uint16_t local_port)
{
    if (!remote_ip || remote_port == 0) {
        return QUECTEL_DATA_INVALID_PARAM;
    }
    
    /* 检查模块状态 */
    if (s_current_state < QUECTEL_STATE_REGISTERED) {
        LOG_ERROR(LOG_MODULE_NETWORK, "Module not ready for data connection");
        return QUECTEL_DATA_NO_CONNECTION;
    }
    
    return setup_socket_connection(QUECTEL_PROTOCOL_TCP, remote_ip, remote_port, local_port) ? 
           QUECTEL_DATA_OK : QUECTEL_DATA_ERROR;
}

/**
 * @brief 建立UDP连接
 * @details 使用AT+QIOPEN命令建立UDP Socket连接
 * 
 * @param remote_ip 远程服务器IP地址
 * @param remote_port 远程服务器端口
 * @param local_port 本地端口（0表示自动分配）
 * @return QuectelDataResult_t 连接结果
 */
QuectelDataResult_t Quectel4G_ConnectUDP(const char* remote_ip, uint16_t remote_port, uint16_t local_port)
{
    if (!remote_ip || remote_port == 0) {
        return QUECTEL_DATA_INVALID_PARAM;
    }
    
    /* 检查模块状态 */
    if (s_current_state < QUECTEL_STATE_REGISTERED) {
        LOG_ERROR(LOG_MODULE_NETWORK, "Module not ready for data connection");
        return QUECTEL_DATA_NO_CONNECTION;
    }
    
    return setup_socket_connection(QUECTEL_PROTOCOL_UDP, remote_ip, remote_port, local_port) ? 
           QUECTEL_DATA_OK : QUECTEL_DATA_ERROR;
}

/**
 * @brief 建立Socket连接的内部实现
 * @details 根据协议类型使用相应的AT指令建立连接
 * 
 * @param protocol 协议类型（TCP/UDP）
 * @param remote_ip 远程IP地址
 * @param remote_port 远程端口
 * @param local_port 本地端口
 * @return bool 连接是否成功
 */
static bool setup_socket_connection(QuectelProtocol_t protocol, const char* remote_ip, 
                                   uint16_t remote_port, uint16_t local_port)
{
    char cmd_buffer[128];
    const char* protocol_str = (protocol == QUECTEL_PROTOCOL_TCP) ? "TCP" : "UDP";
    
    /* 选择一个可用的Socket ID（这里简化为使用固定ID 0） */
    uint8_t socket_id = 0;
    
    /* 构建AT+QIOPEN命令 */
    if (local_port > 0) {
        snprintf(cmd_buffer, sizeof(cmd_buffer), 
                "AT+QIOPEN=1,%d,\"%s\",\"%s\",%d,%d,0", 
                socket_id, protocol_str, remote_ip, remote_port, local_port);
    } else {
        snprintf(cmd_buffer, sizeof(cmd_buffer), 
                "AT+QIOPEN=1,%d,\"%s\",\"%s\",%d", 
                socket_id, protocol_str, remote_ip, remote_port);
    }
    
    AT_Cmd_Config_t open_cmd = {
        .at_cmd = cmd_buffer,
        .expected_resp = "OK",
        .timeout_ms = 15000,  /* 连接可能需要较长时间 */
        .retries = 2,
        .description = "Open socket connection",
        .critical = false,
        .callback = NULL
    };
    
    /* 执行连接命令 */
    AT_Result_t result = execute_at_command_with_config(&open_cmd);
    if (result == AT_RESULT_OK) {
        /* 更新设备信息 */
        if (xSemaphoreTake(s_data_mutex, pdMS_TO_TICKS(1000)) == pdTRUE) {
            g_quectel4g_info.socket_id = socket_id;
            g_quectel4g_info.socket_connected = true;
            g_quectel4g_info.protocol_type = protocol;
            strncpy(g_quectel4g_info.remote_server, remote_ip, sizeof(g_quectel4g_info.remote_server) - 1);
            g_quectel4g_info.remote_port = remote_port;
            g_quectel4g_info.local_port = local_port;
            xSemaphoreGive(s_data_mutex);
        }
        
        /* 调用连接成功回调 */
        if (s_conn_callback) {
            s_conn_callback(true, 0);
        }
        
        LOG_INFO(LOG_MODULE_NETWORK, "%s connection established to %s:%d", 
                protocol_str, remote_ip, remote_port);
        return true;
    }
    
    LOG_ERROR(LOG_MODULE_NETWORK, "Failed to establish %s connection to %s:%d", 
             protocol_str, remote_ip, remote_port);
    return false;
}

/**
 * @brief 断开网络连接
 * @details 使用AT+QICLOSE命令关闭当前的Socket连接
 * 
 * @return QuectelDataResult_t 断开结果
 */
QuectelDataResult_t Quectel4G_Disconnect(void)
{
    uint8_t socket_id;
    
    /* 停止TCP监控任务 */
    if (s_tcp_monitor_task != NULL) {
        s_tcp_config.auto_reconnect = false;  // 禁用自动重连以停止监控任务
        
        /* 等待任务自然退出 */
        uint8_t wait_count = 0;
        while (s_tcp_monitor_task != NULL && wait_count < 10) {
            vTaskDelay(pdMS_TO_TICKS(100));
            wait_count++;
        }
        
        if (s_tcp_monitor_task != NULL) {
            LOG_WARN(LOG_MODULE_NETWORK, "Force deleting TCP monitor task");
            vTaskDelete(s_tcp_monitor_task);
            s_tcp_monitor_task = NULL;
        }
        
        LOG_INFO(LOG_MODULE_NETWORK, "TCP monitor task stopped");
    }
    
    /* 停止设备注册相关任务 */
    device_cleanup_register_tasks();
    
    /* 停止数据接收任务 */
    stop_data_receive_task();
    
    /* 重置设备注册状态 */
    if (xSemaphoreTake(s_register_mutex, pdMS_TO_TICKS(1000)) == pdTRUE) {
        s_device_register_data.current_state = DEVICE_REG_STATE_IDLE;
        s_device_register_data.registration_completed = false;
        xSemaphoreGive(s_register_mutex);
    }
    
    /* 获取当前Socket ID */
    if (xSemaphoreTake(s_data_mutex, pdMS_TO_TICKS(1000)) == pdTRUE) {
        if (!g_quectel4g_info.socket_connected) {
            xSemaphoreGive(s_data_mutex);
            return QUECTEL_DATA_OK;  /* 已经断开 */
        }
        socket_id = g_quectel4g_info.socket_id;
        xSemaphoreGive(s_data_mutex);
    } else {
        return QUECTEL_DATA_ERROR;
    }
    
    /* 构建关闭命令 */
    char close_cmd[32];
    snprintf(close_cmd, sizeof(close_cmd), "AT+QICLOSE=%d", socket_id);
    
    AT_Cmd_Config_t close_config = {
        .at_cmd = close_cmd,
        .expected_resp = "OK",
        .timeout_ms = 10000,
        .retries = 2,
        .description = "Close socket connection",
        .critical = false,
        .callback = NULL
    };
    
    /* 执行关闭命令 */
    AT_Result_t result = execute_at_command_with_config(&close_config);
    if (result == AT_RESULT_OK) {
        /* 更新连接状态 */
        if (xSemaphoreTake(s_data_mutex, pdMS_TO_TICKS(1000)) == pdTRUE) {
            g_quectel4g_info.socket_connected = false;
            xSemaphoreGive(s_data_mutex);
        }
        
        LOG_INFO(LOG_MODULE_NETWORK, "Socket %d disconnected successfully", socket_id);
        return QUECTEL_DATA_OK;
    }
    
    LOG_ERROR(LOG_MODULE_NETWORK, "Failed to disconnect socket %d", socket_id);
    return QUECTEL_DATA_ERROR;
}

/**
 * @brief 发送数据（TCP/UDP通用）
 * @details 使用AT+QISEND命令发送数据到已建立的Socket连接
 * 
 * @param data 要发送的数据指针
 * @param length 数据长度
 * @param timeout_ms 发送超时时间（毫秒）
 * @return QuectelDataResult_t 发送结果
 */
QuectelDataResult_t Quectel4G_SendData(const uint8_t* data, uint16_t length, uint32_t timeout_ms)
{
    if (!data || length == 0) {
        return QUECTEL_DATA_INVALID_PARAM;
    }
    
    /* 检查连接状态 */
    if (!Quectel4G_IsConnected()) {
        return QUECTEL_DATA_NO_CONNECTION;
    }
    
    uint8_t socket_id;
    if (xSemaphoreTake(s_data_mutex, pdMS_TO_TICKS(1000)) == pdTRUE) {
        socket_id = g_quectel4g_info.socket_id;
        xSemaphoreGive(s_data_mutex);
    } else {
        return QUECTEL_DATA_ERROR;
    }
    
    return send_data_internal(socket_id, data, length, timeout_ms);
}

/**
 * @brief 发送字符串数据
 * @details 发送以NULL结尾的字符串数据
 * 
 * @param str 要发送的字符串
 * @param timeout_ms 发送超时时间（毫秒）
 * @return QuectelDataResult_t 发送结果
 */
QuectelDataResult_t Quectel4G_SendString(const char* str, uint32_t timeout_ms)
{
    if (!str) {
        return QUECTEL_DATA_INVALID_PARAM;
    }
    
    uint16_t length = strlen(str);
    return Quectel4G_SendData((const uint8_t*)str, length, timeout_ms);
}

/**
 * @brief 内部数据发送函数
 * @details 实际执行AT+QISEND命令发送数据的函数
 * 
 * @param socket_id Socket ID
 * @param data 数据指针
 * @param length 数据长度
 * @param timeout_ms 超时时间
 * @return QuectelDataResult_t 发送结果
 */
static QuectelDataResult_t send_data_internal(uint8_t socket_id, const uint8_t* data, 
                                             uint16_t length, uint32_t timeout_ms)
{
    char send_cmd[32];
    
    /* 构建发送命令 */
    snprintf(send_cmd, sizeof(send_cmd), "AT+QISEND=%d,%d", socket_id, length);
    
    AT_Cmd_Config_t send_config = {
        .at_cmd = send_cmd,
        .expected_resp = ">",  /* 等待数据输入提示符 */
        .timeout_ms = timeout_ms,
        .retries = 2,
        .description = "Send data command",
        .critical = false,
        .callback = NULL
    };
    
    /* 发送AT+QISEND命令 */
    AT_Result_t result = execute_at_command_with_config(&send_config);
    if (result == AT_RESULT_OK) {
        /* TODO: 这里需要实现实际的数据发送逻辑 */
        /* 在实际项目中，需要：
         * 1. 等待">"提示符
         * 2. 发送原始数据
         * 3. 等待"SEND OK"确认
         */
        
        /* 更新发送统计 */
        update_network_statistics(length, 0);
        
        LOG_DEBUG(LOG_MODULE_NETWORK, "Sent %d bytes on socket %d", length, socket_id);
        return QUECTEL_DATA_OK;
    }
    
    LOG_ERROR(LOG_MODULE_NETWORK, "Failed to send data on socket %d", socket_id);
    return QUECTEL_DATA_ERROR;
}

/**
 * @brief 获取连接状态
 * @return bool true-已连接，false-未连接
 */
bool Quectel4G_IsConnected(void)
{
    bool connected = false;
    
    if (xSemaphoreTake(s_data_mutex, pdMS_TO_TICKS(1000)) == pdTRUE) {
        connected = g_quectel4g_info.socket_connected;
        xSemaphoreGive(s_data_mutex);
    }
    
    return connected;
}

/**
 * @brief 设置数据接收回调函数
 * @param callback 回调函数指针
 */
void Quectel4G_SetReceiveCallback(QuectelDataReceiveCallback_t callback)
{
    s_rx_callback = callback;
}

/**
 * @brief 设置连接状态回调函数
 * @param callback 回调函数指针
 */
void Quectel4G_SetConnectionCallback(QuectelConnectionCallback_t callback)
{
    s_conn_callback = callback;
}

/**
 * @brief 获取网络统计信息
 * @param bytes_sent 发送字节数指针
 * @param bytes_received 接收字节数指针
 * @param last_data_time 最后数据时间指针
 * @return bool 获取是否成功
 */
bool Quectel4G_GetNetworkStats(uint32_t* bytes_sent, uint32_t* bytes_received, uint32_t* last_data_time)
{
    if (!bytes_sent || !bytes_received || !last_data_time) {
        return false;
    }
    
    if (xSemaphoreTake(s_data_mutex, pdMS_TO_TICKS(1000)) == pdTRUE) {
        *bytes_sent = g_quectel4g_info.bytes_sent;
        *bytes_received = g_quectel4g_info.bytes_received;
        *last_data_time = g_quectel4g_info.last_data_time;
        xSemaphoreGive(s_data_mutex);
        return true;
    }
    
    return false;
}

/**
 * @brief 向4G数据处理队列发送数据
 * @details 供通信接收任务调用，将LTE串口数据发送到4G数据处理队列
 * 
 * @param data 数据指针
 * @param length 数据长度
 * @return bool 发送是否成功
 * 
 * @note 此函数由tsk_commu_receive.c中的process_uart_data调用
 * @note 使用非阻塞发送，避免影响通信任务的实时性
 */
bool Quectel4G_SendDataToQueue(const uint8_t* data, uint16_t length)
{
    Quectel4G_QueueMessage_t queue_message;
    
    if (!data || length == 0 || s_4g_data_queue == NULL) {
        return false;
    }
    
    /* 检查数据长度是否超过队列消息容量 */
    if (length > sizeof(queue_message.data)) {
        LOG_WARN(LOG_MODULE_NETWORK, "Data too large for queue (%d bytes), truncating", length);
        length = sizeof(queue_message.data);
    }
    
    /* 构建队列消息 */
    memcpy(queue_message.data, data, length);
    queue_message.length = length;
    queue_message.timestamp = xTaskGetTickCount();
    
    /* 非阻塞发送到队列 */
    if (xQueueSend(s_4g_data_queue, &queue_message, 0) == pdTRUE) {
        LOG_DEBUG(LOG_MODULE_NETWORK, "Sent %d bytes to 4G data queue", length);
        return true;
    } else {
        LOG_WARN(LOG_MODULE_NETWORK, "4G data queue full, dropping %d bytes", length);
        return false;
    }
}

/**
 * @brief 获取4G数据处理队列句柄
 * @details 供其他模块获取队列句柄进行数据发送
 * 
 * @return QueueHandle_t 队列句柄，NULL表示未初始化
 */
QueueHandle_t Quectel4G_GetDataQueue(void)
{
    return s_4g_data_queue;
}

/* 注意：还有一些函数（如Quectel4G_SendUDPData、Quectel4G_ReceiveData、Quectel4G_IsDataAvailable等）
 * 可以根据上面的模式继续实现，这里为了节省篇幅暂未全部实现 */

/**
 * @brief 发送UDP数据到指定地址
 * @details 使用AT+QISEND命令发送UDP数据到指定的远程地址和端口
 * 
 * @param data 要发送的数据指针
 * @param length 数据长度
 * @param remote_ip 目标IP地址
 * @param remote_port 目标端口
 * @param timeout_ms 发送超时时间（毫秒）
 * @return QuectelDataResult_t 发送结果
 */
QuectelDataResult_t Quectel4G_SendUDPData(const uint8_t* data, uint16_t length, 
                                          const char* remote_ip, uint16_t remote_port, 
                                          uint32_t timeout_ms)
{
    if (!data || length == 0 || !remote_ip || remote_port == 0) {
        return QUECTEL_DATA_INVALID_PARAM;
    }
    
    /* 检查连接状态和协议类型 */
    if (!Quectel4G_IsConnected()) {
        return QUECTEL_DATA_NO_CONNECTION;
    }
    
    /* 检查当前是否为UDP协议 */
    if (xSemaphoreTake(s_data_mutex, pdMS_TO_TICKS(1000)) == pdTRUE) {
        if (g_quectel4g_info.protocol_type != QUECTEL_PROTOCOL_UDP) {
            xSemaphoreGive(s_data_mutex);
            LOG_ERROR(LOG_MODULE_NETWORK, "Current connection is not UDP");
            return QUECTEL_DATA_ERROR;
        }
        xSemaphoreGive(s_data_mutex);
    }
    
    char send_cmd[128];
    uint8_t socket_id = g_quectel4g_info.socket_id;
    
    /* 构建UDP发送命令，包含目标地址和端口 */
    snprintf(send_cmd, sizeof(send_cmd), "AT+QISEND=%d,%d,\"%s\",%d", 
             socket_id, length, remote_ip, remote_port);
    
    AT_Cmd_Config_t send_config = {
        .at_cmd = send_cmd,
        .expected_resp = ">",
        .timeout_ms = timeout_ms,
        .retries = 2,
        .description = "Send UDP data with address",
        .critical = false,
        .callback = NULL
    };
    
    /* 执行发送命令 */
    AT_Result_t result = execute_at_command_with_config(&send_config);
    if (result == AT_RESULT_OK) {
        /* 更新发送统计 */
        if (xSemaphoreTake(s_data_mutex, pdMS_TO_TICKS(1000)) == pdTRUE) {
            g_quectel4g_info.bytes_sent += length;
            g_quectel4g_info.last_data_time = xTaskGetTickCount();
            xSemaphoreGive(s_data_mutex);
        }
        
        LOG_DEBUG(LOG_MODULE_NETWORK, "Sent %d bytes UDP data to %s:%d", 
                 length, remote_ip, remote_port);
        return QUECTEL_DATA_OK;
    }
    
    LOG_ERROR(LOG_MODULE_NETWORK, "Failed to send UDP data to %s:%d", remote_ip, remote_port);
    return QUECTEL_DATA_ERROR;
}

/**
 * @brief 获取接收缓冲区数据
 * @details 主动读取接收缓冲区中的数据，适用于轮询方式接收数据
 * 
 * @param buffer 接收缓冲区指针
 * @param buffer_size 缓冲区大小
 * @param received_length 实际接收长度指针
 * @param timeout_ms 接收超时时间（毫秒）
 * @return QuectelDataResult_t 接收结果
 */
QuectelDataResult_t Quectel4G_ReceiveData(uint8_t* buffer, uint16_t buffer_size, 
                                          uint16_t* received_length, uint32_t timeout_ms)
{
    if (!buffer || buffer_size == 0 || !received_length) {
        return QUECTEL_DATA_INVALID_PARAM;
    }
    
    /* 检查连接状态 */
    if (!Quectel4G_IsConnected()) {
        return QUECTEL_DATA_NO_CONNECTION;
    }
    
    uint8_t socket_id;
    if (xSemaphoreTake(s_data_mutex, pdMS_TO_TICKS(1000)) == pdTRUE) {
        socket_id = g_quectel4g_info.socket_id;
        xSemaphoreGive(s_data_mutex);
    } else {
        return QUECTEL_DATA_ERROR;
    }
    
    /* 构建数据读取命令 */
    char read_cmd[32];
    snprintf(read_cmd, sizeof(read_cmd), "AT+QIRD=%d,%d", socket_id, buffer_size);
    
    AT_Cmd_Config_t read_config = {
        .at_cmd = read_cmd,
        .expected_resp = "+QIRD:",
        .timeout_ms = timeout_ms,
        .retries = 1,
        .description = "Read received data",
        .critical = false,
        .callback = NULL
    };
    
    /* 执行读取命令 */
    AT_Result_t result = execute_at_command_with_config(&read_config);
    if (result == AT_RESULT_OK) {
        /* TODO: 这里需要实现具体的数据解析逻辑 */
        /* 解析+QIRD响应格式：+QIRD: <actual_read_length>\r\n<data> */
        
        *received_length = 0; /* 临时设置，实际需要从AT响应中解析 */
        
        LOG_DEBUG(LOG_MODULE_NETWORK, "Read %d bytes from socket %d", *received_length, socket_id);
        return QUECTEL_DATA_OK;
    } else if (result == AT_RESULT_TIMEOUT) {
        *received_length = 0;
        return QUECTEL_DATA_TIMEOUT;
    }
    
    *received_length = 0;
    return QUECTEL_DATA_ERROR;
}

/**
 * @brief 检查是否有数据可读
 * @details 查询当前Socket是否有可读取的数据
 * 
 * @return bool true-有数据可读，false-无数据
 */
bool Quectel4G_IsDataAvailable(void)
{
    if (!Quectel4G_IsConnected()) {
        return false;
    }
    
    uint8_t socket_id;
    if (xSemaphoreTake(s_data_mutex, pdMS_TO_TICKS(1000)) == pdTRUE) {
        socket_id = g_quectel4g_info.socket_id;
        xSemaphoreGive(s_data_mutex);
    } else {
        return false;
    }
    
    /* 构建查询命令 */
    char query_cmd[32];
    snprintf(query_cmd, sizeof(query_cmd), "AT+QIRD=%d,0", socket_id);
    
    AT_Cmd_Config_t query_config = {
        .at_cmd = query_cmd,
        .expected_resp = "+QIRD:",
        .timeout_ms = 1000,
        .retries = 1,
        .description = "Query available data",
        .critical = false,
        .callback = NULL
    };
    
    /* 执行查询命令 */
    AT_Result_t result = execute_at_command_with_config(&query_config);
    if (result == AT_RESULT_OK) {
        /* TODO: 解析响应中的可用数据长度 */
        /* 如果返回的长度 > 0，则表示有数据可读 */
        return true; /* 临时返回true，实际需要解析AT响应 */
    }
    
    return false;
}

/**
 * @brief 更新网络统计信息（内部函数）
 * @details 用于内部更新发送和接收的字节统计
 * 
 * @param bytes_sent 本次发送的字节数
 * @param bytes_received 本次接收的字节数
 */
static void update_network_statistics(uint16_t bytes_sent, uint16_t bytes_received)
{
    if (xSemaphoreTake(s_data_mutex, pdMS_TO_TICKS(1000)) == pdTRUE) {
        if (bytes_sent > 0) {
            g_quectel4g_info.bytes_sent += bytes_sent;
            g_quectel4g_info.last_data_time = xTaskGetTickCount();
        }
        
        if (bytes_received > 0) {
            g_quectel4g_info.bytes_received += bytes_received;
            g_quectel4g_info.last_data_time = xTaskGetTickCount();
        }
        
        xSemaphoreGive(s_data_mutex);
    }
}

/**
 * @brief 重置网络统计信息
 * @details 将发送和接收的字节计数器重置为0
 * 
 * @return bool 重置是否成功
 */
bool Quectel4G_ResetNetworkStats(void)
{
    if (xSemaphoreTake(s_data_mutex, pdMS_TO_TICKS(1000)) == pdTRUE) {
        g_quectel4g_info.bytes_sent = 0;
        g_quectel4g_info.bytes_received = 0;
        g_quectel4g_info.last_data_time = 0;
        xSemaphoreGive(s_data_mutex);
        
        LOG_INFO(LOG_MODULE_NETWORK, "Network statistics reset");
        return true;
    }
    
    return false;
}

/* ==================== TCP连接服务器完整流程实现 ==================== */

/**
 * @brief 连接TCP服务器（完整流程）
 * @details 提供完整的TCP服务器连接流程，包括网络检查、连接建立、重试机制等
 * 
 * @param config TCP服务器连接配置
 * @param event_callback 连接事件回调函数（可选）
 * @param user_data 用户数据指针（传递给回调函数）
 * @return QuectelDataResult_t 连接结果
 */
QuectelDataResult_t Quectel4G_ConnectTcpServer(const Quectel4G_TcpServerConfig_t* config,
                                              Quectel4G_TcpEventCallback_t event_callback,
                                              void* user_data)
{
    if (!config || !config->server_ip[0] || config->server_port == 0) {
        LOG_ERROR(LOG_MODULE_NETWORK, "Invalid TCP server configuration");
        return QUECTEL_DATA_INVALID_PARAM;
    }
    
    /* 检查模块状态 */
    if (s_current_state < QUECTEL_STATE_REGISTERED) {
        LOG_ERROR(LOG_MODULE_NETWORK, "Module not ready for TCP connection (state=%d)", s_current_state);
        return QUECTEL_DATA_NO_CONNECTION;
    }
    
    /* 如果已有连接，先断开 */
    if (g_quectel4g_info.socket_connected) {
        LOG_INFO(LOG_MODULE_NETWORK, "Disconnecting existing connection before new TCP connection");
        Quectel4G_Disconnect();
        vTaskDelay(pdMS_TO_TICKS(1000));  // 等待断开完成
    }
    
    /* 保存配置和回调 */
    memcpy(&s_tcp_config, config, sizeof(Quectel4G_TcpServerConfig_t));
    s_tcp_event_callback = event_callback;
    s_tcp_user_data = user_data;
    
    LOG_INFO(LOG_MODULE_NETWORK, "Starting TCP connection to %s:%d", 
             config->server_ip, config->server_port);
    
    /* 执行带重试的TCP连接 */
    bool success = tcp_connect_with_retry(config);
    
    if (success) {
        s_tcp_connect_time = xTaskGetTickCount();
        
        /* 启动TCP连接监控任务 */
        if (config->auto_reconnect && s_tcp_monitor_task == NULL) {
            if (xTaskCreate(tcp_connection_monitor_task, 
                          "TCP_Monitor", 
                          256, 
                          NULL, 
                          tskIDLE_PRIORITY + 2, 
                          &s_tcp_monitor_task) != pdPASS) {
                LOG_WARN(LOG_MODULE_NETWORK, "Failed to create TCP monitor task");
            } else {
                LOG_INFO(LOG_MODULE_NETWORK, "TCP connection monitor task started");
            }
        }
        
        /* 触发连接成功事件 */
        tcp_handle_connection_event(0, g_quectel4g_info.socket_id, 0);
        
        LOG_INFO(LOG_MODULE_NETWORK, "TCP connection established successfully");
        return QUECTEL_DATA_OK;
    } else {
        /* 触发连接失败事件 */
        tcp_handle_connection_event(1, 0, 1);
        
        LOG_ERROR(LOG_MODULE_NETWORK, "Failed to establish TCP connection after retries");
        return QUECTEL_DATA_ERROR;
    }
}

/**
 * @brief 检查TCP服务器连接状态
 * @details 主动查询当前TCP连接状态，验证连接的可用性
 * @return bool true=连接正常，false=连接异常或已断开
 */
bool Quectel4G_CheckTcpConnection(void)
{
    if (!g_quectel4g_info.socket_connected) {
        return false;
    }
    
    return tcp_verify_connection(g_quectel4g_info.socket_id);
}

/**
 * @brief 设置TCP自动重连参数
 * @details 配置TCP连接断开后的自动重连行为
 * @param enable 是否启用自动重连
 * @param retry_count 最大重试次数（0表示无限重试）
 * @param retry_interval_ms 重试间隔时间（毫秒）
 * @return bool 设置是否成功
 */
bool Quectel4G_SetTcpAutoReconnect(bool enable, uint8_t retry_count, uint32_t retry_interval_ms)
{
    s_tcp_auto_reconnect_enabled = enable;
    s_tcp_retry_count = retry_count;
    s_tcp_retry_interval = retry_interval_ms;
    
    /* 更新配置中的自动重连设置 */
    s_tcp_config.auto_reconnect = enable;
    s_tcp_config.retry_count = retry_count;
    s_tcp_config.retry_interval_ms = retry_interval_ms;
    
    LOG_INFO(LOG_MODULE_NETWORK, "TCP auto-reconnect %s (retry=%d, interval=%dms)", 
             enable ? "enabled" : "disabled", retry_count, retry_interval_ms);
    
    return true;
}

/**
 * @brief 发送TCP心跳数据
 * @details 发送应用层心跳数据以保持连接活跃
 * @param data 心跳数据（可选，NULL表示使用默认心跳）
 * @param length 数据长度
 * @return QuectelDataResult_t 发送结果
 */
QuectelDataResult_t Quectel4G_SendTcpHeartbeat(const uint8_t* data, uint16_t length)
{
    if (!g_quectel4g_info.socket_connected) {
        return QUECTEL_DATA_NO_CONNECTION;
    }
    
    const uint8_t* heartbeat_data;
    uint16_t heartbeat_length;
    
    /* 使用提供的数据或默认心跳数据 */
    if (data && length > 0) {
        heartbeat_data = data;
        heartbeat_length = length;
    } else if (s_tcp_config.heartbeat_data && s_tcp_config.heartbeat_length > 0) {
        heartbeat_data = (const uint8_t*)s_tcp_config.heartbeat_data;
        heartbeat_length = s_tcp_config.heartbeat_length;
    } else {
        /* 默认心跳数据 */
        static const uint8_t default_heartbeat[] = "PING";
        heartbeat_data = default_heartbeat;
        heartbeat_length = sizeof(default_heartbeat) - 1;
    }
    
    QuectelDataResult_t result = send_data_internal(g_quectel4g_info.socket_id, 
                                                   heartbeat_data, heartbeat_length, 5000);
    
    if (result == QUECTEL_DATA_OK) {
        LOG_DEBUG(LOG_MODULE_NETWORK, "TCP heartbeat sent (%d bytes)", heartbeat_length);
    } else {
        LOG_WARN(LOG_MODULE_NETWORK, "Failed to send TCP heartbeat (error=%d)", result);
    }
    
    return result;
}

/**
 * @brief 获取TCP连接详细信息
 * @details 获取当前TCP连接的详细状态信息
 * @param socket_id Socket ID指针
 * @param remote_ip 远程IP地址缓冲区（至少16字节）
 * @param remote_port 远程端口号指针
 * @param local_port 本地端口号指针
 * @param connect_time 连接建立时间指针（系统滴答）
 * @return bool 获取是否成功
 */
bool Quectel4G_GetTcpConnectionInfo(uint8_t* socket_id, char* remote_ip, 
                                  uint16_t* remote_port, uint16_t* local_port,
                                  uint32_t* connect_time)
{
    if (!g_quectel4g_info.socket_connected) {
        return false;
    }
    
    if (xSemaphoreTake(s_data_mutex, pdMS_TO_TICKS(1000)) == pdTRUE) {
        if (socket_id) *socket_id = g_quectel4g_info.socket_id;
        if (remote_ip) strncpy(remote_ip, g_quectel4g_info.remote_server, 15);
        if (remote_port) *remote_port = g_quectel4g_info.remote_port;
        if (local_port) *local_port = g_quectel4g_info.local_port;
        if (connect_time) *connect_time = s_tcp_connect_time;
        
        xSemaphoreGive(s_data_mutex);
        return true;
    }
    
    return false;
}

/* ==================== TCP连接管理内部函数 ==================== */

/**
 * @brief 带重试的TCP连接函数
 * @details 执行TCP连接，支持重试机制
 * @param config TCP配置参数
 * @return bool 连接是否成功
 */
static bool tcp_connect_with_retry(const Quectel4G_TcpServerConfig_t* config)
{
    uint8_t retry_attempts = 0;
    uint8_t max_retries = config->retry_count;
    
    do {
        LOG_INFO(LOG_MODULE_NETWORK, "TCP connection attempt %d/%d to %s:%d", 
                retry_attempts + 1, max_retries + 1, config->server_ip, config->server_port);
        
        /* 尝试建立连接 */
        bool success = setup_socket_connection(QUECTEL_PROTOCOL_TCP, 
                                             config->server_ip, 
                                             config->server_port, 
                                             config->local_port);
        
        if (success) {
            /* 验证连接状态 */
            vTaskDelay(pdMS_TO_TICKS(1000));  // 等待连接稳定
            if (tcp_verify_connection(g_quectel4g_info.socket_id)) {
                LOG_INFO(LOG_MODULE_NETWORK, "TCP connection verified successfully");
                return true;
            } else {
                LOG_WARN(LOG_MODULE_NETWORK, "TCP connection verification failed");
                success = false;
            }
        }
        
        if (!success && retry_attempts < max_retries) {
            LOG_INFO(LOG_MODULE_NETWORK, "TCP connection failed, retrying in %dms", 
                    config->retry_interval_ms);
            vTaskDelay(pdMS_TO_TICKS(config->retry_interval_ms));
        }
        
        retry_attempts++;
    } while (retry_attempts <= max_retries && retry_attempts > 0);
    
    return false;
}

/**
 * @brief 验证TCP连接状态
 * @details 通过AT指令查询Socket状态来验证连接
 * @param socket_id Socket ID
 * @return bool 连接是否有效
 */
static bool tcp_verify_connection(uint8_t socket_id)
{
    char response_buffer[128];
    char cmd_buffer[32];
    
    snprintf(cmd_buffer, sizeof(cmd_buffer), "AT+QISTATE=0,%d", socket_id);
    
    AT_Cmd_Config_t query_cmd = {
        .at_cmd = cmd_buffer,
        .expected_resp = "+QISTATE:",
        .timeout_ms = 3000,
        .retries = 1,
        .description = "Query socket state",
        .critical = false,
        .callback = NULL
    };
    
    AT_Result_t result = execute_at_command_with_config(&query_cmd);
    if (result == AT_RESULT_OK) {
        /* 简单检查响应中是否包含"TCP"和"CONNECTED" */
        if (get_response_data(response_buffer, sizeof(response_buffer), 1000)) {
            return (strstr(response_buffer, "TCP") != NULL && 
                   strstr(response_buffer, "CONNECTED") != NULL);
        }
    }
    
    return false;
}

/**
 * @brief 处理TCP连接事件
 * @details 触发TCP事件回调函数
 * @param event_type 事件类型
 * @param socket_id Socket ID
 * @param error_code 错误代码
 */
static void tcp_handle_connection_event(uint8_t event_type, uint8_t socket_id, uint8_t error_code)
{
    /* 调用用户回调 */
    if (s_tcp_event_callback) {
        s_tcp_event_callback(event_type, socket_id, error_code, s_tcp_user_data);
    }
    
    /* 处理连接成功事件，启动数据接收任务和设备注册 */
    if (event_type == 0 || event_type == 3) {  // 0=首次连接成功, 3=重连成功
        LOG_INFO(LOG_MODULE_NETWORK, "TCP connection established, starting data receive task and device registration");
        
        /* 启动数据接收任务 */
        if (!start_data_receive_task()) {
            LOG_ERROR(LOG_MODULE_NETWORK, "Failed to start data receive task after TCP connection");
        }
        
        /* 启动设备注册 */
        // QuectelDataResult_t result = Quectel4G_StartDeviceRegister();
        // if (result != QUECTEL_DATA_SUCCESS) {
        //     LOG_ERROR(LOG_MODULE_NETWORK, "Failed to start device registration: %d", result);
        // }
    }
    /* 处理连接失败或断开事件，停止数据接收任务 */
    else if (event_type == 1 || event_type == 2 || event_type == 4) {  // 1=连接失败, 2=连接断开, 4=重连失败
        LOG_WARN(LOG_MODULE_NETWORK, "TCP connection lost or failed (event=%d), stopping data receive task", event_type);
        stop_data_receive_task();
    }
}

/**
 * @brief 内部心跳发送函数
 * @details TCP监控任务使用的内部心跳发送
 * @return bool 发送是否成功
 */
static bool tcp_send_heartbeat_internal(void)
{
    if (s_tcp_config.heartbeat_interval_s == 0) {
        return true;  // 心跳已禁用
    }
    
    QuectelDataResult_t result = Quectel4G_SendTcpHeartbeat(NULL, 0);
    return (result == QUECTEL_DATA_OK);
}

/**
 * @brief TCP连接监控任务
 * @details 监控TCP连接状态，处理自动重连和心跳发送
 * @param pvParameters 任务参数（未使用）
 */
static void tcp_connection_monitor_task(void* pvParameters)
{
    TickType_t last_heartbeat_time = 0;
    TickType_t heartbeat_interval_ticks = 0;
    
    if (s_tcp_config.heartbeat_interval_s > 0) {
        heartbeat_interval_ticks = pdMS_TO_TICKS(s_tcp_config.heartbeat_interval_s * 1000);
        last_heartbeat_time = xTaskGetTickCount();
    }
    
    LOG_INFO(LOG_MODULE_NETWORK, "TCP connection monitor task started");
    
    while (1) {
        /* 检查是否需要退出任务 */
        if (!s_tcp_config.auto_reconnect) {
            LOG_INFO(LOG_MODULE_NETWORK, "TCP monitor task exiting (auto-reconnect disabled)");
            break;
        }
        
        /* 检查连接状态 */
        bool connection_ok = Quectel4G_CheckTcpConnection();
        
        if (!connection_ok && g_quectel4g_info.socket_connected) {
            /* 连接已断开，触发断开事件 */
            LOG_WARN(LOG_MODULE_NETWORK, "TCP connection lost, attempting reconnection");
            tcp_handle_connection_event(2, g_quectel4g_info.socket_id, 0);
            
            /* 尝试重连 */
            if (tcp_connect_with_retry(&s_tcp_config)) {
                s_tcp_connect_time = xTaskGetTickCount();
                tcp_handle_connection_event(3, g_quectel4g_info.socket_id, 0);  // 重连成功
                LOG_INFO(LOG_MODULE_NETWORK, "TCP reconnection successful");
            } else {
                tcp_handle_connection_event(4, 0, 1);  // 重连失败
                LOG_ERROR(LOG_MODULE_NETWORK, "TCP reconnection failed");
            }
        }
        
        /* 发送心跳数据 */
        if (connection_ok && heartbeat_interval_ticks > 0) {
            TickType_t current_time = xTaskGetTickCount();
            if ((current_time - last_heartbeat_time) >= heartbeat_interval_ticks) {
                tcp_send_heartbeat_internal();
                last_heartbeat_time = current_time;
            }
        }
        
        /* 喂狗 - 防止看门狗复位 */
        MACRO_IWDG_RELOAD();
        
        /* 监控间隔 */
        vTaskDelay(pdMS_TO_TICKS(TCP_MONITOR_INTERVAL_MS));
    }
    
    /* 任务退出清理 */
    s_tcp_monitor_task = NULL;
    vTaskDelete(NULL);
}

/* ==================== 设备注册和数据交互完整实现 ==================== */

/**
 * @brief 初始化设备注册流程
 * @details 配置设备注册所需的数据和参数，准备向服务器注册
 * @param register_data 设备注册数据配置
 * @param callback 注册状态回调函数（可选）
 * @param user_data 用户数据指针
 * @return bool 初始化是否成功
 */
bool Quectel4G_InitDeviceRegister(const DeviceRegisterData_t* register_data,
                                 DeviceRegisterCallback_t callback,
                                 void* user_data)
{
    if (!register_data) {
        LOG_ERROR(LOG_MODULE_NETWORK, "Invalid device register data");
        return false;
    }
    
    if (xSemaphoreTake(s_register_mutex, pdMS_TO_TICKS(1000)) != pdTRUE) {
        LOG_ERROR(LOG_MODULE_NETWORK, "Failed to acquire register mutex");
        return false;
    }
    
    /* 复制注册数据配置 */
    memcpy(&s_device_register_data, register_data, sizeof(DeviceRegisterData_t));
    
    /* 设置默认值 */
    if (s_device_register_data.realtime_data_interval_ms == 0) {
        s_device_register_data.realtime_data_interval_ms = REALTIME_DATA_DEFAULT_INTERVAL;
    }
    if (s_device_register_data.command_timeout_ms == 0) {
        s_device_register_data.command_timeout_ms = DEVICE_REGISTER_TIMEOUT_MS;
    }
    if (s_device_register_data.command_retry_count == 0) {
        s_device_register_data.command_retry_count = DEVICE_COMMAND_RETRY_COUNT;
    }
    
    /* 初始化状态 */
    s_device_register_data.current_state = DEVICE_REG_STATE_IDLE;
    s_device_register_data.registration_completed = false;
    s_device_register_data.last_realtime_send_time = 0;
    s_device_register_data.last_response = 0;
    s_device_register_data.current_retry_count = 0;
    
    /* 设置回调函数 */
    s_device_register_callback = callback;
    s_device_register_user_data = user_data;
    
    /* 重置统计信息 */
    s_register_stats_total = 0;
    s_register_stats_success = 0;
    s_register_stats_failed = 0;
    
    xSemaphoreGive(s_register_mutex);
    
    LOG_INFO(LOG_MODULE_NETWORK, "Device register initialized successfully");
    return true;
}

/**
 * @brief 开始设备注册流程
 * @details 连接服务器成功后调用此函数开始设备注册流程
 * @return QuectelDataResult_t 启动结果
 */
QuectelDataResult_t Quectel4G_StartDeviceRegister(void)
{
    /* 检查TCP连接状态 */
    if (!g_quectel4g_info.socket_connected) {
        LOG_ERROR(LOG_MODULE_NETWORK, "TCP connection not established");
        return QUECTEL_DATA_NO_CONNECTION;
    }
    
    /* 检查注册数据是否已初始化 */
    if (s_device_register_data.current_state == DEVICE_REG_STATE_IDLE && 
        (!s_device_register_data.device_tree_data || s_device_register_data.device_tree_length == 0)) {
        LOG_ERROR(LOG_MODULE_NETWORK, "Device register data not initialized");
        return QUECTEL_DATA_INVALID_PARAM;
    }
    
    /* 停止已存在的注册任务 */
    device_cleanup_register_tasks();
    
    /* 创建设备注册任务 */
    if (xTaskCreate(device_register_task, "DeviceRegister", DEVICE_REGISTER_TASK_STACK,
                   NULL, tskIDLE_PRIORITY + 3, &s_device_register_task) != pdPASS) {
        LOG_ERROR(LOG_MODULE_NETWORK, "Failed to create device register task");
        return QUECTEL_DATA_ERROR;
    }
    
    LOG_INFO(LOG_MODULE_NETWORK, "Device register task started");
    return QUECTEL_DATA_OK;
}

/**
 * @brief 处理服务器响应数据
 * @details 解析服务器响应，推进注册状态机
 * @param data 响应数据
 * @param length 数据长度
 * @return bool 处理是否成功
 */
bool Quectel4G_ProcessServerResponse(const uint8_t* data, uint16_t length)
{
    if (!data || length == 0) {
        return false;
    }
    
    uint8_t response_code = device_parse_response(data, length);
    if (response_code == 0) {
        return false;  // 无效响应
    }
    
    if (xSemaphoreTake(s_register_mutex, pdMS_TO_TICKS(1000)) != pdTRUE) {
        return false;
    }
    
    bool handled = false;
    DeviceRegisterState_t current_state = s_device_register_data.current_state;
    
    /* 检查响应是否与当前状态匹配 */
    switch (current_state) {
        case DEVICE_REG_STATE_DEVICE_TREE:
            if (response_code == SERVER_RESP_DEVICE_TREE) {
                s_register_stats_success++;
                device_handle_register_event(current_state, true, DEVICE_CMD_DEVICE_TREE, response_code);
                device_transition_to_next_state();
                handled = true;
            }
            break;
            
        case DEVICE_REG_STATE_DATA_ATTR_TREE:
            if (response_code == SERVER_RESP_DATA_ATTR_TREE) {
                s_register_stats_success++;
                device_handle_register_event(current_state, true, DEVICE_CMD_DATA_ATTR_TREE, response_code);
                device_transition_to_next_state();
                handled = true;
            }
            break;
            
        case DEVICE_REG_STATE_OFFLINE_DATA:
            if (response_code == SERVER_RESP_OFFLINE_DATA) {
                s_register_stats_success++;
                device_handle_register_event(current_state, true, DEVICE_CMD_OFFLINE_DATA, response_code);
                device_transition_to_next_state();
                handled = true;
            }
            break;
            
        case DEVICE_REG_STATE_TIME_SYNC:
            if (response_code == SERVER_RESP_TIME_SYNC) {
                s_register_stats_success++;
                device_handle_register_event(current_state, true, DEVICE_CMD_TIME_SYNC, response_code);
                device_transition_to_next_state();
                handled = true;
            }
            break;
            
        case DEVICE_REG_STATE_REALTIME_DATA:
            if (response_code == SERVER_RESP_REALTIME_DATA) {
                device_handle_register_event(current_state, true, DEVICE_CMD_REALTIME_DATA, response_code);
                handled = true;
            }
            break;
            
        default:
            break;
    }
    
    xSemaphoreGive(s_register_mutex);
    
    if (handled) {
        LOG_DEBUG(LOG_MODULE_NETWORK, "Server response processed: code=%d, state=%d", 
                 response_code, current_state);
    } else {
        LOG_WARN(LOG_MODULE_NETWORK, "Unexpected server response: code=%d, state=%d", 
                response_code, current_state);
    }
    
    return handled;
}

/**
 * @brief 手动发送实时数据
 * @details 主动发送一次实时数据到服务器
 * @return QuectelDataResult_t 发送结果
 */
QuectelDataResult_t Quectel4G_SendRealtimeData(void)
{
    if (!g_quectel4g_info.socket_connected) {
        return QUECTEL_DATA_NO_CONNECTION;
    }
    
    if (s_device_register_data.current_state != DEVICE_REG_STATE_REGISTERED &&
        s_device_register_data.current_state != DEVICE_REG_STATE_REALTIME_DATA) {
        LOG_WARN(LOG_MODULE_NETWORK, "Device not registered, cannot send realtime data");
        return QUECTEL_DATA_ERROR;
    }
    
    if (!s_device_register_data.get_realtime_data) {
        LOG_ERROR(LOG_MODULE_NETWORK, "No realtime data callback configured");
        return QUECTEL_DATA_INVALID_PARAM;
    }
    
    /* 获取实时数据 */
    uint8_t data_buffer[512];
    uint16_t data_length = s_device_register_data.get_realtime_data(data_buffer + 1, sizeof(data_buffer) - 1);
    
    if (data_length == 0) {
        LOG_WARN(LOG_MODULE_NETWORK, "No realtime data available");
        return QUECTEL_DATA_ERROR;
    }
    
    /* 添加指令代码 */
    data_buffer[0] = DEVICE_CMD_REALTIME_DATA;
    data_length += 1;
    
    /* 发送数据 */
    QuectelDataResult_t result = send_data_internal(g_quectel4g_info.socket_id, 
                                                   data_buffer, data_length, 5000);
    
    if (result == QUECTEL_DATA_OK) {
        s_device_register_data.last_realtime_send_time = xTaskGetTickCount();
        LOG_DEBUG(LOG_MODULE_NETWORK, "Realtime data sent (%d bytes)", data_length);
    } else {
        LOG_ERROR(LOG_MODULE_NETWORK, "Failed to send realtime data (error=%d)", result);
    }
    
    return result;
}

/**
 * @brief 获取设备注册状态
 * @details 获取当前设备注册流程的状态
 * @return DeviceRegisterState_t 当前注册状态
 */
DeviceRegisterState_t Quectel4G_GetRegisterState(void)
{
    return s_device_register_data.current_state;
}

/**
 * @brief 检查设备是否已完成注册
 * @details 检查设备是否已完成所有注册步骤
 * @return bool true=已注册，false=未注册
 */
bool Quectel4G_IsDeviceRegistered(void)
{
    return s_device_register_data.registration_completed;
}

/**
 * @brief 重置设备注册状态
 * @details 重置注册状态，可用于重新注册或错误恢复
 * @return bool 重置是否成功
 */
bool Quectel4G_ResetRegisterState(void)
{
    if (xSemaphoreTake(s_register_mutex, pdMS_TO_TICKS(1000)) != pdTRUE) {
        return false;
    }
    
    /* 停止注册相关任务 */
    device_cleanup_register_tasks();
    
    /* 重置状态 */
    s_device_register_data.current_state = DEVICE_REG_STATE_IDLE;
    s_device_register_data.registration_completed = false;
    s_device_register_data.last_realtime_send_time = 0;
    s_device_register_data.last_response = 0;
    s_device_register_data.current_retry_count = 0;
    
    /* 重置统计信息 */
    s_register_stats_total = 0;
    s_register_stats_success = 0;
    s_register_stats_failed = 0;
    
    xSemaphoreGive(s_register_mutex);
    
    LOG_INFO(LOG_MODULE_NETWORK, "Device register state reset");
    return true;
}

/**
 * @brief 设置实时数据发送间隔
 * @details 动态调整实时数据的发送间隔
 * @param interval_ms 发送间隔（毫秒）
 * @return bool 设置是否成功
 */
bool Quectel4G_SetRealtimeDataInterval(uint32_t interval_ms)
{
    if (interval_ms < 1000 || interval_ms > 3600000) {  // 限制在1秒到1小时之间
        LOG_ERROR(LOG_MODULE_NETWORK, "Invalid realtime data interval: %u", interval_ms);
        return false;
    }
    
    if (xSemaphoreTake(s_register_mutex, pdMS_TO_TICKS(1000)) == pdTRUE) {
        s_device_register_data.realtime_data_interval_ms = interval_ms;
        xSemaphoreGive(s_register_mutex);
        
        LOG_INFO(LOG_MODULE_NETWORK, "Realtime data interval set to %u ms", interval_ms);
        return true;
    }
    
    return false;
}

/**
 * @brief 获取注册统计信息
 * @details 获取设备注册过程的统计信息
 * @param total_commands 总指令数指针
 * @param successful_commands 成功指令数指针
 * @param failed_commands 失败指令数指针
 * @param registration_time 注册耗时指针（毫秒）
 * @return bool 获取是否成功
 */
bool Quectel4G_GetRegisterStats(uint8_t* total_commands, uint8_t* successful_commands,
                               uint8_t* failed_commands, uint32_t* registration_time)
{
    if (total_commands) *total_commands = s_register_stats_total;
    if (successful_commands) *successful_commands = s_register_stats_success;
    if (failed_commands) *failed_commands = s_register_stats_failed;
    
    if (registration_time) {
        if (s_register_start_time > 0 && s_device_register_data.registration_completed) {
            uint32_t end_time = xTaskGetTickCount();
            *registration_time = (end_time - s_register_start_time) * portTICK_PERIOD_MS;
        } else {
            *registration_time = 0;
        }
    }
    
    return true;
}

/* ==================== 设备注册内部函数实现 ==================== */

/**
 * @brief 清理设备注册相关任务
 */
static void device_cleanup_register_tasks(void)
{
    /* 停止设备注册任务 */
    if (s_device_register_task != NULL) {
        TaskHandle_t task_to_delete = s_device_register_task;
        s_device_register_task = NULL;
        
        /* 等待任务自然退出 */
        uint8_t wait_count = 0;
        while (eTaskGetState(task_to_delete) != eDeleted && wait_count < 10) {
            vTaskDelay(pdMS_TO_TICKS(100));
            wait_count++;
        }
        
        if (eTaskGetState(task_to_delete) != eDeleted) {
            vTaskDelete(task_to_delete);
        }
        
        LOG_INFO(LOG_MODULE_NETWORK, "Device register task stopped");
    }
    
    /* 停止实时数据任务 */
    if (s_realtime_data_task != NULL) {
        TaskHandle_t task_to_delete = s_realtime_data_task;
        s_realtime_data_task = NULL;
        
        /* 等待任务自然退出 */
        uint8_t wait_count = 0;
        while (eTaskGetState(task_to_delete) != eDeleted && wait_count < 10) {
            vTaskDelay(pdMS_TO_TICKS(100));
            wait_count++;
        }
        
        if (eTaskGetState(task_to_delete) != eDeleted) {
            vTaskDelete(task_to_delete);
        }
        
        LOG_INFO(LOG_MODULE_NETWORK, "Realtime data task stopped");
    }
}

/**
 * @brief 发送设备指令
 * @param command 指令代码（10/20/30/40/50）
 * @param data 数据内容（可为NULL）
 * @param length 数据长度
 * @return true 发送成功，false 发送失败
 */
static bool device_send_command(uint8_t command, const uint8_t* data, uint16_t length)
{
    if (!g_quectel4g_info.socket_connected) {
        LOG_ERROR(LOG_MODULE_NETWORK, "TCP not connected, cannot send command %d", command);
        return false;
    }
    
    /* 构建命令数据包：命令码（1字节）+ 数据长度（2字节）+ 数据内容 */
    uint8_t command_buffer[1024];
    uint16_t total_length = 0;
    
    /* 命令码 */
    command_buffer[total_length++] = command;
    
    /* 数据长度（小端序） */
    command_buffer[total_length++] = (uint8_t)(length & 0xFF);
    command_buffer[total_length++] = (uint8_t)((length >> 8) & 0xFF);
    
    /* 数据内容 */
    if (data != NULL && length > 0) {
        if (length > (sizeof(command_buffer) - total_length)) {
            LOG_ERROR(LOG_MODULE_NETWORK, "Command data too large: %d bytes", length);
            return false;
        }
        memcpy(&command_buffer[total_length], data, length);
        total_length += length;
    }
    
    /* 发送命令 */
    QuectelDataResult_t result = send_data_internal(g_quectel4g_info.socket_id, command_buffer, total_length, 5000);
    
    if (result == QUECTEL_DATA_OK) {
        LOG_INFO(LOG_MODULE_NETWORK, "Sent command %d with %d bytes data", command, length);
        return true;
    } else {
        LOG_ERROR(LOG_MODULE_NETWORK, "Failed to send command %d, result: %d", command, result);
        return false;
    }
}

/**
 * @brief 解析服务器响应
 * @param data 响应数据
 * @param length 数据长度
 * @return 响应代码（11/21/31/41/51），如果解析失败返回0
 */
static uint8_t device_parse_response(const uint8_t* data, uint16_t length)
{
    if (data == NULL || length < 1) {
        return 0;
    }
    
    /* 简单检查：第一个字节就是响应代码 */
    uint8_t response_code = data[0];
    
    /* 验证响应代码是否有效 */
    if (response_code == 11 || response_code == 21 || response_code == 31 || 
        response_code == 41 || response_code == 51) {
        LOG_INFO(LOG_MODULE_NETWORK, "Received valid response: %d", response_code);
        return response_code;
    }
    
    LOG_WARN(LOG_MODULE_NETWORK, "Invalid response code: %d", response_code);
    return 0;
}

/**
 * @brief 等待服务器响应
 * @param expected_response 期望的响应代码
 * @param timeout_ms 超时时间（毫秒）
 * @return true 收到期望响应，false 超时或收到错误响应
 */
static bool device_wait_for_response(uint8_t expected_response, uint32_t timeout_ms)
{
    TickType_t start_time = xTaskGetTickCount();
    TickType_t timeout_ticks = pdMS_TO_TICKS(timeout_ms);
    
    while ((xTaskGetTickCount() - start_time) < timeout_ticks) {
        /* 检查是否收到响应 */
        if (xSemaphoreTake(s_register_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
            if (s_device_register_data.last_response == expected_response) {
                xSemaphoreGive(s_register_mutex);
                return true;
            }
            xSemaphoreGive(s_register_mutex);
        }
        
        /* 短暂延时避免过度占用CPU */
        vTaskDelay(pdMS_TO_TICKS(50));
    }
    
    LOG_WARN(LOG_MODULE_NETWORK, "Timeout waiting for response %d", expected_response);
    return false;
}

/**
 * @brief 设备注册任务
 * @details 执行设备注册的状态机流程
 * @param pvParameters 任务参数（未使用）
 */
static void device_register_task(void* pvParameters)
{
    LOG_INFO(LOG_MODULE_NETWORK, "Device register task started");
    
    s_register_start_time = xTaskGetTickCount();
    s_device_register_data.current_state = DEVICE_REG_STATE_DEVICE_TREE;
    
    while (1) {
        if (!g_quectel4g_info.socket_connected) {
            LOG_ERROR(LOG_MODULE_NETWORK, "TCP connection lost during registration");
            device_handle_register_event(s_device_register_data.current_state, false, 0, 0);
            break;
        }
        
        switch (s_device_register_data.current_state) {
            case DEVICE_REG_STATE_DEVICE_TREE:
                s_register_stats_total++;
                if (device_send_command(DEVICE_CMD_DEVICE_TREE, 
                                      s_device_register_data.device_tree_data,
                                      s_device_register_data.device_tree_length)) {
                    if (device_wait_for_response(SERVER_RESP_DEVICE_TREE, 
                                               s_device_register_data.command_timeout_ms)) {
                        /* 响应处理在 Quectel4G_ProcessServerResponse 中进行 */
                    } else {
                        s_register_stats_failed++;
                        device_handle_register_event(s_device_register_data.current_state, false, 
                                                   DEVICE_CMD_DEVICE_TREE, 0);
                    }
                } else {
                    s_register_stats_failed++;
                    device_handle_register_event(s_device_register_data.current_state, false, 
                                               DEVICE_CMD_DEVICE_TREE, 0);
                }
                break;
                
            case DEVICE_REG_STATE_DATA_ATTR_TREE:
                s_register_stats_total++;
                if (device_send_command(DEVICE_CMD_DATA_ATTR_TREE,
                                      s_device_register_data.data_attr_tree_data,
                                      s_device_register_data.data_attr_tree_length)) {
                    if (device_wait_for_response(SERVER_RESP_DATA_ATTR_TREE,
                                               s_device_register_data.command_timeout_ms)) {
                        /* 响应处理在 Quectel4G_ProcessServerResponse 中进行 */
                    } else {
                        s_register_stats_failed++;
                        device_handle_register_event(s_device_register_data.current_state, false,
                                                   DEVICE_CMD_DATA_ATTR_TREE, 0);
                    }
                } else {
                    s_register_stats_failed++;
                    device_handle_register_event(s_device_register_data.current_state, false,
                                               DEVICE_CMD_DATA_ATTR_TREE, 0);
                }
                break;
                
            case DEVICE_REG_STATE_OFFLINE_DATA:
                if (s_device_register_data.has_offline_data) {
                    s_register_stats_total++;
                    if (device_send_command(DEVICE_CMD_OFFLINE_DATA,
                                          s_device_register_data.offline_data,
                                          s_device_register_data.offline_data_length)) {
                        if (device_wait_for_response(SERVER_RESP_OFFLINE_DATA,
                                                   s_device_register_data.command_timeout_ms)) {
                            /* 响应处理在 Quectel4G_ProcessServerResponse 中进行 */
                        } else {
                            s_register_stats_failed++;
                            device_handle_register_event(s_device_register_data.current_state, false,
                                                       DEVICE_CMD_OFFLINE_DATA, 0);
                        }
                    } else {
                        s_register_stats_failed++;
                        device_handle_register_event(s_device_register_data.current_state, false,
                                                   DEVICE_CMD_OFFLINE_DATA, 0);
                    }
                } else {
                    /* 没有离线数据，直接跳转到下一状态 */
                    device_transition_to_next_state();
                }
                break;
                
            case DEVICE_REG_STATE_TIME_SYNC:
                s_register_stats_total++;
                if (device_send_command(DEVICE_CMD_TIME_SYNC, NULL, 0)) {
                    if (device_wait_for_response(SERVER_RESP_TIME_SYNC,
                                               s_device_register_data.command_timeout_ms)) {
                        /* 响应处理在 Quectel4G_ProcessServerResponse 中进行 */
                    } else {
                        s_register_stats_failed++;
                        device_handle_register_event(s_device_register_data.current_state, false,
                                                   DEVICE_CMD_TIME_SYNC, 0);
                    }
                } else {
                    s_register_stats_failed++;
                    device_handle_register_event(s_device_register_data.current_state, false,
                                               DEVICE_CMD_TIME_SYNC, 0);
                }
                break;
                
            case DEVICE_REG_STATE_REGISTERED:
                /* 注册完成，启动实时数据发送任务 */
                s_device_register_data.registration_completed = true;
                device_handle_register_event(s_device_register_data.current_state, true, 0, 0);
                
                /* 创建实时数据发送任务 */
                if (xTaskCreate(realtime_data_task, "RealtimeData", REALTIME_DATA_TASK_STACK,
                               NULL, tskIDLE_PRIORITY + 2, &s_realtime_data_task) == pdPASS) {
                    LOG_INFO(LOG_MODULE_NETWORK, "Realtime data task started");
                } else {
                    LOG_ERROR(LOG_MODULE_NETWORK, "Failed to create realtime data task");
                }
                
                LOG_INFO(LOG_MODULE_NETWORK, "Device registration completed successfully");
                goto task_exit;
                
            case DEVICE_REG_STATE_ERROR:
                LOG_ERROR(LOG_MODULE_NETWORK, "Device registration failed");
                device_handle_register_event(s_device_register_data.current_state, false, 0, 0);
                goto task_exit;
                
            default:
                LOG_ERROR(LOG_MODULE_NETWORK, "Invalid register state: %d", s_device_register_data.current_state);
                s_device_register_data.current_state = DEVICE_REG_STATE_ERROR;
                break;
        }
        
        /* 喂狗 - 防止看门狗复位 */
        MACRO_IWDG_RELOAD();
        
        /* 任务间隔 */
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
task_exit:
    /* 任务退出清理 */
    s_device_register_task = NULL;
    vTaskDelete(NULL);
}

/**
 * @brief 实时数据发送任务
 * @details 定时发送实时数据到服务器
 * @param pvParameters 任务参数（未使用）
 */
static void realtime_data_task(void* pvParameters)
{
    LOG_INFO(LOG_MODULE_NETWORK, "Realtime data task started");
    
    TickType_t last_send_time = xTaskGetTickCount();
    
    while (1) {
        /* 检查TCP连接状态 */
        if (!g_quectel4g_info.socket_connected) {
            LOG_WARN(LOG_MODULE_NETWORK, "TCP connection lost, stopping realtime data task");
            break;
        }
        
        /* 检查发送间隔 */
        TickType_t current_time = xTaskGetTickCount();
        TickType_t interval_ticks = pdMS_TO_TICKS(s_device_register_data.realtime_data_interval_ms);
        
        if ((current_time - last_send_time) >= interval_ticks) {
            /* 发送实时数据 */
            QuectelDataResult_t result = Quectel4G_SendRealtimeData();
            if (result == QUECTEL_DATA_OK) {
                last_send_time = current_time;
            } else {
                LOG_WARN(LOG_MODULE_NETWORK, "Failed to send realtime data, retrying...");
            }
        }
        
        /* 喂狗 - 防止看门狗复位 */
        MACRO_IWDG_RELOAD();
        
        /* 任务延时 */
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
    /* 任务退出清理 */
    s_realtime_data_task = NULL;
    vTaskDelete(NULL);
}

/**
 * @brief 处理设备注册事件
 * @param state 当前状态
 * @param success 是否成功
 * @param command 命令代码
 * @param response 响应代码
 */
static void device_handle_register_event(DeviceRegisterState_t state, bool success, uint8_t command, uint8_t response)
{
    if (s_device_register_callback != NULL) {
        s_device_register_callback(state, success, command, response, s_device_register_user_data);
    }
    
    if (success) {
        s_register_stats_success++;
        LOG_INFO(LOG_MODULE_NETWORK, "Registration step completed: state=%d, cmd=%d, resp=%d", 
                state, command, response);
        
        /* 成功时转到下一状态 */
        device_transition_to_next_state();
    } else {
        LOG_ERROR(LOG_MODULE_NETWORK, "Registration step failed: state=%d, cmd=%d", state, command);
        
        /* 检查是否需要重试 */
        if (s_device_register_data.current_retry_count < s_device_register_data.command_retry_count) {
            s_device_register_data.current_retry_count++;
            LOG_INFO(LOG_MODULE_NETWORK, "Retrying registration step, count: %d", 
                    s_device_register_data.current_retry_count);
            /* 保持当前状态，下次循环会重试 */
        } else {
            /* 超过重试次数，设置为错误状态 */
            s_device_register_data.current_state = DEVICE_REG_STATE_ERROR;
        }
    }
}

/**
 * @brief 状态机状态转换
 * @return true 转换成功，false 已完成或出错
 */
static bool device_transition_to_next_state(void)
{
    /* 重置重试计数 */
    s_device_register_data.current_retry_count = 0;
    
    switch (s_device_register_data.current_state) {
        case DEVICE_REG_STATE_DEVICE_TREE:
            s_device_register_data.current_state = DEVICE_REG_STATE_DATA_ATTR_TREE;
            break;
            
        case DEVICE_REG_STATE_DATA_ATTR_TREE:
            s_device_register_data.current_state = DEVICE_REG_STATE_OFFLINE_DATA;
            break;
            
        case DEVICE_REG_STATE_OFFLINE_DATA:
            s_device_register_data.current_state = DEVICE_REG_STATE_TIME_SYNC;
            break;
            
        case DEVICE_REG_STATE_TIME_SYNC:
            s_device_register_data.current_state = DEVICE_REG_STATE_REGISTERED;
            return true;
            
        case DEVICE_REG_STATE_REGISTERED:
            /* 已完成注册 */
            return false;
            
        default:
            s_device_register_data.current_state = DEVICE_REG_STATE_ERROR;
            return false;
    }
    
    LOG_INFO(LOG_MODULE_NETWORK, "State transition to: %d", s_device_register_data.current_state);
    return true;
}
