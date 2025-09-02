
/************************************************************************************************
File Name:      Bootloader.C
Description:    bootloader operation code
Version:        V1.0
Author:         chenweijie
Date:           2018-06-19
*************************************************************************************************/

#include "BootLoader.h"
#include "BootFlashCode.h"
#include "BootUARTCode.h"
#include "BootCANCode.h"
#include "BootSpiFlashCode.H"
//#include "VisionProcessor.h"

#include "BootLoaderConfig.h"
#include "BootIICCode.h"
//config check
#if(COPY_CODE_ROM_END_ADDR > COPY_CODE_ROM_MAX_ADDR)
#error code rom addr error
#endif

#if(COPY_DATA_ROM_START_ADDR < COPY_CODE_ROM_MAX_ADDR)
#error data rom addr error
#endif

#if(0 != (COPY_CODE_ROM_START_ADDR % 4))
#error code rom start addr error
#endif
#if(0 != (COPY_DATA_ROM_START_ADDR % 4))
#error data rom start addr error
#endif

#if(0 != ((COPY_CODE_ROM_END_ADDR + 1) % 4))
#error code rom end addr error
#endif
#if(0 != ((COPY_DATA_ROM_END_ADDR + 1) % 4))
#error data rom end addr error
#endif


#if(0 != (COPY_CODE_ROM_MAX_LEN % 4))
#error code rom len error
#endif
#if(0 != (COPY_DATA_ROM_MAX_LEN % 4))
#error data rom len error
#endif

#if(cdwRAM_COPY_CODE_MAX_ADDR < (COPY_CODE_ROM_START_ADDR + COPY_CODE_ROM_MAX_LEN + COPY_DATA_ROM_MAX_LEN ))
#error copy ram  big
#endif



//typedef char check_reverse_over [(cdwRAM_BOOT_REVERSE_END_ADDR<COPY_DATA_RAM_END_ADDR) ? 1 : -1];

//#define checkAlignment(addr) typedef char check##addr[(0 == addr%4)? 1 : -1]
#define MACRO_CHECK_CONFIG(x,y) typedef char x [(y) ? -1 : 1]

MACRO_CHECK_CONFIG(check1,cdwRAM_BOOT_REVERSE_END_ADDR > COPY_DATA_RAM_END_ADDR);
MACRO_CHECK_CONFIG(check2,cdwRAM_UARTUPDATEINFO_ADDR % 4);
MACRO_CHECK_CONFIG(check3,cdwRAM_UARTTRANSINFO_ADDR % 4);
MACRO_CHECK_CONFIG(check4,cdwRAM_UPDATE_SPIFLASHINFO_ADDR % 4);
MACRO_CHECK_CONFIG(check5,cdwRAM_UPDATEPACKINFO_ADDR % 4);
MACRO_CHECK_CONFIG(check6,cdwRAM_DEVTREEINFO_ADDR % 4);
MACRO_CHECK_CONFIG(check7,COPY_CODE_RAM_OFFSET % 4);
MACRO_CHECK_CONFIG(check8,COPY_DATA_RAM_OFFSET% 4);





#if (0 == IWDG_SWITCH_ENABLE)
#warning IWGCLOSE
#endif

//#include "drv_rs485.h"
static void sBootLoaderStart(void);
static void sICPProcess(void);
//static void sHardwareInit(void);
//static void sBootClocksInit(void);
//static void sBootSOSC_init_8MHz(void);
//static void sBootSPLL_init_160MHz(void);
//static void sBootNormalRUNmode_80MHz(void);
static INT8U sCopyROMCodeToRAM(INT8U *pbRAMStartAddr, INT16U wRAMLen, INT8U *pbROMStartAddr, INT8U *pbROMEndAddr);
//static void sBootPowerCtrl(void);


