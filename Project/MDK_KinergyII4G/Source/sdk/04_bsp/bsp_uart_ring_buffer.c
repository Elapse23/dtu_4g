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

// LOG到LTE转发功能控制
static bool s_log_to_lte_forward_enabled = true;   // LOG串口数据转发到LTE串口
static bool s_lte_to_log_forward_enabled = true;   // LTE串口数据转发到LOG串口

// 静态缓冲区数组
static uint8_t rs485_rx_buffer_storage[UART_RX_BUFFER_SIZE];
static uint8_t rs485_tx_buffer_storage[UART_TX_BUFFER_SIZE];
// static uint8_t lte_rx_buffer_storage[UART_RX_BUFFER_SIZE * 5];
// static uint8_t lte_tx_buffer_storage[UART_TX_BUFFER_SIZE * 5];
static uint8_t log_rx_buffer_storage[UART_RX_BUFFER_SIZE];
static uint8_t log_tx_buffer_storage[UART_TX_BUFFER_SIZE];
uint8_t lte_rx_buffer_storage[UART_RX_BUFFER_SIZE * 5];
uint8_t lte_tx_buffer_storage[UART_TX_BUFFER_SIZE * 5];
// 串口设备实例
static UART_RingBuffer_t uart_devices[UART_COUNT];

// 内部函数声明
static Serial_Status_t init_uart_hardware(void* uart_instance, uint32_t baudrate);
static Serial_Status_t deinit_uart_hardware(void* uart_instance);
static UART_RingBuffer_t* get_uart_device(UART_ID_t uart_id);
static UART_Status_t UART_RingBuffer_SendPolling(UART_ID_t uart_id, const uint8_t* data, uint32_t length, uint32_t timeout_ms);

/**
 * @brief 初始化串口硬件（适配现有驱动）
 * @param uart_instance UART实例
 * @param baudrate 波特率
 * @return 初始化状态
 */
static Serial_Status_t init_uart_hardware(void* uart_instance, uint32_t baudrate) {
    if (uart_instance == RS485_UART) {
        return Serial_Driver_Init(RS485_UART, baudrate);
    } else if (uart_instance == LTE_UART) {
        return Serial_Driver_Init(LTE_UART, baudrate);
    } else if (uart_instance == LOG_UART) {
        return Serial_Driver_Init(LOG_UART, baudrate);
    }
    return SERIAL_ERR_PARAM;
}

/**
 * @brief 反初始化串口硬件（适配现有驱动）
 * @param uart_instance UART实例
 * @return 状态
 */
static Serial_Status_t deinit_uart_hardware(void* uart_instance) {
    if (uart_instance == RS485_UART) {
        return Serial_Driver_Deinit(RS485_UART);
    } else if (uart_instance == LTE_UART) {
        return Serial_Driver_Deinit(LTE_UART);
    } else if (uart_instance == LOG_UART) {
        return Serial_Driver_Deinit(LOG_UART);
    }
    return SERIAL_ERR_PARAM;
}
/**
 * @brief 获取串口设备实例
 * @param uart_id 串口ID
 * @return 串口设备指针，失败返回NULL
 */
static UART_RingBuffer_t* get_uart_device(UART_ID_t uart_id) {
    if (uart_id >= UART_COUNT) {
        return NULL;
    }
    return &uart_devices[uart_id];
}

/**
 * @brief 初始化串口环形缓冲区驱动
 * @param uart_id 串口ID
 * @param baudrate 波特率
 * @return UART_Status_t 状态码
 */
