#include "net_device_status_kinergy.h"
#include "application_general_macro.h"

net_device_status st_net_device_status_kinergy =
{
    .b_communicate_status = cb_net_device_add,
    .b_virtual_device = TRUE,
};
