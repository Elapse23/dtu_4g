
#ifndef __drv_vsprintf_H__
#define  __drv_vsprintf_H__
//#include <sys/types.h>
//#include <stdarg.h>
//#include <string.h>
//#include <stddef.h>//size_t
//#include <stdio.h>

#include "BootloaderConfig.h"
//#include "BootLoader.h"    //need BootUARTFunc
//#include "BootLoader.h"
typedef char *  va_list;


//typedef unsigned int     size_t;


#define _sizeof_type(T) ((uint8_t)((T*)0 + 1))
//
//#define _sizeof(T) ((size_t)(&T+1)-(size_t)(&T))

//适用于非数组
#define _sizeof(T) ((uint8_t)((T*)0 + 1))

//适用于数组
#define array_sizeof(T) ((int)(&T+1)-(int)(&T))

#define sizeof_v(var)   ((int)((char*)(&(var) + 1) - (char*)&(var)))
//
//#define sizeof_t(type)  ((size_t) ((type *)0 + 1))

#define SIZEOF(n) ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))


#define va_start(va, fmt) (va = (va_list)&fmt + SIZEOF(fmt))
#define va_arg(va, type) (*(type *)((va += SIZEOF(type)) - SIZEOF(type)))
#define va_end(va) (va = (va_list)0)



#define __SIZEOF(x) ((char*)(&(x) + 1) - (char*)&(x))



#if(LOG_LEV_VAL >  LOG_LEV_NONE)
#define VSPRINT_OFFSET_OPEN 1
int self_vsprintf(char *buf, const char *fmt, va_list args);
//int self_sprintf(char* buf ,const char *fmt,...);

typedef int (*t_self_vsprintf)(char *buf, const char *fmt, va_list args);
//typedef int (*t_self_sprintf)(char *buf, const char *fmt,...);

#define macro_vsprintf ((t_self_vsprintf)(BootUARTFunc(self_vsprintf)))
#define macro_sprintf //((t_self_sprintf)(BootUARTFunc(self_sprintf)))

#else

#define macro_vsprintf
#define macro_sprintf

#endif

#endif
