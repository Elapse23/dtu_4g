#include "iot_module_config_general.h"
#include "iot_module_config_common.h"
#include "lte_module.h"
#include "lte_4g_module.h"

//#include "slave_ble_module_operate.h"


static uint8_t b_iot_module_type = iot_module_type_lte;
uint8_t b_communication_type = 0;
void iot_module_detect()	//TODO 后面若通过硬件检测4g或者wifi在这里加个if判断
{
	
	if(TRUE == b_communication_type)
	{
		b_iot_module_type = iot_module_type_4g;
	}
	else
	{
		b_iot_module_type = iot_module_type_lte;
	}
}

uint8_t iot_module_get_type(uint8_t *pb_iot_module_type)
{
	if(NULL == pb_iot_module_type)
	{
		return FALSE;
	}

	*pb_iot_module_type = b_iot_module_type;

	return TRUE;
}

uint8_t iot_module_config_general()
{
	uint8_t b_iot_module_type = iot_module_type_ble;

	if(TRUE == iot_module_get_type(&b_iot_module_type))
	{
		if(iot_module_type_lte == b_iot_module_type)
		{
			return TRUE;
		}
		if(iot_module_type_4g == b_iot_module_type)
		{
			return TRUE;
		}
//		else if(iot_module_type_ble == b_iot_module_type)
//		{
//			return slave_ble_module_driver_general();
//		}
	}

	return FALSE;
}

uint8_t iot_module_response_analysis_general()
{
	uint8_t b_iot_module_type = iot_module_type_ble;

	if(TRUE == iot_module_get_type(&b_iot_module_type))
	{
		if(iot_module_type_lte == b_iot_module_type)
		{
			return TRUE;
		}
		else if(iot_module_type_ble == b_iot_module_type)
		{
			return TRUE;
		}
		else if(iot_module_type_4g == b_iot_module_type)
		{
			return TRUE;
		}
	}

	return FALSE;
}

//uint8_t iot_module_send_data_background()
//{
//	uint8_t b_iot_module_type = iot_module_type_ble;
//
//	if(TRUE == iot_module_get_type(&b_iot_module_type))
//	{
//		if(iot_module_type_lte == b_iot_module_type)
//		{
//			return lte_module_send_data_background();
//		}
////		else if(iot_module_type_ble == b_iot_module_type)
////		{
////			return slave_ble_module_send_data_background();
////		}
//	}
//
//	return FALSE;
//}

uint8_t iot_module_receive_data_background()
{
	uint8_t b_iot_module_type = iot_module_type_ble;

	if(TRUE == iot_module_get_type(&b_iot_module_type))
	{
		if(iot_module_type_lte == b_iot_module_type)
		{
			return lte_module_receive_data_background();
		}
		else if(iot_module_type_4g == b_iot_module_type)
		{
			return lte_module_receive_data_background();
		}
//		else if(iot_module_type_ble == b_iot_module_type)
//		{
//			return TRUE;
//		}

	}

	return FALSE;
}

uint8_t iot_module_data_transmission_channel_get(px_iot_module_send_data *p_send_data_channel,
													  px_iot_module_receive_data *p_receive_data_channel)
{
	if((NULL == p_send_data_channel) || (NULL == p_receive_data_channel))
	{
		return FALSE;
	}

	uint8_t b_iot_module_type = iot_module_type_ble;

	if(TRUE == iot_module_get_type(&b_iot_module_type))
	{
		if(iot_module_type_lte == b_iot_module_type)		//通过Wifi或者4G选择发送接收函数
		{
			*p_send_data_channel = lte_module_user_data_send;
			*p_receive_data_channel = lte_module_user_data_receive;
		}
		else if(iot_module_type_ble == b_iot_module_type)
		{
//			*p_send_data_channel = slave_ble_module_send_data_foreground;
//			*p_receive_data_channel = slave_ble_module_receive_data_foreground;
		}
		else if(iot_module_type_4g == b_iot_module_type)		//通过Wifi或者4G选择发送接收函数
		{
			*p_send_data_channel = lte_4g_module_user_data_send;
			*p_receive_data_channel = lte_4g_module_user_data_receive;
		}
	}

	return TRUE;
}

uint8_t iot_module_transfer_packet_max_size_get(uint16_t *pw_packet_size)
{
	if(NULL == pw_packet_size)
	{
		return FALSE;
	}

	uint8_t b_iot_module_type = iot_module_type_ble;

	if(TRUE == iot_module_get_type(&b_iot_module_type))
	{
		if(iot_module_type_lte == b_iot_module_type)
		{
			*pw_packet_size = lte_module_mps_size;
		}
		else if(iot_module_type_4g == b_iot_module_type)
		{
			*pw_packet_size = lte_module_mps_size;
		}
		else if(iot_module_type_ble == b_iot_module_type)
		{
//			*pw_packet_size = slave_ble_module_mps_size;
		}
	}

	return TRUE;
}
