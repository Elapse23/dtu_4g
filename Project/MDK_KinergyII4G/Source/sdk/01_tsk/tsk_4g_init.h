/**
 * =====================================================================================
 * @file        tsk_4g_init.h
 * @brief       移远4G模块初始化任务头文件
 * @author      23Elapse & GitHub Copilot
 * @version     1.0
 * @date        2025-09-03
 */

#ifndef __TSK_4G_INIT_H__
#define __TSK_4G_INIT_H__

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdint.h>
#include <stdbool.h>

/* ============================= 配置参数定义 ============================= */

/* 任务参数配置 */
#define INIT_TASK_STACK_SIZE        (configMINIMAL_STACK_SIZE * 3)  /**< 初始化任务栈大小 */
#define INIT_TASK_PRIORITY          (tskIDLE_PRIORITY + 4)          /**< 初始化任务优先级 */
#define AT_COMMAND_TIMEOUT_MS       5000                            /**< AT指令默认超时时间 */
#define INIT_RETRY_DELAY_MS         500                            /**< 初始化重试间隔时间 */
#define STATUS_CHECK_INTERVAL_MS    60 * 10000                     /**< 状态检查间隔时间 */

/* 模块重启相关参数 */
#define MODULE_RESET_PULSE_MS       100     /**< 硬件重启脉冲持续时间（毫秒） */
#define MODULE_RESET_WAIT_MS        5000    /**< 模块重启后等待时间（毫秒） */
#define MAX_HARD_RESET_COUNT        3       /**< 最大硬件重启次数 */
#define SOFT_RESET_TIMEOUT_MS       10000   /**< 软件重启命令超时时间（毫秒） */

/* 队列配置参数 */
#define QUEUE_LENGTH                10      /**< 队列长度 */
#define QUEUE_ITEM_SIZE             sizeof(LteToMcu_QueueMessage_t)  /**< 队列项大小 */
#define LTE_MODULE_DATA_BUFFER_SIZE 512     /**< LTE模块数据缓冲区大小 */
/* 资源清理位掩码 */
#define CLEANUP_DATA_QUEUE          (1 << 5)    /**< 清理数据处理队列 */
#define CLEANUP_DATA_TASK           (1 << 6)    /**< 清理数据接收任务 */
#define CLEANUP_AT_QUEUE            (1 << 7)    /**< 清理AT响应队列 */
#define CLEANUP_MUTEXES             (1 << 8)    /**< 清理互斥量和信号量 */
#define CLEANUP_ALL_RESOURCES       (CLEANUP_DATA_QUEUE | CLEANUP_DATA_TASK | CLEANUP_AT_QUEUE | CLEANUP_MUTEXES)  /**< 清理所有资源 */

typedef enum {
    LTE_STATE_UNKNOWN = 0,      /**< 未知状态 - 模块刚上电或状态不明确 */
    LTE_STATE_INITIALIZING,     /**< 初始化中 - 正在执行基础AT指令初始化 */
    LTE_STATE_READY,            /**< 就绪状态 - 基础初始化完成，可执行AT指令 */
    LTE_STATE_REGISTERING,      /**< 网络注册中 - 正在搜索并注册网络 */
    LTE_STATE_REGISTERED,       /**< 已注册网络 - 成功注册到运营商网络 */
    LTE_STATE_CONNECTING,       /**< 数据连接中 - 正在建立数据连接（PDP上下文激活） */
    LTE_STATE_CONNECTED,        /**< 已连接数据网络 - 可进行数据通信 */
    LTE_STATE_ERROR,            /**< 错误状态 - 初始化或操作失败 */
    LTE_STATE_SLEEP             /**< 休眠状态 - 模块进入低功耗模式 */
} LteState_t;

/**
 * @brief AT指令执行结果枚举
 * @note 用于标识AT指令的执行状态和结果
 */
typedef enum {
    AT_RESULT_OK = 0,               /**< 执行成功 - AT指令正常执行并返回期望响应 */
    AT_RESULT_ERROR,                /**< 执行失败 - AT指令返回ERROR或其他错误响应 */
    AT_RESULT_TIMEOUT,              /**< 超时 - AT指令在指定时间内未收到响应 */
    AT_RESULT_RETRY_EXHAUSTED       /**< 重试次数用尽 - 达到最大重试次数仍未成功 */
} AT_Result_t;
/**
 * @brief 支持的模块类型枚举
 * @note 定义系统支持的所有通信模块类型
 */
