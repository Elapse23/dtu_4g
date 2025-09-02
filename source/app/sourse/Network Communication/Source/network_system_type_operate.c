#include "network_system_type_operate.h"
#include "net_device_info.h"
#include "application_general_macro.h"
#include "net_system_info_access.h"
#include "net_device_tree_build.h"
#include "net_system_info_list.h"
#include "net_device_data_access.h"
#include "net_device_data_mppt.h"
#include "net_device_data_ck_inverter.h"
#include "net_device_data_pv.h"
#include "network_energy_statistics.h"
#include "printf_custom.h"
#include "network_eventlog_info_operate.h"
#include "product_test.h"

typedef struct
{
    uint8_t f_system_init;
    uint8_t f_network_init;
    uint8_t f_device_tree_init;
    uint8_t b_device_com_init;
    uint32_t dw_net_system_type;
    uint8_t b_system_protocal_type;
}system_info;
static system_info st_system_info =
{
    .f_system_init = FALSE,
    .f_network_init = FALSE,
    .dw_net_system_type = cdw_net_system_type_ck_4g,		//系统类型 
    .b_system_protocal_type = cb_system_protocal_type_ck,
};

void system_data_init(uint8_t b_protocal_type_type)
{
    if(cb_system_protocal_type_riiosunii == b_protocal_type_type)
    {   //riiosunii type
        net_device_info *pst_net_device_info = 0;
        uint8_t b_cnt_i = 0;

        for(b_cnt_i = cw_device_id_index_ck_inverter_u1;
            b_cnt_i <= cw_device_id_index_ck_inverter_w3;
            b_cnt_i++)
        {
            if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[b_cnt_i],
                                           (net_device_info **)&pst_net_device_info,
                                           &st_net_system_info_list))
            {
                continue;
            }
            pst_net_device_info->pst_net_data_list_info =\
                (const net_data_list_info *)pb_net_data_list_info_ck_inveter_riiosunii[b_cnt_i - cw_device_id_index_ck_inverter_u1];
        }
        for(b_cnt_i = cw_device_id_index_mppt_u1_0;
            b_cnt_i <= cw_device_id_index_mppt_w3_5;
            b_cnt_i++)
        {
            if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[b_cnt_i],
                                           (net_device_info **)&pst_net_device_info,
                                           &st_net_system_info_list))
            {
                continue;
            }
            pst_net_device_info->pst_net_data_list_info =\
                (const net_data_list_info *)pb_net_data_list_info_riiosunii_mppt[b_cnt_i - cw_device_id_index_mppt_u1_0];
        }
        for(b_cnt_i = cw_device_id_index_pv_1;
            b_cnt_i <= cw_device_id_index_pv_2;
            b_cnt_i++)
        {
            if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[b_cnt_i],
                                           (net_device_info **)&pst_net_device_info,
                                           &st_net_system_info_list))
            {
                continue;
            }
            pst_net_device_info->pst_net_data_list_info =\
                (const net_data_list_info *)pb_net_data_list_info_riiosunii_pv[b_cnt_i - cw_device_id_index_pv_1];
        }

        #if( system_init_status_print )
        DebugPrintf(TRUE,"Energy Statistics Type: RIIOSUN II\n");
        #endif

        network_data_process_func_set(data_process_type_riiosunii);
    }
    else
    {   //ck type
        net_device_info *pst_net_device_info = 0;
        uint8_t b_cnt_i = 0;

        for(b_cnt_i = cw_device_id_index_ck_inverter_u1;
            b_cnt_i <= cw_device_id_index_ck_inverter_w3;
            b_cnt_i++)
        {
            if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[b_cnt_i],
                                           (net_device_info **)&pst_net_device_info,
                                           &st_net_system_info_list))
            {
                continue;
            }
            pst_net_device_info->pst_net_data_list_info =\
                (const net_data_list_info *)pb_net_data_list_info_ck_inveter[b_cnt_i - cw_device_id_index_ck_inverter_u1];
        }
        for(b_cnt_i = cw_device_id_index_mppt_u1_0;
            b_cnt_i <= cw_device_id_index_mppt_w3_5;
            b_cnt_i++)
        {
            if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[b_cnt_i],
                                           (net_device_info **)&pst_net_device_info,
                                           &st_net_system_info_list))
            {
                continue;
            }
            pst_net_device_info->pst_net_data_list_info =\
                (const net_data_list_info *)pb_net_data_list_info_mppt[b_cnt_i - cw_device_id_index_mppt_u1_0];
        }
        for(b_cnt_i = cw_device_id_index_pv_1;
            b_cnt_i <= cw_device_id_index_pv_2;
            b_cnt_i++)
        {
            if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[b_cnt_i],
                                           (net_device_info **)&pst_net_device_info,
                                           &st_net_system_info_list))
            {
                continue;
            }
            pst_net_device_info->pst_net_data_list_info =\
                (const net_data_list_info *)pb_net_data_list_info_ck_pv[b_cnt_i - cw_device_id_index_pv_1];
        }
        #if( system_init_status_print )
        DebugPrintf(TRUE,"Energy Statistics Type: CK\n");
        #endif

        network_data_process_func_set(data_process_type_ck);
    }
}

