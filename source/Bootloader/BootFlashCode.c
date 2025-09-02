/************************************************************************************************
File Name:      BootFlashCode.C
Description:    bootloader MCU flash operation code
Version:        V1.0
Author:         chenweijie
Date:           2018-08-24
*************************************************************************************************/


#include "BootFlashCode.H"
#include "BootLoader.h"
//#include "n32g45x_gpio.h"

#include "drv_log.h"


//typedef void (*pReplceAppMask)(INT32U dwFlashAddr, INT8U *pbData, INT16U wDataLen);
//typedef FLASH_STS (*pFLASH_DRV_CommandSequence)(void);
//typedef FLASH_STS (*pFLASH_EraseOnePage)(INT32U dwStartAddr);
//typedef FLASH_STS (*pFLASH_GetSTS)(void);
//typedef FLASH_STS (*pFLASH_WaitForLastOpt)(INT32U Timeout);
//typedef FLASH_STS (*pFLASH_ProgramWord)(INT32U Address, INT32U Data);
//typedef INT16U    (*pFlash_FiramPackOperate)(LocalUpdateSpiFlashFirmwarePacket *pstFirmwarePacket,LocalFirmwareUpdatePackInfo *pstPackInfo);


static void sBootReplceAppMask(INT32U dwFlashAddr, INT8U *pbData, INT16U wDataLen);
static FLASH_STS FLASH_EraseOnePage(INT32U dwStartAddr);


extern void sAppOkMark(void);
#define MACRO_FLASH_ClearFlag(val)          FLASH->STS |= val

#define MACRO_FLASH_Lock()                  FLASH->CTRL |= CTRL_Set_LOCK

#define MACRO_FLASH_Unlock()                do{FLASH->KEY = FLASH_KEY1;\
                                               FLASH->KEY = FLASH_KEY2;}while(0)


//eStatus = ((pFLASH_DRV_EraseSector)BootFlashFunc(BOOT_FLASH_EraseOnePage))(dwSectorStartAddr);

//(void)((pFLASH_Program)BootFlashFunc(sBootFLASH_Program)
/************************************************************************************************
* following functions can be copy to RAM (... ~ sBootFalshCodeEnd())
*************************************************************************************************/
void sBootFalshCodeStart(void)
{

}
/**
 * @brief  Unlocks the FLASH Program Erase Controller.
 * @note   This function can be used for N32G45X devices.
 *         - For N32G45X devices this function unlocks Bank1.
 *           to FLASH_UnlockBank1 function..
 */
//void FLASH_Unlock(void)
//{
//    /* Authorize the FPEC of Bank1 Access */
//    FLASH->KEY = FLASH_KEY1;
//    FLASH->KEY = FLASH_KEY2;
//}

/**
 * @brief  Locks the FLASH Program Erase Controller.
 * @note   This function can be used for N32G45X devices.
 *         - For N32G45X devices this function Locks Bank1.
 *           to FLASH_LockBank1 function.
 */
//void FLASH_Lock(void)
//{
//    /* Set the Lock Bit to lock the FPEC and the CTRL of  Bank1 */
//    FLASH->CTRL |= CTRL_Set_LOCK;
//}
/**
 * @brief  Returns the FLASH Status.
 * @note   This function can be used for N32G45X devices, it is equivalent
 *         to FLASH_GetBank1Status function.
 * @return FLASH Status: The returned value can be: FLASH_BUSY,
 *         FLASH_ERR_PG, FLASH_ERR_PV, FLASH_ERR_WRP, FLASH_COMPL,
 *         FLASH_ERR_EV or FLASH_TIMEOUT.
 */
