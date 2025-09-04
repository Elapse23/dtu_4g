/************************************************************************************************
File Name:      BootCANCode.C
Description:    bootloader CAN operation code
Version:        V1.0
Author:         
Date:           2020-06-02
*************************************************************************************************/
#include "BootCANCode.H"
#include "BootUARTCode.H"
#include "BootFlashCode.H"


#ifdef BOOT_CAN_MODE

extern void sAppOkMark(void);

typedef INT8U (*pCANTxSend)(CAN_Type *pCAN, INT8U bBufNum, INT8U *pbBuffer);
typedef void (*pCANSendData)(CAN_Type *pCAN, INT8U bBufNum,INT32U dwID, INT8U bCmdType, INT16U wLength, INT32U dwListAddress, BootCANSecInfo *pstBootCANSec);
typedef INT16U (*pCANReceiveData)(INT8U *pbRxData,INT32U dwListAddress,BootCANRxInfo *pstBootCANRx);
typedef INT16U (*pCANToUART)(INT8U *pbBuffer, INT16U wlength);


static INT8U sBootCANTxSend(CAN_Type *pCAN, INT8U bBufNum, INT8U *pbBuffer);
static void sBootCANSendData(CAN_Type *pCAN, INT8U bBufNum,INT32U dwID, INT8U bCmdType, INT16U wLength, INT32U dwListAddress, BootCANSecInfo *pstBootCANSec);
static INT16U sBootCANReceiveData(INT8U *pbRxData,INT32U dwListAddress,BootCANRxInfo *pstBootCANRx);
static INT16U sBootCANToUART(INT8U *pbBuffer, INT16U wlength);

/*************************************************************************************
* Input: 
*
* Output: void
* 
* Logic discription: bootloader CAN initialization
*
* run time:
*************************************************************************************/
const INT16U cwBootCANBaudTab[cbSetCANBaudRateMax] = 
{
    cwBOOT_CAN_BAUD_500K,
    cwBOOT_CAN_BAUD_250K,
    cwBOOT_CAN_BAUD_125K,
    cwBOOT_CAN_BAUD_100K,
    cwBOOT_CAN_BAUD_50K,
};

void sBootCAN_IDConfig(CAN_Type *pCAN, INT8U BufNum, INT8U CANRTX,INT8U CANIDE,INT32U MaskCode,INT32U ID)
{
    pCAN->RXIMR[BufNum] = MaskCode;                             //Check all ID bits for incoming messages
    
    M_BOOT_CAN_RXIMR(pCAN,BufNum,MaskCode); 
    if(CANIDE == 0)
    {
        M_BOOT_CAN_RAMn(pCAN,BufNum*4,(CANRTX << 24));
        M_BOOT_CAN_RAMn(pCAN,BufNum*4+1,(ID << 18));
    }
    else if(CANIDE == 1)
    {
        M_BOOT_CAN_RAMn(pCAN,BufNum*4,((CANRTX << 24)|(CANIDE << 21)));
        M_BOOT_CAN_RAMn(pCAN,BufNum*4+1,ID);
    }
	
    M_BOOT_CAN_RAMn(pCAN,BufNum*4+2,0);
    M_BOOT_CAN_RAMn(pCAN,BufNum*4+3,0);
}

