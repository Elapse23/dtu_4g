#ifndef _NETWORK_DEVICE_CONFIG_H_
#define _NETWORK_DEVICE_CONFIG_H_

#include <stdint.h>
#include "net_system_info_list.h"

//subset config one device
uint8_t net_device_config(uint32_t dw_device_id,
                          uint8_t b_cmd,
                          const net_data_info *pst_net_data_info,
                          uint16_t w_data_in_len,
                          uint8_t *pb_data_in,
                          net_system_info_list *pst_net_system_info_list);

//subset config all device
// ->network_device_config_sycn.c

#endif
