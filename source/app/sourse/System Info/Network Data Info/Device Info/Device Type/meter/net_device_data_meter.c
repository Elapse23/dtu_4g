#include "net_device_data_meter.h"
#include "system_info_general_macro.h"
//
#define cw_net_status_data_number_meter  		            26
#define cw_net_set_data_number_meter  		                0
#define cw_net_control_data_number_meter  	                0
#define cw_device_inquire_list_meter                        2
#define cw_net_data_list_length_meter    		            1

static net_status_data_meter st_net_status_data_meter =
{
    0,
};
const net_data_info st_net_status_data_info_meter_1[cw_net_status_data_number_meter] =
{
    {0x0110,0x0000,(uint8_t *)&st_net_status_data_meter.w_0110,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riio_meter_data,},
    {0x0111,0x0001,(uint8_t *)&st_net_status_data_meter.w_0111,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riio_meter_data,},
    {0x0112,0x0002,(uint8_t *)&st_net_status_data_meter.w_0112,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riio_meter_data,},
    {0x0113,0x0003,(uint8_t *)&st_net_status_data_meter.w_0113,&st_net_data_att_r_2_2b_1_0_uva_status,&st_device_data_att_r_2_riio_meter_data,},
    {0x0117,0x0004,(uint8_t *)&st_net_status_data_meter.w_0117,&st_net_data_att_r_2_2b_0_0_1hz_status,&st_device_data_att_r_2_riio_meter_data,},
    {0x0118,0x0005,(uint8_t *)&st_net_status_data_meter.w_0118,&st_net_data_att_r_2_2b_0_1_ukwh_status,&st_device_data_att_r_2_riio_meter_data,},
    {0x0119,0x0006,(uint8_t *)&st_net_status_data_meter.w_0119,&st_net_data_att_r_2_2b_0_1_ukwh_status,&st_device_data_att_r_2_riio_meter_data,},

	{0x0200,0x0000,(uint8_t *)&st_net_status_data_meter.w_0200,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riiosunii_split_phase_meter_data,},
	{0x0201,0x0001,(uint8_t *)&st_net_status_data_meter.w_0201,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riiosunii_split_phase_meter_data,},
	{0x0202,0x0002,(uint8_t *)&st_net_status_data_meter.w_0202,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riiosunii_split_phase_meter_data,},
	{0x0203,0x0003,(uint8_t *)&st_net_status_data_meter.w_0203,&st_net_data_att_r_2_2b_1_0_uva_status,&st_device_data_att_r_2_riiosunii_split_phase_meter_data,},
	{0x0204,0x0004,(uint8_t *)&st_net_status_data_meter.w_0204,&st_net_data_att_r_2_2b_0_0_1hz_status,&st_device_data_att_r_2_riiosunii_split_phase_meter_data,},
	{0x0205,0x0005,(uint8_t *)&st_net_status_data_meter.w_0205,&st_net_data_att_r_2_2b_0_1_ukwh_status,&st_device_data_att_r_2_riiosunii_split_phase_meter_data,},
	{0x0206,0x0006,(uint8_t *)&st_net_status_data_meter.w_0206,&st_net_data_att_r_2_2b_0_1_ukwh_status,&st_device_data_att_r_2_riiosunii_split_phase_meter_data,},
	{0x0207,0x0007,(uint8_t *)&st_net_status_data_meter.w_0207,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riiosunii_split_phase_meter_data,},
	{0x0208,0x0008,(uint8_t *)&st_net_status_data_meter.w_0208,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riiosunii_split_phase_meter_data,},
	{0x0209,0x0009,(uint8_t *)&st_net_status_data_meter.w_0209,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riiosunii_split_phase_meter_data,},
	{0x020A,0x000A,(uint8_t *)&st_net_status_data_meter.w_020A,&st_net_data_att_r_2_2b_1_0_uva_status,&st_device_data_att_r_2_riiosunii_split_phase_meter_data,},
	{0x020B,0x000B,(uint8_t *)&st_net_status_data_meter.w_020B,&st_net_data_att_r_2_2b_0_1_ukwh_status,&st_device_data_att_r_2_riiosunii_split_phase_meter_data,},
	{0x020C,0x000C,(uint8_t *)&st_net_status_data_meter.w_020C,&st_net_data_att_r_2_2b_0_1_ukwh_status,&st_device_data_att_r_2_riiosunii_split_phase_meter_data,},
	{0x020D,0x000D,(uint8_t *)&st_net_status_data_meter.w_020D,&st_net_data_att_r_2_2b_0_1_v_status,&st_device_data_att_r_2_riiosunii_split_phase_meter_data,},
	{0x020E,0x000E,(uint8_t *)&st_net_status_data_meter.w_020E,&st_net_data_att_r_2_2b_0_0_1_ua_status,&st_device_data_att_r_2_riiosunii_split_phase_meter_data,},
	{0x020F,0x000F,(uint8_t *)&st_net_status_data_meter.w_020F,&st_net_data_att_r_2_2b_w_status,&st_device_data_att_r_2_riiosunii_split_phase_meter_data,},
	{0x0210,0x0010,(uint8_t *)&st_net_status_data_meter.w_0210,&st_net_data_att_r_2_2b_1_0_uva_status,&st_device_data_att_r_2_riiosunii_split_phase_meter_data,},
	{0x0211,0x0011,(uint8_t *)&st_net_status_data_meter.w_0211,&st_net_data_att_r_2_2b_0_1_ukwh_status,&st_device_data_att_r_2_riiosunii_split_phase_meter_data,},
	{0x0212,0x0012,(uint8_t *)&st_net_status_data_meter.w_0212,&st_net_data_att_r_2_2b_0_1_ukwh_status,&st_device_data_att_r_2_riiosunii_split_phase_meter_data,},

};

/////////////////////////////////////////////////////////////////////////
const net_data_list st_net_data_list_meter_1[cw_net_data_list_length_meter] =
{
    {
        cb_net_data_att_status,
        cw_net_status_data_number_meter,
        &st_net_status_data_info_meter_1[0],
    },
//	{
//		cb_net_data_att_set,
//		cw_net_set_data_number_meter,
//		&st_net_set_data_info_meter_1[0],
//	},
//	{
//		cb_net_data_att_control,
//		cw_net_control_data_number_meter,
//		&st_net_control_data_info_meter_1[0],
//	}
};

//////////////////////////////////////////////////////////////////////////////
device_inquire_list st_device_inquire_list_meter[cw_device_inquire_list_meter] =
{
    {cw_device_cmd_meter_data_read,{0x00,},0,},
//    {cw_device_cmd_riio_ii_split_phase_meter_data_read,{0x00,},0},
    {cw_device_cmd_riio_ii_split_phase_meter_data_read,{0x00,0x00,0x00,0x12},4,TRUE},
};

//////////////////////////////////////////////////////////////////////////////
const net_data_list_info st_net_data_list_info_meter_1 =
{
    cw_net_data_list_length_meter,
    &st_net_data_list_meter_1[0],
    cw_device_inquire_list_meter,
    &st_device_inquire_list_meter[0],
};
