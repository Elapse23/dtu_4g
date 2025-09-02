#ifndef __DRV_SPIFLASH_H__
#define __DRV_SPIFLASH_H__

#include "stdint.h"
#include "n32g45x.h"

#define sFLASH_SPI                		SPI1
#define sFLASH_SPI_CLK            		RCC_APB2_PERIPH_SPI1

#define sFLASH_SPI_SCK_PIN        		GPIO_PIN_5 /* PA.05 */
#define sFLASH_SPI_SCK_GPIO_PORT  		GPIOA      /* GPIOA */
#define sFLASH_SPI_SCK_GPIO_CLK   		RCC_APB2_PERIPH_GPIOA

#define sFLASH_SPI_MISO_PIN       		GPIO_PIN_6 /* PA.06 */
#define sFLASH_SPI_MISO_GPIO_PORT 		GPIOA      /* GPIOA */
#define sFLASH_SPI_MISO_GPIO_CLK  		RCC_APB2_PERIPH_GPIOA

#define sFLASH_SPI_MOSI_PIN       		GPIO_PIN_7 /* PA.07 */
#define sFLASH_SPI_MOSI_GPIO_PORT 		GPIOA      /* GPIOA */
#define sFLASH_SPI_MOSI_GPIO_CLK  		RCC_APB2_PERIPH_GPIOA

#ifdef KINERGY_II_WIFI
#define sFLASH_CS_PIN             		GPIO_PIN_3
#define sFLASH_CS_GPIO_PORT       		GPIOC
#define sFLASH_CS_GPIO_CLK        		RCC_APB2_PERIPH_GPIOC
#else
#define sFLASH_CS_PIN             		GPIO_PIN_4
#define sFLASH_CS_GPIO_PORT       		GPIOA
#define sFLASH_CS_GPIO_CLK        		RCC_APB2_PERIPH_GPIOA
#endif


#define SPI_FLASH_PAGE_SIZE       		0x100		//256byte 一页
#define SPI_FLASH_SECTOR_SIZE       	0x1000		//4k byte 一页
#define SPI_FLASH_BLOCK_SIZE 			(16 * SPI_FLASH_SECTOR_SIZE)		//64k byte 一块

#define SPIFLASH_CS_LOW() 				GPIO_ResetBits(sFLASH_CS_GPIO_PORT, sFLASH_CS_PIN)
#define SPIFLASH_CS_HIGH() 				GPIO_SetBits(sFLASH_CS_GPIO_PORT, sFLASH_CS_PIN)




//SPI Flash相关指令
#define CMD_PAGE_PROGRAM				0x02	//页编程
#define CMD_READ_DATA              		0x03	//读数据
#define CMD_READ_REG              		0x05	//读状态寄存器
#define CMD_WRITE_EN               		0x06	//写使能
#define CMD_ERASE_SECTOR              	0x20	//扇区擦除
#define CMD_ERASE_BLOCK              	0xD8	//块擦除
#define CMD_ERASE_CHIP              	0xC7	//芯片擦除



extern void drv_spi_flash_init(void);

extern void drv_flash_read(uint8_t *pData, uint32_t Addr, uint16_t Length);
extern void drv_flash_erase_chip(void);
extern void drv_flash_erase_sector(uint32_t Addr);
extern void drv_flash_write_buffer(uint8_t* pData, uint32_t Addr, uint16_t Length);


#endif

