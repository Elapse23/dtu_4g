#ifndef _NET_DEVICE_STATUS_H_
#define _NET_DEVICE_STATUS_H_

#include <stdint.h>

#define cb_net_device_none						0x00
#define cb_net_device_rs485						0x01
#define cb_net_device_can						0x02

#define cb_net_device_delete     				0x00   // 还未判断的默认状态
#define cb_net_device_add    		    		0x01   // 设备存在
#define cb_net_device_loss    					0x02   // 设备失联

typedef struct
{
	uint8_t b_communication_protocal;
    uint8_t b_communicate_status;
	uint8_t b_communicate_timeout_times;
    const uint8_t b_communicate_max_timeout_times;
	uint16_t w_firmware_version_inquire_addr;
    uint8_t ba_device_tree_inquire_identification[2];
	uint16_t w_sn_inquire_addr;
	uint8_t b_virtual_device;
}net_device_status;

#endif
