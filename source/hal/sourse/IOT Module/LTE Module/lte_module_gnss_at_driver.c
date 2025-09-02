#include "lte_module_gnss_at_driver.h"
#include "lte_module.h"
#include <string.h>
#include <stdio.h>
#include "portable.h"

//AT+QGPSCFG=\"outport\"   配置 NMEA 语句输出端口    			usbnmea:通过USB NMEA口输出
static uint8_t lte_module_driver_send_at_gps_config_ouput(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	pst_lte_module_data_info->w_data_len = sprintf((char *)pst_lte_module_data_info->pb_data,"AT+QGPSCFG=\"outport\",\"usbnmea\"\r\n");

	if(0 == pst_lte_module_data_info->w_data_len)
	{
		return 0;
	}

	return 1;
}

static uint8_t lte_module_driver_receive_at_gps_config_ouput(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if((pst_lte_module_data_info->w_data_len <= 8) &&\
	   (strstr((char*)pst_lte_module_data_info->pb_data,"OK") != 0))
	{
		return 1;
	}

	return 2;
}
 
//AT+QGPSCFG=\"nmeasrc\" 启用/禁用通过 AT+QGPSGNMEA 获取 NMEA 语句  0：禁用、1：启用
static uint8_t lte_module_driver_send_at_gps_config_nmeastr(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	pst_lte_module_data_info->w_data_len = sprintf((char *)pst_lte_module_data_info->pb_data,"AT+QGPSCFG=\"nmeasrc\",0\r\n");

	if(0 == pst_lte_module_data_info->w_data_len)
	{
		return 0;
	}

	return 1;
}
static uint8_t lte_module_driver_receive_at_gps_config_nmeastr(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if((pst_lte_module_data_info->w_data_len <= 8) &&\
	   (strstr((char*)pst_lte_module_data_info->pb_data,"OK") != 0))
	{
		return 1;
	}

	return 2;
}

//AT+QGPSCFG="gpsnmeatype" 配置 GPS NMEA 语句的输出类型  0 禁用；1 GPGGA;2 GPRMC;4 GPGSV;8 GPGSA;16 GPVTG;31 输出全部5种类型语句 
static uint8_t lte_module_driver_send_at_gps_config_gpsnmeatype(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	pst_lte_module_data_info->w_data_len =\
		sprintf((char *)pst_lte_module_data_info->pb_data,\
			    "AT+QGPSCFG=\"gpsnmeatype\",2\r\n");

	if(0 == pst_lte_module_data_info->w_data_len)
	{
		return 0;
	}

	return 1;
}
static uint8_t lte_module_driver_receive_at_gps_config_gpsnmeatype(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if((pst_lte_module_data_info->w_data_len <= 8) &&\
	   (strstr((char*)pst_lte_module_data_info->pb_data,"OK") != 0))
	{
		return 1;
	}

	return 2;
}

//AT+QGPSCFG="glonassnmeatype"  配置 GLONASS NMEA 语句的输出类型				0：禁用	1：GLGSV
static uint8_t lte_module_driver_send_at_gps_config_glonassnmeatype(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	pst_lte_module_data_info->w_data_len =\
		sprintf((char *)pst_lte_module_data_info->pb_data,\
			    "AT+QGPSCFG=\"glonassnmeatype\",0\r\n");

	if(0 == pst_lte_module_data_info->w_data_len)
	{
		return 0;
	}

	return 1;
}
static uint8_t lte_module_driver_receive_at_gps_config_glonassnmeatype(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if((pst_lte_module_data_info->w_data_len <= 8) &&\
	   (strstr((char*)pst_lte_module_data_info->pb_data,"OK") != 0))
	{
		return 1;
	}

	return 2;
}

//AT+QGPSCFG="galileonmeatype" 	配置 Galileo NMEA  语句的输出类型	0：禁用	1：GLGSV	
static uint8_t lte_module_driver_send_at_gps_config_galileonmeatype(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	pst_lte_module_data_info->w_data_len =\
		sprintf((char *)pst_lte_module_data_info->pb_data,\
			    "AT+QGPSCFG=\"galileonmeatype\",0\r\n");

	if(0 == pst_lte_module_data_info->w_data_len)
	{
		return 0;
	}

	return 1;
}
static uint8_t lte_module_driver_receive_at_gps_config_galileonmeatype(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if((pst_lte_module_data_info->w_data_len <= 8) &&\
	   (strstr((char*)pst_lte_module_data_info->pb_data,"OK") != 0))
	{
		return 1;
	}

	return 2;
}

