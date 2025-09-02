#ifndef _USART_CONFIG_RS485_H_
#define _USART_CONFIG_RS485_H_

#include <stdint.h>
#include "freertos_user_config.h"
#include "queue.h"

#define rs485_mps_size              600

#define cdw_uart_buadrate_9600       9600
#define cdw_uart_buadrate_115200     115200

#if( RS485_USE_QUEUE)
extern QueueHandle_t xrs485_data_rx_queue;
#else
extern uint8_t ba_rs485_rx_buff[rs485_mps_size];
extern uint16_t w_rs485_rx_data_len;
#endif

extern uint32_t dw_rs485_receive_timeout;
extern uint32_t dw_rs485_trans_task_delay;


void rs485_config();
void disable_rs485_tx_it();
void enable_rs485_tx_it(uint16_t w_data_len,
                        uint8_t *b_data_buf);

uint8_t rs485_send();
uint16_t rs485_tx_index_get();
void rs485_receive_isr();
void rs485_buadrate_set(uint32_t dw_buad_rate);

#endif
