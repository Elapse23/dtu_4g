#ifndef _NETWORK_PROTOCAL_DATA_ASSEMBLY_
#define _NETWORK_PROTOCAL_DATA_ASSEMBLY_

#include <stdint.h>
#include "application_general_macro.h"
#include "network_protocal.h"

typedef enum
{
    net_sever_reconnect,
    net_sever_connect,
    net_ap_reconnect,
    net_ap_connect,
    net_receive_success,
    net_receive_wait,
    net_receive_fail,
    net_receive_none,
}net_data_com_result;

typedef enum
{
    net_ble_connect,
    net_ble_reconnect,
    net_ble_disconnect,
    net_ble_close,
    net_ble_reset,
}net_ble_connect_result;


typedef struct
{
    uint8_t b_channel_index;
    genaral_communicate_data_info st_data_info;
}network_data;

typedef enum
{
    cb_channel_type_wireless_module = 0,
	b_channel_type_gprs_module,
    cb_channel_type_ble_module,
    cb_channel_type_4g_module,
}network_channel_type;

typedef struct
{
    uint8_t b_type;
    uint8_t b_status;
}server_response_msg;

#define     cb_net_data_len_header_part          18
typedef struct
{
    uint8_t ba_net_head_len[2];
    uint8_t ba_sequence[4];
    uint8_t ba_protocol_ver[2];
    uint8_t ba_sys_type[4];
    uint8_t ba_time_stamp[4];
    uint8_t ba_cmd_type[2];
}network_data_header_part;
typedef  uint8_t *network_data_body_part;

typedef struct
{
    network_data_header_part *pst_network_data_header_part;
    network_data_body_part pb_network_data_body_part;
}network_data_info;

#define cb_net_decryption_key_len                4
typedef struct
{
    uint16_t w_return;
    uint8_t ba_decryption_key[cb_net_decryption_key_len];
    uint16_t w_crc16;
}net_connect_ack;

typedef struct
{
    uint16_t w_return;
    uint16_t w_crc16;
}net_seg_att_tree_ack;

typedef struct
{
    uint16_t w_return;
    uint16_t w_crc16;
}net_rtdata_ack;

uint8_t net_ack_assembly(uint16_t w_data_in_len,
						 uint8_t *pb_data_in,
						 uint16_t w_net_cmd,
						 uint8_t b_net_cmd_status,
						 uint32_t dw_secquence,
                         uint8_t b_channel_index,
                         network_data_send_queue px_network_data_send_queue);

uint8_t get_net_ack_header_part_assembly(uint16_t *pw_data_out_len,uint8_t *pb_data_out,uint16_t w_net_cmd,uint32_t dw_secquence);
uint8_t Net_Get_AttributeTree_DiscreteAck(uint16_t w_data_in_len,
													uint8_t *pb_data_in,
													uint16_t *pw_data_out_len,
													uint8_t *pb_data_out,
													uint8_t b_net_cmd_status,
													uint8_t b_num_of_inquest_group);

//
//uint8_t ble_ack_assembly(uint16_t w_data_in_len,
//						 uint8_t *pb_data_in,
//						 uint16_t w_net_cmd,
//						 uint8_t b_net_cmd_status,
//						 uint32_t dw_secquence,
//                         void* parameter);

uint8_t net_data_assembly(uint16_t w_net_cmd);
uint8_t net_data_response();
#endif
