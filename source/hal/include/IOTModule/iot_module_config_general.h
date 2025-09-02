#ifndef _IOT_MODULE_CONFIG_GENERAL_H_
#define _IOT_MODULE_CONFIG_GENERAL_H_

#include <stdint.h>
#include "application_general_macro.h"
#include "lte_module_config_common.h"

#define iot_module_data_info 		communication_data_info


typedef uint8_t (*px_iot_module_send_data)(void *pst_network_data);
typedef uint8_t (*px_iot_module_receive_data)(void *pst_network_data);


uint8_t iot_module_config_general();
uint8_t iot_module_response_analysis_general();
uint8_t iot_module_send_data_background();
uint8_t iot_module_receive_data_background();
uint8_t iot_module_data_transmission_channel_get(px_iot_module_send_data *p_send_data_channel,
											     px_iot_module_receive_data *p_receive_data_channel);
uint8_t iot_module_transfer_packet_max_size_get(uint16_t *pw_packet_size);

void iot_module_detect();
uint8_t iot_module_get_type(uint8_t *pb_iot_module_type);


//#define cw_iot_module_reconnect            0x00

#endif
