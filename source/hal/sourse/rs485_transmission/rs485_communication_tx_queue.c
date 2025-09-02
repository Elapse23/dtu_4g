#include "msg_queue.h"
#include "msg_queue_operate.h"
#include "rs485_communication_tx_queue.h"
#include <ysizet.h>
#include "FreeRTOS.h"//固定引用顺序 NO.1
#include "semphr.h"  //固定引用顺序 NO.2
#include "debug_printf_manage.h"
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

/*********** TX QUEUE ***********/

#define  cb_max_rs485_tx_data_queue_len          30

SemaphoreHandle_t  pst_rs485_tx_data_queue_mutex = NULL;

static void *pv_rs485_tx_data_queue_buffer[cb_max_rs485_tx_data_queue_len];

static void rs485_tx_data_queue_Semaphore_creat();

CirQueue st_rs485_tx_data_queue =
{
    &pv_rs485_tx_data_queue_buffer[0],
    0,
    0,
    cb_max_rs485_tx_data_queue_len,
    0,
};

static void rs485_tx_data_queue_Semaphore_creat()
{
    pst_rs485_tx_data_queue_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_rs485_tx_data_queue_mutex)
    {

    }
}

uint8_t rs485_tx_data_queue_send(pv_queue_operate_msg pv_queue_data,uint8_t b_cmd)
{
	if (b_cmd == cb_rs485_cmd_type_subset_update_pac\
		|| b_cmd == cb_rs485_cmd_type_subset_update_pacx_mcu\
		|| b_cmd == cb_rs485_cmd_type_subset_update_pacx_dsp\
		|| b_cmd == cb_rs485_cmd_type_subset_update_transparent_cmd\
		|| b_cmd == cb_rs485_cmd_type_update)
	{
		return queue_operate_send(NULL,\
								  &st_rs485_tx_data_queue,\
								  pv_queue_data);
	}
    return queue_operate_send(pst_rs485_tx_data_queue_mutex,\
                              &st_rs485_tx_data_queue,\
                              pv_queue_data);
}

uint8_t rs485_tx_data_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue)
{
    return queue_receive_operate(pst_rs485_tx_data_queue_mutex,\
                                 &st_rs485_tx_data_queue,\
                                 pv_queue_data,\
                                 f_shift_queue);
}

uint8_t rs485_tx_data_queue_shift()
{
    return queue_shift_operate(pst_rs485_tx_data_queue_mutex,\
                               &st_rs485_tx_data_queue,\
                               TRUE,\
                               1);
}
uint8_t rs485_tx_data_queue_clear()
{
    return queue_clear_operate(pst_rs485_tx_data_queue_mutex,\
                               &st_rs485_tx_data_queue);
}

/*********** RX POLLING QUEUE ***********/

/**
 * @brief 清空当前队列 当队列长度超过5时清空队列 以防止数据错乱
 * @param {SemaphoreHandle_t} *pst_queue_mutex
 * @param {CirQueue} *data_queue
 * @return {*}
 * @author Yao YiLiang
 */
uint8_t rs485_Clear_Queue(SemaphoreHandle_t *pst_queue_mutex, CirQueue *data_queue)
{
	static TickType_t xTicksToWait = 0;
	static TickType_t xTicksToWait1 = 0;

	if (pst_queue_mutex == NULL || data_queue == NULL)
	{
		return FALSE;
	}
	if(xTicksToWait - xTaskGetTickCount() > 723)
	{
		xTicksToWait1  -= (xTicksToWait - xTaskGetTickCount());
	}

	xTicksToWait = xTaskGetTickCount();

	if (data_queue->wQueueLen == 0)
	{
		xTicksToWait1 = xTaskGetTickCount();
	}

	if ( (xTaskGetTickCount() - xTicksToWait1) > pdMS_TO_TICKS_USER(1000) || data_queue->wQueueLen >= 5)
	{
		#if (r485_transmission_print)
		DebugPrintf(TRUE,"xTaskGetTickCount - xTicksToWait is :%d\r\n",
					xTaskGetTickCount() - xTicksToWait1);
		#endif
		rs485_rx_data_info *pst_rs485_rx_data_info = NULL;
		uint8_t b_queue_len = data_queue->wQueueLen;
		for (uint8_t b_cnt_i = 0; b_cnt_i < b_queue_len; b_cnt_i++)
		{
			queue_receive_operate(*pst_queue_mutex, data_queue, ( pv_queue_operate_msg )&pst_rs485_rx_data_info, TRUE);
			if (NULL != pst_rs485_rx_data_info)
			{
				if (pst_rs485_rx_data_info->pb_data)
				{
					vPortFree(pst_rs485_rx_data_info->pb_data);
				}
				vPortFree(pst_rs485_rx_data_info);
			}
		}
	}

	return TRUE;
}