void sBootCANxInit(INT8U bCANx, INT8U bCANBaud)
{        
    INT8U bCANbrp;
    INT32U dwCANCTRL1;
	
    if(bCANBaud > cbSetCANBaudRateMax)
    {
        bCANBaud = cbCAN0DefaultBaudrate;
    }
    bCANbrp = cwBootCANBaudTab[bCANBaud];
    dwCANCTRL1 = (bCANbrp << 24) | 0x00DB8006;
	
    if(cbCAN0 == bCANx)
    {
#if (fCAN0Enable == true)
        M_BOOT_CLOCK_ENABLE(CAN0_PortPCC); //enable Pin clock
        M_BOOT_PORT_SET_PIN_MUX(CAN0_PORT, CAN0_Rx_Pin, CAN0_PinMux); //SET mux to RX
        M_BOOT_PORT_SET_PIN_MUX(CAN0_PORT, CAN0_Tx_Pin, CAN0_PinMux); //SET mux TX

        //CAN init
        M_BOOT_CLOCK_ENABLE(CAN0_FlexCAN); 
		        
        M_BOOT_CAN_MCR(BootCAN0,0xD890000F); 
        M_BOOT_CAN_MCR_OR(BootCAN0,CAN_MCR_MDIS_MASK); 
        M_BOOT_CAN_CTRL1_AND(BootCAN0,(~CAN_CTRL1_CLKSRC_MASK)); 
        M_BOOT_CAN_MCR_AND(BootCAN0,(~CAN_MCR_MDIS_MASK)); 

        while (!(M_BOOT_CAN_MCR_R(BootCAN0) & CAN_MCR_FRZACK_MASK));                                 
        M_BOOT_CAN_CTRL1(BootCAN0,dwCANCTRL1); 

        sBootCAN_IDConfig(BootCAN0,Boot_RX_ICP,Boot_RX_Active,BootFrame,0x1FFF00FF,0x18A000F7); //ICP Rx
        sBootCAN_IDConfig(BootCAN0,Boot_TX_ICP,Boot_TX_Inactive,BootFrame,0x1FFFFFFF,(0x18A0F700+(cbCAN0Addr << 8))); //ICP Tx
        
        for(INT8U i = 2;i < 32;i++)
        {
            sBootCAN_IDConfig(BootCAN0,i,0,0,0x1FFC0000,0);// 2-31Ô¤Áô
        }
	
        M_BOOT_CAN_MCR(BootCAN0,0x0001001F); 
        while (M_BOOT_CAN_MCR_R(BootCAN0) & CAN_MCR_FRZACK_MASK);                                 
        while (M_BOOT_CAN_MCR_R(BootCAN0) & CAN_MCR_NOTRDY_MASK);      
		
#endif  //#if (fCAN0Enable == true)
    }
    else if(cbCAN1 == bCANx)
    {
#if (fCAN1Enable == true)
        M_BOOT_CLOCK_ENABLE(CAN1_PortPCC); //enable Pin clock
        M_BOOT_PORT_SET_PIN_MUX(CAN1_PORT, CAN1_Rx_Pin, CAN1_PinMux); //SET mux to RX
        M_BOOT_PORT_SET_PIN_MUX(CAN1_PORT, CAN1_Tx_Pin, CAN1_PinMux); //SET mux TX

        //CAN init
        M_BOOT_CLOCK_ENABLE(CAN1_FlexCAN); 
		        
        M_BOOT_CAN_MCR(BootCAN1,0xD890000F); 
        M_BOOT_CAN_MCR_OR(BootCAN1,CAN_MCR_MDIS_MASK); 
        M_BOOT_CAN_CTRL1_AND(BootCAN1,(~CAN_CTRL1_CLKSRC_MASK)); 
        M_BOOT_CAN_MCR_AND(BootCAN1,(~CAN_MCR_MDIS_MASK)); 

        while (!(M_BOOT_CAN_MCR_R(BootCAN1) & CAN_MCR_FRZACK_MASK));                                 
        M_BOOT_CAN_CTRL1(BootCAN1,dwCANCTRL1); 
		
        sBootCAN_IDConfig(BootCAN1,Boot_RX_ICP,Boot_RX_Active,BootFrame,0x1FFF00FF,0x18A000F7); //ICP Rx
        sBootCAN_IDConfig(BootCAN1,Boot_TX_ICP,Boot_TX_Inactive,BootFrame,0x1FFFFFFF,(0x18A0F700+(cbCAN1Addr << 8))); //ICP Tx
        
        for(INT8U i = 2;i < 16;i++)
        {
            sBootCAN_IDConfig(BootCAN1,i,0,0,0x1FFC0000,0);// 2-15Ô¤Áô
        }

        M_BOOT_CAN_MCR(BootCAN1,0x0001001F); 
        while (M_BOOT_CAN_MCR_R(BootCAN1) & CAN_MCR_FRZACK_MASK);                                 
        while (M_BOOT_CAN_MCR_R(BootCAN1) & CAN_MCR_NOTRDY_MASK);                                 

#endif  //#if (fCAN1Enable == true)
    }
    else if(cbCAN2 == bCANx)
    {
#if (fCAN2Enable == true)
        M_BOOT_CLOCK_ENABLE(CAN2_PortPCC); //enable Pin clock
        M_BOOT_PORT_SET_PIN_MUX(CAN2_PORT, CAN2_Rx_Pin, CAN2_PinMux); //SET mux to RX
        M_BOOT_PORT_SET_PIN_MUX(CAN2_PORT, CAN2_Tx_Pin, CAN2_PinMux); //SET mux TX

        //CAN init
        M_BOOT_CLOCK_ENABLE(CAN2_FlexCAN); 
		        
        M_BOOT_CAN_MCR(BootCAN2,0xD890000F); 
        M_BOOT_CAN_MCR_OR(BootCAN2,CAN_MCR_MDIS_MASK); 
        M_BOOT_CAN_CTRL1_AND(BootCAN2,(~CAN_CTRL1_CLKSRC_MASK)); 
        M_BOOT_CAN_MCR_AND(BootCAN2,(~CAN_MCR_MDIS_MASK)); 

        while (!(M_BOOT_CAN_MCR_R(BootCAN2) & CAN_MCR_FRZACK_MASK));                                 
        M_BOOT_CAN_CTRL1(BootCAN2,dwCANCTRL1); 
		
        sBootCAN_IDConfig(BootCAN2,Boot_RX_ICP,Boot_RX_Active,BootFrame,0x1FFF00FF,0x18A000F7); //ICP Rx
        sBootCAN_IDConfig(BootCAN2,Boot_TX_ICP,Boot_TX_Inactive,BootFrame,0x1FFFFFFF,(0x18A0F700+(cbCAN2Addr << 8))); //ICP Tx
        
        for(INT8U i = 2;i < 16;i++)
        {
            sBootCAN_IDConfig(BootCAN2,i,0,0,0x1FFC0000,0);// 2-15Ô¤Áô
        }

        M_BOOT_CAN_MCR(BootCAN2,0x0001001F); 
        while (M_BOOT_CAN_MCR_R(BootCAN2) & CAN_MCR_FRZACK_MASK);                                 
        while (M_BOOT_CAN_MCR_R(BootCAN2) & CAN_MCR_NOTRDY_MASK);                                 

#endif  //#if (fCAN2Enable == true)
    }    
}

