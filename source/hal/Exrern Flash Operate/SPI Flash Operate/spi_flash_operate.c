#include "drv_spiflash.h"
#include "freertos_user_config.h"
#include "projdefs.h"
#include "queue.h"
#include "string.h"
#include "spi_flash_operate_queue.h"
#include "spi_flash_operate.h"
#include "printf_custom.h"
#include "application_general_macro.h"

extern void * pvPortMalloc( size_t xWantedSize );
extern void vPortFree( void * pv );

static TaskHandle_t spi_flash_operate_process_task_handle = 0;
static void spi_flash_operate_process_task(void* parameter);

static uint32_t dw_spi_flash_operate_uuid_local = 0;

QueueHandle_t pst_spi_flash_operate_initiate_queue;
QueueHandle_t pst_spi_flash_operate_response_queue;

uint8_t spi_flash_operate_initiate(uint32_t dw_addr, uint32_t dw_data_len,uint8_t *pb_data,spi_flash_operate_type enum_operate_type)
{
    spi_flash_operate_initiate_msg *pst_spi_flash_operate_initiate_msg = 0;
    spi_flash_operate_response_msg *pst_spi_flash_operate_response_msg = 0;

    /*SPI FLASH OPERATE REQUEST*/
    {
        {
            pst_spi_flash_operate_initiate_msg = (spi_flash_operate_initiate_msg *)pvPortMalloc(sizeof(spi_flash_operate_initiate_msg));
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"spi_flash_operate_initiate_0_malloc%x\n",pst_spi_flash_operate_initiate_msg);
            #endif
            if(0 == pst_spi_flash_operate_initiate_msg)
            {
                return FALSE;
            }

            if(spi_flash_erase_request != enum_operate_type)
            {
                if(0 == pb_data)
                {
                    return FALSE;
                }
            }
        }

        {
            pst_spi_flash_operate_initiate_msg->enum_operate_type = enum_operate_type;
            pst_spi_flash_operate_initiate_msg->dw_operate_addr = dw_addr;
            pst_spi_flash_operate_initiate_msg->st_data_info.w_data_len = (uint16_t)dw_data_len;
            pst_spi_flash_operate_initiate_msg->st_data_info.pb_data = pb_data;
            pst_spi_flash_operate_initiate_msg->dw_spi_flash_operate_initiate_uuid = dw_spi_flash_operate_uuid_local++;
        }

        {
            if(FALSE == spi_flash_operate_initiate_queue_send((pv_queue_operate_msg)pst_spi_flash_operate_initiate_msg))
            {
                dw_spi_flash_operate_uuid_local--;
                return FALSE;
            }
        }
    }

    {
        if(spi_flash_operate_process_task_handle != NULL)
        {
            xTaskNotifyGive(spi_flash_operate_process_task_handle);
        }
    }

    /*SPI FLASH OPERATE REPONSE*/
    {
        uint8_t spi_flash_operate_status = FALSE;

        {
            TickType_t *px_pi_flash_operate_response_timer = (TickType_t *)pvPortMalloc(sizeof(TickType_t));
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"spi_flash_operate_initiate_1_malloc%x\n",px_pi_flash_operate_response_timer);
            #endif
            if(0 == px_pi_flash_operate_response_timer)
            {
                return FALSE;
            }

            TickType_t x_spi_flash_operate_response_start_time = xTaskGetTickCount();

            do{
                *px_pi_flash_operate_response_timer = xTaskGetTickCount();

                if(FALSE == spi_flash_operate_response_queue_receive((pv_queue_operate_msg *)&pst_spi_flash_operate_response_msg,\
                                                                     FALSE))
                {
                    continue;
                }

                if(NULL == pst_spi_flash_operate_response_msg)
                {
                    spi_flash_operate_response_queue_shift();
                    continue;
                }

                if(pst_spi_flash_operate_initiate_msg->dw_spi_flash_operate_initiate_uuid ==\
                   pst_spi_flash_operate_response_msg->dw_spi_flash_operate_response_uuid)
                {
                    if(spi_flash_operate_response_queue_shift())
                    {
                        spi_flash_operate_status = pst_spi_flash_operate_response_msg->b_spi_flash_operate_status;
                        break;
                    }
                }
            }
            while((*px_pi_flash_operate_response_timer - x_spi_flash_operate_response_start_time) < pdMS_TO_TICKS_USER(10000));

            if(px_pi_flash_operate_response_timer != 0)
            {
                vPortFree(px_pi_flash_operate_response_timer);
                #if(dynamic_memory_request_test)
                DebugPrintf(TRUE,"spi_flash_operate_initiate_0_free%x\n",px_pi_flash_operate_response_timer);
                #endif
            }

            if(pst_spi_flash_operate_initiate_msg != 0)
            {
                vPortFree(pst_spi_flash_operate_initiate_msg);
                #if(dynamic_memory_request_test)
                DebugPrintf(TRUE,"spi_flash_operate_initiate_1_free%x\n",pst_spi_flash_operate_initiate_msg);
                #endif

            }

            if(pst_spi_flash_operate_response_msg != 0)
            {
                vPortFree(pst_spi_flash_operate_response_msg);
                #if(dynamic_memory_request_test)
                DebugPrintf(TRUE,"spi_flash_operate_initiate_2_free%x\n",pst_spi_flash_operate_response_msg);
                #endif

            }
        }
        return spi_flash_operate_status;
    }
}

