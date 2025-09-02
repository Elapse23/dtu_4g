#include "led.h"
#include "n32g45x.h"
#include "portmacro.h"
#include "freertos_user_config.h"
#include "application_general_macro.h"



//SemaphoreHandle_t stBinarySemaphore;

typedef struct
{
    uint8_t f_init;
    uint32_t interval;//0:不切换
    uint16_t w_flash_cnt;//0：不计数
    led_flash_type b_led_flash_type;
    led_flash_type b_led_flash_type_next;
}led_flash_state;

led_flash_state net_led_flash_state = {0};
led_flash_state com_led_flash_state = {0};

void led_init()
{
    RCC_EnableAPB2PeriphClk((net_green_led_clk |\
                             net_red_led_clk |\
                             com_green_led_clk |\
                             com_red_led_clk),ENABLE);

    {
        GPIO_InitType GPIO_InitStructure;

        GPIO_InitStructure.Pin          = net_green_led_pin;
        GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;
        GPIO_InitPeripheral(net_green_led_GPIO, &GPIO_InitStructure);

        GPIO_InitStructure.Pin          = net_red_led_pin;
        GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;
        GPIO_InitPeripheral(net_red_led_GPIO, &GPIO_InitStructure);

        GPIO_InitStructure.Pin          = com_green_led_pin;
        GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;
        GPIO_InitPeripheral(com_green_led_GPIO, &GPIO_InitStructure);

        GPIO_InitStructure.Pin          = com_red_led_pin;
        GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;
        GPIO_InitPeripheral(com_red_led_GPIO, &GPIO_InitStructure);
    }

//    stBinarySemaphore = xSemaphoreCreateBinary();
    net_green_led_reset();
    com_green_led_reset();
    net_red_led_set();
    com_red_led_set();
}

void led_ctrl(led_flash_type b_led_flash_type)
{
    switch(b_led_flash_type)
    {
        case net_led_reset:
        {
            net_green_led_reset();
            net_red_led_reset();
        }break;
        case com_led_reset:
        {
            com_green_led_reset();
            com_red_led_reset();
        }break;
        case net_led_set:
        {
            net_green_led_set();
            net_red_led_set();
        }break;
        case com_led_set:
        {
            com_green_led_set();
            com_red_led_set();
        }break;
        case net_green_led:
        {
            net_red_led_reset();
            net_green_led_set();
        }break;
        case net_red_led:
        {
            net_green_led_reset();
            net_red_led_set();
        }break;
        case com_green_led:
        {
            com_red_led_reset();
            com_green_led_set();
        }break;
        case com_red_led:
        {
            com_green_led_reset();
            com_red_led_set();
        }break;
        default:
            return;
    }
}

void led_flash_ctrl(led_type b_led_type,
                    uint32_t interval,
                    uint16_t w_flash_cnt,
                    led_flash_type b_led_flash_type,
                    led_flash_type b_led_flash_type_next)
{
    led_flash_state *pst_led_flash_state = 0;

    switch(b_led_type)
    {
        case net_led:
        {
            pst_led_flash_state = &net_led_flash_state;
        }break;
        case com_led:
        {
            pst_led_flash_state = &com_led_flash_state;
        }break;
        default:
            return;
    }
    if(0 != pst_led_flash_state)
    {
        pst_led_flash_state->f_init = TRUE;
        pst_led_flash_state->interval = interval;
        pst_led_flash_state->w_flash_cnt = w_flash_cnt;
        pst_led_flash_state->b_led_flash_type = b_led_flash_type;
        pst_led_flash_state->b_led_flash_type_next = b_led_flash_type_next;
    }
}

void net_led_flash()
{
    static uint8_t b_state_backup = 1;
    static uint8_t b_state = 0;
    static uint8_t f_timer_init = TRUE;
    static uint16_t w_flash_cnt = 0;
    if(net_led_flash_state.f_init)
    {
        f_timer_init = TRUE;
        net_led_flash_state.f_init = FALSE;
        w_flash_cnt = net_led_flash_state.w_flash_cnt;
        b_state = 0;
        b_state_backup = 1;
    }
    if((0 == w_flash_cnt) &&\
       (0 != net_led_flash_state.w_flash_cnt))
    {
//        xSemaphoreGive(stBinarySemaphore);
        return;
    }
    if(0 == net_led_flash_state.interval)
    {
        if(led_none != net_led_flash_state.b_led_flash_type)
        {
            led_ctrl(net_led_flash_state.b_led_flash_type);
        }
    }
    else
    {
        static TickType_t x_start_time = 0;
        TickType_t x_curent_time = 0;

        if(f_timer_init)
        {
            f_timer_init = FALSE;
            x_start_time = xTaskGetTickCount();
        }
        x_curent_time = xTaskGetTickCount();
        if((x_curent_time - x_start_time) >= net_led_flash_state.interval)
        {
            f_timer_init = TRUE;
            b_state = !b_state;
            if(0 != net_led_flash_state.w_flash_cnt)
            {
                w_flash_cnt--;
            }
        }
    }
    if(b_state != b_state_backup)
    {
        b_state_backup = b_state;
        if(b_state_backup)
        {
            led_ctrl(net_led_flash_state.b_led_flash_type_next);
        }
        else
        {
            led_ctrl(net_led_flash_state.b_led_flash_type);
        }
    }
}
void com_led_flash()
{
    static uint8_t b_state_backup = 1;
    static uint8_t b_state = 0;
    static uint8_t f_timer_init = TRUE;
    static uint16_t w_flash_cnt = 0;
    if(com_led_flash_state.f_init)
    {
        f_timer_init = TRUE;
        com_led_flash_state.f_init = FALSE;
        w_flash_cnt = com_led_flash_state.w_flash_cnt;
        b_state = 0;
        b_state_backup = 1;
    }
    if((0 == w_flash_cnt) &&\
       (0 != com_led_flash_state.w_flash_cnt))
    {
        return;
    }
    if(0 == com_led_flash_state.interval)
    {
        if(led_none != com_led_flash_state.b_led_flash_type)
        {
            led_ctrl(com_led_flash_state.b_led_flash_type);
        }
    }
    else
    {
        static TickType_t x_start_time = 0;
        TickType_t x_curent_time = 0;

        if(f_timer_init)
        {
            f_timer_init = FALSE;
            x_start_time = xTaskGetTickCount();
        }
        x_curent_time = xTaskGetTickCount();
        if((x_curent_time - x_start_time) >= com_led_flash_state.interval)
        {
            f_timer_init = TRUE;
            b_state = !b_state_backup;
            if(0 != com_led_flash_state.w_flash_cnt)
            {
                w_flash_cnt--;
            }
        }
    }
    if(b_state != b_state_backup)
    {
        b_state_backup = b_state;
        if(b_state_backup)
        {
            led_ctrl(com_led_flash_state.b_led_flash_type_next);
        }
        else
        {
            led_ctrl(com_led_flash_state.b_led_flash_type);
        }
    }
}

void led_flash()
{
    net_led_flash();
    com_led_flash();
}
