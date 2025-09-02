#include "system_timestamp_generate.h"
#include "application_general_macro.h"
#include "freertos_user_config.h"
#include "net_system_info.h"
#include "network_protocal.h"
#include "net_system_info_list.h"
#include "net_system_info_access.h"
#include "application_general_func.h"
#include "printf_custom.h"
#include "freertos_user_config.h"
#include "net_device_tree_cklcd.h"
#include "network_device_config_sycn.h"
#include "rs485_communication_transmission.h"
#include "Bootloaderapp.H"
#include "net_device_data_access.h"
#include "net_device_tree_ck_inverter.h"
#include "led.h"
#include "task.h"

#include "net_firmware_update.h"
static uint32_t dw_system_timestamp = 0;
static TaskHandle_t timestamp_task_handle = 0;

uint8_t refresh_system_timestamp(uint32_t dw_timestamp)
{
    DebugPrintf(TRUE,"Timestamp refresh,timestamp:%d\r\n",dw_timestamp);
    dw_system_timestamp = dw_timestamp;

    return TRUE;
}

uint8_t get_system_timestamp(uint32_t *pdw_timestamp)
{
    if(0 == pdw_timestamp)
    {
        return FALSE;
    }

    *pdw_timestamp = dw_system_timestamp;

    return TRUE;
}

void refresh_timestamp()
{
    if(0 != dw_system_timestamp)
    {
        static TickType_t x_start_time = 0;
        TickType_t x_curent_time = 0;
        const TickType_t x_delay_time = pdMS_TO_TICKS_USER(1000);;
        x_curent_time = xTaskGetTickCount();

        if((x_curent_time - x_start_time) >= x_delay_time)
        {
            dw_system_timestamp += (x_curent_time - x_start_time)/x_delay_time;
            x_start_time = xTaskGetTickCount();
        }
    }
}

//由标准UTC时间生成Unix时间戳
uint8_t utc_struct_time_to_unix_time(const utc_time* pst_uct_time,uint32_t* unix_time)
{
    int32_t total_day,total_leap_year,dyear;
    int february_offset;//二月偏移量,零时变量
    //31, 28, 31, 30, 31, 30,    31, 31, 30, 31, 30, 31
    //每个月份对应前面所有月的天数
    const int month_day[]={0,31,59,90,120,151,181,212,243,273,304,334};

    //每天总秒数一定，将UTC时间（年月）转换成天数
    //为了减少额外闰年判断，把时间往前推到1600年，即闰年最大的一次公倍数开始计算判断
    //1970-1600 = 370 年 ，370/4 -(370/100-1)=90 个闰年
    //1600 DAY_OFFSET 365*(1970-1600)+90 = 135140,7为修正天数
    #define  YEAR_START  (1600)    //初始年份
    #define  DAY_OFFSET  (135140)  //时间偏移量
    #define  DAY_SECOND  (86400)

    if(pst_uct_time->tm_year<1970)
    {
        //暂不支持1970之前的时间
        *unix_time = 0;
        return FALSE;
    }
    if(pst_uct_time->tm_year>=3000)
    {
        //暂不支持3200及以后的时间
        *unix_time = 0;
        return FALSE;
    }

    //bug 计算总年数要去掉尾巴，如年数20年，那么实际应该4个闰年，因为20这一年没有包含在里面
    //bug 要减去一年来算闰年次数
    //先计算到相对1600年的天数，再转换到1970年
    dyear = pst_uct_time->tm_year - YEAR_START -1;
    total_leap_year =  dyear/4 - (dyear/100 - dyear/400 -1);
    //bug 恢复减去的一年
    dyear += 1 ;
    total_day = dyear * 365 + total_leap_year ;
    //减去1970到1600的总天数
    total_day -= DAY_OFFSET;

    //增加月和日的总天数
    //判断是否是闰年
    //能被4整除且不被100整除或者能被400整除
    if(((dyear%4 == 0)&&(dyear%100 != 0))||(dyear%400 == 0))
    {
        //闰年
        february_offset = 1;
    }
    else
    {
        february_offset = 0;
    }

    //计算含月和日的总天数,日期要减去当天
    total_day += month_day[pst_uct_time->tm_mon - 1] + pst_uct_time->tm_mday - 1 ;
    //二月以上需要加上偏移量
    if(pst_uct_time->tm_mon>1)
    {
        total_day += february_offset;
    }

    //存在bug,整型溢出
    //根据天数以及时分秒计算Unix时间戳
    *unix_time =\
        (uint32_t)total_day * DAY_SECOND + pst_uct_time->tm_hour * 3600 + pst_uct_time->tm_min *60 + pst_uct_time->tm_sec;

    return TRUE;
}

