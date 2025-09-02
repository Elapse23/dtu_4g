#include "usart_config_log.h"
#include "n32g45x.h"
#include "application_general_macro.h"
//io
#define UART4_LOG
#ifdef UART4_LOG
#define usart_peripheral_log                       UART4
#define usart_peripheral_log_GPIO                  GPIOC
#define usart_peripheral_log_CLK                   RCC_APB1_PERIPH_UART4
#define usart_peripheral_log_GPIO_CLK              RCC_APB2_PERIPH_GPIOC
#define usart_peripheral_log_RxPin                 GPIO_PIN_11
#define usart_peripheral_log_TxPin                 GPIO_PIN_10
#define usart_peripheral_log_APBxClkCmd            RCC_EnableAPB1PeriphClk
//#define usart_peripheral_log_IRQn                  UART4_IRQn
//#define usart_peripheral_log_IRQHandler            UART4_IRQHandler
#else
#define usart_peripheral_log                       USART1
#define usart_peripheral_log_GPIO                  GPIOA
#define usart_peripheral_log_CLK                   RCC_APB2_PERIPH_USART1
#define usart_peripheral_log_GPIO_CLK              RCC_APB2_PERIPH_GPIOA
#define usart_peripheral_log_RxPin                 GPIO_PIN_10
#define usart_peripheral_log_TxPin                 GPIO_PIN_9
#define usart_peripheral_log_APBxClkCmd            RCC_EnableAPB2PeriphClk
#endif


//usart
static uint16_t w_usart_log_transmit_data_index = 0;
static uint16_t w_usart_log_transmit_data_len = 0;
static uint8_t *pb_usart_log_transmit_data = 0;
static uint32_t dw_usart_baudrate_log = 921600;	//921600;//115200

void usart_log_config()
{
    RCC_EnableAPB2PeriphClk(usart_peripheral_log_GPIO_CLK   |\
                            RCC_APB2_PERIPH_AFIO, ENABLE);
    usart_peripheral_log_APBxClkCmd(usart_peripheral_log_CLK, ENABLE);

    USART_DeInit(usart_peripheral_log);
    {
        GPIO_InitType GPIO_InitStructure;

        GPIO_InitStructure.Pin       = usart_peripheral_log_RxPin;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_InitPeripheral(usart_peripheral_log_GPIO, &GPIO_InitStructure);

        GPIO_InitStructure.Pin        = usart_peripheral_log_TxPin;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_InitPeripheral(usart_peripheral_log_GPIO, &GPIO_InitStructure);
    }
//    {
//        NVIC_InitType NVIC_InitStructure;
//
//        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
//
//        NVIC_InitStructure.NVIC_IRQChannel            = usart_peripheral_rs485_IRQn;
//        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//        NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;
//        NVIC_Init(&NVIC_InitStructure);
//    }
    {
        USART_InitType USART_InitStructure;

        USART_InitStructure.BaudRate            = dw_usart_baudrate_log;
        USART_InitStructure.WordLength          = USART_WL_8B;
        USART_InitStructure.StopBits            = USART_STPB_1;
        USART_InitStructure.Parity              = USART_PE_NO;
        USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
        USART_InitStructure.Mode                = USART_MODE_TX;

        USART_Init(usart_peripheral_log, &USART_InitStructure);

//        USART_ConfigInt(usart_peripheral_log, USART_INT_RXDNE, ENABLE);
//        USART_ConfigInt(usart_peripheral_log, USART_INT_TXC, ENABLE);

        USART_Enable(usart_peripheral_log, ENABLE);
    }

}

void usart_log_transmit_trig(uint16_t w_data_in_len,uint8_t *pb_data_in)
{
    if((NULL == pb_data_in) || (0 == w_data_in_len))
    {
        return;
    }

//    w_usart_log_transmit_data_index = 0;
//    w_usart_log_transmit_data_len = w_data_in_len;
//    pb_usart_log_transmit_data = pb_data_in;
//    USART_SendData(usart_peripheral_log,pb_usart_log_transmit_data[w_usart_log_transmit_data_index]);

    for(uint16_t w_cnt_i = 0;w_cnt_i < w_data_in_len;w_cnt_i++)
    {
        USART_SendData(usart_peripheral_log,pb_data_in[w_cnt_i]);

        while(RESET == USART_GetFlagStatus(usart_peripheral_log,USART_FLAG_TXDE))
        {
            ;
        }

        while(RESET == USART_GetFlagStatus(usart_peripheral_log,USART_FLAG_TXDE))
        {
            ;
        }
    }
}

void usart_log_transmit_isr()
{
    if(SET == USART_GetFlagStatus(usart_peripheral_log,USART_INT_TXC))
    {
        USART_ClrFlag(usart_peripheral_log,USART_INT_TXC);
    }

    if(SET == USART_GetFlagStatus(usart_peripheral_log,USART_INT_TXDE))
    {
        USART_ClrFlag(usart_peripheral_log,USART_INT_TXDE);
    }

    w_usart_log_transmit_data_index++;
    if(w_usart_log_transmit_data_index < w_usart_log_transmit_data_len)
    {
        USART_SendData(usart_peripheral_log,pb_usart_log_transmit_data[w_usart_log_transmit_data_index]);
    }
    else
    {
        w_usart_log_transmit_data_index = 0;
        w_usart_log_transmit_data_len = 0;

        return;
    }
}

void usart_log_receive_isr()
{

}

void usart_log_int_flag_clean_isr()
{
    USART_ClrFlag(usart_peripheral_log,USART_FLAG_CTSF);
    USART_ClrFlag(usart_peripheral_log,USART_FLAG_LINBD);
    USART_ClrFlag(usart_peripheral_log,USART_FLAG_NEF);
    USART_ClrFlag(usart_peripheral_log,USART_FLAG_FEF);
    USART_ClrFlag(usart_peripheral_log,USART_FLAG_PEF);
//    USART_ClrFlag(usart_peripheral_log,USART_FLAG_BSY);
//    USART_ClrFlag(usart_peripheral_log,USART_FLAG_EB);
//    USART_ClrFlag(usart_peripheral_log,USART_FLAG_RT);
}
//while(RESET == usart_flag_get(com, USART_INT_TXDE))
//{
//    ;
//}
//
//while(RESET == usart_flag_get(com, USART_INT_TXC))
//{
//    ;
//}
