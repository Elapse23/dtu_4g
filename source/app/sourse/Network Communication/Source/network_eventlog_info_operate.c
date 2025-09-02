#include "network_eventlog_info_operate.h"
#include "net_system_info_list.h"
#include "net_device_tree.h"
#include <string.h>
#include "net_device_data_access.h"
#include "net_device_data.h"
#include "spi_flash_address_allocate.h"
#include "application_general_func.h"
#include "extern_flash_operate.h"
#include "net_device_tree_ck_inverter.h"
#include "net_device_tree_cklcd.h"
#include "net_system_info_access.h"
#include "freertos_user_config.h"
#include "Eeprom.h"
#include "semphr.h"
#include "network_system_type_operate.h"
#include "net_device_data_attribute.h"
#include "system_timestamp_generate.h"
#include "spi_flash_operate.h"
#include "crc16_arithmetic.h"
#include "network_local_firmware_update.h"

#define cw_ck_inverter_alarm_code_start_1               0x0101
#define cw_ck_inverter_alarm_code_start_2               0x0150
#define cw_acin_err_code_start                          0x0114
#define cw_gen_alarm_code_start                         0x013C
#define cw_ck_type_mppt_alarm_code_start                0x0101
#define cw_riiosunii_type_mppt_alarm_code_start         0x0121
#define cw_ck_type_pv_alarm_code_start                  0x0101
#define cw_bms_alarm_code_start                         0x0132
#define cw_bms_fault_code_start                         0x0128
#define cw_riiosunii_type_sp600_alarm_code_start		0x1101


#define cw_ck_inverter_fault_code_start_1               0x0001
#define cw_ck_inverter_fault_code_start_2               0x0015
#define cw_ck_type_mppt_fault_code_start                0x0001
#define cw_riiosunii_type_mppt_fault_code_start         0x0001
#define cw_ck_type_pv_fault_code_start                  0x0001
#define cw_riiosunii_type_sp600_fault_code_start		0x1001

#define cw_riiosunii_type_status_switch_code			0x0201

#define cb_max_alarm_bits_num                   6
#define cb_max_fault_bits_num                   5

typedef struct
{
    uint16_t w_abnormal_bits;
    uint16_t w_abnormal_code_start;
}abnormal_bits_info;

typedef struct
{
    uint32_t dw_device_id;
    abnormal_bits_info st_alarm_bits_info[cb_max_alarm_bits_num];
    abnormal_bits_info st_fault_bits_info[cb_max_fault_bits_num];
}abnormal_info;

event_log_manage_info st_event_log_manage_info =
{
    .dw_event_log_save_start_addr = cdw_flash_addr_alloc_event_log_start,
    .dw_event_log_save_end_addr = cdw_flash_addr_alloc_event_log_start,
};

SemaphoreHandle_t  pst_event_save_mutex = NULL;
void device_event_log_save_Semaphore_creat()
{
    pst_event_save_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_event_save_mutex)
    {

    }
}

