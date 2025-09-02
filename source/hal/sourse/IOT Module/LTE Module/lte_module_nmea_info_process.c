#include "lte_module_nmea_info_process.h"
#include "lte_module.h"
#include "application_general_macro.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "FREERTOS.h"
#include "task.h"
#include "net_system_info_list.h"
#include "net_device_data_access.h"
#include "net_device_tree_system.h"
#include "net_system_info_access.h"
#include "application_general_func.h"
#include "system_timestamp_generate.h"
#include "independ_user_data_operate.h"
/*
$GPVTG
$GPRMC
$GPGSA
$GPGSV #1
$GPGSV #2
$GPGGA
*/
uint32_t dw_total_distance = 0;
uint32_t dw_timestamp_backup = 0;
uint32_t dw_distance = 0;

uint8_t lte_module_nmea_info_receive(uint8_t *pb_data_out,uint16_t *pw_data_out_len,uint16_t w_data_out_buffer_size)
{
    static uint16_t w_nmea_info_data_index = 0;
//    static uint8_t b_start = TRUE;
    uint8_t ba_data[lte_module_mps_size] = {0,};
    lte_module_data_info st_lte_module_data_info = {0,};
    st_lte_module_data_info.pb_data = ba_data;
//    uint8_t b_result = FALSE;


    if(FALSE == lte_module_connect_check())
    {
//        b_start = TRUE;
        *pw_data_out_len = 0;
        return FALSE;
    }

//    uint8_t b_data_temp = 0;
    //if(lte_module_get_parameter(cb_lte_module_gps_on,&b_data_temp))
//    if(1)
//    {
//        if(0 == b_data_temp)
//        {
//            b_start = TRUE;
//            *pw_data_out_len = 0;
//            return FALSE;
//        }
//    }
//    else
//    {
//		*pw_data_out_len = 0;
//
//        b_start = TRUE;
//		w_nmea_info_data_index = 0;
//        return FALSE;
//    }

	if(FALSE == usb_host_nmea_port_receive_data(st_lte_module_data_info.pb_data,(uint8_t *)&st_lte_module_data_info.w_data_len))
	{
		return FALSE;
	}

//	DebugPrintf(TRUE,"usb_nmea_raw_data_len: %d\n",st_lte_module_data_info.w_data_len);
//	DebugPrintf(TRUE,"usb_nmea_raw: %s",st_lte_module_data_info.pb_data);

	{
		uint8_t *pb_data_temp = 0;
		uint8_t b_data_len_temp = 0;

		pb_data_temp = (uint8_t *)strstr((char *)st_lte_module_data_info.pb_data,"$GPRMC");

		if((w_nmea_info_data_index + st_lte_module_data_info.w_data_len) >= w_data_out_buffer_size)
		{
//			b_start = TRUE;
			*pw_data_out_len = w_nmea_info_data_index;
			pb_data_out[*pw_data_out_len] = '\0';

			w_nmea_info_data_index = 0;

//			b_result = TRUE;
		}
		else
		{
			if(pb_data_temp != NULL)
			{
//				if(TRUE == b_start)
				{
//					b_start = FALSE;

					b_data_len_temp = st_lte_module_data_info.w_data_len - (pb_data_temp - st_lte_module_data_info.pb_data);

//                    DebugPrintf(TRUE,"nmea_info_rx :%d,0x%x\n",b_data_len_temp,pb_data_temp);
//                    DebugPrintf(TRUE,"nmea_info_cocy :%d,0x%x\n",w_nmea_info_data_index,pb_data_out);

					memset(pb_data_out,0,w_data_out_buffer_size);
					w_nmea_info_data_index = 0;

					memcpy(&pb_data_out[w_nmea_info_data_index],\
						   pb_data_temp,\
						   b_data_len_temp);

					w_nmea_info_data_index += b_data_len_temp;
				}
//				else
//				{
//					b_start = TRUE;
//
//					b_data_len_temp = (pb_data_temp - st_lte_module_data_info.pb_data);
//
////                    DebugPrintf(TRUE,"nmea_info_rx :%d,0x%x\n",b_data_len_temp,pb_data_temp);
////                    DebugPrintf(TRUE,"nmea_info_cocy :%d,0x%x\n",w_nmea_info_data_index,pb_data_out);
//
//					memcpy(&pb_data_out[w_nmea_info_data_index],\
//						   st_lte_module_data_info.pb_data,\
//						   b_data_len_temp);
//
//					*pw_data_out_len = w_nmea_info_data_index + b_data_len_temp;
//					pb_data_out[*pw_data_out_len] = '\0';
//
//					w_nmea_info_data_index = 0;
//
////					b_result = TRUE;
//				}
			}
			else
			{
//                DebugPrintf(TRUE,"nmea_info_rx :%d,0x%x\n",b_data_len_temp,pb_data_temp);
//                DebugPrintf(TRUE,"nmea_info_cocy :%d,0x%x\n",w_nmea_info_data_index,pb_data_out);

				memcpy(&pb_data_out[w_nmea_info_data_index],\
					   st_lte_module_data_info.pb_data,\
					   st_lte_module_data_info.w_data_len);

				w_nmea_info_data_index += st_lte_module_data_info.w_data_len;
			}
		}
	}

    return TRUE;
}

