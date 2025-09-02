#include "key.h"

#define key_GPIO                    GPIOA
#define key_clk                     RCC_APB2_PERIPH_GPIOA
#define key_pin                     GPIO_PIN_0

void key_init()
{
    RCC_EnableAPB2PeriphClk(key_clk,ENABLE);

    {
        GPIO_InitType GPIO_InitStructure;

        GPIO_InitStructure.Pin          = key_pin;
        GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IPU;
        GPIO_InitPeripheral(key_GPIO, &GPIO_InitStructure);
    }

}

FlagStatus key_scan()
{
    FlagStatus bKeyStatus = SET;

    bKeyStatus = (FlagStatus)GPIO_ReadInputDataBit(key_GPIO,key_pin);

    if(RESET == bKeyStatus)
    {
        bKeyStatus = SET;
    }
    else if(SET == bKeyStatus)
    {
        bKeyStatus = RESET;
    }

    return bKeyStatus;
}




