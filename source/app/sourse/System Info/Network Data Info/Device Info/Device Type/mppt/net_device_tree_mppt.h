#ifndef _NET_DEVICE_TREE_MPPT_H_
#define _NET_DEVICE_TREE_MPPT_H_

#include "net_device_tree.h"

typedef enum{
    cdw_device_id_mppt_u1_0 = (cdw_device_type_mppt | cdw_mppt_parallel_u1_0),
    cdw_device_id_mppt_u1_1 = (cdw_device_type_mppt | cdw_mppt_parallel_u1_1),
    cdw_device_id_mppt_u1_2 = (cdw_device_type_mppt | cdw_mppt_parallel_u1_2),
    cdw_device_id_mppt_u1_3 = (cdw_device_type_mppt | cdw_mppt_parallel_u1_3),
    cdw_device_id_mppt_u1_4 = (cdw_device_type_mppt | cdw_mppt_parallel_u1_4),
    cdw_device_id_mppt_u1_5 = (cdw_device_type_mppt | cdw_mppt_parallel_u1_5),

    cdw_device_id_mppt_u2_0 = (cdw_device_type_mppt | cdw_mppt_parallel_u2_0),
    cdw_device_id_mppt_u2_1 = (cdw_device_type_mppt | cdw_mppt_parallel_u2_1),
    cdw_device_id_mppt_u2_2 = (cdw_device_type_mppt | cdw_mppt_parallel_u2_2),
    cdw_device_id_mppt_u2_3 = (cdw_device_type_mppt | cdw_mppt_parallel_u2_3),
    cdw_device_id_mppt_u2_4 = (cdw_device_type_mppt | cdw_mppt_parallel_u2_4),
    cdw_device_id_mppt_u2_5 = (cdw_device_type_mppt | cdw_mppt_parallel_u2_5),

    cdw_device_id_mppt_u3_0 = (cdw_device_type_mppt | cdw_mppt_parallel_u3_0),
    cdw_device_id_mppt_u3_1 = (cdw_device_type_mppt | cdw_mppt_parallel_u3_1),
    cdw_device_id_mppt_u3_2 = (cdw_device_type_mppt | cdw_mppt_parallel_u3_2),
    cdw_device_id_mppt_u3_3 = (cdw_device_type_mppt | cdw_mppt_parallel_u3_3),
    cdw_device_id_mppt_u3_4 = (cdw_device_type_mppt | cdw_mppt_parallel_u3_4),
    cdw_device_id_mppt_u3_5 = (cdw_device_type_mppt | cdw_mppt_parallel_u3_5),

    cdw_device_id_mppt_v1_0 = (cdw_device_type_mppt | cdw_mppt_parallel_v1_0),
    cdw_device_id_mppt_v1_1 = (cdw_device_type_mppt | cdw_mppt_parallel_v1_1),
    cdw_device_id_mppt_v1_2 = (cdw_device_type_mppt | cdw_mppt_parallel_v1_2),
    cdw_device_id_mppt_v1_3 = (cdw_device_type_mppt | cdw_mppt_parallel_v1_3),
    cdw_device_id_mppt_v1_4 = (cdw_device_type_mppt | cdw_mppt_parallel_v1_4),
    cdw_device_id_mppt_v1_5 = (cdw_device_type_mppt | cdw_mppt_parallel_v1_5),

    cdw_device_id_mppt_v2_0 = (cdw_device_type_mppt | cdw_mppt_parallel_v2_0),
    cdw_device_id_mppt_v2_1 = (cdw_device_type_mppt | cdw_mppt_parallel_v2_1),
    cdw_device_id_mppt_v2_2 = (cdw_device_type_mppt | cdw_mppt_parallel_v2_2),
    cdw_device_id_mppt_v2_3 = (cdw_device_type_mppt | cdw_mppt_parallel_v2_3),
    cdw_device_id_mppt_v2_4 = (cdw_device_type_mppt | cdw_mppt_parallel_v2_4),
    cdw_device_id_mppt_v2_5 = (cdw_device_type_mppt | cdw_mppt_parallel_v2_5),

    cdw_device_id_mppt_v3_0 = (cdw_device_type_mppt | cdw_mppt_parallel_v3_0),
    cdw_device_id_mppt_v3_1 = (cdw_device_type_mppt | cdw_mppt_parallel_v3_1),
    cdw_device_id_mppt_v3_2 = (cdw_device_type_mppt | cdw_mppt_parallel_v3_2),
    cdw_device_id_mppt_v3_3 = (cdw_device_type_mppt | cdw_mppt_parallel_v3_3),
    cdw_device_id_mppt_v3_4 = (cdw_device_type_mppt | cdw_mppt_parallel_v3_4),
    cdw_device_id_mppt_v3_5 = (cdw_device_type_mppt | cdw_mppt_parallel_v3_5),

    cdw_device_id_mppt_w1_0 = (cdw_device_type_mppt | cdw_mppt_parallel_w1_0),
    cdw_device_id_mppt_w1_1 = (cdw_device_type_mppt | cdw_mppt_parallel_w1_1),
    cdw_device_id_mppt_w1_2 = (cdw_device_type_mppt | cdw_mppt_parallel_w1_2),
    cdw_device_id_mppt_w1_3 = (cdw_device_type_mppt | cdw_mppt_parallel_w1_3),
    cdw_device_id_mppt_w1_4 = (cdw_device_type_mppt | cdw_mppt_parallel_w1_4),
    cdw_device_id_mppt_w1_5 = (cdw_device_type_mppt | cdw_mppt_parallel_w1_5),

    cdw_device_id_mppt_w2_0 = (cdw_device_type_mppt | cdw_mppt_parallel_w2_0),
    cdw_device_id_mppt_w2_1 = (cdw_device_type_mppt | cdw_mppt_parallel_w2_1),
    cdw_device_id_mppt_w2_2 = (cdw_device_type_mppt | cdw_mppt_parallel_w2_2),
    cdw_device_id_mppt_w2_3 = (cdw_device_type_mppt | cdw_mppt_parallel_w2_3),
    cdw_device_id_mppt_w2_4 = (cdw_device_type_mppt | cdw_mppt_parallel_w2_4),
    cdw_device_id_mppt_w2_5 = (cdw_device_type_mppt | cdw_mppt_parallel_w2_5),

    cdw_device_id_mppt_w3_0 = (cdw_device_type_mppt | cdw_mppt_parallel_w3_0),
    cdw_device_id_mppt_w3_1 = (cdw_device_type_mppt | cdw_mppt_parallel_w3_1),
    cdw_device_id_mppt_w3_2 = (cdw_device_type_mppt | cdw_mppt_parallel_w3_2),
    cdw_device_id_mppt_w3_3 = (cdw_device_type_mppt | cdw_mppt_parallel_w3_3),
    cdw_device_id_mppt_w3_4 = (cdw_device_type_mppt | cdw_mppt_parallel_w3_4),
    cdw_device_id_mppt_w3_5 = (cdw_device_type_mppt | cdw_mppt_parallel_w3_5),
}device_id_ck_mppt;

