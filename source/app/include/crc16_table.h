#ifndef _CRC16_TABLE_H_
#define _CRC16_TABLE_H_

#include "stdint.h"

void crc16_1_byte(uint8_t b_data, uint8_t* pb_crc16_h, uint8_t* pb_crc16_l);

#endif
