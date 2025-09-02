#include "lte_module_stander_at_driver.h"
#include "lte_module.h"
#include "lte_4g_module.h"
#include "printf_custom.h"
#include "msg_queue_operate.h"
#include "lte_module_data_queue.h"
#include "net_device_data_access.h"
#include "application_general_func.h"
#include "freertos_user_config.h"
#include "portmacro.h"
#include "FreeRTOS.h"
#include "task.h"
#include <string.h>
#include <stdlib.h>
#include "network_local_device_secquence_operate.h"
#include "network_local_config.h"
#include "Eeprom.h"
#include "lte_module_com_interface_init.h"
#include "net_device_id_kinergy_ii.h"


//ready
uint8_t lte_module_driver_receive_at_rdy(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if((pst_lte_module_data_info->w_data_len < 10) &&\
       (strstr((char*)pst_lte_module_data_info->pb_data,"ready") != 0))
    {
        return 1;
    }

	return 2;
}
//AT
uint8_t lte_module_driver_send_at_test(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT\r\n"))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_module_driver_receive_at_test(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if((pst_lte_module_data_info->w_data_len <= 10) &&\
       (strstr((char*)pst_lte_module_data_info->pb_data,"OK") != 0))
    {
        return 1;
    }

	return 2;
}
//AT+RST
uint8_t lte_module_driver_send_at_rst(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+RST\r\n"))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_module_driver_receive_at_rst(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if((pst_lte_module_data_info->w_data_len < 20) &&\
       (strstr((char*)pst_lte_module_data_info->pb_data,"OK") != 0))
    {
        return 1;
    }

	return 2;
}
//ATE
static uint8_t b_lte_ATEfag = 0;
uint8_t lte_module_driver_send_at_ate_mode(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"ATE%d\r\n",b_lte_ATEfag))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_module_driver_receive_at_ate_mode(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if((pst_lte_module_data_info->w_data_len < 20) &&\
       (strstr((char*)pst_lte_module_data_info->pb_data,"OK") != 0))
    {
        return 1;
    }

	return 2;
}
//AT+CIPTCPOPT=[<so_linger>],[<tcp_nodelay>],[<so_sndtimeo>]
uint8_t lte_module_driver_send_at_tcp_port_set(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{

	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,
                    "AT+CIPTCPOPT=-1,0,5000\r\n"))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_module_driver_receive_at_tcp_port_set(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if((pst_lte_module_data_info->w_data_len < 8) &&\
       (strstr((char*)pst_lte_module_data_info->pb_data,"OK") != 0))
    {
        return 1;
    }

	return 2;
}

// AT+UART_DEF=921600,8,1,0,0
uint8_t lte_module_driver_send_at_uart(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,
                    "AT+UART_DEF=%d,%d,%d,%d,%d\r\n",
                    921600,
                    8,
                    1,
                    0,
                    0))
    {
        return 0;
    }
    pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);
    return 1;
}

uint8_t lte_module_driver_receive_at_uart(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if((pst_lte_module_data_info->w_data_len <= 20) &&\
       (strstr((char*)pst_lte_module_data_info->pb_data,"OK") != 0))
    {
        return 1;
    }

    return 2;
}

