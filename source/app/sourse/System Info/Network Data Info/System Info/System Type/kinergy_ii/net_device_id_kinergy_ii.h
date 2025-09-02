#ifndef _NET_DEVICE_ID_KiNERGY_II_H_
#define _NET_DEVICE_ID_KiNERGY_II_H_

#include <stdint.h>
#include "net_device_info.h"


//System device id index    : dwa_device_id_kinergy_ii[cw_device_id_index_system]
//
typedef enum
{
    cw_device_id_index_system = 0,
    cw_device_id_index_kinergy_ii,
    cw_device_id_index_kinergy,

    cw_device_id_index_cklcd_u1,
    cw_device_id_index_cklcd_u2,
    cw_device_id_index_cklcd_u3,
    cw_device_id_index_cklcd_v1,
    cw_device_id_index_cklcd_v2,
    cw_device_id_index_cklcd_v3,
    cw_device_id_index_cklcd_w1,
    cw_device_id_index_cklcd_w2,
    cw_device_id_index_cklcd_w3,

    cw_device_id_index_ck_inverter_u1,
    cw_device_id_index_ck_inverter_u2,
    cw_device_id_index_ck_inverter_u3,
    cw_device_id_index_ck_inverter_v1,
    cw_device_id_index_ck_inverter_v2,
    cw_device_id_index_ck_inverter_v3,
    cw_device_id_index_ck_inverter_w1,
    cw_device_id_index_ck_inverter_w2,
    cw_device_id_index_ck_inverter_w3,

    cw_device_id_index_mppt_u1_0,
    cw_device_id_index_mppt_u1_1,
    cw_device_id_index_mppt_u1_2,
    cw_device_id_index_mppt_u1_3,
    cw_device_id_index_mppt_u1_4,
    cw_device_id_index_mppt_u1_5,

    cw_device_id_index_mppt_u2_0,
    cw_device_id_index_mppt_u2_1,
    cw_device_id_index_mppt_u2_2,
    cw_device_id_index_mppt_u2_3,
    cw_device_id_index_mppt_u2_4,
    cw_device_id_index_mppt_u2_5,

    cw_device_id_index_mppt_u3_0,
    cw_device_id_index_mppt_u3_1,
    cw_device_id_index_mppt_u3_2,
    cw_device_id_index_mppt_u3_3,
    cw_device_id_index_mppt_u3_4,
    cw_device_id_index_mppt_u3_5,

    cw_device_id_index_mppt_v1_0,
    cw_device_id_index_mppt_v1_1,
    cw_device_id_index_mppt_v1_2,
    cw_device_id_index_mppt_v1_3,
    cw_device_id_index_mppt_v1_4,
    cw_device_id_index_mppt_v1_5,

    cw_device_id_index_mppt_v2_0,
    cw_device_id_index_mppt_v2_1,
    cw_device_id_index_mppt_v2_2,
    cw_device_id_index_mppt_v2_3,
    cw_device_id_index_mppt_v2_4,
    cw_device_id_index_mppt_v2_5,

    cw_device_id_index_mppt_v3_0,
    cw_device_id_index_mppt_v3_1,
    cw_device_id_index_mppt_v3_2,
    cw_device_id_index_mppt_v3_3,
    cw_device_id_index_mppt_v3_4,
    cw_device_id_index_mppt_v3_5,

    cw_device_id_index_mppt_w1_0,
    cw_device_id_index_mppt_w1_1,
    cw_device_id_index_mppt_w1_2,
    cw_device_id_index_mppt_w1_3,
    cw_device_id_index_mppt_w1_4,
    cw_device_id_index_mppt_w1_5,

    cw_device_id_index_mppt_w2_0,
    cw_device_id_index_mppt_w2_1,
    cw_device_id_index_mppt_w2_2,
    cw_device_id_index_mppt_w2_3,
    cw_device_id_index_mppt_w2_4,
    cw_device_id_index_mppt_w2_5,

    cw_device_id_index_mppt_w3_0,
    cw_device_id_index_mppt_w3_1,
    cw_device_id_index_mppt_w3_2,
    cw_device_id_index_mppt_w3_3,
    cw_device_id_index_mppt_w3_4,
    cw_device_id_index_mppt_w3_5,

    cw_device_id_index_pv_1,
    cw_device_id_index_pv_2,

    cw_device_id_index_meter_1,

	

    cw_device_id_index_end,
}device_id_index;

