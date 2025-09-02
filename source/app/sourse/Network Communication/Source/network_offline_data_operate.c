#include "network_offline_data_operate.h"
#include "spi_flash_address_allocate.h"
#include "application_general_macro.h"
#include "portmacro.h"
#include "freertos_user_config.h"
#include "system_timestamp_generate.h"
#include "net_system_info.h"
#include "net_system_info_list.h"
#include <string.h>
#include "application_general_func.h"
#include "extern_flash_operate.h"


#define cb_offline_data_interval_1s             1
#define cb_offline_data_interval_10s            2

#define cb_offline_data_len                     256
#define cb_offline_body_data_len                cb_offline_data_len - 4

typedef struct
{
    uint32_t dw_flash_save_start;
    uint32_t dw_flash_save_end;
    uint16_t w_flash_save_cnt;
}offline_data_operate_info;

typedef struct
{
    uint32_t dw_timestamp;
    uint8_t *pb_data;
    uint16_t w_data_len;
}offline_data_info;

offline_data_operate_info st_offline_data_operate_info =
{
    cdw_flash_addr_alloc_offline_data_start,
    cdw_flash_addr_alloc_offline_data_start,
    0,
};

uint8_t f_offline_save_en = FALSE;

void enable_save_offline_data(uint8_t f_enable)
{
    f_offline_save_en = f_enable;
}