uint8_t fill_alarm_fault_code(uint8_t *pb_data_in,
                              uint16_t w_start_code,
                              uint16_t w_alarm_fault_bits,
                              uint16_t w_max_data_len)
{
    uint16_t w_data_len = 0;
    if(0 == w_alarm_fault_bits)
    {
        return 0;
    }
    else
    {
        uint16_t w_alarm_fault_value = 0;

        for(uint8_t b_cnt_i = 0;b_cnt_i < 16;b_cnt_i++)
        {
            w_alarm_fault_value = (w_alarm_fault_bits & 0x0001);
            if(0 != w_alarm_fault_value)
            {
                if(w_data_len + 2 < w_max_data_len)
                {
                    pb_data_in[w_data_len++] = (uint8_t)(((w_start_code + b_cnt_i) & 0xFF00) >> 8);
                    pb_data_in[w_data_len++] = (uint8_t)((w_start_code + b_cnt_i) & 0x00FF);
                }
                else
                {
                    break;
                }
            }
            w_alarm_fault_bits >>= 1;
        }
    }
    return w_data_len;
}
uint8_t fill_alarm_code(uint8_t *pb_data_in,
                        uint16_t w_max_buff_len,
                        abnormal_info *pst_abnormal_info)
{
    uint16_t w_data_len = 0;
    uint8_t b_alarm_code_len = 0;

    if(w_max_buff_len < 7)
    {
        return 0;
    }
    pb_data_in[w_data_len++] = (uint8_t)((pst_abnormal_info->dw_device_id & 0xFF0000) >> 16);
    pb_data_in[w_data_len++] = (uint8_t)((pst_abnormal_info->dw_device_id & 0x00FF00) >> 8);
    pb_data_in[w_data_len++] = (uint8_t)(pst_abnormal_info->dw_device_id & 0x0000FF);

    for(uint8_t b_cnt_i = 0;b_cnt_i < cb_max_alarm_bits_num;b_cnt_i++)
    {
        b_alarm_code_len +=\
            fill_alarm_fault_code(&pb_data_in[w_data_len + b_alarm_code_len + 2],
                                  pst_abnormal_info->st_alarm_bits_info[b_cnt_i].w_abnormal_code_start,
                                  pst_abnormal_info->st_alarm_bits_info[b_cnt_i].w_abnormal_bits,
                                  (w_max_buff_len - (w_data_len + b_alarm_code_len + 2)));
    }
    pb_data_in[w_data_len++] = (uint8_t)0x00;
    pb_data_in[w_data_len++] = (uint8_t)b_alarm_code_len/2;

    return w_data_len + b_alarm_code_len;
}
uint8_t fill_fault_code(uint8_t *pb_data_in,
                        uint16_t w_max_buff_len,
                        abnormal_info *pst_abnormal_info)
{
    uint16_t w_data_len = 0;
    uint8_t b_alarm_code_len = 0;

    if(w_max_buff_len < 7)
    {
        return 0;
    }
    pb_data_in[w_data_len++] = (uint8_t)((pst_abnormal_info->dw_device_id & 0xFF0000) >> 16);
    pb_data_in[w_data_len++] = (uint8_t)((pst_abnormal_info->dw_device_id & 0x00FF00) >> 8);
    pb_data_in[w_data_len++] = (uint8_t)(pst_abnormal_info->dw_device_id & 0x0000FF);

    for(uint8_t b_cnt_i = 0;b_cnt_i < cb_max_fault_bits_num;b_cnt_i++)
    {
        b_alarm_code_len +=\
            fill_alarm_fault_code(&pb_data_in[w_data_len + b_alarm_code_len + 2],
                                  pst_abnormal_info->st_fault_bits_info[b_cnt_i].w_abnormal_code_start,
                                  pst_abnormal_info->st_fault_bits_info[b_cnt_i].w_abnormal_bits,
                                  (w_max_buff_len - (w_data_len + b_alarm_code_len + 2)));
    }
    pb_data_in[w_data_len++] = (uint8_t)0x00;
    pb_data_in[w_data_len++] = (uint8_t)b_alarm_code_len/2;

    return w_data_len + b_alarm_code_len;
}
uint8_t get_ck_inverter_alarm_fault_bits(uint32_t dw_device_id,
                                         abnormal_info *pst_abnormal_info)
{
    uint8_t b_result = 0;
    uint8_t b_protocal_type = cb_system_protocal_type_ck;

    if(FALSE == check_system_protocal_type(&b_protocal_type))
    {
        return b_result;
    }

    if(cb_system_protocal_type_ck == b_protocal_type)
    {
        uint16_t w_data_len = 0;

        //inv alarm
        pst_abnormal_info->dw_device_id = dw_device_id;
        net_device_data_access(dw_device_id,
                               cb_device_data_access_data_type_status,
                               0x0117,
                               &w_data_len,
                               (uint8_t *)&pst_abnormal_info->st_alarm_bits_info[0].w_abnormal_bits,
                               cb_device_data_access_operate_type_read);
        pst_abnormal_info->st_alarm_bits_info[0].w_abnormal_code_start = cw_ck_inverter_alarm_code_start_1;
        //acin err
        net_device_data_access(dw_device_id,
                               cb_device_data_access_data_type_status,
                               0x0118,
                               &w_data_len,
                               (uint8_t *)&pst_abnormal_info->st_alarm_bits_info[1].w_abnormal_bits,
                               cb_device_data_access_operate_type_read);
        pst_abnormal_info->st_alarm_bits_info[1].w_abnormal_code_start = cw_acin_err_code_start;

        //bms protect
        net_device_data_access(dw_device_id,
                               cb_device_data_access_data_type_status,
                               0x0132,
                               &w_data_len,
                               (uint8_t *)&pst_abnormal_info->st_alarm_bits_info[2].w_abnormal_bits,
                               cb_device_data_access_operate_type_read);
        pst_abnormal_info->st_alarm_bits_info[2].w_abnormal_code_start = cw_bms_fault_code_start;

        //bms alarm
        net_device_data_access(dw_device_id,
                               cb_device_data_access_data_type_status,
                               0x0133,
                               &w_data_len,
                               (uint8_t *)&pst_abnormal_info->st_alarm_bits_info[3].w_abnormal_bits,
                               cb_device_data_access_operate_type_read);
        pst_abnormal_info->st_alarm_bits_info[3].w_abnormal_code_start = cw_bms_alarm_code_start;

        //inv fault
        net_device_data_access(dw_device_id,
                               cb_device_data_access_data_type_status,
                               0x0116,
                               &w_data_len,
                               (uint8_t *)&pst_abnormal_info->st_fault_bits_info[0].w_abnormal_bits,
                               cb_device_data_access_operate_type_read);
        pst_abnormal_info->st_fault_bits_info[0].w_abnormal_code_start = cw_ck_inverter_fault_code_start_1;
    }
    else if(cb_system_protocal_type_riiosunii == b_protocal_type)
    {
        uint16_t w_data_len = 0;
		uint16_t w_riiosunii_lcd_ver = 0;
		uint16_t w_sys_waring2_bit = 0;
        pst_abnormal_info->dw_device_id = dw_device_id;
		net_device_data_access(dw_device_id,
							   cb_net_data_att_status,
							   0x012D,
							   &w_data_len,
							   (uint8_t *)&w_riiosunii_lcd_ver,
							   cb_device_data_access_operate_type_read);
        //alarm
        net_device_data_access(dw_device_id,
                               cb_device_data_access_data_type_status,
                               0x0123,
                               &w_data_len,
                               (uint8_t *)&pst_abnormal_info->st_alarm_bits_info[0].w_abnormal_bits,
                               cb_device_data_access_operate_type_read);
		if((check_system_type(cb_inv_type_riio_sun_ii) 
			|| check_system_type(cb_inv_type_riio_sun_ii_gte) 
			|| check_system_type(cb_inv_type_riio_ii))  && (w_riiosunii_lcd_ver < 4192))   //lcd版本低于1.06时将0x0109事件码 对应 高温告警。
		{
		   net_device_data_access(dw_device_id,
                               cb_device_data_access_data_type_status,
                               0x0124,
                               &w_data_len,
                               (uint8_t *)&w_sys_waring2_bit,
                               cb_device_data_access_operate_type_read);
			w_sys_waring2_bit |= ((pst_abnormal_info->st_alarm_bits_info[0].w_abnormal_bits & 0x100) >> 8) << 2;
			net_device_data_access(dw_device_id,
                               cb_device_data_access_data_type_status,
                               0x0124,
                               &w_data_len,
                               (uint8_t *)&w_sys_waring2_bit,
                               cb_device_data_access_operate_type_write);
			pst_abnormal_info->st_alarm_bits_info[0].w_abnormal_bits &= ~(0x0001 << 8);
		}
        pst_abnormal_info->st_alarm_bits_info[0].w_abnormal_code_start = cw_ck_inverter_alarm_code_start_1;
        net_device_data_access(dw_device_id,
                               cb_device_data_access_data_type_status,
                               0x0124,
                               &w_data_len,
                               (uint8_t *)&pst_abnormal_info->st_alarm_bits_info[1].w_abnormal_bits,
                               cb_device_data_access_operate_type_read);
        pst_abnormal_info->st_alarm_bits_info[1].w_abnormal_code_start = cw_ck_inverter_alarm_code_start_2;
        //acin alarm
        net_device_data_access(dw_device_id,
                               cb_device_data_access_data_type_status,
                               0x0125,
                               &w_data_len,
                               (uint8_t *)&pst_abnormal_info->st_alarm_bits_info[2].w_abnormal_bits,
                               cb_device_data_access_operate_type_read);
        pst_abnormal_info->st_alarm_bits_info[2].w_abnormal_code_start = cw_acin_err_code_start;
        //gen alarm
        net_device_data_access(dw_device_id,
                               cb_device_data_access_data_type_status,
                               0x0127,
                               &w_data_len,
                               (uint8_t *)&pst_abnormal_info->st_alarm_bits_info[3].w_abnormal_bits,
                               cb_device_data_access_operate_type_read);
        pst_abnormal_info->st_alarm_bits_info[3].w_abnormal_code_start = cw_gen_alarm_code_start;
        //bms
        net_device_data_access(dw_device_id,
                               cb_device_data_access_data_type_status,
                               0x014C,
                               &w_data_len,
                               (uint8_t *)&pst_abnormal_info->st_alarm_bits_info[4].w_abnormal_bits,
                               cb_device_data_access_operate_type_read);
        pst_abnormal_info->st_alarm_bits_info[4].w_abnormal_code_start = cw_bms_alarm_code_start;
        //bms fault
        net_device_data_access(dw_device_id,
                               cb_device_data_access_data_type_status,
                               0x014B,
                               &w_data_len,
                               (uint8_t *)&pst_abnormal_info->st_alarm_bits_info[5].w_abnormal_bits,
                               cb_device_data_access_operate_type_read);
        pst_abnormal_info->st_alarm_bits_info[5].w_abnormal_code_start = cw_bms_fault_code_start;
        //fault
        net_device_data_access(dw_device_id,
                               cb_device_data_access_data_type_status,
                               0x0121,
                               &w_data_len,
                               (uint8_t *)&pst_abnormal_info->st_fault_bits_info[0].w_abnormal_bits,
                               cb_device_data_access_operate_type_read);
        pst_abnormal_info->st_fault_bits_info[0].w_abnormal_code_start = cw_ck_inverter_fault_code_start_1;
        net_device_data_access(dw_device_id,
                               cb_device_data_access_data_type_status,
                               0x0122,
                               &w_data_len,
                               (uint8_t *)&pst_abnormal_info->st_fault_bits_info[1].w_abnormal_bits,
                               cb_device_data_access_operate_type_read);
        pst_abnormal_info->st_fault_bits_info[1].w_abnormal_code_start = cw_ck_inverter_fault_code_start_2;

        {   //bms set enable
            uint16_t w_data_len = 1;
            uint8_t b_data_temp;
            if(pst_abnormal_info->st_alarm_bits_info[3].w_abnormal_bits & 0x0080)
            {
                b_data_temp = FALSE;
            }
            else
            {
                b_data_temp = TRUE;
            }
            net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                   cb_device_data_access_data_type_set,
                                   0x0037,
                                   &w_data_len,
                                   (uint8_t *)&b_data_temp,
                                   cb_device_data_access_operate_type_write);
        }
    }
    for(uint8_t b_cnt_i = 0;b_cnt_i < cb_max_alarm_bits_num;b_cnt_i++)
    {
        b_result |= ((pst_abnormal_info->st_alarm_bits_info[b_cnt_i].w_abnormal_bits != 0) ? 0x01:0x00);
    }
    for(uint8_t b_cnt_i = 0;b_cnt_i < cb_max_fault_bits_num;b_cnt_i++)
    {
        if(pst_abnormal_info->st_fault_bits_info[b_cnt_i].w_abnormal_bits != 0)
        {
            b_result += 2;
            break;
        }
    }

    return b_result;
}
uint8_t get_mppt_alarm_fault_bits(uint32_t dw_device_id,
                                  abnormal_info *pst_abnormal_info)
{
	#define b_ext_lv_mppt		0
	#define b_inner_mppt		1
	#define b_ext_hv_mppt		2
    uint8_t b_result = 0;
    uint8_t b_protocal_type = cb_system_protocal_type_ck;

    if(FALSE == check_system_protocal_type(&b_protocal_type))
    {
        return b_result;
    }

    if(cb_system_protocal_type_ck == b_protocal_type)
    {
        uint16_t w_data_len = 0;

        pst_abnormal_info->dw_device_id = dw_device_id;
        net_device_data_access(dw_device_id,
                               cb_device_data_access_data_type_status,
                               0x010A,
                               &w_data_len,
                               (uint8_t *)&pst_abnormal_info->st_alarm_bits_info[0].w_abnormal_bits,
                               cb_device_data_access_operate_type_read);
        pst_abnormal_info->st_alarm_bits_info[0].w_abnormal_code_start = cw_ck_type_mppt_alarm_code_start;

        net_device_data_access(dw_device_id,
                               cb_device_data_access_data_type_status,
                               0x0109,
                               &w_data_len,
                               (uint8_t *)&pst_abnormal_info->st_fault_bits_info[0].w_abnormal_bits,
                               cb_device_data_access_operate_type_read);
        pst_abnormal_info->st_fault_bits_info[0].w_abnormal_code_start = cw_ck_type_mppt_fault_code_start;
    }
    else if(cb_system_protocal_type_riiosunii == b_protocal_type)
    {
        uint16_t w_data_len = 2;
        uint16_t w_mppt_identifier = 0;
        uint16_t w_mppt_type = 0;
		uint32_t dw_device_id_lcd = 0;
        uint16_t w_mppt_num = 0;
        net_device_data_access(dw_device_id,
                       cb_device_data_access_data_type_status,
                       0x0104,
                       &w_data_len,
                       (uint8_t *)&w_mppt_identifier,
                       cb_device_data_access_operate_type_read);
        w_mppt_identifier = (w_mppt_identifier >> 8);
		{
			uint8_t b_id = dw_device_id & 0x0000007F;

			if(b_id - 0x2F < 0) 				   dw_device_id_lcd = cdw_device_id_cklcd_u1;
			if(b_id - 0x36 < 0 && b_id - 0x2F > 0) dw_device_id_lcd = cdw_device_id_cklcd_u2;
			if(b_id - 0x3E < 0 && b_id - 0x36 > 0) dw_device_id_lcd = cdw_device_id_cklcd_u3;
			if(b_id - 0x4E < 0 && b_id - 0x3E > 0) dw_device_id_lcd = cdw_device_id_cklcd_v1;
			if(b_id - 0x56 < 0 && b_id - 0x4E > 0) dw_device_id_lcd = cdw_device_id_cklcd_v2;
			if(b_id - 0x5E < 0 && b_id - 0x56 > 0) dw_device_id_lcd = cdw_device_id_cklcd_v3;
			if(b_id - 0x6E < 0 && b_id - 0x5E > 0) dw_device_id_lcd = cdw_device_id_cklcd_w1;
			if(b_id - 0x76 < 0 && b_id - 0x6E > 0) dw_device_id_lcd = cdw_device_id_cklcd_w2;
			if(b_id - 0x7E < 0 && b_id - 0x76 > 0) dw_device_id_lcd = cdw_device_id_cklcd_w3;
			net_device_data_access(dw_device_id_lcd,
							  cb_device_data_access_data_type_set,
							  0x0006,
							  &w_data_len,
							  (uint8_t *)&w_mppt_type,
							  cb_device_data_access_operate_type_read);

		}
		if((check_system_type(cb_inv_type_riio_sun_ii) 
			|| check_system_type(cb_inv_type_riio_sun_ii_split_phase) 
			|| check_system_type(cb_inv_type_riio_sun_ii_gte)) && (w_mppt_type != 0))	//riio sun ii type 不支持sp600
		{
			w_mppt_type = 0;
			net_device_data_access(dw_device_id_lcd,
							  cb_device_data_access_data_type_set,
							  0x0006,
							  &w_data_len,
							  (uint8_t *)&w_mppt_type,
							  cb_device_data_access_operate_type_write);
		}
		if(w_mppt_identifier <= 0x0F && w_mppt_type == 0)   //mppt 1
        {
            pst_abnormal_info->st_alarm_bits_info[0].w_abnormal_code_start = cw_ck_type_mppt_alarm_code_start;
            pst_abnormal_info->st_fault_bits_info[0].w_abnormal_code_start = cw_ck_type_mppt_fault_code_start;
        }
        else if(w_mppt_identifier <= 0x1F && w_mppt_type == 0)  //mppt riiosunii inner
        {
            pst_abnormal_info->st_alarm_bits_info[0].w_abnormal_code_start = cw_riiosunii_type_mppt_alarm_code_start;
            pst_abnormal_info->st_fault_bits_info[0].w_abnormal_code_start = cw_riiosunii_type_mppt_fault_code_start;
        }
        else    //sp600
        {
            pst_abnormal_info->st_alarm_bits_info[0].w_abnormal_code_start = cw_riiosunii_type_sp600_alarm_code_start;
            pst_abnormal_info->st_fault_bits_info[0].w_abnormal_code_start = cw_riiosunii_type_sp600_fault_code_start;
        }
        net_device_data_access(dw_device_id_lcd,
					   cb_device_data_access_data_type_set,
					   0x0001,
					   &w_data_len,
					   (uint8_t *)&w_mppt_num,										 //获取系统mppt数量
					   cb_device_data_access_operate_type_read);
		if(w_mppt_num != 0)
		{
	        pst_abnormal_info->dw_device_id = dw_device_id;
	        net_device_data_access(dw_device_id,
	                               cb_device_data_access_data_type_status,
	                               0x010C,
	                               &w_data_len,
	                               (uint8_t *)&pst_abnormal_info->st_alarm_bits_info[0].w_abnormal_bits,
	                               cb_device_data_access_operate_type_read);


	        net_device_data_access(dw_device_id,
	                               cb_device_data_access_data_type_status,
	                               0x010B,
	                               &w_data_len,
	                               (uint8_t *)&pst_abnormal_info->st_fault_bits_info[0].w_abnormal_bits,
	                               cb_device_data_access_operate_type_read);

		}
		else
		{
			pst_abnormal_info->st_alarm_bits_info[0].w_abnormal_bits = 0;
			pst_abnormal_info->st_fault_bits_info[0].w_abnormal_bits = 0;

		}



    }

    for(uint8_t b_cnt_i = 0;b_cnt_i < cb_max_alarm_bits_num;b_cnt_i++)
    {
        b_result |= ((pst_abnormal_info->st_alarm_bits_info[b_cnt_i].w_abnormal_bits != 0) ? 0x01:0x00);
    }
    for(uint8_t b_cnt_i = 0;b_cnt_i < cb_max_fault_bits_num;b_cnt_i++)
    {
        if(pst_abnormal_info->st_fault_bits_info[b_cnt_i].w_abnormal_bits != 0)
        {
            b_result += 2;
            break;
        }
    }

    return b_result;
}