UART_Status_t UART_RingBuffer_Init(UART_ID_t uart_id, uint32_t baudrate) {
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
    if (RingBuffer_Init(&uart_dev->rx_ring_buffer, 
                       uart_dev->rx_buffer_storage,
                       UART_RX_BUFFER_SIZE,
                       sizeof(uint8_t)) != RB_OK) {
        return UART_ERR_INIT;
    }
    
    // 初始化发送环形缓冲区
    if (RingBuffer_Init(&uart_dev->tx_ring_buffer,
                       uart_dev->tx_buffer_storage,
                       UART_TX_BUFFER_SIZE,
                       sizeof(uint8_t)) != RB_OK) {
        RingBuffer_Deinit(&uart_dev->rx_ring_buffer);
        return UART_ERR_INIT;
    }
    
    // 创建互斥锁
    uart_dev->tx_mutex = xSemaphoreCreateMutex();
    if (!uart_dev->tx_mutex) {
        RingBuffer_Deinit(&uart_dev->rx_ring_buffer);
        RingBuffer_Deinit(&uart_dev->tx_ring_buffer);
        return UART_ERR_INIT;
    }
    
    // 创建接收信号量
    uart_dev->rx_semaphore = xSemaphoreCreateBinary();
    if (!uart_dev->rx_semaphore) {
        vSemaphoreDelete(uart_dev->tx_mutex);
        RingBuffer_Deinit(&uart_dev->rx_ring_buffer);
        RingBuffer_Deinit(&uart_dev->tx_ring_buffer);
        return UART_ERR_INIT;
    }
    
    // 创建发送完成信号量
    uart_dev->tx_complete_semaphore = xSemaphoreCreateBinary();
    if (!uart_dev->tx_complete_semaphore) {
        vSemaphoreDelete(uart_dev->tx_mutex);
        vSemaphoreDelete(uart_dev->rx_semaphore);
        RingBuffer_Deinit(&uart_dev->rx_ring_buffer);
        RingBuffer_Deinit(&uart_dev->tx_ring_buffer);
        return UART_ERR_INIT;
    }
    
    // 调用适配的硬件初始化函数
    if (init_uart_hardware(uart_dev->uart_instance, baudrate) != SERIAL_OK) {
        vSemaphoreDelete(uart_dev->tx_mutex);
        vSemaphoreDelete(uart_dev->rx_semaphore);
        vSemaphoreDelete(uart_dev->tx_complete_semaphore);
        RingBuffer_Deinit(&uart_dev->rx_ring_buffer);
        RingBuffer_Deinit(&uart_dev->tx_ring_buffer);
        return UART_ERR_INIT;
    }
    
    uart_dev->is_initialized = true;
    return UART_OK;
}

/**
 * @brief 反初始化串口环形缓冲区驱动
 * @param uart_id 串口ID
 * @return UART_Status_t 状态码
 */
UART_Status_t UART_RingBuffer_Deinit(UART_ID_t uart_id) {
    UART_RingBuffer_t* uart_dev = get_uart_device(uart_id);
    if (!uart_dev || !uart_dev->is_initialized) {
        return UART_ERR_PARAM;
    }
    
    // 反初始化硬件
    deinit_uart_hardware(uart_dev->uart_instance);
    
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
    
    RingBuffer_Deinit(&uart_dev->rx_ring_buffer);
    RingBuffer_Deinit(&uart_dev->tx_ring_buffer);
    
    uart_dev->is_initialized = false;
    return UART_OK;
}

/**
 * @brief 发送数据（阻塞方式）
 * @param uart_id 串口ID
 * @param data 要发送的数据
 * @param length 数据长度
 * @param timeout_ms 超时时间（毫秒）
 * @return UART_Status_t 状态码
 */
