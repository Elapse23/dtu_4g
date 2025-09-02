#include "product_test.h"
#include "system_info_general_macro.h"
#include "rs485.h"
#include "portmacro.h"
#include "freertos_user_config.h"
#include "crc16_arithmetic.h"
#include "application_general_macro.h"
#include "rs485_communication_tx_queue.h"
#include "rs485_communicate_protocal.h"
#include <string.h>
#include "wired_network_config_general.h"
#include "net_system_info_list.h"
#include "net_system_info_access.h"
#include "lte_module_config_common.h"
#include "led.h"
#include "lte_module_ble_at_driver.h"
#include "key.h"
#include "application_general_func.h"
#include "network_iot_module_data_channel_manage.h"
#include "network_local_device_secquence_operate.h"
#include "rs485_communication_transmission.h"
#include "Eeprom.h"
#include "network_eventlog_info_operate.h"
#include "display_management.h"
#include "net_device_data_access.h"
#include "network_local_config.h"
#include "net_device_tree_build.h"

typedef enum
{
    cb_cmd_type_rs485 = 0,
    cb_cmd_type_can,
}product_test_cmd_type;

typedef struct
{
    uint8_t f_product_test;
    uint8_t f_wireless_com;
}product_test_info;

product_test_info st_product_test_info = {0,};

uint8_t check_product_test()
{
    return st_product_test_info.f_product_test;
}

void product_test_wireless_com_comfirm()
{
    st_product_test_info.f_wireless_com = TRUE;
}

uint8_t product_test_key_test(uint8_t *pb_data_in,
                              uint16_t w_data_in_len,
                              uint8_t *pb_data_out,
                              uint16_t *pw_data_out_len,
                              uint16_t w_max_data_out_len)
{
    if((NULL == pb_data_in) ||\
       (0 == w_data_in_len) ||\
       (NULL == pb_data_out) ||\
       (NULL == pw_data_out_len))
    {
        return FALSE;
    }
    uint8_t b_result = TRUE;

    TickType_t x_start_time = xTaskGetTickCount();
    TickType_t x_curent_time = 0;
    TickType_t x_delay_time =  pdMS_TO_TICKS_USER(60000);

    while(RESET == key_scan())
    {
        x_curent_time = xTaskGetTickCount();

        if((x_curent_time - x_start_time) >= x_delay_time)
        {
            b_result = FALSE;
            break;
        }
        else
        {
            vTaskDelay(0);
        }
    }

    {
        pb_data_out[0] = 0;
        pb_data_out[1] = b_result;
        *pw_data_out_len = 2;
    }
    return TRUE;

}

uint8_t product_test_com_test(uint8_t *pb_data_in,
                              uint16_t w_data_in_len,
                              uint8_t *pb_data_out,
                              uint16_t *pw_data_out_len,
                              uint16_t w_max_data_out_len)
{
    if((NULL == pb_data_in) ||\
       (0 == w_data_in_len) ||\
       (NULL == pb_data_out) ||\
       (NULL == pw_data_out_len))
    {
        return FALSE;
    }
    uint8_t b_result = TRUE;

    {
        pb_data_out[0] = 0;
        pb_data_out[1] = b_result;
        *pw_data_out_len = 2;
    }
    return TRUE;

}

uint8_t product_test_led_test(uint8_t *pb_data_in,
                              uint16_t w_data_in_len,
                              uint8_t *pb_data_out,
                              uint16_t *pw_data_out_len,
                              uint16_t w_max_data_out_len)
{
    if((NULL == pb_data_in) ||\
       (0 == w_data_in_len) ||\
       (NULL == pb_data_out) ||\
       (NULL == pw_data_out_len))
    {
        return FALSE;
    }
//    uint8_t b_result = TRUE;

    led_flash();led_flash();led_flash();
//    {
//        pb_data_out[0] = 0;
//        pb_data_out[1] = b_result;
//        *pw_data_out_len = 2;
//    }
//    return TRUE;
    return FALSE;
}
uint8_t product_test_exit_test(uint8_t *pb_data_in,
                               uint16_t w_data_in_len,
                               uint8_t *pb_data_out,
                               uint16_t *pw_data_out_len,
                               uint16_t w_max_data_out_len)
{
    if((NULL == pb_data_in) ||\
       (0 == w_data_in_len) ||\
       (NULL == pb_data_out) ||\
       (NULL == pw_data_out_len))
    {
        return FALSE;
    }
    uint8_t b_result = TRUE;

    {
        st_product_test_info.f_product_test = FALSE;

        display_management_parameter_send(normal_mode,com_led,device_com,TRUE);
        display_management_parameter_send(normal_mode,net_led,sever_com,TRUE);

        #if( PRODUCT_TEST_DISABLE_INQUIRE)
        vTaskResume(device_data_polling_task_handle);
        vTaskResume(device_tree_refresh_task_handle);
        #endif

        #if( PRODUCT_TEST_EXIT_CLEAR_EEPROM)
        sEepromReset();
        #endif
    }
    {
        pb_data_out[0] = 0;
        pb_data_out[1] = (uint8_t)b_result;
        *pw_data_out_len = 2;
    }
    return TRUE;
}
extern SemaphoreHandle_t pst_net_device_exists_state_mutex;