typedef enum{
    cw_device_id_mppt_u1_0 = (uint16_t)(cdw_mppt_parallel_u1_0),
    cw_device_id_mppt_u1_1 = (uint16_t)(cdw_mppt_parallel_u1_1),
    cw_device_id_mppt_u1_2 = (uint16_t)(cdw_mppt_parallel_u1_2),
    cw_device_id_mppt_u1_3 = (uint16_t)(cdw_mppt_parallel_u1_3),
    cw_device_id_mppt_u1_4 = (uint16_t)(cdw_mppt_parallel_u1_4),
    cw_device_id_mppt_u1_5 = (uint16_t)(cdw_mppt_parallel_u1_5),

    cw_device_id_mppt_u2_0 = (uint16_t)(cdw_mppt_parallel_u2_0),
    cw_device_id_mppt_u2_1 = (uint16_t)(cdw_mppt_parallel_u2_1),
    cw_device_id_mppt_u2_2 = (uint16_t)(cdw_mppt_parallel_u2_2),
    cw_device_id_mppt_u2_3 = (uint16_t)(cdw_mppt_parallel_u2_3),
    cw_device_id_mppt_u2_4 = (uint16_t)(cdw_mppt_parallel_u2_4),
    cw_device_id_mppt_u2_5 = (uint16_t)(cdw_mppt_parallel_u2_5),

    cw_device_id_mppt_u3_0 = (uint16_t)(cdw_mppt_parallel_u3_0),
    cw_device_id_mppt_u3_1 = (uint16_t)(cdw_mppt_parallel_u3_1),
    cw_device_id_mppt_u3_2 = (uint16_t)(cdw_mppt_parallel_u3_2),
    cw_device_id_mppt_u3_3 = (uint16_t)(cdw_mppt_parallel_u3_3),
    cw_device_id_mppt_u3_4 = (uint16_t)(cdw_mppt_parallel_u3_4),
    cw_device_id_mppt_u3_5 = (uint16_t)(cdw_mppt_parallel_u3_5),

    cw_device_id_mppt_v1_0 = (uint16_t)(cdw_mppt_parallel_v1_0),
    cw_device_id_mppt_v1_1 = (uint16_t)(cdw_mppt_parallel_v1_1),
    cw_device_id_mppt_v1_2 = (uint16_t)(cdw_mppt_parallel_v1_2),
    cw_device_id_mppt_v1_3 = (uint16_t)(cdw_mppt_parallel_v1_3),
    cw_device_id_mppt_v1_4 = (uint16_t)(cdw_mppt_parallel_v1_4),
    cw_device_id_mppt_v1_5 = (uint16_t)(cdw_mppt_parallel_v1_5),

    cw_device_id_mppt_v2_0 = (uint16_t)(cdw_mppt_parallel_v2_0),
    cw_device_id_mppt_v2_1 = (uint16_t)(cdw_mppt_parallel_v2_1),
    cw_device_id_mppt_v2_2 = (uint16_t)(cdw_mppt_parallel_v2_2),
    cw_device_id_mppt_v2_3 = (uint16_t)(cdw_mppt_parallel_v2_3),
    cw_device_id_mppt_v2_4 = (uint16_t)(cdw_mppt_parallel_v2_4),
    cw_device_id_mppt_v2_5 = (uint16_t)(cdw_mppt_parallel_v2_5),

    cw_device_id_mppt_v3_0 = (uint16_t)(cdw_mppt_parallel_v3_0),
    cw_device_id_mppt_v3_1 = (uint16_t)(cdw_mppt_parallel_v3_1),
    cw_device_id_mppt_v3_2 = (uint16_t)(cdw_mppt_parallel_v3_2),
    cw_device_id_mppt_v3_3 = (uint16_t)(cdw_mppt_parallel_v3_3),
    cw_device_id_mppt_v3_4 = (uint16_t)(cdw_mppt_parallel_v3_4),
    cw_device_id_mppt_v3_5 = (uint16_t)(cdw_mppt_parallel_v3_5),

    cw_device_id_mppt_w1_0 = (uint16_t)(cdw_mppt_parallel_w1_0),
    cw_device_id_mppt_w1_1 = (uint16_t)(cdw_mppt_parallel_w1_1),
    cw_device_id_mppt_w1_2 = (uint16_t)(cdw_mppt_parallel_w1_2),
    cw_device_id_mppt_w1_3 = (uint16_t)(cdw_mppt_parallel_w1_3),
    cw_device_id_mppt_w1_4 = (uint16_t)(cdw_mppt_parallel_w1_4),
    cw_device_id_mppt_w1_5 = (uint16_t)(cdw_mppt_parallel_w1_5),

    cw_device_id_mppt_w2_0 = (uint16_t)(cdw_mppt_parallel_w2_0),
    cw_device_id_mppt_w2_1 = (uint16_t)(cdw_mppt_parallel_w2_1),
    cw_device_id_mppt_w2_2 = (uint16_t)(cdw_mppt_parallel_w2_2),
    cw_device_id_mppt_w2_3 = (uint16_t)(cdw_mppt_parallel_w2_3),
    cw_device_id_mppt_w2_4 = (uint16_t)(cdw_mppt_parallel_w2_4),
    cw_device_id_mppt_w2_5 = (uint16_t)(cdw_mppt_parallel_w2_5),

    cw_device_id_mppt_w3_0 = (uint16_t)(cdw_mppt_parallel_w3_0),
    cw_device_id_mppt_w3_1 = (uint16_t)(cdw_mppt_parallel_w3_1),
    cw_device_id_mppt_w3_2 = (uint16_t)(cdw_mppt_parallel_w3_2),
    cw_device_id_mppt_w3_3 = (uint16_t)(cdw_mppt_parallel_w3_3),
    cw_device_id_mppt_w3_4 = (uint16_t)(cdw_mppt_parallel_w3_4),
    cw_device_id_mppt_w3_5 = (uint16_t)(cdw_mppt_parallel_w3_5),
}device_id_ex_mppt;

extern uint32_t dwa_device_id_kinergy_ii[cw_device_id_index_end];
extern uint16_t w_device_id_ex_mppt[54];

void device_id_init();


#endif