uint8_t device_alarm_code_fresh(uint32_t dw_device_id,
                                abnormal_info *pst_abnormal_info)
{
    memset(pst_abnormal_info,0,sizeof(abnormal_info));

    switch(dw_device_id & 0xFFFF00)
    {
//        case cdw_device_type_cklcd:
//            {
//
//            }
//            break;
        case cdw_device_type_ck_inverter:
        {
            return get_ck_inverter_alarm_fault_bits(dw_device_id,pst_abnormal_info);
        }
        case cdw_device_type_mppt:
        {
            return get_mppt_alarm_fault_bits(dw_device_id,pst_abnormal_info);
        }
//        case cdw_device_type_pv:
//        {
//            return get_pv_alarm_fault_bits(dw_device_id,pst_abnormal_info);
//        }
        default:
            return 0;
    }
}
/************************************************************************************************
* Input:  void
*
* Output: void
*
* Logic discription: Get device (ck inverter,mppt,...) realtime abnormal code
*                    kinergy data addr :0x0100(fault code),0x0101(alarm code)
*
* run time:
************************************************************************************************/
void network_abnormal_code_fresh()
{
    #define cb_get_device_info_list         0
    #define cb_get_device_index             1
    #define cb_get_device_alarm_code        2
    #define cb_fill_device_alarm_code       3
    #define cb_failed                       4
    #define cb_success                      5

	uint16_t w_device_index = 0;
    net_device_info_list *pst_net_device_info_list = 0;
	uint8_t b_step = cb_get_device_info_list;
    static abnormal_info st_abnormal_info = {0,};
    uint16_t w_data_len_alarm = 0;
    uint16_t w_data_len_fault = 0;
    uint8_t ba_alarm_code_buff[cb_max_alarm_fault_code_len] = {0,};
    uint8_t ba_fault_code_buff[cb_max_alarm_fault_code_len] = {0,};

#if( task_runing_status_print )
    DebugPrintf(TRUE,"Device_error_code_fresh_start\r\n");
#endif

	do{
	    switch(b_step)
	    {
	    	case cb_get_device_info_list:
				{
					uint16_t w_cnt_1 = 0;

					for(w_cnt_1 = 0;w_cnt_1 < st_net_system_info_list.w_length;w_cnt_1++)
					{
						if(st_net_system_info_list.b_current_type ==\
						   st_net_system_info_list.pst_system_info[w_cnt_1].b_type)
						{
                            pst_net_device_info_list =\
                                st_net_system_info_list.pst_system_info[w_cnt_1].pst_device_info_list;
							b_step = cb_get_device_index;
							break;
						}
					}

					if(w_cnt_1 >= st_net_system_info_list.w_length)
					{
						b_step = cb_failed;
					}
				}
	    		break;

	    	case cb_get_device_index:
				{
                    if(w_device_index < pst_net_device_info_list->w_length)
                    {
                        if(cb_net_device_add ==\
                            pst_net_device_info_list->
                            pst_device_info[w_device_index].
                            pst_net_device_status->
                            b_communicate_status)
                        {
                            memset(&st_abnormal_info,0,sizeof(st_abnormal_info));
                            b_step = cb_get_device_alarm_code;
                        }
                        else
                        {
                            w_device_index++;
                        }
                    }
                    else
                    {
                        b_step = cb_success;
                        {
                            uint16_t w_data_len_temp = 100;
                            net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                                   cb_device_data_access_data_type_status,
                                                   0x0101,
                                                   &w_data_len_temp,
                                                   (uint8_t *)&ba_alarm_code_buff[0],
                                                   cb_device_data_access_operate_type_write);
                            net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                                   cb_device_data_access_data_type_status,
                                                   0x0100,
                                                   &w_data_len_temp,
                                                   (uint8_t *)&ba_fault_code_buff[0],
                                                   cb_device_data_access_operate_type_write);
                        }
                    }
				}
	    		break;

	    	case cb_get_device_alarm_code:
                {
                    uint8_t b_result = 0;
                    b_result = device_alarm_code_fresh(*pst_net_device_info_list->
                                                            pst_device_info[w_device_index++].
                                                            pdw_device_id,
                                                       &st_abnormal_info);
                    switch(b_result)
                    {
                        case 3:
                            {
                                w_data_len_alarm += fill_alarm_code(&ba_alarm_code_buff[w_data_len_alarm],
                                                              cb_max_alarm_fault_code_len - w_data_len_alarm,
                                                              &st_abnormal_info);
                                w_data_len_fault += fill_fault_code(&ba_fault_code_buff[w_data_len_fault],
                                                              cb_max_alarm_fault_code_len - w_data_len_fault,
                                                              &st_abnormal_info);
                            }
                            break;
                        case 2:
                            {
                                w_data_len_fault += fill_fault_code(&ba_fault_code_buff[w_data_len_fault],
                                                              cb_max_alarm_fault_code_len - w_data_len_fault,
                                                              &st_abnormal_info);
                            }
                            break;
                        case 1:
                            {
                                w_data_len_alarm += fill_alarm_code(&ba_alarm_code_buff[w_data_len_alarm],
                                                              cb_max_alarm_fault_code_len - w_data_len_alarm,
                                                              &st_abnormal_info);
                                b_step = cb_get_device_index;
                            }
                            break;
                        default:
                            {
                                b_step = cb_get_device_index;
                            }
                            break;
                    }
                }
	    		break;
	    }
	}while((b_step != cb_failed) && (b_step != cb_success));

