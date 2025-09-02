#include "network_local_config.h"
//#include "slave_ble_module_operate.h"
#include "network_protocal_cmd_macro.h"
#include "net_device_data_access.h"
#include <string.h>
#include "application_general_func.h"
#include "system_timestamp_generate.h"
#include "n32g45x.h"
#include "Eeprom.h"
#include "network_tbb_device_server_communicate.h"
#include "network_device_config_sycn.h"
#include "rs485_communication_transmission.h"
#include "rs485_communicate_protocal.h"
#include "net_device_id_kinergy_ii.h"
#include "network_local_device_secquence_operate.h"
#include "net_device_tree_build.h"


#define cb_username_max_len         20
#define cb_passwords_max_len        20
#define cb_set_data_len			    255

#define cb_lan_login_info_max_len   (cb_username_max_len + cb_passwords_max_len + 1)
typedef struct
{
    uint8_t b_user_name[cb_username_max_len];
    uint8_t b_connection;
    uint8_t b_passwords[cb_passwords_max_len];
}lan_login_info;

typedef struct
{
    uint8_t b_user_name[cb_username_max_len];
    uint8_t b_connection;
    uint8_t b_passwords[cb_passwords_max_len];
}super_lan_login_info;
typedef struct
{
    uint8_t addr_len;
    uint8_t addr[5];
    uint8_t data_len;
    uint8_t type[2];
    struct
    {
        uint8_t sn[16];
        uint8_t mode[2];
        uint8_t inv_addr1[2];
        uint8_t inv_addr2[2];
    } data;
}auxiliary_installation_t;
typedef struct
{
    uint8_t addr_len;
    uint8_t addr[5];
    uint8_t data_len;
    uint8_t type[2];
    struct
    {
        uint8_t inv_addr[2];
        uint8_t sn[16];
        uint8_t mppt_model_set[2];
        uint8_t mppt_num[2];
    } data;
}mppt_auxiliary_installation_t;
typedef struct
{
    uint8_t device_id_len;
    uint8_t device_id[5];
    uint8_t data_len;
    uint8_t data[cb_set_data_len];
}receive_net_data;

lan_login_info st_lan_login_info =
{
    .b_user_name = {0x00,},//,?
    .b_connection = '&',
    .b_passwords = {0x00,},//,?
};

super_lan_login_info st_super_lan_login_info =
{
    .b_user_name = "admin",//,?
    .b_connection = '&',
    .b_passwords = "123456",//,?
};

network_ap_connect_info st_network_ap_connect_info =
{
//    .st_ap_info.ba_ssid = "TBB-RD-TEST",
//    .st_ap_info.b_connection = '&',
//    .st_ap_info.ba_pass = "tbb@2020",
//    .st_ap_info.ba_ssid = {"TBB-GUEST"},
//    .st_ap_info.b_connection = '&',
//    .st_ap_info.ba_pass = {"tbbguest2018"},
    .st_ap_info.ba_ssid = {"WCM_00000000000000"},
    .st_ap_info.b_connection = '&',
    .st_ap_info.ba_pass = {"12345678"},
};

network_ap_set_info st_network_ap_set_info =
{
    .bssid.f_write_bssid = FALSE,
    .bssid.bssid_info.ba_data = {'\"','0','0',':','0','0',':','0','0',':','0','0',':','0','0',':','0','0','\"'},
    .b_pci_en = 0,
    .b_reconn_interval = 1,
    .b_listen_interval = 3,
    .b_scan_mode = 0,
    .b_jap_timeout = 10,
    .b_pmf = 0,
    .f_wps_enable = FALSE,
    .f_wps_first_connect = FALSE,
};

