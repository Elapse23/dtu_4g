#ifndef _LED_CONTROL_H_
#define _LED_CONTROL_H_


#include <stdint.h>
#include "FreeRTOS.h"
#include "semphr.h"

//extern SemaphoreHandle_t stBinarySemaphore;

typedef enum
{
    led_none=0,
    net_led_reset,
    com_led_reset,
    net_led_set,
    com_led_set,
    net_green_led,
    net_red_led,
    com_green_led,
    com_red_led,
}led_flash_type;

typedef enum
{
    led_type_none=0,
    net_led,
    com_led,
}led_type;

#ifdef KINERGY_II_WIFI
#define net_green_led_GPIO              GPIOC
#define net_red_led_GPIO                GPIOC
#define com_green_led_GPIO              GPIOC
#define com_red_led_GPIO                GPIOC

#define net_green_led_clk               RCC_APB2_PERIPH_GPIOC
#define net_red_led_clk                 RCC_APB2_PERIPH_GPIOC
#define com_green_led_clk               RCC_APB2_PERIPH_GPIOC
#define com_red_led_clk                 RCC_APB2_PERIPH_GPIOC

#define net_green_led_pin               GPIO_PIN_6
#define net_red_led_pin                 GPIO_PIN_7
#define com_green_led_pin               GPIO_PIN_8
#define com_red_led_pin                 GPIO_PIN_9
#else
#define net_green_led_GPIO              GPIOB
#define net_red_led_GPIO                GPIOB
#define com_green_led_GPIO              GPIOC
#define com_red_led_GPIO                GPIOC

#define net_green_led_clk               RCC_APB2_PERIPH_GPIOB
#define net_red_led_clk                 RCC_APB2_PERIPH_GPIOB
#define com_green_led_clk               RCC_APB2_PERIPH_GPIOC
#define com_red_led_clk                 RCC_APB2_PERIPH_GPIOC

#define net_green_led_pin               GPIO_PIN_1
#define net_red_led_pin                 GPIO_PIN_0
#define com_green_led_pin               GPIO_PIN_5
#define com_red_led_pin                 GPIO_PIN_4
#endif

#define net_green_led_set()             GPIO_ResetBits(net_green_led_GPIO,net_green_led_pin)
#define net_green_led_reset()           GPIO_SetBits(net_green_led_GPIO,net_green_led_pin)
#define net_red_led_set()               GPIO_ResetBits(net_red_led_GPIO,net_red_led_pin)
#define net_red_led_reset()             GPIO_SetBits(net_red_led_GPIO,net_red_led_pin)

#define com_green_led_set()             GPIO_ResetBits(com_green_led_GPIO,com_green_led_pin)
#define com_green_led_reset()           GPIO_SetBits(com_green_led_GPIO,com_green_led_pin)
#define com_red_led_set()               GPIO_ResetBits(com_red_led_GPIO,com_red_led_pin)
#define com_red_led_reset()             GPIO_SetBits(com_red_led_GPIO,com_red_led_pin)
void led_init();
void led_flash();
void led_flash_ctrl(led_type b_led_type,
                    uint32_t interval,
                    uint16_t w_flash_cnt,
                    led_flash_type b_led_flash_type,
                    led_flash_type b_led_flash_type_next);

#endif
