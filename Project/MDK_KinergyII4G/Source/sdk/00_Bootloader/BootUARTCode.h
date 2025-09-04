/************************************************************************************************
File Name:      BootUARTCode.C
Description:    bootloader UART operation code
Version:        V1.0
Author:         chenweijie
Date:           2018-08-24
*************************************************************************************************/
#ifndef BOOT_UARTCODE_H
#define BOOT_UARTCODE_H
#include "BootLoader.h"

/************************************************************************************************
* UART defination
************************************************************************************************/
#define UART_PIN_NULL                           0xFF

#define fUSART2Enable                           false
#define fUSART3Enable                           false
#define fUART4Enable                            false
#define fUART5Enable                            true


//DR
#define RS485_DR_PORT            GPIOB
#define RS485_DR_Pin             3
#define RS485_DR_GPIO            GPIOB,RS485_DR_Pin

#define RS485_DR_SET_SND()       do {JTAG_SW_REMAP();HAL_GPIO_SET_HIGHT(RS485_DR_GPIO);}while(0)
#define RS485_DR_SET_REC()       do {JTAG_SW_REMAP();HAL_GPIO_SET_LOW(RS485_DR_GPIO);}while(0)

#define RS485_GPIO_RCC_OPEN()    do {APB_RCC_OPEN(APB2,AFIO);APB_RCC_OPEN(APB2,GPIOB);\
                                    APB_RCC_OPEN(APB2,GPIOC);APB_RCC_OPEN(APB2,GPIOD);}while(0)
//UART
#define RS485_UART               UART5
#ifndef RS485_TX_GPIO
#define RS485_TX_GPIO            UART5_TX_GPIO
#endif
#ifndef RS485_RX_GPIO
#define RS485_RX_GPIO            UART5_RX_GPIO
#endif
#define RS485_UART_RCC_OPEN()    APB_RCC_OPEN(APB1,UART5)
#define RS485_UART_RCC_CLOSE()   APB_RCC_CLOSE(APB1,UART5)

#define BOOT_RESET_DELAY_TIME  10//
#define BOOT_UART_DELAY_TIME   60




/************************************************************************************************
* UART defination
************************************************************************************************/
#define UART_PIN_NULL                           0xFF

#define fUSART2Enable                           false
#define fUSART3Enable                           false
#define fUART4Enable                            false
#define fUART5Enable                            true

#define cbUART0Rs485Addr                        cbCommuAddr
#define cbUART1Rs485Addr                        cbCommuAddr
#define cbUART2Rs485Addr                        cbCommuAddr

#define cbUSART2Rs485Addr                        cbCommuAddr
#define cbUSART3Rs485Addr                        cbCommuAddr
#define cbUART4Rs485Addr                        cbCommuAddr
#define cbUART5Rs485Addr                        cbCommuAddr

#define cbCAN0Addr                              cbCommuAddr
#define cbCAN1Addr                              cbCommuAddr
#define cbCAN2Addr                              cbCommuAddr

#define cbUART0DefaultBaudrate                  cbSetBandRate9600
#define cbUART1DefaultBaudrate                  cbSetBandRate9600
#define cbUART2DefaultBaudrate                  cbSetBandRate9600

#define cbUSART2DefaultBaudrate                  cbSetBandRate9600
#define cbUSART3DefaultBaudrate                  cbSetBandRate9600
#define cbUART4DefaultBaudrate                  cbSetBandRate9600
#define cbUART5DefaultBaudrate                  cbSetBandRate9600

#define cbCAN0DefaultBaudrate                   cbSetCANBaud500K
#define cbCAN1DefaultBaudrate                   cbSetCANBaud500K
#define cbCAN2DefaultBaudrate                   cbSetCANBaud500K

#define cbUART0Parity                           cbSetNoCheck
#define cbUART1Parity                           cbSetNoCheck
#define cbUART2Parity                           cbSetNoCheck

#define cbUSART2Parity                          cbSetNoCheck
#define cbUSART3Parity                          cbSetNoCheck
#define cbUART4Parity                          cbSetNoCheck
#define cbUART5Parity                          cbSetNoCheck

/************************************************************************************************
* RS485 pin defination
************************************************************************************************/


