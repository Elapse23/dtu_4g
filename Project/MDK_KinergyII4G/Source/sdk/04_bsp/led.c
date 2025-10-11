/*
 * @Author: 23Elapse userszy@163.com
 * @Date: 2025-09-03 10:56:24
 * @LastEditors: 23Elapse userszy@163.com
 * @LastEditTime: 2025-09-03 11:13:32
 * @FilePath: \DTU_4G\Project\MDK_KinergyII4G\Source\sdk\04_bsp\led.c
 * @Description: LED控制实现文件 - 简化命名版本
 * 
 * Copyright (c) 2025 by 23Elapse userszy@163.com, All Rights Reserved. 
 */

#include "led.h"
#include "n32g45x.h"


// LED闪烁状态结构体
typedef struct
{
    uint8_t is_init;                // 初始化标志
    uint32_t interval;              // 闪烁间隔（0:不切换）
    uint16_t flash_count;           // 闪烁次数（0：不计数）
    LED_FlashType_t flash_type;     // 当前闪烁类型
    LED_FlashType_t next_type;      // 下一个闪烁类型
} LED_FlashState_t;

// LED状态变量
static LED_FlashState_t net_led_state = {0};
static LED_FlashState_t com_led_state = {0};

/**
 * @brief  LED硬件初始化
 */
void led_init(void)
{
    // 使能GPIO时钟
    RCC_EnableAPB2PeriphClk((NET_GREEN_CLK | NET_RED_CLK | COM_GREEN_CLK | COM_RED_CLK), ENABLE);

    GPIO_InitType GPIO_InitStructure;
    
    // 配置GPIO参数
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;

    // 初始化网络绿灯
    GPIO_InitStructure.Pin = NET_GREEN_PIN;
    GPIO_InitPeripheral(NET_GREEN_PORT, &GPIO_InitStructure);

    // 初始化网络红灯
    GPIO_InitStructure.Pin = NET_RED_PIN;
    GPIO_InitPeripheral(NET_RED_PORT, &GPIO_InitStructure);

    // 初始化通信绿灯
    GPIO_InitStructure.Pin = COM_GREEN_PIN;
    GPIO_InitPeripheral(COM_GREEN_PORT, &GPIO_InitStructure);

    // 初始化通信红灯
    GPIO_InitStructure.Pin = COM_RED_PIN;
    GPIO_InitPeripheral(COM_RED_PORT, &GPIO_InitStructure);

    // 初始状态：关闭所有LED
    ALL_LED_OFF();
    
    // 默认显示：网络红灯，通信红灯
    NET_RED_ON();
    COM_RED_ON();
}

/**
 * @brief  LED控制函数
 * @param  flash_type: LED闪烁类型
 */
static void led_control(LED_FlashType_t flash_type)
{
    switch(flash_type)
    {
        case LED_NET_OFF:
            NET_LED_OFF();
            break;
            
        case LED_COM_OFF:
            COM_LED_OFF();
            break;
            
        case LED_NET_ON:
            NET_GREEN_ON();
            NET_RED_ON();
            break;
            
        case LED_COM_ON:
            COM_GREEN_ON();
            COM_RED_ON();
            break;
            
        case LED_NET_GREEN:
            NET_RED_OFF();
            NET_GREEN_ON();
            break;
            
        case LED_NET_RED:
            NET_GREEN_OFF();
            NET_RED_ON();
            break;
            
        case LED_COM_GREEN:
            COM_RED_OFF();
            COM_GREEN_ON();
            break;
            
        case LED_COM_RED:
            COM_GREEN_OFF();
            COM_RED_ON();
            break;
            
        default:
        case LED_NONE:
            break;
    }
}

/**
 * @brief  LED闪烁控制
 * @param  led_type: LED类型
 * @param  interval: 闪烁间隔（毫秒）
 * @param  flash_count: 闪烁次数
 * @param  flash_type: 闪烁类型
 * @param  next_type: 下一个类型
 */
void led_flash_ctrl(LED_Type_t led_type,
                   uint32_t interval,
                   uint16_t flash_count,
                   LED_FlashType_t flash_type,
                   LED_FlashType_t next_type)
{
    LED_FlashState_t* state = NULL;
    
    // 选择对应的LED状态
    switch(led_type)
    {
        case LED_TYPE_NET:
            state = &net_led_state;
            break;
            
        case LED_TYPE_COM:
            state = &com_led_state;
            break;
            
        default:
            return;
    }
    
    // 设置闪烁参数
    state->interval = interval;
    state->flash_count = flash_count;
    state->flash_type = flash_type;
    state->next_type = next_type;
    state->is_init = 1;
    
    // 立即应用当前状态
    led_control(flash_type);
}

