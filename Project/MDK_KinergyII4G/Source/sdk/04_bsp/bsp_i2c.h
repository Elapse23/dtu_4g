#ifndef __BSP_I2C_H__
#define __BSP_I2C_H__
#include "stdint.h"






#define IIC_SDA_HIGH()					gpio_bit_set(I2C_SDA_GPIO_PORT, I2C_SDA_PIN)
#define IIC_SDA_LOW()					gpio_bit_reset(I2C_SDA_GPIO_PORT, I2C_SDA_PIN)


#define IIC_SCL_HIGH()					gpio_bit_set(I2C_SCL_GPIO_PORT, I2C_SCL_PIN)
#define IIC_SCL_LOW()					gpio_bit_reset(I2C_SCL_GPIO_PORT, I2C_SCL_PIN)


#define READ_SDA						gpio_input_bit_get(I2C_SDA_GPIO_PORT, I2C_SDA_PIN)



extern void IIC_Start(void);
extern void IIC_Stop(void);
extern void IIC_Send_Byte(uint8_t txd);
extern uint8_t IIC_Read_Byte(unsigned char ack);
extern uint8_t IIC_Wait_Ack(void);



#endif
