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


RB_Status RingBuffer_Init(RingBuffer_t* rb, uint8_t* buffer, uint32_t capacity, uint32_t element_size) {
    if (!rb || !buffer || capacity == 0 || element_size == 0) return RB_ERROR_INIT;

    rb->buffer = buffer;  // 使用外部传入的静态数组
    rb->element_size = element_size;
    rb->capacity = capacity;
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;

    // 不在环形缓冲区层面创建信号量，由上层驱动负责信号量管理
    rb->sem = NULL;
    return RB_OK;
}

RB_Status RingBuffer_Deinit(RingBuffer_t* rb) {
    if (!rb) return RB_ERROR_INIT;

    // 不再释放buffer，因为它是外部静态数组
    rb->buffer = NULL;
    
    // 环形缓冲区本身不创建信号量，无需删除
    rb->sem = NULL;
    return RB_OK;
}

RB_Status RingBuffer_Write(RingBuffer_t* rb, const void* data) {
    if (!rb || !rb->buffer || !data) return RB_ERROR_INIT;
    if (rb->count >= rb->capacity) return RB_ERROR_BUFFER_FULL;

    // 此处可以添加临界区保护，防止多任务同时写入
    memcpy(rb->buffer + rb->tail * rb->element_size, data, rb->element_size);
    rb->tail = (rb->tail + 1) % rb->capacity;
    rb->count++;
    // 结束临界区

    // 信号量由上层驱动管理，环形缓冲区不负责信号量操作
    return RB_OK;
}

RB_Status RingBuffer_WriteFromISR(RingBuffer_t* rb, const void* data, void* xHigherPriorityTaskWoken) {
    if (!rb || !rb->buffer || !data) return RB_ERROR_INIT;
    if (rb->count >= rb->capacity) return RB_ERROR_BUFFER_FULL;

    memcpy(rb->buffer + rb->tail * rb->element_size, data, rb->element_size);
    rb->tail = (rb->tail + 1) % rb->capacity;
    rb->count++;

    // 信号量由上层驱动管理，环形缓冲区不负责信号量操作
    return RB_OK;
}

RB_Status RingBuffer_Read(RingBuffer_t* rb, void* data) {
    if (!rb || !rb->buffer || !data) return RB_ERROR_INIT;
    
    // 注意：信号量的获取(Take)应由消费者任务在使用Read之前完成，而不是在Read函数内部
    if (rb->count == 0) return RB_ERROR_BUFFER_EMPTY;

    // 此处可以添加临界区保护
    memcpy(data, rb->buffer + rb->head * rb->element_size, rb->element_size);
    rb->head = (rb->head + 1) % rb->capacity;
    rb->count--;
    // 结束临界区
    return RB_OK;
}

/**
 * @brief 从环形缓冲区中读取不消费的数据（仅预读）
 * @param rb 环形缓冲区对象
 * @param dest 存储预读数据的缓冲区
 * @param max_len 最大预读长度
 * @return 实际读取的字节数
 */
size_t RingBuffer_Peek(RingBuffer_t* rb, uint8_t* dest, size_t max_len)
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
void RingBuffer_Drop(RingBuffer_t* rb, size_t len)
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
size_t RingBuffer_ReadMulti(RingBuffer_t* rb, uint8_t* dest, size_t len)
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

void RingBuffer_Clear(RingBuffer_t* rb) {
    if (!rb || !rb->buffer) return;

    // 此处可以添加临界区保护
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
    // 结束临界区

    // 清空信号量计数值 (需要RTOS抽象层支持，或者重新创建信号量)
    // 简单起见，消费者任务在调用Clear后需要知道缓冲区已空。
    // 一个简单的方法是循环Take信号量直到超时。
}

/**
 * @brief 判断缓冲区是否已满
 * @param rb 缓冲区指针
 * @return true/false
 */
bool RingBuffer_IsFull(const RingBuffer_t *rb)
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
bool RingBuffer_IsEmpty(const RingBuffer_t *rb)
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
bool RingBuffer_IsAvailable(const RingBuffer_t *rb)
{
    if (!rb)
        return false;
    return !RingBuffer_IsEmpty(rb);
}
