#include "rs485_communicate_protocal.h"
#include "application_general_macro.h"
#include "crc16_arithmetic.h"
#include "rs485.h"

uint8_t send_rs485_event_raw_data(uint8_t b_type,
                                  void *pv_data,
                                  uint16_t *pw_data_out_len,
                                  uint8_t *pb_data_out)
{
    if((NULL == pv_data) ||\
       (NULL == pw_data_out_len) ||\
       (NULL == pb_data_out))
    {
        return FALSE;
    }

    switch(b_type)
    {
        case cb_rs485_cmd_type_inquire:
        case cb_rs485_cmd_type_set:
        case cb_rs485_cmd_type_ctrl:
        case cb_rs485_cmd_type_event_logs:
        case cb_rs485_cmd_type_subset_update_pacx_dsp:
            {
                rs485_protocal_info *pst_rs485_protocal_info = (rs485_protocal_info *)pv_data;

                rs485_protocal_tx_data_assamble(pst_rs485_protocal_info->b_device_id,
                                                ((uint16_t)((((uint16_t)(pst_rs485_protocal_info->b_first_cmd)) << 8) |\
                                                              (uint16_t)pst_rs485_protocal_info->b_sub_cmd)),
                                                pst_rs485_protocal_info->w_body_data_len,
                                                pst_rs485_protocal_info->pb_body_data,
                                                pb_data_out,
                                                pw_data_out_len);
            }
            break;
        case cb_rs485_cmd_type_inv_auxiliary_installation:
            {
                rs485_protocal_info *pst_rs485_protocal_info = (rs485_protocal_info *)pv_data;

                rs485_protocal_tx_inv_auxiliary_installation_assamble(pst_rs485_protocal_info->b_device_id,
                                                ((uint16_t)((((uint16_t)(pst_rs485_protocal_info->b_first_cmd)) << 8) |\
                                                              (uint16_t)pst_rs485_protocal_info->b_sub_cmd)),
                                                pst_rs485_protocal_info->w_body_data_len,
                                                pst_rs485_protocal_info->pb_body_data,
                                                pb_data_out,
                                                pw_data_out_len);
            }
            break;
       case cb_rs485_cmd_type_mppt_auxiliary_installation:
            {
                rs485_protocal_info *pst_rs485_protocal_info = (rs485_protocal_info *)pv_data;

                rs485_protocal_tx_mppt_auxiliary_installation_assamble(pst_rs485_protocal_info->b_device_id,
                                                ((uint16_t)((((uint16_t)(pst_rs485_protocal_info->b_first_cmd)) << 8) |\
                                                              (uint16_t)pst_rs485_protocal_info->b_sub_cmd)),
                                                pst_rs485_protocal_info->w_body_data_len,
                                                pst_rs485_protocal_info->pb_body_data,
                                                pb_data_out,
                                                pw_data_out_len);
            }
            break;
		case cb_rs485_cmd_type_inquire_device_tree:
        case cb_rs485_cmd_type_subset_update_pac:
            {
                rs485_protocal_info *pst_rs485_protocal_info = (rs485_protocal_info *)pv_data;

                rs485_protocal_tx_update_cmd_assamble(pst_rs485_protocal_info->b_device_id,
                                                      ((uint16_t)((((uint16_t)(pst_rs485_protocal_info->b_first_cmd)) << 8) |\
                                                                    (uint16_t)pst_rs485_protocal_info->b_sub_cmd)),
                                                      pst_rs485_protocal_info->w_body_data_len,
                                                      pst_rs485_protocal_info->pb_body_data,
                                                      pb_data_out,
                                                      pw_data_out_len);
            }
            break;
        case cb_rs485_cmd_type_subset_update_pacx_mcu:
            {
                return FALSE;//mark subset update rs485 cmd
            }
            break;
        case cb_rs485_cmd_type_subset_update_transparent_cmd:
            {
                rs485_protocal_info *pst_rs485_protocal_info = (rs485_protocal_info *)pv_data;

                if(pst_rs485_protocal_info->w_body_data_len > rs485_mps_size)
                {
                    return FALSE;
                }

                for(uint16_t w_cnt_i = 0;w_cnt_i < pst_rs485_protocal_info->w_body_data_len;w_cnt_i++)
                {
                    pb_data_out[w_cnt_i] = pst_rs485_protocal_info->pb_body_data[w_cnt_i];
                }
                *pw_data_out_len = pst_rs485_protocal_info->w_body_data_len;
            }
            break;
//        case cb_rs485_cmd_type_update:
//            {
//            }
//            break;
//		case cb_rs485_cmd_type_inquire_device_alarm:
//            {
//            }
//            break;
        case cb_rs485_cmd_type_product_test:
        case cb_rs485_cmd_type_product_test_inquire:
            {
                product_test_protocal_info *pst_product_test_protocal_info =\
                    (product_test_protocal_info *)pv_data;
                rs485_product_test_tx_data_assamble(pst_product_test_protocal_info->b_com_addr,
                                                    pst_product_test_protocal_info->b_func_code,
                                                    pst_product_test_protocal_info->w_ctrl_addr,
                                                    pst_product_test_protocal_info->w_body_data_len,
                                                    pst_product_test_protocal_info->pb_body_data,
                                                    pb_data_out,
                                                    pw_data_out_len);
            }
            break;
		default:
			return FALSE;
    }

    return TRUE;
}

