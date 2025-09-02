#ifndef _NET_DEVICE_DATA_ACCESS_H_
#define _NET_DEVICE_DATA_ACCESS_H_

#include <stdint.h>

#define cb_device_data_access_operate_type_read               0
#define cb_device_data_access_operate_type_write              1

#define cb_device_data_access_data_type_status                0
#define cb_device_data_access_data_type_set                   1
#define cb_device_data_access_data_type_ctrl                  2


#define cw_system_no_update                                   0
#define cw_system_wait_updating                               1
#define cw_system_updating                                    2
#define cw_system_receiving_firmware                          3

//status
#define cw_sys_status_data_addr_update_status                 0x0101
#define cw_sys_status_data_addr_update_device                 0x0102
#define cw_sys_status_data_addr_update_process                0x0103
#define cw_sys_status_data_addr_longitude                     0x0110
#define cw_sys_status_data_addr_latitude                      0x0111
#define cw_sys_status_data_addr_day_mileage                   0x0115
#define cw_sys_status_data_addr_total_mileage                 0x0116
#define cw_sys_status_data_addr_vehicle_speed                 0x0117
#define cw_sys_status_data_addr_altitude                      0x0118
#define cw_sys_status_data_addr_vehicle_overspeed             0x0119
#define cw_sys_status_data_addr_vehicle_driving               0x011A
#define cw_sys_status_data_addr_ble_mac                       0x011B

//set
#define cw_sys_set_data_addr_login_aconut                     0x001D
#define cw_sys_set_data_addr_vehicle_overspeed                0x0020

void net_device_data_access_initial();

void net_device_data_access(uint32_t dw_device_id,
                            uint8_t b_data_type,
                            uint16_t w_operate_addr,
                            uint16_t *pw_data_len,
                            uint8_t *pb_data,
                            uint8_t b_access_type);
void net_system_data_access(uint8_t b_data_type,
                            uint16_t w_operate_addr,
                            uint16_t *pw_data_len,
                            uint8_t *pb_data,
                            uint8_t b_access_type);

void net_device_data_access_simple(uint32_t  dw_device_id,
                            uint8_t   b_data_type,
                            uint16_t  w_operate_addr,
                            uint16_t *pw_data_len,
                            uint8_t  *pb_data,
                            uint8_t   b_access_type);

//void net_device_active_data_refresh(uint32_t dw_device_id,
//                                    uint8_t b_data_type,
//                                    uint16_t w_operate_addr,
//                                    uint16_t w_data_in_len,
//                                    uint8_t *pb_data_in);

#endif
