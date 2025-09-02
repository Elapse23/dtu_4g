#include "msg_queue.h"
#include "msg_queue_operate.h"
#include "network_ble_communication_queue.h"
#include <ysizet.h>
#include "FreeRTOS.h"//固定引用顺序 NO.1
#include "semphr.h"  //固定引用顺序 NO.2
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

QueueHandle_t x_network_ble_communication_queue = 0;


extern void * pvPortMalloc(size_t xWantedSize);

#define  cb_max_network_ble_communication_queue_len          30

SemaphoreHandle_t  pst_network_ble_communication_queue_mutex = NULL;

static network_ble_communication_info st_network_ble_communication_queue_buffer[cb_max_network_ble_communication_queue_len];

static void network_ble_communication_queue_Semaphore_creat();

CirQueue st_network_ble_communication_queue =
{
    &st_network_ble_communication_queue_buffer[0],
    0,
    0,
    cb_max_network_ble_communication_queue_len,
    0,
};

static void network_ble_communication_queue_Semaphore_creat()
{
    pst_network_ble_communication_queue_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_network_ble_communication_queue_mutex)
    {

    }
}

uint8_t network_ble_communication_queue_send(pv_queue_operate_msg pv_queue_data)
{
    return queue_operate_send(pst_network_ble_communication_queue_mutex,\
                              &st_network_ble_communication_queue,\
                              pv_queue_data);
}

uint8_t network_ble_communication_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue)
{
    return queue_receive_operate(pst_network_ble_communication_queue_mutex,\
                                 &st_network_ble_communication_queue,\
                                 pv_queue_data,\
                                 f_shift_queue);
}

uint8_t network_ble_communication_queue_shift()
{
    return queue_shift_operate(pst_network_ble_communication_queue_mutex,\
                               &st_network_ble_communication_queue,\
                               TRUE,\
                               1);
}
uint8_t network_ble_communication_queue_clear()
{
    return queue_clear_operate(pst_network_ble_communication_queue_mutex,\
                               &st_network_ble_communication_queue);
}
//22222222222222222222222222222222222222222222222222222222222222222222222222222222222
#define  cb_max_network_ble_rx_queue_len          30

SemaphoreHandle_t  pst_network_ble_rx_queue_mutex = NULL;

static network_ble_communication_info st_network_ble_rx_queue_buffer[cb_max_network_ble_rx_queue_len];

static void network_ble_rx_queue_Semaphore_creat();

CirQueue st_network_ble_rx_queue =
{
    &st_network_ble_rx_queue_buffer[0],
    0,
    0,
    cb_max_network_ble_rx_queue_len,
    0,
};

static void network_ble_rx_queue_Semaphore_creat()
{
    pst_network_ble_rx_queue_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_network_ble_rx_queue_mutex)
    {

    }
}

uint8_t network_ble_rx_queue_send(pv_queue_operate_msg pv_queue_data)
{
    return queue_operate_send(pst_network_ble_rx_queue_mutex,\
                              &st_network_ble_rx_queue,\
                              pv_queue_data);
}

uint8_t network_ble_rx_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue)
{
    return queue_receive_operate(pst_network_ble_rx_queue_mutex,\
                                 &st_network_ble_rx_queue,\
                                 pv_queue_data,\
                                 f_shift_queue);
}

uint8_t network_ble_rx_queue_shift()
{
    return queue_shift_operate(pst_network_ble_rx_queue_mutex,\
                               &st_network_ble_rx_queue,\
                               TRUE,\
                               1);
}
uint8_t network_ble_rx_queue_clear()
{
    return queue_clear_operate(pst_network_ble_rx_queue_mutex,\
                               &st_network_ble_rx_queue);
}

//33333333333333333333333333333333333333333333333333333333333333333333333333333333333333333

#define  cb_max_network_ble_tx_queue_len          30

SemaphoreHandle_t  pst_network_ble_tx_queue_mutex = NULL;

static network_ble_communication_info st_network_ble_tx_queue_buffer[cb_max_network_ble_rx_queue_len];

static void network_ble_tx_queue_Semaphore_creat();

CirQueue st_network_ble_tx_queue =
{
    &st_network_ble_tx_queue_buffer[0],
    0,
    0,
    cb_max_network_ble_tx_queue_len,
    0,
};

static void network_ble_tx_queue_Semaphore_creat()
{
    pst_network_ble_tx_queue_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_network_ble_tx_queue_mutex)
    {

    }
}

uint8_t network_ble_tx_queue_send(pv_queue_operate_msg pv_queue_data)
{
    return queue_operate_send(pst_network_ble_tx_queue_mutex,\
                              &st_network_ble_tx_queue,\
                              pv_queue_data);
}

uint8_t network_ble_tx_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue)
{
    return queue_receive_operate(pst_network_ble_tx_queue_mutex,\
                                 &st_network_ble_tx_queue,\
                                 pv_queue_data,\
                                 f_shift_queue);
}

uint8_t network_ble_tx_queue_shift()
{
    return queue_shift_operate(pst_network_ble_tx_queue_mutex,\
                               &st_network_ble_tx_queue,\
                               TRUE,\
                               1);
}
uint8_t network_ble_tx_queue_clear()
{
    return queue_clear_operate(pst_network_ble_tx_queue_mutex,\
                               &st_network_ble_tx_queue);
}

void network_ble_communication_queue_init()
{
    network_ble_communication_queue_Semaphore_creat();
    network_ble_rx_queue_Semaphore_creat();
    network_ble_tx_queue_Semaphore_creat();
}
