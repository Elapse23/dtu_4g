#include "network_protocal.h"
#include "network_protocal_cmd_macro.h"
#include "network_communication_queue.h"
#include "network_ble_communication_queue.h"
#include "device_info.h"
#include "printf_custom.h"
#include "network_local_secquence_generate.h"
#include "network_protocal_data_assembly.h"
#include "system_timestamp_generate.h"
#include "network_server_response_queue.h"
#include "application_general_func.h"
#include "crc16_arithmetic.h"
#include "lte_module.h"
#include "net_device_tree_build.h"
#include "net_attribute_tree_build.h"
#include "net_real_time_data_build.h"
#include "net_system_info_list.h"
#include "net_firmware_update.h"
#include "net_segmented_inquire.h"
#include "net_forward_set_ctrl.h"
#include "net_system_info_access.h"
#include "network_local_config.h"
#include "network_device_config.h"
#include <string.h>
#include "freertos_user_config.h"
#include "network_iot_module_data_channel_manage.h"
#include "network_tbb_device_server_communicate.h"
#include "display_management.h"
#include "network_offline_data_operate.h"
#include "network_device_config_sycn.h"
#include "network_system_type_operate.h"
#include "network_ble_communication.h"
#include "net_device_data_access.h"
#include "network_eventlog_info_operate.h"
#include "product_test.h"
#include "network_protocal_data_assembly.h"
#include "network_local_firmware_update.h"
#include "network_protocal_data_assembly.h"
#include "lte_ftp_updata.h"
#include "network_tbb_device_server_communicate.h"

extern void * pvPortMalloc( size_t xWantedSize );
extern void vPortFree( void * pv );
extern void net_inquire_discrete_ack();

typedef struct
{
    uint16_t w_data_len;
    uint8_t *pb_data;
    uint16_t w_net_cmd;
    uint8_t b_net_cmd_status;
    uint32_t dw_secquence;
}net_inquire_task_para;

/*
receive - analysis - send_passive
send_active - receive - analysis - send_active

net potocal:
    route(upload,analysis) <-> tx_rx_queue_for_send_receive_format <-> send_receive_format  [data format]
iot module:                                     |
                           [tx_rx_queue_for_module_driver <-> module_driver]
*/


uint8_t ba_decryption_key_remote[cb_net_decryption_key_len] = {0,};
uint8_t ba_decryption_key_local[cb_net_decryption_key_len] = {0x22,0x22,0x22,0x22};

#define cb_set_addr_len                         5
#define cb_set_data_len			     			255
#define cw_net_tx_data_max_len                  5000
#define cw_net_ack_tx_data_max_len				2500
#define cw_net_inquest_groups_max_len			250			//250
#define cw_net_inquest_attribute_tree_groups_max_len			80

typedef struct
{
    uint8_t b_set_addr_len;
    uint8_t ba_set_addr[cb_set_addr_len];
    uint8_t b_set_data_len;
    uint8_t ba_set_data[cb_set_data_len];
}net_set_info;
typedef union
{
    uint8_t *pb_encryption_data;
	net_set_info *pst_decryption_data;
}net_set;


#define cb_ctrl_addr_len                         5
#define cb_ctrl_data_len						 255

typedef struct
{
    uint8_t b_ctrl_addr_len;
    uint8_t ba_ctrl_addr[cb_ctrl_addr_len];
    uint8_t b_ctrl_data_len;
    uint8_t ba_ctrl_data[cb_ctrl_data_len];
}net_ctrl_info;

typedef union
{
	uint8_t *pb_encryption_data;
	net_ctrl_info *pst_decryption_data;
}net_ctrl;

#define cb_net_inquire_addr_len                       5
typedef struct
{
    uint8_t b_addr_len;
    uint8_t b_start_addr[cb_net_inquire_addr_len];
    uint8_t b_end_addr[cb_net_inquire_addr_len];
}net_inquire;
typedef struct
{
    uint8_t b_start_addr[cb_net_inquire_addr_len];
    uint8_t b_end_addr[cb_net_inquire_addr_len];
}net_inquire_discrete;

#define cb_net_get_logs_index_len                   4
#define cb_net_get_logs_num_len                     2
typedef struct
{
    uint8_t b_log_type;
    uint8_t b_log_index[cb_net_get_logs_index_len];
    uint8_t b_log_num[cb_net_get_logs_num_len];
}net_logs;

#define  cb_net_data_unpack_complete    0
#define  cb_net_data_unpack_failed      1
#define  cb_net_data_unpack_continue    2
#define  cb_net_data_receive_continue   3
/*
typedef enum
{
    cb_net_data_unpack_complete = 0,
    cb_net_data_unpack_failed,
    cb_net_data_unpack_continue,
}net_data_unpack_status;
*/

uint32_t dw_sequence_local = 0;
uint32_t dw_sequence_remote = 0;
uint8_t b_background_sync_reception = 0;

static uint8_t f_time_sync_en = TRUE;

channel_infomation st_channel_infomation =
{
    .w_timer = 0,
    .f_login = FALSE,
};

static uint8_t net_data_decryption(uint16_t w_data_len,uint8_t *pb_data_in,uint8_t *pb_data_out,uint8_t *pb_keys);
static uint8_t net_data_encode_len(uint16_t w_data_in_len,uint8_t *pb_data_in,uint16_t *pw_data_out_len);
static uint8_t net_data_encode(uint16_t w_data_in_len,uint8_t *pb_data_in,uint8_t *pb_data_out,uint16_t *w_data_out_len);
//static uint8_t net_data_pack(uint8_t *pb_data_in,\
//                             uint16_t w_data_in_len,\
//                             uint16_t *pw_data_in_index,\
//                             uint8_t *pb_data_out,\
//                             uint16_t *pw_data_out_len,\
//                             uint16_t w_encode_data_max_len);
//static uint8_t net_data_unpack(uint8_t *pb_data_in,\
//                               uint16_t w_data_in_len,\
//                               uint16_t *pw_data_in_index,\
//                               uint8_t *pb_data_out,\
//                               uint16_t *pw_data_out_len);

void network_communication_with_app()
{
    net_data_assembly(cw_NetRealTimeDataActivePush);
}

void enabke_get_network_timestamp(uint8_t b_state)
{
    f_time_sync_en = b_state;
}

uint8_t check_time_sync_enable()
{
    return f_time_sync_en;
}

