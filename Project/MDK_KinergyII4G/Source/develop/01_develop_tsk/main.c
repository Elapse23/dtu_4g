/*
 * @Author: 23Elapse userszy@163.com
 * @Date: 2025-0
 * @LastEditors: 23Elapse userszy@163.com
 * @LastEditTime: 2025-09-09 20:17:04
 * @FilePath: \DTU_4G\Project\MDK_KinergyII4G\Source\develop\01_develop_tsk\main.c
 * @Description: 
 * 
 * Copyright (c) 2025 by 23Elapse userszy@163.com, All Rights Reserved. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "n32g45x.h"
#include "n32g45x_rcc.h"
#include "n32g45x_flash.h"

/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "bsp_serial_driver.h"
#include "bsp_spiflash.h"
#include "bsp_log_manager.h"
#include "bsp_i2c.h"
#include "led.h"
#include "tsk_4g_init.h"
#include "tsk_commu_receive.h"
#include "tsk_commu_send.h"
#include "bsp_uart_ring_buffer.h"
#include "lte_module_init.h"

/**
 * @brief 配置系统时钟到144MHz
 * @return true: 配置成功, false: 配置失败
 */
static bool SetupSystemClock(void)
{
    RCC->CTRL &= ~RCC_CTRL_PLLEN;  // 关闭PLL
    RCC->CTRL |= RCC_CTRL_HSEEN;   // 使能HSE
    
    // 等待HSE稳定
    uint32_t StartUpCounter = 0;
    bool HSEStatus = false;
    do {
        HSEStatus = (RCC->CTRL & RCC_CTRL_HSERDF) != 0;
        StartUpCounter++;
    } while ((!HSEStatus) && (StartUpCounter != HSE_STARTUP_TIMEOUT));
    
    if (!HSEStatus) {
        return false;  // HSE启动失败
    }
    
    // 配置Flash等待周期
    FLASH->AC &= ~FLASH_AC_LATENCY;
    FLASH->AC |= (uint32_t)FLASH_AC_LATENCY_4;  // 4 wait states
    FLASH->AC |= FLASH_AC_PRFTBFEN;             // Enable prefetch buffer
    
    // 配置时钟分频
    RCC->CFG &= ~(RCC_CFG_AHBPRES | RCC_CFG_APB1PRES | RCC_CFG_APB2PRES);
    RCC->CFG |= RCC_CFG_AHBPRES_DIV1;   // AHB不分频
    RCC->CFG |= RCC_CFG_APB1PRES_DIV4;  // APB1 4分频
    RCC->CFG |= RCC_CFG_APB2PRES_DIV2;  // APB2 2分频
    
    // 配置PLL: HSE * 18 = 8MHz * 18 = 144MHz
    RCC->CFG &= ~(RCC_CFG_PLLSRC | RCC_CFG_PLLHSEPRES | RCC_CFG_PLLMULFCT);
    RCC->CFG |= RCC_CFG_PLLSRC;  // HSE作为PLL输入时钟
    RCC->CFG |= RCC_CFG_PLLMULFCT18;  // PLL倍频系数18
    
    RCC->CTRL |= RCC_CTRL_PLLEN;  // 使能PLL
    StartUpCounter = 0;
    while((RCC->CTRL & RCC_CTRL_PLLRDF) == 0) {
        if (StartUpCounter++ > HSE_STARTUP_TIMEOUT) {
            return false;  // PLL锁定超时
        }
    }
    
    // 切换到PLL时钟
    RCC->CFG &= ~RCC_CFG_SCLKSW;
    RCC->CFG |= RCC_CFG_SCLKSW_PLL;
    StartUpCounter = 0;
    while ((RCC->CFG & RCC_CFG_SCLKSTS) != RCC_CFG_SCLKSTS_PLL) {
        if (StartUpCounter++ > HSE_STARTUP_TIMEOUT) {
            return false;  // 切换到PLL超时
        }
    }
    
    return true;
}

