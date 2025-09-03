/**
 * @file api_esp_wifi_ble.c
 *
 */

/**********************
 *      INCLUDES
 **********************/
#include <stdio.h>
#include <string.h>

#include "api_esp_tcp_ble.h"
#include "bsp_uart.h"
#include "gd32f4xx.h"
#include "crc16.h"
#include "tsk_spiflash.h"
#include "prj_config.h"


/**********************
 *      DEFINES
 **********************/
#define AT_CMD_WAIT_TIME	200
#define ESP_QUEUE_ELEMENT_MAX_NUM	50


/**********************
 *      TYPEDEFS
 **********************/




/**********************
 *  STATIC PROTOTYPES
 **********************/
static uint8_t api_ascii_convert(uint8_t num);
static void api_esp_advdata_combine(char* name, char* data);
void usart_dma_com2_rx_config(void);

/**********************
 *  STATIC VARIABLES
 **********************/
uint8_t s_EspBleMac[13] = {0};
//static char s_BleName[30] = {0};


/**********************
 *  GLOBAL VARIABLES
 **********************/
ESP_DATA_INTERACT_TYPE g_InnerCOM2_Handle;
QueueHandle_t s_bleRxQueue;

char g_pb_EspSoftwareVer[200];


/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * pull up the esp en pin
 * @param null
 * @return null
 */
void api_esp_en_high(void)
{
	gpio_bit_set(GPIOA, GPIO_PIN_5);
}

/**
 * pull down the esp en pin
 * @param null
 * @return null
 */
void api_esp_en_low(void)
{
	gpio_bit_reset(GPIOA, GPIO_PIN_5);
}

/**
 * init esp en pin,will reset it
 * @param null
 * @return null
 */
void api_esp_en_init_reset(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_5);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5);
	
	api_esp_en_low();
	
	vTaskDelay(50);
	
	api_esp_en_high();
}

/**
 * esp ble init pull-up gpio-en, init uart2
 * @param null
 * @return null
 */
void api_esp_ble_init(void)
{
	api_esp_en_init_reset();
	
	bsp_uart_init(COM2, 115200);
	usart_dma_com2_rx_config();
	
	if(NULL == s_bleRxQueue)
	{
		s_bleRxQueue = xQueueCreate(ESP_QUEUE_ELEMENT_MAX_NUM, sizeof(MALLOC_QUEUE_UINT_TYPE));	
	}
}

/**
 * esp ble irq func
 * @param null
 * @return null
 */
void api_esp_ble_func_irq(void)
{
	LOG_INFO("ble rec data len: %d val:0x%02X 0x%02X 0x%02X 0x%02X 0x%02X", g_InnerCOM2_Handle.RxLen, 
	g_InnerCOM2_Handle.RxBuff[0], g_InnerCOM2_Handle.RxBuff[1], g_InnerCOM2_Handle.RxBuff[2], g_InnerCOM2_Handle.RxBuff[3], 
	g_InnerCOM2_Handle.RxBuff[g_InnerCOM2_Handle.RxLen-1]);
	
	if(NULL == s_bleRxQueue)
		return;
	
	if(uxQueueSpacesAvailable(s_bleRxQueue) <= 0)
	{
		LOG_INFO("s_bleRxQueue no space: %d !!!", (int)uxQueueSpacesAvailable(s_bleRxQueue));
		return;
	}
	
	uint8_t* u_p_point = func_pvPortMalloc(g_InnerCOM2_Handle.RxLen);
	
	if(NULL == u_p_point)
	{
		LOG_INFO("ble func_pvPortMalloc failed !!!");
		return;
	}
	
	//push queue
	MALLOC_QUEUE_UINT_TYPE u_st_queue;
	u_st_queue.len = g_InnerCOM2_Handle.RxLen;
	u_st_queue.point = u_p_point;
	
	memcpy(u_st_queue.point, g_InnerCOM2_Handle.RxBuff, g_InnerCOM2_Handle.RxLen);
	
	if(pdFALSE == xQueueSend(s_bleRxQueue, &u_st_queue, 10))
	{
		func_pvPortFree(u_st_queue.point);
	}
}

/**
 * esp ble queue element num get
 * @param null
 * @return num
 */
uint16_t api_esp_ble_queue_element_num_get(void)
{
	return ESP_QUEUE_ELEMENT_MAX_NUM-uxQueueSpacesAvailable(s_bleRxQueue);
}

/**
 * trammist data to esp module
 * @param data the address of the data waiting for trammist
 * @param len the length of the data waiting for trammist
 * @return 0:send over time  1:send success
 */

void usart_dma_com2_config(void)
{
	dma_single_data_parameter_struct dma_init_struct;
	/* enable DMA0 */
	rcu_periph_clock_enable(RCU_DMA0);
	/* deinitialize DMA channe3(USART2 TX) */
	dma_deinit(DMA0, DMA_CH3);
	dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;
	dma_init_struct.memory0_addr = (uint32_t)g_InnerCOM2_Handle.TxBuff;//(uint32_t)tx_buffer;
	dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_init_struct.number = g_InnerCOM2_Handle.TxLen;//ARRAYNUM(tx_buffer);
	dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART2);//0x40004804;
	dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
	dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
	dma_single_data_mode_init(DMA0, DMA_CH3, &dma_init_struct);
	dma_channel_subperipheral_select(DMA0, DMA_CH3, DMA_SUBPERI4);
	/* configure DMA mode */
	dma_circulation_disable(DMA0, DMA_CH3);
	
	dma_channel_enable(DMA0, DMA_CH3);
}

