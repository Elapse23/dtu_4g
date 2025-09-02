#include "network_device_config_sycn.h"
#include "application_general_macro.h"
#include "net_device_tree.h"
#include "net_system_info_list.h"
#include "net_system_info_access.h"
#include "portmacro.h"
#include "freertos_user_config.h"
#include "net_device_data_access.h"
#include "application_general_func.h"
#include "net_device_tree_cklcd.h"
#include "net_device_tree_ck_inverter.h"
#include "printf_custom.h"
#include "network_energy_statistics.h"
#include "network_system_type_operate.h"
#include "semphr.h"
#include "network_local_firmware_update.h"
#include "net_device_id_kinergy_ii.h"
#include "net_device_set_attribute_ck_inverter.h"

#define cb_max_sync_data_len                4
#define ba_auxiliary_installation_data_len  30
#define cb_device_totoal_sync_list_num      1

#define cb_max_sync_trig_info           10
#define cb_cklcd_sync_list_num          9
typedef struct
{
    uint32_t dw_device_type;
    uint16_t w_reg_addr;
    uint16_t w_reg_value;
}device_config_sync_reg_info;

typedef struct
{
    uint8_t b_reg_num;
    device_config_sync_reg_info sta_device_config_sync_reg_info[cb_max_sync_trig_info];
}device_config_sync_reg_list;

typedef struct
{
    device_config_sync_reg_list st_master_reg_list;
    device_config_sync_reg_list st_sub_reg_list;
}device_config_sync_trig_list;
typedef struct
{
    uint32_t dw_device_id;
    uint16_t w_device_type;
    uint16_t w_sync_addr;
	uint16_t w_device_id;
    uint8_t b_device_com_id;
    uint8_t b_sync_type;
    uint8_t b_sync_reg_num;
    uint8_t b_sync_status;
    uint8_t b_config_type;
    uint8_t b_data_len;
    uint8_t ba_data[cb_max_sync_data_len + 2];
	uint8_t ba_auxiliary_installation_data[ba_auxiliary_installation_data_len + 2];
    const device_config_sync_trig_list *pst_device_sync_list;
    uint8_t b_sync_reg_num_total;
    uint8_t b_process;
    uint8_t f_continue;
    uint8_t f_sync_fresh;
}device_config_sync_info;

typedef struct
{
    uint32_t dw_device_id;
    uint16_t w_sync_addr;
    uint8_t b_process;
    uint8_t b_sync_status;
}device_config_sync_status;

uint8_t check_set_data_value(uint32_t dw_device_id,
                             uint16_t w_set_addr,
                             uint32_t dw_set_value,
                             uint32_t *pdw_set_value_out);//mark -> range manage.c



device_config_sync_trig_list st_cklcd_sync_list[cb_cklcd_sync_list_num] =
{
    {//0
        {
            1,
            {
                {cdw_device_type_cklcd,0x0050,0},
            }
        },
        {
            4,
            {
                {cdw_device_type_ck_inverter,0x0040,14100},//PARAM_BAT_U_AVE_CHG
                {cdw_device_type_ck_inverter,0x0041,13700},//PARAM_BAT_U_FLOAT_CHG
                {cdw_device_type_ck_inverter,0x004F,0},//PARAM_BAT_LITHIUM_BMS_SET
                {cdw_device_type_ck_inverter,0x0046,15},//PARAM_BAT_CHG_MAX_CUR
            }
        },
    },
    {//1
        {
            1,
            {
                {cdw_device_type_cklcd,0x0050,1},
            }
        },
        {
            4,
            {
                {cdw_device_type_ck_inverter,0x0040,14400},//PARAM_BAT_U_AVE_CHG
                {cdw_device_type_ck_inverter,0x0041,13500},//PARAM_BAT_U_FLOAT_CHG
                {cdw_device_type_ck_inverter,0x004F,0},//PARAM_BAT_LITHIUM_BMS_SET
                {cdw_device_type_ck_inverter,0x0046,15},//PARAM_BAT_CHG_MAX_CUR
            }
        },
    },
    {//2
        {
            1,
            {
                {cdw_device_type_cklcd,0x0050,2},
            }
        },
        {
            4,
            {
                {cdw_device_type_ck_inverter,0x0040,14100},//PARAM_BAT_U_AVE_CHG
                {cdw_device_type_ck_inverter,0x0041,13500},//PARAM_BAT_U_FLOAT_CHG
                {cdw_device_type_ck_inverter,0x004F,0},//PARAM_BAT_LITHIUM_BMS_SET
                {cdw_device_type_ck_inverter,0x0046,20},//PARAM_BAT_CHG_MAX_CUR
            }
        },
    },
    {//3
        {
            1,
            {
                {cdw_device_type_cklcd,0x0050,3},
            }
        },
        {
            4,
            {
                {cdw_device_type_ck_inverter,0x0040,14700},//PARAM_BAT_U_AVE_CHG
                {cdw_device_type_ck_inverter,0x0041,13500},//PARAM_BAT_U_FLOAT_CHG
                {cdw_device_type_ck_inverter,0x004F,0},//PARAM_BAT_LITHIUM_BMS_SET
                {cdw_device_type_ck_inverter,0x0046,15},//PARAM_BAT_CHG_MAX_CUR
            }
        },
    },
    {//4
        {
            1,
            {
                {cdw_device_type_cklcd,0x0050,4},
            }
        },
        {
            4,
            {
                {cdw_device_type_ck_inverter,0x0040,15200},//PARAM_BAT_U_AVE_CHG
                {cdw_device_type_ck_inverter,0x0041,13500},//PARAM_BAT_U_FLOAT_CHG
                {cdw_device_type_ck_inverter,0x004F,0},//PARAM_BAT_LITHIUM_BMS_SET
                {cdw_device_type_ck_inverter,0x0046,25},//PARAM_BAT_CHG_MAX_CUR
            }
        },
    },
    {//5
        {
            1,
            {
                {cdw_device_type_cklcd,0x0050,5},
            }
        },
        {
            4,
            {
                {cdw_device_type_ck_inverter,0x0040,14200},//PARAM_BAT_U_AVE_CHG
                {cdw_device_type_ck_inverter,0x0041,14000},//PARAM_BAT_U_FLOAT_CHG
                {cdw_device_type_ck_inverter,0x004F,0},//PARAM_BAT_LITHIUM_BMS_SET
                {cdw_device_type_ck_inverter,0x0046,30},//PARAM_BAT_CHG_MAX_CUR
            }
        },
    },
    {//6
        {
            1,
            {
                {cdw_device_type_cklcd,0x0050,6},
            }
        },
        {
            1,
            {
                {cdw_device_type_ck_inverter,0x004F,2},//PARAM_BAT_LITHIUM_BMS_SET
            }
        },
    },
    {//7
        {
            1,
            {
                {cdw_device_type_ck_inverter,0x005C,0},//acin sourse
            }
        },
        {
            1,
            {
                {cdw_device_type_ck_inverter,0x0054,0},//PARAM_GCI_HARMONIC_ADAPTATION
            }
        },
    },
    {//8
        {
            1,
            {
                {cdw_device_type_ck_inverter,0x005C,1},//acin sourse
            }
        },
        {
            1,
            {
                {cdw_device_type_ck_inverter,0x0054,1},//PARAM_GCI_HARMONIC_ADAPTATION
            }
        },
    },

};

typedef enum
{
    sync_no_start = 0,
    synchronizing,
    sync_complete,
    sync_no_response_fail,  // 设备未响应
    sync_impermission_fail, // 设备不允许设置
    sync_device_loss_fail, //  设备失联 目前未用
    sync_device_inexist_fail, // 设备不存在 目前未用
    sync_continue, 
    sync_wait,
}sync_status;

uint8_t network_device_config_sycn_trig_single(uint32_t w_device_type,
                                               uint8_t b_config_type,
                                               uint16_t w_config_addr,
                                               uint8_t b_data_len,
                                               uint8_t *pb_data_in,
                                               uint8_t b_sync_reg_num,
                                               uint8_t f_sync_fresh,
                                               uint8_t b_sync_type);
uint8_t network_device_config_sycn_trig_list(uint16_t w_device_type,
                                             const device_config_sync_trig_list *pst_device_config_sync_trig_list,
                                             uint8_t f_sync_fresh);


static device_config_sync_info st_device_config_sync_info =
{
    .b_sync_type = sync_type_none,
};
static device_config_sync_status st_device_config_sync_status = {0,};

uint8_t check_sync_addr_data(uint32_t dw_device_id,
                             uint8_t *pb_data_out,
                             uint8_t b_data_out_len,
                             uint32_t dw_data_in,
                             uint16_t w_data_addr)
{
    uint32_t dw_data_out_temp = 0;
    uint8_t b_result = FALSE;
    if((NULL == pb_data_out) ||\
       (b_data_out_len > cb_max_sync_data_len))
    {
        return FALSE;
    }
    if(cdw_device_type_cklcd == (dw_device_id & 0xFFFF00))
    {
        check_set_data_value(dw_device_id,w_data_addr,dw_data_in,&dw_data_in);
    }
    else if(cdw_device_type_ck_inverter == (dw_device_id & 0xFFFF00))
    {
        if(0x0052 == w_data_addr)
        {
            uint16_t w_data_temp_1 = 0;
            uint16_t w_data_len = 2;
            net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                   cb_net_data_att_set,
                                   0x0001,
                                   &w_data_len,
                                   (uint8_t *)&w_data_temp_1,
                                   cb_device_data_access_operate_type_read);
            if(dw_data_in > 1000)
            {
                dw_data_out_temp = dw_data_in - w_data_temp_1;
            }
            else
            {
                dw_data_out_temp = dw_data_in;
            }
            int_to_bytes_big_endain(pb_data_out,dw_data_out_temp,b_data_out_len);
            b_result = TRUE;
        }
        else if(0x0053 == w_data_addr)
        {
            uint16_t w_data_temp_1 = 0;
            uint16_t w_data_len = 2;
            net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                   cb_net_data_att_set,
                                   0x0001,
                                   &w_data_len,
                                   (uint8_t *)&w_data_temp_1,
                                   cb_device_data_access_operate_type_read);
            if(dw_data_in > 1000)
            {
                dw_data_out_temp = w_data_temp_1 - dw_data_in;
            }
            else
            {
                dw_data_out_temp = dw_data_in;
            }
            int_to_bytes_big_endain(pb_data_out,dw_data_out_temp,b_data_out_len);
            b_result = TRUE;
        }
        else if(0x004C == w_data_addr)
        {
            uint16_t w_inverter_version = 0;
            uint8_t b_bat_num = 1;
            uint16_t w_data_len = 2;
            net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                   cb_net_data_att_status,
                                   0x012C,
                                   &w_data_len,
                                   (uint8_t *)&w_inverter_version,
                                   cb_device_data_access_operate_type_read);//hardware ver
            w_inverter_version = (uint8_t)w_inverter_version;
            if((1 == w_inverter_version) || (13 == w_inverter_version))
            {
                b_bat_num = 2;
            }
            else
            {
                b_bat_num = 4;
            }
            if(0 != (dw_data_in & 0x8000))
            {
                dw_data_out_temp = ~(dw_data_in / b_bat_num) + 1;
                int_to_bytes_big_endain(pb_data_out,dw_data_out_temp,b_data_out_len);
                b_result = TRUE;
            }
        }
    }

    if(FALSE == b_result)
    {
        int_to_bytes_big_endain(pb_data_out,dw_data_in,b_data_out_len);
    }
    return TRUE;
}

uint8_t network_device_config_sycn_trig(uint32_t dw_device_type,
                                        uint8_t b_config_type,
                                        uint16_t w_config_addr,
                                        uint8_t b_data_len,
                                        uint8_t *pb_data_in,
                                        uint8_t b_sync_reg_num,
                                        uint8_t f_sync_fresh,
                                        uint8_t b_sync_type)
{
    uint8_t b_sync_list_index;
    uint8_t b_cnt_1;
    uint32_t dw_data_temp = 0;
	uint16_t w_data_temp1 = 1;
	uint16_t w_data_temp2 = 2;

    bytes_to_int_big_to_little_endain(pb_data_in,&dw_data_temp,b_data_len);


    uint8_t b_protocal_type = cb_system_protocal_type_ck;
    check_system_protocal_type(&b_protocal_type);

    if(cb_system_protocal_type_riiosunii == b_protocal_type)
    {
        for(b_sync_list_index = 0;b_sync_list_index < cb_cklcd_sync_list_num;b_sync_list_index++)
        {
            for(b_cnt_1 = 0;b_cnt_1 < st_cklcd_sync_list[b_sync_list_index].st_master_reg_list.b_reg_num;b_cnt_1++)
            {
                if(st_cklcd_sync_list[b_sync_list_index].st_master_reg_list.sta_device_config_sync_reg_info[b_cnt_1].dw_device_type ==\
                    (dw_device_type & 0xFF00))
                {
                    if((st_cklcd_sync_list[b_sync_list_index].st_master_reg_list.sta_device_config_sync_reg_info[b_cnt_1].w_reg_addr == w_config_addr) &&\
                       (st_cklcd_sync_list[b_sync_list_index].st_master_reg_list.sta_device_config_sync_reg_info[b_cnt_1].w_reg_value == dw_data_temp))
                    {
						if(b_sync_list_index ==6)
						{
							if(check_system_type(cb_inv_type_riio_sun_ii) 
								|| check_system_type(cb_inv_type_riio_sun_ii_gte) 
								|| check_system_type(cb_inv_type_riio_sun_ii_split_phase) 
								|| check_system_type(cb_inv_type_riio_ii_split_phase) 
								|| check_system_type(cb_inv_type_riio_ii))
                            {
                            	st_cklcd_sync_list[b_sync_list_index].st_sub_reg_list.sta_device_config_sync_reg_info[b_cnt_1].w_reg_value = w_data_temp2;
                                return network_device_config_sycn_trig_list((dw_device_type & 0xFF00),
                                                                            &st_cklcd_sync_list[b_sync_list_index],
                                                                            f_sync_fresh);
                            }
                            else
                            {
                                st_cklcd_sync_list[b_sync_list_index].st_sub_reg_list.sta_device_config_sync_reg_info[b_cnt_1].w_reg_value = w_data_temp1;
                                return network_device_config_sycn_trig_list((dw_device_type & 0xFF00),
                                                                            &st_cklcd_sync_list[b_sync_list_index],
                                                                            f_sync_fresh);
                            }
						}
						else
						{
							return network_device_config_sycn_trig_list((dw_device_type & 0xFF00),
																		&st_cklcd_sync_list[b_sync_list_index],
																		f_sync_fresh);
						}					
                    }
                }
                else
                {
                    break;
                }
            }
        }

    }
    return network_device_config_sycn_trig_single(dw_device_type,
                                                  b_config_type,
                                                  w_config_addr,
                                                  b_data_len,
                                                  pb_data_in,
                                                  b_sync_reg_num,
                                                  f_sync_fresh,
                                                  b_sync_type);
}
uint8_t network_device_config_sycn_trig_single(uint32_t dw_device_type,
                                               uint8_t b_config_type,
                                               uint16_t w_config_addr,      //设备数据表中的地址，如0x0100
                                               uint8_t b_data_len,
                                               uint8_t *pb_data_in,
                                               uint8_t b_sync_reg_num,
                                               uint8_t f_sync_fresh,
                                               uint8_t b_sync_type)
{
	uint16_t w_device_type = 0x0000;
	uint16_t w_device_id = dw_device_type & 0x00FF;
	if(0x00FFFF00 == (dw_device_type & 0xFFFF00))
	{
		w_device_type = (dw_device_type & 0xFFFF00) >> 8;
	}
	else
	{
		w_device_type = (dw_device_type & 0xFFFF00);
	}

    if(st_device_config_sync_info.b_sync_type != sync_type_none)
    {
        if(FALSE == st_device_config_sync_info.f_continue)//mark + if(FALSE == queue in)
        {
            return cb_sync_trig_return_busy;
        }
    }

 	if(((b_data_len > ba_auxiliary_installation_data_len) && cd_device_type_system == w_device_type)|| (0 == pb_data_in))
 	{
        return cb_sync_trig_return_err;
	}
    else if(((b_data_len > cb_max_sync_data_len) && cd_device_type_system != w_device_type)|| (0 == pb_data_in))
    {
        return cb_sync_trig_return_err;
    }

    uint8_t ba_data_buff[cb_max_sync_data_len] = {0,};
    uint32_t dw_data = 0;
    uint8_t b_protocal_type = cb_system_protocal_type_ck;

    check_system_protocal_type(&b_protocal_type);
    bytes_to_int_big_to_little_endain(pb_data_in,&dw_data,b_data_len);

    const net_data_info *pst_data_info = 0;
    net_device_info *pst_net_device_info = 0;
	if(cd_device_type_system == w_device_type)
	{

	}
    else if((cdw_device_type_cklcd == w_device_type) ||\
       (cdw_device_type_ck_inverter == w_device_type))
    {
        if(cb_net_subset_com_set_cmd == b_config_type)
        {
            if(FALSE == net_get_device_info((uint32_t)((w_device_type) | cdw_rs485_id_u1),
                                            (net_device_info **)&pst_net_device_info,
                                            &st_net_system_info_list))
            {
                return cb_sync_trig_return_value_err;
            }
            if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                 w_config_addr,
                                                 (const net_data_info ** const)&pst_data_info,
                                                 pst_net_device_info))
            {
                return cb_sync_trig_return_value_err;
            }
            if(cb_system_protocal_type_ck != b_protocal_type)
            {
                if(NULL != pst_data_info->pst_net_data_set_attribute)
                {
                    if(0x8000 == (pst_data_info->pst_data_attribute->w_unit & 0x8000))
                    {
                        if((dw_data & 0x8000) &&\
                           (2 == b_data_len))
                        {
                            dw_data |= 0xFFFF0000;
                        }
                        if(((int32_t)dw_data > (int32_t)pst_data_info->pst_net_data_set_attribute->dw_max_data) ||\
                           ((int32_t)dw_data < (int32_t)pst_data_info->pst_net_data_set_attribute->dw_min_data))
                        {
                            return cb_sync_trig_return_value_err;
                        }
                    }
                    else
                    {
                        if((dw_data > pst_data_info->pst_net_data_set_attribute->dw_max_data) ||\
                           (dw_data < pst_data_info->pst_net_data_set_attribute->dw_min_data))
                        {
                        	if(0 == ((w_device_type == 0x3300) && (w_config_addr == 0x0046 || w_config_addr == 0x0047)))
                        	{
								return cb_sync_trig_return_value_err;
							}
                            else if((w_device_type == 0x3300) && (w_config_addr == 0x0046))
                            {
                                dw_data = (dw_data > 5) ? dw_data : 5;
                            }
                            else if((w_device_type == 0x3300) && (w_config_addr == 0x0047))
                            {
                                dw_data = (dw_data > 50) ? dw_data : 50;
                            }
                        }
                    }
                }
            }
        }
        if((NULL != pst_data_info->pst_device_data_attribute) &&\
           (NULL != pst_data_info))
        {
            if(FALSE == check_sync_addr_data((uint32_t)w_device_type,
                                             ba_data_buff,
                                             pst_data_info->pst_device_data_attribute->b_len,
                                             dw_data,
                                             w_config_addr))
            {
                return cb_sync_trig_return_err;
            }
            b_data_len = pst_data_info->pst_device_data_attribute->b_len;
        }
        else
        {
            b_data_len = 2;
            for(uint8_t b_cnt_i = 0;b_cnt_i < b_data_len;b_cnt_i++)
            {
                ba_data_buff[b_cnt_i] = pb_data_in[b_cnt_i];
            }
        }
    }
    else
    {
        return cb_sync_trig_return_err;
    }
	
	st_device_config_sync_info.w_device_id = w_device_id;
    st_device_config_sync_info.f_sync_fresh = f_sync_fresh;
    st_device_config_sync_info.f_continue = FALSE;
    st_device_config_sync_info.b_sync_type = b_sync_type;
    st_device_config_sync_info.w_device_type = w_device_type;
    st_device_config_sync_info.b_config_type = b_config_type;
    st_device_config_sync_info.w_sync_addr = w_config_addr;
    st_device_config_sync_info.b_data_len = b_data_len + 2;

	if(cd_device_type_system == w_device_type)
	{
        if(st_device_config_sync_info.w_sync_addr == 0x4F00)        //如果是mppt辅助安装，将传入的inv_addr放在前两个
        {
            st_device_config_sync_info.ba_auxiliary_installation_data[0] = pb_data_in[0];
            st_device_config_sync_info.ba_auxiliary_installation_data[1] = pb_data_in[1];
            st_device_config_sync_info.ba_auxiliary_installation_data[2] = (uint8_t)(w_config_addr >> 8);
            st_device_config_sync_info.ba_auxiliary_installation_data[3] = (uint8_t)w_config_addr;
            for(uint8_t b_cnt_i = 0;b_cnt_i < b_data_len - 2;b_cnt_i++)
            {
                st_device_config_sync_info.ba_auxiliary_installation_data[b_cnt_i + 4] = pb_data_in[b_cnt_i + 2];
            }
        }
        else
        {
            st_device_config_sync_info.ba_auxiliary_installation_data[0] = (uint8_t)(w_config_addr >> 8);
            st_device_config_sync_info.ba_auxiliary_installation_data[1] = (uint8_t)w_config_addr;
            for(uint8_t b_cnt_i = 0;b_cnt_i < b_data_len ;b_cnt_i++)
            {
                st_device_config_sync_info.ba_auxiliary_installation_data[b_cnt_i + 2] = pb_data_in[b_cnt_i];
            }
        }
	}
	else
	{
		st_device_config_sync_info.ba_data[0] = (uint8_t)(w_config_addr >> 8);
    	st_device_config_sync_info.ba_data[1] = (uint8_t)w_config_addr;
		for(uint8_t b_cnt_i = 0;b_cnt_i < b_data_len;b_cnt_i++)
		{
			st_device_config_sync_info.ba_data[b_cnt_i + 2] = ba_data_buff[b_cnt_i];
		}
	}

    if(sync_type_total == b_sync_type)
    {
        st_device_config_sync_info.b_sync_reg_num = b_sync_reg_num;
    }
    else if(sync_type_single == b_sync_type)
    {
        st_device_config_sync_info.b_sync_reg_num = 1;
    }
    st_device_config_sync_info.b_sync_status = sync_continue;
    {
        uint16_t w_data_len = 1;

        st_device_config_sync_status.b_sync_status = synchronizing;
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x014C,
                               &w_data_len,
                               (uint8_t *)&st_device_config_sync_status.b_sync_status,
                               cb_device_data_access_operate_type_write);
    }

	#if(device_sync_status_print)
    DebugPrintf(TRUE,
                "Device Sync Trig,device:%06X,addr:%04X,sync_status:%d\r\n",
                st_device_config_sync_info.w_device_type,
                st_device_config_sync_info.w_sync_addr,
                st_device_config_sync_status.b_sync_status);    //增加串口显示系统同步状态 2023.05.14 S
	#endif
