#ifndef _RS485_COMMUNICATION_PROTOCAL_H_
#define _RS485_COMMUNICATION_PROTOCAL_H_

#include <ysizet.h>
#include <stdint.h>

#define cb_rs485_header_len         6

#define cb_rs485_cmd_type_inquire                           0
#define cb_rs485_cmd_type_set                               1
#define cb_rs485_cmd_type_ctrl                              2
#define cb_rs485_cmd_type_update                            3
#define cb_rs485_cmd_type_inquire_device_tree               4
#define cb_rs485_cmd_type_inquire_device_alarm              5
#define cb_rs485_cmd_type_product_test                      6
#define cb_rs485_cmd_type_event_logs                        7
#define cb_rs485_cmd_type_product_test_inquire              8
#define cb_rs485_cmd_type_subset_update_pac                 9
#define cb_rs485_cmd_type_subset_update_pacx_mcu            10
#define cb_rs485_cmd_type_subset_update_pacx_dsp            11
#define cb_rs485_cmd_type_subset_update_transparent_cmd     12
#define cb_rs485_cmd_type_inv_auxiliary_installation        13
#define cb_rs485_cmd_type_mppt_auxiliary_installation       14

extern void * pvPortMalloc( size_t xWantedSize );
extern void vPortFree( void * pv );


typedef struct
{
    uint8_t b_device_id;
    uint8_t b_first_cmd;
    uint8_t b_sub_cmd;
    uint8_t *pb_body_data;
    uint16_t w_body_data_len;
}rs485_protocal_info;

typedef struct
{
    uint8_t b_com_addr;
    uint8_t b_func_code;
    uint16_t w_ctrl_addr;
    uint16_t w_body_data_len;
    uint8_t *pb_body_data;
}product_test_protocal_info;

uint8_t rs485_protocal_tx_data_assamble(uint8_t b_device_id,
                                        uint16_t w_cmd,
                                        uint16_t w_body_data_len,
                                        uint8_t *pb_body_data,
                                        uint8_t *pb_data_out,
                                        uint16_t *pw_data_out_len);

uint8_t rs485_protocal_tx_inv_auxiliary_installation_assamble(uint8_t b_device_id,
                                        uint16_t w_cmd,
                                        uint16_t w_body_data_len,
                                        uint8_t *pb_body_data,
                                        uint8_t *pb_data_out,
                                        uint16_t *pw_data_out_len);
uint8_t rs485_protocal_tx_mppt_auxiliary_installation_assamble(uint8_t b_device_id,
                                        uint16_t w_cmd,
                                        uint16_t w_body_data_len,
                                        uint8_t *pb_body_data,
                                        uint8_t *pb_data_out,
                                        uint16_t *pw_data_out_len);


uint8_t rs485_protocal_tx_update_cmd_assamble(uint8_t b_device_id,
                                              uint16_t w_cmd,
                                              uint16_t w_body_data_len,
                                              uint8_t *pb_body_data,
                                              uint8_t *pb_data_out,
                                              uint16_t *pw_data_out_len);

uint8_t rs485_product_test_tx_data_assamble(uint8_t b_com_addr,
                                            uint8_t b_func_code,
                                            uint16_t w_ctrl_addr,
                                            uint16_t w_body_data_len,
                                            uint8_t *pb_body_data,
                                            uint8_t *pb_data_out,
                                            uint16_t *pw_data_out_len);

uint8_t send_rs485_event_raw_data(uint8_t b_type,
                                  void *pv_data,
                                  uint16_t *pw_data_out_len,
                                  uint8_t *pb_data_out);


#endif
