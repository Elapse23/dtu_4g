#include "wired_network_config_general.h"
//#include "can_communicate_ex_call.h"
#include "application_general_macro.h"
//#include "can_networking_transmission.h"
#include "rs485_communicate_ex_call.h"
#include "rs485_communication_transmission.h"


uint8_t wired_network_send_data(uint8_t b_cable,
								uint8_t b_cmd,
								uint32_t dw_device_address,
								uint16_t w_data_addr,
								uint16_t w_data_in_len,
								uint8_t *pb_data_in)
{
	if(cb_wired_network_can_cable == b_cable)
	{
#if (can_enable)
		if(cb_wired_network_inquire_cmd == b_cmd)
		{
			return can_inquire_cmd_send_ex_call(dw_device_address,w_data_addr,w_data_in_len,pb_data_in);
		}
		else if(cb_wired_network_set_cmd == b_cmd)
		{
			return can_set_cmd_send_ex_call(dw_device_address,w_data_addr,w_data_in_len,pb_data_in);
		}
		else if(cb_wired_network_ctrl_cmd == b_cmd)
		{
			return can_contral_cmd_send_ex_call(dw_device_address,w_data_addr,w_data_in_len,pb_data_in);
		}
		else if(cb_wired_network_inquire_device_tree_cmd == b_cmd)
		{
			return can_inquire_device_tree_cmd_send_ex_call(dw_device_address,w_data_addr,w_data_in_len,pb_data_in);
		}
		else if(cb_wired_network_inquire_device_alarm_cmd == b_cmd)
		{
			return can_inquire_device_alarm_cmd_send_ex_call(dw_device_address,w_data_addr,w_data_in_len,pb_data_in);
		}
		else
#endif
		{
			return FALSE;
		}
	}
	else if(cb_wired_network_rs485_cable == b_cable)
	{
		if(cb_wired_network_inquire_cmd == b_cmd)
		{
			return rs485_inquire_cmd_send_ex_call(dw_device_address,w_data_addr,w_data_in_len,pb_data_in);
		}
		else if(cb_wired_network_set_cmd == b_cmd)
		{
			return rs485_set_cmd_send_ex_call(dw_device_address,w_data_addr,w_data_in_len,pb_data_in);
		}
		else if(cb_wired_network_ctrl_cmd == b_cmd)
		{
			return rs485_ctrl_cmd_send_ex_call(dw_device_address,w_data_addr,w_data_in_len,pb_data_in);
		}
        else if(cb_wired_network_inquire_device_tree_cmd == b_cmd)
        {
            return rs485_inquire_device_tree_cmd_send_ex_call(dw_device_address,w_data_addr,w_data_in_len,pb_data_in);
        }
        else if(cb_wired_network_product_test_cmd == b_cmd)
        {
            return rs485_product_test_cmd_send_ex_call(dw_device_address,w_data_addr,w_data_in_len,pb_data_in);
        }
        else if(cb_wired_network_event_logs_cmd == b_cmd)
        {
            return rs485_inquire_event_logs_cmd_send_ex_call(dw_device_address,w_data_addr,w_data_in_len,pb_data_in);
        }
        else if(cb_wired_network_product_test_inquire_cmd == b_cmd)
        {
            return rs485_product_test_inquire_cmd_send_ex_call(dw_device_address,w_data_addr,w_data_in_len,pb_data_in);
        }
        else if(cb_wired_network_subset_update_pac_cmd == b_cmd)
        {
            return rs485_subset_update_pac_cmd_send_ex_call(dw_device_address,w_data_addr,w_data_in_len,pb_data_in);
        }
        else if(cb_wired_network_subset_update_pacx_mcu_cmd == b_cmd)
        {
            return rs485_subset_update_pacx_mcu_cmd_send_ex_call(dw_device_address,w_data_addr,w_data_in_len,pb_data_in);
        }
        else if(cb_wired_network_subset_update_pacx_dsp_cmd == b_cmd)
        {
            return rs485_subset_update_pacx_dsp_cmd_send_ex_call(dw_device_address,w_data_addr,w_data_in_len,pb_data_in);
        }
        else if(cb_wired_network_subset_update_transparent_cmd == b_cmd)
        {
            return rs485_transparent_cmd_send_ex_call(dw_device_address,w_data_addr,w_data_in_len,pb_data_in);
        }
		else if (cb_wired_network_inv_auxiliary_installation_cmd == b_cmd)
		{
			return rs485_inv_auxiliary_installation_cmd_send_ex_call(dw_device_address,w_data_addr,w_data_in_len,pb_data_in);
		}
        else if (cb_wired_network_mppt_auxiliary_installation_cmd == b_cmd)
		{
			return rs485_mppt_auxiliary_installation_cmd_send_ex_call(dw_device_address,w_data_addr,w_data_in_len,pb_data_in);
		}
        else
		{
			return FALSE;
		}
	}

	return FALSE;
}

