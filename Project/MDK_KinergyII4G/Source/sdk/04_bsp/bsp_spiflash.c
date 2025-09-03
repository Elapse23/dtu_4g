/*
 * @Author: 23Elapse userszy@163.com
 * @Date: 2025-09-02 10:30:45
 * @LastEditors: 23Elapse userszy@163.com
 * @LastEditTime: 2025-09-03 13:47:02
 * @FilePath: \DTU_4G\Project\MDK_KinergyII4G\Source\sdk\04_bsp\bsp_spiflash.c
 * @Description: SPI Flash驱动实现文件 - 简化命名版本
 * 
 * Copyright (c) 2025 by 23Elapse userszy@163.com, All Rights Reserved. 
 */

#include "bsp_spiflash.h"

#define W25Q80 			0xEF13
#define W25Q16 			0xEF14
#define W25Q32 			0xEF15
#define W25Q64 			0xEF16
#define W25Q128			0xEF17
#define W25Q256 		0xEF18

uint16_t W25QXX_TYPE = W25Q64;

/**
 * @brief  反初始化SPI Flash外设
 */
static void Flash_LowLevel_DeInit(void)
{
    GPIO_InitType GPIO_InitStructure;

    /*!< 禁用 FLASH_SPI  */
    SPI_Enable(FLASH_SPI, DISABLE);

    /*!< 反初始化 FLASH_SPI */
    SPI_I2S_DeInit(FLASH_SPI);

    /*!< FLASH_SPI 外设时钟禁用 */
    RCC_EnableAPB2PeriphClk(FLASH_SPI_CLK, DISABLE);

    /*!< 配置 FLASH_SPI 引脚: SCK */
    GPIO_InitStructure.Pin       = FLASH_SCK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitPeripheral(FLASH_SCK_PORT, &GPIO_InitStructure);

    /*!< 配置 FLASH_SPI 引脚: MISO */
    GPIO_InitStructure.Pin = FLASH_MISO_PIN;
    GPIO_InitPeripheral(FLASH_MISO_PORT, &GPIO_InitStructure);

    /*!< 配置 FLASH_SPI 引脚: MOSI */
    GPIO_InitStructure.Pin = FLASH_MOSI_PIN;
    GPIO_InitPeripheral(FLASH_MOSI_PORT, &GPIO_InitStructure);

    /*!< 配置 FLASH_CS_PIN 引脚: Flash 片选引脚 */
    GPIO_InitStructure.Pin = FLASH_CS_PIN;
    GPIO_InitPeripheral(FLASH_CS_PORT, &GPIO_InitStructure);
}

/**
 * @brief  初始化SPI Flash外设
 */
static void Flash_LowLevel_Init(void)
{
    GPIO_InitType GPIO_InitStructure;

    /*!< FLASH_CS_GPIO, FLASH_MOSI_GPIO, FLASH_MISO_GPIO
         和 FLASH_SCK_GPIO 外设时钟使能 */
    RCC_EnableAPB2PeriphClk(
        FLASH_CS_CLK | FLASH_MOSI_CLK | FLASH_MISO_CLK | FLASH_SCK_CLK, ENABLE);

    /*!< FLASH_SPI 外设时钟使能 */
    RCC_EnableAPB2PeriphClk(FLASH_SPI_CLK, ENABLE);

    /*!< 配置 FLASH_SPI 引脚: SCK */
    GPIO_InitStructure.Pin        = FLASH_SCK_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitPeripheral(FLASH_SCK_PORT, &GPIO_InitStructure);

    /*!< 配置 FLASH_SPI 引脚: MOSI */
    GPIO_InitStructure.Pin = FLASH_MOSI_PIN;
    GPIO_InitPeripheral(FLASH_MOSI_PORT, &GPIO_InitStructure);

    /*!< 配置 FLASH_SPI 引脚: MISO */
    GPIO_InitStructure.Pin       = FLASH_MISO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitPeripheral(FLASH_MISO_PORT, &GPIO_InitStructure);

    /*!< 配置 FLASH_CS_PIN 引脚: Flash 片选引脚 */
    GPIO_InitStructure.Pin       = FLASH_CS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitPeripheral(FLASH_CS_PORT, &GPIO_InitStructure);
}