uint8_t product_test_start_test(uint8_t *pb_data_in,
                                uint16_t w_data_in_len,
                                uint8_t *pb_data_out,
                                uint16_t *pw_data_out_len,
                                uint16_t w_max_data_out_len)
{
    if((NULL == pb_data_in) ||\
       (0 == w_data_in_len) ||\
       (NULL == pb_data_out) ||\
       (NULL == pw_data_out_len))
    {
        return FALSE;
    }
    uint8_t b_result = TRUE;

    {
        st_product_test_info.f_product_test = TRUE;
        st_product_test_info.f_wireless_com = FALSE;
        product_test_led_ctrl();
        #if( PRODUCT_TEST_DISABLE_INQUIRE)
		
		if(pst_net_device_exists_state_mutex != 0)
		{
			xSemaphoreTake(pst_net_device_exists_state_mutex, portMAX_DELAY);
		}
        vTaskSuspend(device_data_polling_task_handle);
        vTaskSuspend(device_tree_refresh_task_handle);
		
		if(pst_net_device_exists_state_mutex != 0)
		{
			xSemaphoreGive(pst_net_device_exists_state_mutex);
		}
        #endif
    }
    {
        pb_data_out[0] = 0;
        pb_data_out[1] = (uint8_t)b_result;
        *pw_data_out_len = 2;
    }
    return TRUE;
}

uint8_t product_test_write_sn(uint8_t *pb_data_in,
                              uint16_t w_data_in_len,
                              uint8_t *pb_data_out,
                              uint16_t *pw_data_out_len,
                              uint16_t w_max_data_out_len)
{
    if((NULL == pb_data_in) ||\
       (0 == w_data_in_len) ||\
       (NULL == pb_data_out) ||\
       (NULL == pw_data_out_len))
    {
        return FALSE;
    }
    uint8_t b_result = TRUE;

    {
        b_result &= set_local_device_secquence(pb_data_in,
                                               (uint8_t)w_data_in_len);
    }

    if(TRUE == b_result)
    {
        TickType_t x_start_time = xTaskGetTickCount();
        TickType_t x_curent_time = 0;

        while((TRUE == fWriteEEPage(0)) ||\
              (TRUE == fWriteEEPage(1)) ||\
              (TRUE == fWriteEEPage(2)))
        {
            x_curent_time = xTaskGetTickCount();

            if((x_curent_time - x_start_time) >= pdMS_TO_TICKS_USER(30000))
            {
                b_result = FALSE;
                break;
            }
            else
            {
                vTaskDelay(100);
            }
        }
    }

    {
        pb_data_out[0] = 0;
        pb_data_out[1] = (uint8_t)b_result;
        *pw_data_out_len = 2;
    }
    return TRUE;
}

uint8_t product_test_read_sn(uint8_t *pb_data_in,
                             uint16_t w_data_in_len,
                             uint8_t *pb_data_out,
                             uint16_t *pw_data_out_len,
                             uint16_t w_max_data_out_len)
{
    if((NULL == pb_data_in) ||\
       (NULL == pb_data_out) ||\
       (NULL == pw_data_out_len))
    {
        return FALSE;
    }

    {
        sEepromReadToRam();
        *pw_data_out_len = get_local_device_secquence(pb_data_out,w_max_data_out_len);
    }

    if(0 == *pw_data_out_len)
    {
        pb_data_out[0] = 0;
        pb_data_out[1] = 0;
        *pw_data_out_len = 2;
    }
    return TRUE;
}

