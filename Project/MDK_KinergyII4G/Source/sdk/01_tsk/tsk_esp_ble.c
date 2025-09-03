/**
 * @file tsk_esp_ble.c
 *
 */

/**********************
 *      INCLUDES
 **********************/
#include <string.h> 
 
#include "tsk_esp_ble.h"

#include "api_wcm.h"
#include "bsp_rtc.h"
#include "tsk_spiflash.h"
#include "tsk_upg.h"

#include "wcm_data_protocol.h"
#include "time.h"

#include "tsk_can.h"
#include "tsk_prj_process.h"
#include "system_time_manage.h"

/**********************
 *      DEFINES
 **********************/
#define BLE_TASK_DELAY				5
#define BLE_LOST_JUDGE_TIME			15000	//15s

#define BLE_CMD_REPLY_WAIT_TIME		300

/**********************
 *      TYPEDEFS
 **********************/


/**********************
 *  STATIC PROTOTYPES
 **********************/
static void wcm_data_process(WCM_FRAME_CONSIST_TYPE* wcm_frame);
static uint16_t fleet_data_process(uint8_t* data, uint16_t len, uint8_t* buff);
//static void fleet_broadcase_check(uint8_t* buff);
static void ble_data_receive_process(void);
static void ble_data_decode(uint8_t* data, uint16_t len);
void time_stamp_rtc_sync(uint32_t sync_stamp, uint8_t set_source);
void system_para_process(uint16_t index, uint16_t data);
static void ble_wcm_data_send(
								WCM_FRAME_CONSIST_TYPE* wcm_frame,
								WCM_CMD_INDEX_TYPE cmd, 
								uint8_t* const data, 
								uint16_t txlen
						);

/**********************
 *  STATIC VARIABLES
 **********************/
BLE_TASK_WORK_STATUS_TYPE s_BleSta = BLE_NULL;
static uint16_t s_BleLostedCnt = 0;
								
static uint8_t s_bleReplyBuff[ESP_DATA_MAX_LEN];
								
uint8_t s_BleAdvOpenFailed = 0;
uint8_t g_BleTestResult = 0;
/**********************
 *  GLOBAL VARIABLES
 **********************/
QueueHandle_t g_Ble_Inform_Queue;
								
uint8_t g_bleOpeBuff[400];

							
/**********************
 *   GLOBAL FUNCTIONS
 **********************/
/**
 * ble status get
 * @param null
 * @return ble status
 */
BLE_TASK_WORK_STATUS_TYPE ble_status_get(void)
{
	return s_BleSta;
}


/**
 * ble refresh task
 * @param pvParameters
 * @return null
 */
void vBLETask(void *pvParameters)
{
	api_esp_ble_init();
	
	g_Ble_Inform_Queue = xQueueCreate(BLE_INFORM_QUEUE_MAX_NUM, sizeof(BLE_INFORM_QUEUE_TYPE));

	while(!g_SPIFlashInitOkFlag)
	{
		vTaskDelay(100);
	}
	dev_data_tab_refresh(NULL, REFRESH_ALL);
	
	for(;;)
	{	
#if (1 == RTOS_REMAIN_STACK_VARIABLE_RECORD_EN)	
		if(2 == g_tskReadStackCtrl)
		{
			g_tskRemainStack[g_tskReadStackCtrl] = uxTaskGetStackHighWaterMark(NULL);
			g_tskReadStackCtrl = 3;
		}
#endif
		static uint8_t _ble_switch_old = 0;
		if(_ble_switch_old != g_SystemPara.ble_switch)
		{
			if(g_SystemPara.ble_switch)
			{
				api_esp_ble_init();
				s_BleSta = BLE_CONFIG;
			}
			else
			{
				api_esp_ble_init();
				s_BleSta = BLE_NULL;
			}
		}
		_ble_switch_old = g_SystemPara.ble_switch;
		
		
		switch(s_BleSta)
		{
			case BLE_CONFIG:
			{
				ESP_BLE_CMD_STA u_result = api_esp_ble_config();

				if(ESP_BLE_CONFIG_FINISH == u_result)
				{
					//set ble name code
					if(0 == strlen(g_SystemPara.BleName))
					{
						if(1)//(0xFF == g_SerialNumber[0]) || (0 == g_SerialNumber[0]))
						{
							sprintf(g_SystemPara.BleName, "%s", BLE_DEFAULT_NAME);
						}
						else
						{
							sprintf(g_SystemPara.BleName, "%s%c%c%c%c", "NEO4000_",g_SerialNumber[0],g_SerialNumber[1],g_SerialNumber[2],g_SerialNumber[3]);
						}
					}
					
					if(0 == strlen(g_SystemPara.Password))
					{
						sprintf(g_SystemPara.Password, "%s", BLE_DEFAULT_CODE);
					}
					
					api_esp_ble_name_set(g_SystemPara.BleName);
					
					//set rf power
					api_esp_ble_power_set(15, 15, 15);
					s_BleSta = BLE_MONITOR;
				}
				else
				{
					vTaskDelay(20);
				}
			}
			break;
			
			case BLE_MONITOR:	
			case BLE_CONNECTED:
			{
				ble_data_receive_process();
			}
			break;
				
			case BLE_SPP_MODE:
			{
				ble_data_receive_process();
//				vTaskDelay(5);
			}
			break;
			
			default:
			{
			
			}
			break;
		}
		

#if (1 == BLE_HEARTBEAT_HANDSHAKE_EN)
		//ble time out count
		if((BLE_CONNECTED == s_BleSta) || (BLE_SPP_MODE == s_BleSta) || s_BleAdvOpenFailed)
		{
			s_BleLostedCnt++;
			if(s_BleLostedCnt > BLE_LOST_JUDGE_TIME/BLE_TASK_DELAY)
			{
				s_BleLostedCnt = 0;
				
				//hardware reset
				api_esp_ble_init();
				
				LOG_INFO("Ble restore!!!!");
				
				s_BleSta = BLE_CONFIG;
				
				s_BleAdvOpenFailed = 0;
			}
		}
#endif
		
#if (1 == BLE_WCM_BROADCAST_EN)
		
		if(BLE_SPP_MODE == s_BleSta)
		{
//			fleet_broadcase_check(s_bleReplyBuff);
		}
		
#endif		
		static uint32_t s_dw_sync_time = 0;
	
		if(0 == s_dw_sync_time)
		{
			s_dw_sync_time = xTaskGetTickCount();
		}
		
		if((xTaskGetTickCount() - s_dw_sync_time) > 2000)
		{
			s_dw_sync_time = 0;
//			ble_name_code_sync();
			
			//adv reopen
			if(s_BleAdvOpenFailed)
			{
				s_BleAdvOpenFailed = api_esp_ble_adv_open();
			}
		}
		
		vTaskDelay(BLE_TASK_DELAY);
	}
}


/**********************
 *   STATIC FUNCTIONS
 **********************/
/**
 * ble receive data and process
 * @param 
 * @return null
 */