//    DebugPrintf(TRUE,
//        "Device Sync Trig,device:%06X,addr:%04X\r\n",
//        st_device_config_sync_info.w_device_type,
//        st_device_config_sync_info.w_sync_addr);
    return cb_sync_trig_return_ok;

}

uint8_t network_device_config_sycn_trig_list(uint16_t w_device_type,
                                             const device_config_sync_trig_list *pst_device_config_sync_trig_list,
                                             uint8_t f_sync_fresh)
{
    if(NULL != st_device_config_sync_info.pst_device_sync_list)
    {
        return cb_sync_trig_return_err;
    }

    st_device_config_sync_info.pst_device_sync_list = pst_device_config_sync_trig_list;
    st_device_config_sync_info.w_device_type = w_device_type;
    st_device_config_sync_info.f_sync_fresh = f_sync_fresh;
    return cb_sync_trig_return_ok;
}
//extern uint8_t b_background_sync_reception;
void device_config_sync_status_fresh()
{
    {//set enable
        uint16_t w_data_temp = 0;
        uint16_t w_data_len = 2;

        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
                               cb_device_data_access_data_type_set,
                               0x00DB,                                                                  //外部设置禁止使能 Remote Ctrl
                               &w_data_len,
                               (uint8_t *)&w_data_temp,
                               cb_device_data_access_operate_type_read);
        w_data_len = 1;
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x013D,                                                              //系统设置使能查询
                               &w_data_len,
                               (uint8_t *)&w_data_temp,
                               cb_device_data_access_operate_type_write);
    }
    if(sync_type_none == st_device_config_sync_info.b_sync_type)
    {
		st_device_config_sync_status.b_sync_status = sync_no_start;
		st_device_config_sync_status.dw_device_id = 0;
		st_device_config_sync_status.b_process = 0;
		st_device_config_sync_status.w_sync_addr = 0;
		goto EXIT;
    }
    if(sync_wait == st_device_config_sync_info.b_sync_status)
    {
        if(sync_type_total == st_device_config_sync_info.b_sync_type)
        {
            st_device_config_sync_info.b_sync_status = sync_no_start;
            st_device_config_sync_info.b_sync_type = sync_type_none;
            st_device_config_sync_info.b_sync_reg_num_total = 0;
			#if( device_sync_status_print)
			DebugPrintf(TRUE,
                "sync_type_total Device Sync finish,dw_device_id:%06X,w_sync_addr:%04X,b_sync_status:%d\r\n",
                st_device_config_sync_status.dw_device_id,
                st_device_config_sync_status.w_sync_addr,
                st_device_config_sync_info.b_sync_status);
			#endif
			st_device_config_sync_status.b_sync_status = sync_no_start;
			st_device_config_sync_status.dw_device_id = 0;
			st_device_config_sync_status.b_process = 0;
			st_device_config_sync_status.w_sync_addr = 0;
			yl_debug_printf("Device b_sync_status is :%d LINE is %d\r\n",st_device_config_sync_info.b_sync_status,__LINE__);
			goto EXIT;
        }
        else if(sync_type_single == st_device_config_sync_info.b_sync_type)
        {
            st_device_config_sync_info.b_sync_status = sync_no_start;
            st_device_config_sync_info.b_sync_type = sync_type_none;
			#if( device_sync_status_print)
			DebugPrintf(TRUE,
                "sync_type_single Device Sync finish,dw_device_id:%06X,w_sync_addr:%04X,b_sync_status:%d\r\n",
                st_device_config_sync_status.dw_device_id,
                st_device_config_sync_status.w_sync_addr,
                st_device_config_sync_info.b_sync_status);
			#endif
			st_device_config_sync_status.b_sync_status = sync_no_start;
			st_device_config_sync_status.dw_device_id = 0;
			st_device_config_sync_status.b_process = 0;
			st_device_config_sync_status.w_sync_addr = 0;
			yl_debug_printf("Device b_sync_status is :%d LINE is %d\r\n",st_device_config_sync_info.b_sync_status,__LINE__);
			goto EXIT;
        }
    }
    goto EXIT;
    EXIT:
    {
        uint16_t w_data_len = 2;
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x014F,
                               &w_data_len,
                               (uint8_t *)&st_device_config_sync_status.w_sync_addr,
                               cb_device_data_access_operate_type_write);
        w_data_len = 1;
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x014C,
                               &w_data_len,
                               (uint8_t *)&st_device_config_sync_status.b_sync_status,
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x014D,
                               &w_data_len,
                               (uint8_t *)&st_device_config_sync_status.b_process,
                               cb_device_data_access_operate_type_write);
        w_data_len = 4;
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x014E,
                               &w_data_len,
                               (uint8_t *)&st_device_config_sync_status.dw_device_id,
                               cb_device_data_access_operate_type_write);
    }

    }
void device_config_sync_list()
{
    #define cb_sync_list_process_init_para              0
    #define cb_sync_list_process_check_device_para      1
    #define cb_sync_list_process_set_master_para        2
    #define cb_sync_list_process_set_sub_para           3
    #define cb_sync_list_process_sync_trig              4

    static uint8_t b_sync_step = cb_sync_list_process_init_para;
    static uint8_t b_sync_index = 0;
    static uint8_t b_sync_reg_num = 0;
    static uint8_t f_sync_fresh = FALSE;
    static uint8_t ba_data[cb_max_sync_data_len] = {0,};
    static uint16_t w_davice_type;
    static uint16_t w_set_addr;

    if(NULL == st_device_config_sync_info.pst_device_sync_list)
    {
        b_sync_step = cb_sync_list_process_init_para;
        b_sync_index = 0;
        b_sync_reg_num = 0;
//        st_device_config_sync_info.b_sync_reg_num_total = 0;
        return;
    }


    switch(b_sync_step)
    {
        case cb_sync_list_process_init_para:
        {
            st_device_config_sync_info.b_sync_reg_num_total =\
                st_device_config_sync_info.pst_device_sync_list->st_master_reg_list.b_reg_num +\
                st_device_config_sync_info.pst_device_sync_list->st_sub_reg_list.b_reg_num;
            b_sync_step = cb_sync_list_process_set_master_para;
        }
        break;
        case cb_sync_list_process_check_device_para:
        {
            if(b_sync_reg_num < st_device_config_sync_info.pst_device_sync_list->st_master_reg_list.b_reg_num)
            {
                b_sync_step = cb_sync_list_process_set_master_para;
            }
            else if((b_sync_reg_num >= st_device_config_sync_info.pst_device_sync_list->st_master_reg_list.b_reg_num) &&\
                    (b_sync_reg_num < st_device_config_sync_info.b_sync_reg_num_total))
            {
                if(b_sync_reg_num == st_device_config_sync_info.pst_device_sync_list->st_master_reg_list.b_reg_num)
                {
                    b_sync_index = 0;
                }
                b_sync_step = cb_sync_list_process_set_sub_para;
            }
            else
            {
                st_device_config_sync_info.pst_device_sync_list = NULL;
                b_sync_step = cb_sync_list_process_init_para;
            }
        }
        break;
        case cb_sync_list_process_set_master_para:
        {
            if(b_sync_index < st_device_config_sync_info.pst_device_sync_list->st_master_reg_list.b_reg_num)
            {
                uint32_t dw_data_temp = 0;
                w_set_addr = st_device_config_sync_info.
                                pst_device_sync_list->
                                st_master_reg_list.
                                sta_device_config_sync_reg_info[b_sync_index].
                                w_reg_addr;
                w_davice_type = (uint16_t)st_device_config_sync_info.
                                            pst_device_sync_list->
                                            st_master_reg_list.
                                            sta_device_config_sync_reg_info[b_sync_index].
                                            dw_device_type;
                dw_data_temp = st_device_config_sync_info.
                                pst_device_sync_list->
                                st_master_reg_list.
                                sta_device_config_sync_reg_info[b_sync_index].
                                w_reg_value;
//                if((0x0046 == w_set_addr) &&\
//                   (cdw_device_type_ck_inverter == w_davice_type))
//                {
//                    uint16_t w_data_len = 2;
//                    uint16_t w_data_temp = 0;
//
//                    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
//                                           cb_device_data_access_data_type_set,
//                                           0x0048,
//                                           &w_data_len,
//                                           (uint8_t *)&w_data_temp,
//                                           cb_device_data_access_operate_type_read);
//                    dw_data_temp = (w_data_temp * dw_data_temp) / 100;
//                }
                int_to_bytes_big_endain(ba_data,
                                        dw_data_temp,
                                        2);

                f_sync_fresh = TRUE;
                b_sync_reg_num++;
                b_sync_step = cb_sync_list_process_sync_trig;
            }
            else
            {
                b_sync_step = cb_sync_list_process_check_device_para;
            }
        }
        break;
        case cb_sync_list_process_set_sub_para:
        {
            if(b_sync_index < st_device_config_sync_info.pst_device_sync_list->st_sub_reg_list.b_reg_num)
            {
                uint32_t dw_data_temp = 0;
                w_set_addr = st_device_config_sync_info.
                                pst_device_sync_list->
                                st_sub_reg_list.
                                sta_device_config_sync_reg_info[b_sync_index].
                                w_reg_addr;
                w_davice_type = (uint16_t)st_device_config_sync_info.
                                            pst_device_sync_list->
                                            st_sub_reg_list.
                                            sta_device_config_sync_reg_info[b_sync_index].
                                            dw_device_type;
                dw_data_temp = st_device_config_sync_info.
                                pst_device_sync_list->
                                st_sub_reg_list.
                                sta_device_config_sync_reg_info[b_sync_index].
                                w_reg_value;
                if((0x0046 == w_set_addr) &&\
                   (cdw_device_type_ck_inverter == w_davice_type))
                {
                    uint16_t w_data_len = 2;
                    uint16_t w_data_temp = 0;

                    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                           cb_device_data_access_data_type_set,
                                           0x0048,
                                           &w_data_len,
                                           (uint8_t *)&w_data_temp,
                                           cb_device_data_access_operate_type_read);
                    dw_data_temp = (w_data_temp * dw_data_temp) / 100;
                }
                else if((0x0040 == w_set_addr) &&\
                        (cdw_device_type_ck_inverter == w_davice_type))
                {
                    uint16_t w_data_len = 2;
                    uint16_t w_data_temp = 0;

                    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                           cb_device_data_access_data_type_status,
                                           0x012C,
                                           &w_data_len,
                                           (uint8_t *)&w_data_temp,
                                           cb_device_data_access_operate_type_read);
                    w_data_temp &= 0x00FF;
                    if((5 == w_data_temp) ||\
                       (6 == w_data_temp) ||\
                       (7 == w_data_temp) ||\
                       (8 == w_data_temp) ||\
                       (10 == w_data_temp) ||\
                       (14 == w_data_temp))
                    {
                        dw_data_temp = 13300;
                    }
                }
                else if((0x0041 == w_set_addr) &&\
                        (cdw_device_type_ck_inverter == w_davice_type))
                {
                    uint16_t w_data_len = 2;
                    uint16_t w_data_temp = 0;

                    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                           cb_device_data_access_data_type_status,
                                           0x012C,
                                           &w_data_len,
                                           (uint8_t *)&w_data_temp,
                                           cb_device_data_access_operate_type_read);
                    w_data_temp &= 0x00FF;
                    if((5 == w_data_temp) ||\
                       (6 == w_data_temp) ||\
                       (7 == w_data_temp) ||\
                       (8 == w_data_temp) ||\
                       (10 == w_data_temp) ||\
                       (14 == w_data_temp))
                    {
                        dw_data_temp = 13100;
                    }
                }
                int_to_bytes_big_endain(ba_data,
                                        dw_data_temp,
                                        2);
                f_sync_fresh = TRUE;
                b_sync_reg_num++;
                b_sync_step = cb_sync_list_process_sync_trig;
            }
            else
            {
                b_sync_step = cb_sync_list_process_check_device_para;
            }
        }
        break;
        case cb_sync_list_process_sync_trig:
        {
            uint8_t b_result;

            b_result = network_device_config_sycn_trig_single(w_davice_type,
                                                              cb_net_subset_com_set_cmd,
                                                              w_set_addr,
                                                              2,
                                                              ba_data,
                                                              b_sync_reg_num,
                                                              f_sync_fresh,
                                                              sync_type_total);

            if(cb_sync_trig_return_busy != b_result)
            {
                b_sync_index++;
                f_sync_fresh = FALSE;
                b_sync_step = cb_sync_list_process_check_device_para;
            }
        }
        break;
    }
}

