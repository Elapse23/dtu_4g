   ; THUMB

    SECTION .text:CODE:REORDER:NOROOT(2)

    EXTERN  NMI_Handler
NMI_Size:
    PUBLIC  NMI_Size
    LDR     R0, = NMI_Handler   
    BX      R0              
    ; B NMI_Handler
     
    EXTERN  HardFault_Handler      
HardFault_Size:
    PUBLIC  HardFault_Size
    LDR     R0, = HardFault_Handler 
    BX      R0     
    ;B HardFault_Handler
      
    
    EXTERN  MemManage_Handler  
MemManage_Size:
    PUBLIC  MemManage_Size  
    LDR     R0, = MemManage_Handler 
    BX      R0     
    ;B MemManage_Handler 
    
    EXTERN  BusFault_Handler 
    PUBLIC  BusFault_Size
BusFault_Size:
    LDR     R0, = BusFault_Handler 
    BX      R0     
    ;B BusFault_Handler     

    
    EXTERN  UsageFault_Handler 
    PUBLIC  UsageFault_Size                   
UsageFault_Size:
    LDR     R0, = UsageFault_Handler  
    BX      R0     
    ;B UsageFault_Handler
  
               
    EXTERN  SVC_Handler    
    PUBLIC  SVC_Size          
SVC_Size:
    LDR     R0, = SVC_Handler   
    BX      R0     
    ;B SVC_Handler  
 
    EXTERN  DebugMon_Handler  
    PUBLIC  DebugMon_Size
             
DebugMon_Size:
    LDR     R0, = DebugMon_Handler  
    BX      R0     
    ;B DebugMon_Handler  
 
    EXTERN  xPortPendSVHandler  
    PUBLIC  PendSV_Size

PendSV_Size:
    EXTERN  PendSV_Handler 
    PUBLIC  PendSV_Size   
    LDR     R0, = PendSV_Handler 
    BX      R0     
    ;B PendSV_Handler 

    EXTERN  SysTick_Handler  
    PUBLIC  SysTick_Size 
SysTick_Size:    
    LDR     R0, = SysTick_Handler
    BX      R0    
    ;B  SysTick_Handler
    
//DCD WWDG_IRQHandler           ; Window Watchdog        
    EXTERN  WWDG_IRQHandler  
    PUBLIC  WWDG_IRQSize    
WWDG_IRQSize:
    LDR     R0, = WWDG_IRQHandler
    BX      R0    
    ;B  WWDG_IRQHandler
    

//DCD       PVD_IRQHandler            ; PVD through EXTI Line detect
    EXTERN  PVD_IRQHandler
    PUBLIC  PVD_IRQSize
PVD_IRQSize:
    LDR     R0, = PVD_IRQHandler
    BX      R0    
    ;B PVD_IRQHandler

//DCD       TAMPER_IRQHandler         ; Tamper
    EXTERN  TAMPER_IRQHandler
    PUBLIC  TAMPER_IRQSize
TAMPER_IRQSize:
    LDR     R0, = TAMPER_IRQHandler  
    BX      R0     
    ;B TAMPER_IRQHandler

    
//DCD     RTC_IRQHandler            ; RTC
    EXTERN  RTC_IRQHandler
    PUBLIC  RTC_IRQSize
RTC_IRQSize:
    LDR     R0, = RTC_IRQHandler   
    BX      R0     
    ;B RTC_IRQHandler 
    

//DCD     FLASH_IRQHandler          ; Flash
    EXTERN  FLASH_IRQHandler
    PUBLIC  FLASH_IRQSize
FLASH_IRQSize:
    LDR     R0, = FLASH_IRQHandler   
    BX      R0     
    ; B FLASH_IRQHandler     

//DCD       RCC_IRQHandler            ; RCC
    EXTERN  RCC_IRQHandler
    PUBLIC  RCC_IRQSize
RCC_IRQSize:
    LDR     R0, = RCC_IRQHandler  
    BX      R0     
    ;B RCC_IRQHandler   

//DCD       EXTI0_IRQHandler          ; EXTI Line 0
    EXTERN  EXTI0_IRQHandler
    PUBLIC  EXTI0_IRQSize
