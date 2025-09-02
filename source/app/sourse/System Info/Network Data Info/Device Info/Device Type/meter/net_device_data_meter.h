#ifndef _NET_DEVICE_DTA_METER_H_
#define _NET_DEVICE_DTA_METER_H_

#include <stdint.h>
#include "net_device_data.h"
#include "net_device_tree.h"
#include "device_data_attribute.h"


typedef struct
{
	uint16_t w_0110;
	uint16_t w_0111;
	uint16_t w_0112;
	uint16_t w_0113;
//	uint16_t w_0114;
//	uint16_t w_0115;
//	uint16_t w_0116;
	uint16_t w_0117;
	uint16_t w_0118;
	uint16_t w_0119;
//	uint16_t w_011A;
//	uint16_t w_011B;
//	uint16_t w_011C;
//	uint16_t w_011D;
//	uint16_t w_011E;
//	uint16_t w_011F;

	uint16_t w_0200;
	uint16_t w_0201;
	uint16_t w_0202;
	uint16_t w_0203;
	uint16_t w_0204;
	uint16_t w_0205;
	uint16_t w_0206;
	uint16_t w_0207;
	uint16_t w_0208;
	uint16_t w_0209;
	uint16_t w_020A;
	uint16_t w_020B;
	uint16_t w_020C;
	uint16_t w_020D;
	uint16_t w_020E;
	uint16_t w_020F;
	uint16_t w_0210;
	uint16_t w_0211;
	uint16_t w_0212;

}net_status_data_meter;

extern const net_data_list_info st_net_data_list_info_meter_1;
#endif