static void ble_data_receive_process(void)
{
	#define CMD_FRAME_COMBINE_OVER_TIME		300
	#define BLE_DATA_BUFF_SIZE		2000
	
    uint8_t u_b_process_finish = 0;
    
    static uint16_t s_w_received_len = 0;
	uint16_t u_w_current_rec_len = 0;
    uint16_t s_w_processed_len = 0;
    
    static uint8_t s_pb_ble_buff[BLE_DATA_BUFF_SIZE] = {0};
	
	static uint32_t s_dw_combine_start_time = 0;
	
    
	//ble rec data
	while(api_esp_ble_queue_element_num_get() && (s_w_received_len+u_w_current_rec_len < BLE_DATA_BUFF_SIZE-200))
	{
		u_w_current_rec_len += api_esp_data_rx(s_pb_ble_buff + s_w_received_len + u_w_current_rec_len, 0);
	}

	
	if((0 == s_dw_combine_start_time) && (s_w_received_len > 0))
	{
		s_dw_combine_start_time = xTaskGetTickCount();
	}
	
	if(s_dw_combine_start_time > 0)
	{
		if((xTaskGetTickCount() - s_dw_combine_start_time) > CMD_FRAME_COMBINE_OVER_TIME)
		{
			s_dw_combine_start_time = 0;
			
			u_b_process_finish = 1;
			
			LOG_INFO("ble wait for combine frame over time !!!");
		}
	}
    
	if(u_w_current_rec_len)	
    {
		s_w_received_len += u_w_current_rec_len;
		
		s_dw_combine_start_time = 0;
		
        if(strstr((char*)s_pb_ble_buff, "+BLEDISCONN:"))  //disconnect
        {
            s_BleSta = BLE_MONITOR;
            
//            api_esp_ble_direct_mode_exit();
			
			vTaskDelay(500);
            
			api_esp_ble_name_set(g_SystemPara.BleName);
            s_BleAdvOpenFailed = api_esp_ble_adv_open();
			
			LOG_INFO("s_BleAdvOpenFailed result : %d", s_BleAdvOpenFailed);
				
            u_b_process_finish = 1;
        }
        else if(strstr((char*)s_pb_ble_buff, "+BLECONN:0"))  //connect
        {
            s_BleSta = BLE_CONNECTED;
            
            u_b_process_finish = 1;
			lv_event_send(ui_GuideCont_BleIcon, LV_EVENT_CLICKED, NULL);
        }
        else if(strstr((char*)s_pb_ble_buff, "+WRITE:0,1,6,1,2"))
        {
            if(ESP_AT_ERROR_SUCCESS == api_esp_cmd_tx("AT+BLESPP\r\n", "OK", 500, NULL))
            {
                s_BleSta = BLE_SPP_MODE;
            }
            else
            {
                //if spp turn on failed
    
            }
            
            vTaskDelay(15);
            
            u_b_process_finish = 1;
        }
        else
        {
            while(s_w_processed_len < s_w_received_len)
            {
                uint32_t u_dw_ret = api_wcm_data_obtain(s_pb_ble_buff + s_w_processed_len, (s_w_received_len - s_w_processed_len));
				
                if(u_dw_ret & 0x0000ffff)
                {
                    static WCM_FRAME_CONSIST_TYPE s_st_wcm = {0};
					WCM_DATA_ERROR_CODE u_error = api_wcm_data_process((s_pb_ble_buff + s_w_processed_len + (u_dw_ret >> 16)), (u_dw_ret & 0x0000ffff), &s_st_wcm);
                    if(WCM_ERROR_NULL == u_error)
                    {
                        s_BleSta = BLE_SPP_MODE;
                        wcm_data_process(&s_st_wcm);

                        s_w_processed_len += ((u_dw_ret >> 16) + (u_dw_ret & 0x0000ffff));
                    }
                    else
                    {
						s_w_processed_len += ((u_dw_ret >> 16) + (u_dw_ret & 0x0000ffff));
                    }
                }
                else
                {
                    break;
                }
            }

			//data offset
			if(s_w_received_len > s_w_processed_len)
			{
				memcpy(s_pb_ble_buff, s_pb_ble_buff+s_w_processed_len, s_w_received_len-s_w_processed_len);
				
				LOG_INFO("offset s_w_received_len %d  s_w_processed_len:%d first byte: 0x%X", s_w_received_len, s_w_processed_len, s_pb_ble_buff[0]);
				
				s_w_received_len -= s_w_processed_len;
				
				memset(&s_pb_ble_buff[s_w_received_len], 0, s_w_processed_len);
			}
			
			if((s_w_received_len > 0) && (0xfe != s_pb_ble_buff[0]))
			{
				u_b_process_finish = 1;
				
				LOG_INFO("s_pb_ble_buff first byte is no 0xfe");
			}
            
             //force clear
            if(
                    (s_w_received_len > (BLE_DATA_BUFF_SIZE)) || 
                    (s_w_processed_len >= s_w_received_len)
            )
            {
                u_b_process_finish = 1;
            }
        }
    }
	
	if(1 == u_b_process_finish)
	{
		s_w_received_len = 0;
		memset(s_pb_ble_buff, 0, BLE_DATA_BUFF_SIZE);
		
		s_dw_combine_start_time = 0;
	}
}



/**
 * wcm frame's process function
 * @param wcm_frame wcm frame data
 * @return null
 */
