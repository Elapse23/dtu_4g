#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "network_tbb_device_server_communicate.h"
#include "network_ble_communication.h"
#include "freertos_user_config.h"
#include "network_ble_communication_queue.h"
#include "iot_module_config_common.h"
#include "lte_module_config_common.h"
#include "network_protocal_data_assembly.h"
#include "network_iot_module_data_channel_manage.h"
#include "printf_custom.h"
#include "task.h"
#include "lte_module_ble_at_driver.h"
#include "crc16_arithmetic.h"



#define  cb_net_data_unpack_complete    0
#define  cb_net_data_unpack_failed      1
#define  cb_net_data_unpack_continue    2
#define  cb_net_data_receive_continue   3

#define  ble_message_splice_cache_max   2000

typedef struct
{
    uint8_t f_ble_enable;
    uint8_t b_ble_channel;
}ble_communication_info;

static TaskHandle_t network_ble_communication_task_handle = 0;
//static ble_communication_info st_ble_communication_info = {0,};


typedef enum
{
    net_ble_succeed,
    net_ble_wait,
    net_ble_timeout,
}net_ble_result;

uint8_t b_net_ble_state = net_ble_reconnect;
uint8_t ble_connect_net_disconncet = 0;
uint8_t f_module_init_succeed = false;
uint8_t auto_disconnect_state = 0;
uint32_t auto_disconnect_cnt = 0;

//void net_ble_communication_enable(uint8_t f_enable)
//{
//    st_ble_communication_info.f_ble_enable = f_enable;
//}

void set_module_initialize_state(uint8_t f_state)
{
    f_module_init_succeed = f_state;
}

uint8_t net_ble_establish_ble_init()
{
    #define cb_tbb_device_ble_init_operate_init                   0
    #define cb_tbb_device_ble_init_operate_comfirm                1
    static uint8_t b_tbb_device_ble_init_step = cb_tbb_device_ble_init_operate_init;

    lte_error_check(&net_ble_establish_ble_init_flag, &b_tbb_device_ble_init_step);

    switch(b_tbb_device_ble_init_step)
    {
        case cb_tbb_device_ble_init_operate_init:
        {
            net_ble_establish_ble_init_cnt = 0;
            if(TRUE == network_iot_module_communicate_channel_operate(iot_module_type_lte,
                                                                      lte_module_operate_ble_config,
                                                                      0,
                                                                      0))
            {
                b_tbb_device_ble_init_step = cb_tbb_device_ble_init_operate_comfirm;
            }
        }break;
        case cb_tbb_device_ble_init_operate_comfirm:
        {
            if(FALSE == lte_error_cnt(&net_ble_establish_ble_init_cnt,default_lte_error_cnt_max))
            {
                yl_debug_printf("net_ble_establish_ble_init_cnt error\n");
                return TRUE;
            }
            uint8_t b_result = 0;
            b_result = network_iot_module_communicate_channel_operate_response(iot_module_type_lte,
                                                                               lte_module_operate_ble_config,
                                                                               0);
            if(1 == b_result)
            {
                b_tbb_device_ble_init_step = cb_tbb_device_ble_init_operate_init;
                return net_ble_succeed;
            }
            else if(2 == b_result)
            {
                b_tbb_device_ble_init_step = cb_tbb_device_ble_init_operate_init;
            }

        }break;
    }

    return net_ble_wait;
}

