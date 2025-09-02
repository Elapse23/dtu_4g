/************************************************************************************************
File Name:      Bootloader.H
Description:    bootloader operation code
Version:        V1.0
Author:         chenweijie
Date:           2018-06-19
*************************************************************************************************/

#ifndef __BOOT_LOADER_H__
#define __BOOT_LOADER_H__


#include "n32g45x.h"
#include "IcpHead.H"
#include "BootloaderHead.H"
#include "BootloaderConfig.h"

#include "drv_log.h"




/************************************************************************************************
* ROM copy RAM area
************************************************************************************************/

#define COPY_CODE_RAM_START_ADDR               (cdwRAM_BOOT_REVERSE_ADDR)
#define COPY_CODE_RAM_END_ADDR                 (COPY_CODE_RAM_START_ADDR + COPY_CODE_ROM_MAX_LEN)
//#define COPY_CODE_RAM_END_ADDR_TEST            (COPY_CODE_RAM_START_ADDR + COPY_CODE_ROM_MAX_LEN + (COPY_CODE_ROM_MAX_LEN%4))

#define COPY_DATA_RAM_START_ADDR               (COPY_CODE_RAM_END_ADDR)
#define COPY_DATA_RAM_END_ADDR                 (COPY_CODE_RAM_END_ADDR   + 0x200)
//#define COPY_DATA_RAM_END_ADDR_TEST            (COPY_DATA_RAM_START_ADDR + COPY_DATA_ROM_MAX_LEN + (COPY_DATA_ROM_MAX_LEN%4))

/************************************************************************************************
* ROM copy RAM OFFSET
************************************************************************************************/

#if(HAL_BOOT_RUN_FLASH  == BOOT_RUN_MODE)
#define COPY_DATA_RAM_OFFSET 0
#define COPY_CODE_RAM_OFFSET 0

#else
#define COPY_CODE_RAM_OFFSET (COPY_CODE_RAM_START_ADDR - COPY_CODE_ROM_START_ADDR)
#define COPY_DATA_RAM_OFFSET (COPY_DATA_RAM_START_ADDR - COPY_DATA_ROM_START_ADDR)

#endif

#define BootRodata(pAddr)                       \
    ((((INT32U)pAddr)<= COPY_DATA_RAM_START_ADDR)?(((INT32U)pAddr)+dwBootRodataAddrOffset):(INT32U)(pAddr))

#define BootFlashFunc(pFunc)                    \
    ((((INT32U)pFunc)<= COPY_CODE_RAM_START_ADDR)?(((INT32U)pFunc)+dwBootFlashCodeAddrOffset):(INT32U)(pFunc))
#define BootUARTFunc(pFunc)                     \
    ((((INT32U)pFunc)<= COPY_CODE_RAM_START_ADDR)?(((INT32U)pFunc)+dwBootUARTCodeAddrOffset):(INT32U)(pFunc))
#define BootCANFunc(pFunc)                     \
    ((((INT32U)pFunc)<= COPY_CODE_RAM_START_ADDR)?(((INT32U)pFunc)+dwBootCANCodeAddrOffset):(INT32U)(pFunc))
#define BootSpiFlashFunc(pFunc)                 \
    ((((INT32U)pFunc)<= COPY_CODE_RAM_START_ADDR)?(((INT32U)pFunc)+dwBootSpiFlashCodeAddrOffset):(INT32U)(pFunc))


#define mDelayNop()                             \
    asm("NOP\n"                                 \
        "NOP\n"                                 \
        "NOP\n"                                 \
        "NOP\n"                                 \
        "NOP\n"                                 \
        "NOP\n")

//#define __DSB()                                 asm("DSB")

#define M_RESET_MCU()                           NVIC_SystemReset()

#define HalBootIntDisable()		                asm("CPSID I")
#define HalBootIntEnable()                      asm("CPSIE I")

//#define M_SYSTICK_INIT()                        \
//    S32_SysTick->CVR = 0x0;	/* clear current timer value */\
//    S32_SysTick->RVR = 40000;                   \
//    S32_SysTick->CSR = 0x07

#define cbSYS_TICK_CORE_BUS                 SysTick_CTRL_CLKSOURCE_Msk
#define cbSYS_TICK_EXTERNAL_BUS             0
#define cbSYS_TICK_INT_EN_MASK              SysTick_CTRL_TICKINT_Msk
#define cbSYS_TICK_START_MASK               SysTick_CTRL_ENABLE_Msk

#define cdwSYS_TICK_100US                   100
#define cdwSYS_TICK_250US                   250
#define cdwSYS_TICK_500US                   500
#define cdwSYS_TICK_TIME(bTime)             ((bTime) * (cdwSYSCLK_FREQ / 1000000) - 1)    //bTime unit:us.bTime(us)*SYSCLK_FREQ(MHz)=cnt
#ifndef cdwSYSCLK_FREQ
    #define cdwSYSCLK_FREQ      144000000//144000000
    #define cdwAHB_FREQ         cdwSYSCLK_FREQ
    #define cdwAPB2_FREQ        (INT32U)(cdwSYSCLK_FREQ / 2)
    #define cdwAPB1_FREQ        (INT32U)(cdwSYSCLK_FREQ / 4)
    #define cdwTIMER2_7_FREQ    (INT32U)(cdwAPB1_FREQ * 2)
#endif
#define M_SYSTICK_INIT()                                                                       \
        SysTick->VAL = 0x0;                                                                    \
        SysTick->LOAD = cdwSYS_TICK_TIME(cdwSYS_TICK_250US);                                   \
        SysTick->CTRL = cbSYS_TICK_CORE_BUS + cbSYS_TICK_INT_EN_MASK + cbSYS_TICK_START_MASK;
#define M_CHK_SYSTICK_FLAG()                    \
    ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0)



