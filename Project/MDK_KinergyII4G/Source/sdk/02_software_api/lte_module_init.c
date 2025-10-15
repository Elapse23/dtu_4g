/*
 * @Author: 23Elapse userszy@163.com
 * @Date: 2025-09-07 19:21:33
 * @LastEditors: Elapse23 userszy@163.com
 * @LastEditTime: 2025-10-15 11:28:38
 * @FilePath: \dtu_4g\Project\MDK_KinergyII4G\Source\sdk\02_software_api\lte_module_init.c
 * @Description: 
 * 
 * Copyright (c) 2025 by 23Elapse userszy@163.com, All Rights Reserved. 
 */
#include "lte_module_init.h"
#include "FreeRTOS.h"
#include "task.h"


void lte_4g_module_ctrl_pin_init(void)
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
    vTaskDelay(pdMS_TO_TICKS(500));             // 500ms
	lte_module_reset_pin_set();
    lte_module_power_pin_set();
	vTaskDelay(pdMS_TO_TICKS(1500));            // 1.5s
	lte_module_reset_pin_reset();
	lte_module_power_pin_reset();
	vTaskDelay(pdMS_TO_TICKS(3000));            // 3s
	lte_module_reset_pin_set();
    lte_module_power_pin_set();
}
void lte_4g_module_hardware_open()  // 4G模块硬件复位
{
    vTaskDelay(pdMS_TO_TICKS(1000));            // 1s
    lte_module_power_pin_reset();
    vTaskDelay(pdMS_TO_TICKS(3000));            // 3s
    lte_module_power_pin_set();
}
void lte_4g_module_hardware_close()  // 4G模块硬件复位
{
	lte_module_power_pin_reset();
    vTaskDelay(pdMS_TO_TICKS(5000));            // 5s
    lte_module_power_pin_set();
    vTaskDelay(pdMS_TO_TICKS(30000));           // 30s

}

