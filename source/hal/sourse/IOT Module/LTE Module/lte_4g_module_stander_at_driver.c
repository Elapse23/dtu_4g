#include "lte_4g_module_stander_at_driver.h"
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
#include "lte_4G_module_ble_at_driver.h"


//ready
uint8_t lte_4g_module_driver_receive_at_rdy(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if((pst_lte_module_data_info->w_data_len < 10) &&\
       (strstr((char*)pst_lte_module_data_info->pb_data,"RDY") != 0))
    {
        return 1;
    }

	return 2;
}
//uint8_t lte_4g_module_driver_send_at_ble_init(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
//{
//	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+QBTPWR=1\r\n"))
//	{
//		return 0;
//	}
//	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);
//
//	return 1;
//}
//uint8_t lte_4g_module_driver_receive_at_ble_init(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
//{
//    if(strstr((char*)pst_lte_module_data_info->pb_data,"\r\nOK") != 0)
//    {
//        return 1;
//    }
//
//	return 2;
//}
//uint8_t lte_4g_module_driver_send_at_change_mtu(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
//{
//	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+QBTLEEXMTU=0,246\r\n"))
//	{
//		return 0;
//	}
//	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);
//
//	return 1;
//}
//uint8_t lte_4g_module_driver_receive_at_change_mtu(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
//{
//    if(strstr((char*)pst_lte_module_data_info->pb_data,"\r\nOK") != 0)
//    {
//        return 1;
//    }
//
//	return 2;
//}

//AT
uint8_t lte_4g_module_driver_send_at_test(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT\r\n"))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_4g_module_driver_receive_at_test(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if((pst_lte_module_data_info->w_data_len <= 10) &&\
       (strstr((char*)pst_lte_module_data_info->pb_data,"OK") != 0))
    {
        return 1;
    }

	return 2;
}
//AT+RST
uint8_t lte_4g_module_driver_send_at_rst_fds(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT&F\r\n"))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_4g_module_driver_receive_at_rst_fds(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if((pst_lte_module_data_info->w_data_len < 20) &&\
       (strstr((char*)pst_lte_module_data_info->pb_data,"OK") != 0))
    {
        return 1;
    }

	return 2;
}

uint8_t lte_4g_module_driver_send_at_close_ack(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"ATE0\r\n"))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_4g_module_driver_receive_at_close_ack(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
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
uint8_t lte_4g_module_driver_send_at_ate_mode(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"ATE%d\r\n",b_lte_ATEfag))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_4g_module_driver_receive_at_ate_mode(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if((pst_lte_module_data_info->w_data_len < 20) &&\
       (strstr((char*)pst_lte_module_data_info->pb_data,"OK") != 0))
    {
        return 1;
    }

	return 2;
}
//AT+CIPTCPOPT=[<so_linger>],[<tcp_nodelay>],[<so_sndtimeo>]
uint8_t lte_4g_module_driver_send_at_tcp_port_set(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{

	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,
                    "AT+CIPTCPOPT=-1,0,5000\r\n"))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_4g_module_driver_receive_at_tcp_port_set(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if((pst_lte_module_data_info->w_data_len < 8) &&\
       (strstr((char*)pst_lte_module_data_info->pb_data,"OK") != 0))
    {
        return 1;
    }

	return 2;
}

// AT+UART_DEF=921600,8,1,0,0
uint8_t lte_4g_module_driver_send_at_uart(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,
                    "AT+IPR=%d\r\n",
                    115200))
    {
        return 0;
    }
    pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);
    return 1;
}

