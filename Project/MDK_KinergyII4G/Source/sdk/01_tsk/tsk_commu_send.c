/**
 * =====================================================================================
 * @file        tsk_commu_send.c
 * @brief       通信发送任务实现 - 支持BLE和4G共享串口
 * @author      23Elapse & GitHub Copilot
 * @version     2.0
 * @date        2025-09-08
 * @note        处理BLE和4G模块通过共享串口的AT命令发送和RS485串口发送
 * 
 * @architecture_note
 * 共享串口发送架构：
 * 1. BLE和4G模块通过UART_ID_LTE共享物理串口
 * 2. 通过模块切换机制管理串口访问权限
 * 3. 智能识别AT指令类型并路由到相应模块
 * 4. 支持模块状态监控和自动恢复
 * =====================================================================================
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "bsp_uart_ring_buffer.h"
#include "bsp_log_manager.h"
#include "tsk_4g_init.h"
#include <string.h>
#include <stdio.h>
#include "tsk_commu_send.h"

/* 静态变量 */
static QueueHandle_t s_lte_send_queue = NULL;              // LTE串口发送队列
static QueueHandle_t s_rs485_send_queue = NULL;            // RS485串口发送队列

static TaskHandle_t s_send_task_handle = NULL;

/* 私有函数声明 */
static void send_heartbeat_data(void);


/**
 * @brief 通信发送任务主函数 - 支持BLE和4G共享串口
 * @param pvParameters 任务参数（未使用）
 */
void vCommuSendTask(void* pvParameters)
{
    (void)pvParameters;
    LteSendMessage_t lte_tx_message;
    Rs485SendMessage_t rs485_tx_message;
    
    /* 初始化消息接收缓冲区 */
    memset(&lte_tx_message, 0, sizeof(lte_tx_message));
    memset(&rs485_tx_message, 0, sizeof(rs485_tx_message));
    
    /* 任务主循环 */
    while (1) {
        
        /* 优先 LTE 队列 */
        if (xQueueReceive(s_lte_send_queue, &lte_tx_message, 0) == pdTRUE) {
            if (lte_tx_message.data) {
                UART_Status_t uart_result = uart_send(UART_ID_LTE, 
                    lte_tx_message.data, lte_tx_message.length, lte_tx_message.timeout_ms);
                LOG_INFO(LOG_MODULE_PROTOCOL, "LTE data sent (%d bytes), Result: %s", 
                    lte_tx_message.length, uart_result == UART_OK ? "SUCCESS" : "FAILED");
            }
        }
        /* 再处理 RS485 */
        else if (xQueueReceive(s_rs485_send_queue, &rs485_tx_message, 0) == pdTRUE) {
            if (rs485_tx_message.data && rs485_tx_message.length > 0) {
                UART_Status_t uart_result = uart_send(UART_ID_RS485, 
                    rs485_tx_message.data, rs485_tx_message.length, SEND_TIMEOUT_MS);
                LOG_INFO(LOG_MODULE_PROTOCOL, "RS485 data sent (%d bytes), Result: %s", 
                    rs485_tx_message.length, uart_result == UART_OK ? "SUCCESS" : "FAILED");
            }
        }
        else {
            send_heartbeat_data();
        }
    }
}

/**
 * @brief 初始化通信发送任务 - 支持共享模块
 * @return BaseType_t 初始化结果
 */
BaseType_t commu_send_init(void)
{
    BaseType_t result = pdPASS;
    
    LOG_INFO(LOG_MODULE_PROTOCOL, "Initializing shared module communication send task...");
    
    /* 创建发送队列 */
    s_lte_send_queue = xQueueCreate(SEND_QUEUE_LENGTH, sizeof(LteSendMessage_t));
    if (s_lte_send_queue == NULL) {
        LOG_ERROR(LOG_MODULE_PROTOCOL, "Failed to create LTE send queue");
        return pdFAIL;
    }
    
    s_rs485_send_queue = xQueueCreate(SEND_QUEUE_LENGTH, sizeof(LteSendMessage_t));
    if (s_rs485_send_queue == NULL) {
        LOG_ERROR(LOG_MODULE_PROTOCOL, "Failed to create RS485 send queue");
        vQueueDelete(s_lte_send_queue);
        return pdFAIL;
    }

    
    /* 创建发送任务 */
    result = xTaskCreate(vCommuSendTask, "SharedCommSendTask", SEND_TASK_STACK_SIZE, 
                        NULL, SEND_TASK_PRIORITY, &s_send_task_handle);
    
    if (result != pdPASS) {
        LOG_ERROR(LOG_MODULE_PROTOCOL, "Failed to create shared comm send task");
        vQueueDelete(s_lte_send_queue);
        vQueueDelete(s_rs485_send_queue);
        return pdFAIL;
    }
    
    LOG_INFO(LOG_MODULE_PROTOCOL, "Shared module communication send task initialized successfully");
    return pdPASS;
}













/**
 * @brief 发送心跳数据
 */
static void send_heartbeat_data(void)
{
    static uint32_t heartbeat_counter = 0;

    /* 每1000次心跳发送一次4G状态查询 */
    if ((heartbeat_counter % 1000) == 0) {
        /* 检查4G模块状态 */
        LteState_t state = lte_get_state();
        if (state >= LTE_STATE_READY) {
            /* 直接发送AT心跳命令 */
            const char* at_cmd = "AT\r\n";
            uart_send(UART_ID_LTE, (const uint8_t*)at_cmd, strlen(at_cmd), 2000);
        }
    }
    
    heartbeat_counter++;
}






