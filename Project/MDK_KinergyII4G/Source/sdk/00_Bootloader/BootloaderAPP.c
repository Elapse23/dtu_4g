/************************************************************************************************
File Name:      BootlaoderAPP.C
Description:    bootloader APP functions and variables, code in APP area
Version:        V1.0
Author:         yebh
Date:           2022-11-02
*************************************************************************************************/
#include "BootLoaderAPP.H"
#include "BootSpiFlashCode.h"
#include "BootLoader.h"     // 为 dw_firmware_update_task_delay 等宏定义提供声明
#include <stdint.h>
#include <string.h>     // 为 memset, memcpy, memcmp 提供声明
#include <stdlib.h>     // 为 malloc/free 相关函数提供声明
#include <stdarg.h>     // 为 va_list, va_start, va_end 提供声明
#include "FreeRTOS.h"   // 为 FreeRTOS 函数提供声明
#include "task.h"       // 为 vTaskDelay 提供声明

#include "n32g45x_wwdg.h"

// 声明LOG_PRINTF函数
extern void LOG_PRINTF(int module, int level, const char* fmt, ...);

//待优化
static UpdatePackInfo  appUpdatePackInfo;
static UpdateUARTInfo  appUpdateUARTInfo;
static DevTreeInfo     appBootDevTree[cbMaxNode];
//static UARTTransInfo   appTransUARTInfo;


uint8_t boot_mode   = 0;
uint8_t boot_result = 0;

// 添加缺失的变量定义
uint8_t ble_ftp_updata_flag = 0;  // 蓝牙升级标志，DTU_4G项目中暂时设为0
uint8_t boot_config = 0;
uint8_t boot_flash  = 0;
uint8_t boot_retcnt = 0;

// LOG_PRINTF函数实现 - 映射到当前项目的日志系统
void LOG_PRINTF(int module, int level, const char* fmt, ...)
{
    // 简单映射到当前项目的日志系统
    // 可以根据实际需要调整
    #ifdef DEBUG
    va_list args;
    va_start(args, fmt);
    // 这里应该调用当前项目的日志函数
    // 临时使用简单输出
    va_end(args);
    #endif
}

// 外部Flash函数映射实现（临时占位符）
bool spi_flash_read(uint32_t addr, uint16_t len, uint8_t* data)
{
    // 这里应该调用当前项目的SPI Flash读取函数
    // 临时返回成功
    return true;
}

bool spi_flash_write(uint32_t addr, uint16_t len, uint8_t* data)
{
    // 这里应该调用当前项目的SPI Flash写入函数
    // 临时返回成功
    return true;
}

// RS485通信函数映射实现（临时占位符）
void uart_tx_data_foreground(uint8_t addr, uint8_t func, uint16_t cmd, uint16_t len, uint8_t *data, bool wait_ack)
{
    // 这里应该调用当前项目的UART发送函数
    // 临时空实现
}

// 移除重复定义的变量，它们已经在文件开头定义了

const uint8_t flash_flag_start = 0x00;
//const uint8_t flash_flag_end   = 0;

#pragma pack(1)

typedef struct{
    uint32_t pack;
    uint16_t dlen;
//    uint16_t crc;
}T_HEAD;
#pragma pack()

typedef struct{
    uint8_t bMcuType;
    uint8_t bFirmwareType[3];
    uint8_t bAppVersion[3];
    uint8_t bbootVersion[2];//13
    uint8_t resever[1];
    uint8_t dwFirmStartAddr[4];
    uint8_t dwFirmEndAddr[4];
    uint8_t bkey[cbLocalFirmwarePacketKeyLen];
    uint8_t bFileMD5[cbFirmUUIDLen];
}T_RS485_INFO;
#if(0 != (cdwLocalUpdateFirmDataLen % 4))
#error cdwLocalUpdateFirmDataLen  error
#endif


#if(UPDAT_EXFLASH_TEST)

void debug_printf_sector(uint8_t* dptr,uint16_t dlen)
{
    uint16_t i;
    for(i = 0; i < dlen ; i++){
        debug_updata_test("%02x",dptr[i]);
    }
    debug_updata_test("\r\n");
}
#endif
void crc16_caculate(uint16_t* wCRC, uint16_t wDataLen,uint8_t* pchMsg)
{
    uint16_t chChar;
    uint16_t temp = *wCRC;
    uint16_t i;
    if((NULL == wCRC) &(NULL == pchMsg)){
        return;
    }
    for (i = 0; i < wDataLen; i++)
    {
        chChar = *pchMsg++;
        temp = (INT16U)(((INT16U*)BootRodata(wCRCTableAbs))[(chChar ^ temp) & 15] ^ (temp >> 4));
        temp = (INT16U)(((INT16U*)BootRodata(wCRCTableAbs))[((chChar >> 4) ^ temp) & 15] ^ (temp >> 4));
    }
    *wCRC = temp;
}