uint8_t product_test_flash_test(uint8_t *pb_data_in,
                                uint16_t w_data_in_len,
                                uint8_t *pb_data_out,
                                uint16_t *pw_data_out_len,
                                uint16_t w_max_data_out_len)
{
    if((NULL == pb_data_in) ||\
       (0 == w_data_in_len) ||\
       (NULL == pb_data_out) ||\
       (NULL == pw_data_out_len))
    {
        return FALSE;
    }
    uint8_t b_result = TRUE;

    {
        b_result &= fill_event_log(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                   event_log_type_wcm,
                                   "WCM Flash Test",
                                   14,
                                   cw_wcm_event_code_wcm_flash_test,
                                   0);

    }
    {
        pb_data_out[0] = 0;
        pb_data_out[1] = (uint8_t)b_result;
        *pw_data_out_len = 2;
    }
    return TRUE;
}
uint8_t product_test_ble_test(uint8_t *pb_data_in,
                              uint16_t w_data_in_len,
                              uint8_t *pb_data_out,
                              uint16_t *pw_data_out_len,
                              uint16_t w_max_data_out_len)
{
    if((NULL == pb_data_in) ||\
       (0 == w_data_in_len) ||\
       (NULL == pb_data_out) ||\
       (NULL == pw_data_out_len))
    {
        return FALSE;
    }
    uint8_t b_result = TRUE;

    {
        TickType_t x_start_time = xTaskGetTickCount();
        TickType_t x_curent_time = 0;

        while((0 == dwa_ble_addr[0]) &&\
              (0 == dwa_ble_addr[1]))
        {
            x_curent_time = xTaskGetTickCount();

            if((x_curent_time - x_start_time) >= pdMS_TO_TICKS_USER(30000))
            {
                b_result = FALSE;
                break;
            }
            else
            {
                vTaskDelay(100);
            }
        }
    }

    if((TRUE == b_result) &&\
       (w_max_data_out_len > 10))
    {
        for(uint8_t b_cnt_i = 0;b_cnt_i < 6;b_cnt_i++)
        {
            pb_data_out[b_cnt_i] = (uint8_t)dwa_ble_addr[b_cnt_i];
        }
        pb_data_out[6] = 0xA0;
        pb_data_out[7] = 0x02;
        pb_data_out[8] = 0x01;
        pb_data_out[9] = 0x06;

        *pw_data_out_len = 10;
    }
    else
    {
        pb_data_out[0] = 0;
        pb_data_out[1] = (uint8_t)b_result;
        *pw_data_out_len = 2;
    }
    return TRUE;
}

uint8_t product_test_wifi_test(uint8_t *pb_data_in,
                               uint16_t w_data_in_len,
                               uint8_t *pb_data_out,
                               uint16_t *pw_data_out_len,
                               uint16_t w_max_data_out_len)
{
    if((NULL == pb_data_in) ||\
       (0 == w_data_in_len) ||\
       (NULL == pb_data_out) ||\
       (NULL == pw_data_out_len))
    {
        return FALSE;
    }
    uint8_t b_result = TRUE;

    {
        TickType_t x_start_time = xTaskGetTickCount();
        TickType_t x_curent_time = 0;

        while(FALSE == st_product_test_info.f_wireless_com)
        {
            x_curent_time = xTaskGetTickCount();

            if((x_curent_time - x_start_time) >= pdMS_TO_TICKS_USER(60000))
            {
                uint8_t f_sever_com = 0;
                uint8_t f_ap_connect = 0;
                {
                    uint16_t w_data_len = 1;
                    net_system_data_access(cb_device_data_access_data_type_status,
                                           0x0105,
                                           &w_data_len,
                                           (uint8_t *)&f_sever_com,
                                           cb_device_data_access_operate_type_read);
                    net_system_data_access(cb_device_data_access_data_type_status,
                                           0x0104,
                                           &w_data_len,
                                           (uint8_t *)&f_ap_connect,
                                           cb_device_data_access_operate_type_read);
                }

                if(FALSE == f_ap_connect)
                {
                    b_result = 0x04;
                }
                else if(FALSE == f_sever_com)
                {
                    b_result = 0x05;
                }
                else
                {
                    b_result = 0x07;
                }
                break;
            }
            else
            {
                vTaskDelay(100);
            }
        }
    }

    {
        pb_data_out[0] = 0;
        pb_data_out[1] = (uint8_t)b_result;
        *pw_data_out_len = 2;
    }
    return TRUE;
}

uint8_t product_test_write_ssid_pass(uint8_t *pb_data_in,
                                     uint16_t w_data_in_len,
                                     uint8_t *pb_data_out,
                                     uint16_t *pw_data_out_len,
                                     uint16_t w_max_data_out_len)
{
    if((NULL == pb_data_in) ||\
       (0 == w_data_in_len) ||\
       (NULL == pb_data_out) ||\
       (NULL == pw_data_out_len))
    {
        return FALSE;
    }
    uint8_t b_result = TRUE;

    {
        b_result &= net_ap_info_set(w_data_in_len,
                                    pb_data_in,
                                    FALSE);
    }

    {
        pb_data_out[0] = 0;
        pb_data_out[1] = (uint8_t)b_result;
        *pw_data_out_len = 2;
    }
    return TRUE;
}

