#ifndef _NET_SYSTEM_INFO_H_
#define _NET_SYSTEM_INFO_H_

#include <stdint.h>
#include "net_device_info.h"
#include "net_device_tree.h"
#include "net_device_tree_relation.h"
#include "net_firmware_update_info.h"
#include "system_info_general_macro.h"
#include "system_info_ex_call.h"

struct net_system_info_list_;

typedef uint8_t (*pfunc_net_get_timestamp)(uint32_t *p_dw);
typedef uint8_t (*pfunc_net_set_timestamp)(uint32_t p_dw);
typedef void (*pfunc_net_inquire_device_tree)(struct net_system_info_list_ *pst_net_system_info_list);
typedef void (*pfunc_net_system_data_polling)(struct net_system_info_list_ *pst_net_system_info_list);

typedef struct
{
    uint8_t b_type;
    net_device_info_list *pst_device_info_list;
    net_device_update_info_list *pst_net_device_update_info_list;
    const net_device_tree_element_info_list *pst_device_tree_element_info_list;
}net_system_info;

typedef struct
{
	pfunc_net_get_timestamp net_get_timestamp;
	pfunc_net_set_timestamp net_set_timestamp;

	pfunc_net_inquire_device_tree net_inquire_device_tree;
    pfunc_net_inquire_device_tree net_inquire_device_event;
    pfunc_net_inquire_device_tree net_inquire_device_alarm;
	pfunc_net_system_data_polling net_device_data_polling;//net_device_data_polling
}net_system_func;

typedef struct net_system_info_list_
{
	net_byte_8_bits_l_endian st_net_system_config;  // 系统配置 暂时不知道用来干嘛
	net_system_func st_net_system_func;   	   // 系统功能函数
    uint8_t b_current_type;  // 当前系统类型
    uint16_t w_length;  // 网络系统信息列表长度 目前为1
    net_system_info *pst_system_info;  // 网络系统信息列表
}net_system_info_list;

void net_system_info_ex_func_initial(pfunc_net_get_timestamp pfunc_get_timestamp,
							         pfunc_net_set_timestamp pfunc_set_timestamp,
									 net_system_info_list *pst_net_system_info_list);

#endif