void usart_dma_com2_rx_config(void)
{
	dma_single_data_parameter_struct dma_init_struct;
	/* enable DMA0 */
	rcu_periph_clock_enable(RCU_DMA0);
	/* deinitialize DMA channe3(USART2 TX) */
	dma_deinit(DMA0, DMA_CH1);
	dma_init_struct.direction = DMA_PERIPH_TO_MEMORY;
	dma_init_struct.memory0_addr = (uint32_t)g_InnerCOM2_Handle.RxBuff;//(uint32_t)tx_buffer;
	dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_init_struct.number = 1500;//g_InnerCOM2_Handle.TxLen;//ARRAYNUM(tx_buffer);
	dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART2);//0x40004804;
	dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
	dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
	dma_single_data_mode_init(DMA0, DMA_CH1, &dma_init_struct);
	dma_channel_subperipheral_select(DMA0, DMA_CH1, DMA_SUBPERI4);

	dma_channel_enable(DMA0, DMA_CH1);
	usart_dma_receive_config(COM2, USART_DENR_ENABLE);

}
void dma_rx_restart()
{
	dma_channel_disable(DMA0, DMA_CH1);
	
	dma_transfer_number_config(DMA0, DMA_CH1, 1500);
	dma_memory_address_config(DMA0, DMA_CH1, DMA_MEMORY_0, (uint32_t)g_InnerCOM2_Handle.RxBuff);
	
	dma_interrupt_flag_clear(DMA0, DMA_CH1, DMA_INT_FLAG_FTF);
	
	usart_dma_receive_config(COM2, USART_DENR_ENABLE);
	dma_channel_enable(DMA0, DMA_CH1);
}

#define DMA_SEND 1

uint8_t api_esp_data_tx(uint8_t* data, uint16_t len)
{
	uint8_t res = 0;

	if(len > ESP_TX_DATA_MAX_LEN) //长度超出单次发送长度 拆包发送
	{
		static uint16_t tx_index = 0;
		static uint16_t remain_tx_data_len = 0; //剩余待发送数据的长度

		remain_tx_data_len = len;

		do{
			//check whether the previous data was successfully sent
			for(uint16_t i = 0; i < ESP_TX_DATA_MAX_LEN/10; i++)
			{
				vTaskDelay(1);
				if(1 == g_InnerCOM2_Handle.TxOver)
				{
					res = 1;
					break;
				}
			}
			g_InnerCOM2_Handle.TxOver = 0;

			if(remain_tx_data_len >= ESP_TX_DATA_MAX_LEN)
			{
				g_InnerCOM2_Handle.TxLen = ESP_TX_DATA_MAX_LEN;
				memcpy(&g_InnerCOM2_Handle.TxBuff[0], &data[tx_index], ESP_TX_DATA_MAX_LEN);

				tx_index += ESP_TX_DATA_MAX_LEN;
				remain_tx_data_len -= ESP_TX_DATA_MAX_LEN;
			}
			else
			{
				g_InnerCOM2_Handle.TxLen = remain_tx_data_len;
				memcpy(&g_InnerCOM2_Handle.TxBuff[0], &data[tx_index], remain_tx_data_len);

				tx_index += remain_tx_data_len;
				remain_tx_data_len = 0;
			}

			//send data
//			usart_int_send(COM2);
			#ifdef DMA_SEND
				usart_dma_com2_config();			
				usart_dma_transmit_config(COM2, USART_DENT_ENABLE);
			#else
				usart_int_send(COM2);
			#endif
			
			vTaskDelay(200);

		}while(remain_tx_data_len!=0);
		
		tx_index = 0;
		remain_tx_data_len = 0;
	}
	else
	{
		//check whether the previous data was successfully sent
		for(uint16_t i = 0; i < ESP_TX_DATA_MAX_LEN/10; i++)
		{
			vTaskDelay(1);
			if(1 == g_InnerCOM2_Handle.TxOver)
			{
				res = 1;
				break;
			}
		}
		g_InnerCOM2_Handle.TxOver = 0;
		
		g_InnerCOM2_Handle.TxLen = len;
		memcpy(&g_InnerCOM2_Handle.TxBuff, data, len);
//		usart_int_send(COM2);
		#ifdef DMA_SEND
			usart_dma_com2_config();			
			usart_dma_transmit_config(COM2, USART_DENT_ENABLE);
		#else
			usart_int_send(COM2);
		#endif	
	}

	return res;
}

uint8_t api_esp_data_tx2(uint8_t* data, uint16_t len)
{
	uint8_t res = 0;
	
	//check whether the previous data was successfully sent
	for(uint16_t i = 0; i < ESP_DATA_MAX_LEN/10; i++)
	{
		if(1 == g_InnerCOM2_Handle.TxOver)
		{
			res = 1;
			break;
		}
		vTaskDelay(1);
	}
	
	g_InnerCOM2_Handle.TxOver = 0;
	g_InnerCOM2_Handle.TxLen = len;
	memcpy(&g_InnerCOM2_Handle.TxBuff, data, len);

	usart_int_send(COM2);

	return res;
}

/**
 * receive data from esp module,
 * @param data the address of the data waiting for receive
 * @param time the time of wait,unit of ms
 * @return the bytes of receive data
 */
uint16_t api_esp_data_rx(uint8_t* data, uint16_t time)
{
	uint16_t res = 0;
	
	if(NULL != s_bleRxQueue)
	{
		MALLOC_QUEUE_UINT_TYPE u_queue;
		
		if(pdTRUE == xQueueReceive(s_bleRxQueue, &u_queue, time))
		{
			memcpy(data, u_queue.point, u_queue.len);
			
			func_pvPortFree(u_queue.point);
			
			res = u_queue.len;
			
			dma_rx_restart();
			
//			LOG_INFO("api_esp_data_rx: %s", data);
		}
	}

	return res;
}

