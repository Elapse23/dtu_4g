#include "net_forward_set_ctrl.h"
#include "net_system_info_access.h"
#include "portmacro.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "freertos_user_config.h"
#include "printf_custom.h"


uint8_t net_forward_set_ctrl(uint32_t dw_device_id,
                             uint8_t b_cmd,
                             uint16_t w_data_addr,
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
    #if(can_networking_test)
    DebugPrintf(TRUE,"net_inquire_device_tree,wait,time: %d\n",xTaskGetTickCount());
    #endif

	if(FALSE == net_device_send_data(pst_net_device_info->b_wired_communication_cable,
						             b_cmd,
									 pst_net_device_info->dw_device_address_send,
									 w_data_addr,
								     w_data_in_len,
							     	 pb_data_in,
						             pst_net_device_info))
	{
		return FALSE;
	}

    {
		uint16_t w_data_len = 0;
		uint8_t b_data = 0;
		uint8_t b_receive_state = 0;

		for(;;)
		{
			if(TRUE == net_device_receive_data(pst_net_device_info->b_wired_communication_cable,
								   			   b_cmd,
								   			   pst_net_device_info->dw_device_address_receive,
								   			   w_data_addr,
								   			   &w_data_len,
								   			   &b_data,
								   			   1,
								   			   &b_receive_state,
								   			   pst_net_device_info))
			{
				DebugPrintf(TRUE,"net_forward_set_ctrl,complete,time: %d\n",xTaskGetTickCount());

				if(cb_wired_network_receive_state_success == b_receive_state)
				{
					return TRUE;
				}
				else if((cb_wired_network_receive_state_fail == b_receive_state))
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

    #if(can_networking_test)
    DebugPrintf(TRUE,"net_forward_set_ctrl,timeout,time: %d\n",xTaskGetTickCount());
    #endif
    return FALSE;
}
