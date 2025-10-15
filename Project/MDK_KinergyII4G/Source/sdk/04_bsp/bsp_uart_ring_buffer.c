/**
 * =====================================================================================
 * @file        bsp_uart_ring_buffer.c
 * @brief       基于环形缓冲区的串口收发驱动实现
 * @author      23Elapse
 * @version     1.0
 * @date        2025-09-02
 * @note        使用环形缓冲区实现串口数据的缓冲收发，支持多串口
 * =====================================================================================
 */

#include "bsp_uart_ring_buffer.h"
#include "bsp_log_manager.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "portmacro.h"
#include "n32g45x.h"
#include <string.h>
#include <stdio.h>

// 静态缓冲区数组
static uint8_t rs485_rx_buffer_storage[UART_RX_BUFFER_SIZE];
static uint8_t rs485_tx_buffer_storage[UART_TX_BUFFER_SIZE];
static uint8_t log_rx_buffer_storage[UART_RX_BUFFER_SIZE];
static uint8_t log_tx_buffer_storage[UART_TX_BUFFER_SIZE];
uint8_t lte_rx_buffer_storage[UART_RX_BUFFER_SIZE * 5];
uint8_t lte_tx_buffer_storage[UART_TX_BUFFER_SIZE * 5];

// 发送中断相关状态变量
typedef enum {
    TX_STATE_IDLE = 0,          // 发送空闲
    TX_STATE_BUSY,              // 发送进行中
    TX_STATE_COMPLETE           // 发送完成
} TX_State_t;

static TX_State_t s_tx_states[UART_COUNT] = {TX_STATE_IDLE}; // 各串口发送状态

// 串口设备实例
static UART_RingBuffer_t uart_devices[UART_COUNT];

// 内部函数声明
static inline UART_RingBuffer_t* get_uart_device(UART_ID_t uart_id);
static UART_Status_t send_polling(UART_ID_t uart_id, const uint8_t* data, uint32_t length, uint32_t timeout_ms);


/**
 * @brief 获取串口设备实例
 */
static inline UART_RingBuffer_t* get_uart_device(UART_ID_t uart_id) {
    return (uart_id < UART_COUNT) ? &uart_devices[uart_id] : NULL;
}

/**
 * @brief 初始化串口环形缓冲区驱动
 */
UART_Status_t uart_init(UART_ID_t uart_id, uint32_t baudrate) {
    if (uart_id >= UART_COUNT) {
        return UART_ERR_PARAM;
    }
    
    UART_RingBuffer_t* uart_dev = &uart_devices[uart_id];
    
    // 配置硬件参数
    switch (uart_id) {
        case UART_ID_RS485:
            uart_dev->uart_instance = RS485_UART;
            uart_dev->tx_buffer_storage = rs485_tx_buffer_storage;
            uart_dev->rx_buffer_storage = rs485_rx_buffer_storage;
            uart_dev->irqn = RS485_IRQn;
            break;
            
        case UART_ID_LTE:
            uart_dev->uart_instance = LTE_UART;
            uart_dev->tx_buffer_storage = lte_tx_buffer_storage;
            uart_dev->rx_buffer_storage = lte_rx_buffer_storage;
            uart_dev->irqn = LTE_IRQn;
            break;
            
        case UART_ID_LOG:
            uart_dev->uart_instance = LOG_UART;
            uart_dev->tx_buffer_storage = log_tx_buffer_storage;
            uart_dev->rx_buffer_storage = log_rx_buffer_storage;
            uart_dev->irqn = LOG_IRQn; // 启用LOG串口接收中断以支持调试命令
            break;
            
        default:
            return UART_ERR_PARAM;
    }
    
    uart_dev->uart_id = uart_id;
    uart_dev->baudrate = baudrate;
    uart_dev->is_initialized = false;
    
    // 初始化接收环形缓冲区
    if (ring_buffer_init(&uart_dev->rx_ring_buffer, 
                       uart_dev->rx_buffer_storage,
                       UART_RX_BUFFER_SIZE,
                       sizeof(uint8_t)) != RB_OK) {
        return UART_ERR_INIT;
    }
    
    // 初始化发送环形缓冲区
    if (ring_buffer_init(&uart_dev->tx_ring_buffer,
                       uart_dev->tx_buffer_storage,
                       UART_TX_BUFFER_SIZE,
                       sizeof(uint8_t)) != RB_OK) {
        ring_buffer_deinit(&uart_dev->rx_ring_buffer);
        return UART_ERR_INIT;
    }
    
    // 创建互斥锁
    uart_dev->tx_mutex = xSemaphoreCreateMutex();
    if (!uart_dev->tx_mutex) {
        ring_buffer_deinit(&uart_dev->rx_ring_buffer);
        ring_buffer_deinit(&uart_dev->tx_ring_buffer);
        return UART_ERR_INIT;
    }
    
    // 创建接收信号量
    uart_dev->rx_semaphore = xSemaphoreCreateBinary();
    if (!uart_dev->rx_semaphore) {
        vSemaphoreDelete(uart_dev->tx_mutex);
        ring_buffer_deinit(&uart_dev->rx_ring_buffer);
        ring_buffer_deinit(&uart_dev->tx_ring_buffer);
        return UART_ERR_INIT;
    }
    
    // 创建发送完成信号量
    uart_dev->tx_complete_semaphore = xSemaphoreCreateBinary();
    if (!uart_dev->tx_complete_semaphore) {
        vSemaphoreDelete(uart_dev->tx_mutex);
        vSemaphoreDelete(uart_dev->rx_semaphore);
        ring_buffer_deinit(&uart_dev->rx_ring_buffer);
        ring_buffer_deinit(&uart_dev->tx_ring_buffer);
        return UART_ERR_INIT;
    }
    
    // 直接调用底层驱动初始化
    if (Serial_Driver_Init(uart_dev->uart_instance, baudrate) != SERIAL_OK) {
        vSemaphoreDelete(uart_dev->tx_mutex);
        vSemaphoreDelete(uart_dev->rx_semaphore);
        vSemaphoreDelete(uart_dev->tx_complete_semaphore);
        ring_buffer_deinit(&uart_dev->rx_ring_buffer);
        ring_buffer_deinit(&uart_dev->tx_ring_buffer);
        return UART_ERR_INIT;
    }
    
    // 初始化发送状态
    s_tx_states[uart_id] = TX_STATE_IDLE;
    
    // 配置串口中断（接收中断默认启用，发送中断按需启用）
    USART_Module* USARTx = (USART_Module*)uart_dev->uart_instance;
    USART_ConfigInt(USARTx, USART_INT_RXDNE, ENABLE);  // 启用接收中断
    // 注意：发送中断将在需要发送数据时动态启用
    
    uart_dev->is_initialized = true;
    return UART_OK;
}