//只要有一台逆变器有通信返回True
uint8_t get_system_communicate_status()
{
	static uint8_t b_communicate_status = 0;
	uint8_t b_communicate_status_back_up = 0;
	net_device_info *pst_net_device_info = 0;
	for(uint8_t b_cnt_i = 0; b_cnt_i < 9; b_cnt_i ++)
	{
        if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1 + b_cnt_i],
                                       (net_device_info **)&pst_net_device_info,
                                       &st_net_system_info_list))
        {
            return FALSE;
        }
		b_communicate_status = net_check_device_communicate_status(cb_net_device_add,pst_net_device_info);
		b_communicate_status = b_communicate_status_back_up | b_communicate_status;
		b_communicate_status_back_up = b_communicate_status;
	}
	return b_communicate_status;

}
uint8_t network_system_init()
{
    uint8_t b_result = FALSE;

    if((st_system_info.b_device_com_init && st_system_info.f_device_tree_init) ||\
       (TRUE == check_product_test()))
    {
        net_device_info *pst_net_device_info = 0;

        if(TRUE == get_system_communicate_status())
        {
            uint16_t w_data_len = 2;
            uint16_t w_system_mode = 0;
//            net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
//                                   cb_device_data_access_data_type_set,
//                                   0x0004,
//                                   &w_data_len,
//                                   (uint8_t *)&w_system_mode,
//                                   cb_device_data_access_operate_type_read);
			uint8_t b_iot_module_type = iot_module_type_ble;
			iot_module_get_type(&b_iot_module_type);
			w_system_mode = get_system_type();
            if((cb_inv_type_riio_sun_ii == w_system_mode) 
			   ||(cb_inv_type_riio_sun_ii_gte == w_system_mode)
               || (cb_inv_type_ck_ii == w_system_mode) 
               || (cb_inv_type_tyrann == w_system_mode) 
               || (cb_inv_type_riio_sun_ii_split_phase == w_system_mode) 
               || (cb_inv_type_apollo_matric_ii_10_15k == w_system_mode) 
               || (cb_inv_type_apollo_matric_ii_lite == w_system_mode)
               || (cb_inv_type_apollo_matric_ii_5_8k == w_system_mode) 
               || (cb_inv_type_Tyrann_3_5_8k == w_system_mode)
               || (cb_inv_type_riio_ii_split_phase == w_system_mode)
               || (cb_inv_type_riio_ii == w_system_mode)
               )
            {

            	if(b_iot_module_type == iot_module_type_lte)
            	{
	                st_system_info.b_system_protocal_type = cb_system_protocal_type_riiosunii;
	                st_system_info.dw_net_system_type = cdw_net_system_type_riio_wifi;//mark check_wireless_module_type
				}
				else if(b_iot_module_type == iot_module_type_4g)
				{
	                st_system_info.b_system_protocal_type = cb_system_protocal_type_riiosunii;
	                st_system_info.dw_net_system_type = cdw_net_system_type_riio_4g;//mark check_wireless_module_type
				}

            }
            else
            {
                if(b_iot_module_type == iot_module_type_lte)
            	{
	                st_system_info.b_system_protocal_type = cb_system_protocal_type_ck;
	                st_system_info.dw_net_system_type = cdw_net_system_type_ck_4g;//mark check_wireless_module_type
				}
				else if(b_iot_module_type == iot_module_type_4g)
				{
	                st_system_info.b_system_protocal_type = cb_system_protocal_type_ck;
	                st_system_info.dw_net_system_type = cdw_net_system_type_ck_4g;//mark check_wireless_module_type
				}
            }

            #if( system_init_status_print )
            DebugPrintf(TRUE,"Inverter Type:%d\n",w_system_mode);
            #endif

            system_data_init(st_system_info.b_system_protocal_type);
            device_id_init();
            b_result = TRUE;
        }

        if(!st_system_info.f_network_init)
        {
            st_system_info.f_network_init = TRUE;

            #if( system_init_status_print )
            DebugPrintf(TRUE,"System init complete\n");
            #endif
        }
    }
    return b_result;
}

