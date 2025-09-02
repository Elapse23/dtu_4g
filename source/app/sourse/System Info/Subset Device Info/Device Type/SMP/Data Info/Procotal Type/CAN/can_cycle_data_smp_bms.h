#ifndef _CAN_CYCLE_DATA_SMP_BMS_H_
#define _CAN_CYCLE_DATA_SMP_BMS_H_

#include <stdint.h>
#include "can_cycle_data.h"

typedef union
{
	uint8_t byte[8];
	struct
	{
		uint16_t bit_0_0_16;
		uint16_t bit_2_0_16;
		uint8_t bit_4_0_8;
		uint8_t bit_5_0_8;
		uint16_t bit_6_0_16;
	}bits;
}can_cycle_data_0452_smp_bms;

typedef union
{
	uint8_t byte[8];
	struct
	{
		uint8_t bit_0_0_8;
		uint16_t : 16;
		uint8_t bit_3_0_8;
		uint8_t bit_4_0_8;
		uint8_t bit_5_0_1;
		uint8_t bit_5_1_1;
		uint8_t bit_5_2_1;
		uint8_t bit_5_3_1;
		uint8_t bit_5_4_1;
		uint8_t : 3;
		uint8_t bit_6_0_8;
		uint8_t bit_7_0_8;
	}bits;
}can_cycle_data_0453_smp_bms;

typedef union
{
	uint8_t byte[4];
	struct
	{
		uint8_t bit_0_0_1;
		uint8_t bit_0_1_1;
		uint8_t bit_0_2_1;
		uint8_t bit_0_3_1;
		uint8_t bit_0_4_1;
		uint8_t bit_0_5_1;
		uint8_t bit_0_6_1;
		uint8_t bit_0_7_1;
		uint8_t bit_1_0_1;
		uint8_t : 7;
		uint8_t bit_2_0_1;
		uint8_t bit_2_1_1;
		uint8_t bit_2_2_1;
		uint8_t bit_2_3_1;
		uint8_t bit_2_4_1;
		uint8_t bit_2_5_1;
		uint8_t bit_2_6_1;
		uint8_t bit_2_7_1;
		uint8_t bit_3_0_1;
		uint8_t : 7;
	}bits;
}can_cycle_data_0454_smp_bms;

typedef union
{
	uint8_t byte[3];
	struct
	{
		uint8_t bit_0_0_8;
		uint8_t bit_1_0_8;
		uint8_t bit_2_0_8;
	}bits;
}can_cycle_data_0455_smp_bms;

#endif
