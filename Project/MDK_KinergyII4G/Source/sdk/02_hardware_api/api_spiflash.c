#include "tsk_spiflash.h"
#include "crc16.h"
#include "string.h"

#include "api_esp_tcp_ble.h"


//uint16_t byte_temp=0, cnt_temp=0 ,record_temp=0, RecordNumxxx = 0;






SYSTEM_PARA_DATATYPE g_SystemPara;

static uint16_t SYS_PARA_RECORD_LENGTH = sizeof(SYSTEM_PARA_DATATYPE);		//一条系统参数的长度
void api_erase_system_para(void);

void api_spi_flash_init(void)
{
	drv_spi_flash_init();
}




/********

上电检测存储版本，当版本号发生变化时，全片擦除

********/
void api_flash_version_init(void)
{
	uint8_t buff[10] = {0};
	drv_flash_read(buff, FLASH_START_ADDR, 10);

	if(0 != strcmp((char*)buff, (char*)FlashVersion))
	{
		//drv_flash_erase_chip();
		api_erase_system_para();

		drv_flash_write_buffer(FlashVersion, FLASH_START_ADDR, 10);
	}
	//drv_flash_read(buff, FLASH_START_ADDR, 10);
}



/********

初始化存储类型，首次上电需要写入扇区头部信息等

********/
void api_record_init(SECTOR_DATA_TYPE data_type)
{
	uint8_t buff[HEAD_INFO_LENGTH] = {0};
	uint32_t AddrTemp = SYSTEM_PARA_START_ADDR;

	uint8_t i = 0, j = 0, n = 0, BuffTemp = 0, SectorAlreadyWrite = 0;
	uint8_t ReadByteTemp = 0, ReadByteNum = 0, WriteByteTemp = 0xFF;
	uint16_t RecordNum = 0;
	uint32_t ReadStartAddr = 0;
	
	switch (data_type)
	{
		case SYSTEM_PARA_DATA:
		{
			SectorAlreadyWrite = 0;
			//寻找当前可用扇区，如果都为0xFF，则初始化第一个扇区，写入扇区头部信息
			for(i=0; i<(SYSTEM_PARA_SECTOR_NUM/2); i++) 						//考虑之后备份方案，将可用扇区除2
			{
				drv_flash_read(buff, SYSTEM_PARA_START_ADDR + (SECTOR_LENGTH * i), HEAD_INFO_LENGTH);		//读取扇区头部信息，判断是否被写入信息
			
				BuffTemp = 0xFF;
				
				for(j=0; j<8; j++)
				{
					BuffTemp &= buff[i];
			
					if(BuffTemp != 0xFF)
					{
						break;
					}
				}

				if((buff[0] == SYSTEM_PARA_DATA) && (buff[1] == MAIN_RECORD_SECTOR))		//发现当前扇区
				{
					SectorAlreadyWrite = 1;
					AddrTemp = SYSTEM_PARA_START_ADDR + (SECTOR_LENGTH * i);
					break;	
				}

			}

			if(0 == SectorAlreadyWrite)				//表示所谓扇区都为空，或没有可用扇区，即需要写入初始值
			{					
				//数据有误，需擦除相关全部扇区
				for(i=0; i<(SYSTEM_PARA_SECTOR_NUM/2); i++)
				{
					drv_flash_erase_sector(SYSTEM_PARA_START_ADDR + (SECTOR_LENGTH * i));
				}	
				
				buff[0] = SYSTEM_PARA_DATA;			//标记存储内容
				buff[1] = MAIN_RECORD_SECTOR;		//主存储区 或 备份区
				buff[2] = 0;						//预留，未定义具体作用
				buff[3] = 0;
				buff[4] = 0;
				buff[5] = 0;
				buff[6] = 0;
				buff[7] = 0;
			
				for(i=8; i<HEAD_INFO_LENGTH; i++)
				{
					buff[i] = 0xFF;					//移位表示存储数量，全置1
				}

				drv_flash_write_buffer(buff, AddrTemp, HEAD_INFO_LENGTH);		//写入扇区头部信息

				api_record_system_para(&DefaultSysPara.BackLight);					//写入默认值

				//上电初始值为系统默认值
				memcpy((char *)&g_SystemPara.BackLight, (char *)&DefaultSysPara.BackLight, SYS_PARA_RECORD_LENGTH);

			}
			else									//已经有扇区被写入过数据		
			{				
				//读取最新的一条记录到RAM
				//1；判断存储条数
				for(j=0; j<STORE_NUM_BYTE; j++)
				{
					if((buff[8+j] != 0xFF) || (buff[8+j] == 0x00))
					{
						ReadByteTemp = buff[8+j];	//存储最后一个非0xFF的字节
						ReadByteNum  = 1 + j;		//存储该字节的序号
						
						break;
					}
				}
				
				//2；计算存储条数
				if((ReadByteTemp == 0) && (ReadByteNum == 0))
				{
					RecordNum = 0;
				}
				else
				{
					if(ReadByteTemp == 0)
					{
						RecordNum = (STORE_NUM_BYTE - (ReadByteNum - 1)) * 8;
					}
					else
					{
						WriteByteTemp = 0xFF;
						
						for(n=0; n<8; n++)
						{
							WriteByteTemp &= ~(1<<n);
							if(ReadByteTemp == WriteByteTemp)
							{
								RecordNum = (STORE_NUM_BYTE - ReadByteNum) * 8 + n + 1;
				
								break;
							}
						}
					}
				}
				/*
				RecordNumxxx = RecordNum;
				byte_temp = ReadByteTemp;
				cnt_temp = ReadByteNum;
				record_temp = RecordNum;
				*/
				ReadStartAddr = AddrTemp + HEAD_INFO_LENGTH + SYS_PARA_RECORD_LENGTH * (RecordNum-1);
				
				drv_flash_read(&g_SystemPara.BackLight, ReadStartAddr, sizeof(g_SystemPara));		//上电初始化后将最新的数值读到该全局变量中
			}

			//如果扇区头部信息校验失败，需拷贝备份区数据
		}
		break;

		default:
		{}
		break;
	}

}





