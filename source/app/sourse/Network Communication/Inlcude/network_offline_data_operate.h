#ifndef _NETWORK_OFFLINE_DATA_OPERATE_
#define _NETWORK_OFFLINE_DATA_OPERATE_

#include <stdint.h>


uint8_t check_have_offline_data();
uint8_t offline_data_flash_read(uint8_t *pb_data_out,uint16_t *pw_data_len);
void offline_data_flash_read_shift();
void enable_save_offline_data(uint8_t f_enable);
void offline_data_save();

#endif
