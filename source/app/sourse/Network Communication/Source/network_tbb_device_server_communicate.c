#include "network_tbb_device_server_communicate.h"
#include "application_general_macro.h"
#include "iot_module_config_common.h"
#include "lte_module_config_common.h"
#include "net_system_info_list.h"
#include "network_iot_module_data_channel_manage.h"
#include "portmacro.h"
#include "freertos_user_config.h"
#include "network_local_secquence_generate.h"
#include "network_protocal_data_assembly.h"
#include "network_protocal_cmd_macro.h"
#include "network_protocal.h"
#include "lte_module_data_queue.h"
#include "net_device_data_access.h"
#include <string.h>
#include "lte_module_nmea_info_process.h"
#include "display_management.h"
#include "network_offline_data_operate.h"
#include "network_system_type_operate.h"
#include "network_communication_queue.h"
#include "network_local_config.h"
#include "network_ble_communication.h"
#include "net_firmware_update.h"

#include "extern_flash_operate.h"
#include "spi_flash_operate.h"
#include "product_test.h"
#include "printf_custom.h"



typedef enum
{
    net_com_succeed,
    net_com_wait,
    net_com_timeout,
    net_com_fail,
}net_com_result;

typedef struct
{
    uint8_t f_modify;
    uint8_t b_state;
    uint8_t f_sever_reconnect_enable;
    uint8_t f_ap_connect;
    uint8_t f_sever_connect;
}net_com_state_managemet;

static net_com_state_managemet st_net_com_state_managemet = {0,};
static uint8_t b_net_connect_ip_cnt = 0;

