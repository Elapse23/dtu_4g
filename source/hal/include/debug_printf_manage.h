#ifndef _DEBUG_PRINTF_MANAGE_H_
#define _DEBUG_PRINTF_MANAGE_H_

/************ General Config *************/

#define local_offline_ble_print                                 TRUE

//Print All Wireless Moudule Uart data
#define lte_tranmist_data_test                                  FALSE

//Print Part Task Runing Status
#define task_runing_status_print                                FALSE

//Print System Init status
#define system_init_status_print                                FALSE

//Print Device Sync Status
#define device_sync_status_print                                FALSE

//Print system alarm
#define system_alarm_print                                      FALSE

//Print Wireless module status
#define wireless_status_print                                   FALSE

//Print Firmware update status
#define update_status_print                                     FALSE

//Eventlog status print
#define event_status_print                                      FALSE

//Print Device Data
#define r485_transmission_print                                 FALSE

//Print Device Data
#define power_saving_print                                      FALSE

//和服务器通信的数据打印
#define netwrok_status_print                                    TRUE

//构建设备树的打印
#define device_tree_print                                       FALSE

//网络数据发送打印
#define net_data_send_print                                     TRUE

//任务堆栈剩余空间打印
#define task_stack_high_water_mark                              FALSE

//蓝牙数据发送打印
#define ble_data_send_print                                     FALSE
/************ Local Config *************/
#define net_check_device_communicate_status_Semaphore_test      FALSE
#define task_call_rate_test                                     FALSE

#define net_inquire_device_tree_Semaphore_test					FALSE



#endif



