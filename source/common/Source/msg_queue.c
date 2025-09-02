#include "msg_queue.h"
#include "application_general_macro.h"

/************************************************************************************************
* Input:  *pCirQueue: point of circle queue struct
*
* Output: length of circle queue buffer
*
* Logic discription: get length of circle queue buffer
*
* run time:
************************************************************************************************/
INT16U sGetCirQueueLen(CirQueue *pCirQueue)
{
    return (pCirQueue->wQueueLen);
}

INT8U sIfCirQueueEmpty(CirQueue *pCirQueue)
{
    if(0 == (pCirQueue->wQueueLen))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

INT8U sIfCirQueueFull(CirQueue *pCirQueue)
{
    if((pCirQueue->wQueueLen) >= (pCirQueue->wMaxQueueLen))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void *spGetCirQueueStartPoint(CirQueue *pCirQueue)
{
    if(FALSE == sIfCirQueueEmpty(pCirQueue))
    {
        return (void *)((INT8U *)pCirQueue->pbQueueBuf + sizeof(INT8U *) * pCirQueue->wStartIndex);
    }

    return (NULL);
}

void *spGetCirQueueEndPoint(CirQueue *pCirQueue)
{
    if(FALSE == sIfCirQueueFull(pCirQueue))
    {
        return (void *)((INT8U *)pCirQueue->pbQueueBuf + sizeof(INT8U *) * pCirQueue->wEndIndex);
    }

    return (NULL);
}

/************************************************************************************************
* Input: *pCirQueue: point of circle queue struct; *pbQueueData: insert buffer point;
*        wElementSize: queue one element size
*
* Output: insert buffer to circle queue OK
*
* Logic discription: insert one element to circle queue
*
* run time:
************************************************************************************************/
INT8U sInsertCirQueueData(CirQueue *pCirQueue,void *pbQueueData)
{
    void *pbQueueEndPoint = 0;

    if((pCirQueue->wQueueLen) < (pCirQueue->wMaxQueueLen))
    {
        pbQueueEndPoint = spGetCirQueueEndPoint(pCirQueue);

        if(pbQueueEndPoint != NULL)
        {
            *((void **)pbQueueEndPoint) = pbQueueData;
            (pCirQueue->wQueueLen)++;
            (pCirQueue->wEndIndex)++;

            if((pCirQueue->wEndIndex) >= (pCirQueue->wMaxQueueLen))
            {
                //reset end index of circle queue while reach end of buffer.
                (pCirQueue->wEndIndex) = 0;
            }

            return TRUE;
        }
    }

    return FALSE;
}

/************************************************************************************************
* Input: *pCirQueue: point of circle queue struct; **pbQueueData: return point of current queue;
*        wElementSize: queue one element size; fShiftIndex: if enable to shift forward start index while getting data
*
* Output: get data from circle queue OK/NG.
*
* Logic discription: get data from circle queue
*
* run time:
************************************************************************************************/
INT8U sGetCirQueueData(CirQueue *pCirQueue,void **pbQueueData,INT8U fShiftIndex)
{
    void *pbQueueStartPoint = 0;

    if((NULL == pCirQueue) ||(NULL == pbQueueData))
    {
        return FALSE;
    }

    if((pCirQueue->wQueueLen) > 0)
    {
        pbQueueStartPoint = spGetCirQueueStartPoint(pCirQueue);

        if(pbQueueStartPoint != NULL)
        {
            *pbQueueData = *((void **)pbQueueStartPoint);

            if(fShiftIndex)
            {
                sShiftCirQueueStartIndex(pCirQueue,TRUE,1);
            }

            return TRUE;
        }
    }
    return FALSE;
}

/************************************************************************************************
* Input: *pCirQueue: point of circle queue struct; fShiftForward: shift forward or shift back
*        wShiftNum: shift number
*
* Output: shift forward or shift back start index of circle queue OK/NG.
*
* Logic discription: shift forward or shift back start index of circle queue.
*
* run time:
************************************************************************************************/
INT8U sShiftCirQueueStartIndex(CirQueue *pCirQueue,INT8U fShiftForward,INT16U wShiftNum)
{
    INT16U wTemp = 0;

    if(0 == pCirQueue)
    {
        return FALSE;
    }

    if(fShiftForward)
    {
        //shift forward start index of queue, and reduce queue length
        if(wShiftNum <= (pCirQueue->wQueueLen))
        {
            wTemp = (INT16U)((pCirQueue->wStartIndex) + wShiftNum);

            if(wTemp >= (pCirQueue->wMaxQueueLen))
            {
                wTemp -= (pCirQueue->wMaxQueueLen);
            }

            (pCirQueue->wStartIndex) = wTemp;
            (pCirQueue->wQueueLen) -= wShiftNum;

            return TRUE;
        }
    }
    else
    {
        //shift back start index of queue, and addup queue length
        if(wShiftNum <= ((pCirQueue->wMaxQueueLen) - (pCirQueue->wQueueLen)))
        {
            for(wTemp = 0; wTemp < wShiftNum; wTemp++)
            {
                if((pCirQueue->wStartIndex) > 0)
                {
                    (pCirQueue->wStartIndex)--;
                }
                else
                {
                    (pCirQueue->wStartIndex) = (pCirQueue->wMaxQueueLen);
                }
            }
            (pCirQueue->wQueueLen) += wShiftNum;

            return TRUE;
        }
    }

    return FALSE;
}
void sClearQueueBuff(CirQueue *pCirQueue)
{
    pCirQueue->wQueueLen = 0;
    pCirQueue->wStartIndex = 0;
    pCirQueue->wEndIndex = 0;
}
