#ifndef _NET_MSG_QUEUE_OPERATE_H_
#define _NET_MSG_QUEUE_OPERATE_H_

#include "msg_queue.h"
#include "system_info_general_macro.h"

typedef void *pv_net_queue_operate_msg;

uint8_t NET_queue_operate_send(CirQueue *pst_queue,pv_net_queue_operate_msg pv_queue_data);
uint8_t NET_queue_operate_send_isr(CirQueue *pst_queue,pv_net_queue_operate_msg pv_queue_data);
uint8_t NET_queue_receive_operate(CirQueue *pst_queue,void **pbQueueData,uint8_t f_shift_queue);
uint8_t NET_queue_shift_operate(CirQueue *pst_queue,INT8U fShiftForward,INT16U wShiftNum);
uint8_t NET_queue_clear_operate(CirQueue *pst_queue);

#endif