void set_usart_lte_baudrate_115200();
void set_usart_lte_baudrate_921600();
uint8_t lte_module_initial(lte_module_driver_config_info_general *pst_config_info)
{
    if(0 == pst_config_info)
    {
        return FALSE;
    }

    #define cb_lte_module_initial_wait_rdy                  0
    #define cb_lte_module_initial_cmd_at_test               1
    #define cb_lte_module_initial_cmd_rst                   2
    #define cb_lte_module_initial_cmd_uart                  3
    #define cb_lte_module_initial_cmd_ate_disable           4
    #define cb_lte_module_initial_cmd_tcp_port_set          5
    #define cb_lte_module_initial_cmd_none                  6

    uint8_t b_step = cb_lte_module_initial_wait_rdy;
    uint8_t b_step_last = cb_lte_module_initial_wait_rdy;
    uint8_t b_err_cnt = 0;

    void *pv_user_data = (void *)pst_config_info;

    lte_module_driver_process_contral st_process_contral =
    {
        .st_operate_result.b_iot_module_type = iot_module_type_lte,
        .st_operate_result.b_iot_module_operation = lte_module_operate_initial,
        .st_operate_result.b_err_code = lte_module_initial_fail,
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
            case cb_lte_module_initial_wait_rdy:
            {
                lte_module_driver_at_cmd_transmit(10000,pv_user_data,0,
                                                  0,
                                                  lte_module_driver_receive_at_rdy);
                if(cb_lte_module_initial_cmd_rst == b_step_last)
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_initial_cmd_ate_disable;
                }
                else
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_initial_cmd_at_test;
                }
            }break;
            case cb_lte_module_initial_cmd_at_test:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
                                                             lte_module_driver_send_at_test,
                                                             lte_module_driver_receive_at_test))
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_initial_cmd_rst;
                }

                if( b_err_cnt >= 2)
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_initial_cmd_uart;
                    set_usart_lte_baudrate_115200();
                }

            }break;
            case cb_lte_module_initial_cmd_rst:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
                                                             lte_module_driver_send_at_rst,
                                                             lte_module_driver_receive_at_rst))
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_initial_wait_rdy;
                    b_step_last = cb_lte_module_initial_cmd_rst;
                }
            }break;
            case cb_lte_module_initial_cmd_uart:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
                                                             lte_module_driver_send_at_uart,
                                                             lte_module_driver_receive_at_uart))
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_initial_cmd_rst;
                    set_usart_lte_baudrate_921600();
                }

                if( b_err_cnt >= 2)
                {
                    set_usart_lte_baudrate_921600();
                }
            }break;
            case cb_lte_module_initial_cmd_ate_disable:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
                                                             lte_module_driver_send_at_ate_mode,
                                                             lte_module_driver_receive_at_ate_mode))
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_initial_cmd_tcp_port_set;
                }
            }break;
            case cb_lte_module_initial_cmd_tcp_port_set:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
                                                             lte_module_driver_send_at_tcp_port_set,
                                                             lte_module_driver_receive_at_tcp_port_set))
                {
                    b_err_cnt = 0;
                    st_process_contral.st_operate_result.b_err_code = lte_module_initial_success;
                    b_step = cb_lte_module_initial_cmd_none;
                }
            }break;
            default:
            {
            }break;
        }
    }while(b_step != cb_lte_module_initial_cmd_none);

    if(pst_config_info && pst_config_info->pfunc_operate_result_notify)
    {
        pst_config_info->pfunc_operate_result_notify(&st_process_contral.st_operate_result);
    }

    return TRUE;
}
/***************************************************************************
//Wifi Config
***************************************************************************/
////AT+BLEINIT=0
//uint8_t lte_module_driver_send_at_ble_deinit(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
//{
//	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+BLEINIT=0\r\n"))
//	{
//		return 0;
//	}
//	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);
//
//	return 1;
//}
//uint8_t lte_module_driver_receive_at_ble_deinit(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
//{
//    if((pst_lte_module_data_info->w_data_len < 8) &&\
//       (strstr((char*)pst_lte_module_data_info->pb_data,"OK") != 0))
//    {
//        return 1;
//    }
//
//	return 2;
//}
//AT+CWMODE
uint8_t lte_module_driver_send_at_wifi_mode(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+CWMODE=1\r\n"))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_module_driver_receive_at_wifi_mode(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if((pst_lte_module_data_info->w_data_len < 8) &&\
       (strstr((char*)pst_lte_module_data_info->pb_data,"OK") != 0))
    {
        return 1;
    }

	return 2;
}
//AT+CWSTATE?
uint8_t lte_module_driver_send_at_state(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+CWSTATE?\r\n"))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_module_driver_receive_at_state(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    uint32_t dw_connect_state = 0;
    uint8_t ba_ssid_temp[30] = {0,};

    lte_module_driver_config_info_general *pst_config_info = (lte_module_driver_config_info_general *)pv_user_data;

    if((NULL == pst_config_info->union_config_info.st_open_ap_connection.pb_ssid) ||\
       (NULL == pst_config_info->union_config_info.st_open_ap_connection.pb_pass))
    {
        return 0;
    }

    if(sscanf(strstr((char*)pst_lte_module_data_info->pb_data,"+CWSTATE"),
              "+CWSTATE: %d,%s",
              &dw_connect_state,
              ba_ssid_temp))
    {
        if((0 == strlen((char const*)pst_config_info->union_config_info.st_open_ap_connection.pb_ssid) ||\
           (0 == memcmp((char *)pst_config_info->union_config_info.st_open_ap_connection.pb_ssid,
                        (char *)ba_ssid_temp,
                        strlen((char *)pst_config_info->union_config_info.st_open_ap_connection.pb_ssid)))))
        {
            if((0 == dw_connect_state) || (4 == dw_connect_state))
            {
                return 0;
            }
            else if(2 == dw_connect_state)
            {
                return 1;
            }
            else
            {
                return 2;
            }
        }
        else
        {
            return 0;
        }
    }
    else if(strstr((char*)pst_lte_module_data_info->pb_data,"GOT IP") != 0)
    {
        return 1;
    }
	return 2;
}
uint8_t lte_module_driver_receive_at_state_for_get_ap_info(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    uint32_t dw_connect_state = 0;
    uint8_t ba_ssid_temp[30] = {0,};

    if(sscanf(strstr((char*)pst_lte_module_data_info->pb_data,"+CWSTATE"),
              "+CWSTATE: %d,%s",
              &dw_connect_state,
              ba_ssid_temp))
    {
        if((0 == dw_connect_state) || (4 == dw_connect_state))
        {
            return 0;
        }
        else if(2 == dw_connect_state)
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }
    else if(strstr((char*)pst_lte_module_data_info->pb_data,"GOT IP") != 0)
    {
        return 1;
    }
	return 2;
}
//AT+CWJAP
uint8_t lte_module_driver_send_at_conncet_ap(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    uint8_t b_null = 0;
    lte_module_driver_config_info_general *pst_config_info = (lte_module_driver_config_info_general *)pv_user_data;

    if((NULL == pst_config_info->union_config_info.st_open_ap_connection.pb_ssid) ||\
       (NULL == pst_config_info->union_config_info.st_open_ap_connection.pb_pass) ||\
       (NULL == pst_config_info->union_config_info.st_open_ap_connection.pb_pci_en) ||\
       (NULL == pst_config_info->union_config_info.st_open_ap_connection.pb_reconn_interval) ||\
       (NULL == pst_config_info->union_config_info.st_open_ap_connection.pb_listen_interval) ||\
       (NULL == pst_config_info->union_config_info.st_open_ap_connection.pb_scan_mode) ||\
       (NULL == pst_config_info->union_config_info.st_open_ap_connection.pb_jap_timeout) ||\
       (NULL == pst_config_info->union_config_info.st_open_ap_connection.pb_pmf))
    {
        return 0;
    }
    if(0 == pst_config_info->union_config_info.st_open_ap_connection.pb_bssid)
    {
        pst_config_info->union_config_info.st_open_ap_connection.pb_bssid = &b_null;
    }
    if((0 == strlen((char const*)pst_config_info->union_config_info.st_open_ap_connection.pb_ssid)) ||\
       (TRUE == *pst_config_info->union_config_info.st_open_ap_connection.pf_wps_first_connect))
    {
        if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+CWJAP=,,%s,%d,%d,%d,%d,%d,%d\r\n",
                        pst_config_info->union_config_info.st_open_ap_connection.pb_bssid,
                        *pst_config_info->union_config_info.st_open_ap_connection.pb_pci_en,
                        *pst_config_info->union_config_info.st_open_ap_connection.pb_reconn_interval,
                        *pst_config_info->union_config_info.st_open_ap_connection.pb_listen_interval,
                        *pst_config_info->union_config_info.st_open_ap_connection.pb_scan_mode,
                        *pst_config_info->union_config_info.st_open_ap_connection.pb_jap_timeout,
                        *pst_config_info->union_config_info.st_open_ap_connection.pb_pmf))
        {
            return 0;
        }
        pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);
    }
    else if(NULL != strstr((char *)pst_config_info->union_config_info.st_open_ap_connection.pb_ssid,
                           "WCM_"))
    {
        pst_lte_module_data_info->w_data_len = 0;
        return 0;
    }
    else
    {
            uint8_t ap_ssid[cb_ap_ssid_len] = {0};
            uint8_t ap_pass[cb_ap_pass_len] = {0};
            memcpy(ap_ssid,pst_config_info->union_config_info.st_open_ap_connection.pb_ssid,cb_ap_ssid_len);
            memcpy(ap_pass,pst_config_info->union_config_info.st_open_ap_connection.pb_pass,cb_ap_pass_len);
            //遍历识别到 0x2C 在前面加上 0x5C
            for(uint8_t b_cnt_i = 0;b_cnt_i < cb_ap_ssid_len;b_cnt_i++)
            {
                if(0x2C == ap_ssid[b_cnt_i])
                {
                    for(uint8_t b_cnt_j = cb_ap_ssid_len - 1;b_cnt_j > b_cnt_i;b_cnt_j--)
                    {
                        ap_ssid[b_cnt_j] = ap_ssid[b_cnt_j - 1];
                    }
                    ap_ssid[b_cnt_i] = 0x5C;
                    b_cnt_i++;
                }
            }

            for(uint8_t b_cnt_i = 0;b_cnt_i < cb_ap_pass_len;b_cnt_i++)
            {
                if(0x2C == ap_pass[b_cnt_i])
                {
                    for(uint8_t b_cnt_j = cb_ap_pass_len - 1;b_cnt_j > b_cnt_i;b_cnt_j--)
                    {
                        ap_pass[b_cnt_j] = ap_pass[b_cnt_j - 1];
                    }
                    ap_pass[b_cnt_i] = 0x5C;
                    b_cnt_i++;
                }
            }

        if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+CWJAP=\"%s\",\"%s\",%s,%d,%d,%d,%d,%d,%d\r\n",
                        ap_ssid,
                        ap_pass,
                        pst_config_info->union_config_info.st_open_ap_connection.pb_bssid,
                        *pst_config_info->union_config_info.st_open_ap_connection.pb_pci_en,
                        *pst_config_info->union_config_info.st_open_ap_connection.pb_reconn_interval,
                        *pst_config_info->union_config_info.st_open_ap_connection.pb_listen_interval,
                        *pst_config_info->union_config_info.st_open_ap_connection.pb_scan_mode,
                        *pst_config_info->union_config_info.st_open_ap_connection.pb_jap_timeout,
                        *pst_config_info->union_config_info.st_open_ap_connection.pb_pmf))
        {
            return 0;
        }
        pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);
    }

	return 1;
}
uint8_t lte_module_driver_receive_at_conncet_ap(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    uint32_t dw_err_code = 0;
    if(strstr((char*)pst_lte_module_data_info->pb_data,"GOT IP"))
    {
        //network_ap_connect_err_code_set((uint8_t)dw_err_code);
        return 1;
    }
    else if(sscanf(strstr((char*)pst_lte_module_data_info->pb_data,"+CWJAP"),
                   "+CWJAP:%d",
                  &dw_err_code))
    {
        network_ap_connect_err_code_set((uint8_t)dw_err_code);
        return 0;
    }
    else if(strstr((char*)pst_lte_module_data_info->pb_data,"ERROR"))
    {
        return 0;
    }
	return 2;
}