uint8_t get_device_timestamp(net_system_info_list *pst_net_system_info_list)
{
    if(0 != st_device_update_info_current.pst_net_device_update_info_current)
    {
        return FALSE;
    }
    net_device_info *pst_net_device_info = 0;
    if(TRUE == net_get_device_info(cdw_device_id_cklcd_u1,
                                   (net_device_info **)&pst_net_device_info,
                                   pst_net_system_info_list))
    {
        if(TRUE == net_check_device_communicate_status(cb_net_device_add,pst_net_device_info))
        {
          /*
            uint8_t ba_data[4] = {0x00,0xD5,0x00,0x04,};
            uint16_t w_data_len = 4;
            uint8_t b_receive_state = 0;
			if(TRUE == net_device_send_data(pst_net_device_info->b_wired_communication_cable,
											cb_wired_network_inquire_cmd,
											pst_net_device_info->dw_device_address_send,
											cw_device_cmd_cklcd_para_read,
											w_data_len,
											(uint8_t *)ba_data,
											pst_net_device_info))
			{
                DebugPrintf(TRUE,"Device timestamp inquire\r\n");
				for(;;)
				{
					if(TRUE == net_device_receive_data(pst_net_device_info->b_wired_communication_cable,
													   cb_wired_network_inquire_cmd,
													   pst_net_device_info->dw_device_address_receive,
													   cw_device_cmd_cklcd_para_read,
													   &w_data_len,
													   ba_data,
													   4,
													   &b_receive_state,
													   pst_net_device_info))
					{
						if(cb_wired_network_receive_state_success == b_receive_state)
						{
                            uint32_t dw_timestamp = 0;
                            if(TRUE == bytes_to_int_big_to_little_endain(ba_data,&dw_timestamp,4))
                            {
                                DebugPrintf(TRUE,"Device timestamp get,timestamp:%d\r\n",dw_timestamp);
                                refresh_system_timestamp(dw_timestamp);
                                return TRUE;
                            }
							break;
						}
						else if(cb_wired_network_receive_state_fail == b_receive_state)
						{
                            DebugPrintf(TRUE,"Device timestamp get fail\r\n");
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
        */
            uint32_t dw_timestamp = 0;
            {
                uint16_t w_data_len = 4;
                net_device_data_access(cdw_device_id_cklcd_u1,
                                       cb_device_data_access_data_type_set,
                                       0x00D5,
                                       &w_data_len,
                                       (uint8_t *)&dw_timestamp,
                                       cb_device_data_access_operate_type_read);
            }
            if(0 != dw_timestamp)
            {
                DebugPrintf(TRUE,"Device timestamp get,timestamp:%d\r\n",dw_timestamp);
                refresh_system_timestamp(dw_timestamp);
                return TRUE;
            }

        }
    }

    return FALSE;
}

uint8_t set_device_timestamp(uint32_t dw_timestamp)
{
    uint8_t ba_data[4] = {0,};
    uint16_t w_data_len = 4;

    if(0 != st_device_update_info_current.pst_net_device_update_info_current)
    {
        return FALSE;
    }

    if(FALSE == int_to_bytes_big_endain(&ba_data[0],dw_timestamp,4))
    {
        return FALSE;
    }
    if(cb_sync_trig_return_ok == network_device_config_sycn_trig(cdw_device_type_cklcd,
                                               cb_net_subset_com_set_cmd,
                                               0x00D5,
                                               w_data_len,
                                               ba_data,
                                               1,
                                               TRUE,
                                               sync_type_single))
    {
        DebugPrintf(TRUE,"Device timestamp set,timestamp:%d\r\n",dw_timestamp);
        return TRUE;
    }
    return FALSE;
}