//UART
#define cbUART0                                 0
#define cbUART1                                 1
#define cbUART2                                 2

#define cbUSART2                                2
#define cbUSART3                                3
#define cbUART4                                 4
#define cbUART5                                 5
#define cbCHNull                                0xFF




#define HalBootUSart5RS485TxEnable()    RS485_DR_SET_SND()//        do {JTAD_SW_ENABLE();HalPortSetPinHigh(USART5_485Tx_GPIO, USART5_485TxPin);}while(0)

#define HalBootUSart5RS485RxEnable()    RS485_DR_SET_REC()//        do {JTAD_SW_ENABLE();HalPortSetPinLow(USART5_485Tx_GPIO,USART5_485TxPin);}while(0)




#define cdwUSART_CTRL1_SEND_BREAK                   ((INT32U)0x01L << 0)    //Send break
#define cdwUSART_CTRL1_RX_WAKEUP                    ((INT32U)0x01L << 1)    //Receiver wakeup
#define cdwUSART_CTRL1_RECEIVER_ENABLE              ((INT32U)0x01L << 2)    //Receiver enable
#define cdwUSART_CTRL1_TRANSMITTER_ENABLE           ((INT32U)0x01L << 3)    //Transmitter enable
#define cdwUSART_CTRL1_IDLE_INT_ENABLE              ((INT32U)0x01L << 4)    //Idle interrupt enable
#define cdwUSART_CTRL1_RX_FULL_INT_ENABLE           ((INT32U)0x01L << 5)    //Receiver buffer full interrupt enable
#define cdwUSART_CTRL1_TX_COMPLETE_INT_ENABLE       ((INT32U)0x01L << 6)    //Transmission complete interrupt enable
#define cdwUSART_CTRL1_TX_BUF_EMPTY_INT_ENABLE      ((INT32U)0x01L << 7)    //Transmitter buffer empty interrupt enable
#define cdwUSART_CTRL1_PARITY_ERR_INT_ENABLE        ((INT32U)0x01L << 8)    //Parity error interrupt enable


#define cdwUSART_CTRL1_PARITY_NO                    ((INT32U)0x00L << 9)    //Parity bit
#define cdwUSART_CTRL1_PARITY_EVEN                  ((INT32U)0x02L << 9)
#define cdwUSART_CTRL1_PARITY_ODD                   ((INT32U)0x03L << 9)

#define cdwUSART_CTRL1_WAKEUP_IDLE_LINE             ((INT32U)0x00L << 11)   //Wake up mode
#define cdwUSART_CTRL1_WAKEUP_ADDR_MASK             ((INT32U)0x01L << 11)

#define cdwUSART_CTRL1_DATA_LEN_8B                  ((INT32U)0x00L << 12)   //Data length
#define cdwUSART_CTRL1_DATA_LEN_9B                  ((INT32U)0x01L << 12)

#define cdwUSART_CTRL1_USART_ENABLE                 ((INT32U)0x01L << 13)   //Usart enable

/*----------------------------------------------------------------------------
//Usart pin remap
----------------------------------------------------------------------------*/
#define cdwUSART1_MAP_ON_PA9_PA10                   0x00000000
#define cdwUSART1_MAP_ON_PB6_PB7                    0x00000004

#define cdwUSART2_MAP_ON_PA2_PA3                    0x00000000
#define cdwUSART2_MAP_ON_PD5_PD6                    0x00000008
#define cdwUSART2_MAP_ON_PC8_PC9                    0x00080000
#define cdwUSART2_MAP_ON_PB4_PB5                    0x00080008

#define cdwUSART3_MAP_ON_PB10_PB11                  0x00000000
#define cdwUSART3_MAP_ON_PC10_PC11                  0x00000010
#define cdwUSART3_MAP_ON_PD8_PD9                    0x00000030

#define cdwUART4_MAP_ON_PC10_PC11                   0x00000000
#define cdwUART4_MAP_ON_PB2_PE7                     0x00100000
#define cdwUART4_MAP_ON_PA13_PA14                   0x00200000
#define cdwUART4_MAP_ON_PD0_PD1                     0x00300000

