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
    void* tx_complete_semaphore;            // 发送完成信号量
} UART_RingBuffer_t;

// === 公共API函数 ===

/**
 * @brief 初始化串口环形缓冲区驱动
 * @param uart_id 串口ID
 * @param baudrate 波特率
 * @return UART_Status_t 状态码
 */
UART_Status_t UART_RingBuffer_Init(UART_ID_t uart_id, uint32_t baudrate);

/**
 * @brief 反初始化串口环形缓冲区驱动
 * @param uart_id 串口ID
 * @return UART_Status_t 状态码
 */
UART_Status_t UART_RingBuffer_Deinit(UART_ID_t uart_id);

/**
 * @brief 发送数据（阻塞方式）
 * @param uart_id 串口ID
 * @param data 要发送的数据
 * @param length 数据长度
 * @param timeout_ms 超时时间（毫秒）
 * @return UART_Status_t 状态码
 */
UART_Status_t UART_RingBuffer_Send(UART_ID_t uart_id, const uint8_t* data, uint32_t length, uint32_t timeout_ms);

/**
 * @brief 接收数据（阻塞方式）
 * @param uart_id 串口ID
 * @param data 接收数据缓冲区
 * @param length 期望接收的字节数
 * @param timeout_ms 超时时间（毫秒）
 * @return UART_Status_t 状态码
 */
UART_Status_t UART_RingBuffer_Receive(UART_ID_t uart_id, uint8_t* data, uint32_t length, uint32_t timeout_ms);

/**
 * @brief 非阻塞读取一个字节
 * @param uart_id 串口ID
 * @param data 存储字节的指针
 * @return UART_Status_t 状态码
 */
UART_Status_t UART_RingBuffer_ReadByte(UART_ID_t uart_id, uint8_t* data);

/**
 * @brief 获取接收缓冲区中可读取的字节数
 * @param uart_id 串口ID
 * @return 可读取的字节数，错误返回0
 */
uint32_t UART_RingBuffer_GetAvailableBytes(UART_ID_t uart_id);

/**
 * @brief 清空接收缓冲区
 * @param uart_id 串口ID
 * @return UART_Status_t 状态码
 */
UART_Status_t UART_RingBuffer_FlushRx(UART_ID_t uart_id);

/**
 * @brief 清空发送缓冲区
 * @param uart_id 串口ID
 * @return UART_Status_t 状态码
 */
UART_Status_t UART_RingBuffer_FlushTx(UART_ID_t uart_id);

/**
 * @brief 发送字符串
 * @param uart_id 串口ID
 * @param str 要发送的字符串
 * @param timeout_ms 超时时间（毫秒）
 * @return UART_Status_t 状态码
 */
UART_Status_t UART_RingBuffer_SendString(UART_ID_t uart_id, const char* str, uint32_t timeout_ms);

/**
 * @brief 格式化发送（类似printf）
 * @param uart_id 串口ID
 * @param timeout_ms 超时时间（毫秒）
 * @param format 格式字符串
 * @param ... 可变参数
 * @return UART_Status_t 状态码
 */
UART_Status_t UART_RingBuffer_Printf(UART_ID_t uart_id, uint32_t timeout_ms, const char* format, ...);

/**
 * @brief 串口中断处理函数（在具体的IRQHandler中调用）
 * @param uart_id 串口ID
 */
void UART_RingBuffer_IRQHandler(UART_ID_t uart_id);

// === 便捷宏定义 ===

// 常用超时时间
#define UART_TIMEOUT_NO_WAIT        0
#define UART_TIMEOUT_DEFAULT        1000
#define UART_TIMEOUT_INFINITE       0xFFFFFFFF

// 串口发送宏（默认超时时间）
#define UART_Send_RS485(data, len)      UART_RingBuffer_Send(UART_ID_RS485, data, len, UART_TIMEOUT_DEFAULT)
#define UART_Send_LTE(data, len)        UART_RingBuffer_Send(UART_ID_LTE, data, len, UART_TIMEOUT_DEFAULT)
#define UART_Send_LOG(data, len)        UART_RingBuffer_Send(UART_ID_LOG, data, len, UART_TIMEOUT_DEFAULT)

// 串口接收宏（默认超时时间）
#define UART_Receive_RS485(data, len)   UART_RingBuffer_Receive(UART_ID_RS485, data, len, UART_TIMEOUT_DEFAULT)
#define UART_Receive_LTE(data, len)     UART_RingBuffer_Receive(UART_ID_LTE, data, len, UART_TIMEOUT_DEFAULT)
#define UART_Receive_LOG(data, len)     UART_RingBuffer_Receive(UART_ID_LOG, data, len, UART_TIMEOUT_DEFAULT)

// 字符串发送宏
#define UART_SendString_RS485(str)      UART_RingBuffer_SendString(UART_ID_RS485, str, UART_TIMEOUT_DEFAULT)
#define UART_SendString_LTE(str)        UART_RingBuffer_SendString(UART_ID_LTE, str, UART_TIMEOUT_DEFAULT)
#define UART_SendString_LOG(str)        UART_RingBuffer_SendString(UART_ID_LOG, str, UART_TIMEOUT_DEFAULT)

// Printf宏
#define UART_Printf_RS485(fmt, ...)     UART_RingBuffer_Printf(UART_ID_RS485, UART_TIMEOUT_DEFAULT, fmt, ##__VA_ARGS__)
#define UART_Printf_LTE(fmt, ...)       UART_RingBuffer_Printf(UART_ID_LTE, UART_TIMEOUT_DEFAULT, fmt, ##__VA_ARGS__)
#define UART_Printf_LOG(fmt, ...)       UART_RingBuffer_Printf(UART_ID_LOG, UART_TIMEOUT_DEFAULT, fmt, ##__VA_ARGS__)

// 串口转发控制函数
void UART_RingBuffer_SetLogToLteForward(bool enable);
void UART_RingBuffer_SetLteToLogForward(bool enable);
bool UART_RingBuffer_IsLogToLteForwardEnabled(void);
bool UART_RingBuffer_IsLteToLogForwardEnabled(void);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_UART_RING_BUFFER_H */
