#ifndef _LTE_MODULE_NMEA_INFO_PROCESS_H_
#define _LTE_MODULE_NMEA_INFO_PROCESS_H_

#include <stdint.h>

extern uint32_t dw_total_distance;
extern uint32_t dw_timestamp_backup;
extern uint32_t dw_distance;

void lte_module_nmea_info_get();

#endif