/****

寻找新的空白扇区
	
****/
uint32_t api_search_new_sector()
{
	uint32_t AddrTemp = 0;//SYSTEM_PARA_START_ADDR;
	uint8_t i = 0, j = 0, sector_num = 0xFF;
	uint8_t buff[10] = {0};
	uint8_t BuffTemp = 0xFF;


	//1；查找当前在使用的扇区 s
	for(i=0; i<(SYSTEM_PARA_SECTOR_NUM/2); i++)							//考虑之后备份方案，将可用扇区除2
	{
		drv_flash_read(buff, SYSTEM_PARA_START_ADDR + (SECTOR_LENGTH * i), 8);		//读取扇区头部信息，判断是否被写入信息
		
		if((buff[0] == SYSTEM_PARA_DATA) && (buff[1] == MAIN_RECORD_SECTOR))	
		{
			//AddrTemp = SYSTEM_PARA_START_ADDR + SECTOR_LENGTH*i;		//当前扇区的起始地址
			sector_num = i;
			break;
		}
	}

	if(0xFF == sector_num)		//没有写过数据的扇区
	{
		//数据有误，需擦除相关全部扇区
		for(i=0; i<(SYSTEM_PARA_SECTOR_NUM/2); i++)
		{
			drv_flash_erase_sector(SYSTEM_PARA_START_ADDR + (SECTOR_LENGTH * i));
		}	
		sector_num = 0;			//从起始地址开始搜索
	}

	//2；往后开始搜新扇区 (s,MAX)
	for(i=sector_num; i<(SYSTEM_PARA_SECTOR_NUM/2); i++)				//考虑之后备份方案，将可用扇区除2
	{
		drv_flash_read(buff, SYSTEM_PARA_START_ADDR + (SECTOR_LENGTH * i), 8);		//读取扇区头部信息，判断是否被写入信息

		BuffTemp = 0xFF;
		
		for(j=0; j<8; j++)
		{
			BuffTemp &= buff[i];

			if(BuffTemp != 0xFF)
			{
				break;
			}
		}

		if(0xFF == BuffTemp)			//扇区头部信息全为0xFF，存在可用
		{
			AddrTemp = SYSTEM_PARA_START_ADDR + (SECTOR_LENGTH * i);
			break;
		}

	}


	//3；没有可用，从头开始 (0,s)
	if(0 == AddrTemp)
	{
		for(i=0; i<sector_num; i++)
		{
			drv_flash_read(buff, SYSTEM_PARA_START_ADDR + (SECTOR_LENGTH * i), 8);		//读取扇区头部信息，判断是否被写入信息
		
			BuffTemp = 0xFF;
			
			for(j=0; j<8; j++)
			{
				BuffTemp &= buff[i];
		
				if(BuffTemp != 0xFF)
				{
					break;
				}
			}
		
			if(0xFF == BuffTemp)			//扇区头部信息全为0xFF，存在可用
			{
				AddrTemp = SYSTEM_PARA_START_ADDR + (SECTOR_LENGTH * i);
				break;
			}
		}
			
	}
	
	return AddrTemp;
}






