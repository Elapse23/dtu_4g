#include "net_device_data_attribute.h"

const net_data_attribute st_net_data_att_r_1_1b_1_status =
{
	cb_net_data_att_read,
	1,
	cb_net_data_att_1b,
	cw_UnitNULL,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_rw_1_1b_1_control =
{
	cb_net_data_att_read_write,
	1,
	cb_net_data_att_1b,
	cw_UnitNULL,
	cb_net_data_att_control,
};

const net_data_attribute st_net_data_att_r_100_1b_1_status =
{
	cb_net_data_att_read,
	100,
	cb_net_data_att_1b,
	cw_UnitNULL,
	cb_net_data_att_status,
};

const net_data_attribute st_net_data_att_r_2_1b_1_status =
{
	cb_net_data_att_read,
	2,
	cb_net_data_att_1b,
	cw_UnitNULL,
	cb_net_data_att_status,
};

const net_data_attribute st_net_data_att_r_2_2b_0_1_status =
{
	cb_net_data_att_read,
	2,
	cb_net_data_att_2b,
	cw_UnitNULL0_1,
	cb_net_data_att_status,
};

const net_data_attribute st_net_data_att_r_138_1b_1_status =
{
	cb_net_data_att_read,
	138,
	cb_net_data_att_1b,
	cw_UnitNULL,
	cb_net_data_att_status,
};

const net_data_attribute st_net_data_att_r_220_1b_1_status =
{
	cb_net_data_att_read,
	220,
	cb_net_data_att_1b,
	cw_UnitNULL,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_rw_1_1b_1_set =
{
	cb_net_data_att_read_write,
	1,
	cb_net_data_att_1b,
	cw_UnitNULL,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_rw_1_1b_1_0_m_set =
{
	cb_net_data_att_read_write,
	1,
	cb_net_data_att_1b,
	cw_UnSignedUnit1_0M,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_rw_1_1b_per_set =
{
	cb_net_data_att_read_write,
	1,
	cb_net_data_att_1b,
	cw_UnSignedUnit1_0Per,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_rw_2_2b_per_set =
{
	cb_net_data_att_read_write,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit1_0Per,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_rw_2_2b_0_1_ah_set =
{
	cb_net_data_att_read_write,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit0_1AH,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_rw_2_2b_1_0_ah_set =
{
	cb_net_data_att_read_write,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit1_0AH,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_rw_2_2b_0_0_1_v_set =
{
	cb_net_data_att_read_write,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit0_0_1V,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_rw_2_2b_0_0_1_c_set =
{
	cb_net_data_att_read_write,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit0_0_1C,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_rw_4_4b_1_set =
{
	cb_net_data_att_read_write,
	4,
	cb_net_data_att_4b,
	cw_UnitNULL,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_rw_4_1b_1_set =
{
	cb_net_data_att_read_write,
	4,
	cb_net_data_att_1b,
	cw_UnitNULL,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_rw_22_1b_1_set =
{
	cb_net_data_att_read_write,
	22,
	cb_net_data_att_1b,
	cw_UnitNULL,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_rw_30_1b_1_set =
{
	cb_net_data_att_read_write,
	30,
	cb_net_data_att_1b,
	cw_UnitNULL,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_rw_30_1b_asciistring_set =
{
	cb_net_data_att_read_write,
	30,
	cb_net_data_att_1b,
	cw_asciistring,
	cb_net_data_att_set,
};

const net_data_attribute st_net_data_att_rw_40_1b_1_set =
{
	cb_net_data_att_read_write,
	40,
	cb_net_data_att_1b,
	cw_UnitNULL,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_r_15_1b_1_set =
{
	cb_net_data_att_read,
	15,
	cb_net_data_att_1b,
	cw_UnitNULL,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_rw_2_2b_1_set =
{
	cb_net_data_att_read_write,
	2,
	cb_net_data_att_2b,
	cw_UnitNULL,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_rw_2_2b_1day_set =
{
	cb_net_data_att_read_write,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit1_0Day,
	cb_net_data_att_set,
};

const net_data_attribute st_net_data_att_rw_129_1b_1_set =
{
	cb_net_data_att_read_write,
	129,
	cb_net_data_att_1b,
	cw_UnitNULL,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_rw_2_2b_0_0_1hz_set =
{
	cb_net_data_att_read_write,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit0_0_1HZ,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_rw_2_2b_1_0hz_set =
{
	cb_net_data_att_read_write,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit1_0HZ,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_r_2_2b_0_0_1hz_status =
{
	cb_net_data_att_read,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit0_0_1HZ,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_rw_2_2b_1_0va_set =
{
	cb_net_data_att_read_write,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit1_0VA,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_r_2_2b_1_0_uva_status =
{
	cb_net_data_att_read,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit1_0VA,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_2_2b_1_0_va_status =
{
	cb_net_data_att_read,
	2,
	cb_net_data_att_2b,
	cw_SignedUnit1_0VA,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_4_4b_1_0_va_status =
{
	cb_net_data_att_read,
	4,
	cb_net_data_att_4b,
	cw_SignedUnit1_0VA,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_w_1_1b_1_control =
{
	cb_net_data_att_write,
	1,
	cb_net_data_att_1b,
	cw_UnitNULL,
	cb_net_data_att_control,
};
const net_data_attribute st_net_data_att_w_2_2b_1_control =
{
	cb_net_data_att_write,
	2,
	cb_net_data_att_2b,
	cw_UnitNULL,
	cb_net_data_att_control,
};
const net_data_attribute st_net_data_att_w_14_1b_1_control =
{
	cb_net_data_att_write,
	14,
	cb_net_data_att_1b,
	cw_UnitNULL,
	cb_net_data_att_control,
};
const net_data_attribute st_net_data_att_r_41_1b_1_control =
{
	cb_net_data_att_read_write,
	41,
	cb_net_data_att_1b,
	cw_UnitNULL,
	cb_net_data_att_control,
};
const net_data_attribute st_net_data_att_r_2_2b_1_status =
{
	cb_net_data_att_read,
	2,
	cb_net_data_att_2b,
	cw_UnitNULL,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_4_4b_1_status =
{
	cb_net_data_att_read,
	4,
	cb_net_data_att_4b,
	cw_UnitNULL,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_12_1b_1_status =
{
	cb_net_data_att_read,
	12,
	cb_net_data_att_1b,
	cw_UnitSTR,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_13_1b_1_status =
{
	cb_net_data_att_read,
	13,
	cb_net_data_att_1b,
	cw_UnitSTR,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_4_4b_0_1_m_status =
{
	cb_net_data_att_read,
	4,
	cb_net_data_att_4b,
	cw_UnSignedUnit0_1M,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_4_4b_1_m_status =
{
	cb_net_data_att_read,
	4,
	cb_net_data_att_4b,
	cw_UnSignedUnit1_0M,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_4_4b_0_1_mh_status =
{
	cb_net_data_att_read,
	4,
	cb_net_data_att_4b,
	cw_UnSignedUnit0_1MH,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_4_4b_1_mh_status =
{
	cb_net_data_att_read,
	4,
	cb_net_data_att_4b,
	cw_UnSignedUnit1_0MH,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_rw_1_1b_1_mh_set =
{
	cb_net_data_att_read_write,
	1,
	cb_net_data_att_1b,
	cw_UnSignedUnit1_0MH,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_r_1_1b_1_1_status =
{
	cb_net_data_att_read,
	1,
	cb_net_data_att_1b,
	cw_UnitNULL,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_6_1b_1_1_status =
{
	cb_net_data_att_read,
	6,
	cb_net_data_att_1b,
	cw_UnitNULL,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_rw_5_1b_1_set =
{
	cb_net_data_att_read_write,
	5,
	cb_net_data_att_1b,
	cw_UnitNULL,
	cb_net_data_att_set,
};

const net_data_attribute st_net_data_att_r_1_1b_per_status =
{
	cb_net_data_att_read,
	1,
	cb_net_data_att_1b,
	cw_UnSignedUnit1_0Per,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_1_1b_s1_status =
{
	cb_net_data_att_read,
	1,
	cb_net_data_att_1b,
	cw_SignedUnitNULL,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_2_2b_min_status =
{
	cb_net_data_att_read,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit1_0Min,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_rw_2_2b_min_set =
{
	cb_net_data_att_read_write,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit1_0Min,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_rw_2_2b_a_set =
{
	cb_net_data_att_read_write,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit1_0A,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_rw_2_2b_w_set =
{
	cb_net_data_att_read_write,
	2,
	cb_net_data_att_2b,
	cw_SignedUnit1_0W,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_rw_2_2b_uw_set =
{
	cb_net_data_att_read_write,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit1_0W,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_r_2_2b_uw_status =
{
	cb_net_data_att_read,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit1_0W,
	cb_net_data_att_status,
};

const net_data_attribute st_net_data_att_r_2_2b_w_status =
{
	cb_net_data_att_read,
	2,
	cb_net_data_att_2b,
	cw_SignedUnit1_0W,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_4_4b_w_status =
{
	cb_net_data_att_read,
	4,
	cb_net_data_att_4b,
	cw_SignedUnit1_0W,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_2_2b_0_1_kwh_status =
{
	cb_net_data_att_read,
	2,
	cb_net_data_att_2b,
	cw_SignedUnit0_1KWH,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_2_2b_0_1_ukwh_status =
{
	cb_net_data_att_read,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit0_1KWH,
	cb_net_data_att_status,
};

const net_data_attribute st_net_data_att_rw_2_2b_1_status =
{
	cb_net_data_att_read_write,
	2,
	cb_net_data_att_2b,
	cw_UnitNULL,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_rw_2_2b_1day_status =
{
	cb_net_data_att_read_write,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit1_0Day,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_rw_4_4b_1_status = 
{
	cb_net_data_att_read_write,
	4,
	cb_net_data_att_4b,
	cw_UnitNULL,
	cb_net_data_att_status,
};

const net_data_attribute st_net_data_att_r_4_4b_0_1_kwh_status =
{
	cb_net_data_att_read,
	4,
	cb_net_data_att_4b,
	cw_UnSignedUnit0_1KWH,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_2_2b_0_0_1_a_status =
{
	cb_net_data_att_read,
	2,
	cb_net_data_att_2b,
	cw_SignedUnit0_0_1A,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_rw_2_2b_s_set =
{
	cb_net_data_att_read_write,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit1_0S,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_r_2_2b_s_status =
{
	cb_net_data_att_read,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit1_0S,
	cb_net_data_att_status,
};
//const net_data_attribute st_net_data_att_r_2_2b_0_1_v_status =
//{
//	cb_net_data_att_read,
//	2,
//	cb_net_data_att_2b,
//	cw_SignedUnit0_1V,
//	cb_net_data_att_status,
//};
const net_data_attribute st_net_data_att_r_2_2b_0_1_v_status =
{
	cb_net_data_att_read,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit0_1V,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_rw_2_2b_0_1_v_set =
{
	cb_net_data_att_read_write,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit0_1V,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_r_1_1b_C_status =
{
	cb_net_data_att_read,
	1,
	cb_net_data_att_1b,
	cw_SignedUnit1_0C,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_2_2b_0_1_C_status =
{
	cb_net_data_att_read,
	2,
	cb_net_data_att_2b,
	cw_SignedUnit0_1C,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_2_2b_wh_status =
{
	cb_net_data_att_read,
	2,
	cb_net_data_att_2b,
	cw_SignedUnit1_0WH,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_4_4b_wh_status =
{
	cb_net_data_att_read,
	4,
	cb_net_data_att_4b,
	cw_SignedUnit1_0WH,
	cb_net_data_att_status,
};


const net_data_attribute st_net_data_att_r_2_2b_0_1h_status =
{
	cb_net_data_att_read,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit0_1Hour,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_2_2b_h_status =
{
	cb_net_data_att_read,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit1_0Hour,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_rw_2_2b_h_set =
{
	cb_net_data_att_read_write,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit1_0Hour,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_r_2_2b_0_0_1_v_status =
{
	cb_net_data_att_read,
	2,
	cb_net_data_att_2b,
	cw_SignedUnit0_0_1V,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_2_2b_0_1_a_status =
{
	cb_net_data_att_read,
	2,
	cb_net_data_att_2b,
	cw_SignedUnit0_1A,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_2_2b_0_1_ua_status =
{
	cb_net_data_att_read,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit0_1A,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_2_2b_0_0_1_ua_status =
{
	cb_net_data_att_read,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit0_0_1A,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_4_4b_0_0_1_ua_status =
{
	cb_net_data_att_read,
	4,
	cb_net_data_att_4b,
	cw_UnSignedUnit0_0_1A,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_1_1_per_status =
{
	cb_net_data_att_read,
	1,
	cb_net_data_att_1b,
	cw_UnSignedUnit1_0Per,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_2_2b_mv_status =
{
	cb_net_data_att_read,
	2,
	cb_net_data_att_2b,
	cw_SignedUnit1_0MV,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_2_2b_umv_status =
{
	cb_net_data_att_read,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit1_0MV,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_rw_2_2b_mv_set =
{
	cb_net_data_att_read_write,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit1_0MV,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_rw_4_4b_mv_set =
{
	cb_net_data_att_read_write,
	4,
	cb_net_data_att_4b,
	cw_UnSignedUnit1_0MV,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_rw_2_2b_1_mv_c_set =
{
	cb_net_data_att_read_write,
	2,
	cb_net_data_att_2b,
	cw_SignedUnit1_0MV_C,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_rw_2_2b_v_set =
{
	cb_net_data_att_read_write,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit1_0V,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_r_2_2b_per_status =
{
	cb_net_data_att_read,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit1_0Per,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_2_2b_C_status =
{
	cb_net_data_att_read,
	2,
	cb_net_data_att_2b,
	cw_SignedUnit1_0C,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_rw_2_2b_C_set =
{
	cb_net_data_att_read_write,
	2,
	cb_net_data_att_2b,
	cw_SignedUnit1_0C,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_2_2b_ah_status =
{
	cb_net_data_att_read,
	2,
	cb_net_data_att_2b,
	cw_UnSignedUnit1_0AH,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_2_2b_0_0_1_status =
{
	cb_net_data_att_read,
	2,
	cb_net_data_att_2b,
	cw_UnitNULL0_0_1,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_4_4b_0_0_1_status =
{
	cb_net_data_att_read,
	4,
	cb_net_data_att_4b,
	cw_UnitNULL0_0_1,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_rw_4_4b_0_0_1_set =
{
	cb_net_data_att_read_write,
	4,
	cb_net_data_att_4b,
	cw_UnitNULL0_0_1,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_rw_4_4b_signed0_0_1_set =
{
	cb_net_data_att_read_write,
	4,
	cb_net_data_att_4b,
	cw_SignedUnitNULL0_0_1,
	cb_net_data_att_set,
};



const net_data_attribute st_net_data_att_rw_4_4b_hexstring_set =
{
	cb_net_data_att_read_write,
	4,
	cb_net_data_att_4b,
	cw_hexstring,
	cb_net_data_att_set,
};
const net_data_attribute st_net_data_att_rw_4_4b_asciistring_set =
{
	cb_net_data_att_read_write,
	4,
	cb_net_data_att_4b,
	cw_asciistring,
	cb_net_data_att_set,
};

const net_data_attribute st_net_data_att_r_20_1b_1_status =
{
	cb_net_data_att_read,
	20,
	cb_net_data_att_1b,
	cw_UnitNULL,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_30_1b_1_status =
{
	cb_net_data_att_read,
	30,
	cb_net_data_att_1b,
	cw_UnitNULL,
	cb_net_data_att_status,
};
const net_data_attribute st_net_data_att_r_41_1b_1_set =
{
	cb_net_data_att_read_write,
	41,
	cb_net_data_att_1b,
	cw_UnitNULL,
	cb_net_data_att_set,
};
