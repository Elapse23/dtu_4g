#include "msg_queue.h"
#include "msg_queue_operate.h"
#include "lte_module_data_queue.h"
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

#define  cb_max_lte_module_rx_data_queue_len          50
#define  cb_max_lte_module_tx_data_queue_len          30

SemaphoreHandle_t  pst_lte_module_rx_data_mutex = NULL;
SemaphoreHandle_t  pst_lte_module_tx_data_mutex = NULL;

static void *pv_lte_module_rx_data_queue_buffer[cb_max_lte_module_rx_data_queue_len];
static void *pv_lte_module_tx_data_queue_buffer[cb_max_lte_module_tx_data_queue_len];

static void lte_module_queue_Semaphore_creat();

CirQueue st_lte_module_rx_data_queue =
{
    &pv_lte_module_rx_data_queue_buffer[0],
    0,
    0,
    cb_max_lte_module_rx_data_queue_len,
    0,
};
CirQueue st_lte_module_tx_data_queue =
{
    &pv_lte_module_tx_data_queue_buffer[0],
    0,
    0,
    cb_max_lte_module_tx_data_queue_len,
    0,
};

static void lte_module_queue_Semaphore_creat()
{
    pst_lte_module_rx_data_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_lte_module_rx_data_mutex)
    {

    }

    pst_lte_module_tx_data_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_lte_module_tx_data_mutex)
    {

    }
}

uint8_t lte_module_rx_data_queue_send(pv_queue_operate_msg pv_queue_data)
{
    return queue_operate_send(pst_lte_module_rx_data_mutex,\
                              &st_lte_module_rx_data_queue,\
                              pv_queue_data);
}

uint8_t lte_module_rx_data_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue)
{
    return queue_receive_operate(pst_lte_module_rx_data_mutex,\
                                 &st_lte_module_rx_data_queue,\
                                 pv_queue_data,\
                                 f_shift_queue);
}

uint8_t lte_module_tx_data_queue_send(pv_queue_operate_msg pv_queue_data)
{
    return queue_operate_send(pst_lte_module_tx_data_mutex,\
                              &st_lte_module_tx_data_queue,\
                              pv_queue_data);
}

uint8_t lte_module_tx_data_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue)
{
    return queue_receive_operate(pst_lte_module_tx_data_mutex,\
                                 &st_lte_module_tx_data_queue,\
                                 pv_queue_data,\
                                 f_shift_queue);
}
uint8_t lte_module_rx_data_queue_shift()
{
    return queue_shift_operate(pst_lte_module_rx_data_mutex,\
                               &st_lte_module_rx_data_queue,\
                               TRUE,\
                               1);
}
uint8_t lte_module_tx_data_queue_shift()
{
    return queue_shift_operate(pst_lte_module_tx_data_mutex,\
                               &st_lte_module_tx_data_queue,\
                               TRUE,\
                               1);
}
uint8_t let_module_rx_data_queue_empty_check()
{
    if(0 == st_lte_module_tx_data_queue.wQueueLen)
    {
        return TRUE;
    }
    return FALSE;
}

void lte_module_queue_init()
{
    lte_module_queue_Semaphore_creat();
}
