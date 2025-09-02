#include "freertos_user_config.h"
#include "application_general_macro.h"
#include "FreeRTOS.h"//ڌ֨ӽԃ˳Ѳ NO.1
#include "task.h"
#include "semphr.h"
#include <ysizet.h>
#include <string.h>

//#include "printf_custom.h"

TaskHandle_t task_handle_key_deal_task = 0;
TaskHandle_t wireless_communicate_receive_data_task_handle = 0;
TaskHandle_t wireless_communicate_send_data_task_handle = 0;
TaskHandle_t wireless_module_receive_data_background_task_handle = 0;
TaskHandle_t wireless_module_receive_iot_data_task_handle = 0;
TaskHandle_t wireless_module_send_data_background_task_handle = 0;
TaskHandle_t wireless_network_analysis_task_handle = 0;
TaskHandle_t lte_module_nmea_info_get_task_handle = 0;
TaskHandle_t network_iot_module_config_excute_task_handle = 0;
TaskHandle_t RS485_queue_recognition_conversion_task_handle = 0;
TaskHandle_t device_tree_refresh_task_handle = 0;
TaskHandle_t device_data_polling_task_handle = 0;
TaskHandle_t network_communicate_task_handle = 0;
TaskHandle_t network_server_inquire_request_process_task_handle = 0;
TaskHandle_t network_server_config_request_process_task_handle = 0;
TaskHandle_t network_server_other_request_process_task_handle = 0;
TaskHandle_t timestamp_task_handle = 0;


#if HEAP_MEMORY_ALLOCATE_TEST
SemaphoreHandle_t  pst_heap_memory_allocate_test_mutex = NULL;
SemaphoreHandle_t  pst_heap_memory_free_test_mutex = NULL;

#define cw_heap_memory_addrss_recode_num        500
static uint16_t w_heap_memory_allocate_addrss_index = 0;
static uint32_t dwa_heap_memory_allocate_addrss[cw_heap_memory_addrss_recode_num] = {0,};
#endif

void heap_memory_allocte_test_init()
{
    #if HEAP_MEMORY_ALLOCATE_TEST
    pst_heap_memory_allocate_test_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_heap_memory_allocate_test_mutex)
    {
        DebugPrintf(TRUE,"heap_memory_allocate_test_mutex creat error%\n");
    }

    pst_heap_memory_free_test_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_heap_memory_free_test_mutex)
    {
        DebugPrintf(TRUE,"pst_heap_memory_free_test_mutex creat error%\n");
    }
    #endif
}

void *pvPortMallocUser(size_t xWantedSize,uint8_t b_block)
{
    void *pv = 0;
    for(;;)
    {
        pv = pvPortMalloc(xWantedSize);
        if(pv)
        {
            #if HEAP_MEMORY_ALLOCATE_TEST
//            if(pst_heap_memory_allocate_test_mutex != 0)
//            {
//                xSemaphoreTake(pst_heap_memory_allocate_test_mutex, portMAX_DELAY);
//            }
//
//            if(w_heap_memory_allocate_addrss_index >= cw_heap_memory_addrss_recode_num)
//            {
//                w_heap_memory_allocate_addrss_index = 0;
//                DebugPrintf(TRUE,"heap_memory_free_block%\n");
//            }
//
//            dwa_heap_memory_allocate_addrss[w_heap_memory_allocate_addrss_index++] = (uint32_t)pv;
//
//            if(pst_heap_memory_allocate_test_mutex != 0)
//            {
//                xSemaphoreGive(pst_heap_memory_allocate_test_mutex);
//            }
            #endif

            memset((uint8_t *)pv,0,xWantedSize);
            break;
        }
        if(FALSE == b_block)
        {
             break;
        }
        vTaskDelay(1);
    }

    return pv;
}
void vPortFreeUser(void **pv,uint8_t *pb_description)
{
    if((0 == pv) || (0 == *pv))
    {
        #if HEAP_MEMORY_ALLOCATE_TEST
//        if(pst_heap_memory_free_test_mutex != 0)
//        {
//            xSemaphoreTake(pst_heap_memory_free_test_mutex, portMAX_DELAY);
//        }
//        DebugPrintf(TRUE,"heap_memory_free_error\n");
//        if(pst_heap_memory_free_test_mutex != 0)
//        {
//            xSemaphoreGive(pst_heap_memory_free_test_mutex);
//        }
        #endif

   		#if HEAP_MEMORY_ALLOCATE_TEST
		DebugPrintf(TRUE,"HEAP FREE,%s,addr: 0x%x,time: %d\n",pb_description,*pv,xTaskGetTickCount());
		#endif

        return;
    }

    #if HEAP_MEMORY_ALLOCATE_TEST
//    if(pst_heap_memory_free_test_mutex != 0)
//    {
//        xSemaphoreTake(pst_heap_memory_free_test_mutex, portMAX_DELAY);
//    }
//    uint16_t w_index = 0;
//
//    for(w_index = 0;w_index <= w_heap_memory_allocate_addrss_index;w_index++)
//    {
//        if((uint32_t)*pv == dwa_heap_memory_allocate_addrss[w_index])
//        {
//            dwa_heap_memory_allocate_addrss[w_index] = 0;
//
//            if(w_heap_memory_allocate_addrss_index > 0)
//            {
//                w_heap_memory_allocate_addrss_index--;
//            }
//            else
//            {
//                DebugPrintf(TRUE,"heap_memory_free_error\n");
//            }
//            break;
//        }
//    }
//
//    w_index--;
//    if(w_index > w_heap_memory_allocate_addrss_index)
//    {
//        DebugPrintf(TRUE,"heap_memory_free_error\n");
//    }
//    if(pst_heap_memory_free_test_mutex != 0)
//    {
//        xSemaphoreGive(pst_heap_memory_free_test_mutex);
//    }
    #endif

	#if HEAP_MEMORY_ALLOCATE_TEST
	if(pb_description)
	{
		DebugPrintf(TRUE,"HEAP FREE,%s,addr: 0x%x,time: %d\n",pb_description,*pv,xTaskGetTickCount());
	}
	else
	{
		DebugPrintf(TRUE,"HEAP FREE DESCRIPTION ERROR\n");
	}
	#endif

    vPortFree(*pv);
    *pv = 0;
}