//AT+QGPSCFG="beidounmeatype" 配置 BDS NMEA 语句的输出类型	0 禁用；1 GPGGA;2 GPRMC;4 GPGSV;8 GPGSA;16 GPVTG;31 输出全部5种类型语句 
static uint8_t lte_module_driver_send_at_gps_config_beidounmeatype(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	pst_lte_module_data_info->w_data_len =\
		sprintf((char *)pst_lte_module_data_info->pb_data,\
			    "AT+QGPSCFG=\"beidounmeatype\",0\r\n");

	if(0 == pst_lte_module_data_info->w_data_len)
	{
		return 0;
	}

	return 1;
}
static uint8_t lte_module_driver_receive_at_gps_config_beidounmeatype(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if((pst_lte_module_data_info->w_data_len <= 8) &&\
	   (strstr((char*)pst_lte_module_data_info->pb_data,"OK") != 0))
	{
		return 1;
	}

	return 2;
}

//AT+QGPS?  查询当前 GNSS 会话状态			<GNSS_state>	0 GNSS关闭;1 GNSS打开
static uint8_t lte_module_driver_send_at_gps_activation_query(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	pst_lte_module_data_info->w_data_len = sprintf((char *)pst_lte_module_data_info->pb_data,"AT+QGPS?\r\n");

	if(0 == pst_lte_module_data_info->w_data_len)
	{
		return 0;
	}

	return 1;
}
static uint8_t lte_module_driver_receive_at_gps_activation_query(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    uint32_t dw_data_temp1 = 0;

    if(sscanf(strstr((char*)pst_lte_module_data_info->pb_data,"+QGPS:"),"+QGPS: %d",&dw_data_temp1) > 0)
    {
        if(0 == dw_data_temp1)
        {
        	return 4;
        }
        else if(1 == dw_data_temp1)
        {
        	return 1;
        }
    }

    return 2;
}
 
//AT+QGPS  设置命令 <GNSS_mode>1 独立模式 
//执行 AT+QGPS=1 打开 GNSS，且 AP 侧 log 输出“GNSS Open Success” 才表示 GNSS 已完全启动，此时方可进行其他 GNSS 相关操作。
static uint8_t lte_module_driver_send_at_gps_turn_on(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	pst_lte_module_data_info->w_data_len = sprintf((char *)pst_lte_module_data_info->pb_data,"AT+QGPS=1\r\n");

	if(0 == pst_lte_module_data_info->w_data_len)
	{
		return 0;
	}

	return 1;
}
static uint8_t lte_module_driver_receive_at_gps_turn_on(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if((pst_lte_module_data_info->w_data_len <= 8) &&\
	   (strstr((char*)pst_lte_module_data_info->pb_data,"OK") != 0))
	{
		return 1;
	}

	return 2;
}

