#include "net_device_data_cklcd.h"
#include "system_info_general_macro.h"
#include "net_device_set_attribute_cklcd.h"
#include "net_device_data_mppt.h"
#include "net_device_data_pv.h"
//222
#define cw_net_status_data_number_cklcd  		        7
#define cw_net_set_data_number_cklcd  		            142
#define cw_net_control_data_number_cklcd  	            0
#define cw_device_inquire_list_cklcd                    9
#define cw_net_data_list_length_cklcd    		        2

uint8_t b_reserve = 0;

/************************** CKLCD 1 *****************************/
static net_status_data_cklcd st_net_status_data_cklcd_u1 =
{
    0,
};
net_data_info st_net_status_data_info_cklcd_u1[cw_net_status_data_number_cklcd] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_mppt_u1.w_0100,&st_net_data_att_r_2_2b_1_status,},
    {0x0101,0x0000,(uint8_t *)&st_net_status_data_mppt_u1.w_0101,&st_net_data_att_r_2_2b_w_status,},
    {0x0102,0x0000,(uint8_t *)&st_net_status_data_mppt_u1.w_0102,&st_net_data_att_r_2_2b_0_0_1_ua_status,},
    {0x0103,0x0000,(uint8_t *)&st_net_status_data_mppt_u1.w_0103,&st_net_data_att_r_2_2b_0_1_kwh_status,},
    {0x0104,0x0000,(uint8_t *)&st_net_status_data_pv.w_0100,&st_net_data_att_r_1_1b_1_status,},
    {0x0105,0x0000,(uint8_t *)&st_net_status_data_cklcd_u1.w_1005,&st_net_data_att_r_2_2b_w_status,},           //电池功率
    {0x0106,0x0000,(uint8_t *)&st_net_status_data_cklcd_u1.w_1006,&st_net_data_att_r_2_2b_0_1_a_status,},       //电池电流
};

static net_set_data_cklcd st_net_set_data_cklcd_u1 = {0,};
const net_data_info st_net_set_data_info_cklcd_u1[cw_net_set_data_number_cklcd] =
{
    {0x0000,0x0000,(uint8_t *)&st_net_set_data_cklcd_u1.w_0000,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0001,0x0001,(uint8_t *)&st_net_set_data_cklcd_u1.w_0001,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0001],},
    {0x0002,0x0002,(uint8_t *)&st_net_set_data_cklcd_u1.w_0002,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0002],},
    {0x0003,0x0003,(uint8_t *)&st_net_set_data_cklcd_u1.w_0003,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0004,0x0004,(uint8_t *)&st_net_set_data_cklcd_u1.w_0004,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0005,0x0005,(uint8_t *)&st_net_set_data_cklcd_u1.w_0005,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0006,0x0006,(uint8_t *)&st_net_set_data_cklcd_u1.w_0006,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0007,0x0007,(uint8_t *)&st_net_set_data_cklcd_u1.w_0007,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0008,0x0008,(uint8_t *)&st_net_set_data_cklcd_u1.w_0008,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0009,0x0009,(uint8_t *)&st_net_set_data_cklcd_u1.w_0009,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x000A,0x000A,(uint8_t *)&st_net_set_data_cklcd_u1.w_000A,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000B,0x000B,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000C,0x000C,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000D,0x000D,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000E,0x000E,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000F,0x000F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//5

    {0x0010,0x0010,(uint8_t *)&st_net_set_data_cklcd_u1.w_0010,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0011,0x0011,(uint8_t *)&st_net_set_data_cklcd_u1.w_0011,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0012,0x0012,(uint8_t *)&st_net_set_data_cklcd_u1.w_0012,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0013,0x0013,(uint8_t *)&st_net_set_data_cklcd_u1.w_0013,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0013],},
    {0x0014,0x0014,(uint8_t *)&st_net_set_data_cklcd_u1.w_0014,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0014],},
    {0x0015,0x0015,(uint8_t *)&st_net_set_data_cklcd_u1.dw_0015,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0015],},
    {0x0016,0x0016,(uint8_t *)&st_net_set_data_cklcd_u1.dw_0016,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0016],},
    {0x0017,0x0017,(uint8_t *)&st_net_set_data_cklcd_u1.w_0017,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0018,0x0018,(uint8_t *)&st_net_set_data_cklcd_u1.w_0018,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0019,0x0019,(uint8_t *)&st_net_set_data_cklcd_u1.w_0019,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001A,0x001A,(uint8_t *)&st_net_set_data_cklcd_u1.w_001A,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x001A],},
    {0x001B,0x001B,(uint8_t *)&st_net_set_data_cklcd_u1.w_001B,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x001B],},
    {0x001C,0x001C,(uint8_t *)&st_net_set_data_cklcd_u1.w_001C,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x001C],},
    {0x001D,0x001D,(uint8_t *)&st_net_set_data_cklcd_u1.w_001D,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x001D],},
    {0x001E,0x001E,(uint8_t *)&st_net_set_data_cklcd_u1.w_001E,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x001E],},
    {0x001F,0x001F,(uint8_t *)&st_net_set_data_cklcd_u1.w_001F,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x001F],},//16

    {0x0020,0x0020,(uint8_t *)&st_net_set_data_cklcd_u1.w_0020,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0021,0x0021,(uint8_t *)&st_net_set_data_cklcd_u1.w_0021,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0022,0x0022,(uint8_t *)&st_net_set_data_cklcd_u1.w_0022,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0023,0x0023,(uint8_t *)&st_net_set_data_cklcd_u1.w_0023,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0024,0x0024,(uint8_t *)&st_net_set_data_cklcd_u1.w_0024,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0025,0x0025,(uint8_t *)&st_net_set_data_cklcd_u1.w_0025,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0026,0x0026,(uint8_t *)&st_net_set_data_cklcd_u1.w_0026,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0027,0x0027,(uint8_t *)&st_net_set_data_cklcd_u1.w_0027,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0028,0x0028,(uint8_t *)&st_net_set_data_cklcd_u1.w_0028,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0029,0x0029,(uint8_t *)&st_net_set_data_cklcd_u1.w_0029,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002A,0x002A,(uint8_t *)&st_net_set_data_cklcd_u1.dw_002A,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x002A],},
    {0x002B,0x002B,(uint8_t *)&st_net_set_data_cklcd_u1.dw_002B,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x002B],},
    {0x002C,0x002C,(uint8_t *)&st_net_set_data_cklcd_u1.w_002C,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x002C],},
    {0x002D,0x002D,(uint8_t *)&st_net_set_data_cklcd_u1.w_002D,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x002D],},
    {0x002E,0x002E,(uint8_t *)&st_net_set_data_cklcd_u1.w_002E,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x002E],},
    {0x002F,0x002F,(uint8_t *)&st_net_set_data_cklcd_u1.w_002F,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x002F],},//16

    {0x0030,0x0030,(uint8_t *)&st_net_set_data_cklcd_u1.w_0030,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0031,0x0031,(uint8_t *)&st_net_set_data_cklcd_u1.w_0031,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0031],},
    {0x0032,0x0032,(uint8_t *)&st_net_set_data_cklcd_u1.w_0032,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0032],},
    {0x0033,0x0033,(uint8_t *)&st_net_set_data_cklcd_u1.w_0033,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0034,0x0034,(uint8_t *)&st_net_set_data_cklcd_u1.w_0034,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0035,0x0035,(uint8_t *)&st_net_set_data_cklcd_u1.w_0035,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0035],},
    {0x0036,0x0036,(uint8_t *)&st_net_set_data_cklcd_u1.w_0036,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0036],},
    {0x0037,0x0037,(uint8_t *)&st_net_set_data_cklcd_u1.w_0037,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0037],},
    {0x0038,0x0038,(uint8_t *)&st_net_set_data_cklcd_u1.w_0038,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0038],},
    {0x0039,0x0039,(uint8_t *)&st_net_set_data_cklcd_u1.w_0039,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0039],},
    {0x003A,0x003A,(uint8_t *)&st_net_set_data_cklcd_u1.w_003A,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x003A],},
    {0x003B,0x003B,(uint8_t *)&st_net_set_data_cklcd_u1.w_003B,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003C,0x003C,(uint8_t *)&st_net_set_data_cklcd_u1.w_003C,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003D,0x003D,(uint8_t *)&st_net_set_data_cklcd_u1.w_003D,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003E,0x003E,(uint8_t *)&st_net_set_data_cklcd_u1.w_003E,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003F,0x003F,(uint8_t *)&st_net_set_data_cklcd_u1.w_003F,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//14

    {0x0040,0x0040,(uint8_t *)&st_net_set_data_cklcd_u1.w_0040,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0041,0x0041,(uint8_t *)&st_net_set_data_cklcd_u1.dw_0041,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0041],},
    {0x0042,0x0042,(uint8_t *)&st_net_set_data_cklcd_u1.dw_0042,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0042],},
    {0x0043,0x0043,(uint8_t *)&st_net_set_data_cklcd_u1.w_0043,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0043],},
    {0x0044,0x0044,(uint8_t *)&st_net_set_data_cklcd_u1.w_0044,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0044],},
    {0x0045,0x0045,(uint8_t *)&st_net_set_data_cklcd_u1.w_0045,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0046,0x0046,(uint8_t *)&st_net_set_data_cklcd_u1.w_0046,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0047,0x0047,(uint8_t *)&st_net_set_data_cklcd_u1.w_0047,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0048,0x0048,(uint8_t *)&st_net_set_data_cklcd_u1.w_0048,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0049,0x0049,(uint8_t *)&st_net_set_data_cklcd_u1.w_0049,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004A,0x004A,(uint8_t *)&st_net_set_data_cklcd_u1.w_004A,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004B,0x004B,(uint8_t *)&st_net_set_data_cklcd_u1.w_004B,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004C,0x004C,(uint8_t *)&st_net_set_data_cklcd_u1.w_004C,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x004C],},
    {0x004D,0x004D,(uint8_t *)&st_net_set_data_cklcd_u1.w_004D,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x004D],},
    {0x004E,0x004E,(uint8_t *)&st_net_set_data_cklcd_u1.w_004E,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004F,0x004F,(uint8_t *)&st_net_set_data_cklcd_u1.w_004F,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//14

    {0x0050,0x0050,(uint8_t *)&st_net_set_data_cklcd_u1.w_0050,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0051,0x0051,(uint8_t *)&st_net_set_data_cklcd_u1.w_0051,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0051],},
    {0x0052,0x0052,(uint8_t *)&st_net_set_data_cklcd_u1.w_0052,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0052],},
    {0x0053,0x0053,(uint8_t *)&st_net_set_data_cklcd_u1.w_0053,&st_net_data_att_rw_2_2b_min_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0053],},
    {0x0054,0x0054,(uint8_t *)&st_net_set_data_cklcd_u1.w_0054,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0055,0x0055,(uint8_t *)&st_net_set_data_cklcd_u1.w_0055,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0055],},
    {0x0056,0x0056,(uint8_t *)&st_net_set_data_cklcd_u1.w_0056,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0056],},
    {0x0057,0x0057,(uint8_t *)&st_net_set_data_cklcd_u1.w_0057,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0057],},
    {0x0058,0x0058,(uint8_t *)&st_net_set_data_cklcd_u1.w_0058,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0058],},
    {0x0059,0x0059,(uint8_t *)&st_net_set_data_cklcd_u1.w_0059,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x005A,0x005A,(uint8_t *)&st_net_set_data_cklcd_u1.w_005A,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x005B,0x005B,(uint8_t *)&st_net_set_data_cklcd_u1.w_005B,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x005C,0x005C,(uint8_t *)&st_net_set_data_cklcd_u1.w_005C,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x005C],},
    {0x005D,0x005D,(uint8_t *)&st_net_set_data_cklcd_u1.w_005D,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x005E,0x005E,(uint8_t *)&st_net_set_data_cklcd_u1.w_005E,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,
    	.pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x005E],},
    {0x005F,0x005F,(uint8_t *)&st_net_set_data_cklcd_u1.w_005F,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//12

    {0x0060,0x0060,(uint8_t *)&st_net_set_data_cklcd_u1.w_0060,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0061,0x0061,(uint8_t *)&st_net_set_data_cklcd_u1.w_0061,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//2
    {0x0062,0x0062,(uint8_t *)&st_net_set_data_cklcd_u1.w_0062,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0063,0x0063,(uint8_t *)&st_net_set_data_cklcd_u1.w_0063,&st_net_data_att_rw_2_2b_w_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0063],},
    {0x0064,0x0064,(uint8_t *)&st_net_set_data_cklcd_u1.w_0064,&st_net_data_att_rw_2_2b_w_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0064],},
    {0x0065,0x0065,(uint8_t *)&st_net_set_data_cklcd_u1.w_0065,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0065],},
    {0x0066,0x0066,(uint8_t *)&st_net_set_data_cklcd_u1.w_0066,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0066],},
    {0x0067,0x0067,(uint8_t *)&st_net_set_data_cklcd_u1.dw_0067,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0067],},
    {0x0068,0x0068,(uint8_t *)&st_net_set_data_cklcd_u1.dw_0068,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0068],},
    {0x0069,0x0069,(uint8_t *)&st_net_set_data_cklcd_u1.w_0069,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0069],},
    {0x006A,0x006A,(uint8_t *)&st_net_set_data_cklcd_u1.w_006A,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x006A],},
    {0x006B,0x006B,(uint8_t *)&st_net_set_data_cklcd_u1.w_006B,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x006B],},
    {0x006C,0x006C,(uint8_t *)&st_net_set_data_cklcd_u1.w_006C,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x006C],},
    {0x006D,0x006D,(uint8_t *)&st_net_set_data_cklcd_u1.w_006D,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x006D],},
    {0x006E,0x006E,(uint8_t *)&st_net_set_data_cklcd_u1.w_006E,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x006E],},
    {0x006F,0x006F,(uint8_t *)&st_net_set_data_cklcd_u1.w_006F,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},

    {0x0070,0x0070,(uint8_t *)&st_net_set_data_cklcd_u1.dw_0070,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0070],},
    {0x0071,0x0071,(uint8_t *)&st_net_set_data_cklcd_u1.dw_0071,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0071],},
    {0x0072,0x0072,(uint8_t *)&st_net_set_data_cklcd_u1.w_0072,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0072],},//3
    {0x0073,0x0073,(uint8_t *)&st_net_set_data_cklcd_u1.w_0073,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,
    	.pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0073],},
    {0x0074,0x0074,(uint8_t *)&st_net_set_data_cklcd_u1.w_0074,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,
    	.pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0074],},
    {0x0075,0x0075,(uint8_t *)&st_net_set_data_cklcd_u1.w_0075,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0076,0x0006,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0077,0x0007,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0078,0x0008,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0079,0x0009,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007A,0x000A,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007B,0x000B,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007C,0x000C,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007D,0x000D,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007E,0x000E,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007F,0x000F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},

    {0x0080,0x0080,(uint8_t *)&st_net_set_data_cklcd_u1.ba_0080,&st_net_data_att_r_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},//1
//    {0x0090,0x0090,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},
//    {0x00A0,0x00A0,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},
//    {0x00B0,0x00B0,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},
//    {0x00C0,0x00C0,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},

    {0x00D0,0x00D0,(uint8_t *)&st_net_set_data_cklcd_u1.w_00D0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D1,0x00D1,(uint8_t *)&st_net_set_data_cklcd_u1.w_00D1,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D2,0x00D2,(uint8_t *)&st_net_set_data_cklcd_u1.w_00D2,&st_net_data_att_rw_2_2b_h_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x00D3,0x00D3,(uint8_t *)&st_net_set_data_cklcd_u1.w_00D3,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x00D4,0x00D4,(uint8_t *)&st_net_set_data_cklcd_u1.w_00D4,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D5,0x00D5,(uint8_t *)&st_net_set_data_cklcd_u1.dw_00D5,&st_net_data_att_rw_4_4b_1_set,&st_device_data_att_rw_4_cklcd_para,},
//    {0x00D6,0x00D6,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D7,0x00D7,(uint8_t *)&st_net_set_data_cklcd_u1.w_00D7,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D8,0x00D8,(uint8_t *)&st_net_set_data_cklcd_u1.w_00D8,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D9,0x00D9,(uint8_t *)&st_net_set_data_cklcd_u1.w_00D9,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DA,0x00DA,(uint8_t *)&st_net_set_data_cklcd_u1.w_00DA,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_r_2_cklcd_para,},
    {0x00DB,0x00DB,(uint8_t *)&st_net_set_data_cklcd_u1.w_00DB,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DC,0x00DC,(uint8_t *)&st_net_set_data_cklcd_u1.w_00DC,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DD,0x00DD,(uint8_t *)&st_net_set_data_cklcd_u1.w_00DD,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DE,0x00DE,(uint8_t *)&st_net_set_data_cklcd_u1.w_00DE,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DF,0x00DF,(uint8_t *)&st_net_set_data_cklcd_u1.w_00DF,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//15

    {0x00E0,0x00E0,(uint8_t *)&st_net_set_data_cklcd_u1.w_00E0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E1,0x00E1,(uint8_t *)&st_net_set_data_cklcd_u1.w_00E1,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E2,0x00E2,(uint8_t *)&st_net_set_data_cklcd_u1.w_00E2,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E3,0x00E3,(uint8_t *)&st_net_set_data_cklcd_u1.w_00E3,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E4,0x00E4,(uint8_t *)&st_net_set_data_cklcd_u1.w_00E4,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E5,0x00E5,(uint8_t *)&st_net_set_data_cklcd_u1.w_00E5,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E6,0x00E6,(uint8_t *)&st_net_set_data_cklcd_u1.w_00E6,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E7,0x00E7,(uint8_t *)&st_net_set_data_cklcd_u1.w_00E7,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0087],},
    {0x00E8,0x00E8,(uint8_t *)&st_net_set_data_cklcd_u1.w_00E8,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0088],},
    {0x00E9,0x00E9,(uint8_t *)&st_net_set_data_cklcd_u1.w_00E9,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x0089],},
    {0x00EA,0x00EA,(uint8_t *)&st_net_set_data_cklcd_u1.w_00EA,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x008A],},
    {0x00EB,0x00EB,(uint8_t *)&st_net_set_data_cklcd_u1.w_00EB,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x008B],},
    {0x00EC,0x00EC,(uint8_t *)&st_net_set_data_cklcd_u1.w_00EC,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00ED,0x00ED,(uint8_t *)&st_net_set_data_cklcd_u1.w_00ED,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EE,0x00EE,(uint8_t *)&st_net_set_data_cklcd_u1.w_00EE,&st_net_data_att_rw_2_2b_w_set,&st_device_data_att_rw_2_cklcd_para,
        .pst_net_data_set_attribute = &st_net_data_set_attribute_cklcd_u1[0x008E],},
//    {0x00EF,0x000F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//15
};

//net_data_info st_net_control_data_info_cklcd_u1[cw_net_control_data_number_cklcd] =
//{
//    {0x03E8,0x03E8,0,&st_net_data_att_rw_2_2b_1_control,&st_device_data_att_w_2_external_trig},
//};

/************************** CKLCD 2 *****************************/
static net_status_data_cklcd st_net_status_data_cklcd_u2 =
{
    0,
};
net_data_info st_net_status_data_info_cklcd_u2[cw_net_status_data_number_cklcd] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_mppt_u2.w_0100,&st_net_data_att_r_2_2b_1_status,},
    {0x0101,0x0000,(uint8_t *)&st_net_status_data_mppt_u2.w_0101,&st_net_data_att_r_2_2b_w_status,},
    {0x0102,0x0000,(uint8_t *)&st_net_status_data_mppt_u2.w_0102,&st_net_data_att_r_2_2b_0_0_1_ua_status,},
    {0x0103,0x0000,(uint8_t *)&st_net_status_data_mppt_u2.w_0103,&st_net_data_att_r_2_2b_0_1_kwh_status,},
    {0x0104,0x0000,(uint8_t *)&b_reserve,&st_net_data_att_r_1_1b_1_status,},
    {0x0105,0x0000,(uint8_t *)&st_net_status_data_cklcd_u2.w_1005,&st_net_data_att_r_2_2b_w_status,},
    {0x0106,0x0000,(uint8_t *)&st_net_status_data_cklcd_u2.w_1006,&st_net_data_att_r_2_2b_0_1_a_status,},
};

