#include "network_energy_statistics.h"
#include "net_device_data_kinergy_ii.h"
#include "net_device_data_ck_inverter.h"
#include "application_general_macro.h"
#include "net_system_info_access.h"
#include "net_system_info_list.h"
#include "freertos_user_config.h"
#include "portmacro.h"
#include "net_device_data_cklcd.h"
#include "net_device_data_access.h"
#include <string.h>
#include "system_timestamp_generate.h"
#include "printf_custom.h"
#include "network_device_config_sycn.h"
#include "network_eventlog_info_operate.h"
#include "network_system_type_operate.h"
#include "Eeprom.h"
#include "application_general_func.h"
#include "network_local_firmware_update.h"
#include "task.h"
#include "display_management.h"
#include "time.h"

typedef enum
{
    cb_operator_add = 0,
    cb_operator_reduce,
    cb_operator_or,
}device_data_get_operator;

#define Gen_Start_Max_Ignored_Power     0         //


TaskHandle_t energy_statistics_task_handle = 0;

typedef void (* pfunc_network_data_process)(void);

void network_ck_type_data_process_func();
void network_riiosunii_data_process_func();

uint32_t dw_power_saving;

#define cb_max_data_process_func_num        2
data_process_func_type enum_data_process_func_type = data_process_type_ck;
const pfunc_network_data_process network_data_process_func[cb_max_data_process_func_num] =
{
    &network_ck_type_data_process_func,
    &network_riiosunii_data_process_func,
};

#define cb_energy_data_num          11
static energy_data st_energy_last_hour = {0,};
const energy_data_info st_energy_data_info_last_hour[cb_energy_data_num] =
{
//    {,2,(uint8_t *)&st_energy_last_hour.w_bat_charge,     },
//    {,2,(uint8_t *)&st_energy_last_hour.w_bat_discharge,  },
    {0x0151,2,(uint8_t *)&st_energy_last_hour.w_bat_to_grid,    },
//    {,2,(uint8_t *)&st_energy_last_hour.w_bat_to_load,    },
//    {,2,(uint8_t *)&st_energy_last_hour.w_sp_total,       },
//    {,2,(uint8_t *)&st_energy_last_hour.w_sp_to_bat,      },
//    {,2,(uint8_t *)&st_energy_last_hour.w_sp_to_load,     },
//    {,2,(uint8_t *)&st_energy_last_hour.w_pv_total,       },
//    {,2,(uint8_t *)&st_energy_last_hour.w_pv_to_bat,      },
//    {,2,(uint8_t *)&st_energy_last_hour.w_pv_to_load,     },//10
    {0x0130,2,(uint8_t *)&st_energy_last_hour.w_solor_total,    },
    {0x0138,2,(uint8_t *)&st_energy_last_hour.w_solar_to_bat,   },
    {0x0142,2,(uint8_t *)&st_energy_last_hour.w_solar_to_load,  },
    {0x0140,2,(uint8_t *)&st_energy_last_hour.w_solor_to_grid,  },
//    {0x0128,2,(uint8_t *)&st_energy_last_hour.w_acin_total,     },
    {0x0134,2,(uint8_t *)&st_energy_last_hour.w_acin_to_bat,    },
    {0x0132,2,(uint8_t *)&st_energy_last_hour.w_acin_to_load,   },
    {0x0150,2,(uint8_t *)&st_energy_last_hour.w_acout_total,    },
    {0x0126,2,(uint8_t *)&st_energy_last_hour.w_acout_ideal,    },
    {0x013E,2,(uint8_t *)&st_energy_last_hour.w_acout_from_bat, },//10
//    {,2,(uint8_t *)&st_energy_last_hour.w_acout_feedback, },
    {0x0152,2,(uint8_t *)&st_energy_last_hour.w_feedback_total, },
//    {0x012D,2,(uint8_t *)&st_energy_last_hour.w_gen_total, },
};

#define cb_power_current_data_num           60
static power_current_data st_power_current_data = {0,};
const energy_data_info st_energy_data_info[cb_power_current_data_num] =
{
    {0x013B,2,(uint8_t *)&st_power_current_data.w_bat_votage             },
    {0x013C,2,(uint8_t *)&st_power_current_data.w_bat_current            },
//    {0x0168,2,(uint8_t *)&st_power_current_data.w_bat_power              },
    {0x0123,2,(uint8_t *)&st_power_current_data.w_bat_temp               },
    {0x0120,1,(uint8_t *)&st_power_current_data.b_bat_type               },
    {0x011A,2,(uint8_t *)&st_power_current_data.w_bat_soc_gather         },
    {0x011B,2,(uint8_t *)&st_power_current_data.w_bat_energy_gather      },
    {0x0102,4,(uint8_t *)&st_power_current_data.dw_mppt_output_power_total},
    {0x0103,4,(uint8_t *)&st_power_current_data.dw_mppt_output_current   },
    {0x0104,2,(uint8_t *)&st_power_current_data.w_u_acin_power_sum       },
    {0x0105,2,(uint8_t *)&st_power_current_data.w_v_acin_power_sum       },//10-1
    {0x0106,2,(uint8_t *)&st_power_current_data.w_w_acin_power_sum       },
    {0x013A,2,(uint8_t *)&st_power_current_data.w_uvw_acin_power_sum     },
    {0x0107,2,(uint8_t *)&st_power_current_data.w_u_acin_current         },
    {0x0108,2,(uint8_t *)&st_power_current_data.w_v_acin_current         },
    {0x0109,2,(uint8_t *)&st_power_current_data.w_w_acin_current         },
    {0x010D,2,(uint8_t *)&st_power_current_data.w_u_acload_power_p_sum   },			//U相acload   有功功率和
    {0x0157,2,(uint8_t *)&st_power_current_data.w_u_acload_power2_p_sum  },			//U相acload2 有功功率和
    {0x010A,2,(uint8_t *)&st_power_current_data.w_u_acload_power_s_sum   },			//U相acload   视在功率和
    {0x010E,2,(uint8_t *)&st_power_current_data.w_v_acload_power_p_sum   },
    {0x0158,2,(uint8_t *)&st_power_current_data.w_v_acload_power2_p_sum  },//10
    {0x010B,2,(uint8_t *)&st_power_current_data.w_v_acload_power_s_sum   },
    {0x010F,2,(uint8_t *)&st_power_current_data.w_w_acload_power_p_sum   },
    {0x0159,2,(uint8_t *)&st_power_current_data.w_w_acload_power2_p_sum  },
    {0x010C,2,(uint8_t *)&st_power_current_data.w_w_acload_power_s_sum   },
    {0x0110,2,(uint8_t *)&st_power_current_data.w_uvw_acload_power_p_sum },
    {0x015D,2,(uint8_t *)&st_power_current_data.w_uvw_acload_power2_p_sum},
    {0x0135,2,(uint8_t *)&st_power_current_data.w_acload_power_p_sum     },
    {0x0168,2,(uint8_t *)&st_power_current_data.w_u_acload_power_p       },			//168 = 10D + 157 + 156
    {0x0169,2,(uint8_t *)&st_power_current_data.w_v_acload_power_p       },
    {0x016A,2,(uint8_t *)&st_power_current_data.w_w_acload_power_p       },//10
    {0x0111,2,(uint8_t *)&st_power_current_data.w_u_acload_current_sum   },
    {0x0112,2,(uint8_t *)&st_power_current_data.w_v_acload_current_sum   },
    {0x0113,2,(uint8_t *)&st_power_current_data.w_w_acload_current_sum   },
    {0x0114,2,(uint8_t *)&st_power_current_data.w_u_inverter_power_sum   },
    {0x0115,2,(uint8_t *)&st_power_current_data.w_v_inverter_power_sum   },
    {0x0116,2,(uint8_t *)&st_power_current_data.w_w_inverter_power_sum   },
    {0x0117,2,(uint8_t *)&st_power_current_data.w_u_inverter_current     },
    {0x0118,2,(uint8_t *)&st_power_current_data.w_v_inverter_current     },
    {0x0119,2,(uint8_t *)&st_power_current_data.w_w_inverter_current     },
    {0x0144,4,(uint8_t *)&st_power_current_data.dw_solor_output_power    },//10
    {0x0121,1,(uint8_t *)&st_power_current_data.b_bat_soc                },
    {0x0122,1,(uint8_t *)&st_power_current_data.b_bat_soh                },
    {0x0171,2,(uint8_t *)&st_power_current_data.w_u_acout1_current       },
    {0x0172,2,(uint8_t *)&st_power_current_data.w_v_acout1_current       },
    {0x0173,2,(uint8_t *)&st_power_current_data.w_w_acout1_current       },
    {0x015A,2,(uint8_t *)&st_power_current_data.w_u_acout2_current       },
    {0x015B,2,(uint8_t *)&st_power_current_data.w_v_acout2_current       },
    {0x015C,2,(uint8_t *)&st_power_current_data.w_w_acout2_current       },
    {0x0154,2,(uint8_t *)&st_power_current_data.w_gen_power_p_sum        },
    {0x016B,2,(uint8_t *)&st_power_current_data.w_u_gen_power_p_sum      },//10
    {0x016C,2,(uint8_t *)&st_power_current_data.w_v_gen_power_p_sum      },
    {0x016D,2,(uint8_t *)&st_power_current_data.w_w_gen_power_p_sum      },
    {0x016E,2,(uint8_t *)&st_power_current_data.w_u_gen_power_i_sum      },
    {0x016F,2,(uint8_t *)&st_power_current_data.w_v_gen_power_i_sum      },
    {0x0170,2,(uint8_t *)&st_power_current_data.w_w_gen_power_i_sum      },
    {0x0174,2,(uint8_t *)&st_power_current_data.w_normalload_current     },//6
	{0x0175,2,(uint8_t *)&st_power_current_data.w_normalload_L1_current	 },//6
	{0x0176,2,(uint8_t *)&st_power_current_data.w_normalload_L2_current	 },//6
	{0x0177,2,(uint8_t *)&st_power_current_data.w_normalload_L1_power    },//6
    {0x0178,2,(uint8_t *)&st_power_current_data.w_normalload_L2_power    },//6

    
};

#define cb_max_pv_data_summary_num                  7
static pv_data_summary st_pv_data_summary = {0,};
const energy_data_info st_pv_data_summary_list[cb_max_pv_data_summary_num] =
{
    {0x0161,2,(uint8_t *)&st_pv_data_summary.w_pv_power_p_total},
    {0x0162,2,(uint8_t *)&st_pv_data_summary.w_pv_power_p_u    },
    {0x0163,2,(uint8_t *)&st_pv_data_summary.w_pv_power_p_v    },
    {0x0164,2,(uint8_t *)&st_pv_data_summary.w_pv_power_p_w    },
    {0x0165,2,(uint8_t *)&st_pv_data_summary.w_pv_max_votage_u },
    {0x0166,2,(uint8_t *)&st_pv_data_summary.w_pv_max_votage_v },
    {0x0167,2,(uint8_t *)&st_pv_data_summary.w_pv_max_votage_w },
};

uint8_t network_data_process_func_set(data_process_func_type enum_data_procee_func_in)
{
    switch(enum_data_procee_func_in)
    {
        case data_process_type_ck:
        case data_process_type_riiosunii:
            break;
        default:
            return FALSE;
    }
    DebugPrintf(TRUE,"Energy Statistics Enum: %d\n",enum_data_procee_func_in);
    enum_data_process_func_type = enum_data_procee_func_in;
    return TRUE;
}
uint8_t get_ck_inverter_data(uint8_t b_start_index,
                             uint8_t b_num,
                             uint16_t w_net_addr,
                             uint16_t *pw_data_out)
{
    net_device_info *pst_net_device_info = 0;
    uint16_t w_data_temp = 0;
    if((b_start_index + b_num) > 9)
    {
        return FALSE;
    }
    *pw_data_out = 0;
    for(uint8_t b_cnt_1 = b_start_index + cw_device_id_index_ck_inverter_u1;
        b_cnt_1 < b_start_index + b_num + cw_device_id_index_ck_inverter_u1;
        b_cnt_1++)
    {
        if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[b_cnt_1],
                                        (net_device_info **)&pst_net_device_info,
                                        &st_net_system_info_list))
        {
            return FALSE;
        }
        if(cb_net_device_add == pst_net_device_info->pst_net_device_status->b_communicate_status)
        {
            uint16_t w_data_len = 0;
            net_device_data_access(dwa_device_id_kinergy_ii[b_cnt_1],
                                   cb_net_data_att_status,
                                   w_net_addr,
                                   &w_data_len,
                                   (uint8_t *)&w_data_temp,
                                   cb_device_data_access_operate_type_read);
//            if(cb_operator_or == b_operator)
//            {
//                *pw_data_out |= w_data_temp;
//            }
//            else// if(cb_operator_add == b_operator)
            {
                *pw_data_out += w_data_temp;
            }
        }
    }
    return TRUE;
}
uint8_t get_ck_inverter_data_4_bytes(uint8_t b_start_index,
                                     uint8_t b_num,
                                     uint16_t w_net_addr,
                                     uint32_t *pdw_data_out,
                                     uint8_t f_sign)
{
    net_device_info *pst_net_device_info = 0;
    uint16_t w_data_temp = 0;
    if((b_start_index + b_num) > 9)
    {
        return FALSE;
    }
    *pdw_data_out = 0;
    for(uint8_t b_cnt_1 = b_start_index + cw_device_id_index_ck_inverter_u1;
        b_cnt_1 < b_start_index + b_num + cw_device_id_index_ck_inverter_u1;
        b_cnt_1++)
    {
        if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[b_cnt_1],
                                        (net_device_info **)&pst_net_device_info,
                                        &st_net_system_info_list))
        {
            return FALSE;
        }
        if(cb_net_device_add == pst_net_device_info->pst_net_device_status->b_communicate_status)
        {
            uint16_t w_data_len = 0;
            net_device_data_access(dwa_device_id_kinergy_ii[b_cnt_1],
                                   cb_net_data_att_status,
                                   w_net_addr,
                                   &w_data_len,
                                   (uint8_t *)&w_data_temp,
                                   cb_device_data_access_operate_type_read);
//            if(cb_operator_or == b_operator)
//            {
//                *pdw_data_out |= w_data_temp;
//            }
//            else// if(cb_operator_add == b_operator)
            if((0x8000 == (0x8000 & w_data_temp)) &&\
               (TRUE == f_sign))
            {
                w_data_temp = ~w_data_temp + 1;
                *pdw_data_out -= w_data_temp;
            }
            else
            {
                *pdw_data_out += w_data_temp;
            }
        }
    }
    return TRUE;
}

uint8_t get_mppt_data(uint16_t w_net_addr,uint16_t *pw_data_in)
{
    net_device_info *pst_net_device_info = 0;
    uint16_t w_data_temp = 0;

    *pw_data_in = 0;

    for(uint8_t b_cnt_1 = cw_device_id_index_mppt_u1_0;
        b_cnt_1 <= cw_device_id_index_mppt_w3_5;
        b_cnt_1++)
    {
        if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[b_cnt_1],
                                        (net_device_info **)&pst_net_device_info,
                                        &st_net_system_info_list))
        {
            return FALSE;
        }
        if(cb_net_device_add == pst_net_device_info->pst_net_device_status->b_communicate_status)
        {
            uint16_t w_data_len = 0;
            net_device_data_access(dwa_device_id_kinergy_ii[b_cnt_1],
                                   cb_net_data_att_status,
                                   w_net_addr,
                                   &w_data_len,
                                   (uint8_t *)&w_data_temp,
                                   cb_device_data_access_operate_type_read);
            *pw_data_in += w_data_temp;
        }
    }
    return TRUE;
}
uint8_t get_mppt_data_4_bytes(uint16_t w_net_addr,uint32_t *pdw_data_in)
{
    net_device_info *pst_net_device_info = 0;
    uint32_t dw_data_temp = 0;

    *pdw_data_in = 0;

    for(uint8_t b_cnt_1 = cw_device_id_index_mppt_u1_0;
        b_cnt_1 <= cw_device_id_index_mppt_w3_5;
        b_cnt_1++)
    {
        if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[b_cnt_1],
                                        (net_device_info **)&pst_net_device_info,
                                        &st_net_system_info_list))
        {
            return FALSE;
        }
        if(cb_net_device_add == pst_net_device_info->pst_net_device_status->b_communicate_status)
        {
            uint16_t w_data_len = 0;
            net_device_data_access(dwa_device_id_kinergy_ii[b_cnt_1],
                                   cb_net_data_att_status,
                                   w_net_addr,
                                   &w_data_len,
                                   (uint8_t *)&dw_data_temp,
                                   cb_device_data_access_operate_type_read);
            *pdw_data_in += dw_data_temp;
        }
    }
    return TRUE;
}
uint8_t get_pv_data(uint16_t w_net_addr,uint16_t *pw_data_in)
{
    net_device_info *pst_net_device_info = 0;
    uint16_t w_data_temp = 0;
    *pw_data_in = 0;
    for(uint8_t b_cnt_1 = cw_device_id_index_pv_1;
        b_cnt_1 <= cw_device_id_index_pv_2;
        b_cnt_1++)
    {
        if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[b_cnt_1],
                                        (net_device_info **)&pst_net_device_info,
                                        &st_net_system_info_list))
        {
            return FALSE;
        }



        if(cb_net_device_add == pst_net_device_info->pst_net_device_status->b_communicate_status)
        {

                uint16_t w_data_len = 0;
                net_device_data_access(dwa_device_id_kinergy_ii[b_cnt_1],
                                       cb_net_data_att_status,
                                       w_net_addr,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp,
                                       cb_device_data_access_operate_type_read);
                if(w_data_len != 0)
                *pw_data_in += w_data_temp;

        }
    }
    return TRUE;
}

uint8_t net_write_energy_data(uint8_t b_start_index,
                              uint8_t b_num,
                              energy_data_info *pst_energy_data_info)
{
    for(uint8_t b_cnt_i = b_start_index;b_cnt_i < b_start_index + b_num;b_cnt_i++)
    {
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               pst_energy_data_info[b_cnt_i].w_network_addr,
                               (uint16_t *)&pst_energy_data_info[b_cnt_i].w_data_len,
                               pst_energy_data_info[b_cnt_i].pb_data,
                               cb_device_data_access_operate_type_write);
    }
    return TRUE;
}

uint16_t negative_to_zero(uint16_t w_data_in)
{
    if(0x8000 == (0x8000 & w_data_in))
    {
        return 0;
    }
    return w_data_in;
}
uint32_t negative_to_zero_u32(uint32_t dw_data_in)
{
    if(0x80000000 == (0x80000000 & dw_data_in))
    {
        return 0;
    }
    return dw_data_in;
}

uint16_t negtive_to_positive(uint16_t w_data_in)
{
    if(0x8000 == (0x8000 & w_data_in))
    {
        return ~w_data_in + 1;
    }
    return w_data_in;

}
uint16_t positive_to_negtive(uint16_t w_data_in)
{
    if(0x0000 == (0x8000 & w_data_in))
    {
        return ~w_data_in + 1;
    }
    return w_data_in;

}

void network_ck_type_energy_statistics()
{
    net_device_info *pst_net_device_info = 0;
    const net_data_info *pst_data_info = 0;
    uint8_t b_system_parallel = stand_alone;
    uint32_t dw_timestamp = 0;
    static uint32_t dw_timestamp_backup = 0;

#if( task_runing_status_print )
    DebugPrintf(TRUE,"Ck_energy_statisics_start\r\n");
#endif

    get_system_timestamp(&dw_timestamp);

    if((dw_timestamp != 0) &&\
       (((dw_timestamp % (3600 * 24)) > (3600 * 24 - 10)) ||\
        ((dw_timestamp % (3600 * 24)) < (10))))
    {
        DebugPrintf(TRUE,"Hour data clear,timestamp:%d\n",dw_timestamp);
        dw_timestamp_backup = dw_timestamp;
        memset(&st_energy_last_hour,0,sizeof(energy_data));
        net_write_energy_data(0,cb_energy_data_num,(energy_data_info *)&st_energy_data_info_last_hour[0]);
    }
    {
        uint16_t w_acin_sourse = 0;
        uint16_t w_data_len = 2;
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                               cb_device_data_access_data_type_set,
                               0x005C,
                               &w_data_len,
                               (uint8_t *)&w_acin_sourse,
                               cb_device_data_access_operate_type_read);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x014B,
                               &w_data_len,
                               (uint8_t *)&w_acin_sourse,
                               cb_device_data_access_operate_type_write);
    }
    //E_acout
    uint16_t w_acout_consume = 0;
    uint16_t w_acout_feedback = 0;
    uint16_t w_pv_today_realtime = 0;
    uint16_t w_acout_today_realtime = 0;

    get_ck_inverter_data(0,9,0x013F,&w_acout_consume);
    get_ck_inverter_data(0,9,0x0141,&w_acout_feedback);
    get_pv_data(0x0102,&w_pv_today_realtime);

    w_acout_today_realtime = w_acout_consume  + w_pv_today_realtime - w_acout_feedback;

    //E_acin
    uint16_t w_acin_today_realtime = 0;
    get_ck_inverter_data(0,9,0x0129,&w_acin_today_realtime);

    //E_feedback
    uint16_t w_feedback_today_realtime = 0;
    get_ck_inverter_data(0,9,0x012B,&w_feedback_today_realtime);

    //E_bat
    uint8_t f_independant_batpack = FALSE;
    uint16_t w_bat_charge_today_realtime = 0;
    uint16_t w_bat_discharge_today_realtime = 0;
    if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                    (net_device_info **)&pst_net_device_info,
                                    &st_net_system_info_list))
    {
        #if( system_alarm_print )
        DebugPrintf(TRUE,"[Alarm]Ck Energy Error :1\r\n");
        #endif

        return;
    }
    if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                         0x0020,
                                         (const net_data_info **)&pst_data_info,
                                         pst_net_device_info))
    {
        #if( system_alarm_print )
        DebugPrintf(TRUE,"[Alarm]Ck Energy Error :2\r\n");
        #endif

        return;
    }
    b_system_parallel = *pst_data_info->pb_data;
    if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                         0x0023,
                                         (const net_data_info **)&pst_data_info,
                                         pst_net_device_info))
    {
        #if( system_alarm_print )
        DebugPrintf(TRUE,"[Alarm]Ck Energy Error :3\r\n");
        #endif

        return;
    }
    f_independant_batpack = *pst_data_info->pb_data;
    if(((one_phase_parallel == b_system_parallel) && f_independant_batpack) ||\
       ((three_phase_parallel == b_system_parallel) && f_independant_batpack))
    {
        get_ck_inverter_data(0,3,0x012F,&w_bat_charge_today_realtime);
        get_ck_inverter_data(0,3,0x012D,&w_bat_discharge_today_realtime);
    }
    else if(((one_phase_parallel == b_system_parallel) && !f_independant_batpack) ||\
            ((three_phase_parallel == b_system_parallel) && !f_independant_batpack) ||\
            (stand_alone == b_system_parallel))
    {
        get_ck_inverter_data(0,1,0x012F,&w_bat_charge_today_realtime);
        get_ck_inverter_data(0,1,0x012D,&w_bat_discharge_today_realtime);
    }
    //E_mppt
    uint16_t w_mppt_today_realtime = 0;
    uint16_t w_solor_today_realtime = 0;

    get_mppt_data(0x010C,&w_mppt_today_realtime);
    //solor
    w_solor_today_realtime = w_mppt_today_realtime + w_pv_today_realtime;

    {//write data
        uint16_t w_data_len = 2;
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x0146,                                                  //EAcOutTodayTotalRealTime
                               &w_data_len,
                               (uint8_t *)&w_acout_today_realtime,
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x015B,                                                  //I_acout2 L2
                               &w_data_len,
                               (uint8_t *)&w_pv_today_realtime,
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x0147,                                                  //EAcInTodayRealTime
                               &w_data_len,
                               (uint8_t *)&w_acin_today_realtime,
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x0148,                                                  //ESolarTodayTotalRealTime
                               &w_data_len,
                               (uint8_t *)&w_solor_today_realtime,
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x012A,                                                  //EFeedBackTodayRealTime
                               &w_data_len,
                               (uint8_t *)&w_feedback_today_realtime,
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x012C,                                                  //EBatChargeTodayRealTime
                               &w_data_len,
                               (uint8_t *)&w_bat_charge_today_realtime,
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x012E,                                                  //EBatDischargeTodayRealTime
                               &w_data_len,
                               (uint8_t *)&w_bat_discharge_today_realtime,
                               cb_device_data_access_operate_type_write);
