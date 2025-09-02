#ifndef _RS485_COMMUNICATE_EX_CALL_H_
#define _RS485_COMMUNICATE_EX_CALL_H_

#include "stdint.h"
#include "product_test.h"



uint8_t rs485_inquire_cmd_send_ex_call(uint32_t dw_device_address,uint16_t w_data_addr,uint16_t w_data_in_len,uint8_t *pb_data_in);
uint8_t rs485_set_cmd_send_ex_call(uint32_t dw_device_address,uint16_t w_data_addr,uint16_t w_data_in_len,uint8_t *pb_data_in);
uint8_t rs485_ctrl_cmd_send_ex_call(uint32_t dw_device_address,uint16_t w_data_addr,uint16_t w_data_in_len,uint8_t *pb_data_in);
uint8_t rs485_inquire_device_tree_cmd_send_ex_call(uint32_t dw_device_address,uint16_t w_data_addr,uint16_t w_data_in_len,uint8_t *pb_data_in);
uint8_t rs485_product_test_cmd_send_ex_call(uint32_t dw_device_address,uint16_t w_data_addr,uint16_t w_data_in_len,uint8_t *pb_data_in);
uint8_t rs485_inquire_event_logs_cmd_send_ex_call(uint32_t dw_device_address,uint16_t w_data_addr,uint16_t w_data_in_len,uint8_t *pb_data_in);
uint8_t rs485_product_test_inquire_cmd_send_ex_call(uint32_t dw_device_address,uint16_t w_data_addr,uint16_t w_data_in_len,uint8_t *pb_data_in);
uint8_t rs485_subset_update_pac_cmd_send_ex_call(uint32_t dw_device_address,uint16_t w_data_addr,uint16_t w_data_in_len,uint8_t *pb_data_in);
uint8_t rs485_subset_update_pacx_mcu_cmd_send_ex_call(uint32_t dw_device_address,uint16_t w_data_addr,uint16_t w_data_in_len,uint8_t *pb_data_in);
uint8_t rs485_subset_update_pacx_dsp_cmd_send_ex_call(uint32_t dw_device_address,uint16_t w_data_addr,uint16_t w_data_in_len,uint8_t *pb_data_in);
uint8_t rs485_transparent_cmd_send_ex_call(uint32_t dw_device_address,uint16_t w_data_addr,uint16_t w_data_in_len,uint8_t *pb_data_in);
uint8_t rs485_inv_auxiliary_installation_cmd_send_ex_call(uint32_t dw_device_address,uint16_t w_data_addr,uint16_t w_data_in_len,uint8_t *pb_data_in);
uint8_t rs485_mppt_auxiliary_installation_cmd_send_ex_call(uint32_t dw_device_address,uint16_t w_data_addr,uint16_t w_data_in_len,uint8_t *pb_data_in);



uint8_t rs485_inquire_cmd_receive_ex_call(uint32_t dw_device_address,
                                          uint16_t w_data_addr,
                                          uint16_t *pw_data_out_len,
                                          uint8_t *pb_data_out,
                                          uint16_t w_data_out_buffer_size,
										  uint8_t *pb_receive_state);

uint8_t rs485_set_cmd_receive_ex_call(uint32_t dw_device_address,
                                          uint16_t w_data_addr,
                                          uint16_t *pw_data_out_len,
                                          uint8_t *pb_data_out,
                                          uint16_t w_data_out_buffer_size,
										  uint8_t *pb_receive_state);

uint8_t rs485_ctrl_cmd_receive_ex_call(uint32_t dw_device_address,
                                          uint16_t w_data_addr,
                                          uint16_t *pw_data_out_len,
                                          uint8_t *pb_data_out,
                                          uint16_t w_data_out_buffer_size,
										  uint8_t *pb_receive_state);

uint8_t rs485_inquire_device_tree_cmd_receive_ex_call(uint32_t dw_device_address,
                                                      uint16_t w_data_addr,
                                                      uint16_t *pw_data_out_len,
                                                      uint8_t *pb_data_out,
                                                      uint16_t w_data_out_buffer_size,
												 	  uint8_t *pb_receive_state);

uint8_t rs485_inquire_event_logs_cmd_receive_ex_call(uint32_t dw_device_address,
                                                      uint16_t w_data_addr,
                                                      uint16_t *pw_data_out_len,
                                                      uint8_t *pb_data_out,
                                                      uint16_t w_data_out_buffer_size,
												 	  uint8_t *pb_receive_state);

uint8_t rs485_product_test_cmd_receive_ex_call(uint32_t dw_device_address,
                                               uint16_t w_data_addr,
                                               uint16_t *pw_data_out_len,
                                               uint8_t *pb_data_out,
                                               uint16_t w_data_out_buffer_size,
                                               uint8_t *pb_receive_state);

uint8_t rs485_product_test_inquire_cmd_receive_ex_call(uint32_t dw_device_address,
                                                       uint16_t w_data_addr,
                                                       uint16_t *pw_data_out_len,
                                                       uint8_t *pb_data_out,
                                                       uint16_t w_data_out_buffer_size,
                                                       uint8_t *pb_receive_state);

uint8_t rs485_subset_update_pac_cmd_receive_ex_call(uint32_t dw_device_address,
                                                    uint16_t w_data_addr,
                                                    uint16_t *pw_data_out_len,
                                                    uint8_t *pb_data_out,
                                                    uint16_t w_data_out_buffer_size,
                                                    uint8_t *pb_receive_state);

uint8_t rs485_dsp_update_cmd_receive_ex_call(uint32_t dw_device_address,
                                              uint16_t w_data_addr,
                                              uint16_t *pw_data_out_len,
                                              uint8_t *pb_data_out,
                                              uint16_t w_data_out_buffer_size,
                                              uint8_t *pb_receive_state);

uint8_t rs485_transparent_cmd_receive_ex_call(uint32_t dw_device_address,
                                              uint16_t w_data_addr,
                                              uint16_t *pw_data_out_len,
                                              uint8_t *pb_data_out,
                                              uint16_t w_data_out_buffer_size,
                                              uint8_t *pb_receive_state);
uint8_t rs485_mppt_auxiliary_installation_cmd_receive_ex_call(uint32_t dw_device_address,
                                                                    uint16_t w_data_addr,
                                                                    uint16_t *pw_data_out_len,
                                                                    uint8_t *pb_data_out,
                                                                    uint16_t w_data_out_buffer_size,
                                                                    uint8_t *pb_receive_state);
uint8_t rs485_auxiliary_installation_cmd_receive_ex_call(uint32_t dw_device_address,
                                                                    uint16_t w_data_addr,
                                                                    uint16_t *pw_data_out_len,
                                                                    uint8_t *pb_data_out,
                                                                    uint16_t w_data_out_buffer_size,
                                                                    uint8_t *pb_receive_state);



#endif
