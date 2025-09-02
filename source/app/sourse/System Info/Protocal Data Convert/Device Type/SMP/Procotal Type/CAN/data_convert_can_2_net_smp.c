#include "data_convert_can_2_net_smp.h"
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOS.h"
#include "freertos_user_config.h"
#include "net_device_data_SMP.h"
#include "net_device_data_bms.h"
#include "net_device_tree_bms.h"
#include "net_system_info_list.h"
#include "net_system_info_access.h"
#include "net_device_tree.h"
#include "net_system_info.h"
#include "can_cycle_data_smp.h"
#include "can_cycle_data_smp_bms.h"
#include "net_device_tree_smp.h"
#include "system_info_general_macro.h"
#include "net_device_data_access.h"
#include "can_network_protocal.h"
#include "application_general_func.h"
#include "net_segmented_inquire.h"


void device_data_polling_smp(net_device_info * const pst_net_device_info)
{
	if(NULL == pst_net_device_info)
	{
		return;
	}

	if(TRUE == net_check_device_communicate_status(cb_net_device_loss,pst_net_device_info))
	{
		uint8_t ba_data[11];
		uint16_t w_data_len = 11;

		if(FALSE == int_to_int_little_to_big_endain(0x00FF,(uint32_t *)&ba_data[0],2))
		{

		}

		ba_data[2] = 1;

		if(FALSE == int_to_int_little_to_big_endain(0x005600,(uint32_t *)&ba_data[3],4))
		{

		}

		if(FALSE == int_to_int_little_to_big_endain(0x00,(uint32_t *)&ba_data[7],4))
		{

		}

		net_device_data_access(cdw_device_id_SMP,
							   cb_net_data_att_status,
							   0x1017,
							   &w_data_len,
							   ba_data,
							   cb_device_data_access_operate_type_write);
	}
	else if(TRUE == net_check_device_communicate_status(cb_net_device_add,pst_net_device_info))
	{
		{
			uint8_t b_receive_state = 0;
			uint16_t w_receive_data_len = 0;
			uint8_t ba_receive_data[220] = {0,};

			if(TRUE == net_device_send_data(pst_net_device_info->b_wired_communication_cable,
											cb_wired_network_inquire_device_alarm_cmd,
											pst_net_device_info->dw_device_address_send,
											0x0000,
											2,
											(uint8_t *)&w_receive_data_len,
											pst_net_device_info))
			{
				for(;;)
				{
					if(TRUE == net_device_receive_data(pst_net_device_info->b_wired_communication_cable,
													   cb_wired_network_inquire_device_alarm_cmd,
													   pst_net_device_info->dw_device_address_receive,
													   0x0000,
													   &w_receive_data_len,
													   ba_receive_data,
													   2,
													   &b_receive_state,
													   pst_net_device_info))
					{
						if(cb_wired_network_receive_state_success == b_receive_state)
						{
							net_device_data_access(cdw_device_id_SMP,
												   cb_net_data_att_status,
												   0x1017,
												   &w_receive_data_len,
												   ba_receive_data,
												   cb_device_data_access_operate_type_write);

							break;
						}
						else if(cb_wired_network_receive_state_fail == b_receive_state)
						{
							break;
						}

						{
							vTaskDelay(0);
						}
					}
					else
					{
						break;
					}
				}
			}
		}

		{
			uint16_t w_receive_data_len = 0;
			uint8_t ba_receive_data[4] = {0,};

			net_data_list *pst_device_data_list = 0;

			if(TRUE == net_get_device_data_list(cb_net_data_att_status,
											    (net_data_list **)&pst_device_data_list,
											    pst_net_device_info))
			{
				net_device_data_inquire_segmented(0x1116,0x1117,&w_receive_data_len,ba_receive_data,4,pst_device_data_list,pst_net_device_info);

				w_receive_data_len = 2;
				byte_sort_in_reverse_order(w_receive_data_len,ba_receive_data);
				net_device_data_access(cdw_device_id_SMP,
									   cb_net_data_att_status,
									   0x1116,
									   &w_receive_data_len,
									   &ba_receive_data[0],
									   cb_device_data_access_operate_type_write);

				byte_sort_in_reverse_order(w_receive_data_len,&ba_receive_data[2]);
				net_device_data_access(cdw_device_id_SMP,
									   cb_net_data_att_status,
									   0x1117,
									   &w_receive_data_len,
									   &ba_receive_data[2],
									   cb_device_data_access_operate_type_write);
			}

		}

		{
			uint16_t w_receive_data_len = 0;
			uint8_t ba_receive_data[2] = {0,};

			net_data_list *pst_device_data_list = 0;

			if(TRUE == net_get_device_data_list(cb_net_data_att_status,
											    (net_data_list **)&pst_device_data_list,
											    pst_net_device_info))
			{
				net_device_data_inquire_segmented(0x1109,0x1109,&w_receive_data_len,ba_receive_data,2,pst_device_data_list,pst_net_device_info);

				w_receive_data_len = 2;
				byte_sort_in_reverse_order(w_receive_data_len,ba_receive_data);
				net_device_data_access(cdw_device_id_SMP,
									   cb_net_data_att_status,
									   0x1109,
									   &w_receive_data_len,
									   &ba_receive_data[0],
									   cb_device_data_access_operate_type_write);
			}

		}
	}
}