#define cdwUART5_MAP_ON_PC12_PD2                    0x00000000
#define cdwUART5_MAP_ON_PB13_PB14                   0x00400000
#define cdwUART5_MAP_ON_PE8_PE9                     0x00800000
#define cdwUART5_MAP_ON_PB8_PB9                     0x00C00000

#define cdwUART6_MAP_ON_PE2_PE3                     0x00000000
#define cdwUART6_MAP_ON_PC0_PC1                     0x02000000
#define cdwUART6_MAP_ON_PB0_PB1                     0x03000000

#define cdwUART7_MAP_ON_PC4_PC5                     0x00000000
#define cdwUART7_MAP_ON_PC2_PC3                     0x08000000
#define cdwUART7_MAP_ON_PG0_PG1                     0x0C000000

#define cdwUSART5_C1_CFG                            (cdwUSART_CTRL1_RECEIVER_ENABLE | cdwUSART_CTRL1_TRANSMITTER_ENABLE\
                                                     cdwUSART_CTRL1_RX_FULL_INT_ENABLE | cdwUSART_CTRL1_TX_COMPLETE_INT_ENABLE\
                                                    )
#define cdwUSART5_C2_CFG                            0x0000
#define cdwUSART5_C3_CFG                            0x0000
#define cdwUSART5_PIN_MAP_SET                       cdwUART5_MAP_ON_PC12_PD2

#define HalUartRxEnable(pUART)                      (pUART)->CTRL1 |= cdwUSART_CTRL1_RECEIVER_ENABLE
#define HalUartTxEnable(pUART)                      (pUART)->CTRL1 |= cdwUSART_CTRL1_TRANSMITTER_ENABLE
#define HalUartEnable(pUART)                        (pUART)->CTRL1 |= cdwUSART_CTRL1_USART_ENABLE
#define HalUartTxIntEnable(pUART)                   (pUART)->CTRL1 |= cdwUSART_CTRL1_TX_COMPLETE_INT_ENABLE
#define HalUartTxIntDisable(pUART)                  (pUART)->CTRL1 &= ~cdwUSART_CTRL1_TX_COMPLETE_INT_ENABLE


//UART
//#if (UART5_MAP == 0)

#define UART5_TX_PORT            GPIOC
#define UART5_TX_PIN             12

#define UART5_RX_PORT            GPIOD
#define UART5_RX_PIN             2

//#endif
#define UART5_TX_GPIO            GPIOC,UART5_TX_PIN
#define UART5_RX_GPIO            UART5_RX_PORT,UART5_RX_PIN


//#if (UART4_MAP == 0)

#define UART4_TX_PORT            GPIOC
#define UART4_TX_PIN             10


#define UART4_RX_PORT            GPIOC
#define UART4_RX_PIN             11

//#endif

#define UART4_TX_GPIO            UART4_TX_PORT,UART4_TX_PIN

#define UART4_RX_GPIO            UART4_RX_PORT,UART4_RX_PIN


#define M_BOOT_UART_BYTE_READ(pUART)          (pUART->DAT)

#define M_BOOT_UART_BYTE_WRITE(pUART,dwData)    \
    (pUART->DAT = dwData)
#define M_BOOT_UART_TX_EMPTY_GET(pUART)        (pUART->STS & USART_STS_TXDE)
#define M_BOOT_UART_TX_EMPTY_CLR(pUART)        (pUART->STS &= ~USART_STS_TXDE)

#define M_BOOT_UART_TX_COMP_CHK(pUART)          \
    ((pUART->STS & USART_STS_TXC) != 0)

#define M_BOOT_UART_RX_FULL_CHK(pUART)          \
    ((pUART->STS & USART_STS_RXDNE) != 0)

#define M_BOOT_UART_RX_INDLE_GET(pUART)       (pUART->STS & USART_STS_IDLEF)
#define M_BOOT_UART_RX_INDLE_CLR(pUART)        (pUART->STS &= ~USART_STS_IDLEF)

#define M_BOOT_UART_FRAMERROR_CHK(pUART)        \
    ((pUART->STS & USART_STS_FEF) != 0)

#define M_BOOT_UART_CLR_FRAMERROR_FLAG(pUART)   \
    (pUART->STS |= USART_STS_FEF)

