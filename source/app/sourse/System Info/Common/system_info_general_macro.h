#ifndef _SYSTEM_INFO_GENERAL_MACRO_H_
#define _SYSTEM_INFO_GENERAL_MACRO_H_

#include <stdint.h>

#ifndef TRUE
    #define TRUE                          	    (1)
#endif

#ifndef FALSE
    #define FALSE                               (0)
#endif

#ifndef NULL
    #define NULL                              	(void *)0
#endif

#ifndef INT_TO_BYTE
	#define INT_TO_BYTE(DATA,POSITION) 		    ((uint8_t)((DATA & (0xFF << (POSITION * 8))) >> (POSITION * 8)))
#endif

typedef union
{
	uint8_t 	byte;
 	struct
 	{
		uint8_t   bit0	:1;
		uint8_t   bit1	:1;
		uint8_t   bit2	:1;
		uint8_t   bit3	:1;
		uint8_t   bit4	:1;
		uint8_t   bit5	:1;
		uint8_t   bit6	:1;
		uint8_t   bit7	:1;
	}bits;
}net_byte_8_bits_l_endian;


#define get_data_addr(x)             			((uint32_t)(&(x)))

#endif
