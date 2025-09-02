#ifndef _RS485_COMMUNICATION_H_
#define _RS485_COMMUNICATION_H_

#include <stdint.h>
#include "portmacro.h"

#define cb_rs485_communication_receive_state_wait						0
#define cb_rs485_communication_receive_state_fail						1
#define cb_rs485_communication_receive_state_success					2

#define rs485_protocal_header               0
#define product_test_header                 1
#define rs485_firmware_update_1             2
#define rs485_firmware_update_2             3
#define rs485_update_subset_cmd_header      4
#define rs485_wait                          0xFF

#define cb_max_protocal_num         5

typedef enum
{
    rs485_normal,
    rs485_block,
    rs485_fualt,
}rs485_com_state;


uint8_t rs485_tx_data_foreground(uint32_t dw_id,
                                 uint8_t b_cmd,
                                 uint16_t w_rs485_cmd,
                                 uint16_t w_data_in_len,
                                 uint8_t *pb_data_in,
                                 uint8_t b_block_en);
uint8_t rs485_rx_data_analysis(uint32_t dw_device_address,
                               uint8_t b_cmd,
                               uint16_t w_data_addr,
                               uint16_t *pw_data_out_len,
                               uint8_t *pb_data_out,
                               uint16_t w_data_out_buffer_size,
                               uint8_t *pb_receive_state,
							   TickType_t *x_start_time);
uint8_t rs485_rx_subset_update_data_analysis(uint32_t dw_device_address,
                                             uint8_t b_cmd,
                                             uint16_t w_data_addr,
                                             uint16_t *pw_data_out_len,
                                             uint8_t *pb_data_out,
                                             uint16_t w_data_out_buffer_size,
                                             uint8_t *pb_receive_state);
uint8_t rs485_transmission_rx_transparent_data(uint32_t dw_device_address,
                                               uint8_t b_cmd,
                                               uint16_t w_data_addr,
                                               uint16_t *pw_data_out_len,
                                               uint8_t *pb_data_out,
                                               uint16_t w_data_out_buffer_size,
                                               uint8_t *pb_receive_state);

void rs485_transmission_task_init();
void rs485_transmission_init();
void rs485_rx_data_background();
uint8_t rs485_transmission_rx_cook_data();
uint8_t rs485_data_write(uint32_t *pdw_device_id,uint8_t *pb_data_in,uint16_t w_data_len,void *pv_data);
uint8_t rs485_queue_recognition_conversion();


#endif

