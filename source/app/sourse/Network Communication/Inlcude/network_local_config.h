#ifndef _NETWORK_LOCAL_CONFIG_H_
#define _NETWORK_LOCAL_CONFIG_H_

#include <stdint.h>
#include "network_protocal.h"
#include "network_protocal_data_assembly.h"

#define cb_ap_ssid_len          64
#define cb_ap_pass_len          64
typedef union
{
    uint8_t ba_data[cb_ap_ssid_len + cb_ap_pass_len + 1];
    struct
    {
        uint8_t ba_ssid[cb_ap_ssid_len];
        uint8_t b_connection;
        uint8_t ba_pass[cb_ap_pass_len];
    }st_ap_info;
}network_ap_connect_info;


typedef struct
{
    struct
    {
        uint8_t f_write_bssid;
        union
        {
            uint8_t ba_data[19];
            struct
            {
                uint8_t b_start;
                uint8_t b_bssid1[2];
                uint8_t b_connection1;
                uint8_t b_bssid2[2];
                uint8_t b_connection2;
                uint8_t b_bssid3[2];
                uint8_t b_connection3;
                uint8_t b_bssid4[2];
                uint8_t b_connection4;
                uint8_t b_bssid5[2];
                uint8_t b_connection5;
                uint8_t b_bssid6[2];
                uint8_t b_end;
            }st_ap_bssid;
        }bssid_info;
    }bssid;
    uint8_t b_pci_en;
    uint16_t b_reconn_interval;
    uint8_t b_listen_interval;
    uint8_t b_scan_mode;
    uint16_t b_jap_timeout;
    uint8_t b_pmf;
    uint8_t f_wps_enable;
    uint8_t f_wps_first_connect;
}network_ap_set_info;

typedef enum
{
    cb_ap_connect_normal = 0,
    cb_ap_connect_timeout   ,
    cb_ap_connect_pass_err  ,
    cb_ap_connect_ssid_err  ,
    cb_ap_connect_fail      ,
    cb_ap_connect_null = 0xFF      ,
}network_ap_connect_error_code;

typedef enum
{
    cb_sim_connect_normal = 0,
    cb_sim_connect_timeout   ,
    cb_sim_connect_signal_err  ,
    cb_sim_connect_ssid_err  ,
    cb_sim_connect_fail      ,
    cb_sim_connect_null = 0xFF      ,
}network_sim_connect_error_code;

typedef struct
{
    uint8_t f_ssid_modify;
    uint8_t f_ap_connect;
    uint8_t b_error_code;
    uint8_t f_reconnect_enable;
//    network_ap_connect_info *pst_network_ap_connect_info;
}network_ap_info_management;
typedef struct
{
    uint8_t f_ssid_modify;
    uint8_t f_ap_connect;
    uint8_t b_error_code;
    uint8_t f_reconnect_enable;
//    network_ap_connect_info *pst_network_ap_connect_info;
}network_sim_info_management;

extern network_sim_info_management st_network_sim_info_management;

extern network_ap_connect_info st_network_ap_connect_info;
extern network_ap_set_info st_network_ap_set_info;
extern network_ap_info_management st_network_ap_info_management;
void network_ap_connect_err_code_set(uint8_t b_err_code);
uint8_t check_ap_connect_enable();

uint8_t net_system_set(uint16_t w_set_addr,
                       network_data_info *pst_network_data_info,
                       channel_infomation *pst_channel_infomation,
                       uint16_t w_net_body_len);

uint8_t net_system_ctrl(uint32_t dw_device_id,
                        uint16_t w_data_address,
                        uint32_t dw_secquence,
                        uint16_t w_data_len_in,
                        uint8_t *pb_data_in,
                        channel_infomation *pst_channel_infomation,
                        uint8_t b_channel_index,
                        network_data_send_queue px_network_data_send_queue);
uint8_t net_kinergy_set(const net_data_info *pst_data_info,
                        uint8_t *pb_set_data,
                        uint8_t b_data_len);
uint8_t net_ap_info_set(uint16_t w_data_len_in,uint8_t *pb_data_in,uint8_t f_save_eeprom);

#endif
