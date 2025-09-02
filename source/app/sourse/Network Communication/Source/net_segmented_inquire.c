#include "net_segmented_inquire.h"
#include "network_protocal_cmd_macro.h"
#include "system_info_general_macro.h"
#include "net_system_info_access.h"
#include "printf_custom.h"
#include "application_general_func.h"
#include "network_protocal.h"
#include "network_protocal_data_assembly.h"
#include "portmacro.h"
#include "projdefs.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include <ysizet.h>
#include <string.h>
#include "freertos_user_config.h"
//#include "rs485_communication_transmission.h"

extern void * pvPortMalloc( size_t xWantedSize );
extern void vPortFree( void * pv );

uint8_t net_device_data_inquire_segmented(uint16_t w_net_data_start_address,
                                          uint16_t w_net_data_end_address,
                                          uint16_t *pw_data_len_out,
                                          uint8_t *pb_data_out,
                                          uint16_t w_data_out_buffer_size,
                                          const net_data_list *pst_net_data_list,
                                          net_device_info *pst_net_device_info)
{
    #define cb_get_continuous_net_data_number          0
	#define	cb_send_inquire			                   1
	#define	cb_receive_inquire		                   2
	#define	cb_inquire_complete				           3
	uint8_t b_step = cb_get_continuous_net_data_number;

    uint16_t w_data_start_index = 0;

	uint16_t w_inquire_data_len = 0;
	uint8_t b_send_data_cnt = 0;

    uint16_t w_data_index = 0;
    uint16_t w_data_len = 0;

	do{
		switch(b_step)
		{
            case cb_get_continuous_net_data_number:
            {
                uint16_t w_data_segment_number = 0;

                w_data_start_index = w_data_index;
                w_inquire_data_len = 0;

                for(;;)
                {
                    if((pst_net_data_list->pst_net_data_info[w_data_index].w_data_address_net < w_net_data_start_address) ||\
                       (pst_net_data_list->pst_net_data_info[w_data_index].w_data_address_net > w_net_data_end_address))
                    {
                        w_data_index++;
                        break;
                    }

                    if(pst_net_data_list->pst_net_data_info[w_data_index].w_data_address_net == w_net_data_end_address)
                    {
                        w_data_segment_number++;
                        w_inquire_data_len += pst_net_data_list->pst_net_data_info[w_data_index].pst_data_attribute->b_len;
                        w_data_index++;
                        break;
                    }

                    if(pst_net_data_list->pst_net_data_info[w_data_index].st_net_data_inquire_config.w_inquire_cmd !=\
                       pst_net_data_list->pst_net_data_info[w_data_index + 1].st_net_data_inquire_config.w_inquire_cmd)
                    {
                        w_data_segment_number++;
                        w_inquire_data_len += pst_net_data_list->pst_net_data_info[w_data_index].pst_data_attribute->b_len;
                        w_data_index++;
                        break;
                    }

                    if((pst_net_data_list->pst_net_data_info[w_data_index].w_data_address_device + 1) !=\
                       pst_net_data_list->pst_net_data_info[w_data_index + 1].w_data_address_device)
                    {
                        w_data_segment_number++;
                        w_inquire_data_len += pst_net_data_list->pst_net_data_info[w_data_index].pst_data_attribute->b_len;
                        w_data_index++;
                        break;
                    }

                    w_data_segment_number++;
                    w_inquire_data_len += pst_net_data_list->pst_net_data_info[w_data_index].pst_data_attribute->b_len;
                    w_data_index++;
                }

                if(w_data_segment_number > 0)
                {
                    b_step = cb_send_inquire;
                }
                else if(pst_net_data_list->pst_net_data_info[w_data_index].w_data_address_net > w_net_data_end_address)
                {
                    *pw_data_len_out = w_data_len;
                    b_step = cb_inquire_complete;
                }
            }break;
			case cb_send_inquire:
			{
				b_send_data_cnt++;
                uint8_t ba_data[4] = {0,};
                ba_data[0] =\
                    (uint8_t)((pst_net_data_list->pst_net_data_info[w_data_start_index].w_data_address_device & 0xFF00) >> 8);
                ba_data[1] =\
                    (uint8_t)(pst_net_data_list->pst_net_data_info[w_data_start_index].w_data_address_device);
                ba_data[2] =\
                    (uint8_t)(((w_inquire_data_len/2) & 0xFF00) >> 8);
                ba_data[3] =\
                    (uint8_t)(w_inquire_data_len/2);
				if(b_send_data_cnt > 3)
				{
					b_send_data_cnt = 0;

					b_step = cb_get_continuous_net_data_number;
				}
				else if(TRUE == net_device_send_data(pst_net_device_info->b_wired_communication_cable,
												     cb_wired_network_inquire_cmd,
												     pst_net_device_info->dw_device_address_send,
												     (pst_net_data_list->pst_net_data_info[w_data_start_index].pst_device_data_attribute->w_device_inquire_cmd),
												     4,
												     ba_data,
												     pst_net_device_info))
				{
                    b_send_data_cnt = 0;
                    #if(can_networking_test)
                    DebugPrintf(TRUE,"net_inquire_device_tree,wait,time: %d\n",xTaskGetTickCount());
                    #endif
					b_step = cb_receive_inquire;
				}
				else
				{
                    *pw_data_len_out = 0;

					return FALSE;
				}
			}break;
			case cb_receive_inquire:
			{
                b_step = cb_send_inquire;

				{
					uint16_t w_receive_data_len = 0;
					uint8_t b_receive_state = 0;

					for(;;)
					{
						if(TRUE == net_device_receive_data(pst_net_device_info->b_wired_communication_cable,
														   pst_net_data_list->pst_net_data_info[w_data_start_index].st_net_data_inquire_config.w_inquire_cmd,
														   pst_net_device_info->dw_device_address_receive,
														   (pst_net_data_list->pst_net_data_info[w_data_start_index].pst_device_data_attribute->w_device_inquire_cmd),
														   &w_receive_data_len,
														   &pb_data_out[w_data_len],
														   (w_data_out_buffer_size - w_data_len),
														   &b_receive_state,
														   pst_net_device_info))
						{
							if(cb_wired_network_receive_state_success == b_receive_state)
							{
								b_send_data_cnt = 0;
								w_data_len += w_receive_data_len;
								b_step = cb_get_continuous_net_data_number;
								#if(can_networking_test)
								DebugPrintf(TRUE,"net_set_data_segmented_inquire_process,complete,time: %d\n",xTaskGetTickCount());
								#endif
								break;
							}
							else if(cb_wired_network_receive_state_fail == b_receive_state)
							{
                                return FALSE;
							}

							{
								vTaskDelay(0);
							}
						}
						else
						{
							break;
						}
					}
				}

			}break;
		}
	}while(b_step != cb_inquire_complete);

	return TRUE;
}

