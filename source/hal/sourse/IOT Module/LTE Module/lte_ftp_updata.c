#include "lte_ftp_updata.h"
#include "network_protocal.h"
#include "network_protocal_cmd_macro.h"
#include "network_communication_queue.h"
#include "network_ble_communication_queue.h"
#include "device_info.h"
#include "printf_custom.h"
#include "network_local_secquence_generate.h"
#include "network_protocal_data_assembly.h"
#include "system_timestamp_generate.h"
#include "network_server_response_queue.h"
#include "application_general_func.h"
#include "crc16_arithmetic.h"
#include "lte_module.h"
#include "net_device_tree_build.h"
#include "net_attribute_tree_build.h"
#include "net_real_time_data_build.h"
#include "net_system_info_list.h"
#include "net_firmware_update.h"
#include "net_segmented_inquire.h"
#include "net_forward_set_ctrl.h"
#include "net_system_info_access.h"
#include "network_local_config.h"
#include "network_device_config.h"
#include <string.h>
#include "freertos_user_config.h"
#include "network_iot_module_data_channel_manage.h"
#include "network_tbb_device_server_communicate.h"
#include "display_management.h"
#include "network_offline_data_operate.h"
#include "network_device_config_sycn.h"
#include "network_system_type_operate.h"
#include "network_ble_communication.h"
#include "net_device_data_access.h"
#include "network_eventlog_info_operate.h"
#include "product_test.h"
#include "network_protocal_data_assembly.h"
#include "network_local_firmware_update.h"
#include "network_protocal_data_assembly.h"
#include "BootLoader.h"
#include "Bootloaderapp.H"
#include <stdlib.h>
#include <stdio.h>
#include "spi_flash_operate.h"
#include "drv_spiflash.h"
#include "Eeprom.h"
#include "spi_flash_address_allocate.h"

#define LTE_UPDATA_DEBUG 1

#define QUEUE_SIZE 10

#define UPDATE_NET_INFO st_device_update_info_current.pst_net_device_update_info_current->st_net_firmware_info
#define UPDATE_CURRENT_CURRENT \
    st_device_update_info_current.pst_net_device_update_info_current->st_net_firmware_info.dw_packets_receive_current
#define UPDATE_CURRENT_TOTAL \
    st_device_update_info_current.pst_net_device_update_info_current->st_net_firmware_info.dw_packets_receive_total
#define UPDATE_CURRENT_FORMAT \
    st_device_update_info_current.pst_net_device_update_info_current->st_net_firmware_info.b_packet_format
#define UPDATE_CURRENT_MD5 \
    st_device_update_info_current.pst_net_device_update_info_current->st_net_firmware_info.ba_firmware_id
#define UPDATE_DATA pst_firmware_packet->st_firmware_packet_pacx.un_pacx_packet.st_pacx_packet_data.b_packet_data
#define UPDATE_INFO pst_firmware_packet->st_firmware_packet_pacx.un_pacx_packet.st_pacx_packet_info

#define fWriteEEPage(i) (bEeWriteCtrlFlag[i])

uint32_t dw_ble_firmware_info_spi_flash_addr = cdw_spi_flash_addr_alloc_firmware_info_start;
uint32_t dw_ble_firmware_data_spi_flash_addr = cdw_ble_spi_flash_addr_alloc_local_firmware_data_start;
uint8_t ble_ftp_updata_flag = 0;
uint8_t ftp_ready_flag = 0;

static uint32_t dw_spi_flash_operate_uuid_local = 0;

extern TickType_t lte_system_tick;

#define cw_extern_flash_read_temp_max_length 1000
static uint8_t ba_extern_flash_read_temp[cw_extern_flash_read_temp_max_length] = {
    0,
};

typedef struct
{
    uint8_t *data[QUEUE_SIZE];
    uint8_t head;
    uint8_t tail;
} circular_queue_t;

void init_queue(circular_queue_t *q)
{
    q->head = 0;
    q->tail = 0;
}

uint8_t enqueue(circular_queue_t *q, void *value)
{
    if ((q->tail + 1) % QUEUE_SIZE == q->head)
    {
        // 队列已满
        return -1;
    }
    q->data[q->tail] = value;
    q->tail = (q->tail + 1) % QUEUE_SIZE;
    return 0;
}

uint8_t dequeue(circular_queue_t *q, void **value)
{
    if (q->head == q->tail)
    {
        // 队列为空
        return -1;
    }
    *value = q->data[q->head];
    q->head = (q->head + 1) % QUEUE_SIZE;
    return 0;
}

// 释放队列中的数据并清空队列 初始化队列
void clear_queue(circular_queue_t *q)
{
    void *value;
    while (dequeue(q, &value) == 0)
    {
        vPortFree(value);
    }
    init_queue(q);
}

circular_queue_t ftp_flash_initiate;
circular_queue_t ftp_flash_response;

typedef struct
{
    uint8_t *data;
    uint16_t len;
} ftp_updata_data_t;

