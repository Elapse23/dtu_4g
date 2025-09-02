#include "rs485_communication_transmission.h"
#include "rs485_communicate_protocal.h"
#include "application_general_macro.h"
#include <string.h>
#include "rs485.h"
#include "freertos_user_config.h"
#include "rs485_communication_tx_queue.h"
#include "net_device_data.h"
#include "device_data_attribute.h"
#include "net_device_info.h"
#include "net_system_info_access.h"
#include "net_system_info_list.h"
#include "wired_network_config_general.h"
#include "crc16_arithmetic.h"
#include "net_device_data_attribute.h"
#include "printf_custom.h"
#include "product_test.h"
#include "network_system_type_operate.h"
#include "led.h"
#include "application_general_func.h"
#include "Bootloaderapp.H"
typedef struct
{
    uint8_t b_header;
    uint8_t b_func_code_len;
    uint8_t b_func_code[10];
}rs485_header;

QueueHandle_t xrs485_cycle_data_rx_queue = 0;
SemaphoreHandle_t  pst_rs485_communicate_mutex = NULL;
SemaphoreHandle_t  pst_rs485_rx_transparent_mutex = NULL;
SemaphoreHandle_t  pst_rs485_data_polling_mutex = NULL;

static TaskHandle_t rs485_tx_data_task_handle = 0;
//static TaskHandle_t rs485_rx_data_analysis_task_handle = 0;

static void vRs485TxDataTask(void* parameter);
uint8_t rs485_tx_data_send(rs485_tx_data_info *pst_rs485_tx_data_info);

static uint8_t f_rs485_receive_transparent_enable = FALSE;

void rs485_communicate_Semaphore_creat()
{
    pst_rs485_rx_transparent_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_rs485_rx_transparent_mutex)
    {

    }
}

void rs485_data_polling_Semaphore_creat()
{
    pst_rs485_data_polling_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_rs485_data_polling_mutex)
    {

    }
}

uint8_t rs485_tx_data_foreground(uint32_t dw_id,uint8_t b_cmd,uint16_t w_rs485_cmd,uint16_t w_data_in_len,uint8_t *pb_data_in,uint8_t b_block_en)
{

    rs485_tx_data_info *pst_rs485_tx_data_info = 0;
    void *pv_data = 0;

	pst_rs485_tx_data_info = (rs485_tx_data_info *)pvPortMalloc(sizeof(rs485_tx_data_info));
	if(NULL == pst_rs485_tx_data_info)
	{
		#if( system_alarm_print)
		DebugPrintf(TRUE,"[ERROR]Rs485 tx malloc err:1\r\n");
		#endif
		return FALSE;
	}
	#if(dynamic_memory_request_test)
	DebugPrintf(TRUE,"rs485_networking_tx_data_foreground_1_malloc%x\n",pst_rs485_tx_data_info);
	#endif
	memset((uint8_t *)pst_rs485_tx_data_info,0,sizeof(rs485_tx_data_info));

	pst_rs485_tx_data_info->st_data_info.pb_data = (uint8_t *)pvPortMalloc(rs485_mps_size * sizeof(uint8_t));
	if(NULL == pst_rs485_tx_data_info->st_data_info.pb_data)
	{
		if(NULL != pst_rs485_tx_data_info)
		{
			vPortFree(pst_rs485_tx_data_info);
		}
		#if( system_alarm_print)
		DebugPrintf(TRUE,"[ERROR]Rs485 tx malloc err:2\r\n");
		#endif
		return FALSE;
	}
	#if(dynamic_memory_request_test)
	DebugPrintf(TRUE,"rs485_networking_tx_data_foreground_2_malloc%x\n",pst_rs485_tx_data_info->st_data_info.pb_data);
	#endif
	memset((uint8_t *)pst_rs485_tx_data_info->st_data_info.pb_data,0,rs485_mps_size);

	if(pst_rs485_tx_data_info->st_data_info.pb_data == 0)
	{
		DebugPrintf(TRUE,"point_default_address_fault\n");
	}
	if((cb_rs485_cmd_type_product_test != b_cmd) &&\
		(cb_rs485_cmd_type_product_test_inquire != b_cmd))
	{
		rs485_protocal_info *pst_rs485_protocal_info = NULL;
		do{
			pst_rs485_protocal_info = (rs485_protocal_info *)pvPortMalloc(sizeof(rs485_protocal_info));
		}
		while(NULL == pst_rs485_protocal_info);
		#if(dynamic_memory_request_test)
		DebugPrintf(TRUE,"rs485_networking_tx_data_foreground_3_malloc%x\n",pst_rs485_protocal_info);
		#endif
		memset((uint8_t *)pst_rs485_protocal_info,0,sizeof(rs485_protocal_info));

		
		pst_rs485_protocal_info->b_first_cmd = (uint8_t)((w_rs485_cmd & 0xFF00) >> 8);
		pst_rs485_protocal_info->b_sub_cmd = (uint8_t)(w_rs485_cmd & 0x00FF);
		pst_rs485_protocal_info->w_body_data_len = w_data_in_len;
        if(cb_rs485_cmd_type_mppt_auxiliary_installation == b_cmd)
        {
            pst_rs485_protocal_info->pb_body_data = &pb_data_in[4];
            pst_rs485_protocal_info->b_device_id = pb_data_in[1];
        }
        else
        {
            pst_rs485_protocal_info->pb_body_data = pb_data_in;
            pst_rs485_protocal_info->b_device_id = dw_id;
        }

		#if(dynamic_memory_request_test)
		DebugPrintf(TRUE,"rs485 tx foreground,id:%02X,cmd:%04X\n",dw_id,w_rs485_cmd);
		#endif
		pv_data = (void *)pst_rs485_protocal_info;
	}
	else
	{
		product_test_protocal_info *pst_product_test_protocal_info = NULL;
		do{
			pst_product_test_protocal_info =\
				(product_test_protocal_info *)pvPortMalloc(sizeof(product_test_protocal_info));
		}
		while(NULL == pst_product_test_protocal_info);
		#if(dynamic_memory_request_test)
		DebugPrintf(TRUE,"rs485_networking_tx_data_foreground_4_malloc%x\n",pst_product_test_protocal_info);
		#endif
		memset((uint8_t *)pst_product_test_protocal_info,0,sizeof(product_test_protocal_info));

		pst_product_test_protocal_info->b_com_addr = dw_id;
		if(cb_rs485_cmd_type_product_test_inquire == b_cmd)
		{
			pst_product_test_protocal_info->b_func_code = 0x00;
		}
		else
		{
			pst_product_test_protocal_info->b_func_code = 0x10;
		}
		pst_product_test_protocal_info->w_ctrl_addr = w_rs485_cmd;
		pst_product_test_protocal_info->w_body_data_len = w_data_in_len;
		pst_product_test_protocal_info->pb_body_data = pb_data_in;

		pv_data = (void *)pst_product_test_protocal_info;
	}
//	TickType_t tick  = xTaskGetTickCount();
//	yl_debug_printf("[%02d:%02d:%02d:%03d]Rx\n",
//					(tick / (1000 * 60 * 60)) % 24,
//					(tick / (1000 * 60)) % 60,
//					(tick / 1000) % 60,
//					tick % 1000);
//	yl_debug_printf("[rs485 send]send data raw_data Line is : %d \r\n",__LINE__);

	if(FALSE == send_rs485_event_raw_data(b_cmd,
											(void *)pv_data,
											&pst_rs485_tx_data_info->st_data_info.w_data_len,
											pst_rs485_tx_data_info->st_data_info.pb_data))
	{
		vPortFree(pst_rs485_tx_data_info->st_data_info.pb_data);
		#if(dynamic_memory_request_test)
		DebugPrintf(TRUE,"rs485_networking_tx_data_foreground_2_free%x\n",pst_rs485_tx_data_info->st_data_info.pb_data);
		#endif
		vPortFree(pst_rs485_tx_data_info);
		#if(dynamic_memory_request_test)
		DebugPrintf(TRUE,"rs485_networking_tx_data_foreground_3_free%x\n",pst_rs485_tx_data_info);
		#endif
        vPortFree(pv_data);
		return FALSE;
	}
	vPortFree(pv_data);
	#if(dynamic_memory_request_test)
	DebugPrintf(TRUE,"rs485_networking_tx_data_foreground_4_free%x\n",pv_data);
	#endif

	if(FALSE == rs485_tx_data_queue_send(pst_rs485_tx_data_info,b_cmd))
	{
		#if(dynamic_memory_request_test)
		DebugPrintf(TRUE,"rs485 tx queue send err\n");
		#endif
		vPortFree(pst_rs485_tx_data_info->st_data_info.pb_data);
		#if(dynamic_memory_request_test)
		DebugPrintf(TRUE,"rs485_networking_tx_data_foreground_0_free%x\n",pst_rs485_tx_data_info->st_data_info.pb_data);
		#endif
		vPortFree(pst_rs485_tx_data_info);
		#if(dynamic_memory_request_test)
		DebugPrintf(TRUE,"rs485_networking_tx_data_foreground_1_free%x\n",pst_rs485_tx_data_info);
		#endif
		return FALSE;
	}
	#if(dynamic_memory_request_test)
	DebugPrintf(TRUE,"rs485 tx queue send ok\n");
	#endif
    return TRUE;
}
void rs485_tx_data_background()
{
    rs485_tx_data_info *pst_rs485_tx_data_info = 0;

    if(TRUE == rs485_tx_data_queue_receive((pv_queue_operate_msg *)&pst_rs485_tx_data_info,FALSE))
    {
		#if(dynamic_memory_request_test)
		DebugPrintf(TRUE,"rs485 tx queue receive ok\n");
		#endif
        if((NULL == pst_rs485_tx_data_info) || (NULL == pst_rs485_tx_data_info->st_data_info.pb_data))
        {
            rs485_tx_data_queue_shift();
			if(pst_rs485_tx_data_info->st_data_info.pb_data)
			{
				vPortFree(pst_rs485_tx_data_info->st_data_info.pb_data);
                #if(dynamic_memory_request_test)
                DebugPrintf(TRUE,"rs485_tx_data_background_0_free%x\n",pst_rs485_tx_data_info->st_data_info.pb_data);
                #endif
			}

			if(pst_rs485_tx_data_info)
			{
				vPortFree(pst_rs485_tx_data_info);
                #if(dynamic_memory_request_test)
                DebugPrintf(TRUE,"rs485_tx_data_background_1_free%x\n",pst_rs485_tx_data_info);
                #endif
			}
            return;
        }
		{
			uint8_t b_result = FALSE;

			b_result = rs485_tx_data_send(pst_rs485_tx_data_info);

			if(TRUE == b_result)
			{
				#if(dynamic_memory_request_test)
				for(uint8_t i = 0;i < pst_rs485_tx_data_info->st_data_info.w_data_len;i++)
				{
					DebugPrintf(TRUE,"%x ",pst_rs485_tx_data_info->st_data_info.pb_data[i]);
				}
				DebugPrintf(TRUE,"\n");
				#endif
				#if(rs485_test)
				DebugPrintf(TRUE,"send_rs485_event_data,id: 0x%x,time: %d,data: \n",
                                 pst_rs485_tx_data_info->dw_id,
                                 xTaskGetTickCount());
				for(uint8_t i = 0;i < pst_rs485_tx_data_info->st_data_info.w_data_len;i++)
				{
					DebugPrintf(TRUE,"%x ",pst_rs485_tx_data_info->st_data_info.pb_data[i]);
				}
				DebugPrintf(TRUE,"\n");
				#endif
				rs485_tx_data_queue_shift();
				if(pst_rs485_tx_data_info->st_data_info.pb_data)
				{
					vPortFree(pst_rs485_tx_data_info->st_data_info.pb_data);
					#if(dynamic_memory_request_test)
					DebugPrintf(TRUE,"rs485_tx_data_background_0_free%x\n",pst_rs485_tx_data_info->st_data_info.pb_data);
					#endif
				}
				if(pst_rs485_tx_data_info)
				{
					vPortFree(pst_rs485_tx_data_info);
					#if(dynamic_memory_request_test)
					DebugPrintf(TRUE,"rs485_tx_data_background_1_free%x\n",pst_rs485_tx_data_info);
					#endif
				}

			}
			else
			{
				#if(dynamic_memory_request_test)
				DebugPrintf(TRUE,"rs485 tx data send err\n");
				#endif
				yl_debug_printf("rs485 tx data send err LINE is :%d \r\n",__LINE__);
				if(pst_rs485_tx_data_info->st_data_info.pb_data)
				{
					vPortFree(pst_rs485_tx_data_info->st_data_info.pb_data);
					#if(dynamic_memory_request_test)
					DebugPrintf(TRUE,"rs485_tx_data_background_0_free%x\n",pst_rs485_tx_data_info->st_data_info.pb_data);
					#endif
				}
				if(pst_rs485_tx_data_info)
				{
					vPortFree(pst_rs485_tx_data_info);
					#if(dynamic_memory_request_test)
					DebugPrintf(TRUE,"rs485_tx_data_background_1_free%x\n",pst_rs485_tx_data_info);
					#endif
				}
			}
		}
    }
}
uint8_t rs485_rx_data_check_header_format(uint8_t *pb_data_in,
                                          uint16_t w_data_in_len)
{
    if(NULL == pb_data_in)
    {
        return cb_max_protocal_num;
    }

    const rs485_header st_rs485_header[cb_max_protocal_num] =
    {
        {0x7E,0,},
        {0xF6,0,},
        {0xF7,1,{0xFF,}},
        {cbMonitorAddr,1,{0xFF,}},
        {0,1,{0xFF}},
    };
    for(uint8_t b_cnt_i = 0;b_cnt_i < cb_max_protocal_num;b_cnt_i++)
    {
        if(st_rs485_header[b_cnt_i].b_header == pb_data_in[0])
        {
//            if(w_data_in_len < st_rs485_header[b_cnt_i].b_func_code_len + 1)
//            {
//                return rs485_wait;
//            }
            for(uint8_t b_cnt_j = 0;b_cnt_j < st_rs485_header[b_cnt_i].b_func_code_len;b_cnt_j++)
            {
                if((pb_data_in[b_cnt_j + 1] != st_rs485_header[b_cnt_i].b_func_code[b_cnt_j]))
                {
                    return cb_max_protocal_num;
                }
            }
            return b_cnt_i;
        }
    }
    {
        for(uint8_t b_cnt_j = 0;b_cnt_j < st_rs485_header[rs485_update_subset_cmd_header].b_func_code_len;b_cnt_j++)
        {
            if((pb_data_in[b_cnt_j + 1] != st_rs485_header[rs485_update_subset_cmd_header].b_func_code[b_cnt_j]))
            {
                return cb_max_protocal_num;
            }
        }
        return rs485_update_subset_cmd_header;
    }
}