// 缺失的CRC函数实现
void calculate_crc16(uint8_t* pdata, uint16_t dlen, uint8_t* crc_low, uint8_t* crc_high)
{
    uint16_t crc = 0;
    crc16_caculate(&crc, dlen, pdata);
    *crc_low = (uint8_t)(crc & 0xFF);
    *crc_high = (uint8_t)(crc >> 8);
}

bool check_crc16(uint8_t* data, uint16_t len)
{
    if (len < 2) return false;
    
    uint16_t received_crc = (data[len-1] << 8) | data[len-2];
    uint16_t calculated_crc = 0;
    crc16_caculate(&calculated_crc, len-2, data);
    
    return (received_crc == calculated_crc);
}

// 波特率设置函数实现
void firmware_update_set_baudrate_9600(void)
{
    // 设置波特率为9600的实现
}

void firmware_update_set_baudrate_115200(void)
{
    // 设置波特率为115200的实现
}

void resver_copy(uint8_t *ptr,uint8_t *src,uint16_t len)
{
    uint16_t i;
    if((NULL == ptr ) ||(NULL == src)){
        return;
    }
    for( i = 0 ; i < len ;i++){
        ptr[len  - 1- i] = src[i];
    }
    //while(len--) {*ptr = src[len];ptr++;}
}



extern uint8_t ble_ftp_updata_flag;
uint16_t updata_data_read_by_addr(uint32_t flash_addr,uint8_t* dptr)
{
    //uint16_t crc  = 0xFFFF;
    //uint8_t crc_buff[2];
    uint16_t len = 0;
    if (ble_ftp_updata_flag == 0)
    {
        if ((NULL == dptr) || (cdwDevFirmAddrStart > flash_addr) || (cdwDevFirmAddrEnd < flash_addr))
        {
            return 0;
        }
    }
    else
    {
        if ((NULL == dptr) || (ble_cdwDevFirmAddrStart > flash_addr) || (ble_cdwDevFirmAddrEnd < flash_addr))
        {
            return 0;
        }
    }

    //MACRO_IWDG_RELOD();
    if (ble_ftp_updata_flag == 0)
    {
        if (cdwPackFirmAddrStart == flash_addr)
        {
            if (false == extern_flash_read(cdwDevFirmAddrStart, 1, dptr))
            {
                debug_updata_error("[FLASH]write flage fail %x\r\n", flash_addr);
                return 0;
            }
            if (0 != dptr[0])
            {
                debug_updata_error("[FLASH]write flage error %x\r\n", flash_addr);
                return 0;
            }
        }
    }
    else
    {
        if (ble_cdwPackFirmAddrStart == flash_addr)
        {
            if (false == extern_flash_read(ble_cdwDevFirmAddrStart, 1, dptr))
            {
                debug_updata_error("[FLASH]write flage fail %x\r\n", flash_addr);
                return 0;
            }
            if (0 != dptr[0])
            {
                debug_updata_error("[FLASH]write flage error %x\r\n", flash_addr);
                return 0;
            }
        }
    }
    //read head
    if(false == extern_flash_read(flash_addr + 4,2,(uint8_t*)&len)){
        debug_updata_error("[FLASH]read head fail  %x\r\n",flash_addr);
        return 0;
    }
    if((cdwLocalUpdateFirmDataLen  < len)||(0 == len )){
        debug_updata_error("[FLASH]read len error  %x,[%d]\r\n",flash_addr, len);
        return 0;

    }
    //read data
    if(false == extern_flash_read(flash_addr,len + 6,dptr)){
        debug_updata_error("[FLASH]read data fail %x\r\n",flash_addr);
        return 0;
    }
//    if(0 != CRC16_2(&dptr[6],len)){
    if(1 != check_crc16(&dptr[6],len)){
        debug_updata_error("[FLASH]read crc error %x\r\n",flash_addr);
//        debug_updata_error("crc cac %x\r\n",CRC16_2(dptr,(len)));
        debug_updata_error("crc read %x%x\r\n",dptr[len + 0],dptr[len + 1]);
        return 0;
    }
    //MACRO_IWDG_RELOD();
    return len;

}