/* Forward declarations */
static void vLedTask(void* pvParameters);
void hardware_init(void);

/*
    * main: initialize system and peripherals, create tasks and start scheduler
    */
int main(void)
{    
    __set_PRIMASK(1);                                  // 关闭所有中断
    // 配置中断分组 - 只配置一次
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);    // 16个抢占优先级，0个子优先级    /* 配置系统时钟到144MHz */
    if (!SetupSystemClock()) {
        // 时钟配置失败，系统无法正常工作
        SYS_LOG_ERROR("Failed to setup system clock!");
        while(1);
    }

    /* 更新系统时钟频率变量 */
    SystemCoreClockUpdate();

    /* board-level init */
    hardware_init();

    /* Initialize 4G module initialization task */
    if (Lte_init(NULL) != pdPASS) {  // 使用默认配置
        SYS_LOG_ERROR("Failed to initialize 4G module task");
    }

    /* Initialize communication send task */
    if (CommuSend_Init() != pdPASS) {
        SYS_LOG_ERROR("Failed to initialize communication send task");
    }

    /* Initialize communication receive task */
    if (CommuReceive_Init() != pdPASS) {
        SYS_LOG_ERROR("Failed to initialize communication receive task");
    }

    /* LED heartbeat task */
    xTaskCreate(vLedTask,           "vLedTask",             configMINIMAL_STACK_SIZE,    NULL, tskIDLE_PRIORITY+1, NULL);

    /* Start scheduler */
    vTaskStartScheduler();

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
    lte_4g_module_ctrl_pin_init(); /* 4G模块控制引脚初始化 */
    UART_RingBuffer_Init(UART_ID_RS485, 115200);
    UART_RingBuffer_Init(UART_ID_LTE, 115200);
    UART_RingBuffer_Init(UART_ID_LOG, 115200);  // 初始化日志串口

    /* Log manager */
    LogConfig_t log_config = {
        .global_level = LOG_LEVEL_DEBUG,
        .output_targets = LOG_OUTPUT_UART,
        .timestamp_type = LOG_TIMESTAMP_TICK,
        .enable_colors = false,
        .enable_file_line = false
    };
    if (LogManager_Init(&log_config) != true) {
        // 如果初始化失败，在这里可能需要一些指示
        while(1) {
            // 可以通过LED闪烁等方式指示错误
            __NOP();
        }
    }

    /* LED subsystem */
    LED_Init();
}

// Simple LED task implementation used by main integration
static void vLedTask(void* pvParameters)
{
    (void)pvParameters;    
    for (;;) {
        /* call LED handler - implementation in led.c */
        LED_Flash();        
        vTaskDelay(3 * 1000);
    }
}

/* FreeRTOS 钩子函数实现 */

/**
 * @brief 堆栈溢出钩子函数
 * @param xTask 发生堆栈溢出的任务句柄
 * @param pcTaskName 任务名称
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask;
    
    /* 发生堆栈溢出时的紧急处理 */
    __disable_irq();  /* 禁用中断 */
    
    /* 如果日志系统可用，记录错误 */
    SYS_LOG_ERROR("STACK OVERFLOW! Task: %s", pcTaskName ? pcTaskName : "Unknown");
    
    /* 无限循环，等待看门狗复位或手动调试 */
    while(1) {
        /* 可以在这里添加LED闪烁指示错误 */
        __NOP();
    }
}

/**
 * @brief 内存分配失败钩子函数
 */
void vApplicationMallocFailedHook(void)
{
    /* 发生内存分配失败时的紧急处理 */
    __disable_irq();  /* 禁用中断 */
    
    /* 如果日志系统可用，记录错误 */
    SYS_LOG_ERROR("MALLOC FAILED! Out of heap memory");
    
    /* 无限循环，等待看门狗复位或手动调试 */
    while(1) {
        /* 可以在这里添加LED闪烁指示错误 */
        __NOP();
    }
}

/* vAssertCalled 已在 FreeRTOSConfig.h 中定义为宏 */