//AT+CIPSTA?
uint8_t ba_local_ip[4] = {0,};
uint8_t lte_module_driver_send_at_read_ip(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+CIPSTA?\r\n"))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_module_driver_receive_at_read_ip(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if(strstr((char*)pst_lte_module_data_info->pb_data,"+CIPSTA"))
    {
        if(sscanf(strstr((char*)pst_lte_module_data_info->pb_data,"ip"),
                  "ip:\"%d,%d,%d,%d\"",
                  (uint32_t *)&ba_local_ip[0],
                  (uint32_t *)&ba_local_ip[1],
                  (uint32_t *)&ba_local_ip[2],
                  (uint32_t *)&ba_local_ip[3]))
        {

        }
        return 1;
    }
	return 2;
}
//AT+CIPSTAMAC?
uint8_t ba_local_mac_addr[30] = {0,};
uint8_t lte_module_driver_send_at_get_mac_addr(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+CIPSTAMAC?\r\n"))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_module_driver_receive_at_get_mac_addr(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	uint8_t ba_local_mac_addr_receive[30] = {0,};
    if(strstr((char*)pst_lte_module_data_info->pb_data,"+CIPSTAMAC:"))
    {
        if(1 == sscanf((char*)pst_lte_module_data_info->pb_data,
                  "+CIPSTAMAC:%[^,]",ba_local_mac_addr_receive))
        {
			for(uint8_t b_cnt_i = 0; b_cnt_i < 17; b_cnt_i ++)
			{
				ba_local_mac_addr[b_cnt_i] = ba_local_mac_addr_receive[b_cnt_i + 1];
			}
        }
        return 1;
    }
	return 2;
}