uint8_t rs485_data_send_queue(uint8_t *pb_data_in,
                              uint16_t w_data_in_len)
{
    if((NULL == pb_data_in) ||\
       (0 == w_data_in_len))
    {
        return FALSE;
    }

    uint8_t b_data_type = cb_max_protocal_num;
    uint8_t b_result = TRUE;
    pfunc_rs485_queue_send pfunc_rs485_queue_send_temp = NULL;
	size_t myPortGetFreeHeapSize;
	myPortGetFreeHeapSize = xPortGetFreeHeapSize();

    b_data_type = rs485_rx_data_check_header_format(pb_data_in,
                                                    w_data_in_len);
    switch(b_data_type)
    {
        case rs485_protocal_header:
            {
                pfunc_rs485_queue_send_temp = rs485_rx_polling_data_queue_send;
            }
            break;
        case product_test_header:
            {
                pfunc_rs485_queue_send_temp = rs485_rx_product_test_data_queue_send;
            }
            break;
        case rs485_firmware_update_1:
        case rs485_firmware_update_2:
            {
                pfunc_rs485_queue_send_temp = rs485_rx_local_update_data_queue_send;
            }
            break;
        case rs485_update_subset_cmd_header:
            {
                pfunc_rs485_queue_send_temp = rs485_rx_subset_update_data_queue_send;
            }
            break;
        default://mark rs485 transparent
            {
                if(NULL != pst_rs485_rx_transparent_mutex)
                {
                    #if(rx_transparent_data_Semaphore_test)
                    DebugPrintf(TRUE,"rs485_data_send_queue_SemaphoreTake task:%s\n",pcTaskGetTaskName(NULL));
                    #endif
                    xSemaphoreTake(pst_rs485_rx_transparent_mutex, 2500);
                }
                if(f_rs485_receive_transparent_enable)
                {
                    pfunc_rs485_queue_send_temp = rs485_rx_transparent_data_queue_send;
                }
                else
                {
                    b_result = FALSE;
                }
                if(NULL != pst_rs485_rx_transparent_mutex)
                {
                    #if(rx_transparent_data_Semaphore_test)
                    DebugPrintf(TRUE,"rs485_data_send_queue_SemaphoreGive task:%s\n",pcTaskGetTaskName(NULL));
                    #endif
                    xSemaphoreGive(pst_rs485_rx_transparent_mutex);
                }
            }
            break;
    }

    if((TRUE == b_result) &&\
       (NULL != pfunc_rs485_queue_send_temp))
    {
        rs485_rx_data_info *pst_rs485_rx_data_info = NULL;

        pst_rs485_rx_data_info = (rs485_rx_data_info *)pvPortMalloc(sizeof(pst_rs485_rx_data_info));
        if(NULL == pst_rs485_rx_data_info)
        {
            #if( system_alarm_print)
            DebugPrintf(TRUE,"[ERROR]Rs485 receive malloc err:1\r\n");
            #endif
            return FALSE;
        }
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"rs485_data_send_queue_malloc_1:%x\n",pst_rs485_rx_data_info);
        #endif
        memset(pst_rs485_rx_data_info,0,sizeof(rs485_rx_data_info));

        pst_rs485_rx_data_info->pb_data = (uint8_t *)pvPortMalloc(rs485_mps_size * sizeof(uint8_t));
        if(NULL == pst_rs485_rx_data_info->pb_data)
        {
            if(NULL != pst_rs485_rx_data_info)
            {
                vPortFree(pst_rs485_rx_data_info);
            }
            #if( system_alarm_print)
            DebugPrintf(TRUE,"[ERROR]Rs485 receive malloc err:2\r\n");
            #endif
            return FALSE;
        }
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"rs485_data_send_queue_malloc_2:%x\n",pst_rs485_rx_data_info->pb_data);
        #endif
        memcpy(pst_rs485_rx_data_info->pb_data,
               pb_data_in,
               w_data_in_len);
        pst_rs485_rx_data_info->w_data_len = w_data_in_len;

        if(TRUE != pfunc_rs485_queue_send_temp((pv_queue_operate_msg *)pst_rs485_rx_data_info))
        {
            b_result = FALSE;
            vPortFree(pst_rs485_rx_data_info->pb_data);
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"rs485_data_send_queue_free_2:%x\n",pst_rs485_rx_data_info->pb_data);
            #endif
            vPortFree(pst_rs485_rx_data_info);
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"rs485_data_send_queue_free_1:%x\n",pst_rs485_rx_data_info);
            #endif
        }
    }

    return b_result;
}

