#include "net_device_info.h"
#include "application_general_macro.h"


/**
 * @brief 调用发送函数的接口 最后调用的是 wired_network_send_data
 * @param {uint8_t} b_cable
 * @param {uint8_t} b_cmd
 * @param {uint32_t} dw_device_address
 * @param {uint16_t} w_data_addr
 * @param {uint16_t} w_data_in_len
 * @param {uint8_t} *pb_data_in
 * @param {net_device_info} *pst_net_device_info
 * @return {uint8_t}
 * @author Yao YiLiang
 */
uint8_t net_device_send_data(uint8_t b_cable,
							 uint8_t b_cmd,
							 uint32_t dw_device_address,
							 uint16_t w_data_addr,
							 uint16_t w_data_in_len,
							 uint8_t *pb_data_in,
							 net_device_info *pst_net_device_info)
{
	if((NULL == pb_data_in) || (NULL == pst_net_device_info))
	{
		return FALSE;
	}

	if(NULL == pst_net_device_info->pst_net_device_func.net_device_send_data)
	{
		return FALSE;
	}

	return pst_net_device_info->pst_net_device_func.net_device_send_data(b_cable,
							 											 b_cmd,
							 											 dw_device_address,
							 											 w_data_addr,
							 											 w_data_in_len,
							 											 pb_data_in);
}

/**
 * @brief 调用接收函数的接口 最后调用的是 wired_network_receive_data
 * @param {uint8_t} b_cable
 * @param {uint8_t} b_cmd
 * @param {uint32_t} dw_device_address
 * @param {uint16_t} w_data_addr
 * @param {uint16_t} *pw_data_out_len
 * @param {uint8_t} *pb_data_out
 * @param {uint16_t} w_data_out_buffer_size
 * @param {uint8_t} *pb_receive_state
 * @param {net_device_info} *pst_net_device_info
 * @return {uint8_t} 
 * @athor Yao YiLiang
 */
uint8_t net_device_receive_data(uint8_t b_cable,
								uint8_t b_cmd,
								uint32_t dw_device_address,
								uint16_t w_data_addr,
								uint16_t *pw_data_out_len,
								uint8_t *pb_data_out,
								uint16_t w_data_out_buffer_size,
								uint8_t *pb_receive_state,
							    net_device_info *pst_net_device_info)
{
	if((NULL == pb_data_out) || (NULL == pst_net_device_info))
	{
		return FALSE;
	}

	if(NULL == pst_net_device_info->pst_net_device_func.net_device_receive_data)
	{
		return FALSE;
	}

	return pst_net_device_info->pst_net_device_func.net_device_receive_data(b_cable,			//wired_network_receive_data
							 											  	b_cmd,
							 											  	dw_device_address,
							 											  	w_data_addr,
																			pw_data_out_len,
							 											  	pb_data_out,
																			w_data_out_buffer_size,
																			pb_receive_state);
}

void net_device_info_initial(pfunc_net_device_send_data p_net_device_send_data,
							 pfunc_net_device_receive_data p_net_device_receive_data,
							 net_device_info *pst_net_device_info)
{
	if((NULL == p_net_device_send_data) || (NULL == p_net_device_receive_data))
	{
		return;
	}

	pst_net_device_info->pst_net_device_func.net_device_send_data = p_net_device_send_data;
	pst_net_device_info->pst_net_device_func.net_device_receive_data = p_net_device_receive_data;
}