uint8_t lte_4g_module_driver_receive_at_uart(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
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


uint8_t lte_4g_module_initial(lte_4g_module_driver_config_info_general *pst_config_info)
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
	#define cb_lte_module_initial_ble_init					7
	#define cb_lte_module_initial_change_mtu				8

    uint8_t b_step = cb_lte_module_initial_wait_rdy;
    uint8_t b_step_last = cb_lte_module_initial_wait_rdy;
    uint8_t b_err_cnt = 0;

    void *pv_user_data = (void *)pst_config_info;

    lte_4g_module_driver_process_contral st_process_contral =
    {
        .st_operate_result.b_iot_module_type = iot_module_type_4g,
        .st_operate_result.b_iot_module_operation = lte_4g_module_operate_initial,
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
                                                  lte_4g_module_driver_receive_at_rdy);
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
//            	lte_4g_module_hardware_open();
                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
															lte_4g_module_driver_send_at_test,
															lte_4g_module_driver_receive_at_test))

                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_initial_cmd_ate_disable;
                }									
            }break;
            case cb_lte_module_initial_cmd_ate_disable:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
                                                             lte_4g_module_driver_send_at_ate_mode,
                                                             lte_4g_module_driver_receive_at_ate_mode))
                {
                    b_err_cnt = 0;
					st_process_contral.st_operate_result.b_err_code = lte_module_initial_success;
                    b_step = cb_lte_module_initial_cmd_none;
                }
            }break;
            case cb_lte_module_initial_cmd_rst:
            {
                lte_4g_module_hardware_reset();
                b_step = cb_lte_module_initial_wait_rdy;
                b_step_last = cb_lte_module_initial_cmd_rst;
            }break;
			case cb_lte_module_initial_cmd_uart:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
                                                             lte_4g_module_driver_send_at_uart,
                                                             lte_4g_module_driver_receive_at_uart))
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_initial_cmd_rst;
                    set_usart_lte_baudrate_115200();
                }

                if( b_err_cnt >= 2)
                {
                	if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
                                                     lte_4g_module_driver_send_at_rst_fds,
                                                     lte_4g_module_driver_receive_at_rst_fds))
                	{

						set_usart_lte_baudrate_115200();
						lte_4g_module_hardware_reset();
						b_step = cb_lte_module_initial_wait_rdy;
					}
					else
					{
						set_usart_lte_baudrate_115200();
						lte_4g_module_hardware_reset();
						b_step = cb_lte_module_initial_cmd_uart;
					}
                }
            }break;
//			case cb_lte_module_initial_ble_init:
//            {
//                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
//                                                             lte_4g_module_driver_send_at_ble_init,
//                                                             lte_4g_module_driver_receive_at_ble_init))
//                {
//                    b_err_cnt = 0;
//					st_process_contral.st_operate_result.b_err_code = lte_module_initial_success;
//                    b_step = cb_lte_module_initial_cmd_none;
//                }
//            }break;
//			case cb_lte_module_initial_change_mtu:
//            {
//                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
//                                                             lte_4g_module_driver_send_at_change_mtu,
//                                                             lte_4g_module_driver_receive_at_change_mtu))
//                {
//                    b_err_cnt = 0;
//					st_process_contral.st_operate_result.b_err_code = lte_module_initial_success;
//                    b_step = cb_lte_module_initial_cmd_none;
//                }
//            }break;
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
//uint8_t lte_module_driver_send_at_chk_sim_signal(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
//{
//	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+CSQ\r\n"))
//	{
//		return 0;
//	}
//	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);
//
//	return 1;
//}
//uint8_t lte_module_driver_receive_at_chk_sim_signal(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
//{
//
//	lte_4g_module_driver_config_info_general *pst_config_info = (lte_4g_module_driver_config_info_general *)pv_user_data;
//
//    uint8_t *pb_data = NULL;
//	uint16_t w_data_len = 0;
//	pb_data = (uint8_t *)strstr((char*)pst_lte_module_data_info->pb_data,"+CSQ: ");
//    if(NULL != pb_data)
//    {
//        if(sscanf((char *)pb_data,"+CSQ: %d,%d",&pst_config_info->union_config_info.st_open_sim_connection.pb_4g_signal_strength,&pst_config_info->union_config_info.st_open_sim_connection.pb_channel_bit_error_rate))
//        {
//            if(pst_config_info->union_config_info.st_open_sim_connection.pb_4g_signal_strength != (uint8_t *)99)
//            {
//
//                return 1;
//
//            }
//        }
//    }
//
////    if((strstr((char*)pst_lte_module_data_info->pb_data,"OK") != 0))
////    {
////        return 1;
////    }
//
//	return 2;
//}
//AT+CWSTATE?
uint8_t lte_module_driver_send_at_chk_sim_reg(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+CREG?\r\n"))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_module_driver_receive_at_chk_sim_reg(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if((strstr((char*)pst_lte_module_data_info->pb_data,"OK") != 0))
    {
        return 1;
    }

	return 2;
}

//AT+CPIN?
uint8_t lte_module_driver_send_at_chk_sim_pin(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+CPIN?\r\n"))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_module_driver_receive_at_chk_sim_pin(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if(strstr((char*)pst_lte_module_data_info->pb_data,"READY"))
    {
        return 1;
    }
	else if(strstr((char*)pst_lte_module_data_info->pb_data,"ERROR"))
    {
        return 0;
    }
	return 2;
}

