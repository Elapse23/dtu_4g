#ifndef __BOOT_LOARDER_CONFIG_H__
#define __BOOT_LOARDER_CONFIG_H__



/************************************************************************************************
* ROM area config
************************************************************************************************/
//MCU vector, reset entry, bootloader are all included in bootloader area.
//the same defination of "BootloaderAsmFuncs.asm"
#define cdwMCU_SECTOR_START                     0x08000000
#define cdwMCU_SECTOR_END                       0x0807FFFF

#define cdwMCU_VECTOR_SECTOR_START              cdwMCU_SECTOR_START
#define cdwMCU_VECTOR_SECTOR_END                0x080001FF

#define cdwBOOTLOADER_SECTOR_START              cdwMCU_VECTOR_SECTOR_START
//#define cdwBOOTLOADER_ADDR_START                0x08000410
#define cdwBOOTLOADER_SECTOR_END                (0x08004FFF)

#define cdwAPP_SECTOR_START                     (cdwBOOTLOADER_SECTOR_END + 1)
#define cdwFIRMWARE_VER_STR_ADDR                cdwAPP_SECTOR_START
#define cdwAPP_SECTOR_END                       0x0807BFFF
#define cdwMCU_SECTOR_USER_INFO_START           (cdwAPP_SECTOR_END + 1)
#define cdwMCU_SECTOR_USER_INFO_END             cdwMCU_SECTOR_END
#define cbAppMaskLen                            4
#define cdwAPPMark                              0x88888889

/************************************************************************************************
* RAM area config
************************************************************************************************/
#define cdwRAM_START_ADDR	                    (0x20000000)  // User RAM Address START
#define cdwRAM_BOOT_REVERSE_ADDR                (0x20000200)  // 0x1FFFF000~0x1FFFF0FF for boot reverse ram
#define cdwRAM_COPY_CODE_MAX_ADDR               (0x20003900)   //
#define cdwRAM_END_ADDR		                    (0x20023FFF)  // User RAM Address END

/************************************************************************************************
* ROM copy config
************************************************************************************************/
//copy 0x8000200 - 0x80037ff
#define COPY_CODE_ROM_START_ADDR               (0x8000200)
#define COPY_CODE_ROM_END_ADDR                 (0x80037FF)
#define COPY_CODE_ROM_MAX_LEN                  (COPY_CODE_ROM_END_ADDR - COPY_CODE_ROM_START_ADDR + 1)
#define COPY_CODE_ROM_MAX_ADDR                 (0x8003800)   //

//copy 0x8004fe00 - 0x8004fff
#define COPY_DATA_ROM_START_ADDR               (cdwBOOTLOADER_SECTOR_END - 0x100 + 1)
#define COPY_DATA_ROM_END_ADDR                 (cdwBOOTLOADER_SECTOR_END)
#define COPY_DATA_ROM_MAX_LEN                  (COPY_DATA_ROM_END_ADDR - COPY_DATA_ROM_START_ADDR + 1)





#define HAL_BOOT_RUN_FLASH     1//flash only
#define HAL_BOOT_RUN_RAM       2//ram only
#define BOOT_RUN_MODE          HAL_BOOT_RUN_RAM


/************************************************************************************************
* debug lev config
************************************************************************************************/
#define LOG_LEV_NONE    0 //CLOSE
#define LOG_LEV_RAM     10
#define LOG_LEV_ERROR   20
#define LOG_LEV_WARNING 30
#define LOG_LEV_INFO    40
#define LOG_LEV_DEBUG   50

#define LOG_LEV_VAL 0//LOG_LEV_DEBUG

#define LOG_MAX_LEN                 300
#define LOG_UART                    UART4
#define LOG_UART_RATE               115200
#define LOG_STR                     STREAM_CAL_USE
#define LOG_RAM_OFFSET_EN           1//1run ram 0run flash


/************************************************************************************************
* iwdg switch
************************************************************************************************/
#define IWDG_SWITCH_ENABLE              0


/************************************************************************************************
* GPIO RCC
************************************************************************************************/

#define RCC_GPIO_OPEN_ALL()   RCC->APB2PCLKEN |= (RCC_APB2_PERIPH_GPIOA |RCC_APB2_PERIPH_GPIOB |RCC_APB2_PERIPH_GPIOC)

/************************************************************************************************
* hal led
************************************************************************************************/

#define BOOT_LED_ENABLE                 1
#if(1 == BOOT_LED_ENABLE)
#define LED_NET_GREEN_GPIO              GPIOC,6
#define LED_NET_RED_GPIO                GPIOC,7
#define LED_COM_GREEN_GPIO              GPIOC,8
#define LED_COM_RED_GPIO                GPIOC,9
#define LED_RCC_OPEN()                  APB_RCC_OPEN(APB2,GPIOC)
#endif


#define SIMULATION   0xff//lev simulation

/************************************************************************************************
* UART CONFIG wait to rebuild
************************************************************************************************/
#define HalBootUSART5RS485EN            true
#define cbUART0Func                     cbICPTransFunc
#define cbUART1Func                     cbICPTransFunc
#define cbUART2Func                     cbNULLFunc

#define cbUSART2Func                    cbICPTransFunc
#define cbUSART3Func                    cbICPTransFunc
#define cbUART4Func                     cbICPTransFunc
#define cbUART5Func                     cbICPFunc


/************************************************************************************************
* I2C Function config
************************************************************************************************/
#define HalBootI2CFuncEN               false
#define I2CCH1  1
#define I2C_CH     SIMULATION//I2C1
#define I2C1_REMAP  0
#if( SIMULATION == I2C_CH)
#define I2C_SCL   GPIOB,8
#define I2C_SDA   GPIOB,9
#define I2C_WP    GPIOC,2
#endif

#define cwBOOT_MAX_ADDRESS_ADDR                 0x07FF  //0x7FFF
#define cbBOOT_I2C_DEVICE0_ADDR                 0xA0    //Device 0.0b10100000
#define cbBOOT_I2C_DEVICE_READ                  0x01    //Read data from eeprom.0b00000001
#define cbBOOT_I2C_DEVICE_WRITE                 0xFE    //Write data to eeprom.0b11111110


/************************************************************************************************
* spi flash cofig
************************************************************************************************/
#define HalBootSpiFlashFuncEN    true
//spi sel
#define EX_FLASH_SPI_CH         SPI1
#define SPI_REMAP               0
#define EX_FLASH_TYPE           W25Q64 //chip

#define cdwLocalUpdateFirmDataLen               600

#define UPDATA_FROM_RS485_PAGE_LEN              256//no totale pack need to cal

#define cbMaxPackageData                        (cdwLocalUpdateFirmDataLen)
#define cbMaxFirmwareData                       (cdwLocalUpdateFirmDataLen - 4)
//#define FLASH_CODE_SAVE_PAGE_MAX                (cbMaxFirmwareData + 8)




#endif