EXTI0_IRQSize:
    LDR     R0, =  EXTI0_IRQHandler   
    BX      R0     
    ;B EXTI0_IRQHandler   


//DCD       EXTI1_IRQHandler          ; EXTI Line 1
    EXTERN  EXTI1_IRQHandler
    PUBLIC  EXTI1_IRQSize
EXTI1_IRQSize:
    LDR     R0, =  EXTI1_IRQHandler   
    BX      R0     
    ;B EXTI1_IRQHandler   

    
//DCD       EXTI2_IRQHandler          ; EXTI Line 2
    EXTERN  EXTI2_IRQHandler
    PUBLIC  EXTI2_IRQSize
EXTI2_IRQSize:
    LDR     R0, = EXTI2_IRQHandler   
    BX      R0     
    ;B EXTI2_IRQHandler   
    
//DCD       EXTI3_IRQHandler          ; EXTI Line 3
    EXTERN  EXTI3_IRQHandler
    PUBLIC  EXTI3_IRQSize
EXTI3_IRQSize:
    LDR     R0, = EXTI3_IRQHandler   
    BX      R0     
    ;B EXTI3_IRQHandler   
    
//DCD       EXTI4_IRQHandler          ; EXTI Line 4
    EXTERN  EXTI4_IRQHandler
    PUBLIC  EXTI4_IRQSize
EXTI4_IRQSize:
    LDR     R0, = EXTI4_IRQHandler   
    BX      R0     
    ;B EXTI4_IRQHandler
    
//DCD       DMA1_Channel1_IRQHandler  ; DMA1 Channel 1
    EXTERN  DMA1_Channel1_IRQHandler
    PUBLIC  DMA1_Channel1_IRQSize
DMA1_Channel1_IRQSize:
    LDR     R0, = DMA1_Channel1_IRQHandler  
    BX      R0     
    ;B DMA1_Channel1_IRQHandler  
    
//DCD       DMA1_Channel2_IRQHandler  ; DMA1 Channel 2
    EXTERN  DMA1_Channel2_IRQHandler
    PUBLIC  DMA1_Channel2_IRQSize
DMA1_Channel2_IRQSize:
    LDR     R0, = DMA1_Channel2_IRQHandler 
    BX      R0     
    ;B DMA1_Channel2_IRQHandler  
    
//DCD       DMA1_Channel3_IRQHandler  ; DMA1 Channel 3
    EXTERN  DMA1_Channel3_IRQHandler
    PUBLIC  DMA1_Channel3_IRQSize
DMA1_Channel3_IRQSize:
    LDR     R0, = DMA1_Channel3_IRQHandler   
    BX      R0     
    ;B DMA1_Channel3_IRQHandler   
    
//DCD       DMA1_Channel4_IRQHandler  ; DMA1 Channel 4
    EXTERN  DMA1_Channel4_IRQHandler
    PUBLIC  DMA1_Channel4_IRQSize
DMA1_Channel4_IRQSize:
    LDR     R0, = DMA1_Channel4_IRQHandler    
    BX      R0     
    ;B DMA1_Channel4_IRQHandler   
  
    
//DCD       DMA1_Channel5_IRQHandler  ; DMA1 Channel 5
    EXTERN  DMA1_Channel5_IRQHandler
    PUBLIC  DMA1_Channel5_IRQSize
            DMA1_Channel5_IRQSize:
    LDR     R0, = DMA1_Channel5_IRQHandler  
    BX      R0     
    ;B  DMA1_Channel5_IRQHandler   
  
    
//DCD       DMA1_Channel6_IRQHandler  ; DMA1 Channel 6
    EXTERN  DMA1_Channel6_IRQHandler
    PUBLIC  DMA1_Channel6_IRQSize
DMA1_Channel6_IRQSize:
    LDR     R0, = DMA1_Channel6_IRQHandler  
    BX      R0     
    ;B DMA1_Channel6_IRQHandler   
  
    
