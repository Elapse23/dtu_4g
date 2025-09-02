#include "network_iot_module_data_queue.h"
#include "msg_queue.h"
#include "msg_queue_operate.h"
#include "network_iot_module_data_queue.h"
#include <ysizet.h>
#include "FreeRTOS.h"//ڌ֨ӽԃ˳Ѳ NO.1
#include "semphr.h"  //ڌ֨ӽԃ˳Ѳ NO.2
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

#define  cb_network_iot_module_data_queue_max_len          10

SemaphoreHandle_t  pst_network_iot_module_data_queue_mutex = NULL;


static void *pv_network_iot_module_data_queue_buffer[cb_network_iot_module_data_queue_max_len];


static void network_iot_module_data_queue_Semaphore_creat();

CirQueue st_network_iot_module_data_queue =
{
    &pv_network_iot_module_data_queue_buffer[0],
    0,
    0,
    cb_network_iot_module_data_queue_max_len,
    0,
};

static void network_iot_module_data_queue_Semaphore_creat()
{
    pst_network_iot_module_data_queue_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_network_iot_module_data_queue_mutex)
    {

    }
}

uint8_t network_iot_module_data_queue_send(pv_queue_operate_msg pv_queue_data)
{
    return queue_operate_send(pst_network_iot_module_data_queue_mutex,\
                              &st_network_iot_module_data_queue,\
                              pv_queue_data);
}

uint8_t network_iot_module_data_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue)
{
    return queue_receive_operate(pst_network_iot_module_data_queue_mutex,\
                                 &st_network_iot_module_data_queue,\
                                 pv_queue_data,\
                                 f_shift_queue);
}

uint8_t network_iot_module_data_queue_shift()
{
    return queue_shift_operate(pst_network_iot_module_data_queue_mutex,\
                               &st_network_iot_module_data_queue,\
                               TRUE,\
                               1);
}

void network_iot_module_data_queue_init()
{
    network_iot_module_data_queue_Semaphore_creat();
}

/**********************************ble queue****************************************/
#define  cb_network_iot_ble_module_data_queue_max_len          10

SemaphoreHandle_t  pst_network_iot_ble_module_data_queue_mutex = NULL;


static void *pv_network_iot_ble_module_data_queue_buffer[cb_network_iot_ble_module_data_queue_max_len];


static void network_iot_ble_module_data_queue_Semaphore_creat();

CirQueue st_network_iot_ble_module_data_queue =
{
    &pv_network_iot_ble_module_data_queue_buffer[0],
    0,
    0,
    cb_network_iot_ble_module_data_queue_max_len,
    0,
};

static void network_iot_ble_module_data_queue_Semaphore_creat()
{
    pst_network_iot_ble_module_data_queue_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_network_iot_ble_module_data_queue_mutex)
    {

    }
}

uint8_t network_iot_ble_module_data_queue_send(pv_queue_operate_msg pv_queue_data)
{
    return queue_operate_send(pst_network_iot_ble_module_data_queue_mutex,\
                              &st_network_iot_ble_module_data_queue,\
                              pv_queue_data);
}

uint8_t network_iot_ble_module_data_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue)
{
    return queue_receive_operate(pst_network_iot_ble_module_data_queue_mutex,\
                                 &st_network_iot_ble_module_data_queue,\
                                 pv_queue_data,\
                                 f_shift_queue);
}

uint8_t network_iot_ble_module_data_queue_shift()
{
    return queue_shift_operate(pst_network_iot_ble_module_data_queue_mutex,\
                               &st_network_iot_ble_module_data_queue,\
                               TRUE,\
                               1);
}

/**********************************wifi queue****************************************/

#define  cb_network_iot_wifi_module_data_queue_max_len          10

SemaphoreHandle_t  pst_network_iot_wifi_module_data_queue_mutex = NULL;


static void *pv_network_iot_wifi_module_data_queue_buffer[cb_network_iot_wifi_module_data_queue_max_len];


static void network_iot_wifi_module_data_queue_Semaphore_creat();

CirQueue st_network_iot_wifi_module_data_queue =
{
    &pv_network_iot_wifi_module_data_queue_buffer[0],
    0,
    0,
    cb_network_iot_wifi_module_data_queue_max_len,
    0,
};

static void network_iot_wifi_module_data_queue_Semaphore_creat()
{
    pst_network_iot_wifi_module_data_queue_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_network_iot_wifi_module_data_queue_mutex)
    {

    }
}

uint8_t network_iot_wifi_module_data_queue_send(pv_queue_operate_msg pv_queue_data)
{
    return queue_operate_send(pst_network_iot_wifi_module_data_queue_mutex,\
                              &st_network_iot_wifi_module_data_queue,\
                              pv_queue_data);
}

uint8_t network_iot_wifi_module_data_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue)
{
    return queue_receive_operate(pst_network_iot_wifi_module_data_queue_mutex,\
                                 &st_network_iot_wifi_module_data_queue,\
                                 pv_queue_data,\
                                 f_shift_queue);
}

uint8_t network_iot_wifi_module_data_queue_shift()
{
    return queue_shift_operate(pst_network_iot_wifi_module_data_queue_mutex,\
                               &st_network_iot_wifi_module_data_queue,\
                               TRUE,\
                               1);
}