#if( task_runing_status_print )
    DebugPrintf(TRUE,"Device_error_code_fresh_end\r\n");
#endif
}

/************************************************************************************************
* Input: pb_data_out:       data out buff addr
*        pw_data_out_len:   data out len addr
*        dw_read_log_index: read log index local
*        w_read_log_num:    read log num
*        w_max_data_out_len: max data out buff len
* Output: result
*
* Logic discription: Read flash event log,inculde:
*                    Ck inverter errcode,ck inverter operate log,kinergy log...
*
* run time:
************************************************************************************************/
uint8_t read_event_log(uint8_t *pb_data_out,
                       uint16_t *pw_data_out_len,
                       uint32_t dw_read_log_index,
                       uint16_t w_read_log_num,
                       uint16_t w_max_data_out_len)
{
    if((NULL == pb_data_out) || (NULL == pw_data_out_len))
    {
        return FALSE;
    }
    if((dw_read_log_index + w_read_log_num > st_event_log_manage_info.dw_event_log_end_index) ||\
       (dw_read_log_index < st_event_log_manage_info.dw_event_log_start_index) ||\
       (0 == w_read_log_num) ||\
       (*pw_data_out_len + w_read_log_num * cb_event_log_len >= w_max_data_out_len))
    {
        return FALSE;
    }

    uint16_t w_data_len = 0;
    uint32_t dw_event_log_read_addr = 0;

    pb_data_out[2] = (uint8_t)(w_read_log_num >> 8);
    pb_data_out[3] = (uint8_t)w_read_log_num;
    *pw_data_out_len += 2;

    event_log_info st_event_log_info = {0,};

    for(uint16_t w_cnt_i = 0;w_cnt_i < w_read_log_num;w_cnt_i++)
    {
        dw_event_log_read_addr =\
             st_event_log_manage_info.dw_event_log_save_start_addr +
             (dw_read_log_index + w_cnt_i) * cb_event_log_flash_save_len;
         w_data_len = cb_event_log_flash_save_len;


        if(FALSE == extern_flash_read(dw_event_log_read_addr,
                                      w_data_len,
                                      st_event_log_info.ba_data))
        {
            return FALSE;
        }
        if(st_event_log_info.event_log_fomat.b_event_log_len > cb_event_log_flash_save_len - 1)		//逆变器恢复出厂设置可解决这个退出的问题
        {
            return FALSE;
        }

        if(FALSE == check_crc16(st_event_log_info.ba_data,
                                cb_event_log_flash_save_len))
        {
            return FALSE;
        }

        memcpy(&pb_data_out[*pw_data_out_len],
               &st_event_log_info.ba_data[1],
               st_event_log_info.event_log_fomat.b_event_log_len);

        *pw_data_out_len += st_event_log_info.event_log_fomat.b_event_log_len;
    }

    return TRUE;
}

