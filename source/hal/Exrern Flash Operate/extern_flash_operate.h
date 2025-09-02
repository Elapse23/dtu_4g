#ifndef _EXTERN_FLASH_OPERATE_H_
#define _EXTERN_FLASH_OPERATE_H_

#include <stdint.h>

uint8_t extern_flash_read(uint32_t dw_addr,uint32_t dw_data_len,uint8_t *pb_data);
uint8_t extern_flash_wirte(uint32_t dw_addr,uint16_t w_data_len,uint8_t *pb_data);

#endif
