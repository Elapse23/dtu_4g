#ifndef _PRODUCT_TEST_H_
#define _PRODUCT_TEST_H_

#include <stdint.h>

#define product_test_can_enable                     FALSE
#define product_test_rs485_enable                   TRUE

#define cb_product_test_debug                       FALSE
#define PRODUCT_TEST_DISABLE_INQUIRE                TRUE
#define PRODUCT_TEST_EXIT_CLEAR_EEPROM              TRUE

#if( TRUE == product_test_rs485_enable)
uint8_t product_test_analysis(uint32_t dw_device_address,
                              uint8_t b_cmd,
                              uint16_t w_data_addr,
                              uint16_t *pw_data_out_len,
                              uint8_t *pb_data_out,
                              uint16_t w_data_out_buffer_size,
                              uint8_t *pb_receive_state);

void product_test_rs485_channel();
#endif

#if( TRUE == product_test_can_enable)
void product_test_can_channel();
#endif

uint8_t check_product_test();
void product_test_wireless_com_comfirm();


#endif
