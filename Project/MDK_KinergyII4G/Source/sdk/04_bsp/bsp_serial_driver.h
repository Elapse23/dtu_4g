/**
 * =====================================================================================
 * @file        serial_driver.h
 * @brief       通用串口驱动头文件 (GD32F470适配)
 * @author      23Elapse & Gemini
 * @version     2.1 (Refactored)
 * @date        2025-06-14
 * @note        本驱动为所有串口通信提供底层硬件操作。
 * =====================================================================================
 */
#ifndef __BSP_SERIAL_DRIVER_H
#define __BSP_SERIAL_DRIVER_H

#include "ring_buffer.h"
#include "n32g45x_usart.h"


/* RS485 UART: 简化命名 */
#define RS485_UART                                   UART5
#define RS485_TX_GPIO                                GPIOC
#define RS485_RX_GPIO                                GPIOD
#define RS485_DIR_GPIO                               GPIOC
#define RS485_UART_CLK                               RCC_APB1_PERIPH_UART5
#define RS485_TX_GPIO_CLK                            RCC_APB2_PERIPH_GPIOC
#define RS485_RX_GPIO_CLK                            RCC_APB2_PERIPH_GPIOD
#define RS485_DIR_GPIO_CLK                           RCC_APB2_PERIPH_GPIOC
#define RS485_RX_PIN                                 GPIO_PIN_2
#define RS485_TX_PIN                                 GPIO_PIN_12
#define RS485_DIR_PIN                                GPIO_PIN_3
#define RS485_CLK_CMD                                RCC_EnableAPB1PeriphClk
#define RS485_IRQn                                   UART5_IRQn
#define RS485_IRQHandler                             UART5_IRQHandler

/* 兼容旧宏名（逐步迁移可直接替换为上面的短名） */
#define usart_peripheral_rs485                       RS485_UART
#define usart_peripheral_rs485_Tx_GPIO               RS485_TX_GPIO
#define usart_peripheral_rs485_Rx_GPIO               RS485_RX_GPIO
#define usart_peripheral_rs485_Dir_GPIO              RS485_DIR_GPIO
#define usart_peripheral_rs485_CLK                   RS485_UART_CLK
#define usart_peripheral_rs485_Tx_GPIO_CLK           RS485_TX_GPIO_CLK
#define usart_peripheral_rs485_Rx_GPIO_CLK           RS485_RX_GPIO_CLK
#define usart_peripheral_rs485_Dir_GPIO_CLK          RS485_DIR_GPIO_CLK
#define usart_peripheral_rs485_RxPin                 RS485_RX_PIN
#define usart_peripheral_rs485_TxPin                 RS485_TX_PIN
#define usart_peripheral_rs485_DirPin                RS485_DIR_PIN
#define usart_peripheral_rs485_APBxClkCmd            RS485_CLK_CMD
#define usart_peripheral_rs485_IRQn                  RS485_IRQn
#define usart_peripheral_rs485_IRQHandler            RS485_IRQHandler

/* RS485方向控制：简化命名（建议：SWJ重映射请在一次性板级初始化中完成，不放在宏内） */
#define RS485_DIR_TX()                                  GPIO_SetBits(RS485_DIR_GPIO, RS485_DIR_PIN)
#define RS485_DIR_RX()                                  GPIO_ResetBits(RS485_DIR_GPIO, RS485_DIR_PIN)

/* 兼容旧宏名 */
#define rs485_send_direction_ctrl()                     RS485_DIR_TX()
#define rs485_receive_direction_ctrl()                  RS485_DIR_RX()
/* LTE UART: 简化命名 */
#define LTE_UART                                    USART3
#define LTE_GPIO                                    GPIOB
#define LTE_UART_CLK                                RCC_APB1_PERIPH_USART3
#define LTE_GPIO_CLK                                RCC_APB2_PERIPH_GPIOB
#define LTE_RX_PIN                                  GPIO_PIN_11
#define LTE_TX_PIN                                  GPIO_PIN_10
#define LTE_CLK_CMD                                 RCC_EnableAPB1PeriphClk
#define LTE_IRQn                                    USART3_IRQn
#define LTE_IRQHandler                              USART3_IRQHandler

/* 兼容旧宏名（LTE） */
#define usart_peripheral_lte                        LTE_UART
#define usart_peripheral_lte_GPIO                   LTE_GPIO
#define usart_peripheral_lte_CLK                    LTE_UART_CLK
#define usart_peripheral_lte_GPIO_CLK               LTE_GPIO_CLK
#define usart_peripheral_lte_RxPin                  LTE_RX_PIN
#define usart_peripheral_lte_TxPin                  LTE_TX_PIN
#define usart_peripheral_lte_APBxClkCmd             LTE_CLK_CMD
#define usart_peripheral_lte_IRQn                   LTE_IRQn
#define usart_peripheral_lte_IRQHandler             LTE_IRQHandler

