#ifndef _NETWORK_BLE_COMMUNICATION_QUEUE_H_
#define _NETWORK_BLE_COMMUNICATION_QUEUE_H_

#include "application_general_macro.h"
#include "msg_queue_operate.h"
#include <stdint.h>

#define  network_ble_communication_info  genaral_communicate_data_info



uint8_t network_ble_communication_queue_send(pv_queue_operate_msg pv_queue_data);
uint8_t network_ble_communication_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue);
uint8_t network_ble_communication_queue_shift();
uint8_t network_ble_communication_queue_clear();

uint8_t network_ble_rx_queue_send(pv_queue_operate_msg pv_queue_data);
uint8_t network_ble_rx_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue);
uint8_t network_ble_rx_queue_shift();
uint8_t network_ble_rx_queue_clear();

uint8_t network_ble_tx_queue_send(pv_queue_operate_msg pv_queue_data);
uint8_t network_ble_tx_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue);
uint8_t network_ble_tx_queue_shift();
uint8_t network_ble_tx_queue_clear();
void network_ble_communication_queue_init();

#endif

