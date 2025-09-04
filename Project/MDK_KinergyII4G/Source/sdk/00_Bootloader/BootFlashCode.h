/************************************************************************************************
File Name:      BootFlashCode.H
Description:    bootloader MCU flash operation code
Version:        V1.0
Author:         chenweijie
Date:           2018-08-24
*************************************************************************************************/
#ifndef BOOT_FLASHCODE_H
#define BOOT_FLASHCODE_H
#include "n32g45x.h"

#include "Bootloader.H"
#include "BootloaderConfig.h"

#include "IcpHead.H"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define FLASH_BLOCK_SECTOR_SIZE  (2048U)
#define FLASH_BLOCK_WRITE_UNIT_SIZE  (4U)
/** @addtogroup FLASH_Private_Defines
 * @{
 */

/* Flash Access Control Register bits */
#define AC_LATENCY_MSK ((uint32_t)0x000000F8)
#define AC_PRFTBE_MSK  ((uint32_t)0xFFFFFFEF)
#define AC_ICAHEN_MSK  ((uint32_t)0xFFFFFF7F)

/* Flash Access Control Register bits */
#define AC_PRFTBS_MSK  ((uint32_t)0x00000020)
#define AC_ICAHRST_MSK ((uint32_t)0x00000040)

/* Flash Control Register bits */
#define CTRL_Set_PG       ((uint32_t)0x00000001)
#define CTRL_Reset_PG     ((uint32_t)0x00003FFE)
#define CTRL_Set_PER      ((uint32_t)0x00000002)
#define CTRL_Reset_PER    ((uint32_t)0x00003FFD)
#define CTRL_Set_MER      ((uint32_t)0x00000004)
#define CTRL_Reset_MER    ((uint32_t)0x00003FFB)
#define CTRL_Set_OPTPG    ((uint32_t)0x00000010)
#define CTRL_Reset_OPTPG  ((uint32_t)0x00003FEF)
#define CTRL_Set_OPTER    ((uint32_t)0x00000020)
#define CTRL_Reset_OPTER  ((uint32_t)0x00003FDF)
#define CTRL_Set_START    ((uint32_t)0x00000040)
#define CTRL_Set_LOCK     ((uint32_t)0x00000080)
#define CTRL_Reset_SMPSEL ((uint32_t)0x00003EFF)
#define CTRL_SMPSEL_SMP1  ((uint32_t)0x00000000)
#define CTRL_SMPSEL_SMP2  ((uint32_t)0x00000100)

/* FLASH Mask */
#define RDPRTL1_MSK  ((uint32_t)0x00000002)
#define RDPRTL2_MSK  ((uint32_t)0x80000000)
#define OBR_USER_MSK ((uint32_t)0x0000001C)
#define WRP0_MSK     ((uint32_t)0x000000FF)
#define WRP1_MSK     ((uint32_t)0x0000FF00)
#define WRP2_MSK     ((uint32_t)0x00FF0000)
#define WRP3_MSK     ((uint32_t)0xFF000000)

/* FLASH Keys */
#define L1_RDP_Key   ((uint32_t)0xFFFF00A5)
#define RDP_USER_Key ((uint32_t)0xFFF800A5)
#define L2_RDP_Key   ((uint32_t)0xFFFF33CC)
#define FLASH_KEY1   ((uint32_t)0x45670123)
#define FLASH_KEY2   ((uint32_t)0xCDEF89AB)

/* Delay definition */
#define EraseTimeout   ((uint32_t)0x000B0000)
#define ProgramTimeout ((uint32_t)0x00002000)
/**
 * @brief  FLASH Status
 */

//typedef enum
//{
//    FLASH_BUSY = 1,
//    FLASH_RESERVED,
//    FLASH_ERR_PG,
//    FLASH_ERR_PV,
//    FLASH_ERR_WRP,
//    FLASH_COMPL,
//    FLASH_ERR_EV,
//    FLASH_ERR_RDP2,
//    FLASH_ERR_ADD,
//    FLASH_TIMEOUT
//} FLASH_STS;

