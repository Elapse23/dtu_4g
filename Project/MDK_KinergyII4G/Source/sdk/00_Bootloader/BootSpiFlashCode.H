/************************************************************************************************
File Name:      BootSpiFlashCode.H
Description:    bootloader spi flash operation code
Version:        V1.0
Author:         chenweijie
Date:           2018-08-24
*************************************************************************************************/
#ifndef __BOOT_SPIFLASH_CODE__
#define __BOOT_SPIFLASH_CODE__

#include "BootLoader.H"
#include "BootFlashCode.H"
#include "BootUARTCode.H"
#include "n32g45x_spi.h"
//#include "drv_spiflash.h"//¨¢¨´¨º¡À
#include "drv_log.h"

#include "BootloaderConfig.h"

#define W25Q80 			    0xEF13 	
#define W25Q16 			    0xEF14
#define W25Q32 			    0xEF15
#define W25Q64 			    0xEF16
#define W25Q128			    0xEF17
#define W25Q256 		    0xEF18

#define CMD_PAGE_PROGRAM    0x02	//é¡µç¼–ç¨?
#define CMD_READ_DATA       0x03	//è¯»æ•°æ?
#define CMD_READ_REG        0x05	//è¯»çŠ¶æ€å¯„å­˜å™¨
#define CMD_WRITE_EN        0x06	//å†™ä½¿èƒ?
#define CMD_ERASE_SECTOR    0x20	//æ‰‡åŒºæ“¦é™¤
#define CMD_ERASE_BLOCK     0xD8	//å—æ“¦é™?
#define CMD_ERASE_CHIP      0xC7	//èŠ¯ç‰‡æ“¦é™¤






//flash pin

//#define EX_FLASH_GPIO_WP        GPIOC,2

//SPI
#if(0 == SPI_REMAP)
#define SPI1_MISO_GPIO          GPIOA,6             
#define SPI1_MOSI_GPIO          GPIOA,7
#define SPI1_SCK_GPIO           GPIOA,5
#define EX_FLASH_GPIO_CS        GPIOC,3
#endif

//SPI pin
#define EX_FLASH_GPIO_MISO      SPI1_MISO_GPIO
#define EX_FLASH_GPIO_MOSI      SPI1_MOSI_GPIO
#define EX_FLASH_GPIO_SCK       SPI1_SCK_GPIO


//uint16_t W25QXX_TYPE =	W25Q64;	


//saved in front of every firmware package




/************************************************************************************************
* spi flash operation funs
************************************************************************************************/


#if (true == HalBootSpiFlashFuncEN)


#define MACRO_FLASH_GPIO_INIT_SCK()    do{ HAL_GPIO_SET_LOW(EX_FLASH_GPIO_SCK);\
                                            HAL_GPIO_SET_MODE(EX_FLASH_GPIO_SCK,AF_MODE_PUSH_PULL);}while(0)
#define MACRO_FLASH_GPIO_INIT_MOSI()   do{ HAL_GPIO_SET_LOW(EX_FLASH_GPIO_MOSI);\
                                                HAL_GPIO_SET_MODE(EX_FLASH_GPIO_MOSI,AF_MODE_PUSH_PULL);}while(0)
#define MACRO_FLASH_GPIO_INIT_MISO()   do{HAL_GPIO_SET_LOW(EX_FLASH_GPIO_MISO);\
                                        HAL_GPIO_SET_MODE(EX_FLASH_GPIO_MISO,AF_MODE_OPEN_DRAIN);}while(0)

#define MACRO_FLASH_GPIO_INIT_CS()     do{ HAL_GPIO_SET_HIGHT(EX_FLASH_GPIO_CS);\
                                                HAL_GPIO_SET_MODE(EX_FLASH_GPIO_CS,OUTPUT_PUSH_PULL);}while(0)
#ifdef  EX_FLASH_GPIO_WP                                               
#define MACRO_FLASH_GPIO_INIT_WP()     do{ HAL_GPIO_SET_LOW(EX_FLASH_GPIO_WP);\
                                                    HAL_GPIO_SET_MODE(EX_FLASH_GPIO_WP,OUTPUT_PUSH_PULL);}while(0)
#endif
#define MACRO_SPI_CS_HIGHT()            HAL_GPIO_SET_HIGHT(EX_FLASH_GPIO_CS)
#define MACRO_SPI_CS_LOW()              HAL_GPIO_SET_LOW(EX_FLASH_GPIO_CS)





//#define HalBootIsrSPI1ClkLow()      HAL_GPIO_SET_LOW(EX_FLASH_GPIO_SCK)
//#define HalBootIsrSPI1ClkHigh()     HAL_GPIO_SET_HIGHT(EX_FLASH_GPIO_SCK)
//#define HalBootIsrSPI1MOSIHigh()    HAL_GPIO_SET_HIGHT(EX_FLASH_GPIO_MOSI)
//#define HalBootIsrSPI1MOSILow()     HAL_GPIO_SET_LOW(EX_FLASH_GPIO_MOSI);

#define HalBootSPI1MISOChk(bData)   bData  = (HAL_GPIO_READ(EX_FLASH_GPIO_MISO) != 0);
    //\(bData = ((SPIFLASH_MISO_GPIO->PDIR & (1 << SPIFLASH_MISO_Pin)) != 0))

#define HalBootIsrFlashCSLow()      HAL_GPIO_SET_LOW(EX_FLASH_GPIO_CS);
    //\(SPIFLASH_CS_GPIO->PDOR &= ~(1 << SPIFLASH_CS_Pin))

#define HalBootIsrFlashCSHigh()     HAL_GPIO_SET_HIGHT(EX_FLASH_GPIO_CS);          
  //  \(SPIFLASH_CS_GPIO->PDOR |= GPIO_PDOR_PDO(1<<SPIFLASH_CS_Pin))

#endif



/************************************************************************************************
* Spi Flash Function defination
************************************************************************************************/
#if (true == HalBootSpiFlashFuncEN)
void sBootSpi1PinInit(void);
void sBootFlashPinInit(void);
void sBootSpiFlashInit(void);

void sBootSpiFlashCodeStart(void);  
void sBootUpdateFromFlash(void);
void sBootSpiFlashCodeEnd(void);

#define MACRO_SpiFlashFunc    ((psVoidFunc)(BootSpiFlashFunc(sBootUpdateFromFlash)))
#else
#define MACRO_SpiFlashFunc 
#define sBootSpi1PinInit()
#endif  //#if(true == HalBootSpiFlashFuncEN)




#endif  //#ifndef __BOOT_SPIFLASH_CODE__


