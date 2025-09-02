#ifndef _FREERTOS_USER_CONFIG_H_
#define _FREERTOS_USER_CONFIG_H_

#include "FreeRTOS.h"
#include "task.h"

/*
 * Task priorities.
 */
#define IDLE_TASK_PRIORITY                  				   							( tskIDLE_PRIORITY )

#define network_iot_module_config_excute_task_priority								    ( tskIDLE_PRIORITY + 2 )
#define RS485_queue_recognition_conversion_task_priority							    ( tskIDLE_PRIORITY + 2 )

#define network_communicate_task_priority                                               ( tskIDLE_PRIORITY + 2 )

#define wireless_module_send_data_background_task_priority								( tskIDLE_PRIORITY + 2 )
#define wireless_module_receive_data_background_task_priority							( tskIDLE_PRIORITY + 2 )
#define wireless_module_receive_iot_data_task_priority									( tskIDLE_PRIORITY + 2 )

#define wireless_communicate_receive_data_task_priority									( tskIDLE_PRIORITY + 2 )

#define rs485_tx_data_task_priority								        	            ( tskIDLE_PRIORITY + 2 )

#define device_tree_refresh_task_priority												( tskIDLE_PRIORITY + 2 )

#define device_data_polling_task_priority												( tskIDLE_PRIORITY + 2 )

#define timestamp_task_priority												            ( tskIDLE_PRIORITY + 2 )

#define network_ble_communication_task_priority										    ( tskIDLE_PRIORITY + 3 )


#define spi_flash_operate_process_task_priority											( tskIDLE_PRIORITY + 3 )
#define display_management_task_priority											    ( tskIDLE_PRIORITY + 2 )

#define energy_statistics_task_priority                                                 ( tskIDLE_PRIORITY + 2 )
#define firmware_update_task_priority                                                   ( tskIDLE_PRIORITY + 2 )

/*
 * Task stack siz.
 */

#define network_iot_module_config_excute_task_stack_size			   				    300
#define RS485_queue_recognition_conversion_task_size									100

#define network_communicate_task_stack_size                                             600

#define wireless_module_send_data_background_task_stack_size							300
#define wireless_module_receive_data_background_task_stack_size							512
#define wireless_module_receive_iot_data_task_stack_size								768

#define wireless_communicate_receive_data_task_stack_size								300

#define device_tree_refresh_task_stack_size		   										300
#define device_data_polling_task_stack_size		   										300

#define rs485_tx_data_task_stack_size		   	    									300

#define timestamp_task_stack_size												        200

#define network_ble_communication_task_stack_size										896

#define spi_flash_operate_process_task_stack_size										300
#define display_management_task_stack_size										        200
#define energy_statistics_task_stack_size										        500
#define firmware_update_task_stack_size										            500

extern TaskHandle_t RS485_queue_recognition_conversion_task_handle;
extern TaskHandle_t network_iot_module_config_excute_task_handle;
extern TaskHandle_t device_tree_refresh_task_handle;
extern TaskHandle_t device_data_polling_task_handle;
extern TaskHandle_t network_communicate_task_handle;
extern TaskHandle_t wireless_communicate_receive_data_task_handle;
extern TaskHandle_t wireless_communicate_send_data_task_handle;
extern TaskHandle_t wireless_module_receive_data_background_task_handle;
extern TaskHandle_t wireless_module_receive_iot_data_task_handle;
extern TaskHandle_t wireless_module_send_data_background_task_handle;
extern TaskHandle_t timestamp_task_handle;
extern TaskHandle_t display_management_task_handle;
extern TaskHandle_t energy_statistics_task_handle;
extern TaskHandle_t firmware_update_task_handle;

//#define pdMS_TO_TICKS_USER(xTimeInMs)  (( TickType_t ) ( xTimeInMs ) == 0?\
//                                            0 : ( TickType_t ) ( xTimeInMs ) < ( TickType_t )1000U / configTICK_RATE_HZ?\
//                                                ( TickType_t )1000U / configTICK_RATE_HZ : ( TickType_t ) ( xTimeInMs ) / ( TickType_t )10U)

#define pdMS_TO_TICKS_USER(xTimeInMs)  pdMS_TO_TICKS(xTimeInMs)

void *pvPortMallocUser(size_t xWantedSize,uint8_t b_block);
void vPortFreeUser(void **pv,uint8_t *pb_description);

#endif
