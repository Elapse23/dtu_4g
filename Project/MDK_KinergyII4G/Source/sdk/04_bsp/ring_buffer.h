/**
 * =====================================================================================
 * @file        ring_buffer.h
 * @brief       通用环形缓冲区接口 (已优化)
 * @author      23Elapse & Gemini
 * @version     2.1 (Refactored)
 * @date        2025-06-08
 * @note        使用计数信号量，增加Clear接口，功能更完善。
 * =====================================================================================
 */
#ifndef __RING_BUFFER_H
#define __RING_BUFFER_H

#include <stdint.h>                  /* 标准整数类型 */
#include <stdbool.h>                 /* 布尔类型 */
#include <stddef.h>                  /* size_t 类型定义 */


// 环形缓冲区状态码
typedef enum
{
    RB_OK = 0,
    RB_ERROR_INIT,
    RB_ERROR_BUFFER_FULL,
    RB_ERROR_BUFFER_EMPTY
} RB_Status;

// 环形缓冲区结构体
typedef struct
{
    uint8_t* buffer;                // 缓冲区指针 (外部静态数组)
    uint32_t element_size;          // 元素大小 (字节)
    uint32_t capacity;              // 缓冲区容量 (元素个数)
    uint32_t head;                  // 头指针 (读位置)
    uint32_t tail;                  // 尾指针 (写位置)  
    volatile uint32_t count;        // 当前已存储的元素数量 声明为volatile，因为可能在中断和主线程中同时访问
    void* sem; // 保留信号量指针字段，但由上层驱动管理，环形缓冲区本身不创建信号量
} RingBuffer_t;

// --- 公共API ---
RB_Status ring_buffer_init(RingBuffer_t* rb, uint8_t* buffer, uint32_t capacity, uint32_t element_size);
RB_Status ring_buffer_deinit(RingBuffer_t* rb);
RB_Status ring_buffer_write(RingBuffer_t* rb, const void* data);
RB_Status ring_buffer_read(RingBuffer_t* rb, void* data);
RB_Status ring_buffer_read_from_isr(RingBuffer_t* rb, void* data);
RB_Status ring_buffer_write_from_isr(RingBuffer_t* rb, const void* data, void* xHigherPriorityTaskWoken);
void ring_buffer_clear(RingBuffer_t* rb);
size_t ring_buffer_peek(RingBuffer_t* rb, uint8_t* dest, size_t max_len);
size_t ring_buffer_read_multi(RingBuffer_t* rb, uint8_t* dest, size_t len);
void ring_buffer_drop(RingBuffer_t* rb, size_t len);
bool ring_buffer_is_full(const RingBuffer_t* rb);
bool ring_buffer_is_empty(const RingBuffer_t* rb);
bool ring_buffer_is_available(const RingBuffer_t* rb);

#endif /* RING_BUFFER_H */
