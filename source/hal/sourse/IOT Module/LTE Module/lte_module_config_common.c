#include "lte_module_config_common.h"
#include "msg_queue_operate.h"
#include "freertos_user_config.h"
#include "FreeRTOS.h"
#include "task.h"
#include <string.h>
#include <stdlib.h>

#include "lte_module_data_queue.h"
#include "network_ble_communication_queue.h"
#include "printf_custom.h"
#include "lte_module.h"
#include "lte_4g_module.h"

#define lte_module_driver_send_buffer_size	1000

uint8_t ba_lte_module_send_buff[lte_module_driver_send_buffer_size] = {0,};
lte_module_data_info st_lte_module_data_info = {.pb_data = ba_lte_module_send_buff};
uint8_t ba_esim_card_iccid[50] = {0,};

uint8_t lte_module_send_data_foreground(lte_module_data_info *pst_lte_module_data_info)
{
    return lte_module_tx_data_queue_send(pst_lte_module_data_info);
}

uint8_t lte_module_receive_data_foreground_peek(lte_module_data_info **pst_lte_module_data_info)
{
    if(TRUE == lte_module_rx_data_queue_receive(pst_lte_module_data_info,FALSE))
    {
        if((NULL == *pst_lte_module_data_info) ||\
           (NULL == (*pst_lte_module_data_info)->pb_data))
        {
            if((*pst_lte_module_data_info)->pb_data)
            {
                vPortFree((*pst_lte_module_data_info)->pb_data);
                #if(dynamic_memory_request_test)
                DebugPrintf(TRUE,"lte_module_receive_data_foreground_peek_0_free%x\n",(*pst_lte_module_data_info)->pb_data);
                #endif
            }
            if(pst_lte_module_data_info)
            {
                vPortFree(*pst_lte_module_data_info);
                #if(dynamic_memory_request_test)
                DebugPrintf(TRUE,"lte_module_receive_data_foreground_peek_1_free%x\n",pst_lte_module_data_info);
                #endif
            }
            return FALSE;
        }
        return TRUE;
    }

    return FALSE;
}
void lte_module_receive_data_foreground_confirm()
{
    lte_module_rx_data_queue_shift();
}