/**
 * @brief  反初始化SPI Flash驱动
 */
void Flash_DeInit(void)
{
    Flash_LowLevel_DeInit();
}

/**
 * @brief  初始化SPI Flash
 */
void Flash_Init(void)
{
	SPI_InitType SPI_InitStructure;

    Flash_LowLevel_Init();

    /*!< 取消选择Flash: 片选拉高 */
    FLASH_CS_HIGH();

    /*!< SPI配置 */
    SPI_InitStructure.DataDirection = SPI_DIR_DOUBLELINE_FULLDUPLEX;
    SPI_InitStructure.SpiMode       = SPI_MODE_MASTER;
    SPI_InitStructure.DataLen       = SPI_DATA_SIZE_8BITS;
    SPI_InitStructure.CLKPOL        = SPI_CLKPOL_HIGH;
    SPI_InitStructure.CLKPHA        = SPI_CLKPHA_SECOND_EDGE;
    SPI_InitStructure.NSS           = SPI_NSS_SOFT;
    SPI_InitStructure.BaudRatePres  = SPI_BR_PRESCALER_4;
    SPI_InitStructure.FirstBit      = SPI_FB_MSB;
    SPI_InitStructure.CRCPoly       = 7;
    SPI_Init(FLASH_SPI, &SPI_InitStructure);

    /*!< 使能 FLASH_SPI  */
    SPI_Enable(FLASH_SPI, ENABLE);
}

/**
 * @brief  发送一个字节并接收一个字节
 * @param  byte: 要发送的字节
 * @return 接收到的字节
 */
uint8_t Flash_WriteByte(uint8_t byte)
{
    /*!< 等待发送缓冲区为空 */
    while (SPI_I2S_GetStatus(FLASH_SPI, SPI_I2S_TE_FLAG) == RESET);

    /*!< 通过SPI发送字节 */
    SPI_I2S_TransmitData(FLASH_SPI, byte);

    /*!< 等待接收到一个字节 */
    while (SPI_I2S_GetStatus(FLASH_SPI, SPI_I2S_RNE_FLAG) == RESET);

    /*!< 返回从SPI总线读取的字节 */
    return SPI_I2S_ReceiveData(FLASH_SPI);
}

/**
 * @brief  读取一个字节
 * @return 读取到的字节
 */
uint8_t Flash_ReadByte(void)
{
    return (Flash_WriteByte(0xFF));
}

/**
 * @brief  从Flash读取数据
 * @param  pData: 数据缓冲区指针
 * @param  Addr: 读取地址
 * @param  Length: 读取长度
 */
void Flash_Read(uint8_t *pData, uint32_t Addr, uint16_t Length)
{
	FLASH_CS_LOW();

	Flash_WriteByte(CMD_READ_DATA);

	if(W25QXX_TYPE == W25Q256)
	{
		Flash_WriteByte((uint8_t)(Addr>>24));
	}
	Flash_WriteByte((uint8_t)(Addr>>16));
	Flash_WriteByte((uint8_t)(Addr>>8));
	Flash_WriteByte((uint8_t)Addr);

	for(uint16_t i=0; i<Length; i++)
	{
		pData[i] = Flash_ReadByte();
	}

	FLASH_CS_HIGH();
}

/**
 * @brief  Flash写使能
 */
void Flash_WriteEnable(void)
{
	FLASH_CS_LOW();
	Flash_WriteByte(CMD_WRITE_ENABLE);
	FLASH_CS_HIGH();
}

/**
 * @brief  等待Flash空闲
 */
void Flash_WaitForIdle(void)
{
	uint8_t status = 0;

	FLASH_CS_LOW();
	Flash_WriteByte(CMD_READ_STATUS);

	do{
		status = Flash_ReadByte();
	}while((status & 0x01) == 0x01);

	FLASH_CS_HIGH();
}

/**
 * @brief  擦除整个Flash芯片
 */
void Flash_EraseChip(void)
{
	Flash_WriteEnable();
	FLASH_CS_LOW();
	Flash_WriteByte(CMD_ERASE_CHIP);
	FLASH_CS_HIGH();
	Flash_WaitForIdle();
}

