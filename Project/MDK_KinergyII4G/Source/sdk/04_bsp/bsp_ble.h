#ifndef __BSP_BLE_H__
#define __BSP_BLE_H__



#include "stdint.h"
#include "gd32f4xx.h"




#define	BLE_SPIn							SPI3
#define BLE_SPI_CLK							RCU_SPI3


#define BLE_SPI_SCK_PIN                		GPIO_PIN_2
#define BLE_SPI_SCK_GPIO_PORT         		GPIOE
#define BLE_SPI_SCK_GPIO_CLK    			RCU_GPIOE
  
#define BLE_SPI_MISO_PIN              		GPIO_PIN_5
#define BLE_SPI_MISO_GPIO_PORT      		GPIOE
#define BLE_SPI_MISO_GPIO_CLK        		RCU_GPIOE
  
#define BLE_SPI_MOSI_PIN           			GPIO_PIN_6
#define BLE_SPI_MOSI_GPIO_PORT      		GPIOE
#define BLE_SPI_MOSI_GPIO_CLK        		RCU_GPIOE
  

//PD13
#define BLE_PWR_PIN           				GPIO_PIN_13
#define BLE_PWR_GPIO_PORT      				GPIOD
#define BLE_PWR_GPIO_CLK        			RCU_GPIOD





#define BLE_PWR_HIGH()       				gpio_bit_set(BLE_PWR_GPIO_PORT, BLE_PWR_PIN)


#define BLE_INT_PIN              			GPIO_PIN_3
#define BLE_INT_GPIO_PORT					GPIOG
#define BLE_INT_GPIO_CLK                    RCU_GPIOG


#define BLE_RST_PIN              			GPIO_PIN_3
#define BLE_RST_GPIO_PORT					GPIOE
#define BLE_RST_GPIO_CLK                    RCU_GPIOE


#define BLE_SPI_RST_LOW()        			gpio_bit_reset(BLE_RST_GPIO_PORT, BLE_RST_PIN)
#define BLE_SPI_RST_HIGH()       			gpio_bit_set(BLE_RST_GPIO_PORT, BLE_RST_PIN)

	

extern void bsp_ble_init(void);
extern uint8_t bsp_ble_int_status_get(void);

extern uint8_t bsp_ble_write_byte(uint8_t byte);
extern uint8_t bsp_ble_read_byte(void);


#endif

