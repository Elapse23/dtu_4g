#include "usart_config_lte.h"
#include "n32g45x.h"
#include "application_general_macro.h"
#include <string.h>
#include "freertos_user_config.h"


//usart
static uint16_t w_usart_lte_transmit_data_index = 0;
static uint16_t w_usart_lte_transmit_data_len = 0;
static uint8_t *pb_usart_lte_transmit_data = 0;
// static uint32_t dw_usart_baudrate_lte = 115200;
static uint32_t dw_usart_baudrate_lte = 921600;
static uint16_t w_usart_lte_receive_data_index = 0;
uint8_t b_usart_lte_rx_buff[cb_lte_mps_size] = {0,};
uint16_t w_usart_lte_rx_len = 0;
static uint8_t sb_receive_state = receive_none;
static uint8_t b_send_state = send_none;
//io

#ifdef KINERGY_II_WIFI
#define usart_peripheral_lte                       USART1
#define usart_peripheral_lte_GPIO                  GPIOA
#define usart_peripheral_lte_CLK                   RCC_APB2_PERIPH_USART1
#define usart_peripheral_lte_GPIO_CLK              RCC_APB2_PERIPH_GPIOA
#define usart_peripheral_lte_RxPin                 GPIO_PIN_10
#define usart_peripheral_lte_TxPin                 GPIO_PIN_9
#define usart_peripheral_lte_APBxClkCmd            RCC_EnableAPB2PeriphClk
#define usart_peripheral_lte_IRQn                  USART1_IRQn
#define usart_peripheral_lte_IRQHandler            USART1_IRQHandler

#else
#define usart_peripheral_lte                       USART3
#define usart_peripheral_lte_GPIO                  GPIOB
#define usart_peripheral_lte_CLK                   RCC_APB1_PERIPH_USART3
#define usart_peripheral_lte_GPIO_CLK              RCC_APB2_PERIPH_GPIOB
#define usart_peripheral_lte_RxPin                 GPIO_PIN_11
#define usart_peripheral_lte_TxPin                 GPIO_PIN_10
#define usart_peripheral_lte_APBxClkCmd            RCC_EnableAPB1PeriphClk
#define usart_peripheral_lte_IRQn                  USART3_IRQn
#define usart_peripheral_lte_IRQHandler            USART3_IRQHandler
#endif