uint8_t net_ble_open_broadcast()
{
    #define cb_tbb_device_ble_broadcast_operate_open                 0
    #define cb_tbb_device_ble_broadcast_operate_comfirm              1
    static uint8_t b_tbb_device_server_connect_step = cb_tbb_device_ble_broadcast_operate_open;

    lte_error_check(&net_ble_open_broadcast_flag, &b_tbb_device_server_connect_step);
    switch(b_tbb_device_server_connect_step)
    {
        case cb_tbb_device_ble_broadcast_operate_open:
        {
            net_ble_open_broadcast_cnt = 0;
            if(TRUE == network_iot_module_communicate_channel_operate(iot_module_type_lte,
                                                                      lte_module_operate_ble_open_broadcast,
                                                                      0,
                                                                      0))
            {
                b_tbb_device_server_connect_step = cb_tbb_device_ble_broadcast_operate_comfirm;
            }
        }break;
        case cb_tbb_device_ble_broadcast_operate_comfirm:
        {
            if(FALSE == lte_error_cnt(&net_ble_open_broadcast_cnt,default_lte_error_cnt_max))
            {
                yl_debug_printf("net_ble_open_broadcast_cnt error\n");
                return TRUE;
            }
            uint8_t b_result = 0;
            b_result = network_iot_module_communicate_channel_operate_response(iot_module_type_lte,
                                                                               lte_module_operate_ble_open_broadcast,
                                                                               0);
            if(1 == b_result)
            {
                b_tbb_device_server_connect_step = cb_tbb_device_ble_broadcast_operate_open;
                return net_ble_succeed;
            }
        }break;
    }

    return net_ble_wait;
}

uint8_t net_ble_establish_ble_deinit()
{
    #define cb_tbb_device_ble_deinit                        0
    #define cb_tbb_device_ble_deinit_comfirm                1
    static uint8_t b_tbb_device_server_connect_step = cb_tbb_device_ble_deinit;
    lte_error_check(&net_ble_establish_ble_deinit_flag, &b_tbb_device_server_connect_step);
    switch(b_tbb_device_server_connect_step)
    {
        case cb_tbb_device_ble_deinit:
        {
            net_ble_establish_ble_deinit_cnt = 0;
            if(TRUE == network_iot_module_communicate_channel_operate(iot_module_type_lte,
                                                                      lte_module_operate_ble_close,
                                                                      0,
                                                                      0))
            {
                b_tbb_device_server_connect_step = cb_tbb_device_ble_deinit_comfirm;
            }
        }break;
        case cb_tbb_device_ble_deinit_comfirm:
        {
            if(FALSE == lte_error_cnt(&net_ble_establish_ble_deinit_cnt,default_lte_error_cnt_max))
            {
                yl_debug_printf("net_ble_establish_ble_deinit_cnt error\n");
                return TRUE;
            }
            if(TRUE == network_iot_module_communicate_channel_operate_response(iot_module_type_lte,
                                                                               lte_module_operate_ble_close,
                                                                               0))
            {
                b_tbb_device_server_connect_step = cb_tbb_device_ble_deinit;
                return net_ble_succeed;
            }
        }break;
    }

    return net_ble_wait;
}