/**
 * sent AT cmd to esp module
 * @param cmd cmd for send
 * @param ack expected replies
 * @param time time to wait for reply
 * @param reply replies received record,if reply equal null,do no record
 * @return return cmd send result
 */
ESP_ATCMD_ERROR_CODE_TYPE api_esp_cmd_tx(char* cmd, char* ack, uint16_t time, char* reply)
{
	//LOG_INFO("api_esp_cmd_tx: %s", cmd);
	
	ESP_ATCMD_ERROR_CODE_TYPE res = ESP_AT_ERROR_NULL;
	
	uint8_t data_rx[200] = {0};
	
	//clear uart buff before send cmd
	while(0 != api_esp_data_rx((uint8_t*)data_rx, 0));
	
	api_esp_data_tx((uint8_t*)cmd, strlen((const char *)cmd));

	uint16_t data_rxlen = 0;
	
	data_rxlen = api_esp_data_rx(data_rx, time);
	if(0 != data_rxlen)
	{
		if(strstr((char*)data_rx, ack))
		{
			res = ESP_AT_ERROR_SUCCESS;
			
			if(reply)
			{
				memcpy(reply, data_rx, data_rxlen);
			}
		}
		else
		{
			res = ESP_AT_ERROR_REPLY_CHECK_FAILED;
		}
	}
	else
	{
		res = ESP_AT_ERROR_NO_REPLY;
	}
	
	return res;
}
uint8_t _ble_advdata[30] = {0};
uint8_t _mac_addr[10] = {0};
/**
 * esp ble config
 * @param null
 * @return the status of esp ble config step
 */
