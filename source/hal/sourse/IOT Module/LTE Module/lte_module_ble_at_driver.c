#include "lte_module_ble_at_driver.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "freertos_user_config.h"
#include "network_protocal_data_assembly.h"
#include "network_tbb_device_server_communicate.h"
#include "network_ble_communication_queue.h"
#include "printf_custom.h"
#include "network_local_device_secquence_operate.h"


//static uint8_t b_lte_ble_channel_idx = 0;

/***************************************************************************
//Ble Config
***************************************************************************/
//AT+BLEINIT=0
uint8_t lte_module_driver_send_at_ble_deinit(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+BLEINIT=0\r\n"))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_module_driver_receive_at_ble_deinit(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if(strstr((char*)pst_lte_module_data_info->pb_data,"\r\nOK") != 0)
    {
        return 1;
    }

	return 2;
}

//AT+BLEINIT=2
uint8_t lte_module_driver_send_at_ble_init(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+BLEINIT=2\r\n"))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_module_driver_receive_at_ble_init(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if(strstr((char*)pst_lte_module_data_info->pb_data,"\r\nOK") != 0)
    {
        return 1;
    }

	return 2;
}
//AT+BLEDISCONN=0
uint8_t lte_module_driver_send_at_ble_disconn(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+BLEDISCONN=0\r\n"))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_module_driver_receive_at_ble_disconn(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if(strstr((char*)pst_lte_module_data_info->pb_data,"\r\nOK") != 0)
    {
        return 1;
    }

	return 2;
}

//AT+BLEGATTSSRVCRE
uint8_t lte_module_driver_send_at_ble_ssrv(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+BLEGATTSSRVCRE\r\n"))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_module_driver_receive_at_ble_ssrv(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if(strstr((char*)pst_lte_module_data_info->pb_data,"\r\nOK") != 0)
    {
        return 1;
    }
    else if(strstr((char*)pst_lte_module_data_info->pb_data,"\r\nERROR") != NULL)
    {
        return 1;
    }

	return 2;
}
//AT+BLEADVDATAEX
uint8_t lte_module_driver_send_at_ble_advdataex(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    const uint8_t *pb_ble_adv_data = "57434D5F424C45";//WCM_BLE

	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,
                    "AT+BLEADVDATAEX=\"WCM_%s\",\"02A0\",\"02010607%s030302A0\",1\r\n",
                    &ba_local_device_secquence[0],
                    pb_ble_adv_data))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_module_driver_receive_at_ble_advdataex(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if(strstr((char*)pst_lte_module_data_info->pb_data,"\r\nOK") != 0)
    {
        return 1;
    }

	return 2;
}
//AT+BLEADVPARAM=50,50,0,0,7,0,,
uint8_t lte_module_driver_send_at_ble_adv_para(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,
                    "AT+BLEADVPARAM=50,50,0,0,7,0,,\r\n"))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_module_driver_receive_at_ble_adv_para(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if(strstr((char*)pst_lte_module_data_info->pb_data,"\r\nOK") != 0)
    {
        return 1;
    }

	return 2;
}
//AT+BLEADVDATA=data
uint8_t lte_module_driver_send_at_ble_advdata(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    uint8_t ba_sn_ascii[60] = {0,};

    for(uint8_t b_cnt_i = 0;(b_cnt_i < 30) && (ba_local_device_secquence[b_cnt_i] != '\0');b_cnt_i++)
    {
        if(0 == sprintf((char *)&ba_sn_ascii[b_cnt_i*2],
                "%02X",
                ba_local_device_secquence[b_cnt_i]))
        {
            return 0;
        }
    }

//    if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,
//                    "AT+BLEADVDATA=\"020106%02X0957434D5F%s030302A0\"\r\n",
//                    (strlen((char *)ba_sn_ascii)/2 + 5),
//                    &ba_sn_ascii[0]))
//	{
//		return 0;
//	}

	if (memchr(ba_local_device_secquence, '/', sizeof(ba_local_device_secquence)) != NULL) 
	{
	    if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,
                "AT+BLEADVDATA=\"020106%02X09%s030302A0\"\r\n",
                (strlen((char *)ba_sn_ascii)/2 + 1),
                &ba_sn_ascii[0]))
		{
			return 0;
		}
	} 
	else 
	{
		  if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,
						  "AT+BLEADVDATA=\"020106%02X0957434D5F%s030302A0\"\r\n",
						  (strlen((char *)ba_sn_ascii)/2 + 5),
						  &ba_sn_ascii[0]))
		{
			return 0;
		}

	}


	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_module_driver_receive_at_ble_advdata(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if((pst_lte_module_data_info->w_data_len < 8) &&\
       (strstr((char*)pst_lte_module_data_info->pb_data,"OK") != 0))
    {
        return 1;
    }

	return 2;
}
//AT+BLEADDR?
uint32_t dwa_ble_addr[6] = {0,};
uint8_t lte_module_driver_send_at_ble_addr(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+BLEADDR?\r\n"))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_module_driver_receive_at_ble_addr(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    uint8_t *pb_data = 0;

    pb_data = (uint8_t *)strstr((char*)pst_lte_module_data_info->pb_data,"+BLEADDR");
    if(pb_data)
    {
        uint8_t ba_ble_addr[6][2] = {0,};

        for(uint8_t b_cnt_i = 0;b_cnt_i < 6;b_cnt_i++)
        {
            for(uint8_t b_cnt_j = 0;b_cnt_j < 2;b_cnt_j++)
            {
                ba_ble_addr[b_cnt_i][b_cnt_j] = pb_data[10 + (b_cnt_i * 3) + b_cnt_j];
            }
            sscanf((char *)ba_ble_addr[b_cnt_i],"%x",&dwa_ble_addr[b_cnt_i]);
//            dwa_ble_addr[b_cnt_i] = atoi((const char *)ba_ble_addr[b_cnt_i]);
        }
        return 1;
    }

	return 2;
}
//AT+BLEGATTSSRVSTART
uint8_t lte_module_driver_send_at_ble_ssrv_start(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+BLEGATTSSRVSTART\r\n"))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_module_driver_receive_at_ble_ssrv_start(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if((pst_lte_module_data_info->w_data_len < 8) &&\
       (strstr((char*)pst_lte_module_data_info->pb_data,"OK") != 0))
    {
        return 1;
    }

	return 2;
}
//AT+BLEADVSTART
uint8_t lte_module_driver_send_at_ble_adv_start(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+BLEADVSTART\r\n"))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_module_driver_receive_at_ble_adv_start(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if((pst_lte_module_data_info->w_data_len < 8) &&\
       (strstr((char*)pst_lte_module_data_info->pb_data,"OK") != 0))
    {
        return 1;
    }

	return 2;
}


