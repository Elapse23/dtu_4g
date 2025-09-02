#include "network_iot_module_data_channel_manage.h"
#include "application_general_macro.h"
#include "iot_module_config_common.h"
#include "extern_flash_operate.h"
#include "network_iot_module_data_queue.h"
#include "spi_flash_address_allocate.h"
#include "crc16_arithmetic.h"
#include "freertos_user_config.h"
#include <string.h>
#include "network_protocal_data_assembly.h"
#include "network_protocal.h"
#include "network_tbb_device_server_communicate.h"
#include "network_ble_communication.h"
#include "display_management.h"
#include "network_local_config.h"
#include "network_local_device_secquence_operate.h"
#include "net_device_data_access.h"
#include "printf_custom.h"



network_send_data_channel iot_module_send_data = 0;
network_receive_data_channel iot_module_receive_data = 0;

uint16_t w_net_iot_module_mps = 0;

static uint8_t network_iot_module_communicate_channel_operate_notify(iot_module_driver_operate_result *pst_operate_result);

/************************************************************************************************
* Input:
*
* Output:
*
* Logic discription: network transfer interface to subset
*
* run time:
************************************************************************************************/
uint8_t network_data_channel_initial(network_send_data_channel px_send,\
                                     network_receive_data_channel px_receive)
{
    if((NULL == px_send) || (NULL == px_receive))
    {
        return FALSE;
    }

    iot_module_receive_data = px_receive;
    iot_module_send_data = px_send;

    return TRUE;
}

/************************************************************************************************************
iot module driver
************************************************************************************************************/
//contextID          1-16
//tcp                multiple,contextID 1-16 : connectID  0-11
//可以建立多个contextID，但每个contextID对应的IP、PORT都一样，则tcp connectID能建立连接的只有一个
//mqtt_channel       multiple,contextID 1-16 : mqtt_client_index  0-5

//http_channel       single,contextID 1-16
//ssl_channle        multiple,contextID 1-16 : sslctxID  0-5

//app_net_channel_id : iot_net_channel_id
//cb_network_protocal_type_tcp_socket : contextID?,(sslctxID?),connectID?

//uint8_t lte_module_driver_reset_tcp_connection(lte_module_driver_config_info_general *pst_config_info)
//{
//    if(NULL == pst_config_info)
//    {
//        return FALSE;
//    }
//
//    return FALSE;
//}
//
//uint8_t lte_module_driver_open_tcp_connection(lte_module_driver_config_info_general *pst_config_info)
//{
//    if(NULL == pst_config_info)
//    {
//        return FALSE;
//    }
//
//    return FALSE;
//}
//
//uint8_t lte_module_driver_close_tcp_connection(lte_module_driver_config_info_general *pst_config_info)
//{
//    if(NULL == pst_config_info)
//    {
//        return FALSE;
//    }
//
//    return FALSE;
//}
//
//uint8_t lte_module_driver_send_tcp_user_data(lte_module_driver_config_info_general *pst_config_info)
//{
//    if(NULL == pst_config_info)
//    {
//        return FALSE;
//    }
//
//    return FALSE;
//}
//
//uint8_t lte_module_driver_receive_tcp_user_data(lte_module_driver_config_info_general *pst_config_info)
//{
//    if(NULL == pst_config_info)
//    {
//        return FALSE;
//    }
//
//    return FALSE;
//}
//
//uint8_t lte_module_driver_reset_mqtts_connection(lte_module_driver_config_info_general *pst_config_info)
//{
//    if(NULL == pst_config_info)
//    {
//        return FALSE;
//    }
//
//    return FALSE;
//}
//
//uint8_t lte_module_driver_reset_https_connection(lte_module_driver_config_info_general *pst_config_info)
//{
//    if(NULL == pst_config_info)
//    {
//        return FALSE;
//    }
//
//    return FALSE;
//}
//
//uint8_t lte_module_driver_open_https_connection(lte_module_driver_config_info_general *pst_config_info)
//{
//    if(NULL == pst_config_info)
//    {
//        return FALSE;
//    }
//
//    return FALSE;
//}
//
//uint8_t lte_module_driver_close_https_connection(lte_module_driver_config_info_general *pst_config_info)
//{
//    if(NULL == pst_config_info)
//    {
//        return FALSE;
//    }
//
//    return FALSE;
//}
//
//uint8_t lte_module_driver_send_http_user_data(lte_module_driver_config_info_general *pst_config_info)
//{
//    if(NULL == pst_config_info)
//    {
//        return FALSE;
//    }
//
//    return FALSE;
//}
//
//uint8_t lte_module_driver_receive_http_user_data(lte_module_driver_config_info_general *pst_config_info)
//{
//    if(NULL == pst_config_info)
//    {
//        return FALSE;
//    }
//
//    return FALSE;
//}

/************************************************************************************************************
middle layer
************************************************************************************************************/
enum
{
    network_iot_connection_state_open,
    network_iot_connection_state_close,
    network_iot_connection_state_busy
}network_iot_connection_state;

network_iot_module_connection_info sta_network_iot_module_connection_info[connection_type_number] =
{
    {
        .b_connection_type = connection_type_tbb_app,
        .connection_state.w_timeout_threshold_ms = 10000,
    },
    {
        .b_connection_type = connection_type_tbb_server,
        .connection_state.w_timeout_threshold_ms = 10000,
        {

            .sta_lte_module_config[connection_operation_open].union_config_info.st_open_tcp_connection.ba_domain =\
                "tbbEquipment.tbbpower.com",
            .sta_lte_module_config[connection_operation_open].union_config_info.st_open_tcp_connection.w_domain_port =\
                10001,

            .sta_lte_module_config[connection_operation_open_ap].union_config_info.st_open_ap_connection.pb_ssid =\
                st_network_ap_connect_info.st_ap_info.ba_ssid,
            .sta_lte_module_config[connection_operation_open_ap].union_config_info.st_open_ap_connection.pb_pass =\
                st_network_ap_connect_info.st_ap_info.ba_pass,
            .sta_lte_module_config[connection_operation_open_ap].union_config_info.st_open_ap_connection.pb_bssid =\
                st_network_ap_set_info.bssid.bssid_info.ba_data,
            .sta_lte_module_config[connection_operation_open_ap].union_config_info.st_open_ap_connection.pb_pci_en =\
                &st_network_ap_set_info.b_pci_en,
            .sta_lte_module_config[connection_operation_open_ap].union_config_info.st_open_ap_connection.pb_reconn_interval =\
                &st_network_ap_set_info.b_reconn_interval,
            .sta_lte_module_config[connection_operation_open_ap].union_config_info.st_open_ap_connection.pb_listen_interval =\
                &st_network_ap_set_info.b_listen_interval,
            .sta_lte_module_config[connection_operation_open_ap].union_config_info.st_open_ap_connection.pb_scan_mode =\
                &st_network_ap_set_info.b_scan_mode,
            .sta_lte_module_config[connection_operation_open_ap].union_config_info.st_open_ap_connection.pb_jap_timeout =\
                &st_network_ap_set_info.b_jap_timeout,
            .sta_lte_module_config[connection_operation_open_ap].union_config_info.st_open_ap_connection.pb_pmf =\
                &st_network_ap_set_info.b_pmf,
            .sta_lte_module_config[connection_operation_open_ap].union_config_info.st_open_ap_connection.pf_wps_enable =\
                &st_network_ap_set_info.f_wps_enable,
            .sta_lte_module_config[connection_operation_open_ap].union_config_info.st_open_ap_connection.pf_wps_first_connect =\
                &st_network_ap_set_info.f_wps_first_connect,

			.sta_lte_4g_module_config[connection_operation_open].union_config_info.st_open_tcp_connection.ba_domain =\
                "tbbEquipment.tbbpower.com",
            .sta_lte_4g_module_config[connection_operation_open].union_config_info.st_open_tcp_connection.w_domain_port =\
                10001,
        },
    },
};