#define M_BOOT_UART_CLR_RXOVERFLOW_FLAG(pUART)  \
    (pUART->STS |= USART_STS_OREF)

#define M_BOOT_UART_RXOVERFLOW_CHK(pUART)       \
    ((pUART->STS & USART_STS_OREF) != 0)


#define UART_FLAG_ERROR      (USART_STS_PEF|USART_STS_FEF|USART_STS_NEF|USART_STS_OREF)
//#define HAL_UART_FLAG_ERROR(UARTx)                   (UARTx->STS & USART_STS_TXC   != 0)
#define M_UART_ERROR_PEF(pUARTx)                       (pUARTx->STS & USART_STS_PEF  )
#define M_UART_ERROR_FEF(pUARTx)                       (pUARTx->STS & USART_STS_FEF )
#define M_UART_ERROR_NEF(pUARTx)                       (pUARTx->STS & USART_STS_NEF  )
#define M_UART_ERROR_OREF(pUARTx)                      (pUARTx->STS & USART_STS_OREF )
#define M_UART_GET_ERROR(pUARTx)                       (pUARTx->STS & UART_FLAG_ERROR )
#define M_UART_CLR_ERROR(pUARTx)                       (pUARTx->STS &= ~UART_FLAG_ERROR )
#define M_UART_CLR_FLAG(pUARTx)                        (pUARTx->STS = 0)


#define HAL_UART_RX_IS_FULL(pUARTx)                 (pUARTx->STS & USART_STS_RXDNE)
#define HAL_UART_RX_IS_INDLE(pUARTx)                (pUARTx->STS & USART_STS_IDLEF )
#define HAL_UART_RX_INDLE_CLR(pUARTx)                (pUARTx->STS &= ~USART_STS_IDLEF)


#define HAL_UART_TX_IS_EMPTY(pUARTx)                  (pUARTx->STS & USART_STS_TXDE  )
#define HAL_UART_TX_IS_DONE(pUARTx)                   (pUARTx->STS & USART_STS_TXC   )
#define HAL_UART_TX_DONE_CLR(pUARTx)                  (pUARTx->STS &= ~USART_STS_TXC)


#define UART_FLAG_ERROR      (USART_STS_PEF|USART_STS_FEF|USART_STS_NEF|USART_STS_OREF)

//#define HAL_UART_FLAG_ERROR(UARTx)                   (UARTx->STS & USART_STS_TXC   != 0)
#define HAL_UART_ERROR_PEF(pUARTx)                       (pUARTx->STS & USART_STS_PEF  )
#define HAL_UART_ERROR_FEF(pUARTx)                       (pUARTx->STS & USART_STS_FEF )
#define HAL_UART_ERROR_NEF(pUARTx)                       (pUARTx->STS & USART_STS_NEF  )
#define HAL_UART_ERROR_OREF(pUARTx)                      (pUARTx->STS & USART_STS_OREF )
#define HAL_UART_IS_ERROR(pUARTx)                        (pUARTx->STS & UART_FLAG_ERROR )
#define HAL_UART_ERROR_CLR(pUARTx)                       (pUARTx->STS &= ~UART_FLAG_ERROR )
#define HAL_UART_FLAG_CLR(pUARTx)                        (pUARTx->STS = 0)

#define HAL_UART_RX_DATA(pUARTx)                         (pUARTx->DAT)
#define HAL_UART_TX_DATA(pUARTx,wData)                   (pUARTx->DAT = wData)

//#define HalUartReadData(pUART)                      (pUART->DAT)
//#define HalUartWriteData(pUART, wData)              (pUART->DAT = wData)

#define HalUartReadData(pUART)                      (pUART->DAT)
#define HalUartWriteData(pUART, wData)              (pUART->DAT = wData)

#define MACRO_integerdivider(pBaudRate)    ((25 * 36000000) / (4 * pBaudRate))
#define MACRO_tmpregister(pBaudRate)       ((MACRO_integerdivider(pBaudRate) / 100) << 4)
#define MACRO_fractionaldivider(pBaudRate) (MACRO_integerdivider(pBaudRate) - (100 * (MACRO_tmpregister(pBaudRate) >> 4)))
#define MACRO_UART_SET_BAUDRATE(pUSART,pBaudRate)   pUSART->BRCF = MACRO_tmpregister(pBaudRate) | ((((MACRO_fractionaldivider(pBaudRate) * 16) + 50) / 100)) & ((uint8_t)0x0F)

