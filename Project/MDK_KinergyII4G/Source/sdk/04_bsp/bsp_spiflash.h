/*
 * @Author: 23Elapse userszy@163.com
 * @Date: 2025-09-02 10:30:45
 * @LastEditors: 23Elapse userszy@163.com
 * @LastEditTime: 2025-09-03 10:43:33
 * @FilePath: \DTU_4G\Project\MDK_KinergyII4G\Source\sdk\04_bsp\bsp_spiflash.h
 * @Description: 
 * 
 * Copyright (c) 2025 by 23Elapse userszy@163.com, All Rights Reserved. 
 */
#ifndef __BSP_SPIFLASH_H__
#define __BSP_SPIFLASH_H__


#include "stdint.h"
#include "n32g45x.h"

#define FLASH_SPI                		SPI1
#define FLASH_SPI_CLK            		RCC_APB2_PERIPH_SPI1

#define FLASH_SCK_PIN        			GPIO_PIN_5 /* PA.05 */
#define FLASH_SCK_PORT  				GPIOA      /* GPIOA */
#define FLASH_SCK_CLK   				RCC_APB2_PERIPH_GPIOA

#define FLASH_MISO_PIN       			GPIO_PIN_6 /* PA.06 */
#define FLASH_MISO_PORT 				GPIOA      /* GPIOA */
#define FLASH_MISO_CLK  				RCC_APB2_PERIPH_GPIOA

#define FLASH_MOSI_PIN       			GPIO_PIN_7 /* PA.07 */
#define FLASH_MOSI_PORT 				GPIOA      /* GPIOA */
#define FLASH_MOSI_CLK  				RCC_APB2_PERIPH_GPIOA

#define FLASH_CS_PIN             		GPIO_PIN_4
#define FLASH_CS_PORT       			GPIOA
#define FLASH_CS_CLK        			RCC_APB2_PERIPH_GPIOA



#define FLASH_PAGE_SIZE       			0x100		//256byte 一页
#define FLASH_SECTOR_SIZE       		0x1000		//4k byte 一扇区
#define FLASH_BLOCK_SIZE 				(16 * FLASH_SECTOR_SIZE)		//64k byte 一块

#define FLASH_CS_LOW() 					GPIO_ResetBits(FLASH_CS_PORT, FLASH_CS_PIN)
#define FLASH_CS_HIGH() 				GPIO_SetBits(FLASH_CS_PORT, FLASH_CS_PIN)




//Flash 操作命令
#define CMD_PAGE_PROGRAM				0x02	//页编程
#define CMD_READ_DATA              		0x03	//读数据
#define CMD_READ_STATUS              	0x05	//读状态寄存器
#define CMD_WRITE_ENABLE               	0x06	//写使能
#define CMD_ERASE_SECTOR              	0x20	//扇区擦除
#define CMD_ERASE_BLOCK              	0xD8	//块擦除
#define CMD_ERASE_CHIP              	0xC7	//芯片擦除



// New unified API (no bsp_ prefix)
extern void Flash_Init(void);
extern uint8_t Flash_WriteByte(uint8_t byte);
extern uint8_t Flash_ReadByte(void);

// Backwards compatibility macros (old bsp_* names)
#define bsp_flash_init   Flash_Init
#define bsp_flash_write_byte Flash_WriteByte
#define bsp_flash_read_byte  Flash_ReadByte




#endif