uint8_t lte_module_receive_data_foreground(lte_module_data_info **pst_lte_module_data_info)
{
    if(lte_module_rx_data_queue_receive(pst_lte_module_data_info,TRUE))
    {
        if((NULL == *pst_lte_module_data_info) ||\
           (NULL == (*pst_lte_module_data_info)->pb_data))
        {
            if((*pst_lte_module_data_info)->pb_data)
            {
                vPortFree((*pst_lte_module_data_info)->pb_data);
                #if(dynamic_memory_request_test)
                DebugPrintf(TRUE,"lte_module_receive_data_foreground_0_free%x\n",(*pst_lte_module_data_info)->pb_data);
                #endif
            }
            if(pst_lte_module_data_info)
            {
                vPortFree(*pst_lte_module_data_info);
                #if(dynamic_memory_request_test)
                DebugPrintf(TRUE,"lte_module_receive_data_foreground_1_free%x\n",pst_lte_module_data_info);
                #endif
            }
            return FALSE;
        }
        return TRUE;
    }

    return FALSE;
}
/*
uint8_t lte_module_send_data_background()
{
//    if(FALSE == lte_module_connect_check())
//    {
//        return FALSE;
//    }

    lte_module_data_info *pst_lte_module_data_info = 0;

    if(TRUE == lte_module_tx_data_queue_receive((pv_queue_operate_msg *)&pst_lte_module_data_info,TRUE))
    {
		#if(lte_tranmist_data_test)
//        DebugPrintf(TRUE,"LTE At Port Tx Len: %d\n",pst_lte_module_data_info->w_data_len);
        if(pst_lte_module_data_info && pst_lte_module_data_info->pb_data)
        {
            {
//                DebugPrintf(TRUE,"LTE At Port Tx Data:\n");
                for(uint16_t i = 0;i < pst_lte_module_data_info->w_data_len;i++)
                {
                    DebugPrintf(TRUE,"%c",pst_lte_module_data_info->pb_data[i]);
                }
                DebugPrintf(TRUE,"\n");
                for(uint16_t i = 0;i < pst_lte_module_data_info->w_data_len;i++)
                {
                    DebugPrintf(TRUE,"%02X ",pst_lte_module_data_info->pb_data[i]);
                }
                DebugPrintf(TRUE,"\n");
            }
        }
		#endif
//        DebugPrintf(TRUE,"lte_module_send_data_background_1_free%x\n",pst_lte_module_data_info->pb_data);

		usart_lte_transmit_trig(pst_lte_module_data_info->w_data_len,pst_lte_module_data_info->pb_data);

//        vTaskDelay(pdMS_TO_TICKS_USER(50));

        if(pst_lte_module_data_info->pb_data)
        {
            vPortFree(pst_lte_module_data_info->pb_data);
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"lte_module_send_data_background_0_free%x\n",pst_lte_module_data_info->pb_data);
            #endif
        }

        if(pst_lte_module_data_info)
        {
            vPortFree(pst_lte_module_data_info);
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"lte_module_send_data_background_1_free%x\n",pst_lte_module_data_info);
            #endif
        }

    }

	 return TRUE;
}
*/
//void lte_module_send_queue(lte_module_data_info *pst_lte_module_data_info)
//{
//    lte_module_data_info *pst_lte_module_ble_data_info = 0;
//    lte_module_data_info *pst_lte_module_other_data_info = 0;
//    uint8_t* pb_data = 0;
//    pb_data = (uint8_t *)strstr((char*)pst_lte_module_data_info->pb_data,"\r\n")+2;
//
//    do{
//        pst_lte_module_ble_data_info = (lte_module_data_info *)pvPortMalloc(sizeof(lte_module_data_info));
//    }
//    while(NULL == pst_lte_module_ble_data_info);
//    #if(dynamic_memory_request_test)
//    DebugPrintf(TRUE,"lte_module_send_queue_0_malloc%x\n",v);
//    #endif
//    memset((uint8_t *)pst_lte_module_ble_data_info,0,sizeof(lte_module_data_info));
//
//    do{
//        pst_lte_module_ble_data_info->pb_data = (uint8_t *)pvPortMalloc(lte_module_mps_size * sizeof(uint8_t));
//    }while(NULL == pst_lte_module_ble_data_info->pb_data);
//    #if(dynamic_memory_request_test)
//    DebugPrintf(TRUE,"lte_module_send_queue_1_malloc%x\n",pst_lte_module_ble_data_info->pb_data);
//    #endif
//
//    do{
//        pst_lte_module_other_data_info = (lte_module_data_info *)pvPortMalloc(sizeof(lte_module_data_info));
//    }
//    while(NULL == pst_lte_module_other_data_info);
//    #if(dynamic_memory_request_test)
//    DebugPrintf(TRUE,"lte_module_send_queue_3_malloc%x\n",v);
//    #endif
//    memset((uint8_t *)pst_lte_module_other_data_info,0,sizeof(lte_module_data_info));
//
//    do{
//        pst_lte_module_other_data_info->pb_data = (uint8_t *)pvPortMalloc(lte_module_mps_size * sizeof(uint8_t));
//    }while(NULL == pst_lte_module_other_data_info->pb_data);
//    #if(dynamic_memory_request_test)
//    DebugPrintf(TRUE,"lte_module_send_queue_4_malloc%x\n",pst_lte_module_other_data_info->pb_data);
//    #endif
//    memset((uint8_t *)pst_lte_module_other_data_info->pb_data,0,lte_module_mps_size);
//
//    if(strstr((char*)pst_lte_module_data_info->pb_data,"+BLE")||
//       strstr((char*)pst_lte_module_data_info->pb_data,"+WRITE:"))
//    {
//        pst_lte_module_ble_data_info->w_data_len=pb_data-pst_lte_module_data_info->pb_data;
//        memcpy(pst_lte_module_ble_data_info->pb_data,pst_lte_module_data_info->pb_data,pst_lte_module_ble_data_info->w_data_len);
//        //DebugPrintf(TRUE,"len%d\n",pst_lte_module_data_info->w_data_len);
//        network_ble_communication_queue_send(pst_lte_module_data_info);
//        if(pst_lte_module_ble_data_info->w_data_len!=pst_lte_module_data_info->w_data_len)
//        {
//            pb_data += pst_lte_module_ble_data_info->w_data_len;
//            pst_lte_module_other_data_info->w_data_len = pb_data-pst_lte_module_data_info->pb_data;
//            memcpy(pst_lte_module_other_data_info->pb_data,pb_data,pst_lte_module_other_data_info->w_data_len);
//            lte_module_rx_data_queue_send(pst_lte_module_other_data_info);
//        }
//    }
//    else
//    {
//        lte_module_rx_data_queue_send(pst_lte_module_data_info);
//    }
//
//    vPortFree(pst_lte_module_ble_data_info->pb_data);
//    #if(dynamic_memory_request_test)
//    DebugPrintf(TRUE,"lte_module_send_queue_0_free%x\n",pst_lte_module_ble_data_info->pb_data);
//    #endif
//    vPortFree(pst_lte_module_ble_data_info);
//    #if(dynamic_memory_request_test)
//    DebugPrintf(TRUE,"lte_module_send_queue_1_free%x\n",pst_lte_module_ble_data_info);
//    #endif
//    vPortFree(pst_lte_module_other_data_info->pb_data);
//    #if(dynamic_memory_request_test)
//    DebugPrintf(TRUE,"lte_module_send_queue_0_free%x\n",pst_lte_module_other_data_info->pb_data);
//    #endif
//    vPortFree(pst_lte_module_other_data_info);
//    #if(dynamic_memory_request_test)
//    DebugPrintf(TRUE,"lte_module_send_queue_1_free%x\n",pst_lte_module_other_data_info);
//    #endif
//}


