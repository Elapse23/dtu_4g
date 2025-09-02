#include "net_system_info.h"
#include "net_system_info_access.h"
#include "net_device_tree_build.h"
#include "BootloaderHead.H"
#include "printf_custom.h"
#include "application_general_func.h"
#include "network_local_device_secquence_operate.h"
#include "FREERTOS.h"
#include "semphr.h"
#include "freertos_user_config.h"
#include <ysizet.h>
#include <string.h>
#include "net_device_tree_kinergy_ii.h"
#include "net_device_tree_system.h"
#include "net_system_info_list.h"
#include "net_device_data_access.h"
#include "network_system_type_operate.h"
#include "network_tbb_device_server_communicate.h"
#include "network_protocal_data_assembly.h"
#include "network_local_firmware_update.h"

extern void * pvPortMalloc( size_t xWantedSize );
extern void vPortFree( void * pv );

SemaphoreHandle_t  pst_net_device_tree_mutex = NULL;
SemaphoreHandle_t  pst_net_device_exists_state_mutex = NULL;
static uint8_t b_reconnet_server = FALSE;
static TickType_t xTicksToWait = 0;
uint16_t gw_agent_facturer = 0;

static void net_build_device_tree_recur(uint32_t dw_master_device_id,
                          	            uint16_t *pw_data_out_len,
                          	            uint8_t *pb_data_out,
                          	            net_system_info_list *pst_info_list);
static uint8_t subset_device_firmware_version_fill(uint32_t dw_device_id,
											       uint16_t w_data_in_len,
											       uint8_t *pb_data_in,
											       net_system_info_list *pst_info_list);
void device_num_statistics(uint32_t dw_device_id,uint8_t f_device_add);
uint8_t clear_all_subset(uint32_t dw_device_id,
                         net_system_info_list *pst_info_list);

/*RESERVE*/
//static uint8_t subset_device_sn_fill(uint32_t dw_device_id,
//							         uint16_t w_data_in_len,
//							         uint8_t *pb_data_in,
//							         net_system_info_list *pst_info_list);


void net_device_tree_refresh_Semaphore_creat()
{
    pst_net_device_tree_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_net_device_tree_mutex)
    {

    }
}

void net_device_exists_state_Semaphore_creat()
{
    pst_net_device_exists_state_mutex = xSemaphoreCreateMutex();
    if(NULL == pst_net_device_exists_state_mutex)
    {

    }
}

//delete->add->delete
//delete->add->loss->add->delete
uint8_t net_device_exists_status_refresh(uint32_t dw_device_address,uint8_t b_device_status,net_system_info_list *pst_info_list)
{
	if(NULL == pst_info_list)
	{
		return FALSE;
	}

	#define	cb_get_system_type					0
	#define cb_get_device_type					1
	#define	cb_refresh_device_status			2
	#define cb_failed							3
	#define cb_success							4
	uint8_t b_step = cb_get_system_type;

	uint16_t w_system_index = 0;
	uint16_t w_device_index = 0;

	do{
	    switch(b_step)
	    {
	    	case cb_get_system_type:
				{
					uint16_t w_cnt_1 = 0;

					for(w_cnt_1 = 0;w_cnt_1 < pst_info_list->w_length;w_cnt_1++)
					{
						if(pst_info_list->b_current_type ==\
						   pst_info_list->pst_system_info[w_cnt_1].b_type)
						{
							w_system_index = w_cnt_1;
							b_step = cb_get_device_type;
							break;
						}
					}

					if(w_cnt_1 >= pst_info_list->w_length)
					{
						b_step = cb_failed;
					}
				}
	    		break;

	    	case cb_get_device_type:
				{
					uint16_t w_cnt_1 = 0;

					for(w_cnt_1 = 0;
						w_cnt_1 < pst_info_list->pst_system_info[w_system_index].pst_device_info_list->w_length;
						w_cnt_1++)
					{
						if(dw_device_address ==\
						   *pst_info_list->pst_system_info[w_system_index].pst_device_info_list->pst_device_info[w_cnt_1].pdw_device_id)
						{
							w_device_index = w_cnt_1;
							b_step = cb_refresh_device_status;
							break;
						}
					}

					if(w_cnt_1 >= pst_info_list->pst_system_info[w_system_index].pst_device_info_list->w_length)
					{
						b_step = cb_failed;
					}
				}
	    		break;

	    	case cb_refresh_device_status:
	    		switch(b_device_status)
	    		{
	    			case cb_net_device_delete:
						if (pst_info_list->
						pst_system_info[w_system_index].
						pst_device_info_list->
						pst_device_info[w_device_index].
						pst_net_device_status->
						b_communicate_status != cb_net_device_delete)
						{
							b_reconnet_server = TRUE;
						}

						pst_info_list->
						pst_system_info[w_system_index].
						pst_device_info_list->
						pst_device_info[w_device_index].
						pst_net_device_status->
						b_communicate_status = cb_net_device_delete;
						b_step = cb_success;
	    				break;
	    			case cb_net_device_add:
						if (pst_info_list->
						pst_system_info[w_system_index].
						pst_device_info_list->
						pst_device_info[w_device_index].
						pst_net_device_status->
						b_communicate_status != cb_net_device_add)
						{
							b_reconnet_server = TRUE;
						}

						pst_info_list->
						pst_system_info[w_system_index].
						pst_device_info_list->
						pst_device_info[w_device_index].
						pst_net_device_status->
						b_communicate_status = cb_net_device_add;

                        pst_info_list->
                        pst_system_info[w_system_index].
                        pst_device_info_list->
                        pst_device_info[w_device_index].
                        pst_net_device_status->
                        b_communicate_timeout_times = 0;

	    				b_step = cb_success;
	    				break;
	    			case cb_net_device_loss:
                        if(pst_info_list->
						   pst_system_info[w_system_index].
						   pst_device_info_list->
						   pst_device_info[w_device_index].
						   pst_net_device_status->
						   b_communicate_status != cb_net_device_delete)
                        {
                           pst_info_list->
						   pst_system_info[w_system_index].
						   pst_device_info_list->
						   pst_device_info[w_device_index].
						   pst_net_device_status->
                           b_communicate_timeout_times++;

                           if(pst_info_list->
                              pst_system_info[w_system_index].
                              pst_device_info_list->
                              pst_device_info[w_device_index].
                              pst_net_device_status->
                              b_communicate_timeout_times >=
                                pst_info_list->
                                  pst_system_info[w_system_index].
                                  pst_device_info_list->
                                  pst_device_info[w_device_index].
                                  pst_net_device_status->
                                  b_communicate_max_timeout_times)
                           {
								if (pst_info_list->
								pst_system_info[w_system_index].
								pst_device_info_list->
								pst_device_info[w_device_index].
								pst_net_device_status->
								b_communicate_status != cb_net_device_loss)
								{
									b_reconnet_server = TRUE;
								}
                               pst_info_list->
                               pst_system_info[w_system_index].
                               pst_device_info_list->
                               pst_device_info[w_device_index].
                               pst_net_device_status->
                               b_communicate_timeout_times = 0;

                               pst_info_list->
                               pst_system_info[w_system_index].
                               pst_device_info_list->
                               pst_device_info[w_device_index].
                               pst_net_device_status->
                               b_communicate_status = cb_net_device_loss;
                           }
                        }
						b_step = cb_success;
	    				break;
					default:
						b_step = cb_failed;
						break;
	    		}
	    		break;
	    }
	}while((b_step != cb_failed) && (b_step != cb_success));

	return TRUE;
}

uint8_t net_check_device_communicate_status(uint8_t b_communicate_status,net_device_info * const pst_net_device_info)
{
	if(NULL == pst_net_device_info)
	{
		return FALSE;
	}
		TaskStatus_t taskStatus;
		vTaskGetInfo(NULL, &taskStatus, pdTRUE, eRunning);

    if(pst_net_device_exists_state_mutex != 0)
    {
        #if(net_check_device_communicate_status_Semaphore_test)
        DebugPrintf(TRUE,"[device_communicate]device_exists_state_SemaphoreTake; Function called by task: %s\r\n", taskStatus.pcTaskName);
        #endif

        xSemaphoreTake(pst_net_device_exists_state_mutex, portMAX_DELAY);
        #if(net_check_device_communicate_status_Semaphore_test)
        DebugPrintf(TRUE,"[device_communicate]device_exists_state_SemaphoreTakeSuccess; Function called by task: %s\r\n", taskStatus.pcTaskName);
        #endif
    }

	if((b_communicate_status & pst_net_device_info->pst_net_device_status->b_communicate_status) != 0)
	{
		if(pst_net_device_exists_state_mutex != 0)
		{
			#if(net_check_device_communicate_status_Semaphore_test)
			DebugPrintf(TRUE,"[device_communicate]device_exists_state_SemaphoreGive; Function called by task: %s\r\n", taskStatus.pcTaskName);
			#endif
			xSemaphoreGive(pst_net_device_exists_state_mutex);
		}

		return TRUE;
	}

    if(pst_net_device_exists_state_mutex != 0)
    {
        #if(net_check_device_communicate_status_Semaphore_test)
        DebugPrintf(TRUE,"[device_communicate]device_exists_state_SemaphoreGive; Function called by task: %s\r\n", taskStatus.pcTaskName);
        #endif
        xSemaphoreGive(pst_net_device_exists_state_mutex);
    }

	return FALSE;
}

