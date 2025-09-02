#include "net_device_data_access.h"
#include "system_info_general_macro.h"
#include "net_device_data.h"
#include "device_info.h"
#include "net_system_info_list.h"
#include "net_system_info_access.h"
#include <ysizet.h>
#include "FreeRTOS.h"//ڌ֨ӽԃ˳Ѳ NO.1
#include "semphr.h"  //ڌ֨ӽԃ˳Ѳ NO.2

SemaphoreHandle_t  pst_net_device_data_access_semaphore = NULL;
static void net_device_data_erase_atom_read_write_atom(uint16_t w_data_in_len,uint8_t *pb_data_in,uint8_t *pb_data_out)
{
    if((NULL == pb_data_in) || (NULL == pb_data_out))
    {
        return;
    }

    if(pst_net_device_data_access_semaphore != 0)
    {
        xSemaphoreTake(pst_net_device_data_access_semaphore, portMAX_DELAY);
    }

    for(uint16_t w_data_len = 0;w_data_len < w_data_in_len;w_data_len++)
    {
        *(pb_data_out + w_data_len) = 0;
    }


    for(uint16_t w_data_len = 0;w_data_len < w_data_in_len;w_data_len++)
    {
        *(pb_data_out + w_data_len) = *(pb_data_in + w_data_len);
    }

    if(pst_net_device_data_access_semaphore != 0)
    {
        xSemaphoreGive(pst_net_device_data_access_semaphore);
    }
}

static void net_device_data_read_write_atom(uint16_t w_data_in_len,uint8_t *pb_data_in,uint8_t *pb_data_out)
{
    if((NULL == pb_data_in) || (NULL == pb_data_out))
    {
        return;
    }

    if(pst_net_device_data_access_semaphore != 0)
    {
        xSemaphoreTake(pst_net_device_data_access_semaphore, portMAX_DELAY);
    }

    for(uint16_t w_data_len = 0;w_data_len < w_data_in_len;w_data_len++)
    {
        *(pb_data_out + w_data_len) = *(pb_data_in + w_data_len);
    }

    if(pst_net_device_data_access_semaphore != 0)
    {
        xSemaphoreGive(pst_net_device_data_access_semaphore);
    }
}

static void net_device_data_erase_atom(uint16_t w_data_in_len,uint8_t *pb_data_in)
{
    if(NULL == pb_data_in)
    {
        return;
    }

    if(pst_net_device_data_access_semaphore != 0)
    {
        xSemaphoreTake(pst_net_device_data_access_semaphore, portMAX_DELAY);
    }

    for(uint16_t w_data_len = 0;w_data_len < w_data_in_len;w_data_len++)
    {
        *(pb_data_in + w_data_len) = 0;
    }

    if(pst_net_device_data_access_semaphore != 0)
    {
        xSemaphoreGive(pst_net_device_data_access_semaphore);
    }
}

uint8_t net_device_data_compare_atom(uint16_t w_data_in_len,uint8_t *pb_data_in,uint8_t *pb_data_out)
{
    if((NULL == pb_data_in) || (NULL == pb_data_out))
    {
        return FALSE;
    }

    if(pst_net_device_data_access_semaphore != 0)
    {
        xSemaphoreTake(pst_net_device_data_access_semaphore, portMAX_DELAY);
    }

    for(uint16_t w_data_len = 0;w_data_len < w_data_in_len;w_data_len++)
    {
        if(*(pb_data_out + w_data_len) != *(pb_data_in + w_data_len))
        {
            if(pst_net_device_data_access_semaphore != 0)
            {
                xSemaphoreGive(pst_net_device_data_access_semaphore);
            }
            return TRUE;
        }
    }

    if(pst_net_device_data_access_semaphore != 0)
    {
        xSemaphoreGive(pst_net_device_data_access_semaphore);
    }

    return FALSE;
}

/**
 * @brief 设备数据读写接口
 * @param dw_device_id 设备ID 设备类型+相位
 * @param b_data_type 设置的操作类型 0:状态 1:设置 2:控制
 * @param w_operate_addr 数据地址
 * @param pw_data_len 数据长度
 * @param pb_data 要处理数据
 * @param b_access_type 操作类型 0:读 1:写
 * @return {*}
 * @author Yao YiLiang
 */
