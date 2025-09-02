#include "application_general_func.h"
#include "application_general_macro.h"
#include <stdarg.h>

uint8_t int_to_bytes_big_endain(uint8_t *pb_data_out,uint32_t dw_data_in,uint8_t b_data_size)
{
    if(NULL == pb_data_out)
    {
        return FALSE;
    }

    if(b_data_size > 4)
    {
        return FALSE;
    }
    uint8_t b_shift_len = 0;

    for(uint8_t b_index = 0;b_index < b_data_size;b_index++)
    {
        b_shift_len = (8 * ((b_data_size - b_index - 1)));
        pb_data_out[b_index] = \
            (uint8_t)(((uint32_t)(0xFF << b_shift_len) & dw_data_in) >> b_shift_len);
    }

    return TRUE;
}

uint8_t int_to_bytes_little_endain(uint8_t *pb_data_out,uint32_t dw_data_in,uint8_t b_data_size)
{
    if(NULL == pb_data_out)
    {
        return FALSE;
    }

    if(b_data_size > 4)
    {
        return FALSE;
    }
    uint8_t b_shift_len = 0;

    for(uint8_t b_index = 0;b_index < b_data_size;b_index++)
    {
		b_shift_len = 8 * b_index;
		pb_data_out[b_index] = \
            (uint8_t)(((uint32_t)(0xFF << b_shift_len) & dw_data_in) >> b_shift_len);
    }

    return TRUE;
}

uint8_t bytes_to_int_big_to_little_endain(uint8_t *pb_data_int,uint32_t *pdw_data_out,uint8_t b_data_size)
{
    if((NULL == pb_data_int) || (NULL == pdw_data_out))
    {
        return FALSE;
    }

    if(b_data_size > 4)
    {
        return FALSE;
    }

    switch(b_data_size)
    {
        case 1:
        {
            uint8_t *pb_int_local = (uint8_t *)pdw_data_out;

            *pb_int_local = 0;

            for(uint8_t b_index = 0;b_index < b_data_size;b_index++)
            {
                *pb_int_local |= (uint32_t)(pb_data_int[b_index] << (8 * (b_data_size - b_index - 1)));
            }
            break;
        }
        case 2:
        {
            uint16_t *pw_int_local = (uint16_t *)pdw_data_out;

            *pw_int_local = 0;

            for(uint8_t b_index = 0;b_index < b_data_size;b_index++)
            {
                *pw_int_local |= (uint16_t)(pb_data_int[b_index] << (8 * (b_data_size - b_index - 1)));
            }
            break;
        }
        case 3:
        case 4:
        {
            uint32_t *pdw_int_local = (uint32_t *)pdw_data_out;

            *pdw_int_local = 0;

            for(uint8_t b_index = 0;b_index < b_data_size;b_index++)
            {
                *pdw_int_local |= (uint32_t)(pb_data_int[b_index] << (8 * (b_data_size - b_index - 1)));
            }
            break;
        }
    }

    return TRUE;
}
uint8_t bytes_to_int_little_to_little_endain(uint8_t *pb_data_int,uint32_t *pdw_data_out,uint8_t b_data_size)
{
    if(NULL == pb_data_int)
    {
        return FALSE;
    }

    if(b_data_size > 4)
    {
        return FALSE;
    }

    switch(b_data_size)
    {
        case 1:
        {
            uint8_t *pb_int_local = (uint8_t *)pdw_data_out;
			*pb_int_local = 0;

            for(uint8_t b_index = 0;b_index < b_data_size;b_index++)
            {
                *pb_int_local |= (uint32_t)(pb_data_int[b_index] << (8 * b_index));
            }
            break;
        }

        case 2:
        {
            uint16_t *pw_int_local = (uint16_t *)pdw_data_out;
			*pw_int_local = 0;

            for(uint8_t b_index = 0;b_index < b_data_size;b_index++)
            {
                *pw_int_local |= (uint16_t)(pb_data_int[b_index] << (8 * b_index));
            }
            break;
        }
        case 3:
        case 4:
        {
            uint32_t *pdw_int_local = (uint32_t *)pdw_data_out;
			*pdw_int_local = 0;

            for(uint8_t b_index = 0;b_index < b_data_size;b_index++)
            {
                *pdw_int_local |= (uint32_t)(pb_data_int[b_index] << (8 * b_index));
            }
            break;
        }
    }

    return TRUE;
}
uint8_t byte_sort_in_reverse_order(uint16_t w_data_in_len,uint8_t *pb_data_in)
{
    if(NULL == pb_data_in)
    {
        return FALSE;
    }

    uint8_t b_header_data = 0;
    uint8_t b_tail_data = 0;

    for(uint16_t w_cnt_i = 0;w_cnt_i < (w_data_in_len / 2);w_cnt_i++)
    {
        b_header_data = pb_data_in[w_cnt_i];
        b_tail_data = pb_data_in[w_data_in_len - w_cnt_i - 1];
        pb_data_in[w_cnt_i] = b_tail_data;
        pb_data_in[w_data_in_len - w_cnt_i - 1] = b_header_data;
    }

    return TRUE;
}