uint8_t net_device_tree_build(uint16_t *pw_data_out_len,
                              uint8_t *pb_data_out,
                              uint16_t w_data_out_buffer_max_len,
                              net_system_info_list *pst_info_list)
{
    if((NULL == pw_data_out_len) || (NULL == pb_data_out) || (NULL == pst_info_list))
    {
        return FALSE;
    }

	if(pst_net_device_tree_mutex != 0)
    {
        #if(net_device_tree_build_Semaphore_test)
        DebugPrintf(TRUE,"build_device_tree_SemaphoreTake\n");
        #endif
        xSemaphoreTake(pst_net_device_tree_mutex, portMAX_DELAY);
    }

	uint16_t w_data_out_len = 0;

	#if (device_tree_print)
    DebugPrintf(TRUE,"Device tree build start\r\n");
	#endif

    #define cb_get_system_type                      0
    #define cb_count_device_tree_total_length       1
    #define cb_get_local_device_tree_only           2
    #define cb_get_system_device_tree               3
    #define cb_device_tree_get_complete             4

    uint8_t b_device_tree_build_step = cb_get_system_type;

    do{
          switch(b_device_tree_build_step)
          {
              case cb_get_system_type:
                  for(uint16_t w_cnt_1 = 0;w_cnt_1 < pst_info_list->w_length;w_cnt_1++)
                  {
                      if(pst_info_list->pst_system_info[w_cnt_1].b_type ==\
                         pst_info_list->b_current_type)
                      {
                          if(pst_info_list->pst_system_info[w_cnt_1].pst_device_info_list->w_length * cb_net_device_tree_length >\
                             w_data_out_buffer_max_len)
                          {
                              b_device_tree_build_step = cb_get_local_device_tree_only;
                          }
                          else
                          {
                              b_device_tree_build_step = cb_get_system_device_tree;
                          }

						  break;
                      }
					  else if((w_cnt_1 + 1) >= pst_info_list->w_length)
					  {
						  b_device_tree_build_step = cb_get_local_device_tree_only;

						  break;
					  }
                  }
                  break;

              case cb_get_local_device_tree_only:
			  {
				  net_device_info *pst_net_device_info_local = 0;

				  {//system
					  for(uint16_t w_cnt_1 = 0;w_cnt_1 < pst_info_list->pst_system_info->pst_device_info_list->w_length;w_cnt_1++)
					  {
						  if(*pst_info_list->\
							 pst_system_info->\
							 pst_device_tree_element_info_list->\
							 pst_device_tree_element_info[cb_net_device_tree_info_system_index].\
							 pdw_master_device_id ==\
							 *pst_info_list->pst_system_info->pst_device_info_list->pst_device_info[w_cnt_1].pdw_device_id)
						  {
							  for(uint16_t w_cnt_2 = 0;w_cnt_2 < cb_net_device_tree_length;w_cnt_2++)
							  {
								  pb_data_out[w_cnt_2] = \
                                      pst_info_list->
                                          pst_system_info->
                                          pst_device_info_list->
                                          pst_device_info[w_cnt_1].
                                          pst_net_device_tree_info->
                                          b_raw_data[w_cnt_2];
							  }
							  #if (device_tree_print)
                              DebugPrintf(TRUE,"Device Tree Build system:%06X\r\n",*pst_info_list->pst_system_info->pst_device_info_list->pst_device_info[w_cnt_1].pdw_device_id);
							  #endif
							  pst_net_device_info_local = &pst_info_list->pst_system_info->pst_device_info_list->pst_device_info[w_cnt_1];
							  w_data_out_len = cb_net_device_tree_length;
							  break;
						  }
					  }
					  pst_net_device_info_local->pst_net_device_tree_info->st_device_tree.b_subset_num = 1;
				  }


				  {//master
					  for(uint16_t w_cnt_1 = 0;w_cnt_1 < pst_info_list->pst_system_info->pst_device_info_list->w_length;w_cnt_1++)
					  {
						  if(*pst_info_list->\
							 pst_system_info->\
							 pst_device_tree_element_info_list->\
							 pst_device_tree_element_info[cb_net_device_tree_info_master_index].\
							 pdw_master_device_id ==\
							 *pst_info_list->pst_system_info->pst_device_info_list->pst_device_info[w_cnt_1].pdw_device_id)
						  {
							  for(uint16_t w_cnt_2 = 0;w_cnt_2 < cb_net_device_tree_length;w_cnt_2++)
							  {
								  pb_data_out[w_cnt_2 + cb_net_device_tree_length] = \
                                        pst_info_list->
                                            pst_system_info->
                                            pst_device_info_list->
                                            pst_device_info[w_cnt_1].
                                            pst_net_device_tree_info->
                                            b_raw_data[w_cnt_2];
							  }
							  #if (device_tree_print)
                              DebugPrintf(TRUE,"Device Tree Build master:%06X\r\n",*pst_info_list->pst_system_info->pst_device_info_list->pst_device_info[w_cnt_1].pdw_device_id);
							  #endif
							  pst_net_device_info_local = &pst_info_list->pst_system_info->pst_device_info_list->pst_device_info[w_cnt_1];
							  w_data_out_len += cb_net_device_tree_length;
							  break;
						  }
					  }

					  for(uint16_t w_cnt_1 = 0;w_cnt_1 < pst_info_list->\
														 pst_system_info->\
														 pst_device_tree_element_info_list->\
														 pst_device_tree_element_info->
														 st_subset_device_id.b_number;w_cnt_1++)
					  {
						  for(uint16_t w_cnt_2 = 0;w_cnt_2 < pst_info_list->pst_system_info->pst_device_info_list->w_length;w_cnt_2++)
						  {
							  if(*pst_info_list->\
								 pst_system_info->\
								 pst_device_tree_element_info_list->\
								 pst_device_tree_element_info->
								 st_subset_device_id.pdw_subset_id_list[w_cnt_1] ==\
								 *pst_info_list->pst_system_info->pst_device_info_list->pst_device_info[w_cnt_2].pdw_device_id)
							  {
								  if(cb_net_device_add ==
									 pst_info_list->
									 pst_system_info->
									 pst_device_info_list->
									 pst_device_info[w_cnt_2].
									 pst_net_device_status->
									 b_communicate_status)
								  {
									  pst_net_device_info_local->pst_net_device_tree_info->st_device_tree.b_subset_num++;
								  }
							  }
						  }
					  }
				  }
                  b_device_tree_build_step = cb_device_tree_get_complete;
                  break;
			  }
              case cb_get_system_device_tree:
                  net_build_device_tree_recur(*pst_info_list->\
					     		              pst_system_info->\
						 		              pst_device_tree_element_info_list->\
						 		              pst_device_tree_element_info->\
					     		              pdw_master_device_id,\
									          &w_data_out_len,\
								              pb_data_out,\
									          pst_info_list);
				  b_device_tree_build_step = cb_device_tree_get_complete;
                  break;
          }
		  {
		      vTaskDelay(0);
		  }
    }while(b_device_tree_build_step != cb_device_tree_get_complete);

	*pw_data_out_len = w_data_out_len;

	#if (device_tree_print)
    DebugPrintf(TRUE,"Device tree build end\r\n");
	#endif

	if(pst_net_device_tree_mutex != 0)
    {
        #if(net_device_tree_build_Semaphore_test)
        DebugPrintf(TRUE,"build_device_tree_SemaphoreGive\n");
        #endif
        xSemaphoreGive(pst_net_device_tree_mutex);
    }

	return TRUE;
}

