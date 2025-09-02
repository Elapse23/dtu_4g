#include "network_communication.h"
#include "network_protocal.h"
#include "application_general_macro.h"
#include "lte_module.h"
#include "net_system_info_list.h"
#include "net_segmented_inquire.h"
#include "system_info_ex_call.h"
#include "net_device_tree_build.h"
#include "net_device_data_access.h"
#include "network_communication_queue.h"
#include "network_local_firmware_update.h"
#include "net_system_info_access.h"
#include "net_system_info_access.h"
#include "network_iot_module_data_channel_manage.h"
#include "iot_module_config_common.h"
#include "network_tbb_device_server_communicate.h"
#include "data_convert_rs485_net_cklcd.h"
#include "network_eventlog_info_operate.h"



void net_system_info_ex_func_initial(pfunc_net_get_timestamp pfunc_get_timestamp,
							         pfunc_net_set_timestamp pfunc_set_timestamp,
									 net_system_info_list *pst_net_system_info_list)
{
	if(pfunc_get_timestamp != NULL)
	{
		pst_net_system_info_list->st_net_system_func.net_get_timestamp = pfunc_get_timestamp;
	}

	if(pfunc_set_timestamp != NULL)
	{
		pst_net_system_info_list->st_net_system_func.net_set_timestamp = pfunc_set_timestamp;
	}

	{
		pfunc_net_device_send_data p_net_device_send_data = 0;
        pfunc_net_device_receive_data p_net_device_receive_data = 0;
		net_system_info *pst_system_info = 0;

		if(TRUE == wired_network_data_transmission_channel_get((pfunc_net_device_send_data *)(&p_net_device_send_data),
                                                               (pfunc_net_device_receive_data *)(&p_net_device_receive_data)))
		{
			if(TRUE == net_get_system_info_current((net_system_info **)&pst_system_info,pst_net_system_info_list))
			{
				for(uint16_t w_cnt_i = 0;w_cnt_i < pst_system_info->pst_device_info_list->w_length;w_cnt_i++)
				{
					net_device_info_initial(p_net_device_send_data,
											p_net_device_receive_data,
											&pst_system_info->pst_device_info_list->pst_device_info[w_cnt_i]);
				}
			}
		}
	}

	{
        uint8_t b_cnt_i;
        for(b_cnt_i = cw_device_id_index_cklcd_u1;
            b_cnt_i <= cw_device_id_index_cklcd_w3;
            b_cnt_i++)
        {
            net_device_info *pst_net_device_info = 0;
            if(TRUE == net_get_device_info(dwa_device_id_kinergy_ii[b_cnt_i],
                                           (net_device_info **)&pst_net_device_info,
                                           pst_net_system_info_list))
            {
                pst_net_device_info->pst_net_device_func.net_device_data_polling = (pfunc_net_device_data_polling)&device_data_polling_common;
            }
        }
        for(b_cnt_i = cw_device_id_index_ck_inverter_u1;
            b_cnt_i <= cw_device_id_index_ck_inverter_w3;
            b_cnt_i++)
        {
            net_device_info *pst_net_device_info = 0;
            if(TRUE == net_get_device_info(dwa_device_id_kinergy_ii[b_cnt_i],
                                           (net_device_info **)&pst_net_device_info,
                                           pst_net_system_info_list))
            {
                pst_net_device_info->pst_net_device_func.net_device_data_polling = (pfunc_net_device_data_polling)&device_data_polling_common;
            }
        }
        for(b_cnt_i = cw_device_id_index_mppt_u1_0;
            b_cnt_i <= cw_device_id_index_mppt_w3_5;
            b_cnt_i += 6)
        {
            net_device_info *pst_net_device_info = 0;
            if(TRUE == net_get_device_info(dwa_device_id_kinergy_ii[b_cnt_i],
                                           (net_device_info **)&pst_net_device_info,
                                           pst_net_system_info_list))
            {
                pst_net_device_info->pst_net_device_func.net_device_data_polling = (pfunc_net_device_data_polling)&device_data_polling_common;
            }
        }
        {
            net_device_info *pst_net_device_info = 0;
            if(TRUE == net_get_device_info(dwa_device_id_kinergy_ii[cw_device_id_index_pv_1],
                                           (net_device_info **)&pst_net_device_info,
                                           pst_net_system_info_list))
            {
                pst_net_device_info->pst_net_device_func.net_device_data_polling = (pfunc_net_device_data_polling)&device_data_polling_common;
            }
        }
        {
            net_device_info *pst_net_device_info = 0;
            if(TRUE == net_get_device_info(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                           (net_device_info **)&pst_net_device_info,
                                           pst_net_system_info_list))
            {
                pst_net_device_info->pst_net_device_func.net_device_event_inquire = (pfunc_net_device_event_inquire)&network_event_log_polling_func;
            }
        }
        {
            net_device_info *pst_net_device_info = 0;
            if(TRUE == net_get_device_info(dwa_device_id_kinergy_ii[cw_device_id_index_meter_1],
                                           (net_device_info **)&pst_net_device_info,
                                           pst_net_system_info_list))
            {
                pst_net_device_info->pst_net_device_func.net_device_data_polling = (pfunc_net_device_event_inquire)&device_data_polling_common;
            }
        }
	}
}

void network_communication_initial()
{
    //system info initial
    {
        net_system_info_ex_func_initial(NULL,
                                        NULL,
                                        &st_net_system_info_list);
    }
    //iot_module_data_channel_initial  TODO 4G发送接收函数在这俩赋值
    {
        network_send_data_channel px_send_data;
        network_receive_data_channel px_receive_data;
        if(TRUE == iot_module_data_transmission_channel_get((network_send_data_channel *)(&px_send_data),
                                                            (network_receive_data_channel *)(&px_receive_data)))
        {
            network_data_channel_initial(px_send_data,px_receive_data);
        }
        else
        {

        }
    }
    //wireless_module_transfer_packet_size
    {
        iot_module_transfer_packet_max_size_get(&w_net_iot_module_mps);		//最大2000
    }
    //device_data_access_init
    {
        net_device_data_access_initial();
    }
    //device_tree_refresh_initial
    {
        device_event_log_init();
        net_device_tree_refresh_Semaphore_creat();
        net_device_exists_state_Semaphore_creat();
    }
    //network_com_queue_init
    {
        network_com_queue_init();
    }
}

void network_communication()
{
#ifdef KINERGY_II_WIFI
    network_com_with_tbb_device_server();
#else
	network_4g_com_with_tbb_device_server();
#endif
}
//run time:20ms
void network_infomation_refresh()
{
	{
		static uint16_t w_firmware_detect_timer = 0;

		w_firmware_detect_timer++;
		if(w_firmware_detect_timer > 50)
		{
			w_firmware_detect_timer = 0;
			local_firmware_update_loop();
		}
	}
}

void network_device_tree_refresh()
{
	st_net_system_info_list.st_net_system_func.net_inquire_device_tree(&st_net_system_info_list);			//net_inquire_device_tree
}

void network_device_data_polling()
{
	st_net_system_info_list.st_net_system_func.net_device_data_polling(&st_net_system_info_list);			//net_device_data_polling
}
