#include "extern_flash_operate.h"
#include "spi_flash_operate.h"
#include "application_general_macro.h"
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "printf_custom.h"

#define cw_extern_flash_read_temp_max_length    1000
static uint8_t ba_extern_flash_read_temp[cw_extern_flash_read_temp_max_length] = {0,};

uint8_t extern_flash_read(uint32_t dw_addr,uint32_t dw_data_len,uint8_t *pb_data)
{
    if(1 == spi_flash_read(dw_addr,dw_data_len,pb_data))
    {
        return FALSE;
    }

    return TRUE;
}
uint8_t extern_flash_wirte(uint32_t dw_addr,uint16_t w_data_len, uint8_t *pb_data)
{
    {
        uint16_t w_data_len_write = 0;
        uint16_t w_data_len_temp = w_data_len;
        uint16_t w_data_index = 0;
        do
        {
            {
                uint16_t w_data_len_write_max = 0;

                w_data_len_write_max = 256 - (dw_addr % 256);

                if(w_data_len_temp >= 256)
                {
                    w_data_len_write = w_data_len_write_max;
                }
                else
                {
                    if(w_data_len_temp < w_data_len_write_max)
                    {
                        w_data_len_write = w_data_len_temp;
                    }
                    else
                    {
                        w_data_len_write = w_data_len_write_max;
                    }
                }
            }

            {
                if(0 == (dw_addr % 0x1000))
                {
                    #if(EXTERN_FLASH_TEST)
                    DebugPrintf(TRUE,"extern_flash_wirte,spi_flash_erase_start,time: %d\n",xTaskGetTickCount());
                    #endif
                    if(1 == spi_flash_erase(dw_addr,0))
                    {
                        return FALSE;
                    }
                    #if(EXTERN_FLASH_TEST)
                    DebugPrintf(TRUE,"extern_flash_wirte,spi_flash_erase_end,time: %d\n",xTaskGetTickCount());
                    #endif
                }
                #if(EXTERN_FLASH_TEST)
				DebugPrintf(TRUE,"extern_flash_wirte,spi_flash_write_start,time: %d\n",xTaskGetTickCount());
                #endif
                if(1 == spi_flash_write(dw_addr,
                                        w_data_len_write,
                                        &pb_data[w_data_index]))
                {
                    return FALSE;
                }
                #if(EXTERN_FLASH_TEST)
                DebugPrintf(TRUE,"extern_flash_wirte,spi_flash_write_end,time: %d\n",xTaskGetTickCount());
                #endif
            }

            {
                uint8_t b_store_ok = 1;
                uint8_t b_cmp_times = 0;

                for(;;)
                {
                    if(w_data_len_write > cw_extern_flash_read_temp_max_length)
                    {
                        return FALSE;
                    }
                    #if(EXTERN_FLASH_TEST)
                    DebugPrintf(TRUE,"extern_flash_wirte,spi_flash_read_start,time: %d\n",xTaskGetTickCount());
                    #endif
                    if(1 == spi_flash_read(dw_addr,
                                           w_data_len_write,
                                           ba_extern_flash_read_temp))
                    {
                        return FALSE;
                    }
                    #if(EXTERN_FLASH_TEST)
                    DebugPrintf(TRUE,"extern_flash_wirte,spi_flash_read_end,time: %d\n",xTaskGetTickCount());
                    #endif

                    b_store_ok = memcmp(&pb_data[w_data_index],\
                                        ba_extern_flash_read_temp,\
                                        w_data_len_write);

                    if(0 == b_store_ok)
                    {
                        dw_addr += w_data_len_write;
                        w_data_len_temp -= w_data_len_write;
                        w_data_index += w_data_len_write;
                        break;
                    }
                    else
                    {
                        b_cmp_times++;
                        if(b_cmp_times > 10)
                        {
                            return FALSE;
                        }
                    }
                }
            }
        }
        while(w_data_len_temp != 0);
    }

    return TRUE;
}
