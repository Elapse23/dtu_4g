#include "network_local_secquence_generate.h"
#include "application_general_macro.h"

//static SemaphoreHandle_t pst_network_secquence_mutex = 0;
static uint32_t dw_network_secquence_local = 1;
static uint32_t dw_network_secquence_local_backup = 0;

//uint8_t network_secquence_semaphore_creat()
//{
//    pst_network_secquence_mutex = xSemaphoreCreateMutex();
//    if(0 == pst_network_secquence_mutex)
//    {
//
//    }
//}

uint8_t network_get_local_message_id(uint32_t *pdw_secquence)
{
    if(0 == pdw_secquence)
    {
        return FALSE;
    }

//    if(pst_network_secquence_mutex != 0)
//    {
//        xSemaphoreTake(pst_network_secquence_mutex, portMAX_DELAY);
//    }

    *pdw_secquence = dw_network_secquence_local;
    dw_network_secquence_local_backup = dw_network_secquence_local;
    dw_network_secquence_local++;

    if(dw_network_secquence_local >= (uint32_t)(~0x00))
    {
        dw_network_secquence_local = 0;
    }

//    if(pst_network_secquence_mutex != 0)
//    {
//        xSemaphoreGive(pst_network_secquence_mutex);
//    }
    return TRUE;
}

uint8_t network_check_local_message_id(uint32_t dw_secquence)
{
    if(dw_secquence == dw_network_secquence_local_backup)
    {
        return TRUE;
    }

    return FALSE;
}