#define  cb_rs485_rx_polling_queue_len          10

SemaphoreHandle_t  pst_rs485_rx_polling_data_queue_mutex = NULL;

static void *pv_rs485_rx_polling_data_queue_buffer[cb_rs485_rx_polling_queue_len];

static void rs485_rx_polling_data_queue_Semaphore_creat();

CirQueue st_rs485_rx_polling_data_queue =
{
    &pv_rs485_rx_polling_data_queue_buffer[0],
    0,
    0,
    cb_rs485_rx_polling_queue_len,
    0,
};

static void rs485_rx_polling_data_queue_Semaphore_creat()
{
    pst_rs485_rx_polling_data_queue_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_rs485_rx_polling_data_queue_mutex)
    {

    }
}

uint8_t rs485_rx_polling_data_queue_send(pv_queue_operate_msg pv_queue_data)
{
    return queue_operate_send(pst_rs485_rx_polling_data_queue_mutex,\
                              &st_rs485_rx_polling_data_queue,\
                              pv_queue_data);
}

uint8_t rs485_rx_polling_data_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue,uint8_t b_cmd)
{
	if (b_cmd == cb_rs485_cmd_type_subset_update_pacx_dsp)
	{
		rs485_Clear_Queue(NULL, &st_rs485_rx_polling_data_queue);
		return queue_receive_operate(NULL,\
			&st_rs485_rx_polling_data_queue,\
			pv_queue_data,\
			f_shift_queue);
	}

	rs485_Clear_Queue(&pst_rs485_rx_polling_data_queue_mutex, &st_rs485_rx_polling_data_queue);

	return queue_receive_operate(pst_rs485_rx_polling_data_queue_mutex,\
                                 &st_rs485_rx_polling_data_queue,\
                                 pv_queue_data,\
                                 f_shift_queue);
}

uint8_t rs485_rx_polling_data_queue_shift()
{
    return queue_shift_operate(pst_rs485_rx_polling_data_queue_mutex,\
                               &st_rs485_rx_polling_data_queue,\
                               TRUE,\
                               1);
}
uint8_t rs485_rx_polling_data_queue_clear()
{
    return queue_clear_operate(pst_rs485_rx_polling_data_queue_mutex,\
                               &st_rs485_rx_polling_data_queue);
}

/*******************query queue*************************/
#define  cb_rs485_rx_query_queue_len          10

SemaphoreHandle_t  pst_rs485_rx_query_data_queue_mutex = NULL;

static void *pv_rs485_rx_query_data_queue_buffer[cb_rs485_rx_polling_queue_len];

static void rs485_rx_query_data_queue_Semaphore_creat();

CirQueue st_rs485_rx_query_data_queue =
{
    &pv_rs485_rx_query_data_queue_buffer[0],
    0,
    0,
    cb_rs485_rx_query_queue_len,
    0,
};

static void rs485_rx_query_data_queue_Semaphore_creat()
{
    pst_rs485_rx_query_data_queue_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_rs485_rx_query_data_queue_mutex)
    {

    }
}

uint8_t rs485_rx_query_data_queue_send(pv_queue_operate_msg pv_queue_data)
{
    return queue_operate_send(pst_rs485_rx_query_data_queue_mutex,\
                              &st_rs485_rx_query_data_queue,\
                              pv_queue_data);
}

uint8_t rs485_rx_query_data_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue,uint8_t b_cmd)
{
	if (b_cmd == cb_rs485_cmd_type_subset_update_pacx_dsp)
	{
		rs485_Clear_Queue(NULL, &st_rs485_rx_query_data_queue);
		return queue_receive_operate(NULL,\
			&st_rs485_rx_query_data_queue,\
			pv_queue_data,\
			f_shift_queue);
	}

	rs485_Clear_Queue(&pst_rs485_rx_query_data_queue_mutex, &st_rs485_rx_query_data_queue);

	return queue_receive_operate(pst_rs485_rx_query_data_queue_mutex,\
                                 &st_rs485_rx_query_data_queue,\
                                 pv_queue_data,\
                                 f_shift_queue);
}

