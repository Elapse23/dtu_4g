#include "Version.h"
#include "string.h"




#if defined ( __CC_ARM   ) || defined   (  __GNUC__  )
const uint8_t bBootFirmwareStr[2]		__attribute__((at(cdwBOOT_FIRMWARE_ADDR))) = 
#elif defined ( __ICCARM__ )
const uint8_t bBootFirmwareStr[2]		@cdwBOOT_FIRMWARE_ADDR =
#endif
{
	0x01,//BootLoader版本号
	0x02
};


#if defined ( __CC_ARM   ) || defined   (  __GNUC__  )
const uint8_t bFirmwareAppStartAddr[4]		__attribute__((at(cdwFWAPP_START_ADDR))) = 
#elif defined ( __ICCARM__ )
const uint8_t bFirmwareAppStartAddr[4]		@cdwFWAPP_START_ADDR =
#endif
{
	0x08,//APP起始地址 
	0x00,
	0x40,
	0x00
};


#if defined ( __CC_ARM   ) || defined   (  __GNUC__  )
const uint8_t bFirmwareBootEndAddr[4]		__attribute__((at(cdwFWBOOT_END_ADDR))) = 
#elif defined ( __ICCARM__ )
const uint8_t bFirmwareBootEndAddr[4]		@cdwFWBOOT_END_ADDR =
#endif
{
	0x08,//BootLoader结束地址
	0x00,
	0x3F,
	0xFF
};


#if defined ( __CC_ARM   ) || defined   (  __GNUC__  )
const uint8_t bFirmwareBootStartAddr[4]		__attribute__((at(cdwFWBOOT_START_ADDR))) = 
#elif defined ( __ICCARM__ )
const uint8_t bFirmwareBootStartAddr[4]		@cdwFWBOOT_START_ADDR =
#endif
{
	0x08,//BootLoader起始地址
	0x00,
	0x00,
	0x00
};

#if defined ( __CC_ARM   ) || defined   (  __GNUC__  )
const uint8_t bFirmwareTypeStr[4]		__attribute__((at(cdwFIRMWARE_TYPE_ADDR))) = 
#elif defined ( __ICCARM__ )
const uint8_t bFirmwareTypeStr[4]		@cdwFIRMWARE_TYPE_ADDR =
#endif

{
	0x0B,//单片机编号GD32F450 (11;Zeta)
	0x06,//[4201 00]>>16, 产品系列及细分编号[420100]  
	0x69,//[4201 00]>>8, 
	0x04 //[4201 00]
};


#if defined ( __CC_ARM   ) || defined   (  __GNUC__  )
const uint8_t bAPPFirmwareAddr[3]		__attribute__((at(cdwAPP_SECTOR_START))) = 
#elif defined ( __ICCARM__ )
const uint8_t bAPPFirmwareAddr[3]		@cdwAPP_SECTOR_START =
#endif
{
	(uint8_t)MAIN_VER,			//App固件版本号
	(uint8_t)(SUB_VER>>8),
	(uint8_t)SUB_VER
};



//根据编译日期，由编译器产生版本号信息
void GetSoftwareVersion_Date(char *pDdata)
{
	const char DATE_NOW[12] = __DATE__;
	char VersionDate[6] = {0};

	//获取编译日期，以数字的形式体现在软件版本号中
	//YYMMDD
	VersionDate[0] = DATE_NOW[9];		//YEAR
	VersionDate[1] = DATE_NOW[10];		//YEAR

	if(0x20 == DATE_NOW[4])
	{
		VersionDate[4] = 0x30;
	}
	else
	{
		VersionDate[4] = DATE_NOW[4];		//DAY
	}
	
	VersionDate[5] = DATE_NOW[5];		//DAY

	VersionDate[2] = '0';				//MONTH

	if(strstr(DATE_NOW,"Jan"))			//MONTH
	{
		VersionDate[3] = '1';
	}
	else if(strstr(DATE_NOW,"Feb"))
	{
		VersionDate[3] = '2';
	}	
	else if(strstr(DATE_NOW,"Mar"))
	{
		VersionDate[3] = '3';
	}	
	else if(strstr(DATE_NOW,"Apr"))
	{
		VersionDate[3] = '4';
	}	
	else if(strstr(DATE_NOW,"May"))
	{
		VersionDate[3] = '5';
	}	
	else if(strstr(DATE_NOW,"Jun"))
	{
		VersionDate[3] = '6';
	}	
	else if(strstr(DATE_NOW,"Jul"))
	{
		VersionDate[3] = '7';
	}	
	else if(strstr(DATE_NOW,"Aug"))
	{
		VersionDate[3] = '8';
	}	
	else if(strstr(DATE_NOW,"Sep"))
	{
		VersionDate[3] = '9';
	}	
	else if(strstr(DATE_NOW,"Oct"))
	{
		VersionDate[2] = '1';
		VersionDate[3] = '0';
	}
	else if(strstr(DATE_NOW,"Nov"))
	{
		VersionDate[2] = '1';
		VersionDate[3] = '1';
	}
	else
	{
		VersionDate[2] = '1';
		VersionDate[3] = '2';
	}


	strcpy(pDdata, VersionDate);	
}



void GetSoftwareVersion(void)
{
	char c_MachineInfo[10] = {0};
	uint32_t MachineInfo = (bFirmwareTypeStr[1]<<16) + (bFirmwareTypeStr[2]<<8) + bFirmwareTypeStr[3];
	sprintf(c_MachineInfo, "%d", MachineInfo);
	
	g_FirmwareID = MachineInfo; 
	g_AppID = (bAPPFirmwareAddr[0]<<16) + (bAPPFirmwareAddr[1]<<8) + bAPPFirmwareAddr[2];
	g_BootloaderVer = (bBootFirmwareStr[0]<<8) + bBootFirmwareStr[1];
	
	//手动更改版本号内容 ↓
	sprintf(c_C4Version, "%s%.4s%s%.2s%s%.2d%s%.4d%s%.2d%s%.2d", "Zeta",c_MachineInfo, " ", &c_MachineInfo[4], ".", bAPPFirmwareAddr[0],
			".", ((bAPPFirmwareAddr[1]<<8) + bAPPFirmwareAddr[2]), " b",bBootFirmwareStr[0], ".",bBootFirmwareStr[1]);
	
	
	uint8_t data[2] = {0};
	data[0] = EXTERNAL_VER_BCD>>8;
	data[1] = (uint8_t)EXTERNAL_VER_BCD;
	
	BCD8421Ver(data);
}


uint8_t g_external_ver[10] = {0};
void BCD8421Ver(uint8_t *data)
{
	uint8_t *_version;
	//char *c_version;
	
	if(data[1]&0x0F)
	{
		g_external_ver[0] = 'V';
		g_external_ver[1] = 0x30 + ((data[0]&0xf0)>>4);
		g_external_ver[2] = '.';
		g_external_ver[3] = 0x30 + (data[0]&0x0f);
		g_external_ver[4] = 0x30 + ((data[1]&0xf0)>>4);
		
		if((data[1]&0x0f) >= 0x0A)
		{
			g_external_ver[5] = 0x37 + (data[1]&0x0f);
		}
	}
	else
	{
		g_external_ver[0] = 'V';
		g_external_ver[1] = 0x30 + ((data[0]&0xf0)>>4);
		g_external_ver[2] = '.';
		g_external_ver[3] = 0x30 + (data[0]&0x0f);
		g_external_ver[4] = 0x30 + ((data[1]&0xf0)>>4);
		g_external_ver[5] = 0;
	}
	//memcpy(c_version, _version, 10);
	

}


