/**
 * =====================================================================================
 * @file        tsk_commu_send.h
 * @brief       统一通信发送任务头文件
 * @author      23Elapse & GitHub Copilot
 * @version     2.0
 * @date        2025-09-08
 * @note        统一的模块通信发送接口 - 支持4G/BLE/RS485等多种模块通信
 * =====================================================================================
 */

#ifndef __TSK_COMMU_SEND_H
#define __TSK_COMMU_SEND_H

#include "FreeRTOS.h"
#include "tsk_4g_init.h"  
#include "bsp_uart_ring_buffer.h"
#include <stdint.h>
#include <stdbool.h>

/* 任务参数 */
#define SEND_TASK_STACK_SIZE        (configMINIMAL_STACK_SIZE * 4)  // 增加栈大小
#define SEND_TASK_PRIORITY          (tskIDLE_PRIORITY + 3)
#define SEND_QUEUE_LENGTH           20
#define SEND_TIMEOUT_MS             1000
#define AT_RESPONSE_TIMEOUT_MS      5000

/* 共享串口参数 */
#define SHARED_UART_ID              UART_ID_LTE     /**< BLE和4G共享的串口ID */
#define MODULE_SWITCH_DELAY_MS      100             /**< 模块切换延时 */
#define MODULE_INIT_DELAY_MS        500             /**< 模块初始化延时 */

/* 发送消息类型 */
typedef enum {
    SEND_TYPE_4G_AT = 0,        // 4G AT命令
    SEND_TYPE_BLE_AT,           // BLE AT命令
    SEND_TYPE_RS485_DATA,       // RS485数据
    SEND_TYPE_HEARTBEAT,        // 心跳数据
    SEND_TYPE_SHARED_AT,        // 共享AT命令（自动识别模块）
    SEND_TYPE_QUERY             // 查询命令
} SendMessageType_t;

/* 发送消息结构 */
typedef struct {
    SendMessageType_t type;         // 消息类型
    UART_ID_t target_uart;          // 目标串口
    uint8_t data[256];              // 发送数据
    uint16_t length;                // 数据长度
    uint32_t timeout_ms;            // 超时时间
    bool wait_response;             // 是否等待响应
    char expected_response[64];     // 期望的响应
    uint32_t sequence_id;           // 序列号（新增）
    void (*callback)(const char* response, AT_Result_t result);  // 回调函数（新增）
} LteSendMessage_t;

/* RS485发送消息类型 */
typedef enum {
    RS485_TX_TYPE_DATA = 0,         // 接收到数据
    RS485_TX_TYPE_COMMAND,          // 接收到命令
    RS485_TX_TYPE_RESPONSE,         // 接收到响应
    RS485_TX_TYPE_ERROR,            // 接收到错误消息
    RS485_TX_TYPE_TIMEOUT           // 接收超时
} Rs485SendMessageType_t;

/* RS485发送消息结构 */
typedef struct {
    Rs485SendMessageType_t type;      // 消息类型
    UART_ID_t source_uart;          // 源串口ID
    uint8_t data[256];              // 接收数据
    uint16_t length;                // 数据长度
    uint32_t timestamp;             // 时间戳
    uint8_t device_addr;            // 设备地址
    uint8_t function_code;          // 功能码
    uint16_t crc;                   // CRC校验码
    bool crc_valid;                 // CRC校验是否有效
    uint32_t sequence_id;           // 序列号
} Rs485SendMessage_t;

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
BaseType_t commu_send_init(void);






/* ============================= 便捷宏定义 ============================= */

/**
 * @brief 4G AT指令发送宏 
 * @param cmd_str AT指令字符串
 * @param expected_resp 期望响应
 * @param timeout 超时时间
 */
#define SEND_4G_AT_COMMAND(cmd_str, expected_resp, timeout) \
    commu_send_module_command(MODULE_TYPE_4G, COMMAND_TYPE_AT, \
                           cmd_str, strlen(cmd_str), timeout, NULL, NULL)

/**
 * @brief BLE指令发送宏
 * @param cmd_str BLE指令字符串  
 * @param timeout 超时时间
 */
#define SEND_BLE_COMMAND(cmd_str, timeout) \
    commu_send_module_command(MODULE_TYPE_BLE, COMMAND_TYPE_AT, \
                           cmd_str, strlen(cmd_str), timeout, NULL, NULL)

/**
 * @brief RS485数据发送宏
 * @param data_ptr 数据指针
 * @param data_len 数据长度
 */
#define SEND_RS485_DATA(data_ptr, data_len) \
    commu_send_module_data(MODULE_TYPE_RS485, data_ptr, data_len, 1000)

/* 便捷宏定义 */

/**
 * @brief 发送简单的4G AT命令（不等待响应）
 * @param cmd AT命令字符串
 */
#define SEND_4G_CMD(cmd) \
    uart_send(UART_ID_LTE, (const uint8_t*)cmd, strlen(cmd), 1000)

/**
 * @brief 发送4G AT命令并等待OK响应
 * @param cmd AT命令字符串
 */
#define SEND_4G_CMD_OK(cmd) \
    uart_send(UART_ID_LTE, (const uint8_t*)cmd, strlen(cmd), 3000)

/**
 * @brief 发送RS485字符串数据
 * @param str 字符串指针
 */
#define SEND_RS485_STRING(str) \
    uart_send(UART_ID_RS485, (const uint8_t*)str, strlen(str), 1000)

/**
 * @brief 发送LOG串口数据
 * @param data 数据指针
 * @param len 数据长度
 */
#define SEND_LOG_DATA(data, len) \
    uart_send(UART_ID_LOG, data, len, 1000)




#endif /* __TSK_COMMU_SEND_H */
