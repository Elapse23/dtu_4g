#ifndef __VERSION_H__
#define __VERSION_H__


#include "stdint.h"
#include "prj_config.h"



#define MAIN_VER					0
#define SUB_VER 					39

#define BOOTLOADER_VER				"V1.01"
#define EXTERNAL_VER_BCD			0x1010
//#define INNER_VER					14


#define PART_NUMBER_VER							"S99999-0001"
//#define SN_NUMBER								"SN_123456"
#define LOCAL_MONITOR_MODEL						"TS43"


#define cbFirmwareType1									8
#define cdwFirmwareType2								503001
#define cbBootVersion1									4
#define cbBootVersion2									6
#define cdwAPP_SECTOR_START								0x08004000
#define cdwMCU_VECTOR_SECTOR_START						0x08000000
#define cdwBOOTLOADER_SECTOR_END						0x080040FF


#define cdwBOOT_RODATA_END_ADDR                         (cdwBOOTLOADER_SECTOR_END - 2 + 1)//0x3FFE
#define cdwBOOT_FIRMWARE_ADDR                           (cdwBOOT_RODATA_END_ADDR - 2)//0x3FFC
#define cdwFIRMWARE_TYPE_ADDR                           (cdwBOOT_FIRMWARE_ADDR - 4)//0x3FF8
#define cdwFWAPP_START_ADDR                             (cdwFIRMWARE_TYPE_ADDR - 4)//0x3FF4
#define cdwFWBOOT_END_ADDR                              (cdwFWAPP_START_ADDR - 4)//0x3FF0
#define cdwFWBOOT_START_ADDR                            (cdwFWBOOT_END_ADDR - 4)//0x3FEC
	

extern uint8_t g_external_ver[10];

extern void GetSoftwareVersion_Date(char *pDdata);
extern void GetSoftwareVersion(void);
extern void BCD8421Ver(uint8_t *data);


#endif

