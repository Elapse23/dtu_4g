/*
 * @Author: 23Elapse userszy@163.com
 * @Date: 2025-09-03 14:59:54
 * @LastEditors: 23Elapse userszy@163.com
 * @LastEditTime: 2025-09-09 10:33:57
 * @FilePath: \DTU_4G\Project\MDK_KinergyII4G\Source\sdk\01_tsk\tsk_commu_receive.h
 * @Description: 
 * 
 * Copyright (c) 2025 by 23Elapse userszy@163.com, All Rights Reserved. 
 */
/**
 * =====================================================================================
 * @file        tsk_commu_receive.h
 * @brief       统一通信接收任务头文件
 * @author      23Elapse & GitHub Copilot
 * @version     2.0
 * @date        2025-09-08
 * @note        统一的模块通信接收接口 - 支持数据路由和回调注册
 * =====================================================================================
 */

#ifndef __TSK_COMMU_RECEIVE_H
#define __TSK_COMMU_RECEIVE_H

#include "FreeRTOS.h"
#include "tsk_commu_send.h"  // 包含 ModuleType_t 定义
#include <stdint.h>
#include <stdbool.h>
/* 任务参数 */
#define COMMU_TASK_STACK_SIZE       (configMINIMAL_STACK_SIZE * 2)
#define COMMU_TASK_PRIORITY         (tskIDLE_PRIORITY + 4)
#define COMMU_PROCESS_DELAY_MS      50

/* 接收缓冲区大小 */
#define RX_BUFFER_SIZE              256
#define MAX_MESSAGE_LENGTH          128

/* 数据来源枚举（与tsk_commu_send.c保持一致） */
typedef enum {
    DATA_SOURCE_4G = 0,             /**< 4G模块数据 */
    DATA_SOURCE_BLE,                /**< BLE模块数据 */
    DATA_SOURCE_UNKNOWN             /**< 未知来源 */
} Data_Source_t;

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

/**
 * @brief 初始化通信接收任务
 * @return BaseType_t 初始化结果 (pdPASS/pdFAIL)
 * @note 创建通信接收处理任务
 */
BaseType_t CommuReceive_Init(void);

/**
 * @brief 通信接收任务主函数
 * @param pvParameters 任务参数（未使用）
 * @note 这是FreeRTOS任务函数，由任务调度器调用
 */
void vCommuReceiveTask(void* pvParameters);


/**
 * @brief 获取通信接收任务当前状态
 * @return 当前状态
 */
CommuState_t CommuReceive_GetState(void);

/* 私有函数声明 */
static void process_uart_data(UART_ID_t uart_id);
static void handle_received_message(CommuMessage_t* message);
static void send_response(UART_ID_t uart_id, const char* response);
static BaseType_t parse_command(const uint8_t* data, uint16_t length, char* response, uint16_t max_response_len);

/* 新增：共享串口数据路由函数 */
static Data_Source_t identify_data_source(const uint8_t* data, size_t length);
static bool is_4g_response(const uint8_t* data, size_t length);
static bool is_ble_response(const uint8_t* data, size_t length);

void vCommuReceiveTask(void* pvParameters);


#endif /* __TSK_COMMU_RECEIVE_H */
