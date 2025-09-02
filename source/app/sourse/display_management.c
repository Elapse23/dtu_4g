#include "display_management.h"
#include "freertos_user_config.h"
#include "printf_custom.h"
#include "application_general_macro.h"
#include "display_management_queue.h"
#include "network_ble_communication.h"
#include "Eeprom.h"
#include "network_device_config_sycn.h"
#include "network_local_config.h"
#include "network_system_type_operate.h"
#include "product_test.h"

#define cb_key_operate_mode     0
#define cb_ble_config_mode      1
#define cb_nomal_mode           2

static TaskHandle_t display_management_task_handle = 0;

typedef struct
{
    uint8_t f_restore_enable;
}display_management_general_info;

//static display_management_general_info st_display_management_info = {0,};

uint8_t display_management_parameter_send(display_mode_enum_type display_enum,
                                         led_type led,
                                         uint8_t msg_parameter,
                                         uint8_t f_restore_enable)
{
    display_management_msg *pst_display_management_msg = 0;

    pst_display_management_msg = (display_management_msg *)pvPortMalloc(sizeof(display_management_msg));
    #if(dynamic_memory_request_test)
    DebugPrintf(TRUE,"display_management_parameter_send_0_malloc%x\n",pst_display_management_msg);
    #endif

    if(0 == pst_display_management_msg)
    {
        return FALSE;
    }
    if(mode_none == display_enum)
    {
        vPortFree(pst_display_management_msg);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"display_management_parameter_send_0_free%x\n",pst_display_management_msg);
        #endif

        return FALSE;
    }

    {
        pst_display_management_msg->display_enum = display_enum;
        pst_display_management_msg->led = led;
        pst_display_management_msg->msg_para.b_parameter = msg_parameter;

//        st_display_management_info.f_restore_enable = f_restore_enable;
    }

    if(FALSE == display_management_queue_send((pv_queue_operate_msg *)pst_display_management_msg))
    {
		vPortFree(pst_display_management_msg);
        return FALSE;
    }
    return TRUE;
}


uint8_t key_operate_mode()
{
    if(SET == key_scan())
    {
        uint8_t b_result;

        b_result = key_operate(SET,
                               pdMS_TO_TICKS_USER(2000),
                               pdMS_TO_TICKS_USER(5000),
                               pdMS_TO_TICKS_USER(30000));
        if(2 == b_result)
        {
            sEepromReset();
//            uint8_t ba_data[2] = {0x03,0x78};
//            if(cb_sync_trig_return_ok == network_device_config_sycn_trig(cdw_device_type_ck_inverter ,
//                                                       cb_net_subset_com_ctrl_cmd,
//                                                       0x046D,
//                                                       2,
//                                                       ba_data,
//                                                       1,
//                                                       TRUE,
//                                                       sync_type_single))
//            {
//            }
        }
        else if(1 == b_result)
        {
            //wps Func
            st_network_ap_set_info.f_wps_enable = TRUE;
            st_network_ap_set_info.f_wps_first_connect = FALSE;
            st_network_ap_info_management.f_ssid_modify = TRUE;
            fWriteEEPage(16) = TRUE;
        }
        {
            led_flash_ctrl(net_led,
                           pdMS_TO_TICKS_USER(150),
                           7,
                           net_green_led,
                           net_led_reset);
            led_flash_ctrl(com_led,
                           pdMS_TO_TICKS_USER(150),
                           6,
                           com_led_reset,
                           com_green_led);
            vTaskDelay(2700);
        }
        display_management_parameter_send(normal_mode,led_type_none,para_none,TRUE);
        return TRUE;
    }
    return FALSE;
}

uint8_t ble_config_mode_display()
{
//    if(st_display_parameter.b_ble_mode)
//    {
//        led_flash_ctrl(net_led,
//                       0,
//                       0,
//                       net_green_led,
//                       net_green_led);
//        led_flash_ctrl(com_led,
//                       0,
//                       0,
//                       com_red_led,
//                       com_red_led);
//        return TRUE;
//    }
    return FALSE;
}