//run time:20ms
#define cb_connect_server                   0
#define cb_send_attribute                   1
#define cb_send_offline_attribute           2
#define cb_send_real_time_data              3
#define cb_send_offline_data                4
#define cb_send_time_sync                   5
#define cb_send_heartbeat_data              6
#define cb_net_com_step_delay               7
#define cb_net_com_send_attribute_delay		8
uint16_t g_real_data_send_delay_time = 20 * 1000;
extern uint8_t g_update_status;
uint8_t network_communication_with_server(uint8_t b_net_data_result)
{
    #define cb_max_net_com_err_cnt          3
    #define cb_max_net_com_time_out         20000
    static uint8_t b_net_com_step = cb_connect_server;
    static uint8_t b_net_com_step_backup = cb_connect_server;
    static uint8_t b_net_com_step_next = cb_connect_server;
	static uint8_t b_net_com_step_next_next = cb_connect_server;
    static uint8_t b_net_com_err_cnt = 0;
    static uint8_t b_net_connect_err_cnt = 0;

    static TickType_t x_start_time = 0;
    TickType_t x_curent_time = 0;
    static TickType_t x_delay_time =  0;
    static uint8_t f_init = TRUE;
	uint16_t w_data_len = 2;
	net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
						   cb_net_data_att_set,
						   0x0043,
						   &w_data_len,
						   (uint8_t *)&g_real_data_send_delay_time,
						   cb_device_data_access_operate_type_read);
	if(g_real_data_send_delay_time < 10 * 1000 || g_real_data_send_delay_time > 60 * 1000)
	{
		g_real_data_send_delay_time = 20 * 1000;
	}
    if((net_sever_reconnect == b_net_data_result) ||\
       (net_sever_connect == b_net_data_result))
    {
        b_net_com_step = cb_connect_server;
//        b_net_com_step_backup = cb_connect_server;
        b_net_com_step_next = cb_connect_server;
    }
    else if((net_receive_success != b_net_data_result) &&\
            (cb_net_com_step_delay != b_net_com_step) &&\
            (cb_send_time_sync != b_net_com_step) &&\
            (cb_net_com_send_attribute_delay != b_net_com_step))
    {
        if(f_init)
        {
            f_init = FALSE;
            x_start_time = xTaskGetTickCount();
            x_delay_time =  pdMS_TO_TICKS_USER(cb_max_net_com_time_out);
        }
        x_curent_time = xTaskGetTickCount();
        if((x_curent_time - x_start_time) >= x_delay_time)
        {
            enable_save_offline_data(TRUE);
            b_net_com_err_cnt++;
            f_init = TRUE;
            if(b_net_com_err_cnt > cb_max_net_com_err_cnt)
            {
                b_net_com_err_cnt = 0;
                b_net_connect_err_cnt++;
                b_net_com_step = cb_connect_server;
                if(b_net_connect_err_cnt > cb_max_net_com_err_cnt)
                {
                    b_net_connect_err_cnt = 0;
                    network_com_with_tbb_device_server_state_set(cb_net_com_state_type_err_code,net_sever_reconnect);
                    return 6;
                }
            }
        }
        else
        {
            return 6;
        }
    }
    else
    {
        b_net_com_err_cnt = 0;
        b_net_connect_err_cnt = 0;
    }

    if(net_receive_success == b_net_data_result)
    {
        b_net_com_step = b_net_com_step_next;
        f_init = TRUE;
    }

    NET_COM_STEP:
    {
        switch(b_net_com_step)
        {
            case cb_connect_server:
				device_id_init();
                network_system_init();
                if(net_data_assembly(cw_NetConnect))
                {
//                    b_net_com_step_next = cb_send_attribute;
					b_net_com_step_next = cb_net_com_send_attribute_delay;
					b_net_com_step_next_next = cb_send_attribute;
                }
                goto RESPONSE;
            case cb_send_attribute:
                if(net_data_assembly(cw_SendAttributeTreeSeg))
                {
//                    b_net_com_step_next = cb_send_offline_attribute;
					b_net_com_step_next = cb_net_com_send_attribute_delay;
					b_net_com_step_next_next = cb_send_offline_attribute;
                }
                goto RESPONSE;
            case cb_send_offline_attribute:
                if(net_data_assembly(cw_SendOffLineAttributeTreeSeg))
                {
//                    b_net_com_step_next = cb_send_real_time_data;
					b_net_com_step_next = cb_net_com_send_attribute_delay;
					b_net_com_step_next_next = cb_send_real_time_data;
                }
                goto RESPONSE;
            case cb_send_real_time_data:
				if(g_update_status ==cw_system_no_update)
				{
					if(net_data_assembly(cw_NetRealTimeData))
					{
						b_net_com_step_backup = b_net_com_step;
						b_net_com_step_next = cb_net_com_step_delay;
					}
				}
				else
				{
					b_net_com_step_next = cb_send_real_time_data;
					b_net_com_step = cb_send_real_time_data;
				}
                goto RESPONSE;
            case cb_send_offline_data:
                if(net_data_assembly(cw_NetOffLineRealTimeData))
                {
//                    b_net_com_step = cb_net_com_step_delay;
                    b_net_com_step_next = cb_net_com_step_delay;
                }
                goto RESPONSE;
            case cb_send_time_sync:
                {
                    enabke_get_network_timestamp(FALSE);
                    net_data_assembly(cw_NetTimeSynchr);
                }
                b_net_com_step = cb_net_com_step_delay;
                b_net_com_step_next = cb_net_com_step_delay;
                goto RESPONSE;
            case cb_send_heartbeat_data:
                net_data_assembly(cw_NetHeartbeat);
                goto RESPONSE;
			case cb_net_com_send_attribute_delay:
			{
				static TickType_t x_start_time1 = 0;
				TickType_t x_curent_time1 = 0;

				if(0 == x_start_time1)
				{
					x_start_time1 = xTaskGetTickCount();
				}
				x_curent_time1 = xTaskGetTickCount();

				if((x_curent_time1 - x_start_time1) >= pdMS_TO_TICKS_USER(20000))
				{
					x_start_time1 = 0;
					b_net_com_step = b_net_com_step_next_next;
					goto NET_COM_STEP;
				}
			}
			goto RESPONSE;
            case cb_net_com_step_delay:
            {
                static TickType_t x_start_time = 0;
                TickType_t x_curent_time = 0;
//                uint8_t b_protocal_type = cb_system_protocal_type_ck;

                if(0 == x_start_time)
                {
                    x_start_time = xTaskGetTickCount();
                }
                x_curent_time = xTaskGetTickCount();

                if((x_curent_time - x_start_time) >= pdMS_TO_TICKS_USER(g_real_data_send_delay_time))
                {
                    x_start_time = 0;
                    b_net_com_step = b_net_com_step_backup;
                    goto NET_COM_STEP;
                }
                if(check_have_offline_data())
                {
                    b_net_com_step = cb_send_offline_data;
                    goto NET_COM_STEP;
                }
                if(check_time_sync_enable())
                {
                    b_net_com_step = cb_send_time_sync;
                    goto NET_COM_STEP;
                }

//                if(cb_system_protocal_type_ck != b_protocal_type)
                {
                    net_data_assembly(cw_SendSetAttributeTreeSeg);		//变更数据属性树
//                    net_data_assembly(cw_NestRealTimeDataActivePush);
                }
                goto RESPONSE;
            }
            default:
                goto RESPONSE;
        }
    }
    RESPONSE:
    return b_net_com_step;
}
extern uint8_t b_firmware_update_pacx_dsp_packet_store_finish_flag;
extern uint8_t b_firmware_update_pac_packet_store_finish_flag;
extern uint8_t ble_ftp_updata_flag;
uint8_t gb_ble_updata_status = 0;
uint8_t gb_work_mode_flag = 0;
void network_data_analysis(network_data_receive_queue px_network_data_receive_queue,
                           net_system_info_list *pst_net_system_info_list,
                           network_data_send_queue px_network_data_send_queue)
{
    network_data *pst_network_rx_data = 0;

    if(TRUE == px_network_data_receive_queue((pv_queue_operate_msg *)&pst_network_rx_data,TRUE))
    {
        network_data_info st_network_rx_data_info = {0,};
        uint16_t w_cmd_type_temp = 0;
        uint16_t w_net_response = 0;
        uint16_t w_head_len_temp = 0;
        uint32_t dw_secquence_temp = 0;
		static uint8_t *pba_decryption_key = 0;


        if((0 == pst_network_rx_data) || (0 == pst_network_rx_data->st_data_info.pb_data))
        {
			if(pst_network_rx_data->st_data_info.pb_data != 0)
			{
				vPortFree(pst_network_rx_data->st_data_info.pb_data);
                #if(dynamic_memory_request_test)
                DebugPrintf(TRUE,"network_data_analysis_0_free%x\n",pst_network_rx_data->pb_data);
                #endif
			}

            if(pst_network_rx_data != 0)
			{
          		vPortFree(pst_network_rx_data);
                #if(dynamic_memory_request_test)
                DebugPrintf(TRUE,"network_data_analysis_1_free%x\n",pst_network_rx_data->pb_data);
                #endif
			}

            return;
        }
		if(check_system_type(cb_inv_type_tyrann)  || check_system_type(cb_inv_type_apollo_matric_ii_10_15k) && 1 == b_firmware_update_pacx_dsp_packet_store_finish_flag)
		{
			#if(update_status_print)
				DebugPrintf(TRUE,"receive last one\n");
			#endif
		}

        {
            uint8_t f_result = TRUE;
            st_network_rx_data_info.pst_network_data_header_part = (network_data_header_part *)pst_network_rx_data->st_data_info.pb_data;
            st_network_rx_data_info.pb_network_data_body_part =\
                (network_data_body_part)(pst_network_rx_data->st_data_info.pb_data + sizeof(network_data_header_part));

            f_result &= bytes_to_int_big_to_little_endain(st_network_rx_data_info.pst_network_data_header_part->ba_cmd_type,\
                                                          (uint32_t *)&w_cmd_type_temp,\
                                                          sizeof(uint16_t));

            f_result &= bytes_to_int_big_to_little_endain(st_network_rx_data_info.pst_network_data_header_part->ba_net_head_len,\
                                                          (uint32_t *)&w_head_len_temp,\
                                                          sizeof(uint16_t));

            f_result &= bytes_to_int_big_to_little_endain(st_network_rx_data_info.pst_network_data_header_part->ba_sequence,\
                                                          (uint32_t *)&dw_secquence_temp,\
                                                          sizeof(uint32_t));
//			#if(update_status_print)
//				DebugPrintf(TRUE,"receive net ba_sequence: %x\n",dw_secquence_temp);
//			#endif
            if(FALSE == f_result)
            {
				vPortFree(pst_network_rx_data->st_data_info.pb_data);//1
                #if(dynamic_memory_request_test)
                DebugPrintf(TRUE,"network_data_analysis_2_free%x\n",pst_network_rx_data->pb_data);
                #endif
                vPortFree(pst_network_rx_data);//2
                #if(dynamic_memory_request_test)
                DebugPrintf(TRUE,"network_data_analysis_3_free%x\n",pst_network_rx_data);
                #endif
                return;
            }
        }

        st_channel_infomation.w_timer = 0;
        if(cb_channel_type_wireless_module == pst_network_rx_data->b_channel_index)
        {
            if(pba_decryption_key != &ba_decryption_key_remote[0])
            {
                pba_decryption_key = &ba_decryption_key_remote[0];
            }
            enable_save_offline_data(FALSE);

            if(TRUE == check_product_test())
            {
                product_test_wireless_com_comfirm();
            }
			#if(netwrok_status_print)
            DebugPrintf(TRUE,"Network data receive,channel:Wifi\r\n");
			#endif
        }
        else if(cb_channel_type_ble_module == pst_network_rx_data->b_channel_index)
        {
            if(pba_decryption_key != &ba_decryption_key_local[0])
            {
                pba_decryption_key = &ba_decryption_key_local[0];
            }
			#if(netwrok_status_print)
            DebugPrintf(TRUE,"Network data receive,channel:Ble\r\n");
			#endif
        }
		else if(cb_channel_type_4g_module == pst_network_rx_data->b_channel_index)
        {
            if(pba_decryption_key != &ba_decryption_key_local[0])
            {
                pba_decryption_key = &ba_decryption_key_local[0];
            }
            enable_save_offline_data(FALSE);

            if(TRUE == check_product_test())
            {
                product_test_wireless_com_comfirm();
            }
			#if(netwrok_status_print)
            DebugPrintf(TRUE,"Network data receive,channel:4G\r\n");
			#endif

        }

		#if(netwrok_status_print)
        DebugPrintf(TRUE,"Network data receive,cmd:%X\r\n",w_cmd_type_temp);
		#endif

        switch(w_cmd_type_temp)
        {
			case cw_NetConnect:
            {
                st_channel_infomation.f_login = TRUE;

				net_ack_assembly(0x04,
								 ba_decryption_key_local,
								 cw_NetConnectAck,
								 cbNetCmdOK,
								 dw_secquence_temp,
                                 pst_network_rx_data->b_channel_index,
                                 px_network_data_send_queue);

                break;
            }break;
            case cw_NetConnectAck:
            {
                if(network_check_local_message_id(dw_secquence_temp))
                {
                    net_connect_ack *pst_net_connect_ack = 0;

                    pst_net_connect_ack = (net_connect_ack *)st_network_rx_data_info.pb_network_data_body_part;

                    if((cbNetCmdOK == pst_net_connect_ack->w_return))
                    {
                        memcpy(ba_decryption_key_remote,pst_net_connect_ack->ba_decryption_key,cb_net_decryption_key_len);
                        st_channel_infomation.f_login = TRUE;
                        net_connect_ip_cnt_set(FALSE);
                        display_management_parameter_send(normal_mode,net_led,sever_connect,FALSE);

                        {
                            server_response_msg *pst_server_response_msg = 0;

//                            do{
//                                pst_server_response_msg = (server_response_msg *)pvPortMalloc(sizeof(server_response_msg));
//                            }
//                            while(0 == pst_server_response_msg);
                            pst_server_response_msg = (server_response_msg *)pvPortMalloc(sizeof(server_response_msg));
                            if(NULL != pst_server_response_msg)
                            {
                                #if(dynamic_memory_request_test)
                                DebugPrintf(TRUE,"network_data_analysis_0_malloc%x\n",pst_server_response_msg);
                                #endif
                                memset((uint8_t *)pst_server_response_msg,0,sizeof(server_response_msg));

                                pst_server_response_msg->b_type = cw_NetConnect;
                                pst_server_response_msg->b_status = cb_net_rsp_success;

                                network_server_response_queue_send(pst_server_response_msg);
                            }
                        }
                    }
                }
                break;
            }break;
            case cw_NetControl:
            {
                net_ctrl pst_net_ctrl;
                uint8_t f_ack_enable = TRUE;

				pst_net_ctrl.pb_encryption_data = st_network_rx_data_info.pb_network_data_body_part;
                //2:pst_net_ctrl->decryption_data.st_net_ctrl_info.b_ctrl_addr_len

				if(NULL == pba_decryption_key)
				{
					break;
				}

                net_data_decryption((pst_network_rx_data->st_data_info.w_data_len - w_head_len_temp - 2),\
                                     pst_net_ctrl.pb_encryption_data,\
                                     pst_net_ctrl.pb_encryption_data,\
                                     pba_decryption_key);
				uint32_t dw_device_id = 0;
                uint16_t w_ctrl_addr = 0;
				dw_device_id = (uint32_t)(pst_net_ctrl.pst_decryption_data->ba_ctrl_addr[0] << 16) |\
                               (uint32_t)(pst_net_ctrl.pst_decryption_data->ba_ctrl_addr[1] << 8) |\
                               (uint32_t)(pst_net_ctrl.pst_decryption_data->ba_ctrl_addr[2] << 0);

                w_ctrl_addr = (uint16_t)(pst_net_ctrl.pst_decryption_data->ba_ctrl_addr[3] << 8) |\
                              (uint16_t)(pst_net_ctrl.pst_decryption_data->ba_ctrl_addr[4] << 0);
				static net_device_info *pst_net_device_info = 0;
				uint32_t dw_master_id = 0x001A11;
				net_get_device_info((dw_master_id),
									(net_device_info **)&pst_net_device_info,
									&st_net_system_info_list);
				if((pst_net_device_info->pst_net_device_status->b_communicate_status != cb_net_device_add)
					&& (dw_device_id != 0xFFFF00 && w_ctrl_addr != 0x1000))
				{
					w_net_response = cbControlError;
				}
                else if(cb_ctrl_addr_len == pst_net_ctrl.pst_decryption_data->b_ctrl_addr_len)
                {
                    uint32_t dw_device_id = 0;
                    uint16_t w_ctrl_addr = 0;

                    dw_device_id = (uint32_t)(pst_net_ctrl.pst_decryption_data->ba_ctrl_addr[0] << 16) |\
                                   (uint32_t)(pst_net_ctrl.pst_decryption_data->ba_ctrl_addr[1] << 8) |\
                                   (uint32_t)(pst_net_ctrl.pst_decryption_data->ba_ctrl_addr[2] << 0);

                    w_ctrl_addr = (uint16_t)(pst_net_ctrl.pst_decryption_data->ba_ctrl_addr[3] << 8) |\
                                  (uint16_t)(pst_net_ctrl.pst_decryption_data->ba_ctrl_addr[4] << 0);

                    net_device_info *pst_net_device_info = 0;
					const net_data_info *pst_data_info = 0;

					if(FALSE == net_get_device_info(dw_device_id,
						    				        (net_device_info **)&pst_net_device_info,
						    				   	    &st_net_system_info_list))
					{
						break;
					}

                    if(FALSE == net_get_device_data_info(cb_net_data_att_control,
										        		 w_ctrl_addr,
										        		 (const net_data_info ** const)&pst_data_info,
										        		 pst_net_device_info))
					{
						break;
					}

                    {
                        if(((cb_net_data_att_read_write == pst_data_info->pst_data_attribute->b_read_write_type) ||\
                            (cb_net_data_att_write == pst_data_info->pst_data_attribute->b_read_write_type)) &&\
                           (pst_net_ctrl.pst_decryption_data->b_ctrl_data_len == pst_data_info->pst_data_attribute->b_len))
                        {
                            if(dwa_device_id_kinergy_ii[cw_device_id_index_system] == dw_device_id)
                            {
                                if(TRUE == net_system_ctrl(dw_device_id,
                                                            w_ctrl_addr,
                                                            dw_secquence_temp,
                                                            pst_net_ctrl.pst_decryption_data->b_ctrl_data_len,
                                                            pst_net_ctrl.pst_decryption_data->ba_ctrl_data,
                                                            &st_channel_infomation,
                                                            pst_network_rx_data->b_channel_index,
                                                            px_network_data_send_queue))
                                {
                                    w_net_response = cbNetCmdOK;
                                    if(w_ctrl_addr == 0x1000)
                                    {
                                        f_ack_enable = FALSE;
                                    }
                                }
                            }
							else if (dwa_device_id_kinergy_ii[cw_device_id_index_kinergy] == dw_device_id)
							{
								if (TRUE == net_kinergy_set(pst_data_info,
														   pst_net_ctrl.pst_decryption_data->ba_ctrl_data,
														   pst_net_ctrl.pst_decryption_data->b_ctrl_data_len))
								{
                                    event_operate_log_save(dw_device_id,
                                                           0,
                                                           pst_data_info->w_data_address_net,
                                                           "Control Trig",
                                                           12,
                                                           event_log_description_type_ascii);

									w_net_response = cbNetCmdOK;
								}
								else
								{
								   	event_operate_log_save(dw_device_id,
                                                           0,
                                                           pst_data_info->w_data_address_net,
                                                           "Trig Fail",
                                                           9,
                                                           event_log_description_type_ascii);
									w_net_response = cbBodyDataError;
								}
							}
                            else
                            {
                                uint8_t b_result;
                                b_result = network_device_config_sycn_trig(dw_device_id,
                                                                           cb_net_subset_com_ctrl_cmd,
                                                                           pst_data_info->w_data_address_net,
                                                                           pst_net_ctrl.pst_decryption_data->b_ctrl_data_len,
                                                                           pst_net_ctrl.pst_decryption_data->ba_ctrl_data,
                                                                           1,
                                                                           TRUE,
                                                                           sync_type_single);
                                if(cb_sync_trig_return_ok == b_result)
                                {
                                    #ifdef SAVE_OPERATE_LOG_BY_LOACL
                                    event_operate_log_save(dw_device_id,
                                                           0,
                                                           pst_data_info->w_data_address_device,
                                                           "Control Trig",
                                                           12,
                                                           event_log_description_type_ascii);
                                    #endif
                                    w_net_response = cbNetCmdOK;
                                }
                                else if(cb_sync_trig_return_busy != b_result)
                                {
                                    #ifdef SAVE_OPERATE_LOG_BY_LOACL
                                    event_operate_log_save(dw_device_id,
                                                           0,
                                                           pst_data_info->w_data_address_device,
                                                           "Trig Fail",
                                                           9,
                                                           event_log_description_type_ascii);
                                    #endif
                                    w_net_response = cbControlError;
                                }
                                else
                                {
                                    f_ack_enable = FALSE;
                                }
                            }
                        }
                    }
                }
                else
				{
					w_net_response = cbControlError;
					network_com_with_tbb_device_server_state_set(cb_net_com_state_type_err_code,net_ap_reconnect);
				}

                if(f_ack_enable)
                {
                    uint8_t ba_data[2] = {0,};

                    net_ack_assembly(2,
                                     ba_data,
                                     cw_NetControlAck,
                                     w_net_response,
                                     dw_secquence_temp,
                                     pst_network_rx_data->b_channel_index,
                                     px_network_data_send_queue);
                }
                break;
            }break;
            case cw_NetSetting:
            {
                net_set st_net_set = {0,};
                uint8_t f_ack_enable = TRUE;

                st_net_set.pb_encryption_data = st_network_rx_data_info.pb_network_data_body_part;

				if(NULL == pba_decryption_key)
				{
					break;
				}

                //2:pst_net_set->decryption_data.st_net_set_info.b_set_addr_len

                // 对数据进行解密
                net_data_decryption((pst_network_rx_data->st_data_info.w_data_len - w_head_len_temp - 2),\
                                     st_net_set.pb_encryption_data,\
                                     st_net_set.pb_encryption_data,\
                                     pba_decryption_key);
			    uint32_t dw_device_id = 0;  // 设备ID
                uint16_t w_set_addr = 0;   // 设置地址
				// 从数据中获取设备ID
				dw_device_id = (uint32_t)(st_net_set.pst_decryption_data->ba_set_addr[0] << 16) |\
							   (uint32_t)(st_net_set.pst_decryption_data->ba_set_addr[1] << 8) |\
							   (uint32_t)(st_net_set.pst_decryption_data->ba_set_addr[2] << 0);
				
				// 从数据中获取设置地址
				w_set_addr = (uint16_t)(st_net_set.pst_decryption_data->ba_set_addr[3] << 8) |\
							 (uint16_t)(st_net_set.pst_decryption_data->ba_set_addr[4] << 0);

                // 判断地址长度是否正确
                	static net_device_info *pst_net_device_info = 0;
				uint32_t dw_master_id = 0x001A11;
			    net_get_device_info((dw_master_id),
			                        (net_device_info **)&pst_net_device_info,
			                        &st_net_system_info_list);
			    if(((pst_net_device_info->pst_net_device_status->b_communicate_status != cb_net_device_add)
					&& ((dwa_device_id_kinergy_ii[cw_device_id_index_system] != dw_device_id) && (0x0000 != w_set_addr)))
					&& (dwa_device_id_kinergy_ii[cw_device_id_index_kinergy] != dw_device_id))
			    {
					w_net_response = cbSetError;
				}
                else if(cb_set_addr_len == st_net_set.pst_decryption_data->b_set_addr_len)
                {

                    uint8_t b_inv_num = 0;
					uint16_t w_data_len = 1;
					uint8_t b_protocal_type = 0;
					check_system_protocal_type(&b_protocal_type);
					net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
										   cb_net_data_att_status,
										   0x0160,
										   &w_data_len,
										   (uint8_t *)&b_inv_num,
										   cb_device_data_access_operate_type_read);


					//riio sun ii 下发001A110020时下发0033110010
					if((check_system_type(cb_inv_type_riio_sun_ii) 
						|| check_system_type(cb_inv_type_riio_sun_ii_gte) 
						|| check_system_type(cb_inv_type_riio_ii)) && (dw_device_id == 0x001A11) && (0x20 == w_set_addr))
					{
						dw_device_id = 0x003311;
						w_set_addr = 0x10;
					}
					//46、47设置项值除以逆变器台数下发
					if((0x003300 == (dw_device_id & 0x00FF00)) && ((0x46 == w_set_addr) || (0x47 == w_set_addr)) && (cb_system_protocal_type_ck != b_protocal_type))
					{
						uint16_t w_set_valul = 0; 
						w_set_valul = st_net_set.pst_decryption_data->ba_set_data[0] << 8 | st_net_set.pst_decryption_data->ba_set_data[1];
						st_net_set.pst_decryption_data->ba_set_data[0] = (w_set_valul / b_inv_num) >> 8;
                        st_net_set.pst_decryption_data->ba_set_data[1] = (w_set_valul / b_inv_num);
					}
					//CK I 电池满充逻辑中变更工作逻辑
					if((0x001A00 == (dw_device_id & 0x00FF00)) && (0x20 == w_set_addr) && (check_system_type(cb_inv_type_ck)))
					{
						uint8_t b_bat_full_charge = 0;
						net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
					                           cb_net_data_att_control,
					                           0x0A00,
					                           &w_data_len,
					                           (uint8_t *)&b_bat_full_charge,
					                           cb_device_data_access_operate_type_read);
						if(b_bat_full_charge == 1)
						{
							gb_work_mode_flag = 1;
							b_bat_full_charge = 0;
							net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
						                           cb_net_data_att_control,
						                           0x0A00,
						                           &w_data_len,
						                           (uint8_t *)&b_bat_full_charge,
						                           cb_device_data_access_operate_type_write);
						}
					}
                    net_device_info *pst_net_device_info = 0;
					const net_data_info *pst_data_info = 0;

                    // 获取设备信息
					if(FALSE == net_get_device_info(dw_device_id,
						    				        (net_device_info **)&pst_net_device_info,
						    				   	    &st_net_system_info_list))
					{
						break;
					}
                    if ((dw_device_id == 0xFFFF00) && (w_set_addr == 0))
                    {
						if (TRUE == net_system_set(w_set_addr,
							                      &st_network_rx_data_info,
							                      &st_channel_infomation,
							                      (pst_network_rx_data->st_data_info.w_data_len - w_head_len_temp - 2)))
						{
							w_net_response = cbNetCmdOK;
						}
						else
						{
							w_net_response = cbBodyDataError;
						}
					}
                    else
                    {
						if (FALSE == net_get_device_data_info(cb_net_data_att_set,
								                                w_set_addr,
								                                ( const net_data_info **const )&pst_data_info,
								                                pst_net_device_info))
						{
							break;
						}

						{
							if ((cb_net_data_att_read_write == pst_data_info->pst_data_attribute->b_read_write_type)
							    && (st_net_set.pst_decryption_data->b_set_data_len
							        == pst_data_info->pst_data_attribute->b_len))
							{
								if (dwa_device_id_kinergy_ii[cw_device_id_index_system] == dw_device_id)
								{
									if (TRUE== net_system_set(
									        w_set_addr,
									        &st_network_rx_data_info,
									        &st_channel_infomation,
									        (pst_network_rx_data->st_data_info.w_data_len - w_head_len_temp - 2)))
									{
										w_net_response = cbNetCmdOK;
									}
									else
									{
										w_net_response = cbBodyDataError;
									}
								}
								else if (dwa_device_id_kinergy_ii[cw_device_id_index_kinergy] == dw_device_id)
								{
									if (TRUE
									    == net_kinergy_set(pst_data_info,
									                       st_net_set.pst_decryption_data->ba_set_data,
									                       st_net_set.pst_decryption_data->b_set_data_len))
									{
										#ifdef SAVE_LOACL_OPERATE_LOG
										event_operate_log_save(dw_device_id,
										                       0,
										                       pst_data_info->w_data_address_net,
										                       "Set Done",
										                       8,
										                       event_log_description_type_ascii);
										#endif
										w_net_response = cbNetCmdOK;
									}
									else
									{
										w_net_response = cbBodyDataError;
										//                                    #ifdef SAVE_LOACL_OPERATE_LOG
										//                                    event_operate_log_save(dw_device_id,
										//                                                           0,
										//                                                           pst_data_info->w_data_address_net,
										//                                                           "Set Fail",
										//                                                           8,
										//                                                           event_log_description_type_ascii);
										//                                    #endif
									}
								}
								else
								{
									// forward to subset
									if ((cdw_device_type_cklcd == (dw_device_id & 0xFF00))
									    && ((0x0001 == pst_data_info->w_data_address_net)
									    || (0x0002 == pst_data_info->w_data_address_net)))
									{
										if (TRUE
										    == net_device_config(dw_device_id,
										                         cb_net_subset_com_set_cmd,
										                         pst_data_info,
										                         st_net_set.pst_decryption_data->b_set_data_len,
										                         st_net_set.pst_decryption_data->ba_set_data,
										                         pst_net_system_info_list))
										{
#ifdef SAVE_OPERATE_LOG_BY_LOACL
											event_operate_log_save(dw_device_id,
											                       0,
											                       pst_data_info->w_data_address_device,
											                       st_net_set.pst_decryption_data->ba_set_data,
											                       st_net_set.pst_decryption_data->b_set_data_len,
											                       event_log_description_type_hex_big_endain);
#endif
											w_net_response = cbNetCmdOK;
										}
										else
										{
											w_net_response = cbBodyDataError;
										}
									}
									else
									{
										uint8_t b_result;
#if (device_sync_status_print)
										DebugPrintf(TRUE, "start set \r\n");
#endif
										b_result = network_device_config_sycn_trig(
										    dw_device_id,
										    cb_net_subset_com_set_cmd,
										    pst_data_info->w_data_address_net,
										    st_net_set.pst_decryption_data->b_set_data_len,
										    st_net_set.pst_decryption_data->ba_set_data,
										    1,
										    TRUE,
										    sync_type_single);
										if (cb_sync_trig_return_ok == b_result)
										{
#ifdef SAVE_OPERATE_LOG_BY_LOACL
											event_operate_log_save(dw_device_id,
											                       0,
											                       pst_data_info->w_data_address_device,
											                       st_net_set.pst_decryption_data->ba_set_data,
											                       st_net_set.pst_decryption_data->b_set_data_len,
											                       event_log_description_type_hex_big_endain);
#endif
											w_net_response = cbNetCmdOK;
										}
										else if (cb_sync_trig_return_busy != b_result)
										{
											w_net_response = cbBodyDataError;
											//                                    #ifdef SAVE_OPERATE_LOG_BY_LOACL
											//                                    event_operate_log_save(dw_device_id,
											//                                                           0,
											//                                                           pst_data_info->w_data_address_device,
											//                                                           "Set Fail",
											//                                                           8,
											//                                                           event_log_description_type_ascii);
											//                                    #endif
										}
										else
										{
											f_ack_enable = FALSE;
										}
									}
								}
							}
						}
					}
				}
                else
				{
					w_net_response = cbSetError;
					network_com_with_tbb_device_server_state_set(cb_net_com_state_type_err_code,net_ap_reconnect);
				}

				if(f_ack_enable)
                {
                    uint8_t ba_data[2] = {0,};

                    net_ack_assembly(2,
                                     ba_data,
                                     cw_NetSettingAck,
                                     w_net_response,
                                     dw_secquence_temp,
                                     pst_network_rx_data->b_channel_index,
                                     px_network_data_send_queue);
                }
                break;
            }break;
            case cw_NetInquire:
            {
                net_inquire *pst_net_inquire = 0;

                pst_net_inquire = (net_inquire *)st_network_rx_data_info.pb_network_data_body_part;
                if(cb_net_inquire_addr_len == pst_net_inquire->b_addr_len)
                {
                    if(memcmp(pst_net_inquire->b_start_addr,pst_net_inquire->b_end_addr,3) != 0)				//比较前三个字节即设备标识
                    {
                        break;
                    }

                    uint8_t ba_data[7];
                    //uint8_t ba_data1[] ={0x00,0x34,0x00,0x01,0x4C};
                    if(FALSE == byte_fill_in_reverse_order(3,&pst_net_inquire->b_start_addr[0],&ba_data[0],3))	//取设备标识段存入ba_data
					{
						break;
					}

                    if(FALSE == byte_fill_in_reverse_order(2,&pst_net_inquire->b_start_addr[3],&ba_data[3],2))	//取起始地址存入ba_data
					{
						break;
					}

                    if(FALSE == byte_fill_in_reverse_order(2,&pst_net_inquire->b_end_addr[3],&ba_data[5],2))	//取结束地址存入ba_data
					{
						break;
					}

					/*if(memcmp(pst_net_inquire->b_start_addr,ba_data1,5))
					{
						b_background_sync_reception = 1;
					}*/
                    net_ack_assembly(7,
                                     ba_data,
                                     cw_NetInquireAck,
                                     cbNetCmdOK,
                                     dw_secquence_temp,
                                     pst_network_rx_data->b_channel_index,
                                     px_network_data_send_queue);

//                    network_com_with_tbb_device_server_state_set(net_receive_success);

                }
                break;
            }break;
			case cw_NetInquire_Discrete:
            {
				uint16_t b_num_of_inquest_groups = 0;
				b_num_of_inquest_groups = (pst_network_rx_data->st_data_info.w_data_len - w_head_len_temp - 2 - 1)/10;	
                uint8_t b_addr_len = 0;
				b_addr_len = st_network_rx_data_info.pb_network_data_body_part[0];
				net_inquire_discrete *pst_net_inquire_discrete[cw_net_inquest_groups_max_len] = {0,}; 

            	uint16_t w_data_len = 0;
				network_data *pst_network_data = 0;
				pst_network_data = (network_data *)pvPortMalloc(sizeof(network_data));
				if(NULL == pst_network_data)
				{
   					#if( system_alarm_print)
					DebugPrintf(TRUE,"[ERROR]Net ack malloc err:1\r\n");
    				#endif
				}
				#if(dynamic_memory_request_test)
				DebugPrintf(TRUE,"net_ack_assembly_0_malloc%x\n",pst_network_data);
				#endif
				memset((uint8_t *)pst_network_data,0,sizeof(network_data));
				pst_network_data->st_data_info.pb_data = (uint8_t *)pvPortMalloc(cw_net_ack_tx_data_max_len * sizeof(uint8_t));
				if(NULL == pst_network_data->st_data_info.pb_data)
				{
					if(NULL != pst_network_data)
					{
						vPortFree(pst_network_data);
					}
    				#if( system_alarm_print)
					DebugPrintf(TRUE,"[ERROR]Net ack malloc err:2\r\n");
    				#endif
				}
				if(TRUE == get_net_ack_header_part_assembly(&w_data_len,pst_network_data->st_data_info.pb_data,cw_NetInquire_DiscreteAck,dw_secquence_temp))
				{
					pst_network_data->st_data_info.w_data_len += w_data_len;

	                for(uint16_t b_cnt_i = 0; b_cnt_i < b_num_of_inquest_groups && b_num_of_inquest_groups < cw_net_inquest_groups_max_len; b_cnt_i ++)
	                {
	                    pst_net_inquire_discrete[b_cnt_i] = (net_inquire_discrete *)(st_network_rx_data_info.pb_network_data_body_part + 1) + b_cnt_i;
	                }
					if(cb_net_inquire_addr_len == b_addr_len)
	                {
	                	for(uint16_t b_cnt_i = 0; b_cnt_i < b_num_of_inquest_groups; b_cnt_i ++)
	                	{
	                	    uint8_t ba_data[7];
							if(memcmp(pst_net_inquire_discrete[b_cnt_i]->b_start_addr,pst_net_inquire_discrete[b_cnt_i]->b_end_addr,3) != 0)				//比较前三个字节即设备标识
	                        {
	                            break;
	                        }
	                        if(FALSE == byte_fill_in_reverse_order(3,&pst_net_inquire_discrete[b_cnt_i]->b_start_addr[0],&ba_data[0],3)) //取设备标识段存入ba_data
	                        {
	                            break;
	                        }
	    
	                        if(FALSE == byte_fill_in_reverse_order(2,&pst_net_inquire_discrete[b_cnt_i]->b_start_addr[3],&ba_data[3],2))	//取起始地址存入ba_data
	                        {
	                            break;
	                        }
	    
	                        if(FALSE == byte_fill_in_reverse_order(2,&pst_net_inquire_discrete[b_cnt_i]->b_end_addr[3],&ba_data[5],2))	//取结束地址存入ba_data
	                        {
	                            break;
	                        }
							net_inquire_discrete_ack(7,
												   ba_data,
												   &w_data_len,
												   &pst_network_data->st_data_info.pb_data[pst_network_data->st_data_info.w_data_len],
												   cbNetCmdOK,
												   b_cnt_i);
							pst_network_data->st_data_info.w_data_len += w_data_len;
							w_data_len = 0;
												   
						}
						{
				            if(TRUE == calculate_crc16(pst_network_data->st_data_info.pb_data,
				                                       pst_network_data->st_data_info.w_data_len,
				                                       &pst_network_data->st_data_info.pb_data[pst_network_data->st_data_info.w_data_len],
				                                       &pst_network_data->st_data_info.pb_data[pst_network_data->st_data_info.w_data_len + 1]))
				            {
				                pst_network_data->st_data_info.w_data_len += 2;//CRC16
				                pst_network_data->b_channel_index = pst_network_rx_data->b_channel_index;

				                px_network_data_send_queue((pv_queue_operate_msg)pst_network_data);

							}
						}
	                }
				}
                break;
            }break;
			case cw_NetGetAttributeTree_Discrete:
            {
				uint16_t b_num_of_inquest_groups = 0;
				b_num_of_inquest_groups = (pst_network_rx_data->st_data_info.w_data_len - w_head_len_temp - 2 - 1)/10;	
                uint8_t b_addr_len = 0;
				b_addr_len = st_network_rx_data_info.pb_network_data_body_part[0];
				net_inquire_discrete *pst_net_inquire_discrete[cw_net_inquest_attribute_tree_groups_max_len] = {0,}; 

            	uint16_t w_data_len = 0;
				network_data *pst_network_data = 0;
				pst_network_data = (network_data *)pvPortMalloc(sizeof(network_data));
				if(NULL == pst_network_data)
				{
   					#if( system_alarm_print)
					DebugPrintf(TRUE,"[ERROR]Net ack malloc err:1\r\n");
    				#endif
				}
				memset((uint8_t *)pst_network_data,0,sizeof(network_data));
				pst_network_data->st_data_info.pb_data = (uint8_t *)pvPortMalloc(cw_net_ack_tx_data_max_len * sizeof(uint8_t));
				if(NULL == pst_network_data->st_data_info.pb_data)
				{
					if(NULL != pst_network_data)
					{
						vPortFree(pst_network_data);
					}
    				#if( system_alarm_print)
					DebugPrintf(TRUE,"[ERROR]Net ack malloc err:2\r\n");
    				#endif
				}
				if(TRUE == get_net_ack_header_part_assembly(&w_data_len,pst_network_data->st_data_info.pb_data,cw_NetGetAttributeTree_DiscreteAck,dw_secquence_temp))
				{
					pst_network_data->st_data_info.w_data_len += w_data_len;

	                for(uint16_t b_cnt_i = 0; b_cnt_i < b_num_of_inquest_groups && b_num_of_inquest_groups < cw_net_inquest_attribute_tree_groups_max_len; b_cnt_i ++)
	                {
	                    pst_net_inquire_discrete[b_cnt_i] = (net_inquire_discrete *)(st_network_rx_data_info.pb_network_data_body_part + 1) + b_cnt_i;
	                }
					if(cb_net_inquire_addr_len == b_addr_len)
	                {
	                	for(uint16_t b_cnt_i = 0; b_cnt_i < b_num_of_inquest_groups; b_cnt_i ++)
	                	{
	                	    uint8_t ba_data[7];
							if(memcmp(pst_net_inquire_discrete[b_cnt_i]->b_start_addr,pst_net_inquire_discrete[b_cnt_i]->b_end_addr,3) != 0)				//比较前三个字节即设备标识
	                        {
	                            break;
	                        }
	                        if(FALSE == byte_fill_in_reverse_order(3,&pst_net_inquire_discrete[b_cnt_i]->b_start_addr[0],&ba_data[0],3)) //取设备标识段存入ba_data
	                        {
	                            break;
	                        }
	    
	                        if(FALSE == byte_fill_in_reverse_order(2,&pst_net_inquire_discrete[b_cnt_i]->b_start_addr[3],&ba_data[3],2))	//取起始地址存入ba_data
	                        {
	                            break;
	                        }
	    
	                        if(FALSE == byte_fill_in_reverse_order(2,&pst_net_inquire_discrete[b_cnt_i]->b_end_addr[3],&ba_data[5],2))	//取结束地址存入ba_data
	                        {
	                            break;
	                        }
							Net_Get_AttributeTree_DiscreteAck(7,
												   ba_data,
												   &w_data_len,
												   &pst_network_data->st_data_info.pb_data[pst_network_data->st_data_info.w_data_len],
												   cbNetCmdOK,
												   b_cnt_i);
							pst_network_data->st_data_info.w_data_len += w_data_len;
							w_data_len = 0;
												   
						}
						{
				            if(TRUE == calculate_crc16(pst_network_data->st_data_info.pb_data,
				                                       pst_network_data->st_data_info.w_data_len,
				                                       &pst_network_data->st_data_info.pb_data[pst_network_data->st_data_info.w_data_len],
				                                       &pst_network_data->st_data_info.pb_data[pst_network_data->st_data_info.w_data_len + 1]))
				            {
				                pst_network_data->st_data_info.w_data_len += 2;//CRC16
				                pst_network_data->b_channel_index = pst_network_rx_data->b_channel_index;

				                px_network_data_send_queue((pv_queue_operate_msg)pst_network_data);

							}
						}
	                }
				}
                break;
            }break;
			case cw_NetGetAttributeTree:
			{
				net_data_assembly(cw_NetGetAttributeTreeAck);
	          	break;
			}

            case cw_SendAttributeTreeSegACK:
            {
                if(network_check_local_message_id(dw_secquence_temp))
                {
                    net_seg_att_tree_ack *pst_net_seg_att_tree_ack = 0;

                    pst_net_seg_att_tree_ack = (net_seg_att_tree_ack *)st_network_rx_data_info.pb_network_data_body_part;

                    if((cbNetCmdOK == pst_net_seg_att_tree_ack->w_return))
                    {
                        {
                            server_response_msg *pst_server_response_msg = 0;

//                            do{
//                                pst_server_response_msg = (server_response_msg *)pvPortMalloc(sizeof(server_response_msg));
//                            }
//                            while(0 == pst_server_response_msg);
                            pst_server_response_msg = (server_response_msg *)pvPortMalloc(sizeof(server_response_msg));
                            if(NULL != pst_server_response_msg)
                            {
                                #if(dynamic_memory_request_test)
                                DebugPrintf(TRUE,"network_data_analysis_1_malloc%x\n",pst_server_response_msg);
                                #endif
                                memset((uint8_t *)pst_server_response_msg,0,sizeof(server_response_msg));

                                pst_server_response_msg->b_type = cw_SendAttributeTreeSeg;
                                pst_server_response_msg->b_status = cb_net_rsp_success;

                                network_server_response_queue_send(pst_server_response_msg);
                            }
                        }
                    }
                }
                break;
            }
            break;
            case cw_SendOffLineAttributeTreeSegACK:
            {
                if(network_check_local_message_id(dw_secquence_temp))
                {
                    net_connect_ack *pst_net_connect_ack = 0;

                    pst_net_connect_ack = (net_connect_ack *)st_network_rx_data_info.pb_network_data_body_part;

                    if((cbNetCmdOK == pst_net_connect_ack->w_return))
                    {
                        {
                            server_response_msg *pst_server_response_msg = 0;

//                            do{
//                                pst_server_response_msg = (server_response_msg *)pvPortMalloc(sizeof(server_response_msg));
//                            }
//                            while(0 == pst_server_response_msg);
                            pst_server_response_msg = (server_response_msg *)pvPortMalloc(sizeof(server_response_msg));
                            if(NULL != pst_server_response_msg)
                            {
                                #if(dynamic_memory_request_test)
                                DebugPrintf(TRUE,"network_data_analysis_1_malloc%x\n",pst_server_response_msg);
                                #endif
                                memset((uint8_t *)pst_server_response_msg,0,sizeof(server_response_msg));

                                pst_server_response_msg->b_type = cw_SendOffLineAttributeTreeSeg;
                                pst_server_response_msg->b_status = cb_net_rsp_success;

                                network_server_response_queue_send(pst_server_response_msg);
                            }
                        }
                    }
                }
                break;
            }
            break;
            case cw_NetRealTimeDataAck:
            {
                if(network_check_local_message_id(dw_secquence_temp))
                {
                    net_rtdata_ack *pst_rtdata_ack = 0;

                    pst_rtdata_ack = (net_rtdata_ack *)st_network_rx_data_info.pb_network_data_body_part;

                    if((cbNetCmdOK == pst_rtdata_ack->w_return))
                    {
                        {
                            server_response_msg *pst_server_response_msg = 0;

//                            do{
//                                pst_server_response_msg = (server_response_msg *)pvPortMalloc(sizeof(server_response_msg));
//                            }
//                            while(0 == pst_server_response_msg);
                            pst_server_response_msg = (server_response_msg *)pvPortMalloc(sizeof(server_response_msg));
                            if(NULL != pst_server_response_msg)
                            {
                                #if(dynamic_memory_request_test)
                                DebugPrintf(TRUE,"network_data_analysis_2_malloc%x\n",pst_server_response_msg);
                                #endif
                                memset((uint8_t *)pst_server_response_msg,0,sizeof(server_response_msg));

                                pst_server_response_msg->b_type = cw_NetRealTimeData;
                                pst_server_response_msg->b_status = cb_net_rsp_success;

                                network_server_response_queue_send(pst_server_response_msg);
                            }
                        }
                    }
                }
                break;
            }
            break;
            case cw_NetIPSet:
            {
                if(6 == (pst_network_rx_data->st_data_info.w_data_len - w_head_len_temp - 2))
                {
                    {
                        uint16_t w_port_temp = 0;
                        net_ip_port *pst_net_ip_port = 0;

                        pst_net_ip_port = (net_ip_port *)st_network_rx_data_info.pb_network_data_body_part;
                        if((TRUE == bytes_to_int_big_to_little_endain(pst_net_ip_port->un_port.ba_port,\
                                                               (uint32_t *)&w_port_temp,\
                                                               sizeof(w_port_temp))) &&\
                           (TRUE == byte_sort_in_reverse_order(sizeof(pst_net_ip_port->ba_ip),pst_net_ip_port->ba_ip)))
                        {


                            {
                                st_net_ip_port.ba_ip[0] = pst_net_ip_port->ba_ip[0];
                                st_net_ip_port.ba_ip[1] = pst_net_ip_port->ba_ip[1];
                                st_net_ip_port.ba_ip[2] = pst_net_ip_port->ba_ip[2];
                                st_net_ip_port.ba_ip[3] = pst_net_ip_port->ba_ip[3];

                                for(uint8_t b_cnt_i = 0;b_cnt_i < 4;b_cnt_i++)
                                {
                                    sta_network_iot_module_connection_info[connection_type_tbb_server].
									union_iot_module_config_info.
									sta_lte_4g_module_config[connection_operation_open].
									union_config_info.st_open_tcp_connection.ba_ip[b_cnt_i] =\
                                            st_net_ip_port.ba_ip[3 - b_cnt_i];
                                }
                                st_net_ip_port.un_port.w_port = w_port_temp;
                                sta_network_iot_module_connection_info[connection_type_tbb_server].
                                    union_iot_module_config_info.
                                    sta_lte_4g_module_config[connection_operation_open].
                                    union_config_info.st_open_tcp_connection.w_ip_port =\
                                        st_net_ip_port.un_port.w_port;

                                uint16_t w_data_len = 4;
                                net_system_data_access(cb_device_data_access_data_type_set,
                                                        0x000C,
                                                        &w_data_len,
                                                        (uint8_t *)&sta_network_iot_module_connection_info[connection_type_tbb_server].
                                                                    union_iot_module_config_info.
                                                                    sta_lte_4g_module_config[connection_operation_open].
                                                                    union_config_info.st_open_tcp_connection.ba_ip[0],
                                                        cb_device_data_access_operate_type_write);
                                w_data_len = 2;
                                net_system_data_access(cb_device_data_access_data_type_set,
                                                        0x000D,
                                                        &w_data_len,
                                                        (uint8_t *)&sta_network_iot_module_connection_info[connection_type_tbb_server].
                                                                    union_iot_module_config_info.
                                                                    sta_lte_4g_module_config[connection_operation_open].
                                                                    union_config_info.st_open_tcp_connection.w_ip_port,
                                                        cb_device_data_access_operate_type_write);
                            }

                            {
                                server_response_msg *pst_server_response_msg = 0;

//                                do{
//                                    pst_server_response_msg = (server_response_msg *)pvPortMalloc(sizeof(server_response_msg));
//                                }
//                                while(0 == pst_server_response_msg);
                                pst_server_response_msg = (server_response_msg *)pvPortMalloc(sizeof(server_response_msg));
                                if(NULL != pst_server_response_msg)
                                {
                                    #if(dynamic_memory_request_test)
                                    DebugPrintf(TRUE,"network_data_analysis_3_malloc%x\n",pst_server_response_msg);
                                    #endif
                                    memset((uint8_t *)pst_server_response_msg,0,sizeof(server_response_msg));

                                    pst_server_response_msg->b_type = cw_NetConnect;
                                    pst_server_response_msg->b_status = cb_net_rsp_retry;

                                    network_server_response_queue_send(pst_server_response_msg);
                                }
                            }
                        }

                    }
                }
                break;
            }
            break;
            case cw_NetGetLogs:
            {
                net_logs *pst_net_logs = 0;
                pst_net_logs = (net_logs *)st_network_rx_data_info.pb_network_data_body_part;

                uint8_t ba_data[7];

                if(FALSE == byte_fill_in_reverse_order(1,&pst_net_logs->b_log_type,&ba_data[0],1))
                {
                    break;
                }

                if(FALSE == byte_fill_in_reverse_order(4,&pst_net_logs->b_log_index[0],&ba_data[1],4))
                {
                    break;
                }

                if(FALSE == byte_fill_in_reverse_order(2,&pst_net_logs->b_log_num[0],&ba_data[5],2))
                {
                    break;
                }

                net_ack_assembly(7,
                                 ba_data,
                                 cw_NetGetLogsAck,
                                 cbNetCmdOK,
                                 dw_secquence_temp,
                                 pst_network_rx_data->b_channel_index,
                                 px_network_data_send_queue);

                break;
            }
            break;
            case cw_NetHeartbeatAck:
            {
                if(2 == (pst_network_rx_data->st_data_info.w_data_len - w_head_len_temp - 2))
                {
                    {
                        uint16_t w_response = 0;

                        if(TRUE == bytes_to_int_big_to_little_endain(st_network_rx_data_info.pb_network_data_body_part,\
                                                              (uint32_t *)&w_response,\
                                                              sizeof(w_response)))
                        {
                            server_response_msg *pst_server_response_msg = 0;

//                            do{
//                                pst_server_response_msg = (server_response_msg *)pvPortMalloc(sizeof(server_response_msg));
//                            }
//                            while(0 == pst_server_response_msg);
                            pst_server_response_msg = (server_response_msg *)pvPortMalloc(sizeof(server_response_msg));
                            if(NULL != pst_server_response_msg)
                            {
                                #if(dynamic_memory_request_test)
                                DebugPrintf(TRUE,"network_data_analysis_4_malloc%x\n",pst_server_response_msg);
                                #endif
                                memset((uint8_t *)pst_server_response_msg,0,sizeof(server_response_msg));

                                pst_server_response_msg->b_type = cw_NetHeartbeat;
                                if(cbNetCmdOK == w_response)
                                {
                                    pst_server_response_msg->b_status = cb_net_rsp_success;
                                }
                                else
                                {
                                    pst_server_response_msg->b_status = cb_net_rsp_failed;
                                }

                                network_server_response_queue_send(pst_server_response_msg);
                            }
                        }

                    }
                }
                break;
            }
            break;
			case cw_NetUpdateStart_Stop:
			{
				net_firmware_update_start(dw_secquence_temp,\
										  (net_firmware_update_start_stop *)st_network_rx_data_info.pb_network_data_body_part,\
										  &st_net_system_info_list,
                                          pst_network_rx_data->b_channel_index,
                                          px_network_data_send_queue);
				break;
			}
            break;
			case cw_NetUpdating:
			{
				ble_ftp_updata_flag = 0;
                uint8_t b_cmd_type;
				b_cmd_type = net_firmware_update_packet_operate((pst_network_rx_data->st_data_info.w_data_len - w_head_len_temp - 2),
                                                                (firmware_packet *)st_network_rx_data_info.pb_network_data_body_part,\
                                                                 &st_net_system_info_list);
				if(check_system_type(cb_inv_type_tyrann)  || check_system_type(cb_inv_type_apollo_matric_ii_10_15k)&& 3 == b_firmware_update_pacx_dsp_packet_store_finish_flag)
				{
					b_firmware_update_pacx_dsp_packet_store_finish_flag = 0;
					net_ack_assembly(0,
									 0,
									 cw_NetUpdatingAck,
									 b_cmd_type,
									 dw_secquence_temp,
									 pst_network_rx_data->b_channel_index,
									 px_network_data_send_queue);
				}
				else
				{
					net_ack_assembly(0,
									 0,
									 cw_NetUpdatingAck,
									 b_cmd_type,
									 dw_secquence_temp,
									 pst_network_rx_data->b_channel_index,
									 px_network_data_send_queue);
				}
                {
                    server_response_msg *pst_server_response_msg = 0;

//                    do{
//                        pst_server_response_msg = (server_response_msg *)pvPortMalloc(sizeof(server_response_msg));
//                    }
//                    while(0 == pst_server_response_msg);
                    pst_server_response_msg = (server_response_msg *)pvPortMalloc(sizeof(server_response_msg));
                    if(NULL != pst_server_response_msg)
                    {
                        #if(dynamic_memory_request_test)
                        DebugPrintf(TRUE,"network_data_analysis_4_malloc%x\n",pst_server_response_msg);
                        #endif
                        memset((uint8_t *)pst_server_response_msg,0,sizeof(server_response_msg));

                        pst_server_response_msg->b_type = cw_NetUpdating;
                        pst_server_response_msg->b_status = cb_net_rsp_success;

                        network_server_response_queue_send(pst_server_response_msg);
                    }
                }
				break;
			}
            break;
            case cw_FTPing:
            {
                ftp_update_start(dw_secquence_temp,
                                 ( net_firmware_update_start_stop * )st_network_rx_data_info.pb_network_data_body_part,
                                 &st_net_system_info_list,
                                 pst_network_rx_data->b_channel_index,
                                 px_network_data_send_queue);
            }
            break;
            case cw_NetTimeSynchrAck:
            {
                uint32_t dw_timestamp = 0;

                if((0 == st_network_rx_data_info.pb_network_data_body_part[0]) &&\
                   (0 == st_network_rx_data_info.pb_network_data_body_part[1]))
                {
                    bytes_to_int_big_to_little_endain(&st_network_rx_data_info.pb_network_data_body_part[2],\
                                                      &dw_timestamp,
                                                      4);
                    DebugPrintf(TRUE,"Netwrok time sync\r\n");
                    refresh_system_timestamp(dw_timestamp);
                    set_device_timestamp(dw_timestamp);
                }
                break;
            }
            break;
            case cw_NetOffLineRealTimeDataAck:
            {
                offline_data_flash_read_shift();
                {
                    server_response_msg *pst_server_response_msg = 0;

//                    do{
//                        pst_server_response_msg = (server_response_msg *)pvPortMalloc(sizeof(server_response_msg));
//                    }
//                    while(0 == pst_server_response_msg);
                    pst_server_response_msg = (server_response_msg *)pvPortMalloc(sizeof(server_response_msg));
                    if(NULL != pst_server_response_msg)
                    {
                        #if(dynamic_memory_request_test)
                        DebugPrintf(TRUE,"network_data_analysis_4_malloc%x\n",pst_server_response_msg);
                        #endif
                        memset((uint8_t *)pst_server_response_msg,0,sizeof(server_response_msg));

                        pst_server_response_msg->b_type = cw_NetOffLineRealTimeData;
                        pst_server_response_msg->b_status = cb_net_rsp_success;

                        network_server_response_queue_send(pst_server_response_msg);
                    }
                }
            }
            break;
            default:
            {
                break;
            }
        }

        {
			vPortFree(pst_network_rx_data->st_data_info.pb_data);//1
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"network_data_analysis_4_free%x\n",pst_network_rx_data->pb_data);
            #endif
			vPortFree(pst_network_rx_data);//2
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"network_data_analysis_5_free%x\n",pst_network_rx_data);
            #endif
        }
    }
}
void network_inquire_cmd_analysis()
{
	network_data_analysis(network_com_inquire_queue_receive,
                          &st_net_system_info_list,
                          network_com_tx_data_queue_send);
}
void network_config_cmd_analysis()
{
	network_data_analysis(network_com_config_queue_receive,
                          &st_net_system_info_list,
                          network_com_tx_data_queue_send);
}
void network_other_cmd_analysis()
{
	network_data_analysis(network_com_other_queue_receive,
                          &st_net_system_info_list,
                          network_com_tx_data_queue_send);
}
uint8_t g_ble_rx_flag = 0;
void network_ble_cmd_analysis()
{
    g_ble_rx_flag = 1;
//	yl_debug_printf("[ble data] ble data analysis!!! Line is: %d \r\n",__LINE__);
	network_data_analysis(network_ble_rx_queue_receive,
                          &st_net_system_info_list,
                          network_ble_tx_queue_send);
//	yl_debug_printf("[ble data] ble tx data queue send Line is: %d \r\n",__LINE__);
    g_ble_rx_flag = 0;
}