typedef enum {
    MODULE_TYPE_MCU = 0,        /**< MCU模块 (本地处理) */
    MODULE_TYPE_4G,         /**< 4G模块 (Lte) */
    MODULE_TYPE_BLE,            /**< BLE模块 (ESP) */
    MODULE_TYPE_WIFI,           /**< WiFi模块 (ESP) */
    MODULE_TYPE_RS485,          /**< RS485串口模块 */
    MODULE_TYPE_COUNT,          /**< 模块类型计数 */
    MODULE_TYPE_UNKNOWN = 0xFF  /**< 未知模块类型 */
} ModuleType_t;

/**
 * @brief 模块指令类型枚举  
 * @note 定义模块支持的指令类型
 */
typedef enum {
    COMMAND_TYPE_AT = 0,        /**< AT指令 */
    COMMAND_TYPE_DATA,          /**< 数据传输 */
    COMMAND_TYPE_CONFIG,        /**< 配置指令 */
    COMMAND_TYPE_CONTROL,       /**< 控制指令 */
    COMMAND_TYPE_QUERY,         /**< 查询指令 */
    COMMAND_TYPE_COUNT          /**< 指令类型计数 */
} CommandType_t;

/**
 * @brief AT指令枚举定义
 * @note 每个枚举值对应at_cmd_seq数组中的一个元素
 */
typedef enum {
    AT_CMD_WAIT_RDY = 0,           /**< 等待模块准备就绪信号 */
    AT_CMD_BASIC_TEST,             /**< 基本通信测试 */
    AT_CMD_SET_ECHO,               /**< 设置回显模式 */
    AT_CMD_SET_FULL_FUNC,          /**< 设置全功能模式 */
    AT_CMD_GET_VERSION,            /**< 获取版本信息 */
    AT_CMD_GET_IMEI,               /**< 获取IMEI */
    AT_CMD_GET_ICCID,              /**< 获取ICCID */
    AT_CMD_GET_IMSI,               /**< 获取IMSI */
    AT_CMD_CHECK_SIM,              /**< 检查SIM卡状态 */
    AT_CMD_SIGNAL_QUALITY,         /**< 获取信号质量 */
    AT_CMD_OPERATOR_INFO,          /**< 获取运营商信息 */
    AT_CMD_NETWORK_REG_STATUS,     /**< 检查网络注册状态 */
    AT_CMD_CHECK_DATA_CONN,        /**< 检查数据连接状态 */
    AT_CMD_NETWORK_REG_CHECK,      /**< 网络注册检查（带\r\n） */
    AT_CMD_GPRS_REG_CHECK,         /**< GPRS注册检查 */
    AT_CMD_SET_APN,                /**< 设置APN配置 */
    AT_CMD_ACTIVATE_PDP,           /**< 激活PDP上下文 */
    AT_CMD_QUERY_IP,               /**< 查询IP地址 */
    AT_CMD_MAX                     /**< 枚举最大值 */
} AT_Cmd_Index_t;

/**
 * @brief AT指令类型枚举
 * @note 定义AT指令的四种标准类型
 */
typedef enum {
    AT_TYPE_TEST = 0,              /**< 测试命令 AT+<cmd>=? - 测试是否存在相应的设置命令，并返回有关其参数的类型、值或范围的信息 */
    AT_TYPE_QUERY,                 /**< 查询命令 AT+<cmd>? - 查询相应设置命令的当前参数值 */
    AT_TYPE_SET,                   /**< 设置命令 AT+<cmd>=<p1>[,<p2>[,<p3>[...]]] - 设置用户可定义的参数值 */
    AT_TYPE_EXECUTE,               /**< 执行命令 AT+<cmd> - 返回特定的参数信息或执行特定的操作 */
    AT_TYPE_BASIC,                 /**< 基本AT命令 - 基础的AT指令，如AT、ATI等 */
    AT_TYPE_EXTENDED,              /**< 扩展AT命令 - 厂商特定的AT指令 */
    AT_TYPE_MAX                    /**< 枚举最大值 */
} AT_Cmd_Type_t;

/**
 * @brief AT指令执行项结构体
 * @note 包含AT指令索引和指令类型的组合，用于Lte_ExecuteAtSequence函数
 */