void network_iot_module_connection_info_initial()
{
    {
        uint16_t w_data_len = 129;
        net_system_data_access(cb_device_data_access_data_type_set,
                               0x000B,
                               &w_data_len,
                               (uint8_t *)&st_network_ap_connect_info.ba_data[0],
                               cb_device_data_access_operate_type_write);
        w_data_len = 30;
        uint8_t ba_sn_temp[30] = {0,};
        get_local_device_secquence(ba_sn_temp,30);
        net_system_data_access(cb_device_data_access_data_type_set,
                               0x000F,
                               &w_data_len,
                               (uint8_t *)&ba_sn_temp[0],
                               cb_device_data_access_operate_type_write);
        net_system_data_access(cb_device_data_access_data_type_set,
                               0x0010,
                               &w_data_len,
                               (uint8_t *)&ba_sn_temp[0],
                               cb_device_data_access_operate_type_write);
    }
}

uint8_t network_iot_module_communicate_channel_operate_prepare(uint8_t b_iot_module_type,
                                                               uint8_t b_iot_module_operation,
                                                               uint16_t w_data_len,
                                                               void *pv_data,
                                                               network_iot_config_info *pst_config_info)
{
    if(NULL == pst_config_info)
    {
        return FALSE;
    }

    switch(b_iot_module_type)
    {
        case iot_module_type_lte:
        {
            pst_config_info->union_config_info.st_lte_module.enum_operate_type = (lte_module_operate_type)b_iot_module_operation;
            pst_config_info->union_config_info.st_lte_module.pfunc_operate_result_notify = network_iot_module_communicate_channel_operate_notify;
            switch(b_iot_module_operation)
            {
                case lte_module_operate_initial:
                case lte_module_operate_reset:
                {
                    return TRUE;
                }break;
                case lte_module_operate_ble_config:
                case lte_module_operate_ble_close:
                case lte_module_operate_ble_open_broadcast:
                case lte_module_operate_close_ap_connection:
                {
                    return TRUE;
                }break;
                case lte_module_operate_get_ap_info:
                {
                    pst_config_info->union_config_info.st_lte_module.union_config_info.st_get_ap_info_connection.pst_data_info =\
                        (lte_module_ap_info *)pv_data;
                    return TRUE;
                }break;
                case lte_module_operate_open_ap_connection:
                {
                    pst_config_info->union_config_info.st_lte_module.union_config_info.st_open_ap_connection.pb_ssid =\
                        sta_network_iot_module_connection_info[connection_type_tbb_server].
                        union_iot_module_config_info.
                        sta_lte_module_config[connection_operation_open_ap].
                        union_config_info.st_open_ap_connection.pb_ssid;
                    pst_config_info->union_config_info.st_lte_module.union_config_info.st_open_ap_connection.pb_pass =\
                        sta_network_iot_module_connection_info[connection_type_tbb_server].
                        union_iot_module_config_info.
                        sta_lte_module_config[connection_operation_open_ap].
                        union_config_info.st_open_ap_connection.pb_pass;
                    if(st_network_ap_set_info.bssid.f_write_bssid)
                    {
                        pst_config_info->union_config_info.st_lte_module.union_config_info.st_open_ap_connection.pb_bssid =\
                            sta_network_iot_module_connection_info[connection_type_tbb_server].
                            union_iot_module_config_info.
                            sta_lte_module_config[connection_operation_open_ap].
                            union_config_info.st_open_ap_connection.pb_bssid;
                    }
                    else
                    {
                        pst_config_info->union_config_info.st_lte_module.union_config_info.st_open_ap_connection.pb_bssid = 0;
                    }
                    pst_config_info->union_config_info.st_lte_module.union_config_info.st_open_ap_connection.pb_pci_en =\
                        sta_network_iot_module_connection_info[connection_type_tbb_server].
                        union_iot_module_config_info.
                        sta_lte_module_config[connection_operation_open_ap].
                        union_config_info.st_open_ap_connection.pb_pci_en;
                    pst_config_info->union_config_info.st_lte_module.union_config_info.st_open_ap_connection.pb_reconn_interval =\
                        sta_network_iot_module_connection_info[connection_type_tbb_server].
                        union_iot_module_config_info.
                        sta_lte_module_config[connection_operation_open_ap].
                        union_config_info.st_open_ap_connection.pb_reconn_interval;
                    pst_config_info->union_config_info.st_lte_module.union_config_info.st_open_ap_connection.pb_listen_interval =\
                        sta_network_iot_module_connection_info[connection_type_tbb_server].
                        union_iot_module_config_info.
                        sta_lte_module_config[connection_operation_open_ap].
                        union_config_info.st_open_ap_connection.pb_listen_interval;
                    pst_config_info->union_config_info.st_lte_module.union_config_info.st_open_ap_connection.pb_scan_mode =\
                        sta_network_iot_module_connection_info[connection_type_tbb_server].
                        union_iot_module_config_info.
                        sta_lte_module_config[connection_operation_open_ap].
                        union_config_info.st_open_ap_connection.pb_scan_mode;
                    pst_config_info->union_config_info.st_lte_module.union_config_info.st_open_ap_connection.pb_jap_timeout =\
                        sta_network_iot_module_connection_info[connection_type_tbb_server].
                        union_iot_module_config_info.
                        sta_lte_module_config[connection_operation_open_ap].
                        union_config_info.st_open_ap_connection.pb_jap_timeout;
                    pst_config_info->union_config_info.st_lte_module.union_config_info.st_open_ap_connection.pb_pmf =\
                        sta_network_iot_module_connection_info[connection_type_tbb_server].
                        union_iot_module_config_info.
                        sta_lte_module_config[connection_operation_open_ap].
                        union_config_info.st_open_ap_connection.pb_pmf;
                    pst_config_info->union_config_info.st_lte_module.union_config_info.st_open_ap_connection.pf_wps_enable =\
                        sta_network_iot_module_connection_info[connection_type_tbb_server].
                        union_iot_module_config_info.
                        sta_lte_module_config[connection_operation_open_ap].
                        union_config_info.st_open_ap_connection.pf_wps_enable;
                    pst_config_info->union_config_info.st_lte_module.union_config_info.st_open_ap_connection.pf_wps_first_connect =\
                        sta_network_iot_module_connection_info[connection_type_tbb_server].
                        union_iot_module_config_info.
                        sta_lte_module_config[connection_operation_open_ap].
                        union_config_info.st_open_ap_connection.pf_wps_first_connect;
                    return TRUE;
                }break;
                case lte_module_operate_reset_tcp_connection:
                case lte_module_operate_open_tcp_connection:
                {
                    pst_config_info->union_config_info.st_lte_module.union_config_info.st_open_tcp_connection =\
                        sta_network_iot_module_connection_info[connection_type_tbb_server].
                        union_iot_module_config_info.
                        sta_lte_module_config[connection_operation_open].
                        union_config_info.st_open_tcp_connection;
                    return TRUE;
                }break;
                case lte_module_operate_close_tcp_connection:
                {
                        pst_config_info->union_config_info.st_lte_module.union_config_info.st_close_tcp_connection.b_channel_index =\
                        sta_network_iot_module_connection_info[connection_type_tbb_server].
                        union_iot_module_config_info.
                        sta_lte_module_config[connection_operation_open].
                        union_config_info.
                        st_open_tcp_connection.
                        b_channel_index;
                    return TRUE;
                }break;
                case lte_module_operate_send_tcp_user_data:
                {
                    pst_config_info->union_config_info.st_lte_module.union_config_info.st_send_tcp_user_data.st_data_info.w_data_len =\
                        w_data_len;

                    pst_config_info->union_config_info.st_lte_module.union_config_info.st_send_tcp_user_data.st_data_info.pb_data =\
                        (uint8_t *)pv_data;
                    return TRUE;
                }break;
                case lte_module_operate_receive_tcp_user_data:
                {
                    pst_config_info->
                        union_config_info.
                        st_lte_module.
                        union_config_info.
                        st_receive_tcp_user_data.
                        b_channel_index =\
                    sta_network_iot_module_connection_info[connection_type_tbb_server].
                        union_iot_module_config_info.
                        sta_lte_module_config[connection_operation_open].
                        union_config_info.
                        st_open_tcp_connection.
                        b_channel_index;
                    pst_config_info->
                        union_config_info.
                        st_lte_module.
                        union_config_info.
                        st_receive_tcp_user_data.
                        pst_data_info = (genaral_communicate_data_info *)pv_data;
                    return TRUE;
                }break;
                case lte_module_operate_send_ble_user_data:
                {
                    pst_config_info->union_config_info.st_lte_module.union_config_info.st_send_ble_user_data.st_data_info.w_data_len =\
                        w_data_len;

                    pst_config_info->union_config_info.st_lte_module.union_config_info.st_send_ble_user_data.st_data_info.pb_data =\
                        (uint8_t *)pv_data;
                    return TRUE;
                }break;
            }
        }break;
		case iot_module_type_4g:
		{
			pst_config_info->union_config_info.st_lte_4g_module.enum_operate_type = (lte_4g_module_operate_type)b_iot_module_operation;
			pst_config_info->union_config_info.st_lte_4g_module.pfunc_operate_result_notify = network_iot_module_communicate_channel_operate_notify;
			switch(b_iot_module_operation)
			{
				case lte_module_operate_initial:
				case lte_module_operate_reset:
				{
					return TRUE;
				}break;
				case lte_module_operate_ble_config:
				case lte_module_operate_ble_close:
				case lte_module_operate_ble_open_broadcast:
//				case lte_module_operate_close_ap_connection:
				{
					return TRUE;
				}break;
				case lte_4g_module_operate_open_sim_card_connection:
				{

					return TRUE;
				}break;
				case lte_4g_module_operate_get_sim_info:
				{
				    pst_config_info->union_config_info.st_lte_4g_module.union_config_info.st_get_sim_info_connection.pst_data_info =\
                        (lte_module_sim_info *)pv_data;
					return TRUE;
				}
				break;
				case lte_module_operate_reset_tcp_connection:
				case lte_module_operate_open_tcp_connection:
				{
					pst_config_info->union_config_info.st_lte_4g_module.union_config_info.st_open_tcp_connection =\
						sta_network_iot_module_connection_info[connection_type_tbb_server].
						union_iot_module_config_info.
						sta_lte_4g_module_config[connection_operation_open].
						union_config_info.st_open_tcp_connection;
					return TRUE;
				}break;
				case lte_module_operate_close_tcp_connection:
				{
						pst_config_info->union_config_info.st_lte_4g_module.union_config_info.st_close_tcp_connection.b_channel_index =\
						sta_network_iot_module_connection_info[connection_type_tbb_server].
						union_iot_module_config_info.
						sta_lte_4g_module_config[connection_operation_open].
						union_config_info.
						st_open_tcp_connection.
						b_channel_index;
					return TRUE;
				}break;
				case lte_module_operate_send_tcp_user_data:
				{
					pst_config_info->union_config_info.st_lte_4g_module.union_config_info.st_send_tcp_user_data.st_data_info.w_data_len =\
						w_data_len;

					pst_config_info->union_config_info.st_lte_4g_module.union_config_info.st_send_tcp_user_data.st_data_info.pb_data =\
						(uint8_t *)pv_data;
					return TRUE;
				}break;
				case lte_module_operate_receive_tcp_user_data:
				{
					pst_config_info->
						union_config_info.
						st_lte_4g_module.
						union_config_info.
						st_receive_tcp_user_data.
						b_channel_index =\
					sta_network_iot_module_connection_info[connection_type_tbb_server].
						union_iot_module_config_info.
						sta_lte_4g_module_config[connection_operation_open].
						union_config_info.
						st_open_tcp_connection.
						b_channel_index;
					pst_config_info->
						union_config_info.
						st_lte_4g_module.
						union_config_info.
						st_receive_tcp_user_data.
						pst_data_info = (genaral_communicate_data_info *)pv_data;
					return TRUE;
				}break;
				case lte_module_operate_send_ble_user_data:
				{
					pst_config_info->union_config_info.st_lte_4g_module.union_config_info.st_send_ble_user_data.st_data_info.w_data_len =\
						w_data_len;

					pst_config_info->union_config_info.st_lte_4g_module.union_config_info.st_send_ble_user_data.st_data_info.pb_data =\
						(uint8_t *)pv_data;
					return TRUE;
				}break;
			}
		}
		break;
        default:
        {
        }break;
    }

    return FALSE;
}

