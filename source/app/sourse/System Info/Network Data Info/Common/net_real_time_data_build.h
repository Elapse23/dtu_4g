#ifndef _NET_REAL_TIME_DATA_BUILD_H_
#define _NET_REAL_TIME_DATA_BUILD_H_

#include <stdint.h>
#include "net_system_info.h"
#include "application_general_func.h"

uint8_t net_get_device_real_time_data(uint8_t b_active_push,
                                      uint32_t dw_device_id,
                                      uint16_t w_start_data_address,
                                      uint16_t w_end_data_address,
                                      uint16_t *pw_data_out_len,
                                      uint8_t *pb_data_out,
                                      uint16_t w_data_out_buffer_size,
                                      const net_data_list *pst_net_data_list);

uint8_t net_real_time_data_build(uint8_t b_active_push,
                                 uint16_t *pw_data_out_len,
                                 uint8_t *pb_data_out,
                                 uint16_t w_data_out_buffer_size,
                                 net_system_info_list *pst_info_list);
uint8_t net_get_device_real_time_attribute_tree(uint8_t b_active_push,
                                      uint32_t dw_device_id,
                                      uint16_t w_start_data_address,
                                      uint16_t w_end_data_address,
                                      uint16_t *pw_data_out_len,
                                      uint8_t *pb_data_out,
                                      uint16_t w_data_out_buffer_size,
                                      const net_data_list *pst_net_data_list);


#endif