//disable watch dog
//#define M_BOOT_DISABLE_WDOG()                   \
//    WDOG->CNT = 0xD928C520;/* Unlock */         \
//    WDOG->TOVAL = 0x0000FFFF;/* Max timeout value */\
//    WDOG->CS = 0x00002100/*Disable Wdog*/
//#define M_WDOG_UNLOCK()                         WDOG->CNT = 0x20C5; WDOG->CNT = 0x28D9


#if (1 == IWDG_SWITCH_ENABLE)
#include "n32g45x_iwdg.h"
#define KEY_ReloadKey ((uint16_t)0xAAAA)
#define KEY_EnableKey ((uint16_t)0xCCCC)
//#define IWDG_WRITE_ENABLE     ((uint16_t)0x5555)
//#define IWDG_WRITE_DISABLE    ((uint16_t)0x0000)

#define MACRO_IWDG_RELOD()      IWDG->KEY = KEY_ReloadKey
//31-32s
#define MACRO_IWDG_OPEN() do{   IWDG->KEY     = IWDG_WRITE_ENABLE; \
                                IWDG->PREDIV  = IWDG_PRESCALER_DIV256; \
                                IWDG->RELV    = 0x0FFF;\
                                IWDG->KEY     = KEY_EnableKey;}while(0)

#define MACRO_IWDG_CLOSE()      IWDG->KEY     = IWDG_WRITE_DISABLE


#else


#define MACRO_IWDG_RELOD()
#define MACRO_IWDG_OPEN()
#define MACRO_IWDG_CLOSE()

#endif


//N32G

//寄存器位置位
#define __REG_SET_BITS(REG,VALUE)         (REG  |= (uint32_t)VALUE)
#define _REG_SET_BITS(REG,SIZE)           __REG_SET_BITS(REG,((uint32_t)0x01 << SIZE))
#define REG_SET_BITS(SREG)                _REG_SET_BITS(SREG)

//寄存器组位清零
#define __REG_CLR_BITS(REG,VALUE)         (REG &= (uint32_t)~VALUE)
#define _REG_CLR_BITS(REG,SIZE)           __REG_CLR_BITS(REG,((uint32_t)0x01 << SIZE))
#define REG_CLR_BITS(SREG)                _REG_CLR_BITS(SREG)

//寄存器组位获取
#define __REG_GET_BITS(REG,VALUE)         (REG &  VALUE)
#define _REG_GET_BITS(REG,SIZE)           __REG_GET_BITS(REG,((uint32_t)0x01 << SIZE))
#define REG_GET_BITS(SREG)                _REG_GET_BITS(SREG)

//#define REG_SET_VALUE(REG,SIZE,MASK,VALUE)  do{ (REG)&= (~(((uint32_t)MASK) << (SIZE)));\
//                                                (REG)|= (((uint32_t)VALUE) << (SIZE));\
//                                              }while(0)
#define REG_SET_VALUE(REG,SIZE,MASK,VALUE)  (REG) = ((REG) & (~(MASK << (SIZE)))|((uint32_t)VALUE << (SIZE)))
#define REG_RESET_MASK(REG,SIZ,MASK)        REG_SET_VALUE(REG,MASK ,SIZE,0)

#define REG_SET_MASK(SREG)                 _REG_SET(SREG)

//RCC
#define RCC_APB2_PERIPH_AFIO   ((uint32_t)0x00000001)

#define APB_RCC_OPEN(APBS,DEV)    RCC->APBS##PCLKEN |= RCC_##APBS##_PERIPH_##DEV
#define APB_RCC_CLOSE(APBS,DEV)   RCC->APBS##PCLKEN &= ~RCC_##APBS##_PERIPH_##DEV


///#define GPIOD_CLK_OPEN()  _APBCLKEN(APB2,GPIOD)//RCC->APB2PCLKEN |=    RCC_APB2_PERIPH_GPIOB
//#define GPIOB_CLK_OPEN()  _APBCLKEN(APB2,GPIOB)//RCC->APB2PCLKEN |=    RCC_APB2_PERIPH_GPIOB
//#define GPIOC_CLK_OPEN()  _APBCLKEN(APB2,GPIOC)//RCC->APB2PCLKEN |=    RCC_APB2_PERIPH_GPIOC

#define GPIO_RCC_OPEN(DEV)  RCC->APBS2PCLKEN |= RCC_APB2_PERIPH_##DEV


//#define UART4_CLK_OPEN()  _APBCLKEN(APB1,UART4)//RCC->APB1PCLKEN |=    RCC_APB1_PERIPH_UART4
//#define UART5_CLK_OPEN()  _APBCLKEN(APB1,UART5)//RCC->APB1PCLKEN |=    RCC_APB1_PERIPH_UART4
#define UART_RCC_OPEN(DEV)  RCC->APBS1PCLKEN |= RCC_APB1_PERIPH_##DEV //UART2-5


//GPIO
#define GPIO_OUT_SPEED_50M 0x03//
#define GPIO_OUT_SPEED  GPIO_OUT_SPEED_50M

typedef enum{
    OUTPUT_PUSH_PULL      = (0x00|GPIO_OUT_SPEED),//GPIO_Mode_Out_PP //普通推挽
    OUTPUT_OPEN_DRAIN     = (0x04|GPIO_OUT_SPEED),//GPIO_Mode_Out_OD //普通开漏
    AF_MODE_PUSH_PULL     = (0x08|GPIO_OUT_SPEED),//GPIO_Mode_AF_PP  //功能推挽
    AF_MODE_OPEN_DRAIN    = (0x0B|GPIO_OUT_SPEED),//GPIO_Mode_AF_OD  //功能开漏
    INPUT_NULL            = 0x00,                 //GPIO_Mode_AIN    //模拟输入
    INPUT_FLOAT           = 0x04,                 //GPIO_Mode_IN_FLOATING
    INPUT_PULL_UP         = 0x08                  //GPIO_Mode_IPU    //上拉输入 GPIO_Mode_IPD
}E_GPIOS_MODE;

