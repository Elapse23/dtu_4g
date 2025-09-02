#include "n32g45x.h"

#include "BootIICCode.h"



#if(true == HalBootI2CFuncEN)


//#if(true == HalBootI2CFuncEN)
void sBootEepromRead(INT16U bAddress, INT8U bDataLength, INT8U *bReadBuf);
static void sBootI2CStart(void);
static void sBootI2CStop(void);
static void sBootAckCheck(void);
static void sBootSendAck(void);
static void sBootSendNoAck(void);
static void sBootI2CWriteByte(INT8U sendData);
static INT8U sBootI2CReadByte(void);
//static void sBootReadEEInfo(void);

//#endif  //#if(true == HalBootI2CFuncEN)
static void sBootEepromRead(INT16U bAddress, INT8U bDataLength, INT8U *bReadBuf)
{
    INT8U bErrorCnt;
    INT8U bDataCnt;
    INT8U *Ptr = bReadBuf;

    fBootI2CAckError = 1;

    if ((bAddress <= cwBOOT_MAX_ADDRESS_ADDR) &&  bDataLength <= 16)
    {
        HalBootI2CWrEnable();
        mDelayNop();

        // dummy byte write
        // write error more than 3 times,terminate sI2CReadBytes.
        for (bErrorCnt = 0; (bErrorCnt < 3) && fBootI2CAckError; bErrorCnt++)
        {
            sBootI2CStart();
// high byte address
#ifdef  cbBootEepromType_24C16
            sBootI2CWriteByte((INT8U)(((INT8U)(bAddress >> 7) | cbBOOT_I2C_DEVICE0_ADDR) & cbBOOT_I2C_DEVICE_WRITE));
#else
            sBootI2CWriteByte(cbBOOT_I2C_DEVICE0_ADDR & cbBOOT_I2C_DEVICE_WRITE);
#endif
            sBootAckCheck();
        }

        if(!fBootI2CAckError)
        {
#ifdef  cbBootEepromType_24C16
            sBootI2CWriteByte((INT8U)bAddress);      //low byte address
            sBootAckCheck();
#else
            sBootI2CWriteByte((INT8U)(bAddress >> 8));  //high byte address
            sBootAckCheck();

            sBootI2CWriteByte((INT8U)bAddress);     //low byte address
            sBootAckCheck();
#endif

            // a current address read
            fBootI2CAckError = 1;

            // write error more than 3 times,terminate sI2CReadBytes.
            for (bErrorCnt = 0; (bErrorCnt < 3) && fBootI2CAckError; bErrorCnt
++)
            {
                sBootI2CStart();
#ifdef  cbBootEepromType_24C16
                sBootI2CWriteByte((INT8U)((INT8U)(bAddress >> 7) |
cbBOOT_I2C_DEVICE0_ADDR | cbBOOT_I2C_DEVICE_READ));
#else
                sBootI2CWriteByte(cbBOOT_I2C_DEVICE0_ADDR |
cbBOOT_I2C_DEVICE_READ);
#endif
                sBootAckCheck();
            }

            if (!fBootI2CAckError)
            {
                for (bDataCnt = 0; bDataCnt < bDataLength; bDataCnt++,Ptr++)
                {
                    *Ptr = sBootI2CReadByte();
                    if(bDataCnt < (bDataLength-1))
                    {
                        sBootSendAck();
                    }
                }
            }

            sBootSendNoAck();
            sBootI2CStop();
            HalBootI2CWrDisable();
        }
    }
}

static void sBootI2CStart(void)
{
    HalBootI2CWriteHigh();
    mDelayNop();
    mDelayNop();

    HalBootI2CClkHigh();
    mDelayNop();
    mDelayNop();

    HalBootI2CWriteLow();
    mDelayNop();
    mDelayNop();

    HalBootI2CClkLow();
    mDelayNop();
    mDelayNop();
}


static void sBootI2CStop(void)
{
    HalBootI2CWriteLow();
    mDelayNop();
    mDelayNop();

    HalBootI2CClkHigh();
    mDelayNop();
    mDelayNop();

    HalBootI2CWriteHigh();
    mDelayNop();
    mDelayNop();

    HalBootI2CClkLow();
    mDelayNop();
    mDelayNop();
}


static void sBootAckCheck(void)
{
    HalBootI2CWriteHigh();
    mDelayNop();
    mDelayNop();

    HalBootI2CDataIP();


    HalBootI2CClkHigh();
    mDelayNop();
    mDelayNop();


    if (HalBootI2CReadHighChk())
    {
        // Do not respond.
        fBootI2CAckError = 1;
    }
    else
    {
        // response is ok
        fBootI2CAckError = 0;
    }

    HalBootI2CClkLow();
    mDelayNop();
    mDelayNop();

    HalBootI2CDataOP();
    mDelayNop();
    mDelayNop();
}