void ble_data_receive(void *pv_data)
{
    uint8_t b_net_data_unpack_staus = cb_net_data_unpack_failed;

    uint16_t w_network_data_fragment_len = 0;
    network_data *pst_network_data_fragment = (network_data *)pv_data;

    #define cw_network_data_buffer_size ble_message_splice_cache_max
    static uint16_t w_network_data_len = 0;
    static uint8_t ba_network_data[cw_network_data_buffer_size] = {0,};
    uint16_t w_data_in_index = 0;

    if(0 == pst_network_data_fragment->st_data_info.w_data_len)
    {
        return;
    }
    do
    {
        if((NULL == pst_network_data_fragment) ||\
           (NULL == pst_network_data_fragment->st_data_info.pb_data))
        {
            w_network_data_len = 0;
            return;
        }

        #if(dynamic_memory_request_test)
        if(pst_network_data_fragment->w_data_len > cw_network_data_buffer_size)
        {
            DebugPrintf(TRUE,"ble_data_receive_data_fill_1,len: %d,addr: 0x%x\n",pst_network_data_fragment->w_data_len,
                                                                                     ba_network_data);
        }
        #endif

        b_net_data_unpack_staus = net_data_unpack(&pst_network_data_fragment->st_data_info.pb_data[w_data_in_index],\
                                                  pst_network_data_fragment->st_data_info.w_data_len,\
                                                  &w_data_in_index,\
                                                  &ba_network_data[w_network_data_len],\
                                                  &w_network_data_fragment_len);

        if((cb_net_data_unpack_complete == b_net_data_unpack_staus) ||\
           (cb_net_data_unpack_continue == b_net_data_unpack_staus))
        {
            w_network_data_len += w_network_data_fragment_len;

            if(FALSE == check_crc16(&ba_network_data[0],w_network_data_len))
            {
                w_network_data_len = 0;
                return;
            }

            {
                network_data *pst_network_data = 0;

                {
//                    do{
//                        pst_network_data = (network_data *)pvPortMalloc(sizeof(network_data));
//                    }
//                    while(0 == pst_network_data);
                    pst_network_data = (network_data *)pvPortMalloc(sizeof(network_data));
                    if(NULL == pst_network_data)
                    {
                        #if( system_alarm_print)
                        DebugPrintf(TRUE,"[ERROR]BLE receive malloc err:1\r\n");
                        #endif
                        return;
                    }
                    #if(dynamic_memory_request_test)
                    DebugPrintf(TRUE,"ble_data_receive_0_malloc%x\n",pst_network_data);
                    #endif
                    memset(pst_network_data,0,sizeof(network_data));

//                    do{
//                        pst_network_data->st_data_info.pb_data = (uint8_t *)pvPortMalloc(w_network_data_len * sizeof(uint8_t));
//                    }
//                    while(0 == pst_network_data->st_data_info.pb_data);
                    pst_network_data->st_data_info.pb_data = (uint8_t *)pvPortMalloc(w_network_data_len * sizeof(uint8_t));
                    if(NULL == pst_network_data->st_data_info.pb_data)
                    {
                        if(NULL != pst_network_data)
                        {
                            vPortFree(pst_network_data);
                        }
                        #if( system_alarm_print)
                        DebugPrintf(TRUE,"[ERROR]BLE receive malloc err:2\r\n");
                        #endif
                        return;
                    }
                    #if(dynamic_memory_request_test)
                    DebugPrintf(TRUE,"ble_data_receive_1_malloc,%d,%x\n",w_network_data_len,pst_network_data->pb_data);
                    #endif
                    memset(pst_network_data->st_data_info.pb_data,0,w_network_data_len);
                }

                {
                    pst_network_data->b_channel_index = cb_channel_type_ble_module;
                    pst_network_data->st_data_info.w_data_len = w_network_data_len;

                    uint8_t *pb_source = &ba_network_data[0];
                    uint8_t *pb_target = pst_network_data->st_data_info.pb_data;
                    for(uint16_t w_cnt_i = 0;w_cnt_i < w_network_data_len;w_cnt_i++)
                    {
                        *pb_target = *pb_source;
                        pb_target++;
                        pb_source++;
                    }
                }
                {
                    if(FALSE == network_recevie_data(0x0000,pst_network_data))
                    {
//                    	yl_debug_printf("[ble data] ble rx data queue send FALSE !!!, Line is: %d \r\n",__LINE__);
                        vPortFree(pst_network_data->st_data_info.pb_data);//call first
                        #if(dynamic_memory_request_test)
                        DebugPrintf(TRUE,"network_data_receive_4_free%x\n",pst_network_data->pb_data);
                        #endif
                        vPortFree(pst_network_data);//call second
                        #if(dynamic_memory_request_test)
                        DebugPrintf(TRUE,"network_data_receive_5_free%x\n",pst_network_data);
                        #endif
                        return;
                    }
                }
                w_network_data_len = 0;
            }
        }
    }while(cb_net_data_unpack_continue == b_net_data_unpack_staus);
}