ESP_BLE_CMD_STA api_esp_ble_config(void)
{
	ESP_BLE_CMD_STA res = ESP_BLE_AT;
	
	static ESP_BLE_CMD_STA s_ble_sta = ESP_BLE_AT;

	char u_buff[200] = {0};
	
	switch(s_ble_sta)
	{
		case ESP_BLE_AT:
		{
			if(ESP_AT_ERROR_SUCCESS == api_esp_cmd_tx("AT\r\n", "OK", AT_CMD_WAIT_TIME, NULL))
			{
				s_ble_sta = ESP_BLE_ATE0;
			}
		}
		break;
		
		case ESP_BLE_ATE0:
		{
			if(ESP_AT_ERROR_SUCCESS == api_esp_cmd_tx("ATE0\r\n", "OK", AT_CMD_WAIT_TIME, NULL))
			{
				s_ble_sta = ESP_BLE_BAUD_CHANGED; 
			}
		}
		break;
	
		case ESP_BLE_BAUD_CHANGED:
		{
			if(ESP_AT_ERROR_SUCCESS == api_esp_cmd_tx("AT+UART_CUR=800000,8,1,0,0\r\n", "OK", AT_CMD_WAIT_TIME, NULL))
			{
				s_ble_sta = ESP_BLE_GMR; 
				bsp_uart_init(COM2, 800000);
				
				usart_dma_com2_rx_config();
				dma_rx_restart();
			}
		}
		break;
		
		case ESP_BLE_GMR:
		{
			if(ESP_AT_ERROR_SUCCESS == api_esp_cmd_tx("AT+GMR\r\n", "OK", AT_CMD_WAIT_TIME, g_pb_EspSoftwareVer))
			{
				s_ble_sta = ESP_BLE_CLOSE_WIFI; 
			}
		}
		break;
		
		case ESP_BLE_CLOSE_WIFI: 
		{
			if(ESP_AT_ERROR_SUCCESS == api_esp_cmd_tx("AT+CWMODE=0\r\n", "OK", AT_CMD_WAIT_TIME, NULL))
			{
				s_ble_sta = ESP_BLE_BLEINIT; 
			}
		}
		break;

		case ESP_BLE_BLEINIT:
		{
			if(ESP_AT_ERROR_SUCCESS == api_esp_cmd_tx("AT+BLEINIT=2\r\n", "OK", AT_CMD_WAIT_TIME, NULL))
			{
				s_ble_sta = ESP_BLE_BLEGATTSSRVCRE;
			}
		}
		break;
		
		case ESP_BLE_BLEGATTSSRVCRE:
		{
			if(ESP_AT_ERROR_SUCCESS == api_esp_cmd_tx("AT+BLEGATTSSRV?\r\n", "+BLEGATTSSRV", AT_CMD_WAIT_TIME, NULL))
			{
				s_ble_sta = ESP_BLE_BLEGATTSSRVSTART; 
			}	
			else
			{
				if(ESP_AT_ERROR_SUCCESS == api_esp_cmd_tx("AT+BLEGATTSSRVCRE\r\n", "OK", AT_CMD_WAIT_TIME, NULL))
				{
					s_ble_sta = ESP_BLE_BLEGATTSSRVSTART;
				}
			}
			
//			if(ESP_AT_ERROR_SUCCESS == api_esp_cmd_tx("AT+BLEGATTSSRV?\r\n", "ERROR", AT_CMD_WAIT_TIME, NULL)) 
//			{
//				if(ESP_AT_ERROR_SUCCESS == api_esp_cmd_tx("AT+BLEGATTSSRVCRE\r\n", "OK", AT_CMD_WAIT_TIME, NULL))
//				{
//					s_ble_sta = ESP_BLE_BLEGATTSSRVSTART;
//				}
//			}
//			else
//			{
//				s_ble_sta = ESP_BLE_BLEGATTSSRVSTART; 
//			}
		}
		break;
		
        case ESP_BLE_BLEGATTSSRVSTART:
		{
			if(ESP_AT_ERROR_SUCCESS == api_esp_cmd_tx("AT+BLEGATTSSRVSTART\r\n", "OK", AT_CMD_WAIT_TIME, NULL))
			{
				s_ble_sta = ESP_BLE_BLEADDR; 
			}
		}
		break;
		
		case ESP_BLE_BLEADDR:
		{
			if(ESP_AT_ERROR_SUCCESS == api_esp_cmd_tx("AT+BLEADDR?\r\n", "BLEADDR", AT_CMD_WAIT_TIME, u_buff))
			{
				s_ble_sta = ESP_BLE_SET_PARAMETER;
				
				char* u_str = NULL;
				u_str = strstr((char*)u_buff, "+BLEADDR"); 
				
				for(uint8_t i = 0,j = 10; i < 12; i++)
				{
					s_EspBleMac[i] = u_str[j];
					if(1 == (i % 2))
					{
						j += 2;
					}
					else
					{
						j += 1;
					}
				}
				s_EspBleMac[12] = '\0';
				
				for(uint8_t i=0; i<6; i++)
				{
					_mac_addr[i] = chars_to_hex(s_EspBleMac[2*i], s_EspBleMac[2*i+1]);
				}
			}
		}
		break;
		
		case ESP_BLE_SET_PARAMETER:
		{
			if(ESP_AT_ERROR_SUCCESS == api_esp_cmd_tx("AT+BLEADVPARAM=50,50,0,0,7,0,,\r\n", "OK", AT_CMD_WAIT_TIME, NULL))
			{
				s_ble_sta = ESP_BLE_BLEADVDATA;
//				s_ble_sta = ESP_BLE_CONFIG_SYSMSG;
			}
		}
		break;
		
		//20240716
		case ESP_BLE_BLEADVDATA:
		{
			
			_ble_advdata[0] = 0x13; //data len
			_ble_advdata[1] = 0xff;	//broadcast type
			
			_ble_advdata[2] = (uint8_t)(DEVICE_ID_LOCAL>>16);
			_ble_advdata[3] = (uint8_t)(DEVICE_ID_LOCAL>>8);
			_ble_advdata[4] = (uint8_t)DEVICE_ID_LOCAL;
			
			_ble_advdata[5]	= 0x22;
			_ble_advdata[6] = 0x00;
			_ble_advdata[7] = 0x01;
			_ble_advdata[8] = 0x06;		//小屏软件项目
			_ble_advdata[9]  = 0x42;		//项目标识	420100
			_ble_advdata[10] = 0x01;
			_ble_advdata[11] = 0x00;		
			_ble_advdata[12] = _mac_addr[0];
			_ble_advdata[13] = _mac_addr[1];
			_ble_advdata[14] = _mac_addr[2];
			_ble_advdata[15] = _mac_addr[3];
			_ble_advdata[16] = _mac_addr[4];
			_ble_advdata[17] = _mac_addr[5];
			uint16_t crc = CRC16(&_ble_advdata[2], 16);
			_ble_advdata[18] = crc;
			_ble_advdata[19] = crc>>8;

/////
			uint16_t u_namelen = 20;
			char u_buff[100] = {0};

			for(uint16_t i = 0; i < u_namelen; i++)
			{
				u_buff[2 * i] = api_ascii_convert(_ble_advdata[i] >> 4);
				u_buff[2 * i + 1] = api_ascii_convert(_ble_advdata[i] & 0xf);
			}

			u_buff[2 * u_namelen] = '\0';
//////
			char _broadcast[80] = {0};
			sprintf(_broadcast, "%s%s%s", "AT+BLESCANRSPDATA=\"", u_buff, "\"\r\n");
			
			
			if(ESP_AT_ERROR_SUCCESS == api_esp_cmd_tx(_broadcast, "OK", AT_CMD_WAIT_TIME, NULL))
			{
				s_ble_sta = ESP_BLE_CONFIG_SYSMSG;
			}
		
			///"AT+BLESCANRSPDATA=\"13FFC0010022000106420100010203040506E0C8\"\r\n"
		}
		break;
		
//		case ESP_BLE_BLEADVDATA:
//		{
//			api_esp_advdata_combine(s_BleName, u_buff);

//			if(ESP_AT_ERROR_SUCCESS == api_esp_cmd_tx(u_buff, "OK", AT_CMD_WAIT_TIME, NULL))
//			{
//				sprintf(u_buff, "%s%s%s", "AT+BLENAME=\"", s_BleName, "\"\r\n");
//				api_esp_cmd_tx(u_buff, "OK", AT_CMD_WAIT_TIME, NULL);
//				
//				s_ble_sta = ESP_BLE_BLEADVSTART;
//			}
//		}
//		break;

//        case ESP_BLE_BLEADVSTART:
//		{
//			if(ESP_AT_ERROR_SUCCESS == api_esp_cmd_tx("AT+BLEADVSTART\r\n", "OK", AT_CMD_WAIT_TIME, NULL))
//			{
//				s_ble_sta = ESP_BLE_CONFIG_SYSMSG;
//			}
//		}
//		break;
		
		case ESP_BLE_CONFIG_SYSMSG:
		{
			if(ESP_AT_ERROR_SUCCESS == api_esp_cmd_tx("AT+SYSMSG=4\r\n", "OK", AT_CMD_WAIT_TIME, NULL))
			{
				s_ble_sta = ESP_BLE_CONFIG_SPP;
			}
		}
		break;

		case ESP_BLE_CONFIG_SPP:
		{
			if(ESP_AT_ERROR_SUCCESS == api_esp_cmd_tx("AT+BLESPPCFG=1,1,6,1,5,0\r\n", "OK", AT_CMD_WAIT_TIME, NULL))
			{
				s_ble_sta = ESP_BLE_CONFIG_FINISH;
			}
		}
		break;

		case ESP_BLE_CONFIG_FINISH:
		{
			s_ble_sta = ESP_BLE_AT;
		}
		break;
		
		default:
		{
		
		}
		break;	
    }
	
	res = s_ble_sta;
	
	return res;	
}