void normal_mode_led_ctrl(display_management_msg *pst_display_management_msg)
{
    static uint8_t msg_para_sever_back_up = para_none;
    static uint8_t msg_para_device_back_up = para_none;
	static uint8_t msg_para_device_back_up_history = para_none;
    uint8_t msg_para_sever_temp = para_none;

    if((net_led == pst_display_management_msg->led) ||\
       (led_type_none == pst_display_management_msg->led))
    {
        if((msg_para_sever_back_up == pst_display_management_msg->msg_para.sever_para) &&\
           (para_none != pst_display_management_msg->msg_para.sever_para))
        {
            return;
        }
        if(led_type_none != pst_display_management_msg->led)
        {
            msg_para_sever_temp = pst_display_management_msg->msg_para.sever_para;
        }
        else
        {
            msg_para_sever_temp = msg_para_sever_back_up;
        }
        switch(msg_para_sever_temp)
        {
            case ap_connect:
            case sever_disconnect:
            {
                led_flash_ctrl(net_led,
                               0,
                               0,
                               net_red_led,
                               net_red_led);
                msg_para_sever_back_up = msg_para_sever_temp;
            }break;
            case ap_disconnect:
            {
                led_flash_ctrl(net_led,
                               500,
                               0,
                               net_red_led,
                               net_led_reset);
                msg_para_sever_back_up = msg_para_sever_temp;
            }break;
            case sever_connecting:
            {
                led_flash_ctrl(net_led,
                               500,
                               0,
                               net_green_led,
                               net_led_reset);
                msg_para_sever_back_up = msg_para_sever_temp;
            }break;
            case sever_connect:
            {
                led_flash_ctrl(net_led,
                               0,
                               0,
                               net_green_led,
                               net_led_reset);
                msg_para_sever_back_up = msg_para_sever_temp;
            }break;
            case sever_com:
            {
                led_flash_ctrl(net_led,
                               100,
                               2,
                               net_green_led,
                               net_led_reset);
//                if(pdTRUE == xSemaphoreTake(stBinarySemaphore,portMAX_DELAY))
//                {
//                    if(TRUE == st_display_management_info.f_restore_enable)
//                    {
//                        display_management_parameter_send(normal_mode,net_led,msg_para_sever_back_up,TRUE);
//                    }
//                    else
//                    {
//                        st_display_management_info.f_restore_enable = TRUE;
//                    }
//                }
                msg_para_sever_back_up = para_none;
            }break;
            default:
                break;
        }
    }
    if((com_led == pst_display_management_msg->led) ||\
       (led_type_none == pst_display_management_msg->led))
    {
        if((msg_para_device_back_up == pst_display_management_msg->msg_para.device_para) &&\
           (para_none != pst_display_management_msg->msg_para.sever_para))
        {
            return;
        }
        if(led_type_none != pst_display_management_msg->led)
        {
            msg_para_device_back_up = pst_display_management_msg->msg_para.device_para;
        }
        switch(msg_para_device_back_up)
        {
            case device_loss:
            {
                led_flash_ctrl(com_led,
                               500,
                               0,
                               com_red_led,
                               com_led_reset);
            }break;
            case device_add:
            {
                led_flash_ctrl(com_led,
                               0,
                               0,
                               com_led_reset,
                               com_led_reset);
            }break;
            case device_com:
            {
                led_flash_ctrl(com_led,
                               500,
                               0,
                               com_green_led,
                               com_led_reset);
                msg_para_device_back_up = para_none;
            }break;
            default:
                break;
        }
		if (msg_para_device_back_up_history != msg_para_device_back_up )
		{
			extern uint8_t f_reconnect_server;
			if ( msg_para_device_back_up_history == device_loss)
			{
				f_reconnect_server = TRUE;
			}
			msg_para_device_back_up_history = msg_para_device_back_up;
		}
    }
}

void product_test_led_ctrl()
{
    led_flash_ctrl(com_led,
                   500,
                   0,
                   com_green_led,
                   com_red_led);
    led_flash_ctrl(net_led,
                   500,
                   0,
                   net_green_led,
                   net_red_led);
}

uint8_t product_test_display()
{
    display_management_msg *pst_display_management_msg = 0;
    if(TRUE == display_management_queue_receive((pv_queue_operate_msg *)&pst_display_management_msg,FALSE))
    {
        display_management_queue_shift();
        if(pst_display_management_msg)
        {
            vPortFree(pst_display_management_msg);
        }
    }
    return TRUE;
}

uint8_t normal_mode_display()
{
    {
        display_management_msg *pst_display_management_msg = 0;
        if(TRUE == display_management_queue_receive((pv_queue_operate_msg *)&pst_display_management_msg,FALSE))
        {
            if(normal_mode == pst_display_management_msg->display_enum)
            {
                normal_mode_led_ctrl(pst_display_management_msg);
                display_management_queue_shift();
                if(pst_display_management_msg)
                {
                    vPortFree(pst_display_management_msg);
                }
            }
            else
            {
                display_management_queue_shift();//mark
            }
        }
        return TRUE;
    }
}

/************************************************************************************************
* Input: void
*
* Output: void
*
* Logic discription: key operate manage +
*                    led operate manage
*
* run time:
************************************************************************************************/
void display_management()
{
    if(check_product_test())
    {
        product_test_display();
    }
    else if(TRUE == check_system_init(cb_system_init_type_system))
    {
        if(key_operate_mode())
        {
        }
        else if(ble_config_mode_display())
        {
        }
        if(normal_mode_display())
        {
        }
    }
}

void vDisplayManagementTask(void* parameter)
{
    DebugPrintf(TRUE,"%d,vDisplayManagementTask_first_entry\n",__LINE__);

    for(;;)
    {
        display_management();
        vTaskDelay(pdMS_TO_TICKS_USER(10));
    }
}

void display_management_task_init()
{
    key_init();
    led_init();

    BaseType_t xReturn = pdPASS;

    display_management_queue_init();
    xReturn = xTaskCreate( vDisplayManagementTask,"vDisplayManagementTask",display_management_task_stack_size,NULL,display_management_task_priority,&display_management_task_handle);
    if(pdFAIL == xReturn)
    {
        DebugPrintf(TRUE,"%d,vDisplayManagementTask Create Fail\n",__LINE__);
    }
}
