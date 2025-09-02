#include "net_device_data_pv.h"
#include "system_info_general_macro.h"
//
#define cw_net_status_data_number_pv                    18
#define cw_net_status_data_number_pv_riiosunii  		26
#define cw_net_status_data_number_pv_riiosunii_master   cw_net_status_data_number_pv_riiosunii + 1
#define cw_net_set_data_number_pv  		                0
#define cw_net_control_data_number_pv  	                0
#define cw_device_inquire_list_pv                       1
#define cw_net_data_list_length_pv    		            1

net_status_data_pv st_net_status_data_pv =
{
    0,
};
///*
const net_data_info st_net_status_data_info_pv_1[cw_net_status_data_number_pv] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_pv.w_0100,&st_net_data_att_r_1_1b_1_status,&st_device_data_att_r_2_pv_data,},
    {0x0101,0x0001,(uint8_t *)&st_net_status_data_pv.w_0101,&st_net_data_att_r_1_1b_1_status,},
    {0x0102,0x0002,(uint8_t *)&st_net_status_data_pv.w_0102,&st_net_data_att_r_2_2b_0_1_ukwh_status,},
    {0x0103,0x0003,(uint8_t *)&st_net_status_data_pv.w_0103,&st_net_data_att_r_2_2b_0_1_ukwh_status,},
    {0x0104,0x0004,(uint8_t *)&st_net_status_data_pv.w_0104,&st_net_data_att_r_2_2b_0_0_1hz_status,},
    {0x0105,0x0005,(uint8_t *)&st_net_status_data_pv.w_0105,&st_net_data_att_r_2_2b_w_status,},
    {0x0106,0x0006,(uint8_t *)&st_net_status_data_pv.w_0106,&st_net_data_att_r_2_2b_w_status,},
    {0x0107,0x0007,(uint8_t *)&st_net_status_data_pv.w_0107,&st_net_data_att_r_2_2b_w_status,},
    {0x0108,0x0008,(uint8_t *)&st_net_status_data_pv.w_0108,&st_net_data_att_r_2_2b_w_status,},
    {0x0109,0x0009,(uint8_t *)&st_net_status_data_pv.w_0109,&st_net_data_att_r_2_2b_0_1_v_status,},
    {0x010A,0x000A,(uint8_t *)&st_net_status_data_pv.w_010A,&st_net_data_att_r_2_2b_0_1_v_status,},
    {0x010B,0x000B,(uint8_t *)&st_net_status_data_pv.w_010B,&st_net_data_att_r_2_2b_0_1_v_status,},
    {0x010C,0x000C,(uint8_t *)&st_net_status_data_pv.dw_010C,&st_net_data_att_r_1_1b_1_status,},
    {0x010D,0x000D,(uint8_t *)&st_net_status_data_pv.dw_010D,&st_net_data_att_r_1_1b_1_status,},
    {0x010E,0x000E,(uint8_t *)&st_net_status_data_pv.dw_010E,&st_net_data_att_r_2_2b_w_status,},
    {0x010F,0x000F,(uint8_t *)&st_net_status_data_pv.w_010F,&st_net_data_att_r_1_1b_1_status,},

    {0x0110,0x0010,(uint8_t *)&st_net_status_data_pv.w_0110,&st_net_data_att_r_1_1b_1_status,},
    {0x0111,0x0011,(uint8_t *)&st_net_status_data_pv.w_0111,&st_net_data_att_r_2_2b_w_status,},
};
//    */
///*
const net_data_info st_net_status_data_info_pv_riiosunii_1[cw_net_status_data_number_pv_riiosunii_master] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_pv.w_0100,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_pv_data,},

    {0x0101,0x0001,(uint8_t *)&st_net_status_data_pv.w_0101,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_pv_data,},
    {0x0102,0x0002,(uint8_t *)&st_net_status_data_pv.w_0102,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_pv_data,},
    {0x0103,0x0003,(uint8_t *)&st_net_status_data_pv.w_0103,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_pv_data,},
    {0x0104,0x0004,(uint8_t *)&st_net_status_data_pv.w_0104,&st_net_data_att_r_2_2b_0_1_C_status,&st_device_data_att_r_2_pv_data,},
    {0x0105,0x0005,(uint8_t *)&st_net_status_data_pv.w_0105,&st_net_data_att_r_2_2b_0_0_1hz_status,&st_device_data_att_r_2_pv_data,},
    {0x0106,0x0006,(uint8_t *)&st_net_status_data_pv.w_0106,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_pv_data,},
    {0x0107,0x0007,(uint8_t *)&st_net_status_data_pv.w_0107,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_pv_data,},
    {0x0108,0x0008,(uint8_t *)&st_net_status_data_pv.w_0108,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_pv_data,},
    {0x0109,0x0009,(uint8_t *)&st_net_status_data_pv.w_0109,&st_net_data_att_r_2_2b_0_1_ua_status,&st_device_data_att_r_2_pv_data,},
    {0x010A,0x000A,(uint8_t *)&st_net_status_data_pv.w_010A,&st_net_data_att_r_2_2b_0_1_ua_status,&st_device_data_att_r_2_pv_data,},
    {0x010B,0x000B,(uint8_t *)&st_net_status_data_pv.w_010B,&st_net_data_att_r_2_2b_0_1_ua_status,&st_device_data_att_r_2_pv_data,},
    {0x010C,0x000C,(uint8_t *)&st_net_status_data_pv.dw_010C,&st_net_data_att_r_4_4b_w_status,&st_device_data_att_r_4_pv_data,},
    {0x010D,0x000D,(uint8_t *)&st_net_status_data_pv.dw_010D,&st_net_data_att_r_4_4b_1_0_va_status,&st_device_data_att_r_4_pv_data,},
    {0x010E,0x000E,(uint8_t *)&st_net_status_data_pv.dw_010E,&st_net_data_att_r_4_4b_1_0_va_status,&st_device_data_att_r_4_pv_data,},
    {0x010F,0x000F,(uint8_t *)&st_net_status_data_pv.w_010F,&st_net_data_att_r_2_2b_0_1_ukwh_status,&st_device_data_att_r_2_pv_data,},

    {0x0110,0x0010,(uint8_t *)&st_net_status_data_pv.w_0110,&st_net_data_att_r_2_2b_0_1_ukwh_status,&st_device_data_att_r_2_pv_data,},
    {0x0111,0x0011,(uint8_t *)&st_net_status_data_pv.w_0111,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_pv_data,},
    {0x0112,0x0012,(uint8_t *)&st_net_status_data_pv.w_0112,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_pv_data,},
    {0x0113,0x0013,(uint8_t *)&st_net_status_data_pv.w_0113,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_pv_data,},
    {0x0114,0x0014,(uint8_t *)&st_net_status_data_pv.w_0114,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_pv_data,},
    {0x0115,0x0015,(uint8_t *)&st_net_status_data_pv.w_0115,&st_net_data_att_r_2_2b_0_1_ua_status,&st_device_data_att_r_2_pv_data,},
    {0x0116,0x0016,(uint8_t *)&st_net_status_data_pv.w_0116,&st_net_data_att_r_2_2b_0_1_ua_status,&st_device_data_att_r_2_pv_data,},
    {0x0117,0x0017,(uint8_t *)&st_net_status_data_pv.w_0117,&st_net_data_att_r_2_2b_0_1_ua_status,&st_device_data_att_r_2_pv_data,},
    {0x0118,0x0018,(uint8_t *)&st_net_status_data_pv.w_0118,&st_net_data_att_r_2_2b_0_1_status,&st_device_data_att_r_2_pv_data,},
    {0x0119,0x0019,(uint8_t *)&st_net_status_data_pv.w_0119,&st_net_data_att_r_1_1b_1_status,},
    {0x011A,0x001A,(uint8_t *)&st_net_status_data_pv.w_011A,&st_net_data_att_r_1_1b_1_status,},
