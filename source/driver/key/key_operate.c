#include "key_operate.h"
#include "freertos_user_config.h"
#include "application_general_macro.h"

/*
    time > min && < timemax return 1
    time > max
*/
uint8_t key_operate(FlagStatus b_Key_Status,
                    uint32_t dw_min_press_cnt,
                    uint32_t dw_max_press_cnt,
                    uint32_t dw_time_out)
{
    TickType_t x_start_time = xTaskGetTickCount();
    TickType_t x_start_time_1 = 0;
    TickType_t x_curent_time = 0;
    TickType_t x_time_out_time =  pdMS_TO_TICKS_USER(dw_time_out);
    TickType_t x_min_press_time =  pdMS_TO_TICKS_USER(dw_min_press_cnt);
    TickType_t x_max_press_time =  pdMS_TO_TICKS_USER(dw_max_press_cnt);
    uint8_t f_init = TRUE;

    for(;;)
    {
        x_curent_time = xTaskGetTickCount();
        if((b_Key_Status != key_scan()) && f_init)
        {
            if((x_curent_time - x_start_time) >= x_time_out_time)
            {
                break;
            }
        }
        else
        {
            if(f_init)
            {
                f_init = FALSE;
                x_start_time_1 = xTaskGetTickCount();
            }
            if(b_Key_Status == key_scan())
            {
                if((x_curent_time - x_start_time_1) >= x_time_out_time)
                {
                    break;
                }
                else if((x_curent_time - x_start_time_1) >= x_max_press_time)
                {
                    return 2;
                }
                else
                {
                    vTaskDelay(0);
                }
            }
            else
            {
                if(((x_curent_time - x_start_time_1) >= x_min_press_time) &&\
                        ((x_curent_time - x_start_time_1) < x_max_press_time))
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}