/*********************************************************
* RAM variable definition
*********************************************************/
//variables
//address offset of MCU flash code in bootloader
INT32U dwBootFlashCodeAddrOffset                        @cdwRAM_BOOTCODEADDR1_OFFSET_ADDR;
//address offset of UART code in bootloader
INT32U dwBootUARTCodeAddrOffset                         @cdwRAM_BOOTCODEADDR2_OFFSET_ADDR;
//address offset of CAN code in bootloader
INT32U dwBootCANCodeAddrOffset                          @cdwRAM_BOOTCODEADDR3_OFFSET_ADDR;
//address offset of Spi flash code in bootloader
INT32U dwBootSpiFlashCodeAddrOffset                     @cdwRAM_BOOTCODEADDR4_OFFSET_ADDR;
//address offset of rodata in bootloader
INT32U dwBootRodataAddrOffset                           @cdwRAM_BOOTCODEADDR5_OFFSET_ADDR;
UpdateUARTInfo stUpdateUARTInfo                         @cdwRAM_UARTUPDATEINFO_ADDR;
UARTTransInfo stTransUARTInfo                           @cdwRAM_UARTTRANSINFO_ADDR;
UpdateSPIFlashInfo stUpdateSPIFlashInfo                 @cdwRAM_UPDATE_SPIFLASHINFO_ADDR;
UpdatePackInfo stUpdatePackInfo                         @cdwRAM_UPDATEPACKINFO_ADDR;
DevTreeInfo stBootDevTree[cbMaxNode]                    @cdwRAM_DEVTREEINFO_ADDR;


/*********************************************************
* constants, RODATA
*********************************************************/


//APP version in app area
__root const INT8U bFrimwareVerStr[3]                          @cdwFIRMWARE_VER_STR_ADDR =
{
    (INT8U)cbAppVersion1,
    (INT8U)(cwAppVersion2 >> 8),
    (INT8U)cwAppVersion2,
};




__root const INT8U bBootFirmwareStr[2]                         @cdwBOOT_FIRMWARE_ADDR =
{
    (INT8U)cbBootVersion1,
    (INT8U)cbBootVersion2,
};
__root const INT8U bFirmwareTypeStr[4]                         @cdwFIRMWARE_TYPE_ADDR =
{
    (INT8U)cbFirmwareType1,
    (INT8U)(cdwFirmwareType2 >> 16),
    (INT8U)(cdwFirmwareType2 >> 8),
    (INT8U)cdwFirmwareType2,
};
__root const INT8U bFirmwareAppStartAddr[4]                    @cdwFWAPP_START_ADDR =
{
    (INT8U)(cdwAPP_SECTOR_START >> 24),
    (INT8U)(cdwAPP_SECTOR_START >> 16),
    (INT8U)(cdwAPP_SECTOR_START >> 8),
    (INT8U)cdwAPP_SECTOR_START,
};
__root const INT8U bFirmwareBootEndAddr[4]                     @cdwFWBOOT_END_ADDR =
{
    (INT8U)(cdwBOOTLOADER_SECTOR_END >> 24),
    (INT8U)(cdwBOOTLOADER_SECTOR_END >> 16),
    (INT8U)(cdwBOOTLOADER_SECTOR_END >> 8),
    (INT8U)cdwBOOTLOADER_SECTOR_END,
};
__root const INT8U bFirmwareBootStartAddr[4]                   @cdwFWBOOT_START_ADDR =
{
    (INT8U)(cdwMCU_VECTOR_SECTOR_START >> 24),
    (INT8U)(cdwMCU_VECTOR_SECTOR_START >> 16),
    (INT8U)(cdwMCU_VECTOR_SECTOR_START >> 8),
    (INT8U)cdwMCU_VECTOR_SECTOR_START,
};

//extern constants, functions' points
//add extern points to the front of the existing points table
__root const INT32U dwBootExtTbl[cbMaxExtTable]                @cwBOOT_EXTTBL_ADDR =
{
    0,
    (INT32U)bFirmwareTypeStr,
    (INT32U)bBootFirmwareStr,
    (INT32U)sBootLoaderStart,
    (INT32U)sBootIcpCmdAnalysis,
    (INT32U)sBootSubsetICPCmdAnalysis,
    (INT32U)sCheckToTransparentCmd,
    (INT32U)sCheckToResponseCmd,
//    (INT32U)sBootCANReceive,
//    (INT32U)sBootUARTToCANSend,
//    (INT32U)sBootCANSendSec,
    0,//(INT32U)sBootDeviceTreeInit,
    0,//(INT32U)bFirmwareBootEndAddr,
    0,//(INT32U)bFirmwareBootStartAddr,
    0,//(INT32U)bFirmwareAppStartAddr,
};

