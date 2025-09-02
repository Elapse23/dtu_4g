#include "net_device_data_kinergy_ii.h"
#include "system_info_general_macro.h"

#define cw_net_status_data_number_kinergy_ii  		    92
#define cw_net_set_data_number_kinergy_ii  		        0
#define cw_net_control_data_number_kinergy_ii  	        0
#define cw_net_data_list_length_kinergy_ii    		    1

static net_status_data_kinergy_ii st_net_status_data_kinergy_ii =
{
    .w_0149 = 0x0141,
};
const net_data_info st_net_status_data_info_kinergy_ii[cw_net_status_data_number_kinergy_ii] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.ba_0100[0],&st_net_data_att_r_100_1b_1_status,},
    {0x0101,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.ba_0101[0],&st_net_data_att_r_100_1b_1_status,},
    {0x0102,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0102,&st_net_data_att_r_2_2b_w_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0103,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.dw_0103,&st_net_data_att_r_4_4b_0_0_1_ua_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0104,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0104,&st_net_data_att_r_2_2b_w_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0105,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0105,&st_net_data_att_r_2_2b_w_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0106,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0106,&st_net_data_att_r_2_2b_w_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0107,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0107,&st_net_data_att_r_2_2b_0_0_1_a_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0108,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0108,&st_net_data_att_r_2_2b_0_0_1_a_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0109,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0109,&st_net_data_att_r_2_2b_0_0_1_a_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x010A,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_010A,&st_net_data_att_r_2_2b_1_0_uva_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x010B,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_010B,&st_net_data_att_r_2_2b_1_0_uva_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x010C,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_010C,&st_net_data_att_r_2_2b_1_0_uva_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x010D,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_010D,&st_net_data_att_r_2_2b_w_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x010E,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_010E,&st_net_data_att_r_2_2b_w_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x010F,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_010F,&st_net_data_att_r_2_2b_w_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},

    {0x0110,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0110,&st_net_data_att_r_2_2b_w_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0111,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0111,&st_net_data_att_r_2_2b_0_0_1_a_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0112,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0112,&st_net_data_att_r_2_2b_0_0_1_a_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0113,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0113,&st_net_data_att_r_2_2b_0_0_1_a_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0114,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0114,&st_net_data_att_r_2_2b_w_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0115,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0115,&st_net_data_att_r_2_2b_w_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0116,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0116,&st_net_data_att_r_2_2b_w_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0117,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0117,&st_net_data_att_r_2_2b_0_0_1_a_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0118,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0118,&st_net_data_att_r_2_2b_0_0_1_a_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0119,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0119,&st_net_data_att_r_2_2b_0_0_1_a_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x011A,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_011A,&st_net_data_att_r_2_2b_1_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x011B,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_011B,&st_net_data_att_r_2_2b_1_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x011C,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.b_011C,&st_net_data_att_r_1_1b_1_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x011D,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.dw_011D,&st_net_data_att_r_4_4b_1_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x011E,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.dw_011E,&st_net_data_att_r_4_4b_1_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x011F,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_011F,&st_net_data_att_r_2_2b_w_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},

    {0x0120,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.b_0120,&st_net_data_att_r_1_1b_1_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0121,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.b_0121,&st_net_data_att_r_1_1b_per_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0122,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.b_0122,&st_net_data_att_r_1_1b_per_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0123,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0123,&st_net_data_att_r_2_2b_C_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0124,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0124,&st_net_data_att_r_2_2b_1_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0125,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0125,&st_net_data_att_r_2_2b_1_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0126,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0126,&st_net_data_att_r_2_2b_0_1_ukwh_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0127,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0127,&st_net_data_att_r_2_2b_0_1_ukwh_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0128,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0128,&st_net_data_att_r_2_2b_0_1_ukwh_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0129,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0129,&st_net_data_att_r_2_2b_1_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x012A,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_012A,&st_net_data_att_r_2_2b_0_1_ukwh_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x012B,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_012B,&st_net_data_att_r_2_2b_ah_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x012C,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_012C,&st_net_data_att_r_2_2b_0_1_ukwh_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x012D,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_012D,&st_net_data_att_r_2_2b_1_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x012E,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_012E,&st_net_data_att_r_2_2b_0_1_ukwh_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x012F,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_012F,&st_net_data_att_r_2_2b_0_1_ukwh_status,//res
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},

    {0x0130,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0130,&st_net_data_att_r_2_2b_0_1_ukwh_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0131,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0131,&st_net_data_att_r_2_2b_1_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0132,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0132,&st_net_data_att_r_2_2b_0_1_ukwh_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0133,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0133,&st_net_data_att_r_2_2b_0_1_ukwh_status,//res
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0134,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0134,&st_net_data_att_r_2_2b_0_1_ukwh_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0135,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0135,&st_net_data_att_r_2_2b_0_1_ukwh_status,//res
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0136,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0136,&st_net_data_att_r_2_2b_0_1_ukwh_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0137,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0137,&st_net_data_att_r_2_2b_0_1_ukwh_status,//res
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0138,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0138,&st_net_data_att_r_2_2b_0_1_ukwh_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0139,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0139,&st_net_data_att_r_2_2b_0_1_ukwh_status,//res
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x013A,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_013A,&st_net_data_att_r_2_2b_w_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x013B,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_013B,&st_net_data_att_r_2_2b_umv_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x013C,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_013C,&st_net_data_att_r_2_2b_0_1_a_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x013D,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.b_013D,&st_net_data_att_r_1_1b_1_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x013E,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_013E,&st_net_data_att_r_2_2b_0_1_ukwh_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x013F,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_013F,&st_net_data_att_r_2_2b_0_1_ukwh_status,//res
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},

    {0x0140,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0140,&st_net_data_att_r_2_2b_0_1_ukwh_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0141,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0141,&st_net_data_att_r_2_2b_0_1_ukwh_status,//res
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0142,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0142,&st_net_data_att_r_2_2b_0_1_ukwh_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0143,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0143,&st_net_data_att_r_2_2b_1_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0144,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0144,&st_net_data_att_r_2_2b_w_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0145,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0145,&st_net_data_att_r_2_2b_0_1_ukwh_status,//res
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0146,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0146,&st_net_data_att_r_2_2b_0_1_ukwh_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0147,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0147,&st_net_data_att_r_2_2b_0_1_ukwh_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0148,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0148,&st_net_data_att_r_2_2b_0_1_ukwh_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0149,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0149,&st_net_data_att_r_2_2b_1_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x014A,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.ba_014A[0],&st_net_data_att_r_12_1b_1_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x014B,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_014B,&st_net_data_att_r_2_2b_1_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x014C,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.b_014C,&st_net_data_att_r_1_1b_1_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x014D,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.b_014D,&st_net_data_att_r_1_1b_1_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x014E,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.dw_014E,&st_net_data_att_r_4_4b_1_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x014F,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_014F,&st_net_data_att_r_2_2b_1_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},

    {0x0150,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0150,&st_net_data_att_r_2_2b_0_1_ukwh_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0151,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0151,&st_net_data_att_r_2_2b_0_1_ukwh_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0152,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0152,&st_net_data_att_r_2_2b_0_1_ukwh_status,
        .st_net_data_offline_config.b_offline_send_enable = TRUE,.st_net_data_offline_config.b_interval_s = 10,},
    {0x0153,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.dw_0153,&st_net_data_att_r_4_4b_1_status,},//res
    {0x0154,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0154,&st_net_data_att_r_2_2b_w_status,},
    {0x0155,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0155,&st_net_data_att_r_2_2b_w_status,},
    {0x0156,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0156,&st_net_data_att_r_2_2b_w_status,},
    {0x0157,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0157,&st_net_data_att_r_2_2b_w_status,},
    {0x0158,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0158,&st_net_data_att_r_2_2b_0_1_v_status,},
    {0x0159,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_0158,&st_net_data_att_r_2_2b_0_1_v_status,},
    {0x015A,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_015A,&st_net_data_att_r_2_2b_0_1_v_status,},
    {0x015B,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_015B,&st_net_data_att_r_2_2b_0_1_ukwh_status,},