void rs485_rx_data_background()
{
    #if(task_runing_status_print)
    DebugPrintf(TRUE,"rs485_rx_data_background start\r\n");
    #endif
#if( RS485_USE_QUEUE)
    if(NULL == xrs485_data_rx_queue)
    {
        return;
    }
    
    uint8_t b_data_temp = 0;
    uint8_t b_result = pdFAIL;
    static uint8_t ba_rs485_rx_data_temp[rs485_mps_size] = {0,};
    uint16_t w_data_len = 0;
    

    do
    {
        b_result = xQueueReceive(xrs485_data_rx_queue,&b_data_temp,(TickType_t)dw_rs485_receive_timeout);//5 * 1byte/ms(baudrate9600)
        if((pdPASS == b_result) &&\
           (w_data_len < rs485_mps_size))
        {
            ba_rs485_rx_data_temp[w_data_len++] = b_data_temp;
            #if(rs485_send_test)
            DebugPrintf(TRUE,"rs485 receive,data:%02X,len:%d\r\n",
                        ba_rs485_rx_data_temp[w_data_len - 1],
                        w_data_len);
            #endif
        }
        else
        {
            if(0 != w_data_len)
            {
                {
                    #if(rs485_send_test)
                    DebugPrintf(TRUE,"rs485 receive,len:%d\r\n",
                                w_data_len);
                    #endif

                    rs485_data_send_queue(ba_rs485_rx_data_temp,w_data_len);
                }
            }
            break;
        }
        vTaskDelay(0);
    }while(pdPASS == b_result);
#else
    static TickType_t x_start_time = 0;
    TickType_t x_curent_time = 0;
    uint16_t rs485_rx_data_len_backup = 0;
    
    if(0 == w_rs485_rx_data_len)
    {
        x_start_time = xTaskGetTickCount();
        return;
    }
    while(0 != w_rs485_rx_data_len)
    {
        if(rs485_rx_data_len_backup == w_rs485_rx_data_len)
        {
            x_curent_time = xTaskGetTickCount();
            if((x_curent_time - x_start_time) >= pdMS_TO_TICKS_USER(dw_rs485_receive_timeout))
            {
                #if(rs485_send_test)
                DebugPrintf(TRUE,"rs485 receive,len:%d\r\n",
                            w_rs485_rx_data_len);
                #endif
                rs485_data_send_queue(ba_rs485_rx_buff,w_rs485_rx_data_len);
                w_rs485_rx_data_len = 0;
            }
        }
        else
        {
            #if(rs485_send_test)
            DebugPrintf(TRUE,"rs485 receive,data:%02X,len:%d\r\n",
                        ba_rs485_rx_buff[w_rs485_rx_data_len - 1],
                        w_rs485_rx_data_len);
            #endif
            x_start_time = xTaskGetTickCount();
            rs485_rx_data_len_backup = w_rs485_rx_data_len;
        }
        vTaskDelay(0);
    }
#endif

    #if(task_runing_status_print)
    DebugPrintf(TRUE,"rs485_rx_data_background end\r\n");
    #endif
}

uint8_t rs485_data_write(uint32_t *pdw_device_id,
                         uint8_t *pb_data_in,
                         uint16_t w_data_len,
                         void *pv_data)
{
	net_device_info *pst_net_device_info = 0;
	const net_data_list *pst_net_data_list = 0;
    uint8_t b_data_type_temp = 0;
    uint32_t dw_start_addr = 0;
    device_inquire_list *pst_device_inquire_list = (device_inquire_list *)pv_data;

    switch(pst_device_inquire_list->w_cmd)
    {
        case cw_device_cmd_ck_data_read             :
        case cw_device_cmd_riio_ii_data_read        :
        case cw_device_cmd_mppt_data_read           :
        case cw_device_cmd_pv_data_read             :
        case cw_device_cmd_bgk_data_read            :
        case cw_device_cmd_mppt_riio_ii_data_read   :
        case cw_device_cmd_meter_data_read          :
		case cw_device_cmd_device_software_info_read:
		case cw_device_cmd_riio_ii_split_phase_data_read:
		case cw_device_cmd_riio_ii_split_phase_meter_data_read:
        {
            b_data_type_temp = cb_net_data_att_status;
        }break;
        case cw_device_cmd_cklcd_para_read          :
        case cw_device_cmd_ck_para_read             :
        case cw_device_cmd_mppt_para_read           :
        {
            b_data_type_temp = cb_net_data_att_set;
        }break;
        default:
        {
            return FALSE;
        }break;
    }

	if(FALSE == net_get_device_info(*pdw_device_id,
									(net_device_info **)&pst_net_device_info,
									&st_net_system_info_list))
	{
		return FALSE;
	}

	if(FALSE == net_get_device_data_list(b_data_type_temp,
										 (const net_data_list **)(&pst_net_data_list),
										 pst_net_device_info))
	{
		return FALSE;
	}


    if(NULL == pst_net_data_list)
    {
        return FALSE;
    }

    if(FALSE == bytes_to_int_big_to_little_endain(pst_device_inquire_list->inquire_info.common_format.ba_data_addr,
                                                  &dw_start_addr,
                                                  2))
    {
        return FALSE;
    }

    if((cw_device_cmd_mppt_data_read == pst_device_inquire_list->w_cmd) ||\
       (cw_device_cmd_mppt_riio_ii_data_read == pst_device_inquire_list->w_cmd) ||\
       (cw_device_cmd_pv_data_read == pst_device_inquire_list->w_cmd))
    {
        uint16_t w_cnt_i = 0;
        uint8_t b_cnt_j = 0;
        const device_data_attribute *pst_device_data_attribute = 0;
        net_data_info *pst_net_data_info = 0;
        uint8_t b_store_data_len = 0;
        uint8_t b_store_data_index = 0;
        uint8_t b_surplus_cnt = 0;
        uint8_t b_device_index = 0;
        uint8_t b_max_device_num = 0;

        if(cw_device_cmd_pv_data_read == pst_device_inquire_list->w_cmd)
        {
            uint8_t b_system_protocal = 0;
            check_system_protocal_type(&b_system_protocal);
            if(cb_system_protocal_type_ck == b_system_protocal)
            {
                return FALSE;
            }
            b_max_device_num = 2;
        }
        else
        {
            b_max_device_num = 6;
        }
        for(b_device_index = 0;b_device_index < b_max_device_num;b_device_index++)
        {
            if(FALSE == net_get_device_info(*(pdw_device_id + b_device_index),
                                    (net_device_info **)&pst_net_device_info,
                                    &st_net_system_info_list))
            {
                return FALSE;
            }

            if(FALSE == net_get_device_data_list(b_data_type_temp,
                                                 (const net_data_list **)(&pst_net_data_list),
                                                 pst_net_device_info))
            {
                return FALSE;
            }


            if(NULL == pst_net_data_list)
            {
                return FALSE;
            }

            for(w_cnt_i = 0;w_cnt_i < pst_net_data_list->w_net_data_number;w_cnt_i++)
            {
                if(0 != ((net_data_info *)(pst_net_data_list->pst_net_data_info + w_cnt_i))->pst_device_data_attribute)
                {
                    pst_device_data_attribute =\
                        ((net_data_info *)(pst_net_data_list->pst_net_data_info + w_cnt_i))->pst_device_data_attribute;
                    if(pst_device_data_attribute->w_device_inquire_cmd ==\
                        pst_device_inquire_list->w_cmd)
                    {
                        if(((b_store_data_len + pst_device_data_attribute->b_len) > w_data_len) ||\
                           (b_store_data_index >= w_data_len))
                        {
                            return TRUE;
                        }
                        pst_net_data_info = ((net_data_info *)(pst_net_data_list->pst_net_data_info + w_cnt_i));
                        b_store_data_index = (pst_net_data_info->w_data_address_device - dw_start_addr)*2 + b_surplus_cnt;
                        if(cb_net_data_att_1b == (pst_net_data_list->pst_net_data_info + w_cnt_i)->pst_data_attribute->b_size)
                        {
                            for(b_cnt_j = 0;b_cnt_j < pst_device_data_attribute->b_len;b_cnt_j++)
                            {
                                pst_net_data_info->pb_data[b_cnt_j] =\
                                    pb_data_in[b_store_data_index + b_cnt_j];
                            }
                        }
                        else
                        {
                            for(b_cnt_j = 0;b_cnt_j < pst_device_data_attribute->b_len;b_cnt_j++)
                            {
                                pst_net_data_info->pb_data[pst_device_data_attribute->b_len - b_cnt_j - 1] =\
                                    pb_data_in[b_store_data_index + b_cnt_j];
                            }
                        }
                        b_store_data_len += pst_device_data_attribute->b_len;
                        b_surplus_cnt += (pst_device_data_attribute->b_len - 2);
                    }
                }
            }
        }
    }
    else
    {	
		uint16_t w_cnt_i = 0;
		uint8_t b_cnt_j = 0;
		const device_data_attribute *pst_device_data_attribute = 0;
		net_data_info *pst_net_data_info = 0;
		uint8_t b_store_data_len = 0;
		uint8_t b_store_data_index = 0;
		uint32_t dw_time_stamp = 0;
		static uint32_t dw_time_stamp_backup = 0;

		for(w_cnt_i = 0;w_cnt_i < pst_net_data_list->w_net_data_number;w_cnt_i++)
		{
			if(dw_start_addr == ((net_data_info *)(pst_net_data_list->pst_net_data_info + w_cnt_i))->w_data_address_device)
			{
				break;
			}
		}

		for(;w_cnt_i < pst_net_data_list->w_net_data_number;w_cnt_i++)
		{
			if(0 != ((net_data_info *)(pst_net_data_list->pst_net_data_info + w_cnt_i))->pst_device_data_attribute)
			{
				pst_device_data_attribute =\
					((net_data_info *)(pst_net_data_list->pst_net_data_info + w_cnt_i))->pst_device_data_attribute;
				if((pst_device_data_attribute->w_device_inquire_cmd == pst_device_inquire_list->w_cmd))
				{
					if(((b_store_data_len + pst_device_data_attribute->b_len) > w_data_len) ||\
					   (b_store_data_index >= w_data_len))
					{
						break;
					}
					pst_net_data_info = ((net_data_info *)(pst_net_data_list->pst_net_data_info + w_cnt_i));
					b_store_data_index = (pst_net_data_info->w_data_address_device - dw_start_addr)*2;
					if(dw_start_addr == 0x00D5)
					{
						dw_time_stamp = pb_data_in[b_store_data_index] << 24 | pb_data_in[b_store_data_index + 1] << 16 | pb_data_in[b_store_data_index + 2] << 8 | pb_data_in[b_store_data_index + 3];
						//yl_debug_printf("Current timestamp is : %d ,backup timestamp is : %d\r\n",dw_time_stamp,dw_time_stamp_backup);
						if(dw_time_stamp < 0x386D4380)
						{
							//yl_debug_printf("[ERROR]Current timestamp is : %d ,backup timestamp is : %d\r\n",dw_time_stamp,dw_time_stamp_backup);
							dw_time_stamp = dw_time_stamp_backup;
						}
						else
						{
							dw_time_stamp_backup = dw_time_stamp;
						}

						pst_net_data_info->pb_data[3] = (dw_time_stamp & 0xFF000000) >> 24;
						pst_net_data_info->pb_data[2] = (dw_time_stamp & 0x00FF0000) >> 16;
						pst_net_data_info->pb_data[1] = (dw_time_stamp & 0x0000FF00) >> 8;
						pst_net_data_info->pb_data[0] = dw_time_stamp & 0x000000FF;
						b_store_data_len += 4;
						return TRUE;
					}
					if(cb_net_data_att_1b == (pst_net_data_list->pst_net_data_info + w_cnt_i)->pst_data_attribute->b_size)
					{
						for(b_cnt_j = 0;b_cnt_j < pst_device_data_attribute->b_len;b_cnt_j++)
						{
							pst_net_data_info->pb_data[b_cnt_j] =\
								pb_data_in[b_store_data_index + b_cnt_j];
						}
					}
					else
					{
						for(b_cnt_j = 0;b_cnt_j < pst_device_data_attribute->b_len;b_cnt_j++)
						{
							pst_net_data_info->pb_data[pst_device_data_attribute->b_len - b_cnt_j - 1] =\
								pb_data_in[b_store_data_index + b_cnt_j];
						}
					}
					b_store_data_len += pst_device_data_attribute->b_len;
				}
			}
		}
    }
    return TRUE;

}