//bootloader constants, rodata
__root const INT8U bBootRodataEnd[2]                           @cdwBOOT_RODATA_END_ADDR = {0xFF,0xFF};
__root const INT16U wCRCTableAbs[16]                           @cdwCRC_TABLE_ADDR=
{
    0x0000,
    0xCC01,
    0xD801,
    0x1400,
    0xF001,
    0x3C00,
    0x2800,
    0xE401,
    0xA001,
    0x6C00,
    0x7800,
    0xB401,
    0x5000,
    0x9C01,
    0x8801,
    0x4400,
};

__root const INT16U wBootRxDelay_500us[cbSetBandRateMax]       @cdwBOOT_RXDELAY_500US_ADDR=
{
    //rx delay between 2bytes (*500us)
    0x002A,
    0x0016,
    0x000C,
    0x0008,
    0x0004,
    0x0004,
    0x0004,
};

__root const INT32U cwBootBaudRateTab[cbSetBandRateMax]        @cdwBOOT_BAUD_TAB=
{
    cwBOOT_BAUD_RATE_1200,
    cwBOOT_BAUD_RATE_2400,
    cwBOOT_BAUD_RATE_4800,
    cwBOOT_BAUD_RATE_9600,
    cwBOOT_BAUD_RATE_19200,
    cwBOOT_BAUD_RATE_38400,
    cwBOOT_BAUD_RATE_115200,
    cwBOOT_BAUD_RATE_750000
};

#if (cbBootReverseDataLen != 0)
__root const INT8U bBootRodataStart[cbBootReverseDataLen]      @cdwBOOT_RODATA_START_ADDR=
{
    0xFF,
};
#else
#define bBootRodataStart                                cwBootBaudRateTab

#endif  //#if (cbBootReverseDataLen != 0)
//#define M_NMI_INT_DISABLE()                     (SIM->SOPT0 &= ~SIM_SOPT0_NMIE_MASK)

#define SYSCLK_USE_HSI     0
#define SYSCLK_USE_HSE     1
#define SYSCLK_USE_HSI_PLL 2
#define SYSCLK_USE_HSE_PLL 3
#ifndef SYSCLK_FREQ
#define SYSCLK_FREQ 144000000
#endif
#ifndef SYSCLK_SRC
#define SYSCLK_SRC SYSCLK_USE_HSE_PLL
#endif
#if SYSCLK_SRC == SYSCLK_USE_HSI
#if SYSCLK_FREQ != HSI_VALUE
#error SYSCL_FREQ must be set to HSI_VALUE
#endif
#elif SYSCLK_SRC == SYSCLK_USE_HSE
#ifndef HSE_VALUE
#error HSE_VALUE must be defined!
#endif
#if SYSCLK_FREQ != HSE_VALUE
#error SYSCL_FREQ must be set to HSE_VALUE
#endif
#elif SYSCLK_SRC == SYSCLK_USE_HSI_PLL
#if (SYSCLK_FREQ % (HSI_VALUE / 2) == 0) && (SYSCLK_FREQ / (HSI_VALUE / 2) >= 2)                                       \
    && (SYSCLK_FREQ / (HSI_VALUE / 2) <= 32)
#define PLLSRC_DIV 2
#define PLL_MUL    (SYSCLK_FREQ / (HSI_VALUE / 2))

#else
#error Cannot make a PLL multiply factor to SYSCLK_FREQ.
#endif

#elif SYSCLK_SRC == SYSCLK_USE_HSE_PLL

#ifndef HSE_VALUE
#error HSE_VALUE must be defined!
#endif

#if ((SYSCLK_FREQ % (HSE_VALUE / 2)) == 0) && (SYSCLK_FREQ / (HSE_VALUE / 2) >= 2)                                     \
    && (SYSCLK_FREQ / (HSE_VALUE / 2) <= 32)

#define PLLSRC_DIV 2
#define PLL_MUL    (SYSCLK_FREQ / (HSE_VALUE / 2))

#elif (SYSCLK_FREQ % HSE_VALUE == 0) && (SYSCLK_FREQ / HSE_VALUE >= 2) && (SYSCLK_FREQ / HSE_VALUE <= 32)

#define PLLSRC_DIV 1
#define PLL_MUL    (SYSCLK_FREQ / HSE_VALUE)

#else
#error Cannot make a PLL multiply factor to SYSCLK_FREQ.
#endif

#else
#error wrong value for SYSCLK_SRC
#endif

