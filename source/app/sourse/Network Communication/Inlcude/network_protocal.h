#ifndef _NETWORK_PROTOCAL_H_
#define _NETWORK_PROTOCAL_H_

#include <stdint.h>
#include "application_general_macro.h"
#include "net_system_info.h"
#include "iot_module_config_general.h"
#include "network_iot_module_data_channel_manage.h"

#define cw_net_protocal_version         0x0000
extern uint16_t g_real_data_send_delay_time;
typedef struct
{
    uint16_t w_timer;
    uint8_t f_login;
}channel_infomation;

typedef uint8_t(*network_data_receive_queue)(void * pv_queue,uint8_t b_mode);
typedef uint8_t(*network_data_send_queue)(void * pv_queue);

uint8_t network_data_channel_initial(network_send_data_channel px_send,network_receive_data_channel px_receive);
void network_inquire_cmd_analysis();
void network_config_cmd_analysis();
void network_other_cmd_analysis();
void network_ble_cmd_analysis();
void network_data_send(network_data_receive_queue px_network_data_receive_queue);
void network_data_receive(void *pv_data,uint8_t b_channel_index);

void network_data_quit();
uint8_t network_communication_with_server(uint8_t f_init_process);
void network_communication_with_app();
void enabke_get_network_timestamp(uint8_t b_state);
uint8_t network_recevie_data(uint16_t w_protocal_cmd,void *pv_data);
uint8_t net_data_unpack(uint8_t *pb_data_in,\
                               uint16_t w_data_in_len,\
                               uint16_t *pw_data_in_index,\
                               uint8_t *pb_data_out,\
                               uint16_t *pw_data_out_len);
uint8_t net_data_pack(uint8_t *pb_data_in,\
                             uint16_t w_data_in_len,\
                             uint16_t *pw_data_in_index,\
                             uint8_t *pb_data_out,\
                             uint16_t *pw_data_out_len,\
                             uint16_t w_encode_data_max_len);


#endif