//DCD       DMA1_Channel7_IRQHandler  ; DMA1 Channel 7
    EXTERN  DMA1_Channel7_IRQHandler
    PUBLIC  DMA1_Channel7_IRQSize
DMA1_Channel7_IRQSize:
    LDR     R0, = DMA1_Channel7_IRQHandler  
    BX      R0     
    ;B DMA1_Channel7_IRQHandler   
  
    
//DCD       ADC1_2_IRQHandler         ; ADC1 & ADC2
    EXTERN  ADC1_2_IRQHandler
    PUBLIC  ADC1_2_IRQSize
ADC1_2_IRQSize:
    LDR     R0, = ADC1_2_IRQHandler    
    BX      R0     
    ;B ADC1_2_IRQHandler   
  
    
//DCD       USB_HP_CAN1_TX_IRQHandler  ; USB High Priority or CAN1 TX
    EXTERN  USB_HP_CAN1_TX_IRQHandler
    PUBLIC  USB_HP_CAN1_TX_IRQSize
USB_HP_CAN1_TX_IRQSize:
    LDR     R0, = USB_HP_CAN1_TX_IRQHandler 
    BX      R0     
    ;B USB_HP_CAN1_TX_IRQHandler   
    
  
    
//DCD       USB_LP_CAN1_RX0_IRQHandler ; USB Low  Priority or CAN1 RX0
    EXTERN  USB_LP_CAN1_RX0_IRQHandler
    PUBLIC  USB_LP_CAN1_RX0_IRQSize
USB_LP_CAN1_RX0_IRQSize:
    LDR     R0, = USB_LP_CAN1_RX0_IRQHandler 
    BX      R0     
    ;B USB_LP_CAN1_RX0_IRQHandler   
   
  
    
//DCD       CAN1_RX1_IRQHandler       ; CAN1 RX1
    EXTERN  CAN1_RX1_IRQHandler
    PUBLIC  CAN1_RX1_IRQSize
CAN1_RX1_IRQSize:
    LDR     R0, = CAN1_RX1_IRQHandler   
    BX      R0     
    ;B CAN1_RX1_IRQHandler   
   
//DCD       CAN1_SCE_IRQHandler       ; CAN1 SCE
    EXTERN  CAN1_SCE_IRQHandler
    PUBLIC  CAN1_SCE_IRQSize
CAN1_SCE_IRQSize:
    LDR     R0, = CAN1_SCE_IRQHandler   
    BX      R0     
    ;B CAN1_SCE_IRQHandler   
   
//DCD       EXTI9_5_IRQHandler        ; EXTI Line 9..5
    EXTERN  EXTI9_5_IRQHandler
    PUBLIC  EXTI9_5_IRQSize
EXTI9_5_IRQSize:
    LDR     R0, = EXTI9_5_IRQHandler   
    BX      R0     
    ;B EXTI9_5_IRQHandler   
   
//DCD       TIM1_BRK_IRQHandler       ; TIM1 Break
    EXTERN  TIM1_BRK_IRQHandler
    PUBLIC  TIM1_BRK_IRQSize
TIM1_BRK_IRQSize:
    LDR     R0, = TIM1_BRK_IRQHandler  
    BX      R0     
    ;B TIM1_BRK_IRQHandler   
   
//DCD       TIM1_UP_IRQHandler        ; TIM1 Update
    EXTERN  TIM1_UP_IRQHandler
    PUBLIC  TIM1_UP_IRQSize
TIM1_UP_IRQSize:
    LDR     R0, = TIM1_UP_IRQHandler   
    BX      R0     
    ;B TIM1_UP_IRQHandler   
   
//DCD       TIM1_TRG_COM_IRQHandler   ; TIM1 Trigger and Commutation
    EXTERN  TIM1_TRG_COM_IRQHandler
    PUBLIC  TIM1_TRG_COM_IRQSize
TIM1_TRG_COM_IRQSize:
    LDR     R0, = TIM1_TRG_COM_IRQHandler  
    BX      R0     
    ;B TIM1_TRG_COM_IRQHandler   
   
//DCD       TIM1_CC_IRQHandler        ; TIM1 Capture Compare
    EXTERN  TIM1_CC_IRQHandler
    PUBLIC  TIM1_CC_IRQSize
