/*
 * @Author: 23Elapse userszy@163.com
 * @Date: 2025-09-02 10:30:44
 * @LastEditors: 23Elapse userszy@163.com
 * @LastEditTime: 2025-09-03 14:56:33
 * @FilePath: \DTU_4G\Project\MDK_KinergyII4G\Source\develop\01_develop_tsk\main.c
 * @Description: 
 * 
 * Copyright (c) 2025 by 23Elapse userszy@163.com, All Rights Reserved. 
 */
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "bsp_serial_driver.h"
#include "bsp_spiflash.h"
#include "bsp_log_manager.h"
#include "bsp_i2c.h"
#include "led.h"
#include "tsk_commu_receive.h"
#include "tsk_commu_send.h"
// #include "tsk_communication_demo.h"  // Optional: uncomment to enable demo

/* Forward declarations for tasks that exist elsewhere in the project */
// extern void vSPIFlashTask(void* pvParameters);
// extern void vBLETask(void* pvParameters);
// extern void vUpgTask(void* pvParameters);
/* vCommuReceiveTask is now defined in tsk_commu_receive.h */

/* Forward declaration: LED task and hardware_init function */
static void vLedTask(void* pvParameters);
void hardware_init(void);

/* Forward declaration: LED task implemented below */
static void vLedTask(void* pvParameters);

/*
    * main: initialize system and peripherals, create tasks and start scheduler
    */
int main(void)
{
    __set_PRIMASK(1);                                  // 关闭所有中断
	__NVIC_SetPriorityGrouping(0x03);                  // 设置中断分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);    // 设置中断分组

    /* board-level init */
    hardware_init();

    /* Initialize communication send task */
    if (CommuSend_Init() != pdPASS) {
        SYS_LOG_ERROR("Failed to initialize communication send task");
    }

    /* Initialize communication receive task */
    if (CommuReceive_Init() != pdPASS) {
        SYS_LOG_ERROR("Failed to initialize communication receive task");
    }

    /* Optional: Initialize communication demo task */
    // if (CommunicationDemo_Init() != pdPASS) {
    //     SYS_LOG_ERROR("Failed to initialize communication demo task");
    // }

    // GetSoftwareVersion();

    /* Create application tasks (keep original tasks) */
    // xTaskCreate(vSPIFlashTask,      "vSPIFlashTask",        configMINIMAL_STACK_SIZE,    NULL, tskIDLE_PRIORITY+3, NULL);
    // xTaskCreate(vBLETask,           "vBLETask",             configMINIMAL_STACK_SIZE*10, NULL, tskIDLE_PRIORITY+3, NULL);
    // xTaskCreate(vUpgTask,           "vUpgTask",             configMINIMAL_STACK_SIZE*5,  NULL, tskIDLE_PRIORITY+3, NULL);

    /* LED heartbeat task */
    xTaskCreate(vLedTask,           "vLedTask",             configMINIMAL_STACK_SIZE,    NULL, tskIDLE_PRIORITY+1, NULL);

    /* Print system info and statistics */
    LogManager_PrintSystemInfo();
    LogManager_PrintStatistics();

    /* Start scheduler */
    vTaskStartScheduler();

    /* should not reach here */
    while (1) {
        __WFI();
    }

    return 0;
}

void hardware_init()
{
    /* remap debug port to SWD only (release pins) */
    bsp_debug_port_remap_swd_only();

    /* Initialize peripherals used by main application */
    Flash_Init();            /* SPI flash */

    /* UARTs: init before log manager (log manager may use LOG UART) */
    UART_RingBuffer_Init(UART_ID_RS485, 115200);
    UART_RingBuffer_Init(UART_ID_LTE, 115200);

    /* Log manager */
    LogManager_Init(NULL);

    /* LED subsystem */
    LED_Init();
}

// Simple LED task implementation used by main integration
static void vLedTask(void* pvParameters)
{
    (void)pvParameters;
    const TickType_t delay = pdMS_TO_TICKS(200);
    for (;;) {
        /* call LED handler - implementation in led.c */
        LED_Flash();
        vTaskDelay(delay);
    }
}