uint8_t lte_module_ble_config(lte_module_driver_config_info_general *pst_config_info)
{
    if(0 == pst_config_info)
    {
        return FALSE;
    }

    #define cb_lte_module_initial_wifi_disable              0
    #define cb_lte_module_initial_ble_init                  1
    #define cb_lte_module_initial_ble_ssrv                  2
    #define cb_lte_module_initial_ble_adv_para              3
    #define cb_lte_module_initial_ble_advdata               4
    #define cb_lte_module_initial_ble_ssrv_start            5
    #define cb_lte_module_initial_ble_addr                  6
    #define cb_lte_module_initial_ble_advstart              7
    #define cb_lte_module_initial_ble_none                  8

    uint8_t b_step = cb_lte_module_initial_ble_init;//mark
    uint8_t b_err_cnt = 0;

    void *pv_user_data = (void *)pst_config_info;

    lte_module_driver_process_contral st_process_contral =
    {
        .st_operate_result.b_iot_module_type = iot_module_type_lte,
        .st_operate_result.b_iot_module_operation = lte_module_operate_ble_config,
        .st_operate_result.b_err_code = ble_config_fail,
    };

    do
    {
        b_err_cnt++;
        if(b_err_cnt > 3)
        {
            break;
        }
        switch(b_step)
        {
//            case cb_lte_module_initial_wifi_disable:
//            {
//                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
//                                                             lte_module_driver_send_at_wifi_disable,
//                                                             lte_module_driver_receive_at_wifi_disable))
//                {
//                    b_err_cnt = 0;
//                    b_step = cb_lte_module_initial_ble_init;
//                }
//            }break;
            case cb_lte_module_initial_ble_init:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
                                                             lte_module_driver_send_at_ble_init,
                                                             lte_module_driver_receive_at_ble_init))
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_initial_ble_ssrv;
                }
            }break;
            case cb_lte_module_initial_ble_ssrv:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
                                                             lte_module_driver_send_at_ble_ssrv,
                                                             lte_module_driver_receive_at_ble_ssrv))
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_initial_ble_adv_para;
                }
            }break;
            case cb_lte_module_initial_ble_adv_para:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
                                                             lte_module_driver_send_at_ble_adv_para,
                                                             lte_module_driver_receive_at_ble_adv_para))
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_initial_ble_advdata;
                }
            }break;
            case cb_lte_module_initial_ble_advdata:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
                                                             lte_module_driver_send_at_ble_advdata,
                                                             lte_module_driver_receive_at_ble_advdata))
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_initial_ble_addr;
                }
            }break;
            case cb_lte_module_initial_ble_addr:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
                                                             lte_module_driver_send_at_ble_addr,
                                                             lte_module_driver_receive_at_ble_addr))//mark ble addr
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_initial_ble_ssrv_start;
                }
            }break;
            case cb_lte_module_initial_ble_ssrv_start:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
                                                             lte_module_driver_send_at_ble_ssrv_start,
                                                             lte_module_driver_receive_at_ble_ssrv_start))
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_initial_ble_advstart;
                }
            }break;
            case cb_lte_module_initial_ble_advstart:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
                                                             lte_module_driver_send_at_ble_adv_start,
                                                             lte_module_driver_receive_at_ble_adv_start))
                {
                    b_err_cnt = 0;
                    st_process_contral.st_operate_result.b_err_code = ble_config_success;
                    b_step = cb_lte_module_initial_ble_none;
                }
            }break;
            default:
            {
            }break;
        }
    }while(b_step != cb_lte_module_initial_ble_none);

    if(pst_config_info && pst_config_info->pfunc_operate_result_notify)
    {
        pst_config_info->pfunc_operate_result_notify(&st_process_contral.st_operate_result);
    }

    return TRUE;
}
 extern uint8_t b_ble_outtime_connect_flag;

