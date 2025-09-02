#ifndef _NET_DEVICE_TREE_H_
#define _NET_DEVICE_TREE_H_

#include <stdint.h>


#define cb_net_device_sn_bytes_len                    		30
#define cb_net_device_id_bytes_len                    		3
#define cb_net_device_firmware_version_bytes_len      		30
#define cb_net_device_subset_num_byte_len             		1

#define cb_net_device_tree_length                     		(cb_net_device_sn_bytes_len +\
                                                      		 cb_net_device_id_bytes_len +\
                                                      		 cb_net_device_firmware_version_bytes_len +\
                                                      		 cb_net_device_subset_num_byte_len)

#define cdw_device_type_cklcd                               0x001A00
#define cdw_device_type_ck_inverter                         0x003300
#define cdw_device_type_mppt                                0x003200
#define cdw_device_type_pv                                  0x00FF00
#define cdw_device_type_meter                               0x800B00
#define cd_device_type_system	                            0xFFFF


#define cdw_rs485_id_u1                                     (0x0011)
#define cdw_rs485_id_u2                                     (0x0012)
#define cdw_rs485_id_u3                                     (0x0013)
#define cdw_rs485_id_v1                                     (0x0021)
#define cdw_rs485_id_v2                                     (0x0022)
#define cdw_rs485_id_v3                                     (0x0023)
#define cdw_rs485_id_w1                                     (0x0031)
#define cdw_rs485_id_w2                                     (0x0032)
#define cdw_rs485_id_w3                                     (0x0033)

#define cdw_mppt_id_type_outer                              0x000080

#define cdw_mppt_parallel_u1_0                              (0x000028 | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_u1_1                              (0x000029 | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_u1_2                              (0x00002A | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_u1_3                              (0x00002B | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_u1_4                              (0x00002C | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_u1_5                              (0x00002D | cdw_mppt_id_type_outer)

#define cdw_mppt_parallel_u2_0                              (0x000030 | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_u2_1                              (0x000031 | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_u2_2                              (0x000032 | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_u2_3                              (0x000033 | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_u2_4                              (0x000034 | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_u2_5                              (0x000035 | cdw_mppt_id_type_outer)

#define cdw_mppt_parallel_u3_0                              (0x000038 | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_u3_1                              (0x000039 | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_u3_2                              (0x00003A | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_u3_3                              (0x00003B | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_u3_4                              (0x00003C | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_u3_5                              (0x00003D | cdw_mppt_id_type_outer)

#define cdw_mppt_parallel_v1_0                              (0x000048 | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_v1_1                              (0x000049 | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_v1_2                              (0x00004A | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_v1_3                              (0x00004B | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_v1_4                              (0x00004C | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_v1_5                              (0x00004D | cdw_mppt_id_type_outer)

#define cdw_mppt_parallel_v2_0                              (0x000050 | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_v2_1                              (0x000051 | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_v2_2                              (0x000052 | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_v2_3                              (0x000053 | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_v2_4                              (0x000054 | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_v2_5                              (0x000055 | cdw_mppt_id_type_outer)

#define cdw_mppt_parallel_v3_0                              (0x000058 | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_v3_1                              (0x000059 | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_v3_2                              (0x00005A | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_v3_3                              (0x00005B | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_v3_4                              (0x00005C | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_v3_5                              (0x00005D | cdw_mppt_id_type_outer)

#define cdw_mppt_parallel_w1_0                              (0x000068 | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_w1_1                              (0x000069 | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_w1_2                              (0x00006A | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_w1_3                              (0x00006B | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_w1_4                              (0x00006C | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_w1_5                              (0x00006D | cdw_mppt_id_type_outer)

#define cdw_mppt_parallel_w2_0                              (0x000070 | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_w2_1                              (0x000071 | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_w2_2                              (0x000072 | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_w2_3                              (0x000073 | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_w2_4                              (0x000074 | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_w2_5                              (0x000075 | cdw_mppt_id_type_outer)

#define cdw_mppt_parallel_w3_0                              (0x000078 | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_w3_1                              (0x000079 | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_w3_2                              (0x00007A | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_w3_3                              (0x00007B | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_w3_4                              (0x00007C | cdw_mppt_id_type_outer)
#define cdw_mppt_parallel_w3_5                              (0x00007D | cdw_mppt_id_type_outer)

typedef struct
{
	uint8_t b_serial_number[cb_net_device_sn_bytes_len];
	uint8_t b_device_id[cb_net_device_id_bytes_len];
	uint8_t b_firmware_version[cb_net_device_firmware_version_bytes_len];
	uint8_t b_subset_num;
}device_tree;

/*
typedef union
{
    uint8_t b_raw_data[cb_net_device_tree_length];
	device_tree st_device_tree;
}device_tree_data;

typedef struct
{
    uint8_t *pb_max_device_tree_num;
    device_tree_data *pst_device_tree_data;
}net_device_tree;
*/
typedef union
{
    uint8_t b_raw_data[cb_net_device_tree_length];
	device_tree st_device_tree;
}net_device_tree;

#endif
