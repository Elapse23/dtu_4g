#ifndef _LTE_MODULE_TCP_AT_DRIVER_H_
#define _LTE_MODULE_TCP_AT_DRIVER_H_

#include <stdint.h>
#include "lte_module.h"
#include "lte_4g_module.h"


#define cw_lte_module_tcp_message_length           500
uint8_t lte_module_driver_tcp_at_driver_state(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info);
uint8_t lte_module_driver_receive_tcp_notify(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info);
uint8_t lte_module_driver_receive_check(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info);
uint8_t lte_module_driver_send_at_tcp_connect(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info);
uint8_t lte_module_driver_receive_at_tcp_connect(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info);
uint8_t lte_module_driver_send_at_tcp_close(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info);
uint8_t lte_module_driver_receive_at_tcp_close(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info);
uint8_t lte_module_driver_send_at_tcp_close(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info);
uint8_t lte_module_driver_receive_at_tcp_close(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info);
uint8_t lte_module_driver_send_at_tcp_state(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info);
uint8_t lte_module_driver_receive_at_tcp_state(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info);
uint8_t lte_module_tcp_channel_connect(lte_module_driver_config_info_general *pst_config_info,
                                         lte_module_driver_process_contral *pst_process_contral);
uint8_t lte_module_driver_send_at_tcp_cmd(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info);
uint8_t lte_module_driver_receive_at_tcp_cmd(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info);
uint8_t lte_module_driver_send_at_tcp_user_data(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info);
uint8_t lte_module_driver_receive_at_tcp_user_data(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info);
uint8_t lte_module_driver_send_at_receive_tcp_message(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info);
uint8_t lte_module_driver_receive_at_receive_tcp_message(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info);






uint8_t lte_module_driver_reset_tcp_connection  (lte_module_driver_config_info_general *pst_config_info);
uint8_t lte_module_driver_open_tcp_connection   (lte_module_driver_config_info_general *pst_config_info);
uint8_t lte_module_driver_close_tcp_connection  (lte_module_driver_config_info_general *pst_config_info);
uint8_t lte_module_driver_send_tcp_user_data    (lte_module_driver_config_info_general *pst_config_info);
uint8_t lte_module_driver_receive_tcp_user_data (lte_module_driver_config_info_general *pst_config_info);
#endif