void net_build_device_tree_recur(uint32_t dw_master_device_id,
                                 uint16_t *pw_data_out_len,
                                 uint8_t *pb_data_out,
                                 net_system_info_list *pst_info_list)
{
	uint8_t b_subset_num = 0;
	uint16_t w_subset_num_byte_index = 0;
    uint8_t b_system_index = 0;

	if((NULL == pw_data_out_len) || (NULL == pb_data_out) || (NULL  == pst_info_list))
	{
		return;
	}

    for(b_system_index = 0;b_system_index < pst_info_list->w_length;b_system_index++)
    {
        if(pst_info_list->b_current_type == pst_info_list->pst_system_info[b_system_index].b_type)
        {
            break;
        }
    }
    if(b_system_index >= pst_info_list->w_length)
    {
        return;
    }
    for(uint16_t w_cnt_1 = 0;w_cnt_1 < pst_info_list->pst_system_info[b_system_index].pst_device_info_list->w_length;w_cnt_1++)
    {
        if((dw_master_device_id ==\
                *pst_info_list->pst_system_info[b_system_index].pst_device_info_list->pst_device_info[w_cnt_1].pdw_device_id) &&\
           ((cb_net_device_add ==\
                pst_info_list->pst_system_info->pst_device_info_list->pst_device_info[w_cnt_1].pst_net_device_status->b_communicate_status) ||\
            (TRUE ==\
                pst_info_list->pst_system_info->pst_device_info_list->pst_device_info[w_cnt_1].pst_net_device_status->b_virtual_device)))
        {
            {
                {
                    if(NULL == pst_info_list->
                                pst_system_info[b_system_index].
                                pst_device_info_list->
                                pst_device_info[w_cnt_1].
                                pst_net_device_tree_info)
                    {
                        continue;
                    }
					#if (device_tree_print)
						DebugPrintf(TRUE,"Device Tree Build:%06X\r\n",*pst_info_list->pst_system_info[b_system_index].pst_device_info_list->pst_device_info[w_cnt_1].pdw_device_id);
						DebugPrintf(TRUE,"Device Tree's ");
					#endif

                    for(uint16_t w_cnt_2 = 0;w_cnt_2 < cb_net_device_tree_length;w_cnt_2++)
                    {
                        pb_data_out[*pw_data_out_len + w_cnt_2] = \
                            pst_info_list->
                                pst_system_info[b_system_index].
                                pst_device_info_list->
                                pst_device_info[w_cnt_1].
                                pst_net_device_tree_info->
                                b_raw_data[w_cnt_2];
						#if (device_tree_print)
						DebugPrintf(TRUE,"%02X ",pst_info_list->pst_system_info[b_system_index].pst_device_info_list->pst_device_info[w_cnt_1].pst_net_device_tree_info->b_raw_data[w_cnt_2]);
						#endif
                    }
					#if (device_tree_print)
					DebugPrintf(TRUE,"\r\n");
					#endif

					*pw_data_out_len += cb_net_device_tree_length;
                    w_subset_num_byte_index = *pw_data_out_len - 1;
                    for(uint16_t w_cnt_3 = 0;w_cnt_3 < pst_info_list->pst_system_info[b_system_index].pst_device_tree_element_info_list->b_number;w_cnt_3++)
                    {
                        if(dw_master_device_id !=\
                            *pst_info_list->pst_system_info[b_system_index].
                                      pst_device_tree_element_info_list->
                                      pst_device_tree_element_info[w_cnt_3].pdw_master_device_id)
                        {
                            continue;
                        }
                        for(uint16_t w_cnt_2 = 0;
                            w_cnt_2 < pst_info_list->pst_system_info[b_system_index].\
                                      pst_device_tree_element_info_list->\
                                      pst_device_tree_element_info[w_cnt_3].
                                      st_subset_device_id.
                                      b_number;
                            w_cnt_2++)
                        {
                            net_device_info *pst_net_device_info = 0;

                            if(FALSE == net_get_device_info(*pst_info_list->pst_system_info[b_system_index].
                                                            pst_device_tree_element_info_list->
                                                            pst_device_tree_element_info[w_cnt_3].
                                                            st_subset_device_id.
                                                            pdw_subset_id_list[w_cnt_2],\
                                                            (net_device_info **)&pst_net_device_info,\
                                                            pst_info_list))
                            {
                                break;
                            }


                            if((TRUE == net_check_device_communicate_status(cb_net_device_add,pst_net_device_info)) ||\
                               (TRUE == pst_net_device_info->pst_net_device_status->b_virtual_device))
                             {
                                 b_subset_num++;
                                 net_build_device_tree_recur(*pst_info_list->pst_system_info[b_system_index].\
                                                             pst_device_tree_element_info_list->\
                                                             pst_device_tree_element_info[w_cnt_3].
                                                             st_subset_device_id.
                                                             pdw_subset_id_list[w_cnt_2],
                                                             pw_data_out_len,
                                                             pb_data_out,
                                                             pst_info_list);
                             }
                        }
                        pb_data_out[w_subset_num_byte_index] = b_subset_num;
//                        break;
                    }
                }
            }
			break;
        }
		else if((w_cnt_1 + 1) >=\
			    pst_info_list->pst_system_info[b_system_index].pst_device_info_list->w_length)
		{
			break;
		}
    }
    /*
    for(uint16_t w_cnt_1 = 0;w_cnt_1 < pst_info_list->pst_system_info[b_system_index].pst_device_tree_element_info_list->b_number;w_cnt_1++)
    {
        if((dw_master_device_id ==\
		pst_info_list->
		pst_system_info[b_system_index].
		pst_device_tree_element_info_list->
		pst_device_tree_element_info[w_cnt_1].
		dw_master_device_id) &&\
           (cb_net_device_add ==
              pst_info_list->
              pst_system_info->
              pst_device_info_list->
              pst_device_info[w_cnt_1].
              pst_net_device_status->
              b_communicate_status))
        {
            for(uint16_t w_cnt_2 = 0;
                w_cnt_2 < pst_info_list->pst_system_info[b_system_index].\
                          pst_device_tree_element_info_list->\
                          pst_device_tree_element_info[w_cnt_1].
						  st_subset_device_id.
						  b_number;
                w_cnt_2++)
            {
				net_device_info *pst_net_device_info = 0;

				if(FALSE == net_get_device_info(pst_info_list->pst_system_info[b_system_index].
												pst_device_tree_element_info_list->
												pst_device_tree_element_info[w_cnt_1].
												st_subset_device_id.
												bdw_subset_id_list[w_cnt_2],\
						    					(net_device_info **)&pst_net_device_info,\
						    					pst_info_list))
				{
					break;
				}


				if((TRUE == net_check_device_communicate_status(cb_net_device_add,pst_net_device_info)) ||\
				   (TRUE == pst_net_device_info->pst_net_device_status->b_virtual_device))
				 {
					 b_subset_num++;
					 net_build_device_tree_recur(pst_info_list->pst_system_info[b_system_index].\
									             pst_device_tree_element_info_list->\
									             pst_device_tree_element_info[w_cnt_1].
											     st_subset_device_id.
												 bdw_subset_id_list[w_cnt_2],
									             pw_data_out_len,
									             pb_data_out,
									             pst_info_list);
				 }
            }
            pb_data_out[w_subset_num_byte_index] = b_subset_num;
            break;
        }
    }
*/
}
								 
