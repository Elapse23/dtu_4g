#include <string.h>
#include <stdlib.h>
#include "net_firmware_update.h"
#include "network_protocal_cmd_macro.h"
#include "application_general_func.h"
#include "application_general_macro.h"
#include "network_protocal_data_assembly.h"
#include "net_firmware_update_info.h"
//#include "Bootloader.h"
#include "network_local_firmware_update.h"
#include "printf_custom.h"
#include "net_device_data_cklcd.h"
#include "net_device_data_access.h"
#include "BootloaderApp.h"
#include "Eeprom.h"


#define cb_firmware_update_stop				1
#define cb_firmware_update_start			2

device_update_info_current st_device_update_info_current =
{
    .pst_net_device_update_info_current = 0,
    .b_update_type = cb_firmware_format_none,
};
#define UPDATE_NET_INFO         st_device_update_info_current.pst_net_device_update_info_current->st_net_firmware_info
#define UPDATE_CURRENT_CURRENT  st_device_update_info_current.pst_net_device_update_info_current->st_net_firmware_info.dw_packets_receive_current
#define UPDATE_CURRENT_TOTAL    st_device_update_info_current.pst_net_device_update_info_current->st_net_firmware_info.dw_packets_receive_total
#define UPDATE_CURRENT_FORMAT   st_device_update_info_current.pst_net_device_update_info_current->st_net_firmware_info.b_packet_format
#define UPDATE_CURRENT_MD5      st_device_update_info_current.pst_net_device_update_info_current->st_net_firmware_info.ba_firmware_id
#define UPDATE_DATA             pst_firmware_packet->st_firmware_packet_pacx.un_pacx_packet.st_pacx_packet_data.b_packet_data
#define UPDATE_INFO             pst_firmware_packet->st_firmware_packet_pacx.un_pacx_packet.st_pacx_packet_info





uint8_t net_firmware_update_continue_check(uint16_t w_firewarm_id_len,
										   uint8_t *pb_firmware_id,
										   uint32_t dw_firmware_packets,
                                           uint8_t b_packet_format,
                                           uint8_t b_update_type,
										   net_system_info_list *pst_net_system_info_list)
{
	if((NULL == pb_firmware_id) || (NULL == pst_net_system_info_list))
	{
		return FALSE;
	}

    if(NULL == memcpy(st_device_update_info_current.ba_firmware_id,pb_firmware_id,w_firewarm_id_len))
    {
        return FALSE;
    }

    st_device_update_info_current.dw_packets_number = dw_firmware_packets;
    st_device_update_info_current.b_packet_format = b_packet_format;

    if(st_device_update_info_current.pst_net_device_update_info_current != NULL)
    {
        if((dw_firmware_packets == st_device_update_info_current.
                                   pst_net_device_update_info_current->
                                   st_net_firmware_info.
                                   dw_packets_receive_total) &&\
           (0 == memcmp(pb_firmware_id,
                        st_device_update_info_current.
                        pst_net_device_update_info_current->
                        st_net_firmware_info.
                        ba_firmware_id,
                        w_firewarm_id_len)))
        {
            return TRUE;
        }
    }
//    else
//    {
//        return FALSE;
//    }

//    for(uint16_t w_cnt_i = 0;w_cnt_i < pst_net_system_info_list->w_length;w_cnt_i++)
//    {
//        if(pst_net_system_info_list->pst_system_info[w_cnt_i].b_type ==\
//           pst_net_system_info_list->b_current_type)
//        {
//
//            for(uint16_t w_cnt_j = 0;w_cnt_j < pst_net_system_info_list->
//                                               pst_system_info[w_cnt_i].
//                                               pst_net_device_update_info_list->
//                                               w_length;
//                                     w_cnt_j++)
//            {
//                if((memcmp(((net_device_update_info *)((pst_net_system_info_list->
//                                                        pst_system_info[w_cnt_i].
//                                                        pst_net_device_update_info_list->pst_net_device_update_info) + w_cnt_j))->\
//                                                        st_net_firmware_info.ba_firmware_id,\
//                                                        pb_firmware_id,
//                                                        w_firewarm_id_len) !=	NULL) &&\
//                   ((((net_device_update_info *)((pst_net_system_info_list->
//                                                  pst_system_info[w_cnt_i].
//                                                  pst_net_device_update_info_list->pst_net_device_update_info) + w_cnt_j))->\
//                    st_net_firmware_info.dw_packets_receive_total) == dw_firmware_packets))
//                {
//                    st_device_update_info_current.pst_net_device_update_info_current =\
//                        (net_device_update_info *)((pst_net_system_info_list->
//                                                    pst_system_info[w_cnt_i].
//                                                    pst_net_device_update_info_list->pst_net_device_update_info) + w_cnt_j);
//                    return TRUE;
//                }
//            }
//        }
//        else if((w_cnt_i + 1) >= pst_net_system_info_list->w_length)
//        {
//            st_device_update_info_current.pst_net_device_update_info_current = 0;
//            break;
//        }
//    }

    if(cb_firmware_format_pac == b_update_type)
    {
        st_device_update_info_current.b_update_type = b_update_type;
    }
    else
    {
        st_device_update_info_current.b_update_type = cb_firmware_format_none;
    }
    #if( update_status_print )
    	DebugPrintf(TRUE,"[Update]st_device_update_info_current.pst_net_device_update_info_current = NULL\r\n");
    #endif
    st_device_update_info_current.pst_net_device_update_info_current = NULL;

	return FALSE;
}

