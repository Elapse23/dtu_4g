/*
 * @Author: 23Elapse userszy@163.com
 * @Date: 2025-09-02 16:48:07
 * @LastEditors: 23Elapse userszy@163.com
 * @LastEditTime: 2025-09-02 17:19:05
 * @FilePath: \DTU_4G\Project\MDK_KinergyII4G\Source\sdk\04_bsp\ring_buffer.c
 * @Description: 
 * 
 * Copyright (c) 2025 by 23Elapse userszy@163.com, All Rights Reserved. 
 */
/**
 * =====================================================================================
 * @file        ring_buffer.c
 * @brief       通用环形缓冲区实现 (已优化)
 * @author      23Elapse & Gemini
 * @version     2.1 (Refactored)
 * @date        2025-06-08
 * =====================================================================================
 */

#include "ring_buffer.h"
#include <string.h>
#include "FreeRTOS.h"
#include "semphr.h"


RB_Status ring_buffer_init(RingBuffer_t* rb, uint8_t* buffer, uint32_t capacity, uint32_t element_size) {
    if (!rb || !buffer || capacity == 0 || element_size == 0) return RB_ERROR_INIT;

    rb->buffer = buffer;  // 使用外部传入的静态数组
    rb->element_size = element_size;
    rb->capacity = capacity;
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;

    rb->sem = NULL;
    return RB_OK;
}

RB_Status ring_buffer_deinit(RingBuffer_t* rb) {
    if (!rb) return RB_ERROR_INIT;

    // 不再释放buffer，因为它是外部静态数组
    rb->buffer = NULL;
    
    rb->sem = NULL;
    return RB_OK;
}

RB_Status ring_buffer_write(RingBuffer_t* rb, const void* data) {
    if (!rb || !rb->buffer || !data) return RB_ERROR_INIT;
    if (rb->count >= rb->capacity) return RB_ERROR_BUFFER_FULL;

    memcpy(rb->buffer + rb->tail * rb->element_size, data, rb->element_size);
    rb->tail = (rb->tail + 1) % rb->capacity;
    rb->count++;

    return RB_OK;
}

RB_Status ring_buffer_write_from_isr(RingBuffer_t* rb, const void* data) {
    if (!rb || !rb->buffer || !data) return RB_ERROR_INIT;
    if (rb->count >= rb->capacity) return RB_ERROR_BUFFER_FULL;

    memcpy(rb->buffer + rb->tail * rb->element_size, data, rb->element_size);
    rb->tail = (rb->tail + 1) % rb->capacity;
    rb->count++;
    
    return RB_OK;
}

RB_Status ring_buffer_read(RingBuffer_t* rb, void* data) {
    if (!rb || !rb->buffer || !data) return RB_ERROR_INIT;
    
    if (rb->count == 0) return RB_ERROR_BUFFER_EMPTY;

    memcpy(data, rb->buffer + rb->head * rb->element_size, rb->element_size);
    rb->head = (rb->head + 1) % rb->capacity;
    rb->count--;
    
    return RB_OK;
}

RB_Status ring_buffer_read_from_isr(RingBuffer_t* rb, void* data) {
    if (!rb || !rb->buffer || !data) return RB_ERROR_INIT;
    
    if (rb->count == 0) return RB_ERROR_BUFFER_EMPTY;

    memcpy(data, rb->buffer + rb->head * rb->element_size, rb->element_size);
    rb->head = (rb->head + 1) % rb->capacity;
    rb->count--;
    
    return RB_OK;
}

/**
 * @brief 从环形缓冲区中读取不消费的数据（仅预读）
 * @param rb 环形缓冲区对象
 * @param dest 存储预读数据的缓冲区
 * @param max_len 最大预读长度
 * @return 实际读取的字节数
 */
size_t ring_buffer_peek(RingBuffer_t* rb, uint8_t* dest, size_t max_len)
{
    if (!rb || !rb->buffer || !dest || max_len == 0) return 0;

    size_t readable = rb->count * rb->element_size;
    size_t to_copy = (readable < max_len) ? readable : max_len;

    for (size_t i = 0; i < to_copy; i++) {
        size_t idx = (rb->head * rb->element_size + i) % (rb->capacity * rb->element_size);
        dest[i] = rb->buffer[idx];
    }

    return to_copy;
}

/**
 * @brief 丢弃缓冲区中的前 n 个字节
 * @param rb 环形缓冲区对象
 * @param len 要丢弃的字节数
 */
void ring_buffer_drop(RingBuffer_t* rb, size_t len)
{
    if (!rb || len == 0 || !rb->buffer) return;

    size_t drop_elements = len / rb->element_size;
    if (drop_elements > rb->count) drop_elements = rb->count;

    rb->head = (rb->head + drop_elements) % rb->capacity;
    rb->count -= drop_elements;
}

/**
 * @brief 一次性读取多个字节（元素）到目标缓冲区
 * @param rb 环形缓冲区对象
 * @param dest 目标缓存区
 * @param len 字节长度
 * @return 实际读取的长度
 */
size_t ring_buffer_read_multi(RingBuffer_t* rb, uint8_t* dest, size_t len)
{
    if (!rb || !rb->buffer || !dest || len == 0) return 0;

    size_t total_read = 0;
    while (len > 0 && rb->count > 0) {
        memcpy(dest + total_read, rb->buffer + rb->head * rb->element_size, rb->element_size);
        rb->head = (rb->head + 1) % rb->capacity;
        rb->count--;
        total_read += rb->element_size;
        len -= rb->element_size;
    }
    return total_read;
}

void ring_buffer_clear(RingBuffer_t* rb) {
    if (!rb || !rb->buffer) return;

    // 此处可以添加临界区保护
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
    // 结束临界区

}

/**
 * @brief 判断缓冲区是否已满
 * @param rb 缓冲区指针
 * @return true/false
 */
bool ring_buffer_is_full(const RingBuffer_t *rb)
{
    if (!rb)
        return false;
    return (rb->count == rb->capacity);
}

/**
 * @brief 判断缓冲区是否为空
 * @param rb 缓冲区指针
 * @return true/false
 */
bool ring_buffer_is_empty(const RingBuffer_t *rb)
{
    if (!rb)
        return true;
    return (rb->count == 0);
}

/**
 * @brief 判断缓冲区是否有数据可读
 * @param rb 缓冲区指针
 * @return true/false
 */
bool ring_buffer_is_available(const RingBuffer_t *rb)
{
    if (!rb)
        return false;
    return !ring_buffer_is_empty(rb);
}