uint8_t lte_module_driver_gps_turn_on(lte_module_driver_config_info_general *pst_config_info)
{
	if(NULL == pst_config_info)
	{
		return FALSE;
	}

	#define cb_lte_gps_config_ouput									0
	#define cb_lte_gps_config_nmeastr								1
	#define cb_lte_gps_config_gpsnmeatype							2
	#define cb_lte_gps_config_glonassnmeatype						3
	#define cb_lte_gps_config_galileonmeatype						4
	#define cb_lte_gps_config_beidounmeatype						5
	#define cb_lte_gps_activation_query								6
	#define cb_lte_gps_turn_on										7
	#define cb_lte_gps_qgpsloc										8
	#define cb_lte_gps_turn_on_none									9
	uint8_t b_step = cb_lte_gps_config_ouput;

    lte_module_driver_process_contral st_process_contral =
    {
        .st_operate_result.b_iot_module_type = iot_module_type_lte,
        .st_operate_result.b_iot_module_operation = lte_module_operate_turn_on_gps,
        .st_operate_result.b_err_code = lte_module_err_none,
//        .st_operate_result.dw_operate_secquence = pst_config_info->dw_operate_secquence,
        .st_operate_result.pv_data = 0,
        .pst_config_info = pst_config_info,
    };

	do
	{
		switch(b_step)
		{
			case cb_lte_gps_config_ouput:
			{
				if(FALSE == lte_module_driver_at_cmd_transmit(1000,pst_config_info,0,
															 lte_module_driver_send_at_gps_config_ouput,
															 lte_module_driver_receive_at_gps_config_ouput))
				{
					st_process_contral.st_operate_result.b_err_code = gps_turn_on_fail;
					b_step = cb_lte_gps_turn_on_none;
					break;
				}

				b_step = cb_lte_gps_config_nmeastr;
			{
			case cb_lte_gps_config_nmeastr:
			{
                if(FALSE == lte_module_driver_at_cmd_transmit(1000,pst_config_info,0,
                                                             lte_module_driver_send_at_gps_config_nmeastr,
                                                             lte_module_driver_receive_at_gps_config_nmeastr))
               	{
					st_process_contral.st_operate_result.b_err_code = gps_turn_on_fail;
					b_step = cb_lte_gps_turn_on_none;
					break;
				}

				b_step = cb_lte_gps_config_gpsnmeatype;
			{
			case cb_lte_gps_config_gpsnmeatype:
			{
                if(FALSE == lte_module_driver_at_cmd_transmit(1000,pst_config_info,0,
                                                             lte_module_driver_send_at_gps_config_gpsnmeatype,
                                                             lte_module_driver_receive_at_gps_config_gpsnmeatype))
                {
					st_process_contral.st_operate_result.b_err_code = gps_turn_on_fail;
					b_step = cb_lte_gps_turn_on_none;
					break;
				}

				b_step = cb_lte_gps_activation_query;
			{
//			case cb_lte_gps_config_glonassnmeatype:
//			{
//                if(FALSE == lte_module_driver_at_cmd_transmit(100,pst_config_info,0,
//                                                             lte_module_driver_send_at_gps_config_glonassnmeatype,
//                                                             lte_module_driver_receive_at_gps_config_glonassnmeatype))
//                {
//					st_process_contral.st_operate_result.b_err_code = gps_turn_on_fail;
//					b_step = cb_lte_gps_turn_on_none;
//					break;
//				}
//
//				b_step = cb_lte_gps_config_galileonmeatype;
//			{
//			case cb_lte_gps_config_galileonmeatype:
//			{
//                if(FALSE == lte_module_driver_at_cmd_transmit(100,pst_config_info,0,
//                                                             lte_module_driver_send_at_gps_config_galileonmeatype,
//                                                             lte_module_driver_receive_at_gps_config_galileonmeatype))
//                {
//					st_process_contral.st_operate_result.b_err_code = gps_turn_on_fail;
//					b_step = cb_lte_gps_turn_on_none;
//					break;
//				}
//
//				b_step = cb_lte_gps_config_beidounmeatype;
//			{
//			case cb_lte_gps_config_beidounmeatype:
//			{
//                if(FALSE == lte_module_driver_at_cmd_transmit(100,pst_config_info,0,
//                                                             lte_module_driver_send_at_gps_config_beidounmeatype,
//                                                             lte_module_driver_receive_at_gps_config_beidounmeatype))
//                {
//					st_process_contral.st_operate_result.b_err_code = gps_turn_on_fail;
//					b_step = cb_lte_gps_turn_on_none;
//					break;
//				}
//
//				b_step = cb_lte_gps_activation_query;
//			{
			case cb_lte_gps_activation_query:
			{
				uint8_t b_result = 0;

                b_result = lte_module_driver_at_cmd_transmit(1000,pst_config_info,0,
                                                             lte_module_driver_send_at_gps_activation_query,
                                                             lte_module_driver_receive_at_gps_activation_query);

                if(0 == b_result)
                {
					st_process_contral.st_operate_result.b_err_code = gps_turn_on_fail;
					b_step = cb_lte_gps_turn_on_none;
					break;
                }

				if(1 == b_result)
				{
					st_process_contral.st_operate_result.b_err_code = mqtts_send_user_data_success;
                    b_step = cb_lte_gps_turn_on_none;
					break;
				}

				if(2 == b_result)
                {
                    b_step = cb_lte_gps_turn_on;
                }
			{
			case cb_lte_gps_turn_on:
			{
                if(FALSE == lte_module_driver_at_cmd_transmit(1000,pst_config_info,0,
                                                             lte_module_driver_send_at_gps_turn_on,
                                                             lte_module_driver_receive_at_gps_turn_on))
                {
					st_process_contral.st_operate_result.b_err_code = gps_turn_on_fail;
					b_step = cb_lte_gps_turn_on_none;
					break;
				}

				st_process_contral.st_operate_result.b_err_code = gps_turn_on_success;
				b_step = cb_lte_gps_turn_on_none;
			{
			case cb_lte_gps_turn_on_none:
			{
			{
			default:
			{
			}break;
		}

		if(b_step != cb_lte_gps_turn_on_none)
		{
        	vTaskDelay(5);
		}
	}while(b_step != cb_lte_gps_turn_on_none);

	if((0 == pst_config_info) ||\
	   (0 == pst_config_info->pfunc_operate_result_notify) ||\
	   (FALSE == pst_config_info->pfunc_operate_result_notify(&st_process_contral.st_operate_result)) &&\
	   (st_process_contral.st_operate_result.pv_data != 0))
	{
		lte_module_driver_reponse_info_general *pst_reponse_info = (lte_module_driver_reponse_info_general *)st_process_contral.st_operate_result.pv_data;

        if(pst_reponse_info && pst_reponse_info->pst_data)
        {
            vPortFree(pst_reponse_info->pst_data->pb_data);
        }
        if(pst_reponse_info)
        {
            vPortFree(pst_reponse_info->pst_data);
        }
        if(pst_reponse_info)
        {
            vPortFree(pst_reponse_info->union_reponse_info.st_receive_mqtt_user_data.pb_payload_str);
        }
		vPortFree(pst_reponse_info);
	}

	return TRUE;
}

uint8_t lte_module_driver_gps_turn_off(lte_module_driver_config_info_general *pst_config_info)
{
	return TRUE;
}

//AT+QGPSLOC
static uint8_t lte_module_driver_send_at_gps_qgpsloc(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	pst_lte_module_data_info->w_data_len = sprintf((char *)pst_lte_module_data_info->pb_data,"AT+QGPSLOC=2\r\n");

	if(0 == pst_lte_module_data_info->w_data_len)
	{
		return 0;
	}

	return 1;
}
static uint8_t lte_module_driver_receive_at_gps_qgpsloc(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if((0 == pv_user_data) || (0 == pst_lte_module_data_info))
    {
        return 0;
    }

    #define cw_lte_gps_loc_info_len     200
    static uint16_t w_lte_gps_loc_info_len = 0;
    static uint8_t ba_lte_gps_loc_info[cw_lte_gps_loc_info_len] = {0,};
    static uint8_t b_lte_gps_loc_info_rec_step = 0;

    if(0 == b_lte_gps_loc_info_rec_step)
    {
        if(strstr((char*)pst_lte_module_data_info->pb_data,"+CME ERROR:") != 0)
        {
            w_lte_gps_loc_info_len = 0;
            return 2;
        }

        if(0 == strstr((char*)pst_lte_module_data_info->pb_data,"+QGPSLOC:"))
        {
            w_lte_gps_loc_info_len = 0;
            return 2;
        }

        b_lte_gps_loc_info_rec_step = 1;
    }

    if((w_lte_gps_loc_info_len + pst_lte_module_data_info->w_data_len) < cw_lte_gps_loc_info_len)
    {
        memcpy(&ba_lte_gps_loc_info[w_lte_gps_loc_info_len],pst_lte_module_data_info->pb_data,pst_lte_module_data_info->w_data_len);
        w_lte_gps_loc_info_len += pst_lte_module_data_info->w_data_len;

        if((0 == strstr((char*)ba_lte_gps_loc_info,"+QGPSLOC:")) ||\
           (0 == strstr((char*)ba_lte_gps_loc_info,"OK")))
        {
            return 2;
        }
    }

    b_lte_gps_loc_info_rec_step = 0;
    w_lte_gps_loc_info_len = 0;

    uint8_t ba_time[10] = {0,};
    uint8_t ba_data[6] = {0,};
    uint8_t ba_reserve[20];
    lte_module_driver_process_contral *pst_process_contral_info = (lte_module_driver_process_contral *)pv_user_data;
    lte_module_driver_reponse_info_general *pst_network_iot_reponse_info =\
        (lte_module_driver_reponse_info_general *)pvPortMallocUser(sizeof(lte_module_driver_reponse_info_general),TRUE);

    receive_gps_info *pst_receive_gps_info = &pst_network_iot_reponse_info->union_reponse_info.st_receive_gps_info;

    if(sscanf(strstr((char*)ba_lte_gps_loc_info,"+QGPSLOC:"),"+QGPSLOC: %10[^,],%20[^,],%20[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%6[^,],%[^\r]\r",
                            ba_time,
                            pst_receive_gps_info->ba_latitude,
                            pst_receive_gps_info->ba_longitude,
                            ba_reserve,ba_reserve,ba_reserve,ba_reserve,ba_reserve,ba_reserve,
                            ba_data,ba_reserve) < 0)
    {
        vPortFree(pst_network_iot_reponse_info);
        return 0;
    }

    pst_receive_gps_info->ba_utc[0] = (uint8_t)((ba_time[4] - 0x30) << 4) | (uint8_t)((ba_time[5] - 0x30) << 0);
    pst_receive_gps_info->ba_utc[1] = (uint8_t)((ba_time[2] - 0x30) << 4) | (uint8_t)((ba_time[3] - 0x30) << 0);
    pst_receive_gps_info->ba_utc[2] = (uint8_t)((ba_time[0] - 0x30) << 4) | (uint8_t)((ba_time[1] - 0x30) << 0);
    pst_receive_gps_info->ba_utc[3] = (uint8_t)((ba_data[0] - 0x30) << 4) | (uint8_t)((ba_data[1] - 0x30) << 0);
    pst_receive_gps_info->ba_utc[4] = (uint8_t)((ba_data[2] - 0x30) << 4) | (uint8_t)((ba_data[3] - 0x30) << 0);
    pst_receive_gps_info->ba_utc[5] = (uint8_t)((ba_data[4] - 0x30) << 4) | (uint8_t)((ba_data[5] - 0x30) << 0);

    pst_process_contral_info->st_operate_result.pv_data = pst_network_iot_reponse_info;

	return 1;
}

uint8_t lte_module_driver_gps_acquire_positioning_info(lte_module_driver_config_info_general *pst_config_info)
{
	if(NULL == pst_config_info)
	{
		return FALSE;
	}

	#define cb_lte_gps_acquire_positioning_info									0
	#define cb_lte_gps_acquire_positioning_info_none							1
	uint8_t b_step = cb_lte_gps_config_ouput;

    lte_module_driver_process_contral st_process_contral =
    {
        .st_operate_result.b_iot_module_type = iot_module_type_lte,
        .st_operate_result.b_iot_module_operation = lte_module_operate_acquire_positioning_info,
        .st_operate_result.b_err_code = gps_acquire_positioning_info_fail,
//        .st_operate_result.dw_operate_secquence = pst_config_info->dw_operate_secquence,
        .st_operate_result.pv_data = 0,
        .pst_config_info = pst_config_info,
    };

	do
	{
		switch(b_step)
		{
			case cb_lte_gps_acquire_positioning_info:
			{
                if(FALSE == lte_module_driver_at_cmd_transmit(1000,&st_process_contral,0,
                                                             lte_module_driver_send_at_gps_qgpsloc,
                                                             lte_module_driver_receive_at_gps_qgpsloc))
                {
					st_process_contral.st_operate_result.b_err_code = gps_acquire_positioning_info_fail;
					b_step = cb_lte_gps_acquire_positioning_info_none;
					break;
				}

				st_process_contral.st_operate_result.b_err_code = gps_acquire_positioning_info_success;
				b_step = cb_lte_gps_acquire_positioning_info_none;
			{
			case cb_lte_gps_acquire_positioning_info_none:
			{
			{
			default:
			{
			}break;
		}

		if(b_step != cb_lte_gps_acquire_positioning_info_none)
		{
        	vTaskDelay(5);
		}
	}while(b_step != cb_lte_gps_acquire_positioning_info_none);

	if((0 == pst_config_info) ||\
	   (0 == pst_config_info->pfunc_operate_result_notify) ||\
	   (FALSE == pst_config_info->pfunc_operate_result_notify(&st_process_contral.st_operate_result)) &&\
	   (st_process_contral.st_operate_result.pv_data != 0))
	{
		lte_module_driver_reponse_info_general *pst_reponse_info = (lte_module_driver_reponse_info_general *)st_process_contral.st_operate_result.pv_data;

        if(pst_reponse_info && pst_reponse_info->pst_data)
        {
            vPortFree(pst_reponse_info->pst_data->pb_data);
        }
        if(pst_reponse_info)
        {
            vPortFree(pst_reponse_info->pst_data);
        }
        if(pst_reponse_info)
        {
            vPortFree(pst_reponse_info->union_reponse_info.st_receive_mqtt_user_data.pb_payload_str);
        }
		vPortFree(pst_reponse_info);
	}

	return TRUE;
}