uint8_t network_iot_module_communicate_channel_operate_initiate(uint8_t b_iot_module_type,
                                                                network_iot_config_info *pst_config_info)
{
    if(NULL == pst_config_info)
    {
        return FALSE;
    }

    switch(b_iot_module_type)
    {
        case iot_module_type_lte:
        {
            return lte_module_driver_user_operate_initiate(&pst_config_info->union_config_info.st_lte_module);
        }break;
        case iot_module_type_ble:
        {
            return FALSE;
        }break;
		case iot_module_type_4g:
        {
            return lte_4g_module_driver_user_operate_initiate(&pst_config_info->union_config_info.st_lte_4g_module);
        }break;
        default:
        {
        }break;
    }

    return FALSE;
}

extern CirQueue st_network_iot_module_data_queue;
extern CirQueue st_lte_module_driver_config_queue;
extern uint8_t iot_send_safe_unlock;
uint8_t network_iot_module_communicate_channel_operate(uint8_t b_iot_module_type,
                                                       uint8_t b_iot_module_operation,
                                                       uint16_t w_data_len,
                                                       void *pv_data)
{
    if(iot_send_safe_unlock == 1)
    {
        return FALSE;
    }

    network_iot_config_info *pst_config_info = 0;

    do{
		pst_config_info = (network_iot_config_info *)pvPortMalloc(sizeof(network_iot_config_info));
        vTaskDelay(0);
	}while(NULL == pst_config_info);
	#if(dynamic_memory_request_test)
	DebugPrintf(TRUE,"network_iot_module_communicate_channel_operate_0_malloc%x\n",pst_config_info);
	#endif
	memset((uint8_t *)pst_config_info,0,sizeof(network_iot_config_info));


    if(FALSE == network_iot_module_communicate_channel_operate_prepare(b_iot_module_type,
                                                                       b_iot_module_operation,
                                                                       w_data_len,
                                                                       pv_data,
                                                                       pst_config_info))
    {
        vPortFree(pst_config_info);
        return FALSE;
    }

    if(FALSE == network_iot_module_communicate_channel_operate_initiate(b_iot_module_type,
                                                                        pst_config_info))
    {
        vPortFree(pst_config_info);
        return FALSE;
    }

    return TRUE;
}