static void wcm_data_process(WCM_FRAME_CONSIST_TYPE* wcm_frame)
{
	//wcm over time count clear
	s_BleLostedCnt = 0;
	
	uint16_t u_w_reply_len = 0;
	
	uint16_t u_cmd_len = wcm_frame->PackLen - wcm_frame->ProtocolHeadLen - 2;
	
//	LOG_INFO("wcm_data_process: 0x%02X, len: %d, cmdlen: %d", wcm_frame->CmdType, wcm_frame->PackLen, u_cmd_len);
	
	//time synchronization
//	if(DATA_BIT_GET(g_SYS_Dev_Data.sys_config_switch_setup, 1))
//	{
//		if(WCM_FRAME_PROTOCOL_HEAD_LEN_18 == wcm_frame->HeadLenType)
//		{
//			time_stamp_rtc_sync(wcm_frame->ProtocolHeadUnion.Long_18.TimeStamp);
//		}
//		else if(WCM_FRAME_PROTOCOL_HEAD_LEN_9 == wcm_frame->HeadLenType)
//		{
//			time_stamp_rtc_sync(wcm_frame->ProtocolHeadUnion.Short_9.TimeStamp);
//		}
//	}
	
	//wcm protocol vesion check
	if((WCM_FRAME_PROTOCOL_HEAD_LEN_18 == wcm_frame->HeadLenType) && (wcm_frame->ProtocolHeadUnion.Long_18.ProtocolVersion < WCM_PROTOCOL_VER))
	{
		s_bleReplyBuff[0] = 0;
		s_bleReplyBuff[1] = 1;
		
		ble_wcm_data_send(wcm_frame, WCM_CONNECT_ACK, s_bleReplyBuff, 2);
	}
	else
	{
		switch(wcm_frame->CmdType)
		{
			case WCM_CONNECT:
			{
				for(uint16_t i = 0; i < 6; i++)
				{
					s_bleReplyBuff[i] = 0;
				}
				
				if(WCM_FRAME_PROTOCOL_HEAD_LEN_18 == wcm_frame->HeadLenType)
				{
					time_stamp_rtc_sync(wcm_frame->ProtocolHeadUnion.Long_18.TimeStamp, 1);
				}
				else if(WCM_FRAME_PROTOCOL_HEAD_LEN_9 == wcm_frame->HeadLenType)
				{
					time_stamp_rtc_sync(wcm_frame->ProtocolHeadUnion.Short_9.TimeStamp, 1);
				}
				
//				smp_time_stamp_sync();
				
				ble_wcm_data_send(wcm_frame, WCM_CONNECT_ACK, s_bleReplyBuff, 6);
			}
			break;
						
			case WCM_CONFIG:
			{
				ble_data_decode(wcm_frame->CmdData, (wcm_frame->PackLen - wcm_frame->ProtocolHeadLen - 2));
				
				WCM_CONFIG_CTRL_FRAME_TYPE u_st_config = {0};
				u_st_config.addr_len = wcm_frame->CmdData[0];
				
				//default reply error
				s_bleReplyBuff[0] = 0;
				s_bleReplyBuff[1] = 1;
				
				if(5 != u_st_config.addr_len)
				{
					
				}
				else
				{
					u_st_config.addr = &wcm_frame->CmdData[1];
					u_st_config.data_len = wcm_frame->CmdData[u_st_config.addr_len+1];
					u_st_config.data = &wcm_frame->CmdData[1 + u_st_config.addr_len + 1];
					
					switch(wcm_special_addr_process(WCM_SETUP_ADDR, &u_st_config, &s_bleReplyBuff[0], &u_w_reply_len))
					{
						case ADDR_NO_SPECIAL:
						{
							uint32_t u_dw_dev_id = (uint32_t)(
																	(0x0 << 24) + 
																	(u_st_config.addr[0] << 16) +
																	(u_st_config.addr[1] << 8) +
																	(u_st_config.addr[2] << 0)
															);
							
							DEV_DATA_TABLE_INFO_TYPE* u_pst_tab_index = dev_info_address_get(
																								u_dw_dev_id,
																								((u_st_config.addr[3] << 8) + u_st_config.addr[4])
																							);
							
							if(NULL != u_pst_tab_index)
							{
								if(WCM_SETUP_ADDR == u_pst_tab_index->type)
								{
									if(u_st_config.data_len == u_pst_tab_index->len)
									{
										if((NULL != u_pst_tab_index->ope_index) && (OPE_INDEX_WRITE_C4_FLASH != u_pst_tab_index->ope_index))
										{
//											INNER_MACHINE_STATUS_UNIT_TYPE u_queue;
//											memset(&u_queue, 0, sizeof(INNER_MACHINE_STATUS_UNIT_TYPE));
//											u_queue.status = INNER_CONFIG_CTRL;
//											u_queue.Set.set_source = SET_REPLY_BLE;
//											u_queue.Set.s_index = (INNER_SETUP_INDEX_TYPE)u_pst_tab_index->ope_index;
//											
//											if(u_pst_tab_index->len == g_InnerConfigCtrlInfoTab[u_pst_tab_index->ope_index].len)
//											{
//												memcpy(&u_queue.Set.value[0], u_st_config.data, u_st_config.data_len);
//											}
//											else if(u_pst_tab_index->len < g_InnerConfigCtrlInfoTab[u_pst_tab_index->ope_index].len)
//											{
//												memcpy(
//														&u_queue.Set.value[g_InnerConfigCtrlInfoTab[u_pst_tab_index->ope_index].len - u_pst_tab_index->len],
//														u_pst_tab_index->data,
//														u_pst_tab_index->len
//													);
//											}
//											else if(u_pst_tab_index->len > g_InnerConfigCtrlInfoTab[u_pst_tab_index->ope_index].len) 
//											{
//												memcpy(
//														&u_queue.Set.value[0],
//														&u_pst_tab_index->data[u_pst_tab_index->len - g_InnerConfigCtrlInfoTab[u_pst_tab_index->ope_index].len],
//														g_InnerConfigCtrlInfoTab[u_pst_tab_index->ope_index].len
//													);
//											}
//											
////											if(Innercomm_Queue != NULL)
////											{
////												xQueueSend(Innercomm_Queue, &u_queue, 0);
////											}
//											
//											//wait for 485 reply
//											uint32_t u_dw_record_time = xTaskGetTickCount();
//									
//											while((xTaskGetTickCount() - u_dw_record_time) < BLE_INFORM_WAIT_TIME)
//											{
//												if(g_Ble_Inform_Queue != NULL)
//												{
//													BLE_INFORM_QUEUE_TYPE u_queue;
//													if(pdTRUE == xQueueReceive(g_Ble_Inform_Queue, &u_queue, 10))
//													{
//														if((true == u_queue.cmd_result) && (u_queue.index == u_pst_tab_index->ope_index))
//														{
//															s_bleReplyBuff[0] = 0;
//															s_bleReplyBuff[1] = 0;
//															
//															break;
//														}
//													}
//												}
//												else
//												{
//													break;
//												}
//											}
										}
										else
										{
											if(2 == u_pst_tab_index->len)
											{
												*(uint16_t*)u_pst_tab_index->data = (uint16_t)((u_st_config.data[0] << 8) + u_st_config.data[1]);
											}
											else
											{
												memcpy(u_pst_tab_index->data, u_st_config.data, u_pst_tab_index->len);
											}
											
											if(OPE_INDEX_WRITE_C4_FLASH == u_pst_tab_index->ope_index)
											{
												dev_data_write_flash_operate(u_dw_dev_id);
											}
											
											s_bleReplyBuff[0] = 0;
											s_bleReplyBuff[1] = 0;
										}
									}
								}
							}
						}
						break;
						
						case ADDR_SPECIAL:
						{

						}
						break;
						
						default:
						break;
					}
				}
				
				u_w_reply_len = 2;
				
				ble_wcm_data_send(wcm_frame, WCM_CONFIG_ACK, s_bleReplyBuff, u_w_reply_len);
			}
			break;
			case WCM_CONTROL:
			{
				//data invert
				ble_data_decode(wcm_frame->CmdData, (wcm_frame->PackLen - wcm_frame->ProtocolHeadLen - 2));
				
				WCM_CONFIG_CTRL_FRAME_TYPE u_st_ctrl = {0};
				u_st_ctrl.addr_len = wcm_frame->CmdData[0];
				
				//default reply error
				s_bleReplyBuff[0] = 0;
				s_bleReplyBuff[1] = 1;
				
				if(5 != u_st_ctrl.addr_len)
				{
					
				}
				else
				{
					u_st_ctrl.addr = &wcm_frame->CmdData[1];
					u_st_ctrl.data_len = wcm_frame->CmdData[u_st_ctrl.addr_len+1];
					u_st_ctrl.data = &wcm_frame->CmdData[1 + u_st_ctrl.addr_len + 1];
					
					switch(wcm_special_addr_process(WCM_SETUP_ADDR, &u_st_ctrl, &s_bleReplyBuff[0], &u_w_reply_len))
					{
//						case ADDR_NO_SPECIAL:
//						{
//							uint32_t u_dw_dev_id = (uint32_t)(
//																	(0x0 << 24) + 
//																	(u_st_ctrl.addr[0] << 16) +
//																	(u_st_ctrl.addr[1] << 8) +
//																	(u_st_ctrl.addr[2] << 0)
//															);
//							
//							DEV_DATA_TABLE_INFO_TYPE* u_pst_tab_index = dev_info_address_get(
//																								u_dw_dev_id,
//																								((u_st_ctrl.addr[3] << 8) + u_st_ctrl.addr[4])
//																							);
//							
//							if(NULL != u_pst_tab_index)
//							{
//								if(WCM_CONTROL_ADDR == u_pst_tab_index->type)
//								{
//									if(u_st_ctrl.data_len == u_pst_tab_index->len)
//									{
////										if(NULL != u_pst_tab_index->ope_index)
////										{
////											INNER_MACHINE_STATUS_UNIT_TYPE u_queue;
////											memset(&u_queue, 0, sizeof(INNER_MACHINE_STATUS_UNIT_TYPE));
////											u_queue.status = INNER_CONFIG_CTRL;
////											u_queue.Set.set_source = SET_REPLY_BLE;
////											u_queue.Set.s_index = (INNER_SETUP_INDEX_TYPE)u_pst_tab_index->ope_index;
////											
////											if(u_pst_tab_index->len == g_InnerConfigCtrlInfoTab[u_pst_tab_index->ope_index].len)
////											{
////												memcpy(&u_queue.Set.value[0], u_st_ctrl.data, u_st_ctrl.data_len);
////											}
////											else if(u_pst_tab_index->len < g_InnerConfigCtrlInfoTab[u_pst_tab_index->ope_index].len)
////											{
////												memcpy(
////														&u_queue.Set.value[g_InnerConfigCtrlInfoTab[u_pst_tab_index->ope_index].len - u_pst_tab_index->len],
////														u_pst_tab_index->data,
////														u_pst_tab_index->len
////													);
////											}
////											else if(u_pst_tab_index->len > g_InnerConfigCtrlInfoTab[u_pst_tab_index->ope_index].len) 
////											{
////												memcpy(
////														&u_queue.Set.value[0],
////														&u_pst_tab_index->data[u_pst_tab_index->len - g_InnerConfigCtrlInfoTab[u_pst_tab_index->ope_index].len],
////														g_InnerConfigCtrlInfoTab[u_pst_tab_index->ope_index].len
////													);
////											}
////											
//////											if(Innercomm_Queue != NULL)
//////											{
//////												xQueueSend(Innercomm_Queue, &u_queue, 0);
//////											}
////											
////											
////											//wait for 485 reply
////											uint32_t u_dw_record_time = xTaskGetTickCount();
////									
////											while((xTaskGetTickCount() - u_dw_record_time) < BLE_INFORM_WAIT_TIME)
////											{
////												if(g_Ble_Inform_Queue != NULL)
////												{
////													BLE_INFORM_QUEUE_TYPE u_queue;
////													if(pdTRUE == xQueueReceive(g_Ble_Inform_Queue, &u_queue, 10))
////													{
////														if((true == u_queue.cmd_result) && (u_queue.index == u_pst_tab_index->ope_index))
////														{
////															s_bleReplyBuff[0] = 0;
////															s_bleReplyBuff[1] = 0;
////															
////															break;
////														}
////													}
////												}
////												else
////												{
////													break;
////												}
////											}
////										}
////										else
////										{
////											
////										}
//									}
//								}
//							}
//							
//							u_w_reply_len = 2;
//						}
//						break;
						case ADDR_SPECIAL:
						{

						}
						break;
						
						default:
						break;
					}
				}
				
				u_w_reply_len = 2;
				
				ble_wcm_data_send(wcm_frame, WCM_CONTROL_ACK, s_bleReplyBuff, u_w_reply_len);
			}
			break;
			
			case WCM_QUERY:
			{
				WCM_QUERY_FRAME_TYPE u_st_query = {0};
				
				u_st_query.addr_len = wcm_frame->CmdData[0];
				
				if(5 != u_st_query.addr_len)
				{
					s_bleReplyBuff[0] = 0;
					s_bleReplyBuff[1] = 1;
					
					u_w_reply_len = 2;
				}
				else
				{
					memcpy(u_st_query.start_addr, &wcm_frame->CmdData[1], u_st_query.addr_len);
					memcpy(u_st_query.end_addr, &wcm_frame->CmdData[1 + u_st_query.addr_len], u_st_query.addr_len);
					
					uint16_t u_w_start_addr = (uint16_t)((u_st_query.start_addr[3] << 8) + u_st_query.start_addr[4]);
					uint16_t u_w_end_addr = (uint16_t)((u_st_query.end_addr[3] << 8) + u_st_query.end_addr[4]);
					
					switch(wcm_special_addr_process(WCM_QUERY_ADDR, &u_st_query, &s_bleReplyBuff[0], &u_w_reply_len))
					{
						case ADDR_NO_SPECIAL:
						{
							uint32_t u_dw_start_dev_id = (uint32_t)(
																	(0x0 << 24) + 
																	(u_st_query.start_addr[0] << 16) +
																	(u_st_query.start_addr[1] << 8) +
																	(u_st_query.start_addr[2] << 0)
															);
							
							uint32_t u_dw_end_dev_id = (uint32_t)(
																	(0x0 << 24) + 
																	(u_st_query.end_addr[0] << 16) +
																	(u_st_query.end_addr[1] << 8) +
																	(u_st_query.end_addr[2] << 0)
															);
							
							DEV_DATA_TABLE_INFO_TYPE* u_pst_tab_index_start = dev_info_address_get(
																									u_dw_start_dev_id,
																									u_w_start_addr
																								);
							
							DEV_DATA_TABLE_INFO_TYPE* u_pst_tab_index_end = dev_info_address_get(
																									u_dw_end_dev_id,
																									u_w_end_addr
																								);
							
							if(
									(NULL != u_pst_tab_index_start) &&
									(NULL != u_pst_tab_index_end)
							)
							{
								if(
										((u_w_end_addr - u_w_start_addr) == (u_pst_tab_index_end - u_pst_tab_index_start)) &&
										((WCM_QUERY_ADDR == u_pst_tab_index_start->type) || (WCM_SETUP_ADDR == u_pst_tab_index_start->type))	
								)
								{
									s_bleReplyBuff[0] = 0;
									s_bleReplyBuff[1] = 0;
									
									memcpy(&s_bleReplyBuff[2], u_st_query.start_addr, 5);
									
									uint8_t u_b_query_num = u_w_end_addr - u_w_start_addr + 1;
									
									s_bleReplyBuff[7] = u_b_query_num >> 8;
									s_bleReplyBuff[8] = (uint8_t)u_b_query_num;
									
									uint16_t u_w_index = 9;
									
									for(uint16_t i = 0; i < u_b_query_num; i++)
									{
										if(2 == ((u_pst_tab_index_start+i)->len))
										{
											s_bleReplyBuff[u_w_index] = (u_pst_tab_index_start+i)->data[1];
											s_bleReplyBuff[u_w_index+1] = (u_pst_tab_index_start+i)->data[0];
											
											u_w_index += 2;
										}
										else
										{
											memcpy(&s_bleReplyBuff[u_w_index], (u_pst_tab_index_start+i)->data, (u_pst_tab_index_start+i)->len);
											u_w_index += (u_pst_tab_index_start+i)->len;
										}
									}
									
									u_w_reply_len = u_w_index;
								}
								else
								{
									s_bleReplyBuff[0] = 0;
									s_bleReplyBuff[1] = 3;
									
									u_w_reply_len = 2;
								}
							}
							else
							{
								s_bleReplyBuff[0] = 0;
								s_bleReplyBuff[1] = 2;
								
								u_w_reply_len = 2;
							}
						}
						break;
						case ADDR_SPECIAL:
						{
							
						}
						break;
						
						default:
						break;
					}

					ble_wcm_data_send(wcm_frame, WCM_QUERY_ACK, s_bleReplyBuff, u_w_reply_len);
				}
			}
			break;
			
			case WCM_UPGRADE_ONOFF:
			{
				u_w_reply_len = tbb_wireless_wcm_upgrade(wcm_frame, s_bleReplyBuff);
				
				if(u_w_reply_len > 0)
				{
					ble_wcm_data_send(wcm_frame, WCM_UPGRADE_ONOFF_ACK, s_bleReplyBuff, u_w_reply_len);
				}
			}
			break;
			
			case WCM_UPGRADE_DATA:
			{
				u_w_reply_len = tbb_wireless_wcm_upgrade(wcm_frame, s_bleReplyBuff);
				
				if(u_w_reply_len > 0)
				{
					ble_wcm_data_send(wcm_frame, WCM_UPGRADE_DATA_ACK, s_bleReplyBuff, u_w_reply_len);
				}
			}
			break;
			
			case WCM_EVENT_QUERY:
			{
				WCM_DEV_RECORD_EVENT_QUERY_TYPE* u_pst_event_info = (WCM_DEV_RECORD_EVENT_QUERY_TYPE*)(&wcm_frame->CmdData[0]);
					
				uint16_t u_w_query_num = (uint16_t)((u_pst_event_info->query_num[0] << 8) +  u_pst_event_info->query_num[1]);
				
				uint32_t u_dw_query_start = (uint32_t)(
															(u_pst_event_info->query_start[0] << 24) +
															(u_pst_event_info->query_start[1] << 16) +
															(u_pst_event_info->query_start[2] << 8) +
															(u_pst_event_info->query_start[3] << 0)
													);
				
				EVENT_LOG_INFO_TYPE u_pst_log_buff[20] = {0};
				uint8_t _log_type = 0;
				switch(u_pst_event_info->event_type)
				{
					case 1:
					{
						_log_type = LOG_TYPE_ALARM;
					}
					break;
					
					case 2:
					{
						_log_type = LOG_TYPE_OPERATE;
					}
					break;
					
					case 3:
					{
						_log_type = LOG_TYPE_RUNNING;
					}
					break;				

					default:
					{
						//return error
						u_w_query_num = 0xFFFF;
					}
					break;
				}
				
				if(u_w_query_num <= 20)
				{
					s_bleReplyBuff[0] = 0;
					s_bleReplyBuff[1] = 0;
					
//					uint16_t u_w_read_log_num_actual = api_userlog_read(u_dw_query_start, u_w_query_num, u_pst_log_buff);
					uint16_t u_w_read_log_num_actual = api_event_log_read(u_dw_query_start, u_w_query_num, u_pst_log_buff, _log_type);
					
					s_bleReplyBuff[2] = (uint8_t)(u_w_read_log_num_actual >> 8);
					s_bleReplyBuff[3] = (uint8_t)(u_w_read_log_num_actual);
					
					if(0 == u_w_read_log_num_actual)
					{
						s_bleReplyBuff[1] = 1;
					}
					else
					{	
						for(uint16_t i = 0; i < u_w_read_log_num_actual; i++)
						{		
							s_bleReplyBuff[4 + 0 + 15*i] = (uint8_t)(u_pst_log_buff[i].LogCode >> 8);
							s_bleReplyBuff[4 + 1 + 15*i] = (uint8_t)u_pst_log_buff[i].LogCode;
							
							s_bleReplyBuff[4 + 2 + 15*i] = 1;
							
							s_bleReplyBuff[4 + 3 + 15*i] = 0x00;
							s_bleReplyBuff[4 + 4 + 15*i] = 0x00;
							s_bleReplyBuff[4 + 5 + 15*i] = 0x00;
							s_bleReplyBuff[4 + 6 + 15*i] = 0x00;
							
							s_bleReplyBuff[4 + 7 + 15*i] = (uint8_t)(u_pst_log_buff[i].TimeStamp >> 24);
							s_bleReplyBuff[4 + 8 + 15*i] = (uint8_t)(u_pst_log_buff[i].TimeStamp >> 16);
							s_bleReplyBuff[4 + 9 + 15*i] = (uint8_t)(u_pst_log_buff[i].TimeStamp >> 8);
							s_bleReplyBuff[4 + 10 + 15*i] = (uint8_t)(u_pst_log_buff[i].TimeStamp >> 0);
							
							s_bleReplyBuff[4 + 11 + 15*i] = (uint8_t)(u_pst_log_buff[i].LogValueOld>>8);
							s_bleReplyBuff[4 + 12 + 15*i] = (uint8_t)(u_pst_log_buff[i].LogValueOld);
							
							s_bleReplyBuff[4 + 13 + 15*i] = (uint8_t)(u_pst_log_buff[i].LogValueNew>>8);
							s_bleReplyBuff[4 + 14 + 15*i] = (uint8_t)(u_pst_log_buff[i].LogValueNew);
							
						}
					}
					
					u_w_reply_len = 4 + 15*u_w_read_log_num_actual;
				}
				else
				{
					s_bleReplyBuff[0] = 0;
					s_bleReplyBuff[1] = 1;
					
					s_bleReplyBuff[2] = 0;
					s_bleReplyBuff[3] = 0;
					
					u_w_reply_len  = 4;	
				}
				
				ble_wcm_data_send(wcm_frame, WCM_EVENT_QUERY_ACK, s_bleReplyBuff, u_w_reply_len);
			}
			break;
			
			case WCM_HEART_BEAT:
			{
				u_w_reply_len = 2;
				
				ble_wcm_data_send(wcm_frame, WCM_HEART_BEAT_ACK, s_bleReplyBuff, u_w_reply_len);
			}
			break;
			
			case WCM_TRANSMIT_ONOFF:
			{
				u_w_reply_len = tbb_wcm_file_transmit_upgrade(wcm_frame, s_bleReplyBuff);
				
				if(u_w_reply_len > 0)
				{
					ble_wcm_data_send(wcm_frame, WCM_TRANSMIT_ONOFF_ACK, s_bleReplyBuff, u_w_reply_len);
				}
			}
			break;
			
			case WCM_TRANSMIT_DATA:
			{
				u_w_reply_len = tbb_wcm_file_transmit_upgrade(wcm_frame, s_bleReplyBuff);
				
				if(u_w_reply_len > 0)
				{
					ble_wcm_data_send(wcm_frame, WCM_TRANSMIT_DATA_ACK, s_bleReplyBuff, u_w_reply_len);
				}
			}
			break;
			
			case WCM_FLEET_PROTOCOL_DATA_SEND:
			{
				u_w_reply_len = fleet_data_process(wcm_frame->CmdData, u_cmd_len, s_bleReplyBuff);
				
				if(u_w_reply_len > 0)
				{
					ble_wcm_data_send(wcm_frame, WCM_FLEET_PROTOCOL_DATA_SEND_ACK, s_bleReplyBuff, u_w_reply_len);
				}
			}
			break;
			
//			case WCM_FLEET_CREATE_DATA_SHEET:
//			{
//				FLEET_INHERENT_SHEET_TYPE u_inherent;

//				memcpy(&u_inherent.info, &wcm_frame->CmdData[0], sizeof(FLEET_DATA_SHEET_INFO_UNIT_TYPE));
//				
//				u_inherent.start_addr = (wcm_frame->CmdData[18] << 8) + wcm_frame->CmdData[19];
//				u_inherent.end_addr = (wcm_frame->CmdData[20] << 8) + wcm_frame->CmdData[21];
//				
//				s_bleReplyBuff[0] = 0;
//				s_bleReplyBuff[1] = 1;
//				u_w_reply_len = 2;
//				
//				//length check
//				if(u_cmd_len == (18+4+(u_inherent.end_addr - u_inherent.start_addr + 1)*2))
//				{
//					s_bleReplyBuff[1] = api_fleet_inherent_sheet_create(&u_inherent);
//				}
//				
//				ble_wcm_data_send(wcm_frame, WCM_FLEET_CREATE_DATA_SHEET_ACK, s_bleReplyBuff, u_w_reply_len);
//			}
//			break;
			
			case WCM_FLEET_DATA_SHEET_OPER_WH_CMD:
			{
				uint32_t dev_id = (uint32_t)((wcm_frame->CmdData[0] << 16) | (wcm_frame->CmdData[1] << 8) | wcm_frame->CmdData[2]);
				uint16_t cmd = (uint16_t)(wcm_frame->CmdData[5] << 8) | wcm_frame->CmdData[6];
				uint16_t start_addr = (uint16_t)((wcm_frame->CmdData[7] << 8) + wcm_frame->CmdData[8]);
				uint16_t end_addr = (uint16_t)((wcm_frame->CmdData[9] << 8) + wcm_frame->CmdData[10]);
				if(u_cmd_len == 11)
				{
					s_bleReplyBuff[0] = 0;
					s_bleReplyBuff[1] = 1;
					u_w_reply_len = 2;
					
					INNER_MACHINE_STATUS_UNIT_TYPE u_send_queue;
					u_send_queue.status = CAN_STATUS_QUERY;
					u_send_queue.Query.type = CAN_QUERY_FLEET_OPERATE;
					u_send_queue.Query.start_addr = (uint16_t)((wcm_frame->CmdData[4] << 8) + wcm_frame->CmdData[5]);
					u_send_queue.Query.end_addr = (uint16_t)((wcm_frame->CmdData[6] << 8) + wcm_frame->CmdData[7]);
					u_send_queue.Query.cmd = cmd;
					u_send_queue.Query.dev_id = dev_id;
					
					if(g_CanMsgQueue != NULL)
					{
						xQueueSend(g_CanMsgQueue, &u_send_queue, 0);
					}
					
					if(g_Ble_Inform_Queue != NULL)
					{
						BLE_INFORM_QUEUE_TYPE u_rec_queue;
						if(pdTRUE == xQueueReceive(g_Ble_Inform_Queue, &u_rec_queue, BLE_CMD_REPLY_WAIT_TIME))
						{
							if(BLE_MESSAGE_QUERY_REPLY == u_rec_queue.type &&  0 == u_rec_queue.result)
							{
								s_bleReplyBuff[0] = 0;
								s_bleReplyBuff[1] = 0;
								
								memcpy(&s_bleReplyBuff[2], g_bleOpeBuff, u_rec_queue.len);
								u_w_reply_len += u_rec_queue.len;
							}
						}
						else
						{
							s_bleReplyBuff[0] = 0;
							s_bleReplyBuff[1] = 2;
						}
					}
				}
				else
				{
					s_bleReplyBuff[0] = 0;
					s_bleReplyBuff[1] = 1;
					u_w_reply_len = 2;
					
					uint16_t u_start_addr = (uint16_t)((wcm_frame->CmdData[4] << 8) + wcm_frame->CmdData[5]);
					uint16_t u_end_addr = (uint16_t)((wcm_frame->CmdData[6] << 8) + wcm_frame->CmdData[7]);
					
					//length check
					if(u_cmd_len != (8 + (u_end_addr - u_start_addr + 1)*2))
					{
						break;
					}
					
					for(uint16_t i = 0; i < (u_end_addr - u_start_addr + 1); i++)
					{
						INNER_MACHINE_STATUS_UNIT_TYPE u_send_queue;
						u_send_queue.status = CAN_STATUS_CONFIG_CTRL;
						u_send_queue.source = CAN_CMD_SOURCE_BLE;
						u_send_queue.Set.type = CAN_SET_USE_INFO;
						u_send_queue.Set.setup_index = u_start_addr;
						u_send_queue.Set.cmd = cmd;
						u_send_queue.Set.dev_id = dev_id;
						u_send_queue.Set.value[0] = wcm_frame->CmdData[11 + 2*i];
						u_send_queue.Set.value[1] = wcm_frame->CmdData[11 + 2*i + 1];
						
						if(g_CanMsgQueue != NULL)
						{
							xQueueSend(g_CanMsgQueue, &u_send_queue, 0);
						}
						
						if(g_Ble_Inform_Queue != NULL)
						{
							BLE_INFORM_QUEUE_TYPE u_rec_queue;
							if(pdTRUE == xQueueReceive(g_Ble_Inform_Queue, &u_rec_queue, BLE_CMD_REPLY_WAIT_TIME))
							{
								if(BLE_MESSAGE_SET_REPLY == u_rec_queue.type &&  0 == u_rec_queue.result)
								{
									s_bleReplyBuff[0] = 0;
									s_bleReplyBuff[1] = 0;
								}
								else
								{
									s_bleReplyBuff[0] = u_rec_queue.result >> 8;
									s_bleReplyBuff[1] = (uint8_t)u_rec_queue.result;
									break;
								}
							}
							else
							{
								s_bleReplyBuff[0] = 0;
								s_bleReplyBuff[1] = 2;
								break;
							}
						}
					}
					
					s_bleReplyBuff[0] = 0;
					s_bleReplyBuff[1] = 0;
				}
				
				ble_wcm_data_send(wcm_frame, WCM_FLEET_DATA_SHEET_OPER_WH_CMD_ACK, s_bleReplyBuff, u_w_reply_len);
			}
			break;
			
			default:
			{
			
			}
			break;
		}
	}
}