/**
 * @brief 反初始化串口环形缓冲区驱动
 */
UART_Status_t uart_deinit(UART_ID_t uart_id) {
    UART_RingBuffer_t* uart_dev = get_uart_device(uart_id);
    if (!uart_dev || !uart_dev->is_initialized) {
        return UART_ERR_PARAM;
    }
    
    // 直接调用底层驱动反初始化
    Serial_Driver_Deinit(uart_dev->uart_instance);
    
    // 清理资源
    if (uart_dev->tx_mutex) {
        vSemaphoreDelete(uart_dev->tx_mutex);
        uart_dev->tx_mutex = NULL;
    }
    
    if (uart_dev->rx_semaphore) {
        vSemaphoreDelete(uart_dev->rx_semaphore);
        uart_dev->rx_semaphore = NULL;
    }
    
    if (uart_dev->tx_complete_semaphore) {
        vSemaphoreDelete(uart_dev->tx_complete_semaphore);
        uart_dev->tx_complete_semaphore = NULL;
    }
    
    ring_buffer_deinit(&uart_dev->rx_ring_buffer);
    ring_buffer_deinit(&uart_dev->tx_ring_buffer);
    
    uart_dev->is_initialized = false;
    return UART_OK;
}

/**
 * @brief 发送数据（阻塞方式）
 * @param uart_id       串口ID
 * @param data          要发送的数据指针
 * @param length        数据长度
 * @param timeout_ms    超时时间（毫秒）
 * @return UART_Status_t 发送状态
 */