#define FRAME_HEADER                0xFE
uint8_t ba_ble_message_splice_cache[ble_message_splice_cache_max]={0};
uint32_t dw_splice_len = 0;
uint8_t net_ble_message_splice(uint8_t *pb_data,uint32_t dw_data_len)
{
    static uint8_t f_message_splice = FALSE;
    uint16_t w_first_find_frame_header_index = 0;
    static TickType_t x_start_time = 0;
    TickType_t x_curent_time = 0;

    if(NULL == pb_data)
    {
        return FALSE;
    }

    x_curent_time = xTaskGetTickCount();
    //超时处理
    if(f_message_splice==FALSE)
    {
        x_start_time = xTaskGetTickCount();
    }
    if((x_curent_time - x_start_time) > pdMS_TO_TICKS_USER(2000)&&\
        f_message_splice == TRUE)
    {

        x_start_time = xTaskGetTickCount();
        memset(ba_ble_message_splice_cache,0,ble_message_splice_cache_max*sizeof(uint8_t));
        dw_splice_len = 0;
        f_message_splice=FALSE;
    }

    if(FALSE == f_message_splice)
    {
        for(uint32_t dw_data_index = 0;dw_data_index < dw_data_len;dw_data_index++)
        {
            if(FRAME_HEADER == pb_data[dw_data_index])
            {
                if(f_message_splice)
                {
                    memcpy(ba_ble_message_splice_cache,
                           &pb_data[w_first_find_frame_header_index],
                           dw_data_len - w_first_find_frame_header_index);
                    f_message_splice = FALSE;
                    dw_splice_len = dw_data_len - w_first_find_frame_header_index;
                    break;
                }
                w_first_find_frame_header_index = dw_data_index;
                f_message_splice = TRUE;
            }
            else if(dw_data_index == (dw_data_len-1))
            {
                if(f_message_splice)
                {
                    memcpy(ba_ble_message_splice_cache,
                           &pb_data[w_first_find_frame_header_index],
                           dw_data_len - w_first_find_frame_header_index);
                    dw_splice_len = dw_data_len - w_first_find_frame_header_index;
                    return 2;
                }
                memset(ba_ble_message_splice_cache,0,ble_message_splice_cache_max*sizeof(uint8_t));
                dw_splice_len = 0;
                return 0;
            }
        }
    }
    else
    {
        for(uint32_t dw_data_index = 0;dw_data_index < dw_data_len;dw_data_index++)
        {
            if(FRAME_HEADER == pb_data[dw_data_index])
            {
                f_message_splice = FALSE;
                memcpy(&ba_ble_message_splice_cache[dw_splice_len],
                       pb_data,
                       dw_data_index + 1);
                dw_splice_len += dw_data_index + 1;
                break;
            }
            else if(dw_data_index == (dw_data_len - 1))
            {
                memcpy(&ba_ble_message_splice_cache[dw_splice_len],
                       pb_data,
                       dw_data_len);
                dw_splice_len += dw_data_len;
                return 2;
            }
        }
    }
    return 1;
}

uint8_t net_ble_send_message(void *pv_data)
{

    #define cb_tbb_device_ble_send_message_operate                  0
    #define cb_tbb_device_ble_send_message_comfirm                  1
    static uint8_t b_ble_send_message_step = cb_tbb_device_ble_send_message_operate;

    network_data *pst_network_data = 0;

    pst_network_data = (network_data *)pv_data;

    lte_error_check(&net_ble_send_message_flag, &b_ble_send_message_step);

    switch(b_ble_send_message_step)
    {
        case cb_tbb_device_ble_send_message_operate:
        {
            net_ble_send_message_cnt = 0;
            if(TRUE == network_iot_module_communicate_channel_operate(iot_module_type_lte,
                                                                      lte_module_operate_send_ble_user_data,
                                                                      pst_network_data->st_data_info.w_data_len,
                                                                      (uint8_t *)pst_network_data->st_data_info.pb_data))
            {
                b_ble_send_message_step = cb_tbb_device_ble_send_message_comfirm;
            }
        }break;
        case cb_tbb_device_ble_send_message_comfirm:
        {
            if(FALSE == lte_error_cnt(&net_ble_send_message_cnt,much_lte_error_cnt_max))
            {
                yl_debug_printf("net_ble_send_message_cnt error\n");
                return TRUE;
            }
            uint8_t b_result = 0;
            b_result = network_iot_module_communicate_channel_operate_response(iot_module_type_lte,
                                                                               lte_module_operate_send_ble_user_data,
                                                                               0);
            if(1 == b_result)
            {
                b_ble_send_message_step = cb_tbb_device_ble_send_message_operate;
                return TRUE;
            }
            else if(2 == b_result)
            {
                b_ble_send_message_step = cb_tbb_device_ble_send_message_operate;
                return b_result;
            }
        }break;
    }

    return FALSE;
}