void sBootCANInit(void)
{
#if (fCAN0Enable == true)
    sBootCANxInit(cbCAN0, (pstBootUARTConfig->bBaudrate)[cbCAN0]);
#endif  //#if (fCAN0Enable == true)
    
#if (fCAN1Enable == true)
    sBootCANxInit(cbCAN1, (pstBootUARTConfig->bBaudrate)[cbCAN1]);
#endif  //#if (fCAN1Enable == true)

#if (fCAN2Enable == true)
    sBootCANxInit(cbCAN2, (pstBootUARTConfig->bBaudrate)[cbCAN2]);
#endif  //#if (fCAN2Enable == true)
                                                                                
}
/************************************************************************************************
* following functions can be copy to RAM (... ~ sBootCANCodeEnd())
*************************************************************************************************/
void sBootCANCodeStart(void)
{
}

/************************************************************************************************
* Input: pstBootCANSec: point of CAN continuous send info;
*
* Output: 
*
* Logic discription: can buffer send
*
* run time:
************************************************************************************************/
INT8U sBootCAN_Rx(CAN_Type *pCAN,INT8U bBufNum,INT8U *pbBufOut)
{
    BootCanMsgType_Func *CanMsg;
		
    if(pCAN->IFLAG1 & (1 << Boot_RX_ICP))
    {
        pCAN->IFLAG1 = (1 << Boot_RX_ICP);
        CanMsg = (BootCanMsgType_Func *)(&pCAN->RAMn[Boot_RX_ICP * 4]);
        
        if(((*CanMsg).bit.ID == (0x18A000F7 + ((stUpdateUARTInfo.bRs485Addr) << 8)))||
           ((*CanMsg).bit.ID == 0x18A0F7F7))
        {
            if(Boot_RX_ICP == bBufNum)
            {	
                for(INT8U j = 0; j < 16; j++)
                {
                    pbBufOut[j] = (*CanMsg).Msg[j];            
                }
                (*CanMsg).bit.Status = Boot_RX_Active;
                return 1;
            }     
        } 
    }	
        
    return 0;
}

/************************************************************************************************
* Input: pbBuffer: point of CAN send info;
*
* Output: 
*
* Logic discription: can buffer
*
* run time:
************************************************************************************************/
static INT8U sBootCANTxSend(CAN_Type *pCAN, INT8U bBufNum, INT8U *pbBuffer)
{
    BootCanMsgType_Func *CanMsg;
    CanMsg = (BootCanMsgType_Func *)(&pCAN->RAMn[bBufNum*4]);

    if((*CanMsg).bit.Status == Boot_TX_Inactive)
    {
        for(INT8U i = 0; i < 16; i++)
        {
            (*CanMsg).Msg[i] = pbBuffer[i];            
        }	
        (*CanMsg).bit.Status = Boot_TX_Active;
        return 1;
    }
        
    return 0;
}