/**
 * fleet data process function
 * @param data the data for process
 * @param len 
 * @param buff reply data
 * @return the length of reply data
 */
static uint16_t fleet_data_process(uint8_t* data, uint16_t len, uint8_t* buff)
{
//	LOG_INFO("fleet_data_process cmd: 0x%X, len: %d", data[0], len);
	
	uint16_t u_len = 0;
	
	buff[u_len++] = 0;
	buff[u_len++] = 0;
	
	switch(data[0])
	{
		case FLEET_GET_DATA_SHEET_INFO:
		{
			uint16_t u_info_len = 0;
			buff[u_len++] = get_dev_list_info_by_filt_para(data[1], buff+5, &u_info_len);
			buff[u_len++] = FLEET_GET_DATA_SHEET_INFO_ACK;
			buff[u_len++] = data[1];
			
			u_len += u_info_len;
		}
		break;
		
//		case FLEET_GET_DATA_SHEET_UNIT:
//		{
//			buff[u_len++] = FLEET_ERR_UNKNOWN;
//			buff[u_len++] = FLEET_GET_DATA_SHEET_UNIT_ACK;
//		}
//		break;
		
		case FLEET_GET_DATA_SHEET_CONTENT:
		{
//			if(api_fleet_database_data_ready())
			{
				uint16_t u_rec_len = 0;
				buff[u_len++] = get_dev_list_data(&data[1], 0xff, &buff[4+9], &u_rec_len);
				buff[u_len++] = FLEET_GET_DATA_SHEET_CONTENT_ACK;
				
				memcpy(&buff[u_len], &data[1], 9);
				
				u_len += (9 + u_rec_len);
			}
//			else
//			{
//				buff[u_len++] = FLEET_ERR_SHEET_CONTENT_EMPTY;
//				buff[u_len++] = FLEET_GET_DATA_SHEET_CONTENT_ACK;
//			}
		}
		break;
		
//		case FLEET_SET_DATA_SHEET_PARA:
//		{
//			buff[u_len++] = FLEET_ERR_UNKNOWN;
//			buff[u_len++] = FLEET_SET_DATA_SHEET_PARA_ACK;
//		}
//		break;
		
//		case FLEET_CREATE_CUSTOM_SHEET:
//		{
//			buff[u_len++] = api_fleet_custom_sheet_create(&data[1], len-1);
//			buff[u_len++] = FLEET_CREATE_CUSTOM_SHEET_ACK;
//			
//			if(FLEET_ERR_SUCCESS == buff[2])
//			{
//				FLEET_CUSTOM_SHEET_TYPE* u_p_custom_info = api_fleet_sheet_as_id_point_get(FLEET_SHEET_TYPE_CUSTOM, (FLEET_DATA_SHEET_ID_TYPE*)&data[1]);
//				
//				memcpy(&buff[u_len], &u_p_custom_info->info, sizeof(FLEET_DATA_SHEET_ID_TYPE));
//				u_len += sizeof(FLEET_DATA_SHEET_ID_TYPE);
//				
//				for(uint8_t i = 0; i < u_p_custom_info->num; i++)
//				{
//					buff[u_len++] = i;
//					buff[u_len++] = 0;
//					buff[u_len++] = 0;
//				}
//			}
//		}
//		break;
		
//		case FLEET_DELETE_CUSTOM_SHEET:
//		{
//			buff[u_len++] = api_fleet_sheet_delete(FLEET_SHEET_TYPE_CUSTOM, (FLEET_DATA_SHEET_ID_TYPE*)&data[1]);
//			buff[u_len++] = FLEET_DELETE_CUSTOM_SHEET_ACK;
//		}
//		break;
		
		case FLEET_WRITE_DATA_SHEET:
		{
			buff[u_len++] = 0;
			buff[u_len++] = FLEET_WRITE_DATA_SHEET_ACK;
			
			//check the sheet exist?
			const device_data_list_t *pst_list_info = NULL;
			INNER_MACHINE_STATUS_UNIT_TYPE u_send_queue;
			if(true != get_dev_list_by_list_id(&data[1],(const device_data_list_t **)&pst_list_info))
			{
				buff[0] = FLEET_ERR_SHEET_NO_FOUND;
				break;
			}
			if(false == pst_list_info->pst_list_att->st_innate_att.un_description.bits.b_write_enable)
			{
				buff[0] = FLEET_ERR_UNKNOWN;
				break;
			}

			u_send_queue.status = CAN_STATUS_CONFIG_CTRL;
			u_send_queue.source = CAN_CMD_SOURCE_BLE;
			u_send_queue.Set.type = CAN_SET_USE_INFO;
			u_send_queue.Set.dev_id =
				(pst_list_info->pst_list_att->un_list_id.u8a_list_id[0] << 16) |
				(pst_list_info->pst_list_att->un_list_id.u8a_list_id[1] << 8) |
				(pst_list_info->pst_list_att->un_list_id.u8a_list_id[2]);
						
			memcpy(&buff[u_len], &data[1], 5);
			u_len += 5;
			
			uint16_t u_process = 1+6;
			

			
			
			while(u_process < len)
			{
				if(2 == data[u_process+1])  //length has to be 2
				{
					uint16_t u_addr = data[u_process];
					
					if(pst_list_info->pst_list_att->st_innate_att.un_description.bits.b_read_enable)
					{
						u_send_queue.Set.cmd = 0x0606;
						u_send_queue.Set.setup_index = pst_list_info->pst_data_list->pst_data[u_addr].addr;
					}
					else
					{
						INNER_CONFIG_CTRL_INFO_TYPE *ctrl_tab_temp = (INNER_CONFIG_CTRL_INFO_TYPE *)pst_list_info->pst_data_list->pst_data[u_addr].pv_data;
						
						u_send_queue.Set.cmd = ctrl_tab_temp->cmd;
						u_send_queue.Set.setup_index = ctrl_tab_temp->addr;

					}
					
					u_send_queue.Set.value[0] = data[u_process+2];
					u_send_queue.Set.value[1] = data[u_process+3];
					
					if(u_send_queue.Set.dev_id == TS43_ID)
					{

						buff[u_len++] = data[u_process];
						buff[u_len++] = 3;
						buff[u_len++] = 2;
						buff[u_len++] = 0;
						buff[u_len++] = 0;
					
						uint16_t _data = (u_send_queue.Set.value[0]<<8) + u_send_queue.Set.value[1];
						system_para_process(u_send_queue.Set.setup_index, _data);						
					}
					else
					{
						if(g_CanMsgQueue != NULL)
						{
							xQueueSend(g_CanMsgQueue, &u_send_queue, 0);
						}
						
						while(1)
						{
							if(g_Ble_Inform_Queue != NULL)
							{
								BLE_INFORM_QUEUE_TYPE u_rec_queue;
								if(pdTRUE == xQueueReceive(g_Ble_Inform_Queue, &u_rec_queue, BLE_CMD_REPLY_WAIT_TIME))
								{
									if(BLE_MESSAGE_SET_REPLY == u_rec_queue.type)
									{
										if(0 == u_rec_queue.result)
										{
											buff[u_len++] = data[u_process];
											buff[u_len++] = 3;
											buff[u_len++] = 2;
											buff[u_len++] = 0;
											buff[u_len++] = 0;
										}
										else
										{
											buff[u_len++] = data[u_process];
											buff[u_len++] = 1;
											buff[u_len++] = 2;
											buff[u_len++] = u_rec_queue.result >> 8;
											buff[u_len++] = (uint8_t)u_rec_queue.result;
										}
										
										break;
									}
									else
									{
									
									}
								}
								else
								{
									buff[u_len++] = data[u_process];
									buff[u_len++] = 1;
									buff[u_len++] = 2;
									buff[u_len++] = 0;
									buff[u_len++] = 2;
									break;
								}
							}
						}
					}
					
					u_process += (2 + data[u_process+1]);
				}
				else if((4 == data[u_process+1]) && pst_list_info->pst_list_att->st_innate_att.un_description.bits.b_write_u32)
				{
					//ps ��ؼ���ʱ������ ���⴦��
					u_send_queue.Set.cmd = 0x060D;
					u_send_queue.Set.setup_index = (data[u_process+2]<<8) + data[u_process+3];
					u_send_queue.Set.value[0] = data[u_process+4];
					u_send_queue.Set.value[1] = data[u_process+5];
					u_send_queue.source = CAN_CMD_SOURCE_BLE;
					u_send_queue.Set.type = CAN_SET_USE_INFO;
					
					if(g_CanMsgQueue != NULL)
					{
						xQueueSend(g_CanMsgQueue, &u_send_queue, 0);
					}
					
					while(1)
					{
						if(g_Ble_Inform_Queue != NULL)
						{
							BLE_INFORM_QUEUE_TYPE u_rec_queue;
							if(pdTRUE == xQueueReceive(g_Ble_Inform_Queue, &u_rec_queue, BLE_CMD_REPLY_WAIT_TIME))
							{
								if(BLE_MESSAGE_SET_REPLY == u_rec_queue.type)
								{
									if(0 == u_rec_queue.result)
									{
										buff[u_len++] = data[u_process];
										buff[u_len++] = 3;
										buff[u_len++] = 2;
										buff[u_len++] = 0;
										buff[u_len++] = 0;
									}
									else
									{
										buff[u_len++] = data[u_process];
										buff[u_len++] = 1;
										buff[u_len++] = 2;
										buff[u_len++] = u_rec_queue.result >> 8;
										buff[u_len++] = (uint8_t)u_rec_queue.result;
									}
									
									break;
								}
								else
								{
								
								}
							}
							else
							{
								buff[u_len++] = data[u_process];
								buff[u_len++] = 1;
								buff[u_len++] = 2;
								buff[u_len++] = 0;
								buff[u_len++] = 2;
								break;
							}
						}
					}					
					
					u_process += (2 + data[u_process+1]);
				}
				else
				{
					buff[u_len++] = data[u_process];
					buff[u_len++] = 0;
					buff[u_len++] = 2;
					buff[u_len++] = 0;
					buff[u_len++] = FLEET_ERR_UNKNOWN;
					
					u_process += (2 + data[u_process+1]);
				}
			}
		}
		break;
		
		default:
		{
			buff[u_len++] = FLEET_ERR_UNRECOGNIZED_OPE_CODE;
			buff[u_len++] = FLEET_WRITE_DATA_SHEET_ACK;
		}
		break;
	}
	
	
	return u_len;
}

