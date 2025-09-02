#include "net_device_tree_pv.h"
#include "network_local_device_secquence_operate.h"

//////////////////////////////////////////////////////////
net_device_tree st_net_device_tree_pv_1 =
{
    .st_device_tree.b_device_id[2] = (cdw_device_id_pv_1 & 0x000000FF) >> 0,
    .st_device_tree.b_device_id[1] = (cdw_device_id_pv_1 & 0x0000FF00) >> 8,
    .st_device_tree.b_device_id[0] = (cdw_device_id_pv_1 & 0x00FF0000) >> 16,
};
//////////////////////////////////////////////////////////
net_device_tree st_net_device_tree_pv_2 =
{
    .st_device_tree.b_device_id[2] = (cdw_device_id_pv_2 & 0x000000FF) >> 0,
    .st_device_tree.b_device_id[1] = (cdw_device_id_pv_2 & 0x0000FF00) >> 8,
    .st_device_tree.b_device_id[0] = (cdw_device_id_pv_2 & 0x00FF0000) >> 16,
};