FLASH_STS BOOT_FLASH_GetSTS(void)
{
    FLASH_STS flashstatus = FLASH_COMPL;

    if ((FLASH->STS & FLASH_FLAG_BUSY) == FLASH_FLAG_BUSY)
    {
        flashstatus = FLASH_BUSY;
    }
    else
    {
        if ((FLASH->STS & FLASH_FLAG_PGERR) != 0)
        {
            flashstatus = FLASH_ERR_PG;
        }
        else
        {
            if ((FLASH->STS & FLASH_FLAG_PVERR) != 0)
            {
                flashstatus = FLASH_ERR_PV;
            }
            else
            {
                if ((FLASH->STS & FLASH_FLAG_WRPERR) != 0)
                {
                    flashstatus = FLASH_ERR_WRP;
                }
                else
                {
                    if ((FLASH->STS & FLASH_FLAG_EVERR) != 0)
                    {
                        flashstatus = FLASH_ERR_EV;
                    }
                    else
                    {
                        flashstatus = FLASH_COMPL;
                    }
                }
            }
        }
    }

    /* Return the Flash Status */
    return flashstatus;
}

/**
 * @brief  Waits for a Flash operation to complete or a TIMEOUT to occur.
 * @note   This function can be used for N32G45X devices,
 *         it is equivalent to FLASH_WaitForLastBank1Operation..
 * @param Timeout FLASH programming Timeout
 * @return FLASH Status: The returned value can be: FLASH_BUSY,
 *         FLASH_ERR_PG, FLASH_ERR_PV, FLASH_ERR_WRP, FLASH_COMPL,
 *         FLASH_ERR_EV or FLASH_TIMEOUT.
 */
FLASH_STS BOOT_FLASH_WaitForLastOpt(INT32U Timeout)
{
    FLASH_STS status = FLASH_COMPL;

    /* Check for the Flash Status */
    status = MACRO_FLASH_GetSTS();
    /* Wait for a Flash operation to complete or a TIMEOUT to occur */
    while ((status == FLASH_BUSY) && (Timeout != 0x00))
    {
        status = MACRO_FLASH_GetSTS();
        Timeout--;
    }
    if (Timeout == 0x00)
    {
        status = FLASH_TIMEOUT;
    }
    /* Return the operation status */
    return status;
}



/**
 * @brief  Erases a specified FLASH page.
 * @note   This function can be used for N32G45X devices.
 * @param Page_Address The page address to be erased.
 * @return FLASH Status: The returned value can be: FLASH_BUSY,
 *         FLASH_ERR_PG, FLASH_ERR_PV, FLASH_ERR_WRP, FLASH_COMPL,
 *         FLASH_ERR_EV or FLASH_TIMEOUT.
 */
FLASH_STS BOOT_FLASH_EraseOnePage(INT32U Page_Address)
{
    FLASH_STS status = FLASH_COMPL;
    /* Check the parameters */
    assert_param(IS_FLASH_ADDRESS(Page_Address));

    /* Clears the FLASH's pending flags */
    MACRO_FLASH_ClearFlag(FLASH_STS_CLRFLAG);
    /* Wait for last operation to be completed */
    status = MACRO_FLASH_WaitForLastOpt(EraseTimeout);

    if (status == FLASH_COMPL)
    {
        /* if the previous operation is completed, proceed to erase the page */
        FLASH->CTRL |= CTRL_Set_PER;
        FLASH->ADD = Page_Address;
        FLASH->CTRL |= CTRL_Set_START;

        /* Wait for last operation to be completed */
        status = MACRO_FLASH_WaitForLastOpt(EraseTimeout);

        /* Disable the PER Bit */
        FLASH->CTRL &= CTRL_Reset_PER;
    }

    /* Return the Erase Status */
    return status;
}

/**
 * @brief  Programs a word at a specified address.
 * @note   This function can be used for N32G45X devices.
 * @param Address specifies the address to be programmed.
 * @param Data specifies the data to be programmed.
 * @return FLASH Status: The returned value can be: FLASH_BUSY,
 *         FLASH_ERR_PG, FLASH_ERR_PV, FLASH_ERR_WRP, FLASH_COMPL,
 *         FLASH_ERR_EV, FLASH_ERR_ADD or FLASH_TIMEOUT.
 */