uint8_t ftp_spi_flash_operate_initiate(uint32_t dw_addr,
                                       uint32_t dw_data_len,
                                       uint8_t *pb_data,
                                       spi_flash_operate_type enum_operate_type)
{
    spi_flash_operate_initiate_msg *pst_spi_flash_operate_initiate_msg = 0;
    spi_flash_operate_response_msg *pst_spi_flash_operate_response_msg = 0;

    /*SPI FLASH OPERATE REQUEST*/
    {
        {
            pst_spi_flash_operate_initiate_msg =
                ( spi_flash_operate_initiate_msg * )pvPortMalloc(sizeof(spi_flash_operate_initiate_msg));
            if (0 == pst_spi_flash_operate_initiate_msg)
            {
                return FALSE;
            }

            if (spi_flash_erase_request != enum_operate_type)
            {
                if (0 == pb_data)
                {
                    return FALSE;
                }
            }
        }

        {
            pst_spi_flash_operate_initiate_msg->enum_operate_type = enum_operate_type;
            pst_spi_flash_operate_initiate_msg->dw_operate_addr = dw_addr;
            pst_spi_flash_operate_initiate_msg->st_data_info.w_data_len = ( uint16_t )dw_data_len;
            pst_spi_flash_operate_initiate_msg->st_data_info.pb_data = pb_data;
            pst_spi_flash_operate_initiate_msg->dw_spi_flash_operate_initiate_uuid = dw_spi_flash_operate_uuid_local++;
        }

        {
            if (0 != enqueue(&ftp_flash_initiate, ( void * )pst_spi_flash_operate_initiate_msg))
            {
                yl_debug_printf("ftp_flash_initiate is full\r\n");
                vPortFree(pst_spi_flash_operate_initiate_msg);
                dw_spi_flash_operate_uuid_local--;
                return FALSE;
            }
        }
    }

    /*SPI FLASH OPERATE REPONSE*/
    {
        uint8_t spi_flash_operate_status = FALSE;

        {
            TickType_t *px_pi_flash_operate_response_timer = ( TickType_t * )pvPortMalloc(sizeof(TickType_t));
            if (0 == px_pi_flash_operate_response_timer)
            {
                return FALSE;
            }

            TickType_t x_spi_flash_operate_response_start_time = xTaskGetTickCount();
            do
            {
                *px_pi_flash_operate_response_timer = xTaskGetTickCount();

                if (0 == dequeue(&ftp_flash_response, ( void * )&pst_spi_flash_operate_response_msg))
                {
                    if (pst_spi_flash_operate_initiate_msg->dw_spi_flash_operate_initiate_uuid
                        == pst_spi_flash_operate_response_msg->dw_spi_flash_operate_response_uuid)
                    {
                        spi_flash_operate_status = pst_spi_flash_operate_response_msg->b_spi_flash_operate_status;
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }

            } while ((*px_pi_flash_operate_response_timer - x_spi_flash_operate_response_start_time)
                     < pdMS_TO_TICKS_USER(10000));

            if (px_pi_flash_operate_response_timer != 0)
            {
                vPortFree(px_pi_flash_operate_response_timer);
            }

            if (pst_spi_flash_operate_initiate_msg != 0)
            {
                vPortFree(pst_spi_flash_operate_initiate_msg);
            }

            if (pst_spi_flash_operate_response_msg != 0)
            {
                vPortFree(pst_spi_flash_operate_response_msg);
            }
        }
        return spi_flash_operate_status;
    }
}

int32_t ftp_spi_flash_read(uint32_t dw_addr, uint32_t dw_data_len, uint8_t *pb_data)
{
    uint8_t b_operate_times = 0;

    do
    {
        if (TRUE == ftp_spi_flash_operate_initiate(dw_addr, dw_data_len, pb_data, spi_flash_read_request))
        {
            return 0;
        }
        else
        {
            b_operate_times++;
        }
    } while (b_operate_times < 10);

    return 1;
}

int32_t ftp_spi_flash_write(uint32_t dw_addr, uint32_t dw_data_len, uint8_t *pb_data)
{
    uint8_t b_operate_times = 0;

    do
    {
        if (TRUE == ftp_spi_flash_operate_initiate(dw_addr, dw_data_len, pb_data, spi_flash_write_request))
        {
            return 0;
        }
        else
        {
            b_operate_times++;
        }
    } while (b_operate_times < 10);

    return 1;
}

uint8_t ftp_extern_flash_read(uint32_t dw_addr, uint32_t dw_data_len, uint8_t *pb_data)
{
    if (1 == ftp_spi_flash_read(dw_addr, dw_data_len, pb_data))
    {
        return FALSE;
    }

    return TRUE;
}

int32_t ftp_spi_flash_erase(uint32_t dw_addr, uint32_t dw_data_len)
{
    uint8_t b_operate_times = 0;

    do
    {
         if (TRUE == ftp_spi_flash_operate_initiate(dw_addr, dw_data_len, 0, spi_flash_erase_request))
         {
             b_operate_times++;
         }
    } while (b_operate_times < 10);

    return 0;
}

uint8_t ftp_extern_flash_wirte(uint32_t dw_addr, uint16_t w_data_len, uint8_t *pb_data)
{
    {
        uint16_t w_data_len_write = 0;
        uint16_t w_data_len_temp = w_data_len;
        uint16_t w_data_index = 0;
        do
        {
            {
                uint16_t w_data_len_write_max = 0;

                w_data_len_write_max = 256 - (dw_addr % 256);

                if (w_data_len_temp >= 256)
                {
                    w_data_len_write = w_data_len_write_max;
                }
                else
                {
                    if (w_data_len_temp < w_data_len_write_max)
                    {
                        w_data_len_write = w_data_len_temp;
                    }
                    else
                    {
                        w_data_len_write = w_data_len_write_max;
                    }
                }
            }

            {
                if (0 == (dw_addr % 0x1000))
                {
                    if (1 == ftp_spi_flash_erase(dw_addr, 0))
                    {
                        return FALSE;
                    }
                }
//				else if (0 == (dw_addr % 0x00497000))
//                {
//                    if (1 == ftp_spi_flash_erase(dw_addr, 0))
//                    {
//                        return FALSE;
//                    }
//                }
                if (1 == ftp_spi_flash_write(dw_addr, w_data_len_write, &pb_data[w_data_index]))
                {
                    return FALSE;
                }
            }

            {
                uint8_t b_store_ok = 1;
                uint8_t b_cmp_times = 0;

                for (;;)
                {
                    if (w_data_len_write > cw_extern_flash_read_temp_max_length)
                    {
                        return FALSE;
                    }
                    if (1 == ftp_spi_flash_read(dw_addr, w_data_len_write, ba_extern_flash_read_temp))
                    {
                        return FALSE;
                    }

                    b_store_ok = memcmp(&pb_data[w_data_index], ba_extern_flash_read_temp, w_data_len_write);

                    if (0 == b_store_ok)
                    {
                        dw_addr += w_data_len_write;
                        w_data_len_temp -= w_data_len_write;
                        w_data_index += w_data_len_write;
                        break;
                    }
                    else
                    {
                        b_cmp_times++;
                        if (b_cmp_times > 10)
                        {
                            return FALSE;
                        }
                    }
                }
            }
        } while (w_data_len_temp != 0);
    }

    return TRUE;
}

uint8_t ftp_updata_task_delete = 1;
uint16_t ftp_updata_package_num_all = 0;
uint16_t ftp_updata_package_num_now = 0;
uint16_t ftp_updata_package_num_last = 0;
TaskHandle_t ftp_updata_task_handle = NULL;
TaskHandle_t ftp_printf_task_handle = NULL;
TaskHandle_t ftp_flash_task_handle = NULL;
TaskHandle_t flash_erase_sector_all_task_handle = NULL;
circular_queue_t ftp_updata_queue;
static void ftp_printf_task(void *parameter);
static void ftp_flash_task(void *parameter);

uint32_t ftp_cnt = 0;
uint32_t ftp_len = 0;

void ftp_data_receive_data(uint8_t *ftp_receive_buff, uint16_t ftp_receive_buff_len)
{
    uint16_t ftp_data_len = 0;
    if (TRUE == uart_lte_receive_data(ftp_receive_buff, &ftp_data_len, lte_module_mps_size))
    {
        uint16_t index = 0;
        uint16_t index_temp = 0;
        ftp_len < ftp_data_len ? ftp_len = ftp_data_len : ftp_len;
        for (index = 0; index < ftp_data_len; index++)
        {
            if ((('\r' == ftp_receive_buff[index]) && ('\n' == ftp_receive_buff[index + 1])
                 && (index + 2 == ftp_data_len))
                || (('\r' == ftp_receive_buff[index]) && ('\n' == ftp_receive_buff[index + 1])
                    && ('+' == ftp_receive_buff[index + 2])))
            {
                if (strstr(( char * )&ftp_receive_buff[index_temp], "+WRITE:"))
                {
                    uint8_t *data = ( uint8_t * )pvPortMalloc((index - index_temp + 2) * sizeof(uint8_t));
                    memcpy(data, &ftp_receive_buff[index_temp], index - index_temp + 2);
                    ftp_updata_data_t *ftp_updata_data =
                        ( ftp_updata_data_t * )pvPortMalloc(sizeof(ftp_updata_data_t));
                    ftp_updata_data->data = data;
                    ftp_updata_data->len = index - index_temp + 2;
                    if (0 != enqueue(&ftp_updata_queue, ( void * )ftp_updata_data))
                    {
                        yl_debug_printf("ftp_updata_queue is full\r\n");
                        vPortFree(ftp_updata_data->data);
                        vPortFree(ftp_updata_data);
                    }
                    ftp_cnt++;
                }
                index_temp = index + 2;
            }
        }
    }
}

uint8_t ftp_firmware_update_traget_pairing_pacx(firmware_packet *pst_firmware_packet,
                                                net_system_info_list *pst_net_system_info_list)
{
    if ((NULL == pst_firmware_packet) || (NULL == pst_net_system_info_list))
    {
        return FALSE;
    }

    if (st_device_update_info_current.pst_net_device_update_info_current != NULL)
    {
        return TRUE;
    }

    for (uint16_t w_cnt_i = 0; w_cnt_i < pst_net_system_info_list->w_length; w_cnt_i++)
    {
        if (pst_net_system_info_list->pst_system_info[w_cnt_i].b_type == pst_net_system_info_list->b_current_type)
        {

            for (uint16_t w_cnt_j = 0;
                 w_cnt_j
                 < pst_net_system_info_list->pst_system_info[w_cnt_i].pst_net_device_update_info_list->w_length;
                 w_cnt_j++)
            {
                if (pst_firmware_packet->st_firmware_packet_pacx.un_pacx_packet.st_pacx_packet_info.b_mcu_type
                    == (( net_device_update_info * )((pst_net_system_info_list->pst_system_info[w_cnt_i]
                                                          .pst_net_device_update_info_list->pst_net_device_update_info)
                                                     + w_cnt_j))
                           ->b_mcu_type)
                {
                    for (uint16_t w_cnt_k = 0;
                         w_cnt_k < pst_net_system_info_list->pst_system_info[w_cnt_i].pst_device_info_list->w_length;
                         w_cnt_k++)
                    {
                        if ((( net_device_update_info * )((pst_net_system_info_list->pst_system_info[w_cnt_i]
                                                               .pst_net_device_update_info_list
                                                               ->pst_net_device_update_info)
                                                          + w_cnt_j))
                                ->w_device_type
                            == ((*pst_net_system_info_list->pst_system_info[w_cnt_i]
                                      .pst_device_info_list->pst_device_info[w_cnt_k]
                                      .pdw_device_id
                                 & 0xFFFF00)
                                >> 8))
                        {
                            uint32_t dw_version_int_net = 0;
                            uint32_t dw_version_int_local = 1;
                            uint8_t b_version_str_local[6];

                            memcpy(&b_version_str_local[0],
                                   &pst_net_system_info_list->pst_system_info[w_cnt_i]
                                        .pst_device_info_list->pst_device_info[w_cnt_k]
                                        .pst_net_device_tree_info->st_device_tree.b_firmware_version[7],
                                   4);

                            memcpy(&b_version_str_local[4],
                                   &pst_net_system_info_list->pst_system_info[w_cnt_i]
                                        .pst_device_info_list->pst_device_info[w_cnt_k]
                                        .pst_net_device_tree_info->st_device_tree.b_firmware_version[12],
                                   2);

                            dw_version_int_local = atoi(( char * )&b_version_str_local[0]);

                            bytes_to_int_big_to_little_endain(pst_firmware_packet->st_firmware_packet_pacx
                                                                  .un_pacx_packet.st_pacx_packet_info.ba_firmware_type,
                                                              &dw_version_int_net,
                                                              3);

                            if (dw_version_int_net == dw_version_int_local)
                            {
                                st_device_update_info_current.pst_net_device_update_info_current =
                                    ( net_device_update_info * )((pst_net_system_info_list->pst_system_info[w_cnt_i]
                                                                      .pst_net_device_update_info_list
                                                                      ->pst_net_device_update_info)
                                                                 + w_cnt_j);
                                return TRUE;
                            }
                        }
                    }
                }
            }
        }
        else if ((w_cnt_i + 1) >= pst_net_system_info_list->w_length)
        {
            return FALSE;
        }
    }

    return FALSE;
}

uint8_t ftp_firmware_update_traget_pairing_pacx_dsp(firmware_packet *pst_firmware_packet,
                                                    net_system_info_list *pst_net_system_info_list)
{
    if ((NULL == pst_firmware_packet) || (NULL == pst_net_system_info_list))
    {
        return FALSE;
    }

    if (st_device_update_info_current.pst_net_device_update_info_current != NULL)
    {
        return TRUE;
    }

    for (uint16_t w_cnt_i = 0; w_cnt_i < pst_net_system_info_list->w_length; w_cnt_i++)
    {
        if (pst_net_system_info_list->pst_system_info[w_cnt_i].b_type == pst_net_system_info_list->b_current_type)
        {
            for (uint16_t w_cnt_j = 0;
                 w_cnt_j
                 < pst_net_system_info_list->pst_system_info[w_cnt_i].pst_net_device_update_info_list->w_length;
                 w_cnt_j++)
            {
                if ((pst_firmware_packet->st_firmware_packet_pacx_dsp.un_pacx_dsp_packet.st_pacx_dsp_packet_info.b_identification == 
					(( net_device_update_info * )((pst_net_system_info_list->pst_system_info[w_cnt_i].
                    pst_net_device_update_info_list->pst_net_device_update_info)+ w_cnt_j))->b_firmware_format) && 
                   (st_device_update_info_current.w_device_type ==
                   ((net_device_update_info *)((pst_net_system_info_list->
                                                pst_system_info[w_cnt_i].
                                                pst_net_device_update_info_list->pst_net_device_update_info) + w_cnt_j))->w_device_type))
                {
                    for (uint16_t w_cnt_k = 0;
                         w_cnt_k < pst_net_system_info_list->pst_system_info[w_cnt_i].pst_device_info_list->w_length;
                         w_cnt_k++)
                    {
                        if ((( net_device_update_info * )((pst_net_system_info_list->pst_system_info[w_cnt_i]
                                                               .pst_net_device_update_info_list
                                                               ->pst_net_device_update_info)
                                                          + w_cnt_j))
                                ->w_device_type
                            == ((*pst_net_system_info_list->pst_system_info[w_cnt_i]
                                      .pst_device_info_list->pst_device_info[w_cnt_k]
                                      .pdw_device_id
                                 & 0xFFFF00)
                                >> 8))
                        {
                            st_device_update_info_current.pst_net_device_update_info_current =
                                ( net_device_update_info * )((pst_net_system_info_list->pst_system_info[w_cnt_i]
                                                                  .pst_net_device_update_info_list
                                                                  ->pst_net_device_update_info)
                                                             + w_cnt_j);
                            return TRUE;
                        }
                    }
                }
            }
        }
        else if ((w_cnt_i + 1) >= pst_net_system_info_list->w_length)
        {
            return FALSE;
        }
    }
    return FALSE;
}
extern uint8_t b_firmware_update_pac_packet_store_finish_flag;

uint8_t ftp_firmware_update_traget_pairing_pac(firmware_packet *pst_firmware_packet,
                                               net_system_info_list *pst_net_system_info_list)
{
    if ((NULL == pst_firmware_packet) || (NULL == pst_net_system_info_list))
    {
        return FALSE;
    }

    if (st_device_update_info_current.pst_net_device_update_info_current != NULL)
    {
        return FALSE;
    }

    for (uint16_t w_cnt_i = 0; w_cnt_i < pst_net_system_info_list->w_length; w_cnt_i++)
    {
        if (pst_net_system_info_list->pst_system_info[w_cnt_i].b_type == pst_net_system_info_list->b_current_type)
        {

            for (uint16_t w_cnt_j = 0;
                 w_cnt_j
                 < pst_net_system_info_list->pst_system_info[w_cnt_i].pst_net_device_update_info_list->w_length;
                 w_cnt_j++)
            {
                if (1)
                {
                    for (uint16_t w_cnt_k = 0;
                         w_cnt_k < pst_net_system_info_list->pst_system_info[w_cnt_i].pst_device_info_list->w_length;
                         w_cnt_k++)
                    {
                        if ((( net_device_update_info * )((pst_net_system_info_list->pst_system_info[w_cnt_i]
                                                               .pst_net_device_update_info_list
                                                               ->pst_net_device_update_info)
                                                          + w_cnt_j))
                                ->w_device_type
                            == ((*pst_net_system_info_list->pst_system_info[w_cnt_i]
                                      .pst_device_info_list->pst_device_info[w_cnt_k]
                                      .pdw_device_id
                                 & 0xFFFF00)
                                >> 8))
                        {
                            if (st_device_update_info_current.w_device_type
                                == ((*pst_net_system_info_list->pst_system_info[w_cnt_i]
                                          .pst_device_info_list->pst_device_info[w_cnt_k]
                                          .pdw_device_id
                                     & 0xFFFF00)
                                    >> 8))
                            {
                                st_device_update_info_current.pst_net_device_update_info_current =
                                    ( net_device_update_info * )((pst_net_system_info_list->pst_system_info[w_cnt_i]
                                                                      .pst_net_device_update_info_list
                                                                      ->pst_net_device_update_info)
                                                                 + w_cnt_j);
                                return TRUE;
                            }
                        }
                    }
                }
            }
        }
        else if ((w_cnt_i + 1) >= pst_net_system_info_list->w_length)
        {
            return FALSE;
        }
    }
    return FALSE;
}

uint8_t ftp_firmware_update_pac_packet_info_store(uint16_t w_firmware_packet_len,
                                                  firmware_packet *pst_firmware_packet,
                                                  device_update_info_current *pst_device_update_info_current)
{
    uint16_t w_data_len = 0;
    uint32_t dw_package_num_temp = 0;
    uint32_t dw_package_sum_temp = 0;

    LocalUpdateSpiFlashFirmwarePacket *pstLocalUpdateSpiFlashFirmwarePacket = 0;

    dw_ble_firmware_data_spi_flash_addr = ble_cdwDevFirmAddrStart;
    if (FALSE == ftp_extern_flash_wirte(dw_ble_firmware_data_spi_flash_addr, 1, ( uint8_t * )&flash_flag_start))
    {
        debug_updata_error("[flash]write flag fail \r\n");
        return FALSE;
    }
    dw_ble_firmware_data_spi_flash_addr += cbFlashFirmInfoLen;

    //    do{
    //        pstLocalUpdateSpiFlashFirmwarePacket = (LocalUpdateSpiFlashFirmwarePacket
    //        *)pvPortMalloc(sizeof(LocalUpdateSpiFlashFirmwarePacket));
    //    }while(NULL == pstLocalUpdateSpiFlashFirmwarePacket);
    pstLocalUpdateSpiFlashFirmwarePacket =
        ( LocalUpdateSpiFlashFirmwarePacket * )pvPortMalloc(sizeof(LocalUpdateSpiFlashFirmwarePacket));
    if (NULL == pstLocalUpdateSpiFlashFirmwarePacket)
    {
        return FALSE;
    }

    // debug_updata_test("[updata] flash addr 0x%x\r\n", dw_ble_firmware_data_spi_flash_addr);
    if (NULL == memset(pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketIndex, 0x00, 4))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        return FALSE;
    }

    if (FALSE
        == bytes_to_int_big_to_little_endain(
            pst_firmware_packet->st_firmware_packet_pac.st_pac_packet.ba_package_len, ( uint32_t * )&w_data_len, 2))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        return FALSE;
    }

    if (FALSE
        == bytes_to_int_big_to_little_endain(pst_firmware_packet->st_firmware_packet_pac.st_pac_packet.ba_package_num,
                                             ( uint32_t * )&dw_package_num_temp,
                                             4))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        return FALSE;
    }

    if (FALSE
        == bytes_to_int_big_to_little_endain(pst_firmware_packet->st_firmware_packet_pac.st_pac_packet.ba_package_sum,
                                             ( uint32_t * )&dw_package_sum_temp,
                                             4))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        return FALSE;
    }

    if (dw_package_sum_temp < dw_package_num_temp)
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        return FALSE;
    }

    // small to big edain
    if (FALSE
        == byte_fill_in_reverse_order(4,
                                      pst_firmware_packet->st_firmware_packet_pac.st_pac_packet.ba_package_num,
                                      pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketIndex,
                                      4))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        return FALSE;
    }

    if (w_data_len + 10 == w_firmware_packet_len)    // 4num 4sum 2len
    {
        w_data_len = w_firmware_packet_len;
    }
    else
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        return FALSE;
    }

    memcpy(( char * )&pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.bData[4],
           ( char * )&pst_firmware_packet->st_firmware_packet_pac.st_pac_packet.ba_package_num[0],
           w_data_len);

    *(( uint32_t * )&pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.bData[0]) = dw_package_sum_temp;

    w_data_len += 4;
    if (FALSE
        == calculate_crc16(pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.bData,
                           w_data_len,
                           &pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.bData[w_data_len],
                           &pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.bData[w_data_len + 1]))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        return FALSE;
    }
    w_data_len += 2;

    pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketDataLen[0] = ( uint8_t )(w_data_len & 0x00FF);
    pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketDataLen[1] = ( uint8_t )((w_data_len & 0xFF00) >> 8);

    if (FALSE
        == ftp_extern_flash_wirte(
            dw_ble_firmware_data_spi_flash_addr, (w_data_len + 6), pstLocalUpdateSpiFlashFirmwarePacket->bData))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        debug_updata_error("\r\n[updata]flash fail LINE is : %d\r\n",__LINE__);
		return FALSE;
    }

    dw_ble_firmware_data_spi_flash_addr += w_data_len + 6;

    {
        pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info
            .dw_packets_receive_current++;

        pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info.b_packet_format =
            pst_device_update_info_current->b_packet_format;

        pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info
            .dw_packets_receive_total = pst_device_update_info_current->dw_packets_number;

        memcpy(pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info.ba_firmware_id,
               pst_device_update_info_current->ba_firmware_id,
               cb_firmware_id_max_len);
    }

    vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
    return TRUE;
}