//typedef enum
//{
//    FLASH_SMP1 = 0,
//    FLASH_SMP2
//} FLASH_SMPSEL;

/** @addtogroup FLASH_Flags
 * @{
 */
#define FLASH_FLAG_BUSY     ((uint32_t)0x00000001) /*!< FLASH Busy flag */
#define FLASH_FLAG_PGERR    ((uint32_t)0x00000004) /*!< FLASH Program error flag */
#define FLASH_FLAG_PVERR    ((uint32_t)0x00000008) /*!< FLASH Program Verify ERROR flag after program */
#define FLASH_FLAG_WRPERR   ((uint32_t)0x00000010) /*!< FLASH Write protected error flag */
#define FLASH_FLAG_EOP      ((uint32_t)0x00000020) /*!< FLASH End of Operation flag */
#define FLASH_FLAG_EVERR    ((uint32_t)0x00000040) /*!< FLASH Erase Verify ERROR flag after page erase */
#define FLASH_FLAG_OBERR    ((uint32_t)0x00000001) /*!< FLASH Option Byte error flag */

#define IS_FLASH_CLEAR_FLAG(FLAG) ((((FLAG) & 0xFFFFFF83) == 0x00) && (FLAG != 0x00))

#define IS_FLASH_GET_FLAG(FLAG)                                                                                       \
       (((FLAG) == FLASH_FLAG_BUSY)   || ((FLAG) == FLASH_FLAG_PGERR) || ((FLAG) == FLASH_FLAG_PVERR)                 \
     || ((FLAG) == FLASH_FLAG_WRPERR) || ((FLAG) == FLASH_FLAG_EOP)   || ((FLAG) == FLASH_FLAG_EVERR)                 \
     || ((FLAG) == FLASH_FLAG_OBERR))

/** @addtogroup FLASH_STS_CLRFLAG
 * @{
 */
#define FLASH_STS_CLRFLAG   (FLASH_FLAG_PGERR | FLASH_FLAG_PVERR | FLASH_FLAG_WRPERR | FLASH_FLAG_EOP |FLASH_FLAG_EVERR)

/* FLASH module features */
#define IS_FLASH_ADDRESS(ADDRESS) (((ADDRESS) >= 0x08000000) && ((ADDRESS) < 0x0807FFFF))
#define assert_param(expr) ((void)0)


/* self defination */


typedef FLASH_STS (*pFLASH_Program)(INT32U dwStartAddr, const INT8U *pbData, INT32U dwDataLen);
typedef INT8U (*pFlashEraseSectors)(INT32U dwSectorStartAddr, INT32U dwSectorEndAddr);

typedef INT16U (*pCRC16_2)(INT8U* pchMsg, INT16U wDataLen);
typedef INT16U (*pFlashFirmPackOperate)(FirmwareDataInfo *pstFirmDataInfo, UpdatePackInfo *pstPackInfo, INT8U bBootUpdateType);
typedef INT8U (*pKEYConvert)(INT8U bKEYIndex, INT8U *pbKEYData);
typedef INT8U (*pStrncmp)(INT8U *pbStr1, INT8U *pbStr2, INT16U wStrLen);
typedef void (*pCiphertext2Plaintext)(INT16U wDataLen, INT8U *pbDataIn, INT8U *pbDataOut, INT8U *pbKeys);
typedef void (*pPlaintext2Ciphertext)(INT16U wDataLen, INT8U *pbDataIn, INT8U *pbDataOut, INT8U *pbKeys);
typedef INT8U (*pMatchFirmware)(DevTreeInfo *pDevTree, INT8U *pbFirmwareStr);
typedef INT8U (*pMatchFirmwareType)(DevTreeInfo *pDevTree, INT8U *pbFirmwareStr);
typedef INT8U (*pChkSameMCU)(DevTreeInfo *pDevTree, INT8U *bFirmMCUType);
typedef INT8U (*pChkSameDevID)(DevTreeInfo *pDevTree, INT8U *bFirmDevID, BootFirmCompareInfo *pstCompVar);
typedef INT32U (*pStrlen)(char *pbString);
typedef void (*pReplceAppMask)(INT32U dwFlashAddr, INT8U *pbData, INT16U wDataLen);
typedef FLASH_STS (*pFLASH_DRV_CommandSequence)(void);
typedef FLASH_STS (*pFLASH_EraseOnePage)(INT32U dwStartAddr);
typedef FLASH_STS (*pFLASH_GetSTS)(void);
typedef FLASH_STS (*pFLASH_WaitForLastOpt)(INT32U Timeout);
typedef FLASH_STS (*pFLASH_ProgramWord)(INT32U Address, INT32U Data);
typedef INT16U    (*pFlash_FiramPackOperate)(LocalUpdateSpiFlashFirmwarePacket *pstFirmwarePacket,LocalFirmwareUpdatePackInfo *pstPackInfo);