uint8_t check_have_offline_data()//加个参数判断网络是否稳定，等网络通讯稳定
{
    if(st_offline_data_operate_info.w_flash_save_cnt != 0)
    {
    	//上次上传和本次上传时间间隔大于100ms允许上传
    	{
			static TickType_t x_start_time = 0;
        	TickType_t x_curent_time = 0;
			static uint8_t f_init = TRUE;
			if(f_init)
        	{
            	f_init = FALSE;
            	x_start_time = xTaskGetTickCount();
        	}
			x_curent_time = xTaskGetTickCount();
       		if((x_curent_time - x_start_time) >= pdMS_TO_TICKS_USER(10 * 1000))
        	{
        	    f_init = TRUE;
				return TRUE;
       	 	}
		}
    }
    else
    {
        st_offline_data_operate_info.dw_flash_save_start =\
            st_offline_data_operate_info.dw_flash_save_end;
    }
    return FALSE;
}
uint8_t check_offline_save_enable()
{
    if(TRUE == f_offline_save_en)
    {
        static TickType_t x_start_time = 0;
        TickType_t x_curent_time = 0;
        static uint8_t b_interval_1s = 0;
        static uint8_t f_init = TRUE;

        if(f_init)
        {
            f_init = FALSE;
            b_interval_1s = 0;
            x_start_time = xTaskGetTickCount();
        }
        x_curent_time = xTaskGetTickCount();
        if((x_curent_time - x_start_time) >= pdMS_TO_TICKS_USER(10000))
        {
            f_init = TRUE;
            return cb_offline_data_interval_10s;
        }
        else if((x_curent_time - x_start_time)/pdMS_TO_TICKS_USER(1000) != b_interval_1s)
        {
            b_interval_1s = (x_curent_time - x_start_time)/pdMS_TO_TICKS_USER(1000);
            return cb_offline_data_interval_1s;
        }
    }
    return FALSE;
}
uint8_t get_device_offline_data(uint8_t *pb_data_out,\
                                uint16_t *pw_data_out_len,\
                                uint16_t w_max_buff_len,\
                                uint8_t b_interval,\
                                uint32_t dw_device_id,\
                                const net_data_list_info *pst_net_data_list_info)
{
    uint16_t w_data_list_index = 0;
	uint16_t w_data_list_segment_start = 0;
	uint16_t w_data_list_segment_number = 0;
    uint16_t w_data_index = 0;
	uint16_t w_device_data_list_index = 0;

	#define cb_get_data_list						            0
	#define cb_get_continuous_offline_data_number				1
	#define cb_get_offline_data_header						    2
	#define cb_get_offline_data								    3
	#define cb_complete											4

    uint8_t b_step = cb_get_data_list;
	do{
		switch(b_step)
		{
            case cb_get_data_list:
            {
                if(w_device_data_list_index < pst_net_data_list_info->w_net_data_list_length)
                {
                    if(cb_net_data_att_status ==\
                        pst_net_data_list_info->pst_net_data_list[w_device_data_list_index].b_net_data_typde)
                    {
                        b_step = cb_get_continuous_offline_data_number;
                    }
                    else
                    {
                        w_device_data_list_index++;
                    }
                }
                else
                {
                    b_step = cb_complete;
                    *pw_data_out_len += w_data_index;
                }
                break;
            }
			case cb_get_continuous_offline_data_number:
			{
				w_data_list_segment_number = 0;
                for(;;)
                {
                    if((w_data_list_index + 1) <\
                        pst_net_data_list_info->pst_net_data_list[w_device_data_list_index].w_net_data_number)
                    {
                        {
                            if((pst_net_data_list_info->
                                pst_net_data_list[w_device_data_list_index].
                                pst_net_data_info[w_data_list_index].
                                st_net_data_offline_config.
                                b_offline_send_enable) &&\
                               (pst_net_data_list_info->
                                pst_net_data_list[w_device_data_list_index].
                                pst_net_data_info[w_data_list_index].
                                st_net_data_offline_config.
                                b_interval_s == b_interval))
                            {
                                if(0 == w_data_list_segment_number)
                                {
                                    w_data_list_segment_start = w_data_list_index;
                                }
                                w_data_list_segment_number++;
                                if(((pst_net_data_list_info->
                                    pst_net_data_list[w_device_data_list_index].
                                    pst_net_data_info[w_data_list_index].
                                    w_data_address_net + 1) !=\
                                       pst_net_data_list_info->
                                       pst_net_data_list[w_device_data_list_index].
                                       pst_net_data_info[w_data_list_index + 1].
                                       w_data_address_net) ||\
                                    !(pst_net_data_list_info->
                                       pst_net_data_list[w_device_data_list_index].
                                       pst_net_data_info[w_data_list_index + 1].
                                       st_net_data_offline_config.
                                       b_offline_send_enable) ||\
                                    (pst_net_data_list_info->
                                       pst_net_data_list[w_device_data_list_index].
                                       pst_net_data_info[w_data_list_index + 1].
                                       st_net_data_offline_config.
                                       b_interval_s != b_interval))
                                {
                                    w_data_list_index++;
                                    break;
                                }
                                else
                                {
                                    w_data_list_index++;
                                }
                            }
                            else
                            {
                                w_data_list_index++;
                            }
                        }
                    }
                    else if((w_data_list_index + 1) == pst_net_data_list_info->pst_net_data_list[w_device_data_list_index].w_net_data_number)
                    {
                        if((pst_net_data_list_info->
                            pst_net_data_list[w_device_data_list_index].
                            pst_net_data_info[w_data_list_index].
                            st_net_data_offline_config.
                            b_offline_send_enable) &&\
                           (pst_net_data_list_info->
                            pst_net_data_list[w_device_data_list_index].
                            pst_net_data_info[w_data_list_index].
                            st_net_data_offline_config.
                            b_interval_s == b_interval))
                        {
                            w_data_list_segment_start = w_data_list_index;
                            w_data_list_segment_number++;
                        }
                        w_data_list_index++;
                        break;
                    }
                    else
                    {
                        break;
                    }
                }

				if(w_data_list_segment_number > 0)
				{
					b_step = cb_get_offline_data_header;
				}
				else
				{
                    w_data_list_index = 0;
					w_device_data_list_index++;
					b_step = cb_get_data_list;
				}
				break;
			}
			case cb_get_offline_data_header:
			{
				uint8_t b_result_temp = TRUE;

                pb_data_out[w_data_index++] = (uint8_t)((dw_device_id & 0xFF0000) >> 16);
                pb_data_out[w_data_index++] = (uint8_t)((dw_device_id & 0x00FF00) >> 8);
                pb_data_out[w_data_index++] = (uint8_t)(dw_device_id);

				b_result_temp &=  int_to_bytes_big_endain(&pb_data_out[w_data_index],
														  pst_net_data_list_info->
														  pst_net_data_list[w_device_data_list_index].
														  pst_net_data_info[w_data_list_segment_start].
														  w_data_address_net,
														  2);
				w_data_index += 2;

				b_result_temp &= int_to_bytes_big_endain(&pb_data_out[w_data_index],w_data_list_segment_number,2);
				w_data_index += 2;

				b_step = cb_get_offline_data;

				if(FALSE == b_result_temp)
				{
					return FALSE;
				}
				break;
			}
			case cb_get_offline_data:
			{
				for(uint16_t w_cnt_1 = 0;w_cnt_1 < w_data_list_segment_number;w_cnt_1++)
				{
                    if((w_data_index + pst_net_data_list_info->
                                        pst_net_data_list[w_device_data_list_index].
                                        pst_net_data_info[w_data_list_segment_start + w_cnt_1].
                                        pst_data_attribute->
                                        b_len) > w_max_buff_len)
					{
                        return FALSE;
                    }

                    if(cb_net_data_att_1b == pst_net_data_list_info->
                                                pst_net_data_list[w_device_data_list_index].
                                                pst_net_data_info[w_data_list_segment_start + w_cnt_1].
                                                pst_data_attribute->b_size)
                    {
                        memcpy(&pb_data_out[w_data_index],
                               pst_net_data_list_info->
                                pst_net_data_list[w_device_data_list_index].
                                pst_net_data_info[w_data_list_segment_start + w_cnt_1].
                                pb_data,
                               pst_net_data_list_info->
                                pst_net_data_list[w_device_data_list_index].
                                pst_net_data_info[w_data_list_segment_start + w_cnt_1].
                                pst_data_attribute->
                                b_len);
                    }
                    else
                    {
                        for(uint8_t b_cnt_2 = 0;
                            b_cnt_2 < pst_net_data_list_info->
                                        pst_net_data_list[w_device_data_list_index].
                                        pst_net_data_info[w_data_list_segment_start + w_cnt_1].
                                        pst_data_attribute->
                                        b_len;
                            b_cnt_2++)
                        {
                            pb_data_out[w_data_index + b_cnt_2] =\
                                pst_net_data_list_info->
                                pst_net_data_list[w_device_data_list_index].
                                pst_net_data_info[w_data_list_segment_start + w_cnt_1].
                                pb_data[pst_net_data_list_info->
                                            pst_net_data_list[w_device_data_list_index].
                                            pst_net_data_info[w_data_list_segment_start + w_cnt_1].
                                            pst_data_attribute->
                                            b_len - b_cnt_2 - 1];
                        }
                    }
                    w_data_index += pst_net_data_list_info->
                                    pst_net_data_list[w_device_data_list_index].
                                    pst_net_data_info[w_data_list_segment_start + w_cnt_1].
                                    pst_data_attribute->
                                    b_len;
				}

				b_step = cb_get_continuous_offline_data_number;
				break;
			}
		}
	}while(b_step != cb_complete);

	return TRUE;
}
uint8_t offline_data_build(uint8_t *pb_data_out,\
                           uint16_t *pw_data_out_len,\
                           uint16_t w_max_buff_len,\
                           uint8_t b_interval,\
                           net_system_info_list *pst_info_list)
{
	if((NULL == pw_data_out_len) || (NULL == pb_data_out) || (NULL == pst_info_list))
    {
        return FALSE;
    }
	uint16_t w_data_len = 0;
	uint16_t w_system_list_index = 0;
	uint16_t w_device_list_index = 0;
    uint16_t w_device_data_list_index = 0;

    #define cb_get_system_type                         0
    #define cb_get_local_offline_data                  1
    #define cb_get_system_device_index                 2
    #define cb_get_system_device_data_list_index       3
    #define cb_get_system_device_offline_data          4
	#define cb_build_offline_data_complete		  	   5

    uint8_t b_offline_data_build_step = cb_get_system_type;

    do{
          switch(b_offline_data_build_step)
          {
              case cb_get_system_type:
			  {
			  	  uint16_t w_cnt_1 = 0;
                  uint16_t w_data_len_max = 0;

                  for(w_cnt_1 = 0;w_cnt_1 < pst_info_list->w_length;w_cnt_1++)
                  {
                      if(pst_info_list->pst_system_info[w_cnt_1].b_type ==\
                         pst_info_list->b_current_type)
                      {
						  for(uint16_t w_cnt_2 = 0;w_cnt_2 < pst_info_list->pst_system_info[w_cnt_1].pst_device_info_list->w_length;w_cnt_2++)
						  {
                              if(NULL == pst_info_list->
                                         pst_system_info[w_cnt_1].
                                         pst_device_info_list->
                                         pst_device_info[w_cnt_2].
                                         pst_net_data_list_info)
                              {
                                  continue;
                              }

							  for(uint16_t w_cnt_3 = 0;
								  w_cnt_3 <  pst_info_list->
											 pst_system_info[w_cnt_1].
											 pst_device_info_list->
											 pst_device_info[w_cnt_2].
											 pst_net_data_list_info->w_net_data_list_length;
								  w_cnt_3++)
							  {
								  for(uint16_t w_cnt_4 = 0;
								  	  w_cnt_4 <  pst_info_list->
											     pst_system_info[w_cnt_1].
											     pst_device_info_list->
											     pst_device_info[w_cnt_2].
											     pst_net_data_list_info->pst_net_data_list[w_cnt_3].w_net_data_number;
								  	  w_cnt_4++)
								  {
									  if(TRUE == pst_info_list->
									  			 pst_system_info[w_cnt_1].
									  			 pst_device_info_list->
									  			 pst_device_info[w_cnt_2].
									  			 pst_net_data_list_info->
									  			 pst_net_data_list[w_cnt_3].
									  			 pst_net_data_info[w_cnt_4].
									  			 st_net_data_offline_config.
									  			 b_offline_send_enable)
									  {
									      w_data_len_max += pst_info_list->
											                pst_system_info[w_cnt_1].
											                pst_device_info_list->
											                pst_device_info[w_cnt_2].
											                pst_net_data_list_info->
													        pst_net_data_list[w_cnt_3].
													        pst_net_data_info[w_cnt_4].
													        pst_data_attribute->
													        b_len;
									  }
								  }
							  }
						  }
						  break;
                      }
                  }

				  if(w_data_len_max > w_max_buff_len)
				  {
					  b_offline_data_build_step = cb_get_local_offline_data;
				  }
				  else
				  {
					  w_system_list_index = w_cnt_1;
					  b_offline_data_build_step = cb_get_system_device_index;
				  }
                  break;
              }
              case cb_get_local_offline_data:
              {
                  for(uint16_t w_cnt_1 = 0;w_cnt_1 < pst_info_list->pst_system_info->pst_device_info_list->w_length;w_cnt_1++)
                  {
                      if(*pst_info_list->\
					     pst_system_info[w_system_list_index].\
						 pst_device_tree_element_info_list->\
						 pst_device_tree_element_info->\
					     pdw_master_device_id ==\
                         *pst_info_list->pst_system_info[w_system_list_index].pst_device_info_list->pst_device_info[w_cnt_1].pdw_device_id)
                      {

                          if(FALSE == get_device_offline_data(&pb_data_out[w_data_len],
                                                                &w_data_len,
                                                                w_max_buff_len,
                                                                b_interval,
                                                                *pst_info_list->
                                                                    pst_system_info[w_system_list_index].
                                                                    pst_device_info_list->
                                                                    pst_device_info[w_cnt_1].
                                                                    pdw_device_id,
                                                                pst_info_list->
                                                                    pst_system_info[w_system_list_index].
                                                                    pst_device_info_list->
                                                                    pst_device_info[w_cnt_1].
                                                                    pst_net_data_list_info))
                          {
                              return FALSE;
                          }
						  break;
                      }
                  }
                  b_offline_data_build_step = cb_build_offline_data_complete;
                  break;
              }
              case cb_get_system_device_index:
              {
                  for(;;)
                  {
                      if((w_device_list_index + 1) <= pst_info_list->pst_system_info[w_system_list_index].pst_device_info_list->w_length)
                      {
                          if((NULL == pst_info_list->
                                     pst_system_info[w_system_list_index].
                                     pst_device_info_list->
                                     pst_device_info[w_device_list_index].
                                     pst_net_data_list_info) ||
                             (cb_net_device_add != pst_info_list->
                                                   pst_system_info[w_system_list_index].
                                                   pst_device_info_list->
                                                   pst_device_info[w_device_list_index].
                                                   pst_net_device_status->
                                                   b_communicate_status))
                          {
                              w_device_list_index++;
                              continue;
                          }
                          else
                          {
                              b_offline_data_build_step = cb_get_system_device_data_list_index;
                              break;
                          }
                      }
                      else
                      {
                          b_offline_data_build_step = cb_build_offline_data_complete;
                          w_device_list_index = 0;
                          break;
                      }
                  }
				  break;
              }
              case cb_get_system_device_data_list_index:
              {
                  for(;;)
                  {
                      if((w_device_data_list_index + 1) <= pst_info_list->
                                                           pst_system_info[w_system_list_index].
                                                           pst_device_info_list->
                                                           pst_device_info[w_device_list_index].
                                                           pst_net_data_list_info->
                                                           w_net_data_list_length)
                      {
                          if(cb_net_data_att_status != pst_info_list->
                                                       pst_system_info[w_system_list_index].
                                                       pst_device_info_list->
                                                       pst_device_info[w_device_list_index].
                                                       pst_net_data_list_info->
                                                       pst_net_data_list[w_device_data_list_index].
                                                       b_net_data_typde)
                          {
                              w_device_data_list_index++;
                              continue;
                          }
                          else
                          {
                              b_offline_data_build_step = cb_get_system_device_offline_data;
                              break;
                          }
                      }
                      else
                      {
                          w_device_data_list_index = 0;
                          w_device_list_index++;
                          b_offline_data_build_step = cb_get_system_device_index;
                          break;
                      }
                  }
                  break;
              }
			  case cb_get_system_device_offline_data:
              {
                  if(FALSE == get_device_offline_data(&pb_data_out[w_data_len],
                                      &w_data_len,
                                      w_max_buff_len,
                                      b_interval,
                                      *pst_info_list->
                                          pst_system_info[w_system_list_index].
                                          pst_device_info_list->
                                          pst_device_info[w_device_list_index].
                                          pdw_device_id,
                                      pst_info_list->
                                          pst_system_info[w_system_list_index].
                                          pst_device_info_list->
                                          pst_device_info[w_device_list_index].
                                          pst_net_data_list_info))
                  {
                      return FALSE;
                  }

				  w_device_data_list_index++;
				  b_offline_data_build_step = cb_get_system_device_data_list_index;
                  break;
              }
          }
    }while(b_offline_data_build_step != cb_build_offline_data_complete);

    *pw_data_out_len = w_data_len;

	return TRUE;

}

