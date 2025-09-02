#include "n32g45x_gpio.h"
#include "portmacro.h"
#include "projdefs.h"
#include "freertos_user_config.h"
#include "rs485.h"
#include "IICDriver.h"
#include "drv_spiflash.h"
#include "usart_config_lte.h"
#include "usart_config_log.h"
#include "iot_module_config_common.h"
#include "application_general_macro.h"
#include "network_iot_module_data_channel_manage.h"
#include "lte_module_data_queue.h"
#include "printf_custom.h"
#include "network_communication.h"
#include "lte_module_config_queue.h"
#include "Eeprom.h"
#include "spi_flash_operate.h"
#include "system_timestamp_generate.h"
#include "rs485_communication_transmission.h"
#include "display_management.h"
#include "network_energy_statistics.h"
#include "network_tbb_device_server_communicate.h"
#include "network_4g_tbb_device_server_communicate.h"
#include "network_ble_communication.h"
#include "network_local_firmware_update.h"
#include "BootLoaderAPP.H"
#include "lte_module.h"
//#include "lte_4g_module.h"
#include "network_system_type_operate.h"
#include "lte_ftp_updata.h"
static void network_iot_module_config_excute_task(void* parameter);
static void network_communicate_task(void* parameter);
//static void wireless_communicate_receive_data_task(void* parameter);
//static void wireless_module_send_data_background_task(void* parameter);
static void wireless_module_receive_data_background_task(void* parameter);
static void device_tree_refresh_task(void* parameter);
static void device_data_polling_task(void* parameter);
static void RS485_queue_recognition_conversion_task(void* parameter);
static void wireless_module_receive_iot_data_task(void* parameter);



