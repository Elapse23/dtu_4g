/**
 * =====================================================================================
 * @file        tsk_4g_init.h
 * @brief       移远4G模块初始化任务头文件
 * @author      23Elapse & GitHub Copilot
 * @version     1.0
 * @date        2025-09-03
 * @note        负责移远4G模块的初始化、配置和状态管理
 * =====================================================================================
 */

#ifndef __TSK_4G_INIT_H__
#define __TSK_4G_INIT_H__

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief 移远4G模块状态枚举
 * @note 定义模块从初始化到正常工作的各个状态阶段
 */
typedef enum {
    QUECTEL_STATE_UNKNOWN = 0,      /**< 未知状态 - 模块刚上电或状态不明确 */
    QUECTEL_STATE_INITIALIZING,     /**< 初始化中 - 正在执行基础AT指令初始化 */
    QUECTEL_STATE_READY,            /**< 就绪状态 - 基础初始化完成，可执行AT指令 */
    QUECTEL_STATE_REGISTERING,      /**< 网络注册中 - 正在搜索并注册网络 */
    QUECTEL_STATE_REGISTERED,       /**< 已注册网络 - 成功注册到运营商网络 */
    QUECTEL_STATE_CONNECTING,       /**< 数据连接中 - 正在建立数据连接（PDP上下文激活） */
    QUECTEL_STATE_CONNECTED,        /**< 已连接数据网络 - 可进行数据通信 */
    QUECTEL_STATE_ERROR,            /**< 错误状态 - 初始化或操作失败 */
    QUECTEL_STATE_SLEEP             /**< 休眠状态 - 模块进入低功耗模式 */
} QuectelState_t;

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
 * @brief 数据传输协议类型枚举
 * @note 支持的网络通信协议类型
 */
typedef enum {
    QUECTEL_PROTOCOL_TCP = 0,       /**< TCP协议 - 面向连接的可靠传输 */
    QUECTEL_PROTOCOL_UDP = 1,       /**< UDP协议 - 无连接的快速传输 */
    QUECTEL_PROTOCOL_HTTP = 2       /**< HTTP协议 - 应用层协议 */
} QuectelProtocol_t;

/**
 * @brief 数据传输结果枚举
 * @note 数据发送和接收操作的结果状态
 */
typedef enum {
    QUECTEL_DATA_OK = 0,            /**< 数据操作成功 */
    QUECTEL_DATA_ERROR,             /**< 数据操作失败 */
    QUECTEL_DATA_TIMEOUT,           /**< 数据操作超时 */
    QUECTEL_DATA_BUFFER_FULL,       /**< 缓冲区已满 */
    QUECTEL_DATA_NO_CONNECTION,     /**< 无网络连接 */
    QUECTEL_DATA_INVALID_PARAM      /**< 参数无效 */
} QuectelDataResult_t;

/**
 * @brief 数据接收回调函数类型
 * @param data 接收到的数据指针
 * @param length 数据长度
 * @param remote_ip 远端IP地址（UDP协议有效）
 * @param remote_port 远端端口号（UDP协议有效）
 */
typedef void (*QuectelDataReceiveCallback_t)(const uint8_t* data, uint16_t length, 
                                            const char* remote_ip, uint16_t remote_port);

/**
 * @brief 数据连接状态回调函数类型
 * @param connected 连接状态：true-已连接，false-已断开
 * @param error_code 错误代码（断开时有效）
 */
typedef void (*QuectelConnectionCallback_t)(bool connected, uint8_t error_code);

/**
 * @brief 4G数据队列消息结构体
 * @note 用于在通信接收任务和4G数据处理任务之间传递数据
 */
typedef struct {
    uint8_t data[512];              /**< 数据内容 - 接收到的原始数据 */
    uint16_t length;                /**< 数据长度 - 有效数据字节数 */
    uint32_t timestamp;             /**< 时间戳 - 数据接收时间（系统滴答） */
} Quectel4G_QueueMessage_t;

/**
 * @brief AT指令配置结构体
 * @note 统一管理AT指令的执行参数、重试策略和回调处理
 */
typedef struct {
    const char* at_cmd;             /**< AT指令字符串 - 要发送的完整AT指令（不含\r\n） */
    const char* expected_resp;      /**< 期望的响应字符串 - 用于判断指令是否执行成功 */
    uint32_t timeout_ms;            /**< 超时时间（毫秒） - 等待响应的最大时间 */
    uint8_t retries;                /**< 重试次数 - 指令失败时的最大重试次数 */
    const char* description;        /**< 指令描述 - 用于日志输出和调试 */
    bool critical;                  /**< 关键指令标志：
                                     *   true=失败时立即停止序列并重启模块
                                     *   false=失败时记录日志但继续执行后续指令 */
    void (*callback)(const char* resp, AT_Result_t result);  /**< 回调函数 - 指令完成后的处理函数 */
} AT_Cmd_Config_t;

