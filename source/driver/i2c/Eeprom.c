/*************************************************************************
* File name:	Eeprom.c
* Version:		v00
* Date:			2011-6-30
* Author:		Abel Song
* Modifier:
* Description:	Eeprom data save & store
*************************************************************************/
#include "Eeprom.h"
#include "network_local_device_secquence_operate.h"
#include "application_general_macro.h"
#include "network_eventlog_info_operate.h"
#include "spi_flash_address_allocate.h"
#include "network_local_config.h"
#include "network_local_firmware_update.h"
#include "net_device_data_kinergy.h"
#include "network_energy_statistics.h"
#include "net_device_tree_build.h"
/****************EE Constant definition*********/
#define cbEEReReadMax               3
//10min
#define cwMinSaveEnergyDatPeriod           40000

/***********************************************/
//EEPROM version:   Mark: chenweijie 20160414.3
#define   cbEEVersions_0              0x01
#define   cbEEVersions_1              0x01
#define   cbEEVersions_2              0x01
#define   cbEEVersions_3              0x02
#define   cbEEVersions_4              0x02
#define   cbEEVersions_5              0x02
#define   cbEEVersions_6              0x02
#define   cbEEVersions_7              0x02
#define   cbEEVersions_8              0x02
#define   cbEEVersions_9              0x02
#define   cbEEVersions_10             0x02
#define   cbEEVersions_11             0x02
#define   cbEEVersions_12             0x02
#define   cbEEVersions_13             0x02
#define   cbEEVersions_14             0x02
#define   cbEEVersions_15             0x01
#define   cbEEVersions_16             0x01
#define   cbEEVersions_17             0x01
#define   cbEEVersions_18             0x01
#define   cbEEVersions_19             0x01
#define   cbEEVersions_20             0x01
#define   cbEEVersions_21             0x01
#define   cbEEVersions_22             0x01
#define   cbEEVersions_23             0x01
#define   cbEEVersions_24             0x01
#define   cbEEVersions_25             0x01
#define   cbEEVersions_26             0x01
#define   cbEEVersions_27             0x01

const INT8U cbEEVersions[cwEE_MODULE_NUM] =
{
    cbEEVersions_0,
    cbEEVersions_1,
    cbEEVersions_2,
    cbEEVersions_3,
    cbEEVersions_4,
    cbEEVersions_5,
    cbEEVersions_6,
    cbEEVersions_7,
    cbEEVersions_8,
    cbEEVersions_9,
    cbEEVersions_10,
    cbEEVersions_11,
    cbEEVersions_12,
    cbEEVersions_13,
    cbEEVersions_14,
    cbEEVersions_15,
    cbEEVersions_16,
    cbEEVersions_17,
    cbEEVersions_18,
    cbEEVersions_19,
    cbEEVersions_20,
    cbEEVersions_21,
    cbEEVersions_22,
    cbEEVersions_23,
    cbEEVersions_24,
    cbEEVersions_25,
    cbEEVersions_26,
    cbEEVersions_27,
};

const INT8U EepromLengthTbl[cwEE_MODULE_NUM] =
{
    16,16,4,16,8,4,     //0-5
    16,16,16,16,        //6-9
    16,16,16,16,        //10-13
    16,8,8,             //14-16
    16,16,16,16,        //17-20
    16,16,16,16,        //21-24
    16,16,16,           //25-27
};
/**************EE variable definition***********/
//static variables
static INT8U bEepromRwBuf[16];
static INT8U bEEVersions[cwEE_MODULE_NUM];
static INT8U bCheckSum;


static INT8U bEEPROMReadCnt = 0;      //repeated reading eeprom cnt

//global variables
INT8U bDefaultPageFlags[cwEE_MODULE_NUM];    //Mark: chenweijie 20160423.4
INT8U bEeWriteCtrlFlag[cwEE_MODULE_NUM] = {0,};
INT8U fEEWriteReady = true;
INT8U fSaveEnergyDat = false;
INT8U bReserve = 0;
extern uint8_t b_communication_type;

