#ifndef _DEVICE_DATA_ATTRIBUTE_H_
#define _DEVICE_DATA_ATTRIBUTE_H_

#include <stdint.h>

//cmd type
#define cw_device_cmd_type_read                     0x0300
#define cw_device_cmd_type_write                    0x0600

/*Parameter Read Cmd*/
#define cw_device_cmd_cklcd_para_read               (cw_device_cmd_type_read | 0x0033)
#define cw_device_cmd_ck_para_read                  (cw_device_cmd_type_read | 0x0003)
#define cw_device_cmd_mppt_para_read                (cw_device_cmd_type_read | 0x0043)

#define cw_device_cmd_ck_data_read                  		(cw_device_cmd_type_read | 0x00A0)
#define cw_device_cmd_mppt_data_read                		(cw_device_cmd_type_read | 0x00A1)
#define cw_device_cmd_pv_data_read                  		(cw_device_cmd_type_read | 0x00A2)
#define cw_device_cmd_bgk_data_read                 		(cw_device_cmd_type_read | 0x00A3)
#define cw_device_cmd_riio_ii_data_read             		(cw_device_cmd_type_read | 0x00C0)
#define cw_device_cmd_mppt_riio_ii_data_read        		(cw_device_cmd_type_read | 0x00C1)
#define cw_device_cmd_meter_data_read               		(cw_device_cmd_type_read | 0x00C2)
#define cw_device_cmd_riio_ii_split_phase_data_read 		(cw_device_cmd_type_read | 0x00C3)
#define cw_device_cmd_riio_ii_split_phase_meter_data_read 	(cw_device_cmd_type_read | 0x00C5)

#define cw_device_cmd_event_log_read                (cw_device_cmd_type_read | 0x00B0)
#define cw_device_cmd_operate_log_read              (cw_device_cmd_type_read | 0x00B1)
#define cw_device_cmd_device_software_info_read     (cw_device_cmd_type_read | 0x00AB)		//系统读设备软件信息


#define cw_device_cmd_write_only                    0xFFFF

/*Parameter Write Cmd*/
#define cw_device_cmd_cklcd_para_write               (cw_device_cmd_type_write | 0x0066)
#define cw_device_cmd_ck_para_write                  (cw_device_cmd_type_write | 0x0006)
#define cw_device_cmd_mppt_para_write                (cw_device_cmd_type_write | 0x0046)

#define cw_device_cmd_ck_calibration_para_write      (cw_device_cmd_type_write | 0x000A)
#define cw_device_cmd_mppt_calibration_para_write    (cw_device_cmd_type_write | 0x000B)
#define cw_device_cmd_wcm_network_state_write        (cw_device_cmd_type_write | 0x00F1)

#define cw_device_cmd_read_only                      0xFFFF

/*Parameter Control Cmd*/
#define cw_device_cmd_external_trig_write            (cw_device_cmd_type_write | 0x0000)
#define cw_device_cmd_timing_external_trig_1_write   (cw_device_cmd_type_write | 0x00FF)
#define cw_device_cmd_timing_external_trig_2_write   (cw_device_cmd_type_write | 0x00E4)

/*Inverter auxiliary installation command*/
#define cw_device_cmd_inv_auxiliary_installation_write   (cw_device_cmd_type_write | 0x000F)
#define cw_device_cmd_mppt_auxiliary_installation_write  (cw_device_cmd_type_write | 0x004F)



////bound cmomand type
//#define cb_data_att_status             0
//#define cb_data_att_set                1
//#define cb_data_att_control            2

typedef struct
{
    uint8_t b_len;
    uint16_t w_device_inquire_cmd;
    uint16_t w_device_set_cmd;
//    uint8_t b_bound_cmd_type;
}device_data_attribute;

extern const device_data_attribute st_device_data_att_rw_2_cklcd_para;
extern const device_data_attribute st_device_data_att_rw_4_cklcd_para;
extern const device_data_attribute st_device_data_att_r_15_cklcd_para;
extern const device_data_attribute st_device_data_att_r_2_cklcd_para;

extern const device_data_attribute st_device_data_att_r_2_ck_data;
extern const device_data_attribute st_device_data_att_r_2_riiosunii_data;
extern const device_data_attribute st_device_data_att_r_2_riiosunii_splitphase_data;

extern const device_data_attribute st_device_data_att_rw_2_ck_para;

extern const device_data_attribute st_device_data_att_r_2_mppt_data;
extern const device_data_attribute st_device_data_att_r_2_riio_mppt_data;

extern const device_data_attribute st_device_data_att_r_2_pv_data;
extern const device_data_attribute st_device_data_att_r_4_pv_data;

extern const device_data_attribute st_device_data_att_r_2_riio_meter_data;
extern const device_data_attribute st_device_data_att_r_2_riiosunii_split_phase_meter_data;

extern const device_data_attribute st_device_data_att_w_2_external_trig;
extern const device_data_attribute st_device_data_att_w_30_auxiliary_installation;
extern const device_data_attribute st_device_data_att_w_30_mppt_auxiliary_installation;



#endif