/**
 * @brief 移远4G设备信息结构体
 * @note 统一管理4G模块的所有状态信息，包括设备信息、网络状态、连接配置等
 * @details 这是系统的核心数据结构，通过全局变量g_quectel4g_info实例化
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
    
    /* 数据传输状态信息 */
    uint8_t socket_id;              /**< Socket连接ID - 当前活动的Socket标识(0-11) */
    bool socket_connected;          /**< Socket连接状态 - 是否已建立Socket连接 */
    QuectelProtocol_t protocol_type;/**< 当前使用的协议类型 - TCP/UDP/HTTP */
    uint32_t bytes_sent;            /**< 累计发送字节数 - 统计已发送的数据量 */
    uint32_t bytes_received;        /**< 累计接收字节数 - 统计已接收的数据量 */
    uint32_t last_data_time;        /**< 最后数据传输时间 - 记录最近一次数据收发的时间戳 */
} Quectel4G_Device_Info_t;

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
    
    /* 数据传输配置 */
    QuectelDataReceiveCallback_t rx_callback;  /**< 数据接收回调函数 */
    QuectelConnectionCallback_t conn_callback; /**< 连接状态回调函数 */
    uint16_t rx_buffer_size;        /**< 接收缓冲区大小 - 默认1024字节 */
    uint32_t keepalive_interval;    /**< 心跳间隔时间(秒) - TCP连接保活间隔 */
} QuectelInitConfig_t;

/* 公共函数声明 */

/**
 * @brief 初始化4G模块初始化任务
 * @param config 初始化配置指针，NULL使用默认配置
 * @return BaseType_t 初始化结果
 */
BaseType_t Quectel4G_Init(const QuectelInitConfig_t* config);

/**
 * @brief 获取4G模块当前状态
 * @return QuectelState_t 当前状态
 */
QuectelState_t Quectel4G_GetState(void);

/**
 * @brief 强制重新初始化4G模块
 * @return BaseType_t 操作结果
 */
BaseType_t Quectel4G_Reinitialize(void);

/**
 * @brief 硬件重启4G模块
 * @return BaseType_t 操作结果
 */
BaseType_t Quectel4G_HardReset(void);

/**
 * @brief 软件重启4G模块
 * @return BaseType_t 操作结果
 */
BaseType_t Quectel4G_SoftReset(void);

/**
 * @brief 设置4G模块为休眠模式
 * @return BaseType_t 操作结果
 */
BaseType_t Quectel4G_Sleep(void);

/**
 * @brief 唤醒4G模块
 * @return BaseType_t 操作结果
 */
BaseType_t Quectel4G_Wakeup(void);

/**
 * @brief 检查4G模块是否就绪
 * @return bool 是否就绪
 */
bool Quectel4G_IsReady(void);

/**
 * @brief 获取信号强度
 * @param rssi 信号强度输出
 * @param ber 误码率输出
 * @return bool 获取是否成功
 */
bool Quectel4G_GetSignalStrength(int8_t* rssi, uint8_t* ber);

/**
 * @brief 等待4G模块初始化完成
 * @param timeout_ms 等待超时时间
 * @return bool 是否在超时前完成初始化
 */
bool Quectel4G_WaitForReady(uint32_t timeout_ms);

/**
 * @brief 执行单个AT指令
 * @param cmd_config AT指令配置
 * @return AT_Result_t 执行结果
 */
AT_Result_t Quectel4G_ExecuteAtCommand(const AT_Cmd_Config_t* cmd_config);

/**
 * @brief 执行AT指令序列
 * @param cmd_sequence AT指令序列数组
 * @param count 指令数量
 * @return bool 是否全部执行成功
 */
bool Quectel4G_ExecuteAtSequence(const AT_Cmd_Config_t* cmd_sequence, uint8_t count);

/**
 * @brief 获取默认初始化配置
 * @return QuectelInitConfig_t 默认配置
 */
QuectelInitConfig_t Quectel4G_GetDefaultConfig(void);

/* 声明全局唯一的4G设备信息实例 */
extern Quectel4G_Device_Info_t g_quectel4g_info;

/**
 * @brief 获取4G设备信息
 * @param info 设备信息结构指针
 * @return bool 获取是否成功
 */
