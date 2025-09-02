#include "msg_queue.h"
#include "msg_queue_operate.h"
#include "lte_module_config_queue.h"
#include <ysizet.h>
#include "FreeRTOS.h"//ڌ֨ӽԃ˳Ѳ NO.1
#include "semphr.h"  //ڌ֨ӽԃ˳Ѳ NO.2
#include "application_general_macro.h"
/*
     in
     |
 ||||
|
out
*/
/*
void *
***  ...  ,,,
***  ...  ,,,
***  ...  ,,,

void **
***  ...  ...  ,,,
***  ...  ...  ,,,
***  ...  ...  ,,,
*/

#define  cb_max_lte_module_driver_config_queue_len          10

SemaphoreHandle_t  pst_lte_module_driver_config_queue_mutex = NULL;

static void *pv_lte_module_driver_config_queue_buffer[cb_max_lte_module_driver_config_queue_len];

static void lte_module_driver_config_queue_Semaphore_creat();

CirQueue st_lte_module_driver_config_queue =
{
    &pv_lte_module_driver_config_queue_buffer[0],
    0,
    0,
    cb_max_lte_module_driver_config_queue_len,
    0,
};

static void lte_module_driver_config_queue_Semaphore_creat()
{
    pst_lte_module_driver_config_queue_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_lte_module_driver_config_queue_mutex)
    {

    }
}

extern CirQueue st_network_iot_module_data_queue;
uint8_t lte_module_driver_config_queue_send(pv_queue_operate_msg pv_queue_data)
{
    // while (FALSE == sIfCirQueueEmpty(&st_lte_module_driver_config_queue)
    //         && FALSE == sIfCirQueueFull(&st_network_iot_module_data_queue))
    // {
    //     vTaskDelay(5);
    // }
    return queue_operate_send(pst_lte_module_driver_config_queue_mutex,\
                              &st_lte_module_driver_config_queue,\
                              pv_queue_data);
}

uint8_t lte_module_driver_config_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue)
{
    return queue_receive_operate(pst_lte_module_driver_config_queue_mutex,\
                                 &st_lte_module_driver_config_queue,\
                                 pv_queue_data,\
                                 f_shift_queue);
}

uint8_t lte_module_driver_config_queue_shift()
{
    return queue_shift_operate(pst_lte_module_driver_config_queue_mutex,\
                               &st_lte_module_driver_config_queue,\
                               TRUE,\
                               1);
}

uint8_t lte_module_driver_config_queue_clear()
{
    return queue_clear_operate(pst_lte_module_driver_config_queue_mutex,\
                               &st_lte_module_driver_config_queue);
}

void lte_module_driver_config_queue_init()
{
    lte_module_driver_config_queue_Semaphore_creat();
}