UART_Status_t uart_send(UART_ID_t uart_id, const uint8_t* data, uint32_t length, uint32_t timeout_ms) 
{
    // 1. 参数有效性检查
    UART_RingBuffer_t* uart_dev = get_uart_device(uart_id);
    if (!uart_dev || !uart_dev->is_initialized || !data || length == 0) 
    {
        return UART_ERR_PARAM;
    }
    if (!uart_dev->tx_mutex || !uart_dev->tx_complete_semaphore) 
    {
        return UART_ERR_INIT;
    }
    // 2. 检查调度器状态，决定发送方式
    BaseType_t scheduler_state = xTaskGetSchedulerState();
    if (scheduler_state == taskSCHEDULER_NOT_STARTED) 
    {
        return send_polling(uart_id, data, length, timeout_ms);
    } 
    else if (scheduler_state != taskSCHEDULER_RUNNING) 
    {
        return UART_ERR_INIT;
    }
    // 3. 检查串口发送状态
    if (s_tx_states[uart_id] == TX_STATE_BUSY) 
    {
        return UART_ERR_TIMEOUT;
    }
    // 4. 获取发送互斥锁
    if (xSemaphoreTake(uart_dev->tx_mutex, pdMS_TO_TICKS(timeout_ms)) != pdTRUE) 
    {
        UART_LOG_INFO("UART %d send busy", uart_id);
        return UART_ERR_TIMEOUT;
    }   
    UART_Status_t result = UART_OK;
    USART_Module* USARTx = (USART_Module*)uart_dev->uart_instance;
    // 5. RS485切换到发送模式
    if (uart_id == UART_ID_RS485) 
    {
        RS485_DIR_TX();
    }
    // 6. 将数据写入发送环形缓冲区
    uint32_t written = 0;
    for (uint32_t i = 0; i < length; i++) 
    {
        if (ring_buffer_write(&uart_dev->tx_ring_buffer, &data[i]) == RB_OK) 
        {
            written++;
        } 
        else 
        {
            result = UART_ERR_BUFFER_FULL;
            break;
        }
    }
    // 7. 启动中断发送并等待完成
    if (written > 0) 
    {
        // 设置发送状态并启用中断
        s_tx_states[uart_id] = TX_STATE_BUSY;
        USART_ConfigInt(USARTx, USART_INT_TXC, ENABLE);
        // 发送第一个字节启动发送过程
        uint8_t first_byte;
        if (ring_buffer_read(&uart_dev->tx_ring_buffer, &first_byte) == RB_OK) 
        {
            USART_SendData(USARTx, first_byte);
        }
        // 等待发送完成
        if (xSemaphoreTake(uart_dev->tx_complete_semaphore, pdMS_TO_TICKS(timeout_ms)) == pdTRUE) 
        {
            s_tx_states[uart_id] = TX_STATE_IDLE;
        } 
        else 
        {
            // 发送超时处理
            s_tx_states[uart_id] = TX_STATE_IDLE;
            USART_ConfigInt(USARTx, USART_INT_TXC, DISABLE);
            ring_buffer_clear(&uart_dev->tx_ring_buffer);
            result = UART_ERR_TIMEOUT;
        }
    }
    // 8. RS485切换回接收模式
    if (uart_id == UART_ID_RS485) 
    {
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXC) == RESET);
        RS485_DIR_RX();
    }
    // 9. 释放互斥锁
    xSemaphoreGive(uart_dev->tx_mutex);
    return result;
}

/**
 * @brief 轮询方式发送数据（用于调度器未启动时）
 */
static UART_Status_t send_polling(UART_ID_t uart_id, const uint8_t* data, uint32_t length, uint32_t timeout_ms) 
{
    UART_RingBuffer_t* uart_dev = get_uart_device(uart_id);
    if (!uart_dev || !uart_dev->is_initialized || !data || length == 0) 
    {
        return UART_ERR_PARAM;
    }
    
    USART_Module* USARTx = (USART_Module*)uart_dev->uart_instance;
    
    // RS485需要切换到发送模式
    if (uart_id == UART_ID_RS485) 
    {
        RS485_DIR_TX();
    }
    
    // 逐字节发送
    for (uint32_t i = 0; i < length; i++) 
    {
        // 等待发送缓冲区空
        uint32_t timeout_count = timeout_ms * 1000; // 粗略的超时计数
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXDE) == RESET) 
        {
            if (timeout_count-- == 0) 
            {
                if (uart_id == UART_ID_RS485) 
                {
                    RS485_DIR_RX();
                }
                return UART_ERR_TIMEOUT;
            }
        }
        // 发送字节
        USART_SendData(USARTx, data[i]);
    }
    // 等待发送完成
    uint32_t timeout_count = timeout_ms * 1000;
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXC) == RESET) 
    {
        if (timeout_count-- == 0) 
        {
            if (uart_id == UART_ID_RS485) 
            {
                RS485_DIR_RX();
            }
            return UART_ERR_TIMEOUT;
        }
    }
    // RS485发送完成后切换回接收模式
    if (uart_id == UART_ID_RS485) 
    {
        RS485_DIR_RX();
    }
    return UART_OK;
}

/**
 * @brief 接收数据（阻塞方式）
 */
UART_Status_t uart_receive(UART_ID_t uart_id, uint8_t* data, uint32_t length, uint32_t timeout_ms) {
    UART_RingBuffer_t* uart_dev = get_uart_device(uart_id);
    if (!uart_dev || !uart_dev->is_initialized || !data || length == 0) 
    {
        return UART_ERR_PARAM;
    }
    
    uint32_t received = 0;
    TickType_t start_time = xTaskGetTickCount();
    TickType_t timeout_ticks = pdMS_TO_TICKS(timeout_ms);
    
    while (received < length) 
    {
        // 尝试从环形缓冲区读取数据
        if (ring_buffer_read(&uart_dev->rx_ring_buffer, &data[received]) == RB_OK) 
        {
            received++;
        } 
        else 
        {
            // 没有数据，等待接收信号量
            TickType_t elapsed = xTaskGetTickCount() - start_time;
            if (elapsed >= timeout_ticks) 
            {
                return UART_ERR_TIMEOUT;
            }
            
            TickType_t remaining = timeout_ticks - elapsed;
            if (xSemaphoreTake(uart_dev->rx_semaphore, remaining) != pdTRUE) 
            {
                return UART_ERR_TIMEOUT;
            }
        }
    }
    
    return UART_OK;
}

