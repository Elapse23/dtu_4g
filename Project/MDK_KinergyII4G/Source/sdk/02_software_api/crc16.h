#ifndef __CRC16_H__
#define __CRC16_H__


#include "stdint.h"


extern uint16_t CRC16(unsigned char *pBuffer, int Length);
extern uint16_t CRC16_M(unsigned char *pBuffer, int Length, uint16_t crc_val);

#endif



