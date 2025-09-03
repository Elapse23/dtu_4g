/**
 * =====================================================================================
 * @file        tsk_commu_send.h
 * @brief       通信发送任务头文件
 * @author      23Elapse & GitHub Copilot
 * @version     1.0
 * @date        2025-09-03
 * @note        ESP AT命令发送和RS485数据发送任务接口
 * =====================================================================================
 */

#ifndef __TSK_COMMU_SEND_H
#define __TSK_COMMU_SEND_H

#include "FreeRTOS.h"
#include <stdint.h>
#include <stdbool.h>

/* ESP状态枚举 */
typedef enum {
    ESP_STATE_IDLE = 0,         // 空闲状态
    ESP_STATE_CONNECTING,       // 连接中
    ESP_STATE_CONNECTED,        // 已连接
    ESP_STATE_SENDING,          // 发送中
    ESP_STATE_ERROR             // 错误状态
} EspState_t;

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
 * @brief 发送ESP AT命令
 * @param command AT命令字符串（不需要包含\r\n）
 * @param expected_response 期望的响应字符串，NULL表示不等待响应
 * @param timeout_ms 超时时间（毫秒）
 * @return BaseType_t 发送结果 (pdPASS/pdFAIL)
 * @note 命令会被加入发送队列异步处理
 */
BaseType_t CommuSend_EspAtCommand(const char* command, const char* expected_response, uint32_t timeout_ms);

/**
 * @brief 发送RS485数据
 * @param data 数据指针
 * @param length 数据长度
 * @return BaseType_t 发送结果 (pdPASS/pdFAIL)
 * @note 数据会被加入发送队列异步处理
 */
BaseType_t CommuSend_Rs485Data(const uint8_t* data, uint16_t length);

/**
 * @brief 获取ESP模块当前状态
 * @return EspState_t ESP状态
 */
EspState_t CommuSend_GetEspState(void);

/**
 * @brief 连接WiFi网络
 * @param ssid WiFi网络名称
 * @param password WiFi密码
 * @return BaseType_t 连接结果 (pdPASS/pdFAIL)
 * @note 这是一个阻塞操作，会等待连接完成
 */
BaseType_t CommuSend_ConnectWiFi(const char* ssid, const char* password);

/* 便捷宏定义 */

/**
 * @brief 发送简单的AT命令（不等待响应）
 * @param cmd AT命令字符串
 */
#define SEND_AT_CMD(cmd)            CommuSend_EspAtCommand(cmd, NULL, 1000)

/**
 * @brief 发送AT命令并等待OK响应
 * @param cmd AT命令字符串
 */
#define SEND_AT_CMD_OK(cmd)         CommuSend_EspAtCommand(cmd, "OK", 3000)

/**
 * @brief 发送RS485字符串数据
 * @param str 字符串指针
 */
#define SEND_RS485_STRING(str)      CommuSend_Rs485Data((const uint8_t*)str, strlen(str))

/* 常用ESP AT命令宏 */
#define ESP_CMD_TEST                "AT"
#define ESP_CMD_RESET               "AT+RST"
#define ESP_CMD_VERSION             "AT+GMR"
#define ESP_CMD_WIFI_MODE_STA       "AT+CWMODE=1"
#define ESP_CMD_WIFI_MODE_AP        "AT+CWMODE=2"
#define ESP_CMD_WIFI_MODE_BOTH      "AT+CWMODE=3"
#define ESP_CMD_WIFI_DISCONNECT     "AT+CWQAP"
#define ESP_CMD_WIFI_LIST           "AT+CWLAP"
#define ESP_CMD_GET_IP              "AT+CIFSR"

#endif /* __TSK_COMMU_SEND_H */
