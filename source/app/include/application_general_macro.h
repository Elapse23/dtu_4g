#ifndef _GENERAL_MACROL_H_
#define _GENERAL_MACROL_H_

#include <stdint.h>

#ifndef TRUE
    #define TRUE                            (1)
#endif

#ifndef FALSE
  #define FALSE                             (0)
#endif

#ifndef NULL
  #define NULL                              (void *)0
#endif

/* bit operations */
#ifndef REG32
    #define REG32(addr)                     (*(volatile uint32_t *)(uint32_t)(addr))
#endif

#ifndef REG16
#define REG16(addr)                         (*(volatile uint16_t *)(uint32_t)(addr))
#endif

#ifndef REG8
#define REG8(addr)                          (*(volatile uint8_t *)(uint32_t)(addr))
#endif

#ifndef BIT
#define BIT(x)                              ((uint32_t)((uint32_t)0x01U<<(x)))
#endif

#ifndef BITS
#define BITS(start, end)                    ((0xFFFFFFFFUL << (start)) & (0xFFFFFFFFUL >> (31U - (uint32_t)(end))))
#endif

#ifndef GET_BITS
#define GET_BITS(regval, start, end)        (((regval) & BITS((start),(end))) >> (start))
#endif

#ifndef MIN
#define MIN(a,b) 							((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a,b) 							((a) > (b) ? (a) : (b))
#endif

#define INT_TO_BYTE(DATA,POSITION)  		((uint8_t)((DATA & (0xFF << (POSITION * 8))) >> (POSITION * 8)))

#ifndef SWITCH_CASE_START
	#define SWITCH_CASE_START		        {
#endif

#ifndef SWITCH_CASE_END
	#define SWITCH_CASE_END			        }break;
#endif

#ifndef SWITCH_CASE_START_NO_BREAK
	#define SWITCH_CASE_START_NO_BREAK		{
#endif

#ifndef SWITCH_CASE_END_NO_BREAK
	#define SWITCH_CASE_END_NO_BREAK	    }
#endif

#ifndef SWITCH_DEFAULT_START
	#define SWITCH_DEFAULT_START	        {
#endif

#ifndef SWITCH_DEFAULT_END
	#define SWITCH_DEFAULT_END		        }break;
#endif

#ifndef TO_STRING
#define TO_STRING(STR)                      #STR
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
}byte_8_bits_l_endian;

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *
 @ msg queue
 * *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
typedef struct
{
    uint16_t w_data_len;
    uint8_t *pb_data;
	void *pv_append_info;
}genaral_communicate_data_info;

typedef void (*msg_queue_send)(void * const pb_data_in);

typedef enum
{
    msg_queue_peek,
    msg_queue_shift
}msg_queue_receive_mode;
typedef void (*msg_queue_receive)(void * const * const pb_data_out,msg_queue_receive_mode enum_receive_mode);

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *
 @ mutual exclusion section
 * *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifndef MUTUAL_EXCLUSION_SECTION_ENTRY
    #define MUTUAL_EXCLUSION_SECTION_ENTRY
#endif
#ifndef MUTUAL_EXCLUSION_SECTION_EXIT
    #define MUTUAL_EXCLUSION_SECTION_EXIT
#endif

#endif
