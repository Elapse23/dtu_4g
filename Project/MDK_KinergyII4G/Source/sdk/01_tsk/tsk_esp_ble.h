/**
 * @file tsk_esp_ble.h
 *
 */

#ifndef __TSK_ESP_BLE_H__
#define __TSK_ESP_BLE_H__



/**********************
 *      INCLUDES
 **********************/
#include "prj_config.h"
#include "api_esp_tcp_ble.h"
#include "tsk_uart.h"

/**********************
 *      DEFINES
 **********************/
#define BLE_INFORM_QUEUE_MAX_NUM		10
#define BLE_INFORM_WAIT_TIME			500
#define BLE_PICO_LEARN_CMD_WAIT_TIME	1000


/**********************
 *      TYPEDEFS
 **********************/
typedef enum
{
	BLE_CONFIG,
	BLE_MONITOR,
	BLE_CONNECTED,
	BLE_SPP_MODE,
	BLE_NULL,
	
}BLE_TASK_WORK_STATUS_TYPE;

typedef enum
{
	BLE_MESSAGE_QUERY_REPLY,
	BLE_MESSAGE_SET_REPLY,
	BLE_MESSAGE_BROADCASE_CUSTOM,
	
}BLE_INFORM_MESSAGE_TYPE;

typedef struct
{
	BLE_INFORM_MESSAGE_TYPE type;
	uint16_t len;
	uint16_t result;

}BLE_INFORM_QUEUE_TYPE;


/**********************
 *  GLOBAL VARIABLES
 **********************/
extern QueueHandle_t g_Ble_Inform_Queue;
extern uint8_t g_bleOpeBuff[400];
extern uint8_t g_BleTestResult;

/**********************
 * GLOBAL PROTOTYPES
 **********************/
/**
 * ble status get
 * @param null
 * @return ble status
 */
extern BLE_TASK_WORK_STATUS_TYPE ble_status_get(void);
extern void time_stamp_rtc_sync(uint32_t sync_stamp, uint8_t set_source);

/**
 * ble refresh task
 * @param pvParameters
 * @return null
 */
extern void vBLETask(void *pvParameters);




#endif