//    {0x011B,0x0034,(uint8_t *)&st_net_status_data_pv.w_0134,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_pv_data,},
};
const net_data_info st_net_status_data_info_pv_riiosunii_2[cw_net_status_data_number_pv_riiosunii] =
{
    {0x0101,0x0019,(uint8_t *)&st_net_status_data_pv.w_011C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_pv_data,},
    {0x0102,0x001A,(uint8_t *)&st_net_status_data_pv.w_011D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_pv_data,},
    {0x0103,0x001B,(uint8_t *)&st_net_status_data_pv.w_011E,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_pv_data,},
    {0x0104,0x001C,(uint8_t *)&st_net_status_data_pv.w_011F,&st_net_data_att_r_2_2b_0_1_C_status,&st_device_data_att_r_2_pv_data,},
    {0x0105,0x001D,(uint8_t *)&st_net_status_data_pv.w_0120,&st_net_data_att_r_2_2b_0_0_1hz_status,&st_device_data_att_r_2_pv_data,},
    {0x0106,0x001E,(uint8_t *)&st_net_status_data_pv.w_0121,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_pv_data,},
    {0x0107,0x001F,(uint8_t *)&st_net_status_data_pv.w_0122,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_pv_data,},
    {0x0108,0x0020,(uint8_t *)&st_net_status_data_pv.w_0123,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_pv_data,},
    {0x0109,0x0021,(uint8_t *)&st_net_status_data_pv.w_0122,&st_net_data_att_r_2_2b_0_1_ua_status,&st_device_data_att_r_2_pv_data,},
    {0x010A,0x0022,(uint8_t *)&st_net_status_data_pv.w_0123,&st_net_data_att_r_2_2b_0_1_ua_status,&st_device_data_att_r_2_pv_data,},
    {0x010B,0x0023,(uint8_t *)&st_net_status_data_pv.w_0124,&st_net_data_att_r_2_2b_0_1_ua_status,&st_device_data_att_r_2_pv_data,},
    {0x010C,0x0022,(uint8_t *)&st_net_status_data_pv.dw_0125,&st_net_data_att_r_4_4b_w_status,&st_device_data_att_r_4_pv_data,},
    {0x010D,0x0023,(uint8_t *)&st_net_status_data_pv.dw_0126,&st_net_data_att_r_4_4b_1_0_va_status,&st_device_data_att_r_4_pv_data,},
    {0x010E,0x0024,(uint8_t *)&st_net_status_data_pv.dw_0127,&st_net_data_att_r_4_4b_1_0_va_status,&st_device_data_att_r_4_pv_data,},
    {0x010F,0x0025,(uint8_t *)&st_net_status_data_pv.w_0128,&st_net_data_att_r_2_2b_0_1_ukwh_status,&st_device_data_att_r_2_pv_data,},

    {0x0110,0x0026,(uint8_t *)&st_net_status_data_pv.w_0129,&st_net_data_att_r_2_2b_0_1_ukwh_status,&st_device_data_att_r_2_pv_data,},
    {0x0111,0x0027,(uint8_t *)&st_net_status_data_pv.w_012A,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_pv_data,},
    {0x0112,0x0028,(uint8_t *)&st_net_status_data_pv.w_012B,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_pv_data,},
    {0x0113,0x0029,(uint8_t *)&st_net_status_data_pv.w_012C,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_pv_data,},
    {0x0114,0x002A,(uint8_t *)&st_net_status_data_pv.w_012D,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_pv_data,},
    {0x0115,0x002B,(uint8_t *)&st_net_status_data_pv.w_012E,&st_net_data_att_r_2_2b_0_1_ua_status,&st_device_data_att_r_2_pv_data,},
    {0x0116,0x002C,(uint8_t *)&st_net_status_data_pv.w_012F,&st_net_data_att_r_2_2b_0_1_ua_status,&st_device_data_att_r_2_pv_data,},
    {0x0117,0x002D,(uint8_t *)&st_net_status_data_pv.w_0130,&st_net_data_att_r_2_2b_0_1_ua_status,&st_device_data_att_r_2_pv_data,},
    {0x0118,0x002E,(uint8_t *)&st_net_status_data_pv.w_0131,&st_net_data_att_r_2_2b_0_1_status,&st_device_data_att_r_2_pv_data,},
    {0x0119,0x0000,(uint8_t *)&st_net_status_data_pv.w_0132,&st_net_data_att_r_1_1b_1_status,},
    {0x011A,0x0000,(uint8_t *)&st_net_status_data_pv.w_0133,&st_net_data_att_r_1_1b_1_status,},
//    {0x011B,0x0034,(uint8_t *)&st_net_status_data_pv.w_0134,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_pv_data,},
};
//    */
/////////////////////////////////////////////////////////////////////////
const net_data_list st_net_data_list_pv_1[cw_net_data_list_length_pv] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_pv,
        &st_net_status_data_info_pv_1[0],
    },
};
const net_data_list st_net_data_list_pv_riiosunii_1[cw_net_data_list_length_pv] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_pv_riiosunii_master,
        &st_net_status_data_info_pv_riiosunii_1[0],
    },
};
const net_data_list st_net_data_list_pv_riiosunii_2[cw_net_data_list_length_pv] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_pv_riiosunii,
        &st_net_status_data_info_pv_riiosunii_2[0],
    },
};
//////////////////////////////////////////////////////////////////////////////
device_inquire_list st_device_inquire_list_pv[cw_device_inquire_list_pv] =
{
    {cw_device_cmd_pv_data_read,{0x00,},0,},
};

//////////////////////////////////////////////////////////////////////////////
const net_data_list_info st_net_data_list_info_pv_1 =
{
    cw_net_data_list_length_pv,
    &st_net_data_list_pv_1[0],
    cw_device_inquire_list_pv,
    &st_device_inquire_list_pv[0],
};
const net_data_list_info st_net_data_list_info_pv_riiosunii_1 =
{
    cw_net_data_list_length_pv,
    &st_net_data_list_pv_riiosunii_1[0],
    cw_device_inquire_list_pv,
    &st_device_inquire_list_pv[0],
};
///*
const net_data_list_info st_net_data_list_info_pv_riiosunii_2 =
{
    cw_net_data_list_length_pv,
    &st_net_data_list_pv_riiosunii_2[0],
};
//    */
//////////////ck type pv///////////
const int pb_net_data_list_info_ck_pv[cb_max_pv_num] =
{
    (int)&st_net_data_list_info_pv_1,
    (int)0,
};
/////////////riio pv/////////////
const int pb_net_data_list_info_riiosunii_pv[cb_max_pv_num] =
{
    (int)&st_net_data_list_info_pv_riiosunii_1,
    (int)&st_net_data_list_info_pv_riiosunii_2,
};