/****

将系统参数以滚存的方式写入，如果该扇区空间不够则寻找可用扇区并写入
	
****/
void api_record_system_para(uint8_t *pData)
{
	uint8_t i = 0, j = 0, n = 0;

	uint8_t buff[100] = {0};

	uint8_t ReadByteTemp = 0, ReadByteNum = 0, WriteByteTemp = 0xFF, BitTemp = 0;
	
	uint16_t RecordNum = 0;										//一个扇区内已滚存的记录条数

	uint32_t SysParaAddrNow = 0;

	uint32_t WriteStartAddr = 0, WriteStartAddrNew = 0;			//新数据写入地址

	//1；搜索主存储扇区地址
	for(i=0; i<(SYSTEM_PARA_SECTOR_NUM/2); i++)
	{
		drv_flash_read(buff, SYSTEM_PARA_START_ADDR + SECTOR_LENGTH*i, HEAD_INFO_LENGTH);

		if((buff[0] == SYSTEM_PARA_DATA) && (buff[1] == MAIN_RECORD_SECTOR))	
		{
			SysParaAddrNow = SYSTEM_PARA_START_ADDR + SECTOR_LENGTH*i;		//当前扇区的起始地址
			break;
		}
	}

	//2；解析存储位置数据
	for(j=0; j<STORE_NUM_BYTE; j++)
	{
		if((buff[8+j] != 0xFF) || (buff[8+j] == 0x00))
		{
			ReadByteTemp = buff[8+j];				//存储最后一个非0xFF的字节
			ReadByteNum  = 1 + j;					//存储该字节的序号
			
			break;
		}
	}

	//3；计算存储条数
	if((ReadByteTemp == 0) && (ReadByteNum == 0))
	{
		RecordNum = 0;
	}
	else
	{
		if(ReadByteTemp == 0)
		{
			RecordNum = (STORE_NUM_BYTE - (ReadByteNum - 1))*8;
		}
		else
		{
			WriteByteTemp = 0xFF;
			
			for(n=0; n<8; n++)
			{
				WriteByteTemp &= ~(1<<n);
				if(ReadByteTemp == WriteByteTemp)
				{
					RecordNum = (STORE_NUM_BYTE - ReadByteNum)*8 + n + 1;
					BitTemp = n;
					break;
				}
			}
		}
	}
		
	//4；写入数据 system_para
	if((HEAD_INFO_LENGTH + SYS_PARA_RECORD_LENGTH*(RecordNum+1)) < SECTOR_LENGTH)		//剩余空间足够
	{	
		//修改写入的地址
		WriteStartAddr = SysParaAddrNow + HEAD_INFO_LENGTH + SYS_PARA_RECORD_LENGTH*RecordNum;
		
		drv_flash_write_buffer(pData, WriteStartAddr, SYS_PARA_RECORD_LENGTH);		//写入新数据

		//5；写入地址移位修改
		if(0 == RecordNum)
		{
			buff[0] = 0xFE;
			drv_flash_write_buffer(buff, SysParaAddrNow + HEAD_INFO_LENGTH - 1, 1);
		}
		else
		{
			if(ReadByteTemp == 0)
			{
				buff[0] = 0xFE;
				drv_flash_write_buffer(buff, SysParaAddrNow + HEAD_INFO_LENGTH - (STORE_NUM_BYTE - ReadByteNum) - 2, 1);
			}
			else
			{
				buff[0] = ReadByteTemp & (~(1<<(BitTemp+1)));
				drv_flash_write_buffer(buff, SysParaAddrNow + HEAD_INFO_LENGTH - (STORE_NUM_BYTE - ReadByteNum) - 1, 1);
			}
		}			
	}
	else
	{
		//寻找新的扇区 地址
		WriteStartAddrNew = api_search_new_sector();
		
		buff[0] = SYSTEM_PARA_DATA; 		//标记存储内容
		buff[1] = MAIN_RECORD_SECTOR;		//主存储区 或 备份区
		buff[2] = 0;
		buff[3] = 0;
		buff[4] = 0;
		buff[5] = 0;
		buff[6] = 0;
		buff[7] = 0;
		
		for(i=8; i<(HEAD_INFO_LENGTH-1); i++)
		{
			buff[i] = 0xFF; 				//移位表示存储数量，全置1
		}
		buff[HEAD_INFO_LENGTH - 1] = 0xFE;

		drv_flash_write_buffer(buff, WriteStartAddrNew, HEAD_INFO_LENGTH);		//写入扇区头部信息
		
		drv_flash_write_buffer(pData, WriteStartAddrNew + HEAD_INFO_LENGTH, SYS_PARA_RECORD_LENGTH);		//写入新数据

		//擦除上一个扇区
		drv_flash_erase_sector(SysParaAddrNow);
		
	}
}




//按 升级包存放区擦除
uint8_t api_erase_upgrade_file(void)
{
	uint8_t res = 0;
	
	for(uint16_t i=0; i<UPGRADE_FILE_SECTOR; i++)
	{
		drv_flash_erase_sector(UPGRADE_HEADINFO_ADDR + i*SECTOR_LENGTH);
	}
	
	res = 1;
	return res;
}


//擦除所有历史记录扇区
uint8_t api_erase_record(void)
{
	uint8_t res = 0;
	return res;
}


