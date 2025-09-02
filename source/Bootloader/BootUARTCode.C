/************************************************************************************************
File Name:      BootUARTCode.C
Description:    bootloader UART operation code
Version:        V1.0
Author:         chenweijie
Date:           2018-08-24
*************************************************************************************************/
#include "n32g45x.h"


#include "IcpHead.h"


#include "BootFlashCode.H"

//#include "BSP_USART.h"

#include "BootUARTCode.H"


//#include "BSP_SYSTEM_RCC.h"
//#include "BootCANCode.H"
//#include "BSP_GPIO.h"
//#include "hal_rcc.h"
//#include "hal_gpio.h"
//#include "hal_uart.h"

#include "BootSpiFlashCode.H"

#include "drv_log.h"

typedef INT16U (*pCombineICPCmd)(INT8U bRs485Addr, INT16U wCmdAddr, INT8U *pbData, INT16U wDataLen, INT8U *pbDataOut,INT8U fReturnCmd);
typedef void (*pBulidDevTree)(INT8U *pbDataIn, 
                              INT16U wDataLen,
                              DevTreeInfo *pDevTree,
                              UARTTransInfo *pstTransInfo);





static INT16U sBootCombineICPCmd(INT8U bRs485Addr, INT16U wCmdAddr, INT8U *pbData, INT16U wDataLen, INT8U *pbDataOut,INT8U fReturnCmd);
static void sBootBulidDevTree(INT8U *pbDataIn, 
                              INT16U wDataLen,
                              DevTreeInfo *pDevTree,
                              UARTTransInfo *pstTransInfo);






#define cbNullProcess                           0
#define cbCXCmd                                 1
#define cbCookCmd                               2
#define cbTigerCmd                              3
#define cbTransFirmData                         4
#define cbICPOver                               5

extern void sAppOkMark(void);

#define cwBOOT_ApbAhbPresTable                          (boot_n32g45x_rcc_rodata_end - 16)   
#define cwBOOT_AdcHclkPresTable                         (cwBOOT_ApbAhbPresTable  - 16)
#define cwBOOT_AdcPllClkPresTable                       (cwBOOT_AdcHclkPresTable - (2 *16))

static const uint8_t s_ApbAhbPresTable[16] @cwBOOT_ApbAhbPresTable = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9}; 
static const uint8_t s_AdcHclkPresTable[16] @cwBOOT_AdcHclkPresTable = {1, 2, 4, 6, 8, 10, 12, 16, 32, 32, 32, 32, 32, 32, 32, 32};
static const uint16_t s_AdcPllClkPresTable[16]@cwBOOT_AdcPllClkPresTable ={1, 2, 4, 6, 8, 10, 12, 16, 32, 64, 128, 256, 256, 256, 256, 256};

#define CTRL_HSEBP_RESET  ((uint32_t)0xFFFBFFFF)
#define CTRL_HSEBP_SET    ((uint32_t)0x00040000)
#define CTRL_HSEEN_RESET  ((uint32_t)0xFFFEFFFF)
#define CTRL_HSEEN_SET    ((uint32_t)0x00010000)
#define CTRL_HSITRIM_MASK ((uint32_t)0xFFFFFF07)

/* CFG register bit mask */
#define CFG_PLL_MASK ((uint32_t)0xF7C0FFFF)

#define CFG_PLLMULFCT_MASK      ((uint32_t)0x083C0000)
#define CFG_PLLSRC_MASK         ((uint32_t)0x00010000)
#define CFG_PLLHSEPRES_MASK     ((uint32_t)0x00020000)
#define CFG_SCLKSTS_MASK        ((uint32_t)0x0000000C)
#define CFG_SCLKSW_MASK         ((uint32_t)0xFFFFFFFC)
#define CFG_AHBPRES_RESET_MASK  ((uint32_t)0xFFFFFF0F)
#define CFG_AHBPRES_SET_MASK    ((uint32_t)0x000000F0)
#define CFG_APB1PRES_RESET_MASK ((uint32_t)0xFFFFF8FF)
#define CFG_APB1PRES_SET_MASK   ((uint32_t)0x00000700)
#define CFG_APB2PRES_RESET_MASK ((uint32_t)0xFFFFC7FF)
#define CFG_APB2PRES_SET_MASK   ((uint32_t)0x00003800)

/* CFG2 register bit mask */
#define CFG2_TIM18CLKSEL_SET_MASK   ((uint32_t)0x20000000)
#define CFG2_TIM18CLKSEL_RESET_MASK ((uint32_t)0xDFFFFFFF)
#define CFG2_RNGCPRES_SET_MASK      ((uint32_t)0x1F000000)
#define CFG2_RNGCPRES_RESET_MASK    ((uint32_t)0xE0FFFFFF)
#define CFG2_ETHCLKSEL_SET_MASK     ((uint32_t)0x00100000)
#define CFG2_ETHCLKSEL_RESET_MASK   ((uint32_t)0xFFEFFFFF)
#define CFG2_ADC1MSEL_SET_MASK      ((uint32_t)0x00000400)
#define CFG2_ADC1MSEL_RESET_MASK    ((uint32_t)0xFFFFFBFF)
#define CFG2_ADC1MPRES_SET_MASK     ((uint32_t)0x0000F800)
#define CFG2_ADC1MPRES_RESET_MASK   ((uint32_t)0xFFFF07FF)
#define CFG2_ADCPLLPRES_SET_MASK    ((uint32_t)0x000001F0)
#define CFG2_ADCPLLPRES_RESET_MASK  ((uint32_t)0xFFFFFE0F)
#define CFG2_ADCHPRES_SET_MASK      ((uint32_t)0x0000000F)
#define CFG2_ADCHPRES_RESET_MASK    ((uint32_t)0xFFFFFFF0)

/* CFG3 register bit mask */
#define CFGR3_TRNG1MSEL_SET_MASK    ((uint32_t)0x00020000)
#define CFGR3_TRNG1MSEL_RESET_MASK  ((uint32_t)0xFFFDFFFF)
#define CFGR3_TRNG1MPRES_SET_MASK   ((uint32_t)0x0000F800)
#define CFGR3_TRNG1MPRES_RESET_MASK ((uint32_t)0xFFFF07FF)


void sBootParitySet(USART_Module* USARTx, INT8U bParity)
{
    //USARTx->CTRL1 &= ~(cdwUSART_CTRL1_PARITY_ODD|cdwUSART_CTRL1_DATA_LEN_9B);//CLR
    if(cbSetNoCheck == bParity)
    {
        USARTx->CTRL1 |= cdwUSART_CTRL1_PARITY_NO|cdwUSART_CTRL1_DATA_LEN_8B;
    }
    else if(cbSetOddCheck == bParity)
    {
        USARTx->CTRL1 |= cdwUSART_CTRL1_PARITY_ODD|cdwUSART_CTRL1_DATA_LEN_9B;
    }
    else if(cbSetEvenCheck == bParity)
    {
        USARTx->CTRL1 |= cdwUSART_CTRL1_PARITY_EVEN|cdwUSART_CTRL1_DATA_LEN_9B;
    }
}



void Boot_RCC_GetClocksFreqValue(RCC_ClocksType* RCC_Clocks)
{
    uint32_t tmp = 0, pllclk = 0, pllmull = 0, pllsource = 0, presc = 0;

    /* Get PLL clock source and multiplication factor ----------------------*/
    pllmull   = RCC->CFG & CFG_PLLMULFCT_MASK;
    pllsource = RCC->CFG & CFG_PLLSRC_MASK;

    if ((pllmull & RCC_CFG_PLLMULFCT_4) == 0)
    {
        pllmull = (pllmull >> 18) + 2; // PLLMUL[4]=0
    }
    else
    {
        pllmull = ((pllmull >> 18) - 496) + 1; // PLLMUL[4]=1
    }

    if (pllsource == 0x00)
    { /* HSI oscillator clock divided by 2 selected as PLL clock entry */
        pllclk = (HSI_VALUE >> 1) * pllmull;
    }
    else
    {
        /* HSE selected as PLL clock entry */
        if ((RCC->CFG & CFG_PLLHSEPRES_MASK) != (uint32_t)RESET)
        { /* HSE oscillator clock divided by 2 */
            pllclk = (HSE_VALUE >> 1) * pllmull;
        }
        else
        {
            pllclk = HSE_VALUE * pllmull;
        }
    }

    /* Get SYSCLK source -------------------------------------------------------*/
    tmp = RCC->CFG & CFG_SCLKSTS_MASK;

    switch (tmp)
    {
    case 0x00: /* HSI used as system clock */
        RCC_Clocks->SysclkFreq = HSI_VALUE;
        break;
    case 0x04: /* HSE used as system clock */
        RCC_Clocks->SysclkFreq = HSE_VALUE;
        break;
    case 0x08: /* PLL used as system clock */
        RCC_Clocks->SysclkFreq = pllclk;
        break;

    default:
        RCC_Clocks->SysclkFreq = HSI_VALUE;
        break;
    }

    /* Compute HCLK, PCLK1, PCLK2 and ADCCLK clocks frequencies ----------------*/
    /* Get HCLK prescaler */
    tmp   = RCC->CFG & CFG_AHBPRES_SET_MASK;
    tmp   = tmp >> 4;
    presc = s_ApbAhbPresTable[tmp];
    /* HCLK clock frequency */
    RCC_Clocks->HclkFreq = RCC_Clocks->SysclkFreq >> presc;
    /* Get PCLK1 prescaler */
    tmp   = RCC->CFG & CFG_APB1PRES_SET_MASK;
    tmp   = tmp >> 8;
    presc = s_ApbAhbPresTable[tmp];
    /* PCLK1 clock frequency */
    RCC_Clocks->Pclk1Freq = RCC_Clocks->HclkFreq >> presc;
    /* Get PCLK2 prescaler */
    tmp   = RCC->CFG & CFG_APB2PRES_SET_MASK;
    tmp   = tmp >> 11;
    presc = s_ApbAhbPresTable[tmp];
    /* PCLK2 clock frequency */
    RCC_Clocks->Pclk2Freq = RCC_Clocks->HclkFreq >> presc;

    /* Get ADCHCLK prescaler */
    tmp   = RCC->CFG2 & CFG2_ADCHPRES_SET_MASK;
    presc = s_AdcHclkPresTable[tmp];
    /* ADCHCLK clock frequency */
    RCC_Clocks->AdcHclkFreq = RCC_Clocks->HclkFreq / presc;
    /* Get ADCPLLCLK prescaler */
    tmp   = RCC->CFG2 & CFG2_ADCPLLPRES_SET_MASK;
    tmp   = tmp >> 4;
    presc = s_AdcPllClkPresTable[(tmp & 0xF)]; // ignore BIT5
    /* ADCPLLCLK clock frequency */
    RCC_Clocks->AdcPllClkFreq = pllclk / presc;
}






