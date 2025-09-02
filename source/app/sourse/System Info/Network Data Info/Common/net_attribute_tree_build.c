#include "net_attribute_tree_build.h"
#include "printf_custom.h"
#include "network_system_type_operate.h"

static uint8_t net_get_device_attribute(uint16_t *pw_data_out_len,uint8_t *pb_data_out,net_device_info *pst_device_info,uint8_t b_offline);

uint8_t net_get_device_attribute(uint16_t *pw_data_out_len,\
                                uint8_t *pb_data_out,\
                                net_device_info *pst_device_info,\
                                uint8_t b_attribute_tree_type)
{
	if((NULL == pw_data_out_len) || (NULL == pb_data_out) || (NULL == pst_device_info))
	{
		return FALSE;
	}

	uint16_t w_data_len = 0;
	uint16_t w_device_data_list_index = 0;
	uint16_t w_data_list_index = 0;
	uint16_t w_data_list_segment_start = 0;
	uint16_t w_data_list_segment_number = 0;

	#define cb_get_device_data_list_index						0
	#define cb_get_continuous_data_attribute_number				1
	#define cb_get_data_attribute_header						2
	#define cb_get_data_attribute								3
	#define cb_complete											4

	uint8_t b_step = cb_get_device_data_list_index;

    uint8_t b_protocal_type = cb_system_protocal_type_ck;
    check_system_protocal_type(&b_protocal_type);

	do{
		switch(b_step)
		{
			case cb_get_device_data_list_index:
			{
				if(w_device_data_list_index < pst_device_info->pst_net_data_list_info->w_net_data_list_length)
				{
					b_step = cb_get_continuous_data_attribute_number;
				}
				else
				{
					*pw_data_out_len += w_data_len;
					b_step = cb_complete;
				}
				break;
			}
			case cb_get_continuous_data_attribute_number:
			{
				w_data_list_segment_number = 0;
                if((cb_net_attribute_tree_type_offline != b_attribute_tree_type) &&\
                   (cb_net_attribute_tree_type_set_modify != b_attribute_tree_type))
                {
                    w_data_list_segment_start = w_data_list_index;
                }

                for(;;)
                {
                    if((w_data_list_index + 1) < pst_device_info->pst_net_data_list_info->pst_net_data_list[w_device_data_list_index].w_net_data_number)
                    {
                        if((cb_net_attribute_tree_type_offline != b_attribute_tree_type) &&\
                           (cb_net_attribute_tree_type_set_modify != b_attribute_tree_type))
                        {
                            if((pst_device_info->
                                pst_net_data_list_info->
                                pst_net_data_list[w_device_data_list_index].
                                pst_net_data_info[w_data_list_index].
                                w_data_address_net + 1) !=\
                               pst_device_info->
                               pst_net_data_list_info->
                               pst_net_data_list[w_device_data_list_index].
                               pst_net_data_info[w_data_list_index + 1].
                               w_data_address_net)
                            {
                                w_data_list_segment_number++;
                                w_data_list_index++;
                                break;
                            }
                            else
                            {
                                w_data_list_segment_number++;
                                w_data_list_index++;
                            }
                        }
                        else if(cb_net_attribute_tree_type_offline == b_attribute_tree_type)
                        {
                            if(pst_device_info->
                               pst_net_data_list_info->
                               pst_net_data_list[w_device_data_list_index].
                               pst_net_data_info[w_data_list_index].
                               st_net_data_offline_config.
                               b_offline_send_enable)
                            {
                                if(0 == w_data_list_segment_number)
                                {
                                    w_data_list_segment_start = w_data_list_index;
                                }
                                w_data_list_segment_number++;
                                if(((pst_device_info->
                                    pst_net_data_list_info->
                                    pst_net_data_list[w_device_data_list_index].
                                    pst_net_data_info[w_data_list_index].
                                    w_data_address_net + 1) !=\
                                   pst_device_info->
                                   pst_net_data_list_info->
                                   pst_net_data_list[w_device_data_list_index].
                                   pst_net_data_info[w_data_list_index + 1].
                                   w_data_address_net) ||\
                                    !(pst_device_info->
                                       pst_net_data_list_info->
                                       pst_net_data_list[w_device_data_list_index].
                                       pst_net_data_info[w_data_list_index + 1].
                                       st_net_data_offline_config.
                                       b_offline_send_enable))
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
                        else// if(cb_net_attribute_tree_type_set_modify == b_attribute_tree_type)
                        {
                            if((NULL != pst_device_info->pst_net_data_list_info->pst_net_data_list[w_device_data_list_index].pst_net_data_info[w_data_list_index].pst_net_data_set_attribute) \
								&& (pst_device_info->pst_net_data_list_info->pst_net_data_list[w_device_data_list_index].pst_net_data_info[w_data_list_index].pst_net_data_set_attribute->f_modify == 0x01))
                            {
                                if(0 == w_data_list_segment_number)
                                {
                                    w_data_list_segment_start = w_data_list_index;
                                }
//                                pst_device_info->
//                                    pst_net_data_list_info->
//                                    pst_net_data_list[w_device_data_list_index].
//                                    pst_net_data_info[w_data_list_index].
//                                    pst_net_data_set_attribute->
//                                    f_modify = FALSE;
                                w_data_list_segment_number++;
                                if(((pst_device_info->
                                    pst_net_data_list_info->
                                    pst_net_data_list[w_device_data_list_index].
                                    pst_net_data_info[w_data_list_index].
                                    w_data_address_net + 1) !=\
                                               pst_device_info->
                                               pst_net_data_list_info->
                                               pst_net_data_list[w_device_data_list_index].
                                               pst_net_data_info[w_data_list_index + 1].
                                               w_data_address_net) ||\
                                   ((pst_device_info->
                                       pst_net_data_list_info->
                                       pst_net_data_list[w_device_data_list_index].
                                       pst_net_data_info[w_data_list_index + 1].
                                       pst_net_data_set_attribute->
                                       f_modify) &&\
                                      (NULL != pst_device_info->
                                                pst_net_data_list_info->
                                                pst_net_data_list[w_device_data_list_index].
                                                pst_net_data_info[w_data_list_index + 1].
                                                pst_net_data_set_attribute)) ||\
                                   (NULL == pst_device_info->
                                                pst_net_data_list_info->
                                                pst_net_data_list[w_device_data_list_index].
                                                pst_net_data_info[w_data_list_index + 1].
                                                pst_net_data_set_attribute))
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
                    else if((w_data_list_index + 1) == pst_device_info->pst_net_data_list_info->pst_net_data_list[w_device_data_list_index].w_net_data_number)
                    {
                        if(((cb_net_attribute_tree_type_offline != b_attribute_tree_type) &&\
                            (cb_net_attribute_tree_type_set_modify != b_attribute_tree_type)) ||\
                           (cb_net_attribute_tree_type_offline == b_attribute_tree_type &&\
                            pst_device_info->pst_net_data_list_info->pst_net_data_list[w_device_data_list_index].pst_net_data_info[w_data_list_index].st_net_data_offline_config.b_offline_send_enable))
                        {
                            if((0 == w_data_list_segment_number) &&\
                               (cb_net_attribute_tree_type_offline == b_attribute_tree_type))
                            {
                                w_data_list_segment_start = w_data_list_index;
                            }
                            w_data_list_segment_number++;
                        }
                        else if((cb_net_attribute_tree_type_set_modify == b_attribute_tree_type) &&\
                                ((NULL != pst_device_info->
                                                pst_net_data_list_info->
                                                pst_net_data_list[w_device_data_list_index].
                                                pst_net_data_info[w_data_list_index].
                                                pst_net_data_set_attribute) &&\
                                 (pst_device_info->
                                       pst_net_data_list_info->
                                       pst_net_data_list[w_device_data_list_index].
                                       pst_net_data_info[w_data_list_index].
                                       pst_net_data_set_attribute->
                                       f_modify)))
                        {
                            if((0 == w_data_list_segment_number) &&\
                               (cb_net_attribute_tree_type_set_modify == b_attribute_tree_type))
                            {
                                w_data_list_segment_start = w_data_list_index;
                            }
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
					b_step = cb_get_data_attribute_header;
				}
				else
				{
                    w_data_list_index = 0;
					w_device_data_list_index++;
					b_step = cb_get_device_data_list_index;
				}
				break;
			}
			case cb_get_data_attribute_header:
			{
				uint8_t b_result_temp = TRUE;

				b_result_temp &= int_to_bytes_big_endain(&pb_data_out[w_data_len],*pst_device_info->pdw_device_id,3);
				w_data_len += 3;

				b_result_temp &=  int_to_bytes_big_endain(&pb_data_out[w_data_len],
														  pst_device_info->
														  pst_net_data_list_info->
														  pst_net_data_list[w_device_data_list_index].
														  pst_net_data_info[w_data_list_segment_start].
														  w_data_address_net,
														  2);
				w_data_len += 2;

				b_result_temp &= int_to_bytes_big_endain(&pb_data_out[w_data_len],w_data_list_segment_number,2);
				w_data_len += 2;

				b_step = cb_get_data_attribute;

				if(FALSE == b_result_temp)
				{
					return FALSE;
				}
				break;
			}
			case cb_get_data_attribute:
			{
				uint8_t b_result_temp = TRUE;
                uint16_t w_data_index_temp = 0;
                uint8_t b_attribute_len = 0;

				for(uint16_t w_cnt_1 = 0;w_cnt_1 < w_data_list_segment_number;w_cnt_1++)
				{
                    w_data_index_temp = w_data_len;
                    b_attribute_len = 4;
                    w_data_len++;

					pb_data_out[w_data_len++] = pst_device_info->
												pst_net_data_list_info->
												pst_net_data_list[w_device_data_list_index].
												pst_net_data_info[w_data_list_segment_start + w_cnt_1].
												pst_data_attribute->
												b_read_write_type;

					pb_data_out[w_data_len++] = pst_device_info->
												pst_net_data_list_info->
												pst_net_data_list[w_device_data_list_index].
												pst_net_data_info[w_data_list_segment_start + w_cnt_1].
												pst_data_attribute->
												b_len;

					b_result_temp &= int_to_bytes_big_endain(&pb_data_out[w_data_len],
															 pst_device_info->
															 pst_net_data_list_info->
															 pst_net_data_list[w_device_data_list_index].
															 pst_net_data_info[w_data_list_segment_start + w_cnt_1].
															 pst_data_attribute->
															 w_unit,
															 2);
					w_data_len += 2;

                    if((NULL != pst_device_info->
                               pst_net_data_list_info->
                               pst_net_data_list[w_device_data_list_index].
                               pst_net_data_info[w_data_list_segment_start + w_cnt_1].
                               pst_net_data_set_attribute) &&\
                       (cb_system_protocal_type_ck != b_protocal_type))
                    {
                        b_result_temp &= int_to_bytes_big_endain(&pb_data_out[w_data_len],
                                                                 pst_device_info->
                                                                     pst_net_data_list_info->
                                                                     pst_net_data_list[w_device_data_list_index].
                                                                     pst_net_data_info[w_data_list_segment_start + w_cnt_1].
                                                                     pst_net_data_set_attribute->
                                                                     dw_max_data,
                                                                 pst_device_info->
                                                                     pst_net_data_list_info->
                                                                     pst_net_data_list[w_device_data_list_index].
                                                                     pst_net_data_info[w_data_list_segment_start + w_cnt_1].
                                                                     pst_data_attribute->
                                                                     b_len);
                        w_data_len += pst_device_info->
                                         pst_net_data_list_info->
                                         pst_net_data_list[w_device_data_list_index].
                                         pst_net_data_info[w_data_list_segment_start + w_cnt_1].
                                         pst_data_attribute->
                                         b_len;

                        b_result_temp &= int_to_bytes_big_endain(&pb_data_out[w_data_len],
                                                                 pst_device_info->
                                                                     pst_net_data_list_info->
                                                                     pst_net_data_list[w_device_data_list_index].
                                                                     pst_net_data_info[w_data_list_segment_start + w_cnt_1].
                                                                     pst_net_data_set_attribute->
                                                                     dw_min_data,
                                                                 pst_device_info->
                                                                     pst_net_data_list_info->
                                                                     pst_net_data_list[w_device_data_list_index].
                                                                     pst_net_data_info[w_data_list_segment_start + w_cnt_1].
                                                                     pst_data_attribute->
                                                                     b_len);
                        w_data_len += pst_device_info->
                                         pst_net_data_list_info->
                                         pst_net_data_list[w_device_data_list_index].
                                         pst_net_data_info[w_data_list_segment_start + w_cnt_1].
                                         pst_data_attribute->
                                         b_len;

                        b_result_temp &= int_to_bytes_big_endain(&pb_data_out[w_data_len],
                                                                 pst_device_info->
                                                                     pst_net_data_list_info->
                                                                     pst_net_data_list[w_device_data_list_index].
                                                                     pst_net_data_info[w_data_list_segment_start + w_cnt_1].
                                                                     pst_net_data_set_attribute->
                                                                     dw_step,
                                                                 pst_device_info->
                                                                     pst_net_data_list_info->
                                                                     pst_net_data_list[w_device_data_list_index].
                                                                     pst_net_data_info[w_data_list_segment_start + w_cnt_1].
                                                                     pst_data_attribute->
                                                                     b_len);

                        w_data_len += pst_device_info->
                                         pst_net_data_list_info->
                                         pst_net_data_list[w_device_data_list_index].
                                         pst_net_data_info[w_data_list_segment_start + w_cnt_1].
                                         pst_data_attribute->
                                         b_len;
                        b_attribute_len += pst_device_info->
                                             pst_net_data_list_info->
                                             pst_net_data_list[w_device_data_list_index].
                                             pst_net_data_info[w_data_list_segment_start + w_cnt_1].
                                             pst_data_attribute->
                                             b_len * 3;
                        pst_device_info->
                               pst_net_data_list_info->
                               pst_net_data_list[w_device_data_list_index].
                               pst_net_data_info[w_data_list_segment_start + w_cnt_1].
                               pst_net_data_set_attribute->
                               f_modify = FALSE;
                    }

                    pb_data_out[w_data_index_temp] = b_attribute_len;

					if(FALSE == b_result_temp)
					{
						return FALSE;
					}
				}

				b_step = cb_get_continuous_data_attribute_number;
				break;
			}
		}
	}while(b_step != cb_complete);

	return TRUE;
}

uint8_t net_attribute_tree_build(uint16_t *pw_data_out_len,
                                 uint8_t *pb_data_out,
                                 uint16_t w_data_out_buffer_max_len,
                                 net_system_info_list *pst_info_list,
                                 uint8_t b_attribute_tree_type)
{
	if((NULL == pw_data_out_len) || (NULL == pb_data_out) || (NULL == pst_info_list))
    {
        return FALSE;
    }

	uint16_t w_data_len = 0;
	uint16_t w_system_list_index = 0;
	uint16_t w_device_list_index = 0;

    #define cb_get_system_type                         0
    #define cb_get_local_attribute_tree                1
    #define cb_get_system_device_index                 2
    #define cb_get_system_device_attribute             3
	#define cb_complete		  					       4

    uint8_t b_device_tree_build_step = cb_get_system_type;

    uint32_t dwa_device_type_stored[10] = {0,};
    uint8_t b_device_type_stored_num = 0;

    do{
          switch(b_device_tree_build_step)
          {
              case cb_get_system_type:
			  {
                  uint16_t w_data_number = 0;
			  	  uint16_t w_cnt_1 = 0;

                  for(w_cnt_1 = 0;w_cnt_1 < pst_info_list->w_length;w_cnt_1++)
                  {
                      if(pst_info_list->pst_system_info[w_cnt_1].b_type ==\
                         pst_info_list->b_current_type)
                      {
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
                                  uint8_t b_cnt_i = 0;
                                  for(b_cnt_i = 0;b_cnt_i < b_device_type_stored_num;b_cnt_i++)
                                  {
                                      if(dwa_device_type_stored[b_cnt_i] == (*pst_info_list->
                                                                                pst_system_info[w_cnt_1].
                                                                                pst_device_info_list->
                                                                                pst_device_info[w_cnt_2].
                                                                                pdw_device_id & 0x00FFFF00))
                                      {
                                          break;
                                      }
                                  }
                                  if(b_cnt_i < b_device_type_stored_num)
                                  {
                                      continue;
                                  }
                                  else
                                  {
                                      dwa_device_type_stored[b_device_type_stored_num] =\
                                          (*pst_info_list->
                                            pst_system_info[w_cnt_1].
                                            pst_device_info_list->
                                            pst_device_info[w_cnt_2].
                                            pdw_device_id & 0x00FFFF00);
                                      b_device_type_stored_num++;
                                  }

                                  for(uint16_t w_cnt_3 = 0;
                                      w_cnt_3 <  pst_info_list->
                                                 pst_system_info[w_cnt_1].
                                                 pst_device_info_list->
                                                 pst_device_info[w_cnt_2].
                                                 pst_net_data_list_info->w_net_data_list_length;
                                      w_cnt_3++)
                                  {
                                      w_data_number += pst_info_list->
                                                       pst_system_info[w_cnt_1].
                                                       pst_device_info_list->
                                                       pst_device_info[w_cnt_2].
                                                       pst_net_data_list_info->pst_net_data_list[w_cnt_3].w_net_data_number;
                                  }
                              }
                          }
						  break;
                      }
                  }

                  b_device_type_stored_num = 0;
				  if((w_data_number * 0x04) > w_data_out_buffer_max_len)
				  {
					  b_device_tree_build_step = cb_get_local_attribute_tree;
				  }
				  else
				  {
					  w_system_list_index = w_cnt_1;
					  b_device_tree_build_step = cb_get_system_device_index;
				  }
                  break;
			  }
              case cb_get_local_attribute_tree:
              {
                  for(uint16_t w_cnt_1 = 0;w_cnt_1 < pst_info_list->pst_system_info->pst_device_info_list->w_length;w_cnt_1++)
                  {
                      if(*pst_info_list->\
					     pst_system_info->\
						 pst_device_tree_element_info_list->\
						 pst_device_tree_element_info->\
					     pdw_master_device_id ==\
                         *pst_info_list->pst_system_info->pst_device_info_list->pst_device_info[w_cnt_1].pdw_device_id)
                      {
					      if(FALSE == net_get_device_attribute(&w_data_len,
												               pb_data_out,
												  			   &(pst_info_list->pst_system_info->pst_device_info_list->pst_device_info[w_cnt_1]),
                                                               b_attribute_tree_type))
						  {
							  return FALSE;
						  }

						  break;
                      }
                  }
                  b_device_tree_build_step = cb_complete;
                  break;
              }
              case cb_get_system_device_index:
              {
                  for(;;)
                  {
                      if((w_device_list_index) < pst_info_list->pst_system_info[w_system_list_index].pst_device_info_list->w_length)
                      {
                          if(NULL == pst_info_list->
                                     pst_system_info[w_system_list_index].
                                     pst_device_info_list->
                                     pst_device_info[w_device_list_index].
                                     pst_net_data_list_info)
                          {
                              w_device_list_index++;
                              continue;
                          }
                          else
                          {
                              {
                                  uint8_t b_cnt_i = 0;

                                  for(b_cnt_i = 0;b_cnt_i < b_device_type_stored_num;b_cnt_i++)
                                  {
                                      if(dwa_device_type_stored[b_cnt_i] == (*pst_info_list->
                                                                                pst_system_info[w_system_list_index].
                                                                                pst_device_info_list->
                                                                                pst_device_info[w_device_list_index].
                                                                                pdw_device_id & 0x00FFFF00))
                                      {
                                          break;
                                      }
                                  }
                                  if(b_cnt_i < b_device_type_stored_num)
                                  {
                                      w_device_list_index++;
                                      continue;
                                  }
                                  else
                                  {
                                      dwa_device_type_stored[b_device_type_stored_num] =\
                                          (*pst_info_list->
                                            pst_system_info[w_system_list_index].
                                            pst_device_info_list->
                                            pst_device_info[w_device_list_index].
                                            pdw_device_id & 0x00FFFF00);
                                      b_device_type_stored_num++;
                                  }
                              }

                              if(cb_net_attribute_tree_type_set_modify != b_attribute_tree_type)
                              {
								#if (device_tree_print)
                                DebugPrintf(TRUE,"Device Attribute Tree Build:%06X\r\n",*pst_info_list->pst_system_info[w_system_list_index].pst_device_info_list->pst_device_info[w_device_list_index].pdw_device_id);
								#endif
                              }

                              b_device_tree_build_step = cb_get_system_device_attribute;
                              break;
                          }
                      }
                      else
                      {
                          b_device_tree_build_step = cb_complete;
                          w_device_list_index = 0;
                          break;
                      }
                  }
				  break;
              }
			  case cb_get_system_device_attribute:
              {
			  	  if(FALSE == net_get_device_attribute(&w_data_len,
													   &pb_data_out[w_data_len],
													   &(pst_info_list->
														 pst_system_info[w_system_list_index].
														 pst_device_info_list->
														 pst_device_info[w_device_list_index]),
                                                       b_attribute_tree_type))
				  {
					  return FALSE;
				  }

                  w_device_list_index++;
				  b_device_tree_build_step = cb_get_system_device_index;
                  break;
              }
          }
    }while(b_device_tree_build_step != cb_complete);

    if(0 == w_data_len)
    {
        *pw_data_out_len = 0;
        return FALSE;
    }
	else
    {
        *pw_data_out_len = w_data_len;
        return TRUE;
    }
}
//uint8_t net_offline_attribute_tree_build(uint16_t *pw_data_out_len,
//                                         uint8_t *pb_data_out,
//                                         uint16_t w_data_out_buffer_max_len,
//                                         net_system_info_list *pst_info_list)
//{
//	if((NULL == pw_data_out_len) || (NULL == pb_data_out) || (NULL == pst_info_list))
//    {
//        return FALSE;
//    }
//
//	uint16_t w_data_len = 0;
//	uint16_t w_system_list_index = 0;
//	uint16_t w_device_list_index = 0;
//
//    #define cb_get_system_type                         0
//    #define cb_get_local_attribute_tree                1
//    #define cb_get_system_device_index                 2
//    #define cb_get_system_device_attribute             3
//	#define cb_complete		  					       4
//
//    uint8_t b_device_tree_build_step = cb_get_system_type;
//
//    do{
//          switch(b_device_tree_build_step)
//          {
//              case cb_get_system_type:
//			  {
//                  uint16_t w_data_number = 0;
//			  	  uint16_t w_cnt_1 = 0;
//
//                  for(w_cnt_1 = 0;w_cnt_1 < pst_info_list->w_length;w_cnt_1++)
//                  {
//                      if(pst_info_list->pst_system_info[w_cnt_1].b_type ==\
//                         pst_info_list->b_current_type)
//                      {
//						  for(uint16_t w_cnt_2 = 0;w_cnt_2 < pst_info_list->pst_system_info[w_cnt_1].pst_device_info_list->w_length;w_cnt_2++)
//						  {
//                              if(NULL == pst_info_list->
//									     pst_system_info[w_cnt_1].
//									     pst_device_info_list->
//									     pst_device_info[w_cnt_2].
//									     pst_net_data_list_info)
//                              {
//                                  continue;
//                              }
//
//							  for(uint16_t w_cnt_3 = 0;
//								  w_cnt_3 <  pst_info_list->
//											 pst_system_info[w_cnt_1].
//											 pst_device_info_list->
//											 pst_device_info[w_cnt_2].
//											 pst_net_data_list_info->w_net_data_list_length;
//								  w_cnt_3++)
//							  {
//								  w_data_number += pst_info_list->
//                                                   pst_system_info[w_cnt_1].
//                                                   pst_device_info_list->
//                                                   pst_device_info[w_cnt_2].
//                                                   pst_net_data_list_info->pst_net_data_list[w_cnt_3].w_net_data_number;
//							  }
//						  }
//						  break;
//                      }
//                  }
//
//				  if((w_data_number * 0x04) > w_data_out_buffer_max_len)
//				  {
//					  b_device_tree_build_step = cb_get_local_attribute_tree;
//				  }
//				  else
//				  {
//					  w_system_list_index = w_cnt_1;
//					  b_device_tree_build_step = cb_get_system_device_index;
//				  }
//                  break;
//			  }
//              case cb_get_local_attribute_tree:
//              {
//                  for(uint16_t w_cnt_1 = 0;w_cnt_1 < pst_info_list->pst_system_info->pst_device_info_list->w_length;w_cnt_1++)
//                  {
//                      if(pst_info_list->\
//					     pst_system_info->\
//						 pst_device_tree_element_info_list->\
//						 pst_device_tree_element_info->\
//					     dw_master_device_id ==\
//                         pst_info_list->pst_system_info->pst_device_info_list->pst_device_info[w_cnt_1].dw_device_id)
//                      {
//					      if(FALSE == net_get_device_attribute(&w_data_len,
//												               pb_data_out,
//												  			   &(pst_info_list->pst_system_info->pst_device_info_list->pst_device_info[w_cnt_1])))
//						  {
//							  return FALSE;
//						  }
//
//						  break;
//                      }
//                  }
//                  b_device_tree_build_step = cb_complete;
//                  break;
//              }
//              case cb_get_system_device_index:
//              {
//                  for(;;)
//                  {
//                      if((w_device_list_index) < pst_info_list->pst_system_info[w_system_list_index].pst_device_info_list->w_length)
//                      {
//                          if(NULL == pst_info_list->
//                                     pst_system_info[w_system_list_index].
//                                     pst_device_info_list->
//                                     pst_device_info[w_device_list_index].
//                                     pst_net_data_list_info)
//                          {
//                              w_device_list_index++;
//                              continue;
//                          }
//                          else
//                          {
//                              b_device_tree_build_step = cb_get_system_device_attribute;
//                              break;
//                          }
//                      }
//                      else
//                      {
//                          b_device_tree_build_step = cb_complete;
//                          w_device_list_index = 0;
//                          break;
//                      }
//                  }
//				  break;
//              }
//			  case cb_get_system_device_attribute:
//              {
//			  	  if(FALSE == net_get_device_attribute(&w_data_len,
//													   &pb_data_out[w_data_len],
//													   &(pst_info_list->
//														 pst_system_info[w_system_list_index].
//														 pst_device_info_list->
//														 pst_device_info[w_device_list_index])))
//				  {
//					  return FALSE;
//				  }
//
//                  w_device_list_index++;
//				  b_device_tree_build_step = cb_get_system_device_index;
//                  break;
//              }
//          }
//    }while(b_device_tree_build_step != cb_complete);
//
//	*pw_data_out_len = w_data_len;
//
//	return TRUE;
//}