/**
 * get esp ble power, adv scan and conn
 * @param buff to store three power param
 * @return true or false
 */
uint8_t api_esp_ble_power_get(uint32_t* wifi_power, uint32_t* adv_power, uint32_t* scan_power, uint32_t* conn_power)
{
	uint8_t u_b_status = false;
	
	char* u_pb_buff = func_cacheGet(CACHE_SHARE_SIZE_500);
	
	if(NULL == u_pb_buff)
	{
		LOG_INFO("api_esp_ble_power_get func_cacheGet failed!!!");
		return ESP_AT_ERROR_NULL;
	}
	
	if(ESP_AT_ERROR_SUCCESS == api_esp_cmd_tx("AT+RFPOWER?\r\n", "OK", AT_CMD_WAIT_TIME, u_pb_buff))
	{
		sscanf(u_pb_buff, "+RFPOWER:%d,%d,%d,%d", wifi_power, adv_power, scan_power, conn_power);
		u_b_status = true;
	}
	
	func_cacheFree(CACHE_SHARE_SIZE_500);
	
	return u_b_status;
}


/**
 * set esp ble power, adv scan and conn
 * @param buff to store three power param
 * @return true or false
 */
uint8_t api_esp_ble_power_set(uint8_t adv_power, uint8_t scan_power, uint8_t conn_power)
{
	uint8_t u_b_status = false;
	
	char* u_pb_buff = func_cacheGet(CACHE_SHARE_SIZE_500);
	
	if(NULL == u_pb_buff)
	{
		LOG_INFO("api_esp_ble_power_set func_cacheGet failed!!!");
		return ESP_AT_ERROR_NULL;
	}
	
	sprintf(u_pb_buff, "AT+RFPOWER=%d,%d,%d,%d\r\n", 78, adv_power, scan_power, conn_power);
	
	if(ESP_AT_ERROR_SUCCESS == api_esp_cmd_tx("AT+CWMODE=1\r\n", "OK", AT_CMD_WAIT_TIME, NULL))
	{
		
	}
	else
	{
		return false;
	}
	
	if(ESP_AT_ERROR_SUCCESS == api_esp_cmd_tx(u_pb_buff, "OK", AT_CMD_WAIT_TIME, NULL))
	{
		u_b_status = true;
	}
	
	api_esp_cmd_tx("AT+BLEADVSTART\r\n", "OK", AT_CMD_WAIT_TIME, NULL);
	
	if(ESP_AT_ERROR_SUCCESS == api_esp_cmd_tx("AT+CWMODE=0\r\n", "OK", AT_CMD_WAIT_TIME, NULL))
	{
		
	}
	else
	{
		return false;
	}
	
	func_cacheFree(CACHE_SHARE_SIZE_500);

	return u_b_status;
}

/**
 * esp ble mac address query
 * @param operate type get or write
 * @param name 
 * @return null
 */
uint8_t api_esp_ble_mac_query(char* mac)
{
	if(0 == strlen((char*)s_EspBleMac))
	{
		return false;
	}
	
	memcpy(mac, s_EspBleMac, sizeof(s_EspBleMac));
	
	return true;
}

/**
 * esp module exit direct data transmiss mode
 * @param null 
 * @return null
 */
void api_esp_ble_direct_mode_exit(void)
{
	vTaskDelay(1000);
	
	api_esp_data_tx((uint8_t*)"+++", 3);
	
	vTaskDelay(1200);
}

/**
 * esp module ble name set
 * @param null
 * @return fail code 
 */
uint8_t api_esp_ble_name_set(char* name)
{
	uint8_t u_name_len = strlen(name);
	
	if(0 == u_name_len)
	{
		return 1;
	}
	
	if(u_name_len > BLE_NAME_MAX_LENGTH)
	{
		name[BLE_NAME_MAX_LENGTH] = 0;
	}
	
	char u_buff[200] = {0};
	api_esp_advdata_combine(name, u_buff);
	
	if(ESP_AT_ERROR_SUCCESS == api_esp_cmd_tx(u_buff, "OK", AT_CMD_WAIT_TIME, NULL))
	{
		if(api_esp_cmd_tx("AT+BLEADVSTART\r\n", "OK", AT_CMD_WAIT_TIME, NULL))
		{
			return 0;
		}
	}
	
	return 2;
}

/**
 * esp module adv open
 * @param null
 * @return fail code
 */
uint8_t api_esp_ble_adv_open(void)
{
	LOG_INFO("Try Open BLEADVSTART---");
	
	if(ESP_AT_ERROR_SUCCESS == api_esp_cmd_tx("AT+BLEADVSTART\r\n", "OK", AT_CMD_WAIT_TIME, NULL))
	{
		LOG_INFO("BLE Start BLEADVSTART!!!");
		return 0;
	}
	
	return 1;
}


/**
 * generate ble name and code
 * @param name to store ble name
 * @param code to store ble code
 * @return null
 */
