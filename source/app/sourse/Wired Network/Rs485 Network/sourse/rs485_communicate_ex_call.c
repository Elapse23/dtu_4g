#include "rs485_communicate_ex_call.h"
#include "rs485_communication_transmission.h"
#include "rs485_communicate_protocal.h"
#include "application_general_macro.h"
#include "product_test.h"


uint8_t rs485_inquire_cmd_send_ex_call(uint32_t dw_device_address,uint16_t w_data_addr,uint16_t w_data_in_len,uint8_t *pb_data_in)
{
	return rs485_tx_data_foreground(dw_device_address,cb_rs485_cmd_type_inquire,w_data_addr,w_data_in_len,pb_data_in,TRUE);
}

uint8_t rs485_set_cmd_send_ex_call(uint32_t dw_device_address,uint16_t w_data_addr,uint16_t w_data_in_len,uint8_t *pb_data_in)
{
	return rs485_tx_data_foreground(dw_device_address,cb_rs485_cmd_type_set,w_data_addr,w_data_in_len,pb_data_in,TRUE);
}

uint8_t rs485_ctrl_cmd_send_ex_call(uint32_t dw_device_address,uint16_t w_data_addr,uint16_t w_data_in_len,uint8_t *pb_data_in)
{
	return rs485_tx_data_foreground(dw_device_address,cb_rs485_cmd_type_ctrl,w_data_addr,w_data_in_len,pb_data_in,TRUE);
}

uint8_t rs485_inquire_device_tree_cmd_send_ex_call(uint32_t dw_device_address,uint16_t w_data_addr,uint16_t w_data_in_len,uint8_t *pb_data_in)
{
    return rs485_tx_data_foreground(dw_device_address,cb_rs485_cmd_type_inquire_device_tree,w_data_addr,w_data_in_len,pb_data_in,TRUE);
}

uint8_t rs485_inquire_event_logs_cmd_send_ex_call(uint32_t dw_device_address,uint16_t w_data_addr,uint16_t w_data_in_len,uint8_t *pb_data_in)
{
    return rs485_tx_data_foreground(dw_device_address,cb_rs485_cmd_type_event_logs,w_data_addr,w_data_in_len,pb_data_in,TRUE);
}

uint8_t rs485_product_test_cmd_send_ex_call(uint32_t dw_device_address,uint16_t w_data_addr,uint16_t w_data_in_len,uint8_t *pb_data_in)
{
    return rs485_tx_data_foreground(dw_device_address,cb_rs485_cmd_type_product_test,w_data_addr,w_data_in_len,pb_data_in,TRUE);
}
uint8_t rs485_product_test_inquire_cmd_send_ex_call(uint32_t dw_device_address,uint16_t w_data_addr,uint16_t w_data_in_len,uint8_t *pb_data_in)
{
    return rs485_tx_data_foreground(dw_device_address,cb_rs485_cmd_type_product_test_inquire,w_data_addr,w_data_in_len,pb_data_in,TRUE);
}
uint8_t rs485_subset_update_pac_cmd_send_ex_call(uint32_t dw_device_address,uint16_t w_data_addr,uint16_t w_data_in_len,uint8_t *pb_data_in)
{
    return rs485_tx_data_foreground(dw_device_address,cb_rs485_cmd_type_subset_update_pac,w_data_addr,w_data_in_len,pb_data_in,TRUE);
}
uint8_t rs485_subset_update_pacx_mcu_cmd_send_ex_call(uint32_t dw_device_address,uint16_t w_data_addr,uint16_t w_data_in_len,uint8_t *pb_data_in)
{
    return rs485_tx_data_foreground(dw_device_address,cb_rs485_cmd_type_subset_update_pacx_mcu,w_data_addr,w_data_in_len,pb_data_in,TRUE);
}
uint8_t rs485_subset_update_pacx_dsp_cmd_send_ex_call(uint32_t dw_device_address,uint16_t w_data_addr,uint16_t w_data_in_len,uint8_t *pb_data_in)
{
    return rs485_tx_data_foreground(dw_device_address,cb_rs485_cmd_type_subset_update_pacx_dsp,w_data_addr,w_data_in_len,pb_data_in,TRUE);
}
uint8_t rs485_transparent_cmd_send_ex_call(uint32_t dw_device_address,uint16_t w_data_addr,uint16_t w_data_in_len,uint8_t *pb_data_in)
{
    return rs485_tx_data_foreground(dw_device_address,cb_rs485_cmd_type_subset_update_transparent_cmd,w_data_addr,w_data_in_len,pb_data_in,TRUE);
}