bool updata_data_check_all(void)
{
    uint16_t len = 0;
    LocalUpdateSpiFlashFirmwarePacket* buff;
    LocalFirmwareUpdatePackInfo info;

    MALLOC_RAM_TYPE(buff,LocalUpdateSpiFlashFirmwarePacket);
   // MALLOC_RAM_TYPE(info,LocalFirmwareUpdatePackInfo);
    if(NULL == buff){
        debug_updata_error("ram fail\r\n");
        return false;
    }

    if (ble_ftp_updata_flag == 0)
    {
        info.dwSpiFlashAddress = cdwPackFirmAddrStart;
    }
    else
    {
        info.dwSpiFlashAddress = ble_cdwPackFirmAddrStart;
    }
    len = updata_data_read_by_addr(info.dwSpiFlashAddress,buff->bData);

    if(0 == len){
        debug_updata_error("read fail\r\n");
        vPortFree(buff);
        return false;
    }


    //resver_copy((uint8_t*)info.dwCurrentFirmSum,buff->SpiFlashPacket.Packet.info.bPacketsNumber,4);
    memcpy((uint8_t*)&info.dwCurrentFirmSum,buff->SpiFlashPacket.Packet.info.bPacketsNumber,4);
    debug_updata_test("check total %d pack%d,len %d\r\n",info.dwCurrentFirmSum,*((uint32_t*)buff->SpiFlashPacket.bPacketIndex),len);

    //convert 4 bytes keys from 12 bytes key data
//    (info->bKEY)[0] = ((pKEYConvert)BootFlashFunc(sKEYConvert))(0, buff->SpiFlashPacket.Packet.info.bkey);
//    (info->bKEY)[1] = ((pKEYConvert)BootFlashFunc(sKEYConvert))(1, buff->SpiFlashPacket.Packet.info.bkey);
//    (info->bKEY)[2] = ((pKEYConvert)BootFlashFunc(sKEYConvert))(2, buff->SpiFlashPacket.Packet.info.bkey);
//    (info->bKEY)[3] = ((pKEYConvert)BootFlashFunc(sKEYConvert))(3, buff->SpiFlashPacket.Packet.info.bkey);



    if(1 >= info.dwCurrentFirmSum){
        debug_updata_error("total fail %d len %d\r\n",info.dwCurrentFirmSum,len);
        debug_updata_sector(buff->SpiFlashPacket.Packet.info.bPacketsNumber,4);
        vPortFree(buff);
        return false;
    }


    MACRO_IWDG_RELOD();
    for(info.dwCurrentFirmNum = 1; info.dwCurrentFirmNum < info.dwCurrentFirmSum; info.dwCurrentFirmNum++){
        info.dwSpiFlashAddress += len + 6;
        len = updata_data_read_by_addr(info.dwSpiFlashAddress,buff->bData);
        if((0 == len)
            //||(0 != ((len - 2) % FLASH_BLOCK_WRITE_UNIT_SIZE))//must 4
            ||((info.dwCurrentFirmNum + 1) != *((uint32_t*)buff->SpiFlashPacket.bPacketIndex))){
            debug_updata_error("read fail pack %d-%d,len %d\r\n",info.dwCurrentFirmNum + 1,*((uint32_t*)buff->SpiFlashPacket.bPacketIndex),len);
//            if(0 != ((len - 2) % FLASH_BLOCK_WRITE_UNIT_SIZE)){//must 4
//                debug_updata_error("Four byte alignment error pack %d[%d]",len);
//            }
            vPortFree(buff);
            return false;
        }

//        debug_updata_test("[FLASH] pack%d[%d]\r\n",info.dwCurrentFirmNum + 1,len - 2);
//        ((pCiphertext2Plaintext)BootFlashFunc(sBootCiphertext2Plaintext))(len - 2,
//                                                                          buff->SpiFlashPacket.Packet.data.bPacketData,
//                                                                          buff->SpiFlashPacket.Packet.data.bPacketData,
//                                                                          info->bKEY);
//        debug_updata_sector(buff->SpiFlashPacket.Packet.data.bPacketData,len - 2);

    }
    MACRO_IWDG_RELOD();
    if((info.dwCurrentFirmSum) == *((uint32_t*)buff->SpiFlashPacket.bPacketIndex)){
        debug_updata_test("check success \r\n");
        vPortFree(buff);
        return true;
    }else{
        debug_updata_test("check fail\r\n");
        vPortFree(buff);
        return false;
    }

}



