#ifndef _NETWORK_COMMUNICATION_QUEUE_H_
#define _NETWORK_COMMUNICATION_QUEUE_H_

#include "msg_queue_operate.h"

uint8_t network_com_inquire_queue_shift();
uint8_t network_com_inquire_queue_clear();
uint8_t network_com_inquire_queue_send(pv_queue_operate_msg pv_queue_data);
uint8_t network_com_inquire_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue);

uint8_t network_com_config_queue_shift();
uint8_t network_com_config_queue_clear();
uint8_t network_com_config_queue_send(pv_queue_operate_msg pv_queue_data);
uint8_t network_com_config_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue);

uint8_t network_com_other_queue_shift();
uint8_t network_com_other_queue_clear();
uint8_t network_com_other_queue_send(pv_queue_operate_msg pv_queue_data);
uint8_t network_com_other_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue);

uint8_t network_com_tx_data_queue_shift();
uint8_t network_com_tx_data_queue_clear();
uint8_t network_com_tx_data_queue_send(pv_queue_operate_msg pv_queue_data);
uint8_t network_com_tx_data_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue);


void network_com_queue_init();

#endif
