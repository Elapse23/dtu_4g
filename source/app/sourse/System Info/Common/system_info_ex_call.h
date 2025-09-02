#ifndef _SYSTEM_INFO_EX_CALL_H_
#define _SYSTEM_INFO_EX_CALL_H_

#include "wired_network_config_general.h"

#define pfunc_net_subset_send_data 							pfunc_wired_network_send_data
#define pfunc_net_subset_receive_data 						pfunc_wired_network_receive_data

#define cb_net_subset_com_can_cable 						cb_wired_network_can_cable
#define cb_net_subset_com_rs485_cable						cb_wired_network_rs485_cable
#define cb_net_subset_com_none								(cb_wired_network_can_cable + cb_wired_network_rs485_cable)

#define cb_net_subset_com_inquire_cmd       				cb_wired_network_inquire_cmd
#define cb_net_subset_com_set_cmd           				cb_wired_network_set_cmd
#define cb_net_subset_com_ctrl_cmd          				cb_wired_network_ctrl_cmd
#define cb_net_subset_com_inquire_device_tree_cmd			cb_wired_network_inquire_device_tree_cmd


#endif