extern const INT8U bBootFirmwareStr[2];
uint8_t get_local_firmware_version(uint8_t *pb_data_out,uint16_t w_buffer_size)
{
    if(NULL == pb_data_out)
    {
        return FALSE;
    }

    if(w_buffer_size < 30)
    {
        return FALSE;
    }
    uint8_t b_data_cnt = 0;

    pb_data_out[b_data_cnt++] = 'U';
    pb_data_out[b_data_cnt++] = 'p';
    pb_data_out[b_data_cnt++] = 's';
    pb_data_out[b_data_cnt++] = 'i';
    pb_data_out[b_data_cnt++] = 'l';
    pb_data_out[b_data_cnt++] = 'o';
    pb_data_out[b_data_cnt++] = 'n';
    pb_data_out[b_data_cnt++] = (uint8_t)(cdwFirmwareType2 /  100000) + 0x30;
    pb_data_out[b_data_cnt++] = (uint8_t)((cdwFirmwareType2 / 10000) % 10) + 0x30;
    pb_data_out[b_data_cnt++] = (uint8_t)((cdwFirmwareType2 / 1000) % 10) + 0x30;
    pb_data_out[b_data_cnt++] = (uint8_t)((cdwFirmwareType2 / 100) % 10) + 0x30;
    pb_data_out[b_data_cnt++] = ' ';
    pb_data_out[b_data_cnt++] = (uint8_t)((cdwFirmwareType2 % 100) / 10) + 0x30;
    pb_data_out[b_data_cnt++] = (uint8_t)(cdwFirmwareType2 % 10) + 0x30;
    pb_data_out[b_data_cnt++] = '.';
    pb_data_out[b_data_cnt++] = (uint8_t)(cbAppVersion1 / 10) + 0x30;
    pb_data_out[b_data_cnt++] = (uint8_t)(cbAppVersion1 % 10) + 0X30;
    pb_data_out[b_data_cnt++] = '.';
    pb_data_out[b_data_cnt++] = (uint8_t)(cwAppVersion2 / 1000) + 0x30;
    pb_data_out[b_data_cnt++] = (uint8_t)((cwAppVersion2 / 100) % 10) + 0x30;
    pb_data_out[b_data_cnt++] = (uint8_t)((cwAppVersion2 / 10) % 10) + 0x30;
    pb_data_out[b_data_cnt++] = (uint8_t)(cwAppVersion2 % 10) + 0x30;
    pb_data_out[b_data_cnt++] = ' ';
    pb_data_out[b_data_cnt++] = 'b';
    pb_data_out[b_data_cnt++] = (uint8_t)(bBootFirmwareStr[0]  / 10) + 0x30;
    pb_data_out[b_data_cnt++] = (uint8_t)(bBootFirmwareStr[0]  % 10) + 0X30;
    pb_data_out[b_data_cnt++] = '.';
    pb_data_out[b_data_cnt++] = (uint8_t)(bBootFirmwareStr[1] / 10) + 0x30;
    pb_data_out[b_data_cnt++] = (uint8_t)(bBootFirmwareStr[1] % 10) + 0X30;

    for(uint8_t b_cnt_i = b_data_cnt;b_cnt_i < 30;b_cnt_i++)
    {
        pb_data_out[b_cnt_i] = '\0';
    }

    return TRUE;
}

