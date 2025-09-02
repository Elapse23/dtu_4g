#include "net_firmware_info_kinergy_ii.h"
#include "net_device_tree.h"
#include "net_device_tree_kinergy_ii.h"


#define cw_net_device_update_info_length_kinergy_ii 					5

net_device_update_info st_net_device_update_info_list_kinergy_ii_system[cw_net_device_update_info_length_kinergy_ii] =
{
	{
		.w_device_type = (cdw_device_id_kinergy_ii & 0xFFFF00) >> 8,
        .b_firmware_format = cb_firmware_format_pacx_mcu,
		.b_mcu_type = cb_N32G455RE,
		.pb_firmware_file = "kinergy_ii_firmware",
		.pb_firmware_file_info = "kinergy_ii_firmware_info",
	},
	{
		.w_device_type = (cdw_device_type_cklcd & 0xFFFF00) >> 8,
        .b_firmware_format = cb_firmware_format_pac,
		.b_mcu_type = cb_Z128,
		.pb_firmware_file = "cklcd_firmware",
		.pb_firmware_file_info = "cklcd_firmware_info",
	},
	{
		.w_device_type = (cdw_device_type_ck_inverter & 0xFFFF00) >> 8,
        .b_firmware_format = cb_firmware_format_pacx_dsp,
		.b_mcu_type = cb_Z128,
		.pb_firmware_file = "ck_inverter_firmware",
		.pb_firmware_file_info = "ck_inverter_firmware_info",
	},
	{
		.w_device_type = (cdw_device_type_mppt & 0xFFFF00) >> 8,
        .b_firmware_format = cb_firmware_format_pacx_dsp,
		.b_mcu_type = cb_Z128,
		.pb_firmware_file = "mppt_inverter_firmware",
		.pb_firmware_file_info = "mppt_inverter_firmware_info",
	},
	{
		.w_device_type = (cdw_device_type_mppt & 0xFFFF00) >> 8,
        .b_firmware_format = cb_firmware_format_pac,
		.b_mcu_type = cb_Z128,
		.pb_firmware_file = "mppt_firmware",
		.pb_firmware_file_info = "mppt_firmware_info",
	},
};

net_device_update_info_list st_net_device_update_info_list_kinergy_ii =
{
	cw_net_device_update_info_length_kinergy_ii,
	&st_net_device_update_info_list_kinergy_ii_system[0],
};

