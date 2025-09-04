/************************************************************************************************
File Name:      BootlaoderAPP.C
Description:    bootloader APP functions and variables, code in APP area
Version:        V1.0
Author:         yebh
Date:           2022-11-02
*************************************************************************************************/
#ifndef __BOOT_LOADER_APP_H__
#define __BOOT_LOADER_APP_H__
#include "BootloaderHead.H"
#include "BootLoader.H"    // 取消注释以获得变量定义
#include "BootUARTCode.H"
//#include "BootCANCode.H"
#include "BootFlashCode.H"
#include "IcpHead.h"



#define UPDAT_EXFLASH_TEST  1
typedef enum{
    NET_CMD_OK                       = 0,
    NET_ERROR_VER                   ,// = 1,
    NET_ERROR_TYPE                  ,// = 2,
    NET_ERROR_ADDR                  ,// = 3,
    NET_ERROR_LEN                   ,// = 4,
    NET_ERROR_DATA                  ,// = 5,
    NET_ERROR_CONTROL               ,// = 6,
    NET_ERROR_SET                   ,// = 7,
    NET_ERROR_UPADTE_START          ,// = 8,
    NET_ERROR_UPADTE_STOP           ,// = 9,
    NET_ERROR_UPADTE_TYPE           ,// = 10 ,
    NET_ERROR_UPADTE_PACK_SUM       ,// = 11 ,
    NET_ERROR_CNETER                ,// = 12 ,
    NET_ERROR_MCU_SUPPORT           ,// = 13 ,
    NET_ERROR_MCU_TYPE              ,// = 14 ,
    NET_ERROR_FIRWARE_IDN_SUPPORT   ,// = 15 ,
    NET_ERROR_FIRWARE_IDN           ,// = 16 ,
    NET_ERROR_UPADTE_PACK_NUM       ,// = 17 ,
    NET_ERROR_UPADTE_DATA           ,// = 18 ,
    NET_ERROR_CHN_NOT_LOGIN         ,// = 19 ,
    NET_ERROR_PACK_OPERAT           ,// = 20 ,
    NET_ERROR_SHUT_DOWN             ,// = 21 ,
    NET_ERROR_FLASH_ERASE           ,// = 22 ,
    NET_ERROR_GET_LOGS              ,// = 23 ,
    NET_ERROR_UPADTE_FLASHING       ,// = 24 ,
    NET_ERROR_UPADTE_UUID           ,// = 25 ,
    NET_ERROR_TARGE_ADDR_FORBID     //,// = 26 ,
}E_NET_CMD;

//#define MALLOC_RAM(ptr,type) do{  ptr = (type*)pvPortMalloc(sizeof(type));}while(NULL == ptr)

//#define MALLOC_RAM_TYPE(ptr,type)   do{  ptr = (type*)pvPortMalloc(sizeof(type));}while(0)


#define MALLOC_RAM_TYPE(ptr,type)   do{   ptr = (type*)pvPortMalloc(sizeof(type));\
                                         if(ptr != NULL)memset(ptr,0,sizeof(type));}while(0)

#define MALLOC_RAM_LEN(ptr,len)    do{   ptr = (uint8_t*)pvPortMalloc(len);\
                                        if(ptr != NULL)memset(ptr,0,len);}while(0)


//#define MALLOC_RAM_LEN(ptr,len)    do{  uint8_t macro_timeout = 0xff; \
//                                        do{
//                                            ptr = (uint8_t*)pvPortMalloc(len); \
//                                            if(ptr != NULL){ \
//                                                memset(ptr,0,len); \
//                                                break; \
//                                            } \
//                                        }while(macro_timeout--); \
//                                    }while(0)

//__INLINE void malloc_ram_len(uint8_t *ptr ,uint8_t len)
//{
//    uint8_t timeout = 0xff;
//    do{
//        ptr = (uint8_t*)pvPortMalloc(len);
//        if(ptr != NULL){
//            memset(ptr,0,len);
//            break;
//        }
//    }while( timeout--);
//}




//#define MALLOC_RAM_LEN(ptr,len)    do{   ptr = (uint8_t*)pvPortMalloc(len);\
//                                    if(ptr != NULL){memset(ptr,0,len);break;}}while(1)

//#pragma pack()
#define FIRST_PACK_SIZE (cdwDevFirmAddrStart + sizof(T_FIRWAREINFO) + 1 + 2 + 4 + 2)


//DebugPrintf(UPDAT_EXFLASH_TEST,"%02x",stUpdatePackInfo.bFileMD5[i]);

// 直接使用当前工程的日志接口替代printf_custom.h
#include "bsp_log_manager.h"

// 调试宏定义 - 使用当前工程的日志系统
#ifndef UPDAT_EXFLASH_TEST
#define UPDAT_EXFLASH_TEST          0
#endif

