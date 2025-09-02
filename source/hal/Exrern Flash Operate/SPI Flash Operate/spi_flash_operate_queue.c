#include "msg_queue.h"
#include "msg_queue_operate.h"
#include "spi_flash_operate_queue.h"
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

#define  cb_max_spi_flash_operate_initiate_queue_len         50
#define  cb_max_spi_flash_operate_response_queue_len          30

SemaphoreHandle_t  pst_spi_flash_operate_initiate_mutex = NULL;
SemaphoreHandle_t  pst_spi_flash_operate_response_mutex = NULL;

static void *pv_spi_flash_operate_initiate_queue_buffer[cb_max_spi_flash_operate_initiate_queue_len];
static void *pv_spi_flash_operate_response_queue_buffer[cb_max_spi_flash_operate_response_queue_len];

static void spi_flash_operate_queue_Semaphore_creat();

CirQueue st_spi_flash_operate_initiate_queue =
{
    &pv_spi_flash_operate_initiate_queue_buffer[0],
    0,
    0,
    cb_max_spi_flash_operate_initiate_queue_len,
    0,
};
CirQueue st_spi_flash_operate_response_queue =
{
    &pv_spi_flash_operate_response_queue_buffer[0],
    0,
    0,
    cb_max_spi_flash_operate_response_queue_len,
    0,
};

static void spi_flash_operate_queue_Semaphore_creat()
{
    pst_spi_flash_operate_initiate_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_spi_flash_operate_initiate_mutex)
    {

    }

    pst_spi_flash_operate_response_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_spi_flash_operate_response_mutex)
    {

    }
}

uint8_t spi_flash_operate_initiate_queue_send(pv_queue_operate_msg pv_queue_data)
{
    return queue_operate_send(pst_spi_flash_operate_initiate_mutex,\
                              &st_spi_flash_operate_initiate_queue,\
                              pv_queue_data);
}

uint8_t spi_flash_operate_initiate_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue)
{
    return queue_receive_operate(pst_spi_flash_operate_initiate_mutex,\
                                 &st_spi_flash_operate_initiate_queue,\
                                 pv_queue_data,\
                                 f_shift_queue);
}

uint8_t spi_flash_operate_response_queue_send(pv_queue_operate_msg pv_queue_data)
{
    return queue_operate_send(pst_spi_flash_operate_response_mutex,\
                              &st_spi_flash_operate_response_queue,\
                              pv_queue_data);
}

uint8_t spi_flash_operate_response_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue)
{
    return queue_receive_operate(pst_spi_flash_operate_response_mutex,\
                                 &st_spi_flash_operate_response_queue,\
                                 pv_queue_data,\
                                 f_shift_queue);
}
uint8_t spi_flash_operate_initiate_queue_shift()
{
    return queue_shift_operate(pst_spi_flash_operate_initiate_mutex,\
                               &st_spi_flash_operate_initiate_queue,\
                               TRUE,\
                               1);
}
uint8_t spi_flash_operate_response_queue_shift()
{
    return queue_shift_operate(pst_spi_flash_operate_response_mutex,\
                               &st_spi_flash_operate_response_queue,\
                               TRUE,\
                               1);
}

void spi_flash_operate_queue_init()
{
    spi_flash_operate_queue_Semaphore_creat();
}