uint8_t offline_data_flash_write(offline_data_info *pst_offline_data_info)
{
    uint8_t ba_offline_data[cb_offline_data_len] = {0,};
    uint16_t w_data_index = 0;

    if((st_offline_data_operate_info.dw_flash_save_end + cb_offline_data_len) >= cdw_flash_addr_alloc_offline_data_end)
    {
        st_offline_data_operate_info.dw_flash_save_end = cdw_flash_addr_alloc_offline_data_start;
        if(st_offline_data_operate_info.dw_flash_save_start < cdw_flash_addr_alloc_offline_data_start + 4 *1024)
        {
            st_offline_data_operate_info.w_flash_save_cnt -=\
                ((cdw_flash_addr_alloc_offline_data_start + 4 * 1024) -\
                    st_offline_data_operate_info.dw_flash_save_start) / cb_offline_data_len;
            st_offline_data_operate_info.dw_flash_save_start = cdw_flash_addr_alloc_offline_data_start + 4 * 1024;
        }
    }

    if(FALSE == int_to_bytes_big_endain(ba_offline_data,pst_offline_data_info->w_data_len + 4,2))
    {
        return FALSE;
    }
    w_data_index += 2;

    if(FALSE == int_to_bytes_big_endain(&ba_offline_data[w_data_index],pst_offline_data_info->dw_timestamp,4))
    {
        return FALSE;
    }
    w_data_index += 4;

    for(;w_data_index - 6 < pst_offline_data_info->w_data_len;w_data_index++)
    {
        ba_offline_data[w_data_index] = pst_offline_data_info->pb_data[w_data_index - 6];
    }

    if(FALSE == extern_flash_wirte(st_offline_data_operate_info.dw_flash_save_end,
                                   cb_offline_data_len,
                                   ba_offline_data))
    {
        return FALSE;
    }
    else
    {
        st_offline_data_operate_info.dw_flash_save_end += cb_offline_data_len;
        if(st_offline_data_operate_info.w_flash_save_cnt >=\
            (uint16_t)((cdw_flash_addr_alloc_offline_data_end - cdw_flash_addr_alloc_offline_data_start + 1) / cb_offline_data_len))
        {
            st_offline_data_operate_info.w_flash_save_cnt =\
                (uint16_t)((cdw_flash_addr_alloc_offline_data_end - cdw_flash_addr_alloc_offline_data_start + 1) / cb_offline_data_len);
        }
        else
        {
            st_offline_data_operate_info.w_flash_save_cnt++;
        }
    }
    return TRUE;
}