#define M_PORT_SET_PIN_HIGH(pGPIO, pin)         ((pGPIO)->PBSC  |= ((INT32U)1 << (pin)))
#define M_PORT_SET_PIN_LOW(pGPIO, pin)          ((pGPIO)->PBC   |= ((INT32U)1 << (pin)))

#define M_PORT_SET_PIN_TOGGLE(pGPIO, pin)       ((pGPIO)->POD   ^= ((INT32U)1 << (pin)))
#define M_PORT_PIN_READ(pGPIO, pin)             (((pGPIO)->PID) &  ((INT32U)1 << (pin)))

#define M_PORT_PIN_IS_SET(pGPIO, pin)           ((((pGPIO)->PID) &  ((INT32U)1 << (pin))) == ((INT32U)1 << (pin)))

#define HalPortSetPinToggle(pGPIO, pin)         M_PORT_SET_PIN_TOGGLE(pGPIO, pin)
#define HalPortPinRead(pGPIO, pin)              M_PORT_PIN_READ(pGPIO, pin)


#define HAL_GPIO_SET_HIGHT(GPIOS)               M_PORT_SET_PIN_HIGH(GPIOS)
#define HAL_GPIO_SET_LOW(GPIOS)                 M_PORT_SET_PIN_LOW(GPIOS)
#define HAL_GPIO_TOGGLE(GPIOS)                  M_PORT_SET_PIN_TOGGLE(GPIOS)
#define HAL_GPIO_READ(GPIOS)                    M_PORT_PIN_READ(GPIOS)
#define HAL_GPIO_IS_SET(GPIOS)                  M_PORT_PIN_IS_SET(GPIOS)



#define GPIO_MODE_MASK (uint32_t)0x0F
#define _HAL_GPIO_SET_MODE_L(port,pin,mode)     REG_SET_VALUE(port->PL_CFG,((pin - 0) << 2),GPIO_MODE_MASK,mode)
#define _HAL_GPIO_SET_MODE_H(port,pin,mode)     REG_SET_VALUE(port->PH_CFG,((pin - 8) << 2),GPIO_MODE_MASK,mode)

//void gpio_set_mode(GPIO_Module* port,uint8_t pin,E_GPIOS_MODE mode);
//#define _HAL_GPIO_SET_MODE(port,pin,mode)       gpio_set_mode(port,pin,mode)

//warning?
//#define _HAL_GPIO_SET_MODE(port,pin,mode) do{   if (8 <= (pin)){\
//                                                    _HAL_GPIO_SET_MODE_H(port,pin,mode);\
//                                                }else{\
//                                                    _HAL_GPIO_SET_MODE_L(port,pin,mode);\
//                                                } }while(0)\

#define _HAL_GPIO_SET_MODE(port,pin,mode)      REG_SET_VALUE(*((&port->PL_CFG) + ((pin>7)?1:0)),(((pin>7)?(pin - 8):pin) << 2),GPIO_MODE_MASK,mode)


//#define HAL_GPIO_SET_MODE_H(GPIOS,mode)         _HAL_GPIO_SET_MODE_H(GPIOS,mode)
//#define HAL_GPIO_SET_MODE_L(GPIOS,mode)         _HAL_GPIO_SET_MODE_L(GPIOS,mode)

#define HAL_GPIO_SET_MODE(GPIOS,mode)           _HAL_GPIO_SET_MODE(GPIOS,mode)


#define GPIO_JTAG_MASK 0X07
#define GPIO_JTAG_SIZE 24
#define HAL_JTAG_REMAP(value)                   REG_SET_VALUE(AFIO->RMP_CFG,GPIO_JTAG_SIZE,GPIO_JTAG_MASK,value)
#define JTAG_SW_REMAP()                         HAL_JTAG_REMAP(0x02)


/******************************************************************************
* CAN
******************************************************************************/
#define M_BOOT_CAN_MCR_R(pCAN)		(pCAN->MCR)

#define M_BOOT_CAN_MCR(pCAN,dwData)          \
    (pCAN->MCR = dwData)

#define M_BOOT_CAN_MCR_OR(pCAN,dwData)          \
    (pCAN->MCR |= dwData)

#define M_BOOT_CAN_MCR_AND(pCAN,dwData)          \
    (pCAN->MCR &= dwData)

#define M_BOOT_CAN_CTRL1(pCAN,dwData)          \
    (pCAN->CTRL1 = dwData)

#define M_BOOT_CAN_CTRL1_AND(pCAN,dwData)          \
    (pCAN->CTRL1 &= dwData)

#define M_BOOT_CAN_IMASK1(pCAN,dwData)          \
    (pCAN->IMASK1 |= dwData)

#define M_BOOT_CAN_RXIMR(pCAN,bNum,dwData)          \
    (pCAN->RXIMR[bNum] = dwData)

#define M_BOOT_CAN_RAMn(pCAN,bNum,dwData)          \
    (pCAN->RAMn[bNum] = dwData)

/******************************************************************************
*UART baud rate
  Baud rate = SCI clock source / (BR)
  UART clock source = bus clock / 16
******************************************************************************/
#define cRxByteDelay_500us(BaudRate)\
    ((INT8U)(((INT8U)((24000.0 / BaudRate) + 0.5 + 1)) * 2))    //3Bytes bus free + 1ms

#define cdwBOOT_INTERNAL_BUS                    40000000

#define cwBOOT_BAUD_RATE_1200                   1200//(cdwBOOT_INTERNAL_BUS / 16 / 1200)
#define cwBOOT_BAUD_RATE_2400                   2400//(cdwBOOT_INTERNAL_BUS / 16 / 2400)
#define cwBOOT_BAUD_RATE_4800                   4800//(cdwBOOT_INTERNAL_BUS / 16 / 4800)
#define cwBOOT_BAUD_RATE_9600                   9600//(cdwBOOT_INTERNAL_BUS / 16 / 9600)
#define cwBOOT_BAUD_RATE_19200                  19200//(cdwBOOT_INTERNAL_BUS / 16 / 19200)
#define cwBOOT_BAUD_RATE_38400                  38400//(cdwBOOT_INTERNAL_BUS / 16 / 38400)
#define cwBOOT_BAUD_RATE_115200                 115200//(cdwBOOT_INTERNAL_BUS / 16 / 115200)
#define cwBOOT_BAUD_RATE_750000                 750000