//        w_data_len = 1;
//        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
//                               cb_device_data_access_data_type_status,
//                               0x011C,                                                  //系统单机、并机配置情况
//                               &w_data_len,
//                               (uint8_t *)&b_system_parallel,
//                               cb_device_data_access_operate_type_write);
    }
    if(((((dw_timestamp % 3600) - 3540) < 10) &&\
        (dw_timestamp_backup - dw_timestamp) > 10) ||\
       (0 == dw_timestamp_backup))
    {
        dw_timestamp_backup = dw_timestamp;
        uint32_t dw_data_temp = 0;
        energy_data st_energy_growth = {0,};

        //bat
        st_energy_growth.w_bat_charge =\
            negative_to_zero(w_bat_charge_today_realtime - st_energy_last_hour.w_bat_charge);
        st_energy_last_hour.w_bat_charge = w_bat_charge_today_realtime;

        st_energy_growth.w_bat_discharge =\
            negative_to_zero(w_bat_discharge_today_realtime - st_energy_last_hour.w_bat_discharge);
        st_energy_last_hour.w_bat_discharge = w_bat_discharge_today_realtime;

        //mppt
        st_energy_growth.w_sp_total =\
            negative_to_zero(w_mppt_today_realtime - st_energy_last_hour.w_sp_total);
        st_energy_last_hour.w_sp_total = w_mppt_today_realtime;

        //pv
        st_energy_growth.w_pv_total =\
            negative_to_zero(w_pv_today_realtime - st_energy_last_hour.w_pv_total);
        st_energy_last_hour.w_pv_total = w_pv_today_realtime;

        //solor
        st_energy_growth.w_solor_total =\
            negative_to_zero(w_solor_today_realtime - st_energy_last_hour.w_solor_total);
        st_energy_last_hour.w_solor_total = w_solor_today_realtime;

        //acin
        st_energy_growth.w_acin_total =\
            negative_to_zero(w_acin_today_realtime - st_energy_last_hour.w_acin_total);
        st_energy_last_hour.w_acin_total = w_acin_today_realtime;

        //acout
        st_energy_growth.w_acout_total =\
            negative_to_zero(w_acout_today_realtime - st_energy_last_hour.w_acout_total);
        st_energy_last_hour.w_acout_total = w_acout_today_realtime;

        //acout feedback
        st_energy_growth.w_acout_feedback =\
            negative_to_zero(w_acout_feedback - st_energy_last_hour.w_acout_feedback);
        st_energy_last_hour.w_acout_feedback = w_acout_feedback;

        //feedback
        st_energy_growth.w_feedback_total =\
            negative_to_zero(w_feedback_today_realtime - st_energy_last_hour.w_feedback_total);
        st_energy_last_hour.w_feedback_total = w_feedback_today_realtime;

        {//w_sp_to_bat_today   //   w_sp_to_load_today
            if(0 != w_mppt_today_realtime)
            {
                dw_data_temp = negative_to_zero_u32(st_energy_growth.w_sp_total - st_energy_growth.w_bat_discharge);
                st_energy_last_hour.w_sp_to_bat += dw_data_temp;

                if(0 == w_pv_today_realtime)
                {
                    if(0 == dw_data_temp)
                    {
                        st_energy_last_hour.w_sp_to_load += st_energy_growth.w_sp_total;
                    }
                    else
                    {
                        st_energy_last_hour.w_sp_to_load += st_energy_growth.w_bat_discharge;
                    }
                }
                else
                {
                    st_energy_last_hour.w_sp_to_load +=\
                        negative_to_zero(st_energy_growth.w_sp_total - dw_data_temp);
                }
            }
            else
            {
                st_energy_last_hour.w_sp_to_bat = 0;
                st_energy_last_hour.w_sp_to_load = 0;
            }
        }
        {//pv2bat pv2load
            if(0 != w_pv_today_realtime)
            {
                st_energy_growth.w_pv_to_bat =\
                    negative_to_zero(st_energy_growth.w_acout_feedback - st_energy_growth.w_feedback_total);
                if(st_energy_growth.w_pv_to_bat > st_energy_growth.w_bat_charge)
                {
                    st_energy_growth.w_pv_to_bat = st_energy_growth.w_bat_charge;
                    st_energy_growth.w_acout_total += (st_energy_growth.w_pv_to_bat - st_energy_growth.w_bat_charge);
                    st_energy_last_hour.w_acout_total += st_energy_growth.w_acout_total;
                }
                st_energy_growth.w_pv_to_load =\
                    negative_to_zero(st_energy_growth.w_pv_total -\
                                     st_energy_growth.w_pv_to_bat -\
                                     st_energy_growth.w_feedback_total);
                st_energy_last_hour.w_pv_to_bat += st_energy_growth.w_pv_to_bat;
                st_energy_last_hour.w_pv_to_load += st_energy_growth.w_pv_to_load;
            }
            else
            {
                st_energy_last_hour.w_pv_to_bat = 0;
                st_energy_last_hour.w_pv_to_load = 0;
            }
        }

        {//solor2grid
            if(0 == st_energy_growth.w_solor_total)
            {
                st_energy_last_hour.w_bat_to_grid += st_energy_growth.w_feedback_total;
            }
            else
            {
                st_energy_last_hour.w_solor_to_grid += st_energy_growth.w_feedback_total;
            }
        }
        {//solor2bat
            dw_data_temp = st_energy_last_hour.w_sp_to_bat + st_energy_last_hour.w_pv_to_bat;
            if(0 == dw_data_temp)
            {
                st_energy_last_hour.w_solar_to_bat = 0;
            }
            else
            {
                st_energy_growth.w_solar_to_bat =\
                    negative_to_zero(dw_data_temp - st_energy_last_hour.w_solar_to_bat);
                st_energy_last_hour.w_solar_to_bat = negative_to_zero(dw_data_temp);
            }
        }
        {//solor2load
            dw_data_temp = st_energy_last_hour.w_sp_to_load + st_energy_last_hour.w_pv_to_load;
            if(0 == dw_data_temp)
            {
                st_energy_last_hour.w_solar_to_load = 0;
            }
            else
            {
                st_energy_growth.w_solar_to_load =\
                    negative_to_zero(dw_data_temp - st_energy_last_hour.w_solar_to_load);
                st_energy_last_hour.w_solar_to_load = negative_to_zero(dw_data_temp);
            }
        }
        {//acin
            if(0 == st_energy_last_hour.w_acin_total)
            {
                st_energy_last_hour.w_acin_to_bat = 0;
                st_energy_last_hour.w_acin_to_load = 0;
            }
            else
            {
                if(0 != st_energy_growth.w_acin_total)
                {
                    st_energy_growth.w_acin_to_bat =\
                        negative_to_zero(st_energy_growth.w_bat_charge - st_energy_growth.w_pv_to_bat);
                    if(st_energy_growth.w_acin_total >= st_energy_growth.w_acin_to_bat)
                    {
                        st_energy_last_hour.w_acin_to_bat += st_energy_growth.w_acin_to_bat;
                    }
                    else
                    {
                        st_energy_growth.w_acin_to_bat = st_energy_growth.w_acin_total;
                        st_energy_last_hour.w_acin_to_bat += st_energy_growth.w_acin_to_bat;
                    }
                    st_energy_growth.w_acin_to_load =\
                        negative_to_zero(st_energy_growth.w_acin_total - st_energy_growth.w_acin_to_bat);
                    st_energy_last_hour.w_acin_to_load += st_energy_growth.w_acin_to_load;
                }
                else
                {
                    st_energy_growth.w_acin_to_bat = 0;
                    st_energy_growth.w_acin_to_load = 0;
                }
            }
        }
        {//acout
            if((0 == st_energy_last_hour.w_acout_total) &&\
               (0 == st_energy_last_hour.w_pv_total))
            {
                st_energy_last_hour.w_acout_from_bat = 0;
            }
            else
            {
                st_energy_growth.w_acout_from_bat =\
                    negative_to_zero(st_energy_growth.w_acout_total -\
                                     st_energy_growth.w_solar_to_load -\
                                     st_energy_growth.w_acin_to_load);
                st_energy_last_hour.w_acout_from_bat += st_energy_growth.w_acout_from_bat;
            }
            st_energy_last_hour.w_acout_ideal +=\
                st_energy_growth.w_acout_from_bat +\
                st_energy_growth.w_solar_to_load +\
                st_energy_growth.w_acin_to_load;
        }
        net_write_energy_data(0,cb_energy_data_num,(energy_data_info *)&st_energy_data_info_last_hour[0]);
//        DebugPrintf(TRUE,"Hour data statistics compete,timestamp:%d\n",dw_timestamp);
    }
#if( task_runing_status_print )
    DebugPrintf(TRUE,"Ck_energy_statisics_end\r\n");
#endif
}

energy_data st_energy_growth = {0,};
extern uint16_t g_parallel_system_mode;
void network_riiosunii_energy_statistics()
{
//    uint16_t w_system_parallel = stand_alone;
    uint32_t dw_timestamp = 0;									//1个dw_timestamp等于1s
    static uint32_t dw_timestamp_backup = 0;
    static uint16_t w_meter_comsume_backup = 0;
    static uint16_t w_meter_feedback_backup = 0;
    static uint16_t w_normal_load_consumption = 0;				//0155
    static uint16_t w_normal_load_consumption_back_up = 0;				//0155
	static uint16_t w_grid_consume_today_realtime = 0;			//0145
    static uint16_t w_gird_feedback_today_realtime = 0;			//012A

	
#if( task_runing_status_print )
    DebugPrintf(TRUE,"Riiosunii_energy_statisics_start\r\n");
#endif
    get_system_timestamp(&dw_timestamp);

    uint16_t b_meter_enable = 0;
    uint16_t b_parallel_system_mode = 0;

	if(check_system_type(cb_inv_type_riio_sun_ii) 
		|| check_system_type(cb_inv_type_riio_sun_ii_gte) 
		|| check_system_type(cb_inv_type_riio_sun_ii_split_phase) 
		|| check_system_type(cb_inv_type_riio_ii_split_phase)
		|| check_system_type(cb_inv_type_riio_ii))
	{
		uint16_t w_data_len = 2;
//    	net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
//		                       cb_device_data_access_data_type_set,
//		                       0x0020,
//		                       &w_data_len,
//		                       (uint8_t *)&b_parallel_system_mode,                                       //获取系统单并机模式
//		                       cb_device_data_access_operate_type_read);

		if(g_parallel_system_mode == 0)
		{

			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
					   cb_net_data_att_status,
					   0x0131,
					   &w_data_len,
					   (uint8_t *)&b_meter_enable,					//电表功能 Meter_Enable 0：关闭电表功能 1：打开电表功能
					   cb_device_data_access_operate_type_read);//merter enable
		}
	}
	
    uint16_t b_ct_ctrl_mode = 0;
	uint16_t b_ct_iac_ext = 0;
    {
        uint16_t w_data_len = 2;
		if(FALSE == check_system_type(cb_inv_type_riio_sun_ii) 
			&& FALSE == check_system_type(cb_inv_type_riio_sun_ii_gte) 
			&& FALSE == check_system_type(cb_inv_type_riio_sun_ii_split_phase) 
			&& FALSE == check_system_type(cb_inv_type_riio_ii_split_phase)
			&& FALSE == check_system_type(cb_inv_type_riio_ii))
		{
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
								   cb_device_data_access_data_type_set,
								   0x0034,														//b_ct_ctrl_mode 	0-Display Only 1-Zero Export to 2-AC Cur Limit 3-Feedback Ctrl
								   &w_data_len,
								   (uint8_t *)&b_ct_ctrl_mode,
								   cb_device_data_access_operate_type_read);
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
								   cb_device_data_access_data_type_set,
								   0x0033,														//b_ct_ctrl_mode 	0-Display Only 1-Zero Export to 2-AC Cur Limit 3-Feedback Ctrl
								   &w_data_len,
								   (uint8_t *)&b_ct_iac_ext,
								   cb_device_data_access_operate_type_read);

		}
    }
	{		//将freertos最小剩余堆栈记录到017B中
		uint16_t w_min_free_byte_remaining = 0;
		uint16_t w_data_len = 2;
		w_min_free_byte_remaining = xPortGetMinimumEverFreeHeapSize();
		net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
							   cb_device_data_access_data_type_status,
							   0x017B,
							   &w_data_len,
							   (uint8_t *)&w_min_free_byte_remaining,
							   cb_device_data_access_operate_type_write);	 
    }
    if((dw_timestamp != 0) &&\
       (((dw_timestamp % (3600 * 24)) > (3600 * 24 - 10)) ||\
        ((dw_timestamp % (3600 * 24)) < (10))))								//每天能量统计置零
    {
        DebugPrintf(TRUE,"Hour data clear,timestamp:%d\n",dw_timestamp);
        dw_timestamp_backup = dw_timestamp;
        memset(&st_energy_last_hour,0,sizeof(energy_data));
//        w_normal_load_backup = 0;
        w_grid_consume_today_realtime = 0;
        w_gird_feedback_today_realtime = 0;
		w_normal_load_consumption = 0;
        net_write_energy_data(0,cb_energy_data_num,(energy_data_info *)&st_energy_data_info_last_hour[0]);
    }
    uint16_t w_acin_sourse = 0;
	uint16_t w_smart_port_source = 0;
    uint16_t w_data_len = 2;
    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                           cb_device_data_access_data_type_set,
                           0x005C,                                                      //ACin_Source_SEL 0-Grid(Default); 1-Generator
                           &w_data_len,
                           (uint8_t *)&w_acin_sourse,
                           cb_device_data_access_operate_type_read);
    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                           cb_device_data_access_data_type_status,
                           0x014B,                                                      //AcIn_Source_Select     
                           &w_data_len,
                           (uint8_t *)&w_acin_sourse,
                           cb_device_data_access_operate_type_write);//acin sourse
    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                           cb_device_data_access_data_type_set,
                           0x001F,                                                      //smart port : 0:Smart ACout2,  1:mart GEN ,2:City GEN
                           &w_data_len,													//机型为Tyrann时 1:GEN ,2:City GEN
                           (uint8_t *)&w_smart_port_source,
                           cb_device_data_access_operate_type_read);

    //E_acout1 E_acout2
    uint16_t w_acout_consume_1 = 0;
    uint16_t w_acout_consume_2 = 0;
    uint16_t w_acout_feedback_1 = 0;
    uint16_t w_acout_feedback_2 = 0;	
	uint16_t w_acout_today_realtime = 0;		//0146
    uint16_t w_acout2_today_realtime = 0;		//012f
    uint16_t w_pv_today_realtime = 0;

    get_ck_inverter_data(0,9,0x0141,&w_acout_consume_1);
    get_ck_inverter_data(0,9,0x0145,&w_acout_consume_2);
    get_ck_inverter_data(0,9,0x0143,&w_acout_feedback_1);
    get_ck_inverter_data(0,9,0x0147,&w_acout_feedback_2);

    get_pv_data(0x0102,&w_pv_today_realtime);

    w_acout_today_realtime = negative_to_zero(w_acout_consume_1 + w_pv_today_realtime -\
                                              w_acout_feedback_1);
    w_acout2_today_realtime = negative_to_zero(w_acout_consume_2 - w_acout_feedback_2);

    //E_acin
    uint16_t w_acin_today_realtime = 0;
	uint16_t w_acin_today_realtime_true = 0;
    get_ck_inverter_data(0,9,0x013B,&w_acin_today_realtime);
	w_acin_today_realtime_true = w_acin_today_realtime;

    //E_acin_gen
    uint16_t w_acin_gen_today_realtime = 0;
    get_ck_inverter_data(0,9,0x014F,&w_acin_gen_today_realtime);

    //Riio_SUN_ii:Smart_E_GEN  Tyrann:ACin2
    uint16_t w_gen_today_realtime = 0;
    get_ck_inverter_data(0,9,0x013F,&w_gen_today_realtime);

    //E_feedback
    uint16_t w_acin_feedback_today_realtime = 0;
    get_ck_inverter_data(0,9,0x013D,&w_acin_feedback_today_realtime);

	{
		uint16_t w_sys_condition_bit = 0;
        uint16_t w_data_len = 0;
		net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
	                               cb_device_data_access_data_type_status,
	                               0x0120,
	                               &w_data_len,
	                               (uint8_t *)&w_sys_condition_bit,
	                               cb_device_data_access_operate_type_read);
		if(check_system_type(cb_inv_type_tyrann) || check_system_type(cb_inv_type_apollo_matric_ii_10_15k) && (0 == w_acin_sourse) && (1 != (w_sys_condition_bit & 0x0001)) && (1 == (w_sys_condition_bit & 0x0100)))		
		{
			w_acin_today_realtime = w_gen_today_realtime;		//机型为tyrann且acin_source为市电但是未接市电时 并且发电机接入
		}
    }
	
    //E_bat
    uint16_t f_independant_batpack = FALSE;
    uint16_t w_bat_charge_today_realtime = 0;			//012C
    uint16_t w_bat_discharge_today_realtime = 0;		//012E
    {
        uint16_t w_data_len = 2;
//        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
//                               cb_device_data_access_data_type_set,
//                               0x0020,
//                               &w_data_len,
//                               (uint8_t *)&w_system_parallel,
//                               cb_device_data_access_operate_type_read);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                               cb_device_data_access_data_type_set,
                               0x0023,
                               &w_data_len,
                               (uint8_t *)&f_independant_batpack,
                               cb_device_data_access_operate_type_read);//bat independant
    }

//	if(check_system_type(cb_inv_type_ck_ii) || check_system_type(cb_inv_type_tyrann) || check_system_type(cb_inv_type_riio_sun_ii_split_phase))
//	{
        get_ck_inverter_data(0,9,0x0139,&w_bat_charge_today_realtime);
        get_ck_inverter_data(0,9,0x0137,&w_bat_discharge_today_realtime);
//	}
//	else
//	{
//		if(((one_phase_parallel == w_system_parallel) && f_independant_batpack) ||\
//	       ((three_phase_parallel == w_system_parallel) && f_independant_batpack))
//	    {
//	        get_ck_inverter_data(0,3,0x0139,&w_bat_charge_today_realtime);
//	        get_ck_inverter_data(0,3,0x0137,&w_bat_discharge_today_realtime);
//	    }
//	    else if(((one_phase_parallel == w_system_parallel) && !f_independant_batpack) ||\
//	            ((three_phase_parallel == w_system_parallel) && !f_independant_batpack) ||\
//	            (stand_alone == w_system_parallel))
//	    {
//	        get_ck_inverter_data(0,1,0x0139,&w_bat_charge_today_realtime);
//	        get_ck_inverter_data(0,1,0x0137,&w_bat_discharge_today_realtime);
//	    }
//	}

    //E_mppt
    uint16_t w_mppt_today_realtime = 0;
    uint16_t w_solor_today_realtime = 0;

    get_mppt_data(0x010E,&w_mppt_today_realtime);
    //solor
    w_solor_today_realtime = w_mppt_today_realtime + w_pv_today_realtime;

    //acin port
    //E_Acin + E_Gen
    uint16_t w_acin_port_today_realtime = 0;		//0147

    w_acin_port_today_realtime = w_gen_today_realtime + w_acin_gen_today_realtime;	// acin端gen+grid 和ac out2 gen 这里计算两个gen处的能量，acin grid在下面分析电表数据时加

    //normal load consumption // grid
    if(FALSE == check_system_type(cb_inv_type_riio_sun_ii) 
		&& FALSE == check_system_type(cb_inv_type_riio_sun_ii_gte) 
		&& FALSE == check_system_type(cb_inv_type_riio_sun_ii_split_phase) 
		&& FALSE == check_system_type(cb_inv_type_riio_ii_split_phase)
		&& FALSE == check_system_type(cb_inv_type_riio_ii))		//Tyrann和CK II使用外部CT
    {
        uint16_t w_data_len = 2;

		uint16_t w_meter_to_sys_energy = 0;
		uint16_t w_meter_from_sys_energy = 0;
		uint16_t w_ct_power = 0;
		static uint8_t b_change_meter_mode_flag = 0;
		static uint8_t w_normal_load_consumption_compensation = 0;
        if(b_ct_iac_ext != 0)//外部ct值为3即馈网
        {

		   net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
								  cb_device_data_access_data_type_status,
								  0x011D,
								  &w_data_len,
								  (uint8_t *)&w_ct_power,						//ct_w
								  cb_device_data_access_operate_type_read);
		   get_ck_inverter_data(0,9,0x0151,&w_meter_to_sys_energy);
		   get_ck_inverter_data(0,9,0x0153,&w_meter_from_sys_energy);
//	   		net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
//								   cb_device_data_access_data_type_status,
//								   0x0151,															//正向有功电量
//								   &w_data_len,
//								   (uint8_t *)&w_meter_to_sys_energy,
//								   cb_device_data_access_operate_type_read);
//			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
//								   cb_device_data_access_data_type_status,
//								   0x0153,															//反向有功电量
//								   &w_data_len,
//								   (uint8_t *)&w_meter_from_sys_energy,
//								   cb_device_data_access_operate_type_read);

			if(b_change_meter_mode_flag == 1)
			{
				b_change_meter_mode_flag = 0;
				w_normal_load_consumption_compensation = w_normal_load_consumption_back_up;
			}
			else
			{
				w_normal_load_consumption_compensation = 0;
			}
			w_normal_load_consumption = negative_to_zero(w_acin_feedback_today_realtime - w_gird_feedback_today_realtime + 
														 w_grid_consume_today_realtime - w_acin_today_realtime_true + w_normal_load_consumption_compensation); //前端载 = （逆变器端馈网 - 电表端馈网增量）+（电表端输入增量 - 逆变器端输入）
			w_normal_load_consumption_back_up = w_normal_load_consumption;
			if((0 != w_meter_comsume_backup) || (0 != w_meter_feedback_backup))		
			{
	            w_grid_consume_today_realtime += negative_to_zero(w_meter_to_sys_energy - w_meter_comsume_backup);
	            w_gird_feedback_today_realtime += negative_to_zero(w_meter_from_sys_energy -  w_meter_feedback_backup);
			}
            w_meter_comsume_backup = w_meter_to_sys_energy;
            w_meter_feedback_backup = w_meter_from_sys_energy;
        }
        else
        {
        	b_change_meter_mode_flag = 1;
            w_grid_consume_today_realtime = w_acin_today_realtime_true;
            w_gird_feedback_today_realtime = w_acin_feedback_today_realtime;
        }
        w_acin_port_today_realtime += w_grid_consume_today_realtime;
    }
	else if(check_system_type(cb_inv_type_riio_sun_ii_split_phase) 
		|| check_system_type(cb_inv_type_riio_sun_ii) 
		|| check_system_type(cb_inv_type_riio_sun_ii_gte) 
		|| check_system_type(cb_inv_type_riio_ii_split_phase)
		|| check_system_type(cb_inv_type_riio_ii))
	{
		uint16_t w_data_len = 2;
		uint16_t w_meter_to_sys_energy = 0;
		uint16_t w_meter_from_sys_energy = 0;
		if(1 == b_meter_enable)//merter enable
		{
			if(check_system_type(cb_inv_type_riio_sun_ii) 
			|| check_system_type(cb_inv_type_riio_sun_ii_gte) 
			|| check_system_type(cb_inv_type_riio_ii))
			{
				net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_meter_1],
									   cb_net_data_att_status,
									   0x0118,															//正向有功电量
									   &w_data_len,
									   (uint8_t *)&w_meter_to_sys_energy,
									   cb_device_data_access_operate_type_read);
				net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_meter_1],
									   cb_net_data_att_status,
									   0x0119,															//反向有功电量
									   &w_data_len,
									   (uint8_t *)&w_meter_from_sys_energy,
									   cb_device_data_access_operate_type_read);

			}
			else if(check_system_type(cb_inv_type_riio_sun_ii_split_phase) || check_system_type(cb_inv_type_riio_ii_split_phase))
			{
				net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_meter_1],
									   cb_net_data_att_status,
									   0x0205,															//正向有功电量
									   &w_data_len,
									   (uint8_t *)&w_meter_to_sys_energy,
									   cb_device_data_access_operate_type_read);
				net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_meter_1],
									   cb_net_data_att_status,
									   0x0206,															//反向有功电量
									   &w_data_len,
									   (uint8_t *)&w_meter_from_sys_energy,
									   cb_device_data_access_operate_type_read);
			}

			if((0 != w_meter_comsume_backup) || (0 != w_meter_feedback_backup)
				|| (w_meter_to_sys_energy < 2 && w_meter_from_sys_energy < 2))		//原&&
			{
				w_grid_consume_today_realtime += negative_to_zero(w_meter_to_sys_energy - w_meter_comsume_backup);		//今日实时电网消耗 = 电表输入增量累加
				w_gird_feedback_today_realtime += negative_to_zero(w_meter_from_sys_energy - w_meter_feedback_backup);	//今日实时馈网 = 电表馈网增量累加
				w_normal_load_consumption = negative_to_zero(w_acin_feedback_today_realtime - w_gird_feedback_today_realtime + 
														 w_grid_consume_today_realtime - w_acin_today_realtime_true); //前端载 = （逆变器端馈网 - 电表端馈网增量）+（电表端输入增量 - 逆变器端输入）
			}
			w_meter_comsume_backup = w_meter_to_sys_energy;
			w_meter_feedback_backup = w_meter_from_sys_energy;
		}
		else
		{
			w_grid_consume_today_realtime = w_acin_today_realtime_true;
			w_gird_feedback_today_realtime = w_acin_feedback_today_realtime;
		}
		w_acin_port_today_realtime += w_grid_consume_today_realtime;		//acin端gen+grid 和ac out2 gen 这里加上ac in端grid数据
	}

    //acout total realtime
    uint16_t w_acout_load_realtime = 0;		//0133
    {
        w_acout_load_realtime = w_acout_today_realtime +\
                                w_acout2_today_realtime +\
                                w_normal_load_consumption;		//0146 + 012f + 0155
    }
	{
        uint16_t w_data_len = 2;
		uint16_t w_exCT = 0;
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                               cb_device_data_access_data_type_set,
                               0x0033,
                               &w_data_len,
                               (uint8_t *)&w_exCT,
                               cb_device_data_access_operate_type_read);
		net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x0141,
                               &w_data_len,
                               (uint8_t *)&w_exCT,
                               cb_device_data_access_operate_type_write);
	}
	{
        uint16_t w_data_len = 2;
		uint16_t w_inv_run_status = 0;
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                               cb_device_data_access_data_type_status,
                               0x011F,
                               &w_data_len,
                               (uint8_t *)&w_inv_run_status,
                               cb_device_data_access_operate_type_read);
		net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x0179,
                               &w_data_len,
                               (uint8_t *)&w_inv_run_status,
                               cb_device_data_access_operate_type_write);
	}
    {//write data
        uint16_t w_data_len = 2;
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x0146,
                               &w_data_len,
                               (uint8_t *)&w_acout_today_realtime,
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x0137,
                               &w_data_len,
                               (uint8_t *)&w_mppt_today_realtime,
                               cb_device_data_access_operate_type_write);//mppt sum
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_net_data_att_status,
                               0x0155,
                               &w_data_len,
                               (uint8_t *)&w_normal_load_consumption,
                               cb_device_data_access_operate_type_write);//normal load consumption
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_net_data_att_status,
                               0x0133,
                               &w_data_len,
                               (uint8_t *)&w_acout_load_realtime,
                               cb_device_data_access_operate_type_write);//acout load sum
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x012F,
                               &w_data_len,
                               (uint8_t *)&w_acout2_today_realtime,
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x015B,
                               &w_data_len,
                               (uint8_t *)&w_pv_today_realtime,
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x0145,
                               &w_data_len,
                               (uint8_t *)&w_grid_consume_today_realtime,
                               cb_device_data_access_operate_type_write);//w_acin_today_realtime
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x0147,
                               &w_data_len,
                               (uint8_t *)&w_acin_port_today_realtime,
                               cb_device_data_access_operate_type_write);//w_acin_total_today_realtime
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x0139,
                               &w_data_len,
                               (uint8_t *)&w_acin_gen_today_realtime,
                               cb_device_data_access_operate_type_write);//w_acin_gen_today_realtime
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x0127,
                               &w_data_len,
                               (uint8_t *)&w_gen_today_realtime,										//Tyrann：ACin2		Riio sun ii：GEN
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x0148,
                               &w_data_len,
                               (uint8_t *)&w_solor_today_realtime,
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x012A,
                               &w_data_len,
                               (uint8_t *)&w_gird_feedback_today_realtime,
                               cb_device_data_access_operate_type_write);//w_acin_feedback_today_realtime
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x012C,
                               &w_data_len,
                               (uint8_t *)&w_bat_charge_today_realtime,
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x012E,
                               &w_data_len,
                               (uint8_t *)&w_bat_discharge_today_realtime,
                               cb_device_data_access_operate_type_write);
        w_data_len = 1;
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x011C,
                               &w_data_len,
                               (uint8_t *)&g_parallel_system_mode,
                               cb_device_data_access_operate_type_write);
    }
    
    {//屏蔽实时数据界面频率
        uint16_t w_data_len = 2;
        uint16_t w_data_temp1 = 0;
		uint16_t w_data_temp2 = 0;
		uint16_t w_data_temp3 = 0;
		uint16_t w_smart_gen_vol = 0;
		uint16_t w_acout2_vol = 0;
		uint16_t w_acout1_vol = 0;
		for(uint8_t b_cnt_i = 0; b_cnt_i < 9; b_cnt_i++)
		{
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1 + b_cnt_i],
								   cb_device_data_access_data_type_status,
								   0x0107,
								   &w_data_len,
								   (uint8_t *)&w_data_temp1,		//w_acin_voltage
								   cb_device_data_access_operate_type_read);
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1 + b_cnt_i],
								   cb_device_data_access_data_type_status,
								   0x011F,
								   &w_data_len,
								   (uint8_t *)&w_data_temp2,		//inv_status
								   cb_device_data_access_operate_type_read);
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1 + b_cnt_i],
								   cb_device_data_access_data_type_status,
								   0x0108,
								   &w_data_len,
								   (uint8_t *)&w_smart_gen_vol,		//w_gen_voltage
								   cb_device_data_access_operate_type_read);
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1 + b_cnt_i],
								   cb_device_data_access_data_type_status,
								   0x010B,
								   &w_data_len,
								   (uint8_t *)&w_acout2_vol,		//w_acout2_voltage
								   cb_device_data_access_operate_type_read);
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1 + b_cnt_i],
								   cb_device_data_access_data_type_status,
								   0x010A,
								   &w_data_len,
								   (uint8_t *)&w_acout1_vol,		//w_acout2_voltage
								   cb_device_data_access_operate_type_read);
			if(40 > w_data_temp1)
			{
				w_data_temp1 = 0;
				net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1 + b_cnt_i],
							   cb_device_data_access_data_type_status,
							   0x0111,
							   &w_data_len,
							   (uint8_t *)&w_data_temp1,
							   cb_device_data_access_operate_type_write);
			}
