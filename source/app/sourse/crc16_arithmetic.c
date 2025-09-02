#include "crc16_arithmetic.h"
#include "crc16_table.h"
#include "application_general_macro.h"

uint8_t check_crc16(uint8_t *pb_data_in, uint16_t w_data_in_len)
{
    if(NULL == pb_data_in)
    {
        return FALSE;
    }

    uint8_t b_crc16_high = 0xFF;
    uint8_t b_crc16_low = 0xFF;

    for(uint16_t w_cnt_i = 0;w_cnt_i < w_data_in_len;w_cnt_i++)
    {
        crc16_1_byte(*(pb_data_in + w_cnt_i), &b_crc16_high, &b_crc16_low);
    }

    if((0 == b_crc16_high) && (0 == b_crc16_low))
    {
        return TRUE;
    }

    return FALSE;
}

uint8_t calculate_crc16(uint8_t *pb_data_in, uint16_t w_data_in_len,uint8_t *pb_crcl_out,uint8_t *pb_crch_out)
{
    if((NULL == pb_data_in) || (NULL == pb_crcl_out) || (NULL == pb_crch_out))
    {
        return FALSE;
    }

    uint8_t b_crc16_high = 0xFF;
    uint8_t b_crc16_low = 0xFF;

    for(uint16_t w_cnt_i = 0;w_cnt_i < w_data_in_len;w_cnt_i++)
    {
        crc16_1_byte(*(pb_data_in + w_cnt_i), &b_crc16_high, &b_crc16_low);
    }

    *pb_crcl_out = b_crc16_low;
    *pb_crch_out = b_crc16_high;

    return TRUE;
}
