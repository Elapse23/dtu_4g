#include "net_device_id_kinergy_ii.h"
#include "net_device_tree_kinergy_ii.h"
#include "net_device_tree_kinergy.h"
#include "net_device_tree_system.h"
#include "net_device_tree_cklcd.h"
#include "net_device_tree_ck_inverter.h"
#include "net_device_tree_mppt.h"
#include "net_device_tree_pv.h"
#include "net_device_tree_meter.h"
#include "net_system_info_list.h"
#include "network_energy_statistics.h"
#include "net_system_info_access.h"
#include "printf_custom.h"
#include "net_device_data_cklcd.h"
#include "network_local_firmware_update.h"
#include "net_device_data_access.h"


uint16_t w_device_id_ex_mppt[54] =
{
    cw_device_id_mppt_u1_0,    //mppt u1
    cw_device_id_mppt_u1_1,
    cw_device_id_mppt_u1_2,
    cw_device_id_mppt_u1_3,
    cw_device_id_mppt_u1_4,
    cw_device_id_mppt_u1_5,
    cw_device_id_mppt_u2_0,    //mppt u2
    cw_device_id_mppt_u2_1,
    cw_device_id_mppt_u2_2,
    cw_device_id_mppt_u2_3,
    cw_device_id_mppt_u2_4,
    cw_device_id_mppt_u2_5,
    cw_device_id_mppt_u3_0,    //mppt u3
    cw_device_id_mppt_u3_1,
    cw_device_id_mppt_u3_2,
    cw_device_id_mppt_u3_3,
    cw_device_id_mppt_u3_4,
    cw_device_id_mppt_u3_5,
    cw_device_id_mppt_v1_0,    //mppt v1
    cw_device_id_mppt_v1_1,
    cw_device_id_mppt_v1_2,
    cw_device_id_mppt_v1_3,
    cw_device_id_mppt_v1_4,
    cw_device_id_mppt_v1_5,
    cw_device_id_mppt_v2_0,    //mppt v2
    cw_device_id_mppt_v2_1,
    cw_device_id_mppt_v2_2,
    cw_device_id_mppt_v2_3,
    cw_device_id_mppt_v2_4,
    cw_device_id_mppt_v2_5,
    cw_device_id_mppt_v3_0,    //mppt v3
    cw_device_id_mppt_v3_1,
    cw_device_id_mppt_v3_2,
    cw_device_id_mppt_v3_3,
    cw_device_id_mppt_v3_4,
    cw_device_id_mppt_v3_5,
    cw_device_id_mppt_w1_0,    //mppt w1
    cw_device_id_mppt_w1_1,
    cw_device_id_mppt_w1_2,
    cw_device_id_mppt_w1_3,
    cw_device_id_mppt_w1_4,
    cw_device_id_mppt_w1_5,
    cw_device_id_mppt_w2_0,    //mppt w2
    cw_device_id_mppt_w2_1,
    cw_device_id_mppt_w2_2,
    cw_device_id_mppt_w2_3,
    cw_device_id_mppt_w2_4,
    cw_device_id_mppt_w2_5,
    cw_device_id_mppt_w3_0,    //mppt w3
    cw_device_id_mppt_w3_1,
    cw_device_id_mppt_w3_2,
    cw_device_id_mppt_w3_3,
    cw_device_id_mppt_w3_4,
    cw_device_id_mppt_w3_5,
};