//			if(0 == w_data_temp2)
//			{
//				w_data_temp2 = 0;
//				net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1 + b_cnt_i],
//							   cb_device_data_access_data_type_status,
//							   0x0113,
//							   &w_data_len,
//							   (uint8_t *)&w_data_temp2,
//							   cb_device_data_access_operate_type_write);
//			}
			if(40 > w_acout1_vol)
			{
				w_data_temp3 = 0;
				net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1 + b_cnt_i],
							   cb_device_data_access_data_type_status,
							   0x0113,
							   &w_data_len,
							   (uint8_t *)&w_data_temp3,
							   cb_device_data_access_operate_type_write);
			}
			if(40 > w_smart_gen_vol && 40 > w_acout2_vol)
			{
				w_data_temp3 = 0;
				net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1 + b_cnt_i],
							   cb_device_data_access_data_type_status,
							   0x0112,
							   &w_data_len,
							   (uint8_t *)&w_data_temp3,
							   cb_device_data_access_operate_type_write);
			}

		}
    }
    
    if(((((dw_timestamp % 600) - 540) < 10) &&\
        (dw_timestamp - dw_timestamp_backup) > 10) ||\
       (0 == dw_timestamp_backup))		//十分钟更新一次
    {	
        uint32_t dw_data_temp = 0;
        //energy_data st_energy_growth = {0,};

        dw_timestamp_backup = dw_timestamp;

        //bat  逆变器端电池充放电
        st_energy_growth.w_bat_charge =\
            negative_to_zero(w_bat_charge_today_realtime - st_energy_last_hour.w_bat_charge);
        st_energy_last_hour.w_bat_charge = w_bat_charge_today_realtime;

        st_energy_growth.w_bat_discharge =\
            negative_to_zero(w_bat_discharge_today_realtime - st_energy_last_hour.w_bat_discharge);
        st_energy_last_hour.w_bat_discharge = w_bat_discharge_today_realtime;

        //mppt
        st_energy_growth.w_sp_total =\
            negative_to_zero(w_mppt_today_realtime - st_energy_last_hour.w_sp_total);
        st_energy_last_hour.w_sp_total = w_mppt_today_realtime;

        //pv
        st_energy_growth.w_pv_total =\
            negative_to_zero(w_pv_today_realtime - st_energy_last_hour.w_pv_total);
        st_energy_last_hour.w_pv_total = w_pv_today_realtime;

        //solor
        st_energy_growth.w_solor_total =\
            negative_to_zero(w_solor_today_realtime - st_energy_last_hour.w_solor_total);
        st_energy_last_hour.w_solor_total = w_solor_today_realtime;

        //acin
//		if(check_system_type(cb_inv_type_tyrann) || check_system_type(cb_inv_type_apollo_matric_ii_10_15k))
//		{
//			//acin
//			st_energy_growth.w_acin_total =\
//				negative_to_zero(w_acin_port_today_realtime - st_energy_last_hour.w_acin_total);	
//			st_energy_last_hour.w_acin_total = w_acin_port_today_realtime; 
//		}
//		else	
		{		
			st_energy_growth.w_acin_total =\
				negative_to_zero(w_acin_port_today_realtime - st_energy_last_hour.w_acin_total);	
			st_energy_last_hour.w_acin_total = w_acin_port_today_realtime;	
			w_data_len = 2;
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
	                               cb_device_data_access_data_type_status,
	                               0x0128,
	                               &w_data_len,
	                               (uint8_t *)&w_acin_port_today_realtime,
	                               cb_device_data_access_operate_type_write);//w_acin_total_today_realtime

		}
        //acout
        st_energy_growth.w_acout_total =\
            negative_to_zero(w_acout_today_realtime +\
                                w_acout2_today_realtime +\
                                w_normal_load_consumption -\
                                st_energy_last_hour.w_acout_total);
        st_energy_last_hour.w_acout_total += st_energy_growth.w_acout_total;

        //acout feedback
        st_energy_growth.w_acout_feedback =\
            negative_to_zero(w_acout_feedback_1 + w_acout_feedback_2 - st_energy_last_hour.w_acout_feedback);
        st_energy_last_hour.w_acout_feedback = w_acout_feedback_1 + w_acout_feedback_2;

        //feedback
        st_energy_growth.w_feedback_total =\
            negative_to_zero(w_gird_feedback_today_realtime - st_energy_last_hour.w_feedback_total);
        st_energy_last_hour.w_feedback_total = w_gird_feedback_today_realtime;//w_acin_feedback_today_realtime

        {//w_sp_to_bat_today   //   w_sp_to_load_today
//        	static uint16_t w_solor_to_load_growth = 0;
            if(0 != w_mppt_today_realtime)
            {
                dw_data_temp = negative_to_zero_u32(st_energy_growth.w_sp_total - st_energy_growth.w_bat_discharge);		//w_bat_discharge等于mppt端输出的能量与电池实际放电的和
                st_energy_last_hour.w_sp_to_bat += dw_data_temp;								//若mppt输出大于w_bat_discharge则有部分去给电池充电，否则dw_data_temp为0，无影响
                if(0 == dw_data_temp)															//mppt输出没有给电池充电
                {
                    if(0 == st_energy_growth.w_feedback_total)									//系统馈网为0，说明电池放电和mppt输全部带载
                	{
						st_energy_last_hour.w_sp_to_load += st_energy_growth.w_sp_total;			//mppt的能量全部去带载
					}
					else if(st_energy_last_hour.w_sp_total > st_energy_growth.w_feedback_total)		//mppt输入大于馈网电量,馈网全部来自mppt，mppt剩余带载
					{
						st_energy_last_hour.w_sp_to_load += st_energy_growth.w_sp_total - st_energy_growth.w_feedback_total;	
					}
					else																			//mppt输入小于馈网电量，mppt全部馈网，带载为0
					{
						st_energy_last_hour.w_sp_to_load += 0;	
					}
                  }
                else//mppt输出大于逆变器端电池放电 则有给电池充电
                {
                	if(0 == st_energy_growth.w_feedback_total)									//系统馈网为0，说明电池放电和mppt输全部带载
                	{
						st_energy_last_hour.w_sp_to_load += st_energy_growth.w_bat_discharge;	//w_bat_discharge全部去带载
					}
					else
					{
						st_energy_last_hour.w_sp_to_load += st_energy_growth.w_bat_discharge - st_energy_growth.w_feedback_total;	//如果存在馈网，去带载的能量是w_bat_discharge总能量减去馈网的能量
					}
                }
//                	st_energy_growth.w_solar_to_load = negative_to_zero(st_energy_last_hour.w_solar_to_load - w_solor_to_load_growth);
//					w_solor_to_load_growth = st_energy_last_hour.w_solar_to_load;
            }
            else
            {
                st_energy_last_hour.w_sp_to_bat = 0;
                st_energy_last_hour.w_sp_to_load = 0;
            }
        }

        {
        /*{//w_sp_to_bat_today   //   w_sp_to_load_today
            if(0 != w_mppt_today_realtime)
            {
                uint16_t w_sp_to_grid_growth = 0;
                if(0 == w_pv_today_realtime)
                {
                    if((st_energy_growth.w_sp_total >= st_energy_growth.w_bat_charge) &&\
                       (st_energy_growth.w_sp_total >= st_energy_growth.w_feedback_total))
                    {
                        w_sp_to_grid_growth = st_energy_growth.w_feedback_total;
                    }
                    else if((st_energy_growth.w_sp_total >= st_energy_growth.w_bat_charge) &&\
                            (st_energy_growth.w_sp_total < st_energy_growth.w_feedback_total))
                    {
                        w_sp_to_grid_growth = st_energy_growth.w_sp_total - st_energy_growth.w_bat_charge;
                    }
                }

                st_energy_growth.w_sp_to_load =\
                    negative_to_zero(st_energy_growth.w_sp_total -\
                                     st_energy_growth.w_bat_charge -\
                                     w_sp_to_grid_growth);
                st_energy_last_hour.w_sp_to_load += st_energy_growth.w_sp_to_load;

                {
                    if(0 == st_energy_growth.w_sp_to_load)
                    {
                        st_energy_growth.w_sp_to_bat = st_energy_growth.w_sp_total - w_sp_to_grid_growth;
                    }
                    else
                    {
                        st_energy_growth.w_sp_to_bat = st_energy_growth.w_bat_charge;
                    }

                    st_energy_last_hour.w_sp_to_bat += st_energy_growth.w_sp_to_bat;
                }
            }
            else
            {
                st_energy_last_hour.w_sp_to_bat = 0;
                st_energy_last_hour.w_sp_to_load = 0;
            }
        }*/
        }
		
        {//pv2bat pv2load   //mark
            if(0 != w_pv_today_realtime)
            {
                st_energy_growth.w_pv_to_bat =\
                    negative_to_zero(st_energy_growth.w_acout_feedback - st_energy_growth.w_feedback_total);
                if(st_energy_growth.w_pv_to_bat > st_energy_growth.w_bat_charge)
                {
                    st_energy_growth.w_pv_to_bat = st_energy_growth.w_bat_charge;
                    st_energy_growth.w_acout_total += (st_energy_growth.w_pv_to_bat - st_energy_growth.w_bat_charge);
                    st_energy_last_hour.w_acout_total += st_energy_growth.w_acout_total;
                }
                st_energy_growth.w_pv_to_load =\
                    negative_to_zero(st_energy_growth.w_pv_total -\
                                     st_energy_growth.w_pv_to_bat -\
                                     st_energy_growth.w_feedback_total);
                st_energy_last_hour.w_pv_to_bat += st_energy_growth.w_pv_to_bat;
                st_energy_last_hour.w_pv_to_load += st_energy_growth.w_pv_to_load;
            }
            else
            {
                st_energy_last_hour.w_pv_to_bat = 0;
                st_energy_last_hour.w_pv_to_load = 0;
            }
        }

//        {//solor2grid
//            if(0 == st_energy_growth.w_solor_total)
//            {
//                st_energy_last_hour.w_bat_to_grid += st_energy_growth.w_feedback_total;			//没有光伏情况下馈网的能量全部由电池出
//            }
//            else
//            {
//                st_energy_last_hour.w_solor_to_grid += st_energy_growth.w_feedback_total;		//有光伏情况下馈网能量由光伏出
//            }
//        }

        {//solor2bat
            dw_data_temp = st_energy_last_hour.w_sp_to_bat + st_energy_last_hour.w_pv_to_bat;
            if(0 == dw_data_temp)
            {
                st_energy_last_hour.w_solar_to_bat = 0;
            }
            else
            {
                st_energy_growth.w_solar_to_bat =\
                    negative_to_zero(dw_data_temp - st_energy_last_hour.w_solar_to_bat);
                st_energy_last_hour.w_solar_to_bat = negative_to_zero(dw_data_temp);
            }
        }
        {//solor2load
            dw_data_temp = st_energy_last_hour.w_sp_to_load + st_energy_last_hour.w_pv_to_load;
            if(0 == dw_data_temp)
            {
                st_energy_last_hour.w_solar_to_load = 0;
            }
            else
            {
                st_energy_growth.w_solar_to_load =\
                    negative_to_zero(dw_data_temp - st_energy_last_hour.w_solar_to_load);
                st_energy_last_hour.w_solar_to_load = negative_to_zero(dw_data_temp);
            }
        }
        {//solor2grid   bat2grid
            st_energy_growth.w_solor_to_grid = negative_to_zero(st_energy_growth.w_solor_total -\
                                                                st_energy_growth.w_solar_to_bat -\
                                                                st_energy_growth.w_solar_to_load);
            st_energy_growth.w_bat_to_grid = negative_to_zero(st_energy_growth.w_feedback_total -\
                                                               st_energy_growth.w_solor_to_grid);

            st_energy_last_hour.w_bat_to_grid += st_energy_growth.w_bat_to_grid;
            st_energy_last_hour.w_solor_to_grid += st_energy_growth.w_solor_to_grid;
        }
        {//acin
            if(0 == st_energy_last_hour.w_acin_total)
            {
                st_energy_last_hour.w_acin_to_bat = 0;
                st_energy_last_hour.w_acin_to_load = 0;
            }
            else
            {
                if(0 != st_energy_growth.w_acin_total || 0 != st_energy_growth.w_bat_charge)
                {	//w_acin_to_bat 003400000134
                    st_energy_growth.w_acin_to_bat =\
                        negative_to_zero(st_energy_growth.w_bat_charge -\
                                         st_energy_growth.w_pv_to_bat);
                    if((st_energy_growth.w_acin_total >= st_energy_growth.w_acin_to_bat) 
						||(0 == st_energy_growth.w_acin_total && 0 != st_energy_growth.w_acin_to_bat) )
                    {
                        st_energy_last_hour.w_acin_to_bat += st_energy_growth.w_acin_to_bat;
                    }
                    else
                    {
                        st_energy_growth.w_acin_to_bat = st_energy_growth.w_acin_total;
                        st_energy_last_hour.w_acin_to_bat += st_energy_growth.w_acin_to_bat;
                    }
                    st_energy_growth.w_acin_to_load = st_energy_growth.w_acin_total - st_energy_growth.w_acin_to_bat;
                    st_energy_last_hour.w_acin_to_load += st_energy_growth.w_acin_to_load;
                }
                else
                {
                    st_energy_growth.w_acin_to_bat = 0;
                    st_energy_growth.w_acin_to_load = 0;
                }
            }
        }
        {//acout
            if((0 == st_energy_last_hour.w_acout_total) &&\
               (0 == st_energy_last_hour.w_pv_total))
            {
                st_energy_last_hour.w_acout_from_bat = 0;
            }
            else
            {
					st_energy_growth.w_acout_from_bat =\
                    negative_to_zero(st_energy_growth.w_acout_total -\
                                     st_energy_growth.w_solar_to_load -\
                                     st_energy_growth.w_acin_to_load);
                st_energy_last_hour.w_acout_from_bat += st_energy_growth.w_acout_from_bat;
            }
            st_energy_last_hour.w_acout_ideal +=\
                st_energy_growth.w_acout_from_bat +\
                st_energy_growth.w_solar_to_load +\
                st_energy_growth.w_acin_to_load;
			st_energy_last_hour.w_acout_ideal = negative_to_zero(st_energy_last_hour.w_acout_ideal);
        }
        net_write_energy_data(0,cb_energy_data_num,(energy_data_info *)&st_energy_data_info_last_hour[0]);
//        DebugPrintf(TRUE,"Hour data statistics compete,timestamp:%d\n",dw_timestamp);
    }
//    */
#if( task_runing_status_print )
    DebugPrintf(TRUE,"Riiosunii_energy_statisics_end\r\n");
#endif
}

