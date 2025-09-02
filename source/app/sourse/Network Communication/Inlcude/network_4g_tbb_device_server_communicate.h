#ifndef _NETWORK_TBB_DEVICE_SERVER_COMMUNICATE_H_
#define _NETWORK_TBB_DEVICE_SERVER_COMMUNICATE_H_

#include <stdint.h>

#define cb_max_network_data_len     1000

//typedef enum
//{
//    cb_net_com_state_type_sever = 0,
//    cb_net_com_state_type_ap,
//    cb_net_com_state_type_err_code,
//}net_com_state_type;

uint8_t network_4g_com_with_tbb_device_server();

//void network_com_with_tbb_device_server_state_set(uint8_t b_state_type,
//                                                  uint8_t b_state);
uint8_t network_4g_com_with_tbb_device_server_state_get();
uint8_t net_get_sim_info();


#endif