//AT+CIPRECVMODE
uint8_t lte_module_driver_send_at_set_receive_mode(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+CIPRECVMODE=1\r\n"))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_module_driver_receive_at_set_receive_mode(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if(strstr((char*)pst_lte_module_data_info->pb_data,"OK"))
    {
        return 1;
    }
	return 2;
}
//AT+CWAUTOCONN=<enable>
uint8_t lte_module_driver_send_at_auto_connect(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+CWAUTOCONN=0\r\n"))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_module_driver_receive_at_auto_connect(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if(strstr((char*)pst_lte_module_data_info->pb_data,"\r\nOK"))
    {
        return 1;
    }
	return 2;
}

//AT+CWQAP
uint8_t lte_module_driver_send_at_close_ap(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+CWQAP\r\n"))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_module_driver_receive_at_close_ap(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if(strstr((char*)pst_lte_module_data_info->pb_data,"\r\nOK"))
    {
        return 1;
    }
	return 2;
}

//AT+CWJAP?
uint8_t lte_module_driver_send_at_get_ap_info(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+CWJAP?\r\n"))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_module_driver_receive_at_get_ap_info(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    int dw_rssi_temp = 0;

    lte_module_driver_config_info_general *pst_config_info = (lte_module_driver_config_info_general *)pv_user_data;

    if(strstr((char*)pst_lte_module_data_info->pb_data,"+CWJAP:"))
    {
        if(9==sscanf((char*)pst_lte_module_data_info->pb_data,
                  "+CWJAP:\"%[^\"]\",%[^,],%d,-%d,%d,%d,%d,%d,%d",
                  pst_config_info->union_config_info.st_get_ap_info_connection.pst_data_info->ba_ssid,
                  pst_config_info->union_config_info.st_get_ap_info_connection.pst_data_info->ba_bssid,
                  (uint32_t*)&pst_config_info->union_config_info.st_get_ap_info_connection.pst_data_info->b_channel,
                  &dw_rssi_temp,
                  (uint32_t*)&pst_config_info->union_config_info.st_get_ap_info_connection.pst_data_info->b_pci_en,
                  &pst_config_info->union_config_info.st_get_ap_info_connection.pst_data_info->dw_reconn_interval,
                  &pst_config_info->union_config_info.st_get_ap_info_connection.pst_data_info->dw_listen_interval,
                  (uint32_t*)&pst_config_info->union_config_info.st_get_ap_info_connection.pst_data_info->b_scan_mode,
                  (uint32_t*)&pst_config_info->union_config_info.st_get_ap_info_connection.pst_data_info->b_pmf
                  ))
        {
            if(dw_rssi_temp<50)
            {
                pst_config_info->union_config_info.st_get_ap_info_connection.pst_data_info->b_signal_pct = 100;
            }
            else
            {
                pst_config_info->union_config_info.st_get_ap_info_connection.pst_data_info->b_signal_pct = \
                    2*(100-dw_rssi_temp);
            }
            return 1;
        }
    }
    else if(strstr((char*)pst_lte_module_data_info->pb_data,"\r\nOK"))
    {
        return 0;
    }
	return 2;
}

