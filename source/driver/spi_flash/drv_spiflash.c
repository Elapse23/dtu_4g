#include "drv_spiflash.h"



#define W25Q80 			0xEF13
#define W25Q16 			0xEF14
#define W25Q32 			0xEF15
#define W25Q64 			0xEF16
#define W25Q128			0xEF17
#define W25Q256 		0xEF18



uint16_t W25QXX_TYPE =	W25Q64;

/**
 * @brief  DeInitializes the peripherals used by the SPI FLASH driver.
 */
static void sFLASH_LowLevel_DeInit(void)
{
    GPIO_InitType GPIO_InitStructure;

    /*!< Disable the sFLASH_SPI  */
    SPI_Enable(sFLASH_SPI, DISABLE);

    /*!< DeInitializes the sFLASH_SPI */
    SPI_I2S_DeInit(sFLASH_SPI);

    /*!< sFLASH_SPI Periph clock disable */
    RCC_EnableAPB2PeriphClk(sFLASH_SPI_CLK, DISABLE);

    /*!< Configure sFLASH_SPI pins: SCK */
    GPIO_InitStructure.Pin       = sFLASH_SPI_SCK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitPeripheral(sFLASH_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

    /*!< Configure sFLASH_SPI pins: MISO */
    GPIO_InitStructure.Pin = sFLASH_SPI_MISO_PIN;
    GPIO_InitPeripheral(sFLASH_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

    /*!< Configure sFLASH_SPI pins: MOSI */
    GPIO_InitStructure.Pin = sFLASH_SPI_MOSI_PIN;
    GPIO_InitPeripheral(sFLASH_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

    /*!< Configure sFLASH_CS_PIN pin: sFLASH Card CS pin */
    GPIO_InitStructure.Pin = sFLASH_CS_PIN;
    GPIO_InitPeripheral(sFLASH_CS_GPIO_PORT, &GPIO_InitStructure);
}

/**
 * @brief  Initializes the peripherals used by the SPI FLASH driver.
 */
static void sFLASH_LowLevel_Init(void)
{
    GPIO_InitType GPIO_InitStructure;

    /*!< sFLASH_SPI_CS_GPIO, sFLASH_SPI_MOSI_GPIO, sFLASH_SPI_MISO_GPIO
         and sFLASH_SPI_SCK_GPIO Periph clock enable */
    RCC_EnableAPB2PeriphClk(
        sFLASH_CS_GPIO_CLK | sFLASH_SPI_MOSI_GPIO_CLK | sFLASH_SPI_MISO_GPIO_CLK | sFLASH_SPI_SCK_GPIO_CLK, ENABLE);

    /*!< sFLASH_SPI Periph clock enable */
    RCC_EnableAPB2PeriphClk(sFLASH_SPI_CLK, ENABLE);

    /*!< Configure sFLASH_SPI pins: SCK */
    GPIO_InitStructure.Pin        = sFLASH_SPI_SCK_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitPeripheral(sFLASH_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

    /*!< Configure sFLASH_SPI pins: MOSI */
    GPIO_InitStructure.Pin = sFLASH_SPI_MOSI_PIN;
    GPIO_InitPeripheral(sFLASH_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

    /*!< Configure sFLASH_SPI pins: MISO */
    GPIO_InitStructure.Pin       = sFLASH_SPI_MISO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitPeripheral(sFLASH_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

    /*!< Configure sFLASH_CS_PIN pin: sFLASH Card CS pin */
    GPIO_InitStructure.Pin       = sFLASH_CS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitPeripheral(sFLASH_CS_GPIO_PORT, &GPIO_InitStructure);
}

/**
 * @brief  DeInitializes the peripherals used by the SPI FLASH driver.
 */
void sFLASH_DeInit(void)
{
    sFLASH_LowLevel_DeInit();
}


void spi_flash_init(void)
{
	SPI_InitType SPI_InitStructure;

    sFLASH_LowLevel_Init();

    /*!< Deselect the FLASH: Chip Select high */
    SPIFLASH_CS_HIGH();

    /*!< SPI configuration */
    SPI_InitStructure.DataDirection = SPI_DIR_DOUBLELINE_FULLDUPLEX;
    SPI_InitStructure.SpiMode       = SPI_MODE_MASTER;
    SPI_InitStructure.DataLen       = SPI_DATA_SIZE_8BITS;
    SPI_InitStructure.CLKPOL        = SPI_CLKPOL_HIGH;
    SPI_InitStructure.CLKPHA        = SPI_CLKPHA_SECOND_EDGE;
    SPI_InitStructure.NSS           = SPI_NSS_SOFT;

    SPI_InitStructure.BaudRatePres = SPI_BR_PRESCALER_4;

    SPI_InitStructure.FirstBit = SPI_FB_MSB;
    SPI_InitStructure.CRCPoly  = 7;
    SPI_Init(sFLASH_SPI, &SPI_InitStructure);

    /*!< Enable the sFLASH_SPI  */
    SPI_Enable(sFLASH_SPI, ENABLE);
}


//sFLASH_SendByte
uint8_t flash_write_byte(uint8_t byte)
{
    /*!< Loop while DAT register in not emplty */
    while (SPI_I2S_GetStatus(sFLASH_SPI, SPI_I2S_TE_FLAG) == RESET)
        ;

    /*!< Send byte through the SPI1 peripheral */
    SPI_I2S_TransmitData(sFLASH_SPI, byte);

    /*!< Wait to receive a byte */
    while (SPI_I2S_GetStatus(sFLASH_SPI, SPI_I2S_RNE_FLAG) == RESET)
        ;

    /*!< Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(sFLASH_SPI);
}



//sFLASH_ReadByte
uint8_t flash_read_byte(void)
{
    return (flash_write_byte(0xA5));
}

void drv_spi_flash_init(void)
{
	spi_flash_init();
}



//从SPIFlash读或写数据
void drv_flash_read(uint8_t *pData, uint32_t Addr, uint16_t Length)
{
	SPIFLASH_CS_LOW();

	flash_write_byte(CMD_READ_DATA);

	if(W25QXX_TYPE == W25Q256)
	{
		flash_write_byte((uint8_t)(Addr>>24));
	}
	flash_write_byte((uint8_t)(Addr>>16));
	flash_write_byte((uint8_t)(Addr>>8));
	flash_write_byte((uint8_t)Addr);

	for(uint16_t i=0; i<Length; i++)
	{
		pData[i] = flash_read_byte();
	}

	SPIFLASH_CS_HIGH();
}





//FLASH 写使能
void drv_flash_write_enable(void)
{
	SPIFLASH_CS_LOW();

	flash_write_byte(CMD_WRITE_EN);

	SPIFLASH_CS_HIGH();
}



//等待FLASH空闲
void drv_flash_wait_for_idle(void)
{
	uint8_t byte = 0;

	SPIFLASH_CS_LOW();

	flash_write_byte(CMD_READ_REG);

	do{
		byte = flash_read_byte();
	}while((byte&0x01)==0x01);

	SPIFLASH_CS_HIGH();
}



//擦除整片FLASH
void drv_flash_erase_chip(void)
{
	drv_flash_write_enable();
//	drv_flash_wait_for_idle();
	SPIFLASH_CS_LOW();
	flash_write_byte(CMD_ERASE_CHIP);
	SPIFLASH_CS_HIGH();
	drv_flash_wait_for_idle();
}


//擦除扇区
void drv_flash_erase_sector(uint32_t Addr)
{
	drv_flash_write_enable();
	drv_flash_wait_for_idle();
	SPIFLASH_CS_LOW();
	flash_write_byte(CMD_ERASE_SECTOR);
	if(W25QXX_TYPE == W25Q256)
	{
		flash_write_byte((uint8_t)(Addr>>24));
	}
	flash_write_byte((uint8_t)(Addr>>16));
	flash_write_byte((uint8_t)(Addr>>8));
	flash_write_byte((uint8_t)Addr);
	SPIFLASH_CS_HIGH();
	drv_flash_wait_for_idle();
}


void drv_flash_write_page(uint8_t *pData, uint32_t Addr, uint16_t Length)
{
	drv_flash_write_enable();

	SPIFLASH_CS_LOW();
	flash_write_byte(CMD_PAGE_PROGRAM);
	if(W25QXX_TYPE == W25Q256)
	{
		flash_write_byte((uint8_t)(Addr>>24));
	}
	flash_write_byte((uint8_t)(Addr>>16));
	flash_write_byte((uint8_t)(Addr>>8));
	flash_write_byte((uint8_t)Addr);

	for(uint16_t i=0; i<Length; i++)
	{
		flash_write_byte(pData[i]);
	}

	SPIFLASH_CS_HIGH();
	drv_flash_wait_for_idle();
}


//写入长度不超过一页
void drv_flash_write_no_check(uint8_t *pData, uint32_t Addr, uint16_t Length)
{
	uint16_t pageremain;

	pageremain = SPI_FLASH_PAGE_SIZE - Addr % SPI_FLASH_PAGE_SIZE;  	//单页剩余字节数

	if(Length <= pageremain)			//不大于256
	{
		pageremain = Length;
	}
	while(1)
	{
		drv_flash_write_page(pData,Addr,pageremain);

		if(Length == pageremain)
		{
			break;						//写入结束
		}
	 	else
		{
			pData 	+= pageremain;
			Addr 	+= pageremain;
			Length  -= pageremain;

			if(Length > SPI_FLASH_PAGE_SIZE)
			{
				pageremain = SPI_FLASH_PAGE_SIZE; 		//一次最大写入256
			}
			else
			{
				pageremain = Length;
			}
		}
	};
}




//可按任意长度写入，可翻页
void drv_flash_write_buffer(uint8_t* pData, uint32_t Addr, uint16_t Length)
{
    uint8_t num_of_page = 0, num_of_single = 0, addr = 0, count = 0, temp = 0;

    addr          = Addr % SPI_FLASH_PAGE_SIZE;
    count         = SPI_FLASH_PAGE_SIZE - addr;
    num_of_page   = Length / SPI_FLASH_PAGE_SIZE;
    num_of_single = Length % SPI_FLASH_PAGE_SIZE;

     /* Addr is SPI_FLASH_PAGE_SIZE aligned  */
    if(0 == addr)
	{
        /* Length < SPI_FLASH_PAGE_SIZE */
        if(0 == num_of_page)
        {
            drv_flash_write_page(pData,Addr,Length);
        }
        /* Length > SPI_FLASH_PAGE_SIZE */
        else
		{
            while(num_of_page--)
			{
                drv_flash_write_page(pData,Addr,SPI_FLASH_PAGE_SIZE);
                Addr += SPI_FLASH_PAGE_SIZE;
                pData += SPI_FLASH_PAGE_SIZE;
            }
            drv_flash_write_page(pData,Addr,num_of_single);
		}
    }
	else
	{
        /* Addr is not SPI_FLASH_PAGE_SIZE aligned  */
        if(0 == num_of_page)
		{
            /* (Length + Addr) > SPI_FLASH_PAGE_SIZE */
            if(num_of_single > count)
			{
                temp = num_of_single - count;
                drv_flash_write_page(pData,Addr,count);
                Addr += count;
                pData += count;
                drv_flash_write_page(pData,Addr,temp);
            }
			else
			{
                drv_flash_write_page(pData,Addr,Length);
			}
        }
		else
		{
            /* Length > SPI_FLASH_PAGE_SIZE */
            Length -= count;
            num_of_page = Length / SPI_FLASH_PAGE_SIZE;
            num_of_single = Length % SPI_FLASH_PAGE_SIZE;

            drv_flash_write_page(pData,Addr, count);
            Addr += count;
            pData += count;

            while(num_of_page--)
			{
                drv_flash_write_page(pData,Addr,SPI_FLASH_PAGE_SIZE);
                Addr += SPI_FLASH_PAGE_SIZE;
                pData += SPI_FLASH_PAGE_SIZE;
            }

            if(0 != num_of_single)
            {
                drv_flash_write_page(pData,Addr,num_of_single);
            }
        }
    }
}