uint8_t product_test_eeprom_test(uint8_t *pb_data_in,
                                 uint16_t w_data_in_len,
                                 uint8_t *pb_data_out,
                                 uint16_t *pw_data_out_len,
                                 uint16_t w_max_data_out_len)
{
    if((NULL == pb_data_in) ||\
       (0 == w_data_in_len) ||\
       (NULL == pb_data_out) ||\
       (NULL == pw_data_out_len))
    {
        return FALSE;
    }
    uint8_t b_result = TRUE;

    {
        fWriteEEPage(3) = TRUE;
        fWriteEEPage(4) = TRUE;
    }

    {
        TickType_t x_start_time = xTaskGetTickCount();
        TickType_t x_curent_time = 0;

        while((TRUE == fWriteEEPage(3)) ||\
              (TRUE == fWriteEEPage(4)))
        {
            x_curent_time = xTaskGetTickCount();

            if((x_curent_time - x_start_time) >= pdMS_TO_TICKS_USER(10000))
            {
                b_result = FALSE;
                break;
            }
            else
            {
                vTaskDelay(100);
            }
        }
    }

    {
        pb_data_out[0] = 0;
        pb_data_out[1] = (uint8_t)b_result;
        *pw_data_out_len = 2;
    }
    return TRUE;
}
uint8_t product_test_writ_agent_facturer(uint8_t *pb_data_in,
                                 uint16_t w_data_in_len,
                                 uint8_t *pb_data_out,
                                 uint16_t *pw_data_out_len,
                                 uint16_t w_max_data_out_len)
{
    if((NULL == pb_data_in) ||\
       (0 == w_data_in_len) ||\
       (NULL == pb_data_out) ||\
       (NULL == pw_data_out_len))
    {
        return FALSE;
    }
    uint8_t b_result = TRUE;

    {
    	gw_agent_facturer = pb_data_in[0] << 8 | pb_data_in[1];
        fWriteEEPage(27) = TRUE;
    }

    {
        pb_data_out[0] = pb_data_in[0];
        pb_data_out[1] = pb_data_in[1];
        *pw_data_out_len = 2;
    }
    return TRUE;
}

