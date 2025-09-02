#ifndef _CRC16_ARITHMETIC_H_
#define _CRC16_ARITHMETIC_H_

#include "stdint.h"

uint8_t check_crc16(uint8_t *pb_data_in, uint16_t w_data_in_len);
uint8_t calculate_crc16(uint8_t *pb_data_in, uint16_t w_data_in_len,uint8_t *pb_crcl_out,uint8_t *pb_crch_out);

#endif