uint8_t ftp_firmware_update_pac_packet_data_store(uint16_t w_firmware_packet_len,
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
    //        pstLocalUpdateSpiFlashFirmwarePacket = (LocalUpdateSpiFlashFirmwarePacket
    //        *)pvPortMalloc(sizeof(LocalUpdateSpiFlashFirmwarePacket));
    //    }while(NULL == pstLocalUpdateSpiFlashFirmwarePacket);
    pstLocalUpdateSpiFlashFirmwarePacket =
        ( LocalUpdateSpiFlashFirmwarePacket * )pvPortMalloc(sizeof(LocalUpdateSpiFlashFirmwarePacket));
    if (NULL == pstLocalUpdateSpiFlashFirmwarePacket)
    {
        return FALSE;
    }

    //    if(FALSE ==
    //    bytes_to_int_big_to_little_endain(pst_firmware_packet->st_firmware_packet_pac.st_pac_packet.ba_package_len,
    //                                                  (uint32_t *)&w_data_len,
    //                                                  2))
    //    {
    //		vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
    //        #if(dynamic_memory_request_test)
    //        DebugPrintf(TRUE,"firmware_update_pac_packet_info_store_1_free%x\n",pstLocalUpdateSpiFlashFirmwarePacket);
    //        #endif
    //        return FALSE;
    //    }

    if (FALSE
        == bytes_to_int_big_to_little_endain(pst_firmware_packet->st_firmware_packet_pac.st_pac_packet.ba_package_num,
                                             ( uint32_t * )&dw_package_num_temp,
                                             2))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        return FALSE;
    }

    if (FALSE
        == bytes_to_int_big_to_little_endain(pst_firmware_packet->st_firmware_packet_pac.st_pac_packet.ba_package_sum,
                                             ( uint32_t * )&dw_package_sum_temp,
                                             2))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        return FALSE;
    }

    if (dw_package_sum_temp < dw_package_num_temp)
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        return FALSE;
    }

    if (FALSE
        == byte_fill_in_reverse_order(4,
                                      pst_firmware_packet->st_firmware_packet_pac.st_pac_packet.ba_package_num,
                                      pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketIndex,
                                      4))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        return FALSE;
    }

    for (uint16_t w_cnt_i = 0; w_cnt_i < w_firmware_packet_len; w_cnt_i++)
    {
        pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.data.bPacketData[w_cnt_i] =
            pst_firmware_packet->st_firmware_packet_pac.st_pac_packet.ba_package_num[w_cnt_i];
    }
    w_data_len += w_firmware_packet_len;

    if (FALSE
        == calculate_crc16(
            pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.data.bPacketData,
            w_data_len,
            &pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.data.bPacketData[w_firmware_packet_len],
            &pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.data.bPacketData[w_firmware_packet_len + 1]))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        return FALSE;
    }
    w_data_len += 2;

    pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketDataLen[0] = ( uint8_t )(w_data_len & 0x00FF);
    pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketDataLen[1] = ( uint8_t )((w_data_len & 0xFF00) >> 8);

    if (w_packet_index_backup
        == pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info
               .dw_packets_receive_current)
    {
        uint8_t ba_packet_temp[cdwLocalUpdateFirmDataLen] = {
            0,
        };

        ftp_extern_flash_read(
            (dw_ble_firmware_data_spi_flash_addr - w_data_len_backup - 6), (w_data_len_backup + 6), ba_packet_temp);
        if (0
            == memcmp(( char * )ba_packet_temp,
                      ( char * )pstLocalUpdateSpiFlashFirmwarePacket->bData,
                      w_data_len_backup + 6))
        {
            vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
            debug_updata_error("[updata]line %d\r\n", __LINE__);
            return TRUE;
        }
        else
        {
            vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
            debug_updata_error("[updata]line %d\r\n", __LINE__);
            return FALSE;
        }
    }

    if (FALSE
        == ftp_extern_flash_wirte(
            dw_ble_firmware_data_spi_flash_addr, (w_data_len + 6), pstLocalUpdateSpiFlashFirmwarePacket->bData))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        return FALSE;
    }
    {
        w_packet_index_backup = pst_device_update_info_current->pst_net_device_update_info_current
                                    ->st_net_firmware_info.dw_packets_receive_current;
        w_data_len_backup = w_data_len;
    }

    dw_ble_firmware_data_spi_flash_addr += w_data_len + 6;

    //    DebugPrintf(TRUE,"firmware_update_pac_packet_data_store,store_data_end,time: %d\n",xTaskGetTickCount());

    {

        if (pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info
                .dw_packets_receive_current
            == pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info
                   .dw_packets_receive_total)
        {
            b_local_firmware_enable = TRUE;
            w_packet_index_backup = 1;
            w_data_len_backup = 0;
            fWriteEEPage(15) = TRUE;
            yl_debug_printf(
                "dw_packets_receive_current = dw_packets_receive_total&& b_local_firmware_enable == TRUE \n");
        }

        if (pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info
                .dw_packets_receive_current
            == pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info
                       .dw_packets_receive_total
                   - 1)
        {
            b_firmware_update_pac_packet_store_finish_flag = 1;
            yl_debug_printf("dw_packets_receive_current = dw_packets_receive_total - 1 \n");
        }
    }

    vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);

    return TRUE;
}