uint8_t lte_module_open_broadcast(lte_module_driver_config_info_general *pst_config_info)
{
    if(0 == pst_config_info)
    {
        return FALSE;
    }

    void *pv_user_data = (void *)pst_config_info;

    lte_module_driver_process_contral st_process_contral =
    {
        .st_operate_result.b_iot_module_type = iot_module_type_lte,
        .st_operate_result.b_iot_module_operation = lte_module_operate_ble_open_broadcast,
        .st_operate_result.b_err_code = ble_broadcast_open_fail,
    };
	if(1 == b_ble_outtime_connect_flag)
	{
		b_ble_outtime_connect_flag = 0;
		if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
													 lte_module_driver_send_at_ble_disconn,
													 lte_module_driver_receive_at_ble_disconn))
		{
		}
	}


    if(TRUE == lte_module_driver_at_cmd_transmit(3000,pv_user_data,0,
                                                 lte_module_driver_send_at_ble_adv_start,
                                                 lte_module_driver_receive_at_ble_adv_start))
    {
        st_process_contral.st_operate_result.b_err_code = ble_broadcast_open_success;
    }

    if(pst_config_info && pst_config_info->pfunc_operate_result_notify)
    {
        pst_config_info->pfunc_operate_result_notify(&st_process_contral.st_operate_result);
    }

    return TRUE;
}

uint8_t lte_module_ble_deinit(lte_module_driver_config_info_general *pst_config_info)
{
    if(0 == pst_config_info)
    {
        return FALSE;
    }

    void *pv_user_data = (void *)pst_config_info;

    lte_module_driver_process_contral st_process_contral =
    {
        .st_operate_result.b_iot_module_type = iot_module_type_lte,
        .st_operate_result.b_iot_module_operation = lte_module_operate_ble_close,
        .st_operate_result.b_err_code = ble_close_fail,
    };

    if(TRUE == lte_module_driver_at_cmd_transmit(3000,pv_user_data,0,
                                                 lte_module_driver_send_at_ble_deinit,
                                                 lte_module_driver_receive_at_ble_deinit))
    {
        st_process_contral.st_operate_result.b_err_code = ble_close_success;
    }

    if(pst_config_info && pst_config_info->pfunc_operate_result_notify)
    {
        pst_config_info->pfunc_operate_result_notify(&st_process_contral.st_operate_result);
    }

    return TRUE;
}

/************************************************************************************************************
*   ble receive notify
************************************************************************************************************/