void api_esp_ble_name_code_generate(char* name, char* code)
{
	uint8_t u_prefix_len = strlen(BLE_NAME_HEAD_FIELD);
	
	if((9 - u_prefix_len) < 0)
	{
		sprintf(name, "%s%s", BLE_NAME_HEAD_FIELD, &s_EspBleMac[u_prefix_len - 9]);
	}
	else
	{
		sprintf(name, "%s%s", BLE_NAME_HEAD_FIELD, &s_EspBleMac[0]);
	}
	
	uint32_t u_dw_rand_mac = 0;
	for(uint8_t i = 0; i < 12; i++)
	{
		u_dw_rand_mac += s_EspBleMac[i];
	}
	
	u_dw_rand_mac = func_rand_number_generate(u_dw_rand_mac);
	
	code[0] = ((u_dw_rand_mac / 1000 % 10) + 0x30);
	code[1] = ((u_dw_rand_mac / 100 % 10) + 0x30);
	code[2] = ((u_dw_rand_mac / 10 % 10) + 0x30);
	code[3] = ((u_dw_rand_mac / 1 % 10) + 0x30);
}


/**********************
 *   STATIC FUNCTIONS
 **********************/
/**
 * Convert numbers to ascii codes
 * @param num
 * @return Converted value
 */
static uint8_t api_ascii_convert(uint8_t num)
{
	uint8_t res = 0;
	if(num > 9)
	{
		res = num + 0x37;
	}
	else
	{
		res = num + 0x30;
	}
	
	return res;
}

/**
 * esp ble broadcast data combine
 * @param name ble name
 * @param data the address of data array receive data after framing
 * @return null
 */
static void api_esp_advdata_combine(char* name, char* data)
{
	uint16_t u_namelen = strlen(name);

	char u_buff[100] = {0};

	for(uint16_t i = 0; i < u_namelen; i++)
	{
		u_buff[2 * i] = api_ascii_convert(name[i] >> 4);
		u_buff[2 * i + 1] = api_ascii_convert(name[i] & 0xf);
	}

	u_buff[2 * u_namelen] = '\0';

	sprintf(data, "%s%c%c%s%s%s", "AT+BLEADVDATA=\"020106", api_ascii_convert((u_namelen+1) >> 4), 
			api_ascii_convert((u_namelen+1) & 0xf), "09", u_buff , "030302A0\"\r\n");
}



//wifi配置
static char cmd_reply_data[100] = {0};  	 			/* 命令回复数据 */
char WIFI_SSID[50] = "Test room";   		 		/* wifi的ssid 名称 */
char WIFI_PWD[50]  = "Aa123456";  	  			/* wifi的pwd 密码 */

uint8_t api_esp_wifi_judge_switch(bool en)
{
	uint8_t rev;
	
	
	if(true == en) 
	{
		if(api_esp_cmd_tx("AT+CIPMODE?\r\n", "+CIPMODE:0", AT_CMD_WAIT_TIME, cmd_reply_data))  /* 先查询当前传输模式 */
		{
			if(api_esp_cmd_tx("AT+CIPMODE=1\r\n", "OK", AT_CMD_WAIT_TIME, cmd_reply_data))  /* 当前为普通传输模式 */
			{
				if(api_esp_cmd_tx("AT+CIPSEND\r\n", ">", AT_CMD_WAIT_TIME, cmd_reply_data))  /* 开启数据透传 */
				{
					rev = 1;
				}
				else
				{
					rev = 0;
				}
			}
			else
			{
				rev = 0;
			}
		}
		else
		{
			/* 当前为透传模式 */
			if(api_esp_cmd_tx("AT+CIPSEND\r\n", ">", AT_CMD_WAIT_TIME, cmd_reply_data))  /* 开启数据透传 */
			{
				rev = 1;
			}
			else
			{
				rev = 0;
			}
		}
	}
	else if(false == en) 
	{
		vTaskDelay(20);  /* 发送+++命令前后 都要有一定时间延时 */
		api_esp_cmd_tx("+++", "", 0, cmd_reply_data);
		vTaskDelay(20);
		
		vTaskDelay(1500);
		api_esp_cmd_tx("AT\r\n", "OK", AT_CMD_WAIT_TIME, cmd_reply_data);  /* 实测 发送+++后的第一条指令可能会失败，挡掉 */
		api_esp_cmd_tx("AT\r\n", "OK", AT_CMD_WAIT_TIME, cmd_reply_data); 
		if(api_esp_cmd_tx("AT+CIPMODE?\r\n", "+CIPMODE:1", AT_CMD_WAIT_TIME, cmd_reply_data))
		{
			rev = 1;
		}
		else
		{
			api_esp_cmd_tx("AT+CIPMODE=1\r\n", "OK", AT_CMD_WAIT_TIME, cmd_reply_data);
			rev = 0;  /* 已经处于关闭状态 */
		}
	}
	return rev;	
}