uint8_t save_event_log(event_log_info *pst_event_log_info)
{
//    static uint8_t ba_event_log_buff[cb_event_log_len] = {0,};
	if(pst_event_save_mutex != 0)
    {
        #if(save_event_log_Semaphore_test)
        DebugPrintf(TRUE,"event_save_SemaphoreTake\n");
        #endif
        xSemaphoreTake(pst_event_save_mutex, portMAX_DELAY);
    }

    if(st_event_log_manage_info.dw_event_log_save_end_addr >= cdw_flash_addr_alloc_event_log_end)
    {
        st_event_log_manage_info.dw_event_log_save_end_addr = cdw_flash_addr_alloc_event_log_start;
    }
    if((st_event_log_manage_info.dw_event_log_save_end_addr == st_event_log_manage_info.dw_event_log_save_start_addr) &&\
       (0 != st_event_log_manage_info.dw_event_log_end_index))
    {
        st_event_log_manage_info.dw_event_log_start_index +=\
            (4 * 1024) / cb_event_log_flash_save_len;

        if(st_event_log_manage_info.dw_event_log_save_start_addr + 4 * 1024 >= cdw_flash_addr_alloc_event_log_end)
        {
            st_event_log_manage_info.dw_event_log_save_start_addr = cdw_flash_addr_alloc_event_log_start;
        }
        else
        {
            st_event_log_manage_info.dw_event_log_save_start_addr += 4 * 1024;
        }
    }

    if(FALSE == calculate_crc16(pst_event_log_info->ba_data,
                                cb_event_log_flash_save_len - 2,
                                &pst_event_log_info->event_log_fomat.b_crc_h,
                                &pst_event_log_info->event_log_fomat.b_crc_l))
    {
        if(pst_event_save_mutex != 0)
        {
            #if(save_event_log_Semaphore_test)
            DebugPrintf(TRUE,"event_save_SemaphoreGive\n");
            #endif
            xSemaphoreGive(pst_event_save_mutex);
        }

        return FALSE;
    }

    if(FALSE == extern_flash_wirte(st_event_log_manage_info.dw_event_log_save_end_addr,
                                   cb_event_log_len,
                                   pst_event_log_info->ba_data))
    {
        event_log_info st_event_log_info_temp = {0,};

        if(TRUE == extern_flash_read(st_event_log_manage_info.dw_event_log_save_end_addr,
                                     cb_event_log_flash_save_len,
                                     st_event_log_info_temp.ba_data))
        {
            if(FALSE == check_crc16(st_event_log_info_temp.ba_data,
                                    cb_event_log_flash_save_len))
            {
                #if( event_status_print )
                DebugPrintf(TRUE,"[Event]Device event log clear\r\n");
                #endif

                spi_flash_erase(st_event_log_manage_info.dw_event_log_save_end_addr,
                                cb_event_log_flash_save_len);

            }
            else
            {
                st_event_log_manage_info.dw_event_log_save_end_addr += cb_event_log_flash_save_len;
                st_event_log_manage_info.dw_event_log_end_index++;
                fWriteEEPage(3) = TRUE;
                fWriteEEPage(4) = TRUE;

                #if( event_status_print )
                DebugPrintf(TRUE,"[Event]Device event log flash fail\r\n");
                #endif

            }

            if(pst_event_save_mutex != 0)
            {
                #if(save_event_log_Semaphore_test)
                DebugPrintf(TRUE,"event_save_SemaphoreGive\n");
                #endif
                xSemaphoreGive(pst_event_save_mutex);
            }
            return FALSE;
        }
    }
    else
    {
        st_event_log_manage_info.dw_event_log_save_end_addr += cb_event_log_flash_save_len;
        st_event_log_manage_info.dw_event_log_end_index++;
        fWriteEEPage(3) = TRUE;
        fWriteEEPage(4) = TRUE;
    }

	if(pst_event_save_mutex != 0)
    {
        #if(net_device_tree_build_Semaphore_test)
        DebugPrintf(TRUE,"event_save_SemaphoreGive\n");
        #endif
        xSemaphoreGive(pst_event_save_mutex);
    }
    #if( event_status_print )
    DebugPrintf(TRUE,"[Event]Device event log save success\r\n");
    #endif

    return TRUE;
}
///*  //mark device operate log
const uint8_t b_unit_null[] = {'\0',};
const uint8_t b_unit_v[] = {'V','\0'};
const uint8_t b_unit_a[] = {'A','\0'};
const uint8_t b_unit_w[] = {'W','\0'};
const uint8_t b_unit_kwh[] = {'k','W','h','\0'};
const uint8_t b_unit_c[] = {'C','\0'};
const uint8_t b_unit_va[] = {'V','A','\0'};
const uint8_t b_unit_hz[] = {'H','z','\0'};
const uint8_t b_unit_ah[] = {'A','h','\0'};
const uint8_t b_unit_per[] = {'%','\0'};
uint8_t fill_operate_value_hex_to_ascii(uint16_t w_operate_adddr,
                                        uint32_t dw_device_id,
                                        uint8_t *pb_data_out,
                                        uint8_t b_max_data_out_len,
                                        uint32_t dw_operate_value)
{
    if(NULL == pb_data_out)
    {
        return 0;
    }

    const net_data_attribute *pst_net_data_attribute = 0;

    if(FALSE == net_get_device_data_attribute(dw_device_id,
                                              cb_net_data_att_set,
                                              w_operate_adddr,
                                              (net_data_attribute **)&pst_net_data_attribute,
                                              &st_net_system_info_list))
    {
        return 0;
    }

    uint8_t b_shift_len = 0;
    uint8_t b_data_len = 0;
    const uint8_t *pb_unit = 0;
    uint8_t f_sign = FALSE;
    uint8_t ba_event_log_str[cb_max_str_description] = {0,};

    switch(pst_net_data_attribute->w_unit)
    {
//        case cw_UnitNULL:
        case cw_UnitNULL0_1:
            {
                b_shift_len = 1;
                pb_unit = b_unit_null;
                f_sign = FALSE;
            }
            break;
        case cw_UnitNULL0_0_1:
            {
                b_shift_len = 2;
                pb_unit = b_unit_null;
                f_sign = FALSE;
            }
            break;
        case cw_UnitNULL0_0_0_1:
            {
                b_shift_len = 3;
                pb_unit = b_unit_null;
                f_sign = FALSE;
            }
            break;
        case cw_UnitNULL0_0_0_0_1:
            {
                b_shift_len = 4;
                pb_unit = b_unit_null;
                f_sign = FALSE;
            }
            break;
        case cw_UnitSTR:
            {
                b_shift_len = 0;
                pb_unit = b_unit_null;
                f_sign = FALSE;
            }
            break;
        case cw_UnSignedUnit0_1V:
            {
                b_shift_len = 1;
                pb_unit = b_unit_v;
                f_sign = FALSE;
            }
            break;
        case cw_UnSignedUnit1_0V:
            {
                b_shift_len = 0;
                pb_unit = b_unit_v;
                f_sign = FALSE;
            }
            break;
        case cw_SignedUnit1_0V:
            {
                b_shift_len = 0;
                pb_unit = b_unit_v;
                f_sign = TRUE;
            }
            break;
        case cw_SignedUnit0_1V:
            {
                b_shift_len = 1;
                pb_unit = b_unit_v;
                f_sign = TRUE;
            }
            break;
        case cw_UnSignedUnit0_0_1V:
            {
                b_shift_len = 2;
                pb_unit = b_unit_v;
                f_sign = FALSE;
            }
            break;
        case cw_SignedUnit0_0_1V:
            {
                b_shift_len = 2;
                pb_unit = b_unit_v;
                f_sign = TRUE;
            }
            break;
        case cw_SignedUnit1_0MV:
            {
                b_shift_len = 3;
                pb_unit = b_unit_v;
                f_sign = TRUE;
            }
            break;
        case cw_UnSignedUnit1_0MV:
            {
                b_shift_len = 3;
                pb_unit = b_unit_v;
                f_sign = FALSE;
            }
            break;
        case cw_UnSignedUnit1_0A:
            {
                b_shift_len = 0;
                pb_unit = b_unit_a;
                f_sign = FALSE;
            }
            break;
        case cw_SignedUnit1_0A:
            {
                b_shift_len = 0;
                pb_unit = b_unit_a;
                f_sign = TRUE;
            }
            break;
        case cw_SignedUnit0_1A:
            {
                b_shift_len = 1;
                pb_unit = b_unit_a;
                f_sign = TRUE;
            }
            break;
        case cw_UnSignedUnit0_1A:
            {
                b_shift_len = 1;
                pb_unit = b_unit_a;
                f_sign = FALSE;
            }
            break;
        case cw_UnSignedUnit0_0_1A:
            {
                b_shift_len = 2;
                pb_unit = b_unit_a;
                f_sign = FALSE;
            }
            break;
        case cw_SignedUnit0_0_1A:
            {
                b_shift_len = 2;
                pb_unit = b_unit_a;
                f_sign = TRUE;
            }
            break;
        case cw_SignedUnit1_0W:
            {
                b_shift_len = 0;
                pb_unit = b_unit_w;
                f_sign = TRUE;
            }
            break;
        case cw_UnSignedUnit1_0W:
            {
                b_shift_len = 0;
                pb_unit = b_unit_w;
                f_sign = FALSE;
            }
            break;
        case cw_UnSignedUnit1_0VA:
            {
                b_shift_len = 0;
                pb_unit = b_unit_va;
                f_sign = FALSE;
            }
            break;
        case cw_SignedUnit1_0VA:
            {
                b_shift_len = 0;
                pb_unit = b_unit_va;
                f_sign = TRUE;
            }
            break;
        case cw_SignedUnit1_0C:
            {
                b_shift_len = 0;
                pb_unit = b_unit_c;
                f_sign = TRUE;
            }
            break;
        case cw_UnSignedUnit0_1C:
            {
                b_shift_len = 1;
                pb_unit = b_unit_c;
                f_sign = FALSE;
            }
            break;
        case cw_SignedUnit0_1C:
            {
                b_shift_len = 1;
                pb_unit = b_unit_c;
                f_sign = TRUE;
            }
            break;
        case cw_UnSignedUnit0_0_1C:
            {
                b_shift_len = 2;
                pb_unit = b_unit_c;
                f_sign = FALSE;
            }
            break;
        case cw_UnSignedUnit0_0_1HZ:
            {
                b_shift_len = 2;
                pb_unit = b_unit_hz;
                f_sign = FALSE;
            }
            break;
        case cw_UnSignedUnit1_0HZ:
            {
                b_shift_len = 0;
                pb_unit = b_unit_hz;
                f_sign = FALSE;
            }
            break;
        case cw_SignedUnit1_0WH:
            {
                b_shift_len = 3;
                pb_unit = b_unit_kwh;
                f_sign = TRUE;
            }
            break;
        case cw_SignedUnit0_1KWH:
            {
                b_shift_len = 2;
                pb_unit = b_unit_kwh;
                f_sign = TRUE;
            }
            break;
        case cw_UnSignedUnit0_1KWH:
            {
                b_shift_len = 2;
                pb_unit = b_unit_kwh;
                f_sign = FALSE;
            }
            break;
        case cw_UnSignedUnit1_0Per:
            {
                b_shift_len = 0;
                pb_unit = b_unit_per;
                f_sign = FALSE;
            }
            break;
        case cw_UnSignedUnit1_0AH:
            {
                b_shift_len = 0;
                pb_unit = b_unit_ah;
                f_sign = FALSE;
            }
            break;
        case cw_UnSignedUnit0_1AH:
            {
                b_shift_len = 1;
                pb_unit = b_unit_ah;
                f_sign = FALSE;
            }
            break;
//        case cw_UnSignedUnit1_0S:
//            {
//                b_shift_len = 0;
//                pb_unit = b_unit_s;
//                f_sign = FALSE;
//            }
//            break;
//        case cw_UnSignedUnit1_0Min:
//            {
//                b_shift_len = 0;
//                pb_unit = b_unit_min;
//                f_sign = FALSE;
//            }
//            break;
//        case cw_UnSignedUnit1_0Hour:
//            {
//                b_shift_len = 0;
//                pb_unit = b_unit_hour;
//                f_sign = FALSE;
//            }
//            break;
        case cw_UnSignedUnit1_0KMH:
            {
                b_shift_len = 0;
                pb_unit = b_unit_kwh;
                f_sign = FALSE;
            }
            break;
//        case cw_UnSignedUnit1_0MH:
//            {
//                b_shift_len = 0;
//                pb_unit = b_unit_per;
//                f_sign = FALSE;
//            }
//            break;
//        case cw_UnSignedUnit0_1MH:
//            {
//                b_shift_len = 0;
//                pb_unit = b_unit_per;
//                f_sign = FALSE;
//            }
//            break;
//        case cw_UnSignedUnit1_0MM:
//            {
//                b_shift_len = 0;
//                pb_unit = b_unit_per;
//                f_sign = FALSE;
//            }
//            break;
//        case cw_UnSignedUnit1_0CM:
//            {
//                b_shift_len = 0;
//                pb_unit = b_unit_per;
//                f_sign = FALSE;
//            }
//            break;
//        case cw_UnSignedUnit1_0M:
//            {
//                b_shift_len = 0;
//                pb_unit = b_unit_per;
//                f_sign = FALSE;
//            }
//            break;
//        case cw_UnSignedUnit0_1M:
//            {
//                b_shift_len = 0;
//                pb_unit = b_unit_per;
//                f_sign = FALSE;
//            }
//            break;
//        case cw_UnSignedUnit1_0RPM:
//            {
//                b_shift_len = 0;
//                pb_unit = b_unit_per;
//                f_sign = FALSE;
//            }
//            break;
        default:
            {
                b_shift_len = 0;
                pb_unit = b_unit_null;
                f_sign = FALSE;
            }
            break;
    }
    b_data_len = sFigure2Str(f_sign,(uint16_t)dw_operate_value,ba_event_log_str,cb_max_str_description);
    b_data_len = sAddPoint2Str(b_data_len,b_shift_len,ba_event_log_str,pb_data_out);
    b_data_len += sAddUnitStr(&pb_data_out[b_data_len],pb_unit);

    return b_data_len;
}
//    */