/*************************************************************************************
* Input: void
*
* Output: void
* 
* Logic discription: bootloader USART BaudRate
*
* run time:
*************************************************************************************/
void sBootBaudRateSet(USART_Module* USARTx, INT32U dwBaudRate)
{
    uint32_t tmpregister = 0x00, apbclock = 0x00;
    uint32_t integerdivider    = 0x00;
    uint32_t fractionaldivider = 0x00;
    uint32_t usartxbase        = 0;
    usartxbase = (uint32_t)USARTx;
    RCC_ClocksType RCC_ClocksStatus;
    /*---------------------------- USART PBC Configuration -----------------------*/
    /* Configure the USART Baud Rate -------------------------------------------*/
    Boot_RCC_GetClocksFreqValue(&RCC_ClocksStatus);
    if ((usartxbase == USART1_BASE) || (usartxbase == UART6_BASE) || (usartxbase == UART7_BASE))
    {
        apbclock = cdwAPB2_FREQ;
    }
    else
    {
        apbclock = cdwAPB1_FREQ;
    }
    /* Determine the integer part */
    integerdivider = ((25 * apbclock) / (4 * dwBaudRate));
    tmpregister = (integerdivider / 100) << 4;

    /* Determine the fractional part */
    fractionaldivider = integerdivider - (100 * (tmpregister >> 4));

    /* Implement the fractional part in the register */   
    tmpregister |= ((((fractionaldivider * 16) + 50) / 100)) & ((uint8_t)0x0F);
    USARTx->BRCF = (uint16_t)tmpregister;

    /* Write to USART PBC */

}
/*************************************************************************************
* Input: void
*
* Output: void
* 
* Logic discription: bootloader UART0 initialization
*
* run time:
*************************************************************************************/
void sBootUartxInit(INT8U bUARTx, INT8U bBaudrate, INT8U bParity)
{
//    INT8U bTemp;
    
    if(bBaudrate >= cbSetBandRateMax)
    {
        bBaudrate = cbSetBandRate9600;
    }
    
    if(cbUSART2 == bUARTx)
    {
#if (fUSART2Enable == true)     
        HalRccPeriphOn(cbRCC_APB1, cdwRCC_APB1_PERIPH_USART2);
        HalRccPeriphOn(cbRCC_APB2, cdwRCC_APB2_PERIPH_AFIO);//AFIO CLK EN
        
        AFIO->RMP_CFG  |= (cdwUSART2_PIN_MAP_SET & 0x0000FFFF);
        AFIO->RMP_CFG3 |= (cdwUSART2_PIN_MAP_SET & 0xFFFF0000);
        
        sBootParitySet(USART2,bParity);
        
        macro_BaudRateSet(USART2,(INT32U)cwBootBaudRateTab[bBaudrate]);
        
        HalUartRxEnable(USART2);
        HalUartTxEnable(USART2);
        HalUartEnable(USART2);//Usart enable.
        
#endif //#if (fUSART2Enable == true)
    }
    else if(cbUSART3 == bUARTx)
    {
#if (fUSART3Enable == true)     
        HalRccPeriphOn(cbRCC_APB1, cdwRCC_APB1_PERIPH_USART3);
        HalRccPeriphOn(cbRCC_APB2, cdwRCC_APB2_PERIPH_AFIO);//AFIO CLK EN
        
        AFIO->RMP_CFG  |= cdwUSART3_PIN_MAP_SET;
        
        sBootParitySet(USART3,bParity);
        
        macro_BaudRateSet(USART3,(INT32U)cwBootBaudRateTab[bBaudrate]);
        
        HalUartRxEnable(USART3);
        HalUartTxEnable(USART3);
        HalUartEnable(USART3);//Usart enable.
        
#endif //#if (fUSART3Enable == true)
    }
    else if(cbUART4 == bUARTx)
    {
#if (fUART4Enable == true)     
        HalRccPeriphOn(cbRCC_APB1, cdwRCC_APB1_PERIPH_UART4);
        HalRccPeriphOn(cbRCC_APB2, cdwRCC_APB2_PERIPH_AFIO);//AFIO CLK EN
        
        AFIO->RMP_CFG3 |= cdwUSART4_PIN_MAP_SET;
        
        sBootParitySet(UART4,bParity);
        
        macro_BaudRateSet(UART4,(INT32U)cwBootBaudRateTab[bBaudrate]);
        
        HalUartRxEnable(UART4);
        HalUartTxEnable(UART4);
        HalUartEnable(UART4);//Usart enable.
        
#endif //#if (fUART4Enable == true)    
    }
    else if(cbUART5 == bUARTx)
    {
#if (fUART5Enable == true)     
        //HalRccPeriphOn(cbRCC_APB1, cdwRCC_APB1_PERIPH_UART5);
       // HalRccPeriphOn(cbRCC_APB2, cdwRCC_APB2_PERIPH_AFIO);//AFIO CLK EN
        APB_RCC_OPEN(APB1,UART5);
        APB_RCC_OPEN(APB2,AFIO);

        AFIO->RMP_CFG3 |= cdwUSART5_PIN_MAP_SET;
#if (cdwUSART5_PIN_MAP_SET == cdwUART5_MAP_ON_PC12_PD2)
        RS485_GPIO_RCC_OPEN();
        HAL_GPIO_SET_HIGHT(RS485_TX_GPIO);
        HAL_GPIO_SET_MODE(RS485_TX_GPIO,AF_MODE_PUSH_PULL);
        
        HAL_GPIO_SET_HIGHT(RS485_RX_GPIO);
        HAL_GPIO_SET_MODE(RS485_RX_GPIO,INPUT_PULL_UP);    
#endif
        RS485_UART_RCC_CLOSE();//reset
        RS485_UART_RCC_OPEN();

        sBootParitySet(UART5,bParity);
        macro_BaudRateSet(UART5,9600);//(INT32U)cwBootBaudRateTab[bBaudrate]);

        //log_info("parity %d,rate %d \r\n",bParity,(INT32U)cwBootBaudRateTab[bBaudrate]);

        HalUartRxEnable(UART5);
        HalUartTxEnable(UART5);
        HalUartEnable(UART5);//Usart enable.
        
#endif    
    }
    
}

void sBootUARTInit(void)
{
    //UART info init
    (pstBootUARTConfig->bParity)[cbUART5]  = stUpdateUARTInfo.bParity;
    (pstBootUARTConfig->bBaudrate)[cbUART5]= stUpdateUARTInfo.bBootBaudrate;
#if (fUART5Enable == true)
    sBootUartxInit(cbUART5, (pstBootUARTConfig->bBaudrate)[cbUART5],
                   (pstBootUARTConfig->bParity)[cbUART5]);
#endif 

#if(true == HalBootUSART5RS485EN)
    HAL_GPIO_SET_MODE(RS485_DR_GPIO,OUTPUT_PUSH_PULL);
    HalBootUSart5RS485RxEnable();
#endif  //#if(true == HalBootUSART2RS485EN) 
}
/************************************************************************************************
* following functions can be copy to RAM (... ~ sBootUARTCodeEnd())
*************************************************************************************************/
void sBootUARTCodeStart(void)
{
}