int32_t spi_flash_read(uint32_t dw_addr, uint32_t dw_data_len, uint8_t *pb_data)
{
    uint8_t b_operate_times = 0;

    do{
        if(TRUE == spi_flash_operate_initiate(dw_addr,dw_data_len,pb_data,spi_flash_read_request))
        {
            return 0;
        }
        else
        {
            b_operate_times++;
        }
    }while(b_operate_times < 10);

    return 1;
}

int32_t spi_flash_write(uint32_t dw_addr, uint32_t dw_data_len,uint8_t *pb_data)
{
    uint8_t b_operate_times = 0;

    do{
        if(TRUE == spi_flash_operate_initiate(dw_addr,dw_data_len,pb_data,spi_flash_write_request))
        {
            return 0;
        }
        else
        {
            b_operate_times++;
        }
    }while(b_operate_times < 10);

    return 1;
}
int32_t spi_flash_erase(uint32_t dw_addr, uint32_t dw_data_len)
{
   uint8_t b_operate_times = 0;

    do{
        if(TRUE == spi_flash_operate_initiate(dw_addr,dw_data_len,0,spi_flash_erase_request))
        {
            b_operate_times++;
        }
    }while(b_operate_times < 10);

    return 0;
}

void spi_flash_operate_process()
{
//    uint8_t *pb_spi_flash_tx_buffer = 0;
    uint8_t *pb_spi_flash_rx_buffer = 0;
    spi_flash_operate_initiate_msg *pst_spi_flash_operate_initiate_msg = 0;
    spi_flash_operate_response_msg *pst_spi_flash_operate_response_msg = 0;
//    int8_t b_operate_status = 1;

    {
        if(FALSE == spi_flash_operate_initiate_queue_receive((pv_queue_operate_msg *)&pst_spi_flash_operate_initiate_msg,\
                                                             TRUE))
        {
            return;
        }

        if(0 == pst_spi_flash_operate_initiate_msg)
        {
            return;
        }
    }

    switch(pst_spi_flash_operate_initiate_msg->enum_operate_type)
    {
        case spi_flash_read_request:
        {
            /*Fast Read*/
            {
                pb_spi_flash_rx_buffer =\
                    (uint8_t *)pvPortMalloc((pst_spi_flash_operate_initiate_msg->st_data_info.w_data_len) * sizeof(uint8_t));
                #if(dynamic_memory_request_test)
                DebugPrintf(TRUE,"spi_flash_operate_process_1_malloc%x\n",pb_spi_flash_rx_buffer);
                #endif
                if(pb_spi_flash_rx_buffer)
                {
                    {
                        drv_flash_read(pb_spi_flash_rx_buffer,\
                                       pst_spi_flash_operate_initiate_msg->dw_operate_addr,\
                                       (pst_spi_flash_operate_initiate_msg->st_data_info.w_data_len));

                        memcpy(pst_spi_flash_operate_initiate_msg->st_data_info.pb_data,\
                               (uint8_t *)(pb_spi_flash_rx_buffer),\
                               pst_spi_flash_operate_initiate_msg->st_data_info.w_data_len);
                        if(pb_spi_flash_rx_buffer != 0)
                        {
                            vPortFree(pb_spi_flash_rx_buffer);
                            #if(dynamic_memory_request_test)
                            DebugPrintf(TRUE,"spi_flash_operate_process_1_free%x\n",pb_spi_flash_rx_buffer);
                            #endif
                        }
                        /*TEST LOG*/
                        /*
                        DebugPrintf(TRUE,\
                                    "spi flash read info,addr : 0x%X,len : %i,data_h : 0x%X,data_l : 0x%X,time : %d\n",\
                                    pst_spi_flash_operate_initiate_msg->dw_operate_addr,\
                                    pst_spi_flash_operate_initiate_msg->st_data_info.w_data_len,\
                                    pst_spi_flash_operate_initiate_msg->st_data_info.pb_data[1],\
                                    pst_spi_flash_operate_initiate_msg->st_data_info.pb_data[0],\
                                    xTaskGetTickCount());
                        */
                    }
                }
                else
                {
                }
//                if(0 == b_operate_status)
//                {
//                    DebugPrintf(TRUE,"spi flash read fail\n");
//                    break;
//                }
            }
            break;
        }
        case spi_flash_write_request:
        {
            drv_flash_write_buffer(pst_spi_flash_operate_initiate_msg->st_data_info.pb_data,
                                   pst_spi_flash_operate_initiate_msg->dw_operate_addr,
                                   pst_spi_flash_operate_initiate_msg->st_data_info.w_data_len);
            break;
        }
        case spi_flash_erase_request:
        {
            drv_flash_erase_sector(pst_spi_flash_operate_initiate_msg->dw_operate_addr);
            break;
        }
        default:
        {
//            b_operate_status = 0;
            break;
        }
    }

    {
        pst_spi_flash_operate_response_msg = (spi_flash_operate_response_msg *)pvPortMalloc(sizeof(spi_flash_operate_response_msg));
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"spi_flash_operate_process_14_malloc%x\n",pst_spi_flash_operate_response_msg);
        #endif

        if(0 == pst_spi_flash_operate_response_msg)
        {
            return;
        }

//        pst_spi_flash_operate_response_msg->b_spi_flash_operate_status = b_operate_status;
        pst_spi_flash_operate_response_msg->b_spi_flash_operate_status = 1;
        pst_spi_flash_operate_response_msg->dw_spi_flash_operate_response_uuid =\
            pst_spi_flash_operate_initiate_msg->dw_spi_flash_operate_initiate_uuid;

        if(FALSE == spi_flash_operate_response_queue_send((pv_queue_operate_msg)pst_spi_flash_operate_response_msg))
        {
            return;
        }
    }
}

