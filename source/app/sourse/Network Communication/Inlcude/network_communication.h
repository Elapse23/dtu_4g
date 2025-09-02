#ifndef _NETWORK_COMMUNICATION_H_
#define _NETWORK_COMMUNICATION_H_

#include <stdint.h>

void network_communication_initial();
void network_communication();

void network_infomation_refresh();

void network_device_tree_refresh();
void network_device_data_polling();

#endif