#define cbSetBandRate1200  	                0
#define cbSetBandRate2400  	                1
#define cbSetBandRate4800  	                2
#define cbSetBandRate9600  	                3
#define cbSetBandRate19200  	                4
#define cbSetBandRate38400  	                5
#define cbSetBandRate115200  	                6
#define cbSetBandRate750000                     7
#define cbSetBandRateMax                        8

#define cbSetNoCheck  		                0
#define cbSetOddCheck  		                1
#define cbSetEvenCheck  		        2
#define cbSetMaxOddEvenCheck  		        3

/******************************************************************************
*CAN baud rate
******************************************************************************/
#define cdwBOOT_CANBR_BUS                       8000000

#define cwBOOT_CAN_BAUD_500K                    (cdwBOOT_CANBR_BUS / 16 / 500000 - 1)
#define cwBOOT_CAN_BAUD_250K                    (cdwBOOT_CANBR_BUS / 16 / 250000 - 1)
#define cwBOOT_CAN_BAUD_125K                    (cdwBOOT_CANBR_BUS / 16 / 125000 - 1)
#define cwBOOT_CAN_BAUD_100K                    (cdwBOOT_CANBR_BUS / 16 / 100000 - 1)
#define cwBOOT_CAN_BAUD_50K                     (cdwBOOT_CANBR_BUS / 16 / 50000 - 1)

#define cbSetCANBaud500K                        0
#define cbSetCANBaud250K                        1
#define cbSetCANBaud125K                        2
#define cbSetCANBaud100K                        3
#define cbSetCANBaud50K                         4
#define cbSetCANBaudRateMax                     5

//index of dwBootExtTbl[]
#define cbFirmwareTypeTbl                       1
#define cbBootFirmwareTbl                       2
#define cbBootLoaderStart                       3
#define cbBootIcpCmdAnalysis                    4
#define cbBootIcpSubsetCmdAnalysis              5
#define cbBootTransparent                       6
#define cbBootResponse                          7
//#define cbBootCANReceive                        8
//#define cbBootUARTToCANSend                     9
//#define cbBootCANSendSec                        10
#define cbBootDeviceTreeInit                    8
#define cbMaxExtTable                           12



#define cbNULLFunc                              0
#define cbICPFunc                               1       //only update it's own
#define cbTransFunc                             2       //only transmit
#define cbICPTransFunc                          3       //update it's own or transmit


#define cbBootReverseDataLen                            4

#define cdwBOOT_RODATA_END_ADDR                         (cdwBOOTLOADER_SECTOR_END - 2 + 1)//0x4FFE
#define cdwBOOT_FIRMWARE_ADDR                           (cdwBOOT_RODATA_END_ADDR - 2)//0x4FFC
#define cdwFIRMWARE_TYPE_ADDR                           (cdwBOOT_FIRMWARE_ADDR - 4)//0x4FF8
#define cdwFWAPP_START_ADDR                             (cdwFIRMWARE_TYPE_ADDR - 4)//0x4FF4
#define cdwFWBOOT_END_ADDR                              (cdwFWAPP_START_ADDR - 4)//0x4FF0
#define cdwFWBOOT_START_ADDR                            (cdwFWBOOT_END_ADDR - 4)//0x4FEC

#define cdwCRC_TABLE_ADDR                               (cdwFWBOOT_START_ADDR - 16 * 2)//0x4FCC
#define cdwBOOT_RXDELAY_500US_ADDR                      (cdwCRC_TABLE_ADDR - cbSetBandRateMax * 2)//0x4FBE
#define cdwBOOT_RESERVE_ADDR                            (cdwBOOT_RXDELAY_500US_ADDR - 4)//0x4FBC  4 byte alignment
#define cdwBOOT_BAUD_TAB                                (cdwBOOT_RESERVE_ADDR - cbSetBandRateMax * 4)//0X4FA0
#define cdwBOOT_RODATA_START_ADDR                       (cdwBOOT_BAUD_TAB - cbBootReverseDataLen)//0X4F9C
#define cwBOOT_EXTTBL_ADDR                              (cdwBOOT_RODATA_START_ADDR - cbMaxExtTable * 4)//0X4F6C

#define boot_n32g45x_rcc_rodata_end                     cwBOOT_EXTTBL_ADDR
#define cboot_n32g45x_rcc_rodata_len                    (4 * 16)
#define boot_n32g45x_rcc_rodata_start                   (boot_n32g45x_rcc_rodata_end - cboot_n32g45x_rcc_rodata_len)

#define boot_printf_data_rom_end                        boot_n32g45x_rcc_rodata_start
#define boot_printf_data_rom_len                        (36 *2)
#define boot_printf_data_rom_start                      (boot_printf_data_rom_end - boot_printf_data_rom_len)

//#define boot_md5_rodata_end                             boot_n32g45x_rcc_rodata_start
//#define boot_md5_rodata_len                             64
//#define boot_md5_rodata_start                           (boot_md5_rodata_end - boot_md5_rodata_len)



/******************************************************************************
* Others
******************************************************************************/
#define M_BOOT_PORT_SET_PIN_MUX(pPORT,pin,mux)  \
    pPORT->PCR[pin] |=  PORT_PCR_MUX(mux)

#define M_BOOT_CLOCK_ENABLE(pPCC_x_INDEX)       \
    PCC->PCCn[pPCC_x_INDEX] |= PCC_PCCn_CGC_MASK

#define M_BOOT_PORT_SET_PIN_OUT(pPORT,pin)      \
    pPORT->PDDR |= GPIO_PDDR_PDD(1<<pin)

#define M_BOOT_PORT_SET_PIN_IN(pPORT,pin)       \
    pPORT->PDDR &= (~GPIO_PDDR_PDD(1<<pin))

