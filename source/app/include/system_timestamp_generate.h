#ifndef _TIMESTAMP_GENERATE_H_
#define _TIMESTAMP_GENERATE_H_

#include <stdint.h>

typedef struct
{
    uint16_t tm_year;
    uint16_t tm_mon;
    uint16_t tm_mday;
    uint16_t tm_hour;
    uint16_t tm_min;
    uint16_t tm_sec;
}utc_time;

uint8_t refresh_system_timestamp(uint32_t dw_timestamp);
uint8_t get_system_timestamp(uint32_t *pdw_timestamp);
void refresh_timestamp();
uint8_t utc_struct_time_to_unix_time(const utc_time* pst_uct_time,uint32_t* unix_time);
uint8_t set_device_timestamp(uint32_t dw_timestamp);
void system_timestamp_task_init();

#endif