uint8_t ftp_firmware_update_packet_operate_pac(uint16_t w_firmware_packet_len,
                                               firmware_packet *pst_firmware_packet,
                                               net_system_info_list *pst_net_system_info_list)
{
    if ((NULL == pst_firmware_packet) || (NULL == pst_net_system_info_list))
    {
        return FALSE;
    }

    uint8_t b_cmd_type = cbFirmwareDataError;
    uint32_t dw_packet_number = 0;

    if (FALSE
        == bytes_to_int_big_to_little_endain(
            pst_firmware_packet->st_firmware_packet_pac.ba_package_num, &dw_packet_number, 4))
    {
        return b_cmd_type;
    }

    if (0x0001 == dw_packet_number)    // first packet:contain packet info
    {
        if (TRUE == ftp_firmware_update_traget_pairing_pac(pst_firmware_packet, pst_net_system_info_list))
        {
            if (NULL == st_device_update_info_current.pst_net_device_update_info_current)
            {
                b_cmd_type = cbFirmwareIDNotSupport;
            }
            else
            {
                UPDATE_CURRENT_CURRENT = 1;
                if (TRUE
                    == ftp_firmware_update_pac_packet_info_store(
                        w_firmware_packet_len - 4, pst_firmware_packet, &st_device_update_info_current))
                {
                    b_cmd_type = cbNetCmdOK;
                }
                else
                {
                    b_cmd_type = cbFirmwareDataError;
                }
            }
        }
    }
    else if (dw_packet_number > 1)    // remaining packets:contain packet data
    {
        if (1 == b_firmware_update_pac_packet_store_finish_flag)
        {
            b_firmware_update_pac_packet_store_finish_flag = 0;
        }

        if ((UPDATE_CURRENT_CURRENT) == dw_packet_number)
        {

            if (TRUE
                == ftp_firmware_update_pac_packet_data_store(
                    w_firmware_packet_len - 4, pst_firmware_packet, &st_device_update_info_current))
            {
                b_cmd_type = cbNetCmdOK;
                if (UPDATE_CURRENT_CURRENT != UPDATE_CURRENT_TOTAL)
                {
                    UPDATE_CURRENT_CURRENT++;
                }
            }
            else
            {
                yl_debug_printf("\r\n[updata]flash error %d\r\n", UPDATE_CURRENT_CURRENT);
            }
        }
        else if ((UPDATE_CURRENT_CURRENT) > dw_packet_number)
        {
            b_cmd_type = cbNetCmdOK;
        }
        else
        {
            b_cmd_type = cbFirmwareDataError;
        }
    }
    else
    {
        b_cmd_type = cbUpdatePackageSumError;
    }
    return b_cmd_type;
}

uint8_t ftp_firmware_update_pacx_dsp_packet_info_store(uint16_t w_firmware_packet_len,
                                                       firmware_packet *pst_firmware_packet,
                                                       device_update_info_current *pst_device_update_info_current)
{
    uint16_t w_data_len = 0;
    uint32_t dw_package_num_temp = 0;
    uint32_t dw_package_sum_temp = 0;

    LocalUpdateSpiFlashFirmwarePacket *pstLocalUpdateSpiFlashFirmwarePacket = 0;

    dw_ble_firmware_data_spi_flash_addr = ble_cdwDevFirmAddrStart;
    if (FALSE == ftp_extern_flash_wirte(dw_ble_firmware_data_spi_flash_addr, 1, ( uint8_t * )&flash_flag_start))
    {
        debug_updata_error("[flash]write flag fail \r\n");
        return FALSE;
    }
    dw_ble_firmware_data_spi_flash_addr += cbFlashFirmInfoLen;

    //    do{
    //        pstLocalUpdateSpiFlashFirmwarePacket = (LocalUpdateSpiFlashFirmwarePacket
    //        *)pvPortMalloc(sizeof(LocalUpdateSpiFlashFirmwarePacket));
    //    }while(NULL == pstLocalUpdateSpiFlashFirmwarePacket);
    pstLocalUpdateSpiFlashFirmwarePacket =
        ( LocalUpdateSpiFlashFirmwarePacket * )pvPortMalloc(sizeof(LocalUpdateSpiFlashFirmwarePacket));
    if (NULL == pstLocalUpdateSpiFlashFirmwarePacket)
    {
        return FALSE;
    }

    // debug_updata_test("[updata] flash addr 0x%x\r\n", dw_ble_firmware_data_spi_flash_addr);
    if (NULL == memset(pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketIndex, 0x00, 4))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        return FALSE;
    }

    if (FALSE
        == bytes_to_int_big_to_little_endain(
            pst_firmware_packet->st_firmware_packet_pacx_dsp.ba_packet_number, ( uint32_t * )&dw_package_num_temp, 4))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        return FALSE;
    }

    dw_package_sum_temp = pst_device_update_info_current->dw_packets_number;

    if (dw_package_sum_temp < dw_package_num_temp)
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        return FALSE;
    }

    // small to big edain
    if (FALSE
        == byte_fill_in_reverse_order(4,
                                      pst_firmware_packet->st_firmware_packet_pacx_dsp.ba_packet_number,
                                      pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketIndex,
                                      4))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        return FALSE;
    }

    memcpy(( char * )&pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.bData[4],
           ( char * )&pst_firmware_packet->st_firmware_packet_pacx_dsp.un_pacx_dsp_packet.st_pacx_dsp_packet_info
               .ba_header,
           w_firmware_packet_len);

    *(( uint32_t * )&pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.bData[0]) = dw_package_sum_temp;

    w_data_len = w_firmware_packet_len + 4;
    if (FALSE
        == calculate_crc16(pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.bData,
                           w_data_len,
                           &pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.bData[w_data_len],
                           &pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.bData[w_data_len + 1]))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        return FALSE;
    }
    w_data_len += 2;

    pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketDataLen[0] = ( uint8_t )(w_data_len & 0x00FF);
    pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketDataLen[1] = ( uint8_t )((w_data_len & 0xFF00) >> 8);

    if (FALSE
        == ftp_extern_flash_wirte(
            dw_ble_firmware_data_spi_flash_addr, (w_data_len + 6), pstLocalUpdateSpiFlashFirmwarePacket->bData))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        debug_updata_error("\r\n[updata]flash fail LINE is : %d\r\n",__LINE__);
		return FALSE;
    }
    DebugPrintf(TRUE, "firmware_update_pac_packet_info_store,store_data_end,time: %d\n", xTaskGetTickCount());

    dw_ble_firmware_data_spi_flash_addr += w_data_len + 6;

    {
        pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info
            .dw_packets_receive_current++;

        pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info.b_packet_format =
            pst_device_update_info_current->b_packet_format;

        pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info
            .dw_packets_receive_total = pst_device_update_info_current->dw_packets_number;

        memcpy(pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info.ba_firmware_id,
               pst_device_update_info_current->ba_firmware_id,
               cb_firmware_id_max_len);
    }

    vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);

    return TRUE;
}