network_ap_info_management st_network_ap_info_management =
{
    .f_ssid_modify = FALSE,
    .f_ap_connect = FALSE,
    .b_error_code = cb_ap_connect_null,
//    .pst_network_ap_connect_info = &st_network_ap_connect_info,
};
/************************************************************************************************
* Input:
*
* Output:
*
* Logic discription: ap info set
*
* run time:
************************************************************************************************/
uint8_t ble_connect_flag = 0; // 蓝牙连接ap流程
uint8_t net_ap_info_set(uint16_t w_data_len_in,uint8_t *pb_data_in,uint8_t f_save_eeprom)
{
    if(NULL == pb_data_in)
    {
        return FALSE;
    }

    network_ap_connect_info *pst_network_ap_connect_info = (network_ap_connect_info *)pb_data_in;

    if(('&' != pst_network_ap_connect_info->st_ap_info.b_connection) ||\
       (w_data_len_in < (cb_ap_ssid_len + cb_ap_pass_len + 1)))
    {
        return FALSE;
    }

    if(0 != strlen((char const*)pst_network_ap_connect_info->st_ap_info.ba_ssid))
    {
        memcpy((char *)st_network_ap_connect_info.ba_data,
               (char *)pst_network_ap_connect_info->ba_data,
               sizeof(network_ap_connect_info));
        {
            uint16_t w_data_len = 129;
            net_system_data_access(cb_device_data_access_data_type_set,
                                   0x000B,
                                   &w_data_len,
                                   (uint8_t *)&st_network_ap_connect_info.ba_data[0],
                                   cb_device_data_access_operate_type_write);
        }

        st_network_ap_set_info.f_wps_enable = FALSE;
        st_network_ap_set_info.f_wps_first_connect = FALSE;
        st_network_ap_info_management.f_ssid_modify = TRUE;
        network_ap_connect_err_code_set(cb_ap_connect_null);
        if(TRUE == f_save_eeprom)
        {
            fWriteEEPage(5) = TRUE;
            fWriteEEPage(6) = TRUE;
            fWriteEEPage(7) = TRUE;
            fWriteEEPage(8) = TRUE;
            fWriteEEPage(9) = TRUE;
            fWriteEEPage(10) = TRUE;
            fWriteEEPage(11) = TRUE;
            fWriteEEPage(12) = TRUE;
            fWriteEEPage(13) = TRUE;
            fWriteEEPage(14) = TRUE;
            fWriteEEPage(16) = TRUE;
        }
        ble_connect_flag =1;
        return TRUE;
    }
    return FALSE;
}
void network_ap_connect_err_code_set(uint8_t b_err_code)
{
    switch(b_err_code)
    {
        case cb_ap_connect_normal:
        case cb_ap_connect_timeout:
        case cb_ap_connect_pass_err:
        case cb_ap_connect_ssid_err:
        case cb_ap_connect_fail:
        case cb_ap_connect_null:
            st_network_ap_info_management.b_error_code = b_err_code;
            break;
        default:
            st_network_ap_info_management.b_error_code = cb_ap_connect_fail;
            break;
    }
    {
        uint16_t w_data_len = 1;
        if(cb_ap_connect_normal == st_network_ap_info_management.b_error_code)
        {
            st_network_ap_info_management.f_ap_connect = TRUE;
        }
        else
        {
            st_network_ap_info_management.f_ap_connect = FALSE;
        }
        net_system_data_access(cb_device_data_access_data_type_status,
                               0x0109,
                               &w_data_len,
                               (uint8_t *)&st_network_ap_info_management.b_error_code,
                               cb_device_data_access_operate_type_write);
        net_system_data_access(cb_device_data_access_data_type_status,
                               0x0104,
                               &w_data_len,
                               (uint8_t *)&st_network_ap_info_management.f_ap_connect,
                               cb_device_data_access_operate_type_write);
    }
}
uint8_t check_ap_connect_enable()
{
    uint8_t b_result = 0;

    if(st_network_ap_info_management.f_ssid_modify)
    {
        st_network_ap_info_management.f_ssid_modify = FALSE;
        st_network_ap_info_management.b_error_code = cb_ap_connect_normal;
        b_result = 1;
    }
    else if((cb_ap_connect_pass_err != st_network_ap_info_management.b_error_code) &&\
            (cb_ap_connect_normal != st_network_ap_info_management.b_error_code))
    {
        b_result = 2;
    }
    else if(TRUE == st_network_ap_info_management.f_reconnect_enable)
    {
        b_result = 3;
    }
    return b_result;
}
uint8_t check_sim_connect_enable()
{
    uint8_t b_result = 0;

    if(st_network_ap_info_management.f_ssid_modify)
    {
        st_network_ap_info_management.f_ssid_modify = FALSE;
        st_network_ap_info_management.b_error_code = cb_sim_connect_normal;
        b_result = 1;
    }
    else if((cb_ap_connect_pass_err != st_network_ap_info_management.b_error_code) &&\
            (cb_ap_connect_normal != st_network_ap_info_management.b_error_code))
    {
        b_result = 2;
    }
    else if(TRUE == st_network_ap_info_management.f_reconnect_enable)
    {
        b_result = 3;
    }
    return b_result;
}