int main(void)
{
	__set_PRIMASK(1);                                  // 关闭所有中断
	__NVIC_SetPriorityGrouping(0x03);                  // 设置中断分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);    // 设置中断分组

	rs485_config();              // 485 相关端口配置
	rs485_transmission_init();   // 485 一些互斥锁的初始化
	usart_lte_config();          // 4G模块串口配置
	usart_log_config();          // 日志串口配置
	sIICPinInit();               // IIC引脚配置
	drv_spi_flash_init();        // spi flash 端口初始化
	spi_flash_operate_init();    // spi flash 操作初始化 创建了 spi_flash_operate_process_task 任务
	flash_erase_sector_all_task_start();   // 擦除flash ftp升级所有扇区任务 创建了 flash_erase_sector_all_task 任务

	debug_printf_init();                  // 调试打印初始化 创建一个互斥量 pst_printf_mutex
	debug_updata_info("app version-");    // 打印版本号
	debug_updata_sector(( uint8_t * )bFrimwareVerStr, 3);    // 打印版本号
	sEepromReadToRam();                                      // 读取eeprom数据到ram

	BaseType_t xReturn = pdPASS;

	uint8_t b_type = iot_module_type_4g;
	iot_module_detect();                             // 将 iot 模块类型初始化为 4G模块
	network_communication_initial();                 // 初始化网络通信
	network_iot_module_connection_info_initial();    // 初始化网络iot模块连接信息
	system_timestamp_task_init();                    // 初始化时间戳任务 创建了 vSystemTimestampTask 任务
	energy_statistics_task_init();    // 初始化能耗统计任务 创建了 vEnergyStatisticsTask 任务

	if (TRUE == iot_module_get_type(&b_type))
	{
		if (iot_module_type_ble == b_type)
		{
		}
		else if (iot_module_type_lte == b_type)
		{
			lte_module_queue_init();                  // 初始化wifi模块队列
			lte_module_driver_config_queue_init();    // 初始化wifi模块配置队列
			lte_module_driver_initial();              // 初始化wifi模块驱动
		}
		else if (iot_module_type_4g == b_type)
		{
			lte_module_queue_init();                  // 初始化4G模块队列
			lte_module_driver_config_queue_init();    // 初始化4G模块配置队列
			lte_4g_module_driver_initial();           // 初始化4G模块驱动
		}
	}

	xReturn = xTaskCreate(network_iot_module_config_excute_task,    // 操作wifi 蓝牙 4G模块 并进行第一次解析数据 HAL层
	                      "network_iot_module_config_excute_task",
	                      network_iot_module_config_excute_task_stack_size,
	                      NULL,
	                      network_iot_module_config_excute_task_priority,
	                      &network_iot_module_config_excute_task_handle);
	if (pdFAIL == xReturn)
	{
		DebugPrintf(TRUE, "%d,lte_module_nmea_info_get_task Create Fail\n", __LINE__);
	}
	xReturn = xTaskCreate(RS485_queue_recognition_conversion_task,    // 操作wifi 蓝牙 4G模块 并进行第一次解析数据 HAL层
	                      "RS485_queue_recognition_conversion_task",
	                      RS485_queue_recognition_conversion_task_size,
	                      NULL,
	                      RS485_queue_recognition_conversion_task_priority,
	                      &RS485_queue_recognition_conversion_task_handle);
	if (pdFAIL == xReturn)
	{
		DebugPrintf(TRUE, "%d,RS485_queue_recognition_conversion_task Create Fail\n", __LINE__);
	}

	// local_firmware_info_initial();
	firmware_update_task_init();       // 初始化固件升级任务 创建了 vFirmwareUpdateTask 任务
	display_management_task_init();    // 初始化灯显管理任务 创建了 vDisplayManagementTask 任务

	xReturn = xTaskCreate(network_communicate_task,    // 网络通信任务 解析无线数据 应用层
	                      "network_communicate_task",
	                      network_communicate_task_stack_size,
	                      NULL,
	                      network_communicate_task_priority,
	                      &network_communicate_task_handle);
	if (pdFAIL == xReturn)
	{
		DebugPrintf(TRUE, "%d,network_communicate_task Create Fail\n", __LINE__);
	}
	network_ble_communication_task_init();    // 初始化蓝牙通信任务 创建了 network_ble_communication_task 任务

	//		xReturn = xTaskCreate(wireless_communicate_receive_data_task,
	//							  "wireless_communicate_receive_data_task",
	//							  wireless_communicate_receive_data_task_stack_size,
	//							  NULL,
	//							  wireless_communicate_receive_data_task_priority,
	//							  &wireless_communicate_receive_data_task_handle);
	//		if(pdFAIL == xReturn)
	//		{
	//			DebugPrintf(TRUE,"%d,vTestTask_1 Create Fail\n",__LINE__);
	//		}

	//		xReturn = xTaskCreate( wireless_module_send_data_background_task,
	//							  "wireless_module_send_data_background_task",
	//							  wireless_module_send_data_background_task_stack_size,
	//							  NULL,
	//							  wireless_module_send_data_background_task_priority,
	//							  &wireless_module_send_data_background_task_handle);
	//		if(pdFAIL == xReturn)
	//		{
	//			DebugPrintf(TRUE,"%d,wireless_module_send_data_background_task Create Fail\n",__LINE__);
	//		}

	xReturn = xTaskCreate(wireless_module_receive_data_background_task,    // 无线模块接收数据任务 驱动层
	                      "wireless_module_receive_data_background_task",
	                      wireless_module_receive_data_background_task_stack_size,
	                      NULL,
	                      wireless_module_receive_data_background_task_priority,
	                      &wireless_module_receive_data_background_task_handle);
	if (pdFAIL == xReturn)
	{
		DebugPrintf(TRUE, "%d,wireless_module_receive_data_background_task Create Fail\n", __LINE__);
	}
	xReturn = xTaskCreate(wireless_module_receive_iot_data_task,    // 无线模块接收数据任务 驱动层
	                      "wireless_module_receive_iot_data_task",
	                      wireless_module_receive_iot_data_task_stack_size,
	                      NULL,
	                      wireless_module_receive_iot_data_task_priority,
	                      &wireless_module_receive_iot_data_task_handle);
	if (pdFAIL == xReturn)
	{
		DebugPrintf(TRUE, "%d,wireless_module_receive_iot_data_task Create Fail\n", __LINE__);
	}
	rs485_transmission_task_init();    // 初始化485通信任务 创建了 vRs485TxDataTask 任务

	xReturn = xTaskCreate(device_data_polling_task,    // 485设备数据轮询解析任务 应用层
	                      "device_data_polling_task",
	                      device_data_polling_task_stack_size,
	                      NULL,
	                      device_data_polling_task_priority,
	                      &device_data_polling_task_handle);
	if (pdFAIL == xReturn)
	{
		DebugPrintf(TRUE, "%d,device_data_polling_task Create Fail\n", __LINE__);
	}

	xReturn = xTaskCreate(device_tree_refresh_task,    // 485设备树刷新任务 应用层
	                      "device_tree_refresh_task",
	                      device_tree_refresh_task_stack_size,
	                      NULL,
	                      device_tree_refresh_task_priority,
	                      &device_tree_refresh_task_handle);
	if (pdFAIL == xReturn)
	{
		DebugPrintf(TRUE, "%d,device_tree_refresh_task Create Fail\n", __LINE__);
	}
	/* Start the scheduler. */
	vTaskStartScheduler();    // 启动调度器
}