//    {0x015C,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_015C,&st_net_data_att_r_2_2b_1_status,},
//    {0x015D,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_015D,&st_net_data_att_r_2_2b_1_status,},
//    {0x015E,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_015E,&st_net_data_att_r_2_2b_1_status,},
//    {0x015F,0x0000,(uint8_t *)&st_net_status_data_kinergy_ii.w_015F,&st_net_data_att_r_2_2b_1_status,},
};

//static net_set_data_kenergy_ii st_net_set_data_kinergy_ii = {0,};
//net_data_info st_net_set_data_info_kinergy_ii[cw_net_set_data_number_kinergy_ii] =
//{
//};
//
//net_data_info st_net_control_data_info_kinergy_ii[cw_net_control_data_number_kinergy_ii] =
//{
//};

const net_data_list st_net_data_list_kinergy_ii[cw_net_data_list_length_kinergy_ii] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_kinergy_ii,
        &st_net_status_data_info_kinergy_ii[0],
    },
//	{
//		cb_net_data_att_set,
//		cw_net_set_data_number_kinergy_ii,
//		&st_net_set_data_info_kinergy_ii[0],
//	},
//	{
//		cb_net_data_att_control,
//		cw_net_control_data_number_kinergy_ii,
//		&st_net_control_data_info_kinergy_ii[0],
//	}
};

const net_data_list_info st_net_data_list_info_kinergy_ii =
{
    cw_net_data_list_length_kinergy_ii,
    &st_net_data_list_kinergy_ii[0],
};