/**
 * @brief  擦除Flash扇区
 * @param  Addr: 扇区地址
 */
void Flash_EraseSector(uint32_t Addr)
{
	Flash_WriteEnable();
	Flash_WaitForIdle();
	
	FLASH_CS_LOW();
	Flash_WriteByte(CMD_ERASE_SECTOR);
	if(W25QXX_TYPE == W25Q256)
	{
		Flash_WriteByte((uint8_t)(Addr>>24));
	}
	Flash_WriteByte((uint8_t)(Addr>>16));
	Flash_WriteByte((uint8_t)(Addr>>8));
	Flash_WriteByte((uint8_t)Addr);
	FLASH_CS_HIGH();
	
	Flash_WaitForIdle();
}

/**
 * @brief  向Flash写入一页数据
 * @param  pData: 数据缓冲区指针
 * @param  Addr: 写入地址
 * @param  Length: 写入长度（不超过一页）
 */
void Flash_WritePage(uint8_t *pData, uint32_t Addr, uint16_t Length)
{
	Flash_WriteEnable();

	FLASH_CS_LOW();
	Flash_WriteByte(CMD_PAGE_PROGRAM);
	if(W25QXX_TYPE == W25Q256)
	{
		Flash_WriteByte((uint8_t)(Addr>>24));
	}
	Flash_WriteByte((uint8_t)(Addr>>16));
	Flash_WriteByte((uint8_t)(Addr>>8));
	Flash_WriteByte((uint8_t)Addr);

	for(uint16_t i=0; i<Length; i++)
	{
		Flash_WriteByte(pData[i]);
	}

	FLASH_CS_HIGH();
	Flash_WaitForIdle();
}

/**
 * @brief  向Flash写入任意长度数据（带分页处理）
 * @param  pData: 数据缓冲区指针
 * @param  Addr: 写入地址
 * @param  Length: 写入长度
 */
void Flash_WriteBuffer(uint8_t* pData, uint32_t Addr, uint16_t Length)
{
    uint8_t num_of_page = 0, num_of_single = 0, addr = 0, count = 0, temp = 0;

    addr          = Addr % FLASH_PAGE_SIZE;
    count         = FLASH_PAGE_SIZE - addr;
    num_of_page   = Length / FLASH_PAGE_SIZE;
    num_of_single = Length % FLASH_PAGE_SIZE;

    /* 地址按页对齐 */
    if(0 == addr)
	{
        /* 长度小于一页 */
        if(0 == num_of_page)
        {
            Flash_WritePage(pData, Addr, Length);
        }
        /* 长度大于一页 */
        else
		{
            while(num_of_page--)
			{
                Flash_WritePage(pData, Addr, FLASH_PAGE_SIZE);
                Addr += FLASH_PAGE_SIZE;
                pData += FLASH_PAGE_SIZE;
            }
            Flash_WritePage(pData, Addr, num_of_single);
		}
    }
	else
	{
        /* 地址不按页对齐 */
        if(0 == num_of_page)
		{
            /* (长度 + 地址) > 页大小 */
            if(num_of_single > count)
			{
                temp = num_of_single - count;
                Flash_WritePage(pData, Addr, count);
                Addr += count;
                pData += count;
                Flash_WritePage(pData, Addr, temp);
            }
			else
			{
                Flash_WritePage(pData, Addr, Length);
			}
        }
		else
		{
            /* 长度大于页大小 */
            Length -= count;
            num_of_page = Length / FLASH_PAGE_SIZE;
            num_of_single = Length % FLASH_PAGE_SIZE;

            Flash_WritePage(pData, Addr, count);
            Addr += count;
            pData += count;

            while(num_of_page--)
			{
                Flash_WritePage(pData, Addr, FLASH_PAGE_SIZE);
                Addr += FLASH_PAGE_SIZE;
                pData += FLASH_PAGE_SIZE;
            }

            if(0 != num_of_single)
            {
                Flash_WritePage(pData, Addr, num_of_single);
            }
        }
    }
}
