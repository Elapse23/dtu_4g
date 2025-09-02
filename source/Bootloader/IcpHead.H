/************************************************************************************************
File Name:      IcpHead.H
Description:    header of TBB ICP programer
Version:        V1.0
Author:         chenweijie
Date:           2019-07-04
*************************************************************************************************/
#ifndef __ICP_HEAD_H__
#define __ICP_HEAD_H__

#include "BootloaderConfig.h"


/*********************************************************
* MCU definition
*********************************************************/
//MCU Type
#define cbMR32                                  1
#define cbAC60                                  2
#define cbAC256                                 3
#define cbDZ60                                  4
#define cbZ64                                   5
#define cbZ128                                  6
#define cbQD4                                   7
#define cbS32K                                  8
#define cbS9KEA                                 9
#define Upsilon                                 14        
#define cbMaxMCUType                            9

/*********************************************************
* Flash operating definition
*********************************************************/
//UART ICP Command type
#define cwICP_CMD_RX_DISABLE_ADDR               0x0000
#define cwICP_CMD_RX_ENABLE_ADDR                0x0001
#define cwICP_CMD_COOK_AUTH_ADDR                0x0002
#define cwICP_CMD_DEVICE_TREE_ADDR              0x0003
#define cwICP_CMD_START_COOK_ADDR               0x0004
#define cwICP_CMD_DATA_TRF_ADDR                 0x0005
#define cwICP_CMD_FINISH_COOK_ADDR              0x0006
#define cwICP_CMD_QUERY_ADDR_ADDR               0x0007
#define cwICP_CMD_SET_BAUDRATE_ADDR             0x0008
#define cwICP_CMD_MD5_CHECK_ADDR                0x0009

//run flash error code
#define cwFlashOperateOK                        0
#define cwMCUEraseError                         0x0001
#define cwLoginError                            0x0002
#define cwCRCError                              0x0015
#define cwUpdateAddrError                       0x0023
#define cwUpdateDataError                       0x0030
#define cwFlashOperateError                     0x0039
#define cwWaitingFlash                          0x003A
#define cwSwitchBoot                            0x003B
#define cwCantChgUpdateDev                      0x003C
#define cwOtherError                            0xFFFF

#define cbNormalUpdating                        0
#define cbAdvUpdating                           1
#define cbForceUpdating                         2

/*********************************************************
* Device ID
*********************************************************/
#define cwDeviceType_System                     0xFFFF

#define cwDeviceType_CCS			            0x0001
#define cwDeviceType_Cyber			            0x0002
#define cwDeviceType_COMBI			            0x0003
#define cwDeviceType_MEDU			            0x0004
#define cwDeviceType_SPCtl			            0x0005
#define cwDeviceType_SPLcd			            0x0006
#define cwDeviceType_Crystal		            0x0007
#define cwDeviceType_MP				            0x0008
#define cwDeviceType_LCM10			            0x0009
#define cwDeviceType_RS32			            0x000A
#define cwDeviceType_CMP			            0x000B
#define cwDeviceType_LCM15			            0x000C
#define cwDeviceType_MASTER                     0x000D
#define cwDeviceType_DX                         0x000E
#define cwDeviceType_DDX			            0x000F
#define cwDeviceType_CH                         0x0010
#define cwDeviceType_LCM8                       0x0011
#define cwDeviceType_Switch                     0x0012
#define cwDeviceType_DXT                        0x0013
#define cwDeviceType_MPPT                       0x0014
#define cwDeviceType_DDC                        0x0015
#define cwDeviceType_CS                         0x0016
#define cwDeviceType_CSLcd                      0x0017
#define cwDeviceType_DU                         0x0018
#define cwDeviceType_CC_LCD                     0x0019
#define cwDeviceType_CK_LCD                     0x001A
#define cwDeviceType_Vision                     0x001B
#define cwDeviceType_CC_MPPT                    0x001C
#define cwDeviceType_ACBOX                      0x001D
#define cwDeviceType_R62C                       0x001E
#define cwDeviceType_RS45                       0x001F
#define cwDeviceType_BM			                0x0030
#define cwDeviceType_N5000_CTRL                 0x0031
#define cwDeviceType_SP120_150 	                0x0032
#define cwDeviceType_CK 	                    0x0033
#define cwDeviceType_WCM 	                    0x0034
#define cwDeviceType_N5000_LCD                  0x0035
#define cwDeviceType_XCM16			            0x0036
#define cwDeviceType_TU600_CTRL 	            0x0037
#define cwDeviceType_TU600_LCD  	            0x0038
#define cwDeviceType_RP4			            0x0037
#define cwDeviceType_CN_LCD			            0x003C
#define cwDeviceType_RS10			            0x003F
#define cwDeviceType_SP100_LCD			        0x0042
#define cwDeviceType_GY485                      0x004D
#define cwDeviceType_PCU                        0x0052
#define cwDeviceType_KINERGY_II                 0x006B
/*********************************************************
* Type definition
*********************************************************/



