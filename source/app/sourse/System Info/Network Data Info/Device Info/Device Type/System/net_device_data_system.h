#ifndef _NET_DEVICE_DTA_SYSTEM_H_
#define _NET_DEVICE_DTA_SYSTEM_H_

#include <stdint.h>
#include "net_device_data.h"

typedef struct
{
    uint16_t w_0100;
    uint16_t w_0101;
    uint32_t dw_0102;
    uint16_t w_0103;
    uint8_t b_0104;
    uint8_t b_0105;
    uint8_t b_0106;
    uint8_t ba_0107[30];
    uint8_t b_0109;
    uint8_t b_010A;
}net_status_data_system;

typedef struct
{
	uint8_t ba_0000[30];
    uint8_t ba_4F00[30];
    uint32_t dw_0002;
    uint8_t ba_000B[129];
    uint8_t ba_000C[4];
    uint16_t w_000D;
    uint8_t ba_000F[30];
    uint8_t ba_0010[30];
    uint8_t b_AABB;
    uint8_t b_00FF;
}net_set_data_system;

extern const net_data_list_info st_net_data_list_info_system;

#endif