uint8_t lte_module_nmea_info_verify(uint8_t *pb_data_in,uint16_t w_data_in_len)
{
    #define cb_nmea_check_str_num   4
    const uint8_t *pba_nmea_check_str[cb_nmea_check_str_num] =
    {
        "$GPVTG",
        "$GPGSA",
        "$GPGGA",
        "$GPRMC",
    };

    uint8_t ba_gps_info_xor_check_receive_str[3] = {0,};
    uint32_t dw_gps_info_xor_check_receive_hex = 0;
    uint8_t b_gps_info_xor_check_calculate = 0;
    uint8_t *pb_data = 0;

    pb_data = pb_data_in;

    for(uint8_t b_cnt_i = 0;b_cnt_i < cb_nmea_check_str_num;b_cnt_i++)
    {
        pb_data = (uint8_t *)strstr((char *)pb_data_in,(char *)pba_nmea_check_str[b_cnt_i]);

        if(pb_data != 0)
        {
            pb_data++;
            b_gps_info_xor_check_calculate = pb_data[0];
            pb_data++;

            for(;;)
            {
                if(*pb_data == '\0')
                {
                    return FALSE;
                }
                if(*pb_data != '*')
                {
                    b_gps_info_xor_check_calculate ^= *pb_data;
                    pb_data++;
                }
                else
                {
                    pb_data++;

                    ba_gps_info_xor_check_receive_str[0] = *pb_data;
                    pb_data++;

                    ba_gps_info_xor_check_receive_str[1] = *pb_data;
                    ba_gps_info_xor_check_receive_str[2] = '\0';
                    pb_data++;

                    pb_data += 2;//0d 0a

                    break;
                }
            }

            if(sscanf((const char *)ba_gps_info_xor_check_receive_str,"%x",&dw_gps_info_xor_check_receive_hex))
            {
                if(dw_gps_info_xor_check_receive_hex != b_gps_info_xor_check_calculate)
                {
                    return FALSE;
                }
            }
            else
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}

//extern double abs(double);
double abs_double(double i)
{
return i < 0 ? -i : i;
}


#define earth_radius 		(double)6378137.0//unit:m

#define pi	 			    (double)3.1415926535898

#define rad(d)			    (double)(d * pi / 180.0)

double lte_module_nmea_lantitude_longitude_distance(double lon1, double lat1,double lon2, double lat2)
{
	double radLat1 = rad(lat1);
	double radLat2 = rad(lat2);

	double radLon1 = rad(lon1);
	double radLon2 = rad(lon2);

	if (radLat1 < 0)
		radLat1 = pi / 2 + abs_double(radLat1);// south
	if (radLat1 > 0)
		radLat1 = pi / 2 - abs_double(radLat1);// north
	if (radLon1 < 0)
		radLon1 = pi * 2 - abs_double(radLon1);// west
	if (radLat2 < 0)
		radLat2 = pi / 2 + abs_double(radLat2);// south
	if (radLat2 > 0)
		radLat2 = pi / 2 - abs_double(radLat2);// north
	if (radLon2 < 0)
		radLon2 = pi * 2 - abs_double(radLon2);// west

	double x1 = earth_radius * cos(radLon1) * sin(radLat1);
	double y1 = earth_radius * sin(radLon1) * sin(radLat1);
	double z1 = earth_radius * cos(radLat1);

	double x2 = earth_radius * cos(radLon2) * sin(radLat2);
	double y2 = earth_radius * sin(radLon2) * sin(radLat2);
	double z2 = earth_radius * cos(radLat2);

	double d = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)+ (z1 - z2) * (z1 - z2));

	double theta = acos((earth_radius * earth_radius + earth_radius * earth_radius - d * d) / (2 * earth_radius * earth_radius));
	double dist = theta * earth_radius;

    return dist;
}