uint8_t check_all_in_one(uint32_t dw_device_id)
{
    const net_data_info *pst_data_info = 0;
    net_device_info *pst_net_device_info = 0;
    uint16_t w_system_type = cb_inv_type_ck;

    if(FALSE == net_get_device_info(((dw_device_id & 0x0000FF) | cdw_device_type_cklcd),
                                    (net_device_info **)&pst_net_device_info,
                                    &st_net_system_info_list))
    {
        #if( system_alarm_print )
        DebugPrintf(TRUE,"[Alarm]Check all in one error :1\r\n");
        #endif

        return FALSE;
    }

    if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                         0x0004,
                                         (const net_data_info **)&pst_data_info,
                                         pst_net_device_info))
    {
        #if( system_alarm_print )
        DebugPrintf(TRUE,"[Alarm]Check all in one error :2\r\n");
        #endif

        return FALSE;
    }

    w_system_type = ((uint16_t)(*pst_data_info->pb_data) |\
                     ((uint16_t)(*(pst_data_info->pb_data + 1)) << 8));
	w_system_type = get_system_type();
    {
        uint16_t w_data_len = 2;
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x0125,
                               &w_data_len,
                               (uint8_t *)&w_system_type,
                               cb_device_data_access_operate_type_write);
    }

    if((cb_inv_type_apollo_maxx == w_system_type) ||\
       (cb_inv_type_riio_sun == w_system_type) ||\
       (cb_inv_type_cg == w_system_type)||\
       (cb_inv_type_apollo_matrix == w_system_type) ||\
       (cb_inv_type_riio_sun_ii == w_system_type)||\
       (cb_inv_type_riio_sun_ii_gte == w_system_type)||\
       (cb_inv_type_riio_sun_ii_split_phase == w_system_type)||\
       (cb_inv_type_apollo_matric_ii_lite == w_system_type)||\
       (cb_inv_type_apollo_matric_ii_5_8k == w_system_type))
    {
        return TRUE;
    }
    return FALSE;
}
uint8_t check_ck_bypass_connect()
{
    #define cb_ck_model_ckii_30m      1
    #define cb_ck_model_ck20l         2
    #define cb_ck_model_ck50m         3
    #define cb_ck_model_ck30l         4
    #define cb_ck_model_ckii_30s      5
    #define cb_ck_model_ck40s         6
    #define cb_ck_model_ck60s         7
    #define cb_ck_model_ck80s         8
    #define cb_ck_model_ck40m         9
    #define cb_ck_model_ck50s         10
    #define cb_ck_model_ck30l_simple  11
    #define cb_ck_model_ck20l_simple  12

    #define cb_total_bypass           0
    #define cb_bypass_assist          1

    uint16_t w_ck_module_version = 0;
    if(data_process_type_riiosunii == enum_data_process_func_type)
    {
        get_ck_inverter_data(0,1,0x012C,&w_ck_module_version);
    }
    else
    {
        get_ck_inverter_data(0,1,0x0128,&w_ck_module_version);
    }

    w_ck_module_version = (w_ck_module_version & 0x00FF);

    if((cb_ck_model_ck50m == w_ck_module_version) ||\
       (cb_ck_model_ck40s == w_ck_module_version) ||\
       (cb_ck_model_ck60s == w_ck_module_version) ||\
       (cb_ck_model_ck80s == w_ck_module_version) ||\
       (cb_ck_model_ck40m == w_ck_module_version) ||\
       (cb_ck_model_ck50s == w_ck_module_version))
    {
        return TRUE;
    }
    return FALSE;
}
/************************************************************************************************
* Input: void
*
* Output: void
*
* Logic discription: system energy flow gather
*                    kinergy ststus 0x0124
*
* run time:
************************************************************************************************/
void energy_flow_gather()
{
    #define cw_energy_flow_bit0_1                   0x0003
    #define cw_energy_flow_bit2_3                   0x000C
    #define cw_energy_flow_bit4_5                   0x0030
    #define cw_energy_flow_bit6_7                   0x00C0
    #define cw_energy_flow_bit8                     0x0100
    #define cw_energy_flow_bit9                     0x0200
    #define cw_energy_flow_bit10                    0x0400
    #define cw_energy_flow_bit11_12                 0x1800
    #define cw_energy_flow_bit13                    0x2000
    #define cw_energy_flow_bit14_15                 0xC000
    #define cw_energy_flow_bit16                    0x00010000

    #define cb_energy_flow_value_0                  0
    #define cb_energy_flow_value_1                  1
    #define cb_energy_flow_value_2                  2
    #define cb_energy_flow_value_3                  3

    const net_data_info *pst_data_info = 0;
    net_device_info *pst_net_device_info = 0;

    uint16_t w_energy_flow_gather = 0;
    uint32_t dw_system_energy_flow_gather = 0;
    uint8_t b_dc_couple_last = 0;
    uint8_t b_inv_swtich_last = 0;
    uint8_t b_smart_gen_last = 0;
    uint8_t b_sys_run_status_last = 0;
    uint8_t b_system_alarm_last = 0;
    uint8_t b_system_fault_last = 0;
    uint16_t w_system_conditional_bits = 0;
    uint8_t b_acout_last = 0;
    uint8_t b_acin_last = 0;
    uint8_t b_acin_flag = 0;
	uint8_t b_acin2_exist = 0;

//    uint16_t w_data_temp = 0;
#if( task_runing_status_print )
    DebugPrintf(TRUE,"Energy_flow_gather_start\r\n");
#endif
	if(check_system_type(cb_inv_type_tyrann) || check_system_type(cb_inv_type_apollo_matric_ii_10_15k))
	{
		b_acin2_exist = 1;
	}
    for(uint8_t b_cnt_i = 0;b_cnt_i < 9;b_cnt_i++)
    {
        if(get_ck_inverter_data(b_cnt_i,1,0x0100,&w_energy_flow_gather))
        {
            //dc couple
            if(st_power_current_data.dw_mppt_output_power_total > 100)
            {
                if(cb_energy_flow_value_1 == (uint8_t)((w_energy_flow_gather & cw_energy_flow_bit6_7) >> 6))		//6-7位为1 mppt流向系统
                {
                    dw_system_energy_flow_gather &= 0xFF3F;
					dw_system_energy_flow_gather |= (cb_energy_flow_value_1 << 6);
                    b_dc_couple_last = cb_energy_flow_value_1;
                }
                else if(cb_energy_flow_value_3 == (uint8_t)((w_energy_flow_gather & cw_energy_flow_bit6_7) >> 6))	//6-7位为3 无能量流 mppt待供电
                {
                    if(cb_energy_flow_value_1 != b_dc_couple_last)													//如果前一台逆变器6-7位不是1则设为3，是1则还是保存不变还为1
                    {
                        dw_system_energy_flow_gather &= 0xFF3F;
                        dw_system_energy_flow_gather |= (cb_energy_flow_value_3 << 6);
                        b_dc_couple_last = cb_energy_flow_value_3;
                    }
                }
                else if(cb_energy_flow_value_0 == (uint8_t)((w_energy_flow_gather & cw_energy_flow_bit6_7) >> 6))	//6-7位为0
                {
                	uint16_t w_inv_power = 0;
					uint16_t w_mppt_power = 0;
					get_ck_inverter_data(0,9,0x0103,&w_inv_power);
					get_mppt_data(0x0102,&w_mppt_power);
                    if((cb_energy_flow_value_1 != b_dc_couple_last) && (cb_energy_flow_value_3 != b_dc_couple_last)) //如果前几台逆变器均不是1或3则置0 mppt离线，是1或3则还是保存不变还为1或3
                    {
                        dw_system_energy_flow_gather &= 0xFF3F;
                        b_dc_couple_last = cb_energy_flow_value_0;
                    }
					if((0 == (st_power_current_data.w_bat_current & 0x8000) && (st_power_current_data.w_bat_current > 20))
						||(0 == ( w_inv_power & 0x8000) && (w_inv_power > 100)))		//bat charege cruuent >0 
					{
						dw_system_energy_flow_gather &= 0xFF3F;
						dw_system_energy_flow_gather |= (cb_energy_flow_value_1 << 6);
	                    b_dc_couple_last = cb_energy_flow_value_1;
					}
					else if(1 == (w_inv_power & 0x8000) && w_inv_power < 0xFF9C)		//bat discharge cruuent < 0 
					{
	                    if((cb_energy_flow_value_1 != b_dc_couple_last) &&\
	                       (cb_energy_flow_value_3 != b_dc_couple_last) &&\
	                       (cb_energy_flow_value_0 != b_dc_couple_last))
	                    {
	                        dw_system_energy_flow_gather &= 0xFF3F;
	                        dw_system_energy_flow_gather |= (cb_energy_flow_value_2 << 6);
	                        b_dc_couple_last = cb_energy_flow_value_2;
	                    }
					}
                }
                else if(cb_energy_flow_value_2 == (uint8_t)((w_energy_flow_gather & cw_energy_flow_bit6_7) >> 6))
                {
                    if((cb_energy_flow_value_1 != b_dc_couple_last) &&\
                       (cb_energy_flow_value_3 != b_dc_couple_last) &&\
                       (cb_energy_flow_value_0 != b_dc_couple_last))
                    {
                        dw_system_energy_flow_gather &= 0xFF3F;
                        dw_system_energy_flow_gather |= (cb_energy_flow_value_2 << 6);
                        b_dc_couple_last = cb_energy_flow_value_2;
                    }
                }
            }
            //system runing status
            if(cb_energy_flow_value_1 == (uint8_t)((w_energy_flow_gather & cw_energy_flow_bit8) >> 8))
            {
                dw_system_energy_flow_gather &= 0xFEFF;
                dw_system_energy_flow_gather |= (cb_energy_flow_value_1 << 8);
                b_sys_run_status_last = cb_energy_flow_value_1;
            }
            else if(cb_energy_flow_value_0 == (uint8_t)((w_energy_flow_gather & cw_energy_flow_bit8) >> 8))
            {
                if(cb_energy_flow_value_1 != b_sys_run_status_last)
                {
                    dw_system_energy_flow_gather &= 0xFEFF;
                    b_sys_run_status_last = cb_energy_flow_value_0;
                }
            }
            //system alarm
            if(cb_energy_flow_value_1 == (uint8_t)((w_energy_flow_gather & cw_energy_flow_bit9) >> 9))
            {
                dw_system_energy_flow_gather &= 0xFDFF;
                dw_system_energy_flow_gather |= (cb_energy_flow_value_1 << 9);
                b_system_alarm_last = cb_energy_flow_value_1;
            }
            else if(cb_energy_flow_value_0 == (uint8_t)((w_energy_flow_gather & cw_energy_flow_bit9) >> 9))
            {
                if(cb_energy_flow_value_1 != b_system_alarm_last)
                {
                    dw_system_energy_flow_gather &= 0xFDFF;
                    b_system_alarm_last = cb_energy_flow_value_0;
                }
            }
            //system fault
            if(cb_energy_flow_value_1 == (uint8_t)((w_energy_flow_gather & cw_energy_flow_bit10) >> 10))
            {
                dw_system_energy_flow_gather &= 0xFBFF;
                dw_system_energy_flow_gather |= (cb_energy_flow_value_1 << 10);
                b_system_fault_last = cb_energy_flow_value_1;
            }
            else if(cb_energy_flow_value_0 == (uint8_t)((w_energy_flow_gather & cw_energy_flow_bit10) >> 10))
            {
                if(cb_energy_flow_value_1 != b_system_fault_last)
                {
                    dw_system_energy_flow_gather &= 0xFBFF;
                    b_system_fault_last = cb_energy_flow_value_0;
                }
            }

            if(data_process_type_riiosunii == enum_data_process_func_type)
            {   //acout
                get_ck_inverter_data(0,1,0x0120,&w_system_conditional_bits);
                if((cb_energy_flow_value_3 == (uint8_t)((w_energy_flow_gather & cw_energy_flow_bit2_3) >> 2)) &&\
                   ((0x0000 == (st_power_current_data.w_uvw_acload_power_p_sum & 0x8000)) && (st_power_current_data.w_uvw_acload_power_p_sum > 100)) &&\
                   ((0x0000 == (st_power_current_data.w_uvw_acload_power2_p_sum & 0x8000)) && (st_power_current_data.w_uvw_acload_power2_p_sum > 100)))			//2-3位为3且acload 1\2功率非负大于100
                {
                    dw_system_energy_flow_gather &= 0xFFF3;
                    dw_system_energy_flow_gather |= (cb_energy_flow_value_3 << 2);
                    b_acout_last = cb_energy_flow_value_3;
                }
                else if((cb_energy_flow_value_1 == (uint8_t)((w_energy_flow_gather & cw_energy_flow_bit2_3) >> 2)) &&\
                        ((0x0000 == (st_power_current_data.w_uvw_acload_power_p_sum & 0x8000)) &&(st_power_current_data.w_uvw_acload_power_p_sum > 100)))		//2-3位为1且acload 1功率非负大于100
                {
                    if((cb_energy_flow_value_1 != b_acout_last) &&\
                       (cb_energy_flow_value_2 != b_acout_last) &&\
                       (cb_energy_flow_value_3 != b_acout_last))
                    {
                        dw_system_energy_flow_gather &= 0xFFF3;
                        dw_system_energy_flow_gather |= (cb_energy_flow_value_1 << 2);
                        b_acout_last = cb_energy_flow_value_1;
                    }
                    else if(cb_energy_flow_value_2 == b_acout_last)
                    {
                        dw_system_energy_flow_gather &= 0xFFF3;
                        dw_system_energy_flow_gather |= (cb_energy_flow_value_3 << 2);
                        b_acout_last = cb_energy_flow_value_3;
                    }
                }
                else if((cb_energy_flow_value_2 == (uint8_t)((w_energy_flow_gather & cw_energy_flow_bit2_3) >> 2)) &&\
                        ((0x0000 == (st_power_current_data.w_uvw_acload_power2_p_sum & 0x8000)) &&(st_power_current_data.w_uvw_acload_power2_p_sum > 100)))		//2-3位为2且acload 1功率非负大于100
                {
                    if((cb_energy_flow_value_1 != b_acout_last) &&\
                       (cb_energy_flow_value_3 != b_acout_last))
                    {
                        dw_system_energy_flow_gather &= 0xFFF3;
                        dw_system_energy_flow_gather |= (cb_energy_flow_value_2 << 2);
                        b_acout_last = cb_energy_flow_value_2;
                    }
                    else if(cb_energy_flow_value_1 == b_acout_last)
                    {
                        dw_system_energy_flow_gather &= 0xFFF3;
                        dw_system_energy_flow_gather |= (cb_energy_flow_value_3 << 2);
                        b_acout_last = cb_energy_flow_value_3;
                    }
                }
                else if(cb_energy_flow_value_0 == (uint8_t)((w_energy_flow_gather & cw_energy_flow_bit2_3) >> 2))												//2-3位为0且acload 1功率非负大于100
                {
                    if((cb_energy_flow_value_1 != b_acout_last) &&\
                       (cb_energy_flow_value_2 != b_acout_last) &&\
                       (cb_energy_flow_value_3 != b_acout_last))
                    {
                        dw_system_energy_flow_gather &= 0xFFF3;
                    }
                }
            }
            else
            {
                get_ck_inverter_data(0,1,0x0115,&w_system_conditional_bits);
                //acout
                if((0x80 == (w_system_conditional_bits & 0x80)) || (0x40 == (w_system_conditional_bits & 0x40)))
                {
                    if(0x8000 == (st_power_current_data.w_uvw_acload_power_p_sum & 0x8000))
                    {
                        if(st_power_current_data.w_uvw_acload_power_p_sum < 0xFF9C)
                        {
                            dw_system_energy_flow_gather &= 0xFFF3;
                            dw_system_energy_flow_gather |= (cb_energy_flow_value_1 << 2);
                            b_acout_last = cb_energy_flow_value_1;
                        }
                    }
                    else
                    {
//                      if(cb_energy_flow_value_1 != b_acout_last)
                      if((cb_energy_flow_value_1 != b_acout_last) && (st_power_current_data.w_uvw_acload_power_p_sum > 100) )
                        {
                            dw_system_energy_flow_gather &= 0xFFF3;
                            dw_system_energy_flow_gather |= (cb_energy_flow_value_2 << 2);
                            b_acout_last = cb_energy_flow_value_2;
                        }
                    }
                }
                else
                {
                    if((cb_energy_flow_value_1 != b_acout_last) &&\
                       (cb_energy_flow_value_2 != b_acout_last))
                    {
                        dw_system_energy_flow_gather &= 0xFFF3;
                        b_acout_last = cb_energy_flow_value_0;
                    }
                }              
            }

            //acin
            {   //mark w_acin_logic read by net_device_data_access
                uint16_t w_acin_logic = 0;
                if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                                (net_device_info **)&pst_net_device_info,
                                                &st_net_system_info_list))
                {
                    #if( system_alarm_print )
                    DebugPrintf(TRUE,"[Alarm]Energy flow error :1\r\n");
                    #endif

                    return;
                }

                if(FALSE == net_get_device_data_info(cb_net_data_att_status,
                                                     0x0120,
                                                     (const net_data_info **)&pst_data_info,
                                                     pst_net_device_info))
                {
                    #if( system_alarm_print )
                    DebugPrintf(TRUE,"[Alarm]Energy flow error :2\r\n");
                    #endif

                    return;
                }
                w_acin_logic = *pst_data_info->pb_data;

                if (((0 != (w_acin_logic & 0x01)) &&\
                   ((st_power_current_data.w_uvw_acin_power_sum > 0xFF9C && (0x8000 == (st_power_current_data.w_uvw_acin_power_sum & 0x8000))) ||		//ACin1存在，且ACin1总功率绝对值小于100
                    (st_power_current_data.w_uvw_acin_power_sum < 0x0064 && (0x0000 == (st_power_current_data.w_uvw_acin_power_sum & 0x8000)))))||\
                    (b_acin2_exist && (0 != (w_acin_logic & 0x100)) &&\
                    ((st_power_current_data.w_gen_power_p_sum > 0xFF9C && (0x8000 == (st_power_current_data.w_gen_power_p_sum & 0x8000))) ||		//ACin2存在，且ACin2总功率绝对值小于100
                    (st_power_current_data.w_gen_power_p_sum < 0x0064 && (0x0000 == (st_power_current_data.w_gen_power_p_sum & 0x8000))))))
                {
                    dw_system_energy_flow_gather &= 0xFFFC;
                    dw_system_energy_flow_gather |= (cb_energy_flow_value_3 << 0);
                    b_acin_last = cb_energy_flow_value_3;
                }
                else if((0x8000 == (st_power_current_data.w_uvw_acin_power_sum & 0x8000)) || (b_acin2_exist && (0x8000 == (st_power_current_data.w_gen_power_p_sum & 0x8000)))) 		//ACin1总功率为负数
                {
                    if((st_power_current_data.w_uvw_acin_power_sum <= 0xFF9C) || (b_acin2_exist && (st_power_current_data.w_gen_power_p_sum <= 0xFF9C)))					//ACin1总功率为负数且绝对值大于100
                    {
                        dw_system_energy_flow_gather &= 0xFFFC;
                        dw_system_energy_flow_gather |= (cb_energy_flow_value_2 << 0);
                        b_acin_last = cb_energy_flow_value_2;
                        b_acin_flag = (cb_energy_flow_value_2 << 0);
                    }
                    else if((st_power_current_data.w_uvw_acin_power_sum < 0xFF9C) || (b_acin2_exist && (st_power_current_data.w_gen_power_p_sum < 0xFF9C)))				//ACin1总功率为负数且绝对值大于100
                    {
                        if(cb_energy_flow_value_2 != b_acin_last)
                        {
                            dw_system_energy_flow_gather &= 0xFFFC;
                            dw_system_energy_flow_gather |= (cb_energy_flow_value_1 << 0);
                            b_acin_last = cb_energy_flow_value_1;
                            b_acin_flag = (cb_energy_flow_value_1 << 0);
                        }
                    }
                    else
                    {
                        dw_system_energy_flow_gather |= b_acin_flag;
                    }
                }
                else
                {
                    if(cb_energy_flow_value_2 != b_acin_last)
                    {
                        if(st_power_current_data.w_uvw_acin_power_sum >= 0x0064 ||\
							(b_acin2_exist && st_power_current_data.w_gen_power_p_sum >= 0x0064 ))
                        {
                            dw_system_energy_flow_gather &= 0xFFFC;
                            dw_system_energy_flow_gather |= (cb_energy_flow_value_1 << 0);
                            b_acin_last = cb_energy_flow_value_1;
                            b_acin_flag = (cb_energy_flow_value_1 << 0);
                        }
                    }
                }
            }
            if(0 == b_cnt_i)
            {
				//bat
                if((0x8000 == (st_power_current_data.w_bat_current & 0x8000)) && (st_power_current_data.w_bat_current <= 0xFFEC)) 
                    //&& (0 != (w_energy_flow_gather & cw_energy_flow_bit2_3) >> 2 ))  //加这句会导致主机没有接负载从机有接负载 电池端不会显示能量流
                {
                    dw_system_energy_flow_gather |= (cb_energy_flow_value_1 << 4);
                }
                else if((0x0000 == (st_power_current_data.w_bat_current & 0x8000)) && (st_power_current_data.w_bat_current >= 20))
                {
                    dw_system_energy_flow_gather |= (cb_energy_flow_value_2 << 4);
                }
                else     //加入电池电流处于-2A~2A直接不显示能量流
                {
                    dw_system_energy_flow_gather &= ~(cb_energy_flow_value_3 << 4);
                }
                //ac couple
                dw_system_energy_flow_gather |= (w_energy_flow_gather & cw_energy_flow_bit11_12);
            }
            //inv switch
            if(cb_energy_flow_value_1 == (uint8_t)((w_energy_flow_gather & cw_energy_flow_bit13) >> 13))
            {
                dw_system_energy_flow_gather &= 0xDFFF;
                dw_system_energy_flow_gather |= (cb_energy_flow_value_1 << 13);
                b_inv_swtich_last = cb_energy_flow_value_1;
            }
            else if(cb_energy_flow_value_0 == (uint8_t)((w_energy_flow_gather & cw_energy_flow_bit13) >> 13))
            {
                if(cb_energy_flow_value_1 != b_inv_swtich_last)
                {
                    dw_system_energy_flow_gather &= 0xDFFF;
                }
            }
            //smart gen
            if(cb_energy_flow_value_1 == (uint8_t)((w_energy_flow_gather & cw_energy_flow_bit14_15) >> 14))
            {
                dw_system_energy_flow_gather &= 0x3FFF;
                dw_system_energy_flow_gather |= (cb_energy_flow_value_1 << 14);
                b_smart_gen_last = cb_energy_flow_value_1;
            }
            else if(cb_energy_flow_value_3 == (uint8_t)((w_energy_flow_gather & cw_energy_flow_bit14_15) >> 14))
            {
                if(cb_energy_flow_value_1 != b_smart_gen_last)
                {
                    dw_system_energy_flow_gather &= 0x3FFF;
                    dw_system_energy_flow_gather |= (cb_energy_flow_value_3 << 14);
                    b_smart_gen_last = cb_energy_flow_value_3;
                }
            }
            else if(cb_energy_flow_value_0 == (uint8_t)((w_energy_flow_gather & cw_energy_flow_bit14_15) >> 14))
            {
                if((cb_energy_flow_value_1 != b_smart_gen_last) &&\
                   (cb_energy_flow_value_3 != b_smart_gen_last))
                {
                    dw_system_energy_flow_gather &= 0x3FFF;
                }
            }
        }
    }
    if(data_process_type_riiosunii == enum_data_process_func_type)
    {
        uint16_t w_data_len = 2;
        uint16_t w_data_temp = 0;
        {
            net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                   cb_net_data_att_status,
                                   0x017D,
                                   &w_data_len,
                                   (uint8_t *)&w_data_temp,
                                   cb_device_data_access_operate_type_read);//normal load p
            if(w_data_temp >= 100)
            {
                dw_system_energy_flow_gather |= (cb_energy_flow_value_1 << 16);
            }
        }
        if((st_power_current_data.w_uvw_acin_power_sum <= 0xFF9C && (0x8000 == (st_power_current_data.w_uvw_acin_power_sum & 0x8000))) ||\
           (st_power_current_data.w_uvw_acin_power_sum >= 0x0064 && (0x0000 == (st_power_current_data.w_uvw_acin_power_sum & 0x8000))) ||			//acin1功率绝对值大于100
           (w_data_temp >= 0x0064 && (0x0000 == (w_data_temp & 0x8000))) ||																		//前端载功率非负大于100
           (b_acin2_exist &&\
           (st_power_current_data.w_gen_power_p_sum >= 0x0064 && (0x0000 == (st_power_current_data.w_gen_power_p_sum & 0x8000))))) 			//存在acin2且功率非负大于100
        {   //acin port
            uint32_t dw_i_acin = 0;
			uint32_t dw_i_acin2 = 0;
            get_ck_inverter_data_4_bytes(0,9,0x010C,&dw_i_acin,TRUE);
			get_ck_inverter_data_4_bytes(0,9,0x010D,&dw_i_acin2,TRUE);
            if(0 != (0x80000000 & dw_i_acin))
            {
                dw_i_acin = ~dw_i_acin + 1;
                if(dw_i_acin >= 50)     
                {
                    dw_system_energy_flow_gather |= (cb_energy_flow_value_2 << 17);
                }
            }
            else if(dw_i_acin >= 50 || dw_i_acin2 >=50)
            {
                dw_system_energy_flow_gather |= (cb_energy_flow_value_1 << 17);
            }
        }
        if(((0 != (dw_system_energy_flow_gather & 0x000F)) && (3 != (dw_system_energy_flow_gather & 0x000F))) ||\
           (cb_energy_flow_value_1 == (uint8_t)((dw_system_energy_flow_gather & cw_energy_flow_bit14_15) >> 14)) ||\
           (cb_energy_flow_value_1 == (uint8_t)((dw_system_energy_flow_gather & cw_energy_flow_bit16) >> 16)) ||\
           (cb_energy_flow_value_1 == (uint8_t)((dw_system_energy_flow_gather & cw_energy_flow_bit6_7) >> 6)))
        {   //bat port
            uint32_t dw_i_bat = 0;  //st_power_current_data.w_bat_current

            get_ck_inverter_data_4_bytes(0,9,0x0115,&dw_i_bat,TRUE);

            if((0 == (dw_i_bat & 0x80000000)) &&\
               (dw_i_bat >= 20))
            {
                dw_system_energy_flow_gather |= (cb_energy_flow_value_2 << 21);
            }
            else if((0 != (dw_i_bat & 0x80000000)) &&\
                    (dw_i_bat <= 0xFFFFFFEC))		//-20  2A
            {
                dw_system_energy_flow_gather |= (cb_energy_flow_value_1 << 21);
            }

            if(((0x0000 == (st_power_current_data.w_bat_current & 0x8000)) && (st_power_current_data.w_bat_current < 20) || 
                (0x8000 == (st_power_current_data.w_bat_current & 0x8000)) && (st_power_current_data.w_bat_current > 0xFFEC)) &&
                ((cb_energy_flow_value_0 == (uint8_t)((w_energy_flow_gather & cw_energy_flow_bit2_3) >> 2))||
                (cb_energy_flow_value_0 == (uint8_t)((w_energy_flow_gather & cw_energy_flow_bit0_1) >> 0))))
            {
                
                dw_system_energy_flow_gather |= (cb_energy_flow_value_1 << 21);
            }
					
			if(cb_energy_flow_value_1 == (uint8_t)((dw_system_energy_flow_gather & cw_energy_flow_bit6_7) >> 6) &&\
				(cb_energy_flow_value_2 == (uint8_t)(dw_system_energy_flow_gather >> 19) ||\
				cb_energy_flow_value_2 == (uint8_t)(dw_system_energy_flow_gather >> 17)))
			{
				dw_system_energy_flow_gather |= (cb_energy_flow_value_1 << 21);
			}
        }
        {
			if((0x8000 == (st_power_current_data.w_bat_current & 0x8000)) && (st_power_current_data.w_bat_current <= 0xFFEC))
			{
				dw_system_energy_flow_gather |= (cb_energy_flow_value_1 << 21);
			}

		}

        if(0 != (dw_system_energy_flow_gather & 0x00664000))
        {   //acout port
            uint16_t w_p_load_temp = 0;

            w_p_load_temp =\
              (0 == (st_power_current_data.w_uvw_acload_power_p_sum & 0x8000))?\
                st_power_current_data.w_uvw_acload_power_p_sum:(~st_power_current_data.w_uvw_acload_power_p_sum + 1);				//把acload功率负数转化为正数并赋值
            if((b_acin2_exist == 0) &&(0 == (st_power_current_data.w_gen_power_p_sum & 0x8000)) &&\
               (st_power_current_data.w_gen_power_p_sum >= (w_p_load_temp + 100)) &&\
               ((0x00400000 == (dw_system_energy_flow_gather & 0x00600000)) ||\
                (0x00040000 == (dw_system_energy_flow_gather & 0x00060000))||\
                (cb_energy_flow_value_1 == (uint8_t)((dw_system_energy_flow_gather & cw_energy_flow_bit14_15) >> 14))))   //out -> inv
            {
                dw_system_energy_flow_gather |= (cb_energy_flow_value_1 << 19);
            }
            else if(((b_acin2_exist == 0) && (0 == (st_power_current_data.w_gen_power_p_sum & 0x8000)) &&\
                    ((st_power_current_data.w_gen_power_p_sum + 100) <= w_p_load_temp) &&   									//原这里使用&& 
                    ((0x00200000 == (dw_system_energy_flow_gather & 0x00600000)) ||												//Bit21~22 	bat状态：1能量线条流入
                     (0x00020000 == (dw_system_energy_flow_gather & 0x00060000)))||												//Bit17~18  acin状态：1能量线条流入
                     (cb_energy_flow_value_2 == (uint8_t)((dw_system_energy_flow_gather & cw_energy_flow_bit2_3) >> 2))||		//Bit2~3	交流输出状态：2系统 ---->交流输出2
                     (cb_energy_flow_value_1 == (uint8_t)((dw_system_energy_flow_gather & cw_energy_flow_bit6_7) >> 6))) ||		//bit6~7	光伏充电器能量状态：1充电器---->系统
                     ((b_acin2_exist == 1) && (w_p_load_temp >= 100))) //inv -> out
            {
                dw_system_energy_flow_gather |= (cb_energy_flow_value_2 << 19);
            }
        }
		else if(0 == (dw_system_energy_flow_gather & 0x00664000))
		{//acout port
            uint16_t w_p_load_temp = 0;

            w_p_load_temp =\
              (0 == (st_power_current_data.w_uvw_acload_power_p_sum & 0x8000))?\
                st_power_current_data.w_uvw_acload_power_p_sum:(~st_power_current_data.w_uvw_acload_power_p_sum + 1);
            if((0 == (st_power_current_data.w_gen_power_p_sum & 0x8000)) &&\
               (st_power_current_data.w_gen_power_p_sum >= (w_p_load_temp + 100)) &&\
               ((0x00400000 == (dw_system_energy_flow_gather & 0x00600000)) ||\
                (0x00040000 == (dw_system_energy_flow_gather & 0x00060000))||\
                (cb_energy_flow_value_1 == (uint8_t)((dw_system_energy_flow_gather & cw_energy_flow_bit14_15) >> 14))))   //out -> inv
            {
                dw_system_energy_flow_gather |= (cb_energy_flow_value_1 << 19);
            }

		}
    }
    {
        uint16_t w_data_len = 4;
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x0124,
                               &w_data_len,
                               (uint8_t *)&dw_system_energy_flow_gather,
                               cb_device_data_access_operate_type_write);
    }
#if( task_runing_status_print )
    DebugPrintf(TRUE,"Energy_flow_gather_end\r\n");
#endif
}

/************************************************************************************************
* Input: void
*
* Output: void
*
* Logic discription: ck inverter data process
*                    power,u,i
*
* run time:
************************************************************************************************/
void system_ck_type_power_current_count()
{
    net_device_info *pst_net_device_info = 0;
    const net_data_info *pst_data_info = 0;
    uint16_t w_data_temp_1 = 0;
    uint16_t w_data_temp_2 = 0;
    uint16_t w_data_temp_3 = 0;
    //uint16_t w_energy_flow_gather = 0;
    uint16_t w_bat_cur = 0;

#if( task_runing_status_print )
    DebugPrintf(TRUE,"Ck_power_process_start\r\n");
#endif

    //bat v / bat i
    get_ck_inverter_data(0,1,0x010E,&st_power_current_data.w_bat_votage);
    if(check_all_in_one(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1]))
    {
        get_ck_inverter_data(0,9,0x0126,&st_power_current_data.w_bat_current);
    }
    else
    {
        get_ck_inverter_data(0,1,0x0126,&st_power_current_data.w_bat_current);
    }

//    if(0x8000 == (st_power_current_data.w_bat_current & 0x8000))
//    {
//        st_power_current_data.w_bat_power =\
//            st_power_current_data.w_bat_votage * ((~st_power_current_data.w_bat_current + 1) / 10);
//    }
//    else
//    {
//        st_power_current_data.w_bat_power =\
//            (st_power_current_data.w_bat_votage / 1000) *
//            (st_power_current_data.w_bat_current / 10);
//    }

    //bat temp
    get_ck_inverter_data(0,1,0x0134,&st_power_current_data.w_bat_temp);

    //bat type
    uint8_t b_bms_type = 0;
    if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
                                    (net_device_info **)&pst_net_device_info,
                                    &st_net_system_info_list))
    {
        return;
    }
    if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                         0x0003,
                                         (const net_data_info **)&pst_data_info,
                                         pst_net_device_info))
    {
        return;
    }
    b_bms_type = *pst_data_info->pb_data;

    if(bms == b_bms_type)
    {
        st_power_current_data.b_bat_type = 20;
    }
    else
    {
        if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
                                        (net_device_info **)&pst_net_device_info,
                                        &st_net_system_info_list))
        {
            return;
        }
        if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                             0x0050,
                                             (const net_data_info **)&pst_data_info,
                                             pst_net_device_info))
        {
            return;
        }

        st_power_current_data.b_bat_type = *pst_data_info->pb_data;
    }
    uint16_t w_data_len = 2;
    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                       cb_net_data_att_status,
                       0x0133,
                       &w_data_len,
                       (uint8_t *)&w_data_temp_1,                                       //获取BMS告警信息
                       cb_device_data_access_operate_type_read);
    if(w_data_temp_1 & 0x0080)                                                          //判断BMS告警信息是否为57电池失联告警
    {
        //bat soc energyStatus gather
        st_power_current_data.w_bat_soc_gather = -1;
        st_power_current_data.b_bat_soc = (uint8_t)(st_power_current_data.w_bat_soc_gather);
        st_power_current_data.b_bat_soh = (uint8_t)(st_power_current_data.w_bat_soc_gather >> 8);

    }
    else
    {
    	//bat soc energyStatus gather
        get_ck_inverter_data(0,1,0x011B,&st_power_current_data.w_bat_soc_gather);
        st_power_current_data.b_bat_soc = (uint8_t)(st_power_current_data.w_bat_soc_gather);
        st_power_current_data.b_bat_soh = (uint8_t)(st_power_current_data.w_bat_soc_gather >> 8);
    }

    if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                    (net_device_info **)&pst_net_device_info,
                                    &st_net_system_info_list))
    {
        return;
    }
    if(FALSE == net_get_device_data_info(cb_net_data_att_status,
                                         0x0100,
                                         (const net_data_info **)&pst_data_info,
                                         pst_net_device_info))
    {
        return;
    }
										 
	get_ck_inverter_data(0,9,0x0126,&w_bat_cur);
	if(0 == (w_bat_cur & 0x8000))
	{
		st_power_current_data.w_bat_energy_gather = 2;
	}
	else
	{
		st_power_current_data.w_bat_energy_gather = 1;

	}
/*    st_power_current_data.w_bat_energy_gather =\
        (((((uint16_t)*pst_data_info->pb_data) | ((uint16_t)*(pst_data_info->pb_data + 1) << 8)) &\
        0x0030) >> 4);*/								 

    //max_output_power
    //mark

    //mpptoutput power
    get_mppt_data_4_bytes(0x0104,&st_power_current_data.dw_mppt_output_power_total);
    get_mppt_data_4_bytes(0x0107,&st_power_current_data.dw_mppt_output_current);//mark

    //acin power
    get_ck_inverter_data(0,3,0x0104,&st_power_current_data.w_u_acin_power_sum);
    get_ck_inverter_data(3,3,0x0104,&st_power_current_data.w_v_acin_power_sum);
    get_ck_inverter_data(6,3,0x0104,&st_power_current_data.w_w_acin_power_sum);
	if(check_system_type(cb_inv_type_riio_sun) || check_system_type(cb_inv_type_ck) || check_system_type(cb_inv_type_apollo_maxx))		//RiiO SUN I\MAXX和CK不允许馈网，屏蔽ACin负数功率
	{
	    st_power_current_data.w_u_acin_power_sum = negative_to_zero(st_power_current_data.w_u_acin_power_sum);
    	st_power_current_data.w_v_acin_power_sum = negative_to_zero(st_power_current_data.w_v_acin_power_sum);
    	st_power_current_data.w_w_acin_power_sum = negative_to_zero(st_power_current_data.w_w_acin_power_sum);
	}

    st_power_current_data.w_uvw_acin_power_sum =
        st_power_current_data.w_u_acin_power_sum +\
        st_power_current_data.w_v_acin_power_sum +\
        st_power_current_data.w_w_acin_power_sum;

    //acin current
    get_ck_inverter_data(0,3,0x010B,&st_power_current_data.w_u_acin_current);
    get_ck_inverter_data(3,3,0x010B,&st_power_current_data.w_v_acin_current);
    get_ck_inverter_data(6,3,0x010B,&st_power_current_data.w_w_acin_current);
	if(check_system_type(cb_inv_type_riio_sun) || check_system_type(cb_inv_type_ck) || check_system_type(cb_inv_type_apollo_maxx))		//RiiO SUN I\MAXX和CK不允许馈网，屏蔽ACin负数电流
	{
		st_power_current_data.w_u_acin_current = negative_to_zero(st_power_current_data.w_u_acin_current);
		st_power_current_data.w_v_acin_current = negative_to_zero(st_power_current_data.w_v_acin_current);
		st_power_current_data.w_w_acin_current = negative_to_zero(st_power_current_data.w_w_acin_current);
	}

    //acload power(P)(S)
    //u
    get_ck_inverter_data(0,3,0x0106,&w_data_temp_1);//P表示有功功率
    get_ck_inverter_data(0,3,0x0107,&w_data_temp_2);//S代表视在功率，也就是电源所提供的容量，它为有功功率和无功功率相量叠加
    w_data_temp_3 = (uint16_t)((uint16_t)w_data_temp_2 - negtive_to_positive((uint16_t)w_data_temp_1));
    w_data_temp_3 = (uint16_t)(negtive_to_positive((uint16_t)w_data_temp_3));

    uint8_t b_solor_mode = na;
    if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
                                    (net_device_info **)&pst_net_device_info,
                                    &st_net_system_info_list))
    {
        return;
    }
    if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                         0x0000,
                                         (const net_data_info **)&pst_data_info,
                                         pst_net_device_info))
    {
        return;
    }

    b_solor_mode = *pst_data_info->pb_data;
    if((ac_couple == b_solor_mode) ||\
       (ac_dc_couple == b_solor_mode))
    {
        w_data_temp_1 += st_pv_data_summary.w_pv_power_p_u;//pv act powerout   //mark
    }
    st_power_current_data.w_u_acload_power_p_sum = w_data_temp_1;
	st_power_current_data.w_u_acload_power_p_sum = negative_to_zero(st_power_current_data.w_u_acload_power_p_sum); 
    st_power_current_data.w_u_acload_power_s_sum = w_data_temp_3 + w_data_temp_1;
    st_power_current_data.w_u_acload_power_s_sum = negative_to_zero(st_power_current_data.w_u_acload_power_s_sum);

    //v
    get_ck_inverter_data(3,3,0x0106,&w_data_temp_1);//P
    get_ck_inverter_data(3,3,0x0107,&w_data_temp_2);//S
    w_data_temp_3 = (uint16_t)((uint16_t)w_data_temp_2 - negtive_to_positive((uint16_t)w_data_temp_1));

    if((ac_couple == b_solor_mode) ||\
       (ac_dc_couple == b_solor_mode))
    {
        w_data_temp_1 += st_pv_data_summary.w_pv_power_p_v;//pv act powerout
    }
    st_power_current_data.w_v_acload_power_p_sum = w_data_temp_1;
	st_power_current_data.w_v_acload_power_p_sum = negative_to_zero(st_power_current_data.w_v_acload_power_p_sum); 
    st_power_current_data.w_v_acload_power_s_sum = w_data_temp_3 + w_data_temp_1;
    st_power_current_data.w_v_acload_power_s_sum = negative_to_zero(st_power_current_data.w_v_acload_power_s_sum);

    //w
    get_ck_inverter_data(6,3,0x0106,&w_data_temp_1);//P
    get_ck_inverter_data(6,3,0x0107,&w_data_temp_2);//S
    w_data_temp_3 = (uint16_t)((uint16_t)w_data_temp_2 - negtive_to_positive((uint16_t)w_data_temp_1));

    if((ac_couple == b_solor_mode) ||\
       (ac_dc_couple == b_solor_mode))
    {
        w_data_temp_1 += st_pv_data_summary.w_pv_power_p_w;//pv act powerout
    }
    st_power_current_data.w_w_acload_power_p_sum = w_data_temp_1;
	st_power_current_data.w_w_acload_power_p_sum = negative_to_zero(st_power_current_data.w_w_acload_power_p_sum); 
    st_power_current_data.w_w_acload_power_s_sum = w_data_temp_3 + w_data_temp_1;
    st_power_current_data.w_w_acload_power_s_sum = negative_to_zero(st_power_current_data.w_w_acload_power_s_sum);
    
    st_power_current_data.w_uvw_acload_power_p_sum =\
        st_power_current_data.w_u_acload_power_p_sum +\
        st_power_current_data.w_v_acload_power_p_sum +\
        st_power_current_data.w_w_acload_power_p_sum;

    //acload current
    get_ck_inverter_data(0,3,0x010D,&st_power_current_data.w_u_acload_current_sum);
    get_ck_inverter_data(3,3,0x010D,&st_power_current_data.w_v_acload_current_sum);
    get_ck_inverter_data(6,3,0x010D,&st_power_current_data.w_w_acload_current_sum);

    //inverter power
    get_ck_inverter_data(0,3,0x0105,&st_power_current_data.w_u_inverter_power_sum);
    get_ck_inverter_data(3,3,0x0105,&st_power_current_data.w_v_inverter_power_sum);
    get_ck_inverter_data(6,3,0x0105,&st_power_current_data.w_w_inverter_power_sum);

    //inberter current
    get_ck_inverter_data(0,3,0x010C,&st_power_current_data.w_u_inverter_current);
    get_ck_inverter_data(3,3,0x010C,&st_power_current_data.w_v_inverter_current);
    get_ck_inverter_data(6,3,0x010C,&st_power_current_data.w_w_inverter_current);

    //solor output power
    st_power_current_data.dw_solor_output_power =\
        st_power_current_data.dw_mppt_output_power_total +\
        st_pv_data_summary.w_pv_power_p_total;
    
    energy_flow_gather();
    

    
    net_write_energy_data(0,cb_power_current_data_num,(energy_data_info *)&st_energy_data_info[0]);

    {   //增加电池通讯离线状态
        uint16_t w_data_len = 2;
        uint16_t w_data_temp_1 = 0;
        uint16_t w_data_temp_2 = 0;
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
                               cb_net_data_att_set,
                               0x0050,
                               &w_data_len,
                               (uint8_t *)&w_data_temp_1,                                       //获取电池类型（6：为锂电池）
                               cb_device_data_access_operate_type_read);
        if(w_data_temp_1 == 6)
        {
            net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                               cb_net_data_att_status,
                               0x0133,
                               &w_data_len,
                               (uint8_t *)&w_data_temp_1,                                       //获取BMS告警信息
                               cb_device_data_access_operate_type_read);
            if(w_data_temp_1 & 0x0080)                                                          //判断BMS告警信息是否为57电池失联告警
            {
                  w_data_temp_2 = 3;                                                            //BatteryEnergyStatusGather 3：电池通讯失联
                  net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                       cb_net_data_att_status,
                                       0x011B,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_2,
                                       cb_device_data_access_operate_type_write);
            }
        }
    }

