#include "network_protocal_data_assembly.h"
#include "network_protocal_cmd_macro.h"
#include "network_server_response_queue.h"
#include "network_communication_queue.h"
#include "application_general_macro.h"
#include "net_system_info_list.h"
#include "application_general_func.h"
#include "msg_queue_operate.h"
#include "network_local_secquence_generate.h"
#include "system_timestamp_generate.h"
#include "net_device_tree_build.h"
#include "net_active_push_data_build.h"
#include "net_attribute_tree_build.h"
#include "net_real_time_data_build.h"
#include "crc16_arithmetic.h"
#include "iot_module_config_general.h"
#include "net_system_info_access.h"
#include "net_device_data_access.h"
#include "net_segmented_inquire.h"
#include "printf_custom.h"
#include <string.h>
#include "freertos_user_config.h"
#include "network_tbb_device_server_communicate.h"
#include "net_device_tree_system.h"
#include "net_device_tree_kinergy_ii.h"
#include "network_system_type_operate.h"
#include "network_offline_data_operate.h"
#include "network_eventlog_info_operate.h"
#include "network_ble_communication_queue.h"


extern void * pvPortMalloc( size_t xWantedSize );
extern void vPortFree( void * pv );


#define cw_net_tx_data_max_len                  5000
#define cw_net_ack_tx_data_max_len				2000


uint8_t net_inquire_ack(uint16_t w_data_in_len,
					    uint8_t *pb_data_in,
					    uint16_t *pw_data_out_len,
					    uint8_t *pb_data_out,
						uint8_t b_net_cmd_status)
{
    uint32_t dw_device_id = 0;
    uint16_t w_data_start_address = 0;
    uint16_t w_data_end_address = 0;
    uint16_t w_data_len = 0;
//    uint8_t *pb_data_out_backup = 0;

    *pw_data_out_len = 0;
//    pb_data_out_backup = pb_data_out;

	*pb_data_out = 0x00;
    pb_data_out++;
    w_data_len++;

	*pb_data_out = b_net_cmd_status;
    pb_data_out++;
    w_data_len++;

    *pw_data_out_len += w_data_len;

	/*		
		从pd_data_in中获取设备id、起始、结束地址
	*/
    if(FALSE == bytes_to_int_little_to_little_endain(&pb_data_in[0],&dw_device_id,3))
    {
        return FALSE;
    }

    if(FALSE == bytes_to_int_little_to_little_endain(&pb_data_in[3],(uint32_t *)&w_data_start_address,2))
    {
        return FALSE;
    }

    if(FALSE == bytes_to_int_little_to_little_endain(&pb_data_in[5],(uint32_t *)&w_data_end_address,2))
    {
        return FALSE;
    }
	if(0xFFFF00== dw_device_id && 0x0200 == w_data_start_address && 0x0200 == w_data_end_address)
	{
		extern uint8_t gb_device_tree_update_flag;
		uint16_t w_data_len = 1;
		gb_device_tree_update_flag = 0;
		net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
							   cb_device_data_access_data_type_status,
							   0x017E,
							   &w_data_len,
							   (uint8_t *)&gb_device_tree_update_flag,
							   cb_device_data_access_operate_type_write);
		
	}
    if(dwa_device_id_kinergy_ii[cw_device_id_index_system] == dw_device_id)
    {
        if((0x0200 == w_data_start_address ) && (0x0200 == w_data_end_address))
        {
            w_data_len = 0;

            if(FALSE == int_to_bytes_big_endain(&pb_data_out[0],dw_device_id,3))
            {
                return FALSE;
            }
            w_data_len += 3;

            if(FALSE == int_to_bytes_big_endain(&pb_data_out[w_data_len],w_data_start_address,2))
            {
                return FALSE;
            }
            w_data_len += 2;

            if(FALSE == int_to_bytes_big_endain(&pb_data_out[w_data_len],1,2))
            {
                return FALSE;
            }
            w_data_len += 2;

            *pw_data_out_len += w_data_len;

            if(FALSE == net_device_tree_build(&w_data_len,
                                              &pb_data_out[w_data_len],
                                              cw_net_tx_data_max_len,
                                              &st_net_system_info_list))
            {
                return FALSE;
            }
            *pw_data_out_len += w_data_len;
        }
        else if((w_data_end_address >= w_data_start_address) && (w_data_end_address <= 0x00FF))
        {
			net_device_info *pst_net_device_info = 0;
			const net_data_list *pst_net_data_list = 0;

			if(FALSE == net_get_device_info(dw_device_id,
											(net_device_info **)&pst_net_device_info,
											&st_net_system_info_list))
			{
				return FALSE;
			}

            if(FALSE == net_get_device_data_list(cb_net_data_att_set,
                                                 (const net_data_list **)(&pst_net_data_list),
                                                 pst_net_device_info))
            {
                return FALSE;
            }

            if(FALSE == net_get_device_real_time_data(FALSE,
                                                      dw_device_id,
                                                      w_data_start_address,
                                                      w_data_end_address,
                                                      &w_data_len,
                                                      pb_data_out,
                                                      (cw_net_tx_data_max_len - w_data_len),
                                                      pst_net_data_list))
            {
                return FALSE;
            }

            *pw_data_out_len += w_data_len;
        }
        else if((w_data_start_address >= 0x0100) && (w_data_end_address >= w_data_start_address) && (w_data_end_address <= 0x0FFF))
        {
			net_device_info *pst_net_device_info = 0;
			const net_data_list *pst_net_data_list = 0;

			if(FALSE == net_get_device_info(dw_device_id,
											(net_device_info **)&pst_net_device_info,
											&st_net_system_info_list))
			{
				return FALSE;
			}

            if(FALSE == net_get_device_data_list(cb_net_data_att_status,
                                                 (const net_data_list **)(&pst_net_data_list),
                                                 pst_net_device_info))
            {
                return FALSE;
            }

            if(FALSE == net_get_device_real_time_data(FALSE,
                                                      dw_device_id,
                                                      w_data_start_address,
                                                      w_data_end_address,
                                                      &w_data_len,
                                                      pb_data_out,
                                                      (cw_net_tx_data_max_len - w_data_len),
                                                      pst_net_data_list))
            {
                return FALSE;
            }

            *pw_data_out_len += w_data_len;
        }
    }
	/*
		查询设备状态信�?x01xx;
	*/
    else if((w_data_start_address > 0) &&
            (w_data_start_address <= w_data_end_address) &&
            ((w_data_start_address |cw_net_set_addr_boundary) >= cw_net_status_addr_boundary) &&
	        ((w_data_end_address |cw_net_set_addr_boundary) >= cw_net_status_addr_boundary))

    {
		net_device_info *pst_net_device_info = 0;
		const net_data_list *pst_net_data_list = 0;

		if(FALSE == net_get_device_info(dw_device_id,
										(net_device_info **)&pst_net_device_info,
										&st_net_system_info_list))
		{
			return FALSE;
		}

		if(FALSE == net_get_device_data_list(cb_net_data_att_status,
											 (const net_data_list **)(&pst_net_data_list),
											 pst_net_device_info))
		{
			return FALSE;
		}

        if(FALSE == net_get_device_real_time_data(FALSE,
                                                  dw_device_id,
                                                  w_data_start_address,
                                                  w_data_end_address,
                                                  &w_data_len,
                                                  pb_data_out,
                                                  (cw_net_tx_data_max_len - w_data_len),
                                                  pst_net_data_list))
        {
            return FALSE;
        }

        *pw_data_out_len += w_data_len;
    }

	/*
		查询设备设置信息 0x00xx;
	*/
    else if((w_data_start_address <= w_data_end_address) &&
            (w_data_end_address <= cw_net_set_addr_boundary) &&
		    ((w_data_start_address & cw_net_status_addr_boundary) <= cw_net_set_addr_boundary) &&
	  		((w_data_end_address & cw_net_status_addr_boundary) <= cw_net_set_addr_boundary))//(w_data_start_address >= 0)
    {
		net_device_info *pst_net_device_info = 0;
		const net_data_list *pst_net_data_list = 0;

		if(FALSE == net_get_device_info(dw_device_id,
										(net_device_info **)&pst_net_device_info,
										&st_net_system_info_list))
		{
			return FALSE;
		}

		if(FALSE == net_get_device_data_list(cb_net_data_att_set,
											 (const net_data_list **)(&pst_net_data_list),
											 pst_net_device_info))
		{
			return FALSE;
		}

        if(FALSE == net_get_device_real_time_data(FALSE,
                                                  dw_device_id,
                                                  w_data_start_address,
                                                  w_data_end_address,
                                                  &w_data_len,
                                                  pb_data_out,
                                                  (cw_net_tx_data_max_len - w_data_len),
                                                  pst_net_data_list))
        {
            return FALSE;
        }

        *pw_data_out_len += w_data_len;
    }
    else
    {
        return FALSE;
    }

	return TRUE;
}