/************************************************************************************************
* Input: dwID: send ID; bCmdType: function code; wLength:send data and CRC16 length;
*        dwListAddress: point of send data address;pstBootCANSec: point of CAN continuous send info;
*
* Output: 
*
* Logic discription: can data send
*
* run time:
************************************************************************************************/
static void sBootCANSendData(CAN_Type *pCAN, INT8U bBufNum,INT32U dwID, INT8U bCmdType, INT16U wLength, INT32U dwListAddress, BootCANSecInfo *pstBootCANSec)
{
    INT8U i;
    BootCanMsgType_Func CanMsg_FuncTx;

    pstBootCANSec->dwAddressOffset = dwListAddress;
    CanMsg_FuncTx.fir.Reserve1 = 0;
    CanMsg_FuncTx.fir.Reserve2 = 0;

    CanMsg_FuncTx.fir.CmdType = bCmdType;
    CanMsg_FuncTx.fir.Length = (wLength - 2);//CRC
    for(i = 0; i < 5; i++)
    {
        if(i < wLength)
        {
            if(i == 0)
            {
                CanMsg_FuncTx.fir.DataL[0] = *((INT8U*)pstBootCANSec->dwAddressOffset);
            }
            else
            {
                CanMsg_FuncTx.fir.DataH[4-i] = *((INT8U*)pstBootCANSec->dwAddressOffset);
            }
            pstBootCANSec->dwAddressOffset++;
        }
        else
        {
            break;
        }
    }
    CanMsg_FuncTx.bit.DLC = 3 + i;
    CanMsg_FuncTx.bit.ID = dwID;
    CanMsg_FuncTx.bit.IDE = BootFrame;

    pstBootCANSec->dwID = dwID;
    pstBootCANSec->fIDE = BootFrame;

    if(1 == ((pCANTxSend)BootCANFunc(sBootCANTxSend))(pCAN,bBufNum,CanMsg_FuncTx.Msg))
    {
        if(wLength > 5)
        {
            pstBootCANSec->bFuncCode = bCmdType + 0x10;
            pstBootCANSec->wLength = wLength;
            pstBootCANSec->fSecSend = true;
            pstBootCANSec->wSendCnt = 0;
            pCAN->IFLAG1 = (1 << bBufNum);          
            pCAN->IMASK1 |= (1 << bBufNum);         
        }
        else
        {
            pstBootCANSec->bFuncCode = 0;
            pstBootCANSec->wLength = 0;
            pstBootCANSec->fSecSend = false;
            pstBootCANSec->wSendCnt = 0;
        }
    }
			    
}

/************************************************************************************************
* Input: pstBootCANSec: point of CAN continuous send info;
*
* Output: 
*
* Logic discription: can data continuous send
*
* run time:
************************************************************************************************/
void sBootCANSendSec(CAN_Type *pCAN, INT8U bBufNum,BootCANSecInfo *pstBootCANSec)
{
    INT8U i;
    BootCanMsgType_Func CanMsg_FuncTx;

    if(pstBootCANSec->fSecSend == true)
    {
        pstBootCANSec->dwAddressOffsetLast = pstBootCANSec->dwAddressOffset;
        CanMsg_FuncTx.sec.Reserve1 = 0;
        CanMsg_FuncTx.sec.Reserve2 = 0;
		
        CanMsg_FuncTx.sec.CmdType = pstBootCANSec->bFuncCode;
        CanMsg_FuncTx.sec.Cnt = pstBootCANSec->wSendCnt;
        for(i = 0; i < 5; i++)
        {
            if(i + pstBootCANSec->wSendCnt * 5 + 5 < pstBootCANSec->wLength)
            {
                if(i == 0)
                {
                    CanMsg_FuncTx.sec.DataL[0] = *((INT8U*)pstBootCANSec->dwAddressOffset);
                }
                else
                {
                    CanMsg_FuncTx.sec.DataH[4-i] = *((INT8U*)pstBootCANSec->dwAddressOffset);
                }

                pstBootCANSec->dwAddressOffset++;
            }
            else
            {
                break;
            }
        }
        CanMsg_FuncTx.bit.DLC = 3 + i;
        CanMsg_FuncTx.bit.ID = pstBootCANSec->dwID;
        CanMsg_FuncTx.bit.IDE = pstBootCANSec->fIDE;
        
        if(1 == ((pCANTxSend)BootCANFunc(sBootCANTxSend))(pCAN,bBufNum,CanMsg_FuncTx.Msg))
        {
            if(i + pstBootCANSec->wSendCnt * 5 + 5 < pstBootCANSec->wLength)
            {
                pstBootCANSec->wSendCnt++;
                pCAN->IFLAG1 = (1 << bBufNum);          
                pCAN->IMASK1 |= (1 << bBufNum);         
            }
            else
            {
                pstBootCANSec->fSecSend = false;
                pstBootCANSec->wSendCnt = 0;
            }
        }
        else
        {
            pstBootCANSec->dwAddressOffset = pstBootCANSec->dwAddressOffsetLast;
        }
    }
}