extern uint16_t ftp_updata_package_num_all;
extern uint8_t ftp_ready_flag;
uint8_t ftp_updata_task_start_flag = 0;
extern uint8_t flash_erase_sector_all_flag;
extern uint8_t g_update_status;
extern uint8_t gb_ble_updata_status;
extern uint8_t gb_wifi_updata_status;
uint8_t ftp_update_start(uint32_t dw_secquence,
                         net_firmware_update_start_stop *pst_net_firmware_update_start_stop,
                         net_system_info_list *pst_net_system_info_list,
                         uint8_t b_channel_index,
                         network_data_send_queue px_network_data_send_queue)
{
    uint8_t b_response_cmd = cbNetCmdOK;
    uint32_t dw_packets_total;
    uint8_t ba_packets_current[4] = {
        0,
    };
	static net_device_info *pst_net_device_info = 0;
	uint32_t dw_master_id = 0x001A11;
	bytes_to_int_big_to_little_endain(pst_net_firmware_update_start_stop->ba_device_id,
								  (uint32_t *)&st_device_update_info_current.w_device_type,2);
    net_get_device_info((dw_master_id),
                        (net_device_info **)&pst_net_device_info,
                        &st_net_system_info_list);
    if((pst_net_device_info->pst_net_device_status->b_communicate_status != cb_net_device_add)  && (0x006B != st_device_update_info_current.w_device_type && 0x0034 != st_device_update_info_current.w_device_type))
    {
		b_response_cmd = cbStartUpdateError;
	}
	else if (cb_firmware_update_stop == pst_net_firmware_update_start_stop->b_status)
	{
		b_response_cmd = cbNetCmdOK;
	}
	else if(g_update_status == cw_system_no_update && 0 == gb_wifi_updata_status)
	{
		if (cb_firmware_update_start == pst_net_firmware_update_start_stop->b_status)
		{
		
			if (TRUE
				== bytes_to_int_big_to_little_endain(
					pst_net_firmware_update_start_stop->ba_firmware_packet_num, &dw_packets_total, 4))
			{
				bytes_to_int_big_to_little_endain(pst_net_firmware_update_start_stop->ba_device_id,
												  ( uint32_t * )&st_device_update_info_current.w_device_type,
												  2);
		
				if (TRUE
					== net_firmware_update_continue_check(cb_firmware_id_max_len,
														  pst_net_firmware_update_start_stop->ba_firmware_id,
														  dw_packets_total,
														  pst_net_firmware_update_start_stop->b_type,
														  pst_net_firmware_update_start_stop->b_update_type,
														  pst_net_system_info_list))
				{
					if (FALSE
						== int_to_bytes_big_endain(ba_packets_current,
												   st_device_update_info_current.pst_net_device_update_info_current
														   ->st_net_firmware_info.dw_packets_receive_current
													   - 1,
												   4))
					{
						b_response_cmd = cbStartUpdateError;
					}
				}
			}
			else
			{
				b_response_cmd = cbStartUpdateError;
			}
		
			if(cbNetCmdOK == b_response_cmd)
			{
				extern uint8_t g_update_status;
				uint16_t w_data_len = 2;
				uint32_t dw_data = 0;
				dw_data = cw_system_receiving_firmware;
				net_system_data_access(cb_device_data_access_data_type_status,//cb_device_data_access_data_type_status
									   cw_sys_status_data_addr_update_status,
									   &w_data_len,
									   (uint8_t *)&dw_data,
									   cb_device_data_access_operate_type_write);
				g_update_status = cw_system_receiving_firmware;
				gb_ble_updata_status = 1;
				ftp_updata_package_num_all = dw_packets_total;
				yl_debug_printf("[ble update] ftp_updata_package_num_all = dw_packets_total, Line is: %d \r\n",__LINE__);
				if(ftp_ready_flag == 1)
				{
					ftp_updata_task_start_flag = 1;
				}
				else
				{
					flash_erase_sector_all_flag = 1;
					ftp_updata_task_start_flag = 0;
					ftp_updata_package_num_all = 0;
					gb_ble_updata_status = 0;
					g_update_status = cw_system_no_update;
					b_response_cmd = cbFTPEraseFlash;
				}
			}
		}

	}
	else
	{
		b_response_cmd = cbStartUpdateError;
	}

    return net_ack_assembly(0x04,
                            ba_packets_current,
                            cw_FTPingAck,
                            b_response_cmd,
                            dw_secquence,
                            b_channel_index,
                            px_network_data_send_queue);
}
extern uint8_t ftp_updata_task_delete;
uint8_t gb_wifi_updata_status = 0;
uint8_t net_firmware_update_start(uint32_t dw_secquence,
								  net_firmware_update_start_stop *pst_net_firmware_update_start_stop,\
							      net_system_info_list *pst_net_system_info_list,
                                  uint8_t b_channel_index,
                                  network_data_send_queue px_network_data_send_queue)
{
	uint8_t b_response_cmd = cbNetCmdOK;
	uint32_t dw_packets_total;
	uint8_t ba_packets_current[4] = {0,};
	static net_device_info *pst_net_device_info = 0;
	uint32_t dw_master_id = 0x001A11;
	bytes_to_int_big_to_little_endain(pst_net_firmware_update_start_stop->ba_device_id,
								  (uint32_t *)&st_device_update_info_current.w_device_type,2);
    net_get_device_info((dw_master_id),
                        (net_device_info **)&pst_net_device_info,
                        &st_net_system_info_list);
    if(pst_net_device_info->pst_net_device_status->b_communicate_status != cb_net_device_add && (0x006B != st_device_update_info_current.w_device_type && 0x0034 != st_device_update_info_current.w_device_type))
    {
		b_response_cmd = cbStartUpdateError;
	}
	else if(cb_firmware_update_stop == pst_net_firmware_update_start_stop->b_status)
	{
		b_response_cmd = cbNetCmdOK;
	}
	else if((g_update_status == cw_system_no_update && 0 == gb_ble_updata_status)
		||(g_update_status != cw_system_no_update && 1 == gb_wifi_updata_status && 0 == gb_ble_updata_status))	//断点续传
	{

		if(cb_firmware_update_start == pst_net_firmware_update_start_stop->b_status)
		{
	
			if(TRUE == bytes_to_int_big_to_little_endain(pst_net_firmware_update_start_stop->ba_firmware_packet_num,&dw_packets_total,4))
			{
				bytes_to_int_big_to_little_endain(pst_net_firmware_update_start_stop->ba_device_id,
												  (uint32_t *)&st_device_update_info_current.w_device_type,2);
			  	gb_wifi_updata_status = 1;
				if(TRUE == net_firmware_update_continue_check(cb_firmware_id_max_len,\
															  pst_net_firmware_update_start_stop->ba_firmware_id,\
															  dw_packets_total,\
															  pst_net_firmware_update_start_stop->b_type,
															  pst_net_firmware_update_start_stop->b_update_type,
															  pst_net_system_info_list))
				{
					if(FALSE == int_to_bytes_big_endain(ba_packets_current,
														st_device_update_info_current.
														pst_net_device_update_info_current->
														st_net_firmware_info.
														dw_packets_receive_current - 1,
														4))
					{
						b_response_cmd = cbStartUpdateError;
						gb_wifi_updata_status = 0;
					}														
				}
	
			}
			else
			{
				b_response_cmd = cbStartUpdateError;
				gb_wifi_updata_status = 0;
			}
		}
	}
	else
	{
		b_response_cmd = cbStartUpdateError;
		gb_wifi_updata_status = 0;
	}


	return net_ack_assembly(0x04,
							ba_packets_current,
							cw_NetUpdateStart_StopAck,
							b_response_cmd,
							dw_secquence,
                            b_channel_index,
                            px_network_data_send_queue);
}

