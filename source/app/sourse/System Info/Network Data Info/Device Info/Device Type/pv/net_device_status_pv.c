#include "net_device_status_pv.h"
#include "application_general_macro.h"

////////////////////////////
net_device_status st_net_device_status_pv_1 =
{
    .b_communicate_status = cb_net_device_loss,
};
net_device_status st_net_device_status_pv_2 =
{
    .b_communicate_status = cb_net_device_loss,
};