static void sBootSendAck(void)
{
    HalBootI2CWriteLow();

    mDelayNop();
    mDelayNop();
    HalBootI2CClkHigh();

    mDelayNop();
    mDelayNop();
    HalBootI2CClkLow();

    mDelayNop();
    mDelayNop();
    HalBootI2CWriteHigh();
    mDelayNop();
    mDelayNop();
}


static void sBootSendNoAck(void)
{
    HalBootI2CWriteHigh();
    mDelayNop();
    mDelayNop();

    HalBootI2CClkHigh();
    mDelayNop();
    mDelayNop();

    HalBootI2CClkLow();
    mDelayNop();
    mDelayNop();
}


static void sBootI2CWriteByte(INT8U sendData)
{
    INT8U i = 0;

    for (; i < 8; i++)
    {
        if ((sendData & 0x80) != 0)
        {
            HalBootI2CWriteHigh();
        }
        else
        {
            HalBootI2CWriteLow();
        }
        sendData <<= 1;

        mDelayNop();
        HalBootI2CClkHigh();

        mDelayNop();
        HalBootI2CClkLow();

    }

    mDelayNop();
}

static INT8U sBootI2CReadByte(void)
{
    INT8U i;
    INT8U ReadData = 0;
    HalBootI2CDataIP();
    mDelayNop();
    mDelayNop();

    for (i = 0; i < 8; i++)
    {
        HalBootI2CClkHigh();
        mDelayNop();

        ReadData <<= 1;
        if (HalBootI2CReadHighChk())
        {
            ReadData |= 0x01;
        }

        HalBootI2CClkLow();
        mDelayNop();

    }

    HalBootI2CDataOP();
    mDelayNop();
    mDelayNop();
    return(ReadData);
}

void sBootReadEEInfo(void)
{
	//sBootGPIO_PinInit(O_Boot_I2C_SCL, GPIO_PinOutput, 0);//EE I2C SCL
    //sBootGPIO_PinInit(O_Boot_I2C_SDA, GPIO_PinOutput, 0);//EE I2C SDA
    //sBootGPIO_PinInit(O_Boot_I2C_WP, GPIO_PinOutput, 0);//EE I2C WP
    log_line();
#if(cbEEADDR_UART0458Addr != 0)
    //read eeprom
    //read RS485 address
    sBootEepromRead(cbUART0RS485_EESaveAddr, 1, &(pstBootUARTConfig->bRs485Addr)[cbUART0]);
#endif

#if(cbEEADDR_UART1458Addr != 0)
    //read eeprom
    //read RS485 address
    sBootEepromRead(cbUART1RS485_EESaveAddr, 1, &(pstBootUARTConfig->bRs485Addr)[cbUART1]);
#endif

#if(cbEEADDR_UART2458Addr != 0)
    //read eeprom
    //read RS485 address
    sBootEepromRead(cbUART2RS485_EESaveAddr, 1, &(pstBootUARTConfig->bRs485Addr)[cbUART2]);
#endif

#if(cbEEADDR_UART0Baudrate != 0)
    //read eeprom
    //read baudrate
    sBootEepromRead(cbEEADDR_UART0Baudrate, 1, &(pstBootUARTConfig->bBaudrate)[cbUART0]);
#endif

#if(cbEEADDR_UART1Baudrate != 0)
    //read eeprom
    //read baudrate
    sBootEepromRead(cbEEADDR_UART1Baudrate, 1, &(pstBootUARTConfig->bBaudrate)[cbUART1]);
#endif

#if(cbEEADDR_UART2Baudrate != 0)
    //read eeprom
    //read baudrate
    sBootEepromRead(cbEEADDR_UART2Baudrate, 1, &(pstBootUARTConfig->bBaudrate)[cbUART2]);
#endif

#if(cbEEADDR_UART0Parity != 0)
    //read eeprom
    //read parity
    sBootEepromRead(cbEEADDR_UART0Parity, 1, &(pstBootUARTConfig->bParity)[cbUART0]);
#endif

#if(cbEEADDR_UART1Parity != 0)
    //read eeprom
    //read parity
    sBootEepromRead(cbEEADDR_UART1Parity, 1, &(pstBootUARTConfig->bParity)[cbUART1]);
#endif

#if(cbEEADDR_UART2Parity != 0)
    //read eeprom
    //read parity
    sBootEepromRead(cbEEADDR_UART2Parity, 1, &(pstBootUARTConfig->bParity)[cbUART2]);
#endif

#if(cbEEADDR_SerialNum != 0)
    //read eeprom
    //read seiral-number
    sBootEepromRead(cbEEADDR_SerialNum, cbDevSNLen, (ppstBootDevTree(0)->bDevSN));
#endif
    uint8_t temp[16];
    sBootEepromRead(208,15,temp);
    log_debug("eeread");
    log_sector(temp,15);
}

void  sBootReadEEInit(void)
{
    MACRO_I2C_WP_INIT();
    MACRO_I2C_SCL_INIT();
    MACRO_I2C_SDA_INIT();
    sBootReadEEInfo();
}
#endif  //#if(true == HalBootI2CFuncEN)