#define mBootDelayNOP()                         asm("NOP")



#define cbUpdateFromNull                        0//null but uart
#define cbUpdateFromICP                         1//uart
#define cbUpdateFromFlash                       2//exflash
#define cbUpdateFinish                          3//finish to app
#define cbUpdateFromapp                         4//on app
//Spi flash
#define cwBootMaxFlashData                      300

#define cbBootReadIntactFlag                    0
#define cbBootReadPackHead                      1
#define cbBootReadPackData                      2

#define cbBootOperateOK                         0
#define cbBootOperateOverRange                  1
#define cbBootWriteDisable                      2
#define cbBootFlashBUSY                         3
#define cbBootOperateContinue                   4
#define cbBootReadFail                          5

#define cdwBootMaxFlashSize                     0x0097000//0x00800000

//spi flash registers
#define cbBoot_CMD_WriteStatus                  0x01
#define cbBoot_CMD_PageProg                     0x02
#define cbBoot_CMD_ReadData                     0x03
#define cbBoot_CMD_Status_1                     0x05
#define cbBoot_CMD_WriteEN                      0x06
#define cbBoot_CMD_SecErase                     0x20
#define cbBoot_CMD_Status_2                     0x35
#define cbBoot_CMD_UUID                         0x4B
/************************************************************************************************
* Local Firmware Update
************************************************************************************************/
#define cdwLocalUpdateFirmAddrStart             cdw_spi_flash_addr_alloc_start
#define cdwLocalUpdateFirmAddrEnd               cdw_spi_flash_addr_alloc_local_firmware_data_end
#define cdwLocalUpdateFirmInfoLen               cdw_spi_flash_addr_alloc_firmware_info_len


/*********************************************************
* RAM variable definition
*********************************************************/
#define cwMaxUARTCodeLen                        0x2000
#define cwMaxCANCodeLen                         0x900
#define cwMaxFlashCodeLen                       0x900
#define cwMaxSpiFlashCodeLen                    0x800
#define cwMaxRodataLen                          0x100//3D00-200 3B00

//address defination
//address must be aligned
#define cdwRAM_BOOTCODEADDR1_OFFSET_ADDR        (cdwRAM_START_ADDR)//0x20000000
#define cdwRAM_BOOTCODEADDR2_OFFSET_ADDR        (cdwRAM_BOOTCODEADDR1_OFFSET_ADDR+4)//0x20000004
#define cdwRAM_BOOTCODEADDR3_OFFSET_ADDR        (cdwRAM_BOOTCODEADDR2_OFFSET_ADDR+4)//0x20000008
#define cdwRAM_BOOTCODEADDR4_OFFSET_ADDR        (cdwRAM_BOOTCODEADDR3_OFFSET_ADDR+4)//0x2000000C
#define cdwRAM_BOOTCODEADDR5_OFFSET_ADDR        (cdwRAM_BOOTCODEADDR4_OFFSET_ADDR+4)//0x20000010
#define cdwRAM_UARTUPDATEINFO_ADDR              (cdwRAM_BOOTCODEADDR5_OFFSET_ADDR+4)//0x20000014
#define cdwRAM_UARTTRANSINFO_ADDR               (cdwRAM_UARTUPDATEINFO_ADDR+sizeof(UpdateUARTInfo))//0x20000024
#define cdwRAM_UPDATE_SPIFLASHINFO_ADDR         (cdwRAM_UARTTRANSINFO_ADDR+sizeof(UARTTransInfo))//0x200000A0
#define cdwRAM_UPDATEPACKINFO_ADDR              (cdwRAM_UPDATE_SPIFLASHINFO_ADDR+sizeof(UpdateSPIFlashInfo))//0x200000A4
#define cdwRAM_DEVTREEINFO_ADDR                 (cdwRAM_UPDATEPACKINFO_ADDR+sizeof(UpdatePackInfo))//0x200000DE
#define cdwRAM_BOOT_REVERSE_END_ADDR            (cdwRAM_DEVTREEINFO_ADDR)

#define cdwRAM_F_IIC_ACKERR_ADDR                (COPY_DATA_RAM_END_ADDR + 1)
//#define cdwRAM_RESREVER_ALIGNMENT_ADDR          (cdwRAM_F_IIC_ACKERR_ADDR+2)//alignment 4
#define cdwRAM_APPMASK_BUFF_ADDR                (cdwRAM_F_IIC_ACKERR_ADDR   + 2)

#define cdwRAM_BOOT_UARTCONFIG_ADDR             (cdwRAM_APPMASK_BUFF_ADDR+cbAppMaskLen)/* 1FFFBD02+4=1FFFBD06 */
#define cdwRAM_ICPTxBUF_ADDR                    (cdwRAM_BOOT_UARTCONFIG_ADDR+sizeof(BootUARTConfig))/* 1FFFBD06+0x10=1FFFBD16 */
#define cdwRAM_ICPRxBUF_ADDR                    (cdwRAM_ICPTxBUF_ADDR+(cbMaxPackageData+8))/* 1FFFBD16+0x134=0x1FFFBE4A */

