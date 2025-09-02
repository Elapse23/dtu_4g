/************************************************************************************************
File Name:      BootSpiFlashCode.C
Description:    bootloader MCU flash operation code
Version:        V1.0
Author:         chenweijie
Date:           2018-08-24
*************************************************************************************************/
#include "BootSpiFlashCode.H"
#include "drv_log.h"
#include <stdio.h>

extern void sAppOkMark(void);
//static INT8U sBootSpiReadByte(void);

#if (true == HalBootSpiFlashFuncEN)
//typedef INT16U (*pBootFlashReadFirmPackage)(INT32U , INT32U , INT8U *);
typedef INT8U (*pBootFlashMemeryRead)(INT32U dwAddr, INT16U wDataLen, INT8U *pbData);
//typedef void (*pBootFlashCmdW_R)(INT8U bCommand, INT16U wWDataLen, INT8U *pbWData, INT16U wRDataLen, INT8U *pbRData);
typedef INT8U (*pBootSpiWriteByte)(INT8U bTxData);
//typedef void (*pBootSpiWriteBuff)(INT16U wDataLen, INT8U *pbData);
//typedef INT8U (*pBootSpiReadByte)(void);
//typedef void (*pBootSpiReadBuff)(INT16U wDataLen, INT8U * pbData);
typedef INT16U (*pBootread_flash_by_addr)(INT32U flash_addr,INT8U* dptr);
//static INT16U sBootFlashReadFirmPackage(INT32U dwFirmStartAddr, INT32U dwPackStartAddr, INT8U *pbData);
//static INT8U sBootFlashMemeryRead(INT32U dwAddr, INT16U wDataLen, INT8U *pbData);
//static void sBootFlashCmdW_R(INT8U bCommand, INT16U wWDataLen, INT8U *pbWData, INT16U wRDataLen, INT8U *pbRData);
//static void sBootSpiWriteByte(INT8U bTxData);
//static void sBootSpiWriteBuff(INT16U wDataLen, INT8U *pbData);
//static INT8U sBootSpiReadByte(void);
//static void sBootSpiReadBuff(INT16U wDataLen, INT8U * pbData);


#define MACRO_SpiWriteByte      ((pBootSpiWriteByte)BootSpiFlashFunc(sBootSpiWriteByte))
//#define MACRO_SpiReadByte       ((pBootSpiReadByte)BootSpiFlashFunc(sBootSpiReadByte))
#define MACRO_FlashMemeryRead   ((pBootFlashMemeryRead)BootSpiFlashFunc(sBootFlashMemeryRead))
//#define MACRO_SpiSpiWriteBuff ((pBootSpiWriteBuff)BootSpiFlashFunc(sBootSpiWriteBuff))
//#define MACRO_SpiReadBuff      ((pBootSpiReadBuff)BootSpiFlashFunc(sBootSpiReadBuff))
#define MACRO_read_flash_by_addr ((pBootread_flash_by_addr)BootSpiFlashFunc(boot_read_flash_by_addr))





/* SPI SPE mask */
#define CTRL1_SPIEN_ENABLE  ((uint16_t)0x0040)
#define CTRL1_SPIEN_DISABLE ((uint16_t)0xFFBF)

/* I2S I2SE mask */
#define I2SCFG_I2SEN_ENABLE  ((uint16_t)0x0400)
#define I2SCFG_I2SEN_DISABLE ((uint16_t)0xFBFF)

/* SPI CRCNext mask */
#define CTRL1_CRCNEXT_ENABLE ((uint16_t)0x1000)

/* SPI CRCEN mask */
#define CTRL1_CRCEN_ENABLE  ((uint16_t)0x2000)
#define CTRL1_CRCEN_DISABLE ((uint16_t)0xDFFF)

/* SPI SSOE mask */
#define CTRL2_SSOEN_ENABLE  ((uint16_t)0x0004)
#define CTRL2_SSOEN_DISABLE ((uint16_t)0xFFFB)

/* SPI registers Masks */
#define CTRL1_CLR_MASK  ((uint16_t)0x3040)
#define I2SCFG_CLR_MASK ((uint16_t)0xF040)

/* SPI or I2S mode selection masks */
#define SPI_MODE_ENABLE ((uint16_t)0xF7FF)
#define I2S_MODE_ENABLE ((uint16_t)0x0800)

