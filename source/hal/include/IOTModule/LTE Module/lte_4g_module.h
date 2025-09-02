//#ifdef KINERGY_II_4G
#ifndef _LTE_4G_MODULE_H_
#define _LTE_4G_MODULE_H_

#include "application_general_macro.h"
#include "iot_module_config_common.h"
#include "lte_module_config_common.h"
#include "lte_module_com_interface_init.h"
#include "lte_module.h"
#define cb_lte_module_get_ip_port        						                0
#define cb_lte_module_connect_status     						                1
#define cb_lte_module_gps_on             						                2

#define cb_lte_module_set_ip_port        						                0
#define cb_lte_module_reconect_server    						                1
#define cb_lte_module_reinitialize       						                2
#define cb_lte_module_pdp_pdpdeact       						                3



#define cb_lte_module_driver_additional_operate_mqtt_sub_topic                  0
#define cb_lte_module_driver_additional_operate_reset_connection                1

#define cb_lte_module_driver_connection_type_tcp					            0
#define cb_lte_module_driver_connection_type_mqtts				                1
#define cb_lte_module_driver_connection_type_https					            2
#define cb_lte_module_driver_connection_type_none				                3


//typedef uint8_t (* lte_module_driver_operate_result_notify_callback)(iot_module_driver_operate_result *pv_data);

//typedef enum
//{
//    lte_module_err_none,
//    lte_module_initial_success,
//    lte_module_initial_fail,
//    lte_module_reset_success,
//    lte_module_reset_fail,
//    //wifi
//    ap_open_connection_success,
//    ap_open_connection_fail,    //all cmd err
//    ap_connect_fail,            //only connect err
//    ap_close_connection_success,
//    ap_close_connection_fail,
//    ap_connection_closed,
//    ap_connection_auto_connect_success,
//    ap_get_info_fail,
//    ap_get_info_success,
//    //tcp
//    tcp_connection_closed,
//    tcp_open_connection_success,
//    tcp_open_connection_fail,
//    tcp_send_user_data_success,
//    tcp_send_user_data_fail,
//    tcp_close_connection_success,
//    tcp_close_connection_fail,
//    tcp_receive_user_data_success,
//    tcp_receive_user_data_fail,
//    tcp_reset_connection_success,
//    tcp_reset_connection_fail,
//    //ble
//    ble_config_success,
//    ble_config_fail,
//    ble_close_success,
//    ble_close_fail,
//    ble_broadcast_open_success,
//    ble_broadcast_open_fail,
//    ble_send_user_data_success,
//    ble_send_user_data_fail,
//}lte_module_driver_operate_err_code;

//typedef struct
//{
//    uint8_t *pb_ssid;
//    uint8_t *pb_pass;
//    uint8_t *pb_bssid;
//    uint8_t *pb_pci_en;
//    uint16_t *pb_reconn_interval;
//    uint8_t *pb_listen_interval;
//    uint8_t *pb_scan_mode;
//    uint16_t *pb_jap_timeout;
//    uint8_t *pb_pmf;
//    uint8_t *pf_wps_enable;
//    uint8_t *pf_wps_first_connect;
//}lte_module_open_ap_connection;
typedef enum
{
    CMNET = 0,
    UNINET,
    CTLTE,
    lte_4g_module_sim__operate_type_number,
}lte_4g_module_sim_operator;

typedef struct
{
    uint8_t b_4g_signal_strength;
	uint8_t b_channel_bit_error_rate;
	uint8_t b_cops_type;
    uint16_t *pb_reconn_interval;
    uint8_t *pb_listen_interval;
    uint8_t *pb_scan_mode;
    uint16_t *pb_jap_timeout;
    uint8_t *pb_pmf;
    uint8_t *pf_wps_enable;
    uint8_t *pf_wps_first_connect;
}lte_module_open_sim_connection;

typedef struct
{
    uint8_t b_channel;
	uint8_t b_cops_type;				//运营商类型
    uint8_t b_signal_pct;				//信号强度 
	uint8_t b_channel_bit_error_rate;
    uint8_t b_pci_en;
    uint32_t dw_reconn_interval;
    uint32_t dw_listen_interval;
    uint8_t b_scan_mode;
    uint8_t b_pmf;
}lte_module_sim_info;

