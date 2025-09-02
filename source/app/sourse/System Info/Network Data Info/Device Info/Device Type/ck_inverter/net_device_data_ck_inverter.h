#ifndef _NET_DEVICE_DTA_CK_INVERTER_H_
#define _NET_DEVICE_DTA_CK_INVERTER_H_

#include <stdint.h>
#include "net_device_data.h"
#include "net_device_tree.h"
#include "device_data_attribute.h"

#define cb_max_ck_inverter_num          9

#define cdw_rs485_id_send_ck_inverter_u1           (/*cdw_device_type_ck_inverter | */0x0011)
#define cdw_rs485_id_send_ck_inverter_u2           (/*cdw_device_type_ck_inverter | */0x0012)
#define cdw_rs485_id_send_ck_inverter_u3           (/*cdw_device_type_ck_inverter | */0x0013)
#define cdw_rs485_id_send_ck_inverter_v1           (/*cdw_device_type_ck_inverter | */0x0021)
#define cdw_rs485_id_send_ck_inverter_v2           (/*cdw_device_type_ck_inverter | */0x0022)
#define cdw_rs485_id_send_ck_inverter_w1           (/*cdw_device_type_ck_inverter | */0x0031)
#define cdw_rs485_id_send_ck_inverter_w2           (/*cdw_device_type_ck_inverter | */0x0032)

typedef struct
{
	uint16_t w_0100;
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
//	uint16_t w_0155;
//	uint16_t w_0156;
//	uint16_t w_0157;
//	uint16_t w_0158;
//	uint16_t w_0159;
//	uint16_t w_015A;
//	uint16_t w_015B;
//	uint16_t w_015C;
//	uint16_t w_015D;
//	uint16_t w_015E;
//	uint16_t w_015F;
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
	uint16_t w_0213;
	uint16_t w_0214;
	uint16_t w_0215;
	uint16_t w_0216;
	uint16_t w_0217;
	uint16_t w_0218;
	uint16_t w_0219;
	uint16_t w_021A;
	uint16_t w_021B;
	uint16_t w_021C;
	uint16_t w_021D;
	uint16_t w_021E;
	uint16_t w_021F;

	uint16_t w_0220;
	uint16_t w_0221;
	uint16_t w_0222;
	uint16_t w_0223;
	uint16_t w_0224;
	uint16_t w_0225;
	uint16_t w_0226;
	uint16_t w_0227;
	uint16_t w_0228;
	uint16_t w_0229;
	uint16_t w_022A;
	uint16_t w_022B;
	uint16_t w_022C;
	uint16_t w_022D;
//	uint16_t w_022E;
//	uint16_t w_022F;
//	
//	uint16_t w_0230;
//	uint16_t w_0231;
//	uint16_t w_0232;
//	uint16_t w_0233;
//	uint16_t w_0234;
//	uint16_t w_0235;
//	uint16_t w_0236;
//	uint16_t w_0237;
//	uint16_t w_0238;
//	uint16_t w_0239;
//	uint16_t w_023A;
//	uint16_t w_023B;
//	uint16_t w_023C;
//	uint16_t w_023D;
//	uint16_t w_023E;
//	uint16_t w_023F;
//	
//	uint16_t w_0240;
//	uint16_t w_0241;
//	uint16_t w_0242;
//	uint16_t w_0243;
//	uint16_t w_0244;
//	uint16_t w_0245;
//	uint16_t w_0246;
//	uint16_t w_0247;
//	uint16_t w_0248;
//	uint16_t w_0249;
//	uint16_t w_024A;
//	uint16_t w_024B;
//	uint16_t w_024C;
}net_status_data_ck_inverter;

typedef struct
{
	uint16_t w_0000;
	uint16_t w_0001;
//	uint16_t w_0002;
//	uint16_t w_0003;
//	uint16_t w_0004;
//	uint16_t w_0005;
//	uint16_t w_0006;
//	uint16_t w_0007;
//	uint16_t w_0008;
//	uint16_t w_0009;
//	uint16_t w_000A;
//	uint16_t w_000B;
//	uint16_t w_000C;
//	uint16_t w_000D;
//	uint16_t w_000E;
	uint16_t w_000F;

	uint16_t w_0010;
	uint16_t w_0011;
	uint16_t w_0012;
	uint16_t w_0013;
	uint16_t w_0014;
	uint16_t w_0015;
	uint16_t w_0016;
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
//	uint16_t w_0025;
//	uint16_t w_0026;
//	uint16_t w_0027;
//	uint16_t w_0028;
//	uint16_t w_0029;
//	uint16_t w_002A;
//	uint16_t w_002B;
//	uint16_t w_002C;
//	uint16_t w_002D;
//	uint16_t w_002E;
//	uint16_t w_002F;

    uint16_t w_0030;
	uint16_t w_0031;
	uint16_t w_0032;
	uint16_t w_0033;
	uint16_t w_0034;
	uint16_t w_0035;
//	uint16_t w_0036;
//	uint16_t w_0037;
	uint16_t w_0038;
	uint16_t w_0039;
	uint16_t w_003A;
	uint16_t w_003B;
//	uint16_t w_003C;
//	uint16_t w_003D;
//	uint16_t w_003E;
//	uint16_t w_003F;

	uint16_t w_0040;
	uint16_t w_0041;
	uint16_t w_0042;
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
//	uint16_t w_005F;

	uint16_t w_0090;
	uint16_t w_0091;
	uint16_t w_0092;
	uint16_t w_0093;
	uint16_t w_0094;
	uint16_t w_0095;
//	uint16_t w_0096;
//	uint16_t w_0097;
	uint16_t w_0098;
//	uint16_t w_0099;
//	uint16_t w_009A;
//	uint16_t w_009B;
//	uint16_t w_009C;
//	uint16_t w_009D;
//	uint16_t w_009E;
//	uint16_t w_009F;

	uint16_t w_00B0;
	uint16_t w_00B1;
	uint16_t w_00B2;
	uint16_t w_00B3;		//蜂鸣器
//	uint16_t w_00B4;
//	uint16_t w_00B5;
//	uint16_t w_00B6;
//	uint16_t w_00B7;
//	uint16_t w_00B8;
//	uint16_t w_00B9;
//	uint16_t w_00BA;
//	uint16_t w_00BB;
//	uint16_t w_00BC;
//	uint16_t w_00BD;
//	uint16_t w_00BE;
//	uint16_t w_00BF;
	uint16_t w_00C3;		//电池类型

}net_set_data_ck_inverter;

typedef enum
{
    stand_alone = 0,
    one_phase_parallel,
    three_phase_parallel,
}ck_parallel_config;

extern const net_data_list_info st_net_data_list_info_ck_inverter_u1;
extern const net_data_list_info st_net_data_list_info_ck_inverter_u2;
extern const net_data_list_info st_net_data_list_info_ck_inverter_u3;

extern const net_data_list_info st_net_data_list_info_ck_inverter_v1;
extern const net_data_list_info st_net_data_list_info_ck_inverter_v2;
extern const net_data_list_info st_net_data_list_info_ck_inverter_v3;

extern const net_data_list_info st_net_data_list_info_ck_inverter_w1;
extern const net_data_list_info st_net_data_list_info_ck_inverter_w2;
extern const net_data_list_info st_net_data_list_info_ck_inverter_w3;


extern const int pb_net_data_list_info_ck_inveter[cb_max_ck_inverter_num];
extern const int pb_net_data_list_info_ck_inveter_riiosunii[cb_max_ck_inverter_num];
extern const int pb_net_data_list_info_ck_inveter_riiosunii_split_phase[cb_max_ck_inverter_num];

#endif