//删除存储 的屏幕参数
void api_erase_system_para(void)
{
	for(uint16_t i=0; i<SYSTEM_PARA_SECTOR_NUM+1; i++)
	{
		drv_flash_erase_sector(FLASH_START_ADDR + i*SECTOR_LENGTH);
	}
}


//删除串号
void api_erase_serial_number()
{
	drv_flash_erase_sector(SERIAL_NUMBER_ADDR);
}

void api_set_serial_number(uint8_t *pData, uint8_t len)
{
	api_erase_serial_number();
	drv_flash_write_buffer(pData, SERIAL_NUMBER_ADDR, len);
}

void api_restore_default_para(void)
{
	//处理不恢复的数值
	uint8_t g_sn_len = 16;
	uint16_t _para_tmp[5] = {0};
	_para_tmp[0] = g_SystemPara.system_master_slave;
	_para_tmp[1] = g_SystemPara.ble_switch;
	
	memcpy(&g_SystemPara, &DefaultSysPara, sizeof(g_SystemPara));
	g_SystemPara.system_master_slave 	= _para_tmp[0];	
	g_SystemPara.ble_switch 			= _para_tmp[1];
	
	
	
//	for(uint8_t i=0; i<sizeof(g_bms_ver_info_data.u8_product_sn); i++)
//	{
//		if(0 == g_bms_ver_info_data.u8_product_sn[i])
//		{
//			g_sn_len = i;
//			break;
//		}
//	}
		
	if((0xFF == g_bms_ver_info_data.u8_product_sn[0]) || (0 == g_bms_ver_info_data.u8_product_sn[0]))
	{
		sprintf(g_SystemPara.BleName, "%s", BLE_DEFAULT_NAME);
	}
	else
	{
		sprintf(g_SystemPara.BleName, "%s%c%c%c%c", "NEO-",g_bms_ver_info_data.u8_product_sn[g_sn_len-5],g_bms_ver_info_data.u8_product_sn[g_sn_len-4],g_bms_ver_info_data.u8_product_sn[g_sn_len-3],g_bms_ver_info_data.u8_product_sn[g_sn_len-2]);
	}
	api_esp_ble_name_set(g_SystemPara.BleName);
						
	FLASH_CTRL_TYPEDEF FlashMsgValue = RECORD_SYS_PARA;
	if(SPIFlash_Queue != NULL)
	{
		xQueueSend(SPIFlash_Queue, &FlashMsgValue, 0);
	}
}


//xxx：坏块管理：搜寻下一个可用扇区或者块，防止某些坏块影响整理的存储功能


/*!
	@brief  读取当前日志系统信息 已存储条数
	@param  NULL
	@return 当前日志系统的头部信息
 */
USERLOG_HEADINFO_TYPE api_userlog_headinfo_read(void)
{
	USERLOG_HEADINFO_TYPE res = {0};
	uint8_t buff[100] = {0};
	uint16_t SectorIdTemp = 0;
	
	uint8_t ReadByteTemp = 0, ReadByteNum = 0, WriteByteTemp = 0xFF;

	for(uint8_t i = 0; i < USERLOG_SECTOR_NUM; i++)
	{
		drv_flash_read(buff, USERLOG_RECORD_ADDR + SECTOR_LENGTH*i, USERLOG_HEAD_LENGTH);
		
		/* 1、解析扇区的存储编号 */
		uint16_t sec_temp = (uint16_t)((buff[0] << 8) + buff[1]);

		for(uint8_t j = 0; j <= 16; j++)
		{
			if((0xffff >> j) == sec_temp)
			{
				SectorIdTemp = j;
				break;
			}
		}
		
		if((SectorIdTemp != 0) && (SectorIdTemp > res.sector_id))
		{
			res.sector_id = SectorIdTemp;
			
			/* 2、解析存储位置数据 */
			for(uint8_t n = 0; n < (USERLOG_HEAD_LENGTH - 2); n++)
			{
				if(buff[n + 2] != 0x00)
				{
					ReadByteTemp = buff[n + 2];				/* 存储最后一个非0x00的字节 */
					ReadByteNum  = n;						/* 存储该字节的序号 */
					break;
				}
			}
			
			/* 3、计算存储条数 */
			if((ReadByteTemp == 0xFF) && (ReadByteNum == 0))
			{
				res.secinfo_num = 0;
			}
			else
			{
				if(ReadByteTemp == 0xFF)
				{
					res.secinfo_num = ReadByteNum*8;
				}
				else
				{
					WriteByteTemp = 0xFF;
					
					for(uint8_t m = 0; m <= 8; m++)
					{
						if(ReadByteTemp == (WriteByteTemp >> m))
						{
							res.secinfo_num = ReadByteNum * 8 + m;
						}
					}
				}
			}
		}
		
		/* 4、计算日志系统中最新一条的id */
		if((res.secinfo_num < USERLOG_NUM_EACH_SECTOR) || (0 == SectorIdTemp) || (SectorIdTemp < res.sector_id))
		{
			if(0 == res.sector_id)  res.lastinfo_id = 0;
			else  res.lastinfo_id = (res.sector_id - 1) * USERLOG_NUM_EACH_SECTOR + res.secinfo_num;
			
			break;
		}
	}
	
	if((0 == (res.sector_id % USERLOG_SECTOR_NUM)) && (USERLOG_NUM_EACH_SECTOR == res.secinfo_num))
		res.lastinfo_id = res.sector_id * USERLOG_NUM_EACH_SECTOR;
	
	return res;
}


