#ifndef _DATA_CONVERT_CAN_CYCLE_2_NET_STATUS_SMP_H_
#define _DATA_CONVERT_CAN_CYCLE_2_NET_STATUS_SMP_H_

#include <stdint.h>
#include "net_system_info.h"
#include "application_general_func.h"

uint8_t data_convert_can_cycle_2_net_status(uint32_t dw_can_id,uint8_t b_can_cmd,uint8_t *pb_can_data_in);
void device_data_polling_smp(net_device_info * const pst_net_device_info);

#endif