void sBootFalshCodeStart(void);

//void FLASH_Unlock(void);
//void FLASH_Lock(void);
//FLASH_STS FLASH_ProgramWord(INT32U Address, INT32U Data);
//FLASH_STS FLASH_EraseOnePage(INT32U Page_Address);
//void FLASH_ClearFlag(INT32U FLASH_FLAG);
//FLASH_STS FLASH_GetSTS(void);
//FLASH_STS FLASH_WaitForLastOpt(INT32U Timeout);

FLASH_STS sBootFLASH_Program(INT32U dwStartAddr,INT8U *pbData, INT32U dwDataLen);
INT8U sBootFlashEraseSectors(INT32U dwSectorStartAddr, INT32U dwSectorEndAddr);

INT16U sFlashFirmPackOperate(FirmwareDataInfo *pstFirmDataInfo, UpdatePackInfo *pstPackInfo, INT8U bBootUpdateType);

INT16U CRC16_2(INT8U* pchMsg, INT16U wDataLen);
INT8U sKEYConvert(INT8U bKEYIndex, INT8U *pbKEYData);
INT8U sBootStrncmp(INT8U *pbStr1, INT8U *pbStr2, INT16U wStrLen);
void sBootCiphertext2Plaintext(INT16U wDataLen, INT8U *pbDataIn, INT8U *pbDataOut, INT8U *pbKeys);
void sBootPlaintext2Ciphertext(INT16U wDataLen, INT8U *pbDataIn, INT8U *pbDataOut, INT8U *pbKeys);
INT8U sBootMatchFirmware(DevTreeInfo *pDevTree, INT8U *pbFirmware);
INT8U sBootMatchFirmwareType(DevTreeInfo *pDevTree, INT8U *pbFirmwareStr);
INT8U sBootChkSameMCU(DevTreeInfo *pDevTree, INT8U *bFirmMCUType);
INT8U sBootChkSameDevID(DevTreeInfo *pDevTree, INT8U *bFirmDevID, BootFirmCompareInfo *pstCompVar);
INT32U sBootStrlen(char* pbString);
INT16U sLocalUpdateSpiFlashFirmPackOperate(LocalUpdateSpiFlashFirmwarePacket *pstFirmwarePacket,
                                           LocalFirmwareUpdatePackInfo *pstPackInfo);

void sBootFalshCodeEnd(void);

#define MACRO_FLASH_GetSTS              ((pFLASH_GetSTS)BootFlashFunc(BOOT_FLASH_GetSTS))
#define MACRO_FLASH_WaitForLastOpt      ((pFLASH_WaitForLastOpt)BootFlashFunc(BOOT_FLASH_WaitForLastOpt))
#define MACRO_FLASH_ProgramWord         ((pFLASH_ProgramWord)BootFlashFunc(BOOT_FLASH_ProgramWord))
#define MACRO_FLASH_EraseOnePage        ((pFLASH_EraseOnePage)BootFlashFunc(BOOT_FLASH_EraseOnePage))
#define MACRO_FLASH_FirmPackOperate     ((pFlash_FiramPackOperate)BootFlashFunc(sLocalUpdateSpiFlashFirmPackOperate))
#define MACRO_FLASH_Strlen              ((pStrlen)BootFlashFunc(sBootStrlen))


#endif