/////////////
#define cbFirmwareVerAllLen                     (4+3+2+1)//(cbFirmwareTypeStrLen+cbFirmwareVersionLen+cbBootFirmwareStrLen+1)//(4+3+2+1)
#define cbFirmwareTypeStrLen                    4
#define cbFirmwareVersionLen                    3//App版本
#define cbBootFirmwareStrLen                    2//Boot版本

#define cbDevIDLen                              3
#define cbDevTreeInfoStLen                      (1+cbDevIDLen+cbFirmwareVerAllLen+1+1)
#define cbDevTreeInfoLen                        (1+cbDevIDLen+cbFirmwareVerAllLen+1)
#define cbFirstDateLen                          30
#define cbFirstPackLen                          (cbFirstDateLen + 6)

#define cbFirmUUIDLen                           32

#define cbLocalFirmwarePacketKeyLen             12


typedef struct
{
    INT8U bFirmDataLen[2];
    INT8U bFirmwareAddr[4];
    INT8U bFirmwareData[cbMaxFirmwareData];
}FirmwareDataInfo;


typedef struct
{
    INT8U bCommuAddr;//communition address
    INT8U bDevID[cbDevIDLen];
    INT8U bFirmwareVer[cbFirmwareVerAllLen];
    INT8U bDevSubsetNum;
    INT8U bCommuCH;//0数组母设备表示子设备总数(包含子设备的子设备)，其余表示子设备通道
}DevTreeInfo;


///////////////
#define cbFirmwareVerStrLen                     30
#define cbFirmwareIDLen                         6
#define cbFirmwareVerLen                        6
#define cbDevSNLen                              30
#define cbPackageHeadLen                        2
#define cbGasMacVerStrLen                       12

typedef struct
{
    INT8U bDevSN[cbDevSNLen];
    INT8U bDevID[cbDevIDLen];
    INT8U bFirmwareVerStr[cbFirmwareVerStrLen];
    INT8U bDevSubsetNum;
}DevTreeStrInfo;



typedef struct{
    INT8U bMCUType;//0
    INT8U bFirmwareID[3];//1-3
    INT8U bFirmwareVer[cbFirmwareVersionLen];//4 -6
    INT8U bFirmwareBootVer[cbBootFirmwareStrLen];//7 -8
    INT8U bKey[12];    //keys to decode firmware data//9-20
    INT8U bUpdateType; //0:normal96; 1:advance//21
    INT8U bStartAddr[4];//22-25
    INT8U bEndAddr[4];//26-29   
    uint8_t  bFileMD5[cbFirmUUIDLen];//MD5 of PAC file//30-254
}T_FIRWAREINFO;



typedef struct{
    uint8_t bPacketsNumber[4];//totale pack
    uint8_t bMcuType;
    uint8_t bFirmwareType[3];
    uint8_t bAppVersion[3];
    uint8_t bbootVersion[2];//13
    uint8_t bkey[cbLocalFirmwarePacketKeyLen];//25
    uint8_t bUpdateType; //26
    uint8_t bCrc16[2];
}T_FLASH_INFO;

typedef union
{
    uint8_t bData[cdwLocalUpdateFirmDataLen];
    struct
    {
        uint8_t bPacketIndex[4];//pack num
        uint8_t bPacketDataLen[2];
        union
        {
            uint8_t bData[cdwLocalUpdateFirmDataLen - 6];
            T_FLASH_INFO info;
            struct
            {
                uint8_t bPacketData[cdwLocalUpdateFirmDataLen - 6];//data  + crc16
            }data;
        }Packet;
    }SpiFlashPacket;
}LocalUpdateSpiFlashFirmwarePacket;