/*************************************************************************************
* Input: bFirmTypeStrLen: firmware type string length(like "Omega4040 00");
*        pbFirmwareVerStr: point of full firmware version string
*        pbBootVerStr: point of bootloader string (like "b00.01")
*
* Output: firmware transfering percentage
*
* Logic discription: calculate current firmware transfering percentage
*
* run time:
*************************************************************************************/
INT8U sMatchBootVersion(INT8U *pbCurrentVerStr, INT8U *pbNewVerStr)
{
    INT8U i = 0;
    INT8U j = 0;
    INT16U wTemp1, wTemp2;
    //whether current firmware string have boot version
    INT8U fCurrentHaveBootVer = false;
    //whether new firmware string have boot version
    INT8U fNewHaveBootVer = false;
    INT8U bFirmTypeStrLen = 0;
    if((0 == pbCurrentVerStr) || (0 == pbNewVerStr))
    {
        return false;
    }//mark2019/10/23 05
    //serching and calculate MCU type strlen
    i = 0;
    while(i < cbFirmwareVerStrLen)
    {
        if((pbCurrentVerStr[i] >= '0') && (pbCurrentVerStr[i] <= '9'))
        {
            break;
        }
        i++;
    }

    //7 is "4040 00" for "Omega4040 00"
    bFirmTypeStrLen = i;

    //serching bootloader version in firmware version string
    j = bFirmTypeStrLen;
    while(j < cbFirmwareVerStrLen)
    {
        //bootloader version start at 'b'
        if('b' == pbCurrentVerStr[j])
        {
            break;
        }
        j++;
    }

    if((j < cbFirmwareVerStrLen) && ((j + 6) < cbFirmwareVerStrLen))
    {
        fCurrentHaveBootVer = true;
    }

    //serching bootloader version in firmware version string
    i = bFirmTypeStrLen;
    while(i < cbFirmwareVerStrLen)
    {
        //bootloader version start at 'b'
        if('b' == pbNewVerStr[i])
        {
            break;
        }
        i++;
    }

    if((i < cbFirmwareVerStrLen) && ((i + 6) < cbFirmwareVerStrLen))
    {
        fNewHaveBootVer = true;
    }

    //6 bytes bootloader version string
    if(fNewHaveBootVer && fCurrentHaveBootVer)
    {
        //match boot version "bxx."
        if(0 == strncmp((char *)(&pbCurrentVerStr[j]),
                        (char *)(&pbNewVerStr[i]),
                        4))
        {
            if((pbCurrentVerStr[j+4] >= '0') && (pbCurrentVerStr[j+4] <= '9') &&
               (pbCurrentVerStr[j+5] >= '0') && (pbCurrentVerStr[j+5] <= '9') &&
               (pbNewVerStr[i+4] >= '0') && (pbNewVerStr[i+4] <= '9') &&
               (pbNewVerStr[i+5] >= '0') && (pbNewVerStr[i+5] <= '9'))
            {
                wTemp1 = (INT16U)(((pbCurrentVerStr[j+4] - 0x30) * 10) + (pbCurrentVerStr[j+5] - 0x30));
                wTemp2 = (INT16U)(((pbNewVerStr[i+4] - 0x30) * 10) + (pbNewVerStr[i+5] - 0x30));

                if(wTemp2 >= wTemp1)
                {
                    //MCU bootloader version is high than PAC firmware's bootloader version
                    return true;
                }
            }
        }
    }
    else if(fNewHaveBootVer && (!fCurrentHaveBootVer))
    {
        if((pbNewVerStr[i+4] >= '0') && (pbNewVerStr[i+4] <= '9') &&
           (pbNewVerStr[i+5] >= '0') && (pbNewVerStr[i+5] <= '9'))
        {
            return true;
        }
    }
    else if((!fCurrentHaveBootVer) && (!fNewHaveBootVer))
    {
        for(i = 0;i < cbFirmwareVerStrLen;i++)
        {
            if((((pbNewVerStr[i] >= 'a') && (pbNewVerStr[i] <= 'z')) ||
                ((pbNewVerStr[i] >= 'A') && (pbNewVerStr[i] <= 'Z'))) ||
               (((pbCurrentVerStr[j] >= 'a') && (pbCurrentVerStr[j] <= 'z')) ||
                ((pbCurrentVerStr[j] >= 'A') && (pbCurrentVerStr[j] <= 'Z'))))
            {
                if(pbCurrentVerStr[j] != pbNewVerStr[i])
                {
                    return false;
                }
            }
            else
            {
                break;
            }
        }
        return true;
    }
/*
    else if((!fNewHaveBootVer) && fCurrentHaveBootVer)
    {
        return false;
    }
*/
    return false;
}