#if( task_runing_status_print )
    DebugPrintf(TRUE,"Ck_power_process_end\r\n");
#endif

}
/************************************************************************************************
* Input: void
*
* Output: void
*
* Logic discription: riiosun ii type inverter data process
*                    power,u,i...
*
* run time:
************************************************************************************************/
void system_riiosunii_power_current_count()
{
    net_device_info *pst_net_device_info = 0;
    const net_data_info *pst_data_info = 0;
    uint16_t w_data_temp_1 = 0;
    uint16_t w_data_temp_2 = 0;
    uint16_t w_data_temp_3 = 0;
    uint16_t w_data_temp_4 = 0;
    uint16_t w_data_len = 0;

#if( task_runing_status_print )
    DebugPrintf(TRUE,"Riiosunii_power_process_start\r\n");
#endif

    //bat v / bat i
    get_ck_inverter_data(0,1,0x0114,&st_power_current_data.w_bat_votage);
   if(check_all_in_one(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1]) 
   	|| check_system_type(cb_inv_type_ck_ii) || check_system_type(cb_inv_type_Tyrann_3_5_8k)  || check_system_type(cb_inv_type_tyrann)
   	|| check_system_type(cb_inv_type_apollo_matric_ii_lite) || check_system_type(cb_inv_type_apollo_matric_ii_5_8k)
   	|| check_system_type(cb_inv_type_riio_ii) || check_system_type(cb_inv_type_riio_ii_split_phase) )
   {
       net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
                               cb_net_data_att_set,
                               0x0050,
                               &w_data_len,
                               (uint8_t *)&w_data_temp_1,                                       //获取电池类型（6：为锂电池）
                               cb_device_data_access_operate_type_read);
		if(6 == w_data_temp_1 || g_parallel_system_mode == 0)
		{
			get_ck_inverter_data(0,1,0x0117,&st_power_current_data.w_bat_current);				//split phase 为9台和
		}
		else
		{
			get_ck_inverter_data(0,9,0x0117,&st_power_current_data.w_bat_current);
		}
   }
   else
   {
       get_ck_inverter_data(0,1,0x0117,&st_power_current_data.w_bat_current);
   }
	

    //bat power / bat i
    {
        uint16_t w_data_len = 2;
        for(uint8_t b_cnt_i = 0;b_cnt_i < 9;b_cnt_i++)
        {
            get_ck_inverter_data(b_cnt_i,1,0x0115,&w_data_temp_2);				//电流 0.1A 充电为正放电为负
            get_ck_inverter_data(b_cnt_i,1,0x0114,&w_data_temp_3);				//bat电压 0.001V
            net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_mppt_u1_0 + b_cnt_i * 6],
                                   cb_net_data_att_status,
                                   0x0102,
                                   &w_data_len,
                                   (uint8_t *)&w_data_temp_1,
                                   cb_device_data_access_operate_type_read);

            w_data_temp_2 = w_data_temp_2 + w_data_temp_1 / 10;					//电池端电流+mppt输出电流
            w_data_temp_4 =\
                (((int16_t)w_data_temp_2 * (int32_t)w_data_temp_3) / 10000);//mv * 0.1a = w		电池和mppt总输出功率

            net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1 + b_cnt_i],
                                   cb_net_data_att_status,
                                   0x0106,
                                   &w_data_len,
                                   (uint8_t *)&w_data_temp_2,
                                   cb_device_data_access_operate_type_write);
            net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1 + b_cnt_i],
                                   cb_net_data_att_status,
                                   0x0105,
                                   &w_data_len,
                                   (uint8_t *)&w_data_temp_4,
                                   cb_device_data_access_operate_type_write);
        }
    }

    //bat temp
    get_ck_inverter_data(0,1,0x011E,&st_power_current_data.w_bat_temp);

    //bat type
    uint8_t b_bms_type = 0;
    if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
                                    (net_device_info **)&pst_net_device_info,
                                    &st_net_system_info_list))
    {
        return;
    }
    if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                         0x0003,
                                         (const net_data_info **)&pst_data_info,
                                         pst_net_device_info))
    {
        return;
    }
    b_bms_type = *pst_data_info->pb_data;

    if(bms == b_bms_type)
    {
        st_power_current_data.b_bat_type = 20;
    }
    else
    {
        if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
                                        (net_device_info **)&pst_net_device_info,
                                        &st_net_system_info_list))
        {
            return;
        }
        if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                             0x0050,                                        //电池类型
                                             (const net_data_info **)&pst_data_info,
                                             pst_net_device_info))
        {
            return;
        }

        st_power_current_data.b_bat_type = *pst_data_info->pb_data;
    }

    //bat soc energyStatus gather
    get_ck_inverter_data(0,1,0x014A,&st_power_current_data.w_bat_soc_gather);
    st_power_current_data.b_bat_soc = (uint8_t)(st_power_current_data.w_bat_soc_gather);
    st_power_current_data.b_bat_soh = (uint8_t)(st_power_current_data.w_bat_soc_gather >> 8);

    if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                    (net_device_info **)&pst_net_device_info,
                                    &st_net_system_info_list))
    {
        return;
    }
    if(FALSE == net_get_device_data_info(cb_net_data_att_status,
                                         0x0100,                                        //能量流状态字
                                         (const net_data_info **)&pst_data_info,
                                         pst_net_device_info))
    {
        return;
    }

    if(st_power_current_data.w_bat_current >= 20)
    {
        st_power_current_data.w_bat_energy_gather =\
            (((((uint16_t)*pst_data_info->pb_data) | ((uint16_t)*(pst_data_info->pb_data + 1) << 8)) &\
            0x0030) >> 4);
//		st_power_current_data.w_bat_energy_gather = 2;

    }
//	else if(1 == (st_power_current_data.w_bat_current & 0x8000) && st_power_current_data.w_bat_current < 0xFFEC)
//    {
//		st_power_current_data.w_bat_energy_gather = 1;
//    }
    else
    {
        st_power_current_data.w_bat_energy_gather = 0;
    }

    //firmware ver
    {
        uint16_t w_data_len = 2;

        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                               cb_net_data_att_status,
                               0x012C,
                               &w_data_len,
                               (uint8_t *)&w_data_temp_1,
                               cb_device_data_access_operate_type_read);

		uint8_t b_test_mode = 0;
		uint8_t b_test_hd = 0;
		uint8_t b_test_md = 0;
        w_data_len = 1;
		net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
							   cb_net_data_att_set,
							   0x003F,
							   &w_data_len,
							   (uint8_t *)&b_test_mode,
							   cb_device_data_access_operate_type_read);
		net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
							   cb_net_data_att_set,
							   0x0040,
							   &w_data_len,
							   (uint8_t *)&b_test_hd,
							   cb_device_data_access_operate_type_read);
		net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
							   cb_net_data_att_set,
							   0x0041,
							   &w_data_len,
							   (uint8_t *)&b_test_md,
							   cb_device_data_access_operate_type_read);

		if(0x33 == b_test_mode)
		{
			w_data_temp_1 = b_test_hd << 8 | b_test_md;

		}
        w_data_len = 2;
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_net_data_att_set,
                               0x0039,
                               &w_data_len,
                               (uint8_t *)&w_data_temp_1,
                               cb_device_data_access_operate_type_write);
		 net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_net_data_att_status,
                               0x018E,
                               &w_data_len,
                               (uint8_t *)&w_data_temp_1,
                               cb_device_data_access_operate_type_write);
    }

    //max_output_power
    //mark

    //mpptoutput power
    get_mppt_data_4_bytes(0x0105,&st_power_current_data.dw_mppt_output_power_total);
	st_power_current_data.dw_mppt_output_power_total =  (st_power_current_data.dw_mppt_output_power_total > 100) ? st_power_current_data.dw_mppt_output_power_total : 0;
    get_mppt_data_4_bytes(0x0107,&st_power_current_data.dw_mppt_output_current);//mark

    uint16_t w_normal_load_power = 0;
    uint16_t b_meter_enable = 0;
    uint16_t b_parallel_system_mode = 0;
	net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                               cb_device_data_access_data_type_set,
                               0x0020,
                               &w_data_len,
                               (uint8_t *)&b_parallel_system_mode,                                       //获取系统单并机模式
                               cb_device_data_access_operate_type_read);
	if(check_system_type(cb_inv_type_riio_sun_ii) 
		|| check_system_type(cb_inv_type_riio_sun_ii_gte) 
		|| check_system_type(cb_inv_type_riio_sun_ii_split_phase) 
		|| check_system_type(cb_inv_type_riio_ii_split_phase)
		|| check_system_type(cb_inv_type_riio_ii))
	{

		if(b_parallel_system_mode == 0)
		{
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
								   cb_net_data_att_status,
								   0x0131,
								   &w_data_len,
								   (uint8_t *)&b_meter_enable,
								   cb_device_data_access_operate_type_read);//merter enable
		}
	}
    {
//        if(1 == b_meter_enable)//merter enable
//        {
//            net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_meter_1],
//                                   cb_net_data_att_status,
//                                   0x0112,
//                                   &w_data_len,
//                                   (uint8_t *)&w_data_temp_1,
//                                   cb_device_data_access_operate_type_read);//meter p
//            get_ck_inverter_data(0,3,0x0101,&w_data_temp_2);//w_u_acin_power_sum
//
//            w_normal_load_power = negative_to_zero(w_data_temp_1 - w_data_temp_2);
//        }
//        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
//                               cb_net_data_att_status,
//                               0x0156,
//                               &w_data_len,
//                               (uint8_t *)&w_normal_load_power,
//                               cb_device_data_access_operate_type_write);//normal load p
    }

    uint16_t b_ct_ctrl_mode = 0;
	uint16_t b_ct_iac_ext = 0;
    {
        uint16_t w_data_len = 2;
		if(FALSE == check_system_type(cb_inv_type_riio_sun_ii) 
			&& FALSE == check_system_type(cb_inv_type_riio_sun_ii_gte) 
			&& FALSE == check_system_type(cb_inv_type_riio_sun_ii_split_phase) 
			&& FALSE == check_system_type(cb_inv_type_riio_ii_split_phase)
			&& FALSE == check_system_type(cb_inv_type_riio_ii))
		{
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
								   cb_device_data_access_data_type_set,
								   0x0034,														//b_ct_ctrl_mode 	0-Display Only 1-Zero Export to 2-AC Cur Limit 3-Feedback Ctrl
								   &w_data_len,
								   (uint8_t *)&b_ct_ctrl_mode,
								   cb_device_data_access_operate_type_read);
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
								   cb_device_data_access_data_type_set,
								   0x0033,														//b_ct_ctrl_mode 	0-Display Only 1-Zero Export to 2-AC Cur Limit 3-Feedback Ctrl
								   &w_data_len,
								   (uint8_t *)&b_ct_iac_ext,
								   cb_device_data_access_operate_type_read);

		}
    }


	uint16_t w_v_normal_load_power = 0;
	uint16_t w_w_normal_load_power = 0;
	uint16_t w_uvw_normal_load_power = 0;

//    if(3 == b_ct_ctrl_mode && 0 != b_ct_iac_ext &&(check_system_type(cb_inv_type_ck_ii) || check_system_type(cb_inv_type_tyrann)))//merter enable  有接电表，使用电表数据作为u项的电流功率数据
	if(0 != b_ct_iac_ext && (FALSE == check_system_type(cb_inv_type_riio_sun_ii) 
		&& FALSE == check_system_type(cb_inv_type_riio_sun_ii_gte) 
		&& FALSE == check_system_type(cb_inv_type_riio_sun_ii_split_phase) 
		&& FALSE == check_system_type(cb_inv_type_riio_ii_split_phase)
		&& FALSE == check_system_type(cb_inv_type_riio_ii)))		//merter enable  有接电表，使用电表数据作为u项的电流功率数据
	{
		uint16_t w_data_len = 2;
		uint16_t w_data_temp = 0;
		uint16_t w_u_ext_ct_power = 0;
		uint16_t w_v_ext_ct_power = 0;
		uint16_t w_w_ext_ct_power = 0;
//		uint16_t w_v_normal_load_power = 0;
//		uint16_t w_w_normal_load_power = 0;
//		uint16_t w_uvw_normal_load_power = 0;
		uint16_t w_u_ext_ct_current = 0;
		uint16_t w_v_ext_ct_current = 0;
		uint16_t w_w_ext_ct_current = 0;
		uint16_t w_v_normal_load_current = 0;
		uint16_t w_w_normal_load_current = 0;
		uint16_t w_u_acin_power = 0;
		uint16_t w_v_acin_power = 0;
		uint16_t w_w_acin_power = 0;
		uint16_t w_u_acin_current = 0;
		uint16_t w_v_acin_current = 0;
		uint16_t w_w_acin_current = 0;
		//前端载u项功率
		net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
							   cb_device_data_access_data_type_status,
							   0x011D,
							   &w_data_len,
							   (uint8_t *)&w_u_ext_ct_power,
							   cb_device_data_access_operate_type_read);//CT功率
		st_power_current_data.w_u_acin_power_sum = w_u_ext_ct_power;   
		get_ck_inverter_data(0,3,0x0101,&w_u_acin_power);//w_u_acin_power_sum
		
		w_normal_load_power = negative_to_zero(w_u_ext_ct_power - w_u_acin_power);
		net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
							   cb_net_data_att_status,
							   0x0156,
							   &w_data_len,
							   (uint8_t *)&w_normal_load_power,
							   cb_device_data_access_operate_type_write);//normal load p

		if(2 == b_parallel_system_mode)
		{
			//前端载v项功率
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_v1],
								  cb_device_data_access_data_type_status,
								  0x011D,
								  &w_data_len,
								  (uint8_t *)&w_v_ext_ct_power,
								  cb_device_data_access_operate_type_read);//CT功率 v
			st_power_current_data.w_v_acin_power_sum = w_v_ext_ct_power;   
			get_ck_inverter_data(3,3,0x0101,&w_v_acin_power);//w_v_acin_power_sum

			w_v_normal_load_power = negative_to_zero(w_v_ext_ct_power - w_v_acin_power);
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
								  cb_net_data_att_status,
								  0x0177,
								  &w_data_len,
								  (uint8_t *)&w_v_normal_load_power,
								  cb_device_data_access_operate_type_write);//normal load p v
			st_power_current_data.w_normalload_L1_power = w_v_normal_load_power;

			//前端载w项功率
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_w1],
								 cb_device_data_access_data_type_status,
								 0x011D,
								 &w_data_len,
								 (uint8_t *)&w_w_ext_ct_power,
								 cb_device_data_access_operate_type_read);//CT功率 w
			st_power_current_data.w_w_acin_power_sum = w_w_ext_ct_power;	 
			get_ck_inverter_data(6,3,0x0101,&w_w_acin_power);//w_w_acin_power_sum

			w_w_normal_load_power = negative_to_zero(w_w_ext_ct_power - w_w_acin_power);
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
								 cb_net_data_att_status,
								 0x0178,
								 &w_data_len,
								 (uint8_t *)&w_w_normal_load_power,
								 cb_device_data_access_operate_type_write);//normal load p w
			st_power_current_data.w_normalload_L2_power = w_w_normal_load_power;

		}
		
	  w_uvw_normal_load_power = w_normal_load_power + w_v_normal_load_power + w_w_normal_load_power;
	  w_uvw_normal_load_power =  (w_uvw_normal_load_power > 100) ?w_uvw_normal_load_power : 0;
	  
  	  net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
						 cb_net_data_att_status,
						 0x017D,
						 &w_data_len,
						 (uint8_t *)&w_uvw_normal_load_power,
						 cb_device_data_access_operate_type_write);//normal load p w
//		net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
//							   cb_device_data_access_data_type_status,
//							   0x011D,
//							   &w_data_len,
//							   (uint8_t *)&st_power_current_data.w_u_acin_power_sum,
//							   cb_device_data_access_operate_type_read);//CT功率
		//前端载u项电流
		net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
							  cb_device_data_access_data_type_status,
							  0x011C,
							  &w_data_len,
							  (uint8_t *)&w_u_ext_ct_current, 					   //ct_i ct 端的电流
							  cb_device_data_access_operate_type_read);
		st_power_current_data.w_u_acin_current = w_u_ext_ct_current * 10;

		net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
							 cb_device_data_access_data_type_status,
							 0x010C,
							 &w_data_len,
							 (uint8_t *)&w_u_acin_current,					  //acin_i 逆变器端的电流
							 cb_device_data_access_operate_type_read);

		 //前端载v项电流
		 net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_v1],
							   cb_device_data_access_data_type_status,
							   0x011C,
							   &w_data_len,
							   (uint8_t *)&w_v_ext_ct_current,						//ct_i ct 端的电流
							   cb_device_data_access_operate_type_read);
		 st_power_current_data.w_v_acin_current = w_v_ext_ct_current * 10;
		 
		 net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_v1],
							  cb_device_data_access_data_type_status,
							  0x010C,
							  &w_data_len,
							  (uint8_t *)&w_v_acin_current, 				   //acin_i 逆变器端的电流
							  cb_device_data_access_operate_type_read);

		  //前端载w项电流
		  net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_w1],
								cb_device_data_access_data_type_status,
								0x011C,
								&w_data_len,
								(uint8_t *)&w_w_ext_ct_current, 					 //ct_i ct 端的电流
								cb_device_data_access_operate_type_read);
		  st_power_current_data.w_w_acin_current = w_w_ext_ct_current * 10;
		  
		  net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_w1],
							   cb_device_data_access_data_type_status,
							   0x010C,
							   &w_data_len,
							   (uint8_t *)&w_w_acin_current,					//acin_i 逆变器端的电流
							   cb_device_data_access_operate_type_read);


		if(0x8000 == (st_power_current_data.w_u_acin_power_sum & 0x8000))
		{
			st_power_current_data.w_normalload_current =\
				-(st_power_current_data.w_u_acin_current  - w_u_acin_current);
			st_power_current_data.w_normalload_current = negtive_to_positive(st_power_current_data.w_normalload_current);
			
			st_power_current_data.w_normalload_L1_current =\
				-(st_power_current_data.w_v_acin_current  - w_v_acin_current);
			st_power_current_data.w_normalload_L1_current = negtive_to_positive(st_power_current_data.w_normalload_L1_current);

			st_power_current_data.w_normalload_L2_current =\
				-(st_power_current_data.w_w_acin_current  - w_w_acin_current);
			st_power_current_data.w_normalload_L2_current = negtive_to_positive(st_power_current_data.w_normalload_L2_current);

		}
		else
		{
			st_power_current_data.w_normalload_current =\
				st_power_current_data.w_u_acin_current  - w_u_acin_current;

			st_power_current_data.w_normalload_L1_current =\
				st_power_current_data.w_v_acin_current  - w_v_acin_current;

			st_power_current_data.w_normalload_L2_current =\
				st_power_current_data.w_w_acin_current  - w_w_acin_current;
		}
    }
    else if(1 == b_meter_enable)//merter enable  有接电表，使用电表数据作为u项的电流功率数据
    {
    	if(check_system_type(cb_inv_type_riio_sun_ii) 
			|| check_system_type(cb_inv_type_riio_sun_ii_gte) 
			|| check_system_type(cb_inv_type_riio_ii))
    	{
			uint16_t w_data_len = 2;
			uint16_t w_data_temp = 0;
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_meter_1],
								   cb_net_data_att_status,
								   0x0112,
								   &w_data_len,
								   (uint8_t *)&w_data_temp_1,
								   cb_device_data_access_operate_type_read);//meter p
			get_ck_inverter_data(0,3,0x0101,&w_data_temp_2);//w_u_acin_power_sum
			
			w_normal_load_power = negative_to_zero(w_data_temp_1 - w_data_temp_2);
			w_normal_load_power =  (w_normal_load_power > 100) ?w_normal_load_power : 0;
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
								   cb_net_data_att_status,
								   0x0156,
								   &w_data_len,
								   (uint8_t *)&w_normal_load_power,
								   cb_device_data_access_operate_type_write);//normal load p
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
								   cb_net_data_att_status,
								   0x017D,
								   &w_data_len,
								   (uint8_t *)&w_normal_load_power,
								   cb_device_data_access_operate_type_write);//normal load p		//normal load 总功率
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_meter_1],
								   cb_net_data_att_status,
								   0x0111,
								   &w_data_len,
								   (uint8_t *)&st_power_current_data.w_u_acin_current,
								   cb_device_data_access_operate_type_read);//acin current
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_meter_1],
								   cb_net_data_att_status,
								   0x0112,
								   &w_data_len,
								   (uint8_t *)&st_power_current_data.w_u_acin_power_sum,
								   cb_device_data_access_operate_type_read);//acin power
	
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
								   cb_net_data_att_status,
								   0x010C,
								   &w_data_len,
								   (uint8_t *)&w_data_temp,
								   cb_device_data_access_operate_type_read);//acin current
			if(0x8000 == (st_power_current_data.w_u_acin_power_sum & 0x8000))
			{
				st_power_current_data.w_normalload_current =\
					-st_power_current_data.w_u_acin_current - w_data_temp;
				st_power_current_data.w_u_acin_current = positive_to_negtive(st_power_current_data.w_u_acin_current);
	
			}
			else
			{
				st_power_current_data.w_normalload_current =\
					st_power_current_data.w_u_acin_current - w_data_temp;
			}
		}
		else if(check_system_type(cb_inv_type_riio_sun_ii_split_phase) || check_system_type(cb_inv_type_riio_ii_split_phase))
		{	
			uint16_t w_data_len = 2;
			uint16_t w_acin_power_sum = 0;
			uint16_t w_acin_L1_power_sum = 0;
			uint16_t w_acin_L2_power_sum = 0;
			uint16_t w_acin_current = 0;
			uint16_t w_acin_L1_current = 0;
			uint16_t w_acin_L2_current = 0;
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_meter_1],
								   cb_net_data_att_status,
								   0x0202,
								   &w_data_len,
								   (uint8_t *)&st_power_current_data.w_uvw_acin_power_sum,							//电表总有功功率
								   cb_device_data_access_operate_type_read);//meter p
			get_ck_inverter_data(0,3,0x0101,&w_acin_power_sum);//w_u_acin_power_sum
			w_normal_load_power = negative_to_zero(st_power_current_data.w_uvw_acin_power_sum - w_acin_power_sum);	//normal_load L1-L2功率即总功率
			w_normal_load_power =  (w_normal_load_power > 100) ?w_normal_load_power : 0;
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_meter_1],
								   cb_net_data_att_status,
								   0x0209,
								   &w_data_len,
								   (uint8_t *)&st_power_current_data.w_u_acin_power_sum ,							//电表L1有功功率
								   cb_device_data_access_operate_type_read);//meter p
			get_ck_inverter_data(0,3,0x0200,&w_acin_L1_power_sum);//w_u_acin_power_sum
			st_power_current_data.w_normalload_L1_power = negative_to_zero(st_power_current_data.w_u_acin_power_sum  - w_acin_L1_power_sum);	//normal_load L1功率

			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_meter_1],
								   cb_net_data_att_status,
								   0x020F,
								   &w_data_len,
								   (uint8_t *)&st_power_current_data.w_v_acin_power_sum,							//电表L2有功功率
								   cb_device_data_access_operate_type_read);//meter p
			get_ck_inverter_data(0,3,0x0205,&w_acin_L2_power_sum);//w_u_acin_power_sum
			st_power_current_data.w_normalload_L2_power = negative_to_zero(st_power_current_data.w_v_acin_power_sum - w_acin_L2_power_sum);	//normal_load L2功率
			
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
								   cb_net_data_att_status,
								   0x0156,
								   &w_data_len,
								   (uint8_t *)&w_normal_load_power,
								   cb_device_data_access_operate_type_write);//normal load p		//normal load 总功率
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
								   cb_net_data_att_status,
								   0x017D,
								   &w_data_len,
								   (uint8_t *)&w_normal_load_power,
								   cb_device_data_access_operate_type_write);//normal load p		//normal load 总功率