uint8_t net_tbb_server_communicate_initial()
{
    #define cb_tbb_device_server_iot_module_initial               0
    #define cb_tbb_device_server_iot_module_initial_wait          1
    static uint8_t b_tbb_device_server_connect_step = cb_tbb_device_server_iot_module_initial;
    lte_error_check(&net_tbb_server_communicate_initial_flag,&b_tbb_device_server_connect_step);
    switch(b_tbb_device_server_connect_step)
    {
        case cb_tbb_device_server_iot_module_initial:
        {
            net_tbb_server_communicate_initial_cnt = 0;
            if(TRUE == network_iot_module_communicate_channel_operate(iot_module_type_lte,
                                                                      lte_module_operate_initial,
                                                                      0,
                                                                      0))
            {
                b_tbb_device_server_connect_step = cb_tbb_device_server_iot_module_initial_wait;
            }
        }break;
        case cb_tbb_device_server_iot_module_initial_wait:
        {
            if(FALSE == lte_error_cnt(&net_tbb_server_communicate_initial_cnt,default_lte_error_cnt_max))
            {
                yl_debug_printf("net_tbb_server_communicate_initial_cnt error\n");
                return TRUE;
            }
            uint8_t b_result = 0;

            b_result = network_iot_module_communicate_channel_operate_response(iot_module_type_lte,
                                                                               lte_module_operate_initial,
                                                                               0);
            if(1 == b_result)
            {
                b_tbb_device_server_connect_step = cb_tbb_device_server_iot_module_initial;
                return net_com_succeed;
            }
            else if(2 == b_result)
            {
                b_tbb_device_server_connect_step = cb_tbb_device_server_iot_module_initial;
            }
        }break;
    }

    return net_com_wait;
}
uint8_t net_tbb_server_establish_module_reset()
{
    #define cb_tbb_device_server_iot_module_reset               0
    #define cb_tbb_device_server_iot_module_reset_wait          1
    static uint8_t b_tbb_device_server_connect_step = cb_tbb_device_server_iot_module_reset;
    lte_error_check(&net_tbb_server_establish_module_reset_flag,&b_tbb_device_server_connect_step);
    switch(b_tbb_device_server_connect_step)
    {
        case cb_tbb_device_server_iot_module_reset:
        {
            net_tbb_server_establish_module_reset_cnt = 0;
            if(TRUE == network_iot_module_communicate_channel_operate(iot_module_type_lte,
                                                                      lte_module_operate_reset,
                                                                      0,
                                                                      0))
            {
                b_tbb_device_server_connect_step = cb_tbb_device_server_iot_module_reset_wait;
            }
        }break;
        case cb_tbb_device_server_iot_module_reset_wait:
        {
            if(FALSE == lte_error_cnt(&net_tbb_server_establish_module_reset_cnt,default_lte_error_cnt_max))
            {
                yl_debug_printf("net_tbb_server_establish_module_reset_cnt error\n");
                return TRUE;
            }
            uint8_t b_result = 0;

            b_result = network_iot_module_communicate_channel_operate_response(iot_module_type_lte,
                                                                               lte_module_operate_reset,
                                                                               0);
            if(1 == b_result)
            {
                b_tbb_device_server_connect_step = cb_tbb_device_server_iot_module_reset;
                return net_com_succeed;
            }
            else if(2 == b_result)
            {
                b_tbb_device_server_connect_step = cb_tbb_device_server_iot_module_reset;
                return net_com_fail;
            }
        }break;
    }

    return net_com_wait;
}
uint8_t net_tbb_server_establish_sever_connect()
{
    #define cb_tbb_device_server_initiate_connect                   0
    #define cb_tbb_device_server_confirm_connect                    1
    static uint8_t b_tbb_device_server_connect_step = cb_tbb_device_server_initiate_connect;
    static uint8_t b_err_cnt = 0;
    lte_error_check(&net_tbb_server_establish_sever_connect_flag,&b_tbb_device_server_connect_step);
    switch(b_tbb_device_server_connect_step)
    {
        case cb_tbb_device_server_initiate_connect:
        {
            net_tbb_server_establish_sever_connect_cnt = 0;
            if(TRUE == network_iot_module_communicate_channel_operate(iot_module_type_lte,
                                                                      lte_module_operate_open_tcp_connection,
                                                                      0,
                                                                      0))
            {
                b_err_cnt++;
                b_tbb_device_server_connect_step = cb_tbb_device_server_confirm_connect;
            }
        }break;
        case cb_tbb_device_server_confirm_connect:
        {
            if(FALSE == lte_error_cnt(&net_tbb_server_establish_sever_connect_cnt,default_lte_error_cnt_max))
            {
                yl_debug_printf("net_tbb_server_establish_sever_connect_cnt error\n");
                return TRUE;
            }
            uint8_t b_result = 0;
            b_result = network_iot_module_communicate_channel_operate_response(iot_module_type_lte,
                                                                               lte_module_operate_open_tcp_connection,
                                                                               0);
            if(1 == b_result)
            {
                b_err_cnt = 0;
                network_com_with_tbb_device_server_state_set(cb_net_com_state_type_sever,TRUE);
                b_tbb_device_server_connect_step = cb_tbb_device_server_initiate_connect;
                return net_com_succeed;
            }
            else if(2 == b_result)
            {
                if(b_err_cnt > 3)
                {
                    network_com_with_tbb_device_server_state_set(cb_net_com_state_type_err_code,net_ap_reconnect);
                }
                b_tbb_device_server_connect_step = cb_tbb_device_server_initiate_connect;
                return net_com_timeout;
            }

        }break;
    }

    return net_com_wait;
}
uint8_t net_tbb_server_establish_ap_connect()
{
    #define cb_tbb_device_server_initiate_ap_connect                   0
    #define cb_tbb_device_server_confirm_ap_connect                    1
    static uint8_t b_tbb_device_server_connect_step = cb_tbb_device_server_initiate_ap_connect;
    lte_error_check(&net_tbb_server_establish_ap_connect_flag,&b_tbb_device_server_connect_step);

    switch(b_tbb_device_server_connect_step)
    {
        case cb_tbb_device_server_initiate_ap_connect:
        {
            net_tbb_server_establish_ap_connect_cnt = 0;
            if(TRUE == network_iot_module_communicate_channel_operate(iot_module_type_lte,
                                                                      lte_module_operate_open_ap_connection,
                                                                      0,
                                                                      0))
            {
                b_tbb_device_server_connect_step = cb_tbb_device_server_confirm_ap_connect;
            }
        }break;
        case cb_tbb_device_server_confirm_ap_connect:
        {
            if(FALSE == lte_error_cnt(&net_tbb_server_establish_ap_connect_cnt,default_lte_error_cnt_max))
            {
                yl_debug_printf("net_tbb_server_establish_ap_connect_cnt error\n");
                return TRUE;
            }
            uint8_t b_result = 0;
            b_result = network_iot_module_communicate_channel_operate_response(iot_module_type_lte,
                                                                               lte_module_operate_open_ap_connection,
                                                                               0);
            if(1 == b_result)
            {
                network_com_with_tbb_device_server_state_set(cb_net_com_state_type_ap,cb_ap_connect_normal);
                b_tbb_device_server_connect_step = cb_tbb_device_server_initiate_ap_connect;
                return net_com_succeed;
            }
            else if(2 == b_result)
            {
                b_tbb_device_server_connect_step = cb_tbb_device_server_initiate_ap_connect;
                return net_com_timeout;
            }
            else if(3 == b_result)
            {
                b_tbb_device_server_connect_step = cb_tbb_device_server_initiate_ap_connect;
                return net_com_fail;
            }

        }break;
    }

    return net_com_wait;
}
void net_connect_ip_cnt_set(uint8_t f_cnt_add)
{
    if(TRUE == f_cnt_add)
    {
        b_net_connect_ip_cnt++;
    }
    else
    {
        b_net_connect_ip_cnt = 0;
    }
}
uint8_t net_connect_ip_cnt_get()
{
    return b_net_connect_ip_cnt;
}
uint8_t net_tbb_server_establish_sever_close()
{
    #define cb_tbb_device_server_initiate_close                   0
    #define cb_tbb_device_server_confirm_close                    1
    static uint8_t b_tbb_device_server_close_step = cb_tbb_device_server_initiate_close;
    lte_error_check(&net_tbb_server_establish_sever_close_flag,&b_tbb_device_server_close_step);
    switch(b_tbb_device_server_close_step)
    {
        case cb_tbb_device_server_initiate_close:
        {
            net_tbb_server_establish_sever_close_cnt = 0;
            if(TRUE == network_iot_module_communicate_channel_operate(iot_module_type_lte,
                                                                      lte_module_operate_close_tcp_connection,
                                                                      0,
                                                                      0))
            {
                b_tbb_device_server_close_step = cb_tbb_device_server_confirm_close;
            }
        }break;
        case cb_tbb_device_server_confirm_close:
        {
            if(FALSE == lte_error_cnt(&net_tbb_server_establish_sever_close_cnt,default_lte_error_cnt_max))
            {
                yl_debug_printf("net_tbb_server_establish_sever_close_cnt error\n");
                return TRUE;
            }
            uint8_t b_result = 0;
            b_result = network_iot_module_communicate_channel_operate_response(iot_module_type_lte,
                                                                               lte_module_operate_close_tcp_connection,
                                                                               0);
            if(1 == b_result)
            {
                b_tbb_device_server_close_step = cb_tbb_device_server_initiate_close;
                return net_com_succeed;
            }
            else if(2 == b_result)
            {
                b_tbb_device_server_close_step = cb_tbb_device_server_initiate_close;
                return net_com_timeout;
            }

        }break;
    }

    return net_com_wait;
}
uint8_t net_get_ap_info()
{
    #define cb_get_ap_info_initiate                    0
    #define cb_get_ap_info_comfirm                     1
    #define cb_get_ap_info_delay                       2
    #define cb_get_ap_info_none                        3

    static uint8_t b_get_ap_info_step = cb_get_ap_info_initiate;
    lte_module_ap_info st_ap_info = {0,};

    lte_error_check(&net_get_ap_info_flag,&b_get_ap_info_step);

    if(0 != st_device_update_info_current.pst_net_device_update_info_current)
    {
        return TRUE;
    }
    do{
        switch(b_get_ap_info_step)
        {
            case cb_get_ap_info_initiate:
            {
                net_get_ap_info_cnt = 0;
                if(TRUE == network_iot_module_communicate_channel_operate(iot_module_type_lte,
                                                                          lte_module_operate_get_ap_info,
                                                                          0,
                                                                          &st_ap_info))
                {
                    b_get_ap_info_step = cb_get_ap_info_comfirm;
                }
            }break;
            case cb_get_ap_info_comfirm:
            {
                if(FALSE == lte_error_cnt(&net_get_ap_info_cnt,much_lte_error_cnt_max))
                {
                    yl_debug_printf("net_get_ap_info_cnt error\n");
                    return TRUE;
                }
                uint8_t b_result = 0;
                uint16_t w_data_len = 1;
                b_result = network_iot_module_communicate_channel_operate_response(iot_module_type_lte,
                                                                                   lte_module_operate_get_ap_info,
                                                                                   0);
                if(TRUE == b_result)
                {
                    b_get_ap_info_step = cb_get_ap_info_delay;

                    net_system_data_access(cb_device_data_access_data_type_status,
                                   0x0106,
                                   &w_data_len,
                                   (uint8_t *)&st_ap_info.b_signal_pct,
                                   cb_device_data_access_operate_type_write);
                }
                else if(2 == b_result)
                {
                    b_get_ap_info_step = cb_get_ap_info_delay;

                }
            }break;
            case cb_get_ap_info_delay:
            {
                static uint8_t f_init = TRUE;
                static TickType_t x_start_time = 0;
                TickType_t x_curent_time = 0;

                if(f_init)
                {
                    f_init = FALSE;
                    x_start_time = xTaskGetTickCount();
                }
                x_curent_time = xTaskGetTickCount();

                if((x_curent_time - x_start_time) >= pdMS_TO_TICKS_USER(60000))
                {
                    f_init = TRUE;
                    b_get_ap_info_step = cb_get_ap_info_initiate;
                }
                else
                {
                    return TRUE;
                }
            }break;
        }
        vTaskDelay(1);
    }while(cb_get_ap_info_none != b_get_ap_info_step);

    return FALSE;
}
uint8_t net_tbb_server_establish_ap_disconnect()
{
    #define cb_tbb_device_server_initiate_ap_close                   0
    #define cb_tbb_device_server_confirm_ap_close                    1
    static uint8_t b_tbb_device_server_close_step = cb_tbb_device_server_initiate_ap_close;
    lte_error_check(&net_tbb_server_establish_ap_disconnect_flag,&b_tbb_device_server_close_step);
    switch(b_tbb_device_server_close_step)
    {
        case cb_tbb_device_server_initiate_ap_close:
        {
            net_tbb_server_establish_ap_disconnect_cnt = 0;
            if(TRUE == network_iot_module_communicate_channel_operate(iot_module_type_lte,
                                                                      lte_module_operate_close_ap_connection,
                                                                      0,
                                                                      0))
            {
                b_tbb_device_server_close_step = cb_tbb_device_server_confirm_ap_close;
            }
        }break;
        case cb_tbb_device_server_confirm_ap_close:
        {
            if(FALSE == lte_error_cnt(&net_tbb_server_establish_ap_disconnect_cnt,default_lte_error_cnt_max))
            {
                yl_debug_printf("net_tbb_server_establish_ap_disconnect_cnt error\n");
                return TRUE;
            }
            uint8_t b_result = 0;
            b_result = network_iot_module_communicate_channel_operate_response(iot_module_type_lte,
                                                                               lte_module_operate_close_ap_connection,
                                                                               0);
            if(1 == b_result)
            {
                b_tbb_device_server_close_step = cb_tbb_device_server_initiate_ap_close;
                return net_com_succeed;
            }
            else if(2 == b_result)
            {
                b_tbb_device_server_close_step = cb_tbb_device_server_initiate_ap_close;
                return net_com_timeout;
            }

        }break;
    }

    return net_com_wait;
}
uint8_t net_tbb_server_establish_sever_reconnect()
{
    #define cb_tbb_device_server_initiate_reconnect                   0
    #define cb_tbb_device_server_confirm_reconnect                    1
    static uint8_t b_tbb_device_server_connect_step = cb_tbb_device_server_initiate_reconnect;
    lte_error_check(&net_tbb_server_establish_sever_reconnect_flag,&b_tbb_device_server_connect_step);
    switch(b_tbb_device_server_connect_step)
    {
        case cb_tbb_device_server_initiate_reconnect:
        {
            net_tbb_server_establish_sever_reconnect_cnt = 0;
            if(TRUE == network_iot_module_communicate_channel_operate(iot_module_type_lte,
                                                                      lte_module_operate_reset_tcp_connection,
                                                                      0,
                                                                      0))
            {
                b_tbb_device_server_connect_step = cb_tbb_device_server_confirm_reconnect;
            }
        }break;
        case cb_tbb_device_server_confirm_connect:
        {
            if(FALSE == lte_error_cnt(&net_tbb_server_establish_sever_reconnect_cnt,default_lte_error_cnt_max))
            {
                yl_debug_printf("net_tbb_server_establish_sever_reconnect_cnt error\n");
                return TRUE;
            }
            if(TRUE == network_iot_module_communicate_channel_operate_response(iot_module_type_lte,
                                                                               lte_module_operate_reset_tcp_connection,
                                                                               0))
            {
                b_tbb_device_server_connect_step = cb_tbb_device_server_initiate_reconnect;
                return net_com_succeed;
            }
        }break;
    }

    return net_com_wait;
}
uint8_t net_tbb_device_server_send_message(uint8_t b_net_data_result)
{
    uint8_t b_result = 0;

    b_result = network_communication_with_server(b_net_data_result);

    if(6 != b_result)
    {
        if((net_receive_success == b_net_data_result) ||\
           (net_sever_reconnect == b_net_data_result) ||\
           (net_sever_connect == b_net_data_result))
        {
            network_com_with_tbb_device_server_state_set(cb_net_com_state_type_err_code,net_receive_wait);
        }
        else
        {
            network_com_with_tbb_device_server_state_set(cb_net_com_state_type_err_code,net_receive_none);
        }
    }
    network_data_send(network_com_tx_data_queue_receive);
    return b_result;
}
uint8_t net_tbb_device_server_receive_message()
{
    if(NULL == iot_module_receive_data)
    {
        return net_receive_none;
    }
    network_data *pst_network_data = 0;
    uint8_t b_receive_result = 0;
    uint8_t b_result = net_receive_none;

    pst_network_data = (network_data *)pvPortMallocUser(sizeof(network_data),TRUE);
    pst_network_data->st_data_info.pb_data = (uint8_t *)pvPortMallocUser(cb_max_network_data_len * sizeof(uint8_t),TRUE);


    do{
        b_receive_result = iot_module_receive_data((void *)(pst_network_data));		//lte_module_user_data_receive
        vTaskDelay(1);
    }while(b_receive_result != 2);
    network_data_receive((void *)pst_network_data,cb_channel_type_wireless_module);
    network_inquire_cmd_analysis();
    network_config_cmd_analysis();
    network_other_cmd_analysis();

    b_result = net_data_response();
    network_com_with_tbb_device_server_state_set(cb_net_com_state_type_err_code,b_result);

    vPortFree(pst_network_data->st_data_info.pb_data);
    vPortFree(pst_network_data);

    return b_result;
}