void usart_lte_config()
{
    RCC_EnableAPB2PeriphClk(usart_peripheral_lte_GPIO_CLK   |\
                            RCC_APB2_PERIPH_AFIO, ENABLE);
    usart_peripheral_lte_APBxClkCmd(usart_peripheral_lte_CLK, ENABLE);

    USART_DeInit(usart_peripheral_lte);
    {
        GPIO_InitType GPIO_InitStructure;

        GPIO_InitStructure.Pin       = usart_peripheral_lte_RxPin;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_InitPeripheral(usart_peripheral_lte_GPIO, &GPIO_InitStructure);

        GPIO_InitStructure.Pin        = usart_peripheral_lte_TxPin;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_InitPeripheral(usart_peripheral_lte_GPIO, &GPIO_InitStructure);
    }
    {
        NVIC_InitType NVIC_InitStructure;

//        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

        NVIC_InitStructure.NVIC_IRQChannel                      = usart_peripheral_lte_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 5;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
    {
        USART_InitType USART_InitStructure;

        USART_InitStructure.BaudRate            = 115200;	//dw_usart_baudrate_lte;
        USART_InitStructure.WordLength          = USART_WL_8B;
        USART_InitStructure.StopBits            = USART_STPB_1;
        USART_InitStructure.Parity              = USART_PE_NO;
        USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
        USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;

        USART_Init(usart_peripheral_lte, &USART_InitStructure);

        USART_ConfigInt(usart_peripheral_lte, USART_INT_RXDNE, ENABLE);
        USART_ConfigInt(usart_peripheral_lte, USART_INT_IDLEF, ENABLE);
//        USART_ConfigInt(usart_peripheral_lte, USART_INT_TXDE,ENABLE);
//        USART_ConfigInt(usart_peripheral_lte, USART_INT_TXC, ENABLE);

        USART_Enable(usart_peripheral_lte, ENABLE);
    }
}


void set_usart_lte_baudrate_921600()
{
    dw_usart_baudrate_lte = 921600;
    USART_InitType USART_InitStructure;

    USART_InitStructure.BaudRate            = dw_usart_baudrate_lte;
    USART_InitStructure.WordLength          = USART_WL_8B;
    USART_InitStructure.StopBits            = USART_STPB_1;
    USART_InitStructure.Parity              = USART_PE_NO;
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;

    USART_Init(usart_peripheral_lte, &USART_InitStructure);

    USART_ConfigInt(usart_peripheral_lte, USART_INT_RXDNE, ENABLE);
    USART_ConfigInt(usart_peripheral_lte, USART_INT_IDLEF, ENABLE);
    USART_Enable(usart_peripheral_lte, ENABLE);
}

void set_usart_lte_baudrate_115200()
{
    dw_usart_baudrate_lte = 115200;
    USART_InitType USART_InitStructure;

    USART_InitStructure.BaudRate            = dw_usart_baudrate_lte;
    USART_InitStructure.WordLength          = USART_WL_8B;
    USART_InitStructure.StopBits            = USART_STPB_1;
    USART_InitStructure.Parity              = USART_PE_NO;
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;

    USART_Init(usart_peripheral_lte, &USART_InitStructure);

    USART_ConfigInt(usart_peripheral_lte, USART_INT_RXDNE, ENABLE);
    USART_ConfigInt(usart_peripheral_lte, USART_INT_IDLEF, ENABLE);
    USART_Enable(usart_peripheral_lte, ENABLE);
}


//void usart_lte_transmit_trig(uint16_t w_data_in_len,uint8_t *pb_data_in)
//{
//    if(NULL == pb_data_in)
//    {
//        return;
//    }
//
////    w_usart_lte_transmit_data_index = 0;
////    w_usart_lte_transmit_data_len = w_data_in_len;
////    pb_usart_lte_transmit_data = pb_data_in;
////    USART_SendData(usart_peripheral_lte,pb_usart_lte_transmit_data[w_usart_lte_transmit_data_index]);
//
//    for(uint16_t w_cnt_i = 0;w_cnt_i < w_data_in_len;w_cnt_i++)
//    {
//        USART_SendData(usart_peripheral_lte,pb_data_in[w_cnt_i]);
//
//        while(RESET == USART_GetFlagStatus(usart_peripheral_lte,USART_INT_TXDE))
//        {
//            ;
//        }
//
//        while(RESET == USART_GetFlagStatus(usart_peripheral_lte,USART_INT_TXC))
//        {
//            ;
//        }
//    }
//}

void usart_lte_transmit_trig(uint16_t w_data_in_len,uint8_t *pb_data_in)
{
    if(NULL == pb_data_in)
    {
        return;
    }
    b_send_state = send_none;
    w_usart_lte_transmit_data_len = w_data_in_len;
    {
        pb_usart_lte_transmit_data = pb_data_in;
        USART_ConfigInt(usart_peripheral_lte,USART_INT_TXC,ENABLE);
    }
    while(b_send_state != send_done)
    {
        vTaskDelay(0);
    };
}

void usart_lte_transmit_disable()
{
    w_usart_lte_transmit_data_len = 0;
    pb_usart_lte_transmit_data = 0;
    w_usart_lte_transmit_data_index = 0;
    USART_ConfigInt(usart_peripheral_lte,USART_INT_TXC,DISABLE);
    b_send_state = send_done;
}

void usart_lte_transmit_isr()
{
    if(RESET == USART_GetIntStatus(usart_peripheral_lte,USART_INT_TXC))
    {
        return;
    }
    b_send_state = sending;

    if(w_usart_lte_transmit_data_index < w_usart_lte_transmit_data_len)
    {
        USART_ClrIntPendingBit(usart_peripheral_lte,USART_INT_TXC);
    //    USART_ClrIntPendingBit(usart_peripheral_lte,USART_INT_TXDE);
        USART_SendData(usart_peripheral_lte,pb_usart_lte_transmit_data[w_usart_lte_transmit_data_index++]);
    }
    else
    {
        w_usart_lte_transmit_data_index = 0;
        w_usart_lte_transmit_data_len = 0;
        usart_lte_transmit_disable();
        return;
    }
}

void usart_lte_receive_isr()
{
	{
        if(RESET != USART_GetIntStatus(usart_peripheral_lte,USART_INT_RXDNE))
		{
            USART_ClrIntPendingBit(usart_peripheral_lte,USART_INT_RXDNE);
			b_usart_lte_rx_buff[w_usart_lte_receive_data_index] = (uint8_t)USART_ReceiveData(usart_peripheral_lte);
			w_usart_lte_receive_data_index++;
            sb_receive_state = receiving;

			w_usart_lte_rx_len += 1;

			if(w_usart_lte_rx_len >= cb_lte_mps_size)
			{
                w_usart_lte_rx_len = 0;
                w_usart_lte_receive_data_index = 0;
			}
		}
        else if(RESET != USART_GetIntStatus(usart_peripheral_lte,USART_INT_IDLEF))
        {
            USART_ReceiveData(usart_peripheral_lte);
            sb_receive_state = receive_done;
        }
	}

	return ;
}

void usart_lte_clear_data()
{
    w_usart_lte_rx_len = 0;
    w_usart_lte_receive_data_index = 0;
    sb_receive_state = receive_none;
}

uint8_t uart_lte_receive_data(uint8_t *pb_data_in,
                              uint16_t *pw_data_len,
                              uint16_t w_max_data_out_len)
{
    if(receive_done == sb_receive_state)
    {
        if(w_max_data_out_len > w_usart_lte_receive_data_index)
        {
            memcpy(pb_data_in,b_usart_lte_rx_buff,w_usart_lte_receive_data_index);
            *pw_data_len = w_usart_lte_receive_data_index;
        }
        else
        {
            memcpy(pb_data_in,b_usart_lte_rx_buff,w_max_data_out_len);
            *pw_data_len = w_max_data_out_len;
        }
        usart_lte_clear_data();
        return TRUE;
    }
    return FALSE;
}

//void usart_lte_int_flag_clean_isr()
//{
//    USART_ClrFlag(usart_peripheral_lte,USART_FLAG_CTSF);
//    USART_ClrFlag(usart_peripheral_lte,USART_FLAG_LINBD);
//    USART_ClrFlag(usart_peripheral_lte,USART_FLAG_NEF);
//    USART_ClrFlag(usart_peripheral_lte,USART_FLAG_FEF);
//    USART_ClrFlag(usart_peripheral_lte,USART_FLAG_PEF);
////    USART_ClrFlag(usart_peripheral_lte,USART_FLAG_BSY);
////    USART_ClrFlag(usart_peripheral_lte,USART_FLAG_EB);
////    USART_ClrFlag(usart_peripheral_lte,USART_FLAG_RT);
//}
