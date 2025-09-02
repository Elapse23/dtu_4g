#ifndef _DISPLAY_MANAGEMENT_H_
#define _DISPLAY_MANAGEMENT_H_

#include "led.h"
#include "key_operate.h"

//#define cb_ap_connect_flag_bit          0x01
//#define cb_sever_connect_flag_bit       0x02;
//#define cb_sever_com_flag_bit           0x04;
//
//#define cb_device_loss_flag             0x01;
//#define cb_device_add_flag              0x02;
//#define cb_device_com_flag              0x04;

typedef enum
{
    mode_none = 0,
    key_config_mode,
    ble_config_mode,
    normal_mode,
}display_mode_enum_type;

typedef enum
{
    para_none = 0,
    ap_connect,
    ap_disconnect,
    sever_connecting,
    sever_connect,
    sever_disconnect,
    sever_com,
}sever_parameter;

typedef enum
{
    device_loss = 1,
    device_add,
    device_com,
}device_parameter;

typedef union
{
    uint8_t b_parameter;
    sever_parameter sever_para;
    device_parameter device_para;
}display_msg_para;

typedef struct
{
    display_mode_enum_type display_enum;
    led_type led;
    display_msg_para msg_para;
}display_management_msg;


void display_management_task_init();
uint8_t display_management_parameter_send(display_mode_enum_type display_enum,
                                         led_type led,
                                         uint8_t msg_parameter,
                                         uint8_t f_restore_enable);

void product_test_led_ctrl();





#endif