extern uint8_t b_firmware_update_pacx_dsp_packet_store_finish_flag;
uint8_t ftp_firmware_update_pacx_dsp_packet_data_store(uint16_t w_firmware_packet_len,
                                                       firmware_packet *pst_firmware_packet,
                                                       device_update_info_current *pst_device_update_info_current)
{
    uint16_t w_data_len = 0;
    static uint16_t w_packet_index_backup = 1;
    static uint16_t w_data_len_backup = 0;

    LocalUpdateSpiFlashFirmwarePacket *pstLocalUpdateSpiFlashFirmwarePacket = 0;

    //    do{
    //        pstLocalUpdateSpiFlashFirmwarePacket = (LocalUpdateSpiFlashFirmwarePacket
    //        *)pvPortMalloc(sizeof(LocalUpdateSpiFlashFirmwarePacket));
    //    }while(NULL == pstLocalUpdateSpiFlashFirmwarePacket);
    pstLocalUpdateSpiFlashFirmwarePacket =
        ( LocalUpdateSpiFlashFirmwarePacket * )pvPortMalloc(sizeof(LocalUpdateSpiFlashFirmwarePacket));
    if (NULL == pstLocalUpdateSpiFlashFirmwarePacket)
    {

        return FALSE;
    }

    if (FALSE
        == byte_fill_in_reverse_order(4,
                                      pst_firmware_packet->st_firmware_packet_pacx_dsp.ba_packet_number,
                                      pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketIndex,
                                      4))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        return FALSE;
    }

    *(( uint32_t * )&pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.bData[0]) =
        pst_device_update_info_current->dw_packets_number;

    w_data_len += 4;

    for (uint16_t w_cnt_i = 0; w_cnt_i < w_firmware_packet_len; w_cnt_i++)
    {
        pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.data.bPacketData[w_cnt_i + 4] =
            pst_firmware_packet->st_firmware_packet_pacx_dsp.un_pacx_dsp_packet.st_pacx_packet_data
                .b_packet_data[w_cnt_i];
    }
    w_data_len += w_firmware_packet_len;

    if (FALSE
        == calculate_crc16(
            pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.data.bPacketData,
            w_data_len,
            &pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.data.bPacketData[w_data_len],
            &pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.data.bPacketData[w_data_len + 1]))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        return FALSE;
    }
    w_data_len += 2;

    pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketDataLen[0] = ( uint8_t )(w_data_len & 0x00FF);
    pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketDataLen[1] = ( uint8_t )((w_data_len & 0xFF00) >> 8);

    if (w_packet_index_backup
        == pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info
               .dw_packets_receive_current)
    {
        uint8_t ba_packet_temp[cdwLocalUpdateFirmDataLen] = {
            0,
        };

        ftp_extern_flash_read(
            (dw_ble_firmware_data_spi_flash_addr - w_data_len_backup - 6), (w_data_len_backup + 6), ba_packet_temp);
        if (0
            == memcmp(( char * )ba_packet_temp,
                      ( char * )pstLocalUpdateSpiFlashFirmwarePacket->bData,
                      w_data_len_backup + 6))
        {
            vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
            debug_updata_error("[updata]line %d\r\n", __LINE__);
            return TRUE;
        }
        else
        {
            vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
            debug_updata_error("[updata]line %d\r\n", __LINE__);
            return FALSE;
        }
    }

    if (FALSE
        == ftp_extern_flash_wirte(
            dw_ble_firmware_data_spi_flash_addr, (w_data_len + 6), pstLocalUpdateSpiFlashFirmwarePacket->bData))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        return FALSE;
    }
    {
        w_packet_index_backup = pst_device_update_info_current->pst_net_device_update_info_current
                                    ->st_net_firmware_info.dw_packets_receive_current;
        w_data_len_backup = w_data_len;
    }

    dw_ble_firmware_data_spi_flash_addr += w_data_len + 6;

    //    DebugPrintf(TRUE,"local_firmware_update_packet_data_store,store_data_end,time: %d\n",xTaskGetTickCount());

    {
        if (pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info
                .dw_packets_receive_current
            == pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info
                   .dw_packets_receive_total)
        {
            b_local_firmware_enable = TRUE;
            w_packet_index_backup = 1;
            w_data_len_backup = 0;
            fWriteEEPage(15) = TRUE;
            yl_debug_printf(
                "dw_packets_receive_current = dw_packets_receive_total&& b_local_firmware_enable == TRUE \n");
        }
    }
    if (pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info
            .dw_packets_receive_current
        == pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info
                   .dw_packets_receive_total
               - 2)
    {
        b_firmware_update_pacx_dsp_packet_store_finish_flag = 1;
        yl_debug_printf("[Update]dw_packets_receive_current is : %d,dw_packets_receive_total is %d\r\n",
                        pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info
                            .dw_packets_receive_current,
                        pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info
                            .dw_packets_receive_total);
    }

    vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);

    return TRUE;
}

uint8_t ftp_firmware_update_packet_operate_pacx_dsp(uint16_t w_firmware_packet_len,
                                                    firmware_packet *pst_firmware_packet,
                                                    net_system_info_list *pst_net_system_info_list)
{
    if ((NULL == pst_firmware_packet) || (NULL == pst_net_system_info_list))
    {
        return FALSE;
    }

    uint8_t b_cmd_type = cbFirmwareDataError;
    uint32_t dw_packet_number = 0;

    if (FALSE
        == bytes_to_int_big_to_little_endain(
            pst_firmware_packet->st_firmware_packet_pacx_dsp.ba_packet_number, &dw_packet_number, 4))
    {
        return b_cmd_type;
    }

    if (0x0001 == dw_packet_number)    // first packet:contain packet info
    {
        if (TRUE == ftp_firmware_update_traget_pairing_pacx_dsp(pst_firmware_packet, pst_net_system_info_list))
        {
            if (NULL == st_device_update_info_current.pst_net_device_update_info_current)
            {
                b_cmd_type = cbFirmwareIDNotSupport;
            }
            else
            {
                UPDATE_CURRENT_CURRENT = 1;
                if (TRUE
                    == ftp_firmware_update_pacx_dsp_packet_info_store(
                        w_firmware_packet_len - 4, pst_firmware_packet, &st_device_update_info_current))
                {
                    b_cmd_type = cbNetCmdOK;
                }
                else
                {
                    b_cmd_type = cbFirmwareDataError;
                }
            }
        }
    }
    else if (dw_packet_number > 1)    // remaining packets:contain packet data
    {
        if ((UPDATE_CURRENT_CURRENT) == dw_packet_number)
        {

            if (TRUE
                == ftp_firmware_update_pacx_dsp_packet_data_store(
                    w_firmware_packet_len - 4, pst_firmware_packet, &st_device_update_info_current))
            {
                b_cmd_type = cbNetCmdOK;
                if (UPDATE_CURRENT_CURRENT != UPDATE_CURRENT_TOTAL)
                {
                    UPDATE_CURRENT_CURRENT++;
                }
            }
            else
            {
                yl_debug_printf("\r\n[updata]flash error %d\r\n", UPDATE_CURRENT_CURRENT);
            }
        }
        else if ((UPDATE_CURRENT_CURRENT) > dw_packet_number)
        {
            b_cmd_type = cbNetCmdOK;
        }
        else
        {
            b_cmd_type = cbFirmwareDataError;
        }
    }
    else
    {
        b_cmd_type = cbUpdatePackageSumError;
    }
    return b_cmd_type;
}

/* -------------------------------------------------------------------------- */
/*                               PACX MCU固件消息接收                               */
/* -------------------------------------------------------------------------- */
uint8_t ftp_local_firmware_update_packet_info_store(uint16_t w_firmware_packet_len,
                                                    firmware_packet *pst_firmware_packet,
                                                    device_update_info_current *pst_device_update_info_current)
{
    uint16_t w_data_len = 0;

    LocalUpdateSpiFlashFirmwarePacket *pstLocalUpdateSpiFlashFirmwarePacket = 0;

    dw_ble_firmware_data_spi_flash_addr = ble_cdwDevFirmAddrStart;
    if (FALSE == ftp_extern_flash_wirte(dw_ble_firmware_data_spi_flash_addr, 1, ( uint8_t * )&flash_flag_start))
    {
        debug_updata_error("[flash]write flag fail \r\n");
        return FALSE;
    }
    dw_ble_firmware_data_spi_flash_addr += cbFlashFirmInfoLen;
    pstLocalUpdateSpiFlashFirmwarePacket =
        ( LocalUpdateSpiFlashFirmwarePacket * )pvPortMalloc(sizeof(LocalUpdateSpiFlashFirmwarePacket));
    if (NULL == pstLocalUpdateSpiFlashFirmwarePacket)
    {
        return FALSE;
    }

    if (NULL == memset(pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketIndex, 0x00, 4))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        return FALSE;
    }
    // small to big edain
    if (FALSE
        == byte_fill_in_reverse_order(4,
                                      pst_firmware_packet->st_firmware_packet_pacx.ba_packet_number,
                                      pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketIndex,
                                      4))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        return FALSE;
    }

    if (FALSE
        == int_to_bytes_little_endain(pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.info.bPacketsNumber,
                                      pst_device_update_info_current->dw_packets_number,
                                      4))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        return FALSE;
    }

    w_data_len += 4;

    pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.info.bMcuType =
        pst_firmware_packet->st_firmware_packet_pacx.un_pacx_packet.st_pacx_packet_info.b_mcu_type;
    w_data_len += 1;

    if (FALSE
        == byte_fill_in_reverse_order(
            3,
            pst_firmware_packet->st_firmware_packet_pacx.un_pacx_packet.st_pacx_packet_info.ba_firmware_type,
            pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.info.bFirmwareType,
            3))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        return FALSE;
    }
    w_data_len += 3;

    if (FALSE
        == byte_fill_in_reverse_order(
            3,
            pst_firmware_packet->st_firmware_packet_pacx.un_pacx_packet.st_pacx_packet_info.ba_firmware_app_version,
            pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.info.bAppVersion,
            3))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        return FALSE;
    }
    w_data_len += 3;
    if (FALSE
        == byte_fill_in_reverse_order(
            2,
            pst_firmware_packet->st_firmware_packet_pacx.un_pacx_packet.st_pacx_packet_info.ba_firmware_boot_version,
            pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.info.bbootVersion,
            2))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