uint8_t get_net_system_type(uint32_t *pdw_system_type_in)
{
    if(FALSE == st_system_info.f_system_init)
    {
        return FALSE;
    }
    *pdw_system_type_in = st_system_info.dw_net_system_type;
    return TRUE;
}

uint8_t check_system_protocal_type(uint8_t *pb_data_in)
{
    *pb_data_in = st_system_info.b_system_protocal_type;
    if(st_system_info.f_system_init)
    {
        return TRUE;
    }
    return FALSE;
}

uint8_t check_system_init(uint8_t b_init_type)
{
    switch(b_init_type)
    {
        case cb_system_init_type_device_tree:
            return st_system_info.f_device_tree_init;
        case cb_system_init_type_device_com:
            return st_system_info.b_device_com_init;
        case cb_system_init_type_network:
            return st_system_info.f_network_init;
        default:
            return st_system_info.f_system_init;
    }
}

void system_init_comfirm(uint8_t b_comfirm_type)
{
    if(st_system_info.f_system_init)
    {
        return;
    }

    if(cb_system_init_type_device_tree == b_comfirm_type)
    {
        st_system_info.f_device_tree_init = TRUE;
        #if( system_init_status_print )
        DebugPrintf(TRUE,"System init device tree complete\n");
        #endif
    }
    else if(cb_system_init_type_device_com == b_comfirm_type)
    {
        if(st_system_info.f_device_tree_init)
        {
            if(st_system_info.f_network_init)
            {
                st_system_info.b_device_com_init = 2;
            }
            else
            {
                st_system_info.b_device_com_init = 1;
            }
            #if( system_init_status_print )
            DebugPrintf(TRUE,"System init com complete:%d\n",st_system_info.b_device_com_init);
            #endif
        }
    }
    else if(cb_system_init_type_system == b_comfirm_type)
    {
        st_system_info.f_system_init = TRUE;
        fill_event_log(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                       event_log_type_wcm,
                       "WCM init complete",
                       17,
                       cw_wcm_event_code_wcm_init,
                       0);
    }
}

/**
 * @brief 判断当前设备类型
 * @param {uint32_t} dw_device_id 设备id
 * @param {inv_type} enum_device_type 设备类型
 * @return {uint8_t} 返回值
 * @author Yao YiLiang
 */
uint8_t model_identification(uint32_t dw_device_id, inv_type enum_device_type)
{
	uint16_t             w_system_type       = cb_inv_type_ck;
	uint16_t             w_data_len          = 2;

	net_device_data_access(((dw_device_id & 0x0000FF) | cdw_device_type_cklcd),
	                       cb_device_data_access_data_type_set,
	                       0x0004,
	                       &w_data_len,
	                       ( uint8_t * )&w_system_type,
	                       cb_device_data_access_operate_type_read);

	if (w_data_len != 0 && w_system_type == enum_device_type)
	{
		return TRUE;
	}
	return FALSE;
}