/*!
	@brief  写入日志信息
	@param  userlog 要写入的日志
	@return NULL
 */
void api_userlog_write(USERLOG_INFO_TYPE userlog)
{
	USERLOG_HEADINFO_TYPE UserLogHeadInfo = {0};
	uint8_t WriteSector = 0;
	uint32_t WriteAddr = 0;
	
	UserLogHeadInfo = api_userlog_headinfo_read();
	
	userlog.LogId = UserLogHeadInfo.lastinfo_id + 1;
	
	if(0 == UserLogHeadInfo.sector_id) WriteSector = 0;
	else if(0 == (UserLogHeadInfo.sector_id % USERLOG_SECTOR_NUM)) WriteSector = USERLOG_SECTOR_NUM;
	else WriteSector = UserLogHeadInfo.sector_id % USERLOG_SECTOR_NUM;
	
	if((0 == UserLogHeadInfo.secinfo_num) || (USERLOG_NUM_EACH_SECTOR == UserLogHeadInfo.secinfo_num))		/* 需要重新开辟扇区 */
	{
		if(USERLOG_SECTOR_NUM == WriteSector) WriteSector = 0;
		
		/* 1、擦除下一个扇区 */
		drv_flash_erase_sector(USERLOG_RECORD_ADDR + SECTOR_LENGTH*WriteSector);
		
		/* 2、写入扇区id号 */
		uint8_t buff[2];
		buff[0] = (uint8_t)((0xffff >> (UserLogHeadInfo.sector_id + 1)) >> 8);
		buff[1] = (uint8_t)(0xffff >> (UserLogHeadInfo.sector_id + 1));
		
		drv_flash_write_buffer(&buff[0], USERLOG_RECORD_ADDR + WriteSector*SECTOR_LENGTH, 2);
		
		/* 3、写入存储条数 */
		uint8_t buff_1 = 0xff >> 1;
		drv_flash_write_buffer(&buff_1, USERLOG_RECORD_ADDR + WriteSector*SECTOR_LENGTH + 2, 1);
		
		/* 4、写入日志 */
		WriteAddr = USERLOG_RECORD_ADDR + WriteSector*SECTOR_LENGTH + USERLOG_HEAD_LENGTH;
		drv_flash_write_buffer((uint8_t*)&userlog, WriteAddr, sizeof(USERLOG_INFO_TYPE));
	}
	else
	{
		/* 写入存储条数 */
		uint8_t temp1 = UserLogHeadInfo.secinfo_num / 8;
		uint8_t temp2 = 0xFF >> ((UserLogHeadInfo.secinfo_num % 8) + 1);
		
		drv_flash_write_buffer(&temp2, USERLOG_RECORD_ADDR + (WriteSector - 1)*SECTOR_LENGTH + 2 + temp1, 1);
		
		/* 写入日志 */
		WriteAddr = USERLOG_RECORD_ADDR + SECTOR_LENGTH*(WriteSector - 1) + USERLOG_HEAD_LENGTH + 8*UserLogHeadInfo.secinfo_num;
		drv_flash_write_buffer((uint8_t*)&userlog, WriteAddr, sizeof(USERLOG_INFO_TYPE));
	}
}


/*!
	@brief  读取日志信息 读取上限20条
	@param  startid 读取的起始id
	@param  readnum 读取条数
	@param  userlog 接收缓存
	@return 0: 读取失败 非0: 读取的log条数
 */
