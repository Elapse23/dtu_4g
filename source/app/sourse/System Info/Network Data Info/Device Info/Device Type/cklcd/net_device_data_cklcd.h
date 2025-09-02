#ifndef _NET_DEVICE_DTA_CKLCD_H_
#define _NET_DEVICE_DTA_CKLCD_H_

#include <stdint.h>
#include "net_device_data.h"
#include "net_device_tree.h"
#include "device_data_attribute.h"


typedef struct
{
	uint16_t w_1005;
	uint16_t w_1006;
}net_status_data_cklcd;

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
//	uint16_t w_000B;
//	uint16_t w_000C;
//	uint16_t w_000D;
//	uint16_t w_000E;
//	uint16_t w_000F;

	uint16_t w_0010;
	uint16_t w_0011;
	uint16_t w_0012;
	uint16_t w_0013;
	uint16_t w_0014;
	uint32_t dw_0015;
	uint32_t dw_0016;
	uint16_t w_0017;
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
	uint16_t w_0028;
	uint16_t w_0029;
	uint32_t dw_002A;
	uint32_t dw_002B;
	uint16_t w_002C;
	uint16_t w_002D;
	uint16_t w_002E;
	uint16_t w_002F;

	uint16_t w_0030;
	uint16_t w_0031;
	uint16_t w_0032;
	uint16_t w_0033;
	uint16_t w_0034;
	uint16_t w_0035;
	uint16_t w_0036;
	uint16_t w_0037;
	uint16_t w_0038;
	uint16_t w_0039;
	uint16_t w_003A;
	uint16_t w_003B;
	uint16_t w_003C;
	uint16_t w_003D;
	uint16_t w_003E;
	uint16_t w_003F;

	uint16_t w_0040;
	uint32_t dw_0041;
	uint32_t dw_0042;
	uint16_t w_0043;
	uint16_t w_0044;
	uint16_t w_0045;
	uint16_t w_0046;
	uint16_t w_0047;
	uint16_t w_0048;
	uint16_t w_0049;
	uint16_t w_004A;
	uint16_t w_004B;
	uint16_t w_004C;
	uint16_t w_004D;
	uint16_t w_004E;
	uint16_t w_004F;

	uint16_t w_0050;
	uint16_t w_0051;
	uint16_t w_0052;
	uint16_t w_0053;
	uint16_t w_0054;
	uint16_t w_0055;
	uint16_t w_0056;
	uint16_t w_0057;
	uint16_t w_0058;
	uint16_t w_0059;
	uint16_t w_005A;
	uint16_t w_005B;
	uint16_t w_005C;
	uint16_t w_005D;
	uint16_t w_005E;
	uint16_t w_005F;

	uint16_t w_0060;
	uint16_t w_0061;
	uint16_t w_0062;
	uint16_t w_0063;
	uint16_t w_0064;
	uint16_t w_0065;
	uint16_t w_0066;
	uint32_t dw_0067;
	uint32_t dw_0068;
	uint16_t w_0069;
	uint16_t w_006A;
	uint16_t w_006B;
	uint16_t w_006C;
	uint16_t w_006D;
	uint16_t w_006E;
	uint16_t w_006F;

	uint32_t dw_0070;
	uint32_t dw_0071;
	uint16_t w_0072;
	uint16_t w_0073;
	uint16_t w_0074;
	uint16_t w_0075;
//	uint16_t w_0076;
//	uint16_t w_0077;
//	uint16_t w_0078;
//	uint16_t w_0079;
//	uint16_t w_007A;
//	uint16_t w_007B;
//	uint16_t w_007C;
//	uint16_t w_007D;
//	uint16_t w_007E;
//	uint16_t w_007F;

	uint8_t ba_0080[15];

	uint16_t w_00D0;
	uint16_t w_00D1;
	uint16_t w_00D2;
	uint16_t w_00D3;
	uint16_t w_00D4;
	uint32_t dw_00D5;
//	uint16_t w_00D6;
	uint16_t w_00D7;
	uint16_t w_00D8;
	uint16_t w_00D9;
	uint16_t w_00DA;
	uint16_t w_00DB;
	uint16_t w_00DC;
	uint16_t w_00DD;
	uint16_t w_00DE;
	uint16_t w_00DF;

	uint16_t w_00E0;
	uint16_t w_00E1;
	uint16_t w_00E2;
	uint16_t w_00E3;
	uint16_t w_00E4;
	uint16_t w_00E5;
	uint16_t w_00E6;
	uint16_t w_00E7;
	uint16_t w_00E8;
	uint16_t w_00E9;
	uint16_t w_00EA;
	uint16_t w_00EB;
	uint16_t w_00EC;
	uint16_t w_00ED;
	uint16_t w_00EE;
	uint16_t w_00EF;
}net_set_data_cklcd;

typedef enum{
    cb_inv_type_ck = 0,
    cb_inv_type_apollo_maxx,
    cb_inv_type_ck_ii_hdv2,
    cb_inv_type_riio_sun,
    cb_inv_type_cg,
    cb_inv_type_riio,
    cb_inv_type_ck_,
    cb_inv_type_cm,
    cb_inv_type_cc,
    cb_inv_type_apollo_matrix,
    cb_inv_type_riio_sun_ii,
    cb_inv_type_tyrann,
    cb_inv_type_ck_ii,
    cb_inv_type_riio_sun_ii_split_phase,
    cb_inv_type_apollo_matric_ii_10_15k,
    cb_inv_type_apollo_matric_ii_lite,
    cb_inv_type_apollo_matric_ii_5_8k,
    cb_inv_type_Tyrann_3_5_8k,
    cb_inv_type_riio_ii,
    cb_inv_type_riio_ii_split_phase,
    cb_inv_type_Tyrann_ii_3_5_8k,
    cb_inv_type_Tyrann_ii_10_15k,
    cb_inv_type_riio_sun_ii_gte,
}inv_type;

typedef enum{
    na = 0,
    dc_couple,
    ac_couple,
    ac_dc_couple,
}solar_system;

typedef enum{
    bgk_12 = 1,
    bgk_balancer,
    bms = 4,
}bat_type;

extern const net_data_list_info st_net_data_list_info_cklcd_u1;
extern const net_data_list_info st_net_data_list_info_cklcd_u2;
extern const net_data_list_info st_net_data_list_info_cklcd_u3;

extern const net_data_list_info st_net_data_list_info_cklcd_v1;
extern const net_data_list_info st_net_data_list_info_cklcd_v2;
extern const net_data_list_info st_net_data_list_info_cklcd_v3;

extern const net_data_list_info st_net_data_list_info_cklcd_w1;
extern const net_data_list_info st_net_data_list_info_cklcd_w2;
extern const net_data_list_info st_net_data_list_info_cklcd_w3;

#endif
