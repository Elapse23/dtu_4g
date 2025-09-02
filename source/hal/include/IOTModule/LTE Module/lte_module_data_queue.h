#ifndef _LTE_MODULE_DATA_QUEUE_H_
#define _LTE_MODULE_DATA_QUEUE_H_

#include "msg_queue_operate.h"

uint8_t lte_module_rx_data_queue_send(pv_queue_operate_msg pv_queue_data);
uint8_t lte_module_rx_data_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue);
uint8_t lte_module_tx_data_queue_send(pv_queue_operate_msg pv_queue_data);
uint8_t lte_module_tx_data_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue);
uint8_t lte_module_rx_data_queue_shift();
uint8_t lte_module_tx_data_queue_shift();
uint8_t let_module_rx_data_queue_empty_check();
void lte_module_queue_init();

#endif
