#ifndef __BSP_TIMER_H__
#define __BSP_TIMER_H__

#include "stdint.h"
#include "gd32f4xx.h"


extern void bsp_timer_init(uint32_t timer_periph);
extern void bsp_timer_interrupt_onoff(uint32_t timer_periph, uint8_t on_off);
#endif