void network_data_send(network_data_receive_queue px_network_data_receive_queue)
{
    network_data *pst_network_data_raw = 0;
    network_data *pst_network_data_encode = 0;
    uint16_t w_raw_data_index = 0;
	uint16_t w_data_len = 0;
    uint8_t b_result = FALSE;


    if(TRUE == px_network_data_receive_queue(&pst_network_data_raw,TRUE))	//network_com_tx_data_queue_receive  network_ble_tx_queue_receive
    {
//    	yl_debug_printf("[ble data] ble tx data queue receive, Line is: %d \r\n",__LINE__);
        if(xPortGetFreeHeapSize() < 6000)
	    {
			if(pst_network_data_raw->st_data_info.pb_data)
			{
				vPortFree(pst_network_data_raw->st_data_info.pb_data);
			}

            if(pst_network_data_raw)
			{
				vPortFree(pst_network_data_raw);
			}
	        return;
	    }
        if((NULL == pst_network_data_raw) || (NULL == pst_network_data_raw->st_data_info.pb_data))
        {
			if(pst_network_data_raw->st_data_info.pb_data)
			{
				vPortFree(pst_network_data_raw->st_data_info.pb_data);
                #if(dynamic_memory_request_test)
                DebugPrintf(TRUE,"network_data_send_1_free%x\n",pst_network_data_raw->pb_data);
                #endif
			}

            if(pst_network_data_raw)
			{
				vPortFree(pst_network_data_raw);
                #if(dynamic_memory_request_test)
                DebugPrintf(TRUE,"network_data_send_0_free%x\n",pst_network_data_raw);
                #endif
			}
            return;
        }

		#if(wireless_net_transmission_test)
        {
			for(uint16_t w_cnt_i = 0;w_cnt_i < pst_network_data_raw->st_data_info.w_data_len;w_cnt_i++)
			{
				DebugPrintf(TRUE,"%02X ",pst_network_data_raw->st_data_info.pb_data[w_cnt_i]);
			}
			DebugPrintf(TRUE,"\n");
        }
		#endif

        /*
        if((pstNetChannelData->wTxDataLen != 0) &&\
           (fWiFiCmdReplyOK ||\
           (!fWiFiCmdReplyOK &&\
           ((NULL == bWiFiComType) ||\
           (cbWLINK_R == bWiFiComType) ||\
           (cbCONS_R == bWiFiComType)))))
        */
        do{
//            do{
//                pst_network_data_encode = (network_data *)pvPortMalloc(sizeof(network_data));
//            }while(0 == pst_network_data_encode);
            pst_network_data_encode = (network_data *)pvPortMalloc(sizeof(network_data));
            if(NULL == pst_network_data_encode)
            {
                #if( system_alarm_print)
                DebugPrintf(TRUE,"[ERROR]Net send malloc err:1\r\n");
                #endif
                return;
            }
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"network_data_send_0_malloc%x\n",pst_network_data_encode);
            #endif
            memset(pst_network_data_encode,0,sizeof(network_data));

//            do{
////                pst_network_data_encode->pb_data = (uint8_t *)pvPortMalloc(w_net_iot_module_mps * sizeof(uint8_t));
//                pst_network_data_encode->st_data_info.pb_data = (uint8_t *)pvPortMalloc(cb_max_network_data_len * sizeof(uint8_t));
//            }while(0 == pst_network_data_encode->st_data_info.pb_data);
            if(cb_channel_type_wireless_module == pst_network_data_raw->b_channel_index || cb_channel_type_4g_module == pst_network_data_raw->b_channel_index)
            {
				w_data_len = cb_max_network_data_len;
            }
			else if(cb_channel_type_ble_module == pst_network_data_raw->b_channel_index)
			{
				w_data_len = cb_max_ble_data_len;
			}
            pst_network_data_encode->st_data_info.pb_data = (uint8_t *)pvPortMalloc(w_data_len  * sizeof(uint8_t));

            if(NULL == pst_network_data_encode->st_data_info.pb_data)
            {
                if(NULL != pst_network_data_encode)
                {
                    vPortFree(pst_network_data_encode);
                }
                #if( system_alarm_print)
                DebugPrintf(TRUE,"[ERROR]Net send malloc err:2\r\n");
                #endif
                return;
            }
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"network_data_send_1_malloc%x\n",pst_network_data_encode->pb_data);
            #endif
            memset(pst_network_data_encode->st_data_info.pb_data,0,w_data_len );
//            memset(pst_network_data_encode->pb_data,0,w_net_iot_module_mps);

			#if(wireless_net_transmission_test)
			{
				DebugPrintf(TRUE,"network_data_send,raw_data: \n");
				for(uint16_t w_cnt_i = 0;w_cnt_i < pst_network_data_raw->st_data_info.w_data_len;w_cnt_i++)
				{
					DebugPrintf(TRUE,"%02X ",pst_network_data_raw->st_data_info.pb_data[w_cnt_i]);
				}
				DebugPrintf(TRUE,"\n");
			}
			#endif

            b_result = net_data_pack(pst_network_data_raw->st_data_info.pb_data,\
                                     pst_network_data_raw->st_data_info.w_data_len,\
                                     &w_raw_data_index,\
                                     pst_network_data_encode->st_data_info.pb_data,\
                                     &pst_network_data_encode->st_data_info.w_data_len,\
                                     w_data_len  - 1);
            if((FALSE == b_result) ||\
               (NULL == iot_module_send_data))
            {
                vPortFree(pst_network_data_encode->st_data_info.pb_data);
                #if(dynamic_memory_request_test)
                DebugPrintf(TRUE,"network_data_send_2_free%x\n",pst_network_data_raw->pb_data);
                #endif
                vPortFree(pst_network_data_encode);
                #if(dynamic_memory_request_test)
                DebugPrintf(TRUE,"network_data_send_3_free%x\n",pst_network_data_raw);
                #endif

                vPortFree(pst_network_data_raw->st_data_info.pb_data);
                #if(dynamic_memory_request_test)
                DebugPrintf(TRUE,"network_data_send_5_free%x\n",pst_network_data_raw->pb_data);
                #endif
				vPortFree(pst_network_data_raw);
                #if(dynamic_memory_request_test)
                DebugPrintf(TRUE,"network_data_send_4_free%x\n",pst_network_data_raw);
                #endif

				#if(wireless_net_transmission_test)
				DebugPrintf(TRUE,"network_data_send,net_data_pack_fail,iot_module_null,send_active_data\n");
				#endif
                return;
            }

//            {
//                network_iot_user_data_info *pst_iot_user_data =\
//                    (network_iot_user_data_transmit *)pst_network_data_raw->pb_data;
//
//                while(FALSE == network_iot_module_communicate_channel_operate(pst_iot_user_data))
//                {
//                    vTaskDelay(0);
//                }
//            }

            if(cb_channel_type_wireless_module == pst_network_data_raw->b_channel_index)
            {
                while(FALSE == iot_module_send_data(pst_network_data_encode))			//调用 lte_module_user_data_send
                {
                    if(net_sever_reconnect == network_com_with_tbb_device_server_state_get())
                    {
                        vPortFree(pst_network_data_encode->st_data_info.pb_data);
                        vPortFree(pst_network_data_encode);
                        vPortFree(pst_network_data_raw->st_data_info.pb_data);
                        vPortFree(pst_network_data_raw);
                        return;
                    }
                    vTaskDelay(1);
                }
            }
            else if(cb_channel_type_ble_module == pst_network_data_raw->b_channel_index)//mark ble 01
            {
                uint8_t b_result = 0;
//				yl_debug_printf("[ble data] ble send data by usart, Line is: %d \r\n",__LINE__);
                do{
                    b_result = net_ble_send_message(pst_network_data_encode);
                    if(2 == b_result)
                    {
                        vPortFree(pst_network_data_encode->st_data_info.pb_data);
                        vPortFree(pst_network_data_encode);
                        vPortFree(pst_network_data_raw->st_data_info.pb_data);
                        vPortFree(pst_network_data_raw);
                        return;
                    }
                    vTaskDelay(1);
                }while(FALSE == b_result);
            }
			else if(cb_channel_type_4g_module == pst_network_data_raw->b_channel_index)
            {
                while(FALSE == iot_module_send_data(pst_network_data_encode))			//调用 lte_4g_module_user_data_send
                {
                    if(net_sever_reconnect == network_com_with_tbb_device_server_state_get())
                    {
                        vPortFree(pst_network_data_encode->st_data_info.pb_data);
                        vPortFree(pst_network_data_encode);
                        vPortFree(pst_network_data_raw->st_data_info.pb_data);
                        vPortFree(pst_network_data_raw);
                        return;
                    }
                    vTaskDelay(0);
                }
            }
            vPortFree(pst_network_data_encode->st_data_info.pb_data);
            vPortFree(pst_network_data_encode);

			#if(wireless_net_transmission_test)
			{
				extern CirQueue st_slave_ble_module_tx_data_queue;
				DebugPrintf(TRUE,"slave_ble_module_tx_data_queue_send,start_index: %d\n",st_slave_ble_module_tx_data_queue.wEndIndex);
			}
			#endif

			#if(wireless_net_transmission_test)
			{
				DebugPrintf(TRUE,"slave_ble_module_tx_data_queue_send,data: \n");
				for(uint16_t w_cnt_i = 0;w_cnt_i < pst_network_data_encode->w_data_len;w_cnt_i++)
				{
					DebugPrintf(TRUE,"%02X ",pst_network_data_encode->pb_data[w_cnt_i]);
				}
				DebugPrintf(TRUE,"\n");
			}
			#endif

        }while(w_raw_data_index < pst_network_data_raw->st_data_info.w_data_len);

		w_raw_data_index = 0;
        if(cb_channel_type_wireless_module == pst_network_data_raw->b_channel_index)
        {
            display_management_parameter_send(normal_mode,net_led,sever_com,TRUE);
        }

		vPortFree(pst_network_data_raw->st_data_info.pb_data);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"network_data_send_6_free%x\n",pst_network_data_raw->pb_data);
        #endif
		vPortFree(pst_network_data_raw);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"network_data_send_7_free%x\n",pst_network_data_raw);
        #endif
    }
}
uint8_t network_recevie_data(uint16_t w_protocal_cmd,void *pv_data)
{
    if(NULL == pv_data)
    {
        return FALSE;
    }

    network_data *pst_network_data = 0;
    pst_network_data = (network_data *)pv_data;

    if(cb_channel_type_ble_module == pst_network_data->b_channel_index)//mark ble 02
    {
        return network_ble_rx_queue_send(pv_data);
    }
    else if(cb_channel_type_wireless_module == pst_network_data->b_channel_index)
    {
        switch(w_protocal_cmd)
        {
        	case cw_NetInquire_Discrete               :
            case cw_NetInquire                        :
            {
                return network_com_inquire_queue_send(pv_data);
            }break;
            case cw_NetSetting                        :
            case cw_NetControl                        :
            {
                return network_com_config_queue_send(pv_data);
            }break;
            case cw_NetConnect                        :
            case cw_NetConnectAck                     :
            case cw_NetRealTimeData                   :
            case cw_NetRealTimeDataAck                :
            case cw_NetOffLineRealTimeData            :
            case cw_NetOffLineRealTimeDataAck         :
            case cw_NetSettingAck                     :
            case cw_NetControlAck                     :
            case cw_NetInquireAck                     :
            case cw_NetInquire_DiscreteAck            :				
            case cw_NetCostRenew                      :
            case cw_NetCostRenewAck                   :
            case cw_NetUpdateStart_Stop               :
            case cw_NetUpdateStart_StopAck            :
            case cw_FTPingAck                         :
            case cw_NetUpdating                       :
            case cw_NetUpdatingAck                    :
            case cw_NetUpdateFirmList                 :
            case cw_NetUpdateFirmListAck              :
            case cw_NetAskForUpdate                   :
            case cw_NetAskForUpdateAck                :
            case cw_NetIPSet                          :
            case cw_NetIPSetAck                       :
            case cw_NetTimeSynchr                     :
            case cw_NetTimeSynchrAck                  :
            case cw_NetInquireAttribute               :
            case cw_NetInquireAttributeAck            :
            case cw_NetGetAttributeTree               :
            case cw_NetGetAttributeTreeAck            :
            case cw_SendAttributeTreeSeg              :
            case cw_SendAttributeTreeSegACK           :
			case cw_NetGetAttributeTree_Discrete	  :
			case cw_NetGetAttributeTree_DiscreteAck	  :
            case cw_SendOffLineAttributeTreeSeg       :
            case cw_SendOffLineAttributeTreeSegACK    :
            case cw_NetHeartbeat                      :
            case cw_NetHeartbeatAck                   :
            case cw_NetAdvPassChk                     :
            case cw_NetAdvPassChkAck                  :
            case cw_NetSendNewLogs                    :
            case cw_NetSendNewLogsAck                 :
            case cw_NetGetLogs                        :
            case cw_NetGetLogsAck                     :
            case cw_NetGetLogsContinue                :
            case cw_NetGetLogsContinueAck             :
            case cw_NetRealTimeDataActivePush         :
            case cw_NetTestTransparent                :
            case cw_NetTestTransparentAck             :
            case cw_NetTestGetAddrData                :
            case cw_NetTestGetAddrDataAck             :
            case cw_NetTestWriteAddrData              :
            case cw_NetTestWriteAddrDataAck           :
            {
                return network_com_other_queue_send(pv_data);
            }break;
            default:
            {
            }break;
        }
    }

    return FALSE;
}
void network_data_receive(void *pv_data,uint8_t b_channel_index)
{
    uint8_t b_net_data_unpack_staus = cb_net_data_unpack_failed;

    uint16_t w_network_data_fragment_len = 0;
    network_data *pst_network_data_fragment = (network_data *)pv_data;

    #define cw_network_data_buffer_size         cb_max_network_data_len
    static uint16_t w_network_data_len = 0;
    static uint8_t ba_network_data[cw_network_data_buffer_size] = {0,};
    uint16_t w_data_in_index = 0;

    if(0 == pst_network_data_fragment->st_data_info.w_data_len)
    {
        return;
    }
    do
    {
        if((NULL == pst_network_data_fragment) ||\
           (NULL == pst_network_data_fragment->st_data_info.pb_data))
        {
            w_network_data_len = 0;
            return;
        }

        #if(dynamic_memory_request_test)
        if(pst_network_data_fragment->w_data_len > cw_network_data_buffer_size)
        {
            DebugPrintf(TRUE,"network_data_receive_data_fill_1,len: %d,addr: 0x%x\n",pst_network_data_fragment->w_data_len,
                                                                                     ba_network_data);
        }
        #endif

        b_net_data_unpack_staus = net_data_unpack(&pst_network_data_fragment->st_data_info.pb_data[w_data_in_index],\
                                                  pst_network_data_fragment->st_data_info.w_data_len,\
                                                  &w_data_in_index,\
                                                  &ba_network_data[w_network_data_len],\
                                                  &w_network_data_fragment_len);
//		DebugPrintf(TRUE, "the network_data_fragment is :\r");
//		for(uint16_t b_cnt_i = 0; b_cnt_i < w_network_data_fragment_len; b_cnt_i++)
//		{
//			DebugPrintf(TRUE,"%02x ",ba_network_data[w_network_data_fragment_len]);
//		}
		if(1 == b_firmware_update_pacx_dsp_packet_store_finish_flag)
		{
			b_firmware_update_pacx_dsp_packet_store_finish_flag = 2;
			DebugPrintf(TRUE,"net receive net_data_unpack\n");

		}
        if((cb_net_data_unpack_complete == b_net_data_unpack_staus) ||\
           (cb_net_data_unpack_continue == b_net_data_unpack_staus))
        {
            w_network_data_len += w_network_data_fragment_len;

            if(FALSE == check_crc16(&ba_network_data[0],w_network_data_len))
            {
                w_network_data_len = 0;
                return;
            }

            if(xPortGetFreeHeapSize() < 6000)
            {
                #if( system_alarm_print )
                DebugPrintf(TRUE,"[Alarm]Wireless unpack protect\r\n");
                #endif

                w_network_data_len = 0;
                return;
            }

            {
                network_data *pst_network_data = 0;

                {
//                    do{
//                        pst_network_data = (network_data *)pvPortMalloc(sizeof(network_data));
//                    }
//                    while(0 == pst_network_data);
                    pst_network_data = (network_data *)pvPortMalloc(sizeof(network_data));
                    if(NULL == pst_network_data)
                    {
                        w_network_data_len = 0;
                        #if( system_alarm_print)
                        DebugPrintf(TRUE,"[ERROR]Net unpack malloc err:1\r\n");
                        #endif
                        return;
                    }
                    #if(dynamic_memory_request_test)
                    DebugPrintf(TRUE,"network_data_receive_0_malloc%x\n",pst_network_data);
                    #endif
                    memset(pst_network_data,0,sizeof(network_data));

//                    do{
//                        pst_network_data->st_data_info.pb_data = (uint8_t *)pvPortMalloc(w_network_data_len * sizeof(uint8_t));
//                    }
//                    while(0 == pst_network_data->st_data_info.pb_data);
                    pst_network_data->st_data_info.pb_data = (uint8_t *)pvPortMalloc(w_network_data_len * sizeof(uint8_t));
                    if(NULL == pst_network_data->st_data_info.pb_data)
                    {
                        w_network_data_len = 0;
                        if(NULL != pst_network_data)
                        {
                            vPortFree(pst_network_data);
                        }
                        #if( system_alarm_print)
                        DebugPrintf(TRUE,"[ERROR]Net unpack malloc err:2\r\n");
                        #endif
                        return;
                    }
                    #if(dynamic_memory_request_test)
                    DebugPrintf(TRUE,"network_data_receive_1_malloc,%d,%x\n",w_network_data_len,pst_network_data->pb_data);
                    #endif
                    memset(pst_network_data->st_data_info.pb_data,0,w_network_data_len);
                }

                {
                    pst_network_data->b_channel_index = b_channel_index;	//cb_channel_type_wireless_module;
                    pst_network_data->st_data_info.w_data_len = w_network_data_len;

                    uint8_t *pb_source = &ba_network_data[0];
                    uint8_t *pb_target = pst_network_data->st_data_info.pb_data;
                    for(uint16_t w_cnt_i = 0;w_cnt_i < w_network_data_len;w_cnt_i++)
                    {
                        *pb_target = *pb_source;
                        pb_target++;
                        pb_source++;
                    }
                }

                {
                    uint16_t w_protocal_cmd = 0;

                    {
                        network_data_header_part *pst_network_data_header_part = 0;

                        pst_network_data_header_part =\
                            (network_data_header_part *)&ba_network_data[0];

                        if(FALSE == bytes_to_int_big_to_little_endain(pst_network_data_header_part->ba_cmd_type,\
                                                                      (uint32_t *)&w_protocal_cmd,\
                                                                      sizeof(uint16_t)))
                        {
                            w_network_data_len = 0;
                            return;
                        }
                    }

                    if(FALSE == network_recevie_data(w_protocal_cmd,pst_network_data))
                    {
                        vPortFree(pst_network_data->st_data_info.pb_data);//call first
                        #if(dynamic_memory_request_test)
                        DebugPrintf(TRUE,"network_data_receive_4_free%x\n",pst_network_data->pb_data);
                        #endif
                        vPortFree(pst_network_data);//call second
                        #if(dynamic_memory_request_test)
                        DebugPrintf(TRUE,"network_data_receive_5_free%x\n",pst_network_data);
                        #endif
                        return;
                    }
                }
                w_network_data_len = 0;
            }
        }
        else if(cb_net_data_receive_continue == b_net_data_unpack_staus)
        {
            w_network_data_len += w_network_data_fragment_len;
            return;
        }
        else
        {
            w_network_data_len = 0;
            return;
        }
    }while(cb_net_data_unpack_continue == b_net_data_unpack_staus);
}
void network_data_quit()
{
    network_com_tx_data_queue_clear();
    network_com_inquire_queue_clear();
    network_com_config_queue_clear();
    network_com_other_queue_clear();
}
/************************************************************************************************
* Input:
*
* Output:
*
* Logic discription: net data encode len
*
* run time:
************************************************************************************************/
static uint8_t net_data_encode_len(uint16_t w_data_in_len,uint8_t *pb_data_in,uint16_t *pw_data_out_len)
{
    uint16_t w_cnt_i = 0;
    uint16_t w_cnt_j = 0;

    if((0 == pb_data_in) || (0 == pw_data_out_len))
    {
        return FALSE;
    }

    for(w_cnt_i = 0;w_cnt_i < w_data_in_len;w_cnt_i++)
    {
        if(0xFE == *(pb_data_in + w_cnt_i))
        {
            //0xFE -> 0xFD 0x01
            w_cnt_j += 2;
        }
        else if(0xFD == *(pb_data_in + w_cnt_i))
        {
            //0xFD -> 0xFD 0x00
            w_cnt_j += 2;
        }
        else
        {
            //common data
            w_cnt_j++;
        }
    }

    *pw_data_out_len = w_cnt_j;

    return TRUE;
}
/************************************************************************************************
* Input:
*
* Output:
*
* Logic discription: Encode real buffer to network buffer (0xFE -> 0xFD 0x01)
*                                                         (0xFD -> 0xFD 0x00)
* run time:
************************************************************************************************/
static uint8_t net_data_encode(uint16_t w_data_in_len,uint8_t *pb_data_in,uint8_t *pb_data_out,uint16_t *pw_data_out_len)
{
    uint16_t w_cnt_i = 0;
    uint16_t w_cnt_j = 0;

    if((0 == pb_data_in) || (0 == pb_data_out) || (0 == pw_data_out_len))
    {
        return FALSE;
    }

    for(w_cnt_i = 0; w_cnt_i < w_data_in_len; w_cnt_i++)
    {
        if(0xFE == *(pb_data_in + w_cnt_i))
        {
            //0xFE -> 0xFD 0x01
            *(pb_data_out + w_cnt_j) = 0xFD;
            w_cnt_j++;
            *(pb_data_out + w_cnt_j) = 0x01;
            w_cnt_j++;
        }
        else if(0xFD == *(pb_data_in + w_cnt_i))
        {
            //0xFD -> 0xFD 0x00
            *(pb_data_out + w_cnt_j) = 0xFD;
            w_cnt_j++;
            *(pb_data_out + w_cnt_j) = 0x00;
            w_cnt_j++;
        }
        else
        {
            //common data
            *(pb_data_out + w_cnt_j) = *(pb_data_in + w_cnt_i);
            w_cnt_j++;
        }
    }

    *pw_data_out_len = w_cnt_j;

    return TRUE;
}
/************************************************************************************************
* Input:
*
* Output: output data length
*
* Logic discription: to split network output data(after encode)
*
* run time:
************************************************************************************************/
uint8_t net_data_pack(uint8_t *pb_data_in,\
                      uint16_t w_data_in_len,\
                      uint16_t *pw_data_in_index,\
                      uint8_t *pb_data_out,\
                      uint16_t *pw_data_out_len,\
                      uint16_t w_encode_data_max_len)
{
    if((0 == pb_data_in) || (0 == pw_data_in_index) || (0 == pb_data_out) || (0 == pw_data_out_len))
    {
        return FALSE;
    }

    uint16_t w_data = 0;
    uint16_t w_data_out_index = 0;

    //if the first byte of raw data
    if(0 == (*pw_data_in_index))
    {
        //first byte for 0xFE
        pb_data_out[w_data_out_index++] = 0xFE;

        //buffer length after encode
        {
            uint8_t ba_data_temp[2] = {0,};

            if(net_data_encode_len(w_data_in_len,pb_data_in,&w_data))
            {
                if(int_to_bytes_big_endain(ba_data_temp,\
                                           w_data,\
                                           sizeof(uint16_t)) &&\
                   net_data_encode(2,ba_data_temp,&pb_data_out[w_data_out_index],&w_data))
                {
                     w_data_out_index += w_data;
                }
                else
                {
                    return FALSE;
                }
            }
            else
            {
                return FALSE;
            }
        }
    }

    while(w_data_out_index < w_encode_data_max_len)
    {
        if((*pw_data_in_index) < w_data_in_len)
        {
            if(FALSE == net_data_encode_len(1,&pb_data_in[(*pw_data_in_index)],&w_data))
            {
                return FALSE;
            }

            //buffer is not too large
            if((w_data_out_index + w_data) < w_encode_data_max_len)
            {
                if(net_data_encode(1,&pb_data_in[(*pw_data_in_index)],&pb_data_out[w_data_out_index],&w_data))
                {
                    w_data_out_index += w_data;
                    (*pw_data_in_index)++;
                }
                else
                {
                    return FALSE;
                }
            }
            else
            {
                *pw_data_out_len = w_data_out_index;
                return TRUE;
            }
        }
        else
        {
            pb_data_out[w_data_out_index++] = 0xFE;
            *pw_data_out_len = w_data_out_index;
            return TRUE;
        }
    }

    return FALSE;
}
/************************************************************************************************
* Input:
*
* Output:
*
* Logic discription: Decode network data buffer to real data (0xFD 0x01 -> 0xFE)
*                                                            (0xFD 0x00 -> 0xFD)
* run time:
************************************************************************************************/
/*
#define net_data_decode_failed          0
#define net_data_decode_success         1
#define net_data_decode_wait            2
INT8U net_data_decode_1_byte(uint16_t w_data_in_len,uint8_t *pb_data_in,uint8_t *pb_data_out,uint16_t *pw_data_out_len)
{
    uint16_t w_cnt_i = 0;
    uint16_t w_cnt_j = 0;

    if((0 == pb_data_in) || (0 == pb_data_out) || (0 == pw_data_out_len))
    {
        return net_data_decode_failed;
    }

    if(0xFD == pb_data_in[w_cnt_i])
    {
        if((w_cnt_i + 1) < w_data_in_len)
        {
            if(0x01 == pb_data_in[w_cnt_i + 1])
            {
                //(0xFE -> 0xFE 0x01)
                *(pb_data_out + w_cnt_j) = 0xFE;
                w_cnt_j++;
            }
            else if(0x00 == pb_data_in[w_cnt_i + 1])
            {
                //(0xFD -> 0xFD 0x00)
                *(pb_data_out + w_cnt_j) = 0xFD;
                w_cnt_j++;
            }
            else
            {
                *(pb_data_out + w_cnt_j) = 0xFD;
                w_cnt_j++;

                *(pb_data_out + w_cnt_j) = pb_data_in[w_cnt_i + 1];
                w_cnt_j++;
            }
            w_cnt_i++;
        }
        else
        {
            *pw_data_out_len = w_cnt_j;

            return net_data_decode_wait;
        }
    }
    else
    {
        //common data
        *(pb_data_out + w_cnt_j) = pb_data_in[w_cnt_i];
        w_cnt_j++;
    }

    *pw_data_out_len = w_cnt_j;

    return net_data_decode_success;
}
*/
/************************************************************************************************
* Input:
*
* Output:
*
* Logic discription:
*
* run time:
************************************************************************************************/
uint8_t net_data_decode(uint16_t w_code_data_in_len,
                        uint8_t * pb_code_data_in,
                        uint16_t *pw_code_data_in_index,
                        uint8_t *pb_decode_data_out,
                        uint16_t *pw_decode_data_out_index,
                        uint8_t *pb_decode_data_len)
{
    if((NULL == pb_code_data_in) || (NULL == pb_decode_data_out) || (NULL == pw_decode_data_out_index))
    {
        return FALSE;
    }

    static uint8_t f_decode_wait_last = FALSE;
    uint8_t b_decode_data_len = 0;

    if(0 == *pw_decode_data_out_index)
    {
        f_decode_wait_last = FALSE;//... FD FE
    }

    if(f_decode_wait_last)
    {
        f_decode_wait_last = FALSE;

        if(0x01 == pb_code_data_in[*pw_code_data_in_index])
        {
            //(0xFE -> 0xFE 0x01)
            pb_decode_data_out[(*pw_decode_data_out_index)++] = 0xFE;
            b_decode_data_len++;
        }
        else if(0x00 == pb_code_data_in[*pw_code_data_in_index])
        {
            //(0xFD -> 0xFD 0x00)
            pb_decode_data_out[(*pw_decode_data_out_index)++] = 0xFD;
            b_decode_data_len++;
        }
        else
        {
            pb_decode_data_out[(*pw_decode_data_out_index)++] = 0xFD;
            pb_decode_data_out[(*pw_decode_data_out_index)++] = pb_code_data_in[*pw_code_data_in_index + 1];
        }
    }
    else
    {
        if(0xFD == pb_code_data_in[*pw_code_data_in_index])
        {
            if((*pw_code_data_in_index + 1) < w_code_data_in_len)
            {
                if(0x01 == pb_code_data_in[*pw_code_data_in_index + 1])
                {
                    //(0xFE -> 0xFE 0x01)
                    pb_decode_data_out[(*pw_decode_data_out_index)++] = 0xFE;
                    b_decode_data_len++;
                }
                else if(0x00 == pb_code_data_in[*pw_code_data_in_index + 1])
                {
                    //(0xFD -> 0xFD 0x00)
                    pb_decode_data_out[(*pw_decode_data_out_index)++] = 0xFD;
                    b_decode_data_len++;
                }
                else
                {
                    pb_decode_data_out[(*pw_decode_data_out_index)++] = 0xFD;
                    pb_decode_data_out[(*pw_decode_data_out_index)++] = pb_code_data_in[*pw_code_data_in_index + 1];
                }
                (*pw_code_data_in_index)++;
            }
            else
            {
                f_decode_wait_last = TRUE;
                //wait next frame
            }
        }
        else
        {
            pb_decode_data_out[(*pw_decode_data_out_index)++] = pb_code_data_in[*pw_code_data_in_index];
        }
    }

    *pb_decode_data_len = b_decode_data_len;

    return TRUE;
}
/************************************************************************************************
* Input:
*
* Output:
*
* Logic discription: combine data from network to an intact buffer.
*
* run time:
************************************************************************************************/
#define cb_wait_new_frame       0
#define cb_get_frame_len        1
#define cb_get_frame_data       2

