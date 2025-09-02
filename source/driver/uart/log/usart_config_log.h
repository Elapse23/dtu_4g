#ifndef _UART_CONFIG_LOG_H_
#define _UART_CONFIG_LOG_H_

#include <stdint.h>

void usart_log_config();
void usart_log_transmit_trig(uint16_t w_data_in_len,uint8_t *pb_data_in);
void usart_log_transmit_isr();
void usart_log_receive_isr();
void usart_log_int_flag_clean_isr();

#endif
