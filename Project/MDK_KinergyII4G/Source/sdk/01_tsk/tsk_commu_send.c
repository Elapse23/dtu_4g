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
static void process_lte_message(const LteSendMessage_t* message);
static void process_rs485_message(const Rs485SendMessage_t* message);
static bool send_lte_data(const char* data, uint32_t length);
static void send_heartbeat_data(void);

/* 公共函数声明 */
BaseType_t CommuSend_LTECommand(const AT_Cmd_Config_t* at_cmd);
BaseType_t CommuSend_Rs485Data(const uint8_t* data, uint16_t length);


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
            process_lte_message(&lte_tx_message);
        }
        /* 再处理 RS485 */
        else if (xQueueReceive(s_rs485_send_queue, &rs485_tx_message, 0) == pdTRUE) {
            process_rs485_message(&rs485_tx_message);
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
BaseType_t CommuSend_Init(void)
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
 * @brief 统一的UART数据发送接口
 * @param uart_id 串口ID
 * @param data 要发送的数据
 * @param length 数据长度
 * @param timeout_ms 超时时间（毫秒）
 * @return BaseType_t 发送结果 (pdPASS/pdFAIL)
 */
BaseType_t CommuSend_UartData(UART_ID_t uart_id, const uint8_t* data, uint32_t length, uint32_t timeout_ms)
{
    UART_Status_t uart_result;
    if (uart_id == UART_ID_LTE) {
        uart_result = UART_RingBuffer_Send(uart_id, data, length, timeout_ms);
        LOG_DEBUG(LOG_MODULE_PROTOCOL, "LTE direct send result: %s, data: %.*s", 
                 uart_result == UART_OK ? "SUCCESS" : "FAILED", 
                 (int)length, (const char*)data);
    } else {
        uart_result = UART_RingBuffer_Send(uart_id, data, length, timeout_ms);
    }
    return (uart_result == UART_OK) ? pdPASS : pdFAIL;
}

/**
 * @brief 处理发送消息
 * @param message 发送消息指针
 */
static void process_lte_message(const LteSendMessage_t* message)
{
    bool send_result = false;
    if (!message)return;
    
    send_result = send_lte_data((const char*)message->data, message->length);
    LOG_INFO(LOG_MODULE_PROTOCOL, "4G AT command: %s, Result: %s", 
            message->data, send_result ? "SUCCESS" : "FAILED");
}

/**
 * @brief 处理发送消息
 * @param message 发送消息指针
 */
static void process_rs485_message(const Rs485SendMessage_t* message)
{
    BaseType_t send_result;
    
    if (!message) {
        return;
    }
    send_result = CommuSend_UartData(UART_ID_RS485, message->data, message->length, SEND_TIMEOUT_MS);
    LOG_INFO(LOG_MODULE_PROTOCOL, "RS485 data sent (%d bytes), Result: %s", 
            message->length, send_result == pdPASS ? "SUCCESS" : "FAILED");

}

/**
 * @brief 发送lte 数据
 * @param data 数据内容
 * @param length 数据长度
 * @return bool 发送结果
 */
static bool send_lte_data(const char* data, uint32_t length)
{
    BaseType_t send_result;
    send_result = CommuSend_UartData(UART_ID_LTE, (const uint8_t*)data, length, SEND_TIMEOUT_MS);
    LOG_INFO(LOG_MODULE_PROTOCOL, "LTE data sent (%d bytes), Result: %s", 
                length, send_result ? "SUCCESS" : "FAILED");
    return (send_result == pdPASS);
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
        LteState_t state = Lte_GetState();
        if (state >= LTE_STATE_READY) {
            static const AT_Cmd_Config_t at_cmd = {
                .module_type = MODULE_TYPE_4G,
                .at_cmd = "AT",
                .expected_resp = "OK",
                .timeout_ms = 2000,
                .retries = 1,
                .description = "Heartbeat test",
                .critical = false,
                .callback = NULL
            };
            CommuSend_LTECommand(&at_cmd);
        }
    }
    
    heartbeat_counter++;
}

/**
 * @brief 发送LTE AT命令
 * @param at_cmd AT命令配置结构
 * @return BaseType_t 发送结果
 */
BaseType_t CommuSend_LTECommand(const AT_Cmd_Config_t* at_cmd)
{
    if (!at_cmd) {
        return pdFAIL;
    }
    
    // 直接使用execute_at_command_with_config函数执行AT命令
    AT_Result_t result = execute_at_command_with_config(at_cmd);
    
    // 将AT_Result_t转换为BaseType_t
    return (result == AT_RESULT_OK) ? pdPASS : pdFAIL;
}


/**
 * @brief 发送RS485数据
 * @param data 数据指针
 * @param length 数据长度
 * @return BaseType_t 发送结果
 */
BaseType_t CommuSend_Rs485Data(const uint8_t* data, uint16_t length)
{
    if (!data || length == 0) {
        return pdFAIL;
    }
    
    // 直接通过UART发送RS485数据
    return CommuSend_UartData(UART_ID_RS485, data, length, 1000);
}

