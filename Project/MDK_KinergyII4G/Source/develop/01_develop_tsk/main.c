/*
 * @Author: 23Elapse userszy@163.com
 * @Date: 2025-0
 * @LastEditors: 23Elapse userszy@163.com
 * @LastEditTime: 2025-09-04 09:27:04
 * @FilePath: \DTU_4G\Project\MDK_KinergyII4G\Source\develop\01_develop_tsk\main.c
 * @Description: 
 * 
 * Copyright (c) 2025 by 23Elapse userszy@163.com, All Rights Reserved. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

/* 看门狗喂狗宏定义 */
#define KEY_ReloadKey ((uint16_t)0xAAAA)
#define MACRO_IWDG_RELOAD()      IWDG->KEY = KEY_ReloadKey

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

/* TCP事件回调函数 */
static void tcp_event_callback(uint8_t event_type, uint8_t socket_id, uint8_t error_code, void* user_data);

/*
    * main: initialize system and peripherals, create tasks and start scheduler
    */
int main(void)
{    
    __set_PRIMASK(1);                                  // 关闭所有中断
	__NVIC_SetPriorityGrouping(0x03);                  // 设置中断分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);    // 设置中断分组

    /* 更新系统时钟频率变量 */
    SystemCoreClockUpdate();

    /* board-level init */
    hardware_init();

    /* Initialize 4G module initialization task */
    if (Quectel4G_Init(NULL) != pdPASS) {  // 使用默认配置
        SYS_LOG_ERROR("Failed to initialize 4G module task");
    }

    /* 2. 配置TCP服务器连接 */
    Quectel4G_TcpServerConfig_t tcp_config = {
        .server_port = 8080,
        .local_port = 0,  // 自动分配
        .connect_timeout_ms = 30000,  // 30秒，以毫秒为单位
        .retry_count = 5,
        .retry_interval_ms = 10000,   // 10秒重试间隔
        .auto_reconnect = true,
        .keepalive_interval_s = 0,    // 禁用TCP保活
        .heartbeat_interval_s = 60,
        .heartbeat_data = NULL,
        .heartbeat_length = 0
    };
    
    /* 复制服务器IP地址到结构体 */
    strncpy(tcp_config.server_ip, "192.168.51.91", sizeof(tcp_config.server_ip) - 1);
    tcp_config.server_ip[sizeof(tcp_config.server_ip) - 1] = '\0';
    QuectelDataResult_t result = Quectel4G_ConnectTcpServer(&tcp_config, tcp_event_callback, NULL);
    if (result != QUECTEL_DATA_OK) {  /* 修正返回值检查 */
        SYS_LOG_ERROR("Failed to connect TCP server: %d", result);
        /* 继续执行，不要返回 */
    }

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

    /* 在启动调度器前喂一次看门狗，确保有足够时间启动任务 */
    MACRO_IWDG_RELOAD();
    SYS_LOG_INFO("Initial watchdog feed before scheduler start");

    /* Start scheduler */
    vTaskStartScheduler();

    /* should not reach here */
    while (1) {
        __WFI();
    }

    return 0;
}

/* TCP事件回调函数实现 */
static void tcp_event_callback(uint8_t event_type, uint8_t socket_id, uint8_t error_code, void* user_data)
{
    switch (event_type) {
        case 0:  /* 首次连接成功 */
            SYS_LOG_INFO("TCP connection established successfully (Socket ID: %d)", socket_id);
            break;
        case 1:  /* 连接失败 */
            SYS_LOG_ERROR("TCP connection failed (Error code: %d)", error_code);
            break;
        case 2:  /* 连接断开 */
            SYS_LOG_WARN("TCP connection disconnected (Socket ID: %d)", socket_id);
            break;
        case 3:  /* 重连成功 */
            SYS_LOG_INFO("TCP reconnection successful (Socket ID: %d)", socket_id);
            break;
        case 4:  /* 重连失败 */
            SYS_LOG_ERROR("TCP reconnection failed (Error code: %d)", error_code);
            break;
        default:
            SYS_LOG_WARN("Unknown TCP event type: %d", event_type);
            break;
    }
}

void hardware_init()
{
    /* 检查复位原因，用于调试 */
    uint32_t reset_flags = RCC->CTRLSTS;
    if (reset_flags & RCC_CTRLSTS_IWDGRSTF) {
        SYS_LOG_ERROR("System reset by Independent Watchdog!");
    }
    if (reset_flags & RCC_CTRLSTS_WWDGRSTF) {
        SYS_LOG_ERROR("System reset by Window Watchdog!");
    }
    if (reset_flags & RCC_CTRLSTS_SFTRSTF) {
        SYS_LOG_INFO("System reset by Software Reset");
    }
    if (reset_flags & RCC_CTRLSTS_PORRSTF) {
        SYS_LOG_INFO("System reset by Power-On Reset");
    }
    if (reset_flags & RCC_CTRLSTS_PINRSTF) {
        SYS_LOG_INFO("System reset by Pin Reset");
    }
    
    /* 清除复位标志 */
    RCC->CTRLSTS |= RCC_CTRLSTS_RMRSTF;

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
    TickType_t last_watchdog_feed = xTaskGetTickCount();
    
    for (;;) {
        /* call LED handler - implementation in led.c */
        LED_Flash();
        
        /* 每2秒喂一次看门狗 (看门狗超时时间约31秒，留足安全余量) */
        TickType_t current_time = xTaskGetTickCount();
        if ((current_time - last_watchdog_feed) >= pdMS_TO_TICKS(2000)) {
            MACRO_IWDG_RELOAD();
            last_watchdog_feed = current_time;
            SYS_LOG_DEBUG("Watchdog fed at tick %lu", current_time);
        }
        
        vTaskDelay(delay);
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