typedef struct {
    AT_Cmd_Index_t cmd_index;      /**< AT指令枚举索引 - 指定要执行的AT指令 */
    AT_Cmd_Type_t cmd_type;        /**< AT指令类型 - 指定AT指令的执行类型（测试/查询/设置/执行） */
} AT_Cmd_Item_t;

/**
 * @brief LTE统一消息头
 */
typedef struct {
    ModuleType_t module_type;                   /**< 消息类型 */
    CommandType_t cmd_type;                     /**< 命令类型 */
    uint8_t target_module;                      /**< 目标模块类型 */
    uint16_t sequence_id;                       /**< 消息序列号 */
    uint16_t data_length;                       /**< 数据负载长度 */
    uint32_t timestamp;                         /**< 时间戳 */
    uint8_t priority;                           /**< 消息优先级 (0-7) */
    uint8_t flags;                              /**< 消息标志位 */
} __attribute__((packed)) LTE_Message_Header_t;
/**
 * @brief 4G数据队列消息结构体
 * @note 用于在通信接收任务和4G数据处理任务之间传递数据
 */
typedef struct {
    LTE_Message_Header_t header;    /**< 消息头 - 包含类型、长度、时间戳等信息 */
    uint8_t data[LTE_MODULE_DATA_BUFFER_SIZE  - sizeof(LTE_Message_Header_t)];              /**< 数据内容 - 接收到的原始数据 */
    uint16_t checksum;               /**< 校验和 - 用于数据完整性校验 */
} __attribute__((packed)) LteToMcu_QueueMessage_t;

/**
 * @brief AT指令配置结构体
 * @note 统一管理AT指令的执行参数、重试策略和回调处理
 */
typedef struct {
    AT_Cmd_Index_t cmd_index;       /**< AT指令枚举索引 - 唯一标识指令类型 */
    ModuleType_t module_type;       /**< 模块类型 - 指定目标模块（4G/WiFi/BT等） */
    const char* at_cmd;             /**< AT指令字符串 - 要发送的完整AT指令（含\r\n） */
    const char* expected_resp;      /**< 期望的响应字符串 - 用于判断指令是否执行成功 */
    uint32_t timeout_ms;            /**< 超时时间（毫秒） - 等待响应的最大时间 */
    uint8_t retries;                /**< 重试次数 - 指令失败时的最大重试次数 */
    bool critical;                  /**< 关键指令标志：
                                     *   true=失败时立即停止序列并重启模块
                                     *   false=失败时记录日志但继续执行后续指令 */
    void (*callback)(const char* resp, AT_Result_t result);  /**< 回调函数 - 指令完成后的处理函数 */
} AT_Cmd_Config_t;

/**
 * @brief 移远4G设备信息结构体
 * @note 统一管理4G模块的所有状态信息，包括设备信息、网络状态、连接配置等
 * @details 这是系统的核心数据结构，通过全局变量g_Lte_info实例化
 */
typedef struct {
    /* 设备基本信息 */
    char firmware_version[32];      /**< 4G模块固件版本号 - 从ATI指令获取 */
    char module_model[32];          /**< 模块型号 - 如EC200U、EC600U等 */
    char imei[16];                  /**< 设备IMEI (15位数字) - 国际移动设备识别码 */
    char imsi[16];                  /**< SIM卡IMSI (15位数字) - 国际移动用户识别码 */
    char iccid[21];                 /**< SIM卡ICCID (20位数字) - 集成电路卡识别码 */
    
    /* 网络配置信息 */
    char apn[32];                   /**< 当前使用的APN - 接入点名称 */
    char operator_name[32];         /**< 运营商名称 - 如"China Mobile" */
    char ip_address[16];            /**< 分配的IP地址 - 点分十进制格式 */
    char dns_primary[16];           /**< 主DNS服务器 - 域名解析服务器 */
    char dns_secondary[16];         /**< 备用DNS服务器 - 备用域名解析服务器 */
    
    /* 连接配置信息 */
    char remote_server[64];         /**< 远程服务器地址 - 域名或IP地址 */
    uint16_t remote_port;           /**< 远程服务器端口 - 目标端口号 */
    uint16_t local_port;            /**< 本地端口 - 本地绑定端口，0表示自动分配 */
    uint8_t context_id;             /**< PDP上下文ID - 数据连接上下文标识(通常为1) */
    uint8_t connection_type;        /**< 连接类型：0=TCP, 1=UDP, 2=HTTP */
    
    /* 网络状态信息 */
    int8_t signal_rssi;             /**< 信号强度 (dBm) - 范围-113到-51，越大信号越强 */
    uint8_t signal_ber;             /**< 误码率 - 比特错误率，0-7有效 */
    uint8_t network_reg_status;     /**< 网络注册状态 - 0=未注册,1=已注册,2=搜索中等 */
    uint8_t gprs_reg_status;        /**< GPRS注册状态 - 数据服务注册状态 */
    bool data_call_active;          /**< 数据连接是否激活 - PDP上下文是否已激活 */
    bool roaming_status;            /**< 是否处于漫游状态 - true表示在非归属网络 */
} Lte_Device_Info_t;

