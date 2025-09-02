#include "msg_queue.h"
#include "msg_queue_operate.h"
#include "display_management_queue.h"
#include <ysizet.h>
#include "application_general_macro.h"
//#include "FreeRTOS.h"//固定引用顺序 NO.1
//#include "semphr.h"  //固定引用顺序 NO.2
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

#define  cb_max_display_management_queue_len         20

SemaphoreHandle_t  pst_display_management_mutex = NULL;

static void *pv_display_management_queue_buffer[cb_max_display_management_queue_len];

static void display_management_queue_Semaphore_creat();

CirQueue st_display_management_queue =
{
    &pv_display_management_queue_buffer[0],
    0,
    0,
    cb_max_display_management_queue_len,
    0,
};

static void display_management_queue_Semaphore_creat()
{
    pst_display_management_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_display_management_mutex)
    {

    }
}

uint8_t display_management_queue_send(pv_queue_operate_msg pv_queue_data)
{
    return queue_operate_send(pst_display_management_mutex,\
                              &st_display_management_queue,\
                              pv_queue_data);
}

uint8_t display_management_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue)
{
    return queue_receive_operate(pst_display_management_mutex,\
                                 &st_display_management_queue,\
                                 pv_queue_data,\
                                 f_shift_queue);
}

uint8_t display_management_queue_shift()
{
    return queue_shift_operate(pst_display_management_mutex,\
                               &st_display_management_queue,\
                               TRUE,\
                               1);
}
uint8_t display_management_queue_clear()
{
    return queue_clear_operate(pst_display_management_mutex,\
                               &st_display_management_queue);
}

void display_management_queue_init()
{
    display_management_queue_Semaphore_creat();
}