/************************************************************************************************
* Input: void
*
* Output: void
*
* Logic discription: bootloader UART ICP process, UART ICP communication, flash operation
*
* run time:
************************************************************************************************/
void sBootUARTICPProcess(void)
{
    
    INT8U bICPReTimer = 0;
   // INT8U bTransReTimer = 0;

    INT8U fICPRxOver = 0;
    INT8U fTransRxOver = 0;
    
    INT16U w1SCnt = 0;
    INT8U b1MinICPTimeoutCnt = BOOT_RESET_DELAY_TIME;
    INT8U bCommand[10];
        
    INT16U wTemp = 0;
    INT8U bRxData = 0;
    INT8U fResetEN = false;
    INT8U bResetDelay = 1;//delay 1s

    INT8U bICPCH = 0xFF;
    INT8U bTransCH = 0xFF;

//    INT8U bCANRxBuf[16];
    INT8U fCheckSend = false;
	
//    BootCANSecInfo stCANICPSecInfo;
//    BootCANSecInfo stCANTransSecInfo;
//    BootCANRxInfo stCANRxVarInfo;

    bTransCH = stTransUARTInfo.bTransTxCH;
    bICPCH = stUpdateUARTInfo.bICPTxCH;
    stUpdateUARTInfo.bAppICPTxCH = stUpdateUARTInfo.bICPTxCH;
	
//    M_BOOT_FLASH_INIT();//if jump to RAM, need to init FLASH again

    if(stUpdateUARTInfo.fAppUARTICP == cbUpdateFromICP)
    {
        stUpdateUARTInfo.fAppUARTICP = cbUpdateFromNull;
        
        //from APP ICP
        bCommand[0] = 0;
        bCommand[1] = 0;
        bCommand[2] = 0;
        bCommand[3] = 0;
        bCommand[4] = (INT8U)((UPDATA_FROM_RS485_PAGE_LEN & 0xFF00) >> 8);
        bCommand[5] = (INT8U)(UPDATA_FROM_RS485_PAGE_LEN & 0x00FF);
        bCommand[6] = 0;
        bCommand[7] = 0;
        
        if(!stUpdateUARTInfo.fTransparence)
        {
            //return start COOK command via UART0, sBootCombineICPCmd()
            wICPTxBufLen = ((pCombineICPCmd)BootUARTFunc(sBootCombineICPCmd))\
                             ((stUpdateUARTInfo.bRs485Addr),
                             cwICP_CMD_START_COOK_ADDR,
                             bCommand, 8, bBootICPTxBuf, true);

            //log_info("ICPCH%d\r\n",bICPCH);	

        }
        else
        {
            wTransTxBufLen = ((pCheckToTransparentCmd)BootUARTFunc(sCheckToTransparentCmd))\
                              (&stTransUARTInfo, &stUpdateUARTInfo, bBootTransTxBuf, bCommand, 8);

            //log_info("TranCH%d\r\n",bTransCH);	
    
        }
					
    }	
    
    //need to init flash again while jump to RAM
//    M_BOOT_FLASH_INIT();
//    stUpdateUARTInfo.fAppUARTICP    = cbUpdateFromICP;    //only uart updata
    for(;;)
    {
        /*********************************************************
        * UART ICP/Subset ICP analysis
        *********************************************************/
        if(fICPRxOver)
        {
            //sBootIcpCmdAnalysis();
            #if 0
            if(wICPRxBufLen){
                log_debug("rec");
                log_sector(bBootICPRxBuf,wICPRxBufLen);
            }
            #endif
            wTemp = ((pIcpCmdAnalysis)BootUARTFunc(sBootIcpCmdAnalysis))
                    (bBootICPRxBuf, wICPRxBufLen,
                     bCommand, &stUpdateUARTInfo,&stTransUARTInfo,
                     &stUpdatePackInfo, stBootDevTree);
            if(true == stUpdateUARTInfo.fCmdOK)
            {
                b1MinICPTimeoutCnt = BOOT_UART_DELAY_TIME;
                MACRO_IWDG_RELOD();
                //check if to transparent ICP cmd to subset
//                if((stUpdateUARTInfo.fBootStartUpdate) &&
//                   (stUpdateUARTInfo.fTransparence))
                if((stUpdateUARTInfo.fTransparence)&&\
                  ((cwICP_CMD_RX_ENABLE_ADDR != stUpdateUARTInfo.wCmdType)&&\
                    (cwICP_CMD_DEVICE_TREE_ADDR != stUpdateUARTInfo.wCmdType)&&\
                      (cwICP_CMD_QUERY_ADDR_ADDR != stUpdateUARTInfo.wCmdType)))
                {
                    wTemp = ((pCheckToTransparentCmd)BootUARTFunc(sCheckToTransparentCmd))
                            (&stTransUARTInfo, &stUpdateUARTInfo, bBootTransTxBuf, bBootICPRxBuf, wICPRxBufLen);
                    
                    if(0 == wTransTxBufLen)
                    {
                        wTransTxBufLen = wTemp;
 
                    }
                }


                      
                else
                {
                    if(wTemp)
                    {
                        wTemp = ((pCheckToResponseCmd)BootUARTFunc(sCheckToResponseCmd))
                                (&stTransUARTInfo, &stUpdateUARTInfo, bBootICPTxBuf, bCommand, wTemp,
                                stBootDevTree);
                    }
                    if(0 == wICPTxBufLen)
                    {
                        wICPTxBufLen = wTemp;
                        

                    }
                    
                }
            }
			
            wICPRxBufLen = 0;
            fICPRxOver = false;
        }		
        if(fTransRxOver)
        {
            //analysis subset ICP return
            wTemp = ((pSubsetICPCmdAnalysis)BootUARTFunc(sBootSubsetICPCmdAnalysis))
                    (bBootTransRxBuf, wTransRxBufLen, &stTransUARTInfo, &stUpdateUARTInfo, stBootDevTree);
            if(wTemp)
            {
                wTemp = ((pCheckToResponseCmd)BootUARTFunc(sCheckToResponseCmd))
                        (&stTransUARTInfo, &stUpdateUARTInfo, bBootICPTxBuf, bBootTransRxBuf + 6, wTemp,
                        stBootDevTree);
            }

            if(wTemp != 0)
            {
                b1MinICPTimeoutCnt = BOOT_UART_DELAY_TIME;
                MACRO_IWDG_RELOD();
            }
            
            if(0 == wICPTxBufLen)
            {
                wICPTxBufLen = wTemp;
            }
 
            wTransRxBufLen = 0;
            fTransRxOver = false;
        }	
		
        /*********************************************************
        * UART TX process
        *********************************************************/

        if(cbUART5 >= bICPCH)
        {
            if(wICPTxBufLen)
            {
                fCheckSend = true;
                ((pUARTTx)BootUARTFunc(sBootUARTTx))(bICPCH,bBootICPTxBuf,&wICPTxBufLen,&wICPTxCnt);
            }              
        }

        if(cbUART5 >= bTransCH)
        {    
            ((pUARTTx)BootUARTFunc(sBootUARTTx))(bTransCH,bBootTransTxBuf,&wTransTxBufLen,&wTransTxCnt);
        }

        //check if send over
        if((0 == wICPTxBufLen)&&(fCheckSend))
        {
            fCheckSend = false;
            //change baudrate after send command
            if(cwICP_CMD_SET_BAUDRATE_ADDR == stUpdateUARTInfo.wCmdType)
            {                    
                if(cbUART5 == bICPCH)
                {
                    if((stUpdateUARTInfo.bBootBaudrate) < cbSetBandRateMax)
                    {
                        macro_BaudRateSet\
                        (UART5,
                         ((INT32U*)(BootRodata(cwBootBaudRateTab)))\
                         [stUpdateUARTInfo.bBootBaudrate]);
                    }
                }                   
            }
            //update over, write app mask to flash and reboot MCU
            if((cwICP_CMD_FINISH_COOK_ADDR == stUpdateUARTInfo.wCmdType))
            {
                if(!(stUpdateUARTInfo.fTransparence))
                {
                    //write app mask data
                    (void)((pFLASH_Program)BootFlashFunc(sBootFLASH_Program))\
                    ((INT32U)sAppOkMark, bBootAppMaskBuff, cbAppMaskLen);
                }
                stUpdateUARTInfo.wCmdType = 0;
                //M_RESET_MCU();
                fResetEN = true;
            }
        }       		
        /*********************************************************
        * UART RX process
        *********************************************************/
        if(cbUART5 == bICPCH)
        {         
#if (true == fUART5Enable)
            if(RESET != M_UART_GET_ERROR(UART5)){
                M_UART_CLR_ERROR(UART5); 
            }
            if(M_BOOT_UART_RX_FULL_CHK(UART5))
            {
                bRxData = M_BOOT_UART_BYTE_READ(UART5);
                bBootICPRxBuf[wICPRxCnt] = bRxData;
                wICPRxCnt++;
                bICPReTimer = 0;

              //  printf("%x",bRxData);
            }

#endif  //#if (true == fUART2Enable)
        }
        if(cbUART5 == bTransCH)
        {         
#if (true == fUART5Enable)
            if(M_BOOT_UART_RXOVERFLOW_CHK(UART5))
            {
                M_BOOT_UART_CLR_RXOVERFLOW_FLAG(UART5);
            }
            if(M_BOOT_UART_FRAMERROR_CHK(UART5))
            {
                M_BOOT_UART_CLR_FRAMERROR_FLAG(UART5);
            }
            if(M_BOOT_UART_RX_FULL_CHK(UART5))
            {
                bRxData = M_BOOT_UART_BYTE_READ(UART5);
                bBootTransRxBuf[wTransRxCnt] = bRxData;
                wTransRxCnt++;
            //    bTransReTimer = 0;
                
            }
#endif  //#if (true == fUART5Enable)            
        }

		
        /*********************************************************
        * 500us timer
        *********************************************************/
        if(M_CHK_SYSTICK_FLAG())
        {
            //PER 500us
//            M_BOOT_FEEDDOG();
            /*********************************************************
            * ICP transparent process
            *********************************************************/            
            w1SCnt++;
            if(w1SCnt >= 2000)
            {
                w1SCnt = 0;
                
                if(0 == b1MinICPTimeoutCnt){  
                        //init boot updating flags
                    //if(false == stUpdateUARTInfo.fBootUARTUpdateReg){                       
                    (stUpdateUARTInfo.fBootStartUpdate) = false;                        
                    (stUpdatePackInfo.fMD5VerifyOK) = false;
                    (stUpdatePackInfo.fUpdateEN) = false;
                        MACRO_IWDG_RELOD();
                        if(cdwAPPMark == *((INT32U*)sAppOkMark)){//
                            //log_info("Dret\r\n");
                            stUpdateUARTInfo.fAppUARTICP = cbUpdateFinish;
                            M_RESET_MCU();
                        }else{                                   
    #if (true == HalBootSpiFlashFuncEN)//no main try flash
                            MACRO_SpiFlashFunc();
    #endif
                            //log_info("Eret\r\n");
                            stUpdateUARTInfo.fAppUARTICP = cbUpdateFinish;
                            M_RESET_MCU();                     //TRY RESET
                        }
                    //}else{
                    //    b1MinICPTimeoutCnt =
                    //}
                    //stUpdateUARTInfo.fBootUARTUpdateReg = false;
                       

                }else{
                        b1MinICPTimeoutCnt--;
                        //log_debug("UARTICPProcess1 %d\r\n",b1MinICPTimeoutCnt); 
                }
                    

                
                if(fResetEN)
                {
                    //Reset delay 2s
                    if(bResetDelay > 0)
                    {
                        bResetDelay--;
                    }
                    else
                    {
                        //if flash OK
                        //Mark: chenweijie 20191015.2
                        //log_info("boot uart reset\r\n");
                        fResetEN = false;
                        M_RESET_MCU();
                    }
                }
                else
                {
                    bResetDelay = 1;
                }

                
            }   
//            
//            //check data frame           
            if(cbUART5 >= bICPCH)
            {
                fICPRxOver = ((pUARTCheckDataFrame)BootUARTFunc(sBootUARTCheckDataFrame))\
                              (&bICPReTimer, &wICPRxCnt, stUpdateUARTInfo.bBootBaudrate); //stUpdateUARTInfo.bBootBaudrate);
                if(fICPRxOver)
                {
                    wICPRxBufLen = wICPRxCnt;
                    wICPRxCnt = 0;
                }     
           }
//            if(cbUART5 >= bTransCH)
//            {
//                fTransRxOver = ((pUARTCheckDataFrame)BootUARTFunc(sBootUARTCheckDataFrame))\
//                              (&bTransReTimer, &wTransRxCnt, stUpdateUARTInfo.bBootBaudrate);
//                if(fICPRxOver)
//                {
//                    wTransRxBufLen = wTransRxCnt;
//                    wTransRxCnt = 0;
//                }
//            }
            
				
        }

//        drv_log_handle();     

    }
}