/**
 * @brief 初始化配置结构
 * @note 定义4G模块初始化过程中的各种参数和策略
 */
typedef struct {
    uint32_t init_timeout_ms;       /**< 初始化超时时间 - 整个初始化过程的最大允许时间 */
    uint32_t at_response_timeout_ms;/**< AT命令响应超时 - 单个AT指令的默认超时时间 */
    uint8_t max_retry_count;        /**< 最大重试次数 - 初始化失败时的最大重试次数 */
    bool auto_register;             /**< 是否自动注册网络 - true=自动搜索并注册网络 */
    bool enable_echo;               /**< 是否启用AT命令回显 - true=启用ATE1,false=ATE0 */
    char apn[32];                   /**< APN设置 - 接入点名称,如"3gnet","cmnet"等 */
    char username[32];              /**< 用户名 - APN认证用户名,大多数情况下为空 */
    char password[32];              /**< 密码 - APN认证密码,大多数情况下为空 */
    
} LteInitConfig_t;

/* 公共函数声明 */
/**
 * @brief 向4G数据处理队列发送数据
 * @param data 数据指针
 * @param length 数据长度
 * @return bool 发送是否成功
 */
bool lte_send_data_to_mcu(ModuleType_t module_type, const uint8_t* data, uint32_t length);
/**
 * @brief 初始化4G模块初始化任务
 * @param config 初始化配置指针，NULL使用默认配置
 * @return BaseType_t 初始化结果
 */
BaseType_t lte_init(const LteInitConfig_t* config);



/**
 * @brief 获取4G模块当前状态
 * @return LteState_t 当前状态
 */
LteState_t lte_get_state(void);

/**
 * @brief 强制重新初始化4G模块
 * @return BaseType_t 操作结果
 */
BaseType_t lte_reinitialize(void);
/**
 * @brief 获取4G模块初始化状态
 * @return bool 初始化是否完成
 */
bool get_lte_initialization_status(void);
/**
 * @brief 硬件重启4G模块
 * @return BaseType_t 操作结果
 */
BaseType_t lte_hard_reset(void);

/**
 * @brief 软件重启4G模块
 * @return BaseType_t 操作结果
 */
BaseType_t lte_soft_reset(void);

/**
 * @brief 检查4G模块是否就绪
 * @return bool 是否就绪
 */
bool lte_is_ready(void);

/**
 * @brief 获取信号强度
 * @param rssi 信号强度输出
 * @param ber 误码率输出
 * @return bool 获取是否成功
 */
bool lte_get_signal_strength(int8_t* rssi, uint8_t* ber);

/**
 * @brief 将RSSI信号强度(dBm)转换为百分比
 * @param rssi_dbm 信号强度(dBm)，范围通常为 -113 到 -51 dBm
 * @return 信号强度百分比 (0-100%)
 */
uint8_t lte_rssi_to_percentage(int8_t rssi_dbm);

/**
 * @brief 将RSSI信号强度转换为信号等级
 * @param rssi_dbm 信号强度(dBm)
 * @return 信号等级 (0-4): 0=无信号, 1=较差, 2=一般, 3=良好, 4=优秀
 */
uint8_t lte_rssi_to_level(int8_t rssi_dbm);

/**
 * @brief 获取信号质量描述字符串
 * @param rssi_dbm 信号强度(dBm)
 * @return 信号质量描述字符串
 */
const char* lte_rssi_to_description(int8_t rssi_dbm);

/**
 * @brief 等待4G模块初始化完成
 * @param timeout_ms 等待超时时间
 * @return bool 是否在超时前完成初始化
 */
bool lte_wait_for_ready(uint32_t timeout_ms);

/* ============================= 互斥访问接口 ============================= */