extern SemaphoreHandle_t  pst_rs485_data_polling_mutex;
uint8_t device_config_sync()  // 设置数据
{
    #define cb_sync_process_check_sync_enable       0
    #define cb_sync_process_send_sync_data          1
    #define cb_sync_process_receive_sync_data       2
    #define cb_sync_process_hold_sync_status        3
    #define cb_sync_process_sync_finish             4

    static uint8_t f_init = TRUE;

    static uint8_t b_device_index      = 0;
    static uint8_t b_device_index_init = 0;
    static uint8_t b_device_sync_cnt   = 0;
    static uint8_t b_process           = cb_sync_process_check_sync_enable;
    static uint8_t b_rty_cnt           = 0;
    static uint8_t f_sync_status_hold  = TRUE;
    static TickType_t x_start_time = 0;
    static net_device_info *pst_net_device_info = 0;
    static const net_data_info *pst_data_info = 0;

    const uint8_t ba_device_com_id[9] =
    {
        cdw_rs485_id_u1,
        cdw_rs485_id_u2,
        cdw_rs485_id_u3,
        cdw_rs485_id_v1,
        cdw_rs485_id_v2,
        cdw_rs485_id_v3,
        cdw_rs485_id_w1,
        cdw_rs485_id_w2,
        cdw_rs485_id_w3,
    };


    if(sync_type_none == st_device_config_sync_info.b_sync_type)
    {
        f_init = TRUE;
        return st_device_config_sync_info.b_sync_status;
    }


    if(f_init)
    {
        f_init = FALSE;

        b_device_index      = 0;
        b_device_index_init = 0;
        b_device_sync_cnt   = 0;
        b_process           = cb_sync_process_check_sync_enable;
        b_rty_cnt           = 0;
        f_sync_status_hold  = TRUE;
    }

    if(st_device_config_sync_info.f_sync_fresh)
    {
        st_device_config_sync_info.f_sync_fresh = FALSE;

        if(cdw_device_type_cklcd == st_device_config_sync_info.w_device_type)
        {
			if ((0X0020 == st_device_config_sync_info.w_sync_addr 
			    && (TRUE == model_identification(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],cb_inv_type_apollo_matrix)
                || TRUE == model_identification(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],cb_inv_type_apollo_maxx)
                || TRUE == model_identification(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],cb_inv_type_ck_ii)))
                || TRUE == check_system_type(cb_inv_type_apollo_matric_ii_lite)
                || TRUE == check_system_type(cb_inv_type_Tyrann_3_5_8k)
                || TRUE == check_system_type(cb_inv_type_apollo_matric_ii_5_8k)
                || 0X0006 == st_device_config_sync_info.w_sync_addr)
			{
				b_device_sync_cnt = 1;
			}
			else
			{
				b_device_sync_cnt = 9;
			}
			b_device_index_init = 0;
			b_device_index = b_device_index_init;
            //mark      check if sync master only
        }
        else if(cdw_device_type_ck_inverter == st_device_config_sync_info.w_device_type)
        {
			b_device_sync_cnt = 9;
			if (0x03F5 == st_device_config_sync_info.w_sync_addr)   // 判断 是否是 1013 触发主从同步 不需要设置从机
			{
				b_device_index_init = 1;
			}
			else
			{
				b_device_index_init = 0;
			}
			b_device_index = b_device_index_init;
            //mark      check if sync master only
        }
		else if(cd_device_type_system == st_device_config_sync_info.w_device_type)  //设备类型为0xFFFF00
		{
			b_device_sync_cnt = 1;
			b_device_index = 0;
		}
    }
	uint8_t b_sync_flag = 1;
//	if(st_device_config_sync_info.b_sync_type == sync_type_total)
//	{
//		b_sync_flag = 0;
//	
//	}
	do
	{
		switch(b_process)
		{
	        case cb_sync_process_check_sync_enable:     // 检查是否可以同步
	        {
	        	if(FALSE == net_get_device_info((cdw_device_type_cklcd | cdw_rs485_id_u1),
                                                (net_device_info **)&pst_net_device_info,
                                                &st_net_system_info_list))
                {
                    continue;
                }
                if(FALSE == net_check_device_communicate_status(cb_net_device_add,pst_net_device_info) 
					&&(cb_wired_network_inv_auxiliary_installation_cmd != st_device_config_sync_info.b_config_type 
					&& cb_wired_network_mppt_auxiliary_installation_cmd != st_device_config_sync_info.b_config_type))			//主机失联不去设置设置项
                {
                		st_device_config_sync_info.b_sync_status = sync_impermission_fail;
	                    b_process = cb_sync_process_hold_sync_status;
                }
	            st_device_config_sync_info.b_process = (uint8_t)(((uint16_t)b_device_index* 100)/b_device_sync_cnt);
	            if(b_device_index < b_device_sync_cnt)
	            {
	                uint8_t w_set_permission = FALSE;

	                st_device_config_sync_info.b_sync_status = synchronizing;

					if(0X0006 == st_device_config_sync_info.w_sync_addr)									//mppt类型可以单独设置
					{
						st_device_config_sync_info.dw_device_id =\
	                    (st_device_config_sync_info.w_device_type | st_device_config_sync_info.w_device_id);
					}
					else if(cd_device_type_system == st_device_config_sync_info.w_device_type)
					{
						st_device_config_sync_info.dw_device_id =0xFFFF00;
					}
                    else
                    {
                        st_device_config_sync_info.dw_device_id =\
                            (st_device_config_sync_info.w_device_type | ba_device_com_id[b_device_index]);
                    }

	                if(FALSE == net_get_device_info(st_device_config_sync_info.dw_device_id,
	                                                (net_device_info **)&pst_net_device_info,
	                                                &st_net_system_info_list))
	                {
	                    b_device_index++;
	                    break;
	                }
					yl_debug_printf("Device index is :%d \r\n",b_device_index);
	                {
	                    uint16_t w_data_len = 2;
	                    net_device_data_access((cdw_device_type_cklcd | ba_device_com_id[b_device_index]),
	                                           cb_device_data_access_data_type_set,
	                                           0x00DB,
	                                           &w_data_len,
	                                           (uint8_t *)&w_set_permission,
	                                           cb_device_data_access_operate_type_read);
	                }
					if(cb_wired_network_inv_auxiliary_installation_cmd == st_device_config_sync_info.b_config_type 
						|| cb_wired_network_mppt_auxiliary_installation_cmd == st_device_config_sync_info.b_config_type)
					{
						st_device_config_sync_info.b_sync_status = synchronizing;
						b_process = cb_sync_process_send_sync_data;
					}
	                else if(cb_net_device_loss == pst_net_device_info->pst_net_device_status->b_communicate_status)
	                {
	                    b_device_index++;
	                    //loss fail mark

	                }
	                else if(FALSE == w_set_permission)
	                {
	                    st_device_config_sync_info.b_sync_status = sync_impermission_fail;
	                    b_process = cb_sync_process_hold_sync_status;

	                    #if( system_alarm_print )
	                    DebugPrintf(TRUE,"[Alarm]Device set impermission\r\n");
	                    #endif
	                }
	                else if(cb_net_device_add == pst_net_device_info->pst_net_device_status->b_communicate_status)
	                {
	                	yl_debug_printf("Device index is :%d \r\n",b_device_index);
	                    st_device_config_sync_info.b_sync_status = synchronizing;
	                    b_process = cb_sync_process_send_sync_data;
	                }
	            }
	            else
	            {
	                b_device_index = b_device_index_init;
	                st_device_config_sync_info.b_sync_status = sync_complete;
	                b_process = cb_sync_process_hold_sync_status;
					yl_debug_printf("Device b_sync_status is :%d LINE is %d\r\n",st_device_config_sync_info.b_sync_status,__LINE__);
	                #if( device_sync_status_print)
	                DebugPrintf(TRUE,"Device Sync Complete\n");
	                #endif
	            }
	        }
	        break;
	        case cb_sync_process_send_sync_data:
	        {
	            if(b_rty_cnt < 3)
	            {
	                b_rty_cnt++;
	                uint8_t b_net_data_type = cb_net_data_att_set;

	                if(cb_net_subset_com_set_cmd == st_device_config_sync_info.b_config_type || cb_wired_network_inv_auxiliary_installation_cmd == st_device_config_sync_info.b_config_type 
                        || cb_wired_network_mppt_auxiliary_installation_cmd == st_device_config_sync_info.b_config_type)
	                {
	                    b_net_data_type = cb_net_data_att_set;
	                }
	                else if(cb_net_subset_com_ctrl_cmd == st_device_config_sync_info.b_config_type)
	                {
	                    b_net_data_type = cb_net_data_att_control;
	                }

	                if((FALSE == net_get_device_data_info(b_net_data_type,st_device_config_sync_info.w_sync_addr,(const net_data_info **)&pst_data_info,pst_net_device_info))
	                   ||(cb_net_device_add != pst_net_device_info->pst_net_device_status->b_communicate_status))
	                {
	                    b_device_index++;
	                    b_process = cb_sync_process_check_sync_enable;
	                    break;
	                }

					if(NULL != pst_rs485_data_polling_mutex)
	                {
	                    xSemaphoreTake(pst_rs485_data_polling_mutex, portMAX_DELAY);
						vTaskSuspend(device_data_polling_task_handle);
						xSemaphoreGive(pst_rs485_data_polling_mutex);
	                }
					uint8_t *pb_data_in = 0;
					if(cb_wired_network_inv_auxiliary_installation_cmd == st_device_config_sync_info.b_config_type)
					{
						pb_data_in =  st_device_config_sync_info.ba_auxiliary_installation_data;	//逆变器辅助安装
					}
                    else if(cb_wired_network_mppt_auxiliary_installation_cmd == st_device_config_sync_info.b_config_type)
                    {
                        pb_data_in =  st_device_config_sync_info.ba_auxiliary_installation_data;    //mppt辅助安装
                    }
					else
					{
						pb_data_in =  st_device_config_sync_info.ba_data;							//常规设置项

					}
	                if(TRUE == net_device_send_data(pst_net_device_info->b_wired_communication_cable,
	                                                st_device_config_sync_info.b_config_type,
	                                                pst_net_device_info->dw_device_address_send,
	                                                pst_data_info->pst_device_data_attribute->w_device_set_cmd, //06 66
	                                                st_device_config_sync_info.b_data_len,
	                                                pb_data_in,
	                                                pst_net_device_info))
	                {
	                    #if( device_sync_status_print)
	                    DebugPrintf(TRUE,"Device_Sync_Send,Addr:%04X\n",st_device_config_sync_info.w_sync_addr);
	                    #endif

	                    b_process = cb_sync_process_receive_sync_data;
	                }
	                else
	                {
						vTaskResume(device_data_polling_task_handle);
	                    b_rty_cnt = 0;
	                    b_process = cb_sync_process_hold_sync_status;
	                    st_device_config_sync_info.b_sync_status = sync_no_response_fail;
						yl_debug_printf("Device_Sync_No_Response %d\n",__LINE__);
	                }
//					yl_debug_printf("Device b_sync_status is :%d LINE is %d\r\n",st_device_config_sync_info.b_sync_status,__LINE__);
	            }
	            else
	            {
					vTaskResume(device_data_polling_task_handle);
	                b_rty_cnt = 0;
	                b_process = cb_sync_process_hold_sync_status;
	                st_device_config_sync_info.b_sync_status = sync_no_response_fail;
					yl_debug_printf("Device_Sync_No_Response after three times\n");
	                #if( device_sync_status_print)
	                DebugPrintf(TRUE,"Device_Sync_No_Response\n");
	                #endif
	            }
	        }
	        break;
	        case cb_sync_process_receive_sync_data:
	        {
	            uint8_t b_receive_state = 0;
	            uint16_t w_receive_data_len = 0;
	            uint8_t ba_receive_data[40] = {0,};
	            if(TRUE == net_device_receive_data(pst_net_device_info->b_wired_communication_cable,
	                                               st_device_config_sync_info.b_config_type,
	                                               pst_net_device_info->dw_device_address_receive,
	                                               pst_data_info->pst_device_data_attribute->w_device_set_cmd,
	                                               &w_receive_data_len,
	                                               ba_receive_data,
	                                               40,
	                                               &b_receive_state,
	                                               pst_net_device_info))
	            {
	                if(cb_wired_network_receive_state_success == b_receive_state)
	                {
						vTaskResume(device_data_polling_task_handle);
	                    b_rty_cnt = 0;
	                    b_device_index++;
	                    b_process = cb_sync_process_check_sync_enable;
	                    st_device_config_sync_info.b_sync_status = synchronizing;

	                    if(cb_net_subset_com_set_cmd == st_device_config_sync_info.b_config_type)
	                    {
	                        uint16_t w_data_len;
	                        uint32_t dw_data_temp = 0;
							if(check_system_type(cb_inv_type_riio_sun_ii_split_phase) || check_system_type(cb_inv_type_riio_ii_split_phase))
							{
								w_data_len = w_receive_data_len;
		                        if(TRUE == bytes_to_int_big_to_little_endain(&ba_receive_data[0],
		                                                                     &dw_data_temp,
		                                                                     w_data_len))
		                        {                       	
		                            net_device_data_access(*pst_net_device_info->pdw_device_id,
		                                                   cb_device_data_access_data_type_set,
		                                                   st_device_config_sync_info.w_sync_addr,
		                                                   &w_data_len,
		                                                   (uint8_t *)&dw_data_temp,
		                                                   cb_device_data_access_operate_type_write);
		                        }
							}
							else
							{
								w_data_len = w_receive_data_len - 2;
		                        if(TRUE == bytes_to_int_big_to_little_endain(&ba_receive_data[2],
		                                                                     &dw_data_temp,
		                                                                     w_data_len))
		                        {                       	
                                    if((check_system_type(cb_inv_type_riio_sun_ii) 
										|| check_system_type(cb_inv_type_riio_ii)
										|| check_system_type(cb_inv_type_riio_sun_ii_gte)) 
										&& (*pst_net_device_info->pdw_device_id == 0x003311) 
										&& (0x10 == st_device_config_sync_info.w_sync_addr))
                                    {
                                        net_device_data_access(0x001A11,
                                                               cb_device_data_access_data_type_set,
                                                               0x20,
                                                               &w_data_len,
                                                               ( uint8_t * )&dw_data_temp,
                                                               cb_device_data_access_operate_type_write);
                                    }
									if(((*pst_net_device_info->pdw_device_id & 0xFF00) == 0x3300) && (st_device_config_sync_info.w_sync_addr == 0x0046 || st_device_config_sync_info.w_sync_addr == 0x0047))
									{
										uint8_t b_inv_num = 0;
										uint16_t w_data_len = 1;
										net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
															   cb_net_data_att_status,
															   0x0160,
															   &w_data_len,
															   (uint8_t *)&b_inv_num,
															   cb_device_data_access_operate_type_read);
										dw_data_temp = dw_data_temp * b_inv_num;
									}

		                            net_device_data_access_simple(*pst_net_device_info->pdw_device_id,
		                                                   cb_device_data_access_data_type_set,
		                                                   st_device_config_sync_info.w_sync_addr,
		                                                   &w_data_len,
		                                                   (uint8_t *)&dw_data_temp,
		                                                   cb_device_data_access_operate_type_write);
		                        }
							}
	                    }
                        else if(cb_wired_network_inv_auxiliary_installation_cmd == st_device_config_sync_info.b_config_type)
                        {
                            uint16_t w_receive_cmd_status = 0;
							b_process = cb_sync_process_hold_sync_status;
                            w_receive_cmd_status = (ba_receive_data[16] << 4) | ba_receive_data[17];
							if(1 == w_receive_cmd_status)
							{
								st_device_config_sync_info.b_sync_status = sync_complete;
							}
							else
							{
								yl_debug_printf("inv auxiliary installation receive cmd status false!!! status is :%d\r\n",w_receive_cmd_status);
								st_device_config_sync_info.b_sync_status = sync_no_response_fail;
							}
                        }
                        else if(cb_wired_network_mppt_auxiliary_installation_cmd == st_device_config_sync_info.b_config_type)
                        {
                            uint16_t w_inv_addr = 0;
                            uint16_t w_receive_mppt_addr = 0;
                            uint16_t w_mppt_index = 0;
							b_process = cb_sync_process_hold_sync_status;
                            w_inv_addr = (st_device_config_sync_info.ba_auxiliary_installation_data[22] << 8) 
											| st_device_config_sync_info.ba_auxiliary_installation_data[23];
                            w_receive_mppt_addr = (ba_receive_data[0] << 4) | ba_receive_data[1];
							if(0xFFFF != w_receive_mppt_addr && w_inv_addr == w_receive_mppt_addr)
							{
								st_device_config_sync_info.b_sync_status = sync_complete;
							}
							else
							{
								yl_debug_printf("mppt auxiliary installation receive mppt addr false!!! receive mppt addr is :%d\r\n",w_receive_mppt_addr);
								st_device_config_sync_info.b_sync_status = sync_no_response_fail;
							}                   
                        }
	                }
	                else if(cb_wired_network_receive_state_fail == b_receive_state)
	                {
						vTaskResume(device_data_polling_task_handle);
	                    {
	                        #if( device_sync_status_print)
	                        DebugPrintf(TRUE,"Device Sync No Response Cnt:%d\n",b_rty_cnt);   // 返回错误的位置
	                        #endif

	                        b_process = cb_sync_process_send_sync_data;
	                    }
	                }
	            }
	            else
	            {
					vTaskResume(device_data_polling_task_handle);
                    if(cb_wired_network_inv_auxiliary_installation_cmd == st_device_config_sync_info.b_config_type)
                    {
                        b_process = cb_sync_process_send_sync_data;
                    }
                    else
                    {
                        b_device_index++;
                        b_process = cb_sync_process_check_sync_enable;
                    }

	            }
	        }
	        break;
	        case cb_sync_process_hold_sync_status:
	        {
                if((synchronizing == st_device_config_sync_info.b_sync_status) 
					||(sync_complete == st_device_config_sync_info.b_sync_status) 
					||((sync_no_response_fail <= st_device_config_sync_info.b_sync_status) 
					&&(sync_device_inexist_fail >= st_device_config_sync_info.b_sync_status)))
                {
                    st_device_config_sync_status.b_sync_status = st_device_config_sync_info.b_sync_status;
                    st_device_config_sync_status.w_sync_addr = st_device_config_sync_info.w_sync_addr;
                    st_device_config_sync_status.dw_device_id = st_device_config_sync_info.dw_device_id;

                    if((sync_type_total == st_device_config_sync_info.b_sync_type) 
						&&(0 != st_device_config_sync_info.b_sync_reg_num_total))
                    {
                        st_device_config_sync_status.b_process = ((st_device_config_sync_info.b_sync_reg_num) * 100) / st_device_config_sync_info.b_sync_reg_num_total;
                    }
                    else if(sync_type_single == st_device_config_sync_info.b_sync_type)
                    {
                        st_device_config_sync_status.b_process = st_device_config_sync_info.b_process;
                    }
                }
                {
                    uint16_t w_data_len = 2;
                    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                           cb_device_data_access_data_type_status,
                                           0x014F,
                                           &w_data_len,
                                           (uint8_t *)&st_device_config_sync_status.w_sync_addr,
                                           cb_device_data_access_operate_type_write);
                    w_data_len = 1;
                    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                           cb_device_data_access_data_type_status,
                                           0x014C,
                                           &w_data_len,
                                           (uint8_t *)&st_device_config_sync_status.b_sync_status,
                                           cb_device_data_access_operate_type_write);
                    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                           cb_device_data_access_data_type_status,
                                           0x014D,
                                           &w_data_len,
                                           (uint8_t *)&st_device_config_sync_status.b_process,
                                           cb_device_data_access_operate_type_write);
                    w_data_len = 4;
                    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                           cb_device_data_access_data_type_status,
                                           0x014E,
                                           &w_data_len,
                                           (uint8_t *)&st_device_config_sync_status.dw_device_id,
                                           cb_device_data_access_operate_type_write);
                }
	            TickType_t x_curent_time = 0;
	            static TickType_t x_delay_time = 0;
	            if(f_sync_status_hold)
	            {
	                f_sync_status_hold = FALSE;
	                x_start_time = xTaskGetTickCount();

                    if ((st_device_config_sync_info.b_sync_status >= sync_complete)
                        && (st_device_config_sync_info.b_sync_status <= sync_device_inexist_fail))
                    {
                        if (cb_wired_network_inv_auxiliary_installation_cmd
                            == st_device_config_sync_info.b_config_type)
                        {
                            x_delay_time = pdMS_TO_TICKS_USER(1000);
                        }
                        else if (cb_wired_network_mppt_auxiliary_installation_cmd
                                 == st_device_config_sync_info.b_config_type)
                        {
                            x_delay_time = pdMS_TO_TICKS_USER(600);    // mppt辅助安装1000
                        }
                        else
                        {
                            x_delay_time = pdMS_TO_TICKS_USER(100);    // 常规设置项
                        }
                    }
                    else
	                {
	                    x_delay_time = 0;
	                }
	            }
	            x_curent_time = xTaskGetTickCount();
	            if((x_curent_time - x_start_time) >= x_delay_time)
	            {
	                f_sync_status_hold = TRUE;
	                if(sync_complete == st_device_config_sync_info.b_sync_status)
	                {
	                    b_process = cb_sync_process_sync_finish;
						
	                }
	                else
	                {
	                    b_device_index++;
	                    b_process = cb_sync_process_check_sync_enable;
	                }
	            }