#define cdwRAM_TRANS_TXBUF_ADDR                 (cdwRAM_ICPRxBUF_ADDR+(cbMaxPackageData+8))/* 0x1FFFBE4A+0x134=0x1FFFBF7E */
#define cdwRAM_TRANS_RXBUF_ADDR                 (cdwRAM_TRANS_TXBUF_ADDR+(cbMaxPackageData+8))/* 0x1FFFBF7E+0x134=0x1FFFC0B2 */
//#define cdwRAM_UART0_TXBUFLEN_ADDR              (cdwRAM_TRANS_RXBUF_ADDR+(cbMaxPackageData+8))/* 0x1FFFC0B2+0x134=0x1FFFC1E6 */
//#define cdwRAM_UART1_TXBUFLEN_ADDR              (cdwRAM_UART0_TXBUFLEN_ADDR+2)/* 0x1FFFC1E6+2=0x1FFFC1E8 */
//#define cdwRAM_UART2_TXBUFLEN_ADDR              (cdwRAM_UART1_TXBUFLEN_ADDR+2)/* 0x1FFFC1E8+2=0x1FFFC1EA */
//#define cdwRAM_UART0_RXBUFLEN_ADDR              (cdwRAM_UART2_TXBUFLEN_ADDR+2)/* 0x1FFFC1EA+2=0x1FFFC1EC */
//#define cdwRAM_UART1_RXBUFLEN_ADDR              (cdwRAM_UART0_RXBUFLEN_ADDR+2)/* 0x1FFFC1EC+2=0x1FFFC1EE */
//#define cdwRAM_UART2_RXBUFLEN_ADDR              (cdwRAM_UART1_RXBUFLEN_ADDR+2)/* 0x1FFFC1EE+2=0x1FFFC1F0 */
//#define cdwRAM_UART0_TXCNT_ADDR                 (cdwRAM_UART2_RXBUFLEN_ADDR+2)/* 0x1FFFC1F0+2=0x1FFFC1F2 */
//#define cdwRAM_UART1_TXCNT_ADDR                 (cdwRAM_UART0_TXCNT_ADDR+2)/* 0x1FFFC1F2+2=0x1FFFC1F4 */
//#define cdwRAM_UART2_TXCNT_ADDR                 (cdwRAM_UART1_TXCNT_ADDR+2)/* 0x1FFFC1F4+2=0x1FFFC1F6 */
//#define cdwRAM_UART0_RXCNT_ADDR                 (cdwRAM_UART2_TXCNT_ADDR+2)/* 0x1FFFC1F6+2=0x1FFFC1F8 */
//#define cdwRAM_UART1_RXCNT_ADDR                 (cdwRAM_UART0_RXCNT_ADDR+2)/* 0x1FFFC1F8+2=0x1FFFC1FA */
//#define cdwRAM_UART2_RXCNT_ADDR                 (cdwRAM_UART1_RXCNT_ADDR+2)/* 0x1FFFC1FA+2=0x1FFFC1FC */

#define cdwRAM_USART2_TXBUFLEN_ADDR              (cdwRAM_TRANS_RXBUF_ADDR+(cbMaxPackageData+8))/* 0x1FFFC0B2+0x134=0x1FFFC1E6 */
#define cdwRAM_USART3_TXBUFLEN_ADDR              (cdwRAM_USART2_TXBUFLEN_ADDR+2)/* 0x1FFFC1E6+2=0x1FFFC1E8 */
#define cdwRAM_UART4_TXBUFLEN_ADDR               (cdwRAM_USART3_TXBUFLEN_ADDR+2)/* 0x1FFFC1E8+2=0x1FFFC1EA */

#define cdwRAM_UART5_TXBUFLEN_ADDR               (cdwRAM_UART4_TXBUFLEN_ADDR+2)/* 0x1FFFC1EA+2=0x1FFFC1EC */
#define cdwRAM_USART2_RXBUFLEN_ADDR              (cdwRAM_UART5_TXBUFLEN_ADDR+2)/* 0x1FFFC1EC+2=0x1FFFC1EE */

#define cdwRAM_USART3_RXBUFLEN_ADDR              (cdwRAM_USART2_RXBUFLEN_ADDR+2)/* 0x1FFFC1EE+2=0x1FFFC1F0 */
#define cdwRAM_UART4_RXBUFLEN_ADDR               (cdwRAM_USART3_RXBUFLEN_ADDR+2)/* 0x1FFFC1F0+2=0x1FFFC1F2 */

#define cdwRAM_UART5_RXBUFLEN_ADDR               (cdwRAM_UART4_RXBUFLEN_ADDR+2)/* 0x1FFFC1F2+2=0x1FFFC1F4 */
#define cdwRAM_USART2_TXCNT_ADDR                 (cdwRAM_UART5_RXBUFLEN_ADDR+2)/* 0x1FFFC1F4+2=0x1FFFC1F6 */

#define cdwRAM_USART3_TXCNT_ADDR                 (cdwRAM_USART2_TXCNT_ADDR+2)/* 0x1FFFC1F6+2=0x1FFFC1F8 */
#define cdwRAM_UART4_TXCNT_ADDR                  (cdwRAM_USART3_TXCNT_ADDR+2)/* 0x1FFFC1F8+2=0x1FFFC1FA */

#define cdwRAM_UART5_TXCNT_ADDR                  (cdwRAM_UART4_TXCNT_ADDR+2)/* 0x1FFFC1FA+2=0x1FFFC1FC */
#define cdwRAM_USART2_RXCNT_ADDR                 (cdwRAM_UART5_TXCNT_ADDR+2)/* 0x1FFFC1FA+2=0x1FFFC1FE */

#define cdwRAM_USART3_RXCNT_ADDR                 (cdwRAM_USART2_RXCNT_ADDR+2)/* 0x1FFFC1FA+2=0x1FFFC200 */
#define cdwRAM_UART4_RXCNT_ADDR                  (cdwRAM_USART3_RXCNT_ADDR+2)/* 0x1FFFC1FA+2=0x1FFFC204 */

#define cdwRAM_UART5_RXCNT_ADDR                  (cdwRAM_UART4_RXCNT_ADDR+2)/* 0x1FFFC1FA+2=0x1FFFC208 */
#define cdwRAM_RESREVER_ALIGNMENT_ADDR2          (cdwRAM_UART5_RXCNT_ADDR+2)//alignment 4


//#define bBootRAMUARTCode                        ((INT8U*)cdwRAM_UARTCODE_ADDR)
//#define bBootRAMCANCode                         ((INT8U*)cdwRAM_CANCODE_ADDR)
//#define bBootRAMFlashCode                       ((INT8U*)cdwRAM_FLASHCODE_ADDR)
//#define bBootRAMRodata                          ((INT8U*)cdwRAM_RODATA_ADDR)
//#define bBootRAMSpiFlashCode                    ((INT8U*)cdwRAM_SpiFLASHCODE_ADDR)

