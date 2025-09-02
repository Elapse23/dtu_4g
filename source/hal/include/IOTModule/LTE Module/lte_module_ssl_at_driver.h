#ifndef _LTE_MODULE_SSL_AT_DRIVER_H_
#define _LTE_MODULE_SSL_AT_DRIVER_H_

#include <stdint.h>
#include "lte_module.h"

uint8_t lte_module_driver_ssl_configure(lte_module_driver_config_info_ssl *pst_ssl,
                                        lte_module_driver_process_contral *pst_process_contral);

#endif
