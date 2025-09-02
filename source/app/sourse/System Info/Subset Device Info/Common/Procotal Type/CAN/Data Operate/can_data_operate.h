#ifndef _CAN_DATA_OPERATE_H_
#define _CAN_DATA_OPERATE_H_

#include <stdint.h>
#include "can_cycle_data.h"
#include "system_info_general_macro.h"

extern uint8_t can_cycle_data_fill(uint32_t dw_can_id,can_cycle_data_list_info *pst_data_list,uint8_t *pb_data_in);

#endif
