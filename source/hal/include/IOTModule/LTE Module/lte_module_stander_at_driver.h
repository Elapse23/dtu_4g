#ifndef _LTE_MODULE_STANDER_AT_DRIVER_H_
#define _LTE_MODULE_STANDER_AT_DRIVER_H_

#include <stdint.h>
#include "lte_module.h"
#include "lte_4g_module.h"


uint8_t lte_module_initial(lte_module_driver_config_info_general *pst_config_info);
uint8_t lte_module_reset(lte_module_driver_config_info_general *pst_config_info);

uint8_t lte_module_open_ap_connect(lte_module_driver_config_info_general *pst_config_info);
uint8_t lte_module_close_ap_connect(lte_module_driver_config_info_general *pst_config_info);
uint8_t lte_module_get_ap_info(lte_module_driver_config_info_general *pst_config_info);
#endif
