/*
 * @Author: 23Elapse userszy@163.com
 * @Date: 2025-09-03 10:56:24
 * @LastEditors: 23Elapse userszy@163.com
 * @LastEditTime: 2025-09-03 15:30:00
 * @FilePath: \DTU_4G\Project\MDK_KinergyII4G\Source\sdk\04_bsp\led.h
 * @Description: LED控制头文件 - 简化命名版本
 * 
 * Copyright (c) 2025 by 23Elapse userszy@163.com, All Rights Reserved. 
 */
#ifndef _LED_H_
#define _LED_H_

#include <stdint.h>
#include "n32g45x.h"
#include "FreeRTOS.h"
#include "semphr.h"

// LED 闪烁类型枚举
typedef enum
{
    LED_NONE = 0,
    LED_NET_OFF,        // 网络LED关闭
    LED_COM_OFF,        // 通信LED关闭
    LED_NET_ON,         // 网络LED开启
    LED_COM_ON,         // 通信LED开启
    LED_NET_GREEN,      // 网络绿灯
    LED_NET_RED,        // 网络红灯
    LED_COM_GREEN,      // 通信绿灯
    LED_COM_RED,        // 通信红灯
} LED_FlashType_t;

// LED 类型枚举
typedef enum
{
    LED_TYPE_NONE = 0,
    LED_TYPE_NET,       // 网络LED
    LED_TYPE_COM,       // 通信LED
} LED_Type_t;

// 网络LED硬件配置
#define NET_GREEN_PORT          GPIOB
#define NET_GREEN_PIN           GPIO_PIN_1
#define NET_GREEN_CLK           RCC_APB2_PERIPH_GPIOB

#define NET_RED_PORT            GPIOB
#define NET_RED_PIN             GPIO_PIN_0
#define NET_RED_CLK             RCC_APB2_PERIPH_GPIOB

// 通信LED硬件配置
#define COM_GREEN_PORT          GPIOC
#define COM_GREEN_PIN           GPIO_PIN_5
#define COM_GREEN_CLK           RCC_APB2_PERIPH_GPIOC

#define COM_RED_PORT            GPIOC
#define COM_RED_PIN             GPIO_PIN_4
#define COM_RED_CLK             RCC_APB2_PERIPH_GPIOC

// LED控制宏定义（低电平点亮）
#define NET_GREEN_ON()          GPIO_ResetBits(NET_GREEN_PORT, NET_GREEN_PIN)
#define NET_GREEN_OFF()         GPIO_SetBits(NET_GREEN_PORT, NET_GREEN_PIN)
#define NET_RED_ON()            GPIO_ResetBits(NET_RED_PORT, NET_RED_PIN)
#define NET_RED_OFF()           GPIO_SetBits(NET_RED_PORT, NET_RED_PIN)

#define COM_GREEN_ON()          GPIO_ResetBits(COM_GREEN_PORT, COM_GREEN_PIN)
#define COM_GREEN_OFF()         GPIO_SetBits(COM_GREEN_PORT, COM_GREEN_PIN)
#define COM_RED_ON()            GPIO_ResetBits(COM_RED_PORT, COM_RED_PIN)
#define COM_RED_OFF()           GPIO_SetBits(COM_RED_PORT, COM_RED_PIN)

// 便捷宏定义
#define NET_LED_OFF()           do { NET_GREEN_OFF(); NET_RED_OFF(); } while(0)
#define COM_LED_OFF()           do { COM_GREEN_OFF(); COM_RED_OFF(); } while(0)
#define ALL_LED_OFF()           do { NET_LED_OFF(); COM_LED_OFF(); } while(0)

// 函数声明
void LED_Init(void);
void LED_Flash(void);
void LED_FlashCtrl(LED_Type_t led_type,
                   uint32_t interval,
                   uint16_t flash_count,
                   LED_FlashType_t flash_type,
                   LED_FlashType_t next_type);

#endif /* _LED_H_ */
