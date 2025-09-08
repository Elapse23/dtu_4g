/**
 * =====================================================================================
 * @file        tsk_commu_send.h
 * @brief       通信发送任务头文件
 * @author      23Elapse & GitHub Copilot
 * @version     1.0
 * @date        2025-09-03
 * @note        4G AT命令发送和RS485数据发送任务接口
 * =====================================================================================
 */

#ifndef __TSK_COMMU_SEND_H
#define __TSK_COMMU_SEND_H

#include "FreeRTOS.h"
#include "tsk_4g_init.h"  // 包含 AT_Cmd_Config_t 定义
#include "bsp_uart_ring_buffer.h"  // 包含 UART_ID_t 定义
#include <stdint.h>
#include <stdbool.h>

/* 4G模块状态枚举 */
typedef enum {
    MODEM_STATE_IDLE = 0,       // 空闲状态
    MODEM_STATE_CONNECTING,     // 连接中
    MODEM_STATE_CONNECTED,      // 已连接
    MODEM_STATE_SENDING,        // 发送中
    MODEM_STATE_ERROR           // 错误状态
} ModemState_t;

/* 公共函数声明 */

/**
 * @brief 通信发送任务主函数
 * @param pvParameters 任务参数（未使用）
 * @note 这是FreeRTOS任务函数，由任务调度器调用
 */
void vCommuSendTask(void* pvParameters);

/**
 * @brief 初始化通信发送任务
 * @return BaseType_t 初始化结果 (pdPASS/pdFAIL)
 * @note 创建任务队列、互斥量和任务本身
 */
BaseType_t CommuSend_Init(void);

/**
 * @brief 发送4G AT命令
 * @param cmd_config AT命令配置结构体
 * @return BaseType_t 发送结果 (pdPASS/pdFAIL)
 * @note 命令会被加入发送队列异步处理
 */
BaseType_t CommuSend_4gAtCommand(const AT_Cmd_Config_t* cmd_config);

/**
 * @brief 统一的UART数据发送接口
 * @param uart_id 串口ID (UART_ID_LOG, UART_ID_RS485)
 * @param data 要发送的数据
 * @param length 数据长度
 * @param timeout_ms 超时时间（毫秒）
 * @return BaseType_t 发送结果 (pdPASS/pdFAIL)
 * @note LTE串口请使用 CommuSend_4gAtCommand，支持LOG→LTE转发
 */
BaseType_t CommuSend_UartData(UART_ID_t uart_id, const uint8_t* data, uint32_t length, uint32_t timeout_ms);

/**
 * @brief 发送RS485数据
 * @param data 数据指针
 * @param length 数据长度
 * @return BaseType_t 发送结果 (pdPASS/pdFAIL)
 * @note 数据会被加入发送队列异步处理
 */
BaseType_t CommuSend_Rs485Data(const uint8_t* data, uint16_t length);

/**
 * @brief 获取4G模块当前状态
 * @return ModemState_t 4G模块状态
 */
ModemState_t CommuSend_GetModemState(void);

/**
 * @brief 发送HTTP请求
 * @param url 请求URL
 * @param data 请求数据（NULL为GET请求，非NULL为POST请求）
 * @return BaseType_t 发送结果 (pdPASS/pdFAIL)
 * @note 支持HTTP GET和POST请求
 */
BaseType_t CommuSend_HttpRequest(const char* url, const char* data);

/* 便捷宏定义 */

/**
 * @brief 发送简单的4G AT命令（不等待响应）
 * @param cmd AT命令字符串
 */
#define SEND_4G_CMD(cmd) \
    do { \
        AT_Cmd_Config_t at_cfg = {cmd, NULL, 1000, 0, NULL, false, NULL}; \
        CommuSend_4gAtCommand(&at_cfg); \
    } while(0)

/**
 * @brief 发送4G AT命令并等待OK响应
 * @param cmd AT命令字符串
 */
#define SEND_4G_CMD_OK(cmd) \
    do { \
        AT_Cmd_Config_t at_cfg = {cmd, "OK", 3000, 0, NULL, false, NULL}; \
        CommuSend_4gAtCommand(&at_cfg); \
    } while(0)

/**
 * @brief 发送RS485字符串数据
 * @param str 字符串指针
 */
#define SEND_RS485_STRING(str)      CommuSend_Rs485Data((const uint8_t*)str, strlen(str))

/**
 * @brief 发送LOG串口数据（支持转发到LTE）
 * @param data 数据指针
 * @param len 数据长度
 */
#define SEND_LOG_DATA(data, len)    CommuSend_UartData(UART_ID_LOG, data, len, 1000)

/* 常用移远4G AT命令宏 */
#define QUECTEL_CMD_TEST            "AT"
#define QUECTEL_CMD_RESET           "AT+CFUN=1,1"
#define QUECTEL_CMD_VERSION         "ATI"
#define QUECTEL_CMD_IMEI            "AT+GSN"
#define QUECTEL_CMD_ICCID           "AT+QCCID"
#define QUECTEL_CMD_SIGNAL          "AT+CSQ"
#define QUECTEL_CMD_NETWORK_REG     "AT+CREG?"
#define QUECTEL_CMD_GPRS_REG        "AT+CGREG?"
#define QUECTEL_CMD_HTTP_CFG        "AT+QHTTPCFG=\"contextid\",1"
#define QUECTEL_CMD_ACTIVATE_PDP    "AT+QIACT=1"
#define QUECTEL_CMD_DEACT_PDP       "AT+QIDEACT=1"

/* 便捷的AT命令发送函数 */

/**
 * @brief 发送简单AT命令（不等待响应）
 * @param at_cmd AT命令字符串
 * @param timeout_ms 超时时间（毫秒）
 * @return BaseType_t 发送结果 (pdPASS/pdFAIL)
 */
static inline BaseType_t CommuSend_SimpleAtCommand(const char* at_cmd, uint32_t timeout_ms) {
    AT_Cmd_Config_t at_cfg = {at_cmd, NULL, timeout_ms, 0, "Simple Command", false, NULL};
    return CommuSend_4gAtCommand(&at_cfg);
}

/**
 * @brief 发送AT命令并等待OK响应
 * @param at_cmd AT命令字符串
 * @param timeout_ms 超时时间（毫秒）
 * @return BaseType_t 发送结果 (pdPASS/pdFAIL)
 */
static inline BaseType_t CommuSend_AtCommandWithOk(const char* at_cmd, uint32_t timeout_ms) {
    AT_Cmd_Config_t at_cfg = {at_cmd, "OK", timeout_ms, 0, "Command with OK", false, NULL};
    return CommuSend_4gAtCommand(&at_cfg);
}

#endif /* __TSK_COMMU_SEND_H */