//			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
//								   cb_net_data_att_status,
//								   0x0177,
//								   &w_data_len,
//								   (uint8_t *)&st_power_current_data.w_normalload_L1_power,
//								   cb_device_data_access_operate_type_write);//normal load p		//Normal load L1功率
//			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
//								   cb_net_data_att_status,
//								   0x0178,
//								   &w_data_len,
//								   (uint8_t *)&st_power_current_data.w_normalload_L2_power,
//								   cb_device_data_access_operate_type_write);//normal load p		//Normal load L2功率
/*********************************************************************************************************/
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_meter_1],
								   cb_net_data_att_status,
								   0x0202,
								   &w_data_len,
								   (uint8_t *)&st_power_current_data.w_uvw_acin_power_sum,		//存在meter时使用meter的功率
								   cb_device_data_access_operate_type_read);//acin power
	
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_meter_1],
								  cb_net_data_att_status,
								  0x0201,
								  &w_data_len,
								  (uint8_t *)&st_power_current_data.w_w_acin_current,				//sp时acin L1-L2总电流  使用meter的电流
								  cb_device_data_access_operate_type_read);//acin current			//
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
								  cb_net_data_att_status,
								  0x010C,
								  &w_data_len,
								  (uint8_t *)&w_acin_current,
								  cb_device_data_access_operate_type_read);//acin current

			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_meter_1],
								  cb_net_data_att_status,
								  0x0208,
								  &w_data_len,
								  (uint8_t *)&st_power_current_data.w_u_acin_current,
								  cb_device_data_access_operate_type_read);//acin current
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
								  cb_net_data_att_status,
								  0x021C,
								  &w_data_len,
								  (uint8_t *)&w_acin_L1_current,
								  cb_device_data_access_operate_type_read);//acin current

			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_meter_1],
								  cb_net_data_att_status,
								  0x020E,
								  &w_data_len,
								  (uint8_t *)&st_power_current_data.w_v_acin_current,
								  cb_device_data_access_operate_type_read);//acin current
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
								  cb_net_data_att_status,
								  0x0221,
								  &w_data_len,
								  (uint8_t *)&w_acin_L2_current,
								  cb_device_data_access_operate_type_read);//acin current



			if(0x8000 == (st_power_current_data.w_uvw_acin_power_sum & 0x8000))
			{
//				st_power_current_data.w_normalload_current =\
//					-st_power_current_data.w_w_acin_current - w_acin_current;
				
				st_power_current_data.w_normalload_L1_current =\
					-st_power_current_data.w_u_acin_current - w_acin_L1_current;
				st_power_current_data.w_normalload_L1_current = negative_to_zero(st_power_current_data.w_normalload_L1_current);		//屏蔽normalload负数电流
				
				st_power_current_data.w_normalload_L2_current =\
					-st_power_current_data.w_v_acin_current - w_acin_L2_current;
				st_power_current_data.w_normalload_L2_current = negative_to_zero(st_power_current_data.w_normalload_L2_current);
				
				st_power_current_data.w_normalload_current = (st_power_current_data.w_normalload_L1_current + st_power_current_data.w_normalload_L2_current) / 2 +
					(st_power_current_data.w_normalload_L1_current + st_power_current_data.w_normalload_L2_current) % 2;
				
				st_power_current_data.w_w_acin_current = (st_power_current_data.w_u_acin_current + st_power_current_data.w_v_acin_current) / 2 +  
					(st_power_current_data.w_u_acin_current + st_power_current_data.w_v_acin_current) % 2;
				st_power_current_data.w_u_acin_current = positive_to_negtive(st_power_current_data.w_u_acin_current);
				st_power_current_data.w_v_acin_current = positive_to_negtive(st_power_current_data.w_v_acin_current);
				st_power_current_data.w_w_acin_current = positive_to_negtive(st_power_current_data.w_w_acin_current);
			}
			else
			{
//				st_power_current_data.w_normalload_current =\
//					st_power_current_data.w_w_acin_current - w_acin_current;
				
				st_power_current_data.w_normalload_L1_current =\
					st_power_current_data.w_u_acin_current - w_acin_L1_current;
				st_power_current_data.w_normalload_L1_current = negative_to_zero(st_power_current_data.w_normalload_L1_current);
				
				st_power_current_data.w_normalload_L2_current =\
					st_power_current_data.w_v_acin_current - w_acin_L2_current;
				st_power_current_data.w_normalload_L2_current = negative_to_zero(st_power_current_data.w_normalload_L2_current);

				st_power_current_data.w_normalload_current = (st_power_current_data.w_normalload_L1_current + st_power_current_data.w_normalload_L2_current) / 2 +
					(st_power_current_data.w_normalload_L1_current + st_power_current_data.w_normalload_L2_current) % 2;
				
				st_power_current_data.w_w_acin_current = (st_power_current_data.w_u_acin_current + st_power_current_data.w_v_acin_current) / 2 +  
					(st_power_current_data.w_u_acin_current + st_power_current_data.w_v_acin_current) % 2;
			}
		}
        
    }
    else
    {
    	uint16_t w_uvw_normal_load_power = 0;
    	st_power_current_data.w_normalload_L1_power = 0;
		st_power_current_data.w_normalload_L2_power = 0;
		st_power_current_data.w_normalload_current = 0;
		st_power_current_data.w_normalload_L1_current = 0;
		st_power_current_data.w_normalload_L2_current = 0;
		net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
						 cb_net_data_att_status,
						 0x017D,
						 &w_data_len,
						 (uint8_t *)&w_uvw_normal_load_power,
						 cb_device_data_access_operate_type_write);//normal load p w
        get_ck_inverter_data(0,3,0x010C,&st_power_current_data.w_u_acin_current);
        get_ck_inverter_data(0,3,0x0101,&st_power_current_data.w_u_acin_power_sum);
		if(check_system_type(cb_inv_type_riio_sun_ii_split_phase) || check_system_type(cb_inv_type_riio_ii_split_phase))
		{
			get_ck_inverter_data(0,3,0x021C,&st_power_current_data.w_u_acin_current);			//0034000107	w_u_acin_current
			get_ck_inverter_data(0,3,0x0200,&st_power_current_data.w_u_acin_power_sum);			//0034000104	w_u_acin_power_sum
		}
    }
	//gen power p-sum

	//w_gen_power_p_sum 0034000154

	if(check_system_type(cb_inv_type_riio_sun_ii_split_phase) || check_system_type(cb_inv_type_riio_ii_split_phase))
	{
	    get_ck_inverter_data(0,3,0x0204,&st_power_current_data.w_u_gen_power_p_sum);
   		get_ck_inverter_data(0,3,0x0209,&st_power_current_data.w_v_gen_power_p_sum);
		st_power_current_data.w_gen_power_p_sum =\
	        st_power_current_data.w_u_gen_power_p_sum +\
	        st_power_current_data.w_v_gen_power_p_sum;
		st_power_current_data.w_gen_power_p_sum =  (st_power_current_data.w_gen_power_p_sum > 100) ? st_power_current_data.w_gen_power_p_sum : 0;
		//gen i
	    get_ck_inverter_data(0,3,0x010D,&st_power_current_data.w_u_gen_power_i_sum);
	    get_ck_inverter_data(0,3,0x0220,&st_power_current_data.w_v_gen_power_i_sum);
	    get_ck_inverter_data(0,3,0x0225,&st_power_current_data.w_w_gen_power_i_sum);
	}
	else
	{
	    get_ck_inverter_data(0,3,0x0102,&st_power_current_data.w_u_gen_power_p_sum);				//003400016B	w_u_gen_power_p_sum
	    get_ck_inverter_data(3,3,0x0102,&st_power_current_data.w_v_gen_power_p_sum);				//003400016C	w_v_gen_power_p_sum
	    get_ck_inverter_data(6,3,0x0102,&st_power_current_data.w_w_gen_power_p_sum);				//003400016D	w_v_gen_power_p_sum
    
	    st_power_current_data.w_gen_power_p_sum =\
	        st_power_current_data.w_u_gen_power_p_sum +\
	        st_power_current_data.w_v_gen_power_p_sum +\
	        st_power_current_data.w_w_gen_power_p_sum;
		st_power_current_data.w_gen_power_p_sum =  (st_power_current_data.w_gen_power_p_sum > 100) ? st_power_current_data.w_gen_power_p_sum : 0;

		//gen i
	    get_ck_inverter_data(0,3,0x010D,&st_power_current_data.w_u_gen_power_i_sum);
	    get_ck_inverter_data(3,3,0x010D,&st_power_current_data.w_v_gen_power_i_sum);
	    get_ck_inverter_data(6,3,0x010D,&st_power_current_data.w_w_gen_power_i_sum);
	
	}
	

    //acin current
    if(0 == check_system_type(cb_inv_type_riio_sun_ii_split_phase) && 0 ==  check_system_type(cb_inv_type_riio_ii_split_phase))
    {
	    get_ck_inverter_data(3,3,0x010C,&st_power_current_data.w_v_acin_current);
	    get_ck_inverter_data(6,3,0x010C,&st_power_current_data.w_w_acin_current);
    }
    //acin power
	if(check_system_type(cb_inv_type_riio_sun_ii_split_phase) || check_system_type(cb_inv_type_riio_ii_split_phase))
	{
		if(0 == b_meter_enable)
		{
			get_ck_inverter_data(0,3,0x0221,&st_power_current_data.w_v_acin_current);
			get_ck_inverter_data(0,3,0x0205,&st_power_current_data.w_v_acin_power_sum); 			//0034000105 w_v_acin_power_sum
			get_ck_inverter_data(0,3,0x010C,&st_power_current_data.w_w_acin_current);
		}
	}
	else if(0 == b_ct_iac_ext && 1 != b_meter_enable)
	{
		get_ck_inverter_data(3,3,0x0101,&st_power_current_data.w_v_acin_power_sum); 				//0034000105 w_v_acin_power_sum
		get_ck_inverter_data(6,3,0x0101,&st_power_current_data.w_w_acin_power_sum); 				//0034000106 w_w_acin_power_sum

	}

    uint16_t w_acin_source = 0;
    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                               cb_net_data_att_set,
                               0x005C,
                               &w_data_len,
                               (uint8_t *)&w_acin_source,
                               cb_device_data_access_operate_type_read);//w_acin_source_select

	//003400013A		w_uvw_acin_power_sum

	if(check_system_type(cb_inv_type_riio_sun_ii_split_phase) || check_system_type(cb_inv_type_riio_ii_split_phase))
	{
		if(0 == b_meter_enable)
		{
			st_power_current_data.w_uvw_acin_power_sum =				//003400013A
				st_power_current_data.w_u_acin_power_sum +\
				st_power_current_data.w_v_acin_power_sum;
			st_power_current_data.w_uvw_acin_power_sum = negative_to_zero(st_power_current_data.w_uvw_acin_power_sum);
		}

		st_power_current_data.w_uvw_acin_power_sum =  (st_power_current_data.w_uvw_acin_power_sum < 0xFF9B) ? st_power_current_data.w_uvw_acin_power_sum : 0;
		st_power_current_data.w_uvw_acin_power_sum =  (st_power_current_data.w_uvw_acin_power_sum > 100) ? st_power_current_data.w_uvw_acin_power_sum : 0;

	}
	else
	{
		st_power_current_data.w_uvw_acin_power_sum =
			st_power_current_data.w_u_acin_power_sum +\
			st_power_current_data.w_v_acin_power_sum +\
			st_power_current_data.w_w_acin_power_sum;
		if(check_system_type(cb_inv_type_tyrann) || check_system_type(cb_inv_type_apollo_matric_ii_10_15k))
		{
			st_power_current_data.w_uvw_acin_power_sum += st_power_current_data.w_gen_power_p_sum;
		}
		st_power_current_data.w_uvw_acin_power_sum =  (st_power_current_data.w_uvw_acin_power_sum < 0xFF9B) ? st_power_current_data.w_uvw_acin_power_sum : 0;
		st_power_current_data.w_uvw_acin_power_sum =  (st_power_current_data.w_uvw_acin_power_sum > 100) ? st_power_current_data.w_uvw_acin_power_sum : 0;
	}
    //aout1 current
    get_ck_inverter_data(0,3,0x010F,&st_power_current_data.w_u_acout1_current);
    get_ck_inverter_data(3,3,0x010F,&st_power_current_data.w_v_acout1_current);
    get_ck_inverter_data(6,3,0x010F,&st_power_current_data.w_w_acout1_current);

    //aout2 current
    get_ck_inverter_data(0,3,0x0110,&st_power_current_data.w_u_acout2_current);
    get_ck_inverter_data(3,3,0x0110,&st_power_current_data.w_v_acout2_current);
    get_ck_inverter_data(6,3,0x0110,&st_power_current_data.w_w_acout2_current);
	st_power_current_data.w_u_acout2_current = negtive_to_positive(st_power_current_data.w_u_acout2_current);
	st_power_current_data.w_v_acout2_current = negtive_to_positive(st_power_current_data.w_v_acout2_current);
	st_power_current_data.w_w_acout2_current = negtive_to_positive(st_power_current_data.w_w_acout2_current);
    //acload power(P)(S)
    //u
    get_ck_inverter_data(0,3,0x0104,&w_data_temp_1);//P-acout 有功功率
    get_ck_inverter_data(0,3,0x0105,&w_data_temp_4);//P-acou2

    get_ck_inverter_data(0,3,0x0106,&w_data_temp_2);//S	视在功率

    w_data_temp_3 = (uint16_t)((uint16_t)w_data_temp_2 - negtive_to_positive((uint16_t)w_data_temp_1));//无功功率（Q） = 视在功率 - 有功功率

    st_power_current_data.w_u_acload_power_p_sum = negative_to_zero(w_data_temp_1);
    st_power_current_data.w_u_acload_power2_p_sum = negative_to_zero(w_data_temp_4);
    st_power_current_data.w_u_acload_power_s_sum = w_data_temp_3 + w_data_temp_1;						//视在功率(S) = 有功功率(P) + 无功功率(Q)


    //v
    get_ck_inverter_data(3,3,0x0104,&w_data_temp_1);//P-acout
    get_ck_inverter_data(3,3,0x0105,&w_data_temp_4);//P-acou2

    get_ck_inverter_data(3,3,0x0106,&w_data_temp_2);//S
    w_data_temp_3 = (uint16_t)((uint16_t)w_data_temp_2 - negtive_to_positive((uint16_t)w_data_temp_1));//无功功率Q

    st_power_current_data.w_v_acload_power_p_sum = negative_to_zero(w_data_temp_1);
    st_power_current_data.w_v_acload_power2_p_sum = negative_to_zero(w_data_temp_4);
    st_power_current_data.w_v_acload_power_s_sum = w_data_temp_3 + w_data_temp_1;

    //w
    get_ck_inverter_data(6,3,0x0104,&w_data_temp_1);//P-acout
    get_ck_inverter_data(6,3,0x0105,&w_data_temp_4);//P-acou2

    get_ck_inverter_data(6,3,0x0106,&w_data_temp_2);//S
    w_data_temp_3 = (uint16_t)((uint16_t)w_data_temp_2 - negtive_to_positive((uint16_t)w_data_temp_1));

    st_power_current_data.w_w_acload_power_p_sum = negative_to_zero(w_data_temp_1);
    st_power_current_data.w_w_acload_power2_p_sum = negative_to_zero(w_data_temp_4);
    st_power_current_data.w_w_acload_power_s_sum = w_data_temp_3 + w_data_temp_1;

	//
    st_power_current_data.w_uvw_acload_power_p_sum =					//0034000110			
        st_power_current_data.w_u_acload_power_p_sum +\
        st_power_current_data.w_v_acload_power_p_sum +\
        st_power_current_data.w_w_acload_power_p_sum;
	st_power_current_data.w_uvw_acload_power_p_sum =  (st_power_current_data.w_uvw_acload_power_p_sum > 100) ? st_power_current_data.w_uvw_acload_power_p_sum : 0;

    st_power_current_data.w_uvw_acload_power2_p_sum =					//003400015D
        st_power_current_data.w_u_acload_power2_p_sum +\
        st_power_current_data.w_v_acload_power2_p_sum +\
        st_power_current_data.w_w_acload_power2_p_sum;
	st_power_current_data.w_uvw_acload_power2_p_sum =  (st_power_current_data.w_uvw_acload_power2_p_sum > 100) ? st_power_current_data.w_uvw_acload_power2_p_sum : 0;

    //acload sum
    st_power_current_data.w_acload_power_p_sum =						//0034000135	
        st_power_current_data.w_uvw_acload_power_p_sum +\
        st_power_current_data.w_uvw_acload_power2_p_sum +\
        w_normal_load_power;

    st_power_current_data.w_u_acload_power_p =							//0034000168
        st_power_current_data.w_u_acload_power_p_sum +\
        st_power_current_data.w_u_acload_power2_p_sum +\
        w_normal_load_power;

    st_power_current_data.w_v_acload_power_p =							//0034000169
        st_power_current_data.w_v_acload_power_p_sum +\
        st_power_current_data.w_v_acload_power2_p_sum + w_v_normal_load_power;

	if(check_system_type(cb_inv_type_riio_sun_ii_split_phase) || check_system_type(cb_inv_type_riio_ii_split_phase))
	{
		get_ck_inverter_data(0,3,0x0202,&w_data_temp_1);
		get_ck_inverter_data(0,3,0x0203,&w_data_temp_2);
		st_power_current_data.w_u_acload_power_p_sum = negative_to_zero(w_data_temp_1);			//003400010D
		st_power_current_data.w_u_acload_power2_p_sum = negative_to_zero(w_data_temp_2);		//0034000157
		
		st_power_current_data.w_u_acload_power_p =								//0034000168
	        st_power_current_data.w_u_acload_power_p_sum +\
	        st_power_current_data.w_u_acload_power2_p_sum +\
	        w_normal_load_power;


		get_ck_inverter_data(0,3,0x0207,&w_data_temp_1);
		get_ck_inverter_data(0,3,0x0208,&w_data_temp_2);
		st_power_current_data.w_v_acload_power_p_sum = negative_to_zero(w_data_temp_1);			//003400010E
		st_power_current_data.w_v_acload_power2_p_sum = negative_to_zero(w_data_temp_2);		//0034000158

	    st_power_current_data.w_v_acload_power_p =								//0034000169
	        st_power_current_data.w_v_acload_power_p_sum +\
	        st_power_current_data.w_v_acload_power2_p_sum +\
	        w_normal_load_power;
		
		st_power_current_data.w_uvw_acload_power_p_sum =						//0034000110
			st_power_current_data.w_u_acload_power_p_sum +\
			st_power_current_data.w_v_acload_power_p_sum;
		st_power_current_data.w_uvw_acload_power_p_sum =  (st_power_current_data.w_uvw_acload_power_p_sum > 100) ? st_power_current_data.w_uvw_acload_power_p_sum : 0;

		st_power_current_data.w_uvw_acload_power2_p_sum =						//003400015D
	        st_power_current_data.w_u_acload_power2_p_sum +\
	        st_power_current_data.w_v_acload_power2_p_sum;
		st_power_current_data.w_uvw_acload_power2_p_sum =  (st_power_current_data.w_uvw_acload_power2_p_sum > 100) ? st_power_current_data.w_uvw_acload_power2_p_sum : 0;

		    //acload sum
	    st_power_current_data.w_acload_power_p_sum =							//0034000135
	        st_power_current_data.w_uvw_acload_power_p_sum +\
	        st_power_current_data.w_uvw_acload_power2_p_sum +\
	        w_normal_load_power;
		
	}

    st_power_current_data.w_w_acload_power_p =\
        st_power_current_data.w_w_acload_power_p_sum +\
        st_power_current_data.w_w_acload_power2_p_sum + w_w_normal_load_power;

    //acload current
    st_power_current_data.w_u_acload_current_sum =\
        st_power_current_data.w_u_acout1_current + st_power_current_data.w_u_acout2_current;

    st_power_current_data.w_v_acload_current_sum  =\
        st_power_current_data.w_v_acout1_current + st_power_current_data.w_v_acout2_current;

    st_power_current_data.w_w_acload_current_sum  =\
        st_power_current_data.w_w_acout1_current + st_power_current_data.w_w_acout2_current;
	if(check_system_type(cb_inv_type_riio_sun_ii_split_phase) || check_system_type(cb_inv_type_riio_ii_split_phase))
	{
	    //aout1 current
    get_ck_inverter_data(0,3,0x010F,&st_power_current_data.w_u_acout1_current);
    get_ck_inverter_data(0,3,0x021E,&st_power_current_data.w_v_acout1_current);
    get_ck_inverter_data(0,3,0x0223,&st_power_current_data.w_w_acout1_current);

    //aout2 current
    get_ck_inverter_data(0,3,0x0110,&st_power_current_data.w_u_acout2_current);
    get_ck_inverter_data(0,3,0x021F,&st_power_current_data.w_v_acout2_current);
    get_ck_inverter_data(0,3,0x0224,&st_power_current_data.w_w_acout2_current);
	st_power_current_data.w_u_acout2_current = negtive_to_positive(st_power_current_data.w_u_acout2_current);
	st_power_current_data.w_v_acout2_current = negtive_to_positive(st_power_current_data.w_v_acout2_current);
	st_power_current_data.w_w_acout2_current = negtive_to_positive(st_power_current_data.w_w_acout2_current);
	//acload current
	st_power_current_data.w_u_acload_current_sum =\
		st_power_current_data.w_u_acout1_current + st_power_current_data.w_u_acout2_current;
	
	st_power_current_data.w_v_acload_current_sum  =\
		st_power_current_data.w_v_acout1_current + st_power_current_data.w_v_acout2_current;
	
	st_power_current_data.w_w_acload_current_sum  =\
		st_power_current_data.w_w_acout1_current + st_power_current_data.w_w_acout2_current;

	}
    //inverter power
    get_ck_inverter_data(0,3,0x0103,&st_power_current_data.w_u_inverter_power_sum);
    get_ck_inverter_data(3,3,0x0103,&st_power_current_data.w_v_inverter_power_sum);
    get_ck_inverter_data(6,3,0x0103,&st_power_current_data.w_w_inverter_power_sum);
	if(check_system_type(cb_inv_type_riio_sun_ii_split_phase) || check_system_type(cb_inv_type_riio_ii_split_phase))
	{
		get_ck_inverter_data(0,3,0x0201,&st_power_current_data.w_u_inverter_power_sum);
		get_ck_inverter_data(0,3,0x0206,&st_power_current_data.w_v_inverter_power_sum);
		get_ck_inverter_data(0,3,0x0103,&st_power_current_data.w_w_inverter_power_sum);
	}
    //inverter current
    get_ck_inverter_data(0,3,0x010E,&st_power_current_data.w_u_inverter_current);
    get_ck_inverter_data(3,3,0x010E,&st_power_current_data.w_v_inverter_current);
    get_ck_inverter_data(6,3,0x010E,&st_power_current_data.w_w_inverter_current);
	if(check_system_type(cb_inv_type_riio_sun_ii_split_phase) || check_system_type(cb_inv_type_riio_ii_split_phase))
	{
		get_ck_inverter_data(0,3,0x021D,&st_power_current_data.w_u_inverter_current);
		get_ck_inverter_data(0,3,0x0222,&st_power_current_data.w_v_inverter_current);
		get_ck_inverter_data(0,3,0x010E,&st_power_current_data.w_w_inverter_current);
	}

    //solor output power
    st_power_current_data.dw_solor_output_power =\
        st_power_current_data.dw_mppt_output_power_total +\
        st_pv_data_summary.w_pv_power_p_total;




    energy_flow_gather();

    net_write_energy_data(0,cb_power_current_data_num,(energy_data_info *)&st_energy_data_info[0]);
	{   //增加电池通讯离线状态
        uint16_t w_data_len = 2;
        uint16_t w_data_temp_1 = 0;
        uint16_t w_data_temp_2 = 0;
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
                               cb_net_data_att_set,
                               0x0050,
                               &w_data_len,
                               (uint8_t *)&w_data_temp_1,                                       //获取电池类型（6：为锂电池）
                               cb_device_data_access_operate_type_read);
        if(w_data_temp_1 == 6)
        {
            net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                               cb_net_data_att_status,
                               0x014C,
                               &w_data_len,
                               (uint8_t *)&w_data_temp_1,                                       //获取BMS告警信息
                               cb_device_data_access_operate_type_read);
            if(w_data_temp_1 & 0x0080)                                                          //判断BMS告警信息是否为57电池失联告警
            {
                  w_data_temp_2 = 3;                                                            //BatteryEnergyStatusGather 3：电池通讯失联
                  net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                       cb_net_data_att_status,
                                       0x011B,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_2,
                                       cb_device_data_access_operate_type_write);
            }
        }
    }

#if( task_runing_status_print )
    DebugPrintf(TRUE,"Riiosunii_power_process_end\r\n");
#endif

}



/************************************************************************************************
* Input: void
*
* Output: void
*
* Logic discription: ck inverter pv data process
*
*
* run time:
************************************************************************************************/
uint16_t wa_array[2] = {0,};

void network_ck_type_pv_data_process(net_device_info * const pst_net_device_info_in,
                                     uint8_t *pb_data_in,
                                     uint16_t w_data_len)//mark pv data from uart
{
    #define cb_pv_data_interval         54
    #define cb_pv_1_data_start_index    2
//    #define cb_pv_2_data_start_index    2 + cb_pv_data_interval
    #define cb_pv_inv_1_phase           0
    #define cb_pv_inv_3_phase           1

    if(data_process_type_ck != enum_data_process_func_type)
    {
       return;
    }

    #if( task_runing_status_print )
    DebugPrintf(TRUE,"Ck_pv_data_process_start\r\n");
    #endif

    net_device_info * const pst_net_device_info = 0;
    if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[cw_device_id_index_pv_1],
                                    (net_device_info **)&pst_net_device_info,
                                    &st_net_system_info_list))
    {
        return;
    }

    if((TRUE == net_check_device_communicate_status(cb_net_device_loss,pst_net_device_info)) ||\
       (pst_net_device_info_in != pst_net_device_info))
    {
        return;
    }

    uint8_t b_pv_num = 0;
    uint8_t b_pv_type = 0;
    uint16_t w_pv_today_realtime = 0;
    uint16_t w_pv_yesterday_realtime = 0;
    uint16_t w_pv_freq = 0;
    uint32_t dw_pv_power_total = 0;

    uint16_t w_pv_max_voltage_u = 0;
    uint16_t w_pv_max_voltage_v = 0;
    uint16_t w_pv_max_voltage_w = 0;

    uint16_t w_pv_max_power_u = 0;
    uint16_t w_pv_max_power_v = 0;
    uint16_t w_pv_max_power_w = 0;

    uint16_t w_data_temp = 0;
    uint8_t ba_pv_runing_status[2] = {0,};
    uint8_t ba_inverter_type[2] = {0,};
    uint32_t dwa_pv_power_out[2] = {0,};

    b_pv_num = pb_data_in[1];
    for(uint8_t b_cnt_i = 0;b_cnt_i < 2;b_cnt_i++)
    {
        //3-1Phase3KW
        //5-1Phase5KW
        //15-3Phase15KW
        //30-3Phase30KW
        //60-3Phase60KW
        switch(pb_data_in[cb_pv_1_data_start_index + 1 + b_cnt_i * cb_pv_data_interval])
        {
            case 3:
            case 5:
            case 15:
            case 30:
            case 60:
                ba_inverter_type[b_cnt_i] =\
                    pb_data_in[cb_pv_1_data_start_index + 1 + b_cnt_i * cb_pv_data_interval];
                break;
            default:
                ba_inverter_type[b_cnt_i] = 0;
                break;
        }

        //0 Waiting
        //1 SoftRun
        //2 Grid off
        //3 Generating
        //4 Fault Stop

        //0 Normal
        //1 StandBy
        //2 Fault
        switch(pb_data_in[cb_pv_1_data_start_index + 1 + b_cnt_i * cb_pv_data_interval])
        {
            case 0x01:
            case 0x03:
               ba_pv_runing_status[b_cnt_i] = 0;
               break;
            case 0x00:
            case 0x02:
               ba_pv_runing_status[b_cnt_i] = 1;
               break;
            case 0x04:
               ba_pv_runing_status[b_cnt_i] = 2;
               break;
           case 0xFF:
            default:
               ba_pv_runing_status[b_cnt_i] = 3;
               break;
        }
        bytes_to_int_big_to_little_endain(&pb_data_in[cb_pv_1_data_start_index + 22 + b_cnt_i * cb_pv_data_interval],
                                          (uint32_t *)&dwa_pv_power_out[b_cnt_i],4);

        w_data_temp = 0;
        bytes_to_int_big_to_little_endain(&pb_data_in[cb_pv_1_data_start_index + 34 + b_cnt_i * cb_pv_data_interval],
                                          (uint32_t *)&w_data_temp,2);
        wa_array[b_cnt_i] = w_data_temp;
        
        w_pv_today_realtime += w_data_temp;

        w_data_temp = 0;
        bytes_to_int_big_to_little_endain(&pb_data_in[cb_pv_1_data_start_index + 36 + b_cnt_i * cb_pv_data_interval],
                                          (uint32_t *)&w_data_temp,2);
        w_pv_yesterday_realtime += w_data_temp;
    }
    b_pv_type = (ba_inverter_type[0] != 0)?ba_inverter_type[0]:ba_inverter_type[1];
    bytes_to_int_big_to_little_endain(&pb_data_in[cb_pv_1_data_start_index + 8],(uint32_t *)&w_pv_freq,2);
    if((0 == w_pv_freq) && (b_pv_num > 1))
    {
        bytes_to_int_big_to_little_endain(&pb_data_in[cb_pv_1_data_start_index + 8 + cb_pv_data_interval],
                                          (uint32_t *)&w_pv_freq,2);
    }
    dw_pv_power_total = dwa_pv_power_out[0] + dwa_pv_power_out[1];

    switch(b_pv_type)
    {
        case cb_pv_inv_1_phase:
            {
                w_data_temp = 0;
                for(uint8_t b_cnt_i = 0;b_cnt_i < 2;b_cnt_i++)
                {
                    bytes_to_int_big_to_little_endain(&pb_data_in[cb_pv_1_data_start_index + 10 + b_cnt_i * cb_pv_data_interval],
                                                      (uint32_t *)&w_data_temp,2);
                    if(w_pv_max_voltage_u < w_data_temp)
                    {
                        w_pv_max_voltage_u = w_data_temp;
                    }
                }
                w_pv_max_power_u = (uint16_t)dw_pv_power_total;//mark -> uint32
            }
            break;
        case cb_pv_inv_3_phase:
            {
                w_data_temp = 0;
                for(uint8_t b_cnt_i = 0;b_cnt_i < 2;b_cnt_i++)
                {
                    bytes_to_int_big_to_little_endain(&pb_data_in[cb_pv_1_data_start_index + 10 + b_cnt_i * cb_pv_data_interval],
                                                      (uint32_t *)&w_data_temp,2);
                    if(w_pv_max_voltage_u < w_data_temp)
                    {
                        w_pv_max_voltage_u = w_data_temp;
                    }
                }

                w_data_temp = 0;
                for(uint8_t b_cnt_i = 0;b_cnt_i < 2;b_cnt_i++)
                {
                    bytes_to_int_big_to_little_endain(&pb_data_in[cb_pv_1_data_start_index + 12 + b_cnt_i * cb_pv_data_interval],
                                                      (uint32_t *)&w_data_temp,2);
                    if(w_pv_max_voltage_v < w_data_temp)
                    {
                        w_pv_max_voltage_v = w_data_temp;
                    }
                }

                w_data_temp = 0;
                for(uint8_t b_cnt_i = 0;b_cnt_i < 2;b_cnt_i++)
                {
                    bytes_to_int_big_to_little_endain(&pb_data_in[cb_pv_1_data_start_index + 14 + b_cnt_i * cb_pv_data_interval],
                                                      (uint32_t *)&w_data_temp,2);
                    if(w_pv_max_voltage_w < w_data_temp)
                    {
                        w_pv_max_voltage_w = w_data_temp;
                    }
                }

                w_pv_max_power_u = (uint16_t)dw_pv_power_total/3;//mark -> uint32
                w_pv_max_power_v = w_pv_max_power_u;
                w_pv_max_power_w = w_pv_max_power_u;
            }
            break;
    }
    {
        uint16_t w_data_len = 1;
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_pv_1],
                               cb_device_data_access_data_type_status,
                               0x0100,
                               &w_data_len,
                               &b_pv_num,
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_pv_1],
                               cb_device_data_access_data_type_status,
                               0x0101,
                               &w_data_len,
                               &b_pv_type,
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_pv_1],
                               cb_device_data_access_data_type_status,
                               0x010C,
                               &w_data_len,
                               &ba_inverter_type[0],
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_pv_1],
                               cb_device_data_access_data_type_status,
                               0x010F,
                               &w_data_len,
                               &ba_inverter_type[1],
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_pv_1],
                               cb_device_data_access_data_type_status,
                               0x010D,
                               &w_data_len,
                               &ba_pv_runing_status[0],
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_pv_1],
                               cb_device_data_access_data_type_status,
                               0x0110,
                               &w_data_len,
                               &ba_pv_runing_status[1],
                               cb_device_data_access_operate_type_write);

        w_data_len = 2;
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_pv_1],
                               cb_device_data_access_data_type_status,
                               0x0102,
                               &w_data_len,
                               (uint8_t *)&w_pv_today_realtime,
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_pv_1],
                               cb_device_data_access_data_type_status,
                               0x0103,
                               &w_data_len,
                               (uint8_t *)&w_pv_yesterday_realtime,
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_pv_1],
                               cb_device_data_access_data_type_status,
                               0x0104,
                               &w_data_len,
                               (uint8_t *)&w_pv_freq,
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_pv_1],
                               cb_device_data_access_data_type_status,
                               0x0105,
                               &w_data_len,
                               (uint8_t *)&dw_pv_power_total,
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_pv_1],
                               cb_device_data_access_data_type_status,
                               0x0106,
                               &w_data_len,
                               (uint8_t *)&w_pv_max_power_u,
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_pv_1],
                               cb_device_data_access_data_type_status,
                               0x0107,
                               &w_data_len,
                               (uint8_t *)&w_pv_max_power_v,
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_pv_1],
                               cb_device_data_access_data_type_status,
                               0x0108,
                               &w_data_len,
                               (uint8_t *)&w_pv_max_power_w,
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_pv_1],
                               cb_device_data_access_data_type_status,
                               0x0109,
                               &w_data_len,
                               (uint8_t *)&w_pv_max_voltage_u,
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_pv_1],
                               cb_device_data_access_data_type_status,
                               0x010A,
                               &w_data_len,
                               (uint8_t *)&w_pv_max_voltage_v,
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_pv_1],
                               cb_device_data_access_data_type_status,
                               0x010B,
                               &w_data_len,
                               (uint8_t *)&w_pv_max_voltage_w,
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_pv_1],
                               cb_device_data_access_data_type_status,
                               0x010E,
                               &w_data_len,
                               (uint8_t *)&dwa_pv_power_out[0],
                               cb_device_data_access_operate_type_write);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_pv_1],
                               cb_device_data_access_data_type_status,
                               0x0111,
                               &w_data_len,
                               (uint8_t *)&dwa_pv_power_out[1],
                               cb_device_data_access_operate_type_write);

    }

