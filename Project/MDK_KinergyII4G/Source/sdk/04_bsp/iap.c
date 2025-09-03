#include "iap.h"
#include "gd32f4xx.h"


uint32_t iapbuf[512];//预留2k byte


typedef  void (*iapfun)(void);				//定义一个函数类型的参数.   

iapfun jump2app; 


/*
static uint8_t STMFLASH_GetFlashSector(uint32_t addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)return CTL_SECTOR_NUMBER_0;
	else if(addr<ADDR_FLASH_SECTOR_2)return CTL_SECTOR_NUMBER_1;
	else if(addr<ADDR_FLASH_SECTOR_3)return CTL_SECTOR_NUMBER_2;
	else if(addr<ADDR_FLASH_SECTOR_4)return CTL_SECTOR_NUMBER_3;
	else if(addr<ADDR_FLASH_SECTOR_5)return CTL_SECTOR_NUMBER_4;
	else if(addr<ADDR_FLASH_SECTOR_6)return CTL_SECTOR_NUMBER_5;
	else if(addr<ADDR_FLASH_SECTOR_7)return CTL_SECTOR_NUMBER_6;
	else if(addr<ADDR_FLASH_SECTOR_8)return CTL_SECTOR_NUMBER_7;
	else if(addr<ADDR_FLASH_SECTOR_9)return CTL_SECTOR_NUMBER_8;
	else if(addr<ADDR_FLASH_SECTOR_10)return CTL_SECTOR_NUMBER_9;
	else if(addr<ADDR_FLASH_SECTOR_11)return CTL_SECTOR_NUMBER_10; 
	else if(addr<ADDR_FLASH_SECTOR_12)return CTL_SECTOR_NUMBER_11; 
	else if(addr<ADDR_FLASH_SECTOR_13)return CTL_SECTOR_NUMBER_12; 
	else if(addr<ADDR_FLASH_SECTOR_14)return CTL_SECTOR_NUMBER_13; 
	else if(addr<ADDR_FLASH_SECTOR_15)return CTL_SECTOR_NUMBER_14; 
	else if(addr<ADDR_FLASH_SECTOR_16)return CTL_SECTOR_NUMBER_15; 
	else if(addr<ADDR_FLASH_SECTOR_17)return CTL_SECTOR_NUMBER_16; 
	else if(addr<ADDR_FLASH_SECTOR_18)return CTL_SECTOR_NUMBER_17; 
	else if(addr<ADDR_FLASH_SECTOR_19)return CTL_SECTOR_NUMBER_18; 
	else if(addr<ADDR_FLASH_SECTOR_20)return CTL_SECTOR_NUMBER_19; 
	else if(addr<ADDR_FLASH_SECTOR_21)return CTL_SECTOR_NUMBER_20; 
	else if(addr<ADDR_FLASH_SECTOR_22)return CTL_SECTOR_NUMBER_21; 
	else if(addr<ADDR_FLASH_SECTOR_23)return CTL_SECTOR_NUMBER_22;  
	return CTL_SECTOR_NUMBER_23;	
}
*/


static void IAP_WriteFlash(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite)
{                                          

//	uint32_t addrx=0;
	uint32_t endaddr=0;	
	
//	fmc_unlock();                                                        // 解锁闪存
		
//	addrx   = WriteAddr;				//写入的起始地址
	endaddr = WriteAddr + NumToWrite * 4;	//写入的结束地址
	
	    
//	if(addrx < 0X1FFF0000)
    {
//        while(addrx < endaddr)		//扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
		{
////			fmc_sector_erase(STMFLASH_GetFlashSector(addrx));
//			fmc_sector_erase(CTL_SECTOR_NUMBER_1);
//			fmc_sector_erase(CTL_SECTOR_NUMBER_2);
//			fmc_sector_erase(CTL_SECTOR_NUMBER_3);
//			fmc_sector_erase(CTL_SECTOR_NUMBER_4);
//			fmc_sector_erase(CTL_SECTOR_NUMBER_5);
//			fmc_sector_erase(CTL_SECTOR_NUMBER_6);
//			fmc_sector_erase(CTL_SECTOR_NUMBER_7);
//			fmc_sector_erase(CTL_SECTOR_NUMBER_8);
			
//			addrx+=0x00004000;	//最小扇区16k
			
//			if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//有非0XFFFFFFFF的地方,要擦除这个扇区
//			{   
//                FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //擦除类型，扇区擦除 
//                FlashEraseInit.Sector=STMFLASH_GetFlashSector(addrx);   //要擦除的扇区
//                FlashEraseInit.NbSectors=1;                             //一次只擦除一个扇区
//                FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //电压范围，VCC=2.7~3.6V之间!!
//                if(HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError)!=HAL_OK) 
//                {
//                    break;//发生错误了	
//                }
//			}
//			else 
//			{	
//			}
//            FLASH_WaitForLastOperation(FLASH_WAITETIME);                //等待上次操作完成
        }
    }
			
	while(WriteAddr<endaddr)//写数据
	{
		fmc_word_program(WriteAddr,*pBuffer);//写入数据

		WriteAddr+=4;
		pBuffer++;
	} 
	
//	fmc_lock();                                                          // 上锁
}


void iap_write_app(uint32_t appxaddr, uint8_t *appbuf, uint32_t appsize)
{

	uint16_t t;
	uint16_t i=0;
	uint32_t temp;
	uint32_t fwaddr=appxaddr;//当前写入的地址
	uint8_t *dfu=appbuf;
		
	for(t=0;t<appsize;t+=4)
	{						   
		temp=(uint32_t)dfu[3]<<24;   
		temp|=(uint32_t)dfu[2]<<16;    
		temp|=(uint32_t)dfu[1]<<8;
		temp|=(uint32_t)dfu[0];	  
		dfu+=4;//偏移4个字节
		iapbuf[i++]=temp;	    
		if(i==512)
		{
			i=0; 
			IAP_WriteFlash(fwaddr,iapbuf,512);
			fwaddr+=2048;//偏移2048  512*4=2048
		}
	} 	
	
	if(i)
	{
		IAP_WriteFlash(fwaddr,iapbuf,i);//将最后的一些内容字节写进去.  
	}
}


//跳转到应用程序段
//appxaddr:用户代码起始地址.
void iap_load_app(uint32_t appxaddr)
{ 
//	if(((*(__IO uint32_t*)appxaddr)&0x2FFE0000) == 0x20000000)	//检查栈顶地址是否合法
	{ 
		__disable_irq();//close irq
		jump2app=(iapfun)*(__IO uint32_t*)(appxaddr+4);			//用户代码区第二个字为程序开始地址(复位地址)		
		__set_MSP(*(__IO uint32_t*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		jump2app();												//跳转到APP.
	}
}

