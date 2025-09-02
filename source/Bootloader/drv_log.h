#ifndef __DRV_LOG_H__
#define __DRV_LOG_H__

//#include "n32g45x.h"
#include "Bootloader.h"

//#include "stream_cal.h"

//#include <stdint.h>
//#include <stdio.h>
//#include <string.h>
#include "BootloaderConfig.h"
#include "drv_vsprintf.h"


//#include "hal_rcc.h"



#if(LOG_LEV_VAL >  LOG_LEV_NONE)
//config

//#define LOG_VSPRINTF                vsprintf//
#define LOG_VSPRINTF                macro_vsprintf//vsprintf
//#define LOG_BUF                      wUART4TxBufLen

#define LOG_UART_TX_GPIO            UART4_TX_PORT,UART4_TX_PIN
//#define LOG_UART_RX_GPIO            UART4_RX_PORT,UART4_RX_PIN
//#define LOG_UART_RCC_OPEN()         APB_RCC_OPEN(APB1,UART4)
//#define LOG_UART_RCC_CLOSE()        APB_RCC_CLOSE(APB1,UART4)
//about mcu reg
//#define LOG_GPIO_RCC_OPEN()         APB_RCC_OPEN(APB2,GPIOC)
#define LOG_UART_TX_INIT()          do{ HAL_GPIO_SET_HIGHT(LOG_UART_TX_GPIO);\
                                        HAL_GPIO_SET_MODE(LOG_UART_TX_GPIO,AF_MODE_PUSH_PULL);}while(0)//do{ APB_RCC_OPEN(APB2,GPIOC);\

#ifdef  LOG_UART_RX_GPIO

#define LOG_UART_RX_INIT()          do{HAL_GPIO_SET_HIGHT(LOG_UART_RX_GPIO);\
                                       HAL_GPIO_SET_MODE(LOG_UART_RX_GPIO,INPUT_PULL_UP);}while(0)
#endif

#define LOG_URAT_RESET()           do{APB_RCC_CLOSE(APB1,UART4);APB_RCC_OPEN(APB1,UART4);}while(0)


void drv_log_init(void);




#if(LOG_LEV_VAL >=  LOG_LEV_RAM)

#define log_char(x) do{   HAL_UART_TX_DATA(LOG_UART,'0' + (x >>4) &0xf); \
                          while(RESET == HAL_UART_TX_IS_EMPTY(LOG_UART)); \
                          HAL_UART_TX_DATA(LOG_UART,'0' + (x >>0)&0x0f); \
                          while(RESET == HAL_UART_TX_IS_EMPTY(LOG_UART));}while(0)
//#define log_num_8(x)do{
//                          HAL_UART_TX_DATA(LOG_UART,'0' + (x /100)); \
//                          while(RESET == HAL_UART_TX_IS_EMPTY(LOG_UART)); \
//                          HAL_UART_TX_DATA(LOG_UART,'0' + ((x / 10) % 10)); \
//                          while(RESET == HAL_UART_TX_IS_EMPTY(LOG_UART)); \
//                          HAL_UART_TX_DATA(LOG_UART,'0' + (x % 10)); \
//                          while(RESET == HAL_UART_TX_IS_EMPTY(LOG_UART));}while(0)
//#define log_char(x)
#else
#define log_char(x)
#endif

void log_hold_printf(const char *fmt, ...);

typedef  void (*t_log_printf)(const char *fmt, ...);
typedef  void (*t_log_sector)(uint8_t*ptr,uint16_t len);


#if (LOG_RAM_OFFSET_EN)
#define macro_log_printf ((t_log_printf)(BootUARTFunc(log_hold_printf)))
#define macro_log_sector ((t_log_sector)(BootUARTFunc(log_sector_printf)))
#else
#define macro_log_printf log_hold_printf//((t_log_printf)BootUARTFunc(log_hold_printf))
#define macro_log_sector log_sector_printf//((t_log_sector)BootUARTFunc(log_sector_printf))
#endif



#if(LOG_LEV_VAL >=  LOG_LEV_ERROR)
    #define drv_log_handle()//void drv_log_handle(void);
    #define log_error  macro_log_printf
#else
    #define log_error(...)
    #define drv_log_handle(...)
    #define log_holde(...)
    //#define log_data(...)
#endif

#if(LOG_LEV_VAL >=  LOG_LEV_WARNING)
#define log_warning macro_log_printf
#else
#define log_warning(...)
#endif

#if(LOG_LEV_VAL >=  LOG_LEV_INFO )
    #define log_info  macro_log_printf
    void log_sector_printf(uint8_t*ptr,uint16_t len);
    #define log_sector macro_log_sector
#else
    #define log_info(...)
    #define log_sector(...)
#endif

#if(LOG_LEV_VAL >=  LOG_LEV_DEBUG)
#define log_debug      macro_log_printf
#if (LOG_RAM_OFFSET_EN)
#define log_line()
#else
#define log_line() macro_log_printf("%s[L-%d]\r\n",__func__,__LINE__)
#endif
#else
#define log_debug(...)
#define log_line()

#endif




#else
#define drv_log_init()
#define drv_log_handle()
#define log_data()
#define log_hold(...)
#define log_sector(...)
#define log_char(x)
#define log_error(...)
#define log_info(...)
#define log_debug(...)

#endif

#define log_fun_addr(func) log_debug("ram %x,rom %x\r\n",(((INT32U)func)+COPY_CODE_RAM_OFFSET),(INT32U)(func))

//





#endif