uint8_t receive_state_switch_can_2_general(uint8_t b_can_state,uint8_t *b_general_state)
{
	switch(b_can_state)
	{
#if (can_enable)
		case cb_can_network_receive_state_wait:
		{
			*b_general_state = cb_wired_network_receive_state_wait;
		}break;
		case cb_can_network_receive_state_fail:
		{
			*b_general_state = cb_wired_network_receive_state_fail;
		}break;
		case cb_can_network_receive_state_success:
		{
			*b_general_state = cb_wired_network_receive_state_success;
		}break;
#endif
		default:
		{
			return FALSE;
		}break;
	}
#if (can_enable)
	return TRUE;
#endif
}

uint8_t wired_network_receive_data(uint8_t b_cable,
								   uint8_t b_cmd,
								   uint32_t dw_device_address,
								   uint16_t w_data_addr,
								   uint16_t *pw_data_out_len,
								   uint8_t *pb_data_out,
                                   uint16_t w_data_out_buffer_size,
								   uint8_t *pb_receive_state)
{
	if(cb_wired_network_can_cable == b_cable)
	{
#if (can_enable)
		if(cb_wired_network_inquire_cmd == b_cmd)
		{
			if(TRUE == can_inquire_cmd_receive_ex_call(dw_device_address,
                                                   	   w_data_addr,
                                                   	   pw_data_out_len,
                                                   	   pb_data_out,
                                                   	   w_data_out_buffer_size,
												   	   pb_receive_state))
			{
				return receive_state_switch_can_2_general(*pb_receive_state,pb_receive_state);
			}

			return FALSE;
		}
		else if(cb_wired_network_set_cmd == b_cmd)
		{
			if(TRUE == can_set_cmd_receive_ex_call(dw_device_address,
                                              	   w_data_addr,
                                              	   pw_data_out_len,
                                              	   pb_data_out,
                                              	   w_data_out_buffer_size,
											  	   pb_receive_state))
			{
				return receive_state_switch_can_2_general(*pb_receive_state,pb_receive_state);
			}

			return FALSE;
		}
		else if(cb_wired_network_ctrl_cmd == b_cmd)
		{
			if(TRUE == can_contral_cmd_receive_ex_call(dw_device_address,
                                                   	   w_data_addr,
                                                   	   pw_data_out_len,
                                                   	   pb_data_out,
                                                   	   w_data_out_buffer_size,
												   	   pb_receive_state))
			{
				return receive_state_switch_can_2_general(*pb_receive_state,pb_receive_state);
			}

			return FALSE;
		}
		else if(cb_wired_network_inquire_device_tree_cmd == b_cmd)
		{
			if(TRUE == can_inquire_device_tree_cmd_receive_ex_call(dw_device_address,
                                                               	   w_data_addr,
                                                               	   pw_data_out_len,
                                                               	   pb_data_out,
                                                               	   w_data_out_buffer_size,
															   	   pb_receive_state))
			{
				return receive_state_switch_can_2_general(*pb_receive_state,pb_receive_state);
			}

			return FALSE;
		}
		else if(cb_wired_network_inquire_device_alarm_cmd == b_cmd)
		{
			if(TRUE == can_inquire_device_alarm_cmd_receive_ex_call(dw_device_address,
                                                               	    w_data_addr,
                                                               	    pw_data_out_len,
                                                               	    pb_data_out,
                                                               	    w_data_out_buffer_size,
															   	    pb_receive_state))
			{
				return receive_state_switch_can_2_general(*pb_receive_state,pb_receive_state);
			}

			return FALSE;
		}
		else
#endif
		{
			return FALSE;
		}
	}
	else if(cb_wired_network_rs485_cable == b_cable)
	{
		if(cb_wired_network_inquire_cmd == b_cmd)
		{
			return rs485_inquire_cmd_receive_ex_call(dw_device_address,
                                                     w_data_addr,
                                                     pw_data_out_len,
                                                     pb_data_out,
                                                     w_data_out_buffer_size,
                                                     pb_receive_state);
		}
		else if(cb_wired_network_set_cmd == b_cmd)
		{
			return rs485_set_cmd_receive_ex_call(dw_device_address,
                                                     w_data_addr,
                                                     pw_data_out_len,
                                                     pb_data_out,
                                                     w_data_out_buffer_size,
                                                     pb_receive_state);
		}
		else if(cb_wired_network_ctrl_cmd == b_cmd)
		{
			return rs485_ctrl_cmd_receive_ex_call(dw_device_address,
                                                     w_data_addr,
                                                     pw_data_out_len,
                                                     pb_data_out,
                                                     w_data_out_buffer_size,
                                                     pb_receive_state);
		}
		else if(cb_wired_network_inquire_device_tree_cmd == b_cmd)
		{
			return rs485_inquire_device_tree_cmd_receive_ex_call(dw_device_address,
                                                                 w_data_addr,
                                                                 pw_data_out_len,
                                                                 pb_data_out,
                                                                 w_data_out_buffer_size,
                                                                 pb_receive_state);
		}
        else if(cb_wired_network_event_logs_cmd == b_cmd)
        {
			return rs485_inquire_event_logs_cmd_receive_ex_call(dw_device_address,
                                                                w_data_addr,
                                                                pw_data_out_len,
                                                                pb_data_out,
                                                                w_data_out_buffer_size,
                                                                pb_receive_state);
        }
        else if(cb_wired_network_product_test_cmd == b_cmd)
        {
			return rs485_product_test_cmd_receive_ex_call(dw_device_address,
                                                          w_data_addr,
                                                          pw_data_out_len,
                                                          pb_data_out,
                                                          w_data_out_buffer_size,
                                                          pb_receive_state);
        }
        else if(cb_wired_network_product_test_inquire_cmd == b_cmd)
        {
			return rs485_product_test_inquire_cmd_receive_ex_call(dw_device_address,
                                                                  w_data_addr,
                                                                  pw_data_out_len,
                                                                  pb_data_out,
                                                                  w_data_out_buffer_size,
                                                                  pb_receive_state);
        }
        else if(cb_wired_network_subset_update_pac_cmd == b_cmd)
        {
			return rs485_subset_update_pac_cmd_receive_ex_call(dw_device_address,
                                                               w_data_addr,
                                                               pw_data_out_len,
                                                               pb_data_out,
                                                               w_data_out_buffer_size,
                                                               pb_receive_state);
        }
        else if(cb_wired_network_subset_update_pacx_mcu_cmd == b_cmd)//mark rs485 subset update receive
        {
        }
        else if(cb_wired_network_subset_update_pacx_dsp_cmd == b_cmd)//mark rs485 subset update receive
        {
			return rs485_dsp_update_cmd_receive_ex_call(dw_device_address,
                                                        w_data_addr,
                                                        pw_data_out_len,
                                                        pb_data_out,
                                                        w_data_out_buffer_size,
                                                        pb_receive_state);
        }
        else if(cb_wired_network_subset_update_transparent_cmd == b_cmd)
        {
            return rs485_transparent_cmd_receive_ex_call(dw_device_address,
                                                         w_data_addr,
                                                         pw_data_out_len,
                                                         pb_data_out,
                                                         w_data_out_buffer_size,
                                                         pb_receive_state);
        }
        else if(cb_wired_network_inv_auxiliary_installation_cmd == b_cmd)
        {
            return rs485_auxiliary_installation_cmd_receive_ex_call(dw_device_address,
                                                                     w_data_addr,
                                                                     pw_data_out_len,
                                                                     pb_data_out,
                                                                     w_data_out_buffer_size,
                                                                     pb_receive_state);
        }
        else if(cb_wired_network_mppt_auxiliary_installation_cmd == b_cmd)
        {
            return rs485_mppt_auxiliary_installation_cmd_receive_ex_call(dw_device_address,
                                                                         w_data_addr,
                                                                         pw_data_out_len,
                                                                         pb_data_out,
                                                                         w_data_out_buffer_size,
                                                                         pb_receive_state);
        }
		else
		{
			return FALSE;
		}
	}

	return FALSE;
}

uint8_t wired_network_data_transmission_channel_get(pfunc_net_device_send_data *p_send_data_channel,
													pfunc_net_device_receive_data *p_receive_data_channel)
{
	if((NULL == p_send_data_channel) || (NULL == p_receive_data_channel))
	{
		return FALSE;
	}

	*p_send_data_channel = wired_network_send_data;
	*p_receive_data_channel = wired_network_receive_data;

	return TRUE;
}