uint8_t net_firmware_update_traget_pairing_pac(firmware_packet *pst_firmware_packet,\
                                               net_system_info_list *pst_net_system_info_list)
{
	if((NULL == pst_firmware_packet) || (NULL == pst_net_system_info_list))
	{
		return FALSE;
	}

    if(st_device_update_info_current.pst_net_device_update_info_current != NULL)
    {
        return FALSE;
    }

    for(uint16_t w_cnt_i = 0;w_cnt_i < pst_net_system_info_list->w_length;w_cnt_i++)
    {
        if(pst_net_system_info_list->pst_system_info[w_cnt_i].b_type ==\
           pst_net_system_info_list->b_current_type)
        {

            for(uint16_t w_cnt_j = 0;w_cnt_j < pst_net_system_info_list->
                                               pst_system_info[w_cnt_i].
                                               pst_net_device_update_info_list->
                                               w_length;
                                     w_cnt_j++)
            {
//            	uint8_t b_mcu_flag = 0;
//				if(check_system_type(cb_inv_type_tyrann))
//				{
//					b_mcu_flag = 1;
//				}
//				else
//				{
//					if(pst_firmware_packet->st_firmware_packet_pac.st_pac_packet.st_pac_packet_data.st_firmware_pac_info.b_mcu_type ==
//					   ((net_device_update_info *)((pst_net_system_info_list->
//													pst_system_info[w_cnt_i].
//													pst_net_device_update_info_list->pst_net_device_update_info) + w_cnt_j))->b_mcu_type)
//					{
//						b_mcu_flag = 1;
//					}
//				}
//				if(1 == b_mcu_flag)
				if(1)
                {
                    for(uint16_t w_cnt_k = 0;w_cnt_k < pst_net_system_info_list->pst_system_info[w_cnt_i].pst_device_info_list->w_length;w_cnt_k++)
                    {
                        if(((net_device_update_info *)((pst_net_system_info_list->
                                                        pst_system_info[w_cnt_i].
                                                        pst_net_device_update_info_list->
                                                        pst_net_device_update_info) + w_cnt_j))->w_device_type ==
                           ((*pst_net_system_info_list->
                             pst_system_info[w_cnt_i].
                             pst_device_info_list->
                             pst_device_info[w_cnt_k].
                             pdw_device_id & 0xFFFF00) >> 8))
                        {
                            if(st_device_update_info_current.w_device_type ==
                                ((*pst_net_system_info_list->
                                 pst_system_info[w_cnt_i].
                                 pst_device_info_list->
                                 pst_device_info[w_cnt_k].
                                 pdw_device_id & 0xFFFF00) >> 8))
                            {
								#if( update_status_print )
									DebugPrintf(TRUE,"[Update]st_device_update_info_current.pst_net_device_update_info_current is assigned the value111\r\n");
								#endif

                                st_device_update_info_current.pst_net_device_update_info_current =\
                                    (net_device_update_info *)((pst_net_system_info_list->
                                                                pst_system_info[w_cnt_i].
                                                                pst_net_device_update_info_list->
                                                                pst_net_device_update_info) + w_cnt_j);
								#if( update_status_print )
									   DebugPrintf(TRUE,"[Update]UPDATE_CURRENT_CURRENT:%d\r\n",st_device_update_info_current.pst_net_device_update_info_current->st_net_firmware_info.dw_packets_receive_current);
								#endif

                                return TRUE;
                            }
                        }
                    }
                }
            }
        }
		else if((w_cnt_i + 1) >= pst_net_system_info_list->w_length)
		{
			return FALSE;
		}
    }
    return FALSE;
}

uint8_t net_firmware_update_traget_pairing_pacx(firmware_packet *pst_firmware_packet,\
                                                net_system_info_list *pst_net_system_info_list)
{
	if((NULL == pst_firmware_packet) || (NULL == pst_net_system_info_list))
	{
		return FALSE;
	}

    if(st_device_update_info_current.pst_net_device_update_info_current != NULL)
    {
        return TRUE;
    }

    for(uint16_t w_cnt_i = 0;w_cnt_i < pst_net_system_info_list->w_length;w_cnt_i++)
    {
        if(pst_net_system_info_list->pst_system_info[w_cnt_i].b_type ==\
           pst_net_system_info_list->b_current_type)
        {

            for(uint16_t w_cnt_j = 0;w_cnt_j < pst_net_system_info_list->
                                               pst_system_info[w_cnt_i].
                                               pst_net_device_update_info_list->
                                               w_length;
                                     w_cnt_j++)
            {
                if(pst_firmware_packet->st_firmware_packet_pacx.un_pacx_packet.st_pacx_packet_info.b_mcu_type ==
                   ((net_device_update_info *)((pst_net_system_info_list->
                                                pst_system_info[w_cnt_i].
                                                pst_net_device_update_info_list->pst_net_device_update_info) + w_cnt_j))->b_mcu_type)
                {
                    for(uint16_t w_cnt_k = 0;w_cnt_k < pst_net_system_info_list->pst_system_info[w_cnt_i].pst_device_info_list->w_length;w_cnt_k++)
                    {
                        if(((net_device_update_info *)((pst_net_system_info_list->
                                                        pst_system_info[w_cnt_i].
                                                        pst_net_device_update_info_list->
                                                        pst_net_device_update_info) + w_cnt_j))->w_device_type ==
                           ((*pst_net_system_info_list->
                             pst_system_info[w_cnt_i].
                             pst_device_info_list->
                             pst_device_info[w_cnt_k].
                             pdw_device_id & 0xFFFF00) >> 8))
                        {
                            uint32_t dw_version_int_net = 0;
                            uint32_t dw_version_int_local = 1;
                            uint8_t b_version_str_local[6];

                            memcpy(&b_version_str_local[0],
                                   &pst_net_system_info_list->
                                    pst_system_info[w_cnt_i].
                                    pst_device_info_list->
                                    pst_device_info[w_cnt_k].
                                    pst_net_device_tree_info->
                                    st_device_tree.
                                    b_firmware_version[7],4);

                            memcpy(&b_version_str_local[4],
                                   &pst_net_system_info_list->
                                    pst_system_info[w_cnt_i].
                                    pst_device_info_list->
                                    pst_device_info[w_cnt_k].
                                    pst_net_device_tree_info->
                                    st_device_tree.
                                    b_firmware_version[12],2);


                            dw_version_int_local = atoi((char *)&b_version_str_local[0]);


                            bytes_to_int_big_to_little_endain(pst_firmware_packet->
                                                              st_firmware_packet_pacx.
                                                              un_pacx_packet.
                                                              st_pacx_packet_info.
                                                              ba_firmware_type,
                                                              &dw_version_int_net,3);

							if(dw_version_int_net == dw_version_int_local)
							{							
								st_device_update_info_current.pst_net_device_update_info_current =\
                                    (net_device_update_info *)((pst_net_system_info_list->
                                                                pst_system_info[w_cnt_i].
                                                                pst_net_device_update_info_list->
                                                                pst_net_device_update_info) + w_cnt_j);
                                return TRUE;
							}
                        }
//                        else{
//                            debug_updata_error("\r\n[updata]mcu_id error rec %x local %x\r\n",
//                            ((net_device_update_info *)((pst_net_system_info_list->
//                                                                pst_system_info[w_cnt_i].
//                                                                pst_net_device_update_info_list->
//                                                                pst_net_device_update_info) + w_cnt_j))->w_device_type,
//                           ((pst_net_system_info_list->
//                                     pst_system_info[w_cnt_i].
//                                     pst_device_info_list->
//                                     pst_device_info[w_cnt_k].
//                                     dw_device_id & 0xFFFF00) >> 8)
//                            );
//                        }


                    }
                }
//                else{
//                    debug_updata_error("\r\n[upata]mcu_type error %x ,%x\r\n",
//                    pst_firmware_packet->st_firmware_packet_pacx.un_pacx_packet.st_pacx_packet_info.b_mcu_type,
//
//                    ((net_device_update_info *)((pst_net_system_info_list->
//                                                pst_system_info[w_cnt_i].
//                                                pst_net_device_update_info_list->pst_net_device_update_info) + w_cnt_j))->b_mcu_type
//                    );
//                }
            }
        }
		else if((w_cnt_i + 1) >= pst_net_system_info_list->w_length)
		{
			return FALSE;
		}
    }

	return FALSE;
}