uint8_t net_data_unpack(uint8_t *pb_data_in,\
                        uint16_t w_data_in_len,\
                        uint16_t *pw_data_in_index,\
                        uint8_t *pb_data_out,\
                        uint16_t *pw_data_out_len)
{
    if((0 == pb_data_in) || (0 == pb_data_out) || (0 == pw_data_out_len))
    {
        return cb_net_data_unpack_failed;
    }

    static uint16_t w_net_combine_data_index = 0;
    static uint8_t b_net_raw_data_receive_step = cb_wait_new_frame;

    static uint8_t ba_net_raw_data_len[2] = {0,};
    static uint16_t w_net_raw_data_len = 0;

    uint8_t b_decode_data_len_temp = 0;
    static uint16_t w_decode_data_len = 0;

//    static uint8_t f_net_data_decode_wait_last = FALSE;

    uint8_t b_result = cb_net_data_receive_continue;

    for(uint16_t w_data_in_index = 0;w_data_in_index < (w_data_in_len - *pw_data_in_index);w_data_in_index++)
    {
        //combine data start/end 0xFE
        if(0xFE == pb_data_in[w_data_in_index])
        {
            if(0 == w_net_combine_data_index)
            {
                b_net_raw_data_receive_step = cb_get_frame_len;//start new frame
            }
            else
            {
                //check if data length correct
                if(w_net_combine_data_index == (w_net_raw_data_len - w_decode_data_len))
                {
                    b_net_raw_data_receive_step = cb_wait_new_frame;

                    w_decode_data_len = 0;

                    *pw_data_out_len = w_net_combine_data_index;
                    w_net_combine_data_index = 0;

                    if(0xFE == pb_data_in[w_data_in_index + 1])
                    {
                        *pw_data_in_index += w_data_in_index + 1;
                        b_result = cb_net_data_unpack_continue;
                        return b_result;
                    }
                    else
                    {
                        b_result = cb_net_data_unpack_complete;//receive complete frame
                    }
                }
                else
                {
                    w_net_combine_data_index = 0;
                    //start receive network buffer again.
                    b_net_raw_data_receive_step = cb_get_frame_len;//last frame error,contiue new frame

                    w_decode_data_len = 0;

                    b_result = cb_net_data_unpack_failed;
                }
            }
        }
        else
        {
            if(cb_get_frame_len == b_net_raw_data_receive_step)//get data length
            {
                //2bytes data length info
                if(TRUE == net_data_decode(w_data_in_len,
                                           pb_data_in,
                                           &w_data_in_index,
                                           ba_net_raw_data_len,
                                           (uint16_t *)&w_net_combine_data_index,
                                           &b_decode_data_len_temp))
                {
                    w_decode_data_len += b_decode_data_len_temp;

                    if(w_net_combine_data_index >= 2)
                    {
                        w_net_combine_data_index = 0;

                        w_net_raw_data_len = (uint16_t)(ba_net_raw_data_len[0] << 8) | (uint16_t)(ba_net_raw_data_len[1] << 0);

                        if(w_net_raw_data_len <= 0x12)
                        {
                            w_decode_data_len = 0;

                            b_result = cb_net_data_unpack_failed;
                            break;
                        }
                        else
                        {
                            b_net_raw_data_receive_step = cb_get_frame_data;
                        }
                    }
                }

//                if(f_decode_wait_last)
//                {
//                    f_net_data_decode_wait_last = FALSE;
//
//                    if(0x01 == pb_data_in[w_data_in_index])
//                    {
//                        //(0xFE -> 0xFE 0x01)
//                        b_net_raw_data_len[b_net_raw_data_len_index++] = 0xFE;
//                    }
//                    else if(0x00 == pb_data_in[w_data_in_index])
//                    {
//                        //(0xFD -> 0xFD 0x00)
//                        b_net_raw_data_len[b_net_raw_data_len_index++] = 0xFD;
//                    }
//                    else
//                    {
//                        b_net_raw_data_len[b_net_raw_data_len_index++] = 0xFD;
//                        b_net_raw_data_len[b_net_raw_data_len_index++] = pb_data_in[w_data_in_index + 1];
//                    }
//                }
//                else
//                {
//                    if(0xFD == pb_data_in[w_data_in_index])
//                    {
//                        if((w_data_in_index + 1) < w_data_in_len)
//                        {
//                            if(0x01 == pb_data_in[w_data_in_index + 1])
//                            {
//                                //(0xFE -> 0xFE 0x01)
//                                b_net_raw_data_len[b_net_raw_data_len_index++] = 0xFE;
//                            }
//                            else if(0x00 == pb_data_in[w_data_in_index + 1])
//                            {
//                                //(0xFD -> 0xFD 0x00)
//                                b_net_raw_data_len[b_net_raw_data_len_index++] = 0xFD;
//                            }
//                            else
//                            {
//                                b_net_raw_data_len[b_net_raw_data_len_index++] = 0xFD;
//                                b_net_raw_data_len[b_net_raw_data_len_index++] = pb_data_in[w_data_in_index + 1];
//                            }
//                            w_data_in_index++;
//                        }
//                        else
//                        {
//                            f_net_data_decode_wait_last = TRUE;
//                            //wait next frame
//                        }
//                    }
//                    else
//                    {
//                        b_net_raw_data_len[b_net_raw_data_len_index++] = pb_data_in[w_data_in_index];
//                    }
//                }

//                //2bytes data length info
//                if(b_net_raw_data_len_index >= 2)
//                {
//                    b_net_raw_data_len_index = 0;
//
//                    w_net_raw_data_len = (uint16_t)(b_net_raw_data_len[0] << 8) | (uint16_t)(b_net_raw_data_len[1] << 0);
//
//                    if(w_net_raw_data_len <= 0x12)
//                    {
//                        b_result = cb_net_data_unpack_failed;
//                        break;
//                    }
//                    else
//                    {
//                        b_net_raw_data_receive_step = cb_get_frame_data;
//                    }
//                }
            }
            else if(cb_get_frame_data == b_net_raw_data_receive_step)//receiving data
            {
                if(w_net_combine_data_index >= w_net_raw_data_len)
                {
                    w_net_combine_data_index = 0;
                    w_decode_data_len = 0;
                    b_result = cb_net_data_unpack_failed;
                    break;
                }
                else
                {
                    if(TRUE == net_data_decode(w_data_in_len,
                                               pb_data_in,
                                               &w_data_in_index,
                                               pb_data_out,
                                               &w_net_combine_data_index,
                                               &b_decode_data_len_temp))
                    {
                        w_decode_data_len += b_decode_data_len_temp;
                    }
                    else
                    {
                        w_decode_data_len = 0;
                        b_result = cb_net_data_unpack_failed;
                        w_net_combine_data_index = 0;
                        break;
                    }
//                    if(f_net_data_decode_wait_last)
//                    {
//                        f_net_data_decode_wait_last = FALSE;
//
//                        if(0x01 == pb_data_in[w_data_in_index])
//                        {
//                            //(0xFE -> 0xFE 0x01)
//                            pb_data_out[w_net_combine_data_index++] = 0xFE;
//                        }
//                        else if(0x00 == pb_data_in[w_data_in_index])
//                        {
//                            //(0xFD -> 0xFD 0x00)
//                            pb_data_out[w_net_combine_data_index++] = 0xFD;
//                        }
//                        else
//                        {
//                            pb_data_out[w_net_combine_data_index++] = 0xFD;
//                            pb_data_out[w_net_combine_data_index++] = pb_data_in[w_data_in_index + 1];
//                        }
//                    }
//                    else
//                    {
//                        //save 2bytes data length info in pb_data_out firstly
//                        if(0xFD == pb_data_in[w_data_in_index])
//                        {
//                            if((w_data_in_index + 1) < w_data_in_len)
//                            {
//                                if(0x01 == pb_data_in[w_data_in_index + 1])
//                                {
//                                    //(0xFE -> 0xFE 0x01)
//                                    pb_data_out[w_net_combine_data_index++] = 0xFE;
//                                }
//                                else if(0x00 == pb_data_in[w_data_in_index + 1])
//                                {
//                                    //(0xFD -> 0xFD 0x00)
//                                    pb_data_out[w_net_combine_data_index++] = 0xFD;
//                                }
//                                else
//                                {
//                                    pb_data_out[w_net_combine_data_index++] = 0xFD;
//                                    pb_data_out[w_net_combine_data_index++] = pb_data_in[w_data_in_index + 1];
//                                }
//                                w_data_in_index++;
//                            }
//                            else
//                            {
//                                f_net_data_decode_wait_last = TRUE;
//                                //wait next frame
//                            }
//                        }
//                        else
//                        {
//                            pb_data_out[w_net_combine_data_index++] = pb_data_in[w_data_in_index];
//                        }
//                    }
                }
            }
            else
            {
                //Find the frame header
                if(0xFE == pb_data_in[w_data_in_index])
                {
                    b_net_raw_data_receive_step = cb_get_frame_len;
                    w_net_combine_data_index = 0;
                }
            }
        }
    }

    if(cb_net_data_receive_continue == b_result)
    {
        *pw_data_out_len = w_net_combine_data_index;
        w_net_combine_data_index = 0;
    }

    return b_result;
}