//				yl_debug_printf("Device b_sync_status is :%d LINE is %d\r\n",st_device_config_sync_info.b_sync_status,__LINE__);
	        }
	        break;
	        case cb_sync_process_sync_finish        :
	        {
	            if(sync_continue == st_device_config_sync_info.b_sync_status)
	            {
	                b_process = cb_sync_process_check_sync_enable;
	            }
	            else
	            {	

	                st_device_config_sync_info.b_sync_status = sync_wait;
					yl_debug_printf("Device b_sync_status is :%d LINE is %d\r\n",st_device_config_sync_info.b_sync_status,__LINE__);
					b_sync_flag = 0;
					#if( device_sync_status_print)
	                DebugPrintf(TRUE,"Device Sync sync_wait\n");
	                #endif
	            }
	        }
	        break;
	    }

	}while(b_sync_flag);

    
    return st_device_config_sync_info.b_sync_status;
}

void device_config_sync_main()
{
#if( task_runing_status_print )
    DebugPrintf(TRUE,"Device_sync_main_start\r\n");
#endif

    uint8_t b_sync_status = device_config_sync();
    device_config_sync_status_fresh();
    device_config_sync_list();
	vTaskResume(device_data_polling_task_handle);
//	vTaskResume(device_data_polling_task_handle);
#if( task_runing_status_print )
    DebugPrintf(TRUE,"Device_sync_main_end\r\n");
#endif
}

/////////////////////////////////////////////////////////////////////
/////////////////////////--->range manage .c/////////////////////////mark
typedef struct
{
    uint16_t w_net_addr;
    uint32_t *pdw_max_value_device_id;
    uint32_t *pdw_min_value_device_id;
    uint16_t w_max_value_net_addr;
    uint16_t w_min_value_net_addr;
    int32_t dw_max_value_increment;
    int32_t dw_min_value_increment;
    int16_t w_max_local_value;
    int16_t w_min_local_value;
    int8_t *pb_rate;
    uint32_t dw_step;
}device_set_range_info;

typedef struct
{
    uint8_t b_model_ver;
    uint8_t b_hd_ver;
	uint8_t b_rate_power;
	uint8_t b_bat_voltage_level;
    uint16_t w_ac_current_max;
    uint16_t w_ac_current_in_max;
	uint16_t w_iner_mppt_charge_current_max;
}cklcd_ver_para;

int8_t b_bat_num = 1;

#define cb_max_cklcd_set_range_info_num                 22
const device_set_range_info st_cklcd_set_range_info[cb_max_cklcd_set_range_info_num] =
{
    {//0
        0x0015,
        &dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
        &dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
        0x0016,
        0x0044,
        -500,
        0,
        13500,
        10000,
        &b_bat_num,
        1,
    },
    {//1
        0x0016,
        &dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
        &dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
        0x0040,
        0x0015,
        -200,
        500,
        14500,
        12000,
        &b_bat_num,
        1,
    },
    {//2
        0x001A,
        NULL,
        &dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
        0,
        0x001B,
        150,
        10,
        150,
        10,
        NULL,
        1,
    },
    {//3
        0x001B,
        &dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
        NULL,
        0x001A,
        0,
        -10,
        0,
        140,
        1,
        NULL,
        1,
    },
    {//4
        0x001C,
        &dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
        &dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
        0x001D,
        0x0091,
        -5,
        1,
        80,
        0,
        NULL,
        1,
    },
    {//5
        0x001D,
        NULL,
        &dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
        0,
        0x001C,
        100,
        5,
        100,
        11,
        NULL,
        1,
    },
    {//6
        0x002A,
        &dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
        &dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
        0x002B,
        0x0044,
        -500,
        0,
        13500,
        11500,
        &b_bat_num,
        1,
    },
    {//7
        0x002B,
        NULL,
        &dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
        0,
        0x002A,
        14500,
        500,
        13500,
        12000,
        &b_bat_num,
        1,
    },
    {//8
        0x002E,
        &dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
        &dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
        0x002F,
        0x0091,
        -5,
        1,
        80,
        0,
        NULL,
        1,
    },
    {//9
        0x002F,
        NULL,
        &dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
        0,
        0x002E,
        100,
        5,
        100,
        0,
        NULL,
        1,
    },
    {//10
        0x0041,
        &dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
        &dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
        0x0042,
        0x0044,
        -500,
        0,
        13500,
        10000,
        &b_bat_num,
        1,
    },
    {//11
        0x0042,
        0,
        &dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
        0,
        0x0041,
        14500,
        500,
        14500,
        12000,
        &b_bat_num,
        1,
    },
    {//12
        0x004C,
        &dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
        &dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
        0x004D,
        0x0092,
        -5,
        1,
        80,
        5,
        NULL,
        1,
    },
    {//13
        0x004D,
        0,
        &dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
        0,
        0x004C,
        100,
        5,
        100,
        11,
        NULL,
        1,
    },
    {//14
        0x0056,
        0,
        &dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
        0,
        0x0091,
        100,
        5,
        100,
        0,
        NULL,
        1,
    },
    {//15
        0x0065,
        0,
        &dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
        0,
        0x0066,
        100,
        5,
        100,
        15,
        NULL,
        1,
    },
    {//16
        0x0066,
        &dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
        &dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
        0x0065,
        0x0091,
        -5,
        1,
        90,
        5,
        NULL,
        1,
    },
    {//17
        0x0067,
        0,
        &dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
        0,
        0x0068,
        14500,
        500,
        14500,
        10500,
        &b_bat_num,
        1,
    },
    {//18
        0x0068,
        &dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
        &dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
        0x0067,
        0x0044,
        -500,
        0,
        14500,
        10500,
        &b_bat_num,
        1,
    },
/*    {//19
        0x006B,
        &dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
        0,
        0x006C,
        0,
        -10,
        0,
        100,
        0,
        NULL,
        1,
    },*/
    {//20
        0x006C,
        0,
        &dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
        0,
        0x006B,
        100,
        10,
        100,
        0,
        NULL,
        1,
    },
    /*{//21
        0x0070,
        &dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
        &dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
        0x0041,
        0x0043,
        -100,
        100,
        12500,
        10000,
        &b_bat_num,
        1,
    },*/
    {//23
        0x0063,
        0,
        &dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
        0,
        0x0064,
        5000,
        100,
        5000,
        200,
        NULL,
        1,
    },
    {//24
        0x0064,
        &dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
        0,
        0x0063,
        0,
        -100,
        100,
        4900,
        100,
        NULL,
        1,
    },
};

#define cb_max_ck_inverter_set_range_info_num             3
const device_set_range_info st_ck_inverter_set_range_info[cb_max_ck_inverter_set_range_info_num] =
{

//    {//1
//        0x004E,
//        0,
//        &dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
//        0,
//        0x0043,
//        14500,
//        100,
//        14500,
//        11000,
//        NULL,
//        1,
//    },
    {//3
        0x0042,
        &dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
        0,
        0x0043,
        0,
        -100,
        9000,
        11000,
        9000,
        NULL,
        1,
    },
//    {//5
//        0x0093,
//        0,
//        &dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
//        0,
//        0x0091,
//        99,
//        1,
//        99,
//        30,
//        NULL,
//        1,
//    },
    {//6
        0x00B1,
        &dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
        0,
        0x00B2,
        0,
        -10,
        110,
        350,
        110,
        NULL,
        10,
    }, 
    {//7
        0x00B2,
        0,
        &dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
        0,
        0x00B1,
        450,
        10,
        450,
        150,
        NULL,
        10,
    },    

};

//#define cb_max_kinergy_set_range_info_num                   2
//const device_set_range_info st_kinergy_set_range_info[cb_max_kinergy_set_range_info_num] =
//{
//    {//1
//        0x0035,
//        &dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
//        0,
//        0x0036,
//        0,
//        -1,
//        0,
//        0,
//        NULL,
//        1,
//    },
//    {//2
//        0x0036,
//        0,
//        &dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
//        0,
//        0x0035,
//        100,
//        1,
//        0,
//        NULL,
//        1,
//    },
//};

#define cb_riiosunii_cklcd_para_max             17
const cklcd_ver_para st_riiosunii_cklcd_para[cb_riiosunii_cklcd_para_max] =
{
    {1,7,30,24,32,70,80,},
    {1,8,30,24,50,70,80,},
    {5,7,30,48,32,35,60,},
    {5,8,30,48,50,35,60,},
    {6,7,40,48,32,50,60,},
    {6,8,40,48,50,50,60,},
    {7,7,60,48,50,70,100,},
    {8,7,80,48,50,90,100,},
    {9,7,10,48,16,10,30,},
    {9,8,10,48,32,10,30,},
    {10,7,50,48,50,60,100,},
    {11,7,15,24,16,35,60,},
    {12,7,10,12,16,40,60},
    {13,7,20,24,32,40,80,},
    {13,8,20,24,50,40,80,},
    {14,7,20,48,20,20,40,},
    {14,8,20,48,50,20,40,},
};
#define cb_ck_ii_cklcd_para_max             7
const cklcd_ver_para st_ck_ii_cklcd_para[cb_ck_ii_cklcd_para_max] =
{
    {1,7,30,24,32,80,},
    {3,7,50,24,50,150,},
    {5,7,30,48,32,40,},
    {8,7,80,48,50,110,},
    {10,7,50,48,50,70,},
    {13,7,20,24,32,50,},
    {14,7,20,48,32,25,},
};
#define cb_tyrann_cklcd_para_max             2
const cklcd_ver_para st_tyrann_cklcd_para[cb_tyrann_cklcd_para_max] =
{
    {2,7,100,48,100,140,},
    {4,7,150,48,100,200,},
};
#define cb_matrix_ii_cklcd_para_max             4
const cklcd_ver_para st_matrix_ii_cklcd_para[cb_matrix_ii_cklcd_para_max] =
{
	{2,7,100,48,100,140,},
	{4,7,150,48,100,200,300,},
	{8,7,80,48,50,110,150,},
	{10,7,50,48,50,70,120,},
};
#define cb_matrix_ii_lite_cklcd_para_max             1
const cklcd_ver_para st_matrix_ii_lite_cklcd_para[cb_matrix_ii_lite_cklcd_para_max] =
{
	{10,7,50,48,50,70,100,},
};


uint8_t check_set_data_value(uint32_t dw_device_id,
                             uint16_t w_set_addr,
                             uint32_t dw_set_value,
                             uint32_t *pdw_set_value_out)
{
    if(cdw_device_type_cklcd == (dw_device_id & 0xFFFF00))
    {
        for(uint8_t b_cnt_i = 0;b_cnt_i < cb_max_cklcd_set_range_info_num;b_cnt_i++)
        {
            if(w_set_addr == st_cklcd_set_range_info[b_cnt_i].w_net_addr)
            {
                if((dw_set_value > st_cklcd_set_range_info[b_cnt_i].w_max_local_value) &&\
                   (NULL != st_cklcd_set_range_info[b_cnt_i].pb_rate))
                {
                    *pdw_set_value_out = dw_set_value / (*st_cklcd_set_range_info[b_cnt_i].pb_rate);
                    return TRUE;
                }
                else
                {
                    return FALSE;
                }
            }
        }
    }
    else if(cdw_device_type_ck_inverter == (dw_device_id & 0xFFFF00))
    {
        for(uint8_t b_cnt_i = 0;b_cnt_i < cb_max_ck_inverter_set_range_info_num;b_cnt_i++)
        {
            if(w_set_addr == st_ck_inverter_set_range_info[b_cnt_i].w_net_addr)
            {
                if((dw_set_value > st_ck_inverter_set_range_info[b_cnt_i].w_max_local_value) &&\
                   (NULL != st_ck_inverter_set_range_info[b_cnt_i].pb_rate))
                {
                    *pdw_set_value_out = dw_set_value / (*st_ck_inverter_set_range_info[b_cnt_i].pb_rate);
                    return TRUE;
                }
                else
                {
                    return FALSE;
                }
            }
        }
    }
    return FALSE;
}

uint8_t device_set_attribute_range_set(int32_t dw_max_data,
                                       int32_t dw_min_data,
                                       uint32_t dw_step,
                                       net_data_set_attribute *pst_net_data_set_attribute)
{
    if(NULL == pst_net_data_set_attribute)
    {
        return FALSE;
    }
//    if(dw_max_data <= dw_min_data) //2023.08.22 
    if(dw_max_data < dw_min_data)
    {
        if(dw_min_data == 0)
        {
            //dw_max_data = dw_min_data + dw_step;
            dw_max_data = dw_min_data;
        }
        else
        {
//            dw_min_data = dw_max_data - dw_step;
			dw_min_data = dw_max_data;
        }
    }

    if((pst_net_data_set_attribute->dw_max_data != dw_max_data) ||\
       (pst_net_data_set_attribute->dw_min_data != dw_min_data) ||\
       (pst_net_data_set_attribute->dw_step != dw_step))
    {
//    	if(st_device_config_sync_info.b_sync_status != 0)
//    	{
			pst_net_data_set_attribute->f_modify = TRUE;
//		}
        pst_net_data_set_attribute->dw_max_data = dw_max_data;
        pst_net_data_set_attribute->dw_min_data = dw_min_data;
        pst_net_data_set_attribute->dw_step = dw_step;
        return TRUE;
    }
    return FALSE;
}

