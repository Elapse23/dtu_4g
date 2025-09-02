#ifndef _LTE_MODULE_COM_INTERFACE_INIT_H_
#define _LTE_MODULE_COM_INTERFACE_INIT_H_
#include "application_general_func.h"
#include <stdint.h>


#ifdef KINERGY_II_WIFI
#define lte_module_reset_GPIO                   GPIOB
#define lte_module_reset_clk                    RCC_APB2_PERIPH_GPIOB
#define lte_module_reset_pin                    GPIO_PIN_0
#define lte_module_reset_pin_reset()            GPIO_ResetBits(lte_module_reset_GPIO,lte_module_reset_pin)
#define lte_module_reset_pin_set()              GPIO_SetBits(lte_module_reset_GPIO,lte_module_reset_pin)

#else

#define lte_module_reset_GPIO                   GPIOB
#define lte_module_reset_clk                    RCC_APB2_PERIPH_GPIOB
#define lte_module_reset_pin                    GPIO_PIN_6
#define lte_module_reset_pin_reset()            GPIO_ResetBits(lte_module_reset_GPIO,lte_module_reset_pin)
#define lte_module_reset_pin_set()              GPIO_SetBits(lte_module_reset_GPIO,lte_module_reset_pin)
#define lte_module_power_GPIO                   GPIOB
#define lte_module_power_clk                    RCC_APB2_PERIPH_GPIOB
#define lte_module_power_pin                    GPIO_PIN_5
#define lte_module_power_pin_reset()            GPIO_ResetBits(lte_module_power_GPIO,lte_module_power_pin)
#define lte_module_power_pin_set()              GPIO_SetBits(lte_module_power_GPIO,lte_module_power_pin)
#define lte_module_PWR_GPIO                   	GPIOC
#define lte_module_PWR_clk                    	RCC_APB2_PERIPH_GPIOC
#define lte_module_PWR_pin                    	GPIO_PIN_13
#define lte_module_PWR_pin_reset()            	GPIO_ResetBits(lte_module_PWR_GPIO,lte_module_PWR_pin)
#define lte_module_PWR_pin_set()              	GPIO_SetBits(lte_module_PWR_GPIO,lte_module_PWR_pin)


#endif


void lte_module_ctrl_pin_init();
void lte_module_hardware_reset();
void lte_4g_module_hardware_reset();
void lte_4g_module_ctrl_pin_init();
void lte_4g_module_hardware_open();




#endif
