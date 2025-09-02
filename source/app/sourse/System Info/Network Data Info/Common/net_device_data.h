#ifndef _NET_DEVICE_DATA_H_
#define _NET_DEVICE_DATA_H_

#include <stdint.h>
#include "net_device_data_attribute.h"
#include "device_data_attribute.h"

#define cw_net_set_addr_boundary		 		0x00FF
#define cw_net_status_addr_boundary		 		0x01FF
#define cw_net_control_addr_boundary	 		0x02FF

#define cb_net_data_normal_inquire              0
#define cb_net_data_exclude_general_inquire     1

typedef struct
{
    uint8_t b_active_push_enable;
    uint32_t dw_data_modify_count;
    uint32_t dw_data_modify_count_check;
}net_data_active_push_config;

typedef struct
{
    uint8_t b_inquire_mode;
    uint16_t w_inquire_cmd;
}net_data_inquire_config;

typedef struct
{
    uint8_t b_offline_send_enable;
    uint8_t b_interval_s;
}net_data_offline_config;

typedef struct
{
    uint32_t dw_max_data;
    uint32_t dw_min_data;
    uint32_t dw_step;
    uint8_t f_modify;
}net_data_set_attribute;

typedef struct
{
    uint16_t w_data_address_net;  // 无线通讯协议 后两个字节地址 ck inv status 0x0033110100
	uint16_t w_data_address_device; // 485 协议地址  逆变器显示定时刷新 ck inv status 0xA0 表里为 0x00 开始
    uint8_t *pb_data;  // kII 存储位置
    const net_data_attribute *pst_data_attribute;  // 后台设置项状态描述
    const device_data_attribute *pst_device_data_attribute; // 下位机设置项状态描述
//    net_data_active_push_config st_net_data_auxiliary_config;
    net_data_inquire_config st_net_data_inquire_config;
    net_data_offline_config st_net_data_offline_config;
    net_data_set_attribute *pst_net_data_set_attribute;
}net_data_info;

typedef struct
{
    uint8_t b_net_data_typde;
    uint16_t w_net_data_number;					//每个设备信息表中的最大设备数量
    const net_data_info *pst_net_data_info;
}net_data_list;

typedef struct
{
    const uint16_t w_cmd;
//    const uint8_t *pb_data;//->union//
    union{
        uint8_t ba_data[4];
        struct{
            uint8_t ba_data_addr[2];
            uint8_t ba_inquire_len[2];
        }common_format;
    }inquire_info;
    uint8_t b_data_len;
    uint8_t f_poll_en;
}device_inquire_list;
typedef struct
{
    uint16_t w_net_data_list_length;
    const net_data_list *pst_net_data_list;
    uint16_t w_device_inquire_list_length;
    device_inquire_list *pst_device_inquire_list;
}net_data_list_info;

#endif