void offline_data_flash_read_shift()
{
    if(st_offline_data_operate_info.w_flash_save_cnt != 0)
    {
        st_offline_data_operate_info.dw_flash_save_start += cb_offline_data_len;
        st_offline_data_operate_info.w_flash_save_cnt--;
    }
}

uint8_t offline_data_flash_read(uint8_t *pb_data_out,uint16_t *pw_data_len)
{
    if(st_offline_data_operate_info.dw_flash_save_start >= cdw_flash_addr_alloc_offline_data_end)
    {
        st_offline_data_operate_info.dw_flash_save_start = cdw_flash_addr_alloc_offline_data_start;
    }
    else if((st_offline_data_operate_info.dw_flash_save_start ==\
             st_offline_data_operate_info.dw_flash_save_end) &&\
            (st_offline_data_operate_info.w_flash_save_cnt == 0))
    {
        return FALSE;
    }

    uint8_t ba_offline_data_buff[cb_offline_data_len];
    uint16_t w_data_len = 0;

    if(FALSE == extern_flash_read(st_offline_data_operate_info.dw_flash_save_start,
                                  cb_offline_data_len,
                                  ba_offline_data_buff))
    {
        return FALSE;
    }

    if(FALSE == bytes_to_int_big_to_little_endain(ba_offline_data_buff,(uint32_t *)&w_data_len,2))
    {
        return FALSE;
    }

    memcpy(pb_data_out,&ba_offline_data_buff[2],w_data_len);

    *pw_data_len = w_data_len;

    return TRUE;
}

