#ifndef _NET_DEVICE_DATA_ATTRIBUTE_H_
#define _NET_DEVICE_DATA_ATTRIBUTE_H_

#include <stdint.h>

//read_write_type
#define cb_net_data_att_read        	   0
#define cb_net_data_att_write       	   1
#define cb_net_data_att_read_write         2

//size
#define cb_net_data_att_1b  	           1
#define cb_net_data_att_2b  	           2
#define cb_net_data_att_4b                 4

//unit
#define cw_UnitNULL                        0x7FFF
#define cw_UnitNULL0_1                     0x7FF0
#define cw_UnitNULL0_0_1                   0x7FF1
#define cw_UnitNULL0_0_0_1                 0x7FF2
#define cw_UnitNULL0_0_0_0_1               0x7FF3
#define cw_SignedUnitNULL                  0xFFFF
#define cw_SignedUnitNULL0_1               0xFFF0
#define cw_SignedUnitNULL0_0_1             0xFFF1
#define cw_SignedUnitNULL0_0_0_1           0xFFF2
#define cw_SignedUnitNULL0_0_0_0_1         0xFFF3
#define cw_asciistring					   0x00F1
#define cw_hexstring					   0x00F7

#define cw_UnitSTR                         0x00F1

#define cw_UnSignedUnit0_1V                0x0001
#define cw_UnSignedUnit1_0V                0x0000
#define cw_SignedUnit1_0V                  0x8000
#define cw_SignedUnit0_1V				   0x8001
#define cw_UnSignedUnit0_0_1V              0x0002
#define cw_SignedUnit0_0_1V                0x8002
#define cw_SignedUnit1_0MV                 0x80F2
#define cw_UnSignedUnit1_0MV               0x00F2
#define cw_SignedUnit1_0MV_C               0x80F4

#define cw_UnSignedUnit1_0A                0x0010
#define cw_SignedUnit1_0A                  0x8010
#define cw_SignedUnit0_1A                  0x8011
#define cw_UnSignedUnit0_1A                0x0011
#define cw_UnSignedUnit0_0_1A              0x0012
#define cw_SignedUnit0_0_1A                0x8012

#define cw_SignedUnit1_0W                  0x8031
#define cw_UnSignedUnit1_0W                0x0031
#define cw_UnSignedUnit1_0VA               0x0030
#define cw_SignedUnit1_0VA                 0x8030

#define cw_SignedUnit1_0C                  0x8040//℃
#define cw_UnSignedUnit0_1C                0x0091//C
#define cw_SignedUnit0_1C                  0x8091//C
#define cw_UnSignedUnit0_0_1C              0x0092//C

#define cw_UnSignedUnit0_0_1HZ             0x0022
#define cw_UnSignedUnit1_0HZ               0x0020

#define cw_SignedUnit1_0WH                 0x8062
#define cw_SignedUnit0_1KWH                0x8063
#define cw_UnSignedUnit0_1KWH              0x0063

#define cw_UnSignedUnit1_0AH               0x0070
#define cw_UnSignedUnit0_1AH               0x0071

#define cw_UnSignedUnit1_0S                0x0080
#define cw_UnSignedUnit1_0Min              0x0081
#define cw_UnSignedUnit1_0Hour             0x0082
#define cw_UnSignedUnit0_1Hour             0x0087
#define cw_UnSignedUnit1_0Day              0x0083


#define cw_UnSignedUnit1_0KMH              0x00B0
#define cw_UnSignedUnit1_0MH               0x00B1
#define cw_UnSignedUnit0_1MH               0x00B2

#define cw_UnSignedUnit1_0MM               0x00C0
#define cw_UnSignedUnit1_0CM               0x00C1
#define cw_UnSignedUnit1_0M                0x00C2
#define cw_UnSignedUnit0_1M                0x00C3

#define cw_UnSignedUnit1_0Per              0x0050
#define cw_UnSignedUnit1_0RPM              0x00F5

//bound cmomand type
#define cb_net_data_att_status             0
#define cb_net_data_att_set                1
#define cb_net_data_att_control            2

typedef struct
{
    uint8_t b_read_write_type;
    uint8_t b_len;
	uint8_t b_size;
    uint16_t w_unit;
    uint8_t b_bound_cmd_type;
}net_data_attribute;