/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET 0x0 /*!< Vector Table base offset field. This value must be a multiple of 0x200. */

/**
 * @brief  Configures the System clock frequency, HCLK, PCLK2 and PCLK1
 * prescalers.
 */
static void BootSetSysClock(void)
{
    uint32_t rcc_cfgr       = 0;
    bool HSEStatus          = 0;
    uint32_t StartUpCounter = 0;

#if SYSCLK_SRC == SYSCLK_USE_HSE || SYSCLK_SRC == SYSCLK_USE_HSE_PLL

    /* Enable HSE */
    RCC->CTRL |= ((uint32_t)RCC_CTRL_HSEEN);

    /* Wait till HSE is ready and if Time out is reached exit */
    do
    {
        HSEStatus = RCC->CTRL & RCC_CTRL_HSERDF;
        StartUpCounter++;
    } while ((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    HSEStatus = ((RCC->CTRL & RCC_CTRL_HSERDF) != RESET);
    if (!HSEStatus)
    {
        /* If HSE fails to start-up, the application will have wrong clock
         * configuration. User can add here some code to deal with this error */
        //SystemCoreClock = HSI_VALUE;
        return;
    }
#endif

    /* Flash wait state
        0: HCLK <= 32M
        1: HCLK <= 64M
        2: HCLK <= 96M
        3: HCLK <= 128M
        4: HCLK <= 144M
     */
    FLASH->AC &= (uint32_t)((uint32_t)~FLASH_AC_LATENCY);
    FLASH->AC |= (uint32_t)((SYSCLK_FREQ - 1) / 32000000);

    /* HCLK = SYSCLK */
    RCC->CFG |= (uint32_t)RCC_CFG_AHBPRES_DIV1;

    /* PCLK2 max 72M */
    if (SYSCLK_FREQ > 72000000)
    {
        RCC->CFG |= (uint32_t)RCC_CFG_APB2PRES_DIV2;
    }
    else
    {
        RCC->CFG |= (uint32_t)RCC_CFG_APB2PRES_DIV1;
    }

    /* PCLK1 max 36M */
    if (SYSCLK_FREQ > 72000000)
    {
        RCC->CFG |= (uint32_t)RCC_CFG_APB1PRES_DIV4;
    }
    else if (SYSCLK_FREQ > 36000000)
    {
        RCC->CFG |= (uint32_t)RCC_CFG_APB1PRES_DIV2;
    }
    else
    {
        RCC->CFG |= (uint32_t)RCC_CFG_APB1PRES_DIV1;
    }

#if SYSCLK_SRC == SYSCLK_USE_HSE
    /* Select HSE as system clock source */
    RCC->CFG &= (uint32_t)((uint32_t) ~(RCC_CFG_SCLKSW));
    RCC->CFG |= (uint32_t)RCC_CFG_SCLKSW_HSE;

    /* Wait till HSE is used as system clock source */
    while ((RCC->CFG & (uint32_t)RCC_CFG_SCLKSTS) != (uint32_t)0x04)
    {
    }
#elif SYSCLK_SRC == SYSCLK_USE_HSI_PLL || SYSCLK_SRC == SYSCLK_USE_HSE_PLL

    /* clear bits */
    RCC->CFG &= (uint32_t)((uint32_t) ~(RCC_CFG_PLLSRC | RCC_CFG_PLLHSEPRES | RCC_CFG_PLLMULFCT));

    /* set PLL source */
    rcc_cfgr = RCC->CFG;
    rcc_cfgr |= (SYSCLK_SRC == SYSCLK_USE_HSI_PLL ? RCC_CFG_PLLSRC_HSI_DIV2 : RCC_CFG_PLLSRC_HSE);

#if SYSCLK_SRC == SYSCLK_USE_HSE_PLL
    rcc_cfgr |= (PLLSRC_DIV == 1 ? RCC_CFG_PLLHSEPRES_HSE : RCC_CFG_PLLHSEPRES_HSE_DIV2);
#endif

    /* set PLL multiply factor */
#if PLL_MUL <= 16
    rcc_cfgr |= (PLL_MUL - 2) << 18;
#else
    rcc_cfgr |= ((PLL_MUL - 17) << 18) | (1 << 27);
#endif

    RCC->CFG = rcc_cfgr;

    /* Enable PLL */
    RCC->CTRL |= RCC_CTRL_PLLEN;

    /* Wait till PLL is ready */
    while ((RCC->CTRL & RCC_CTRL_PLLRDF) == 0)
    {
    }

    /* Select PLL as system clock source */
    RCC->CFG &= (uint32_t)((uint32_t) ~(RCC_CFG_SCLKSW));
    RCC->CFG |= (uint32_t)RCC_CFG_SCLKSW_PLL;

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFG & (uint32_t)RCC_CFG_SCLKSTS) != (uint32_t)0x08)
    {
    }
#endif
}



