#ifndef __IIC_DRIVER_H__
#define __IIC_DRIVER_H__

//#include "MCU_Init.h"
//#include "typedef.H"
//#include "Gpio.h"
#include "n32g45x.h"
#include "system_n32g45x.h"

#define cbTIMER_10MS        10
#ifdef KINERGY_II_WIFI
#define I2Cx I2C1
#define I2Cx_SCL_PIN        GPIO_PIN_8
#define I2Cx_SDA_PIN        GPIO_PIN_9
#define I2Cx_WP_PIN         GPIO_PIN_4
#define I2C_GPIO            GPIOB
#define I2Cx_WP_GPIO        GPIOA
#else
#define I2Cx I2C1
#define I2Cx_SCL_PIN        GPIO_PIN_8
#define I2Cx_SDA_PIN        GPIO_PIN_9
#define I2Cx_WP_PIN         GPIO_PIN_3		//沒有使用
#define I2C_GPIO            GPIOB
#define I2Cx_WP_GPIO        GPIOA
#endif


#define I2Cx_peripheral_clk_en() RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_I2C1, ENABLE)
#define I2Cx_scl_clk_en()        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE)
#define I2Cx_sda_clk_en()        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE)
#define I2Cx_wp_clk_en()         RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE)

#define I2Cx_write_enable()          GPIO_ConfigPinRemap(GPIO_Remap_SPI1_NSS,ENABLE);\
                                     GPIO_ResetBits(I2Cx_WP_GPIO,I2Cx_WP_PIN)
#define I2Cx_write_disable()         GPIO_ConfigPinRemap(GPIO_Remap_SPI1_NSS,ENABLE);\
                                     GPIO_SetBits(I2Cx_WP_GPIO,I2Cx_WP_PIN)

#define HalI2CWriteHigh()   \
    asm("CPSID I");         \
    I2C_GPIO->PBSC = I2Cx_SDA_PIN;\
    asm("CPSIE I")
//      GPIO_ConfigPinRemap(GPIO_RMP_I2C1,ENABLE);\
//      GPIO_SetBits(GPIOB,GPIO_PIN_9);\

#define HalI2CWriteLow()    \
    asm("CPSID I");         \
    I2C_GPIO->PBC = I2Cx_SDA_PIN;\
    asm("CPSIE I")
//      GPIO_ConfigPinRemap(GPIO_RMP_I2C1,ENABLE);\
//      GPIO_ResetBits(GPIOB,GPIO_PIN_9);\

//#define	HalI2CReadHighChk()(1 == (I2C_GPIO->PID & I2Cx_SDA_PIN))
//
//#define HalI2CReadLowChk() (0 == (I2C_GPIO->PID & I2Cx_SDA_PIN))
#define	HalI2CReadHighChk()(1 == GPIO_ReadInputDataBit(I2C_GPIO,I2Cx_SDA_PIN))

#define HalI2CReadLowChk() (0 == GPIO_ReadInputDataBit(I2C_GPIO,I2Cx_SDA_PIN))


//clk
#define HalI2CClkHigh()     \
	asm("CPSID I");         \
    I2C_GPIO->PBSC = I2Cx_SCL_PIN;\
    asm("CPSIE I")

#define HalI2CClkLow()      \
	asm("CPSID I");         \
    I2C_GPIO->PBC = I2Cx_SCL_PIN;\
    asm("CPSIE I")

//wp
#define HalEEPROMWrEnable()	\
	asm("CPSID I");         \
    I2Cx_WP_GPIO->PBC = I2Cx_WP_PIN;\
    asm("CPSIE I")//reset

#define HalEEPROMWrDisable() \
	asm("CPSID I");         \
    I2Cx_WP_GPIO->PBSC = I2Cx_WP_PIN;\
    asm("CPSIE I")//set


#define Delay_1us_Cnt       (int)(SYSCLK_FREQ/10000000)
extern INT8U bI2CWREnTimer;
void sI2CPinInit(void);

void sEepromRead(INT16U wAddress, INT8U bDataLength, INT8U *bReadBuf);
void sEepromWrite(INT16U wAddress, INT8U bDataLength, INT8U *bWriteBuf);

INT8U sIICSlaveWrite(INT8U bSlaveAddress, INT8U bStartAddr, INT8U bDataLength, const INT8U *bWriteBuf);
INT8U sIICSlaveRead(INT8U bSlaveAddress, INT8U bStartAddr, INT8U bDataLength, INT8U *bReadBuf);

void sDelay100ms(void);
void sDelay20ms(void);
void sDelay10ms(void);
void sDelay2ms(void);
void sDelay4us(void);
void sDelay1Nop(void);
void sIICPinInit(void);
//void sCONFIG_PIN_AS_GPIO(GPIO_Module* GPIOx,uint16_t Pin,uint8_t f_output);
#endif