uint8_t api_userlog_read(uint16_t startid, uint8_t readnum, USERLOG_INFO_TYPE* userlog)
{
	uint8_t res = 0;
	uint8_t buff[160];
	USERLOG_HEADINFO_TYPE UserLogHeadInfo = {0};
	uint8_t readnumtemp = 0;
	
	uint8_t ReadSector = 0;
	uint32_t ReadAddr = 0;
	
	UserLogHeadInfo = api_userlog_headinfo_read();
		
	if((readnum <= 20) && (startid <= UserLogHeadInfo.lastinfo_id) && (startid > 0))
	{
		/* 1、读取数量判断 */
		readnumtemp = ((UserLogHeadInfo.lastinfo_id - startid + 1) > readnum)? readnum:(UserLogHeadInfo.lastinfo_id - startid + 1);
		
		/* 2、读取的扇区编号判断 */
		uint8_t temp1 = (0 != (startid % USERLOG_NUM_EACH_SECTOR))? (startid/USERLOG_NUM_EACH_SECTOR + 1):startid/USERLOG_NUM_EACH_SECTOR;
		ReadSector = (0 == (temp1 % USERLOG_SECTOR_NUM))? USERLOG_SECTOR_NUM:(temp1 % USERLOG_SECTOR_NUM);
		
		/* 3、判定是否需要跨扇区 */
		uint16_t idtemp = (0 == startid % USERLOG_NUM_EACH_SECTOR)? USERLOG_NUM_EACH_SECTOR:(startid % USERLOG_NUM_EACH_SECTOR);
		
		if((idtemp + readnum) <= USERLOG_NUM_EACH_SECTOR+1)
		{
			ReadAddr = USERLOG_RECORD_ADDR + SECTOR_LENGTH*(ReadSector - 1) + 8 * (idtemp - 1) + USERLOG_HEAD_LENGTH;
			drv_flash_read(&buff[0], ReadAddr, 8 * readnumtemp);
		}
		else
		{
			uint8_t read_sector_num_1 = (USERLOG_NUM_EACH_SECTOR - idtemp + 1);
			uint8_t read_sector_num_2 = readnumtemp - read_sector_num_1;
			
			ReadAddr = USERLOG_RECORD_ADDR + SECTOR_LENGTH*(ReadSector - 1) + 8 * (idtemp - 1) + USERLOG_HEAD_LENGTH;
			drv_flash_read(&buff[0], ReadAddr, 8 * read_sector_num_1);
			
			uint8_t ReadSector_Next = 0;
			ReadSector_Next = ((ReadSector + 1) > USERLOG_SECTOR_NUM)? 1:(ReadSector + 1);
			
			ReadAddr = USERLOG_RECORD_ADDR + SECTOR_LENGTH*(ReadSector_Next - 1) + USERLOG_HEAD_LENGTH;
			drv_flash_read(&buff[8 * read_sector_num_1], ReadAddr, 8 * read_sector_num_2);
		}
		
		memcpy((uint8_t*)userlog, &buff[0], 8 * readnumtemp);
		
		res = readnumtemp;
	}
	else
	{
		res = 0;
	}

	return res;
}

/*!
	@brief  擦除日志系统
	@param  NULL
	@return NULL
 */
void api_userlog_erase(void)
{
	for(uint8_t i = 0; i < USERLOG_SECTOR_NUM; i++)
	{
		drv_flash_erase_sector(USERLOG_RECORD_ADDR + i*SECTOR_LENGTH);
	}
}

//0000
USERLOG_HEADINFO_TYPE api_event_log_headinfo_read(uint8_t logtype)
{
	USERLOG_HEADINFO_TYPE res = {0};
	uint8_t buff[100] = {0};
	uint16_t SectorIdTemp = 0;
	
	uint8_t ReadByteTemp = 0, ReadByteNum = 0, WriteByteTemp = 0xFF;
	
	uint32_t StartAddr = 0;
	uint32_t LogNumSector = 0;
	
	switch(logtype)
	{
		case LOG_TYPE_ALARM:
		{			
			StartAddr = USERLOG_RECORD_ADDR;
			LogNumSector = USERLOG_NUM_EACH_SECTOR;
		}
		break;
		
		case LOG_TYPE_OPERATE:
		{
			StartAddr = EVENT_LOG_RECORD_ADDR_1;
			LogNumSector = EVENT_LOG_NUM_EACH_SECTOR;
		}
		break;
		
		case LOG_TYPE_RUNNING:
		{
			StartAddr = EVENT_LOG_RECORD_ADDR_2;
			LogNumSector = EVENT_LOG_NUM_EACH_SECTOR;
		}
		break;
		
		case LOG_TYPE_UPG:
		{
			return res;
		}
		break;
		
		default:
		{
		
		}
		break;
	}
	
	
	for(uint8_t i = 0; i < USERLOG_SECTOR_NUM; i++)
	{
		drv_flash_read(buff, StartAddr + SECTOR_LENGTH*i, USERLOG_HEAD_LENGTH);
		
		/* 1、解析扇区的存储编号 */
		uint16_t sec_temp = (uint16_t)((buff[0] << 8) + buff[1]);

		for(uint8_t j = 0; j <= 16; j++)
		{
			if((0xffff >> j) == sec_temp)
			{
				SectorIdTemp = j;
				break;
			}
		}
		
		if((SectorIdTemp != 0) && (SectorIdTemp > res.sector_id))
		{
			res.sector_id = SectorIdTemp;
			
			/* 2、解析存储位置数据 */
			for(uint8_t n = 0; n < (USERLOG_HEAD_LENGTH - 2); n++)
			{
				if(buff[n + 2] != 0x00)
				{
					ReadByteTemp = buff[n + 2];				/* 存储最后一个非0x00的字节 */
					ReadByteNum  = n;						/* 存储该字节的序号 */
					break;
				}
			}
			
			/* 3、计算存储条数 */
			if((ReadByteTemp == 0xFF) && (ReadByteNum == 0))
			{
				res.secinfo_num = 0;
			}
			else
			{
				if(ReadByteTemp == 0xFF)
				{
					res.secinfo_num = ReadByteNum*8;
				}
				else
				{
					WriteByteTemp = 0xFF;
					
					for(uint8_t m = 0; m <= 8; m++)
					{
						if(ReadByteTemp == (WriteByteTemp >> m))
						{
							res.secinfo_num = ReadByteNum * 8 + m;
						}
					}
				}
			}
		}
		
		/* 4、计算日志系统中最新一条的id */
		if((res.secinfo_num < LogNumSector) || (0 == SectorIdTemp) || (SectorIdTemp < res.sector_id))
		{
			if(0 == res.sector_id)  res.lastinfo_id = 0;
			else  res.lastinfo_id = (res.sector_id - 1) * LogNumSector + res.secinfo_num;
			
			break;
		}
	}
	
	if((0 == (res.sector_id % USERLOG_SECTOR_NUM)) && (LogNumSector == res.secinfo_num))
		res.lastinfo_id = res.sector_id * LogNumSector;
	
	return res;
}


