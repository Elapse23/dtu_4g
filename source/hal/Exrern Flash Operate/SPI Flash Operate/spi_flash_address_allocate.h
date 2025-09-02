#ifndef _SPI_FLASH_ADDRESS_ALLOCATE_H_
#define _SPI_FLASH_ADDRESS_ALLOCATE_H_

//#define cdwLocalUpdateFirmAddrStart             0x00
//#define cdwLocalUpdateFirmAddrEnd               0x5000
//#define cdwLocalUpdateFirmInfoLen               0x1000
//#define cdwLocalUpdateFirmDataLen               300


//memory start
#define cdw_spi_flash_addr_alloc_start                            0x00
#define cdw_spi_flash_addr_alloc_end                              0x800000

//firmware info 0x0000 - 0x0fff					4k
#define cdw_spi_flash_addr_alloc_firmware_info_len                  \
    (4* 1024)
#define cdw_spi_flash_addr_alloc_firmware_info_start                cdw_spi_flash_addr_alloc_start
#define cdw_spi_flash_addr_alloc_firmware_info_end                  (cdw_spi_flash_addr_alloc_start + cdw_spi_flash_addr_alloc_firmware_info_len - 1)

//local firmware data 0x1000 - 0x96fff			600k
#define cdw_spi_flash_addr_alloc_local_firmware_data_len            \
    (600 * 1024)
#define cdw_spi_flash_addr_alloc_local_firmware_data_start          \
    (cdw_spi_flash_addr_alloc_firmware_info_end + 1)
#define cdw_spi_flash_addr_alloc_local_firmware_data_end            \
    (cdw_spi_flash_addr_alloc_local_firmware_data_start + cdw_spi_flash_addr_alloc_local_firmware_data_len - 1)

//offline data 10s 0x97000 - 0x496fff		4M
#define cdw_flash_addr_alloc_offline_data_len             \
    (4 * 1024 * 1024)
#define cdw_flash_addr_alloc_offline_data_start           \
    (cdw_spi_flash_addr_alloc_local_firmware_data_end + 1)
#define cdw_flash_addr_alloc_offline_data_end             \
    (cdw_flash_addr_alloc_offline_data_start + cdw_flash_addr_alloc_offline_data_len - 1)

////data backup 0x497000 - 0x596fff			1M
//#define cdw_flash_addr_alloc_data_backup_len             \
//    (1 * 1024 * 1024)
//#define cdw_flash_addr_alloc_data_backup_start           \
//    (cdw_flash_addr_alloc_offline_data_end + 1)
//#define cdw_flash_addr_alloc_data_backup_end             \
//    (cdw_flash_addr_alloc_data_backup_start + cdw_flash_addr_alloc_data_backup_len - 1)

//ble local firmware data 0x497000 - 0x52Cfff			600k
#define cdw_ble_spi_flash_addr_alloc_local_firmware_data_len            \
    (600 * 1024)
#define cdw_ble_spi_flash_addr_alloc_local_firmware_data_start          \
    (cdw_flash_addr_alloc_offline_data_end + 1)
#define cdw_ble_spi_flash_addr_alloc_local_firmware_data_end            \
    (cdw_ble_spi_flash_addr_alloc_local_firmware_data_start + cdw_ble_spi_flash_addr_alloc_local_firmware_data_len - 1)

//countdown time rollover data 0x52D000 -  0x53CFFF
#define cdw_flash_addr_alloc_countdown_time_rollover_data_len			\
	(64 * 1024)
#define cdw_flash_addr_alloc_countdown_time_rollover_data_start			\
	(cdw_ble_spi_flash_addr_alloc_local_firmware_data_end + 1)
#define cdw_flash_addr_alloc_countdown_time_rollover_data_end			\
	(cdw_flash_addr_alloc_countdown_time_rollover_data_start + cdw_flash_addr_alloc_countdown_time_rollover_data_len - 1)

//flash addr reverse 0x53CFFF - 0x596FFF 
#define cdw_flash_addr_alloc_data_backup_len             \
    (360 * 1024)
#define cdw_flash_addr_alloc_data_backup_start           \
    (cdw_flash_addr_alloc_countdown_time_rollover_data_end + 1)
#define cdw_flash_addr_alloc_data_backup_end             \
    (cdw_flash_addr_alloc_data_backup_start + cdw_flash_addr_alloc_data_backup_len - 1)



//event log 0x597000 - 0x696fff				1M 
#define cdw_flash_addr_alloc_event_log_len             \
    (1 * 1024 * 1024)
#define cdw_flash_addr_alloc_event_log_start           \
    (cdw_flash_addr_alloc_data_backup_end + 1)
#define cdw_flash_addr_alloc_event_log_end             \
    (cdw_flash_addr_alloc_event_log_start + cdw_flash_addr_alloc_event_log_len - 1)

#define  cw_local_firmware_info_version                         0x1000


#endif
