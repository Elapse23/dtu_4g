#include <string.h>
#include <stdlib.h>
#include "lte_module.h"
#include "lte_4g_module.h"
#include "portmacro.h"
#include "FreeRTOS.h"
#include "task.h"
#include "printf_custom.h"
#include "msg_queue_operate.h"
#include "lte_module_data_queue.h"
#include "lte_module_config_queue.h"
#include "net_device_data_access.h"
#include "application_general_func.h"
#include "freertos_user_config.h"
#include "lte_module_com_interface_init.h"
#include "lte_module_stander_at_driver.h"
#include "network_iot_module_data_channel_manage.h"
#include "lte_module_tcp_at_driver.h"
#include "network_protocal_data_assembly.h"
#include "lte_module_config_queue.h"
#include "lte_module_ble_at_driver.h"
#include "lte_4G_module_ble_at_driver.h"



typedef struct
{
    uint8_t f_connect_server;
    uint8_t f_gps_on;
	uint8_t b_lte_module_net_protocal;
}lte_module_status;

//lte_module_status st_lte_module_status;

void lte_4g_module_driver_initial()
{
    lte_4g_module_ctrl_pin_init();
}

uint8_t lte_4g_module_driver_user_operate_initiate(lte_4g_module_driver_config_info_general *pst_config_info)
{
    if(NULL == pst_config_info)
    {
        return FALSE;
    }

    if(pst_config_info->enum_operate_type >= lte_4g_module_operate_type_number)
    {
        return FALSE;
    }

    return lte_module_driver_config_queue_send(pst_config_info);
}

uint8_t lte_4g_module_driver_user_operate_execution()
{
    static uint8_t f_lte_module_reset = FALSE;

    if(FALSE == f_lte_module_reset)
    {
//        lte_4g_module_hardware_reset();
		lte_4g_module_hardware_open();

        f_lte_module_reset = TRUE;
    }

    uint8_t b_result = FALSE;
    lte_4g_module_driver_config_info_general *pst_config_info = 0;

    if(FALSE == lte_module_driver_config_queue_receive((lte_4g_module_driver_config_info_general **)&pst_config_info,TRUE))
    {
        return FALSE;
    }

    if(0 == pst_config_info)
    {
        return FALSE;
    }

    switch(pst_config_info->enum_operate_type)
    {
        case lte_4g_module_operate_initial:
        {
            b_result = lte_4g_module_initial(pst_config_info);
        }break;
        case lte_4g_module_operate_reset:
        {
            b_result = lte_4g_module_reset(pst_config_info);
        }break;
        case lte_4g_module_operate_open_sim_card_connection:
        {
            b_result = lte_4g_module_open_sim_card_connect(pst_config_info);
        }break;
//        case lte_4g_module_operate_close_sim_card_connection:
//        {
//            b_result = lte_4g_module_close_sim_card_connect(pst_config_info);
//        }break;
        case lte_4g_module_operate_ble_config:
        {
            b_result = lte_4g_module_ble_config(pst_config_info);
        }break;
        case lte_4g_module_operate_ble_open_broadcast:
        {
            b_result = lte_4g_module_open_broadcast(pst_config_info);
        }break;
        case lte_4g_module_operate_send_ble_user_data:
        {
            b_result = lte_4g_module_driver_send_ble_user_data(pst_config_info);
        }break;
        case lte_4g_module_operate_ble_close:
        {
            b_result = lte_4g_module_ble_deinit(pst_config_info);
        }break;
//        case lte_module_operate_restart:
//        {
//            b_result = lte_module_driver_restart(pst_config_info);
//        }break;
        case lte_4g_module_operate_reset_tcp_connection:
        {
            b_result = lte_4g_module_driver_reset_tcp_connection(pst_config_info);
        }break;
        case lte_4g_module_operate_open_tcp_connection:
        {
            b_result = lte_4g_module_driver_open_tcp_connection(pst_config_info);
        }break;
        case lte_4g_module_operate_close_tcp_connection:
        {
            b_result = lte_4g_module_driver_close_tcp_connection(pst_config_info);
        }break;
        case lte_4g_module_operate_send_tcp_user_data:
        {
             b_result = lte_4g_module_driver_send_tcp_user_data(pst_config_info);
        }break;
        case lte_4g_module_operate_receive_tcp_user_data:
        {
            b_result = lte_4g_module_driver_receive_tcp_user_data(pst_config_info);
        }break;
        case lte_4g_module_operate_get_sim_info:
        {
            b_result = lte_4g_module_get_sim_info(pst_config_info);
        }break;
        default:
        {
        }break;
    }

    vPortFree(pst_config_info);

    return b_result;
}

uint8_t lte_4g_module_user_data_send(void *pv_data)
{
    #define cb_tcp_user_send_initiate                    0
    #define cb_tcp_user_send_comfirm                     1
    #define cb_max_net_send_user_time_out                16000

    static uint8_t b_tcp_user_send_step = cb_tcp_user_send_initiate;
    network_data *pst_network_data = 0;

    pst_network_data = (network_data *)pv_data;
    switch(b_tcp_user_send_step)
    {
        case cb_tcp_user_send_initiate:
        {
            if(TRUE == network_iot_module_communicate_channel_operate(iot_module_type_4g,
                                                                      lte_4g_module_operate_send_tcp_user_data,
                                                                      pst_network_data->st_data_info.w_data_len,
                                                                      (uint8_t *)pst_network_data->st_data_info.pb_data))
            {
                b_tcp_user_send_step = cb_tcp_user_send_comfirm;
            }
        }break;
        case cb_tcp_user_send_comfirm:
        {
            uint8_t b_result = 0;

            b_result = network_iot_module_communicate_channel_operate_response(iot_module_type_4g,
                                                                               lte_4g_module_operate_send_tcp_user_data,
                                                                               0);
            if(TRUE == b_result)
            {
                b_tcp_user_send_step = cb_tcp_user_send_initiate;
                return TRUE;
            }
            else if(2 == b_result)
            {
                b_tcp_user_send_step = cb_tcp_user_send_initiate;
                return FALSE;
            }
        }break;
    }

    return FALSE;
}

uint8_t lte_4g_module_user_data_receive(void *pv_data)
{
    #define cb_tcp_user_receive_initiate                    0
    #define cb_tcp_user_receive_comfirm                     1
    #define cb_tcp_user_receive_complete                    2

    static uint8_t b_tcp_user_receive_step = cb_tcp_user_receive_initiate;
    network_data *pst_network_data = 0;

    pst_network_data = (network_data *)pv_data;
    switch(b_tcp_user_receive_step)
    {
        case cb_tcp_user_receive_initiate:
        {
            if(TRUE == network_iot_module_communicate_channel_operate(iot_module_type_4g,
                                                                      lte_4g_module_operate_receive_tcp_user_data,
                                                                      0,
                                                                      (void *)&pst_network_data->st_data_info))
            {
                b_tcp_user_receive_step = cb_tcp_user_receive_comfirm;
            }
        }break;
        case cb_tcp_user_receive_comfirm:
        {
            if(TRUE == network_iot_module_communicate_channel_operate_response(iot_module_type_4g,
                                                                               lte_4g_module_operate_receive_tcp_user_data,
                                                                               0))
            {
                b_tcp_user_receive_step = cb_tcp_user_receive_initiate;
                return cb_tcp_user_receive_complete;
            }
        }break;
    }

    return b_tcp_user_receive_step;
}

//uint8_t lte_module_abnormal_state_at_driver()
//{
//    return TRUE;
//}