uint8_t Net_Get_AttributeTree_DiscreteAck(uint16_t w_data_in_len,
													uint8_t *pb_data_in,
													uint16_t *pw_data_out_len,
													uint8_t *pb_data_out,
													uint8_t b_net_cmd_status,
													uint8_t b_num_of_inquest_group)
{
	uint32_t dw_device_id = 0;
	uint16_t w_data_start_address = 0;
	uint16_t w_data_end_address = 0;
	static uint16_t w_data_len = 0;
	if(b_num_of_inquest_group == 0)
	{
		w_data_len = 0;
		*pw_data_out_len = 0;
		
		*pb_data_out = 0x00;
		pb_data_out++;
		w_data_len++;
		
		*pb_data_out = b_net_cmd_status;
		pb_data_out++;
		w_data_len++;
		
		*pw_data_out_len += w_data_len;
	}


	/*		
		从pd_data_in中获取设备id、起始、结束地址
	*/
	if(FALSE == bytes_to_int_little_to_little_endain(&pb_data_in[0],&dw_device_id,3))
	{
		return FALSE;
	}

	if(FALSE == bytes_to_int_little_to_little_endain(&pb_data_in[3],(uint32_t *)&w_data_start_address,2))
	{
		return FALSE;
	}

	if(FALSE == bytes_to_int_little_to_little_endain(&pb_data_in[5],(uint32_t *)&w_data_end_address,2))
	{
		return FALSE;
	}
    if(dwa_device_id_kinergy_ii[cw_device_id_index_system] == dw_device_id)
    {
        if((w_data_end_address >= w_data_start_address) && (w_data_end_address <= 0x00FF))
        {
			net_device_info *pst_net_device_info = 0;
			const net_data_list *pst_net_data_list = 0;

			if(FALSE == net_get_device_info(dw_device_id,
											(net_device_info **)&pst_net_device_info,
											&st_net_system_info_list))
			{
				return FALSE;
			}

            if(FALSE == net_get_device_data_list(cb_net_data_att_set,
                                                 (const net_data_list **)(&pst_net_data_list),
                                                 pst_net_device_info))
            {
                return FALSE;
            }

            if(FALSE == net_get_device_real_time_attribute_tree(FALSE,
                                                      dw_device_id,
                                                      w_data_start_address,
                                                      w_data_end_address,
                                                      &w_data_len,
                                                      pb_data_out,
                                                      (cw_net_tx_data_max_len - w_data_len),
                                                      pst_net_data_list))
            {
                return FALSE;
            }

            *pw_data_out_len += w_data_len;
        }
        else if((w_data_start_address >= 0x0100) && (w_data_end_address >= w_data_start_address) && (w_data_end_address <= 0x0FFF))
        {
			net_device_info *pst_net_device_info = 0;
			const net_data_list *pst_net_data_list = 0;

			if(FALSE == net_get_device_info(dw_device_id,
											(net_device_info **)&pst_net_device_info,
											&st_net_system_info_list))
			{
				return FALSE;
			}

            if(FALSE == net_get_device_data_list(cb_net_data_att_status,
                                                 (const net_data_list **)(&pst_net_data_list),
                                                 pst_net_device_info))
            {
                return FALSE;
            }

            if(FALSE == net_get_device_real_time_attribute_tree(FALSE,
                                                      dw_device_id,
                                                      w_data_start_address,
                                                      w_data_end_address,
                                                      &w_data_len,
                                                      pb_data_out,
                                                      (cw_net_tx_data_max_len - w_data_len),
                                                      pst_net_data_list))
            {
                return FALSE;
            }

            *pw_data_out_len += w_data_len;
        }
    }

	/*
		查询设备状态信�?x01xx;
	*/
	if((w_data_start_address > 0) &&
			(w_data_start_address <= w_data_end_address) &&
			((w_data_start_address |cw_net_set_addr_boundary) >= cw_net_status_addr_boundary) &&
			((w_data_end_address |cw_net_set_addr_boundary) >= cw_net_status_addr_boundary))

	{
		net_device_info *pst_net_device_info = 0;
		const net_data_list *pst_net_data_list = 0;

		if(FALSE == net_get_device_info(dw_device_id,
										(net_device_info **)&pst_net_device_info,
										&st_net_system_info_list))
		{
			return FALSE;
		}

		if(FALSE == net_get_device_data_list(cb_net_data_att_status,
											 (const net_data_list **)(&pst_net_data_list),
											 pst_net_device_info))
		{
			return FALSE;
		}

		if(FALSE == net_get_device_real_time_attribute_tree(FALSE,
												  dw_device_id,
												  w_data_start_address,
												  w_data_end_address,
												  &w_data_len,
												  pb_data_out,
												  (cw_net_tx_data_max_len - w_data_len),
												  pst_net_data_list))
		{
			return FALSE;
		}

		*pw_data_out_len += w_data_len;
	}

	/*
		查询设备设置信息 0x00xx;
	*/
	else if((w_data_start_address <= w_data_end_address) &&
			(w_data_end_address <= cw_net_set_addr_boundary) &&
			((w_data_start_address & cw_net_status_addr_boundary) <= cw_net_set_addr_boundary) &&
			((w_data_end_address & cw_net_status_addr_boundary) <= cw_net_set_addr_boundary))//(w_data_start_address >= 0)
	{
		
		net_device_info *pst_net_device_info = 0;
		const net_data_list *pst_net_data_list = 0;

		if(FALSE == net_get_device_info(dw_device_id,
										(net_device_info **)&pst_net_device_info,
										&st_net_system_info_list))
		{
			return FALSE;
		}

		if(FALSE == net_get_device_data_list(cb_net_data_att_set,
											 (const net_data_list **)(&pst_net_data_list),
											 pst_net_device_info))
		{
			return FALSE;
		}		
		if(FALSE == net_get_device_real_time_attribute_tree(FALSE,
															  dw_device_id,
															  w_data_start_address,
															  w_data_end_address,
															  &w_data_len,
															  pb_data_out,
															  (cw_net_tx_data_max_len - w_data_len),
															  pst_net_data_list))
		{
			return FALSE;
		}

		*pw_data_out_len += w_data_len;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}



uint8_t net_inquire_discrete_ack(uint16_t w_data_in_len,
                        uint8_t *pb_data_in,
                        uint16_t *pw_data_out_len,
                        uint8_t *pb_data_out,
                        uint8_t b_net_cmd_status,
                        uint8_t b_num_of_inquest_group)
{
    uint32_t dw_device_id = 0;
    uint16_t w_data_start_address = 0;
    uint16_t w_data_end_address = 0;
    static uint16_t w_data_len = 0;
	if(b_num_of_inquest_group == 0)
	{
		w_data_len = 0;
		*pw_data_out_len = 0;
		
		*pb_data_out = 0x00;
		pb_data_out++;
		w_data_len++;
		
		*pb_data_out = b_net_cmd_status;
		pb_data_out++;
		w_data_len++;
		
		*pw_data_out_len += w_data_len;
	}


    /*		
        从pd_data_in中获取设备id、起始、结束地址
    */
    if(FALSE == bytes_to_int_little_to_little_endain(&pb_data_in[0],&dw_device_id,3))
    {
        return FALSE;
    }

    if(FALSE == bytes_to_int_little_to_little_endain(&pb_data_in[3],(uint32_t *)&w_data_start_address,2))
    {
        return FALSE;
    }

    if(FALSE == bytes_to_int_little_to_little_endain(&pb_data_in[5],(uint32_t *)&w_data_end_address,2))
    {
        return FALSE;
    }

    /*
        查询设备状态信�?x01xx;
    */
    if((w_data_start_address > 0) &&
            (w_data_start_address <= w_data_end_address) &&
            ((w_data_start_address |cw_net_set_addr_boundary) >= cw_net_status_addr_boundary) &&
            ((w_data_end_address |cw_net_set_addr_boundary) >= cw_net_status_addr_boundary))

    {
        net_device_info *pst_net_device_info = 0;
        const net_data_list *pst_net_data_list = 0;

        if(FALSE == net_get_device_info(dw_device_id,
                                        (net_device_info **)&pst_net_device_info,
                                        &st_net_system_info_list))
        {
            return FALSE;
        }

        if(FALSE == net_get_device_data_list(cb_net_data_att_status,
                                             (const net_data_list **)(&pst_net_data_list),
                                             pst_net_device_info))
        {
            return FALSE;
        }

        if(FALSE == net_get_device_real_time_data(FALSE,
                                                  dw_device_id,
                                                  w_data_start_address,
                                                  w_data_end_address,
                                                  &w_data_len,
                                                  pb_data_out,
                                                  (cw_net_tx_data_max_len - w_data_len),
                                                  pst_net_data_list))
        {
            return FALSE;
        }

        *pw_data_out_len += w_data_len;
    }

    /*
        查询设备设置信息 0x00xx;
    */
    else if((w_data_start_address <= w_data_end_address) &&
            (w_data_end_address <= cw_net_set_addr_boundary) &&
            ((w_data_start_address & cw_net_status_addr_boundary) <= cw_net_set_addr_boundary) &&
            ((w_data_end_address & cw_net_status_addr_boundary) <= cw_net_set_addr_boundary))//(w_data_start_address >= 0)
    {
        
        net_device_info *pst_net_device_info = 0;
        const net_data_list *pst_net_data_list = 0;

        if(FALSE == net_get_device_info(dw_device_id,
                                        (net_device_info **)&pst_net_device_info,
                                        &st_net_system_info_list))
        {
            return FALSE;
        }

        if(FALSE == net_get_device_data_list(cb_net_data_att_set,
                                             (const net_data_list **)(&pst_net_data_list),
                                             pst_net_device_info))
        {
            return FALSE;
        }

        if(FALSE == net_get_device_real_time_data(FALSE,
                                                  dw_device_id,
                                                  w_data_start_address,
                                                  w_data_end_address,
                                                  &w_data_len,
                                                  pb_data_out,
                                                  (cw_net_tx_data_max_len - w_data_len),
                                                  pst_net_data_list))
        {
            return FALSE;
        }

        *pw_data_out_len += w_data_len;
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

						
uint8_t net_get_logs_ack(uint16_t w_data_in_len,
					     uint8_t *pb_data_in,
					     uint16_t *pw_data_out_len,
					     uint8_t *pb_data_out,
					 	 uint8_t b_net_cmd_status)
{
    uint32_t dw_read_log_index = 0;
    uint16_t w_read_log_num = 0;

    if(FALSE == bytes_to_int_little_to_little_endain(&pb_data_in[1],&dw_read_log_index,4))
    {
        return FALSE;
    }
    if(FALSE == bytes_to_int_little_to_little_endain(&pb_data_in[5],(uint32_t *)&w_read_log_num,2))
    {
        return FALSE;
    }

    return read_event_log(pb_data_out,
                          pw_data_out_len,
                          dw_read_log_index,
                          w_read_log_num,
                          cw_net_tx_data_max_len - *pw_data_out_len);
}

static uint8_t net_ack_header_part_assembly(uint16_t *pw_data_out_len,uint8_t *pb_data_out,uint16_t w_net_cmd,uint32_t dw_secquence)
{
    uint8_t f_result = TRUE;

    if(0 == pb_data_out)
    {
        return FALSE;
    }

    network_data_header_part *pst_network_data_header_part = 0;

    pst_network_data_header_part = (network_data_header_part *)pb_data_out;

    f_result &= int_to_bytes_big_endain(pst_network_data_header_part->ba_net_head_len,\
                                        cb_net_data_len_header_part,\
                                        sizeof(uint16_t));

	f_result &= int_to_bytes_big_endain(pst_network_data_header_part->ba_sequence,\
										dw_secquence,\
										sizeof(uint32_t));

    f_result &= int_to_bytes_big_endain(pst_network_data_header_part->ba_protocol_ver,\
                                        0x00,\
                                        sizeof(uint16_t));

    {
        uint32_t dw_net_system_type = 0;
        if(FALSE == get_net_system_type(&dw_net_system_type))
        {
            return FALSE;
        }
        f_result &= int_to_bytes_big_endain(pst_network_data_header_part->ba_sys_type,\
                                            dw_net_system_type,\
                                            sizeof(uint32_t));
    }

    {
        uint32_t dw_timestamp_temp = 0;
        if(get_system_timestamp(&dw_timestamp_temp))
        {
            f_result &= int_to_bytes_big_endain(pst_network_data_header_part->ba_time_stamp,\
                                                dw_timestamp_temp,\
                                                sizeof(uint32_t));
        }
        else
        {
            return FALSE;
        }
    }

    f_result &= int_to_bytes_big_endain(pst_network_data_header_part->ba_cmd_type,\
                                        w_net_cmd,\
                                        sizeof(uint16_t));
    *pw_data_out_len = cb_net_data_len_header_part;

    return f_result;
}
uint8_t get_net_ack_header_part_assembly(uint16_t *pw_data_out_len,uint8_t *pb_data_out,uint16_t w_net_cmd,uint32_t dw_secquence)
{
	return net_ack_header_part_assembly(pw_data_out_len,
										pb_data_out,
										w_net_cmd,
										dw_secquence);

}
static uint8_t net_ack_body_part_assembly(uint16_t w_data_in_len,
										  uint8_t *pb_data_in,
										  uint16_t *pw_data_out_len,
										  uint8_t *pb_data_out,
										  uint16_t w_net_cmd,
										  uint8_t b_net_cmd_status)
{
	if((w_data_in_len != 0) && (NULL == pb_data_in))
	{
		return FALSE;
	}

    if((NULL == pw_data_out_len) || (NULL == pb_data_out))
    {
        return FALSE;
    }

    switch(w_net_cmd)
    {
		case cw_NetConnectAck:
		{
			*pw_data_out_len = 0x02 + w_data_in_len;

			pb_data_out[0] = 0x00;
			pb_data_out[1] = b_net_cmd_status;
			memcpy(&pb_data_out[2],pb_data_in,w_data_in_len);
			return TRUE;
		}
        case cw_NetSettingAck:
		{
			*pw_data_out_len = 0x02 + w_data_in_len;

			pb_data_out[0] = 0x00;
			pb_data_out[1] = b_net_cmd_status;
			memcpy(&pb_data_out[2],pb_data_in,w_data_in_len);
			return TRUE;
		}
		case cw_NetControlAck:
		{
			*pw_data_out_len = 0x02 + w_data_in_len;

			pb_data_out[0] = 0x00;
			pb_data_out[1] = b_net_cmd_status;
			memcpy(&pb_data_out[2],pb_data_in,w_data_in_len);
			return TRUE;
		}
		case cw_NetUpdateStart_StopAck:
        case cw_FTPingAck:
		{
			*pw_data_out_len = 0x02 + w_data_in_len;

			pb_data_out[0] = 0x00;
			pb_data_out[1] = b_net_cmd_status;
			memcpy(&pb_data_out[2],pb_data_in,w_data_in_len);
			return TRUE;
		}
        case cw_NetUpdatingAck:
		{
			*pw_data_out_len = 0x02 + w_data_in_len;

			pb_data_out[0] = 0x00;
			pb_data_out[1] = b_net_cmd_status;
			memcpy(&pb_data_out[2],pb_data_in,w_data_in_len);
			return TRUE;
		}
        case cw_NetInquireAck:
		{
			return net_inquire_ack(w_data_in_len,
								   pb_data_in,
								   pw_data_out_len,
								   pb_data_out,
								   b_net_cmd_status);
		}
        break;
		case cw_NetInquire_DiscreteAck:
		{
			return net_inquire_discrete_ack(w_data_in_len,
								   pb_data_in,
								   pw_data_out_len,
								   pb_data_out,
								   b_net_cmd_status,
								   0);
		}
		break;
        case cw_NetGetLogsAck:
        {
            *pw_data_out_len = 0x02;

			pb_data_out[0] = 0x00;
			pb_data_out[1] = b_net_cmd_status;

            return net_get_logs_ack(w_data_in_len,
								   pb_data_in,
								   pw_data_out_len,
								   pb_data_out,
								   b_net_cmd_status);
        }
        break;
//		case cw_NetSettingAck:
//		{
//		break;
//		case cw_NetControlAck:
//		{
//		}
//		break;
		default:
		break;
    }

     return FALSE;
}

uint8_t net_ack_assembly(uint16_t w_data_in_len,
						 uint8_t *pb_data_in,
						 uint16_t w_net_cmd,
						 uint8_t b_net_cmd_status,
						 uint32_t dw_secquence,
                         uint8_t b_channel_index,
                         network_data_send_queue px_network_data_send_queue)
{
	if(((w_data_in_len != 0) && (NULL == pb_data_in)) ||\
       (NULL == px_network_data_send_queue))
	{
		return FALSE;
	}

	network_data *pst_network_data = 0;
    uint16_t w_data_len = 0;

    if(xPortGetFreeHeapSize() < 6000)
    {
        #if( system_alarm_print)
        DebugPrintf(TRUE,"[ERROR]Net ack err:1\r\n");
        #endif
        return FALSE;
    }
//    do{
//        pst_network_data = (network_data *)pvPortMalloc(sizeof(network_data));
//    }
//    while(0 == pst_network_data);
    pst_network_data = (network_data *)pvPortMalloc(sizeof(network_data));
    if(NULL == pst_network_data)
    {
        #if( system_alarm_print)
        DebugPrintf(TRUE,"[ERROR]Net ack malloc err:1\r\n");
        #endif
        return FALSE;
    }
    #if(dynamic_memory_request_test)
    DebugPrintf(TRUE,"net_ack_assembly_0_malloc%x\n",pst_network_data);
    #endif
    memset((uint8_t *)pst_network_data,0,sizeof(network_data));

//    do{
//        pst_network_data->st_data_info.pb_data = (uint8_t *)pvPortMalloc(cw_net_ack_tx_data_max_len * sizeof(uint8_t));
//    }
//    while(0 == pst_network_data->st_data_info.pb_data);
    pst_network_data->st_data_info.pb_data = (uint8_t *)pvPortMalloc(cw_net_ack_tx_data_max_len * sizeof(uint8_t));
    if(NULL == pst_network_data->st_data_info.pb_data)
    {
        if(NULL != pst_network_data)
        {
            vPortFree(pst_network_data);
        }
        #if( system_alarm_print)
        DebugPrintf(TRUE,"[ERROR]Net ack malloc err:2\r\n");
        #endif
        return FALSE;
    }
    #if(dynamic_memory_request_test)
    DebugPrintf(TRUE,"net_ack_assembly_1_malloc%x\n",pst_network_data->pb_data);
    #endif

    if(TRUE == net_ack_header_part_assembly(&w_data_len,pst_network_data->st_data_info.pb_data,w_net_cmd,dw_secquence))
    {
        pst_network_data->st_data_info.w_data_len += w_data_len;
        if(TRUE == net_ack_body_part_assembly(w_data_in_len,
											  pb_data_in,
											  &w_data_len,
											  &pst_network_data->st_data_info.pb_data[pst_network_data->st_data_info.w_data_len],
											  w_net_cmd,
											  b_net_cmd_status))
        {
            pst_network_data->st_data_info.w_data_len += w_data_len;
            if(TRUE == calculate_crc16(pst_network_data->st_data_info.pb_data,
                                       pst_network_data->st_data_info.w_data_len,
                                       &pst_network_data->st_data_info.pb_data[pst_network_data->st_data_info.w_data_len],
                                       &pst_network_data->st_data_info.pb_data[pst_network_data->st_data_info.w_data_len + 1]))
            {
                pst_network_data->st_data_info.w_data_len += 2;//CRC16
                pst_network_data->b_channel_index = b_channel_index;

                //send
                if(px_network_data_send_queue((pv_queue_operate_msg)pst_network_data))
                {
					return TRUE;
				}
			}
		}
	}

	vPortFree(pst_network_data->st_data_info.pb_data);//1
    #if(dynamic_memory_request_test)
    DebugPrintf(TRUE,"net_ack_assembly_0_free%x\n",pst_network_data->pb_data);
    #endif
	vPortFree(pst_network_data);//2
    #if(dynamic_memory_request_test)
    DebugPrintf(TRUE,"net_ack_assembly_1_free%x\n",pst_network_data);
    #endif

	return FALSE;
}

uint8_t ble_ack_assembly(uint16_t w_data_in_len,
						 uint8_t *pb_data_in,
						 uint16_t w_net_cmd,
						 uint8_t b_net_cmd_status,
						 uint32_t dw_secquence,
                         void* parameter)
{
	if((w_data_in_len != 0) && (NULL == pb_data_in))
	{
		return FALSE;
	}

	network_data *pst_network_data = (network_data *)parameter;
    uint16_t w_data_len = 0;

    if(TRUE == net_ack_header_part_assembly(&w_data_len,pst_network_data->st_data_info.pb_data,w_net_cmd,dw_secquence))
    {
        pst_network_data->st_data_info.w_data_len += w_data_len;
        if(TRUE == net_ack_body_part_assembly(w_data_in_len,
											  pb_data_in,
											  &w_data_len,
											  &pst_network_data->st_data_info.pb_data[pst_network_data->st_data_info.w_data_len],
											  w_net_cmd,
											  b_net_cmd_status))
        {
            pst_network_data->st_data_info.w_data_len += w_data_len;
            if(TRUE == calculate_crc16(pst_network_data->st_data_info.pb_data,
                                       pst_network_data->st_data_info.w_data_len,
                                       &pst_network_data->st_data_info.pb_data[pst_network_data->st_data_info.w_data_len],
                                       &pst_network_data->st_data_info.pb_data[pst_network_data->st_data_info.w_data_len + 1]))
            {
                pst_network_data->st_data_info.w_data_len += 2;//CRC16
			}
		}
	}

	return FALSE;
}

static uint8_t net_data_header_part_assembly(uint16_t *pw_data_out_len,uint8_t *pb_data_out,uint16_t w_net_cmd)
{
    uint8_t f_result = TRUE;

    if(0 == pb_data_out)
    {
        return FALSE;
    }

    network_data_header_part *pst_network_data_header_part = 0;

    pst_network_data_header_part = (network_data_header_part *)pb_data_out;

    f_result &= int_to_bytes_big_endain(pst_network_data_header_part->ba_net_head_len,\
                                        cb_net_data_len_header_part,\
                                        sizeof(uint16_t));
    {
        uint32_t dw_secquence_temp = 0;
        if(network_get_local_message_id(&dw_secquence_temp))
        {
            f_result &= int_to_bytes_big_endain(pst_network_data_header_part->ba_sequence,\
                                                dw_secquence_temp,\
                                                sizeof(uint32_t));
        }
        else
        {
            return FALSE;
        }
    }

    f_result &= int_to_bytes_big_endain(pst_network_data_header_part->ba_protocol_ver,\
                                        0x00,\
                                        sizeof(uint16_t));

    {
        uint32_t dw_net_system_type = 0;
        if(FALSE == get_net_system_type(&dw_net_system_type))
        {
            return FALSE;
        }
        f_result &= int_to_bytes_big_endain(pst_network_data_header_part->ba_sys_type,\
                                            dw_net_system_type,\
                                            sizeof(uint32_t));
    }

    {
        uint32_t dw_timestamp_temp = 0;
        if(get_system_timestamp(&dw_timestamp_temp))
        {
            f_result &= int_to_bytes_big_endain(pst_network_data_header_part->ba_time_stamp,\
                                                dw_timestamp_temp,\
                                                sizeof(uint32_t));
        }
        else
        {
            return FALSE;
        }
    }

    f_result &= int_to_bytes_big_endain(pst_network_data_header_part->ba_cmd_type,\
                                        (w_net_cmd & 0x00FF),\
                                        sizeof(uint16_t));
    *pw_data_out_len = cb_net_data_len_header_part;

    return f_result;
}

//static uint8_t net_data_len_body_part_calculate(uint16_t *w_data_out_len,uint16_t w_net_cmd)
//{
//    if(NULL == w_data_out_len)
//    {
//        return FALSE;
//    }
//
//    switch(w_net_cmd)
//    {
//        case cw_NetConnect:
//            {
//                *w_data_out_len = 64;
//            }
//            break;
//    }
//
//    return TRUE;
//}
static uint8_t net_data_body_part_assembly(uint16_t *pw_data_out_len,uint8_t *pb_data_out,uint16_t w_net_cmd)
{
    if((NULL == pw_data_out_len) || (NULL == pb_data_out))
    {
        return FALSE;
    }

    switch(w_net_cmd)
    {
        case cw_NetConnect:
		{
			return net_device_tree_build(pw_data_out_len,
										 pb_data_out,
										 cw_net_tx_data_max_len,
										 &st_net_system_info_list);
		}
        break;
		case cw_NetGetAttributeTreeAck:
		{
			return net_attribute_tree_build(pw_data_out_len,
										    pb_data_out,
										    cw_net_tx_data_max_len,
										    &st_net_system_info_list,
                                            cb_net_attribute_tree_type_none);
		}
		break;
		case cw_SendAttributeTreeSeg:
		{
			return net_attribute_tree_build(pw_data_out_len,
										    pb_data_out,
										    cw_net_tx_data_max_len,
										    &st_net_system_info_list,
                                            cb_net_attribute_tree_type_none);
		}
		break;
		case cw_SendOffLineAttributeTreeSeg:
		{
			return net_attribute_tree_build(pw_data_out_len,
                                            pb_data_out,
                                            cw_net_tx_data_max_len,
                                            &st_net_system_info_list,
                                            cb_net_attribute_tree_type_offline);
		}
		break;
        case cw_SendSetAttributeTreeSeg:
		{
			return net_attribute_tree_build(pw_data_out_len,
                                            pb_data_out,
                                            cw_net_tx_data_max_len,
                                            &st_net_system_info_list,
                                            cb_net_attribute_tree_type_set_modify);
		}
		case cw_NetRealTimeData:
		{
			return net_real_time_data_build(FALSE,
                                            pw_data_out_len,
                                            pb_data_out,
                                            cw_net_tx_data_max_len,
                                            &st_net_system_info_list);
		}
		break;
//        case cw_NetRealTimeDataActivePush:
//		{
//            uint16_t w_data_out_len = 0;
//
//            pb_data_out[0] = 0x00;
//			pb_data_out[1] = 0x00;
//			*pw_data_out_len = 2;
//
//			if(FALSE == net_active_push_data_build(&w_data_out_len,
//                                                   &pb_data_out[2],
//                                                   cw_net_tx_data_max_len,
//                                                   &st_net_system_info_list))
//            {
//                return FALSE;
//            }
//
//            if(w_data_out_len != 0)
//            {
//                #if(active_push_data_test)
//				DebugPrintf(TRUE,"cw_NetRealTimeDataActivePush,%d\n",xTaskGetTickCount());
//                #endif
//                *pw_data_out_len += w_data_out_len;
//                return TRUE;
//            }
//		}
//		break;
        case cw_NetHeartbeat:
		{
			pb_data_out[0] = 0x00;
			pb_data_out[1] = 0x01;
			*pw_data_out_len = 2;

			return TRUE;
		}
        case cw_NetTimeSynchr:
        {
            return TRUE;
        }
        case cw_NetOffLineRealTimeData:
        {
            return offline_data_flash_read(pb_data_out,pw_data_out_len);
        }
        default:
            break;
    }

     return FALSE;
}

uint8_t net_data_assembly(uint16_t w_net_cmd)
{
    network_data *pst_network_data = 0;
    uint16_t w_data_len = 0;
    if(xPortGetFreeHeapSize() < 6000)
    {
	#if( system_alarm_print)
        DebugPrintf(TRUE,"[ERROR]Net ack err:1\r\n");
	#endif
        return FALSE;
    }

//    do{
//        pst_network_data = (network_data *)pvPortMalloc(sizeof(network_data));
//    }
//    while(0 == pst_network_data);
    pst_network_data = (network_data *)pvPortMalloc(sizeof(network_data));
    if(NULL == pst_network_data)
    {
        #if( system_alarm_print)
        DebugPrintf(TRUE,"[ERROR]Net date malloc err:1\r\n");
        #endif
        return FALSE;
    }
    #if(dynamic_memory_request_test)
    DebugPrintf(TRUE,"net_data_assembly_0_malloc%x\n",pst_network_data);
    #endif
    memset((uint8_t *)pst_network_data,0,sizeof(network_data));

//    do{
//        pst_network_data->st_data_info.pb_data = (uint8_t *)pvPortMalloc(cw_net_tx_data_max_len * sizeof(uint8_t));
//    }
//    while(0 == pst_network_data->st_data_info.pb_data);
    pst_network_data->st_data_info.pb_data = (uint8_t *)pvPortMalloc(cw_net_tx_data_max_len * sizeof(uint8_t));
    if(NULL == pst_network_data->st_data_info.pb_data)
    {
        if(NULL != pst_network_data)
        {
            vPortFree(pst_network_data);
        }
        #if( system_alarm_print)
        DebugPrintf(TRUE,"[ERROR]Net date malloc err:2\r\n");
        #endif
        return FALSE;
    }
    #if(dynamic_memory_request_test)
    DebugPrintf(TRUE,"net_data_assembly_1_malloc%x\n",pst_network_data->pb_data);
    #endif

    if(FALSE == net_data_header_part_assembly(&w_data_len,pst_network_data->st_data_info.pb_data,w_net_cmd))
    {
        vPortFree(pst_network_data->st_data_info.pb_data);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"net_data_assembly_0_free%x\n",pst_network_data->pb_data);
        #endif
        vPortFree(pst_network_data);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"net_data_assembly_1_free%x\n",pst_network_data);
        #endif
        return FALSE;
    }

    if(0 == w_data_len)
    {
        vPortFree(pst_network_data->st_data_info.pb_data);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"net_data_assembly_2_free%x\n",pst_network_data->pb_data);
        #endif
        vPortFree(pst_network_data);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"net_data_assembly_3_free%x\n",pst_network_data);
        #endif
        return FALSE;
    }

    pst_network_data->st_data_info.w_data_len += w_data_len;

    if(FALSE == net_data_body_part_assembly(&w_data_len,&pst_network_data->st_data_info.pb_data[pst_network_data->st_data_info.w_data_len],w_net_cmd))
    {
        vPortFree(pst_network_data->st_data_info.pb_data);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"net_data_assembly_4_free%x\n",pst_network_data->pb_data);
        #endif
        vPortFree(pst_network_data);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"net_data_assembly_5_free%x\n",pst_network_data);
        #endif
        return FALSE;
    }


    if(0 == w_data_len)
    {
        vPortFree(pst_network_data->st_data_info.pb_data);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"net_data_assembly_6_free%x\n",pst_network_data->pb_data);
        #endif
        vPortFree(pst_network_data);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"net_data_assembly_7_free%x\n",pst_network_data);
        #endif
        return FALSE;
    }

    pst_network_data->st_data_info.w_data_len += w_data_len;

    if(FALSE == calculate_crc16(pst_network_data->st_data_info.pb_data,
                                pst_network_data->st_data_info.w_data_len,
                                &pst_network_data->st_data_info.pb_data[pst_network_data->st_data_info.w_data_len],
                                &pst_network_data->st_data_info.pb_data[pst_network_data->st_data_info.w_data_len + 1]))
    {
        vPortFree(pst_network_data->st_data_info.pb_data);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"net_data_assembly_8_free%x\n",pst_network_data->pb_data);
        #endif
        vPortFree(pst_network_data);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"net_data_assembly_9_free%x\n",pst_network_data);
        #endif
        return FALSE;
    }

    pst_network_data->st_data_info.w_data_len += 2;//CRC16

    //send
    if(w_net_cmd == cw_NetGetAttributeTreeAck)
    {
        pst_network_data->b_channel_index = 1;   // cb_channel_type_ble_modulel
        if( FALSE == network_ble_tx_queue_send((pv_queue_operate_msg)pst_network_data))
        {
			vPortFree(pst_network_data->st_data_info.pb_data);
            vPortFree(pst_network_data);
			return FALSE;
		}
    }
    else
    {
        if(FALSE == network_com_tx_data_queue_send((pv_queue_operate_msg)pst_network_data))
        {
            vPortFree(pst_network_data->st_data_info.pb_data);
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"net_data_assembly_10_free%x\n",pst_network_data->pb_data);
            #endif
            vPortFree(pst_network_data);
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"net_data_assembly_11_free%x\n",pst_network_data);
            #endif
            return FALSE;
        }
    }


	#if(net_data_send_print)
    DebugPrintf(TRUE,"Network data send,cmd:%02X,body data len:%d\r\n",w_net_cmd,pst_network_data->st_data_info.w_data_len);
	#endif
    return TRUE;
}