#define HalUartEnable(pUART)                        (pUART)->CTRL1 |= cdwUSART_CTRL1_USART_ENABLE

#define CTRL1_UEN_SET   ((uint16_t)0x2000) /*!< USART Enable Mask */

#define HAL_UART_ENABLE(UART)                       UART->CTRL1 |= CTRL1_UEN_SET
#define HAL_UART_DISABLE(UART)                      UART->CTRL1 &= ~CTRL1_UEN_SET
#define HalUartReadData(pUART)                      (pUART->DAT)
#define HalUartWriteData(pUART, wData)              (pUART->DAT = wData)

#define HAL_UART_RX_IS_FULL(pUARTx)                 (pUARTx->STS & USART_STS_RXDNE)
#define HAL_UART_RX_IS_INDLE(pUARTx)                (pUARTx->STS & USART_STS_IDLEF )
#define HAL_UART_RX_INDLE_CLR(pUARTx)                (pUARTx->STS &= ~USART_STS_IDLEF)


#define HAL_UART_TX_IS_EMPTY(pUARTx)                  (pUARTx->STS & USART_STS_TXDE  )
#define HAL_UART_TX_IS_DONE(pUARTx)                   (pUARTx->STS & USART_STS_TXC   )
#define HAL_UART_TX_DONE_CLR(pUARTx)                  (pUARTx->STS &= ~USART_STS_TXC)


#define UART_FLAG_ERROR      (USART_STS_PEF|USART_STS_FEF|USART_STS_NEF|USART_STS_OREF)

//#define HAL_UART_FLAG_ERROR(UARTx)                   (UARTx->STS & USART_STS_TXC   != 0)
#define HAL_UART_ERROR_PEF(pUARTx)                       (pUARTx->STS & USART_STS_PEF  )
#define HAL_UART_ERROR_FEF(pUARTx)                       (pUARTx->STS & USART_STS_FEF )
#define HAL_UART_ERROR_NEF(pUARTx)                       (pUARTx->STS & USART_STS_NEF  )
#define HAL_UART_ERROR_OREF(pUARTx)                      (pUARTx->STS & USART_STS_OREF )
#define HAL_UART_IS_ERROR(pUARTx)                        (pUARTx->STS & UART_FLAG_ERROR )
#define HAL_UART_ERROR_CLR(pUARTx)                       (pUARTx->STS &= ~UART_FLAG_ERROR )
#define HAL_UART_FLAG_CLR(pUARTx)                        (pUARTx->STS = 0)

#define HAL_UART_RX_DATA(pUARTx)                         (pUARTx->DAT)
#define HAL_UART_TX_DATA(pUARTx,wData)                   (pUARTx->DAT = wData)

//#define HalUartReadData(pUART)                      (pUART->DAT)
//#define HalUartWriteData(pUART, wData)              (pUART->DAT = wData)

#define HalUartReadData(pUART)                      (pUART->DAT)
#define HalUartWriteData(pUART, wData)              (pUART->DAT = wData)

#define MACRO_integerdivider(pBaudRate)    ((25 * 36000000) / (4 * pBaudRate))
#define MACRO_tmpregister(pBaudRate)       ((MACRO_integerdivider(pBaudRate) / 100) << 4)
#define MACRO_fractionaldivider(pBaudRate) (MACRO_integerdivider(pBaudRate) - (100 * (MACRO_tmpregister(pBaudRate) >> 4)))
#define MACRO_UART_SET_BAUDRATE(pUSART,pBaudRate)   pUSART->BRCF = MACRO_tmpregister(pBaudRate) | ((((MACRO_fractionaldivider(pBaudRate) * 16) + 50) / 100)) & ((uint8_t)0x0F)




