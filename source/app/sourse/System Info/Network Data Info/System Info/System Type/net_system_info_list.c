#include "net_system_info_list.h"
#include "net_device_info_kinergy_ii.h"
#include "net_device_tree_relation_kinergy_ii.h"
#include "net_firmware_info_kinergy_ii.h"
#include "net_device_tree_build.h"
#include "net_device_data_polling.h"
//#include "network_eventlog_info_operate.h"

#define cw_net_system_info_list_length    					  			 1


net_system_info sta_net_system_info[cw_net_system_info_list_length] =
{
    {
        cb_system_type_kinergy_ii,
        &st_net_device_info_list_kinergy_ii,
        &st_net_device_update_info_list_kinergy_ii,
        &st_net_device_tree_element_info_list_kinergy_ii,
    },
};

// !程序总数据结构体 关键结构体
net_system_info_list st_net_system_info_list =
{
	.st_net_system_config.byte = 0,
	.st_net_system_func.net_set_timestamp = 0,
	.st_net_system_func.net_get_timestamp = 0,
	.st_net_system_func.net_inquire_device_tree = &net_inquire_device_tree,
	.st_net_system_func.net_device_data_polling = &net_device_data_polling,
//    .st_net_system_func.net_inquire_device_event = &network_event_log_polling_func;
    .b_current_type = cb_system_type_kinergy_ii,
    .w_length = cw_net_system_info_list_length,
    .pst_system_info = &sta_net_system_info[0],
};
