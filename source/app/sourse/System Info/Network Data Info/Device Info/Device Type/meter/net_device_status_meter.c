#include "net_device_status_meter.h"
#include "application_general_macro.h"

////////////////////////////
net_device_status st_net_device_status_meter_1 =
{
    .b_communicate_status = cb_net_device_loss,
};