void spi_flash_operate_process_task(void* parameter)
{
	DebugPrintf(TRUE,"%d,spi_flash_operate_process_task_first_entry\n",__LINE__);

    for(;;)
    {
        ulTaskNotifyTake(TRUE,portMAX_DELAY);

        #if(task_call_rate_test)
		DebugPrintf(TRUE,"spi_flash_operate_process_task,start: %d,priority: %d\n",xTaskGetTickCount(),spi_flash_operate_process_task_priority);
		#endif

        spi_flash_operate_process();

		#if(task_call_rate_test)
		DebugPrintf(TRUE,"spi_flash_operate_process_task,end: %d,priority: %d\n",xTaskGetTickCount(),spi_flash_operate_process_task_priority);
		#endif

//        vTaskDelay(pdMS_TO_TICKS_USER(100));
    }
}

void spi_flash_operate_init()
{
    spi_flash_operate_queue_init();

    //хϢטҪìѨ͡ǰռԃࠕݤìȷѣһܡӲΪࠕݤһأϞרעԶлϢ
    pst_spi_flash_operate_initiate_queue = xQueueCreate(10, sizeof(spi_flash_operate_initiate_msg));
    pst_spi_flash_operate_response_queue = xQueueCreate(10, sizeof(spi_flash_operate_response_msg));

    BaseType_t xReturn = pdPASS;

	xReturn = xTaskCreate(spi_flash_operate_process_task,
						  "spi_flash_operate_process_task",
						  spi_flash_operate_process_task_stack_size,
						  NULL,
						  spi_flash_operate_process_task_priority,
						  &spi_flash_operate_process_task_handle);

    if(pdFAIL == xReturn)
    {
        DebugPrintf(TRUE,"%d,spi_flash_operate_process_task Create Fail\n",__LINE__);
    }
}