extern const net_data_attribute st_net_data_att_rw_4_4b_asciistring_set;
extern const net_data_attribute st_net_data_att_rw_4_4b_signed0_0_1_set;

extern const net_data_attribute st_net_data_att_rw_4_4b_hexstring_set;
extern const net_data_attribute st_net_data_att_r_1_1b_s1_status;
extern const net_data_attribute st_net_data_att_r_1_1b_1_status;
extern const net_data_attribute st_net_data_att_r_100_1b_1_status;
extern const net_data_attribute st_net_data_att_r_2_1b_1_status;
extern const net_data_attribute st_net_data_att_r_220_1b_1_status;
extern const net_data_attribute st_net_data_att_rw_1_1b_1_set;
extern const net_data_attribute st_net_data_att_rw_1_1b_per_set;
extern const net_data_attribute st_net_data_att_rw_2_2b_0_1_ah_set;
extern const net_data_attribute st_net_data_att_rw_2_2b_0_0_1_v_set;
extern const net_data_attribute st_net_data_att_rw_2_2b_0_0_1_c_set;
extern const net_data_attribute st_net_data_att_rw_4_4b_1_set;
extern const net_data_attribute st_net_data_att_rw_22_1b_1_set;
extern const net_data_attribute st_net_data_att_rw_2_2b_per_set;
extern const net_data_attribute st_net_data_att_rw_40_1b_1_set;
extern const net_data_attribute st_net_data_att_rw_2_2b_1_set;
extern const net_data_attribute st_net_data_att_w_1_1b_1_control;
extern const net_data_attribute st_net_data_att_w_14_1b_1_control;
extern const net_data_attribute st_net_data_att_r_41_1b_1_control;
extern const net_data_attribute st_net_data_att_rw_1_1b_1_control;
extern const net_data_attribute st_net_data_att_rw_4_4b_1_status;
extern const net_data_attribute st_net_data_att_rw_2_2b_1day_status;
extern const net_data_attribute st_net_data_att_rw_2_2b_1day_set;

extern const net_data_attribute st_net_data_att_rw_2_2b_1_status;

