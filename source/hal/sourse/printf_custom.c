#include "printf_custom.h"
#include "n32g45x.h"
#include "usart_config_log.h"
#include "stdarg.h"
#include "FreeRTOS.h"//固定引用顺序 NO.1
#include "semphr.h"  //固定引用顺序 NO.2
#include "application_general_macro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define debug_printf_enable
#define local_printf
#define dx_printf
#define yl_printf

#ifdef debug_printf_enable
#ifdef local_printf
#define TRANSMIT_BY_UART
#ifdef  TRANSMIT_BY_USB
    #define data_transmit(data,len) usbd_com_send(data,len)
#endif

#ifdef TRANSMIT_BY_UART
    #define data_transmit(data,len) usart_log_transmit_trig(len,data)
#endif

static SemaphoreHandle_t  pst_printf_mutex = NULL;
static uint8_t debugVsprintfBuff[2000] = {0};

static void debug_printf_Semaphore_creat();
#endif
#endif

void yl_debug_printf(char *str, ... )
{
    #ifdef yl_printf
    #ifdef debug_printf_enable
    va_list argptr = {0,};
    uint16_t len = 0;

    if(strlen(str) >= 512)
    {
        data_transmit("DebugPrintfOverLen\n", 20);
        return;
    }

    xSemaphoreTake(pst_printf_mutex, portMAX_DELAY);

    va_start(argptr, str);
//    len = vsprintf((char *)debugVsprintfBuff, str, argptr);
    len = vsnprintf((char *)debugVsprintfBuff, 2000, str, argptr);
    va_end(argptr);

    data_transmit(debugVsprintfBuff,len);

    xSemaphoreGive(pst_printf_mutex);
    #else
    va_list argptr = {0,};
    (void)enableFlag;
    va_start(argptr, str);
    __set_PRIMASK(1);
    printf(str,argptr);
    __set_PRIMASK(0);
    va_end(argptr);
    #endif
    #endif
}

void DebugPrintf(uint32_t enableFlag,char *str, ... )
{
    #ifdef dx_printf
    #ifdef debug_printf_enable
    va_list argptr = {0,};
    uint16_t len = 0;

    if( enableFlag != TRUE )
    {
        data_transmit("DebugPrintfDisable\n", 20);
        return;
    }

    if(strlen(str) >= 512)
    {
        data_transmit("DebugPrintfOverLen\n", 20);
        return;
    }

    if(pst_printf_mutex != 0)
    {
        xSemaphoreTake(pst_printf_mutex, portMAX_DELAY);
    }

    va_start(argptr, str);
//    len = vsprintf((char *)debugVsprintfBuff, str, argptr);
    len = vsnprintf((char *)debugVsprintfBuff, 2000, str, argptr);
    va_end(argptr);

    data_transmit(debugVsprintfBuff,len);

    if(pst_printf_mutex != 0)
    {
        xSemaphoreGive(pst_printf_mutex);
    }
    #else
    va_list argptr = {0,};
    (void)enableFlag;
    va_start(argptr, str);
    __set_PRIMASK(1);
    printf(str,argptr);
    __set_PRIMASK(0);
    va_end(argptr);
    #endif
    #endif
}

void debug_printf_init()
{
    #ifdef debug_printf_enable
    #ifdef local_printf
    debug_printf_Semaphore_creat();
    #endif
    #endif

}
#ifdef debug_printf_enable
#ifdef local_printf
static void debug_printf_Semaphore_creat()
{
    pst_printf_mutex = xSemaphoreCreateMutex();

    if(NULL == pst_printf_mutex)
    {

    }
}
#endif
#endif