#if (dynamic_memory_request_test)
#endif
        return FALSE;
    }
    w_data_len += 2;

    if (0
        == memcpy(pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.info.bkey,
                  pst_firmware_packet->st_firmware_packet_pacx.un_pacx_packet.st_pacx_packet_info.ba_encryption_key,
                  cbLocalFirmwarePacketKeyLen))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        return FALSE;
    }
    w_data_len += 12;

    pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.info.bUpdateType =
        pst_firmware_packet->st_firmware_packet_pacx.un_pacx_packet.st_pacx_packet_info.b_encryption_type;
    w_data_len += 1;

    if (FALSE
        == calculate_crc16(pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.bData,
                           w_data_len,
                           &pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.info.bCrc16[0],
                           &pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.info.bCrc16[1]))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        return FALSE;
    }
    w_data_len += 2;

    pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketDataLen[0] = ( uint8_t )(w_data_len & 0x00FF);
    pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketDataLen[1] = ( uint8_t )((w_data_len & 0xFF00) >> 8);

    if (FALSE == ftp_extern_flash_wirte(dw_ble_firmware_data_spi_flash_addr, (w_data_len + 6), pstLocalUpdateSpiFlashFirmwarePacket->bData))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
        debug_updata_error("\r\n[updata]flash fail LINE is : %d\r\n",__LINE__);
		return FALSE;
    }

    dw_ble_firmware_data_spi_flash_addr += w_data_len + 6;

    {
        pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info
            .dw_packets_receive_current++;

        pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info.b_packet_format =
            pst_device_update_info_current->b_packet_format;

        pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info
            .dw_packets_receive_total = pst_device_update_info_current->dw_packets_number;

        memcpy(pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info.ba_firmware_id,
               pst_device_update_info_current->ba_firmware_id,
               cb_firmware_id_max_len);
    }

    vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
    return TRUE;
}

/* -------------------------------------------------------------------------- */
/*                              PACX MCU类型固件包无线接收                             */
/* -------------------------------------------------------------------------- */
uint8_t ftp_local_firmware_update_packet_data_store(uint16_t w_firmware_packet_len,
                                                    firmware_packet *pst_firmware_packet,
                                                    device_update_info_current *pst_device_update_info_current)
{
    uint16_t w_data_len = 0;
    static uint16_t w_packet_index_backup = 1;
    static uint16_t w_data_len_backup = 0;

    LocalUpdateSpiFlashFirmwarePacket *pstLocalUpdateSpiFlashFirmwarePacket = 0;

    //    do{
    //        pstLocalUpdateSpiFlashFirmwarePacket = (LocalUpdateSpiFlashFirmwarePacket
    //        *)pvPortMalloc(sizeof(LocalUpdateSpiFlashFirmwarePacket));
    //    }while(NULL == pstLocalUpdateSpiFlashFirmwarePacket);
    pstLocalUpdateSpiFlashFirmwarePacket =
        ( LocalUpdateSpiFlashFirmwarePacket * )pvPortMalloc(sizeof(LocalUpdateSpiFlashFirmwarePacket));
    if (NULL == pstLocalUpdateSpiFlashFirmwarePacket)
    {
#if (system_alarm_print)
        DebugPrintf(TRUE, "[ERROR]Update pacx malloc err:2\r\n");
#endif
        return FALSE;
    }
#if (dynamic_memory_request_test)
    DebugPrintf(TRUE, "local_firmware_update_packet_data_store_0_malloc%x\n", pstLocalUpdateSpiFlashFirmwarePacket);
#endif

    if (FALSE
        == byte_fill_in_reverse_order(4,
                                      pst_firmware_packet->st_firmware_packet_pacx.ba_packet_number,
                                      pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketIndex,
                                      4))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
#if (dynamic_memory_request_test)
        DebugPrintf(TRUE, "local_firmware_update_packet_data_store_0_free%x\n", pstLocalUpdateSpiFlashFirmwarePacket);
#endif
        return FALSE;
    }

    //    debug_updata_test("[updata] pack num\r\n");
    //    debug_updata_sector( pst_firmware_packet->st_firmware_packet_pacx.ba_packet_number,4);
    //    debug_updata_sector(pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketIndex,4);

    for (uint16_t w_cnt_i = 0; w_cnt_i < w_firmware_packet_len; w_cnt_i++)
    {
        pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.data.bPacketData[w_cnt_i] =
            pst_firmware_packet->st_firmware_packet_pacx.un_pacx_packet.st_pacx_packet_data.b_packet_data[w_cnt_i];
    }
    w_data_len += w_firmware_packet_len;

    if (FALSE
        == calculate_crc16(
            pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.data.bPacketData,
            w_data_len,
            &pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.data.bPacketData[w_firmware_packet_len],
            &pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.Packet.data.bPacketData[w_firmware_packet_len + 1]))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
#if (dynamic_memory_request_test)
        DebugPrintf(TRUE, "local_firmware_update_packet_data_store_1_free%x\n", pstLocalUpdateSpiFlashFirmwarePacket);
#endif
        return FALSE;
    }
    w_data_len += 2;

    pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketDataLen[0] = ( uint8_t )(w_data_len & 0x00FF);
    pstLocalUpdateSpiFlashFirmwarePacket->SpiFlashPacket.bPacketDataLen[1] = ( uint8_t )((w_data_len & 0xFF00) >> 8);

    if (w_packet_index_backup
        == pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info
               .dw_packets_receive_current)
    {
        uint8_t ba_packet_temp[cdwLocalUpdateFirmDataLen] = {
            0,
        };

        ftp_extern_flash_read(
            (dw_ble_firmware_data_spi_flash_addr - w_data_len_backup - 6), (w_data_len_backup + 6), ba_packet_temp);
        if (0
            == memcmp(( char * )ba_packet_temp,
                      ( char * )pstLocalUpdateSpiFlashFirmwarePacket->bData,
                      w_data_len_backup + 6))
        {
            vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
            debug_updata_error("[updata]line %d\r\n", __LINE__);
            return TRUE;
        }
        else
        {
            vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
            debug_updata_error("[updata]line %d\r\n", __LINE__);
            return FALSE;
        }
    }

    // debug_updata_test("[updata] fire addr %x\r\n", dw_ble_firmware_data_spi_flash_addr);
    //    DebugPrintf(TRUE,"local_firmware_update_packet_data_store,store_data_start,time: %d\n",xTaskGetTickCount());
    if (FALSE
        == ftp_extern_flash_wirte(
            dw_ble_firmware_data_spi_flash_addr, (w_data_len + 6), pstLocalUpdateSpiFlashFirmwarePacket->bData))
    {
        vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
#if (dynamic_memory_request_test)
        DebugPrintf(TRUE, "local_firmware_update_packet_data_store_2_free%x\n", pstLocalUpdateSpiFlashFirmwarePacket);
#endif
        return FALSE;
    }
    {
        w_packet_index_backup = pst_device_update_info_current->pst_net_device_update_info_current
                                    ->st_net_firmware_info.dw_packets_receive_current;
        w_data_len_backup = w_data_len;
    }

    dw_ble_firmware_data_spi_flash_addr += w_data_len + 6;

    //    DebugPrintf(TRUE,"local_firmware_update_packet_data_store,store_data_end,time: %d\n",xTaskGetTickCount());

    {
        //        pst_device_update_info_current->
        //        pst_net_device_update_info_current->
        //        st_net_firmware_info.
        //        dw_packets_receive_current++;

        if (pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info
                .dw_packets_receive_current
            == pst_device_update_info_current->pst_net_device_update_info_current->st_net_firmware_info
                   .dw_packets_receive_total)
        {
            b_local_firmware_enable = TRUE;
            w_packet_index_backup = 1;
            w_data_len_backup = 0;
            fWriteEEPage(15) = TRUE;
            yl_debug_printf("\r\n[updata]pack finish\r\n");
        }
    }

    vPortFree(pstLocalUpdateSpiFlashFirmwarePacket);
#if (dynamic_memory_request_test)
    DebugPrintf(TRUE, "local_firmware_update_packet_data_store_4_free%x\n", pstLocalUpdateSpiFlashFirmwarePacket);
#endif

    return TRUE;
}

uint8_t ftp_firmware_update_packet_operate_pacx(uint16_t w_firmware_packet_len,
                                                firmware_packet *pst_firmware_packet,
                                                net_system_info_list *pst_net_system_info_list)
{
    if ((NULL == pst_firmware_packet) || (NULL == pst_net_system_info_list))
    {
        return FALSE;
    }

    uint8_t b_cmd_type = cbFirmwareDataError;
    uint32_t dw_packet_number = 0;

    if (FALSE
        == bytes_to_int_big_to_little_endain(
            pst_firmware_packet->st_firmware_packet_pacx.ba_packet_number, &dw_packet_number, 4))
    {
        return b_cmd_type;
    }

    if (0x0001 == dw_packet_number)    // first packet:contain packet info
    {
        if (TRUE == ftp_firmware_update_traget_pairing_pacx(pst_firmware_packet, pst_net_system_info_list))
        {
            if (NULL == st_device_update_info_current.pst_net_device_update_info_current)
            {
                b_cmd_type = cbFirmwareIDNotSupport;
            }
            else
            {
                UPDATE_CURRENT_CURRENT = 1;
                if (TRUE
                    == ftp_local_firmware_update_packet_info_store(
                        w_firmware_packet_len - 4, pst_firmware_packet, &st_device_update_info_current))
                {
                    b_cmd_type = cbNetCmdOK;
                }
                else
                {
                    yl_debug_printf("\r\n[updata]local_firmware_update_packet_info_store %d\r\n");
                }
            }
        }
        else
        {
            yl_debug_printf("\r\n[updata]cbFirmwareIDNotSupport\r\n");
            b_cmd_type = cbFirmwareIDNotSupport;
        }
    }
    else if (dw_packet_number > 1)    // remaining packets:contain packet data
    {

        if ((UPDATE_CURRENT_CURRENT) == dw_packet_number)
        {

            if (TRUE
                == ftp_local_firmware_update_packet_data_store(
                    w_firmware_packet_len - 4, pst_firmware_packet, &st_device_update_info_current))
            {
                b_cmd_type = cbNetCmdOK;
                if (UPDATE_CURRENT_CURRENT < UPDATE_CURRENT_TOTAL)
                {
                    UPDATE_CURRENT_CURRENT++;
                }
            }
            else
            {
                yl_debug_printf("\r\n[updata]flash error %d\r\n", UPDATE_CURRENT_CURRENT);
            }
        }
        else if ((UPDATE_CURRENT_CURRENT) > dw_packet_number)
        {
            yl_debug_printf("\r\n[updata]repeat %d,%d\r\n", dw_packet_number, UPDATE_CURRENT_CURRENT);
            b_cmd_type = cbNetCmdOK;
        }
        else
        {
            yl_debug_printf("\r\n[updata]advance %d,%d\r\n", dw_packet_number, UPDATE_CURRENT_CURRENT);
            b_cmd_type = cbFirmwareDataError;
        }
    }
    else
    {
        b_cmd_type = cbUpdatePackageSumError;
    }

    return b_cmd_type;
}

