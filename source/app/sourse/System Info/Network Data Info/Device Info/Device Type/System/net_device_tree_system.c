#include "net_device_tree_system.h"
#include "network_local_device_secquence_operate.h"

net_device_tree st_net_device_tree_system =
{
	.st_device_tree.b_device_id[2] = (cdw_device_id_system & 0x000000FF) >> 0,
	.st_device_tree.b_device_id[1] = (cdw_device_id_system & 0x0000FF00) >> 8,
	.st_device_tree.b_device_id[0] = (cdw_device_id_system & 0x00FF0000) >> 16,
	.st_device_tree.b_serial_number = "ck223458223459",
	.st_device_tree.b_firmware_version = "WCM_II V1.0",
};




