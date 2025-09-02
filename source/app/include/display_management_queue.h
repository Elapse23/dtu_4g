#ifndef _SPI_FLASH_OPERATE_QUEUE_H_
#define _SPI_FLASH_OPERATE_QUEUE_H_

#include "msg_queue_operate.h"

uint8_t display_management_queue_send(pv_queue_operate_msg pv_queue_data);
uint8_t display_management_queue_receive(pv_queue_operate_msg pv_queue_data,uint8_t f_shift_queue);
uint8_t display_management_queue_shift();
uint8_t display_management_queue_clear();
void display_management_queue_init();

#endif
