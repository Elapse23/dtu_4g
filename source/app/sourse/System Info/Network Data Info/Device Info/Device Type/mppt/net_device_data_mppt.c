#include "net_device_data_mppt.h"
#include "system_info_general_macro.h"
//222

#define cw_net_status_data_number_mppt  		        14
#define cw_net_status_data_number_mppt_master           cw_net_status_data_number_mppt + 4

#define cw_net_status_data_number_riio_mppt  		    16
#define cw_net_status_data_number_riio_mppt_master      cw_net_status_data_number_riio_mppt + 4

#define cw_net_set_data_number_mppt  		            0
#define cw_net_control_data_number_mppt  	            0
#define cw_device_inquire_list_mppt                     1
#define cw_net_data_list_length_mppt    		        1

/**************************** MPPT u1 ****************************/
net_status_data_mppt st_net_status_data_mppt_u1 =
{
    0,
};
const net_data_info st_net_status_data_info_mppt_u1_0[cw_net_status_data_number_mppt_master] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_mppt_u1.w_0100,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0101,0x0001,(uint8_t *)&st_net_status_data_mppt_u1.w_0101,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0102,0x0002,(uint8_t *)&st_net_status_data_mppt_u1.w_0102,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0103,0x0003,(uint8_t *)&st_net_status_data_mppt_u1.w_0103,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},

    {0x0104,0x0004,(uint8_t *)&st_net_status_data_mppt_u1.w_0104,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x0005,(uint8_t *)&st_net_status_data_mppt_u1.w_0105,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0006,(uint8_t *)&st_net_status_data_mppt_u1.w_0106,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0007,(uint8_t *)&st_net_status_data_mppt_u1.w_0107,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0008,(uint8_t *)&st_net_status_data_mppt_u1.w_0108,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0009,(uint8_t *)&st_net_status_data_mppt_u1.w_0109,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x000A,(uint8_t *)&st_net_status_data_mppt_u1.w_010A,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x000B,(uint8_t *)&st_net_status_data_mppt_u1.w_010B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x000C,(uint8_t *)&st_net_status_data_mppt_u1.w_010C,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x000D,(uint8_t *)&st_net_status_data_mppt_u1.w_010D,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x000E,(uint8_t *)&st_net_status_data_mppt_u1.w_010E,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x000F,(uint8_t *)&st_net_status_data_mppt_u1.w_010F,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x0010,(uint8_t *)&st_net_status_data_mppt_u1.w_0110,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x0011,(uint8_t *)&st_net_status_data_mppt_u1.w_0111,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
const net_data_info st_net_status_data_info_mppt_u1_1[cw_net_status_data_number_mppt] =
{
    {0x0104,0x0012,(uint8_t *)&st_net_status_data_mppt_u1.w_0112,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x0013,(uint8_t *)&st_net_status_data_mppt_u1.w_0113,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0014,(uint8_t *)&st_net_status_data_mppt_u1.w_0114,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0015,(uint8_t *)&st_net_status_data_mppt_u1.w_0115,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0016,(uint8_t *)&st_net_status_data_mppt_u1.w_0116,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0017,(uint8_t *)&st_net_status_data_mppt_u1.w_0117,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0018,(uint8_t *)&st_net_status_data_mppt_u1.w_0118,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0019,(uint8_t *)&st_net_status_data_mppt_u1.w_0119,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x001A,(uint8_t *)&st_net_status_data_mppt_u1.w_011A,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x001B,(uint8_t *)&st_net_status_data_mppt_u1.w_011B,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x001C,(uint8_t *)&st_net_status_data_mppt_u1.w_011C,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x001D,(uint8_t *)&st_net_status_data_mppt_u1.w_011D,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x001E,(uint8_t *)&st_net_status_data_mppt_u1.w_011E,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x001F,(uint8_t *)&st_net_status_data_mppt_u1.w_011F,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
const net_data_info st_net_status_data_info_mppt_u1_2[cw_net_status_data_number_mppt] =
{
    {0x0104,0x0020,(uint8_t *)&st_net_status_data_mppt_u1.w_0120,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x0021,(uint8_t *)&st_net_status_data_mppt_u1.w_0121,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0022,(uint8_t *)&st_net_status_data_mppt_u1.w_0122,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0023,(uint8_t *)&st_net_status_data_mppt_u1.w_0123,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0024,(uint8_t *)&st_net_status_data_mppt_u1.w_0124,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0025,(uint8_t *)&st_net_status_data_mppt_u1.w_0125,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0026,(uint8_t *)&st_net_status_data_mppt_u1.w_0126,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0027,(uint8_t *)&st_net_status_data_mppt_u1.w_0127,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0028,(uint8_t *)&st_net_status_data_mppt_u1.w_0128,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0029,(uint8_t *)&st_net_status_data_mppt_u1.w_0129,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x002A,(uint8_t *)&st_net_status_data_mppt_u1.w_012A,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x002B,(uint8_t *)&st_net_status_data_mppt_u1.w_012B,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x002C,(uint8_t *)&st_net_status_data_mppt_u1.w_012C,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x002D,(uint8_t *)&st_net_status_data_mppt_u1.w_012D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
const net_data_info st_net_status_data_info_mppt_u1_3[cw_net_status_data_number_mppt] =
{
    {0x0104,0x002E,(uint8_t *)&st_net_status_data_mppt_u1.w_012E,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x002F,(uint8_t *)&st_net_status_data_mppt_u1.w_012F,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0030,(uint8_t *)&st_net_status_data_mppt_u1.w_0130,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0031,(uint8_t *)&st_net_status_data_mppt_u1.w_0131,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0032,(uint8_t *)&st_net_status_data_mppt_u1.w_0132,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0033,(uint8_t *)&st_net_status_data_mppt_u1.w_0133,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0034,(uint8_t *)&st_net_status_data_mppt_u1.w_0134,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0035,(uint8_t *)&st_net_status_data_mppt_u1.w_0135,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0036,(uint8_t *)&st_net_status_data_mppt_u1.w_0136,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0037,(uint8_t *)&st_net_status_data_mppt_u1.w_0137,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x0038,(uint8_t *)&st_net_status_data_mppt_u1.w_0138,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x0039,(uint8_t *)&st_net_status_data_mppt_u1.w_0139,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x003A,(uint8_t *)&st_net_status_data_mppt_u1.w_013A,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x003B,(uint8_t *)&st_net_status_data_mppt_u1.w_013B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},

};
const net_data_info st_net_status_data_info_mppt_u1_4[cw_net_status_data_number_mppt] =
{
    {0x0104,0x003C,(uint8_t *)&st_net_status_data_mppt_u1.w_013C,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x003D,(uint8_t *)&st_net_status_data_mppt_u1.w_013D,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x003E,(uint8_t *)&st_net_status_data_mppt_u1.w_013E,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x003F,(uint8_t *)&st_net_status_data_mppt_u1.w_013F,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0040,(uint8_t *)&st_net_status_data_mppt_u1.w_0140,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0041,(uint8_t *)&st_net_status_data_mppt_u1.w_0141,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0042,(uint8_t *)&st_net_status_data_mppt_u1.w_0142,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0043,(uint8_t *)&st_net_status_data_mppt_u1.w_0143,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0044,(uint8_t *)&st_net_status_data_mppt_u1.w_0144,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0045,(uint8_t *)&st_net_status_data_mppt_u1.w_0145,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x0046,(uint8_t *)&st_net_status_data_mppt_u1.w_0146,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x0047,(uint8_t *)&st_net_status_data_mppt_u1.w_0147,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x0048,(uint8_t *)&st_net_status_data_mppt_u1.w_0148,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x0049,(uint8_t *)&st_net_status_data_mppt_u1.w_0149,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},

};
const net_data_info st_net_status_data_info_mppt_u1_5[cw_net_status_data_number_mppt] =
{
    {0x0104,0x004A,(uint8_t *)&st_net_status_data_mppt_u1.w_014A,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x004B,(uint8_t *)&st_net_status_data_mppt_u1.w_014B,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x004C,(uint8_t *)&st_net_status_data_mppt_u1.w_014C,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x004D,(uint8_t *)&st_net_status_data_mppt_u1.w_014D,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x004E,(uint8_t *)&st_net_status_data_mppt_u1.w_014E,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x004F,(uint8_t *)&st_net_status_data_mppt_u1.w_014F,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0050,(uint8_t *)&st_net_status_data_mppt_u1.w_0150,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0051,(uint8_t *)&st_net_status_data_mppt_u1.w_0151,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0052,(uint8_t *)&st_net_status_data_mppt_u1.w_0152,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0053,(uint8_t *)&st_net_status_data_mppt_u1.w_0153,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x0054,(uint8_t *)&st_net_status_data_mppt_u1.w_0154,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x0055,(uint8_t *)&st_net_status_data_mppt_u1.w_0155,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x0056,(uint8_t *)&st_net_status_data_mppt_u1.w_0156,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x0057,(uint8_t *)&st_net_status_data_mppt_u1.w_0157,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};

//static net_set_data_mppt st_net_set_data_mppt_1 = {0,};
//net_data_info st_net_set_data_info_mppt_1[cw_net_set_data_number_mppt] =
//{
//};

//net_data_info st_net_control_data_info_mppt[cw_net_control_data_number_mppt] =
//{
//};
/**************************** MPPT RIIO SUN u1 ****************************/
const net_data_info st_net_status_data_info_riiosunii_mppt_u1_0[cw_net_status_data_number_riio_mppt_master] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_mppt_u1.w_0100,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0101,0x0001,(uint8_t *)&st_net_status_data_mppt_u1.w_0101,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0102,0x0002,(uint8_t *)&st_net_status_data_mppt_u1.w_0102,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0103,0x0003,(uint8_t *)&st_net_status_data_mppt_u1.w_0103,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0104,0x0004,(uint8_t *)&st_net_status_data_mppt_u1.w_0104,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0005,(uint8_t *)&st_net_status_data_mppt_u1.w_0105,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0006,(uint8_t *)&st_net_status_data_mppt_u1.w_0106,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0007,(uint8_t *)&st_net_status_data_mppt_u1.w_0107,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0008,(uint8_t *)&st_net_status_data_mppt_u1.w_0108,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0009,(uint8_t *)&st_net_status_data_mppt_u1.w_0109,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x000A,(uint8_t *)&st_net_status_data_mppt_u1.w_010A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x000B,(uint8_t *)&st_net_status_data_mppt_u1.w_010B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x000C,(uint8_t *)&st_net_status_data_mppt_u1.w_010C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x000D,(uint8_t *)&st_net_status_data_mppt_u1.w_010D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x000E,(uint8_t *)&st_net_status_data_mppt_u1.w_010E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x000F,(uint8_t *)&st_net_status_data_mppt_u1.w_010F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0010,(uint8_t *)&st_net_status_data_mppt_u1.w_0110,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0011,(uint8_t *)&st_net_status_data_mppt_u1.w_0111,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0012,(uint8_t *)&st_net_status_data_mppt_u1.w_0112,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0013,(uint8_t *)&st_net_status_data_mppt_u1.w_0113,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_u1_1[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0014,(uint8_t *)&st_net_status_data_mppt_u1.w_0114,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0015,(uint8_t *)&st_net_status_data_mppt_u1.w_0115,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0016,(uint8_t *)&st_net_status_data_mppt_u1.w_0116,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0017,(uint8_t *)&st_net_status_data_mppt_u1.w_0117,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0018,(uint8_t *)&st_net_status_data_mppt_u1.w_0118,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0019,(uint8_t *)&st_net_status_data_mppt_u1.w_0119,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x001A,(uint8_t *)&st_net_status_data_mppt_u1.w_011A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x001B,(uint8_t *)&st_net_status_data_mppt_u1.w_011B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x001C,(uint8_t *)&st_net_status_data_mppt_u1.w_011C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x001D,(uint8_t *)&st_net_status_data_mppt_u1.w_011D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x001E,(uint8_t *)&st_net_status_data_mppt_u1.w_011E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x001F,(uint8_t *)&st_net_status_data_mppt_u1.w_011F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0020,(uint8_t *)&st_net_status_data_mppt_u1.w_0120,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0021,(uint8_t *)&st_net_status_data_mppt_u1.w_0121,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0022,(uint8_t *)&st_net_status_data_mppt_u1.w_0122,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0023,(uint8_t *)&st_net_status_data_mppt_u1.w_0123,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_u1_2[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0024,(uint8_t *)&st_net_status_data_mppt_u1.w_0124,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0025,(uint8_t *)&st_net_status_data_mppt_u1.w_0125,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0026,(uint8_t *)&st_net_status_data_mppt_u1.w_0126,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0027,(uint8_t *)&st_net_status_data_mppt_u1.w_0127,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0028,(uint8_t *)&st_net_status_data_mppt_u1.w_0128,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0029,(uint8_t *)&st_net_status_data_mppt_u1.w_0129,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x002A,(uint8_t *)&st_net_status_data_mppt_u1.w_012A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x002B,(uint8_t *)&st_net_status_data_mppt_u1.w_012B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x002C,(uint8_t *)&st_net_status_data_mppt_u1.w_012C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x002D,(uint8_t *)&st_net_status_data_mppt_u1.w_012D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x002E,(uint8_t *)&st_net_status_data_mppt_u1.w_012E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x002F,(uint8_t *)&st_net_status_data_mppt_u1.w_012F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0030,(uint8_t *)&st_net_status_data_mppt_u1.w_0130,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0031,(uint8_t *)&st_net_status_data_mppt_u1.w_0131,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0032,(uint8_t *)&st_net_status_data_mppt_u1.w_0132,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0033,(uint8_t *)&st_net_status_data_mppt_u1.w_0133,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_u1_3[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0034,(uint8_t *)&st_net_status_data_mppt_u1.w_0134,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0035,(uint8_t *)&st_net_status_data_mppt_u1.w_0135,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0036,(uint8_t *)&st_net_status_data_mppt_u1.w_0136,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0037,(uint8_t *)&st_net_status_data_mppt_u1.w_0137,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0038,(uint8_t *)&st_net_status_data_mppt_u1.w_0138,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0039,(uint8_t *)&st_net_status_data_mppt_u1.w_0139,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x003A,(uint8_t *)&st_net_status_data_mppt_u1.w_013A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x003B,(uint8_t *)&st_net_status_data_mppt_u1.w_013B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x003C,(uint8_t *)&st_net_status_data_mppt_u1.w_013C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x003D,(uint8_t *)&st_net_status_data_mppt_u1.w_013D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x003E,(uint8_t *)&st_net_status_data_mppt_u1.w_013E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x003F,(uint8_t *)&st_net_status_data_mppt_u1.w_013F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0040,(uint8_t *)&st_net_status_data_mppt_u1.w_0140,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0041,(uint8_t *)&st_net_status_data_mppt_u1.w_0141,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0042,(uint8_t *)&st_net_status_data_mppt_u1.w_0142,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0043,(uint8_t *)&st_net_status_data_mppt_u1.w_0143,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_u1_4[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0044,(uint8_t *)&st_net_status_data_mppt_u1.w_0144,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0045,(uint8_t *)&st_net_status_data_mppt_u1.w_0145,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0046,(uint8_t *)&st_net_status_data_mppt_u1.w_0146,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0047,(uint8_t *)&st_net_status_data_mppt_u1.w_0147,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0048,(uint8_t *)&st_net_status_data_mppt_u1.w_0148,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0049,(uint8_t *)&st_net_status_data_mppt_u1.w_0149,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x004A,(uint8_t *)&st_net_status_data_mppt_u1.w_014A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x004B,(uint8_t *)&st_net_status_data_mppt_u1.w_014B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x004C,(uint8_t *)&st_net_status_data_mppt_u1.w_014C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x004D,(uint8_t *)&st_net_status_data_mppt_u1.w_014D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x004E,(uint8_t *)&st_net_status_data_mppt_u1.w_014E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x004F,(uint8_t *)&st_net_status_data_mppt_u1.w_014F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0050,(uint8_t *)&st_net_status_data_mppt_u1.w_0150,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0051,(uint8_t *)&st_net_status_data_mppt_u1.w_0151,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0052,(uint8_t *)&st_net_status_data_mppt_u1.w_0152,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0053,(uint8_t *)&st_net_status_data_mppt_u1.w_0153,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_u1_5[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0054,(uint8_t *)&st_net_status_data_mppt_u1.w_0154,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0055,(uint8_t *)&st_net_status_data_mppt_u1.w_0155,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0056,(uint8_t *)&st_net_status_data_mppt_u1.w_0156,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0057,(uint8_t *)&st_net_status_data_mppt_u1.w_0157,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0058,(uint8_t *)&st_net_status_data_mppt_u1.w_0158,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0059,(uint8_t *)&st_net_status_data_mppt_u1.w_0159,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x005A,(uint8_t *)&st_net_status_data_mppt_u1.w_015A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x005B,(uint8_t *)&st_net_status_data_mppt_u1.w_015B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x005C,(uint8_t *)&st_net_status_data_mppt_u1.w_015C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x005D,(uint8_t *)&st_net_status_data_mppt_u1.w_015D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x005E,(uint8_t *)&st_net_status_data_mppt_u1.w_015E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x005F,(uint8_t *)&st_net_status_data_mppt_u1.w_015F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0060,(uint8_t *)&st_net_status_data_mppt_u1.w_0160,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0061,(uint8_t *)&st_net_status_data_mppt_u1.w_0161,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0062,(uint8_t *)&st_net_status_data_mppt_u1.w_0162,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0063,(uint8_t *)&st_net_status_data_mppt_u1.w_0163,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
/**************************** MPPT u2 ****************************/
net_status_data_mppt st_net_status_data_mppt_u2 =
{
    0,
};
const net_data_info st_net_status_data_info_mppt_u2_0[cw_net_status_data_number_mppt_master] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_mppt_u2.w_0100,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0101,0x0001,(uint8_t *)&st_net_status_data_mppt_u2.w_0101,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0102,0x0002,(uint8_t *)&st_net_status_data_mppt_u2.w_0102,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0103,0x0003,(uint8_t *)&st_net_status_data_mppt_u2.w_0103,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},

    {0x0104,0x0004,(uint8_t *)&st_net_status_data_mppt_u2.w_0104,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x0005,(uint8_t *)&st_net_status_data_mppt_u2.w_0105,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0006,(uint8_t *)&st_net_status_data_mppt_u2.w_0106,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0007,(uint8_t *)&st_net_status_data_mppt_u2.w_0107,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0008,(uint8_t *)&st_net_status_data_mppt_u2.w_0108,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0009,(uint8_t *)&st_net_status_data_mppt_u2.w_0109,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x000A,(uint8_t *)&st_net_status_data_mppt_u2.w_010A,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x000B,(uint8_t *)&st_net_status_data_mppt_u2.w_010B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x000C,(uint8_t *)&st_net_status_data_mppt_u2.w_010C,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x000D,(uint8_t *)&st_net_status_data_mppt_u2.w_010D,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x000E,(uint8_t *)&st_net_status_data_mppt_u2.w_010E,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x000F,(uint8_t *)&st_net_status_data_mppt_u2.w_010F,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x0010,(uint8_t *)&st_net_status_data_mppt_u2.w_0110,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x0011,(uint8_t *)&st_net_status_data_mppt_u2.w_0111,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
const net_data_info st_net_status_data_info_mppt_u2_1[cw_net_status_data_number_mppt] =
{
    {0x0104,0x0012,(uint8_t *)&st_net_status_data_mppt_u2.w_0112,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x0013,(uint8_t *)&st_net_status_data_mppt_u2.w_0113,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0014,(uint8_t *)&st_net_status_data_mppt_u2.w_0114,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0015,(uint8_t *)&st_net_status_data_mppt_u2.w_0115,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0016,(uint8_t *)&st_net_status_data_mppt_u2.w_0116,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0017,(uint8_t *)&st_net_status_data_mppt_u2.w_0117,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0018,(uint8_t *)&st_net_status_data_mppt_u2.w_0118,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0019,(uint8_t *)&st_net_status_data_mppt_u2.w_0119,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x001A,(uint8_t *)&st_net_status_data_mppt_u2.w_011A,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x001B,(uint8_t *)&st_net_status_data_mppt_u2.w_011B,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x001C,(uint8_t *)&st_net_status_data_mppt_u2.w_011C,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x001D,(uint8_t *)&st_net_status_data_mppt_u2.w_011D,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x001E,(uint8_t *)&st_net_status_data_mppt_u2.w_011E,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x001F,(uint8_t *)&st_net_status_data_mppt_u2.w_011F,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
const net_data_info st_net_status_data_info_mppt_u2_2[cw_net_status_data_number_mppt] =
{
    {0x0104,0x0020,(uint8_t *)&st_net_status_data_mppt_u2.w_0120,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x0021,(uint8_t *)&st_net_status_data_mppt_u2.w_0121,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0022,(uint8_t *)&st_net_status_data_mppt_u2.w_0122,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0023,(uint8_t *)&st_net_status_data_mppt_u2.w_0123,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0024,(uint8_t *)&st_net_status_data_mppt_u2.w_0124,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0025,(uint8_t *)&st_net_status_data_mppt_u2.w_0125,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0026,(uint8_t *)&st_net_status_data_mppt_u2.w_0126,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0027,(uint8_t *)&st_net_status_data_mppt_u2.w_0127,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0028,(uint8_t *)&st_net_status_data_mppt_u2.w_0128,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0029,(uint8_t *)&st_net_status_data_mppt_u2.w_0129,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x002A,(uint8_t *)&st_net_status_data_mppt_u2.w_012A,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x002B,(uint8_t *)&st_net_status_data_mppt_u2.w_012B,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x002C,(uint8_t *)&st_net_status_data_mppt_u2.w_012C,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x002D,(uint8_t *)&st_net_status_data_mppt_u2.w_012D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
const net_data_info st_net_status_data_info_mppt_u2_3[cw_net_status_data_number_mppt] =
{
    {0x0104,0x002E,(uint8_t *)&st_net_status_data_mppt_u2.w_012E,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x002F,(uint8_t *)&st_net_status_data_mppt_u2.w_012F,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0030,(uint8_t *)&st_net_status_data_mppt_u2.w_0130,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0031,(uint8_t *)&st_net_status_data_mppt_u2.w_0131,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0032,(uint8_t *)&st_net_status_data_mppt_u2.w_0132,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0033,(uint8_t *)&st_net_status_data_mppt_u2.w_0133,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0034,(uint8_t *)&st_net_status_data_mppt_u2.w_0134,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0035,(uint8_t *)&st_net_status_data_mppt_u2.w_0135,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0036,(uint8_t *)&st_net_status_data_mppt_u2.w_0136,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0037,(uint8_t *)&st_net_status_data_mppt_u2.w_0137,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x0038,(uint8_t *)&st_net_status_data_mppt_u2.w_0138,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x0039,(uint8_t *)&st_net_status_data_mppt_u2.w_0139,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x003A,(uint8_t *)&st_net_status_data_mppt_u2.w_013A,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x003B,(uint8_t *)&st_net_status_data_mppt_u2.w_013B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},

};
const net_data_info st_net_status_data_info_mppt_u2_4[cw_net_status_data_number_mppt] =
{
    {0x0104,0x003C,(uint8_t *)&st_net_status_data_mppt_u2.w_013C,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x003D,(uint8_t *)&st_net_status_data_mppt_u2.w_013D,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x003E,(uint8_t *)&st_net_status_data_mppt_u2.w_013E,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x003F,(uint8_t *)&st_net_status_data_mppt_u2.w_013F,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0040,(uint8_t *)&st_net_status_data_mppt_u2.w_0140,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0041,(uint8_t *)&st_net_status_data_mppt_u2.w_0141,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0042,(uint8_t *)&st_net_status_data_mppt_u2.w_0142,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0043,(uint8_t *)&st_net_status_data_mppt_u2.w_0143,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0044,(uint8_t *)&st_net_status_data_mppt_u2.w_0144,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0045,(uint8_t *)&st_net_status_data_mppt_u2.w_0145,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x0046,(uint8_t *)&st_net_status_data_mppt_u2.w_0146,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x0047,(uint8_t *)&st_net_status_data_mppt_u2.w_0147,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x0048,(uint8_t *)&st_net_status_data_mppt_u2.w_0148,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x0049,(uint8_t *)&st_net_status_data_mppt_u2.w_0149,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},

};
const net_data_info st_net_status_data_info_mppt_u2_5[cw_net_status_data_number_mppt] =
{
    {0x0104,0x004A,(uint8_t *)&st_net_status_data_mppt_u2.w_014A,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x004B,(uint8_t *)&st_net_status_data_mppt_u2.w_014B,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x004C,(uint8_t *)&st_net_status_data_mppt_u2.w_014C,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x004D,(uint8_t *)&st_net_status_data_mppt_u2.w_014D,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x004E,(uint8_t *)&st_net_status_data_mppt_u2.w_014E,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x004F,(uint8_t *)&st_net_status_data_mppt_u2.w_014F,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0050,(uint8_t *)&st_net_status_data_mppt_u2.w_0150,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0051,(uint8_t *)&st_net_status_data_mppt_u2.w_0151,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0052,(uint8_t *)&st_net_status_data_mppt_u2.w_0152,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0053,(uint8_t *)&st_net_status_data_mppt_u2.w_0153,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x0054,(uint8_t *)&st_net_status_data_mppt_u2.w_0154,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x0055,(uint8_t *)&st_net_status_data_mppt_u2.w_0155,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x0056,(uint8_t *)&st_net_status_data_mppt_u2.w_0156,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x0057,(uint8_t *)&st_net_status_data_mppt_u2.w_0157,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
/**************************** MPPT RIIO SUN u2 ****************************/
const net_data_info st_net_status_data_info_riiosunii_mppt_u2_0[cw_net_status_data_number_riio_mppt_master] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_mppt_u2.w_0100,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0101,0x0001,(uint8_t *)&st_net_status_data_mppt_u2.w_0101,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0102,0x0002,(uint8_t *)&st_net_status_data_mppt_u2.w_0102,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0103,0x0003,(uint8_t *)&st_net_status_data_mppt_u2.w_0103,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0104,0x0004,(uint8_t *)&st_net_status_data_mppt_u2.w_0104,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0005,(uint8_t *)&st_net_status_data_mppt_u2.w_0105,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0006,(uint8_t *)&st_net_status_data_mppt_u2.w_0106,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0007,(uint8_t *)&st_net_status_data_mppt_u2.w_0107,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0008,(uint8_t *)&st_net_status_data_mppt_u2.w_0108,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0009,(uint8_t *)&st_net_status_data_mppt_u2.w_0109,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x000A,(uint8_t *)&st_net_status_data_mppt_u2.w_010A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x000B,(uint8_t *)&st_net_status_data_mppt_u2.w_010B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x000C,(uint8_t *)&st_net_status_data_mppt_u2.w_010C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x000D,(uint8_t *)&st_net_status_data_mppt_u2.w_010D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x000E,(uint8_t *)&st_net_status_data_mppt_u2.w_010E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x000F,(uint8_t *)&st_net_status_data_mppt_u2.w_010F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0010,(uint8_t *)&st_net_status_data_mppt_u2.w_0110,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0011,(uint8_t *)&st_net_status_data_mppt_u2.w_0111,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0012,(uint8_t *)&st_net_status_data_mppt_u2.w_0112,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0013,(uint8_t *)&st_net_status_data_mppt_u2.w_0113,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_u2_1[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0014,(uint8_t *)&st_net_status_data_mppt_u2.w_0114,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0015,(uint8_t *)&st_net_status_data_mppt_u2.w_0115,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0016,(uint8_t *)&st_net_status_data_mppt_u2.w_0116,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0017,(uint8_t *)&st_net_status_data_mppt_u2.w_0117,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0018,(uint8_t *)&st_net_status_data_mppt_u2.w_0118,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0019,(uint8_t *)&st_net_status_data_mppt_u2.w_0119,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x001A,(uint8_t *)&st_net_status_data_mppt_u2.w_011A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x001B,(uint8_t *)&st_net_status_data_mppt_u2.w_011B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x001C,(uint8_t *)&st_net_status_data_mppt_u2.w_011C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x001D,(uint8_t *)&st_net_status_data_mppt_u2.w_011D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x001E,(uint8_t *)&st_net_status_data_mppt_u2.w_011E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x001F,(uint8_t *)&st_net_status_data_mppt_u2.w_011F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0020,(uint8_t *)&st_net_status_data_mppt_u2.w_0120,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0021,(uint8_t *)&st_net_status_data_mppt_u2.w_0121,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0022,(uint8_t *)&st_net_status_data_mppt_u2.w_0122,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0023,(uint8_t *)&st_net_status_data_mppt_u2.w_0123,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_u2_2[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0024,(uint8_t *)&st_net_status_data_mppt_u2.w_0124,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0025,(uint8_t *)&st_net_status_data_mppt_u2.w_0125,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0026,(uint8_t *)&st_net_status_data_mppt_u2.w_0126,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0027,(uint8_t *)&st_net_status_data_mppt_u2.w_0127,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0028,(uint8_t *)&st_net_status_data_mppt_u2.w_0128,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0029,(uint8_t *)&st_net_status_data_mppt_u2.w_0129,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x002A,(uint8_t *)&st_net_status_data_mppt_u2.w_012A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x002B,(uint8_t *)&st_net_status_data_mppt_u2.w_012B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x002C,(uint8_t *)&st_net_status_data_mppt_u2.w_012C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x002D,(uint8_t *)&st_net_status_data_mppt_u2.w_012D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x002E,(uint8_t *)&st_net_status_data_mppt_u2.w_012E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x002F,(uint8_t *)&st_net_status_data_mppt_u2.w_012F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0030,(uint8_t *)&st_net_status_data_mppt_u2.w_0130,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0031,(uint8_t *)&st_net_status_data_mppt_u2.w_0131,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0032,(uint8_t *)&st_net_status_data_mppt_u2.w_0132,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0033,(uint8_t *)&st_net_status_data_mppt_u2.w_0133,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_u2_3[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0034,(uint8_t *)&st_net_status_data_mppt_u2.w_0134,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0035,(uint8_t *)&st_net_status_data_mppt_u2.w_0135,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0036,(uint8_t *)&st_net_status_data_mppt_u2.w_0136,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0037,(uint8_t *)&st_net_status_data_mppt_u2.w_0137,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0038,(uint8_t *)&st_net_status_data_mppt_u2.w_0138,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0039,(uint8_t *)&st_net_status_data_mppt_u2.w_0139,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x003A,(uint8_t *)&st_net_status_data_mppt_u2.w_013A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x003B,(uint8_t *)&st_net_status_data_mppt_u2.w_013B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x003C,(uint8_t *)&st_net_status_data_mppt_u2.w_013C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x003D,(uint8_t *)&st_net_status_data_mppt_u2.w_013D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x003E,(uint8_t *)&st_net_status_data_mppt_u2.w_013E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x003F,(uint8_t *)&st_net_status_data_mppt_u2.w_013F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0040,(uint8_t *)&st_net_status_data_mppt_u2.w_0140,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0041,(uint8_t *)&st_net_status_data_mppt_u2.w_0141,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0042,(uint8_t *)&st_net_status_data_mppt_u2.w_0142,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0043,(uint8_t *)&st_net_status_data_mppt_u2.w_0143,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_u2_4[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0044,(uint8_t *)&st_net_status_data_mppt_u2.w_0144,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0045,(uint8_t *)&st_net_status_data_mppt_u2.w_0145,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0046,(uint8_t *)&st_net_status_data_mppt_u2.w_0146,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0047,(uint8_t *)&st_net_status_data_mppt_u2.w_0147,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0048,(uint8_t *)&st_net_status_data_mppt_u2.w_0148,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0049,(uint8_t *)&st_net_status_data_mppt_u2.w_0149,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x004A,(uint8_t *)&st_net_status_data_mppt_u2.w_014A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x004B,(uint8_t *)&st_net_status_data_mppt_u2.w_014B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x004C,(uint8_t *)&st_net_status_data_mppt_u2.w_014C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x004D,(uint8_t *)&st_net_status_data_mppt_u2.w_014D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x004E,(uint8_t *)&st_net_status_data_mppt_u2.w_014E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x004F,(uint8_t *)&st_net_status_data_mppt_u2.w_014F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0050,(uint8_t *)&st_net_status_data_mppt_u2.w_0150,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0051,(uint8_t *)&st_net_status_data_mppt_u2.w_0151,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0052,(uint8_t *)&st_net_status_data_mppt_u2.w_0152,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0053,(uint8_t *)&st_net_status_data_mppt_u2.w_0153,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_u2_5[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0054,(uint8_t *)&st_net_status_data_mppt_u2.w_0154,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0055,(uint8_t *)&st_net_status_data_mppt_u2.w_0155,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0056,(uint8_t *)&st_net_status_data_mppt_u2.w_0156,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0057,(uint8_t *)&st_net_status_data_mppt_u2.w_0157,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0058,(uint8_t *)&st_net_status_data_mppt_u2.w_0158,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0059,(uint8_t *)&st_net_status_data_mppt_u2.w_0159,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x005A,(uint8_t *)&st_net_status_data_mppt_u2.w_015A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x005B,(uint8_t *)&st_net_status_data_mppt_u2.w_015B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x005C,(uint8_t *)&st_net_status_data_mppt_u2.w_015C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x005D,(uint8_t *)&st_net_status_data_mppt_u2.w_015D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x005E,(uint8_t *)&st_net_status_data_mppt_u2.w_015E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x005F,(uint8_t *)&st_net_status_data_mppt_u2.w_015F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0060,(uint8_t *)&st_net_status_data_mppt_u2.w_0160,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0061,(uint8_t *)&st_net_status_data_mppt_u2.w_0161,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0062,(uint8_t *)&st_net_status_data_mppt_u2.w_0162,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0063,(uint8_t *)&st_net_status_data_mppt_u2.w_0163,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
/**************************** MPPT u3 ****************************/
net_status_data_mppt st_net_status_data_mppt_u3 =
{
    0,
};
const net_data_info st_net_status_data_info_mppt_u3_0[cw_net_status_data_number_mppt_master] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_mppt_u3.w_0100,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0101,0x0001,(uint8_t *)&st_net_status_data_mppt_u3.w_0101,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0102,0x0002,(uint8_t *)&st_net_status_data_mppt_u3.w_0102,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0103,0x0003,(uint8_t *)&st_net_status_data_mppt_u3.w_0103,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},

    {0x0104,0x0004,(uint8_t *)&st_net_status_data_mppt_u3.w_0104,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x0005,(uint8_t *)&st_net_status_data_mppt_u3.w_0105,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0006,(uint8_t *)&st_net_status_data_mppt_u3.w_0106,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0007,(uint8_t *)&st_net_status_data_mppt_u3.w_0107,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0008,(uint8_t *)&st_net_status_data_mppt_u3.w_0108,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0009,(uint8_t *)&st_net_status_data_mppt_u3.w_0109,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x000A,(uint8_t *)&st_net_status_data_mppt_u3.w_010A,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x000B,(uint8_t *)&st_net_status_data_mppt_u3.w_010B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x000C,(uint8_t *)&st_net_status_data_mppt_u3.w_010C,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x000D,(uint8_t *)&st_net_status_data_mppt_u3.w_010D,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x000E,(uint8_t *)&st_net_status_data_mppt_u3.w_010E,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x000F,(uint8_t *)&st_net_status_data_mppt_u3.w_010F,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x0010,(uint8_t *)&st_net_status_data_mppt_u3.w_0110,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x0011,(uint8_t *)&st_net_status_data_mppt_u3.w_0111,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
const net_data_info st_net_status_data_info_mppt_u3_1[cw_net_status_data_number_mppt] =
{
    {0x0104,0x0012,(uint8_t *)&st_net_status_data_mppt_u3.w_0112,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x0013,(uint8_t *)&st_net_status_data_mppt_u3.w_0113,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0014,(uint8_t *)&st_net_status_data_mppt_u3.w_0114,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0015,(uint8_t *)&st_net_status_data_mppt_u3.w_0115,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0016,(uint8_t *)&st_net_status_data_mppt_u3.w_0116,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0017,(uint8_t *)&st_net_status_data_mppt_u3.w_0117,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0018,(uint8_t *)&st_net_status_data_mppt_u3.w_0118,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0019,(uint8_t *)&st_net_status_data_mppt_u3.w_0119,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x001A,(uint8_t *)&st_net_status_data_mppt_u3.w_011A,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x001B,(uint8_t *)&st_net_status_data_mppt_u3.w_011B,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x001C,(uint8_t *)&st_net_status_data_mppt_u3.w_011C,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x001D,(uint8_t *)&st_net_status_data_mppt_u3.w_011D,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x001E,(uint8_t *)&st_net_status_data_mppt_u3.w_011E,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x001F,(uint8_t *)&st_net_status_data_mppt_u3.w_011F,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
const net_data_info st_net_status_data_info_mppt_u3_2[cw_net_status_data_number_mppt] =
{
    {0x0104,0x0020,(uint8_t *)&st_net_status_data_mppt_u3.w_0120,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x0021,(uint8_t *)&st_net_status_data_mppt_u3.w_0121,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0022,(uint8_t *)&st_net_status_data_mppt_u3.w_0122,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0023,(uint8_t *)&st_net_status_data_mppt_u3.w_0123,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0024,(uint8_t *)&st_net_status_data_mppt_u3.w_0124,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0025,(uint8_t *)&st_net_status_data_mppt_u3.w_0125,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0026,(uint8_t *)&st_net_status_data_mppt_u3.w_0126,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0027,(uint8_t *)&st_net_status_data_mppt_u3.w_0127,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0028,(uint8_t *)&st_net_status_data_mppt_u3.w_0128,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0029,(uint8_t *)&st_net_status_data_mppt_u3.w_0129,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x002A,(uint8_t *)&st_net_status_data_mppt_u3.w_012A,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x002B,(uint8_t *)&st_net_status_data_mppt_u3.w_012B,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x002C,(uint8_t *)&st_net_status_data_mppt_u3.w_012C,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x002D,(uint8_t *)&st_net_status_data_mppt_u3.w_012D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
const net_data_info st_net_status_data_info_mppt_u3_3[cw_net_status_data_number_mppt] =
{
    {0x0104,0x002E,(uint8_t *)&st_net_status_data_mppt_u3.w_012E,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x002F,(uint8_t *)&st_net_status_data_mppt_u3.w_012F,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0030,(uint8_t *)&st_net_status_data_mppt_u3.w_0130,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0031,(uint8_t *)&st_net_status_data_mppt_u3.w_0131,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0032,(uint8_t *)&st_net_status_data_mppt_u3.w_0132,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0033,(uint8_t *)&st_net_status_data_mppt_u3.w_0133,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0034,(uint8_t *)&st_net_status_data_mppt_u3.w_0134,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0035,(uint8_t *)&st_net_status_data_mppt_u3.w_0135,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0036,(uint8_t *)&st_net_status_data_mppt_u3.w_0136,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0037,(uint8_t *)&st_net_status_data_mppt_u3.w_0137,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x0038,(uint8_t *)&st_net_status_data_mppt_u3.w_0138,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x0039,(uint8_t *)&st_net_status_data_mppt_u3.w_0139,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x003A,(uint8_t *)&st_net_status_data_mppt_u3.w_013A,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x003B,(uint8_t *)&st_net_status_data_mppt_u3.w_013B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},

};
const net_data_info st_net_status_data_info_mppt_u3_4[cw_net_status_data_number_mppt] =
{
    {0x0104,0x003C,(uint8_t *)&st_net_status_data_mppt_u3.w_013C,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x003D,(uint8_t *)&st_net_status_data_mppt_u3.w_013D,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x003E,(uint8_t *)&st_net_status_data_mppt_u3.w_013E,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x003F,(uint8_t *)&st_net_status_data_mppt_u3.w_013F,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0040,(uint8_t *)&st_net_status_data_mppt_u3.w_0140,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0041,(uint8_t *)&st_net_status_data_mppt_u3.w_0141,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0042,(uint8_t *)&st_net_status_data_mppt_u3.w_0142,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0043,(uint8_t *)&st_net_status_data_mppt_u3.w_0143,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0044,(uint8_t *)&st_net_status_data_mppt_u3.w_0144,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0045,(uint8_t *)&st_net_status_data_mppt_u3.w_0145,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x0046,(uint8_t *)&st_net_status_data_mppt_u3.w_0146,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x0047,(uint8_t *)&st_net_status_data_mppt_u3.w_0147,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x0048,(uint8_t *)&st_net_status_data_mppt_u3.w_0148,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x0049,(uint8_t *)&st_net_status_data_mppt_u3.w_0149,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},

};
const net_data_info st_net_status_data_info_mppt_u3_5[cw_net_status_data_number_mppt] =
{
    {0x0104,0x004A,(uint8_t *)&st_net_status_data_mppt_u3.w_014A,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x004B,(uint8_t *)&st_net_status_data_mppt_u3.w_014B,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x004C,(uint8_t *)&st_net_status_data_mppt_u3.w_014C,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x004D,(uint8_t *)&st_net_status_data_mppt_u3.w_014D,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x004E,(uint8_t *)&st_net_status_data_mppt_u3.w_014E,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x004F,(uint8_t *)&st_net_status_data_mppt_u3.w_014F,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0050,(uint8_t *)&st_net_status_data_mppt_u3.w_0150,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0051,(uint8_t *)&st_net_status_data_mppt_u3.w_0151,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0052,(uint8_t *)&st_net_status_data_mppt_u3.w_0152,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0053,(uint8_t *)&st_net_status_data_mppt_u3.w_0153,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x0054,(uint8_t *)&st_net_status_data_mppt_u3.w_0154,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x0055,(uint8_t *)&st_net_status_data_mppt_u3.w_0155,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x0056,(uint8_t *)&st_net_status_data_mppt_u3.w_0156,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x0057,(uint8_t *)&st_net_status_data_mppt_u3.w_0157,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
/**************************** MPPT RIIO SUN u3 ****************************/
const net_data_info st_net_status_data_info_riiosunii_mppt_u3_0[cw_net_status_data_number_riio_mppt_master] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_mppt_u3.w_0100,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0101,0x0001,(uint8_t *)&st_net_status_data_mppt_u3.w_0101,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0102,0x0002,(uint8_t *)&st_net_status_data_mppt_u3.w_0102,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0103,0x0003,(uint8_t *)&st_net_status_data_mppt_u3.w_0103,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0104,0x0004,(uint8_t *)&st_net_status_data_mppt_u3.w_0104,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0005,(uint8_t *)&st_net_status_data_mppt_u3.w_0105,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0006,(uint8_t *)&st_net_status_data_mppt_u3.w_0106,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0007,(uint8_t *)&st_net_status_data_mppt_u3.w_0107,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0008,(uint8_t *)&st_net_status_data_mppt_u3.w_0108,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0009,(uint8_t *)&st_net_status_data_mppt_u3.w_0109,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x000A,(uint8_t *)&st_net_status_data_mppt_u3.w_010A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x000B,(uint8_t *)&st_net_status_data_mppt_u3.w_010B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x000C,(uint8_t *)&st_net_status_data_mppt_u3.w_010C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x000D,(uint8_t *)&st_net_status_data_mppt_u3.w_010D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x000E,(uint8_t *)&st_net_status_data_mppt_u3.w_010E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x000F,(uint8_t *)&st_net_status_data_mppt_u3.w_010F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0010,(uint8_t *)&st_net_status_data_mppt_u3.w_0110,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0011,(uint8_t *)&st_net_status_data_mppt_u3.w_0111,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0012,(uint8_t *)&st_net_status_data_mppt_u3.w_0112,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0013,(uint8_t *)&st_net_status_data_mppt_u3.w_0113,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_u3_1[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0014,(uint8_t *)&st_net_status_data_mppt_u3.w_0114,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0015,(uint8_t *)&st_net_status_data_mppt_u3.w_0115,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0016,(uint8_t *)&st_net_status_data_mppt_u3.w_0116,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0017,(uint8_t *)&st_net_status_data_mppt_u3.w_0117,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0018,(uint8_t *)&st_net_status_data_mppt_u3.w_0118,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0019,(uint8_t *)&st_net_status_data_mppt_u3.w_0119,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x001A,(uint8_t *)&st_net_status_data_mppt_u3.w_011A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x001B,(uint8_t *)&st_net_status_data_mppt_u3.w_011B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x001C,(uint8_t *)&st_net_status_data_mppt_u3.w_011C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x001D,(uint8_t *)&st_net_status_data_mppt_u3.w_011D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x001E,(uint8_t *)&st_net_status_data_mppt_u3.w_011E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x001F,(uint8_t *)&st_net_status_data_mppt_u3.w_011F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0020,(uint8_t *)&st_net_status_data_mppt_u3.w_0120,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0021,(uint8_t *)&st_net_status_data_mppt_u3.w_0121,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0022,(uint8_t *)&st_net_status_data_mppt_u3.w_0122,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0023,(uint8_t *)&st_net_status_data_mppt_u3.w_0123,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_u3_2[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0024,(uint8_t *)&st_net_status_data_mppt_u3.w_0124,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0025,(uint8_t *)&st_net_status_data_mppt_u3.w_0125,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0026,(uint8_t *)&st_net_status_data_mppt_u3.w_0126,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0027,(uint8_t *)&st_net_status_data_mppt_u3.w_0127,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0028,(uint8_t *)&st_net_status_data_mppt_u3.w_0128,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0029,(uint8_t *)&st_net_status_data_mppt_u3.w_0129,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x002A,(uint8_t *)&st_net_status_data_mppt_u3.w_012A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x002B,(uint8_t *)&st_net_status_data_mppt_u3.w_012B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x002C,(uint8_t *)&st_net_status_data_mppt_u3.w_012C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x002D,(uint8_t *)&st_net_status_data_mppt_u3.w_012D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x002E,(uint8_t *)&st_net_status_data_mppt_u3.w_012E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x002F,(uint8_t *)&st_net_status_data_mppt_u3.w_012F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0030,(uint8_t *)&st_net_status_data_mppt_u3.w_0130,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0031,(uint8_t *)&st_net_status_data_mppt_u3.w_0131,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0032,(uint8_t *)&st_net_status_data_mppt_u3.w_0132,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0033,(uint8_t *)&st_net_status_data_mppt_u3.w_0133,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_u3_3[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0034,(uint8_t *)&st_net_status_data_mppt_u3.w_0134,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0035,(uint8_t *)&st_net_status_data_mppt_u3.w_0135,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0036,(uint8_t *)&st_net_status_data_mppt_u3.w_0136,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0037,(uint8_t *)&st_net_status_data_mppt_u3.w_0137,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0038,(uint8_t *)&st_net_status_data_mppt_u3.w_0138,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0039,(uint8_t *)&st_net_status_data_mppt_u3.w_0139,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x003A,(uint8_t *)&st_net_status_data_mppt_u3.w_013A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x003B,(uint8_t *)&st_net_status_data_mppt_u3.w_013B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x003C,(uint8_t *)&st_net_status_data_mppt_u3.w_013C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x003D,(uint8_t *)&st_net_status_data_mppt_u3.w_013D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x003E,(uint8_t *)&st_net_status_data_mppt_u3.w_013E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x003F,(uint8_t *)&st_net_status_data_mppt_u3.w_013F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0040,(uint8_t *)&st_net_status_data_mppt_u3.w_0140,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0041,(uint8_t *)&st_net_status_data_mppt_u3.w_0141,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0042,(uint8_t *)&st_net_status_data_mppt_u3.w_0142,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0043,(uint8_t *)&st_net_status_data_mppt_u3.w_0143,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_u3_4[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0044,(uint8_t *)&st_net_status_data_mppt_u3.w_0144,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0045,(uint8_t *)&st_net_status_data_mppt_u3.w_0145,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0046,(uint8_t *)&st_net_status_data_mppt_u3.w_0146,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0047,(uint8_t *)&st_net_status_data_mppt_u3.w_0147,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0048,(uint8_t *)&st_net_status_data_mppt_u3.w_0148,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0049,(uint8_t *)&st_net_status_data_mppt_u3.w_0149,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x004A,(uint8_t *)&st_net_status_data_mppt_u3.w_014A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x004B,(uint8_t *)&st_net_status_data_mppt_u3.w_014B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x004C,(uint8_t *)&st_net_status_data_mppt_u3.w_014C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x004D,(uint8_t *)&st_net_status_data_mppt_u3.w_014D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x004E,(uint8_t *)&st_net_status_data_mppt_u3.w_014E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x004F,(uint8_t *)&st_net_status_data_mppt_u3.w_014F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0050,(uint8_t *)&st_net_status_data_mppt_u3.w_0150,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0051,(uint8_t *)&st_net_status_data_mppt_u3.w_0151,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0052,(uint8_t *)&st_net_status_data_mppt_u3.w_0152,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0053,(uint8_t *)&st_net_status_data_mppt_u3.w_0153,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_u3_5[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0054,(uint8_t *)&st_net_status_data_mppt_u3.w_0154,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0055,(uint8_t *)&st_net_status_data_mppt_u3.w_0155,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0056,(uint8_t *)&st_net_status_data_mppt_u3.w_0156,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0057,(uint8_t *)&st_net_status_data_mppt_u3.w_0157,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0058,(uint8_t *)&st_net_status_data_mppt_u3.w_0158,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0059,(uint8_t *)&st_net_status_data_mppt_u3.w_0159,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x005A,(uint8_t *)&st_net_status_data_mppt_u3.w_015A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x005B,(uint8_t *)&st_net_status_data_mppt_u3.w_015B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x005C,(uint8_t *)&st_net_status_data_mppt_u3.w_015C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x005D,(uint8_t *)&st_net_status_data_mppt_u3.w_015D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x005E,(uint8_t *)&st_net_status_data_mppt_u3.w_015E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x005F,(uint8_t *)&st_net_status_data_mppt_u3.w_015F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0060,(uint8_t *)&st_net_status_data_mppt_u3.w_0160,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0061,(uint8_t *)&st_net_status_data_mppt_u3.w_0161,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0062,(uint8_t *)&st_net_status_data_mppt_u3.w_0162,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0063,(uint8_t *)&st_net_status_data_mppt_u3.w_0163,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
/**************************** MPPT v1 ****************************/
net_status_data_mppt st_net_status_data_mppt_v1 =
{
    0,
};
const net_data_info st_net_status_data_info_mppt_v1_0[cw_net_status_data_number_mppt_master] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_mppt_v1.w_0100,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0101,0x0001,(uint8_t *)&st_net_status_data_mppt_v1.w_0101,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0102,0x0002,(uint8_t *)&st_net_status_data_mppt_v1.w_0102,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0103,0x0003,(uint8_t *)&st_net_status_data_mppt_v1.w_0103,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},

    {0x0104,0x0004,(uint8_t *)&st_net_status_data_mppt_v1.w_0104,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x0005,(uint8_t *)&st_net_status_data_mppt_v1.w_0105,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0006,(uint8_t *)&st_net_status_data_mppt_v1.w_0106,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0007,(uint8_t *)&st_net_status_data_mppt_v1.w_0107,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0008,(uint8_t *)&st_net_status_data_mppt_v1.w_0108,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0009,(uint8_t *)&st_net_status_data_mppt_v1.w_0109,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x000A,(uint8_t *)&st_net_status_data_mppt_v1.w_010A,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x000B,(uint8_t *)&st_net_status_data_mppt_v1.w_010B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x000C,(uint8_t *)&st_net_status_data_mppt_v1.w_010C,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x000D,(uint8_t *)&st_net_status_data_mppt_v1.w_010D,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x000E,(uint8_t *)&st_net_status_data_mppt_v1.w_010E,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x000F,(uint8_t *)&st_net_status_data_mppt_v1.w_010F,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x0010,(uint8_t *)&st_net_status_data_mppt_v1.w_0110,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x0011,(uint8_t *)&st_net_status_data_mppt_v1.w_0111,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
const net_data_info st_net_status_data_info_mppt_v1_1[cw_net_status_data_number_mppt] =
{
    {0x0104,0x0012,(uint8_t *)&st_net_status_data_mppt_v1.w_0112,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x0013,(uint8_t *)&st_net_status_data_mppt_v1.w_0113,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0014,(uint8_t *)&st_net_status_data_mppt_v1.w_0114,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0015,(uint8_t *)&st_net_status_data_mppt_v1.w_0115,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0016,(uint8_t *)&st_net_status_data_mppt_v1.w_0116,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0017,(uint8_t *)&st_net_status_data_mppt_v1.w_0117,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0018,(uint8_t *)&st_net_status_data_mppt_v1.w_0118,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0019,(uint8_t *)&st_net_status_data_mppt_v1.w_0119,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x001A,(uint8_t *)&st_net_status_data_mppt_v1.w_011A,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x001B,(uint8_t *)&st_net_status_data_mppt_v1.w_011B,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x001C,(uint8_t *)&st_net_status_data_mppt_v1.w_011C,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x001D,(uint8_t *)&st_net_status_data_mppt_v1.w_011D,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x001E,(uint8_t *)&st_net_status_data_mppt_v1.w_011E,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x001F,(uint8_t *)&st_net_status_data_mppt_v1.w_011F,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
const net_data_info st_net_status_data_info_mppt_v1_2[cw_net_status_data_number_mppt] =
{
    {0x0104,0x0020,(uint8_t *)&st_net_status_data_mppt_v1.w_0120,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x0021,(uint8_t *)&st_net_status_data_mppt_v1.w_0121,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0022,(uint8_t *)&st_net_status_data_mppt_v1.w_0122,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0023,(uint8_t *)&st_net_status_data_mppt_v1.w_0123,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0024,(uint8_t *)&st_net_status_data_mppt_v1.w_0124,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0025,(uint8_t *)&st_net_status_data_mppt_v1.w_0125,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0026,(uint8_t *)&st_net_status_data_mppt_v1.w_0126,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0027,(uint8_t *)&st_net_status_data_mppt_v1.w_0127,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0028,(uint8_t *)&st_net_status_data_mppt_v1.w_0128,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0029,(uint8_t *)&st_net_status_data_mppt_v1.w_0129,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x002A,(uint8_t *)&st_net_status_data_mppt_v1.w_012A,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x002B,(uint8_t *)&st_net_status_data_mppt_v1.w_012B,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x002C,(uint8_t *)&st_net_status_data_mppt_v1.w_012C,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x002D,(uint8_t *)&st_net_status_data_mppt_v1.w_012D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
const net_data_info st_net_status_data_info_mppt_v1_3[cw_net_status_data_number_mppt] =
{
    {0x0104,0x002E,(uint8_t *)&st_net_status_data_mppt_v1.w_012E,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x002F,(uint8_t *)&st_net_status_data_mppt_v1.w_012F,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0030,(uint8_t *)&st_net_status_data_mppt_v1.w_0130,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0031,(uint8_t *)&st_net_status_data_mppt_v1.w_0131,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0032,(uint8_t *)&st_net_status_data_mppt_v1.w_0132,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0033,(uint8_t *)&st_net_status_data_mppt_v1.w_0133,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0034,(uint8_t *)&st_net_status_data_mppt_v1.w_0134,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0035,(uint8_t *)&st_net_status_data_mppt_v1.w_0135,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0036,(uint8_t *)&st_net_status_data_mppt_v1.w_0136,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0037,(uint8_t *)&st_net_status_data_mppt_v1.w_0137,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x0038,(uint8_t *)&st_net_status_data_mppt_v1.w_0138,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x0039,(uint8_t *)&st_net_status_data_mppt_v1.w_0139,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x003A,(uint8_t *)&st_net_status_data_mppt_v1.w_013A,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x003B,(uint8_t *)&st_net_status_data_mppt_v1.w_013B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},

};
const net_data_info st_net_status_data_info_mppt_v1_4[cw_net_status_data_number_mppt] =
{
    {0x0104,0x003C,(uint8_t *)&st_net_status_data_mppt_v1.w_013C,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x003D,(uint8_t *)&st_net_status_data_mppt_v1.w_013D,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x003E,(uint8_t *)&st_net_status_data_mppt_v1.w_013E,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x003F,(uint8_t *)&st_net_status_data_mppt_v1.w_013F,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0040,(uint8_t *)&st_net_status_data_mppt_v1.w_0140,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0041,(uint8_t *)&st_net_status_data_mppt_v1.w_0141,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0042,(uint8_t *)&st_net_status_data_mppt_v1.w_0142,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0043,(uint8_t *)&st_net_status_data_mppt_v1.w_0143,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0044,(uint8_t *)&st_net_status_data_mppt_v1.w_0144,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0045,(uint8_t *)&st_net_status_data_mppt_v1.w_0145,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x0046,(uint8_t *)&st_net_status_data_mppt_v1.w_0146,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x0047,(uint8_t *)&st_net_status_data_mppt_v1.w_0147,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x0048,(uint8_t *)&st_net_status_data_mppt_v1.w_0148,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x0049,(uint8_t *)&st_net_status_data_mppt_v1.w_0149,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},

};
const net_data_info st_net_status_data_info_mppt_v1_5[cw_net_status_data_number_mppt] =
{
    {0x0104,0x004A,(uint8_t *)&st_net_status_data_mppt_v1.w_014A,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x004B,(uint8_t *)&st_net_status_data_mppt_v1.w_014B,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x004C,(uint8_t *)&st_net_status_data_mppt_v1.w_014C,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x004D,(uint8_t *)&st_net_status_data_mppt_v1.w_014D,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x004E,(uint8_t *)&st_net_status_data_mppt_v1.w_014E,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x004F,(uint8_t *)&st_net_status_data_mppt_v1.w_014F,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0050,(uint8_t *)&st_net_status_data_mppt_v1.w_0150,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0051,(uint8_t *)&st_net_status_data_mppt_v1.w_0151,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0052,(uint8_t *)&st_net_status_data_mppt_v1.w_0152,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0053,(uint8_t *)&st_net_status_data_mppt_v1.w_0153,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x0054,(uint8_t *)&st_net_status_data_mppt_v1.w_0154,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x0055,(uint8_t *)&st_net_status_data_mppt_v1.w_0155,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x0056,(uint8_t *)&st_net_status_data_mppt_v1.w_0156,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x0057,(uint8_t *)&st_net_status_data_mppt_v1.w_0157,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
/**************************** MPPT RIIO SUN v1 ****************************/
const net_data_info st_net_status_data_info_riiosunii_mppt_v1_0[cw_net_status_data_number_riio_mppt_master] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_mppt_v1.w_0100,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0101,0x0001,(uint8_t *)&st_net_status_data_mppt_v1.w_0101,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0102,0x0002,(uint8_t *)&st_net_status_data_mppt_v1.w_0102,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0103,0x0003,(uint8_t *)&st_net_status_data_mppt_v1.w_0103,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0104,0x0004,(uint8_t *)&st_net_status_data_mppt_v1.w_0104,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0005,(uint8_t *)&st_net_status_data_mppt_v1.w_0105,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0006,(uint8_t *)&st_net_status_data_mppt_v1.w_0106,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0007,(uint8_t *)&st_net_status_data_mppt_v1.w_0107,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0008,(uint8_t *)&st_net_status_data_mppt_v1.w_0108,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0009,(uint8_t *)&st_net_status_data_mppt_v1.w_0109,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x000A,(uint8_t *)&st_net_status_data_mppt_v1.w_010A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x000B,(uint8_t *)&st_net_status_data_mppt_v1.w_010B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x000C,(uint8_t *)&st_net_status_data_mppt_v1.w_010C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x000D,(uint8_t *)&st_net_status_data_mppt_v1.w_010D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x000E,(uint8_t *)&st_net_status_data_mppt_v1.w_010E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x000F,(uint8_t *)&st_net_status_data_mppt_v1.w_010F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0010,(uint8_t *)&st_net_status_data_mppt_v1.w_0110,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0011,(uint8_t *)&st_net_status_data_mppt_v1.w_0111,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0012,(uint8_t *)&st_net_status_data_mppt_v1.w_0112,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0013,(uint8_t *)&st_net_status_data_mppt_v1.w_0113,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_v1_1[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0014,(uint8_t *)&st_net_status_data_mppt_v1.w_0114,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0015,(uint8_t *)&st_net_status_data_mppt_v1.w_0115,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0016,(uint8_t *)&st_net_status_data_mppt_v1.w_0116,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0017,(uint8_t *)&st_net_status_data_mppt_v1.w_0117,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0018,(uint8_t *)&st_net_status_data_mppt_v1.w_0118,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0019,(uint8_t *)&st_net_status_data_mppt_v1.w_0119,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x001A,(uint8_t *)&st_net_status_data_mppt_v1.w_011A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x001B,(uint8_t *)&st_net_status_data_mppt_v1.w_011B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x001C,(uint8_t *)&st_net_status_data_mppt_v1.w_011C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x001D,(uint8_t *)&st_net_status_data_mppt_v1.w_011D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x001E,(uint8_t *)&st_net_status_data_mppt_v1.w_011E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x001F,(uint8_t *)&st_net_status_data_mppt_v1.w_011F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0020,(uint8_t *)&st_net_status_data_mppt_v1.w_0120,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0021,(uint8_t *)&st_net_status_data_mppt_v1.w_0121,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0022,(uint8_t *)&st_net_status_data_mppt_v1.w_0122,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0023,(uint8_t *)&st_net_status_data_mppt_v1.w_0123,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_v1_2[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0024,(uint8_t *)&st_net_status_data_mppt_v1.w_0124,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0025,(uint8_t *)&st_net_status_data_mppt_v1.w_0125,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0026,(uint8_t *)&st_net_status_data_mppt_v1.w_0126,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0027,(uint8_t *)&st_net_status_data_mppt_v1.w_0127,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0028,(uint8_t *)&st_net_status_data_mppt_v1.w_0128,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0029,(uint8_t *)&st_net_status_data_mppt_v1.w_0129,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x002A,(uint8_t *)&st_net_status_data_mppt_v1.w_012A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x002B,(uint8_t *)&st_net_status_data_mppt_v1.w_012B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x002C,(uint8_t *)&st_net_status_data_mppt_v1.w_012C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x002D,(uint8_t *)&st_net_status_data_mppt_v1.w_012D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x002E,(uint8_t *)&st_net_status_data_mppt_v1.w_012E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x002F,(uint8_t *)&st_net_status_data_mppt_v1.w_012F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0030,(uint8_t *)&st_net_status_data_mppt_v1.w_0130,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0031,(uint8_t *)&st_net_status_data_mppt_v1.w_0131,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0032,(uint8_t *)&st_net_status_data_mppt_v1.w_0132,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0033,(uint8_t *)&st_net_status_data_mppt_v1.w_0133,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_v1_3[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0034,(uint8_t *)&st_net_status_data_mppt_v1.w_0134,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0035,(uint8_t *)&st_net_status_data_mppt_v1.w_0135,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0036,(uint8_t *)&st_net_status_data_mppt_v1.w_0136,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0037,(uint8_t *)&st_net_status_data_mppt_v1.w_0137,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0038,(uint8_t *)&st_net_status_data_mppt_v1.w_0138,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0039,(uint8_t *)&st_net_status_data_mppt_v1.w_0139,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x003A,(uint8_t *)&st_net_status_data_mppt_v1.w_013A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x003B,(uint8_t *)&st_net_status_data_mppt_v1.w_013B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x003C,(uint8_t *)&st_net_status_data_mppt_v1.w_013C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x003D,(uint8_t *)&st_net_status_data_mppt_v1.w_013D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x003E,(uint8_t *)&st_net_status_data_mppt_v1.w_013E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x003F,(uint8_t *)&st_net_status_data_mppt_v1.w_013F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0040,(uint8_t *)&st_net_status_data_mppt_v1.w_0140,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0041,(uint8_t *)&st_net_status_data_mppt_v1.w_0141,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0042,(uint8_t *)&st_net_status_data_mppt_v1.w_0142,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0043,(uint8_t *)&st_net_status_data_mppt_v1.w_0143,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_v1_4[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0044,(uint8_t *)&st_net_status_data_mppt_v1.w_0144,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0045,(uint8_t *)&st_net_status_data_mppt_v1.w_0145,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0046,(uint8_t *)&st_net_status_data_mppt_v1.w_0146,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0047,(uint8_t *)&st_net_status_data_mppt_v1.w_0147,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0048,(uint8_t *)&st_net_status_data_mppt_v1.w_0148,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0049,(uint8_t *)&st_net_status_data_mppt_v1.w_0149,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x004A,(uint8_t *)&st_net_status_data_mppt_v1.w_014A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x004B,(uint8_t *)&st_net_status_data_mppt_v1.w_014B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x004C,(uint8_t *)&st_net_status_data_mppt_v1.w_014C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x004D,(uint8_t *)&st_net_status_data_mppt_v1.w_014D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x004E,(uint8_t *)&st_net_status_data_mppt_v1.w_014E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x004F,(uint8_t *)&st_net_status_data_mppt_v1.w_014F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0050,(uint8_t *)&st_net_status_data_mppt_v1.w_0150,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0051,(uint8_t *)&st_net_status_data_mppt_v1.w_0151,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0052,(uint8_t *)&st_net_status_data_mppt_v1.w_0152,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0053,(uint8_t *)&st_net_status_data_mppt_v1.w_0153,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_v1_5[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0054,(uint8_t *)&st_net_status_data_mppt_v1.w_0154,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0055,(uint8_t *)&st_net_status_data_mppt_v1.w_0155,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0056,(uint8_t *)&st_net_status_data_mppt_v1.w_0156,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0057,(uint8_t *)&st_net_status_data_mppt_v1.w_0157,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0058,(uint8_t *)&st_net_status_data_mppt_v1.w_0158,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0059,(uint8_t *)&st_net_status_data_mppt_v1.w_0159,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x005A,(uint8_t *)&st_net_status_data_mppt_v1.w_015A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x005B,(uint8_t *)&st_net_status_data_mppt_v1.w_015B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x005C,(uint8_t *)&st_net_status_data_mppt_v1.w_015C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x005D,(uint8_t *)&st_net_status_data_mppt_v1.w_015D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x005E,(uint8_t *)&st_net_status_data_mppt_v1.w_015E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x005F,(uint8_t *)&st_net_status_data_mppt_v1.w_015F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0060,(uint8_t *)&st_net_status_data_mppt_v1.w_0160,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0061,(uint8_t *)&st_net_status_data_mppt_v1.w_0161,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0062,(uint8_t *)&st_net_status_data_mppt_v1.w_0162,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0063,(uint8_t *)&st_net_status_data_mppt_v1.w_0163,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
/**************************** MPPT v2 ****************************/
net_status_data_mppt st_net_status_data_mppt_v2 =
{
    0,
};
const net_data_info st_net_status_data_info_mppt_v2_0[cw_net_status_data_number_mppt_master] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_mppt_v2.w_0100,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0101,0x0001,(uint8_t *)&st_net_status_data_mppt_v2.w_0101,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0102,0x0002,(uint8_t *)&st_net_status_data_mppt_v2.w_0102,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0103,0x0003,(uint8_t *)&st_net_status_data_mppt_v2.w_0103,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},

    {0x0104,0x0004,(uint8_t *)&st_net_status_data_mppt_v2.w_0104,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x0005,(uint8_t *)&st_net_status_data_mppt_v2.w_0105,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0006,(uint8_t *)&st_net_status_data_mppt_v2.w_0106,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0007,(uint8_t *)&st_net_status_data_mppt_v2.w_0107,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0008,(uint8_t *)&st_net_status_data_mppt_v2.w_0108,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0009,(uint8_t *)&st_net_status_data_mppt_v2.w_0109,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x000A,(uint8_t *)&st_net_status_data_mppt_v2.w_010A,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x000B,(uint8_t *)&st_net_status_data_mppt_v2.w_010B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x000C,(uint8_t *)&st_net_status_data_mppt_v2.w_010C,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x000D,(uint8_t *)&st_net_status_data_mppt_v2.w_010D,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x000E,(uint8_t *)&st_net_status_data_mppt_v2.w_010E,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x000F,(uint8_t *)&st_net_status_data_mppt_v2.w_010F,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x0010,(uint8_t *)&st_net_status_data_mppt_v2.w_0110,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x0011,(uint8_t *)&st_net_status_data_mppt_v2.w_0111,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
const net_data_info st_net_status_data_info_mppt_v2_1[cw_net_status_data_number_mppt] =
{
    {0x0104,0x0012,(uint8_t *)&st_net_status_data_mppt_v2.w_0112,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x0013,(uint8_t *)&st_net_status_data_mppt_v2.w_0113,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0014,(uint8_t *)&st_net_status_data_mppt_v2.w_0114,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0015,(uint8_t *)&st_net_status_data_mppt_v2.w_0115,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0016,(uint8_t *)&st_net_status_data_mppt_v2.w_0116,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0017,(uint8_t *)&st_net_status_data_mppt_v2.w_0117,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0018,(uint8_t *)&st_net_status_data_mppt_v2.w_0118,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0019,(uint8_t *)&st_net_status_data_mppt_v2.w_0119,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x001A,(uint8_t *)&st_net_status_data_mppt_v2.w_011A,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x001B,(uint8_t *)&st_net_status_data_mppt_v2.w_011B,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x001C,(uint8_t *)&st_net_status_data_mppt_v2.w_011C,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x001D,(uint8_t *)&st_net_status_data_mppt_v2.w_011D,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x001E,(uint8_t *)&st_net_status_data_mppt_v2.w_011E,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x001F,(uint8_t *)&st_net_status_data_mppt_v2.w_011F,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
const net_data_info st_net_status_data_info_mppt_v2_2[cw_net_status_data_number_mppt] =
{
    {0x0104,0x0020,(uint8_t *)&st_net_status_data_mppt_v2.w_0120,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x0021,(uint8_t *)&st_net_status_data_mppt_v2.w_0121,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0022,(uint8_t *)&st_net_status_data_mppt_v2.w_0122,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0023,(uint8_t *)&st_net_status_data_mppt_v2.w_0123,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0024,(uint8_t *)&st_net_status_data_mppt_v2.w_0124,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0025,(uint8_t *)&st_net_status_data_mppt_v2.w_0125,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0026,(uint8_t *)&st_net_status_data_mppt_v2.w_0126,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0027,(uint8_t *)&st_net_status_data_mppt_v2.w_0127,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0028,(uint8_t *)&st_net_status_data_mppt_v2.w_0128,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0029,(uint8_t *)&st_net_status_data_mppt_v2.w_0129,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x002A,(uint8_t *)&st_net_status_data_mppt_v2.w_012A,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x002B,(uint8_t *)&st_net_status_data_mppt_v2.w_012B,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x002C,(uint8_t *)&st_net_status_data_mppt_v2.w_012C,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x002D,(uint8_t *)&st_net_status_data_mppt_v2.w_012D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
const net_data_info st_net_status_data_info_mppt_v2_3[cw_net_status_data_number_mppt] =
{
    {0x0104,0x002E,(uint8_t *)&st_net_status_data_mppt_v2.w_012E,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x002F,(uint8_t *)&st_net_status_data_mppt_v2.w_012F,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0030,(uint8_t *)&st_net_status_data_mppt_v2.w_0130,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0031,(uint8_t *)&st_net_status_data_mppt_v2.w_0131,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0032,(uint8_t *)&st_net_status_data_mppt_v2.w_0132,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0033,(uint8_t *)&st_net_status_data_mppt_v2.w_0133,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0034,(uint8_t *)&st_net_status_data_mppt_v2.w_0134,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0035,(uint8_t *)&st_net_status_data_mppt_v2.w_0135,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0036,(uint8_t *)&st_net_status_data_mppt_v2.w_0136,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0037,(uint8_t *)&st_net_status_data_mppt_v2.w_0137,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x0038,(uint8_t *)&st_net_status_data_mppt_v2.w_0138,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x0039,(uint8_t *)&st_net_status_data_mppt_v2.w_0139,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x003A,(uint8_t *)&st_net_status_data_mppt_v2.w_013A,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x003B,(uint8_t *)&st_net_status_data_mppt_v2.w_013B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},

};
const net_data_info st_net_status_data_info_mppt_v2_4[cw_net_status_data_number_mppt] =
{
    {0x0104,0x003C,(uint8_t *)&st_net_status_data_mppt_v2.w_013C,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x003D,(uint8_t *)&st_net_status_data_mppt_v2.w_013D,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x003E,(uint8_t *)&st_net_status_data_mppt_v2.w_013E,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x003F,(uint8_t *)&st_net_status_data_mppt_v2.w_013F,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0040,(uint8_t *)&st_net_status_data_mppt_v2.w_0140,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0041,(uint8_t *)&st_net_status_data_mppt_v2.w_0141,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0042,(uint8_t *)&st_net_status_data_mppt_v2.w_0142,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0043,(uint8_t *)&st_net_status_data_mppt_v2.w_0143,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0044,(uint8_t *)&st_net_status_data_mppt_v2.w_0144,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0045,(uint8_t *)&st_net_status_data_mppt_v2.w_0145,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x0046,(uint8_t *)&st_net_status_data_mppt_v2.w_0146,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x0047,(uint8_t *)&st_net_status_data_mppt_v2.w_0147,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x0048,(uint8_t *)&st_net_status_data_mppt_v2.w_0148,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x0049,(uint8_t *)&st_net_status_data_mppt_v2.w_0149,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},

};
const net_data_info st_net_status_data_info_mppt_v2_5[cw_net_status_data_number_mppt] =
{
    {0x0104,0x004A,(uint8_t *)&st_net_status_data_mppt_v2.w_014A,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x004B,(uint8_t *)&st_net_status_data_mppt_v2.w_014B,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x004C,(uint8_t *)&st_net_status_data_mppt_v2.w_014C,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x004D,(uint8_t *)&st_net_status_data_mppt_v2.w_014D,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x004E,(uint8_t *)&st_net_status_data_mppt_v2.w_014E,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x004F,(uint8_t *)&st_net_status_data_mppt_v2.w_014F,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0050,(uint8_t *)&st_net_status_data_mppt_v2.w_0150,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0051,(uint8_t *)&st_net_status_data_mppt_v2.w_0151,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0052,(uint8_t *)&st_net_status_data_mppt_v2.w_0152,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0053,(uint8_t *)&st_net_status_data_mppt_v2.w_0153,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x0054,(uint8_t *)&st_net_status_data_mppt_v2.w_0154,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x0055,(uint8_t *)&st_net_status_data_mppt_v2.w_0155,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x0056,(uint8_t *)&st_net_status_data_mppt_v2.w_0156,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x0057,(uint8_t *)&st_net_status_data_mppt_v2.w_0157,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
/**************************** MPPT RIIO SUN v2 ****************************/
const net_data_info st_net_status_data_info_riiosunii_mppt_v2_0[cw_net_status_data_number_riio_mppt_master] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_mppt_v2.w_0100,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0101,0x0001,(uint8_t *)&st_net_status_data_mppt_v2.w_0101,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0102,0x0002,(uint8_t *)&st_net_status_data_mppt_v2.w_0102,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0103,0x0003,(uint8_t *)&st_net_status_data_mppt_v2.w_0103,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0104,0x0004,(uint8_t *)&st_net_status_data_mppt_v2.w_0104,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0005,(uint8_t *)&st_net_status_data_mppt_v2.w_0105,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0006,(uint8_t *)&st_net_status_data_mppt_v2.w_0106,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0007,(uint8_t *)&st_net_status_data_mppt_v2.w_0107,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0008,(uint8_t *)&st_net_status_data_mppt_v2.w_0108,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0009,(uint8_t *)&st_net_status_data_mppt_v2.w_0109,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x000A,(uint8_t *)&st_net_status_data_mppt_v2.w_010A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x000B,(uint8_t *)&st_net_status_data_mppt_v2.w_010B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x000C,(uint8_t *)&st_net_status_data_mppt_v2.w_010C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x000D,(uint8_t *)&st_net_status_data_mppt_v2.w_010D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x000E,(uint8_t *)&st_net_status_data_mppt_v2.w_010E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x000F,(uint8_t *)&st_net_status_data_mppt_v2.w_010F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0010,(uint8_t *)&st_net_status_data_mppt_v2.w_0110,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0011,(uint8_t *)&st_net_status_data_mppt_v2.w_0111,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0012,(uint8_t *)&st_net_status_data_mppt_v2.w_0112,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0013,(uint8_t *)&st_net_status_data_mppt_v2.w_0113,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_v2_1[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0014,(uint8_t *)&st_net_status_data_mppt_v2.w_0114,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0015,(uint8_t *)&st_net_status_data_mppt_v2.w_0115,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0016,(uint8_t *)&st_net_status_data_mppt_v2.w_0116,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0017,(uint8_t *)&st_net_status_data_mppt_v2.w_0117,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0018,(uint8_t *)&st_net_status_data_mppt_v2.w_0118,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0019,(uint8_t *)&st_net_status_data_mppt_v2.w_0119,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x001A,(uint8_t *)&st_net_status_data_mppt_v2.w_011A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x001B,(uint8_t *)&st_net_status_data_mppt_v2.w_011B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x001C,(uint8_t *)&st_net_status_data_mppt_v2.w_011C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x001D,(uint8_t *)&st_net_status_data_mppt_v2.w_011D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x001E,(uint8_t *)&st_net_status_data_mppt_v2.w_011E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x001F,(uint8_t *)&st_net_status_data_mppt_v2.w_011F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0020,(uint8_t *)&st_net_status_data_mppt_v2.w_0120,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0021,(uint8_t *)&st_net_status_data_mppt_v2.w_0121,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0022,(uint8_t *)&st_net_status_data_mppt_v2.w_0122,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0023,(uint8_t *)&st_net_status_data_mppt_v2.w_0123,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_v2_2[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0024,(uint8_t *)&st_net_status_data_mppt_v2.w_0124,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0025,(uint8_t *)&st_net_status_data_mppt_v2.w_0125,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0026,(uint8_t *)&st_net_status_data_mppt_v2.w_0126,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0027,(uint8_t *)&st_net_status_data_mppt_v2.w_0127,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0028,(uint8_t *)&st_net_status_data_mppt_v2.w_0128,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0029,(uint8_t *)&st_net_status_data_mppt_v2.w_0129,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x002A,(uint8_t *)&st_net_status_data_mppt_v2.w_012A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x002B,(uint8_t *)&st_net_status_data_mppt_v2.w_012B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x002C,(uint8_t *)&st_net_status_data_mppt_v2.w_012C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x002D,(uint8_t *)&st_net_status_data_mppt_v2.w_012D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x002E,(uint8_t *)&st_net_status_data_mppt_v2.w_012E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x002F,(uint8_t *)&st_net_status_data_mppt_v2.w_012F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0030,(uint8_t *)&st_net_status_data_mppt_v2.w_0130,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0031,(uint8_t *)&st_net_status_data_mppt_v2.w_0131,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0032,(uint8_t *)&st_net_status_data_mppt_v2.w_0132,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0033,(uint8_t *)&st_net_status_data_mppt_v2.w_0133,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_v2_3[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0034,(uint8_t *)&st_net_status_data_mppt_v2.w_0134,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0035,(uint8_t *)&st_net_status_data_mppt_v2.w_0135,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0036,(uint8_t *)&st_net_status_data_mppt_v2.w_0136,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0037,(uint8_t *)&st_net_status_data_mppt_v2.w_0137,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0038,(uint8_t *)&st_net_status_data_mppt_v2.w_0138,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0039,(uint8_t *)&st_net_status_data_mppt_v2.w_0139,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x003A,(uint8_t *)&st_net_status_data_mppt_v2.w_013A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x003B,(uint8_t *)&st_net_status_data_mppt_v2.w_013B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x003C,(uint8_t *)&st_net_status_data_mppt_v2.w_013C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x003D,(uint8_t *)&st_net_status_data_mppt_v2.w_013D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x003E,(uint8_t *)&st_net_status_data_mppt_v2.w_013E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x003F,(uint8_t *)&st_net_status_data_mppt_v2.w_013F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0040,(uint8_t *)&st_net_status_data_mppt_v2.w_0140,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0041,(uint8_t *)&st_net_status_data_mppt_v2.w_0141,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0042,(uint8_t *)&st_net_status_data_mppt_v2.w_0142,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0043,(uint8_t *)&st_net_status_data_mppt_v2.w_0143,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_v2_4[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0044,(uint8_t *)&st_net_status_data_mppt_v2.w_0144,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0045,(uint8_t *)&st_net_status_data_mppt_v2.w_0145,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0046,(uint8_t *)&st_net_status_data_mppt_v2.w_0146,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0047,(uint8_t *)&st_net_status_data_mppt_v2.w_0147,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0048,(uint8_t *)&st_net_status_data_mppt_v2.w_0148,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0049,(uint8_t *)&st_net_status_data_mppt_v2.w_0149,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x004A,(uint8_t *)&st_net_status_data_mppt_v2.w_014A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x004B,(uint8_t *)&st_net_status_data_mppt_v2.w_014B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x004C,(uint8_t *)&st_net_status_data_mppt_v2.w_014C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x004D,(uint8_t *)&st_net_status_data_mppt_v2.w_014D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x004E,(uint8_t *)&st_net_status_data_mppt_v2.w_014E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x004F,(uint8_t *)&st_net_status_data_mppt_v2.w_014F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0050,(uint8_t *)&st_net_status_data_mppt_v2.w_0150,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0051,(uint8_t *)&st_net_status_data_mppt_v2.w_0151,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0052,(uint8_t *)&st_net_status_data_mppt_v2.w_0152,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0053,(uint8_t *)&st_net_status_data_mppt_v2.w_0153,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_v2_5[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0054,(uint8_t *)&st_net_status_data_mppt_v2.w_0154,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0055,(uint8_t *)&st_net_status_data_mppt_v2.w_0155,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0056,(uint8_t *)&st_net_status_data_mppt_v2.w_0156,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0057,(uint8_t *)&st_net_status_data_mppt_v2.w_0157,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0058,(uint8_t *)&st_net_status_data_mppt_v2.w_0158,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0059,(uint8_t *)&st_net_status_data_mppt_v2.w_0159,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x005A,(uint8_t *)&st_net_status_data_mppt_v2.w_015A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x005B,(uint8_t *)&st_net_status_data_mppt_v2.w_015B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x005C,(uint8_t *)&st_net_status_data_mppt_v2.w_015C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x005D,(uint8_t *)&st_net_status_data_mppt_v2.w_015D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x005E,(uint8_t *)&st_net_status_data_mppt_v2.w_015E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x005F,(uint8_t *)&st_net_status_data_mppt_v2.w_015F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0060,(uint8_t *)&st_net_status_data_mppt_v2.w_0160,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0061,(uint8_t *)&st_net_status_data_mppt_v2.w_0161,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0062,(uint8_t *)&st_net_status_data_mppt_v2.w_0162,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0063,(uint8_t *)&st_net_status_data_mppt_v2.w_0163,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
/**************************** MPPT v3 ****************************/
net_status_data_mppt st_net_status_data_mppt_v3 =
{
    0,
};
const net_data_info st_net_status_data_info_mppt_v3_0[cw_net_status_data_number_mppt_master] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_mppt_v3.w_0100,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0101,0x0001,(uint8_t *)&st_net_status_data_mppt_v3.w_0101,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0102,0x0002,(uint8_t *)&st_net_status_data_mppt_v3.w_0102,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0103,0x0003,(uint8_t *)&st_net_status_data_mppt_v3.w_0103,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},

    {0x0104,0x0004,(uint8_t *)&st_net_status_data_mppt_v3.w_0104,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x0005,(uint8_t *)&st_net_status_data_mppt_v3.w_0105,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0006,(uint8_t *)&st_net_status_data_mppt_v3.w_0106,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0007,(uint8_t *)&st_net_status_data_mppt_v3.w_0107,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0008,(uint8_t *)&st_net_status_data_mppt_v3.w_0108,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0009,(uint8_t *)&st_net_status_data_mppt_v3.w_0109,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x000A,(uint8_t *)&st_net_status_data_mppt_v3.w_010A,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x000B,(uint8_t *)&st_net_status_data_mppt_v3.w_010B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x000C,(uint8_t *)&st_net_status_data_mppt_v3.w_010C,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x000D,(uint8_t *)&st_net_status_data_mppt_v3.w_010D,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x000E,(uint8_t *)&st_net_status_data_mppt_v3.w_010E,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x000F,(uint8_t *)&st_net_status_data_mppt_v3.w_010F,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x0010,(uint8_t *)&st_net_status_data_mppt_v3.w_0110,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x0011,(uint8_t *)&st_net_status_data_mppt_v3.w_0111,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
const net_data_info st_net_status_data_info_mppt_v3_1[cw_net_status_data_number_mppt] =
{
    {0x0104,0x0012,(uint8_t *)&st_net_status_data_mppt_v3.w_0112,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x0013,(uint8_t *)&st_net_status_data_mppt_v3.w_0113,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0014,(uint8_t *)&st_net_status_data_mppt_v3.w_0114,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0015,(uint8_t *)&st_net_status_data_mppt_v3.w_0115,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0016,(uint8_t *)&st_net_status_data_mppt_v3.w_0116,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0017,(uint8_t *)&st_net_status_data_mppt_v3.w_0117,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0018,(uint8_t *)&st_net_status_data_mppt_v3.w_0118,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0019,(uint8_t *)&st_net_status_data_mppt_v3.w_0119,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x001A,(uint8_t *)&st_net_status_data_mppt_v3.w_011A,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x001B,(uint8_t *)&st_net_status_data_mppt_v3.w_011B,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x001C,(uint8_t *)&st_net_status_data_mppt_v3.w_011C,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x001D,(uint8_t *)&st_net_status_data_mppt_v3.w_011D,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x001E,(uint8_t *)&st_net_status_data_mppt_v3.w_011E,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x001F,(uint8_t *)&st_net_status_data_mppt_v3.w_011F,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
const net_data_info st_net_status_data_info_mppt_v3_2[cw_net_status_data_number_mppt] =
{
    {0x0104,0x0020,(uint8_t *)&st_net_status_data_mppt_v3.w_0120,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x0021,(uint8_t *)&st_net_status_data_mppt_v3.w_0121,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0022,(uint8_t *)&st_net_status_data_mppt_v3.w_0122,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0023,(uint8_t *)&st_net_status_data_mppt_v3.w_0123,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0024,(uint8_t *)&st_net_status_data_mppt_v3.w_0124,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0025,(uint8_t *)&st_net_status_data_mppt_v3.w_0125,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0026,(uint8_t *)&st_net_status_data_mppt_v3.w_0126,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0027,(uint8_t *)&st_net_status_data_mppt_v3.w_0127,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0028,(uint8_t *)&st_net_status_data_mppt_v3.w_0128,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0029,(uint8_t *)&st_net_status_data_mppt_v3.w_0129,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x002A,(uint8_t *)&st_net_status_data_mppt_v3.w_012A,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x002B,(uint8_t *)&st_net_status_data_mppt_v3.w_012B,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x002C,(uint8_t *)&st_net_status_data_mppt_v3.w_012C,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x002D,(uint8_t *)&st_net_status_data_mppt_v3.w_012D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
const net_data_info st_net_status_data_info_mppt_v3_3[cw_net_status_data_number_mppt] =
{
    {0x0104,0x002E,(uint8_t *)&st_net_status_data_mppt_v3.w_012E,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x002F,(uint8_t *)&st_net_status_data_mppt_v3.w_012F,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0030,(uint8_t *)&st_net_status_data_mppt_v3.w_0130,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0031,(uint8_t *)&st_net_status_data_mppt_v3.w_0131,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0032,(uint8_t *)&st_net_status_data_mppt_v3.w_0132,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0033,(uint8_t *)&st_net_status_data_mppt_v3.w_0133,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0034,(uint8_t *)&st_net_status_data_mppt_v3.w_0134,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0035,(uint8_t *)&st_net_status_data_mppt_v3.w_0135,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0036,(uint8_t *)&st_net_status_data_mppt_v3.w_0136,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0037,(uint8_t *)&st_net_status_data_mppt_v3.w_0137,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x0038,(uint8_t *)&st_net_status_data_mppt_v3.w_0138,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x0039,(uint8_t *)&st_net_status_data_mppt_v3.w_0139,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x003A,(uint8_t *)&st_net_status_data_mppt_v3.w_013A,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x003B,(uint8_t *)&st_net_status_data_mppt_v3.w_013B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},

};
const net_data_info st_net_status_data_info_mppt_v3_4[cw_net_status_data_number_mppt] =
{
    {0x0104,0x003C,(uint8_t *)&st_net_status_data_mppt_v3.w_013C,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x003D,(uint8_t *)&st_net_status_data_mppt_v3.w_013D,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x003E,(uint8_t *)&st_net_status_data_mppt_v3.w_013E,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x003F,(uint8_t *)&st_net_status_data_mppt_v3.w_013F,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0040,(uint8_t *)&st_net_status_data_mppt_v3.w_0140,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0041,(uint8_t *)&st_net_status_data_mppt_v3.w_0141,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0042,(uint8_t *)&st_net_status_data_mppt_v3.w_0142,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0043,(uint8_t *)&st_net_status_data_mppt_v3.w_0143,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0044,(uint8_t *)&st_net_status_data_mppt_v3.w_0144,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0045,(uint8_t *)&st_net_status_data_mppt_v3.w_0145,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x0046,(uint8_t *)&st_net_status_data_mppt_v3.w_0146,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x0047,(uint8_t *)&st_net_status_data_mppt_v3.w_0147,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x0048,(uint8_t *)&st_net_status_data_mppt_v3.w_0148,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x0049,(uint8_t *)&st_net_status_data_mppt_v3.w_0149,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},

};
const net_data_info st_net_status_data_info_mppt_v3_5[cw_net_status_data_number_mppt] =
{
    {0x0104,0x004A,(uint8_t *)&st_net_status_data_mppt_v3.w_014A,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x004B,(uint8_t *)&st_net_status_data_mppt_v3.w_014B,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x004C,(uint8_t *)&st_net_status_data_mppt_v3.w_014C,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x004D,(uint8_t *)&st_net_status_data_mppt_v3.w_014D,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x004E,(uint8_t *)&st_net_status_data_mppt_v3.w_014E,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x004F,(uint8_t *)&st_net_status_data_mppt_v3.w_014F,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0050,(uint8_t *)&st_net_status_data_mppt_v3.w_0150,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0051,(uint8_t *)&st_net_status_data_mppt_v3.w_0151,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0052,(uint8_t *)&st_net_status_data_mppt_v3.w_0152,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0053,(uint8_t *)&st_net_status_data_mppt_v3.w_0153,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x0054,(uint8_t *)&st_net_status_data_mppt_v3.w_0154,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x0055,(uint8_t *)&st_net_status_data_mppt_v3.w_0155,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x0056,(uint8_t *)&st_net_status_data_mppt_v3.w_0156,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x0057,(uint8_t *)&st_net_status_data_mppt_v3.w_0157,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
/**************************** MPPT RIIO SUN v3 ****************************/
const net_data_info st_net_status_data_info_riiosunii_mppt_v3_0[cw_net_status_data_number_riio_mppt_master] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_mppt_v3.w_0100,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0101,0x0001,(uint8_t *)&st_net_status_data_mppt_v3.w_0101,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0102,0x0002,(uint8_t *)&st_net_status_data_mppt_v3.w_0102,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0103,0x0003,(uint8_t *)&st_net_status_data_mppt_v3.w_0103,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0104,0x0004,(uint8_t *)&st_net_status_data_mppt_v3.w_0104,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0005,(uint8_t *)&st_net_status_data_mppt_v3.w_0105,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0006,(uint8_t *)&st_net_status_data_mppt_v3.w_0106,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0007,(uint8_t *)&st_net_status_data_mppt_v3.w_0107,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0008,(uint8_t *)&st_net_status_data_mppt_v3.w_0108,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0009,(uint8_t *)&st_net_status_data_mppt_v3.w_0109,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x000A,(uint8_t *)&st_net_status_data_mppt_v3.w_010A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x000B,(uint8_t *)&st_net_status_data_mppt_v3.w_010B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x000C,(uint8_t *)&st_net_status_data_mppt_v3.w_010C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x000D,(uint8_t *)&st_net_status_data_mppt_v3.w_010D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x000E,(uint8_t *)&st_net_status_data_mppt_v3.w_010E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x000F,(uint8_t *)&st_net_status_data_mppt_v3.w_010F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0010,(uint8_t *)&st_net_status_data_mppt_v3.w_0110,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0011,(uint8_t *)&st_net_status_data_mppt_v3.w_0111,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0012,(uint8_t *)&st_net_status_data_mppt_v3.w_0112,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0013,(uint8_t *)&st_net_status_data_mppt_v3.w_0113,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_v3_1[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0014,(uint8_t *)&st_net_status_data_mppt_v3.w_0114,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0015,(uint8_t *)&st_net_status_data_mppt_v3.w_0115,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0016,(uint8_t *)&st_net_status_data_mppt_v3.w_0116,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0017,(uint8_t *)&st_net_status_data_mppt_v3.w_0117,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0018,(uint8_t *)&st_net_status_data_mppt_v3.w_0118,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0019,(uint8_t *)&st_net_status_data_mppt_v3.w_0119,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x001A,(uint8_t *)&st_net_status_data_mppt_v3.w_011A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x001B,(uint8_t *)&st_net_status_data_mppt_v3.w_011B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x001C,(uint8_t *)&st_net_status_data_mppt_v3.w_011C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x001D,(uint8_t *)&st_net_status_data_mppt_v3.w_011D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x001E,(uint8_t *)&st_net_status_data_mppt_v3.w_011E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x001F,(uint8_t *)&st_net_status_data_mppt_v3.w_011F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0020,(uint8_t *)&st_net_status_data_mppt_v3.w_0120,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0021,(uint8_t *)&st_net_status_data_mppt_v3.w_0121,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0022,(uint8_t *)&st_net_status_data_mppt_v3.w_0122,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0023,(uint8_t *)&st_net_status_data_mppt_v3.w_0123,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_v3_2[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0024,(uint8_t *)&st_net_status_data_mppt_v3.w_0124,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0025,(uint8_t *)&st_net_status_data_mppt_v3.w_0125,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0026,(uint8_t *)&st_net_status_data_mppt_v3.w_0126,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0027,(uint8_t *)&st_net_status_data_mppt_v3.w_0127,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0028,(uint8_t *)&st_net_status_data_mppt_v3.w_0128,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0029,(uint8_t *)&st_net_status_data_mppt_v3.w_0129,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x002A,(uint8_t *)&st_net_status_data_mppt_v3.w_012A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x002B,(uint8_t *)&st_net_status_data_mppt_v3.w_012B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x002C,(uint8_t *)&st_net_status_data_mppt_v3.w_012C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x002D,(uint8_t *)&st_net_status_data_mppt_v3.w_012D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x002E,(uint8_t *)&st_net_status_data_mppt_v3.w_012E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x002F,(uint8_t *)&st_net_status_data_mppt_v3.w_012F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0030,(uint8_t *)&st_net_status_data_mppt_v3.w_0130,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0031,(uint8_t *)&st_net_status_data_mppt_v3.w_0131,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0032,(uint8_t *)&st_net_status_data_mppt_v3.w_0132,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0033,(uint8_t *)&st_net_status_data_mppt_v3.w_0133,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_v3_3[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0034,(uint8_t *)&st_net_status_data_mppt_v3.w_0134,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0035,(uint8_t *)&st_net_status_data_mppt_v3.w_0135,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0036,(uint8_t *)&st_net_status_data_mppt_v3.w_0136,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0037,(uint8_t *)&st_net_status_data_mppt_v3.w_0137,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0038,(uint8_t *)&st_net_status_data_mppt_v3.w_0138,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0039,(uint8_t *)&st_net_status_data_mppt_v3.w_0139,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x003A,(uint8_t *)&st_net_status_data_mppt_v3.w_013A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x003B,(uint8_t *)&st_net_status_data_mppt_v3.w_013B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x003C,(uint8_t *)&st_net_status_data_mppt_v3.w_013C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x003D,(uint8_t *)&st_net_status_data_mppt_v3.w_013D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x003E,(uint8_t *)&st_net_status_data_mppt_v3.w_013E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x003F,(uint8_t *)&st_net_status_data_mppt_v3.w_013F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0040,(uint8_t *)&st_net_status_data_mppt_v3.w_0140,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0041,(uint8_t *)&st_net_status_data_mppt_v3.w_0141,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0042,(uint8_t *)&st_net_status_data_mppt_v3.w_0142,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0043,(uint8_t *)&st_net_status_data_mppt_v3.w_0143,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_v3_4[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0044,(uint8_t *)&st_net_status_data_mppt_v3.w_0144,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0045,(uint8_t *)&st_net_status_data_mppt_v3.w_0145,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0046,(uint8_t *)&st_net_status_data_mppt_v3.w_0146,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0047,(uint8_t *)&st_net_status_data_mppt_v3.w_0147,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0048,(uint8_t *)&st_net_status_data_mppt_v3.w_0148,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0049,(uint8_t *)&st_net_status_data_mppt_v3.w_0149,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x004A,(uint8_t *)&st_net_status_data_mppt_v3.w_014A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x004B,(uint8_t *)&st_net_status_data_mppt_v3.w_014B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x004C,(uint8_t *)&st_net_status_data_mppt_v3.w_014C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x004D,(uint8_t *)&st_net_status_data_mppt_v3.w_014D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x004E,(uint8_t *)&st_net_status_data_mppt_v3.w_014E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x004F,(uint8_t *)&st_net_status_data_mppt_v3.w_014F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0050,(uint8_t *)&st_net_status_data_mppt_v3.w_0150,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0051,(uint8_t *)&st_net_status_data_mppt_v3.w_0151,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0052,(uint8_t *)&st_net_status_data_mppt_v3.w_0152,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0053,(uint8_t *)&st_net_status_data_mppt_v3.w_0153,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_v3_5[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0054,(uint8_t *)&st_net_status_data_mppt_v3.w_0154,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0055,(uint8_t *)&st_net_status_data_mppt_v3.w_0155,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0056,(uint8_t *)&st_net_status_data_mppt_v3.w_0156,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0057,(uint8_t *)&st_net_status_data_mppt_v3.w_0157,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0058,(uint8_t *)&st_net_status_data_mppt_v3.w_0158,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0059,(uint8_t *)&st_net_status_data_mppt_v3.w_0159,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x005A,(uint8_t *)&st_net_status_data_mppt_v3.w_015A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x005B,(uint8_t *)&st_net_status_data_mppt_v3.w_015B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x005C,(uint8_t *)&st_net_status_data_mppt_v3.w_015C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x005D,(uint8_t *)&st_net_status_data_mppt_v3.w_015D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x005E,(uint8_t *)&st_net_status_data_mppt_v3.w_015E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x005F,(uint8_t *)&st_net_status_data_mppt_v3.w_015F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0060,(uint8_t *)&st_net_status_data_mppt_v3.w_0160,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0061,(uint8_t *)&st_net_status_data_mppt_v3.w_0161,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0062,(uint8_t *)&st_net_status_data_mppt_v3.w_0162,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0063,(uint8_t *)&st_net_status_data_mppt_v3.w_0163,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
/**************************** MPPT w1 ****************************/
net_status_data_mppt st_net_status_data_mppt_w1 =
{
    0,
};
const net_data_info st_net_status_data_info_mppt_w1_0[cw_net_status_data_number_mppt_master] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_mppt_w1.w_0100,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0101,0x0001,(uint8_t *)&st_net_status_data_mppt_w1.w_0101,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0102,0x0002,(uint8_t *)&st_net_status_data_mppt_w1.w_0102,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0103,0x0003,(uint8_t *)&st_net_status_data_mppt_w1.w_0103,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},

    {0x0104,0x0004,(uint8_t *)&st_net_status_data_mppt_w1.w_0104,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x0005,(uint8_t *)&st_net_status_data_mppt_w1.w_0105,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0006,(uint8_t *)&st_net_status_data_mppt_w1.w_0106,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0007,(uint8_t *)&st_net_status_data_mppt_w1.w_0107,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0008,(uint8_t *)&st_net_status_data_mppt_w1.w_0108,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0009,(uint8_t *)&st_net_status_data_mppt_w1.w_0109,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x000A,(uint8_t *)&st_net_status_data_mppt_w1.w_010A,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x000B,(uint8_t *)&st_net_status_data_mppt_w1.w_010B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x000C,(uint8_t *)&st_net_status_data_mppt_w1.w_010C,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x000D,(uint8_t *)&st_net_status_data_mppt_w1.w_010D,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x000E,(uint8_t *)&st_net_status_data_mppt_w1.w_010E,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x000F,(uint8_t *)&st_net_status_data_mppt_w1.w_010F,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x0010,(uint8_t *)&st_net_status_data_mppt_w1.w_0110,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x0011,(uint8_t *)&st_net_status_data_mppt_w1.w_0111,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
const net_data_info st_net_status_data_info_mppt_w1_1[cw_net_status_data_number_mppt] =
{
    {0x0104,0x0012,(uint8_t *)&st_net_status_data_mppt_w1.w_0112,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x0013,(uint8_t *)&st_net_status_data_mppt_w1.w_0113,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0014,(uint8_t *)&st_net_status_data_mppt_w1.w_0114,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0015,(uint8_t *)&st_net_status_data_mppt_w1.w_0115,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0016,(uint8_t *)&st_net_status_data_mppt_w1.w_0116,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0017,(uint8_t *)&st_net_status_data_mppt_w1.w_0117,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0018,(uint8_t *)&st_net_status_data_mppt_w1.w_0118,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0019,(uint8_t *)&st_net_status_data_mppt_w1.w_0119,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x001A,(uint8_t *)&st_net_status_data_mppt_w1.w_011A,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x001B,(uint8_t *)&st_net_status_data_mppt_w1.w_011B,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x001C,(uint8_t *)&st_net_status_data_mppt_w1.w_011C,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x001D,(uint8_t *)&st_net_status_data_mppt_w1.w_011D,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x001E,(uint8_t *)&st_net_status_data_mppt_w1.w_011E,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x001F,(uint8_t *)&st_net_status_data_mppt_w1.w_011F,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
const net_data_info st_net_status_data_info_mppt_w1_2[cw_net_status_data_number_mppt] =
{
    {0x0104,0x0020,(uint8_t *)&st_net_status_data_mppt_w1.w_0120,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x0021,(uint8_t *)&st_net_status_data_mppt_w1.w_0121,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0022,(uint8_t *)&st_net_status_data_mppt_w1.w_0122,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0023,(uint8_t *)&st_net_status_data_mppt_w1.w_0123,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0024,(uint8_t *)&st_net_status_data_mppt_w1.w_0124,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0025,(uint8_t *)&st_net_status_data_mppt_w1.w_0125,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0026,(uint8_t *)&st_net_status_data_mppt_w1.w_0126,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0027,(uint8_t *)&st_net_status_data_mppt_w1.w_0127,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0028,(uint8_t *)&st_net_status_data_mppt_w1.w_0128,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0029,(uint8_t *)&st_net_status_data_mppt_w1.w_0129,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x002A,(uint8_t *)&st_net_status_data_mppt_w1.w_012A,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x002B,(uint8_t *)&st_net_status_data_mppt_w1.w_012B,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x002C,(uint8_t *)&st_net_status_data_mppt_w1.w_012C,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x002D,(uint8_t *)&st_net_status_data_mppt_w1.w_012D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
const net_data_info st_net_status_data_info_mppt_w1_3[cw_net_status_data_number_mppt] =
{
    {0x0104,0x002E,(uint8_t *)&st_net_status_data_mppt_w1.w_012E,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x002F,(uint8_t *)&st_net_status_data_mppt_w1.w_012F,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0030,(uint8_t *)&st_net_status_data_mppt_w1.w_0130,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0031,(uint8_t *)&st_net_status_data_mppt_w1.w_0131,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0032,(uint8_t *)&st_net_status_data_mppt_w1.w_0132,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0033,(uint8_t *)&st_net_status_data_mppt_w1.w_0133,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0034,(uint8_t *)&st_net_status_data_mppt_w1.w_0134,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0035,(uint8_t *)&st_net_status_data_mppt_w1.w_0135,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0036,(uint8_t *)&st_net_status_data_mppt_w1.w_0136,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0037,(uint8_t *)&st_net_status_data_mppt_w1.w_0137,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x0038,(uint8_t *)&st_net_status_data_mppt_w1.w_0138,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x0039,(uint8_t *)&st_net_status_data_mppt_w1.w_0139,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x003A,(uint8_t *)&st_net_status_data_mppt_w1.w_013A,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x003B,(uint8_t *)&st_net_status_data_mppt_w1.w_013B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},

};
const net_data_info st_net_status_data_info_mppt_w1_4[cw_net_status_data_number_mppt] =
{
    {0x0104,0x003C,(uint8_t *)&st_net_status_data_mppt_w1.w_013C,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x003D,(uint8_t *)&st_net_status_data_mppt_w1.w_013D,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x003E,(uint8_t *)&st_net_status_data_mppt_w1.w_013E,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x003F,(uint8_t *)&st_net_status_data_mppt_w1.w_013F,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0040,(uint8_t *)&st_net_status_data_mppt_w1.w_0140,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0041,(uint8_t *)&st_net_status_data_mppt_w1.w_0141,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0042,(uint8_t *)&st_net_status_data_mppt_w1.w_0142,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0043,(uint8_t *)&st_net_status_data_mppt_w1.w_0143,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0044,(uint8_t *)&st_net_status_data_mppt_w1.w_0144,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0045,(uint8_t *)&st_net_status_data_mppt_w1.w_0145,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x0046,(uint8_t *)&st_net_status_data_mppt_w1.w_0146,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x0047,(uint8_t *)&st_net_status_data_mppt_w1.w_0147,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x0048,(uint8_t *)&st_net_status_data_mppt_w1.w_0148,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x0049,(uint8_t *)&st_net_status_data_mppt_w1.w_0149,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},

};
const net_data_info st_net_status_data_info_mppt_w1_5[cw_net_status_data_number_mppt] =
{
    {0x0104,0x004A,(uint8_t *)&st_net_status_data_mppt_w1.w_014A,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x004B,(uint8_t *)&st_net_status_data_mppt_w1.w_014B,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x004C,(uint8_t *)&st_net_status_data_mppt_w1.w_014C,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x004D,(uint8_t *)&st_net_status_data_mppt_w1.w_014D,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x004E,(uint8_t *)&st_net_status_data_mppt_w1.w_014E,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x004F,(uint8_t *)&st_net_status_data_mppt_w1.w_014F,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0050,(uint8_t *)&st_net_status_data_mppt_w1.w_0150,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0051,(uint8_t *)&st_net_status_data_mppt_w1.w_0151,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0052,(uint8_t *)&st_net_status_data_mppt_w1.w_0152,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0053,(uint8_t *)&st_net_status_data_mppt_w1.w_0153,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x0054,(uint8_t *)&st_net_status_data_mppt_w1.w_0154,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x0055,(uint8_t *)&st_net_status_data_mppt_w1.w_0155,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x0056,(uint8_t *)&st_net_status_data_mppt_w1.w_0156,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x0057,(uint8_t *)&st_net_status_data_mppt_w1.w_0157,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
/**************************** MPPT RIIO SUN w1 ****************************/
const net_data_info st_net_status_data_info_riiosunii_mppt_w1_0[cw_net_status_data_number_riio_mppt_master] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_mppt_w1.w_0100,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0101,0x0001,(uint8_t *)&st_net_status_data_mppt_w1.w_0101,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0102,0x0002,(uint8_t *)&st_net_status_data_mppt_w1.w_0102,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0103,0x0003,(uint8_t *)&st_net_status_data_mppt_w1.w_0103,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0104,0x0004,(uint8_t *)&st_net_status_data_mppt_w1.w_0104,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0005,(uint8_t *)&st_net_status_data_mppt_w1.w_0105,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0006,(uint8_t *)&st_net_status_data_mppt_w1.w_0106,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0007,(uint8_t *)&st_net_status_data_mppt_w1.w_0107,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0008,(uint8_t *)&st_net_status_data_mppt_w1.w_0108,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0009,(uint8_t *)&st_net_status_data_mppt_w1.w_0109,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x000A,(uint8_t *)&st_net_status_data_mppt_w1.w_010A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x000B,(uint8_t *)&st_net_status_data_mppt_w1.w_010B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x000C,(uint8_t *)&st_net_status_data_mppt_w1.w_010C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x000D,(uint8_t *)&st_net_status_data_mppt_w1.w_010D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x000E,(uint8_t *)&st_net_status_data_mppt_w1.w_010E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x000F,(uint8_t *)&st_net_status_data_mppt_w1.w_010F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0010,(uint8_t *)&st_net_status_data_mppt_w1.w_0110,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0011,(uint8_t *)&st_net_status_data_mppt_w1.w_0111,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0012,(uint8_t *)&st_net_status_data_mppt_w1.w_0112,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0013,(uint8_t *)&st_net_status_data_mppt_w1.w_0113,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_w1_1[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0014,(uint8_t *)&st_net_status_data_mppt_w1.w_0114,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0015,(uint8_t *)&st_net_status_data_mppt_w1.w_0115,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0016,(uint8_t *)&st_net_status_data_mppt_w1.w_0116,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0017,(uint8_t *)&st_net_status_data_mppt_w1.w_0117,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0018,(uint8_t *)&st_net_status_data_mppt_w1.w_0118,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0019,(uint8_t *)&st_net_status_data_mppt_w1.w_0119,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x001A,(uint8_t *)&st_net_status_data_mppt_w1.w_011A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x001B,(uint8_t *)&st_net_status_data_mppt_w1.w_011B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x001C,(uint8_t *)&st_net_status_data_mppt_w1.w_011C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x001D,(uint8_t *)&st_net_status_data_mppt_w1.w_011D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x001E,(uint8_t *)&st_net_status_data_mppt_w1.w_011E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x001F,(uint8_t *)&st_net_status_data_mppt_w1.w_011F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0020,(uint8_t *)&st_net_status_data_mppt_w1.w_0120,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0021,(uint8_t *)&st_net_status_data_mppt_w1.w_0121,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0022,(uint8_t *)&st_net_status_data_mppt_w1.w_0122,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0023,(uint8_t *)&st_net_status_data_mppt_w1.w_0123,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_w1_2[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0024,(uint8_t *)&st_net_status_data_mppt_w1.w_0124,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0025,(uint8_t *)&st_net_status_data_mppt_w1.w_0125,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0026,(uint8_t *)&st_net_status_data_mppt_w1.w_0126,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0027,(uint8_t *)&st_net_status_data_mppt_w1.w_0127,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0028,(uint8_t *)&st_net_status_data_mppt_w1.w_0128,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0029,(uint8_t *)&st_net_status_data_mppt_w1.w_0129,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x002A,(uint8_t *)&st_net_status_data_mppt_w1.w_012A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x002B,(uint8_t *)&st_net_status_data_mppt_w1.w_012B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x002C,(uint8_t *)&st_net_status_data_mppt_w1.w_012C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x002D,(uint8_t *)&st_net_status_data_mppt_w1.w_012D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x002E,(uint8_t *)&st_net_status_data_mppt_w1.w_012E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x002F,(uint8_t *)&st_net_status_data_mppt_w1.w_012F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0030,(uint8_t *)&st_net_status_data_mppt_w1.w_0130,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0031,(uint8_t *)&st_net_status_data_mppt_w1.w_0131,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0032,(uint8_t *)&st_net_status_data_mppt_w1.w_0132,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0033,(uint8_t *)&st_net_status_data_mppt_w1.w_0133,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_w1_3[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0034,(uint8_t *)&st_net_status_data_mppt_w1.w_0134,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0035,(uint8_t *)&st_net_status_data_mppt_w1.w_0135,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0036,(uint8_t *)&st_net_status_data_mppt_w1.w_0136,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0037,(uint8_t *)&st_net_status_data_mppt_w1.w_0137,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0038,(uint8_t *)&st_net_status_data_mppt_w1.w_0138,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0039,(uint8_t *)&st_net_status_data_mppt_w1.w_0139,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x003A,(uint8_t *)&st_net_status_data_mppt_w1.w_013A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x003B,(uint8_t *)&st_net_status_data_mppt_w1.w_013B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x003C,(uint8_t *)&st_net_status_data_mppt_w1.w_013C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x003D,(uint8_t *)&st_net_status_data_mppt_w1.w_013D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x003E,(uint8_t *)&st_net_status_data_mppt_w1.w_013E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x003F,(uint8_t *)&st_net_status_data_mppt_w1.w_013F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0040,(uint8_t *)&st_net_status_data_mppt_w1.w_0140,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0041,(uint8_t *)&st_net_status_data_mppt_w1.w_0141,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0042,(uint8_t *)&st_net_status_data_mppt_w1.w_0142,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0043,(uint8_t *)&st_net_status_data_mppt_w1.w_0143,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_w1_4[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0044,(uint8_t *)&st_net_status_data_mppt_w1.w_0144,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0045,(uint8_t *)&st_net_status_data_mppt_w1.w_0145,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0046,(uint8_t *)&st_net_status_data_mppt_w1.w_0146,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0047,(uint8_t *)&st_net_status_data_mppt_w1.w_0147,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0048,(uint8_t *)&st_net_status_data_mppt_w1.w_0148,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0049,(uint8_t *)&st_net_status_data_mppt_w1.w_0149,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x004A,(uint8_t *)&st_net_status_data_mppt_w1.w_014A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x004B,(uint8_t *)&st_net_status_data_mppt_w1.w_014B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x004C,(uint8_t *)&st_net_status_data_mppt_w1.w_014C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x004D,(uint8_t *)&st_net_status_data_mppt_w1.w_014D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x004E,(uint8_t *)&st_net_status_data_mppt_w1.w_014E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x004F,(uint8_t *)&st_net_status_data_mppt_w1.w_014F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0050,(uint8_t *)&st_net_status_data_mppt_w1.w_0150,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0051,(uint8_t *)&st_net_status_data_mppt_w1.w_0151,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0052,(uint8_t *)&st_net_status_data_mppt_w1.w_0152,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0053,(uint8_t *)&st_net_status_data_mppt_w1.w_0153,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_w1_5[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0054,(uint8_t *)&st_net_status_data_mppt_w1.w_0154,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0055,(uint8_t *)&st_net_status_data_mppt_w1.w_0155,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0056,(uint8_t *)&st_net_status_data_mppt_w1.w_0156,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0057,(uint8_t *)&st_net_status_data_mppt_w1.w_0157,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0058,(uint8_t *)&st_net_status_data_mppt_w1.w_0158,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0059,(uint8_t *)&st_net_status_data_mppt_w1.w_0159,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x005A,(uint8_t *)&st_net_status_data_mppt_w1.w_015A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x005B,(uint8_t *)&st_net_status_data_mppt_w1.w_015B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x005C,(uint8_t *)&st_net_status_data_mppt_w1.w_015C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x005D,(uint8_t *)&st_net_status_data_mppt_w1.w_015D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x005E,(uint8_t *)&st_net_status_data_mppt_w1.w_015E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x005F,(uint8_t *)&st_net_status_data_mppt_w1.w_015F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0060,(uint8_t *)&st_net_status_data_mppt_w1.w_0160,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0061,(uint8_t *)&st_net_status_data_mppt_w1.w_0161,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0062,(uint8_t *)&st_net_status_data_mppt_w1.w_0162,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0063,(uint8_t *)&st_net_status_data_mppt_w1.w_0163,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
/**************************** MPPT w2 ****************************/
net_status_data_mppt st_net_status_data_mppt_w2 =
{
    0,
};
const net_data_info st_net_status_data_info_mppt_w2_0[cw_net_status_data_number_mppt_master] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_mppt_w2.w_0100,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0101,0x0001,(uint8_t *)&st_net_status_data_mppt_w2.w_0101,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0102,0x0002,(uint8_t *)&st_net_status_data_mppt_w2.w_0102,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0103,0x0003,(uint8_t *)&st_net_status_data_mppt_w2.w_0103,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},

    {0x0104,0x0004,(uint8_t *)&st_net_status_data_mppt_w2.w_0104,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x0005,(uint8_t *)&st_net_status_data_mppt_w2.w_0105,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0006,(uint8_t *)&st_net_status_data_mppt_w2.w_0106,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0007,(uint8_t *)&st_net_status_data_mppt_w2.w_0107,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0008,(uint8_t *)&st_net_status_data_mppt_w2.w_0108,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0009,(uint8_t *)&st_net_status_data_mppt_w2.w_0109,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x000A,(uint8_t *)&st_net_status_data_mppt_w2.w_010A,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x000B,(uint8_t *)&st_net_status_data_mppt_w2.w_010B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x000C,(uint8_t *)&st_net_status_data_mppt_w2.w_010C,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x000D,(uint8_t *)&st_net_status_data_mppt_w2.w_010D,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x000E,(uint8_t *)&st_net_status_data_mppt_w2.w_010E,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x000F,(uint8_t *)&st_net_status_data_mppt_w2.w_010F,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x0010,(uint8_t *)&st_net_status_data_mppt_w2.w_0110,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x0011,(uint8_t *)&st_net_status_data_mppt_w2.w_0111,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
const net_data_info st_net_status_data_info_mppt_w2_1[cw_net_status_data_number_mppt] =
{
    {0x0104,0x0012,(uint8_t *)&st_net_status_data_mppt_w2.w_0112,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x0013,(uint8_t *)&st_net_status_data_mppt_w2.w_0113,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0014,(uint8_t *)&st_net_status_data_mppt_w2.w_0114,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0015,(uint8_t *)&st_net_status_data_mppt_w2.w_0115,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0016,(uint8_t *)&st_net_status_data_mppt_w2.w_0116,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0017,(uint8_t *)&st_net_status_data_mppt_w2.w_0117,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0018,(uint8_t *)&st_net_status_data_mppt_w2.w_0118,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0019,(uint8_t *)&st_net_status_data_mppt_w2.w_0119,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x001A,(uint8_t *)&st_net_status_data_mppt_w2.w_011A,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x001B,(uint8_t *)&st_net_status_data_mppt_w2.w_011B,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x001C,(uint8_t *)&st_net_status_data_mppt_w2.w_011C,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x001D,(uint8_t *)&st_net_status_data_mppt_w2.w_011D,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x001E,(uint8_t *)&st_net_status_data_mppt_w2.w_011E,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x001F,(uint8_t *)&st_net_status_data_mppt_w2.w_011F,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
const net_data_info st_net_status_data_info_mppt_w2_2[cw_net_status_data_number_mppt] =
{
    {0x0104,0x0020,(uint8_t *)&st_net_status_data_mppt_w2.w_0120,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x0021,(uint8_t *)&st_net_status_data_mppt_w2.w_0121,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0022,(uint8_t *)&st_net_status_data_mppt_w2.w_0122,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0023,(uint8_t *)&st_net_status_data_mppt_w2.w_0123,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0024,(uint8_t *)&st_net_status_data_mppt_w2.w_0124,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0025,(uint8_t *)&st_net_status_data_mppt_w2.w_0125,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0026,(uint8_t *)&st_net_status_data_mppt_w2.w_0126,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0027,(uint8_t *)&st_net_status_data_mppt_w2.w_0127,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0028,(uint8_t *)&st_net_status_data_mppt_w2.w_0128,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0029,(uint8_t *)&st_net_status_data_mppt_w2.w_0129,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x002A,(uint8_t *)&st_net_status_data_mppt_w2.w_012A,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x002B,(uint8_t *)&st_net_status_data_mppt_w2.w_012B,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x002C,(uint8_t *)&st_net_status_data_mppt_w2.w_012C,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x002D,(uint8_t *)&st_net_status_data_mppt_w2.w_012D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
const net_data_info st_net_status_data_info_mppt_w2_3[cw_net_status_data_number_mppt] =
{
    {0x0104,0x002E,(uint8_t *)&st_net_status_data_mppt_w2.w_012E,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x002F,(uint8_t *)&st_net_status_data_mppt_w2.w_012F,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0030,(uint8_t *)&st_net_status_data_mppt_w2.w_0130,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0031,(uint8_t *)&st_net_status_data_mppt_w2.w_0131,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0032,(uint8_t *)&st_net_status_data_mppt_w2.w_0132,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0033,(uint8_t *)&st_net_status_data_mppt_w2.w_0133,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0034,(uint8_t *)&st_net_status_data_mppt_w2.w_0134,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0035,(uint8_t *)&st_net_status_data_mppt_w2.w_0135,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0036,(uint8_t *)&st_net_status_data_mppt_w2.w_0136,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0037,(uint8_t *)&st_net_status_data_mppt_w2.w_0137,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x0038,(uint8_t *)&st_net_status_data_mppt_w2.w_0138,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x0039,(uint8_t *)&st_net_status_data_mppt_w2.w_0139,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x003A,(uint8_t *)&st_net_status_data_mppt_w2.w_013A,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x003B,(uint8_t *)&st_net_status_data_mppt_w2.w_013B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},

};
const net_data_info st_net_status_data_info_mppt_w2_4[cw_net_status_data_number_mppt] =
{
    {0x0104,0x003C,(uint8_t *)&st_net_status_data_mppt_w2.w_013C,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x003D,(uint8_t *)&st_net_status_data_mppt_w2.w_013D,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x003E,(uint8_t *)&st_net_status_data_mppt_w2.w_013E,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x003F,(uint8_t *)&st_net_status_data_mppt_w2.w_013F,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0040,(uint8_t *)&st_net_status_data_mppt_w2.w_0140,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0041,(uint8_t *)&st_net_status_data_mppt_w2.w_0141,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0042,(uint8_t *)&st_net_status_data_mppt_w2.w_0142,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0043,(uint8_t *)&st_net_status_data_mppt_w2.w_0143,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0044,(uint8_t *)&st_net_status_data_mppt_w2.w_0144,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0045,(uint8_t *)&st_net_status_data_mppt_w2.w_0145,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x0046,(uint8_t *)&st_net_status_data_mppt_w2.w_0146,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x0047,(uint8_t *)&st_net_status_data_mppt_w2.w_0147,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x0048,(uint8_t *)&st_net_status_data_mppt_w2.w_0148,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x0049,(uint8_t *)&st_net_status_data_mppt_w2.w_0149,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},

};
const net_data_info st_net_status_data_info_mppt_w2_5[cw_net_status_data_number_mppt] =
{
    {0x0104,0x004A,(uint8_t *)&st_net_status_data_mppt_w2.w_014A,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x004B,(uint8_t *)&st_net_status_data_mppt_w2.w_014B,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x004C,(uint8_t *)&st_net_status_data_mppt_w2.w_014C,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x004D,(uint8_t *)&st_net_status_data_mppt_w2.w_014D,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x004E,(uint8_t *)&st_net_status_data_mppt_w2.w_014E,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x004F,(uint8_t *)&st_net_status_data_mppt_w2.w_014F,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0050,(uint8_t *)&st_net_status_data_mppt_w2.w_0150,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0051,(uint8_t *)&st_net_status_data_mppt_w2.w_0151,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0052,(uint8_t *)&st_net_status_data_mppt_w2.w_0152,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0053,(uint8_t *)&st_net_status_data_mppt_w2.w_0153,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x0054,(uint8_t *)&st_net_status_data_mppt_w2.w_0154,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x0055,(uint8_t *)&st_net_status_data_mppt_w2.w_0155,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x0056,(uint8_t *)&st_net_status_data_mppt_w2.w_0156,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x0057,(uint8_t *)&st_net_status_data_mppt_w2.w_0157,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
/**************************** MPPT RIIO SUN w2 ****************************/
const net_data_info st_net_status_data_info_riiosunii_mppt_w2_0[cw_net_status_data_number_riio_mppt_master] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_mppt_w2.w_0100,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0101,0x0001,(uint8_t *)&st_net_status_data_mppt_w2.w_0101,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0102,0x0002,(uint8_t *)&st_net_status_data_mppt_w2.w_0102,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0103,0x0003,(uint8_t *)&st_net_status_data_mppt_w2.w_0103,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0104,0x0004,(uint8_t *)&st_net_status_data_mppt_w2.w_0104,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0005,(uint8_t *)&st_net_status_data_mppt_w2.w_0105,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0006,(uint8_t *)&st_net_status_data_mppt_w2.w_0106,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0007,(uint8_t *)&st_net_status_data_mppt_w2.w_0107,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0008,(uint8_t *)&st_net_status_data_mppt_w2.w_0108,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0009,(uint8_t *)&st_net_status_data_mppt_w2.w_0109,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x000A,(uint8_t *)&st_net_status_data_mppt_w2.w_010A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x000B,(uint8_t *)&st_net_status_data_mppt_w2.w_010B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x000C,(uint8_t *)&st_net_status_data_mppt_w2.w_010C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x000D,(uint8_t *)&st_net_status_data_mppt_w2.w_010D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x000E,(uint8_t *)&st_net_status_data_mppt_w2.w_010E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x000F,(uint8_t *)&st_net_status_data_mppt_w2.w_010F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0010,(uint8_t *)&st_net_status_data_mppt_w2.w_0110,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0011,(uint8_t *)&st_net_status_data_mppt_w2.w_0111,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0012,(uint8_t *)&st_net_status_data_mppt_w2.w_0112,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0013,(uint8_t *)&st_net_status_data_mppt_w2.w_0113,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_w2_1[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0014,(uint8_t *)&st_net_status_data_mppt_w2.w_0114,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0015,(uint8_t *)&st_net_status_data_mppt_w2.w_0115,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0016,(uint8_t *)&st_net_status_data_mppt_w2.w_0116,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0017,(uint8_t *)&st_net_status_data_mppt_w2.w_0117,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0018,(uint8_t *)&st_net_status_data_mppt_w2.w_0118,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0019,(uint8_t *)&st_net_status_data_mppt_w2.w_0119,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x001A,(uint8_t *)&st_net_status_data_mppt_w2.w_011A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x001B,(uint8_t *)&st_net_status_data_mppt_w2.w_011B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x001C,(uint8_t *)&st_net_status_data_mppt_w2.w_011C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x001D,(uint8_t *)&st_net_status_data_mppt_w2.w_011D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x001E,(uint8_t *)&st_net_status_data_mppt_w2.w_011E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x001F,(uint8_t *)&st_net_status_data_mppt_w2.w_011F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0020,(uint8_t *)&st_net_status_data_mppt_w2.w_0120,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0021,(uint8_t *)&st_net_status_data_mppt_w2.w_0121,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0022,(uint8_t *)&st_net_status_data_mppt_w2.w_0122,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0023,(uint8_t *)&st_net_status_data_mppt_w2.w_0123,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_w2_2[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0024,(uint8_t *)&st_net_status_data_mppt_w2.w_0124,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0025,(uint8_t *)&st_net_status_data_mppt_w2.w_0125,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0026,(uint8_t *)&st_net_status_data_mppt_w2.w_0126,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0027,(uint8_t *)&st_net_status_data_mppt_w2.w_0127,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0028,(uint8_t *)&st_net_status_data_mppt_w2.w_0128,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0029,(uint8_t *)&st_net_status_data_mppt_w2.w_0129,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x002A,(uint8_t *)&st_net_status_data_mppt_w2.w_012A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x002B,(uint8_t *)&st_net_status_data_mppt_w2.w_012B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x002C,(uint8_t *)&st_net_status_data_mppt_w2.w_012C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x002D,(uint8_t *)&st_net_status_data_mppt_w2.w_012D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x002E,(uint8_t *)&st_net_status_data_mppt_w2.w_012E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x002F,(uint8_t *)&st_net_status_data_mppt_w2.w_012F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0030,(uint8_t *)&st_net_status_data_mppt_w2.w_0130,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0031,(uint8_t *)&st_net_status_data_mppt_w2.w_0131,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0032,(uint8_t *)&st_net_status_data_mppt_w2.w_0132,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0033,(uint8_t *)&st_net_status_data_mppt_w2.w_0133,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_w2_3[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0034,(uint8_t *)&st_net_status_data_mppt_w2.w_0134,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0035,(uint8_t *)&st_net_status_data_mppt_w2.w_0135,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0036,(uint8_t *)&st_net_status_data_mppt_w2.w_0136,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0037,(uint8_t *)&st_net_status_data_mppt_w2.w_0137,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0038,(uint8_t *)&st_net_status_data_mppt_w2.w_0138,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0039,(uint8_t *)&st_net_status_data_mppt_w2.w_0139,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x003A,(uint8_t *)&st_net_status_data_mppt_w2.w_013A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x003B,(uint8_t *)&st_net_status_data_mppt_w2.w_013B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x003C,(uint8_t *)&st_net_status_data_mppt_w2.w_013C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x003D,(uint8_t *)&st_net_status_data_mppt_w2.w_013D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x003E,(uint8_t *)&st_net_status_data_mppt_w2.w_013E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x003F,(uint8_t *)&st_net_status_data_mppt_w2.w_013F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0040,(uint8_t *)&st_net_status_data_mppt_w2.w_0140,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0041,(uint8_t *)&st_net_status_data_mppt_w2.w_0141,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0042,(uint8_t *)&st_net_status_data_mppt_w2.w_0142,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0043,(uint8_t *)&st_net_status_data_mppt_w2.w_0143,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_w2_4[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0044,(uint8_t *)&st_net_status_data_mppt_w2.w_0144,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0045,(uint8_t *)&st_net_status_data_mppt_w2.w_0145,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0046,(uint8_t *)&st_net_status_data_mppt_w2.w_0146,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0047,(uint8_t *)&st_net_status_data_mppt_w2.w_0147,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0048,(uint8_t *)&st_net_status_data_mppt_w2.w_0148,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0049,(uint8_t *)&st_net_status_data_mppt_w2.w_0149,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x004A,(uint8_t *)&st_net_status_data_mppt_w2.w_014A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x004B,(uint8_t *)&st_net_status_data_mppt_w2.w_014B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x004C,(uint8_t *)&st_net_status_data_mppt_w2.w_014C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x004D,(uint8_t *)&st_net_status_data_mppt_w2.w_014D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x004E,(uint8_t *)&st_net_status_data_mppt_w2.w_014E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x004F,(uint8_t *)&st_net_status_data_mppt_w2.w_014F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0050,(uint8_t *)&st_net_status_data_mppt_w2.w_0150,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0051,(uint8_t *)&st_net_status_data_mppt_w2.w_0151,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0052,(uint8_t *)&st_net_status_data_mppt_w2.w_0152,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0053,(uint8_t *)&st_net_status_data_mppt_w2.w_0153,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_w2_5[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0054,(uint8_t *)&st_net_status_data_mppt_w2.w_0154,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0055,(uint8_t *)&st_net_status_data_mppt_w2.w_0155,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0056,(uint8_t *)&st_net_status_data_mppt_w2.w_0156,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0057,(uint8_t *)&st_net_status_data_mppt_w2.w_0157,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0058,(uint8_t *)&st_net_status_data_mppt_w2.w_0158,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0059,(uint8_t *)&st_net_status_data_mppt_w2.w_0159,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x005A,(uint8_t *)&st_net_status_data_mppt_w2.w_015A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x005B,(uint8_t *)&st_net_status_data_mppt_w2.w_015B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x005C,(uint8_t *)&st_net_status_data_mppt_w2.w_015C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x005D,(uint8_t *)&st_net_status_data_mppt_w2.w_015D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x005E,(uint8_t *)&st_net_status_data_mppt_w2.w_015E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x005F,(uint8_t *)&st_net_status_data_mppt_w2.w_015F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0060,(uint8_t *)&st_net_status_data_mppt_w2.w_0160,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0061,(uint8_t *)&st_net_status_data_mppt_w2.w_0161,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0062,(uint8_t *)&st_net_status_data_mppt_w2.w_0162,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0063,(uint8_t *)&st_net_status_data_mppt_w2.w_0163,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
/**************************** MPPT w3 ****************************/
net_status_data_mppt st_net_status_data_mppt_w3 =
{
    0,
};
const net_data_info st_net_status_data_info_mppt_w3_0[cw_net_status_data_number_mppt_master] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_mppt_w3.w_0100,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0101,0x0001,(uint8_t *)&st_net_status_data_mppt_w3.w_0101,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0102,0x0002,(uint8_t *)&st_net_status_data_mppt_w3.w_0102,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0103,0x0003,(uint8_t *)&st_net_status_data_mppt_w3.w_0103,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},

    {0x0104,0x0004,(uint8_t *)&st_net_status_data_mppt_w3.w_0104,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x0005,(uint8_t *)&st_net_status_data_mppt_w3.w_0105,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0006,(uint8_t *)&st_net_status_data_mppt_w3.w_0106,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0007,(uint8_t *)&st_net_status_data_mppt_w3.w_0107,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0008,(uint8_t *)&st_net_status_data_mppt_w3.w_0108,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0009,(uint8_t *)&st_net_status_data_mppt_w3.w_0109,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x000A,(uint8_t *)&st_net_status_data_mppt_w3.w_010A,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x000B,(uint8_t *)&st_net_status_data_mppt_w3.w_010B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x000C,(uint8_t *)&st_net_status_data_mppt_w3.w_010C,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x000D,(uint8_t *)&st_net_status_data_mppt_w3.w_010D,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x000E,(uint8_t *)&st_net_status_data_mppt_w3.w_010E,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x000F,(uint8_t *)&st_net_status_data_mppt_w3.w_010F,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x0010,(uint8_t *)&st_net_status_data_mppt_w3.w_0110,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x0011,(uint8_t *)&st_net_status_data_mppt_w3.w_0111,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
const net_data_info st_net_status_data_info_mppt_w3_1[cw_net_status_data_number_mppt] =
{
    {0x0104,0x0012,(uint8_t *)&st_net_status_data_mppt_w3.w_0112,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x0013,(uint8_t *)&st_net_status_data_mppt_w3.w_0113,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0014,(uint8_t *)&st_net_status_data_mppt_w3.w_0114,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0015,(uint8_t *)&st_net_status_data_mppt_w3.w_0115,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0016,(uint8_t *)&st_net_status_data_mppt_w3.w_0116,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0017,(uint8_t *)&st_net_status_data_mppt_w3.w_0117,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0018,(uint8_t *)&st_net_status_data_mppt_w3.w_0118,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0019,(uint8_t *)&st_net_status_data_mppt_w3.w_0119,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x001A,(uint8_t *)&st_net_status_data_mppt_w3.w_011A,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x001B,(uint8_t *)&st_net_status_data_mppt_w3.w_011B,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x001C,(uint8_t *)&st_net_status_data_mppt_w3.w_011C,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x001D,(uint8_t *)&st_net_status_data_mppt_w3.w_011D,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x001E,(uint8_t *)&st_net_status_data_mppt_w3.w_011E,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x001F,(uint8_t *)&st_net_status_data_mppt_w3.w_011F,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
const net_data_info st_net_status_data_info_mppt_w3_2[cw_net_status_data_number_mppt] =
{
    {0x0104,0x0020,(uint8_t *)&st_net_status_data_mppt_w3.w_0120,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x0021,(uint8_t *)&st_net_status_data_mppt_w3.w_0121,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0022,(uint8_t *)&st_net_status_data_mppt_w3.w_0122,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0023,(uint8_t *)&st_net_status_data_mppt_w3.w_0123,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0024,(uint8_t *)&st_net_status_data_mppt_w3.w_0124,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0025,(uint8_t *)&st_net_status_data_mppt_w3.w_0125,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0026,(uint8_t *)&st_net_status_data_mppt_w3.w_0126,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0027,(uint8_t *)&st_net_status_data_mppt_w3.w_0127,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0028,(uint8_t *)&st_net_status_data_mppt_w3.w_0128,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0029,(uint8_t *)&st_net_status_data_mppt_w3.w_0129,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x002A,(uint8_t *)&st_net_status_data_mppt_w3.w_012A,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x002B,(uint8_t *)&st_net_status_data_mppt_w3.w_012B,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x002C,(uint8_t *)&st_net_status_data_mppt_w3.w_012C,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x002D,(uint8_t *)&st_net_status_data_mppt_w3.w_012D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
const net_data_info st_net_status_data_info_mppt_w3_3[cw_net_status_data_number_mppt] =
{
    {0x0104,0x002E,(uint8_t *)&st_net_status_data_mppt_w3.w_012E,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x002F,(uint8_t *)&st_net_status_data_mppt_w3.w_012F,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x0030,(uint8_t *)&st_net_status_data_mppt_w3.w_0130,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x0031,(uint8_t *)&st_net_status_data_mppt_w3.w_0131,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0032,(uint8_t *)&st_net_status_data_mppt_w3.w_0132,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0033,(uint8_t *)&st_net_status_data_mppt_w3.w_0133,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0034,(uint8_t *)&st_net_status_data_mppt_w3.w_0134,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0035,(uint8_t *)&st_net_status_data_mppt_w3.w_0135,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0036,(uint8_t *)&st_net_status_data_mppt_w3.w_0136,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0037,(uint8_t *)&st_net_status_data_mppt_w3.w_0137,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x0038,(uint8_t *)&st_net_status_data_mppt_w3.w_0138,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x0039,(uint8_t *)&st_net_status_data_mppt_w3.w_0139,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x003A,(uint8_t *)&st_net_status_data_mppt_w3.w_013A,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x003B,(uint8_t *)&st_net_status_data_mppt_w3.w_013B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},

};
const net_data_info st_net_status_data_info_mppt_w3_4[cw_net_status_data_number_mppt] =
{
    {0x0104,0x003C,(uint8_t *)&st_net_status_data_mppt_w3.w_013C,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x003D,(uint8_t *)&st_net_status_data_mppt_w3.w_013D,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x003E,(uint8_t *)&st_net_status_data_mppt_w3.w_013E,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x003F,(uint8_t *)&st_net_status_data_mppt_w3.w_013F,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x0040,(uint8_t *)&st_net_status_data_mppt_w3.w_0140,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x0041,(uint8_t *)&st_net_status_data_mppt_w3.w_0141,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0042,(uint8_t *)&st_net_status_data_mppt_w3.w_0142,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0043,(uint8_t *)&st_net_status_data_mppt_w3.w_0143,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0044,(uint8_t *)&st_net_status_data_mppt_w3.w_0144,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0045,(uint8_t *)&st_net_status_data_mppt_w3.w_0145,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x0046,(uint8_t *)&st_net_status_data_mppt_w3.w_0146,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x0047,(uint8_t *)&st_net_status_data_mppt_w3.w_0147,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x0048,(uint8_t *)&st_net_status_data_mppt_w3.w_0148,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x0049,(uint8_t *)&st_net_status_data_mppt_w3.w_0149,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},

};
const net_data_info st_net_status_data_info_mppt_w3_5[cw_net_status_data_number_mppt] =
{
    {0x0104,0x004A,(uint8_t *)&st_net_status_data_mppt_w3.w_014A,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_mppt_data,},
    {0x0105,0x004B,(uint8_t *)&st_net_status_data_mppt_w3.w_014B,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_mppt_data,},
    {0x0106,0x004C,(uint8_t *)&st_net_status_data_mppt_w3.w_014C,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_mppt_data,},
    {0x0107,0x004D,(uint8_t *)&st_net_status_data_mppt_w3.w_014D,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_mppt_data,},
    {0x0108,0x004E,(uint8_t *)&st_net_status_data_mppt_w3.w_014E,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x0109,0x004F,(uint8_t *)&st_net_status_data_mppt_w3.w_014F,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010A,0x0050,(uint8_t *)&st_net_status_data_mppt_w3.w_0150,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010B,0x0051,(uint8_t *)&st_net_status_data_mppt_w3.w_0151,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
    {0x010C,0x0052,(uint8_t *)&st_net_status_data_mppt_w3.w_0152,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010D,0x0053,(uint8_t *)&st_net_status_data_mppt_w3.w_0153,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_mppt_data,},
    {0x010E,0x0054,(uint8_t *)&st_net_status_data_mppt_w3.w_0154,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x010F,0x0055,(uint8_t *)&st_net_status_data_mppt_w3.w_0155,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},

    {0x0110,0x0056,(uint8_t *)&st_net_status_data_mppt_w3.w_0156,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_mppt_data,},
    {0x0111,0x0057,(uint8_t *)&st_net_status_data_mppt_w3.w_0157,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_mppt_data,},
};
/**************************** MPPT RIIO SUN w3 ****************************/
const net_data_info st_net_status_data_info_riiosunii_mppt_w3_0[cw_net_status_data_number_riio_mppt_master] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_mppt_w3.w_0100,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0101,0x0001,(uint8_t *)&st_net_status_data_mppt_w3.w_0101,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0102,0x0002,(uint8_t *)&st_net_status_data_mppt_w3.w_0102,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0103,0x0003,(uint8_t *)&st_net_status_data_mppt_w3.w_0103,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0104,0x0004,(uint8_t *)&st_net_status_data_mppt_w3.w_0104,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0005,(uint8_t *)&st_net_status_data_mppt_w3.w_0105,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0006,(uint8_t *)&st_net_status_data_mppt_w3.w_0106,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0007,(uint8_t *)&st_net_status_data_mppt_w3.w_0107,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0008,(uint8_t *)&st_net_status_data_mppt_w3.w_0108,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0009,(uint8_t *)&st_net_status_data_mppt_w3.w_0109,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x000A,(uint8_t *)&st_net_status_data_mppt_w3.w_010A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x000B,(uint8_t *)&st_net_status_data_mppt_w3.w_010B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x000C,(uint8_t *)&st_net_status_data_mppt_w3.w_010C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x000D,(uint8_t *)&st_net_status_data_mppt_w3.w_010D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x000E,(uint8_t *)&st_net_status_data_mppt_w3.w_010E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x000F,(uint8_t *)&st_net_status_data_mppt_w3.w_010F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0010,(uint8_t *)&st_net_status_data_mppt_w3.w_0110,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0011,(uint8_t *)&st_net_status_data_mppt_w3.w_0111,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0012,(uint8_t *)&st_net_status_data_mppt_w3.w_0112,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0013,(uint8_t *)&st_net_status_data_mppt_w3.w_0113,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_w3_1[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0014,(uint8_t *)&st_net_status_data_mppt_w3.w_0114,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0015,(uint8_t *)&st_net_status_data_mppt_w3.w_0115,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0016,(uint8_t *)&st_net_status_data_mppt_w3.w_0116,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0017,(uint8_t *)&st_net_status_data_mppt_w3.w_0117,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0018,(uint8_t *)&st_net_status_data_mppt_w3.w_0118,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0019,(uint8_t *)&st_net_status_data_mppt_w3.w_0119,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x001A,(uint8_t *)&st_net_status_data_mppt_w3.w_011A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x001B,(uint8_t *)&st_net_status_data_mppt_w3.w_011B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x001C,(uint8_t *)&st_net_status_data_mppt_w3.w_011C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x001D,(uint8_t *)&st_net_status_data_mppt_w3.w_011D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x001E,(uint8_t *)&st_net_status_data_mppt_w3.w_011E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x001F,(uint8_t *)&st_net_status_data_mppt_w3.w_011F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0020,(uint8_t *)&st_net_status_data_mppt_w3.w_0120,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0021,(uint8_t *)&st_net_status_data_mppt_w3.w_0121,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0022,(uint8_t *)&st_net_status_data_mppt_w3.w_0122,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0023,(uint8_t *)&st_net_status_data_mppt_w3.w_0123,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_w3_2[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0024,(uint8_t *)&st_net_status_data_mppt_w3.w_0124,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0025,(uint8_t *)&st_net_status_data_mppt_w3.w_0125,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0026,(uint8_t *)&st_net_status_data_mppt_w3.w_0126,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0027,(uint8_t *)&st_net_status_data_mppt_w3.w_0127,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0028,(uint8_t *)&st_net_status_data_mppt_w3.w_0128,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0029,(uint8_t *)&st_net_status_data_mppt_w3.w_0129,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x002A,(uint8_t *)&st_net_status_data_mppt_w3.w_012A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x002B,(uint8_t *)&st_net_status_data_mppt_w3.w_012B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x002C,(uint8_t *)&st_net_status_data_mppt_w3.w_012C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x002D,(uint8_t *)&st_net_status_data_mppt_w3.w_012D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x002E,(uint8_t *)&st_net_status_data_mppt_w3.w_012E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x002F,(uint8_t *)&st_net_status_data_mppt_w3.w_012F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0030,(uint8_t *)&st_net_status_data_mppt_w3.w_0130,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0031,(uint8_t *)&st_net_status_data_mppt_w3.w_0131,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0032,(uint8_t *)&st_net_status_data_mppt_w3.w_0132,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0033,(uint8_t *)&st_net_status_data_mppt_w3.w_0133,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_w3_3[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0034,(uint8_t *)&st_net_status_data_mppt_w3.w_0134,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0035,(uint8_t *)&st_net_status_data_mppt_w3.w_0135,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0036,(uint8_t *)&st_net_status_data_mppt_w3.w_0136,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0037,(uint8_t *)&st_net_status_data_mppt_w3.w_0137,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0038,(uint8_t *)&st_net_status_data_mppt_w3.w_0138,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0039,(uint8_t *)&st_net_status_data_mppt_w3.w_0139,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x003A,(uint8_t *)&st_net_status_data_mppt_w3.w_013A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x003B,(uint8_t *)&st_net_status_data_mppt_w3.w_013B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x003C,(uint8_t *)&st_net_status_data_mppt_w3.w_013C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x003D,(uint8_t *)&st_net_status_data_mppt_w3.w_013D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x003E,(uint8_t *)&st_net_status_data_mppt_w3.w_013E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x003F,(uint8_t *)&st_net_status_data_mppt_w3.w_013F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0040,(uint8_t *)&st_net_status_data_mppt_w3.w_0140,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0041,(uint8_t *)&st_net_status_data_mppt_w3.w_0141,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0042,(uint8_t *)&st_net_status_data_mppt_w3.w_0142,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0043,(uint8_t *)&st_net_status_data_mppt_w3.w_0143,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_w3_4[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0044,(uint8_t *)&st_net_status_data_mppt_w3.w_0144,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0045,(uint8_t *)&st_net_status_data_mppt_w3.w_0145,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0046,(uint8_t *)&st_net_status_data_mppt_w3.w_0146,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0047,(uint8_t *)&st_net_status_data_mppt_w3.w_0147,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0048,(uint8_t *)&st_net_status_data_mppt_w3.w_0148,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0049,(uint8_t *)&st_net_status_data_mppt_w3.w_0149,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x004A,(uint8_t *)&st_net_status_data_mppt_w3.w_014A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x004B,(uint8_t *)&st_net_status_data_mppt_w3.w_014B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x004C,(uint8_t *)&st_net_status_data_mppt_w3.w_014C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x004D,(uint8_t *)&st_net_status_data_mppt_w3.w_014D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x004E,(uint8_t *)&st_net_status_data_mppt_w3.w_014E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x004F,(uint8_t *)&st_net_status_data_mppt_w3.w_014F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0050,(uint8_t *)&st_net_status_data_mppt_w3.w_0150,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0051,(uint8_t *)&st_net_status_data_mppt_w3.w_0151,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0052,(uint8_t *)&st_net_status_data_mppt_w3.w_0152,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0053,(uint8_t *)&st_net_status_data_mppt_w3.w_0153,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};
const net_data_info st_net_status_data_info_riiosunii_mppt_w3_5[cw_net_status_data_number_riio_mppt] =
{
    {0x0104,0x0054,(uint8_t *)&st_net_status_data_mppt_w3.w_0154,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0105,0x0055,(uint8_t *)&st_net_status_data_mppt_w3.w_0155,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0106,0x0056,(uint8_t *)&st_net_status_data_mppt_w3.w_0156,&st_net_data_att_r_2_2b_umv_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0107,0x0057,(uint8_t *)&st_net_status_data_mppt_w3.w_0157,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0108,0x0058,(uint8_t *)&st_net_status_data_mppt_w3.w_0158,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0109,0x0059,(uint8_t *)&st_net_status_data_mppt_w3.w_0159,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010A,0x005A,(uint8_t *)&st_net_status_data_mppt_w3.w_015A,&st_net_data_att_r_2_2b_0_0_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010B,0x005B,(uint8_t *)&st_net_status_data_mppt_w3.w_015B,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010C,0x005C,(uint8_t *)&st_net_status_data_mppt_w3.w_015C,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010D,0x005D,(uint8_t *)&st_net_status_data_mppt_w3.w_015D,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010E,0x005E,(uint8_t *)&st_net_status_data_mppt_w3.w_015E,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x010F,0x005F,(uint8_t *)&st_net_status_data_mppt_w3.w_015F,&st_net_data_att_r_2_2b_0_1_kwh_status,&st_device_data_att_r_2_riio_mppt_data,},

    {0x0110,0x0060,(uint8_t *)&st_net_status_data_mppt_w3.w_0160,&st_net_data_att_r_2_2b_C_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0111,0x0061,(uint8_t *)&st_net_status_data_mppt_w3.w_0161,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0112,0x0062,(uint8_t *)&st_net_status_data_mppt_w3.w_0162,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
    {0x0113,0x0063,(uint8_t *)&st_net_status_data_mppt_w3.w_0163,&st_net_data_att_r_2_2b_1_status,&st_device_data_att_r_2_riio_mppt_data,},
};

/////////////////////////////////////////////////////////////////////////
/*******************    MPPT U1     ********************/
//{
const net_data_list st_net_data_list_mppt_u1_0[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt_master,
        &st_net_status_data_info_mppt_u1_0[0],
    },
//	{
//		cb_net_data_att_set,
//		cw_net_set_data_number_mppt,
//		&st_net_set_data_info_mppt_1[0],
//	},
//	{
//		cb_net_data_att_control,
//		cw_net_control_data_number_mppt,
//		&st_net_control_data_info_mppt_1[0],
//	}
};
const net_data_list st_net_data_list_mppt_u1_1[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_u1_1[0],
    },
};
const net_data_list st_net_data_list_mppt_u1_2[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_u1_2[0],
    },
};

const net_data_list st_net_data_list_mppt_u1_3[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_u1_3[0],
    },
};
const net_data_list st_net_data_list_mppt_u1_4[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_u1_4[0],
    },
};
const net_data_list st_net_data_list_mppt_u1_5[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_u1_5[0],
    },
};
//}
/*******************    MPPT RIIO SUN II U1     ********************/
//{
const net_data_list st_net_data_list_riiosunii_mppt_u1_0[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_riio_mppt_master,
        &st_net_status_data_info_riiosunii_mppt_u1_0[0],
    },
//	{
//		cb_net_data_att_set,
//		cw_net_set_data_number_mppt,
//		&st_net_set_data_info_mppt_1[0],
//	},
//	{
//		cb_net_data_att_control,
//		cw_net_control_data_number_mppt,
//		&st_net_control_data_info_mppt_1[0],
//	}
};
const net_data_list st_net_data_list_riiosunii_mppt_u1_1[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_riio_mppt,
        &st_net_status_data_info_riiosunii_mppt_u1_1[0],
    },
};
const net_data_list st_net_data_list_riiosunii_mppt_u1_2[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_u1_2[0],
    },
};

const net_data_list st_net_data_list_riiosunii_mppt_u1_3[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_u1_3[0],
    },
};
const net_data_list st_net_data_list_riiosunii_mppt_u1_4[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_u1_4[0],
    },
};
const net_data_list st_net_data_list_riiosunii_mppt_u1_5[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_u1_5[0],
    },
};
//}
/*******************    MPPT U2     ********************/
//{
const net_data_list st_net_data_list_mppt_u2_0[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt_master,
        &st_net_status_data_info_mppt_u2_0[0],
    },
//	{
//		cb_net_data_att_set,
//		cw_net_set_data_number_mppt,
//		&st_net_set_data_info_mppt_1[0],
//	},
//	{
//		cb_net_data_att_control,
//		cw_net_control_data_number_mppt,
//		&st_net_control_data_info_mppt_1[0],
//	}
};
const net_data_list st_net_data_list_mppt_u2_1[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_u2_1[0],
    },
};
const net_data_list st_net_data_list_mppt_u2_2[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_u2_2[0],
    },
};

const net_data_list st_net_data_list_mppt_u2_3[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_u2_3[0],
    },
};
const net_data_list st_net_data_list_mppt_u2_4[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_u2_4[0],
    },
};
const net_data_list st_net_data_list_mppt_u2_5[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_u2_5[0],
    },
};
//}
/*******************    MPPT RIIO SUN II U2     ********************/
//{
const net_data_list st_net_data_list_riiosunii_mppt_u2_0[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_riio_mppt_master,
        &st_net_status_data_info_riiosunii_mppt_u2_0[0],
    },
//	{
//		cb_net_data_att_set,
//		cw_net_set_data_number_mppt,
//		&st_net_set_data_info_mppt_1[0],
//	},
//	{
//		cb_net_data_att_control,
//		cw_net_control_data_number_mppt,
//		&st_net_control_data_info_mppt_1[0],
//	}
};
const net_data_list st_net_data_list_riiosunii_mppt_u2_1[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_riio_mppt,
        &st_net_status_data_info_riiosunii_mppt_u2_1[0],
    },
};
const net_data_list st_net_data_list_riiosunii_mppt_u2_2[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_u2_2[0],
    },
};

const net_data_list st_net_data_list_riiosunii_mppt_u2_3[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_u2_3[0],
    },
};
const net_data_list st_net_data_list_riiosunii_mppt_u2_4[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_u2_4[0],
    },
};
const net_data_list st_net_data_list_riiosunii_mppt_u2_5[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_u2_5[0],
    },
};
//}
/*******************    MPPT U3     ********************/
//{
const net_data_list st_net_data_list_mppt_u3_0[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt_master,
        &st_net_status_data_info_mppt_u3_0[0],
    },
//	{
//		cb_net_data_att_set,
//		cw_net_set_data_number_mppt,
//		&st_net_set_data_info_mppt_1[0],
//	},
//	{
//		cb_net_data_att_control,
//		cw_net_control_data_number_mppt,
//		&st_net_control_data_info_mppt_1[0],
//	}
};
const net_data_list st_net_data_list_mppt_u3_1[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_u3_1[0],
    },
};
const net_data_list st_net_data_list_mppt_u3_2[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_u3_2[0],
    },
};

const net_data_list st_net_data_list_mppt_u3_3[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_u3_3[0],
    },
};
const net_data_list st_net_data_list_mppt_u3_4[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_u3_4[0],
    },
};
const net_data_list st_net_data_list_mppt_u3_5[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_u3_5[0],
    },
};
//}
/*******************    MPPT RIIO SUN II U3     ********************/
//{
const net_data_list st_net_data_list_riiosunii_mppt_u3_0[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_riio_mppt_master,
        &st_net_status_data_info_riiosunii_mppt_u3_0[0],
    },
//	{
//		cb_net_data_att_set,
//		cw_net_set_data_number_mppt,
//		&st_net_set_data_info_mppt_1[0],
//	},
//	{
//		cb_net_data_att_control,
//		cw_net_control_data_number_mppt,
//		&st_net_control_data_info_mppt_1[0],
//	}
};
const net_data_list st_net_data_list_riiosunii_mppt_u3_1[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_riio_mppt,
        &st_net_status_data_info_riiosunii_mppt_u3_1[0],
    },
};
const net_data_list st_net_data_list_riiosunii_mppt_u3_2[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_u3_2[0],
    },
};

const net_data_list st_net_data_list_riiosunii_mppt_u3_3[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_u3_3[0],
    },
};
const net_data_list st_net_data_list_riiosunii_mppt_u3_4[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_u3_4[0],
    },
};
const net_data_list st_net_data_list_riiosunii_mppt_u3_5[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_u3_5[0],
    },
};
//}
/*******************    MPPT v1     ********************/
//{
const net_data_list st_net_data_list_mppt_v1_0[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt_master,
        &st_net_status_data_info_mppt_v1_0[0],
    },
//	{
//		cb_net_data_att_set,
//		cw_net_set_data_number_mppt,
//		&st_net_set_data_info_mppt_1[0],
//	},
//	{
//		cb_net_data_att_control,
//		cw_net_control_data_number_mppt,
//		&st_net_control_data_info_mppt_1[0],
//	}
};
const net_data_list st_net_data_list_mppt_v1_1[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_v1_1[0],
    },
};
const net_data_list st_net_data_list_mppt_v1_2[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_v1_2[0],
    },
};

const net_data_list st_net_data_list_mppt_v1_3[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_v1_3[0],
    },
};
const net_data_list st_net_data_list_mppt_v1_4[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_v1_4[0],
    },
};
const net_data_list st_net_data_list_mppt_v1_5[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_v1_5[0],
    },
};
//}
/*******************    MPPT RIIO SUN II V1     ********************/
//{
const net_data_list st_net_data_list_riiosunii_mppt_v1_0[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_riio_mppt_master,
        &st_net_status_data_info_riiosunii_mppt_v1_0[0],
    },
//	{
//		cb_net_data_att_set,
//		cw_net_set_data_number_mppt,
//		&st_net_set_data_info_mppt_1[0],
//	},
//	{
//		cb_net_data_att_control,
//		cw_net_control_data_number_mppt,
//		&st_net_control_data_info_mppt_1[0],
//	}
};
const net_data_list st_net_data_list_riiosunii_mppt_v1_1[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_riio_mppt,
        &st_net_status_data_info_riiosunii_mppt_v1_1[0],
    },
};
const net_data_list st_net_data_list_riiosunii_mppt_v1_2[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_v1_2[0],
    },
};

const net_data_list st_net_data_list_riiosunii_mppt_v1_3[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_v1_3[0],
    },
};
const net_data_list st_net_data_list_riiosunii_mppt_v1_4[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_v1_4[0],
    },
};
const net_data_list st_net_data_list_riiosunii_mppt_v1_5[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_v1_5[0],
    },
};
//}
/*******************    MPPT v2     ********************/
//{
const net_data_list st_net_data_list_mppt_v2_0[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt_master,
        &st_net_status_data_info_mppt_v2_0[0],
    },
//	{
//		cb_net_data_att_set,
//		cw_net_set_data_number_mppt,
//		&st_net_set_data_info_mppt_1[0],
//	},
//	{
//		cb_net_data_att_control,
//		cw_net_control_data_number_mppt,
//		&st_net_control_data_info_mppt_1[0],
//	}
};
const net_data_list st_net_data_list_mppt_v2_1[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_v2_1[0],
    },
};
const net_data_list st_net_data_list_mppt_v2_2[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_v2_2[0],
    },
};

const net_data_list st_net_data_list_mppt_v2_3[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_v2_3[0],
    },
};
const net_data_list st_net_data_list_mppt_v2_4[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_v2_4[0],
    },
};
const net_data_list st_net_data_list_mppt_v2_5[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_v2_5[0],
    },
};
//}
/*******************    MPPT RIIO SUN II V2     ********************/
//{
const net_data_list st_net_data_list_riiosunii_mppt_v2_0[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_riio_mppt_master,
        &st_net_status_data_info_riiosunii_mppt_v2_0[0],
    },
//	{
//		cb_net_data_att_set,
//		cw_net_set_data_number_mppt,
//		&st_net_set_data_info_mppt_1[0],
//	},
//	{
//		cb_net_data_att_control,
//		cw_net_control_data_number_mppt,
//		&st_net_control_data_info_mppt_1[0],
//	}
};
const net_data_list st_net_data_list_riiosunii_mppt_v2_1[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_riio_mppt,
        &st_net_status_data_info_riiosunii_mppt_v2_1[0],
    },
};
const net_data_list st_net_data_list_riiosunii_mppt_v2_2[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_v2_2[0],
    },
};

const net_data_list st_net_data_list_riiosunii_mppt_v2_3[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_v2_3[0],
    },
};
const net_data_list st_net_data_list_riiosunii_mppt_v2_4[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_v2_4[0],
    },
};
const net_data_list st_net_data_list_riiosunii_mppt_v2_5[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_v2_5[0],
    },
};
//}
/*******************    MPPT V3     ********************/
//{
const net_data_list st_net_data_list_mppt_v3_0[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt_master,
        &st_net_status_data_info_mppt_v3_0[0],
    },
//	{
//		cb_net_data_att_set,
//		cw_net_set_data_number_mppt,
//		&st_net_set_data_info_mppt_1[0],
//	},
//	{
//		cb_net_data_att_control,
//		cw_net_control_data_number_mppt,
//		&st_net_control_data_info_mppt_1[0],
//	}
};
const net_data_list st_net_data_list_mppt_v3_1[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_v3_1[0],
    },
};
const net_data_list st_net_data_list_mppt_v3_2[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_v3_2[0],
    },
};

const net_data_list st_net_data_list_mppt_v3_3[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_v3_3[0],
    },
};
const net_data_list st_net_data_list_mppt_v3_4[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_v3_4[0],
    },
};
const net_data_list st_net_data_list_mppt_v3_5[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_v3_5[0],
    },
};
//}
/*******************    MPPT RIIO SUN II V3     ********************/
//{
const net_data_list st_net_data_list_riiosunii_mppt_v3_0[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_riio_mppt_master,
        &st_net_status_data_info_riiosunii_mppt_v3_0[0],
    },
//	{
//		cb_net_data_att_set,
//		cw_net_set_data_number_mppt,
//		&st_net_set_data_info_mppt_1[0],
//	},
//	{
//		cb_net_data_att_control,
//		cw_net_control_data_number_mppt,
//		&st_net_control_data_info_mppt_1[0],
//	}
};
const net_data_list st_net_data_list_riiosunii_mppt_v3_1[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_riio_mppt,
        &st_net_status_data_info_riiosunii_mppt_v3_1[0],
    },
};
const net_data_list st_net_data_list_riiosunii_mppt_v3_2[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_v3_2[0],
    },
};

const net_data_list st_net_data_list_riiosunii_mppt_v3_3[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_v3_3[0],
    },
};
const net_data_list st_net_data_list_riiosunii_mppt_v3_4[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_v3_4[0],
    },
};
const net_data_list st_net_data_list_riiosunii_mppt_v3_5[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_v3_5[0],
    },
};
//}
/*******************    MPPT W1     ********************/
//{
const net_data_list st_net_data_list_mppt_w1_0[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt_master,
        &st_net_status_data_info_mppt_w1_0[0],
    },
//	{
//		cb_net_data_att_set,
//		cw_net_set_data_number_mppt,
//		&st_net_set_data_info_mppt_1[0],
//	},
//	{
//		cb_net_data_att_control,
//		cw_net_control_data_number_mppt,
//		&st_net_control_data_info_mppt_1[0],
//	}
};
const net_data_list st_net_data_list_mppt_w1_1[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_w1_1[0],
    },
};
const net_data_list st_net_data_list_mppt_w1_2[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_w1_2[0],
    },
};

const net_data_list st_net_data_list_mppt_w1_3[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_w1_3[0],
    },
};
const net_data_list st_net_data_list_mppt_w1_4[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_w1_4[0],
    },
};
const net_data_list st_net_data_list_mppt_w1_5[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_w1_5[0],
    },
};
//}
/*******************    MPPT RIIO SUN II W1     ********************/
//{
const net_data_list st_net_data_list_riiosunii_mppt_w1_0[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_riio_mppt_master,
        &st_net_status_data_info_riiosunii_mppt_w1_0[0],
    },
//	{
//		cb_net_data_att_set,
//		cw_net_set_data_number_mppt,
//		&st_net_set_data_info_mppt_1[0],
//	},
//	{
//		cb_net_data_att_control,
//		cw_net_control_data_number_mppt,
//		&st_net_control_data_info_mppt_1[0],
//	}
};
const net_data_list st_net_data_list_riiosunii_mppt_w1_1[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_riio_mppt,
        &st_net_status_data_info_riiosunii_mppt_w1_1[0],
    },
};
const net_data_list st_net_data_list_riiosunii_mppt_w1_2[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_w1_2[0],
    },
};

const net_data_list st_net_data_list_riiosunii_mppt_w1_3[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_w1_3[0],
    },
};
const net_data_list st_net_data_list_riiosunii_mppt_w1_4[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_w1_4[0],
    },
};
const net_data_list st_net_data_list_riiosunii_mppt_w1_5[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_w1_5[0],
    },
};
//}
/*******************    MPPT W2     ********************/
//{
const net_data_list st_net_data_list_mppt_w2_0[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt_master,
        &st_net_status_data_info_mppt_w2_0[0],
    },
//	{
//		cb_net_data_att_set,
//		cw_net_set_data_number_mppt,
//		&st_net_set_data_info_mppt_1[0],
//	},
//	{
//		cb_net_data_att_control,
//		cw_net_control_data_number_mppt,
//		&st_net_control_data_info_mppt_1[0],
//	}
};
const net_data_list st_net_data_list_mppt_w2_1[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_w2_1[0],
    },
};
const net_data_list st_net_data_list_mppt_w2_2[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_w2_2[0],
    },
};

const net_data_list st_net_data_list_mppt_w2_3[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_w2_3[0],
    },
};
const net_data_list st_net_data_list_mppt_w2_4[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_w2_4[0],
    },
};
const net_data_list st_net_data_list_mppt_w2_5[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_w2_5[0],
    },
};
//}
/*******************    MPPT RIIO SUN II W2     ********************/
//{
const net_data_list st_net_data_list_riiosunii_mppt_w2_0[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_riio_mppt_master,
        &st_net_status_data_info_riiosunii_mppt_w2_0[0],
    },
//	{
//		cb_net_data_att_set,
//		cw_net_set_data_number_mppt,
//		&st_net_set_data_info_mppt_1[0],
//	},
//	{
//		cb_net_data_att_control,
//		cw_net_control_data_number_mppt,
//		&st_net_control_data_info_mppt_1[0],
//	}
};
const net_data_list st_net_data_list_riiosunii_mppt_w2_1[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_riio_mppt,
        &st_net_status_data_info_riiosunii_mppt_w2_1[0],
    },
};
const net_data_list st_net_data_list_riiosunii_mppt_w2_2[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_w2_2[0],
    },
};

const net_data_list st_net_data_list_riiosunii_mppt_w2_3[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_w2_3[0],
    },
};
const net_data_list st_net_data_list_riiosunii_mppt_w2_4[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_w2_4[0],
    },
};
const net_data_list st_net_data_list_riiosunii_mppt_w2_5[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_w2_5[0],
    },
};
//}
/*******************    MPPT W3     ********************/
//{
const net_data_list st_net_data_list_mppt_w3_0[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt_master,
        &st_net_status_data_info_mppt_w3_0[0],
    },
//	{
//		cb_net_data_att_set,
//		cw_net_set_data_number_mppt,
//		&st_net_set_data_info_mppt_1[0],
//	},
//	{
//		cb_net_data_att_control,
//		cw_net_control_data_number_mppt,
//		&st_net_control_data_info_mppt_1[0],
//	}
};
const net_data_list st_net_data_list_mppt_w3_1[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_w3_1[0],
    },
};
const net_data_list st_net_data_list_mppt_w3_2[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_w3_2[0],
    },
};