uint8_t api_esp_key_connet_app_config(void)
{
	static ESP_KEY_CMD_STA key_config_status = ESP_KEY_EXIT_DATAMODE;
	char cmd_temp[80] = {0};
	static uint8_t b_key_at_err_num = 0;		//AT 指令错误，可能 ESP32 不回应
	
	switch (key_config_status)
	{	
		case ESP_KEY_RESET:
		{	
			usart_receive_config(COM2, USART_RECEIVE_ENABLE);
			
			key_config_status = ESP_KEY_EXIT_DATAMODE;
		}
		
		case ESP_KEY_EXIT_DATAMODE:
		{
			if(b_key_at_err_num >= 3)
			{
				key_config_status = ESP_KEY_RESET;
			}

			if(api_esp_wifi_judge_switch(false))
			{
				key_config_status = ESP_KEY_AT;
				b_key_at_err_num = 0;
			}
			else
			{
				b_key_at_err_num++;
			}
		}
		break;
		case ESP_KEY_AT:
		{
			if(b_key_at_err_num >= 3)
			{
				key_config_status = ESP_KEY_RESET;
			}
			
			if(api_esp_cmd_tx("AT\r\n", "OK", AT_CMD_WAIT_TIME, cmd_reply_data))
			{
				key_config_status = ESP_KEY_CWQAP; 
				b_key_at_err_num = 0;
			}
			else
			{
				b_key_at_err_num++;
			}
		}
		break;
		
		case ESP_KEY_CWQAP:
		{
			if(b_key_at_err_num >= 3)
			{
				key_config_status = ESP_KEY_CWMODE;//ESP_KEY_RESET;
			}
			
			if(api_esp_cmd_tx("AT+CWQAP\r\n", "OK", AT_CMD_WAIT_TIME, cmd_reply_data))
			{
				key_config_status = ESP_KEY_CWMODE; 
				b_key_at_err_num = 0;
			}
			else
			{
				b_key_at_err_num++;
			}
			
		}
		break;
		
    case ESP_KEY_CWMODE:
		{
			if(b_key_at_err_num >= 3)
			{
				key_config_status = ESP_KEY_RESET;
			}
			
			if(api_esp_cmd_tx("AT+CWMODE=1\r\n", "OK", AT_CMD_WAIT_TIME, cmd_reply_data))
			{
				key_config_status = ESP_KEY_CWJAP;
				b_key_at_err_num = 0;
			}
			else
			{
				b_key_at_err_num++;
			}
		}
		break;
		
		case ESP_KEY_CWJAP:
		{
			//if((0 != strlen(WIFI_SSID)) && (0 != strlen(WIFI_PWD)))
			{	
				sprintf(cmd_temp, "%s%s%s%s%s%s%s", "AT+CWJAP=", "\"", WIFI_SSID, "\",", "\"", WIFI_PWD,"\"\r\n");
				if(api_esp_cmd_tx(cmd_temp, "CONNECT", 500, cmd_reply_data))
				{	
					key_config_status = ESP_KEY_CONFIG_FINISH;
					b_key_at_err_num = 0;

				}
			}
		}
		break;
		
		
		default:
			break;	
    }
	if(ESP_KEY_CONFIG_FINISH == key_config_status) 
	{
		key_config_status = ESP_KEY_EXIT_DATAMODE;
		b_key_at_err_num = 0;
		return 1;
	}
	else
		return 0;
}

uint8_t api_esp_wifi_status_check(char* wifi_ssid)
{
	uint8_t retval = 0;
	
	if(api_esp_cmd_tx("AT+CWSTATE?\r\n", "+CWSTATE:", AT_CMD_WAIT_TIME, cmd_reply_data))
	{
		switch(cmd_reply_data[9] - 0x30)
		{
			case 0:
			{
				memset(wifi_ssid, 0, 20);
				retval = 0;		/* wifi 未配置过 */
			}
			break;
			
			case 1:
			case 2:
			{
				for(uint8_t i = 0; i < 20; i++)
				{
					if('"' == cmd_reply_data[12 + i]) break;
					
					wifi_ssid[i] = cmd_reply_data[12 + i];
				}
				
				retval = 2;		/* wifi 连接正常 */
			}
			break;
			
			case 3:
			case 4:
			{
				for(uint8_t i = 0; i < 20; i++)
				{
					if('"' == cmd_reply_data[12 + i]) break;
					
					wifi_ssid[i] = cmd_reply_data[12 + i];
				}
				
				retval = 1;		/* wifi已配置过，但未连接 */
			}
			break;
			
			default:
			{
			
			}
			break;
		}
	
	}
	
	return retval;
}

const uint8_t SERVER_IP[20] = "8.210.132.188";  		/* 公司服务器IP地址 */
const uint8_t PORT_NUM[10]  = "10001";          		/* 公司服务器的端口号 */

