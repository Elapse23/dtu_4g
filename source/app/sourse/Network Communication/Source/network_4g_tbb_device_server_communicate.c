#include "network_tbb_device_server_communicate.h"
#include "network_4g_tbb_device_server_communicate.h"
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
uint8_t net_4g_tbb_server_communicate_initial()
{
    #define cb_tbb_device_server_iot_module_initial               0
    #define cb_tbb_device_server_iot_module_initial_wait          1
    static uint8_t b_tbb_device_server_connect_step = cb_tbb_device_server_iot_module_initial;
    switch(b_tbb_device_server_connect_step)
    {
        case cb_tbb_device_server_iot_module_initial:
        {

            if(TRUE == network_iot_module_communicate_channel_operate(iot_module_type_4g,
                                                                      lte_4g_module_operate_initial,
                                                                      0,
                                                                      0))
            {
                b_tbb_device_server_connect_step = cb_tbb_device_server_iot_module_initial_wait;
            }
        }break;
        case cb_tbb_device_server_iot_module_initial_wait:
        {
            uint8_t b_result = 0;

            b_result = network_iot_module_communicate_channel_operate_response(iot_module_type_4g,
                                                                               lte_4g_module_operate_initial,
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
uint8_t net_4g_tbb_server_establish_sim_connect()
{
    #define cb_tbb_device_server_initiate_sim_connect                   0
    #define cb_tbb_device_server_confirm_sim_connect                    1
    static uint8_t b_tbb_device_server_connect_step = cb_tbb_device_server_initiate_sim_connect;

    switch(b_tbb_device_server_connect_step)
    {
        case cb_tbb_device_server_initiate_sim_connect:
        {
            if(TRUE == network_iot_module_communicate_channel_operate(iot_module_type_4g,
                                                                      lte_4g_module_operate_open_sim_card_connection,
                                                                      0,
                                                                      0))
            {
                b_tbb_device_server_connect_step = cb_tbb_device_server_confirm_sim_connect;
            }
        }break;
        case cb_tbb_device_server_confirm_sim_connect:
        {
            uint8_t b_result = 0;
            b_result = network_iot_module_communicate_channel_operate_response(iot_module_type_4g,
                                                                               lte_4g_module_operate_open_sim_card_connection,
                                                                               0);
            if(1 == b_result)
            {
                network_com_with_tbb_device_server_state_set(cb_net_com_state_type_sim,cb_ap_connect_normal);
                b_tbb_device_server_connect_step = cb_tbb_device_server_initiate_sim_connect;
                return net_com_succeed;
            }
            else if(2 == b_result)
            {
                b_tbb_device_server_connect_step = cb_tbb_device_server_initiate_sim_connect;
                return net_com_timeout;
            }
            else if(3 == b_result)
            {
                b_tbb_device_server_connect_step = cb_tbb_device_server_initiate_sim_connect;
                return net_com_fail;
            }

        }break;
    }

    return net_com_wait;
}

uint8_t net_4g_tbb_server_establish_module_reset()
{
    #define cb_tbb_device_server_iot_module_reset               0
    #define cb_tbb_device_server_iot_module_reset_wait          1
    static uint8_t b_tbb_device_server_connect_step = cb_tbb_device_server_iot_module_reset;
    switch(b_tbb_device_server_connect_step)
    {
        case cb_tbb_device_server_iot_module_reset:
        {

            if(TRUE == network_iot_module_communicate_channel_operate(iot_module_type_4g,
                                                                      lte_4g_module_operate_reset,
                                                                      0,
                                                                      0))
            {
                b_tbb_device_server_connect_step = cb_tbb_device_server_iot_module_reset_wait;
            }
        }break;
        case cb_tbb_device_server_iot_module_reset_wait:
        {
            uint8_t b_result = 0;

            b_result = network_iot_module_communicate_channel_operate_response(iot_module_type_4g,
                                                                               lte_4g_module_operate_reset,
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
uint8_t net_4g_tbb_server_establish_sever_connect()
{
    #define cb_tbb_device_server_initiate_connect                   0
    #define cb_tbb_device_server_confirm_connect                    1
    static uint8_t b_tbb_device_server_connect_step = cb_tbb_device_server_initiate_connect;
    static uint8_t b_err_cnt = 0;
    switch(b_tbb_device_server_connect_step)
    {
        case cb_tbb_device_server_initiate_connect:
        {
            if(TRUE == network_iot_module_communicate_channel_operate(iot_module_type_4g,
                                                                      lte_4g_module_operate_open_tcp_connection,
                                                                      0,
                                                                      0))
            {
                b_err_cnt++;
                b_tbb_device_server_connect_step = cb_tbb_device_server_confirm_connect;
            }
        }break;
        case cb_tbb_device_server_confirm_connect:
        {
            uint8_t b_result = 0;
            b_result = network_iot_module_communicate_channel_operate_response(iot_module_type_4g,
                                                                               lte_4g_module_operate_open_tcp_connection,
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


uint8_t net_4g_tbb_server_establish_sever_close()
{
    #define cb_tbb_device_server_initiate_close                   0
    #define cb_tbb_device_server_confirm_close                    1
    static uint8_t b_tbb_device_server_close_step = cb_tbb_device_server_initiate_close;
    switch(b_tbb_device_server_close_step)
    {
        case cb_tbb_device_server_initiate_close:
        {
            if(TRUE == network_iot_module_communicate_channel_operate(iot_module_type_4g,
                                                                      lte_4g_module_operate_close_tcp_connection,
                                                                      0,
                                                                      0))
            {
                b_tbb_device_server_close_step = cb_tbb_device_server_confirm_close;
            }
        }break;
        case cb_tbb_device_server_confirm_close:
        {
            uint8_t b_result = 0;
            b_result = network_iot_module_communicate_channel_operate_response(iot_module_type_4g,
                                                                               lte_4g_module_operate_close_tcp_connection,
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


uint8_t net_4g_tbb_server_establish_sever_reconnect()
{
    #define cb_tbb_device_server_initiate_reconnect                   0
    #define cb_tbb_device_server_confirm_reconnect                    1
    static uint8_t b_tbb_device_server_connect_step = cb_tbb_device_server_initiate_reconnect;
    switch(b_tbb_device_server_connect_step)
    {
        case cb_tbb_device_server_initiate_reconnect:
        {
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
uint8_t net_4g_tbb_device_server_send_message(uint8_t b_net_data_result)
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
uint8_t net_4g_tbb_device_server_receive_message()
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
        b_receive_result = iot_module_receive_data((void *)(pst_network_data));		//lte_module_user_data_receive  lte_4g_module_user_data_receive 
        vTaskDelay(0);
    }while(b_receive_result != 2);
    network_data_receive((void *)pst_network_data,cb_channel_type_4g_module);
    network_inquire_cmd_analysis();
    network_config_cmd_analysis();
    network_other_cmd_analysis();

    b_result = net_data_response();
    network_com_with_tbb_device_server_state_set(cb_net_com_state_type_err_code,b_result);

    vPortFree(pst_network_data->st_data_info.pb_data);
    vPortFree(pst_network_data);

    return b_result;
}
uint8_t net_get_sim_info()
{
    #define cb_get_sim_info_initiate                    0
    #define cb_get_sim_info_comfirm                     1
    #define cb_get_sim_info_delay                       2
    #define cb_get_sim_info_none                        3

    static uint8_t b_get_sim_info_step = cb_get_sim_info_initiate;
    lte_module_sim_info st_sim_info = {0,};

    if(0 != st_device_update_info_current.pst_net_device_update_info_current)
    {
        return TRUE;
    }
    do{
        switch(b_get_sim_info_step)
        {
            case cb_get_sim_info_initiate:
            {
                if(TRUE == network_iot_module_communicate_channel_operate(iot_module_type_4g,
                                                                          lte_4g_module_operate_get_sim_info,
                                                                          0,
                                                                          &st_sim_info))
                {
                    b_get_sim_info_step = cb_get_sim_info_comfirm;
                }
            }break;
            case cb_get_sim_info_comfirm:
            {
                uint8_t b_result = 0;
                uint16_t w_data_len = 1;
                b_result = network_iot_module_communicate_channel_operate_response(iot_module_type_4g,
                                                                                   lte_4g_module_operate_get_sim_info,
                                                                                   0);
                if(TRUE == b_result)
                {
                    b_get_sim_info_step = cb_get_sim_info_delay;

                    net_system_data_access(cb_device_data_access_data_type_status,
                                   0x0106,
                                   &w_data_len,
                                   (uint8_t *)&st_sim_info.b_signal_pct,
                                   cb_device_data_access_operate_type_write);
                }
                else if(2 == b_result)
                {
                    b_get_sim_info_step = cb_get_sim_info_delay;

                }
            }break;
            case cb_get_sim_info_delay:
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
                    b_get_sim_info_step = cb_get_sim_info_initiate;
                }
                else
                {
                    return TRUE;
                }
            }break;
        }
        vTaskDelay(0);
    }while(cb_get_sim_info_none != b_get_sim_info_step);

    return FALSE;
}


void network_com_with_tbb_device_server_state_set(uint8_t b_state_type,
                                                  uint8_t b_state)
{
    switch(b_state_type)
    {
        case cb_net_com_state_type_sever:
        {
            st_net_com_state_managemet.f_sever_connect = b_state;
            {
                uint16_t w_data_len = 1;
                net_system_data_access(cb_device_data_access_data_type_status,
                                       0x0105,
                                       &w_data_len,
                                       (uint8_t *)&st_net_com_state_managemet.f_sever_connect,
                                       cb_device_data_access_operate_type_write);
            }
        }
        break;
        case cb_net_com_state_type_ap:
        {
            network_ap_connect_err_code_set(b_state);
        }
        break;
		case cb_net_com_state_type_sim:
        {
            network_ap_connect_err_code_set(b_state);
        }
        break;
        case cb_net_com_state_type_err_code:
        {
            if(net_sever_connect == b_state)
            {
                st_net_com_state_managemet.f_sever_reconnect_enable = TRUE;
            }
            else
            {
                st_net_com_state_managemet.f_modify = TRUE;
                st_net_com_state_managemet.b_state = b_state;
            }
        }
        break;
        default:
            break;
    }
}
uint8_t network_com_with_tbb_device_server_state_get()
{
    return st_net_com_state_managemet.b_state;
}

/**
 * @brief 与TBB服务器通信主函数 在 network_communicate_task 中轮询调用
 * @return {uint8_t}
 * @author Yao YiLiang
 */
#pragma optimize=none
uint8_t network_4g_com_with_tbb_device_server()
{
    #define cb_tbb_device_server_com_initial                                0
    #define cb_startup_delay                                                1
    #define cb_connect_with_sim                                             2
    #define cb_disconnect_with_ap                                           3
    #define cb_connect_with_tbb_device_server                               4
    #define cb_disconnect_with_tbb_device_server                            5
    #define cb_reset_wireless_module                                        6
    #define cb_tbb_device_server_send_message                               7
    #define cb_tbb_device_server_receive_message                            8
    #define cb_reconncet_delay                                              9

    volatile static uint8_t b_tbb_device_server_com_step = cb_tbb_device_server_com_initial;
    volatile static uint8_t b_tbb_device_server_com_step_next = cb_tbb_device_server_com_initial;
    static uint32_t dw_net_connect_delay_ms = 1500;

    uint8_t b_result = net_com_wait;
    if((cb_tbb_device_server_com_initial != b_tbb_device_server_com_step) &&\
       (TRUE == st_net_com_state_managemet.f_modify))
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
    }
    else if(st_net_com_state_managemet.f_sever_reconnect_enable)
    {
        st_net_com_state_managemet.f_sever_reconnect_enable = FALSE;
        st_net_com_state_managemet.b_state = net_sever_connect;
        if((cb_tbb_device_server_receive_message == b_tbb_device_server_com_step) ||\
               (st_net_com_state_managemet.f_sever_connect &&\
                (cb_reconncet_delay == b_tbb_device_server_com_step)))
        {
            b_tbb_device_server_com_step = cb_tbb_device_server_send_message;
        }
    }
    offline_data_save();

    switch(b_tbb_device_server_com_step)
    {
        case cb_tbb_device_server_com_initial:
        {
            b_result = net_4g_tbb_server_communicate_initial();

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
                network_system_init();
                if(FALSE == check_system_init(cb_system_init_type_network))
                {
                    network_system_init();
                    dw_net_connect_delay_ms = 30000;
                }
                else
                {
                    if(TRUE == network_tbb_sever_com_delay(dw_net_connect_delay_ms))
                    {
                        b_tbb_device_server_com_step = cb_connect_with_sim;
                        dw_net_connect_delay_ms = 15000;
                        system_init_comfirm(cb_system_init_type_system);
                    }
                }
            }
        }break;
		case cb_connect_with_sim:
        {
            b_result = net_4g_tbb_server_establish_sim_connect();

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
                b_tbb_device_server_com_step_next = cb_connect_with_sim;
                b_tbb_device_server_com_step = cb_reconncet_delay;
            }
        }break;   
        case cb_connect_with_tbb_device_server:
        {
            b_result = net_4g_tbb_server_establish_sever_connect();

            if(net_com_succeed == b_result)
            {
                b_tbb_device_server_com_step = cb_tbb_device_server_send_message;
            }
            else if(net_com_timeout == b_result)
            {
            }
        }break;
        case cb_disconnect_with_tbb_device_server:
        {
            b_result = net_4g_tbb_server_establish_sever_close();

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
            b_result = net_4g_tbb_server_establish_module_reset();

            if(net_com_succeed == b_result)
            {
                set_module_initialize_state(TRUE);
                network_ble_state_set(net_ble_reset);
                b_tbb_device_server_com_step = cb_connect_with_sim;
                dw_net_connect_delay_ms = 15000;
            }
        }break;
        case cb_tbb_device_server_send_message:
        {
            net_4g_tbb_device_server_send_message(network_com_with_tbb_device_server_state_get()); //! 发送下面接收到的数据到AT发送消息队列

            b_tbb_device_server_com_step = cb_tbb_device_server_receive_message;
        }break;
        case cb_tbb_device_server_receive_message:
        {
            net_4g_tbb_device_server_receive_message();  //! 接收AT接收消息队列的数据，解析并处理。最后发送消息队列到上面net_tbb_device_server_send_message的发送消息队列

            b_tbb_device_server_com_step = cb_reconncet_delay;
            dw_net_connect_delay_ms = 0;
            b_tbb_device_server_com_step_next = cb_tbb_device_server_send_message;
        }break;
        case cb_reconncet_delay:
        {
			network_tbb_sever_com_delay(dw_net_connect_delay_ms);
			uint8_t b_result = 0;

            b_result += check_sim_connect_enable();
            if(1 == b_result)
            {
                dw_net_connect_delay_ms = 0;
            }
            if(TRUE == network_tbb_sever_com_delay(dw_net_connect_delay_ms))
            {
                if(net_get_sim_info())
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