/* LOG UART: 简化命名 */
#define LOG_UART                                    UART4
#define LOG_GPIO                                    GPIOC
#define LOG_UART_CLK                                RCC_APB1_PERIPH_UART4
#define LOG_GPIO_CLK                                RCC_APB2_PERIPH_GPIOC
#define LOG_RX_PIN                                  GPIO_PIN_11
#define LOG_TX_PIN                                  GPIO_PIN_10
#define LOG_CLK_CMD                                 RCC_EnableAPB1PeriphClk
#define LOG_IRQn                                    UART4_IRQn
#define LOG_IRQHandler                              UART4_IRQHandler

/* 兼容旧宏名（LOG） */
#define usart_peripheral_log                        LOG_UART
#define usart_peripheral_log_GPIO                   LOG_GPIO
#define usart_peripheral_log_CLK                    LOG_UART_CLK
#define usart_peripheral_log_GPIO_CLK               LOG_GPIO_CLK
#define usart_peripheral_log_RxPin                  LOG_RX_PIN
#define usart_peripheral_log_TxPin                  LOG_TX_PIN
#define usart_peripheral_log_APBxClkCmd             LOG_CLK_CMD
#define usart_peripheral_log_IRQn                   LOG_IRQn
#define usart_peripheral_log_IRQHandler             LOG_IRQHandler

// 串口接收缓冲区默认大小
#define SERIAL_RX_BUFFER_DEFAULT_SIZE 1024  // 从512增加到1024，提供更大缓冲空间

// 串口操作状态码
typedef enum {
    SERIAL_OK = 0,
    SERIAL_ERR_PARAM,        // 参数错误
    SERIAL_ERR_INIT,         // 初始化失败
    SERIAL_ERR_TRANSMIT,     // 发送失败
    SERIAL_ERR_BUFFER_FULL,  // 接收缓冲区满
    SERIAL_ERR_NO_DATA,      // 没有可读取的数据
    SERIAL_ERR_FRAME,        // 帧错误 (校验、溢出等)
    SERIAL_ERR_MUTEX_TIMEOUT,// 获取互斥锁超时
    SERIAL_ERR_TIMEOUT       // 发送/接收超时
} Serial_Status_t;

// 串口工作模式
typedef enum {
    UART_MODE,  // 普通UART模式
    RS485_MODE  // RS485模式，需要DE引脚控制收发
} Serial_Mode_t;

// 串口设备结构体 (作为配置和句柄)
// typedef struct {
//     // 硬件配置
//     uint32_t      instance;   // USART 外设基地址 (如 USART0, USART1)
//     uint32_t      tx_port;    // TX GPIO 端口 (如 GPIOA, GPIOB)
//     uint32_t      tx_pin;     // TX GPIO 引脚 (如 GPIO_PIN_9)
//     uint32_t      rx_port;    // RX GPIO 端口
//     uint32_t      rx_pin;     // RX GPIO 引脚
//     uint32_t      de_port;    // RS485 DE/RE 控制引脚, UART模式下为0
//     uint32_t      de_pin;     // RS485 DE/RE 引脚
//     uint32_t      baudrate;   // 波特率
//     uint8_t       af;         // GPIO复用功能 (如 GPIO_AF_7)
//     IRQn_Type     irqn;       // 中断号 (如 USART0_IRQn)

//     // 运行时数据
//     RingBuffer_t   rx_buffer;    // 接收环形缓冲区
//     Serial_Mode_t  mode;         // 工作模式
//     void* tx_mutex;     // 用于保护发送的互斥锁
//     void* rx_semaphore; // 用于通知有新数据的信号量 (可选)
//     // uint32_t       silent_ticks; // RS485发送后的静默时间 (如需要)
// } Serial_Device_t;

// --- 公共API ---
/**
 * @brief 通用状态枚举
 */
typedef enum
{
    COMMON_OK = 0,
    COMMON_ERR_INIT
} Common_Status;

/**
 * @brief 初始化一个串口设备
 * @param com 串口实例指针（如 RS485_UART/LTE_UART/LOG_UART）
 * @param baudrate 波特率
 * @return Serial_Status_t 操作状态
 */
Serial_Status_t Serial_Driver_Init(USART_Module* com, uint32_t baudrate);

/**
 * @brief 反初始化一个串口设备
 * @param com 串口实例指针（如 RS485_UART/LTE_UART/LOG_UART）
 * @return Serial_Status_t 操作状态
 */
Serial_Status_t Serial_Driver_Deinit(USART_Module* com);

/**
 * @brief 将调试端口重映射为仅 SWD（禁用 JTAG，释放相关引脚）
 * @note 一次性调用：需开启 AFIO 时钟后调用一次即可
 */
void bsp_debug_port_remap_swd_only(void);

#endif