//FirmwarePackage

typedef struct
{
    uint32_t dwCurrentFirmNum;
    uint32_t dwCurrentFirmSum;
    uint32_t dwSpiFlashAddress;
    uint32_t dwMcuFlashAddress;
    uint8_t bFileMD5[cbFirmUUIDLen];//MD5 of PAC file
    uint8_t bUpdateType;
    uint8_t bKEY[4];
    uint8_t fUpdateEN;
    uint8_t fMD5VerifyOK;
    uint8_t fHaveRecPackage;
}LocalFirmwareUpdatePackInfo;




#define __CHAR_TO_UINT16(pU8,num)  (((uint16_t)pU8[(num) + 0] << 8) + ((uint16_t)pU8[(num) + 1] << 0) )
#define _CHAR_TO_UINT16(U16,pU8,num) U16 = __CHAR_TO_UINT16(pU8,num)
#define _UINT16_TO_CHAR(U16,pU8,num) do{pU8[(num) + 0] = (U16 >> 8) & 0xff; \
                                        pU8[(num) + 1] = (U16 >> 0) & 0xff;  }while(0)
#define CHAR_TO_UINT16(U16,pU8)      _CHAR_TO_UINT16(U16,pU8,0)
#define UINT16_TO_CHAR(U16,pU8)      _UINT16_TO_CHAR(U16,pU8,0)

#define __CHAR_TO_UINT32(pU8,num)    (((uint32_t)pU8[(num) + 0] << 24) + ((uint32_t)pU8[(num) + 1] << 16) + ((uint32_t)pU8[(num) + 2] << 8) + ((uint32_t)pU8[(num) +3] << 0))
#define _CHAR_TO_UINT32(U32,pU8,num) U32 = __CHAR_TO_UINT32(pU8,num)
#define _UINT32_TO_CHAR(U32,pU8,num) do{ pU8[(num) + 0] = (U32 >> 24) & 0xff; \
                                         pU8[(num) + 1] = (U32 >> 16) & 0xff; \
                                         pU8[(num) + 2] = (U32 >> 8)  & 0xff; \
                                         pU8[(num) + 3] = (U32 >> 0)  & 0xff; }while(0)
#define CHAR_TO_UINT32(U32,pU8)      _CHAR_TO_UINT32(U32,pU8,0)
#define UINT32_TO_CHAR(U32,pU8)      _UINT32_TO_CHAR(U32,pU8,0)

#define cdwDevFirmAddrStart                     0x00001000
#define cbFlashFirmInfoLen                      1

#define cdwPackFirmAddrStart                    (cdwDevFirmAddrStart + cbFlashFirmInfoLen)

#define cdwDevFirmSizeLen                       (600 * 1024)
#define cdwDevFirmAddrEnd                       (cdwDevFirmAddrStart  + cdwDevFirmSizeLen - 1)

//#define ble_cdwDevFirmAddrStart                 0x00697000 
#define ble_cdwDevFirmAddrStart                 0x00497000 

#define ble_cbFlashFirmInfoLen                  1

#define ble_cdwPackFirmAddrStart                (ble_cdwDevFirmAddrStart + ble_cbFlashFirmInfoLen)
#define ble_cdwDevFirmSizeLen                   (600 * 1024)

#define ble_cdwDevFirmAddrEnd                   (ble_cdwDevFirmAddrStart  + ble_cdwDevFirmSizeLen - 1)


#define  _PU8_32_RESVER(pU8,num)                do{ pU8[(num) + 0] =  pU8[(num) + 3]; \
                                                    pU8[(num) + 1] =  pU8[(num) + 2]; \
                                                    pU8[(num) + 2] =  pU8[(num) + 1]; \
                                                    pU8[(num) + 3] =  pU8[(num) + 0]; }while(0) 
                                                 
#define  PU8_32_RESVER(pU8)                     _PU8_32_RESVER(pU8,0)


#endif  //__ICP_HEAD_H__