uint8_t net_ble_parse_head_data(network_ble_communication_info *pst_network_ble_communication_info)
{
    if(strstr((char*)pst_network_ble_communication_info->pb_data,"+BLEDISCONN"))
    {
        auto_disconnect_state = 0;
        extern TickType_t lte_system_tick;
        lte_system_tick = xTaskGetTickCount();
        auto_disconnect_cnt = 0;
        b_net_ble_state = net_ble_disconnect;
        return 1;
    }
    else if(strstr((char*)pst_network_ble_communication_info->pb_data,"+BLECONN"))
    {
        b_net_ble_state = net_ble_connect;
        return 2;
    }
    else if(strstr((char*)pst_network_ble_communication_info->pb_data,"+WRITE:"))
    {

        auto_disconnect_state = 1;
        auto_disconnect_cnt = 0;

        uint8_t *pb_data = 0;
        uint32_t dw_data_len = 0;
        uint8_t b_cnt = 0;

        pb_data = (uint8_t *)strstr((char*)pst_network_ble_communication_info->pb_data,"+WRITE");

            for(uint16_t w_index = 0;(b_cnt < 5) && (w_index < pst_network_ble_communication_info->w_data_len);w_index++)
            {
                if(',' == pb_data[w_index])
                {
                    b_cnt++;

                    if(4 == b_cnt)
                    {
                        dw_data_len = atoi((char *)&pb_data[w_index + 1]);
                    }
                    else if(5 == b_cnt)
                    {
                        uint8_t b_result = 0;
                        pb_data = &pb_data[w_index + 1];

                        b_result = net_ble_message_splice(pb_data,dw_data_len);
                        if(2 == b_result)
                        {
                            return 4;
                        }

                        network_data *pst_network_data = 0;

//                        do{
//                            pst_network_data = (network_data *)pvPortMalloc(sizeof(network_data));
//                        }while(0 == pst_network_data);
                        pst_network_data = (network_data *)pvPortMalloc(sizeof(network_data));
                        if(NULL == pst_network_data)
                        {
                            #if( system_alarm_print)
                            DebugPrintf(TRUE,"[ERROR]BLE malloc err:1\r\n");
                            #endif
                            return 0;
                        }
                        memset(pst_network_data,0,sizeof(network_data));

//                        do{
//                            pst_network_data->st_data_info.pb_data = (uint8_t *)pvPortMalloc(500 * sizeof(uint8_t));
//                        }while(0 == pst_network_data->st_data_info.pb_data);
                        pst_network_data->st_data_info.pb_data = ba_ble_message_splice_cache;
                        if(NULL == pst_network_data->st_data_info.pb_data)
                        {
                            #if( system_alarm_print)
                            DebugPrintf(TRUE,"[ERROR]BLE malloc err:2\r\n");
                            #endif
                            return 0;
                        }


                        pst_network_data->st_data_info.w_data_len = dw_splice_len;

                        ble_data_receive((void *)pst_network_data);

                        network_ble_cmd_analysis();

                        // network_data_send(network_ble_tx_queue_receive);

                        #if(dynamic_memory_request_test)
                        DebugPrintf(TRUE,"net_ble_parse_head_data_1_free%x\n",pst_network_data->st_data_info.pb_data);
                        #endif
                        vPortFree(pst_network_data);//2
                        #if(dynamic_memory_request_test)
                        DebugPrintf(TRUE,"net_ble_parse_head_data_2_free%x\n",pst_network_data);
                        #endif


                        break;
                    }
                }
            }
        return 3;
    }
    return 0;
}