//typedef struct
//{
//	uint8_t b_channel_index;
//}lte_module_reset_tcp_connection;
//
//typedef struct
//{
//	uint8_t b_channel_index;
//	uint8_t ba_domain[100];
//	uint8_t ba_ip[4];
//	uint16_t w_domain_port;
//	uint16_t w_ip_port;
//    void *pv_data;
//}lte_module_open_tcp_connection;
//
//typedef struct
//{
//	uint8_t b_channel_index;
//}lte_module_close_tcp_connection;
//
//typedef struct
//{
//    uint8_t b_channel_index;
//	genaral_communicate_data_info st_data_info;
//}lte_module_send_tcp_user_data;
//
//typedef struct
//{
//    uint8_t b_channel_index;
//	genaral_communicate_data_info *pst_data_info;
//}lte_module_receive_tcp_user_data;
//
//typedef struct
//{
//    uint8_t b_channel_index;
//	genaral_communicate_data_info st_data_info;
//}lte_module_send_ble_user_data;
//
//typedef struct
//{
//    uint8_t b_channel_index;
//	genaral_communicate_data_info st_data_info;
//}lte_module_receive_ble_user_data;
//
typedef struct
{
    uint8_t b_channel_index;
	lte_module_sim_info *pst_data_info;
}lte_module_get_sim_info_connection;

typedef enum
{
    //Initial
    lte_4g_module_operate_initial,
    lte_4g_module_operate_reset,
    //Wifi config
    lte_4g_module_operate_open_ap_connection,
    lte_4g_module_operate_close_ap_connection,
    lte_4g_module_operate_get_ap_info,

    //Ble config
    lte_4g_module_operate_ble_config,
    lte_4g_module_operate_ble_close,
    lte_4g_module_operate_ble_open_broadcast,
    lte_4g_module_operate_send_ble_user_data,
    lte_4g_module_operate_receive_ble_user_data,
    //TCP connection
    lte_4g_module_operate_reset_tcp_connection,
    lte_4g_module_operate_open_tcp_connection,
    lte_4g_module_operate_close_tcp_connection,
    lte_4g_module_operate_send_tcp_user_data,
    lte_4g_module_operate_receive_tcp_user_data,
    //4g config
	lte_4g_module_operate_open_sim_card_connection,
	lte_4g_module_operate_close_sim_card_connection,
	lte_4g_module_operate_get_sim_info,
    //number
    lte_4g_module_operate_type_number,
}lte_4g_module_operate_type;



typedef struct
{
    lte_4g_module_operate_type enum_operate_type;
    lte_module_driver_operate_result_notify_callback pfunc_operate_result_notify;
    union
    {
            //sim
        lte_module_open_sim_connection st_open_sim_connection;
        lte_module_get_sim_info_connection st_get_sim_info_connection;
        //tcp
        lte_module_reset_tcp_connection st_reset_tcp_connection;
        lte_module_open_tcp_connection st_open_tcp_connection;
        lte_module_close_tcp_connection st_close_tcp_connection;
        lte_module_send_tcp_user_data st_send_tcp_user_data;
        lte_module_receive_tcp_user_data st_receive_tcp_user_data;
        //ble
        lte_module_send_ble_user_data st_send_ble_user_data;
        lte_module_receive_ble_user_data st_receive_ble_user_data;
    }union_config_info;
}lte_4g_module_driver_config_info_general;

typedef struct
{
    struct
    {
        uint8_t b_process_reset;
    }operate_contral;

    iot_module_driver_operate_result st_operate_result;
    lte_4g_module_driver_config_info_general *pst_config_info;
}lte_4g_module_driver_process_contral;

void lte_4g_module_driver_initial();
uint8_t lte_4g_module_driver_user_operate_initiate(lte_4g_module_driver_config_info_general *pst_config_info);
uint8_t lte_4g_module_driver_user_operate_execution();
uint8_t lte_4g_module_user_data_send(void *pv_data);
uint8_t lte_4g_module_user_data_receive(void *pv_data);
//uint8_t lte_module_user_data_send(void *pv_data);
//uint8_t lte_module_user_data_receive(void *pv_data);
//
//#endif
#endif

//#endif

