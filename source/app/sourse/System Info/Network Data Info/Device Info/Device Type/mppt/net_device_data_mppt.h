#ifndef _NET_DEVICE_DTA_MPPT_H_
#define _NET_DEVICE_DTA_MPPT_H_

#include <stdint.h>
#include "net_device_data.h"
#include "net_device_tree.h"
#include "device_data_attribute.h"

#define cb_max_mppt_num         54

typedef struct
{
	uint16_t w_0100;//SUM
	uint16_t w_0101;
	uint16_t w_0102;
	uint16_t w_0103;
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
	uint16_t w_011C;
	uint16_t w_011D;
	uint16_t w_011E;
	uint16_t w_011F;

	uint16_t w_0120;
	uint16_t w_0121;
	uint16_t w_0122;
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
	uint16_t w_013D;
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
	uint16_t w_014A;
	uint16_t w_014B;
	uint16_t w_014C;
	uint16_t w_014D;
	uint16_t w_014E;
	uint16_t w_014F;

	uint16_t w_0150;
	uint16_t w_0151;
	uint16_t w_0152;
	uint16_t w_0153;
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
	uint16_t w_015E;
	uint16_t w_015F;

	uint16_t w_0160;
	uint16_t w_0161;
	uint16_t w_0162;
	uint16_t w_0163;
}net_status_data_mppt;

//typedef struct
//{
//}net_set_data_cklcd;

extern const net_data_list_info st_net_data_list_info_mppt_u1_0;
extern const net_data_list_info st_net_data_list_info_mppt_u1_1;
extern const net_data_list_info st_net_data_list_info_mppt_u1_2;
extern const net_data_list_info st_net_data_list_info_mppt_u1_3;
extern const net_data_list_info st_net_data_list_info_mppt_u1_4;
extern const net_data_list_info st_net_data_list_info_mppt_u1_5;

extern const net_data_list_info st_net_data_list_info_mppt_u2_0;
extern const net_data_list_info st_net_data_list_info_mppt_u2_1;
extern const net_data_list_info st_net_data_list_info_mppt_u2_2;
extern const net_data_list_info st_net_data_list_info_mppt_u2_3;
extern const net_data_list_info st_net_data_list_info_mppt_u2_4;
extern const net_data_list_info st_net_data_list_info_mppt_u2_5;

extern const net_data_list_info st_net_data_list_info_mppt_u3_0;
extern const net_data_list_info st_net_data_list_info_mppt_u3_1;
extern const net_data_list_info st_net_data_list_info_mppt_u3_2;
extern const net_data_list_info st_net_data_list_info_mppt_u3_3;
extern const net_data_list_info st_net_data_list_info_mppt_u3_4;
extern const net_data_list_info st_net_data_list_info_mppt_u3_5;

extern const net_data_list_info st_net_data_list_info_mppt_v1_0;
extern const net_data_list_info st_net_data_list_info_mppt_v1_1;
extern const net_data_list_info st_net_data_list_info_mppt_v1_2;
extern const net_data_list_info st_net_data_list_info_mppt_v1_3;
extern const net_data_list_info st_net_data_list_info_mppt_v1_4;
extern const net_data_list_info st_net_data_list_info_mppt_v1_5;

extern const net_data_list_info st_net_data_list_info_mppt_v2_0;
extern const net_data_list_info st_net_data_list_info_mppt_v2_1;
extern const net_data_list_info st_net_data_list_info_mppt_v2_2;
extern const net_data_list_info st_net_data_list_info_mppt_v2_3;
extern const net_data_list_info st_net_data_list_info_mppt_v2_4;
extern const net_data_list_info st_net_data_list_info_mppt_v2_5;

extern const net_data_list_info st_net_data_list_info_mppt_v3_0;
extern const net_data_list_info st_net_data_list_info_mppt_v3_1;
extern const net_data_list_info st_net_data_list_info_mppt_v3_2;
extern const net_data_list_info st_net_data_list_info_mppt_v3_3;
extern const net_data_list_info st_net_data_list_info_mppt_v3_4;
extern const net_data_list_info st_net_data_list_info_mppt_v3_5;

extern const net_data_list_info st_net_data_list_info_mppt_w1_0;
extern const net_data_list_info st_net_data_list_info_mppt_w1_1;
extern const net_data_list_info st_net_data_list_info_mppt_w1_2;
extern const net_data_list_info st_net_data_list_info_mppt_w1_3;
extern const net_data_list_info st_net_data_list_info_mppt_w1_4;
extern const net_data_list_info st_net_data_list_info_mppt_w1_5;

extern const net_data_list_info st_net_data_list_info_mppt_w2_0;
extern const net_data_list_info st_net_data_list_info_mppt_w2_1;
extern const net_data_list_info st_net_data_list_info_mppt_w2_2;
extern const net_data_list_info st_net_data_list_info_mppt_w2_3;
extern const net_data_list_info st_net_data_list_info_mppt_w2_4;
extern const net_data_list_info st_net_data_list_info_mppt_w2_5;

extern const net_data_list_info st_net_data_list_info_mppt_w3_0;
extern const net_data_list_info st_net_data_list_info_mppt_w3_1;
extern const net_data_list_info st_net_data_list_info_mppt_w3_2;
extern const net_data_list_info st_net_data_list_info_mppt_w3_3;
extern const net_data_list_info st_net_data_list_info_mppt_w3_4;
extern const net_data_list_info st_net_data_list_info_mppt_w3_5;

extern net_status_data_mppt st_net_status_data_mppt_u1;
extern net_status_data_mppt st_net_status_data_mppt_u2;
extern net_status_data_mppt st_net_status_data_mppt_u3;
extern net_status_data_mppt st_net_status_data_mppt_v1;
extern net_status_data_mppt st_net_status_data_mppt_v2;
extern net_status_data_mppt st_net_status_data_mppt_v3;
extern net_status_data_mppt st_net_status_data_mppt_w1;
extern net_status_data_mppt st_net_status_data_mppt_w2;
extern net_status_data_mppt st_net_status_data_mppt_w3;

extern const int pb_net_data_list_info_mppt[cb_max_mppt_num];
extern const int pb_net_data_list_info_riiosunii_mppt[cb_max_mppt_num];
#endif