uint8_t network_iot_module_communicate_channel_operate_notify(iot_module_driver_operate_result *pst_operate_result)
{
    if(NULL == pst_operate_result)
    {
        return FALSE;
    }

    iot_module_driver_operate_result *pst_operate_result_internal = 0;

//    for(;;)
//    {
//        pst_operate_result_internal = (iot_module_driver_operate_result *)pvPortMalloc(sizeof(iot_module_driver_operate_result));
//        if(pst_operate_result_internal)
//        {
//            break;
//        }
//        vTaskDelay(0);
//    }
    pst_operate_result_internal = (iot_module_driver_operate_result *)pvPortMallocUser(sizeof(iot_module_driver_operate_result),TRUE);
    #if(dynamic_memory_request_test)
    DebugPrintf(TRUE,"network_iot_module_communicate_channel_operate_notify_0_malloc%x\n",pst_operate_result_internal);
    #endif

    *pst_operate_result_internal = *pst_operate_result;

    return network_iot_module_data_queue_send(pst_operate_result_internal);
}


uint8_t network_response_safe_check(uint8_t iot_type)
{
    // iot_module_driver_operate_result *operate_result = 0;
    uint8_t result = TRUE;

    // if(FALSE == network_iot_module_data_queue_receive((iot_module_driver_operate_result **)&operate_result,FALSE))
    // {
    //     return TRUE;
    // }

    // switch(operate_result->b_iot_module_operation)
    // {
    //     case lte_module_operate_ble_config:
    //     case lte_module_operate_ble_close:
    //     case lte_module_operate_ble_open_broadcast:
    //     case lte_module_operate_send_ble_user_data:
    //     case lte_module_operate_receive_ble_user_data:
    //         result = TRUE;
    //         break;
    //     default:
    //         result = FALSE;
    // }

    // if(iot_type)
    // {
    //     result = !result;
    // }

    return result;
}
uint8_t network_iot_queue_recognition_conversion()
{
    uint8_t b_result = FALSE;

    iot_module_driver_operate_result *pst_operate_result = 0;
    b_result = network_iot_module_data_queue_receive((iot_module_driver_operate_result **)&pst_operate_result,TRUE);
    if (TRUE == b_result)
    {
        if ((NULL == pst_operate_result))
        {
            network_iot_module_data_queue_shift();
            return FALSE;
        }
		switch(pst_operate_result->b_iot_module_operation)
		{
			case lte_module_operate_ble_config:
			case lte_module_operate_ble_close:
			case lte_module_operate_ble_open_broadcast:
			case lte_module_operate_send_ble_user_data:
			case lte_module_operate_receive_ble_user_data:
				network_iot_ble_module_data_queue_send((iot_module_driver_operate_result **)pst_operate_result);
			break;
			default:
//                if(pst_operate_result->b_iot_module_operation == 0)
//                {
////                    yl_debug_printf("[send]pst_operate_result->b_iot_module_operation == 0 \r\n");
//                }
				network_iot_wifi_module_data_queue_send((iot_module_driver_operate_result **)pst_operate_result);
		}
		return TRUE;
    }
}