/**
 * @brief 执行单个AT指令（通过配置）
 * @param cmd_config AT指令配置
 * @return AT_Result_t 执行结果
 */
AT_Result_t at_exec(const AT_Cmd_Config_t* cmd_config);

/**
 * @brief 执行单个AT指令（通过枚举索引）
 * @param cmd_index AT指令枚举索引
 * @return AT_Result_t 执行结果
 */
AT_Result_t at_exec_by_id(AT_Cmd_Index_t cmd_index);

/**
 * @brief 执行AT指令序列（支持指令类型）
 * @param cmd_items AT指令项数组，包含指令索引和类型
 * @param count 指令数量
 * @return bool 是否全部执行成功
 */
bool at_exec_batch(const AT_Cmd_Item_t cmd_items[], uint8_t count);

/**
 * @brief 根据枚举索引和类型获取AT指令配置
 * @param cmd_index AT指令枚举索引
 * @param cmd_type AT指令类型
 * @return const AT_Cmd_Config_t* 指令配置指针，NULL表示索引无效
 */
const AT_Cmd_Config_t* at_get_config(AT_Cmd_Index_t cmd_index, AT_Cmd_Type_t cmd_type);

/**
 * @brief 获取默认初始化配置
 * @return 默认配置
 */
LteInitConfig_t lte_get_default_config(void);

/**
 * @brief 向4G数据处理队列发送数据
 * @param data 数据指针
 * @param length 数据长度
 * @return bool 发送是否成功
 * @note 统一接口：将接收到的4G模块数据推送到内部处理队列
 */
bool lte_send_data_to_queue(const uint8_t* data, uint32_t length);

/* 声明全局唯一的4G设备信息实例 */
extern Lte_Device_Info_t g_Lte_info;

/**
 * @brief 获取4G设备信息
 * @param info 设备信息结构指针
 * @return bool 获取是否成功
 */
bool lte_get_device_info(Lte_Device_Info_t* info);

/**
 * @brief 更新4G设备信息
 * @param info 设备信息结构体指针（输入）
 * @return bool 更新是否成功
 */
bool lte_update_device_info(const Lte_Device_Info_t* info);

/* ==================== TCP连接服务器完整流程接口 ==================== */

/**
 * @brief TCP服务器连接配置结构体
 * @note 用于配置TCP服务器连接的所有参数
 */
typedef struct {
    char server_ip[16];             /**< 服务器IP地址 */
    uint16_t server_port;           /**< 服务器端口号 */
    uint16_t local_port;            /**< 本地端口号（可选，0表示自动分配） */
    uint32_t connect_timeout_ms;    /**< 连接超时时间（毫秒） */
    uint8_t retry_count;            /**< 连接失败重试次数 */
    uint32_t retry_interval_ms;     /**< 重试间隔时间（毫秒） */
    bool auto_reconnect;            /**< 是否启用自动重连 */
    uint32_t keepalive_interval_s;  /**< TCP保活间隔（秒，0表示禁用） */
    uint32_t heartbeat_interval_s;  /**< 应用层心跳间隔（秒，0表示禁用） */
    const char* heartbeat_data;     /**< 心跳数据内容（可选） */
    uint16_t heartbeat_length;      /**< 心跳数据长度 */
} Lte_TcpServerConfig_t;

/**
 * @brief TCP连接事件类型枚举
 * @note 定义TCP连接过程中可能出现的各种事件类型
 */
typedef enum {
    TCP_EVENT_CONNECTED = 0,         /**< 首次连接成功 */
    TCP_EVENT_CONNECT_FAILED,        /**< 连接失败 */
    TCP_EVENT_DISCONNECTED,          /**< 连接断开 */
    TCP_EVENT_RECONNECTED,           /**< 重连成功 */
    TCP_EVENT_RECONNECT_FAILED       /**< 重连失败 */
} TcpEventType_t;

/**
 * @brief TCP连接事件回调函数类型
 * @param event_type 事件类型（使用TcpEventType_t枚举值）
 * @param socket_id Socket ID
 * @param error_code 错误代码（失败时有效）
 * @param user_data 用户数据指针
 */
typedef void (*Lte_TcpEventCallback_t)(TcpEventType_t event_type, uint8_t socket_id, 
                                            uint8_t error_code, void* user_data);

/* ==================== 设备注册和数据交互协议 ==================== */