bool exflash_write_pack(uint32_t pack,uint8_t*pdata,uint16_t dlen)
{
    static uint32_t exflash_addr = cdwPackFirmAddrStart;
    if (ble_ftp_updata_flag == 1)
    {
        exflash_addr = ble_cdwPackFirmAddrStart;
    }
    T_HEAD flash_head;
    uint16_t crc = 0;;
    if((NULL == pdata) ||(dlen > cdwLocalUpdateFirmDataLen) ||(0 == dlen )){
        return false;
    }
    MACRO_IWDG_RELOD();
    if (ble_ftp_updata_flag == 0)
    {
        if (1 == pack)
        {
            if (false == extern_flash_wirte(cdwDevFirmAddrStart, cbFlashFirmInfoLen, ( uint8_t * )&flash_flag_start))
            {
                debug_updata_error("[flash]write flag fail\r\n");
                return cwFlashOperateError;
            }
            exflash_addr = cdwPackFirmAddrStart;
        }
    }
    else
    {
        if (1 == pack)
        {
            if (false
                == extern_flash_wirte(ble_cdwDevFirmAddrStart, ble_cbFlashFirmInfoLen, ( uint8_t * )&flash_flag_start))
            {
                debug_updata_error("[flash]write flag fail\r\n");
                return cwFlashOperateError;
            }
            exflash_addr = ble_cdwPackFirmAddrStart;
        }
    }
    flash_head.pack = pack;
    flash_head.dlen = dlen + 2;
//    crc  = CRC16_2(pdata,dlen);
    calculate_crc16(pdata,dlen,(uint8_t *)(&crc),((uint8_t *)(&crc) + 1));
    if(false == extern_flash_wirte(exflash_addr,6,(uint8_t*)&flash_head)){
        debug_updata_error("[flash]write head fail\r\n");
        return false;
    }
    if(false == extern_flash_wirte(exflash_addr + 6,dlen,pdata)){
        debug_updata_error("[flash]write data fail %x[%d]\r\n",exflash_addr,dlen);
        return false;
    }
    if(false == extern_flash_wirte((exflash_addr + 6 + dlen),2,(uint8_t*)&crc)){
        debug_updata_error("[flash]write crc fail\r\n");
        return false;
    }
    MACRO_IWDG_RELOD();
    if((dlen + 2) != updata_data_read_by_addr(exflash_addr,pdata)){
        debug_updata_error("[flash]read len fail\r\n");
        return false;
    }
    exflash_addr += 8 + dlen;

    return true;

}


