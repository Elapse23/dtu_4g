#ifndef _NET_SEGMENTED_INQUIRED_H_
#define _NET_SEGMENTED_INQUIRED_H_

#include <stdint.h>
#include "net_system_info.h"

uint8_t net_device_data_inquire_segmented(uint16_t w_net_data_start_address,
                                          uint16_t w_net_data_end_address,
                                          uint16_t *pw_data_len_out,
                                          uint8_t *pb_data_out,
                                          uint16_t w_data_out_buffer_size,
                                          const net_data_list *pst_net_data_list,
                                          net_device_info *pst_net_device_info);

uint8_t net_device_inquire_data_build(uint16_t w_net_data_start_address,
									  uint16_t w_net_data_end_address,
									  uint16_t *pw_data_len_out,
									  uint8_t *pb_data_out,
									  uint16_t w_data_out_buffer_size,
									  uint16_t w_data_list_type,
									  net_device_info * const pst_net_device_info);

uint8_t net_device_data_segmented_inquire_general(uint16_t *pw_data_len_out,
                                                  uint8_t *pb_data_out,
                                                  uint16_t w_data_out_buffer_size,
                                                  net_system_info_list *pst_net_system_info_list);


#endif
