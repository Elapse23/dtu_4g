#ifndef _NETWORK_LOCAL_DEVICE_SECQUENCE_OPERATE_H_
#define _NETWORK_LOCAL_DEVICE_SECQUENCE_OPERATE_H_

#include <stdint.h>

extern uint8_t ba_local_device_secquence[30];

uint8_t get_local_device_secquence(uint8_t *pb_data,uint16_t w_buffer_size);
uint8_t set_local_device_secquence(uint8_t *pb_data_in,uint8_t b_data_len);
uint8_t get_client_system_id(uint8_t *pb_data,uint16_t w_buffer_size);

#endif