/**
 * @brief  LED闪烁处理（需要在定时器或任务中周期调用）
 * @note   为了与原始实现保持一致，使用FreeRTOS时间戳进行精确计时
 */
void led_flash(void)
{
    // 网络LED闪烁处理
    static uint8_t net_state_backup = 1;
    static uint8_t net_state = 0;
    static uint8_t net_timer_init = 1;
    static uint16_t net_flash_count = 0;
    static TickType_t net_start_time = 0;
    
    if(net_led_state.is_init)
    {
        net_timer_init = 1;
        net_led_state.is_init = 0;
        net_flash_count = net_led_state.flash_count;
        net_state = 0;
        net_state_backup = 1;
    }
    
    // 如果闪烁次数已完成且设置了计数限制
    if((0 == net_flash_count) && (0 != net_led_state.flash_count))
    {
        return;
    }
    
    // 如果间隔为0，直接设置状态
    if(0 == net_led_state.interval)
    {
        if(LED_NONE != net_led_state.flash_type)
        {
            led_control(net_led_state.flash_type);
        }
    }
    else
    {
        // 使用FreeRTOS时间戳进行精确计时
        TickType_t net_current_time = 0;
        
        if(net_timer_init)
        {
            net_timer_init = 0;
            net_start_time = xTaskGetTickCount();
        }
        
        net_current_time = xTaskGetTickCount();
        if((net_current_time - net_start_time) >= net_led_state.interval)
        {
            net_timer_init = 1;
            net_state = !net_state;
            if(0 != net_led_state.flash_count)
            {
                net_flash_count--;
            }
        }
    }
    
    // 状态变化时切换LED
    if(net_state != net_state_backup)
    {
        net_state_backup = net_state;
        if(net_state_backup)
        {
            led_control(net_led_state.next_type);
        }
        else
        {
            led_control(net_led_state.flash_type);
        }
    }
    
    // 通信LED闪烁处理
    static uint8_t com_state_backup = 1;
    static uint8_t com_state = 0;
    static uint8_t com_timer_init = 1;
    static uint16_t com_flash_count = 0;
    static TickType_t com_start_time = 0;
    
    if(com_led_state.is_init)
    {
        com_timer_init = 1;
        com_led_state.is_init = 0;
        com_flash_count = com_led_state.flash_count;
        com_state = 0;
        com_state_backup = 1;
    }
    
    // 如果闪烁次数已完成且设置了计数限制
    if((0 == com_flash_count) && (0 != com_led_state.flash_count))
    {
        return;
    }
    
    // 如果间隔为0，直接设置状态
    if(0 == com_led_state.interval)
    {
        if(LED_NONE != com_led_state.flash_type)
        {
            led_control(com_led_state.flash_type);
        }
    }
    else
    {
        // 使用FreeRTOS时间戳进行精确计时
        TickType_t com_current_time = 0;
        
        if(com_timer_init)
        {
            com_timer_init = 0;
            com_start_time = xTaskGetTickCount();
        }
        
        com_current_time = xTaskGetTickCount();
        if((com_current_time - com_start_time) >= com_led_state.interval)
        {
            com_timer_init = 1;
            com_state = !com_state; // 注意：这里修复了原始代码的bug
            if(0 != com_led_state.flash_count)
            {
                com_flash_count--;
            }
        }
    }
    
    // 状态变化时切换LED
    if(com_state != com_state_backup)
    {
        com_state_backup = com_state;
        if(com_state_backup)
        {
            led_control(com_led_state.next_type);
        }
        else
        {
            led_control(com_led_state.flash_type);
        }
    }
}

/**
 * @brief  设置网络LED状态
 * @param  state: LED状态（绿灯/红灯/关闭）
 */
void LED_SetNetState(LED_FlashType_t state)
{
    led_control(state);
}

/**
 * @brief  设置通信LED状态
 * @param  state: LED状态（绿灯/红灯/关闭）
 */
void LED_SetComState(LED_FlashType_t state)
{
    led_control(state);
}

/**
 * @brief  LED自检
 */
void LED_SelfTest(void)
{
    // 所有LED亮2秒
    ALL_LED_OFF();
    NET_GREEN_ON();
    NET_RED_ON();
    COM_GREEN_ON();
    COM_RED_ON();
    
    // 这里需要延时函数，具体实现根据项目需要
    // vTaskDelay(pdMS_TO_TICKS(2000));
    
    // 恢复默认状态
    ALL_LED_OFF();
    NET_RED_ON();
    COM_RED_ON();
}