uint8_t product_test_data_analysis(uint8_t *pb_data_in,
                                   uint16_t w_data_in_len,
                                   uint8_t b_cmd_type,
                                   uint16_t *pw_product_test_addr,
                                   uint8_t *pb_data_out,
                                   uint16_t *pw_data_out_len,
                                   uint16_t w_max_data_out_len)
{
    #define cw_rs485_1_test                             0x0000
    #define cw_ble_mac_inquire                          0x000A
    #define cw_ble_test_comfirm                         0x000F
    #define cw_product_test_start                       0x0016
    #define cw_product_test_exit                        0x0018
    #define cw_product_test_key_test                    0x0019
    #define cw_product_test_com_test                    0x001A
    #define cw_product_test_flash_test                  0x001B
    #define cw_product_test_lte_test                    0x001C
    #define cw_product_test_led_test                    0x001D
    #define cw_product_test_gps_test                    0x001E
    #define cw_product_test_write_sn                    0x001F
    #define cw_product_test_wifi_test                   0x0020
    #define cw_product_test_write_ssid_pass             0x0022
    #define cw_product_test_eeprom_test                 0x002D
    #define cw_product_test_can_test                    0x1000
	#define cw_product_test_write_agent_facturer        0x002C

    if((NULL == pb_data_in) ||\
       (0 == w_data_in_len) ||\
       (NULL == pb_data_out) ||\
       (NULL == pw_data_out_len))
    {
        return FALSE;
    }
    if(cb_cmd_type_can == b_cmd_type)
    {
        #if(product_test_can_enable)
        uint16_t w_product_test_addr = 0;

        bytes_to_int_big_to_little_endain(pb_data_in,(uint32_t *)&w_product_test_addr,2);

        *pw_product_test_addr = w_product_test_addr;
        switch(w_product_test_addr)
        {
            case cw_product_test_key_test:
                {
                    return product_test_key_test(&pb_data_in[2],
                                                 w_data_in_len,
                                                 pb_data_out,
                                                 pw_data_out_len,
                                                 w_max_data_out_len);
                }
            case cw_product_test_com_test:
                {
                    return product_test_com_test(&pb_data_in[2],
                                                 w_data_in_len,
                                                 pb_data_out,
                                                 pw_data_out_len,
                                                 w_max_data_out_len);
                }
            case cw_product_test_led_test:
                {
                    return product_test_led_test(&pb_data_in[2],
                                                 w_data_in_len,
                                                 pb_data_out,
                                                 pw_data_out_len,
                                                 w_max_data_out_len);
                }
            case cw_product_test_flash_test:
                {
                    return product_test_flash_test(&pb_data_in[2],
                                                   w_data_in_len,
                                                   pb_data_out,
                                                   pw_data_out_len,
                                                   w_max_data_out_len);
                }
            case cw_product_test_exit:
                {
                    return product_test_exit_test(&pb_data_in[2],
                                                  w_data_in_len,
                                                  pb_data_out,
                                                  pw_data_out_len,
                                                  w_max_data_out_len);
                }
            default:
                return FALSE;
        }
        #endif
    }
    else if(cb_cmd_type_rs485 == b_cmd_type)
    {
        #if(product_test_rs485_enable)
        uint16_t w_product_test_addr = 0;

        bytes_to_int_big_to_little_endain(pb_data_in,(uint32_t *)&w_product_test_addr,2);

        *pw_product_test_addr = w_product_test_addr;
        w_data_in_len -= 2;
        switch(w_product_test_addr)
        {
            case cw_product_test_start:
                {
                    return product_test_start_test(&pb_data_in[2],
                                                   w_data_in_len,
                                                   pb_data_out,
                                                   pw_data_out_len,
                                                   w_max_data_out_len);
                }
            case cw_product_test_exit:
                {
                    return product_test_exit_test(&pb_data_in[2],
                                                  w_data_in_len,
                                                  pb_data_out,
                                                  pw_data_out_len,
                                                  w_max_data_out_len);
                }
            case cw_product_test_key_test:
                {
                    return product_test_key_test(&pb_data_in[2],
                                                 w_data_in_len,
                                                 pb_data_out,
                                                 pw_data_out_len,
                                                 w_max_data_out_len);
                }
            case cw_rs485_1_test:
            case cw_product_test_com_test:
                {
                    return product_test_com_test(&pb_data_in[2],
                                                 w_data_in_len,
                                                 pb_data_out,
                                                 pw_data_out_len,
                                                 w_max_data_out_len);
                }
//            case cw_product_test_led_test:
//                {
//                    return product_test_led_test(&pb_data_in[2],
//                                                 w_data_in_len,
//                                                 pb_data_out,
//                                                 pw_data_out_len,
//                                                 w_max_data_out_len);
//                }
            case cw_product_test_write_sn:
                {
                    return product_test_write_sn(&pb_data_in[2],
                                                 w_data_in_len,
                                                 pb_data_out,
                                                 pw_data_out_len,
                                                 w_max_data_out_len);
                }
            case cw_ble_mac_inquire:
                {
                    return product_test_ble_test(&pb_data_in[2],
                                                 w_data_in_len,
                                                 pb_data_out,
                                                 pw_data_out_len,
                                                 w_max_data_out_len);
                }
            case cw_product_test_wifi_test:
                {
                    return product_test_wifi_test(&pb_data_in[2],
                                                  w_data_in_len,
                                                  pb_data_out,
                                                  pw_data_out_len,
                                                  w_max_data_out_len);
                }
            case cw_product_test_write_ssid_pass:
                {
                    return product_test_write_ssid_pass(&pb_data_in[2],
                                                        w_data_in_len,
                                                        pb_data_out,
                                                        pw_data_out_len,
                                                        w_max_data_out_len);
                }
            case cw_product_test_eeprom_test:
                {
                    return product_test_eeprom_test(&pb_data_in[2],
                                                    w_data_in_len,
                                                    pb_data_out,
                                                    pw_data_out_len,
                                                    w_max_data_out_len);
                }
            case cw_product_test_flash_test:
                {
                    return product_test_flash_test(&pb_data_in[2],
                                                   w_data_in_len,
                                                   pb_data_out,
                                                   pw_data_out_len,
                                                   w_max_data_out_len);
                }
			case cw_product_test_write_agent_facturer:
                {
                    return product_test_writ_agent_facturer(&pb_data_in[2],
		                                                   w_data_in_len,
		                                                   pb_data_out,
		                                                   pw_data_out_len,
		                                                   w_max_data_out_len);
                }
            default:
                return FALSE;
        }
        #endif
    }
    return FALSE;
}