uint8_t rs485_queue_recognition_conversion()
{
    uint8_t b_result = FALSE;

    rs485_rx_data_info *pst_rs485_rx_data_info = NULL;
    b_result = rs485_rx_polling_data_queue_receive(( pv_queue_operate_msg * )&pst_rs485_rx_data_info, TRUE, FALSE);
    if (TRUE == b_result)
    {
        rs485_protocal_info st_rs485_protocal_info = {0};

        if ((NULL == pst_rs485_rx_data_info) || (NULL == pst_rs485_rx_data_info->pb_data))
        {
            return FALSE;
        }
        {
            uint16_t w_data_len = 0;
            uint8_t *pb_data = NULL;
            pb_data = pst_rs485_rx_data_info->pb_data;
            w_data_len = pst_rs485_rx_data_info->w_data_len;
            if (w_data_len != 0)
            {

                st_rs485_protocal_info.b_device_id = pb_data[1];
                st_rs485_protocal_info.b_first_cmd = pb_data[3];
                st_rs485_protocal_info.b_sub_cmd = pb_data[4];

                if (0x06 == ( uint16_t )st_rs485_protocal_info.b_first_cmd
                    || 0x86 == ( uint16_t )st_rs485_protocal_info.b_first_cmd)
                {
                    rs485_rx_set_data_queue_send(( pv_queue_operate_msg * )pst_rs485_rx_data_info);
                }
                else if (0x03 == ( uint16_t )st_rs485_protocal_info.b_first_cmd
                         || 0x83 == ( uint16_t )st_rs485_protocal_info.b_first_cmd)
                {
                    rs485_rx_query_data_queue_send(( pv_queue_operate_msg * )pst_rs485_rx_data_info);
                }
                else
                {
                    rs485_rx_other_data_queue_send(( pv_queue_operate_msg * )pst_rs485_rx_data_info);
                }
            }
            else
            {
                vPortFree(pst_rs485_rx_data_info->pb_data);
                vPortFree(pst_rs485_rx_data_info);
            }
        }
        return TRUE;
    }
}
uint8_t rs485_rx_query_data_analysis(uint32_t dw_device_address,   // 485 数据解析
                               uint8_t b_cmd,
                               uint16_t w_data_addr,
                               uint16_t *pw_data_out_len,
                               uint8_t *pb_data_out,
                               uint16_t w_data_out_buffer_size,
                               uint8_t *pb_receive_state,
							   TickType_t *x_start_time)

