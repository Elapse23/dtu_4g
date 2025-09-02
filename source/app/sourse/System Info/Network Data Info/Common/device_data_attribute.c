#include "device_data_attribute.h"

//const device_data_attribute st_device_data_att_2_reserve =
//{
//	2,
//	cw_device_cmd_reserve,
//};
/* CKLCD PARAMETER */
const device_data_attribute st_device_data_att_rw_2_cklcd_para =
{
	2,
	cw_device_cmd_cklcd_para_read,
    cw_device_cmd_cklcd_para_write,
};

const device_data_attribute st_device_data_att_rw_4_cklcd_para =
{
	4,
	cw_device_cmd_cklcd_para_read,
    cw_device_cmd_cklcd_para_write,
};

const device_data_attribute st_device_data_att_r_2_cklcd_para =
{
	2,
	cw_device_cmd_cklcd_para_read,
    cw_device_cmd_read_only,
};

const device_data_attribute st_device_data_att_r_15_cklcd_para =
{
	15,
	cw_device_cmd_cklcd_para_read,
    cw_device_cmd_read_only,
};

/* CK INVERTER DATA*/
const device_data_attribute st_device_data_att_r_2_ck_data =
{
	2,
	cw_device_cmd_ck_data_read,
    cw_device_cmd_read_only,
};
const device_data_attribute st_device_data_att_r_2_riiosunii_data =
{
	2,
	cw_device_cmd_riio_ii_data_read,
    cw_device_cmd_read_only,
};

const device_data_attribute st_device_data_att_r_2_riiosunii_splitphase_data =
{
    2,
    cw_device_cmd_riio_ii_split_phase_data_read,
    cw_device_cmd_read_only,
};

/* CK INVERTER PARAMETER*/
const device_data_attribute st_device_data_att_rw_2_ck_para =
{
	2,
	cw_device_cmd_ck_para_read,
    cw_device_cmd_ck_para_write,
};
/* MPPT DATA*/
const device_data_attribute st_device_data_att_r_2_mppt_data =
{
	2,
	cw_device_cmd_mppt_data_read,
    cw_device_cmd_read_only,
};
/* RIIO SUN II MPPT DATA*/
const device_data_attribute st_device_data_att_r_2_riio_mppt_data =
{
	2,
	cw_device_cmd_mppt_riio_ii_data_read,
    cw_device_cmd_read_only,
};
/* PV DATA */
const device_data_attribute st_device_data_att_r_2_pv_data =
{
	2,
	cw_device_cmd_pv_data_read,
    cw_device_cmd_read_only,
};
const device_data_attribute st_device_data_att_r_4_pv_data =
{
	4,
	cw_device_cmd_pv_data_read,
    cw_device_cmd_read_only,
};
/* RIIO SUN II METER DATA*/
const device_data_attribute st_device_data_att_r_2_riio_meter_data =
{
	2,
	cw_device_cmd_meter_data_read,
    cw_device_cmd_read_only,
};

/* Control */
const device_data_attribute st_device_data_att_w_2_external_trig =
{
	2,
	cw_device_cmd_write_only,
    cw_device_cmd_external_trig_write,
};
/*Inverter auxiliary installation command*/
const device_data_attribute st_device_data_att_w_30_auxiliary_installation =
{
	30,
	cw_device_cmd_write_only,
    cw_device_cmd_inv_auxiliary_installation_write,
};
    
const device_data_attribute st_device_data_att_w_30_mppt_auxiliary_installation =
{
    30,
    cw_device_cmd_write_only,
    cw_device_cmd_mppt_auxiliary_installation_write,
};

const device_data_attribute st_device_data_att_r_2_riiosunii_split_phase_meter_data =
{
	2,
	cw_device_cmd_riio_ii_split_phase_meter_data_read,
    cw_device_cmd_read_only,
};

