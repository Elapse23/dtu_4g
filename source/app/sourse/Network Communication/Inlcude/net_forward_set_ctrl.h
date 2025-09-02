#ifndef _NET_FORWARD_SET_CTRL_H_
#define _NET_FORWARD_SET_CTRL_H_

#include <stdint.h>
#include "net_system_info.h"

uint8_t net_forward_set_ctrl(uint32_t dw_device_id,
                             uint8_t b_cmd,
                             uint16_t w_data_addr,
                             uint16_t w_data_in_len,
                             uint8_t *pb_data_in,
                             net_system_info_list *pst_net_system_info_list);


#endif