uint8_t ftp_firmware_update_packet_operate(uint16_t w_firmware_packet_len,
                                           firmware_packet *pst_firmware_packet,
                                           net_system_info_list *pst_net_system_info_list)
{
    uint8_t b_cmd = cbUpdateTypeError;
    uint32_t dw_package_num_temp = 0;
    bytes_to_int_big_to_little_endain(
        pst_firmware_packet->st_firmware_packet_pacx_dsp.ba_packet_number, ( uint32_t * )&dw_package_num_temp, 4);

    if ((dw_package_num_temp == 1) && (cb_firmware_format_none == st_device_update_info_current.b_update_type))
    {
        st_device_update_info_current.b_update_type = pst_firmware_packet->st_firmware_packet_pacx_dsp
                                                          .un_pacx_dsp_packet.st_pacx_dsp_packet_info.b_identification;
    }

    switch (st_device_update_info_current.b_update_type)
    {
        case cb_firmware_format_pac:
        {
            b_cmd = ftp_firmware_update_packet_operate_pac(
                w_firmware_packet_len, pst_firmware_packet, pst_net_system_info_list);
        }
        break;
        case cb_firmware_format_pacx_mcu:
        {
            b_cmd = ftp_firmware_update_packet_operate_pacx(
                w_firmware_packet_len, pst_firmware_packet, pst_net_system_info_list);
        }
        break;
        case cb_firmware_format_pacx_dsp:
        {
            b_cmd = ftp_firmware_update_packet_operate_pacx_dsp(
                w_firmware_packet_len, pst_firmware_packet, pst_net_system_info_list);
        }
        break;
        default:
            st_device_update_info_current.b_update_type = 0;
            break;
    }
    return b_cmd;
}

// 保存数据到flash
uint8_t ftp_updata_save_to_flash(uint8_t *data, uint16_t *len)
{

    ftp_firmware_update_packet_operate(*len, ( firmware_packet * )data, &st_net_system_info_list);

    return 0;
}

uint16_t pcakage_num_index = 0;
// 解析数据包
uint16_t pcakage_num_index_last = 0;
void ftp_updata_analysis(uint8_t *data, uint16_t *len)
{
    uint8_t *data_temp = ( uint8_t * )pvPortMalloc(*len * sizeof(uint8_t));
    *len = *len - 18;
    memcpy(data_temp, data + 18, *len);

    uint16_t cmd = data[16] << 8 | data[17];

    switch (cmd)
    {
        case cw_FTPData:
        {
            pcakage_num_index = data[18] << 24 | data[19] << 16 | data[20] << 8 | data[21];

            if (pcakage_num_index != pcakage_num_index_last + 1)
            {
                yl_debug_printf(
                    "pcakage_num_index_last:%d, pcakage_num_index:%d\n", pcakage_num_index_last, pcakage_num_index);
            }
            else
            {
                ftp_updata_save_to_flash(data_temp, len);
            }
            pcakage_num_index_last = pcakage_num_index;
        }
        break;

        default:
            yl_debug_printf("cmd error = %d\n", cmd);
            ftp_updata_task_delete = 1;
            lte_system_tick = xTaskGetTickCount();
            break;
    }

    if (pcakage_num_index >= ftp_updata_package_num_all)
    {
    	ftp_updata_package_num_all = 0;
        ftp_updata_task_delete = 1;
        ble_ftp_updata_flag = 1;
        lte_system_tick = xTaskGetTickCount();
    }

    vPortFree(data_temp);
}

void ftp_spi_flash_operate_process()
{
    uint8_t *pb_spi_flash_rx_buffer = 0;
    spi_flash_operate_initiate_msg *pst_spi_flash_operate_initiate_msg = 0;
    spi_flash_operate_response_msg *pst_spi_flash_operate_response_msg = 0;

    {
        if (0 != dequeue(&ftp_flash_initiate, ( void * )&pst_spi_flash_operate_initiate_msg))
        {
            return;
        }

        if (0 == pst_spi_flash_operate_initiate_msg)
        {
            return;
        }
    }

    switch (pst_spi_flash_operate_initiate_msg->enum_operate_type)
    {
        case spi_flash_read_request:
        {
            /*Fast Read*/
            {
                pb_spi_flash_rx_buffer = ( uint8_t * )pvPortMalloc(
                    (pst_spi_flash_operate_initiate_msg->st_data_info.w_data_len) * sizeof(uint8_t));
                if (pb_spi_flash_rx_buffer)
                {
                    {
                        drv_flash_read(pb_spi_flash_rx_buffer,
                                       pst_spi_flash_operate_initiate_msg->dw_operate_addr,
                                       (pst_spi_flash_operate_initiate_msg->st_data_info.w_data_len));

                        memcpy(pst_spi_flash_operate_initiate_msg->st_data_info.pb_data,
                               ( uint8_t * )(pb_spi_flash_rx_buffer),
                               pst_spi_flash_operate_initiate_msg->st_data_info.w_data_len);
                        if (pb_spi_flash_rx_buffer != 0)
                        {
                            vPortFree(pb_spi_flash_rx_buffer);
                        }
                    }
                }
                else
                {
                }
            }
            break;
        }
        case spi_flash_write_request:
        {
            drv_flash_write_buffer(pst_spi_flash_operate_initiate_msg->st_data_info.pb_data,
                                   pst_spi_flash_operate_initiate_msg->dw_operate_addr,
                                   pst_spi_flash_operate_initiate_msg->st_data_info.w_data_len);
            break;
        }
        case spi_flash_erase_request:
        {
            drv_flash_erase_sector(pst_spi_flash_operate_initiate_msg->dw_operate_addr);
            break;
        }
        default:
        {
            break;
        }
    }

    {
        pst_spi_flash_operate_response_msg =
            ( spi_flash_operate_response_msg * )pvPortMalloc(sizeof(spi_flash_operate_response_msg));
#if (dynamic_memory_request_test)
        DebugPrintf(TRUE, "spi_flash_operate_process_14_malloc%x\n", pst_spi_flash_operate_response_msg);
#endif

        if (0 == pst_spi_flash_operate_response_msg)
        {
            return;
        }

        pst_spi_flash_operate_response_msg->b_spi_flash_operate_status = 1;
        pst_spi_flash_operate_response_msg->dw_spi_flash_operate_response_uuid =
            pst_spi_flash_operate_initiate_msg->dw_spi_flash_operate_initiate_uuid;

        if (0 != enqueue(&ftp_flash_response, ( void * )pst_spi_flash_operate_response_msg))
        {
            yl_debug_printf("ftp_flash_response is full\r\n");
            vPortFree(pst_spi_flash_operate_response_msg);
            return;
        }
    }
}

static void ftp_flash_task(void *parameter)
{

    while (1)
    {
        if (ftp_updata_task_delete == 0)
        {
            ftp_spi_flash_operate_process();
        }
        else
        {
            clear_queue(&ftp_flash_initiate);
            clear_queue(&ftp_flash_response);
            vTaskDelay(pdMS_TO_TICKS_USER(300));
        }
    }
}

static void ftp_updata_task(void *parameter)
{
    uint8_t ftp_receive_buff[lte_module_mps_size] = {
        0,
    };

    init_queue(&ftp_updata_queue);
    init_queue(&ftp_flash_initiate);
    init_queue(&ftp_flash_response);

    while (1)
    {
        if (ftp_updata_task_delete == 0)
        {
            ftp_data_receive_data(ftp_receive_buff, lte_module_mps_size);
//			yl_debug_printf("/***********************MACRO_IWDG_RELOD**************************/ LINE is : %d \r\n",__LINE__);
            MACRO_IWDG_RELOD();    // 喂狗
        }
        else
        {
            // yl_debug_printf("ftp_cnt = %d\r\n", ftp_cnt);
            // yl_debug_printf("ftp_len = %d\r\n", ftp_len);
            vTaskDelay(pdMS_TO_TICKS_USER(300));
        }
    }
}

typedef enum
{
    full_frame = 0,    // 完整帧
    head_frame,        // 头帧
    middle_frame,      // 中间帧
    end_frame          // 结尾帧
} ftp_updata_data_type_t;

// 将+WRITE:0,1,5,,**, 及结尾的0D0A从数据中删除
void ftp_delete_write(uint8_t *data, uint16_t *len)
{
    uint8_t cnt = 0;
    uint16_t i;

    for (i = 0; i < *len; i++)
    {
        if (data[i] == ',')
        {
            cnt++;
        }
        if (cnt == 5)
        {
            break;
        }
    }

    if (cnt == 5)
    {
        uint16_t new_len = *len - (i + 1) - 2;
        memmove(data, data + i + 1, new_len);
        *len = new_len;
    }
}

// 数据帧检测 如果有FE开头和FE结尾则为完整帧
ftp_updata_data_type_t ftp_updata_data_type_check(uint8_t *data, uint16_t len)
{
    uint16_t start_index = 0;
    uint16_t end_index = 0;
    uint16_t i = 0;
    uint16_t j = 0;

    for (i = 0; i < len; i++)
    {
        if (data[i] == 0xFE)
        {
            start_index = i;
            break;
        }
    }

    for (j = 0; j < len; j++)
    {
        if (data[len - j] == 0xFE)
        {
            end_index = len - j;
            break;
        }
    }

    if (end_index > start_index)
    {
        return full_frame;
    }
    else if (end_index == start_index && i < j && len > 19)
    {
        return head_frame;
    }
    else if (end_index == 0 && start_index == 0 && i == len && j == len)
    {
        return middle_frame;
    }
    else if (end_index == start_index)
    {
        return end_frame;
    }
}

typedef enum
{
    frame_complete = 0,    // 拼帧完成
    frame_fail,            // 拼帧失败
    frame_wait,            // 等待拼帧
} ftp_updata_data_merge_t;