#if(UPDAT_EXFLASH_TEST)
#define  debug_updata_test(...)             LOG_PRINTF(LOG_MODULE_SYSTEM, LOG_LEVEL_DEBUG, __VA_ARGS__)
#define  debug_updata_sector(dptr,dlen)     debug_printf_sector(dptr,dlen)
void debug_printf_sector(uint8_t* dptr,uint16_t dlen);
#else
#define  debug_updata_test(...)
#define  debug_updata_sector(dptr,dlen)
#endif

// 通用调试信息宏 - 映射到当前工程日志系统
#define  debug_updata_info(...)             LOG_PRINTF(LOG_MODULE_SYSTEM, LOG_LEVEL_INFO, __VA_ARGS__)

// DebugPrintf宏定义 - 兼容原有代码
#ifndef DebugPrintf
#define DebugPrintf(enable, fmt, ...)   \
    do { \
        if (enable) { \
            LOG_PRINTF(LOG_MODULE_SYSTEM, LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__); \
        } \
    } while(0)
#endif

// yl_debug_printf函数定义 - 映射到日志系统
#define yl_debug_printf(fmt, ...)       LOG_PRINTF(LOG_MODULE_SYSTEM, LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
//#define  debug_updata_info(...)

#define  debug_updata_error(...)            yl_debug_printf(__VA_ARGS__)
//#define  debug_updata_error(...)
#define  debug_line() DebugPrintf(TRUE,"%s[L-%d]\r\n",__func__,__LINE__)

extern const uint8_t flash_flag_start;

// 外部函数声明 - 需要在当前工程中实现或链接
extern bool extern_flash_read(uint32_t addr, uint16_t len, uint8_t *data);
extern bool extern_flash_wirte(uint32_t addr, uint16_t len, uint8_t *data);
extern bool check_crc16(uint8_t *data, uint16_t len);
extern void calculate_crc16(uint8_t *data, uint16_t len, uint8_t *crc_low, uint8_t *crc_high);
extern void rs485_tx_data_foreground(uint8_t addr, uint8_t func, uint16_t cmd, uint16_t len, uint8_t *data, bool wait_ack);
extern void FIRMWARE_UPDATE_SET_BUADRATE_9600(void);
extern void FIRMWARE_UPDATE_SET_BUADRATE_115200(void);

// FreeRTOS相关宏定义
#ifndef pdMS_TO_TICKS_USER
#define pdMS_TO_TICKS_USER(ms)  pdMS_TO_TICKS(ms)
#endif

void boot_config_init(void);
void boot_cmd_handle(uint8_t* rx_buf,uint16_t rx_len);
//bool flash_updata_read_check(void);
bool updata_data_check_all(void);
uint16_t updata_data_read_by_addr(uint32_t flash_addr,uint8_t* dptr);

INT8U sBootAppICPAnalysis(AppUartInfo *pstAppUartInfo);

//E_NET_CMD wire_updata_self_code(firmware_packet* pst_firmware_packet,uint16_t w_firmware_packet_len,device_update_info_current *pst_device_update_info_current);


//E_NET_CMD wire_updata_self_config(firmware_packet* pst_firmware_packet,uint16_t w_firmware_packet_len,device_update_info_current *pst_device_update_info_current);
//E_NET_CMD wireless_updata_save(uint32_t pack,uint8_t *pdata,uint16_t w_firmware_packet_len);

//bool updata_self_pack_save(uint32_t pack,uint8_t* ptr,uint16_t len);
extern uint8_t boot_mode   ;
extern uint8_t boot_result ;
extern uint8_t boot_config ;
extern uint8_t boot_flash  ;
extern uint8_t boot_retcnt ;

// 缺失的变量定义
extern uint8_t ble_ftp_updata_flag;

// 缺失的函数声明
void calculate_crc16(uint8_t* pdata, uint16_t dlen, uint8_t* crc_low, uint8_t* crc_high);
bool check_crc16(uint8_t* data, uint16_t len);
void LOG_PRINTF(int module, int level, const char* fmt, ...);

// 波特率设置函数
extern void firmware_update_set_baudrate_9600(void);
extern void firmware_update_set_baudrate_115200(void);

// 兼容性宏定义 - 将原函数名映射到新函数名
#define FIRMWARE_UPDATE_SET_BUADRATE_9600    firmware_update_set_baudrate_9600
#define FIRMWARE_UPDATE_SET_BUADRATE_115200  firmware_update_set_baudrate_115200

// 外部Flash函数映射
#define extern_flash_read   spi_flash_read
#define extern_flash_wirte  spi_flash_write  // 注意：原拼写错误保持一致

// RS485通信函数映射
#define rs485_tx_data_foreground  uart_tx_data_foreground

#endif
