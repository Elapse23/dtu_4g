#ifndef _NET_FIRMWARE_UPDATE_H_
#define _NET_FIRMWARE_UPDATE_H_

#include <stdint.h>
#include "net_system_info_list.h"
#include "network_protocal_data_assembly.h"

#define cw_net_max_receive_package_len          512
#define cw_subset_update_max_receive_len        256

#if ((0 != (cw_net_max_receive_package_len % 256)) ||\
     (0 != (cw_net_max_receive_package_len % cw_subset_update_max_receive_len)))
    #error net receive package len error
#endif

typedef struct
{
	uint8_t b_status;
	uint8_t b_type;
	uint8_t ba_firmware_packet_num[4];
	uint8_t ba_firmware_id[cb_firmware_id_max_len];
    uint8_t ba_device_id[3];
    uint8_t b_update_type;
}net_firmware_update_start_stop;

typedef struct
{
    uint8_t b_mcu_type;
    uint8_t ba_firmware_type[3];
    uint8_t ba_firmware_app_version[3];
    uint8_t ba_firmware_boot_version[2];
    uint8_t ba_encryption_key[12];
    uint8_t b_encryption_type;
    uint8_t b_update_start_address[4];
    uint8_t b_update_end_address[4];
    uint8_t bFileMD5[cb_firmware_id_max_len];
    uint8_t b_reserve_data[520 - 30 - cb_firmware_id_max_len];
}pacx_packet_info;

typedef struct
{
    uint8_t b_packet_data[520];
}pacx_packet_data;

typedef struct
{
    uint8_t ba_package_num[4];
    uint8_t ba_package_sum[4];
    uint8_t ba_package_len[2];
    uint8_t b_package_type;
    union
    {
        uint8_t ba_package_data[520 - 11];
        struct
        {
            uint8_t b_mcu_type;
            uint8_t ba_firmware_id[6];
            uint8_t ba_firmware_ver[6];
            uint8_t ba_firmware_ver_str[cb_firmware_pac_ver_str_len];
            uint8_t ba_key[12];
            uint8_t b_update_type;
        }st_firmware_pac_info;
        struct
        {
            uint8_t b_mcu_type;
            uint8_t ba_firmware_id[6];
            uint8_t ba_firmware_ver[6];
            uint8_t ba_flash_code_addr[4];
            uint8_t ba_firmware_data[520 - 11 - 17];
        }st_firmware_pac_data;
    }st_pac_packet_data;
}pac_packet;

typedef struct
{
    uint8_t ba_header[4];
    uint8_t ba_key[12];
    uint8_t ba_reserve[239];
    uint8_t b_identification;
}pacx_dsp_packet_info;

typedef struct
{
    uint8_t ba_package_num[4];
    pac_packet st_pac_packet;
}firmware_packet_pac;

typedef struct
{
    uint8_t ba_packet_number[4];
    union pacx_packet
    {
        pacx_packet_info st_pacx_packet_info;
        pacx_packet_data st_pacx_packet_data;
    }un_pacx_packet;
}firmware_packet_pacx;

typedef struct
{
    uint8_t ba_packet_number[4];
    union
    {
        pacx_dsp_packet_info st_pacx_dsp_packet_info;
        pacx_packet_data st_pacx_packet_data;
    }un_pacx_dsp_packet;
}firmware_packet_pacx_dsp;

typedef struct
{
    uint8_t b_packet_format;
    uint8_t b_update_type;
    uint32_t dw_packets_number;
    uint16_t w_device_type;
    uint8_t ba_firmware_id[cb_firmware_id_max_len];
    net_device_update_info *pst_net_device_update_info_current;
}device_update_info_current;

typedef union
{
    firmware_packet_pac      st_firmware_packet_pac;
    firmware_packet_pacx     st_firmware_packet_pacx;
    firmware_packet_pacx_dsp st_firmware_packet_pacx_dsp;
}firmware_packet;


extern device_update_info_current st_device_update_info_current;

uint8_t ftp_update_start(uint32_t dw_secquence,
                         net_firmware_update_start_stop *pst_net_firmware_update_start_stop,
                         net_system_info_list *pst_net_system_info_list,
                         uint8_t b_channel_index,
                         network_data_send_queue px_network_data_send_queue);
uint8_t net_firmware_update_start(uint32_t dw_secquence,
                                  net_firmware_update_start_stop *pst_net_firmware_update_start_stop,\
                                  net_system_info_list *pst_net_system_info_list,
                                  uint8_t b_channel_index,
                                  network_data_send_queue px_network_data_send_queue);
uint8_t net_firmware_update_packet_operate(uint16_t w_firmware_packet_len,\
                                           firmware_packet *pst_firmware_packet,\
                                           net_system_info_list *pst_net_system_info_list);

#endif
