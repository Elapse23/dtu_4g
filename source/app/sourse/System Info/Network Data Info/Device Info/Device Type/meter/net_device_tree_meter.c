#include "net_device_tree_meter.h"
#include "network_local_device_secquence_operate.h"

//////////////////////////////////////////////////////////
net_device_tree st_net_device_tree_meter_1 =
{
    .st_device_tree.b_device_id[2] = (cdw_device_id_meter_1 & 0x000000FF) >> 0,
    .st_device_tree.b_device_id[1] = (cdw_device_id_meter_1 & 0x0000FF00) >> 8,
    .st_device_tree.b_device_id[0] = (cdw_device_id_meter_1 & 0x00FF0000) >> 16,
};
//////////////////////////////////////////////////////////