/* I2S clock source selection masks */
#define I2S2_CLKSRC  ((uint32_t)(0x00020000))
#define I2S3_CLKSRC  ((uint32_t)(0x00040000))
#define I2S_MUL_MASK ((uint32_t)(0x0000F000))
#define I2S_DIV_MASK ((uint32_t)(0x000000F0))

#define MACRO_SPI_OPEN()    EX_FLASH_SPI_CH->CTRL1 |= CTRL1_SPIEN_ENABLE



void boot_SPI_Init(SPI_Module* SPIx, SPI_InitType* SPI_InitStruct)
{
    uint16_t tmpregister = 0;

    /* check the parameters */
    assert_param(IS_SPI_PERIPH(SPIx));

    /* Check the SPI parameters */
    assert_param(IS_SPI_DIR_MODE(SPI_InitStruct->DataDirection));
    assert_param(IS_SPI_MODE(SPI_InitStruct->SpiMode));
    assert_param(IS_SPI_DATASIZE(SPI_InitStruct->DataLen));
    assert_param(IS_SPI_CLKPOL(SPI_InitStruct->CLKPOL));
    assert_param(IS_SPI_CLKPHA(SPI_InitStruct->CLKPHA));
    assert_param(IS_SPI_NSS(SPI_InitStruct->NSS));
    assert_param(IS_SPI_BR_PRESCALER(SPI_InitStruct->BaudRatePres));
    assert_param(IS_SPI_FIRST_BIT(SPI_InitStruct->FirstBit));
    assert_param(IS_SPI_CRC_POLYNOMIAL(SPI_InitStruct->CRCPoly));

    /*---------------------------- SPIx CTRL1 Configuration ------------------------*/
    /* Get the SPIx CTRL1 value */
    tmpregister = SPIx->CTRL1;
    /* Clear BIDIMode, BIDIOE, RxONLY, SSM, SSI, LSBFirst, BR, MSTR, CPOL and CPHA bits */
    tmpregister &= CTRL1_CLR_MASK;
    /* Configure SPIx: direction, NSS management, first transmitted bit, BaudRate prescaler
       master/salve mode, CPOL and CPHA */
    /* Set BIDImode, BIDIOE and RxONLY bits according to DataDirection value */
    /* Set SSM, SSI and MSTR bits according to SpiMode and NSS values */
    /* Set LSBFirst bit according to FirstBit value */
    /* Set BR bits according to BaudRatePres value */
    /* Set CPOL bit according to CLKPOL value */
    /* Set CPHA bit according to CLKPHA value */
    tmpregister |= (uint16_t)((uint32_t)SPI_InitStruct->DataDirection | SPI_InitStruct->SpiMode
                              | SPI_InitStruct->DataLen | SPI_InitStruct->CLKPOL | SPI_InitStruct->CLKPHA
                              | SPI_InitStruct->NSS | SPI_InitStruct->BaudRatePres | SPI_InitStruct->FirstBit);
    /* Write to SPIx CTRL1 */
    SPIx->CTRL1 = tmpregister;

    /* Activate the SPI mode (Reset I2SMOD bit in I2SCFG register) */
    SPIx->I2SCFG &= SPI_MODE_ENABLE;

    /*---------------------------- SPIx CRCPOLY Configuration --------------------*/
    /* Write to SPIx CRCPOLY */
    SPIx->CRCPOLY = SPI_InitStruct->CRCPoly;
}

void sBootSpiFlashInit(void)
{
    SPI_InitType SPI_InitStructure;
    MACRO_FLASH_GPIO_INIT_MISO();
    MACRO_FLASH_GPIO_INIT_MOSI();
    MACRO_FLASH_GPIO_INIT_SCK();
    MACRO_FLASH_GPIO_INIT_CS();

    APB_RCC_CLOSE(APB2,SPI1);
    APB_RCC_OPEN(APB2,SPI1);//RCC->APB2PRST |= RCC_APB2_PERIPH_SPI1;//RCC_EnableAPB2PeriphReset(, ENABLE);

    /*!< SPI configuration */
    SPI_InitStructure.DataDirection = SPI_DIR_DOUBLELINE_FULLDUPLEX;
    SPI_InitStructure.SpiMode       = SPI_MODE_MASTER;
    SPI_InitStructure.DataLen       = SPI_DATA_SIZE_8BITS;
    SPI_InitStructure.CLKPOL        = SPI_CLKPOL_HIGH;
    SPI_InitStructure.CLKPHA        = SPI_CLKPHA_SECOND_EDGE;
    SPI_InitStructure.NSS           = SPI_NSS_SOFT;
    SPI_InitStructure.BaudRatePres = SPI_BR_PRESCALER_4;
    SPI_InitStructure.FirstBit = SPI_FB_MSB;
    SPI_InitStructure.CRCPoly  = 7;
    boot_SPI_Init(EX_FLASH_SPI_CH, &SPI_InitStructure);
    MACRO_SPI_OPEN();
}