uint32_t dwa_device_id_kinergy_ii[cw_device_id_index_end] =
{
    cdw_device_id_system,       //system
    cdw_device_id_kinergy_ii,   //kinergy_ii
    cdw_device_id_kinergy,      //kinergy
    cdw_device_id_cklcd_u1,     //cklcd
    cdw_device_id_cklcd_u2,
    cdw_device_id_cklcd_u3,
    cdw_device_id_cklcd_v1,
    cdw_device_id_cklcd_v2,
    cdw_device_id_cklcd_v3,
    cdw_device_id_cklcd_w1,
    cdw_device_id_cklcd_w2,
    cdw_device_id_cklcd_w3,
    cdw_device_id_ck_inverter_u1,   //ck inverter
    cdw_device_id_ck_inverter_u2,
    cdw_device_id_ck_inverter_u3,
    cdw_device_id_ck_inverter_v1,
    cdw_device_id_ck_inverter_v2,
    cdw_device_id_ck_inverter_v3,
    cdw_device_id_ck_inverter_w1,
    cdw_device_id_ck_inverter_w2,
    cdw_device_id_ck_inverter_w3,
    cdw_device_id_mppt_u1_0,    //mppt u1
    cdw_device_id_mppt_u1_1,
    cdw_device_id_mppt_u1_2,
    cdw_device_id_mppt_u1_3,
    cdw_device_id_mppt_u1_4,
    cdw_device_id_mppt_u1_5,
    cdw_device_id_mppt_u2_0,    //mppt u2
    cdw_device_id_mppt_u2_1,
    cdw_device_id_mppt_u2_2,
    cdw_device_id_mppt_u2_3,
    cdw_device_id_mppt_u2_4,
    cdw_device_id_mppt_u2_5,
    cdw_device_id_mppt_u3_0,    //mppt u3
    cdw_device_id_mppt_u3_1,
    cdw_device_id_mppt_u3_2,
    cdw_device_id_mppt_u3_3,
    cdw_device_id_mppt_u3_4,
    cdw_device_id_mppt_u3_5,
    cdw_device_id_mppt_v1_0,    //mppt v1
    cdw_device_id_mppt_v1_1,
    cdw_device_id_mppt_v1_2,
    cdw_device_id_mppt_v1_3,
    cdw_device_id_mppt_v1_4,
    cdw_device_id_mppt_v1_5,
    cdw_device_id_mppt_v2_0,    //mppt v2
    cdw_device_id_mppt_v2_1,
    cdw_device_id_mppt_v2_2,
    cdw_device_id_mppt_v2_3,
    cdw_device_id_mppt_v2_4,
    cdw_device_id_mppt_v2_5,
    cdw_device_id_mppt_v3_0,    //mppt v3
    cdw_device_id_mppt_v3_1,
    cdw_device_id_mppt_v3_2,
    cdw_device_id_mppt_v3_3,
    cdw_device_id_mppt_v3_4,
    cdw_device_id_mppt_v3_5,
    cdw_device_id_mppt_w1_0,    //mppt w1
    cdw_device_id_mppt_w1_1,
    cdw_device_id_mppt_w1_2,
    cdw_device_id_mppt_w1_3,
    cdw_device_id_mppt_w1_4,
    cdw_device_id_mppt_w1_5,
    cdw_device_id_mppt_w2_0,    //mppt w2
    cdw_device_id_mppt_w2_1,
    cdw_device_id_mppt_w2_2,
    cdw_device_id_mppt_w2_3,
    cdw_device_id_mppt_w2_4,
    cdw_device_id_mppt_w2_5,
    cdw_device_id_mppt_w3_0,    //mppt w3
    cdw_device_id_mppt_w3_1,
    cdw_device_id_mppt_w3_2,
    cdw_device_id_mppt_w3_3,
    cdw_device_id_mppt_w3_4,
    cdw_device_id_mppt_w3_5,
    cdw_device_id_pv_1,
    cdw_device_id_pv_2,
    cdw_device_id_meter_1,
};



