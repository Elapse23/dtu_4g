#ifndef _KEY_OPERATE_H_
#define _KEY_OPERATE_H_

#include "key.h"

uint8_t key_operate(FlagStatus b_Key_Status,
                    uint32_t dw_min_press_cnt,
                    uint32_t dw_max_press_cnt,
                    uint32_t dw_time_out);

#endif



