#ifndef _NETWORK_TBB_DEVICE_SERVER_COMMUNICATE_H_
#define _NETWORK_TBB_DEVICE_SERVER_COMMUNICATE_H_

#include <stdint.h>

#define cb_max_network_data_len     1000 //1000
#define cb_max_ble_data_len     	250 //1000

typedef enum
{
    cb_net_com_state_type_sever = 0,
    cb_net_com_state_type_ap,
    cb_net_com_state_type_sim,
    cb_net_com_state_type_err_code,
}net_com_state_type;

uint8_t network_com_with_tbb_device_server();
uint8_t network_tbb_sever_com_delay(uint32_t dw_dalay_time_ms);

void network_com_with_tbb_device_server_state_set(uint8_t b_state_type,
                                                  uint8_t b_state);
uint8_t network_com_with_tbb_device_server_state_get();
void net_connect_ip_cnt_set(uint8_t f_cnt_add);
uint8_t net_connect_ip_cnt_get();

#endif
