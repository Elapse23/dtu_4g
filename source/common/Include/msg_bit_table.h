#ifndef _MSG_BIT_TABLE_H_
#define _MSG_BIT_TABLE_H_

#include <stdint.h>

typedef struct
{
    uint16_t w_cell_timer;
    void *pv_cell_data;
}cell_info;

typedef struct
{
    uint32_t dw_cell_bit;
    uint16_t w_cell_length;
    cell_info *pst_cell_info;
}net_data_transfer_info_cell;

#endif