static net_set_data_cklcd st_net_set_data_cklcd_u2 = {0,};
const net_data_info st_net_set_data_info_cklcd_u2[cw_net_set_data_number_cklcd] =
{
    {0x0000,0x0000,(uint8_t *)&st_net_set_data_cklcd_u2.w_0000,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0001,0x0001,(uint8_t *)&st_net_set_data_cklcd_u2.w_0001,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0002,0x0002,(uint8_t *)&st_net_set_data_cklcd_u2.w_0002,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0003,0x0003,(uint8_t *)&st_net_set_data_cklcd_u2.w_0003,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0004,0x0004,(uint8_t *)&st_net_set_data_cklcd_u2.w_0004,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x0005,0x0005,(uint8_t *)&st_net_set_data_cklcd_u2.w_0005,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x0006,0x0006,(uint8_t *)&st_net_set_data_cklcd_u2.w_0006,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0007,0x0007,(uint8_t *)&st_net_set_data_cklcd_u2.w_0007,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0008,0x0008,(uint8_t *)&st_net_set_data_cklcd_u2.w_0008,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0009,0x0009,(uint8_t *)&st_net_set_data_cklcd_u2.w_0009,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x000A,0x000A,(uint8_t *)&st_net_set_data_cklcd_u2.w_000A,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000B,0x000B,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000C,0x000C,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000D,0x000D,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000E,0x000E,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000F,0x000F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//5

    {0x0010,0x0010,(uint8_t *)&st_net_set_data_cklcd_u2.w_0010,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0011,0x0011,(uint8_t *)&st_net_set_data_cklcd_u2.w_0011,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0012,0x0012,(uint8_t *)&st_net_set_data_cklcd_u2.w_0012,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0013,0x0013,(uint8_t *)&st_net_set_data_cklcd_u2.w_0013,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0014,0x0014,(uint8_t *)&st_net_set_data_cklcd_u2.w_0014,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0015,0x0015,(uint8_t *)&st_net_set_data_cklcd_u2.dw_0015,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0016,0x0016,(uint8_t *)&st_net_set_data_cklcd_u2.dw_0016,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0017,0x0017,(uint8_t *)&st_net_set_data_cklcd_u2.w_0017,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0018,0x0018,(uint8_t *)&st_net_set_data_cklcd_u2.w_0018,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0019,0x0019,(uint8_t *)&st_net_set_data_cklcd_u2.w_0019,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001A,0x001A,(uint8_t *)&st_net_set_data_cklcd_u2.w_001A,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001B,0x001B,(uint8_t *)&st_net_set_data_cklcd_u2.w_001B,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001C,0x001C,(uint8_t *)&st_net_set_data_cklcd_u2.w_001C,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001D,0x001D,(uint8_t *)&st_net_set_data_cklcd_u2.w_001D,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001E,0x001E,(uint8_t *)&st_net_set_data_cklcd_u2.w_001E,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001F,0x001F,(uint8_t *)&st_net_set_data_cklcd_u2.w_001F,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},//16

    {0x0020,0x0020,(uint8_t *)&st_net_set_data_cklcd_u2.w_0020,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0021,0x0021,(uint8_t *)&st_net_set_data_cklcd_u2.w_0021,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0022,0x0022,(uint8_t *)&st_net_set_data_cklcd_u2.w_0022,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0023,0x0023,(uint8_t *)&st_net_set_data_cklcd_u2.w_0023,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0024,0x0024,(uint8_t *)&st_net_set_data_cklcd_u2.w_0024,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0025,0x0025,(uint8_t *)&st_net_set_data_cklcd_u2.w_0025,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0026,0x0026,(uint8_t *)&st_net_set_data_cklcd_u2.w_0026,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0027,0x0027,(uint8_t *)&st_net_set_data_cklcd_u2.w_0027,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0028,0x0028,(uint8_t *)&st_net_set_data_cklcd_u2.w_0028,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0029,0x0029,(uint8_t *)&st_net_set_data_cklcd_u2.w_0029,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002A,0x002A,(uint8_t *)&st_net_set_data_cklcd_u2.dw_002A,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002B,0x002B,(uint8_t *)&st_net_set_data_cklcd_u2.dw_002B,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002C,0x002C,(uint8_t *)&st_net_set_data_cklcd_u2.w_002C,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002D,0x002D,(uint8_t *)&st_net_set_data_cklcd_u2.w_002D,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002E,0x002E,(uint8_t *)&st_net_set_data_cklcd_u2.w_002E,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002F,0x002F,(uint8_t *)&st_net_set_data_cklcd_u2.w_002F,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},//16

    {0x0030,0x0030,(uint8_t *)&st_net_set_data_cklcd_u2.w_0030,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0031,0x0031,(uint8_t *)&st_net_set_data_cklcd_u2.w_0031,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0032,0x0032,(uint8_t *)&st_net_set_data_cklcd_u2.w_0032,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0033,0x0033,(uint8_t *)&st_net_set_data_cklcd_u2.w_0033,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0034,0x0034,(uint8_t *)&st_net_set_data_cklcd_u2.w_0034,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0035,0x0035,(uint8_t *)&st_net_set_data_cklcd_u2.w_0035,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0036,0x0036,(uint8_t *)&st_net_set_data_cklcd_u2.w_0036,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0037,0x0037,(uint8_t *)&st_net_set_data_cklcd_u2.w_0037,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0038,0x0038,(uint8_t *)&st_net_set_data_cklcd_u2.w_0038,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0039,0x0039,(uint8_t *)&st_net_set_data_cklcd_u2.w_0039,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003A,0x003A,(uint8_t *)&st_net_set_data_cklcd_u2.w_003A,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003B,0x003B,(uint8_t *)&st_net_set_data_cklcd_u2.w_003B,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003C,0x003C,(uint8_t *)&st_net_set_data_cklcd_u2.w_003C,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003D,0x003D,(uint8_t *)&st_net_set_data_cklcd_u2.w_003D,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003E,0x003E,(uint8_t *)&st_net_set_data_cklcd_u2.w_003E,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003F,0x003F,(uint8_t *)&st_net_set_data_cklcd_u2.w_003F,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//14

    {0x0040,0x0040,(uint8_t *)&st_net_set_data_cklcd_u2.w_0040,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0041,0x0041,(uint8_t *)&st_net_set_data_cklcd_u2.dw_0041,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0042,0x0042,(uint8_t *)&st_net_set_data_cklcd_u2.dw_0042,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0043,0x0043,(uint8_t *)&st_net_set_data_cklcd_u2.w_0043,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0044,0x0044,(uint8_t *)&st_net_set_data_cklcd_u2.w_0044,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0045,0x0045,(uint8_t *)&st_net_set_data_cklcd_u2.w_0045,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0046,0x0046,(uint8_t *)&st_net_set_data_cklcd_u2.w_0046,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0047,0x0047,(uint8_t *)&st_net_set_data_cklcd_u2.w_0047,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0048,0x0048,(uint8_t *)&st_net_set_data_cklcd_u2.w_0048,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0049,0x0049,(uint8_t *)&st_net_set_data_cklcd_u2.w_0049,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004A,0x004A,(uint8_t *)&st_net_set_data_cklcd_u2.w_004A,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004B,0x004B,(uint8_t *)&st_net_set_data_cklcd_u2.w_004B,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004C,0x004C,(uint8_t *)&st_net_set_data_cklcd_u2.w_004C,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004D,0x004D,(uint8_t *)&st_net_set_data_cklcd_u2.w_004D,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004E,0x004E,(uint8_t *)&st_net_set_data_cklcd_u2.w_004E,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004F,0x004F,(uint8_t *)&st_net_set_data_cklcd_u2.w_004F,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//14

    {0x0050,0x0050,(uint8_t *)&st_net_set_data_cklcd_u2.w_0050,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0051,0x0051,(uint8_t *)&st_net_set_data_cklcd_u2.w_0051,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0052,0x0052,(uint8_t *)&st_net_set_data_cklcd_u2.w_0052,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0053,0x0053,(uint8_t *)&st_net_set_data_cklcd_u2.w_0053,&st_net_data_att_rw_2_2b_min_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0054,0x0054,(uint8_t *)&st_net_set_data_cklcd_u2.w_0054,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0055,0x0055,(uint8_t *)&st_net_set_data_cklcd_u2.w_0055,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0056,0x0056,(uint8_t *)&st_net_set_data_cklcd_u2.w_0056,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0057,0x0057,(uint8_t *)&st_net_set_data_cklcd_u2.w_0057,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0058,0x0058,(uint8_t *)&st_net_set_data_cklcd_u2.w_0058,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0059,0x0059,(uint8_t *)&st_net_set_data_cklcd_u2.w_0059,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x005A,0x005A,(uint8_t *)&st_net_set_data_cklcd_u2.w_005A,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x005B,0x005B,(uint8_t *)&st_net_set_data_cklcd_u2.w_005B,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x005C,0x005C,(uint8_t *)&st_net_set_data_cklcd_u2.w_005C,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x005D,0x005D,(uint8_t *)&st_net_set_data_cklcd_u2.w_005D,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x005E,0x005E,(uint8_t *)&st_net_set_data_cklcd_u2.w_005E,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},

//    {0x005F,0x005F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//12

    {0x0060,0x0060,(uint8_t *)&st_net_set_data_cklcd_u2.w_0060,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0061,0x0061,(uint8_t *)&st_net_set_data_cklcd_u2.w_0061,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//2
    {0x0062,0x0062,(uint8_t *)&st_net_set_data_cklcd_u2.w_0062,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0063,0x0063,(uint8_t *)&st_net_set_data_cklcd_u2.w_0063,&st_net_data_att_rw_2_2b_w_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0064,0x0064,(uint8_t *)&st_net_set_data_cklcd_u2.w_0064,&st_net_data_att_rw_2_2b_w_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0065,0x0065,(uint8_t *)&st_net_set_data_cklcd_u2.w_0065,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0066,0x0066,(uint8_t *)&st_net_set_data_cklcd_u2.w_0066,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0067,0x0067,(uint8_t *)&st_net_set_data_cklcd_u2.dw_0067,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0068,0x0068,(uint8_t *)&st_net_set_data_cklcd_u2.dw_0068,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0069,0x0069,(uint8_t *)&st_net_set_data_cklcd_u2.w_0069,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006A,0x006A,(uint8_t *)&st_net_set_data_cklcd_u2.w_006A,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006B,0x006B,(uint8_t *)&st_net_set_data_cklcd_u2.w_006B,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006C,0x006C,(uint8_t *)&st_net_set_data_cklcd_u2.w_006C,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006D,0x006D,(uint8_t *)&st_net_set_data_cklcd_u2.w_006D,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006E,0x006E,(uint8_t *)&st_net_set_data_cklcd_u2.w_006E,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006F,0x006F,(uint8_t *)&st_net_set_data_cklcd_u2.w_006F,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},

    {0x0070,0x0070,(uint8_t *)&st_net_set_data_cklcd_u2.dw_0070,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0071,0x0071,(uint8_t *)&st_net_set_data_cklcd_u2.dw_0071,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0072,0x0072,(uint8_t *)&st_net_set_data_cklcd_u2.w_0072,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},//3
	{0x0073,0x0073,(uint8_t *)&st_net_set_data_cklcd_u2.w_0073,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x0074,0x0074,(uint8_t *)&st_net_set_data_cklcd_u2.w_0074,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},

//    {0x0075,0x0005,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0076,0x0006,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0077,0x0007,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0078,0x0008,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0079,0x0009,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007A,0x000A,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007B,0x000B,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007C,0x000C,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007D,0x000D,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007E,0x000E,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007F,0x000F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},

    {0x0080,0x0080,(uint8_t *)&st_net_set_data_cklcd_u2.ba_0080,&st_net_data_att_r_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},//1
//    {0x0090,0x0090,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},
//    {0x00A0,0x00A0,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},
//    {0x00B0,0x00B0,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},
//    {0x00C0,0x00C0,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},

    {0x00D0,0x00D0,(uint8_t *)&st_net_set_data_cklcd_u2.w_00D0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D1,0x00D1,(uint8_t *)&st_net_set_data_cklcd_u2.w_00D1,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D2,0x00D2,(uint8_t *)&st_net_set_data_cklcd_u2.w_00D2,&st_net_data_att_rw_2_2b_h_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x00D3,0x00D3,(uint8_t *)&st_net_set_data_cklcd_u2.w_00D3,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x00D4,0x00D4,(uint8_t *)&st_net_set_data_cklcd_u2.w_00D4,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D5,0x00D5,(uint8_t *)&st_net_set_data_cklcd_u2.dw_00D5,&st_net_data_att_rw_4_4b_1_set,&st_device_data_att_rw_4_cklcd_para,},
//    {0x00D6,0x00D6,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D7,0x00D7,(uint8_t *)&st_net_set_data_cklcd_u2.w_00D7,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D8,0x00D8,(uint8_t *)&st_net_set_data_cklcd_u2.w_00D8,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D9,0x00D9,(uint8_t *)&st_net_set_data_cklcd_u2.w_00D9,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DA,0x00DA,(uint8_t *)&st_net_set_data_cklcd_u2.w_00DA,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_r_2_cklcd_para,},
    {0x00DB,0x00DB,(uint8_t *)&st_net_set_data_cklcd_u2.w_00DB,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DC,0x00DC,(uint8_t *)&st_net_set_data_cklcd_u2.w_00DC,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DD,0x00DD,(uint8_t *)&st_net_set_data_cklcd_u2.w_00DD,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DE,0x00DE,(uint8_t *)&st_net_set_data_cklcd_u2.w_00DE,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DF,0x00DF,(uint8_t *)&st_net_set_data_cklcd_u2.w_00DF,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//15

    {0x00E0,0x00E0,(uint8_t *)&st_net_set_data_cklcd_u2.w_00E0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E1,0x00E1,(uint8_t *)&st_net_set_data_cklcd_u2.w_00E1,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E2,0x00E2,(uint8_t *)&st_net_set_data_cklcd_u2.w_00E2,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E3,0x00E3,(uint8_t *)&st_net_set_data_cklcd_u2.w_00E3,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E4,0x00E4,(uint8_t *)&st_net_set_data_cklcd_u2.w_00E4,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E5,0x00E5,(uint8_t *)&st_net_set_data_cklcd_u2.w_00E5,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E6,0x00E6,(uint8_t *)&st_net_set_data_cklcd_u2.w_00E6,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E7,0x00E7,(uint8_t *)&st_net_set_data_cklcd_u2.w_00E7,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E8,0x00E8,(uint8_t *)&st_net_set_data_cklcd_u2.w_00E8,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E9,0x00E9,(uint8_t *)&st_net_set_data_cklcd_u2.w_00E9,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EA,0x00EA,(uint8_t *)&st_net_set_data_cklcd_u2.w_00EA,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EB,0x00EB,(uint8_t *)&st_net_set_data_cklcd_u2.w_00EB,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EC,0x00EC,(uint8_t *)&st_net_set_data_cklcd_u2.w_00EC,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00ED,0x00ED,(uint8_t *)&st_net_set_data_cklcd_u2.w_00ED,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EE,0x00EE,(uint8_t *)&st_net_set_data_cklcd_u2.w_00EE,&st_net_data_att_rw_2_2b_w_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x00EF,0x000F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//15
};
//net_data_info st_net_control_data_info_cklcd_u2[cw_net_control_data_number_cklcd] =
//{
//    {0x03E8,0x03E8,0,&st_net_data_att_rw_2_2b_1_control,&st_device_data_att_w_2_external_trig},
//};
/************************** CKLCD 3 *****************************/
static net_status_data_cklcd st_net_status_data_cklcd_u3 =
{
    0,
};
net_data_info st_net_status_data_info_cklcd_u3[cw_net_status_data_number_cklcd] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_mppt_u3.w_0100,&st_net_data_att_r_2_2b_1_status,},
    {0x0101,0x0000,(uint8_t *)&st_net_status_data_mppt_u3.w_0101,&st_net_data_att_r_2_2b_w_status,},
    {0x0102,0x0000,(uint8_t *)&st_net_status_data_mppt_u3.w_0102,&st_net_data_att_r_2_2b_0_0_1_ua_status,},
    {0x0103,0x0000,(uint8_t *)&st_net_status_data_mppt_u3.w_0103,&st_net_data_att_r_2_2b_0_1_kwh_status,},
    {0x0104,0x0000,(uint8_t *)&b_reserve,&st_net_data_att_r_1_1b_1_status,},
    {0x0105,0x0000,(uint8_t *)&st_net_status_data_cklcd_u3.w_1005,&st_net_data_att_r_2_2b_w_status,},
    {0x0106,0x0000,(uint8_t *)&st_net_status_data_cklcd_u3.w_1006,&st_net_data_att_r_2_2b_0_1_a_status,},
};

static net_set_data_cklcd st_net_set_data_cklcd_u3 = {0,};
const net_data_info st_net_set_data_info_cklcd_u3[cw_net_set_data_number_cklcd] =
{
    {0x0000,0x0000,(uint8_t *)&st_net_set_data_cklcd_u3.w_0000,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0001,0x0001,(uint8_t *)&st_net_set_data_cklcd_u3.w_0001,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0002,0x0002,(uint8_t *)&st_net_set_data_cklcd_u3.w_0002,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0003,0x0003,(uint8_t *)&st_net_set_data_cklcd_u3.w_0003,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0004,0x0004,(uint8_t *)&st_net_set_data_cklcd_u3.w_0004,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x0005,0x0005,(uint8_t *)&st_net_set_data_cklcd_u3.w_0005,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x0006,0x0006,(uint8_t *)&st_net_set_data_cklcd_u3.w_0006,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0007,0x0007,(uint8_t *)&st_net_set_data_cklcd_u3.w_0007,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0008,0x0008,(uint8_t *)&st_net_set_data_cklcd_u3.w_0008,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0009,0x0009,(uint8_t *)&st_net_set_data_cklcd_u3.w_0009,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x000A,0x000A,(uint8_t *)&st_net_set_data_cklcd_u3.w_000A,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000B,0x000B,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000C,0x000C,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000D,0x000D,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000E,0x000E,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000F,0x000F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//5

    {0x0010,0x0010,(uint8_t *)&st_net_set_data_cklcd_u3.w_0010,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0011,0x0011,(uint8_t *)&st_net_set_data_cklcd_u3.w_0011,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0012,0x0012,(uint8_t *)&st_net_set_data_cklcd_u3.w_0012,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0013,0x0013,(uint8_t *)&st_net_set_data_cklcd_u3.w_0013,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0014,0x0014,(uint8_t *)&st_net_set_data_cklcd_u3.w_0014,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0015,0x0015,(uint8_t *)&st_net_set_data_cklcd_u3.dw_0015,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0016,0x0016,(uint8_t *)&st_net_set_data_cklcd_u3.dw_0016,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0017,0x0017,(uint8_t *)&st_net_set_data_cklcd_u3.w_0017,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0018,0x0018,(uint8_t *)&st_net_set_data_cklcd_u3.w_0018,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0019,0x0019,(uint8_t *)&st_net_set_data_cklcd_u3.w_0019,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001A,0x001A,(uint8_t *)&st_net_set_data_cklcd_u3.w_001A,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001B,0x001B,(uint8_t *)&st_net_set_data_cklcd_u3.w_001B,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001C,0x001C,(uint8_t *)&st_net_set_data_cklcd_u3.w_001C,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001D,0x001D,(uint8_t *)&st_net_set_data_cklcd_u3.w_001D,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001E,0x001E,(uint8_t *)&st_net_set_data_cklcd_u3.w_001E,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001F,0x001F,(uint8_t *)&st_net_set_data_cklcd_u3.w_001F,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},//16

    {0x0020,0x0020,(uint8_t *)&st_net_set_data_cklcd_u3.w_0020,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0021,0x0021,(uint8_t *)&st_net_set_data_cklcd_u3.w_0021,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0022,0x0022,(uint8_t *)&st_net_set_data_cklcd_u3.w_0022,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0023,0x0023,(uint8_t *)&st_net_set_data_cklcd_u3.w_0023,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0024,0x0024,(uint8_t *)&st_net_set_data_cklcd_u3.w_0024,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0025,0x0025,(uint8_t *)&st_net_set_data_cklcd_u3.w_0025,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0026,0x0026,(uint8_t *)&st_net_set_data_cklcd_u3.w_0026,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0027,0x0027,(uint8_t *)&st_net_set_data_cklcd_u3.w_0027,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0028,0x0028,(uint8_t *)&st_net_set_data_cklcd_u3.w_0028,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0029,0x0029,(uint8_t *)&st_net_set_data_cklcd_u3.w_0029,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002A,0x002A,(uint8_t *)&st_net_set_data_cklcd_u3.dw_002A,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002B,0x002B,(uint8_t *)&st_net_set_data_cklcd_u3.dw_002B,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002C,0x002C,(uint8_t *)&st_net_set_data_cklcd_u3.w_002C,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002D,0x002D,(uint8_t *)&st_net_set_data_cklcd_u3.w_002D,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002E,0x002E,(uint8_t *)&st_net_set_data_cklcd_u3.w_002E,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002F,0x002F,(uint8_t *)&st_net_set_data_cklcd_u3.w_002F,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},//16

    {0x0030,0x0030,(uint8_t *)&st_net_set_data_cklcd_u3.w_0030,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0031,0x0031,(uint8_t *)&st_net_set_data_cklcd_u3.w_0031,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0032,0x0032,(uint8_t *)&st_net_set_data_cklcd_u3.w_0032,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0033,0x0033,(uint8_t *)&st_net_set_data_cklcd_u3.w_0033,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0034,0x0034,(uint8_t *)&st_net_set_data_cklcd_u3.w_0034,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0035,0x0035,(uint8_t *)&st_net_set_data_cklcd_u3.w_0035,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0036,0x0036,(uint8_t *)&st_net_set_data_cklcd_u3.w_0036,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0037,0x0037,(uint8_t *)&st_net_set_data_cklcd_u3.w_0037,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0038,0x0038,(uint8_t *)&st_net_set_data_cklcd_u3.w_0038,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0039,0x0039,(uint8_t *)&st_net_set_data_cklcd_u3.w_0039,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003A,0x003A,(uint8_t *)&st_net_set_data_cklcd_u3.w_003A,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003B,0x003B,(uint8_t *)&st_net_set_data_cklcd_u3.w_003B,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003C,0x003C,(uint8_t *)&st_net_set_data_cklcd_u3.w_003C,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003D,0x003D,(uint8_t *)&st_net_set_data_cklcd_u3.w_003D,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003E,0x003E,(uint8_t *)&st_net_set_data_cklcd_u3.w_003E,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003F,0x003F,(uint8_t *)&st_net_set_data_cklcd_u3.w_003F,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//14

    {0x0040,0x0040,(uint8_t *)&st_net_set_data_cklcd_u3.w_0040,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0041,0x0041,(uint8_t *)&st_net_set_data_cklcd_u3.dw_0041,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0042,0x0042,(uint8_t *)&st_net_set_data_cklcd_u3.dw_0042,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0043,0x0043,(uint8_t *)&st_net_set_data_cklcd_u3.w_0043,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0044,0x0044,(uint8_t *)&st_net_set_data_cklcd_u3.w_0044,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0045,0x0045,(uint8_t *)&st_net_set_data_cklcd_u3.w_0045,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0046,0x0046,(uint8_t *)&st_net_set_data_cklcd_u3.w_0046,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0047,0x0047,(uint8_t *)&st_net_set_data_cklcd_u3.w_0047,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0048,0x0048,(uint8_t *)&st_net_set_data_cklcd_u3.w_0048,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0049,0x0049,(uint8_t *)&st_net_set_data_cklcd_u3.w_0049,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004A,0x004A,(uint8_t *)&st_net_set_data_cklcd_u3.w_004A,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004B,0x004B,(uint8_t *)&st_net_set_data_cklcd_u3.w_004B,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004C,0x004C,(uint8_t *)&st_net_set_data_cklcd_u3.w_004C,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004D,0x004D,(uint8_t *)&st_net_set_data_cklcd_u3.w_004D,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004E,0x004E,(uint8_t *)&st_net_set_data_cklcd_u3.w_004E,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004F,0x004F,(uint8_t *)&st_net_set_data_cklcd_u3.w_004F,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//14

    {0x0050,0x0050,(uint8_t *)&st_net_set_data_cklcd_u3.w_0050,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0051,0x0051,(uint8_t *)&st_net_set_data_cklcd_u3.w_0051,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0052,0x0052,(uint8_t *)&st_net_set_data_cklcd_u3.w_0052,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0053,0x0053,(uint8_t *)&st_net_set_data_cklcd_u3.w_0053,&st_net_data_att_rw_2_2b_min_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0054,0x0054,(uint8_t *)&st_net_set_data_cklcd_u3.w_0054,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0055,0x0055,(uint8_t *)&st_net_set_data_cklcd_u3.w_0055,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0056,0x0056,(uint8_t *)&st_net_set_data_cklcd_u3.w_0056,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0057,0x0057,(uint8_t *)&st_net_set_data_cklcd_u3.w_0057,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0058,0x0058,(uint8_t *)&st_net_set_data_cklcd_u3.w_0058,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0059,0x0059,(uint8_t *)&st_net_set_data_cklcd_u3.w_0059,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x005A,0x005A,(uint8_t *)&st_net_set_data_cklcd_u3.w_005A,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x005B,0x005B,(uint8_t *)&st_net_set_data_cklcd_u3.w_005B,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x005C,0x005C,(uint8_t *)&st_net_set_data_cklcd_u3.w_005C,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x005D,0x005D,(uint8_t *)&st_net_set_data_cklcd_u3.w_005D,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x005E,0x005E,(uint8_t *)&st_net_set_data_cklcd_u3.w_005E,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x005F,0x005F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//12

    {0x0060,0x0060,(uint8_t *)&st_net_set_data_cklcd_u3.w_0060,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0061,0x0061,(uint8_t *)&st_net_set_data_cklcd_u3.w_0061,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//2
    {0x0062,0x0062,(uint8_t *)&st_net_set_data_cklcd_u3.w_0062,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0063,0x0063,(uint8_t *)&st_net_set_data_cklcd_u3.w_0063,&st_net_data_att_rw_2_2b_w_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0064,0x0064,(uint8_t *)&st_net_set_data_cklcd_u3.w_0064,&st_net_data_att_rw_2_2b_w_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0065,0x0065,(uint8_t *)&st_net_set_data_cklcd_u3.w_0065,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0066,0x0066,(uint8_t *)&st_net_set_data_cklcd_u3.w_0066,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0067,0x0067,(uint8_t *)&st_net_set_data_cklcd_u3.dw_0067,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0068,0x0068,(uint8_t *)&st_net_set_data_cklcd_u3.dw_0068,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0069,0x0069,(uint8_t *)&st_net_set_data_cklcd_u3.w_0069,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006A,0x006A,(uint8_t *)&st_net_set_data_cklcd_u3.w_006A,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006B,0x006B,(uint8_t *)&st_net_set_data_cklcd_u3.w_006B,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006C,0x006C,(uint8_t *)&st_net_set_data_cklcd_u3.w_006C,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006D,0x006D,(uint8_t *)&st_net_set_data_cklcd_u3.w_006D,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006E,0x006E,(uint8_t *)&st_net_set_data_cklcd_u3.w_006E,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006F,0x006F,(uint8_t *)&st_net_set_data_cklcd_u3.w_006F,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},

    {0x0070,0x0070,(uint8_t *)&st_net_set_data_cklcd_u3.dw_0070,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0071,0x0071,(uint8_t *)&st_net_set_data_cklcd_u3.dw_0071,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0072,0x0072,(uint8_t *)&st_net_set_data_cklcd_u3.w_0072,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},//3
	{0x0073,0x0073,(uint8_t *)&st_net_set_data_cklcd_u3.w_0073,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x0074,0x0074,(uint8_t *)&st_net_set_data_cklcd_u3.w_0074,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},

//    {0x0075,0x0005,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0076,0x0006,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0077,0x0007,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0078,0x0008,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0079,0x0009,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007A,0x000A,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007B,0x000B,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007C,0x000C,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007D,0x000D,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007E,0x000E,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007F,0x000F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},

    {0x0080,0x0080,(uint8_t *)&st_net_set_data_cklcd_u3.ba_0080,&st_net_data_att_r_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},//1
//    {0x0090,0x0090,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},
//    {0x00A0,0x00A0,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},
//    {0x00B0,0x00B0,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},
//    {0x00C0,0x00C0,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},

    {0x00D0,0x00D0,(uint8_t *)&st_net_set_data_cklcd_u3.w_00D0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D1,0x00D1,(uint8_t *)&st_net_set_data_cklcd_u3.w_00D1,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D2,0x00D2,(uint8_t *)&st_net_set_data_cklcd_u3.w_00D2,&st_net_data_att_rw_2_2b_h_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x00D3,0x00D3,(uint8_t *)&st_net_set_data_cklcd_u3.w_00D3,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x00D4,0x00D4,(uint8_t *)&st_net_set_data_cklcd_u3.w_00D4,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D5,0x00D5,(uint8_t *)&st_net_set_data_cklcd_u3.dw_00D5,&st_net_data_att_rw_4_4b_1_set,&st_device_data_att_rw_4_cklcd_para,},
//    {0x00D6,0x00D6,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D7,0x00D7,(uint8_t *)&st_net_set_data_cklcd_u3.w_00D7,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D8,0x00D8,(uint8_t *)&st_net_set_data_cklcd_u3.w_00D8,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D9,0x00D9,(uint8_t *)&st_net_set_data_cklcd_u3.w_00D9,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DA,0x00DA,(uint8_t *)&st_net_set_data_cklcd_u3.w_00DA,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_r_2_cklcd_para,},
    {0x00DB,0x00DB,(uint8_t *)&st_net_set_data_cklcd_u3.w_00DB,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DC,0x00DC,(uint8_t *)&st_net_set_data_cklcd_u3.w_00DC,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DD,0x00DD,(uint8_t *)&st_net_set_data_cklcd_u3.w_00DD,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DE,0x00DE,(uint8_t *)&st_net_set_data_cklcd_u3.w_00DE,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DF,0x00DF,(uint8_t *)&st_net_set_data_cklcd_u3.w_00DF,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//15

    {0x00E0,0x00E0,(uint8_t *)&st_net_set_data_cklcd_u3.w_00E0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E1,0x00E1,(uint8_t *)&st_net_set_data_cklcd_u3.w_00E1,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E2,0x00E2,(uint8_t *)&st_net_set_data_cklcd_u3.w_00E2,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E3,0x00E3,(uint8_t *)&st_net_set_data_cklcd_u3.w_00E3,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E4,0x00E4,(uint8_t *)&st_net_set_data_cklcd_u3.w_00E4,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E5,0x00E5,(uint8_t *)&st_net_set_data_cklcd_u3.w_00E5,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E6,0x00E6,(uint8_t *)&st_net_set_data_cklcd_u3.w_00E6,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E7,0x00E7,(uint8_t *)&st_net_set_data_cklcd_u3.w_00E7,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E8,0x00E8,(uint8_t *)&st_net_set_data_cklcd_u3.w_00E8,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E9,0x00E9,(uint8_t *)&st_net_set_data_cklcd_u3.w_00E9,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EA,0x00EA,(uint8_t *)&st_net_set_data_cklcd_u3.w_00EA,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EB,0x00EB,(uint8_t *)&st_net_set_data_cklcd_u3.w_00EB,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EC,0x00EC,(uint8_t *)&st_net_set_data_cklcd_u3.w_00EC,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00ED,0x00ED,(uint8_t *)&st_net_set_data_cklcd_u3.w_00ED,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EE,0x00EE,(uint8_t *)&st_net_set_data_cklcd_u3.w_00EE,&st_net_data_att_rw_2_2b_w_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x00EF,0x000F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//15
};
//net_data_info st_net_control_data_info_cklcd_u3[cw_net_control_data_number_cklcd] =
//{
//    {0x03E8,0x03E8,0,&st_net_data_att_rw_2_2b_1_control,&st_device_data_att_w_2_external_trig},
//};
/************************** CKLCD 4 *****************************/
static net_status_data_cklcd st_net_status_data_cklcd_v1 =
{
    0,
};
net_data_info st_net_status_data_info_cklcd_v1[cw_net_status_data_number_cklcd] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_mppt_v1.w_0100,&st_net_data_att_r_2_2b_1_status,},
    {0x0101,0x0000,(uint8_t *)&st_net_status_data_mppt_v1.w_0101,&st_net_data_att_r_2_2b_w_status,},
    {0x0102,0x0000,(uint8_t *)&st_net_status_data_mppt_v1.w_0102,&st_net_data_att_r_2_2b_0_0_1_ua_status,},
    {0x0103,0x0000,(uint8_t *)&st_net_status_data_mppt_v1.w_0103,&st_net_data_att_r_2_2b_0_1_kwh_status,},
    {0x0104,0x0000,(uint8_t *)&b_reserve,&st_net_data_att_r_1_1b_1_status,},
    {0x0105,0x0000,(uint8_t *)&st_net_status_data_cklcd_v1.w_1005,&st_net_data_att_r_2_2b_w_status,},
    {0x0106,0x0000,(uint8_t *)&st_net_status_data_cklcd_v1.w_1006,&st_net_data_att_r_2_2b_0_1_a_status,},
};
static net_set_data_cklcd st_net_set_data_cklcd_v1 = {0,};
const net_data_info st_net_set_data_info_cklcd_v1[cw_net_set_data_number_cklcd] =
{
    {0x0000,0x0000,(uint8_t *)&st_net_set_data_cklcd_v1.w_0000,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0001,0x0001,(uint8_t *)&st_net_set_data_cklcd_v1.w_0001,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0002,0x0002,(uint8_t *)&st_net_set_data_cklcd_v1.w_0002,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0003,0x0003,(uint8_t *)&st_net_set_data_cklcd_v1.w_0003,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0004,0x0004,(uint8_t *)&st_net_set_data_cklcd_v1.w_0004,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x0005,0x0005,(uint8_t *)&st_net_set_data_cklcd_v1.w_0005,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x0006,0x0006,(uint8_t *)&st_net_set_data_cklcd_v1.w_0006,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0007,0x0007,(uint8_t *)&st_net_set_data_cklcd_v1.w_0007,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0008,0x0008,(uint8_t *)&st_net_set_data_cklcd_v1.w_0008,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0009,0x0009,(uint8_t *)&st_net_set_data_cklcd_v1.w_0009,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x000A,0x000A,(uint8_t *)&st_net_set_data_cklcd_v1.w_000A,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000B,0x000B,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000C,0x000C,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000D,0x000D,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000E,0x000E,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000F,0x000F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//5

    {0x0010,0x0010,(uint8_t *)&st_net_set_data_cklcd_v1.w_0010,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0011,0x0011,(uint8_t *)&st_net_set_data_cklcd_v1.w_0011,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0012,0x0012,(uint8_t *)&st_net_set_data_cklcd_v1.w_0012,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0013,0x0013,(uint8_t *)&st_net_set_data_cklcd_v1.w_0013,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0014,0x0014,(uint8_t *)&st_net_set_data_cklcd_v1.w_0014,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0015,0x0015,(uint8_t *)&st_net_set_data_cklcd_v1.dw_0015,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0016,0x0016,(uint8_t *)&st_net_set_data_cklcd_v1.dw_0016,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0017,0x0017,(uint8_t *)&st_net_set_data_cklcd_v1.w_0017,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0018,0x0018,(uint8_t *)&st_net_set_data_cklcd_v1.w_0018,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0019,0x0019,(uint8_t *)&st_net_set_data_cklcd_v1.w_0019,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001A,0x001A,(uint8_t *)&st_net_set_data_cklcd_v1.w_001A,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001B,0x001B,(uint8_t *)&st_net_set_data_cklcd_v1.w_001B,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001C,0x001C,(uint8_t *)&st_net_set_data_cklcd_v1.w_001C,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001D,0x001D,(uint8_t *)&st_net_set_data_cklcd_v1.w_001D,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001E,0x001E,(uint8_t *)&st_net_set_data_cklcd_v1.w_001E,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001F,0x001F,(uint8_t *)&st_net_set_data_cklcd_v1.w_001F,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},//16

    {0x0020,0x0020,(uint8_t *)&st_net_set_data_cklcd_v1.w_0020,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0021,0x0021,(uint8_t *)&st_net_set_data_cklcd_v1.w_0021,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0022,0x0022,(uint8_t *)&st_net_set_data_cklcd_v1.w_0022,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0023,0x0023,(uint8_t *)&st_net_set_data_cklcd_v1.w_0023,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0024,0x0024,(uint8_t *)&st_net_set_data_cklcd_v1.w_0024,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0025,0x0025,(uint8_t *)&st_net_set_data_cklcd_v1.w_0025,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0026,0x0026,(uint8_t *)&st_net_set_data_cklcd_v1.w_0026,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0027,0x0027,(uint8_t *)&st_net_set_data_cklcd_v1.w_0027,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0028,0x0028,(uint8_t *)&st_net_set_data_cklcd_v1.w_0028,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0029,0x0029,(uint8_t *)&st_net_set_data_cklcd_v1.w_0029,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002A,0x002A,(uint8_t *)&st_net_set_data_cklcd_v1.dw_002A,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002B,0x002B,(uint8_t *)&st_net_set_data_cklcd_v1.dw_002B,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002C,0x002C,(uint8_t *)&st_net_set_data_cklcd_v1.w_002C,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002D,0x002D,(uint8_t *)&st_net_set_data_cklcd_v1.w_002D,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002E,0x002E,(uint8_t *)&st_net_set_data_cklcd_v1.w_002E,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002F,0x002F,(uint8_t *)&st_net_set_data_cklcd_v1.w_002F,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},//16

    {0x0030,0x0030,(uint8_t *)&st_net_set_data_cklcd_v1.w_0030,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0031,0x0031,(uint8_t *)&st_net_set_data_cklcd_v1.w_0031,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0032,0x0032,(uint8_t *)&st_net_set_data_cklcd_v1.w_0032,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0033,0x0033,(uint8_t *)&st_net_set_data_cklcd_v1.w_0033,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0034,0x0034,(uint8_t *)&st_net_set_data_cklcd_v1.w_0034,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0035,0x0035,(uint8_t *)&st_net_set_data_cklcd_v1.w_0035,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0036,0x0036,(uint8_t *)&st_net_set_data_cklcd_v1.w_0036,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0037,0x0037,(uint8_t *)&st_net_set_data_cklcd_v1.w_0037,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0038,0x0038,(uint8_t *)&st_net_set_data_cklcd_v1.w_0038,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0039,0x0039,(uint8_t *)&st_net_set_data_cklcd_v1.w_0039,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003A,0x003A,(uint8_t *)&st_net_set_data_cklcd_v1.w_003A,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003B,0x003B,(uint8_t *)&st_net_set_data_cklcd_v1.w_003B,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003C,0x003C,(uint8_t *)&st_net_set_data_cklcd_v1.w_003C,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003D,0x003D,(uint8_t *)&st_net_set_data_cklcd_v1.w_003D,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003E,0x003E,(uint8_t *)&st_net_set_data_cklcd_v1.w_003E,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003F,0x003F,(uint8_t *)&st_net_set_data_cklcd_v1.w_003F,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//14

    {0x0040,0x0040,(uint8_t *)&st_net_set_data_cklcd_v1.w_0040,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0041,0x0041,(uint8_t *)&st_net_set_data_cklcd_v1.dw_0041,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0042,0x0042,(uint8_t *)&st_net_set_data_cklcd_v1.dw_0042,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0043,0x0043,(uint8_t *)&st_net_set_data_cklcd_v1.w_0043,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0044,0x0044,(uint8_t *)&st_net_set_data_cklcd_v1.w_0044,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0045,0x0045,(uint8_t *)&st_net_set_data_cklcd_v1.w_0045,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0046,0x0046,(uint8_t *)&st_net_set_data_cklcd_v1.w_0046,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0047,0x0047,(uint8_t *)&st_net_set_data_cklcd_v1.w_0047,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0048,0x0048,(uint8_t *)&st_net_set_data_cklcd_v1.w_0048,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0049,0x0049,(uint8_t *)&st_net_set_data_cklcd_v1.w_0049,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004A,0x004A,(uint8_t *)&st_net_set_data_cklcd_v1.w_004A,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004B,0x004B,(uint8_t *)&st_net_set_data_cklcd_v1.w_004B,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004C,0x004C,(uint8_t *)&st_net_set_data_cklcd_v1.w_004C,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004D,0x004D,(uint8_t *)&st_net_set_data_cklcd_v1.w_004D,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004E,0x004E,(uint8_t *)&st_net_set_data_cklcd_v1.w_004E,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004F,0x004F,(uint8_t *)&st_net_set_data_cklcd_v1.w_004F,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//14

    {0x0050,0x0050,(uint8_t *)&st_net_set_data_cklcd_v1.w_0050,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0051,0x0051,(uint8_t *)&st_net_set_data_cklcd_v1.w_0051,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0052,0x0052,(uint8_t *)&st_net_set_data_cklcd_v1.w_0052,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0053,0x0053,(uint8_t *)&st_net_set_data_cklcd_v1.w_0053,&st_net_data_att_rw_2_2b_min_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0054,0x0054,(uint8_t *)&st_net_set_data_cklcd_v1.w_0054,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0055,0x0055,(uint8_t *)&st_net_set_data_cklcd_v1.w_0055,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0056,0x0056,(uint8_t *)&st_net_set_data_cklcd_v1.w_0056,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0057,0x0057,(uint8_t *)&st_net_set_data_cklcd_v1.w_0057,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0058,0x0058,(uint8_t *)&st_net_set_data_cklcd_v1.w_0058,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0059,0x0059,(uint8_t *)&st_net_set_data_cklcd_v1.w_0059,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x005A,0x005A,(uint8_t *)&st_net_set_data_cklcd_v1.w_005A,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x005B,0x005B,(uint8_t *)&st_net_set_data_cklcd_v1.w_005B,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x005C,0x005C,(uint8_t *)&st_net_set_data_cklcd_v1.w_005C,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x005D,0x005D,(uint8_t *)&st_net_set_data_cklcd_v1.w_005D,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x005E,0x005E,(uint8_t *)&st_net_set_data_cklcd_v1.w_005E,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x005F,0x005F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//12

    {0x0060,0x0060,(uint8_t *)&st_net_set_data_cklcd_v1.w_0060,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0061,0x0061,(uint8_t *)&st_net_set_data_cklcd_v1.w_0061,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//2
    {0x0062,0x0062,(uint8_t *)&st_net_set_data_cklcd_v1.w_0062,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0063,0x0063,(uint8_t *)&st_net_set_data_cklcd_v1.w_0063,&st_net_data_att_rw_2_2b_w_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0064,0x0064,(uint8_t *)&st_net_set_data_cklcd_v1.w_0064,&st_net_data_att_rw_2_2b_w_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0065,0x0065,(uint8_t *)&st_net_set_data_cklcd_v1.w_0065,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0066,0x0066,(uint8_t *)&st_net_set_data_cklcd_v1.w_0066,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0067,0x0067,(uint8_t *)&st_net_set_data_cklcd_v1.dw_0067,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0068,0x0068,(uint8_t *)&st_net_set_data_cklcd_v1.dw_0068,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0069,0x0069,(uint8_t *)&st_net_set_data_cklcd_v1.w_0069,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006A,0x006A,(uint8_t *)&st_net_set_data_cklcd_v1.w_006A,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006B,0x006B,(uint8_t *)&st_net_set_data_cklcd_v1.w_006B,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006C,0x006C,(uint8_t *)&st_net_set_data_cklcd_v1.w_006C,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006D,0x006D,(uint8_t *)&st_net_set_data_cklcd_v1.w_006D,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006E,0x006E,(uint8_t *)&st_net_set_data_cklcd_v1.w_006E,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006F,0x006F,(uint8_t *)&st_net_set_data_cklcd_v1.w_006F,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},

    {0x0070,0x0070,(uint8_t *)&st_net_set_data_cklcd_v1.dw_0070,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0071,0x0071,(uint8_t *)&st_net_set_data_cklcd_v1.dw_0071,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0072,0x0072,(uint8_t *)&st_net_set_data_cklcd_v1.w_0072,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},//3
	{0x0073,0x0073,(uint8_t *)&st_net_set_data_cklcd_v1.w_0073,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x0074,0x0074,(uint8_t *)&st_net_set_data_cklcd_v1.w_0074,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},

//    {0x0075,0x0005,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0076,0x0006,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0077,0x0007,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0078,0x0008,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0079,0x0009,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007A,0x000A,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007B,0x000B,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007C,0x000C,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007D,0x000D,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007E,0x000E,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007F,0x000F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},

    {0x0080,0x0080,(uint8_t *)&st_net_set_data_cklcd_v1.ba_0080,&st_net_data_att_r_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},//1
//    {0x0090,0x0090,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},
//    {0x00A0,0x00A0,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},
//    {0x00B0,0x00B0,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},
//    {0x00C0,0x00C0,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},

    {0x00D0,0x00D0,(uint8_t *)&st_net_set_data_cklcd_v1.w_00D0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D1,0x00D1,(uint8_t *)&st_net_set_data_cklcd_v1.w_00D1,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D2,0x00D2,(uint8_t *)&st_net_set_data_cklcd_v1.w_00D2,&st_net_data_att_rw_2_2b_h_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x00D3,0x00D3,(uint8_t *)&st_net_set_data_cklcd_v1.w_00D3,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x00D4,0x00D4,(uint8_t *)&st_net_set_data_cklcd_v1.w_00D4,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D5,0x00D5,(uint8_t *)&st_net_set_data_cklcd_v1.dw_00D5,&st_net_data_att_rw_4_4b_1_set,&st_device_data_att_rw_4_cklcd_para,},
//    {0x00D6,0x00D6,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D7,0x00D7,(uint8_t *)&st_net_set_data_cklcd_v1.w_00D7,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D8,0x00D8,(uint8_t *)&st_net_set_data_cklcd_v1.w_00D8,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D9,0x00D9,(uint8_t *)&st_net_set_data_cklcd_v1.w_00D9,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DA,0x00DA,(uint8_t *)&st_net_set_data_cklcd_v1.w_00DA,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_r_2_cklcd_para,},
    {0x00DB,0x00DB,(uint8_t *)&st_net_set_data_cklcd_v1.w_00DB,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DC,0x00DC,(uint8_t *)&st_net_set_data_cklcd_v1.w_00DC,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DD,0x00DD,(uint8_t *)&st_net_set_data_cklcd_v1.w_00DD,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DE,0x00DE,(uint8_t *)&st_net_set_data_cklcd_v1.w_00DE,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DF,0x00DF,(uint8_t *)&st_net_set_data_cklcd_v1.w_00DF,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//15

    {0x00E0,0x00E0,(uint8_t *)&st_net_set_data_cklcd_v1.w_00E0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E1,0x00E1,(uint8_t *)&st_net_set_data_cklcd_v1.w_00E1,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E2,0x00E2,(uint8_t *)&st_net_set_data_cklcd_v1.w_00E2,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E3,0x00E3,(uint8_t *)&st_net_set_data_cklcd_v1.w_00E3,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E4,0x00E4,(uint8_t *)&st_net_set_data_cklcd_v1.w_00E4,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E5,0x00E5,(uint8_t *)&st_net_set_data_cklcd_v1.w_00E5,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E6,0x00E6,(uint8_t *)&st_net_set_data_cklcd_v1.w_00E6,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E7,0x00E7,(uint8_t *)&st_net_set_data_cklcd_v1.w_00E7,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E8,0x00E8,(uint8_t *)&st_net_set_data_cklcd_v1.w_00E8,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E9,0x00E9,(uint8_t *)&st_net_set_data_cklcd_v1.w_00E9,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EA,0x00EA,(uint8_t *)&st_net_set_data_cklcd_v1.w_00EA,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EB,0x00EB,(uint8_t *)&st_net_set_data_cklcd_v1.w_00EB,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EC,0x00EC,(uint8_t *)&st_net_set_data_cklcd_v1.w_00EC,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00ED,0x00ED,(uint8_t *)&st_net_set_data_cklcd_v1.w_00ED,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EE,0x00EE,(uint8_t *)&st_net_set_data_cklcd_v1.w_00EE,&st_net_data_att_rw_2_2b_w_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x00EF,0x000F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//15
};
//net_data_info st_net_control_data_info_cklcd_v1[cw_net_control_data_number_cklcd] =
//{
//    {0x03E8,0x03E8,0,&st_net_data_att_rw_2_2b_1_control,&st_device_data_att_w_2_external_trig},
//};
/************************** CKLCD 5 *****************************/
static net_status_data_cklcd st_net_status_data_cklcd_v2 =
{
    0,
};
net_data_info st_net_status_data_info_cklcd_v2[cw_net_status_data_number_cklcd] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_mppt_v2.w_0100,&st_net_data_att_r_2_2b_1_status,},
    {0x0101,0x0000,(uint8_t *)&st_net_status_data_mppt_v2.w_0101,&st_net_data_att_r_2_2b_w_status,},
    {0x0102,0x0000,(uint8_t *)&st_net_status_data_mppt_v2.w_0102,&st_net_data_att_r_2_2b_0_0_1_ua_status,},
    {0x0103,0x0000,(uint8_t *)&st_net_status_data_mppt_v2.w_0103,&st_net_data_att_r_2_2b_0_1_kwh_status,},
    {0x0104,0x0000,(uint8_t *)&b_reserve,&st_net_data_att_r_1_1b_1_status,},
    {0x0105,0x0000,(uint8_t *)&st_net_status_data_cklcd_v2.w_1005,&st_net_data_att_r_2_2b_w_status,},
    {0x0106,0x0000,(uint8_t *)&st_net_status_data_cklcd_v2.w_1006,&st_net_data_att_r_2_2b_0_1_a_status,},
};
static net_set_data_cklcd st_net_set_data_cklcd_v2 = {0,};
const net_data_info st_net_set_data_info_cklcd_v2[cw_net_set_data_number_cklcd] =
{
    {0x0000,0x0000,(uint8_t *)&st_net_set_data_cklcd_v2.w_0000,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0001,0x0001,(uint8_t *)&st_net_set_data_cklcd_v2.w_0001,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0002,0x0002,(uint8_t *)&st_net_set_data_cklcd_v2.w_0002,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0003,0x0003,(uint8_t *)&st_net_set_data_cklcd_v2.w_0003,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0004,0x0004,(uint8_t *)&st_net_set_data_cklcd_v2.w_0004,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x0005,0x0005,(uint8_t *)&st_net_set_data_cklcd_v2.w_0005,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x0006,0x0006,(uint8_t *)&st_net_set_data_cklcd_v2.w_0006,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0007,0x0007,(uint8_t *)&st_net_set_data_cklcd_v2.w_0007,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0008,0x0008,(uint8_t *)&st_net_set_data_cklcd_v2.w_0008,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0009,0x0009,(uint8_t *)&st_net_set_data_cklcd_v2.w_0009,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x000A,0x000A,(uint8_t *)&st_net_set_data_cklcd_v2.w_000A,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000B,0x000B,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000C,0x000C,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000D,0x000D,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000E,0x000E,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000F,0x000F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//5

    {0x0010,0x0010,(uint8_t *)&st_net_set_data_cklcd_v2.w_0010,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0011,0x0011,(uint8_t *)&st_net_set_data_cklcd_v2.w_0011,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0012,0x0012,(uint8_t *)&st_net_set_data_cklcd_v2.w_0012,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0013,0x0013,(uint8_t *)&st_net_set_data_cklcd_v2.w_0013,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0014,0x0014,(uint8_t *)&st_net_set_data_cklcd_v2.w_0014,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0015,0x0015,(uint8_t *)&st_net_set_data_cklcd_v2.dw_0015,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0016,0x0016,(uint8_t *)&st_net_set_data_cklcd_v2.dw_0016,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0017,0x0017,(uint8_t *)&st_net_set_data_cklcd_v2.w_0017,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0018,0x0018,(uint8_t *)&st_net_set_data_cklcd_v2.w_0018,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0019,0x0019,(uint8_t *)&st_net_set_data_cklcd_v2.w_0019,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001A,0x001A,(uint8_t *)&st_net_set_data_cklcd_v2.w_001A,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001B,0x001B,(uint8_t *)&st_net_set_data_cklcd_v2.w_001B,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001C,0x001C,(uint8_t *)&st_net_set_data_cklcd_v2.w_001C,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001D,0x001D,(uint8_t *)&st_net_set_data_cklcd_v2.w_001D,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001E,0x001E,(uint8_t *)&st_net_set_data_cklcd_v2.w_001E,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001F,0x001F,(uint8_t *)&st_net_set_data_cklcd_v2.w_001F,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},//16

    {0x0020,0x0020,(uint8_t *)&st_net_set_data_cklcd_v2.w_0020,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0021,0x0021,(uint8_t *)&st_net_set_data_cklcd_v2.w_0021,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0022,0x0022,(uint8_t *)&st_net_set_data_cklcd_v2.w_0022,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0023,0x0023,(uint8_t *)&st_net_set_data_cklcd_v2.w_0023,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0024,0x0024,(uint8_t *)&st_net_set_data_cklcd_v2.w_0024,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0025,0x0025,(uint8_t *)&st_net_set_data_cklcd_v2.w_0025,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0026,0x0026,(uint8_t *)&st_net_set_data_cklcd_v2.w_0026,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0027,0x0027,(uint8_t *)&st_net_set_data_cklcd_v2.w_0027,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0028,0x0028,(uint8_t *)&st_net_set_data_cklcd_v2.w_0028,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0029,0x0029,(uint8_t *)&st_net_set_data_cklcd_v2.w_0029,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002A,0x002A,(uint8_t *)&st_net_set_data_cklcd_v2.dw_002A,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002B,0x002B,(uint8_t *)&st_net_set_data_cklcd_v2.dw_002B,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002C,0x002C,(uint8_t *)&st_net_set_data_cklcd_v2.w_002C,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002D,0x002D,(uint8_t *)&st_net_set_data_cklcd_v2.w_002D,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002E,0x002E,(uint8_t *)&st_net_set_data_cklcd_v2.w_002E,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002F,0x002F,(uint8_t *)&st_net_set_data_cklcd_v2.w_002F,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},//16

    {0x0030,0x0030,(uint8_t *)&st_net_set_data_cklcd_v2.w_0030,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0031,0x0031,(uint8_t *)&st_net_set_data_cklcd_v2.w_0031,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0032,0x0032,(uint8_t *)&st_net_set_data_cklcd_v2.w_0032,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0033,0x0033,(uint8_t *)&st_net_set_data_cklcd_v2.w_0033,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0034,0x0034,(uint8_t *)&st_net_set_data_cklcd_v2.w_0034,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0035,0x0035,(uint8_t *)&st_net_set_data_cklcd_v2.w_0035,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0036,0x0036,(uint8_t *)&st_net_set_data_cklcd_v2.w_0036,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0037,0x0037,(uint8_t *)&st_net_set_data_cklcd_v2.w_0037,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0038,0x0038,(uint8_t *)&st_net_set_data_cklcd_v2.w_0038,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0039,0x0039,(uint8_t *)&st_net_set_data_cklcd_v2.w_0039,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003A,0x003A,(uint8_t *)&st_net_set_data_cklcd_v2.w_003A,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003B,0x003B,(uint8_t *)&st_net_set_data_cklcd_v2.w_003B,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003C,0x003C,(uint8_t *)&st_net_set_data_cklcd_v2.w_003C,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003D,0x003D,(uint8_t *)&st_net_set_data_cklcd_v2.w_003D,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003E,0x003E,(uint8_t *)&st_net_set_data_cklcd_v2.w_003E,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003F,0x003F,(uint8_t *)&st_net_set_data_cklcd_v2.w_003F,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//14

    {0x0040,0x0040,(uint8_t *)&st_net_set_data_cklcd_v2.w_0040,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0041,0x0041,(uint8_t *)&st_net_set_data_cklcd_v2.dw_0041,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0042,0x0042,(uint8_t *)&st_net_set_data_cklcd_v2.dw_0042,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0043,0x0043,(uint8_t *)&st_net_set_data_cklcd_v2.w_0043,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0044,0x0044,(uint8_t *)&st_net_set_data_cklcd_v2.w_0044,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0045,0x0045,(uint8_t *)&st_net_set_data_cklcd_v2.w_0045,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0046,0x0046,(uint8_t *)&st_net_set_data_cklcd_v2.w_0046,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0047,0x0047,(uint8_t *)&st_net_set_data_cklcd_v2.w_0047,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0048,0x0048,(uint8_t *)&st_net_set_data_cklcd_v2.w_0048,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0049,0x0049,(uint8_t *)&st_net_set_data_cklcd_v2.w_0049,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004A,0x004A,(uint8_t *)&st_net_set_data_cklcd_v2.w_004A,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004B,0x004B,(uint8_t *)&st_net_set_data_cklcd_v2.w_004B,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004C,0x004C,(uint8_t *)&st_net_set_data_cklcd_v2.w_004C,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004D,0x004D,(uint8_t *)&st_net_set_data_cklcd_v2.w_004D,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004E,0x004E,(uint8_t *)&st_net_set_data_cklcd_v2.w_004E,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004F,0x004F,(uint8_t *)&st_net_set_data_cklcd_v2.w_004F,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//14

    {0x0050,0x0050,(uint8_t *)&st_net_set_data_cklcd_v2.w_0050,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0051,0x0051,(uint8_t *)&st_net_set_data_cklcd_v2.w_0051,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0052,0x0052,(uint8_t *)&st_net_set_data_cklcd_v2.w_0052,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0053,0x0053,(uint8_t *)&st_net_set_data_cklcd_v2.w_0053,&st_net_data_att_rw_2_2b_min_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0054,0x0054,(uint8_t *)&st_net_set_data_cklcd_v2.w_0054,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0055,0x0055,(uint8_t *)&st_net_set_data_cklcd_v2.w_0055,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0056,0x0056,(uint8_t *)&st_net_set_data_cklcd_v2.w_0056,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0057,0x0057,(uint8_t *)&st_net_set_data_cklcd_v2.w_0057,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0058,0x0058,(uint8_t *)&st_net_set_data_cklcd_v2.w_0058,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0059,0x0059,(uint8_t *)&st_net_set_data_cklcd_v2.w_0059,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x005A,0x005A,(uint8_t *)&st_net_set_data_cklcd_v2.w_005A,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x005B,0x005B,(uint8_t *)&st_net_set_data_cklcd_v2.w_005B,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x005C,0x005C,(uint8_t *)&st_net_set_data_cklcd_v2.w_005C,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x005D,0x005D,(uint8_t *)&st_net_set_data_cklcd_v2.w_005D,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x005E,0x005E,(uint8_t *)&st_net_set_data_cklcd_v2.w_005E,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x005F,0x005F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//12

    {0x0060,0x0060,(uint8_t *)&st_net_set_data_cklcd_v2.w_0060,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0061,0x0061,(uint8_t *)&st_net_set_data_cklcd_v2.w_0061,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//2
    {0x0062,0x0062,(uint8_t *)&st_net_set_data_cklcd_v2.w_0062,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0063,0x0063,(uint8_t *)&st_net_set_data_cklcd_v2.w_0063,&st_net_data_att_rw_2_2b_w_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0064,0x0064,(uint8_t *)&st_net_set_data_cklcd_v2.w_0064,&st_net_data_att_rw_2_2b_w_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0065,0x0065,(uint8_t *)&st_net_set_data_cklcd_v2.w_0065,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0066,0x0066,(uint8_t *)&st_net_set_data_cklcd_v2.w_0066,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0067,0x0067,(uint8_t *)&st_net_set_data_cklcd_v2.dw_0067,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0068,0x0068,(uint8_t *)&st_net_set_data_cklcd_v2.dw_0068,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0069,0x0069,(uint8_t *)&st_net_set_data_cklcd_v2.w_0069,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006A,0x006A,(uint8_t *)&st_net_set_data_cklcd_v2.w_006A,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006B,0x006B,(uint8_t *)&st_net_set_data_cklcd_v2.w_006B,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006C,0x006C,(uint8_t *)&st_net_set_data_cklcd_v2.w_006C,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006D,0x006D,(uint8_t *)&st_net_set_data_cklcd_v2.w_006D,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006E,0x006E,(uint8_t *)&st_net_set_data_cklcd_v2.w_006E,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006F,0x006F,(uint8_t *)&st_net_set_data_cklcd_v2.w_006F,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},

    {0x0070,0x0070,(uint8_t *)&st_net_set_data_cklcd_v2.dw_0070,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0071,0x0071,(uint8_t *)&st_net_set_data_cklcd_v2.dw_0071,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0072,0x0072,(uint8_t *)&st_net_set_data_cklcd_v2.w_0072,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},//3
	{0x0073,0x0073,(uint8_t *)&st_net_set_data_cklcd_v2.w_0073,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x0074,0x0074,(uint8_t *)&st_net_set_data_cklcd_v2.w_0074,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},

//    {0x0075,0x0005,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0076,0x0006,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0077,0x0007,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0078,0x0008,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0079,0x0009,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007A,0x000A,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007B,0x000B,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007C,0x000C,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007D,0x000D,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007E,0x000E,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007F,0x000F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},

    {0x0080,0x0080,(uint8_t *)&st_net_set_data_cklcd_v2.ba_0080,&st_net_data_att_r_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},//1
//    {0x0090,0x0090,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},
//    {0x00A0,0x00A0,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},
//    {0x00B0,0x00B0,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},
//    {0x00C0,0x00C0,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},

    {0x00D0,0x00D0,(uint8_t *)&st_net_set_data_cklcd_v2.w_00D0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D1,0x00D1,(uint8_t *)&st_net_set_data_cklcd_v2.w_00D1,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D2,0x00D2,(uint8_t *)&st_net_set_data_cklcd_v2.w_00D2,&st_net_data_att_rw_2_2b_h_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x00D3,0x00D3,(uint8_t *)&st_net_set_data_cklcd_v2.w_00D3,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x00D4,0x00D4,(uint8_t *)&st_net_set_data_cklcd_v2.w_00D4,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D5,0x00D5,(uint8_t *)&st_net_set_data_cklcd_v2.dw_00D5,&st_net_data_att_rw_4_4b_1_set,&st_device_data_att_rw_4_cklcd_para,},
//    {0x00D6,0x00D6,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D7,0x00D7,(uint8_t *)&st_net_set_data_cklcd_v2.w_00D7,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D8,0x00D8,(uint8_t *)&st_net_set_data_cklcd_v2.w_00D8,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D9,0x00D9,(uint8_t *)&st_net_set_data_cklcd_v2.w_00D9,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DA,0x00DA,(uint8_t *)&st_net_set_data_cklcd_v2.w_00DA,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_r_2_cklcd_para,},
    {0x00DB,0x00DB,(uint8_t *)&st_net_set_data_cklcd_v2.w_00DB,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DC,0x00DC,(uint8_t *)&st_net_set_data_cklcd_v2.w_00DC,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DD,0x00DD,(uint8_t *)&st_net_set_data_cklcd_v2.w_00DD,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DE,0x00DE,(uint8_t *)&st_net_set_data_cklcd_v2.w_00DE,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DF,0x00DF,(uint8_t *)&st_net_set_data_cklcd_v2.w_00DF,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//15

    {0x00E0,0x00E0,(uint8_t *)&st_net_set_data_cklcd_v2.w_00E0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E1,0x00E1,(uint8_t *)&st_net_set_data_cklcd_v2.w_00E1,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E2,0x00E2,(uint8_t *)&st_net_set_data_cklcd_v2.w_00E2,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E3,0x00E3,(uint8_t *)&st_net_set_data_cklcd_v2.w_00E3,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E4,0x00E4,(uint8_t *)&st_net_set_data_cklcd_v2.w_00E4,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E5,0x00E5,(uint8_t *)&st_net_set_data_cklcd_v2.w_00E5,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E6,0x00E6,(uint8_t *)&st_net_set_data_cklcd_v2.w_00E6,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E7,0x00E7,(uint8_t *)&st_net_set_data_cklcd_v2.w_00E7,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E8,0x00E8,(uint8_t *)&st_net_set_data_cklcd_v2.w_00E8,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E9,0x00E9,(uint8_t *)&st_net_set_data_cklcd_v2.w_00E9,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EA,0x00EA,(uint8_t *)&st_net_set_data_cklcd_v2.w_00EA,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EB,0x00EB,(uint8_t *)&st_net_set_data_cklcd_v2.w_00EB,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EC,0x00EC,(uint8_t *)&st_net_set_data_cklcd_v2.w_00EC,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00ED,0x00ED,(uint8_t *)&st_net_set_data_cklcd_v2.w_00ED,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EE,0x00EE,(uint8_t *)&st_net_set_data_cklcd_v2.w_00EE,&st_net_data_att_rw_2_2b_w_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x00EF,0x000F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//15
};
//net_data_info st_net_control_data_info_cklcd_v2[cw_net_control_data_number_cklcd] =
//{
//    {0x03E8,0x03E8,0,&st_net_data_att_rw_2_2b_1_control,&st_device_data_att_w_2_external_trig},
//};
/************************** CKLCD 6 *****************************/
static net_status_data_cklcd st_net_status_data_cklcd_v3 =
{
    0,
};
net_data_info st_net_status_data_info_cklcd_v3[cw_net_status_data_number_cklcd] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_mppt_v3.w_0100,&st_net_data_att_r_2_2b_1_status,},
    {0x0101,0x0000,(uint8_t *)&st_net_status_data_mppt_v3.w_0101,&st_net_data_att_r_2_2b_w_status,},
    {0x0102,0x0000,(uint8_t *)&st_net_status_data_mppt_v3.w_0102,&st_net_data_att_r_2_2b_0_0_1_ua_status,},
    {0x0103,0x0000,(uint8_t *)&st_net_status_data_mppt_v3.w_0103,&st_net_data_att_r_2_2b_0_1_kwh_status,},
    {0x0104,0x0000,(uint8_t *)&b_reserve,&st_net_data_att_r_1_1b_1_status,},
    {0x0105,0x0000,(uint8_t *)&st_net_status_data_cklcd_v3.w_1005,&st_net_data_att_r_2_2b_w_status,},
    {0x0106,0x0000,(uint8_t *)&st_net_status_data_cklcd_v3.w_1006,&st_net_data_att_r_2_2b_0_1_a_status,},
};
static net_set_data_cklcd st_net_set_data_cklcd_v3 = {0,};
const net_data_info st_net_set_data_info_cklcd_v3[cw_net_set_data_number_cklcd] =
{
    {0x0000,0x0000,(uint8_t *)&st_net_set_data_cklcd_v3.w_0000,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0001,0x0001,(uint8_t *)&st_net_set_data_cklcd_v3.w_0001,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0002,0x0002,(uint8_t *)&st_net_set_data_cklcd_v3.w_0002,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0003,0x0003,(uint8_t *)&st_net_set_data_cklcd_v3.w_0003,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0004,0x0004,(uint8_t *)&st_net_set_data_cklcd_v3.w_0004,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x0005,0x0005,(uint8_t *)&st_net_set_data_cklcd_v3.w_0005,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x0006,0x0006,(uint8_t *)&st_net_set_data_cklcd_v3.w_0006,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0007,0x0007,(uint8_t *)&st_net_set_data_cklcd_v3.w_0007,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0008,0x0008,(uint8_t *)&st_net_set_data_cklcd_v3.w_0008,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0009,0x0009,(uint8_t *)&st_net_set_data_cklcd_v3.w_0009,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x000A,0x000A,(uint8_t *)&st_net_set_data_cklcd_v3.w_000A,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000B,0x000B,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000C,0x000C,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000D,0x000D,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000E,0x000E,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000F,0x000F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//5

    {0x0010,0x0010,(uint8_t *)&st_net_set_data_cklcd_v3.w_0010,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0011,0x0011,(uint8_t *)&st_net_set_data_cklcd_v3.w_0011,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0012,0x0012,(uint8_t *)&st_net_set_data_cklcd_v3.w_0012,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0013,0x0013,(uint8_t *)&st_net_set_data_cklcd_v3.w_0013,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0014,0x0014,(uint8_t *)&st_net_set_data_cklcd_v3.w_0014,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0015,0x0015,(uint8_t *)&st_net_set_data_cklcd_v3.dw_0015,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0016,0x0016,(uint8_t *)&st_net_set_data_cklcd_v3.dw_0016,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0017,0x0017,(uint8_t *)&st_net_set_data_cklcd_v3.w_0017,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0018,0x0018,(uint8_t *)&st_net_set_data_cklcd_v3.w_0018,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0019,0x0019,(uint8_t *)&st_net_set_data_cklcd_v3.w_0019,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001A,0x001A,(uint8_t *)&st_net_set_data_cklcd_v3.w_001A,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001B,0x001B,(uint8_t *)&st_net_set_data_cklcd_v3.w_001B,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001C,0x001C,(uint8_t *)&st_net_set_data_cklcd_v3.w_001C,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001D,0x001D,(uint8_t *)&st_net_set_data_cklcd_v3.w_001D,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001E,0x001E,(uint8_t *)&st_net_set_data_cklcd_v3.w_001E,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001F,0x001F,(uint8_t *)&st_net_set_data_cklcd_v3.w_001F,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},//16

    {0x0020,0x0020,(uint8_t *)&st_net_set_data_cklcd_v3.w_0020,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0021,0x0021,(uint8_t *)&st_net_set_data_cklcd_v3.w_0021,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0022,0x0022,(uint8_t *)&st_net_set_data_cklcd_v3.w_0022,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0023,0x0023,(uint8_t *)&st_net_set_data_cklcd_v3.w_0023,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0024,0x0024,(uint8_t *)&st_net_set_data_cklcd_v3.w_0024,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0025,0x0025,(uint8_t *)&st_net_set_data_cklcd_v3.w_0025,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0026,0x0026,(uint8_t *)&st_net_set_data_cklcd_v3.w_0026,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0027,0x0027,(uint8_t *)&st_net_set_data_cklcd_v3.w_0027,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0028,0x0028,(uint8_t *)&st_net_set_data_cklcd_v3.w_0028,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0029,0x0029,(uint8_t *)&st_net_set_data_cklcd_v3.w_0029,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002A,0x002A,(uint8_t *)&st_net_set_data_cklcd_v3.dw_002A,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002B,0x002B,(uint8_t *)&st_net_set_data_cklcd_v3.dw_002B,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002C,0x002C,(uint8_t *)&st_net_set_data_cklcd_v3.w_002C,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002D,0x002D,(uint8_t *)&st_net_set_data_cklcd_v3.w_002D,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002E,0x002E,(uint8_t *)&st_net_set_data_cklcd_v3.w_002E,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002F,0x002F,(uint8_t *)&st_net_set_data_cklcd_v3.w_002F,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},//16

    {0x0030,0x0030,(uint8_t *)&st_net_set_data_cklcd_v3.w_0030,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0031,0x0031,(uint8_t *)&st_net_set_data_cklcd_v3.w_0031,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0032,0x0032,(uint8_t *)&st_net_set_data_cklcd_v3.w_0032,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0033,0x0033,(uint8_t *)&st_net_set_data_cklcd_v3.w_0033,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0034,0x0034,(uint8_t *)&st_net_set_data_cklcd_v3.w_0034,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0035,0x0035,(uint8_t *)&st_net_set_data_cklcd_v3.w_0035,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0036,0x0036,(uint8_t *)&st_net_set_data_cklcd_v3.w_0036,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0037,0x0037,(uint8_t *)&st_net_set_data_cklcd_v3.w_0037,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0038,0x0038,(uint8_t *)&st_net_set_data_cklcd_v3.w_0038,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0039,0x0039,(uint8_t *)&st_net_set_data_cklcd_v3.w_0039,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003A,0x003A,(uint8_t *)&st_net_set_data_cklcd_v3.w_003A,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003B,0x003B,(uint8_t *)&st_net_set_data_cklcd_v3.w_003B,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003C,0x003C,(uint8_t *)&st_net_set_data_cklcd_v3.w_003C,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003D,0x003D,(uint8_t *)&st_net_set_data_cklcd_v3.w_003D,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003E,0x003E,(uint8_t *)&st_net_set_data_cklcd_v3.w_003E,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003F,0x003F,(uint8_t *)&st_net_set_data_cklcd_v3.w_003F,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//14

    {0x0040,0x0040,(uint8_t *)&st_net_set_data_cklcd_v3.w_0040,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0041,0x0041,(uint8_t *)&st_net_set_data_cklcd_v3.dw_0041,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0042,0x0042,(uint8_t *)&st_net_set_data_cklcd_v3.dw_0042,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0043,0x0043,(uint8_t *)&st_net_set_data_cklcd_v3.w_0043,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0044,0x0044,(uint8_t *)&st_net_set_data_cklcd_v3.w_0044,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0045,0x0045,(uint8_t *)&st_net_set_data_cklcd_v3.w_0045,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0046,0x0046,(uint8_t *)&st_net_set_data_cklcd_v3.w_0046,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0047,0x0047,(uint8_t *)&st_net_set_data_cklcd_v3.w_0047,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0048,0x0048,(uint8_t *)&st_net_set_data_cklcd_v3.w_0048,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0049,0x0049,(uint8_t *)&st_net_set_data_cklcd_v3.w_0049,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004A,0x004A,(uint8_t *)&st_net_set_data_cklcd_v3.w_004A,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004B,0x004B,(uint8_t *)&st_net_set_data_cklcd_v3.w_004B,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004C,0x004C,(uint8_t *)&st_net_set_data_cklcd_v3.w_004C,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004D,0x004D,(uint8_t *)&st_net_set_data_cklcd_v3.w_004D,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004E,0x004E,(uint8_t *)&st_net_set_data_cklcd_v3.w_004E,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004F,0x004F,(uint8_t *)&st_net_set_data_cklcd_v3.w_004F,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//14

    {0x0050,0x0050,(uint8_t *)&st_net_set_data_cklcd_v3.w_0050,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0051,0x0051,(uint8_t *)&st_net_set_data_cklcd_v3.w_0051,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0052,0x0052,(uint8_t *)&st_net_set_data_cklcd_v3.w_0052,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0053,0x0053,(uint8_t *)&st_net_set_data_cklcd_v3.w_0053,&st_net_data_att_rw_2_2b_min_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0054,0x0054,(uint8_t *)&st_net_set_data_cklcd_v3.w_0054,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0055,0x0055,(uint8_t *)&st_net_set_data_cklcd_v3.w_0055,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0056,0x0056,(uint8_t *)&st_net_set_data_cklcd_v3.w_0056,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0057,0x0057,(uint8_t *)&st_net_set_data_cklcd_v3.w_0057,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0058,0x0058,(uint8_t *)&st_net_set_data_cklcd_v3.w_0058,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0059,0x0059,(uint8_t *)&st_net_set_data_cklcd_v3.w_0059,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x005A,0x005A,(uint8_t *)&st_net_set_data_cklcd_v3.w_005A,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x005B,0x005B,(uint8_t *)&st_net_set_data_cklcd_v3.w_005B,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x005C,0x005C,(uint8_t *)&st_net_set_data_cklcd_v3.w_005C,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x005D,0x005D,(uint8_t *)&st_net_set_data_cklcd_v3.w_005D,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x005E,0x005E,(uint8_t *)&st_net_set_data_cklcd_v3.w_005E,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x005F,0x005F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//12

    {0x0060,0x0060,(uint8_t *)&st_net_set_data_cklcd_v3.w_0060,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0061,0x0061,(uint8_t *)&st_net_set_data_cklcd_v3.w_0061,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//2
    {0x0062,0x0062,(uint8_t *)&st_net_set_data_cklcd_v3.w_0062,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0063,0x0063,(uint8_t *)&st_net_set_data_cklcd_v3.w_0063,&st_net_data_att_rw_2_2b_w_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0064,0x0064,(uint8_t *)&st_net_set_data_cklcd_v3.w_0064,&st_net_data_att_rw_2_2b_w_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0065,0x0065,(uint8_t *)&st_net_set_data_cklcd_v3.w_0065,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0066,0x0066,(uint8_t *)&st_net_set_data_cklcd_v3.w_0066,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0067,0x0067,(uint8_t *)&st_net_set_data_cklcd_v3.dw_0067,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0068,0x0068,(uint8_t *)&st_net_set_data_cklcd_v3.dw_0068,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0069,0x0069,(uint8_t *)&st_net_set_data_cklcd_v3.w_0069,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006A,0x006A,(uint8_t *)&st_net_set_data_cklcd_v3.w_006A,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006B,0x006B,(uint8_t *)&st_net_set_data_cklcd_v3.w_006B,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006C,0x006C,(uint8_t *)&st_net_set_data_cklcd_v3.w_006C,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006D,0x006D,(uint8_t *)&st_net_set_data_cklcd_v3.w_006D,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006E,0x006E,(uint8_t *)&st_net_set_data_cklcd_v3.w_006E,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006F,0x006F,(uint8_t *)&st_net_set_data_cklcd_v3.w_006F,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},

    {0x0070,0x0070,(uint8_t *)&st_net_set_data_cklcd_v3.dw_0070,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0071,0x0071,(uint8_t *)&st_net_set_data_cklcd_v3.dw_0071,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0072,0x0072,(uint8_t *)&st_net_set_data_cklcd_v3.w_0072,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},//3
	{0x0073,0x0073,(uint8_t *)&st_net_set_data_cklcd_v3.w_0073,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x0074,0x0074,(uint8_t *)&st_net_set_data_cklcd_v3.w_0074,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},

//    {0x0075,0x0005,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0076,0x0006,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0077,0x0007,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0078,0x0008,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0079,0x0009,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007A,0x000A,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007B,0x000B,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007C,0x000C,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007D,0x000D,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007E,0x000E,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007F,0x000F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},

    {0x0080,0x0080,(uint8_t *)&st_net_set_data_cklcd_v3.ba_0080,&st_net_data_att_r_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},//1
//    {0x0090,0x0090,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},
//    {0x00A0,0x00A0,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},
//    {0x00B0,0x00B0,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},
//    {0x00C0,0x00C0,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},

    {0x00D0,0x00D0,(uint8_t *)&st_net_set_data_cklcd_v3.w_00D0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D1,0x00D1,(uint8_t *)&st_net_set_data_cklcd_v3.w_00D1,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D2,0x00D2,(uint8_t *)&st_net_set_data_cklcd_v3.w_00D2,&st_net_data_att_rw_2_2b_h_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x00D3,0x00D3,(uint8_t *)&st_net_set_data_cklcd_v3.w_00D3,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x00D4,0x00D4,(uint8_t *)&st_net_set_data_cklcd_v3.w_00D4,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D5,0x00D5,(uint8_t *)&st_net_set_data_cklcd_v3.dw_00D5,&st_net_data_att_rw_4_4b_1_set,&st_device_data_att_rw_4_cklcd_para,},
//    {0x00D6,0x00D6,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D7,0x00D7,(uint8_t *)&st_net_set_data_cklcd_v3.w_00D7,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D8,0x00D8,(uint8_t *)&st_net_set_data_cklcd_v3.w_00D8,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D9,0x00D9,(uint8_t *)&st_net_set_data_cklcd_v3.w_00D9,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DA,0x00DA,(uint8_t *)&st_net_set_data_cklcd_v3.w_00DA,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_r_2_cklcd_para,},
    {0x00DB,0x00DB,(uint8_t *)&st_net_set_data_cklcd_v3.w_00DB,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DC,0x00DC,(uint8_t *)&st_net_set_data_cklcd_v3.w_00DC,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DD,0x00DD,(uint8_t *)&st_net_set_data_cklcd_v3.w_00DD,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DE,0x00DE,(uint8_t *)&st_net_set_data_cklcd_v3.w_00DE,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DF,0x00DF,(uint8_t *)&st_net_set_data_cklcd_v3.w_00DF,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//15

    {0x00E0,0x00E0,(uint8_t *)&st_net_set_data_cklcd_v3.w_00E0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E1,0x00E1,(uint8_t *)&st_net_set_data_cklcd_v3.w_00E1,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E2,0x00E2,(uint8_t *)&st_net_set_data_cklcd_v3.w_00E2,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E3,0x00E3,(uint8_t *)&st_net_set_data_cklcd_v3.w_00E3,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E4,0x00E4,(uint8_t *)&st_net_set_data_cklcd_v3.w_00E4,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E5,0x00E5,(uint8_t *)&st_net_set_data_cklcd_v3.w_00E5,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E6,0x00E6,(uint8_t *)&st_net_set_data_cklcd_v3.w_00E6,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E7,0x00E7,(uint8_t *)&st_net_set_data_cklcd_v3.w_00E7,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E8,0x00E8,(uint8_t *)&st_net_set_data_cklcd_v3.w_00E8,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E9,0x00E9,(uint8_t *)&st_net_set_data_cklcd_v3.w_00E9,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EA,0x00EA,(uint8_t *)&st_net_set_data_cklcd_v3.w_00EA,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EB,0x00EB,(uint8_t *)&st_net_set_data_cklcd_v3.w_00EB,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EC,0x00EC,(uint8_t *)&st_net_set_data_cklcd_v3.w_00EC,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00ED,0x00ED,(uint8_t *)&st_net_set_data_cklcd_v3.w_00ED,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EE,0x00EE,(uint8_t *)&st_net_set_data_cklcd_v3.w_00EE,&st_net_data_att_rw_2_2b_w_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x00EF,0x000F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//15
};
//net_data_info st_net_control_data_info_cklcd_v3[cw_net_control_data_number_cklcd] =
//{
//    {0x03E8,0x03E8,0,&st_net_data_att_rw_2_2b_1_control,&st_device_data_att_w_2_external_trig},
//};
/************************** CKLCD 7 *****************************/
static net_status_data_cklcd st_net_status_data_cklcd_w1 =
{
    0,
};
net_data_info st_net_status_data_info_cklcd_w1[cw_net_status_data_number_cklcd] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_mppt_w1.w_0100,&st_net_data_att_r_2_2b_1_status,},
    {0x0101,0x0000,(uint8_t *)&st_net_status_data_mppt_w1.w_0101,&st_net_data_att_r_2_2b_w_status,},
    {0x0102,0x0000,(uint8_t *)&st_net_status_data_mppt_w1.w_0102,&st_net_data_att_r_2_2b_0_0_1_ua_status,},
    {0x0103,0x0000,(uint8_t *)&st_net_status_data_mppt_w1.w_0103,&st_net_data_att_r_2_2b_0_1_kwh_status,},
    {0x0104,0x0000,(uint8_t *)&b_reserve,&st_net_data_att_r_1_1b_1_status,},
    {0x0105,0x0000,(uint8_t *)&st_net_status_data_cklcd_w1.w_1005,&st_net_data_att_r_2_2b_w_status,},
    {0x0106,0x0000,(uint8_t *)&st_net_status_data_cklcd_w1.w_1006,&st_net_data_att_r_2_2b_0_1_a_status,},
};
static net_set_data_cklcd st_net_set_data_cklcd_w1 = {0,};
const net_data_info st_net_set_data_info_cklcd_w1[cw_net_set_data_number_cklcd] =
{
    {0x0000,0x0000,(uint8_t *)&st_net_set_data_cklcd_w1.w_0000,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0001,0x0001,(uint8_t *)&st_net_set_data_cklcd_w1.w_0001,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0002,0x0002,(uint8_t *)&st_net_set_data_cklcd_w1.w_0002,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0003,0x0003,(uint8_t *)&st_net_set_data_cklcd_w1.w_0003,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0004,0x0004,(uint8_t *)&st_net_set_data_cklcd_w1.w_0004,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x0005,0x0005,(uint8_t *)&st_net_set_data_cklcd_w1.w_0005,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x0006,0x0006,(uint8_t *)&st_net_set_data_cklcd_w1.w_0006,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0007,0x0007,(uint8_t *)&st_net_set_data_cklcd_w1.w_0007,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0008,0x0008,(uint8_t *)&st_net_set_data_cklcd_w1.w_0008,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0009,0x0009,(uint8_t *)&st_net_set_data_cklcd_w1.w_0009,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x000A,0x000A,(uint8_t *)&st_net_set_data_cklcd_w1.w_000A,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000B,0x000B,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000C,0x000C,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000D,0x000D,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000E,0x000E,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000F,0x000F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//5

    {0x0010,0x0010,(uint8_t *)&st_net_set_data_cklcd_w1.w_0010,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0011,0x0011,(uint8_t *)&st_net_set_data_cklcd_w1.w_0011,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0012,0x0012,(uint8_t *)&st_net_set_data_cklcd_w1.w_0012,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0013,0x0013,(uint8_t *)&st_net_set_data_cklcd_w1.w_0013,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0014,0x0014,(uint8_t *)&st_net_set_data_cklcd_w1.w_0014,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0015,0x0015,(uint8_t *)&st_net_set_data_cklcd_w1.dw_0015,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0016,0x0016,(uint8_t *)&st_net_set_data_cklcd_w1.dw_0016,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0017,0x0017,(uint8_t *)&st_net_set_data_cklcd_w1.w_0017,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0018,0x0018,(uint8_t *)&st_net_set_data_cklcd_w1.w_0018,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0019,0x0019,(uint8_t *)&st_net_set_data_cklcd_w1.w_0019,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001A,0x001A,(uint8_t *)&st_net_set_data_cklcd_w1.w_001A,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001B,0x001B,(uint8_t *)&st_net_set_data_cklcd_w1.w_001B,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001C,0x001C,(uint8_t *)&st_net_set_data_cklcd_w1.w_001C,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001D,0x001D,(uint8_t *)&st_net_set_data_cklcd_w1.w_001D,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001E,0x001E,(uint8_t *)&st_net_set_data_cklcd_w1.w_001E,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001F,0x001F,(uint8_t *)&st_net_set_data_cklcd_w1.w_001F,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},//16

    {0x0020,0x0020,(uint8_t *)&st_net_set_data_cklcd_w1.w_0020,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0021,0x0021,(uint8_t *)&st_net_set_data_cklcd_w1.w_0021,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0022,0x0022,(uint8_t *)&st_net_set_data_cklcd_w1.w_0022,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0023,0x0023,(uint8_t *)&st_net_set_data_cklcd_w1.w_0023,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0024,0x0024,(uint8_t *)&st_net_set_data_cklcd_w1.w_0024,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0025,0x0025,(uint8_t *)&st_net_set_data_cklcd_w1.w_0025,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0026,0x0026,(uint8_t *)&st_net_set_data_cklcd_w1.w_0026,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0027,0x0027,(uint8_t *)&st_net_set_data_cklcd_w1.w_0027,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0028,0x0028,(uint8_t *)&st_net_set_data_cklcd_w1.w_0028,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0029,0x0029,(uint8_t *)&st_net_set_data_cklcd_w1.w_0029,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002A,0x002A,(uint8_t *)&st_net_set_data_cklcd_w1.dw_002A,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002B,0x002B,(uint8_t *)&st_net_set_data_cklcd_w1.dw_002B,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002C,0x002C,(uint8_t *)&st_net_set_data_cklcd_w1.w_002C,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002D,0x002D,(uint8_t *)&st_net_set_data_cklcd_w1.w_002D,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002E,0x002E,(uint8_t *)&st_net_set_data_cklcd_w1.w_002E,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002F,0x002F,(uint8_t *)&st_net_set_data_cklcd_w1.w_002F,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},//16

    {0x0030,0x0030,(uint8_t *)&st_net_set_data_cklcd_w1.w_0030,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0031,0x0031,(uint8_t *)&st_net_set_data_cklcd_w1.w_0031,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0032,0x0032,(uint8_t *)&st_net_set_data_cklcd_w1.w_0032,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0033,0x0033,(uint8_t *)&st_net_set_data_cklcd_w1.w_0033,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0034,0x0034,(uint8_t *)&st_net_set_data_cklcd_w1.w_0034,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0035,0x0035,(uint8_t *)&st_net_set_data_cklcd_w1.w_0035,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0036,0x0036,(uint8_t *)&st_net_set_data_cklcd_w1.w_0036,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0037,0x0037,(uint8_t *)&st_net_set_data_cklcd_w1.w_0037,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0038,0x0038,(uint8_t *)&st_net_set_data_cklcd_w1.w_0038,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0039,0x0039,(uint8_t *)&st_net_set_data_cklcd_w1.w_0039,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003A,0x003A,(uint8_t *)&st_net_set_data_cklcd_w1.w_003A,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003B,0x003B,(uint8_t *)&st_net_set_data_cklcd_w1.w_003B,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003C,0x003C,(uint8_t *)&st_net_set_data_cklcd_w1.w_003C,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003D,0x003D,(uint8_t *)&st_net_set_data_cklcd_w1.w_003D,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003E,0x003E,(uint8_t *)&st_net_set_data_cklcd_w1.w_003E,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003F,0x003F,(uint8_t *)&st_net_set_data_cklcd_w1.w_003F,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//14

    {0x0040,0x0040,(uint8_t *)&st_net_set_data_cklcd_w1.w_0040,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0041,0x0041,(uint8_t *)&st_net_set_data_cklcd_w1.dw_0041,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0042,0x0042,(uint8_t *)&st_net_set_data_cklcd_w1.dw_0042,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0043,0x0043,(uint8_t *)&st_net_set_data_cklcd_w1.w_0043,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0044,0x0044,(uint8_t *)&st_net_set_data_cklcd_w1.w_0044,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0045,0x0045,(uint8_t *)&st_net_set_data_cklcd_w1.w_0045,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0046,0x0046,(uint8_t *)&st_net_set_data_cklcd_w1.w_0046,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0047,0x0047,(uint8_t *)&st_net_set_data_cklcd_w1.w_0047,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0048,0x0048,(uint8_t *)&st_net_set_data_cklcd_w1.w_0048,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0049,0x0049,(uint8_t *)&st_net_set_data_cklcd_w1.w_0049,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004A,0x004A,(uint8_t *)&st_net_set_data_cklcd_w1.w_004A,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004B,0x004B,(uint8_t *)&st_net_set_data_cklcd_w1.w_004B,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004C,0x004C,(uint8_t *)&st_net_set_data_cklcd_w1.w_004C,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004D,0x004D,(uint8_t *)&st_net_set_data_cklcd_w1.w_004D,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004E,0x004E,(uint8_t *)&st_net_set_data_cklcd_w1.w_004E,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004F,0x004F,(uint8_t *)&st_net_set_data_cklcd_w1.w_004F,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//14

    {0x0050,0x0050,(uint8_t *)&st_net_set_data_cklcd_w1.w_0050,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0051,0x0051,(uint8_t *)&st_net_set_data_cklcd_w1.w_0051,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0052,0x0052,(uint8_t *)&st_net_set_data_cklcd_w1.w_0052,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0053,0x0053,(uint8_t *)&st_net_set_data_cklcd_w1.w_0053,&st_net_data_att_rw_2_2b_min_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0054,0x0054,(uint8_t *)&st_net_set_data_cklcd_w1.w_0054,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0055,0x0055,(uint8_t *)&st_net_set_data_cklcd_w1.w_0055,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0056,0x0056,(uint8_t *)&st_net_set_data_cklcd_w1.w_0056,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0057,0x0057,(uint8_t *)&st_net_set_data_cklcd_w1.w_0057,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0058,0x0058,(uint8_t *)&st_net_set_data_cklcd_w1.w_0058,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0059,0x0059,(uint8_t *)&st_net_set_data_cklcd_w1.w_0059,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x005A,0x005A,(uint8_t *)&st_net_set_data_cklcd_w1.w_005A,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x005B,0x005B,(uint8_t *)&st_net_set_data_cklcd_w1.w_005B,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x005C,0x005C,(uint8_t *)&st_net_set_data_cklcd_w1.w_005C,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x005D,0x005D,(uint8_t *)&st_net_set_data_cklcd_w1.w_005D,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x005E,0x005E,(uint8_t *)&st_net_set_data_cklcd_w1.w_005E,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x005F,0x005F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//12

    {0x0060,0x0060,(uint8_t *)&st_net_set_data_cklcd_w1.w_0060,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0061,0x0061,(uint8_t *)&st_net_set_data_cklcd_w1.w_0061,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//2
    {0x0062,0x0062,(uint8_t *)&st_net_set_data_cklcd_w1.w_0062,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0063,0x0063,(uint8_t *)&st_net_set_data_cklcd_w1.w_0063,&st_net_data_att_rw_2_2b_w_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0064,0x0064,(uint8_t *)&st_net_set_data_cklcd_w1.w_0064,&st_net_data_att_rw_2_2b_w_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0065,0x0065,(uint8_t *)&st_net_set_data_cklcd_w1.w_0065,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0066,0x0066,(uint8_t *)&st_net_set_data_cklcd_w1.w_0066,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0067,0x0067,(uint8_t *)&st_net_set_data_cklcd_w1.dw_0067,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0068,0x0068,(uint8_t *)&st_net_set_data_cklcd_w1.dw_0068,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0069,0x0069,(uint8_t *)&st_net_set_data_cklcd_w1.w_0069,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006A,0x006A,(uint8_t *)&st_net_set_data_cklcd_w1.w_006A,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006B,0x006B,(uint8_t *)&st_net_set_data_cklcd_w1.w_006B,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006C,0x006C,(uint8_t *)&st_net_set_data_cklcd_w1.w_006C,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006D,0x006D,(uint8_t *)&st_net_set_data_cklcd_w1.w_006D,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006E,0x006E,(uint8_t *)&st_net_set_data_cklcd_w1.w_006E,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006F,0x006F,(uint8_t *)&st_net_set_data_cklcd_w1.w_006F,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},

    {0x0070,0x0070,(uint8_t *)&st_net_set_data_cklcd_w1.dw_0070,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0071,0x0071,(uint8_t *)&st_net_set_data_cklcd_w1.dw_0071,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0072,0x0072,(uint8_t *)&st_net_set_data_cklcd_w1.w_0072,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},//3
	{0x0073,0x0073,(uint8_t *)&st_net_set_data_cklcd_w1.w_0073,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x0074,0x0074,(uint8_t *)&st_net_set_data_cklcd_w1.w_0074,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},

//    {0x0075,0x0005,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0076,0x0006,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0077,0x0007,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0078,0x0008,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0079,0x0009,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007A,0x000A,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007B,0x000B,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007C,0x000C,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007D,0x000D,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007E,0x000E,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007F,0x000F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},

    {0x0080,0x0080,(uint8_t *)&st_net_set_data_cklcd_w1.ba_0080,&st_net_data_att_r_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},//1
//    {0x0090,0x0090,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},
//    {0x00A0,0x00A0,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},
//    {0x00B0,0x00B0,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},
//    {0x00C0,0x00C0,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},

    {0x00D0,0x00D0,(uint8_t *)&st_net_set_data_cklcd_w1.w_00D0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D1,0x00D1,(uint8_t *)&st_net_set_data_cklcd_w1.w_00D1,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D2,0x00D2,(uint8_t *)&st_net_set_data_cklcd_w1.w_00D2,&st_net_data_att_rw_2_2b_h_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x00D3,0x00D3,(uint8_t *)&st_net_set_data_cklcd_w1.w_00D3,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x00D4,0x00D4,(uint8_t *)&st_net_set_data_cklcd_w1.w_00D4,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D5,0x00D5,(uint8_t *)&st_net_set_data_cklcd_w1.dw_00D5,&st_net_data_att_rw_4_4b_1_set,&st_device_data_att_rw_4_cklcd_para,},
//    {0x00D6,0x00D6,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D7,0x00D7,(uint8_t *)&st_net_set_data_cklcd_w1.w_00D7,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D8,0x00D8,(uint8_t *)&st_net_set_data_cklcd_w1.w_00D8,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D9,0x00D9,(uint8_t *)&st_net_set_data_cklcd_w1.w_00D9,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DA,0x00DA,(uint8_t *)&st_net_set_data_cklcd_w1.w_00DA,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_r_2_cklcd_para,},
    {0x00DB,0x00DB,(uint8_t *)&st_net_set_data_cklcd_w1.w_00DB,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DC,0x00DC,(uint8_t *)&st_net_set_data_cklcd_w1.w_00DC,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DD,0x00DD,(uint8_t *)&st_net_set_data_cklcd_w1.w_00DD,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DE,0x00DE,(uint8_t *)&st_net_set_data_cklcd_w1.w_00DE,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DF,0x00DF,(uint8_t *)&st_net_set_data_cklcd_w1.w_00DF,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//15

    {0x00E0,0x00E0,(uint8_t *)&st_net_set_data_cklcd_w1.w_00E0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E1,0x00E1,(uint8_t *)&st_net_set_data_cklcd_w1.w_00E1,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E2,0x00E2,(uint8_t *)&st_net_set_data_cklcd_w1.w_00E2,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E3,0x00E3,(uint8_t *)&st_net_set_data_cklcd_w1.w_00E3,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E4,0x00E4,(uint8_t *)&st_net_set_data_cklcd_w1.w_00E4,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E5,0x00E5,(uint8_t *)&st_net_set_data_cklcd_w1.w_00E5,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E6,0x00E6,(uint8_t *)&st_net_set_data_cklcd_w1.w_00E6,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E7,0x00E7,(uint8_t *)&st_net_set_data_cklcd_w1.w_00E7,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E8,0x00E8,(uint8_t *)&st_net_set_data_cklcd_w1.w_00E8,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E9,0x00E9,(uint8_t *)&st_net_set_data_cklcd_w1.w_00E9,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EA,0x00EA,(uint8_t *)&st_net_set_data_cklcd_w1.w_00EA,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EB,0x00EB,(uint8_t *)&st_net_set_data_cklcd_w1.w_00EB,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EC,0x00EC,(uint8_t *)&st_net_set_data_cklcd_w1.w_00EC,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00ED,0x00ED,(uint8_t *)&st_net_set_data_cklcd_w1.w_00ED,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EE,0x00EE,(uint8_t *)&st_net_set_data_cklcd_w1.w_00EE,&st_net_data_att_rw_2_2b_w_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x00EF,0x000F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//15
};
//net_data_info st_net_control_data_info_cklcd_w1[cw_net_control_data_number_cklcd] =
//{
//    {0x03E8,0x03E8,0,&st_net_data_att_rw_2_2b_1_control,&st_device_data_att_w_2_external_trig},
//};
/************************** CKLCD 8 *****************************/
static net_status_data_cklcd st_net_status_data_cklcd_w2 =
{
    0,
};
net_data_info st_net_status_data_info_cklcd_w2[cw_net_status_data_number_cklcd] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_mppt_w2.w_0100,&st_net_data_att_r_2_2b_1_status,},
    {0x0101,0x0000,(uint8_t *)&st_net_status_data_mppt_w2.w_0101,&st_net_data_att_r_2_2b_w_status,},
    {0x0102,0x0000,(uint8_t *)&st_net_status_data_mppt_w2.w_0102,&st_net_data_att_r_2_2b_0_0_1_ua_status,},
    {0x0103,0x0000,(uint8_t *)&st_net_status_data_mppt_w2.w_0103,&st_net_data_att_r_2_2b_0_1_kwh_status,},
    {0x0104,0x0000,(uint8_t *)&b_reserve,&st_net_data_att_r_1_1b_1_status,},
    {0x0105,0x0000,(uint8_t *)&st_net_status_data_cklcd_w2.w_1005,&st_net_data_att_r_2_2b_w_status,},
    {0x0106,0x0000,(uint8_t *)&st_net_status_data_cklcd_w2.w_1006,&st_net_data_att_r_2_2b_0_1_a_status,},
};
static net_set_data_cklcd st_net_set_data_cklcd_w2 = {0,};
const net_data_info st_net_set_data_info_cklcd_w2[cw_net_set_data_number_cklcd] =
{
    {0x0000,0x0000,(uint8_t *)&st_net_set_data_cklcd_w2.w_0000,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0001,0x0001,(uint8_t *)&st_net_set_data_cklcd_w2.w_0001,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0002,0x0002,(uint8_t *)&st_net_set_data_cklcd_w2.w_0002,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0003,0x0003,(uint8_t *)&st_net_set_data_cklcd_w2.w_0003,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0004,0x0004,(uint8_t *)&st_net_set_data_cklcd_w2.w_0004,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x0005,0x0005,(uint8_t *)&st_net_set_data_cklcd_w2.w_0005,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x0006,0x0006,(uint8_t *)&st_net_set_data_cklcd_w2.w_0006,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0007,0x0007,(uint8_t *)&st_net_set_data_cklcd_w2.w_0007,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0008,0x0008,(uint8_t *)&st_net_set_data_cklcd_w2.w_0008,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0009,0x0009,(uint8_t *)&st_net_set_data_cklcd_w2.w_0009,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x000A,0x000A,(uint8_t *)&st_net_set_data_cklcd_w2.w_000A,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000B,0x000B,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000C,0x000C,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000D,0x000D,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000E,0x000E,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000F,0x000F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//5

    {0x0010,0x0010,(uint8_t *)&st_net_set_data_cklcd_w2.w_0010,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0011,0x0011,(uint8_t *)&st_net_set_data_cklcd_w2.w_0011,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0012,0x0012,(uint8_t *)&st_net_set_data_cklcd_w2.w_0012,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0013,0x0013,(uint8_t *)&st_net_set_data_cklcd_w2.w_0013,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0014,0x0014,(uint8_t *)&st_net_set_data_cklcd_w2.w_0014,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0015,0x0015,(uint8_t *)&st_net_set_data_cklcd_w2.dw_0015,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0016,0x0016,(uint8_t *)&st_net_set_data_cklcd_w2.dw_0016,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0017,0x0017,(uint8_t *)&st_net_set_data_cklcd_w2.w_0017,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0018,0x0018,(uint8_t *)&st_net_set_data_cklcd_w2.w_0018,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0019,0x0019,(uint8_t *)&st_net_set_data_cklcd_w2.w_0019,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001A,0x001A,(uint8_t *)&st_net_set_data_cklcd_w2.w_001A,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001B,0x001B,(uint8_t *)&st_net_set_data_cklcd_w2.w_001B,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001C,0x001C,(uint8_t *)&st_net_set_data_cklcd_w2.w_001C,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001D,0x001D,(uint8_t *)&st_net_set_data_cklcd_w2.w_001D,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001E,0x001E,(uint8_t *)&st_net_set_data_cklcd_w2.w_001E,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001F,0x001F,(uint8_t *)&st_net_set_data_cklcd_w2.w_001F,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},//16

    {0x0020,0x0020,(uint8_t *)&st_net_set_data_cklcd_w2.w_0020,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0021,0x0021,(uint8_t *)&st_net_set_data_cklcd_w2.w_0021,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0022,0x0022,(uint8_t *)&st_net_set_data_cklcd_w2.w_0022,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0023,0x0023,(uint8_t *)&st_net_set_data_cklcd_w2.w_0023,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0024,0x0024,(uint8_t *)&st_net_set_data_cklcd_w2.w_0024,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0025,0x0025,(uint8_t *)&st_net_set_data_cklcd_w2.w_0025,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0026,0x0026,(uint8_t *)&st_net_set_data_cklcd_w2.w_0026,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0027,0x0027,(uint8_t *)&st_net_set_data_cklcd_w2.w_0027,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0028,0x0028,(uint8_t *)&st_net_set_data_cklcd_w2.w_0028,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0029,0x0029,(uint8_t *)&st_net_set_data_cklcd_w2.w_0029,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002A,0x002A,(uint8_t *)&st_net_set_data_cklcd_w2.dw_002A,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002B,0x002B,(uint8_t *)&st_net_set_data_cklcd_w2.dw_002B,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002C,0x002C,(uint8_t *)&st_net_set_data_cklcd_w2.w_002C,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002D,0x002D,(uint8_t *)&st_net_set_data_cklcd_w2.w_002D,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002E,0x002E,(uint8_t *)&st_net_set_data_cklcd_w2.w_002E,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002F,0x002F,(uint8_t *)&st_net_set_data_cklcd_w2.w_002F,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},//16

    {0x0030,0x0030,(uint8_t *)&st_net_set_data_cklcd_w2.w_0030,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0031,0x0031,(uint8_t *)&st_net_set_data_cklcd_w2.w_0031,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0032,0x0032,(uint8_t *)&st_net_set_data_cklcd_w2.w_0032,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0033,0x0033,(uint8_t *)&st_net_set_data_cklcd_w2.w_0033,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0034,0x0034,(uint8_t *)&st_net_set_data_cklcd_w2.w_0034,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0035,0x0035,(uint8_t *)&st_net_set_data_cklcd_w2.w_0035,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0036,0x0036,(uint8_t *)&st_net_set_data_cklcd_w2.w_0036,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0037,0x0037,(uint8_t *)&st_net_set_data_cklcd_w2.w_0037,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0038,0x0038,(uint8_t *)&st_net_set_data_cklcd_w2.w_0038,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0039,0x0039,(uint8_t *)&st_net_set_data_cklcd_w2.w_0039,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003A,0x003A,(uint8_t *)&st_net_set_data_cklcd_w2.w_003A,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003B,0x003B,(uint8_t *)&st_net_set_data_cklcd_w2.w_003B,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003C,0x003C,(uint8_t *)&st_net_set_data_cklcd_w2.w_003C,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003D,0x003D,(uint8_t *)&st_net_set_data_cklcd_w2.w_003D,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003E,0x003E,(uint8_t *)&st_net_set_data_cklcd_w2.w_003E,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003F,0x003F,(uint8_t *)&st_net_set_data_cklcd_w2.w_003F,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//14

    {0x0040,0x0040,(uint8_t *)&st_net_set_data_cklcd_w2.w_0040,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0041,0x0041,(uint8_t *)&st_net_set_data_cklcd_w2.dw_0041,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0042,0x0042,(uint8_t *)&st_net_set_data_cklcd_w2.dw_0042,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0043,0x0043,(uint8_t *)&st_net_set_data_cklcd_w2.w_0043,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0044,0x0044,(uint8_t *)&st_net_set_data_cklcd_w2.w_0044,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0045,0x0045,(uint8_t *)&st_net_set_data_cklcd_w2.w_0045,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0046,0x0046,(uint8_t *)&st_net_set_data_cklcd_w2.w_0046,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0047,0x0047,(uint8_t *)&st_net_set_data_cklcd_w2.w_0047,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0048,0x0048,(uint8_t *)&st_net_set_data_cklcd_w2.w_0048,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0049,0x0049,(uint8_t *)&st_net_set_data_cklcd_w2.w_0049,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004A,0x004A,(uint8_t *)&st_net_set_data_cklcd_w2.w_004A,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004B,0x004B,(uint8_t *)&st_net_set_data_cklcd_w2.w_004B,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004C,0x004C,(uint8_t *)&st_net_set_data_cklcd_w2.w_004C,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004D,0x004D,(uint8_t *)&st_net_set_data_cklcd_w2.w_004D,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004E,0x004E,(uint8_t *)&st_net_set_data_cklcd_w2.w_004E,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004F,0x004F,(uint8_t *)&st_net_set_data_cklcd_w2.w_004F,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//14

    {0x0050,0x0050,(uint8_t *)&st_net_set_data_cklcd_w2.w_0050,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0051,0x0051,(uint8_t *)&st_net_set_data_cklcd_w2.w_0051,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0052,0x0052,(uint8_t *)&st_net_set_data_cklcd_w2.w_0052,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0053,0x0053,(uint8_t *)&st_net_set_data_cklcd_w2.w_0053,&st_net_data_att_rw_2_2b_min_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0054,0x0054,(uint8_t *)&st_net_set_data_cklcd_w2.w_0054,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0055,0x0055,(uint8_t *)&st_net_set_data_cklcd_w2.w_0055,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0056,0x0056,(uint8_t *)&st_net_set_data_cklcd_w2.w_0056,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0057,0x0057,(uint8_t *)&st_net_set_data_cklcd_w2.w_0057,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0058,0x0058,(uint8_t *)&st_net_set_data_cklcd_w2.w_0058,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0059,0x0059,(uint8_t *)&st_net_set_data_cklcd_w2.w_0059,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x005A,0x005A,(uint8_t *)&st_net_set_data_cklcd_w2.w_005A,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x005B,0x005B,(uint8_t *)&st_net_set_data_cklcd_w2.w_005B,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x005C,0x005C,(uint8_t *)&st_net_set_data_cklcd_w2.w_005C,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x005D,0x005D,(uint8_t *)&st_net_set_data_cklcd_w2.w_005D,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x005E,0x005E,(uint8_t *)&st_net_set_data_cklcd_w2.w_005E,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x005F,0x005F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//12

    {0x0060,0x0060,(uint8_t *)&st_net_set_data_cklcd_w2.w_0060,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0061,0x0061,(uint8_t *)&st_net_set_data_cklcd_w2.w_0061,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//2
    {0x0062,0x0062,(uint8_t *)&st_net_set_data_cklcd_w2.w_0062,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0063,0x0063,(uint8_t *)&st_net_set_data_cklcd_w2.w_0063,&st_net_data_att_rw_2_2b_w_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0064,0x0064,(uint8_t *)&st_net_set_data_cklcd_w2.w_0064,&st_net_data_att_rw_2_2b_w_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0065,0x0065,(uint8_t *)&st_net_set_data_cklcd_w2.w_0065,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0066,0x0066,(uint8_t *)&st_net_set_data_cklcd_w2.w_0066,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0067,0x0067,(uint8_t *)&st_net_set_data_cklcd_w2.dw_0067,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0068,0x0068,(uint8_t *)&st_net_set_data_cklcd_w2.dw_0068,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0069,0x0069,(uint8_t *)&st_net_set_data_cklcd_w2.w_0069,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006A,0x006A,(uint8_t *)&st_net_set_data_cklcd_w2.w_006A,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006B,0x006B,(uint8_t *)&st_net_set_data_cklcd_w2.w_006B,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006C,0x006C,(uint8_t *)&st_net_set_data_cklcd_w2.w_006C,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006D,0x006D,(uint8_t *)&st_net_set_data_cklcd_w2.w_006D,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006E,0x006E,(uint8_t *)&st_net_set_data_cklcd_w2.w_006E,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006F,0x006F,(uint8_t *)&st_net_set_data_cklcd_w2.w_006F,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},

    {0x0070,0x0070,(uint8_t *)&st_net_set_data_cklcd_w2.dw_0070,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0071,0x0071,(uint8_t *)&st_net_set_data_cklcd_w2.dw_0071,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0072,0x0072,(uint8_t *)&st_net_set_data_cklcd_w2.w_0072,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},//3
	{0x0073,0x0073,(uint8_t *)&st_net_set_data_cklcd_w2.w_0073,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x0074,0x0074,(uint8_t *)&st_net_set_data_cklcd_w2.w_0074,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},

//    {0x0075,0x0005,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0076,0x0006,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0077,0x0007,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0078,0x0008,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0079,0x0009,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007A,0x000A,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007B,0x000B,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007C,0x000C,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007D,0x000D,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007E,0x000E,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007F,0x000F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},

    {0x0080,0x0080,(uint8_t *)&st_net_set_data_cklcd_w2.ba_0080,&st_net_data_att_r_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},//1
//    {0x0090,0x0090,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},
//    {0x00A0,0x00A0,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},
//    {0x00B0,0x00B0,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},
//    {0x00C0,0x00C0,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},

    {0x00D0,0x00D0,(uint8_t *)&st_net_set_data_cklcd_w2.w_00D0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D1,0x00D1,(uint8_t *)&st_net_set_data_cklcd_w2.w_00D1,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D2,0x00D2,(uint8_t *)&st_net_set_data_cklcd_w2.w_00D2,&st_net_data_att_rw_2_2b_h_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x00D3,0x00D3,(uint8_t *)&st_net_set_data_cklcd_w2.w_00D3,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x00D4,0x00D4,(uint8_t *)&st_net_set_data_cklcd_w2.w_00D4,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D5,0x00D5,(uint8_t *)&st_net_set_data_cklcd_w2.dw_00D5,&st_net_data_att_rw_4_4b_1_set,&st_device_data_att_rw_4_cklcd_para,},
//    {0x00D6,0x00D6,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D7,0x00D7,(uint8_t *)&st_net_set_data_cklcd_w2.w_00D7,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D8,0x00D8,(uint8_t *)&st_net_set_data_cklcd_w2.w_00D8,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D9,0x00D9,(uint8_t *)&st_net_set_data_cklcd_w2.w_00D9,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DA,0x00DA,(uint8_t *)&st_net_set_data_cklcd_w2.w_00DA,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_r_2_cklcd_para,},
    {0x00DB,0x00DB,(uint8_t *)&st_net_set_data_cklcd_w2.w_00DB,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DC,0x00DC,(uint8_t *)&st_net_set_data_cklcd_w2.w_00DC,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DD,0x00DD,(uint8_t *)&st_net_set_data_cklcd_w2.w_00DD,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DE,0x00DE,(uint8_t *)&st_net_set_data_cklcd_w2.w_00DE,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DF,0x00DF,(uint8_t *)&st_net_set_data_cklcd_w2.w_00DF,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//15

    {0x00E0,0x00E0,(uint8_t *)&st_net_set_data_cklcd_w2.w_00E0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E1,0x00E1,(uint8_t *)&st_net_set_data_cklcd_w2.w_00E1,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E2,0x00E2,(uint8_t *)&st_net_set_data_cklcd_w2.w_00E2,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E3,0x00E3,(uint8_t *)&st_net_set_data_cklcd_w2.w_00E3,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E4,0x00E4,(uint8_t *)&st_net_set_data_cklcd_w2.w_00E4,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E5,0x00E5,(uint8_t *)&st_net_set_data_cklcd_w2.w_00E5,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E6,0x00E6,(uint8_t *)&st_net_set_data_cklcd_w2.w_00E6,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E7,0x00E7,(uint8_t *)&st_net_set_data_cklcd_w2.w_00E7,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E8,0x00E8,(uint8_t *)&st_net_set_data_cklcd_w2.w_00E8,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E9,0x00E9,(uint8_t *)&st_net_set_data_cklcd_w2.w_00E9,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EA,0x00EA,(uint8_t *)&st_net_set_data_cklcd_w2.w_00EA,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EB,0x00EB,(uint8_t *)&st_net_set_data_cklcd_w2.w_00EB,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EC,0x00EC,(uint8_t *)&st_net_set_data_cklcd_w2.w_00EC,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00ED,0x00ED,(uint8_t *)&st_net_set_data_cklcd_w2.w_00ED,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EE,0x00EE,(uint8_t *)&st_net_set_data_cklcd_w2.w_00EE,&st_net_data_att_rw_2_2b_w_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x00EF,0x000F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//15
};
//net_data_info st_net_control_data_info_cklcd_w2[cw_net_control_data_number_cklcd] =
//{
//    {0x03E8,0x03E8,0,&st_net_data_att_rw_2_2b_1_control,&st_device_data_att_w_2_external_trig},
//};
/************************** CKLCD 9 *****************************/
static net_status_data_cklcd st_net_status_data_cklcd_w3 =
{
    0,
};
net_data_info st_net_status_data_info_cklcd_w3[cw_net_status_data_number_cklcd] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_mppt_w3.w_0100,&st_net_data_att_r_2_2b_1_status,},
    {0x0101,0x0000,(uint8_t *)&st_net_status_data_mppt_w3.w_0101,&st_net_data_att_r_2_2b_w_status,},
    {0x0102,0x0000,(uint8_t *)&st_net_status_data_mppt_w3.w_0102,&st_net_data_att_r_2_2b_0_0_1_ua_status,},
    {0x0103,0x0000,(uint8_t *)&st_net_status_data_mppt_w3.w_0103,&st_net_data_att_r_2_2b_0_1_kwh_status,},
    {0x0104,0x0000,(uint8_t *)&b_reserve,&st_net_data_att_r_1_1b_1_status,},
    {0x0105,0x0000,(uint8_t *)&st_net_status_data_cklcd_w3.w_1005,&st_net_data_att_r_2_2b_w_status,},
    {0x0106,0x0000,(uint8_t *)&st_net_status_data_cklcd_w3.w_1006,&st_net_data_att_r_2_2b_0_1_a_status,},
};
static net_set_data_cklcd st_net_set_data_cklcd_w3 = {0,};
const net_data_info st_net_set_data_info_cklcd_w3[cw_net_set_data_number_cklcd] =
{
    {0x0000,0x0000,(uint8_t *)&st_net_set_data_cklcd_w3.w_0000,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0001,0x0001,(uint8_t *)&st_net_set_data_cklcd_w3.w_0001,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0002,0x0002,(uint8_t *)&st_net_set_data_cklcd_w3.w_0002,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0003,0x0003,(uint8_t *)&st_net_set_data_cklcd_w3.w_0003,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0004,0x0004,(uint8_t *)&st_net_set_data_cklcd_w3.w_0004,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x0005,0x0005,(uint8_t *)&st_net_set_data_cklcd_w3.w_0005,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x0006,0x0006,(uint8_t *)&st_net_set_data_cklcd_w3.w_0006,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0007,0x0007,(uint8_t *)&st_net_set_data_cklcd_w3.w_0007,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0008,0x0008,(uint8_t *)&st_net_set_data_cklcd_w3.w_0008,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0009,0x0009,(uint8_t *)&st_net_set_data_cklcd_w3.w_0009,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x000A,0x000A,(uint8_t *)&st_net_set_data_cklcd_w3.w_000A,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000B,0x000B,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000C,0x000C,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000D,0x000D,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000E,0x000E,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x000F,0x000F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//5

    {0x0010,0x0010,(uint8_t *)&st_net_set_data_cklcd_w3.w_0010,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0011,0x0011,(uint8_t *)&st_net_set_data_cklcd_w3.w_0011,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0012,0x0012,(uint8_t *)&st_net_set_data_cklcd_w3.w_0012,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0013,0x0013,(uint8_t *)&st_net_set_data_cklcd_w3.w_0013,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0014,0x0014,(uint8_t *)&st_net_set_data_cklcd_w3.w_0014,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0015,0x0015,(uint8_t *)&st_net_set_data_cklcd_w3.dw_0015,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0016,0x0016,(uint8_t *)&st_net_set_data_cklcd_w3.dw_0016,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0017,0x0017,(uint8_t *)&st_net_set_data_cklcd_w3.w_0017,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0018,0x0018,(uint8_t *)&st_net_set_data_cklcd_w3.w_0018,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0019,0x0019,(uint8_t *)&st_net_set_data_cklcd_w3.w_0019,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001A,0x001A,(uint8_t *)&st_net_set_data_cklcd_w3.w_001A,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001B,0x001B,(uint8_t *)&st_net_set_data_cklcd_w3.w_001B,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001C,0x001C,(uint8_t *)&st_net_set_data_cklcd_w3.w_001C,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001D,0x001D,(uint8_t *)&st_net_set_data_cklcd_w3.w_001D,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001E,0x001E,(uint8_t *)&st_net_set_data_cklcd_w3.w_001E,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x001F,0x001F,(uint8_t *)&st_net_set_data_cklcd_w3.w_001F,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},//16

    {0x0020,0x0020,(uint8_t *)&st_net_set_data_cklcd_w3.w_0020,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0021,0x0021,(uint8_t *)&st_net_set_data_cklcd_w3.w_0021,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0022,0x0022,(uint8_t *)&st_net_set_data_cklcd_w3.w_0022,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0023,0x0023,(uint8_t *)&st_net_set_data_cklcd_w3.w_0023,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0024,0x0024,(uint8_t *)&st_net_set_data_cklcd_w3.w_0024,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0025,0x0025,(uint8_t *)&st_net_set_data_cklcd_w3.w_0025,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0026,0x0026,(uint8_t *)&st_net_set_data_cklcd_w3.w_0026,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0027,0x0027,(uint8_t *)&st_net_set_data_cklcd_w3.w_0027,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0028,0x0028,(uint8_t *)&st_net_set_data_cklcd_w3.w_0028,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0029,0x0029,(uint8_t *)&st_net_set_data_cklcd_w3.w_0029,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002A,0x002A,(uint8_t *)&st_net_set_data_cklcd_w3.dw_002A,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002B,0x002B,(uint8_t *)&st_net_set_data_cklcd_w3.dw_002B,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002C,0x002C,(uint8_t *)&st_net_set_data_cklcd_w3.w_002C,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002D,0x002D,(uint8_t *)&st_net_set_data_cklcd_w3.w_002D,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002E,0x002E,(uint8_t *)&st_net_set_data_cklcd_w3.w_002E,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x002F,0x002F,(uint8_t *)&st_net_set_data_cklcd_w3.w_002F,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},//16

    {0x0030,0x0030,(uint8_t *)&st_net_set_data_cklcd_w3.w_0030,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0031,0x0031,(uint8_t *)&st_net_set_data_cklcd_w3.w_0031,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0032,0x0032,(uint8_t *)&st_net_set_data_cklcd_w3.w_0032,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0033,0x0033,(uint8_t *)&st_net_set_data_cklcd_w3.w_0033,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0034,0x0034,(uint8_t *)&st_net_set_data_cklcd_w3.w_0034,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0035,0x0035,(uint8_t *)&st_net_set_data_cklcd_w3.w_0035,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0036,0x0036,(uint8_t *)&st_net_set_data_cklcd_w3.w_0036,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0037,0x0037,(uint8_t *)&st_net_set_data_cklcd_w3.w_0037,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0038,0x0038,(uint8_t *)&st_net_set_data_cklcd_w3.w_0038,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0039,0x0039,(uint8_t *)&st_net_set_data_cklcd_w3.w_0039,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003A,0x003A,(uint8_t *)&st_net_set_data_cklcd_w3.w_003A,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003B,0x003B,(uint8_t *)&st_net_set_data_cklcd_w3.w_003B,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003C,0x003C,(uint8_t *)&st_net_set_data_cklcd_w3.w_003C,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003D,0x003D,(uint8_t *)&st_net_set_data_cklcd_w3.w_003D,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003E,0x003E,(uint8_t *)&st_net_set_data_cklcd_w3.w_003E,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x003F,0x003F,(uint8_t *)&st_net_set_data_cklcd_w3.w_003F,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//14

    {0x0040,0x0040,(uint8_t *)&st_net_set_data_cklcd_w3.w_0040,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0041,0x0041,(uint8_t *)&st_net_set_data_cklcd_w3.dw_0041,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0042,0x0042,(uint8_t *)&st_net_set_data_cklcd_w3.dw_0042,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0043,0x0043,(uint8_t *)&st_net_set_data_cklcd_w3.w_0043,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0044,0x0044,(uint8_t *)&st_net_set_data_cklcd_w3.w_0044,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0045,0x0045,(uint8_t *)&st_net_set_data_cklcd_w3.w_0045,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0046,0x0046,(uint8_t *)&st_net_set_data_cklcd_w3.w_0046,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0047,0x0047,(uint8_t *)&st_net_set_data_cklcd_w3.w_0047,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0048,0x0048,(uint8_t *)&st_net_set_data_cklcd_w3.w_0048,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0049,0x0049,(uint8_t *)&st_net_set_data_cklcd_w3.w_0049,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004A,0x004A,(uint8_t *)&st_net_set_data_cklcd_w3.w_004A,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004B,0x004B,(uint8_t *)&st_net_set_data_cklcd_w3.w_004B,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004C,0x004C,(uint8_t *)&st_net_set_data_cklcd_w3.w_004C,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004D,0x004D,(uint8_t *)&st_net_set_data_cklcd_w3.w_004D,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004E,0x004E,(uint8_t *)&st_net_set_data_cklcd_w3.w_004E,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x004F,0x004F,(uint8_t *)&st_net_set_data_cklcd_w3.w_004F,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//14

    {0x0050,0x0050,(uint8_t *)&st_net_set_data_cklcd_w3.w_0050,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0051,0x0051,(uint8_t *)&st_net_set_data_cklcd_w3.w_0051,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0052,0x0052,(uint8_t *)&st_net_set_data_cklcd_w3.w_0052,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0053,0x0053,(uint8_t *)&st_net_set_data_cklcd_w3.w_0053,&st_net_data_att_rw_2_2b_min_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0054,0x0054,(uint8_t *)&st_net_set_data_cklcd_w3.w_0054,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0055,0x0055,(uint8_t *)&st_net_set_data_cklcd_w3.w_0055,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0056,0x0056,(uint8_t *)&st_net_set_data_cklcd_w3.w_0056,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0057,0x0057,(uint8_t *)&st_net_set_data_cklcd_w3.w_0057,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0058,0x0058,(uint8_t *)&st_net_set_data_cklcd_w3.w_0058,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0059,0x0059,(uint8_t *)&st_net_set_data_cklcd_w3.w_0059,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x005A,0x005A,(uint8_t *)&st_net_set_data_cklcd_w3.w_005A,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x005B,0x005B,(uint8_t *)&st_net_set_data_cklcd_w3.w_005B,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x005C,0x005C,(uint8_t *)&st_net_set_data_cklcd_w3.w_005C,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x005D,0x005D,(uint8_t *)&st_net_set_data_cklcd_w3.w_005D,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x005E,0x005E,(uint8_t *)&st_net_set_data_cklcd_w3.w_005E,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x005F,0x005F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//12

    {0x0060,0x0060,(uint8_t *)&st_net_set_data_cklcd_w3.w_0060,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0061,0x0061,(uint8_t *)&st_net_set_data_cklcd_w3.w_0061,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//2
    {0x0062,0x0062,(uint8_t *)&st_net_set_data_cklcd_w3.w_0062,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0063,0x0063,(uint8_t *)&st_net_set_data_cklcd_w3.w_0063,&st_net_data_att_rw_2_2b_w_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0064,0x0064,(uint8_t *)&st_net_set_data_cklcd_w3.w_0064,&st_net_data_att_rw_2_2b_w_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0065,0x0065,(uint8_t *)&st_net_set_data_cklcd_w3.w_0065,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0066,0x0066,(uint8_t *)&st_net_set_data_cklcd_w3.w_0066,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0067,0x0067,(uint8_t *)&st_net_set_data_cklcd_w3.dw_0067,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0068,0x0068,(uint8_t *)&st_net_set_data_cklcd_w3.dw_0068,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0069,0x0069,(uint8_t *)&st_net_set_data_cklcd_w3.w_0069,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006A,0x006A,(uint8_t *)&st_net_set_data_cklcd_w3.w_006A,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006B,0x006B,(uint8_t *)&st_net_set_data_cklcd_w3.w_006B,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006C,0x006C,(uint8_t *)&st_net_set_data_cklcd_w3.w_006C,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006D,0x006D,(uint8_t *)&st_net_set_data_cklcd_w3.w_006D,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006E,0x006E,(uint8_t *)&st_net_set_data_cklcd_w3.w_006E,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x006F,0x006F,(uint8_t *)&st_net_set_data_cklcd_w3.w_006F,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},

    {0x0070,0x0070,(uint8_t *)&st_net_set_data_cklcd_w3.dw_0070,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0071,0x0071,(uint8_t *)&st_net_set_data_cklcd_w3.dw_0071,&st_net_data_att_rw_4_4b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x0072,0x0072,(uint8_t *)&st_net_set_data_cklcd_w3.w_0072,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},//3
	{0x0073,0x0073,(uint8_t *)&st_net_set_data_cklcd_w3.w_0073,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},
	{0x0074,0x0074,(uint8_t *)&st_net_set_data_cklcd_w3.w_0074,&st_net_data_att_rw_2_2b_s_set,&st_device_data_att_rw_2_cklcd_para,},

//    {0x0075,0x0005,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0076,0x0006,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0077,0x0007,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0078,0x0008,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x0079,0x0009,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007A,0x000A,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007B,0x000B,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007C,0x000C,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007D,0x000D,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007E,0x000E,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x007F,0x000F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},

    {0x0080,0x0080,(uint8_t *)&st_net_set_data_cklcd_w3.ba_0080,&st_net_data_att_r_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},//1
//    {0x0090,0x0090,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},
//    {0x00A0,0x00A0,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},
//    {0x00B0,0x00B0,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},
//    {0x00C0,0x00C0,0,&st_net_data_att_rw_15_1b_1_set,&st_device_data_att_r_15_cklcd_para,},

    {0x00D0,0x00D0,(uint8_t *)&st_net_set_data_cklcd_w3.w_00D0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D1,0x00D1,(uint8_t *)&st_net_set_data_cklcd_w3.w_00D1,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D2,0x00D2,(uint8_t *)&st_net_set_data_cklcd_w3.w_00D2,&st_net_data_att_rw_2_2b_h_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x00D3,0x00D3,(uint8_t *)&st_net_set_data_cklcd_w3.w_00D3,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x00D4,0x00D4,(uint8_t *)&st_net_set_data_cklcd_w3.w_00D4,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D5,0x00D5,(uint8_t *)&st_net_set_data_cklcd_w3.dw_00D5,&st_net_data_att_rw_4_4b_1_set,&st_device_data_att_rw_4_cklcd_para,},
//    {0x00D6,0x00D6,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D7,0x00D7,(uint8_t *)&st_net_set_data_cklcd_w3.w_00D7,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D8,0x00D8,(uint8_t *)&st_net_set_data_cklcd_w3.w_00D8,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00D9,0x00D9,(uint8_t *)&st_net_set_data_cklcd_w3.w_00D9,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DA,0x00DA,(uint8_t *)&st_net_set_data_cklcd_w3.w_00DA,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_r_2_cklcd_para,},
    {0x00DB,0x00DB,(uint8_t *)&st_net_set_data_cklcd_w3.w_00DB,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DC,0x00DC,(uint8_t *)&st_net_set_data_cklcd_w3.w_00DC,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DD,0x00DD,(uint8_t *)&st_net_set_data_cklcd_w3.w_00DD,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DE,0x00DE,(uint8_t *)&st_net_set_data_cklcd_w3.w_00DE,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00DF,0x00DF,(uint8_t *)&st_net_set_data_cklcd_w3.w_00DF,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//15

    {0x00E0,0x00E0,(uint8_t *)&st_net_set_data_cklcd_w3.w_00E0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E1,0x00E1,(uint8_t *)&st_net_set_data_cklcd_w3.w_00E1,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E2,0x00E2,(uint8_t *)&st_net_set_data_cklcd_w3.w_00E2,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E3,0x00E3,(uint8_t *)&st_net_set_data_cklcd_w3.w_00E3,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E4,0x00E4,(uint8_t *)&st_net_set_data_cklcd_w3.w_00E4,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E5,0x00E5,(uint8_t *)&st_net_set_data_cklcd_w3.w_00E5,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E6,0x00E6,(uint8_t *)&st_net_set_data_cklcd_w3.w_00E6,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E7,0x00E7,(uint8_t *)&st_net_set_data_cklcd_w3.w_00E7,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E8,0x00E8,(uint8_t *)&st_net_set_data_cklcd_w3.w_00E8,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00E9,0x00E9,(uint8_t *)&st_net_set_data_cklcd_w3.w_00E9,&st_net_data_att_rw_2_2b_a_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EA,0x00EA,(uint8_t *)&st_net_set_data_cklcd_w3.w_00EA,&st_net_data_att_rw_2_2b_mv_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EB,0x00EB,(uint8_t *)&st_net_set_data_cklcd_w3.w_00EB,&st_net_data_att_rw_2_2b_per_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EC,0x00EC,(uint8_t *)&st_net_set_data_cklcd_w3.w_00EC,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00ED,0x00ED,(uint8_t *)&st_net_set_data_cklcd_w3.w_00ED,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},
    {0x00EE,0x00EE,(uint8_t *)&st_net_set_data_cklcd_w3.w_00EE,&st_net_data_att_rw_2_2b_w_set,&st_device_data_att_rw_2_cklcd_para,},
//    {0x00EF,0x000F,0,&st_net_data_att_rw_2_2b_1_set,&st_device_data_att_rw_2_cklcd_para,},//15
};
//net_data_info st_net_control_data_info_cklcd_w3[cw_net_control_data_number_cklcd] =
//{
//    {0x03E8,0x03E8,0,&st_net_data_att_rw_2_2b_1_control,&st_device_data_att_w_2_external_trig},
//};

///////////////////////////////////////////////////////////////////////////////////////
const net_data_list st_net_data_list_cklcd_u1[cw_net_data_list_length_cklcd] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_cklcd,
        &st_net_status_data_info_cklcd_u1[0],
    },
	{
		cb_net_data_att_set,
		cw_net_set_data_number_cklcd,
		&st_net_set_data_info_cklcd_u1[0],
	},
//	{
//		cb_net_data_att_control,
//		cw_net_control_data_number_cklcd,
//		&st_net_control_data_info_cklcd_u1[0],
//	}
};

const net_data_list st_net_data_list_cklcd_u2[cw_net_data_list_length_cklcd] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_cklcd,
        &st_net_status_data_info_cklcd_u2[0],
    },
	{
		cb_net_data_att_set,
		cw_net_set_data_number_cklcd,
		&st_net_set_data_info_cklcd_u2[0],
	},
//	{
//		cb_net_data_att_control,
//		cw_net_control_data_number_cklcd,
//		&st_net_control_data_info_cklcd_u2[0],
//	}
};

const net_data_list st_net_data_list_cklcd_u3[cw_net_data_list_length_cklcd] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_cklcd,
        &st_net_status_data_info_cklcd_u3[0],
    },
	{
		cb_net_data_att_set,
		cw_net_set_data_number_cklcd,
		&st_net_set_data_info_cklcd_u3[0],
	},
//	{
//		cb_net_data_att_control,
//		cw_net_control_data_number_cklcd,
//		&st_net_control_data_info_cklcd_u3[0],
//	}
};

const net_data_list st_net_data_list_cklcd_v1[cw_net_data_list_length_cklcd] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_cklcd,
        &st_net_status_data_info_cklcd_v1[0],
    },
	{
		cb_net_data_att_set,
		cw_net_set_data_number_cklcd,
		&st_net_set_data_info_cklcd_v1[0],
	},
//	{
//		cb_net_data_att_control,
//		cw_net_control_data_number_cklcd,
//		&st_net_control_data_info_cklcd_v1[0],
//	}
};

const net_data_list st_net_data_list_cklcd_v2[cw_net_data_list_length_cklcd] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_cklcd,
        &st_net_status_data_info_cklcd_v2[0],
    },
	{
		cb_net_data_att_set,
		cw_net_set_data_number_cklcd,
		&st_net_set_data_info_cklcd_v2[0],
	},
//	{
//		cb_net_data_att_control,
//		cw_net_control_data_number_cklcd,
//		&st_net_control_data_info_cklcd_v2[0],
//	}
};

const net_data_list st_net_data_list_cklcd_v3[cw_net_data_list_length_cklcd] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_cklcd,
        &st_net_status_data_info_cklcd_v3[0],
    },
	{
		cb_net_data_att_set,
		cw_net_set_data_number_cklcd,
		&st_net_set_data_info_cklcd_v3[0],
	},
//	{
//		cb_net_data_att_control,
//		cw_net_control_data_number_cklcd,
//		&st_net_control_data_info_cklcd_v3[0],
//	}
};

const net_data_list st_net_data_list_cklcd_w1[cw_net_data_list_length_cklcd] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_cklcd,
        &st_net_status_data_info_cklcd_w1[0],
    },
	{
		cb_net_data_att_set,
		cw_net_set_data_number_cklcd,
		&st_net_set_data_info_cklcd_w1[0],
	},
//	{
//		cb_net_data_att_control,
//		cw_net_control_data_number_cklcd,
//		&st_net_control_data_info_cklcd_w1[0],
//	}
};

const net_data_list st_net_data_list_cklcd_w2[cw_net_data_list_length_cklcd] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_cklcd,
        &st_net_status_data_info_cklcd_w2[0],
    },
	{
		cb_net_data_att_set,
		cw_net_set_data_number_cklcd,
		&st_net_set_data_info_cklcd_w2[0],
	},
//	{
//		cb_net_data_att_control,
//		cw_net_control_data_number_cklcd,
//		&st_net_control_data_info_cklcd_w2[0],
//	}
};

const net_data_list st_net_data_list_cklcd_w3[cw_net_data_list_length_cklcd] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_cklcd,
        &st_net_status_data_info_cklcd_w3[0],
    },
	{
		cb_net_data_att_set,
		cw_net_set_data_number_cklcd,
		&st_net_set_data_info_cklcd_w3[0],
	},
//	{
//		cb_net_data_att_control,
//		cw_net_control_data_number_cklcd,
//		&st_net_control_data_info_cklcd_w3[0],
//	}
};

device_inquire_list st_device_inquire_list_cklcd[cw_device_inquire_list_cklcd] =
{
    {cw_device_cmd_cklcd_para_read,{0x00,0x00,0x00,0x0A},4,TRUE},
    {cw_device_cmd_cklcd_para_read,{0x00,0x10,0x00,0x4D},4,TRUE},
    {cw_device_cmd_cklcd_para_read,{0x00,0x60,0x00,0x10},4,TRUE},
    {cw_device_cmd_cklcd_para_read,{0x00,0x70,0x00,0x05},4,TRUE},
    {cw_device_cmd_cklcd_para_read,{0x00,0x80,0x00,0x0E},4,TRUE},
    {cw_device_cmd_cklcd_para_read,{0x00,0xD0,0x00,0x03},4,TRUE},
    {cw_device_cmd_cklcd_para_read,{0x00,0xD5,0x00,0x02},4,TRUE},
    {cw_device_cmd_cklcd_para_read,{0x00,0xDB,0x00,0x02},4,TRUE},
    {cw_device_cmd_cklcd_para_read,{0x00,0xDD,0x00,0x12},4,TRUE},
//    {cw_device_cmd_event_log_read ,{0x00,0x01,0x00,0x01},4,TRUE},
//    {cw_device_cmd_operate_log_read,{0x00,0x01,0x00,0x01},4,TRUE},
//    {cw_device_cmd_cklcd_para_read,{0x00,0xD0,0x00,0x0C},4,TRUE},
};
	
//device_inquire_list st_device_inquire_list_cklcd[cw_device_inquire_list_cklcd] =
//{
//    {cw_device_cmd_cklcd_para_read,{0x00,0x00,0x00,0x5D},4,TRUE},
//    {cw_device_cmd_cklcd_para_read,{0x00,0x60,0x00,0x15},4,TRUE},
//    {cw_device_cmd_cklcd_para_read,{0x00,0x80,0x00,0x5C},4,TRUE},
//    {cw_device_cmd_cklcd_para_read,{0x00,0xDD,0x00,0x12},4,TRUE},
//};

//////////////////////////////////////////////////////////////////////////////////////////
const net_data_list_info st_net_data_list_info_cklcd_u1 =
{
    cw_net_data_list_length_cklcd,
    &st_net_data_list_cklcd_u1[0],
    cw_device_inquire_list_cklcd,
    &st_device_inquire_list_cklcd[0],
};


const net_data_list_info st_net_data_list_info_cklcd_u2 =
{
    cw_net_data_list_length_cklcd,
    &st_net_data_list_cklcd_u2[0],
    cw_device_inquire_list_cklcd,
    &st_device_inquire_list_cklcd[0],
};

const net_data_list_info st_net_data_list_info_cklcd_u3 =
{
    cw_net_data_list_length_cklcd,
    &st_net_data_list_cklcd_u3[0],
    cw_device_inquire_list_cklcd,
    &st_device_inquire_list_cklcd[0],
};

const net_data_list_info st_net_data_list_info_cklcd_v1 =
{
    cw_net_data_list_length_cklcd,
    &st_net_data_list_cklcd_v1[0],
    cw_device_inquire_list_cklcd,
    &st_device_inquire_list_cklcd[0],
};

const net_data_list_info st_net_data_list_info_cklcd_v2 =
{
    cw_net_data_list_length_cklcd,
    &st_net_data_list_cklcd_v2[0],
    cw_device_inquire_list_cklcd,
    &st_device_inquire_list_cklcd[0],
};

const net_data_list_info st_net_data_list_info_cklcd_v3 =
{
    cw_net_data_list_length_cklcd,
    &st_net_data_list_cklcd_v3[0],
    cw_device_inquire_list_cklcd,
    &st_device_inquire_list_cklcd[0],
};

const net_data_list_info st_net_data_list_info_cklcd_w1 =
{
    cw_net_data_list_length_cklcd,
    &st_net_data_list_cklcd_w1[0],
    cw_device_inquire_list_cklcd,
    &st_device_inquire_list_cklcd[0],
};

const net_data_list_info st_net_data_list_info_cklcd_w2 =
{
    cw_net_data_list_length_cklcd,
    &st_net_data_list_cklcd_w2[0],
    cw_device_inquire_list_cklcd,
    &st_device_inquire_list_cklcd[0],
};

const net_data_list_info st_net_data_list_info_cklcd_w3 =
{
    cw_net_data_list_length_cklcd,
    &st_net_data_list_cklcd_w3[0],
    cw_device_inquire_list_cklcd,
    &st_device_inquire_list_cklcd[0],
};