void net_device_data_access(uint32_t  dw_device_id,
                            uint8_t   b_data_type,
                            uint16_t  w_operate_addr,
                            uint16_t *pw_data_len,
                            uint8_t  *pb_data,
                            uint8_t   b_access_type)
{
	const net_data_info *pst_net_data_info = 0;

	if ((NULL == pw_data_len) || (NULL == pb_data))
	{
		return;
	}

	uint8_t b_data_type_temp = 0;

	switch (b_data_type)
	{
		case cb_device_data_access_data_type_status:
		{
			b_data_type_temp = cb_net_data_att_status;
		}
		break;
		case cb_device_data_access_data_type_set:
		{
			b_data_type_temp = cb_net_data_att_set;
		}
		break;
		case cb_device_data_access_data_type_ctrl:
		{
			b_data_type_temp = cb_net_data_att_control;
		}
		break;
		default:
		{
			return;
		}
		break;
	}

	net_device_info     *pst_net_device_info = 0;
	const net_data_list *pst_net_data_list   = 0;

	if (FALSE
	    == net_get_device_info(dw_device_id, ( net_device_info ** )&pst_net_device_info, &st_net_system_info_list))
	{
		*pw_data_len = 0;
		return;
	}

	if (FALSE
	    == net_get_device_data_list(
	        b_data_type_temp, ( const net_data_list ** )(&pst_net_data_list), pst_net_device_info))
	{
		*pw_data_len = 0;
		return;
	}

	if (NULL == pst_net_data_list)
	{
		*pw_data_len = 0;
		return;
	}

	{
		uint16_t w_cnt_i = 0;
		for (w_cnt_i = 0; w_cnt_i < pst_net_data_list->w_net_data_number; w_cnt_i++)
		{
			if (w_operate_addr
			    == (( const net_data_info * )(pst_net_data_list->pst_net_data_info + w_cnt_i))->w_data_address_net)
			{
				pst_net_data_info = (( const net_data_info * )(pst_net_data_list->pst_net_data_info + w_cnt_i));
				break;
			}
		}

		if (w_cnt_i >= pst_net_data_list->w_net_data_number)
		{
			return;
		}
	}

	if ((NULL == pst_net_data_info->pb_data) || (NULL == pst_net_data_info))
	{
		*pw_data_len = 0;
		return;
	}

	if (cb_device_data_access_operate_type_read == b_access_type)
	{
		*pw_data_len = pst_net_data_info->pst_data_attribute->b_len;
		net_device_data_read_write_atom(
		    pst_net_data_info->pst_data_attribute->b_len, pst_net_data_info->pb_data, pb_data);
		return;
	}

	if (cb_device_data_access_operate_type_write == b_access_type)
	{
		if (*pw_data_len > pst_net_data_info->pst_data_attribute->b_len)
		{
			return;
		}

//		net_device_data_erase_atom(pst_net_data_info->pst_data_attribute->b_len, pst_net_data_info->pb_data);
		net_device_data_erase_atom_read_write_atom(*pw_data_len, pb_data, pst_net_data_info->pb_data);
		return;
	}
}

						
void yl_debug_printf(char *str, ... );
// 不需要互斥锁
void net_device_data_access_simple(uint32_t  dw_device_id,
                            uint8_t   b_data_type,
                            uint16_t  w_operate_addr,
                            uint16_t *pw_data_len,
                            uint8_t  *pb_data,
                            uint8_t   b_access_type)
{
	const net_data_info *pst_net_data_info = 0;

	if ((NULL == pw_data_len) || (NULL == pb_data))
	{
		return;
	}

	uint8_t b_data_type_temp = 0;

	switch (b_data_type)
	{
		case cb_device_data_access_data_type_status:
		{
			b_data_type_temp = cb_net_data_att_status;
		}
		break;
		case cb_device_data_access_data_type_set:
		{
			b_data_type_temp = cb_net_data_att_set;
		}
		break;
		default:
		{
			return;
		}
		break;
	}

	net_device_info     *pst_net_device_info = 0;
	const net_data_list *pst_net_data_list   = 0;

	if (FALSE
	    == net_get_device_info(dw_device_id, ( net_device_info ** )&pst_net_device_info, &st_net_system_info_list))
	{
		*pw_data_len = 0;
		yl_debug_printf("net_device_data_access %d\r\n",__LINE__);
		return;
	}

	if (FALSE
	    == net_get_device_data_list(
	        b_data_type_temp, ( const net_data_list ** )(&pst_net_data_list), pst_net_device_info))
	{
		*pw_data_len = 0;
		yl_debug_printf("net_device_data_access %d\r\n",__LINE__);
		return;
	}

	if (NULL == pst_net_data_list)
	{
		*pw_data_len = 0;
		yl_debug_printf("net_device_data_access %d\r\n",__LINE__);
		return;
	}

	{
		uint16_t w_cnt_i = 0;
		for (w_cnt_i = 0; w_cnt_i < pst_net_data_list->w_net_data_number; w_cnt_i++)
		{
			if (w_operate_addr
			    == (( const net_data_info * )(pst_net_data_list->pst_net_data_info + w_cnt_i))->w_data_address_net)
			{
				pst_net_data_info = (( const net_data_info * )(pst_net_data_list->pst_net_data_info + w_cnt_i));
				break;
			}
		}

		if (w_cnt_i >= pst_net_data_list->w_net_data_number)
		{
			yl_debug_printf("net_device_data_access %d\r\n",__LINE__);
			return;
		}
	}

	if ((NULL == pst_net_data_info->pb_data) || (NULL == pst_net_data_info))
	{
		*pw_data_len = 0;
		yl_debug_printf("net_device_data_access %d\r\n",__LINE__);
		return;
	}

	if (cb_device_data_access_operate_type_read == b_access_type)
	{
		*pw_data_len = pst_net_data_info->pst_data_attribute->b_len;

        for (uint16_t w_data_len = 0; w_data_len < pst_net_data_info->pst_data_attribute->b_len; w_data_len++)
        {
            *(pb_data + w_data_len) = *(pst_net_data_info->pb_data + w_data_len);
        }

        return;
	}

	if (cb_device_data_access_operate_type_write == b_access_type)
	{
		if (*pw_data_len > pst_net_data_info->pst_data_attribute->b_len)
		{
			yl_debug_printf("net_device_data_access %d\r\n",__LINE__);
			return;
		}

        for (uint16_t w_data_len = 0; w_data_len < pst_net_data_info->pst_data_attribute->b_len; w_data_len++)
        {
            *(pst_net_data_info->pb_data + w_data_len) = 0;
        }

        for (uint16_t w_data_len = 0; w_data_len < *pw_data_len; w_data_len++)
        {
            *(pst_net_data_info->pb_data + w_data_len) = *(pb_data + w_data_len);
        }
        return;
	}
}