uint8_t network_iot_module_communicate_channel_operate_response(uint8_t b_iot_module_type,
                                                                uint8_t b_iot_module_operation,
                                                                void **pv_data)
{
    iot_module_driver_operate_result *pst_operate_result = 0;
    uint8_t b_response_result = 0;
	switch(b_iot_module_operation)
	{
		case lte_module_operate_ble_config:
		case lte_module_operate_ble_close:
		case lte_module_operate_ble_open_broadcast:
		case lte_module_operate_send_ble_user_data:
		case lte_module_operate_receive_ble_user_data:
		{
		    if(FALSE == network_iot_ble_module_data_queue_receive((iot_module_driver_operate_result **)&pst_operate_result,TRUE))
		    {
		        return FALSE;
		    }
		}
		break;
		default:
		{
			if(FALSE == network_iot_wifi_module_data_queue_receive((iot_module_driver_operate_result **)&pst_operate_result,TRUE))
		    {
		        return FALSE;
		    }
		}
		break;
	}

    if((b_iot_module_type != pst_operate_result->b_iot_module_type) ||
       (b_iot_module_operation != pst_operate_result->b_iot_module_operation))
    {
        #if( wireless_status_print)
        DebugPrintf(TRUE,"[Status]Wireless operation response vary, in:%d,queue:%d\r\n",
                    b_iot_module_operation,
                    pst_operate_result->b_iot_module_operation);
        #endif
        yl_debug_printf("[Status]Wireless operation response vary, in:%d,queue:%d\r\n",
                                b_iot_module_operation,
                                pst_operate_result->b_iot_module_operation);
        // 打印当前调用线程
        yl_debug_printf("current thread:%s\r\n",pcTaskGetTaskName(NULL));
        return FALSE;
    }

    if((ap_connection_closed == pst_operate_result->b_err_code) ||\
       (ap_connection_auto_connect_success == pst_operate_result->b_err_code))
    {
        network_com_with_tbb_device_server_state_set(cb_net_com_state_type_err_code,net_ap_reconnect);
    }

//    if(ble_connection_closed == pst_operate_result->b_err_code)
//    {
//        network_ble_state_set(net_ble_disconnect);
//    }
//    else if(ble_connection_success == pst_operate_result->b_err_code)
//    {
//        network_ble_state_set(net_ble_connect);
//    }

    switch(b_iot_module_type)
    {
        case iot_module_type_lte:
        {
            switch(b_iot_module_operation)
            {
                case lte_module_operate_initial:
                {
                    if(lte_module_initial_success == pst_operate_result->b_err_code)
                    {
                        b_response_result = 1;
                    }
                    else
                    {
                        b_response_result = 2;
                    }
                    if(pst_operate_result->pv_data)
                    {
                        vPortFree(pst_operate_result->pv_data);
                    }
                    vPortFree(pst_operate_result);
//                    network_iot_module_data_queue_shift();
                }break;
                case lte_module_operate_reset:
                {
                    if(lte_module_reset_success == pst_operate_result->b_err_code)
                    {
                        b_response_result = 1;
                    }
                    else
                    {
                        b_response_result = 2;
                    }
                    if(pst_operate_result->pv_data)
                    {
                        vPortFree(pst_operate_result->pv_data);
                    }
                    vPortFree(pst_operate_result);
//                    network_iot_module_data_queue_shift();
                }break;
                case lte_module_operate_ble_config:
                {
                    if(ble_config_success == pst_operate_result->b_err_code)
                    {
                        b_response_result = 1;
                    }
                    else if(ble_config_fail == pst_operate_result->b_err_code)
                    {
                        b_response_result = 2;
                    }
                    if(pst_operate_result->pv_data)
                    {
                        vPortFree(pst_operate_result->pv_data);
                    }
                    vPortFree(pst_operate_result);
//                    network_iot_module_data_queue_shift();
                }break;
                case lte_module_operate_ble_close:
                {
                    if(ble_close_success == pst_operate_result->b_err_code)
                    {
                        b_response_result = 1;
                    }
                    if(pst_operate_result->pv_data)
                    {
                        vPortFree(pst_operate_result->pv_data);
                    }
                    vPortFree(pst_operate_result);
//                    network_iot_module_data_queue_shift();
                }break;
                case lte_module_operate_ble_open_broadcast:
                {
                    if(ble_broadcast_open_success == pst_operate_result->b_err_code)
                    {
                        b_response_result = 1;
                    }
                    else if(ble_broadcast_open_fail == pst_operate_result->b_err_code)
                    {
                        b_response_result = 2;
                    }
                    if(pst_operate_result->pv_data)
                    {
                        vPortFree(pst_operate_result->pv_data);
                    }
                    vPortFree(pst_operate_result);
//                    network_iot_module_data_queue_shift();
                }break;
                case lte_module_operate_send_ble_user_data:
                {
                    if(ble_send_user_data_success == pst_operate_result->b_err_code)
                    {
                        b_response_result = 1;
                    }
                    else
                    {
                        b_response_result = 2;
                    }
                    if(pst_operate_result->pv_data)
                    {
                        vPortFree(pst_operate_result->pv_data);
                    }
                    vPortFree(pst_operate_result);
//                    network_iot_module_data_queue_shift();
                }break;
                case lte_module_operate_open_ap_connection:
                {
                    if(ap_open_connection_success == pst_operate_result->b_err_code)
                    {
                        display_management_parameter_send(normal_mode,net_led,ap_connect,TRUE);
                        b_response_result = 1;
                    }
                    else if(ap_connect_fail == pst_operate_result->b_err_code)
                    {
                        display_management_parameter_send(normal_mode,net_led,ap_disconnect,TRUE);
                        b_response_result = 2;
                    }
                    else
                    {
                        display_management_parameter_send(normal_mode,net_led,ap_disconnect,TRUE);
                        b_response_result = 3;
                    }
                    if(pst_operate_result->pv_data)
                    {
                        vPortFree(pst_operate_result->pv_data);
                    }
                    vPortFree(pst_operate_result);
//                    network_iot_module_data_queue_shift();
                }break;
                case lte_module_operate_close_ap_connection:
                {
                    if(ap_close_connection_success == pst_operate_result->b_err_code)
                    {
                        b_response_result = 1;
                    }
                    if(pst_operate_result->pv_data)
                    {
                        vPortFree(pst_operate_result->pv_data);
                    }
                    vPortFree(pst_operate_result);
//                    network_iot_module_data_queue_shift();
                }break;
                case lte_module_operate_get_ap_info:
                {
                    if(ap_get_info_success == pst_operate_result->b_err_code)
                    {
                        b_response_result = 1;
                    }
                    else if(ap_get_info_fail == pst_operate_result->b_err_code)
                    {
                        b_response_result = 2;
                    }
                    if(pst_operate_result->pv_data)
                    {
                        vPortFree(pst_operate_result->pv_data);
                    }
                    vPortFree(pst_operate_result);
//                    network_iot_module_data_queue_shift();
                }break;
                case lte_module_operate_reset_tcp_connection:
                {
                    if(tcp_reset_connection_success == pst_operate_result->b_err_code)
                    {
                        b_response_result = 1;
                    }
                    if(pst_operate_result->pv_data)
                    {
                        vPortFree(pst_operate_result->pv_data);
                    }
                    vPortFree(pst_operate_result);

                    #if(dynamic_memory_request_test)
                    DebugPrintf(TRUE,"network_iot_module_communicate_channel_operate_response_1_free%x\n",pst_operate_result);
                    #endif

//                    network_iot_module_data_queue_shift();
                }break;
                case lte_module_operate_open_tcp_connection:
                {
                    if(tcp_open_connection_success == pst_operate_result->b_err_code)
                    {
                        display_management_parameter_send(normal_mode,net_led,sever_connecting,TRUE);
                        b_response_result = 1;
                    }
                    else
                    {
                        b_response_result = 2;
                        network_com_with_tbb_device_server_state_set(cb_net_com_state_type_err_code,net_sever_reconnect);		//感觉应该要改成net_ap_connect
                    }
                    if(pst_operate_result->pv_data)
                    {
                        vPortFree(pst_operate_result->pv_data);
                    }
                    vPortFree(pst_operate_result);

                    #if(dynamic_memory_request_test)
                    DebugPrintf(TRUE,"network_iot_module_communicate_channel_operate_response_1_free%x\n",pst_operate_result);
                    #endif

//                    network_iot_module_data_queue_shift();
                }break;
                case lte_module_operate_close_tcp_connection:
                {
                    if(tcp_close_connection_success == pst_operate_result->b_err_code)
                    {
                        b_response_result = 1;
                    }
                    else
                    {
                        network_com_with_tbb_device_server_state_set(cb_net_com_state_type_err_code,net_ap_connect);
                        b_response_result = 2;
                    }
                    if(pst_operate_result->pv_data)
                    {
                        vPortFree(pst_operate_result->pv_data);
                    }
                    vPortFree(pst_operate_result);

                    #if(dynamic_memory_request_test)
                    DebugPrintf(TRUE,"network_iot_module_communicate_channel_operate_response_1_free%x\n",pst_operate_result);
                    #endif

//                    network_iot_module_data_queue_shift();
                }break;
                case lte_module_operate_send_tcp_user_data:
                {
                    if(tcp_send_user_data_success == pst_operate_result->b_err_code)
                    {
                        b_response_result = 1;
                    }
                    else if((tcp_close_connection_success == pst_operate_result->b_err_code) ||\
                            (tcp_send_user_data_fail == pst_operate_result->b_err_code))
                    {
                        network_com_with_tbb_device_server_state_set(cb_net_com_state_type_err_code,net_sever_reconnect);
                        b_response_result = 2;
                    }
                    if(pst_operate_result->pv_data)
                    {
                        vPortFree(pst_operate_result->pv_data);
                    }
                    vPortFree(pst_operate_result);

                    #if(dynamic_memory_request_test)
                    DebugPrintf(TRUE,"network_iot_module_communicate_channel_operate_response_2_free%x\n",pst_operate_result);
                    #endif

//                    network_iot_module_data_queue_shift();
                }break;
                case lte_module_operate_receive_tcp_user_data:
                {
                    if(tcp_receive_user_data_success == pst_operate_result->b_err_code)
                    {
                        display_management_parameter_send(normal_mode,net_led,sever_com,TRUE);
                    }
                    else if(tcp_connection_closed == pst_operate_result->b_err_code)
                    {
                        network_com_with_tbb_device_server_state_set(cb_net_com_state_type_err_code,net_sever_reconnect);
                    }
                    if(pst_operate_result->pv_data)
                    {
                        vPortFree(pst_operate_result->pv_data);
                    }
                    vPortFree(pst_operate_result);

                    #if(dynamic_memory_request_test)
                    DebugPrintf(TRUE,"network_iot_module_communicate_channel_operate_response_2_free%x\n",pst_operate_result);
                    #endif
                    b_response_result = 1;

//                    network_iot_module_data_queue_shift();
                }break;
            }
        }break;
		case iot_module_type_4g:
		{
            switch(b_iot_module_operation)
            {
                case lte_4g_module_operate_initial:
                {
                    if(lte_module_initial_success == pst_operate_result->b_err_code)
                    {
                        b_response_result = 1;
                    }
                    else
                    {
                        b_response_result = 2;
                    }
                    if(pst_operate_result->pv_data)
                    {
                        vPortFree(pst_operate_result->pv_data);
                    }
                    vPortFree(pst_operate_result);
//                    network_iot_module_data_queue_shift();
                }break;
                case lte_4g_module_operate_reset:
                {
                    if(lte_module_reset_success == pst_operate_result->b_err_code)
                    {
                        b_response_result = 1;
                    }
                    else
                    {
                        b_response_result = 2;
                    }
                    if(pst_operate_result->pv_data)
                    {
                        vPortFree(pst_operate_result->pv_data);
                    }
                    vPortFree(pst_operate_result);
//                    network_iot_module_data_queue_shift();
                }break;
                case lte_4g_module_operate_ble_config:
                {
                    if(ble_config_success == pst_operate_result->b_err_code)
                    {
                        b_response_result = 1;
                    }
                    else if(ble_config_fail == pst_operate_result->b_err_code)
                    {
                        b_response_result = 2;
                    }
                    if(pst_operate_result->pv_data)
                    {
                        vPortFree(pst_operate_result->pv_data);
                    }
                    vPortFree(pst_operate_result);
//                    network_iot_module_data_queue_shift();
                }break;
                case lte_4g_module_operate_ble_close:
                {
                    if(ble_close_success == pst_operate_result->b_err_code)
                    {
                        b_response_result = 1;
                    }
                    if(pst_operate_result->pv_data)
                    {
                        vPortFree(pst_operate_result->pv_data);
                    }
                    vPortFree(pst_operate_result);
//                    network_iot_module_data_queue_shift();
                }break;
                case lte_4g_module_operate_ble_open_broadcast:
                {
                    if(ble_broadcast_open_success == pst_operate_result->b_err_code)
                    {
                        b_response_result = 1;
                    }
                    else if(ble_broadcast_open_fail == pst_operate_result->b_err_code)
                    {
                        b_response_result = 2;
                    }
                    if(pst_operate_result->pv_data)
                    {
                        vPortFree(pst_operate_result->pv_data);
                    }
                    vPortFree(pst_operate_result);
//                    network_iot_module_data_queue_shift();
                }break;
                case lte_4g_module_operate_send_ble_user_data:
                {
                    if(ble_send_user_data_success == pst_operate_result->b_err_code)
                    {
                        b_response_result = 1;
                    }
                    else
                    {
                        b_response_result = 2;
                    }
                    if(pst_operate_result->pv_data)
                    {
                        vPortFree(pst_operate_result->pv_data);
                    }
                    vPortFree(pst_operate_result);
//                    network_iot_module_data_queue_shift();
                }break;
                case lte_4g_module_operate_open_sim_card_connection:
                {
                    if(sim_open_connection_success == pst_operate_result->b_err_code)
                    {
                        display_management_parameter_send(normal_mode,net_led,ap_connect,TRUE);
                        b_response_result = 1;
                    }
                    else if(sim_open_connection_fail == pst_operate_result->b_err_code)
                    {
                        display_management_parameter_send(normal_mode,net_led,ap_disconnect,TRUE);
                        b_response_result = 2;
                    }
                    else
                    {
                        display_management_parameter_send(normal_mode,net_led,ap_disconnect,TRUE);
                        b_response_result = 3;
                    }
                    if(pst_operate_result->pv_data)
                    {
                        vPortFree(pst_operate_result->pv_data);
                    }
                    vPortFree(pst_operate_result);
//                    network_iot_module_data_queue_shift();
                }break;
                case lte_4g_module_operate_close_sim_card_connection:
                {
                    if(ap_close_connection_success == pst_operate_result->b_err_code)
                    {
                        b_response_result = 1;
                    }
                    if(pst_operate_result->pv_data)
                    {
                        vPortFree(pst_operate_result->pv_data);
                    }
                    vPortFree(pst_operate_result);
//                    network_iot_module_data_queue_shift();
                }break;
                case lte_4g_module_operate_get_sim_info:
                {
                    if(sim_get_info_success == pst_operate_result->b_err_code)
                    {
                        b_response_result = 1;
                    }
                    else if(sim_get_info_fail == pst_operate_result->b_err_code)
                    {
                        b_response_result = 2;
                    }
                    if(pst_operate_result->pv_data)
                    {
                        vPortFree(pst_operate_result->pv_data);
                    }
                    vPortFree(pst_operate_result);
//                    network_iot_module_data_queue_shift();
                }break;
                case lte_4g_module_operate_reset_tcp_connection:
                {
                    if(tcp_reset_connection_success == pst_operate_result->b_err_code)
                    {
                        b_response_result = 1;
                    }
                    if(pst_operate_result->pv_data)
                    {
                        vPortFree(pst_operate_result->pv_data);
                    }
                    vPortFree(pst_operate_result);

                    #if(dynamic_memory_request_test)
                    DebugPrintf(TRUE,"network_iot_module_communicate_channel_operate_response_1_free%x\n",pst_operate_result);
                    #endif

//                    network_iot_module_data_queue_shift();
                }break;
                case lte_4g_module_operate_open_tcp_connection:
                {
                    if(tcp_open_connection_success == pst_operate_result->b_err_code)
                    {
                        display_management_parameter_send(normal_mode,net_led,sever_connecting,TRUE);
                        b_response_result = 1;
                    }
                    else
                    {
                        b_response_result = 2;
                        network_com_with_tbb_device_server_state_set(cb_net_com_state_type_err_code,net_sever_reconnect);
                    }
                    if(pst_operate_result->pv_data)
                    {
                        vPortFree(pst_operate_result->pv_data);
                    }
                    vPortFree(pst_operate_result);

                    #if(dynamic_memory_request_test)
                    DebugPrintf(TRUE,"network_iot_module_communicate_channel_operate_response_1_free%x\n",pst_operate_result);
                    #endif

//                    network_iot_module_data_queue_shift();
                }break;
                case lte_4g_module_operate_close_tcp_connection:
                {
                    if(tcp_close_connection_success == pst_operate_result->b_err_code)
                    {
                        b_response_result = 1;
                    }
                    else
                    {
                        network_com_with_tbb_device_server_state_set(cb_net_com_state_type_err_code,net_ap_connect);
                        b_response_result = 2;
                    }
                    if(pst_operate_result->pv_data)
                    {
                        vPortFree(pst_operate_result->pv_data);
                    }
                    vPortFree(pst_operate_result);

                    #if(dynamic_memory_request_test)
                    DebugPrintf(TRUE,"network_iot_module_communicate_channel_operate_response_1_free%x\n",pst_operate_result);
                    #endif

//                    network_iot_module_data_queue_shift();
                }break;
                case lte_4g_module_operate_send_tcp_user_data:
                {
                    if(tcp_send_user_data_success == pst_operate_result->b_err_code)
                    {
                        b_response_result = 1;
                    }
                    else if((tcp_close_connection_success == pst_operate_result->b_err_code) ||\
                            (tcp_send_user_data_fail == pst_operate_result->b_err_code))
                    {
                        network_com_with_tbb_device_server_state_set(cb_net_com_state_type_err_code,net_sever_reconnect);
                        b_response_result = 2;
                    }
                    if(pst_operate_result->pv_data)
                    {
                        vPortFree(pst_operate_result->pv_data);
                    }
                    vPortFree(pst_operate_result);

                    #if(dynamic_memory_request_test)
                    DebugPrintf(TRUE,"network_iot_module_communicate_channel_operate_response_2_free%x\n",pst_operate_result);
                    #endif

//                    network_iot_module_data_queue_shift();
                }break;
                case lte_4g_module_operate_receive_tcp_user_data:
                {
                    if(tcp_receive_user_data_success == pst_operate_result->b_err_code)
                    {
                        display_management_parameter_send(normal_mode,net_led,sever_com,TRUE);
                    }
                    else if(tcp_connection_closed == pst_operate_result->b_err_code)
                    {
                        network_com_with_tbb_device_server_state_set(cb_net_com_state_type_err_code,net_sever_reconnect);
                    }
                    if(pst_operate_result->pv_data)
                    {
                        vPortFree(pst_operate_result->pv_data);
                    }
                    vPortFree(pst_operate_result);

                    #if(dynamic_memory_request_test)
                    DebugPrintf(TRUE,"network_iot_module_communicate_channel_operate_response_2_free%x\n",pst_operate_result);
                    #endif
                    b_response_result = 1;

//                    network_iot_module_data_queue_shift();
                }break;
            }
        }
		default:
        {
//            network_iot_module_data_queue_shift();
        }break;
    }

    return b_response_result;
}