uint8_t net_device_inquire_data_build(uint16_t w_net_data_start_address,
									  uint16_t w_net_data_end_address,
									  uint16_t *pw_data_len_out,
									  uint8_t *pb_data_out,
									  uint16_t w_data_out_buffer_size,
									  uint16_t w_data_list_type,
									  net_device_info * const pst_net_device_info)
{
    if((NULL == pb_data_out) || (NULL == pst_net_device_info))
    {
        return FALSE;
    }

    uint16_t w_data_len = 0;
    uint16_t w_data_index = 0;
    uint16_t w_segmented_data_number = 0;
    uint16_t w_net_segmented_data_start_address = 0;
    uint16_t w_net_segmented_data_end_address = 0;

    #define cb_get_data_start_index                     0
    #define cb_get_continuous_device_data_number        1
    #define cb_fill_inquire_ack_header                  2
    #define cb_segmented_inquire                        3
    #define cb_inquire_specific_complete                4
    uint8_t b_step = cb_get_data_start_index;

    const net_data_list *pst_net_data_list = 0;

    if(FALSE == net_get_device_data_list(w_data_list_type,
                                         (const net_data_list **)(&pst_net_data_list),
                                         pst_net_device_info))
    {
        return FALSE;
    }

    do
    {
        switch(b_step)
        {
            case cb_get_data_start_index:
            {
                for(uint16_t w_cnt_i = 0;w_cnt_i < pst_net_data_list->w_net_data_number;w_cnt_i++)
                {
                    if(w_net_data_start_address == pst_net_data_list->
                                                   pst_net_data_info[w_cnt_i].
                                                   w_data_address_net)
                    {
                        w_data_index = w_cnt_i;
                        break;
                    }
                    else if((w_cnt_i + 1) == pst_net_data_list->w_net_data_number)
                    {
                        *pw_data_len_out = 0;

                        return FALSE;
                    }
                }

                b_step = cb_get_continuous_device_data_number;
                break;
            }break;
            case cb_get_continuous_device_data_number:
            {
				//data address may not be continuous
                //uint8_t b_get_start_index = FALSE;

                w_segmented_data_number = 0;
                w_net_segmented_data_start_address = pst_net_data_list->pst_net_data_info[w_data_index].w_data_address_net;

                for(;;)
                {
                    if(w_data_index >= pst_net_data_list->w_net_data_number)
                    {
                        break;
                    }

                    if((pst_net_data_list->
                        pst_net_data_info[w_data_index].
                        w_data_address_net < w_net_data_start_address) ||\
                        pst_net_data_list->
                        pst_net_data_info[w_data_index].
                        w_data_address_net > w_net_data_end_address)
                    {
                        break;
                    }

                    if((w_data_index + 1) == pst_net_data_list->w_net_data_number)
                    {
                        w_segmented_data_number++;
                        w_data_index++;
                        break;
                    }

                    if((pst_net_data_list->
                        pst_net_data_info[w_data_index].
                        w_data_address_net + 1) !=\
                       pst_net_data_list->
                       pst_net_data_info[w_data_index + 1].
                       w_data_address_net)
                    {

                        w_segmented_data_number++;
                        w_data_index++;
                        break;
                    }

                    {
                        w_segmented_data_number++;
                        w_data_index++;
                    }
                }

                if(w_segmented_data_number > 0)
                {
					w_net_segmented_data_end_address = pst_net_data_list->pst_net_data_info[w_data_index - 1].w_data_address_net;
                    b_step = cb_fill_inquire_ack_header;
                }
                else if(w_net_segmented_data_end_address >= w_net_data_end_address)
                {
                    *pw_data_len_out = w_data_len;
                    b_step = cb_inquire_specific_complete;
                }
            }break;
            case cb_fill_inquire_ack_header:
            {
                uint8_t b_result = TRUE;

                b_result &= int_to_bytes_big_endain(&pb_data_out[w_data_len],*pst_net_device_info->pdw_device_id,3);
                w_data_len += 3;

                b_result &=  int_to_bytes_big_endain(&pb_data_out[w_data_len],w_net_data_start_address,2);
                w_data_len += 2;

                b_result &= int_to_bytes_big_endain(&pb_data_out[w_data_len],w_segmented_data_number,2);
                w_data_len += 2;

                if(FALSE == b_result)
                {
                    *pw_data_len_out = 0;

                    return FALSE;
                }

                b_step = cb_segmented_inquire;
            }break;
            case cb_segmented_inquire:
            {
                uint16_t w_inquire_data_len = 0;

                if(FALSE == net_device_data_inquire_segmented(w_net_segmented_data_start_address,
                                                              w_net_segmented_data_end_address,
                                                              &w_inquire_data_len,
                                                              &pb_data_out[w_data_len],
                                                              (w_data_out_buffer_size - w_data_len),
                                                              pst_net_data_list,
                                                              pst_net_device_info))
                {
                    *pw_data_len_out = 0;

                    return FALSE;
                }

                w_data_len += w_inquire_data_len;

                b_step = cb_get_continuous_device_data_number;
            }break;
            default:
            {
            }break;
        }
    }while(b_step != cb_inquire_specific_complete);

    return TRUE;
}