void BootSystemInit(void)
{
    /* FPU settings
     * ------------------------------------------------------------*/
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10 and CP11 Full Access */
#endif

    /* Reset the RCC clock configuration to the default reset state(for debug purpose) */
    /* Set HSIEN bit */
    RCC->CTRL |= (uint32_t)0x00000001;

    /* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
    RCC->CFG &= (uint32_t)0xF8FFC000;

    /* Reset HSEON, CLKSSEN and PLLEN bits */
    RCC->CTRL &= (uint32_t)0xFEF6FFFF;

    /* Reset HSEBYP bit */
    RCC->CTRL &= (uint32_t)0xFFFBFFFF;

    /* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRES/OTGFSPRE bits */
    RCC->CFG &= (uint32_t)0xF700FFFF;

    /* Reset CFG2 register */
    RCC->CFG2 = 0x00000000;

    /* Reset CFG3 register */
    RCC->CFG3 = 0x00000000;

    /* Disable all interrupts and clear pending bits  */
    RCC->CLKINT = 0x009F0000;

    /* Enable ex mode */
    RCC->APB1PCLKEN |= RCC_APB1PCLKEN_PWREN;
    PWR->CTRL3 |= 0x00000001;
    RCC->APB1PCLKEN &= (uint32_t)(~RCC_APB1PCLKEN_PWREN);

    /* Enable ICACHE and Prefetch Buffer */
     FLASH->AC |= (uint32_t)(FLASH_AC_ICAHEN | FLASH_AC_PRFTBFEN);

//#ifdef DATA_IN_ExtSRAM
//    SystemInit_ExtMemCtl();
//#endif /* DATA_IN_ExtSRAM */

    /* Configure the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers */
    /* Configure the Flash Latency cycles and enable prefetch buffer */
    BootSetSysClock();

#ifdef VECT_TAB_SRAM
    SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM. */
#else
    SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH. */
#endif
}
/************************************************************************************************
* Input: void
*
* Output: void
*
* Logic discription: reset entry, jump controler
*
* run time:
************************************************************************************************/
void sResetEntry(void)
{
    //reset SP
    sResetSP();
    sClearUserRam();
    sBootLoaderStart();
}

/************************************************************************************************
* Input: void
*
* Output: void
*
* Logic discription: bootloader entry
*
* run time:
************************************************************************************************/
static void sBootLoaderStart(void)
{
    //hardware initial
    HalBootIntDisable();
    //run bootloader
    uint16_t delay = 0xffff;
    while(delay--);
    sICPProcess();


}
//static T_RS485_config rs485_config;//ram clear
//static U_RS485_status rs485_status;




//void gpio_set_mode(GPIO_Module* port,uint8_t pin,E_GPIOS_MODE mode)
//{
//    if (8 <= pin){
//        _HAL_GPIO_SET_MODE_H(port,pin,mode);
//    }else{
//        _HAL_GPIO_SET_MODE_L(port,pin,mode);
//    }
//}

#if(1 == BOOT_LED_ENABLE)

#define LED_OPEN(GPIOS)          M_PORT_SET_PIN_LOW(GPIOS)
#define LED_CLOSE(GPIOS)         M_PORT_SET_PIN_HIGH(GPIOS)
//#include "led.h"
void boot_led_set(void)
{
    //LED_RCC_OPEN();//APB_RCC_OPEN(APB2,GPIOC);

    LED_CLOSE(LED_NET_RED_GPIO);
    HAL_GPIO_SET_MODE(LED_NET_RED_GPIO,OUTPUT_PUSH_PULL);

    LED_CLOSE(LED_NET_GREEN_GPIO);
    HAL_GPIO_SET_MODE(LED_NET_GREEN_GPIO,OUTPUT_PUSH_PULL);


    LED_OPEN(LED_COM_RED_GPIO);
    HAL_GPIO_SET_MODE(LED_COM_RED_GPIO,OUTPUT_PUSH_PULL);

    LED_CLOSE(LED_COM_GREEN_GPIO);
    HAL_GPIO_SET_MODE(LED_COM_GREEN_GPIO,OUTPUT_PUSH_PULL);

}
#else
#define LED_OPEN(GPIOS)
#define LED_CLOSE(GPIOS)
//#include "led.h"
#define boot_led_set()