{
    uint8_t b_result = FALSE;
    rs485_rx_data_info *pst_rs485_rx_data_info = NULL;

	b_result = rs485_rx_query_data_queue_receive((pv_queue_operate_msg *)&pst_rs485_rx_data_info,TRUE,b_cmd);
	if((TRUE == rs485_rx_other_data_queue_receive((pv_queue_operate_msg *)&pst_rs485_rx_data_info,TRUE,b_cmd)) 
	 || (TRUE == rs485_rx_set_data_queue_receive((pv_queue_operate_msg *)&pst_rs485_rx_data_info,TRUE,b_cmd)))
	{
        if(pst_rs485_rx_data_info->pb_data)
        {
            vPortFree(pst_rs485_rx_data_info->pb_data);
        }
        vPortFree(pst_rs485_rx_data_info);
		yl_debug_printf("[queue receive] queue receive false Line is : %d \r\n",__LINE__);
		return TRUE;
	}

//	yl_debug_printf("[queue send]query queue receive Line is : %d \r\n",__LINE__);

    if(TRUE == b_result)
    {
        rs485_protocal_info st_rs485_protocal_info = {0};

        if((NULL == pst_rs485_rx_data_info) ||\
           (NULL == pst_rs485_rx_data_info->pb_data))
        {
            goto ANALYSIS_FAIL;
        }
        {
            uint16_t w_data_len = 0;
            uint8_t *pb_data = NULL;
            pb_data = pst_rs485_rx_data_info->pb_data;
            w_data_len = pst_rs485_rx_data_info->w_data_len;
            if(w_data_len != 0)
            {
                if(TRUE == check_crc16(pb_data,w_data_len))
                {
                    st_rs485_protocal_info.b_device_id = pb_data[1];
                    st_rs485_protocal_info.b_first_cmd = pb_data[3];
                    st_rs485_protocal_info.b_sub_cmd = pb_data[4];
                    st_rs485_protocal_info.w_body_data_len = w_data_len - cb_rs485_header_len - 2;
                    st_rs485_protocal_info.pb_body_data = &pb_data[6];

                    if((w_data_addr !=\
                         (((uint16_t)st_rs485_protocal_info.b_first_cmd << 8) |\
                          ((uint16_t)st_rs485_protocal_info.b_sub_cmd))) &&\
                       ((w_data_addr | 0x8000) !=\
                          (((uint16_t)st_rs485_protocal_info.b_first_cmd << 8) |\
                            (uint16_t)st_rs485_protocal_info.b_sub_cmd)))
                    {
						DebugPrintf(TRUE,
						            "wait id:%02X,cmd:%02X%02X,inquire_cmd:%04X,current time:%d\r\n",
						            st_rs485_protocal_info.b_device_id,
						            st_rs485_protocal_info.b_first_cmd,
						            st_rs485_protocal_info.b_sub_cmd,
						            w_data_addr,
						            xTaskGetTickCount());
						goto WAIT;
                    }
                    else if((((w_data_addr | 0x8000) ==\
                          (((uint16_t)st_rs485_protocal_info.b_first_cmd << 8) | (uint16_t)st_rs485_protocal_info.b_sub_cmd)) ||\
                        ((uint8_t)dw_device_address != st_rs485_protocal_info.b_device_id)) &&\
                       (cb_rs485_cmd_type_subset_update_pacx_dsp != b_cmd))
                    {
						#if (r485_transmission_print)
						DebugPrintf(TRUE,
						            "inv reply err quene id:%02X,quene cmd:%02X%02X,inquire_cmd:%04X,inquire_id:%02X,current time:%d\r\n",
						            st_rs485_protocal_info.b_device_id,
						            st_rs485_protocal_info.b_first_cmd,
						            st_rs485_protocal_info.b_sub_cmd,
                                    dw_device_address,
						            w_data_addr,
						            xTaskGetTickCount());
						#endif
                        goto ANALYSIS_FAIL;
                    }
                    if(st_rs485_protocal_info.w_body_data_len <= w_data_out_buffer_size)
                    {
                        *pw_data_out_len += st_rs485_protocal_info.w_body_data_len;
                        memcpy((uint8_t *)pb_data_out,
                               (uint8_t *)st_rs485_protocal_info.pb_body_data,
                               st_rs485_protocal_info.w_body_data_len);
                    }
                    else
                    {
                        *pw_data_out_len += w_data_out_buffer_size;
                        memcpy((uint8_t *)pb_data_out,
                               (uint8_t *)st_rs485_protocal_info.pb_body_data,
                               w_data_out_buffer_size);
                    }
                    goto ANALYSIS_SUCCESS;
                }
            }
            {
//				yl_debug_printf("rs485_rx_data_analysis check_crc16 fail\r\n");
				goto ANALYSIS_FAIL;
            }
        }
    }
    else
    {
        goto WAIT;
    }

    ANALYSIS_SUCCESS:
    if((NULL != pst_rs485_rx_data_info) &&\
       (TRUE == b_result))
    {
        if(pst_rs485_rx_data_info->pb_data)
        {
            vPortFree(pst_rs485_rx_data_info->pb_data);
        }
        vPortFree(pst_rs485_rx_data_info);
    }
    *x_start_time = 0;
    *pb_receive_state = cb_wired_network_receive_state_success;

    return TRUE;

    ANALYSIS_FAIL:
    if((NULL != pst_rs485_rx_data_info) &&\
       (TRUE == b_result))
    {
        if(pst_rs485_rx_data_info->pb_data)
        {
            vPortFree(pst_rs485_rx_data_info->pb_data);
        }
        vPortFree(pst_rs485_rx_data_info);
    }

    *x_start_time = 0;
    *pb_receive_state = cb_wired_network_receive_state_fail;
    return TRUE;

    WAIT:
    if(*x_start_time == 0)
    {
        *x_start_time = xTaskGetTickCount();
    }

    TickType_t start_one_delay;
    extern uint8_t start_one_flag;

    if(start_one_flag == 0)
    {
        start_one_delay = pdMS_TO_TICKS_USER(300);
    }
    else
    {
        start_one_delay = pdMS_TO_TICKS_USER(1000);
    }

    if((xTaskGetTickCount() - *x_start_time) >= start_one_delay)
    {
		#if (r485_transmission_print)
		DebugPrintf(TRUE,"rs485_rx_data_analysis timeout,id:%02X,cmd:%04X\r\n,time:%d,current time %d\r\n",
					dw_device_address,
					w_data_addr,
					xTaskGetTickCount() - *x_start_time,
					xTaskGetTickCount());
		#endif
		*x_start_time = 0;
        goto ANALYSIS_FAIL;
    }
    else
    {
        *pb_receive_state = cb_wired_network_receive_state_wait;
    }
    if ((NULL != pst_rs485_rx_data_info) && (TRUE == b_result))
    {
        vPortFree(pst_rs485_rx_data_info->pb_data);
        vPortFree(pst_rs485_rx_data_info);
    }

    return TRUE;
}

uint8_t rs485_rx_set_query_data_analysis(uint32_t dw_device_address,   // 485 数据解析
									  uint8_t b_cmd,
									  uint16_t w_data_addr,
									  uint16_t *pw_data_out_len,
									  uint8_t *pb_data_out,
									  uint16_t w_data_out_buffer_size,
									  uint8_t *pb_receive_state,
									  TickType_t *x_start_time)
							   
{
   uint8_t b_result = FALSE;
   rs485_rx_data_info *pst_rs485_rx_data_info = NULL;
   b_result = rs485_rx_set_data_queue_receive((pv_queue_operate_msg *)&pst_rs485_rx_data_info,TRUE,b_cmd);
   if((TRUE == rs485_rx_other_data_queue_receive((pv_queue_operate_msg *)&pst_rs485_rx_data_info,TRUE,b_cmd)) 
   	|| (TRUE == rs485_rx_query_data_queue_receive((pv_queue_operate_msg *)&pst_rs485_rx_data_info,TRUE,b_cmd)))
   {
	   if(pst_rs485_rx_data_info->pb_data)
	   {
		   vPortFree(pst_rs485_rx_data_info->pb_data);
	   }
	   vPortFree(pst_rs485_rx_data_info);
	   yl_debug_printf("[queue receive] queue receive false Line is : %d \r\n",__LINE__);
	   return TRUE;
   }

   if(TRUE == b_result)
   {
	   rs485_protocal_info st_rs485_protocal_info = {0};

	   if((NULL == pst_rs485_rx_data_info) ||\
		  (NULL == pst_rs485_rx_data_info->pb_data))
	   {
		   goto ANALYSIS_FAIL;
	   }
	   {
		   uint16_t w_data_len = 0;
		   uint8_t *pb_data = NULL;
		   pb_data = pst_rs485_rx_data_info->pb_data;
		   w_data_len = pst_rs485_rx_data_info->w_data_len;
		   if(w_data_len != 0)
		   {
			   if(TRUE == check_crc16(pb_data,w_data_len))
			   {
				   st_rs485_protocal_info.b_device_id = pb_data[1];
				   st_rs485_protocal_info.b_first_cmd = pb_data[3];
				   st_rs485_protocal_info.b_sub_cmd = pb_data[4];
				   st_rs485_protocal_info.w_body_data_len = w_data_len - cb_rs485_header_len - 2;
				   st_rs485_protocal_info.pb_body_data = &pb_data[6];

				   if((w_data_addr !=\
						(((uint16_t)st_rs485_protocal_info.b_first_cmd << 8) |\
						 ((uint16_t)st_rs485_protocal_info.b_sub_cmd))) &&\
					  ((w_data_addr | 0x8000) !=\
						 (((uint16_t)st_rs485_protocal_info.b_first_cmd << 8) |\
						   (uint16_t)st_rs485_protocal_info.b_sub_cmd)))
				   {
					   DebugPrintf(TRUE,
								   "wait id:%02X,cmd:%02X%02X,inquire_cmd:%04X,current time:%d\r\n",
								   st_rs485_protocal_info.b_device_id,
								   st_rs485_protocal_info.b_first_cmd,
								   st_rs485_protocal_info.b_sub_cmd,
								   w_data_addr,
								   xTaskGetTickCount());
					   goto WAIT;
				   }
				   else if((((w_data_addr | 0x8000) ==\
						 (((uint16_t)st_rs485_protocal_info.b_first_cmd << 8) | (uint16_t)st_rs485_protocal_info.b_sub_cmd)) ||\
					   ((uint8_t)dw_device_address != st_rs485_protocal_info.b_device_id)) &&\
					  (cb_rs485_cmd_type_subset_update_pacx_dsp != b_cmd && cb_rs485_cmd_type_mppt_auxiliary_installation != b_cmd))
				   {
#if (r485_transmission_print)
					   DebugPrintf(TRUE,
								   "inv reply err quene id:%02X,quene cmd:%02X%02X,inquire_cmd:%04X,inquire_id:%02X,current time:%d\r\n",
								   st_rs485_protocal_info.b_device_id,
								   st_rs485_protocal_info.b_first_cmd,
								   st_rs485_protocal_info.b_sub_cmd,
								   dw_device_address,
								   w_data_addr,
								   xTaskGetTickCount());
#endif
                       yl_debug_printf("[set queue receive] ANALYSIS_FAIL Line is : %d \r\n",__LINE__);
					   goto ANALYSIS_FAIL;
				   }
				   if(st_rs485_protocal_info.w_body_data_len <= w_data_out_buffer_size)
				   {
					   *pw_data_out_len += st_rs485_protocal_info.w_body_data_len;
					   memcpy((uint8_t *)pb_data_out,
							  (uint8_t *)st_rs485_protocal_info.pb_body_data,
							  st_rs485_protocal_info.w_body_data_len);
				   }
				   else
				   {
					   *pw_data_out_len += w_data_out_buffer_size;
					   memcpy((uint8_t *)pb_data_out,
							  (uint8_t *)st_rs485_protocal_info.pb_body_data,
							  w_data_out_buffer_size);
				   }
				   goto ANALYSIS_SUCCESS;
			   }
		   }
		   {
//			   yl_debug_printf("rs485_rx_data_analysis check_crc16 fail\r\n");
			   goto ANALYSIS_FAIL;
		   }
	   }
   }
   else
   {
	   goto WAIT;
   }

   ANALYSIS_SUCCESS:
   if((NULL != pst_rs485_rx_data_info) &&\
	  (TRUE == b_result))
   {
	   if(pst_rs485_rx_data_info->pb_data)
	   {
		   vPortFree(pst_rs485_rx_data_info->pb_data);
	   }
	   vPortFree(pst_rs485_rx_data_info);
   }
   *x_start_time = 0;
   *pb_receive_state = cb_wired_network_receive_state_success;

   return TRUE;

   ANALYSIS_FAIL:
   if((NULL != pst_rs485_rx_data_info) &&\
	  (TRUE == b_result))
   {
	   if(pst_rs485_rx_data_info->pb_data)
	   {
		   vPortFree(pst_rs485_rx_data_info->pb_data);
	   }
	   vPortFree(pst_rs485_rx_data_info);
   }

   *x_start_time = 0;
   *pb_receive_state = cb_wired_network_receive_state_fail;
   return TRUE;

   WAIT:
   if(*x_start_time == 0)
   {
	   *x_start_time = xTaskGetTickCount();
   }

   TickType_t start_one_delay;
   extern uint8_t start_one_flag;

   if(start_one_flag == 0)
   {
	   start_one_delay = pdMS_TO_TICKS_USER(300);
   }
   else
   {
	   start_one_delay = pdMS_TO_TICKS_USER(1000);
   }
   if(b_cmd == cb_rs485_cmd_type_mppt_auxiliary_installation || b_cmd == cb_rs485_cmd_type_inv_auxiliary_installation)
   {
   		start_one_delay = pdMS_TO_TICKS_USER(3000);
   }

   if((xTaskGetTickCount() - *x_start_time) >= start_one_delay)
   {
#if (r485_transmission_print)
	   DebugPrintf(TRUE,"rs485_rx_data_analysis timeout,id:%02X,cmd:%04X\r\n,time:%d,current time %d\r\n",
				   dw_device_address,
				   w_data_addr,
				   xTaskGetTickCount() - *x_start_time,
				   xTaskGetTickCount());
#endif
	   *x_start_time = 0;
	   goto ANALYSIS_FAIL;
   }
   else
   {
	   *pb_receive_state = cb_wired_network_receive_state_wait;
   }
   if ((NULL != pst_rs485_rx_data_info) && (TRUE == b_result))
   {
	   vPortFree(pst_rs485_rx_data_info->pb_data);
	   vPortFree(pst_rs485_rx_data_info);
   }

   return TRUE;
}