//AT+CGREG
uint8_t lte_module_driver_send_at_chk_sim_net_reg(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+CGREG?\r\n"))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_module_driver_receive_at_chk_sim_net_reg(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if(strstr((char*)pst_lte_module_data_info->pb_data,"OK"))
    {
        return 1;
    }
	return 2;
}

uint8_t lte_module_driver_send_at_chk_operator(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+COPS?\r\n"))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}

uint8_t lte_module_driver_receive_at_chk_operator(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	lte_4g_module_driver_config_info_general *pst_config_info = (lte_4g_module_driver_config_info_general *)pv_user_data;

	if(strstr((char *)pst_lte_module_data_info->pb_data, "CHINA MOBILE"))				//中国移动
	{
		pst_config_info->union_config_info.st_open_sim_connection.b_cops_type = CMNET;

		return 1;
	}
	else if(strstr((char *)pst_lte_module_data_info->pb_data, "CHN-UNICOM"))			//中国联通
	{
        pst_config_info->union_config_info.st_open_sim_connection.b_cops_type = UNINET;

		return 1;
	}
	else if(strstr((char *)pst_lte_module_data_info->pb_data, "CHN-CT"))				//中国电信
	{
        pst_config_info->union_config_info.st_open_sim_connection.b_cops_type = CTLTE;

		return 1;
	}
	return 2;
}

uint8_t lte_module_driver_send_at_set_operator_apn(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    lte_4g_module_driver_config_info_general *pst_config_info = (lte_4g_module_driver_config_info_general *)pv_user_data;
	if(CMNET == pst_config_info->union_config_info.st_open_sim_connection.b_cops_type)
	{
		if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+QICSGP=1,1,\"CMNET\",\"\",\"\",1\r\n"))//设置接入点（移动）APN,USERNAME,PASSWORD
		{
			return 0;
		}
	}
	else if(UNINET == pst_config_info->union_config_info.st_open_sim_connection.b_cops_type)
	{
		if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+QICSGP=1,1,\"UNINET\",\"\",\"\",1\r\n"))//设置接入点（联通） APN,USERNAME,PASSWORD
		{
			return 0;
		}
	}
	else if(CTLTE == pst_config_info->union_config_info.st_open_sim_connection.b_cops_type)
	{
		if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+QICSGP=1,1,\"CTLTE\",\"\",\"\",1\r\n"))//设置接入点（电信） APN,USERNAME,PASSWORD
		{
			return 0;
		}
	}

	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_module_driver_receive_at_set_operator_apn(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if(strstr((char*)pst_lte_module_data_info->pb_data,"OK"))
    {
        return 1;
    }
	return 2;
}