#if( task_runing_status_print )
    DebugPrintf(TRUE,"Ck_pv_data_process_end\r\n");
#endif
}
/************************************************************************************************
* Input: void
*
* Output: void
*
* Logic discription: riiosunii inverter pv data process
*
*
* run time:
************************************************************************************************/
void network_riiosunii_type_pv_data_process()
{
    #define cb_pv_inv_1_phase           0
    #define cb_pv_inv_3_phase           1

    #if( task_runing_status_print )
    DebugPrintf(TRUE,"Ck_pv_data_process_start\r\n");
    #endif

    net_device_info * const pst_net_device_info = 0;
    if(FALSE == net_get_device_info(dwa_device_id_kinergy_ii[cw_device_id_index_pv_1],
                                    (net_device_info **)&pst_net_device_info,
                                    &st_net_system_info_list))
    {
        return;
    }
    if(TRUE == net_check_device_communicate_status(cb_net_device_loss,pst_net_device_info))
    {
        return;
    }


    uint8_t ba_pv_runing_status[2] = {0,};
    uint8_t ba_inverter_type[2] = {0,};

    for(uint8_t b_cnt_1 = cw_device_id_index_pv_1;
        b_cnt_1 <= cw_device_id_index_pv_2;
        b_cnt_1++)
    {
        uint16_t w_data_len = 0;
        uint16_t w_data_temp = 0;
        net_device_data_access(dwa_device_id_kinergy_ii[b_cnt_1],
                               cb_device_data_access_data_type_status,
                               0x0103,
                               &w_data_len,
                               (uint8_t *)&w_data_temp,
                               cb_device_data_access_operate_type_read);
        switch(w_data_temp)
        {
            case 0x01:
            case 0x03:
                ba_pv_runing_status[b_cnt_1] = 0;//normal
                break;
            case 0x02:
            case 0x00:
                ba_pv_runing_status[b_cnt_1] = 1;//standby
                break;
            case 0x04:
                ba_pv_runing_status[b_cnt_1] = 2;//fault
                break;
            default:
                ba_pv_runing_status[b_cnt_1] = 3;//offline
                break;
        }
        net_device_data_access(dwa_device_id_kinergy_ii[b_cnt_1],
                               cb_device_data_access_data_type_status,
                               0x0101,
                               &w_data_len,
                               (uint8_t *)&w_data_temp,
                               cb_device_data_access_operate_type_read);
        switch(w_data_temp)
        {
            case 3:
            case 5:
                ba_inverter_type[b_cnt_1] = cb_pv_inv_1_phase;
                break;
            case 15:
            case 30:
            case 60:
                ba_inverter_type[b_cnt_1] = cb_pv_inv_3_phase;
                break;
            default:
                ba_inverter_type[b_cnt_1] = cb_pv_inv_1_phase;
                break;
        }
        {
            uint16_t w_data_len = 1;
            net_device_data_access(dwa_device_id_kinergy_ii[b_cnt_1],
                                   cb_device_data_access_data_type_status,
                                   0x0119,
                                   &w_data_len,
                                   (uint8_t *)&ba_pv_runing_status[b_cnt_1],
                                   cb_device_data_access_operate_type_write);
            net_device_data_access(dwa_device_id_kinergy_ii[b_cnt_1],
                                   cb_device_data_access_data_type_status,
                                   0x011A,
                                   &w_data_len,
                                   (uint8_t *)&ba_inverter_type[b_cnt_1],
                                   cb_device_data_access_operate_type_write);
        }
    }

    get_pv_data(0x010C,&st_pv_data_summary.w_pv_power_p_total);
    switch(ba_inverter_type[0])
    {
        case cb_pv_inv_1_phase:
            {
                for(uint8_t b_cnt_2 = cw_device_id_index_pv_1;
                    b_cnt_2 <= cw_device_id_index_pv_2;
                    b_cnt_2++)
                {
                    uint16_t w_data_len = 0;
                    uint16_t w_data_temp = 0;
                    net_device_data_access(dwa_device_id_kinergy_ii[b_cnt_2],
                                           cb_device_data_access_data_type_status,
                                           0x0106,
                                           &w_data_len,
                                           (uint8_t *)&w_data_temp,
                                           cb_device_data_access_operate_type_read);
                    if(w_data_temp > st_pv_data_summary.w_pv_max_votage_u)
                    {
                        st_pv_data_summary.w_pv_max_votage_u = w_data_temp;
                    }
                }
                st_pv_data_summary.w_pv_max_votage_v = 0;
                st_pv_data_summary.w_pv_max_votage_w = 0;
                st_pv_data_summary.w_pv_power_p_u = st_pv_data_summary.w_pv_power_p_total;
                st_pv_data_summary.w_pv_power_p_v = 0;
                st_pv_data_summary.w_pv_power_p_w = 0;
            }
            break;
        case cb_pv_inv_3_phase:
            {
                for(uint8_t b_cnt_2 = cw_device_id_index_pv_1;
                    b_cnt_2 <= cw_device_id_index_pv_2;
                    b_cnt_2++)
                {
                    uint16_t w_data_len = 0;
                    uint16_t w_data_temp_1 = 0;
                    uint16_t w_data_temp_2 = 0;
                    uint16_t w_data_temp_3 = 0;
                    net_device_data_access(dwa_device_id_kinergy_ii[b_cnt_2],
                                           cb_device_data_access_data_type_status,
                                           0x0106,
                                           &w_data_len,
                                           (uint8_t *)&w_data_temp_1,
                                           cb_device_data_access_operate_type_read);
                    if(w_data_temp_1 > st_pv_data_summary.w_pv_max_votage_u)
                    {
                        st_pv_data_summary.w_pv_max_votage_u = w_data_temp_1;
                    }
                    net_device_data_access(dwa_device_id_kinergy_ii[b_cnt_2],
                                           cb_device_data_access_data_type_status,
                                           0x0107,
                                           &w_data_len,
                                           (uint8_t *)&w_data_temp_2,
                                           cb_device_data_access_operate_type_read);
                    if(w_data_temp_2 > st_pv_data_summary.w_pv_max_votage_u)
                    {
                        st_pv_data_summary.w_pv_max_votage_v = w_data_temp_2;
                    }
                    net_device_data_access(dwa_device_id_kinergy_ii[b_cnt_2],
                                           cb_device_data_access_data_type_status,
                                           0x0108,
                                           &w_data_len,
                                           (uint8_t *)&w_data_temp_3,
                                           cb_device_data_access_operate_type_read);
                    if(w_data_temp_3 > st_pv_data_summary.w_pv_max_votage_u)
                    {
                        st_pv_data_summary.w_pv_max_votage_w = w_data_temp_3;
                    }
                }
                st_pv_data_summary.w_pv_power_p_u = st_pv_data_summary.w_pv_power_p_total / 3;
                st_pv_data_summary.w_pv_power_p_v = st_pv_data_summary.w_pv_power_p_total / 3;
                st_pv_data_summary.w_pv_power_p_w = st_pv_data_summary.w_pv_power_p_total / 3;
            }
            break;
        default:
            break;
    }
    net_write_energy_data(0,cb_max_pv_data_summary_num,(energy_data_info *)&st_pv_data_summary_list[0]);
#if( task_runing_status_print )
    DebugPrintf(TRUE,"Riiosunii_pv_data_process_end\r\n");
#endif
}

//新增matrix 发电机运行时间显示 2023.05.13
void system_ck_type_data_process()
{
    uint32_t dw_timestamp = 0;
    uint16_t w_data_temp_1 = 0;
    static uint32_t dw_acin_gen_run_time;
    static uint32_t dw_acin_gen_run_time_backup;
    static uint32_t dw_timestamp_backup;
//    uint16_t w_system_conditional_bits = 0;
    #if( task_runing_status_print )
        DebugPrintf(TRUE,"system_ck_type_data_process_start\r\n");
    #endif

    get_system_timestamp(&dw_timestamp);
    {
        uint16_t w_data_len = 2;
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                               cb_device_data_access_data_type_set,
                               0x001F,                                          //PARAM_MODE_ACFLEX_SET  0-Acout2; 1-Alternator
                               &w_data_len,
                               (uint8_t *)&w_data_temp_1,
                               cb_device_data_access_operate_type_read);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x012D,                                          //Smart port set    0-Smart ACout2; 1-Smart GEN；  Tyrann  1-GEN ,2- City GEN
                               &w_data_len,
                               (uint8_t *)&w_data_temp_1,
                               cb_device_data_access_operate_type_write);
    }
    if(((dw_timestamp % (3600 * 24)) <= 10) &&\
       ((dw_timestamp - dw_timestamp_backup) >= 10))
    {
        dw_timestamp_backup = dw_timestamp;
        dw_acin_gen_run_time = 0;
    }
    else if((dw_timestamp != dw_timestamp_backup) ||\
            (0 == dw_timestamp_backup))
    {
        //gen run time
        {
            {
                uint16_t w_data_len = 2;
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                       cb_device_data_access_data_type_set,
                                       0x005C,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_1,
                                       cb_device_data_access_operate_type_read);
//                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
//                                       cb_device_data_access_data_type_status,
//                                       0x0114,
//                                       &w_data_len,
//                                       (uint8_t *)&w_system_conditional_bits,
//                                       cb_device_data_access_operate_type_read);
            }
//            if((1 == w_data_temp_1) && (0x3C & w_system_conditional_bits ) && (st_power_current_data.w_uvw_acin_power_sum > Gen_Start_Max_Ignored_Power))
            if((1 == w_data_temp_1) && (st_power_current_data.w_uvw_acin_power_sum > Gen_Start_Max_Ignored_Power))
            {
                if(0 != dw_acin_gen_run_time_backup)
                {
                    dw_acin_gen_run_time += negative_to_zero_u32(dw_timestamp - dw_acin_gen_run_time_backup);
                }
                dw_acin_gen_run_time_backup = dw_timestamp;
            }
            else
            {
                dw_acin_gen_run_time_backup = 0;
            }
            {
                uint16_t w_data_len = 2;
                uint16_t w_data_temp = 0;

                w_data_temp = dw_acin_gen_run_time / 360;
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                       cb_device_data_access_data_type_status,
                                       0x013F,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp,
                                       cb_device_data_access_operate_type_write);
            }
        }
   }
}


uint16_t g_parallel_system_mode = 0;
uint8_t get_system_parallel_mode()
{
	static uint16_t b_parallel_system_mode = 0;
	uint8_t b_parallel_system_mode_back_up = 0;
	uint16_t w_data_len;
	for(uint8_t b_cnt_i = 0; b_cnt_i < 9; b_cnt_i ++)
	{
		static net_device_info *pst_net_device_info = 0;
        if(FALSE == net_get_device_info((dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1 + b_cnt_i]),
                                        (net_device_info **)&pst_net_device_info,
                                        &st_net_system_info_list))
        {
            continue;
        }
		if(FALSE == net_check_device_communicate_status(cb_net_device_add,pst_net_device_info))	
		{
			continue;
		}
		net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1 + b_cnt_i],
					   cb_device_data_access_data_type_set,
					   0x0020,
					   &w_data_len,
					   (uint8_t *)&b_parallel_system_mode,										 //获取系统单并机模式
					   cb_device_data_access_operate_type_read);
		b_parallel_system_mode = b_parallel_system_mode_back_up | b_parallel_system_mode;
		b_parallel_system_mode_back_up = b_parallel_system_mode;
		g_parallel_system_mode = b_parallel_system_mode;
		w_data_len = 1;
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x011C,                                                  //系统单机、并机配置情况
                               &w_data_len,
                               (uint8_t *)&b_parallel_system_mode,
                               cb_device_data_access_operate_type_write);
	
	}
	return b_parallel_system_mode;

}
uint8_t get_system_type()
{
	static uint16_t b_system_type = 0;
	uint8_t b_system_type_back_up = 0;
	uint16_t w_data_len;
	for(uint8_t b_cnt_i = 0; b_cnt_i < 9; b_cnt_i ++)
	{
		net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1 + b_cnt_i],
					   cb_net_data_att_set,
					   0x0004,
					   &w_data_len,
					   (uint8_t *)&b_system_type,										 //获取系统单并机模式
					   cb_device_data_access_operate_type_read);
		b_system_type = b_system_type_back_up | b_system_type;
		b_system_type_back_up = b_system_type;
	
	}
	return b_system_type;

}

/************************************************************************************************
* Input: void
*
* Output: void
*
* Logic discription: riiosun ii type inverter meter data process
*                    power saving data process(kinergy status 0x015E)
*
* run time:
************************************************************************************************/
void system_riiosunii_meter_data_process()
{
    uint32_t dw_timestamp = 0;
    uint16_t w_data_temp_1 = 0;
    uint16_t w_data_temp_2 = 0;
    uint16_t w_load_consume = 0;
    static uint32_t dw_acout2_gen_run_time;
    static uint32_t dw_acout2_gen_run_time_backup;
    static uint32_t dw_acin_gen_run_time;
    static uint32_t dw_acin_gen_run_time_backup;
    static uint32_t dw_timestamp_backup;
	uint16_t b_parallel_system_mode = 0;

#if( task_runing_status_print )
    DebugPrintf(TRUE,"Riiosunii_meter_data_process_start\r\n");
#endif

    get_system_timestamp(&dw_timestamp);
	b_parallel_system_mode = get_system_parallel_mode();

    {
        uint16_t w_data_len = 2;
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                               cb_device_data_access_data_type_set,
                               0x001F,														//SMART_PORT_SET   Tyrann: ACin2_source_select
                               &w_data_len,
                               (uint8_t *)&w_data_temp_1,
                               cb_device_data_access_operate_type_read);
		if(check_system_type(cb_inv_type_tyrann) || check_system_type(cb_inv_type_apollo_matric_ii_10_15k))
		{
			w_data_temp_1 = 0;
		}
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x012D,
                               &w_data_len,
                               (uint8_t *)&w_data_temp_1,
                               cb_device_data_access_operate_type_write);//smart port set   0-Smart ACout2; 1-Smart GEN；  Tyrann  1-GEN ,2- City GEN

        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                               cb_device_data_access_data_type_status,
                               0x014E,
                               &w_data_len,
                               (uint8_t *)&w_data_temp_1,
                               cb_device_data_access_operate_type_read);
        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                               cb_device_data_access_data_type_status,
                               0x012B,
                               &w_data_len,
                               (uint8_t *)&w_data_temp_1,
                               cb_device_data_access_operate_type_write);//bat cap
		if(check_system_type(cb_inv_type_riio_sun_ii) 
			|| check_system_type(cb_inv_type_riio_sun_ii_gte) 
			|| check_system_type(cb_inv_type_riio_sun_ii_split_phase) 
			|| check_system_type(cb_inv_type_riio_ii_split_phase)
			|| check_system_type(cb_inv_type_riio_ii))
		{
            static uint8_t b_meter_mode = 0;

			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
								   cb_device_data_access_data_type_set,
								   0x00DD,
								   &w_data_len,
								   (uint8_t *)&b_meter_mode,
								   cb_device_data_access_operate_type_read);
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
								   cb_device_data_access_data_type_status,
								   0x0131,
								   &w_data_len,
								   (uint8_t *)&b_meter_mode,
								   cb_device_data_access_operate_type_write);//meter enable 
        }
    }
    if(((dw_timestamp % (3600 * 24)) <= 10) &&\
       ((dw_timestamp - dw_timestamp_backup) >= 10))
    {
        dw_timestamp_backup = dw_timestamp;
        dw_acout2_gen_run_time = 0;
        dw_acin_gen_run_time = 0;
    }
    else if((dw_timestamp != dw_timestamp_backup) ||\
            (0 == dw_timestamp_backup))
    {
        //gen run time
        {
            uint16_t w_system_conditional_bits = 0;
            {
                uint16_t w_data_len = 2;
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                       cb_device_data_access_data_type_status,
                                       0x0120,
                                       &w_data_len,
                                       (uint8_t *)&w_system_conditional_bits,
                                       cb_device_data_access_operate_type_read);
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                                       cb_device_data_access_data_type_set,
                                       0x005C,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_1,
                                       cb_device_data_access_operate_type_read);
            }

            //if(w_system_conditional_bits & 0x0100)
            if(w_system_conditional_bits & 0x0100 && st_power_current_data.w_gen_power_p_sum > Gen_Start_Max_Ignored_Power)  //加入判断发电机当前acout功率大于等于Gen_Start_Max_Ignored_Power
            {
                if(0 != dw_acout2_gen_run_time_backup)
                {
                    dw_acout2_gen_run_time += negative_to_zero_u32(dw_timestamp - dw_acout2_gen_run_time_backup);
                }
                dw_acout2_gen_run_time_backup = dw_timestamp;
            }
            else
            {
                dw_acout2_gen_run_time_backup = 0;
            }
			uint16_t w_energy_acin1_power = 0;
            if(check_system_type(cb_inv_type_tyrann) || check_system_type(cb_inv_type_apollo_matric_ii_10_15k))  //Tyrann 
            {
				w_energy_acin1_power = st_power_current_data.w_uvw_acin_power_sum - st_power_current_data.w_gen_power_p_sum;
			}
			else
			{
				w_energy_acin1_power = st_power_current_data.w_uvw_acin_power_sum;

			}
            if(1 == w_data_temp_1 && w_energy_acin1_power > Gen_Start_Max_Ignored_Power)       //åŠ å…¥åˆ¤æ–­å‘ç”µæœºå½“å‰acinåŠŸçŽ‡å¤§äºŽç­‰äºŽGen_Start_Max_Ignored_Power
            {
                if(0 != dw_acin_gen_run_time_backup)
                {
                    dw_acin_gen_run_time += negative_to_zero_u32(dw_timestamp - dw_acin_gen_run_time_backup);
                }
                dw_acin_gen_run_time_backup = dw_timestamp;
            }
            else
            {
                dw_acin_gen_run_time_backup = 0;
            }
            {
                uint16_t w_data_len = 2;
                uint16_t w_data_temp = 0;

                w_data_temp = dw_acout2_gen_run_time / 360;
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                       cb_device_data_access_data_type_status,
                                       0x0129,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp,
                                       cb_device_data_access_operate_type_write);

                w_data_temp = dw_acin_gen_run_time / 360;
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                       cb_device_data_access_data_type_status,
                                       0x013F,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp,
                                       cb_device_data_access_operate_type_write);
            }
        }
        {   //power saving
            uint16_t w_data_len = 2;
            uint16_t w_all_power_sum;
            static uint16_t w_all_power_sum_last;
            static uint16_t w_e_acin_last;
            static uint16_t w_e_acin_feedback_last;
            uint16_t w_all_power_sum_growth;
            uint16_t w_e_acin_growth;
            uint16_t w_e_acin_feedback_growth;
            uint32_t dw_timestamp_today;
//            static uint32_t dw_timestamp_today_backup;
            static uint32_t dw_power_timestamp_backup;
            uint32_t dw_timestamp_start;
            uint32_t dw_timestamp_end = 0;
            uint32_t dw_price;
            static uint32_t dw_power_saving_backup;

            dw_timestamp_today = (dw_timestamp % (3600 * 24));
            if((0 != dw_timestamp) &&\
               (0 == (dw_timestamp % 3600)) &&\
               (dw_timestamp != dw_power_timestamp_backup))
            {
                dw_power_timestamp_backup = dw_timestamp;
//                dw_timestamp_today_backup = 0;
                //mark save eeprom
                if(dw_power_saving != dw_power_saving_backup)
                {
                    dw_power_saving_backup = dw_power_saving;
                    fWriteEEPage(17) = TRUE;
                }
            }
            net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                  cb_net_data_att_status,
                                  0x0131,
                                  &w_data_len,
                                  (uint8_t *)&w_data_temp_1,
                                  cb_device_data_access_operate_type_read);//meter enable
			if(1 == w_data_temp_1)//meter enable
            {
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_meter_1],
                                      cb_net_data_att_status,
                                      0x0118,
                                      &w_data_len,
                                      (uint8_t *)&w_data_temp_1,
                                      cb_device_data_access_operate_type_read);//meter acin
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_meter_1],
                                       cb_net_data_att_status,
                                       0x0119,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_2,
                                       cb_device_data_access_operate_type_read);//meter acin feed						  
            }
            else
            {
               net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                      cb_net_data_att_status,
                                      0x0147,
                                      &w_data_len,
                                      (uint8_t *)&w_data_temp_1,
                                      cb_device_data_access_operate_type_read);//acin
               net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                      cb_net_data_att_status,
                                      0x012A,
                                      &w_data_len,
                                      (uint8_t *)&w_data_temp_2,
                                      cb_device_data_access_operate_type_read);//acin feed
            }
            net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                   cb_device_data_access_data_type_status,
                                   0x0133,
                                   &w_data_len,
                                   (uint8_t *)&w_load_consume,
                                   cb_device_data_access_operate_type_read);//w_load_consume

            if(b_power_saving_start_flag)
            {
                    b_power_saving_start_flag = 0;
                    w_all_power_sum_last = w_load_consume;
                    w_e_acin_last = w_data_temp_1;
                    w_e_acin_feedback_last = w_data_temp_2;
            }
            #if (power_saving_print)
                uint16_t w_load_consume_backup = w_all_power_sum_last;
            #endif
            w_all_power_sum = w_load_consume;
//			if((w_all_power_sum - w_all_power_sum_last) >= 100)
//			{
//				w_all_power_sum_last = w_all_power_sum;
//			}
            w_all_power_sum_growth = negative_to_zero(w_all_power_sum - w_all_power_sum_last);
            w_all_power_sum_last = w_all_power_sum;
            for(uint8_t b_cnt_i = 0;
                (b_cnt_i < 8) && (dw_timestamp_end <= ((3600 * 24) - 60));
                b_cnt_i++)// buy
            {
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                       cb_device_data_access_data_type_set,
                                       0x0000 + b_cnt_i,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_1,
                                       cb_device_data_access_operate_type_read);//buy start time
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                       cb_device_data_access_data_type_set,
                                       0x0008 + b_cnt_i,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_2,
                                       cb_device_data_access_operate_type_read);//buy end time
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                       cb_device_data_access_data_type_set,
                                       0x0010 + b_cnt_i,
                                       &w_data_len,
                                       (uint8_t *)&dw_price,
                                       cb_device_data_access_operate_type_read);//buy price
                dw_timestamp_start = (((uint8_t)(w_data_temp_1 >> 8)) * 3600) +\
                                    (((uint8_t)(w_data_temp_1)) * 60);
                dw_timestamp_end = (((uint8_t)(w_data_temp_2 >> 8)) * 3600) +\
                                  (((uint8_t)(w_data_temp_2)) * 60);
                if((dw_timestamp_start >= dw_timestamp_end))
                {
                    break;
                }
                else if((dw_timestamp_today >= dw_timestamp_start) &&\
                        (dw_timestamp_today <= dw_timestamp_end))
                {
                    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                           cb_net_data_att_status,
                                           0x0131,
                                           &w_data_len,
                                           (uint8_t *)&w_data_temp_1,
                                           cb_device_data_access_operate_type_read);//meter enable
                    if(1 == w_data_temp_1)//meter enable
                    {
                        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_meter_1],
                                               cb_net_data_att_status,
                                               0x0118,
                                               &w_data_len,
                                               (uint8_t *)&w_data_temp_1,
                                               cb_device_data_access_operate_type_read);//meter acin
                    }
                    else
                    {
                        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                               cb_net_data_att_status,
                                               0x0147,
                                               &w_data_len,
                                               (uint8_t *)&w_data_temp_1,
                                               cb_device_data_access_operate_type_read);//acin
                    }
