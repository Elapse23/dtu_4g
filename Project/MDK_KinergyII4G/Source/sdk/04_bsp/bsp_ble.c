#include "bsp_ble.h"



void bsp_ble_spi_init(void)
{
	spi_parameter_struct spi_init_struct;
	
	rcu_periph_clock_enable(BLE_SPI_SCK_GPIO_CLK);
	rcu_periph_clock_enable(BLE_SPI_MISO_GPIO_CLK);
	rcu_periph_clock_enable(BLE_SPI_MOSI_GPIO_CLK);
	rcu_periph_clock_enable(BLE_SPI_CLK);

	gpio_af_set(BLE_SPI_SCK_GPIO_PORT, GPIO_AF_5, BLE_SPI_SCK_PIN | BLE_SPI_MISO_PIN | BLE_SPI_MOSI_PIN );
    gpio_mode_set(BLE_SPI_SCK_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, BLE_SPI_SCK_PIN | BLE_SPI_MISO_PIN | BLE_SPI_MOSI_PIN );
    gpio_output_options_set(BLE_SPI_SCK_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, BLE_SPI_SCK_PIN | BLE_SPI_MISO_PIN | BLE_SPI_MOSI_PIN );	

	/* SPI parameter config */
	spi_init_struct.trans_mode			 = SPI_TRANSMODE_FULLDUPLEX;
	spi_init_struct.device_mode 		 = SPI_MASTER;
	spi_init_struct.frame_size			 = SPI_FRAMESIZE_8BIT;
	spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_2EDGE;
	spi_init_struct.nss 				 = SPI_NSS_SOFT;
	spi_init_struct.prescale			 = SPI_PSC_64 ;//CLK 3M+（最大支持4M）
	spi_init_struct.endian				 = SPI_ENDIAN_MSB;
	spi_init(BLE_SPIn, &spi_init_struct);

	/* set crc polynomial */
	spi_crc_polynomial_set(BLE_SPIn,7);
	
	/* enable SPI */
	spi_enable(BLE_SPIn);

}



void ble_gpio_init(void)
{
	//INT
    /* enable the key clock */
    rcu_periph_clock_enable(BLE_INT_GPIO_CLK);
    rcu_periph_clock_enable(RCU_SYSCFG);

    /* configure button pin as input */
	gpio_mode_set(BLE_INT_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, BLE_INT_PIN);

	//RST
    /* enable the clock */
    rcu_periph_clock_enable(BLE_RST_GPIO_CLK);
    gpio_mode_set(BLE_RST_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, BLE_RST_PIN);
	/* configure GPIO port */ 
	gpio_output_options_set(BLE_RST_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BLE_RST_PIN);
	

	//BLE Power
    /* enable the clock */
    rcu_periph_clock_enable(BLE_PWR_GPIO_CLK);
    gpio_mode_set(BLE_PWR_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, BLE_PWR_PIN);
	/* configure GPIO port */ 
	gpio_output_options_set(BLE_PWR_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BLE_PWR_PIN);	
	
	BLE_PWR_HIGH();
}



void bsp_ble_init()
{
	bsp_ble_spi_init();
	ble_gpio_init();
}



uint8_t bsp_ble_int_status_get()
{
	return gpio_input_bit_get(BLE_INT_GPIO_PORT, BLE_INT_PIN);
}



uint8_t bsp_ble_write_byte(uint8_t byte)
{
    /* loop while data register in not emplty */
    while (RESET == spi_i2s_flag_get(BLE_SPIn,SPI_FLAG_TBE));

    /* send byte through the SPI0 peripheral */
    spi_i2s_data_transmit(BLE_SPIn,byte);

    /* wait to receive a byte */
    while(RESET == spi_i2s_flag_get(BLE_SPIn,SPI_FLAG_RBNE));

    /* return the byte read from the SPI bus */
    return(spi_i2s_data_receive(BLE_SPIn));
}


uint8_t bsp_ble_read_byte(void)
{
    return(bsp_ble_write_byte(0xFF));
}

