/************************************************************************************************
File Name:      IICDriver.C
Description:    simulation of MCU's IO to I2C time sequence
Version:        V1.0
Author:
Date:           2018-11-16
*************************************************************************************************/
#include "IICDriver.h"


INT8U fI2CAckError;
INT8U fI2CWriting;
INT8U fI2C2AckError;
INT8U fLrc8Write;

INT8U bI2CWREnTimer = 0;
void sDelayNop(void);
//IIC1
void sI2CStart(void);
void sI2CStop(void);
void sAckCheck(void);
void sSendAck(void);
void sSendNoAck(void);
void sI2CWriteByte(INT8U sendData);
INT8U sI2CReadByte(void);
void sCONFIG_PIN_AS_GPIO(GPIO_Module* GPIOx,uint16_t Pin,uint8_t f_output);
#define	HalI2CDataOP()      \
    asm("CPSID I");         \
    sCONFIG_PIN_AS_GPIO(I2C_GPIO,I2Cx_SDA_PIN,1);\
    asm("CPSIE I")

#define	HalI2CDataIP()      \
    asm("CPSID I");         \
    sCONFIG_PIN_AS_GPIO(I2C_GPIO,I2Cx_SDA_PIN,0);\
    asm("CPSIE I")

/************************************************************************************************
Function name:  sI2CStart
Description:    I2C start signal.
                A high-to-low transition of SDA with SCL high is a start condition which must precede any other command.
Calls:
Called By:
Parameters:     None
Return:         None.
Others:
************************************************************************************************/
void sI2CStart(void)
{
    HalI2CWriteHigh();
    sDelay4us();

    HalI2CClkHigh();
    sDelay4us();

    HalI2CWriteLow();
    sDelay4us();

    HalI2CClkLow();
    sDelay4us();
}
/************************************************************************************************
Function name:  sI2CStop
Description:    I2C start condition.
                A low-to-high transition of SDA with SCL high is a stop condition.
                After a read sequence, the stop command will place the EEPROM in a standby power mode.
                It must delay a internal clear/write cycle(max=10ms) between a valid stop condition and
                the next start condition.
Calls:
Called By:
Parameters:     None
Return:         None.
Others:
************************************************************************************************/
void sI2CStop(void)
{
    HalI2CWriteLow();
    sDelay4us();

    HalI2CClkHigh();
    sDelay4us();

    HalI2CWriteHigh();
    sDelay4us();
}
/************************************************************************************************
Function name:  sAckCheck
Description:    24cxx will respond with a zero when receipt of a 8-bit data word.
Calls:
Called By:
Parameters:     None
Return:         fI2CAckError=1,24cxx did not respond.
Others:
************************************************************************************************/
void sAckCheck(void)
{
    HalI2CWriteHigh();
    sDelayNop();

    HalI2CDataIP();

    HalI2CClkHigh();
    sDelay4us();

    if (HalI2CReadHighChk())
    {
    	// Do not respond.
        fI2CAckError = 1;
    }
    else
    {
    	// response is ok
        fI2CAckError = 0;
    }

    HalI2CClkLow();
    sDelay4us();

    HalI2CDataOP();
}
/************************************************************************************************
Function name:  sSendAck
Description:    MCU will respond with a zero when receipt of a 8-bit data word.
Calls:
Called By:
Parameters:     None
Return:         None.
Others:
************************************************************************************************/
void sSendAck(void)
{
    HalI2CWriteLow();

    sDelay4us();
    HalI2CClkHigh();

    sDelay4us();
    HalI2CClkLow();

    sDelay4us();
    HalI2CWriteHigh();
    sDelay4us();
}
/************************************************************************************************
Function name:  sSendNoAck
Description:    MCU does not respond with a zero but does generate a following stop condition.
Calls:
Called By:
Parameters:     None
Return:         None.
Others:
************************************************************************************************/
void sSendNoAck(void)
{
    HalI2CWriteHigh();
    sDelay4us();

    HalI2CClkHigh();
    sDelay4us();

    HalI2CClkLow();
    sDelay4us();
}
/************************************************************************************************
Function name:  sI2CWriteByte
Description:    MCU writes a 8-bit data word to 24cxx.
Calls:
Called By:
Parameters:     None
Return:         None.
Others:
************************************************************************************************/
void sI2CWriteByte(INT8U sendData)
{
    INT8U i = 0;
//    HalI2CDataOP();
    for (; i < 8; i++)
    {
        if ((sendData & 0x80) != 0)
        {
            HalI2CWriteHigh();
        }
        else
        {
            HalI2CWriteLow();
        }
        sendData <<= 1;

        sDelayNop();
        HalI2CClkHigh();

        sDelay4us();
        HalI2CClkLow();

    }

    sDelayNop();
}
/************************************************************************************************
Function name:  sI2CReadByte
Description:    MCU reads a 8-bit data word from 24cxx.
Calls:
Called By:
Parameters:     None
Return:         The 8-bit data word.
Others:
************************************************************************************************/
INT8U sI2CReadByte(void)
{
    INT8U i;
    INT8U ReadData = 0;
    HalI2CDataIP();

    for (i = 0; i < 8; i++)
    {
        HalI2CClkHigh();
        sDelay4us();

        ReadData <<= 1;
        if (HalI2CReadHighChk())
        {
            ReadData |= 0x01;
        }

        HalI2CClkLow();
        sDelay4us();

    }

    HalI2CDataOP();

    return(ReadData);
}
/************************************************************************************************
Function name:  sI2CWriteBytes
Description:    MCU writes data of g_bI2CWriteData[] into the location of address from bAddress
                to (bAddress+bDataLength).

Calls:
Called By:
Parameters:     1.bAddress:the start location.0x0000~0x07ff.
                  bDataLength:number of 8-bit data word.max=16.Current project is 10 bytes data frame.
                  fI2CAckError=1,can not write to 24cxx.
                2.bAddress
                MSB-BIT7 BIT6 BIT5 BIT4 BIT3 BIT2 BIT1 BIT0-LSB.
                     1    0    1    0   B2   B1   B0   R/W
                B2,B1,B0=high 3 bits of bAddress.LSB=0:write data to 24cxx;LSB=1:read data from 24cxx.
Return:         None.
Others:
************************************************************************************************/
void sEepromWrite(INT16U wAddress, INT8U bDataLength, INT8U *bWriteBuf)
{
    INT8U bErrorCnt;
    INT8U bDataCnt;
    INT8U bLrc8Chk = 0;
    INT8U *Ptr = bWriteBuf;

    fI2CAckError = 1;

    if((bDataLength <= 16))
    {
        HalEEPROMWrEnable();
        sDelayNop();
        // write error more than 3 times,terminate sI2CWriteBytes.
        for (bErrorCnt = 0; (bErrorCnt < 3) && fI2CAckError; bErrorCnt++)
        {
            sI2CStart();

            // high byte address
            sI2CWriteByte(0xA0 & 0xFE);
            sAckCheck();
        }
        if(!fI2CAckError && (bErrorCnt < 2))
        {
            // high byte address
            sI2CWriteByte((INT8U)(wAddress>>8));
            sAckCheck();
        	// low byte address
            sI2CWriteByte((INT8U)wAddress);
            sAckCheck();
            for (bDataCnt = 0; bDataCnt < bDataLength; bDataCnt++,Ptr++)//
            {
                bLrc8Chk += *Ptr;
                sI2CWriteByte(*Ptr);
                sAckCheck();
            }
            if (fLrc8Write)
            {
                sI2CWriteByte((INT8U)(0 - bLrc8Chk));
                sAckCheck();
            }
        }
        sI2CStop();
        HalEEPROMWrDisable();
    }
    fLrc8Write = 0;
}

