#include "network_local_firmware_update.h"
#include "spi_flash_address_allocate.h"
//#include "Bootloader.h"
#include "Bootloaderapp.H"
#include "printf_custom.h"
#include "spi_flash_operate.h"
#include "application_general_func.h"
#include "crc16_arithmetic.h"
#include "extern_flash_operate.h"
#include "net_device_data_access.h"
#include "FreeRTOS.h"
#include "task.h"
#include "net_device_tree_kinergy_ii.h"
#include "Eeprom.h"
#include "net_firmware_update.h"
#include "net_system_info_access.h"
#include "rs485.h"
#include "rs485_communication_transmission.h"
#include "rs485_communicate_protocal.h"
#include "net_device_tree_system.h"
#include "display_management.h"
#include "network_system_type_operate.h"
#include "network_tbb_device_server_communicate.h"

#include <string.h>
#include <ysizet.h>




extern void * pvPortMalloc( size_t xWantedSize );
extern void vPortFree( void * pv );

static TaskHandle_t firmware_update_task_handle = 0;
extern TaskHandle_t energy_statistics_task_handle;

#define cw_local_firmware_info_version                  0x1000
#define cb_max_subset_firmware_format                   3
#define cb_max_subset_update_tx_cmd_len                 300
#define cb_max_subset_update_rx_cmd_len                 50

uint32_t dw_firmware_info_spi_flash_addr = cdw_spi_flash_addr_alloc_firmware_info_start;
uint32_t dw_firmware_data_spi_flash_addr = cdw_spi_flash_addr_alloc_local_firmware_data_start;
uint8_t b_local_firmware_enable = FALSE;
uint32_t dw_firmware_update_task_delay = 100;
uint8_t b_pac_addr = 0xFE;
uint8_t gb_mppt_pac_enable_addr = 0xF7;
uint8_t gb_update_firware_fail_flag = 0;
uint8_t gb_iner_mppt_id = 0;
uint8_t subset_update_firmware_format_pac(net_device_info *pst_net_device_info);
uint8_t subset_update_firmware_format_pacx_mcu(net_device_info *pst_net_device_info);
uint8_t subset_update_firmware_format_pacx_dsp(net_device_info *pst_net_device_info);

typedef uint8_t (*pfun_subset_update_process_func)(net_device_info *pst_net_device_info);

typedef union
{
    uint8_t ba_data[4];
    struct
    {
        uint8_t b_firmware_info_version[2];
        uint8_t b_firmware_info_intact[2];
    }info;
}firmware_info;

typedef enum
{
    cb_subset_update_none       ,
    cb_subset_update_wait       ,
    cb_subset_update_com_fail   ,
    cb_subset_update_com_success,
    cb_subset_update_success    ,
    cb_subset_update_fail       ,
}subset_update_state;

typedef struct
{
    uint16_t w_update_per;
    uint16_t w_update_state;
    struct
    {
        uint16_t w_data_len;
        uint8_t ba_data[cb_max_subset_update_tx_cmd_len];
    }st_subset_update_tx_info;
    struct
    {
        uint16_t w_data_len;
        uint8_t ba_data[cb_max_subset_update_rx_cmd_len];
    }st_subset_update_rx_info;
}subset_update_info;

static subset_update_info st_subset_update_info = {0,};
static uint8_t ba_flash_data_read[600] = {0,};

const uint8_t cba_cklcd_com_id[9] =
{
    cdw_rs485_id_u1,
    cdw_rs485_id_u2,
    cdw_rs485_id_u3,
    cdw_rs485_id_v1,
    cdw_rs485_id_v2,
    cdw_rs485_id_v3,
    cdw_rs485_id_w1,
    cdw_rs485_id_w2,
    cdw_rs485_id_w3,
};

const pfun_subset_update_process_func subset_update_process_func[cb_max_subset_firmware_format] =
{
    subset_update_firmware_format_pac,
    subset_update_firmware_format_pacx_mcu,
    subset_update_firmware_format_pacx_dsp,
};

void local_firmware_info_initial()
{
    firmware_info st_firmware_info;
	firmware_info st_firmware_info_backup;

    for(;;)
    {
  		if(TRUE == extern_flash_read(dw_firmware_info_spi_flash_addr,
							         4,
							         st_firmware_info.ba_data))
        {
            if((0xFFFF == ((uint16_t)(st_firmware_info.info.b_firmware_info_version[0] << 0) |
                           (uint16_t)(st_firmware_info.info.b_firmware_info_version[1] << 8))) &&
               (0xFFFF == ((uint16_t)(st_firmware_info.info.b_firmware_info_intact[0] << 0) |
                           (uint16_t)(st_firmware_info.info.b_firmware_info_intact[1] << 8))))
			{
                break;
            }
            else
            {
				memcpy(st_firmware_info_backup.ba_data,st_firmware_info.ba_data,4);
                dw_firmware_info_spi_flash_addr += 4;
            }
        }
        else
        {
            //spi flash error
            break;
        }
    }

	if((cw_local_firmware_info_version == ((uint16_t)(st_firmware_info_backup.info.b_firmware_info_version[0] << 0) |
                                           (uint16_t)(st_firmware_info_backup.info.b_firmware_info_version[1] << 8))) &&
	   (0x0001 == ((uint16_t)(st_firmware_info_backup.info.b_firmware_info_intact[0] << 0) |
		   		   (uint16_t)(st_firmware_info_backup.info.b_firmware_info_intact[1] << 8))))
	{
		b_local_firmware_enable = TRUE;
	}
}

//uint8_t local_firmware_info_refresh(uint8_t b_firmware_intact)
//{
//    firmware_info st_firmware_info;
//
//    if(dw_firmware_info_spi_flash_addr >= (cdwLocalUpdateFirmAddrStart + cdwLocalUpdateFirmInfoLen))
//    {
//        dw_firmware_info_spi_flash_addr = cdwLocalUpdateFirmAddrStart;
//        if(FALSE == spi_flash_erase(cdwLocalUpdateFirmAddrStart,0))
//        {
//            //spi flash error
//            return FALSE;
//        }
//    }
//
//	st_firmware_info.info.b_firmware_info_version[0] = (uint8_t)(cw_local_firmware_info_version & 0x00FF);
//	st_firmware_info.info.b_firmware_info_version[1] = (uint8_t)((cw_local_firmware_info_version & 0xFF00) >> 8);
//	st_firmware_info.info.b_firmware_info_intact[0] = 0x00;
//	st_firmware_info.info.b_firmware_info_intact[1] = b_firmware_intact;
//
//    if(FALSE == spi_flash_write(dw_firmware_info_spi_flash_addr,
//                                4,
//                                st_firmware_info.ba_data))
//    {
//        //spi flash error
//        return FALSE;
//    }
//
//    dw_firmware_info_spi_flash_addr += 4;
//
//	if(TRUE == b_firmware_intact)
//	{
//		b_local_firmware_enable = TRUE;
//	}
//	else
//	{
//		b_local_firmware_enable = FALSE;
//	}
//
//    return TRUE;
//}

uint8_t local_firmware_info_check()
{
	if(TRUE == b_local_firmware_enable)
	{
		//local_firmware_info_refresh(FALSE);
        b_local_firmware_enable = FALSE;
		fWriteEEPage(15) = TRUE;
		return TRUE;
	}
    return FALSE;
}

uint8_t local_firmware_verify()
{
    uint32_t dw_data_address = cdwLocalUpdateFirmAddrStart + cdwLocalUpdateFirmInfoLen;
    uint32_t dw_packet_number_total = 0;
    uint32_t dw_packet_number_backup = 0;
    uint32_t dw_packet_number_current = 0;
    uint16_t w_packet_data_len = 0;
    uint8_t *pb_firmware_packet = 0;
    LocalUpdateSpiFlashFirmwarePacket *pst_firmware_packet = 0;

    do
    {
        pb_firmware_packet = (uint8_t *)pvPortMalloc(cdwLocalUpdateFirmDataLen * sizeof(uint8_t));
        vTaskDelay(0);
    }while(NULL == pb_firmware_packet);

//    {
//        #if( system_alarm_print)
//        DebugPrintf(TRUE,"[ERROR] malloc err:1\r\n");
//        #endif
//        return ;
//    }
    #if(dynamic_memory_request_test)
    DebugPrintf(TRUE,"local_firmware_verify_0_malloc%x\n",pb_firmware_packet);
    #endif

    do{
        if(TRUE == extern_flash_read(dw_data_address,
                                     cdwLocalUpdateFirmDataLen,
                                     pb_firmware_packet))
        {
            pst_firmware_packet = (LocalUpdateSpiFlashFirmwarePacket *)pb_firmware_packet;
            dw_packet_number_current = *((uint32_t*)(pst_firmware_packet->SpiFlashPacket.bPacketIndex));
            //bytes_to_int_little_to_little_endain(pst_firmware_packet->SpiFlashPacket.bPacketIndex,&dw_packet_number_current,4);
            //memcpy((uint8_t*)&dw_packet_number_current,pst_firmware_packet->SpiFlashPacket.bPacketIndex,4);
            //bytes_to_int_little_to_little_endain(pst_firmware_packet->SpiFlashPacket.bPacketDataLen,(uint32_t *)&w_packet_data_len,2);
            w_packet_data_len = *((uint16_t*)(pst_firmware_packet->SpiFlashPacket.bPacketDataLen));
            if((dw_packet_number_backup + 1) == dw_packet_number_current)
            {
                dw_packet_number_backup = dw_packet_number_current;
            }
            else
            {
                vPortFree(pb_firmware_packet);
                #if(dynamic_memory_request_test)
                DebugPrintf(TRUE,"local_firmware_verify_0_free%x\n",pb_firmware_packet,);
                #endif
                debug_updata_error("[updata]current %d back %d len%d\r\n",dw_packet_number_current,dw_packet_number_backup,w_packet_data_len);
                debug_updata_sector(pst_firmware_packet->SpiFlashPacket.bPacketIndex,4);
                debug_updata_sector(pst_firmware_packet->SpiFlashPacket.bPacketDataLen,2);
                return FALSE;
            }

            if(1 == dw_packet_number_current)
            {
                bytes_to_int_little_to_little_endain(pst_firmware_packet->SpiFlashPacket.Packet.info.bPacketsNumber,
                                                     &dw_packet_number_total,4);
            }



            if(FALSE == check_crc16(pst_firmware_packet->SpiFlashPacket.Packet.bData,w_packet_data_len))
            {
                vPortFree(pb_firmware_packet);
                #if(dynamic_memory_request_test)
                DebugPrintf(TRUE,"local_firmware_verify_1_free%x\n",pb_firmware_packet);
                #endif
                return FALSE;
            }

            dw_data_address += w_packet_data_len + 6;
        }
      }
    while(dw_packet_number_current < dw_packet_number_total);

    vPortFree(pb_firmware_packet);
    #if(dynamic_memory_request_test)
    DebugPrintf(TRUE,"local_firmware_verify_2_free%x\n",pb_firmware_packet);
    #endif
    return TRUE;
}

/* -------------------------------------------------------------------------- */
/*                                flash读取固件包总数                                */
/* -------------------------------------------------------------------------- */
extern uint8_t ble_ftp_updata_flag;
uint8_t read_send_subset_packet_num(uint16_t w_max_subset_package_len,
                                    uint32_t *pdw_subset_package_sum)
{
    uint32_t dw_data_address = cdw_spi_flash_addr_alloc_local_firmware_data_start + 1;
    if(ble_ftp_updata_flag == 1)
    {
        dw_data_address = cdw_ble_spi_flash_addr_alloc_local_firmware_data_start + 1;
    }
    uint32_t dw_packet_number_total = 0;
    uint32_t dw_packet_number_backup = 0;
    uint32_t dw_packet_number_current = 0;
    uint16_t w_packet_data_len = 0;

    uint8_t ba_flash_read_buff[10] = {0,};
    uint32_t dw_subset_package_sum_temp = 0;

    do{
        if(TRUE == extern_flash_read(dw_data_address,
                                     10,
                                     ba_flash_read_buff))
        {
            dw_packet_number_current = *((uint32_t*)(&ba_flash_read_buff[0]));
            w_packet_data_len = *((uint16_t*)(&ba_flash_read_buff[4]));
            if((dw_packet_number_backup + 1) == dw_packet_number_current)
            {
                dw_packet_number_backup = dw_packet_number_current;
            }
            else
            {
                return FALSE;
            }

            if(1 == dw_packet_number_current)
            {
                bytes_to_int_little_to_little_endain(&ba_flash_read_buff[6],
                                                     &dw_packet_number_total,4);
                dw_subset_package_sum_temp += ((w_packet_data_len - 6) / w_max_subset_package_len);
            }
            else if(0 != ((w_packet_data_len - 6) % w_max_subset_package_len))
            {
                dw_subset_package_sum_temp +=\
                    ((w_packet_data_len - 6) / w_max_subset_package_len) + 1;
            }
            else
            {
                dw_subset_package_sum_temp +=\
                    ((w_packet_data_len - 6) / w_max_subset_package_len);
            }

            dw_data_address += w_packet_data_len + 6;
        }
      }
    while(dw_packet_number_current < dw_packet_number_total);

    *pdw_subset_package_sum = dw_subset_package_sum_temp;

    return TRUE;
}

void local_firmware_update()
{
    DebugPrintf(TRUE,"local_firmware_update into boot\r\n");
    stUpdateUARTInfo.fAppSPIFLASHICP = TRUE;
    stUpdateUARTInfo.fAppUARTICP  = cbUpdateFromFlash;
    ((psVoidFunc)(dwBootExtTbl[cbBootLoaderStart]))();
}

void get_queue_mutex()
{
	extern SemaphoreHandle_t pst_rs485_tx_data_queue_mutex;
	extern SemaphoreHandle_t pst_rs485_rx_polling_data_queue_mutex;
	extern SemaphoreHandle_t pst_rs485_rx_product_test_data_queue_mutex;
	extern SemaphoreHandle_t pst_rs485_rx_local_update_data_queue_mutex;
	extern SemaphoreHandle_t pst_rs485_rx_subset_update_data_queue_mutex;
	extern SemaphoreHandle_t pst_rs485_rx_transparent_data_queue_mutex;
	extern SemaphoreHandle_t pst_rs485_rx_transparent_mutex;
	extern SemaphoreHandle_t pst_rs485_data_polling_mutex;
	extern SemaphoreHandle_t pst_net_device_data_access_semaphore;
	extern SemaphoreHandle_t pst_net_device_exists_state_mutex;
	{
		if (NULL != pst_rs485_tx_data_queue_mutex)
		{
			xSemaphoreTake(pst_rs485_tx_data_queue_mutex, portMAX_DELAY);
			DebugPrintf(TRUE,"[Update]pst_rs485_tx_data_queue_mutex take\r\n");
		}
		if (NULL != pst_rs485_rx_polling_data_queue_mutex)
		{
			xSemaphoreTake(pst_rs485_rx_polling_data_queue_mutex, portMAX_DELAY);
			DebugPrintf(TRUE,"[Update]pst_rs485_rx_polling_data_queue_mutex take\r\n");
		}
		if (NULL != pst_rs485_rx_product_test_data_queue_mutex)
		{
			xSemaphoreTake(pst_rs485_rx_product_test_data_queue_mutex, portMAX_DELAY);
			DebugPrintf(TRUE,"[Update]pst_rs485_rx_polling_data_queue_mutex take\r\n");
		}
		if (NULL != pst_rs485_rx_local_update_data_queue_mutex)
		{
			xSemaphoreTake(pst_rs485_rx_local_update_data_queue_mutex, portMAX_DELAY);
			DebugPrintf(TRUE,"[Update]pst_rs485_rx_local_update_data_queue_mutex take\r\n");
		}
		if (NULL != pst_rs485_rx_subset_update_data_queue_mutex)
		{
			xSemaphoreTake(pst_rs485_rx_subset_update_data_queue_mutex, portMAX_DELAY);
			DebugPrintf(TRUE,"[Update]pst_rs485_rx_subset_update_data_queue_mutex take\r\n");
		}
		if (NULL != pst_rs485_rx_transparent_data_queue_mutex)
		{
			xSemaphoreTake(pst_rs485_rx_transparent_data_queue_mutex, portMAX_DELAY);
			DebugPrintf(TRUE,"[Update]pst_rs485_rx_transparent_data_queue_mutex take\r\n");
		}
		if (NULL != pst_rs485_rx_transparent_mutex)
		{
			xSemaphoreTake(pst_rs485_rx_transparent_mutex, portMAX_DELAY);
			DebugPrintf(TRUE,"[Update]pst_rs485_rx_transparent_mutex take\r\n");
		}
		if (NULL != pst_rs485_data_polling_mutex)
		{
			xSemaphoreTake(pst_rs485_data_polling_mutex, portMAX_DELAY);
			DebugPrintf(TRUE,"[Update]pst_rs485_data_polling_mutex take\r\n");
		}
		if(pst_net_device_data_access_semaphore != 0)
		{
			xSemaphoreTake(pst_net_device_data_access_semaphore, portMAX_DELAY);
			DebugPrintf(TRUE,"[Update]pst_net_device_data_access_semaphore take \r\n");
		}

		if(pst_net_device_exists_state_mutex != 0)
		{
			DebugPrintf(TRUE,"[Update]device_exists_state_SemaphoreTake\n");
			xSemaphoreTake(pst_net_device_exists_state_mutex, portMAX_DELAY);
			DebugPrintf(TRUE,"[Update]device_exists_state_SemaphoreTakeSuccess\n");
		}

		vTaskSuspend(device_data_polling_task_handle);
		vTaskSuspend(device_tree_refresh_task_handle);
		vTaskSuspend(energy_statistics_task_handle);

		if (NULL != pst_rs485_tx_data_queue_mutex)
		{
			xSemaphoreGive(pst_rs485_tx_data_queue_mutex);
			DebugPrintf(TRUE,"[Update]pst_rs485_tx_data_queue_mutex Give\r\n");
		}
		if (NULL != pst_rs485_rx_polling_data_queue_mutex)
		{
			xSemaphoreGive(pst_rs485_rx_polling_data_queue_mutex);
			DebugPrintf(TRUE,"[Update]pst_rs485_rx_polling_data_queue_mutex Give\r\n");
		}
		if (NULL != pst_rs485_rx_product_test_data_queue_mutex)
		{
			xSemaphoreGive(pst_rs485_rx_product_test_data_queue_mutex);
			DebugPrintf(TRUE,"[Update]pst_rs485_rx_product_test_data_queue_mutex Give\r\n");
		}
		if (NULL != pst_rs485_rx_local_update_data_queue_mutex)
		{
			xSemaphoreGive(pst_rs485_rx_local_update_data_queue_mutex);
			DebugPrintf(TRUE,"[Update]pst_rs485_rx_local_update_data_queue_mutex Give\r\n");
		}
		if (NULL != pst_rs485_rx_subset_update_data_queue_mutex)
		{
			xSemaphoreGive(pst_rs485_rx_subset_update_data_queue_mutex);
			DebugPrintf(TRUE,"[Update]pst_rs485_rx_subset_update_data_queue_mutex Give\r\n");
		}
		if (NULL != pst_rs485_rx_transparent_data_queue_mutex)
		{
			xSemaphoreGive(pst_rs485_rx_transparent_data_queue_mutex);
			DebugPrintf(TRUE,"[Update]pst_rs485_rx_transparent_data_queue_mutex Give\r\n");
		}
		if (NULL != pst_rs485_rx_transparent_mutex)
		{
			xSemaphoreGive(pst_rs485_rx_transparent_mutex);
			DebugPrintf(TRUE,"[Update]pst_rs485_rx_transparent_mutex Give\r\n");
		}
		if (NULL != pst_rs485_data_polling_mutex)
		{
			xSemaphoreGive(pst_rs485_data_polling_mutex);
			DebugPrintf(TRUE,"[Update]pst_rs485_data_polling_mutex Give\r\n");
		}
		if(pst_net_device_data_access_semaphore != 0)
		{
			xSemaphoreGive(pst_net_device_data_access_semaphore);
			DebugPrintf(TRUE,"[Update]pst_net_device_data_access_semaphore give \r\n");
		}
		if(pst_net_device_exists_state_mutex != 0)
		{
			xSemaphoreGive(pst_net_device_exists_state_mutex);
			DebugPrintf(TRUE,"[Update]device_exists_state_SemaphoreGive\n");
		}
	
	}

}

