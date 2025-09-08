/**
 * =====================================================================================
 * @file        bsp_serial_driver.c
 * @brief       通用串口驱动实现 (Refactored)
 * @author      23Elapse & Gemini
 * @version     2.1 (Refactored)
 * @date        2025-06-14
 * =====================================================================================
 */
#include "n32g45x_rcc.h"            
#include "n32g45x_gpio.h"            
#include "n32g45x_usart.h"
#include "stdio.h"   
#include "bsp_serial_driver.h"        



Serial_Status_t Serial_Driver_Init(USART_Module* com, uint32_t baudrate) {

	if(LOG_UART == com)
	{
        // 初始化LOG串口时钟和GPIO时钟
        RCC_EnableAPB2PeriphClk(LOG_GPIO_CLK | RCC_APB2_PERIPH_AFIO, ENABLE);
        LOG_CLK_CMD(LOG_UART_CLK, ENABLE);
        USART_DeInit(LOG_UART);
        {
            GPIO_InitType GPIO_InitStructure;
            GPIO_InitStructure.Pin = LOG_RX_PIN;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
            GPIO_InitPeripheral(LOG_GPIO, &GPIO_InitStructure);
            GPIO_InitStructure.Pin = LOG_TX_PIN;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
            GPIO_InitPeripheral(LOG_GPIO, &GPIO_InitStructure);
        }
        {
            NVIC_InitType NVIC_InitStructure;
            NVIC_InitStructure.NVIC_IRQChannel = LOG_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6; // 比RS485优先级低
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStructure);
        }
        {
            USART_InitType USART_InitStructure;
            USART_InitStructure.BaudRate = baudrate;
            USART_InitStructure.WordLength = USART_WL_8B;
            USART_InitStructure.StopBits = USART_STPB_1;
            USART_InitStructure.Parity = USART_PE_NO;
            USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
            USART_InitStructure.Mode = USART_MODE_RX | USART_MODE_TX;
            USART_Init(LOG_UART, &USART_InitStructure);
            USART_ConfigInt(LOG_UART, USART_INT_RXDNE, ENABLE);
            USART_Enable(LOG_UART, ENABLE);
        }
    }
	else if(RS485_UART == com)
	{
        RCC_EnableAPB2PeriphClk(RS485_TX_GPIO_CLK | RS485_RX_GPIO_CLK | RS485_DIR_GPIO_CLK | RCC_APB2_PERIPH_AFIO, ENABLE);
        RS485_CLK_CMD(RS485_UART_CLK, ENABLE);
        USART_DeInit(RS485_UART);
        {
            GPIO_InitType GPIO_InitStructure;
            GPIO_InitStructure.Pin       = RS485_RX_PIN;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
            GPIO_InitPeripheral(RS485_RX_GPIO, &GPIO_InitStructure);
            GPIO_InitStructure.Pin        = RS485_TX_PIN;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
            GPIO_InitPeripheral(RS485_TX_GPIO, &GPIO_InitStructure);
            GPIO_InitStructure.Pin        = RS485_DIR_PIN;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
            GPIO_InitPeripheral(RS485_DIR_GPIO, &GPIO_InitStructure);
        }
        {
            NVIC_InitType NVIC_InitStructure;
            NVIC_InitStructure.NVIC_IRQChannel                      = RS485_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 5;
            NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 0;
            NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
            NVIC_Init(&NVIC_InitStructure);
        }
        {
            USART_InitType USART_InitStructure;
            USART_InitStructure.BaudRate            = baudrate;
            USART_InitStructure.WordLength          = USART_WL_8B;
            USART_InitStructure.StopBits            = USART_STPB_1;
            USART_InitStructure.Parity              = USART_PE_NO;
            USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
            USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;
            USART_Init(RS485_UART, &USART_InitStructure);
            USART_ConfigInt(RS485_UART, USART_INT_RXDNE, ENABLE);
            rs485_receive_direction_ctrl();
            USART_Enable(RS485_UART, ENABLE);
        }
    }
    else if(LTE_UART == com)
    {
        RCC_EnableAPB2PeriphClk(LTE_GPIO_CLK | RCC_APB2_PERIPH_AFIO, ENABLE);
        LTE_CLK_CMD(LTE_UART_CLK, ENABLE);
        USART_DeInit(LTE_UART);
        {
            GPIO_InitType GPIO_InitStructure;
            GPIO_InitStructure.Pin       = LTE_RX_PIN;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
            GPIO_InitPeripheral(LTE_GPIO, &GPIO_InitStructure);
            GPIO_InitStructure.Pin        = LTE_TX_PIN;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
            GPIO_InitPeripheral(LTE_GPIO, &GPIO_InitStructure);
        }
        {
            NVIC_InitType NVIC_InitStructure;
            NVIC_InitStructure.NVIC_IRQChannel                      = LTE_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 5;
            NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 0;
            NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
            NVIC_Init(&NVIC_InitStructure);
        }
        {
            USART_InitType USART_InitStructure;
            USART_InitStructure.BaudRate            = baudrate;	//dw_usart_baudrate_lte;
            USART_InitStructure.WordLength          = USART_WL_8B;
            USART_InitStructure.StopBits            = USART_STPB_1;
            USART_InitStructure.Parity              = USART_PE_NO;
            USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
            USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;
            USART_Init(LTE_UART, &USART_InitStructure);
            USART_ConfigInt(LTE_UART, USART_INT_RXDNE, ENABLE);
            USART_ConfigInt(LTE_UART, USART_INT_IDLEF, ENABLE);
            USART_Enable(LTE_UART, ENABLE);
        }
    }
    else
    {
        return SERIAL_ERR_PARAM;
    }
    return SERIAL_OK;
}