uint8_t get_event_log(uint8_t *pb_data_in,
                      uint16_t w_data_len,
                      uint16_t w_cmd)
{
    uint8_t b_result = FALSE;

    if(cw_device_cmd_event_log_read == w_cmd)
    {
        if(0 != w_data_len)
        {
            event_log_info st_event_log_info = {0,};
            uint16_t w_data_index = 0;

            memcpy((char *)&st_event_log_info.event_log_fomat.ba_time_stamp[0],
                   (char *)&pb_data_in[w_data_index],4);
            w_data_index += 4;

            uint32_t dw_device_id_temp = 0;

            bytes_to_int_big_to_little_endain(&pb_data_in[w_data_index],
                                              &dw_device_id_temp,
                                              2);
            #if( event_status_print )
            DebugPrintf(TRUE,"[Event]Device event log,local device id:%06X\r\n",dw_device_id_temp);
            #endif

            switch(dw_device_id_temp & 0xFFFF00)
            {
                case cdw_device_type_cklcd       :
                case cdw_device_type_ck_inverter :
                    dw_device_id_temp = dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1];
                    break;
                case cdw_device_type_mppt        :
                    dw_device_id_temp = dwa_device_id_kinergy_ii[cw_device_id_index_mppt_u1_0];
                    break;
                case cdw_device_type_pv          :
                    dw_device_id_temp = dwa_device_id_kinergy_ii[cw_device_id_index_pv_1];
                    break;
                default:
                    dw_device_id_temp = dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1];
                    break;
            }
            int_to_bytes_big_endain(&st_event_log_info.event_log_fomat.ba_device_id[0],
                                    dw_device_id_temp,
                                    4);
            w_data_index += 2;

            #if( event_status_print )
            DebugPrintf(TRUE,"[Event]Device event log,device id:%06X\r\n",dw_device_id_temp);
            #endif

            memcpy((char *)&st_event_log_info.event_log_fomat.event_log_data.event_log_type.ba_event_code[2],
                   (char *)&pb_data_in[w_data_index],2);
            w_data_index += 2;

            st_event_log_info.event_log_fomat.str_description_info.ba_description_len[1] = 1;
            switch(st_event_log_info.event_log_fomat.event_log_data.event_log_type.ba_event_code[2])
            {
                case 0x00:
                case 0x01:
                {
                    st_event_log_info.event_log_fomat.str_description_info.b_event_type = event_log_type_abnormal;
                    break;
                }
                case 0x02:
                {
                    st_event_log_info.event_log_fomat.str_description_info.b_event_type = event_log_type_status;
                    break;
                }
                default:
                    break;
            }
            st_event_log_info.event_log_fomat.b_event_log_len =\
                 14 +\
                 st_event_log_info.event_log_fomat.str_description_info.ba_description_len[1];
			if(st_event_log_info.event_log_fomat.b_event_log_len > 64)
			{
				#if( event_status_print )
				DebugPrintf(TRUE,"[Event]st_event_log_info.event_log_fomat.b_event_log_len2 is:%04X\r\n",st_event_log_info.event_log_fomat.b_event_log_len);
				#endif
			}

            b_result = save_event_log(&st_event_log_info);
        }
        if(TRUE == b_result)
        {
            st_event_log_manage_info.w_event_log_inquire_index++;
        }
    }