uint8_t f_reconnect_server = FALSE;  // 手动重连服务器标志位
void net_inquire_device_tree(net_system_info_list *pst_info_list)
{
	if(NULL == pst_info_list)
	{
		return;
	}

	if (f_reconnect_server == TRUE)
	{
		f_reconnect_server = FALSE;
		network_com_with_tbb_device_server_state_set(cb_net_com_state_type_err_code,net_sever_connect);
	}
	TaskStatus_t taskStatus;
	vTaskGetInfo(NULL, &taskStatus, pdTRUE, eRunning);

	if(pst_net_device_tree_mutex != 0)
    {
        xSemaphoreTake(pst_net_device_tree_mutex, portMAX_DELAY);
    }

    if(pst_net_device_exists_state_mutex != 0)
    {
        xSemaphoreTake(pst_net_device_exists_state_mutex, portMAX_DELAY);
    }

	#define cb_get_system_type							0
	#define cb_get_master_index						    1
	#define cb_get_subset_index						    2
	#define cb_inquire_subset_device_tree				3
	#define cb_receive_subset_device_tree				4
    #define cb_build_master_device_tree                 5
	#define cb_device_tree_build_complete			    6

	uint8_t b_step = cb_get_system_type;

	uint16_t w_system_index = 0;
	uint16_t w_inquire_data_buffer_size = 0;
	uint8_t *pb_inquire_data = 0;

    uint8_t b_master_index = 0;
    uint8_t b_subset_index = 0;

    uint8_t b_master_subset_number = 0;


	uint8_t b_send_data_cnt = 0;
    net_device_info *pst_net_device_info = 0;

	do
	{
		switch(b_step)
		{
			case cb_get_system_type:
                {
                    for(uint16_t w_cnt_1 = 0;w_cnt_1 < pst_info_list->w_length;w_cnt_1++)
                    {
                        if(pst_info_list->b_current_type ==\
                           pst_info_list->pst_system_info[w_cnt_1].b_type)
                        {
                            w_system_index = w_cnt_1;

                            b_step = cb_get_master_index;
                            break;
                        }
                        else if((w_cnt_1 + 1) == pst_info_list->w_length)
                        {
                            if(pst_net_device_tree_mutex != 0)
                            {
                                xSemaphoreGive(pst_net_device_tree_mutex);
                            }

                            if(pst_net_device_exists_state_mutex != 0)
                            {
                                xSemaphoreGive(pst_net_device_exists_state_mutex);
                            }
                            return;
                        }
                    }
                }break;
			case cb_get_master_index:
                {
                    for(uint8_t b_cnt_1 = 0;
                        b_cnt_1 < pst_info_list->pst_system_info[w_system_index].pst_device_tree_element_info_list->b_number;
                        b_cnt_1++)
                    {
                        if(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy_ii] == *pst_info_list->
                                                      pst_system_info[w_system_index].
                                                      pst_device_tree_element_info_list->
                                                      pst_device_tree_element_info[b_cnt_1].
                                                      pdw_master_device_id)
                        {
                            b_master_index = b_cnt_1;
                            b_step = cb_get_subset_index;
                            break;
                        }
                        else if((b_cnt_1 + 1) == pst_info_list->pst_system_info[w_system_index].pst_device_tree_element_info_list->b_number)
                        {
                            b_step = cb_build_master_device_tree;
                            break;
                        }
                    }
                }break;
            case cb_get_subset_index:
                 {
                    if(b_subset_index >= pst_info_list->
                                         pst_system_info[w_system_index].
                                         pst_device_tree_element_info_list->
                                         pst_device_tree_element_info[b_master_index].
                                         st_subset_device_id.
                                         b_number)
                    {
                        b_subset_index = 0;
						b_send_data_cnt = 0;

                        b_step = cb_build_master_device_tree;
                    }
                    else
                    {
                        w_inquire_data_buffer_size = (pst_info_list->
                                                      pst_system_info[w_system_index].
                                                      pst_device_tree_element_info_list->
                                                      pst_device_tree_element_info[b_master_index].
                                                      st_subset_device_id.
                                                      b_number + 1) * cb_net_device_tree_length;

                        pb_inquire_data = (uint8_t *)pvPortMalloc(w_inquire_data_buffer_size);
                        if(NULL == pb_inquire_data)
                        {
                            if(pst_net_device_tree_mutex != 0)
                            {
                                xSemaphoreGive(pst_net_device_tree_mutex);
                            }

                            if(pst_net_device_exists_state_mutex != 0)
                            {
                                xSemaphoreGive(pst_net_device_exists_state_mutex);
                            }
                            return;
                        }
                        memset((uint8_t *)pb_inquire_data,0,w_inquire_data_buffer_size);

                        if(FALSE == net_get_device_info(*pst_info_list->
                                                        pst_system_info[w_system_index].
                                                        pst_device_tree_element_info_list->
                                                        pst_device_tree_element_info[b_master_index].
                                                        st_subset_device_id.
                                                        pdw_subset_id_list[b_subset_index],
                                                        (net_device_info **)&pst_net_device_info,
                                                        pst_info_list))
                        {
                            vPortFree(pb_inquire_data);
                            b_step = cb_build_master_device_tree;
                        }
                        else
                        {
                            b_step = cb_inquire_subset_device_tree;
                        }
                    }
                }break;
            case cb_inquire_subset_device_tree:
                {
//                    uint16_t w_data_len = 0;

                    b_send_data_cnt++;
                    if((b_send_data_cnt > 1 && (pst_net_device_info->dw_device_address_send != 0x11)) 
						||(b_send_data_cnt > 2 && (pst_net_device_info->dw_device_address_send == 0x11)) )
                    {
                        b_send_data_cnt = 0;

                        if(FALSE == net_device_exists_status_refresh(*pst_net_device_info->pdw_device_id,
                                                                     cb_net_device_loss,
                                                                     pst_info_list))
                        {
                            b_step = cb_build_master_device_tree;
                        }
						else
						{
							b_subset_index++;
							b_step = cb_get_subset_index;
						}

						vPortFree(pb_inquire_data);
						#if(dynamic_memory_request_test)
						DebugPrintf(TRUE,"net_inquire_device_tree_1_free%x\n",pb_inquire_data);
						#endif
                    }
                    else
                    {
                        if(FALSE == net_device_send_data(pst_net_device_info->b_wired_communication_cable,
													     cb_net_subset_com_inquire_device_tree_cmd,
													     pst_net_device_info->dw_device_address_send,
													     pst_net_device_info->pst_net_device_status->w_sn_inquire_addr,\
													     2,
													     pst_net_device_info->pst_net_device_status->ba_device_tree_inquire_identification,
														 pst_net_device_info))
                        {
							vPortFree(pb_inquire_data);
                            #if(dynamic_memory_request_test)
                            DebugPrintf(TRUE,"net_inquire_device_tree_2_free%x\n",pb_inquire_data);
                            #endif
                            b_step = cb_build_master_device_tree;
                        }
                        else
                        {
                            b_step = cb_receive_subset_device_tree;
                        }
                    }
                    #if(net_inquire_device_tree_Semaphore_test)
                    DebugPrintf(TRUE,"inquire_device_tree_inquire_subset_device_tree_end\n");
                    #endif
                }break;
			case cb_receive_subset_device_tree:
                {
                    #if(net_inquire_device_tree_Semaphore_test)
                    DebugPrintf(TRUE,"inquire_device_tree_receive_subset_device_tree_start\n");
                    #endif
					b_step = cb_inquire_subset_device_tree;

					uint8_t b_receive_state = 0;
					uint16_t w_data_len = 0;
                    TickType_t x_start_time = xTaskGetTickCount();
                    TickType_t x_curent_time = 0;

					for(;;)
					{
						if(TRUE == net_device_receive_data(pst_net_device_info->b_wired_communication_cable,\
														   cb_net_subset_com_inquire_device_tree_cmd,\
														   pst_net_device_info->dw_device_address_receive,\
														   pst_net_device_info->pst_net_device_status->w_sn_inquire_addr,\
														   &w_data_len,
                                                           pb_inquire_data,
                                                           w_inquire_data_buffer_size,
														   &b_receive_state,
														   pst_net_device_info))
						{
							if(cb_wired_network_receive_state_success == b_receive_state)
							{
                                b_send_data_cnt = 0;
                                device_num_statistics(*pst_net_device_info->pdw_device_id,TRUE);
								if(FALSE == subset_device_firmware_version_fill(*pst_net_device_info->pdw_device_id,
																				w_data_len,
																				pb_inquire_data,
																				pst_info_list))
								{
//									b_step = cb_build_master_device_tree;
								}
//								else
								{
									vPortFree(pb_inquire_data);
									#if(dynamic_memory_request_test)
									DebugPrintf(TRUE,"net_inquire_device_tree_3_free%x\n",pb_inquire_data);
									#endif
									b_master_subset_number++;
									b_subset_index++;
									b_step = cb_get_subset_index;
									#if(can_networking_test)
									DebugPrintf(TRUE,"net_inquire_device_tree,complete,time: %d\n",xTaskGetTickCount());
									#endif
									if(check_system_type(cb_inv_type_riio_sun_ii_split_phase) || check_system_type(cb_inv_type_riio_ii_split_phase))
									{
										vTaskDelay(pdMS_TO_TICKS_USER(200));
									}
								}

								break;
							}
							else if(cb_wired_network_receive_state_fail == b_receive_state)
							{
								if(FALSE == net_device_exists_status_refresh(*pst_net_device_info->pdw_device_id,
												 							 cb_net_device_loss,
												 							 pst_info_list))
								{
									vPortFree(pb_inquire_data);
//									b_step = cb_build_master_device_tree;
									b_master_subset_number++;
									b_subset_index++;
									b_step = cb_get_subset_index;
								}
                                if(cb_net_device_loss == \
                                    pst_net_device_info->pst_net_device_status->b_communicate_status)
                                {
                                    device_num_statistics(*pst_net_device_info->pdw_device_id,FALSE);
                                    clear_all_subset(*pst_net_device_info->pdw_device_id,
                                                     pst_info_list);
                                }

								break;
							}
                            else
							{
                                x_curent_time = xTaskGetTickCount();
                                if((x_curent_time - x_start_time) >= pdMS_TO_TICKS_USER(500))
                                {
                                    if(FALSE == net_device_exists_status_refresh(*pst_net_device_info->pdw_device_id,
                                                                                 cb_net_device_loss,
                                                                                 pst_info_list))
                                    {
                                        vPortFree(pb_inquire_data);
                                        b_master_subset_number++;
                                        b_subset_index++;
                                        b_step = cb_get_subset_index;
                                    }
                                    if(cb_net_device_loss == \
                                        pst_net_device_info->pst_net_device_status->b_communicate_status)
                                    {
                                        device_num_statistics(*pst_net_device_info->pdw_device_id,FALSE);
                                        clear_all_subset(*pst_net_device_info->pdw_device_id,
                                                         pst_info_list);
                                    }
                                    break;
                                }
								vTaskDelay(0);
							}
						}
						else
						{
							break;
						}
					}

                    #if(net_inquire_device_tree_Semaphore_test)
                    DebugPrintf(TRUE,"inquire_device_tree_receive_subset_device_tree_end\n");
                    #endif
                }break;
			case cb_build_master_device_tree:
				{
                    #if(net_inquire_device_tree_Semaphore_test)
                    DebugPrintf(TRUE,"inquire_device_tree_build_master_device_tree_start\n");
                    #endif
                    if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[cw_device_id_index_system],
                                                    (net_device_info **)&pst_net_device_info,
                                                    pst_info_list))
                    {
                        if(pst_net_device_tree_mutex != 0)
                        {
                            #if(net_inquire_device_tree_Semaphore_test)
                            DebugPrintf(TRUE,"inquire_device_tree_SemaphoreGive\n");
                            #endif
                            xSemaphoreGive(pst_net_device_tree_mutex);
                        }

                        if(pst_net_device_exists_state_mutex != 0)
                        {
                            #if(net_inquire_device_tree_Semaphore_test)
                            DebugPrintf(TRUE,"device_exists_state_SemaphoreGive; Function called by task: %s\r\n", taskStatus.pcTaskName);
                            #endif
                            xSemaphoreGive(pst_net_device_exists_state_mutex);
                        }
                        return;
                    }

                    int_to_bytes_big_endain(pst_net_device_info->
                                            pst_net_device_tree_info->
                                            st_device_tree.
                                            b_device_id,dwa_device_id_kinergy_ii[cw_device_id_index_system],3);


                    get_client_system_id(pst_net_device_info->
	                                       pst_net_device_tree_info->
	                                       st_device_tree.b_firmware_version,30);


                    get_local_device_secquence(pst_net_device_info->
                                               pst_net_device_tree_info->
                                               st_device_tree.b_serial_number,30);


                    pst_net_device_info->
                    pst_net_device_tree_info->
                    st_device_tree.
                    b_subset_num = 1;

                    if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy_ii],
                                                    (net_device_info **)&pst_net_device_info,
                                                    pst_info_list))
                    {
                        if(pst_net_device_tree_mutex != 0)
                        {
                            #if(net_inquire_device_tree_Semaphore_test)
                            DebugPrintf(TRUE,"inquire_device_tree_SemaphoreGive\n");
                            #endif
                            xSemaphoreGive(pst_net_device_tree_mutex);
                        }

                        if(pst_net_device_exists_state_mutex != 0)
                        {
                            #if(net_inquire_device_tree_Semaphore_test)
                            DebugPrintf(TRUE,"device_exists_state_SemaphoreGive; Function called by task: %s\r\n", taskStatus.pcTaskName);
                            #endif
                            xSemaphoreGive(pst_net_device_exists_state_mutex);
                        }
                        return;
                    }

                    int_to_bytes_big_endain(pst_net_device_info->
                                            pst_net_device_tree_info->
                                            st_device_tree.
                                            b_device_id,dwa_device_id_kinergy_ii[cw_device_id_index_kinergy_ii],3);


                    get_local_firmware_version(pst_net_device_info->
                                               pst_net_device_tree_info->
                                               st_device_tree.b_firmware_version,30);


                    get_local_device_secquence(pst_net_device_info->
                                               pst_net_device_tree_info->
                                               st_device_tree.b_serial_number,30);
                    {
                        uint16_t w_data_len = 30;
//                        net_system_data_access(cb_device_data_access_data_type_status,
//                                   0x0107,
//                                   &w_data_len,
//                                   (uint8_t *)&st_net_device_tree_system.st_device_tree.b_firmware_version,
//                                   cb_device_data_access_operate_type_write);
						  net_system_data_access(cb_device_data_access_data_type_status,
									 0x0107,
									 &w_data_len,
									 (uint8_t *)&pst_net_device_info->pst_net_device_tree_info->st_device_tree.b_firmware_version,
									 cb_device_data_access_operate_type_write);

                    }
                    pst_net_device_info->
                    pst_net_device_tree_info->
                    st_device_tree.
                    b_subset_num = b_master_subset_number;

                    system_init_comfirm(cb_system_init_type_device_tree);
                    b_step = cb_device_tree_build_complete;
                    #if(net_inquire_device_tree_Semaphore_test)
                    DebugPrintf(TRUE,"inquire_device_tree_build_master_device_tree_end\n");
                    #endif
                }break;
			default:
				{
                    #if(net_inquire_device_tree_Semaphore_test)
                    DebugPrintf(TRUE,"inquire_device_tree_default\n");
                    #endif
				}break;
		}

		{
			vTaskDelay(0);
		}
	}while(b_step != cb_device_tree_build_complete);

	if(pst_net_device_tree_mutex != 0)
    {
        #if(net_inquire_device_tree_Semaphore_test)
        DebugPrintf(TRUE,"inquire_device_tree_SemaphoreGive\n");
        #endif
        xSemaphoreGive(pst_net_device_tree_mutex);
    }

    if(pst_net_device_exists_state_mutex != 0)
    {
        #if(net_inquire_device_tree_Semaphore_test)
        DebugPrintf(TRUE,"device_exists_state_SemaphoreGive; Function called by task: %s\r\n", taskStatus.pcTaskName);
        #endif
        xSemaphoreGive(pst_net_device_exists_state_mutex);
    }

	#if( task_runing_status_print )
    DebugPrintf(TRUE,"Device_tree_inquire_end\r\n");
	#endif
	if (xTaskGetTickCount() > (portMAX_DELAY - pdMS_TO_TICKS_USER(60000)))
	{
		xTicksToWait = 0;
	}
	if (TRUE == b_reconnet_server)
	{
		b_reconnet_server = FALSE;
		if ((xTaskGetTickCount() - xTicksToWait) > pdMS_TO_TICKS_USER(60000))
		{
			xTicksToWait = xTaskGetTickCount();
			network_com_with_tbb_device_server_state_set(cb_net_com_state_type_err_code, net_sever_reconnect);
		}
	}
}