/************************************************************************************************
* BOOT1 CODE
************************************************************************************************/
void sBootSpiFlashCodeStart(void)
{

}



/************************************************************************************************
* Input: bTxData: 1byte data
*
* Output: void
*
* Logic discription: transmit 1byte to MOSI
*
* run time:
************************************************************************************************/
static INT8U sBootSpiWriteByte(INT8U bTxData)
{
    uint8_t time = 0xff;
    while(((EX_FLASH_SPI_CH->STS & SPI_I2S_TE_FLAG) == RESET)&&(time--));//while (SPI_I2S_GetStatus(sFLASH_SPI, SPI_I2S_TE_FLAG) == RESET);
    /*!< Send byte through the SPI1 peripheral */
    if(0 == time){
        return 0;
    }
    EX_FLASH_SPI_CH->DAT = bTxData;//SPI_I2S_TransmitData(sFLASH_SPI, byte);
    /*!< Wait to receive a byte */
    while(((EX_FLASH_SPI_CH->STS & SPI_I2S_RNE_FLAG) == RESET)&&(time--));//while (SPI_I2S_GetStatus(sFLASH_SPI, SPI_I2S_RNE_FLAG) == RESET)
    /*!< Return the byte read from the SPI bus */
    if(0 == time){
        return 0;
    }
    return EX_FLASH_SPI_CH->DAT;// = SPI_I2S_ReceiveData(sFLASH_SPI);
}



/*************************************************************************************
* Input: dwAddr: address of flash to read; wDataLen: data length to read from falsh; pbData.
*
* Output: error code
*
* Logic discription: read spi flash memery
*
* Run time:
*************************************************************************************/

INT8U sBootFlashMemeryRead(INT32U dwAddr, INT16U wDataLen, INT8U *pbData)
{
    uint16_t i;
	MACRO_SPI_CS_LOW();//SPIFLASH_CS_LOW();

	MACRO_SpiWriteByte(CMD_READ_DATA);//flash_write_byte(CMD_READ_DATA);

	if(EX_FLASH_TYPE == W25Q256)
	{
		MACRO_SpiWriteByte((uint8_t)(dwAddr>>24));//flash_write_byte((uint8_t)(Addr>>24));
	}
	MACRO_SpiWriteByte((uint8_t)(dwAddr>>16));//flash_write_byte((uint8_t)(Addr>>16));
	MACRO_SpiWriteByte((uint8_t)(dwAddr>>8));//flash_write_byte((uint8_t)(Addr>>8));
	MACRO_SpiWriteByte((uint8_t)(dwAddr>>0));//flash_write_byte((uint8_t)Addr);
    //log_debug("spi addr%x[%d]",dwAddr,wDataLen);
	for(i=0; i < wDataLen; i++)
	{
		pbData[i] = MACRO_SpiWriteByte(0xA5);//flash_read_byte();
		//log_debug("%02x",pbData[i]);
	}

	MACRO_SPI_CS_HIGHT();//SPIFLASH_CS_HIGH();
    return 0;
}

