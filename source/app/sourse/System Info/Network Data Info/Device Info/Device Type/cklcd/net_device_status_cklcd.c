#include "net_device_status_cklcd.h"
#include "application_general_macro.h"

net_device_status st_net_device_status_cklcd_u1 =
{
    .b_communicate_status = cb_net_device_loss,
    .w_sn_inquire_addr = 0x0003,
    .ba_device_tree_inquire_identification = {0x00,0x01},
    .b_communicate_max_timeout_times = 10,
};

net_device_status st_net_device_status_cklcd_u2 =
{
    .b_communicate_status = cb_net_device_loss,
    .w_sn_inquire_addr = 0x0003,
    .ba_device_tree_inquire_identification = {0x00,0x01},
    .b_communicate_max_timeout_times = 1,
};

net_device_status st_net_device_status_cklcd_u3 =
{
    .b_communicate_status = cb_net_device_loss,
    .w_sn_inquire_addr = 0x0003,
    .ba_device_tree_inquire_identification = {0x00,0x01},
    .b_communicate_max_timeout_times = 1,
};

net_device_status st_net_device_status_cklcd_w1 =
{
    .b_communicate_status = cb_net_device_loss,
    .w_sn_inquire_addr = 0x0003,
    .ba_device_tree_inquire_identification = {0x00,0x01},
    .b_communicate_max_timeout_times = 1,
};

net_device_status st_net_device_status_cklcd_w2 =
{
    .b_communicate_status = cb_net_device_loss,
    .w_sn_inquire_addr = 0x0003,
    .ba_device_tree_inquire_identification = {0x00,0x01},
    .b_communicate_max_timeout_times = 1,
};

net_device_status st_net_device_status_cklcd_w3 =
{
    .b_communicate_status = cb_net_device_loss,
    .w_sn_inquire_addr = 0x0003,
    .ba_device_tree_inquire_identification = {0x00,0x01},
    .b_communicate_max_timeout_times = 1,
};

net_device_status st_net_device_status_cklcd_v1 =
{
    .b_communicate_status = cb_net_device_loss,
    .w_sn_inquire_addr = 0x0003,
    .ba_device_tree_inquire_identification = {0x00,0x01},
    .b_communicate_max_timeout_times = 1,
};

net_device_status st_net_device_status_cklcd_v2 =
{
    .b_communicate_status = cb_net_device_loss,
    .w_sn_inquire_addr = 0x0003,
    .ba_device_tree_inquire_identification = {0x00,0x01},
    .b_communicate_max_timeout_times = 1,
};

net_device_status st_net_device_status_cklcd_v3 =
{
    .b_communicate_status = cb_net_device_loss,
    .w_sn_inquire_addr = 0x0003,
    .ba_device_tree_inquire_identification = {0x00,0x01},
    .b_communicate_max_timeout_times = 1,
};



