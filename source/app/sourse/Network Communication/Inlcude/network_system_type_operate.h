#ifndef _NETWORK_SYSTEM_TYPE_OPERATE_
#define _NETWORK_SYSTEM_TYPE_OPERATE_

#include <stdint.h>
#include "net_device_data_cklcd.h"

#define     cdw_net_system_type_ck_wifi          0x01000400
#define     cdw_net_system_type_ck_gprs          0x01000401
#define     cdw_net_system_type_ck_4g            0x01000404

#define     cdw_net_system_type_riio_wifi        0x01000500
#define     cdw_net_system_type_riio_gprs        0x01000501
#define     cdw_net_system_type_riio_4g          0x01000504		//系统类型


typedef enum
{
    cb_system_protocal_type_ck = 0,
    cb_system_protocal_type_riiosunii,
}system_protocal_tpye;



#define cb_system_init_type_device_tree             0
#define cb_system_init_type_device_com              1
#define cb_system_init_type_network                 2
#define cb_system_init_type_system                  0xFF

uint8_t get_net_system_type(uint32_t *pdw_system_type_in);
uint8_t network_system_init();
void system_init_comfirm(uint8_t b_comfirm_type);
uint8_t check_system_init(uint8_t b_init_type);
uint8_t check_system_protocal_type(uint8_t *pb_data_in);
uint8_t model_identification(uint32_t dw_device_id, inv_type enum_device_type);
uint8_t get_system_communicate_status();



#endif
