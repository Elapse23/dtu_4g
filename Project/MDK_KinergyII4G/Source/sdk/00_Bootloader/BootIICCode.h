
#ifndef  __BOOT_IIC_CODE_H__
#define __BOOT_IIC_CODE_H__

#include "BootLoader.h"
#include "BootloaderConfig.h"
#include "drv_log.h"

/************************************************************************************************
* I2C Function defination
************************************************************************************************/

#if (1 == HalBootI2CFuncEN)

#if( I2CCH1  == I2C_CH)
#if (0 == I2C1_REMAP)
#define I2C_REG   I2C1
#define I2C_SCL   GPIOB,8
#define I2C_SDA   GPIOB,9
#define I2C_WP    GPIOC,2
#endif
#endif



#define MACRO_I2C_WP_INIT()             do{ HAL_GPIO_SET_HIGHT(I2C_WP);\
                                             HAL_GPIO_SET_MODE(I2C_SDA,INPUT_PULL_UP);}while(0)
#define MACRO_I2C_SCL_INIT()            do{ HAL_GPIO_SET_LOW(I2C_SCL);\
                                                HAL_GPIO_SET_MODE(I2C_SCL,AF_MODE_PUSH_PULL);}while(0)
#define MACRO_I2C_SDA_INIT()            do{ HAL_GPIO_SET_LOW(I2C_SDA);\
                                                HAL_GPIO_SET_MODE(I2C_SDA,INPUT_PULL_UP);}while(0)





#define HalBootI2CDataOP()              HAL_GPIO_SET_MODE(I2C_SDA,OUTPUT_PUSH_PULL);
#define HalBootI2CDataIP()              HAL_GPIO_SET_MODE(I2C_SDA,INPUT_PULL_UP);

#define HalBootI2CWrEnable()            HAL_GPIO_SET_LOW(I2C_WP)        //(GPIOA->PDOR &= ~GPIO_PTD4_MASK)//wp  0
#define HalBootI2CWrDisable()           HAL_GPIO_SET_HIGHT(I2C_WP)        //(GPIOA->PDOR |=  GPIO_PTD4_MASK)1
#define HalBootI2CClkHigh()             HAL_GPIO_SET_HIGHT(I2C_SCL)        //(GPIOA->PDOR |= GPIO_PTA3_MASK)//scl 1
#define HalBootI2CClkLow()              HAL_GPIO_SET_LOW(I2C_SCL)        //(GPIOA->PDOR &= ~GPIO_PTA3_MASK) 0
#define HalBootI2CWriteHigh()           HAL_GPIO_SET_HIGHT(I2C_SDA)         //(GPIOA->PDOR |= GPIO_PTA2_MASK) 1
#define HalBootI2CWriteLow()            HAL_GPIO_SET_LOW(I2C_SDA)       //(GPIOA->PDOR &= ~GPIO_PTA2_MASK)//sda 0
#define HalBootI2CReadHighChk()         HAL_GPIO_IS_SET(I2C_SDA)       //((GPIOA->PDIR & GPIO_PTA2_MASK) == GPIO_PTA2_MASK) 1
#define HalBootI2CReadLowChk()          !HAL_GPIO_IS_SET(I2C_SDA)        //((GPIOA->PDIR & GPIO_PTA2_MASK) != GPIO_PTA2_MASK) 0



//#define HalBootI2CDataOP()              \
//        GPIOA->PDDR |= GPIO_PTA2_MASK;  \
//        GPIOA->PIDR |= GPIO_PTA2_MASK;
//#define HalBootI2CDataIP()              \
//        GPIOA->PDDR &= ~GPIO_PTA2_MASK; \
//        GPIOA->PIDR &= ~GPIO_PTA2_MASK;
//#define HalBootI2CWrEnable()                    (GPIOA->PDOR &= ~GPIO_PTD4_MASK)//wp
//#define HalBootI2CWrDisable()                   (GPIOA->PDOR |=  GPIO_PTD4_MASK)
//#define HalBootI2CClkHigh()                     (GPIOA->PDOR |= GPIO_PTA3_MASK)//scl
//#define HalBootI2CClkLow()                      (GPIOA->PDOR &= ~GPIO_PTA3_MASK)
//#define HalBootI2CWriteHigh()                   (GPIOA->PDOR |= GPIO_PTA2_MASK)
//#define HalBootI2CWriteLow()                    (GPIOA->PDOR &= ~GPIO_PTA2_MASK)//sda
//#define HalBootI2CReadHighChk()                 ((GPIOA->PDIR & GPIO_PTA2_MASK) == GPIO_PTA2_MASK)
//#define HalBootI2CReadLowChk()                  ((GPIOA->PDIR & GPIO_PTA2_MASK) != GPIO_PTA2_MASK)

void  sBootReadEEInit(void);
//void sBootReadEEInfo(void);


#else
#define sBootReadEEInit
//#define sBootReadEEInfo
#endif


#endif