bool app_version_type_check(T_FLASH_INFO   info,uint32_t addr_start)
{
    uint32_t firmware_type;
    firmware_type = (info.bFirmwareType[2] << 16) + (info.bFirmwareType[1] << 8) + (info.bFirmwareType[0] << 0);
    switch (info.bUpdateType){
    case cbNormalUpdating:
        if((cbFirmwareType1      == info.bMcuType)
        && (cdwFirmwareType2     == firmware_type)
        && (info.bbootVersion[1] == bBootFirmwareStr[0])
        && (info.bbootVersion[0] == bBootFirmwareStr[1])
        && (cdwAPP_SECTOR_START  == addr_start)){
            return true;
        }

        break;
    case  cbAdvUpdating:
        if((cbFirmwareType1      == info.bMcuType)
        && (cdwFirmwareType2     == firmware_type)
        && (cdwMCU_SECTOR_START  == addr_start)){
            return true;
        }
        break;
    case cbForceUpdating:
        if((cbFirmwareType1      == info.bMcuType)
        && ( cdwMCU_SECTOR_START == addr_start)){
            return true;
        }
        break;
    }
    debug_updata_info("mcu:%d,type:%d,boot:%02x%02x\r\n",info.bMcuType,firmware_type,info.bbootVersion[1],info.bbootVersion[0]);
    return false;
}
uint16_t rs485_updata_self_config(uint8_t* buf,uint16_t len)
{

    LocalUpdateSpiFlashFirmwarePacket* pstFirmPacket;
    uint16_t index;
    T_RS485_INFO *dptr;
    uint16_t totale;
    //uint32_t fire_addr = cdwDevFirmAddrStart;
    index = buf[1] * 3 + 2;
    //uint16_t crc = 0;

    //check
    if(len < (sizeof(T_RS485_INFO) + index)){
       debug_updata_error("[updata] len error %d\r\n",len);
       return cwUpdateDataError;
    }
    dptr = (T_RS485_INFO *)&buf[index];
    resver_copy((uint8_t*)&appUpdatePackInfo.dwFirmStartAddr,dptr->dwFirmStartAddr,4);//small endian save
    resver_copy((uint8_t*)&appUpdatePackInfo.dwFirmEndAddr,dptr->dwFirmEndAddr,4);//small endian save



    totale = (appUpdatePackInfo.dwFirmEndAddr - appUpdatePackInfo.dwFirmStartAddr) /  UPDATA_FROM_RS485_PAGE_LEN + 2;
    //debug_updata_info("[updata]start addr %x, end addr%x totale %d\r\n",
    //    stUpdatePackInfo.dwFirmStartAddr,stUpdatePackInfo.dwFirmEndAddr,totale);

    //check boot ver





    //copy
    MALLOC_RAM_TYPE(pstFirmPacket,LocalUpdateSpiFlashFirmwarePacket);
    if(NULL == pstFirmPacket){
       debug_updata_error("[updata] ram fail\r\n");
       return cwUpdateDataError;
    }
    if(buf[0] < 1){
        return cwUpdateDataError;
    }
    pstFirmPacket->SpiFlashPacket.Packet.info.bUpdateType = buf[0] - 1;//cbNormalUpdating;//buf[0]; 烧录类型 1 2 3  -> 0 1 2
    pstFirmPacket->SpiFlashPacket.Packet.info.bMcuType = dptr->bMcuType;
    resver_copy(pstFirmPacket->SpiFlashPacket.Packet.info.bFirmwareType,dptr->bFirmwareType,3);
    resver_copy(pstFirmPacket->SpiFlashPacket.Packet.info.bAppVersion,dptr->bAppVersion,3);
    resver_copy(pstFirmPacket->SpiFlashPacket.Packet.info.bbootVersion,dptr->bbootVersion,2);
    memcpy(pstFirmPacket->SpiFlashPacket.Packet.info.bkey,dptr->bkey,cbLocalFirmwarePacketKeyLen);

    memcpy(pstFirmPacket->SpiFlashPacket.Packet.info.bPacketsNumber,(uint8_t*)&totale,4);

    len = 26;
    if(cdwAPP_SECTOR_END < appUpdatePackInfo.dwFirmStartAddr){
        return cwUpdateAddrError;
    }
    if(false  == app_version_type_check(pstFirmPacket->SpiFlashPacket.Packet.info,appUpdatePackInfo.dwFirmStartAddr)){
        return cwUpdateAddrError;
    }
    if((true == appUpdatePackInfo.fMD5VerifyOK)&&(0 == memcmp(appUpdatePackInfo.bFileMD5,dptr->bFileMD5,cbFirmUUIDLen))){
        debug_updata_info("[updata]continue\r\n");
        return cwFlashOperateOK;
    } //continue no save


    if(false  == exflash_write_pack(1,pstFirmPacket->SpiFlashPacket.Packet.bData,len)){
        vPortFree(pstFirmPacket);
        debug_updata_error("[updata]flash read fail\r\n");
        return cwFlashOperateError;
    }
    appUpdatePackInfo.dwCurrentFirmAddr  = 0;
    appUpdatePackInfo.wLastFirmLen = 0;
    appUpdatePackInfo.fMD5VerifyOK = true;
    debug_updata_info("[updata]new updata\r\n");
    memcpy(appUpdatePackInfo.bFileMD5,dptr->bFileMD5,cbFirmUUIDLen);//copy to save
    vPortFree(pstFirmPacket);


    return cwFlashOperateOK;

}


uint16_t rs485_updata_self_code(uint8_t* buf,uint16_t len)
{
    uint32_t pack       = 0;
    //uint16_t crc        = 0;
    uint32_t index_addr = 0;
   // LocalUpdateSpiFlashFirmwarePacket* pstFirmPacket;

    if(false == appUpdatePackInfo.fMD5VerifyOK){
        debug_updata_info("[updata]no check m5d\r\n");
        return cwUpdateDataError;
    }

    if((len < 4) ||(len > ( UPDATA_FROM_RS485_PAGE_LEN + 4))){
        debug_updata_error("[updata]len error %d\r\n",len);
        return cwUpdateDataError;
    }

    resver_copy((uint8_t*)&index_addr,&buf[0],4);
    debug_updata_test("[updata]rec index %x[%d]\r\n",index_addr,len);

    if(index_addr != (appUpdatePackInfo.dwCurrentFirmAddr) ){//Repeat packet
        if(index_addr < (appUpdatePackInfo.dwCurrentFirmAddr)){
            debug_updata_error("[updata]addr repeat%x-%x[%d]\r\n",index_addr,appUpdatePackInfo.dwCurrentFirmAddr,appUpdatePackInfo.wLastFirmLen);
            return cwFlashOperateOK;//repeat no handle
        }else{
            debug_updata_error("[updata]addr advance%x-%x[%d]\r\n",index_addr,appUpdatePackInfo.dwCurrentFirmAddr,appUpdatePackInfo.wLastFirmLen);
            return cwUpdateAddrError;//advance
        }
    }
    len -= 4;
    pack = (index_addr /  UPDATA_FROM_RS485_PAGE_LEN) + 2;


    if(false == exflash_write_pack(pack,&buf[4],len)){
        debug_updata_error("[updata]flash read fail\r\n");
        return cwFlashOperateError;
    }

    debug_updata_info("[updata]pack%04d[%d]%x/%x\r\n",\
    pack,len,\
    appUpdatePackInfo.dwCurrentFirmAddr + appUpdatePackInfo.dwFirmStartAddr,\
    appUpdatePackInfo.dwFirmEndAddr + 1);
    //debug_updata_sector(&buf[4],16);



    appUpdatePackInfo.dwCurrentFirmAddr += len;
    appUpdatePackInfo.wLastFirmLen = len;



    return cwFlashOperateOK;
}