uint8_t device_set_attribute_range_common_set(net_device_info *pst_net_device_info,
                                              uint16_t w_net_addr,
                                              uint32_t *pdw_max_value_device_id,
                                              uint32_t *pdw_min_value_device_id,
                                              uint16_t w_max_value_net_addr,
                                              uint16_t w_min_value_net_addr,
                                              int32_t dw_max_value_increment,
                                              int32_t dw_min_value_increment,
                                              int32_t dw_max_local_value,
                                              int32_t dw_min_local_value,
                                              int8_t b_rate,
                                              uint32_t dw_step)
{
    if(NULL == pst_net_device_info)
    {
        return FALSE;
    }

    const net_data_info *pst_data_info = 0;
    uint16_t w_data_len = 2;
    uint32_t dw_data_temp_1 = 0;
    uint32_t dw_data_temp_2 = 0;
    if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                         w_net_addr,
                                         (const net_data_info ** const)&pst_data_info,
                                         pst_net_device_info))
    {
        return FALSE;
    }

    if(NULL != pdw_max_value_device_id)
    {
        net_device_data_access(*pdw_max_value_device_id,
                               cb_net_data_att_set,
                               w_max_value_net_addr,
                               &w_data_len,
                               (uint8_t *)&dw_data_temp_1,
                               cb_device_data_access_operate_type_read);
        check_set_data_value(*pdw_max_value_device_id,
                             w_max_value_net_addr,
                             dw_data_temp_1,
                             &dw_data_temp_1);
    }
    dw_data_temp_1 =\
        (((dw_data_temp_1 + dw_max_value_increment) < dw_max_local_value)?(dw_data_temp_1 + dw_max_value_increment):dw_max_local_value);

    if(NULL != pdw_min_value_device_id)
    {
        net_device_data_access(*pdw_min_value_device_id,
                               cb_net_data_att_set,
                               w_min_value_net_addr,
                               &w_data_len,
                               (uint8_t *)&dw_data_temp_2,
                               cb_device_data_access_operate_type_read);
        check_set_data_value(*pdw_min_value_device_id,
                             w_min_value_net_addr,
                             dw_data_temp_2,
                             &dw_data_temp_2);
    }
    dw_data_temp_2 =\
        (((dw_data_temp_2 + dw_min_value_increment) > dw_min_local_value)?(dw_data_temp_2 + dw_min_value_increment):dw_min_local_value);

    device_set_attribute_range_set((b_rate * dw_data_temp_1),
                                   (b_rate * dw_data_temp_2),
                                   dw_step,
                                   pst_data_info->pst_net_data_set_attribute);

    return TRUE;
}

void device_set_value_rate(net_device_info *pst_net_device_info,
                           uint16_t w_data_addr,
                           uint16_t w_max_local_value,
                           int8_t b_rate,
                           uint8_t f_sign)
{
    if(NULL == pst_net_device_info)
    {
        return;
    }

    uint32_t dw_data_temp = 0;
    uint16_t w_data_len = 2;

    net_device_data_access(*pst_net_device_info->pdw_device_id,
                           cb_net_data_att_set,
                           w_data_addr,
                           &w_data_len,
                           (uint8_t *)&dw_data_temp,
                           cb_device_data_access_operate_type_read);

    if(f_sign)
    {
        if((int32_t)dw_data_temp <= (int16_t)w_max_local_value)
        {
            dw_data_temp *= b_rate;
        }
    }
    else
    {
        if(dw_data_temp <= w_max_local_value)
        {
            dw_data_temp *= b_rate;
        }
    }
    net_device_data_access(*pst_net_device_info->pdw_device_id,
                           cb_net_data_att_set,
                           w_data_addr,
                           &w_data_len,
                           (uint8_t *)&dw_data_temp,
                           cb_device_data_access_operate_type_write);

}

void device_set_attribute_range_manage()
{
    const net_data_info *pst_data_info = 0;
    net_device_info *pst_net_device_info = 0;
    uint16_t w_data_temp_1 = 0;
    uint16_t w_data_temp_2 = 0;
    uint16_t w_data_len = 1;

#if( task_runing_status_print )
    DebugPrintf(TRUE,"Device_set_range_manage_start\r\n");
#endif

    uint8_t b_inv_num = 0;
	uint8_t b_test_mode = 0;
	uint8_t b_test_hd = 0;
	uint8_t b_test_md = 0;
    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                           cb_net_data_att_status,
                           0x0160,
                           &w_data_len,
                           (uint8_t *)&b_inv_num,
                           cb_device_data_access_operate_type_read);
	net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                           cb_net_data_att_set,
                           0x003F,
                           &w_data_len,
                           (uint8_t *)&b_test_mode,
                           cb_device_data_access_operate_type_read);
	net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                           cb_net_data_att_set,
                           0x0040,
                           &w_data_len,
                           (uint8_t *)&b_test_hd,
                           cb_device_data_access_operate_type_read);
	net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                           cb_net_data_att_set,
                           0x0041,
                           &w_data_len,
                           (uint8_t *)&b_test_md,
                           cb_device_data_access_operate_type_read);
	if(b_test_mode != 0x33)
	{
		b_test_mode = 0;
	}
    //Model_Version判断 电池数量
    uint16_t w_inverter_version = 0;
	uint8_t b_inverter_hd_version = 0;
	uint8_t b_inverter_model_version = 0;
	uint8_t b_pro_type = 0;
    w_data_len = 2;
    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                           cb_net_data_att_status,
                           0x012C,
                           &w_data_len,
                           (uint8_t *)&w_inverter_version,
                           cb_device_data_access_operate_type_read);//hardware ver
//    b_inverter_hd_version = (uint8_t)(w_inverter_version >> 8);
//    b_inverter_model_version = (uint8_t)w_inverter_version;
	if(0x33 == b_test_mode)
	{
	    b_inverter_hd_version = b_test_hd;
	    b_inverter_model_version = b_test_md;
	}
	else
	{
	    b_inverter_hd_version = (uint8_t)(w_inverter_version >> 8);
	    b_inverter_model_version = (uint8_t)w_inverter_version;

	}
	{
		if(check_system_type(cb_inv_type_riio_sun_ii) 
			|| check_system_type(cb_inv_type_riio_sun_ii_gte) 
			|| check_system_type(cb_inv_type_riio_sun_ii_split_phase)
			|| check_system_type(cb_inv_type_riio_ii_split_phase)
			|| check_system_type(cb_inv_type_riio_ii))
		{
			for(uint8_t b_cnt_i = 0;b_cnt_i < cb_riiosunii_cklcd_para_max;b_cnt_i++)
			{
				if((b_inverter_model_version == st_riiosunii_cklcd_para[b_cnt_i].b_model_ver))
				{
					b_bat_num = st_riiosunii_cklcd_para[b_cnt_i].b_bat_voltage_level / 12;			// 80 = 0.8 * 1000 / 10						
					break;
				}
			}
		}
		else if(check_system_type(cb_inv_type_ck_ii) || check_system_type(cb_inv_type_Tyrann_3_5_8k))
		{
			for(uint8_t b_cnt_i = 0;b_cnt_i < cb_ck_ii_cklcd_para_max;b_cnt_i++)
			{
				if((b_inverter_model_version == st_ck_ii_cklcd_para[b_cnt_i].b_model_ver))
				{
					b_bat_num = st_ck_ii_cklcd_para[b_cnt_i].b_bat_voltage_level / 12;			// 80 = 0.8 * 1000 / 10						
					break;

				}
			}
		}
		else if(check_system_type(cb_inv_type_tyrann))
		{
			for(uint8_t b_cnt_i = 0;b_cnt_i < cb_tyrann_cklcd_para_max;b_cnt_i++)
			{
				if((b_inverter_model_version == st_tyrann_cklcd_para[b_cnt_i].b_model_ver))
				{
					b_bat_num = st_tyrann_cklcd_para[b_cnt_i].b_bat_voltage_level / 12;			// 80 = 0.8 * 1000 / 10						
					break;

				}
			}
		}
		else if(check_system_type(cb_inv_type_apollo_matric_ii_10_15k) || check_system_type(cb_inv_type_apollo_matric_ii_5_8k))
		{
			for(uint8_t b_cnt_i = 0;b_cnt_i < cb_matrix_ii_cklcd_para_max;b_cnt_i++)
			{
				if((b_inverter_model_version == st_matrix_ii_cklcd_para[b_cnt_i].b_model_ver))
				{
					b_bat_num = st_matrix_ii_cklcd_para[b_cnt_i].b_bat_voltage_level / 12;			// 80 = 0.8 * 1000 / 10						
					break;

				}
			}
		}
		else if(check_system_type(cb_inv_type_apollo_matric_ii_lite))
		{
			for(uint8_t b_cnt_i = 0;b_cnt_i < cb_matrix_ii_lite_cklcd_para_max;b_cnt_i++)
			{
				if((b_inverter_model_version == st_matrix_ii_lite_cklcd_para[b_cnt_i].b_model_ver))
				{
					b_bat_num = st_matrix_ii_lite_cklcd_para[b_cnt_i].b_bat_voltage_level / 12;			// 80 = 0.8 * 1000 / 10						
					break;
				}
			}
		}
	}
	w_data_len = 1;
	net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                       cb_net_data_att_set,
                       0x0038,
                       &w_data_len,
                       (uint8_t *)&b_bat_num,
                       cb_device_data_access_operate_type_write);//bat num

    {   //cklcd
        if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
                                        (net_device_info **)&pst_net_device_info,
                                        &st_net_system_info_list))
        {
            return ;
        }
        if(cb_net_device_add == pst_net_device_info->pst_net_device_status->b_communicate_status)
        {
            for(uint8_t b_cnt_i = 0;b_cnt_i < cb_max_cklcd_set_range_info_num;b_cnt_i++)
            {
                if(NULL == st_cklcd_set_range_info[b_cnt_i].pb_rate)
                {
                    w_data_temp_1 = 1;
                }
                else
                {
                    w_data_temp_1 = *st_cklcd_set_range_info[b_cnt_i].pb_rate;
                }
                device_set_attribute_range_common_set(pst_net_device_info,
                                                      st_cklcd_set_range_info[b_cnt_i].w_net_addr,
                                                      st_cklcd_set_range_info[b_cnt_i].pdw_max_value_device_id,
                                                      st_cklcd_set_range_info[b_cnt_i].pdw_min_value_device_id,
                                                      st_cklcd_set_range_info[b_cnt_i].w_max_value_net_addr,
                                                      st_cklcd_set_range_info[b_cnt_i].w_min_value_net_addr,
                                                      st_cklcd_set_range_info[b_cnt_i].dw_max_value_increment,
                                                      st_cklcd_set_range_info[b_cnt_i].dw_min_value_increment,
                                                      (int32_t)st_cklcd_set_range_info[b_cnt_i].w_max_local_value,
                                                      (int32_t)st_cklcd_set_range_info[b_cnt_i].w_min_local_value,
                                                      (int8_t)w_data_temp_1,
                                                      st_cklcd_set_range_info[b_cnt_i].dw_step);
                device_set_value_rate(pst_net_device_info,
                                      st_cklcd_set_range_info[b_cnt_i].w_net_addr,
                                      st_cklcd_set_range_info[b_cnt_i].w_max_local_value,
                                      (int8_t)w_data_temp_1,
                                      FALSE);
            }
            {//0001
                if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                     0x0001,
                                                     (const net_data_info ** const)&pst_data_info,
                                                     pst_net_device_info))
                {
                    return ;
                }
				w_data_len = 2;
				net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
                                       cb_net_data_att_set,
                                       0x0000,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_1,
                                       cb_device_data_access_operate_type_read);
				if(check_system_type(cb_inv_type_apollo_matric_ii_10_15k))
				{
				 device_set_attribute_range_set(6,
												2,
												1,
												pst_data_info->pst_net_data_set_attribute);
				}
                else if(check_all_in_one(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1]) || check_system_type(cb_inv_type_apollo_matric_ii_lite)|| check_system_type(cb_inv_type_apollo_matric_ii_5_8k))
                {
                    device_set_attribute_range_set(6,
                                                   1,
                                                   1,
                                                   pst_data_info->pst_net_data_set_attribute);
                }
				else
                {
                    device_set_attribute_range_set(6,
                                                   0,
                                                   1,
                                                   pst_data_info->pst_net_data_set_attribute);
                }
            }
            {//0051     005C//ACin_Chg_Max_Current
                uint16_t w_data_temp_3 = 50;
//                w_data_temp_1 = (uint8_t)w_inverter_version;
////                w_data_temp_2 = (uint8_t)(w_inverter_version >> 8);
				if(check_system_type(cb_inv_type_ck_ii) || check_system_type(cb_inv_type_Tyrann_3_5_8k))
				{
					
					for(uint8_t b_cnt_i = 0;b_cnt_i < cb_ck_ii_cklcd_para_max;b_cnt_i++)
	                {
	                    if((b_inverter_model_version == st_ck_ii_cklcd_para[b_cnt_i].b_model_ver))
	                    {
	                        w_data_temp_3 = st_ck_ii_cklcd_para[b_cnt_i].w_ac_current_in_max;
	                        break;
	                    }
	                }
				}
				else if(check_system_type(cb_inv_type_riio_sun_ii) 
					|| check_system_type(cb_inv_type_riio_sun_ii_gte) 
					|| check_system_type(cb_inv_type_riio_sun_ii_split_phase) 
					|| check_system_type(cb_inv_type_riio_ii_split_phase)
					|| check_system_type(cb_inv_type_riio_ii))
				{
					 for(uint8_t b_cnt_i = 0;b_cnt_i < cb_riiosunii_cklcd_para_max;b_cnt_i++)
	                {
	                    if((b_inverter_model_version == st_riiosunii_cklcd_para[b_cnt_i].b_model_ver))
	                    {
	                        w_data_temp_3 = st_riiosunii_cklcd_para[b_cnt_i].w_ac_current_in_max;
	                        break;
	                    }
	                }
				}
				else if(check_system_type(cb_inv_type_tyrann))
				{
					 for(uint8_t b_cnt_i = 0;b_cnt_i < cb_tyrann_cklcd_para_max;b_cnt_i++)
	                {
	                    if((b_inverter_model_version == st_tyrann_cklcd_para[b_cnt_i].b_model_ver))
	                    {
	                        w_data_temp_3 = st_tyrann_cklcd_para[b_cnt_i].w_ac_current_in_max;
	                        break;
	                    }
	                }
				}
				else if(check_system_type(cb_inv_type_apollo_matric_ii_10_15k) || check_system_type(cb_inv_type_apollo_matric_ii_5_8k))
				{
					 for(uint8_t b_cnt_i = 0;b_cnt_i < cb_matrix_ii_cklcd_para_max;b_cnt_i++)
	                {
	                    if((b_inverter_model_version == st_matrix_ii_cklcd_para[b_cnt_i].b_model_ver))
	                    {
	                        w_data_temp_3 = st_matrix_ii_cklcd_para[b_cnt_i].w_ac_current_in_max;
	                        break;
	                    }
	                }
				}
				else if(check_system_type(cb_inv_type_apollo_matric_ii_lite))
				{
					 for(uint8_t b_cnt_i = 0;b_cnt_i < cb_matrix_ii_lite_cklcd_para_max;b_cnt_i++)
	                {
	                    if((b_inverter_model_version == st_matrix_ii_lite_cklcd_para[b_cnt_i].b_model_ver))
	                    {
	                        w_data_temp_3 = st_matrix_ii_lite_cklcd_para[b_cnt_i].w_ac_current_in_max;
	                        break;
	                    }
	                }
				}
                if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                     0x0051,
                                                     (const net_data_info ** const)&pst_data_info,
                                                     pst_net_device_info))
                {
                    return ;
                }

                device_set_attribute_range_set(w_data_temp_3,
                                               0,
                                               1,
                                               pst_data_info->pst_net_data_set_attribute);

                if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                     0x005C,
                                                     (const net_data_info ** const)&pst_data_info,
                                                     pst_net_device_info))
                {
                    return ;
                }

                device_set_attribute_range_set(w_data_temp_3,
                                               0,
                                               1,
                                               pst_data_info->pst_net_data_set_attribute);
				if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                     0x005E,
                                                     (const net_data_info ** const)&pst_data_info,
                                                     pst_net_device_info))
                {
                    return ;
                }

                device_set_attribute_range_set(w_data_temp_3,
                                               1,
                                               1,
                                               pst_data_info->pst_net_data_set_attribute);
            }

			{//006B
				if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                     0x006B,
                                                     (const net_data_info ** const)&pst_data_info,
                                                     pst_net_device_info))
                {
                    return ;
                }
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
                                       cb_net_data_att_set,
                                       0x006C,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_1,
                                       cb_device_data_access_operate_type_read);
				w_data_temp_1 = w_data_temp_1 - 10;
				if(check_system_type(cb_inv_type_ck_ii) || check_system_type(cb_inv_type_tyrann) || check_system_type(cb_inv_type_apollo_matric_ii_10_15k) || check_system_type(cb_inv_type_apollo_matric_ii_lite) || check_system_type(cb_inv_type_apollo_matric_ii_5_8k) || check_system_type(cb_inv_type_Tyrann_3_5_8k))
				{
					w_data_temp_2 = 5;
				}
				else
				{
					w_data_temp_2 = 0;
				}
				
                device_set_attribute_range_set(w_data_temp_1,							//max
                                               w_data_temp_2,							//min
                                               1,
                                               pst_data_info->pst_net_data_set_attribute);
			}
			{//0070
                uint16_t w_data_temp_3 = 0;
				uint16_t w_bat_lv_alarm = 0;
				if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                     0x0070,
                                                     (const net_data_info ** const)&pst_data_info,
                                                     pst_net_device_info))
                {
                    return ;
                }