/************************************************************************************************
* Input: void
*
* Output: void
*
* Logic discription: UART send
*
* run time: 
************************************************************************************************/
void sBootUARTTx(INT8U bUARTx, INT8U *pbBuffer, INT16U *wTxBufLen, INT16U *wTxCnt)
{
    if(*wTxBufLen != 0)
    {
        if(bUARTx == cbUART5)
        {
#if(true == HalBootUSART5RS485EN)
            HalBootUSart5RS485TxEnable();//enable rs485 TX
#endif  //#if(true == HalBootUSART5RS485EN)
        }

        
        if(bUARTx == cbUART5)
        {
#if(true == HalBootUSART5RS485EN)
            M_BOOT_UART_BYTE_WRITE(UART5, pbBuffer[*wTxCnt]);
            while(!M_BOOT_UART_TX_COMP_CHK(UART5)){;}
#endif  //#if(true == HalBootUSART5RS485EN)
        }
        
        (*wTxCnt)++;				

        if(bUARTx == cbUART5)
        {
#if(true == HalBootUSART5RS485EN)
            HalBootUSart5RS485RxEnable();//enable rs485 RX
#endif  //#if(true == HalBootUSART5RS485EN)
        }
        
        if(*wTxCnt >= *wTxBufLen)
        {
            #if 0
            log_debug("send");
            log_sector(pbBuffer,*wTxBufLen);
            #endif
    
        
            *wTxBufLen = 0;
            *wTxCnt = 0;

        }            
    }
}

/************************************************************************************************
* Input: void
*
* Output: void
*
* Logic discription: check UART data frame according to the time between two bytes.
*
* run time: 500us
************************************************************************************************/
INT8U sBootUARTCheckDataFrame(INT8U *pbReTimer, INT16U *pwRxCnt, INT8U bBandRate)
{
    
    if((*pwRxCnt) != 0)
    {            
        (*pbReTimer)++;
        
        //BootRodatax(wBootRxDelay_500us, dwOffsetAddr);
        if((*pbReTimer) >= ((INT16U*)BootRodata(wBootRxDelay_500us))[bBandRate])
        {
            (*pbReTimer) = 0;
    	    //log_debug("delaly %d,%d\r\n",((INT16U*)BootRodata(wBootRxDelay_500us))[bBandRate],((INT16U*)((INT32U)wBootRxDelay_500us + 0))[bBandRate]);
    	    return true;
        }
    }
    else
    {
        (*pbReTimer) = 0;
    }
    
    return false;
}

/*************************************************************************************
* Input: *bData: addr of data; bDataLen: bytes of data
*
* Output: Null
* 
* Logic discription: sending data VIA UART
*
* run time:
*************************************************************************************/
void sBootUARTSendCOM(USART_Module *pUART, INT8U *bData, INT16U wDataLen)
{
    INT16U i;
    
    //send command
    for(i = 0; i < wDataLen; i++)
    {
        
        M_BOOT_UART_BYTE_WRITE(pUART, bData[i]);
        while(!M_BOOT_UART_TX_COMP_CHK(pUART))
        {
            
        }
    }
}

static INT16U sBootCombineICPCmd(INT8U bRs485Addr, INT16U wCmdAddr, INT8U *pbData, INT16U wDataLen, INT8U *pbDataOut,INT8U fReturnCmd)
{
    INT16U i;
    INT16U wTemp = 0;
    //log_debug("CombineICPCmd\r\n");
    pbDataOut[0] = bRs485Addr;
    pbDataOut[1] = 0xFF;
    if(fReturnCmd)
    {
        wCmdAddr |= 0x8000;
    }
    pbDataOut[2] = (INT8U)((wCmdAddr & 0xFF00) >> 8);
    pbDataOut[3] = (INT8U)(wCmdAddr & 0x00FF);
    pbDataOut[4] = (INT8U)((wDataLen & 0xFF00) >> 8);
    pbDataOut[5] = (INT8U)(wDataLen & 0x00FF);

    if((cbMaxPackageData < wDataLen)||(wDataLen == 0))                        
    {
        return 0;
    }
    
    
    for(i = 0; i < wDataLen; i++)                        
    {
        pbDataOut[6 + i] = pbData[i];
    }
    
    i += 6;
    
    //original function:CRC16_2()
    //add CRC16
    wTemp = ((pCRC16_2)BootFlashFunc(CRC16_2))(pbDataOut, i);
    pbDataOut[i++] = (INT8U)(wTemp & 0x00FF);
    pbDataOut[i++] = (INT8U)((wTemp & 0xFF00) >> 8);

    return i;
}

/*
static void sBootMR32ICPProcess(INT8U *pbProcess, INT8U *pfHaveCmdReturn)
{
    
    switch(*pbProcess)
    {
        case cbCXCmd:
            if(*pfHaveCmdReturn)
            {
                *pfHaveCmdReturn = false;
                
                
            }
            else
            {
                if(*pbSendCnt < 3)
                {
                    //send "CX\r" to read CS's firmware version
                    bBootUART2Buf[0] = 'C';
                    bBootUART2Buf[1] = 'X';
                    bBootUART2Buf[2] = '\r';
                    wUART2TxBufLen = 3;
                }
                else
                {
                    
                }
            }
            break;
            
        case cbTigerCmd:
            
            break;
            
        case cbTransFirmData:
            break;
            
        case cbICPOver:
            break;
    }
}
*/

