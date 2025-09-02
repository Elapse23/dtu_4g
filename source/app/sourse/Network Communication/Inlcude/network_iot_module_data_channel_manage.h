#ifndef _NETWORK_IOT_MODULE_DATA_CHANNEL_MANAGE_H_
#define _NETWORK_IOT_MODULE_DATA_CHANNEL_MANAGE_H_

#include <stdint.h>
#include "iot_module_config_general.h"
#include "lte_module.h"
#include "lte_4g_module.h"

#define network_send_data_channel 		                    px_iot_module_send_data
#define network_receive_data_channel 		                px_iot_module_receive_data

extern uint16_t w_net_iot_module_mps;

extern network_send_data_channel iot_module_send_data;
extern network_receive_data_channel iot_module_receive_data;


void network_iot_module_connection_info_initial();

uint8_t network_data_channel_initial(network_send_data_channel px_send,\
                                     network_receive_data_channel px_receive);


void network_iot_module_communicate_channel_config_excute();


#define cb_iot_module_type_lte                              0
#define cb_iot_module_type_ble                              1

#define cb_network_protocal_type_tcp_socket                 0

#define PARAM_NAME(a) #a

typedef enum
{
    connection_type_tbb_app,
    connection_type_tbb_server,
#ifdef lte_protocal_mqtt
    connection_type_positec_device_server,
    connection_type_positec_ota_server,
#endif
    connection_type_number
}network_iot_connection_type;

typedef enum
{
    connection_operation_open,
    connection_operation_open_ap,
//    connection_operation_close,
//    connection_operation_send_0,
//    connection_operation_send_1,
//    connection_operation_receive_0,
//    connection_operation_receive_1,
//    connection_operation_receive_2,
    connection_operation_number
}network_iot_module_connection_operation;

typedef struct
{
    union
    {
        lte_module_driver_config_info_general st_lte_module;
        //ble_module_driver_config_info st_ble_module;
        lte_4g_module_driver_config_info_general st_lte_4g_module;
    }union_config_info;
}network_iot_config_info;

typedef struct
{
    union
    {
        lte_module_driver_reponse_info_general st_lte_module;
        //ble_module_driver_reponse_info_general st_ble_module;
    }union_reponse_info;
}network_iot_reponse_info;

typedef struct
{
    uint8_t b_connection_type;
    struct
    {
        uint8_t b_state;
        uint16_t w_timeout;
        const uint16_t w_timeout_threshold_ms;
    }connection_state;
    union
    {
        lte_module_driver_config_info_general sta_lte_module_config[connection_operation_number];
		lte_4g_module_driver_config_info_general sta_lte_4g_module_config[connection_operation_number];
    }union_iot_module_config_info;
}network_iot_module_connection_info;

extern network_iot_module_connection_info sta_network_iot_module_connection_info[connection_type_number];

uint8_t network_iot_module_communicate_channel_operate(uint8_t b_iot_module_type,
                                                       uint8_t b_iot_module_operation,
                                                       uint16_t w_data_len,
                                                       void *pv_data);
uint8_t network_iot_module_communicate_channel_operate_response(uint8_t b_iot_module_type,
                                                                uint8_t b_iot_module_operation,
                                                                void **pv_data);
uint8_t network_iot_module_mqtt_connection_subscribe_topic(uint8_t b_connection_type,
                                                           uint8_t *pb_topic_str,
                                                           uint8_t b_qos,
                                                           uint8_t b_retain);

uint8_t network_iot_queue_recognition_conversion();




//uint8_t network_communicate_channel_open(uint8_t b_iot_module_type,
//                                         uint8_t b_network_protocal_type,
//                                         network_send_data_channel pfunc_network_send_data_channel,
//                                         network_send_data_channel pfunc_network_send_data_channel);


#define default_lte_error_cnt_max 555
#define much_lte_error_cnt_max 7777

uint8_t lte_error_cnt(uint16_t *error_cnt, uint16_t cnt_max);
uint8_t lte_error_check(uint8_t *error_clear_flag, uint8_t *lte_step);

extern uint16_t net_ble_establish_ble_init_cnt;
extern uint8_t net_ble_establish_ble_init_flag;
extern uint16_t net_ble_open_broadcast_cnt;
extern uint8_t net_ble_open_broadcast_flag;
extern uint16_t net_ble_establish_ble_deinit_cnt;
extern uint8_t net_ble_establish_ble_deinit_flag;
extern uint16_t net_ble_send_message_cnt;
extern uint8_t net_ble_send_message_flag;
extern uint16_t net_tbb_server_communicate_initial_cnt;
extern uint8_t net_tbb_server_communicate_initial_flag;
extern uint16_t net_tbb_server_establish_module_reset_cnt;
extern uint8_t net_tbb_server_establish_module_reset_flag;
extern uint16_t net_tbb_server_establish_sever_connect_cnt;
extern uint8_t net_tbb_server_establish_sever_connect_flag;
extern uint16_t net_tbb_server_establish_ap_connect_cnt;
extern uint8_t net_tbb_server_establish_ap_connect_flag;
extern uint16_t net_tbb_server_establish_sever_close_cnt;
extern uint8_t net_tbb_server_establish_sever_close_flag;
extern uint16_t net_get_ap_info_cnt;
extern uint8_t net_get_ap_info_flag;
extern uint16_t net_tbb_server_establish_ap_disconnect_cnt;
extern uint8_t net_tbb_server_establish_ap_disconnect_flag;
extern uint16_t net_tbb_server_establish_sever_reconnect_cnt;
extern uint8_t net_tbb_server_establish_sever_reconnect_flag;
extern uint16_t lte_module_user_data_send_cnt;
extern uint8_t lte_module_user_data_send_flag;
extern uint16_t lte_module_user_data_receive_cnt;
extern uint8_t lte_module_user_data_receive_flag;

#endif
