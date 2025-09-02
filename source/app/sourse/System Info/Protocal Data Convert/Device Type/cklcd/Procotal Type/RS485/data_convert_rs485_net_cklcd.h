#ifndef _DATA_CONVERT_RS485_NET_STATUS_CKLCD_H_
#define _DATA_CONVERT_RS485_NET_STATUS_CKLCD_H_

#include <stdint.h>
#include "net_system_info.h"
#include "application_general_func.h"

uint8_t data_convert_rs485_net_status(uint32_t dw_device_id,uint8_t b_485_cmd,uint8_t *pb_485_data_in);
void device_data_polling_common(net_device_info * const pst_net_device_info);
uint8_t rs485_check_device_exist(net_system_info_list *pst_net_system_info_list,net_device_info * const pst_net_device_info_in);

#endif