#define fBootI2CAckError                        (*((INT8U*)cdwRAM_F_IIC_ACKERR_ADDR))
#define bBootAppMaskBuff                        ((INT8U*)cdwRAM_APPMASK_BUFF_ADDR)
#define pstBootUARTConfig                       ((BootUARTConfig*)cdwRAM_BOOT_UARTCONFIG_ADDR)





//boot ICP buffer
#define bBootICPTxBuf                           ((INT8U*)cdwRAM_ICPTxBUF_ADDR)/*(cbMaxPackageData + 8)*/
#define bBootICPRxBuf                           ((INT8U*)cdwRAM_ICPRxBUF_ADDR)/*(cbMaxPackageData + 8)*/
//boot transparent buffer
#define bBootTransTxBuf                         ((INT8U*)cdwRAM_TRANS_TXBUF_ADDR)/*(cbMaxPackageData + 8)*/
#define bBootTransRxBuf                         ((INT8U*)cdwRAM_TRANS_RXBUF_ADDR)/*(cbMaxPackageData + 8)*/


#define wUSART2TxBufLen                         (*((INT16U*)cdwRAM_USART2_TXBUFLEN_ADDR))
#define wUSART3TxBufLen                         (*((INT16U*)cdwRAM_USART3_TXBUFLEN_ADDR))
#define wUART4TxBufLen                          (*((INT16U*)cdwRAM_UART4_TXBUFLEN_ADDR))
#define wUART5TxBufLen                          (*((INT16U*)cdwRAM_UART5_TXBUFLEN_ADDR))
//multiplexing!!!!!
#define wICPTxBufLen                            (*((INT16U*)cdwRAM_UART5_TXBUFLEN_ADDR))
#define wTransTxBufLen                          (*((INT16U*)cdwRAM_USART3_TXBUFLEN_ADDR))


#define wUSART2RxBufLen                         (*((INT16U*)cdwRAM_USART2_TXBUFLEN_ADDR))
#define wUSART3RxBufLen                         (*((INT16U*)cdwRAM_USART3_TXBUFLEN_ADDR))
#define wUART4RxBufLen                          (*((INT16U*)cdwRAM_UART4_RXBUFLEN_ADDR))
#define wUART5RxBufLen                          (*((INT16U*)cdwRAM_UART5_RXBUFLEN_ADDR))

//multiplexing!!!!!
#define wICPRxBufLen                            (*((INT16U*)cdwRAM_UART5_RXBUFLEN_ADDR))
#define wTransRxBufLen                          (*((INT16U*)cdwRAM_USART3_RXBUFLEN_ADDR))





#define wUSART2TxCnt                            (*((INT16U*)cdwRAM_USART2_TXCNT_ADDR))
#define wUSART3TxCnt                            (*((INT16U*)cdwRAM_USART3_TXCNT_ADDR))
#define wUART4TxCnt                             (*((INT16U*)cdwRAM_UART4_TXCNT_ADDR))
#define wUART5TxCnt                             (*((INT16U*)cdwRAM_UART5_TXCNT_ADDR))


#define wICPTxCnt                               (*((INT16U*)cdwRAM_UART5_TXCNT_ADDR))
#define wTransTxCnt                             (*((INT16U*)cdwRAM_USART3_TXCNT_ADDR))





#define wUSART2RxCnt                            (*((INT16U*)cdwRAM_USART2_RXCNT_ADDR))
#define wUSART3RxCnt                            (*((INT16U*)cdwRAM_USART3_RXCNT_ADDR))
#define wUART4RxCnt                             (*((INT16U*)cdwRAM_UART4_RXCNT_ADDR))
#define wUART5RxCnt                             (*((INT16U*)cdwRAM_UART5_RXCNT_ADDR))


#define wICPRxCnt                               (*((INT16U*)cdwRAM_UART5_RXCNT_ADDR))
#define wTransRxCnt                             (*((INT16U*)cdwRAM_USART3_RXCNT_ADDR))


//offset calculation
//#define BootRodata(pAddr)                       \
//    ((((INT32U)pAddr)<=cdwRAM_RODATA_ADDR)?(((INT32U)pAddr)+dwBootRodataAddrOffset):(INT32U)(pAddr))
//#define BootFlashFunc(pFunc)                    \
//    ((((INT32U)pFunc)<=cdwRAM_FLASHCODE_ADDR)?(((INT32U)pFunc)+dwBootFlashCodeAddrOffset):(INT32U)(pFunc))
//#define BootUARTFunc(pFunc)                     \
//    ((((INT32U)pFunc)<=cdwRAM_UARTCODE_ADDR)?(((INT32U)pFunc)+dwBootUARTCodeAddrOffset):(INT32U)(pFunc))
//#define BootCANFunc(pFunc)                     \
//    ((((INT32U)pFunc)<=cdwRAM_CANCODE_ADDR)?(((INT32U)pFunc)+dwBootCANCodeAddrOffset):(INT32U)(pFunc))
//#define BootSpiFlashFunc(pFunc)                 \
//    ((((INT32U)pFunc)<=cdwRAM_SpiFLASHCODE_ADDR)?(((INT32U)pFunc)+dwBootSpiFlashCodeAddrOffset):(INT32U)(pFunc))




/*********************************************************
* type definition
*********************************************************/
typedef void (*psVoidFunc)(void);

typedef struct
{
    INT32U dwFirmStartAddr;
    INT32U dwFirmEndAddr;
    INT32U dwCurrentFirmAddr;
    INT16U wLastFirmLen;
    INT8U bFileMD5[cbFirmUUIDLen];//MD5 of PAC file
    INT8U bKEY[4];
    INT8U fUpdateEN;
    INT8U fMD5VerifyOK;
    INT8U fHaveRecPackage;
    INT8U fHaveErase;
}UpdatePackInfo;