/************************************************************************************************
* Input:
*
* Output:
*
* Logic discription: lan device login
*
* run time:
************************************************************************************************/
static uint8_t net_lan_login(uint16_t w_data_len_in,uint8_t *pb_data_in)
{
	if(NULL == pb_data_in)
	{
		return FALSE;
	}

    //1byte '&'
    if(((cb_username_max_len + cb_passwords_max_len + 1) == w_data_len_in) &&\
       ('&' == *(uint8_t *)(pb_data_in + cb_username_max_len)))
    {
        //passwords is all '\0', means no passwords
        if(0 == strlen((char const*)(st_lan_login_info.b_passwords)))
        {
            return TRUE;
        }
        else
        {
            if((memcmp((const uint8_t *)(pb_data_in + cb_username_max_len + 1),st_lan_login_info.b_passwords,cb_passwords_max_len) &&\
                memcmp((const uint8_t *)(pb_data_in),st_lan_login_info.b_user_name,cb_username_max_len) ||\
               (memcmp((uint8_t *)(pb_data_in + cb_username_max_len + 1),st_super_lan_login_info.b_passwords,cb_passwords_max_len) &&\
                memcmp((uint8_t *)(pb_data_in),st_super_lan_login_info.b_user_name,cb_username_max_len))))
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}
/************************************************************************************************
* Input:
*
* Output:
*
* Logic discription: net system set
*
* run time:
************************************************************************************************/
uint8_t net_system_set(uint16_t w_set_addr,
                       network_data_info *pst_network_data_info,
                       channel_infomation *pst_channel_infomation,
                       uint16_t w_net_body_len)
{
    switch(w_set_addr)
    {
        case 0x0000:  // 辅助安装
        {
            auxiliary_installation_t *auxiliary_installation = (auxiliary_installation_t *)pst_network_data_info->pb_network_data_body_part;
            uint16_t w_install_type = (auxiliary_installation->type[0] << 4) | auxiliary_installation->type[1];
            if(0x000F == w_install_type)
            {
                network_device_config_sycn_trig(dwa_device_id_kinergy_ii[cw_device_id_index_system],
                                                    cb_wired_network_inv_auxiliary_installation_cmd,
                                                    0x0000,
                                                    sizeof(auxiliary_installation->data),
                                                    (uint8_t *)&auxiliary_installation->data,
                                                    1,
                                                    TRUE,
                                                    1);//sync_type_single
            }
            else if(0x004F == w_install_type)
            {
//                uint16_t w_inv_addr = 0;
                mppt_auxiliary_installation_t *mppt_auxiliary_installation = (mppt_auxiliary_installation_t *)pst_network_data_info->pb_network_data_body_part;
//                w_inv_addr = (mppt_auxiliary_installation->inv_addr[0] << 4) | mppt_auxiliary_installation->inv_addr[1];
                network_device_config_sycn_trig(dwa_device_id_kinergy_ii[cw_device_id_index_system],
                                                    cb_wired_network_mppt_auxiliary_installation_cmd,
                                                    0x4F00,
                                                    sizeof(mppt_auxiliary_installation->data),
                                                    (uint8_t *)&mppt_auxiliary_installation->data,
                                                    1,
                                                    TRUE,
                                                    1);//sync_type_single
            }
            return TRUE;
		}break;

        case 0x0002:
        {
            uint32_t dw_timestamp = 0;

            bytes_to_int_big_to_little_endain(&pst_network_data_info->pb_network_data_body_part[7],\
                                              &dw_timestamp,
                                              4);
            if(0 != dw_timestamp)
            {
                refresh_system_timestamp(dw_timestamp);
                set_device_timestamp(dw_timestamp);
            }
            else
            {
                enabke_get_network_timestamp(TRUE);
            }

            return TRUE;
        }
        case 0x000B:
        {
            return net_ap_info_set((uint16_t)pst_network_data_info->pb_network_data_body_part[6],
                                   &pst_network_data_info->pb_network_data_body_part[7],
                                   TRUE);
        }
        case 0x000C:
        {
        }
        case 0x000D:
        {
        }
        case 0x000F:
        {
            return FALSE;
        }
		case 0x0010:
        {
//			//这个设置项可以远程写系统的sn号，将这行打开set_local_device_secquence(set_sn->data,30);可以远程将sn号写入eeprom        
//		    uint16_t w_data_len = 30;
//			receive_net_data *set_sn = (receive_net_data *)pst_network_data_info->pb_network_data_body_part;
//			memset(ba_local_device_secquence,0,30);
//			memcpy(ba_local_device_secquence,set_sn->data,w_data_len);
////	        set_local_device_secquence(set_sn->data,30);
//        	net_system_data_access(cb_device_data_access_data_type_set,
//                               0x0010,
//                               &w_data_len,
//                               (uint8_t *)set_sn->data,
//                               cb_device_data_access_operate_type_write);
			return FALSE;
        }
        case 0xAABB:
        {
            return TRUE;
        }
        case 0x00FF:
        {
            sEepromReset();
            return TRUE;
        }
        default:
        {
            break;
        }
    }

    return FALSE;
}
/************************************************************************************************
* Input:
*
* Output:
*
* Logic discription: net system ctrl
*
* run time:
************************************************************************************************/
uint8_t net_system_ctrl(uint32_t dw_device_id,
                        uint16_t w_data_address,
                        uint32_t dw_secquence,
                        uint16_t w_data_len_in,
                        uint8_t *pb_data_in,
                        channel_infomation *pst_channel_infomation,
                        uint8_t b_channel_index,
                        network_data_send_queue px_network_data_send_queue)
{
    switch(w_data_address)
    {
		case 0x1000:
		{
			if(TRUE == net_lan_login(w_data_len_in,pb_data_in))
			{
                net_system_data_access(cb_device_data_access_data_type_set,
                                       cw_sys_set_data_addr_login_aconut,
                                       &w_data_len_in,
                                       pb_data_in,
                                       cb_device_data_access_operate_type_write);

				return net_ack_assembly(0x00,
								        0,
								        cw_NetControlAck,
								        cbNetCmdOK,
								        dw_secquence,
                                        b_channel_index,
                                        px_network_data_send_queue);
			}
			break;
		}
        case 0x0101:
        {
            //f_response = func(pst_network_data_info,&pst_channel_infomation,w_net_body_len);
            //pst_network_data_info->pb_net_body
            //pst_network_data_info->w_cmd_type
            break;
        }
        case 0xFFF1:
        {
            NVIC_SystemReset();
        }
        default:
        {
            break;
        }
    }

    return FALSE;
}

/************************************************************************************************
* Input:
*
* Output:
*
* Logic discription: net kinergy set
*
* run time:
************************************************************************************************/
uint8_t gb_bat_full_charge_gap_flag = 0;
uint8_t net_kinergy_set(const net_data_info *pst_data_info,
                        uint8_t *pb_set_data,
                        uint8_t b_data_len)
{
    if(NULL == pst_data_info)
    {
        return FALSE;
    }
    if(NULL != pst_data_info->pst_net_data_set_attribute)
    {
        uint32_t dw_data_temp = 0;

        bytes_to_int_big_to_little_endain(pb_set_data,&dw_data_temp,b_data_len);
 		if(((0 == (0x80000000 & dw_data_temp)) & (dw_data_temp > pst_data_info->pst_net_data_set_attribute->dw_max_data)) ||\
			((1 == (0x80000000 & dw_data_temp)) & (dw_data_temp < pst_data_info->pst_net_data_set_attribute->dw_min_data)))
		{
			return FALSE;
		}	
    }
	uint8_t b_test_mode = 0;
	uint16_t w_data_len = 1;
	net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
						   cb_net_data_att_set,
						   0x003F,
						   &w_data_len,
						   (uint8_t *)&b_test_mode,
						   cb_device_data_access_operate_type_read);

	if(pst_data_info->w_data_address_net == 0x0042 )
	{

		if(0x55 != b_test_mode)
		{
	    	return FALSE;
		}
	}
    for(uint8_t b_cnt_i = 0;b_cnt_i < pst_data_info->pst_data_attribute->b_len;b_cnt_i++)
    {
        pst_data_info->pb_data[b_cnt_i] =\
            pb_set_data[pst_data_info->pst_data_attribute->b_len - b_cnt_i - 1];
    }

    if(pst_data_info->w_data_address_net <= 0x0004)
    {
        fWriteEEPage(17) = TRUE;
    }
    else if((pst_data_info->w_data_address_net >= 0x0005) &&\
            (pst_data_info->w_data_address_net <= 0x000B))
    {
        fWriteEEPage(18) = TRUE;
    }
    else if((pst_data_info->w_data_address_net >= 0x000C) &&\
            (pst_data_info->w_data_address_net < 0x0011))
    {
        fWriteEEPage(19) = TRUE;
    }
    else if(pst_data_info->w_data_address_net == 0x0011)
    {
        fWriteEEPage(19) = TRUE;
        fWriteEEPage(20) = TRUE;
    }
    else if((pst_data_info->w_data_address_net >= 0x0012) &&\
            (pst_data_info->w_data_address_net <= 0x0014))
    {
        fWriteEEPage(20) = TRUE;
    }
    else if((pst_data_info->w_data_address_net >= 0x0015) &&\
            (pst_data_info->w_data_address_net <= 0x0018))
    {
        fWriteEEPage(21) = TRUE;
    }
    else if((pst_data_info->w_data_address_net >= 0x0019) &&\
            (pst_data_info->w_data_address_net <= 0x001F))
    {
        fWriteEEPage(22) = TRUE;
    }
    else if((pst_data_info->w_data_address_net >= 0x0020) &&\
            (pst_data_info->w_data_address_net <= 0x0026))
    {
        fWriteEEPage(23) = TRUE;
    }
    else if((pst_data_info->w_data_address_net >= 0x0027) &&\
            (pst_data_info->w_data_address_net <= 0x002A))
    {
        fWriteEEPage(24) = TRUE;
    }
    else if((pst_data_info->w_data_address_net >= 0x002B) &&\
            (pst_data_info->w_data_address_net <= 0x002D))
    {
        fWriteEEPage(25) = TRUE;
    }
    else if(pst_data_info->w_data_address_net == 0x002E)
    {
        fWriteEEPage(25) = TRUE;
        fWriteEEPage(26) = TRUE;
    }
    else if((pst_data_info->w_data_address_net >= 0x002F) &&\
            (pst_data_info->w_data_address_net <= 0x0036))
    {
        fWriteEEPage(26) = TRUE;
    }
	else if(pst_data_info->w_data_address_net == 0x003A 
		|| pst_data_info->w_data_address_net == 0x003B)
	{
		gb_bat_full_charge_gap_flag = 1;
		fWriteEEPage(26) = TRUE;
	}
	else if(pst_data_info->w_data_address_net == 0x0042 )
	{
		if(0x55 == b_test_mode)
		{
	    	gw_agent_facturer = pb_set_data[0] << 8 | pb_set_data[1];
	        fWriteEEPage(27) = TRUE;	
		}
	}

    return TRUE;
}
