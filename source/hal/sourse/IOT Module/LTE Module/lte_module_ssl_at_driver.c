#include "lte_module_ssl_at_driver.h"
#include "application_general_macro.h"
#include <stdio.h>
#include <string.h>


typedef enum
{
    ssl_context_idle,
    ssl_context_occupy,
}lte_module_driver_ssl_context_state;

typedef struct
{
    uint8_t b_connect_id;
    lte_module_driver_ssl_context_state enum_ssl_context_state;
}lte_module_driver_ssl_context_id_map;

lte_module_driver_ssl_context_id_map st_lte_module_driver_ssl_context_id_map[ssl_context_id_number];

static uint16_t w_cert_length = 0;

uint8_t lte_module_driver_ssl_context_allocate(uint8_t b_connect_id,uint8_t *pb_ssl_context_id)
{
    if(NULL == pb_ssl_context_id)
    {
        return FALSE;
    }

    //mutex take
    for(uint8_t b_cnt_i = 0;b_cnt_i < ssl_context_id_number;b_cnt_i++)
    {
        if(ssl_context_idle == st_lte_module_driver_ssl_context_id_map[b_cnt_i].enum_ssl_context_state)
        {
            st_lte_module_driver_ssl_context_id_map[b_cnt_i].b_connect_id = b_connect_id;
            st_lte_module_driver_ssl_context_id_map[b_cnt_i].enum_ssl_context_state = ssl_context_occupy;
            *pb_ssl_context_id = b_cnt_i;

            //mutex give
            return TRUE;
        }
    }
    //mutex give
    return FALSE;
}
uint8_t lte_module_driver_ssl_context_deallocate(uint8_t b_ssl_context_id)
{
    //mutex take
    if(b_ssl_context_id < ssl_context_id_number)
    {
        st_lte_module_driver_ssl_context_id_map[b_ssl_context_id].enum_ssl_context_state = ssl_context_idle;

        //mutex give
        return TRUE;
    }

    //mutex give
    return FALSE;
}