uint8_t int_to_int_little_to_big_endain(uint32_t dw_data_in,uint32_t *pdw_data_out,uint8_t b_data_size)
{
    if(NULL == pdw_data_out)
    {
        return FALSE;
    }

    if(b_data_size > 4)
    {
        return FALSE;
    }

//    switch(b_data_size)
//    {
//        case 1:
//        {
//            uint8_t *pb_int_local = (uint8_t *)pdw_data_out;
//
//            *pb_int_local = (uint8_t)dw_data_in;
//            break;
//        }
//        case 2:
//        {
//            uint8_t *pb_int_local = (uint8_t *)pdw_data_out;
//
//            for(uint8_t b_index = 0;b_index < b_data_size;b_index++)
//            {
//                *(pb_int_local + b_index) = (uint8_t)(dw_data_in & (0xFF << ((b_data_size - b_index - 1) * 8))) >> ((b_data_size - b_index - 1) * 8);
//            }
//            break;
//        }
//        case 3:
//        case 4:
//        {
//            uint32_t *pdw_int_local = (uint32_t *)pdw_data_out;
//
//            *pdw_int_local = 0;
//
//            for(uint8_t b_index = 0;b_index < b_data_size;b_index++)
//            {
//                *(pdw_int_local + b_index) = (uint8_t)(dw_data_in & (0xFF << ((b_data_size - b_index - 1) * 8))) >> ((b_data_size - b_index - 1) * 8);
//            }
//            break;
//        }
//    }

	uint8_t *pb_int_local = (uint8_t *)pdw_data_out;

	for(uint8_t b_index = 0;b_index < b_data_size;b_index++)
	{
		*(pb_int_local + b_index) = (uint8_t)((dw_data_in & (0xFF << ((b_data_size - b_index - 1) * 8))) >> ((b_data_size - b_index - 1) * 8));
	}

    return TRUE;
}

uint8_t byte_fill_in_reverse_order(uint16_t w_data_in_len,uint8_t *pb_data_in,uint8_t *pb_data_out,uint16_t w_data_out_buffer_max_len)
{
    if((NULL == pb_data_in) || (NULL == pb_data_out))
    {
        return FALSE;
    }

    for(uint16_t w_cnt_i = 0;w_cnt_i < w_data_in_len;w_cnt_i++)
    {
		pb_data_out[w_data_in_len  - w_cnt_i - 1] = pb_data_in[w_cnt_i];
    }

    return TRUE;
}