uint8_t rs485_protocal_tx_data_assamble(uint8_t b_device_id,
                                        uint16_t w_cmd,
                                        uint16_t w_body_data_len,
                                        uint8_t *pb_body_data,
                                        uint8_t *pb_data_out,
                                        uint16_t *pw_data_out_len)
{
    if((0 == pb_body_data) ||\
       (0 == pb_data_out) ||\
       (0 == pw_data_out_len))
    {
        return FALSE;
    }

    uint8_t b_crc16_h = 0xFF;
    uint8_t b_crc16_l = 0xFF;

    *pw_data_out_len = 0;

    pb_data_out[(*pw_data_out_len)++] = 0x7E;
    pb_data_out[(*pw_data_out_len)++] = 0xFF;

    pb_data_out[(*pw_data_out_len)++] = b_device_id;

    pb_data_out[(*pw_data_out_len)++] = (uint8_t)(w_cmd >> 8);
    pb_data_out[(*pw_data_out_len)++] = (uint8_t)w_cmd;

    pb_data_out[(*pw_data_out_len)++] = w_body_data_len + cb_rs485_header_len + 2;

    for(uint8_t b_cnt_i = 0;b_cnt_i < w_body_data_len;b_cnt_i++)
    {
        pb_data_out[(*pw_data_out_len)++] = pb_body_data[b_cnt_i];
    }
    calculate_crc16(pb_data_out,*pw_data_out_len,&b_crc16_h,&b_crc16_l);
    pb_data_out[(*pw_data_out_len)++] = b_crc16_h;
    pb_data_out[(*pw_data_out_len)++] = b_crc16_l;

    return TRUE;
}

uint8_t rs485_protocal_tx_inv_auxiliary_installation_assamble(uint8_t b_device_id,
                                        uint16_t w_cmd,
                                        uint16_t w_body_data_len,
                                        uint8_t *pb_body_data,
                                        uint8_t *pb_data_out,
                                        uint16_t *pw_data_out_len)
{
    if((0 == pb_body_data) ||\
       (0 == pb_data_out) ||\
       (0 == pw_data_out_len))
    {
        return FALSE;
    }

    uint8_t b_crc16_h = 0xFF;
    uint8_t b_crc16_l = 0xFF;

    *pw_data_out_len = 0;

    pb_data_out[(*pw_data_out_len)++] = 0x7E;
    pb_data_out[(*pw_data_out_len)++] = 0xFF;

    pb_data_out[(*pw_data_out_len)++] = 0x00;

    pb_data_out[(*pw_data_out_len)++] = 0x06;
    pb_data_out[(*pw_data_out_len)++] = 0x0F;

    pb_data_out[(*pw_data_out_len)++] = w_body_data_len + cb_rs485_header_len;

    for(uint8_t b_cnt_i = 2;b_cnt_i < w_body_data_len;b_cnt_i++)
    {
        pb_data_out[(*pw_data_out_len)++] = pb_body_data[b_cnt_i];
    }
    calculate_crc16(pb_data_out,*pw_data_out_len,&b_crc16_h,&b_crc16_l);
    pb_data_out[(*pw_data_out_len)++] = b_crc16_h;
    pb_data_out[(*pw_data_out_len)++] = b_crc16_l;

    return TRUE;
}
uint8_t rs485_protocal_tx_mppt_auxiliary_installation_assamble(uint8_t b_device_id,
                                        uint16_t w_cmd,
                                        uint16_t w_body_data_len,
                                        uint8_t *pb_body_data,
                                        uint8_t *pb_data_out,
                                        uint16_t *pw_data_out_len)
{
    if((0 == pb_body_data) ||\
       (0 == pb_data_out) ||\
       (0 == pw_data_out_len))
    {
        return FALSE;
    }

    uint8_t b_crc16_h = 0xFF;
    uint8_t b_crc16_l = 0xFF;

    *pw_data_out_len = 0;

    pb_data_out[(*pw_data_out_len)++] = 0x7E;
    pb_data_out[(*pw_data_out_len)++] = 0xFF;

    pb_data_out[(*pw_data_out_len)++] = b_device_id;

    pb_data_out[(*pw_data_out_len)++] = (uint8_t)(w_cmd >> 8);;
    pb_data_out[(*pw_data_out_len)++] = (uint8_t)w_cmd;

    pb_data_out[(*pw_data_out_len)++] = w_body_data_len + cb_rs485_header_len - 2;

    for(uint8_t b_cnt_i = 0;b_cnt_i < w_body_data_len - 4;b_cnt_i++)
    {
        pb_data_out[(*pw_data_out_len)++] = pb_body_data[b_cnt_i];
    }
    calculate_crc16(pb_data_out,*pw_data_out_len,&b_crc16_h,&b_crc16_l);
    pb_data_out[(*pw_data_out_len)++] = b_crc16_h;
    pb_data_out[(*pw_data_out_len)++] = b_crc16_l;

    return TRUE;
}

