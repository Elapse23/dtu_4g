#include "data_convert_rs485_net_cklcd.h"
#include <string.h>
#include "freertos_user_config.h"
#include "net_system_info_list.h"
#include "net_system_info_access.h"
#include "net_device_tree.h"
#include "net_system_info.h"
#include "system_info_general_macro.h"
#include "net_device_data_access.h"
#include "application_general_func.h"
#include "device_data_attribute.h"
#include "net_segmented_inquire.h"
#include "rs485_communication_transmission.h"
#include "network_tbb_device_server_communicate.h"
#include "network_protocal_data_assembly.h"
#include "display_management.h"
#include "net_device_tree_build.h"
#include "net_device_data_cklcd.h"
#include "net_device_data_ck_inverter.h"
#include "network_energy_statistics.h"
#include "network_system_type_operate.h"

/**
 * @brief 485设备查询是设备是否变更
 * @param {net_system_info_list} *pst_net_system_info_list
 * @param {net_device_info} *pst_net_device_info_in
 * @param {uint8_t} f_refresh 强制刷新设备树
 * @return {*}
 * @author Yao YiLiang
 */
#define cb_cklcd_sync_list_num          9
uint8_t gb_device_tree_update_flag = 0;
uint8_t rs485_check_device_exist(net_system_info_list *pst_net_system_info_list,net_device_info * const pst_net_device_info_in)
{
    uint8_t b_system_index = 0;
    for(b_system_index = 0;b_system_index < pst_net_system_info_list->w_length;b_system_index++)
    {
        if(pst_net_system_info_list->b_current_type ==\
            pst_net_system_info_list->pst_system_info[b_system_index].b_type)
        {
            break;
        }
    }
    if(b_system_index >= pst_net_system_info_list->w_length)
    {
        return FALSE;
    }

		if(cdw_device_type_cklcd == (*pst_net_device_info_in->pdw_device_id & 0x0000FF00))
        {
			uint8_t b_cnt_1 = 0;
			uint16_t w_system_mode = 0;
            uint16_t w_solar_system = 0;
            // uint16_t w_pv_num = 0;
            uint16_t w_parallel_mode = 0;
            uint16_t w_dsp_ver = 0;
            uint16_t w_lcd_ver = 0;
            uint16_t w_mppt_ver = 0;
			uint8_t  b_inv_num = 0;
			static uint8_t b_inv_num_backup = 0;
            static uint16_t w_system_mode_backup[cb_cklcd_sync_list_num] = {0,};
//            static uint16_t w_solar_system_backup[cb_cklcd_sync_list_num] = {0,};
            // static uint16_t w_pv_num_backup[b_cnt_1] = 0;
            static uint16_t w_parallel_mode_backup[cb_cklcd_sync_list_num] = {0,};
            static uint16_t w_dsp_ver_backup[cb_cklcd_sync_list_num] = {0,};
            static uint16_t w_lcd_ver_backup[cb_cklcd_sync_list_num] = {0,};
            static uint16_t w_mppt_ver_backup[cb_cklcd_sync_list_num] = {0,};

			b_cnt_1 = ((((*pst_net_device_info_in->pdw_device_id & 0xF0) >> 4) - 1)* 3) + ((*pst_net_device_info_in->pdw_device_id & 0x0F) - 1);
            {
                uint16_t w_data_len = 2;
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1 + b_cnt_1],
                                       cb_device_data_access_data_type_set,
                                       0x0004,
                                       &w_data_len,
                                       (uint8_t *)&w_system_mode,
                                       cb_device_data_access_operate_type_read);
            }
            {
                uint16_t w_data_len = 2;
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1 + b_cnt_1],
                                       cb_device_data_access_data_type_set,
                                       0x0000,
                                       &w_data_len,
                                       (uint8_t *)&w_solar_system,
                                       cb_device_data_access_operate_type_read);
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                       cb_device_data_access_data_type_status,
                                       0x0143,
                                       &w_data_len,
                                       (uint8_t *)&w_solar_system,
                                       cb_device_data_access_operate_type_write);
            }
            {
                uint16_t w_data_len = 2;
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1 + b_cnt_1],
                                       cb_device_data_access_data_type_set,
                                       0x0020,
                                       &w_data_len,
                                       (uint8_t *)&w_parallel_mode,
                                       cb_device_data_access_operate_type_read);
				w_parallel_mode = get_system_parallel_mode();
            }
            {
                uint16_t w_data_len = 1;
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                       cb_net_data_att_status,
                                       0x0160,
                                       &w_data_len,
                                       (uint8_t *)&b_inv_num,
                                       cb_device_data_access_operate_type_read);

			}
            {
                uint16_t w_data_len = 2;
                uint8_t b_system_protocal = 0;
                if(TRUE == check_system_protocal_type(&b_system_protocal))
                {
                    if(cb_system_protocal_type_riiosunii == b_system_protocal)
                    {
                        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1 + b_cnt_1],
                                               cb_net_data_att_status,
                                               0x012B,
                                               &w_data_len,
                                               (uint8_t *)&w_dsp_ver,
                                               cb_device_data_access_operate_type_read);
                        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1 + b_cnt_1],
                                               cb_net_data_att_status,
                                               0x012D,
                                               &w_data_len,
                                               (uint8_t *)&w_lcd_ver,
                                               cb_device_data_access_operate_type_read);
						net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_mppt_u1_0 + b_cnt_1],
                                               cb_net_data_att_status,
                                               0x0111,
                                               &w_data_len,
                                               (uint8_t *)&w_mppt_ver,
                                               cb_device_data_access_operate_type_read);
                    }
                    else 
                    {
                      net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1 + b_cnt_1],
                                               cb_net_data_att_status,
                                               0x011C,
                                               &w_data_len,
                                               (uint8_t *)&w_dsp_ver,
                                               cb_device_data_access_operate_type_read);
                      net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1 + b_cnt_1],
                                               cb_net_data_att_status,
                                               0x013A,
                                               &w_data_len,
                                               (uint8_t *)&w_lcd_ver,
                                               cb_device_data_access_operate_type_read);
                      net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_mppt_u1_0 + b_cnt_1],
                                               cb_net_data_att_status,
                                               0x0111,
                                               &w_data_len,
                                               (uint8_t *)&w_mppt_ver,
                                               cb_device_data_access_operate_type_read);
                    }
                }
            }
            if((w_system_mode_backup[b_cnt_1] != w_system_mode) ||\
               //(w_solar_system_backup[b_cnt_1] != w_solar_system) ||\
               /*(w_pv_num_backup != w_pv_num) ||\*/
               (w_parallel_mode_backup[b_cnt_1] != w_parallel_mode) ||\
               (w_dsp_ver_backup[b_cnt_1] != w_dsp_ver) ||\
               (w_lcd_ver_backup[b_cnt_1] != w_lcd_ver) ||\
               (w_mppt_ver_backup[b_cnt_1] != w_mppt_ver)||
               (b_inv_num_backup != b_inv_num))//(w_mppt_num_backup != w_mppt_num) ||
            {
                uint8_t b_system_protocal = 0;
                if(TRUE == check_system_protocal_type(&b_system_protocal))
                {

                    w_system_mode_backup[b_cnt_1] = w_system_mode;
     //               w_solar_system_backup[b_cnt_1] = w_solar_system;
    //                w_mppt_num_backup[b_cnt_1] = w_mppt_num;
                    // w_pv_num_backup[b_cnt_1] = w_pv_num;
                    w_parallel_mode_backup[b_cnt_1] = w_parallel_mode;
                    w_dsp_ver_backup[b_cnt_1] = w_dsp_ver;
                    w_lcd_ver_backup[b_cnt_1] = w_lcd_ver;
                    w_mppt_ver_backup[b_cnt_1] = w_mppt_ver;
					b_inv_num_backup = b_inv_num;
                }
                net_device_info *pst_net_device_info = 0;
		        uint16_t w_data_len = 1;
	        	gb_device_tree_update_flag = 1;
			    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
	                                   cb_device_data_access_data_type_status,
	                                   0x017E,
	                                   &w_data_len,
	                                   (uint8_t *)&gb_device_tree_update_flag,
	                                   cb_device_data_access_operate_type_write);
                network_com_with_tbb_device_server_state_set(cb_net_com_state_type_err_code,net_sever_connect);
                if(cb_system_protocal_type_riiosunii == b_system_protocal)
                {
                    if(stand_alone == w_parallel_mode)
                    {
                        if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[cw_device_id_index_meter_1],
                                                        (net_device_info **)&pst_net_device_info,
                                                        pst_net_system_info_list))
                        {
                            return FALSE;
                        }
                        if(NULL == pst_net_device_info->pst_net_data_list_info->pst_device_inquire_list)
                        {
                            return FALSE;
                        }
                        pst_net_device_info->pst_net_data_list_info->pst_device_inquire_list[0].f_poll_en = TRUE;
                        return net_device_exists_status_refresh(dwa_device_id_kinergy_ii[cw_device_id_index_meter_1],
                                                                cb_net_device_add,
                                                                pst_net_system_info_list);
                    }
                }
                return net_device_exists_status_refresh(dwa_device_id_kinergy_ii[cw_device_id_index_meter_1],
                                                        cb_net_device_loss,
                                                        pst_net_system_info_list);
            }
        }
		else if(cdw_device_type_ck_inverter == (*pst_net_device_info_in->pdw_device_id & 0x00FFFF00))
        {
            if(TRUE == check_system_init(cb_system_init_type_device_tree))
            {
                device_inquire_list *pst_device_inquire_list = 0;
                net_device_info *pst_net_device_info = 0;
                
                if(FALSE == net_get_device_info(*pst_net_device_info_in->pdw_device_id,
                                                (net_device_info **)&pst_net_device_info,
                                                pst_net_system_info_list))
                {
                    return FALSE;
                }
    
                if(NULL == pst_net_device_info->pst_net_data_list_info->pst_device_inquire_list)
                {
                    return FALSE;
                }   
                {                    
                    pst_device_inquire_list =\
                        pst_net_device_info->pst_net_data_list_info->pst_device_inquire_list;
                    
                    uint8_t b_system_protocal = 0;
                    
                    check_system_protocal_type(&b_system_protocal);
                    {
                        if(cb_system_protocal_type_riiosunii == b_system_protocal)
                        {
                            //system_init mark
                            pst_device_inquire_list[1].f_poll_en = TRUE;
                            pst_device_inquire_list[0].f_poll_en = FALSE;
                        }
                        else
                        {
                            pst_device_inquire_list[1].f_poll_en = FALSE;
                            pst_device_inquire_list[0].f_poll_en = TRUE;
                        }
                    }
                }
            }
        }
    return TRUE;
}
uint8_t rs485_get_inquire_cmd(device_inquire_list **pst_device_inquire_list_in,
                              net_device_info * const pst_net_device_info,
                              uint8_t *pb_device_inquire_index)
{
//        static uint8_t b_device_inquire_index = 0;
        {
            if((0 != pst_net_device_info->pst_net_data_list_info->w_device_inquire_list_length) &&\
               (NULL != pst_net_device_info->pst_net_data_list_info))
            {
                for(;*pb_device_inquire_index < pst_net_device_info->pst_net_data_list_info->w_device_inquire_list_length;(*pb_device_inquire_index)++)
                {
                    if(TRUE == pst_net_device_info->
                                pst_net_data_list_info->
                                pst_device_inquire_list[*pb_device_inquire_index].
                                f_poll_en)
                    {
                    	if(FALSE == check_system_type(cb_inv_type_riio_sun_ii_split_phase) && FALSE == check_system_type(cb_inv_type_riio_ii_split_phase))
						{
							if(cw_device_cmd_riio_ii_split_phase_data_read == pst_net_device_info->pst_net_data_list_info->pst_device_inquire_list[*pb_device_inquire_index].w_cmd ||
                               cw_device_cmd_riio_ii_split_phase_meter_data_read == pst_net_device_info->pst_net_data_list_info->pst_device_inquire_list[*pb_device_inquire_index].w_cmd)
                            {
                                continue;
                            }
//                            if(check_system_type(cb_inv_type_tyrann) || check_system_type(cb_inv_type_ck_ii) || check_system_type(cb_inv_type_apollo_matric_ii_10_15k) || 
//                                check_system_type(cb_inv_type_apollo_matric_ii_lite) || check_system_type(cb_inv_type_apollo_matric_ii_5_8k) || check_system_type(cb_inv_type_Tyrann_3_5_8k))
//                            {
//                                if(0xDD <= pst_net_device_info->pst_net_data_list_info->pst_device_inquire_list[*pb_device_inquire_index].inquire_info.common_format.ba_data_addr[1] ||
//									cw_device_cmd_meter_data_read == pst_net_device_info->pst_net_data_list_info->pst_device_inquire_list[*pb_device_inquire_index].w_cmd)
//                                {
//                                    continue;
//                                }
//                            }
						}
						else
						{
							
							if(cw_device_cmd_meter_data_read == pst_net_device_info->pst_net_data_list_info->pst_device_inquire_list[*pb_device_inquire_index].w_cmd)
                            {
                                continue;
                            }
						}

                        *pst_device_inquire_list_in =\
                            &pst_net_device_info->
                                pst_net_data_list_info->
                                pst_device_inquire_list[(*pb_device_inquire_index)++];
                        return TRUE;
                    }
                }
                if(*pb_device_inquire_index >= pst_net_device_info->pst_net_data_list_info->w_device_inquire_list_length)
                {
//                    *pb_device_inquire_index = 0;
                    return FALSE;
                }
            }
        }
    return FALSE;
}