/**************EE functions definition***********/
static void sSaveToEEPeriodically(void);
static void sEepromPageDefaultOperate(void);
static INT8U sRamSaveToEeprom(void);
/***********************************************/
// Eeprom  setup table
const stEepromTableEntry EE_Table[] =
{
    //address   RAM address,	                                                            default value,                                                                      max value,		   min value, 		Default character
    //page0  //16
    {0,        &bEEVersions[0], 	                                                        cbEEVersions_0,                                                                     0x7F,	            0x10,        	    EEPROM_RESET },
    {1,        &ba_local_device_secquence[0],                                               0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {2,        &ba_local_device_secquence[1],                                               0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {3,        &ba_local_device_secquence[2],                                               0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {4,        &ba_local_device_secquence[3],                                               0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {5,        &ba_local_device_secquence[4],                                               0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {6,        &ba_local_device_secquence[5],                                               0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {7,        &ba_local_device_secquence[6],                                               0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {8,        &ba_local_device_secquence[7],                                               0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {9,        &ba_local_device_secquence[8],                                               0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {10,       &ba_local_device_secquence[9],                                               0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {11,       &ba_local_device_secquence[10],                                              0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {12,       &ba_local_device_secquence[11],                                              0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {13,       &ba_local_device_secquence[12],                                              0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {14,       &ba_local_device_secquence[13],                                              0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {15,       &bCheckSum,                                                                  0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    //page1  //16
    {16,       &bEEVersions[1], 	                                                        cbEEVersions_1,                                                                     0x7F,		       0x10,                EEPROM_RESET },
    {17,       &ba_local_device_secquence[14],                                              0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {18,       &ba_local_device_secquence[15],                                              0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {19,       &ba_local_device_secquence[16],                                              0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {20,       &ba_local_device_secquence[17],                                              0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {21,       &ba_local_device_secquence[18],                                              0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {22,       &ba_local_device_secquence[19],                                              0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {23,       &ba_local_device_secquence[20],                                              0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {24,       &ba_local_device_secquence[21],                                              0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {25,       &ba_local_device_secquence[22],                                              0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {26,       &ba_local_device_secquence[23],                                              0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {27,       &ba_local_device_secquence[24],                                              0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {28,       &ba_local_device_secquence[25],                                              0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {29,       &ba_local_device_secquence[26],                                              0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {30,       &ba_local_device_secquence[27],                                              0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {31,       &bCheckSum,                                                                  0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    //page2  //4
    {32,       &bEEVersions[2], 	                                                        cbEEVersions_2,                                                                     0x7F,	            0x10,        	    EEPROM_RESET },
    {33,       &ba_local_device_secquence[28],                                              0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {34,       &ba_local_device_secquence[29],                                              0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {35,       &bCheckSum,                                                                  0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    //page3  //16
    {36,       &bEEVersions[3], 	                                                        cbEEVersions_3,                                                                     0x7F,		       0x10,                EEPROM_RESET },
    {37,       (((uint8_t *)(&st_event_log_manage_info.w_event_log_inquire_index)) + 0),    0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {38,       (((uint8_t *)(&st_event_log_manage_info.w_event_log_inquire_index)) + 1),    0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {39,       (((uint8_t *)(&st_event_log_manage_info.w_operate_log_inquire_index)) + 0),  0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {40,       (((uint8_t *)(&st_event_log_manage_info.w_operate_log_inquire_index)) + 1),  0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {41,       ((uint8_t *)(&st_event_log_manage_info.dw_event_log_start_index)),           0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {42,       (((uint8_t *)(&st_event_log_manage_info.dw_event_log_start_index)) + 1),     0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {43,       (((uint8_t *)(&st_event_log_manage_info.dw_event_log_start_index)) + 2),     0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {44,       (((uint8_t *)(&st_event_log_manage_info.dw_event_log_start_index)) + 3),     0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {45,       ((uint8_t *)(&st_event_log_manage_info.dw_event_log_end_index)),             0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {46,       (((uint8_t *)(&st_event_log_manage_info.dw_event_log_end_index)) + 1),       0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {47,       (((uint8_t *)(&st_event_log_manage_info.dw_event_log_end_index)) + 2),       0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {48,       (((uint8_t *)(&st_event_log_manage_info.dw_event_log_end_index)) + 3),       0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {49,       ((uint8_t *)(&st_event_log_manage_info.dw_event_log_save_start_addr)),       (uint8_t)((cdw_flash_addr_alloc_event_log_start & 0x000000FF) >> 0),                0xFF,               0,                  EEPROM_RESET },
    {50,       (((uint8_t *)(&st_event_log_manage_info.dw_event_log_save_start_addr)) + 1), (uint8_t)((cdw_flash_addr_alloc_event_log_start & 0x0000FF00) >> 8),                0xFF,               0,                  EEPROM_RESET },
    {51,       &bCheckSum,                                                                  0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    //page4  //8
    {52,       &bEEVersions[4], 	                                                        cbEEVersions_4,                                                                     0x7F,		       0x10,                EEPROM_RESET },
    {53,       (((uint8_t *)(&st_event_log_manage_info.dw_event_log_save_start_addr)) + 2), (uint8_t)((cdw_flash_addr_alloc_event_log_start & 0x00FF0000) >> 16),               0xFF,               0,                  EEPROM_RESET },
    {54,       (((uint8_t *)(&st_event_log_manage_info.dw_event_log_save_start_addr)) + 3), (uint8_t)((cdw_flash_addr_alloc_event_log_start & 0xFF000000) >> 24),               0xFF,               0,                  EEPROM_RESET },
    {55,       (((uint8_t *)(&st_event_log_manage_info.dw_event_log_save_end_addr)) + 0),   (uint8_t)((cdw_flash_addr_alloc_event_log_start & 0x000000FF) >> 0),                0xFF,               0,                  EEPROM_RESET },
    {56,       (((uint8_t *)(&st_event_log_manage_info.dw_event_log_save_end_addr)) + 1),   (uint8_t)((cdw_flash_addr_alloc_event_log_start & 0x0000FF00) >> 8),                0xFF,               0,                  EEPROM_RESET },
    {57,       (((uint8_t *)(&st_event_log_manage_info.dw_event_log_save_end_addr)) + 2),   (uint8_t)((cdw_flash_addr_alloc_event_log_start & 0x00FF0000) >> 16),               0xFF,               0,                  EEPROM_RESET },
    {58,       (((uint8_t *)(&st_event_log_manage_info.dw_event_log_save_end_addr)) + 3),   (uint8_t)((cdw_flash_addr_alloc_event_log_start & 0xFF000000) >> 24),               0xFF,               0,                  EEPROM_RESET },
    {59,       &bCheckSum,                                                                  0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    //page5  //4
    {60,       &bEEVersions[5], 	                                                        cbEEVersions_5,                                                                     0x7F,		       0x10,                EEPROM_RESET },
    {61,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[0])),            0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {62,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[1])),            0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {63,       &bCheckSum,                                                                  0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    //page6  //16
    {64,       &bEEVersions[6], 	                                                        cbEEVersions_6,                                                                     0x7F,		       0x10,                EEPROM_RESET },
    {65,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[2])),            0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {66,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[3])),            0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {67,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[4])),            0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {68,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[5])),            0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {69,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[6])),            0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {70,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[7])),            0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {71,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[8])),            0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {72,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[9])),            0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {73,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[10])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {74,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[11])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {75,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[12])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {76,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[13])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {77,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[14])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {78,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[15])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {79,       &bCheckSum,                                                                  0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    //page7  //16
    {80,       &bEEVersions[7], 	                                                        cbEEVersions_7,                                                                     0x7F,		       0x10,                EEPROM_RESET },
    {81,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[16])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {82,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[17])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {83,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[18])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {84,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[19])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {85,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[20])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {86,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[21])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {87,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[22])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {88,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[23])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {89,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[24])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {90,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[25])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {91,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[26])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {92,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[27])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {93,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[28])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {94,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[29])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {95,       &bCheckSum,                                                                  0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    //page8  //16
    {96,       &bEEVersions[8], 	                                                        cbEEVersions_8,                                                                     0x7F,		       0x10,                EEPROM_RESET },
    {97,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[30])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {98,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[31])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {99,       ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[32])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {100,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[33])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {101,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[34])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {102,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[35])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {103,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[36])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {104,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[37])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {105,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[38])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {106,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[39])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {107,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[40])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {108,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[41])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {109,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[42])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {110,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[43])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {111,      &bCheckSum,                                                                  0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    //page9  //16
    {112,      &bEEVersions[9], 	                                                        cbEEVersions_9,                                                                     0x7F,		       0x10,                EEPROM_RESET },
    {113,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[44])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {114,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[45])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {115,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[46])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {116,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[47])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {117,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[48])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {118,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[49])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {119,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[50])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {120,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[51])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {121,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[52])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {122,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[53])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {123,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[54])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {124,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[55])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {125,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[56])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {126,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[57])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {127,      &bCheckSum,                                                                  0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    //page10  //16
    {128,      &bEEVersions[10], 	                                                        cbEEVersions_10,                                                                     0x7F,		       0x10,                EEPROM_RESET },
    {129,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[58])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {130,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[59])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {131,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[60])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {132,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[61])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {133,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[62])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {134,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_ssid[63])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {135,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[0])),            0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {136,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[1])),            0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {137,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[2])),            0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {138,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[3])),            0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {139,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[4])),            0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {140,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[5])),            0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {141,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[6])),            0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {142,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[7])),            0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {143,      &bCheckSum,                                                                  0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    //page11  //16
    {144,      &bEEVersions[11], 	                                                        cbEEVersions_11,                                                                     0x7F,		       0x10,                EEPROM_RESET },
    {145,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[8])),            0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {146,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[9])),            0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {147,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[10])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {148,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[11])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {149,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[12])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {150,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[13])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {151,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[14])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {152,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[15])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {153,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[16])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {154,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[17])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {155,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[18])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {156,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[19])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {157,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[20])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {158,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[21])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {159,      &bCheckSum,                                                                  0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    //page12  //16
    {160,      &bEEVersions[12], 	                                                        cbEEVersions_12,                                                                     0x7F,		       0x10,                EEPROM_RESET },
    {161,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[22])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {162,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[23])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {163,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[24])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {164,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[25])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {165,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[26])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {166,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[27])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {167,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[28])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {168,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[29])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {169,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[30])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {170,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[31])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {171,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[32])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {172,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[33])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {173,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[34])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {174,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[35])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {175,      &bCheckSum,                                                                  0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    //page13  //16
    {176,      &bEEVersions[13], 	                                                        cbEEVersions_13,                                                                     0x7F,		       0x10,                EEPROM_RESET },
    {177,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[36])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {178,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[37])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {179,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[38])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {180,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[39])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {181,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[40])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {182,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[41])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {183,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[42])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {184,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[43])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {185,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[44])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {186,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[45])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {187,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[46])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {188,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[47])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {189,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[48])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {190,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[49])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {191,      &bCheckSum,                                                                  0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    //page14  //16
    {192,      &bEEVersions[14], 	                                                        cbEEVersions_14,                                                                     0x7F,		       0x10,                EEPROM_RESET },
    {193,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[50])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {194,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[51])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {195,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[52])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {196,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[53])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {197,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[54])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {198,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[55])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {199,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[56])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {200,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[57])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {201,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[58])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {202,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[59])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {203,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[60])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {204,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[61])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {205,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[62])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {206,      ((uint8_t *)(&st_network_ap_connect_info.st_ap_info.ba_pass[63])),           0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {207,      &bCheckSum,                                                                  0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    //page15  //8
    {208,      &bEEVersions[15], 	                                                        cbEEVersions_15,                                                                    0x7F,		       0x10,                EEPROM_RESET },
    {209,      &b_local_firmware_enable,                                                    0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {210,      &boot_mode      ,                                                            0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {211,      &boot_result ,                                                               0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {212,      &boot_config,                                                                0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {213,      &boot_flash,                                                                 0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {214,      &boot_retcnt,                                                                0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {215,      &bCheckSum,                                                                  0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    //page16  //8
    {216,      &bEEVersions[16], 	                                                        cbEEVersions_16,                                                                    0x7F,		       0x10,                EEPROM_RESET },
    {217,      &st_network_ap_set_info.f_wps_enable,                                        0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {218,      &st_network_ap_set_info.f_wps_first_connect,                                 0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {219,      &b_communication_type,                                                       0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {220,      &bReserve,                                                                   0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {221,      &bReserve,                                                                   0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {222,      &bReserve,                                                                   0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {223,      &bCheckSum,                                                                  0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    //page17  //16
    {224,      &bEEVersions[17], 	                                                        cbEEVersions_17,                                                                    0x7F,		       0x10,                EEPROM_RESET },
    {225,      (((uint8_t *)(&dw_power_saving))),                                             0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {226,      (((uint8_t *)(&dw_power_saving)) + 1),                                         0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {227,      (((uint8_t *)(&dw_power_saving)) + 2),                                         0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {228,      (((uint8_t *)(&dw_power_saving)) + 3),                                         0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {229,      (((uint8_t *)(&st_net_set_data_kinergy.w_0000))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {230,      (((uint8_t *)(&st_net_set_data_kinergy.w_0000)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {231,      (((uint8_t *)(&st_net_set_data_kinergy.w_0001))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {232,      (((uint8_t *)(&st_net_set_data_kinergy.w_0001)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {233,      (((uint8_t *)(&st_net_set_data_kinergy.w_0002))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {234,      (((uint8_t *)(&st_net_set_data_kinergy.w_0002)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {235,      (((uint8_t *)(&st_net_set_data_kinergy.w_0003))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {236,      (((uint8_t *)(&st_net_set_data_kinergy.w_0003)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {237,      (((uint8_t *)(&st_net_set_data_kinergy.w_0004))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {238,      (((uint8_t *)(&st_net_set_data_kinergy.w_0004)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {239,      &bCheckSum,                                                                  0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    //page18  //16
    {240,      &bEEVersions[18], 	                                                        cbEEVersions_18,                                                                    0x7F,		       0x10,                EEPROM_RESET },
    {241,      (((uint8_t *)(&st_net_set_data_kinergy.w_0005))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {242,      (((uint8_t *)(&st_net_set_data_kinergy.w_0005)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {243,      (((uint8_t *)(&st_net_set_data_kinergy.w_0006))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {244,      (((uint8_t *)(&st_net_set_data_kinergy.w_0006)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {245,      (((uint8_t *)(&st_net_set_data_kinergy.w_0007))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {246,      (((uint8_t *)(&st_net_set_data_kinergy.w_0007)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {247,      (((uint8_t *)(&st_net_set_data_kinergy.w_0008))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {248,      (((uint8_t *)(&st_net_set_data_kinergy.w_0008)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {249,      (((uint8_t *)(&st_net_set_data_kinergy.w_0009))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {250,      (((uint8_t *)(&st_net_set_data_kinergy.w_0009)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {251,      (((uint8_t *)(&st_net_set_data_kinergy.w_000A))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {252,      (((uint8_t *)(&st_net_set_data_kinergy.w_000A)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {253,      (((uint8_t *)(&st_net_set_data_kinergy.w_000B))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {254,      (((uint8_t *)(&st_net_set_data_kinergy.w_000B)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {255,      &bCheckSum,                                                                  0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    //page19  //16
    {256,      &bEEVersions[19], 	                                                        cbEEVersions_19,                                                                    0x7F,		       0x10,                EEPROM_RESET },
    {257,      (((uint8_t *)(&st_net_set_data_kinergy.w_000C))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {258,      (((uint8_t *)(&st_net_set_data_kinergy.w_000C)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {259,      (((uint8_t *)(&st_net_set_data_kinergy.w_000D))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {260,      (((uint8_t *)(&st_net_set_data_kinergy.w_000D)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {261,      (((uint8_t *)(&st_net_set_data_kinergy.w_000E))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {262,      (((uint8_t *)(&st_net_set_data_kinergy.w_000E)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {263,      (((uint8_t *)(&st_net_set_data_kinergy.w_000F))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {264,      (((uint8_t *)(&st_net_set_data_kinergy.w_000F)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {265,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0010))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {266,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0010)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {267,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0010)) + 2),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {268,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0010)) + 3),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {269,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0011))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {270,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0011)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {271,      &bCheckSum,                                                                  0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    //page20  //16
    {272,      &bEEVersions[20], 	                                                        cbEEVersions_20,                                                                    0x7F,		       0x10,                EEPROM_RESET },
    {273,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0011)) + 2),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {274,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0011)) + 3),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {275,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0012))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {276,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0012)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {277,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0012)) + 2),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {278,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0012)) + 3),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {279,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0013))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {280,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0013)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {281,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0013)) + 2),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {282,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0013)) + 3),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {283,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0014))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {284,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0014)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {285,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0014)) + 2),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {286,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0014)) + 3),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {287,      &bCheckSum,                                                                  0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    //page21  //16
    {288,      &bEEVersions[21], 	                                                        cbEEVersions_21,                                                                    0x7F,		       0x10,                EEPROM_RESET },
    {289,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0015))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {290,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0015)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {291,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0015)) + 2),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {292,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0015)) + 3),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {293,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0016))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {294,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0016)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {295,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0016)) + 2),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {296,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0016)) + 3),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {297,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0017))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {298,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0017)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {399,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0017)) + 2),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {300,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0017)) + 3),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {301,      (((uint8_t *)(&st_net_set_data_kinergy.w_0018))),                               0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {302,      (((uint8_t *)(&st_net_set_data_kinergy.w_0018) + 1)),                           0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {303,      &bCheckSum,                                                                     0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    //page22  //16
    {304,      &bEEVersions[22], 	                                                        cbEEVersions_22,                                                                    0x7F,		       0x10,                EEPROM_RESET },
    {305,      (((uint8_t *)(&st_net_set_data_kinergy.w_0019))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {306,      (((uint8_t *)(&st_net_set_data_kinergy.w_0019) + 1)),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {307,      (((uint8_t *)(&st_net_set_data_kinergy.w_001A))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {308,      (((uint8_t *)(&st_net_set_data_kinergy.w_001A) + 1)),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {309,      (((uint8_t *)(&st_net_set_data_kinergy.w_001B))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {310,      (((uint8_t *)(&st_net_set_data_kinergy.w_001B) + 1)),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {311,      (((uint8_t *)(&st_net_set_data_kinergy.w_001C))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {312,      (((uint8_t *)(&st_net_set_data_kinergy.w_001C) + 1)),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {313,      (((uint8_t *)(&st_net_set_data_kinergy.w_001D))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {314,      (((uint8_t *)(&st_net_set_data_kinergy.w_001D) + 1)),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {315,      (((uint8_t *)(&st_net_set_data_kinergy.w_001E))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {316,      (((uint8_t *)(&st_net_set_data_kinergy.w_001E) + 1)),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {317,      (((uint8_t *)(&st_net_set_data_kinergy.w_001F))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {318,      (((uint8_t *)(&st_net_set_data_kinergy.w_001F) + 1)),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {319,      &bCheckSum,                                                                  0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    //page23  //16
    {320,      &bEEVersions[23], 	                                                        cbEEVersions_23,                                                                    0x7F,		       0x10,                EEPROM_RESET },
    {321,      (((uint8_t *)(&st_net_set_data_kinergy.w_0020))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {322,      (((uint8_t *)(&st_net_set_data_kinergy.w_0020) + 1)),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {323,      (((uint8_t *)(&st_net_set_data_kinergy.w_0021))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {324,      (((uint8_t *)(&st_net_set_data_kinergy.w_0021) + 1)),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {325,      (((uint8_t *)(&st_net_set_data_kinergy.w_0022))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {326,      (((uint8_t *)(&st_net_set_data_kinergy.w_0022) + 1)),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {327,      (((uint8_t *)(&st_net_set_data_kinergy.w_0023))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {328,      (((uint8_t *)(&st_net_set_data_kinergy.w_0023) + 1)),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {329,      (((uint8_t *)(&st_net_set_data_kinergy.w_0024))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {330,      (((uint8_t *)(&st_net_set_data_kinergy.w_0024) + 1)),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {331,      (((uint8_t *)(&st_net_set_data_kinergy.w_0025))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {332,      (((uint8_t *)(&st_net_set_data_kinergy.w_0025) + 1)),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {333,      (((uint8_t *)(&st_net_set_data_kinergy.w_0026))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {334,      (((uint8_t *)(&st_net_set_data_kinergy.w_0026) + 1)),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {335,      &bCheckSum,                                                                  0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    //page24  //16
    {336,      &bEEVersions[24], 	                                                          cbEEVersions_24,                                                                    0x7F,		       0x10,                EEPROM_RESET },
    {337,      (((uint8_t *)(&st_net_set_data_kinergy.w_0027))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {338,      (((uint8_t *)(&st_net_set_data_kinergy.w_0027) + 1)),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {339,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0028))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {340,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0028)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {341,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0028)) + 2),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {342,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0028)) + 3),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {343,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0029))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {344,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0029)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {345,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0029)) + 2),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {346,      (((uint8_t *)(&st_net_set_data_kinergy.dw_0029)) + 3),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {347,      (((uint8_t *)(&st_net_set_data_kinergy.dw_002A))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {348,      (((uint8_t *)(&st_net_set_data_kinergy.dw_002A)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {349,      (((uint8_t *)(&st_net_set_data_kinergy.dw_002A)) + 2),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {350,      (((uint8_t *)(&st_net_set_data_kinergy.dw_002A)) + 3),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {351,      &bCheckSum,                                                                    0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    //page25  //16
    {352,      &bEEVersions[25], 	                                                           cbEEVersions_25,                                                                    0x7F,		       0x10,                EEPROM_RESET },
    {353,      (((uint8_t *)(&st_net_set_data_kinergy.dw_002B))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {354,      (((uint8_t *)(&st_net_set_data_kinergy.dw_002B)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {355,      (((uint8_t *)(&st_net_set_data_kinergy.dw_002B)) + 2),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {356,      (((uint8_t *)(&st_net_set_data_kinergy.dw_002B)) + 3),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {357,      (((uint8_t *)(&st_net_set_data_kinergy.dw_002C))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {358,      (((uint8_t *)(&st_net_set_data_kinergy.dw_002C)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {359,      (((uint8_t *)(&st_net_set_data_kinergy.dw_002C)) + 2),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {360,      (((uint8_t *)(&st_net_set_data_kinergy.dw_002C)) + 3),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {361,      (((uint8_t *)(&st_net_set_data_kinergy.dw_002D))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {362,      (((uint8_t *)(&st_net_set_data_kinergy.dw_002D)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {363,      (((uint8_t *)(&st_net_set_data_kinergy.dw_002D)) + 2),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {364,      (((uint8_t *)(&st_net_set_data_kinergy.dw_002D)) + 3),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {365,      (((uint8_t *)(&st_net_set_data_kinergy.dw_002E))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {366,      (((uint8_t *)(&st_net_set_data_kinergy.dw_002E)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {367,      &bCheckSum,                                                                     0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    //page26  //16
    {368,      &bEEVersions[26], 	                                                           cbEEVersions_26,                                                                    0x7F,		       0x10,                EEPROM_RESET },
    {369,      (((uint8_t *)(&st_net_set_data_kinergy.dw_002E)) + 2),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {370,      (((uint8_t *)(&st_net_set_data_kinergy.dw_002E)) + 3),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {371,      (((uint8_t *)(&st_net_set_data_kinergy.dw_002F))),                              0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {372,      (((uint8_t *)(&st_net_set_data_kinergy.dw_002F)) + 1),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {373,      (((uint8_t *)(&st_net_set_data_kinergy.dw_002F)) + 2),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {374,      (((uint8_t *)(&st_net_set_data_kinergy.dw_002F)) + 3),                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {375,      (((uint8_t *)(&st_net_status_data_kinergy.dw_0180))),                           0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {376,      (((uint8_t *)(&st_net_status_data_kinergy.dw_0180)) + 1),                       0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {377,      (((uint8_t *)(&st_net_status_data_kinergy.dw_0180)) + 2),                       0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {378,      (((uint8_t *)(&st_net_status_data_kinergy.dw_0180)) + 3),                       0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {379,      (((uint8_t *)(&st_net_set_data_kinergy.w_003A))),                               0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {380,      (((uint8_t *)(&st_net_set_data_kinergy.w_003A)) + 1),                           0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {381,      (((uint8_t *)(&st_net_set_data_kinergy.w_003B))),                               0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {382,      (((uint8_t *)(&st_net_set_data_kinergy.w_003B)) + 1),                           0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {383,      &bCheckSum,                                                                     0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },

    //page27  //16
    {384,      &bEEVersions[27], 	                                                           cbEEVersions_27,                                                                    0x7F,		       0x10,                EEPROM_RESET },
    {385,      (uint8_t *)(&gw_agent_facturer), 									            0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {386,      ((uint8_t *)(&gw_agent_facturer)+1),                          					0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },
    {387,      (uint8_t *)(&bReserve),                             								0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {388,      (uint8_t *)(&bReserve),                        									0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {389,      (uint8_t *)(&bReserve), 									                        0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {390,      (uint8_t *)(&bReserve),                           								0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {391,      (uint8_t *)(&bReserve),                                                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {392,      (uint8_t *)(&bReserve),                                                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {393,      (uint8_t *)(&bReserve),                                                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {394,      (uint8_t *)(&bReserve),                                                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {395,      (uint8_t *)(&bReserve),                                                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {396,      (uint8_t *)(&bReserve),                                                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {397,      (uint8_t *)(&bReserve),                                                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {398,      (uint8_t *)(&bReserve),                                                          0,                                                                                  0xFF,               0,                  EEPROM_RESET },
    {399,      &bCheckSum,                                                                      0,                                                                                  0xFF,               0,                  EEPROM_NO_RESET },




};

/************************************************************************************************
* Input:  void
*
* Output: true: void
*
* Logic discription: eeprom timing running functions
*
* run time: 15ms
************************************************************************************************/
void sEepromTimingFuncs(void)
{
    static TickType_t x_start_time = 0;
    TickType_t x_curent_time = 0;
    static TickType_t x_delay_time =  0;
    static uint8_t f_init = TRUE;

    if(f_init)
    {
        f_init = FALSE;
        x_start_time = xTaskGetTickCount();
        x_delay_time =  pdMS_TO_TICKS_USER(50);
    }
    x_curent_time = xTaskGetTickCount();
    if((x_curent_time - x_start_time) >= x_delay_time)
    {
        f_init = TRUE;

        sSaveToEEPeriodically();
        sEepromPageDefaultOperate();
        {
	        uint8_t ba_sn_temp[30] = {0,};
	        get_local_device_secquence(ba_sn_temp,30);
			if(0 == memcmp(ba_sn_temp, "ck654321", 8))
			{
//				uint8_t b_sn_len = 30;
				sEepromRead(EE_Table[1].EE_Addr, 16, bEepromRwBuf);
				for(uint8_t j = 0; j < 14 ; j++)
				{
					ba_local_device_secquence[j] =  bEepromRwBuf[j];
				}
			}
        }
        if(sRamSaveToEeprom())
        {
    //        OSEventSend(PrioUpdateJob,eUpdateWaitEEWriteOK);
        }
        fEEWriteReady = true;
    }
}

/************************************************************************************************
* Input:  void
*
* Output: true: write all ee page OK
*
* Logic discription: Save data to eeprom according to dwEeWriteCtrlFlag.
*                    Check sum saved at the last byte of every page.
*
* run time: 15ms     Mark: chenweijie 20160419.1
************************************************************************************************/
static INT8U sRamSaveToEeprom(void)
{
    INT8U i,j;
    INT16U wPageFirstIndex = 0;
    static INT8U fHaveWritedEE = false;
//	uint8_t ba_sn[16] = "CI0427210398EE";
//	uint8_t b_sn_len = 16;
//    set_local_device_secquence(ba_sn,b_sn_len);
//	set_local_device_communication_type_4g();
    if(fEEWriteReady)
    {
        //Settings need to save
        for(i = 0; i < cwEE_MODULE_NUM; i++)
        {
        	if(fWriteEEPage(i))
        	{

        	    bCheckSum = 0;

           		for(j = 0; j < (EepromLengthTbl[i] - 1); j++)
           		{
           		    bEepromRwBuf[j] = *(EE_Table[wPageFirstIndex + j].RAM_Addr);
            		bCheckSum += *(EE_Table[wPageFirstIndex + j].RAM_Addr);
           		}

            	bEepromRwBuf[EepromLengthTbl[i] - 1] = bCheckSum;

            	// write to EE from RAM
            	sEepromWrite(EE_Table[wPageFirstIndex].EE_Addr, EepromLengthTbl[i], bEepromRwBuf);

        		// clear current eeprom write ctrl flag
        		fWriteEEPage(i) = false;

        		// delay 15ms, because the max write 24c16 time is 10ms
        		fEEWriteReady = false;

                fHaveWritedEE = true;

        		return false;
        	}
        	wPageFirstIndex += EepromLengthTbl[i];
        }

        if(fHaveWritedEE)
        {
            fHaveWritedEE = false;
            return true;
        }
    }

    return false;
}
/************************************************************************************************
* Input:  void
*
* Output: void
*
* Logic discription: Read data from eeprom.
*                    If bFirstPowerOnJudge not correct, then default all pages
*                    If bEEVersions or checksum not correct, then default the current page.
*
* run time: MCU power on                    Mark: chenweijie 20160419.1
************************************************************************************************/
void sEepromReadToRam(void)
{
    INT8U i;
    INT8U j;
    INT16U wPageFirstIndex = 0;

    for(i = 0; i < cwEE_MODULE_NUM; i++)
    {
		sEepromRead(EE_Table[wPageFirstIndex].EE_Addr, EepromLengthTbl[i], bEepromRwBuf);

		bCheckSum = 0;

		for(j = 0; j < (EepromLengthTbl[i] - 1); j++)
		{
		    bCheckSum += bEepromRwBuf[j];
		}

        //Checksum or ee version not correct
        if((bEepromRwBuf[0] != cbEEVersions[i]) || (bEepromRwBuf[EepromLengthTbl[i] - 1] != bCheckSum))
        {
            if(bEEPROMReadCnt >= cbEEReReadMax)
            {
                bEEPROMReadCnt = 0;
                sDefaultEEData(i);
				if(i > 2)
				{
					fDefaultPage(i) = true;  //Save which page has been default
					
					fWriteEEPage(i) = true;   //set ee beg

				}
                wPageFirstIndex += EepromLengthTbl[i];
            }
            else
            {
                bEEPROMReadCnt++;
                i--;       //run this page again.
            }
        }
        else
        {
            bEEPROMReadCnt = 0;
            for(j = 0; j < EepromLengthTbl[i]; j++)
            {
                // load RW BUF value to RAM
                *(EE_Table[wPageFirstIndex + j].RAM_Addr) =  bEepromRwBuf[j];
            }
            wPageFirstIndex += EepromLengthTbl[i];
        }
    }
}

/************************************************************************************************
* Input:  wPageNum: which page of EE to default
*
* Output: void
*
* Logic discription: Assign data to the ram variable of EE_Table if the ram variable is able to
*                    be initialized. Then enable the EEPROM write.
*
* run time:
************************************************************************************************/
void sDefaultEEData(INT16U wPageNum)
{
    INT8U i;
    INT16U wPageFirstIndex = 0;

    for(i = 0; i < wPageNum; i++)
    {
        wPageFirstIndex += EepromLengthTbl[i];
    }
    //mark 2020/10/12 01
    *(EE_Table[wPageFirstIndex + 0].RAM_Addr) =  EE_Table[wPageFirstIndex + 0].Sys_Var_Dflt;//EEPROM Page Data Ver Force To Reset
    for(i = 0; i < EepromLengthTbl[wPageNum]; i++)
	{
	    if(EE_Table[wPageFirstIndex + i].FactoryReset != EEPROM_NO_RESET)
	    {
    	    // load default value to RAM
    	    *(EE_Table[wPageFirstIndex + i].RAM_Addr) =  EE_Table[wPageFirstIndex + i].Sys_Var_Dflt;
	    }
	}
}
/*************************************************************************************
* Input: Null
*
* Output: Null
*
* Logic discription:default eeprom data
*
* run time:
*************************************************************************************/
void sEepromReset(void)
{
    INT8U i;

    for(i = 0; i < cwEE_MODULE_NUM; i++)     //Default all EE pages
    {
        sDefaultEEData(i);

        //set ALL ee beg
        fWriteEEPage(i) = true;
    }

    //read all subsets' settings
    //sReadAllSubsetsSet();
}

/*************************************************************************************
* Input: void
*
* Output: void
*
* Logic discription: save something to eeprom periodically
*
* run time: 15ms
*************************************************************************************/
static void sSaveToEEPeriodically(void)
{

}

/*************************************************************************************
* Input: void
*
* Output: void
*
* Logic discription: eeprom default operation functions
*
* run time: 15ms
*************************************************************************************/
static void sEepromPageDefaultOperate(void)
{
    if(fDefaultPage(0))
    {
        fDefaultPage(0) = false;
    }
}