uint8_t rs485_rx_data_analysis(uint32_t dw_device_address,   // 485 数据解析
                               uint8_t b_cmd,
                               uint16_t w_data_addr,
                               uint16_t *pw_data_out_len,
                               uint8_t *pb_data_out,
                               uint16_t w_data_out_buffer_size,
                               uint8_t *pb_receive_state,
							   TickType_t *x_start_time)
{
    uint8_t b_result = FALSE;
    rs485_rx_data_info *pst_rs485_rx_data_info = NULL;
	uint8_t b_receive_cmd = 0;
	b_receive_cmd = w_data_addr >> 8;
	b_result = rs485_rx_other_data_queue_receive((pv_queue_operate_msg *)&pst_rs485_rx_data_info,TRUE,b_cmd);
	if((TRUE == rs485_rx_set_data_queue_receive((pv_queue_operate_msg *)&pst_rs485_rx_data_info,TRUE,b_cmd)) 
	 || (TRUE == rs485_rx_query_data_queue_receive((pv_queue_operate_msg *)&pst_rs485_rx_data_info,TRUE,b_cmd)))
	{
		if(pst_rs485_rx_data_info->pb_data)
		{
			vPortFree(pst_rs485_rx_data_info->pb_data);
		}
		vPortFree(pst_rs485_rx_data_info);
		yl_debug_printf("[queue receive] queue receive false Line is : %d \r\n",__LINE__);
		return TRUE;
	}

//	yl_debug_printf("[queue send]other queue receive Line is : %d \r\n",__LINE__);

    if(TRUE == b_result)
    {
        rs485_protocal_info st_rs485_protocal_info = {0};

        if((NULL == pst_rs485_rx_data_info) ||\
           (NULL == pst_rs485_rx_data_info->pb_data))
        {
            goto ANALYSIS_FAIL;
        }
        {
            uint16_t w_data_len = 0;
            uint8_t *pb_data = NULL;
            pb_data = pst_rs485_rx_data_info->pb_data;
            w_data_len = pst_rs485_rx_data_info->w_data_len;
            if(w_data_len != 0)
            {
                if(TRUE == check_crc16(pb_data,w_data_len))
                {
                    st_rs485_protocal_info.b_device_id = pb_data[1];
                    st_rs485_protocal_info.b_first_cmd = pb_data[3];
                    st_rs485_protocal_info.b_sub_cmd = pb_data[4];
                    st_rs485_protocal_info.w_body_data_len = w_data_len - cb_rs485_header_len - 2;
                    st_rs485_protocal_info.pb_body_data = &pb_data[6];

					#if(dynamic_memory_request_test)
					DebugPrintf(TRUE,"rs485_rx_data_analysis,id:%02X,cmd:%02X%02X,inquire_cmd:%04X\r\n",
					st_rs485_protocal_info.b_device_id,
					st_rs485_protocal_info.b_first_cmd,
					st_rs485_protocal_info.b_sub_cmd,
					w_data_addr);
                    #endif

                    if((w_data_addr !=\
                         (((uint16_t)st_rs485_protocal_info.b_first_cmd << 8) |\
                          ((uint16_t)st_rs485_protocal_info.b_sub_cmd))) &&\
                       ((w_data_addr | 0x8000) !=\
                          (((uint16_t)st_rs485_protocal_info.b_first_cmd << 8) |\
                            (uint16_t)st_rs485_protocal_info.b_sub_cmd)))
                    {
						DebugPrintf(TRUE,
						            "wait id:%02X,cmd:%02X%02X,inquire_cmd:%04X,current time:%d\r\n",
						            st_rs485_protocal_info.b_device_id,
						            st_rs485_protocal_info.b_first_cmd,
						            st_rs485_protocal_info.b_sub_cmd,
						            w_data_addr,
						            xTaskGetTickCount());
						goto WAIT;
                    }
                    else if((((w_data_addr | 0x8000) ==\
                          (((uint16_t)st_rs485_protocal_info.b_first_cmd << 8) | (uint16_t)st_rs485_protocal_info.b_sub_cmd)) ||\
                        ((uint8_t)dw_device_address != st_rs485_protocal_info.b_device_id)) &&\
                       (cb_rs485_cmd_type_subset_update_pacx_dsp != b_cmd))
                    {
						#if (r485_transmission_print)
						DebugPrintf(TRUE,
						            "inv reply err quene id:%02X,quene cmd:%02X%02X,inquire_cmd:%04X,inquire_id:%02X,current time:%d\r\n",
						            st_rs485_protocal_info.b_device_id,
						            st_rs485_protocal_info.b_first_cmd,
						            st_rs485_protocal_info.b_sub_cmd,
                                    dw_device_address,
						            w_data_addr,
						            xTaskGetTickCount());
						#endif
                        goto ANALYSIS_FAIL;
                    }
					#if(dynamic_memory_request_test)
					DebugPrintf(TRUE,"rs485_rx_data_analysis success\r\n");
					#endif
                    if(st_rs485_protocal_info.w_body_data_len <= w_data_out_buffer_size)
                    {
                        *pw_data_out_len += st_rs485_protocal_info.w_body_data_len;
                        memcpy((uint8_t *)pb_data_out,
                               (uint8_t *)st_rs485_protocal_info.pb_body_data,
                               st_rs485_protocal_info.w_body_data_len);
                    }
                    else
                    {
                        *pw_data_out_len += w_data_out_buffer_size;
                        memcpy((uint8_t *)pb_data_out,
                               (uint8_t *)st_rs485_protocal_info.pb_body_data,
                               w_data_out_buffer_size);
                    }
                    goto ANALYSIS_SUCCESS;
                }
            }
            {
//				yl_debug_printf("rs485_rx_data_analysis check_crc16 fail\r\n");
				goto ANALYSIS_FAIL;
            }
        }
    }
    else
    {
        goto WAIT;
    }

    ANALYSIS_SUCCESS:
    if((NULL != pst_rs485_rx_data_info) &&\
       (TRUE == b_result))
    {
//        rs485_rx_polling_data_queue_shift();
        if(pst_rs485_rx_data_info->pb_data)
        {
            vPortFree(pst_rs485_rx_data_info->pb_data);
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"rs485_rx_data_analysis_4:%x\n",pst_rs485_rx_data_info->pb_data);
            #endif

        }
        vPortFree(pst_rs485_rx_data_info);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"rs485_rx_data_analysis_3:%x\n",pst_rs485_rx_data_info);
        #endif
    }
    *x_start_time = 0;
    *pb_receive_state = cb_wired_network_receive_state_success;

    return TRUE;

    ANALYSIS_FAIL:
    if((NULL != pst_rs485_rx_data_info) &&\
       (TRUE == b_result))
    {
//        rs485_rx_polling_data_queue_shift();
        if(pst_rs485_rx_data_info->pb_data)
        {
            vPortFree(pst_rs485_rx_data_info->pb_data);
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"rs485_rx_data_analysis_4:%x\n",pst_rs485_rx_data_info->pb_data);
            #endif

        }
        vPortFree(pst_rs485_rx_data_info);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"rs485_rx_data_analysis_3:%x\n",pst_rs485_rx_data_info);
        #endif
    }

    *x_start_time = 0;
    *pb_receive_state = cb_wired_network_receive_state_fail;
    return TRUE;

    WAIT:
    if(*x_start_time == 0)
    {
        *x_start_time = xTaskGetTickCount();
    }

    #if(tick_count_test)
    if(x_curent_time < x_start_time)
    {
        DebugPrintf(TRUE,"rs485_receive_time_out,x_start_time: %d,x_curent_time: %d\n",x_start_time,x_curent_time);
    }
    #endif

    TickType_t start_one_delay;
    extern uint8_t start_one_flag;

    if(start_one_flag == 0)
    {
        start_one_delay = pdMS_TO_TICKS_USER(300);
    }
    else
    {
        start_one_delay = pdMS_TO_TICKS_USER(1000);
    }

    if((xTaskGetTickCount() - *x_start_time) >= start_one_delay)
    {
		#if (r485_transmission_print)
		DebugPrintf(TRUE,"rs485_rx_data_analysis timeout,id:%02X,cmd:%04X\r\n,time:%d,current time %d\r\n",
					dw_device_address,
					w_data_addr,
					xTaskGetTickCount() - *x_start_time,
					xTaskGetTickCount());
		#endif
		*x_start_time = 0;
        goto ANALYSIS_FAIL;
    }
    else
    {
        *pb_receive_state = cb_wired_network_receive_state_wait;
    }

    if ((NULL != pst_rs485_rx_data_info) && (TRUE == b_result))
    {
        vPortFree(pst_rs485_rx_data_info->pb_data);
        vPortFree(pst_rs485_rx_data_info);
    }

    return TRUE;
}
uint8_t rs485_rx_subset_update_data_analysis(uint32_t dw_device_address,
                                             uint8_t b_cmd,
                                             uint16_t w_data_addr,
                                             uint16_t *pw_data_out_len,
                                             uint8_t *pb_data_out,
                                             uint16_t w_data_out_buffer_size,
                                             uint8_t *pb_receive_state)
{
    uint8_t b_result = FALSE;
    rs485_rx_data_info *pst_rs485_rx_data_info = NULL;

    b_result = rs485_rx_subset_update_data_queue_receive((pv_queue_operate_msg *)&pst_rs485_rx_data_info,FALSE,b_cmd);
    if(TRUE == b_result)
    {
        rs485_protocal_info st_rs485_protocal_info = {0};

        if((NULL == pst_rs485_rx_data_info) ||\
           (NULL == pst_rs485_rx_data_info->pb_data))
        {
            goto ANALYSIS_FAIL;
        }
        {
            uint16_t w_data_len = 0;
            uint8_t *pb_data = NULL;
            pb_data = pst_rs485_rx_data_info->pb_data;
            w_data_len = pst_rs485_rx_data_info->w_data_len;
            if(w_data_len != 0)
            {
                if(TRUE == check_crc16(pb_data,w_data_len))
                {
                    st_rs485_protocal_info.b_device_id = pb_data[0];
                    st_rs485_protocal_info.b_first_cmd = pb_data[2];
                    st_rs485_protocal_info.b_sub_cmd = pb_data[3];
                    st_rs485_protocal_info.w_body_data_len = (uint16_t)((((uint16_t)pb_data[4]) << 8) |\
                                                                         ((uint16_t)pb_data[5]));
                    st_rs485_protocal_info.pb_body_data = &pb_data[6];
					if(w_data_addr != 0x8007)
					{
						if(((w_data_addr | 0x8000) != (((uint16_t)st_rs485_protocal_info.b_first_cmd << 8) | ((uint16_t)st_rs485_protocal_info.b_sub_cmd))) ||\
						   ((uint8_t)dw_device_address != st_rs485_protocal_info.b_device_id))
						{
							goto ANALYSIS_FAIL;
						}
					}

                    if(st_rs485_protocal_info.w_body_data_len <= w_data_out_buffer_size)
                    {
                        *pw_data_out_len += st_rs485_protocal_info.w_body_data_len;
                        memcpy((uint8_t *)pb_data_out,
                               (uint8_t *)st_rs485_protocal_info.pb_body_data,
                               st_rs485_protocal_info.w_body_data_len);
                    }
                    else
                    {
                        *pw_data_out_len += w_data_out_buffer_size;
                        memcpy((uint8_t *)pb_data_out,
                               (uint8_t *)st_rs485_protocal_info.pb_body_data,
                               w_data_out_buffer_size);
                    }
                    goto ANALYSIS_SUCCESS;
                }
            }
            {
                goto ANALYSIS_FAIL;
            }
        }
    }
    else
    {
        *pb_receive_state = cb_wired_network_receive_state_wait;
        return TRUE;
    }

    ANALYSIS_SUCCESS:
    if((NULL != pst_rs485_rx_data_info) &&\
       (TRUE == b_result))
    {
        rs485_rx_subset_update_data_queue_shift();
        if(pst_rs485_rx_data_info->pb_data)
        {
            vPortFree(pst_rs485_rx_data_info->pb_data);
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"rs485_rx_data_analysis_4:%x\n",pst_rs485_rx_data_info->pb_data);
            #endif

        }
        vPortFree(pst_rs485_rx_data_info);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"rs485_rx_data_analysis_3:%x\n",pst_rs485_rx_data_info);
        #endif
    }
    *pb_receive_state = cb_wired_network_receive_state_success;

    return TRUE;

    ANALYSIS_FAIL:
    if((NULL != pst_rs485_rx_data_info) &&\
       (TRUE == b_result))
    {
        rs485_rx_subset_update_data_queue_shift();
        if(pst_rs485_rx_data_info->pb_data)
        {
            vPortFree(pst_rs485_rx_data_info->pb_data);
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"rs485_rx_data_analysis_4:%x\n",pst_rs485_rx_data_info->pb_data);
            #endif

        }
        vPortFree(pst_rs485_rx_data_info);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"rs485_rx_data_analysis_3:%x\n",pst_rs485_rx_data_info);
        #endif
    }
    *pb_receive_state = cb_wired_network_receive_state_fail;
    return TRUE;
}
uint8_t rs485_transmission_rx_cook_data()
{
    #define cb_rs485_cook_protocal_header      6

    rs485_rx_data_info *pst_rs485_rx_data_info = NULL;

    if(TRUE == rs485_rx_local_update_data_queue_receive((pv_queue_operate_msg *)&pst_rs485_rx_data_info,FALSE))
    {
        if((NULL == pst_rs485_rx_data_info) ||\
           (NULL == pst_rs485_rx_data_info->pb_data))
        {
            rs485_rx_local_update_data_queue_shift();
            goto ANALYSIS_FAIL;
        }
        {
            uint16_t w_data_len = 0;
            uint8_t *pb_data = NULL;
            pb_data = pst_rs485_rx_data_info->pb_data;
            w_data_len = pst_rs485_rx_data_info->w_data_len;
            if(w_data_len != 0)
            {
                uint16_t w_body_data_len = (((uint16_t)pb_data[4] << 8) |\
                                             (uint16_t)pb_data[5]);
                uint16_t w_cmd = (((uint16_t)pb_data[2] << 8) |\
                                   (uint16_t)pb_data[3]);
                if(w_data_len >= cb_rs485_cook_protocal_header + w_body_data_len + 2)
                {
                    if(cwICP_CMD_RX_DISABLE_ADDR == w_cmd)
                    {
                        vTaskSuspend(device_data_polling_task_handle);
                        vTaskSuspend(device_tree_refresh_task_handle);
                    }
//                    else if(cwICP_CMD_RX_ENABLE_ADDR == w_cmd)
//                    {
//                        vTaskResume(device_data_polling_task_handle);
//                        vTaskResume(device_tree_refresh_task_handle);
//                    }

                    boot_cmd_handle(pb_data,w_data_len);
                    rs485_rx_local_update_data_queue_shift();
                    goto ANALYSIS_SUCCESS;
                }
            }
            {
                rs485_rx_local_update_data_queue_shift();
                goto ANALYSIS_FAIL;
            }
        }
    }
    else
    {
        return FALSE;
    }

    ANALYSIS_SUCCESS:
    if(pst_rs485_rx_data_info)
    {
        if(pst_rs485_rx_data_info->pb_data)
        {
            vPortFree(pst_rs485_rx_data_info->pb_data);
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"rs485_rx_data_analysis_4:%x\n",pst_rs485_rx_data_info->pb_data);
            #endif
        }
        vPortFree(pst_rs485_rx_data_info);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"rs485_rx_data_analysis_3:%x\n",pst_rs485_rx_data_info);
        #endif
    }
    return TRUE;

    ANALYSIS_FAIL:
    if(pst_rs485_rx_data_info)
    {
        if(pst_rs485_rx_data_info->pb_data)
        {
            vPortFree(pst_rs485_rx_data_info->pb_data);
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"rs485_rx_data_analysis_4:%x\n",pst_rs485_rx_data_info->pb_data);
            #endif
        }
        vPortFree(pst_rs485_rx_data_info);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"rs485_rx_data_analysis_3:%x\n",pst_rs485_rx_data_info);
        #endif
    }

    return TRUE;
}
///*      //mark rs485 rx transparent
uint8_t rs485_transmission_rx_transparent_data(uint32_t dw_device_address,
                                               uint8_t b_cmd,
                                               uint16_t w_data_addr,
                                               uint16_t *pw_data_out_len,
                                               uint8_t *pb_data_out,
                                               uint16_t w_data_out_buffer_size,
                                               uint8_t *pb_receive_state)
{
    rs485_rx_data_info *pst_rs485_rx_data_info = NULL;
    uint8_t b_result = FALSE;
	size_t myPortGetFreeHeapSize;
	myPortGetFreeHeapSize = xPortGetFreeHeapSize();

    // if(NULL != pst_rs485_rx_transparent_mutex)
    // {
    //     #if(rx_transparent_data_Semaphore_test)
    //     DebugPrintf(TRUE,"rx_transparent_data_SemaphoreTake task:%s\n",pcTaskGetTaskName(NULL));
    //     #endif
    //     xSemaphoreTake(pst_rs485_rx_transparent_mutex, 2500);
    // }
    if(FALSE == f_rs485_receive_transparent_enable)
    {
        f_rs485_receive_transparent_enable = TRUE;
        return FALSE;
    }

    b_result = rs485_rx_transparent_data_queue_receive((pv_queue_operate_msg *)&pst_rs485_rx_data_info,FALSE,b_cmd);
    if(TRUE == b_result)
    {
        if((NULL == pst_rs485_rx_data_info) ||\
           (NULL == pst_rs485_rx_data_info->pb_data))
        {
            goto ANALYSIS_FAIL;
        }
        {
            if(pst_rs485_rx_data_info->w_data_len != 0)
            {
                if(pst_rs485_rx_data_info->w_data_len <= w_data_out_buffer_size)
                {
                    *pw_data_out_len += pst_rs485_rx_data_info->w_data_len;
                    memcpy((uint8_t *)pb_data_out,
                           (uint8_t *)pst_rs485_rx_data_info->pb_data,
                           pst_rs485_rx_data_info->w_data_len);
                }
                else
                {
                    *pw_data_out_len += w_data_out_buffer_size;
                    memcpy((uint8_t *)pb_data_out,
                           (uint8_t *)pst_rs485_rx_data_info->pb_data,
                           w_data_out_buffer_size);
                }
                goto ANALYSIS_SUCCESS;
            }
            {
                goto ANALYSIS_FAIL;
            }
        }
    }
    else
    {
        *pb_receive_state = cb_wired_network_receive_state_wait;
        // if(NULL != pst_rs485_rx_transparent_mutex)
        // {
        //     #if(rx_transparent_data_Semaphore_test)
        //     DebugPrintf(TRUE,"rx_transparent_data_SemaphoreGive task:%s\n",pcTaskGetTaskName(NULL));
        //     #endif
        //     xSemaphoreGive(pst_rs485_rx_transparent_mutex);
        // }
        return TRUE;
    }

    ANALYSIS_SUCCESS:
    if((NULL != pst_rs485_rx_data_info) &&\
       (TRUE == b_result))
    {
        rs485_rx_transparent_data_queue_shift();
        if(pst_rs485_rx_data_info->pb_data)
        {
            vPortFree(pst_rs485_rx_data_info->pb_data);
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"rs485_rx_data_analysis_4:%x\n",pst_rs485_rx_data_info->pb_data);
            #endif

        }
        vPortFree(pst_rs485_rx_data_info);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"rs485_rx_data_analysis_3:%x\n",pst_rs485_rx_data_info);
        #endif

    }
    f_rs485_receive_transparent_enable = FALSE;
    *pb_receive_state = cb_wired_network_receive_state_success;
    if(NULL != pst_rs485_rx_transparent_mutex)
    {
        #if(rx_transparent_data_Semaphore_test)
        DebugPrintf(TRUE,"rx_transparent_data_SemaphoreGive task:%s\n",pcTaskGetTaskName(NULL));
        #endif
        xSemaphoreGive(pst_rs485_rx_transparent_mutex);
    }
    return TRUE;

    ANALYSIS_FAIL:
    if((NULL != pst_rs485_rx_data_info) &&\
       (TRUE == b_result))
    {
        rs485_rx_transparent_data_queue_shift();
        if(pst_rs485_rx_data_info->pb_data)
        {
            vPortFree(pst_rs485_rx_data_info->pb_data);
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"rs485_rx_data_analysis_4:%x\n",pst_rs485_rx_data_info->pb_data);
            #endif

        }
        vPortFree(pst_rs485_rx_data_info);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"rs485_rx_data_analysis_3:%x\n",pst_rs485_rx_data_info);
        #endif
    }
    f_rs485_receive_transparent_enable = FALSE;
    *pb_receive_state = cb_wired_network_receive_state_fail;
    if(NULL != pst_rs485_rx_transparent_mutex)
    {
        #if(rx_transparent_data_Semaphore_test)
        DebugPrintf(TRUE,"rx_transparent_data_SemaphoreGive task:%s\n",pcTaskGetTaskName(NULL));
        #endif
        xSemaphoreGive(pst_rs485_rx_transparent_mutex);
    }
    return TRUE;
}
//        */