//                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
//                                       cb_net_data_att_set,
//                                       0x0043,
//                                       &w_data_len,
//                                       (uint8_t *)&w_data_temp_1,				//低压保护
//                                       cb_device_data_access_operate_type_read);
				net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                       cb_net_data_att_set,
                                       0x0044,
                                       &w_data_len,
                                       (uint8_t *)&w_bat_lv_alarm,				//低压告警
                                       cb_device_data_access_operate_type_read);
				net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                       cb_net_data_att_set,
                                       0x0041,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_2,				//浮充电压
                                       cb_device_data_access_operate_type_read);
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
                                       cb_net_data_att_set,
                                       0x0071,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_3,
                                       cb_device_data_access_operate_type_read);

				w_data_temp_1 = (((w_bat_lv_alarm + 100)  > 10000) ? (w_bat_lv_alarm + 100) : (10000));
				//w_data_temp_2 = ((w_data_temp_2  < 12500) ? (w_data_temp_2) : (12500)) * b_bat_num;
				w_data_temp_2 = get_min_for_uint(3, (w_data_temp_2 - 100), (w_data_temp_3 - 500), 13900);							
                device_set_attribute_range_set(w_data_temp_2,
                                               w_data_temp_1,
                                               1,
                                               pst_data_info->pst_net_data_set_attribute);
			}
			
            {//0071
				if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                     0x0071,
                                                     (const net_data_info ** const)&pst_data_info,
                                                     pst_net_device_info))
                {
                    return ;
                }
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                       cb_net_data_att_set,
                                       0x0040,										//absorbtion voltage
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_1,
                                       cb_device_data_access_operate_type_read);
				net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
                                       cb_net_data_att_set,
                                       0x0070,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_2,
                                       cb_device_data_access_operate_type_read);
				w_data_temp_1 = (((w_data_temp_1 - 100)  < 14400) ? (w_data_temp_1 - 100) : (14400));
				w_data_temp_2 = (((w_data_temp_2 + 500) > 12000 ) ? (w_data_temp_2 + 500) : 12000);

				//w_data_temp_2 = 13000 * b_bat_num;
                device_set_attribute_range_set(w_data_temp_1,			//MAX
                                               w_data_temp_2,
                                               1,
                                               pst_data_info->pst_net_data_set_attribute);
			}
            {//0072
            	uint16_t w_data_temp_3 = 0;
				uint16_t w_soc_low_alarm = 0;
                if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                     0x0072,
                                                     (const net_data_info ** const)&pst_data_info,
                                                     pst_net_device_info))
                {
                    return ;
                }

                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
                                       cb_net_data_att_set,
                                       0x0020,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_1,
                                       cb_device_data_access_operate_type_read);
				net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                        cb_net_data_att_set,
                                        0x005D,
                                        &w_data_len,
                                        (uint8_t *)&w_data_temp_3,
                                        cb_device_data_access_operate_type_read);

				if(check_system_type(cb_inv_type_tyrann) || check_system_type(cb_inv_type_Tyrann_3_5_8k) || check_system_type(cb_inv_type_ck_ii) || check_system_type(cb_inv_type_apollo_matric_ii_10_15k) || check_system_type(cb_inv_type_apollo_matric_ii_lite) || check_system_type(cb_inv_type_apollo_matric_ii_5_8k))
				{
					if(w_data_temp_3 == 1)
					{
						w_data_temp_2 = 60;
					}
					else if(w_data_temp_3 == 0)
					{
						w_data_temp_2 = 80;
					}
				}
				else
				{
					if(2 == w_data_temp_1)
                    {
                        w_data_temp_2 = 60;
                    }
                    else if(3 == w_data_temp_1)
                    {
                        w_data_temp_2 = 90;
                    }
                    else
                    {
                        w_data_temp_2 = 80;
                    }
				}
                

                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                       cb_net_data_att_set,
                                       0x0092,														//SOC_Low_Protect
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_1,
                                       cb_device_data_access_operate_type_read);
				net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                       cb_net_data_att_set,
                                       0x0091,														//SOC_Low_Warning/SOC_Low_alarm
                                       &w_data_len,
                                       (uint8_t *)&w_soc_low_alarm,
                                       cb_device_data_access_operate_type_read);
				w_data_temp_1 = w_soc_low_alarm;
                if((w_data_temp_1 + 1) < 15)
                {
                    w_data_temp_1 = 15;
                }
                else
                {
                    w_data_temp_1 += 1;
                }
                device_set_attribute_range_set(w_data_temp_2,
                                               w_data_temp_1,
                                               1,
                                               pst_data_info->pst_net_data_set_attribute);
            }
            {//00EE
            	uint16_t w_feedback_power = 0;
                if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                     0x00EE,
                                                     (const net_data_info ** const)&pst_data_info,
                                                     pst_net_device_info))
                {
                    return ;
                }
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                       cb_net_data_att_status,
                                       0x012C,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_1,
                                       cb_device_data_access_operate_type_read);//hardware ver
                w_data_temp_1 &= 0x00ff;
				w_data_temp_1 = (uint8_t)w_inverter_version;
                if(check_system_type(cb_inv_type_riio_sun_ii) 
					|| check_system_type(cb_inv_type_riio_sun_ii_gte) 
					|| check_system_type(cb_inv_type_riio_sun_ii_split_phase)  
					|| check_system_type(cb_inv_type_riio_ii_split_phase)
					|| check_system_type(cb_inv_type_riio_ii))
				{
					for(uint8_t b_cnt_i = 0;b_cnt_i < cb_riiosunii_cklcd_para_max;b_cnt_i++)
	                {
	                    if((b_inverter_model_version == st_riiosunii_cklcd_para[b_cnt_i].b_model_ver))
	                    {
	                    	if(b_inverter_model_version == 5 || b_inverter_model_version == 1)
	                    	{
								w_feedback_power = 2500;				//RiiO SUN II 3.0s 馈网功率为2500w
							}
							else if(b_inverter_model_version == 13 || b_inverter_model_version == 14)
	                    	{
								w_feedback_power = 1500;				//RiiO SUN II 2.0 馈网功率为1500w
							}
							else
							{
								w_feedback_power = st_riiosunii_cklcd_para[b_cnt_i].b_rate_power * 80;			// 80 = 0.8 * 1000 / 10
							}	                        
	                        break;
	                    }
	                }
				}
				else if(check_system_type(cb_inv_type_ck_ii) || check_system_type(cb_inv_type_Tyrann_3_5_8k))
				{
					for(uint8_t b_cnt_i = 0;b_cnt_i < cb_ck_ii_cklcd_para_max;b_cnt_i++)
	                {
	                    if((b_inverter_model_version == st_ck_ii_cklcd_para[b_cnt_i].b_model_ver))
	                    {
							w_feedback_power = st_ck_ii_cklcd_para[b_cnt_i].b_rate_power * 80;			// 80 = 0.8 * 1000 / 10	                        
	                        break;
	                    }
	                }
				}
				else if(check_system_type(cb_inv_type_tyrann))
				{
					for(uint8_t b_cnt_i = 0;b_cnt_i < cb_tyrann_cklcd_para_max;b_cnt_i++)
	                {
	                    if((b_inverter_model_version == st_tyrann_cklcd_para[b_cnt_i].b_model_ver))
	                    {
							w_feedback_power = st_tyrann_cklcd_para[b_cnt_i].b_rate_power * 80;			// 80 = 0.8 * 1000 / 10	                        
	                        break;
	                    }
	                }
				}
				else if(check_system_type(cb_inv_type_apollo_matric_ii_10_15k) || check_system_type(cb_inv_type_apollo_matric_ii_5_8k))
				{
					for(uint8_t b_cnt_i = 0;b_cnt_i < cb_matrix_ii_cklcd_para_max;b_cnt_i++)
	                {
	                    if((b_inverter_model_version == st_matrix_ii_cklcd_para[b_cnt_i].b_model_ver))
	                    {
							w_feedback_power = st_matrix_ii_cklcd_para[b_cnt_i].b_rate_power * 80;			// 80 = 0.8 * 1000 / 10	                        
	                        break;
	                    }
	                }
				}
				else if(check_system_type(cb_inv_type_apollo_matric_ii_lite))
				{
					for(uint8_t b_cnt_i = 0;b_cnt_i < cb_matrix_ii_lite_cklcd_para_max;b_cnt_i++)
	                {
	                    if((b_inverter_model_version == st_matrix_ii_lite_cklcd_para[b_cnt_i].b_model_ver))
	                    {
							w_feedback_power = st_matrix_ii_lite_cklcd_para[b_cnt_i].b_rate_power * 80;			// 80 = 0.8 * 1000 / 10	                        
	                        break;
	                    }
	                }
				}
				device_set_attribute_range_set(w_feedback_power,
											   0,
											   1,
											   pst_data_info->pst_net_data_set_attribute);

            }
        }
    }
    {   //ck inverter
        if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                        (net_device_info **)&pst_net_device_info,
                                        &st_net_system_info_list))
        {
            return ;
        }
        if(cb_net_device_add == pst_net_device_info->pst_net_device_status->b_communicate_status)
        {
            uint8_t f_hv = FALSE;

            if(b_inverter_hd_version > 7)
            {
                f_hv = FALSE;
            }
            else
            {
                f_hv = TRUE;
            }

            for(uint8_t b_cnt_i = 0;b_cnt_i < cb_max_ck_inverter_set_range_info_num;b_cnt_i++)
            {
                if(NULL == st_ck_inverter_set_range_info[b_cnt_i].pb_rate)
                {
                    w_data_temp_1 = 1;
                }
                else
                {
                    w_data_temp_1 = *st_ck_inverter_set_range_info[b_cnt_i].pb_rate;
                }
                device_set_attribute_range_common_set(pst_net_device_info,
                                                      st_ck_inverter_set_range_info[b_cnt_i].w_net_addr,
                                                      st_ck_inverter_set_range_info[b_cnt_i].pdw_max_value_device_id,
                                                      st_ck_inverter_set_range_info[b_cnt_i].pdw_min_value_device_id,
                                                      st_ck_inverter_set_range_info[b_cnt_i].w_max_value_net_addr,
                                                      st_ck_inverter_set_range_info[b_cnt_i].w_min_value_net_addr,
                                                      st_ck_inverter_set_range_info[b_cnt_i].dw_max_value_increment,
                                                      st_ck_inverter_set_range_info[b_cnt_i].dw_min_value_increment,
                                                      (int32_t)st_ck_inverter_set_range_info[b_cnt_i].w_max_local_value,
                                                      (int32_t)st_ck_inverter_set_range_info[b_cnt_i].w_min_local_value,
                                                      (int8_t)w_data_temp_1,
                                                      st_ck_inverter_set_range_info[b_cnt_i].dw_step);
                device_set_value_rate(pst_net_device_info,
                                      st_ck_inverter_set_range_info[b_cnt_i].w_net_addr,
                                      st_ck_inverter_set_range_info[b_cnt_i].w_max_local_value,
                                      (int8_t)w_data_temp_1,
                                      FALSE);
            }
			{//0032
				if(FALSE == net_get_device_data_info(cb_net_data_att_set,
													 0x0032,
													 (const net_data_info ** const)&pst_data_info,
													 pst_net_device_info))
				{
					return ;
				}
				uint16_t w_inv_output_volt_max = 2400;
				uint16_t w_inv_output_volt_min = 2200;
				if(FALSE == f_hv)
				{
					w_inv_output_volt_max = 1270 * 2;	
					w_inv_output_volt_min = 1100 * 2;
				}
				device_set_attribute_range_set(w_inv_output_volt_max,
											   w_inv_output_volt_min,
											   1,
											   pst_data_info->pst_net_data_set_attribute);
				
			}

			{//0035
                if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                     0x0035,
                                                     (const net_data_info ** const)&pst_data_info,
                                                     pst_net_device_info))
                {
                    return ;
                }
				uint16_t w_feedback_power = 0;
				{
					if(check_system_type(cb_inv_type_riio_sun_ii) 
						|| check_system_type(cb_inv_type_riio_sun_ii_gte) 
						|| check_system_type(cb_inv_type_riio_sun_ii_split_phase) 
						|| check_system_type(cb_inv_type_riio_ii_split_phase)
						|| check_system_type(cb_inv_type_riio_ii))
					{
						for(uint8_t b_cnt_i = 0;b_cnt_i < cb_riiosunii_cklcd_para_max;b_cnt_i++)
						{
							if((b_inverter_model_version == st_riiosunii_cklcd_para[b_cnt_i].b_model_ver))
							{
								if(b_inverter_model_version == 5)
								{
									w_feedback_power = 2500;				//RiiO SUN II 3.0s 馈网功率为2500w
								}
								else
								{
									w_feedback_power = st_riiosunii_cklcd_para[b_cnt_i].b_rate_power * 80;			// 80 = 0.8 * 1000 / 10
								}							
								break;
							}
						}
					}
					else if(check_system_type(cb_inv_type_ck_ii) || check_system_type(cb_inv_type_Tyrann_3_5_8k))
					{
						for(uint8_t b_cnt_i = 0;b_cnt_i < cb_ck_ii_cklcd_para_max;b_cnt_i++)
						{
							if((b_inverter_model_version == st_ck_ii_cklcd_para[b_cnt_i].b_model_ver))
							{
								if(b_inverter_model_version == 8)
								{
									w_feedback_power = 6500;				//CK-II 8.0S上限为6500W 
								}
								else
								{
									w_feedback_power = st_ck_ii_cklcd_para[b_cnt_i].b_rate_power * 80;			// 80 = 0.8 * 1000 / 10 	
								}
								break;
							}
						}
					}
					else if(check_system_type(cb_inv_type_tyrann))
					{
						for(uint8_t b_cnt_i = 0;b_cnt_i < cb_tyrann_cklcd_para_max;b_cnt_i++)
						{
							if((b_inverter_model_version == st_tyrann_cklcd_para[b_cnt_i].b_model_ver))
							{
								w_feedback_power = st_tyrann_cklcd_para[b_cnt_i].b_rate_power * 80; 		// 80 = 0.8 * 1000 / 10 						
								break;
							}
						}
					}
					else if(check_system_type(cb_inv_type_apollo_matric_ii_10_15k) || check_system_type(cb_inv_type_apollo_matric_ii_5_8k))
					{
						for(uint8_t b_cnt_i = 0;b_cnt_i < cb_matrix_ii_cklcd_para_max;b_cnt_i++)
						{
							if((b_inverter_model_version == st_matrix_ii_cklcd_para[b_cnt_i].b_model_ver))
							{
								w_feedback_power = st_matrix_ii_cklcd_para[b_cnt_i].b_rate_power * 80;			// 80 = 0.8 * 1000 / 10 						
								break;
							}
						}
					}
					else if(check_system_type(cb_inv_type_apollo_matric_ii_lite))
					{
						for(uint8_t b_cnt_i = 0;b_cnt_i < cb_matrix_ii_lite_cklcd_para_max;b_cnt_i++)
						{
							if((b_inverter_model_version == st_matrix_ii_lite_cklcd_para[b_cnt_i].b_model_ver))
							{
								w_feedback_power = st_matrix_ii_lite_cklcd_para[b_cnt_i].b_rate_power * 80; 		// 80 = 0.8 * 1000 / 10 						
								break;
							}
						}
					}
					device_set_attribute_range_set(w_feedback_power,
												   0,
												   1,
												   pst_data_info->pst_net_data_set_attribute);
				}
				
            }

            {//0046 ///max charge cur
                if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                     0x0046,
                                                     (const net_data_info ** const)&pst_data_info,
                                                     pst_net_device_info))
                {
                    return ;
                }

                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
                                       cb_net_data_att_set,
                                       0x0050,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_1,
                                       cb_device_data_access_operate_type_read);//
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                       cb_net_data_att_set,
                                       0x0048,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_2,
                                       cb_device_data_access_operate_type_read);// bat ah
                if(0 != w_data_temp_2)
                {
                    switch(w_data_temp_1)
                    {
                        case 0:
                        case 1:
                        case 3:
                        case 4:
                        {
                            w_data_temp_1 = 4;
                        }
                        break;
                        case 2:
                        {
                            w_data_temp_1 = 2;
                        }
                        break;
                        case 5:
                        default:
                        {
                            w_data_temp_1 = 1;
                        }
                        break;
                    }

                    if((w_data_temp_2 / w_data_temp_1) > 900)
                    {
                        w_data_temp_2 = 900;
                    }
                    else
                    {
                        w_data_temp_2 = w_data_temp_2 / w_data_temp_1;
                        w_data_temp_2 = (w_data_temp_2 / b_inv_num) * b_inv_num;
                    }
                    device_set_attribute_range_set(w_data_temp_2,
                                                   5 * b_inv_num,
                                                   1,
                                                   pst_data_info->pst_net_data_set_attribute);
                }
            }
            {//0040 0041
                uint16_t w_data_temp_3 = 0;
                uint16_t w_data_temp_4 = 0;
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
                                       cb_net_data_att_set,
                                       0x0050,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_1,
                                       cb_device_data_access_operate_type_read);//
                switch(w_data_temp_1)
                {
                    case 0:
                    case 1:
                    case 2:
                    default:
                        {
                            w_data_temp_1 = 13500;
                            w_data_temp_2 = 14500;
                            w_data_temp_3 = 13000;
                            w_data_temp_4 = 14000;
                        }
                        break;
                    case 3:
                    case 4:
                        {
                            w_data_temp_1 = 14000;
                            w_data_temp_2 = 15500;
                            w_data_temp_3 = 13000;
                            w_data_temp_4 = 14000;
                        }
                        break;
                    case 5:
                        {
                            w_data_temp_1 = 12000;
                            w_data_temp_2 = 14600;
                            w_data_temp_3 = 12000;
                            w_data_temp_4 = 14600;
                        }
                        break;
                }
                device_set_attribute_range_common_set(pst_net_device_info,
                                                      0x0040,
                                                      NULL,
                                                      &dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                                      0,
                                                      0x0041,
                                                      w_data_temp_2,
                                                      100,
                                                      (int32_t)w_data_temp_2,
                                                      (int32_t)w_data_temp_1,
                                                      (int8_t)1,
                                                      1);
                device_set_attribute_range_common_set(pst_net_device_info,
                                                      0x0041,
                                                      &dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                                      &dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                                      0x0040,
                                                      0x0044,
                                                      -100,
                                                      100,
                                                      (int32_t)w_data_temp_4,
                                                      (int32_t)w_data_temp_3,
                                                      (int8_t)1,
                                                      1);
            }
            {//0043
                uint16_t w_data_temp_3 = 0;
                uint16_t w_data_temp_4 = 0;
				uint16_t w_data_temp_5 = 0;
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                       cb_net_data_att_set,
                                       0x0044,									
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_1,
                                       cb_device_data_access_operate_type_read);//BAT_LV_WARN
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                       cb_net_data_att_set,
                                       0x004E,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_2,
                                       cb_device_data_access_operate_type_read);//LV_PRO_Recover
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
                                       cb_net_data_att_set,
                                       0x0070,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_5,
                                       cb_device_data_access_operate_type_read);//Voltage back to utility when”SBU” /”SB”/ “SUB”
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
                                       cb_net_data_att_set,
                                       0x0020,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_4,
                                       cb_device_data_access_operate_type_read);//ACin Logic
