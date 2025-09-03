
#ifndef __API_SPIFLASH_H__
#define __API_SPIFLASH_H__


#include "drv_spiflash.h"
#include "gui_guider.h"






/**W25Q64 64M-bit / 16M-byte
**第一个扇区用于存储版本信息
**规划前6扇区 用于系统参数存储
**规划双备份功能，因此实际存储为3个扇区
**
**
**/

#include "tsk_prj_process.h"


//基本单位长度定义
#define PAGE_LEN						0x00000100			//页(256 Byte)
#define SECTOR_LENGTH					0x00001000			//扇区(4 K) 最小擦除单位
#define	BLOCK_LEN						0x00010000			//块(64 K)

#define FLASH_START_ADDR				0x00000000
#define	SYSTEM_PARA_SECTOR_NUM			6
#define	STORE_NUM_BYTE					40

#define	SYSTEM_PARA_START_ADDR			SECTOR_LENGTH


#define HEAD_INFO_LENGTH				48					//头部信息数据长度


//地址定义
#define SERIAL_NUMBER_ADDR 				0x00007000
#define UPGRADE_HEADINFO_ADDR 			0x00008000
#define UPGRADE_FILE_ADDR 				0x00009000//(UPGRADE_HEADINFO_ADDR + sizeof(UPGRADE_HEADIF_DATATYPE))	//存放在线升级文件的SPIFlash起始地址
#define UPGRADE_FILE_SECTOR				512				//存放在线升级文件的 SECTOR 数量


/* 日志系统 */
#define USERLOG_HEAD_LENGTH				66						/* 用户日志头部信息 第一、二个字节代表存储编号 0x0：1-500 0x00 501-1000 后64个字节代表扇区内信息条数 */
#define USERLOG_INFO_LENGTH				8						/* 用户日志每条信息长度 */
#define USERLOG_SECTOR_NUM				3						/* 用户日志存储扇区数 有一个作为缓冲区 */
#define USERLOG_NUM_EACH_SECTOR			500						/* 用户日志每个扇区存储记录条数上限 */
#define USERLOG_RECORD_ADDR				0x00400000				/* 用户日志起始地址 */



//操作记录 / 运行状态
#define EVENT_LOG_HEAD_LENGTH				66								/* 用户日志头部信息 第一、二个字节代表存储编号 0x0：1-500 0x00 501-1000 后64个字节代表扇区内信息条数 */
#define EVENT_LOG_INFO_LENGTH				12								/* 用户日志每条信息长度 */
#define EVENT_LOG_NUM_EACH_SECTOR			330								/* 用户日志每个扇区存储记录条数上限 */
#define EVENT_LOG_SECTOR_NUM_1				4								/* 操作记录 存储扇区数 有一个作为缓冲区 */
#define EVENT_LOG_SECTOR_NUM_2				4								/* 运行状态 存储扇区数 有一个作为缓冲区 */
#define EVENT_LOG_RECORD_ADDR_1				USERLOG_RECORD_ADDR+SECTOR_LENGTH*USERLOG_SECTOR_NUM	/* 用户日志起始地址 */
#define EVENT_LOG_RECORD_ADDR_2				EVENT_LOG_RECORD_ADDR_1+SECTOR_LENGTH*EVENT_LOG_SECTOR_NUM_1

typedef struct
{
	uint8_t sector_id;				/* 最新一条的存储扇区编号 编号1 0-499 2 500-999 */
	uint16_t secinfo_num;			/* 最新一条在扇区中的存储位置 */
	uint16_t lastinfo_id;			/* 最新一条的id值 起始值1 */
	
}USERLOG_HEADINFO_TYPE;

typedef struct
{
	uint16_t LogId;					/* 日志编号 */
	uint16_t LogCode;				/* 日志编码 */
	uint32_t TimeStamp;				/* 日志时间戳 */
	
}USERLOG_INFO_TYPE;

typedef struct
{
	uint16_t LogId;					/* 日志编号 */
	uint16_t LogCode;				/* 日志编码 */
	uint32_t TimeStamp;				/* 日志时间戳 */
	uint16_t LogValueOld;				//
	uint16_t LogValueNew;
	
}EVENT_LOG_INFO_TYPE;

typedef enum
{
	LOG_TYPE_ALARM,
	LOG_TYPE_OPERATE,
	LOG_TYPE_RUNNING,
	LOG_TYPE_UPG,
	
	LOG_TYPE_NUM_MAX,
	
} EVENT_LOG_DATA_TYPE;


typedef enum
{
	SYSTEM_PARA_DATA			=1,			//系统参数
	USERLOG_RECORD_DATA			,			//用户日志(未实现)
						
} SECTOR_DATA_TYPE;


typedef enum
{
	MAIN_RECORD_SECTOR			=0,		//主存储区
	BACKUP_RECORD_SECTOR		,		//备份区
					
} FLASH_BACKUP_TYPEDEF;


//升级存储区头部信息 数据结构体 定义
typedef struct
{
	uint8_t 	HMIUpgradeFlag;		//0-无需烧录；0x01-需刷写烧录Nor; 0x0F-高级烧录Adv
	uint8_t     FlashEraseFinish;	//0xFF-未擦除；1-擦除完成
	uint8_t 	UpgradeDevice;		//0xFF-NULL；1-HMI；2-INV
	uint8_t 	UpgradeResult;		//0xFF-NULL；1-成功；2-失败
	uint32_t 	UpgradeSize;		//文件包大小 字节数
	
}UPGRADE_HEADIF_DATATYPE;


extern uint8_t api_erase_upgrade_file(void);
extern uint8_t api_erase_record(void);
extern void api_restore_default_para(void);
extern void api_set_serial_number(uint8_t *pData, uint8_t len);

extern void api_spi_flash_init(void);
extern void api_record_init(SECTOR_DATA_TYPE type);
extern void api_flash_version_init(void);
extern void api_record_system_para(uint8_t *pData);
extern uint32_t api_search_new_sector(void);

extern SYSTEM_PARA_DATATYPE g_SystemPara;


//extern USERLOG_HEADINFO_TYPE api_userlog_headinfo_read(void);
//extern void api_userlog_write(USERLOG_INFO_TYPE userlog);
//extern uint8_t api_userlog_read(uint16_t startid, uint8_t readnum, USERLOG_INFO_TYPE* userlog);

extern USERLOG_HEADINFO_TYPE api_event_log_headinfo_read(uint8_t logtype);
extern void api_event_log_write(EVENT_LOG_INFO_TYPE userlog, uint8_t logtype);
extern uint8_t api_event_log_read(uint16_t startid, uint8_t readnum, EVENT_LOG_INFO_TYPE* userlog, uint8_t logtype);


extern void api_userlog_erase(void);

#endif
