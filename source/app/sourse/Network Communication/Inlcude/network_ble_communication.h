#ifndef _NETWORK_BLE_COMMUNICATION_H_
#define _NETWORK_BLE_COMMUNICATION_H_

#include <stdint.h>

void network_ble_state_set(uint8_t b_state);
void network_ble_communication_task_init();
uint8_t net_ble_send_message(void *pv_data);

//void net_ble_communication_enable(uint8_t f_enable);
void set_module_initialize_state(uint8_t f_state);
uint8_t network_4g_ble_communication_with_app();

#endif