/**
 * fleet broadcase check 
 * @param null
 * @return null
 */
//static void fleet_broadcase_check(uint8_t* buff)
//{
//	if(g_Ble_Inform_Queue != NULL)
//	{
//		BLE_INFORM_QUEUE_TYPE u_rec_queue;
//		if(pdTRUE == xQueueReceive(g_Ble_Inform_Queue, &u_rec_queue, 0))
//		{
//			if(BLE_MESSAGE_BROADCASE_CUSTOM == u_rec_queue.type)
//			{
//				for(uint8_t i = 0; i < u_rec_queue.len; i++)
//				{
//					uint16_t u_len = 0;
//					
//					buff[u_len++] = FLEET_GET_DATA_SHEET_CONTENT_ACK;
//					
//					api_fleet_sheet_index_transfer_to_id(FLEET_SHEET_TYPE_CUSTOM, g_bleOpeBuff[i], (FLEET_DATA_SHEET_ID_TYPE*)&buff[u_len]);
//					u_len += 5;
//					
//					buff[u_len++] = 0;  //expect time
//					buff[u_len++] = 0;
//					buff[u_len++] = 0;
//					buff[u_len++] = 0;
//					
//					uint16_t u_rec_len = 0;
//					api_fleet_custom_sheet_content_get(NULL, g_bleOpeBuff[i], &buff[u_len], &u_rec_len);
//					
//					u_len += u_rec_len;
//					
//					ble_wcm_data_send(NULL, WCM_FLEET_PROTOCOL_DATA_SEND, buff, u_len);
//				}
//			}
//		}
//	}
//}