typedef struct
{
    INT16U wCmdType;//command type
    INT8U bRs485Addr;
    INT8U bBootBaudrate;
    INT8U fBootUARTUpdateTxDisable;
    INT8U fBootUARTUpdateReg;//if regist updating
    INT8U fAppUARTICP;      //if from app
    INT8U bBootUpdateType;  //force update, advance update, normal update
    INT8U fBootStartUpdate; //if start to update enable
    INT8U bICPTxCH;        //UART1, UART2, UART3,CAN
    INT8U bAppICPTxCH;
    INT8U fCmdOK;           //if command OK
    INT8U fTransparence;    //if transparence data
    INT8U fQueryAddrOK;  //if to communition address comfirm
    INT8U fAppSPIFLASHICP;
    INT8U bParity;
}UpdateUARTInfo;

typedef struct
{
    INT8U bUARTxFunc[6]; //transparent; ICP; NULL
    INT8U bRs485Addr[6];
    INT8U bBaudrate[6];
    INT8U bParity[6];
}BootUARTConfig;

typedef struct
{
    INT16U wMasterCmdType;
    INT16U wSubCmdType;
    INT8U bSubRs485Addr;
    INT8U bMasterRs485Addr;
    INT8U fCmdOK;
    INT8U bTransTxCH;
    INT8U bBootTrsBaudrate;
    INT8U fNeedTrans;    //need to transparence data
    INT8U bSubRsCnt;
    INT8U bSubRsSendCnt;
    INT8U fQuerySend;
    INT8U bDevID[3];
    INT16U wTransLenth;
    INT16U wReserve;
    INT8U bSubRsCANAddr[8];
    INT8U bTransStartCmdInfo[100];//transparent version info

}UARTTransInfo;//info of ICP transparent

typedef struct
{
    INT8U fForceUpdate;
    INT8U bReverse[3];
}
UpdateSPIFlashInfo;

typedef struct
{
    INT32U dwAddressOffset;
    INT32U dwID;
    INT32U dwAddressOffsetLast;
    INT16U wLength;
    INT16U wSendCnt;
    INT8U fIDE;
    INT8U bFuncCode;
    INT8U fSecSend;
}BootCANSecInfo;

typedef struct
{
    INT8U bFuncCode;
    INT16U wLength;
    INT16U wRevCnt;
    INT32U dwAddress;
}BootCANRxInfo;

typedef struct
{
    INT8U *pbRxBuf;
    INT8U *pbTxBuf;
    INT16U wRxBufLen;
    INT16U wTxBufLen;
    INT8U b485Addr;
    INT8U bBaudrate;
    INT8U bUartCH;
    INT8U bParity;
}AppUartInfo;

typedef struct
{
    INT8U bRecursionCnt;
    INT8U bCommuAddr;
    INT8U bTransCH;
    INT8U bRoute;
    INT8U bRouteNext;
}BootFirmCompareInfo;

typedef union
{
    INT8U Msg[16];
    struct
    {
        //DLC and ID
        INT32U                  :16;
        INT32U  DLC             :4;
        INT32U	RTR             :1;
        INT32U	IDE             :1;
        INT32U                  :2;
        INT32U  Status          :4;
        INT32U                  :4;
        INT32U  ID              :29;
        INT32U                  :3;
    }bit;
    struct
    {
        INT32U Reserve1         :32;
        INT32U Reserve2         :32;
        //8 bytes data
        INT8U   DataL[1];
        INT32U  Length          :16;
        INT32U  CmdType         :8;
        INT8U   DataH[4];
    }fir;
    struct
    {
        INT32U Reserve1         :32;
        INT32U Reserve2         :32;
        //8 bytes data
        INT8U   DataL[1];
        INT32U  Cnt             :16;
        INT32U  CmdType         :8;
        INT8U   DataH[4];
    }sec;
}BootCanMsgType_Func;
/*********************************************************
* extern variables and functions
*********************************************************/
//following extern CANNOT be used in APP area
extern const INT8U bFrimwareVerStr[3];
extern const INT8U bFirmwareTypeStr[4];
extern const INT8U bBootFirmwareStr[2];
extern const INT8U FirmwareAppStartAddr[4];

extern const INT16U wCRCTableAbs[16];
extern const INT16U wBootRxDelay_500us[cbSetBandRateMax];
extern const INT32U cwBootBaudRateTab[cbSetBandRateMax];



//asm functions
//cannot be used in APP area
extern void sAppOkMark(void);
extern void sDoIarcmain(void);
extern void sResetSP(void);
extern void sClearReverseRam(void);
extern void sClearUserRam(void);
extern void sAsmTest(void);

//following extern variables, can NOT be used in APP area
extern INT32U dwBootFlashCodeAddrOffset;
extern INT32U dwBootUARTCodeAddrOffset;
extern INT32U dwBootCANCodeAddrOffset;
extern INT32U dwBootSpiFlashCodeAddrOffset;
extern INT32U dwBootRodataAddrOffset;
extern UpdateUARTInfo stUpdateUARTInfo;
extern UARTTransInfo stTransUARTInfo;
extern UpdateSPIFlashInfo stUpdateSPIFlashInfo;
extern UpdatePackInfo stUpdatePackInfo;
extern DevTreeInfo stBootDevTree[cbMaxNode];

//extern variables, constants, functions must be constant address
extern const INT32U dwBootExtTbl[cbMaxExtTable];

//APP functions and variables
extern INT8U fSCIICPProcess;
extern INT8U bICPTimer;

extern INT8U sBootAppICPAnalysis(AppUartInfo *pstAppUartInfo);
extern void sSpiFlashICP(INT8U fForceUpdate);
//extern void sBootLoaderStart(void);
extern void SystemReset(void);
extern void sResetEntry(void);
extern void BootSystemInit(void);

/*
//ICP Process return, copy to 1s time base
if(fSCIICPProcess)
{
    bICPTimer++;

    if(bICPTimer >= 60)  //1min
    {
        bICPTimer = 0;
        fSCIICPProcess = false;
    }
}
else
{
    bICPTimer = 0;
}
*/

#endif

