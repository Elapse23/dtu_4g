#include "network_local_device_secquence_operate.h"
#include "system_info_general_macro.h"
#include <string.h>
#include "Eeprom.h"
#include "net_device_tree_build.h"

uint8_t ba_local_device_secquence[30] = "ck654321";


uint8_t get_local_device_secquence(uint8_t *pb_data,uint16_t w_buffer_size)
{
    if(NULL == pb_data)
    {
        return FALSE;
    }

    uint8_t b_secquence_len = strlen((char *)&ba_local_device_secquence[0]);

    if(w_buffer_size < b_secquence_len)
    {
        return FALSE;
    }

    memcpy(pb_data,ba_local_device_secquence,b_secquence_len);

    return b_secquence_len;
}
uint8_t get_client_system_id(uint8_t *pb_data,uint16_t w_buffer_size)		//获取客户标志
{
    if(NULL == pb_data)
    {
        return FALSE;
    }
//tbb后台 0x00
//中性后台 0x01
	uint8_t ba_client_system_id[30] = "0000";
	memset(pb_data,0,sizeof(uint8_t) * w_buffer_size);
	if(gw_agent_facturer == 0)
	{
		 return 0;
	}
	sprintf(ba_client_system_id, "%04X", gw_agent_facturer);
	memcpy(pb_data,ba_client_system_id,4);
    return 0;
}

uint8_t set_local_device_secquence(uint8_t *pb_data_in,uint8_t b_data_len)
{
    if((b_data_len > 30) ||\
       (NULL == pb_data_in))
    {
        return FALSE;
    }
    memset(ba_local_device_secquence,0,30);
    memcpy(ba_local_device_secquence,pb_data_in,b_data_len);

    fWriteEEPage(0) = TRUE;
    fWriteEEPage(1) = TRUE;
    fWriteEEPage(2) = TRUE;

    return TRUE;

}
extern uint8_t b_communication_type;
uint8_t set_local_device_communication_type_4g()
{

	b_communication_type = TRUE;
	fWriteEEPage(16) = TRUE;

    return TRUE;

}

