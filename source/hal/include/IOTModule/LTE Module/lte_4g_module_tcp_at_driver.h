#ifndef _LTE_4G_MODULE_TCP_AT_DRIVER_H_
#define _LTE_4G_MODULE_TCP_AT_DRIVER_H_

#include <stdint.h>
#include "lte_module.h"
#include "lte_4g_module.h"

#define cw_lte_module_tcp_message_length           500

uint8_t lte_4g_module_driver_reset_tcp_connection  (lte_4g_module_driver_config_info_general *pst_config_info);
uint8_t lte_4g_module_driver_open_tcp_connection   (lte_4g_module_driver_config_info_general *pst_config_info);
uint8_t lte_4g_module_driver_close_tcp_connection  (lte_4g_module_driver_config_info_general *pst_config_info);
uint8_t lte_4g_module_driver_send_tcp_user_data    (lte_4g_module_driver_config_info_general *pst_config_info);
uint8_t lte_4g_module_driver_receive_tcp_user_data (lte_4g_module_driver_config_info_general *pst_config_info);
uint8_t lte_4g_module_get_sim_info(lte_4g_module_driver_config_info_general *pst_config_info);
uint8_t lte_4g_module_driver_send_at_get_sim_info(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info);
uint8_t lte_4g_module_driver_receive_at_get_sim_info(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info);



#endif

