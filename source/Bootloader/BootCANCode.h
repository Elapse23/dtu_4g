/************************************************************************************************
File Name:      BootCANCode.C
Description:    bootloader CAN operation code
Version:        V1.0
Author:
Date:           2020-06-02
*************************************************************************************************/
#ifndef BOOT_CANCODE_H
#define BOOT_CANCODE_H

//#include "MCU_Initial.h"
#include "Bootloader.H"
//#define  BOOT_CAN_MODE

#ifdef BOOT_CAN_MODE
#define Boot_RX_Active          4
#define Boot_TX_Inactive        8
#define Boot_TX_Active          12

#define Boot_RX_ICP             0
#define Boot_TX_ICP             1
#define Boot_RX_TRS             2
#define Boot_TX_TRS             3

#define BootStdFrame            0
#define BootExtFrame            1
#define BootFrame               BootExtFrame
typedef union
{
    INT8U Msg[8];
    struct
    {
        INT8U   FuncCode;
        INT16U  Length_H        :8;
        INT16U  Length_L        :8;
        INT8U   data[5];
    }fir;
    struct
    {
        INT8U  FuncCode;
        INT16U  Cnt_H           :8;
        INT16U  Cnt_L           :8;
        INT8U   data[5];
    }sec;
}boot_can_msg;

typedef struct
{
    INT32U dwCanId;
    INT8U b_can_msg_len;
    boot_can_msg st_boot_can_msg;
}boot_can_msg_info;
#define cbCAN0Func                              cbICPTransFunc//cbICPTransFunc//cbTransFunc//cbICPFunc
#define cbCAN1Func                              cbICPTransFunc//cbICPTransFunc//cbTransFunc//cbICPFunc
#define cbCAN2Func                              cbNULLFunc//cbICPTransFunc//cbTransFunc//cbICPFunc

/* select CAN0 pin */
#define fCAN0Enable                             true
#define CAN0_GPIO                               PTE
#define CAN0_PORT                               PORTE
#define CAN0_PortPCC                            PCC_PORTE_INDEX
#define CAN0_FlexCAN                            PCC_FlexCAN0_INDEX
#define CAN0_Tx_Pin                             5
#define CAN0_Rx_Pin                             4
#define CAN0_PinMux                             5

/* select CAN1 pin */
#define fCAN1Enable                             true
#define CAN1_GPIO                               PTA
#define CAN1_PORT                               PORTA
#define CAN1_PortPCC                            PCC_PORTA_INDEX
#define CAN1_FlexCAN                            PCC_FlexCAN1_INDEX
#define CAN1_Tx_Pin                             13
#define CAN1_Rx_Pin                             12
#define CAN1_PinMux                             3

/* select CAN2 pin */
#define fCAN2Enable                             false
#define CAN2_GPIO                               PTC
#define CAN2_PORT                               PORTC
#define CAN2_PortPCC                            PCC_PORTC_INDEX
#define CAN2_FlexCAN                            PCC_FlexCAN2_INDEX
#define CAN2_Tx_Pin                             17
#define CAN2_Rx_Pin                             16
#define CAN2_PinMux                             3
//CAN
#define BootCAN0                                   CAN0
#define BootCAN1                                   CAN1
#define BootCAN2                                   CAN2


#define cbCAN0                                  3
#define cbCAN1                                  4
#define cbCAN2                                  5



typedef INT8U (*pBootCAN_Rx)(CAN_Type *pCAN,INT8U bBufNum,INT8U *pbBufOut);
typedef void (*pCANSendSec)(CAN_Type *pCAN, INT8U bBufNum,BootCANSecInfo *pstBootCANSec);
typedef INT16U (*pCANReceive)(INT8U *pbRxBuf, INT8U *pbBufOut, BootCANRxInfo *pstBootCANRx);
typedef void (*pUARTToCANSend)(CAN_Type *pCAN, INT8U bBufNum, INT8U *pbBuffer, INT16U wlength, INT8U bCHFun, BootCANSecInfo *pstBootCANSec);

void sBootCANInit(void);

void sBootCANCodeStart(void);
INT8U sBootCAN_Rx(CAN_Type *pCAN,INT8U bBufNum,INT8U *pbBufOut);
void sBootCANSendSec(CAN_Type *pCAN, INT8U bBufNum,BootCANSecInfo *pstBootCANSec);
INT16U sBootCANReceive(INT8U *pbRxBuf, INT8U *pbBufOut, BootCANRxInfo *pstBootCANRx);
void sBootUARTToCANSend(CAN_Type *pCAN, INT8U bBufNum, INT8U *pbBuffer, INT16U wlength, INT8U bCHFun, BootCANSecInfo *pstBootCANSec);
void sBootCANCodeEnd(void);
#endif
#endif