Serial_Status_t Serial_Driver_Deinit(USART_Module* com) {

    if (RS485_UART == com)
    {
        // 1) 关闭NVIC与USART中断
        NVIC_DisableIRQ(RS485_IRQn);
        USART_ConfigInt(RS485_UART, USART_INT_RXDNE, DISABLE);

        // 2) 关闭USART并复位
        USART_Enable(RS485_UART, DISABLE);
        USART_DeInit(RS485_UART);

        // 3) 复位GPIO到默认状态（输入/模拟），释放总线
        GPIO_DeInit(RS485_RX_GPIO);
        GPIO_DeInit(RS485_TX_GPIO);
        GPIO_DeInit(RS485_DIR_GPIO);

        // 4) 关闭时钟（与初始化对称）
        RS485_CLK_CMD(RS485_UART_CLK, DISABLE);
        RCC_EnableAPB2PeriphClk(RS485_TX_GPIO_CLK | RS485_RX_GPIO_CLK | RS485_DIR_GPIO_CLK | RCC_APB2_PERIPH_AFIO, DISABLE);
    }
    else if (LTE_UART == com)
    {
        // 1) 关闭NVIC与USART中断
        NVIC_DisableIRQ(LTE_IRQn);
        USART_ConfigInt(LTE_UART, USART_INT_RXDNE, DISABLE);
        USART_ConfigInt(LTE_UART, USART_INT_IDLEF, DISABLE);

        // 2) 关闭USART并复位
        USART_Enable(LTE_UART, DISABLE);
        USART_DeInit(LTE_UART);

        // 3) 复位GPIO
        GPIO_DeInit(LTE_GPIO);

        // 4) 关闭时钟
        LTE_CLK_CMD(LTE_UART_CLK, DISABLE);
        RCC_EnableAPB2PeriphClk(LTE_GPIO_CLK | RCC_APB2_PERIPH_AFIO, DISABLE);
    }
    else if (LOG_UART == com)
    {
        // LOG口未开启NVIC，这里仅关闭USART
        USART_Enable(LOG_UART, DISABLE);
        USART_DeInit(LOG_UART);

        // 复位GPIO
        GPIO_DeInit(LOG_GPIO);

        // 关闭时钟
        LOG_CLK_CMD(LOG_UART_CLK, DISABLE);
        RCC_EnableAPB2PeriphClk(LOG_GPIO_CLK | RCC_APB2_PERIPH_AFIO, DISABLE);
    }
    else
    {
        return SERIAL_ERR_PARAM;
    }
    return SERIAL_OK;
}



/**
 * @brief 将调试端口重映射为仅 SWD（禁用 JTAG，释放相关引脚）
 * @note 一次性调用：需开启 AFIO 时钟后调用一次即可
 */
void bsp_debug_port_remap_swd_only(void)
{
    /* N32 标准外设：开启 AFIO 时钟 */
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO, ENABLE);
    /* 仅启用 SWD，禁用 JTAG，释放相关引脚供普通 GPIO 使用 */
    GPIO_ConfigPinRemap(GPIO_RMP_SW_JTAG_SW_ENABLE, ENABLE);
}