uint8_t product_test_inquire_analysis(uint8_t *pb_data_in,
                                      uint16_t w_data_in_len,
                                      uint8_t b_cmd_type,
                                      uint16_t *pw_product_test_addr,
                                      uint8_t *pb_data_out,
                                      uint16_t *pw_data_out_len,
                                      uint16_t w_max_data_out_len)
{
    #define cw_product_test_lte_csq_inquire                 0x0004
    #define cw_product_test_ble_trans_power_inquire         0x0005
    #define cw_product_test_gps_s2nr                        0x0006
    #define cw_product_test_sn_read                         0x001F

    if((NULL == pb_data_in) ||\
       (NULL == pb_data_out) ||\
       (NULL == pw_data_out_len))
    {
        return FALSE;
    }
    if(cb_cmd_type_can == b_cmd_type)
    {
        #if(product_test_can_enable)
        uint16_t w_product_test_addr = 0;

        bytes_to_int_big_to_little_endain(pb_data_in,(uint32_t *)&w_product_test_addr,2);

        *pw_product_test_addr = w_product_test_addr;
        switch(w_product_test_addr)
        {
            default:
                return FALSE;
        }
        #endif
    }
    else if(cb_cmd_type_rs485 == b_cmd_type)
    {
        #if(product_test_rs485_enable)
        uint16_t w_product_test_addr = 0;

        bytes_to_int_big_to_little_endain(pb_data_in,(uint32_t *)&w_product_test_addr,2);

        *pw_product_test_addr = w_product_test_addr;
        w_data_in_len -= 2;
        switch(w_product_test_addr)
        {
            case cw_product_test_sn_read:
                {
                    return product_test_read_sn(&pb_data_in[2],
                                                w_data_in_len,
                                                pb_data_out,
                                                pw_data_out_len,
                                                w_max_data_out_len);
                }
            default:
                return FALSE;
        }
        #endif
    }
    return FALSE;
}

#if(product_test_rs485_enable)
#define cb_product_test_protocal_header     6
#define cw_product_test_cmd_set             0x0010
#define cw_product_test_cmd_inquire         0x0000