//extern net_com_state_managemet st_net_com_state_managemet;
//
//void network_com_with_tbb_device_server_state_set(uint8_t b_state_type,
//                                                  uint8_t b_state)
//{
//    switch(b_state_type)
//    {
//        case cb_net_com_state_type_sever:
//        {
//            st_net_com_state_managemet.f_sever_connect = b_state;
//            {
//                uint16_t w_data_len = 1;
//                net_system_data_access(cb_device_data_access_data_type_status,
//                                       0x0105,
//                                       &w_data_len,
//                                       (uint8_t *)&st_net_com_state_managemet.f_sever_connect,
//                                       cb_device_data_access_operate_type_write);
//            }
//        }
//        break;
//        case cb_net_com_state_type_ap:
//        {
//            network_ap_connect_err_code_set(b_state);
//        }
//        break;
//        case cb_net_com_state_type_err_code:
//        {
//            if(net_sever_connect == b_state)
//            {
//                st_net_com_state_managemet.f_sever_reconnect_enable = TRUE;
//            }
//            else
//            {
//                st_net_com_state_managemet.f_modify = TRUE;
//                st_net_com_state_managemet.b_state = b_state;
//            }
//        }
//        break;
//        default:
//            break;
//    }
//}
//uint8_t network_com_with_tbb_device_server_state_get()
//{
//    return st_net_com_state_managemet.b_state;
//}