uint8_t rs485_inv_auxiliary_installation_cmd_send_ex_call(uint32_t dw_device_address,uint16_t w_data_addr,uint16_t w_data_in_len,uint8_t *pb_data_in)
{
    return rs485_tx_data_foreground(dw_device_address,cb_rs485_cmd_type_inv_auxiliary_installation,w_data_addr,w_data_in_len,pb_data_in,TRUE);
}
uint8_t rs485_mppt_auxiliary_installation_cmd_send_ex_call(uint32_t dw_device_address,uint16_t w_data_addr,uint16_t w_data_in_len,uint8_t *pb_data_in)
{
    return rs485_tx_data_foreground(dw_device_address,cb_rs485_cmd_type_mppt_auxiliary_installation,w_data_addr,w_data_in_len,pb_data_in,TRUE);
}


uint8_t rs485_inquire_cmd_receive_ex_call(uint32_t dw_device_address,
                                          uint16_t w_data_addr,
                                          uint16_t *pw_data_out_len,
                                          uint8_t *pb_data_out,
                                          uint16_t w_data_out_buffer_size,
										  uint8_t *pb_receive_state)
{
	static TickType_t x_start_time = 0;
	return rs485_rx_query_data_analysis(dw_device_address,
                                  cb_rs485_cmd_type_inquire,
                                  w_data_addr,
                                  pw_data_out_len,
                                  pb_data_out,
                                  w_data_out_buffer_size,
                                  pb_receive_state,
								  &x_start_time);
}

uint8_t rs485_set_cmd_receive_ex_call(uint32_t dw_device_address,
                                          uint16_t w_data_addr,
                                          uint16_t *pw_data_out_len,
                                          uint8_t *pb_data_out,
                                          uint16_t w_data_out_buffer_size,
										  uint8_t *pb_receive_state)
{
	static TickType_t x_start_time = 0;
	return rs485_rx_set_query_data_analysis(dw_device_address,
                                  cb_rs485_cmd_type_set,
                                  w_data_addr,
                                  pw_data_out_len,
                                  pb_data_out,
                                  w_data_out_buffer_size,
                                  pb_receive_state,
    							  &x_start_time);                        
  

}
uint8_t rs485_auxiliary_installation_cmd_receive_ex_call(uint32_t dw_device_address,
                                                                    uint16_t w_data_addr,
                                                                    uint16_t *pw_data_out_len,
                                                                    uint8_t *pb_data_out,
                                                                    uint16_t w_data_out_buffer_size,
                                                                    uint8_t *pb_receive_state)
{
	static TickType_t x_start_time = 0;
	return rs485_rx_set_query_data_analysis(dw_device_address,
                              cb_rs485_cmd_type_inv_auxiliary_installation,
                              w_data_addr,
                              pw_data_out_len,
                              pb_data_out,
                              w_data_out_buffer_size,
                              pb_receive_state,
							  &x_start_time);   

}
uint8_t rs485_mppt_auxiliary_installation_cmd_receive_ex_call(uint32_t dw_device_address,
                                                                    uint16_t w_data_addr,
                                                                    uint16_t *pw_data_out_len,
                                                                    uint8_t *pb_data_out,
                                                                    uint16_t w_data_out_buffer_size,
                                                                    uint8_t *pb_receive_state)
{
	static TickType_t x_start_time = 0;
	return rs485_rx_set_query_data_analysis(dw_device_address,
                              cb_rs485_cmd_type_mppt_auxiliary_installation,
                              w_data_addr,
                              pw_data_out_len,
                              pb_data_out,
                              w_data_out_buffer_size,
                              pb_receive_state,
							  &x_start_time);   

}

uint8_t rs485_ctrl_cmd_receive_ex_call(uint32_t dw_device_address,
                                          uint16_t w_data_addr,
                                          uint16_t *pw_data_out_len,
                                          uint8_t *pb_data_out,
                                          uint16_t w_data_out_buffer_size,
										  uint8_t *pb_receive_state)
{
	static TickType_t x_start_time = 0;
	return rs485_rx_set_query_data_analysis(dw_device_address,
                                  cb_rs485_cmd_type_ctrl,
                                  w_data_addr,
                                  pw_data_out_len,
                                  pb_data_out,
                                  w_data_out_buffer_size,
                                  pb_receive_state,
								  &x_start_time);
}