//111
void api_event_log_write(EVENT_LOG_INFO_TYPE userlog, uint8_t logtype)
{
	USERLOG_HEADINFO_TYPE UserLogHeadInfo = {0};
	uint8_t WriteSector = 0;
	uint32_t WriteAddr = 0;

	
	UserLogHeadInfo = api_event_log_headinfo_read(logtype);
	
		
	uint32_t StartAddr = 0;
	uint32_t LogNumSector = 0;
	uint8_t WriteLogLength = 0;
	switch(logtype)
	{
		case LOG_TYPE_ALARM:
		{
			StartAddr = USERLOG_RECORD_ADDR;
			WriteLogLength = USERLOG_INFO_LENGTH;
			LogNumSector = USERLOG_NUM_EACH_SECTOR;
		}
		break;
		
		case LOG_TYPE_OPERATE:
		{
			StartAddr = EVENT_LOG_RECORD_ADDR_1;
			WriteLogLength = EVENT_LOG_INFO_LENGTH;
			LogNumSector = EVENT_LOG_NUM_EACH_SECTOR;
		}
		break;
		
		case LOG_TYPE_RUNNING:
		{
			StartAddr = EVENT_LOG_RECORD_ADDR_2;
			WriteLogLength = EVENT_LOG_INFO_LENGTH;
			LogNumSector = EVENT_LOG_NUM_EACH_SECTOR;
		}
		break;
		
		case LOG_TYPE_UPG:
		{
			
		}
		break;
		
		default:
		{
			return;
		}
		break;
	}
	
	
	userlog.LogId = UserLogHeadInfo.lastinfo_id + 1;
	
	if(0 == UserLogHeadInfo.sector_id) WriteSector = 0;
	else if(0 == (UserLogHeadInfo.sector_id % USERLOG_SECTOR_NUM)) WriteSector = USERLOG_SECTOR_NUM;
	else WriteSector = UserLogHeadInfo.sector_id % USERLOG_SECTOR_NUM;
	
	if((0 == UserLogHeadInfo.secinfo_num) || (LogNumSector == UserLogHeadInfo.secinfo_num))		/* 需要重新开辟扇区 */
	{
		if(USERLOG_SECTOR_NUM == WriteSector) WriteSector = 0;
		
		/* 1、擦除下一个扇区 */
		drv_flash_erase_sector(StartAddr + SECTOR_LENGTH*WriteSector);
		
		/* 2、写入扇区id号 */
		uint8_t buff[2];
		buff[0] = (uint8_t)((0xffff >> (UserLogHeadInfo.sector_id + 1)) >> 8);
		buff[1] = (uint8_t)(0xffff >> (UserLogHeadInfo.sector_id + 1));
		
		drv_flash_write_buffer(&buff[0], StartAddr + WriteSector*SECTOR_LENGTH, 2);
		
		/* 3、写入存储条数 */
		uint8_t buff_1 = 0xff >> 1;
		drv_flash_write_buffer(&buff_1, StartAddr + WriteSector*SECTOR_LENGTH + 2, 1);
		
		/* 4、写入日志 */
		WriteAddr = StartAddr + WriteSector*SECTOR_LENGTH + USERLOG_HEAD_LENGTH;
		drv_flash_write_buffer((uint8_t*)&userlog, WriteAddr, WriteLogLength);
	}
	else
	{
		/* 写入存储条数 */
		uint8_t temp1 = UserLogHeadInfo.secinfo_num / WriteLogLength;
		uint8_t temp2 = 0xFF >> ((UserLogHeadInfo.secinfo_num % WriteLogLength) + 1);
		
		drv_flash_write_buffer(&temp2, StartAddr + (WriteSector - 1)*SECTOR_LENGTH + 2 + temp1, 1);
		
		/* 写入日志 */
		WriteAddr = StartAddr + SECTOR_LENGTH*(WriteSector - 1) + USERLOG_HEAD_LENGTH + WriteLogLength*UserLogHeadInfo.secinfo_num;
		drv_flash_write_buffer((uint8_t*)&userlog, WriteAddr, WriteLogLength);
	}
}