uint8_t lte_module_send_receive_data_queue(uint8_t *pb_data,uint16_t w_data_len)
{
    lte_module_data_info *pst_lte_module_data_info = 0;

    if(xPortGetFreeHeapSize() < 6000)
    {
        #if( system_alarm_print )
        DebugPrintf(TRUE,"[Alarm]Wireless receive protect\r\n");
        #endif
        yl_debug_printf("[Alarm]Wireless receive protect %d\r\n",xPortGetFreeHeapSize());

        return FALSE;
    }

//    do{
//        pst_lte_module_data_info = (lte_module_data_info *)pvPortMalloc(sizeof(lte_module_data_info));
//    }
//    while(NULL == pst_lte_module_data_info);
    pst_lte_module_data_info = (lte_module_data_info *)pvPortMalloc(sizeof(lte_module_data_info));
    if(NULL == pst_lte_module_data_info)
    {
        #if( system_alarm_print)
        DebugPrintf(TRUE,"[ERROR]Lte send queue malloc err:1\r\n");
        #endif
        return FALSE;
    }
    #if(dynamic_memory_request_test)
    DebugPrintf(TRUE,"lte_module_send_receive_data_queue_0_malloc%x\n",pst_lte_module_data_info);
    #endif
    memset((uint8_t *)pst_lte_module_data_info,0,sizeof(lte_module_data_info));

//    do{
//        pst_lte_module_data_info->pb_data = (uint8_t *)pvPortMalloc(w_data_len * sizeof(uint8_t));
//    }while(NULL == pst_lte_module_data_info->pb_data);
    pst_lte_module_data_info->pb_data = (uint8_t *)pvPortMalloc(w_data_len * sizeof(uint8_t));
    if(NULL == pst_lte_module_data_info->pb_data)
    {
        if(NULL != pst_lte_module_data_info)
        {
            vPortFree(pst_lte_module_data_info);
        }
        #if( system_alarm_print)
        DebugPrintf(TRUE,"[ERROR]Lte send queue malloc err:2\r\n");
        #endif
        yl_debug_printf("[ERROR]Lte send queue malloc err:2\r\n");
        return FALSE;
    }
    #if(dynamic_memory_request_test)
    DebugPrintf(TRUE,"lte_module_send_receive_data_queue_1_malloc%x\n",pst_lte_module_data_info->pb_data);
    #endif
    memset((uint8_t *)pst_lte_module_data_info->pb_data,0,w_data_len);

    memcpy((char *)pst_lte_module_data_info->pb_data,(char *)pb_data,w_data_len *sizeof(uint8_t));
    pst_lte_module_data_info->w_data_len = w_data_len;

    if(FALSE == lte_module_rx_data_queue_send(pst_lte_module_data_info))
    {
        vPortFree(pst_lte_module_data_info->pb_data);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"lte_module_send_receive_data_queue_0_free%x\n",pst_lte_module_data_info->pb_data);
        #endif
        vPortFree(pst_lte_module_data_info);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"lte_module_send_receive_data_queue_1_free%x\n",pst_lte_module_data_info);
        #endif
        yl_debug_printf("lte_module_send_receive_data_queue fail\r\n");
        return FALSE;
    }
    return TRUE;
}