#endif

//fCopyRamOk = sCopyROMCodeToRAM((INT8U*)(cdwRAM_BOOT_REVERSE_ADDR),
//                               COPY_RAM_CODE_LEN,
//                               (INT8U*)((INT32U)COPY_RAM_CODE_START_ADDR),
//                               (INT8U*)((INT32U)COPY_RAM_CODE_END_ADDR));
//((psVoidFunc)((INT32U)sBootUARTICPProcess + cdwRAM_BOOT_REVERSE_ADDR -COPY_RAM_CODE_START_ADDR ))();
//#define HAL_BOOT_RUN_FLASH


static void sICPProcess(void)
{
//    INT8U fCopyRamOk = false;
//    INT8U bAlignmentFactor = 0;
//    INT8U fJumpUARTRamEN = false;
//    INT16U wDelayCnt = 0;
    uint8_t boot_mode = 0;
    MACRO_IWDG_OPEN();
    boot_mode = stUpdateUARTInfo.fAppUARTICP;
    sClearReverseRam();
    if(cbUpdateFinish == boot_mode){
        if  (cdwAPPMark == *((INT32U*)sAppOkMark)){
            sResetSP();
            sClearUserRam();
            sDoIarcmain();
        }
    }
    if(cbUpdateFromapp > boot_mode){
        stUpdateUARTInfo.fAppUARTICP = boot_mode;
    }else{//ERROR
        M_RESET_MCU();
    }
//    dwBootUARTCodeAddrOffset        = 0;
//    dwBootCANCodeAddrOffset         = 0;
//    dwBootFlashCodeAddrOffset       = 0;
//    dwBootSpiFlashCodeAddrOffset    = 0;
//	dwBootRodataAddrOffset          = 0;
    M_SYSTICK_INIT();//500us system tick
    RCC_GPIO_OPEN_ALL();
    boot_led_set();
    drv_log_init();

    log_info("Bmode%x\r\n",boot_mode);
    log_info("Btype-");
    log_sector((uint8_t*)&bFirmwareTypeStr[0],4);
    log_info("Bver-");
    log_sector((uint8_t*)&bBootFirmwareStr[0],2);

    /*********************************************************
    * hardware and variables initial
    *********************************************************/
    //init UART configuration


    (pstBootUARTConfig->bUARTxFunc)[cbUART5] = cbUART5Func;

#if(true == HalBootI2CFuncEN)
    sBootReadEEInit();
   // sBootReadEEInfo();
#endif  //#if(true == HalBootI2CFuncEN)

#if(true == HalBootSpiFlashFuncEN)
    sBootSpiFlashInit();
#endif  //#if(true == HalBootSpiFlashFuncEN)

    stUpdateUARTInfo.bRs485Addr     = cbCommuAddr;//cbUART5Rs485Addr;
    stUpdateUARTInfo.bICPTxCH       = cbUART5;
    stUpdateUARTInfo.bBootBaudrate  = cbSetBandRate9600;
    //stUpdateUARTInfo.fAppUARTICP    = cbUpdateFromICP; //upadat flash first
    stTransUARTInfo.bTransTxCH      = cbCHNull;
    stUpdateUARTInfo.bParity        = cbUART5Parity;

    sBootDeviceTreeInit(cbCommuAddr);//device tree init

    sBootUARTInit();//UART init
    for(;;){


        #if(HAL_BOOT_RUN_RAM == BOOT_RUN_MODE)
        //COPY code
       if(true == sCopyROMCodeToRAM((INT8U*)COPY_CODE_RAM_START_ADDR ,\
                                       COPY_CODE_ROM_MAX_LEN ,\
                                   (INT8U*)((INT32U)(COPY_CODE_ROM_START_ADDR)),\
                                   (INT8U*)((INT32U)(COPY_CODE_ROM_END_ADDR)))){
            dwBootUARTCodeAddrOffset        = COPY_CODE_RAM_OFFSET;
            dwBootCANCodeAddrOffset         = COPY_CODE_RAM_OFFSET;
            dwBootFlashCodeAddrOffset       = COPY_CODE_RAM_OFFSET;
            dwBootSpiFlashCodeAddrOffset    = COPY_CODE_RAM_OFFSET;
       }
       //COPPY DATA
       if(true == sCopyROMCodeToRAM((INT8U*)COPY_DATA_RAM_START_ADDR ,\
                                       COPY_DATA_ROM_MAX_LEN ,\
                                   (INT8U*)((INT32U)(COPY_DATA_ROM_START_ADDR)),\
                                   (INT8U*)((INT32U)(COPY_DATA_ROM_END_ADDR)))){
            dwBootRodataAddrOffset          = COPY_DATA_RAM_OFFSET;
        }

        #endif


        #if (true == HalBootSpiFlashFuncEN)
        if(cbUpdateFromFlash == stUpdateUARTInfo.fAppUARTICP){
            stUpdateUARTInfo.fAppUARTICP = cbUpdateFromICP;
            MACRO_SpiFlashFunc();
        }
        #endif
        macro_UARTICPProcess();

    }
//#endif


}