uint8_t rs485_rx_query_data_queue_shift()
{
    return queue_shift_operate(pst_rs485_rx_query_data_queue_mutex,\
                               &st_rs485_rx_query_data_queue,\
                               TRUE,\
                               1);
}
uint8_t rs485_rx_query_data_queue_clear()
{
    return queue_clear_operate(pst_rs485_rx_query_data_queue_mutex,\
                               &st_rs485_rx_query_data_queue);
}
/*******************set queue*************************/
#define  cb_rs485_rx_set_queue_len          10

SemaphoreHandle_t  pst_rs485_rx_set_data_queue_mutex = NULL;

static void *pv_rs485_rx_set_data_queue_buffer[cb_rs485_rx_polling_queue_len];

static void rs485_rx_set_data_queue_Semaphore_creat();

CirQueue st_rs485_rx_set_data_queue =
{
    &pv_rs485_rx_set_data_queue_buffer[0],
    0,
    0,
    cb_rs485_rx_set_queue_len,
    0,
};

static void rs485_rx_set_data_queue_Semaphore_creat()
{
    pst_rs485_rx_set_data_queue_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_rs485_rx_set_data_queue_mutex)
    {

    }
}

uint8_t rs485_rx_set_data_queue_send(pv_queue_operate_msg pv_queue_data)
{
    return queue_operate_send(pst_rs485_rx_set_data_queue_mutex,\
                              &st_rs485_rx_set_data_queue,\
                              pv_queue_data);
}

uint8_t rs485_rx_set_data_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue,uint8_t b_cmd)
{
	if (b_cmd == cb_rs485_cmd_type_subset_update_pacx_dsp)
	{
		rs485_Clear_Queue(NULL, &st_rs485_rx_set_data_queue);
		return queue_receive_operate(NULL,\
			&st_rs485_rx_set_data_queue,\
			pv_queue_data,\
			f_shift_queue);
	}

	rs485_Clear_Queue(&pst_rs485_rx_set_data_queue_mutex, &st_rs485_rx_set_data_queue);

	return queue_receive_operate(pst_rs485_rx_set_data_queue_mutex,\
                                 &st_rs485_rx_set_data_queue,\
                                 pv_queue_data,\
                                 f_shift_queue);
}

uint8_t rs485_rx_set_data_queue_shift()
{
    return queue_shift_operate(pst_rs485_rx_set_data_queue_mutex,\
                               &st_rs485_rx_set_data_queue,\
                               TRUE,\
                               1);
}
uint8_t rs485_rx_set_data_queue_clear()
{
    return queue_clear_operate(pst_rs485_rx_set_data_queue_mutex,\
                               &st_rs485_rx_set_data_queue);
}
/*******************set queue*************************/
#define  cb_rs485_rx_other_queue_len          10

SemaphoreHandle_t  pst_rs485_rx_other_data_queue_mutex = NULL;

static void *pv_rs485_rx_other_data_queue_buffer[cb_rs485_rx_polling_queue_len];

static void rs485_rx_other_data_queue_Semaphore_creat();

CirQueue st_rs485_rx_other_data_queue =
{
    &pv_rs485_rx_other_data_queue_buffer[0],
    0,
    0,
    cb_rs485_rx_other_queue_len,
    0,
};

static void rs485_rx_other_data_queue_Semaphore_creat()
{
    pst_rs485_rx_other_data_queue_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_rs485_rx_other_data_queue_mutex)
    {

    }
}

uint8_t rs485_rx_other_data_queue_send(pv_queue_operate_msg pv_queue_data)
{
    return queue_operate_send(pst_rs485_rx_other_data_queue_mutex,\
                              &st_rs485_rx_other_data_queue,\
                              pv_queue_data);
}

uint8_t rs485_rx_other_data_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue,uint8_t b_cmd)
{
	if (b_cmd == cb_rs485_cmd_type_subset_update_pacx_dsp)
	{
		rs485_Clear_Queue(NULL, &st_rs485_rx_other_data_queue);
		return queue_receive_operate(NULL,\
			&st_rs485_rx_other_data_queue,\
			pv_queue_data,\
			f_shift_queue);
	}

	rs485_Clear_Queue(&pst_rs485_rx_other_data_queue_mutex, &st_rs485_rx_other_data_queue);

	return queue_receive_operate(pst_rs485_rx_other_data_queue_mutex,\
                                 &st_rs485_rx_other_data_queue,\
                                 pv_queue_data,\
                                 f_shift_queue);
}