void net_ble_receive_message()
{
    network_ble_communication_info *pst_network_ble_communication_info = 0;

    if(TRUE == network_ble_communication_queue_receive((pv_queue_operate_msg *)&pst_network_ble_communication_info,TRUE))
    {
        net_ble_parse_head_data(pst_network_ble_communication_info);

        vPortFree(pst_network_ble_communication_info->pb_data);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"net_ble_receive_message_0_free%x\n",pst_network_ble_communication_info->pb_data);
        #endif
        vPortFree(pst_network_ble_communication_info);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"net_ble_receive_message_1_free%x\n",pst_network_ble_communication_info);
        #endif

    }
    network_data_send(network_ble_tx_queue_receive);
}

void network_ble_state_set(uint8_t b_state)
{
    b_net_ble_state = b_state;
}

uint8_t network_step_safe_switch(uint8_t* step, uint8_t next_step);
uint8_t get_net_system_type(uint32_t *pdw_system_type_in);
uint8_t network_response_safe_check(uint8_t iot_type);
extern uint8_t iot_send_safe_unlock;
uint8_t ble_init_flag = 0;
uint8_t b_tbb_device_ble_com_step = 0;
TickType_t ble_x_start_time = 0;
uint8_t b_ble_outtime_connect_flag = 0;
uint8_t network_ble_communication_with_app()
{

    #define cb_ble_initial                                                  0
    #define cb_ble_lost_connect                                             1
    #define cb_tbb_device_ble_receive_message                               2

    if(auto_disconnect_state == 1)
    {
        auto_disconnect_cnt+=50;
    }
    if(auto_disconnect_cnt > 30000)
    {
    	b_ble_outtime_connect_flag = 1;
        auto_disconnect_cnt = 0;
        auto_disconnect_state = 0;
        extern TickType_t lte_system_tick;
        lte_system_tick = xTaskGetTickCount();
        b_net_ble_state = net_ble_disconnect;
    }
    if(FALSE == network_response_safe_check(0) || iot_send_safe_unlock == 1)
    {
        return FALSE;
    }
    uint8_t b_result = net_ble_wait;
    if(cb_ble_initial != b_tbb_device_ble_com_step)
    {
        if(net_ble_disconnect == b_net_ble_state)
        {
			b_tbb_device_ble_com_step = cb_ble_lost_connect;
        }
        else if((net_ble_close == b_net_ble_state) || (net_ble_reset == b_net_ble_state))
        {
			b_tbb_device_ble_com_step = cb_ble_initial;
        }
    }
    uint32_t dw_net_system_type = 0;
    if(FALSE == get_net_system_type(&dw_net_system_type))
    {
        return FALSE;
    }
    switch(b_tbb_device_ble_com_step)
    {
        case cb_ble_initial:
        {
            if(f_module_init_succeed)
            {
                b_result = net_ble_establish_ble_init();

                if(net_ble_succeed == b_result)
                {
                    ble_init_flag = 1;
                    b_net_ble_state = net_ble_reconnect;
                    network_step_safe_switch(&b_tbb_device_ble_com_step, cb_tbb_device_ble_receive_message);
                }
            }
        }break;
        case cb_ble_lost_connect:
        {
            TickType_t x_curent_time = 0;
            x_curent_time = xTaskGetTickCount();
            if(x_curent_time - ble_x_start_time >= pdMS_TO_TICKS_USER(15000))
            {
                b_result = net_ble_open_broadcast();

                if(net_ble_succeed == b_result)
                {
                	ble_x_start_time = xTaskGetTickCount();
                    b_net_ble_state = net_ble_reconnect;
					b_tbb_device_ble_com_step = cb_tbb_device_ble_receive_message;
                }
            }
        }break;
        case cb_tbb_device_ble_receive_message:
        {
            net_ble_receive_message();
        }break;
        default:
        {
        }break;
    }
    return TRUE;
}

uint8_t network_ble_communication_close()
{
    uint8_t b_result = net_ble_wait;

    if(net_ble_close != b_net_ble_state)
    {
        b_result = net_ble_establish_ble_deinit();
        if(net_ble_succeed == b_result)
        {
            b_net_ble_state = net_ble_close;
        }
    }
    return TRUE;
}