extern uint8_t ble_ftp_updata_flag;
INT16U boot_read_flash_by_addr(INT32U flash_addr,INT8U* dptr)
{
    INT16U len = 0;
    uint8_t times = 3;//retry time
    if (ble_ftp_updata_flag == 0)
    {
        if ((NULL == dptr) || (cdwPackFirmAddrStart > flash_addr) || (cdwDevFirmAddrEnd < flash_addr))
        {
            return 0;
        }
    }
    else
    {
        if ((NULL == dptr) || (ble_cdwPackFirmAddrStart > flash_addr) || (ble_cdwDevFirmAddrEnd < flash_addr))
        {
            return 0;
        }
    }
    //log_debug("read_addr %x\r\n",flash_addr);
    while(times--){
        //log_debug("time %d\r\n",times);
        if (ble_ftp_updata_flag == 0)
        {
            if (flash_addr == cdwPackFirmAddrStart)
            {
                MACRO_FlashMemeryRead(cdwDevFirmAddrStart, 1, dptr);
                if (0 != dptr[0])
                {
                    log_error("[FLASH]write flage error %x\r\n", flash_addr);
                    continue;
                }
            }
        }
        else
        {
            if (flash_addr == ble_cdwPackFirmAddrStart)
            {
                MACRO_FlashMemeryRead(ble_cdwDevFirmAddrStart, 1, dptr);
                if (0 != dptr[0])
                {
                    log_error("[FLASH]write flage error %x\r\n", flash_addr);
                    continue;
                }
            }
        }
        MACRO_FlashMemeryRead(flash_addr + 4,2,(uint8_t*)&len);
        if((cbMaxPackageData  <= len) ||(0 == len)){
            log_error("[FLASH]read len error  %x,[%d]\r\n",flash_addr,len);
            continue;
        }

        MACRO_FlashMemeryRead(flash_addr,len + 6,dptr);
        if(0 != ((pCRC16_2)BootFlashFunc(CRC16_2))(&dptr[6],len)){
            log_error("[FLASH] crc error addr%x[%d]\r\n",flash_addr,len);
            log_sector(dptr,len +6);
            continue;
        }
        return len;
    }

    return 0;

}

/************************************************************************************************
* Input: void
*
* Output: SPI data
*
* Logic discription: read byte from MISO
*
* run time:
************************************************************************************************/
//static INT8U sBootSpiReadByte(void)
//{
//    return MACRO_SpiWriteByte(0xA5);
//}