uint8_t net_firmware_update_traget_pairing_pacx_dsp(firmware_packet *pst_firmware_packet,\
                                                    net_system_info_list *pst_net_system_info_list)
{
    if((NULL == pst_firmware_packet) || (NULL == pst_net_system_info_list))
	{
		return FALSE;
	}

    if(st_device_update_info_current.pst_net_device_update_info_current != NULL)
    {
        return TRUE;
    }

    for(uint16_t w_cnt_i = 0;w_cnt_i < pst_net_system_info_list->w_length;w_cnt_i++)
    {
        if(pst_net_system_info_list->pst_system_info[w_cnt_i].b_type ==\
           pst_net_system_info_list->b_current_type)
        {
            for(uint16_t w_cnt_j = 0;w_cnt_j < pst_net_system_info_list->
                                               pst_system_info[w_cnt_i].
                                               pst_net_device_update_info_list->
                                               w_length;
                                                w_cnt_j++)
            {
                if((pst_firmware_packet->st_firmware_packet_pacx_dsp.un_pacx_dsp_packet.st_pacx_dsp_packet_info.b_identification ==
                   ((net_device_update_info *)((pst_net_system_info_list->
                                                pst_system_info[w_cnt_i].
                                                pst_net_device_update_info_list->pst_net_device_update_info) + w_cnt_j))->b_firmware_format) && 
                                                st_device_update_info_current.w_device_type ==
                   ((net_device_update_info *)((pst_net_system_info_list->
                                                pst_system_info[w_cnt_i].
                                                pst_net_device_update_info_list->pst_net_device_update_info) + w_cnt_j))->w_device_type)
                {
                    for(uint16_t w_cnt_k = 0;w_cnt_k < pst_net_system_info_list->pst_system_info[w_cnt_i].pst_device_info_list->w_length;w_cnt_k++)
                    {
                        if(((net_device_update_info *)((pst_net_system_info_list->
                                                        pst_system_info[w_cnt_i].
                                                        pst_net_device_update_info_list->
                                                        pst_net_device_update_info) + w_cnt_j))->w_device_type ==
                           ((*pst_net_system_info_list->
                             pst_system_info[w_cnt_i].
                             pst_device_info_list->
                             pst_device_info[w_cnt_k].
                             pdw_device_id & 0xFFFF00) >> 8))
                        {
                            st_device_update_info_current.pst_net_device_update_info_current =\
                                    (net_device_update_info *)((pst_net_system_info_list->
                                                                pst_system_info[w_cnt_i].
                                                                pst_net_device_update_info_list->
                                                                pst_net_device_update_info) + w_cnt_j);
                            return TRUE;
                        }
                    }
                }
            }
        }
        else if((w_cnt_i + 1) >= pst_net_system_info_list->w_length)
		{
			return FALSE;
		}
    }
    return FALSE;
}
extern uint8_t b_firmware_update_pac_packet_store_finish_flag;