/************************************************************************************************
* Input:
*
* Output:
*
* Logic discription: net data decryption
*
* run time:
************************************************************************************************/
static uint8_t net_data_decryption(uint16_t w_data_len,uint8_t *pb_data_in,uint8_t *pb_data_out,uint8_t *pb_keys)
{
    if((0 == pb_data_in) || (0 == pb_data_out) || (0 == pb_keys))
    {
        return FALSE;
    }

    for(uint16_t w_cnt_i = 0;w_cnt_i < w_data_len;w_cnt_i++)
    {
        pb_data_out[w_cnt_i] = (uint8_t)((((~(pb_data_in[w_cnt_i])) ^ pb_keys[3]) + pb_keys[2] - pb_keys[1]) ^ pb_keys[0]);
    }

    #if (local_offline_ble_print)
	TickType_t tick  = xTaskGetTickCount();
	yl_debug_printf("[%02d:%02d:%02d:%03d]Rx\n",
                    (tick / (1000 * 60 * 60)) % 24,
                    (tick / (1000 * 60)) % 60,
                    (tick / 1000) % 60,
                    tick % 1000);
	yl_debug_printf("net_data_decryption: ");
    for(uint16_t w_cnt_i = 0;w_cnt_i < w_data_len;w_cnt_i++)
    {
        yl_debug_printf("%02X ",pb_data_out[w_cnt_i]);
    }
    yl_debug_printf("\n");
    #endif

	return TRUE;
}
