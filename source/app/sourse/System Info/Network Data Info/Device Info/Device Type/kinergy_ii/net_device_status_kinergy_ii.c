#include "net_device_status_kinergy_ii.h"
#include "application_general_macro.h"

net_device_status st_net_device_status_kinergy_ii =
{
    .b_communicate_status = cb_net_device_add,
    .b_virtual_device = TRUE,
};
