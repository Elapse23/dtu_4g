#ifndef _NETWORK_LOCAL_FIRMWARE_UPDATE_H_
#define _NETWORK_LOCAL_FIRMWARE_UPDATE_H_

#include <stdint.h>
#include "net_firmware_update.h"

#define cw_subset_update_pac_cmd_start_bus                          0x0001
#define cw_subset_update_pac_cmd_identify                           0x0002
#define cw_subset_update_pac_cmd_get_device_tree                    0x0003
#define cw_subset_update_pac_cmd_update_start                       0x0004
#define cw_subset_update_pac_cmd_send_package                       0x0005
#define cw_subset_update_pac_cmd_send_package_over                  0x0006
#define cw_subset_update_pac_cmd_set_sub_buadrate                   0x0008
#define cw_subset_update_pac_cmd_send_lcd_enable                    0x060C
#define cw_subset_update_pac_cmd_send_mppt_enable                   0x0666
#define cw_subset_update_pac_cmd_stop_bus                           0x0000
#define cw_subset_update_pac_cmd_get_comm_addr                      0x0007
#define cw_subset_update_pac_sp_cmd_change_lcd_buadrate             0xF000



#define cw_subset_update_pacx_dsp_cmd_common                        0xF000
#define cw_subset_update_pacx_dsp_cmd_enable_subset                 0x0001
#define cw_subset_update_pacx_dsp_cmd_set_buadrate                  0x0002
#define cw_subset_update_pacx_dsp_cmd_send_key                      0x0003
#define cw_subset_update_pacx_dsp_cmd_send_firmware                 0x0004
#define cw_subset_update_pacx_dsp_cmd_send_firm_over                0x0005
#define cw_subset_update_pacx_dsp_cmd_send_mppt_enable              0x0666



#define cb_firmware_update_status_none                              0
#define cb_firmware_update_status_wait_update                       1
#define cb_firmware_update_status_updating                          2
#define cb_firmware_update_status_firmware_receive                  3

void local_firmware_update_loop();
extern uint8_t b_local_firmware_enable;
extern uint32_t dw_firmware_update_task_delay;
void firmware_update_task_init();
#define FIRMWARE_UPDATE_SET_BUADRATE_9600()     rs485_buadrate_set(cdw_uart_buadrate_9600);\
                                                dw_firmware_update_task_delay = 100
#define FIRMWARE_UPDATE_SET_BUADRATE_115200()   rs485_buadrate_set(cdw_uart_buadrate_115200);\
                                                dw_firmware_update_task_delay = 10
uint8_t firmware_update_pac_packet_info_store(uint16_t w_firmware_packet_len,
                                              firmware_packet *pst_firmware_packet,
                                              device_update_info_current *pst_device_update_info_current);

uint8_t firmware_update_pac_packet_data_store(uint16_t w_firmware_packet_len,
                                              firmware_packet *pst_firmware_packet,
                                              device_update_info_current *pst_device_update_info_current);

uint8_t firmware_update_pacx_dsp_packet_info_store(uint16_t w_firmware_packet_len,
                                                   firmware_packet *pst_firmware_packet,
                                                   device_update_info_current *pst_device_update_info_current);

uint8_t firmware_update_pacx_dsp_packet_data_store(uint16_t w_firmware_packet_len,
                                                   firmware_packet *pst_firmware_packet,
                                                   device_update_info_current *pst_device_update_info_current);

uint8_t local_firmware_update_packet_info_store(uint16_t w_firmware_packet_len,
												firmware_packet *pst_firmware_packet,
                                                device_update_info_current *pst_device_update_info_current);


uint8_t local_firmware_update_packet_data_store(uint16_t w_firmware_packet_len,
                                                firmware_packet *pst_firmware_packet,
                                                device_update_info_current *pst_device_update_info_current);

uint8_t check_system_type(uint8_t w_system_type_in);


#endif