TIM1_CC_IRQSize:
    LDR     R0, = TIM1_CC_IRQHandler   
    BX      R0     
    ;B TIM1_CC_IRQHandler   
   
//DCD       TIM2_IRQHandler           ; TIM2
    EXTERN  TIM2_IRQHandler
    PUBLIC  TIM2_IRQSize
TIM2_IRQSize:
    LDR     R0, = TIM2_IRQHandler   
    BX      R0     
    ;B TIM2_IRQHandler   
   
//DCD       TIM3_IRQHandler           ; TIM3
    EXTERN  TIM3_IRQHandler
    PUBLIC  TIM3_IRQSize
TIM3_IRQSize:
    LDR     R0, = TIM3_IRQHandler  
    BX      R0     
    ;B TIM3_IRQHandler   
   
//DCD       TIM4_IRQHandler           ; TIM4
    EXTERN  TIM4_IRQHandler
    PUBLIC  TIM4_IRQSize
TIM4_IRQSize:
    LDR     R0, = TIM4_IRQHandler  
    BX      R0     
    ;B TIM4_IRQHandler   
   
//DCD       I2C1_EV_IRQHandler        ; I2C1 Event
    EXTERN  I2C1_EV_IRQHandler
    PUBLIC  I2C1_EV_IRQSize
I2C1_EV_IRQSize:
    LDR     R0, = I2C1_EV_IRQHandler    
    BX      R0     
    ;B I2C1_EV_IRQHandler   
   
//DCD       I2C1_ER_IRQHandler        ; I2C1 Error
    EXTERN  I2C1_ER_IRQHandler
    PUBLIC  I2C1_ER_IRQSize
I2C1_ER_IRQSize:
    LDR     R0, = I2C1_ER_IRQHandler    
    BX      R0     
    ;B I2C1_ER_IRQHandler   
   
//DCD     I2C2_EV_IRQHandler        ; I2C2 Event
    EXTERN  I2C2_EV_IRQHandler
    PUBLIC  I2C2_EV_IRQSize
I2C2_EV_IRQSize:
    LDR     R0, = I2C2_EV_IRQHandler   
    BX      R0     
    ;B I2C2_EV_IRQHandler   
   
//DCD       I2C2_ER_IRQHandler        ; I2C2 Error
    EXTERN  I2C2_ER_IRQHandler
    PUBLIC  I2C2_ER_IRQSize
I2C2_ER_IRQSize:
    LDR     R0, = I2C2_ER_IRQHandler   
    BX      R0     
    ;B I2C2_ER_IRQHandler   
   
//DCD       SPI1_IRQHandler           ; SPI1
    EXTERN  SPI1_IRQHandler
    PUBLIC  SPI1_IRQSize
SPI1_IRQSize:
    LDR     R0, = SPI1_IRQHandler  
    BX      R0     
    ;B SPI1_IRQHandler   
   
//DCD       SPI2_IRQHandler           ; SPI2
    EXTERN  SPI2_IRQHandler
    PUBLIC  SPI2_IRQSize
SPI2_IRQSize:
    LDR     R0, = SPI2_IRQHandler  
    BX      R0     
    ;B SPI2_IRQHandler   
   
//DCD       USART1_IRQHandler         ; USART1
    EXTERN  USART1_IRQHandler
    PUBLIC  USART1_IRQSize
USART1_IRQSize:
    LDR     R0, = USART1_IRQHandler   
    BX      R0     
    ;B USART1_IRQHandler   
   
//DCD       USART2_IRQHandler         ; USART2
    EXTERN  USART2_IRQHandler
    PUBLIC  USART2_IRQSize
USART2_IRQSize:
    LDR     R0, = USART2_IRQHandler  
    BX      R0     
    ;B USART2_IRQHandler   
   
//DCD       USART3_IRQHandler         ; USART3
    EXTERN  USART3_IRQHandler
    PUBLIC  USART3_IRQSize
USART3_IRQSize:
    LDR     R0, = USART3_IRQHandler    
    BX      R0     
    ;B USART3_IRQHandler   
   