extern const uint32_t cdw_outer_mppt_device_id[];

extern net_device_tree st_net_device_tree_mppt_u1_0;
extern net_device_tree st_net_device_tree_mppt_u1_1;
extern net_device_tree st_net_device_tree_mppt_u1_2;
extern net_device_tree st_net_device_tree_mppt_u1_3;
extern net_device_tree st_net_device_tree_mppt_u1_4;
extern net_device_tree st_net_device_tree_mppt_u1_5;

extern net_device_tree st_net_device_tree_mppt_u2_0;
extern net_device_tree st_net_device_tree_mppt_u2_1;
extern net_device_tree st_net_device_tree_mppt_u2_2;
extern net_device_tree st_net_device_tree_mppt_u2_3;
extern net_device_tree st_net_device_tree_mppt_u2_4;
extern net_device_tree st_net_device_tree_mppt_u2_5;

extern net_device_tree st_net_device_tree_mppt_u3_0;
extern net_device_tree st_net_device_tree_mppt_u3_1;
extern net_device_tree st_net_device_tree_mppt_u3_2;
extern net_device_tree st_net_device_tree_mppt_u3_3;
extern net_device_tree st_net_device_tree_mppt_u3_4;
extern net_device_tree st_net_device_tree_mppt_u3_5;

extern net_device_tree st_net_device_tree_mppt_v1_0;
extern net_device_tree st_net_device_tree_mppt_v1_1;
extern net_device_tree st_net_device_tree_mppt_v1_2;
extern net_device_tree st_net_device_tree_mppt_v1_3;
extern net_device_tree st_net_device_tree_mppt_v1_4;
extern net_device_tree st_net_device_tree_mppt_v1_5;

extern net_device_tree st_net_device_tree_mppt_v2_0;
extern net_device_tree st_net_device_tree_mppt_v2_1;
extern net_device_tree st_net_device_tree_mppt_v2_2;
extern net_device_tree st_net_device_tree_mppt_v2_3;
extern net_device_tree st_net_device_tree_mppt_v2_4;
extern net_device_tree st_net_device_tree_mppt_v2_5;

extern net_device_tree st_net_device_tree_mppt_v3_0;
extern net_device_tree st_net_device_tree_mppt_v3_1;
extern net_device_tree st_net_device_tree_mppt_v3_2;
extern net_device_tree st_net_device_tree_mppt_v3_3;
extern net_device_tree st_net_device_tree_mppt_v3_4;
extern net_device_tree st_net_device_tree_mppt_v3_5;

extern net_device_tree st_net_device_tree_mppt_w1_0;
extern net_device_tree st_net_device_tree_mppt_w1_1;
extern net_device_tree st_net_device_tree_mppt_w1_2;
extern net_device_tree st_net_device_tree_mppt_w1_3;
extern net_device_tree st_net_device_tree_mppt_w1_4;
extern net_device_tree st_net_device_tree_mppt_w1_5;

extern net_device_tree st_net_device_tree_mppt_w2_0;
extern net_device_tree st_net_device_tree_mppt_w2_1;
extern net_device_tree st_net_device_tree_mppt_w2_2;
extern net_device_tree st_net_device_tree_mppt_w2_3;
extern net_device_tree st_net_device_tree_mppt_w2_4;
extern net_device_tree st_net_device_tree_mppt_w2_5;

extern net_device_tree st_net_device_tree_mppt_w3_0;
extern net_device_tree st_net_device_tree_mppt_w3_1;
extern net_device_tree st_net_device_tree_mppt_w3_2;
extern net_device_tree st_net_device_tree_mppt_w3_3;
extern net_device_tree st_net_device_tree_mppt_w3_4;
extern net_device_tree st_net_device_tree_mppt_w3_5;

#endif
