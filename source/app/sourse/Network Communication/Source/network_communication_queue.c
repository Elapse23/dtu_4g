#include "msg_queue.h"
#include "msg_queue_operate.h"
#include "network_communication_queue.h"
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

//1111111111111111111111111111111111111111111111111111111
#define  cb_max_network_com_inquire_queue_len          50
static void *pv_network_com_inquire_queue_buffer[cb_max_network_com_inquire_queue_len];
CirQueue st_network_com_inquire_queue =
{
    &pv_network_com_inquire_queue_buffer[0],
    0,
    0,
    cb_max_network_com_inquire_queue_len,
    0,
};
SemaphoreHandle_t  pst_network_com_inquire_mutex = NULL;

static void network_com_inquire_queue_Semaphore_creat()
{
    pst_network_com_inquire_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_network_com_inquire_mutex)
    {

    }
}

uint8_t network_com_inquire_queue_send(pv_queue_operate_msg pv_queue_data)
{
    return queue_operate_send(pst_network_com_inquire_mutex,\
                              &st_network_com_inquire_queue,\
                              pv_queue_data);
}

uint8_t network_com_inquire_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue)
{
    return queue_receive_operate(pst_network_com_inquire_mutex,\
                                 &st_network_com_inquire_queue,\
                                 pv_queue_data,\
                                 f_shift_queue);
}

uint8_t network_com_inquire_queue_shift()
{
    return queue_shift_operate(pst_network_com_inquire_mutex,\
                               &st_network_com_inquire_queue,\
                               TRUE,\
                               1);
}

uint8_t network_com_inquire_queue_clear()
{
    return queue_clear_operate(pst_network_com_inquire_mutex,\
                               &st_network_com_inquire_queue);
}

//22222222222222222222222222222222222222222222222222
#define  cb_max_network_com_config_queue_len          50
static void *pv_network_com_config_queue_buffer[cb_max_network_com_config_queue_len];
CirQueue st_network_com_config_queue =
{
    &pv_network_com_config_queue_buffer[0],
    0,
    0,
    cb_max_network_com_config_queue_len,
    0,
};
SemaphoreHandle_t  pst_network_com_config_mutex = NULL;

static void network_com_config_queue_Semaphore_creat()
{
    pst_network_com_config_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_network_com_config_mutex)
    {

    }
}
uint8_t network_com_config_queue_send(pv_queue_operate_msg pv_queue_data)
{
    return queue_operate_send(pst_network_com_config_mutex,\
                              &st_network_com_config_queue,\
                              pv_queue_data);
}

uint8_t network_com_config_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue)
{
    return queue_receive_operate(pst_network_com_config_mutex,\
                                 &st_network_com_config_queue,\
                                 pv_queue_data,\
                                 f_shift_queue);
}

uint8_t network_com_config_queue_shift()
{
    return queue_shift_operate(pst_network_com_config_mutex,\
                               &st_network_com_config_queue,\
                               TRUE,\
                               1);
}

uint8_t network_com_config_queue_clear()
{
    return queue_clear_operate(pst_network_com_config_mutex,\
                               &st_network_com_config_queue);
}

//33333333333333333333333333333333333333333333333333
#define  cb_max_network_com_other_queue_len          50
static void *pv_network_com_other_queue_buffer[cb_max_network_com_other_queue_len];
CirQueue st_network_com_other_queue =
{
    &pv_network_com_other_queue_buffer[0],
    0,
    0,
    cb_max_network_com_other_queue_len,
    0,
};
SemaphoreHandle_t  pst_network_com_other_mutex = NULL;

static void network_com_other_queue_Semaphore_creat()
{
    pst_network_com_other_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_network_com_other_mutex)
    {

    }
}
uint8_t network_com_other_queue_send(pv_queue_operate_msg pv_queue_data)
{
    return queue_operate_send(pst_network_com_other_mutex,\
                              &st_network_com_other_queue,\
                              pv_queue_data);
}

uint8_t network_com_other_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue)
{
    return queue_receive_operate(pst_network_com_other_mutex,\
                                 &st_network_com_other_queue,\
                                 pv_queue_data,\
                                 f_shift_queue);
}

uint8_t network_com_other_queue_shift()
{
    return queue_shift_operate(pst_network_com_other_mutex,\
                               &st_network_com_other_queue,\
                               TRUE,\
                               1);
}

uint8_t network_com_other_queue_clear()
{
    return queue_clear_operate(pst_network_com_other_mutex,\
                               &st_network_com_other_queue);
}
//44444444444444444444444444444444444444444444444444
#define  cb_max_network_com_tx_data_queue_len          50
static void *pv_network_com_tx_data_queue_buffer[cb_max_network_com_tx_data_queue_len];
CirQueue st_network_com_tx_data_queue =
{
    &pv_network_com_tx_data_queue_buffer[0],
    0,
    0,
    cb_max_network_com_tx_data_queue_len,
    0,
};
SemaphoreHandle_t  pst_network_com_tx_data_mutex = NULL;

static void network_com_tx_data_queue_Semaphore_creat()
{
    pst_network_com_tx_data_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_network_com_tx_data_mutex)
    {

    }
}
uint8_t network_com_tx_data_queue_send(pv_queue_operate_msg pv_queue_data)
{
    return queue_operate_send(pst_network_com_tx_data_mutex,\
                              &st_network_com_tx_data_queue,\
                              pv_queue_data);
}

uint8_t network_com_tx_data_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue)
{
    return queue_receive_operate(pst_network_com_tx_data_mutex,\
                                 &st_network_com_tx_data_queue,\
                                 pv_queue_data,\
                                 f_shift_queue);
}

uint8_t network_com_tx_data_queue_shift()
{
    return queue_shift_operate(pst_network_com_tx_data_mutex,\
                               &st_network_com_tx_data_queue,\
                               TRUE,\
                               1);
}

uint8_t network_com_tx_data_queue_clear()
{
    return queue_clear_operate(pst_network_com_tx_data_mutex,\
                               &st_network_com_tx_data_queue);
}

void network_com_queue_init()
{
    network_com_inquire_queue_Semaphore_creat();
    network_com_config_queue_Semaphore_creat();
    network_com_other_queue_Semaphore_creat();
    network_com_tx_data_queue_Semaphore_creat();
}
