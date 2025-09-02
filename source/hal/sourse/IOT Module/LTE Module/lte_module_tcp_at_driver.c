#include "lte_module_tcp_at_driver.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "freertos_user_config.h"
#include "network_protocal_data_assembly.h"
#include "network_tbb_device_server_communicate.h"


static uint8_t b_lte_tcp_client_idx = 0;
static uint8_t f_receive_data_en = FALSE;

/************************************************************************************************************
*   tcp at driver state
************************************************************************************************************/
uint8_t lte_module_driver_tcp_at_driver_state(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
    if((NULL == pv_user_data) || (NULL == pst_lte_module_data_info))
	{
		return 0;
	}

    lte_module_driver_process_contral *pst_driver_contral_info = (lte_module_driver_process_contral *)pv_user_data;

    if(strstr((char *)pst_lte_module_data_info->pb_data,"WIFI"))
    {
        if(strstr((char *)pst_lte_module_data_info->pb_data,"CONNECTED"))
        {
            pst_driver_contral_info->st_operate_result.b_err_code = ap_connection_auto_connect_success;
        }
        else if(strstr((char *)pst_lte_module_data_info->pb_data,"DISCONNECT"))
        {
            pst_driver_contral_info->st_operate_result.b_err_code = ap_connection_closed;
        }
//        else if(strstr((char *)pst_lte_module_data_info->pb_data,"GOT IP"))
//        {
//            pst_driver_contral_info->st_operate_result.b_err_code = ap_connection_auto_connect_success;
//        }
        return 1;
    }
    else if((NULL != strstr((char *)pst_lte_module_data_info->pb_data,"CLOSED")) &&\
            (lte_module_operate_receive_tcp_user_data == pst_driver_contral_info->st_operate_result.b_iot_module_operation))
    {
        pst_driver_contral_info->st_operate_result.b_err_code = ap_connection_closed;
        return 0;
    }

	return 0;
}
/************************************************************************************************************
*   tcp receive notify
************************************************************************************************************/
uint16_t w_receive_data_len = 0;
uint8_t lte_module_driver_receive_check(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
    if((NULL == pv_user_data) || (NULL == pst_lte_module_data_info))
	{
		return 0;
	}

	uint32_t dw_data_temp_0 = 0;
    uint8_t *pb_data = NULL;

    pb_data = (uint8_t *)strstr((char*)pst_lte_module_data_info->pb_data,"+IPD");
    if(NULL != pb_data)
    {
        if(sscanf((char *)pb_data,
                  "+IPD,%d",
                  &dw_data_temp_0) != 0)
        {
            if(dw_data_temp_0 != 0)
            {
                w_receive_data_len = dw_data_temp_0;
                if(dw_data_temp_0 > 500)
                {
                    dw_data_temp_0 = 0;
                }
                return 1;
            }
        }
    }

	return 0;
}
uint8_t lte_module_driver_receive_tcp_notify(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
    if(TRUE == lte_module_driver_tcp_at_driver_state(pv_user_data,pst_lte_module_data_info))
    {
        return 1;
    }

    lte_module_driver_process_contral *pst_config_info = (lte_module_driver_process_contral *)pv_user_data;

    if(TRUE == lte_module_driver_receive_check(pv_user_data,pst_lte_module_data_info))
    {
        f_receive_data_en = TRUE;
        if(lte_module_operate_receive_tcp_user_data ==\
           pst_config_info->st_operate_result.b_iot_module_operation)
        {
            return 0;
        }
    }
    return 3;
}