bool Quectel4G_GetDeviceInfo(Quectel4G_Device_Info_t* info);

/**
 * @brief 更新4G设备信息
 * @param info 设备信息结构指针
 * @return bool 更新是否成功
 */
bool Quectel4G_UpdateDeviceInfo(const Quectel4G_Device_Info_t* info);

/* ========================== 数据传输相关函数 ========================== */

/**
 * @brief 建立TCP连接
 * @param remote_ip 远程服务器IP地址
 * @param remote_port 远程服务器端口
 * @param local_port 本地端口（0表示自动分配）
 * @return QuectelDataResult_t 连接结果
 */
QuectelDataResult_t Quectel4G_ConnectTCP(const char* remote_ip, uint16_t remote_port, uint16_t local_port);

/**
 * @brief 建立UDP连接
 * @param remote_ip 远程服务器IP地址
 * @param remote_port 远程服务器端口
 * @param local_port 本地端口（0表示自动分配）
 * @return QuectelDataResult_t 连接结果
 */
QuectelDataResult_t Quectel4G_ConnectUDP(const char* remote_ip, uint16_t remote_port, uint16_t local_port);

/**
 * @brief 断开网络连接
 * @return QuectelDataResult_t 断开结果
 */
QuectelDataResult_t Quectel4G_Disconnect(void);

/**
 * @brief 发送数据（TCP/UDP通用）
 * @param data 要发送的数据指针
 * @param length 数据长度
 * @param timeout_ms 发送超时时间（毫秒）
 * @return QuectelDataResult_t 发送结果
 */
QuectelDataResult_t Quectel4G_SendData(const uint8_t* data, uint16_t length, uint32_t timeout_ms);

/**
 * @brief 发送字符串数据
 * @param str 要发送的字符串
 * @param timeout_ms 发送超时时间（毫秒）
 * @return QuectelDataResult_t 发送结果
 */
QuectelDataResult_t Quectel4G_SendString(const char* str, uint32_t timeout_ms);

/**
 * @brief 发送UDP数据到指定地址
 * @param data 要发送的数据指针
 * @param length 数据长度
 * @param remote_ip 目标IP地址
 * @param remote_port 目标端口
 * @param timeout_ms 发送超时时间（毫秒）
 * @return QuectelDataResult_t 发送结果
 */
QuectelDataResult_t Quectel4G_SendUDPData(const uint8_t* data, uint16_t length, 
                                          const char* remote_ip, uint16_t remote_port, 
                                          uint32_t timeout_ms);

/**
 * @brief 获取接收缓冲区数据
 * @param buffer 接收缓冲区指针
 * @param buffer_size 缓冲区大小
 * @param received_length 实际接收长度指针
 * @param timeout_ms 接收超时时间（毫秒）
 * @return QuectelDataResult_t 接收结果
 */
QuectelDataResult_t Quectel4G_ReceiveData(uint8_t* buffer, uint16_t buffer_size, 
                                          uint16_t* received_length, uint32_t timeout_ms);

/**
 * @brief 检查是否有数据可读
 * @return bool true-有数据可读，false-无数据
 */
bool Quectel4G_IsDataAvailable(void);

/**
 * @brief 获取连接状态
 * @return bool true-已连接，false-未连接
 */
bool Quectel4G_IsConnected(void);

/**
 * @brief 设置数据接收回调函数
 * @param callback 回调函数指针
 */
void Quectel4G_SetReceiveCallback(QuectelDataReceiveCallback_t callback);

/**
 * @brief 设置连接状态回调函数
 * @param callback 回调函数指针
 */
void Quectel4G_SetConnectionCallback(QuectelConnectionCallback_t callback);

/**
 * @brief 获取网络统计信息
 * @param bytes_sent 发送字节数指针
 * @param bytes_received 接收字节数指针
 * @param last_data_time 最后数据时间指针
 * @return bool 获取是否成功
 */
bool Quectel4G_GetNetworkStats(uint32_t* bytes_sent, uint32_t* bytes_received, uint32_t* last_data_time);

/**
 * @brief 重置网络统计信息
 * @details 将发送和接收的字节计数器重置为0
 * @return bool 重置是否成功
 */
bool Quectel4G_ResetNetworkStats(void);

/**
 * @brief 向4G数据处理队列发送数据
 * @details 供通信接收任务调用，将LTE串口数据发送到4G数据处理队列
 * @param data 数据指针
 * @param length 数据长度
 * @return bool 发送是否成功
 */
bool Quectel4G_SendDataToQueue(const uint8_t* data, uint16_t length);

