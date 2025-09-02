#ifndef _UART_CONFIG_LTE_H_
#define _UART_CONFIG_LTE_H_

#include <stdint.h>

typedef enum{
    receive_none = 0,
    receiving,
    receive_done,
}lte_module_receive_state;

typedef enum{
    send_none = 0,
    sending,
    send_done,
}lte_module_send_state;

#define cb_lte_mps_size         2000

extern uint8_t b_usart_lte_rx_buff[cb_lte_mps_size];
extern uint16_t w_usart_lte_rx_len;

void usart_lte_config();
void usart_lte_transmit_trig(uint16_t w_data_in_len,uint8_t *pb_data_in);
void usart_lte_transmit_isr();
void usart_lte_receive_isr();
void usart_lte_clear_data();
uint8_t uart_lte_receive_data(uint8_t *pb_data_in,
                              uint16_t *pw_data_len,
                              uint16_t w_max_data_out_len);
#endif
