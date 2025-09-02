#ifndef _LTE_MODULE_CONFIG_COMMON_H_
#define _LTE_MODULE_CONFIG_COMMON_H_

#include <stdint.h>
#include "application_general_macro.h"
#include "usart_config_lte.h"

#define lte_module_mps_size             						    cb_lte_mps_size
#define lte_module_data_info             						    genaral_communicate_data_info


//typedef enum
//{
//    //Initial
//    lte_module_operate_initial,
//    lte_module_operate_reset,
//    //Wifi config
//    lte_module_operate_open_ap_connection,
//    lte_module_operate_close_ap_connection,
//    lte_module_operate_get_ap_info,
//
//    //Ble config
//    lte_module_operate_ble_config,
//    lte_module_operate_ble_close,
//    lte_module_operate_ble_open_broadcast,
//    lte_module_operate_send_ble_user_data,
//    lte_module_operate_receive_ble_user_data,
//    //TCP connection
//    lte_module_operate_reset_tcp_connection,
//    lte_module_operate_open_tcp_connection,
//    lte_module_operate_close_tcp_connection,
//    lte_module_operate_send_tcp_user_data,
//    lte_module_operate_receive_tcp_user_data,
//    //number
//    lte_module_operate_type_number,
//}lte_module_operate_type;
//typedef enum
//{
//	//Initial
//	lte_4g_module_operate_initial,
//	lte_4g_module_operate_reset,
//	//Ble config
//	lte_4g_module_operate_ble_config,
//	lte_4g_module_operate_ble_close,
//	lte_4g_module_operate_ble_open_broadcast,
//	lte_4g_module_operate_send_ble_user_data,
//	lte_4g_module_operate_receive_ble_user_data,
//	//4g config
//	lte_4g_module_operate_open_sim_card_connection,
//	lte_4g_module_operate_close_sim_card_connection,
//	//TCP connection
//	lte_4g_module_operate_reset_tcp_connection,
//	lte_4g_module_operate_open_tcp_connection,
//	lte_4g_module_operate_close_tcp_connection,
//	lte_4g_module_operate_send_tcp_user_data,
//	lte_4g_module_operate_receive_tcp_user_data,
//	//number
//	lte_4g_module_operate_type_number,
//}lte_4g_module_operate_type;

typedef uint8_t (*pfunc_lte_module_driver_cmd_str_notify)(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info);
typedef uint8_t (*pfunc_lte_module_driver_cmd_str_send)(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info);
typedef uint8_t (*pfunc_lte_module_driver_cmd_str_receive)(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info);

extern uint8_t ba_esim_card_iccid[50];

uint8_t lte_module_send_data_foreground(lte_module_data_info *pst_lte_module_data_info);
uint8_t lte_module_receive_data_foreground_peek(lte_module_data_info **pst_lte_module_data_info);
void lte_module_receive_data_foreground_confirm();
uint8_t lte_module_receive_data_foreground(lte_module_data_info **pst_lte_module_data_info);
uint8_t lte_module_send_data_background();
uint8_t lte_module_receive_data_background();

uint8_t lte_module_driver_at_cmd_transmit(uint16_t w_receive_timeout,void *pv_user_data,
                                          pfunc_lte_module_driver_cmd_str_notify pfunc_cmd_str_receive_notify,
										  pfunc_lte_module_driver_cmd_str_send pfunc_cmd_str_send,
									      pfunc_lte_module_driver_cmd_str_receive pfunc_cmd_str_receive);
#endif