/************************************************************************************************
Function name:  sI2CReadBytes
Description:    MCU reads the concents of the location of address from bAddress to (bAddress+bDataLength).

Calls:
Called By:
Parameters:     1.bAddress:the start location.0x0000~0x07ff.
                  bDataLength:number of 8-bit data word.Current project is 9 bytes data frame.
                  fI2CAckError=1,can not write to 24cxx.
                2.bAddress
                MSB-BIT7 BIT6 BIT5 BIT4 BIT3 BIT2 BIT1 BIT0-LSB.
                     1    0    1    0   B2   B1   B0   R/W
                B2,B1,B0=high 3 bits of bAddress.LSB=0:write data to 24cxx;LSB=1:read data from 24cxx.
                3.g_bI2CReadData[]:data read from 24cxx.
Return:         None.
Others:           A random read requires a "dummy" byte write sequence to load in the data word address.
                Once the device address word and data word address are clocked in and acknowledged by the
                EEPROM, the microcontroller must generate another start condition. The microcontroller
                now initiates a current address read by sending a device address with the read/write select
                bit high. The EEPROM acknowledges the device address and serially clocks out the data word.
                The microcontroller does not respond with a zero but does generate a following stop condition.
************************************************************************************************/
void sEepromRead(INT16U wAddress, INT8U bDataLength, INT8U *bReadBuf)
{
    INT8U bErrorCnt;
    INT8U bDataCnt;
    INT8U *Ptr = bReadBuf;

    fI2CAckError = 1;

    if ((bDataLength <= 16))//(wAddress <= 0xFFFF)
    {
        HalEEPROMWrEnable();
        sDelayNop();

		// dummy byte write
		// write error more than 3 times,terminate sI2CReadBytes.
        for (bErrorCnt = 0; (bErrorCnt < 3) && fI2CAckError; bErrorCnt++)
        {
            sI2CStart();
            // high byte address;
            sI2CWriteByte(0xA0 & 0xFE);
            sAckCheck();
        }

        if(!fI2CAckError)
        {
            // high byte address
            sI2CWriteByte((INT8U)(wAddress>>8));
            sAckCheck();
        	// low byte address
            sI2CWriteByte((INT8U)wAddress);
            sAckCheck();
			// a current address read
            fI2CAckError = 1;
            // write error more than 3 times,terminate sI2CReadBytes.
            for (bErrorCnt = 0; (bErrorCnt < 3) && fI2CAckError; bErrorCnt++)
            {
                sI2CStart();
                sI2CWriteByte(0xA1);
                sAckCheck();
            }
            if (!fI2CAckError)
            {
                for (bDataCnt = 0; bDataCnt < bDataLength; bDataCnt++,Ptr++)//
                {
                    *Ptr = sI2CReadByte();
                    if (bDataCnt < (bDataLength - 1))
                    {
                        sSendAck();
                    }
                }
                fI2CAckError = 0;
            }
            sSendNoAck();
            sI2CStop();
            HalEEPROMWrDisable();
        }
    }
}
/*************************************************************************************
* Input: bSlaveAddress; bStartAddr: slave register address; bDataLength; *bWriteBuf.
*
* Output: communication stauts.
*
* Logic discription: write buffer to IIC slave module
*
* run time:
*************************************************************************************/
INT8U sIICSlaveWrite(INT8U bSlaveAddress, INT8U bStartAddr, INT8U bDataLength, const INT8U *bWriteBuf)
{
    INT8U bErrorCnt;
    INT8U bDataCnt;
    const INT8U *Ptr = bWriteBuf;

    fI2CAckError = 1;

    sDelayNop();

    // write error more than 3 times,terminate sI2CWriteBytes.
    for (bErrorCnt = 0; (bErrorCnt < 5) && fI2CAckError; bErrorCnt++)
    {
        sI2CStart();

        // write byte
        sI2CWriteByte((INT8U)((bSlaveAddress << 1) & 0xFE));
        sAckCheck();
    }

    if(!fI2CAckError)
    {
    	// register start address
        sI2CWriteByte(bStartAddr);
        sAckCheck();

        if(!fI2CAckError)
        {
            //send data
            for (bDataCnt = 0; bDataCnt < bDataLength; bDataCnt++, Ptr++)
            {
                sI2CWriteByte(*Ptr);
                sAckCheck();

                if(fI2CAckError)
                {
                    break;
                }
            }
        }
    }

    sI2CStop();

    if(!fI2CAckError)
    {
        return true;
    }
    else
    {
        return false;   //communication error.
    }

}
/*************************************************************************************
* Input: bSlaveAddress; bStartAddr: slave register address; bDataLength; *bReadBuf.
*
* Output: communication stauts.
*
* Logic discription: Read buffer from IIC slave module
*
* run time:
*************************************************************************************/
INT8U sIICSlaveRead(INT8U bSlaveAddress, INT8U bStartAddr, INT8U bDataLength, INT8U *bReadBuf)
{
    INT8U bErrorCnt;
    INT8U bDataCnt;
    INT8U *Ptr = bReadBuf;

    fI2CAckError = 1;

    sDelayNop();

    // write error more than 3 times,terminate sI2CWriteBytes.
    for (bErrorCnt = 0; (bErrorCnt < 5) && fI2CAckError; bErrorCnt++)
    {
        sI2CStart();

        // write byte
        sI2CWriteByte((INT8U)((bSlaveAddress << 1) & 0xFE));
        sAckCheck();
    }

    if(!fI2CAckError)
    {
    	// register start address
        sI2CWriteByte(bStartAddr);
        sAckCheck();

        if(!fI2CAckError)
        {
            sI2CStart();

            // read byte
            sI2CWriteByte((INT8U)((bSlaveAddress << 1) | 0x01));
            sAckCheck();

            if(!fI2CAckError)
            {
                //read data
                for (bDataCnt = 0; bDataCnt < bDataLength; bDataCnt++,Ptr++)
                {
                    *Ptr = sI2CReadByte();
                    if (bDataCnt < (bDataLength - 1))
                    {
                        sSendAck();
                    }

                }
            }
        }
    }

    sI2CStop();

    if(!fI2CAckError)
    {
        return true;
    }
    else
    {
        return false;   //communication error.
    }
}

