#include "msg_queue.h"
#include "msg_queue_operate.h"
#include "network_server_response_queue.h"
#include <ysizet.h>
#include "FreeRTOS.h"//固定引用顺序 NO.1
#include "semphr.h"  //固定引用顺序 NO.2
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

extern void * pvPortMalloc(size_t xWantedSize);

#define  cb_server_response_queue_max_len          50

SemaphoreHandle_t  pst_server_response_mutex = NULL;


static void *pv_server_response_queue_buffer[cb_server_response_queue_max_len];


static void network_server_response_queue_Semaphore_creat();

CirQueue st_server_response_queue =
{
    &pv_server_response_queue_buffer[0],
    0,
    0,
    cb_server_response_queue_max_len,
    0,
};

static void network_server_response_queue_Semaphore_creat()
{
    pst_server_response_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_server_response_mutex)
    {

    }
}

uint8_t network_server_response_queue_send(pv_queue_operate_msg pv_queue_data)
{
    return queue_operate_send(pst_server_response_mutex,\
                              &st_server_response_queue,\
                              pv_queue_data);
}

uint8_t network_server_response_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue)
{
    return queue_receive_operate(pst_server_response_mutex,\
                                 &st_server_response_queue,\
                                 pv_queue_data,\
                                 f_shift_queue);
}


uint8_t network_server_response_queue_shift()
{
    return queue_shift_operate(pst_server_response_mutex,\
                               &st_server_response_queue,\
                               TRUE,\
                               1);
}


void network_server_response_queue_init()
{
    network_server_response_queue_Semaphore_creat();
}