uint8_t net_firmware_update_packet_operate_pac(uint16_t w_firmware_packet_len,\
                                               firmware_packet *pst_firmware_packet,\
                                               net_system_info_list *pst_net_system_info_list)
{
    if((NULL == pst_firmware_packet) || (NULL == pst_net_system_info_list))
    {
        return FALSE;
    }

    uint8_t b_cmd_type = cbFirmwareDataError;
	uint32_t dw_packet_number = 0;

	if(FALSE == bytes_to_int_big_to_little_endain(pst_firmware_packet->st_firmware_packet_pac.ba_package_num,\
												 &dw_packet_number,
												 4))
	{
		return b_cmd_type;
	}

    if(0x0001 == dw_packet_number)//first packet:contain packet info
    {
        if(TRUE == net_firmware_update_traget_pairing_pac(pst_firmware_packet,pst_net_system_info_list))
        {
            if(NULL == st_device_update_info_current.pst_net_device_update_info_current)
            {
                b_cmd_type = cbFirmwareIDNotSupport;
            }
            else
            {
                    UPDATE_CURRENT_CURRENT = 1;
                    if(TRUE == firmware_update_pac_packet_info_store(w_firmware_packet_len - 4,
    																   pst_firmware_packet,
                                                                       &st_device_update_info_current))
                    {
    					b_cmd_type = cbNetCmdOK;
                    }
                    else
                    {
                        b_cmd_type = cbFirmwareDataError;
                    }
            }
        }
    }
    else if(dw_packet_number > 1)//remaining packets:contain packet data
    {
    	if(1 == b_firmware_update_pac_packet_store_finish_flag)
		{
            b_firmware_update_pac_packet_store_finish_flag = 0;
			DebugPrintf(TRUE,"dw_packets_receive_current = dw_packets_receive_total\n");
		}
		
        if((UPDATE_CURRENT_CURRENT) == dw_packet_number){

            if(TRUE == firmware_update_pac_packet_data_store(w_firmware_packet_len - 4,
                                                             pst_firmware_packet,
                                                             &st_device_update_info_current))
            {
                b_cmd_type = cbNetCmdOK;
                if(UPDATE_CURRENT_CURRENT != UPDATE_CURRENT_TOTAL)
                {
                    UPDATE_CURRENT_CURRENT++;
                }
            }
            else
            {
                debug_updata_error("\r\n[updata]flash error %d\r\n",UPDATE_CURRENT_CURRENT);
            }
        }
        else if((UPDATE_CURRENT_CURRENT ) > dw_packet_number)
        {
            b_cmd_type = cbNetCmdOK;
        }
        else
        {
            b_cmd_type = cbFirmwareDataError;
        }
    }
    else
    {
        b_cmd_type = cbUpdatePackageSumError;
    }
    return b_cmd_type;
}
uint8_t net_firmware_update_packet_operate_pacx_dsp(uint16_t w_firmware_packet_len,\
                                                    firmware_packet *pst_firmware_packet,\
                                                    net_system_info_list *pst_net_system_info_list)
{
    if((NULL == pst_firmware_packet) || (NULL == pst_net_system_info_list))
    {
        return FALSE;
    }

    uint8_t b_cmd_type = cbFirmwareDataError;
	uint32_t dw_packet_number = 0;

	if(FALSE == bytes_to_int_big_to_little_endain(pst_firmware_packet->st_firmware_packet_pacx_dsp.ba_packet_number,\
												 &dw_packet_number,
												 4))
	{
		return b_cmd_type;
	}

    if(0x0001 == dw_packet_number)//first packet:contain packet info
    {
        if(TRUE == net_firmware_update_traget_pairing_pacx_dsp(pst_firmware_packet,pst_net_system_info_list))
        {
            if(NULL == st_device_update_info_current.pst_net_device_update_info_current)
            {
                b_cmd_type = cbFirmwareIDNotSupport;
            }
            else
            {
                    UPDATE_CURRENT_CURRENT = 1;
                    if(TRUE == firmware_update_pacx_dsp_packet_info_store(w_firmware_packet_len - 4,
                                                                          pst_firmware_packet,
                                                                          &st_device_update_info_current))
                    {
    					b_cmd_type = cbNetCmdOK;
                    }
                    else
                    {
                        b_cmd_type = cbFirmwareDataError;
                    }
            }
        }
    }
    else if(dw_packet_number > 1)//remaining packets:contain packet data
    {
        if((UPDATE_CURRENT_CURRENT) == dw_packet_number){

            if(TRUE == firmware_update_pacx_dsp_packet_data_store(w_firmware_packet_len - 4,
                                                                  pst_firmware_packet,
                                                                  &st_device_update_info_current))
            {
                b_cmd_type = cbNetCmdOK;
                if(UPDATE_CURRENT_CURRENT != UPDATE_CURRENT_TOTAL)
                {
                    UPDATE_CURRENT_CURRENT++;
                }
            }
            else
            {
                debug_updata_error("\r\n[updata]flash error %d\r\n",UPDATE_CURRENT_CURRENT);
            }
        }
        else if((UPDATE_CURRENT_CURRENT ) > dw_packet_number)
        {
            b_cmd_type = cbNetCmdOK;
        }
        else
        {
            b_cmd_type = cbFirmwareDataError;
        }
    }
    else
    {
        b_cmd_type = cbUpdatePackageSumError;
    }
    return b_cmd_type;
}

