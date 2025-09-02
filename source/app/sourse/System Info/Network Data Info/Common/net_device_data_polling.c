#include "net_device_data_polling.h"
#include "application_general_macro.h"
#include "net_system_info_access.h"
#include "network_system_type_operate.h"


void net_device_data_polling(net_system_info_list *pst_net_system_info_list)
{
	if(NULL == pst_net_system_info_list)
	{
		return;
	}

	#define cb_data_polling_get_system_type			0
	#define cb_data_polling							1

	static uint8_t b_step = cb_data_polling_get_system_type;
	static uint16_t w_device_index = 0;
	static net_system_info *pst_net_system_info = 0;
	net_device_info *pst_device_info = 0;
	switch(b_step)
	{
		case cb_data_polling_get_system_type:
		{
			if(TRUE == net_get_system_info_current((net_system_info **)(&pst_net_system_info),pst_net_system_info_list))
			{
				b_step = cb_data_polling;
			}
		}break;
		case cb_data_polling:
		{
            for(;;)
            {
                if(w_device_index >= pst_net_system_info->pst_device_info_list->w_length)
                {
                    system_init_comfirm(cb_system_init_type_device_com);
                    w_device_index = 0;
                }
                pst_device_info = &pst_net_system_info->pst_device_info_list->pst_device_info[w_device_index++];

                if((pst_device_info != 0) && (pst_device_info->pst_net_device_func.net_device_data_polling != 0))
                {
                    pst_device_info->pst_net_device_func.net_device_data_polling((net_device_info * )pst_device_info);      //device_data_polling_common
                    break;
                }
            }
            if((pst_device_info != 0) && (pst_device_info->pst_net_device_func.net_device_event_inquire != 0))
            {
                pst_device_info->pst_net_device_func.net_device_event_inquire((net_device_info * )pst_device_info);		//network_event_log_polling_func
            }
		}break;
		default:
		{
		}break;
	}
}