uint8_t api_event_log_read(uint16_t startid, uint8_t readnum, EVENT_LOG_INFO_TYPE* userlog, uint8_t logtype)
{
	uint8_t res = 0;
	uint8_t buff[160];
	USERLOG_HEADINFO_TYPE UserLogHeadInfo = {0};
	uint8_t readnumtemp = 0;
	
	uint8_t ReadSector = 0;
	uint32_t ReadAddr = 0;
	
	UserLogHeadInfo = api_event_log_headinfo_read(logtype);
	
	uint32_t StartAddr = 0;
	uint32_t LogNumSector = 0;
	uint8_t WriteLogLength = 0;
	switch(logtype)
	{
		case LOG_TYPE_ALARM:
		{
			StartAddr = USERLOG_RECORD_ADDR;
			WriteLogLength = USERLOG_INFO_LENGTH;
			LogNumSector = USERLOG_NUM_EACH_SECTOR;
		}
		break;
		
		case LOG_TYPE_OPERATE:
		{
			StartAddr = EVENT_LOG_RECORD_ADDR_1;
			WriteLogLength = EVENT_LOG_INFO_LENGTH;
			LogNumSector = EVENT_LOG_NUM_EACH_SECTOR;
		}
		break;
		
		case LOG_TYPE_RUNNING:
		{
			StartAddr = EVENT_LOG_RECORD_ADDR_2;
			WriteLogLength = EVENT_LOG_INFO_LENGTH;
			LogNumSector = EVENT_LOG_NUM_EACH_SECTOR;
		}
		break;
		
		case LOG_TYPE_UPG:
		{
			
		}
		break;
		
		default:
		{
			return res;
		}
		break;
	}
	
	if((readnum <= 20) && (startid <= UserLogHeadInfo.lastinfo_id) && (startid > 0))
	{
		/* 1、读取数量判断 */
		readnumtemp = ((UserLogHeadInfo.lastinfo_id - startid + 1) > readnum)? readnum:(UserLogHeadInfo.lastinfo_id - startid + 1);
		
		/* 2、读取的扇区编号判断 */
		uint8_t temp1 = (0 != (startid % LogNumSector))? (startid/LogNumSector + 1):startid/LogNumSector;
		ReadSector = (0 == (temp1 % USERLOG_SECTOR_NUM))? USERLOG_SECTOR_NUM:(temp1 % USERLOG_SECTOR_NUM);
		
		/* 3、判定是否需要跨扇区 */
		uint16_t idtemp = (0 == startid % LogNumSector)? LogNumSector:(startid % LogNumSector);
		
		if((idtemp + readnum) <= LogNumSector+1)
		{
			ReadAddr = StartAddr + SECTOR_LENGTH*(ReadSector - 1) + WriteLogLength * (idtemp - 1) + USERLOG_HEAD_LENGTH;
			drv_flash_read(&buff[0], ReadAddr, WriteLogLength * readnumtemp);
		}
		else
		{
			uint8_t read_sector_num_1 = (LogNumSector - idtemp + 1);
			uint8_t read_sector_num_2 = readnumtemp - read_sector_num_1;
			
			ReadAddr = StartAddr + SECTOR_LENGTH*(ReadSector - 1) + WriteLogLength * (idtemp - 1) + USERLOG_HEAD_LENGTH;
			drv_flash_read(&buff[0], ReadAddr, WriteLogLength * read_sector_num_1);
			
			uint8_t ReadSector_Next = 0;
			ReadSector_Next = ((ReadSector + 1) > USERLOG_SECTOR_NUM)? 1:(ReadSector + 1);
			
			ReadAddr = StartAddr + SECTOR_LENGTH*(ReadSector_Next - 1) + USERLOG_HEAD_LENGTH;
			drv_flash_read(&buff[WriteLogLength * read_sector_num_1], ReadAddr, WriteLogLength * read_sector_num_2);
		}
		
		for(uint8_t n=0; n<readnumtemp; n++)
		{
			memcpy(((uint8_t*)userlog) + sizeof(EVENT_LOG_INFO_TYPE)*n, &buff[WriteLogLength*n], WriteLogLength);
		}
//		memcpy((uint8_t*)userlog, &buff[0], 8 * readnumtemp);
		
		res = readnumtemp;
	}
	else
	{
		res = 0;
	}

	return res;
}