void net_system_data_access(uint8_t b_data_type,
                            uint16_t w_operate_addr,
                            uint16_t *pw_data_len,
                            uint8_t *pb_data,
                            uint8_t b_access_type)
{
    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_system],
                           b_data_type,
                           w_operate_addr,
                           pw_data_len,
                           pb_data,
                           b_access_type);
}

//void net_device_active_data_refresh(uint32_t dw_device_id,
//                                    uint8_t b_data_type,
//                                    uint16_t w_operate_addr,
//                                    uint16_t w_data_in_len,
//                                    uint8_t *pb_data_in)
//{
//    if(NULL == pb_data_in)
//    {
//        return;
//    }
//
//    net_data_list *pst_net_data = 0;
//    net_data_info *pst_net_data_info = 0;
//    uint8_t b_data_type_temp = 0;
//
//    switch(b_data_type)
//    {
//        case cb_device_data_access_data_type_status:
//        {
//            b_data_type_temp = cb_net_data_att_status;
//        }break;
//        case cb_device_data_access_data_type_set:
//        {
//            b_data_type_temp = cb_net_data_att_set;
//        }break;
//        default:
//        {
//            return;
//        }break;
//    }
//
//    if(FALSE == net_get_device_data_list(dw_device_id,
//                                         b_data_type_temp,
//                                         (net_data_list **)&pst_net_data,
//                                         &st_net_system_info_list))
//    {
//        return;
//    }
//
//    if(NULL == pst_net_data)
//    {
//        return;
//    }
//
//    {
//        uint16_t w_cnt_i = 0;
//        for(w_cnt_i = 0;w_cnt_i < pst_net_data->w_net_data_number;w_cnt_i++)
//        {
//            if(w_operate_addr == ((net_data_info *)(pst_net_data->pst_net_data_info + w_cnt_i))->w_data_address_net)
//            {
//                pst_net_data_info = ((net_data_info *)(pst_net_data->pst_net_data_info + w_cnt_i));
//                break;
//            }
//        }
//
//        if(w_cnt_i >= pst_net_data->w_net_data_number)
//        {
//            return;
//        }
//    }
//
//    if(FALSE == pst_net_data_info->st_net_data_auxiliary_config.b_active_push_enable)
//    {
//        return;
//    }
//
//    if(w_data_in_len > pst_net_data_info->pst_data_attribute->b_len)
//    {
//        return;
//    }
//
//    if(NULL == pst_net_data_info->pb_data)
//    {
//         return;
//    }
//
//    net_device_data_erase_atom(pst_net_data_info->pst_data_attribute->b_len,
//                               pst_net_data_info->pb_data);
//
//    net_device_data_read_write_atom(w_data_in_len,
//                                    pb_data_in,
//                                    pst_net_data_info->pb_data);
//
//    pst_net_data_info->st_net_data_auxiliary_config.dw_data_modify_count++;
//}

void net_device_data_access_initial()
{
    pst_net_device_data_access_semaphore = xSemaphoreCreateMutex();
    if(NULL == pst_net_device_data_access_semaphore)
    {

    }
}
