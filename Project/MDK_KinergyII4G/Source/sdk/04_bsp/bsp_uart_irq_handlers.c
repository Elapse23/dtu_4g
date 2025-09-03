/**
 * =====================================================================================
 * @file        bsp_uart_irq_handlers.c
 * @brief       串口中断处理函数实现
 * @author      23Elapse
 * @version     1.0
 * @date        2025-09-02
 * @note        此文件包含各个串口的中断处理函数，连接硬件中断和环形缓冲区驱动
 * =====================================================================================
 */

#include "bsp_uart_ring_buffer.h"
#include "n32g45x.h"

/**
 * @brief RS485串口中断处理函数
 * @note 在启动文件或中断向量表中被调用
 */
void UART5_IRQHandler(void) {
    UART_RingBuffer_IRQHandler(UART_ID_RS485);
}

/**
 * @brief LTE串口中断处理函数
 * @note 在启动文件或中断向量表中被调用
 */
void USART3_IRQHandler(void) {
    UART_RingBuffer_IRQHandler(UART_ID_LTE);
}

/**
 * @brief LOG串口中断处理函数（如果需要）
 * @note LOG口通常只用于发送，一般不需要中断
 */
void UART4_IRQHandler(void) {
    UART_RingBuffer_IRQHandler(UART_ID_LOG);
}
