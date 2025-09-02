#ifndef _NETWORK_ENERGY_STATISTICS_H_
#define _NETWORK_ENERGY_STATISTICS_H_

#include <stdint.h>
#include "net_device_info.h"

typedef enum
{
    data_process_type_ck = 0,
    data_process_type_riiosunii,
}data_process_func_type;
typedef struct
{
    uint16_t w_bat_charge      ;
    uint16_t w_bat_discharge   ;
    uint16_t w_bat_to_grid     ;	//0151
    uint16_t w_bat_to_load     ;
    uint16_t w_sp_total        ;
    uint16_t w_sp_to_bat       ;
    uint16_t w_sp_to_load      ;
    uint16_t w_pv_total        ;
    uint16_t w_pv_to_bat       ;
    uint16_t w_pv_to_load      ;
    uint16_t w_solor_total     ;	//0130
    uint16_t w_solar_to_bat    ;	//0138
    uint16_t w_solar_to_load   ;	//0142	
    uint16_t w_solor_to_grid   ;	//0140
    uint16_t w_acin_total      ;	//0128
    uint16_t w_acin_to_bat     ;	//0134	
    uint16_t w_acin_to_load    ;	//1032
    uint16_t w_acout_total     ;	//0150
    uint16_t w_acout2_total    ;	//
    uint16_t w_acout_ideal     ;	//0126
    uint16_t w_acout_from_bat  ;	//013e
    uint16_t w_acout_feedback  ;	//
    uint16_t w_feedback_total  ;	//0152
    uint16_t w_gen_total       ;
}energy_data;

typedef struct
{
    const uint16_t w_network_addr;
    const uint16_t w_data_len;
    uint8_t *pb_data;
}energy_data_info;


typedef struct
{
    uint16_t w_bat_votage               ;	//013B
    uint16_t w_bat_current              ;	//013C
//    uint16_t w_bat_power                ;
    uint16_t w_bat_temp                 ;	//0123
    uint8_t b_bat_type                  ;	//0120
    uint16_t w_bat_soc_gather           ;	//011A
    uint16_t w_bat_energy_gather        ;	//011B
    uint32_t dw_mppt_output_power_total ;	//0102
    uint32_t dw_mppt_output_current     ;	//0103
    uint16_t w_u_acin_power_sum         ;	//0104
    uint16_t w_v_acin_power_sum         ;	//0105
    uint16_t w_w_acin_power_sum         ;	//0106
    uint16_t w_uvw_acin_power_sum       ;	//013A
    uint16_t w_u_acin_current           ;	//0107
    uint16_t w_v_acin_current           ;	//0108
    uint16_t w_w_acin_current           ;	//0109
    uint16_t w_u_acload_power_p_sum     ;//p-acout1 010D
    uint16_t w_u_acload_power2_p_sum    ;//p-acout2 0157
    uint16_t w_u_acload_power_s_sum     ;//s-acout  010A
    uint16_t w_v_acload_power_p_sum     ;//p-acout1 010E
    uint16_t w_v_acload_power2_p_sum    ;//p-acout2 0158
    uint16_t w_v_acload_power_s_sum     ;//s-acout  010B
    uint16_t w_w_acload_power_p_sum     ;//p-acout1 010F
    uint16_t w_w_acload_power2_p_sum    ;//p-acout2 0159
    uint16_t w_w_acload_power_s_sum     ;//s-acout  010C
    uint16_t w_uvw_acload_power_p_sum   ;	//0110
    uint16_t w_uvw_acload_power2_p_sum  ;	//015D
    uint16_t w_acload_power_p_sum       ;	//0135
    uint16_t w_u_acload_power_p         ;	//0168
    uint16_t w_v_acload_power_p         ;	//0169
    uint16_t w_w_acload_power_p         ;	//016A
    uint16_t w_u_acload_current_sum     ;	//0111
    uint16_t w_v_acload_current_sum     ;	//0112
    uint16_t w_w_acload_current_sum     ;	//0113
    uint16_t w_u_inverter_power_sum     ;	//0114
    uint16_t w_v_inverter_power_sum     ;	//0115
    uint16_t w_w_inverter_power_sum     ;	//0116
    uint16_t w_u_inverter_current       ;	//0117
    uint16_t w_v_inverter_current       ;	//0118
    uint16_t w_w_inverter_current       ;	//0119
    uint16_t w_u_acout1_current         ;	//0171
    uint16_t w_v_acout1_current         ;	//0172
    uint16_t w_w_acout1_current         ;	//0173
    uint16_t w_u_acout2_current         ;	//015A
    uint16_t w_v_acout2_current         ;	//015B
    uint16_t w_w_acout2_current         ;	//015C
    uint32_t dw_solor_output_power      ;	//0144
    uint8_t b_bat_soc                   ;	//0121
    uint8_t b_bat_soh                   ;	//0122
    uint16_t w_gen_power_p_sum          ;	//0154
    uint16_t w_u_gen_power_p_sum        ;	//016B
    uint16_t w_v_gen_power_p_sum        ;	//016C
    uint16_t w_w_gen_power_p_sum        ;	//016D
    uint16_t w_u_gen_power_i_sum        ;	//016E
    uint16_t w_v_gen_power_i_sum        ;	//016F
    uint16_t w_w_gen_power_i_sum        ;	//0170
    uint16_t w_normalload_current       ;	//0174
	uint16_t w_normalload_L1_current    ;	//0175
	uint16_t w_normalload_L2_current    ;	//0176
	uint16_t w_normalload_L1_power      ;	//0177
	uint16_t w_normalload_L2_power      ;	//0178
}power_current_data;

typedef struct
{
    uint16_t w_pv_power_p_total         ;
    uint16_t w_pv_power_p_u             ;
    uint16_t w_pv_power_p_v             ;
    uint16_t w_pv_power_p_w             ;
    uint16_t w_pv_max_votage_u          ;
    uint16_t w_pv_max_votage_v          ;
    uint16_t w_pv_max_votage_w          ;
}pv_data_summary;


extern uint32_t dw_power_saving;
extern uint8_t b_power_saving_start_flag;

uint32_t negative_to_zero_u32(uint32_t dw_data_in);
uint16_t positive_to_negtive(uint16_t w_data_in);

uint8_t check_all_in_one(uint32_t dw_device_id);
uint8_t check_ck_bypass_connect();
void energy_statistics_task_init();
uint8_t network_data_process_func_set(data_process_func_type enum_data_procee_func_in);
void network_ck_type_pv_data_process(net_device_info * const pst_net_device_info_in,
                                     uint8_t *pb_data_in,
                                     uint16_t w_data_len);
uint8_t get_system_type();
uint8_t get_system_parallel_mode();
void system_ck_type_bat_full_charge();




#endif