#ifdef READ_OPERATE_LOG_BY_UART
    else if(cw_device_cmd_operate_log_read == w_cmd)
    {
//        /*    //mark device operate log
        if(0 != w_data_len)
        {
            event_log_info st_event_log_info = {0,};
            uint16_t w_data_index = 0;
            uint32_t dw_set_value = 0;

            memcpy((char *)&st_event_log_info.event_log_fomat.ba_time_stamp[0],
                   (char *)&pb_data_in[w_data_index],4);
            w_data_index += 4;

            uint32_t dw_device_id_temp = 0;
            uint16_t w_operate_adddr = 0;

            bytes_to_int_big_to_little_endain(&pb_data_in[w_data_index],
                                              &dw_device_id_temp,
                                              2);

            #if( event_status_print )
            DebugPrintf(TRUE,"[Event]Device event log,local device id:%06X\r\n",dw_device_id_temp);
            #endif
			uint8_t b_set_timestamp = 0;
			if(check_system_type(cb_inv_type_ck))
			{
	            switch(dw_device_id_temp)
	            {
	                case cdw_rs485_id_u1       :
						dw_device_id_temp = dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1];
	                    break;
	                case 0xFFFF:    //timestamp
	                    dw_device_id_temp = cdw_device_id_cklcd_u1;
						b_set_timestamp = 1;
	                    break;
	                case 0x0061://reserve logs
	                    st_event_log_manage_info.w_operate_log_inquire_index++;
	                    return FALSE;
	                default:
	                     dw_device_id_temp = cdw_device_id_cklcd_u1;
	                    break;
	            }
			}
			else
			{
	            switch(dw_device_id_temp)
	            {
	                case cdw_rs485_id_u1       :
	                case 0xFFFF:    //timestamp
	                	b_set_timestamp = 1;
	                    dw_device_id_temp = cdw_device_id_cklcd_u1;
	                    break;
	                case 0x0061://reserve logs
	                    st_event_log_manage_info.w_operate_log_inquire_index++;
	                    return FALSE;
	                default:
	                    dw_device_id_temp = dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1];
	                    break;
	            }	
			}

            int_to_bytes_big_endain(&st_event_log_info.event_log_fomat.ba_device_id[0],
                                    dw_device_id_temp,
                                    4);
            w_data_index += 2;

            #if( event_status_print )
            DebugPrintf(TRUE,"[Event]Device event log,device id:%06X\r\n",dw_device_id_temp);
            #endif
            if(1 == b_set_timestamp)
            {
            	w_operate_adddr = 0x00D5;
				st_event_log_info.event_log_fomat.event_log_data.operate_log_type.ba_operate_addr[2] = 0x00;
				st_event_log_info.event_log_fomat.event_log_data.operate_log_type.ba_operate_addr[3] = 0xD5;	
//				memcpy((char *)&st_event_log_info.event_log_fomat.event_log_data.operate_log_type.ba_operate_addr[2],
//					   (char *)0x00D5,
//					   2);

            }
            else
            {
				memcpy((char *)&st_event_log_info.event_log_fomat.event_log_data.operate_log_type.ba_operate_addr[2],
					   (char *)&pb_data_in[w_data_index],
					   2);

            }

//            memcpy((char *)&st_event_log_info.event_log_fomat.event_log_data.operate_log_type.ba_operate_addr[2],
//                   (char *)&pb_data_in[w_data_index],
//                   2);
            bytes_to_int_big_to_little_endain(&pb_data_in[w_data_index],
                                              (uint32_t *)&w_operate_adddr,
                                              2);
            w_data_index += 2;

            #if( event_status_print )
            DebugPrintf(TRUE,"[Event]Device event log,operate addr:%04X\r\n",w_operate_adddr);
            #endif

            if(1 == b_set_timestamp)
            {
                w_operate_adddr = 0x00D5;
                w_data_index -= 2;
                bytes_to_int_big_to_little_endain(&pb_data_in[w_data_index],&dw_set_value,4);
                w_data_index += 4;
            }
            else
            {
                bytes_to_int_big_to_little_endain(&pb_data_in[w_data_index],&dw_set_value,2);
                w_data_index += 2;
            }
            st_event_log_info.event_log_fomat.str_description_info.ba_description_len[1] = 1;
            st_event_log_info.event_log_fomat.str_description_info.ba_description_len[1] +=\
                fill_operate_value_hex_to_ascii(w_operate_adddr,
                                                dw_device_id_temp,
                                                &st_event_log_info.event_log_fomat.str_description_info.ba_str_description[0],
                                                cb_max_str_description,
                                                dw_set_value);
            st_event_log_info.event_log_fomat.str_description_info.b_event_type = event_log_type_operate;
            st_event_log_info.event_log_fomat.b_event_log_len =\
                 14 +\
                 st_event_log_info.event_log_fomat.str_description_info.ba_description_len[1];
			if(st_event_log_info.event_log_fomat.b_event_log_len > 64)
			{
				#if( event_status_print )
					DebugPrintf(TRUE,"[Event]st_event_log_info.event_log_fomat.b_event_log_len1 is:%04X\r\n",st_event_log_info.event_log_fomat.b_event_log_len);
				#endif
			}


            b_result = save_event_log(&st_event_log_info);

        }
        if(TRUE == b_result)
        {
            st_event_log_manage_info.w_operate_log_inquire_index++;
        }
//        */
    }
#endif
    return FALSE;
}

uint8_t event_operate_log_save(uint32_t dw_device_id,
                               uint32_t dw_timestamp,
                               uint16_t w_operate_addr,
                               uint8_t *pb_set_value,
                               uint8_t b_set_value_len,
                               uint8_t b_set_value_type)
{
    if((NULL == pb_set_value) ||\
       (cb_max_str_description < b_set_value_len))
    {
        return FALSE;
    }

    uint8_t ba_str_description[cb_max_str_description] = {0,};
    uint8_t b_description_len = 0;

    if(((event_log_description_type_hex_little_endain == b_set_value_type) ||\
        (event_log_description_type_hex_big_endain == b_set_value_type)) &&\
       (b_set_value_len <= 4))
    {
        uint32_t dw_set_value = 0;

        if(event_log_description_type_hex_little_endain == b_set_value_type)
        {
            bytes_to_int_little_to_little_endain(pb_set_value,&dw_set_value,b_set_value_len);
        }
        else
        {
            bytes_to_int_big_to_little_endain(pb_set_value,&dw_set_value,b_set_value_len);
        }
        b_description_len = fill_operate_value_hex_to_ascii(w_operate_addr,
                                                            dw_device_id,
                                                            ba_str_description,
                                                            cb_max_str_description,
                                                            dw_set_value);
    }
    else if(event_log_description_type_ascii == b_set_value_type)
    {
        for(uint8_t b_cnt_i = 0;b_cnt_i < b_set_value_len;b_cnt_i++)
        {
            if(b_description_len >= cb_max_str_description)
            {
                break;
            }
            else
            {
                ba_str_description[b_description_len++] = pb_set_value[b_cnt_i];
            }
        }
    }
    else
    {
        return FALSE;
    }
    return fill_event_log(dw_device_id,
                          event_log_type_operate,
                          ba_str_description,
                          b_description_len,
                          w_operate_addr,
                          dw_timestamp);
}

uint8_t fill_event_log(uint32_t dw_device_id,
                       network_event_log_type enum_event_log_type,
                       uint8_t *pb_description,
                       uint8_t b_description_len,
                       uint16_t w_event_code,
                       uint32_t dw_timestamp)
{
    if(((NULL == pb_description) &&\
        (0 != b_description_len)) ||\
       (b_description_len > cb_max_str_description))
    {
        return FALSE;
    }

    event_log_info st_event_log_info = {0,};
    uint8_t b_result = TRUE;
    uint32_t dw_timestamp_temp = 0;

    if(0 == dw_timestamp)
    {
        get_system_timestamp(&dw_timestamp_temp);
    }
    else
    {
        dw_timestamp_temp = dw_timestamp;
    }

    b_result &= int_to_bytes_big_endain(&st_event_log_info.event_log_fomat.ba_time_stamp[0],
                                        dw_timestamp_temp,
                                        4);
    b_result &= int_to_bytes_big_endain(&st_event_log_info.event_log_fomat.ba_device_id[0],
                                        dw_device_id,
                                        4);
    b_result &= int_to_bytes_big_endain(&st_event_log_info.event_log_fomat.event_log_data.event_log_type.ba_event_code[2],
                                        w_event_code,
                                        2);

    #if( event_status_print )
    DebugPrintf(TRUE,"[Event]Event log save,device id:%06X\r\n",dw_device_id);
    DebugPrintf(TRUE,"[Event]Event log save,event code:%02X\r\n",w_event_code);
    #endif

    if(TRUE == b_result)
    {
        st_event_log_info.event_log_fomat.str_description_info.ba_description_len[1] = b_description_len + 1;
        st_event_log_info.event_log_fomat.str_description_info.b_event_type = enum_event_log_type;

        if(0 != b_description_len)
        {
            memcpy(st_event_log_info.event_log_fomat.str_description_info.ba_str_description,
                   pb_description,
                   b_description_len);
        }

        st_event_log_info.event_log_fomat.b_event_log_len =\
            14 + st_event_log_info.event_log_fomat.str_description_info.ba_description_len[1];
		if(st_event_log_info.event_log_fomat.b_event_log_len > 64)
		{
			#if( event_status_print )
				DebugPrintf(TRUE,"[Event]st_event_log_info.event_log_fomat.b_event_log_len3 is:%04X\r\n",st_event_log_info.event_log_fomat.b_event_log_len);
			#endif
		}

        b_result &= save_event_log(&st_event_log_info);
    }
    return b_result;
}