void rs485_transmission_init()
{
//    rs485_raw_data_queue_init();
	// rs485_data_queue_init();

    // rs485_communicate_Semaphore_creat();
	rs485_data_polling_Semaphore_creat();
}
void vRs485TxDataTask(void* parameter)
{
	DebugPrintf(TRUE,"%d,vRs485TxDataTask_first_entry\n",__LINE__);

    for(;;)
    {
//        ulTaskNotifyTake(TRUE,portMAX_DELAY);

		#if(task_call_rate_test)
		DebugPrintf(TRUE,"vRs485TxDataTask,start: %d,priority: %d\n",xTaskGetTickCount(),RS485_TX_DATA_TASK_PRIORITY);
		#endif

        rs485_tx_data_background();

        #if(product_test_rs485_enable)
        product_test_rs485_channel();
        #endif

		#if(task_call_rate_test)
		DebugPrintf(TRUE,"vRs485TxDataTask,end: %d,priority: %d\n",xTaskGetTickCount(),RS485_TX_DATA_TASK_PRIORITY);
		#endif

		vTaskDelay(pdMS_TO_TICKS_USER(dw_rs485_trans_task_delay));
    }
}
//void vRs485RxDataAnalysisTask(void* parameter)
//{
//	DebugPrintf(TRUE,"%d,vRs485RxDataAnalysisTask_first_entry\n",__LINE__);
//
//    for(;;)
//    {
//
//		#if(task_call_rate_test)
//		DebugPrintf(TRUE,"vRs485RxDataAnalysisTask,start: %d,priority: %d\n",xTaskGetTickCount(),RS485_RX_DATA_ANALYSIS_TASK_PRIORITY);
//		#endif
//
//        rs485_rx_data_analysis();
//
//		#if(task_call_rate_test)
//		DebugPrintf(TRUE,"vRs485RxDataAnalysisTask,end: %d,priority: %d\n",xTaskGetTickCount(),RS485_RX_DATA_ANALYSIS_TASK_PRIORITY);
//		#endif
//
//		vTaskDelay(pdMS_TO_TICKS_USER(5));
//    }
//}