#if 1
uint8_t net_firmware_update_packet_operate_pacx(uint16_t w_firmware_packet_len,\
                                                firmware_packet *pst_firmware_packet,\
                                                net_system_info_list *pst_net_system_info_list)
{
    if((NULL == pst_firmware_packet) || (NULL == pst_net_system_info_list))
    {
        return FALSE;
    }

    uint8_t b_cmd_type = cbFirmwareDataError;
	uint32_t dw_packet_number = 0;

	if(FALSE == bytes_to_int_big_to_little_endain(pst_firmware_packet->st_firmware_packet_pacx.ba_packet_number,\
												 &dw_packet_number,
												 4))
	{
		return b_cmd_type;
	}

    if(0x0001 == dw_packet_number)//first packet:contain packet info
    {
        if(TRUE == net_firmware_update_traget_pairing_pacx(pst_firmware_packet,pst_net_system_info_list))
        {
            if(NULL == st_device_update_info_current.pst_net_device_update_info_current)
            {
                b_cmd_type = cbFirmwareIDNotSupport;
            }
            else
            {
                //if(0 != memcmp(UPDATE_CURRENT_MD5,st_device_update_info_current.ba_firmware_id,cb_firmware_id_max_len)){

                    UPDATE_CURRENT_CURRENT = 1;
                    if(TRUE == local_firmware_update_packet_info_store(w_firmware_packet_len - 4,
    																   pst_firmware_packet,
                                                                       &st_device_update_info_current))
                    {
                        debug_updata_info("[updata]config\r\n");
    					b_cmd_type = cbNetCmdOK;
                    }else{
                          debug_updata_error("\r\n[updata]local_firmware_update_packet_info_store %d\r\n");
                    }
                //}else{
                //    debug_updata_info("\r\n[updata]continue %d\r\n");
                //}

            }
        }
        else
        {
            debug_updata_error("\r\n[updata]cbFirmwareIDNotSupport\r\n");
            b_cmd_type = cbFirmwareIDNotSupport;
        }
    }
    else if(dw_packet_number > 1)//remaining packets:contain packet data
    {

        if((UPDATE_CURRENT_CURRENT) == dw_packet_number){

            if(TRUE == local_firmware_update_packet_data_store(w_firmware_packet_len - 4,
                                                               pst_firmware_packet,
                                                               &st_device_update_info_current))
            {
                b_cmd_type = cbNetCmdOK;
                debug_updata_info("[updata]curren %d/%d [%d]\r\n",UPDATE_CURRENT_CURRENT,UPDATE_CURRENT_TOTAL,w_firmware_packet_len);
                if(UPDATE_CURRENT_CURRENT < UPDATE_CURRENT_TOTAL){
                    UPDATE_CURRENT_CURRENT++;
                }
            }else{
                debug_updata_error("\r\n[updata]flash error %d\r\n",UPDATE_CURRENT_CURRENT);
            }
        }  else if((UPDATE_CURRENT_CURRENT ) > dw_packet_number){
            debug_updata_error("\r\n[updata]repeat %d,%d\r\n",dw_packet_number,UPDATE_CURRENT_CURRENT);
            b_cmd_type = cbNetCmdOK;
        }else{
            debug_updata_error("\r\n[updata]advance %d,%d\r\n",dw_packet_number,UPDATE_CURRENT_CURRENT);
           // UPDATE_CURRENT_CURRENT = 0;
            b_cmd_type = cbFirmwareDataError;
        }
    }
	else
	{
		b_cmd_type = cbUpdatePackageSumError;
	}

    return b_cmd_type;
}
#else
#if 0
uint8_t net_firmware_update_packet_operate_pacx(uint32_t dw_secquence,\
                                                uint16_t w_firmware_packet_len,\
                                                firmware_packet *pst_firmware_packet,\
                                                net_system_info_list *pst_net_system_info_list)
{
    uint32_t dw_packet_number = 0;

    if((NULL == pst_firmware_packet) || (NULL == pst_net_system_info_list) ||(w_firmware_packet_len < 4))
    {
        return cbFirmwareDataError;
    }
    if(FALSE == bytes_to_int_big_to_little_endain(pst_firmware_packet->st_firmware_packet_pacx.ba_packet_number,\
                                                                                             &dw_packet_number,
                                                                                             4))
    {
        return cbFirmwareDataError;
    }
    //big //debug_updata_sector(pst_firmware_packet->st_firmware_packet_pacx.ba_packet_number,4);
    debug_updata_test("[updata] packet%d\r\n",dw_packet_number);
    if(0x0001 == dw_packet_number){
        if(FALSE == net_firmware_update_traget_pairing_pacx(pst_firmware_packet,pst_net_system_info_list)){
            debug_updata_error("[updata]id no support\r\n");
            return cbFirmwareIDNotSupport;
        }
        if(NULL == st_device_update_info_current.pst_net_device_update_info_current){
            debug_updata_error("[updata]current null \r\n");
            return cbFirmwareIDNotSupport;
        }
        if(0 != memcmp(UPDATE_CURRENT_MD5,st_device_update_info_current.ba_firmware_id,cb_firmware_id_max_len)){
             //exchange config
            //debug_updata_test("[updata]flash_info malloc0  %p \r\n",flash_info);
             //debug_updata_test("[updata]flash_info free2 %p\r\n",flash_info);
             if(w_firmware_packet_len < 56){
                return cbFirmwareDataError;
             }
             memcpy(&UPDATE_DATA[26],&UPDATE_DATA[0],26);
             memcpy(&UPDATE_DATA[4],&UPDATE_DATA[26],26 - 4);
             memcpy(&UPDATE_DATA[0],(uint8_t*)&(st_device_update_info_current.dw_packets_number),4);
             w_firmware_packet_len = 26;
             memcpy(UPDATE_CURRENT_MD5,st_device_update_info_current.ba_firmware_id,cb_firmware_id_max_len);
             UPDATE_CURRENT_TOTAL       = st_device_update_info_current.dw_packets_number;
             UPDATE_CURRENT_FORMAT      = st_device_update_info_current.b_packet_format;
             UPDATE_CURRENT_CURRENT     = 1;//num from 0      info pack 1
             debug_updata_info("[updata]totle number %d\r\n",UPDATE_CURRENT_TOTAL);

        }else{
            debug_updata_info("[updata] continue \r\n");
            return cbNetCmdOK;
        }
    }
    if(NULL == st_device_update_info_current.pst_net_device_update_info_current){
        debug_updata_info("[updata]current null \r\n");
        return cbFirmwareIDNotSupport;
    }

    if((UPDATE_CURRENT_CURRENT )== dw_packet_number){//save collect
        if(false  == exflash_write_pack(dw_packet_number,UPDATE_DATA,w_firmware_packet_len)){
            debug_updata_info("[updata]flash errro %d len %d\r\n",UPDATE_CURRENT_CURRENT,w_firmware_packet_len);
            return cbFirmwareDataError;
        }
        UPDATE_CURRENT_CURRENT += 1;
        debug_updata_info("[updata]pack current %d/%d len %d\r\n",UPDATE_CURRENT_CURRENT,UPDATE_CURRENT_TOTAL,w_firmware_packet_len);
        return cbNetCmdOK;
    }else if((UPDATE_CURRENT_CURRENT )> dw_packet_number){//repeat do noting
        debug_updata_error("[updata]pack repeat %d,%d\r\n",dw_packet_number,UPDATE_CURRENT_CURRENT);
        return cbNetCmdOK;
    }else{
        debug_updata_error("[updata]pack advance %d,%d\r\n",dw_packet_number,UPDATE_CURRENT_CURRENT);
	    return cbUpdatePackageSumError;
	}

    //return cbNetCmdOK;
}
#else