/**
 * @brief 获取4G数据处理队列句柄
 * @details 供其他模块获取队列句柄进行数据发送
 * @return QueueHandle_t 队列句柄，NULL表示未初始化
 */
QueueHandle_t Quectel4G_GetDataQueue(void);

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
} Quectel4G_TcpServerConfig_t;

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
typedef void (*Quectel4G_TcpEventCallback_t)(TcpEventType_t event_type, uint8_t socket_id, 
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

/**
 * @brief 连接TCP服务器（完整流程）
 * @details 提供完整的TCP服务器连接流程，包括：
 *          - 网络状态检查
 *          - Socket连接建立
 *          - 连接状态验证
 *          - 重试机制
 *          - 自动重连配置
 *          - 心跳保活机制
 * 
 * @param config TCP服务器连接配置
 * @param event_callback 连接事件回调函数（可选）
 * @param user_data 用户数据指针（传递给回调函数）
 * @return QuectelDataResult_t 连接结果
 * 
 * @note 此函数会阻塞执行直到连接成功或失败
 * @note 如果启用自动重连，连接断开后会自动尝试重连
 */
QuectelDataResult_t Quectel4G_ConnectTcpServer(const Quectel4G_TcpServerConfig_t* config,
                                              Quectel4G_TcpEventCallback_t event_callback,
                                              void* user_data);

/**
 * @brief 检查TCP服务器连接状态
 * @details 主动查询当前TCP连接状态，验证连接的可用性
 * @return bool true=连接正常，false=连接异常或已断开
 */
bool Quectel4G_CheckTcpConnection(void);

/**
 * @brief 设置TCP自动重连参数
 * @details 配置TCP连接断开后的自动重连行为
 * @param enable 是否启用自动重连
 * @param retry_count 最大重试次数（0表示无限重试）
 * @param retry_interval_ms 重试间隔时间（毫秒）
 * @return bool 设置是否成功
 */
bool Quectel4G_SetTcpAutoReconnect(bool enable, uint8_t retry_count, uint32_t retry_interval_ms);

/**
 * @brief 发送TCP心跳数据
 * @details 发送应用层心跳数据以保持连接活跃
 * @param data 心跳数据（可选，NULL表示使用默认心跳）
 * @param length 数据长度
 * @return QuectelDataResult_t 发送结果
 */
QuectelDataResult_t Quectel4G_SendTcpHeartbeat(const uint8_t* data, uint16_t length);

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
                                  uint32_t* connect_time);

/* ==================== 设备注册和数据交互API ==================== */

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
                                 void* user_data);

/**
 * @brief 开始设备注册流程
 * @details 连接服务器成功后调用此函数开始设备注册流程
 * @return QuectelDataResult_t 启动结果
 * 
 * @note 此函数需要在TCP连接成功后调用
 * @note 注册过程是异步的，通过回调函数通知状态变化
 */
QuectelDataResult_t Quectel4G_StartDeviceRegister(void);

/**
 * @brief 处理服务器响应数据
 * @details 解析服务器响应，推进注册状态机
 * @param data 响应数据
 * @param length 数据长度
 * @return bool 处理是否成功
 */
bool Quectel4G_ProcessServerResponse(const uint8_t* data, uint16_t length);

/**
 * @brief 手动发送实时数据
 * @details 主动发送一次实时数据到服务器
 * @return QuectelDataResult_t 发送结果
 */
QuectelDataResult_t Quectel4G_SendRealtimeData(void);

/**
 * @brief 获取设备注册状态
 * @details 获取当前设备注册流程的状态
 * @return DeviceRegisterState_t 当前注册状态
 */
DeviceRegisterState_t Quectel4G_GetRegisterState(void);

/**
 * @brief 检查设备是否已完成注册
 * @details 检查设备是否已完成所有注册步骤
 * @return bool true=已注册，false=未注册
 */
bool Quectel4G_IsDeviceRegistered(void);

/**
 * @brief 重置设备注册状态
 * @details 重置注册状态，可用于重新注册或错误恢复
 * @return bool 重置是否成功
 */
bool Quectel4G_ResetRegisterState(void);

/**
 * @brief 设置实时数据发送间隔
 * @details 动态调整实时数据的发送间隔
 * @param interval_ms 发送间隔（毫秒）
 * @return bool 设置是否成功
 */
bool Quectel4G_SetRealtimeDataInterval(uint32_t interval_ms);

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
                               uint8_t* failed_commands, uint32_t* registration_time);

#endif /* __TSK_4G_INIT_H__ */
