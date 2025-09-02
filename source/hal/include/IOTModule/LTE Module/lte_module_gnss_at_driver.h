#ifndef _LTE_MODULE_GNSS_AT_DRIVER_
#define _LTE_MODULE_GNSS_AT_DRIVER_

#include <stdint.h>
#include "lte_module.h"

uint8_t lte_module_driver_gps_turn_on(lte_module_driver_config_info_general *pst_config_info);
uint8_t lte_module_driver_gps_turn_off(lte_module_driver_config_info_general *pst_config_info);
uint8_t lte_module_driver_gps_acquire_positioning_info(lte_module_driver_config_info_general *pst_config_info);



#endif
