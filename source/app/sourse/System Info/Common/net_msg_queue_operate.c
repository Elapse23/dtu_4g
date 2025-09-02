#include "msg_queue.h"
#include "net_msg_queue_operate.h"
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

uint8_t net_queue_operate_send(CirQueue *pst_queue,pv_net_queue_operate_msg pv_queue_data)
{
    if((NULL == pst_queue) || (NULL == pv_queue_data))
    {
        return FALSE;
    }

	if(TRUE == pst_queue->fUsed)
	{
		return FALSE;
	}

	pst_queue->fUsed = TRUE;

    if(FALSE == sIfCirQueueFull(pst_queue))
    {
        if(sInsertCirQueueData(pst_queue,pv_queue_data))
        {
			pst_queue->fUsed = FALSE;

			return TRUE;
        }
    }

	pst_queue->fUsed = FALSE;

    return FALSE;
}

uint8_t net_queue_operate_send_isr(CirQueue *pst_queue,pv_net_queue_operate_msg pv_queue_data)
{
    if((NULL == pst_queue) || (NULL == pv_queue_data))
    {
        return FALSE;
    }

	if(TRUE == pst_queue->fUsed)
	{
		return FALSE;
	}

	pst_queue->fUsed = TRUE;

    if(FALSE == sIfCirQueueFull(pst_queue))
    {
        if(sInsertCirQueueData(pst_queue,pv_queue_data))
        {
			pst_queue->fUsed = FALSE;

            return TRUE;
        }
    }

	pst_queue->fUsed = FALSE;

    return FALSE;
}

uint8_t net_queue_receive_operate(CirQueue *pst_queue,void **pbQueueData,uint8_t f_shift_queue)
{
    if((NULL == pst_queue) || (NULL == pbQueueData))
	{
		return FALSE;
	}

	if(TRUE == pst_queue->fUsed)
	{
		return FALSE;
	}

	pst_queue->fUsed = TRUE;

    if(FALSE == sIfCirQueueEmpty(pst_queue))
    {
        if(sGetCirQueueData(pst_queue,pbQueueData,f_shift_queue))
        {
		 	pst_queue->fUsed = FALSE;

            return TRUE;
        }
    }

	pst_queue->fUsed = FALSE;

    return FALSE;
}

uint8_t net_queue_shift_operate(CirQueue *pst_queue,INT8U fShiftForward,INT16U wShiftNum)
{
    if(NULL == pst_queue)
	{
		return FALSE;
	}

	if(TRUE == pst_queue->fUsed)
	{
		return FALSE;
	}

	pst_queue->fUsed = TRUE;

    if(sShiftCirQueueStartIndex(pst_queue,fShiftForward,wShiftNum))
    {
		pst_queue->fUsed = FALSE;

        return TRUE;
    }

	pst_queue->fUsed = FALSE;

    return FALSE;
}
uint8_t net_queue_clear_operate(CirQueue *pst_queue)
{
    if(NULL == pst_queue)
	{
		return FALSE;
	}

	if(TRUE == pst_queue->fUsed)
	{
		return FALSE;
	}

	pst_queue->fUsed = TRUE;

    sClearQueueBuff(pst_queue);

	pst_queue->fUsed = FALSE;

    return TRUE;
}