uint8_t sGetStrFromSpecRangeWithChar(uint8_t bStartChar,
                                     uint16_t wStartCharOrder,
                                     uint8_t bEndChar,
                                     uint16_t bEndCharOrder,
                                     uint8_t *pbStrIn,
                                     uint8_t *pbStrOut,
                                     uint16_t w_data_out_buffer_size)
{
    uint16_t wCntI = 0;
    uint16_t wCntJ = 0;
    uint16_t wStartCharOrderTemp = 0;
    uint16_t bEndCharOrderTemp = 0;

    if((0 == pbStrIn) || (0 == pbStrOut))
    {
        return FALSE;
    }

    for(wStartCharOrderTemp = 0,wCntI = 0,wCntJ = 0;;wCntI++)
    {
        if('\0' == *(pbStrIn + wCntI))
        {
            return FALSE;
        }
        else if(bStartChar == *(pbStrIn + wCntI))
        {
            wStartCharOrderTemp++;
            if(wStartCharOrderTemp == wStartCharOrder)
            {
                wCntI++;
                break;
            }
        }
    }

    for(bEndCharOrderTemp = 0,wCntJ = 0;;wCntI++,wCntJ++)
    {
        if('\0' == *(pbStrIn + wCntI))
        {
            break;
        }
        else
        {
            if(bEndChar == *(pbStrIn + wCntI))
            {
                bEndCharOrderTemp++;
                if(bEndCharOrderTemp == bEndCharOrder)
                {
                    break;
                }
            }
            else
            {
                if(wCntJ < w_data_out_buffer_size)
                {
                    *(pbStrOut  + wCntJ) = *(pbStrIn + wCntI);
                }
                else
                {
                    return FALSE;
                }
            }
        }
    }

    if(wCntJ != 0)
    {
        return TRUE;
    }

    return FALSE;
}
uint8_t sFigure2Str(uint8_t bfSign,uint32_t FigureIn,uint8_t *StrOut,uint8_t bMaxDataLen)
{
    uint8_t bCntI = 0;
    uint8_t bCntJ = 0;
    uint8_t bCntk = 0;
    uint8_t bValRemainder = 0;
    uint32_t dwValDivider = FigureIn;
    uint8_t StrOutTemp[20] = {0,}; //up to 7 char

    if((FigureIn & 0x80) && bfSign && (0 != dwValDivider))
    {
        StrOut[bCntI++] = '-';
        dwValDivider = ~(FigureIn - 1);
    }
    while(0 != StrOutTemp)
    {
        bValRemainder = dwValDivider % 10;
        dwValDivider /= 10;
        StrOutTemp[bCntJ++] = bValRemainder + 48;
        if(dwValDivider < 10)
        {
            if(dwValDivider != 0)
            {
                StrOutTemp[bCntJ++] = dwValDivider + 48;
            }
            break;
        }
    }
    if(bMaxDataLen < bCntJ)
    {
        return 0;
    }
    for(bCntk = 0;bCntk < bCntJ;bCntk++)
    {
        StrOut[bCntI++] = StrOutTemp[bCntJ - bCntk - 1];
    }
    return bCntI;
}

uint8_t sAddPoint2Str(uint8_t CharNum,uint8_t bShiftLeft,uint8_t *StrIn,uint8_t *StrOut)
{
    uint8_t bCnti = 0;
    if((0 == StrIn) || (0 == StrOut))
    {
        return FALSE;
    }
    if(0 != bShiftLeft)
    {

        for(bCnti = 0;bCnti < CharNum;bCnti++)
        {
            if(bCnti < (CharNum - bShiftLeft))//1:'\0'  CharNum - bShiftLeft -1:Decimal point add position
            {
              StrOut[bCnti] = StrIn[bCnti];
            }
            else
            {
                StrOut[bCnti+1] = StrIn[bCnti];
            }
        }
        StrOut[CharNum - bShiftLeft] = '.';
    }
    else
    {
        for(bCnti = 0;bCnti < CharNum;bCnti++)
        {
            StrOut[bCnti] = StrIn[bCnti];
        }
        return CharNum;
    }
    return (CharNum + 1);//mark2019/11/06 01
}

uint8_t sAddUnitStr(uint8_t * bFigure2Str,const uint8_t * PbUint2Bytes)
{
	uint8_t bCntI = 0;
    if((0 == bFigure2Str) || (0 == PbUint2Bytes))
    {
        return 0;
    }//mark2019/10/23 05
	for(;*(PbUint2Bytes+bCntI) != '\0';bCntI++)
	{
		bFigure2Str[bCntI]= *(PbUint2Bytes + bCntI);
	}
	bFigure2Str[bCntI] = *(PbUint2Bytes + bCntI);
    return (bCntI);//2??óé??áê?・?  //mark2019/11/03 01
}

uint32_t get_max_for_uint(uint32_t dw_value_num, ...)//value_num, value1, value2, ...
{
    uint32_t dw_result = 0;
    uint32_t dw_data_temp = 0;
    va_list argptr = {0,};

    va_start(argptr, dw_value_num);

    for(uint32_t dw_cnt_i = 0;
        dw_cnt_i < dw_value_num;
        dw_cnt_i++)
    {
        dw_data_temp = va_arg(argptr,uint32_t);

        if(dw_data_temp > dw_result)
        {
            dw_result = dw_data_temp;
        }
    }

    va_end(argptr);

    return dw_result;
}
uint32_t get_min_for_uint(uint32_t dw_value_num, ...)//value_num, value1, value2, ...
{
    uint32_t dw_result = 0;
    uint32_t dw_data_temp = 0;
    va_list argptr = {0,};

    va_start(argptr, dw_value_num);

    dw_result = va_arg(argptr,uint32_t);

    for(uint32_t dw_cnt_i = 1;
        dw_cnt_i < dw_value_num;
        dw_cnt_i++)
    {
        dw_data_temp = va_arg(argptr,uint32_t);

        if(dw_data_temp < dw_result)
        {
            dw_result = dw_data_temp;
        }
    }

    va_end(argptr);

    return dw_result;
}
