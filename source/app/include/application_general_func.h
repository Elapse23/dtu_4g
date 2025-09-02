#ifndef _APPLICATION_GENERAL_FUNC_H_
#define _APPLICATION_GENERAL_FUNC_H_

#include <stdint.h>

uint8_t int_to_bytes_big_endain(uint8_t *pb_data_out,uint32_t dw_data_in,uint8_t b_data_size);
uint8_t int_to_bytes_little_endain(uint8_t *pb_data_out,uint32_t dw_data_in,uint8_t b_data_size);
uint8_t bytes_to_int_big_to_little_endain(uint8_t *pb_data_int,uint32_t *pdw_data_out,uint8_t b_data_size);
uint8_t bytes_to_int_little_to_little_endain(uint8_t *pb_data_int,uint32_t *pdw_data_out,uint8_t b_data_size);
uint8_t byte_sort_in_reverse_order(uint16_t w_data_in_len,uint8_t *pb_data_in);
uint8_t byte_fill_in_reverse_order(uint16_t w_data_in_len,uint8_t *pb_data_in,uint8_t *pb_data_out,uint16_t w_data_out_buffer_max_len);
uint8_t int_to_int_little_to_big_endain(uint32_t dw_data_in,uint32_t *pdw_data_out,uint8_t b_data_size);
uint8_t sGetStrFromSpecRangeWithChar(uint8_t bStartChar,
                                     uint16_t wStartCharOrder,
                                     uint8_t bEndChar,
                                     uint16_t bEndCharOrder,
                                     uint8_t *pbStrIn,
                                     uint8_t *pbStrOut,
                                     uint16_t w_data_out_buffer_size);
uint8_t sAddUnitStr(uint8_t * bFigure2Str,const uint8_t * PbUint2Bytes);
uint8_t sFigure2Str(uint8_t bfSign,uint32_t FigureIn,uint8_t *StrOut,uint8_t bMaxDataLen);
uint8_t sAddPoint2Str(uint8_t CharNum,uint8_t bShiftLeft,uint8_t *StrIn,uint8_t *StrOut);
uint32_t get_max_for_uint(uint32_t dw_value_num, ...);
uint32_t get_min_for_uint(uint32_t dw_value_num, ...);
#endif