void network_iot_module_communicate_channel_config_excute()
{
#ifdef KINERGY_II_WIFI
    lte_module_driver_user_operate_execution();
#else
    lte_4g_module_driver_user_operate_execution();
#endif

}
/************************************************************************************************************
network commnication
************************************************************************************************************/

//{
//    network_iot_user_data_info *pst_iot_user_data =\
//        (network_iot_user_data_transmit *)pst_network_data_raw->pb_data;
//
//    pst_iot_user_data->b_iot_module_type = iot_module_type_lte;
//    pst_iot_user_data->b_iot_module_operation = lte_module_operate_send_mqtt_user_data;
//    pst_iot_user_data->st_lte_module_message_info.
//
//    while(FALSE == network_iot_module_communicate_channel_operate(pst_iot_user_data))
//    {
//        vTaskDelay(0);
//    }
//}
//
//{
//    network_iot_user_data_info st_iot_user_data =
//    {
//        .b_iot_module_type = iot_module_type_lte,
//        .b_iot_module_operation = lte_module_operate_receive_tcp_user_data,
//    }
//
//    while(TRUE == network_iot_module_communicate_channel_operate(&st_iot_user_data))
//    {
//
//    }
//}

uint8_t lte_module_driver_config_queue_receive(pv_queue_operate_msg pv_queue_data, uint8_t f_shift_queue);
uint8_t network_com_tx_data_queue_receive(pv_queue_operate_msg pv_queue_data, uint8_t f_shift_queue);
uint8_t network_ble_tx_queue_receive(pv_queue_operate_msg pv_queue_data, uint8_t f_shift_queue);
uint8_t network_ble_communication_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue);
uint8_t lte_clear_queue(void)
{
    lte_module_driver_config_info_general *pst_config_info = 0;

    while (
        TRUE
        == lte_module_driver_config_queue_receive(( lte_module_driver_config_info_general ** )&pst_config_info, TRUE))
    {
        vPortFree(pst_config_info);
    }

    iot_module_driver_operate_result *pst_operate_result = 0;

    while (
        TRUE
        == network_iot_ble_module_data_queue_receive(( iot_module_driver_operate_result ** )&pst_operate_result, TRUE))
    {
        if (pst_operate_result->pv_data)
        {
            vPortFree(pst_operate_result->pv_data);
        }
        vPortFree(pst_operate_result);
    }

    while (TRUE
           == network_iot_wifi_module_data_queue_receive(( iot_module_driver_operate_result ** )&pst_operate_result,
                                                         TRUE))
    {
        if (pst_operate_result->pv_data)
        {
            vPortFree(pst_operate_result->pv_data);
        }
        vPortFree(pst_operate_result);
    }

    while (TRUE
           == network_iot_module_data_queue_receive(( iot_module_driver_operate_result ** )&pst_operate_result, TRUE))
    {
        if (pst_operate_result->pv_data)
        {
            vPortFree(pst_operate_result->pv_data);
        }
        vPortFree(pst_operate_result);
    }

    network_data *pst_network_data_raw = 0;
    while (TRUE == network_com_tx_data_queue_receive(&pst_network_data_raw, TRUE))
    {
        if (pst_network_data_raw->st_data_info.pb_data)
        {
            vPortFree(pst_network_data_raw->st_data_info.pb_data);
        }
        vPortFree(pst_network_data_raw);
    }

    while (TRUE == network_ble_tx_queue_receive(&pst_network_data_raw, TRUE))
    {
        if (pst_network_data_raw->st_data_info.pb_data)
        {
            vPortFree(pst_network_data_raw->st_data_info.pb_data);
        }
        vPortFree(pst_network_data_raw);
    }

    genaral_communicate_data_info *pst_network_ble_communication_info = 0;

    while (TRUE
           == network_ble_communication_queue_receive(( pv_queue_operate_msg * )&pst_network_ble_communication_info,
                                                      TRUE))
    {
        vPortFree(pst_network_ble_communication_info->pb_data);
        vPortFree(pst_network_ble_communication_info);
    }

    return TRUE;
}

