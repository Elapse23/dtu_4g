#ifndef _SPI_FLASH_OPERATE_H_
#define _SPI_FLASH_OPERATE_H_

#include <stdint.h>

typedef enum
{
    spi_flash_read_request = 0,
    spi_flash_write_request,
    spi_flash_erase_request,
}spi_flash_operate_type;

typedef struct
{
    spi_flash_operate_type  enum_operate_type;
    uint32_t dw_operate_addr;
    struct
    {
        uint8_t *pb_data;
        uint16_t w_data_len;
    }st_data_info;
    uint32_t dw_spi_flash_operate_initiate_uuid;
}spi_flash_operate_initiate_msg;

//#pragma pack(push, 1)
typedef struct
{
    uint32_t dw_spi_flash_operate_response_uuid;
    uint8_t b_spi_flash_operate_status;
}spi_flash_operate_response_msg;


void spi_flash_operate_init();
int32_t spi_flash_read(uint32_t dw_addr, uint32_t dw_data_len, uint8_t *pb_data);
int32_t spi_flash_write(uint32_t dw_addr, uint32_t dw_data_len,uint8_t *pb_data);
int32_t spi_flash_erase(uint32_t dw_addr, uint32_t dw_data_len);

#endif
