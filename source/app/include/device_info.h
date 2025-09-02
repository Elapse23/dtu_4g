#ifndef _DEVICE_INFO_H_
#define _DEVICE_INFO_H_

#include <stdint.h>
#define cdw_device_id_system            0xFFFF00

#define cb_device_info_max_num                          10
#define cb_device_info_file_name_max_len                4

#define cb_firmware_version_max_len                     30
#define cb_deivce_id_max_len                            3
#define cb_deivce_serial_number_max_len                 30
#define cb_device_info_max_len                          (cb_firmware_version_max_len + cb_deivce_id_max_len + cb_deivce_serial_number_max_len + 1)


void master_device_info_init();
uint8_t device_tree_build(uint16_t *pw_data_out_len,uint8_t *pb_data_out,uint16_t w_data_out_buffer_max_len);

#endif