void network_ble_communication()
{
//    if(st_ble_communication_info.f_ble_enable)
//    {
//        network_ble_communication_with_app();
//    }
//    else
//    {
//        network_ble_communication_close();
//    }
#ifdef KINERGY_II_WIFI
    network_ble_communication_with_app();
#else
	network_4g_ble_communication_with_app();
#endif

}

void network_ble_communication_task(void* parameter)
{
	DebugPrintf(TRUE,"%d,network_ble_communication_task_entry\n",__LINE__);

    for(;;)
    {
		#if(task_call_rate_test)
		DebugPrintf(TRUE,"network_ble_communication_task,start: %d,priority: %d\n",xTaskGetTickCount(),TIMESTAMP_TASK_PRIORITY);
		#endif
		network_ble_communication();
		#if(task_call_rate_test)
		DebugPrintf(TRUE,"network_ble_communication_task,end: %d,priority: %d\n",xTaskGetTickCount(),TIMESTAMP_TASK_PRIORITY);
		#endif

		vTaskDelay(pdMS_TO_TICKS_USER(50));
    }
}

void network_ble_communication_task_init()
{
    BaseType_t xReturn = pdPASS;

    network_ble_communication_queue_init();
    xReturn = xTaskCreate( network_ble_communication_task,"network_ble_communication_task",network_ble_communication_task_stack_size,NULL,network_ble_communication_task_priority,&network_ble_communication_task_handle);
    if(pdFAIL == xReturn)
    {
        DebugPrintf(TRUE,"%d,network_ble_communication_task Create Fail\n",__LINE__);
    }
}
uint8_t net_4g_ble_establish_ble_init()
{
    #define cb_tbb_device_ble_init_operate_init                   0
    #define cb_tbb_device_ble_init_operate_comfirm                1
    static uint8_t b_tbb_device_ble_init_step = cb_tbb_device_ble_init_operate_init;
    switch(b_tbb_device_ble_init_step)
    {
        case cb_tbb_device_ble_init_operate_init:
        {
            if(TRUE == network_iot_module_communicate_channel_operate(iot_module_type_4g,
                                                                      lte_4g_module_operate_ble_config,
                                                                      0,
                                                                      0))
            {
                b_tbb_device_ble_init_step = cb_tbb_device_ble_init_operate_comfirm;
            }
        }break;
        case cb_tbb_device_ble_init_operate_comfirm:
        {
            uint8_t b_result = 0;
            b_result = network_iot_module_communicate_channel_operate_response(iot_module_type_4g,
                                                                               lte_4g_module_operate_ble_config,
                                                                               0);
            if(1 == b_result)
            {
                b_tbb_device_ble_init_step = cb_tbb_device_ble_init_operate_init;
                return net_ble_succeed;
            }
            else if(2 == b_result)
            {
                b_tbb_device_ble_init_step = cb_tbb_device_ble_init_operate_init;
            }

        }break;
    }

    return net_ble_wait;
}

uint8_t net_4g_ble_open_broadcast()
{
    #define cb_tbb_device_ble_broadcast_operate_open                 0
    #define cb_tbb_device_ble_broadcast_operate_comfirm              1
    static uint8_t b_tbb_device_server_connect_step = cb_tbb_device_ble_broadcast_operate_open;
    switch(b_tbb_device_server_connect_step)
    {
        case cb_tbb_device_ble_broadcast_operate_open:
        {

            if(TRUE == network_iot_module_communicate_channel_operate(iot_module_type_4g,
                                                                      lte_4g_module_operate_ble_open_broadcast,
                                                                      0,
                                                                      0))
            {
                b_tbb_device_server_connect_step = cb_tbb_device_ble_broadcast_operate_comfirm;
            }
        }break;
        case cb_tbb_device_ble_broadcast_operate_comfirm:
        {
            uint8_t b_result = 0;
            b_result = network_iot_module_communicate_channel_operate_response(iot_module_type_4g,
                                                                               lte_4g_module_operate_ble_open_broadcast,
                                                                               0);
            if(1 == b_result)
            {
                b_tbb_device_server_connect_step = cb_tbb_device_ble_broadcast_operate_open;
                return net_ble_succeed;
            }
        }break;
    }

    return net_ble_wait;
}