void system_timestamp_manage(net_system_info_list *pst_net_system_info_list)
{
    #define cb_startup_delay                    0
    #define cb_get_device_timestamp             1
    #define cb_get_network_timestamp            2
    #define cb_wait_refresh_timestamp           3
    if(NULL == pst_net_system_info_list)
    {
        return;
    }
    uint32_t dw_timetamp = 0;
    static uint8_t b_timestamp_manage_step = cb_startup_delay;

    switch(b_timestamp_manage_step)
    {
        case cb_startup_delay:
        {
            vTaskDelay(pdMS_TO_TICKS_USER(5000));
            b_timestamp_manage_step = cb_get_device_timestamp;
        }
        break;
        case cb_get_device_timestamp:
        {
            if(FALSE == get_device_timestamp(pst_net_system_info_list))
            {
                b_timestamp_manage_step = cb_get_network_timestamp;
            }
            else
            {
                b_timestamp_manage_step = cb_wait_refresh_timestamp;
            }
        }
        break;
        case cb_get_network_timestamp:
        {
            enabke_get_network_timestamp(TRUE);
            b_timestamp_manage_step = cb_wait_refresh_timestamp;
        }
        break;
        case cb_wait_refresh_timestamp:
        {
            get_system_timestamp(&dw_timetamp);
            if(0 == dw_timetamp)
            {
                static TickType_t x_start_time = 0;
                TickType_t x_curent_time = 0;
                static TickType_t x_delay_time =  0;
                static uint8_t f_init = TRUE;

                if(f_init)
                {
                    f_init = FALSE;
                    x_start_time = xTaskGetTickCount();
                    x_delay_time =  pdMS_TO_TICKS_USER(10000);
                }
                x_curent_time = xTaskGetTickCount();
                if((x_curent_time - x_start_time) >= x_delay_time)
                {
                    b_timestamp_manage_step = cb_get_device_timestamp;
                    f_init = TRUE;
                }
            }
            else
            {
                static uint8_t b_inv_num = 0;
                uint8_t b_inv_num_temp = 0;
                uint16_t w_data_len = 1;

                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                       cb_net_data_att_status,
                                       0x0160,
                                       &w_data_len,
                                       (uint8_t *)&b_inv_num_temp,
                                       cb_device_data_access_operate_type_read);//invnum

                if(((3600*24 - (dw_timetamp % (3600*24))) == 1800) ||\
                   (b_inv_num_temp > b_inv_num))
                {
                    b_timestamp_manage_step = cb_get_network_timestamp;
                }
                else if((dw_timetamp % 3600) == 0)
                {
                    b_timestamp_manage_step = cb_get_device_timestamp;
                }
                b_inv_num = b_inv_num_temp;
            }
        }
        break;
        default:
            break;
    }
}
void system_reset_protect()
{
    static uint32_t dw_timestamp_backup = 0;

    uint32_t dw_FreeBytesRemaining = 0;

    dw_FreeBytesRemaining = xPortGetFreeHeapSize();

    if(dw_FreeBytesRemaining < 9000)
    {
        if(0 == dw_timestamp_backup)
        {
            dw_timestamp_backup = dw_system_timestamp;
        }
        else if(dw_system_timestamp - dw_timestamp_backup > 60)
        {
            NVIC_SystemReset();
        }

        #if( system_alarm_print )
        DebugPrintf(TRUE,"[Alarm]System Protect,Timestamp:%d,BytesRemain:%d\r\n",dw_timestamp_backup,dw_FreeBytesRemaining);
        #endif
    }
    else
    {
        dw_timestamp_backup = 0;
    }
}
//#define FREERTOS_TASK_RUN_STATE_TEST        FALSE
void vSystemTimestampTask(void* parameter)
{
	DebugPrintf(TRUE,"%d,vEnergyStatisticsTask_first_entry\n",__LINE__);

    for(;;)
    {
		#if(task_call_rate_test)
		DebugPrintf(TRUE,"vSystemTimestampTask,start: %d,priority: %d\n",xTaskGetTickCount(),TIMESTAMP_TASK_PRIORITY);
		#endif

        system_timestamp_manage(&st_net_system_info_list);
        refresh_timestamp();
        MACRO_IWDG_RELOD();
        system_reset_protect();
        led_flash();

        #if(FREERTOS_TASK_RUN_STATE_TEST)
		{
			static uint8_t pcWriteBuffer[1000] = {0,};
			vTaskList((char *)pcWriteBuffer);
			DebugPrintf(TRUE,"\r\n%s\r\n",pcWriteBuffer);

			vTaskGetRunTimeStats((char *)pcWriteBuffer);
			DebugPrintf(TRUE,"\r\n%s\r\n",pcWriteBuffer);
		}
		#endif

		#if(task_call_rate_test)
		DebugPrintf(TRUE,"vSystemTimestampTask,end: %d,priority: %d\n",xTaskGetTickCount(),TIMESTAMP_TASK_PRIORITY);
		#endif

		vTaskDelay(pdMS_TO_TICKS_USER(100));
    }
}

void system_timestamp_task_init()
{
    BaseType_t xReturn = pdPASS;

    xReturn = xTaskCreate( vSystemTimestampTask,"vSystemTimestampTask",timestamp_task_stack_size,NULL,timestamp_task_priority,&timestamp_task_handle);
    if(pdFAIL == xReturn)
    {
        DebugPrintf(TRUE,"%d,vSystemTimestampTask Create Fail\n",__LINE__);
    }
}
