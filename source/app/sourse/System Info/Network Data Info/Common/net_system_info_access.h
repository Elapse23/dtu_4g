#ifndef _SYSTEM_INFO_ACCESS_H_
#define _SYSTEM_INFO_ACCESS_H_

#include <stdint.h>
#include "net_system_info.h"
#include "lte_module.h"

typedef lte_module_ip_port net_ip_port;
extern net_ip_port st_net_ip_port;

uint8_t net_get_system_info_current(net_system_info ** const pst_system_info,net_system_info_list * const pst_system_info_list);

uint8_t net_get_device_info(uint32_t dw_device_id,
						    net_device_info ** const pst_net_device_info,
						    net_system_info_list * const pst_system_info_list);

uint8_t net_get_device_data_list(uint8_t data_list_type,
								 const net_data_list ** const pst_device_data_list,
								 net_device_info * const pst_net_device_info);

uint8_t net_get_device_tree_info(uint32_t dw_device_id,
							     net_device_tree **pst_net_device_tree,
							     net_system_info_list * const pst_system_info_list);

uint8_t net_get_device_tree_relasion(uint32_t dw_device_id,
							         net_device_tree_element_info ** const pst_net_device_tree_element_info,
							         net_system_info_list * const pst_system_info_list);

uint8_t net_get_device_data_attribute(uint32_t dw_device_id,
							          uint8_t b_net_data_type,
							          uint16_t w_data_addr,
							          net_data_attribute ** const pst_net_data_attribute,
							          net_system_info_list * const pst_system_info_list);

uint8_t net_get_device_data_info(uint8_t b_net_data_type,
						         uint16_t w_data_addr,
						         const net_data_info ** const pst_net_data_info,
						         net_device_info * const pst_net_device_info);

#define cb_net_get_device_send_address          0
#define cb_net_get_device_receive_address       1
uint8_t net_device_id_convert_2_address(uint8_t b_device_address_type,
                                        uint32_t dw_device_id,
										uint32_t * const pdw_device_address,
										net_system_info_list * const pst_system_info_list);

uint8_t net_device_address_convert_2_id(uint32_t dw_device_address,
									    uint32_t * const pdw_device_id,
									    net_system_info_list * const pst_system_info_list);
uint8_t net_check_device_communicate_status(uint8_t b_communicate_status,net_device_info * const pst_net_device_info);
void net_device_status_data_update_from_subset(net_data_list * const pst_net_data_list,
                                               uint8_t b_data_size,
                                               void * const pv_status_data,
                                               uint32_t dw_subset_data);

#endif
