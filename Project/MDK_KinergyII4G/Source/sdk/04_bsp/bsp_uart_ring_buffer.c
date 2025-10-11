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

/* 外部变量声明 - LTE数据监控标志 */
extern bool s_lte_monitor_enabled;

// NOTE: 2025-09 已移除LOG<->LTE转发功能，相关标志删除。

// 静态缓冲区数组
static uint8_t rs485_rx_buffer_storage[UART_RX_BUFFER_SIZE];
static uint8_t rs485_tx_buffer_storage[UART_TX_BUFFER_SIZE];
// static uint8_t lte_rx_buffer_storage[UART_RX_BUFFER_SIZE * 5];
// static uint8_t lte_tx_buffer_storage[UART_TX_BUFFER_SIZE * 5];
static uint8_t log_rx_buffer_storage[UART_RX_BUFFER_SIZE];
static uint8_t log_tx_buffer_storage[UART_TX_BUFFER_SIZE];
uint8_t lte_rx_buffer_storage[UART_RX_BUFFER_SIZE * 5];
uint8_t lte_tx_buffer_storage[UART_TX_BUFFER_SIZE * 5];

// 调试用静态变量
static uint32_t s_lte_irq_count = 0;  // LTE中断计数器
static uint32_t s_lte_send_count = 0; // LTE发送字节计数器
static uint32_t s_lte_write_ok_count = 0; // LTE成功写入环形缓冲区计数
static uint32_t s_lte_write_fail_count = 0; // LTE写入环形缓冲区失败计数
static uint8_t s_lte_debug_bytes[8]; // LTE最近接收的调试数据
static uint32_t s_lte_debug_index = 0; // 调试数据索引

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
 */
UART_Status_t uart_send(UART_ID_t uart_id, const uint8_t* data, uint32_t length, uint32_t timeout_ms) {
    UART_RingBuffer_t* uart_dev = get_uart_device(uart_id);
    if (!uart_dev || !uart_dev->is_initialized || !data || length == 0) {
        return UART_ERR_PARAM;
    }    
    // 增加信号量有效性检查
    if (!uart_dev->tx_mutex || !uart_dev->tx_complete_semaphore) {
        return UART_ERR_INIT;
    }
    
    // 检查 FreeRTOS 调度器状态
    BaseType_t scheduler_state = xTaskGetSchedulerState();
    if (scheduler_state == taskSCHEDULER_NOT_STARTED) {
        // 调度器未启动，使用轮询方式发送
        return send_polling(uart_id, data, length, timeout_ms);
    } else if (scheduler_state != taskSCHEDULER_RUNNING) {
        return UART_ERR_INIT;
    }

    // 限制超时时间范围，防止溢出
    if (timeout_ms > 60000) {  // 最大60秒
        timeout_ms = 60000;
    }
    
    // 获取发送互斥锁
    if (xSemaphoreTake(uart_dev->tx_mutex, pdMS_TO_TICKS(timeout_ms)) != pdTRUE) {
        UART_LOG_INFO("UART %d send busy", uart_id);
        return UART_ERR_TIMEOUT;
    }
    UART_Status_t result = UART_OK;

    // RS485需要切换到发送模式
    if (uart_id == UART_ID_RS485) {
        RS485_DIR_TX();
    }
    
    // 启动发送 - 简化为直接轮询发送，避免缓冲区竞争条件
    USART_Module* USARTx = (USART_Module*)uart_dev->uart_instance;
    
    // LTE调试：记录发送数据
    if (uart_id == UART_ID_LTE) {
        // 注意：由于在互斥锁内，这里应该避免使用可能导致任务切换的LOG函数
        // 但为了调试，我们记录一下发送的字节数
        s_lte_send_count += length;
        
        // 如果启用了LTE监控，将数据同步发送到LOG串口
        if (s_lte_monitor_enabled) {
            // 发送数据到LOG串口 - 使用轮询方式避免任务切换
            send_polling(UART_ID_LOG, data, length, 1000);
        }
    }
    
    for (uint32_t i = 0; i < length; i++) {
        // 等待发送数据寄存器空中断标志位
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXDE) == RESET);
        USART_SendData(USARTx, data[i]);
    }
    // 等待传输完成标志 (确保最后一个字节完全发送出去)
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXC) == RESET);


    // RS485发送完成后切换回接收模式
    if (uart_id == UART_ID_RS485) {
        // 等待发送完全完成
        while (USART_GetFlagStatus((USART_Module*)uart_dev->uart_instance, USART_FLAG_TXC) == RESET);
        RS485_DIR_RX();
    }
    
    // 释放互斥锁
    xSemaphoreGive(uart_dev->tx_mutex);
    
    return result;
}

/**
 * @brief 轮询方式发送数据（用于调度器未启动时）
 */