//					if((w_data_temp_1 - w_e_acin_last) >= 100)
//					{
//						w_e_acin_last = w_data_temp_1;
//					}
                    #if (power_saving_print)
                        uint16_t w_e_acin_backup = w_e_acin_last;
                    #endif
                    w_e_acin_growth = negative_to_zero(w_data_temp_1 - w_e_acin_last);
                    w_e_acin_last = w_data_temp_1;
                    dw_power_saving += abs((dw_price * w_all_power_sum_growth) / 10);
					#if (power_saving_print)
                            DebugPrintf(TRUE,"dw_power_saving:%d\r\n,",dw_power_saving);
                    #endif
                    dw_power_saving = negative_to_zero_u32(dw_power_saving - (dw_price * w_e_acin_growth) / 10);
                    #if (power_saving_print)
                            DebugPrintf(TRUE,"dw_power_saving += load_consume_grown - from_grid_growth :%d\r\n,load_consumw_growth:%d\r\n,load_consumw_now:%d\r\n,load_consumw_last:%d\r\n,from_grid_growth:%d\r\n,from_grid_now:%d\r\n,from_grid_last:%d\r\n",
                            dw_power_saving,w_all_power_sum_growth,w_all_power_sum,w_load_consume_backup,w_e_acin_growth,w_data_temp_1,w_e_acin_backup);
                    #endif
                    break;
                }
            }
            for(uint8_t b_cnt_i = 0;
                (b_cnt_i < 8) && (dw_timestamp_end <= ((3600 * 24) - 60));
                b_cnt_i++)// sell
            {
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                       cb_device_data_access_data_type_set,
                                       0x0018 + b_cnt_i,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_1,
                                       cb_device_data_access_operate_type_read);//sell start time
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                       cb_device_data_access_data_type_set,
                                       0x0020 + b_cnt_i,
                                       &w_data_len,
                                       (uint8_t *)&w_data_temp_2,
                                       cb_device_data_access_operate_type_read);//sell end time
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                       cb_device_data_access_data_type_set,
                                       0x0028 + b_cnt_i,
                                       &w_data_len,
                                       (uint8_t *)&dw_price,
                                       cb_device_data_access_operate_type_read);//sell price
                dw_timestamp_start = (((uint8_t)(w_data_temp_1 >> 8)) * 3600) +\
                                    (((uint8_t)(w_data_temp_1)) * 60);
                dw_timestamp_end = (((uint8_t)(w_data_temp_2 >> 8)) * 3600) +\
                                  (((uint8_t)(w_data_temp_2)) * 60);
                if((dw_timestamp_start >= dw_timestamp_end))
                {
                    break;
                }
                else if((dw_timestamp_today >= dw_timestamp_start) &&\
                        (dw_timestamp_today <= dw_timestamp_end))
                {
                    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                           cb_net_data_att_status,
                                           0x0131,
                                           &w_data_len,
                                           (uint8_t *)&w_data_temp_1,
                                           cb_device_data_access_operate_type_read);//meter enable
                    if(1 == w_data_temp_1)//meter enable
                    {
                        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_meter_1],
                                               cb_net_data_att_status,
                                               0x0119,
                                               &w_data_len,
                                               (uint8_t *)&w_data_temp_1,
                                               cb_device_data_access_operate_type_read);//meter acin feed
                    }
                    else
                    {
                        net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                               cb_net_data_att_status,
                                               0x012A,
                                               &w_data_len,
                                               (uint8_t *)&w_data_temp_1,
                                               cb_device_data_access_operate_type_read);//acin feed
                    }
//					if((w_data_temp_1 - w_e_acin_feedback_last) >= 100)
//					{
//						w_e_acin_feedback_last = w_data_temp_1;
//					}
                    #if (power_saving_print)
                        uint16_t w_e_acin_feedback_backup = w_e_acin_feedback_last;
                    #endif
                    w_e_acin_feedback_growth = negative_to_zero(w_data_temp_1 - w_e_acin_feedback_last);
                    w_e_acin_feedback_last = w_data_temp_1;
                    dw_power_saving += dw_price * w_e_acin_feedback_growth / 10 ;
                    #if (power_saving_print)
                            DebugPrintf(TRUE,"dw_power_saving += load_consume_grown - from_grid_growth + feedback_to_grid_growth :%d\r\n,feedback_to_grid_growth :%d\r\n,feedback_to_grid_now :%d\r\n,feedback_to_grid_last :%d\r\n",
                            dw_power_saving,w_e_acin_feedback_growth,w_data_temp_1,w_e_acin_feedback_backup);
                    #endif
                    break;
                }
            }
            {
                w_data_len = 4;
                net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                                       cb_net_data_att_status,
                                       0x015E,
                                       &w_data_len,
                                       (uint8_t *)&dw_power_saving,
                                       cb_device_data_access_operate_type_write);//
            }
//            dw_timestamp_today_backup = dw_timestamp_today;
        }

    }
#if( task_runing_status_print )
    DebugPrintf(TRUE,"Riiosunii_meter_data_process\r\n");
#endif
}

void system_riiosunii_inverter_data_process()
{
    uint16_t w_inverter_version = 0;
    uint16_t w_data_len = 2;
    uint16_t w_data_temp_1;
    uint16_t w_data_temp_2;

    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                           cb_net_data_att_status,
                           0x012C,
                           &w_data_len,
                           (uint8_t *)&w_inverter_version,
                           cb_device_data_access_operate_type_read);//hardware ver
    w_data_temp_1 = (uint8_t)w_inverter_version;
    if((1 == w_data_temp_1) || (13 == w_data_temp_1))
    {
        w_data_temp_2 = 2;
    }
    else
    {
        w_data_temp_2 = 4;
    }
    w_data_len = 1;
    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                           cb_net_data_att_set,
                           0x0038,
                           &w_data_len,
                           (uint8_t *)&w_data_temp_2,
                           cb_device_data_access_operate_type_write);//bat num

}
void system_ck_type_bat_full_charge()
{
    uint16_t w_data_len = 0;
	uint8_t b_bat_full_charge_status = 0;
	static uint8_t b_start_full_charge = 0;
	static uint8_t b_bat_full_charge_status_backup = 0;
	uint16_t w_acin_logic = 0;				//AC First
	static uint16_t w_acin_logic_backup = 0;		//AC First
	static uint32_t dw_current_full_charge_time = 0;
	static uint32_t dw_next_full_charge_time = 0;
	static uint32_t dw_current_timestamp = 0;
	uint32_t dw_current_time = 0;
	uint16_t w_full_charge_gap = 1;
	static uint16_t sw_full_charge_gap_backup = 0;
	uint16_t w_current_day_full_charge_time = 0;
	static uint16_t w_current_day_full_charge_time_backup = 0;
	uint16_t w_bat_soc = 0;
	uint16_t w_bat_i = 0;
	uint8_t b_bat_full_flag = 0;
	w_data_len = 1;

	net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                           cb_net_data_att_control,
                           0x0A00,
                           &w_data_len,
                           (uint8_t *)&b_bat_full_charge_status,
                           cb_device_data_access_operate_type_read);
    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                           cb_net_data_att_status,
                           0x017F,
                           &w_data_len,
                           (uint8_t *)&b_bat_full_charge_status,
                           cb_device_data_access_operate_type_write);
	w_data_len = 2;
	net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
						   cb_net_data_att_set,
						   0x003A,
						   &w_data_len,
						   (uint8_t *)&w_full_charge_gap,
						   cb_device_data_access_operate_type_read);
	w_data_len = 2;
	net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
	                       cb_net_data_att_set,
	                       0x003B,
	                       &w_data_len,
	                       (uint8_t *)&w_current_day_full_charge_time,
	                       cb_device_data_access_operate_type_read);
	if(w_full_charge_gap == 0)																	//满充间隔默认为1
	{
		w_full_charge_gap = 1;
		net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
						   cb_net_data_att_set,
						   0x003A,
						   &w_data_len,
						   (uint8_t *)&w_full_charge_gap,
						   cb_device_data_access_operate_type_write);
	}
	w_data_len = 4;
    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                           cb_net_data_att_status,
                           0x0180,
                           &w_data_len,
                           (uint8_t *)&dw_next_full_charge_time,
                           cb_device_data_access_operate_type_read);
	extern uint8_t gb_bat_full_charge_gap_flag;
	if((sw_full_charge_gap_backup != w_full_charge_gap ||
		w_current_day_full_charge_time != w_current_day_full_charge_time_backup)&& dw_next_full_charge_time != 0 && gb_bat_full_charge_gap_flag)			//满充间隔有变，更新下次满充时间
	{
		gb_bat_full_charge_gap_flag = 0;

		time_t timestamp = dw_next_full_charge_time;
		struct tm *tm_info;
		tm_info = localtime(&timestamp);
		tm_info->tm_hour = w_current_day_full_charge_time;		//修改用户自定义满充时间只需改这几个变量即可
		tm_info->tm_min = 0;
		tm_info->tm_sec = 0;
		dw_next_full_charge_time = mktime(tm_info);	
		dw_next_full_charge_time = dw_next_full_charge_time + (w_full_charge_gap - sw_full_charge_gap_backup) * 24 * 60 * 60;
		{
			w_data_len = 4;
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
						   cb_net_data_att_status,
						   0x0180,
						   &w_data_len,
						   (uint8_t *)&dw_next_full_charge_time,
						   cb_device_data_access_operate_type_write);
			fWriteEEPage(26) = TRUE;
		}
	}
	sw_full_charge_gap_backup = w_full_charge_gap;
	w_current_day_full_charge_time_backup = w_current_day_full_charge_time;
	w_data_len = 2;
	net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                           cb_net_data_att_status,
                           0x011B,
                           &w_data_len,
                           (uint8_t *)&w_bat_soc,
                           cb_device_data_access_operate_type_read);//hardware ver  低8位SOC_Percent  高8位SOH_Percent
	net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                           cb_net_data_att_status,
                           0x0126,
                           &w_data_len,
                           (uint8_t *)&w_bat_i,
                           cb_device_data_access_operate_type_read);//电池电流
	//满充结束判断条件：SOC 达到100% 或者 SOC 为99%并且电池电流小于2A
    w_bat_soc = w_bat_soc & 0xff;
	if(w_bat_soc == 100 )
	{
		b_bat_full_flag = 1;
	}
	else if((w_bat_soc < 100) && ((0x8000 != (w_bat_i & 0x8000)) && (w_bat_i > 20)))
	{
		b_bat_full_flag = 0;
	}
	else if((w_bat_soc == 99) && (((0x8000 != (w_bat_i & 0x8000)) && (w_bat_i < 20)) || ((0x8000 == (w_bat_i & 0x8000)) && (w_bat_i > 0xFFEC))))
	{
		b_bat_full_flag = 1;
	}
	else
	{
		b_bat_full_flag = 0;
	}
	//电池满充
	if(b_bat_full_flag)
	{
		//有触发满充，将满充标志置0，更新下次满充时间
		if(b_bat_full_charge_status == 1)
		{
			b_bat_full_charge_status = 0;
			w_data_len = 1;
//			uint16_t w_current_day_full_charge_time = 0;
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
	                           cb_net_data_att_control,
	                           0x0A00,
	                           &w_data_len,
	                           (uint8_t *)&b_bat_full_charge_status,
	                           cb_device_data_access_operate_type_write);
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
	                           cb_net_data_att_status,
	                           0x017F,
	                           &w_data_len,
	                           (uint8_t *)&b_bat_full_charge_status,
	                           cb_device_data_access_operate_type_write);
			get_system_timestamp(&dw_current_full_charge_time);
			time_t timestamp = dw_current_full_charge_time;
			struct tm *tm_info;
			tm_info = localtime(&timestamp);
//			tm_info->tm_mday += 1;
			tm_info->tm_hour = w_current_day_full_charge_time;		//修改用户自定义满充时间只需改这几个变量即可
			tm_info->tm_min = 0;
			tm_info->tm_sec = 0;
			dw_current_full_charge_time = mktime(tm_info);		
			w_data_len = 2;
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
	                       cb_net_data_att_set,
	                       0x003A,
	                       &w_data_len,
	                       (uint8_t *)&w_full_charge_gap,
	                       cb_device_data_access_operate_type_read);
			dw_next_full_charge_time = dw_current_full_charge_time + w_full_charge_gap * 24 * 60 * 60;
			{
				w_data_len = 4;
				net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
	                           cb_net_data_att_status,
	                           0x0180,
	                           &w_data_len,
	                           (uint8_t *)&dw_next_full_charge_time,
	                           cb_device_data_access_operate_type_write);
				fWriteEEPage(26) = TRUE;
			}
		}
		//没有触发满充 更新满充完成时间、下次满充时间    		  待完善
		
	}
	if(b_bat_full_charge_status_backup != b_bat_full_charge_status)		//电池满充状态发送变化，开始满充判定
	{
		b_start_full_charge = 1;
	}
	if(1 == b_start_full_charge)
	{
		extern uint8_t gb_work_mode_flag;
		if(b_bat_full_charge_status == 1)		//如果触发了满充保存当前工作逻辑，并将当前工作逻辑改为acin first
		{
			w_acin_logic = 0;
			w_data_len = 2;
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
								   cb_device_data_access_data_type_set,
								   0x0020,
								   &w_data_len,
								   (uint8_t *)&w_acin_logic_backup,
								   cb_device_data_access_operate_type_read);
			network_device_config_sycn_trig(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
											cb_net_subset_com_set_cmd,
											0x0020,
											1,
											(uint8_t *)&w_acin_logic,
											1,
											TRUE,
											1);//sync_type_single
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
								   cb_device_data_access_data_type_set,
								   0x0020,
								   &w_data_len,
								   (uint8_t *)&w_acin_logic,
								   cb_device_data_access_operate_type_write);
			gb_work_mode_flag = 0;
		}
		else if(b_bat_full_charge_status == 0 && 0 == gb_work_mode_flag)		//如果取消满充或满充完成恢复满充前工作状态
		{
			network_device_config_sycn_trig(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
											cb_net_subset_com_set_cmd,
											0x0020,
											1,
											(uint8_t *)&w_acin_logic_backup,
											1,
											TRUE,
											1);//sync_type_single
			w_data_len = 2;
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
								   cb_device_data_access_data_type_set,
								   0x0020,
								   &w_data_len,
								   (uint8_t *)&w_acin_logic_backup,
								   cb_device_data_access_operate_type_write);

		}
		b_start_full_charge = 0;		//判定完成等待下次判断
	}

	b_bat_full_charge_status_backup = b_bat_full_charge_status;        
	w_data_len = 4;
    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                           cb_net_data_att_status,
                           0x0180,
                           &w_data_len,
                           (uint8_t *)&dw_next_full_charge_time,
                           cb_device_data_access_operate_type_read);
	get_system_timestamp(&dw_current_timestamp);

	//当前时间大于下次满充时间 开始满充
	if((dw_current_timestamp > dw_next_full_charge_time) && dw_next_full_charge_time > 0x3842A300)
	{
		b_bat_full_charge_status = 1;
		w_data_len = 1;
		net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
	                           cb_net_data_att_control,
	                           0x0A00,
	                           &w_data_len,
	                           (uint8_t *)&b_bat_full_charge_status,
	                           cb_device_data_access_operate_type_write);
		w_data_len = 1;
		net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
			                   cb_net_data_att_status,
			                   0x017F,
			                   &w_data_len,
			                   (uint8_t *)&b_bat_full_charge_status,
			                   cb_device_data_access_operate_type_write);
	}
	
}

void system_type_bat_full_charge()
{
    uint16_t w_data_len = 0;
	uint8_t b_bat_full_charge_status = 0;
	static uint8_t b_start_full_charge = 0;
	static uint8_t b_bat_full_charge_status_backup = 0;
	uint16_t w_acin_logic = 0;				//AC First
	static uint16_t w_acin_logic_backup = 0;		//AC First
	static uint32_t dw_current_full_charge_time = 0;
	static uint32_t dw_next_full_charge_time = 0;
	static uint32_t dw_current_timestamp = 0;
	uint32_t dw_current_time = 0;
	uint16_t w_full_charge_gap = 1;
	static uint16_t sw_full_charge_gap_backup = 0;
	uint16_t w_current_day_full_charge_time = 0;
	static uint16_t w_current_day_full_charge_time_backup = 0;
	uint16_t w_bat_soc = 0;
	uint16_t w_bat_i = 0;
	uint8_t b_bat_full_flag = 0;
//	w_data_len = 1;

//	net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
//                           cb_net_data_att_control,
//                           0x0A00,
//                           &w_data_len,
//                           (uint8_t *)&b_bat_full_charge_status,
//                           cb_device_data_access_operate_type_read);
//    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
//                           cb_net_data_att_status,
//                           0x017F,
//                           &w_data_len,
//                           (uint8_t *)&b_bat_full_charge_status,
//                           cb_device_data_access_operate_type_write);
	w_data_len = 2;
	net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
						   cb_net_data_att_set,
						   0x003A,
						   &w_data_len,
						   (uint8_t *)&w_full_charge_gap,
						   cb_device_data_access_operate_type_read);
	w_data_len = 2;
	net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
	                       cb_net_data_att_set,
	                       0x003B,
	                       &w_data_len,
	                       (uint8_t *)&w_current_day_full_charge_time,
	                       cb_device_data_access_operate_type_read);
	if(w_full_charge_gap == 0)																	//满充间隔默认为1
	{
		w_full_charge_gap = 1;
		net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
						   cb_net_data_att_set,
						   0x003A,
						   &w_data_len,
						   (uint8_t *)&w_full_charge_gap,
						   cb_device_data_access_operate_type_write);
	}
	w_data_len = 4;
    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                           cb_net_data_att_status,
                           0x0180,
                           &w_data_len,
                           (uint8_t *)&dw_next_full_charge_time,
                           cb_device_data_access_operate_type_read);
	extern uint8_t gb_bat_full_charge_gap_flag;
	if((sw_full_charge_gap_backup != w_full_charge_gap ||
		w_current_day_full_charge_time != w_current_day_full_charge_time_backup)&& dw_next_full_charge_time != 0 && gb_bat_full_charge_gap_flag)			//满充间隔有变，更新下次满充时间
	{
		gb_bat_full_charge_gap_flag = 0;
		dw_next_full_charge_time = dw_next_full_charge_time + (w_full_charge_gap - sw_full_charge_gap_backup) * 24 * 60 * 60 
									+ (w_current_day_full_charge_time - w_current_day_full_charge_time_backup) * 60 * 60;
		{
			w_data_len = 4;
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
						   cb_net_data_att_status,
						   0x0180,
						   &w_data_len,
						   (uint8_t *)&dw_next_full_charge_time,
						   cb_device_data_access_operate_type_write);
			fWriteEEPage(26) = TRUE;
		}
	}
	sw_full_charge_gap_backup = w_full_charge_gap;
	w_current_day_full_charge_time_backup = w_current_day_full_charge_time;
	w_data_len = 2;
	net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                           cb_net_data_att_status,
                           0x011B,
                           &w_data_len,
                           (uint8_t *)&w_bat_soc,
                           cb_device_data_access_operate_type_read);//hardware ver  低8位SOC_Percent  高8位SOH_Percent
	net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_ck_inverter_u1],
                           cb_net_data_att_status,
                           0x0126,
                           &w_data_len,
                           (uint8_t *)&w_bat_i,
                           cb_device_data_access_operate_type_read);//电池电流
	//满充结束判断条件：SOC 达到100% 或者 SOC 为99%并且电池电流小于2A
    w_bat_soc = w_bat_soc & 0xff;
	if(w_bat_soc == 100 )
	{
		b_bat_full_flag = 1;
	}
	else if((w_bat_soc < 100) && ((0x8000 != (w_bat_i & 0x8000)) && (w_bat_i > 20)))
	{
		b_bat_full_flag = 0;
	}
	else if((w_bat_soc == 99) && (((0x8000 != (w_bat_i & 0x8000)) && (w_bat_i < 20)) || ((0x8000 == (w_bat_i & 0x8000)) && (w_bat_i > 0xFFEC))))
	{
		b_bat_full_flag = 1;
	}
	else
	{
		b_bat_full_flag = 0;
	}
	//电池满充
	if(b_bat_full_flag)
	{
		//有触发满充，将满充标志置0，更新下次满充时间
		if(b_bat_full_charge_status == 1)
		{
			b_bat_full_charge_status = 0;
			w_data_len = 1;
//			uint16_t w_current_day_full_charge_time = 0;
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
	                           cb_net_data_att_control,
	                           0x0A00,
	                           &w_data_len,
	                           (uint8_t *)&b_bat_full_charge_status,
	                           cb_device_data_access_operate_type_write);
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
	                           cb_net_data_att_status,
	                           0x017F,
	                           &w_data_len,
	                           (uint8_t *)&b_bat_full_charge_status,
	                           cb_device_data_access_operate_type_write);
			get_system_timestamp(&dw_current_full_charge_time);
			time_t timestamp = dw_current_full_charge_time;
			struct tm *tm_info;
			tm_info = localtime(&timestamp);
//			tm_info->tm_mday += 1;
			tm_info->tm_hour = w_current_day_full_charge_time;		//修改用户自定义满充时间只需改这几个变量即可
			tm_info->tm_min = 0;
			tm_info->tm_sec = 0;
			dw_current_full_charge_time = mktime(tm_info);		
			w_data_len = 2;
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
	                       cb_net_data_att_set,
	                       0x003A,
	                       &w_data_len,
	                       (uint8_t *)&w_full_charge_gap,
	                       cb_device_data_access_operate_type_read);
			dw_next_full_charge_time = dw_current_full_charge_time + w_full_charge_gap * 24 * 60 * 60;
			{
				w_data_len = 4;
				net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
	                           cb_net_data_att_status,
	                           0x0180,
	                           &w_data_len,
	                           (uint8_t *)&dw_next_full_charge_time,
	                           cb_device_data_access_operate_type_write);
				fWriteEEPage(26) = TRUE;
			}
		}
		//没有触发满充 更新满充完成时间、下次满充时间    		  待完善
		
	}
	if(b_bat_full_charge_status_backup != b_bat_full_charge_status)		//电池满充状态发送变化，开始满充判定
	{
		b_start_full_charge = 1;
	}
	if(1 == b_start_full_charge)
	{
		extern uint8_t gb_work_mode_flag;
		if(b_bat_full_charge_status == 1)		//如果触发了满充保存当前工作逻辑，并将当前工作逻辑改为acin first
		{
			w_acin_logic = 0;
			w_data_len = 2;
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
								   cb_device_data_access_data_type_set,
								   0x0020,
								   &w_data_len,
								   (uint8_t *)&w_acin_logic_backup,
								   cb_device_data_access_operate_type_read);
			network_device_config_sycn_trig(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
											cb_net_subset_com_set_cmd,
											0x0020,
											1,
											(uint8_t *)&w_acin_logic,
											1,
											TRUE,
											1);//sync_type_single
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
								   cb_device_data_access_data_type_set,
								   0x0020,
								   &w_data_len,
								   (uint8_t *)&w_acin_logic,
								   cb_device_data_access_operate_type_write);
			gb_work_mode_flag = 0;
		}
		else if(b_bat_full_charge_status == 0 && 0 == gb_work_mode_flag)		//如果取消满充或满充完成恢复满充前工作状态
		{
			network_device_config_sycn_trig(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
											cb_net_subset_com_set_cmd,
											0x0020,
											1,
											(uint8_t *)&w_acin_logic_backup,
											1,
											TRUE,
											1);//sync_type_single
			w_data_len = 2;
			net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_cklcd_u1],
								   cb_device_data_access_data_type_set,
								   0x0020,
								   &w_data_len,
								   (uint8_t *)&w_acin_logic_backup,
								   cb_device_data_access_operate_type_write);

		}
		b_start_full_charge = 0;		//判定完成等待下次判断
	}

	b_bat_full_charge_status_backup = b_bat_full_charge_status;        
	w_data_len = 4;
    net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
                           cb_net_data_att_status,
                           0x0180,
                           &w_data_len,
                           (uint8_t *)&dw_next_full_charge_time,
                           cb_device_data_access_operate_type_read);
	get_system_timestamp(&dw_current_timestamp);

	//当前时间大于下次满充时间 开始满充
	if((dw_current_timestamp > dw_next_full_charge_time) && dw_next_full_charge_time > 0x3842A300)
	{
		b_bat_full_charge_status = 1;
		w_data_len = 1;
		net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
	                           cb_net_data_att_control,
	                           0x0A00,
	                           &w_data_len,
	                           (uint8_t *)&b_bat_full_charge_status,
	                           cb_device_data_access_operate_type_write);
		w_data_len = 1;
		net_device_data_access(dwa_device_id_kinergy_ii[cw_device_id_index_kinergy],
			                   cb_net_data_att_status,
			                   0x017F,
			                   &w_data_len,
			                   (uint8_t *)&b_bat_full_charge_status,
			                   cb_device_data_access_operate_type_write);
	}
	
}


void network_ck_type_data_process_func()
{
	get_system_parallel_mode();
	if(check_system_type(cb_inv_type_ck))
	{
		system_ck_type_bat_full_charge();
	}

    network_ck_type_energy_statistics();
    system_ck_type_power_current_count();
    system_ck_type_data_process();
//    network_ck_type_pv_data_process();
}
void network_riiosunii_data_process_func()
{
	get_system_parallel_mode();

    network_riiosunii_energy_statistics();
	system_riiosunii_power_current_count();
    network_riiosunii_type_pv_data_process();
    system_riiosunii_meter_data_process();
//    system_riiosunii_inverter_data_process();
//    device_control_ags_control();
    device_set_attribute_range_manage();
}
/************************************************************************************************
* Input: void
*
* Output: void
*
* Logic discription: data process main func +
*                    device sync main func +
*                    device realtime error code process func +
*                    device set range manage func
*
* run time:
************************************************************************************************/
uint8_t b_power_saving_start_flag = 0;
void vEnergyStatisticsTask(void* parameter)
{
	DebugPrintf(TRUE,"%d,vEnergyStatisticsTask_first_entry\n",__LINE__);
	b_power_saving_start_flag = 1;
    for(;;)
    {
		#if(task_call_rate_test)
		DebugPrintf(TRUE,"vEnergyStatisticsTask,start: %d,priority: %d\n",xTaskGetTickCount(),energy_statistics_task_priority);
		#endif
		#if( task_stack_high_water_mark )
			DebugPrintf(TRUE,"vEnergyStatisticsTask_size: %d\n",uxTaskGetStackHighWaterMark(NULL));
		#endif

        if(TRUE == check_system_init(cb_system_init_type_system))
        {
            #if( task_runing_status_print )
                DebugPrintf(TRUE,"\r\n");
            #endif

            network_data_process_func[enum_data_process_func_type]();
            device_config_sync_main();
            network_abnormal_code_fresh();

            #if( task_runing_status_print )
                DebugPrintf(TRUE,"\r\n");
            #endif
        }

		#if(task_call_rate_test)
		DebugPrintf(TRUE,"vEnergyStatisticsTask,end: %d,priority: %d\n",xTaskGetTickCount(),energy_statistics_task_priority);
		#endif

		vTaskDelay(pdMS_TO_TICKS_USER(10));
    }
}

void energy_statistics_task_init()
{
    BaseType_t xReturn = pdPASS;

    xReturn = xTaskCreate( vEnergyStatisticsTask,"vEnergyStatisticsTask",energy_statistics_task_stack_size,NULL,energy_statistics_task_priority,&energy_statistics_task_handle);
    if(pdFAIL == xReturn)
    {
        DebugPrintf(TRUE,"%d,vEnergyStatisticsTask Create Fail\n",__LINE__);
    }
}
