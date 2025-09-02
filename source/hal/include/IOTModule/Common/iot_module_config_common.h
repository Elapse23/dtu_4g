#ifndef _IOT_MODULE_CONFIG_COMMON_H_
#define _IOT_MODULE_CONFIG_COMMON_H_

#include <stdint.h>

typedef enum
{
    iot_module_type_lte,
    iot_module_type_ble,
    iot_module_type_number,
    iot_module_type_4g
}iot_module_type;

typedef struct
{
    uint8_t b_iot_module_type;
    uint8_t b_iot_module_operation;
    uint8_t b_channel_index;
    uint8_t b_err_code;
    void *pv_data;
}iot_module_driver_operate_result;

#endif