/**
 * @brief 非阻塞读取一个字节
 */
UART_Status_t uart_read_byte(UART_ID_t uart_id, uint8_t* data) {
    UART_RingBuffer_t* uart_dev = get_uart_device(uart_id);
    if (!uart_dev || !uart_dev->is_initialized || !data) {
        return UART_ERR_PARAM;
    }
    
    if (ring_buffer_read(&uart_dev->rx_ring_buffer, data) == RB_OK) {
        return UART_OK;
    }
    
    return UART_ERR_NO_DATA;
}

/**
 * @brief 获取串口发送状态
 * @param uart_id 串口ID
 * @return true=发送忙碌中, false=发送空闲
 */
bool uart_is_tx_busy(UART_ID_t uart_id) {
    if (uart_id >= UART_COUNT) {
        return false;
    }
    return (s_tx_states[uart_id] == TX_STATE_BUSY);
}

/**
 * @brief 获取接收缓冲区中可读取的字节数
 */
uint32_t uart_get_available_bytes(UART_ID_t uart_id) 
{
    UART_RingBuffer_t* uart_dev = get_uart_device(uart_id);
    return (uart_dev && uart_dev->is_initialized) ? uart_dev->rx_ring_buffer.count : 0;
}

/**
 * @brief 清空接收缓冲区
 */
UART_Status_t uart_flush_rx(UART_ID_t uart_id) {
    UART_RingBuffer_t* uart_dev = get_uart_device(uart_id);
    if (!uart_dev || !uart_dev->is_initialized) {
        return UART_ERR_PARAM;
    }
    
    ring_buffer_clear(&uart_dev->rx_ring_buffer);
    return UART_OK;
}

/**
 * @brief 清空发送缓冲区
 */
UART_Status_t uart_flush_tx(UART_ID_t uart_id) {
    UART_RingBuffer_t* uart_dev = get_uart_device(uart_id);
    if (!uart_dev || !uart_dev->is_initialized) {
        return UART_ERR_PARAM;
    }
    
    ring_buffer_clear(&uart_dev->tx_ring_buffer);
    return UART_OK;
}

/**
 * @brief 串口中断处理函数
 */
void uart_irq_handler(UART_ID_t uart_id) 
{
    UART_RingBuffer_t* uart_dev = get_uart_device(uart_id);
    if (!uart_dev || !uart_dev->is_initialized) 
    {
        return;
    }
    
    USART_Module* USARTx = (USART_Module*)uart_dev->uart_instance;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    // 处理接收中断
    if (USART_GetIntStatus(USARTx, USART_INT_RXDNE) == SET) 
    {
        uint8_t received_byte = USART_ReceiveData(USARTx);
        
        // 将接收到的字节写入环形缓冲区
        RB_Status rb_result = ring_buffer_write_from_isr(&uart_dev->rx_ring_buffer, &received_byte);
        if (rb_result == RB_OK) 
        {
            // 通知有新数据到达
            xSemaphoreGiveFromISR(uart_dev->rx_semaphore, &xHigherPriorityTaskWoken);
        }
    }
    
    // 处理发送完成中断（每个字节发送完成后触发）
    if (USART_GetIntStatus(USARTx, USART_INT_TXC) == SET) 
    {
        // 清除发送完成标志
        USART_ClrIntPendingBit(USARTx, USART_INT_TXC);
        uint8_t tx_byte;
        // 从发送环形缓冲区读取数据
        if (ring_buffer_read_from_isr(&uart_dev->tx_ring_buffer, &tx_byte) == RB_OK) 
        {
            USART_SendData(USARTx, tx_byte);
        } 
        else 
        {
            // 发送缓冲区空，所有数据发送完成
            // 禁用发送完成中断
            USART_ConfigInt(USARTx, USART_INT_TXC, DISABLE);
            
            // 更新发送状态
            s_tx_states[uart_id] = TX_STATE_COMPLETE;
            
            // RS485发送完成后切换回接收模式
            if (uart_id == UART_ID_RS485) 
            {
                RS485_DIR_RX();
            }
            // 通知发送完成
            xSemaphoreGiveFromISR(uart_dev->tx_complete_semaphore, &xHigherPriorityTaskWoken);
        }
    }
    
    // 如果有更高优先级的任务被唤醒，进行任务切换
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

