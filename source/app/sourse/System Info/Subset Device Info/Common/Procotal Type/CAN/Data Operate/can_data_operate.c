#include "can_data_operate.h"

uint8_t can_cycle_data_fill(uint32_t dw_can_id,can_cycle_data_list_info *pst_data_list,uint8_t *pb_data_in)
{
    if((NULL == pst_data_list) || (NULL == pb_data_in))
    {
        return FALSE;
    }

    for(uint16_t w_cnt_i = 0;w_cnt_i < pst_data_list->w_length;w_cnt_i++)
    {
        if(dw_can_id == pst_data_list->pst_list->dw_id)
        {
            for(w_cnt_i = 0;w_cnt_i < cb_can_cycle_data_length;w_cnt_i++)
            {
                pst_data_list->pst_list->pb_data[w_cnt_i] = pb_data_in[w_cnt_i];
            }

            return TRUE;
        }
    }

    return FALSE;
}