void sBootDeviceTreeInit(INT8U bCommuAddr)
{
    INT8U i;
    INT8U j;
    
    //fill device tree
    
    //Serial_Num
    //read from eeprom
    //device ID
    (stBootDevTree[0].bCommuAddr) = bCommuAddr;

    (stBootDevTree[0].bDevID)[0] = (INT8U)((cwDeviceType & 0xFF00) >> 8);
    (stBootDevTree[0].bDevID)[1] = (INT8U)(cwDeviceType & 0x00FF);
    (stBootDevTree[0].bDevID)[2] = 0x00;
    
    //firmware string
    j = 0;
    //firmware type
    for(i = 0; i < cbFirmwareTypeStrLen; i++)
    {
        (stBootDevTree[0].bFirmwareVer)[j++] = ((INT8U*)BootRodata(bFirmwareTypeStr))[i];
    }
	
    for(i = 0; i < cbFirmwareVersionLen; i++)
    {
        (stBootDevTree[0].bFirmwareVer)[j++] = ((INT8U*)BootRodata(bFrimwareVerStr))[i];
    }
	
    for(i = 0; i < cbBootFirmwareStrLen; i++)
    {
        (stBootDevTree[0].bFirmwareVer)[j++] = ((INT8U*)BootRodata(bBootFirmwareStr))[i];
    }

    //rest of firmware string filled of 0
    while(j < cbFirmwareVerAllLen)
    {
        (stBootDevTree[0].bFirmwareVer)[j++] = 0;
    }
    
    //number of subsets
//    (stBootDevTree[0].bDevSubsetNum) = 0;
	
}
/*************************************************************************************
* Input: pbDevTreeInfo: point of device tree; pbDevTreeInfoIn: Input point of device tree;
*
* Output: true: information in device tree is the same as the one from (pbDevTreeInfo)
* 
* Logic discription: compare the information of device tree and (from pbDevTreeInfo)
*
* run time: 
*************************************************************************************/
INT8U sBootCmpDevTreeInfo(DevTreeInfo *pbDevTreeInfo, DevTreeInfo *pbDevTreeInfoIn,UARTTransInfo *pstTransInfo)
{
    INT8U i;
    INT8U fCmpFail = false;
                    
    if((pbDevTreeInfo->bCommuAddr == pbDevTreeInfoIn->bCommuAddr)&&(pbDevTreeInfo->bCommuCH == pstTransInfo->bTransTxCH))
    {
        //compare device type ID (first 2 bytes of device ID)
        for(i = 0; i < 2; i++)
        {
            if((pbDevTreeInfo->bDevID)[i] != (pbDevTreeInfoIn->bDevID)[i]) 
            {
                fCmpFail = true;
                break;
            }
            else
            {
                fCmpFail = false;
            }
        }
        
        //compare firmware 
        for(i = 0; i < cbFirmwareVerAllLen; i++)
        {
            if(((pbDevTreeInfo->bFirmwareVer)[i] !=\
                (pbDevTreeInfoIn->bFirmwareVer)[i]) ||
               fCmpFail)
            {
                fCmpFail = true;
                break;
            }
            else
            {
                fCmpFail = false;
            }
        }
        
        if(!fCmpFail)
        {
            if(pbDevTreeInfo->bDevSubsetNum != pbDevTreeInfoIn->bDevSubsetNum)
            {
                return 1;//变化重建
            }
        }
        else
        {
            return 1;
        }
		
        return 0;//无变化无需处理
    }
    return 2;//新增树数据
}
/************************************************************************************************
* Input: pDevTree: point of device tree; 
*
* Output: 
*
* Logic discription: get subset device tree information
*
* run time:
************************************************************************************************/
static void sBootBulidDevTree(INT8U *pbDataIn, 
                              INT16U wDataLen,
                              DevTreeInfo *pDevTree,
                              UARTTransInfo *pstTransInfo)
{	
    INT8U bDevNum;
    INT8U i,j;
    INT8U bSameDevID = 0;
    DevTreeInfo *pDevTreeTempIn;
    DevTreeInfo *pDevTreeTemp;
    
    if((wDataLen % cbDevTreeInfoLen) != 0)
    {
        return;
    }
    bDevNum = 0;
    while(wDataLen >= cbDevTreeInfoLen)
    {
        wDataLen -= cbDevTreeInfoLen;
        bDevNum++;
    }
//    (pDevTree->bCommuCH)子设备总数(包含子设备的子设备)
    if(((pDevTree->bCommuCH) + bDevNum) <= cbMaxBootSubsets)
    {
        //check tree                                     
        for(i = 0; i < bDevNum; i++)
        {
            for(j = 0; j < ((pDevTree->bCommuCH) + 1); j++)
            {
                pDevTreeTempIn = (DevTreeInfo *)(&pbDataIn[6 + (i * cbDevTreeInfoLen)]);
                pDevTreeTemp = (DevTreeInfo*)((INT32U)pDevTree + (j * cbDevTreeInfoStLen));
                if(0 == sBootCmpDevTreeInfo(pDevTreeTemp,pDevTreeTempIn,pstTransInfo))
                {
                    if(i == (bDevNum - 1))
                    {
                        return;          
                    }
                    break;
                }            
                else if(1 == sBootCmpDevTreeInfo(pDevTreeTemp,pDevTreeTempIn,pstTransInfo))
                {
                    (pDevTree->bDevSubsetNum) = 0;
                    (pDevTree->bCommuCH) = 0xFF;
                    return;
                }            
            }
        }      
		
        (pDevTree->bDevSubsetNum)++;
		
        for(i = 0; i < bDevNum; i++)
        {
            pDevTreeTempIn = (DevTreeInfo *)(&pbDataIn[6 + (i * cbDevTreeInfoLen)]);

            //copy device ID
            if(i == 0)
            {
                if(((pstTransInfo->bDevID)[0] == (pDevTreeTempIn->bDevID)[0])&&((pstTransInfo->bDevID)[1] == (pDevTreeTempIn->bDevID)[1]))
                {
                    bSameDevID = (pstTransInfo->bDevID)[2];
                }
            }
            else
            {
                bSameDevID = 0;
            }
 
            pDevTreeTemp = (DevTreeInfo*)((INT32U)pDevTree + ((i + ((pDevTree->bCommuCH) + 1)) * cbDevTreeInfoStLen));

            (pDevTreeTemp->bCommuCH) = (pstTransInfo->bTransTxCH);
            //copy communition address                                
            (pDevTreeTemp->bCommuAddr) = (pDevTreeTempIn->bCommuAddr);

            //copy device ID
            if(bSameDevID)
            {
                (pDevTreeTemp->bDevID)[0] = (pDevTreeTempIn->bDevID)[0];            
                (pDevTreeTemp->bDevID)[1] = (pDevTreeTempIn->bDevID)[1];            
                (pDevTreeTemp->bDevID)[2] = bSameDevID;            
            }
            else
            {
                for(j = 0; j < cbDevIDLen; j++)
                {
                    (pDevTreeTemp->bDevID)[j] = (pDevTreeTempIn->bDevID)[j];
                }
            }
			
            //copy firmware version string
            for(j = 0; j < cbFirmwareVerAllLen; j++)
            {
                (pDevTreeTemp->bFirmwareVer)[j] = (pDevTreeTempIn->bFirmwareVer)[j];
            }
            //copy subset number                                
            (pDevTreeTemp->bDevSubsetNum) = (pDevTreeTempIn->bDevSubsetNum);
        }  
		
        (pDevTree->bCommuCH) += bDevNum;
		
    }				
}