//run time :1S
/* -------------------------------------------------------------------------- */
/*                                   烧录功能主流程                                  */
/* -------------------------------------------------------------------------- */
extern uint8_t flash_erase_sector_all_flag;
uint8_t g_update_status = 0;
extern uint8_t gb_wifi_updata_status;
void local_firmware_update_loop()
{
    #define cb_firmware_update_enable_detect                0
    #define cb_firmware_verify                              1
    #define cb_firmware_update_delay                        2
    #define cb_firmware_update                              3
    #define cb_subset_update_detect                         4
    #define cb_subset_update_stop_bus                       5
    #define cb_subset_update_start_bus                      6
    #define cb_subset_update                                7
    static uint8_t b_step = cb_firmware_update_enable_detect;
    static uint8_t b_subset_update_num = 0;
    static uint8_t b_subset_update_index = 0;
    static net_device_info *pst_net_device_info = 0;
    static uint8_t b_retry_cnt = 0;

	extern SemaphoreHandle_t pst_rs485_tx_data_queue_mutex;
	extern SemaphoreHandle_t pst_rs485_rx_polling_data_queue_mutex;
	extern SemaphoreHandle_t pst_rs485_rx_product_test_data_queue_mutex;
	extern SemaphoreHandle_t pst_rs485_rx_local_update_data_queue_mutex;
	extern SemaphoreHandle_t pst_rs485_rx_subset_update_data_queue_mutex;
	extern SemaphoreHandle_t pst_rs485_rx_transparent_data_queue_mutex;
	extern SemaphoreHandle_t pst_rs485_rx_transparent_mutex;
	extern SemaphoreHandle_t pst_rs485_data_polling_mutex;
	extern SemaphoreHandle_t pst_net_device_data_access_semaphore;
	extern SemaphoreHandle_t pst_net_device_exists_state_mutex;
//	#if( update_status_print )
//	DebugPrintf(TRUE,"[Update]local_firmware_update_loop beganning\r\n");
//	#endif
    #if( update_status_print )
    if(cb_firmware_update_enable_detect != b_step)
    {
        static TickType_t x_start_time = 0;
        TickType_t x_curent_time = 0;

        if(0 == x_start_time)
        {
            x_start_time = xTaskGetTickCount();
        }
        x_curent_time = xTaskGetTickCount();
        if(x_curent_time - x_start_time > pdMS_TO_TICKS_USER(1000))
        {
            uint32_t dw_data_temp = 0;
            uint16_t w_data_len = 2;

            x_start_time = 0;

            net_system_data_access(cb_device_data_access_data_type_status,
                                   cw_sys_status_data_addr_update_status,
                                   &w_data_len,
                                   (uint8_t *)&dw_data_temp,
                                   cb_device_data_access_operate_type_read);
            DebugPrintf(TRUE,"[Update]Update Status:%d\r\n",dw_data_temp);

            net_system_data_access(cb_device_data_access_data_type_status,
                                   cw_sys_status_data_addr_update_process,
                                   &w_data_len,
                                   (uint8_t *)&dw_data_temp,
                                   cb_device_data_access_operate_type_read);
            DebugPrintf(TRUE,"[Update]Update Process:%d\r\n",dw_data_temp);

            w_data_len = 4;
            net_system_data_access(cb_device_data_access_data_type_status,
                                   cw_sys_status_data_addr_update_device,
                                   &w_data_len,
                                   (uint8_t *)&dw_data_temp,
                                   cb_device_data_access_operate_type_read);
            DebugPrintf(TRUE,"[Update]Update Device:%06X\r\n",dw_data_temp);
        }

    }
    #endif
	static TickType_t x_start_time = 0;
	static TickType_t x_start_time0 = 0;
	static TickType_t x_start_time1 = 0;
	extern uint16_t ftp_updata_package_num_all;
    switch(b_step)
    {
        case cb_firmware_update_enable_detect:
        {
            if(TRUE == local_firmware_info_check())
            {
                if(0 != st_device_update_info_current.pst_net_device_update_info_current)
                {
                    uint32_t dw_data = 0;
                    {
                        uint16_t w_data_len = 2;
                        dw_data = cw_system_wait_updating;
                        net_system_data_access(cb_device_data_access_data_type_status,
                                               cw_sys_status_data_addr_update_status,
                                               &w_data_len,
                                               (uint8_t *)&dw_data,
                                               cb_device_data_access_operate_type_write);
						g_update_status = cw_system_wait_updating;
                    }
                    {
                        uint16_t w_data_len = 4;
                        dw_data =\
                            st_device_update_info_current.pst_net_device_update_info_current->w_device_type << 8;
                        net_system_data_access(cb_device_data_access_data_type_status,
                                               cw_sys_status_data_addr_update_device,
                                               &w_data_len,
                                               (uint8_t *)&dw_data,
                                               cb_device_data_access_operate_type_write);
                    }

                    {
                        uint16_t w_data_len = 2;
                        dw_data = 0;
                        net_system_data_access(cb_device_data_access_data_type_status,
                                               cw_sys_status_data_addr_update_process,
                                               &w_data_len,
                                               (uint8_t *)&dw_data,
                                               cb_device_data_access_operate_type_write);
                    }
                    #if( update_status_print )
                    DebugPrintf(TRUE,"[Update]Firmware update enable\r\n");
                    #endif
                    b_step = cb_firmware_verify;
                }
                else{
                    DebugPrintf(TRUE,"st_device_update_info_current.pst_net_device_update_info_current\r\n");
                }
            }
            else if(0 != st_device_update_info_current.pst_net_device_update_info_current)
            {   
				if(st_device_update_info_current.pst_net_device_update_info_current->st_net_firmware_info.dw_packets_receive_current == 1)
				{
                    uint16_t w_data_len = 2;
					uint32_t dw_data = 0;
                    dw_data = cw_system_receiving_firmware;
                    net_system_data_access(cb_device_data_access_data_type_status,
                                           cw_sys_status_data_addr_update_status,
                                           &w_data_len,
                                           (uint8_t *)&dw_data,
                                           cb_device_data_access_operate_type_write);
					g_update_status = cw_system_receiving_firmware;
                }
                if(st_device_update_info_current.pst_net_device_update_info_current->st_net_firmware_info.dw_packets_receive_total !=\
                    st_device_update_info_current.pst_net_device_update_info_current->st_net_firmware_info.dw_packets_receive_current)
                {
                    static uint32_t dw_current_package_num_backup = 0;
					
                    TickType_t x_curent_time = 0;
					static TickType_t s_start_time = 0;

                    if(dw_current_package_num_backup ==\
                        st_device_update_info_current.pst_net_device_update_info_current->st_net_firmware_info.dw_packets_receive_current)
                    {
                    	if(0 == s_start_time)
						{
							s_start_time = xTaskGetTickCount();
						}
                        x_curent_time = xTaskGetTickCount();
                        if((0 != s_start_time) &&\
                           (x_curent_time - s_start_time >= pdMS_TO_TICKS_USER(10000)))
                        {
	                        uint16_t w_data_len = 2;
							uint32_t dw_data = 0;
		                    dw_data = cw_system_no_update;
		                    net_system_data_access(cb_device_data_access_data_type_status,
		                                           cw_sys_status_data_addr_update_status,
		                                           &w_data_len,
		                                           (uint8_t *)&dw_data,
		                                           cb_device_data_access_operate_type_write);
	                    	g_update_status = cw_system_no_update;
							s_start_time = 0;
                        }
						if(0 == x_start_time0)
						{
							x_start_time0 = xTaskGetTickCount();
						}
                        x_curent_time = xTaskGetTickCount();
                        if((0 != x_start_time0) &&\
                           (x_curent_time - x_start_time0 >= pdMS_TO_TICKS_USER(120000)))
                        {
                        	#if( update_status_print )							
                            DebugPrintf(TRUE,"[Update]Firmware update info clear\r\n");
                            #endif
                            st_device_update_info_current.pst_net_device_update_info_current = 0;
                            st_device_update_info_current.b_update_type = cb_firmware_format_none;
							g_update_status = cw_system_no_update;
							gb_wifi_updata_status = 0;
                            x_start_time0 = 0;
                        }
                    }
                    else
                    {
                    	if(g_update_status == cw_system_no_update)
						{
	                        uint16_t w_data_len = 2;
							uint32_t dw_data = 0;
		                    dw_data = cw_system_receiving_firmware;
		                    net_system_data_access(cb_device_data_access_data_type_status,
		                                           cw_sys_status_data_addr_update_status,
		                                           &w_data_len,
		                                           (uint8_t *)&dw_data,
		                                           cb_device_data_access_operate_type_write);
	                    	g_update_status = cw_system_receiving_firmware;
						}
                        x_start_time0 = 0;
						s_start_time = 0;
                        dw_current_package_num_backup =\
                            st_device_update_info_current.
                            pst_net_device_update_info_current->
                            st_net_firmware_info.
                            dw_packets_receive_current;
                    }
                }
				else
				{
		            TickType_t x_curent_time = 0;

		            if(0 == x_start_time1)
		            {
		                x_start_time1 = xTaskGetTickCount();
		            }
		            x_curent_time = xTaskGetTickCount();
		            if((0 != x_start_time1) && (x_curent_time - x_start_time1) >= pdMS_TO_TICKS_USER(120000))
		            {
		                x_start_time1 = 0;
						b_subset_update_index = 0;
	                    b_subset_update_num = 0;
						gb_wifi_updata_status = 0;
	                    st_device_update_info_current.pst_net_device_update_info_current = 0;
	                    st_device_update_info_current.b_update_type = cb_firmware_format_none;
						g_update_status = cw_system_no_update;
		            }
				}
            }
            else if(0 == ftp_updata_package_num_all)
            {
                uint32_t dw_data = 0;
                {
                    uint16_t w_data_len = 2;
                    dw_data = cw_system_no_update;
                    net_system_data_access(cb_device_data_access_data_type_status,
                                           cw_sys_status_data_addr_update_status,
                                           &w_data_len,
                                           (uint8_t *)&dw_data,
                                           cb_device_data_access_operate_type_write);
					g_update_status = cw_system_no_update;
                }
                {
                    uint16_t w_data_len = 4;
                    dw_data = 0;
                    net_system_data_access(cb_device_data_access_data_type_status,
                                           cw_sys_status_data_addr_update_device,
                                           &w_data_len,
                                           (uint8_t *)&dw_data,
                                           cb_device_data_access_operate_type_write);
                }
                {
                    uint16_t w_data_len = 1;
                    dw_data = 0;
                    net_system_data_access(cb_device_data_access_data_type_status,
                                           cw_sys_status_data_addr_update_process,
                                           &w_data_len,
                                           (uint8_t *)&dw_data,
                                           cb_device_data_access_operate_type_write);
                }
            }
            break;
        }
        case cb_firmware_verify:
        {
            if(TRUE == updata_data_check_all())
            {
                if(((dwa_device_id_kinergy_ii[cw_device_id_index_kinergy_ii] & 0xFFFF00) >> 8) ==\
                    st_device_update_info_current.pst_net_device_update_info_current->w_device_type)
                {
                    uint32_t dw_data = 0;
                    {
                        uint16_t w_data_len = 2;
                        dw_data = cw_system_updating;
                        net_system_data_access(cb_device_data_access_data_type_status,
                                               cw_sys_status_data_addr_update_status,
                                               &w_data_len,
                                               (uint8_t *)&dw_data,
                                               cb_device_data_access_operate_type_write);
						g_update_status = cw_system_updating;
                    }
                    {
                        uint16_t w_data_len = 4;
                        dw_data = dwa_device_id_kinergy_ii[cw_device_id_index_kinergy_ii];
                        net_system_data_access(cb_device_data_access_data_type_status,
                                               cw_sys_status_data_addr_update_device,
                                               &w_data_len,
                                               (uint8_t *)&dw_data,
                                               cb_device_data_access_operate_type_write);
                    }
                    {
                        uint16_t w_data_len = 1;
                        dw_data = 100;
                        net_system_data_access(cb_device_data_access_data_type_status,
                                               cw_sys_status_data_addr_update_process,
                                               &w_data_len,
                                               (uint8_t *)&dw_data,
                                               cb_device_data_access_operate_type_write);
                    }
                    #if( update_status_print )
                    DebugPrintf(TRUE,"[Update]Local firmware update\r\n");
                    #endif
					yl_debug_printf("[Update]Local firmware update update LINE is:%d\r\n",__LINE__);
                    b_step = cb_firmware_update_delay;
                }
                else
                {
                    b_step  = cb_subset_update_detect;
                    yl_debug_printf("subset update:%x\r\n",st_device_update_info_current.pst_net_device_update_info_current->w_device_type);
                }
            }
			else
			{
                {
                    uint32_t dw_data = 0;
                    {
                        uint16_t w_data_len = 2;
                        dw_data = cw_system_no_update;
                        net_system_data_access(cb_device_data_access_data_type_status,
                                               cw_sys_status_data_addr_update_status,
                                               &w_data_len,
                                               (uint8_t *)&dw_data,
                                               cb_device_data_access_operate_type_write);
						g_update_status = cw_system_no_update;
                    }
                    {
                        uint16_t w_data_len = 4;
                        dw_data = dwa_device_id_kinergy_ii[cw_device_id_index_kinergy_ii];
                        net_system_data_access(cb_device_data_access_data_type_status,
                                               cw_sys_status_data_addr_update_device,
                                               &w_data_len,
                                               (uint8_t *)&dw_data,
                                               cb_device_data_access_operate_type_write);
                    }
                    {
                        uint16_t w_data_len = 1;
                        dw_data = 0;
                        net_system_data_access(cb_device_data_access_data_type_status,
                                               cw_sys_status_data_addr_update_process,
                                               &w_data_len,
                                               (uint8_t *)&dw_data,
                                               cb_device_data_access_operate_type_write);
                    }
                    DebugPrintf(TRUE,"local_firmware_verify fail\r\n");

                }

				b_step  = cb_firmware_update_enable_detect;
			}
            break;
        }
        case cb_firmware_update_delay:
        {

            TickType_t x_curent_time = 0;

            if(0 == x_start_time)
            {
                x_start_time = xTaskGetTickCount();
            }
            x_curent_time = xTaskGetTickCount();
            if((x_curent_time - x_start_time) >= pdMS_TO_TICKS_USER(1000))
            {
                x_start_time = 0;
                b_step = cb_firmware_update;
            }

            break;
        }
        case cb_firmware_update:
        {
        	yl_debug_printf("[Update]Local firmware update update LINE is:%d\r\n",__LINE__);
            local_firmware_update();
            break;
        }
        case cb_subset_update_detect:
        {
            if((cdw_device_type_cklcd >> 8) ==\
                st_device_update_info_current.pst_net_device_update_info_current->w_device_type)
            {
                b_subset_update_num = 9;
                for(;b_subset_update_index < b_subset_update_num;b_subset_update_index++)
                {
                    if(FALSE == net_get_device_info((cdw_device_type_cklcd | cba_cklcd_com_id[b_subset_update_index]),
                                                    (net_device_info **)&pst_net_device_info,
                                                    &st_net_system_info_list))
                    {
                        continue;
                    }
                    if(TRUE == net_check_device_communicate_status(cb_net_device_add,pst_net_device_info))
                    {
                        b_step = cb_subset_update_stop_bus;
                        b_retry_cnt = 0;
                        {
                            uint16_t w_data_len = 4;
                            net_system_data_access(cb_device_data_access_data_type_status,
                                                   cw_sys_status_data_addr_update_device,
                                                   &w_data_len,
                                                   (uint8_t *)pst_net_device_info->pdw_device_id,
                                                   cb_device_data_access_operate_type_write);
                            #if( update_status_print )
                            DebugPrintf(TRUE,"[Update]Cklcd update device detect\r\n");
                            #endif
                        }
                        {
                            uint16_t w_data_len = 2;
                            uint16_t w_data = cw_system_updating;
                            net_system_data_access(cb_device_data_access_data_type_status,
                                                   cw_sys_status_data_addr_update_status,
                                                   &w_data_len,
                                                   (uint8_t *)&w_data,
                                                   cb_device_data_access_operate_type_write);
							g_update_status = cw_system_updating;
                            #if( update_status_print )
                            DebugPrintf(TRUE,"[Update]Firmware update status fresh,%d\r\n",w_data);
                            #endif
                        }
						{
                            uint16_t w_data_len = 2;
                            uint16_t w_data = 0;
                            net_system_data_access(cb_device_data_access_data_type_status,
                                                   cw_sys_status_data_addr_update_process,
                                                   &w_data_len,
                                                   (uint8_t *)&w_data,
                                                   cb_device_data_access_operate_type_write);
                            #if( update_status_print )
                            DebugPrintf(TRUE,"[Update]Firmware update process fresh,%d\r\n",w_data);
                            #endif
                        }
                        break;
                    }
                }
                if(b_subset_update_index >= b_subset_update_num)
                {
                    b_subset_update_index = 0;
                    b_subset_update_num = 0;
                    st_device_update_info_current.pst_net_device_update_info_current = 0;
                    st_device_update_info_current.b_update_type = cb_firmware_format_none;
                    b_step = cb_subset_update_start_bus;
                }
            }
            else if((cdw_device_type_ck_inverter >> 8) ==\
                st_device_update_info_current.pst_net_device_update_info_current->w_device_type)
            {
                b_subset_update_num = 9;
                for(;b_subset_update_index < b_subset_update_num;b_subset_update_index++)
                {
                    if(FALSE == net_get_device_info((cdw_device_type_ck_inverter | cba_cklcd_com_id[b_subset_update_index]),
                                                    (net_device_info **)&pst_net_device_info,
                                                    &st_net_system_info_list))
                    {
                        continue;
                    }
                    if(TRUE == net_check_device_communicate_status(cb_net_device_add,pst_net_device_info))
                    {
                        b_step = cb_subset_update_stop_bus;
                        b_retry_cnt = 0;
                        {
                            uint16_t w_data_len = 4;
                            net_system_data_access(cb_device_data_access_data_type_status,
                                                   cw_sys_status_data_addr_update_device,
                                                   &w_data_len,
                                                   (uint8_t *)pst_net_device_info->pdw_device_id,
                                                   cb_device_data_access_operate_type_write);
                            #if( update_status_print )
                            DebugPrintf(TRUE,"[Update]Ck Inverter update device detect\r\n");
                            #endif
                        }
                        {
                            uint16_t w_data_len = 2;
                            uint16_t w_data = cw_system_updating;
                            net_system_data_access(cb_device_data_access_data_type_status,
                                                   cw_sys_status_data_addr_update_status,
                                                   &w_data_len,
                                                   (uint8_t *)&w_data,
                                                   cb_device_data_access_operate_type_write);
							g_update_status = cw_system_updating;
                            #if( update_status_print )
                            DebugPrintf(TRUE,"[Update]Firmware update status fresh,%d\r\n",w_data);
                            #endif
                        }
						{
                            uint16_t w_data_len = 2;
                            uint16_t w_data = 0;
                            net_system_data_access(cb_device_data_access_data_type_status,
                                                   cw_sys_status_data_addr_update_process,
                                                   &w_data_len,
                                                   (uint8_t *)&w_data,
                                                   cb_device_data_access_operate_type_write);
                            #if( update_status_print )
                            DebugPrintf(TRUE,"[Update]Firmware update process fresh,%d\r\n",w_data);
                            #endif
                        }
                        break;
                    }
                }
                if(b_subset_update_index >= b_subset_update_num)
                {
                    b_subset_update_index = 0;
                    b_subset_update_num = 0;
                    st_device_update_info_current.pst_net_device_update_info_current = 0;
                    st_device_update_info_current.b_update_type = cb_firmware_format_none;
                    b_step = cb_subset_update_start_bus;

                    #if( update_status_print )
                    DebugPrintf(TRUE,"[Update]Firmware update finish\r\n");
                    #endif
                }
            }
            else if((cdw_device_type_mppt >> 8) ==\
                st_device_update_info_current.pst_net_device_update_info_current->w_device_type)
            {
            	if(check_system_type(cb_inv_type_apollo_matric_ii_10_15k) || check_system_type(cb_inv_type_apollo_matric_ii_5_8k))
            	{
					b_subset_update_num = 54;
					for(;b_subset_update_index < b_subset_update_num;b_subset_update_index++)
					{
						if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[cw_device_id_index_mppt_u1_0 + b_subset_update_index],
														(net_device_info **)&pst_net_device_info,
														&st_net_system_info_list))
						{
							continue;
						}

						if(TRUE == net_check_device_communicate_status(cb_net_device_add,pst_net_device_info))
						{
							net_get_device_info(dwa_device_id_kinergy_ii[cw_device_id_index_mppt_u1_0 + ((b_subset_update_index / 6) * 6)],
												(net_device_info **)&pst_net_device_info,
												&st_net_system_info_list);

							if(b_subset_update_index % 6 == 0)
							{
								gb_iner_mppt_id = 0x61;
							}
							else if((b_subset_update_index % 6 == 1) && check_system_type(cb_inv_type_apollo_matric_ii_10_15k))
							{
								gb_iner_mppt_id = 0x62;
							}
							else
							{
								continue;
							}
							b_step = cb_subset_update_stop_bus;
							b_retry_cnt = 0;
							{
								uint16_t w_data_len = 4;
								uint32_t *dw_device_id = (gb_iner_mppt_id == 0x61) ? pst_net_device_info->pdw_device_id : (pst_net_device_info->pdw_device_id + 1);

								net_system_data_access(cb_device_data_access_data_type_status,
													   cw_sys_status_data_addr_update_device,
													   &w_data_len,
													   (uint8_t *)dw_device_id,
													   cb_device_data_access_operate_type_write);
								#if( update_status_print )
								DebugPrintf(TRUE,"[Update]Mppt update device detect\r\n");
								#endif
							}
							{
								uint16_t w_data_len = 2;
								uint16_t w_data = cw_system_updating;
								net_system_data_access(cb_device_data_access_data_type_status,
													   cw_sys_status_data_addr_update_status,
													   &w_data_len,
													   (uint8_t *)&w_data,
													   cb_device_data_access_operate_type_write);
								g_update_status = cw_system_updating;
								#if( update_status_print )
								DebugPrintf(TRUE,"[Update]Firmware update status fresh,%d\r\n",w_data);
								#endif
							}
							{
								uint16_t w_data_len = 2;
								uint16_t w_data = 0;
								net_system_data_access(cb_device_data_access_data_type_status,
													   cw_sys_status_data_addr_update_process,
													   &w_data_len,
													   (uint8_t *)&w_data,
													   cb_device_data_access_operate_type_write);
								#if( update_status_print )
								DebugPrintf(TRUE,"[Update]Firmware update process fresh,%d\r\n",w_data);
								#endif
							}
							break;
						}
					}
				}
				else
				{
					b_subset_update_num = 9;
					for(;b_subset_update_index < b_subset_update_num;b_subset_update_index++)
					{
						if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[cw_device_id_index_mppt_u1_0 + b_subset_update_index * 6],
														(net_device_info **)&pst_net_device_info,
														&st_net_system_info_list))
						{
							continue;
						}
						if(TRUE == net_check_device_communicate_status(cb_net_device_add,pst_net_device_info))
						{
							b_step = cb_subset_update_stop_bus;
							b_retry_cnt = 0;
							{
								uint16_t w_data_len = 4;
								net_system_data_access(cb_device_data_access_data_type_status,
													   cw_sys_status_data_addr_update_device,
													   &w_data_len,
													   (uint8_t *)pst_net_device_info->pdw_device_id,
													   cb_device_data_access_operate_type_write);
								#if( update_status_print )
								DebugPrintf(TRUE,"[Update]Mppt update device detect\r\n");
								#endif
							}
							{
								uint16_t w_data_len = 2;
								uint16_t w_data = cw_system_updating;
								net_system_data_access(cb_device_data_access_data_type_status,
													   cw_sys_status_data_addr_update_status,
													   &w_data_len,
													   (uint8_t *)&w_data,
													   cb_device_data_access_operate_type_write);
								g_update_status = cw_system_updating;
								#if( update_status_print )
								DebugPrintf(TRUE,"[Update]Firmware update status fresh,%d\r\n",w_data);
								#endif
							}
							{
								uint16_t w_data_len = 2;
								uint16_t w_data = 0;
								net_system_data_access(cb_device_data_access_data_type_status,
													   cw_sys_status_data_addr_update_process,
													   &w_data_len,
													   (uint8_t *)&w_data,
													   cb_device_data_access_operate_type_write);
								#if( update_status_print )
								DebugPrintf(TRUE,"[Update]Firmware update process fresh,%d\r\n",w_data);
								#endif
							}
							break;
						}
					}
				}

                if(b_subset_update_index >= b_subset_update_num)
                {
                    b_subset_update_index = 0;
                    b_subset_update_num = 0;
                    st_device_update_info_current.pst_net_device_update_info_current = 0;
                    st_device_update_info_current.b_update_type = cb_firmware_format_none;
                    b_step = cb_subset_update_start_bus;
                }
            }
            else
            {
                b_subset_update_index = 0;
                b_subset_update_num = 0;
                st_device_update_info_current.pst_net_device_update_info_current = 0;
                st_device_update_info_current.b_update_type = cb_firmware_format_none;
                b_step  = cb_firmware_update_enable_detect;

                #if( update_status_print )
                DebugPrintf(TRUE,"[Update]Firmware update device type err\r\n");
                #endif
            }
            break;
        }
        case cb_subset_update_stop_bus:
        {
			if (NULL != pst_rs485_tx_data_queue_mutex)
			{
				xSemaphoreTake(pst_rs485_tx_data_queue_mutex, portMAX_DELAY);
				DebugPrintf(TRUE,"[Update]pst_rs485_tx_data_queue_mutex take\r\n");
			}
			if (NULL != pst_rs485_rx_polling_data_queue_mutex)
			{
				xSemaphoreTake(pst_rs485_rx_polling_data_queue_mutex, portMAX_DELAY);
				DebugPrintf(TRUE,"[Update]pst_rs485_rx_polling_data_queue_mutex take\r\n");
			}
			if (NULL != pst_rs485_rx_product_test_data_queue_mutex)
			{
				xSemaphoreTake(pst_rs485_rx_product_test_data_queue_mutex, portMAX_DELAY);
				DebugPrintf(TRUE,"[Update]pst_rs485_rx_polling_data_queue_mutex take\r\n");
			}
			if (NULL != pst_rs485_rx_local_update_data_queue_mutex)
			{
				xSemaphoreTake(pst_rs485_rx_local_update_data_queue_mutex, portMAX_DELAY);
				DebugPrintf(TRUE,"[Update]pst_rs485_rx_local_update_data_queue_mutex take\r\n");
			}
			if (NULL != pst_rs485_rx_subset_update_data_queue_mutex)
			{
				xSemaphoreTake(pst_rs485_rx_subset_update_data_queue_mutex, portMAX_DELAY);
				DebugPrintf(TRUE,"[Update]pst_rs485_rx_subset_update_data_queue_mutex take\r\n");
			}
			if (NULL != pst_rs485_rx_transparent_data_queue_mutex)
			{
				xSemaphoreTake(pst_rs485_rx_transparent_data_queue_mutex, portMAX_DELAY);
				DebugPrintf(TRUE,"[Update]pst_rs485_rx_transparent_data_queue_mutex take\r\n");
			}
			if (NULL != pst_rs485_rx_transparent_mutex)
			{
				xSemaphoreTake(pst_rs485_rx_transparent_mutex, portMAX_DELAY);
				DebugPrintf(TRUE,"[Update]pst_rs485_rx_transparent_mutex take\r\n");
			}
			if (NULL != pst_rs485_data_polling_mutex)
			{
				xSemaphoreTake(pst_rs485_data_polling_mutex, portMAX_DELAY);
				DebugPrintf(TRUE,"[Update]pst_rs485_data_polling_mutex take\r\n");
			}
			if(pst_net_device_data_access_semaphore != 0)
			{
				xSemaphoreTake(pst_net_device_data_access_semaphore, portMAX_DELAY);
				DebugPrintf(TRUE,"[Update]pst_net_device_data_access_semaphore take \r\n");
			}

			if(pst_net_device_exists_state_mutex != 0)
		    {
		        DebugPrintf(TRUE,"[Update]device_exists_state_SemaphoreTake\n");
		        xSemaphoreTake(pst_net_device_exists_state_mutex, portMAX_DELAY);
		        DebugPrintf(TRUE,"[Update]device_exists_state_SemaphoreTakeSuccess\n");
		    }

			vTaskSuspend(device_data_polling_task_handle);
            vTaskSuspend(device_tree_refresh_task_handle);
            vTaskSuspend(energy_statistics_task_handle);

			if (NULL != pst_rs485_tx_data_queue_mutex)
			{
				xSemaphoreGive(pst_rs485_tx_data_queue_mutex);
				DebugPrintf(TRUE,"[Update]pst_rs485_tx_data_queue_mutex Give\r\n");
			}
			if (NULL != pst_rs485_rx_polling_data_queue_mutex)
			{
				xSemaphoreGive(pst_rs485_rx_polling_data_queue_mutex);
				DebugPrintf(TRUE,"[Update]pst_rs485_rx_polling_data_queue_mutex Give\r\n");
			}
			if (NULL != pst_rs485_rx_product_test_data_queue_mutex)
			{
				xSemaphoreGive(pst_rs485_rx_product_test_data_queue_mutex);
				DebugPrintf(TRUE,"[Update]pst_rs485_rx_product_test_data_queue_mutex Give\r\n");
			}
			if (NULL != pst_rs485_rx_local_update_data_queue_mutex)
			{
				xSemaphoreGive(pst_rs485_rx_local_update_data_queue_mutex);
				DebugPrintf(TRUE,"[Update]pst_rs485_rx_local_update_data_queue_mutex Give\r\n");
			}
			if (NULL != pst_rs485_rx_subset_update_data_queue_mutex)
			{
				xSemaphoreGive(pst_rs485_rx_subset_update_data_queue_mutex);
				DebugPrintf(TRUE,"[Update]pst_rs485_rx_subset_update_data_queue_mutex Give\r\n");
			}
			if (NULL != pst_rs485_rx_transparent_data_queue_mutex)
			{
				xSemaphoreGive(pst_rs485_rx_transparent_data_queue_mutex);
				DebugPrintf(TRUE,"[Update]pst_rs485_rx_transparent_data_queue_mutex Give\r\n");
			}
			if (NULL != pst_rs485_rx_transparent_mutex)
			{
				xSemaphoreGive(pst_rs485_rx_transparent_mutex);
				DebugPrintf(TRUE,"[Update]pst_rs485_rx_transparent_mutex Give\r\n");
			}
			if (NULL != pst_rs485_data_polling_mutex)
			{
				xSemaphoreGive(pst_rs485_data_polling_mutex);
				DebugPrintf(TRUE,"[Update]pst_rs485_data_polling_mutex Give\r\n");
			}
			if(pst_net_device_data_access_semaphore != 0)
			{
				xSemaphoreGive(pst_net_device_data_access_semaphore);
				DebugPrintf(TRUE,"[Update]pst_net_device_data_access_semaphore give \r\n");
			}
			if(pst_net_device_exists_state_mutex != 0)
			{
				xSemaphoreGive(pst_net_device_exists_state_mutex);
				DebugPrintf(TRUE,"[Update]device_exists_state_SemaphoreGive\n");
			}


            b_step = cb_subset_update;
            #if( update_status_print )
            DebugPrintf(TRUE,"[Update]Stop Bus\r\n");
            #endif
        }
        break;
        case cb_subset_update_start_bus:
        {
        	extern uint8_t gb_ble_updata_status;
        	x_start_time = 0;
        	x_start_time0 = 0;
			x_start_time1 = 0;
			if(gb_ble_updata_status == 1)
			{
				gb_ble_updata_status = 0;
			}
			gb_wifi_updata_status = 0;
            vTaskResume(device_data_polling_task_handle);
            vTaskResume(device_tree_refresh_task_handle);
			vTaskResume(energy_statistics_task_handle);
            flash_erase_sector_all_flag = 1;
            b_step = cb_firmware_update_enable_detect;
			#if( update_status_print )
            DebugPrintf(TRUE,"[Update]Start Bus\r\n");
            #endif
        }
        break;
        case cb_subset_update:
        {
            if(st_device_update_info_current.
               pst_net_device_update_info_current->
               b_firmware_format < cb_max_subset_firmware_format)
            {
                uint8_t b_result = 0;
                b_result = subset_update_process_func[st_device_update_info_current.
                                                      pst_net_device_update_info_current->
                                                      b_firmware_format](pst_net_device_info);
                switch(b_result)
                {
                    case cb_subset_update_com_success:
                        {

                        }
                        break;
                    case cb_subset_update_success    :
                        {
                            display_management_parameter_send(normal_mode,com_led,device_add,TRUE);
                            b_subset_update_index++;
                            b_step = cb_subset_update_detect;
                            b_retry_cnt = 0;
                            #if( update_status_print )
                            DebugPrintf(TRUE,"[Update]Subset update success\r\n");
                            #endif
                        }
                        break;
                    case cb_subset_update_fail       :
                        {
                            b_retry_cnt++;
                            if(b_retry_cnt > 2)
							{
                                display_management_parameter_send(normal_mode,com_led,device_loss,TRUE);
								if(gb_update_firware_fail_flag)
								{
									gb_update_firware_fail_flag = 0;
							        uint16_t w_data_len = 2;
									st_subset_update_info.w_update_per = 200;
									st_subset_update_info.w_update_state = cb_firmware_update_status_none;
									net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_system],
									                       cb_device_data_access_data_type_status,
									                       cw_sys_status_data_addr_update_process,
									                       &w_data_len,
									                       ( uint8_t * )&st_subset_update_info.w_update_per,
									                       cb_device_data_access_operate_type_write);
									net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_system],
							                               cb_device_data_access_data_type_status,
							                               cw_sys_status_data_addr_update_status,
							                               &w_data_len,
							                               (uint8_t *)&st_subset_update_info.w_update_state,
							                               cb_device_data_access_operate_type_write);

								}
                                b_subset_update_index++;
                                b_step = cb_subset_update_detect;
                                b_retry_cnt = 0;
                                #if( update_status_print )
                                DebugPrintf(TRUE,"[Update]Subset update fail\r\n");
                                #endif
                                vTaskDelay(120000);
                            }
                        }
                        break;
                    case cb_subset_update_com_fail   :
                    case cb_subset_update_none       :
                    default:
                        {
                        }
                        break;
                }
            }
            else
            {
                b_subset_update_index = 0;
                b_subset_update_num = 0;
                st_device_update_info_current.pst_net_device_update_info_current = 0;
                st_device_update_info_current.b_update_type = cb_firmware_format_none;
                b_step = cb_subset_update_start_bus;
            }
            break;
        }
    }
}
uint8_t firmware_update_pac_packet_info_store(uint16_t w_firmware_packet_len,
                                              firmware_packet *pst_firmware_packet,
                                              device_update_info_current *pst_device_update_info_current)
{
    uint16_t w_data_len = 0;
    uint32_t dw_package_num_temp = 0;
    uint32_t dw_package_sum_temp = 0;

    LocalUpdateSpiFlashFirmwarePacket *pstLocalUpdateSpiFlashFirmwarePacket = 0;

    dw_firmware_data_spi_flash_addr = cdwDevFirmAddrStart;
    if(FALSE == extern_flash_wirte(dw_firmware_data_spi_flash_addr,1,(uint8_t*)&flash_flag_start)){
        debug_updata_error("[flash]write flag fail \r\n");
        return FALSE;
    }
    dw_firmware_data_spi_flash_addr += cbFlashFirmInfoLen;

//    do{
//        pstLocalUpdateSpiFlashFirmwarePacket = (LocalUpdateSpiFlashFirmwarePacket *)pvPortMalloc(sizeof(LocalUpdateSpiFlashFirmwarePacket));
//    }while(NULL == pstLocalUpdateSpiFlashFirmwarePacket);
    pstLocalUpdateSpiFlashFirmwarePacket = (LocalUpdateSpiFlashFirmwarePacket *)pvPortMalloc(sizeof(LocalUpdateSpiFlashFirmwarePacket));
    if(NULL == pstLocalUpdateSpiFlashFirmwarePacket)
    {
        #if( system_alarm_print)
        DebugPrintf(TRUE,"[ERROR]Update pac malloc err:1\r\n");
        #endif
        return FALSE;
    }
    #if(dynamic_memory_request_test)
    DebugPrintf(TRUE,"firmware_update_pac_packet_info_store_0_malloc%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
    #endif

    //debug_updata_test("[updata] flash addr 0x%x\r\n", dw_firmware_data_spi_flash_addr);
	if(NULL == memset(pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketIndex,0x00,4))
	{
		vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"firmware_update_pac_packet_info_store_0_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
	}

    if(FALSE == bytes_to_int_big_to_little_endain(pst_firmware_packet->st_firmware_packet_pac.st_pac_packet.ba_package_len,
                                                  (uint32_t *)&w_data_len,
                                                  2))
    {
		vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"firmware_update_pac_packet_info_store_1_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }

    if(FALSE == bytes_to_int_big_to_little_endain(pst_firmware_packet->st_firmware_packet_pac.st_pac_packet.ba_package_num,
                                                  (uint32_t *)&dw_package_num_temp,
                                                  4))
    {
		vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"firmware_update_pac_packet_info_store_2_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }

    if(FALSE == bytes_to_int_big_to_little_endain(pst_firmware_packet->st_firmware_packet_pac.st_pac_packet.ba_package_sum,
                                                  (uint32_t *)&dw_package_sum_temp,
                                                  4))
    {
		vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"firmware_update_pac_packet_info_store_3_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }

    if(dw_package_sum_temp < dw_package_num_temp)
    {
		vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"firmware_update_pac_packet_info_store_4_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }

	//small to big edain
	if(FALSE == byte_fill_in_reverse_order(4,
                                           pst_firmware_packet->st_firmware_packet_pac.st_pac_packet.ba_package_num,
                                           pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketIndex,
                                           4))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"firmware_update_pac_packet_info_store_7_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }

    if(w_data_len + 10 == w_firmware_packet_len)//4num 4sum 2len
    {
        w_data_len = w_firmware_packet_len;
    }
    else
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"firmware_update_pac_packet_info_store_7_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }

    memcpy((char *)&pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.bData[4],
           (char *)&pst_firmware_packet->st_firmware_packet_pac.st_pac_packet.ba_package_num[0],
           w_data_len);

    *((uint32_t *)&pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.bData[0]) = dw_package_sum_temp;

    w_data_len += 4;
    if(FALSE == calculate_crc16(pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.bData,
                                w_data_len,
                                &pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.bData[w_data_len],
                                &pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.bData[w_data_len + 1]))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"firmware_update_pac_packet_info_store_7_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }
    w_data_len += 2;

    pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketDataLen[0] = (uint8_t)(w_data_len & 0x00FF);
    pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketDataLen[1] = (uint8_t)((w_data_len & 0xFF00) >> 8);

    DebugPrintf(TRUE,"firmware_update_pac_packet_info_store,store_data_start,time: %d\n",xTaskGetTickCount());
    if(FALSE == extern_flash_wirte(dw_firmware_data_spi_flash_addr,(w_data_len + 6),pstLocalUpdateSpiFlashFirmwarePacket->bData))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"firmware_update_pac_packet_info_store_8_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        debug_updata_error("\r\n[updata]flash fail LINE is : %d\r\n",__LINE__);
        return FALSE;
    }
    DebugPrintf(TRUE,"firmware_update_pac_packet_info_store,store_data_end,time: %d\n",xTaskGetTickCount());

    dw_firmware_data_spi_flash_addr += w_data_len + 6;
	if(st_device_update_info_current.pst_net_device_update_info_current->w_device_type == (cdw_device_type_mppt & 0xFFFF00) >> 8)
	{
		st_device_update_info_current.pst_net_device_update_info_current->b_firmware_format = cb_firmware_format_pac;
		st_device_update_info_current.pst_net_device_update_info_current->pb_firmware_file = "mppt_firmware";
		st_device_update_info_current.pst_net_device_update_info_current->pb_firmware_file_info = "mppt_firmware_info";
	}

    {
		pst_device_update_info_current->
        pst_net_device_update_info_current->
        st_net_firmware_info.
        dw_packets_receive_current++;

        pst_device_update_info_current->
        pst_net_device_update_info_current->
        st_net_firmware_info.b_packet_format = pst_device_update_info_current->b_packet_format;

        pst_device_update_info_current->
        pst_net_device_update_info_current->
        st_net_firmware_info.
        dw_packets_receive_total =\
        pst_device_update_info_current->dw_packets_number;

        memcpy(pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info.ba_firmware_id,\
               pst_device_update_info_current->ba_firmware_id,\
               cb_firmware_id_max_len);
    }

    vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
    #if(dynamic_memory_request_test)
    DebugPrintf(TRUE,"firmware_update_pac_packet_info_store_9_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
    #endif
    return TRUE;
}
uint8_t b_firmware_update_pac_packet_store_finish_flag = 0;
uint8_t firmware_update_pac_packet_data_store(uint16_t w_firmware_packet_len,
                                              firmware_packet *pst_firmware_packet,
                                              device_update_info_current *pst_device_update_info_current)
{
    static uint16_t w_packet_index_backup = 1;
    static uint16_t w_data_len_backup = 0;
    uint16_t w_data_len = 0;
    uint32_t dw_package_num_temp = 0;
    uint32_t dw_package_sum_temp = 0;

    LocalUpdateSpiFlashFirmwarePacket *pstLocalUpdateSpiFlashFirmwarePacket = 0;

//    do{
//        pstLocalUpdateSpiFlashFirmwarePacket = (LocalUpdateSpiFlashFirmwarePacket *)pvPortMalloc(sizeof(LocalUpdateSpiFlashFirmwarePacket));
//    }while(NULL == pstLocalUpdateSpiFlashFirmwarePacket);
    pstLocalUpdateSpiFlashFirmwarePacket = (LocalUpdateSpiFlashFirmwarePacket *)pvPortMalloc(sizeof(LocalUpdateSpiFlashFirmwarePacket));
    if(NULL == pstLocalUpdateSpiFlashFirmwarePacket)
    {
        #if( system_alarm_print)
        DebugPrintf(TRUE,"[ERROR]Update pac malloc err:2\r\n");
        #endif
        return FALSE;
    }
    #if(dynamic_memory_request_test)
    DebugPrintf(TRUE,"local_firmware_update_packet_data_store_0_malloc%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
    #endif

//    if(FALSE == bytes_to_int_big_to_little_endain(pst_firmware_packet->st_firmware_packet_pac.st_pac_packet.ba_package_len,
//                                                  (uint32_t *)&w_data_len,
//                                                  2))
//    {
//		vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
//        #if(dynamic_memory_request_test)
//        DebugPrintf(TRUE,"firmware_update_pac_packet_info_store_1_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
//        #endif
//        return FALSE;
//    }

    if(FALSE == bytes_to_int_big_to_little_endain(pst_firmware_packet->st_firmware_packet_pac.st_pac_packet.ba_package_num,
                                                  (uint32_t *)&dw_package_num_temp,
                                                  2))
    {
		vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"firmware_update_pac_packet_data_store_2_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }

    if(FALSE == bytes_to_int_big_to_little_endain(pst_firmware_packet->st_firmware_packet_pac.st_pac_packet.ba_package_sum,
                                                  (uint32_t *)&dw_package_sum_temp,
                                                  2))
    {
		vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"firmware_update_pac_packet_data_store_3_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }

    if(dw_package_sum_temp < dw_package_num_temp)
    {
		vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"firmware_update_pac_packet_data_store_4_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }

	if(FALSE == byte_fill_in_reverse_order(4,
                                           pst_firmware_packet->st_firmware_packet_pac.st_pac_packet.ba_package_num,
                                           pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketIndex,
                                           4))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"firmware_update_pac_packet_data_store_5_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }

    for(uint16_t w_cnt_i = 0;w_cnt_i < w_firmware_packet_len;w_cnt_i++)
    {
        pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.data.bPacketData[w_cnt_i] =\
			pst_firmware_packet->st_firmware_packet_pac.st_pac_packet.ba_package_num[w_cnt_i];
    }
    w_data_len += w_firmware_packet_len;

    if(FALSE == calculate_crc16(pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.data.bPacketData,
                                w_data_len,
                                &pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.data.bPacketData[w_firmware_packet_len],
                                &pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.data.bPacketData[w_firmware_packet_len + 1]))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"firmware_update_pac_packet_data_store_6_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }
    w_data_len += 2;

    pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketDataLen[0] = (uint8_t)(w_data_len & 0x00FF);
    pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketDataLen[1] = (uint8_t)((w_data_len & 0xFF00) >> 8);

    if(w_packet_index_backup == pst_device_update_info_current->
                                pst_net_device_update_info_current->
                                st_net_firmware_info.
                                dw_packets_receive_current)
    {
        uint8_t ba_packet_temp[cdwLocalUpdateFirmDataLen] = {0,};

        extern_flash_read((dw_firmware_data_spi_flash_addr - w_data_len_backup - 6),
                          (w_data_len_backup + 6),
                          ba_packet_temp);
        if(0 == memcmp((char *)ba_packet_temp,
                       (char *)pstLocalUpdateSpiFlashFirmwarePacket->bData,
                       w_data_len_backup + 6))
        {
            vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"firmware_update_pac_packet_data_store_7_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
            #endif
            debug_updata_error("[updata]line %d\r\n",__LINE__);
            return TRUE;
        }
        else
        {
            vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"firmware_update_pac_packet_data_store_8_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
            #endif
            debug_updata_error("[updata]line %d\r\n",__LINE__);
            return FALSE;
        }
    }

    if(FALSE == extern_flash_wirte(dw_firmware_data_spi_flash_addr,(w_data_len + 6),pstLocalUpdateSpiFlashFirmwarePacket->bData))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"firmware_update_pac_packet_data_store_9_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }
    {
        w_packet_index_backup = pst_device_update_info_current->
                                   pst_net_device_update_info_current->
                                   st_net_firmware_info.
                                   dw_packets_receive_current;
        w_data_len_backup = w_data_len;
    }

    dw_firmware_data_spi_flash_addr += w_data_len + 6;

//    DebugPrintf(TRUE,"firmware_update_pac_packet_data_store,store_data_end,time: %d\n",xTaskGetTickCount());

    {

        if(pst_device_update_info_current->
           pst_net_device_update_info_current->
           st_net_firmware_info.
           dw_packets_receive_current ==\
           pst_device_update_info_current->
           pst_net_device_update_info_current->
           st_net_firmware_info.
           dw_packets_receive_total)
        {
            b_local_firmware_enable = TRUE;
            w_packet_index_backup = 1;
            w_data_len_backup = 0;
            fWriteEEPage(15) = TRUE;
			DebugPrintf(TRUE,"dw_packets_receive_current = dw_packets_receive_total&& b_local_firmware_enable == TRUE \n");
        }

		 if(pst_device_update_info_current->
           pst_net_device_update_info_current->
           st_net_firmware_info.
           dw_packets_receive_current ==\
           pst_device_update_info_current->
           pst_net_device_update_info_current->
           st_net_firmware_info.
           dw_packets_receive_total - 1)
        {
        	b_firmware_update_pac_packet_store_finish_flag = 1;
			DebugPrintf(TRUE,"dw_packets_receive_current = dw_packets_receive_total - 1 \n");
        }
    }

    vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
    #if(dynamic_memory_request_test)
    DebugPrintf(TRUE,"firmware_update_pac_packet_data_store_10_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
    #endif

    return TRUE;
}

uint8_t firmware_update_pacx_dsp_packet_info_store(uint16_t w_firmware_packet_len,
                                                   firmware_packet *pst_firmware_packet,
                                                   device_update_info_current *pst_device_update_info_current)
{
    uint16_t w_data_len = 0;
    uint32_t dw_package_num_temp = 0;
    uint32_t dw_package_sum_temp = 0;

    LocalUpdateSpiFlashFirmwarePacket *pstLocalUpdateSpiFlashFirmwarePacket = 0;

    dw_firmware_data_spi_flash_addr = cdwDevFirmAddrStart;
    if(FALSE == extern_flash_wirte(dw_firmware_data_spi_flash_addr,1,(uint8_t*)&flash_flag_start)){
        debug_updata_error("[flash]write flag fail \r\n");
        return FALSE;
    }
    dw_firmware_data_spi_flash_addr += cbFlashFirmInfoLen;

//    do{
//        pstLocalUpdateSpiFlashFirmwarePacket = (LocalUpdateSpiFlashFirmwarePacket *)pvPortMalloc(sizeof(LocalUpdateSpiFlashFirmwarePacket));
//    }while(NULL == pstLocalUpdateSpiFlashFirmwarePacket);
    pstLocalUpdateSpiFlashFirmwarePacket = (LocalUpdateSpiFlashFirmwarePacket *)pvPortMalloc(sizeof(LocalUpdateSpiFlashFirmwarePacket));
    if(NULL == pstLocalUpdateSpiFlashFirmwarePacket)
    {
        #if( system_alarm_print)
        DebugPrintf(TRUE,"[ERROR]Update dsp malloc err:1\r\n");
        #endif
        return FALSE;
    }

    #if(dynamic_memory_request_test)
    DebugPrintf(TRUE,"firmware_update_pac_packet_info_store_0_malloc%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
    #endif

    //debug_updata_test("[updata] flash addr 0x%x\r\n", dw_firmware_data_spi_flash_addr);
	if(NULL == memset(pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketIndex,0x00,4))
	{
		vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"firmware_update_pac_packet_info_store_0_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
	}

    if(FALSE == bytes_to_int_big_to_little_endain(pst_firmware_packet->st_firmware_packet_pacx_dsp.ba_packet_number,
                                                  (uint32_t *)&dw_package_num_temp,
                                                  4))
    {
		vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"firmware_update_pac_packet_info_store_2_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }

    dw_package_sum_temp =\
        pst_device_update_info_current->dw_packets_number;

    if(dw_package_sum_temp < dw_package_num_temp)
    {
		vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"firmware_update_pac_packet_info_store_4_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }

	//small to big edain
	if(FALSE == byte_fill_in_reverse_order(4,
                                           pst_firmware_packet->st_firmware_packet_pacx_dsp.ba_packet_number,
                                           pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketIndex,
                                           4))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"firmware_update_pac_packet_info_store_7_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }

    memcpy((char *)&pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.bData[4],
           (char *)&pst_firmware_packet->st_firmware_packet_pacx_dsp.un_pacx_dsp_packet.st_pacx_dsp_packet_info.ba_header,
           w_firmware_packet_len);

    *((uint32_t *)&pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.bData[0]) = dw_package_sum_temp;

    w_data_len = w_firmware_packet_len + 4;
    if(FALSE == calculate_crc16(pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.bData,
                                w_data_len,
                                &pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.bData[w_data_len],
                                &pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.bData[w_data_len + 1]))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"firmware_update_pac_packet_info_store_7_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }
    w_data_len += 2;

    pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketDataLen[0] = (uint8_t)(w_data_len & 0x00FF);
    pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketDataLen[1] = (uint8_t)((w_data_len & 0xFF00) >> 8);

    DebugPrintf(TRUE,"firmware_update_pac_packet_info_store,store_data_start,time: %d\n",xTaskGetTickCount());
    if(FALSE == extern_flash_wirte(dw_firmware_data_spi_flash_addr,(w_data_len + 6),pstLocalUpdateSpiFlashFirmwarePacket->bData))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"firmware_update_pac_packet_info_store_8_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        debug_updata_error("\r\n[updata]flash fail LINE is : %d\r\n",__LINE__);
        return FALSE;
    }
    DebugPrintf(TRUE,"firmware_update_pac_packet_info_store,store_data_end,time: %d\n",xTaskGetTickCount());

    dw_firmware_data_spi_flash_addr += w_data_len + 6;

    {
		pst_device_update_info_current->
        pst_net_device_update_info_current->
        st_net_firmware_info.
        dw_packets_receive_current++;

        pst_device_update_info_current->
        pst_net_device_update_info_current->
        st_net_firmware_info.b_packet_format = pst_device_update_info_current->b_packet_format;

        pst_device_update_info_current->
        pst_net_device_update_info_current->
        st_net_firmware_info.
        dw_packets_receive_total =\
        pst_device_update_info_current->dw_packets_number;

        memcpy(pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info.ba_firmware_id,\
               pst_device_update_info_current->ba_firmware_id,\
               cb_firmware_id_max_len);
    }

    vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
    #if(dynamic_memory_request_test)
    DebugPrintf(TRUE,"firmware_update_pac_packet_info_store_9_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
    #endif

    return TRUE;
}
uint8_t b_firmware_update_pacx_dsp_packet_store_finish_flag = 0;
uint8_t firmware_update_pacx_dsp_packet_data_store(uint16_t w_firmware_packet_len,
                                                   firmware_packet *pst_firmware_packet,
                                                   device_update_info_current *pst_device_update_info_current)
{
    uint16_t w_data_len = 0;
    static uint16_t w_packet_index_backup = 1;
    static uint16_t w_data_len_backup = 0;

    LocalUpdateSpiFlashFirmwarePacket *pstLocalUpdateSpiFlashFirmwarePacket = 0;

//    do{
//        pstLocalUpdateSpiFlashFirmwarePacket = (LocalUpdateSpiFlashFirmwarePacket *)pvPortMalloc(sizeof(LocalUpdateSpiFlashFirmwarePacket));
//    }while(NULL == pstLocalUpdateSpiFlashFirmwarePacket);
    pstLocalUpdateSpiFlashFirmwarePacket = (LocalUpdateSpiFlashFirmwarePacket *)pvPortMalloc(sizeof(LocalUpdateSpiFlashFirmwarePacket));
    if(NULL == pstLocalUpdateSpiFlashFirmwarePacket)
    {
        #if( system_alarm_print)
        DebugPrintf(TRUE,"[ERROR]Update dsp malloc err:2\r\n");
        #endif
        return FALSE;
    }
    #if(dynamic_memory_request_test)
    DebugPrintf(TRUE,"local_firmware_update_packet_data_store_0_malloc%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
    #endif

	if(FALSE == byte_fill_in_reverse_order(4,
                                           pst_firmware_packet->st_firmware_packet_pacx_dsp.ba_packet_number,
                                           pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketIndex,
                                           4))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"local_firmware_update_packet_data_store_0_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }

    *((uint32_t *)&pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.bData[0]) = pst_device_update_info_current->dw_packets_number;

    w_data_len += 4;

    for(uint16_t w_cnt_i = 0;w_cnt_i < w_firmware_packet_len;w_cnt_i++)
    {
        pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.data.bPacketData[w_cnt_i+4] =\
			pst_firmware_packet->st_firmware_packet_pacx_dsp.un_pacx_dsp_packet.st_pacx_packet_data.b_packet_data[w_cnt_i];
    }
    w_data_len += w_firmware_packet_len;

    if(FALSE == calculate_crc16(pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.data.bPacketData,
                                w_data_len,
                                &pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.data.bPacketData[w_data_len],
                                &pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.data.bPacketData[w_data_len + 1]))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"local_firmware_update_packet_data_store_1_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }
    w_data_len += 2;

    pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketDataLen[0] = (uint8_t)(w_data_len & 0x00FF);
    pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketDataLen[1] = (uint8_t)((w_data_len & 0xFF00) >> 8);

    if(w_packet_index_backup == pst_device_update_info_current->
                                pst_net_device_update_info_current->
                                st_net_firmware_info.
                                dw_packets_receive_current)
    {
        uint8_t ba_packet_temp[cdwLocalUpdateFirmDataLen] = {0,};

        extern_flash_read((dw_firmware_data_spi_flash_addr - w_data_len_backup - 6),
                          (w_data_len_backup + 6),
                          ba_packet_temp);
        if(0 == memcmp((char *)ba_packet_temp,
                       (char *)pstLocalUpdateSpiFlashFirmwarePacket->bData,
                       w_data_len_backup + 6))
        {
            vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"local_firmware_update_packet_data_store_3_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
            #endif
            debug_updata_error("[updata]line %d\r\n",__LINE__);
            return TRUE;
        }
        else
        {
            vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"local_firmware_update_packet_data_store_3_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
            #endif
            debug_updata_error("[updata]line %d\r\n",__LINE__);
            return FALSE;
        }
    }


//    debug_updata_test("[updata] fire addr %x\r\n", dw_firmware_data_spi_flash_addr);
    if(FALSE == extern_flash_wirte(dw_firmware_data_spi_flash_addr,(w_data_len + 6),pstLocalUpdateSpiFlashFirmwarePacket->bData))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"local_firmware_update_packet_data_store_2_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }
    {
        w_packet_index_backup = pst_device_update_info_current->
                                   pst_net_device_update_info_current->
                                   st_net_firmware_info.
                                   dw_packets_receive_current;
        w_data_len_backup = w_data_len;
    }

    dw_firmware_data_spi_flash_addr += w_data_len + 6;

//    DebugPrintf(TRUE,"local_firmware_update_packet_data_store,store_data_end,time: %d\n",xTaskGetTickCount());

    {
        if(pst_device_update_info_current->
           pst_net_device_update_info_current->
           st_net_firmware_info.
           dw_packets_receive_current ==\
           pst_device_update_info_current->
           pst_net_device_update_info_current->
           st_net_firmware_info.
           dw_packets_receive_total)
        {
            b_local_firmware_enable = TRUE;
            w_packet_index_backup = 1;
            w_data_len_backup = 0;
            fWriteEEPage(15) = TRUE;
			DebugPrintf(TRUE,"dw_packets_receive_current = dw_packets_receive_total&& b_local_firmware_enable == TRUE \n");
        }
    }
	if(pst_device_update_info_current->
           pst_net_device_update_info_current->
           st_net_firmware_info.
           dw_packets_receive_current ==\
           pst_device_update_info_current->
           pst_net_device_update_info_current->
           st_net_firmware_info.
           dw_packets_receive_total -2)
	{
		b_firmware_update_pacx_dsp_packet_store_finish_flag = 1;
		#if( update_status_print )
			DebugPrintf(TRUE,"[Update]dw_packets_receive_current is : %d,dw_packets_receive_total is %d\r\n",
			pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info.dw_packets_receive_current,
			pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info.dw_packets_receive_total);
		#endif
	}

    vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
    #if(dynamic_memory_request_test)
    DebugPrintf(TRUE,"local_firmware_update_packet_data_store_4_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
    #endif


    return TRUE;
}

/* -------------------------------------------------------------------------- */
/*                               PACX MCU固件消息接收                               */
/* -------------------------------------------------------------------------- */
uint8_t local_firmware_update_packet_info_store(uint16_t w_firmware_packet_len,
                                                firmware_packet *pst_firmware_packet,
												device_update_info_current *pst_device_update_info_current)
{
    uint16_t w_data_len = 0;

    LocalUpdateSpiFlashFirmwarePacket *pstLocalUpdateSpiFlashFirmwarePacket = 0;

    dw_firmware_data_spi_flash_addr = cdwDevFirmAddrStart;
    if(FALSE == extern_flash_wirte(dw_firmware_data_spi_flash_addr,1,(uint8_t*)&flash_flag_start)){
        debug_updata_error("[flash]write flag fail \r\n");
        return FALSE;
    }
    dw_firmware_data_spi_flash_addr += cbFlashFirmInfoLen;

//    do{
//        pstLocalUpdateSpiFlashFirmwarePacket = (LocalUpdateSpiFlashFirmwarePacket *)pvPortMalloc(sizeof(LocalUpdateSpiFlashFirmwarePacket));
//    }while(NULL == pstLocalUpdateSpiFlashFirmwarePacket);
    pstLocalUpdateSpiFlashFirmwarePacket = (LocalUpdateSpiFlashFirmwarePacket *)pvPortMalloc(sizeof(LocalUpdateSpiFlashFirmwarePacket));
    if(NULL == pstLocalUpdateSpiFlashFirmwarePacket)
    {
        #if( system_alarm_print)
        DebugPrintf(TRUE,"[ERROR]Update pacx malloc err:1\r\n");
        #endif
        return FALSE;
    }
    #if(dynamic_memory_request_test)
    DebugPrintf(TRUE,"local_firmware_update_packet_info_store_store_0_malloc%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
    #endif


    //debug_updata_test("[updata] flash addr 0x%x\r\n", dw_firmware_data_spi_flash_addr);
	if(NULL == memset(pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketIndex,0x00,4))
	{
		vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"local_firmware_update_packet_info_store_0_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
	}
	//small to big edain
	if(FALSE == byte_fill_in_reverse_order(4,
                                           pst_firmware_packet->st_firmware_packet_pacx.ba_packet_number,
                                           pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketIndex,
                                           4))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"local_firmware_update_packet_info_store_1_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }
//    debug_updata_test("[updata]pack num \r\n");
//    debug_updata_sector(pst_firmware_packet->st_firmware_packet_pacx.ba_packet_number,4);
//    debug_updata_sector(pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketIndex,4);

    if(FALSE == int_to_bytes_little_endain(pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.info.bPacketsNumber,
                                           pst_device_update_info_current->dw_packets_number,
                                           4))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"local_firmware_update_packet_info_store_2_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }
//    debug_updata_test("[updata]packets totale number %d\r\n",pst_device_update_info_current->dw_packets_number);
//    debug_updata_sector(pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.info.bPacketsNumber,4);
    w_data_len += 4;

    pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.info.bMcuType =\
        pst_firmware_packet->st_firmware_packet_pacx.un_pacx_packet.st_pacx_packet_info.b_mcu_type;
    w_data_len += 1;

    if(FALSE == byte_fill_in_reverse_order(3,
                                           pst_firmware_packet->st_firmware_packet_pacx.un_pacx_packet.st_pacx_packet_info.ba_firmware_type,
                                           pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.info.bFirmwareType,
                                           3))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"local_firmware_update_packet_info_store_3_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }
    w_data_len += 3;
//    debug_updata_test("[updata]ba_firmware_type\r\n");
//    debug_updata_sector(pst_firmware_packet->st_firmware_packet_pacx.un_pacx_packet.st_pacx_packet_info.ba_firmware_type,3);
//    debug_updata_sector(pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.info.bFirmwareType,3);

    if(FALSE == byte_fill_in_reverse_order(3,
                                           pst_firmware_packet->st_firmware_packet_pacx.un_pacx_packet.st_pacx_packet_info.ba_firmware_app_version,
                                           pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.info.bAppVersion,
                                           3))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"local_firmware_update_packet_info_store_4_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }
    w_data_len += 3;
//    debug_updata_test("[updata]ba_firmware_app_version\r\n");
//    debug_updata_sector(pst_firmware_packet->st_firmware_packet_pacx.un_pacx_packet.st_pacx_packet_info.ba_firmware_app_version,3);
//    debug_updata_sector(pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.info.bAppVersion,3);
    if(FALSE == byte_fill_in_reverse_order(2,
                                           pst_firmware_packet->st_firmware_packet_pacx.un_pacx_packet.st_pacx_packet_info.ba_firmware_boot_version,
                                           pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.info.bbootVersion,
                                           2))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"local_firmware_update_packet_info_store_5_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }
    w_data_len += 2;

    if(0 == memcpy(pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.info.bkey,\
                   pst_firmware_packet->st_firmware_packet_pacx.un_pacx_packet.st_pacx_packet_info.ba_encryption_key,\
                   cbLocalFirmwarePacketKeyLen))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"local_firmware_update_packet_info_store_6_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }
    w_data_len += 12;

    pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.info.bUpdateType =\
        pst_firmware_packet->st_firmware_packet_pacx.un_pacx_packet.st_pacx_packet_info.b_encryption_type;
    w_data_len += 1;

    if(FALSE == calculate_crc16(pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.bData,
                                w_data_len,
                                &pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.info.bCrc16[0],
                                &pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.info.bCrc16[1]))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"local_firmware_update_packet_info_store_7_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }
    w_data_len += 2;

    pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketDataLen[0] = (uint8_t)(w_data_len & 0x00FF);
    pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketDataLen[1] = (uint8_t)((w_data_len & 0xFF00) >> 8);

    DebugPrintf(TRUE,"local_firmware_update_packet_info_store,store_data_start,time: %d\n",xTaskGetTickCount());
    if(FALSE == extern_flash_wirte(dw_firmware_data_spi_flash_addr,(w_data_len + 6),pstLocalUpdateSpiFlashFirmwarePacket->bData))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"local_firmware_update_packet_info_store_8_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        debug_updata_error("\r\n[updata]flash fail LINE is : %d\r\n",__LINE__);
        return FALSE;
    }
    DebugPrintf(TRUE,"local_firmware_update_packet_info_store,store_data_end,time: %d\n",xTaskGetTickCount());

    dw_firmware_data_spi_flash_addr += w_data_len + 6;

    {
		pst_device_update_info_current->
        pst_net_device_update_info_current->
        st_net_firmware_info.
        dw_packets_receive_current++;

        pst_device_update_info_current->
        pst_net_device_update_info_current->
        st_net_firmware_info.b_packet_format = pst_device_update_info_current->b_packet_format;

        pst_device_update_info_current->
        pst_net_device_update_info_current->
        st_net_firmware_info.
        dw_packets_receive_total =\
        pst_device_update_info_current->dw_packets_number;

        memcpy(pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info.ba_firmware_id,\
               pst_device_update_info_current->ba_firmware_id,\
               cb_firmware_id_max_len);
    }

    vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
    #if(dynamic_memory_request_test)
    DebugPrintf(TRUE,"local_firmware_update_packet_info_store_9_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
    #endif
    return TRUE;
}

/* -------------------------------------------------------------------------- */
/*                              PACX MCU类型固件包无线接收                             */
/* -------------------------------------------------------------------------- */
uint8_t local_firmware_update_packet_data_store(uint16_t w_firmware_packet_len,
												firmware_packet *pst_firmware_packet,
												device_update_info_current *pst_device_update_info_current)
{
    uint16_t w_data_len = 0;
    static uint16_t w_packet_index_backup = 1;
    static uint16_t w_data_len_backup = 0;

    LocalUpdateSpiFlashFirmwarePacket *pstLocalUpdateSpiFlashFirmwarePacket = 0;

//    do{
//        pstLocalUpdateSpiFlashFirmwarePacket = (LocalUpdateSpiFlashFirmwarePacket *)pvPortMalloc(sizeof(LocalUpdateSpiFlashFirmwarePacket));
//    }while(NULL == pstLocalUpdateSpiFlashFirmwarePacket);
    pstLocalUpdateSpiFlashFirmwarePacket = (LocalUpdateSpiFlashFirmwarePacket *)pvPortMalloc(sizeof(LocalUpdateSpiFlashFirmwarePacket));
    if(NULL == pstLocalUpdateSpiFlashFirmwarePacket)
    {
        #if( system_alarm_print)
        DebugPrintf(TRUE,"[ERROR]Update pacx malloc err:2\r\n");
        #endif
        return FALSE;
    }
    #if(dynamic_memory_request_test)
    DebugPrintf(TRUE,"local_firmware_update_packet_data_store_0_malloc%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
    #endif

	if(FALSE == byte_fill_in_reverse_order(4,
                                           pst_firmware_packet->st_firmware_packet_pacx.ba_packet_number,
                                           pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketIndex,
                                           4))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"local_firmware_update_packet_data_store_0_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }

//    debug_updata_test("[updata] pack num\r\n");
//    debug_updata_sector( pst_firmware_packet->st_firmware_packet_pacx.ba_packet_number,4);
//    debug_updata_sector(pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketIndex,4);

    for(uint16_t w_cnt_i = 0;w_cnt_i < w_firmware_packet_len;w_cnt_i++)
    {
        pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.data.bPacketData[w_cnt_i] =\
			pst_firmware_packet->st_firmware_packet_pacx.un_pacx_packet.st_pacx_packet_data.b_packet_data[w_cnt_i];
    }
    w_data_len += w_firmware_packet_len;

    if(FALSE == calculate_crc16(pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.data.bPacketData,
                                w_data_len,
                                &pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.data.bPacketData[w_firmware_packet_len],
                                &pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.data.bPacketData[w_firmware_packet_len + 1]))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"local_firmware_update_packet_data_store_1_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }
    w_data_len += 2;

    pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketDataLen[0] = (uint8_t)(w_data_len & 0x00FF);
    pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketDataLen[1] = (uint8_t)((w_data_len & 0xFF00) >> 8);

    if(w_packet_index_backup == pst_device_update_info_current->
                                pst_net_device_update_info_current->
                                st_net_firmware_info.
                                dw_packets_receive_current)
    {
        uint8_t ba_packet_temp[cdwLocalUpdateFirmDataLen] = {0,};

        extern_flash_read((dw_firmware_data_spi_flash_addr - w_data_len_backup - 6),
                          (w_data_len_backup + 6),
                          ba_packet_temp);
        if(0 == memcmp((char *)ba_packet_temp,
                       (char *)pstLocalUpdateSpiFlashFirmwarePacket->bData,
                       w_data_len_backup + 6))
        {
            vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"local_firmware_update_packet_data_store_3_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
            #endif
            debug_updata_error("[updata]line %d\r\n",__LINE__);
            return TRUE;
        }
        else
        {
            vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
            #if(dynamic_memory_request_test)
            DebugPrintf(TRUE,"local_firmware_update_packet_data_store_3_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
            #endif
            debug_updata_error("[updata]line %d\r\n",__LINE__);
            return FALSE;
        }
    }


//    debug_updata_test("[updata] fire addr %x\r\n", dw_firmware_data_spi_flash_addr);
//    DebugPrintf(TRUE,"local_firmware_update_packet_data_store,store_data_start,time: %d\n",xTaskGetTickCount());
    if(FALSE == extern_flash_wirte(dw_firmware_data_spi_flash_addr,(w_data_len + 6),pstLocalUpdateSpiFlashFirmwarePacket->bData))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        #if(dynamic_memory_request_test)
        DebugPrintf(TRUE,"local_firmware_update_packet_data_store_2_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
        #endif
        return FALSE;
    }
    {
        w_packet_index_backup = pst_device_update_info_current->
                                   pst_net_device_update_info_current->
                                   st_net_firmware_info.
                                   dw_packets_receive_current;
        w_data_len_backup = w_data_len;
    }

    dw_firmware_data_spi_flash_addr += w_data_len + 6;

//    DebugPrintf(TRUE,"local_firmware_update_packet_data_store,store_data_end,time: %d\n",xTaskGetTickCount());

    {
//        pst_device_update_info_current->
//        pst_net_device_update_info_current->
//        st_net_firmware_info.
//        dw_packets_receive_current++;

        if(pst_device_update_info_current->
           pst_net_device_update_info_current->
           st_net_firmware_info.
           dw_packets_receive_current ==\
           pst_device_update_info_current->
           pst_net_device_update_info_current->
           st_net_firmware_info.
           dw_packets_receive_total)
        {
            b_local_firmware_enable = TRUE;
            w_packet_index_backup = 1;
            w_data_len_backup = 0;
            fWriteEEPage(15) = TRUE;
//            {
//                vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
//                #if(dynamic_memory_request_test)
//                DebugPrintf(TRUE,"local_firmware_update_packet_data_store_3_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
//                #endif
//                debug_updata_error("[updata]pack finish\r\n");
//                return TRUE;
//            }
////            if(FALSE == local_firmware_info_refresh(TRUE))
////            {
////                vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
////                #if(dynamic_memory_request_test)
////                DebugPrintf(TRUE,"local_firmware_update_packet_data_store_3_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
////                #endif
////                return FALSE;
////            }
        }
    }

    vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
    #if(dynamic_memory_request_test)
    DebugPrintf(TRUE,"local_firmware_update_packet_data_store_4_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
    #endif

    return TRUE;
}
uint8_t subset_update_delay_func(uint32_t dw_dalay_time_ms)
{
    static uint8_t f_init = TRUE;
    static TickType_t x_start_time = 0;
    TickType_t x_curent_time = 0;

    if(f_init)
    {
        f_init = FALSE;
        x_start_time = xTaskGetTickCount();
    }
    x_curent_time = xTaskGetTickCount();

    if((x_curent_time - x_start_time) >= pdMS_TO_TICKS_USER(dw_dalay_time_ms))
    {
        f_init = TRUE;
        return TRUE;
    }
    return FALSE;
}
/* -------------------------------------------------------------------------- */
/*                                  烧录指令发送函数                                  */
/* -------------------------------------------------------------------------- */
uint8_t subset_update_send_update_cmd(uint8_t b_firmware_format,
                                      uint8_t b_device_addr,
                                      uint16_t w_cmd,
                                      uint16_t w_data_in_len,
                                      uint8_t *pb_data_in,
                                      net_device_info *pst_net_device_info)
{
    if((0 == w_data_in_len) ||\
       (NULL == pb_data_in))
    {
        return FALSE;
    }
    uint8_t b_cmd_type = 0;
    uint16_t w_cmd_temp = 0;

    switch(b_firmware_format)
    {
        case cb_firmware_format_pac         :
            {


                if((cw_subset_update_pac_cmd_send_lcd_enable == w_cmd) || (cw_subset_update_pac_cmd_send_mppt_enable == w_cmd))
                {
                  b_cmd_type = cb_wired_network_set_cmd;
                }
				else if(cw_subset_update_pac_sp_cmd_change_lcd_buadrate == w_cmd)
				{
					b_cmd_type = cb_wired_network_subset_update_pacx_dsp_cmd;
				}
                else
				{
					b_cmd_type = cb_wired_network_subset_update_pac_cmd;
				}
                w_cmd_temp = w_cmd  ;

//                w_cmd_temp = w_cmd;
//                b_cmd_type = cb_wired_network_subset_update_pac_cmd;
            }
            break;
        case cb_firmware_format_pacx_mcu    :
            {
                w_cmd_temp = w_cmd;
                b_cmd_type = cb_wired_network_subset_update_pacx_mcu_cmd;
            }
            break;
        case cb_firmware_format_pacx_dsp    :
            {
                if(cw_subset_update_pacx_dsp_cmd_enable_subset == w_cmd)
                {
                    w_cmd_temp = cw_subset_update_pacx_dsp_cmd_common;
                    b_cmd_type = cb_wired_network_subset_update_pacx_dsp_cmd;
                }
				else if(cw_subset_update_pacx_dsp_cmd_send_mppt_enable == w_cmd)
				{
                    w_cmd_temp = cw_subset_update_pacx_dsp_cmd_send_mppt_enable;
                    b_cmd_type = cb_wired_network_subset_update_pacx_dsp_cmd;

				}
                else
                {
                    b_cmd_type = cb_wired_network_subset_update_transparent_cmd;
                }
            }
            break;
        default:
            return FALSE;
    }
    return net_device_send_data(pst_net_device_info->b_wired_communication_cable,
                                b_cmd_type,
                                b_device_addr,
                                w_cmd_temp,
                                w_data_in_len,
                                (uint8_t *)pb_data_in,
                                pst_net_device_info);
}

/* -------------------------------------------------------------------------- */
/*                                  烧录指令回复接收                                  */
/* -------------------------------------------------------------------------- */

uint8_t subset_update_receive_update_response(uint8_t b_firmware_format,
                                              uint8_t b_device_addr,
                                              uint16_t w_cmd,
                                              uint16_t w_data_in_len,
                                              uint8_t *pb_data_in,
                                              uint32_t dw_receive_timeout_ms,
                                              net_device_info *pst_net_device_info)
{
    if(NULL == pb_data_in)
    {
        return cb_subset_update_com_fail;
    }
    else if(0 == w_data_in_len)
    {
        return cb_subset_update_com_success;
    }

	const static uint8_t ba_char_buff[cb_max_subset_update_rx_cmd_len] = {"\r\nProgram Running"};
	const static uint8_t ba_char_buff_1[cb_max_subset_update_rx_cmd_len] ={"\r\nDSP Reset an Runnig"};
	const static uint8_t ba_buff[2]  = {0x00,0x41};
	const static uint8_t ba_buff1[2] = {0x00,0xFE};
	const static uint8_t ba_buff2[2] = {0x00,0x4B};
    uint8_t b_cmd_type = 0;
    uint16_t w_receive_data_len = 0;
    uint8_t b_receive_state;
    static uint8_t ba_receive_buff[cb_max_subset_update_rx_cmd_len] = {0,};
    uint16_t w_cmd_temp = 0;
    static TickType_t x_start_time = 0;
    TickType_t x_curent_time = 0;
    static uint8_t f_init = TRUE;
    switch(b_firmware_format)
    {
        case cb_firmware_format_pac         :
            {
                if((cw_subset_update_pac_cmd_send_lcd_enable  == w_cmd) || (cw_subset_update_pac_cmd_send_mppt_enable == w_cmd))
                {
                    b_cmd_type = cb_wired_network_set_cmd;
                    w_cmd_temp = w_cmd ;
                }
				else if(cw_subset_update_pac_sp_cmd_change_lcd_buadrate  == w_cmd)
                {
                    b_cmd_type = cb_wired_network_subset_update_pacx_dsp_cmd;
                    w_cmd_temp = cw_subset_update_pacx_dsp_cmd_common ;
                }
                else
                {
                    b_cmd_type = cb_wired_network_subset_update_pac_cmd;
                    w_cmd_temp = w_cmd | 0x8000 ;
                }
            }
            break;
        case cb_firmware_format_pacx_mcu    :
            {
                w_cmd_temp = w_cmd | 0x8000;
                b_cmd_type = cb_wired_network_subset_update_pacx_mcu_cmd;
            }
            break;
        case cb_firmware_format_pacx_dsp    :
            {
                if(cw_subset_update_pacx_dsp_cmd_enable_subset == w_cmd )
                {
                    w_cmd_temp = cw_subset_update_pacx_dsp_cmd_common;
                    b_cmd_type = cb_wired_network_subset_update_pacx_dsp_cmd;
                }
				else if(cw_subset_update_pacx_dsp_cmd_send_mppt_enable == w_cmd)
				{
                    w_cmd_temp = cw_subset_update_pacx_dsp_cmd_send_mppt_enable;
                    b_cmd_type = cb_wired_network_subset_update_pacx_dsp_cmd;

				}
                else
                {
                    b_cmd_type = cb_wired_network_subset_update_transparent_cmd;
                }
            }
            break;
        default:
            return cb_subset_update_com_fail;
    }
    if(TRUE == net_device_receive_data(pst_net_device_info->b_wired_communication_cable,
                                       b_cmd_type,
                                       b_device_addr,
                                       w_cmd_temp,
                                       &w_receive_data_len,
                                       ba_receive_buff,
                                       cb_max_subset_update_rx_cmd_len,
                                       &b_receive_state,
                                       pst_net_device_info))
    {
        if(cb_wired_network_receive_state_success == b_receive_state)
        {
			if (w_receive_data_len >= w_data_in_len)
			{
				f_init = TRUE;
				if (0 == memcmp(ba_receive_buff, pb_data_in, w_data_in_len) || 0 == memcmp(ba_receive_buff, ba_char_buff, 3)
					|| 0 == memcmp(ba_receive_buff, ba_char_buff_1, 3) || 0 == memcmp(ba_receive_buff, ba_buff, 2) || 0 == memcmp(ba_receive_buff, ba_buff1, 2) || 0 == memcmp(ba_receive_buff, ba_buff2, 2))
				{
					if(0 == memcmp(ba_receive_buff, ba_buff, 2))
					{
						b_pac_addr = 0x41;
					}
					else if(0 == memcmp(ba_receive_buff, ba_buff1, 2))
					{
						b_pac_addr = 0xFE;
					}
					else if(0 == memcmp(ba_receive_buff, ba_buff2, 2))
					{
						b_pac_addr = 0x4B;
					}
					#if( update_status_print )
						DebugPrintf(TRUE,"cb_subset_update_com_success\r\n");
					#endif
					return cb_subset_update_com_success;
				}
				else
				{
					#if( update_status_print )
						DebugPrintf(TRUE,"cb_subset_update_com_fail,cb_wired_network_receive_state_success == b_receive_state\r\n");
						DebugPrintf(TRUE,"[Update]receive_buff:%s\r\n",ba_receive_buff);
					#endif
					return cb_subset_update_com_fail;
				}
            }
        }
        else if(cb_wired_network_receive_state_fail == b_receive_state)
        {
            f_init = TRUE;
			#if( update_status_print )
				DebugPrintf(TRUE,"cb_subset_update_com_fail,cb_wired_network_receive_state_fail == b_receive_state\r\n");
				DebugPrintf(TRUE,"[Update]receive_buff:%s\r\n",ba_receive_buff);
			#endif
            return cb_subset_update_com_fail;
        }
        else
        {
            if(f_init)
            {
                f_init = FALSE;
                x_start_time = xTaskGetTickCount();
            }
            x_curent_time = xTaskGetTickCount();
            if((x_curent_time - x_start_time) >= dw_receive_timeout_ms)
            {
                f_init = TRUE;
				#if( update_status_print )
					DebugPrintf(TRUE,"cb_subset_update_com_fail,TimeOUT\r\n");
					DebugPrintf(TRUE,"[Update]receive_buff:%s\r\n",ba_receive_buff);
				#endif
                return cb_subset_update_com_fail;
            }
        }


    }
    return cb_subset_update_none;
}

uint8_t check_system_type(uint8_t w_system_type_in)
{
    uint16_t w_system_type = cb_inv_type_ck;
    uint16_t w_data_len = 2;
    
    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                           cb_device_data_access_data_type_status,
                           0x0125,
                           &w_data_len,
                           (uint8_t *)&w_system_type,
                           cb_device_data_access_operate_type_read);
    

    if((w_system_type_in == w_system_type) &&\
       (0 != w_data_len))
    {
        return TRUE;
    }
    return FALSE;
}

uint8_t b_update_device_mppt = 0;

extern uint8_t ble_ftp_updata_flag;
/* -------------------------------------------------------------------------- */
/*                                 PAC类型对下烧录主流程                                 */
/* -------------------------------------------------------------------------- */
uint8_t subset_update_firmware_format_pac(net_device_info *pst_net_device_info)
{
    #define cb_pac_update_step_enable_subset                    0
    #define cb_pac_update_step_identify                         1
    #define cb_pac_update_step_get_device_tree                  2
    #define cb_pac_update_step_update_start                     3
    #define cb_pac_update_step_set_sub_buadrate                 4
    #define cb_pac_update_step_set_local_buadrate               5
    #define cb_pac_update_step_send_package                     6
    #define cb_pac_update_step_send_package_over                7
    #define cb_pac_update_step_start_bus                        8
    #define cb_pac_update_step_update_success                   9
    #define cb_pac_update_step_enable_lcd                       10
    #define cb_pac_update_step_stop_bus                         11
    #define cb_pac_update_step_get_comm_addr                    12
	#define cb_pac_update_step_set_sub_lcd_buadrate				13
	#define cb_pac_update_step_set_sub_buadrate_9600			14
    
    static uint8_t b_update_step = cb_pac_update_step_enable_lcd;
    static uint8_t b_update_step_next = cb_pac_update_step_enable_lcd;
    static uint8_t f_send_cmd = FALSE;
    static uint8_t f_receive_comfirm = FALSE;
    static uint8_t b_retry_cnt = 0;
    static uint8_t b_send_addr = 0;
    static uint8_t b_receive_addr = 0;
    static uint32_t dw_firmware_read_addr = cdwPackFirmAddrStart;
    static uint16_t w_cmd;
    uint32_t dw_firmware_num = 1;
    uint32_t dw_firmware_sum = 0;
    static uint16_t w_package_len = 0;
    uint8_t b_result = cb_subset_update_none;
    static uint8_t bp_lcd_cook_enable_flag = 0;
	static uint16_t w_update_per_old = 0;
	static TickType_t xTicksToWait = 0;
    uint16_t w_wait_receive_delay = 0;
    uint8_t b_receive_retry_cnt = 0; 

    if(NULL == pst_net_device_info)
    {
        st_subset_update_info.w_update_per = 0;    //mark update error
        st_subset_update_info.w_update_state = cb_firmware_update_status_none;
        return cb_subset_update_fail;
    }

	if ((xTaskGetTickCount() - xTicksToWait) > pdMS_TO_TICKS_USER(6000 * 3))
	{
		w_update_per_old = 0;
	}
	xTicksToWait = xTaskGetTickCount();

	if(FALSE == f_send_cmd)
    {
        switch(b_update_step)
        {
            case cb_pac_update_step_enable_lcd:
            {
                if(cdw_device_type_mppt == (*pst_net_device_info->pdw_device_id & 0xFFFF00))    //判断是否是mppt
                {
                    st_subset_update_info.st_subset_update_tx_info.ba_data[0] = 0x00;           //tx发送指令的data部分，其余部分在rs485_protocal_tx_update_cmd_assamble中有赋值
                    st_subset_update_info.st_subset_update_tx_info.ba_data[1] = 0x05;
                    st_subset_update_info.st_subset_update_tx_info.ba_data[2] = 0x00;
                    st_subset_update_info.st_subset_update_tx_info.ba_data[3] = 0x01;
                    st_subset_update_info.st_subset_update_tx_info.w_data_len = 4;

                    st_subset_update_info.st_subset_update_rx_info.ba_data[0] = 0x00;           //rx默认接收指令的data部分需要同实际接收到的指令进行比较，其余同tx
                    st_subset_update_info.st_subset_update_rx_info.ba_data[1] = 0x05;
                    st_subset_update_info.st_subset_update_rx_info.ba_data[2] = 0x00;
                    st_subset_update_info.st_subset_update_rx_info.ba_data[3] = 0x01;
                    w_cmd = cw_subset_update_pac_cmd_send_mppt_enable;                          //cw_subset_update_pac_cmd_send_mppt_enable：0x0666 发送指令的cmd部分
                    b_send_addr = pst_net_device_info->dw_device_address_send;                  //发送地址
                    b_receive_addr = pst_net_device_info->dw_device_address_receive;            //接收地址    
                    b_update_step_next = cb_pac_update_step_stop_bus;
                }
                else
                {
                    st_subset_update_info.st_subset_update_tx_info.ba_data[0] = 0x00;
                    st_subset_update_info.st_subset_update_tx_info.ba_data[1] = 0x00;
                    st_subset_update_info.st_subset_update_tx_info.ba_data[2] = 0x00;
                    st_subset_update_info.st_subset_update_tx_info.ba_data[3] = 0x01;
                    st_subset_update_info.st_subset_update_tx_info.w_data_len = 4;

                    st_subset_update_info.st_subset_update_rx_info.ba_data[0] = 0x00;
                    st_subset_update_info.st_subset_update_rx_info.ba_data[1] = 0x00;
                    st_subset_update_info.st_subset_update_rx_info.ba_data[2] = 0x00;
                    st_subset_update_info.st_subset_update_rx_info.ba_data[3] = 0x01;
                    st_subset_update_info.st_subset_update_rx_info.w_data_len = 4;
                    w_cmd = cw_subset_update_pac_cmd_send_lcd_enable;          //cw_subset_update_pac_cmd_send_lcd_enable:0x060c
                    b_send_addr = pst_net_device_info->dw_device_address_send;
                    b_receive_addr = pst_net_device_info->dw_device_address_receive;
                    b_update_step_next = cb_pac_update_step_enable_subset;              
                }
                bp_lcd_cook_enable_flag = 1;
				st_subset_update_info.w_update_per = w_update_per_old;
				st_subset_update_info.w_update_state = cb_firmware_update_status_updating;
			}
                break;
            case cb_pac_update_step_stop_bus:
                {
                    st_subset_update_info.st_subset_update_tx_info.ba_data[0] = 0x00;
                    st_subset_update_info.st_subset_update_tx_info.ba_data[1] = 0x01;
                    st_subset_update_info.st_subset_update_tx_info.w_data_len = 2;

                    st_subset_update_info.st_subset_update_rx_info.w_data_len = 0;

                    w_cmd = cw_subset_update_pac_cmd_stop_bus;
                    b_update_step_next = cb_pac_update_step_enable_subset;
                    if(cdw_device_type_mppt == (*pst_net_device_info->pdw_device_id & 0xFFFF00))
                    {
                    	b_send_addr = b_receive_addr = 0xF7;
					}
					else
					{
                        b_send_addr = pst_net_device_info->dw_device_address_send;
                        b_receive_addr = pst_net_device_info->dw_device_address_receive;
					}
                }
                break;
            case cb_pac_update_step_enable_subset:
                {
                    st_subset_update_info.st_subset_update_tx_info.ba_data[0] = 0x00;
                    st_subset_update_info.st_subset_update_tx_info.ba_data[1] = 0x01;
                    st_subset_update_info.st_subset_update_tx_info.w_data_len = 2;

                    w_cmd = cw_subset_update_pac_cmd_start_bus;
					if(cdw_device_type_mppt == (*pst_net_device_info->pdw_device_id & 0xFFFF00))
                    {
                        st_subset_update_info.st_subset_update_rx_info.w_data_len = 0;
                        b_send_addr = b_receive_addr = gb_mppt_pac_enable_addr;
                        b_update_step_next = cb_pac_update_step_get_comm_addr;
					}
					else
					{
                        st_subset_update_info.st_subset_update_rx_info.ba_data[0] = 0x00;
                        st_subset_update_info.st_subset_update_rx_info.ba_data[1] = 0x00;
                        st_subset_update_info.st_subset_update_rx_info.w_data_len = 2;
                        b_update_step_next = cb_pac_update_step_identify;
                        b_send_addr = pst_net_device_info->dw_device_address_send;
                        b_receive_addr = pst_net_device_info->dw_device_address_receive;
					}
                }
                break;
            case cb_pac_update_step_get_comm_addr:
                {
                    st_subset_update_info.st_subset_update_tx_info.ba_data[0] = 0x00;
                    st_subset_update_info.st_subset_update_tx_info.ba_data[1] = 0x01;
                    st_subset_update_info.st_subset_update_tx_info.w_data_len = 2;

                    st_subset_update_info.st_subset_update_rx_info.ba_data[0] = 0x00;
                    st_subset_update_info.st_subset_update_rx_info.ba_data[1] = gb_mppt_pac_enable_addr;
                    st_subset_update_info.st_subset_update_rx_info.w_data_len = 2;

                    w_cmd = cw_subset_update_pac_cmd_get_comm_addr;
                    b_update_step_next = cb_pac_update_step_identify;
                    if(cdw_device_type_mppt == (*pst_net_device_info->pdw_device_id & 0xFFFF00))
                    {
                        b_send_addr = b_receive_addr = gb_mppt_pac_enable_addr;
					}
					else
					{
                        b_send_addr = pst_net_device_info->dw_device_address_send;
                        b_receive_addr = pst_net_device_info->dw_device_address_receive;
					}
                }
                break;
            case cb_pac_update_step_identify:
                {  
                    st_subset_update_info.st_subset_update_tx_info.ba_data[0] = 0xAA;
                    st_subset_update_info.st_subset_update_tx_info.ba_data[1] = 0xBB;
                    st_subset_update_info.st_subset_update_tx_info.w_data_len = 2;

                    st_subset_update_info.st_subset_update_rx_info.ba_data[0] = 0x00;
                    st_subset_update_info.st_subset_update_rx_info.ba_data[1] = 0x00;
                    st_subset_update_info.st_subset_update_rx_info.w_data_len = 2;

                    w_cmd = cw_subset_update_pac_cmd_identify;
					if(cdw_device_type_mppt == (*pst_net_device_info->pdw_device_id & 0xFFFF00))
                    {
                    	b_update_step_next = cb_pac_update_step_get_device_tree;
						b_send_addr = b_receive_addr = b_pac_addr;
                        
					}
					else
					{
                        b_update_step_next = cb_pac_update_step_update_start;
                        b_send_addr = pst_net_device_info->dw_device_address_send;
                        b_receive_addr = pst_net_device_info->dw_device_address_receive;
					}
                }
                break;
            case cb_pac_update_step_get_device_tree:
                {

                    if(cdw_device_type_mppt == (*pst_net_device_info->pdw_device_id & 0xFFFF00))
                    {
                        st_subset_update_info.st_subset_update_tx_info.ba_data[0] = 0x00;
                        st_subset_update_info.st_subset_update_tx_info.ba_data[1] = 0x01;
                        st_subset_update_info.st_subset_update_tx_info.w_data_len = 2;

                        st_subset_update_info.st_subset_update_rx_info.ba_data[0] = 0x00;
                        st_subset_update_info.st_subset_update_rx_info.ba_data[1] = 0x00;
                        st_subset_update_info.st_subset_update_rx_info.w_data_len = 2;

                        w_cmd = cw_subset_update_pac_cmd_get_device_tree;
                        b_update_step_next = cb_pac_update_step_update_start;
						b_send_addr = b_receive_addr = b_pac_addr;
					}
                }
                break;
            case cb_pac_update_step_update_start:
                {
                    if (ble_ftp_updata_flag == 0)
                    {
                        dw_firmware_read_addr = cdwPackFirmAddrStart;
                    }
                    else
                    {
                        dw_firmware_read_addr = ble_cdwPackFirmAddrStart;
                    }
                    if(0 == updata_data_read_by_addr(dw_firmware_read_addr,
                                                     ba_flash_data_read))
                    {
                        b_retry_cnt = 0;
                        b_result = cb_subset_update_fail;
                        b_update_step = cb_pac_update_step_enable_lcd;
//                        FIRMWARE_UPDATE_SET_BUADRATE_9600();
                        break;
                    }
                    st_subset_update_info.st_subset_update_tx_info.ba_data[0] = 0x01;
                    pac_packet *pst_pac_packet =\
                        (pac_packet *)&ba_flash_data_read[6 + 4];//4 pack sum

                    if(1 ==\
                            pst_pac_packet->
                            st_pac_packet_data.
                            st_firmware_pac_info.
                            b_update_type)
                    {
                        st_subset_update_info.st_subset_update_tx_info.ba_data[1] = 0x02;
                    }
                    else
                    {
                        st_subset_update_info.st_subset_update_tx_info.ba_data[1] = 0x01;
                    }
                    for(uint8_t b_cnt_i = 0;b_cnt_i < cb_firmware_pac_ver_str_len;b_cnt_i++)
                    {
                        st_subset_update_info.st_subset_update_tx_info.ba_data[2 + b_cnt_i] =\
                            pst_pac_packet->
                            st_pac_packet_data.
                            st_firmware_pac_info.
                            ba_firmware_ver_str[b_cnt_i];
                    }
                    st_subset_update_info.st_subset_update_tx_info.w_data_len = 32;

                    st_subset_update_info.st_subset_update_rx_info.ba_data[0] = 0x00;
                    st_subset_update_info.st_subset_update_rx_info.ba_data[1] = 0x00;
                    st_subset_update_info.st_subset_update_rx_info.w_data_len = 2;
                    
                    w_cmd = cw_subset_update_pac_cmd_update_start;
                    {
                        uint8_t b_system_protocal = 0;
                        check_system_protocal_type(&b_system_protocal);
                        if(cb_system_protocal_type_riiosunii == b_system_protocal)
                        {
							if(cdw_device_type_mppt == (*pst_net_device_info->pdw_device_id & 0xFFFF00))
							{
								b_update_step_next = cb_pac_update_step_set_sub_buadrate;
//								b_update_step_next = cb_pac_update_step_send_package;
							}
                            else 
                            {
                                b_update_step_next = cb_pac_update_step_set_sub_buadrate;
                            }

                        }
                        else
                        {
                            b_update_step_next = cb_pac_update_step_send_package;
                        }
                    }
                    display_management_parameter_send(normal_mode,com_led,device_com,TRUE);
                    if(cdw_device_type_mppt == (*pst_net_device_info->pdw_device_id & 0xFFFF00))
                    {
						b_send_addr = b_receive_addr = b_pac_addr;
					}
					else
					{
                        b_send_addr = pst_net_device_info->dw_device_address_send;
                        b_receive_addr = pst_net_device_info->dw_device_address_receive;
					}
					st_subset_update_info.w_update_per = 0;
					st_subset_update_info.w_update_state = cb_firmware_update_status_updating;
                }
                break;
            case cb_pac_update_step_set_sub_buadrate:
                {
                    st_subset_update_info.st_subset_update_tx_info.ba_data[0] = 0x00;
                    st_subset_update_info.st_subset_update_tx_info.ba_data[1] = 0x06;
                    st_subset_update_info.st_subset_update_tx_info.w_data_len = 2;

                    st_subset_update_info.st_subset_update_rx_info.ba_data[0] = 0x00;
                    st_subset_update_info.st_subset_update_rx_info.ba_data[1] = 0x00;
                    st_subset_update_info.st_subset_update_rx_info.w_data_len = 2;

                    w_cmd = cw_subset_update_pac_cmd_set_sub_buadrate;

                    if(cdw_device_type_mppt == (*pst_net_device_info->pdw_device_id & 0xFFFF00))
                    {
						b_send_addr = b_receive_addr = b_pac_addr;
						b_update_step_next = cb_pac_update_step_set_local_buadrate;
					}
					else
					{
					    b_update_step_next = cb_pac_update_step_set_local_buadrate;
                        b_send_addr = pst_net_device_info->dw_device_address_send;
                        b_receive_addr = pst_net_device_info->dw_device_address_receive;
					}
                }
                break;
			case cb_pac_update_step_set_sub_lcd_buadrate:
				{
					st_subset_update_info.st_subset_update_tx_info.ba_data[0] = 0x03;
					st_subset_update_info.st_subset_update_tx_info.ba_data[1] = 0x06;
					st_subset_update_info.st_subset_update_tx_info.w_data_len = 2;

					st_subset_update_info.st_subset_update_rx_info.ba_data[0] = 0x03;
					st_subset_update_info.st_subset_update_rx_info.ba_data[1] = 0x06;
					st_subset_update_info.st_subset_update_rx_info.w_data_len = 2;

					w_cmd = cw_subset_update_pac_sp_cmd_change_lcd_buadrate;
					b_update_step_next = cb_pac_update_step_set_local_buadrate;
					b_send_addr = pst_net_device_info->dw_device_address_send;
					b_receive_addr = pst_net_device_info->dw_device_address_receive;
				}
                break;
            case cb_pac_update_step_set_local_buadrate:
                {
                    FIRMWARE_UPDATE_SET_BUADRATE_115200();  //update task

                    st_subset_update_info.st_subset_update_tx_info.w_data_len = 0;
                    st_subset_update_info.st_subset_update_rx_info.w_data_len = 0;

                    b_update_step = cb_pac_update_step_send_package;
                    b_update_step_next = cb_pac_update_step_send_package;
                    w_cmd = cw_subset_update_pac_cmd_update_start;
                    if(cdw_device_type_mppt == (*pst_net_device_info->pdw_device_id & 0xFFFF00))
                    {
						b_send_addr = b_receive_addr = b_pac_addr;
					}
					else
					{
                        b_send_addr = pst_net_device_info->dw_device_address_send;
                        b_receive_addr = pst_net_device_info->dw_device_address_receive;
					}
                }
                break;
            case cb_pac_update_step_send_package:
                {
                    pac_packet *pst_pac_packet = 0;

                    if(f_receive_comfirm &&\
                      (w_cmd != cw_subset_update_pac_cmd_update_start))
                    {
                        f_receive_comfirm = FALSE;
                        if (ble_ftp_updata_flag == 0)
                        {
                            if (cdwPackFirmAddrStart == dw_firmware_read_addr)
                            {
                                dw_firmware_read_addr += w_package_len + 6 + 2 + 4;    // 2:crc 4:pack sum
                            }
                            else
                            {
                                dw_firmware_read_addr += w_package_len + 6 + 2;    // 2:crc
                            }
                        }
                        else
                        {
                            if (ble_cdwPackFirmAddrStart == dw_firmware_read_addr)
                            {
                                dw_firmware_read_addr += w_package_len + 6 + 2 + 4;    // 2:crc 4:pack sum
                            }
                            else
                            {
                                dw_firmware_read_addr += w_package_len + 6 + 2;    // 2:crc
                            }
                        }
                    }
                    if(0 == updata_data_read_by_addr(dw_firmware_read_addr,
                                                     ba_flash_data_read))
                    {
                        b_retry_cnt = 0;
                        b_result = cb_subset_update_fail;
                        b_update_step = cb_pac_update_step_enable_lcd;
                        FIRMWARE_UPDATE_SET_BUADRATE_9600();
                        break;
                    }
                    if (ble_ftp_updata_flag == 0)
                    {
                        if(cdwPackFirmAddrStart == dw_firmware_read_addr)
                        {
                            pst_pac_packet = (pac_packet *)&ba_flash_data_read[6 + 4];//4:pack sum
                        }
                        else
                        {
                            pst_pac_packet = (pac_packet *)&ba_flash_data_read[6];
                        }
                    }
                    else
                    {
                        if(ble_cdwPackFirmAddrStart == dw_firmware_read_addr)
                        {
                            pst_pac_packet = (pac_packet *)&ba_flash_data_read[6 + 4];//4:pack sum
                        }
                        else
                        {
                            pst_pac_packet = (pac_packet *)&ba_flash_data_read[6];
                        }
                    }
                    dw_firmware_num = (uint32_t)((pst_pac_packet->ba_package_num[0] << 24) |\
                                                 (pst_pac_packet->ba_package_num[1] << 16) |\
                                                 (pst_pac_packet->ba_package_num[2] << 8) |\
                                                 (pst_pac_packet->ba_package_num[3]));
                    dw_firmware_sum = (uint32_t)((pst_pac_packet->ba_package_sum[0] << 24) |\
                                                 (pst_pac_packet->ba_package_sum[1] << 16) |\
                                                 (pst_pac_packet->ba_package_sum[2] << 8) |\
                                                 (pst_pac_packet->ba_package_sum[3]));
                    w_package_len = (uint16_t)((pst_pac_packet->ba_package_len[0] << 8) |\
                                               (pst_pac_packet->ba_package_len[1])) + 10;
                    memcpy(st_subset_update_info.st_subset_update_tx_info.ba_data,
                           pst_pac_packet->ba_package_num,
                           w_package_len);
                    st_subset_update_info.st_subset_update_tx_info.w_data_len = w_package_len;

                    memcpy(st_subset_update_info.st_subset_update_rx_info.ba_data,
                           pst_pac_packet->ba_package_num,
                           4);
                    memcpy(&st_subset_update_info.st_subset_update_rx_info.ba_data[4],
                           pst_pac_packet->ba_package_sum,
                           4);
                    st_subset_update_info.st_subset_update_rx_info.ba_data[8] = 0x00;
                    st_subset_update_info.st_subset_update_rx_info.ba_data[9] = 0x00;
                    st_subset_update_info.st_subset_update_rx_info.w_data_len = 10;

                    st_subset_update_info.w_update_per = (dw_firmware_num * 100) / dw_firmware_sum;
					w_update_per_old = st_subset_update_info.w_update_per;
                    if(dw_firmware_num == dw_firmware_sum)
                    {
                        if(cdw_device_type_mppt == (*pst_net_device_info->pdw_device_id & 0xFFFF00))
	                    {
							b_update_step_next = cb_pac_update_step_set_sub_buadrate_9600;
						}
						else
						{
							b_update_step_next = cb_pac_update_step_send_package_over;
						}
                    }
                    w_cmd = cw_subset_update_pac_cmd_send_package;
                    if(cdw_device_type_mppt == (*pst_net_device_info->pdw_device_id & 0xFFFF00))
                    {
						b_send_addr = b_receive_addr = b_pac_addr;
					}
					else
					{
                        b_send_addr = pst_net_device_info->dw_device_address_send;
                        b_receive_addr = pst_net_device_info->dw_device_address_receive;
					}
                }
                break;
			case cb_pac_update_step_set_sub_buadrate_9600:
                {
                    st_subset_update_info.st_subset_update_tx_info.ba_data[0] = 0x00;
                    st_subset_update_info.st_subset_update_tx_info.ba_data[1] = 0x03;
                    st_subset_update_info.st_subset_update_tx_info.w_data_len = 2;

                    st_subset_update_info.st_subset_update_rx_info.ba_data[0] = 0x00;
                    st_subset_update_info.st_subset_update_rx_info.ba_data[1] = 0x00;
                    st_subset_update_info.st_subset_update_rx_info.w_data_len = 2;

                    w_cmd = cw_subset_update_pac_cmd_set_sub_buadrate;

                    if(cdw_device_type_mppt == (*pst_net_device_info->pdw_device_id & 0xFFFF00))
                    {
						b_send_addr = b_receive_addr = b_pac_addr;
						b_update_step_next = cb_pac_update_step_send_package_over;
					}
					else
					{
					    b_update_step_next = cb_pac_update_step_send_package_over;
                        b_send_addr = pst_net_device_info->dw_device_address_send;
                        b_receive_addr = pst_net_device_info->dw_device_address_receive;
					}
                }
                break;
            case cb_pac_update_step_send_package_over:
                {
                    if(cdw_device_type_mppt == (*pst_net_device_info->pdw_device_id & 0xFFFF00))
                    {
						FIRMWARE_UPDATE_SET_BUADRATE_9600();
						vTaskDelay(4000);
					}


                    st_subset_update_info.st_subset_update_tx_info.ba_data[0] = 0xAB;
                    st_subset_update_info.st_subset_update_tx_info.ba_data[1] = 0xAB;
                    st_subset_update_info.st_subset_update_tx_info.w_data_len = 2;

                    st_subset_update_info.st_subset_update_rx_info.ba_data[0] = 0x00;
                    st_subset_update_info.st_subset_update_rx_info.ba_data[1] = 0x00;
                    st_subset_update_info.st_subset_update_rx_info.w_data_len = 2;

                    w_cmd = cw_subset_update_pac_cmd_send_package_over;
                    
                    if(cdw_device_type_mppt == (*pst_net_device_info->pdw_device_id & 0xFFFF00))
                    {
                        b_update_step_next = cb_pac_update_step_update_success;
						b_send_addr = b_receive_addr = b_pac_addr;
					}
					else
					{
                        b_update_step_next = cb_pac_update_step_start_bus;
                        b_send_addr = pst_net_device_info->dw_device_address_send;
                        b_receive_addr = pst_net_device_info->dw_device_address_receive;
					}
                }
                break;
            case cb_pac_update_step_start_bus:
                {
                    st_subset_update_info.st_subset_update_tx_info.ba_data[0] = 0x00;
                    st_subset_update_info.st_subset_update_tx_info.ba_data[1] = 0x01;
                    st_subset_update_info.st_subset_update_tx_info.w_data_len = 2;

                    st_subset_update_info.st_subset_update_rx_info.ba_data[0] = 0x00;
                    st_subset_update_info.st_subset_update_rx_info.ba_data[1] = 0x00;
                    st_subset_update_info.st_subset_update_rx_info.w_data_len = 2;

                    w_cmd = cw_subset_update_pac_cmd_start_bus;
                    b_update_step_next = cb_pac_update_step_update_success;
                    if(cdw_device_type_mppt == (*pst_net_device_info->pdw_device_id & 0xFFFF00))
                    {
                        b_send_addr = b_receive_addr = 0xF7;
					}
					else
					{
                        b_send_addr = pst_net_device_info->dw_device_address_send;
                        b_receive_addr = pst_net_device_info->dw_device_address_receive;
					}
                }
                break;
            case cb_pac_update_step_update_success:
                {
                    f_receive_comfirm = FALSE;
                    f_send_cmd = FALSE;
                    dw_firmware_read_addr = cdwPackFirmAddrStart;
                    if (ble_ftp_updata_flag == 1)
                    {
                        dw_firmware_read_addr = ble_cdwPackFirmAddrStart;
                    }
                    b_update_step = cb_pac_update_step_enable_lcd;
                    b_update_step_next = cb_pac_update_step_enable_lcd;
                    st_subset_update_info.st_subset_update_tx_info.w_data_len = 0;
                    b_result = cb_subset_update_success;
                    b_retry_cnt = 0;
					w_update_per_old = 0;
                    FIRMWARE_UPDATE_SET_BUADRATE_9600();
                    if(cdw_device_type_mppt == (*pst_net_device_info->pdw_device_id & 0xFFFF00))
                    {
						b_send_addr = b_receive_addr = b_pac_addr;
					}
					else
					{
                        b_send_addr = pst_net_device_info->dw_device_address_send;
                        b_receive_addr = pst_net_device_info->dw_device_address_receive;
					}
                }
                break;
            default:
                {
                    b_result = cb_subset_update_fail;
                    b_retry_cnt = 0;
                    f_send_cmd = FALSE;
                    b_update_step = cb_pac_update_step_enable_lcd;
                    b_update_step_next = cb_pac_update_step_enable_lcd;
                    st_subset_update_info.st_subset_update_tx_info.w_data_len = 0;
                    FIRMWARE_UPDATE_SET_BUADRATE_9600();
                    if(cdw_device_type_mppt == (*pst_net_device_info->pdw_device_id & 0xFFFF00))
                    {
						b_send_addr = b_receive_addr = b_pac_addr;
					}
					else
					{
                        b_send_addr = pst_net_device_info->dw_device_address_send;
                        b_receive_addr = pst_net_device_info->dw_device_address_receive;
					}
                }
                break;
        }
    }
    {   //send func + receive func
        if(FALSE == f_send_cmd)
        {
            f_receive_comfirm = FALSE;
            if(0 != st_subset_update_info.st_subset_update_tx_info.w_data_len)
            {
                f_send_cmd = subset_update_send_update_cmd(cb_firmware_format_pac,
                                                           b_send_addr,
                                                           w_cmd,
                                                           st_subset_update_info.st_subset_update_tx_info.w_data_len,
                                                           st_subset_update_info.st_subset_update_tx_info.ba_data,
                                                           pst_net_device_info);
            }
        }
        else
        {
            if(check_system_type(cb_inv_type_riio_sun) || check_system_type(cb_inv_type_apollo_maxx))
            {
                if(w_cmd == cw_subset_update_pac_cmd_update_start)
				{
					w_wait_receive_delay = 300;
                	b_receive_retry_cnt = 10;
                }
				else
            	{
                	w_wait_receive_delay = 10000;
                	b_receive_retry_cnt = 3;
            	}
            }
            else if(check_system_type(cb_inv_type_riio_sun_ii_split_phase)  || check_system_type(cb_inv_type_riio_ii_split_phase) 
				&& (w_cmd == cw_subset_update_pac_cmd_update_start))		//split phase 烧录04指令逆变器不回，修改延时后ok
            {
				w_wait_receive_delay = 7000;
				b_receive_retry_cnt = 3;
            }
			else if(cdw_device_type_mppt == (*pst_net_device_info->pdw_device_id & 0xFFFF00) && w_cmd == cw_subset_update_pac_cmd_send_package_over)		//split phase 烧录04指令逆变器不回，修改延时后ok
            {
				w_wait_receive_delay = 1000;
				b_receive_retry_cnt = 3;
            }
			else
            {
                w_wait_receive_delay = 10000;
                b_receive_retry_cnt = 2;
            }
            b_result =\
                subset_update_receive_update_response(cb_firmware_format_pac,
                                                      b_receive_addr,
                                                      w_cmd,
                                                      st_subset_update_info.st_subset_update_rx_info.w_data_len,
                                                      st_subset_update_info.st_subset_update_rx_info.ba_data,
                                                      pdMS_TO_TICKS_USER(w_wait_receive_delay),
                                                      pst_net_device_info);
            if(cb_subset_update_com_success == b_result)
            {
                bp_lcd_cook_enable_flag = 0;
                b_retry_cnt = 0;
                f_send_cmd = FALSE;
                f_receive_comfirm = TRUE;
                b_update_step = b_update_step_next;
            }
            else if(cb_subset_update_com_fail == b_result)
            {
                b_retry_cnt++;
                f_send_cmd = FALSE;

                #if( update_status_print )
                DebugPrintf(TRUE,"[Update]Pac update com fail,cmd:%04X\r\n",w_cmd);
                #endif
                if(b_retry_cnt > b_receive_retry_cnt)
                {
                    b_retry_cnt = 0;
					if(w_cmd == cw_subset_update_pac_cmd_get_comm_addr)
					{
						if(check_system_type(cb_inv_type_apollo_matric_ii_lite))
						{
							b_pac_addr = 0x4B;
							gb_mppt_pac_enable_addr = b_pac_addr;
						}
						else
						{
							b_pac_addr = 0x41;
						}
						b_update_step = cb_pac_update_step_identify;
					}
                    if(bp_lcd_cook_enable_flag == 0)
                    {
                        b_update_step = cb_pac_update_step_enable_lcd;
                        b_result = cb_subset_update_fail;
//                        st_subset_update_info.w_update_per = 200;
//                        st_subset_update_info.w_update_state = cb_firmware_update_status_none;
//                        st_subset_update_info.w_update_per = 200;
						gb_update_firware_fail_flag = 1;
                    }
                    else if(bp_lcd_cook_enable_flag == 1)
                    { 
                        b_update_step = cb_pac_update_step_enable_subset;
                        bp_lcd_cook_enable_flag = 0;
                    }
//                    b_update_step = cb_pac_update_step_enable_subset;
                    FIRMWARE_UPDATE_SET_BUADRATE_9600();
                }
            }
        }
    }
    {//write update status
        uint16_t w_data_len = 2;
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_system],
                               cb_device_data_access_data_type_status,
                               cw_sys_status_data_addr_update_process,
                               &w_data_len,
                               (uint8_t *)&st_subset_update_info.w_update_per,
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_system],
                               cb_device_data_access_data_type_status,
                               cw_sys_status_data_addr_update_status,
                               &w_data_len,
                               (uint8_t *)&st_subset_update_info.w_update_state,
                               cb_device_data_access_operate_type_write);
    }

    return b_result;
}
/* -------------------------------------------------------------------------- */
/*                               PACX MCU类型对下烧录主流程                              */
/* -------------------------------------------------------------------------- */
uint8_t subset_update_firmware_format_pacx_mcu(net_device_info *pst_net_device_info)
{
    static uint8_t b_update_step = 0;
    uint8_t b_result = cb_subset_update_none;

    switch(b_update_step)
    {
        default:
            b_result = cb_subset_update_fail;
            break;
    }

    return b_result;
}
/* -------------------------------------------------------------------------- */
/*                                  DSP对下烧录主流程                                  */
/* -------------------------------------------------------------------------- */
uint8_t subset_update_firmware_format_pacx_dsp(net_device_info *pst_net_device_info)
{
    #define cb_pacx_dsp_update_step_wait_update                         0
    #define cb_pacx_dsp_update_step_set_sub_buad_rate_115200            1
    #define cb_pacx_dsp_update_step_set_local_buadrate_115200           2
    #define cb_pacx_dsp_update_step_enable_subset                       3
    #define cb_pacx_dsp_update_step_set_buadrate                        4
    #define cb_cb_pacx_dsp_update_step_wait_flash_erase                 5
    #define cb_pacx_dsp_update_step_send_firm_key                       6
    #define cb_pacx_dsp_update_step_send_firm_package                   7
    #define cb_pacx_dsp_update_step_wait_program                        8
    #define cb_pacx_dsp_update_step_send_package_over                   9
    #define cb_pacx_dsp_update_step_set_sub_buad_rate_9600              10
    #define cb_pacx_dsp_update_step_set_local_buadrate_9600             11
    #define cb_pacx_dsp_update_step_update_success                      12
	#define cb_pacx_dsp_update_step_enable_sp							13


    static uint8_t f_send_cmd = FALSE;
    static uint8_t b_retry_cnt = 0;
    static uint32_t dw_delay_cnt_ms = 3000;
    static uint16_t w_enable_delay_ms = 1400;
    static uint32_t dw_receive_timeout_ms = 500;
    static uint32_t dw_package_send_interval_ms = 500;
    static uint32_t dw_firmware_read_addr = cdwPackFirmAddrStart;
    static uint16_t w_cmd;
	static uint8_t b_cnt_j = 0;
    static uint8_t b_mppt_enable_flag = 0;
    uint32_t dw_firmware_num = 1;
    uint32_t dw_firmware_sum = 0;
    static uint32_t dw_subset_firmware_num = 0;
    static uint32_t dw_subset_firmware_sum = 0;
    static uint16_t w_package_len = 0;
    uint8_t b_result = cb_subset_update_none;
    static uint8_t bp_dsp_cook_finish_flag = 0;


	if(cdw_device_type_mppt == (*pst_net_device_info->pdw_device_id & 0xFFFF00))
	{

		static uint8_t b_send_addr = 0;
	    static uint8_t b_update_step = cb_pacx_dsp_update_step_wait_update;
    	static uint8_t b_update_step_next = cb_pacx_dsp_update_step_enable_sp;
	    if(0 == dw_subset_firmware_sum)
	    {
	        if(FALSE == read_send_subset_packet_num(cw_subset_update_max_receive_len,
	                                                &dw_subset_firmware_sum))
	        {
	            b_retry_cnt = 0;
	            b_result = cb_subset_update_fail;
	            b_update_step = cb_pacx_dsp_update_step_wait_update;
	            b_update_step_next = cb_pacx_dsp_update_step_enable_sp;
	            return FALSE;
	        }
	        st_subset_update_info.w_update_per = 0;
	        st_subset_update_info.w_update_state = cb_firmware_update_status_updating;
	        #if( update_status_print )
	        DebugPrintf(TRUE,"[Update]DSP_SP update package sum :%d\r\n",dw_subset_firmware_sum);
	        #endif
	    }
	    if(FALSE == f_send_cmd)
		{
			switch(b_update_step)
			{
				case cb_pacx_dsp_update_step_enable_sp:
	            {
                    st_subset_update_info.st_subset_update_tx_info.ba_data[0] = 0x00;           //tx发送指令的data部分，其余部分在rs485_protocal_tx_update_cmd_assamble中有赋值
                    st_subset_update_info.st_subset_update_tx_info.ba_data[1] = 0x05;
                    st_subset_update_info.st_subset_update_tx_info.ba_data[2] = 0x00;
                    st_subset_update_info.st_subset_update_tx_info.ba_data[3] = 0x01;
                    st_subset_update_info.st_subset_update_tx_info.w_data_len = 4;

                    st_subset_update_info.st_subset_update_rx_info.ba_data[0] = 0x00;           //rx默认接收指令的data部分需要同实际接收到的指令进行比较，其余同tx
                    st_subset_update_info.st_subset_update_rx_info.ba_data[1] = 0x05;
                    st_subset_update_info.st_subset_update_rx_info.ba_data[2] = 0x00;
                    st_subset_update_info.st_subset_update_rx_info.ba_data[3] = 0x01;
                    w_cmd = cw_subset_update_pacx_dsp_cmd_send_mppt_enable;                          //cw_subset_update_pac_cmd_send_mppt_enable：0x0666 发送指令的cmd部分  
					b_send_addr = pst_net_device_info->dw_device_address_send;
//					if(b_mppt_enable_flag > 1)
//					{
//						b_mppt_enable_flag = 0;
                        b_update_step = cb_pacx_dsp_update_step_wait_update;
						b_update_step_next = cb_pacx_dsp_update_step_set_sub_buad_rate_115200;
//					}
//					b_mppt_enable_flag ++;

					#if( update_status_print )
			        DebugPrintf(TRUE,"[Update]DSP_SP update enable \r\n");
			        #endif
				}
		        break;
				case cb_pacx_dsp_update_step_set_sub_buad_rate_115200:
					{
						uint8_t b_system_protocal = 0;
						check_system_protocal_type(&b_system_protocal);
						st_subset_update_info.w_update_per = 0;
						st_subset_update_info.w_update_state = cb_firmware_update_status_updating;
	
						if(cb_system_protocal_type_riiosunii == b_system_protocal)
						{
							st_subset_update_info.st_subset_update_tx_info.ba_data[0] = 0x03;
							st_subset_update_info.st_subset_update_tx_info.ba_data[1] = 0x06;
							st_subset_update_info.st_subset_update_tx_info.w_data_len = 2;
	
							st_subset_update_info.st_subset_update_rx_info.ba_data[0] = 0x03;
							st_subset_update_info.st_subset_update_rx_info.ba_data[1] = 0x06;
							st_subset_update_info.st_subset_update_rx_info.w_data_len = 2;
	
							w_cmd = cw_subset_update_pacx_dsp_cmd_enable_subset;
							b_update_step_next = cb_pacx_dsp_update_step_set_local_buadrate_115200;
							b_send_addr = pst_net_device_info->dw_device_address_send;
							dw_package_send_interval_ms = 150;			
							w_enable_delay_ms = 1500;
						}
						else
						{
							st_subset_update_info.st_subset_update_tx_info.w_data_len = 0;
							st_subset_update_info.st_subset_update_rx_info.w_data_len = 0;
							f_send_cmd = FALSE;
							b_update_step = cb_pacx_dsp_update_step_wait_update;
							b_update_step_next = cb_pacx_dsp_update_step_enable_subset;
							dw_package_send_interval_ms = 500;
							//w_enable_delay_ms = 1400;
							w_enable_delay_ms = 1313;
						}
						dw_delay_cnt_ms = 3000;
					}
					break;
				case cb_pacx_dsp_update_step_set_local_buadrate_115200:
					{
						st_subset_update_info.st_subset_update_tx_info.w_data_len = 0;
						st_subset_update_info.st_subset_update_rx_info.w_data_len = 0;
						f_send_cmd = FALSE;
						b_update_step = cb_pacx_dsp_update_step_wait_update;
						b_update_step_next = cb_pacx_dsp_update_step_enable_subset;
						FIRMWARE_UPDATE_SET_BUADRATE_115200();
					}
					break;
				case cb_pacx_dsp_update_step_wait_update:
					{
	
						st_subset_update_info.st_subset_update_tx_info.w_data_len = 0;
						st_subset_update_info.st_subset_update_rx_info.w_data_len = 0;
						f_send_cmd = FALSE;
						if(TRUE == subset_update_delay_func(dw_delay_cnt_ms))
						{
							if(b_update_step_next == cb_pacx_dsp_update_step_wait_update)
							{
								b_update_step_next = cb_pacx_dsp_update_step_set_sub_buad_rate_9600;
							}
							b_update_step = b_update_step_next;
						}
					}
					break;
				case cb_pacx_dsp_update_step_enable_subset:
					{
						st_subset_update_info.st_subset_update_tx_info.ba_data[0] = 0x01;
						st_subset_update_info.st_subset_update_tx_info.ba_data[1] = 0x02;
						st_subset_update_info.st_subset_update_tx_info.w_data_len = 2;
						b_send_addr = gb_iner_mppt_id;

						st_subset_update_info.st_subset_update_rx_info.w_data_len = 0;
	
						w_cmd = cw_subset_update_pacx_dsp_cmd_enable_subset;
	
						dw_delay_cnt_ms = w_enable_delay_ms;//1500 ms//1.4 + 0.1s
	
						b_update_step = cb_pacx_dsp_update_step_wait_update;
						b_update_step_next = cb_pacx_dsp_update_step_set_buadrate;
					}
					break;
				case cb_pacx_dsp_update_step_set_buadrate:
					{
						uint16_t w_data_len = 0;
						uint8_t ba_data_temp[4] = {0xAA,0x55,0x55,0xAA,};
						rs485_protocal_tx_data_assamble(gb_iner_mppt_id,
														cw_subset_update_pacx_dsp_cmd_common,
														4,
														ba_data_temp,
														st_subset_update_info.st_subset_update_tx_info.ba_data,
														&w_data_len);
	
						st_subset_update_info.st_subset_update_tx_info.w_data_len = w_data_len;
	
						memset(ba_data_temp,
							   0xCD,
							   4);
						rs485_protocal_tx_data_assamble(gb_iner_mppt_id,
														cw_subset_update_pacx_dsp_cmd_common,
														4,
														ba_data_temp,
														&st_subset_update_info.st_subset_update_tx_info.ba_data[w_data_len],
														&w_data_len);
						st_subset_update_info.st_subset_update_tx_info.w_data_len += w_data_len;
	
						st_subset_update_info.st_subset_update_rx_info.w_data_len =\
							sprintf((char *)st_subset_update_info.st_subset_update_rx_info.ba_data,"\r\nErasing Flash");
						b_send_addr = pst_net_device_info->dw_device_address_send;
						bp_dsp_cook_finish_flag = 1;
						dw_receive_timeout_ms = 5000;//1500
						w_cmd = cw_subset_update_pacx_dsp_cmd_set_buadrate;
						b_update_step_next = cb_cb_pacx_dsp_update_step_wait_flash_erase;
					}
					break;
				case cb_cb_pacx_dsp_update_step_wait_flash_erase:
					{
						f_send_cmd = TRUE;
						st_subset_update_info.st_subset_update_tx_info.w_data_len = 0;
	
						st_subset_update_info.st_subset_update_rx_info.w_data_len =\
							sprintf((char *)st_subset_update_info.st_subset_update_rx_info.ba_data,"\r\nErase Flash Success");
						display_management_parameter_send(normal_mode,com_led,device_com,TRUE);
	
						dw_receive_timeout_ms = 15000;
						w_cmd = cw_subset_update_pacx_dsp_cmd_set_buadrate;
						b_update_step_next = cb_pacx_dsp_update_step_send_firm_key;
					}
					break;
				case cb_pacx_dsp_update_step_send_firm_key:
					{//read package 1
					    if (ble_ftp_updata_flag == 0)
                        {
                            dw_firmware_read_addr = cdwPackFirmAddrStart;
                        }
                        else
                        {
                            dw_firmware_read_addr = ble_cdwPackFirmAddrStart;
                        }
						LocalUpdateSpiFlashFirmwarePacket *pst_local_firmware_packet = 0;
						pacx_dsp_packet_info *pst_pacx_dsp_packet_info = 0;
						if(0 == updata_data_read_by_addr(dw_firmware_read_addr,
														 ba_flash_data_read))
						{
							b_retry_cnt = 0;
							b_result = cb_subset_update_fail;
							b_update_step = cb_pacx_dsp_update_step_wait_update;
							b_update_step_next = cb_pacx_dsp_update_step_set_sub_buad_rate_115200;
							f_send_cmd = FALSE;
							dw_subset_firmware_sum = 0;
							break;
						}
						pst_local_firmware_packet = (LocalUpdateSpiFlashFirmwarePacket *)ba_flash_data_read;
						pst_pacx_dsp_packet_info = (pacx_dsp_packet_info *)&pst_local_firmware_packet->SpiFlashPacket.Packet.bData[4];
						w_package_len = (*(uint16_t *)(pst_local_firmware_packet->SpiFlashPacket.bPacketDataLen)) - 2;
	
						st_subset_update_info.st_subset_update_tx_info.ba_data[0] = ';';
						memcpy(&st_subset_update_info.st_subset_update_tx_info.ba_data[1],
							   pst_pacx_dsp_packet_info->ba_key,
							   12);
						st_subset_update_info.st_subset_update_tx_info.w_data_len = 13;
						calculate_crc16(st_subset_update_info.st_subset_update_tx_info.ba_data,
										st_subset_update_info.st_subset_update_tx_info.w_data_len,
										&st_subset_update_info.st_subset_update_tx_info.ba_data[st_subset_update_info.st_subset_update_tx_info.w_data_len],
										&st_subset_update_info.st_subset_update_tx_info.ba_data[st_subset_update_info.st_subset_update_tx_info.w_data_len+1]);
	
						st_subset_update_info.st_subset_update_tx_info.w_data_len += 2;
	
						st_subset_update_info.st_subset_update_rx_info.w_data_len =\
							sprintf((char *)st_subset_update_info.st_subset_update_rx_info.ba_data,"OK!");
						b_send_addr = pst_net_device_info->dw_device_address_send;

						dw_receive_timeout_ms = 5000;
						w_cmd = cw_subset_update_pacx_dsp_cmd_send_key;
						b_update_step_next = cb_pacx_dsp_update_step_send_firm_package;
					}
					break;
				case cb_pacx_dsp_update_step_send_firm_package:
					{
						static uint16_t w_package_data_index = 0;
						LocalUpdateSpiFlashFirmwarePacket *pst_local_firmware_packet = 0;
						pacx_packet_data *pst_pacx_packet_data = 0;
						uint16_t w_subset_package_len = 0;
	
						if((w_package_data_index >= (w_package_len - 4 - cw_subset_update_max_receive_len)) ||\
						   (cw_subset_update_pacx_dsp_cmd_send_key == w_cmd))
						{
							w_package_data_index = 0;
							dw_firmware_read_addr += w_package_len + 6 + 2;
							if(0 == updata_data_read_by_addr(dw_firmware_read_addr,
															 ba_flash_data_read))
							{
								b_retry_cnt = 0;
								b_result = cb_subset_update_fail;
								b_update_step = cb_pacx_dsp_update_step_wait_update;
								b_update_step_next = cb_pacx_dsp_update_step_set_sub_buad_rate_115200;
								dw_subset_firmware_sum = 0;
								dw_subset_firmware_num = 0;
								break;
							}
						}
						else
						{
							w_package_data_index += cw_subset_update_max_receive_len;
						}
						dw_subset_firmware_num++;
						pst_local_firmware_packet = (LocalUpdateSpiFlashFirmwarePacket *)ba_flash_data_read;
						pst_pacx_packet_data =\
							(pacx_packet_data *)&pst_local_firmware_packet->SpiFlashPacket.Packet.data.bPacketData[4 + w_package_data_index];
						w_package_len = (*(uint16_t *)(pst_local_firmware_packet->SpiFlashPacket.bPacketDataLen)) - 2;//4:sum	2:crc
						dw_firmware_num = *(uint32_t *)pst_local_firmware_packet->SpiFlashPacket.bPacketIndex;
						dw_firmware_sum = *(uint32_t *)pst_local_firmware_packet->SpiFlashPacket.Packet.data.bPacketData;
						if(w_package_len - 4 > cw_subset_update_max_receive_len)
						{
							if((w_package_len - 4 - w_package_data_index) < cw_subset_update_max_receive_len)
							{
								w_subset_package_len = w_package_len - 4 - w_package_data_index;
							}
							else
							{
								w_subset_package_len = cw_subset_update_max_receive_len;
							}
						}
						else
						{
							w_subset_package_len = w_package_len - 4;
						}
	
	//					  st_subset_update_info.st_subset_update_tx_info.ba_data[0] = (uint8_t)((dw_firmware_num - 1) >> 8);
	//					  st_subset_update_info.st_subset_update_tx_info.ba_data[1] = (uint8_t)(dw_firmware_num - 1);
						st_subset_update_info.st_subset_update_tx_info.ba_data[0] = (uint8_t)((dw_subset_firmware_num) >> 8);
						st_subset_update_info.st_subset_update_tx_info.ba_data[1] = (uint8_t)(dw_subset_firmware_num);
						st_subset_update_info.st_subset_update_tx_info.ba_data[2] = (uint8_t)((dw_subset_firmware_sum - 1) >> 8);
						st_subset_update_info.st_subset_update_tx_info.ba_data[3] = (uint8_t)(dw_subset_firmware_sum - 1);
						memcpy(&st_subset_update_info.st_subset_update_tx_info.ba_data[4],
							   pst_pacx_packet_data->b_packet_data,
							   (w_subset_package_len));
						st_subset_update_info.st_subset_update_tx_info.w_data_len = w_subset_package_len + 4;
						b_send_addr = pst_net_device_info->dw_device_address_send;

						st_subset_update_info.st_subset_update_rx_info.w_data_len = 0;
	
						st_subset_update_info.w_update_per = (dw_firmware_num * 100) / dw_firmware_sum;
						w_cmd = cw_subset_update_pacx_dsp_cmd_send_firmware;
                		#if( update_status_print )
						DebugPrintf(TRUE,"dw_subset_firmware_num:%d ,dw_subset_firmware_sum:%d\n",dw_subset_firmware_num,dw_subset_firmware_sum);
                		#endif
						if(dw_subset_firmware_num == dw_subset_firmware_sum - 1)
						{
							w_package_data_index = 0;
							dw_subset_firmware_num = 0;
							st_subset_update_info.st_subset_update_rx_info.w_data_len =\
								sprintf((char *)st_subset_update_info.st_subset_update_rx_info.ba_data,"\r\nProgram Success");
							bp_dsp_cook_finish_flag = 1;
							dw_receive_timeout_ms = 15000;
							w_cmd = cw_subset_update_pacx_dsp_cmd_send_firm_over;
							b_update_step_next = cb_pacx_dsp_update_step_send_package_over;
						}
						else
						{
							dw_delay_cnt_ms = dw_package_send_interval_ms;
							b_update_step = cb_pacx_dsp_update_step_wait_update;
							b_update_step_next = cb_pacx_dsp_update_step_send_firm_package;
						}
					}
					break;
				case cb_pacx_dsp_update_step_send_package_over:
					{
						st_subset_update_info.st_subset_update_tx_info.ba_data[0] = 'Y';
						st_subset_update_info.st_subset_update_tx_info.w_data_len = 1;
						b_send_addr = pst_net_device_info->dw_device_address_send;
						st_subset_update_info.st_subset_update_rx_info.w_data_len = \
						  sprintf((char *)st_subset_update_info.st_subset_update_rx_info.ba_data,"\r\nFlash Error");		//成功返回"\r\nProgram Running"或者"\r\nDSP Reset an Runnig"
						dw_receive_timeout_ms = 5000;
						w_cmd = cw_subset_update_pacx_dsp_cmd_send_firm_over;
						b_update_step_next = cb_pacx_dsp_update_step_set_sub_buad_rate_9600;
						w_enable_delay_ms = 3000;
					}
					break;
				case cb_pacx_dsp_update_step_set_sub_buad_rate_9600:
					{
						uint8_t b_system_protocal = 0;
						check_system_protocal_type(&b_system_protocal);
						if(cb_system_protocal_type_riiosunii == b_system_protocal)
						{
							st_subset_update_info.st_subset_update_tx_info.ba_data[0] = 0x03;
							st_subset_update_info.st_subset_update_tx_info.ba_data[1] = 0x03;
							st_subset_update_info.st_subset_update_tx_info.w_data_len = 2;
	
							st_subset_update_info.st_subset_update_rx_info.ba_data[0] = 0x03;
							st_subset_update_info.st_subset_update_rx_info.ba_data[1] = 0x03;
							st_subset_update_info.st_subset_update_rx_info.w_data_len = 2;
							b_send_addr = pst_net_device_info->dw_device_address_send;

							w_cmd = cw_subset_update_pacx_dsp_cmd_enable_subset;
							b_update_step_next = cb_pacx_dsp_update_step_set_local_buadrate_9600;
						}
						else
						{
							st_subset_update_info.st_subset_update_tx_info.w_data_len = 0;
							st_subset_update_info.st_subset_update_rx_info.w_data_len = 0;
							f_send_cmd = FALSE;
							b_update_step = cb_pacx_dsp_update_step_update_success;
						}
					}
					break;
				case cb_pacx_dsp_update_step_set_local_buadrate_9600:
					{
						st_subset_update_info.st_subset_update_tx_info.w_data_len = 0;
						st_subset_update_info.st_subset_update_rx_info.w_data_len = 0;
						f_send_cmd = FALSE;
						b_update_step = cb_pacx_dsp_update_step_update_success;
						FIRMWARE_UPDATE_SET_BUADRATE_9600();
					}
					break;
				case cb_pacx_dsp_update_step_update_success:
					{
						b_result = cb_subset_update_success;
						b_update_step = cb_pacx_dsp_update_step_wait_update;
						b_update_step_next = cb_pacx_dsp_update_step_enable_sp;
						st_subset_update_info.st_subset_update_rx_info.w_data_len = 0;
						f_send_cmd = FALSE;
						b_retry_cnt = 0;
						b_cnt_j = 0;
						dw_firmware_sum = 0;
						dw_subset_firmware_sum = 0;
						bp_dsp_cook_finish_flag = 0;
						b_mppt_enable_flag = 0;
					}
					break;
				default:
					b_result = cb_subset_update_fail;
					break;
			}
		}
		{   //send func + receive func
	        if(FALSE == f_send_cmd)
	        {
	            if(0 != st_subset_update_info.st_subset_update_tx_info.w_data_len)
	            {
	                f_send_cmd = subset_update_send_update_cmd(cb_firmware_format_pacx_dsp,
	                                                           b_send_addr,
	                                                           w_cmd,
	                                                           st_subset_update_info.st_subset_update_tx_info.w_data_len,
	                                                           st_subset_update_info.st_subset_update_tx_info.ba_data,
	                                                           pst_net_device_info);
	            }
	        }

	        {
	            if(0 != st_subset_update_info.st_subset_update_rx_info.w_data_len)
	            {
	                b_result =\
	                    subset_update_receive_update_response(cb_firmware_format_pacx_dsp,
	                                                          pst_net_device_info->dw_device_address_receive,
	                                                          w_cmd,
	                                                          st_subset_update_info.st_subset_update_rx_info.w_data_len,
	                                                          st_subset_update_info.st_subset_update_rx_info.ba_data,
	                                                          pdMS_TO_TICKS_USER(dw_receive_timeout_ms),
	                                                          pst_net_device_info);
	                if(cb_subset_update_com_success == b_result)
	                {
	                    if(cb_pacx_dsp_update_step_set_sub_buad_rate_115200 != b_update_step)
	                    {
	                        b_retry_cnt = 0;
	                    }
	                    f_send_cmd = FALSE;
	                    st_subset_update_info.st_subset_update_tx_info.w_data_len = 0;
	                    b_update_step = b_update_step_next;
	                }
	                else if(cb_subset_update_com_fail == b_result)
	                {
	                    b_retry_cnt++;
	                    f_send_cmd = FALSE;
						if(b_cnt_j > 2 || (check_system_type(cb_inv_type_apollo_maxx) && w_cmd != cw_subset_update_pacx_dsp_cmd_enable_subset))
						{
							b_cnt_j = 0;
							b_update_step = cb_pacx_dsp_update_step_wait_update;
							if(check_system_type(cb_inv_type_apollo_matric_ii_10_15k) || check_system_type(cb_inv_type_apollo_matric_ii_5_8k))
							{

								b_update_step_next = cb_pacx_dsp_update_step_enable_sp;
							}
							else
							{
								b_update_step_next = cb_pacx_dsp_update_step_set_sub_buad_rate_115200;
							}
						}
						else
						{
							b_cnt_j ++;
							b_update_step_next = b_update_step;
						}		

	                    #if( update_status_print )
	                    DebugPrintf(TRUE,"[Update]Dsp update com fail,cmd:%04X\r\n",w_cmd);
	                    #endif

	                    if(b_retry_cnt > 2)
	                    {
	                        b_retry_cnt = 0;
	                        b_result = cb_subset_update_fail;
	                        dw_delay_cnt_ms = 3000;
	                        if(bp_dsp_cook_finish_flag)
	                        {
	                          bp_dsp_cook_finish_flag = 0;
//	                          st_subset_update_info.w_update_per = 200;
//							  st_subset_update_info.w_update_state = cb_firmware_update_status_none;
							  gb_update_firware_fail_flag = 1;
	                        }
	                        else
	                        {
	                          st_subset_update_info.w_update_per = 0;
	                        }
	                        dw_firmware_sum = 0;

	                        FIRMWARE_UPDATE_SET_BUADRATE_9600();
	                    }
	                }
	            }
	            else
	            {
	                f_send_cmd = FALSE;
	            }
	        }
	    }
	}
	else if(cdw_device_type_ck_inverter == (*pst_net_device_info->pdw_device_id & 0xFFFF00))
	{
	    static uint8_t b_update_step = cb_pacx_dsp_update_step_wait_update;
    	static uint8_t b_update_step_next = cb_pacx_dsp_update_step_set_sub_buad_rate_115200;
	    if(0 == dw_subset_firmware_sum)
	    {
	        if(FALSE == read_send_subset_packet_num(cw_subset_update_max_receive_len,
	                                                &dw_subset_firmware_sum))
	        {
	            b_retry_cnt = 0;
	            b_result = cb_subset_update_fail;
	            b_update_step = cb_pacx_dsp_update_step_wait_update;
	            b_update_step_next = cb_pacx_dsp_update_step_set_sub_buad_rate_115200;
				#if( update_status_print )
		        DebugPrintf(TRUE,"[Update]DSP update get package sum fail\r\n");
		        #endif
	            return FALSE;
	        }
	        st_subset_update_info.w_update_per = 0;
	        st_subset_update_info.w_update_state = cb_firmware_update_status_updating;
	        #if( update_status_print )
	        DebugPrintf(TRUE,"[Update]DSP update package sum :%d\r\n",dw_subset_firmware_sum);
	        #endif
	    }
	    if(FALSE == f_send_cmd)
		{
			switch(b_update_step)
			{
				case cb_pacx_dsp_update_step_set_sub_buad_rate_115200:
					{
						uint8_t b_system_protocal = 0;
						check_system_protocal_type(&b_system_protocal);
						st_subset_update_info.w_update_per = 0;
						st_subset_update_info.w_update_state = cb_firmware_update_status_updating;
	
						if(cb_system_protocal_type_riiosunii == b_system_protocal)
						{
							st_subset_update_info.st_subset_update_tx_info.ba_data[0] = 0x03;
							st_subset_update_info.st_subset_update_tx_info.ba_data[1] = 0x06;
							st_subset_update_info.st_subset_update_tx_info.w_data_len = 2;
	
							st_subset_update_info.st_subset_update_rx_info.ba_data[0] = 0x03;
							st_subset_update_info.st_subset_update_rx_info.ba_data[1] = 0x06;
							st_subset_update_info.st_subset_update_rx_info.w_data_len = 2;
	
							w_cmd = cw_subset_update_pacx_dsp_cmd_enable_subset;
							b_update_step_next = cb_pacx_dsp_update_step_set_local_buadrate_115200;
	
							dw_package_send_interval_ms = 150;
							w_enable_delay_ms = 1500;
						}
						else
						{
							st_subset_update_info.st_subset_update_tx_info.w_data_len = 0;
							st_subset_update_info.st_subset_update_rx_info.w_data_len = 0;
							f_send_cmd = FALSE;
							b_update_step = cb_pacx_dsp_update_step_wait_update;
							b_update_step_next = cb_pacx_dsp_update_step_enable_subset;
							dw_package_send_interval_ms = 500;
							//w_enable_delay_ms = 1400;
							w_enable_delay_ms = 1313;
						}
						dw_delay_cnt_ms = 3000;
					}
					break;
				case cb_pacx_dsp_update_step_set_local_buadrate_115200:
					{
						st_subset_update_info.st_subset_update_tx_info.w_data_len = 0;
						st_subset_update_info.st_subset_update_rx_info.w_data_len = 0;
						f_send_cmd = FALSE;
						b_update_step = cb_pacx_dsp_update_step_wait_update;
						b_update_step_next = cb_pacx_dsp_update_step_enable_subset;
						FIRMWARE_UPDATE_SET_BUADRATE_115200();
					}
					break;
				case cb_pacx_dsp_update_step_wait_update:
					{
	
						st_subset_update_info.st_subset_update_tx_info.w_data_len = 0;
						st_subset_update_info.st_subset_update_rx_info.w_data_len = 0;
						f_send_cmd = FALSE;
						if(TRUE == subset_update_delay_func(dw_delay_cnt_ms))
						{
							if(b_update_step_next == cb_pacx_dsp_update_step_wait_update)
							{
								b_update_step_next = cb_pacx_dsp_update_step_set_sub_buad_rate_9600;
							}
							b_update_step = b_update_step_next;
						}
					}
					break;
				case cb_pacx_dsp_update_step_enable_subset:
					{
						st_subset_update_info.st_subset_update_tx_info.ba_data[0] = 0x01;
						st_subset_update_info.st_subset_update_tx_info.ba_data[1] = 0x02;
						st_subset_update_info.st_subset_update_tx_info.w_data_len = 2;
	
						st_subset_update_info.st_subset_update_rx_info.w_data_len = 0;
	
						w_cmd = cw_subset_update_pacx_dsp_cmd_enable_subset;
	
						dw_delay_cnt_ms = w_enable_delay_ms;//1500 ms//1.4 + 0.1s
	
						b_update_step = cb_pacx_dsp_update_step_wait_update;
						b_update_step_next = cb_pacx_dsp_update_step_set_buadrate;
					}
					break;
				case cb_pacx_dsp_update_step_set_buadrate:
					{
						uint16_t w_data_len = 0;
						uint8_t ba_data_temp[4] = {0xAA,0x55,0x55,0xAA,};
						rs485_protocal_tx_data_assamble(0x00,
														cw_subset_update_pacx_dsp_cmd_common,
														4,
														ba_data_temp,
														st_subset_update_info.st_subset_update_tx_info.ba_data,
														&w_data_len);
	
						st_subset_update_info.st_subset_update_tx_info.w_data_len = w_data_len;
	
						memset(ba_data_temp,
							   0xCD,
							   4);
						rs485_protocal_tx_data_assamble(0x00,
														cw_subset_update_pacx_dsp_cmd_common,
														4,
														ba_data_temp,
														&st_subset_update_info.st_subset_update_tx_info.ba_data[w_data_len],
														&w_data_len);
						st_subset_update_info.st_subset_update_tx_info.w_data_len += w_data_len;
	
						st_subset_update_info.st_subset_update_rx_info.w_data_len =\
							sprintf((char *)st_subset_update_info.st_subset_update_rx_info.ba_data,"\r\nErasing Flash");
	
						dw_receive_timeout_ms = 5000;//1500
						w_cmd = cw_subset_update_pacx_dsp_cmd_set_buadrate;
						b_update_step_next = cb_cb_pacx_dsp_update_step_wait_flash_erase;
					}
					break;
				case cb_cb_pacx_dsp_update_step_wait_flash_erase:
					{
						f_send_cmd = TRUE;
						st_subset_update_info.st_subset_update_tx_info.w_data_len = 0;
	
						st_subset_update_info.st_subset_update_rx_info.w_data_len =\
							sprintf((char *)st_subset_update_info.st_subset_update_rx_info.ba_data,"\r\nErase Flash Success");
						display_management_parameter_send(normal_mode,com_led,device_com,TRUE);
	
						dw_receive_timeout_ms = 15000;
						w_cmd = cw_subset_update_pacx_dsp_cmd_set_buadrate;
						b_update_step_next = cb_pacx_dsp_update_step_send_firm_key;
					}
					break;
				case cb_pacx_dsp_update_step_send_firm_key:
					{//read package 1
                        if (ble_ftp_updata_flag == 0)
                        {
                            dw_firmware_read_addr = cdwPackFirmAddrStart;
                        }
                        else
                        {
                            dw_firmware_read_addr = ble_cdwPackFirmAddrStart;
                        }
                        LocalUpdateSpiFlashFirmwarePacket *pst_local_firmware_packet = 0;
						pacx_dsp_packet_info *pst_pacx_dsp_packet_info = 0;
						if(0 == updata_data_read_by_addr(dw_firmware_read_addr,
														 ba_flash_data_read))
						{
							b_retry_cnt = 0;
							b_result = cb_subset_update_fail;
							b_update_step = cb_pacx_dsp_update_step_wait_update;
							b_update_step_next = cb_pacx_dsp_update_step_set_sub_buad_rate_115200;
							f_send_cmd = FALSE;
							dw_subset_firmware_sum = 0;
							break;
						}
						pst_local_firmware_packet = (LocalUpdateSpiFlashFirmwarePacket *)ba_flash_data_read;
						pst_pacx_dsp_packet_info = (pacx_dsp_packet_info *)&pst_local_firmware_packet->SpiFlashPacket.Packet.bData[4];
						w_package_len = (*(uint16_t *)(pst_local_firmware_packet->SpiFlashPacket.bPacketDataLen)) - 2;
	
						st_subset_update_info.st_subset_update_tx_info.ba_data[0] = ';';
						memcpy(&st_subset_update_info.st_subset_update_tx_info.ba_data[1],
							   pst_pacx_dsp_packet_info->ba_key,
							   12);
						st_subset_update_info.st_subset_update_tx_info.w_data_len = 13;
						calculate_crc16(st_subset_update_info.st_subset_update_tx_info.ba_data,
										st_subset_update_info.st_subset_update_tx_info.w_data_len,
										&st_subset_update_info.st_subset_update_tx_info.ba_data[st_subset_update_info.st_subset_update_tx_info.w_data_len],
										&st_subset_update_info.st_subset_update_tx_info.ba_data[st_subset_update_info.st_subset_update_tx_info.w_data_len+1]);
	
						st_subset_update_info.st_subset_update_tx_info.w_data_len += 2;
	
						st_subset_update_info.st_subset_update_rx_info.w_data_len =\
							sprintf((char *)st_subset_update_info.st_subset_update_rx_info.ba_data,"OK!");
	
						dw_receive_timeout_ms = 5000;
						w_cmd = cw_subset_update_pacx_dsp_cmd_send_key;
						b_update_step_next = cb_pacx_dsp_update_step_send_firm_package;
					}
					break;
				case cb_pacx_dsp_update_step_send_firm_package:
					{
						static uint16_t w_package_data_index = 0;
						LocalUpdateSpiFlashFirmwarePacket *pst_local_firmware_packet = 0;
						pacx_packet_data *pst_pacx_packet_data = 0;
						uint16_t w_subset_package_len = 0;
	
						if((w_package_data_index >= (w_package_len - 4 - cw_subset_update_max_receive_len)) ||\
						   (cw_subset_update_pacx_dsp_cmd_send_key == w_cmd))
						{
							w_package_data_index = 0;
							dw_firmware_read_addr += w_package_len + 6 + 2;
							if(0 == updata_data_read_by_addr(dw_firmware_read_addr,
															 ba_flash_data_read))
							{
								b_retry_cnt = 0;
								b_result = cb_subset_update_fail;
								b_update_step = cb_pacx_dsp_update_step_wait_update;
								b_update_step_next = cb_pacx_dsp_update_step_set_sub_buad_rate_115200;
								dw_subset_firmware_sum = 0;
								dw_subset_firmware_num = 0;
								break;
							}
						}
						else
						{
							w_package_data_index += cw_subset_update_max_receive_len;
						}
						dw_subset_firmware_num++;
						pst_local_firmware_packet = (LocalUpdateSpiFlashFirmwarePacket *)ba_flash_data_read;
						pst_pacx_packet_data =\
							(pacx_packet_data *)&pst_local_firmware_packet->SpiFlashPacket.Packet.data.bPacketData[4 + w_package_data_index];
						w_package_len = (*(uint16_t *)(pst_local_firmware_packet->SpiFlashPacket.bPacketDataLen)) - 2;//4:sum	2:crc
						dw_firmware_num = *(uint32_t *)pst_local_firmware_packet->SpiFlashPacket.bPacketIndex;
						dw_firmware_sum = *(uint32_t *)pst_local_firmware_packet->SpiFlashPacket.Packet.data.bPacketData;
						if(w_package_len - 4 > cw_subset_update_max_receive_len)
						{
							if((w_package_len - 4 - w_package_data_index) < cw_subset_update_max_receive_len)
							{
								w_subset_package_len = w_package_len - 4 - w_package_data_index;
							}
							else
							{
								w_subset_package_len = cw_subset_update_max_receive_len;
							}
						}
						else
						{
							w_subset_package_len = w_package_len - 4;
						}
	
	//					  st_subset_update_info.st_subset_update_tx_info.ba_data[0] = (uint8_t)((dw_firmware_num - 1) >> 8);
	//					  st_subset_update_info.st_subset_update_tx_info.ba_data[1] = (uint8_t)(dw_firmware_num - 1);
						st_subset_update_info.st_subset_update_tx_info.ba_data[0] = (uint8_t)((dw_subset_firmware_num) >> 8);
						st_subset_update_info.st_subset_update_tx_info.ba_data[1] = (uint8_t)(dw_subset_firmware_num);
						st_subset_update_info.st_subset_update_tx_info.ba_data[2] = (uint8_t)((dw_subset_firmware_sum - 1) >> 8);
						st_subset_update_info.st_subset_update_tx_info.ba_data[3] = (uint8_t)(dw_subset_firmware_sum - 1);
						memcpy(&st_subset_update_info.st_subset_update_tx_info.ba_data[4],
							   pst_pacx_packet_data->b_packet_data,
							   (w_subset_package_len));
						st_subset_update_info.st_subset_update_tx_info.w_data_len = w_subset_package_len + 4;
	
						st_subset_update_info.st_subset_update_rx_info.w_data_len = 0;
	
						st_subset_update_info.w_update_per = (dw_firmware_num * 100) / dw_firmware_sum;
						w_cmd = cw_subset_update_pacx_dsp_cmd_send_firmware;
                #if( update_status_print )
						DebugPrintf(TRUE,"dw_subset_firmware_num:%d ,dw_subset_firmware_sum:%d\n",dw_subset_firmware_num,dw_subset_firmware_sum);
                #endif
						if(dw_subset_firmware_num == dw_subset_firmware_sum - 1)
						{
							w_package_data_index = 0;
							dw_subset_firmware_num = 0;
							st_subset_update_info.st_subset_update_rx_info.w_data_len =\
								sprintf((char *)st_subset_update_info.st_subset_update_rx_info.ba_data,"\r\nProgram Success");
							bp_dsp_cook_finish_flag = 1;
							dw_receive_timeout_ms = 15000;
							w_cmd = cw_subset_update_pacx_dsp_cmd_send_firm_over;
							b_update_step_next = cb_pacx_dsp_update_step_send_package_over;
						}
						else
						{
							dw_delay_cnt_ms = dw_package_send_interval_ms;
							b_update_step = cb_pacx_dsp_update_step_wait_update;
							b_update_step_next = cb_pacx_dsp_update_step_send_firm_package;
						}
					}
					break;
				case cb_pacx_dsp_update_step_send_package_over:
					{
						st_subset_update_info.st_subset_update_tx_info.ba_data[0] = 'Y';
						st_subset_update_info.st_subset_update_tx_info.w_data_len = 1;
	
						st_subset_update_info.st_subset_update_rx_info.w_data_len = \
						  sprintf((char *)st_subset_update_info.st_subset_update_rx_info.ba_data,"\r\nFlash Error");		//成功返回"\r\nProgram Running"或者"\r\nDSP Reset an Runnig"
						dw_receive_timeout_ms = 5000;
						w_cmd = cw_subset_update_pacx_dsp_cmd_send_firm_over;
						b_update_step_next = cb_pacx_dsp_update_step_set_sub_buad_rate_9600;
						w_enable_delay_ms = 3000;
					}
					break;
				case cb_pacx_dsp_update_step_set_sub_buad_rate_9600:
					{
						uint8_t b_system_protocal = 0;
						check_system_protocal_type(&b_system_protocal);
						if(cb_system_protocal_type_riiosunii == b_system_protocal)
						{
							st_subset_update_info.st_subset_update_tx_info.ba_data[0] = 0x03;
							st_subset_update_info.st_subset_update_tx_info.ba_data[1] = 0x03;
							st_subset_update_info.st_subset_update_tx_info.w_data_len = 2;
	
							st_subset_update_info.st_subset_update_rx_info.ba_data[0] = 0x03;
							st_subset_update_info.st_subset_update_rx_info.ba_data[1] = 0x03;
							st_subset_update_info.st_subset_update_rx_info.w_data_len = 2;
	
							w_cmd = cw_subset_update_pacx_dsp_cmd_enable_subset;
							b_update_step_next = cb_pacx_dsp_update_step_set_local_buadrate_9600;
						}
						else
						{
							st_subset_update_info.st_subset_update_tx_info.w_data_len = 0;
							st_subset_update_info.st_subset_update_rx_info.w_data_len = 0;
							f_send_cmd = FALSE;
							b_update_step = cb_pacx_dsp_update_step_update_success;
						}
					}
					break;
				case cb_pacx_dsp_update_step_set_local_buadrate_9600:
					{
						st_subset_update_info.st_subset_update_tx_info.w_data_len = 0;
						st_subset_update_info.st_subset_update_rx_info.w_data_len = 0;
						f_send_cmd = FALSE;
						b_update_step = cb_pacx_dsp_update_step_update_success;
						FIRMWARE_UPDATE_SET_BUADRATE_9600();
					}
					break;
				case cb_pacx_dsp_update_step_update_success:
					{
						b_result = cb_subset_update_success;
						b_update_step = cb_pacx_dsp_update_step_wait_update;
						b_update_step_next = cb_pacx_dsp_update_step_set_sub_buad_rate_115200;
						st_subset_update_info.st_subset_update_rx_info.w_data_len = 0;
						f_send_cmd = FALSE;
						b_retry_cnt = 0;
						b_cnt_j = 0;
						dw_firmware_sum = 0;
						dw_subset_firmware_sum = 0;
						bp_dsp_cook_finish_flag = 0;
					}
					break;
				default:
					b_result = cb_subset_update_fail;
					break;
			}
		}
		{   //send func + receive func
	        if(FALSE == f_send_cmd)
	        {
	            if(0 != st_subset_update_info.st_subset_update_tx_info.w_data_len)
	            {
	                f_send_cmd = subset_update_send_update_cmd(cb_firmware_format_pacx_dsp,
	                                                           pst_net_device_info->dw_device_address_send,
	                                                           w_cmd,
	                                                           st_subset_update_info.st_subset_update_tx_info.w_data_len,
	                                                           st_subset_update_info.st_subset_update_tx_info.ba_data,
	                                                           pst_net_device_info);
	            }
	        }

	        {
	            if(0 != st_subset_update_info.st_subset_update_rx_info.w_data_len)
	            {
	                b_result =\
	                    subset_update_receive_update_response(cb_firmware_format_pacx_dsp,
	                                                          pst_net_device_info->dw_device_address_receive,
	                                                          w_cmd,
	                                                          st_subset_update_info.st_subset_update_rx_info.w_data_len,
	                                                          st_subset_update_info.st_subset_update_rx_info.ba_data,
	                                                          pdMS_TO_TICKS_USER(dw_receive_timeout_ms),
	                                                          pst_net_device_info);
	                if(cb_subset_update_com_success == b_result)
	                {
	                    if(cb_pacx_dsp_update_step_set_sub_buad_rate_115200 != b_update_step)
	                    {
	                        b_retry_cnt = 0;
	                    }
	                    f_send_cmd = FALSE;
	                    st_subset_update_info.st_subset_update_tx_info.w_data_len = 0;
	                    b_update_step = b_update_step_next;
	                }
	                else if(cb_subset_update_com_fail == b_result)
	                {
	                    b_retry_cnt++;
	                    f_send_cmd = FALSE;
						if(b_cnt_j > 2 || (check_system_type(cb_inv_type_apollo_maxx) && w_cmd != cw_subset_update_pacx_dsp_cmd_enable_subset))
						{
							b_cnt_j = 0;
							b_update_step = cb_pacx_dsp_update_step_wait_update;
							b_update_step_next = cb_pacx_dsp_update_step_set_sub_buad_rate_115200;
						}
						else
						{
							b_cnt_j ++;
							b_update_step_next = b_update_step;
						}		

	                    #if( update_status_print )
	                    DebugPrintf(TRUE,"[Update]Dsp update com fail,cmd:%04X\r\n",w_cmd);
	                    #endif

	                    if(b_retry_cnt > 2)
	                    {
	                        b_retry_cnt = 0;
	                        b_result = cb_subset_update_fail;
	                        dw_delay_cnt_ms = 3000;
	                        if(bp_dsp_cook_finish_flag)
	                        {
	                          bp_dsp_cook_finish_flag = 0;
							  b_retry_cnt = 0;
							  b_cnt_j = 0;
							  dw_subset_firmware_sum = 0;
							  bp_dsp_cook_finish_flag = 0;
							  dw_subset_firmware_sum = 0;
							  gb_update_firware_fail_flag  = 1;
//	                          st_subset_update_info.w_update_per = 200;
//							  st_subset_update_info.w_update_state = cb_firmware_update_status_none;
	                        }
	                        else
	                        {
	                          st_subset_update_info.w_update_per = 0;
	                        }
	                        dw_firmware_sum = 0;
	                        FIRMWARE_UPDATE_SET_BUADRATE_9600();
	                    }
	                }
	            }
	            else
	            {
	                f_send_cmd = FALSE;
	            }
	        }
	    }
	}
    
    {//write update status
        uint16_t w_data_len = 2;

		if (st_subset_update_info.w_update_per != 0)
		{
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_system],
			                       cb_device_data_access_data_type_status,
			                       cw_sys_status_data_addr_update_process,
			                       &w_data_len,
			                       ( uint8_t * )&st_subset_update_info.w_update_per,
			                       cb_device_data_access_operate_type_write);
		}
		#if( update_status_print )
			DebugPrintf(TRUE,"[Update]st_subset_update_info.w_update_per:%d\r\n",st_subset_update_info.w_update_per);
		#endif
	
		net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_system],
                               cb_device_data_access_data_type_status,
                               cw_sys_status_data_addr_update_status,
                               &w_data_len,
                               (uint8_t *)&st_subset_update_info.w_update_state,
                               cb_device_data_access_operate_type_write);

    }

    return b_result;
}

void vFirmwareUpdateTask(void* parameter)
{
	DebugPrintf(TRUE,"%d,vFirmwareUpdateTask_first_entry\n",__LINE__);
    boot_config_init();
    for(;;)
    {
		#if(task_call_rate_test)
		DebugPrintf(TRUE,"vFirmwareUpdateTask,start: %d,priority: %d\n",xTaskGetTickCount(),firmware_update_task_priority);
		#endif
		#if( task_stack_high_water_mark )
			uint16_t w_firmware_update_task_stack_size = 0;
			w_firmware_update_task_stack_size = uxTaskGetStackHighWaterMark(firmware_update_task_handle);
			DebugPrintf(TRUE,"w_firmware_update_task_stack_size: %d\n",w_firmware_update_task_stack_size);
		#endif
        rs485_transmission_rx_cook_data();
        local_firmware_update_loop();

		#if(task_call_rate_test)
		DebugPrintf(TRUE,"vFirmwareUpdateTask,end: %d,priority: %d\n",xTaskGetTickCount(),firmware_update_task_priority);
		#endif
		
		vTaskDelay(pdMS_TO_TICKS_USER(dw_firmware_update_task_delay));

    }
}

void firmware_update_task_init()
{
    BaseType_t xReturn = pdPASS;

    xReturn = xTaskCreate( vFirmwareUpdateTask,
                          "vFirmwareUpdateTask",
                          firmware_update_task_stack_size,
                          NULL,
                          firmware_update_task_priority,
                          &firmware_update_task_handle);
    if(pdFAIL == xReturn)
    {
        DebugPrintf(TRUE,"%d,vFirmwareUpdateTask Create Fail\n",__LINE__);
    }
}
