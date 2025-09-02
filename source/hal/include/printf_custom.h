#ifndef _PRINTF_CUSTOM_H_
#define _PRINTF_CUSTOM_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "debug_printf_manage.h"

void yl_debug_printf(char *str, ... );
void DebugPrintf(uint32_t enableFlag,char *str, ... );
void debug_printf_init();

#endif