extern const net_data_attribute st_net_data_att_r_2_2b_1_status;
extern const net_data_attribute st_net_data_att_r_4_4b_1_status;
extern const net_data_attribute st_net_data_att_r_12_1b_1_status;
extern const net_data_attribute st_net_data_att_r_13_1b_1_status;
extern const net_data_attribute st_net_data_att_r_4_4b_1_m_status;
extern const net_data_attribute st_net_data_att_r_4_4b_1_mh_status;
extern const net_data_attribute st_net_data_att_r_4_4b_0_1_mh_status;
extern const net_data_attribute st_net_data_att_r_1_1b_1_1_status;
extern const net_data_attribute st_net_data_att_r_6_1b_1_1_status;
extern const net_data_attribute st_net_data_att_r_4_4b_0_1_m_status;
extern const net_data_attribute st_net_data_att_rw_5_1b_1_set;
extern const net_data_attribute st_net_data_att_r_1_1b_per_status;
extern const net_data_attribute st_net_data_att_r_2_2b_min_status;
extern const net_data_attribute st_net_data_att_r_2_2b_0_1_v_status;
extern const net_data_attribute st_net_data_att_r_1_1b_C_status;
extern const net_data_attribute st_net_data_att_r_2_2b_wh_status;
extern const net_data_attribute st_net_data_att_r_4_4b_wh_status;
extern const net_data_attribute st_net_data_att_r_4_4b_0_wh_status;
extern const net_data_attribute st_net_data_att_r_2_2b_h_status;
extern const net_data_attribute st_net_data_att_r_2_2b_0_0_1_v_status;
extern const net_data_attribute st_net_data_att_r_2_2b_0_1_a_status;
extern const net_data_attribute st_net_data_att_r_1_1_per_status;
extern const net_data_attribute st_net_data_att_r_2_2b_0_0_0_1_v_status;
extern const net_data_attribute st_net_data_att_r_2_2b_per_status;
extern const net_data_attribute st_net_data_att_r_2_2b_C_status;
extern const net_data_attribute st_net_data_att_r_2_2b_ah_status;
extern const net_data_attribute st_net_data_att_r_2_2b_0_0_1_status;
extern const net_data_attribute st_net_data_att_r_2_2b_0_0_0_1_V_status;
extern const net_data_attribute st_net_data_att_r_20_1b_1_status;
extern const net_data_attribute st_net_data_att_r_30_1b_1_status;
extern const net_data_attribute st_net_data_att_r_2_2b_mv_status;
extern const net_data_attribute st_net_data_att_rw_30_1b_1_set;
extern const net_data_attribute st_net_data_att_rw_30_1b_asciistring_set;
extern const net_data_attribute st_net_data_att_r_41_1b_1_set;
extern const net_data_attribute st_net_data_att_rw_2_2b_1_0_ah_set;
extern const net_data_attribute st_net_data_att_rw_2_2b_0_0_1hz_set;
extern const net_data_attribute st_net_data_att_w_2_2b_1_control;
extern const net_data_attribute st_net_data_att_rw_2_2b_mv_set;
extern const net_data_attribute st_net_data_att_rw_2_2b_w_set;
extern const net_data_attribute st_net_data_att_rw_2_2b_s_set;
extern const net_data_attribute st_net_data_att_rw_2_2b_1_0va_set;
extern const net_data_attribute st_net_data_att_rw_2_2b_min_set;
extern const net_data_attribute st_net_data_att_r_2_2b_w_status;
extern const net_data_attribute st_net_data_att_r_2_2b_0_1_kwh_status;
extern const net_data_attribute st_net_data_att_r_2_2b_0_0_1_a_status;
extern const net_data_attribute st_net_data_att_r_4_4b_0_1_kwh_status;
extern const net_data_attribute st_net_data_att_r_2_2b_0_0_1hz_status;
extern const net_data_attribute st_net_data_att_r_138_1b_1_status;
extern const net_data_attribute st_net_data_att_rw_1_1b_1_0_m_set;
extern const net_data_attribute st_net_data_att_rw_1_1b_1_mh_set;
extern const net_data_attribute st_net_data_att_rw_2_2b_v_set;
extern const net_data_attribute st_net_data_att_rw_2_2b_1_0hz_set;
extern const net_data_attribute st_net_data_att_r_15_1b_1_set;
extern const net_data_attribute st_net_data_att_rw_2_2b_a_set;
extern const net_data_attribute st_net_data_att_rw_2_2b_h_set;
extern const net_data_attribute st_net_data_att_r_2_2b_1_0_va_status;
extern const net_data_attribute st_net_data_att_r_2_2b_1_0_uva_status;
extern const net_data_attribute st_net_data_att_r_4_4b_1_0_va_status;
extern const net_data_attribute st_net_data_att_r_2_2b_umv_status;
extern const net_data_attribute st_net_data_att_r_2_2b_0_1_ukwh_status;
extern const net_data_attribute st_net_data_att_rw_2_2b_0_1_v_set;
extern const net_data_attribute st_net_data_att_rw_2_2b_C_set;
extern const net_data_attribute st_net_data_att_r_2_2b_0_1_ua_status;
extern const net_data_attribute st_net_data_att_r_2_2b_0_0_1_ua_status;
extern const net_data_attribute st_net_data_att_r_4_4b_0_0_1_ua_status;
extern const net_data_attribute st_net_data_att_r_2_2b_0_1_C_status;
extern const net_data_attribute st_net_data_att_r_4_4b_w_status;
extern const net_data_attribute st_net_data_att_r_2_2b_0_1_status;
extern const net_data_attribute st_net_data_att_rw_129_1b_1_set;
extern const net_data_attribute st_net_data_att_rw_4_1b_1_set;
extern const net_data_attribute st_net_data_att_rw_4_4b_0_0_1_set;
extern const net_data_attribute st_net_data_att_r_4_4b_0_0_1_status;
extern const net_data_attribute st_net_data_att_r_2_2b_s_status;
extern const net_data_attribute st_net_data_att_rw_2_2b_1_mv_c_set;
extern const net_data_attribute st_net_data_att_rw_4_4b_mv_set;
extern const net_data_attribute st_net_data_att_r_2_2b_0_1h_status;


#endif