//DCD       EXTI15_10_IRQHandler      ; EXTI Line 15..10
    EXTERN  EXTI15_10_IRQHandler
    PUBLIC  EXTI15_10_IRQSize
EXTI15_10_IRQSize:
    LDR     R0, = EXTI15_10_IRQHandler    
    BX      R0     
    ;B EXTI15_10_IRQHandler   
   
//DCD       RTCAlarm_IRQHandler       ; RTC Alarm through EXTI Line
    EXTERN  RTCAlarm_IRQHandler
    PUBLIC  RTCAlarm_IRQSize
RTCAlarm_IRQSize:
    LDR     R0, = RTCAlarm_IRQHandler   
    BX      R0     
    ;B RTCAlarm_IRQHandler   
   
//DCD       USBWakeUp_IRQHandler      ; USB Wakeup from suspend
    EXTERN  USBWakeUp_IRQHandler
    PUBLIC  USBWakeUp_IRQSize
USBWakeUp_IRQSize:
    LDR     R0, = USBWakeUp_IRQHandler   
    BX      R0     
    ;B USBWakeUp_IRQHandler   
   
//DCD       TIM8_BRK_IRQHandler       ; TIM8 Break
    EXTERN  TIM8_BRK_IRQHandler
    PUBLIC  TIM8_BRK_IRQSize
TIM8_BRK_IRQSize:
    LDR     R0, = TIM8_BRK_IRQHandler  
    BX      R0     
    ;B TIM8_BRK_IRQHandler   
   
//DCD       TIM8_UP_IRQHandler        ; TIM8 Update
    EXTERN  TIM8_UP_IRQHandler
    PUBLIC  TIM8_UP_IRQSize
TIM8_UP_IRQSize:    
    LDR     R0, = TIM8_UP_IRQHandler   
    BX      R0     
    ; B TIM8_UP_IRQHandler   
   
//DCD       TIM8_TRG_COM_IRQHandler   ; TIM8 Trigger and Commutation
    EXTERN  TIM8_TRG_COM_IRQHandler
    PUBLIC  TIM8_TRG_COM_IRQSize
TIM8_TRG_COM_IRQSize:
    LDR     R0, = TIM8_TRG_COM_IRQHandler  
    BX      R0     
    ;B TIM8_TRG_COM_IRQHandler   
   
//DCD       TIM8_CC_IRQHandler        ; TIM8 Capture Compare
    EXTERN  TIM8_CC_IRQHandler
    PUBLIC  TIM8_CC_IRQSize
TIM8_CC_IRQSize:
    LDR     R0, = TIM8_CC_IRQHandler 
    BX      R0     
    ;B TIM8_CC_IRQHandler   
   
//DCD       ADC3_4_IRQHandler         ; ADC3 & ADC4
    EXTERN  ADC3_4_IRQHandler
    PUBLIC  ADC3_4_IRQSize
ADC3_4_IRQSize:
    LDR     R0, = ADC3_4_IRQHandler 
    BX      R0     
    ;B ADC3_4_IRQHandler   
   
//DCD       XFMC_IRQHandler           ; XFMC
    EXTERN  XFMC_IRQHandler
    PUBLIC  XFMC_IRQSize
XFMC_IRQSize:
    LDR     R0, = XFMC_IRQHandler  
    BX      R0     
    ;B XFMC_IRQHandler   
   
//DCD       SDIO_IRQHandler           ; SDIO
    EXTERN  SDIO_IRQHandler
    PUBLIC  SDIO_IRQSize
SDIO_IRQSize:
    LDR     R0, = SDIO_IRQHandler   
    BX      R0     
    ;B SDIO_IRQHandler   
   
//DCD     TIM5_IRQHandler           ; TIM5
    EXTERN  TIM5_IRQHandler
    PUBLIC  TIM5_IRQSize
TIM5_IRQSize:
    LDR     R0, = TIM5_IRQHandler  
    BX      R0     
    ;B TIM5_IRQHandler   
   
