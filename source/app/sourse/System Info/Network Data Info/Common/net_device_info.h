#ifndef _NET_DEVICE_INFO_H_
#define _NET_DEVICE_INFO_H_

#include <stdint.h>
#include "net_device_data.h"
#include "net_device_tree.h"
#include "net_device_status.h"

struct _net_device_info;

typedef void (*pfunc_net_device_data_polling)(struct _net_device_info * const pst_net_device_info);
typedef void (*pfunc_net_device_event_inquire)(struct _net_device_info * const pst_net_device_info);
typedef uint8_t (*pfunc_net_device_send_data)(uint8_t b_cable,
										 	  uint8_t b_cmd,
										 	  uint32_t dw_device_address,
										 	  uint16_t w_data_addr,
										 	  uint16_t w_data_in_len,
										 	  uint8_t *pb_data_in);

typedef uint8_t (*pfunc_net_device_receive_data)(uint8_t b_cable,
								            	 uint8_t b_cmd,
								            	 uint32_t dw_device_address,
								            	 uint16_t w_data_addr,
								            	 uint16_t *pw_data_out_len,
								            	 uint8_t *pb_data_out,
                                            	 uint16_t w_data_out_buffer_size,
												 uint8_t *pb_receive_state);

typedef struct
{
	pfunc_net_device_data_polling net_device_data_polling;//device_data_polling_common
	pfunc_net_device_event_inquire net_device_event_inquire;
	pfunc_net_device_send_data net_device_send_data; // wired_network_send_data 数据发送函数
	pfunc_net_device_receive_data net_device_receive_data; // wired_network_receive_data 数据接收分析函数
}net_device_func;

typedef struct _net_device_info
{
    uint32_t *pdw_device_id;
	uint32_t dw_device_address_send;
	uint32_t dw_device_address_receive;
	uint8_t b_wired_communication_cable;
    const net_data_list_info *pst_net_data_list_info;

    net_device_tree *pst_net_device_tree_info;

    net_device_status *pst_net_device_status;
	net_device_func pst_net_device_func;
}net_device_info;

typedef struct
{
    uint16_t w_length;
    net_device_info *pst_device_info;
}net_device_info_list;

void net_device_info_initial(pfunc_net_device_send_data p_net_device_send_data,
							 pfunc_net_device_receive_data p_net_device_receive_data,
							 net_device_info *pst_net_device_info);

uint8_t net_device_send_data(uint8_t b_cable,
							 uint8_t b_cmd,
							 uint32_t dw_device_address,
							 uint16_t w_data_addr,
							 uint16_t w_data_in_len,
							 uint8_t *pb_data_in,
							 net_device_info *pst_net_device_info);

uint8_t net_device_receive_data(uint8_t b_cable,
								uint8_t b_cmd,
								uint32_t dw_device_address,
								uint16_t w_data_addr,
								uint16_t *pw_data_out_len,
								uint8_t *pb_data_out,
								uint16_t w_data_out_buffer_size,
								uint8_t *pb_receive_state,
							    net_device_info *pst_net_device_info);

#endif