uint8_t lte_module_driver_send_at_active_point(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+QIACT=1\r\n"))
	{
		return 0;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return 1;
}
uint8_t lte_module_driver_receive_at_active_point(void *pv_user_data, lte_module_data_info *pst_lte_module_data_info)
{
    if(strstr((char*)pst_lte_module_data_info->pb_data,"OK"))
    {
        return 1;
    }
	return 2;
}

uint8_t lte_4g_module_open_sim_card_connect(lte_4g_module_driver_config_info_general *pst_config_info)
{
    if(0 == pst_config_info)
    {
        return FALSE;
    }

    #define cb_lte_module_outof_time_reboot_cmd                         0
    #define cb_lte_module_open_sim_card_cmd_deinit_ble                  1
    #define cb_lte_module_open_sim_card_cmd_at_chk_sim_card             2
    #define cb_lte_module_open_sim_card_cmd_chk_signal                	3
    #define cb_lte_module_open_sim_card_cmd_chk_reg                     4
    #define cb_lte_module_open_sim_card_cmd_chk_net_reg                 5
    #define cb_lte_module_open_sim_card_cmd_chk_operator                6
    #define cb_lte_module_open_sim_card_cmd_set_point                  	7
    #define cb_lte_module_open_sim_card_cmd_active_point                8
	#define cb_lte_module_open_sim_card_cmd_none						9

    uint8_t b_step = cb_lte_module_open_sim_card_cmd_at_chk_sim_card;//mark
    uint8_t b_err_cnt = 0;
	uint8_t b_reboot_cnt = 0;
    void *pv_user_data = (void *)pst_config_info;

    lte_4g_module_driver_process_contral st_process_contral =
    {
        .st_operate_result.b_iot_module_type = iot_module_type_4g,
        .st_operate_result.b_iot_module_operation = lte_4g_module_operate_open_sim_card_connection,
        .st_operate_result.b_err_code = sim_open_connection_fail,
    };

    do
    {

        if(b_err_cnt > 3)
        {
        	b_err_cnt = 0;
            break;
        }
        switch(b_step)
        {
        	case cb_lte_module_outof_time_reboot_cmd://超时重启
			{
//				lte_4g_module_hardware_reset();
	            b_step = cb_lte_module_open_sim_card_cmd_at_chk_sim_card;
			}
			break;
            case cb_lte_module_open_sim_card_cmd_deinit_ble:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
                                                             lte_4g_module_driver_send_at_ble_deinit,
                                                             lte_4g_module_driver_receive_at_ble_deinit))
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_open_sim_card_cmd_at_chk_sim_card;
                }
            }break;
            case cb_lte_module_open_sim_card_cmd_at_chk_sim_card:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(5000,pv_user_data,0,
                                                             lte_module_driver_send_at_chk_sim_pin,
                                                             lte_module_driver_receive_at_chk_sim_pin))
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_open_sim_card_cmd_chk_reg;
                }
				else
				{
					b_reboot_cnt ++;
				  	b_step = cb_lte_module_open_sim_card_cmd_at_chk_sim_card;
				}
				if(b_reboot_cnt > 9)
				{
				 	b_reboot_cnt = 0;
				 	b_step = cb_lte_module_outof_time_reboot_cmd;
				 	b_err_cnt ++;
				}
            }break;
//            case cb_lte_module_open_sim_card_cmd_chk_signal:
//            {
//                if(TRUE == lte_module_driver_at_cmd_transmit(300,pv_user_data,0,
//                                                             lte_module_driver_send_at_chk_sim_signal,
//                                                             lte_module_driver_receive_at_chk_sim_signal))
//				{
//					b_step = cb_lte_module_open_sim_card_cmd_chk_reg;
//				}
//				else
//				{
//					b_step = cb_lte_module_outof_time_reboot_cmd;
//				}
//
//            }break;
            case cb_lte_module_open_sim_card_cmd_chk_reg:
            {
            	vTaskDelay(pdMS_TO_TICKS_USER(500));
                if(TRUE == lte_module_driver_at_cmd_transmit(600,pv_user_data,0,
                                                             lte_module_driver_send_at_chk_sim_reg,
                                                             lte_module_driver_receive_at_chk_sim_reg))
				{
					b_step = cb_lte_module_open_sim_card_cmd_chk_net_reg;
				}
				else
				{
					b_reboot_cnt ++;
					b_step = cb_lte_module_open_sim_card_cmd_chk_reg;
				}
				if(b_reboot_cnt > 30)
				{
					b_reboot_cnt = 0;
					b_step = cb_lte_module_outof_time_reboot_cmd;
					b_err_cnt ++;

				}

            }break;
            case cb_lte_module_open_sim_card_cmd_chk_net_reg:
            {
            	vTaskDelay(pdMS_TO_TICKS_USER(500));
                if(TRUE == lte_module_driver_at_cmd_transmit(600,pv_user_data,0,
                                                             lte_module_driver_send_at_chk_sim_net_reg,
                                                             lte_module_driver_receive_at_chk_sim_net_reg))
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_open_sim_card_cmd_chk_operator;
                }
				else
				{
					 b_reboot_cnt ++;
					 b_step = cb_lte_module_open_sim_card_cmd_chk_net_reg;
				}
				if(b_reboot_cnt > 30)
				{
					b_reboot_cnt = 0;
					b_step = cb_lte_module_outof_time_reboot_cmd;
					b_err_cnt ++;

				}

            }break;
			case cb_lte_module_open_sim_card_cmd_chk_operator:
            {
            	vTaskDelay(pdMS_TO_TICKS_USER(500));
                if(TRUE == lte_module_driver_at_cmd_transmit(6000,pv_user_data,0,
                                                             lte_module_driver_send_at_chk_operator,
                                                             lte_module_driver_receive_at_chk_operator))
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_open_sim_card_cmd_set_point;
                }
				else
				{
					 b_reboot_cnt ++;
					 b_step = cb_lte_module_open_sim_card_cmd_chk_operator;
				}
				if(b_reboot_cnt > 9)
				{
					b_reboot_cnt = 0;
				 	b_step = cb_lte_module_outof_time_reboot_cmd;
					b_err_cnt ++;

				}

            }break;
			case cb_lte_module_open_sim_card_cmd_set_point:
            {
				vTaskDelay(pdMS_TO_TICKS_USER(500));
                if(TRUE == lte_module_driver_at_cmd_transmit(6000,pv_user_data,0,
                                                             lte_module_driver_send_at_set_operator_apn,
                                                             lte_module_driver_receive_at_set_operator_apn))
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_open_sim_card_cmd_active_point;
                }
				else
				{
					b_reboot_cnt ++;
				  	b_step = cb_lte_module_open_sim_card_cmd_set_point;
				}
				if(b_reboot_cnt > 9)
				{
				 	b_reboot_cnt = 0;
				 	b_step = cb_lte_module_outof_time_reboot_cmd;
				 	b_err_cnt ++;

				}

            }break;
			case cb_lte_module_open_sim_card_cmd_active_point:
            {
            	vTaskDelay(pdMS_TO_TICKS_USER(500));
                if(TRUE == lte_module_driver_at_cmd_transmit(150000,pv_user_data,0,
                                                             lte_module_driver_send_at_active_point,
                                                             lte_module_driver_receive_at_active_point))
                {
                    b_err_cnt = 0;
					st_process_contral.st_operate_result.b_err_code = sim_open_connection_success;
                    b_step = cb_lte_module_open_sim_card_cmd_none;
                }
				else
				{
					b_reboot_cnt ++;
				  	b_step = cb_lte_module_open_sim_card_cmd_active_point;
				}
				if(b_reboot_cnt > 9)
				{
				 	b_reboot_cnt = 0;
				 	b_step = cb_lte_module_outof_time_reboot_cmd;
				 	b_err_cnt ++;

				}
            }break;
            default:
            {
            }break;
        }
    }while(b_step != cb_lte_module_open_sim_card_cmd_none);

    if(pst_config_info && pst_config_info->pfunc_operate_result_notify)
    {
        pst_config_info->pfunc_operate_result_notify(&st_process_contral.st_operate_result);
    }

    return TRUE;
}