uint16_t net_ble_establish_ble_init_cnt = 0;
uint8_t net_ble_establish_ble_init_flag = 0;
uint16_t net_ble_open_broadcast_cnt = 0;
uint8_t net_ble_open_broadcast_flag = 0;
uint16_t net_ble_establish_ble_deinit_cnt = 0;
uint8_t net_ble_establish_ble_deinit_flag = 0;
uint16_t net_ble_send_message_cnt = 0;
uint8_t net_ble_send_message_flag = 0;
uint16_t net_tbb_server_communicate_initial_cnt = 0;
uint8_t net_tbb_server_communicate_initial_flag = 0;
uint16_t net_tbb_server_establish_module_reset_cnt = 0;
uint8_t net_tbb_server_establish_module_reset_flag = 0;
uint16_t net_tbb_server_establish_sever_connect_cnt = 0;
uint8_t net_tbb_server_establish_sever_connect_flag = 0;
uint16_t net_tbb_server_establish_ap_connect_cnt = 0;
uint8_t net_tbb_server_establish_ap_connect_flag = 0;
uint16_t net_tbb_server_establish_sever_close_cnt = 0;
uint8_t net_tbb_server_establish_sever_close_flag = 0;
uint16_t net_get_ap_info_cnt = 0;
uint8_t net_get_ap_info_flag = 0;
uint16_t net_tbb_server_establish_ap_disconnect_cnt = 0;
uint8_t net_tbb_server_establish_ap_disconnect_flag = 0;
uint16_t net_tbb_server_establish_sever_reconnect_cnt = 0;
uint8_t net_tbb_server_establish_sever_reconnect_flag = 0;
uint16_t lte_module_user_data_send_cnt = 0;
uint8_t lte_module_user_data_send_flag = 0;
uint16_t lte_module_user_data_receive_cnt = 0;
uint8_t lte_module_user_data_receive_flag = 0;