//AT+WPS=1
uint8_t lte_module_driver_send_at_wps_connect(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+WPS=1\r\n"))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_module_driver_receive_at_wps_connect(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    uint32_t dw_err_code = 0;
    if(strstr((char*)pst_lte_module_data_info->pb_data,"GOT IP"))
    {
        return 1;
    }
    else if(sscanf(strstr((char*)pst_lte_module_data_info->pb_data,"+CWJAP"),
                   "+CWJAP:%d",
                  &dw_err_code))
    {
        network_ap_connect_err_code_set((uint8_t)dw_err_code);
        return 0;
    }
    else if(strstr((char*)pst_lte_module_data_info->pb_data,"ERROR"))
    {
        return 0;
    }
	return 2;
}

uint8_t lte_module_open_ap_connect(lte_module_driver_config_info_general *pst_config_info)
{
    if(0 == pst_config_info)
    {
        return FALSE;
    }

    #define cb_lte_module_open_ap_cmd_deinit_ble                        0
    #define cb_lte_module_open_ap_cmd_at_wifi_mode                      1
    #define cb_lte_module_open_ap_cmd_auto_conncect_disable             2
    #define cb_lte_module_open_ap_cmd_read_connect_state                3
    #define cb_lte_module_open_ap_cmd_close_ap                          4
    #define cb_lte_module_open_ap_cmd_connect_ap                        5
    #define cb_lte_module_open_ap_cmd_read_ip                           6
    #define cb_lte_module_open_ap_cmd_set_receive_mode                  7
    #define cb_lte_module_open_ap_cmd_none                              8
	#define cb_lte_module_open_ap_cmd_get_mac_addr						9

    uint8_t b_step = cb_lte_module_open_ap_cmd_at_wifi_mode;//mark
    uint8_t b_err_cnt = 0;

    void *pv_user_data = (void *)pst_config_info;

    lte_module_driver_process_contral st_process_contral =
    {
        .st_operate_result.b_iot_module_type = iot_module_type_lte,
        .st_operate_result.b_iot_module_operation = lte_module_operate_open_ap_connection,
        .st_operate_result.b_err_code = ap_open_connection_fail,
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
            case cb_lte_module_open_ap_cmd_deinit_ble:
            {
//                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
//                                                             lte_module_driver_send_at_ble_deinit,
//                                                             lte_module_driver_receive_at_ble_deinit))
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_open_ap_cmd_at_wifi_mode;
                }
            }break;
            case cb_lte_module_open_ap_cmd_at_wifi_mode:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
                                                             lte_module_driver_send_at_wifi_mode,
                                                             lte_module_driver_receive_at_wifi_mode))
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_open_ap_cmd_auto_conncect_disable;
                }
            }break;
            case cb_lte_module_open_ap_cmd_auto_conncect_disable:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
                                                             lte_module_driver_send_at_auto_connect,
                                                             lte_module_driver_receive_at_auto_connect))
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_open_ap_cmd_read_connect_state;
                }
            }break;
            case cb_lte_module_open_ap_cmd_read_connect_state:
            {
                uint8_t b_result = 0;
                b_result = lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
                                                             lte_module_driver_send_at_state,
                                                             lte_module_driver_receive_at_state);
                if(1 == b_result)
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_open_ap_cmd_none;
                }
                else if(0 == b_result)
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_open_ap_cmd_close_ap;
                }
            }break;
            case cb_lte_module_open_ap_cmd_close_ap:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(5000,pv_user_data,0,
                                                             lte_module_driver_send_at_close_ap,
                                                             lte_module_driver_receive_at_close_ap))
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_open_ap_cmd_connect_ap;
                }
            }break;
            case cb_lte_module_open_ap_cmd_connect_ap:
            {
                if((FALSE == *pst_config_info->union_config_info.st_open_ap_connection.pf_wps_first_connect) &&\
                   (TRUE == *pst_config_info->union_config_info.st_open_ap_connection.pf_wps_enable))
                {
                    *pst_config_info->union_config_info.st_open_ap_connection.pf_wps_enable = FALSE;
                    fWriteEEPage(16) = TRUE;
                    if(TRUE == lte_module_driver_at_cmd_transmit(16000,pv_user_data,0,
                                                                 lte_module_driver_send_at_wps_connect,
                                                                 lte_module_driver_receive_at_wps_connect))
                    {
                        b_err_cnt = 0;
                        b_step = cb_lte_module_open_ap_cmd_read_ip;
                        *pst_config_info->union_config_info.st_open_ap_connection.pf_wps_first_connect = TRUE;
                        fWriteEEPage(16) = TRUE;
                    }
                    else
                    {
                        st_process_contral.st_operate_result.b_err_code = ap_connect_fail;
                        b_step = cb_lte_module_open_ap_cmd_none;
                    }
                }
                else
                {
                    if(TRUE == lte_module_driver_at_cmd_transmit(16000,pv_user_data,0,
                                                                 lte_module_driver_send_at_conncet_ap,
                                                                 lte_module_driver_receive_at_conncet_ap))
                    {
                        b_err_cnt = 0;
                        b_step = cb_lte_module_open_ap_cmd_read_ip;
                    }
                    else
                    {
                        st_process_contral.st_operate_result.b_err_code = ap_connect_fail;
                        b_step = cb_lte_module_open_ap_cmd_none;
                    }
                }
            }break;
            case cb_lte_module_open_ap_cmd_read_ip:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
                                                             lte_module_driver_send_at_read_ip,
                                                             lte_module_driver_receive_at_read_ip))
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_open_ap_cmd_get_mac_addr;
                }
            }break;
            case cb_lte_module_open_ap_cmd_get_mac_addr:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
                                                             lte_module_driver_send_at_get_mac_addr,
                                                             lte_module_driver_receive_at_get_mac_addr))
                {
                    b_err_cnt = 0;
					uint16_t w_data_len = 30;
					extern uint8_t ba_local_mac_addr[30];
                    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                           cb_device_data_access_data_type_status,
                                           0x017C,
                                           &w_data_len,
                                           (uint8_t *)&ba_local_mac_addr,
                                           cb_device_data_access_operate_type_write);
                    b_step = cb_lte_module_open_ap_cmd_set_receive_mode;
                }
            }break;											 
            case cb_lte_module_open_ap_cmd_set_receive_mode:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
                                                             lte_module_driver_send_at_set_receive_mode,
                                                             lte_module_driver_receive_at_set_receive_mode))
                {
                    b_err_cnt = 0;
                    st_process_contral.st_operate_result.b_err_code = ap_open_connection_success;
                    b_step = cb_lte_module_open_ap_cmd_none;
                }
            }break;
            default:
            {
            }break;
        }
    }while(b_step != cb_lte_module_open_ap_cmd_none);

    if(pst_config_info && pst_config_info->pfunc_operate_result_notify)
    {
        pst_config_info->pfunc_operate_result_notify(&st_process_contral.st_operate_result);
    }

    return TRUE;
}