extern SemaphoreHandle_t  pst_rs485_data_polling_mutex;
/************************************************************************************************
* Input: pst_net_device_info : device info
*
* Output: void
*
* Logic discription: read subset data polling func
*                    
*
* run time: 
************************************************************************************************/
void device_data_polling_common(net_device_info * const pst_net_device_info)  // 485轮询
{
    
#if( task_runing_status_print )
    DebugPrintf(TRUE,"Device_data_polling_start\r\n");
#endif

	if(NULL == pst_net_device_info)
	{
		return;
	}
    uint8_t b_device_inquire_index = 0;
	for(;;)
    {
        if(TRUE == net_check_device_communicate_status(cb_net_device_loss,pst_net_device_info))
        {
            if(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1] == *pst_net_device_info->pdw_device_id)
            {
                display_management_parameter_send(normal_mode,com_led,device_loss,TRUE);
            }
            return;
        }
        else if(TRUE == net_check_device_communicate_status(cb_net_device_add,pst_net_device_info))
        {
            {
                rs485_check_device_exist(&st_net_system_info_list,pst_net_device_info);
                uint8_t b_receive_state = 0;
                uint16_t w_receive_data_len = 0;
                static uint8_t ba_receive_data[200] = {0,};
                device_inquire_list *pst_device_inquire_list = 0;

                if(FALSE == rs485_get_inquire_cmd((device_inquire_list **)&pst_device_inquire_list,
                                                  pst_net_device_info,
                                                  &b_device_inquire_index))
                {
                    #if( task_runing_status_print )
                        DebugPrintf(TRUE,"Device_data_polling_end_1\r\n");
                    #endif
                    return;
                }

				if(NULL != pst_rs485_data_polling_mutex)
                {
                    xSemaphoreTake(pst_rs485_data_polling_mutex, portMAX_DELAY);
//					DebugPrintf(TRUE,"[Polling]pst_rs485_data_polling_mutex take\r\n");
                }

				if(TRUE == net_device_send_data(pst_net_device_info->b_wired_communication_cable,
												cb_wired_network_inquire_cmd,
												pst_net_device_info->dw_device_address_send,
												pst_device_inquire_list->w_cmd,
												pst_device_inquire_list->b_data_len,
												(uint8_t *)pst_device_inquire_list->inquire_info.ba_data,
												pst_net_device_info))
				{
					for(;;)
					{
						memset(ba_receive_data,0,sizeof(ba_receive_data));
						if(TRUE == net_device_receive_data(pst_net_device_info->b_wired_communication_cable,
														   cb_wired_network_inquire_cmd,
														   pst_net_device_info->dw_device_address_receive,
														   pst_device_inquire_list->w_cmd,
														   &w_receive_data_len,
														   ba_receive_data,
														   200,
														   &b_receive_state,
														   pst_net_device_info))
						{
							if(cb_wired_network_receive_state_success == b_receive_state)
							{
								if(((*pst_net_device_info->pdw_device_id & 0xFF00) == 0x3300))
								{
						            uint8_t b_inv_num = 0;
									uint16_t w_data_len = 1;
									uint16_t w_max_charge_curr = 0;
									uint16_t w_max_discharge_curr = 0;
									net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
														   cb_net_data_att_status,
														   0x0160,
														   &w_data_len,
														   (uint8_t *)&b_inv_num,
														   cb_device_data_access_operate_type_read);
									w_max_charge_curr = ba_receive_data[140] << 8 | ba_receive_data[141];
									w_max_charge_curr *= b_inv_num;
									ba_receive_data[140] = (w_max_charge_curr & 0xFF00) >> 8;
									ba_receive_data[141] = (w_max_charge_curr & 0x00FF);
									w_max_discharge_curr = ba_receive_data[142] << 8 | ba_receive_data[143];
									w_max_discharge_curr *= b_inv_num;
									ba_receive_data[142] = (w_max_discharge_curr & 0xFF00) >> 8;
									ba_receive_data[143] = (w_max_discharge_curr & 0x00FF);
								}
								rs485_data_write(pst_net_device_info->pdw_device_id,
												 ba_receive_data,
												 w_receive_data_len,
												 pst_device_inquire_list);
								network_ck_type_pv_data_process(pst_net_device_info,
																ba_receive_data,
																w_receive_data_len);
								if(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1] == *pst_net_device_info->pdw_device_id)
								{
									display_management_parameter_send(normal_mode,com_led,device_add,TRUE);
								}
								break;
							}
							else if(cb_wired_network_receive_state_fail == b_receive_state)
							{
								break;
							}
							
							{
								vTaskDelay(0);
							}
						}
						else
						{
							break;
						}
					}
				}

				if (NULL != pst_rs485_data_polling_mutex)
				{
					xSemaphoreGive(pst_rs485_data_polling_mutex);
//					DebugPrintf(TRUE,"[Polling]pst_rs485_data_polling_mutex Give\r\n");
				}
			}
        }
        #if( task_runing_status_print )
            DebugPrintf(TRUE,"Device_data_polling_end_2\r\n");
        #endif

        vTaskDelay(pdMS_TO_TICKS_USER(50));//2023.10.19 原300
    }
}
