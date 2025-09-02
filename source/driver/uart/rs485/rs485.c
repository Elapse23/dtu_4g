#include "rs485.h"
#include "application_general_macro.h"
#include "n32g45x.h"
#include <string.h>


uint32_t dw_usart_baudrate_rs485 = 9600;
uint32_t dw_rs485_receive_timeout = 15;
uint32_t dw_rs485_trans_task_delay = 100;
#ifdef KINERGY_II_WIFI
#define usart_peripheral_rs485                       UART5
#define usart_peripheral_rs485_Tx_GPIO               GPIOC
#define usart_peripheral_rs485_Rx_GPIO               GPIOD
#define usart_peripheral_rs485_Dir_GPIO              GPIOB
#define usart_peripheral_rs485_CLK                   RCC_APB1_PERIPH_UART5
#define usart_peripheral_rs485_Tx_GPIO_CLK           RCC_APB2_PERIPH_GPIOC
#define usart_peripheral_rs485_Rx_GPIO_CLK           RCC_APB2_PERIPH_GPIOD
#define usart_peripheral_rs485_Dir_GPIO_CLK          RCC_APB2_PERIPH_GPIOB
#define usart_peripheral_rs485_RxPin                 GPIO_PIN_2
#define usart_peripheral_rs485_TxPin                 GPIO_PIN_12
#define usart_peripheral_rs485_DirPin                GPIO_PIN_3
#define usart_peripheral_rs485_APBxClkCmd            RCC_EnableAPB1PeriphClk
#define usart_peripheral_rs485_IRQn                  UART5_IRQn
#define usart_peripheral_rs485_IRQHandler            UART5_IRQHandler
#else
#define usart_peripheral_rs485                       UART5
#define usart_peripheral_rs485_Tx_GPIO               GPIOC
#define usart_peripheral_rs485_Rx_GPIO               GPIOD
#define usart_peripheral_rs485_Dir_GPIO              GPIOC
#define usart_peripheral_rs485_CLK                   RCC_APB1_PERIPH_UART5
#define usart_peripheral_rs485_Tx_GPIO_CLK           RCC_APB2_PERIPH_GPIOC
#define usart_peripheral_rs485_Rx_GPIO_CLK           RCC_APB2_PERIPH_GPIOD
#define usart_peripheral_rs485_Dir_GPIO_CLK          RCC_APB2_PERIPH_GPIOC
#define usart_peripheral_rs485_RxPin                 GPIO_PIN_2
#define usart_peripheral_rs485_TxPin                 GPIO_PIN_12
#define usart_peripheral_rs485_DirPin                GPIO_PIN_3
#define usart_peripheral_rs485_APBxClkCmd            RCC_EnableAPB1PeriphClk
#define usart_peripheral_rs485_IRQn                  UART5_IRQn
#define usart_peripheral_rs485_IRQHandler            UART5_IRQHandler

#endif


#define rs485_send_direction_ctrl()						GPIO_ConfigPinRemap(GPIO_RMP_SW_JTAG_SW_ENABLE,ENABLE);\
                                                        GPIO_SetBits(usart_peripheral_rs485_Dir_GPIO,usart_peripheral_rs485_DirPin)
#define rs485_receive_direction_ctrl()					GPIO_ConfigPinRemap(GPIO_RMP_SW_JTAG_SW_ENABLE,ENABLE);\
                                                        GPIO_ResetBits(usart_peripheral_rs485_Dir_GPIO,usart_peripheral_rs485_DirPin)


#if( RS485_USE_QUEUE)

//////////// RX QUEUE ////////////////
QueueHandle_t xrs485_data_rx_queue = 0;
#define cb_rs485_rx_queue_max_len           1000

#else

uint8_t ba_rs485_rx_buff[rs485_mps_size] = {0,};
uint16_t w_rs485_rx_data_len = 0;

#endif


uint16_t w_usart_rs485_transmit_data_index = 0;
uint16_t w_usart_rs485_transmit_data_len = 0;
uint8_t *pb_usart_rs485_transmit_data = 0;

void disable_rs485_tx_it()
{
    w_usart_rs485_transmit_data_len = 0;
    pb_usart_rs485_transmit_data = 0;
    w_usart_rs485_transmit_data_index = 0;
    USART_ConfigInt(usart_peripheral_rs485,USART_INT_TXC,DISABLE);
//    USART_ConfigInt(usart_peripheral_rs485,USART_INT_TXDE,DISABLE);
//    usart_interrupt_enable(usart_peripheral_rs485,USART_INT_RBNE);
}
void enable_rs485_tx_it(uint16_t w_data_len,uint8_t *b_data_buf)
{
    if(NULL == b_data_buf)
    {
        return;
    }
    w_usart_rs485_transmit_data_len = w_data_len;
    {
        pb_usart_rs485_transmit_data = b_data_buf;
//        USART_ConfigInt(usart_peripheral_rs485,USART_INT_TXDE,ENABLE);
        USART_ConfigInt(usart_peripheral_rs485,USART_INT_TXC,ENABLE);
    }
}
uint16_t rs485_tx_index_get()
{
    return w_usart_rs485_transmit_data_index;
}

