//net_data_transfer_info      sta_net_data_transfer_info[cw_cell_number];
//struct base_ino
//{
//    uint32_t dw_device_id;
//    uint8_t b_protocal_cmd;
//    uint16_t w_data_address;
//}net_data_transfer_base_info;
//typedef struct
//{
//    net_data_transfer_base_info st_net_data_transfer_base_info;
//    uint32-t dw_secquence;
//    uint8_t b_result;
//}net_data_transfer_detailed_info;

//===============================================================================================================================
#include "msg_bit_table.h"



uint8_t msg_bit_table_set(uint8_t *pb_cell_data_in,uint16_t w_cell_deta_in_len,net_data_transfer_info_cell *pst_net_data_transfer_info_cell)
{
    if(NULL == pst_net_data_transfer_info_cell)
    {
        return FALSE;
    }

    uint16_t w_cell_index = 0;

    uint16_t w_cell_w_cell_length = (pst_net_data_transfer_info_cell->w_cell_length < w_cell_number)?pst_net_data_transfer_info_cell->w_cell_length:w_cell_number;
    uint16_t w_cell_bit_middle = 0x01 << (w_cell_w_cell_length /2);
    uint16_t w_cell_bit_end = 0x01 << w_cell_w_cell_length;
    for(uint16_t w_cnt_i = 0;w_cnt_i < w_cell_w_cell_length;w_cnt_i++)
    {
        if((pst_net_data_transfer_info_cell->dw_cell_bit & (0x0001 << w_cnt_i)) != 0)
        {
            w_cell_index = w_cnt_i;
            break;
        }

        if((pst_net_data_transfer_info_cell->dw_cell_bit & (w_cell_bit_middle >> w_cnt_i)) != 0)
        {
            w_cell_index = w_cell_bit_middle - w_cnt_i;
            break;
        }

        if((pst_net_data_transfer_info_cell->dw_cell_bit & (w_cell_bit_middle << w_cnt_i)) != 0)
        {
            w_cell_index = w_cell_bit_middle + w_cnt_i;
            break;
        }

        if((pst_net_data_transfer_info_cell->dw_cell_bit & (w_cell_bit_end >> w_cnt_i)) != 0)
        {
            w_cell_index = w_cell_bit_end - w_cnt_i;
            break;
        }
    }

    if(w_cnt_i >= w_cell_number)
    {
        return FALSE;
    }

    pst_net_data_transfer_info_cell->dw_cell_bit |= (1 << w_cell_index);
    for(uint16_t w_cnt_i = 0;w_cnt_i < w_cell_deta_in_len;w_cnt_i++)
    {
        ((uint8_t *)(pst_net_data_transfer_info_cell->pv_cell_data[w_cell_index]) + w_cnt_i) =\
            pb_cell_data_in[w_cnt_i];
    }

    return TRUE;
}

uint8_t msg_bit_table_reset(uint8_t *pb_cell_data_in,uint16_t w_cell_deta_in_len,net_data_transfer_info_cell *pst_net_data_transfer_info_cell)
{
    if((NULL == pb_cell_data_in) || (NULL == pst_net_data_transfer_info_cell))
    {
        return FALSE;
    }

    uint16_t w_cell_index = 0;

    uint16_t w_cell_w_cell_length = (pst_net_data_transfer_info_cell->w_cell_length < w_cell_number)?pst_net_data_transfer_info_cell->w_cell_length:w_cell_number;
    uint16_t w_cell_bit_middle = 0x01 << (w_cell_w_cell_length /2);
    uint16_t w_cell_bit_end = 0x01 << w_cell_w_cell_length;
    for(uint16_t w_cnt_i = 0;w_cnt_i < w_cell_w_cell_length;w_cnt_i++)
    {
        if((pst_net_data_transfer_info_cell->dw_cell_bit & (0x0001 << w_cnt_i)) != 0)
        {
            uint16_t w_cnt_j = 0;
            for(w_cnt_j = 0;w_cnt_j < w_cell_deta_in_len;w_cnt_j++)
            {
                if(((uint8_t *)(pst_net_data_transfer_info_cell->pv_cell_data[w_cnt_i]) + w_cnt_j) !=
                    pb_cell_data_in[w_cnt_j])
                {
                    break;
                }
            }

            if(w_cnt_j >= w_cell_deta_in_len)
            {
                break;
            }
        }

        if((pst_net_data_transfer_info_cell->dw_cell_bit & (w_cell_bit_middle >> w_cnt_i)) != 0)
        {
            uint16_t w_cnt_j = 0;
            for(w_cnt_j = 0;w_cnt_j < w_cell_deta_in_len;w_cnt_j++)
            {
                if(((uint8_t *)(pst_net_data_transfer_info_cell->pv_cell_data[w_cnt_i]) + w_cnt_j) !=
                    pb_cell_data_in[w_cnt_j])
                {
                    break;
                }
            }

            if(w_cnt_i >= w_cell_deta_in_len)
            {
                break;
            }
        }

        if((pst_net_data_transfer_info_cell->dw_cell_bit & (w_cell_bit_middle << w_cnt_i)) != 0)
        {
            uint16_t w_cnt_j = 0;
            for(w_cnt_j = 0;w_cnt_j < w_cell_deta_in_len;w_cnt_j++)
            {
                if(((uint8_t *)(pst_net_data_transfer_info_cell->pv_cell_data[w_cnt_i]) + w_cnt_j) !=
                    pb_cell_data_in[w_cnt_j])
                {
                    break;
                }
            }

            if(w_cnt_i >= w_cell_deta_in_len)
            {
                break;
            }
        }

        if((pst_net_data_transfer_info_cell->dw_cell_bit & (w_cell_bit_end >> w_cnt_i)) != 0)
        {
            uint16_t w_cnt_j = 0;
            for(w_cnt_j = 0;w_cnt_j < w_cell_deta_in_len;w_cnt_j++)
            {
                if(((uint8_t *)(pst_net_data_transfer_info_cell->pv_cell_data[w_cnt_i]) + w_cnt_j) !=
                    pb_cell_data_in[w_cnt_j])
                {
                    break;
                }
            }

            if(w_cnt_j >= w_cell_deta_in_len)
            {
                break;
            }
        }
    }

    if(w_cnt_i >= w_cell_number)
    {
        return FALSE;
    }

    pst_net_data_transfer_info_cell->dw_cell_bit &= ~(1 << w_cell_index);

    return TRUE;
}

uint8_t msg_bit_clean(net_data_transfer_info_cell *pst_net_data_transfer_info_cell)
{

}

////=======================================================================================================
//
//void net_transfer_callback_task()
//{
//
//}
