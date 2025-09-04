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
 * @param command AT命令字符串（不需要包含\r\n）
 * @param expected_response 期望的响应字符串，NULL表示不等待响应
 * @param timeout_ms 超时时间（毫秒）
 * @return BaseType_t 发送结果 (pdPASS/pdFAIL)
 * @note 命令会被加入发送队列异步处理
 */
BaseType_t CommuSend_4gAtCommand(const char* command, const char* expected_response, uint32_t timeout_ms);

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
#define SEND_4G_CMD(cmd)            CommuSend_4gAtCommand(cmd, NULL, 1000)

/**
 * @brief 发送4G AT命令并等待OK响应
 * @param cmd AT命令字符串
 */
#define SEND_4G_CMD_OK(cmd)         CommuSend_4gAtCommand(cmd, "OK", 3000)

/**
 * @brief 发送RS485字符串数据
 * @param str 字符串指针
 */
#define SEND_RS485_STRING(str)      CommuSend_Rs485Data((const uint8_t*)str, strlen(str))

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

#endif /* __TSK_COMMU_SEND_H */