uint8_t lte_module_send_ble_receive_data_queue(uint8_t *pb_data,uint16_t w_data_len)
{
    lte_module_data_info *pst_lte_module_data_info = 0;

    do{
        pst_lte_module_data_info = (lte_module_data_info *)pvPortMalloc(sizeof(lte_module_data_info));
        vTaskDelay(0);
    }
    while(NULL == pst_lte_module_data_info);
    #if(dynamic_memory_request_test)
    DebugPrintf(TRUE,"lte_module_send_ble_receive_data_queue_0_malloc%x\n",pst_lte_module_data_info);
    #endif
    memset((uint8_t *)pst_lte_module_data_info,0,sizeof(lte_module_data_info));

    do{
        pst_lte_module_data_info->pb_data = (uint8_t *)pvPortMalloc(w_data_len * sizeof(uint8_t));
        vTaskDelay(0);
    }while(NULL == pst_lte_module_data_info->pb_data);
    #if(dynamic_memory_request_test)
    DebugPrintf(TRUE,"lte_module_send_ble_receive_data_queue_1_malloc%x\n",pst_lte_module_data_info->pb_data);
    #endif
    memset((uint8_t *)pst_lte_module_data_info->pb_data,0,w_data_len);

    memcpy((char *)pst_lte_module_data_info->pb_data,(char *)pb_data,w_data_len *sizeof(uint8_t));
    pst_lte_module_data_info->w_data_len = w_data_len;

    if(FALSE == network_ble_communication_queue_send(pst_lte_module_data_info))
    {
        yl_debug_printf("ble communication queue send fail\r\n");
        vPortFree(pst_lte_module_data_info->pb_data);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"lte_module_send_ble_receive_data_queue_0_free%x\n",pst_lte_module_data_info->pb_data);
        #endif
        vPortFree(pst_lte_module_data_info);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"lte_module_send_ble_receive_data_queue_1_free%x\n",pst_lte_module_data_info);
        #endif
        return FALSE;
    }
    return TRUE;
}
void lte_module_send_queue(uint8_t *pb_data_in,uint16_t w_data_len_in)
{
    uint16_t w_data_index = 0;
    uint16_t w_data_index_back_up = 0;

    if(NULL == pb_data_in)
    {
        return;
    }
    for(w_data_index = 0;w_data_index < w_data_len_in;w_data_index++)
    {
        if(('\r' == pb_data_in[w_data_index]) &&\
            ('\n' == pb_data_in[w_data_index + 1]) &&\
            w_data_index != w_data_index_back_up && w_data_index > 2)
        {
            w_data_index += 2;
            if('+' == pb_data_in[w_data_index_back_up])
            {
                if(0 == memcmp((char *)&pb_data_in[w_data_index],(char *)"\r\nOK\r\n",6))
                {
                    w_data_index += 6;
                }
                else if(0 == memcmp((char *)&pb_data_in[w_data_index],(char *)"OK\r\n",4))
                {
                    w_data_index += 4;
                }

                if(strstr((char*)&pb_data_in[w_data_index_back_up],"+BLE"))
                {
                    if(strstr((char*)&pb_data_in[w_data_index_back_up],"+BLEADDR"))
                    {
                        #if( lte_tranmist_data_test )
                        DebugPrintf(TRUE,"lte_module_send_receive_data_queue_1:%d\n",w_data_index - w_data_index_back_up);
                        #endif
                        lte_module_send_receive_data_queue(&pb_data_in[w_data_index_back_up],
                                                           (w_data_index - w_data_index_back_up));
                    }
                    else
                    {
						#if( lte_tranmist_data_test )
                        DebugPrintf(TRUE,"lte_module_send_receive_data_queue_2:%d\n",w_data_index - w_data_index_back_up);
						#endif
                        lte_module_send_ble_receive_data_queue(&pb_data_in[w_data_index_back_up],
                                                               (w_data_index - w_data_index_back_up));
                    }
                }
                else if(strstr((char*)&pb_data_in[w_data_index_back_up],"+WRITE:"))			//蓝牙数据接收
                {
					#if( lte_tranmist_data_test )
                    DebugPrintf(TRUE,"lte_module_send_receive_data_queue_2:%d\n",w_data_index - w_data_index_back_up);
					#endif
                    lte_module_send_ble_receive_data_queue(&pb_data_in[w_data_index_back_up],
                                                           (w_data_index - w_data_index_back_up));
                }
                else				//AT指令接收包括wifi 、 ble
                {
					#if( lte_tranmist_data_test )
                    DebugPrintf(TRUE,"lte_module_send_receive_data_queue_1:%d\n",w_data_index - w_data_index_back_up);
					#endif
                    lte_module_send_receive_data_queue(&pb_data_in[w_data_index_back_up],
                                                       (w_data_index - w_data_index_back_up));
                }
            }
			else if(strstr((char*)&pb_data_in[w_data_index_back_up],"+QIRD:"))
            {
                lte_module_send_receive_data_queue(&pb_data_in[w_data_index_back_up],
                                   					w_data_len_in);
			}
            else
            {
				#if( lte_tranmist_data_test )
                DebugPrintf(TRUE,"lte_module_send_receive_data_queue_1:%d\n",w_data_index - w_data_index_back_up);
				#endif
                lte_module_send_receive_data_queue(&pb_data_in[w_data_index_back_up],
                                                   (w_data_index - w_data_index_back_up));
            }
            w_data_index_back_up = w_data_index;
        }
    }
    if((1 != (w_data_index - w_data_index_back_up)) ||\
       (w_data_len_in - w_data_index_back_up != 0))
    {
		#if( lte_tranmist_data_test )
        DebugPrintf(TRUE,"lte_module_send_receive_data_queue_1:%d\n",w_data_index - w_data_index_back_up);
		#endif
        lte_module_send_receive_data_queue(&pb_data_in[w_data_index_back_up],
                                           (w_data_index - w_data_index_back_up));
    }
}
uint8_t lte_module_receive_data_background()
{
    static uint8_t ba_lte_module_receive_buff[lte_module_mps_size] = {0,};
    uint16_t w_data_len = 0;

    if(TRUE == uart_lte_receive_data(ba_lte_module_receive_buff,
                                     &w_data_len,
                                     lte_module_mps_size))
    {
		#if(lte_tranmist_data_test)
        {
            DebugPrintf(TRUE,"At Port Rx Len2: %d\n",w_data_len);
            DebugPrintf(TRUE,"At Port Rx Data2:\n");
            for(uint16_t i = 0;i < w_data_len;i++)
            {
                DebugPrintf(TRUE,"%c",ba_lte_module_receive_buff[i]);
            }
            DebugPrintf(TRUE,"\n");
            for(uint16_t i = 0;i < w_data_len;i++)
            {
                DebugPrintf(TRUE,"%02X ",ba_lte_module_receive_buff[i]);
            }
            DebugPrintf(TRUE,"\n");
        }
		#endif

        #if (local_offline_ble_print)
        TickType_t tick  = xTaskGetTickCount();
        yl_debug_printf("[%02d:%02d:%02d:%03d]Rx\nLen: %d\n",
                        (tick / (1000 * 60 * 60)) % 24,
                        (tick / (1000 * 60)) % 60,
                        (tick / 1000) % 60,
                        tick % 1000,
                        w_data_len);
        yl_debug_printf("ASCII:  ");
        for(uint16_t i = 0;i < w_data_len;i++)
        {
            // 将换行符改为空格
            if(('\r' == ba_lte_module_receive_buff[i]) ||\
               ('\n' == ba_lte_module_receive_buff[i]))
            {
                yl_debug_printf(" ");
                continue;
            }
            yl_debug_printf("%c",ba_lte_module_receive_buff[i]);
        }
        yl_debug_printf("\n");
        yl_debug_printf("HEX:  ");
        for(uint16_t i = 0;i < w_data_len;i++)
        {
            yl_debug_printf("%02X ",ba_lte_module_receive_buff[i]);
        }
        yl_debug_printf("\n");
        yl_debug_printf("\n");
        #endif

        lte_module_send_queue(ba_lte_module_receive_buff,w_data_len);
    }
//    else
//    {
//        yl_debug_printf("[ERROR]usart receive esp32 data false!\n");
//    }

	return TRUE;
}