static void RS485_queue_recognition_conversion_task(void* parameter)
{
	DebugPrintf(TRUE,"%d,RS485_queue_recognition_conversion_task_first_entry\n",__LINE__);

    for(;;)
    {

		rs485_queue_recognition_conversion();
        vTaskDelay(pdMS_TO_TICKS_USER(50));
    }
}



static void network_iot_module_config_excute_task(void* parameter)
{
	DebugPrintf(TRUE,"%d,iot_module_config_excute_task_first_entry\n",__LINE__);

    for(;;)
    {
		#if(task_call_rate_test)
		DebugPrintf(TRUE,"iot_module_config_excute_task,start: %d,priority: %d\n",xTaskGetTickCount(),iot_module_config_excute_task_priority);
		#endif

        network_iot_module_communicate_channel_config_excute();

		#if(task_call_rate_test)
		DebugPrintf(TRUE,"iot_module_config_excute_task,end: %d,priority: %d\n",xTaskGetTickCount(),iot_module_config_excute_task_priority);
		#endif

        vTaskDelay(pdMS_TO_TICKS_USER(50));
    }
}
static void network_communicate_task(void* parameter)
{
	DebugPrintf(TRUE,"%d,network_communicate_task first_entry\n",__LINE__);

    for(;;)
    {
		#if(task_call_rate_test)
		DebugPrintf(TRUE,"network_communicate_task,start: %d,priority: %d\n",xTaskGetTickCount(),wireless_communicate_send_task_priority);
		#endif

		network_communication();
        sEepromTimingFuncs();

		#if(task_call_rate_test)
		DebugPrintf(TRUE,"network_communicate_task,end: %d,priority: %d\n",xTaskGetTickCount(),wireless_communicate_send_task_priority);
		#endif

        vTaskDelay(pdMS_TO_TICKS_USER(20));
    }
}
//static void wireless_communicate_receive_data_task(void* parameter)
//{
//	DebugPrintf(TRUE,"%d,wireless_communicate_receive_data_task_first_entry\n",__LINE__);
//
//    for(;;)
//    {
//		#if(task_call_rate_test)
//		DebugPrintf(TRUE,"wireless_communicate_receive_data_task,start: %d,priority: %d\n",xTaskGetTickCount(),wireless_communicate_receive_data_task_priority);
//		#endif
//
////		if(TRUE == iot_module_response_analysis_general())
////		{
////			//network_data_receive();
////		}
//
//		#if(task_call_rate_test)
//		DebugPrintf(TRUE,"wireless_communicate_receive_data_task,end: %d,priority: %d\n",xTaskGetTickCount(),wireless_communicate_receive_data_task_priority);
//		#endif
//
//        vTaskDelay(pdMS_TO_TICKS_USER(5));//10
//    }
//}
//static void wireless_module_send_data_background_task(void* parameter)
//{
//	DebugPrintf(TRUE,"%d,wireless_module_send_data_background_task_first_entry\n",__LINE__);
//
//    for(;;)
//    {
//		#if(task_call_rate_test)
//		DebugPrintf(TRUE,"wireless_module_send_data_background_task,start: %d,priority: %d\n",xTaskGetTickCount(),wireless_module_send_data_background_task_priority);
//		#endif
//
//        iot_module_send_data_background();
//
//
//		#if(task_call_rate_test)
//		DebugPrintf(TRUE,"wireless_module_send_data_background_task,end: %d,priority: %d\n",xTaskGetTickCount(),wireless_module_send_data_background_task_priority);
//		#endif
//
//        vTaskDelay(pdMS_TO_TICKS_USER(5));
//    }
//}
extern TickType_t lte_system_tick;
extern uint8_t start_one_flag;
extern uint8_t auto_disconnect_state;
static void wireless_module_receive_data_background_task(void* parameter)
{
	DebugPrintf(TRUE,"%d,wireless_module_receive_data_background_task_first_entry\n",__LINE__);

    for(;;)
    {
		#if(task_call_rate_test)
		DebugPrintf(TRUE,"wireless_module_receive_data_background_task,start: %d,priority: %d\n",xTaskGetTickCount(),wireless_module_receive_data_background_task_priority);
		#endif
//		iot_module_receive_data_background();
        rs485_rx_data_background();

		#if(task_call_rate_test)
		DebugPrintf(TRUE,"wireless_module_receive_data_background_task,end: %d,priority: %d\n",xTaskGetTickCount(),wireless_module_receive_data_background_task_priority);
		#endif

        vTaskDelay(pdMS_TO_TICKS_USER(10));  //10
    }
}
size_t myPortGetMinimumEverFreeHeapSize;
static void wireless_module_receive_iot_data_task(void* parameter)
{
	DebugPrintf(TRUE,"%d,wireless_module_receive_iot_data_task_first_entry\n",__LINE__);

    for(;;)
    {
		#if(task_call_rate_test)
		DebugPrintf(TRUE,"wireless_module_receive_iot_data_task,start: %d,priority: %d\n",xTaskGetTickCount(),wireless_module_receive_iot_data_task_priority);
		#endif
		iot_module_receive_data_background();
		network_iot_queue_recognition_conversion();
		#if(task_call_rate_test)
		DebugPrintf(TRUE,"wireless_module_receive_iot_data_task,end: %d,priority: %d\n",xTaskGetTickCount(),wireless_module_receive_iot_data_task_priority);
		#endif

		if((xTaskGetTickCount() - lte_system_tick > pdMS_TO_TICKS_USER(17000)) && start_one_flag == 1 && auto_disconnect_state == 0)
		{
			// NVIC_SystemReset();
		}

//		if (myPortGetMinimumEverFreeHeapSize != xPortGetMinimumEverFreeHeapSize())
//        {
//            yl_debug_printf("Free Heap Size: %d decrease: %d ---------------------\r\n", xPortGetMinimumEverFreeHeapSize(),myPortGetMinimumEverFreeHeapSize - xPortGetMinimumEverFreeHeapSize());
//            myPortGetMinimumEverFreeHeapSize = xPortGetMinimumEverFreeHeapSize();
//        }

        vTaskDelay(pdMS_TO_TICKS_USER(5));  //10
    }
}