/************************************************************************************************
* Input: void
*
* Output: void
*
* Logic discription: get firmware package from SPI flash, and update MCU flash. (running in RAM area)
*
* run time:
************************************************************************************************/
void sBootUpdateFromFlash(void)
{
    INT16U wPackageLen;
    //INT16U wPacketDataLen;
    INT16U wErrorCode = 0;
    INT32U dwFirmwareIndex = 0;
    INT32U dwPacketIndexBackup = 0;
    INT32U dwFirmwareNumber = 0;
    INT8U bRetryFlashCnt = 0;
    LocalUpdateSpiFlashFirmwarePacket *pstFirmPacket;
    LocalFirmwareUpdatePackInfo stLocalFirmwareUpdatePackInfo;

    MACRO_IWDG_RELOD();
    stLocalFirmwareUpdatePackInfo.dwSpiFlashAddress = cdwDevFirmAddrStart + cbFlashFirmInfoLen;
    if (ble_ftp_updata_flag == 1)
    {
        stLocalFirmwareUpdatePackInfo.dwSpiFlashAddress = ble_cdwDevFirmAddrStart + ble_cbFlashFirmInfoLen;
    }

    wPackageLen = MACRO_read_flash_by_addr(stLocalFirmwareUpdatePackInfo.dwSpiFlashAddress,bBootICPRxBuf);

    pstFirmPacket = (LocalUpdateSpiFlashFirmwarePacket *)(bBootICPRxBuf);

//    wPacketDataLen = (INT16U)(pstFirmPacket->SpiFlashPacket.bPacketDataLen[0] << 0) |
//                     (INT16U)(pstFirmPacket->SpiFlashPacket.bPacketDataLen[1] << 8);
//    wPacketDataLen =*(INT16U*)(pstFirmPacket->SpiFlashPacket.bPacketDataLen);
//    if((20 >= wPackageLen)){
//        log_error("[boot] len error\r\n");
//        return;//((psVoidFunc)(BootUARTFunc(sBootUARTICPProcess)))();
//    }
//
//    if(((pCRC16_2)BootFlashFunc(CRC16_2))(pstFirmPacket->SpiFlashPacket.Packet.bData,\
//                                          wPacketDataLen) != 0)
//    {
//        log_error("[boot] crc error\r\n");
//        return;//((psVoidFunc)(BootUARTFunc(sBootUARTICPProcess)))();
//    }
    {
        dwFirmwareIndex =*(INT32U*)(pstFirmPacket->SpiFlashPacket.bPacketIndex);
        dwFirmwareNumber =*(INT32U*)(pstFirmPacket->SpiFlashPacket.Packet.info.bPacketsNumber);
        if(dwFirmwareIndex != 1)
        {
            log_error("[boot] index error\r\n");
            return;//((psVoidFunc)(BootUARTFunc(sBootUARTICPProcess)))();
        }
        else
        {
            dwPacketIndexBackup = dwFirmwareIndex;
        }

        if(cbDevMCUType != pstFirmPacket->SpiFlashPacket.Packet.info.bMcuType)
        {
            log_error("[boot] type error\r\n");
            return;//((psVoidFunc)(BootUARTFunc(sBootUARTICPProcess)))();
        }

        //if is advcance updating && force updating, it is no need to check firmware ID
        stLocalFirmwareUpdatePackInfo.bUpdateType = pstFirmPacket->SpiFlashPacket.Packet.info.bUpdateType;

        if(cbForceUpdating == stLocalFirmwareUpdatePackInfo.bUpdateType)
        {
            (stLocalFirmwareUpdatePackInfo.fUpdateEN) = true;
            (stLocalFirmwareUpdatePackInfo.dwCurrentFirmNum) = 1;//the first package
            (stLocalFirmwareUpdatePackInfo.dwCurrentFirmSum) = dwFirmwareNumber;//sum of firmware packages
        }
        else
        {

            //match firmware ID
            if((bFirmwareTypeStr[3]   == pstFirmPacket->SpiFlashPacket.Packet.info.bFirmwareType[0])
            && (bFirmwareTypeStr[2]   == pstFirmPacket->SpiFlashPacket.Packet.info.bFirmwareType[1])
            && (bFirmwareTypeStr[1]   == pstFirmPacket->SpiFlashPacket.Packet.info.bFirmwareType[2]))

            {
                (stLocalFirmwareUpdatePackInfo.fUpdateEN) = true;
                (stLocalFirmwareUpdatePackInfo.dwCurrentFirmNum) = 1;//the first package
                (stLocalFirmwareUpdatePackInfo.dwCurrentFirmSum) = dwFirmwareNumber;//sum of firmware packages
            }
            else
            {
               log_error("[boot] ver error\r\n");
               return;// ((psVoidFunc)(BootUARTFunc(sBootUARTICPProcess)))();
            }
        }

        if(!stLocalFirmwareUpdatePackInfo.fUpdateEN)
        {
            log_error("[boot] updata error\r\n");
            return;//((psVoidFunc)(BootUARTFunc(sBootUARTICPProcess)))();
        }

        //convert 4 bytes keys from 12 bytes key data
        (stLocalFirmwareUpdatePackInfo.bKEY)[0] = ((pKEYConvert)BootFlashFunc(sKEYConvert))(0, pstFirmPacket->SpiFlashPacket.Packet.info.bkey);
        (stLocalFirmwareUpdatePackInfo.bKEY)[1] = ((pKEYConvert)BootFlashFunc(sKEYConvert))(1, pstFirmPacket->SpiFlashPacket.Packet.info.bkey);
        (stLocalFirmwareUpdatePackInfo.bKEY)[2] = ((pKEYConvert)BootFlashFunc(sKEYConvert))(2, pstFirmPacket->SpiFlashPacket.Packet.info.bkey);
        (stLocalFirmwareUpdatePackInfo.bKEY)[3] = ((pKEYConvert)BootFlashFunc(sKEYConvert))(3, pstFirmPacket->SpiFlashPacket.Packet.info.bkey);

        //check all pack
        for(dwFirmwareIndex = 1 ; dwFirmwareIndex < dwFirmwareNumber ; dwFirmwareIndex++){
            stLocalFirmwareUpdatePackInfo.dwSpiFlashAddress += wPackageLen + 6;
            wPackageLen = MACRO_read_flash_by_addr(stLocalFirmwareUpdatePackInfo.dwSpiFlashAddress,bBootICPRxBuf);
            if(0 == wPackageLen){
                //log_error("[boot] check len error\r\n");
                return;//((psVoidFunc)(BootUARTFunc(sBootUARTICPProcess)))();
            }
            pstFirmPacket = (LocalUpdateSpiFlashFirmwarePacket *)(bBootICPRxBuf);
            if((dwFirmwareIndex + 1) != *(uint32_t*)pstFirmPacket->SpiFlashPacket.bPacketIndex){
                log_error("[boot] check pack error %d,%d\r\n",dwFirmwareIndex,*(uint32_t*)pstFirmPacket->SpiFlashPacket.bPacketIndex);
                return;
            }
            if((dwFirmwareIndex + 1) < dwFirmwareNumber){
                if(0 != (((*(uint16_t*)pstFirmPacket->SpiFlashPacket.bPacketDataLen)- 2) & (FLASH_BLOCK_WRITE_UNIT_SIZE - 1U))){
                    log_error("[boot] len no alignment\r\n");
                    return;
                }
            }
//                else{
//                log_debug("boot last check\r\n");
//            }
        }
        //reback
        stLocalFirmwareUpdatePackInfo.dwSpiFlashAddress = cdwDevFirmAddrStart + cbFlashFirmInfoLen;
        if (ble_ftp_updata_flag == 1)
        {
            stLocalFirmwareUpdatePackInfo.dwSpiFlashAddress = ble_cdwDevFirmAddrStart + ble_cbFlashFirmInfoLen;
        }
        wPackageLen = MACRO_read_flash_by_addr(stLocalFirmwareUpdatePackInfo.dwSpiFlashAddress,bBootICPRxBuf);
        if((20 >= wPackageLen)){
            log_error("[boot] len error\r\n");
            return;//((psVoidFunc)(BootUARTFunc(sBootUARTICPProcess)))();
        }
        //dwFirmwareIndex = 1;
        log_info("boot erase type %x\r\n",stLocalFirmwareUpdatePackInfo.bUpdateType);

        //erase MCU flash(advance or normal)
        if((cbForceUpdating == stLocalFirmwareUpdatePackInfo.bUpdateType) ||\
           (cbAdvUpdating == stLocalFirmwareUpdatePackInfo.bUpdateType))
        {
            //erase all flash area
            if(FLASH_COMPL != ((pFlashEraseSectors)BootFlashFunc(sBootFlashEraseSectors))(cdwMCU_VECTOR_SECTOR_START, cdwMCU_SECTOR_END))
            {
                log_error("[boot] erase error\r\n");
                return;//((psVoidFunc)(BootUARTFunc(sBootUARTICPProcess)))();
                //return cwMCUEraseError;
            }
            stLocalFirmwareUpdatePackInfo.dwMcuFlashAddress = cdwMCU_VECTOR_SECTOR_START;
        }
        else
        {
            //erase APP flash area
            if(FLASH_COMPL != ((pFlashEraseSectors)BootFlashFunc(sBootFlashEraseSectors))(cdwAPP_SECTOR_START, cdwAPP_SECTOR_END))
            {
                return;//((psVoidFunc)(BootUARTFunc(sBootUARTICPProcess)))();
                //return cwMCUEraseError;
            }
            stLocalFirmwareUpdatePackInfo.dwMcuFlashAddress = cdwAPP_SECTOR_START;
        }

        stLocalFirmwareUpdatePackInfo.dwSpiFlashAddress += wPackageLen + 6;

        wErrorCode = cwFlashOperateOK;
    }
    MACRO_IWDG_RELOD();
    for(;;)
    {
        //loop updating.
        wPackageLen = MACRO_read_flash_by_addr(stLocalFirmwareUpdatePackInfo.dwSpiFlashAddress,bBootICPRxBuf);
        if(0 == wPackageLen){
            return;//((psVoidFunc)(BootUARTFunc(sBootUARTICPProcess)))();
        }
        else
        {
            pstFirmPacket = (LocalUpdateSpiFlashFirmwarePacket *)(bBootICPRxBuf);
            dwFirmwareIndex = *(INT32U*)(pstFirmPacket->SpiFlashPacket.bPacketIndex);
            if((dwPacketIndexBackup + 1) != dwFirmwareIndex)
            {
                //update from flash failed, run UART update process
                return;//((psVoidFunc)(BootUARTFunc(sBootUARTICPProcess)))();
            }
           wErrorCode = MACRO_FLASH_FirmPackOperate(pstFirmPacket,&stLocalFirmwareUpdatePackInfo);

            if(cwFlashOperateOK == wErrorCode)
            {
                stLocalFirmwareUpdatePackInfo.dwSpiFlashAddress += wPackageLen + 6;
                dwPacketIndexBackup = dwFirmwareIndex;
                //update over, write app mask to flash and reboot MCU
                if(dwFirmwareIndex == dwFirmwareNumber)
                {
                    //write app mask data
                    if(FLASH_COMPL != ((pFLASH_Program)BootFlashFunc(sBootFLASH_Program))((INT32U)sAppOkMark, bBootAppMaskBuff, cbAppMaskLen))
                    {
                      return; // ((psVoidFunc)(BootUARTFunc(sBootUARTICPProcess)))();
                    }

                    //flash operate OK, reset MCU
                    //M_SYSTICK_CLOSE();
                    //log_info("spiret\r\n");

                    stUpdateUARTInfo.fAppUARTICP = cbUpdateFinish;
                    M_RESET_MCU();
                }
            }
            else
            {
                bRetryFlashCnt++;
                if(bRetryFlashCnt >= 5)//retry 5 times
                {

                    return;//update from flash failed, run UART update process
                    //((psVoidFunc)(BootUARTFunc(sBootUARTICPProcess)))();
                }
            }
        }

        /*********************************************************
        * 500us timer
        *********************************************************/
        if(M_CHK_SYSTICK_FLAG())
        {
            MACRO_IWDG_RELOD();//            M_BOOT_FEEDDOG();//WAIT  IAP
        }
    }
}