ftp_updata_data_merge_t
ftp_updata_data_merge(uint8_t *dest, uint16_t *dest_len, uint16_t dest_max_len, uint8_t *src, uint16_t src_len)
{
    ftp_updata_data_type_t type = ftp_updata_data_type_check(src, src_len);

    switch (type)
    {
        case full_frame:
            // 如果源数据已经是一个完整的帧，直接复制到目标缓冲区
            if (src_len <= dest_max_len)
            {
                memcpy(dest, src, src_len);
                *dest_len = src_len;
                return frame_complete;
            }
            else
            {
                return frame_fail;
            }

        case head_frame:
            // 如果源数据是一个头帧，复制到目标缓冲区的开始位置
            if (src_len <= dest_max_len)
            {
                memcpy(dest, src, src_len);
                *dest_len = src_len;
                return frame_wait;
            }
            else
            {
                return frame_fail;
            }

        case middle_frame:
        case end_frame:
            // 如果源数据是一个中间帧或尾帧，追加到目标缓冲区的末尾
            if (*dest_len + src_len <= dest_max_len)
            {
                memcpy(dest + *dest_len, src, src_len);
                *dest_len += src_len;
                return (type == end_frame) ? frame_complete : frame_wait;
            }
            else
            {
                return frame_fail;
            }

        default:
            return frame_fail;
    }
}

// 拆分数据包
void ftp_updata_data_split(uint8_t *data, uint16_t *len)
{
    uint16_t start_index = 0;
    uint16_t end_index = 0;

    for (uint16_t i = 0; i < *len; i++)
    {
        if (0xFE == data[i])
        {
            start_index = i;
            break;
        }
    }
    for (uint16_t i = *len - 1; i > start_index; i--)
    {
        if (0xFE == data[i])
        {
            end_index = i;
            break;
        }
    }

    if (end_index > start_index)
    {
        uint8_t *data_temp = ( uint8_t * )pvPortMalloc((end_index - start_index + 1) * sizeof(uint8_t));
        memcpy(data_temp, &data[start_index], end_index - start_index + 1);
        *len = end_index - start_index + 1;

        // FD 00 转换为 FD，FD 01 转换为 FE
        uint16_t j = 0;
        for (uint16_t i = 0; i < *len; i++)
        {
            if (i < *len - 1 && data_temp[i] == 0xFD)
            {
                if (data_temp[i + 1] == 0x00)
                {
                    data_temp[j++] = 0xFD;
                    i++;
                }
                else if (data_temp[i + 1] == 0x01)
                {
                    data_temp[j++] = 0xFE;
                    i++;
                }
            }
            else
            {
                data_temp[j++] = data_temp[i];
            }
        }
        *len = j;

        // 头去掉三个直接 尾部去掉一个
        *len = *len - 4;
        memcpy(data, data_temp + 3, *len);
        vPortFree(data_temp);

        if (FALSE == check_crc16(data, *len))
        {
            *len = 0;
            yl_debug_printf("crc16 check err\r\n");
        }
        else
        {
            *len = *len - 2;    // 去掉crc16
            // yl_debug_printf("crc16 check ok\r\n");
        }
    }
    else
    {
        *len = 0;
    }
}

extern uint8_t flash_erase_sector_all_flag;
extern uint8_t gb_ble_updata_status;
static void ftp_printf_task(void *parameter)
{
    ftp_updata_data_t *ftp_updata_data = NULL;
    uint8_t frame_buffer[1024];    // 假设最大帧长度为1024
    uint16_t frame_len = 0;
    TickType_t xLastWakeTime;

    while (1)
    {
        if (ftp_updata_task_delete == 0)
        {
            if (xTaskGetTickCount() - xLastWakeTime > pdMS_TO_TICKS_USER(7000))
            {
            	extern uint8_t g_update_status;
                ftp_updata_task_delete = 1;
                lte_system_tick = xTaskGetTickCount();
                yl_debug_printf("receive ftp data timeout\r\n");
                st_device_update_info_current.pst_net_device_update_info_current = 0;
                st_device_update_info_current.b_update_type = cb_firmware_format_none;
				g_update_status = cw_system_no_update;
				ftp_updata_package_num_all = 0;
				gb_ble_updata_status = 0;
                flash_erase_sector_all_flag = 1;
            }

            if (0 == dequeue(&ftp_updata_queue, ( void * )&ftp_updata_data))
            {
                // yl_debug_printf("ASCII: \n%s\n", ftp_updata_data->data);
                // yl_debug_printf("\nlen: %d, HEX: ", ftp_updata_data->len);
                // for (uint16_t i = 0; i < ftp_updata_data->len; i++)
                // {
                //     yl_debug_printf("%02X ", ftp_updata_data->data[i]);
                // }
                if (ftp_updata_data->len > 0)
                {
                    ftp_delete_write(ftp_updata_data->data, &ftp_updata_data->len);
                    ftp_updata_data_merge_t merge_result = ftp_updata_data_merge(
                        frame_buffer, &frame_len, sizeof(frame_buffer), ftp_updata_data->data, ftp_updata_data->len);

                    if (merge_result == frame_complete)
                    {
                        xLastWakeTime = xTaskGetTickCount(); // 获取当前时间
                        // 如果拼帧完成，处理完整的帧
                        ftp_updata_data_split(frame_buffer, &frame_len);
                        if (frame_len > 0)
                        {
                            ftp_updata_analysis(frame_buffer, &frame_len);
                        }

                        frame_len = 0;    // 重置帧长度
                    }
                    else if (merge_result == frame_fail)
                    {
                        // 如果拼帧失败，清空缓冲区
                        frame_len = 0;
                    }
                }

                vPortFree(ftp_updata_data->data);
                vPortFree(ftp_updata_data);
            }
        }
        else
        {
            xLastWakeTime = xTaskGetTickCount(); // 获取当前时间
            pcakage_num_index_last = 0;
            clear_queue(&ftp_updata_queue);
            vTaskDelay(pdMS_TO_TICKS_USER(300));
        }
    }
}

void ftp_updata_task_start(void)
{
    ftp_updata_task_delete = 0;
    ftp_ready_flag = 0;
}

void ftp_updata_task_delete_func(void)
{
    if (ftp_updata_task_handle != NULL)
    {
        vTaskDelete(ftp_updata_task_handle);
        ftp_updata_task_handle = NULL;
    }

    if (ftp_flash_task_handle != NULL)
    {
        vTaskDelete(ftp_flash_task_handle);
        ftp_flash_task_handle = NULL;
    }

    if (ftp_printf_task_handle != NULL)
    {
        vTaskDelete(ftp_printf_task_handle);
        ftp_printf_task_handle = NULL;
    }
    yl_debug_printf("ftp_updata_task_delete_func\r\n");
}

uint8_t flash_erase_sector_all_flag = 1;
extern TickType_t lte_system_tick;
extern uint8_t start_one_flag;
extern uint8_t auto_disconnect_state;
static void flash_erase_sector_all_task(void *parameter)
{
    while (1)
    {
        if (1 == flash_erase_sector_all_flag)
        {
            for (uint32_t addr = cdw_ble_spi_flash_addr_alloc_local_firmware_data_start;
                 addr < cdw_ble_spi_flash_addr_alloc_local_firmware_data_end;
                 addr += 0x1000)
            {
                if (0 == (addr % 0x1000))
                {
                    drv_flash_erase_sector(addr);
                }
                vTaskDelay(pdMS_TO_TICKS_USER(50));
            }

            flash_erase_sector_all_flag = 0;
            ftp_ready_flag = 1;
            yl_debug_printf("---------------------------flash erase OK--------------------------\r\n");
        }

        // if(ftp_updata_task_delete_ok == 1)
        // {
        //     ftp_updata_task_delete_ok = 0;
        //     ftp_updata_task_delete = 0;
        //     // flash_erase_sector_all_flag = 1;
        //     // 删除任务
        //     // ftp_updata_task_delete_func();
        // }

        // 10秒钟执行一次
        // static TickType_t xLastWakeTime;
        // if (xLastWakeTime == 0)
        // {
        //     xLastWakeTime = xTaskGetTickCount();
        // }
        // if (xTaskGetTickCount() - xLastWakeTime > pdMS_TO_TICKS_USER(10000))
        // {
        //     xLastWakeTime = xTaskGetTickCount();
        //     static char pcWriteBuffer[1000] = {0};
        //     vTaskList(pcWriteBuffer);
        //     yl_debug_printf(
        //         "--------------------------------------------------------------------------------------------\r\n");
        //     yl_debug_printf(
        //         "Task Name                                                     State  Priority  Stack   Num\r\n");
        //     yl_debug_printf("%s\n", pcWriteBuffer);
        //     // 系统剩余堆栈最小大小
        //     yl_debug_printf("Free Heap Size: %d\r\n", xPortGetMinimumEverFreeHeapSize());
        // }

        if((xTaskGetTickCount() - lte_system_tick > pdMS_TO_TICKS_USER(27000)) && start_one_flag == 1 && auto_disconnect_state == 0)
		{
			// NVIC_SystemReset();
		}

        vTaskDelay(pdMS_TO_TICKS_USER(1000));
    }
}

void flash_erase_sector_all_task_start(void)
{
    BaseType_t xReturn = pdPASS;

    xReturn =
        xTaskCreate(ftp_updata_task, "ftp_updata_task", 1024, NULL, tskIDLE_PRIORITY + 5, &ftp_updata_task_handle);
    if (pdFAIL == xReturn)
    {
        yl_debug_printf("[ERROR]ftp_updata_task create err\r\n");
    }

    xReturn =
        xTaskCreate(ftp_printf_task, "ftp_printf_task", 512, NULL, tskIDLE_PRIORITY + 5, &ftp_printf_task_handle);
    if (pdFAIL == xReturn)
    {
        yl_debug_printf("[ERROR]ftp_printf_task create err\r\n");
    }

    xReturn = xTaskCreate(ftp_flash_task, "ftp_flash_task", 512, NULL, tskIDLE_PRIORITY + 5, &ftp_flash_task_handle);
    if (pdFAIL == xReturn)
    {
        yl_debug_printf("[ERROR]ftp_flash_task create err\r\n");
    }

    xReturn = xTaskCreate(flash_erase_sector_all_task,
                          "ftp_flash_erase_sector_all_task",
                          256,
                          NULL,
                          tskIDLE_PRIORITY + 3,
                          &flash_erase_sector_all_task_handle);
    if (pdFAIL == xReturn)
    {
        yl_debug_printf("[ERROR]ftp_flash_erase_sector_all_task create err\r\n");
    }
}