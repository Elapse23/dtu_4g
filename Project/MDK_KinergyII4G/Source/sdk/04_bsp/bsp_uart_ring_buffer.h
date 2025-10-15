/**
 * =====================================================================================
 * @file        bsp_uart_ring_buffer.h
 * @brief       基于环形缓冲区的串口收发驱动头文件
 * @author      23Elapse
 * @version     1.0
 * @date        2025-09-02
 * @note        使用环形缓冲区实现串口数据的缓冲收发，支持多串口
 * =====================================================================================
 */

#ifndef __BSP_UART_RING_BUFFER_H
#define __BSP_UART_RING_BUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include "n32g45x.h"  // 包含 IRQn_Type 定义
#include "ring_buffer.h"
#include "bsp_serial_driver.h"

// 前向声明，避免循环包含
// #include "bsp_log_manager.h" // 如需要日志功能，在.c文件中包含



// 缓冲区大小定义
#define UART_RX_BUFFER_SIZE         1024    // 接收缓冲区大小
#define UART_TX_BUFFER_SIZE         512     // 发送缓冲区大小
#define UART_PRINTF_BUFFER_SIZE     256     // printf缓冲区大小

// 串口ID枚举
typedef enum {
    UART_ID_RS485 = 0,  // RS485串口
    UART_ID_LTE,        // LTE串口
    UART_ID_LOG,        // 日志串口
    UART_COUNT          // 串口总数
} UART_ID_t;

// 串口状态枚举
typedef enum {
    UART_OK = 0,                // 操作成功
    UART_ERR_PARAM,            // 参数错误
    UART_ERR_INIT,             // 初始化失败
    UART_ERR_TIMEOUT,          // 超时
    UART_ERR_BUFFER_FULL,      // 缓冲区满
    UART_ERR_NO_DATA,          // 无数据
    UART_ERR_BUSY              // 设备忙
} UART_Status_t;

// 串口设备结构体
typedef struct {
    UART_ID_t uart_id;                      // 串口ID
    void* uart_instance;                    // UART硬件实例
    uint32_t baudrate;                      // 波特率
    IRQn_Type irqn;                         // 中断号
    bool is_initialized;                    // 初始化状态
    
    // 环形缓冲区
    RingBuffer_t rx_ring_buffer;            // 接收环形缓冲区
    RingBuffer_t tx_ring_buffer;            // 发送环形缓冲区
    uint8_t* rx_buffer_storage;             // 接收缓冲区存储空间
    uint8_t* tx_buffer_storage;             // 发送缓冲区存储空间
    
    // RTOS同步对象
    void* tx_mutex;                         // 发送互斥锁
    void* rx_semaphore;                     // 接收信号量
    void* tx_complete_semaphore;            // 发送完成信号量（保留，用于兼容性）
    
    // 注意：发送调试相关的字段已移除，因为现在使用直接轮询发送
} UART_RingBuffer_t;

// === 公共API函数 ===

/**
 * @brief 初始化串口环形缓冲区驱动
 */
UART_Status_t uart_init(UART_ID_t uart_id, uint32_t baudrate);

/**
 * @brief 反初始化串口环形缓冲区驱动
 */
UART_Status_t uart_deinit(UART_ID_t uart_id);

/**
 * @brief 发送数据（阻塞方式）
 */
UART_Status_t uart_send(UART_ID_t uart_id, const uint8_t* data, uint32_t length, uint32_t timeout_ms);

/**
 * @brief 接收数据（阻塞方式）
 */
UART_Status_t uart_receive(UART_ID_t uart_id, uint8_t* data, uint32_t length, uint32_t timeout_ms);

/**
 * @brief 非阻塞读取一个字节
 */
UART_Status_t uart_read_byte(UART_ID_t uart_id, uint8_t* data);

/**
 * @brief 获取接收缓冲区中可读取的字节数
 */
uint32_t uart_get_available_bytes(UART_ID_t uart_id);

/**
 * @brief 清空接收缓冲区
 */
UART_Status_t uart_flush_rx(UART_ID_t uart_id);

/**
 * @brief 清空发送缓冲区
 */
UART_Status_t uart_flush_tx(UART_ID_t uart_id);



/**
 * @brief 串口中断处理函数
 */
void uart_irq_handler(UART_ID_t uart_id);

/**
 * @brief 获取串口发送状态
 * @param uart_id 串口ID
 * @return true=发送忙碌中, false=发送空闲
 */
bool uart_is_tx_busy(UART_ID_t uart_id);

// === 便捷宏定义 ===

// 常用超时时间
#define UART_TIMEOUT_NO_WAIT        0
#define UART_TIMEOUT_DEFAULT        1000
#define UART_TIMEOUT_INFINITE       0xFFFFFFFF

// 串口发送宏（默认超时时间）
#define uart_send_rs485(data, len)      uart_send(UART_ID_RS485, data, len, UART_TIMEOUT_DEFAULT)
#define uart_send_lte(data, len)        uart_send(UART_ID_LTE, data, len, UART_TIMEOUT_DEFAULT)
#define uart_send_log(data, len)        uart_send(UART_ID_LOG, data, len, UART_TIMEOUT_DEFAULT)

// 串口接收宏（默认超时时间）
#define uart_receive_rs485(data, len)   uart_receive(UART_ID_RS485, data, len, UART_TIMEOUT_DEFAULT)
#define uart_receive_lte(data, len)     uart_receive(UART_ID_LTE, data, len, UART_TIMEOUT_DEFAULT)
#define uart_receive_log(data, len)     uart_receive(UART_ID_LOG, data, len, UART_TIMEOUT_DEFAULT)

// NOTE: 2025-09 移除原LOG<->LTE串口转发功能，若需调试请直接使用对应串口。

#ifdef __cplusplus
}
#endif

#endif /* __BSP_UART_RING_BUFFER_H */