//DCD       SPI3_IRQHandler           ; SPI3
    EXTERN  SPI3_IRQHandler
    PUBLIC  SPI3_IRQSize
SPI3_IRQSize:
    LDR     R0, = SPI3_IRQHandler    
    BX      R0     
    ;B SPI3_IRQHandler   
   
//DCD       UART4_IRQHandler          ; UART4
    EXTERN  UART4_IRQHandler
    PUBLIC  UART4_IRQSize
UART4_IRQSize:
    LDR     R0, = UART4_IRQHandler   
    BX      R0     
    ;   B UART4_IRQHandler   
   
//DCD       UART5_IRQHandler          ; UART5
    EXTERN  UART5_IRQHandler
    PUBLIC  UART5_IRQSize
UART5_IRQSize:
    LDR     R0, = UART5_IRQHandler   
    BX      R0     
    ;B UART5_IRQHandler   

//DCD       TIM6_IRQHandler           ; TIM6
    EXTERN  TIM6_IRQHandler
    PUBLIC  TIM6_IRQSize
TIM6_IRQSize:
    LDR     R0, = TIM6_IRQHandler  
    BX      R0     
    ;B TIM6_IRQHandler   
   
//DCD       TIM7_IRQHandler           ; TIM7
    EXTERN  TIM7_IRQHandler
    PUBLIC  TIM7_IRQSize
TIM7_IRQSize:
    LDR     R0, = TIM7_IRQHandler    
    BX      R0     
    ;B TIM7_IRQHandler   
   
//DCD       DMA2_Channel1_IRQHandler  ; DMA2 Channel1
    EXTERN  DMA2_Channel1_IRQHandler
    PUBLIC  DMA2_Channel1_IRQSize
DMA2_Channel1_IRQSize:
    LDR     R0, = DMA2_Channel1_IRQHandler   
    BX      R0     
    ;B DMA2_Channel1_IRQHandler   
   
//DCD       DMA2_Channel2_IRQHandler  ; DMA2 Channel2
    EXTERN  DMA2_Channel2_IRQHandler
    PUBLIC  DMA2_Channel2_IRQSize
DMA2_Channel2_IRQSize:
    LDR     R0, = DMA2_Channel2_IRQHandler   
    BX      R0     
    ;B DMA2_Channel2_IRQHandler   
   
//DCD       DMA2_Channel3_IRQHandler  ; DMA2 Channel3
    EXTERN  DMA2_Channel3_IRQHandler
    PUBLIC  DMA2_Channel3_IRQSize
DMA2_Channel3_IRQSize:
    LDR     R0, = DMA2_Channel3_IRQHandler  
    BX      R0     
    ;B DMA2_Channel3_IRQHandler   
   
//DCD       DMA2_Channel4_IRQHandler  ; DMA2 Channel4
    EXTERN  DMA2_Channel4_IRQHandler
    PUBLIC  DMA2_Channel4_IRQSize
DMA2_Channel4_IRQSize:
    LDR     R0, = DMA2_Channel4_IRQHandler  
    BX      R0     
    ;B DMA2_Channel4_IRQHandler   
   
//DCD       DMA2_Channel5_IRQHandler  ; DMA2 Channel5
    EXTERN  DMA2_Channel5_IRQHandler
    PUBLIC  DMA2_Channel5_IRQSize
DMA2_Channel5_IRQSize:
    LDR     R0, = DMA2_Channel5_IRQHandler   
    BX      R0     
    ;B DMA2_Channel5_IRQHandler   
   
//DCD       ETH_IRQHandler            ; Ethernet global interrupt
    EXTERN  ETH_IRQHandler
    PUBLIC  ETH_IRQSize
ETH_IRQSize:
    LDR     R0, = ETH_IRQHandler   
    BX      R0     
    ;B ETH_IRQHandler   
   
//DCD       ETH_WKUP_IRQHandler       ; Ethernet Wakeup through EXTI line interrupt
    EXTERN  ETH_WKUP_IRQHandler
    PUBLIC  ETH_WKUP_IRQSize
ETH_WKUP_IRQSize:
    LDR     R0, = ETH_WKUP_IRQHandler    
    BX      R0     
    ;B ETH_WKUP_IRQHandler   
   