void sDelayNop(void)
{
    for(uint32_t dw_cnt_i = 0;dw_cnt_i < 24;dw_cnt_i++)
    {
        asm("NOP\n");
    }
}
void sDelay4us(void)
{
    INT8U i;

    for(i = 0; i < 4; i++)
    {
        //bus 40Mhz, delay 4us
        sDelayNop();
    }
}
void sDelay2ms(void)
{
    INT8U i;

    for(i = 0; i < 95; i++)
    {
        //bus 20Mhz, delay 2ms
        sDelayNop();
    }
}
void sDelay10ms(void)
{
    INT16U i;

    for(i = 0; i < 4762; i++)
    {
        //bus 20Mhz, delay 10ms
        sDelayNop();
    }
}
void sDelay20ms(void)
{
    INT16U i;

    for(i = 0; i < 9524; i++)
    {
        //bus 20Mhz, delay 20ms
        sDelayNop();
    }
}
void sDelay100ms(void)
{
    INT16U i;

    for(i = 0; i < 47619; i++)
    {
        //bus 20Mhz, delay 100ms
        sDelayNop();
    }
}
void sIICPinInit(void)
{
//    CONFIG_PIN_AS_GPIO(PTA,PTA0,OUTPUT);//EEPROM_SDA
//    CONFIG_PIN_AS_GPIO(PTA,PTA1,OUTPUT);//EEPROM_SCL
//    CONFIG_PIN_AS_GPIO(PTE,PTE6,OUTPUT);//EEPROM_WP
    GPIO_InitType GPIO_InitStructure;

    GPIO_InitStructure.Pin        = I2Cx_SCL_PIN | I2Cx_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitPeripheral(I2C_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.Pin        = I2Cx_WP_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitPeripheral(I2Cx_WP_GPIO, &GPIO_InitStructure);
}

void sCONFIG_PIN_AS_GPIO(GPIO_Module* GPIOx,uint16_t Pin,uint8_t f_output)
{
    GPIO_InitType GPIO_InitStructure;
    GPIO_InitStructure.Pin        = Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    if(f_output)
    {
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    }
    else
    {
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    }
    GPIO_InitPeripheral(GPIOx, &GPIO_InitStructure);
}