void rs485_transmission_task_init()
{
    BaseType_t xReturn = pdPASS;

    xReturn = xTaskCreate( vRs485TxDataTask,"vRs485TxDataTask",rs485_tx_data_task_stack_size,NULL,rs485_tx_data_task_priority,&rs485_tx_data_task_handle);
    if(pdFAIL == xReturn)
    {
        DebugPrintf(TRUE,"%d,vRs485TxDataTask Create Fail\n",__LINE__);
    }

//	xReturn = xTaskCreate( vRs485RxDataAnalysisTask,"vRs485RxDataAnalysisTask",RS485_RX_DATA_ANALYSIS_TASK_STACK_SIZE,NULL,RS485_RX_DATA_ANALYSIS_TASK_PRIORITY,&rs485_rx_data_analysis_task_handle );
//    if(pdFAIL == xReturn)
//    {
//        DebugPrintf(TRUE,"%d,vRs485RxCycleDataTask Create Fail\n",__LINE__);
//    }
}

uint8_t rs485_tx_data_send(rs485_tx_data_info *pst_rs485_tx_data_info)
{
    TickType_t x_start_time = xTaskGetTickCount();
    TickType_t x_curent_time = 0;
    TickType_t x_delay_time =  pdMS_TO_TICKS_USER(5000);

    if(0 == pst_rs485_tx_data_info->st_data_info.w_data_len)
    {
        return FALSE;
    }
    enable_rs485_tx_it(pst_rs485_tx_data_info->st_data_info.w_data_len,
                       pst_rs485_tx_data_info->st_data_info.pb_data);
    while(0 == rs485_tx_index_get())
    {
        x_curent_time = xTaskGetTickCount();

        #if(tick_count_test)
        if(x_curent_time < x_start_time)
        {
            DebugPrintf(TRUE,"tick_count_error,x_start_time: %d,x_curent_time: %d\n",x_start_time,x_curent_time);
        }
        #endif

        if((x_curent_time - x_start_time) >= x_delay_time)
        {
            break;
        }
        vTaskDelay(0);
    }
    while(0 != rs485_tx_index_get())
    {
        x_curent_time = xTaskGetTickCount();

        #if(tick_count_test)
        if(x_curent_time < x_start_time)
        {
            DebugPrintf(TRUE,"tick_count_error,x_start_time: %d,x_curent_time: %d\n",x_start_time,x_curent_time);
        }
        #endif

        if((x_curent_time - x_start_time) >= x_delay_time)
        {
            break;
        }
        vTaskDelay(0);

    }

    return TRUE;
}