/*tcp connect*/
//AT+CIPSTART=<"type">,<"remote host">,<remote port>[,<keep alive>][,<"local IP">]
uint8_t lte_module_driver_send_at_tcp_connect(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
    #define cb_max_ip_connect_cnt   3
	if((NULL == pv_user_data) || (NULL == pst_lte_module_data_info))
	{
		return FALSE;
	}

    lte_module_driver_process_contral *pst_process_contral = (lte_module_driver_process_contral *)pv_user_data;
    lte_module_driver_config_info_general *pst_config_info = pst_process_contral->pst_config_info;
    uint8_t *pb_domain_ip = NULL;
    uint16_t w_port = 0;

    pst_config_info->union_config_info.st_open_tcp_connection.b_channel_index = b_lte_tcp_client_idx;
    if((pst_config_info->union_config_info.st_open_tcp_connection.ba_ip[0] != '\0') &&\
       (cb_max_ip_connect_cnt >= net_connect_ip_cnt_get()))
    {
        pb_domain_ip = pst_config_info->union_config_info.st_open_tcp_connection.ba_ip;
        w_port = pst_config_info->union_config_info.st_open_tcp_connection.w_ip_port;
        if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+CIPSTART=\"TCP\",\"%d.%d.%d.%d\",%d\r\n",
                        pb_domain_ip[0],
                        pb_domain_ip[1],
                        pb_domain_ip[2],
                        pb_domain_ip[3],
                        w_port))
        {
            return FALSE;
        }
        net_connect_ip_cnt_set(TRUE);
    }
    else
    {
        pb_domain_ip = pst_config_info->union_config_info.st_open_tcp_connection.ba_domain;
        w_port = pst_config_info->union_config_info.st_open_tcp_connection.w_domain_port;
        if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",
                        pb_domain_ip,
                        w_port))
        {
            return FALSE;
        }
        net_connect_ip_cnt_set(FALSE);
    }

	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return TRUE;
}
uint8_t lte_module_driver_receive_at_tcp_connect(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if(NULL == pst_lte_module_data_info)
	{
		return FALSE;
	}

    if(NULL != strstr((char*)pst_lte_module_data_info->pb_data,"CONNECT"))
    {
        return 1;
    }
    else if(NULL != strstr((char*)pst_lte_module_data_info->pb_data,"\r\nOK"))
    {
        return 2;
    }
    return 0;
}
/*tcp close*/
//AT+CIPCLOSE=0\r\n
uint8_t lte_module_driver_send_at_tcp_close(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if((NULL == pv_user_data) || (NULL == pst_lte_module_data_info))
	{
		return FALSE;
	}

//    lte_module_driver_config_info_general *pst_config_info = (lte_module_driver_config_info_general *)pv_user_data;

	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+CIPCLOSE\r\n"))
	{
		return FALSE;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return TRUE;
}
uint8_t lte_module_driver_receive_at_tcp_close(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if(NULL == pst_lte_module_data_info)
	{
		return FALSE;
	}

	if((strstr((char*)pst_lte_module_data_info->pb_data,"\r\nOK") != 0) ||\
       (strstr((char*)pst_lte_module_data_info->pb_data,"\r\nERROR") != 0))
	{
        return 1;
	}
//    else if(strstr((char*)pst_lte_module_data_info->pb_data,"CLOSED") != 0)
//    {
//        return 1;
//    }

	return 2;
}
/*tcp state*/
//AT+CIPSTATE?\r\n
uint8_t lte_module_driver_send_at_tcp_state(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if((NULL == pv_user_data) || (NULL == pst_lte_module_data_info))
	{
		return FALSE;
	}

    lte_module_driver_process_contral *pst_process_contral = (lte_module_driver_process_contral *)pv_user_data;
    lte_module_driver_config_info_general *pst_config_info = pst_process_contral->pst_config_info;

    pst_config_info->union_config_info.st_open_tcp_connection.b_channel_index = b_lte_tcp_client_idx;

	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+CIPSTATE?\r\n"))
	{
		return FALSE;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return TRUE;
}

uint8_t lte_module_driver_receive_at_tcp_state(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
    if((NULL == pv_user_data) || (NULL == pst_lte_module_data_info))
	{
		return 0;
	}

	uint32_t dw_data_temp_0 = 0;

	if(sscanf(strstr((char*)pst_lte_module_data_info->pb_data,"+CIPSTATE:"),"+CIPSTATE: %d",&dw_data_temp_0) != 0)
	{
        return 1;
	}

	return 2;
}

uint8_t lte_module_tcp_channel_connect(lte_module_driver_config_info_general *pst_config_info,
                                         lte_module_driver_process_contral *pst_process_contral)
{
	#define cb_lte_connect_tcp_channel_connect_connect						    0
	#define cb_lte_connect_tcp_channel_connect_state							1
	#define cb_lte_connect_tcp_channel_connect_none								2
//	#define cb_lte_connect_tcp_channel_connect_close						    3
	uint8_t b_step = cb_lte_connect_tcp_channel_connect_connect;
    uint8_t b_cnt = 0;

    uint8_t b_result = FALSE;

    if(NULL == pst_process_contral)
    {
        return FALSE;
    }

    if(TRUE == pst_process_contral->operate_contral.b_process_reset)
    {
        b_step = cb_lte_connect_tcp_channel_connect_connect;
    }

    do{
        switch(b_step)
        {
            case cb_lte_connect_tcp_channel_connect_connect:
            {
                uint8_t b_result_ = 3;

                b_result_ = lte_module_driver_at_cmd_transmit(60000,pst_process_contral,lte_module_driver_receive_tcp_notify,
                                                             lte_module_driver_send_at_tcp_connect,
                                                             lte_module_driver_receive_at_tcp_connect);
                if(1 == b_result_)
                {
                    b_step = cb_lte_connect_tcp_channel_connect_state;
                }
                else if(0 == b_result_)
                {
                    b_result = FALSE;
                    pst_process_contral->st_operate_result.b_err_code = tcp_open_connection_fail;
                    pst_process_contral->operate_contral.b_process_reset = TRUE;
                    b_step = cb_lte_connect_tcp_channel_connect_none;
                }
            }break;
            case cb_lte_connect_tcp_channel_connect_state:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(3000,pst_process_contral,lte_module_driver_receive_tcp_notify,
                                                             lte_module_driver_send_at_tcp_state,
                                                             lte_module_driver_receive_at_tcp_state))
                {
                    b_result = TRUE;
                    b_step = cb_lte_connect_tcp_channel_connect_none;
                }
                else
                {
                    b_result = FALSE;
                    b_cnt++;
                    if(b_cnt > 20)
                    {
                        b_step = cb_lte_connect_tcp_channel_connect_none;
                    }
                    else
                    {
                        b_step = cb_lte_connect_tcp_channel_connect_state;
                    }
                }
            }break;
            case cb_lte_connect_tcp_channel_connect_none:
            {
            }break;
            default:
            {
            }break;
        }
        vTaskDelay(10);
    }while(b_step != cb_lte_connect_tcp_channel_connect_none);

    return b_result;
}
uint8_t lte_module_driver_open_tcp_connection(lte_module_driver_config_info_general *pst_config_info)
{
    if(NULL == pst_config_info)
    {
        return FALSE;
    }

//    #define cb_lte_connect_tcp_client_id_allocate						0
    #define cb_lte_connect_tcp_connect								    1
    #define cb_lte_connect_tcp_response                                 2
    #define cb_lte_connect_tcp_none									    3
    uint8_t b_step = cb_lte_connect_tcp_connect;
    uint8_t b_result = FALSE;

    lte_module_driver_process_contral st_process_contral =
    {
        .st_operate_result.b_iot_module_type = iot_module_type_lte,
        .st_operate_result.b_iot_module_operation = lte_module_operate_open_tcp_connection,
        .st_operate_result.b_err_code = lte_module_err_none,
        .pst_config_info = pst_config_info,
    };

    do
    {
        switch(b_step)
        {
            case cb_lte_connect_tcp_connect:
            {
                if(TRUE == lte_module_tcp_channel_connect(pst_config_info,&st_process_contral))
                {
                    st_process_contral.st_operate_result.b_err_code = tcp_open_connection_success;
                    b_step = cb_lte_connect_tcp_response;
                }
                else
                {
                    st_process_contral.st_operate_result.b_err_code = tcp_open_connection_fail;
                    b_step = cb_lte_connect_tcp_response;
                }
            }break;
            case cb_lte_connect_tcp_response:
            {
                if(pst_config_info && pst_config_info->pfunc_operate_result_notify)
                {
                    b_result = TRUE;
                    pst_config_info->pfunc_operate_result_notify(&st_process_contral.st_operate_result);
                }
                b_step = cb_lte_connect_tcp_none;
            }break;
            case cb_lte_connect_tcp_none:
            {
            }break;
            default:
            {
            }break;
        }
        vTaskDelay(10);
    }while(b_step != cb_lte_connect_tcp_none);

    return b_result;
}
uint8_t lte_module_driver_close_tcp_connection(lte_module_driver_config_info_general *pst_config_info)
{
    #define cb_lte_connect_tcp_channel_disconnect_disconnect						    0
	#define cb_lte_connect_tcp_channel_disconnect_none								    1
	uint8_t b_step = cb_lte_connect_tcp_channel_disconnect_disconnect;

    uint8_t b_result = FALSE;

    lte_module_driver_process_contral st_process_contral =
    {
        .st_operate_result.b_iot_module_type = iot_module_type_lte,
        .st_operate_result.b_iot_module_operation = lte_module_operate_close_tcp_connection,
        .st_operate_result.b_err_code = lte_module_err_none,
    };

    do
    {
        switch(b_step)
        {
            case cb_lte_connect_tcp_channel_disconnect_disconnect:
            {
                uint8_t b_disconnect_result = 3;
                b_disconnect_result = lte_module_driver_at_cmd_transmit(10000,&st_process_contral,lte_module_driver_receive_tcp_notify,
                                                                        lte_module_driver_send_at_tcp_close,
                                                                        lte_module_driver_receive_at_tcp_close);
                if(1 == b_disconnect_result)
                {
                    st_process_contral.st_operate_result.b_err_code = tcp_close_connection_success;
                    b_step = cb_lte_connect_tcp_channel_disconnect_none;
                }
                else if(0 == b_disconnect_result)
                {
                    st_process_contral.st_operate_result.b_err_code = tcp_close_connection_fail;
                    b_step = cb_lte_connect_tcp_channel_disconnect_none;
                }
                b_result = TRUE;
            }break;
            case cb_lte_connect_tcp_channel_disconnect_none:
            {
            }break;
            default:
            {
            }break;
        }
        vTaskDelay(10);
    }while(b_step != cb_lte_connect_tcp_channel_disconnect_none);

    if(pst_config_info && pst_config_info->pfunc_operate_result_notify)
    {
        pst_config_info->pfunc_operate_result_notify(&st_process_contral.st_operate_result);
    }

    return b_result;
}
uint8_t lte_module_driver_reset_tcp_connection(lte_module_driver_config_info_general *pst_config_info)
{
    #define cb_lte_connect_tcp_channel_reset_disconnect						            0
    #define cb_lte_connect_tcp_channel_reset_connect						            1
	#define cb_lte_connect_tcp_channel_reset_none							    	    2
	uint8_t b_step = cb_lte_connect_tcp_channel_reset_disconnect;

    uint8_t b_result = FALSE;

    lte_module_driver_process_contral st_process_contral =
    {
        .st_operate_result.b_iot_module_type = iot_module_type_lte,
        .st_operate_result.b_iot_module_operation = lte_module_operate_reset_tcp_connection,
        .st_operate_result.b_err_code = lte_module_err_none,
    };

    do
    {
        switch(b_step)
        {
            case cb_lte_connect_tcp_channel_reset_disconnect:
            {
                uint8_t b_disconnect_result = 3;
                b_disconnect_result = lte_module_driver_at_cmd_transmit(10000,&st_process_contral,lte_module_driver_receive_tcp_notify,
                                                                        lte_module_driver_send_at_tcp_close,
                                                                        lte_module_driver_receive_at_tcp_close);
                if(1 == b_disconnect_result)
                {
                    b_step = cb_lte_connect_tcp_channel_reset_connect;
                }
                else if(0 == b_disconnect_result)
                {
                    b_result = TRUE;
                    st_process_contral.st_operate_result.b_err_code = tcp_reset_connection_fail;
                    b_step = cb_lte_connect_tcp_channel_reset_none;
                }
            }break;
            case cb_lte_connect_tcp_channel_reset_connect:
            {
                uint8_t b_connect_result = 3;
                b_connect_result = lte_module_driver_at_cmd_transmit(15000,&st_process_contral,lte_module_driver_receive_tcp_notify,
                                                                     lte_module_driver_send_at_tcp_connect,
                                                                     lte_module_driver_receive_at_tcp_connect);
                if(1 == b_connect_result)
                {
                    st_process_contral.st_operate_result.b_err_code = tcp_reset_connection_success;
                    b_step = cb_lte_connect_tcp_channel_reset_none;
                }
                else if(0 == b_connect_result)
                {
                    b_result = TRUE;
                    st_process_contral.st_operate_result.b_err_code = tcp_reset_connection_fail;
                    b_step = cb_lte_connect_tcp_channel_reset_none;
                }
            }break;
            case cb_lte_connect_tcp_channel_reset_none:
            {
            }break;
            default:
            {
            }break;
        }
        vTaskDelay(10);
    }while(b_step != cb_lte_connect_tcp_channel_reset_none);

    if(pst_config_info && pst_config_info->pfunc_operate_result_notify)
    {
        pst_config_info->pfunc_operate_result_notify(&st_process_contral.st_operate_result);
    }

    return b_result;
}
/************************************************************************************************************
*   send tcp user data
************************************************************************************************************/
/*tcp publish messages*/
uint8_t lte_module_driver_send_at_tcp_cmd(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if((NULL == pv_user_data) || (NULL == pst_lte_module_data_info))
	{
		return FALSE;
	}

    lte_module_driver_process_contral *pst_process_contral = (lte_module_driver_process_contral *)pv_user_data;
	lte_module_driver_config_info_general *pst_config_info = pst_process_contral->pst_config_info;

    if(0 == pst_config_info->union_config_info.st_send_tcp_user_data.st_data_info.w_data_len)
    {
        return FALSE;
    }

	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,
				    "AT+CIPSEND=%d\r\n",
                    pst_config_info->union_config_info.st_send_tcp_user_data.st_data_info.w_data_len))
	{
		return FALSE;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return TRUE;
}
uint8_t lte_module_driver_receive_at_tcp_cmd(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if(NULL == pst_lte_module_data_info)
	{
		return 0;
	}

	if((strstr((char*)pst_lte_module_data_info->pb_data,"\r\n>") != NULL))
	{
		return 1;
	}
    else if((strstr((char*)pst_lte_module_data_info->pb_data,"ERROR") != NULL) ||\
            (strstr((char*)pst_lte_module_data_info->pb_data,"CLOSED") != NULL))
    {
        return 0;
    }

	return 2;
}
uint8_t lte_module_driver_send_at_tcp_user_data(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if((NULL == pv_user_data) || (NULL == pst_lte_module_data_info))
	{
		return FALSE;
	}

    lte_module_driver_process_contral *pst_process_contral = (lte_module_driver_process_contral *)pv_user_data;
	lte_module_driver_config_info_general *pst_config_info = pst_process_contral->pst_config_info;

    if(NULL == pst_config_info->union_config_info.st_send_tcp_user_data.st_data_info.pb_data)
    {
        return FALSE;
    }

    for(uint16_t w_cnt_i = 0;w_cnt_i < pst_config_info->union_config_info.st_send_tcp_user_data.st_data_info.w_data_len;w_cnt_i++)
    {
        pst_lte_module_data_info->pb_data[w_cnt_i] =\
            pst_config_info->union_config_info.st_send_tcp_user_data.st_data_info.pb_data[w_cnt_i];
    }
	pst_lte_module_data_info->w_data_len = pst_config_info->union_config_info.st_send_tcp_user_data.st_data_info.w_data_len;

//    vPortFree(pst_config_info->union_config_info.st_send_tcp_user_data.st_data_info.pb_data);

	return TRUE;
}
uint8_t lte_module_driver_receive_at_tcp_user_data(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if(NULL == pst_lte_module_data_info)
	{
		return FALSE;
	}

	if(strstr((char*)pst_lte_module_data_info->pb_data,"SEND OK") != 0)
	{
		return 1;
	}
    else if((strstr((char*)pst_lte_module_data_info->pb_data,"ERROR") != NULL) ||\
            (strstr((char*)pst_lte_module_data_info->pb_data,"CLOSED") != NULL) ||\
            (strstr((char*)pst_lte_module_data_info->pb_data,"SEND FAIL") != NULL))
    {
        return 0;
    }

	return 2;
}
uint8_t lte_module_driver_send_tcp_user_data(lte_module_driver_config_info_general *pst_config_info)
{
	if(NULL == pst_config_info)
	{
		return FALSE;
	}

    uint8_t b_result = FALSE;

	#define cb_lte_tcp_send_cmd 				0
	#define cb_lte_tcp_send_data				1
	#define cb_lte_tcp_none			    		2
	uint8_t b_step = cb_lte_tcp_send_cmd;

    lte_module_driver_process_contral st_process_contral =
    {
        .st_operate_result.b_iot_module_type = iot_module_type_lte,
        .st_operate_result.b_err_code = lte_module_err_none,
        .st_operate_result.b_iot_module_operation = lte_module_operate_send_tcp_user_data,
        .pst_config_info = pst_config_info,
    };

	do
	{
		switch(b_step)
		{
			case cb_lte_tcp_send_cmd:
			{
				if(TRUE == lte_module_driver_at_cmd_transmit(3000,&st_process_contral,lte_module_driver_receive_tcp_notify,
															 lte_module_driver_send_at_tcp_cmd,
															 lte_module_driver_receive_at_tcp_cmd))
				{
					b_step = cb_lte_tcp_send_data;
				}
                else
                {
                    st_process_contral.st_operate_result.b_err_code = tcp_send_user_data_fail;
                    b_step = cb_lte_tcp_none;
                }
			}break;
			case cb_lte_tcp_send_data:
			{
                if(TRUE == lte_module_driver_at_cmd_transmit(10000,&st_process_contral,lte_module_driver_receive_tcp_notify,
                                                             lte_module_driver_send_at_tcp_user_data,
                                                             lte_module_driver_receive_at_tcp_user_data))
                {
                    st_process_contral.st_operate_result.b_err_code = tcp_send_user_data_success;
                    b_step = cb_lte_tcp_none;
                }
                else
                {
                    st_process_contral.st_operate_result.b_err_code = tcp_send_user_data_fail;
                    b_step = cb_lte_tcp_none;
                }
			}break;
			case cb_lte_tcp_none:
			{
                b_result = TRUE;
			}break;
			default:
			{
			}break;
		}
        vTaskDelay(10);
	}while(b_step != cb_lte_tcp_none);

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
            DebugPrintf(TRUE,"lte_module_driver_send_tcp_user_data_0_free%x\n",pst_reponse_info->pst_data->pb_data);
            #endif
        }

        if(pst_reponse_info->pst_data)
        {
            vPortFree(pst_reponse_info->pst_data);
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"lte_module_driver_send_tcp_user_data_1_free%x\n",pst_reponse_info->pst_data);
            #endif
        }

        if(pst_reponse_info)
        {
            vPortFree(pst_reponse_info);
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"lte_module_driver_send_tcp_user_data_2_free%x\n",pst_reponse_info);
            #endif
        }
    }

	return b_result;
}
uint8_t lte_module_driver_send_at_receive_tcp_message(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if((NULL == pv_user_data) || (NULL == pst_lte_module_data_info))
	{
		return FALSE;
	}

    static uint8_t f_timer_init = TRUE;

    if(f_receive_data_en)
    {
        f_receive_data_en = FALSE;
        f_timer_init = TRUE;
        uint16_t w_max_buff_len = 0;
        w_max_buff_len =\
//            (w_receive_data_len >= cb_max_network_data_len)?w_receive_data_len:cb_max_network_data_len;
            (w_receive_data_len >= cb_max_network_data_len - 2)?(cb_max_network_data_len - 2):w_receive_data_len;
        if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,
                        "AT+CIPRECVDATA=%d\r\n",
                        w_max_buff_len))
        {
            return FALSE;
        }
        pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);
    }
    else
    {
        static TickType_t x_start_time = 0;
        TickType_t x_curent_time = 0;
        if(f_timer_init)
        {
            f_timer_init = FALSE;
            x_start_time = xTaskGetTickCount();
        }
        x_curent_time = xTaskGetTickCount();
        if((x_curent_time - x_start_time) >= pdMS_TO_TICKS_USER(15000))
        {
            f_timer_init = TRUE;
            f_receive_data_en = TRUE;
            w_receive_data_len = cb_max_network_data_len - 2;
        }

        pst_lte_module_data_info->w_data_len = 0;
        return FALSE;
    }
	return TRUE;
}
uint8_t lte_module_driver_receive_at_receive_tcp_message(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if((NULL == pst_lte_module_data_info) || (NULL == pv_user_data))
	{
        return 0;
	}

    static uint8_t b_tcp_receive_new_message = TRUE;
    static uint16_t w_tcp_data_index = 0;
    static uint32_t dw_data_len = 0;
    uint32_t dw_receive_data_len = 0;
    uint8_t *pb_data = 0;


    lte_module_driver_process_contral *pst_process_contral_info = (lte_module_driver_process_contral *)pv_user_data;
    genaral_communicate_data_info *pst_data_info =\
        (genaral_communicate_data_info *)pst_process_contral_info->pst_config_info->union_config_info.st_receive_tcp_user_data.pst_data_info;

    if(TRUE == b_tcp_receive_new_message)
    {
        if(0 != strstr((char*)pst_lte_module_data_info->pb_data,"CLOSED"))
        {
            pst_process_contral_info->st_operate_result.b_err_code = tcp_connection_closed;
            goto RESPONSE;
        }
        if(0 == strstr((char*)pst_lte_module_data_info->pb_data,"+CIPRECVDATA:"))
        {
            pst_process_contral_info->st_operate_result.b_err_code = tcp_receive_user_data_fail;
            goto CONTINUE;
        }

        b_tcp_receive_new_message = FALSE;


        if(0 == sscanf(strstr((char *)pst_lte_module_data_info->pb_data,"+CIPRECVDATA:"),
                       "+CIPRECVDATA:%d,",
                       &dw_data_len))
        {
            pst_process_contral_info->st_operate_result.b_err_code = tcp_receive_user_data_fail;
            goto RESPONSE;
        }
        if(0 == dw_data_len)
        {
            b_tcp_receive_new_message = TRUE;
            goto RESPONSE;
        }
//        else if(dw_data_len > cb_max_network_data_len)
//        {
//            b_tcp_receive_new_message = TRUE;
//            goto RESPONSE;
//        }
        pb_data = &pst_lte_module_data_info->
                    pb_data[15 +\
                            ((dw_data_len/1000)>0?1:0) +\
                            ((dw_data_len/100)>0?1:0) +\
                            ((dw_data_len/10)>0?1:0)];
        dw_receive_data_len = pst_lte_module_data_info->w_data_len - 15 -\
                                (((dw_data_len/1000)>0?1:0) +\
                                 ((dw_data_len/100)>0?1:0) +\
                                 ((dw_data_len/10)>0?1:0));
        if(dw_receive_data_len > cb_max_network_data_len)
        {
            b_tcp_receive_new_message = TRUE;
            goto RESPONSE;
        }
        else
        {
            pst_data_info->w_data_len = dw_data_len;
            memcpy(pst_data_info->pb_data,
                   pb_data,
                   dw_receive_data_len);
        }
    }
    else
    {
        dw_receive_data_len = pst_lte_module_data_info->w_data_len;
        if((w_tcp_data_index + dw_receive_data_len) < cb_max_network_data_len)
        {
            memcpy(&pst_data_info->pb_data[w_tcp_data_index],
                   pst_lte_module_data_info->pb_data,
                   dw_receive_data_len);
        }
        else
        {
            if((cb_max_network_data_len - w_tcp_data_index - 1) < cb_max_network_data_len)
            {
                memcpy(&pst_data_info->pb_data[w_tcp_data_index],
                       pst_lte_module_data_info->pb_data,
                       (cb_max_network_data_len - w_tcp_data_index - 1));
            }
            dw_data_len = 0;
        }
    }

    pst_process_contral_info->st_operate_result.b_err_code = tcp_receive_user_data_success;
    w_tcp_data_index += dw_receive_data_len;
    if(w_tcp_data_index >= dw_data_len)
    {
        b_tcp_receive_new_message = TRUE;
        w_tcp_data_index = 0;
        dw_data_len = 0;
        goto RESPONSE;
    }
    else
    {
        goto CONTINUE;
    }

    CONTINUE:
    return 2;

    RESPONSE:
	return 1;
}
uint8_t lte_module_driver_receive_tcp_user_data(lte_module_driver_config_info_general *pst_config_info)
{
    if(NULL == pst_config_info)
	{
		return FALSE;
	}

    lte_module_driver_process_contral st_process_contral =
    {
        .st_operate_result.b_iot_module_type = iot_module_type_lte,
        .st_operate_result.b_err_code = lte_module_err_none,
        .st_operate_result.b_iot_module_operation = lte_module_operate_receive_tcp_user_data,
        .pst_config_info = pst_config_info,
    };

    lte_module_driver_at_cmd_transmit(100,&st_process_contral,lte_module_driver_receive_tcp_notify,
                                      0,
                                      0);
    lte_module_driver_at_cmd_transmit(1000,&st_process_contral,lte_module_driver_receive_tcp_notify,
                                      lte_module_driver_send_at_receive_tcp_message,
                                      lte_module_driver_receive_at_receive_tcp_message);

    if(pst_config_info && pst_config_info->pfunc_operate_result_notify)
    {
        pst_config_info->pfunc_operate_result_notify(&st_process_contral.st_operate_result);
    }
    return TRUE;
}
