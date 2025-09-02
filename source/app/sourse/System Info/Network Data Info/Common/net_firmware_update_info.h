#ifndef _NET_FIRMWARE_UPDATE_INFO_H_
#define _NET_FIRMWARE_UPDATE_INFO_H_

#include <stdint.h>

#define cb_Z128                             6
#define	cb_S9KEAZ128	                    9
#define cb_GD32F305RCT6                     13
#define cb_N32G455RE                        14
#define cw_device_type_kinergy_ii	        1

#define cb_firmware_format_pac              0
#define cb_firmware_format_pacx_mcu         1
#define cb_firmware_format_pacx_dsp         2
#define cb_firmware_format_none             0xFF

#define cb_firmware_pac_ver_str_len         30

#define cb_firmware_id_max_len 				32

typedef struct
{
	uint8_t b_packet_format;
	uint32_t dw_packets_receive_total;
	uint32_t dw_packets_receive_current;
	uint8_t ba_firmware_id[cb_firmware_id_max_len];
}net_firmware_info;

typedef struct
{
	const uint16_t w_device_type;
	const uint8_t b_mcu_type;
    uint8_t b_firmware_format;
	uint8_t *pb_firmware_file;
	uint8_t *pb_firmware_file_info;
	net_firmware_info st_net_firmware_info;
}net_device_update_info;

typedef struct
{
	uint16_t w_length;
	net_device_update_info *pst_net_device_update_info;
}net_device_update_info_list;

//typedef struct
//{
//	net_device_update_info *pst_net_device_update_info_current;
//	net_device_update_info_list *pst_net_device_update_info_list;
//}net_device_update;

#endif
