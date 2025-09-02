#ifndef _LTE_4G_MODULE_BLE_AT_DRIVER_H_
#define _LTE_4G_MODULE_BLE_AT_DRIVER_H_

#include <stdint.h>
#include "lte_module.h"
#include "lte_4g_module.h"

#define cw_lte_module_ble_message_length           500

extern uint32_t dwa_ble_addr[6];

uint8_t lte_module_ble_config(lte_module_driver_config_info_general *pst_config_info);
uint8_t lte_module_open_broadcast(lte_module_driver_config_info_general *pst_config_info);
uint8_t lte_module_ble_deinit(lte_module_driver_config_info_general *pst_config_info);

uint8_t lte_module_driver_send_ble_user_data    (lte_module_driver_config_info_general *pst_config_info);
uint8_t lte_4g_module_driver_receive_at_ble_deinit(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info);
uint8_t lte_4g_module_driver_send_at_ble_deinit(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info);

#endif

