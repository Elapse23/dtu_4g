#ifndef _NET_DEVICE_DTA_KINERGY_II_H_
#define _NET_DEVICE_DTA_KINERGY_II_H_

#include <stdint.h>
#include "net_device_data.h"
#include "device_data_attribute.h"

typedef struct
{
	uint8_t  ba_0100[100];
	uint8_t  ba_0101[100];
	uint16_t w_0102;
	uint32_t dw_0103;
	uint16_t w_0104;
	uint16_t w_0105;
	uint16_t w_0106;
	uint16_t w_0107;
	uint16_t w_0108;
	uint16_t w_0109;
	uint16_t w_010A;
	uint16_t w_010B;
	uint16_t w_010C;
	uint16_t w_010D;
	uint16_t w_010E;
	uint16_t w_010F;

	uint16_t w_0110;
	uint16_t w_0111;
	uint16_t w_0112;
	uint16_t w_0113;
	uint16_t w_0114;
	uint16_t w_0115;
	uint16_t w_0116;
	uint16_t w_0117;
	uint16_t w_0118;
	uint16_t w_0119;
	uint16_t w_011A;
	uint16_t w_011B;
	uint8_t  b_011C;
	uint32_t dw_011D;
	uint32_t dw_011E;
	uint16_t w_011F;

	uint8_t  b_0120;
	uint8_t  b_0121;
	uint8_t  b_0122;
	uint16_t w_0123;
	uint16_t w_0124;
	uint16_t w_0125;
	uint16_t w_0126;
	uint16_t w_0127;
	uint16_t w_0128;
	uint16_t w_0129;
	uint16_t w_012A;
	uint16_t w_012B;
	uint16_t w_012C;
	uint16_t w_012D;
	uint16_t w_012E;
	uint16_t w_012F;

	uint16_t w_0130;
	uint16_t w_0131;
	uint16_t w_0132;
	uint16_t w_0133;
	uint16_t w_0134;
	uint16_t w_0135;
	uint16_t w_0136;
	uint16_t w_0137;
	uint16_t w_0138;
	uint16_t w_0139;
	uint16_t w_013A;
	uint16_t w_013B;
	uint16_t w_013C;
	uint8_t  b_013D;
	uint16_t w_013E;
	uint16_t w_013F;

	uint16_t w_0140;
	uint16_t w_0141;
	uint16_t w_0142;
	uint16_t w_0143;
	uint16_t w_0144;
	uint16_t w_0145;
	uint16_t w_0146;
	uint16_t w_0147;
	uint16_t w_0148;
	uint16_t w_0149;
	uint8_t  ba_014A[12];
	uint16_t w_014B;
	uint8_t  b_014C;
	uint8_t  b_014D;
	uint32_t dw_014E;
	uint16_t w_014F;

	uint16_t w_0150;
	uint16_t w_0151;
	uint16_t w_0152;
	uint32_t dw_0153;
	uint16_t w_0154;
	uint16_t w_0155;
	uint16_t w_0156;
	uint16_t w_0157;
	uint16_t w_0158;
	uint16_t w_0159;
	uint16_t w_015A;
	uint16_t w_015B;
//	uint16_t w_015C;
//	uint16_t w_015D;
//	uint16_t w_015E;
//	uint16_t w_015F;
}net_status_data_kinergy_ii;

//typedef struct
//{
//    uint8_t ba_0000[2];
//}net_set_data_datastick;

extern const net_data_list_info st_net_data_list_info_kinergy_ii;


#endif