const net_data_list st_net_data_list_mppt_w3_3[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_w3_3[0],
    },
};
const net_data_list st_net_data_list_mppt_w3_4[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_w3_4[0],
    },
};
const net_data_list st_net_data_list_mppt_w3_5[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_mppt_w3_5[0],
    },
};
//}
/*******************    MPPT RIIO SUN II W3     ********************/
//{
const net_data_list st_net_data_list_riiosunii_mppt_w3_0[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_riio_mppt_master,
        &st_net_status_data_info_riiosunii_mppt_w3_0[0],
    },
//	{
//		cb_net_data_att_set,
//		cw_net_set_data_number_mppt,
//		&st_net_set_data_info_mppt_1[0],
//	},
//	{
//		cb_net_data_att_control,
//		cw_net_control_data_number_mppt,
//		&st_net_control_data_info_mppt_1[0],
//	}
};
const net_data_list st_net_data_list_riiosunii_mppt_w3_1[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_riio_mppt,
        &st_net_status_data_info_riiosunii_mppt_w3_1[0],
    },
};
const net_data_list st_net_data_list_riiosunii_mppt_w3_2[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_w3_2[0],
    },
};

const net_data_list st_net_data_list_riiosunii_mppt_w3_3[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_w3_3[0],
    },
};
const net_data_list st_net_data_list_riiosunii_mppt_w3_4[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_w3_4[0],
    },
};
const net_data_list st_net_data_list_riiosunii_mppt_w3_5[cw_net_data_list_length_mppt] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_mppt,
        &st_net_status_data_info_riiosunii_mppt_w3_5[0],
    },
};
//}