/**
 * wcm ble data decode
 * @param data the address of data for decode
 * @param len the length of data for decode	
 * @return null
 */
static void ble_data_decode(uint8_t* data, uint16_t len)
{
	//data invert
	for(uint16_t i = 0; i < len; i++)
	{
		data[i] = ~data[i];
	}
}

/**
 * timestamp sync rtc
 * @param null
 * @return null
 */
void time_stamp_rtc_sync(uint32_t sync_stamp, uint8_t set_source)
{
	bsp_get_time(&g_RTCDate, &g_RTCTime);
	
	uint32_t u_RTCTimeStamp = GetTimeStamp(g_RTCDate.Year, g_RTCDate.Month, g_RTCDate.Day, 
										   g_RTCTime.Hour, g_RTCTime.Minute, g_RTCTime.Second);
	
	int32_t u_minus_value = u_RTCTimeStamp - sync_stamp;
	
	if((u_minus_value < -60) || (u_minus_value > 60))
	{
		struct tm u_st_tm;
		
		u_st_tm = *localtime(&sync_stamp);
	
		uint8_t _year = u_st_tm.tm_year + 1900 - 2000;
		uint8_t _month = u_st_tm.tm_mon + 1;
		uint8_t _day = u_st_tm.tm_mday;
		uint8_t _hour = u_st_tm.tm_hour;
		uint8_t _min = u_st_tm.tm_min;
		uint8_t _sec = u_st_tm.tm_sec;
		uint8_t _week = (0 == u_st_tm.tm_wday)? 7:u_st_tm.tm_wday;
		
		if(set_source)
		{
			batt_time_stamp_sync(_year, _month, _day, 0, _hour, _min, _sec);		
		}			
		else
		{
			bsp_set_time(_year, _month, _day, _week, _hour, _min, _sec);
		}
	}
}


