#ifndef _NETWORK_LOCAL_SECQUENCE_GENERATE_H_
#define _NETWORK_LOCAL_SECQUENCE_GENERATE_H_

#include <stdint.h>

//uint8_t network_secquence_semaphore_creat();
uint8_t network_get_local_message_id(uint32_t *pdw_secquence);
uint8_t network_check_local_message_id(uint32_t dw_secquence);
#endif