uint8_t product_test_analysis(uint32_t dw_device_address,
                              uint8_t b_cmd,
                              uint16_t w_data_addr,
                              uint16_t *pw_data_out_len,
                              uint8_t *pb_data_out,
                              uint16_t w_data_out_buffer_size,
                              uint8_t *pb_receive_state)
{
    uint8_t b_result = FALSE;
    rs485_rx_data_info *pst_rs485_rx_data_info = NULL;

    b_result = rs485_rx_product_test_data_queue_receive((pv_queue_operate_msg *)&pst_rs485_rx_data_info,FALSE);
    if(TRUE == b_result)
    {
        if((NULL == pst_rs485_rx_data_info) ||\
           (NULL == pst_rs485_rx_data_info->pb_data))
        {
            rs485_rx_product_test_data_queue_shift();
            goto ANALYSIS_FAIL;
        }
        {
//            uint16_t w_data_len = 0;
            uint8_t *pb_data = NULL;
            uint16_t w_body_data_len = 0;

            pb_data = pst_rs485_rx_data_info->pb_data;
//            w_data_len = pst_rs485_rx_data_info->w_data_len;
            w_body_data_len = (uint16_t)(((uint16_t)pb_data[4] << 8) |\
                                          (uint16_t)pb_data[5]);
//            if((0 != w_body_data_len) &&\
//               ((w_body_data_len + cb_product_test_protocal_header + 2) <= w_data_len))
            {
                product_test_protocal_info st_product_test_protocal_info = {0,};

                st_product_test_protocal_info.b_com_addr = pb_data[0];
                st_product_test_protocal_info.b_func_code = pb_data[1];
                st_product_test_protocal_info.w_ctrl_addr =\
                    (uint16_t)(((uint16_t)(pb_data[2] << 8)) |\
                               ((uint16_t)pb_data[3]));
                if(cw_product_test_cmd_inquire == w_data_addr)
                {
                    w_body_data_len = 0;
                }
                st_product_test_protocal_info.w_body_data_len = w_body_data_len;
                st_product_test_protocal_info.pb_body_data = &pb_data[6];

                if(((uint8_t)w_data_addr) != st_product_test_protocal_info.b_func_code)
                {
                    return FALSE;
                }
                if(TRUE == check_crc16(pb_data,w_body_data_len + cb_product_test_protocal_header + 2))
                {
                    if(*pw_data_out_len + st_product_test_protocal_info.w_body_data_len + 2 <\
                         w_data_out_buffer_size)
                    {
                        pb_data_out[(*pw_data_out_len)++] =\
                            (uint8_t)(st_product_test_protocal_info.w_ctrl_addr >> 8);
                        pb_data_out[(*pw_data_out_len)++] =\
                            (uint8_t)(st_product_test_protocal_info.w_ctrl_addr);
                        memcpy(&pb_data_out[(*pw_data_out_len)],
                               st_product_test_protocal_info.pb_body_data,
                               st_product_test_protocal_info.w_body_data_len);
                        *pw_data_out_len += st_product_test_protocal_info.w_body_data_len;
                    }
                    else
                    {
                        goto ANALYSIS_FAIL;
                    }

                    goto ANALYSIS_SUCCESS;
                }
            }
        }
        goto ANALYSIS_FAIL;
    }
    else
    {
        return FALSE;
    }

    ANALYSIS_SUCCESS:
    if((NULL != pst_rs485_rx_data_info) &&\
       (TRUE == b_result))
    {
        rs485_rx_product_test_data_queue_shift();
        if(pst_rs485_rx_data_info->pb_data)
        {
            vPortFree(pst_rs485_rx_data_info->pb_data);
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"rs485_rx_data_analysis_4:%x\n",pst_rs485_rx_data_info->pb_data);
            #endif

        }
        vPortFree(pst_rs485_rx_data_info);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"rs485_rx_data_analysis_3:%x\n",pst_rs485_rx_data_info);
        #endif
    }
    *pb_receive_state = cb_wired_network_receive_state_success;
    return TRUE;

    ANALYSIS_FAIL:
    if((NULL != pst_rs485_rx_data_info) &&\
       (TRUE == b_result))
    {
        rs485_rx_product_test_data_queue_shift();
        if(pst_rs485_rx_data_info->pb_data)
        {
            vPortFree(pst_rs485_rx_data_info->pb_data);
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"rs485_rx_data_analysis_4:%x\n",pst_rs485_rx_data_info->pb_data);
            #endif

        }
        vPortFree(pst_rs485_rx_data_info);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"rs485_rx_data_analysis_3:%x\n",pst_rs485_rx_data_info);
        #endif
    }
    *pb_receive_state = cb_wired_network_receive_state_fail;
    return FALSE;

}
void product_test_rs485_channel()
{
    static pfunc_net_device_send_data p_net_device_send_data = 0;
    static pfunc_net_device_receive_data p_net_device_receive_data = 0;

    if((0 == p_net_device_send_data) ||\
       (0 == p_net_device_receive_data))
    {
        if(FALSE == wired_network_data_transmission_channel_get((pfunc_net_device_send_data *)(&p_net_device_send_data),		//wired_network_send_data
                                                               (pfunc_net_device_receive_data *)(&p_net_device_receive_data)))	//wired_network_receive_data
        {
            return;
        }
    }
    if((0 != p_net_device_send_data) &&\
       (0 != p_net_device_receive_data))
    {
        uint8_t b_receive_state = 0;
        uint16_t w_receive_data_len = 0;
        static uint8_t ba_receive_data[200] = {0,};

        if(TRUE == p_net_device_receive_data(cb_wired_network_rs485_cable,
                                             cb_wired_network_product_test_cmd,
                                             0xF6,
                                             cw_product_test_cmd_set,
                                             &w_receive_data_len,
                                             ba_receive_data,
                                             200,
                                             &b_receive_state))
        {
            if(cb_wired_network_receive_state_success == b_receive_state)
            {
                uint8_t ba_send_ack_buff[100] = {0,};
                uint16_t w_send_ack_len = 0;
                uint16_t w_product_test_addr = 0;
                #if(cb_product_test_debug)
                uint16_t w_cnt_i = 0;
                for(w_cnt_i = 0;w_cnt_i < w_receive_data_len;w_cnt_i++)
                {
                    DebugPrintf(TRUE,"%c",ba_receive_data[2 + w_cnt_i]);
                }
                #endif
                if(TRUE ==  product_test_data_analysis(ba_receive_data,
                                                       w_receive_data_len,
                                                       cb_cmd_type_rs485,
                                                       &w_product_test_addr,
                                                       ba_send_ack_buff,
                                                       &w_send_ack_len,
                                                       100))
                {
                    if(TRUE == p_net_device_send_data(cb_wired_network_rs485_cable,
                                                      cb_wired_network_product_test_cmd,
                                                      0xF6,
                                                      w_product_test_addr,
                                                      w_send_ack_len,
                                                      ba_send_ack_buff))
                    {
                    }
                }
            }
            else if(cb_wired_network_receive_state_fail == b_receive_state)
            {
            }
        }
        if(TRUE == p_net_device_receive_data(cb_wired_network_rs485_cable,
                                             cb_wired_network_product_test_inquire_cmd,
                                             0xF6,
                                             cw_product_test_cmd_inquire,
                                             &w_receive_data_len,
                                             ba_receive_data,
                                             200,
                                             &b_receive_state))
        {
            if(cb_wired_network_receive_state_success == b_receive_state)
            {
                uint8_t ba_send_ack_buff[100] = {0,};
                uint16_t w_send_ack_len = 0;
                uint16_t w_product_test_addr = 0;
                #if(cb_product_test_debug)
                uint16_t w_cnt_i = 0;
                for(w_cnt_i = 0;w_cnt_i < w_receive_data_len;w_cnt_i++)
                {
                    DebugPrintf(TRUE,"%c",ba_receive_data[2 + w_cnt_i]);
                }
                #endif
                if(TRUE ==  product_test_inquire_analysis(ba_receive_data,
                                                          w_receive_data_len,
                                                          cb_cmd_type_rs485,
                                                          &w_product_test_addr,
                                                          ba_send_ack_buff,
                                                          &w_send_ack_len,
                                                          100))
                {
                    if(TRUE == p_net_device_send_data(cb_wired_network_rs485_cable,
                                                      cb_wired_network_product_test_inquire_cmd,
                                                      0xF6,
                                                      w_product_test_addr,
                                                      w_send_ack_len,
                                                      ba_send_ack_buff))
                    {
                    }
                }
            }
            else if(cb_wired_network_receive_state_fail == b_receive_state)
            {
            }
        }
    }
}
#endif  //#if(product_test_rs485_enable)