uint8_t lte_error_cnt(uint16_t *error_cnt, uint16_t cnt_max)
{
    (*error_cnt)++;
    if ((*error_cnt) > cnt_max)
    {
        net_ble_establish_ble_init_cnt = 0;
        net_ble_open_broadcast_cnt = 0;
        net_ble_establish_ble_deinit_cnt = 0;
        net_ble_send_message_cnt = 0;
        net_tbb_server_communicate_initial_cnt = 0;
        net_tbb_server_establish_module_reset_cnt = 0;
        net_tbb_server_establish_sever_connect_cnt = 0;
        net_tbb_server_establish_ap_connect_cnt = 0;
        net_tbb_server_establish_sever_close_cnt = 0;
        net_get_ap_info_cnt = 0;
        net_tbb_server_establish_ap_disconnect_cnt = 0;
        net_tbb_server_establish_sever_reconnect_cnt = 0;
        lte_module_user_data_send_cnt = 0;
        lte_module_user_data_receive_cnt = 0;

        net_ble_establish_ble_init_flag = 1;
        net_ble_open_broadcast_flag = 1;
        net_ble_establish_ble_deinit_flag = 1;
        net_ble_send_message_flag = 1;
        net_tbb_server_communicate_initial_flag = 1;
        net_tbb_server_establish_module_reset_flag = 1;
        net_tbb_server_establish_sever_connect_flag = 1;
        net_tbb_server_establish_ap_connect_flag = 1;
        net_tbb_server_establish_sever_close_flag = 1;
        net_get_ap_info_flag = 1;
        net_tbb_server_establish_ap_disconnect_flag = 1;
        net_tbb_server_establish_sever_reconnect_flag = 1;
        lte_module_user_data_send_flag = 1;
        lte_module_user_data_receive_flag = 1;

        lte_clear_queue();

        extern uint8_t b_tbb_device_ble_com_step;
        extern uint8_t b_net_ble_state;
        extern uint8_t ble_connect_net_disconncet;
        extern uint8_t f_module_init_succeed;
        extern uint8_t auto_disconnect_state;
        extern uint32_t auto_disconnect_cnt;
        extern uint8_t iot_send_safe_unlock;
        extern uint8_t enum_operate_type_last;
        extern uint8_t enum_operate_type_cur;
        extern uint8_t f_lte_module_reset;
        extern uint8_t b_net_ble_state_last;
        extern uint8_t start_one_flag;
        extern uint8_t ble_connect_flag;
        extern volatile uint8_t b_tbb_device_server_com_step;
        extern volatile uint8_t b_tbb_device_server_com_step_next;
        extern uint32_t dw_net_connect_delay_ms;
        extern TickType_t ble_x_start_time;


        b_tbb_device_ble_com_step = 0;
        b_net_ble_state = net_ble_reconnect;
        ble_connect_net_disconncet = 0;
        f_module_init_succeed = false;
        auto_disconnect_state = 0;
        auto_disconnect_cnt = 0;
        iot_send_safe_unlock = 0;
        enum_operate_type_last = 0;
        enum_operate_type_cur = 0;
        f_lte_module_reset = FALSE;
        b_net_ble_state_last = net_ble_disconnect;
        start_one_flag = 0;
        ble_connect_flag = 0;
        b_tbb_device_server_com_step = 0;
        b_tbb_device_server_com_step_next = 0;
        dw_net_connect_delay_ms = 0;
        ble_x_start_time = 0;

        return FALSE;
    }
    return TRUE;
}

uint8_t lte_error_reset(void)
{
    net_ble_establish_ble_init_cnt = 0;
    net_ble_open_broadcast_cnt = 0;
    net_ble_establish_ble_deinit_cnt = 0;
    net_ble_send_message_cnt = 0;
    net_tbb_server_communicate_initial_cnt = 0;
    net_tbb_server_establish_module_reset_cnt = 0;
    net_tbb_server_establish_sever_connect_cnt = 0;
    net_tbb_server_establish_ap_connect_cnt = 0;
    net_tbb_server_establish_sever_close_cnt = 0;
    net_get_ap_info_cnt = 0;
    net_tbb_server_establish_ap_disconnect_cnt = 0;
    net_tbb_server_establish_sever_reconnect_cnt = 0;
    lte_module_user_data_send_cnt = 0;
    lte_module_user_data_receive_cnt = 0;

    net_ble_establish_ble_init_flag = 1;
    net_ble_open_broadcast_flag = 1;
    net_ble_establish_ble_deinit_flag = 1;
    net_ble_send_message_flag = 1;
    net_tbb_server_communicate_initial_flag = 1;
    net_tbb_server_establish_module_reset_flag = 1;
    net_tbb_server_establish_sever_connect_flag = 1;
    net_tbb_server_establish_ap_connect_flag = 1;
    net_tbb_server_establish_sever_close_flag = 1;
    net_get_ap_info_flag = 1;
    net_tbb_server_establish_ap_disconnect_flag = 1;
    net_tbb_server_establish_sever_reconnect_flag = 1;
    lte_module_user_data_send_flag = 1;
    lte_module_user_data_receive_flag = 1;

    lte_clear_queue();

    extern uint8_t b_tbb_device_ble_com_step;
    extern uint8_t b_net_ble_state;
    extern uint8_t ble_connect_net_disconncet;
    extern uint8_t f_module_init_succeed;
    extern uint8_t auto_disconnect_state;
    extern uint32_t auto_disconnect_cnt;
    extern uint8_t iot_send_safe_unlock;
    extern uint8_t enum_operate_type_last;
    extern uint8_t enum_operate_type_cur;
    extern uint8_t f_lte_module_reset;
    extern uint8_t b_net_ble_state_last;
    extern uint8_t start_one_flag;
    extern uint8_t ble_connect_flag;
    extern volatile uint8_t b_tbb_device_server_com_step;
    extern volatile uint8_t b_tbb_device_server_com_step_next;
    extern uint32_t dw_net_connect_delay_ms;
    extern TickType_t ble_x_start_time;

    b_tbb_device_ble_com_step = 0;
    b_net_ble_state = net_ble_reconnect;
    ble_connect_net_disconncet = 0;
    f_module_init_succeed = false;
    auto_disconnect_state = 0;
    auto_disconnect_cnt = 0;
    iot_send_safe_unlock = 0;
    enum_operate_type_last = 0;
    enum_operate_type_cur = 0;
    f_lte_module_reset = FALSE;
    b_net_ble_state_last = net_ble_disconnect;
    start_one_flag = 0;
    ble_connect_flag = 0;
    b_tbb_device_server_com_step = 0;
    b_tbb_device_server_com_step_next = 0;
    dw_net_connect_delay_ms = 0;
    ble_x_start_time = 0;

    return TRUE;
}

uint8_t lte_error_check(uint8_t *error_clear_flag, uint8_t *lte_step)
{
    if (*error_clear_flag == 1)
    {
        *error_clear_flag = 0;
        *lte_step = 0;
    }

    return TRUE;
}