uint8_t net_4g_ble_establish_ble_deinit()
{
    #define cb_tbb_device_ble_deinit                        0
    #define cb_tbb_device_ble_deinit_comfirm                1
    static uint8_t b_tbb_device_server_connect_step = cb_tbb_device_ble_deinit;
    switch(b_tbb_device_server_connect_step)
    {
        case cb_tbb_device_ble_deinit:
        {

            if(TRUE == network_iot_module_communicate_channel_operate(iot_module_type_4g,
                                                                      lte_4g_module_operate_ble_close,
                                                                      0,
                                                                      0))
            {
                b_tbb_device_server_connect_step = cb_tbb_device_ble_deinit_comfirm;
            }
        }break;
        case cb_tbb_device_ble_deinit_comfirm:
        {
            if(TRUE == network_iot_module_communicate_channel_operate_response(iot_module_type_4g,
                                                                               lte_4g_module_operate_ble_close,
                                                                               0))
            {
                b_tbb_device_server_connect_step = cb_tbb_device_ble_deinit;
                return net_ble_succeed;
            }
        }break;
    }

    return net_ble_wait;
}
uint8_t network_4g_ble_communication_close()
{
    uint8_t b_result = net_ble_wait;

    if(net_ble_close != b_net_ble_state)
    {
        b_result = net_4g_ble_establish_ble_deinit();
        if(net_ble_succeed == b_result)
        {
            b_net_ble_state = net_ble_close;
        }
    }
    return TRUE;
}

uint8_t network_4g_ble_communication_with_app()
{

    #define cb_ble_initial                                                  0
    #define cb_ble_lost_connect                                             1
    #define cb_tbb_device_ble_receive_message                               2

    if(auto_disconnect_state == 1)
    {
        auto_disconnect_cnt+=50;
    }

    if(auto_disconnect_cnt > 39000)
    {
        auto_disconnect_cnt = 0;
        auto_disconnect_state = 0;
        b_net_ble_state = net_ble_disconnect;
    }

    static uint8_t b_tbb_device_ble_com_step = cb_ble_initial;
    uint8_t b_result = net_ble_wait;
    if(cb_ble_initial != b_tbb_device_ble_com_step)
    {
        if(net_ble_disconnect == b_net_ble_state)
        {
            b_tbb_device_ble_com_step = cb_ble_lost_connect;
        }
        else if((net_ble_close == b_net_ble_state) ||\
                (net_ble_reset == b_net_ble_state))
        {
            b_tbb_device_ble_com_step = cb_ble_initial;
        }
    }
    switch(b_tbb_device_ble_com_step)
    {
        case cb_ble_initial:
        {
            if(f_module_init_succeed)
            {
                b_result = net_4g_ble_establish_ble_init();

                if(net_ble_succeed == b_result)
                {
                    //DebugPrintf(TRUE,"ble_init_success\n");
                    b_net_ble_state = net_ble_reconnect;
                    b_tbb_device_ble_com_step = cb_tbb_device_ble_receive_message;
                }
            }
        }break;
        case cb_ble_lost_connect:
        {
            //DebugPrintf(TRUE,"ble_open_broadcast\n");
            static TickType_t x_start_time = 0;
            TickType_t x_curent_time = 0;

            x_curent_time = xTaskGetTickCount();
            if(x_curent_time - x_start_time >= pdMS_TO_TICKS_USER(5000))
            {
                x_start_time = xTaskGetTickCount();
                b_result = net_4g_ble_open_broadcast();

                if(net_ble_succeed == b_result)
                {
                    b_net_ble_state = net_ble_reconnect;
                    b_tbb_device_ble_com_step = cb_tbb_device_ble_receive_message;
                }
            }
        }break;
        case cb_tbb_device_ble_receive_message:
        {
            //DebugPrintf(TRUE,"ble_receive\n");
            net_ble_receive_message();
        }break;
        default:
        {
        }break;
    }
    return TRUE;
}