uint8_t lte_module_close_ap_connect(lte_module_driver_config_info_general *pst_config_info)
{
    if(0 == pst_config_info)
    {
        return FALSE;
    }

    void *pv_user_data = (void *)pst_config_info;

    lte_module_driver_process_contral st_process_contral =
    {
        .st_operate_result.b_iot_module_type = iot_module_type_lte,
        .st_operate_result.b_iot_module_operation = lte_module_operate_close_ap_connection,
        .st_operate_result.b_err_code = ap_close_connection_fail,
    };

    if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
                                                 lte_module_driver_send_at_close_ap,
                                                 lte_module_driver_receive_at_close_ap))
    {
        st_process_contral.st_operate_result.b_err_code = ap_close_connection_success;
    }

    if(pst_config_info && pst_config_info->pfunc_operate_result_notify)
    {
        pst_config_info->pfunc_operate_result_notify(&st_process_contral.st_operate_result);
    }

    return TRUE;
}

uint8_t lte_module_get_ap_info(lte_module_driver_config_info_general *pst_config_info)
{
    if(0 == pst_config_info)
    {
        return FALSE;
    }

    void *pv_user_data = (void *)pst_config_info;


    lte_module_driver_process_contral st_process_contral =
    {
        .st_operate_result.b_iot_module_type = iot_module_type_lte,
        .st_operate_result.b_iot_module_operation = lte_module_operate_get_ap_info,
        .st_operate_result.b_err_code = ap_get_info_fail,
    };

    if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
                                                 lte_module_driver_send_at_get_ap_info,
                                                 lte_module_driver_receive_at_get_ap_info))
    {
        st_process_contral.st_operate_result.b_err_code = ap_get_info_success;
    }


    if(pst_config_info && pst_config_info->pfunc_operate_result_notify)
    {
        pst_config_info->pfunc_operate_result_notify(&st_process_contral.st_operate_result);
    }
    return TRUE;
}