/**
 * @brief 设备注册状态枚举
 * @note 定义设备连接服务器后的注册流程状态
 */
typedef enum {
    DEVICE_REG_STATE_IDLE = 0,          /**< 空闲状态 - 未开始注册 */
    DEVICE_REG_STATE_DEVICE_TREE,       /**< 发送设备树 - 指令10 */
    DEVICE_REG_STATE_DATA_ATTR_TREE,    /**< 发送数据属性树 - 指令20 */
    DEVICE_REG_STATE_OFFLINE_DATA,      /**< 发送离线数据 - 指令30 */
    DEVICE_REG_STATE_TIME_SYNC,         /**< 对时指令 - 指令40 */
    DEVICE_REG_STATE_REGISTERED,        /**< 注册完成 - 开始发送实时数据 */
    DEVICE_REG_STATE_REALTIME_DATA,     /**< 发送实时数据 - 指令50 */
    DEVICE_REG_STATE_ERROR              /**< 注册错误状态 */
} DeviceRegisterState_t;

/**
 * @brief 设备注册指令枚举
 * @note 定义设备注册过程中使用的指令代码
 */
typedef enum {
    DEVICE_CMD_DEVICE_TREE = 10,        /**< 设备树指令 */
    DEVICE_CMD_DATA_ATTR_TREE = 20,     /**< 数据属性树指令 */
    DEVICE_CMD_OFFLINE_DATA = 30,       /**< 离线数据指令 */
    DEVICE_CMD_TIME_SYNC = 40,          /**< 对时指令 */
    DEVICE_CMD_REALTIME_DATA = 50       /**< 实时数据指令 */
} DeviceCommand_t;

/**
 * @brief 服务器响应指令枚举
 * @note 定义服务器对设备指令的响应代码
 */
typedef enum {
    SERVER_RESP_DEVICE_TREE = 11,       /**< 设备树响应 */
    SERVER_RESP_DATA_ATTR_TREE = 21,    /**< 数据属性树响应 */
    SERVER_RESP_OFFLINE_DATA = 31,      /**< 离线数据响应 */
    SERVER_RESP_TIME_SYNC = 41,         /**< 对时响应 */
    SERVER_RESP_REALTIME_DATA = 51      /**< 实时数据响应 */
} ServerResponse_t;

/**
 * @brief 设备注册数据结构体
 * @note 用于存储设备注册过程中需要发送的各种数据
 */
typedef struct {
    /* 设备树数据 */
    uint8_t* device_tree_data;          /**< 设备树数据指针 */
    uint16_t device_tree_length;        /**< 设备树数据长度 */
    
    /* 数据属性树数据 */
    uint8_t* data_attr_tree_data;       /**< 数据属性树数据指针 */
    uint16_t data_attr_tree_length;     /**< 数据属性树数据长度 */
    
    /* 离线数据 */
    uint8_t* offline_data;              /**< 离线数据指针 */
    uint16_t offline_data_length;       /**< 离线数据长度 */
    bool has_offline_data;              /**< 是否有离线数据 */
    
    /* 实时数据生成回调 */
    uint16_t (*get_realtime_data)(uint8_t* buffer, uint16_t max_length);  /**< 获取实时数据回调函数 */
    
    /* 配置参数 */
    uint32_t realtime_data_interval_ms; /**< 实时数据发送间隔（毫秒） */
    uint32_t command_timeout_ms;        /**< 指令响应超时时间（毫秒） */
    uint8_t command_retry_count;        /**< 指令重试次数 */
    
    /* 状态信息 */
    DeviceRegisterState_t current_state; /**< 当前注册状态 */
    bool registration_completed;         /**< 注册是否完成 */
    uint32_t last_realtime_send_time;   /**< 上次实时数据发送时间 */
    uint8_t last_response;              /**< 最近收到的服务器响应代码 */
    uint8_t current_retry_count;        /**< 当前重试计数器 */
} DeviceRegisterData_t;

/**
 * @brief 设备注册事件回调函数类型
 * @param state 当前注册状态
 * @param success 操作是否成功
 * @param command 当前处理的指令
 * @param response 服务器响应（成功时有效）
 * @param user_data 用户数据指针
 */
typedef void (*DeviceRegisterCallback_t)(DeviceRegisterState_t state, bool success, 
                                        uint8_t command, uint8_t response, void* user_data);



#endif /* __TSK_4G_INIT_H__ */