//                w_data_temp_3 = w_data_temp_5 / b_bat_num;
//				if(check_system_type(cb_inv_type_tyrann) || check_system_type(cb_inv_type_riio_sun_ii_split_phase) || check_system_type(cb_inv_type_riio_sun_ii))
//				{
					w_data_temp_3 = w_data_temp_5;
//				}
				if((TRUE == check_system_type(cb_inv_type_riio_sun_ii)
					|| check_system_type(cb_inv_type_riio_sun_ii_gte) 
					|| check_system_type(cb_inv_type_riio_ii)) && (w_data_temp_4 != 0))
				{//当ACin Logic选择SB、SBU、SUB、Time Control模式时（Tyrann/CK-II: ACin Logic选择Battery First mode、Time Control时）
					w_data_temp_2 = get_min_for_uint(4,
													 w_data_temp_1 - 100,
													 w_data_temp_2 - 100,
													 w_data_temp_3 - 100,
													 13500);
				}
				else//当ACin Logic选择USB模式(0x20 == 0)时（Tyrann/CK-II: ACin Logic选择ACin First mode(0x20 == 0)时）
				{
					w_data_temp_2 = ((w_data_temp_2 < w_data_temp_1)?(w_data_temp_2):(w_data_temp_1));
					w_data_temp_2 = ((w_data_temp_2 - 100) < 13500)?(w_data_temp_2 - 100):13500;
				}
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                       cb_net_data_att_set,
                                       0x0042,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_1,
                                       cb_device_data_access_operate_type_read);//U_DisCHG_End
                w_data_temp_1 = ((w_data_temp_1 + 100) > 9500)?(w_data_temp_1 + 100):9500;
                if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                     0x0043,
                                                     (const net_data_info ** const)&pst_data_info,
                                                     pst_net_device_info))
                {
                    return ;
                }
                device_set_attribute_range_set(w_data_temp_2,
                                               w_data_temp_1,
                                               1,
                                               pst_data_info->pst_net_data_set_attribute);
            }
			uint16_t w_acin_logic = 0;
			uint16_t w_bat_lv_protect = 0;
			uint16_t w_bat_float_charge = 0;
			uint16_t w_vol_back_to_utility = 0;
			uint16_t w_vol_back_to_battery = 0;
			uint16_t w_bat_lv_waring_max = 0;
			uint16_t w_bat_lv_waring_min = 0;
			uint16_t w_data_temp_0 = 0;
            {//0044 bat_low_voltage_waring
				w_data_len = 2;
			    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                       cb_net_data_att_set,
                                       0x0041,									
                                       &w_data_len,
                                       (uint8_t *)&w_bat_float_charge,
                                       cb_device_data_access_operate_type_read);//BAT_U_FLOAT_CHG 	
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                       cb_net_data_att_set,
                                       0x0043,
                                       &w_data_len,
                                       (uint8_t *)&w_bat_lv_protect,
                                       cb_device_data_access_operate_type_read);//BAT_LV_PROTECT	
                w_data_temp_0 = w_bat_lv_protect;
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
                                       cb_net_data_att_set,
                                       0x0071,
                                       &w_data_len,
                                       (uint8_t *)&w_vol_back_to_battery,
                                       cb_device_data_access_operate_type_read);//Voltage back to battery mode when ”SBU” /”SB”/ “SUB”
               net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
                                       cb_net_data_att_set,
                                       0x0070,
                                       &w_data_len,
                                       (uint8_t *)&w_vol_back_to_utility,
                                       cb_device_data_access_operate_type_read);//Voltage back to utility mode when ”SBU” /”SB”/ “SUB”
               net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
                                       cb_net_data_att_set,
                                       0x0020,
                                       &w_data_len,
                                       (uint8_t *)&w_acin_logic,
                                       cb_device_data_access_operate_type_read);//ACin Logic
                if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                     0x0044,
                                                     (const net_data_info ** const)&pst_data_info,
                                                     pst_net_device_info))
                {
                    return ;
                }
				if(0 != w_acin_logic)
				{
					w_bat_lv_waring_max = (w_vol_back_to_utility - 100 < w_bat_float_charge - 100)?(((w_vol_back_to_utility - 100) < 13900)?(w_vol_back_to_utility - 100):13900):\
									(((w_bat_float_charge - 100) < 13900)?(w_bat_float_charge - 100):13900);
				}
				else
				{
					w_bat_lv_waring_max = ((w_bat_float_charge - 100) < 13900)?(w_bat_float_charge - 100):13900;
				}
				w_bat_lv_waring_min = ((w_data_temp_0 + 100) > 10000)?(w_data_temp_0 + 100):10000;
                device_set_attribute_range_set(w_bat_lv_waring_max,
                                               w_bat_lv_waring_min,
                                               1,
                                               pst_data_info->pst_net_data_set_attribute);								   
			}
            {//004C
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                       cb_net_data_att_set,
                                       0x004C,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_1,
                                       cb_device_data_access_operate_type_read);//
                if((0 != w_data_temp_1) &&\
                   (0 == (w_data_temp_1 & 0x8000)))
                {
                    w_data_temp_1 = ~(w_data_temp_1 * b_bat_num) + 1;
                    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                           cb_net_data_att_set,
                                           0x004C,
                                           &w_data_len,
                                           (uint8_t *)&w_data_temp_1,
                                           cb_device_data_access_operate_type_write);//
                }
                if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                     0x004C,
                                                     (const net_data_info ** const)&pst_data_info,
                                                     pst_net_device_info))
                {
                    return ;
                }
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                       cb_net_data_att_set,
                                       0x0050,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_1,
                                       cb_device_data_access_operate_type_read);//
                if(3 == w_data_temp_1)
                {
                    w_data_temp_1 = 4;
                }
                else if(5 == w_data_temp_1)
                {
                    w_data_temp_1 = 0;
                }
                else
                {
                    w_data_temp_1 = 36;
                }
                device_set_attribute_range_set(0,
                                               (~(b_bat_num * w_data_temp_1) + 1),
                                               b_bat_num,
                                               pst_data_info->pst_net_data_set_attribute);
            }
			{//0x4E Undervoltage Protect Recover
				
				if(FALSE == net_get_device_data_info(cb_net_data_att_set,
													 0x004E,
													 (const net_data_info ** const)&pst_data_info,
													 pst_net_device_info))
				{
					return ;
				} 
				net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                       cb_net_data_att_set,
                                       0x0043,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_1,
                                       cb_device_data_access_operate_type_read);//
                w_data_temp_1 = ((w_data_temp_1 + 100) > 11000)?(w_data_temp_1 + 100):11000;
				if(check_system_type(cb_inv_type_riio_sun_ii) 
					|| check_system_type(cb_inv_type_riio_sun_ii_gte) 
					|| check_system_type(cb_inv_type_riio_sun_ii_split_phase) 
					|| check_system_type(cb_inv_type_riio_ii_split_phase)
					|| check_system_type(cb_inv_type_riio_ii))
				{
					w_data_temp_2 = 14000;
				}
				else
				{
					w_data_temp_2 = 14500;
				}
				device_set_attribute_range_set(w_data_temp_2,
											   w_data_temp_1,
											   1,
											   pst_data_info->pst_net_data_set_attribute);
			}
			
            {//0050 0051   ///PARAM_REC_AC_U_MAX    PARAM_REC_AC_U_MIN
                if(f_hv || (check_system_type(cb_inv_type_riio_sun_ii_split_phase) || check_system_type(cb_inv_type_riio_ii_split_phase)))
                {
                    if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                         0x0050,
                                                         (const net_data_info ** const)&pst_data_info,
                                                         pst_net_device_info))
                    {
                        return ;
                    }
					if(check_system_type(cb_inv_type_riio_sun_ii_split_phase) || check_system_type(cb_inv_type_riio_ii_split_phase))
					{
						device_set_attribute_range_set(2760,
													   2400,
													   1,
													   pst_data_info->pst_net_data_set_attribute);

					}	
					else
					{
						device_set_attribute_range_set(2650,
													   2400,
													   1,
													   pst_data_info->pst_net_data_set_attribute);
					}
                    

                    if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                         0x0051,
                                                         (const net_data_info ** const)&pst_data_info,
                                                         pst_net_device_info))
                    {
                        return ;
                    }
					if(check_system_type(cb_inv_type_riio_sun_ii_split_phase) || check_system_type(cb_inv_type_riio_ii_split_phase))
					{
						device_set_attribute_range_set(2200,
													    1800,
														1,
														pst_data_info->pst_net_data_set_attribute);

					}
					else
					{
						device_set_attribute_range_set(2000,
													   1450,
													   1,
													   pst_data_info->pst_net_data_set_attribute);
					}

                }
                else
                {
                    if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                         0x0050,
                                                         (const net_data_info ** const)&pst_data_info,
                                                         pst_net_device_info))
                    {
                        return ;
                    }
                    device_set_attribute_range_set(2800,
                                                   2400,
                                                   1,
                                                   pst_data_info->pst_net_data_set_attribute);

                    if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                         0x0051,
                                                         (const net_data_info ** const)&pst_data_info,
                                                         pst_net_device_info))
                    {
                        return ;
                    }
                    device_set_attribute_range_set(2200,
                                                   1700,
                                                   1,
                                                   pst_data_info->pst_net_data_set_attribute);
                }
            }
            {//0052 0053   ///PARAM_REC_AC_F_MAX    PARAM_REC_AC_F_MIN
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                       cb_net_data_att_set,
                                       0x0001,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_1,
                                       cb_device_data_access_operate_type_read);// PARAM_REC_AC_F_MAX
                //0052
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                       cb_net_data_att_set,
                                       0x0052,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_2,
                                       cb_device_data_access_operate_type_read);// PARAM_REC_AC_F_MAX

                if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                     0x0052,
                                                     (const net_data_info ** const)&pst_data_info,
                                                     pst_net_device_info))
                {
                    return ;
                }
                if(w_data_temp_2 < 1000)
                {
                    w_data_temp_2 += w_data_temp_1;
                    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                           cb_net_data_att_set,
                                           0x0052,
                                           &w_data_len,
                                           (uint8_t *)&w_data_temp_2,
                                           cb_device_data_access_operate_type_write);// PARAM_REC_AC_F_MAX
                }
                device_set_attribute_range_set(w_data_temp_1 + 900,
                                               w_data_temp_1 + 100,
                                               1,
                                               pst_data_info->pst_net_data_set_attribute);
                //0053
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                       cb_net_data_att_set,
                                       0x0053,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_2,
                                       cb_device_data_access_operate_type_read);// PARAM_REC_AC_F_MIN

                if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                     0x0053,
                                                     (const net_data_info ** const)&pst_data_info,
                                                     pst_net_device_info))
                {
                    return ;
                }
                if(w_data_temp_2 < 1000)
                {
                    w_data_temp_2 = w_data_temp_1 - w_data_temp_2;
                    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                           cb_net_data_att_set,
                                           0x0053,
                                           &w_data_len,
                                           (uint8_t *)&w_data_temp_2,
                                           cb_device_data_access_operate_type_write);// PARAM_REC_AC_F_MIN
                }
                device_set_attribute_range_set(w_data_temp_1 - 100,
                                               w_data_temp_1 - 900,
                                               1,
                                               pst_data_info->pst_net_data_set_attribute);
            }
			//0x47 Tyrann 为50~550A

			{
                if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                     0x0047,
                                                     (const net_data_info ** const)&pst_data_info,
                                                     pst_net_device_info))
                {
                    return ;
                }  
                if(check_system_type(cb_inv_type_tyrann) || check_system_type(cb_inv_type_apollo_matric_ii_10_15k))
                {
                    w_data_temp_1 = 550 * b_inv_num;
                    w_data_temp_2 = 50 * b_inv_num;
                }
                else
                {
                    w_data_temp_1 = 300 * b_inv_num;
                    w_data_temp_2 = 50 * b_inv_num;
                }
			 	device_set_attribute_range_set(w_data_temp_1,
                                               w_data_temp_2,
                                               1,
                                               pst_data_info->pst_net_data_set_attribute);
			}
            
            {//0056     //005E
                uint16_t w_data_temp_4 = 50;
				uint16_t w_data_temp_5 = 10;
                if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                     0x0056,
                                                     (const net_data_info ** const)&pst_data_info,
                                                     pst_net_device_info))
                {
                    return ;
                }
				w_data_temp_5 = 5;
				{
					if(check_system_type(cb_inv_type_riio_sun_ii) 
						|| check_system_type(cb_inv_type_riio_sun_ii_gte) 
						|| check_system_type(cb_inv_type_riio_sun_ii_split_phase) 
						|| check_system_type(cb_inv_type_riio_ii_split_phase)
						|| check_system_type(cb_inv_type_riio_ii))
					{
						for(uint8_t b_cnt_i = 0;b_cnt_i < cb_riiosunii_cklcd_para_max;b_cnt_i++)
						{
							if((b_inverter_model_version == st_riiosunii_cklcd_para[b_cnt_i].b_model_ver) 
								&&(b_inverter_hd_version <= st_riiosunii_cklcd_para[b_cnt_i].b_hd_ver))
							{
							    w_data_temp_4 = st_riiosunii_cklcd_para[b_cnt_i].w_ac_current_max;
                        		break;
							}
							else if((b_inverter_model_version == st_riiosunii_cklcd_para[b_cnt_i].b_model_ver) 
								&& (b_inverter_hd_version > 7 && b_inverter_hd_version >= st_riiosunii_cklcd_para[b_cnt_i].b_hd_ver))
							{
								w_data_temp_4 = st_riiosunii_cklcd_para[b_cnt_i + 1].w_ac_current_max;
								break;
							}
						}
					}
					else if(check_system_type(cb_inv_type_ck_ii) || check_system_type(cb_inv_type_Tyrann_3_5_8k))
					{
						for(uint8_t b_cnt_i = 0;b_cnt_i < cb_ck_ii_cklcd_para_max;b_cnt_i++)
						{
							if((b_inverter_model_version == st_ck_ii_cklcd_para[b_cnt_i].b_model_ver) 
								&& ((b_inverter_hd_version <= st_ck_ii_cklcd_para[b_cnt_i].b_hd_ver) 
								|| (b_inverter_hd_version > 7 && b_inverter_hd_version >= st_ck_ii_cklcd_para[b_cnt_i].b_hd_ver)))
							{
							    w_data_temp_4 = st_ck_ii_cklcd_para[b_cnt_i].w_ac_current_max;
                        		break;
							}
							else if((b_inverter_model_version == st_ck_ii_cklcd_para[b_cnt_i].b_model_ver) 
								&& (b_inverter_hd_version > 7 && b_inverter_hd_version >= st_ck_ii_cklcd_para[b_cnt_i].b_hd_ver))
							{
								w_data_temp_4 = st_ck_ii_cklcd_para[b_cnt_i + 1].w_ac_current_max;
								break;
							}
						}
					}
					else if(check_system_type(cb_inv_type_tyrann))
					{
						for(uint8_t b_cnt_i = 0;b_cnt_i < cb_tyrann_cklcd_para_max;b_cnt_i++)
						{
							if((b_inverter_model_version == st_tyrann_cklcd_para[b_cnt_i].b_model_ver) 
								&& ((b_inverter_hd_version <= st_tyrann_cklcd_para[b_cnt_i].b_hd_ver) 
								|| (b_inverter_hd_version > 7 && b_inverter_hd_version >= st_tyrann_cklcd_para[b_cnt_i].b_hd_ver)))
							{
								w_data_temp_4 = st_tyrann_cklcd_para[b_cnt_i].w_ac_current_max;
								w_data_temp_5 = 10;
                        		break;
							}
							else if((b_inverter_model_version == st_tyrann_cklcd_para[b_cnt_i].b_model_ver) 
								&& (b_inverter_hd_version > 7 && b_inverter_hd_version >= st_tyrann_cklcd_para[b_cnt_i].b_hd_ver))
							{
								w_data_temp_4 = st_tyrann_cklcd_para[b_cnt_i + 1].w_ac_current_max;
								break;
							}
						}
					}
					else if(check_system_type(cb_inv_type_apollo_matric_ii_10_15k) || check_system_type(cb_inv_type_apollo_matric_ii_5_8k))
					{
						for(uint8_t b_cnt_i = 0;b_cnt_i < cb_matrix_ii_cklcd_para_max;b_cnt_i++)
						{
							if(st_matrix_ii_cklcd_para[b_cnt_i].b_rate_power == 100 || st_matrix_ii_cklcd_para[b_cnt_i].b_rate_power == 150)
							{
								w_data_temp_4 = st_matrix_ii_cklcd_para[b_cnt_i].w_ac_current_max;
								w_data_temp_5 = 10;
								break;

							}
							else if((b_inverter_model_version == st_matrix_ii_cklcd_para[b_cnt_i].b_model_ver) 
								&& ((b_inverter_hd_version <= st_matrix_ii_cklcd_para[b_cnt_i].b_hd_ver) 
								|| (b_inverter_hd_version > 7 && b_inverter_hd_version >= st_matrix_ii_cklcd_para[b_cnt_i].b_hd_ver)))
							{
							    w_data_temp_4 = st_matrix_ii_cklcd_para[b_cnt_i].w_ac_current_max;
                        		break;
							}
							else if((b_inverter_model_version == st_matrix_ii_cklcd_para[b_cnt_i].b_model_ver) 
								&& (b_inverter_hd_version > 7 && b_inverter_hd_version >= st_matrix_ii_cklcd_para[b_cnt_i].b_hd_ver))
							{
								w_data_temp_4 = st_matrix_ii_cklcd_para[b_cnt_i + 1].w_ac_current_max;
								break;
							}
						}
					}
					else if(check_system_type(cb_inv_type_apollo_matric_ii_lite))
					{
						for(uint8_t b_cnt_i = 0;b_cnt_i < cb_matrix_ii_lite_cklcd_para_max;b_cnt_i++)
						{
							if((b_inverter_model_version == st_matrix_ii_lite_cklcd_para[b_cnt_i].b_model_ver) 
								&& ((b_inverter_hd_version <= st_matrix_ii_lite_cklcd_para[b_cnt_i].b_hd_ver) 
								|| (b_inverter_hd_version > 7 && b_inverter_hd_version >= st_matrix_ii_lite_cklcd_para[b_cnt_i].b_hd_ver)))
							{
							    w_data_temp_4 = st_matrix_ii_lite_cklcd_para[b_cnt_i].w_ac_current_max;
                        		break;
							}
							else if((b_inverter_model_version == st_matrix_ii_lite_cklcd_para[b_cnt_i].b_model_ver) 
								&& (b_inverter_hd_version > 7 && b_inverter_hd_version >= st_matrix_ii_lite_cklcd_para[b_cnt_i].b_hd_ver))
							{
								w_data_temp_4 = st_matrix_ii_lite_cklcd_para[b_cnt_i + 1].w_ac_current_max;
								break;
							}
						}
					}
				}
                device_set_attribute_range_set(w_data_temp_4,
                                               w_data_temp_5,
                                               1,
                                               pst_data_info->pst_net_data_set_attribute);

                if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                     0x005E,
                                                     (const net_data_info ** const)&pst_data_info,
                                                     pst_net_device_info))
                {
                    return ;
                }
                device_set_attribute_range_set(w_data_temp_4,
                                               w_data_temp_5,
                                               1,
                                               pst_data_info->pst_net_data_set_attribute);
            }

            {//0091 SOC_Low_waring
                uint16_t w_data_temp_3 = 0;
                uint16_t w_data_temp_4 = 0;
				uint16_t w_data_temp_5 = 0;
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                       cb_net_data_att_set,
                                       0x0092,							//SOC_Low_protect
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_1,
                                       cb_device_data_access_operate_type_read);
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                       cb_net_data_att_set,
                                       0x0093,							//SOC_CHG_Enough
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_4,
                                       cb_device_data_access_operate_type_read);
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
                                       cb_net_data_att_set,
                                       0x0072,							//Battery Reserved Capacity 
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_2,
                                       cb_device_data_access_operate_type_read);
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
                                       cb_net_data_att_set,
                                       0x0020,							//ACin Logic USB(交流主用):0  SB（直流主用）:1 SBU （电网直挂，直流主用）:2  SUB :3
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_3,
                                       cb_device_data_access_operate_type_read);

                if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                     0x0091,
                                                     (const net_data_info ** const)&pst_data_info,
                                                     pst_net_device_info))
                {
                    return ;
                }
				if((check_system_type(cb_inv_type_riio_sun_ii) 
					|| check_system_type(cb_inv_type_riio_sun_ii_gte) 
					|| check_system_type(cb_inv_type_riio_sun_ii_split_phase) 
					|| check_system_type(cb_inv_type_riio_ii_split_phase)
					|| check_system_type(cb_inv_type_riio_ii)) && w_data_temp_3 != 0)
				{
					w_data_temp_5 = (w_data_temp_4 - 1 < w_data_temp_2 - 1)?(((w_data_temp_4 - 1) < 80)?(w_data_temp_4 - 1):80):\
									  (((w_data_temp_2 - 1) < 80)?(w_data_temp_2 - 1):80);
					w_data_temp_2 = (((w_data_temp_1 + 1) > 5)?(w_data_temp_1 + 1):5);
					w_data_temp_1 = w_data_temp_5;
				}
				else if(check_system_type(cb_inv_type_riio_sun_ii)
					|| check_system_type(cb_inv_type_riio_sun_ii_gte) 
					|| check_system_type(cb_inv_type_riio_sun_ii_split_phase) 
					|| check_system_type(cb_inv_type_riio_ii_split_phase)
					|| check_system_type(cb_inv_type_riio_ii))
				{
					w_data_temp_2 = (((w_data_temp_1 + 1) > 5)?(w_data_temp_1 + 1):5);
					w_data_temp_1 = (((w_data_temp_4 - 1) < 80)?(w_data_temp_4 - 1):80);
				}
				else
				{
					w_data_temp_2 = ((w_data_temp_1 + 1) > 5) ? (w_data_temp_1 + 1) : 5;
					w_data_temp_1 = ((w_data_temp_4 - 5) < 80) ? (w_data_temp_4 - 5) : 80;
				} 

                device_set_attribute_range_set(w_data_temp_1,
                                               w_data_temp_2,
                                               1,
                                               pst_data_info->pst_net_data_set_attribute);
            }
            {//0092 SOC_Low_Protect
                uint16_t w_data_temp_3 = 0;
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                       cb_net_data_att_set,
                                       0x0091,							//SOC_Low_Warning
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_1,
                                       cb_device_data_access_operate_type_read);

                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
                                       cb_net_data_att_set,
                                       0x0072,							//Battery Reserved Capacity 
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_2,
                                       cb_device_data_access_operate_type_read);
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
                                       cb_net_data_att_set,
                                       0x0020,							//ACin Logic USB(交流主用):0  SB（直流主用）:1 SBU （电网直挂，直流主用）:2  SUB :3
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_3,
                                       cb_device_data_access_operate_type_read);

                if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                     0x0092,
                                                     (const net_data_info ** const)&pst_data_info,
                                                     pst_net_device_info))
                {
                    return ;
                }

                if(w_data_temp_3!=0)                    //若机型为RiiO SUN II或Tyrann USB(交流主用):0  SB（直流主用）:1 SBU （电网直挂，直流主用）:2  SUB :3
                {
                	if(check_system_type(cb_inv_type_riio_sun_ii_split_phase) || check_system_type(cb_inv_type_riio_ii_split_phase))
                	{
						w_data_temp_1 = ((w_data_temp_1 - 1) < 40)?(w_data_temp_1 - 1):40;
					}
					else
					{
						w_data_temp_1 = (w_data_temp_1 < w_data_temp_2)?(((w_data_temp_1 - 1) < 40)?(w_data_temp_1 - 1):40):\
                                    (((w_data_temp_2 - 1) < 40)?(w_data_temp_2 - 1):40);
					}

                }else
                {
                     w_data_temp_1 = (((w_data_temp_1 - 1) < 40)?(w_data_temp_1 - 1):40);
                }