const uint32_t baud_rate[7]  ={1200,2400,4800,9600,19200,38400,115200};//3 6
const uint32_t baud_delay[7] ={800,400,200,100,50,20,10};

void boot_cmd_handle(uint8_t* rx_buf,uint16_t rx_len)
{
    INT8U i,j;
    DevTreeInfo *pDevTreeTempIn;
    uint8_t* tx_buf;
    uint16_t tx_len = 0;
    uint16_t dlen   = 0;
    uint16_t result = 0;
    uint16_t cmd    = 0;
    static uint8_t  flash_error = 0;
    if(true == check_crc16(rx_buf, rx_len)){
       cmd  = (INT16U)((rx_buf[2] << 8) + rx_buf[3]);
       dlen = (INT16U)((rx_buf[4] << 8) + rx_buf[5]);
       debug_updata_info("cmd:%02x[%d]\r\n",cmd,dlen);
       MALLOC_RAM_LEN(tx_buf,300);
        switch(cmd){
        case cwICP_CMD_RX_DISABLE_ADDR:
            if(2 == dlen){
                if((0 == rx_buf[6]) && (1 == rx_buf[7]))
                {
                    appUpdateUARTInfo.fBootUARTUpdateTxDisable = true;
                }
            }
            break;
        case cwICP_CMD_RX_ENABLE_ADDR://2
            if(2 == dlen){
                if((0 == rx_buf[6]) && (1 == rx_buf[7]))
                {
                    appUpdateUARTInfo.fBootUARTUpdateTxDisable = false;
                    appUpdateUARTInfo.fQueryAddrOK = false;
                    if(rx_buf[0] != 0xF7)
                    {
                        tx_buf[0] = 0;
                        tx_buf[1] = 0;
                        tx_len = 2;
                    }
                }
            }
            break;
        case cwICP_CMD_DEVICE_TREE_ADDR://3
            if(2 == dlen){
                if((0 == rx_buf[6]) && (1 == rx_buf[7])){
                    if((appBootDevTree->bCommuCH) <= cbMaxBootSubsets){
                        tx_len = (INT16U)(cbDevTreeInfoLen * ((appBootDevTree->bCommuCH) + 1));
                        for(i = 0;i < ((appBootDevTree->bCommuCH) + 1);i++){
                            pDevTreeTempIn = (DevTreeInfo*)((INT32U)appBootDevTree + (i * cbDevTreeInfoStLen));
                            tx_buf[i * cbDevTreeInfoLen + 0] = (pDevTreeTempIn->bCommuAddr);
                            for(j = 0; j < cbDevIDLen; j++)
                            {
                                tx_buf[i * cbDevTreeInfoLen + j + 1] = (pDevTreeTempIn->bDevID)[j];
                            }
                            for(j = 0; j < cbFirmwareVerAllLen; j++)
                            {
                                tx_buf[i * cbDevTreeInfoLen + j + 4] = (pDevTreeTempIn->bFirmwareVer)[j];
                            }
                            tx_buf[i * cbDevTreeInfoLen + 14] = (pDevTreeTempIn->bDevSubsetNum);
                        }
                    }
                }
             }
             break;
        case cwICP_CMD_START_COOK_ADDR://4
            if(67 <= dlen){
                result =  rs485_updata_self_config(&rx_buf[6],rx_len - 6 - 2);//crc[2]
                tx_buf[0] = (INT8U)(((appUpdatePackInfo.dwCurrentFirmAddr) & 0xFF000000) >> 24);
                tx_buf[1] = (INT8U)(((appUpdatePackInfo.dwCurrentFirmAddr) & 0x00FF0000) >> 16);
                tx_buf[2] = (INT8U)(((appUpdatePackInfo.dwCurrentFirmAddr) & 0x0000FF00) >> 8);
                tx_buf[3] = (INT8U)((appUpdatePackInfo.dwCurrentFirmAddr) & 0x000000FF);
                tx_buf[4] = (INT8U)((UPDATA_FROM_RS485_PAGE_LEN & 0xFF00) >> 8);
                tx_buf[5] = (INT8U)(UPDATA_FROM_RS485_PAGE_LEN & 0x00FF);
                tx_buf[6] = (result >> 8) & 0xff;
                tx_buf[7] = (result >> 0) & 0xff;
                tx_len = 8;
                if(cwFlashOperateError == result){
                    flash_error++;
                }else{
                    flash_error = 0;
                }
            }
            break;
        case cwICP_CMD_DATA_TRF_ADDR://5
            result =  rs485_updata_self_code(&rx_buf[6],rx_len - 6 - 2);//crc[2]
            tx_buf[0] = (INT8U)(((appUpdatePackInfo.dwCurrentFirmAddr) & 0xFF000000) >> 24);
            tx_buf[1] = (INT8U)(((appUpdatePackInfo.dwCurrentFirmAddr) & 0x00FF0000) >> 16);
            tx_buf[2] = (INT8U)(((appUpdatePackInfo.dwCurrentFirmAddr) & 0x0000FF00) >> 8);
            tx_buf[3] = (INT8U)((appUpdatePackInfo.dwCurrentFirmAddr) & 0x000000FF);
            tx_buf[4] = (result >> 8) & 0xff;
            tx_buf[5] = (result >> 0) & 0xff;
            tx_len = 6;
            if(cwFlashOperateError == result){
                flash_error++;
            }else{
                flash_error = 0;
            }
            break;
        case cwICP_CMD_FINISH_COOK_ADDR://6
            if((appUpdatePackInfo.dwCurrentFirmAddr + appUpdatePackInfo.dwFirmStartAddr) == (appUpdatePackInfo.dwFirmEndAddr + 1)){
                tx_buf[0] = 00;
                tx_buf[1] = 00;
                tx_len = 2;
                rs485_tx_data_foreground(appUpdateUARTInfo.bRs485Addr,0x04,0x8000|cmd,tx_len,tx_buf,true);
                tx_len = 0;
                if(true == updata_data_check_all()){
                    result = cwFlashOperateOK;
                }else{
                    result = cwFlashOperateError;
                }
            }else{
                debug_updata_info("[updata]finish addr errro\r\n");
                tx_buf[0] = 00;
                tx_buf[1] = cwFlashOperateError;
                tx_len = 2;
                rs485_tx_data_foreground(appUpdateUARTInfo.bRs485Addr,0x04,0x8000|cmd,tx_len,tx_buf,true);
                tx_len = 0;
            }
            if(cwFlashOperateOK == result){
                memset(&appUpdatePackInfo,0,sizeof(UpdatePackInfo));
                debug_updata_info("[updata]into boot\r\n");
                stUpdateUARTInfo.fAppSPIFLASHICP = TRUE;//specil to boot
                stUpdateUARTInfo.fAppUARTICP  = cbUpdateFromFlash;//specil to boot
                ((psVoidFunc)(dwBootExtTbl[cbBootLoaderStart]))();
            }
            break;
        case cwICP_CMD_QUERY_ADDR_ADDR://7//code:0x07 ask for the address
            if(2 == dlen){
                if((0 == rx_buf[6]) && (1 == rx_buf[7]))
                {
                    if(!(appUpdateUARTInfo.fBootUARTUpdateTxDisable))
                    {
                        tx_buf[0] = 0;
                        tx_buf[1] = (appUpdateUARTInfo.bRs485Addr);
                        tx_len = 2;
                    }
                }
            }else{
                if((0 == rx_buf[6]) && (1 == rx_buf[7])&& (rx_buf[0] == 0xF7)&&((appUpdateUARTInfo.fQueryAddrOK) == false))
                {
                    tx_buf[0] = 0;
                    tx_buf[1] = (appUpdateUARTInfo.bRs485Addr);
                    tx_len = 2;
                }
                else if((0 == rx_buf[6]) && (1 == rx_buf[7])&&(rx_buf[0] == (appUpdateUARTInfo.bRs485Addr)))
                {
                    (appUpdateUARTInfo.fQueryAddrOK) = true;
                }
            }
            break;
        case cwICP_CMD_SET_BAUDRATE_ADDR://8
            if(2 == dlen){
                if(0 == rx_buf[6]){
                    if(rx_buf[7] < cbSetBandRateMax)
                    {
                        appUpdateUARTInfo.bBootBaudrate = rx_buf[7];
                        tx_buf[0] = 0;
                        tx_buf[1] = 0;
                        tx_len = 2;

                        rs485_tx_data_foreground(appUpdateUARTInfo.bRs485Addr,0x04,0x8000|cmd,tx_len,tx_buf,true);
                        debug_updata_info("[updata]rate %d delay %d\r\n",baud_rate[appUpdateUARTInfo.bBootBaudrate],dw_firmware_update_task_delay);
                        MACRO_IWDG_RELOD();
                        vTaskDelay(pdMS_TO_TICKS_USER(300));//10ms*10ms + sendinterver
                        if(appUpdateUARTInfo.bBootBaudrate == 3){
                           FIRMWARE_UPDATE_SET_BUADRATE_9600();
                        }else if(appUpdateUARTInfo.bBootBaudrate == 6){
                           FIRMWARE_UPDATE_SET_BUADRATE_115200();
                       }
                       tx_len = 0;
                    }
                }
            }
            break;
        default:
            tx_len = 0;
            break;
        }
    }
    if(tx_len){
        rs485_tx_data_foreground(appUpdateUARTInfo.bRs485Addr,0x04,0x8000|cmd,tx_len,tx_buf,true);
    }
    vPortFree(tx_buf);

    }


