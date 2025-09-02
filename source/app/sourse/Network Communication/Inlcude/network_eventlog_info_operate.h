#ifndef _NETWORK_EVENT_LOG_INFO_OPERATE_H_
#define _NETWORK_EVENT_LOG_INFO_OPERATE_H_

#include <stdint.h>
#include "net_system_info.h"

#define READ_OPERATE_LOG_BY_UART
//#define SAVE_OPERATE_LOG_BY_LOACL

#define SAVE_LOACL_OPERATE_LOG

#define cb_max_alarm_fault_code_len         100
#define cb_event_log_len                    64
#define cb_max_str_description              46
#define cb_event_log_flash_save_len         cb_event_log_len

typedef enum
{
    event_log_type_status = 0,
    event_log_type_abnormal,
    event_log_type_operate,
    event_log_type_wcm,
}network_event_log_type;

typedef enum
{
    event_log_description_type_hex_little_endain = 0,
    event_log_description_type_hex_big_endain,
    event_log_description_type_ascii,
}network_event_log_description_type;

typedef enum
{
    cw_wcm_event_code_device_online     = 0,
    cw_wcm_event_code_device_offline    = 1,
    cw_wcm_event_code_wcm_init          = 4,
    cw_wcm_event_code_wcm_flash_test    = 5,
}wcm_event_code;

typedef struct
{
    uint32_t dw_event_log_save_start_addr;
    uint32_t dw_event_log_save_end_addr;
    uint32_t dw_event_log_start_index;
    uint32_t dw_event_log_end_index;
    uint16_t w_event_log_inquire_index;
    uint16_t w_operate_log_inquire_index;
}event_log_manage_info;

typedef union
{
    uint8_t ba_data[cb_event_log_len];
    struct{
        uint8_t b_event_log_len;
        uint8_t ba_time_stamp[4];
        uint8_t ba_device_id[4];
        union
        {
            uint8_t ba_data[4];
            struct
            {
                uint8_t ba_event_code[4];
            }event_log_type;
            struct
            {
                uint8_t ba_operate_addr[4];
            }operate_log_type;
        }event_log_data;
        struct
        {
            uint8_t ba_description_len[2];
            uint8_t b_event_type;
            uint8_t ba_str_description[cb_max_str_description];
        }str_description_info;
        uint8_t b_crc_h;
        uint8_t b_crc_l;
        //crc
    }event_log_fomat;
}event_log_info;

extern event_log_manage_info st_event_log_manage_info;

void device_event_log_init();
void network_abnormal_code_fresh();
void network_event_log_polling_func(net_device_info * const pst_net_device_info);

uint8_t read_event_log(uint8_t *pb_data_out,
                       uint16_t *pw_data_out_len,
                       uint32_t dw_read_log_index,
                       uint16_t w_read_log_num,
                       uint16_t w_max_data_out_len);

uint8_t fill_event_log(uint32_t dw_device_id,
                       network_event_log_type enum_event_log_type,
                       uint8_t *pb_description,
                       uint8_t b_description_len,
                       uint16_t w_event_code,
                       uint32_t dw_timestamp);
uint8_t event_operate_log_save(uint32_t dw_device_id,
                               uint32_t dw_timestamp,
                               uint16_t w_operate_addr,
                               uint8_t *pb_set_value,
                               uint8_t b_set_value_len,
                               uint8_t b_set_value_type);

#endif