uint8_t lte_4g_module_close_sim_card_connect(lte_4g_module_driver_config_info_general *pst_config_info)
{
    if(0 == pst_config_info)
    {
        return FALSE;
    }

    void *pv_user_data = (void *)pst_config_info;

    lte_module_driver_process_contral st_process_contral =
    {
        .st_operate_result.b_iot_module_type = iot_module_type_4g,
        .st_operate_result.b_iot_module_operation = lte_4g_module_operate_close_sim_card_connection,
        .st_operate_result.b_err_code = sim_close_connection_fail,
    };

//    if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
//                                                 lte_module_driver_send_at_close_ap,
//                                                 lte_module_driver_receive_at_close_ap))
//    {
//        st_process_contral.st_operate_result.b_err_code = ap_close_connection_success;
//    }

    if(pst_config_info && pst_config_info->pfunc_operate_result_notify)
    {
        pst_config_info->pfunc_operate_result_notify(&st_process_contral.st_operate_result);
    }

    return TRUE;
}

uint8_t lte_4g_module_reset(lte_4g_module_driver_config_info_general *pst_config_info)
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

    lte_4g_module_driver_process_contral st_process_contral =
    {
        .st_operate_result.b_iot_module_type = iot_module_type_4g,
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
                lte_4g_module_hardware_reset();
                b_step = cb_lte_module_reset_cmd_at_test;
            }break;
            case cb_lte_module_reset_wait_rdy:
            {
                lte_module_driver_at_cmd_transmit(10000,pv_user_data,0,
                                                  0,
                                                  lte_4g_module_driver_receive_at_rdy);
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_reset_cmd_at_test;
                }
            }break;
            case cb_lte_module_reset_cmd_at_test:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
                                                             lte_4g_module_driver_send_at_test,
                                                             lte_4g_module_driver_receive_at_test))
                {
                    b_err_cnt = 0;
                    b_step = cb_lte_module_reset_cmd_ate_disable;
                }
            }break;
            case cb_lte_module_reset_cmd_ate_disable:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(1000,pv_user_data,0,
                                                             lte_4g_module_driver_send_at_ate_mode,
                                                             lte_4g_module_driver_receive_at_ate_mode))
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

