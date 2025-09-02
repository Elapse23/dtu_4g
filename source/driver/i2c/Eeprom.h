
#ifndef __EEPROM_H__
#define __EEPROM_H__

//#include "n32g45x.H"
#include "IICDriver.H"
#include "freertos_user_config.h"
#include "portmacro.h"
#include "FreeRTOS.h"
#include "task.h"
#include "BootloaderAPP.h"
typedef enum
{
    EEPROM_RESET,
    EEPROM_NO_RESET
} DEFAULT_RESTORE;

typedef struct
{
    INT16U          EE_Addr;      	// eeprom physical address
    INT8U *         RAM_Addr;     	// pointer to RAM variable
    INT8U           Sys_Var_Dflt; 	// default value
	INT8U           Max_Value;		// the max value of the eeprom data for verify eeprom data
	INT8U			Minx_Value;		// the min value of the eeprom data for verify eeprom data
    DEFAULT_RESTORE FactoryReset;  	// does the value get reset when on "Restore Factory Defaults"?
} stEepromTableEntry;


#define fNotZero(x)             ((x) != 0)
#define cwEE_MODULE_NUM 		28

extern INT8U  bEeWriteCtrlFlag[cwEE_MODULE_NUM];
#define fWriteEEPage(i)         (bEeWriteCtrlFlag[i])

extern INT8U  bDefaultPageFlags[cwEE_MODULE_NUM];
#define fDefaultPage(i)         (bDefaultPageFlags[i])

extern INT8U fEEWriteReady;

extern INT32U dwTimeStampTemp;
extern INT8U fSaveEnergyDat;
void sEepromReadToRam(void);
void sEepromReset(void);
void sEepromTimingFuncs(void);
void sClearEeprom();
void sDefaultEEData(INT16U wPageNum);

#endif
