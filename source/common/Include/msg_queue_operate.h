#ifndef _MSG_QUEUE_OPERATE_H_
#define _MSG_QUEUE_OPERATE_H_

#include "FreeRTOS.h"
#include "semphr.h"
#include "msg_queue.h"

typedef void *pv_queue_operate_msg;
typedef struct
{
    SemaphoreHandle_t pst_queue_semaphore;
    CirQueue *pst_queue;
}msg_queue_handle;

uint8_t queue_operate_send(SemaphoreHandle_t pst_queue_semaphore,CirQueue *pst_queue,pv_queue_operate_msg pv_queue_data);
uint8_t queue_operate_send_isr(SemaphoreHandle_t pst_queue_semaphore,CirQueue *pst_queue,pv_queue_operate_msg pv_queue_data,BaseType_t *pf_yield_from_isr);
uint8_t queue_receive_operate(SemaphoreHandle_t pst_queue_semaphore,CirQueue *pst_queue,void **pbQueueData,uint8_t f_shift_queue);
uint8_t queue_shift_operate(SemaphoreHandle_t pst_queue_semaphore,CirQueue *pCirQueue,INT8U fShiftForward,INT16U wShiftNum);
uint8_t queue_clear_operate(SemaphoreHandle_t pst_queue_semaphore,CirQueue *pCirQueue);
#endif