//DCD       CAN2_TX_IRQHandler        ; CAN2 TX
    EXTERN  CAN2_TX_IRQHandler
    PUBLIC  CAN2_TX_IRQSize
CAN2_TX_IRQSize:
    LDR     R0, = CAN2_TX_IRQHandler
    BX      R0     
    ;B CAN2_TX_IRQHandler   
   
//DCD       CAN2_RX0_IRQHandler       ; CAN2 RX0
    EXTERN  CAN2_RX0_IRQHandler
    PUBLIC  CAN2_RX0_IRQSize
CAN2_RX0_IRQSize:
    LDR     R0, = CAN2_RX0_IRQHandler   
    BX      R0     
    ;B CAN2_RX0_IRQHandler   
   
//DCD       CAN2_RX1_IRQHandler       ; CAN2 RX1
    EXTERN  CAN2_RX1_IRQHandler
    PUBLIC  CAN2_RX1_IRQSize
CAN2_RX1_IRQSize:
    LDR     R0, = CAN2_RX1_IRQHandler   
    BX      R0     
    ;B CAN2_RX1_IRQHandler   
   
//DCD       CAN2_SCE_IRQHandler       ; CAN2 SCE
    EXTERN  CAN2_SCE_IRQHandler
    PUBLIC  CAN2_SCE_IRQSize
CAN2_SCE_IRQSize:
    LDR     R0, = CAN2_SCE_IRQHandler   
    BX      R0     
    ;B CAN2_SCE_IRQHandler   
   
//DCD       QSPI_IRQHandler           ; QSPI
    EXTERN  QSPI_IRQHandler
    PUBLIC  QSPI_IRQSize
QSPI_IRQSize:
    LDR     R0, = QSPI_IRQHandler  
    BX      R0     
    ;B QSPI_IRQHandler   
   
//DCD       DMA2_Channel6_IRQHandler  ; DMA2 Channel6
    EXTERN  DMA2_Channel6_IRQHandler
    PUBLIC  DMA2_Channel6_IRQSize
DMA2_Channel6_IRQSize:
    LDR     R0, = DMA2_Channel6_IRQHandler  
    BX      R0     
    ;B DMA2_Channel6_IRQHandler   
   
//DCD       DMA2_Channel7_IRQHandler  ; DMA2 Channel7
    EXTERN  DMA2_Channel7_IRQHandler
    PUBLIC  DMA2_Channel7_IRQSize
DMA2_Channel7_IRQSize:
    LDR     R0, = SVC_Handler   
    BX      R0     
    ;B DMA2_Channel7_IRQHandler   
   
//DCD       I2C3_EV_IRQHandler        ; I2C3 event
    EXTERN  I2C3_EV_IRQHandler
    PUBLIC  I2C3_EV_IRQSize
I2C3_EV_IRQSize:
    LDR     R0, = I2C3_EV_IRQHandler   
    BX      R0     
    ;B I2C3_EV_IRQHandler   
   
//DCD       I2C3_ER_IRQHandler        ; I2C3 error
    EXTERN  I2C3_ER_IRQHandler
    PUBLIC  I2C3_ER_IRQSize
I2C3_ER_IRQSize:
    LDR     R0, = I2C3_ER_IRQHandler   
    BX      R0     
    ;B I2C3_ER_IRQHandler   
   
//DCD       I2C4_EV_IRQHandler        ; I2C4 event
    EXTERN  I2C4_EV_IRQHandler
    PUBLIC  I2C4_EV_IRQSize
I2C4_EV_IRQSize:
    LDR     R0, = I2C4_EV_IRQHandler   
    BX      R0     
    ;B I2C4_EV_IRQHandler   
   
//DCD       I2C4_ER_IRQHandler        ; I2C4 error
    EXTERN  I2C4_ER_IRQHandler
    PUBLIC  I2C4_ER_IRQSize
I2C4_ER_IRQSize:
    LDR     R0, = I2C4_ER_IRQHandler      
    BX      R0     
    ;B I2C4_ER_IRQHandler   
   