/**
 * Tx data frame combine
 * @param wcm_frame wcm rx frame
 * @param cmd wcm's cmd
 * @param data data waiting to be sent
 * @param txlen length of data waiting to be sent
 * @return null
 */
static void ble_wcm_data_send(
								WCM_FRAME_CONSIST_TYPE* wcm_frame,
								WCM_CMD_INDEX_TYPE cmd, 
								uint8_t* const data, 
								uint16_t txlen
						)
{
//	LOG_INFO("ble_wcm_data_send: 0x%X, len: %d, %X %X %X %X %X", cmd, txlen, data[0], data[1], data[2], data[3], data[4]);
	
	static uint8_t s_DataTx[ESP_DATA_MAX_LEN] = {0};
	uint16_t u_DataLen = 0;
	
	bsp_get_time(&g_RTCDate, &g_RTCTime);
	uint32_t u_RTCTimeStamp = GetTimeStamp(g_RTCDate.Year, g_RTCDate.Month, g_RTCDate.Day, 
										   g_RTCTime.Hour, g_RTCTime.Minute, g_RTCTime.Second);
	
	
	if(NULL == wcm_frame)
	{
		WCM_PROTOCOL_HEAD_18_LONG_TYPE u_st_protocol_head_18;
		
		u_st_protocol_head_18.Sequence = u_RTCTimeStamp;
		u_st_protocol_head_18.ProtocolVersion = WCM_PROTOCOL_VER;
		u_st_protocol_head_18.SystemType = WCM_SYS_TYPE_INFO;
		u_st_protocol_head_18.TimeStamp = u_RTCTimeStamp;
		
		u_DataLen = api_wcm_data_combine(&u_st_protocol_head_18, NULL, cmd, data, txlen, s_DataTx);
	}
	else if(WCM_FRAME_PROTOCOL_HEAD_LEN_18 == wcm_frame->HeadLenType)
	{
		WCM_PROTOCOL_HEAD_18_LONG_TYPE u_st_protocol_head_18;
		
		u_st_protocol_head_18.Sequence = wcm_frame->ProtocolHeadUnion.Long_18.Sequence;
		u_st_protocol_head_18.ProtocolVersion = WCM_PROTOCOL_VER;
		u_st_protocol_head_18.SystemType = wcm_frame->ProtocolHeadUnion.Long_18.SystemType;
		u_st_protocol_head_18.TimeStamp = u_RTCTimeStamp;
		
		u_DataLen = api_wcm_data_combine(&u_st_protocol_head_18, NULL, cmd, data, txlen, s_DataTx);
	}
	else if(WCM_FRAME_PROTOCOL_HEAD_LEN_9 == wcm_frame->HeadLenType)
	{
		WCM_PROTOCOL_HEAD_9_SHORT_TYPE u_st_protocol_head_9;
		
		u_st_protocol_head_9.TimeStamp = wcm_frame->ProtocolHeadUnion.Short_9.TimeStamp;
		u_st_protocol_head_9.UniqueCode = wcm_frame->ProtocolHeadUnion.Short_9.UniqueCode; 
		
		u_DataLen = api_wcm_data_combine(NULL, &u_st_protocol_head_9, cmd, data, txlen, s_DataTx);
	}

	api_esp_data_tx(s_DataTx, u_DataLen);
}