/**
 * @brief 网络模块驱动AT指令发送接口
 * @param {uint16_t} w_receive_timeout
 * @param {void} *pv_user_data
 * @param {pfunc_lte_module_driver_cmd_str_notify} pfunc_cmd_str_receive_notify
 * @param {pfunc_lte_module_driver_cmd_str_send} pfunc_cmd_str_send
 * @param {pfunc_lte_module_driver_cmd_str_receive} pfunc_cmd_str_receive
 * @return {*}
 * @author Yao YiLiang
 */
#if(netwrok_status_print)
uint8_t lte_rx_cache[50][100]={0,};
uint8_t lte_rx_cache_flag = 0;
uint8_t lte_rx_cache_indx = 0;
#endif

extern uint8_t ftp_updata_task_start_flag;
extern CirQueue st_lte_module_rx_data_queue;
extern SemaphoreHandle_t  pst_lte_module_rx_data_mutex;
void ftp_updata_task_start(void);
TickType_t lte_system_tick = 0;
uint8_t lte_module_driver_at_cmd_transmit(uint16_t w_receive_timeout,void *pv_user_data,
                                          pfunc_lte_module_driver_cmd_str_notify pfunc_cmd_str_receive_notify,
										  pfunc_lte_module_driver_cmd_str_send pfunc_cmd_str_send,
									      pfunc_lte_module_driver_cmd_str_receive pfunc_cmd_str_receive)
{
    lte_system_tick  = xTaskGetTickCount();
	lte_module_data_info *pst_lte_module_data_info = 0;

    if(pfunc_cmd_str_send)
    {
        pst_lte_module_data_info = (lte_module_data_info *)&st_lte_module_data_info;
        if(FALSE == pfunc_cmd_str_send(pv_user_data,pst_lte_module_data_info))
        {
            return 0;
        }
        #if(netwrok_status_print)
		if(1 == lte_rx_cache_flag)
		{
			lte_rx_cache_flag = 0;
			memset(lte_rx_cache,0,sizeof(lte_rx_cache));
			lte_rx_cache_indx = 0;
		}

		if (pst_lte_module_data_info && pst_lte_module_data_info->pb_data)
		{
			if (0 != pst_lte_module_data_info->w_data_len)
			{
				DebugPrintf(TRUE, "LTE At Port Tx Data:\r");
				for (uint16_t i = 0; i < pst_lte_module_data_info->w_data_len; i++)
				{
					DebugPrintf(TRUE, "%c", pst_lte_module_data_info->pb_data[i]);
				}
				for (uint16_t i = 0; i < pst_lte_module_data_info->w_data_len; i++)
				{
					DebugPrintf(TRUE, "%02X ", pst_lte_module_data_info->pb_data[i]);
				}
				DebugPrintf(TRUE, "\n");
				for(uint16_t i=0;i<pst_lte_module_data_info->w_data_len&i<199;i++)
				{
					lte_rx_cache[lte_rx_cache_indx][i] = pst_lte_module_data_info->pb_data[i];
				}
				if(lte_rx_cache_indx < 50)
				{
					lte_rx_cache_indx ++;
				}
                else
                {
                    lte_rx_cache_flag = 1;
                }
			}
		}
		#endif

#if(lte_tranmist_data_test)
//        DebugPrintf(TRUE,"LTE At Port Tx Len: %d\n",pst_lte_module_data_info->w_data_len);
        if(pst_lte_module_data_info && pst_lte_module_data_info->pb_data)
        {
            if(0 != pst_lte_module_data_info->w_data_len)
            {
//                DebugPrintf(TRUE,"LTE At Port Tx Data:\n");
                for(uint16_t i = 0;i < pst_lte_module_data_info->w_data_len;i++)
                {
                    DebugPrintf(TRUE,"%c",pst_lte_module_data_info->pb_data[i]);
                }
                DebugPrintf(TRUE,"\n");
                for(uint16_t i = 0;i < pst_lte_module_data_info->w_data_len;i++)
                {
                    DebugPrintf(TRUE,"%02X ",pst_lte_module_data_info->pb_data[i]);
                }
                DebugPrintf(TRUE,"\n");
            }
        }
		#endif

        #if (local_offline_ble_print)
        if(pst_lte_module_data_info && pst_lte_module_data_info->pb_data)
        {
            if(0 != pst_lte_module_data_info->w_data_len)
            {
                TickType_t tick  = xTaskGetTickCount();
                yl_debug_printf("[%02d:%02d:%02d:%03d]Tx\nLen: %d\n",
                                (tick / (1000 * 60 * 60)) % 24,
                                (tick / (1000 * 60)) % 60,
                                (tick / 1000) % 60,
                                tick % 1000,
                                pst_lte_module_data_info->w_data_len);
                yl_debug_printf("ASCII:  ");
                for(uint16_t i = 0;i < pst_lte_module_data_info->w_data_len;i++)
                {
                    // 跳过换行符
                    if(('\r' == pst_lte_module_data_info->pb_data[i]) ||\
                    ('\n' == pst_lte_module_data_info->pb_data[i]))
                    {
                        yl_debug_printf(" ");
                        continue;
                    }
                    yl_debug_printf("%c",pst_lte_module_data_info->pb_data[i]);
                }
                yl_debug_printf("\n");
                yl_debug_printf("HEX:  ");
                for(uint16_t i = 0;i < pst_lte_module_data_info->w_data_len;i++)
                {
                    yl_debug_printf("%02X ",pst_lte_module_data_info->pb_data[i]);
                }
                yl_debug_printf("\n");
                yl_debug_printf("\n");
            }
        }
        #endif

		usart_lte_transmit_trig(pst_lte_module_data_info->w_data_len,pst_lte_module_data_info->pb_data);

        if(ftp_updata_task_start_flag == 1 && pst_lte_module_data_info->pb_data[0] == 0xFE)
        {
            ftp_updata_task_start_flag = 0;
            ftp_updata_task_start();
        }
    }

    if(pfunc_cmd_str_receive || pfunc_cmd_str_receive_notify)
    {
        for(;;)
        {
            {
                TickType_t x_start_time = 0;
                TickType_t x_curent_time = 0;

                x_start_time = xTaskGetTickCount();

                for(;;)//at指令队列发生完之后这里等接收
                {
                     if(TRUE == lte_module_receive_data_foreground_peek((lte_module_data_info **)&pst_lte_module_data_info))
                     {
                         break;
                     }

                     x_curent_time = xTaskGetTickCount();

                     if((x_curent_time - x_start_time) > pdMS_TO_TICKS_USER(w_receive_timeout))
                     {
                         return 0;
                     }

                     vTaskDelay(0);
                }
            }

            if((NULL == pst_lte_module_data_info) || (NULL == pst_lte_module_data_info->pb_data))
            {
                if(pst_lte_module_data_info->pb_data)
                {
                    vPortFree(pst_lte_module_data_info->pb_data);
                    #if(dynamic_memory_request_test)
                    DebugPrintf(TRUE,"lte_module_sim_card_command_mode_switch_analysis_0_free%x\n",pst_lte_module_data_info->pb_data);
                    #endif
                }

                if(pst_lte_module_data_info)
                {
                    vPortFree(pst_lte_module_data_info);
                    #if(dynamic_memory_request_test)
                    DebugPrintf(TRUE,"lte_module_sim_card_command_mode_switch_analysis_1_free%x\n",pst_lte_module_data_info);
                    #endif
                }

                continue;
            }

            {
                uint8_t b_result = 3;

                #define lte_module_response_confirm                 0
                #define lte_module_notify_confirm                   1
                #define lte_module_receive_result                   2
                #define lte_module_receive_confirm_none             3
                uint8_t b_lte_module_receive_confirm_step_last = lte_module_notify_confirm;
                uint8_t b_lte_module_receive_confirm_step = lte_module_notify_confirm;

                do
                {
                    switch(b_lte_module_receive_confirm_step)
                    {
                        case lte_module_response_confirm:
                        {
                            if(pfunc_cmd_str_receive)
                            {
                                b_result = pfunc_cmd_str_receive(pv_user_data,pst_lte_module_data_info);
                                b_lte_module_receive_confirm_step_last = lte_module_response_confirm;
                                b_lte_module_receive_confirm_step = lte_module_receive_result;
                            }
                            else
                            {
                                b_result = 0;
                                b_lte_module_receive_confirm_step_last = lte_module_response_confirm;
                                b_lte_module_receive_confirm_step = lte_module_receive_result;
                            }
                        }break;
                        case lte_module_notify_confirm:
                        {
                            if(pfunc_cmd_str_receive_notify)
                            {
                                b_result = pfunc_cmd_str_receive_notify(pv_user_data,pst_lte_module_data_info);
                                b_lte_module_receive_confirm_step_last = lte_module_notify_confirm;
                                b_lte_module_receive_confirm_step = lte_module_receive_result;
                            }
                            else
                            {
                                b_result = 3;
                                b_lte_module_receive_confirm_step_last = lte_module_notify_confirm;
                                b_lte_module_receive_confirm_step = lte_module_receive_result;
                            }

                        }break;
                        case lte_module_receive_result:
                        {
                            if(5 == b_result)
                            {
                                lte_module_receive_data_foreground_confirm();
                                return 1;
                            }
                            else if(4 == b_result)
                            {
                                lte_module_receive_data_foreground_confirm();
                                vPortFree(pst_lte_module_data_info->pb_data);
                                vPortFree(pst_lte_module_data_info);
                                return 2;
                            }
                            else if(3 == b_result)
                            {
                                if(lte_module_response_confirm == b_lte_module_receive_confirm_step_last)
                                {
                                    lte_module_receive_data_foreground_confirm();
                                    vPortFree(pst_lte_module_data_info->pb_data);
                                    vPortFree(pst_lte_module_data_info);
                                    b_lte_module_receive_confirm_step = lte_module_receive_confirm_none;
                                }
                                else if(lte_module_notify_confirm == b_lte_module_receive_confirm_step_last)
                                {
                                    b_lte_module_receive_confirm_step = lte_module_response_confirm;
                                }
                            }
                            else if(2 == b_result)
                            {
                                lte_module_receive_data_foreground_confirm();
                                vPortFree(pst_lte_module_data_info->pb_data);
                                vPortFree(pst_lte_module_data_info);
                                b_lte_module_receive_confirm_step = lte_module_receive_confirm_none;
                            }
                            else if(1 == b_result)
                            {
                                lte_module_receive_data_foreground_confirm();
                                vPortFree(pst_lte_module_data_info->pb_data);
                                vPortFree(pst_lte_module_data_info);
                                return 1;
                            }
                            else if(0 == b_result)
                            {
                                lte_module_receive_data_foreground_confirm();
                                vPortFree(pst_lte_module_data_info->pb_data);
                                vPortFree(pst_lte_module_data_info);
                                return 0;
                            }
                        }break;
                        case lte_module_receive_confirm_none:
                        {
                        }break;
                    }
                }while(b_lte_module_receive_confirm_step != lte_module_receive_confirm_none);

                vTaskDelay(0);
            }
        }
    }

	return 0;
}
//uint8_t lte_module_driver_at_cmd_transmit(uint16_t w_receive_timeout,void *pv_user_data,
//										  pfunc_lte_module_driver_cmd_str_send pfunc_cmd_str_send,
//									      pfunc_lte_module_driver_cmd_str_receive pfunc_cmd_str_receive,
//                                          pfunc_lte_module_driver_cmd_state_receive pfunc_cmd_state_receive)
//{
//	#define lte_module_driver_send_buffer_size	3000
//	lte_module_data_info *pst_lte_module_data_info = 0;
//
//	while(xPortGetFreeHeapSize() < 10000)
//	{
//		vTaskDelay(0);
//	}
//
//    if(pfunc_cmd_str_send)
//    {
//        //send
//        do{
//            pst_lte_module_data_info = (lte_module_data_info *)pvPortMalloc(sizeof(lte_module_data_info));
//            vTaskDelay(0);
//        }while(NULL == pst_lte_module_data_info);
//        #if(dynamic_memory_request_test)
//        DebugPrintf(TRUE,"lte_module_sim_card_command_mode_switch_0_malloc%x\n",pst_lte_module_data_info);
//        #endif
//        memset((uint8_t *)pst_lte_module_data_info,0,sizeof(lte_module_data_info));
//
//        do{
//            pst_lte_module_data_info->pb_data = (uint8_t *)pvPortMalloc(lte_module_driver_send_buffer_size * sizeof(uint8_t));
//            vTaskDelay(0);
//        }while(NULL == pst_lte_module_data_info->pb_data);
//        #if(dynamic_memory_request_test)
//        DebugPrintf(TRUE,"lte_module_sim_card_command_mode_switch_1_malloc%x\n",pst_lte_module_data_info->pb_data);
//        #endif
//        memset((uint8_t *)pst_lte_module_data_info->pb_data,0,lte_module_driver_send_buffer_size);
//
//        if(FALSE == pfunc_cmd_str_send(pv_user_data,pst_lte_module_data_info))
//        {
//            vPortFree(pst_lte_module_data_info->pb_data);
//            #if(dynamic_memory_request_test)
//            DebugPrintf(TRUE,"lte_module_sim_card_command_mode_switch_analysis_0_free%x\n",pst_lte_module_data_info->pb_data);
//            #endif
//
//            vPortFree(pst_lte_module_data_info);
//            #if(dynamic_memory_request_test)
//            DebugPrintf(TRUE,"lte_module_sim_card_command_mode_switch_analysis_1_free%x\n",pst_lte_module_data_info);
//            #endif
//            return FALSE;
//        }
//
//        for(;;)
//        {
//            if(TRUE == lte_module_send_data_foreground(pst_lte_module_data_info))
//            {
//                break;
//            }
//
//            vTaskDelay(0);
//        }
//    }
//
//    if(pfunc_cmd_str_receive)
//    {
//        for(;;)
//        {
//            {
//                TickType_t x_start_time = 0;
//                TickType_t x_curent_time = 0;
//
//                x_start_time = xTaskGetTickCount();
//
//                for(;;)
//                {
//                     if(TRUE == lte_module_receive_data_foreground((lte_module_data_info **)&pst_lte_module_data_info))
//                     {
//                         break;
//                     }
//
//                     x_curent_time = xTaskGetTickCount();
//
//                     if((x_curent_time - x_start_time) > pdMS_TO_TICKS_USER(w_receive_timeout))
//                     {
//                         return FALSE;
//                     }
//
//                     vTaskDelay(0);
//                }
//            }
//
//            if((NULL == pst_lte_module_data_info) || (NULL == pst_lte_module_data_info->pb_data))
//            {
//                if(pst_lte_module_data_info->pb_data)
//                {
//                    vPortFree(pst_lte_module_data_info->pb_data);
//                    #if(dynamic_memory_request_test)
//                    DebugPrintf(TRUE,"lte_module_sim_card_command_mode_switch_analysis_0_free%x\n",pst_lte_module_data_info->pb_data);
//                    #endif
//                }
//
//                if(pst_lte_module_data_info)
//                {
//                    vPortFree(pst_lte_module_data_info);
//                    #if(dynamic_memory_request_test)
//                    DebugPrintf(TRUE,"lte_module_sim_card_command_mode_switch_analysis_1_free%x\n",pst_lte_module_data_info);
//                    #endif
//                }
//
//                continue;
//            }
//
//
//            uint8_t b_result = 3;
//            b_result = pfunc_cmd_str_receive(pv_user_data,pst_lte_module_data_info);
//
//            vPortFree(pst_lte_module_data_info->pb_data);
//            vPortFree(pst_lte_module_data_info);
//
//            if(0 == b_result)
//            {
//                return FALSE;
//            }
//
//            if(1 == b_result)
//            {
//                break;
//            }
//
//            vTaskDelay(0);
//        }
//    }
//
//	return TRUE;
//}