uint8_t subset_device_firmware_version_fill(uint32_t dw_device_id,uint16_t w_data_in_len,uint8_t *pb_data_in,net_system_info_list *pst_info_list)
{
    #define cb_subset_device_sn_len                     30
	#define cb_subset_device_id_len						3
	#define cb_subset_device_firmware_version_len		30
    #define cb_subset_device_subset_num_len             1
    #define cb_subset_device_tree_info_len              (cb_subset_device_sn_len +\
                                                         cb_subset_device_id_len +\
                                                         cb_subset_device_firmware_version_len +\
                                                         cb_subset_device_subset_num_len)
	#define	cb_subset_device_tree_len					sizeof(subset_device_tree_format)
	typedef struct
	{
        uint8_t b_serial_number[cb_subset_device_sn_len];
		uint8_t b_device_id[cb_subset_device_id_len];
		uint8_t b_firmware_version[cb_subset_device_firmware_version_len];
        uint8_t b_subset_num[cb_subset_device_subset_num_len];
	}subset_device_tree_format;

	if((NULL == pb_data_in) || (NULL == pst_info_list))
	{
		return FALSE;
	}

	if((w_data_in_len % cb_subset_device_tree_len) != 0)
	{
		return FALSE;
	}

    net_device_tree *pst_net_device_tree = 0;
	subset_device_tree_format *pst_subset_device_tree_format = 0;
	net_device_tree_element_info *pst_net_device_tree_element_info = 0;
	uint32_t dw_device_id_temp = 0;
	uint16_t w_data_index = 0;
    uint8_t b_master_data_index = 0;
    uint8_t b_subset_number = 0;
	uint16_t w_data_len = 30;
	uint8_t b_inv_serial_number[cb_subset_device_sn_len];
	if((dw_device_id & 0xffffff00) == 0x00001A00)
	{
		net_device_data_access(dw_device_id,
							   cb_net_data_att_set,
							   0x0080,
							   &w_data_len,
							   (uint8_t *)&b_inv_serial_number,
							   cb_device_data_access_operate_type_read);
		for(uint8_t b_cnt_i = 0; b_cnt_i < w_data_len; b_cnt_i ++)
		{
			pb_data_in[b_cnt_i] = b_inv_serial_number[b_cnt_i];
		}

	}

	if(FALSE == net_get_device_tree_relasion(dw_device_id,(net_device_tree_element_info ** const)(&pst_net_device_tree_element_info),pst_info_list))
	{
		return FALSE;
	}

	for(uint8_t b_subset_index = 0;b_subset_index < pst_net_device_tree_element_info->st_subset_device_id.b_number;b_subset_index++)
	{
        for(w_data_index = 0;w_data_index < w_data_in_len;w_data_index += cb_subset_device_tree_info_len)
        {
            pst_subset_device_tree_format = (subset_device_tree_format *)&pb_data_in[w_data_index];

            if(FALSE == bytes_to_int_big_to_little_endain(pst_subset_device_tree_format->b_device_id,&dw_device_id_temp,3))
            {
                return FALSE;
            }

            if(dw_device_id == dw_device_id_temp)
            {
                b_master_data_index = w_data_index;
                continue;
            }
            else if(dw_device_id_temp !=  *pst_net_device_tree_element_info->st_subset_device_id.pdw_subset_id_list[b_subset_index])
            {
                continue;
            }

            if(FALSE == net_get_device_tree_info(dw_device_id_temp,(net_device_tree **)(&pst_net_device_tree),pst_info_list))//从pst_info_list列表中获取dw_device_id_temp的设备树地址
            {
                return FALSE;
            }

            if(NULL == memcpy(pst_net_device_tree->st_device_tree.b_serial_number,
                              pst_subset_device_tree_format->b_serial_number,
                              cb_subset_device_sn_len))
            {
                return FALSE;
            }

            if(NULL == memcpy(pst_net_device_tree->st_device_tree.b_device_id,
                              pst_subset_device_tree_format->b_device_id,
                              cb_subset_device_id_len))
            {
                return FALSE;
            }

//            if(NULL == memcpy(pst_net_device_tree->st_device_tree.b_firmware_version,
//                              pst_subset_device_tree_format->b_firmware_version,
//                              cb_subset_device_firmware_version_len))
//            {
//                return FALSE;
//            }
			if(NULL == memcpy(pst_net_device_tree->st_device_tree.b_firmware_version,
							0,
							cb_subset_device_firmware_version_len))
			{
				return FALSE;
			}

            if(NULL == memcpy(&pst_net_device_tree->st_device_tree.b_subset_num,
                              &pst_subset_device_tree_format->b_subset_num,
                              cb_subset_device_subset_num_len))
            {
                return FALSE;
            }

            {
                if(FALSE == net_device_exists_status_refresh(dw_device_id_temp,
                                                             cb_net_device_add,
                                                             pst_info_list))
                {
                    return FALSE;
                }
            }

            b_subset_number++;
        }
	}

    {
        pst_subset_device_tree_format = (subset_device_tree_format *)&pb_data_in[b_master_data_index];

        if(FALSE == bytes_to_int_big_to_little_endain(pst_subset_device_tree_format->b_device_id,&dw_device_id_temp,3))
        {
            return FALSE;
        }

//        if(dw_device_id_temp !=  dw_device_id)
//        {
//            return FALSE;
//        }

        if(FALSE == net_get_device_tree_info(dw_device_id,(net_device_tree **)(&pst_net_device_tree),pst_info_list))
        {
            return FALSE;
        }

        if(NULL == memcpy(pst_net_device_tree->st_device_tree.b_serial_number,
                          pst_subset_device_tree_format->b_serial_number,
                          cb_subset_device_sn_len))
        {
            return FALSE;
        }


        if(NULL == memcpy(pst_net_device_tree->st_device_tree.b_device_id,
                          pst_subset_device_tree_format->b_device_id,
                          cb_subset_device_id_len))
        {
            return FALSE;
        }

        if(0 != memcmp(pst_net_device_tree->st_device_tree.b_firmware_version,
                       pst_subset_device_tree_format->b_firmware_version,
                       cb_subset_device_firmware_version_len))
        {
        	extern uint8_t gb_device_tree_update_flag;
	        uint16_t w_data_len = 1;
        	gb_device_tree_update_flag = 1;
		    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                   cb_device_data_access_data_type_status,
                                   0x017E,
                                   &w_data_len,
                                   (uint8_t *)&gb_device_tree_update_flag,
                                   cb_device_data_access_operate_type_write);
            network_com_with_tbb_device_server_state_set(cb_net_com_state_type_err_code,net_sever_connect);
        }

        if(NULL == memcpy(pst_net_device_tree->st_device_tree.b_firmware_version,
                          pst_subset_device_tree_format->b_firmware_version,
                          cb_subset_device_firmware_version_len))
        {
            return FALSE;
        }

        pst_net_device_tree->st_device_tree.b_subset_num = b_subset_number;

        {
            if(FALSE == net_device_exists_status_refresh(dw_device_id,
                                                         cb_net_device_add,
                                                         pst_info_list))
            {
                return FALSE;
            }
            if((dw_device_id & 0xFF00) == cdw_device_type_cklcd)
            {
                if(FALSE == net_device_exists_status_refresh((cdw_device_type_ck_inverter | (dw_device_id & 0x00FF)),
                                                             cb_net_device_add,
                                                             pst_info_list))
                {
                    return FALSE;
                }
            }
        }
    }

	return TRUE;
}