/************************************************************************************************************
*   send ble user data
************************************************************************************************************/
/*ble publish messages*/
//AT+BLEGATTSNTFY=<conn_index>,<srv_index>,<char_index>,<length>
uint8_t lte_module_driver_send_at_ble_cmd(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if((NULL == pv_user_data) || (NULL == pst_lte_module_data_info))
	{
		return FALSE;
	}

	lte_module_driver_config_info_general *pst_config_info = (lte_module_driver_config_info_general *)pv_user_data;

    if(0 == pst_config_info->union_config_info.st_send_ble_user_data.st_data_info.w_data_len)
    {
        return FALSE;
    }

	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,
				    "AT+BLEGATTSNTFY=0,1,6,%d\r\n",
                    pst_config_info->union_config_info.st_send_ble_user_data.st_data_info.w_data_len))
	{
		return FALSE;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return TRUE;
}
uint8_t lte_module_driver_receive_at_ble_cmd(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if(NULL == pst_lte_module_data_info)
	{
		return 0;
	}

	if((strstr((char*)pst_lte_module_data_info->pb_data,">") != NULL))
	{
		return 1;
	}
    else if(strstr((char*)pst_lte_module_data_info->pb_data,"ERROR") != NULL)
    {
        yl_debug_printf("lte_module_driver_receive_at_ble_cmd ERROR\n");
        return 0;
    }

	return 2;
}
uint8_t lte_module_driver_send_at_ble_user_data(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if((NULL == pv_user_data) || (NULL == pst_lte_module_data_info))
	{
		return FALSE;
	}

	lte_module_driver_config_info_general *pst_config_info = (lte_module_driver_config_info_general *)pv_user_data;

    if(NULL == pst_config_info->union_config_info.st_send_ble_user_data.st_data_info.pb_data)
    {
        return FALSE;
    }

    for(uint16_t w_cnt_i = 0;w_cnt_i < pst_config_info->union_config_info.st_send_ble_user_data.st_data_info.w_data_len;w_cnt_i++)
    {
        pst_lte_module_data_info->pb_data[w_cnt_i] =\
            pst_config_info->union_config_info.st_send_ble_user_data.st_data_info.pb_data[w_cnt_i];
    }
	pst_lte_module_data_info->w_data_len = pst_config_info->union_config_info.st_send_ble_user_data.st_data_info.w_data_len;


	return TRUE;
}
uint8_t lte_module_driver_receive_at_ble_user_data(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if(NULL == pst_lte_module_data_info)
	{
		return FALSE;
	}

	if(strstr((char*)pst_lte_module_data_info->pb_data,"\r\nOK") != 0)
	{
		return 1;
	}
    else if(strstr((char*)pst_lte_module_data_info->pb_data,"ERROR") != NULL)
    {
        return 0;
    }

	return 2;
}
uint8_t lte_module_driver_send_ble_user_data(lte_module_driver_config_info_general *pst_config_info)
{
	if(NULL == pst_config_info)
	{
		return FALSE;
	}

    uint8_t b_result = FALSE;

	#define cb_lte_ble_send_cmd 				0
	#define cb_lte_ble_send_data				1
	#define cb_lte_ble_none			    		2
	uint8_t b_step = cb_lte_ble_send_cmd;

    lte_module_driver_process_contral st_process_contral =
    {
        .st_operate_result.b_iot_module_type = iot_module_type_lte,
        .st_operate_result.b_err_code = ble_send_user_data_fail,
        .st_operate_result.b_iot_module_operation = lte_module_operate_send_ble_user_data,
        .pst_config_info = pst_config_info,
    };

	do
	{
		switch(b_step)
		{
			case cb_lte_ble_send_cmd:
			{
				if(TRUE == lte_module_driver_at_cmd_transmit(1000,pst_config_info,0,
															 lte_module_driver_send_at_ble_cmd,
															 lte_module_driver_receive_at_ble_cmd))
				{
					b_step = cb_lte_ble_send_data;
				}
                else
                {
                    b_step = cb_lte_ble_none;
                }
			}break;
			case cb_lte_ble_send_data:
			{
                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pst_config_info,0,
                                                             lte_module_driver_send_at_ble_user_data,
                                                             lte_module_driver_receive_at_ble_user_data))
                {
                    st_process_contral.st_operate_result.b_err_code = ble_send_user_data_success;
                    b_step = cb_lte_ble_none;
                }
                else
                {
                    b_step = cb_lte_ble_none;
                }
			}break;
			case cb_lte_ble_none:
			{
                b_result = TRUE;
			}break;
			default:
			{
			}break;
		}
        vTaskDelay(0);
	}while(b_step != cb_lte_ble_none);

    if(pst_config_info && pst_config_info->pfunc_operate_result_notify)
    {
        pst_config_info->pfunc_operate_result_notify(&st_process_contral.st_operate_result);
    }
    else
    {
        lte_module_driver_reponse_info_general *pst_reponse_info = (lte_module_driver_reponse_info_general *)st_process_contral.st_operate_result.pv_data;

        if(pst_reponse_info->pst_data->pb_data)
        {
            vPortFree(pst_reponse_info->pst_data->pb_data);
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"lte_module_driver_send_ble_user_data_0_free%x\n",pst_reponse_info->pst_data->pb_data);
            #endif
        }

        if(pst_reponse_info->pst_data)
        {
            vPortFree(pst_reponse_info->pst_data);
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"lte_module_driver_send_ble_user_data_1_free%x\n",pst_reponse_info->pst_data);
            #endif
        }

        if(pst_reponse_info)
        {
            vPortFree(pst_reponse_info);
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"lte_module_driver_send_ble_user_data_2_free%x\n",pst_reponse_info);
            #endif
        }
    }

	return b_result;
}