uint8_t device_event_log_inquire_check()
{
    uint8_t b_system_type = 0;

    if(FALSE == check_system_protocal_type(&b_system_type))
    {
        return 0;
    }

#ifdef READ_OPERATE_LOG_BY_UART
///*    //mark device operate log
    {
        uint16_t w_data_len = 0;
        uint16_t w_device_log_index = 0;
        uint16_t w_max_device_log_num = 0;

        if(cb_system_protocal_type_ck == b_system_type)
        {
            net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                   cb_device_data_access_data_type_status,
                                   0x0137,
                                   &w_data_len,
                                   (uint8_t *)&w_device_log_index,
                                   cb_device_data_access_operate_type_read);

            net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                   cb_device_data_access_data_type_status,
                                   0x0138,
                                   &w_data_len,
                                   (uint8_t *)&w_max_device_log_num,
                                   cb_device_data_access_operate_type_read);
        }
        else if(cb_system_protocal_type_riiosunii == b_system_type)
        {
            net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                   cb_device_data_access_data_type_status,
                                   0x0134,
                                   &w_data_len,
                                   (uint8_t *)&w_device_log_index,
                                   cb_device_data_access_operate_type_read);

            net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                   cb_device_data_access_data_type_status,
                                   0x0135,
                                   &w_data_len,
                                   (uint8_t *)&w_max_device_log_num,
                                   cb_device_data_access_operate_type_read);
        }

        if(st_event_log_manage_info.w_operate_log_inquire_index < w_max_device_log_num)
        {
            if(st_event_log_manage_info.w_operate_log_inquire_index < w_device_log_index)
            {
                return 2;
            }
            else if(st_event_log_manage_info.w_operate_log_inquire_index > w_device_log_index)
            {
                st_event_log_manage_info.w_operate_log_inquire_index = 0;
            }
        }
        else
        {
            st_event_log_manage_info.w_operate_log_inquire_index = 0;
        }
    }
//    */
#endif

    {
        uint16_t w_data_len = 0;
        uint16_t w_device_log_index = 0;
        uint16_t w_max_device_log_num = 0;

        if(cb_system_protocal_type_ck == b_system_type)
        {
            net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                   cb_device_data_access_data_type_status,
                                   0x0135,
                                   &w_data_len,
                                   (uint8_t *)&w_device_log_index,
                                   cb_device_data_access_operate_type_read);

            net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                   cb_device_data_access_data_type_status,
                                   0x0136,
                                   &w_data_len,
                                   (uint8_t *)&w_max_device_log_num,
                                   cb_device_data_access_operate_type_read);
        }
        else if(cb_system_protocal_type_riiosunii == b_system_type)
        {
            net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                   cb_device_data_access_data_type_status,
                                   0x0132,
                                   &w_data_len,
                                   (uint8_t *)&w_device_log_index,
                                   cb_device_data_access_operate_type_read);

            net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                   cb_device_data_access_data_type_status,
                                   0x0133,
                                   &w_data_len,
                                   (uint8_t *)&w_max_device_log_num,
                                   cb_device_data_access_operate_type_read);
        }

        if(st_event_log_manage_info.w_event_log_inquire_index < w_max_device_log_num)
        {
            if(st_event_log_manage_info.w_event_log_inquire_index < w_device_log_index)
            {
                return 1;
            }
            else if(st_event_log_manage_info.w_event_log_inquire_index > w_device_log_index)
            {
                st_event_log_manage_info.w_event_log_inquire_index = 0;
            }
        }
        else
        {
            st_event_log_manage_info.w_event_log_inquire_index = 0;
        }
    }
    return 0;
}

/************************************************************************************************
* Input: pst_net_device_info : device info
*
* Output: void
*
* Logic discription: Read ck inverter event log,operate log
*
*
* run time:
************************************************************************************************/
void network_event_log_polling_func(net_device_info * const pst_net_device_info)
{
	if(NULL == pst_net_device_info)
	{
		return;
	}
#ifdef EVENT_LOG_SAVE_TEST
    uint8_t ba_event_log_buff[100] = {0,};
    uint16_t dw_data_len = 0;
    uint32_t dw_read_index = 0;

    read_event_log(ba_event_log_buff,
                       &dw_data_len,
                       dw_read_index,
                       1,
                       100);

#endif

//    if(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1] == pst_net_device_info->dw_device_id)
    {
        uint16_t w_data_len = 4;
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x011D,
                               &w_data_len,
                               (uint8_t *)&st_event_log_manage_info.dw_event_log_start_index,
                               cb_device_data_access_operate_type_write);

        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x011E,
                               &w_data_len,
                               (uint8_t *)&st_event_log_manage_info.dw_event_log_end_index,
                               cb_device_data_access_operate_type_write);
    }

    {
        if(TRUE == net_check_device_communicate_status(cb_net_device_loss,pst_net_device_info))
        {
            return;
        }
        else if(TRUE == net_check_device_communicate_status(cb_net_device_add,pst_net_device_info))
        {
            uint8_t b_result = 0;

            b_result = device_event_log_inquire_check();
            uint16_t w_inquire_cmd = 0;
            uint8_t b_data_len = 0;
            uint8_t ba_data[4] = {0,};
            switch(b_result)
            {
                case 1:
                    {
                        w_inquire_cmd = cw_device_cmd_event_log_read;
                        b_data_len = 4;
                        int_to_bytes_big_endain(&ba_data[0],
                                                (uint32_t)st_event_log_manage_info.w_event_log_inquire_index,
                                                2);
                        int_to_bytes_big_endain(&ba_data[2],
                                                (uint32_t)0x0001,
                                                2);
                    }
                    break;
#ifdef READ_OPERATE_LOG_BY_UART
                case 2:
                    {
                        w_inquire_cmd = cw_device_cmd_operate_log_read;
                        b_data_len = 4;
                        int_to_bytes_big_endain(&ba_data[0],
                                                (uint32_t)st_event_log_manage_info.w_operate_log_inquire_index,
                                                2);
                        int_to_bytes_big_endain(&ba_data[2],
                                                (uint32_t)0x0001,
                                                2);
                    }
                    break;
#endif
                default :
                    return;
            }

            if(TRUE == net_device_send_data(pst_net_device_info->b_wired_communication_cable,
                                            cb_wired_network_inquire_cmd,
                                            pst_net_device_info->dw_device_address_send,
                                            w_inquire_cmd,
                                            b_data_len,
                                            (uint8_t *)ba_data,
                                            pst_net_device_info))
            {
                uint16_t w_receive_data_len = 0;
                uint8_t ba_receive_data[200] = {0,};
                uint8_t b_receive_state = 0;

                for(;;)
                {
                    if(TRUE == net_device_receive_data(pst_net_device_info->b_wired_communication_cable,
                                                       cb_wired_network_inquire_cmd,
                                                       pst_net_device_info->dw_device_address_receive,
                                                       w_inquire_cmd,
                                                       &w_receive_data_len,
                                                       ba_receive_data,
                                                       200,
                                                       &b_receive_state,
                                                       pst_net_device_info))
                    {
                        if(cb_wired_network_receive_state_success == b_receive_state)
                        {
                            get_event_log(ba_receive_data,
                                          w_receive_data_len,
                                          w_inquire_cmd);
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
        }
    }
}

void event_log_info_init()
{
    st_event_log_manage_info.dw_event_log_save_start_addr = cdw_flash_addr_alloc_event_log_start;
    st_event_log_manage_info.dw_event_log_save_end_addr = cdw_flash_addr_alloc_event_log_start;

    st_event_log_manage_info.dw_event_log_start_index = 0;
    st_event_log_manage_info.dw_event_log_end_index = 0;

    st_event_log_manage_info.w_event_log_inquire_index = 0;
    st_event_log_manage_info.w_operate_log_inquire_index = 0;
}

void device_event_log_init()
{
    device_event_log_save_Semaphore_creat();
}