/************************************************************************************************
* Input: void
*
* Output: AnalyReturn: return data length, analysis command type
*
* Logic discription: bootloader ICP analysis
*
* run time: 
************************************************************************************************/
INT16U sBootIcpCmdAnalysis(INT8U *pbDataIn, INT16U wDataInLen, INT8U *pbDataOut,
                           UpdateUARTInfo *pstUARTInfo,
                           UARTTransInfo *pstTransInfo,
                           UpdatePackInfo *pstPackInfo,
                           DevTreeInfo *pstDevTreeInfo)
{
    INT16U wCommandAddr = 0;
    INT16U wCommandDataLen = 0;
    INT16U wTemp = 0;
    INT16U i;
    INT8U fEnableToUpdate = false;
    INT8U fTransparenceTemp = false;
    INT16U wErrorCode = cwFlashOperateOK;

    INT8U bIndex = 0;
    INT8U fSameDevID = false;
    INT8U fSameMCUType = false;
    INT8U fSameFirmType = false;
    INT8U fSameFirmAll = false;
    INT8U bCompVar[5] = {0,};
    FirmwareDataInfo *pstFirmDataPackage;
	
    INT16U wResponceLen = 0;
 
 	
    (pstUARTInfo->fCmdOK) = false;
    
    //original function:CRC16_2()
    if(0 == ((pCRC16_2)BootFlashFunc(CRC16_2))(pbDataIn, wDataInLen))
    {
        //0xFF-updating command type
        if(((pbDataIn[0] == (pstUARTInfo->bRs485Addr)) || (pbDataIn[0] == 0xF7)) && (pbDataIn[1] == 0xFF))
        {
            wCommandAddr = (INT16U)((pbDataIn[2] << 8) + pbDataIn[3]);
            wCommandDataLen = (INT16U)((pbDataIn[4] << 8) + pbDataIn[5]);
            (pstUARTInfo->wCmdType) = wCommandAddr;
            
            //printf("rcmd%x\r\n",wCommandAddr);
            
            switch(wCommandAddr)
            {
                case cwICP_CMD_RX_DISABLE_ADDR://code:0x00 disable tranfer
                    if(2 == wCommandDataLen)
                    {
                        if((0 == pbDataIn[6]) && (1 == pbDataIn[7]))
                        {                            
                            (pstUARTInfo->fBootUARTUpdateTxDisable) = true;
                            (pstUARTInfo->fCmdOK) = true;							
                        }
                    }
                    break;

                case cwICP_CMD_RX_ENABLE_ADDR://code:0x01 enable tranfer
                    if(2 == wCommandDataLen)
                    {
                        if((0 == pbDataIn[6]) && (1 == pbDataIn[7]))
                        {
                            (pstUARTInfo->fBootUARTUpdateTxDisable) = false;
                            (pstUARTInfo->fCmdOK) = true;
                            
                            (pstUARTInfo->fQueryAddrOK) = false;	
                            (pstTransInfo->bSubRs485Addr) = 0xF7;							
                            if(pbDataIn[0] != 0xF7)
                            {
                                //return command
                                pbDataOut[0] = 0;
                                pbDataOut[1] = 0;
                                
                                wResponceLen = 2;                               
                            }
                        }
                    }
                    break;
                                        
                case cwICP_CMD_DEVICE_TREE_ADDR://code:0x03
                    //get device tree
                    if(2 == wCommandDataLen)
                    {
                        if((0 == pbDataIn[6]) && (1 == pbDataIn[7]))
                        {
                            (pstUARTInfo->fCmdOK) = true;
//                            (pstDevTreeInfo->bDevSubsetNum) = 0;
//                            (pstUARTInfo->bSubDevTreeNum) = 0;
							
                            if( (!(pstUARTInfo->fBootUARTUpdateTxDisable))&&\
                              (pbDataIn[0] != 0xF7))
                            {
                                wResponceLen = cbDevTreeInfoLen;
                            }
                        }
                    }
                    break;
                    
                case cwICP_CMD_START_COOK_ADDR://code:0x04
                    //start updating					
                    if(67 <= wCommandDataLen)
                    {
                        BootFirmCompareInfo *pstCompVar;
                        pstCompVar = (BootFirmCompareInfo *)(&bCompVar[0]);                        
                        bIndex = (pbDataIn[7] - 1) * 3;
//                        if (!(pstUARTInfo->fRebuildDevTree))
                        if (pbDataIn[0] != 0xF7)
                        {
                            //update type
                            (pstUARTInfo->fCmdOK) = true;
                            if(1 == pbDataIn[6])
                            {
                                if(((pstUARTInfo->bBootUpdateType) != cbAdvUpdating) &&
                                   ((pstUARTInfo->bBootUpdateType) != cbForceUpdating))
                                {
                                    //if it is advance updating or force updating recently,
                                    //it is not able to normal updating
                                    fEnableToUpdate = true;
                                    (pstUARTInfo->bBootUpdateType) = cbNormalUpdating;
                                }
                            }
                            else if(2 == pbDataIn[6])
                            {
                                if((pstUARTInfo->bBootUpdateType) != cbForceUpdating)
                                {
                                    //if it is force updating recently,
                                    //it is not able to advance updating
                                    fEnableToUpdate = true;
                                    (pstUARTInfo->bBootUpdateType) = cbAdvUpdating;
                                }
                            }
                            else if(3 == pbDataIn[6])
                            {
                                fEnableToUpdate = true;
                                (pstUARTInfo->bBootUpdateType) = cbForceUpdating;
                            }

                            //compare device id
                            pstCompVar->bRoute = pbDataIn[7];
                            pstCompVar->bTransCH = 0xFF;
                            if(true == ((pChkSameDevID)BootFlashFunc(sBootChkSameDevID))
                                        (pstDevTreeInfo, &pbDataIn[8], pstCompVar))                                                       
                            {
                                fSameDevID = true;
                                //compare MCU type
                                if(true == ((pChkSameMCU)BootFlashFunc(sBootChkSameMCU))
                                            ((DevTreeInfo*)((INT32U)pstDevTreeInfo + (pstCompVar->bRecursionCnt - 1) * cbDevTreeInfoStLen), &pbDataIn[11 + bIndex]))                                                       
                                {
                                    fSameMCUType = true;
                                }                                   

                                //compare Firmware Type
                                if(true == ((pMatchFirmwareType)BootFlashFunc(sBootMatchFirmwareType))
                                            ((DevTreeInfo*)((INT32U)pstDevTreeInfo + (pstCompVar->bRecursionCnt - 1) * cbDevTreeInfoStLen), &pbDataIn[11 + bIndex]))                                                       
                                {
                                    fSameFirmType = true;
                                } 
								
                                //compare Firmware Type and boot version
                                if(true == ((pMatchFirmware)BootFlashFunc(sBootMatchFirmware))
                                            ((DevTreeInfo*)((INT32U)pstDevTreeInfo + (pstCompVar->bRecursionCnt - 1) * cbDevTreeInfoStLen), &pbDataIn[11 + bIndex]))                                                       
                                {
                                    fSameFirmAll = true;
                                }  
                                
                            }   
			
                            if(fEnableToUpdate && fSameDevID)
                            {
                                (pstTransInfo->wTransLenth) = 0;
                                //force update no need to check firmwareID
                                if(cbForceUpdating == (pstUARTInfo->bBootUpdateType))
                                {
                                    //from the firmware wanted to be updated
                                    if(true == fSameMCUType)
                                    {
                                        (pstUARTInfo->fBootStartUpdate) = true;   
                                        (pstTransInfo->bSubRs485Addr) = pstCompVar->bCommuAddr;	
                                        (pstTransInfo->bTransTxCH) = pstCompVar->bTransCH;	
                                        
                                        if(1 < pbDataIn[7])
                                        {                                                                                        
                                            fTransparenceTemp = true;
                                            //copy start cook version info
                                            (pstTransInfo->bTransStartCmdInfo)[0] = pbDataIn[6];
                                            (pstTransInfo->bTransStartCmdInfo)[1] = pbDataIn[7] - 1;//delete own level
                                            for(i = 0; i < (wCommandDataLen - 3 - 2); i++)
                                            {
                                                (pstTransInfo->bTransStartCmdInfo)[i + 2] = pbDataIn[i + 11];
                                            }
                                            if((pstTransInfo->bTransStartCmdInfo)[4] > 0)
                                            {
                                                (pstTransInfo->bTransStartCmdInfo)[4] = 0;
                                            }											
                                            (pstTransInfo->wTransLenth) = wDataInLen - 8 - 3;	
                                        }
                                    }
                                    else
                                    {
                                        (pstUARTInfo->fBootStartUpdate) = false;
                                        wErrorCode = cwCantChgUpdateDev;
                                    }
                                }
                                else if(cbAdvUpdating == (pstUARTInfo->bBootUpdateType))
                                {
                                    //check firmware type
                                    if(true == fSameFirmType)
                                    {
                                        (pstUARTInfo->fBootStartUpdate) = true;                                             
                                        (pstTransInfo->bSubRs485Addr) = pstCompVar->bCommuAddr;		
                                        (pstTransInfo->bTransTxCH) = pstCompVar->bTransCH;	
                                        
                                        if(1 < pbDataIn[7])
                                        {
                                            fTransparenceTemp = true;
                                                                                        
                                            //copy start cook command info
                                            (pstTransInfo->bTransStartCmdInfo)[0] = pbDataIn[6];
                                            (pstTransInfo->bTransStartCmdInfo)[1] = pbDataIn[7] - 1;//delete own level
                                            for(i = 0; i < (wCommandDataLen - 3 - 2); i++)
                                            {
                                                (pstTransInfo->bTransStartCmdInfo)[i + 2] = pbDataIn[i + 11];
                                            }
                                            if((pstTransInfo->bTransStartCmdInfo)[4] > 0)
                                            {
                                                (pstTransInfo->bTransStartCmdInfo)[4] = 0;
                                            }											
                                            (pstTransInfo->wTransLenth) = wDataInLen - 8 - 3;
                                        }
                                    }
                                    else
                                    {
                                        (pstUARTInfo->fBootStartUpdate) = false;
                                        wErrorCode = cwCantChgUpdateDev;
                                    }
                                }
                                else
                                {
                                    //check firmware version
                                    if(true == fSameFirmAll)
                                    {
                                        (pstUARTInfo->fBootStartUpdate) = true;                                             
                                        (pstTransInfo->bSubRs485Addr) = pstCompVar->bCommuAddr;
                                        (pstTransInfo->bTransTxCH) = pstCompVar->bTransCH;	
                                        
                                        if(1 < pbDataIn[7])
                                        {
                                            fTransparenceTemp = true;
                                                                                        
                                            //copy start cook command info
                                            (pstTransInfo->bTransStartCmdInfo)[0] = pbDataIn[6];
                                            (pstTransInfo->bTransStartCmdInfo)[1] = pbDataIn[7] - 1;//delete own level
                                            for(i = 0; i < (wCommandDataLen - 3 - 2); i++)
                                            {
                                                (pstTransInfo->bTransStartCmdInfo)[i + 2] = pbDataIn[i + 11];
                                            }
                                            if((pstTransInfo->bTransStartCmdInfo)[4] > 0)
                                            {
                                                (pstTransInfo->bTransStartCmdInfo)[4] = 0;
                                            }											
                                            (pstTransInfo->wTransLenth) = wDataInLen - 8 - 3;	
                                        }
                                    }
                                    else
                                    {
                                        (pstUARTInfo->fBootStartUpdate) = false;
                                        wErrorCode = cwCantChgUpdateDev;
                                    }
                                }
                            }
                            else
                            {
                                (pstUARTInfo->fBootStartUpdate) = false;
                                wErrorCode = cwCantChgUpdateDev;
                            }

                            if(pstUARTInfo->fBootStartUpdate)
                            {
                                (pstPackInfo->fHaveRecPackage) = false;     
                            }
							                                
#if (false == cfChangeUpdateDevice)
                            //disable to change updating device
                            //Mark: chenweijie 20191015.3
                            if((pstPackInfo->fHaveRecPackage) &&
                               (pstUARTInfo->fTransparence) != fTransparenceTemp)
                            {
                                wErrorCode = cwCantChgUpdateDev;
                            }
                            else
                            {
                                (pstUARTInfo->fTransparence) = fTransparenceTemp;
                            }
#else
                            (pstUARTInfo->fTransparence) = fTransparenceTemp;
#endif  //#if (false == cfChangeUpdateDevice)
                                
                            if(!(pstUARTInfo->fTransparence))
                            {

                                //get firmware start/end address
                                (pstPackInfo->dwFirmStartAddr) = (INT32U)((pbDataIn[21 + bIndex] << 24) +\
                                                         (pbDataIn[22 + bIndex] << 16) +\
                                                         (pbDataIn[23 + bIndex] << 8) +\
                                                         (pbDataIn[24 + bIndex]));

                                (pstPackInfo->dwFirmEndAddr) = (INT32U)((pbDataIn[25 + bIndex] << 24) +\
                                                         (pbDataIn[26 + bIndex] << 16) +\
                                                         (pbDataIn[27 + bIndex] << 8) +\
                                                         (pbDataIn[28 + bIndex]));

                                //convert 4 bytes keys from 12 bytes key data
                                (pstPackInfo->bKEY)[0] = ((pKEYConvert)BootFlashFunc(sKEYConvert))\
                                                         (0, &pbDataIn[29 + bIndex]);
                                (pstPackInfo->bKEY)[1] = ((pKEYConvert)BootFlashFunc(sKEYConvert))\
                                                         (1, &pbDataIn[29 + bIndex]);
                                (pstPackInfo->bKEY)[2] = ((pKEYConvert)BootFlashFunc(sKEYConvert))\
                                                         (2, &pbDataIn[29 + bIndex]);
                                (pstPackInfo->bKEY)[3] = ((pKEYConvert)BootFlashFunc(sKEYConvert))\
                                                         (3, &pbDataIn[29 + bIndex]);

                                if((cbForceUpdating == (pstUARTInfo->bBootUpdateType))||(cbAdvUpdating == (pstUARTInfo->bBootUpdateType)))
                                {
                                    //check Address
                                    if((pstPackInfo->dwFirmStartAddr) != cdwMCU_VECTOR_SECTOR_START)
                                    {
                                        wErrorCode = cwUpdateAddrError;
                                    }
                                }
                                else
                                {
                                    //check Address
                                    if((pstPackInfo->dwFirmStartAddr) != cdwAPP_SECTOR_START)
                                    {
                                        wErrorCode = cwUpdateAddrError;
                                    }
                                }								

                                for(i = 0; i < cbFirmUUIDLen; i++)
                                {
                                    //check MD5
                                    if((pstPackInfo->bFileMD5)[i] != pbDataIn[41 + bIndex + i])
                                    {
                                        break;
                                    }
                                }
                                
                                if(i < cbFirmUUIDLen)
                                {
                                    //save current MD5
                                    for(i = 0; i < cbFirmUUIDLen; i++)
                                    {
                                        (pstPackInfo->bFileMD5)[i] = pbDataIn[41 + bIndex + i];
                                    }
                                    
                                    (pstPackInfo->fMD5VerifyOK) = false;
                                    (pstPackInfo->dwCurrentFirmAddr) = 0;
                                }
                                else
                                {
                                    //check if MD5 is null
                                    for(i = 0; i < cbFirmUUIDLen; i++)
                                    {
                                        //check MD5
                                        if((pstPackInfo->bFileMD5)[i] != 0)
                                        {
                                            break;
                                        }
                                    }
                                    
                                    if(i < cbFirmUUIDLen)
                                    {
                                        //MD5 verify OK, enable breakpoint resume
                                        (pstPackInfo->fMD5VerifyOK) = true;
                                    }
                                    else
                                    {
                                        //MD5 is all 0, cannot be verified by it
                                        (pstPackInfo->fMD5VerifyOK) = false;
                                    }
                                    
                                }
                                //return command
                                pbDataOut[0] = (INT8U)(((pstPackInfo->dwCurrentFirmAddr) & 0xFF000000) >> 24);
                                pbDataOut[1] = (INT8U)(((pstPackInfo->dwCurrentFirmAddr) & 0x00FF0000) >> 16);
                                pbDataOut[2] = (INT8U)(((pstPackInfo->dwCurrentFirmAddr) & 0x0000FF00) >> 8);
                                pbDataOut[3] = (INT8U)((pstPackInfo->dwCurrentFirmAddr) & 0x000000FF);

                                pbDataOut[4] = (INT8U)((UPDATA_FROM_RS485_PAGE_LEN & 0xFF00) >> 8);
                                pbDataOut[5] = (INT8U)(UPDATA_FROM_RS485_PAGE_LEN & 0x00FF);

                                pbDataOut[6] = (INT8U)((wErrorCode & 0xFF00) >> 8);
                                pbDataOut[7] = (INT8U)(wErrorCode & 0x00FF);     
								
                                wResponceLen = 8;
                            }
							
                            if(wErrorCode != cwFlashOperateOK)
                            {
                                (pstUARTInfo->fBootStartUpdate) = false;
                                (pstUARTInfo->fTransparence) = false;
								
                                pbDataOut[0] = 0;
                                pbDataOut[1] = 0;
                                pbDataOut[2] = 0;
                                pbDataOut[3] = 0;
                                pbDataOut[4] = 0;
                                pbDataOut[5] = 0;
                                pbDataOut[6] = (INT8U)((wErrorCode & 0xFF00) >> 8);
                                pbDataOut[7] = (INT8U)(wErrorCode & 0x00FF);     
								
                                wResponceLen = 8;
                            }
                        }
                    }
                    break;
                    
                case cwICP_CMD_DATA_TRF_ADDR://code:0x05
                    if(pstUARTInfo->fBootStartUpdate)
                    {
                        (pstUARTInfo->fCmdOK) = true;
                        (pstUARTInfo->fBootUARTUpdateReg) = true;					
                        //get firmware packages
                        pstFirmDataPackage = (FirmwareDataInfo *)(&pbDataIn[4]);                        
                        
                        if(pstUARTInfo->fTransparence)
                        {
                            //unvarnished transmission updating
                            wTemp = cwWaitingFlash;
                        }
                        else if( cbUpdateFromapp == pstUARTInfo->fAppUARTICP){
                             wTemp =   cwFlashOperateOK;//out handle
                        }else{
                            //check if MD5 verify OK
                            if(!(pstPackInfo->fHaveRecPackage))
                            {
                                (pstPackInfo->fHaveRecPackage) = true;
                                (pstPackInfo->wLastFirmLen) = 0;

                                if(!(pstPackInfo->fMD5VerifyOK))
                                {
                                    //if not verify file MD5, reset firmware number
                                    //transparent firmware updating no support to check MD5
                                    (pstPackInfo->dwCurrentFirmAddr) = 0;
                                    (pstPackInfo->fUpdateEN) = true;
                                    (pstPackInfo->fHaveErase) = false;
                                }
                                else
                                {
                                    (pstPackInfo->fMD5VerifyOK) = false;
                                    (pstPackInfo->fUpdateEN) = true;
                                    //(pstPackInfo->fHaveErase) = true;
                                    
                                }
                            }                          

                          
                            
                                //Single MCU updating
                                //return error code
                                //original function:sFlashFirmPackOperate()
                            wTemp = ((pFlashFirmPackOperate)BootFlashFunc(sFlashFirmPackOperate))
                                    (pstFirmDataPackage,
                                     pstPackInfo,
                                     (pstUARTInfo->bBootUpdateType));

                        }
                        
                        
                        
                        
                        if((!(pstUARTInfo->fBootUARTUpdateTxDisable)) &&
                           (wTemp != cwWaitingFlash) )
                        {
                            if(wTemp != cwFlashOperateOK)
                            {
                                for(i = 0; i < cbFirmUUIDLen; i++)
                                {
                                    //clear MD5 buffer while flash error, in order to 
                                    //erase flash again
                                    (pstPackInfo->bFileMD5)[i] = 0;
                                }
                            }
                            
                            //return command
                            pbDataOut[0] = (pstFirmDataPackage->bFirmwareAddr)[0];
                            pbDataOut[1] = (pstFirmDataPackage->bFirmwareAddr)[1];
                            pbDataOut[2] = (pstFirmDataPackage->bFirmwareAddr)[2];
                            pbDataOut[3] = (pstFirmDataPackage->bFirmwareAddr)[3];
                            pbDataOut[4] = (INT8U)((wTemp & 0xFF00) >> 8);
                            pbDataOut[5] = (INT8U)(wTemp & 0x00FF);//error code
                            
                            wResponceLen = 6;
                        }
                        else{
                            //printf("txdisable");
                        }
                    }
                    else{
                            //printf("state error");
                    }
                    break;
                    
                case cwICP_CMD_FINISH_COOK_ADDR://code:0x06
                    //end of ICP
                    if(2 == wCommandDataLen)
                    {
                        if((0xAB == pbDataIn[6]) && (0xAB == pbDataIn[7]))
                        {
                            (pstUARTInfo->fCmdOK) = true;
                            
                            if((!(pstUARTInfo->fBootUARTUpdateTxDisable)) &&
                               (!(pstUARTInfo->fTransparence)))
                            {
                                if(((pstPackInfo->dwCurrentFirmAddr) + (pstPackInfo->dwFirmStartAddr) + \
                                  (pstPackInfo->wLastFirmLen)) == \
                                    (pstPackInfo->dwFirmEndAddr) + 1)
                                {
                                    //return command
                                    pbDataOut[0] = 0;
                                    pbDataOut[1] = 0;                                    
                                }
                                else
                                {
                                    //return command
                                    pbDataOut[0] = (INT8U)((cwOtherError & 0xFF00) >> 8);
                                    pbDataOut[1] = (INT8U)(cwOtherError & 0x00FF);
                                }
                                    
                                wResponceLen = 2;
                            }
                            
                        }
                        
                    }
                    break;
                    
                case cwICP_CMD_QUERY_ADDR_ADDR://code:0x07 ask for the address
                    //get device 485 address
                    if(2 == wCommandDataLen)
                    {
//                        if(pstUARTInfo->bAppICPTxCH <= cbUART2)
                        if(pstUARTInfo->bAppICPTxCH <= cbUART5)
                        {
                            if((0 == pbDataIn[6]) && (1 == pbDataIn[7]))
                            {
                                (pstUARTInfo->fCmdOK) = true;
                                if(!(pstUARTInfo->fBootUARTUpdateTxDisable))
                                {
                                    //return command
                                    pbDataOut[0] = 0;
                                    pbDataOut[1] = (pstUARTInfo->bRs485Addr);
                                    
                                    wResponceLen = 2;
                                }
                            }
                        }
                        else
                        {
                            if((0 == pbDataIn[6]) && (1 == pbDataIn[7])&& (pbDataIn[0] == 0xF7)&&((pstUARTInfo->fQueryAddrOK) == false))
                            {
                                (pstUARTInfo->fCmdOK) = true;
                                pbDataOut[0] = 0;
                                pbDataOut[1] = (pstUARTInfo->bRs485Addr);
								                                
                                wResponceLen = 2;
                            }						
                            else if((0 == pbDataIn[6]) && (1 == pbDataIn[7])&&(pbDataIn[0] == (pstUARTInfo->bRs485Addr)))
                            {
                                (pstUARTInfo->fQueryAddrOK) = true;																
                            }
                        }
                    }
                    break;
                    
                case cwICP_CMD_SET_BAUDRATE_ADDR://code:0x08 set baudrate
                    //change baudrate
                    if(2 == wCommandDataLen)
                    {
                        if(0 == pbDataIn[6])
                        {
                            if(pbDataIn[7] < cbSetBandRateMax)
                            {
                                (pstUARTInfo->fCmdOK) = true;
                                (pstUARTInfo->bBootBaudrate) = pbDataIn[7];
                            }
                        }
                        
                        if((!(pstUARTInfo->fBootUARTUpdateTxDisable)) && (pstUARTInfo->fCmdOK))
                        {
                            //return command
                            pbDataOut[0] = 0;
                            pbDataOut[1] = 0;
                            
                            wResponceLen = 2;
                        }
                    }
                    break;
                    
                default:
                    break;
            }
          }
//        else{
//            printf("ERR F\r\n");
//        }
        
    }
//    else{
//        printf("ERR C\r\n");
//    }
    return wResponceLen;
}