static void device_tree_refresh_task(void* parameter)
{
	DebugPrintf(TRUE,"%d,device_tree_refresh_task_first_entry\n",__LINE__);

    for(;;)
    {
		#if(task_call_rate_test)
		DebugPrintf(TRUE,"device_tree_refresh_task,start: %d,priority: %d\n",xTaskGetTickCount(),device_tree_refresh_task_priority);
		#endif

//		ulTaskNotifyTake(TRUE,portMAX_DELAY);

        network_device_tree_refresh();		//net_inquire_device_tree
        device_id_init();
        network_system_init();

		#if(task_call_rate_test)
		DebugPrintf(TRUE,"device_tree_refresh_task,end: %d,priority: %d\n",xTaskGetTickCount(),device_tree_refresh_task_priority);
		#endif

        vTaskDelay(pdMS_TO_TICKS_USER(15000));
    }
}
static void device_data_polling_task(void* parameter)
{
	DebugPrintf(TRUE,"%d,device_data_polling_task_first_entry\n",__LINE__);

    for(;;)
    {
		#if(task_call_rate_test)
		DebugPrintf(TRUE,"device_data_polling_task,start: %d,priority: %d\n",xTaskGetTickCount(),device_tree_refresh_task_priority);
		#endif

//		ulTaskNotifyTake(TRUE,portMAX_DELAY);

        network_device_data_polling();

		#if(task_call_rate_test)
		DebugPrintf(TRUE,"device_data_polling_task,end: %d,priority: %d\n",xTaskGetTickCount(),device_tree_refresh_task_priority);
		#endif

        if (start_one_flag == 0)
        {
            vTaskDelay(pdMS_TO_TICKS_USER(10));    // min 300ms in riiosunii system
        }
        else
        {
            vTaskDelay(pdMS_TO_TICKS_USER(50));    // min 300ms in riiosunii system  50
        }
    }
}