void device_id_init()
{
	#define cb_subset_device_sn_len                     30
    uint32_t dw_mppt_outer = cdw_mppt_id_type_outer;
    net_device_tree *pst_net_device_tree = 0;
//	uint8_t b_inv_serial_number[cb_subset_device_sn_len];		//Ô¤Áômppt snºÅ
//	uint16_t w_data_len = 30;
	if(check_system_type(cb_inv_type_apollo_matric_ii_10_15k))
	{
		for(uint16_t w_cnt_i = cw_device_id_index_mppt_u1_0;
			w_cnt_i <= cw_device_id_index_mppt_w3_5;
			w_cnt_i++)
		{
			if(0 == ((w_cnt_i - cw_device_id_index_mppt_u1_0) % 6) || 1 == ((w_cnt_i - cw_device_id_index_mppt_u1_0) % 6))
			{
//				  dwa_device_id_kinergy_ii[w_cnt_i] &= ~dw_mppt_outer;
				dwa_device_id_kinergy_ii[w_cnt_i] =\
					cdw_outer_mppt_device_id[(w_cnt_i - cw_device_id_index_mppt_u1_0)] & ~dw_mppt_outer;
//				net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],		//Ô¤Áômppt snºÅ
//									   cb_net_data_att_set,
//									   0x0080,
//									   &w_data_len,
//									   (uint8_t *)&b_inv_serial_number,
//									   cb_device_data_access_operate_type_read);
				if(TRUE == net_get_device_tree_info(dwa_device_id_kinergy_ii[w_cnt_i],
													(net_device_tree **)(&pst_net_device_tree),
													&st_net_system_info_list))

				{
					pst_net_device_tree->st_device_tree.b_device_id[2] = (dwa_device_id_kinergy_ii[w_cnt_i] & 0x000000FF) >> 0;
					pst_net_device_tree->st_device_tree.b_device_id[1] = (dwa_device_id_kinergy_ii[w_cnt_i] & 0x0000FF00) >> 8;
					pst_net_device_tree->st_device_tree.b_device_id[0] = (dwa_device_id_kinergy_ii[w_cnt_i] & 0x00FF0000) >> 16;
//					for(uint8_t b_cnt_i = 0; b_cnt_i < cb_subset_device_sn_len; b_cnt_i ++)
//					{
//						pst_net_device_tree->st_device_tree.b_serial_number[b_cnt_i] = b_inv_serial_number[b_cnt_i];
//					}
				}
			}
			else
			{
				dwa_device_id_kinergy_ii[w_cnt_i] =\
					cdw_outer_mppt_device_id[(w_cnt_i - cw_device_id_index_mppt_u1_0) - 2];
//				net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
//									   cb_net_data_att_set,
//									   0x0080,
//									   &w_data_len,
//									   (uint8_t *)&b_inv_serial_number,
//									   cb_device_data_access_operate_type_read);
				if(TRUE == net_get_device_tree_info(dwa_device_id_kinergy_ii[w_cnt_i],
													(net_device_tree **)(&pst_net_device_tree),
													&st_net_system_info_list))

				{
					pst_net_device_tree->st_device_tree.b_device_id[2] = (dwa_device_id_kinergy_ii[w_cnt_i] & 0x000000FF) >> 0;
					pst_net_device_tree->st_device_tree.b_device_id[1] = (dwa_device_id_kinergy_ii[w_cnt_i] & 0x0000FF00) >> 8;
					pst_net_device_tree->st_device_tree.b_device_id[0] = (dwa_device_id_kinergy_ii[w_cnt_i] & 0x00FF0000) >> 16;
//					for(uint8_t b_cnt_i = 0; b_cnt_i < cb_subset_device_sn_len; b_cnt_i++)
//					{
//						pst_net_device_tree->st_device_tree.b_serial_number[b_cnt_i] = b_inv_serial_number[b_cnt_i];
//					}
				}
			}
		}
		DebugPrintf(TRUE,"Mppt Type:1 Inner\n");
	}
    else if(check_all_in_one(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1]))
    {
        for(uint16_t w_cnt_i = cw_device_id_index_mppt_u1_0;
            w_cnt_i <= cw_device_id_index_mppt_w3_5;
            w_cnt_i++)
        {
            if(0 == ((w_cnt_i - cw_device_id_index_mppt_u1_0) % 6))
            {
//                dwa_device_id_kinergy_ii[w_cnt_i] &= ~dw_mppt_outer;
                dwa_device_id_kinergy_ii[w_cnt_i] =\
                    cdw_outer_mppt_device_id[(w_cnt_i - cw_device_id_index_mppt_u1_0)] & ~dw_mppt_outer;
//				net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
//									   cb_net_data_att_set,
//									   0x0080,
//									   &w_data_len,
//									   (uint8_t *)&b_inv_serial_number,
//									   cb_device_data_access_operate_type_read);
                if(TRUE == net_get_device_tree_info(dwa_device_id_kinergy_ii[w_cnt_i],
                                                    (net_device_tree **)(&pst_net_device_tree),
                                                    &st_net_system_info_list))

                {
                    pst_net_device_tree->st_device_tree.b_device_id[2] = (dwa_device_id_kinergy_ii[w_cnt_i] & 0x000000FF) >> 0;
                    pst_net_device_tree->st_device_tree.b_device_id[1] = (dwa_device_id_kinergy_ii[w_cnt_i] & 0x0000FF00) >> 8;
                    pst_net_device_tree->st_device_tree.b_device_id[0] = (dwa_device_id_kinergy_ii[w_cnt_i] & 0x00FF0000) >> 16;
//					for(uint8_t b_cnt_i = 0; b_cnt_i < cb_subset_device_sn_len; b_cnt_i ++)
//					{
//						pst_net_device_tree->st_device_tree.b_serial_number[b_cnt_i] = b_inv_serial_number[b_cnt_i];
//					}
                }
            }
            else
            {
                dwa_device_id_kinergy_ii[w_cnt_i] =\
                    cdw_outer_mppt_device_id[(w_cnt_i - cw_device_id_index_mppt_u1_0) - 1];
//				net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
//									   cb_net_data_att_set,
//									   0x0080,
//									   &w_data_len,
//									   (uint8_t *)&b_inv_serial_number,
//									   cb_device_data_access_operate_type_read);
                if(TRUE == net_get_device_tree_info(dwa_device_id_kinergy_ii[w_cnt_i],
                                                    (net_device_tree **)(&pst_net_device_tree),
                                                    &st_net_system_info_list))

                {
                    pst_net_device_tree->st_device_tree.b_device_id[2] = (dwa_device_id_kinergy_ii[w_cnt_i] & 0x000000FF) >> 0;
                    pst_net_device_tree->st_device_tree.b_device_id[1] = (dwa_device_id_kinergy_ii[w_cnt_i] & 0x0000FF00) >> 8;
                    pst_net_device_tree->st_device_tree.b_device_id[0] = (dwa_device_id_kinergy_ii[w_cnt_i] & 0x00FF0000) >> 16;
//					for(uint8_t b_cnt_i = 0; b_cnt_i < cb_subset_device_sn_len; b_cnt_i++)
//					{
//						pst_net_device_tree->st_device_tree.b_serial_number[b_cnt_i] = b_inv_serial_number[b_cnt_i];
//					}
                }
            }
        }
        DebugPrintf(TRUE,"Mppt Type:1 Inner\n");
    }
    else
    {
        for(uint16_t w_cnt_i = cw_device_id_index_mppt_u1_0;
            w_cnt_i <= cw_device_id_index_mppt_w3_5;
            w_cnt_i++)
        {
            if(FALSE == check_system_type(cb_inv_type_ck) && FALSE == check_system_type(cb_inv_type_ck_ii_hdv2))
            {
                dwa_device_id_kinergy_ii[w_cnt_i] =\
                    cdw_outer_mppt_device_id[(w_cnt_i - cw_device_id_index_mppt_u1_0)];
//				net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
//									   cb_net_data_att_set,
//									   0x0080,
//									   &w_data_len,
//									   (uint8_t *)&b_inv_serial_number,
//									   cb_device_data_access_operate_type_read);
                if(TRUE == net_get_device_tree_info(dwa_device_id_kinergy_ii[w_cnt_i],
                                                    (net_device_tree **)(&pst_net_device_tree),
                                                    &st_net_system_info_list))

                {
                    pst_net_device_tree->st_device_tree.b_device_id[2] = (dwa_device_id_kinergy_ii[w_cnt_i] & 0x000000FF) >> 0;
                    pst_net_device_tree->st_device_tree.b_device_id[1] = (dwa_device_id_kinergy_ii[w_cnt_i] & 0x0000FF00) >> 8;
                    pst_net_device_tree->st_device_tree.b_device_id[0] = (dwa_device_id_kinergy_ii[w_cnt_i] & 0x00FF0000) >> 16;
//					for(uint8_t b_cnt_i = 0; b_cnt_i < cb_subset_device_sn_len; b_cnt_i++)
//					{
//						pst_net_device_tree->st_device_tree.b_serial_number[b_cnt_i] = b_inv_serial_number[b_cnt_i];
//					}
                }
            }
            else
            {
                dwa_device_id_kinergy_ii[w_cnt_i] =\
                    (cdw_device_type_mppt | (w_cnt_i - cw_device_id_index_mppt_u1_0));
//				net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
//									   cb_net_data_att_set,
//									   0x0080,
//									   &w_data_len,
//									   (uint8_t *)&b_inv_serial_number,
//									   cb_device_data_access_operate_type_read);
                if(TRUE == net_get_device_tree_info(dwa_device_id_kinergy_ii[w_cnt_i],
                                                    (net_device_tree **)(&pst_net_device_tree),
                                                    &st_net_system_info_list))

                {
                    pst_net_device_tree->st_device_tree.b_device_id[2] = (dwa_device_id_kinergy_ii[w_cnt_i] & 0x000000FF) >> 0;
                    pst_net_device_tree->st_device_tree.b_device_id[1] = (dwa_device_id_kinergy_ii[w_cnt_i] & 0x0000FF00) >> 8;
                    pst_net_device_tree->st_device_tree.b_device_id[0] = (dwa_device_id_kinergy_ii[w_cnt_i] & 0x00FF0000) >> 16;
//					for(uint8_t b_cnt_i = 0; b_cnt_i < cb_subset_device_sn_len; b_cnt_i++)
//					{
//						pst_net_device_tree->st_device_tree.b_serial_number[b_cnt_i] = b_inv_serial_number[b_cnt_i];
//					}
                }
            }
        }
        DebugPrintf(TRUE,"Mppt Type:All Outer\n");
    }
}