uint8_t network_tbb_sever_com_delay(uint32_t dw_dalay_time_ms)
{
    static uint8_t f_init = TRUE;
    static TickType_t x_start_time = 0;
    TickType_t x_curent_time = 0;

    if(f_init)
    {
        f_init = FALSE;
        x_start_time = xTaskGetTickCount();
    }
    x_curent_time = xTaskGetTickCount();

    if((x_curent_time - x_start_time) >= pdMS_TO_TICKS_USER(dw_dalay_time_ms))
    {
        f_init = TRUE;
        return TRUE;
    }
    return FALSE;
}

extern CirQueue st_network_iot_module_data_queue;
extern CirQueue st_lte_module_driver_config_queue;
uint8_t network_step_safe_switch(uint8_t* step, uint8_t next_step)
{
    // if (TRUE == sIfCirQueueEmpty(&st_network_iot_module_data_queue)
    //     && TRUE == sIfCirQueueEmpty(&st_lte_module_driver_config_queue))
    // {
    //     *step = next_step;
    //     return TRUE;
    // }

    // return FALSE;
    *step = next_step;
    return TRUE;
}
//extern CirQueue st_network_iot_wifi_module_data_queue;

uint8_t network_step_safe_switch_enable(uint8_t* step, uint8_t next_step)
{
     if (TRUE == sIfCirQueueEmpty(&st_network_iot_module_data_queue)
         && TRUE == sIfCirQueueEmpty(&st_lte_module_driver_config_queue))
//         && TRUE == sIfCirQueueEmpty(&st_network_iot_wifi_module_data_queue))
     {
         *step = next_step;
         return TRUE;
     }

     return FALSE;
}



