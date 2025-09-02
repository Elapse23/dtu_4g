#ifndef _NETWORK_DEVICE_CONFIG_SYNC_H_
#define _NETWORK_DEVICE_CONFIG_SYNC_H_

#include <stdint.h>
#include "system_info_ex_call.h"

#define cb_sync_trig_return_ok          0
#define cb_sync_trig_return_busy        1
#define cb_sync_trig_return_value_err   2
#define cb_sync_trig_return_err         3

typedef enum
{
    sync_type_total = 0,
    sync_type_single,
    sync_type_none,
}sync_type;



void device_config_sync_main();
uint8_t network_device_config_sycn_trig(uint32_t w_device_type,
                                        uint8_t b_config_type,
                                        uint16_t w_config_addr,
                                        uint8_t b_data_len,
                                        uint8_t *pb_data_in,
                                        uint8_t b_sync_reg_num,
                                        uint8_t f_sync_fresh,
                                        uint8_t b_sync_type);
void device_set_attribute_range_manage();
//void device_control_ags_control();

#endif
