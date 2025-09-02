#ifndef _NET_DEVICE_DTA_KINERGY_H_
#define _NET_DEVICE_DTA_KINERGY_H_

#include <stdint.h>
#include "net_device_data.h"
#include "device_data_attribute.h"

typedef struct
{
	uint8_t  ba_0100[100];
	uint8_t  ba_0101[100];
	uint32_t dw_0102;
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
	uint32_t dw_0124;
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
	uint32_t dw_0144;
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
	uint16_t w_015C;
	uint16_t w_015D;
	uint32_t dw_015E;
	uint8_t b_015F;
	uint8_t b_0160;
    uint16_t w_0161;
    uint16_t w_0162;
    uint16_t w_0163;
    uint16_t w_0164;
    uint16_t w_0165;
    uint16_t w_0166;
    uint16_t w_0167;
    uint16_t w_0168;
    uint16_t w_0169;
    uint16_t w_016A;
    uint16_t w_016B;
    uint16_t w_016C;
    uint16_t w_016D;
    uint16_t w_016E;
    uint16_t w_016F;

    uint16_t w_0170;
    uint16_t w_0171;
    uint16_t w_0172;
    uint16_t w_0173;
    uint16_t w_0174;
	uint16_t w_0175;
	uint16_t w_0176;
	uint16_t w_0177;
	uint16_t w_0178;
	uint16_t w_0179;
	uint8_t b_017A;
	uint16_t w_017B;
	uint8_t  ba_017C[30];
	uint16_t w_017D;
	uint8_t b_017E;
	uint8_t b_017F;
	uint32_t dw_0180;
	uint16_t w_0181;

	uint16_t w_018E;
}net_status_data_kinergy;

typedef struct
{
	uint16_t w_0000;
	uint16_t w_0001;
	uint16_t w_0002;
	uint16_t w_0003;
	uint16_t w_0004;
	uint16_t w_0005;
	uint16_t w_0006;
	uint16_t w_0007;
	uint16_t w_0008;
	uint16_t w_0009;
	uint16_t w_000A;
	uint16_t w_000B;
	uint16_t w_000C;
	uint16_t w_000D;
	uint16_t w_000E;
	uint16_t w_000F;

	uint32_t dw_0010;
	uint32_t dw_0011;
	uint32_t dw_0012;
	uint32_t dw_0013;
	uint32_t dw_0014;
	uint32_t dw_0015;
	uint32_t dw_0016;
	uint32_t dw_0017;
	uint16_t w_0018;
	uint16_t w_0019;
	uint16_t w_001A;
	uint16_t w_001B;
	uint16_t w_001C;
	uint16_t w_001D;
	uint16_t w_001E;
	uint16_t w_001F;

	uint16_t w_0020;
	uint16_t w_0021;
	uint16_t w_0022;
	uint16_t w_0023;
	uint16_t w_0024;
	uint16_t w_0025;
	uint16_t w_0026;
	uint16_t w_0027;
	uint32_t dw_0028;
	uint32_t dw_0029;
	uint32_t dw_002A;
	uint32_t dw_002B;
	uint32_t dw_002C;
	uint32_t dw_002D;
	uint32_t dw_002E;
	uint32_t dw_002F;

//	uint16_t w_0030;
	uint8_t b_0031;
	uint8_t b_0032;
	uint8_t b_0033;
	uint8_t b_0034;
	uint8_t b_0035;
	uint8_t b_0036;
	uint8_t b_0037;
	uint8_t b_0038;
	uint16_t w_0039;
	uint16_t w_003A;
	uint16_t w_003B;
//	uint16_t w_003C;
//	uint16_t w_003D;
//	uint16_t w_003E;
	uint8_t b_003F;
	uint8_t b_0040;
	uint8_t b_0041;
	uint16_t w_0042;
	uint16_t w_0043;

}net_set_data_kinergy;
typedef struct
{
	uint8_t b_0A00;
}
net_control_data_kinergy;

extern net_control_data_kinergy st_net_control_data_kinergy;

extern net_set_data_kinergy st_net_set_data_kinergy;
extern net_status_data_kinergy st_net_status_data_kinergy;

extern const net_data_list_info st_net_data_list_info_kinergy;


#endif