FLASH_STS BOOT_FLASH_ProgramWord(INT32U Address, INT32U Data)
{
    FLASH_STS status  = FLASH_COMPL;
    __IO INT32U tmp = 0;

    /* Check the parameters */
    assert_param(IS_FLASH_ADDRESS(Address));

    if((Address & (INT32U)0x3) != 0)
    {
        /* The programming address is not a multiple of 4 */
        status = FLASH_ERR_ADD;
        return status;
    }

    /* Clears the FLASH's pending flags */
    MACRO_FLASH_ClearFlag(FLASH_STS_CLRFLAG);
    /* Wait for last operation to be completed */
    status = MACRO_FLASH_WaitForLastOpt(ProgramTimeout);

    if (status == FLASH_COMPL)
    {
        /* if the previous operation is completed, proceed to program the new word */
        FLASH->CTRL |= CTRL_Set_PG;

        *(__IO INT32U*)Address = (INT32U)Data;
        /* Wait for last operation to be completed */
        status = MACRO_FLASH_WaitForLastOpt(ProgramTimeout);

        /* Disable the PG Bit */
        FLASH->CTRL &= CTRL_Reset_PG;
    }

    /* Return the Program Status */
    return status;
}



/**
 * @brief  Clears the FLASH's pending flags.
 * @note   This function can be used for N32G45X devices.
 * @param FLASH_FLAG specifies the FLASH flags to clear.
 *   This parameter can be any combination of the following values:
 *     @arg FLASH_FLAG_PGERR FLASH Program error flag
 *     @arg FLASH_FLAG_PVERR FLASH Program Verify ERROR flag
 *     @arg FLASH_FLAG_WRPERR FLASH Write protected error flag
 *     @arg FLASH_FLAG_EOP FLASH End of Operation flag
 *     @arg FLASH_FLAG_EVERR FLASH Erase Verify ERROR flag
 */
//void FLASH_ClearFlag(INT32U FLASH_FLAG)
//{
//    /* Check the parameters */
//    assert_param(IS_FLASH_CLEAR_FLAG(FLASH_FLAG));
//
//    /* Clear the flags */
//    FLASH->STS |= FLASH_FLAG;
//}