uint8_t rs485_inquire_device_tree_cmd_receive_ex_call(uint32_t dw_device_address,
                                                      uint16_t w_data_addr,
                                                      uint16_t *pw_data_out_len,
                                                      uint8_t *pb_data_out,
                                                      uint16_t w_data_out_buffer_size,
												 	  uint8_t *pb_receive_state)
{
    return rs485_rx_subset_update_data_analysis(dw_device_address,
                                                cb_rs485_cmd_type_inquire_device_tree,
                                                w_data_addr,
                                                pw_data_out_len,
                                                pb_data_out,
                                                w_data_out_buffer_size,
                                                pb_receive_state);
}

uint8_t rs485_inquire_event_logs_cmd_receive_ex_call(uint32_t dw_device_address,
                                                      uint16_t w_data_addr,
                                                      uint16_t *pw_data_out_len,
                                                      uint8_t *pb_data_out,
                                                      uint16_t w_data_out_buffer_size,
												 	  uint8_t *pb_receive_state)
{
	static TickType_t x_start_time = 0;
    return rs485_rx_query_data_analysis(dw_device_address,
		                                  cb_rs485_cmd_type_event_logs,
		                                  w_data_addr,
		                                  pw_data_out_len,
		                                  pb_data_out,
		                                  w_data_out_buffer_size,
		                                  pb_receive_state,
										  &x_start_time);
}

uint8_t rs485_product_test_cmd_receive_ex_call(uint32_t dw_device_address,
                                               uint16_t w_data_addr,
                                               uint16_t *pw_data_out_len,
                                               uint8_t *pb_data_out,
                                               uint16_t w_data_out_buffer_size,
                                               uint8_t *pb_receive_state)
{
#if(product_test_rs485_enable)
    return product_test_analysis(dw_device_address,
                                 cb_rs485_cmd_type_product_test,
                                 w_data_addr,
                                 pw_data_out_len,
                                 pb_data_out,
                                 w_data_out_buffer_size,
                                 pb_receive_state);
#else
    return FALSE;
#endif
}

uint8_t rs485_product_test_inquire_cmd_receive_ex_call(uint32_t dw_device_address,
                                                       uint16_t w_data_addr,
                                                       uint16_t *pw_data_out_len,
                                                       uint8_t *pb_data_out,
                                                       uint16_t w_data_out_buffer_size,
                                                       uint8_t *pb_receive_state)
{
#if(product_test_rs485_enable)
    return product_test_analysis(dw_device_address,
                                 cb_rs485_cmd_type_product_test_inquire,
                                 w_data_addr,
                                 pw_data_out_len,
                                 pb_data_out,
                                 w_data_out_buffer_size,
                                 pb_receive_state);
#else
    return FALSE;
#endif
}

uint8_t rs485_subset_update_pac_cmd_receive_ex_call(uint32_t dw_device_address,
                                                    uint16_t w_data_addr,
                                                    uint16_t *pw_data_out_len,
                                                    uint8_t *pb_data_out,
                                                    uint16_t w_data_out_buffer_size,
                                                    uint8_t *pb_receive_state)
{
    return rs485_rx_subset_update_data_analysis(dw_device_address,
                                                cb_rs485_cmd_type_subset_update_pac,
                                                w_data_addr,
                                                pw_data_out_len,
                                                pb_data_out,
                                                w_data_out_buffer_size,
                                                pb_receive_state);
}

uint8_t rs485_dsp_update_cmd_receive_ex_call(uint32_t dw_device_address,
                                              uint16_t w_data_addr,
                                              uint16_t *pw_data_out_len,
                                              uint8_t *pb_data_out,
                                              uint16_t w_data_out_buffer_size,
                                              uint8_t *pb_receive_state)
{
	static TickType_t x_start_time = 0;
    return rs485_rx_data_analysis(dw_device_address,
                                  cb_rs485_cmd_type_subset_update_pacx_dsp,
                                  w_data_addr,
                                  pw_data_out_len,
                                  pb_data_out,
                                  w_data_out_buffer_size,
                                  pb_receive_state,
								  &x_start_time);
}

uint8_t rs485_transparent_cmd_receive_ex_call(uint32_t dw_device_address,
                                              uint16_t w_data_addr,
                                              uint16_t *pw_data_out_len,
                                              uint8_t *pb_data_out,
                                              uint16_t w_data_out_buffer_size,
                                              uint8_t *pb_receive_state)
{
    return rs485_transmission_rx_transparent_data(dw_device_address,
                                                  cb_rs485_cmd_type_subset_update_transparent_cmd,
                                                  w_data_addr,
                                                  pw_data_out_len,
                                                  pb_data_out,
                                                  w_data_out_buffer_size,
                                                  pb_receive_state);
}