typedef INT8U (*pUARTTx)(INT8U bUARTx, INT8U *pbBuffer, INT16U *wTxBufLen, INT16U *wTxCnt);
typedef INT8U (*pUARTCheckDataFrame)(INT8U *pbReTimer, INT16U *pwRxCnt, INT8U bBandRate);
typedef void (*pUARTSendCOM)(USART_Module *pUART, INT8U *bData, INT16U wDataLen);
typedef INT16U (*pIcpCmdAnalysis)(INT8U *pbDataIn, INT16U wDataInLen, INT8U *pbDataOut,
                                  UpdateUARTInfo *pstUARTInfo,
                                  UARTTransInfo *pstTransInfo,
                                  UpdatePackInfo *pstPackInfo,
                                  DevTreeInfo *pstDevTreeInfo);
typedef INT16U (*pSubsetICPCmdAnalysis)(INT8U *pbDataIn, INT16U wDataInLen,
                                        UARTTransInfo *pstTransInfo,
                                 UpdateUARTInfo *pstUARTInfo,
                                        DevTreeInfo *pstDevTreeInfo);
typedef INT16U (*pCheckToTransparentCmd)(UARTTransInfo *pstTransInfo,
                                         UpdateUARTInfo *pstUARTInfo,
                                         INT8U *pbDataOut,
                                         INT8U *pbICPRxBuf,
                                         INT16U wICPRxDataLen);
typedef INT16U (*pCheckToResponseCmd)(UARTTransInfo *pstResInfo,
                                      UpdateUARTInfo *pstUARTInfo,
                                      INT8U *pbDataOut,
                                      INT8U *pbTxDataBuf,
                                      INT16U wDataLen,
                                      DevTreeInfo *pstDevTreeInfo);
typedef INT16U (*psBootDeviceTreeInit)(INT8U bCommuAddr);

//void sBootUart1Init(INT16U wBaudrate);
//void sBootUart2Init(INT16U wBaudrate);
void sBootUARTInit(void);
//void sBootUart1Rs485TxEnPinInit(void);
//void sBootUart2Rs485TxEnPinInit(void);

void sBootUARTCodeStart(void);

void sBootUARTICPProcess(void);
void sBootUARTTx(INT8U bUARTx, INT8U *pbBuffer, INT16U *wTxBufLen, INT16U *wTxCnt);
INT8U sBootUARTCheckDataFrame(INT8U *pbReTimer, INT16U *pwRxCnt, INT8U bBandRate);
INT16U sBootIcpCmdAnalysis(INT8U *pbDataIn, INT16U wDataInLen, INT8U *pbDataOut,
                           UpdateUARTInfo *pstUARTInfo,
                           UARTTransInfo *pstTransInfo,
                           UpdatePackInfo *pstPackInfo,
                           DevTreeInfo *pstDevTreeInfo);

INT16U sBootSubsetICPCmdAnalysis(INT8U *pbDataIn, INT16U wDataInLen,
                                 UARTTransInfo *pstTransInfo,
                                 UpdateUARTInfo *pstUARTInfo,
                                 DevTreeInfo *pstDevTreeInfo);

INT16U sCheckToTransparentCmd(UARTTransInfo *pstTransInfo,
                              UpdateUARTInfo *pstUARTInfo,
                              INT8U *pbDataOut,
                              INT8U *pbICPRxBuf,
                              INT16U wICPRxDataLen);

INT16U sCheckToResponseCmd(UARTTransInfo *pstResInfo,
                           UpdateUARTInfo *pstUARTInfo,
                           INT8U *pbDataOut,
                           INT8U *pbTxDataBuf,
                           INT16U wDataLen,
                           DevTreeInfo *pstDevTreeInfo);
//void sBootUARTSendCOM(USART_Module *pUART, INT8U *bData, INT16U wDataLen);
void sBootDeviceTreeInit(INT8U bCommuAddr);


void sBootParitySet(USART_Module* USARTx, INT8U bParity);



void sBootBaudRateSet(USART_Module* USARTx, INT32U dwBaudRate);


void sBootUARTCodeEnd(void);
typedef void (*t_BaudRateSet)(USART_Module* USARTx, INT32U dwBaudRate);
#define macro_BaudRateSet  ((t_BaudRateSet)BootUARTFunc(sBootBaudRateSet))
#define macro_UARTICPProcess ((psVoidFunc)(BootUARTFunc(sBootUARTICPProcess)))

#endif