void device_num_statistics(uint32_t dw_device_id,uint8_t f_device_add)
{
    static uint8_t b_inverter_num;
    static uint8_t b_mppt_num;
	static uint16_t b_pv_num;
	static uint16_t b_pv_num_backup;
    static uint16_t w_inverter_exist_bits;
    static uint32_t dw_mppt_exist_bits_l;
    static uint32_t dw_mppt_exist_bits_h;
    static uint32_t dw_mppt_exist_bits_l_backup;
    static uint32_t dw_mppt_exist_bits_h_backup;
	static uint8_t b_fist_connect_flag = 1;
	static uint8_t b_org_inv_num = 0;
    uint8_t b_cnt_i;

    if((cdw_device_type_cklcd == (dw_device_id & 0xFFFF00)) ||\
       (cdw_device_type_ck_inverter == (dw_device_id & 0xFFFF00)))
    {
        uint8_t b_step = 0;
		uint8_t b_data_temp = 0;
        uint16_t w_data_temp = 0;
        uint16_t w_data_len = 2;
        uint32_t dw_device_id_temp = (dw_device_id & 0x00FF) | cdw_device_type_cklcd;

        b_step = (((dw_device_id & 0x00F0) >> 4) - 1) * 3 +\
                  ((dw_device_id & 0x000F) - 1);
        if(b_step < 16)
        {
            if(f_device_add)
            {
                w_inverter_exist_bits |= (1 << b_step);
            }
            else
            {
                w_inverter_exist_bits &= ~(uint16_t)(1 << b_step);
            }
        }

        net_device_data_access(dw_device_id_temp,
                               cb_net_data_att_set,
                               0x0000,
                               &w_data_len,
                               (uint8_t *)&b_data_temp,
                               cb_device_data_access_operate_type_read);
        b_step = (((dw_device_id & 0x00F0) >> 4) - 1) * 18 +\
                  (((dw_device_id & 0x000F) - 1) * 6);
        if(((dc_couple == b_data_temp) ||\
            (ac_dc_couple == b_data_temp)) &&\
           (TRUE == f_device_add))
        {
            net_device_data_access(dw_device_id_temp,
                                   cb_net_data_att_set,
                                   0x0001,
                                   &w_data_len,
                                   (uint8_t *)&w_data_temp,
                                   cb_device_data_access_operate_type_read);
            for(b_cnt_i = 0;b_cnt_i < 6;b_cnt_i++)
            {
                if(b_cnt_i < w_data_temp)
                {
                    if(b_cnt_i + b_step < 32)
                    {
                        dw_mppt_exist_bits_l |= (1 << (b_cnt_i + b_step));
                    }
                    else if(b_cnt_i + b_step < 64)
                    {
                        dw_mppt_exist_bits_h |= (1 << (b_cnt_i + b_step - 32));
                    }
                }
                else
                {
                    if(b_cnt_i + b_step < 32)
                    {
                        dw_mppt_exist_bits_l &= ~(1 << (b_cnt_i + b_step));
                    }
                    else if(b_cnt_i + b_step < 64)
                    {
                        dw_mppt_exist_bits_h &= ~(1 << (b_cnt_i + b_step - 32));
                    }
                }
            }
        }
        else
        {
            for(b_cnt_i = 0;b_cnt_i < 6;b_cnt_i++)
            {
                if(b_cnt_i + b_step < 32)
                {
                    dw_mppt_exist_bits_l &= ~(1 << (b_cnt_i + b_step));
                }
                else if(b_cnt_i + b_step < 64)
                {
                    dw_mppt_exist_bits_h &= ~(1 << (b_cnt_i + b_step - 32));
                }
            }
        }
		net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
		                       cb_net_data_att_set,
		                       0x0000,
		                       &w_data_len,
		                       ( uint8_t * )&b_data_temp,
		                       cb_device_data_access_operate_type_read);
		if (ac_couple == b_data_temp || ac_dc_couple == b_data_temp)
		{
			{
				uint16_t w_data_len = 2;
				net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
				                       cb_device_data_access_data_type_set,
				                       0x0002,
				                       &w_data_len,
				                       ( uint8_t * )&b_pv_num,
				                       cb_device_data_access_operate_type_read);
			}

			net_device_info *pst_net_device_info = 0;
			net_get_device_info(dwa_device_id_kinergy_ii[cw_device_id_index_pv_1],
			                    ( net_device_info ** )&pst_net_device_info,
			                    &st_net_system_info_list);

			pst_net_device_info->pst_net_data_list_info->pst_device_inquire_list[0].f_poll_en = TRUE;
			for (b_cnt_i = 0; b_cnt_i < 2; b_cnt_i++)
			{
				if (b_cnt_i < b_pv_num)
				{
					net_device_exists_status_refresh(dwa_device_id_kinergy_ii[cw_device_id_index_pv_1 + b_cnt_i],
					                                 cb_net_device_add,
					                                 &st_net_system_info_list);
				}
				else
				{
					net_device_exists_status_refresh(dwa_device_id_kinergy_ii[cw_device_id_index_pv_1 + b_cnt_i],
					                                 cb_net_device_loss,
					                                 &st_net_system_info_list);
				}
			}
		}
		else
		{
			b_pv_num = 0;
			net_device_exists_status_refresh(
			    dwa_device_id_kinergy_ii[cw_device_id_index_pv_1], cb_net_device_loss, &st_net_system_info_list);
			net_device_exists_status_refresh(
			    dwa_device_id_kinergy_ii[cw_device_id_index_pv_2], cb_net_device_loss, &st_net_system_info_list);
		}
	}
    {   //inv num
        uint16_t w_data_len = 1;
        uint8_t b_inverter_num_temp = 0;
		static TickType_t x_start_time = 0;
        for(b_cnt_i = 0;b_cnt_i < 9;b_cnt_i++)
        {
            if(w_inverter_exist_bits & (1 << b_cnt_i))
            {
                b_inverter_num_temp++;
            }
        }
        {
			TickType_t x_curent_time = 0;

			if(0 == x_start_time)
			{
				x_start_time = xTaskGetTickCount();
			}
			x_curent_time = xTaskGetTickCount();
			if(b_fist_connect_flag && ((0 != x_start_time) && (x_curent_time - x_start_time) >= pdMS_TO_TICKS_USER(180000)))
			{
				x_start_time = 0;
				b_fist_connect_flag = 0;
				b_org_inv_num = b_inverter_num;
				net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
								   cb_net_data_att_status,
								   0x017A,
								   &w_data_len,
								   (uint8_t *)&b_org_inv_num,
								   cb_device_data_access_operate_type_write);
			
			}
		}

        if((b_inverter_num_temp != b_inverter_num) ||\
           (dw_mppt_exist_bits_l_backup != dw_mppt_exist_bits_l) ||\
           (dw_mppt_exist_bits_h_backup != dw_mppt_exist_bits_h) ||\
		   (b_pv_num != b_pv_num_backup))
        {
            b_inverter_num = b_inverter_num_temp;
			b_pv_num_backup = b_pv_num;
            dw_mppt_exist_bits_l_backup = dw_mppt_exist_bits_l;
            dw_mppt_exist_bits_h_backup = dw_mppt_exist_bits_h;

            network_com_with_tbb_device_server_state_set(cb_net_com_state_type_err_code,net_sever_connect);
			extern uint8_t gb_device_tree_update_flag;
	        uint16_t w_data_len = 1;
        	gb_device_tree_update_flag = 1;
		    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                   cb_device_data_access_data_type_status,
                                   0x017E,
                                   &w_data_len,
                                   (uint8_t *)&gb_device_tree_update_flag,
                                   cb_device_data_access_operate_type_write);
            net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                   cb_net_data_att_status,
                                   0x0160,
                                   &w_data_len,
                                   (uint8_t *)&b_inverter_num,
                                   cb_device_data_access_operate_type_write);
        }
    }
    {   //mppt num
        b_mppt_num = 0;
        uint16_t w_data_len = 1;
        uint8_t b_cnt_j = 0;
        for(b_cnt_i = cw_device_id_index_mppt_u1_0;
            b_cnt_i <= cw_device_id_index_mppt_w3_5;
            b_cnt_i++)
        {
            b_cnt_j = b_cnt_i - cw_device_id_index_mppt_u1_0;
            if(((b_cnt_j < 32) && (dw_mppt_exist_bits_l & (1 << b_cnt_j))) ||\
               ((b_cnt_j >= 32) && (dw_mppt_exist_bits_h & (1 << (b_cnt_j - 32)))))
            {
                net_device_exists_status_refresh(dwa_device_id_kinergy_ii[b_cnt_i],
                                                 cb_net_device_add,
                                                 &st_net_system_info_list);
                b_mppt_num++;
            }
            else
            {
                net_device_exists_status_refresh(dwa_device_id_kinergy_ii[b_cnt_i],
                                                 cb_net_device_loss,
                                                 &st_net_system_info_list);
            }
        }
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_net_data_att_status,
                               0x015F,
                               &w_data_len,
                               (uint8_t *)&b_mppt_num,
                               cb_device_data_access_operate_type_write);
    }
}

