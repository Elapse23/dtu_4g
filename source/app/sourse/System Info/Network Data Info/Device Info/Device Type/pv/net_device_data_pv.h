#ifndef _NET_DEVICE_DTA_PV_H_
#define _NET_DEVICE_DTA_PV_H_

#include <stdint.h>
#include "net_device_data.h"
#include "net_device_tree.h"
#include "device_data_attribute.h"

#define cb_max_pv_num       2

typedef struct
{
	uint16_t w_0100;//SUM
	uint16_t w_0101;//1
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
	uint16_t dw_010C;
	uint16_t dw_010D;
	uint16_t dw_010E;
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
	uint16_t w_011C;//2
	uint16_t w_011D;
	uint16_t w_011E;
	uint16_t w_011F;

	uint16_t w_0120;
	uint16_t w_0121;
	uint16_t w_0122;
	uint16_t w_0123;
	uint16_t w_0124;
	uint16_t dw_0125;
	uint16_t dw_0126;
	uint16_t dw_0127;
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
}net_status_data_pv;

extern const net_data_list_info st_net_data_list_info_pv_1;
extern const net_data_list_info st_net_data_list_info_pv_riiosunii_1;
extern const net_data_list_info st_net_data_list_info_pv_riiosunii_2;

extern net_status_data_pv st_net_status_data_pv;

extern const int pb_net_data_list_info_ck_pv[cb_max_pv_num];
extern const int pb_net_data_list_info_riiosunii_pv[cb_max_pv_num];
#endif
