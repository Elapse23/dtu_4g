#ifndef _CAN_CYCLE_DTA_H_
#define _CAN_CYCLE_DTA_H_

#define cb_can_cycle_data_length    8

typedef struct
{
    uint32_t dw_id;
    uint8_t *pb_data;
}can_cycle_data_list;

typedef struct
{
    uint16_t w_length;
    can_cycle_data_list *pst_list;
}can_cycle_data_list_info;

#endif
