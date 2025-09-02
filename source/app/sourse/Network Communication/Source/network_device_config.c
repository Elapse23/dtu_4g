#include "network_device_config.h"
#include "net_system_info_access.h"
#include "application_general_macro.h"
#include "net_device_data_access.h"
#include "net_device_tree.h"
#include "rs485_communication_transmission.h"
#include "portmacro.h"
#include "freertos_user_config.h"
#include "network_local_firmware_update.h"
#include "net_device_tree_cklcd.h"
#include "application_general_func.h"
#include "network_system_type_operate.h"
#include "semphr.h"


extern SemaphoreHandle_t  pst_rs485_data_polling_mutex;
		  
extern uint8_t g_ble_rx_flag;

uint8_t net_device_config(uint32_t dw_device_id,
                          uint8_t b_cmd,
                          const net_data_info *pst_net_data_info,
                          uint16_t w_data_in_len,
                          uint8_t *pb_data_in,
                          net_system_info_list *pst_net_system_info_list)
{
    net_device_info *pst_net_device_info = 0;
    if(FALSE == net_get_device_info(dw_device_id,
                                    (net_device_info **)&pst_net_device_info,
                                    pst_net_system_info_list))
    {
        return FALSE;
    }
	if(NULL != pst_rs485_data_polling_mutex)
	{
		xSemaphoreTake(pst_rs485_data_polling_mutex, portMAX_DELAY);
		vTaskSuspend(device_data_polling_task_handle);
		xSemaphoreGive(pst_rs485_data_polling_mutex);
	}
    {
        if(pst_net_device_info->b_wired_communication_cable == cb_net_subset_com_rs485_cable)
        {
            if(NULL != pst_net_data_info->pst_device_data_attribute)
            {
				for(uint8_t i = 0; i < 2; i ++)
				{
					uint16_t w_set_data = 0;
					uint8_t b_receive_state = 0;
		            uint16_t w_receive_data_len = 0;
		            uint8_t ba_receive_data[20] = {0,};
					uint8_t b_cnt_j = 2;		//发送失败重发次数
		            uint8_t ba_send_data[20] = {0};
		            uint8_t b_data_len = 0;
					uint16_t w_send_addr = 0;
					if(i == 0)
					{
						w_set_data = pb_data_in[0] << 8 | pb_data_in[1];
						ba_send_data[b_data_len++] = 0x00;
						ba_send_data[b_data_len++] = 0x00;
						w_send_addr = 0x0000;
						if(w_set_data != 0)
						{
							ba_send_data[b_data_len++] = 0x00;
							ba_send_data[b_data_len++] = 0x01;
						}
						else if(w_set_data == 0)
						{
							ba_send_data[b_data_len++] = 0x00;
							ba_send_data[b_data_len++] = 0x00;
						}
					}
					else
					{
						ba_send_data[b_data_len++] = 0x00;
						ba_send_data[b_data_len++] = 0x01;
						w_send_addr = 0x0001;
						for(uint8_t b_cnt_i = 0;b_cnt_i < w_data_in_len;b_cnt_i++)
						{
							ba_send_data[b_data_len++] = pb_data_in[b_cnt_i];
						}
					}
					while(b_cnt_j)
					{
						if(TRUE == net_device_send_data(pst_net_device_info->b_wired_communication_cable,
														cb_net_subset_com_set_cmd,
														pst_net_device_info->dw_device_address_send,
														pst_net_data_info->pst_device_data_attribute->w_device_set_cmd,
														b_data_len,
														ba_send_data,
														pst_net_device_info))
						{
							for(;;)
							{
								if(TRUE == net_device_receive_data(pst_net_device_info->b_wired_communication_cable,
																   cb_net_subset_com_set_cmd,
																   pst_net_device_info->dw_device_address_receive,
																   pst_net_data_info->pst_device_data_attribute->w_device_set_cmd,
																   &w_receive_data_len,
																   ba_receive_data,
																   20,
																   &b_receive_state,
																   pst_net_device_info))
								{
									if(cb_wired_network_receive_state_success == b_receive_state)
									{
										{
											uint16_t w_data_len;
											uint32_t dw_data_temp = 0;
											if(check_system_type(cb_inv_type_riio_sun_ii_split_phase) || check_system_type(cb_inv_type_riio_ii_split_phase))
											{
												w_data_len = 2;
												if(TRUE == bytes_to_int_big_to_little_endain(&ba_send_data[2],
																							 &dw_data_temp,
																							 w_data_len))
												{							
													net_device_data_access(*pst_net_device_info->pdw_device_id,
																		   cb_device_data_access_data_type_set,
																		   w_send_addr,
																		   &w_data_len,
																		   (uint8_t *)&dw_data_temp,
																		   cb_device_data_access_operate_type_write);
												}
											}
											else
											{
												w_data_len = 2;
												if(TRUE == bytes_to_int_big_to_little_endain(&ba_send_data[2],
																							 &dw_data_temp,
																							 w_data_len))
												{							
													net_device_data_access(*pst_net_device_info->pdw_device_id,
																		   cb_device_data_access_data_type_set,
																		   w_send_addr,
																		   &w_data_len,
																		   (uint8_t *)&dw_data_temp,
																		   cb_device_data_access_operate_type_write);
												}
											}
										}
										if(i == 0)
										{
											b_cnt_j = 0;
											break;
										}
										i = 0;
										vTaskResume(device_data_polling_task_handle);
										return TRUE;
									}
									else if(cb_wired_network_receive_state_fail == b_receive_state)
									{
										b_cnt_j--;
										break;
									}
								}
								else
								{
									b_cnt_j--;
									break;
								}
							}
						}
						else
						{
							b_cnt_j--;
						}
					}
					if(g_ble_rx_flag == 1)
					{
						{
							uint16_t w_data_len;
							uint32_t dw_data_temp = 0;
							if(check_system_type(cb_inv_type_riio_sun_ii_split_phase) || check_system_type(cb_inv_type_riio_ii_split_phase))
							{
								w_data_len = 2;
								if(TRUE == bytes_to_int_big_to_little_endain(&ba_send_data[2],
																			 &dw_data_temp,
																			 w_data_len))
								{							
									net_device_data_access(*pst_net_device_info->pdw_device_id,
														   cb_device_data_access_data_type_set,
														   w_send_addr,
														   &w_data_len,
														   (uint8_t *)&dw_data_temp,
														   cb_device_data_access_operate_type_write);
								}
							}
							else
							{
								w_data_len = 2;
								if(TRUE == bytes_to_int_big_to_little_endain(&ba_send_data[2],
																			 &dw_data_temp,
																			 w_data_len))
								{							
									net_device_data_access(*pst_net_device_info->pdw_device_id,
														   cb_device_data_access_data_type_set,
														   w_send_addr,
														   &w_data_len,
														   (uint8_t *)&dw_data_temp,
														   cb_device_data_access_operate_type_write);
								}
							}
						}
					}
				}
			}
        }

	}
	if(g_ble_rx_flag == 1)
	{
		vTaskResume(device_data_polling_task_handle);
		return TRUE;

	}

	vTaskResume(device_data_polling_task_handle);
	return FALSE;	

}


