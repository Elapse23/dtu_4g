/**
 * =====================================================================================
 * @file        tsk_commu_receive.h
 * @brief       通信接收任务头文件
 * @author      23Elapse & GitHub Copilot
 * @version     1.0
 * @date        2025-09-03
 * @note        通信接收任务的公共接口定义
 * =====================================================================================
 */

#ifndef __TSK_COMMU_RECEIVE_H
#define __TSK_COMMU_RECEIVE_H

#include "FreeRTOS.h"

/* 通信处理状态 */
typedef enum {
    COMMU_STATE_IDLE = 0,       // 空闲状态
    COMMU_STATE_RECEIVING,      // 接收数据状态
    COMMU_STATE_PROCESSING,     // 处理数据状态
    COMMU_STATE_RESPONDING      // 发送响应状态
} CommuState_t;

/* 公共函数声明 */

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
 * @return CommuState_t 当前状态
 */
CommuState_t CommuReceive_GetState(void);

#endif /* __TSK_COMMU_RECEIVE_H */