/*************************************************************************************
* Input: bCommand: command of flash; wWDataLen: write data length; pbWData: write data
*        wRDataLen: read data length, pbRData: read data buffer.
*
* Output: void
*
* Logic discription: write data or read data from flash.
*
* Run time:
*************************************************************************************/
//static void sBootFlashCmdW_R(INT8U bCommand, INT16U wWDataLen, INT8U *pbWData, INT16U wRDataLen, INT8U *pbRData)
//{
//    HalBootIsrFlashCSLow();
//
//    //write command to flash through spi
//    //original function:sBootSpiWriteByte()
//    ((pBootSpiWriteByte)BootSpiFlashFunc(sBootSpiWriteByte))(bCommand);
//
//    //looping write data to flash
//    //original function:sBootSpi1WriteBuff()
//    ((pBootSpiWriteBuff)BootSpiFlashFunc(sBootSpiWriteBuff))(wWDataLen, pbWData);
//
//    //looping read data from spi
//    //original function:sBootSpiReadBuff()
//    ((pBootSpiReadBuff)BootSpiFlashFunc(sBootSpiReadBuff))(wRDataLen, pbRData);
//
//    HalBootIsrFlashCSHigh();
//}





/************************************************************************************************
* Input: wDataLen: data length to be read from SPI1; pbData: buffer to be saved.
*
* Output: void
*
* Logic discription: read buffer from MISO
*
* run time:
************************************************************************************************/
//static void sBootSpiReadBuff(INT16U wDataLen, INT8U * pbData)
//{
//	SPIFLASH_CS_LOW();
//
//	flash_write_byte(CMD_READ_DATA);
//
//	if(W25QXX_TYPE == W25Q256)
//	{
//		flash_write_byte((uint8_t)(Addr>>24));
//	}
//	flash_write_byte((uint8_t)(Addr>>16));
//	flash_write_byte((uint8_t)(Addr>>8));
//	flash_write_byte((uint8_t)Addr);
//
//	for(uint16_t i=0; i<Length; i++)
//	{
//		pData[i] = flash_read_byte();
//	}
//
//	SPIFLASH_CS_HIGH();
//
////}
///************************************************************************************************
//* Input: wDataLen: data length to be write to SPI1; pbData.
//*
//* Output: void
//*
//* Logic discription: write buffer to MOSI
//*
//* run time:
//************************************************************************************************/
//static void sBootSpiWriteBuff(INT16U wDataLen, INT8U *pbData)
//{
//    INT16U i;
//
//    for(i = 0; i < wDataLen; i++)
//    {
//        //original function:sBootSpiWriteByte()
//        ((pBootSpiWriteByte)BootSpiFlashFunc(sBootSpiWriteByte))(pbData[i]);
//    }
//}


void sBootSpiFlashCodeEnd(void)
{

}
#endif  //#if(true == HalBootSpiFlashFuncEN)