#if(product_test_can_enable)
void product_test_can_channel()
{
    static pfunc_net_device_send_data p_net_device_send_data = 0;
    static pfunc_net_device_receive_data p_net_device_receive_data = 0;

    if((0 == p_net_device_send_data) ||\
       (0 == p_net_device_receive_data))
    {
        if(FALSE == wired_network_data_transmission_channel_get((pfunc_net_device_send_data *)(&p_net_device_send_data),
                                                               (pfunc_net_device_receive_data *)(&p_net_device_receive_data)))
        {
            return;
        }
    }
    if((0 != p_net_device_send_data) &&\
       (0 != p_net_device_receive_data))
    {
        uint8_t b_receive_state = 0;
        uint16_t w_receive_data_len = 0;
        static uint8_t ba_receive_data[2000] = {0,};

        if(TRUE == p_net_device_receive_data(cb_wired_network_can_cable,
                                              cb_wired_network_product_test_cmd,
                                              0x475,
                                              0x0000,
                                              &w_receive_data_len,
                                              ba_receive_data,
                                              2000,
                                              &b_receive_state))
        {
            if(cb_wired_network_receive_state_success == b_receive_state)
            {
                uint8_t ba_send_ack_buff[100] = {0,};
                uint16_t w_send_ack_len = 0;
                uint16_t w_product_test_addr = 0;
                #if(cb_product_test_debug)
                uint16_t w_cnt_i = 0;
                for(w_cnt_i = 0;w_cnt_i < w_receive_data_len;w_cnt_i++)
                {
                    DebugPrintf(TRUE,"%c",ba_receive_data[2 + w_cnt_i]);
                }
                #endif
                if(TRUE ==  product_test_data_analysis(ba_receive_data,
                                                       w_receive_data_len,
                                                       cb_cmd_type_can,
                                                       &w_product_test_addr,
                                                       ba_send_ack_buff,
                                                       &w_send_ack_len,
                                                       100))
                {
                    if(TRUE == p_net_device_send_data(cb_wired_network_can_cable,
                                                      cb_wired_network_product_test_cmd,
                                                      0x476,
                                                      w_product_test_addr,
                                                      w_send_ack_len,
                                                      ba_send_ack_buff))
                    {
                    }
                }
            }
            else if(cb_wired_network_receive_state_fail == b_receive_state)
            {
            }
        }
        if(TRUE == p_net_device_receive_data(cb_wired_network_can_cable,
                                              cb_wired_network_product_test_inquire_cmd,
                                              0x475,
                                              0x0000,
                                              &w_receive_data_len,
                                              ba_receive_data,
                                              2000,
                                              &b_receive_state))
        {
            if(cb_wired_network_receive_state_success == b_receive_state)
            {
                uint8_t ba_send_ack_buff[100] = {0,};
                uint16_t w_send_ack_len = 0;
                uint16_t w_product_test_addr = 0;
                #if(cb_product_test_debug)
                uint16_t w_cnt_i = 0;
                for(w_cnt_i = 0;w_cnt_i < w_receive_data_len;w_cnt_i++)
                {
                    DebugPrintf(TRUE,"%c",ba_receive_data[2 + w_cnt_i]);
                }
                #endif
                if(TRUE ==  product_test_inquire_analysis(ba_receive_data,
                                                          w_receive_data_len,
                                                          cb_cmd_type_can,
                                                          &w_product_test_addr,
                                                          ba_send_ack_buff,
                                                          &w_send_ack_len,
                                                          100))
                {
                    if(TRUE == p_net_device_send_data(cb_wired_network_can_cable,
                                                      cb_wired_network_product_test_inquire_cmd,
                                                      0x476,
                                                      w_product_test_addr,
                                                      w_send_ack_len,
                                                      ba_send_ack_buff))
                    {
                    }
                }
            }
            else if(cb_wired_network_receive_state_fail == b_receive_state)
            {
            }
        }
    }
}


#endif  //#if(product_test_can_enable)
