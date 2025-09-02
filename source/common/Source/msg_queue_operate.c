#include "msg_queue.h"
#include "msg_queue_operate.h"
#include <ysizet.h>
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

uint8_t queue_operate_send(SemaphoreHandle_t pst_queue_semaphore,CirQueue *pst_queue,pv_queue_operate_msg pv_queue_data)
{
    uint8_t b_result = FALSE;

    if(pst_queue_semaphore != 0)
    {
        xSemaphoreTake(pst_queue_semaphore, portMAX_DELAY);
    }

    if((0 == pst_queue) || (0 == pv_queue_data))
    {
        b_result = FALSE;
    }

    if(FALSE == sIfCirQueueFull(pst_queue))
    {
        if(sInsertCirQueueData(pst_queue,pv_queue_data))
        {
            b_result = TRUE;
        }
        else
        {
            b_result = FALSE;
        }
    }

    if(pst_queue_semaphore != 0)
    {
        xSemaphoreGive(pst_queue_semaphore);
    }

    return b_result;
}

uint8_t queue_operate_send_isr(SemaphoreHandle_t pst_queue_semaphore,CirQueue *pst_queue,pv_queue_operate_msg pv_queue_data,BaseType_t *pf_yield_from_isr)
{
    uint8_t b_result = FALSE;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if(pst_queue_semaphore != 0)
    {
        xSemaphoreTakeFromISR(pst_queue_semaphore,&xHigherPriorityTaskWoken);
    }

    if((0 == pst_queue) || (0 == pv_queue_data))
    {
        b_result = FALSE;
    }

    if(FALSE == sIfCirQueueFull(pst_queue))
    {
        if(sInsertCirQueueData(pst_queue,pv_queue_data))
        {
            b_result = TRUE;
        }
        else
        {
            b_result = FALSE;
        }
    }

    if(pst_queue_semaphore != 0)
    {
        xSemaphoreGiveFromISR(pst_queue_semaphore,&xHigherPriorityTaskWoken);
    }

    *pf_yield_from_isr = xHigherPriorityTaskWoken;

    return b_result;
}

uint8_t queue_receive_operate(SemaphoreHandle_t pst_queue_semaphore,CirQueue *pst_queue,void **pbQueueData,uint8_t f_shift_queue)
{
    uint8_t b_result = FALSE;

    if(pst_queue_semaphore != 0)
    {
        xSemaphoreTake(pst_queue_semaphore, portMAX_DELAY);
    }

    if(FALSE == sIfCirQueueEmpty(pst_queue))
    {
        if(sGetCirQueueData(pst_queue,pbQueueData,f_shift_queue))
        {
            b_result = TRUE;
        }
    }

    if(pst_queue_semaphore != 0)
    {
        xSemaphoreGive(pst_queue_semaphore);
    }

    return b_result;
}

uint8_t queue_shift_operate(SemaphoreHandle_t pst_queue_semaphore,CirQueue *pCirQueue,INT8U fShiftForward,INT16U wShiftNum)
{
    uint8_t b_result = FALSE;

    if(pst_queue_semaphore != 0)
    {
        xSemaphoreTake(pst_queue_semaphore, portMAX_DELAY);
    }

    if(sShiftCirQueueStartIndex(pCirQueue,fShiftForward,wShiftNum))
    {
        b_result = TRUE;
    }
    else
    {
        b_result = FALSE;
    }

    if(pst_queue_semaphore != 0)
    {
        xSemaphoreGive(pst_queue_semaphore);
    }

    return b_result;
}
uint8_t queue_clear_operate(SemaphoreHandle_t pst_queue_semaphore,CirQueue *pCirQueue)
{
    if(pst_queue_semaphore != 0)
    {
        xSemaphoreTake(pst_queue_semaphore, portMAX_DELAY);
    }

    sClearQueueBuff(pCirQueue);

    if(pst_queue_semaphore != 0)
    {
        xSemaphoreGive(pst_queue_semaphore);
    }

    return TRUE;
}
