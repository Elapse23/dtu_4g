/************************************************************************************************
File Name:      BootloaderHead.H
Description:    BOotloader user define header
Version:        V1.0
Author:         chenweijie
Date:           2019-09-04
*************************************************************************************************/
#ifndef __BOOT_HEAD_H__
#define __BOOT_HEAD_H__

//#include "S32K144.H"
//#include "OS_HEAD.h"
#include "n32g45x.h"

#define cbMonitorAddr                                   0x52
#ifndef KINERGY_II_4G
#define KINERGY_II_4G
#endif


/* do process while prepare to jump to bootloader */
#define M_JMP_BOOT_PROCESS()			\
    HalBootIntDisable()
    //all light on
    //buzzer off
    //charger off
    //etc..

//whether enable to change the device which is currently updating
#define cfChangeUpdateDevice                            false
#ifdef KINERGY_II_WIFI
/************************************************************************************************
* boot and APP version
************************************************************************************************/
#define cbFirmwareType1                                 Upsilon//cbS32K//Koala
#define cdwFirmwareType2                                400100//"4020 01"
#define cbAppVersion1                                   24//".01"
#define cwAppVersion2                                   187//".0002"
#define cbBootVersion1                                  1//"b05"
#define cbBootVersion2                                  10//".07"

#define cwDeviceType                                    cwDeviceType_KINERGY_II//cwDeviceType_Vision
#define cbCommuAddr                                     cbMonitorAddr

#define cbDevMCUType                                    cbFirmwareType1
#else
/************************************************************************************************
* boot and APP version
************************************************************************************************/
#define cbFirmwareType1                                 Upsilon//cbS32K//Koala
#define cdwFirmwareType2                                400100//"4020 01"
#define cbAppVersion1                                   10//".01"
#define cwAppVersion2                                   128//".0002"
#define cbBootVersion1                                  1//"b05"
#define cbBootVersion2                                  10//".07"

#define cwDeviceType                                    cwDeviceType_KINERGY_II//cwDeviceType_Vision
#define cbCommuAddr                                     cbMonitorAddr

#define cbDevMCUType                                    cbFirmwareType1
#endif


/************************************************************************************************
* device defination
************************************************************************************************/
//max number of device nodes in device tree
#define cbMaxNode                                       6

#define cbMaxBootSubsets                                (cbMaxNode-1)




///* select UART0 pin */
//#define fUART0Enable                            false
//
//#define UART0_Tx_GPIO                           PTC
//#define UART0_Tx_PORT                           PORTC
//#define UART0_Tx_PortPCC                        PCC_PORTC_INDEX
//#define UART0_Tx_Pin                            3
//#define UART0_Tx_PinMux                         4
//
//#define UART0_Rx_GPIO                           PTC
//#define UART0_Rx_PORT                           PORTC
//#define UART0_Rx_PortPCC                        PCC_PORTC_INDEX
//#define UART0_Rx_Pin                            2
//#define UART0_Rx_PinMux                         4
//
///* select UART1 pin */
//#define fUART1Enable                            false
//
//#define UART1_Tx_GPIO                           PTD
//#define UART1_Tx_PORT                           PORTD
//#define UART1_Tx_PortPCC                        PCC_PORTD_INDEX
//#define UART1_Tx_Pin                            14
//#define UART1_Tx_PinMux                         3
//
//#define UART1_Rx_GPIO                           PTD
//#define UART1_Rx_PORT                           PORTD
//#define UART1_Rx_PortPCC                        PCC_PORTD_INDEX
//#define UART1_Rx_Pin                            13
//#define UART1_Rx_PinMux                         3
//
//
///* select UART2 pin */
//#define fUART2Enable                            false
//
//#define UART2_Tx_GPIO                           PTD
//#define UART2_Tx_PORT                           PORTD
//#define UART2_Tx_PortPCC                        PCC_PORTD_INDEX
//#define UART2_Tx_Pin                            7
//#define UART2_Tx_PinMux                         2
//
//#define UART2_Rx_GPIO                           PTD
//#define UART2_Rx_PORT                           PORTD
//#define UART2_Rx_PortPCC                        PCC_PORTD_INDEX
//#define UART2_Rx_Pin                            6
//#define UART2_Rx_PinMux                         2

/* select CAN0 pin */

/* select CAN1 pin */

/* select CAN2 pin */


//refer to pstBootUARTConfig info
        
        



/************************************************************************************************
* I2C Function defination
************************************************************************************************/


//#define O_Boot_I2C_SCL                          GPIO_PTA3
//#define O_Boot_I2C_SDA                          GPIO_PTA2
//#define O_Boot_I2C_WP                           GPIO_PTD4

//EX_FLASH_GPIO_WP        GPIOC,2



/************************************************************************************************
* watch dog
************************************************************************************************/
//extern watch dog






        



#endif  //#ifndef __BOOT_HEAD_H__


