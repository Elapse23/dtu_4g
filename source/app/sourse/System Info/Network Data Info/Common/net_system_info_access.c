#include "net_system_info_access.h"

net_ip_port st_net_ip_port = {0,};

uint8_t net_inquire_device_communicate_status(uint32_t dw_device_id,uint8_t b_communicate_status,net_system_info_list *pst_system_info_list)
{
	if(NULL == pst_system_info_list)
	{
		return FALSE;
	}

	for(uint16_t w_cnt_1 = 0;w_cnt_1 < pst_system_info_list->w_length;w_cnt_1++)
    {
		if(pst_system_info_list->b_current_type ==\
		   pst_system_info_list->pst_system_info[w_cnt_1].b_type)
		{
			for(uint16_t w_cnt_2 = 0;
			    w_cnt_2 < pst_system_info_list->pst_system_info[w_cnt_1].pst_device_info_list->w_length;
				w_cnt_2++)
			{
				if(dw_device_id ==\
				   *pst_system_info_list->pst_system_info[w_cnt_1].pst_device_info_list->pst_device_info[w_cnt_2].pdw_device_id)
				{
					if(b_communicate_status ==\
					   pst_system_info_list->
					   pst_system_info[w_cnt_1].
					   pst_device_info_list->
				       pst_device_info[w_cnt_2].
					   pst_net_device_status->
					   b_communicate_status)
					{
						return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}
uint8_t net_get_system_info_current(net_system_info ** const pst_system_info,net_system_info_list *pst_system_info_list)
{
	if((NULL == pst_system_info) || (NULL == pst_system_info_list))
	{
		return FALSE;
	}

	for(uint16_t w_cnt_1 = 0;w_cnt_1 < pst_system_info_list->w_length;w_cnt_1++)
	{
		if(pst_system_info_list->b_current_type ==\
		   pst_system_info_list->pst_system_info[w_cnt_1].b_type)
		{
			if(NULL == &pst_system_info_list->pst_system_info[w_cnt_1])
			{
				return FALSE;
			}
			*pst_system_info = &pst_system_info_list->pst_system_info[w_cnt_1];
			return TRUE;
		}
	}

	return FALSE;
}
uint8_t net_get_device_info(uint32_t dw_device_id,
						    net_device_info **pst_net_device_info,
						    net_system_info_list *pst_system_info_list)
{
	uint16_t w_cnt_1 = 0;
	uint16_t w_cnt_2 = 0;

	if((NULL == pst_net_device_info) || (NULL == pst_system_info_list))
	{
		return FALSE;
	}


	for(w_cnt_1 = 0;w_cnt_1 < pst_system_info_list->w_length;w_cnt_1++)
	{
		if(pst_system_info_list->b_current_type ==\
		   pst_system_info_list->pst_system_info[w_cnt_1].b_type)
		{
			for(w_cnt_2 = 0;
				w_cnt_2 < pst_system_info_list->
					      pst_system_info[w_cnt_1].
					      pst_device_info_list->
					      w_length;
				w_cnt_2++)
			{
				if(dw_device_id ==\
				   *pst_system_info_list->
				   pst_system_info[w_cnt_1].
				   pst_device_info_list->
				   pst_device_info[w_cnt_2].
				   pdw_device_id)
				{
				    *pst_net_device_info = &pst_system_info_list->
										    pst_system_info[w_cnt_1].
										    pst_device_info_list->
										    pst_device_info[w_cnt_2];

				   return TRUE;
				}
			}
		}
	}

	return FALSE;
}
uint8_t net_get_device_tree_info(uint32_t dw_device_id,
							     net_device_tree **pst_net_device_tree,
							     net_system_info_list *pst_system_info_list)
{
	uint16_t w_cnt_1 = 0;
	uint16_t w_cnt_2 = 0;

	if((NULL == pst_net_device_tree) || (NULL == pst_system_info_list))
	{
		return FALSE;
	}

	(void)pst_net_device_tree;

	for(w_cnt_1 = 0;w_cnt_1 < pst_system_info_list->w_length;w_cnt_1++)
	{
		if(pst_system_info_list->b_current_type ==\
		   pst_system_info_list->pst_system_info[w_cnt_1].b_type)
		{
			for(w_cnt_2 = 0;
				w_cnt_2 < pst_system_info_list->
					      pst_system_info[w_cnt_1].
					      pst_device_info_list->
					      w_length;
				w_cnt_2++)
			{
				if(dw_device_id ==\
				   *pst_system_info_list->
				   pst_system_info[w_cnt_1].
				   pst_device_info_list->
				   pst_device_info[w_cnt_2].
				   pdw_device_id)
				{
				    *pst_net_device_tree = pst_system_info_list->
										   pst_system_info[w_cnt_1].
										   pst_device_info_list->
										   pst_device_info[w_cnt_2].
										   pst_net_device_tree_info;

				   return TRUE;
				}
			}
		}
	}

	return FALSE;
}
uint8_t net_get_device_tree_relasion(uint32_t dw_device_id,
							         net_device_tree_element_info ** const pst_net_device_tree_element_info,
							         net_system_info_list *pst_system_info_list)
{
	uint16_t w_cnt_1 = 0;
	uint16_t w_cnt_2 = 0;

	if((NULL == pst_net_device_tree_element_info) || (NULL == pst_system_info_list))
	{
		return FALSE;
	}

	for(w_cnt_1 = 0;w_cnt_1 < pst_system_info_list->w_length;w_cnt_1++)
	{
		if(pst_system_info_list->b_current_type ==\
		   pst_system_info_list->pst_system_info[w_cnt_1].b_type)
		{

			for(w_cnt_2 = 0;
				w_cnt_2 < pst_system_info_list->
					      pst_system_info[w_cnt_1].
						  pst_device_tree_element_info_list->
						  b_number;
				w_cnt_2++)
			{
				if(dw_device_id == *pst_system_info_list->
					    		   pst_system_info[w_cnt_1].
								   pst_device_tree_element_info_list->
								   pst_device_tree_element_info[w_cnt_2].
								   pdw_master_device_id)
				{
					*pst_net_device_tree_element_info = &pst_system_info_list->
													     pst_system_info[w_cnt_1].
													     pst_device_tree_element_info_list->
													     pst_device_tree_element_info[w_cnt_2];

					return TRUE;
				}
			}
		}
	}

	return FALSE;
}


uint8_t net_get_device_data_list(uint8_t data_list_type,
								 const net_data_list ** const pst_device_data_list,
								 net_device_info * const pst_net_device_info)
{
	if((NULL == pst_net_device_info) || (NULL == pst_net_device_info->pst_net_data_list_info))
    //if(NULL == pst_net_device_info)
	{
		return FALSE;
	}

	for(uint16_t w_cnt_1 = 0;
		w_cnt_1 < pst_net_device_info->pst_net_data_list_info->w_net_data_list_length;
		w_cnt_1++)
	{
		if(data_list_type ==\
		   pst_net_device_info->
		   pst_net_data_list_info->
		   pst_net_data_list[w_cnt_1].
		   b_net_data_typde)
		{
			*pst_device_data_list = &pst_net_device_info->pst_net_data_list_info->pst_net_data_list[w_cnt_1];
			return TRUE;
		}
	}

	return FALSE;
}

uint8_t net_device_id_convert_2_address(uint8_t b_device_address_type,
                                        uint32_t dw_device_id,
										uint32_t *pdw_device_address,
										net_system_info_list *pst_system_info_list)
{
	uint16_t w_cnt_1 = 0;
	uint16_t w_cnt_2 = 0;

	if((NULL == pdw_device_address) ||\
       (NULL == pst_system_info_list))
	{
		return FALSE;
	}


	for(w_cnt_1 = 0;w_cnt_1 < pst_system_info_list->w_length;w_cnt_1++)
	{
		if(pst_system_info_list->b_current_type ==\
		   pst_system_info_list->pst_system_info[w_cnt_1].b_type)
		{
			for(w_cnt_2 = 0;
				w_cnt_2 < pst_system_info_list->
					      pst_system_info[w_cnt_1].
					      pst_device_info_list->
					      w_length;
				w_cnt_2++)
			{
				if(dw_device_id ==\
				   *pst_system_info_list->
				   pst_system_info[w_cnt_1].
				   pst_device_info_list->
				   pst_device_info[w_cnt_2].
				   pdw_device_id)
				{
                    if(cb_net_get_device_send_address == b_device_address_type)
                    {
                        *pdw_device_address = pst_system_info_list->
                                              pst_system_info[w_cnt_1].
                                              pst_device_info_list->
                                              pst_device_info[w_cnt_2].
                                              dw_device_address_send;
                    }
                    else if(cb_net_get_device_receive_address == b_device_address_type)
                    {
                        *pdw_device_address = pst_system_info_list->
                                              pst_system_info[w_cnt_1].
                                              pst_device_info_list->
                                              pst_device_info[w_cnt_2].
                                              dw_device_address_receive;
                    }

					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

uint8_t net_device_address_convert_2_id(uint32_t dw_device_address,
									    uint32_t *pdw_device_id,
									    net_system_info_list *pst_system_info_list)
{
	uint16_t w_cnt_1 = 0;
	uint16_t w_cnt_2 = 0;

	if((NULL == pdw_device_id) ||\
       (NULL == pst_system_info_list))
	{
		return FALSE;
	}

	for(w_cnt_1 = 0;w_cnt_1 < pst_system_info_list->w_length;w_cnt_1++)
	{
		if(pst_system_info_list->b_current_type ==\
		   pst_system_info_list->pst_system_info[w_cnt_1].b_type)
		{
			for(w_cnt_2 = 0;
				w_cnt_2 < pst_system_info_list->
					      pst_system_info[w_cnt_1].
					      pst_device_info_list->
					      w_length;
				w_cnt_2++)
			{
				if((dw_device_address ==\
				    pst_system_info_list->
				    pst_system_info[w_cnt_1].
				    pst_device_info_list->
				    pst_device_info[w_cnt_2].
				    dw_device_address_send) ||
                   (dw_device_address ==\
				    pst_system_info_list->
				    pst_system_info[w_cnt_1].
				    pst_device_info_list->
				    pst_device_info[w_cnt_2].
				    dw_device_address_receive))
				{
					*pdw_device_id = pst_system_info_list->
								     pst_system_info[w_cnt_1].
								     pst_device_info_list->
								     pst_device_info[w_cnt_2].
								     dw_device_address_send;
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}
uint8_t net_get_device_data_attribute(uint32_t dw_device_id,
						              uint8_t b_net_data_type,
						              uint16_t w_data_addr,
						              net_data_attribute ** const pst_net_data_attribute,
						              net_system_info_list *pst_system_info_list)
{
	if((NULL == pst_net_data_attribute) || (NULL == pst_system_info_list))
	{
		return FALSE;
	}

	uint16_t w_cnt_1 = 0;
	uint16_t w_cnt_2 = 0;
	uint16_t w_cnt_3 = 0;
	uint16_t w_cnt_4 = 0;

	for(w_cnt_1 = 0;w_cnt_1 < pst_system_info_list->w_length;w_cnt_1++)
	{
		if(pst_system_info_list->b_current_type ==\
		   pst_system_info_list->pst_system_info[w_cnt_1].b_type)
		{
			for(w_cnt_2 = 0;
				w_cnt_2 < pst_system_info_list->
					      pst_system_info[w_cnt_1].
					      pst_device_info_list->
					      w_length;
				w_cnt_2++)
			{
				if(dw_device_id ==\
				   *pst_system_info_list->
				   pst_system_info[w_cnt_1].
				   pst_device_info_list->
				   pst_device_info[w_cnt_2].
				   pdw_device_id)
				{
					for(w_cnt_3 = 0;
						w_cnt_3 < pst_system_info_list->
								  pst_system_info[w_cnt_1].
								  pst_device_info_list->
								  pst_device_info[w_cnt_2].
								  pst_net_data_list_info->
								  w_net_data_list_length;
						w_cnt_3++)
					{
						if(b_net_data_type ==\
						   pst_system_info_list->
						   pst_system_info[w_cnt_1].
						   pst_device_info_list->
						   pst_device_info[w_cnt_2].
						   pst_net_data_list_info->
						   pst_net_data_list[w_cnt_3].
						   b_net_data_typde)
						{
							for(w_cnt_4 = 0;
								w_cnt_4 < pst_system_info_list->
										  pst_system_info[w_cnt_1].
										  pst_device_info_list->
										  pst_device_info[w_cnt_2].
										  pst_net_data_list_info->
										  pst_net_data_list[w_cnt_3].
										  w_net_data_number;
								w_cnt_4++)
							{
								if(w_data_addr ==\
								   pst_system_info_list->
								   pst_system_info[w_cnt_1].
								   pst_device_info_list->
								   pst_device_info[w_cnt_2].
								   pst_net_data_list_info->
								   pst_net_data_list[w_cnt_3].
								   pst_net_data_info[w_cnt_4].
								   w_data_address_net)
								{
									*pst_net_data_attribute = (net_data_attribute *)pst_system_info_list->
															  						pst_system_info[w_cnt_1].
															  						pst_device_info_list->
															  						pst_device_info[w_cnt_2].
															  						pst_net_data_list_info->
															  						pst_net_data_list[w_cnt_3].
															  						pst_net_data_info[w_cnt_4].
															  						pst_data_attribute;
									return TRUE;
								}
							}
						}
					}
				}
			}
		}
	}

	return FALSE;
}
uint8_t net_get_device_data_info(uint8_t b_net_data_type,
						         uint16_t w_data_addr,
						         const net_data_info ** const pst_net_data_info,
						         net_device_info * const pst_net_device_info)
{
	if((NULL == pst_net_data_info) || (NULL == pst_net_device_info))
	{
		return FALSE;
	}

	uint16_t w_cnt_1 = 0;
	uint16_t w_cnt_2 = 0;

	for(w_cnt_1 = 0;w_cnt_1 < pst_net_device_info->pst_net_data_list_info->w_net_data_list_length;w_cnt_1++)
	{
		if(b_net_data_type == pst_net_device_info->pst_net_data_list_info->pst_net_data_list[w_cnt_1].b_net_data_typde)
		{
			for(w_cnt_2 = 0;w_cnt_2 < pst_net_device_info->pst_net_data_list_info->pst_net_data_list[w_cnt_1].w_net_data_number;w_cnt_2++)
			{
				if(w_data_addr == pst_net_device_info->
				   				  pst_net_data_list_info->
				   				  pst_net_data_list[w_cnt_1].
				   				  pst_net_data_info[w_cnt_2].
				   				  w_data_address_net)
				{
					*pst_net_data_info = &pst_net_device_info->
										  pst_net_data_list_info->
										  pst_net_data_list[w_cnt_1].
										  pst_net_data_info[w_cnt_2];
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}