uint8_t lte_module_reset(lte_module_driver_config_info_general *pst_config_info)
{
    if(0 == pst_config_info)
    {
        return FALSE;
    }

    #define cb_lte_module_reset_reset_module              0
    #define cb_lte_module_reset_wait_rdy                  1
    #define cb_lte_module_reset_cmd_at_test               2
    #define cb_lte_module_reset_cmd_ate_disable           3
    #define cb_lte_module_reset_cmd_tcp_port_set          4
    #define cb_lte_module_reset_cmd_none                  5

    uint8_t b_step = cb_lte_module_reset_reset_module;
    uint8_t b_err_cnt = 0;

    void *pv_user_data = (void *)pst_config_info;

    lte_module_driver_process_contral st_process_contral =
    {
        .st_operate_result.b_iot_module_type = iot_module_type_lte,
        .st_operate_result.b_iot_module_operation = lte_module_operate_reset,
        .st_operate_result.b_err_code = lte_module_reset_fail,
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
            case cb_lte_module_reset_reset_module:
            {
//                lte_module_hardware_reset();
                b_step = cb_lte_module_reset_wait_rdy;
            }break;
            case cb_lte_module_reset_wait_rdy:
            {
                lte_module_driver_at_cmd_transmit(10000,pv_user_data,0,
                                                  0,
                                                  lte_module_driver_receive_at_rdy);
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_reset_cmd_at_test;
                }
            }break;
            case cb_lte_module_reset_cmd_at_test:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
                                                             lte_module_driver_send_at_test,
                                                             lte_module_driver_receive_at_test))
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_reset_cmd_ate_disable;
                }
            }break;
            case cb_lte_module_reset_cmd_ate_disable:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
                                                             lte_module_driver_send_at_ate_mode,
                                                             lte_module_driver_receive_at_ate_mode))
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_reset_cmd_tcp_port_set;
                }
            }break;
            case cb_lte_module_reset_cmd_tcp_port_set:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
                                                             lte_module_driver_send_at_tcp_port_set,
                                                             lte_module_driver_receive_at_tcp_port_set))
                {
                    b_err_cnt = 0;
                    st_process_contral.st_operate_result.b_err_code = lte_module_reset_success;
                    b_step = cb_lte_module_reset_cmd_none;
                }
            }break;
            default:
            {
            }break;
        }
    }while(b_step != cb_lte_module_reset_cmd_none);

    if(pst_config_info && pst_config_info->pfunc_operate_result_notify)
    {
        pst_config_info->pfunc_operate_result_notify(&st_process_contral.st_operate_result);
    }

    return TRUE;
}