/*SSL CA CERT*/
//AT+QFUPL="RAM:cacert.pem"
uint8_t lte_module_driver_send_at_ssl_ca_cert_pem_delete(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
    if(NULL == pst_lte_module_data_info)
    {
        return FALSE;
    }

	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+QFDEL=\"RAM:cacert.pem\"\r\n"))
	{
		return FALSE;
	}

	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return TRUE;
}
uint8_t lte_module_driver_receive_at_ssl_ca_cert_pem_delete(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if(NULL == pst_lte_module_data_info)
	{
		return 0;
	}

	if((pst_lte_module_data_info->w_data_len <= 11) &&\
        (strstr((char*)pst_lte_module_data_info->pb_data,"+QFDEL: cacert.pem") != 0))
	{
		return 1;
	}

	return 0;
}
uint8_t lte_module_driver_send_at_ssl_ca_cert_pem_step1(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
    if((NULL == pv_user_data) || (NULL == pst_lte_module_data_info))
    {
        return FALSE;
    }

    lte_module_driver_config_info_ssl *pst_ssl = (lte_module_driver_config_info_ssl *)pv_user_data;

    if(NULL == pst_ssl->pfunc_ssl_cert_read)
    {
        return FALSE;
    }

    uint16_t w_data_len = 0;
    uint16_t w_data = 0;

	if(FALSE == pst_ssl->pfunc_ssl_cert_read(ssl_cert_read_length,
                                             pst_ssl->dw_memory_read_address_ca_cert,
                                             &w_data_len,
                                             (uint8_t *)&w_data))
    {
        return FALSE;
    }


	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+QFUPL=\"RAM:cacert.pem\",%d,100\r\n",w_data))
	{
		return FALSE;
	}

    w_cert_length = w_data;
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return TRUE;
}
uint8_t lte_module_driver_receive_at_ssl_ca_cert_pem_step1(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if(NULL == pst_lte_module_data_info)
	{
		return 0;
	}

	if((pst_lte_module_data_info->w_data_len <= 11) &&\
	   (strstr((char*)pst_lte_module_data_info->pb_data,"CONNECT") != 0))
	{
		return 1;
	}
    else if(strstr((char*)pst_lte_module_data_info->pb_data,"+CME ERROR: 407") != 0)
    {
        return 0;
    }

	return 0;
}
uint8_t lte_module_driver_send_at_ssl_ca_cert_pem_step2(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if((NULL == pv_user_data) || (NULL == pst_lte_module_data_info))
	{
		return FALSE;
	}

    lte_module_driver_config_info_ssl *pst_ssl = (lte_module_driver_config_info_ssl *)pv_user_data;

    if(NULL == pst_ssl->pfunc_ssl_cert_read)
    {
        return FALSE;
    }

	return pst_ssl->pfunc_ssl_cert_read(ssl_cert_read_content,
                                        pst_ssl->dw_memory_read_address_ca_cert,
                                        &pst_lte_module_data_info->w_data_len,
                                        pst_lte_module_data_info->pb_data);
}
uint8_t lte_module_driver_receive_at_ssl_ca_cert_pem_step2(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if(NULL == pst_lte_module_data_info)
	{
		return FALSE;
	}

	uint32_t dw_data_temp_0 = 0;
	uint32_t dw_data_temp_1 = 0;

	if(sscanf(strstr((char*)pst_lte_module_data_info->pb_data,"+QFUPL:"),"+QFUPL: %d,%x\r\n",&dw_data_temp_0,&dw_data_temp_1) != 0)
	{
		if(dw_data_temp_0 != w_cert_length)
		{
			return 0;
		}
	}

    if(strstr((char*)pst_lte_module_data_info->pb_data,"OK") != NULL)
    {
        return 1;
    }

	return 2;
}
//AT+QSSLCFG="cacert",2,"RAM:cacert.pem"\r\n
uint8_t lte_module_driver_send_at_ssl_ca_cert_pem_step3(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if((NULL == pv_user_data) || (NULL == pst_lte_module_data_info))
	{
		return FALSE;
	}

    lte_module_driver_config_info_ssl *pst_ssl = (lte_module_driver_config_info_ssl *)pv_user_data;

	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+QSSLCFG=\"cacert\",%d,\"RAM:cacert.pem\"\r\n",pst_ssl->b_ssl_context_id))
	{
		return FALSE;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return TRUE;
}
uint8_t lte_module_driver_receive_at_ssl_ca_cert_pem_step3(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if(NULL == pst_lte_module_data_info)
	{
		return FALSE;
	}

	if((pst_lte_module_data_info->w_data_len <= 8) &&\
	   (strstr((char*)pst_lte_module_data_info->pb_data,"OK") != NULL))
	{
		return TRUE;
	}

	return FALSE;
}
uint8_t lte_module_ssl_config_ca_cert_pem(lte_module_driver_config_info_ssl *pst_ssl,
                                          lte_module_driver_process_contral *pst_process_contral)
{
	#define cb_lte_module_ssl_config_ca_cert_pem_step1								0
	#define cb_lte_module_ssl_config_ca_cert_pem_step2								1
	#define cb_lte_module_ssl_config_ca_cert_pem_step3								2
	#define cb_lte_module_ssl_config_ca_cert_pem_none								3
	uint8_t b_step = cb_lte_module_ssl_config_ca_cert_pem_step1;

    uint8_t b_result = FALSE;

    if(NULL == pst_process_contral)
    {
        return FALSE;
    }

    if(TRUE == pst_process_contral->operate_contral.b_process_reset)
    {
        b_step = cb_lte_module_ssl_config_ca_cert_pem_step1;
    }

    do
    {
        switch(b_step)
        {
            case cb_lte_module_ssl_config_ca_cert_pem_step1:
            {
                uint8_t b_result_temp = 0;

                b_result_temp = lte_module_driver_at_cmd_transmit(3000,pst_ssl,0,
                                                                  lte_module_driver_send_at_ssl_ca_cert_pem_step1,
                                                                  lte_module_driver_receive_at_ssl_ca_cert_pem_step1);
                if(1 == b_result_temp)
                {
                    b_step = cb_lte_module_ssl_config_ca_cert_pem_step2;
                }
                else if(0 == b_result_temp)
                {
                    b_step = cb_lte_module_ssl_config_ca_cert_pem_none;
                    b_result = FALSE;
                }
            }break;
            case cb_lte_module_ssl_config_ca_cert_pem_step2:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(3000,pst_ssl,0,
                                                             lte_module_driver_send_at_ssl_ca_cert_pem_step2,
                                                             lte_module_driver_receive_at_ssl_ca_cert_pem_step2))
                {
                    b_step = cb_lte_module_ssl_config_ca_cert_pem_step3;
                }
                else
                {
                    b_step = cb_lte_module_ssl_config_ca_cert_pem_none;
                    b_result = FALSE;
                }
            }break;
            case cb_lte_module_ssl_config_ca_cert_pem_step3:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(3000,pst_ssl,0,
                                                             lte_module_driver_send_at_ssl_ca_cert_pem_step3,
                                                             lte_module_driver_receive_at_ssl_ca_cert_pem_step3))
                {
                    b_step = cb_lte_module_ssl_config_ca_cert_pem_none;
                    b_result = TRUE;
                }
                else
                {
                    b_step = cb_lte_module_ssl_config_ca_cert_pem_none;
                    b_result = FALSE;
                }
            }break;
            case cb_lte_module_ssl_config_ca_cert_pem_none:
            {
            }break;
            default:
            {
            }break;
        }
    }while(b_step != cb_lte_module_ssl_config_ca_cert_pem_none);

	return b_result;
}
/*SSL CLIENT CERT*/
//AT+QFUPL="RAM:client_cert.pem"
uint8_t lte_module_driver_send_at_ssl_client_cert_pem_delete(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
    if(NULL == pst_lte_module_data_info)
    {
        return FALSE;
    }

	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+QFDEL=\"RAM:client.pem\"\r\n"))
	{
		return FALSE;
	}

	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return TRUE;
}
uint8_t lte_module_driver_receive_at_ssl_client_cert_pem_delete(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if(NULL == pst_lte_module_data_info)
	{
		return 0;
	}

	if((pst_lte_module_data_info->w_data_len <= 11) &&\
        (strstr((char*)pst_lte_module_data_info->pb_data,"+QFDEL: client.pem") != 0))
	{
		return 1;
	}

	return 0;
}
uint8_t lte_module_driver_send_at_ssl_client_cert_pem_step1(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
    if((NULL == pv_user_data) || (NULL == pst_lte_module_data_info))
    {
        return FALSE;
    }

    lte_module_driver_config_info_ssl *pst_ssl = (lte_module_driver_config_info_ssl *)pv_user_data;

    if(NULL == pst_ssl->pfunc_ssl_cert_read)
    {
        return FALSE;
    }

    uint16_t w_data_len = 0;
    uint16_t w_data = 0;

    if(FALSE == pst_ssl->pfunc_ssl_cert_read(ssl_cert_read_length,
                                             pst_ssl->dw_memory_read_address_client_cert,
                                             &w_data_len,
                                             (uint8_t *)&w_data))
    {
        return FALSE;
    }

	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+QFUPL=\"RAM:client.pem\",%d,100\r\n",w_data))
	{
		return FALSE;
	}

    w_cert_length = w_data;
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return TRUE;
}
uint8_t lte_module_driver_receive_at_ssl_client_cert_pem_step1(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if(NULL == pst_lte_module_data_info)
	{
		return 0;
	}

	if((pst_lte_module_data_info->w_data_len <= 11) &&\
	   (strstr((char*)pst_lte_module_data_info->pb_data,"CONNECT") != 0))
	{
		return 1;
	}
    else if(strstr((char*)pst_lte_module_data_info->pb_data,"+CME ERROR: 407") != 0)
    {
        return 4;
    }

	return 0;
}
uint8_t lte_module_driver_send_at_ssl_client_cert_pem_step2(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if((NULL == pv_user_data) || (NULL == pst_lte_module_data_info))
	{
		return FALSE;
	}

    lte_module_driver_config_info_ssl *pst_ssl = (lte_module_driver_config_info_ssl *)pv_user_data;

    if(NULL == pst_ssl->pfunc_ssl_cert_read)
    {
        return FALSE;
    }

	return pst_ssl->pfunc_ssl_cert_read(ssl_cert_read_content,
                                        pst_ssl->dw_memory_read_address_client_cert,
                                        &pst_lte_module_data_info->w_data_len,
                                        pst_lte_module_data_info->pb_data);
}
uint8_t lte_module_driver_receive_at_ssl_client_cert_pem_step2(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if(NULL == pst_lte_module_data_info)
	{
		return FALSE;
	}

	uint32_t dw_data_temp_0 = 0;
	uint32_t dw_data_temp_1 = 0;

	if(sscanf(strstr((char*)pst_lte_module_data_info->pb_data,"+QFUPL:"),"+QFUPL: %d,%x\r\n",&dw_data_temp_0,&dw_data_temp_1) != 0)
	{
		if(dw_data_temp_0 != w_cert_length)
		{
			return 0;
		}
	}

    if(strstr((char*)pst_lte_module_data_info->pb_data,"OK") != NULL)
    {
        return 1;
    }

	return 2;
}
//AT+QSSLCFG="client_cert",2,"RAM:client_cert.pem"\r\n
uint8_t lte_module_driver_send_at_ssl_client_cert_pem_step3(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
    if((NULL == pv_user_data) || (NULL == pst_lte_module_data_info))
    {
        return FALSE;
    }

    lte_module_driver_config_info_ssl *pst_ssl = (lte_module_driver_config_info_ssl *)pv_user_data;

	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+QSSLCFG=\"clientcert\",%d,\"RAM:client.pem\"\r\n",pst_ssl->b_ssl_context_id))
	{
		return FALSE;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return TRUE;
}
uint8_t lte_module_driver_receive_at_ssl_client_cert_pem_step3(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if(NULL == pst_lte_module_data_info)
	{
		return FALSE;
	}

	if((pst_lte_module_data_info->w_data_len <= 8) &&\
	   (strstr((char*)pst_lte_module_data_info->pb_data,"OK") != NULL))
	{
		return TRUE;
	}

	return FALSE;
}
uint8_t lte_module_ssl_config_client_cert_pem(lte_module_driver_config_info_ssl *pst_ssl,
                                              lte_module_driver_process_contral *pst_process_contral)
{
	#define cb_lte_module_ssl_config_client_cert_pem_step1								0
	#define cb_lte_module_ssl_config_client_cert_pem_step2								1
	#define cb_lte_module_ssl_config_client_cert_pem_step3								2
	#define cb_lte_module_ssl_config_client_cert_pem_none								3
	uint8_t b_step = cb_lte_module_ssl_config_client_cert_pem_step1;

    uint8_t b_result = FALSE;

    if(NULL == pst_process_contral)
    {
        return FALSE;
    }

    if(TRUE == pst_process_contral->operate_contral.b_process_reset)
    {
        b_step = cb_lte_module_ssl_config_client_cert_pem_step1;
    }

    do
    {
        switch(b_step)
        {
            case cb_lte_module_ssl_config_client_cert_pem_step1:
            {
                uint8_t b_result_temp = 0;

                b_result_temp = lte_module_driver_at_cmd_transmit(3000,pst_ssl,0,
                                                                  lte_module_driver_send_at_ssl_client_cert_pem_step1,
                                                                  lte_module_driver_receive_at_ssl_client_cert_pem_step1);
                if(1 == b_result_temp)
                {
                    b_step = cb_lte_module_ssl_config_client_cert_pem_step2;
                }
                else if(0 == b_result_temp)
                {
                    b_result = FALSE;
                    b_step = cb_lte_module_ssl_config_client_cert_pem_none;
                }
            }break;
            case cb_lte_module_ssl_config_client_cert_pem_step2:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(3000,pst_ssl,0,
                                                             lte_module_driver_send_at_ssl_client_cert_pem_step2,
                                                             lte_module_driver_receive_at_ssl_client_cert_pem_step2))
                {
                    b_step = cb_lte_module_ssl_config_client_cert_pem_step3;
                }
                else
                {
                    b_result = FALSE;
                    b_step = cb_lte_module_ssl_config_client_cert_pem_none;
                }
            }break;
            case cb_lte_module_ssl_config_client_cert_pem_step3:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(3000,pst_ssl,0,
                                                             lte_module_driver_send_at_ssl_client_cert_pem_step3,
                                                             lte_module_driver_receive_at_ssl_client_cert_pem_step3))
                {
                    b_result = TRUE;
                    b_step = cb_lte_module_ssl_config_client_cert_pem_none;
                }
                else
                {
                    b_result = FALSE;
                    b_step = cb_lte_module_ssl_config_client_cert_pem_none;
                }
            }break;
            case cb_lte_module_ssl_config_ca_cert_pem_none:
            {
            }break;
            default:
            {
            }break;
        }
    }while(b_step != cb_lte_module_ssl_config_client_cert_pem_none);

	return b_result;
}
/*SSL CLIENT CERT*/
//AT+QFUPL="RAM:user_key.pem"
uint8_t lte_module_driver_send_at_ssl_client_key_pem_delete(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
    if(NULL == pst_lte_module_data_info)
    {
        return FALSE;
    }

	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+QFDEL=\"RAM:user_key.pem\"\r\n"))
	{
		return FALSE;
	}

	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return TRUE;
}
uint8_t lte_module_driver_receive_at_ssl_client_key_pem_delete(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if(NULL == pst_lte_module_data_info)
	{
		return 0;
	}

	if((pst_lte_module_data_info->w_data_len <= 11) &&\
        (strstr((char*)pst_lte_module_data_info->pb_data,"+QFDEL: user_key.pem") != 0))
	{
		return 1;
	}

	return 0;
}
uint8_t lte_module_driver_send_at_ssl_client_key_pem_step1(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
    if((NULL == pv_user_data) || (NULL == pst_lte_module_data_info))
    {
        return FALSE;
    }

    lte_module_driver_config_info_ssl *pst_ssl = (lte_module_driver_config_info_ssl *)pv_user_data;

    if(NULL == pst_ssl->pfunc_ssl_cert_read)
    {
        return FALSE;
    }

    uint16_t w_data_len = 0;
    uint16_t w_data = 0;

    if(FALSE == pst_ssl->pfunc_ssl_cert_read(ssl_cert_read_length,
                                             pst_ssl->dw_memory_read_address_client_key,
                                             &w_data_len,
                                             (uint8_t *)&w_data))
    {
        return FALSE;
    }

	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+QFUPL=\"RAM:user_key.pem\",%d,100\r\n",w_data))
	{
		return FALSE;
	}

    w_cert_length = w_data;
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return TRUE;
}
uint8_t lte_module_driver_receive_at_ssl_client_key_pem_step1(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if(NULL == pst_lte_module_data_info)
	{
		return 0;
	}

	if((pst_lte_module_data_info->w_data_len <= 11) &&\
	   (strstr((char*)pst_lte_module_data_info->pb_data,"CONNECT") != 0))
	{
		return 1;
	}
    else if(strstr((char*)pst_lte_module_data_info->pb_data,"+CME ERROR: 407") != 0)
    {
        return 4;
    }

	return 0;
}
uint8_t lte_module_driver_send_at_ssl_client_key_pem_step2(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if((NULL == pv_user_data) || (NULL == pst_lte_module_data_info))
	{
		return FALSE;
	}

    lte_module_driver_config_info_ssl *pst_ssl = (lte_module_driver_config_info_ssl *)pv_user_data;

    if(NULL == pst_ssl->pfunc_ssl_cert_read)
    {
        return FALSE;
    }

	return pst_ssl->pfunc_ssl_cert_read(ssl_cert_read_content,
                                        pst_ssl->dw_memory_read_address_client_key,
                                        &pst_lte_module_data_info->w_data_len,
                                        pst_lte_module_data_info->pb_data);
}
uint8_t lte_module_driver_receive_at_ssl_client_key_pem_step2(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if(NULL == pst_lte_module_data_info)
	{
		return FALSE;
	}

	uint32_t dw_data_temp_0 = 0;
	uint32_t dw_data_temp_1 = 0;

	if(sscanf(strstr((char*)pst_lte_module_data_info->pb_data,"+QFUPL:"),"+QFUPL: %d,%x\r\n",&dw_data_temp_0,&dw_data_temp_1) != 0)
	{
		if(dw_data_temp_0 != w_cert_length)
		{
			return 0;
		}
	}

    if(strstr((char*)pst_lte_module_data_info->pb_data,"OK") != NULL)
    {
        return 1;
    }

	return 2;
}
//AT+QSSLCFG="clientkey",2,"RAM:user_key.pem"\r\n
uint8_t lte_module_driver_send_at_ssl_client_key_pem_step3(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if((NULL == pv_user_data) || (NULL == pst_lte_module_data_info))
	{
		return FALSE;
	}

    lte_module_driver_config_info_ssl *pst_ssl = (lte_module_driver_config_info_ssl *)pv_user_data;

	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+QSSLCFG=\"clientkey\",%d,\"RAM:user_key.pem\"\r\n",pst_ssl->b_ssl_context_id))
	{
		return FALSE;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return TRUE;
}
uint8_t lte_module_driver_receive_at_ssl_client_key_pem_step3(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if(NULL == pst_lte_module_data_info)
	{
		return FALSE;
	}

	if((pst_lte_module_data_info->w_data_len <= 8) &&\
	   (strstr((char*)pst_lte_module_data_info->pb_data,"OK") != NULL))
	{
		return TRUE;
	}

	return FALSE;
}
uint8_t lte_module_ssl_config_client_key_pem(lte_module_driver_config_info_ssl *pst_ssl,
                                             lte_module_driver_process_contral *pst_process_contral)
{
	#define cb_lte_module_ssl_config_client_key_pem_step1								0
	#define cb_lte_module_ssl_config_client_key_pem_step2								1
	#define cb_lte_module_ssl_config_client_key_pem_step3								2
	#define cb_lte_module_ssl_config_client_key_pem_none								3
	uint8_t b_step = cb_lte_module_ssl_config_client_key_pem_step1;

    uint8_t b_result = FALSE;

	void *pv_user_data = 0;

    if(NULL == pst_process_contral)
    {
        return FALSE;
    }

    if(TRUE == pst_process_contral->operate_contral.b_process_reset)
    {
        b_step = cb_lte_module_ssl_config_client_key_pem_step1;
    }

    do
    {
        switch(b_step)
        {
            case cb_lte_module_ssl_config_client_key_pem_step1:
            {
                uint8_t b_result_temp = 0;

                b_result_temp = lte_module_driver_at_cmd_transmit(3000,pst_ssl,0,
                                                                  lte_module_driver_send_at_ssl_client_key_pem_step1,
                                                                  lte_module_driver_receive_at_ssl_client_key_pem_step1);
                if(1 == b_result_temp)
                {
                    b_step = cb_lte_module_ssl_config_client_key_pem_step2;
                }
                else if(0 == b_result_temp)
                {
                    b_result = FALSE;
                    b_step = cb_lte_module_ssl_config_client_key_pem_none;
                }
            }break;
            case cb_lte_module_ssl_config_client_key_pem_step2:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(3000,pst_ssl,0,
                                                             lte_module_driver_send_at_ssl_client_key_pem_step2,
                                                             lte_module_driver_receive_at_ssl_client_key_pem_step2))
                {
                    b_step = cb_lte_module_ssl_config_client_key_pem_step3;
                }
                else
                {
                    b_result = FALSE;
                    b_step = cb_lte_module_ssl_config_client_key_pem_none;
                }
            }break;
            case cb_lte_module_ssl_config_client_key_pem_step3:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(3000,pst_ssl,0,
                                                             lte_module_driver_send_at_ssl_client_key_pem_step3,
                                                             lte_module_driver_receive_at_ssl_client_key_pem_step3))
                {
                    b_result = TRUE;
                    b_step = cb_lte_module_ssl_config_client_key_pem_none;
                }
                else
                {
                    b_result = FALSE;
                    b_step = cb_lte_module_ssl_config_client_key_pem_none;
                }
            }break;
            case cb_lte_module_ssl_config_ca_cert_pem_none:
            {
            }break;
            default:
            {
            }break;
        }
    }while(b_step != cb_lte_module_ssl_config_ca_cert_pem_none);

	return b_result;
}
/*ssl authentication mode*/
//AT+QSSLCFG=\"seclevel\"
uint8_t lte_module_driver_send_at_ssl_authentication_mode(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if((NULL == pv_user_data) || (NULL == pst_lte_module_data_info))
	{
		return FALSE;
	}

    lte_module_driver_config_info_ssl *pst_ssl = (lte_module_driver_config_info_ssl *)pv_user_data;

	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+QSSLCFG=\"seclevel\",%d,2\r\n",pst_ssl->b_ssl_context_id))
	{
		return FALSE;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return TRUE;
}
uint8_t lte_module_driver_receive_at_ssl_authentication_mode(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if(NULL == pst_lte_module_data_info)
	{
		return FALSE;
	}

	if((pst_lte_module_data_info->w_data_len <= 8) &&\
	   (strstr((char*)pst_lte_module_data_info->pb_data,"OK") != NULL))
	{
		return TRUE;
	}

	return FALSE;
}
/*ssl version*/
//AT+QSSLCFG=\"seclevel\"
uint8_t lte_module_driver_send_at_ssl_version(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if((NULL == pv_user_data) || (NULL == pst_lte_module_data_info))
	{
		return FALSE;
	}

    lte_module_driver_config_info_ssl *pst_ssl = (lte_module_driver_config_info_ssl *)pv_user_data;

	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+QSSLCFG=\"sslversion\",%d,4\r\n",pst_ssl->b_ssl_context_id))
	{
		return FALSE;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return TRUE;
}
uint8_t lte_module_driver_receive_at_ssl_version(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if(NULL == pst_lte_module_data_info)
	{
		return FALSE;
	}

	if((pst_lte_module_data_info->w_data_len <= 8) &&\
	   (strstr((char*)pst_lte_module_data_info->pb_data,"OK") != NULL))
	{
		return TRUE;
	}

	return FALSE;
}
/*ssl ciphersuite*/
//AT+QSSLCFG=\"ciphersuite\"
uint8_t lte_module_driver_send_at_ssl_ciphersuite(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if((NULL == pv_user_data) || (NULL == pst_lte_module_data_info))
	{
		return FALSE;
	}

    lte_module_driver_config_info_ssl *pst_ssl = (lte_module_driver_config_info_ssl *)pv_user_data;

	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+QSSLCFG=\"ciphersuite\",%d,0xFFFF\r\n",pst_ssl->b_ssl_context_id))
	{
		return FALSE;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return TRUE;
}
uint8_t lte_module_driver_receive_at_ssl_ciphersuite(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if(NULL == pst_lte_module_data_info)
	{
		return FALSE;
	}

	if((pst_lte_module_data_info->w_data_len <= 8) &&\
	   (strstr((char*)pst_lte_module_data_info->pb_data,"OK") != NULL))
	{
		return TRUE;
	}

	return FALSE;
}
/*ssl cert validity check*/
//AT+QSSLCFG=\"ignorelocaltime\"
uint8_t lte_module_driver_send_at_ssl_cert_validity_check(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if((NULL == pv_user_data) || (NULL == pst_lte_module_data_info))
	{
		return FALSE;
	}

    lte_module_driver_config_info_ssl *pst_ssl = (lte_module_driver_config_info_ssl *)pv_user_data;

	if(0 == sprintf((char *)pst_lte_module_data_info->pb_data,"AT+QSSLCFG=\"ignorelocaltime\",%d,1\r\n",pst_ssl->b_ssl_context_id))
	{
		return FALSE;
	}
	pst_lte_module_data_info->w_data_len = strlen((char *)pst_lte_module_data_info->pb_data);

	return TRUE;
}
uint8_t lte_module_driver_receive_at_ssl_cert_validity_check(void *pv_user_data,lte_module_data_info *pst_lte_module_data_info)
{
	if(NULL == pst_lte_module_data_info)
	{
		return FALSE;
	}

	if((pst_lte_module_data_info->w_data_len <= 8) &&\
	   (strstr((char*)pst_lte_module_data_info->pb_data,"OK") != NULL))
	{
		return TRUE;
	}

	return FALSE;
}
uint8_t lte_module_driver_ssl_configure(lte_module_driver_config_info_ssl *pst_ssl,
                                        lte_module_driver_process_contral *pst_process_contral)
{
	#define cb_lte_module_ssl_config_ca_cert								0
	#define cb_lte_module_ssl_config_client_cert							1
	#define cb_lte_module_ssl_config_client_key								2
	#define cb_lte_module_ssl_config_authentication_mode					3
	#define cb_lte_module_ssl_config_version								4
	#define cb_lte_module_ssl_config_cipher_suites							5
	#define cb_lte_module_ssl_config_cert_validity_check					6
	#define cb_lte_module_ssl_config_none									7
    uint8_t b_step = cb_lte_module_ssl_config_ca_cert;

    uint8_t b_result = FALSE;

    if(NULL == pst_process_contral)
    {
        return FALSE;
    }

    if(TRUE == pst_process_contral->operate_contral.b_process_reset)
    {
        b_step = cb_lte_module_ssl_config_ca_cert;
    }

    do{
        switch(b_step)
        {
            case cb_lte_module_ssl_config_ca_cert:
            {
                if(TRUE == lte_module_ssl_config_ca_cert_pem(pst_ssl,pst_process_contral))
                {
                    b_step = cb_lte_module_ssl_config_client_cert;
                }
                else
                {
                    b_step = cb_lte_module_ssl_config_none;
                    b_result = FALSE;
                }
            }break;
            case cb_lte_module_ssl_config_client_cert:
            {
                if(TRUE == lte_module_ssl_config_client_cert_pem(pst_ssl,pst_process_contral))
                {
                    b_step = cb_lte_module_ssl_config_client_key;
                }
                else
                {
                    b_step = cb_lte_module_ssl_config_none;
                    b_result = FALSE;
                }
            }break;
            case cb_lte_module_ssl_config_client_key:
            {
                if(TRUE == lte_module_ssl_config_client_key_pem(pst_ssl,pst_process_contral))
                {
                    b_step = cb_lte_module_ssl_config_authentication_mode;
                }
                else
                {
                    b_step = cb_lte_module_ssl_config_none;
                    b_result = FALSE;
                }
            }break;
            case cb_lte_module_ssl_config_authentication_mode:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(3000,pst_ssl,0,
                                                             lte_module_driver_send_at_ssl_authentication_mode,
                                                             lte_module_driver_receive_at_ssl_authentication_mode))
                {
                    b_step = cb_lte_module_ssl_config_version;
                }
                else
                {
                    b_step = cb_lte_module_ssl_config_none;
                    b_result = FALSE;
                }
            }break;
            case cb_lte_module_ssl_config_version:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(3000,pst_ssl,0,
                                                             lte_module_driver_send_at_ssl_version,
                                                             lte_module_driver_receive_at_ssl_version))
                {
                    b_step = cb_lte_module_ssl_config_cipher_suites;
                }
                else
                {
                    b_step = cb_lte_module_ssl_config_none;
                    b_result = FALSE;
                }
            }break;
            case cb_lte_module_ssl_config_cipher_suites:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(3000,pst_ssl,0,
                                                             lte_module_driver_send_at_ssl_ciphersuite,
                                                             lte_module_driver_receive_at_ssl_ciphersuite))
                {
                    b_step = cb_lte_module_ssl_config_cert_validity_check;
                }
                else
                {
                    b_step = cb_lte_module_ssl_config_none;
                    b_result = FALSE;
                }
            }break;
            case cb_lte_module_ssl_config_cert_validity_check:
            {
                if(TRUE == lte_module_driver_at_cmd_transmit(3000,pst_ssl,0,
                                                             lte_module_driver_send_at_ssl_cert_validity_check,
                                                             lte_module_driver_receive_at_ssl_cert_validity_check))
                {
                    b_step = cb_lte_module_ssl_config_none;
                    b_result = TRUE;
                }
                else
                {
                    b_step = cb_lte_module_ssl_config_none;
                    b_result = FALSE;
                }
            }break;
            case cb_lte_module_ssl_config_none:
            {
            }break;
            default:
            {
            }break;
        }
    }while(b_step != cb_lte_module_ssl_config_none);

    return b_result;
}
