#ifndef _CAN_CYCLE_DATA_SMP_H_
#define _CAN_CYCLE_DATA_SMP_H_

#include <stdint.h>
#include "can_cycle_data.h"

typedef union
{
	uint8_t byte[8];
	struct
	{
		uint16_t bit_0_0_16;
		uint16_t bit_2_0_16;
		uint16_t bit_4_0_16;
		uint16_t bit_6_0_16;
	}bits;
}can_cycle_data_0458_SMP;

typedef union
{
	uint8_t byte[8];
	struct
	{
		uint16_t bit_0_0_16;
		uint16_t bit_2_0_16;
		uint16_t bit_4_0_16;
		uint16_t bit_6_0_16;
	}bits;
}can_cycle_data_0459_SMP;

typedef union
{
	uint8_t byte[8];
	struct
	{
		uint16_t bit_0_0_16;
		uint8_t bit_2_0_8;
		uint8_t bit_3_0_8;
		uint16_t bit_4_0_16;
		uint16_t bit_6_0_16;
	}bits;
}can_cycle_data_045A_SMP;

typedef union
{
	uint8_t byte[8];
	struct
	{
		uint8_t bit_0_0_8;
		uint8_t bit_1_0_8;
		uint8_t bit_2_0_8;
		uint8_t bit_3_0_8;
		uint8_t bit_4_0_3 : 3;
		uint8_t : 5;
		uint8_t bit_5_0_8;
		uint8_t bit_6_0_8;
		uint8_t bit_7_0_8;
	}bits;
}can_cycle_data_045B_SMP;

typedef union
{
	uint8_t byte[3];
	struct
	{
		uint8_t bit_0_0_1 : 1;
		uint8_t bit_0_1_1 : 1;
		uint8_t bit_0_2_1 : 1;
		uint8_t bit_0_3_1 : 1;
		uint8_t bit_0_4_1 : 1;
		uint8_t bit_0_5_1 : 1;
		uint8_t bit_0_6_1 : 1;
		uint8_t bit_0_7_1 : 1;

		uint8_t bit_1_0_1 : 1;
		uint8_t bit_1_1_1 : 1;
		uint8_t bit_1_2_1 : 1;
		uint8_t bit_1_3_1 : 1;
		uint8_t bit_1_4_1 : 1;
		uint8_t bit_1_5_1 : 1;
		uint8_t bit_1_6_1 : 1;
		uint8_t bit_1_7_1 : 1;

		uint8_t bit_2_0_1 : 1;
	}bits;
}can_cycle_data_0460_SMP;

typedef union
{
	uint8_t byte[8];
	struct
	{
		uint8_t bit_0_0_1 : 1;
		uint8_t bit_0_1_1 : 1;
		uint8_t bit_0_2_1 : 1;
		uint8_t bit_0_3_1 : 1;
		uint8_t bit_0_4_1 : 1;
		uint8_t bit_0_5_1 : 1;
		uint8_t bit_0_6_1 : 1;
		uint8_t bit_0_7_1 : 1;

		uint8_t bit_1_0_1 : 1;
		uint8_t bit_1_1_1 : 1;
		uint8_t bit_1_2_1 : 1;
		uint8_t bit_1_3_1 : 1;
		uint8_t bit_1_4_1 : 1;
		uint8_t bit_1_5_1 : 1;
		uint8_t bit_1_6_1 : 1;
		uint8_t bit_1_7_1 : 1;

		uint8_t bit_2_0_1 : 1;
		uint8_t bit_2_1_1 : 1;
		uint8_t bit_2_2_1 : 1;
		uint8_t bit_2_3_1 : 1;
		uint8_t bit_2_4_1 : 1;
		uint8_t bit_2_5_1 : 1;
		uint8_t bit_2_6_1 : 1;
		uint8_t bit_2_7_1 : 1;

		uint8_t bit_3_0_1 : 1;
		uint8_t bit_3_1_1 : 1;
		uint8_t bit_3_2_1 : 1;
		uint8_t bit_3_3_1 : 1;
		uint8_t bit_3_4_1 : 1;
		uint8_t bit_3_5_1 : 1;
		uint8_t bit_3_6_1 : 1;
		uint8_t bit_3_7_1 : 1;

		uint8_t bit_4_0_1 : 1;
		uint8_t bit_4_1_1 : 1;
		uint8_t bit_4_2_1 : 1;
		uint8_t bit_4_3_1 : 1;
		uint8_t bit_4_4_1 : 1;
		uint8_t bit_4_5_1 : 1;
		uint8_t bit_4_6_2 : 2;

		uint8_t bit_5_0_3 : 3;
		uint8_t bit_5_3_1 : 1;
		uint8_t bit_5_4_1 : 1;
		uint8_t bit_5_5_1 : 1;
		uint8_t bit_5_6_1 : 1;
		uint8_t bit_5_7_1 : 1;

		uint8_t bit_6_0_1 : 1;
		uint8_t bit_6_1_1 : 1;
		uint8_t bit_6_2_1 : 1;
		uint8_t bit_6_3_1 : 1;
		uint8_t bit_6_4_1 : 1;
		uint8_t bit_6_5_1 : 1;
		uint8_t bit_6_6_1 : 1;
		uint8_t bit_6_7_1 : 1;

		uint8_t bit_7_0_1 : 1;
		uint8_t bit_7_1_7 : 7;
	}bits;
}can_cycle_data_0461_SMP;


extern can_cycle_data_list_info st_can_cycle_data_list_smp_info;


#endif