uint8_t data_convert_can_cycle_2_net_status_smp(uint32_t dw_can_id,uint8_t b_can_cmd,uint8_t *pb_can_data_in,net_system_info_list *pst_info_list)
{
	if(NULL == pb_can_data_in)
	{
		return FALSE;
	}

    switch(dw_can_id)
	{
		case 0x0458:
		{
			can_cycle_data_0458_SMP *pst_can_cycle_data_0458 = 0;

			pst_can_cycle_data_0458 = (can_cycle_data_0458_SMP *)pb_can_data_in;

            {
                uint16_t w_data_len = 0;
                uint32_t dw_data = 0;

                w_data_len = 2;
                dw_data = pst_can_cycle_data_0458->bits.bit_2_0_16;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1100,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 2;
                dw_data = pst_can_cycle_data_0458->bits.bit_6_0_16;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1108,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 2;
                dw_data = pst_can_cycle_data_0458->bits.bit_0_0_16;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x110A,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 2;
                dw_data = pst_can_cycle_data_0458->bits.bit_4_0_16;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x110B,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);
            }
		}break;
		case 0x0459:
		{
			can_cycle_data_0459_SMP *pst_can_cycle_data_0459 = 0;

			pst_can_cycle_data_0459 = (can_cycle_data_0459_SMP *)pb_can_data_in;

            {
                uint16_t w_data_len = 0;
                uint32_t dw_data = 0;

                w_data_len = 2;
                dw_data = pst_can_cycle_data_0459->bits.bit_2_0_16;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1101,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 2;
                dw_data = pst_can_cycle_data_0459->bits.bit_0_0_16;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x110C,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 2;
                dw_data = pst_can_cycle_data_0459->bits.bit_4_0_16;
                net_device_data_access(cdw_device_id_SMP,
                                        cb_device_data_access_data_type_status,
                                        0x110D,
                                        &w_data_len,
                                        (uint8_t *)&dw_data,
                                        cb_device_data_access_operate_type_write);

                w_data_len = 2;
                dw_data = pst_can_cycle_data_0459->bits.bit_6_0_16;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x110E,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);
            }
		}break;
		case 0x045A:
		{
			can_cycle_data_045A_SMP *pst_can_cycle_data_045A = 0;
			pst_can_cycle_data_045A = (can_cycle_data_045A_SMP *)pb_can_data_in;

            {
                uint16_t w_data_len = 0;
                uint32_t dw_data = 0;

                w_data_len = 2;
                dw_data = pst_can_cycle_data_045A->bits.bit_0_0_16;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x110F,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_045A->bits.bit_2_0_8;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1110,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_045A->bits.bit_3_0_8;
                net_device_data_access(cdw_device_id_SMP,
                                        cb_device_data_access_data_type_status,
                                        0x1111,
                                        &w_data_len,
                                        (uint8_t *)&dw_data,
                                        cb_device_data_access_operate_type_write);

                w_data_len = 2;
                dw_data = pst_can_cycle_data_045A->bits.bit_4_0_16;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1112,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 2;
                dw_data = pst_can_cycle_data_045A->bits.bit_6_0_16;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1113,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);
            }
		}break;
		case 0x045B:
		{
			can_cycle_data_045B_SMP *pst_can_cycle_data_045B = 0;
			pst_can_cycle_data_045B = (can_cycle_data_045B_SMP *)pb_can_data_in;

            {
                uint16_t w_data_len = 0;
                uint32_t dw_data = 0;

                w_data_len = 1;
                dw_data = pst_can_cycle_data_045B->bits.bit_0_0_8;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1102,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_045B->bits.bit_1_0_8;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1103,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_045B->bits.bit_2_0_8;
                net_device_data_access(cdw_device_id_SMP,
                                        cb_device_data_access_data_type_status,
                                        0x1104,
                                        &w_data_len,
                                        (uint8_t *)&dw_data,
                                        cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_045B->bits.bit_3_0_8;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1105,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_045B->bits.bit_6_0_8;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1106,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_045B->bits.bit_7_0_8;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1107,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_045B->bits.bit_4_0_3;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1114,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 2;
                dw_data = pst_can_cycle_data_045B->bits.bit_5_0_8;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1115,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);
            }
		}break;
		case 0x0460:
		{
			can_cycle_data_0460_SMP *pst_can_cycle_data_0460 = 0;
			pst_can_cycle_data_0460 = (can_cycle_data_0460_SMP *)pb_can_data_in;

            {
                uint16_t w_data_len = 0;
                uint32_t dw_data = 0;

//                dw_data = pst_can_cycle_data_0460->bits.bit_0_0_1;
//                net_device_active_data_refresh(cdw_device_id_SMP,
//                                               cb_device_data_access_data_type_status,
//                                               0x1000,
//                                               1,
//                                               (uint8_t *)&dw_data);
//
//                dw_data = pst_can_cycle_data_0460->bits.bit_0_1_1;
//                net_device_active_data_refresh(cdw_device_id_SMP,
//                                               cb_device_data_access_data_type_status,
//                                               0x1001,
//                                               1,
//                                               (uint8_t *)&dw_data);
//
//                dw_data = pst_can_cycle_data_0460->bits.bit_0_2_1;
//                net_device_active_data_refresh(cdw_device_id_SMP,
//                                               cb_device_data_access_data_type_status,
//                                               0x1002,
//                                               1,
//                                               (uint8_t *)&dw_data);
//
//                dw_data = pst_can_cycle_data_0460->bits.bit_0_3_1;
//                net_device_active_data_refresh(cdw_device_id_SMP,
//                                               cb_device_data_access_data_type_status,
//                                               0x1003,
//                                               1,
//                                               (uint8_t *)&dw_data);
//
//                dw_data = pst_can_cycle_data_0460->bits.bit_0_4_1;
//                net_device_active_data_refresh(cdw_device_id_SMP,
//                                               cb_device_data_access_data_type_status,
//                                               0x1004,
//                                               1,
//                                               (uint8_t *)&dw_data);
//
//                dw_data = pst_can_cycle_data_0460->bits.bit_0_5_1;
//                net_device_active_data_refresh(cdw_device_id_SMP,
//                                               cb_device_data_access_data_type_status,
//                                               0x1005,
//                                               1,
//                                               (uint8_t *)&dw_data);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0460->bits.bit_0_0_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1000,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0460->bits.bit_0_1_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1001,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0460->bits.bit_0_2_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1002,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0460->bits.bit_0_3_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1003,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0460->bits.bit_0_4_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1004,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0460->bits.bit_0_5_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1005,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0460->bits.bit_1_6_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1019,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0460->bits.bit_1_7_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x101A,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0460->bits.bit_1_2_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1020,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0460->bits.bit_1_3_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1021,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0460->bits.bit_1_4_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1022,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0460->bits.bit_1_5_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1023,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0460->bits.bit_2_0_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1024,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);
            }
		}break;
		case 0x0461:
		{
			can_cycle_data_0461_SMP *pst_can_cycle_data_0461 = 0;
			pst_can_cycle_data_0461 = (can_cycle_data_0461_SMP *)pb_can_data_in;

            {
                uint16_t w_data_len = 0;
                uint32_t dw_data = 0;

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0461->bits.bit_3_3_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1008,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0461->bits.bit_4_3_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1009,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0461->bits.bit_4_6_2;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x100A,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0461->bits.bit_7_0_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x100B,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0461->bits.bit_0_0_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x100C,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0461->bits.bit_0_1_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x100D,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0461->bits.bit_0_2_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x100E,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);


                w_data_len = 1;
                dw_data = pst_can_cycle_data_0461->bits.bit_0_3_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x100F,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0461->bits.bit_0_4_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1010,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0461->bits.bit_0_5_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1011,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0461->bits.bit_0_6_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1012,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0461->bits.bit_1_0_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1013,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0461->bits.bit_1_1_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1014,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0461->bits.bit_1_2_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1015,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0461->bits.bit_1_3_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1016,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0461->bits.bit_4_0_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1018,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0461->bits.bit_6_0_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x101B,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0461->bits.bit_6_1_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x101C,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0461->bits.bit_6_2_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x101D,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0461->bits.bit_6_3_1;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x101F,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 2;
                dw_data = pst_can_cycle_data_0461->bits.bit_5_0_3;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1118,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);
            }
		}break;
		case 0x046E:
		{
			switch(b_can_cmd)
			{
				case 0x0A:
				{

				}break;
				default:
				{

				}break;
			}
		}break;
		default:
		{

		}break;
	}

	return TRUE;
}
uint8_t data_convert_can_cycle_2_net_status_genii_bms(uint32_t dw_can_id,uint8_t b_can_cmd,uint8_t *pb_can_data_in,net_system_info_list *pst_info_list)
{
	if(NULL == pb_can_data_in)
	{
		return FALSE;
	}

	switch(dw_can_id)
	{
		case 0x0452:
		{
			can_cycle_data_0452_smp_bms *pst_can_cycle_data_0452 = 0;
			pst_can_cycle_data_0452 = (can_cycle_data_0452_smp_bms *)pb_can_data_in;

            {
                uint16_t w_data_len = 0;
                uint32_t dw_data = 0;

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0452->bits.bit_4_0_8;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1101,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 2;
                dw_data = pst_can_cycle_data_0452->bits.bit_6_0_16;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1102,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 2;
                dw_data = pst_can_cycle_data_0452->bits.bit_0_0_16;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1103,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 2;
                dw_data = pst_can_cycle_data_0452->bits.bit_2_0_16;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1104,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);
            }
		}break;
		case 0x0453:
		{
			can_cycle_data_0453_smp_bms *pst_can_cycle_data_0453 = 0;
			pst_can_cycle_data_0453 = (can_cycle_data_0453_smp_bms *)pb_can_data_in;

            {
                uint16_t w_data_len = 0;
                uint32_t dw_data = 0;

                w_data_len = 2;
                dw_data = (uint16_t)(pst_can_cycle_data_0453->bits.bit_5_0_1 << 0) |\
                          (uint16_t)(pst_can_cycle_data_0453->bits.bit_5_1_1 << 1) |\
                          (uint16_t)(pst_can_cycle_data_0453->bits.bit_5_2_1 << 2);
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1003,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 1;
                dw_data = pst_can_cycle_data_0453->bits.bit_4_0_8;
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1100,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);
            }
		}break;
		case 0x0454:
		{
			can_cycle_data_0454_smp_bms *pst_can_cycle_data_0454 = 0;
			pst_can_cycle_data_0454 = (can_cycle_data_0454_smp_bms *)pb_can_data_in;

            {
                uint16_t w_data_len = 0;
                uint32_t dw_data = 0;

                w_data_len = 2;
                dw_data = (uint16_t)(pst_can_cycle_data_0454->bits.bit_0_0_1 << 0) |\
						  (uint16_t)(pst_can_cycle_data_0454->bits.bit_0_1_1 << 1) |\
						  (uint16_t)(pst_can_cycle_data_0454->bits.bit_0_2_1 << 2) |\
						  (uint16_t)(pst_can_cycle_data_0454->bits.bit_0_3_1 << 3) |\
						  (uint16_t)(pst_can_cycle_data_0454->bits.bit_0_4_1 << 4) |\
						  (uint16_t)(pst_can_cycle_data_0454->bits.bit_0_5_1 << 5) |\
						  (uint16_t)(pst_can_cycle_data_0454->bits.bit_0_6_1 << 6) |\
						  (uint16_t)(pst_can_cycle_data_0454->bits.bit_0_7_1 << 7) |\
						  (uint16_t)(pst_can_cycle_data_0454->bits.bit_1_0_1 << 8);
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1001,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);

                w_data_len = 2;
                dw_data = (uint16_t)(pst_can_cycle_data_0454->bits.bit_2_0_1 << 0) |\
						  (uint16_t)(pst_can_cycle_data_0454->bits.bit_2_1_1 << 1) |\
						  (uint16_t)(pst_can_cycle_data_0454->bits.bit_2_2_1 << 2) |\
						  (uint16_t)(pst_can_cycle_data_0454->bits.bit_2_3_1 << 3) |\
						  (uint16_t)(pst_can_cycle_data_0454->bits.bit_2_4_1 << 4) |\
						  (uint16_t)(pst_can_cycle_data_0454->bits.bit_2_5_1 << 5) |\
						  (uint16_t)(pst_can_cycle_data_0454->bits.bit_2_6_1 << 6) |\
						  (uint16_t)(pst_can_cycle_data_0454->bits.bit_2_7_1 << 7) |\
						  (uint16_t)(pst_can_cycle_data_0454->bits.bit_3_0_1 << 8);
                net_device_data_access(cdw_device_id_SMP,
                                       cb_device_data_access_data_type_status,
                                       0x1002,
                                       &w_data_len,
                                       (uint8_t *)&dw_data,
                                       cb_device_data_access_operate_type_write);
            }
		}break;
		case 0x0455:
		{

		}break;
	}

	return TRUE;
}
uint8_t data_convert_can_cycle_2_net_status(uint32_t dw_can_id,uint8_t b_can_cmd,uint8_t *pb_can_data_in)
{
	uint16_t w_system_index = 0;

	{
		uint16_t w_cnt_1 = 0;

		for(w_cnt_1 = 0;w_cnt_1 < st_net_system_info_list.w_length;w_cnt_1++)
		{
			if(st_net_system_info_list.b_current_type ==\
			   st_net_system_info_list.pst_system_info[w_cnt_1].b_type)
			{
				w_system_index = w_cnt_1;
				break;
			}
		}

		if(w_cnt_1 >= st_net_system_info_list.w_length)
		{
			return FALSE;
		}
	}

	switch(st_net_system_info_list.pst_system_info[w_system_index].b_type)
	{
		case cb_system_type_GenII:
		{
			data_convert_can_cycle_2_net_status_smp(dw_can_id,b_can_cmd,pb_can_data_in,&st_net_system_info_list);
			data_convert_can_cycle_2_net_status_genii_bms(dw_can_id,b_can_cmd,pb_can_data_in,&st_net_system_info_list);
		}break;
	}

	return TRUE;
}