uint8_t clear_all_subset(uint32_t dw_device_id,
                         net_system_info_list *pst_info_list)
{
    net_device_tree_element_info *pst_net_device_tree_element_info = 0;
    uint8_t b_cnt_i;
    if(TRUE == net_get_device_tree_relasion(dw_device_id,
                                            (net_device_tree_element_info **)&pst_net_device_tree_element_info,
                                            pst_info_list))
    {
        for(b_cnt_i = 0;
            b_cnt_i < pst_net_device_tree_element_info->st_subset_device_id.b_number;
            b_cnt_i++)
        {
            if(FALSE == net_device_exists_status_refresh(*pst_net_device_tree_element_info->
                                                            st_subset_device_id.
                                                            pdw_subset_id_list[b_cnt_i],
                                                         cb_net_device_loss,
                                                         pst_info_list))
            {
                return FALSE;
            }
            clear_all_subset(*pst_net_device_tree_element_info->st_subset_device_id.pdw_subset_id_list[b_cnt_i],
                             pst_info_list);
        }
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

//uint8_t subset_device_firmware_version_fill(uint32_t dw_device_id,uint16_t w_data_in_len,uint8_t *pb_data_in,net_system_info_list *pst_info_list)
//{
//	#define cb_subset_device_id_len						3
//	#define cb_subset_device_firmware_version_len		20
//    #define cb_subset_device_tree_info_len              (cb_subset_device_id_len + cb_subset_device_firmware_version_len)
//	#define	cb_subset_device_tree_len					sizeof(subset_device_tree_format)
//	typedef struct
//	{
//		uint8_t b_device_id[cb_subset_device_id_len];
//		uint8_t b_firmware_version[cb_subset_device_firmware_version_len];
//	}subset_device_tree_format;
//
//	if((NULL == pb_data_in) || (NULL == pst_info_list))
//	{
//		return FALSE;
//	}
//
//	if((w_data_in_len % cb_subset_device_tree_len) != 0)
//	{
//		return FALSE;
//	}
//
//	net_device_tree *pst_net_device_tree = 0;
//	subset_device_tree_format *pst_subset_device_tree_format = 0;
//	net_device_tree_element_info *pst_net_device_tree_element_info = 0;
//	uint32_t dw_device_id_temp = 0;
//	uint16_t w_data_index = 0;
//    uint8_t b_master_data_index = 0;
//    uint8_t b_subset_number = 0;
//
//	if(FALSE == net_get_device_tree_relasion(dw_device_id,(net_device_tree_element_info ** const)(&pst_net_device_tree_element_info),pst_info_list))
//	{
//		return FALSE;
//	}
//
//	for(uint8_t b_subset_index = 0;b_subset_index < pst_net_device_tree_element_info->st_subset_device_id.b_number;b_subset_index++)
//	{
//        for(w_data_index = 0;w_data_index < w_data_in_len;w_data_index += cb_subset_device_tree_info_len)
//        {
//            pst_subset_device_tree_format = (subset_device_tree_format *)&pb_data_in[w_data_index];
//
//            if(FALSE == bytes_to_int_little_to_little_endain(pst_subset_device_tree_format->b_device_id,&dw_device_id_temp,3))
//            {
//                return FALSE;
//            }
//
//            if(dw_device_id == dw_device_id_temp)
//            {
//                b_master_data_index = w_data_index;
//                continue;
//            }
//            else if(dw_device_id_temp !=  pst_net_device_tree_element_info->st_subset_device_id.bdw_subset_id_list[b_subset_index])
//            {
//                continue;
//            }
//
//            if(FALSE == net_get_device_tree_info(dw_device_id_temp,(net_device_tree **)(&pst_net_device_tree),pst_info_list))
//            {
//                return FALSE;
//            }
//
//            if(NULL == memcpy(pst_net_device_tree->st_device_tree.b_device_id,
//                              pst_subset_device_tree_format->b_device_id,
//                              cb_subset_device_id_len))
//            {
//                return FALSE;
//            }
//
//            if(NULL == memcpy(pst_net_device_tree->st_device_tree.b_firmware_version,
//                              pst_subset_device_tree_format->b_firmware_version,
//                              cb_subset_device_firmware_version_len))
//            {
//                return FALSE;
//            }
//
//            {
//                if(FALSE == net_device_exists_status_refresh(dw_device_id_temp,
//                                                             cb_net_device_add,
//                                                             pst_info_list))
//                {
//                    return FALSE;
//                }
//            }
//
//            b_subset_number++;
//        }
//	}
//
//    {
//        pst_subset_device_tree_format = (subset_device_tree_format *)&pb_data_in[b_master_data_index];
//
//        if(FALSE == bytes_to_int_little_to_little_endain(pst_subset_device_tree_format->b_device_id,&dw_device_id_temp,3))
//        {
//            return FALSE;
//        }
//
//        if(dw_device_id_temp !=  dw_device_id)
//        {
//            return FALSE;
//        }
//
//        if(FALSE == net_get_device_tree_info(dw_device_id_temp,(net_device_tree **)(&pst_net_device_tree),pst_info_list))
//        {
//            return FALSE;
//        }
//
//        if(NULL == memcpy(pst_net_device_tree->st_device_tree.b_device_id,
//                          pst_subset_device_tree_format->b_device_id,
//                          cb_subset_device_id_len))
//        {
//            return FALSE;
//        }
//
//        if(NULL == memcpy(pst_net_device_tree->st_device_tree.b_firmware_version,
//                          pst_subset_device_tree_format->b_firmware_version,
//                          cb_subset_device_firmware_version_len))
//        {
//            return FALSE;
//        }
//
//        pst_net_device_tree->st_device_tree.b_subset_num = b_subset_number;
//
//        {
//            if(FALSE == net_device_exists_status_refresh(dw_device_id_temp,
//                                                         cb_net_device_add,
//                                                         pst_info_list))
//            {
//                return FALSE;
//            }
//        }
//    }
//
//	return TRUE;
//}
/*RESERVE*/
//uint8_t subset_device_sn_fill(uint32_t dw_device_id,uint16_t w_data_in_len,uint8_t *pb_data_in,net_system_info_list *pst_info_list)
//{
//	#define cb_subset_sn_len						30
//
//	if((NULL == pb_data_in) || (NULL == pst_info_list))
//	{
//		return FALSE;
//	}
//
//	if(w_data_in_len > cb_subset_sn_len)
//	{
//		return FALSE;
//	}
//
//	net_device_tree *pst_net_device_tree = 0;
//
//	if(FALSE == net_get_device_tree_info(dw_device_id,(net_device_tree **)(&pst_net_device_tree),pst_info_list))
//	{
//		return FALSE;
//	}
//
//	if(NULL == memcpy(pst_net_device_tree->st_device_tree.b_serial_number,
//			          pb_data_in,
//			          cb_subset_device_id_len))
//	{
//		return FALSE;
//	}
//
//	return TRUE;
//}