/************************************************************************************************
Function name:  sCopyROMCodeToRAM
Description:    copy ROM code to RAM
Calls:
Called By:
Parameters:     None
Return:         true/false: COPY ROM TO RAM OK OR NOT
************************************************************************************************/
static INT8U sCopyROMCodeToRAM(INT8U *pbRAMStartAddr, INT16U wRAMLen, INT8U *pbROMStartAddr, INT8U *pbROMEndAddr)
{
    if((pbROMEndAddr - pbROMStartAddr) <= wRAMLen)
    {
        while(pbROMStartAddr < pbROMEndAddr)
        {
            //HalCopClr();
            *pbRAMStartAddr = *pbROMStartAddr;
            pbRAMStartAddr++;
            pbROMStartAddr++;
        }
        return true;
    }
    else
    {
        return false;
    }

}

//static void sBootPowerCtrl(void)
//{
    //clock enable
//    PCC->PCCn[PCC_PORTA_INDEX] |= PCC_PCCn_CGC_MASK;
//    PCC->PCCn[PCC_PORTB_INDEX] |= PCC_PCCn_CGC_MASK;
//    PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;
//    PCC->PCCn[PCC_PORTE_INDEX] |= PCC_PCCn_CGC_MASK;

    //Power Key
//    PORTB->PCR[13] |=  PORT_PCR_MUX(1); //as GPIO
//    PTB->PDDR &= (~GPIO_PDDR_PDD(1<<13)); //as GPO

    //Power_48V
//    PORTD->PCR[17] |=  PORT_PCR_MUX(1); //as GPIO
//    PTD->PDDR |= GPIO_PDDR_PDD(1 << 17); //as GPO
//    PTD->PDOR |= GPIO_PDOR_PDO(1<<17); //output high

    //Power_3V3
//    PORTD->PCR[16] |=  PORT_PCR_MUX(1); //as GPIO
//    PTD->PDDR |= GPIO_PDDR_PDD(1 << 16); //as GPO
//    PTD->PDOR |= GPIO_PDOR_PDO(1<<16); //output high

    //Power_5V
//    PORTD->PCR[15] |=  PORT_PCR_MUX(1); //as GPIO
//    PTD->PDDR |= GPIO_PDDR_PDD(1 << 15); //as GPO
//    PTD->PDOR |= GPIO_PDOR_PDO(1<<15); //output high

    //Power_CAN_Wakeup
//    PORTD->PCR[5] |=  PORT_PCR_MUX(1); //as GPIO
//    PTD->PDDR |= GPIO_PDDR_PDD(1 << 5); //as GPO
//    PTD->PDOR |= GPIO_PDOR_PDO(1<<5); //output high

    //A7_POWER
//    PORTE->PCR[12] |=  PORT_PCR_MUX(1); //as GPIO
//    PTE->PDDR |= GPIO_PDDR_PDD(1 << 12); //as GPO
//    PTE->PDOR |= GPIO_PDOR_PDO(1<<12); //output high

//}


