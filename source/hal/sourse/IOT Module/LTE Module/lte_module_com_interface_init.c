#include "lte_module_com_interface_init.h"
#include "application_general_macro.h"
#include "n32g45x.h"
#include "portmacro.h"
#include "freertos_user_config.h"
#include "lte_module_com_interface_init.h"
/*lte module ctrl pin*/
#ifdef KINERGY_II_WIFI
//#define lte_module_reset_GPIO                   GPIOB
//#define lte_module_reset_clk                    RCC_APB2_PERIPH_GPIOB
//#define lte_module_reset_pin                    GPIO_PIN_0
//#define lte_module_reset_pin_reset()            GPIO_ResetBits(lte_module_reset_GPIO,lte_module_reset_pin)
//#define lte_module_reset_pin_set()              GPIO_SetBits(lte_module_reset_GPIO,lte_module_reset_pin)

void lte_module_ctrl_pin_init()
{
    RCC_EnableAPB2PeriphClk(lte_module_reset_clk,ENABLE);
    {
        GPIO_InitType GPIO_InitStructure;

        GPIO_InitStructure.Pin          = lte_module_reset_pin;
        GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;
        GPIO_InitPeripheral(lte_module_reset_GPIO, &GPIO_InitStructure);
    }
    lte_module_reset_pin_set();
}

void lte_module_hardware_reset() // WIFI 模块硬件复位
{
    vTaskDelay(100);
    lte_module_reset_pin_reset();
    vTaskDelay(5);
    lte_module_reset_pin_set();
}

#else

//#define lte_module_reset_GPIO                   GPIOB
//#define lte_module_reset_clk                    RCC_APB2_PERIPH_GPIOB
//#define lte_module_reset_pin                    GPIO_PIN_6
//#define lte_module_reset_pin_reset()            GPIO_ResetBits(lte_module_reset_GPIO,lte_module_reset_pin)
//#define lte_module_reset_pin_set()              GPIO_SetBits(lte_module_reset_GPIO,lte_module_reset_pin)
//#define lte_module_power_GPIO                   GPIOB
//#define lte_module_power_clk                    RCC_APB2_PERIPH_GPIOB
//#define lte_module_power_pin                    GPIO_PIN_5
//#define lte_module_power_pin_reset()            GPIO_ResetBits(lte_module_power_GPIO,lte_module_power_pin)
//#define lte_module_power_pin_set()              GPIO_SetBits(lte_module_power_GPIO,lte_module_power_pin)

void lte_4g_module_ctrl_pin_init()
{
    RCC_EnableAPB2PeriphClk(lte_module_reset_clk | lte_module_power_clk | lte_module_PWR_clk,ENABLE);
    {
        GPIO_InitType GPIO_InitStructure;

        GPIO_InitStructure.Pin          = lte_module_reset_pin | lte_module_power_pin;
        GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;
        GPIO_InitPeripheral(lte_module_reset_GPIO, &GPIO_InitStructure);
		GPIO_InitPeripheral(lte_module_power_GPIO, &GPIO_InitStructure);
		GPIO_InitStructure.Pin          = lte_module_PWR_pin;
        GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;
		GPIO_InitPeripheral(lte_module_PWR_GPIO, &GPIO_InitStructure);
    }
	lte_module_PWR_pin_set();
	lte_module_power_pin_set();
	lte_module_reset_pin_set();
}

void lte_4g_module_hardware_reset()  // 4G模块硬件复位
{
    lte_module_reset_pin_reset();
    vTaskDelay(pdMS_TO_TICKS_USER(500));
	lte_module_reset_pin_set();
    lte_module_power_pin_set();
	vTaskDelay(pdMS_TO_TICKS_USER(1500));
	lte_module_reset_pin_reset();
	lte_module_power_pin_reset();
	vTaskDelay(pdMS_TO_TICKS_USER(3000));
	lte_module_reset_pin_set();
    lte_module_power_pin_set();
}
void lte_4g_module_hardware_open()  // 4G模块硬件复位
{
    vTaskDelay(pdMS_TO_TICKS_USER(1000));
    lte_module_power_pin_reset();
    vTaskDelay(pdMS_TO_TICKS_USER(3000));
    lte_module_power_pin_set();
}
void lte_4g_module_hardware_close()  // 4G模块硬件复位
{
	lte_module_power_pin_reset();
    vTaskDelay(pdMS_TO_TICKS_USER(5000));
    lte_module_power_pin_set();
    vTaskDelay(pdMS_TO_TICKS_USER(30000));

}

#endif