uint8_t rs485_protocal_tx_update_cmd_assamble(uint8_t b_device_id,
                                              uint16_t w_cmd,
                                              uint16_t w_body_data_len,
                                              uint8_t *pb_body_data,
                                              uint8_t *pb_data_out,
                                              uint16_t *pw_data_out_len)
{
    if((0 == pb_body_data) ||\
       (0 == pb_data_out) ||\
       (0 == pw_data_out_len))
    {
        return FALSE;
    }

    uint8_t b_crc16_h = 0xFF;
    uint8_t b_crc16_l = 0xFF;

    *pw_data_out_len = 0;

    pb_data_out[(*pw_data_out_len)++] = b_device_id;
    pb_data_out[(*pw_data_out_len)++] = 0xFF;

    pb_data_out[(*pw_data_out_len)++] = (uint8_t)(w_cmd >> 8);
    pb_data_out[(*pw_data_out_len)++] = (uint8_t)w_cmd;

    pb_data_out[(*pw_data_out_len)++] = (uint8_t)((w_body_data_len & 0xFF00) >> 8);
    pb_data_out[(*pw_data_out_len)++] = (uint8_t)(w_body_data_len & 0x00FF);

    for(uint16_t w_cnt_i = 0;w_cnt_i < w_body_data_len;w_cnt_i++)
    {
        pb_data_out[(*pw_data_out_len)++] = pb_body_data[w_cnt_i];
    }
    calculate_crc16(pb_data_out,*pw_data_out_len,&b_crc16_h,&b_crc16_l);
    pb_data_out[(*pw_data_out_len)++] = b_crc16_h;
    pb_data_out[(*pw_data_out_len)++] = b_crc16_l;

    return TRUE;
}

uint8_t rs485_product_test_tx_data_assamble(uint8_t b_com_addr,
                                            uint8_t b_func_code,
                                            uint16_t w_ctrl_addr,
                                            uint16_t w_body_data_len,
                                            uint8_t *pb_body_data,
                                            uint8_t *pb_data_out,
                                            uint16_t *pw_data_out_len)
{
    if((0 == pb_body_data) ||\
       (0 == pb_data_out) ||\
       (0 == pw_data_out_len))
    {
        return FALSE;
    }

    uint8_t b_crc16_h = 0xFF;
    uint8_t b_crc16_l = 0xFF;

    *pw_data_out_len = 0;

    pb_data_out[(*pw_data_out_len)++] = b_com_addr;
    pb_data_out[(*pw_data_out_len)++] = b_func_code;
    pb_data_out[(*pw_data_out_len)++] = (uint8_t)(w_ctrl_addr >> 8);
    pb_data_out[(*pw_data_out_len)++] = (uint8_t)w_ctrl_addr;
    pb_data_out[(*pw_data_out_len)++] = (uint8_t)(w_body_data_len >> 8);
    pb_data_out[(*pw_data_out_len)++] = (uint8_t)w_body_data_len;

    for(uint8_t b_cnt_i = 0;b_cnt_i < w_body_data_len;b_cnt_i++)
    {
        pb_data_out[(*pw_data_out_len)++] = pb_body_data[b_cnt_i];
    }
    calculate_crc16(pb_data_out,*pw_data_out_len,&b_crc16_h,&b_crc16_l);
    pb_data_out[(*pw_data_out_len)++] = b_crc16_h;
    pb_data_out[(*pw_data_out_len)++] = b_crc16_l;

    return TRUE;
}
