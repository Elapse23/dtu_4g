#ifndef _RS485_QUEUE_H_
#define _RS485_QUEUE_H_

#include "application_general_macro.h"
#include "msg_queue_operate.h"
#include "rs485.h"
#include <stdint.h>
#include "rs485_communicate_protocal.h"

#define rs485_tx_data   genaral_communicate_data_info

typedef struct
{
    uint32_t dw_id;
    rs485_tx_data st_data_info;
}rs485_tx_data_info;

typedef struct
{
    uint16_t w_data_len;
    uint8_t *pb_data;
}rs485_rx_data_info;

typedef uint8_t (* pfunc_rs485_queue_send)(pv_queue_operate_msg pv_queue_data);

uint8_t rs485_tx_data_queue_send(pv_queue_operate_msg pv_queue_data,uint8_t b_cmd);
uint8_t rs485_tx_data_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue);
uint8_t rs485_tx_data_queue_shift();
uint8_t rs485_tx_data_queue_clear();

uint8_t rs485_rx_polling_data_queue_send(pv_queue_operate_msg pv_queue_data);
uint8_t rs485_rx_polling_data_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue,uint8_t b_cmd);
uint8_t rs485_rx_polling_data_queue_shift();
uint8_t rs485_rx_polling_data_queue_clear();

uint8_t rs485_rx_product_test_data_queue_send(pv_queue_operate_msg pv_queue_data);
uint8_t rs485_rx_product_test_data_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue);
uint8_t rs485_rx_product_test_data_queue_shift();
uint8_t rs485_rx_product_test_data_queue_clear();

uint8_t rs485_rx_local_update_data_queue_send(pv_queue_operate_msg pv_queue_data);
uint8_t rs485_rx_local_update_data_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue);
uint8_t rs485_rx_local_update_data_queue_shift();
uint8_t rs485_rx_local_update_data_queue_clear();

uint8_t rs485_rx_subset_update_data_queue_send(pv_queue_operate_msg pv_queue_data);
uint8_t rs485_rx_subset_update_data_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue,uint8_t b_cmd);
uint8_t rs485_rx_subset_update_data_queue_shift();
uint8_t rs485_rx_subset_update_data_queue_clear();

uint8_t rs485_rx_transparent_data_queue_send(pv_queue_operate_msg pv_queue_data);
uint8_t rs485_rx_transparent_data_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue,uint8_t b_cmd);
uint8_t rs485_rx_transparent_data_queue_shift();
uint8_t rs485_rx_transparent_data_queue_clear();

uint8_t rs485_rx_set_data_queue_send(pv_queue_operate_msg pv_queue_data);
uint8_t rs485_rx_set_data_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue,uint8_t b_cmd);
uint8_t rs485_rx_query_data_queue_send(pv_queue_operate_msg pv_queue_data);
uint8_t rs485_rx_query_data_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue,uint8_t b_cmd);

uint8_t rs485_rx_other_data_queue_send(pv_queue_operate_msg pv_queue_data);
uint8_t rs485_rx_other_data_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue,uint8_t b_cmd);
uint8_t rs485_rx_other_data_queue_shift();
uint8_t rs485_rx_other_data_queue_clear();




void rs485_data_queue_init();

#endif

