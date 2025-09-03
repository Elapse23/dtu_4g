/**
 * @file api_esp_wifi_ble.h
 *
 */

#ifndef __API_ESP_TCP_BLE_H__
#define __API_ESP_TCP_BLE_H__



/**********************
 *      INCLUDES
 **********************/
#include "prj_config.h"


/**********************
 *      DEFINES
 **********************/
#define ESP_DATA_MAX_LEN		800

//the head of ble adv name,has length limit
#define BLE_NAME_MAX_LENGTH		22
#define ESP_TX_DATA_MAX_LEN			500

/**********************
 *      TYPEDEFS
 **********************/
typedef enum
{
	ESP_AT_ERROR_NULL,
	ESP_AT_ERROR_NO_REPLY,
	ESP_AT_ERROR_REPLY_CHECK_FAILED,
	ESP_AT_ERROR_SUCCESS,

}ESP_ATCMD_ERROR_CODE_TYPE;


typedef enum{
	
	ESP_BLE_AT = 0,
	ESP_BLE_ATE0,
	ESP_BLE_GMR,
	ESP_BLE_BAUD_CHANGED,
	ESP_BLE_CLOSE_WIFI,
	ESP_BLE_BLEINIT,
	ESP_BLE_BLEGATTSSRVCRE,
	ESP_BLE_BLEGATTSSRVSTART,
	ESP_BLE_BLEADDR,
	ESP_BLE_SET_PARAMETER,
	ESP_BLE_BLEADVDATA,
	ESP_BLE_BLEADVSTART,

	ESP_BLE_CONFIG_SYSMSG,
	ESP_BLE_CONFIG_SPP,

	ESP_BLE_CONFIG_FINISH,
	
}ESP_BLE_CMD_STA;

typedef struct
{
	uint8_t TxBuff[ESP_DATA_MAX_LEN];		
	uint8_t RxBuff[1500];		

	uint16_t RxLen;
	uint8_t RxOver; 

	uint16_t TxLen;
	uint8_t TxOver;

}ESP_DATA_INTERACT_TYPE;

typedef struct
{
	uint16_t rxlen;

}ESP_QUEUE_RX_TYPE;


typedef enum{
	ESP_KEY_RESET= 0,
	ESP_KEY_EXIT_DATAMODE,		//退出透传
	ESP_KEY_AT,
	ESP_KEY_CWMODE,
	ESP_KEY_CWQAP,				//关闭 wifi
	ESP_KEY_CWJAP,				//连接新的 AP
	ESP_KEY_CONFIG_FINISH,		//完成
}ESP_KEY_CMD_STA;

typedef enum{
	ESP_STATION_EXIT_DATAMODE = 0,
	ESP_STATION_AT,
	ESP_STATION_ATE0,
	ESP_STATION_CIPSTATE,
	ESP_STATION_CWMODE,	
	ESP_STATION_CIPSERVER_CLOSE,
	ESP_STATION_CWJAP_CHECK,		
	ESP_STATION_CWJAP,	
	ESP_STATION_CIPSTART,
	ESP_STATION_CONFIG_FINISH,
}ESP_STATION_CMD_STA;


/**********************
 *  GLOBAL VARIABLES
 **********************/
extern ESP_DATA_INTERACT_TYPE g_InnerCOM2_Handle;

extern char g_pb_EspSoftwareVer[200];
extern uint8_t s_EspBleMac[13];

extern char WIFI_SSID[50];
extern char WIFI_PWD[50];


/**********************
 * GLOBAL PROTOTYPES
 **********************/
/**
 * init esp en pin,will reset it
 * @param null
 * @return null
 */
extern void api_esp_en_init_reset(void);
/**
 * esp ble init pull-up gpio-en, init uart2
 * @param null
 * @return null
 */
extern void api_esp_ble_init(void);
/**
 * esp ble irq func
 * @param null
 * @return null
 */
extern void api_esp_ble_func_irq(void);
/**
 * esp ble queue element num get
 * @param null
 * @return num
 */
extern uint16_t api_esp_ble_queue_element_num_get(void);
/**
 * trammist data to esp module
 * @param data the address of the data waiting for trammist
 * @param len the length of the data waiting for trammist
 * @return 0:send over time  1:send success
 */
extern uint8_t api_esp_data_tx(uint8_t* data, uint16_t len);
/**
 * receive data from esp module,
 * @param data the address of the data waiting for receive
 * @param time the time of wait,unit of ms
 * @return the bytes of receive data
 */
extern uint16_t api_esp_data_rx(uint8_t* data, uint16_t time);
/**
 * sent AT cmd to esp module
 * @param cmd cmd for send
 * @param ack expected replies
 * @param time time to wait for reply
 * @param reply replies received record,if reply equal null,do no record
 * @return return cmd send result
 */
extern ESP_ATCMD_ERROR_CODE_TYPE api_esp_cmd_tx(char* cmd, char* ack, uint16_t time, char* reply);
/**
 * esp ble config
 * @param null
 * @return the status of esp ble config step
 */
extern ESP_BLE_CMD_STA api_esp_ble_config(void);
/**
 * get esp ble power, adv scan and conn
 * @param buff to store three power param
 * @return true or false
 */
extern uint8_t api_esp_ble_power_get(uint32_t* wifi_power, uint32_t* adv_power, uint32_t* scan_power, uint32_t* conn_power);
/**
 * set esp ble power, adv scan and conn
 * @param buff to store three power param
 * @return true or false
 */
extern uint8_t api_esp_ble_power_set(uint8_t adv_power, uint8_t scan_power, uint8_t conn_power);
/**
 * esp ble mac address query
 * @param operate type get or write
 * @param name 
 * @return null
 */
extern uint8_t api_esp_ble_mac_query(char* mac);
/**
 * esp module exit direct data transmiss mode
 * @param null 
 * @return null
 */
extern void api_esp_ble_direct_mode_exit(void);
/**
 * esp module ble name set
 * @param null
 * @return fail code 
 */
extern uint8_t api_esp_ble_name_set(char* name);
/**
 * esp module adv open
 * @param null
 * @return fail code
 */
extern uint8_t api_esp_ble_adv_open(void);

extern uint8_t api_esp_key_connet_app_config(void);
extern uint8_t api_esp_wifi_status_check(char* wifi_ssid);
extern uint8_t api_esp_wifi_connet_server_config(void);
extern uint8_t api_esp_wifi_datamode_switch(bool en);


#endif