/************************************************************************************************
* Input: b_interval
*
* Output: void
*
* Logic discription: save offline data every 'b_interval' seconds
*
*
* run time:
************************************************************************************************/
uint8_t offline_data_operate(uint8_t b_interval)
{
    offline_data_info st_offline_data_info = {0,};

    get_system_timestamp(&st_offline_data_info.dw_timestamp);
    if(0 != st_offline_data_info.dw_timestamp)
    {
        uint8_t b_result = TRUE;
        st_offline_data_info.pb_data =\
            (uint8_t *)pvPortMallocUser(cb_offline_body_data_len * sizeof(uint8_t),TRUE);

        if(FALSE == offline_data_build(st_offline_data_info.pb_data,
                                       &st_offline_data_info.w_data_len,
                                       cb_offline_body_data_len,
                                       b_interval,
                                       &st_net_system_info_list))
        {
            b_result = FALSE;
        }
        if(FALSE == offline_data_flash_write(&st_offline_data_info))
        {
            b_result = FALSE;
        }
#ifdef OFFLINE_DATA_SAVE_TEST
        uint8_t ba_offline_test_buff[cb_offline_data_len + 2] = {0,};
        offline_data_flash_read(&ba_offline_test_buff[2],(uint16_t *)&ba_offline_test_buff[0]);
        offline_data_flash_read_shift();
#endif

        vPortFree(st_offline_data_info.pb_data);
        return b_result;
    }

    return FALSE;
}

/************************************************************************************************
* Input: void
*
* Output: void
*
* Logic discription: save offline data main func
*
*
* run time:
************************************************************************************************/
void offline_data_save()
{
    uint8_t b_result = 0;

    b_result = check_offline_save_enable();
    if(cb_offline_data_interval_10s == b_result)
    {
        offline_data_operate(10);
    }
//    if((cb_offline_data_interval_1s == b_result) ||\
//       (cb_offline_data_interval_10s == b_result))
//    {
//        offline_data_operate(1);
//    }
}