/**
 * @brief 与TBB服务器通信主函数 在 network_communicate_task 中轮询调用
 * @return {uint8_t}
 * @author Yao YiLiang
 */
extern uint8_t b_net_ble_state;
uint8_t b_net_ble_state_last = net_ble_disconnect;
uint8_t start_one_flag = 0;
extern uint8_t ble_connect_net_disconncet;
uint8_t network_response_safe_check(uint8_t iot_type);
extern uint8_t iot_send_safe_unlock;
extern uint8_t ble_connect_flag;
volatile uint8_t b_tbb_device_server_com_step = 0;
volatile uint8_t b_tbb_device_server_com_step_next = 0;
uint32_t dw_net_connect_delay_ms = 0;
#pragma optimize=none
uint8_t network_com_with_tbb_device_server()
{
    #define cb_tbb_device_server_com_initial                                0
    #define cb_startup_delay                                                1
    #define cb_connect_with_ap                                              2
    #define cb_disconnect_with_ap                                           3
    #define cb_connect_with_tbb_device_server                               4
    #define cb_disconnect_with_tbb_device_server                            5
    #define cb_reset_wireless_module                                        6
    #define cb_tbb_device_server_send_message                               7
    #define cb_tbb_device_server_receive_message                            8
    #define cb_reconncet_delay                                              9

    if(FALSE == network_response_safe_check(1) || iot_send_safe_unlock == 1)
    {
        return FALSE;
    }
    uint8_t b_result = net_com_wait;
    if((cb_tbb_device_server_com_initial != b_tbb_device_server_com_step) 
		&&(TRUE == st_net_com_state_managemet.f_modify))
    {
        uint8_t b_net_com_state = network_com_with_tbb_device_server_state_get();
        st_net_com_state_managemet.f_modify = FALSE;
        if(net_sever_reconnect == b_net_com_state)
        {
            network_com_with_tbb_device_server_state_set(cb_net_com_state_type_sever,FALSE);
            enable_save_offline_data(TRUE);
            display_management_parameter_send(normal_mode,net_led,sever_disconnect,TRUE);
			b_tbb_device_server_com_step = cb_disconnect_with_tbb_device_server;
        }
        else if(net_ap_reconnect == b_net_com_state)
        {
            network_com_with_tbb_device_server_state_set(cb_net_com_state_type_ap,cb_ap_connect_null);
            network_com_with_tbb_device_server_state_set(cb_net_com_state_type_sever,FALSE);
            enable_save_offline_data(TRUE);
			b_tbb_device_server_com_step = cb_disconnect_with_ap;
            display_management_parameter_send(normal_mode,net_led,ap_disconnect,TRUE);
        }
        else if(net_ap_connect == b_net_com_state)
        {
			b_tbb_device_server_com_step = cb_connect_with_ap;
        }
    }
    else if(st_net_com_state_managemet.f_sever_reconnect_enable)
    {
        st_net_com_state_managemet.f_sever_reconnect_enable = FALSE;
        st_net_com_state_managemet.b_state = net_sever_connect;
        if((cb_tbb_device_server_receive_message == b_tbb_device_server_com_step) 
			||(st_net_com_state_managemet.f_sever_connect 
			&&(cb_reconncet_delay == b_tbb_device_server_com_step)))
        {
        	b_tbb_device_server_com_step = cb_tbb_device_server_send_message;
        }
    }
	
	offline_data_save();
    switch(b_tbb_device_server_com_step)
    {
        case cb_tbb_device_server_com_initial:
        {
            b_result = net_tbb_server_communicate_initial();

            if(net_com_succeed == b_result)
            {
                set_module_initialize_state(TRUE);
                display_management_parameter_send(normal_mode,net_led,ap_disconnect,TRUE);
				b_tbb_device_server_com_step = cb_startup_delay;
            }
        }break;
        case cb_startup_delay:
        {
            vTaskDelay(1000);
            uint8_t b_result = FALSE;

            b_result = check_product_test();
            if((FALSE != check_system_init(cb_system_init_type_device_com)) ||\
               (TRUE == b_result))
            {
                if(TRUE == b_result)
                {
                    dw_net_connect_delay_ms = 0;
                }
				dw_net_connect_delay_ms = 30 * 1000;
				{

					uint16_t w_data_len = 2;
					g_real_data_send_delay_time = 20 * 1000;
					net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
										   cb_net_data_att_set,
										   0x0043,
										   &w_data_len,
										   (uint8_t *)&g_real_data_send_delay_time,
										   cb_device_data_access_operate_type_write);

				}
                if(FALSE == check_system_init(cb_system_init_type_network))
                {
                    network_system_init();
                    dw_net_connect_delay_ms = 1000;
                }
                else
                {
                    if(TRUE == network_tbb_sever_com_delay(dw_net_connect_delay_ms))
                    {
						b_tbb_device_server_com_step = cb_connect_with_ap;
                        dw_net_connect_delay_ms = 15000;
                        system_init_comfirm(cb_system_init_type_system);
                    }
                }
            }
        }break;
        case cb_connect_with_ap:
        {
            extern TickType_t lte_system_tick;
            extern uint8_t ble_init_flag;
			static uint8_t sb_reset_connect_flag = 0;
            start_one_flag = 1;
            lte_system_tick = xTaskGetTickCount();
            if(ble_init_flag == 0)
            {
                return TRUE;
            }

            b_result = net_tbb_server_establish_ap_connect();					//建立ap连接

            if(net_com_succeed == b_result)
            {
				b_tbb_device_server_com_step = cb_connect_with_tbb_device_server;
            }
            else if(net_com_fail == b_result)
            {
				set_module_initialize_state(FALSE);
				b_tbb_device_server_com_step = cb_reset_wireless_module;
            }
            else if(net_com_timeout == b_result)
            {
                if(sb_reset_connect_flag > 1)
            	{
            		sb_reset_connect_flag = 0;
					b_tbb_device_server_com_step = cb_reconncet_delay;
					b_tbb_device_server_com_step_next = cb_connect_with_ap;
					dw_net_connect_delay_ms = 3 * 60 * 1000;
				}
				else
				{
					sb_reset_connect_flag ++;
				}
            }
        }break;
        case cb_disconnect_with_ap:
        {
            static uint8_t cnt = 0;         
            b_result = net_tbb_server_establish_ap_disconnect();			//断开ap连接

            if(net_com_succeed == b_result)
            {
				b_tbb_device_server_com_step = cb_connect_with_ap;
            }
            else if(net_com_timeout == b_result)
            {
            }

        }break;
        case cb_connect_with_tbb_device_server:
        {
            b_result = net_tbb_server_establish_sever_connect();
			static uint8_t sb_reset_connect_server_flag = 0;

            if(net_com_succeed == b_result)
            {
				b_tbb_device_server_com_step = cb_tbb_device_server_send_message;
            }
            else if(net_com_timeout == b_result)
            {
                if(sb_reset_connect_server_flag > 1)
            	{
            		sb_reset_connect_server_flag = 0;
					b_tbb_device_server_com_step = cb_reconncet_delay;
					b_tbb_device_server_com_step_next = cb_connect_with_tbb_device_server;
					dw_net_connect_delay_ms =  3 * 60 * 1000;
				}
				else
				{
					sb_reset_connect_server_flag ++;
				}
            }
        }break;
        case cb_disconnect_with_tbb_device_server:
        {
            b_result = net_tbb_server_establish_sever_close();

            if(net_com_succeed == b_result)
            {
				b_tbb_device_server_com_step = cb_connect_with_tbb_device_server;
            }
            else if(net_com_timeout == b_result)
            {
            }
        }break;
        case cb_reset_wireless_module:
        {
            b_result = net_tbb_server_establish_module_reset();

            if(net_com_succeed == b_result)
            {
                set_module_initialize_state(TRUE);
                network_ble_state_set(net_ble_reset);
				b_tbb_device_server_com_step = cb_connect_with_ap;
                dw_net_connect_delay_ms = 15000;
            }
        }break;
        case cb_tbb_device_server_send_message:
        {
            net_tbb_device_server_send_message(network_com_with_tbb_device_server_state_get()); //! 发送下面接收到的数据到AT发送消息队列
			b_tbb_device_server_com_step = cb_tbb_device_server_receive_message;
        }break;
        case cb_tbb_device_server_receive_message:
        {
            net_tbb_device_server_receive_message();  //! 接收AT接收消息队列的数据，解析并处理。最后发送消息队列到上面net_tbb_device_server_send_message的发送消息队列

			b_tbb_device_server_com_step = cb_reconncet_delay;
			b_tbb_device_server_com_step_next = cb_tbb_device_server_send_message;
            dw_net_connect_delay_ms = 0;
        }break;
        case cb_reconncet_delay:
        {
            uint8_t b_result = 0;

            b_result += check_ap_connect_enable();
            if(1 == b_result)
            {
                dw_net_connect_delay_ms = 0;
            }
            if(TRUE == network_tbb_sever_com_delay(dw_net_connect_delay_ms))
            {
                if(net_get_ap_info())
                {
                    switch(b_result)
                    {
                        case 1:
                        {
                            dw_net_connect_delay_ms = 30000;
                            network_com_with_tbb_device_server_state_set(cb_net_com_state_type_err_code,net_ap_reconnect);
                            display_management_parameter_send(normal_mode,net_led,ap_disconnect,TRUE);
							b_tbb_device_server_com_step_next = cb_reconncet_delay;

                        }break;
                        case 2:
                        {
                            dw_net_connect_delay_ms = 30000;
							b_tbb_device_server_com_step = b_tbb_device_server_com_step_next;
							b_tbb_device_server_com_step_next = cb_reconncet_delay;
                        }break;
                        default:
                        {
                            dw_net_connect_delay_ms = 5000;
							b_tbb_device_server_com_step = b_tbb_device_server_com_step_next;
							b_tbb_device_server_com_step_next = cb_reconncet_delay;
                        }break;
                    }
                }
            }
        }break;
        default:
        {
        }break;
    }


    return TRUE;
}