//$GPVTG,4.3,T,7.4,M,0.0,N,0.0,K,A*27
//$GPRMC,032457.00,A,2433.448464,N,11754.701485,E,0.0,4.3,060921,3.0,W,A,V*53
//$GPGSA,A,3,02,05,06,09,12,13,19,20,30,,,,1.0,0.7,0.7,1*2E
//$GPGSV,4,1,15,02,57,018,30,05,46,281,24,06,37,075,32,09,08,037,21,1*62

//$GPVTG,,T,,M,,N,,K,N*2C
//$GPGSA,A,1,,,,,,,,,,,,,,,*1E
//$GPGGA,,,,,,0,,,,,,,,*66
//$GPRMC,,V,,,,,,,,,,N*53

void lte_module_nmea_info_get()
{
    #define cb_nmea_info_receive            1
    #define cb_nmea_info_verify             2
    #define cb_nmea_info_store              3

    #define cw_nmea_info_max_len            300
	static uint8_t ba_nmea_info[cw_nmea_info_max_len] = {0,};
    static uint8_t *pb_nmea_info = &ba_nmea_info[0];

    uint16_t w_nmea_info_len = 0;

    static uint8_t b_step = cb_nmea_info_receive;

    switch(b_step)
    {
        case cb_nmea_info_receive:
        {
			#if(lte_nmea_info_test)
            DebugPrintf(TRUE,"usb_nmea_info_nmea_info_receive_start: %d\n",xTaskGetTickCount());
			#endif
            if(TRUE == lte_module_nmea_info_receive(pb_nmea_info,&w_nmea_info_len,cw_nmea_info_max_len))
            {
				#if(lte_nmea_info_test)
                DebugPrintf(TRUE,"usb_nmea_info_nmea_info_receive_end: %d\n",xTaskGetTickCount());
				#endif
                b_step = cb_nmea_info_verify;
            }
            break;
        }
        case cb_nmea_info_verify:
        {
			#if(lte_nmea_info_test)
            DebugPrintf(TRUE,"usb_nmea_info_nmea_info_verify_start: %d\n",xTaskGetTickCount());
			#endif
            if(TRUE == lte_module_nmea_info_verify(pb_nmea_info,w_nmea_info_len))
            {
				#if(lte_nmea_info_test)
                DebugPrintf(TRUE,"usb_nmea_info_nmea_info_verify_end: %d\n",xTaskGetTickCount());
				#endif
                b_step = cb_nmea_info_store;
            }
            else
            {

                b_step = cb_nmea_info_receive;
            }
            break;
        }
        case cb_nmea_info_store:
        {
            /*
			double float_distance_temp = 0;
            uint8_t ba_longitude[2][15] = {{"11754.693123E"},{"11754.619456E"}};
            uint8_t ba_latitude[2][15] = {{"2433.439316N"},{"2433.439913N"}};
            static uint8_t b_index = 0;
			double float_longitude_current = 0;
			static double float_longitude_backup = 0;
			double float_latitude_current = 0;
			static double float_latitude_backup = 0;
			static uint32_t dw_distance = 0;
            uint16_t w_integer = 0;

            b_index = !b_index;
            float_longitude_current = atof((const char*)ba_longitude[b_index]) / 100;
            float_latitude_current = atof((const char*)ba_latitude[b_index]) / 100;

            w_integer = (uint16_t)float_longitude_current;
            float_longitude_current = w_integer + (float_longitude_current - w_integer)*100/60;

            w_integer = (uint16_t)float_latitude_current;
            float_latitude_current = w_integer + (float_latitude_current - w_integer)*100/60;
			{
                uint16_t w_data_len = 13;
                net_system_data_access(cb_device_data_access_data_type_status,
                                       cw_sys_status_data_addr_longitude,
                                       &w_data_len,
                                       ba_longitude[b_index],
                                       cb_device_data_access_operate_type_write);
			}
			{
                uint16_t w_data_len = 12;
                net_system_data_access(cb_device_data_access_data_type_status,
                                       cw_sys_status_data_addr_latitude,
                                       &w_data_len,
                                       ba_latitude[b_index],
                                       cb_device_data_access_operate_type_write);
			}
			if((float_longitude_current != 0) && (float_latitude_current != 0))
			{
				if((float_longitude_current != float_longitude_backup) ||\
				   (float_latitude_current != float_latitude_backup))
				{

					if((0 == float_longitude_backup) ||\
					   (0 == float_latitude_backup))
					{
						float_longitude_backup = float_longitude_current;
						float_latitude_backup = float_latitude_current;
					}


					float_distance_temp = lte_module_nmea_lantitude_longitude_distance(float_longitude_current,
                                                                                       float_latitude_current,
                                                                                       float_longitude_backup,
                                                                                       float_latitude_backup);

//					if((uint32_t)float_distance_temp >= 3)
					{
						dw_distance = (uint32_t)float_distance_temp;
					}
					float_longitude_backup = float_longitude_current;
					float_latitude_backup = float_latitude_current;

                    {
                        uint16_t w_data_len = 4;
                        net_system_data_access(cb_device_data_access_data_type_status,
                                               cw_sys_status_data_addr_day_mileage,
                                               &w_data_len,
                                               (uint8_t *)&dw_distance,
                                               cb_device_data_access_operate_type_write);
                    }
					#if(lte_nmea_info_test)
					DebugPrintf(TRUE,"Distance: %d\n",dw_distance);
                    DebugPrintf(TRUE,"Total distance: %d\n",dw_total_distance);
					#endif
				}
			}
//            */
//            /*
			#if(lte_nmea_info_test)
            DebugPrintf(TRUE,"usb_nmea_info_nmea_info_store_start: %d\n",xTaskGetTickCount());
            #endif
            uint8_t ba_str[15];
            uint8_t *pb_str = 0;

			double float_distance_temp = 0;
            static uint32_t dw_distance_backup = 0;
			double float_longitude_current = 0;
			static double float_longitude_backup = 0;
			double float_latitude_current = 0;
			static double float_latitude_backup = 0;
            uint16_t w_integer = 0;

            //longitude
            memset(ba_str,0,15);
            pb_str = (uint8_t *)strstr((const char*)pb_nmea_info,"$GPRMC");

            sGetStrFromSpecRangeWithChar(',',5,',',1,pb_str,ba_str,12);
			float_longitude_current = atof((const char*)ba_str) / 100;
            w_integer = (uint16_t)float_longitude_current;
            float_longitude_current = w_integer + (float_longitude_current - w_integer)*100/60;

			sGetStrFromSpecRangeWithChar(',',6,',',1,pb_str,&ba_str[12],1);

			if((float_longitude_current != 0) &&\
               (((dw_distance - dw_distance_backup) >= 3) ||\
                (0 == float_longitude_backup)))
			{
                uint16_t w_data_len = 13;
                net_system_data_access(cb_device_data_access_data_type_status,
                                       cw_sys_status_data_addr_longitude,
                                       &w_data_len,
                                       ba_str,
                                       cb_device_data_access_operate_type_write);
			}
			#if(lte_nmea_info_test)
			DebugPrintf(TRUE,"Latitude: %s\n",ba_str);
			#endif
            //latitude
            memset(ba_str,0,15);
            pb_str = (uint8_t *)strstr((const char*)pb_nmea_info,"$GPRMC");

            sGetStrFromSpecRangeWithChar(',',3,',',1,pb_str,ba_str,11);
			float_latitude_current = strtof((const char*)ba_str,NULL) / 100;
            w_integer = (uint16_t)float_latitude_current;
            float_latitude_current = w_integer + (float_latitude_current - w_integer)*100/60;

			sGetStrFromSpecRangeWithChar(',',4,',',1,pb_str,&ba_str[11],1);

			if((float_latitude_current != 0) &&\
               (((dw_distance - dw_distance_backup) >= 3) ||\
                (0 == float_latitude_backup)))
			{
                uint16_t w_data_len = 12;
                dw_distance_backup = dw_distance;
                net_system_data_access(cb_device_data_access_data_type_status,
                                       cw_sys_status_data_addr_latitude,
                                       &w_data_len,
                                       ba_str,
                                       cb_device_data_access_operate_type_write);
			}
			#if(lte_nmea_info_test)
            DebugPrintf(TRUE,"Longitude: %s\n",ba_str);
            #endif
            //speed
            memset(ba_str,0,15);
            pb_str = (uint8_t *)strstr((const char*)pb_nmea_info,"$GPRMC");
            sGetStrFromSpecRangeWithChar(',',7,',',1,pb_str,ba_str,15);
			#if(lte_nmea_info_test)
            DebugPrintf(TRUE,"Speed: %s\n",ba_str);
            #endif
            double float_nmea_speed = 0;
            uint32_t dw_nmea_speed = 0;
            static uint32_t dw_nmea_speed_backup = 0;

            float_nmea_speed = atof((const char*)ba_str);
            dw_nmea_speed = (uint32_t)(float_nmea_speed * 10 * 1.852);

            if((dw_nmea_speed_backup != dw_nmea_speed) &&\
               (strlen((const char*)ba_str) != 0))
            {
                dw_nmea_speed_backup = dw_nmea_speed;
                uint16_t w_data_len = 4;
                net_system_data_access(cb_device_data_access_data_type_status,
                                       cw_sys_status_data_addr_vehicle_speed,
                                       &w_data_len,
                                       (uint8_t *)&dw_nmea_speed,
                                       cb_device_data_access_operate_type_write);
                net_device_info *pst_net_device_info = 0;
                net_data_info *pst_data_info = 0;
                uint16_t w_vehicle_overspeed_value = 0;
                uint8_t f_overspeed = FALSE;
                uint8_t f_driving = FALSE;

                if(FALSE == net_get_device_info(cdw_device_id_system,
                                                (net_device_info **)&pst_net_device_info,
                                                &st_net_system_info_list))
                {
                }

                if(FALSE == net_get_device_data_info(cb_net_data_att_set,
                                                     cw_sys_set_data_addr_vehicle_overspeed,
                                                     (net_data_info ** const)&pst_data_info,
                                                     pst_net_device_info))
                {
                }
                if(pst_data_info && pst_data_info->pb_data)
                {
                    w_vehicle_overspeed_value = 10 * (*pst_data_info->pb_data);
                }

                if((0 != w_vehicle_overspeed_value) &&\
                   (dw_nmea_speed >= w_vehicle_overspeed_value))
                {
                    f_overspeed = TRUE;
                }
                if(dw_nmea_speed >= 10)
                {
                    f_driving = TRUE;
                }

                w_data_len = 1;
                net_system_data_access(cb_device_data_access_data_type_status,
                                       cw_sys_status_data_addr_vehicle_overspeed,
                                       &w_data_len,
                                       (uint8_t *)&f_overspeed,
                                       cb_device_data_access_operate_type_write);
                net_system_data_access(cb_device_data_access_data_type_status,
                                       cw_sys_status_data_addr_vehicle_driving,
                                       &w_data_len,
                                       (uint8_t *)&f_driving,
                                       cb_device_data_access_operate_type_write);
            }

//            //altitude
//            memset(ba_str,0,15);
//            pb_str = (uint8_t *)strstr((const char*)pb_nmea_info,"$GPGGA");
//            sGetStrFromSpecRangeWithChar(',',9,',',1,pb_str,ba_str,15);
//			#if(lte_nmea_info_test)
//            DebugPrintf(TRUE,"Altitude: %s\n",ba_str);
//            #endif
//            double float_nmea_altitude = 0;
//            uint32_t dw_nmea_altitude = 0;
//
//            float_nmea_altitude= atof((const char*)ba_str);
//            dw_nmea_altitude = (uint32_t)(float_nmea_altitude * 10);
//
//            if(strlen((const char*)ba_str) != 0)
//            {
//                uint16_t w_data_len = 4;
//                net_system_data_access(cb_device_data_access_data_type_status,
//                                       cw_sys_status_data_addr_altitude,
//                                       &w_data_len,
//                                       (uint8_t *)&dw_nmea_altitude,
//                                       cb_device_data_access_operate_type_write);
//            }

			//distance
			if((float_longitude_current != 0) && (float_latitude_current != 0))
			{
				if((float_longitude_current != float_longitude_backup) ||\
				   (float_latitude_current != float_latitude_backup))
				{

					if((0 == float_longitude_backup) ||\
					   (0 == float_latitude_backup))
					{
						float_longitude_backup = float_longitude_current;
						float_latitude_backup = float_latitude_current;
					}


					float_distance_temp = lte_module_nmea_lantitude_longitude_distance(float_longitude_current,
                                                                                       float_latitude_current,
                                                                                       float_longitude_backup,
                                                                                       float_latitude_backup);

					if((uint32_t)float_distance_temp >= 5)
					{
						dw_distance += (uint32_t)float_distance_temp;
//                        dw_total_distance += dw_distance;
                        dw_total_distance += (uint32_t)float_distance_temp;;
					}
					float_longitude_backup = float_longitude_current;
					float_latitude_backup = float_latitude_current;

                    {
                        uint16_t w_data_len = 4;
                        uint32_t dw_timestamp = 0;
                        if(FALSE == independ_user_data_store(cw_spif_user_data_opr_i37,
                                                             4,
                                                             (uint8_t *)&dw_total_distance,
                                                             &st_user_independ_data_list_info))
                        {
                        }
                        get_system_timestamp(&dw_timestamp);
                        if((dw_timestamp != 0) &&\
                           ((dw_timestamp/(3600*24)) != (dw_timestamp_backup/(3600*24))))
                        {
                            dw_timestamp_backup = dw_timestamp;
                            dw_distance = 0;
                            if(FALSE == independ_user_data_store(cw_spif_user_data_opr_i39,
                                                                 4,
                                                                 (uint8_t *)&dw_timestamp_backup,
                                                                 &st_user_independ_data_list_info))
                            {
                            }
                        }
                        if(FALSE == independ_user_data_store(cw_spif_user_data_opr_i38,
                                                             4,
                                                             (uint8_t *)&dw_distance,
                                                             &st_user_independ_data_list_info))
                        {
                        }
                        net_system_data_access(cb_device_data_access_data_type_status,
                                               cw_sys_status_data_addr_day_mileage,
                                               &w_data_len,
                                               (uint8_t *)&dw_distance,
                                               cb_device_data_access_operate_type_write);
                        net_system_data_access(cb_device_data_access_data_type_status,
                                               cw_sys_status_data_addr_total_mileage,
                                               &w_data_len,
                                               (uint8_t *)&dw_total_distance,
                                               cb_device_data_access_operate_type_write);
                    }
					#if(lte_nmea_info_test)
					DebugPrintf(TRUE,"Distance: %d\n",dw_distance);
                    DebugPrintf(TRUE,"Total distance: %d\n",dw_total_distance);
					#endif
				}

			}
			else
			{
				#if(lte_nmea_info_test)
				DebugPrintf(TRUE,"nmea_error\n");
				#endif
			}
//            //timestamp
//            uint32_t dw_time_temp = 0;
//            uint32_t dw_timestamp = 0;
//            utc_time st_utc_time = {0,};
//
//            memset(ba_str,0,15);
//            pb_str = (uint8_t *)strstr((const char*)pb_nmea_info,"$GPRMC");
//
//            sGetStrFromSpecRangeWithChar(',',1,',',1,pb_str,ba_str,6);
//			dw_time_temp = atoi((const char*)ba_str);
////            if(0 != dw_time_temp)
//            {
//                st_utc_time.tm_hour = dw_time_temp / 10000;
//                st_utc_time.tm_min = (dw_time_temp % 10000) / 100;
//                st_utc_time.tm_sec = (dw_time_temp % 100);
//            }
//
//            memset(ba_str,0,15);
//            sGetStrFromSpecRangeWithChar(',',9,',',1,pb_str,ba_str,6);
//            dw_time_temp = atoi((const char*)ba_str);
//            if(0 != dw_time_temp)
//            {
//                st_utc_time.tm_year = (dw_time_temp % 100) + 2000;
//                st_utc_time.tm_mon = (dw_time_temp % 10000) / 100;
//                st_utc_time.tm_mday = dw_time_temp / 10000;
//                utc_struct_time_to_unix_time(&st_utc_time,&dw_timestamp);
//            }
//
//
//            if(0 != dw_timestamp)
//            {
//                refresh_system_timestamp(dw_timestamp);
//            }

//            */


            b_step = cb_nmea_info_receive;
            #if(lte_nmea_info_test)
            DebugPrintf(TRUE,"usb_nmea_info_store_end: %d\n",xTaskGetTickCount());
            #endif
            break;
        }
    }
}