/************************************************************************************************
* Input: pbDataIn: point of data in; wDataInLen: data in length; point of data out;
*        pUARTTransInfo: point of UART transparent info;pstDevTreeInfo: point of device tree;
*
* Output: data out length
*
* Logic discription: analysis data from transparent site
*
* run time:
************************************************************************************************/
INT16U sBootSubsetICPCmdAnalysis(INT8U *pbDataIn, 
                                 INT16U wDataInLen,
                                 UARTTransInfo *pstTransInfo,
                                 UpdateUARTInfo *pstUARTInfo,
                                 DevTreeInfo *pstDevTreeInfo)
{
    INT16U wCommandAddr;
    INT16U wCommandDataLen;
    
    //original function:CRC16_2()
    (pstTransInfo->fCmdOK) = false;
    if(0 == ((pCRC16_2)BootFlashFunc(CRC16_2))(pbDataIn, wDataInLen))
    {        
        wCommandAddr = (INT16U)((pbDataIn[2] << 8) + pbDataIn[3]);
        wCommandDataLen = (INT16U)((pbDataIn[4] << 8) + pbDataIn[5]);     
        pstTransInfo->wSubCmdType = wCommandAddr;     

        if(pbDataIn[0] == (pstTransInfo->bSubRs485Addr))
        {           
            switch(wCommandAddr)
            {                    
                case (cwICP_CMD_RX_ENABLE_ADDR + 0x8000):
                    if(((pstTransInfo->wMasterCmdType + 0x8000) == wCommandAddr) &&
                       (2 == wCommandDataLen))
                    {
                        if((0 == pbDataIn[6]) && (0 == pbDataIn[7]))
                        {                            
                            (pstTransInfo->fCmdOK) = true;
                            return (wDataInLen - 8);
							
                        }
                    }
                    break;
                                        
                case (cwICP_CMD_DEVICE_TREE_ADDR + 0x8000):
                    //get device tree
                    //check number of subsets					
                    if(((pstTransInfo->wMasterCmdType + 0x8000) == wCommandAddr) &&
                    (15 <= wCommandDataLen))
                    {
                        //copy device tree
                        ((pBulidDevTree)BootUARTFunc(sBootBulidDevTree))
                          ((pbDataIn), 
                          (wCommandDataLen), 
                          (pstDevTreeInfo),
                          (pstTransInfo));
						
                        (pstTransInfo->fCmdOK) = true;
                        return (wDataInLen - 8);
						
                    }
                    break;
                    
                case (cwICP_CMD_START_COOK_ADDR + 0x8000):
                    //start updating
                    if(((pstTransInfo->wMasterCmdType + 0x8000) == wCommandAddr) &&
                       (8 == wCommandDataLen))
                    {
                        if((0 == pbDataIn[12]) && (0 == pbDataIn[13]))
                        {
                            (pstTransInfo->fCmdOK) = true;   
                            return (wDataInLen - 8);
							
                        }
                    }
                    break;
                    
                case (cwICP_CMD_DATA_TRF_ADDR + 0x8000):
                    if(((pstTransInfo->wMasterCmdType+0x8000) == wCommandAddr) &&
                       (6 == wCommandDataLen))
                    {
                        if((0 == pbDataIn[10]) && (0 == pbDataIn[11]))
                        {
                            (pstTransInfo->fCmdOK) = true;                            
                            return (wDataInLen - 8);
                        }
                    }
                    break;
                    
                case (cwICP_CMD_FINISH_COOK_ADDR + 0x8000):
                case (cwICP_CMD_SET_BAUDRATE_ADDR + 0x8000):
                    //end of ICP
                    if(((pstTransInfo->wMasterCmdType + 0x8000) == wCommandAddr) &&
                       (2 == wCommandDataLen))
                    {
                        if((0 == pbDataIn[6]) && (0 == pbDataIn[7]))
                        {
                            (pstTransInfo->fCmdOK) = true;                          
                            return (wDataInLen - 8);
                        }
                    }
                    break;
					
                case (cwICP_CMD_RX_DISABLE_ADDR + 0x8000):
                default:
                    break;
            }
        }
        
    }
    
    return 0;
}