//                w_data_temp_1 = (w_data_temp_1 < w_data_temp_2)?(((w_data_temp_1 - 1) < 40)?(w_data_temp_1 - 1):40):\
//                                (((w_data_temp_2 - 1) < 40)?(w_data_temp_2 - 1):40);

                device_set_attribute_range_set(w_data_temp_1,
                                               3,
                                               1,
                                               pst_data_info->pst_net_data_set_attribute);
            }
			{//0093 SOC_CHG_Enough
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                       cb_net_data_att_set,
                                       0x0091,							//SOC_Low_Warning
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_1,
                                       cb_device_data_access_operate_type_read);
		         if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                     0x0093,
                                                     (const net_data_info ** const)&pst_data_info,
                                                     pst_net_device_info))
                {
                    return ;
                }
                if(check_system_type(cb_inv_type_riio_sun_ii) 
					|| check_system_type(cb_inv_type_riio_sun_ii_gte) 
					|| check_system_type(cb_inv_type_riio_sun_ii_split_phase) 
					|| check_system_type(cb_inv_type_riio_ii_split_phase)
					|| check_system_type(cb_inv_type_riio_ii))               
                {
                	w_data_temp_1 = (((w_data_temp_1 + 1) > 30)?(w_data_temp_1 + 1):30);
                }else
                {
               		w_data_temp_1 = (((w_data_temp_1 + 5) > 30)?(w_data_temp_1 + 5):30);;
                }

                device_set_attribute_range_set(99,
                                               w_data_temp_1,
                                               1,
                                               pst_data_info->pst_net_data_set_attribute);
            }
            /*{//00B0~00B2
                uint16_t w_data_temp_3 = 0;
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                       cb_net_data_att_set,
                                       0x0001,							//INV_Output_FREQ
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_1,
                                       cb_device_data_access_operate_type_read);
                if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                     0x00B0,
                                                     (const net_data_info ** const)&pst_data_info,
                                                     pst_net_device_info))
                {
                    return ;
                }
				w_data_temp_3 = (w_data_temp_1 == 5000) ? 5100 : 6100;	
				w_data_temp_2 = (w_data_temp_1 == 5000) ? 5010 : 6010;										 
				device_set_attribute_range_set(w_data_temp_3,
											   w_data_temp_2,
											   10,
											   pst_data_info->pst_net_data_set_attribute);	
				if(FALSE == net_get_device_data_info(cb_net_data_att_set,
													 0x00B1,
													 (const net_data_info ** const)&pst_data_info,
													 pst_net_device_info))
				{
					return ;
				}
				w_data_temp_3 = (w_data_temp_1 == 5000) ? 5350 : 6350;	
				w_data_temp_2 = (w_data_temp_1 == 5000) ? 5110 : 6110;										 
				device_set_attribute_range_set(w_data_temp_3,
											   w_data_temp_2,
											   10,
											   pst_data_info->pst_net_data_set_attribute);	
		         if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                     0x00B2,
                                                     (const net_data_info ** const)&pst_data_info,
                                                     pst_net_device_info))
                {
                    return ;
                }
				w_data_temp_3 = (w_data_temp_1 == 5000) ? 5450 : 6450;	
				w_data_temp_2 = (w_data_temp_1 == 5000) ? 5150 : 6150;										 
				device_set_attribute_range_set(w_data_temp_3,
											   w_data_temp_2,
											   10,
											   pst_data_info->pst_net_data_set_attribute);	
				}*/				 
			}
        }
#if( task_runing_status_print )
    DebugPrintf(TRUE,"Device_set_range_manage_end\r\n");
#endif
}

//uint8_t device_check_ags_alarm_bits(uint16_t w_alarm_bits_net_addr)
//{
//    static uint8_t f_trig_enable = TRUE;
//
//    uint16_t w_inverter_alarm_bits = 0;
//    uint16_t w_read_data_len = 0;
//    uint16_t w_data_temp;
//    net_device_info *pst_net_device_info = 0;
//    uint8_t f_check_overload_enable;
//    uint8_t f_check_overtemp_enable;
//    uint8_t b_result = FALSE;
//
//    {
//        uint16_t w_data_len = 1;
//        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
//                               cb_net_data_att_set,
//                               0x0031,
//                               &w_data_len,
//                               (uint8_t *)&f_check_overload_enable,
//                               cb_device_data_access_operate_type_read);//overload_enable
//        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
//                               cb_net_data_att_set,
//                               0x0032,
//                               &w_data_len,
//                               (uint8_t *)&f_check_overtemp_enable,
//                               cb_device_data_access_operate_type_read);//overtemp_enable
//
//        if((FALSE == f_check_overload_enable) &&\
//           (FALSE == f_check_overtemp_enable))
//        {
//            f_trig_enable = TRUE;
//            return FALSE;
//        }
//    }
//
//    for(uint8_t b_cnt_i = cw_device_id_index_ck_inverter_u1;
//        b_cnt_i <= cw_device_id_index_ck_inverter_w3;
//        b_cnt_i++)
//    {
//        if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[b_cnt_i],
//                                        (net_device_info **)&pst_net_device_info,
//                                        &st_net_system_info_list))
//        {
//            return FALSE;
//        }
//
//        if(TRUE == net_check_device_communicate_status(cb_net_device_add,pst_net_device_info))
//        {
//            net_device_data_access(dwa_device_id_kinergy_ii[b_cnt_i],
//                                   cb_net_data_att_status,
//                                   w_alarm_bits_net_addr,
//                                   &w_read_data_len,
//                                   (uint8_t *)&w_data_temp,
//                                   cb_device_data_access_operate_type_read);//inverter alarm bits
//            w_inverter_alarm_bits |= w_data_temp;
//        }
//    }
//
//    if((0x0008 == (w_inverter_alarm_bits & 0x0008)) &&\
//       (0x0100 == (w_inverter_alarm_bits & 0x0100)))//overload + overtemp
//    {
//        if(f_trig_enable)
//        {
//            f_trig_enable = FALSE;
//            b_result = TRUE;
//        }
//    }
//    else if((0x0008 == (w_inverter_alarm_bits & 0x0008)) &&\
//            (TRUE == f_check_overload_enable))//overload
//    {
//        {
//            if(f_trig_enable)
//            {
//                f_trig_enable = FALSE;
//                b_result = TRUE;
//            }
//        }
//    }
//    else if((0x0100 == (w_inverter_alarm_bits & 0x0100)) &&\
//            (TRUE == f_check_overtemp_enable))//overtemp
//    {
//        {
//            if(f_trig_enable)
//            {
//                f_trig_enable = FALSE;
//                b_result = TRUE;
//            }
//        }
//    }
//    else
//    {
//        f_trig_enable = TRUE;
//    }
//
//
//    return b_result;
//}
//
//uint8_t device_ags_soc_control(uint8_t *pb_ags_switch_on_off)
//{
//    uint16_t w_data_len = 1;
//    uint16_t w_data_temp_1 = 0;
//    uint16_t w_data_temp_2 = 0;
//    uint16_t w_soc_value = 0;
//    static uint8_t f_trig_enable = TRUE;
//
//    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
//                           cb_net_data_att_set,
//                           0x0034,
//                           &w_data_len,
//                           (uint8_t *)&w_data_temp_1,
//                           cb_device_data_access_operate_type_read);//ags soc ctrl enable
//
//    if(TRUE == w_data_temp_1)
//    {
//        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
//                               cb_net_data_att_status,
//                               0x0121,
//                               &w_data_len,
//                               (uint8_t *)&w_soc_value,
//                               cb_device_data_access_operate_type_read);//bat soc
//        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
//                               cb_net_data_att_set,
//                               0x0035,
//                               &w_data_len,
//                               (uint8_t *)&w_data_temp_1,
//                               cb_device_data_access_operate_type_read);//ags switch on soc
//        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
//                               cb_net_data_att_set,
//                               0x0036,
//                               &w_data_len,
//                               (uint8_t *)&w_data_temp_2,
//                               cb_device_data_access_operate_type_read);//ags switch off soc
//
//        if((0 != w_soc_value) &&\
//           (w_data_temp_2 > w_data_temp_1))
//        {
//            if(w_soc_value <= w_data_temp_1)
//            {
//                if(f_trig_enable)
//                {
//                    f_trig_enable = FALSE;
//                    *pb_ags_switch_on_off = TRUE;
//                    return TRUE;
//                }
//            }
//            else if(w_soc_value >= w_data_temp_2)
//            {
//                if(f_trig_enable)
//                {
//                    f_trig_enable = FALSE;
//                    *pb_ags_switch_on_off = FALSE;
//                    return TRUE;
//                }
//            }
//            else
//            {
//                f_trig_enable = TRUE;
//            }
//        }
//    }
//
//    return FALSE;
//}
//
///************************************************************************************************
//* Input: void
//*
//* Output: void
//*
//* Logic discription: control inverter ags run status
//*                    kinergy set 0x0031 AGS Switch On When Inverter Overload
//*                    kinergy set 0x0032 AGS Switch On When Inverter Overtemp
//*                    kinergy set 0x0033 AGS Switch Off When Grid In (reserve)
//*                    kinergy set 0x0034 AGS SOC Control enable
//*                    kinergy set 0x0035 AGS Switch on SOC < (%) if(1 == kinergy set 0x0034)
//*                    kinergy set 0x0036 AGS Switch off SOC > (%)if(1 == kinergy set 0x0034)
//*
//* run time:
//************************************************************************************************/
//void device_control_ags_control()
//{
//    static uint8_t ba_data[2] = {0,};
//    static uint8_t f_trig = FALSE;
//
////    static uint16_t w_net_cmd;
////    static uint8_t b_config_type;
////    static uint32_t dw_device_type;
////    static uint16_t w_data_len;
//
//#if( task_runing_status_print )
//    DebugPrintf(TRUE,"Ags_control_start\r\n");
//#endif
//
//    if(!f_trig)
//    {
//        uint8_t b_result;
//        b_result = device_check_ags_alarm_bits(0x0123);
//
//        if(TRUE == b_result)
//        {
//            ba_data[0] = 0;
//            ba_data[1] = 1;
//            f_trig = TRUE;
//        }
//        else
//        {
//            b_result = device_ags_soc_control(&ba_data[1]);
//            if(TRUE == b_result)
//            {
//                f_trig = TRUE;
//            }
//        }
//    }
//
//    if(f_trig)
//    {
//        {
//            uint16_t w_data_temp = 0;
//            uint16_t w_data_len = 2;
//
//            net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
//                                   cb_net_data_att_status,
//                                   0x0129,
//                                   &w_data_len,
//                                   (uint8_t *)&w_data_temp,
//                                   cb_device_data_access_operate_type_read);//ags status bits
//
//            if(ba_data[1] == (w_data_temp & 0x0001))
//            {
//                f_trig = FALSE;
//                return;
//            }
//        }
//        {
//            uint8_t b_result = FALSE;
//            b_result = network_device_config_sycn_trig(cdw_device_type_ck_inverter,
//                                                       cb_net_subset_com_ctrl_cmd,
//                                                       0x0455,
//                                                       2,
//                                                       ba_data,
//                                                       1,
//                                                       TRUE,
//                                                       sync_type_single);
//            if(cb_sync_trig_return_busy != b_result)
//            {
//                f_trig = FALSE;
//            }
//        }
//    }
//#if( task_runing_status_print )
//    DebugPrintf(TRUE,"Ags_control_end\r\n");
//#endif
//}