//DCD       UART6_IRQHandler          ; UART6
    EXTERN  UART6_IRQHandler
    PUBLIC  UART6_IRQSize
UART6_IRQSize:
    LDR     R0, = UART6_IRQHandler   
    BX      R0     
    ;B UART6_IRQHandler   
   
//DCD       UART7_IRQHandler          ; UART7
    EXTERN  UART7_IRQHandler
    PUBLIC  UART7_IRQSize
UART7_IRQSize::
    LDR     R0, = UART7_IRQHandler   
    BX      R0     
    ;B UART7_IRQHandler   
   
//DCD       DMA1_Channel8_IRQHandler  ; DMA1 Channel8
    EXTERN  DMA1_Channel8_IRQHandler
    PUBLIC  DMA1_Channel8_IRQSize
DMA1_Channel8_IRQSize:
    LDR     R0, = DMA1_Channel8_IRQHandler   
    BX      R0     
    ;B DMA1_Channel8_IRQHandler   
   
//DCD       DMA2_Channel8_IRQHandler  ; DMA2 Channel8
    EXTERN  DMA2_Channel8_IRQHandler
    PUBLIC  DMA2_Channel8_IRQSize
DMA2_Channel8_IRQSize:
    LDR     R0, = DMA2_Channel8_IRQHandler   
    BX      R0     
    ;B DMA2_Channel8_IRQHandler   
   
//DCD       DVP_IRQHandler            ; DVP
    EXTERN  DVP_IRQHandler
    PUBLIC  DVP_IRQSize
DVP_IRQSize:
    LDR     R0, = DVP_IRQHandler   
    BX      R0     
    ;B DVP_IRQHandler   
   
//DCD       SAC_IRQHandler            ; SAC
    EXTERN  SAC_IRQHandler
    PUBLIC  SAC_IRQSize
SAC_IRQSize:
    LDR     R0, = SAC_IRQHandler   
    BX      R0     
    ;B SAC_IRQHandler   
   
//DCD       MMU_IRQHandler            ; MMU
    EXTERN  MMU_IRQHandler
    PUBLIC  MMU_IRQSize
MMU_IRQSize:
    LDR     R0, = MMU_IRQHandler   
    BX      R0     
    ;B MMU_IRQHandler   
   
//DCD       TSC_IRQHandler            ; TSC
    EXTERN  TSC_IRQHandler
    PUBLIC  TSC_IRQSize
TSC_IRQSize:
    LDR     R0, = TSC_IRQHandler   
    BX      R0     
    ;B TSC_IRQHandler   
   
//DCD       COMP_1_2_3_IRQHandler     ; COMP1 & COMP2 & COMP3
    EXTERN  COMP_1_2_3_IRQHandler
    PUBLIC  COMP_1_2_3_IRQSize
COMP_1_2_3_IRQSize:
    LDR     R0, = COMP_1_2_3_IRQHandler   
    BX      R0     
    ;B COMP_1_2_3_IRQHandler   
   
//DCD       COMP_4_5_6_IRQHandler     ; COMP4 & COMP5 & COMP6
    EXTERN  COMP_4_5_6_IRQHandler
    PUBLIC  COMP_4_5_6_IRQSize
COMP_4_5_6_IRQSize:
    LDR     R0, = COMP_4_5_6_IRQHandler   
    BX      R0     
    ;B COMP_4_5_6_IRQHandler   
   
//DCD       COMP7_IRQHandler          ; COMP7
    EXTERN  COMP7_IRQHandler
    PUBLIC  COMP7_IRQSize
COMP7_IRQSize:
    LDR     R0, = COMP7_IRQHandler   
    BX      R0     
    ;B COMP7_IRQHandler   
   
//DCD     RSRAM_IRQHandler          ; R-SRAM parity error interrupt
    EXTERN  RSRAM_IRQHandler
    PUBLIC  RSRAM_IRQSize
RSRAM_IRQSize:
    LDR     R0, = RSRAM_IRQHandler 
    BX      R0     
    ;B RSRAM_IRQHandler   
   
//
    
    

    END


    
 
  


    