/*************************************************************************************
* Input: pstTransInfo: point of transparent information;
*        pstUARTInfo: point of updating UART information;
*        pbDataOut: point of transparent date buffer
*        pbICPRxBuf: point of Rx date buffer
*        wICPRxDataLen: ICP rx data length
*
* Output: Transparent buffer length
* 
* Logic discription: check to load transparent buffer while receive ICP cmd from cooker.
*
* run time:
*************************************************************************************/
INT16U sCheckToTransparentCmd(UARTTransInfo *pstTransInfo,
                              UpdateUARTInfo *pstUARTInfo,
                              INT8U *pbDataOut,
                              INT8U *pbICPRxBuf,
                              INT16U wICPRxDataLen)
{
    INT16U wTemp = 0;
    INT8U bCommand[10];
    
    (pstTransInfo->wMasterCmdType) = (pstUARTInfo->wCmdType);

    switch(pstTransInfo->wMasterCmdType)
    {
        case cwICP_CMD_START_COOK_ADDR:
            
            wTemp = ((pCombineICPCmd)BootUARTFunc(sBootCombineICPCmd))\
                    (pstTransInfo->bSubRs485Addr,
                     (pstTransInfo->wMasterCmdType),
                     (pstTransInfo->bTransStartCmdInfo),
                     (pstTransInfo->wTransLenth), pbDataOut, false);

            break;

        case cwICP_CMD_SET_BAUDRATE_ADDR:
//            if(((pstUARTInfo->bICPTxCH <= cbUART2)&&(pstTransInfo->bTransTxCH >= cbCAN0))||
//                ((pstUARTInfo->bICPTxCH >= cbCAN0)&&(pstTransInfo->bTransTxCH <= cbUART2)))
          if((pstUARTInfo->bICPTxCH <= cbUART5)||(pstTransInfo->bTransTxCH <= cbUART5))
            {
                bCommand[0] = 0;
                bCommand[1] = stTransUARTInfo.bBootTrsBaudrate;

                wTemp = ((pCombineICPCmd)BootUARTFunc(sBootCombineICPCmd))\
                        ((pstTransInfo->bSubRs485Addr),
                         (pstTransInfo->wMasterCmdType),
                         bCommand,
                         2, pbDataOut, false);
            }
            else
            {
                wTemp = ((pCombineICPCmd)BootUARTFunc(sBootCombineICPCmd))\
                        ((pstTransInfo->bSubRs485Addr),
                         (pstTransInfo->wMasterCmdType),
                         (pbICPRxBuf + 6),
                         (wICPRxDataLen-8), pbDataOut, false);
            }
            break;
			        
        case cwICP_CMD_DEVICE_TREE_ADDR:      
        case cwICP_CMD_RX_ENABLE_ADDR:	
        case cwICP_CMD_QUERY_ADDR_ADDR:
        case cwICP_CMD_DATA_TRF_ADDR:
        case cwICP_CMD_FINISH_COOK_ADDR:            
            
            wTemp = ((pCombineICPCmd)BootUARTFunc(sBootCombineICPCmd))\
                    ((pstTransInfo->bSubRs485Addr),
                     (pstTransInfo->wMasterCmdType),
                     (pbICPRxBuf + 6),
                     (wICPRxDataLen-8), pbDataOut, false);
            break;
        
        case cwICP_CMD_RX_DISABLE_ADDR:
        default:
            break;
    }
    return wTemp;
}

/*************************************************************************************
* Input: pstResInfo: point of response information;
*        pstUARTInfo: point of updating UART information;
*        pbDataOut: point of response date buffer
*        pbTxDataBuf: point of tx date buffer
*        wDataLen: ICP tx data length
*        pstDevTreeInfo: point of device tree;
*
* Output: Response buffer length
* 
* Logic discription: check to load response buffer while receive ICP cmd from cooker.
*
* run time:
*************************************************************************************/
INT16U sCheckToResponseCmd(UARTTransInfo *pstResInfo,
                           UpdateUARTInfo *pstUARTInfo,
                           INT8U *pbDataOut,
                           INT8U *pbTxDataBuf,
                           INT16U wDataLen,
                           DevTreeInfo *pstDevTreeInfo)
{
    INT16U wTemp = 0;
    INT8U i,j;
    INT8U bDevTreeTemp[cbMaxNode * cbDevTreeInfoLen];
    DevTreeInfo *pDevTreeTempIn;
    
    (pstResInfo->wMasterCmdType) = (pstUARTInfo->wCmdType);
    (pstResInfo->bMasterRs485Addr) = (pstUARTInfo->bRs485Addr);
   // printf("scmd%x\r\n",pstResInfo->wMasterCmdType);
    switch(pstResInfo->wMasterCmdType)
    {
        case cwICP_CMD_DEVICE_TREE_ADDR:
                        
            if((pstDevTreeInfo->bCommuCH) <= cbMaxBootSubsets)
            {
                wTemp = (INT16U)(cbDevTreeInfoLen * ((pstDevTreeInfo->bCommuCH) + 1));
                
                for(i = 0;i < ((pstDevTreeInfo->bCommuCH) + 1);i++)
                {
                    pDevTreeTempIn = (DevTreeInfo*)((INT32U)pstDevTreeInfo + (i * cbDevTreeInfoStLen));

                    bDevTreeTemp[i * cbDevTreeInfoLen + 0] = (pDevTreeTempIn->bCommuAddr);
                            
                    for(j = 0; j < cbDevIDLen; j++)
                    {
                        bDevTreeTemp[i * cbDevTreeInfoLen + j + 1] = (pDevTreeTempIn->bDevID)[j];
                    }

                    for(j = 0; j < cbFirmwareVerAllLen; j++)
                    {
                        bDevTreeTemp[i * cbDevTreeInfoLen + j + 4] = (pDevTreeTempIn->bFirmwareVer)[j];
                    }
                            
                    bDevTreeTemp[i * cbDevTreeInfoLen + 14] = (pDevTreeTempIn->bDevSubsetNum);
                }

                if((!stUpdateUARTInfo.fAppUARTICP)||(stUpdateUARTInfo.fBootStartUpdate))
                {
                    for(j = 0; j < 3; j++)
                    {
                        bDevTreeTemp[j + 8] = 0;
                    }
                }
                            
                wTemp = ((pCombineICPCmd)BootUARTFunc(sBootCombineICPCmd))
                        ((pstResInfo->bMasterRs485Addr),
                         (pstResInfo->wMasterCmdType),
                         bDevTreeTemp,
                         wTemp, pbDataOut, true);
            }
			
            
            break;
        
        case cwICP_CMD_RX_ENABLE_ADDR:
        case cwICP_CMD_QUERY_ADDR_ADDR:
        case cwICP_CMD_START_COOK_ADDR:
        case cwICP_CMD_DATA_TRF_ADDR:
        case cwICP_CMD_FINISH_COOK_ADDR:            
        case cwICP_CMD_SET_BAUDRATE_ADDR:
            
            wTemp = ((pCombineICPCmd)BootUARTFunc(sBootCombineICPCmd))\
                    ((pstResInfo->bMasterRs485Addr),
                     (pstResInfo->wMasterCmdType),
                     (pbTxDataBuf),
                     wDataLen, pbDataOut, true);
            break;
        
        case cwICP_CMD_RX_DISABLE_ADDR:
        default:
            break;
    }
    
    return wTemp;
}

void sBootUARTCodeEnd(void)
{
}


