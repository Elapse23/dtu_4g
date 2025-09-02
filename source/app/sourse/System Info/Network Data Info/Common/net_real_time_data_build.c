#include "net_real_time_data_build.h"
#include "printf_custom.h"


/*
	获取设备实时数据，查询指令中调用，用于返回查询地址段的数据
*/
uint8_t net_get_device_real_time_attribute_tree(uint8_t b_active_push,
                                      uint32_t dw_device_id,
                                      uint16_t w_start_data_address,
                                      uint16_t w_end_data_address,
                                      uint16_t *pw_data_out_len,
                                      uint8_t *pb_data_out,
                                      uint16_t w_data_out_buffer_size,
                                      const net_data_list *pst_net_data_list)

{
	if((NULL == pw_data_out_len) || (NULL == pb_data_out) || (NULL == pst_net_data_list))
	{
		return FALSE;
	}

	uint16_t w_data_len = 0;
	uint16_t w_data_list_index = 0;
	uint16_t w_data_list_segment_start = 0;
	uint16_t w_data_list_segment_number = 0;

    #define cb_get_rt_data_start_index				            0
	#define cb_get_continuous_rt_data_number				    1
	#define cb_get_rt_data_header						        2
	#define cb_get_realtime_data								3
	#define cb_get_rt_data_complete							    4

	uint8_t b_step = cb_get_rt_data_start_index;

	do{
		switch(b_step)
		{
            case cb_get_rt_data_start_index:
            {
                for(uint16_t w_cnt_i = 0;w_cnt_i < pst_net_data_list->w_net_data_number;w_cnt_i++)
                {
                    if(w_start_data_address == pst_net_data_list->
                                               pst_net_data_info[w_cnt_i].
                                               w_data_address_net)
                    {
                        w_data_list_index = w_cnt_i;		//第几个设备，如u1~u3、v1~v3、w1~w3
                        break;
                    }
                    else if((w_cnt_i + 1) == pst_net_data_list->w_net_data_number)
                    {
                        return FALSE;
                    }
                }

                b_step = cb_get_continuous_rt_data_number;
                break;
            }
			case cb_get_continuous_rt_data_number:
			{
                w_data_list_segment_start = w_data_list_index;
				w_data_list_segment_number = 0;

                for(;;)
                {
                    if(w_data_list_index < pst_net_data_list->w_net_data_number)
                    {
                        if(NULL == pst_net_data_list->pst_net_data_info[w_data_list_index].pst_data_attribute)
                        {
                            w_data_list_index++;
                            break;
                        }
                        else if((pst_net_data_list->
                                pst_net_data_info[w_data_list_index].
                                w_data_address_net < w_start_data_address) ||\
                                pst_net_data_list->
                                pst_net_data_info[w_data_list_index].
                                w_data_address_net > w_end_data_address)
                        {
                            w_data_list_index++;
                            break;
                        }
                        else if((w_data_list_index + 1) == pst_net_data_list->w_net_data_number)
                        {
                            {
                                w_data_list_index++;
                                w_data_list_segment_number++;
                                break;
                            }
                        }
                        else
                        {
                            if((pst_net_data_list->
                                pst_net_data_info[w_data_list_index].
                                w_data_address_net + 1) !=\
                               pst_net_data_list->
                               pst_net_data_info[w_data_list_index + 1].
                               w_data_address_net)
                            {
                                {
                                    w_data_list_index++;
                                    w_data_list_segment_number++;
                                    break;
                                }
                            }
                            else
                            {
                                {
                                    w_data_list_index++;
                                    w_data_list_segment_number++;
                                }
                            }
                        }
                    }
                    else
                    {
                        break;
                    }
                }

                if(w_data_list_segment_number > 0)
				{
					b_step = cb_get_rt_data_header;
				}
                else if(w_data_list_index >= pst_net_data_list->w_net_data_number)
                {
                    b_step = cb_get_rt_data_complete;
                }

				break;
			}
			case cb_get_rt_data_header:
			{
				uint8_t b_result_temp = TRUE;

				b_result_temp &= int_to_bytes_big_endain(&pb_data_out[w_data_len],dw_device_id,3);
				w_data_len += 3;

				b_result_temp &=  int_to_bytes_big_endain(&pb_data_out[w_data_len],
                                                          pst_net_data_list->
														  pst_net_data_info[w_data_list_segment_start].
														  w_data_address_net,
														  2);
				w_data_len += 2;

				b_result_temp &= int_to_bytes_big_endain(&pb_data_out[w_data_len],w_data_list_segment_number,2);
				w_data_len += 2;

				b_step = cb_get_realtime_data;

				if(FALSE == b_result_temp)
				{
					return FALSE;
				}
				break;
			}
			case cb_get_realtime_data:
			{

				{
					uint8_t b_result_temp = TRUE;
					uint16_t w_data_index_temp = 0;
					uint8_t b_attribute_len = 0;

					for(uint16_t w_cnt_1 = 0 ;w_cnt_1 < w_data_list_segment_number ;w_cnt_1++)
					{
						w_data_index_temp = w_data_len;
						b_attribute_len = 4;
						w_data_len++;

						pb_data_out[w_data_len++] = pst_net_data_list->
													pst_net_data_info[w_data_list_segment_start + w_cnt_1].
													pst_data_attribute->
													b_read_write_type;

						pb_data_out[w_data_len++] = pst_net_data_list->
													pst_net_data_info[w_data_list_segment_start + w_cnt_1].
													pst_data_attribute->
													b_len;

						b_result_temp &= int_to_bytes_big_endain(&pb_data_out[w_data_len],
																 pst_net_data_list->
																 pst_net_data_info[w_data_list_segment_start + w_cnt_1].
																 pst_data_attribute->
																 w_unit,
																 2);
						w_data_len += 2;

						if((NULL != pst_net_data_list->
								   pst_net_data_info[w_data_list_segment_start + w_cnt_1].
								   pst_net_data_set_attribute))
						{
							b_result_temp &= int_to_bytes_big_endain(&pb_data_out[w_data_len],
																	 pst_net_data_list->
																		 pst_net_data_info[w_data_list_segment_start + w_cnt_1].
																		 pst_net_data_set_attribute->
																		 dw_max_data,
																	 pst_net_data_list->
																		 pst_net_data_info[w_data_list_segment_start + w_cnt_1].
																		 pst_data_attribute->
																		 b_len);
							w_data_len += pst_net_data_list->
											 pst_net_data_info[w_data_list_segment_start + w_cnt_1].
											 pst_data_attribute->
											 b_len;

							b_result_temp &= int_to_bytes_big_endain(&pb_data_out[w_data_len],
																	 pst_net_data_list->
																		 pst_net_data_info[w_data_list_segment_start + w_cnt_1].
																		 pst_net_data_set_attribute->
																		 dw_min_data,
																	 pst_net_data_list->
																		 pst_net_data_info[w_data_list_segment_start + w_cnt_1].
																		 pst_data_attribute->
																		 b_len);
							w_data_len += pst_net_data_list->
											 pst_net_data_info[w_data_list_segment_start + w_cnt_1].
											 pst_data_attribute->
											 b_len;

							b_result_temp &= int_to_bytes_big_endain(&pb_data_out[w_data_len],
																	 pst_net_data_list->
																		 pst_net_data_info[w_data_list_segment_start + w_cnt_1].
																		 pst_net_data_set_attribute->
																		 dw_step,
																	 pst_net_data_list->
																		 pst_net_data_info[w_data_list_segment_start + w_cnt_1].
																		 pst_data_attribute->
																		 b_len);

							w_data_len += pst_net_data_list->
											 pst_net_data_info[w_data_list_segment_start + w_cnt_1].
											 pst_data_attribute->
											 b_len;
							b_attribute_len += pst_net_data_list->
												 pst_net_data_info[w_data_list_segment_start + w_cnt_1].
												 pst_data_attribute->
												 b_len * 3;
							pst_net_data_list->
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
				}
				b_step = cb_get_continuous_rt_data_number;
				break;
			}
		}
	}while(b_step != cb_get_rt_data_complete);

    *pw_data_out_len = w_data_len;

	return TRUE;
}

/*
	获取设备实时数据，查询指令中调用，用于返回查询地址段的数据
*/
uint8_t net_get_device_real_time_data(uint8_t b_active_push,
                                      uint32_t dw_device_id,
                                      uint16_t w_start_data_address,
                                      uint16_t w_end_data_address,
                                      uint16_t *pw_data_out_len,
                                      uint8_t *pb_data_out,
                                      uint16_t w_data_out_buffer_size,
                                      const net_data_list *pst_net_data_list)
{
	if((NULL == pw_data_out_len) || (NULL == pb_data_out) || (NULL == pst_net_data_list))
	{
		return FALSE;
	}

	uint16_t w_data_len = 0;
	uint16_t w_data_list_index = 0;
	uint16_t w_data_list_segment_start = 0;
	uint16_t w_data_list_segment_number = 0;

    #define cb_get_rt_data_start_index				            0
	#define cb_get_continuous_rt_data_number				    1
	#define cb_get_rt_data_header						        2
	#define cb_get_realtime_data								3
	#define cb_get_rt_data_complete							    4

	uint8_t b_step = cb_get_rt_data_start_index;

	do{
		switch(b_step)
		{
            case cb_get_rt_data_start_index:
            {
                for(uint16_t w_cnt_i = 0;w_cnt_i < pst_net_data_list->w_net_data_number;w_cnt_i++)
                {
                    if(w_start_data_address == pst_net_data_list->
                                               pst_net_data_info[w_cnt_i].
                                               w_data_address_net)
                    {
                        w_data_list_index = w_cnt_i;		//第几个设备，如u1~u3、v1~v3、w1~w3
                        break;
                    }
                    else if((w_cnt_i + 1) == pst_net_data_list->w_net_data_number)
                    {
                        return FALSE;
                    }
                }

                b_step = cb_get_continuous_rt_data_number;
                break;
            }
			case cb_get_continuous_rt_data_number:
			{
                w_data_list_segment_start = w_data_list_index;
				w_data_list_segment_number = 0;

                for(;;)
                {
                    if(w_data_list_index < pst_net_data_list->w_net_data_number)
                    {
                        if(NULL == pst_net_data_list->pst_net_data_info[w_data_list_index].pb_data)
                        {
                            w_data_list_index++;
                            break;
                        }
                        else if((pst_net_data_list->
                                pst_net_data_info[w_data_list_index].
                                w_data_address_net < w_start_data_address) ||\
                                pst_net_data_list->
                                pst_net_data_info[w_data_list_index].
                                w_data_address_net > w_end_data_address)
                        {
                            w_data_list_index++;
                            break;
                        }
                        else if((w_data_list_index + 1) == pst_net_data_list->w_net_data_number)
                        {
                            {
                                w_data_list_index++;
                                w_data_list_segment_number++;
                                break;
                            }
                        }
                        else
                        {
                            if((pst_net_data_list->
                                pst_net_data_info[w_data_list_index].
                                w_data_address_net + 1) !=\
                               pst_net_data_list->
                               pst_net_data_info[w_data_list_index + 1].
                               w_data_address_net)
                            {
                                {
                                    w_data_list_index++;
                                    w_data_list_segment_number++;
                                    break;
                                }
                            }
                            else
                            {
                                {
                                    w_data_list_index++;
                                    w_data_list_segment_number++;
                                }
                            }
                        }
                    }
                    else
                    {
                        break;
                    }
                }

                if(w_data_list_segment_number > 0)
				{
					b_step = cb_get_rt_data_header;
				}
                else if(w_data_list_index >= pst_net_data_list->w_net_data_number)
                {
                    b_step = cb_get_rt_data_complete;
                }

				break;
			}
			case cb_get_rt_data_header:
			{
				uint8_t b_result_temp = TRUE;

				b_result_temp &= int_to_bytes_big_endain(&pb_data_out[w_data_len],dw_device_id,3);
				w_data_len += 3;

				b_result_temp &=  int_to_bytes_big_endain(&pb_data_out[w_data_len],
                                                          pst_net_data_list->
														  pst_net_data_info[w_data_list_segment_start].
														  w_data_address_net,
														  2);
				w_data_len += 2;

				b_result_temp &= int_to_bytes_big_endain(&pb_data_out[w_data_len],w_data_list_segment_number,2);
				w_data_len += 2;

				b_step = cb_get_realtime_data;

				if(FALSE == b_result_temp)
				{
					return FALSE;
				}
				break;
			}
			case cb_get_realtime_data:
			{
				for(uint16_t w_cnt_1 = 0;w_cnt_1 < w_data_list_segment_number;w_cnt_1++)
				{
                    uint8_t b_data_len = 0;
					uint8_t b_data_segment_num = 0;
                    uint8_t b_data_sgment_size = 0;

                    b_data_len = pst_net_data_list->
								 pst_net_data_info[w_data_list_segment_start + w_cnt_1].
								 pst_data_attribute->
								 b_len;

                    b_data_sgment_size = pst_net_data_list->
									     pst_net_data_info[w_data_list_segment_start + w_cnt_1].
									     pst_data_attribute->
									     b_size;
					b_data_segment_num = b_data_len / b_data_sgment_size;

					for(uint8_t b_cnt_1 = 0;b_cnt_1 < b_data_segment_num;b_cnt_1++)
					{
						if(FALSE == byte_fill_in_reverse_order(b_data_sgment_size,
															   &pst_net_data_list->
															    pst_net_data_info[w_data_list_segment_start + w_cnt_1].
															    pb_data[b_data_sgment_size * b_cnt_1],
															   &pb_data_out[w_data_len + b_data_sgment_size * b_cnt_1],
															   (w_data_out_buffer_size - w_data_len)))
						{
							return FALSE;
						}
					}

					w_data_len += b_data_len;
				}

				b_step = cb_get_continuous_rt_data_number;
				break;
			}
		}
	}while(b_step != cb_get_rt_data_complete);

    *pw_data_out_len = w_data_len;

	return TRUE;
}
uint8_t net_real_time_data_build(uint8_t b_active_push,
                                 uint16_t *pw_data_out_len,
                                 uint8_t *pb_data_out,
                                 uint16_t w_data_out_buffer_size,
                                 net_system_info_list *pst_info_list)
{
	if((NULL == pw_data_out_len) || (NULL == pb_data_out) || (NULL == pst_info_list))
    {
        return FALSE;
    }

    uint16_t w_data_len_total = 0;
	uint16_t w_data_len = 0;
	uint16_t w_system_list_index = 0;
	uint16_t w_device_list_index = 0;
    uint16_t w_device_data_list_index = 0;

    #define cb_get_system_type                         0
    #define cb_get_local_rt_data                       1
    #define cb_get_system_device_index                 2
    #define cb_get_system_device_data_list_index       3
    #define cb_get_system_device_rt_data               4
	#define cb_build_rt_data_complete		  	       5

    uint8_t b_device_tree_build_step = cb_get_system_type;

    do{
          switch(b_device_tree_build_step)
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
                                  if(pst_info_list->
                                         pst_system_info[w_cnt_1].
                                         pst_device_info_list->
                                         pst_device_info[w_cnt_2].
                                         pst_net_data_list_info->pst_net_data_list[w_cnt_3].b_net_data_typde == cb_net_data_att_status)
                                  {
                                      for(uint16_t w_cnt_4 = 0;
                                          w_cnt_4 <  pst_info_list->
                                                     pst_system_info[w_cnt_1].
                                                     pst_device_info_list->
                                                     pst_device_info[w_cnt_2].
                                                     pst_net_data_list_info->pst_net_data_list[w_cnt_3].w_net_data_number;
                                          w_cnt_4++)
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

				  if(w_data_len_max > w_data_out_buffer_size)
				  {
					  #if (device_tree_print)
                      DebugPrintf(TRUE,"Realtime data build local\r\n");
					  #endif
					  b_device_tree_build_step = cb_get_local_rt_data;
				  }
				  else
				  {
					  w_system_list_index = w_cnt_1;
					  b_device_tree_build_step = cb_get_system_device_index;
				  }
                  break;
              }
              case cb_get_local_rt_data:
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
                          for(uint16_t w_cnt_2 = 0;w_cnt_2 < pst_info_list->
                                                             pst_system_info[w_system_list_index].
                                                             pst_device_info_list->
                                                             pst_device_info[w_cnt_1].
                                                             pst_net_data_list_info->w_net_data_list_length;w_cnt_2++)
                          {
                              if(cb_net_data_att_status == pst_info_list->
                                                           pst_system_info[w_system_list_index].
                                                           pst_device_info_list->
                                                           pst_device_info[w_cnt_1].
                                                           pst_net_data_list_info->
                                                           pst_net_data_list[w_cnt_2].
                                                           b_net_data_typde)
                              {
                                  uint16_t w_data_end_index = 0;


                                  w_data_end_index = pst_info_list->
                                                     pst_system_info[w_system_list_index].
                                                     pst_device_info_list->
                                                     pst_device_info[w_cnt_1].
                                                     pst_net_data_list_info->
                                                     pst_net_data_list[w_cnt_2].
                                                     w_net_data_number - 1;

                                  if(FALSE == net_get_device_real_time_data(b_active_push,
                                                                            *pst_info_list->
                                                                            pst_system_info[w_system_list_index].
                                                                            pst_device_info_list->
                                                                            pst_device_info[w_cnt_1].
                                                                            pdw_device_id,\
                                                                            pst_info_list->
                                                                            pst_system_info[w_system_list_index].
                                                                            pst_device_info_list->
                                                                            pst_device_info[w_cnt_1].
                                                                            pst_net_data_list_info->
                                                                            pst_net_data_list[w_cnt_2].
                                                                            pst_net_data_info[0].
                                                                            w_data_address_net,\
                                                                            pst_info_list->
                                                                            pst_system_info[w_system_list_index].
                                                                            pst_device_info_list->
                                                                            pst_device_info[w_cnt_1].
                                                                            pst_net_data_list_info->
                                                                            pst_net_data_list[w_cnt_2].
                                                                            pst_net_data_info[w_data_end_index].
                                                                            w_data_address_net,\
                                                                            &w_data_len,\
                                                                            &pb_data_out[w_data_len],\
                                                                            w_data_out_buffer_size,\
                                                                            &(pst_info_list->
                                                                              pst_system_info[w_system_list_index].
                                                                              pst_device_info_list->
                                                                              pst_device_info[w_cnt_1].
                                                                              pst_net_data_list_info->
                                                                              pst_net_data_list[w_cnt_2])))
                                  {
                                      return FALSE;
                                  }

                                  w_data_len_total += w_data_len;
                              }
                          }

						  break;
                      }
                  }
                  b_device_tree_build_step = cb_build_rt_data_complete;
                  break;
              }
              case cb_get_system_device_index:
              {
                  for(;;)
                  {
                      if((w_device_list_index + 1) <= pst_info_list->pst_system_info[w_system_list_index].pst_device_info_list->w_length)
                      {
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
                                  b_device_tree_build_step = cb_get_system_device_data_list_index;
                                  break;
                              }
                          }
                      }
                      else
                      {
                          b_device_tree_build_step = cb_build_rt_data_complete;
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
                      if((w_device_data_list_index) < pst_info_list->
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
							  #if (device_tree_print)
                              DebugPrintf(TRUE,"Realtime data build,device:%06X\r\n",*pst_info_list->pst_system_info[w_system_list_index].pst_device_info_list->pst_device_info[w_device_list_index].pdw_device_id);
                              #endif
							  b_device_tree_build_step = cb_get_system_device_rt_data;
                              break;
                          }
                      }
                      else
                      {
                          w_device_data_list_index = 0;
                          w_device_list_index++;
                          b_device_tree_build_step = cb_get_system_device_index;
                          break;
                      }
                  }
                  break;
              }
			  case cb_get_system_device_rt_data:
              {
                  uint16_t w_data_end_index = 0;

                  w_data_end_index = pst_info_list->
                                     pst_system_info[w_system_list_index].
                                     pst_device_info_list->
                                     pst_device_info[w_device_list_index].
                                     pst_net_data_list_info->
                                     pst_net_data_list[w_device_data_list_index].
                                     w_net_data_number - 1;

                  if(FALSE == net_get_device_real_time_data(b_active_push,
                                                            *pst_info_list->
														    pst_system_info[w_system_list_index].
														    pst_device_info_list->
														    pst_device_info[w_device_list_index].
                                                            pdw_device_id,\
                                                            pst_info_list->
														    pst_system_info[w_system_list_index].
														    pst_device_info_list->
														    pst_device_info[w_device_list_index].
                                                            pst_net_data_list_info->
                                                            pst_net_data_list[w_device_data_list_index].
                                                            pst_net_data_info[0].
                                                            w_data_address_net,\
                                                            pst_info_list->
														    pst_system_info[w_system_list_index].
														    pst_device_info_list->
														    pst_device_info[w_device_list_index].
                                                            pst_net_data_list_info->
                                                            pst_net_data_list[w_device_data_list_index].
                                                            pst_net_data_info[w_data_end_index].
                                                            w_data_address_net,\
                                                            &w_data_len,\
													        &pb_data_out[w_data_len_total],\
                                                            w_data_out_buffer_size,\
													        &(pst_info_list->
														      pst_system_info[w_system_list_index].
														      pst_device_info_list->
														      pst_device_info[w_device_list_index].
                                                              pst_net_data_list_info->
                                                              pst_net_data_list[w_device_data_list_index])))
				  {
					  return FALSE;
				  }

                  w_data_len_total += w_data_len;
				  w_device_data_list_index++;
				  b_device_tree_build_step = cb_get_system_device_data_list_index;
                  break;
              }
          }
    }while(b_device_tree_build_step != cb_build_rt_data_complete);

    *pw_data_out_len = w_data_len_total;

	return TRUE;
}
