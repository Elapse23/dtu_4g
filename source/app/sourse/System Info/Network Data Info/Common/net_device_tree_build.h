#ifndef _NET_DEVICE_TREE_BUILD_H_
#define _NET_DEVICE_TREE_BUILD_H_

#include "stdint.h"
#include "net_system_info.h"

extern uint16_t gw_agent_facturer;

void net_device_tree_refresh_Semaphore_creat();
void net_device_exists_state_Semaphore_creat();

uint8_t net_device_tree_build(uint16_t *pw_data_out_len,
                              uint8_t *pb_data_out,
                              uint16_t w_data_out_buffer_max_len,
                              net_system_info_list *pst_info_list);

void net_inquire_device_tree(net_system_info_list *pst_info_list);

uint8_t net_device_exists_status_refresh(uint32_t dw_device_address,uint8_t b_device_status,net_system_info_list *pst_info_list);
#endif