/************************************************************************************************
* Input: dwSectorStartAddr: start address of sector(start sector); dwSectorEndAddr: end address of
*        sector(end sector)
*
* Output: true: erase OK
*
* Logic discription: erase more than one flash sectors
*
* run time:
************************************************************************************************/
INT8U sBootFlashEraseSectors(INT32U dwSectorStartAddr, INT32U dwSectorEndAddr)
{
    FLASH_STS eStatus = FLASH_COMPL;

    if(dwSectorEndAddr < dwSectorStartAddr)
    {
        return FLASH_ERR_ADD;
    }

    MACRO_FLASH_Unlock();

    while(dwSectorStartAddr <= dwSectorEndAddr)
    {
//        MACRO_BOOT_FEEDDOG();
        //log_info("EraseAddr %x\r\n",dwSectorStartAddr);
        //erase one sector
        //eStatus = ((pFLASH_DRV_EraseSector)BootFlashFunc(BOOT_FLASH_EraseOnePage))(dwSectorStartAddr);
        eStatus = MACRO_FLASH_EraseOnePage(dwSectorStartAddr);
        if(FLASH_COMPL == eStatus)
        {
            dwSectorStartAddr += FLASH_BLOCK_SECTOR_SIZE;
            //log_info("Addr %x\r\n",dwSectorStartAddr);
        }
        else
        {
            //log_error("erase error %x \r\n",eStatus);
            MACRO_FLASH_Lock();
            return eStatus;
        }
    }
    MACRO_FLASH_Lock();
    return FLASH_COMPL;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : sBootFLASH_Program
 * Description   :
 *
 * Implements    :
 *END**************************************************************************/
FLASH_STS sBootFLASH_Program(INT32U dwStartAddr,INT8U *pbData, INT32U dwDataLen)
{
    FLASH_STS eStatus = FLASH_COMPL;    /* Return code variable */
    INT32U dwDataTemp = 0;
    INT32U size = 0;
    while(0 != (dwDataLen & (FLASH_BLOCK_WRITE_UNIT_SIZE - 1U))){//TODO 暂时不考虑溢出
        pbData[dwDataLen] = 0xff;
        dwDataLen++;
    }

    if (dwStartAddr > cdwMCU_SECTOR_END)
    {
        eStatus = FLASH_ERR_ADD;
    }
    MACRO_FLASH_Unlock();

    while (((size + FLASH_BLOCK_WRITE_UNIT_SIZE) <= dwDataLen) && (FLASH_COMPL == eStatus))
    {
        //log_debug("%d\r\n",size);
        /* Check CCIF to verify the previous command is completed */
        if (FLASH_FLAG_BUSY == (FLASH->STS & FLASH_FLAG_BUSY))
        {
            eStatus = FLASH_BUSY;
            //log_debug(".");
        }
        else
        {
            dwDataTemp = pbData[size + 0] + (pbData[size + 1]<<8) + (pbData[size + 2]<<16) + (pbData[size + 3]<<24);
            //log_info("%08x,",dwDataTemp);
            eStatus = MACRO_FLASH_ProgramWord(dwStartAddr,dwDataTemp);
            /* Update destination address for next iteration */
            dwStartAddr += FLASH_BLOCK_WRITE_UNIT_SIZE;
            size+= FLASH_BLOCK_WRITE_UNIT_SIZE;
        }
       // log_debug("%d\r\n",size);
    }

    MACRO_FLASH_Lock();


//    if(eStatus != FLASH_COMPL){
//        log_info("eStatus %x\r\n",eStatus);
//    }

    return FLASH_COMPL;
}







/************************************************************************************************
* Input: *pstFirmPackage: intact firmware package buffer
*
* Output: error code
*
* Logic discription: operate MCU flash when input one firmware package.
*
* run time:
************************************************************************************************/
INT16U sFlashFirmPackOperate(FirmwareDataInfo *pstFirmDataInfo, UpdatePackInfo *pstPackInfo, INT8U bBootUpdateType)
{
    INT32U dwFirmwareAddr;
    INT16U wFirmwareDataLen;
    INT32U dwFlashAddr;
    INT16U wErrorCode = cwOtherError;
    INT16U i;

    wFirmwareDataLen = (INT16U)(((pstFirmDataInfo->bFirmDataLen)[0] << 8) +\
                               (pstFirmDataInfo->bFirmDataLen)[1]);

    wFirmwareDataLen -= 4;//4bytes address of firmware data

    dwFirmwareAddr = (INT32U)(((pstFirmDataInfo->bFirmwareAddr)[0] << 24) +\
                              ((pstFirmDataInfo->bFirmwareAddr)[1] << 16) +\
                              ((pstFirmDataInfo->bFirmwareAddr)[2] << 8) +\
                              (pstFirmDataInfo->bFirmwareAddr)[3]);

    dwFlashAddr = dwFirmwareAddr + pstPackInfo->dwFirmStartAddr;

   // log_hold("add[%x+%x][%d]\r\n",dwFirmwareAddr,pstPackInfo->dwFirmStartAddr,wFirmwareDataLen);
   // #define cdwAPP_SECTOR_START_TEST (cdwAPP_SECTOR_START + (1 * FLASH_BLOCK_SECTOR_SIZE))
   // #define cdwAPP_SECTOR_END_TEST   (cdwAPP_SECTOR_START + (2 * FLASH_BLOCK_SECTOR_SIZE))
   // wErrorCode = cwFlashOperateOK;
   // log_hold("write Addr:%x\r\n",dwFlashAddr);



    //erase flash
    if((pstPackInfo->fUpdateEN)&&(!(pstPackInfo->fHaveErase)))
    {
       // log_hold("FlashErase\r\n");
        //erase MCU flash(advance or normal)
        if((cbForceUpdating == bBootUpdateType) || (cbAdvUpdating == bBootUpdateType))
        {
            //erase all flash area
            if(FLASH_COMPL != ((pFlashEraseSectors)BootFlashFunc(sBootFlashEraseSectors))\
                         (cdwMCU_VECTOR_SECTOR_START, cdwMCU_SECTOR_END))
            {
                return cwMCUEraseError;
            }
        }
        else
        {
            //erase APP flash area
            if(FLASH_COMPL != ((pFlashEraseSectors)BootFlashFunc(sBootFlashEraseSectors))\
                        (cdwAPP_SECTOR_START, cdwAPP_SECTOR_END))
            {
                return cwMCUEraseError;
            }
        }

        wErrorCode = cwFlashOperateOK;
        pstPackInfo->fHaveErase = true;
    }


    if((dwFlashAddr <= pstPackInfo->dwFirmEndAddr) && (pstPackInfo->fUpdateEN))
    {
        //convert ciphertext to plaintext
        ((pCiphertext2Plaintext)BootFlashFunc(sBootCiphertext2Plaintext))(wFirmwareDataLen,
                                                                          (pstFirmDataInfo->bFirmwareData),
                                                                          (pstFirmDataInfo->bFirmwareData),
                                                                       (pstPackInfo->bKEY));
        //replace app mask data in flash buffer
        ((pReplceAppMask)BootFlashFunc(sBootReplceAppMask))(dwFlashAddr,
                                                            (pstFirmDataInfo->bFirmwareData),
                                                            wFirmwareDataLen);
        //write data to flash
        if((cbForceUpdating == bBootUpdateType) || (cbAdvUpdating == bBootUpdateType))
        {
            if(dwFlashAddr > cdwMCU_SECTOR_END)
            {
                return cwUpdateAddrError;
            }
        }
        else
        {
            if((dwFlashAddr < cdwAPP_SECTOR_START) || (dwFlashAddr > cdwAPP_SECTOR_END))
            {
                return cwUpdateAddrError;
            }
        }

        //run reflash
        if(((pstPackInfo->dwCurrentFirmAddr) + (pstPackInfo->wLastFirmLen)) == dwFirmwareAddr)
        {
            if(wFirmwareDataLen < cbMaxFirmwareData)
            {
                for(i = 0; i < (cbMaxFirmwareData - wFirmwareDataLen); i++)
                {
                    (pstFirmDataInfo->bFirmwareData)[i + wFirmwareDataLen] = 0xFF;
                }
            }

            if(FLASH_COMPL == ((pFLASH_Program)BootFlashFunc(sBootFLASH_Program))(dwFlashAddr,
                                                                                        (pstFirmDataInfo->bFirmwareData),
                                                                                        wFirmwareDataLen))
            {
                wErrorCode = cwFlashOperateOK;
            }
            else
            {
                wErrorCode = cwUpdateDataError;

            }
        }

        //verify firmware data
        for(i = 0; i < wFirmwareDataLen; i++)
        {
            if((pstFirmDataInfo->bFirmwareData)[i] != *((INT8U*)dwFlashAddr + i))
            {
                wErrorCode = cwUpdateDataError;
                //log_error("addr:%x,%02x,%02x\r\n",i + dwFlashAddr,(pstFirmDataInfo->bFirmwareData)[i],*((INT8U*)dwFlashAddr + i));
                break;
            }
        }
        if(i == wFirmwareDataLen)
        {
            //verify OK
            wErrorCode = cwFlashOperateOK;
        }

        if(cwFlashOperateOK == wErrorCode)
        {
            (pstPackInfo->dwCurrentFirmAddr) = dwFirmwareAddr;
            (pstPackInfo->wLastFirmLen) = wFirmwareDataLen;
        }



    }
//    if(cwFlashOperateOK != wErrorCode ){
//        log_hold("ErrorCode %x\r\n");
//    }
    return wErrorCode;
}

INT16U CRC16_2(INT8U* pchMsg, INT16U wDataLen)
{
    INT16U wCRC = 0xFFFF;
    INT16U i;
    INT8U chChar;
    for (i = 0; i < wDataLen; i++)
    {
        chChar = *pchMsg++;

        wCRC = (INT16U)(((INT16U*)BootRodata(wCRCTableAbs))[(chChar ^ wCRC) & 15] ^ (wCRC >> 4));
        wCRC = (INT16U)(((INT16U*)BootRodata(wCRCTableAbs))[((chChar >> 4) ^ wCRC) & 15] ^ (wCRC >> 4));
    }

    return wCRC;
}
/************************************************************************************************
* Input: bKEYIndex: which key(0~3); pbKEYData: point of key data(12bytes)
*
* Output: key(0~3)
*
* Logic discription: convert key from 12bytes key data
*
* run time:
************************************************************************************************/
INT8U sKEYConvert(INT8U bKEYIndex, INT8U *pbKEYData)
{
    return (INT8U)(0xFF - pbKEYData[bKEYIndex] + pbKEYData[bKEYIndex + 4] + pbKEYData[bKEYIndex + 8]);
}

/************************************************************************************************
* Input: pbStr1; pbStr2; wStrLen: compare length
*
* Output: 0: compare OK; 1: compare NG
*
* Logic discription: simple string compare function (the same as strncmp())
*
* run time:
************************************************************************************************/
INT8U sBootStrncmp(INT8U *pbStr1, INT8U *pbStr2, INT16U wStrLen)
{
    INT16U i;
    for(i = 0; i < wStrLen; i++)
    {
        if(pbStr1[i] != pbStr2[i])
        {
            return 1;
        }
    }

    return 0;
}

/************************************************************************************************
* Input: wDataLen; pbDataIn: buffer in (ciphertext); pbDataOut: buffer out (plaintext)
*
* Output: void
*
* Logic discription: convert ciphertext to plaintext
*
* run time:
************************************************************************************************/
void sBootCiphertext2Plaintext(INT16U wDataLen, INT8U *pbDataIn, INT8U *pbDataOut, INT8U *pbKeys)
{
    INT16U i;

    for(i = 0; i < wDataLen; i++)
    {
        pbDataOut[i] = (INT8U)((((~(pbDataIn[i])) ^ pbKeys[3]) + pbKeys[2] - pbKeys[1]) ^ pbKeys[0]);
    }
}

/************************************************************************************************
* Input: wDataLen; pbDataIn: buffer in (plaintext)plaintext; pbDataOut: buffer out (ciphertext)
*
* Output: void
*
* Logic discription: convert plaintext to ciphertext
*
* run time:
************************************************************************************************/
void sBootPlaintext2Ciphertext(INT16U wDataLen, INT8U *pbDataIn, INT8U *pbDataOut, INT8U *pbKeys)
{
    INT16U i;

    for(i = 0; i < wDataLen; i++)
    {
        pbDataOut[i] = (INT8U)(~((((pbDataIn[i] ^ pbKeys[0]) + pbKeys[1]) - pbKeys[2]) ^ pbKeys[3]));
    }
}

/************************************************************************************************
* Input: dwFlashAddr: start address of flash buffer; pbData: flash buffer; wDataLen: flash buffer length
*
* Output: void
*
* Logic discription: replace app mask data in flash buffer
*
* run time:
************************************************************************************************/
static void sBootReplceAppMask(INT32U dwFlashAddr, INT8U *pbData, INT16U wDataLen)
{
    INT16U i;
    INT32U dwAPPMaskAddr;

    dwAPPMaskAddr = (INT32U)sAppOkMark;

    if((dwFlashAddr <= dwAPPMaskAddr) && ((dwFlashAddr + wDataLen) > dwAPPMaskAddr))
    {
        //flash buffer including APP mask
        for(i = 0; i < cbAppMaskLen; i++)
        {
            //save app mask buffer and replace with 0xFF
            bBootAppMaskBuff[i] = pbData[dwAPPMaskAddr - dwFlashAddr + i];
            pbData[dwAPPMaskAddr - dwFlashAddr + i] = 0xFF;
        }
    }
}

/************************************************************************************************
* Input: pDevTree: point of device tree; pbFirmwareStr: firmware type string to be match;
*        bStrLen: firmware type string length; pbRecursionCnt: point of recursion times
*
* Output: true: match OK
*
* Logic discription: do recursion to match firmware type and boot version string in device tree
*
* run time:
************************************************************************************************/
INT8U sBootMatchFirmware(DevTreeInfo *pDevTree, INT8U *pbFirmware)
{
    //compare firmware type string
    if((0 == ((pStrncmp)BootFlashFunc(sBootStrncmp))(pbFirmware,(pDevTree->bFirmwareVer),cbFirmwareTypeStrLen))&&\
      (0 == ((pStrncmp)BootFlashFunc(sBootStrncmp))(pbFirmware + 7,&(pDevTree->bFirmwareVer[7]),cbBootFirmwareStrLen)))
    {
        return true;
    }

    return false;
}

/************************************************************************************************
* Input: pDevTree: point of device tree; pbFirmwareStr: firmware type string to be match;
*        bStrLen: firmware type string length; pbRecursionCnt: point of recursion times
*
* Output: true: match OK
*
* Logic discription: do recursion to match firmware type string in device tree
*
* run time:
************************************************************************************************/
INT8U sBootMatchFirmwareType(DevTreeInfo *pDevTree, INT8U *pbFirmware)
{
    //compare firmware type string
    if(0 == ((pStrncmp)BootFlashFunc(sBootStrncmp))(pbFirmware,
                                                    (pDevTree->bFirmwareVer),
                                                    cbFirmwareTypeStrLen))
    {
        return true;
    }

    return false;
}

/************************************************************************************************
* Input: pDevTree: point of device tree; pbFirmMCUStr: like "Omega"; pbRecursionCnt: point of recursion counter
*        pbNum: point of number of same MCU.
*
* Output: true: compare operation is OK
*
* Logic discription: to check how many the same MCU type in device tree.
*
* run time:
************************************************************************************************/
INT8U sBootChkSameMCU(DevTreeInfo *pDevTree, INT8U *bFirmMCUType)
{
    //same MCU type as the input MCU type
    if(pDevTree->bFirmwareVer[0] == bFirmMCUType[0])
    {
        return true;
    }

    return false;
}

/************************************************************************************************
* Input: pDevTree: point of device tree; bFirmDevID: DeviceID; pbRecursionCnt: point of recursion counter
*        pbNum: point of number of same Device.
*
* Output: true: compare operation is OK
*
* Logic discription: to check how many the same Device type in device tree.
*
* run time:
************************************************************************************************/
INT8U sBootChkSameDevID(DevTreeInfo *pDevTree, INT8U *bFirmDevID, BootFirmCompareInfo *pstCompVar)
{
    INT8U bSubsetIndex = 0;
    INT8U bDevIDIndex = 0;

    //addup sum of recursion
    pstCompVar->bRecursionCnt++;
    if(pstCompVar->bRecursionCnt > (cbMaxBootSubsets + 1))
    {
        return false;
    }

    if(pstCompVar->bRoute == 0)
    {
        return false;
    }

    //same device id as the input device id value
    if((pDevTree->bDevID[0] == bFirmDevID[0])&&\
      (pDevTree->bDevID[1] == bFirmDevID[1])&&\
        (pDevTree->bDevID[2] == bFirmDevID[2]))
    {
        pstCompVar->bRouteNext++;
        if(pstCompVar->bRouteNext == 2)
        {
            pstCompVar->bCommuAddr = pDevTree->bCommuAddr;
            pstCompVar->bTransCH = pDevTree->bCommuCH;
        }

        if(pstCompVar->bRoute == 1)
        {
            return true;
        }
        else
        {
            pstCompVar->bRoute--;
            bDevIDIndex++;
        }
    }

    while(pstCompVar->bRecursionCnt < (cbMaxBootSubsets + 1))//((pDevTree->bDevSubsetNum) > bSubsetIndex)
    {
        bSubsetIndex++;

        //do recursion to compare all subsets' device tree
        return ((pChkSameDevID)BootFlashFunc(sBootChkSameDevID))((DevTreeInfo*)((INT32U)pDevTree + bSubsetIndex * cbDevTreeInfoStLen),
                                                                       bFirmDevID + bDevIDIndex * 3, pstCompVar);
    }

    return false;
}

INT32U sBootStrlen(char* pbString)
{
    INT32U i = 0;

    while(pbString[i] != '\0')
    {
        i++;
    }

    return i;
}
INT16U sLocalUpdateSpiFlashFirmPackOperate(LocalUpdateSpiFlashFirmwarePacket *pstFirmwarePacket,
                                           LocalFirmwareUpdatePackInfo *pstPackInfo)
{

    INT32U dwPacketIndex;
    INT16U wPacketDataLen;
    INT16U wErrorCode = cwOtherError;
    INT16U i;

    if((0 == pstFirmwarePacket) || (0 == pstPackInfo))
    {
        return cwFlashOperateError;
    }//mark2019/10/23 05

    dwPacketIndex = *(INT32U*)(pstFirmwarePacket->SpiFlashPacket.bPacketIndex);

	wPacketDataLen = *(INT16U*)(pstFirmwarePacket->SpiFlashPacket.bPacketDataLen);
	wPacketDataLen -= 2;

    //convert ciphertext to plaintext
    ((pCiphertext2Plaintext)BootFlashFunc(sBootCiphertext2Plaintext))(wPacketDataLen,
                                                                      pstFirmwarePacket->SpiFlashPacket.Packet.data.bPacketData,
                                                                      pstFirmwarePacket->SpiFlashPacket.Packet.data.bPacketData,
                                                                      pstPackInfo->bKEY);
    //write data to flash
    if((cbForceUpdating == pstPackInfo->bUpdateType) ||\
       (cbAdvUpdating == pstPackInfo->bUpdateType))
    {
        if(pstPackInfo->dwMcuFlashAddress > cdwMCU_SECTOR_END)
        {
            return cwUpdateAddrError;
        }
    }
    else
    {
        if((pstPackInfo->dwMcuFlashAddress < cdwAPP_SECTOR_START) || (pstPackInfo->dwMcuFlashAddress > cdwAPP_SECTOR_END))
        {
            return cwUpdateAddrError;
        }
    }

    if(2 == dwPacketIndex)
    {
        //replace app mask data in flash buffer
        ((pReplceAppMask)BootFlashFunc(sBootReplceAppMask))(pstPackInfo->dwMcuFlashAddress,
                                                            pstFirmwarePacket->SpiFlashPacket.Packet.data.bPacketData,
                                                            wPacketDataLen);
    }


    if(FLASH_COMPL == ((pFLASH_Program)BootFlashFunc(sBootFLASH_Program))(pstPackInfo->dwMcuFlashAddress,
                                                                   pstFirmwarePacket->SpiFlashPacket.Packet.data.bPacketData,
                                                                   wPacketDataLen))
    {
        wErrorCode = cwFlashOperateOK;

    }
    else
    {
        wErrorCode = cwUpdateDataError;
        //log_error("program error %x\r\n",pstPackInfo->dwMcuFlashAddress);
    }

//    M_BOOT_CLR_FLASH_CACHE();
    //verify firmware data
    for(i = 0; i < wPacketDataLen; i++)
    {
        if((pstFirmwarePacket->SpiFlashPacket.Packet.data.bPacketData)[i] != *((INT8U*)pstPackInfo->dwMcuFlashAddress + i))
        {
            wErrorCode = cwUpdateDataError;
            //log_error("flash check error %x,%d\r\n",pstPackInfo->dwMcuFlashAddress,i);
            break;
        }
    }
    if(i == wPacketDataLen)
    {
        //verify OK
        wErrorCode = cwFlashOperateOK;
    }

    if(cwFlashOperateOK == wErrorCode)
    {
        pstPackInfo->dwCurrentFirmNum = dwPacketIndex;
        pstPackInfo->dwMcuFlashAddress += wPacketDataLen;
    }

    return wErrorCode;
}

void sBootFalshCodeEnd(void)
{
}



