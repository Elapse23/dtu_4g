#ifndef __QUEUELIB_H__
#define __QUEUELIB_H__

#include <stdint.h>
#include "n32g45x.h"
#include "data_type_extern.h"

#define cbMaxQueueAttData   7

typedef struct
{
    INT16U wQueueDataType;
    INT8U bQueueDataAtt[cbMaxQueueAttData];//7+1
}QueueDataAtt;

typedef struct
{
    void *pbQueueBuf;
    INT16U wStartIndex;
    INT16U wEndIndex;
    INT16U wMaxQueueLen;
    INT16U wQueueLen;
	INT8U fUsed;
}CirQueue;

INT16U sGetCirQueueLen(CirQueue *pCirQueue);
INT8U sIfCirQueueEmpty(CirQueue *pCirQueue);
INT8U sIfCirQueueFull(CirQueue *pCirQueue);
void* spGetCirQueueStartPoint(CirQueue *pCirQueue);
void* spGetCirQueueEndPoint(CirQueue *pCirQueue);
INT8U sInsertCirQueueData(CirQueue *pCirQueue, void *pbQueueData);
INT8U sGetCirQueueData(CirQueue *pCirQueue, void **pbQueueData,INT8U fShiftIndex);
INT8U sShiftCirQueueStartIndex(CirQueue *pCirQueue,INT8U fShiftForward,INT16U wShiftNum);
void sClearQueueBuff(CirQueue *pCirQueue);

#endif
