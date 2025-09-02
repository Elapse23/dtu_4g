#ifndef _LTE_4G_MODULE_STANDER_AT_DRIVER_H_
#define _LTE_4G_MODULE_STANDER_AT_DRIVER_H_

#include <stdint.h>
#include "lte_4g_module.h"

uint8_t lte_module_initial(lte_module_driver_config_info_general *pst_config_info);
uint8_t lte_module_reset(lte_module_driver_config_info_general *pst_config_info);

uint8_t lte_module_open_ap_connect(lte_module_driver_config_info_general *pst_config_info);
uint8_t lte_module_close_ap_connect(lte_module_driver_config_info_general *pst_config_info);
uint8_t lte_module_get_ap_info(lte_module_driver_config_info_general *pst_config_info);
//uint8_t lte_module_driver_receive_at_chk_sim_signal(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info);
//uint8_t lte_module_driver_send_at_chk_sim_signal(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info);


#endif