void boot_updata_test(void)
{
    //test uart
 //
//    debug_updata_info("boot_config_init\r\n");
//    debug_updata_sector((uint8_t*)test,2);
//    if(true == updata_data_check_all()){
//        debug_updata_info("app into boot\r\n");
//        stUpdateUARTInfo.fAppUARTICP    = cbUpdateFromFlash;//cbUpdateFromICP; upadat flash firs
//        ((psVoidFunc)(dwBootExtTbl[cbBootLoaderStart]))();//
//    }

    //test wiress
//    #include "Eeprom.h"
//    b_local_firmware_enable = TRUE;
//    fWriteEEPage(15) = TRUE;
//    st_device_update_info_current.pst_net_device_update_info_current = (net_device_update_info *)((st_net_system_info_list.
//                                                                                                   pst_system_info[0].
//                                                                                                   pst_net_device_update_info_list->
//                                                                                                   pst_net_device_update_info) + 0);
//
//
}

void AppDeviceTreeInit(DevTreeInfo *DevTree)
{
    INT8U i;
    INT8U j;
    //memset(&DevTree,0,sizeof(DevTreeInfo)* cbMaxNode);
    //fill device tree

    //Serial_Num
    //read from eeprom
    //device ID
    (DevTree[0].bCommuAddr) = cbCommuAddr;
     DevTree[0].bCommuCH   = 0;
    (DevTree[0].bDevID)[0] = (INT8U)((cwDeviceType & 0xFF00) >> 8);
    (DevTree[0].bDevID)[1] = (INT8U)(cwDeviceType & 0x00FF);
    (DevTree[0].bDevID)[2] = 0x00;

    //firmware string
    j = 0;
    //firmware type
    for(i = 0; i < cbFirmwareTypeStrLen; i++)
    {
        (DevTree[0].bFirmwareVer)[j++] = bFirmwareTypeStr[i];
    }

    for(i = 0; i < cbFirmwareVersionLen; i++)
    {
        (DevTree[0].bFirmwareVer)[j++] = bFrimwareVerStr[i];
    }

    for(i = 0; i < cbBootFirmwareStrLen; i++)
    {
        (DevTree[0].bFirmwareVer)[j++] = bBootFirmwareStr[i];
    }

    //rest of firmware string filled of 0
    while(j < cbFirmwareVerAllLen)
    {
        (DevTree[0].bFirmwareVer)[j++] = 0;
    }

    //number of subsets
//    (stBootDevTree[0].bDevSubsetNum) = 0;

}
void UpdateUARTInfoinit(UpdateUARTInfo* config)
{
    memset(config,0,sizeof(UpdateUARTInfo));
    config->fBootUARTUpdateTxDisable = false;
    config->fBootUARTUpdateReg       = false;
    config->fBootStartUpdate         = false;
    config->fTransparence            = false;
    config->fQueryAddrOK             = false;
    config->bRs485Addr               = cbUART5Rs485Addr;//cbUART5Rs485Addr;
    config->bICPTxCH                 = cbUART5;
    config->bBootBaudrate            = cbSetBandRate9600;
    config->fAppUARTICP              = cbUpdateFromapp;//??è?FLASHéy??
    config->bParity                  = cbUART5Parity;
    config->fBootUARTUpdateTxDisable = RESET;
}




void boot_config_init(void)
{
    //uint8_t offset = 0;

  //  debug_updata_sector((uint8_t*)bFrimwareVerStr,3);
 //   boot_updata_test();

    memset(&appUpdatePackInfo,0,sizeof(UpdatePackInfo));
    //memset(&appTransUARTInfo,0,sizeof(UARTTransInfo));
    AppDeviceTreeInit(appBootDevTree);
    UpdateUARTInfoinit(&appUpdateUARTInfo);

    MACRO_IWDG_RELOD();
    MACRO_IWDG_OPEN();   // 打开看门狗






}