uint8_t rs485_rx_other_data_queue_shift()
{
    return queue_shift_operate(pst_rs485_rx_other_data_queue_mutex,\
                               &st_rs485_rx_other_data_queue,\
                               TRUE,\
                               1);
}
uint8_t rs485_rx_other_data_queue_clear()
{
    return queue_clear_operate(pst_rs485_rx_other_data_queue_mutex,\
                               &st_rs485_rx_other_data_queue);
}

/*********** RX PRODUCT TEST QUEUE ***********/

#define  cb_rs485_rx_product_test_queue_len          10

SemaphoreHandle_t  pst_rs485_rx_product_test_data_queue_mutex = NULL;

static void *pv_rs485_rx_product_test_data_queue_buffer[cb_rs485_rx_product_test_queue_len];

static void rs485_rx_product_test_data_queue_Semaphore_creat();

CirQueue st_rs485_rx_product_test_data_queue =
{
    &pv_rs485_rx_product_test_data_queue_buffer[0],
    0,
    0,
    cb_rs485_rx_product_test_queue_len,
    0,
};

static void rs485_rx_product_test_data_queue_Semaphore_creat()
{
    pst_rs485_rx_product_test_data_queue_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_rs485_rx_product_test_data_queue_mutex)
    {

    }
}

uint8_t rs485_rx_product_test_data_queue_send(pv_queue_operate_msg pv_queue_data)
{
    return queue_operate_send(pst_rs485_rx_product_test_data_queue_mutex,\
                              &st_rs485_rx_product_test_data_queue,\
                              pv_queue_data);
}

uint8_t rs485_rx_product_test_data_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue)
{
    return queue_receive_operate(pst_rs485_rx_product_test_data_queue_mutex,\
                                 &st_rs485_rx_product_test_data_queue,\
                                 pv_queue_data,\
                                 f_shift_queue);
}

uint8_t rs485_rx_product_test_data_queue_shift()
{
    return queue_shift_operate(pst_rs485_rx_product_test_data_queue_mutex,\
                               &st_rs485_rx_product_test_data_queue,\
                               TRUE,\
                               1);
}
uint8_t rs485_rx_product_test_data_queue_clear()
{
    return queue_clear_operate(pst_rs485_rx_product_test_data_queue_mutex,\
                               &st_rs485_rx_product_test_data_queue);
}

/*********** RX LOCAL UPDATE QUEUE ***********/

#define  cb_rs485_rx_local_update_queue_len          10

SemaphoreHandle_t  pst_rs485_rx_local_update_data_queue_mutex = NULL;

static void *pv_rs485_rx_local_update_data_queue_buffer[cb_rs485_rx_local_update_queue_len];

static void rs485_rx_local_update_data_queue_Semaphore_creat();

CirQueue st_rs485_rx_local_update_data_queue =
{
    &pv_rs485_rx_local_update_data_queue_buffer[0],
    0,
    0,
    cb_rs485_rx_local_update_queue_len,
    0,
};

static void rs485_rx_local_update_data_queue_Semaphore_creat()
{
    pst_rs485_rx_local_update_data_queue_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_rs485_rx_local_update_data_queue_mutex)
    {

    }
}

uint8_t rs485_rx_local_update_data_queue_send(pv_queue_operate_msg pv_queue_data)
{
    return queue_operate_send(pst_rs485_rx_local_update_data_queue_mutex,\
                              &st_rs485_rx_local_update_data_queue,\
                              pv_queue_data);
}

uint8_t rs485_rx_local_update_data_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue)
{
    return queue_receive_operate(pst_rs485_rx_local_update_data_queue_mutex,\
                                 &st_rs485_rx_local_update_data_queue,\
                                 pv_queue_data,\
                                 f_shift_queue);
}

uint8_t rs485_rx_local_update_data_queue_shift()
{
    return queue_shift_operate(pst_rs485_rx_local_update_data_queue_mutex,\
                               &st_rs485_rx_local_update_data_queue,\
                               TRUE,\
                               1);
}
uint8_t rs485_rx_local_update_data_queue_clear()
{
    return queue_clear_operate(pst_rs485_rx_local_update_data_queue_mutex,\
                               &st_rs485_rx_local_update_data_queue);
}
/*********** RX SUBSET UPDATE QUEUE ***********/

#define  cb_rs485_rx_subset_update_queue_len          10

SemaphoreHandle_t  pst_rs485_rx_subset_update_data_queue_mutex = NULL;

static void *pv_rs485_rx_subset_update_data_queue_buffer[cb_rs485_rx_subset_update_queue_len];