void system_para_process(uint16_t index, uint16_t data)
{
	uint8_t f_save = 0;
	
	switch(index)
	{
		case 1:
		{
			f_save = 1;
			g_SystemPara.TimeFormat = data;
		}
		break;
		
		case 2:
		{
			f_save = 1;
			g_SystemPara.BuzzerSwitch = data;
		}
		break;		

		case 4:
		{
			f_save = 1;
			g_SystemPara.SleepTime = data;
		}
		break;		

		case 10:
		{
			f_save = 1;
			g_SystemPara.batt_soc_alarm = data;
		}
		break;
		
		case 11:
		{
			f_save = 1;
			g_SystemPara.eco_mode_switch = data;
		}
		break;
		
		case 12:
		{
			f_save = 1;
			g_SystemPara.min_ac_output = data;
		}
		break;
		
		case 13:
		{
			f_save = 1;
			g_SystemPara.detect_time = data;
		}
		break;					
		
		case 14:
		{
			f_save = 1;
			g_SystemPara.inv_close_soc = data;
		}
		break;	
		
		case 15:
		{
			f_save = 1;
			g_SystemPara.dc_output_close_soc = data;
		}
		break;			
		
		//timer system off
		case 20:
		{
			if(data)
			{				
				cntdownOver(&task_list[TIMER_TSK_SYSTEM]);
				cntdownTimerReloadWithSec(&task_list[TIMER_TSK_SYSTEM], (data*60));
				cntdownBegin(&task_list[TIMER_TSK_SYSTEM]);		
			}
			else
			{
				cntdownOver(&task_list[TIMER_TSK_SYSTEM]);
			}
		}
		break;
		
		//timer ac output
		case 21:
		{
			if(data)
			{				
				cntdownOver(&task_list[TIMER_TSK_AC_OUT]);
				cntdownTimerReloadWithSec(&task_list[TIMER_TSK_AC_OUT], (data*60));
				cntdownBegin(&task_list[TIMER_TSK_AC_OUT]);		
			}
			else
			{
				cntdownOver(&task_list[TIMER_TSK_AC_OUT]);
			}		
		}
		break;
		
		//timer dc power
		case 22:
		{
			if(data)
			{				
				cntdownOver(&task_list[TIMER_TSK_DC_POWER]);
				cntdownTimerReloadWithSec(&task_list[TIMER_TSK_DC_POWER], (data*60));
				cntdownBegin(&task_list[TIMER_TSK_DC_POWER]);		
			}
			else
			{
				cntdownOver(&task_list[TIMER_TSK_DC_POWER]);
			}			
		}
		break;
		
		//timer eco mode
		case 23:
		{
			if(data)
			{
				cntdownOver(&task_list[TIMER_TSK_ECO_MODE]);
				cntdownTimerReloadWithSec(&task_list[TIMER_TSK_ECO_MODE], (data*60));
				cntdownBegin(&task_list[TIMER_TSK_ECO_MODE]);	
			}
			else
			{
				cntdownOver(&task_list[TIMER_TSK_ECO_MODE]);
			}			
		}
		break;

		default:
			break;
	}
	
	if(f_save)
	{
		FLASH_CTRL_TYPEDEF FlashMsgValue = RECORD_SYS_PARA;
		if(SPIFlash_Queue != NULL)
		{
			xQueueSend(SPIFlash_Queue, &FlashMsgValue, 0);
		}
	}	

	dev_data_write_flash_operate(DEVICE_ID_LOCAL);	
}