uint8_t api_esp_wifi_connet_server_config(void)
{
    static ESP_STATION_CMD_STA wifi_config_status = ESP_STATION_EXIT_DATAMODE;
	char cmd_temp[80] = {0};

    switch (wifi_config_status)
    {
		case ESP_STATION_EXIT_DATAMODE:
		{
			if(api_esp_wifi_datamode_switch(false))
			{
				wifi_config_status = ESP_STATION_AT;
			}
		}
		break;
		
		case ESP_STATION_AT:
		{
			if(api_esp_cmd_tx("AT\r\n", "OK", AT_CMD_WAIT_TIME, cmd_reply_data))
			{
				wifi_config_status = ESP_STATION_ATE0;
			}
		}
		break;
		
		case ESP_STATION_ATE0:
		{
			if(api_esp_cmd_tx("ATE0\r\n", "OK", AT_CMD_WAIT_TIME, cmd_reply_data))
			{
				wifi_config_status = ESP_STATION_CIPSTATE;
			}
		}
		break;
		
		case ESP_STATION_CIPSTATE:
		{
			if(api_esp_cmd_tx("AT+CIPSTATE?\r\n", "\"8.210.132.188\",10001", AT_CMD_WAIT_TIME, cmd_reply_data))
			{
				wifi_config_status = ESP_STATION_CONFIG_FINISH;
			}
			else
			{
				api_esp_cmd_tx("AT+CIPCLOSE\r\n", "CLOSED", AT_CMD_WAIT_TIME, cmd_reply_data);
				wifi_config_status = ESP_STATION_CWMODE;
			}
		}
		break;
		
        case ESP_STATION_CWMODE:
		{
			if(api_esp_cmd_tx("AT+CWMODE=1\r\n", "OK", AT_CMD_WAIT_TIME, cmd_reply_data))
			{
				wifi_config_status = ESP_STATION_CIPSERVER_CLOSE;
			}
		}
		break;
		
		case ESP_STATION_CIPSERVER_CLOSE:
		{
			/* 预防配网创建服务器还存在 */
			api_esp_cmd_tx("AT+CIPSERVER=0,1\r\n", "OK", AT_CMD_WAIT_TIME, cmd_reply_data);
			api_esp_cmd_tx("AT+CIPMUX=0\r\n", "OK", AT_CMD_WAIT_TIME, cmd_reply_data);
			
			wifi_config_status = ESP_STATION_CWJAP_CHECK; 
		}
		break;
		
		case ESP_STATION_CWJAP_CHECK:
		{
			switch(api_esp_wifi_status_check(WIFI_SSID))
			{
				case 0:
				{
					wifi_config_status = ESP_STATION_CWJAP; 
				}
				break;
				
				case 1:
				{
					if(api_esp_cmd_tx("AT+CWJAP\r\n", "OK", AT_CMD_WAIT_TIME, cmd_reply_data))
					{
						wifi_config_status = ESP_STATION_CIPSTART;  
					}
				}
				break;
				
				case 2:
				{
					wifi_config_status = ESP_STATION_CIPSTART;
				}
				break;
			}
		}
		break;

        case ESP_STATION_CWJAP:
		{
			if((0 == strlen(WIFI_SSID)) || (0 == strlen(WIFI_PWD)))
			{
				wifi_config_status = ESP_STATION_CWJAP_CHECK;
			}
			else
			{
				sprintf(cmd_temp, "%s%s%s%s%s%s%s", "AT+CWJAP=", "\"", WIFI_SSID, "\",", "\"", WIFI_PWD,"\"\r\n");
				if(api_esp_cmd_tx(cmd_temp, "CONNECTED", AT_CMD_WAIT_TIME, cmd_reply_data))
				{
					wifi_config_status = ESP_STATION_CIPSTART;
				}
				else
				{
					wifi_config_status = ESP_STATION_CWJAP_CHECK;
				}
			}
		}
		break;
		
		
		case ESP_STATION_CIPSTART:
		{
			sprintf(cmd_temp, "%s%s%s%s%s%s", "AT+CIPSTART=\"TCP\",", "\"", SERVER_IP, "\",", PORT_NUM,"\r\n");

			if(api_esp_cmd_tx(cmd_temp, "CONNECT", AT_CMD_WAIT_TIME, cmd_reply_data))
			{
				wifi_config_status = ESP_STATION_CONFIG_FINISH;
			}
			else
			{
				wifi_config_status = ESP_STATION_CIPSTATE;
			}
		}
		break;
		
		default:
			break;	
    }
	if(ESP_STATION_CONFIG_FINISH == wifi_config_status)
	{
		wifi_config_status = ESP_STATION_EXIT_DATAMODE;
		return 1;
	}		
	else 
		return 0;
}
uint8_t api_esp_wifi_datamode_switch(bool en)
{
	uint8_t rev;
	
	/* 为什么这边发送+++后面指令失败 */
//	delay_1ms(50);  /* 发送+++命令前后 都要有一定时间延时 */
//	api_esp_cmd_tx("+++", "", 0, cmd_reply_data);
//	delay_1ms(50);
	
	if(true == en) 
	{
		if(api_esp_cmd_tx("AT+CIPMODE?\r\n", "+CIPMODE:0", AT_CMD_WAIT_TIME, cmd_reply_data))  /* 先查询当前传输模式 */
		{
			if(api_esp_cmd_tx("AT+CIPMODE=1\r\n", "OK", AT_CMD_WAIT_TIME, cmd_reply_data))  /* 当前为普通传输模式 */
			{
				if(api_esp_cmd_tx("AT+CIPSEND\r\n", "OK", AT_CMD_WAIT_TIME, cmd_reply_data))  /* 开启数据透传 */
				{
					rev = 1;
				}
				else
				{
					rev = 0;
				}
			}
			else
			{
				rev = 0;
			}
		}
		else
		{
			/* 当前为透传模式 */
			if(api_esp_cmd_tx("AT+CIPSEND\r\n", "OK", AT_CMD_WAIT_TIME, cmd_reply_data))  /* 开启数据透传 */
			{
				rev = 1;
			}
			else
			{
				rev = 0;
			}
		}
	}
	else if(false == en) 
	{
		vTaskDelay(50);  /* 发送+++命令前后 都要有一定时间延时 */
		api_esp_cmd_tx("+++", "", 0, cmd_reply_data);
		vTaskDelay(50);
		api_esp_cmd_tx("AT\r\n", "OK", AT_CMD_WAIT_TIME, cmd_reply_data);  /* 实测 发送+++后的第一条指令可能会失败，挡掉 */
		api_esp_cmd_tx("AT\r\n", "OK", AT_CMD_WAIT_TIME, cmd_reply_data); 
		if(api_esp_cmd_tx("AT+CIPMODE?\r\n", "+CIPMODE:1", AT_CMD_WAIT_TIME, cmd_reply_data))
		{
			if(api_esp_cmd_tx("AT+CIPMODE=0\r\n", "OK", AT_CMD_WAIT_TIME, cmd_reply_data))
			{
				rev = 1;
			}
			else
			{
				rev = 0;
			}
		}
		else
		{
			rev = 1;  /* 已经处于关闭状态 */
		}
	}
	return rev;	
}