/************************************************************************************************
* Input: pbRxData: receive can buffer;dwListAddress: point of receive data address;
*
* Output: data length
*
* Logic discription: can data receive
*
* run time:
************************************************************************************************/
INT16U sBootCANReceive(INT8U *pbRxBuf, INT8U *pbBufOut, BootCANRxInfo *pstBootCANRx)
{
    INT16U wReceiveLen = 0;
	
    wReceiveLen = ((pCANReceiveData)BootCANFunc(sBootCANReceiveData))(pbRxBuf,(INT32U)(&pbBufOut[0]),pstBootCANRx);

    if(wReceiveLen)
    {
        wReceiveLen = ((pCANToUART)BootCANFunc(sBootCANToUART))(pbBufOut,wReceiveLen);
    }		
	
    return wReceiveLen;	
}

static INT16U sBootCANReceiveData(INT8U *pbRxData, INT32U dwListAddress, BootCANRxInfo *pstBootCANRx)
{
    INT8U i;
    INT8U bLen;
    INT16U wReceiveLen = 0;	
    BootCanMsgType_Func *pBoot;
    pBoot = (BootCanMsgType_Func *)(&pbRxData[0]);   
	
    bLen = ((*pBoot).bit.DLC & 0xFF) - 3;

    if(((*pBoot).fir.CmdType < 0x09)||((((*pBoot).fir.CmdType >= 0x80))&&((*pBoot).fir.CmdType < 0x89)))
    {        
        pstBootCANRx->wLength = (*pBoot).fir.Length + 2;
        pstBootCANRx->dwAddress = dwListAddress;
		
        if(0xF7F7 == ((*pBoot).bit.ID & 0xFFFF))
        {
            *((INT8U*)pstBootCANRx->dwAddress) = 0xF7;
        }
        else if(0xF7 == ((*pBoot).bit.ID & 0x00FF))
        {
            *((INT8U*)pstBootCANRx->dwAddress) = ((*pBoot).bit.ID & 0xFF00) >> 8;
        }
        else if(0xF700 == ((*pBoot).bit.ID & 0xFF00))
        {
            *((INT8U*)pstBootCANRx->dwAddress) = (*pBoot).bit.ID & 0xFF;
        }
        *((INT8U*)pstBootCANRx->dwAddress + 1) = 0xFF;
        *((INT8U*)pstBootCANRx->dwAddress + 2) = (*pBoot).fir.CmdType & 0xF0;
        *((INT8U*)pstBootCANRx->dwAddress + 3) = (*pBoot).fir.CmdType & 0x0F;
        *((INT8U*)pstBootCANRx->dwAddress + 4) = (*pBoot).fir.Length >> 8;
        *((INT8U*)pstBootCANRx->dwAddress + 5) = (*pBoot).fir.Length & 0xFF;	
		
        pstBootCANRx->dwAddress = dwListAddress + 6;

        for(i = 0; i < bLen; i++)
        {
            if(i == 0)
            {
                *((INT8U*)pstBootCANRx->dwAddress) = (*pBoot).fir.DataL[0];
            }
            else
            {
                *((INT8U*)pstBootCANRx->dwAddress) = (*pBoot).fir.DataH[4-i];
            }
            pstBootCANRx->dwAddress++;
        } 
		
        if(pstBootCANRx->wLength > 5)
        {
            pstBootCANRx->bFuncCode = (*pBoot).fir.CmdType + 0x10;
            pstBootCANRx->wRevCnt = 0;
        }
        else
        {
            wReceiveLen = pstBootCANRx->wLength - 2;
        }
		
    }
    else if((((*pBoot).sec.CmdType >= 10)&&((*pBoot).sec.CmdType < 0x19))||((((*pBoot).sec.CmdType >= 0x90))&&((*pBoot).sec.CmdType < 0x99)))
    {
        if(((*pBoot).sec.Cnt == pstBootCANRx->wRevCnt)&&((*pBoot).sec.CmdType == pstBootCANRx->bFuncCode))
        {
            for(i = 0; i < bLen; i++)
            {
                if(i == 0)
                {
                    *((INT8U*)pstBootCANRx->dwAddress) = (*pBoot).sec.DataL[0];
                }
                else
                {
                    *((INT8U*)pstBootCANRx->dwAddress) = (*pBoot).sec.DataH[4-i];
                }
                pstBootCANRx->dwAddress++;
            }
			
            if(i + pstBootCANRx->wRevCnt * 5 + 5 >= pstBootCANRx->wLength)
            {
                pstBootCANRx->wRevCnt = 0;
                pstBootCANRx->bFuncCode = 0xFF;
                wReceiveLen = pstBootCANRx->wLength - 2;
            }
            else
            {
                pstBootCANRx->wRevCnt++;
            }
        }
        else if(((*pBoot).sec.Cnt > pstBootCANRx->wRevCnt)||((*pBoot).sec.CmdType != pstBootCANRx->bFuncCode))
        {
            pstBootCANRx->wRevCnt = 0xFFFF;
        }
    }
	
    return wReceiveLen;	
}
/************************************************************************************************
* Input: pbBuffer: data of CAN format;wlength: data length of CAN format;
*
* Output: 
*
* Logic discription:  check data and add CRC of UART format 
*
* run time:
************************************************************************************************/
static INT16U  sBootCANToUART(INT8U *pbBuffer, INT16U wlength)
{ 
    INT16U wTemp;
    if(0 == ((pCRC16_2)BootFlashFunc(CRC16_2))(pbBuffer + 6, wlength + 2))
    {
        //add CRC16
        wTemp = ((pCRC16_2)BootFlashFunc(CRC16_2))(pbBuffer,wlength + 6);
        pbBuffer[wlength + 6] = (INT8U)(wTemp & 0x00FF);
        pbBuffer[wlength + 6 + 1] = (INT8U)((wTemp & 0xFF00) >> 8);

        return (wlength + 8);
    }
    return 0;
}
/************************************************************************************************
* Input: pbBuffer: data of UART format;wlength: length of UART format(All length);pstBootCANSec: point of CAN continuous send info;
*
* Output: 
*
* Logic discription:  UART format convert to CAN format
*
* run time:
************************************************************************************************/
void sBootUARTToCANSend(CAN_Type *pCAN, INT8U bBufNum, INT8U *pbBuffer, INT16U wlength, INT8U bCHFun, BootCANSecInfo *pstBootCANSec)
{
    INT32U dwSendID;
    INT16U wTemp;

    if(wlength < 6)
    {
        return;
    }
    if(cbICPFunc == bCHFun)
    {
        dwSendID = pbBuffer[0] + 0x18A0F700;//dwSendID = pbBuffer[0] + 0x600;
    }
    else if(cbTransFunc == bCHFun)
    {
        dwSendID = (pbBuffer[0] << 8) + 0x18A000F7;//dwSendID = pbBuffer[0] + 0x700;
    }
    
    pbBuffer[0] = (pbBuffer[2] | pbBuffer[3]);

    for(INT16U i = 0;i < (wlength - 6);i++)//data and data length(-4-2)
    {
        pbBuffer[i + 1] = pbBuffer[4 + i];
    }
    
    wTemp = ((pCRC16_2)BootFlashFunc(CRC16_2))(pbBuffer + 3,wlength - 8);
    pbBuffer[wlength - 5] = (INT8U)(wTemp & 0x00FF);
    pbBuffer[wlength - 5 + 1] = (INT8U)((wTemp & 0xFF00) >> 8);

    ((pCANSendData)BootCANFunc(sBootCANSendData))(pCAN,bBufNum,dwSendID,pbBuffer[0],wlength - 6,(INT32U)(&pbBuffer[0]) + 3,pstBootCANSec);
}
void sBootCANCodeEnd(void)
{
}

#enddif
