#include "net_device_data_system.h"
#include "application_general_macro.h"
#include "net_firmware_update.h"

#define cw_net_data_list_length_system    		     3

#define cw_net_set_data_number_system  	   		     10
static net_set_data_system st_net_set_data_system =
{
    0,
};
const net_data_info st_net_set_data_info_System[cw_net_set_data_number_system] =
{
    {0x0000,0x0000,(uint8_t *)&st_net_set_data_system.ba_0000,&st_net_data_att_rw_30_1b_1_set,&st_device_data_att_w_30_auxiliary_installation,},
    {0x4F00,0x0000,(uint8_t *)&st_net_set_data_system.ba_4F00,&st_net_data_att_rw_30_1b_1_set,&st_device_data_att_w_30_mppt_auxiliary_installation,},
    {0x0002,0x0000,(uint8_t *)&st_net_set_data_system.dw_0002,&st_net_data_att_rw_4_4b_1_set},
    {0x000B,0x0000,(uint8_t *)&st_net_set_data_system.ba_000B,&st_net_data_att_rw_129_1b_1_set},
    {0x000C,0x0000,(uint8_t *)&st_net_set_data_system.ba_000C,&st_net_data_att_rw_4_1b_1_set},
    {0x000D,0x0000,(uint8_t *)&st_net_set_data_system.w_000D,&st_net_data_att_rw_2_2b_1_set},
    {0x000F,0x0000,(uint8_t *)&st_net_set_data_system.ba_000F,&st_net_data_att_rw_30_1b_asciistring_set},
    {0x0010,0x0000,(uint8_t *)&st_net_set_data_system.ba_0010,&st_net_data_att_rw_30_1b_asciistring_set},
    {0xAABB,0x0000,(uint8_t *)&st_net_set_data_system.b_AABB,&st_net_data_att_rw_1_1b_1_set},
    {0x00FF,0x0000,(uint8_t *)&st_net_set_data_system.b_00FF,&st_net_data_att_rw_2_2b_1_set},
};

//#if (sizeof(net_set_data_system) != (st_net_data_att_rw_4_4b_1_set.b_len +\
//                                     st_net_data_att_rw_2_2b_1_set.b_len +\
//                                     st_net_data_att_rw_2_2b_1_set.b_len +\
//                                     st_net_data_att_rw_2_2b_1_set.b_len +\
//                                     st_net_data_att_rw_2_2b_1_set.b_len +\
//                                     st_net_data_att_rw_2_2b_1_set.b_len +\
//                                     st_net_data_att_rw_30_1b_1_set.b_len +\
//                                     st_net_data_att_r_41_1b_1_set.b_len))
//    #error system set data length error
//#endif

#define cw_net_control_data_number_system  	   		 3
net_data_info st_net_control_data_info_System[cw_net_control_data_number_system] =
{
    {0x1000,0x0000,0,&st_net_data_att_r_41_1b_1_control},
    {0x00FF,0x0000,0,&st_net_data_att_w_2_2b_1_control},
    {0xFFF1,0x0000,0,&st_net_data_att_w_2_2b_1_control},
};

#define cw_net_status_data_number_system  	   		 10

static net_status_data_system st_net_status_data_system =
{
    .w_0100 = cw_net_max_receive_package_len,
};

const net_data_info st_net_status_data_info_System[cw_net_status_data_number_system] =
{
    {0x0100,0x0000,(uint8_t *)&st_net_status_data_system.w_0100,&st_net_data_att_r_2_2b_1_status},
    {0x0101,0x0000,(uint8_t *)&st_net_status_data_system.w_0101,&st_net_data_att_r_2_2b_1_status},
    {0x0102,0x0000,(uint8_t *)&st_net_status_data_system.dw_0102,&st_net_data_att_r_4_4b_1_status},
    {0x0103,0x0000,(uint8_t *)&st_net_status_data_system.w_0103,&st_net_data_att_r_2_2b_per_status},
    {0x0104,0x0000,(uint8_t *)&st_net_status_data_system.b_0104,&st_net_data_att_r_1_1b_1_status},
    {0x0105,0x0000,(uint8_t *)&st_net_status_data_system.b_0105,&st_net_data_att_r_1_1b_1_status},
    {0x0106,0x0000,(uint8_t *)&st_net_status_data_system.b_0106,&st_net_data_att_r_1_1b_1_status},
    {0x0107,0x0000,(uint8_t *)&st_net_status_data_system.ba_0107,&st_net_data_att_r_30_1b_1_status},
    {0x0109,0x0000,(uint8_t *)&st_net_status_data_system.b_0109,&st_net_data_att_r_1_1b_1_status},
    {0x010A,0x0000,(uint8_t *)&st_net_status_data_system.b_010A,&st_net_data_att_r_1_1b_1_status},
//    {0x011B,0x0000,(uint8_t *)&st_net_status_data_system.ba_011B[0],&st_net_data_att_r_6_1b_1_1_status},
//    {0x0200,0x0000,0,&st_net_data_att_r_1_1b_1_status},
};

const net_data_list st_net_data_list_system[cw_net_data_list_length_system] =
{
    {
        cb_net_data_att_set,
		cw_net_set_data_number_system,
		&st_net_set_data_info_System[0],
    },
    {
		cb_net_data_att_status,
		cw_net_status_data_number_system,
		&st_net_status_data_info_System[0],
	},
	{
		cb_net_data_att_control,
		cw_net_control_data_number_system,
		&st_net_control_data_info_System[0],
	}
};

const net_data_list_info st_net_data_list_info_system =
{
    cw_net_data_list_length_system,
    &st_net_data_list_system[0],
};