uint8_t net_data_response()
{
    server_response_msg *pst_server_response_msg = 0;
    uint8_t b_result = net_receive_none;
    if(network_server_response_queue_receive((pv_queue_operate_msg *)&pst_server_response_msg,\
                                            FALSE))
    {
        if(pst_server_response_msg != 0)
        {
//            if(w_net_cmd == pst_server_response_msg->b_type)
            {
                  if(network_server_response_queue_shift())
                  {
                      if(cb_net_rsp_success == pst_server_response_msg->b_status)
                      {
                          b_result = net_receive_success;
                      }
                      else if(cb_net_rsp_retry == pst_server_response_msg->b_status)
                      {
                          b_result = net_sever_reconnect;
                      }
                      else if(cb_net_rsp_failed == pst_server_response_msg->b_status)
                      {
                          b_result = net_receive_fail;
                      }
                  }
            }
        }
    }
    else if(net_sever_reconnect == network_com_with_tbb_device_server_state_get())
    {
        b_result = net_sever_reconnect;
    }
	if (pst_server_response_msg)
	{
		vPortFree(pst_server_response_msg);
#if (dynamic_memory_request_test)
		DebugPrintf(TRUE, "net_data_assembly_12_free%x\n", pst_server_response_msg);
#endif
	}
	return b_result;
}

//void clear_network_com_data()
//{
//    {
//        network_data *pst_network_data = 0;
//        while(TRUE == network_com_tx_data_queue_receive(&pst_network_data,TRUE))
//        {
//            vPortFree(pst_network_data->st_data_info.pb_data);
//            #if(dynamic_memory_request_test)
//            DebugPrintf(TRUE,"clear_network_com_data 1,:%x\n",pst_network_data_raw->pb_data);
//            #endif
//            vPortFree(pst_network_data);
//            #if(dynamic_memory_request_test)
//            DebugPrintf(TRUE,"clear_network_com_data 2,:%x\n",pst_network_data_raw);
//            #endif
//        }
//    }
//    {
//        server_response_msg *pst_server_response_msg = 0;
//        while(network_server_response_queue_receive((pv_queue_operate_msg *)&pst_server_response_msg,\
//                                                    TRUE))
//        {
//            if(pst_server_response_msg)
//            {
//              vPortFree(pst_server_response_msg);
//              #if(dynamic_memory_request_test)
//              DebugPrintf(TRUE,"clear_network_com_data 3,:%x\n",pst_server_response_msg);
//              #endif
//            }
//        }
//    }
//}