uint8_t net_firmware_update_packet_operate_pacx(uint32_t dw_secquence,\
                                                uint16_t w_firmware_packet_len,\
                                                firmware_packet *pst_firmware_packet,\
                                                net_system_info_list *pst_net_system_info_list)
{
    uint32_t dw_packet_number = 0;
    uint8_t type = cbFirmwareDataError;
    if((NULL != pst_firmware_packet) && (NULL != pst_net_system_info_list) &&(w_firmware_packet_len < 4))
    {
        if(FALSE != bytes_to_int_big_to_little_endain(pst_firmware_packet->st_firmware_packet_pacx.ba_packet_number,
                                                                                             &dw_packet_number,
                                                                                             4))                                                                                         4))
        {
            //big //debug_updata_sector(pst_firmware_packet->st_firmware_packet_pacx.ba_packet_number,4);
            debug_updata_test("[updata] packet%d\r\n",dw_packet_number);
            if(0x0001 == dw_packet_number){
                if(TRUE == net_firmware_update_traget_pairing_pacx(pst_firmware_packet,pst_net_system_info_list)){
                    if(NULL != st_device_update_info_current.pst_net_device_update_info_current){
                        //if(0 != memcmp(UPDATE_CURRENT_MD5,st_device_update_info_current.ba_firmware_id,cb_firmware_id_max_len)){
                             //exchange config
                            //debug_updata_test("[updata]flash_info malloc0  %p \r\n",flash_info);
                             //debug_updata_test("[updata]flash_info free2 %p\r\n",flash_info);
                             if(w_firmware_packet_len < 56){
                                return cbFirmwareDataError;
                             }
                             memcpy(&UPDATE_DATA[26],&UPDATE_DATA[0],26);
                             memcpy(&UPDATE_DATA[4],&UPDATE_DATA[26],26 - 4);
                             memcpy(&UPDATE_DATA[0],(uint8_t*)&(st_device_update_info_current.dw_packets_number),4);
                             w_firmware_packet_len = 26;
                             memcpy(UPDATE_CURRENT_MD5,st_device_update_info_current.ba_firmware_id,cb_firmware_id_max_len);
                             UPDATE_CURRENT_TOTAL       = st_device_update_info_current.dw_packets_number;
                             UPDATE_CURRENT_FORMAT      = st_device_update_info_current.b_packet_format;
                             UPDATE_CURRENT_CURRENT     = 1;//num from 0      info pack 1
                             debug_updata_info("[updata]totle number %d\r\n",UPDATE_CURRENT_TOTAL);

                       // }else{
                            debug_updata_info("[updata] continue \r\n");
                            type = cbNetCmdOK;
                       // }
                    }
                }

            }else{
                if(NULL != st_device_update_info_current.pst_net_device_update_info_current){
                    if((UPDATE_CURRENT_CURRENT )== dw_packet_number){//save collect
                        if(ture  == exflash_write_pack(dw_packet_number,UPDATE_DATA,w_firmware_packet_len)){
                            UPDATE_CURRENT_CURRENT += 1;
                            debug_updata_info("[updata]pack current %d/%d len %d\r\n",UPDATE_CURRENT_CURRENT,UPDATE_CURRENT_TOTAL,w_firmware_packet_len);
                            type = cbNetCmdOK;
                        }else if((UPDATE_CURRENT_CURRENT )> dw_packet_number){//repeat do noting
                            debug_updata_error("[updata]pack repeat %d,%d\r\n",dw_packet_number,UPDATE_CURRENT_CURRENT);
                            type = cbNetCmdOK;
                        }else{
                            debug_updata_error("[updata]pack advance %d,%d\r\n",dw_packet_number,UPDATE_CURRENT_CURRENT);
                	        type = cbUpdatePackageSumError;
                	    }
                    }
                }
            }
        }
    }
    return   type;
}


#endif

#endif


uint8_t net_firmware_update_packet_operate(uint16_t w_firmware_packet_len,\
                                           firmware_packet *pst_firmware_packet,\
                                           net_system_info_list *pst_net_system_info_list)
{
    uint8_t b_cmd = cbUpdateTypeError;
    uint32_t dw_package_num_temp = 0;
    bytes_to_int_big_to_little_endain(pst_firmware_packet->st_firmware_packet_pacx_dsp.ba_packet_number,
                                      (uint32_t *)&dw_package_num_temp,
                                      4);

    if((dw_package_num_temp == 1) &&\
       (cb_firmware_format_none == st_device_update_info_current.b_update_type))
    {
        st_device_update_info_current.b_update_type =\
            pst_firmware_packet->st_firmware_packet_pacx_dsp.un_pacx_dsp_packet.st_pacx_dsp_packet_info.b_identification;
    }
	   
	#if( update_status_print )
		DebugPrintf(TRUE,"dw_packets_receive_total:%d ,dw_packets_receive_current:%d\n",
					st_device_update_info_current.pst_net_device_update_info_current->st_net_firmware_info.dw_packets_receive_total,
					st_device_update_info_current.pst_net_device_update_info_current->st_net_firmware_info.dw_packets_receive_current);
	#endif
    switch(st_device_update_info_current.b_update_type)
    {
        case cb_firmware_format_pac:
            {
                b_cmd = net_firmware_update_packet_operate_pac(w_firmware_packet_len,
                                                               pst_firmware_packet,
                                                               pst_net_system_info_list);
                #if( update_status_print )
                DebugPrintf(TRUE,"[Update]Pac receive,cmd:%d\r\n",b_cmd);
                #endif
            }
            break;
        case cb_firmware_format_pacx_mcu:
            {
                b_cmd = net_firmware_update_packet_operate_pacx(w_firmware_packet_len,
                                                                pst_firmware_packet,
                                                                pst_net_system_info_list);
                #if( update_status_print )
                DebugPrintf(TRUE,"[Update]Pacx receive,cmd:%d\r\n",b_cmd);
                #endif
            }
            break;
        case cb_firmware_format_pacx_dsp:
            {
                b_cmd = net_firmware_update_packet_operate_pacx_dsp(w_firmware_packet_len,
                                                                    pst_firmware_packet,
                                                                    pst_net_system_info_list);
                #if( update_status_print )
                DebugPrintf(TRUE,"[Update]Dsp receive,cmd:%d\r\n",b_cmd);
                #endif
            }
            break;
        default:
            st_device_update_info_current.b_update_type = 0;
            break;
    }
    return b_cmd;
}