static void rs485_rx_subset_update_data_queue_Semaphore_creat();

CirQueue st_rs485_rx_subset_update_data_queue =
{
    &pv_rs485_rx_subset_update_data_queue_buffer[0],
    0,
    0,
    cb_rs485_rx_subset_update_queue_len,
    0,
};

static void rs485_rx_subset_update_data_queue_Semaphore_creat()
{
    pst_rs485_rx_subset_update_data_queue_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_rs485_rx_subset_update_data_queue_mutex)
    {

    }
}

uint8_t rs485_rx_subset_update_data_queue_send(pv_queue_operate_msg pv_queue_data)
{
    return queue_operate_send(pst_rs485_rx_subset_update_data_queue_mutex,\
                              &st_rs485_rx_subset_update_data_queue,\
                              pv_queue_data);
}

uint8_t rs485_rx_subset_update_data_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue,uint8_t b_cmd)
{
	if (b_cmd == cb_rs485_cmd_type_subset_update_pac)
	{
		return queue_receive_operate(NULL,\
			&st_rs485_rx_subset_update_data_queue,\
			pv_queue_data,\
			f_shift_queue);
	}
    return queue_receive_operate(pst_rs485_rx_subset_update_data_queue_mutex,\
                                 &st_rs485_rx_subset_update_data_queue,\
                                 pv_queue_data,\
                                 f_shift_queue);
}

uint8_t rs485_rx_subset_update_data_queue_shift()
{
    return queue_shift_operate(pst_rs485_rx_subset_update_data_queue_mutex,\
                               &st_rs485_rx_subset_update_data_queue,\
                               TRUE,\
                               1);
}
uint8_t rs485_rx_subset_update_data_queue_clear()
{
    return queue_clear_operate(pst_rs485_rx_subset_update_data_queue_mutex,\
                               &st_rs485_rx_subset_update_data_queue);
}

/*********** RX TRANSPARENT QUEUE ***********/
///*
#define  cb_rs485_rx_transparent_queue_len          10

SemaphoreHandle_t  pst_rs485_rx_transparent_data_queue_mutex = NULL;

static void *pv_rs485_rx_transparent_data_queue_buffer[cb_rs485_rx_transparent_queue_len];

static void rs485_rx_transparent_data_queue_Semaphore_creat();

CirQueue st_rs485_rx_transparent_data_queue =
{
    &pv_rs485_rx_transparent_data_queue_buffer[0],
    0,
    0,
    cb_rs485_rx_transparent_queue_len,
    0,
};

static void rs485_rx_transparent_data_queue_Semaphore_creat()
{
    pst_rs485_rx_transparent_data_queue_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_rs485_rx_transparent_data_queue_mutex)
    {

    }
}

uint8_t rs485_rx_transparent_data_queue_send(pv_queue_operate_msg pv_queue_data)
{
    return queue_operate_send(pst_rs485_rx_transparent_data_queue_mutex,\
                              &st_rs485_rx_transparent_data_queue,\
                              pv_queue_data);
}

uint8_t rs485_rx_transparent_data_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue,uint8_t b_cmd)
{
	if (b_cmd == cb_rs485_cmd_type_subset_update_transparent_cmd)
	{
		return queue_receive_operate(NULL,\
			&st_rs485_rx_transparent_data_queue,\
			pv_queue_data,\
			f_shift_queue);
	}
    return queue_receive_operate(pst_rs485_rx_transparent_data_queue_mutex,\
                                 &st_rs485_rx_transparent_data_queue,\
                                 pv_queue_data,\
                                 f_shift_queue);
}

uint8_t rs485_rx_transparent_data_queue_shift()
{
    return queue_shift_operate(pst_rs485_rx_transparent_data_queue_mutex,\
                               &st_rs485_rx_transparent_data_queue,\
                               TRUE,\
                               1);
}
uint8_t rs485_rx_transparent_data_queue_clear()
{
    return queue_clear_operate(pst_rs485_rx_transparent_data_queue_mutex,\
                               &st_rs485_rx_transparent_data_queue);
}
//    */

void rs485_data_queue_init()
{
    rs485_tx_data_queue_Semaphore_creat();

    rs485_rx_polling_data_queue_Semaphore_creat();
    rs485_rx_product_test_data_queue_Semaphore_creat();
    rs485_rx_local_update_data_queue_Semaphore_creat();
    rs485_rx_subset_update_data_queue_Semaphore_creat();
    rs485_rx_transparent_data_queue_Semaphore_creat();
}