static UART_Status_t send_polling(UART_ID_t uart_id, const uint8_t* data, uint32_t length, uint32_t timeout_ms) {
    UART_RingBuffer_t* uart_dev = get_uart_device(uart_id);
    if (!uart_dev || !uart_dev->is_initialized || !data || length == 0) {
        return UART_ERR_PARAM;
    }
    
    USART_Module* USARTx = (USART_Module*)uart_dev->uart_instance;
    
    // RS485需要切换到发送模式
    if (uart_id == UART_ID_RS485) {
        RS485_DIR_TX();
    }
    
    // 逐字节发送
    for (uint32_t i = 0; i < length; i++) {
        // 等待发送缓冲区空
        uint32_t timeout_count = timeout_ms * 1000; // 粗略的超时计数
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXDE) == RESET) {
            if (timeout_count-- == 0) {
                if (uart_id == UART_ID_RS485) {
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
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXC) == RESET) {
        if (timeout_count-- == 0) {
            if (uart_id == UART_ID_RS485) {
                RS485_DIR_RX();
            }
            return UART_ERR_TIMEOUT;
        }
    }
    
    // RS485发送完成后切换回接收模式
    if (uart_id == UART_ID_RS485) {
        RS485_DIR_RX();
    }
    
    return UART_OK;
}

/**
 * @brief 接收数据（阻塞方式）
 */
UART_Status_t uart_receive(UART_ID_t uart_id, uint8_t* data, uint32_t length, uint32_t timeout_ms) {
    UART_RingBuffer_t* uart_dev = get_uart_device(uart_id);
    if (!uart_dev || !uart_dev->is_initialized || !data || length == 0) {
        return UART_ERR_PARAM;
    }
    
    uint32_t received = 0;
    TickType_t start_time = xTaskGetTickCount();
    TickType_t timeout_ticks = pdMS_TO_TICKS(timeout_ms);
    
    while (received < length) {
        // 尝试从环形缓冲区读取数据
        if (ring_buffer_read(&uart_dev->rx_ring_buffer, &data[received]) == RB_OK) {
            received++;
        } else {
            // 没有数据，等待接收信号量
            TickType_t elapsed = xTaskGetTickCount() - start_time;
            if (elapsed >= timeout_ticks) {
                return UART_ERR_TIMEOUT;
            }
            
            TickType_t remaining = timeout_ticks - elapsed;
            if (xSemaphoreTake(uart_dev->rx_semaphore, remaining) != pdTRUE) {
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
 * @brief LTE调试统计信息获取
 */
uint32_t uart_get_lte_irq_count(void) { return s_lte_irq_count; }
uint32_t uart_get_lte_send_count(void) { return s_lte_send_count; }

void uart_get_lte_write_stats(uint32_t* write_ok, uint32_t* write_fail) {
    if (write_ok) *write_ok = s_lte_write_ok_count;
    if (write_fail) *write_fail = s_lte_write_fail_count;
}

uint32_t uart_get_lte_debug_data(uint8_t* debug_data) {
    if (debug_data && s_lte_write_ok_count > 0) {
        memcpy(debug_data, s_lte_debug_bytes, 8);
        return (s_lte_write_ok_count > 8) ? 8 : s_lte_write_ok_count;
    }
    return 0;
}

/**
 * @brief 获取接收缓冲区中可读取的字节数
 */
uint32_t uart_get_available_bytes(UART_ID_t uart_id) {
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
void uart_irq_handler(UART_ID_t uart_id) {
    UART_RingBuffer_t* uart_dev = get_uart_device(uart_id);
    if (!uart_dev || !uart_dev->is_initialized) {
        return;
    }
    
    USART_Module* USARTx = (USART_Module*)uart_dev->uart_instance;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    // 处理接收中断
    if (USART_GetIntStatus(USARTx, USART_INT_RXDNE) == SET) {
        uint8_t received_byte = USART_ReceiveData(USARTx);
        
        // LTE串口调试计数
        if (uart_id == UART_ID_LTE) {
            s_lte_irq_count++;
        }
        
        // 将接收到的字节写入环形缓冲区
        RB_Status rb_result = ring_buffer_write_from_isr(&uart_dev->rx_ring_buffer, &received_byte, &xHigherPriorityTaskWoken);
        if (rb_result == RB_OK) {
            // 写入成功
            if (uart_id == UART_ID_LTE) {
                s_lte_write_ok_count++;
                // 记录最新接收的几个字节用于调试
                s_lte_debug_bytes[s_lte_debug_index] = received_byte;
                s_lte_debug_index = (s_lte_debug_index + 1) % 8;
            }
            // 通知有新数据到达
            xSemaphoreGiveFromISR(uart_dev->rx_semaphore, &xHigherPriorityTaskWoken);
        } else {
            // 写入失败
            if (uart_id == UART_ID_LTE) {
                s_lte_write_fail_count++;
            }
        }
    }
    
    // 如果有更高优先级的任务被唤醒，进行任务切换
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}



// (原转发控制API已移除)