//////////////////////////////////////////////////////////////////////////////
device_inquire_list st_device_inquire_list_mppt[cw_device_inquire_list_mppt] =
{
    {cw_device_cmd_mppt_data_read,{0x00,},0,TRUE,},
};
device_inquire_list st_device_inquire_list_riiosunii_mppt[cw_device_inquire_list_mppt] =
{
    {cw_device_cmd_mppt_riio_ii_data_read,{0x00,},0,TRUE},
};

//////////////////////////////////////////////////////////////////////////////
const net_data_list_info st_net_data_list_info_mppt_u1_0 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_u1_0[0],
    cw_device_inquire_list_mppt,
    &st_device_inquire_list_mppt[0],
};
const net_data_list_info st_net_data_list_info_mppt_u1_1 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_u1_1[0],
};
const net_data_list_info st_net_data_list_info_mppt_u1_2 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_u1_2[0],
};
const net_data_list_info st_net_data_list_info_mppt_u1_3 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_u1_3[0],
};
const net_data_list_info st_net_data_list_info_mppt_u1_4 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_u1_4[0],
};
const net_data_list_info st_net_data_list_info_mppt_u1_5 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_u1_5[0],
};
/////////////
const net_data_list_info st_net_data_list_info_mppt_u2_0 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_u2_0[0],
    cw_device_inquire_list_mppt,
    &st_device_inquire_list_mppt[0],
};
const net_data_list_info st_net_data_list_info_mppt_u2_1 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_u2_1[0],
};
const net_data_list_info st_net_data_list_info_mppt_u2_2 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_u2_2[0],
};
const net_data_list_info st_net_data_list_info_mppt_u2_3 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_u2_3[0],
};
const net_data_list_info st_net_data_list_info_mppt_u2_4 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_u2_4[0],
};
const net_data_list_info st_net_data_list_info_mppt_u2_5 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_u2_5[0],
};
/////////////////////////
const net_data_list_info st_net_data_list_info_mppt_u3_0 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_u3_0[0],
    cw_device_inquire_list_mppt,
    &st_device_inquire_list_mppt[0],
};
const net_data_list_info st_net_data_list_info_mppt_u3_1 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_u3_1[0],
};
const net_data_list_info st_net_data_list_info_mppt_u3_2 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_u3_2[0],
};
const net_data_list_info st_net_data_list_info_mppt_u3_3 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_u3_3[0],
};
const net_data_list_info st_net_data_list_info_mppt_u3_4 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_u3_4[0],
};
const net_data_list_info st_net_data_list_info_mppt_u3_5 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_u3_5[0],
};
////////////////////
const net_data_list_info st_net_data_list_info_mppt_v1_0 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_v1_0[0],
    cw_device_inquire_list_mppt,
    &st_device_inquire_list_mppt[0],
};
const net_data_list_info st_net_data_list_info_mppt_v1_1 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_v1_1[0],
};
const net_data_list_info st_net_data_list_info_mppt_v1_2 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_v1_2[0],
};
const net_data_list_info st_net_data_list_info_mppt_v1_3 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_v1_3[0],
};
const net_data_list_info st_net_data_list_info_mppt_v1_4 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_v1_4[0],
};
const net_data_list_info st_net_data_list_info_mppt_v1_5 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_v1_5[0],
};
////////////////////////////
const net_data_list_info st_net_data_list_info_mppt_v2_0 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_v2_0[0],
    cw_device_inquire_list_mppt,
    &st_device_inquire_list_mppt[0],
};
const net_data_list_info st_net_data_list_info_mppt_v2_1 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_v2_1[0],
};
const net_data_list_info st_net_data_list_info_mppt_v2_2 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_v2_2[0],
};
const net_data_list_info st_net_data_list_info_mppt_v2_3 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_v2_3[0],
};
const net_data_list_info st_net_data_list_info_mppt_v2_4 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_v2_4[0],
};
const net_data_list_info st_net_data_list_info_mppt_v2_5 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_v2_5[0],
};
//////////////////
const net_data_list_info st_net_data_list_info_mppt_v3_0 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_v3_0[0],
    cw_device_inquire_list_mppt,
    &st_device_inquire_list_mppt[0],
};
const net_data_list_info st_net_data_list_info_mppt_v3_1 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_v3_1[0],
};
const net_data_list_info st_net_data_list_info_mppt_v3_2 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_v3_2[0],
};
const net_data_list_info st_net_data_list_info_mppt_v3_3 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_v3_3[0],
};
const net_data_list_info st_net_data_list_info_mppt_v3_4 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_v3_4[0],
};
const net_data_list_info st_net_data_list_info_mppt_v3_5 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_v3_5[0],
};
////////////////////////
const net_data_list_info st_net_data_list_info_mppt_w1_0 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_w1_0[0],
    cw_device_inquire_list_mppt,
    &st_device_inquire_list_mppt[0],
};
const net_data_list_info st_net_data_list_info_mppt_w1_1 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_w1_1[0],
};
const net_data_list_info st_net_data_list_info_mppt_w1_2 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_w1_2[0],
};
const net_data_list_info st_net_data_list_info_mppt_w1_3 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_w1_3[0],
};
const net_data_list_info st_net_data_list_info_mppt_w1_4 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_w1_4[0],
};
const net_data_list_info st_net_data_list_info_mppt_w1_5 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_w1_5[0],
};
////////////////////
const net_data_list_info st_net_data_list_info_mppt_w2_0 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_w2_0[0],
    cw_device_inquire_list_mppt,
    &st_device_inquire_list_mppt[0],
};
const net_data_list_info st_net_data_list_info_mppt_w2_1 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_w2_1[0],
};
const net_data_list_info st_net_data_list_info_mppt_w2_2 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_w2_2[0],
};
const net_data_list_info st_net_data_list_info_mppt_w2_3 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_w2_3[0],
};
const net_data_list_info st_net_data_list_info_mppt_w2_4 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_w2_4[0],
};
const net_data_list_info st_net_data_list_info_mppt_w2_5 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_w2_5[0],
};
///////////////////
const net_data_list_info st_net_data_list_info_mppt_w3_0 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_w3_0[0],
    cw_device_inquire_list_mppt,
    &st_device_inquire_list_mppt[0],
};
const net_data_list_info st_net_data_list_info_mppt_w3_1 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_w3_1[0],
};
const net_data_list_info st_net_data_list_info_mppt_w3_2 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_w3_2[0],
};
const net_data_list_info st_net_data_list_info_mppt_w3_3 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_w3_3[0],
};
const net_data_list_info st_net_data_list_info_mppt_w3_4 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_w3_4[0],
};
const net_data_list_info st_net_data_list_info_mppt_w3_5 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_mppt_w3_5[0],
};
/************** MPPT RIIO SUN II ****************/
//////////////////////////////////////////////////////////////////////////////
const net_data_list_info st_net_data_list_info_riiosunii_mppt_u1_0 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_u1_0[0],
    cw_device_inquire_list_mppt,
    &st_device_inquire_list_riiosunii_mppt[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_u1_1 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_u1_1[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_u1_2 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_u1_2[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_u1_3 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_u1_3[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_u1_4 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_u1_4[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_u1_5 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_u1_5[0],
};
/////////////
const net_data_list_info st_net_data_list_info_riiosunii_mppt_u2_0 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_u2_0[0],
    cw_device_inquire_list_mppt,
    &st_device_inquire_list_riiosunii_mppt[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_u2_1 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_u2_1[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_u2_2 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_u2_2[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_u2_3 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_u2_3[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_u2_4 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_u2_4[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_u2_5 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_u2_5[0],
};
/////////////////////////
const net_data_list_info st_net_data_list_info_riiosunii_mppt_u3_0 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_u3_0[0],
    cw_device_inquire_list_mppt,
    &st_device_inquire_list_riiosunii_mppt[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_u3_1 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_u3_1[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_u3_2 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_u3_2[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_u3_3 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_u3_3[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_u3_4 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_u3_4[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_u3_5 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_u3_5[0],
};
////////////////////
const net_data_list_info st_net_data_list_info_riiosunii_mppt_v1_0 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_v1_0[0],
    cw_device_inquire_list_mppt,
    &st_device_inquire_list_riiosunii_mppt[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_v1_1 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_v1_1[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_v1_2 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_v1_2[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_v1_3 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_v1_3[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_v1_4 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_v1_4[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_v1_5 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_v1_5[0],
};
////////////////////////////
const net_data_list_info st_net_data_list_info_riiosunii_mppt_v2_0 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_v2_0[0],
    cw_device_inquire_list_mppt,
    &st_device_inquire_list_riiosunii_mppt[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_v2_1 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_v2_1[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_v2_2 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_v2_2[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_v2_3 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_v2_3[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_v2_4 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_v2_4[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_v2_5 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_v2_5[0],
};
//////////////////
const net_data_list_info st_net_data_list_info_riiosunii_mppt_v3_0 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_v3_0[0],
    cw_device_inquire_list_mppt,
    &st_device_inquire_list_riiosunii_mppt[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_v3_1 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_v3_1[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_v3_2 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_v3_2[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_v3_3 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_v3_3[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_v3_4 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_v3_4[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_v3_5 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_v3_5[0],
};
////////////////////////
const net_data_list_info st_net_data_list_info_riiosunii_mppt_w1_0 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_w1_0[0],
    cw_device_inquire_list_mppt,
    &st_device_inquire_list_riiosunii_mppt[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_w1_1 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_w1_1[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_w1_2 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_w1_2[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_w1_3 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_w1_3[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_w1_4 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_w1_4[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_w1_5 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_w1_5[0],
};
////////////////////
const net_data_list_info st_net_data_list_info_riiosunii_mppt_w2_0 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_w2_0[0],
    cw_device_inquire_list_mppt,
    &st_device_inquire_list_riiosunii_mppt[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_w2_1 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_w2_1[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_w2_2 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_w2_2[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_w2_3 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_w2_3[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_w2_4 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_w2_4[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_w2_5 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_w2_5[0],
};
///////////////////
const net_data_list_info st_net_data_list_info_riiosunii_mppt_w3_0 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_w3_0[0],
    cw_device_inquire_list_mppt,
    &st_device_inquire_list_riiosunii_mppt[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_w3_1 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_w3_1[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_w3_2 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_w3_2[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_w3_3 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_w3_3[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_w3_4 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_w3_4[0],
};
const net_data_list_info st_net_data_list_info_riiosunii_mppt_w3_5 =
{
    cw_net_data_list_length_mppt,
    &st_net_data_list_riiosunii_mppt_w3_5[0],
};

/////////////////////////////////
const int pb_net_data_list_info_mppt[cb_max_mppt_num] =
{
    (int)&st_net_data_list_info_mppt_u1_0,
    (int)&st_net_data_list_info_mppt_u1_1,
    (int)&st_net_data_list_info_mppt_u1_2,
    (int)&st_net_data_list_info_mppt_u1_3,
    (int)&st_net_data_list_info_mppt_u1_4,
    (int)&st_net_data_list_info_mppt_u1_5,

    (int)&st_net_data_list_info_mppt_u2_0,
    (int)&st_net_data_list_info_mppt_u2_1,
    (int)&st_net_data_list_info_mppt_u2_2,
    (int)&st_net_data_list_info_mppt_u2_3,
    (int)&st_net_data_list_info_mppt_u2_4,
    (int)&st_net_data_list_info_mppt_u2_5,

    (int)&st_net_data_list_info_mppt_u3_0,
    (int)&st_net_data_list_info_mppt_u3_1,
    (int)&st_net_data_list_info_mppt_u3_2,
    (int)&st_net_data_list_info_mppt_u3_3,
    (int)&st_net_data_list_info_mppt_u3_4,
    (int)&st_net_data_list_info_mppt_u3_5,

    (int)&st_net_data_list_info_mppt_v1_0,
    (int)&st_net_data_list_info_mppt_v1_1,
    (int)&st_net_data_list_info_mppt_v1_2,
    (int)&st_net_data_list_info_mppt_v1_3,
    (int)&st_net_data_list_info_mppt_v1_4,
    (int)&st_net_data_list_info_mppt_v1_5,

    (int)&st_net_data_list_info_mppt_v2_0,
    (int)&st_net_data_list_info_mppt_v2_1,
    (int)&st_net_data_list_info_mppt_v2_2,
    (int)&st_net_data_list_info_mppt_v2_3,
    (int)&st_net_data_list_info_mppt_v2_4,
    (int)&st_net_data_list_info_mppt_v2_5,

    (int)&st_net_data_list_info_mppt_v3_0,
    (int)&st_net_data_list_info_mppt_v3_1,
    (int)&st_net_data_list_info_mppt_v3_2,
    (int)&st_net_data_list_info_mppt_v3_3,
    (int)&st_net_data_list_info_mppt_v3_4,
    (int)&st_net_data_list_info_mppt_v3_5,

    (int)&st_net_data_list_info_mppt_w1_0,
    (int)&st_net_data_list_info_mppt_w1_1,
    (int)&st_net_data_list_info_mppt_w1_2,
    (int)&st_net_data_list_info_mppt_w1_3,
    (int)&st_net_data_list_info_mppt_w1_4,
    (int)&st_net_data_list_info_mppt_w1_5,

    (int)&st_net_data_list_info_mppt_w2_0,
    (int)&st_net_data_list_info_mppt_w2_1,
    (int)&st_net_data_list_info_mppt_w2_2,
    (int)&st_net_data_list_info_mppt_w2_3,
    (int)&st_net_data_list_info_mppt_w2_4,
    (int)&st_net_data_list_info_mppt_w2_5,

    (int)&st_net_data_list_info_mppt_w3_0,
    (int)&st_net_data_list_info_mppt_w3_1,
    (int)&st_net_data_list_info_mppt_w3_2,
    (int)&st_net_data_list_info_mppt_w3_3,
    (int)&st_net_data_list_info_mppt_w3_4,
    (int)&st_net_data_list_info_mppt_w3_5,
};
/////////////riio mppt/////////////
const int pb_net_data_list_info_riiosunii_mppt[cb_max_mppt_num] =
{
    (int)&st_net_data_list_info_riiosunii_mppt_u1_0,
    (int)&st_net_data_list_info_riiosunii_mppt_u1_1,
    (int)&st_net_data_list_info_riiosunii_mppt_u1_2,
    (int)&st_net_data_list_info_riiosunii_mppt_u1_3,
    (int)&st_net_data_list_info_riiosunii_mppt_u1_4,
    (int)&st_net_data_list_info_riiosunii_mppt_u1_5,

    (int)&st_net_data_list_info_riiosunii_mppt_u2_0,
    (int)&st_net_data_list_info_riiosunii_mppt_u2_1,
    (int)&st_net_data_list_info_riiosunii_mppt_u2_2,
    (int)&st_net_data_list_info_riiosunii_mppt_u2_3,
    (int)&st_net_data_list_info_riiosunii_mppt_u2_4,
    (int)&st_net_data_list_info_riiosunii_mppt_u2_5,

    (int)&st_net_data_list_info_riiosunii_mppt_u3_0,
    (int)&st_net_data_list_info_riiosunii_mppt_u3_1,
    (int)&st_net_data_list_info_riiosunii_mppt_u3_2,
    (int)&st_net_data_list_info_riiosunii_mppt_u3_3,
    (int)&st_net_data_list_info_riiosunii_mppt_u3_4,
    (int)&st_net_data_list_info_riiosunii_mppt_u3_5,

    (int)&st_net_data_list_info_riiosunii_mppt_v1_0,
    (int)&st_net_data_list_info_riiosunii_mppt_v1_1,
    (int)&st_net_data_list_info_riiosunii_mppt_v1_2,
    (int)&st_net_data_list_info_riiosunii_mppt_v1_3,
    (int)&st_net_data_list_info_riiosunii_mppt_v1_4,
    (int)&st_net_data_list_info_riiosunii_mppt_v1_5,

    (int)&st_net_data_list_info_riiosunii_mppt_v2_0,
    (int)&st_net_data_list_info_riiosunii_mppt_v2_1,
    (int)&st_net_data_list_info_riiosunii_mppt_v2_2,
    (int)&st_net_data_list_info_riiosunii_mppt_v2_3,
    (int)&st_net_data_list_info_riiosunii_mppt_v2_4,
    (int)&st_net_data_list_info_riiosunii_mppt_v2_5,

    (int)&st_net_data_list_info_riiosunii_mppt_v3_0,
    (int)&st_net_data_list_info_riiosunii_mppt_v3_1,
    (int)&st_net_data_list_info_riiosunii_mppt_v3_2,
    (int)&st_net_data_list_info_riiosunii_mppt_v3_3,
    (int)&st_net_data_list_info_riiosunii_mppt_v3_4,
    (int)&st_net_data_list_info_riiosunii_mppt_v3_5,

    (int)&st_net_data_list_info_riiosunii_mppt_w1_0,
    (int)&st_net_data_list_info_riiosunii_mppt_w1_1,
    (int)&st_net_data_list_info_riiosunii_mppt_w1_2,
    (int)&st_net_data_list_info_riiosunii_mppt_w1_3,
    (int)&st_net_data_list_info_riiosunii_mppt_w1_4,
    (int)&st_net_data_list_info_riiosunii_mppt_w1_5,

    (int)&st_net_data_list_info_riiosunii_mppt_w2_0,
    (int)&st_net_data_list_info_riiosunii_mppt_w2_1,
    (int)&st_net_data_list_info_riiosunii_mppt_w2_2,
    (int)&st_net_data_list_info_riiosunii_mppt_w2_3,
    (int)&st_net_data_list_info_riiosunii_mppt_w2_4,
    (int)&st_net_data_list_info_riiosunii_mppt_w2_5,

    (int)&st_net_data_list_info_riiosunii_mppt_w3_0,
    (int)&st_net_data_list_info_riiosunii_mppt_w3_1,
    (int)&st_net_data_list_info_riiosunii_mppt_w3_2,
    (int)&st_net_data_list_info_riiosunii_mppt_w3_3,
    (int)&st_net_data_list_info_riiosunii_mppt_w3_4,
    (int)&st_net_data_list_info_riiosunii_mppt_w3_5,
};
