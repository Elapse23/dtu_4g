#ifndef _WIRED_NETWORK_CONFIG_GENERAL_H_
#define _WIRED_NETWORK_CONFIG_GENERAL_H_

#include <stdint.h>
#include "net_device_info.h"

#define cb_wired_network_can_cable						0x00
#define cb_wired_network_rs485_cable					0x01

#define cb_wired_network_inquire_cmd					    0x00
#define cb_wired_network_set_cmd						    0x01
#define cb_wired_network_ctrl_cmd						    0x02
#define cb_wired_network_type_update_cmd                    0x03
#define cb_wired_network_inquire_device_tree_cmd 		    0x04
#define cb_wired_network_inquire_device_alarm_cmd 		    0x05
#define cb_wired_network_product_test_cmd                   0x06
#define cb_wired_network_event_logs_cmd                     0x07
#define cb_wired_network_product_test_inquire_cmd           0x08
#define cb_wired_network_subset_update_pac_cmd              0x09
#define cb_wired_network_subset_update_pacx_mcu_cmd         0x0A
#define cb_wired_network_subset_update_pacx_dsp_cmd         0x0B
#define cb_wired_network_subset_update_transparent_cmd      0x0C
#define cb_wired_network_inv_auxiliary_installation_cmd     0x0D
#define cb_wired_network_mppt_auxiliary_installation_cmd    0x0E


#define cb_wired_network_receive_state_wait					0
#define cb_wired_network_receive_state_fail					1
#define cb_wired_network_receive_state_success				2

#define can_enable      FALSE



uint8_t wired_network_data_transmission_channel_get(pfunc_net_device_send_data *p_send_data_channel,
													pfunc_net_device_receive_data *p_receive_data_channel);

#endif