void rs485_config()
{
    RCC_EnableAPB2PeriphClk(usart_peripheral_rs485_Tx_GPIO_CLK   |\
                            usart_peripheral_rs485_Rx_GPIO_CLK   |\
                            usart_peripheral_rs485_Dir_GPIO_CLK  |\
                            RCC_APB2_PERIPH_AFIO, ENABLE);
//    RCC_EnableAPB2PeriphClk(usart_peripheral_rs485_Tx_GPIO_CLK,ENABLE);
//    RCC_EnableAPB2PeriphClk(usart_peripheral_rs485_Rx_GPIO_CLK,ENABLE);
//    RCC_EnableAPB2PeriphClk(usart_peripheral_rs485_Dir_GPIO_CLK,ENABLE);
    usart_peripheral_rs485_APBxClkCmd(usart_peripheral_rs485_CLK, ENABLE);

    USART_DeInit(usart_peripheral_rs485);
    {
        GPIO_InitType GPIO_InitStructure;

        GPIO_InitStructure.Pin       = usart_peripheral_rs485_RxPin;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_InitPeripheral(usart_peripheral_rs485_Rx_GPIO, &GPIO_InitStructure);

        GPIO_InitStructure.Pin        = usart_peripheral_rs485_TxPin;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_InitPeripheral(usart_peripheral_rs485_Tx_GPIO, &GPIO_InitStructure);

        GPIO_InitStructure.Pin        = usart_peripheral_rs485_DirPin;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
        GPIO_InitPeripheral(usart_peripheral_rs485_Dir_GPIO, &GPIO_InitStructure);
    }
    {
        NVIC_InitType NVIC_InitStructure;

//        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

        NVIC_InitStructure.NVIC_IRQChannel                      = usart_peripheral_rs485_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 5;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
    {
        USART_InitType USART_InitStructure;

        USART_InitStructure.BaudRate            = dw_usart_baudrate_rs485;
        USART_InitStructure.WordLength          = USART_WL_8B;
        USART_InitStructure.StopBits            = USART_STPB_1;
        USART_InitStructure.Parity              = USART_PE_NO;
        USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
        USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;

        USART_Init(usart_peripheral_rs485, &USART_InitStructure);

        USART_ConfigInt(usart_peripheral_rs485, USART_INT_RXDNE, ENABLE);
//        USART_ConfigInt(usart_peripheral_rs485, USART_INT_TXC, ENABLE);

        rs485_receive_direction_ctrl();
//        USART_ClrFlag(usart_peripheral_rs485,USART_INT_TXDE);
//        USART_ClrFlag(usart_peripheral_rs485,USART_INT_RXDNE);
        USART_Enable(usart_peripheral_rs485, ENABLE);
    }


#if( RS485_USE_QUEUE)
    if(NULL == xrs485_data_rx_queue)
    {
        xrs485_data_rx_queue = xQueueCreate(cb_rs485_rx_queue_max_len,sizeof(uint8_t));
    }
    
    if(NULL == xrs485_data_rx_queue)
    {
        //Create Error
    }
#endif

}

uint8_t rs485_send()
{
    if((NULL == pb_usart_rs485_transmit_data) ||\
       (0 == w_usart_rs485_transmit_data_len))
    {
        return FALSE;
    }
//    if(RESET == USART_GetIntStatus(usart_peripheral_rs485,USART_INT_TXDE))
//    {
//        return FALSE;
//    }
    if(RESET == USART_GetIntStatus(usart_peripheral_rs485,USART_INT_TXC))
    {
        return FALSE;
    }

	rs485_send_direction_ctrl();
//    USART_ClrFlag(usart_peripheral_rs485,USART_INT_TXDE);
    USART_ClrFlag(usart_peripheral_rs485,USART_INT_TXC);

    if(w_usart_rs485_transmit_data_index >= w_usart_rs485_transmit_data_len)
    {
        disable_rs485_tx_it();
        rs485_receive_direction_ctrl();
    }
	else
    {
    	USART_SendData(usart_peripheral_rs485,pb_usart_rs485_transmit_data[w_usart_rs485_transmit_data_index++]);
    }
    return TRUE;
}

void rs485_receive_isr() 
{
    if(RESET == USART_GetIntStatus(usart_peripheral_rs485,USART_INT_RXDNE))
    {
        return;
    }

	rs485_receive_direction_ctrl();

#if( RS485_USE_QUEUE)
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t b_data_temp = 0;
    
    {
		USART_ClrIntPendingBit(usart_peripheral_rs485,USART_INT_RXDNE);
        b_data_temp = (uint8_t)usart_data_receive(usart_peripheral_rs485);
        
        if(NULL != xrs485_data_rx_queue)
        {
            xQueueSendFromISR(xrs485_data_rx_queue,
                              (void *)&b_data_temp,
                              &xHigherPriorityTaskWoken);
        }
    }

	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
#else
	if(w_rs485_rx_data_len < rs485_mps_size)
	{
		USART_ClrIntPendingBit(usart_peripheral_rs485,USART_INT_RXDNE);
		ba_rs485_rx_buff[w_rs485_rx_data_len++] = (uint8_t)USART_ReceiveData(usart_peripheral_rs485);
	}
	else
	{
		w_rs485_rx_data_len = 0;
	}
#endif
}

void rs485_buadrate_set(uint32_t dw_buad_rate)
{
    if(dw_buad_rate !=  dw_usart_baudrate_rs485)
    {
        if(cdw_uart_buadrate_9600 == dw_buad_rate)
        {
            dw_usart_baudrate_rs485 = dw_buad_rate;
            rs485_config();
            dw_rs485_receive_timeout = 15;
            dw_rs485_trans_task_delay = 100;
        }
        else if(cdw_uart_buadrate_115200 == dw_buad_rate)
        {
            dw_usart_baudrate_rs485 = dw_buad_rate;
            rs485_config();
            dw_rs485_receive_timeout = 3;
            dw_rs485_trans_task_delay = 5;
        }
    }
}



