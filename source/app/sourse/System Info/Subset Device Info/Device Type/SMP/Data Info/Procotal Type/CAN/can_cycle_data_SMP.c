#include "can_cycle_data_smp.h"

#define cw_can_cycle_data_list_length       1
#define cdw_can_id_SMP                     0x00

can_cycle_data_0460_SMP st_can_cycle_data_0460_SMP = {0,};

can_cycle_data_list st_can_cycle_data_list_smp[cw_can_cycle_data_list_length] =
{
    {
        cdw_can_id_SMP,
        (uint8_t *)(&st_can_cycle_data_0460_SMP),
    },
};

#define cw_can_cycle_data_list_smp_length   1

can_cycle_data_list_info st_can_cycle_data_list_smp_info =
{
    cw_can_cycle_data_list_smp_length,
    &st_can_cycle_data_list_smp[0],
};