UART_Status_t UART_RingBuffer_Send(UART_ID_t uart_id, const uint8_t* data, uint32_t length, uint32_t timeout_ms) {
    UART_RingBuffer_t* uart_dev = get_uart_device(uart_id);
    if (!uart_dev || !uart_dev->is_initialized || !data || length == 0) {
        return UART_ERR_PARAM;
    }
    
    // LOG串口数据转发到LTE串口功能
    if (uart_id == UART_ID_LOG && s_log_to_lte_forward_enabled) {
        // 检查数据是否是AT命令格式（以AT开头或者是原始数据）
        if (length >= 2) {
            // 创建带换行符的AT命令
            uint8_t lte_command[length + 2];
            memcpy(lte_command, data, length);
            
            // 如果数据不以\r\n结尾，自动添加
            bool needs_crlf = true;
            if (length >= 2 && data[length-2] == '\r' && data[length-1] == '\n') {
                needs_crlf = false;
            }
            
            uint32_t lte_length = length;
            if (needs_crlf) {
                lte_command[length] = '\r';
                lte_command[length + 1] = '\n';
                lte_length = length + 2;
            }
            
            // 直接发送到LTE串口，避免递归调用
            UART_RingBuffer_t* lte_dev = get_uart_device(UART_ID_LTE);
            if (lte_dev && lte_dev->is_initialized) {
                
                // 直接操作LTE串口的发送缓冲区，避免递归
                UART_Status_t forward_result = UART_ERR_BUSY;
                
                // 检查信号量有效性
                if (lte_dev->tx_mutex && lte_dev->tx_complete_semaphore) {
                    // 获取LTE发送互斥锁
                    if (xSemaphoreTake(lte_dev->tx_mutex, pdMS_TO_TICKS(timeout_ms)) == pdTRUE) {
                        forward_result = UART_OK;
                        
                        // 将数据写入LTE发送缓冲区
                        for (uint32_t i = 0; i < lte_length; i++) {
                            if (RingBuffer_Write(&lte_dev->tx_ring_buffer, &lte_command[i]) != RB_OK) {
                                forward_result = UART_ERR_BUFFER_FULL;
                                break;
                            }
                        }
                        
                        if (forward_result == UART_OK) {
                            // 启动LTE发送
                            uint8_t tx_byte;
                            if (RingBuffer_Read(&lte_dev->tx_ring_buffer, &tx_byte) == RB_OK) {
                                USART_ConfigInt((USART_Module*)lte_dev->uart_instance, USART_INT_TXDE, ENABLE);
                                USART_SendData((USART_Module*)lte_dev->uart_instance, tx_byte);
                                
                                // 等待LTE发送完成
                                if (xSemaphoreTake(lte_dev->tx_complete_semaphore, pdMS_TO_TICKS(timeout_ms)) != pdTRUE) {
                                    forward_result = UART_ERR_TIMEOUT;
                                }
                            }
                        }
                        
                        // 释放LTE互斥锁
                        xSemaphoreGive(lte_dev->tx_mutex);
                    }
                }
                
                if (forward_result == UART_OK) {
                    // 转发成功，LOG串口本身不需要发送
                    return UART_OK;
                } else {
                    // 转发失败，继续正常的LOG串口发送流程
                    // 这样调试信息仍然可以输出
                }
            }
        }
    }
    
    // 增加信号量有效性检查
    if (!uart_dev->tx_mutex || !uart_dev->tx_complete_semaphore) {
        return UART_ERR_INIT;
    }
    
    // 检查 FreeRTOS 调度器状态
    BaseType_t scheduler_state = xTaskGetSchedulerState();
    if (scheduler_state == taskSCHEDULER_NOT_STARTED) {
        // 调度器未启动，使用轮询方式发送
        return UART_RingBuffer_SendPolling(uart_id, data, length, timeout_ms);
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

    // 将数据写入发送缓冲区
    for (uint32_t i = 0; i < length; i++) {
        if (RingBuffer_Write(&uart_dev->tx_ring_buffer, &data[i]) != RB_OK) {
            result = UART_ERR_BUFFER_FULL;
            break;
        }
    }
    
    if (result == UART_OK) {
        // RS485需要切换到发送模式
        if (uart_id == UART_ID_RS485) {
            RS485_DIR_TX();
        }

        // 启动发送，发送缓冲区中的第一个字节
        uint8_t tx_byte;
        if (RingBuffer_Read(&uart_dev->tx_ring_buffer, &tx_byte) == RB_OK) {
            // 安全检查：确保信号量有效
            if (uart_dev->tx_complete_semaphore == NULL) {
                UART_LOG_INFO("TX_COMPLETE_SEMAPHORE_NULL");
                result = UART_ERR_INIT;
            } else {
                // 使能发送中断
                USART_ConfigInt((USART_Module*)uart_dev->uart_instance, USART_INT_TXDE, ENABLE);
                // 发送第一个字节
                USART_SendData((USART_Module*)uart_dev->uart_instance, tx_byte);
                // 等待发送完成 - 检查是否在中断中调用
                if (xPortIsInsideInterrupt() == pdTRUE) {
                    // 在中断中不能使用阻塞API
                    result = UART_ERR_BUSY;
                } else {
                    if (xSemaphoreTake(uart_dev->tx_complete_semaphore, pdMS_TO_TICKS(timeout_ms)) != pdTRUE) {
                        UART_LOG_INFO("UART %d send timeout", uart_id);
                        result = UART_ERR_TIMEOUT;
                    }
                }
            }
        }
        
        // RS485发送完成后切换回接收模式
        if (uart_id == UART_ID_RS485) {
            // 等待发送完全完成
            while (USART_GetFlagStatus((USART_Module*)uart_dev->uart_instance, USART_FLAG_TXC) == RESET);
            RS485_DIR_RX();
        }
    }
    
    // 释放互斥锁
    xSemaphoreGive(uart_dev->tx_mutex);
    
    return result;
}

/**
 * @brief 轮询方式发送数据（用于调度器未启动时）
 * @param uart_id 串口ID
 * @param data 要发送的数据
 * @param length 数据长度
 * @param timeout_ms 超时时间（毫秒）
 * @return UART_Status_t 状态码
 */
static UART_Status_t UART_RingBuffer_SendPolling(UART_ID_t uart_id, const uint8_t* data, uint32_t length, uint32_t timeout_ms) {
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
 * @param uart_id 串口ID
 * @param data 接收数据缓冲区
 * @param length 期望接收的字节数
 * @param timeout_ms 超时时间（毫秒）
 * @return UART_Status_t 状态码
 */
UART_Status_t UART_RingBuffer_Receive(UART_ID_t uart_id, uint8_t* data, uint32_t length, uint32_t timeout_ms) {
    UART_RingBuffer_t* uart_dev = get_uart_device(uart_id);
    if (!uart_dev || !uart_dev->is_initialized || !data || length == 0) {
        return UART_ERR_PARAM;
    }
    
    uint32_t received = 0;
    TickType_t start_time = xTaskGetTickCount();
    TickType_t timeout_ticks = pdMS_TO_TICKS(timeout_ms);
    
    while (received < length) {
        // 尝试从环形缓冲区读取数据
        if (RingBuffer_Read(&uart_dev->rx_ring_buffer, &data[received]) == RB_OK) {
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
 * @param uart_id 串口ID
 * @param data 存储字节的指针
 * @return UART_Status_t 状态码
 */
UART_Status_t UART_RingBuffer_ReadByte(UART_ID_t uart_id, uint8_t* data) {
    UART_RingBuffer_t* uart_dev = get_uart_device(uart_id);
    if (!uart_dev || !uart_dev->is_initialized || !data) {
        return UART_ERR_PARAM;
    }
    
    if (RingBuffer_Read(&uart_dev->rx_ring_buffer, data) == RB_OK) {
        return UART_OK;
    }
    
    return UART_ERR_NO_DATA;
}

/**
 * @brief 获取接收缓冲区中可读取的字节数
 * @param uart_id 串口ID
 * @return 可读取的字节数，错误返回0
 */
uint32_t UART_RingBuffer_GetAvailableBytes(UART_ID_t uart_id) {
    UART_RingBuffer_t* uart_dev = get_uart_device(uart_id);
    if (!uart_dev || !uart_dev->is_initialized) {
        return 0;
    }
    
    return uart_dev->rx_ring_buffer.count;
}

/**
 * @brief 清空接收缓冲区
 * @param uart_id 串口ID
 * @return UART_Status_t 状态码
 */
UART_Status_t UART_RingBuffer_FlushRx(UART_ID_t uart_id) {
    UART_RingBuffer_t* uart_dev = get_uart_device(uart_id);
    if (!uart_dev || !uart_dev->is_initialized) {
        return UART_ERR_PARAM;
    }
    
    RingBuffer_Clear(&uart_dev->rx_ring_buffer);
    return UART_OK;
}

/**
 * @brief 清空发送缓冲区
 * @param uart_id 串口ID
 * @return UART_Status_t 状态码
 */
UART_Status_t UART_RingBuffer_FlushTx(UART_ID_t uart_id) {
    UART_RingBuffer_t* uart_dev = get_uart_device(uart_id);
    if (!uart_dev || !uart_dev->is_initialized) {
        return UART_ERR_PARAM;
    }
    
    RingBuffer_Clear(&uart_dev->tx_ring_buffer);
    return UART_OK;
}

/**
 * @brief 串口中断处理函数（在具体的IRQHandler中调用）
 * @param uart_id 串口ID
 */
void UART_RingBuffer_IRQHandler(UART_ID_t uart_id) {
    UART_RingBuffer_t* uart_dev = get_uart_device(uart_id);
    if (!uart_dev || !uart_dev->is_initialized) {
        return;
    }
    
    USART_Module* USARTx = (USART_Module*)uart_dev->uart_instance;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    // 处理接收中断
    if (USART_GetIntStatus(USARTx, USART_INT_RXDNE) == SET) {
        uint8_t received_byte = USART_ReceiveData(USARTx);
        
        // LTE串口数据转发到LOG串口（用于查看4G模块响应）
        if (uart_id == UART_ID_LTE && s_lte_to_log_forward_enabled) {
            UART_RingBuffer_t* log_dev = get_uart_device(UART_ID_LOG);
            if (log_dev && log_dev->is_initialized) {
                // 将LTE接收的数据同时写入LOG串口的发送缓冲区
                if (RingBuffer_Write(&log_dev->tx_ring_buffer, &received_byte) == RB_OK) {
                    // 如果LOG串口发送缓冲区有数据且当前没在发送，启动发送
                    if (log_dev->tx_ring_buffer.count == 1) { // 刚写入第一个字节
                        uint8_t log_tx_byte;
                        if (RingBuffer_Read(&log_dev->tx_ring_buffer, &log_tx_byte) == RB_OK) {
                            USART_Module* LOG_USARTx = (USART_Module*)log_dev->uart_instance;
                            USART_SendData(LOG_USARTx, log_tx_byte);
                            USART_ConfigInt(LOG_USARTx, USART_INT_TXDE, ENABLE);
                        }
                    }
                }
            }
        }
        
        // 将接收到的字节写入环形缓冲区
        if (RingBuffer_WriteFromISR(&uart_dev->rx_ring_buffer, &received_byte, &xHigherPriorityTaskWoken) == RB_OK) {
            // 通知有新数据到达
            xSemaphoreGiveFromISR(uart_dev->rx_semaphore, &xHigherPriorityTaskWoken);
        }
        
        USART_ClrIntPendingBit(USARTx, USART_INT_RXDNE);
    }
    
    // 处理发送中断
    if (USART_GetIntStatus(USARTx, USART_INT_TXDE) == SET) {
        uint8_t tx_byte;
        
        // 从发送缓冲区读取下一个字节
        if (RingBuffer_Read(&uart_dev->tx_ring_buffer, &tx_byte) == RB_OK) {
            // 发送下一个字节
            USART_SendData(USARTx, tx_byte);
        } else {
            // 发送缓冲区为空，禁用发送中断
            USART_ConfigInt(USARTx, USART_INT_TXDE, DISABLE);
            
            // 通知发送完成
            xSemaphoreGiveFromISR(uart_dev->tx_complete_semaphore, &xHigherPriorityTaskWoken);
        }
        
        USART_ClrIntPendingBit(USARTx, USART_INT_TXDE);
    }
    
    // 处理IDLE中断（一帧数据接收完成）
    if (USART_GetIntStatus(USARTx, USART_INT_IDLEF) == SET) {
        // 清除IDLE标志
        uint32_t temp = USARTx->STS;
        temp = USARTx->DAT;
        (void)temp; // 避免编译器警告
        
        // 可以在这里处理帧接收完成的逻辑
        // 例如通知应用层一帧数据接收完成
    }
    
    // 如果有更高优先级的任务被唤醒，进行任务切换
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/**
 * @brief 发送字符串
 * @param uart_id 串口ID
 * @param str 要发送的字符串
 * @param timeout_ms 超时时间（毫秒）
 * @return UART_Status_t 状态码
 */
UART_Status_t UART_RingBuffer_SendString(UART_ID_t uart_id, const char* str, uint32_t timeout_ms) {
    if (!str) {
        return UART_ERR_PARAM;
    }
    
    uint32_t length = strlen(str);
    
    return UART_RingBuffer_Send(uart_id, (const uint8_t*)str, length, timeout_ms);
}

/**
 * @brief 格式化发送（类似printf）
 * @param uart_id 串口ID
 * @param timeout_ms 超时时间（毫秒）
 * @param format 格式字符串
 * @param ... 可变参数
 * @return UART_Status_t 状态码
 */
UART_Status_t UART_RingBuffer_Printf(UART_ID_t uart_id, uint32_t timeout_ms, const char* format, ...) {
    if (!format) {
        return UART_ERR_PARAM;
    }
    
    char buffer[UART_PRINTF_BUFFER_SIZE];
    va_list args;
    
    va_start(args, format);
    int length = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    if (length < 0 || length >= sizeof(buffer)) {
        return UART_ERR_PARAM;
    }
    
    return UART_RingBuffer_Send(uart_id, (const uint8_t*)buffer, length, timeout_ms);
}

/**
 * @brief 启用或禁用LOG串口到LTE串口的转发
 * @param enable true-启用转发，false-禁用转发
 */
void UART_RingBuffer_SetLogToLteForward(bool enable) {
    s_log_to_lte_forward_enabled = enable;
}

/**
 * @brief 启用或禁用LTE串口到LOG串口的转发
 * @param enable true-启用转发，false-禁用转发
 */
void UART_RingBuffer_SetLteToLogForward(bool enable) {
    s_lte_to_log_forward_enabled = enable;
}

/**
 * @brief 检查LOG串口到LTE串口转发是否启用
 * @return bool true-已启用，false-已禁用
 */
bool UART_RingBuffer_IsLogToLteForwardEnabled(void) {
    return s_log_to_lte_forward_enabled;
}

/**
 * @brief 检查LTE串口到LOG串口转发是否启用
 * @return bool true-已启用，false-已禁用
 */
bool UART_RingBuffer_IsLteToLogForwardEnabled(void) {
    return s_lte_to_log_forward_enabled;
}
