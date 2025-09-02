#ifndef _SERVER_RESPONSE_QUEUE_H_
#define _SERVER_RESPONSE_QUEUE_H_

#include "msg_queue_operate.h"

/*server response status*/
enum net_connect_cmd_response
{
    cb_net_rsp_success = 1,
    cb_net_rsp_failed,
    cb_net_rsp_retry,
};

uint8_t network_server_response_queue_send(pv_queue_operate_msg pv_queue_data);
uint8_t network_server_response_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue);
uint8_t network_server_response_queue_shift();
void network_server_response_queue_init();

#endif
