; ****************************************************************************
; Copyright (c) 2019, Nations Technologies Inc.
;
; All rights reserved.
; ****************************************************************************
;
; Redistribution and use in source and binary forms, with or without
; modification, are permitted provided that the following conditions are met:
;
; - Redistributions of source code must retain the above copyright notice,
; this list of conditions and the disclaimer below.
;
; Nations name may not be used to endorse or promote products derived from
; this software without specific prior written permission.
;
; DISCLAIMER: THIS SOFTWARE IS PROVIDED BY NATIONS "AS IS" AND ANY EXPRESS OR
; IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
; MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
; DISCLAIMED. IN NO EVENT SHALL NATIONS BE LIABLE FOR ANY DIRECT, INDIRECT,
; INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
; LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
; OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
; LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
; NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
; EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
; ****************************************************************************

; Amount of memory (in bytes) allocated for Stack
; Tailor this value to your application needs
; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>
  
    MODULE  ?cstartup
        
        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)
        
        EXTERN  xPortPendSVHandler
        EXTERN  vPortSVCHandler
        EXTERN  xPortSysTickHandler
        ;EXTERN  __iar_program_start
        EXTERN cdwDEFAULT_SP
        EXTERN  sResetEntry
        EXTERN  __cmain
        EXTERN  Reset_Handler     
        EXTERN  sClearUserRam
        EXTERN  sClearReverseRam

        ; //PUBLIC  cdwRAM_BOOT_REVERSE_ADDR
        ; //PUBLIC  cdwRAM_END_ADDR
        
        PUBLIC  __vector_table
        EXTERN  sResetSP

        EXTERN  sAsmTest
        EXTERN  PendSV_Size
        EXTERN  NMI_Size
        EXTERN  HardFault_Size
        EXTERN  MemManage_Size  
        EXTERN  BusFault_Size
        EXTERN  UsageFault_Size
        EXTERN  SVC_Size
        EXTERN  DebugMon_Size
        EXTERN  PendSV_Size
        EXTERN  SysTick_Size 
        EXTERN  WWDG_IRQSize 
        EXTERN  UART5_IRQSize
         ; External Interrupts
        EXTERN  WWDG_IRQSize 
        EXTERN  PVD_IRQSize 
        EXTERN  TAMPER_IRQSize 
        EXTERN  RTC_IRQSize 
        EXTERN  FLASH_IRQSize 
        EXTERN  RCC_IRQSize 
        EXTERN  EXTI0_IRQSize 
        EXTERN  EXTI1_IRQSize 
        EXTERN  EXTI2_IRQSize 
        EXTERN  EXTI3_IRQSize 
        EXTERN  EXTI4_IRQSize 
        EXTERN  DMA1_Channel1_IRQSize 
        EXTERN  DMA1_Channel2_IRQSize 
        EXTERN  DMA1_Channel3_IRQSize 
        EXTERN  DMA1_Channel4_IRQSize 
        EXTERN  DMA1_Channel5_IRQSize 
        EXTERN  DMA1_Channel6_IRQSize 
        EXTERN  DMA1_Channel7_IRQSize 
        EXTERN  ADC1_2_IRQSize 
        EXTERN  USB_HP_CAN1_TX_IRQSize 
        EXTERN  USB_LP_CAN1_RX0_IRQSize 
        EXTERN  CAN1_RX1_IRQSize 
        EXTERN  CAN1_SCE_IRQSize 
        EXTERN  EXTI9_5_IRQSize 
        EXTERN  TIM1_BRK_IRQSize 
        EXTERN  TIM1_UP_IRQSize 
        EXTERN  TIM1_TRG_COM_IRQSize 
        EXTERN  TIM1_CC_IRQSize 
        EXTERN  TIM2_IRQSize 
        EXTERN  TIM3_IRQSize 
        EXTERN  TIM4_IRQSize 
        EXTERN  I2C1_EV_IRQSize 
        EXTERN  I2C1_ER_IRQSize 
        EXTERN  I2C2_EV_IRQSize 
        EXTERN  I2C2_ER_IRQSize 
        EXTERN  SPI1_IRQSize 
        EXTERN  SPI2_IRQSize 
        EXTERN  USART1_IRQSize 
        EXTERN  USART2_IRQSize 
        EXTERN  USART3_IRQSize 
        EXTERN  EXTI15_10_IRQSize 
        EXTERN  RTCAlarm_IRQSize 
        EXTERN  USBWakeUp_IRQSize 
        EXTERN  TIM8_BRK_IRQSize 
        EXTERN  TIM8_UP_IRQSize 
        EXTERN  TIM8_TRG_COM_IRQSize 
        EXTERN  TIM8_CC_IRQSize 
        EXTERN  ADC3_4_IRQSize 
        EXTERN  XFMC_IRQSize 
        EXTERN  SDIO_IRQSize 
        EXTERN  TIM5_IRQSize 
        EXTERN  SPI3_IRQSize 
        EXTERN  UART4_IRQSize 
        EXTERN  UART5_IRSize 
        EXTERN  TIM6_IRQSize 
        EXTERN  TIM7_IRQSize 
        EXTERN  DMA2_Channel1_IRQSize 
        EXTERN  DMA2_Channel2_IRQSize 
        EXTERN  DMA2_Channel3_IRQSize 
        EXTERN  DMA2_Channel4_IRQSize 
        EXTERN  DMA2_Channel5_IRQSize 
        EXTERN  ETH_IRQSize 
        EXTERN  ETH_WKUP_IRQSize 
        EXTERN  CAN2_TX_IRQSize 
        EXTERN  CAN2_RX0_IRQSize 
        EXTERN  CAN2_RX1_IRQSize 
        EXTERN  CAN2_SCE_IRQSize 
        EXTERN  QSPI_IRQSize 
        EXTERN  DMA2_Channel6_IRQSize 
        EXTERN  DMA2_Channel7_IRQSize 
        EXTERN  I2C3_EV_IRQSize 
        EXTERN  I2C3_ER_IRQSize 
        EXTERN  I2C4_EV_IRQSize 
        EXTERN  I2C4_ER_IRQSize 
        EXTERN  UART6_IRQSize 
        EXTERN  UART7_IRQSize 
        EXTERN  DMA1_Channel8_IRQSize 
        EXTERN  DMA2_Channel8_IRQSize 
        EXTERN   DVP_IRQSize 
        EXTERN  SAC_IRQSize 
        EXTERN  MMU_IRQSize 
        EXTERN  TSC_IRQSize 
        EXTERN  COMP_1_2_3_IRQSize 
        EXTERN  COMP_4_5_6_IRQSize 
        EXTERN  COMP7_IRQSize 
        EXTERN  RSRAM_IRQSize 
        PUBLIC  sDoIarcmain
        PUBLIC  sAppOkMark
        PUBLIC  PendSV_Handler

        DATA
       
__vector_table
        DCD     cdwDEFAULT_SP
        DCD     Reset_Handler               ; Reset Handler
        DCD     NMI_Size                 ; NMI Handler
        DCD     HardFault_Size           ; Hard Fault Handler
        DCD     MemManage_Size           ; MPU Fault Handler
        DCD     BusFault_Size            ; Bus Fault Handler
        DCD     UsageFault_Size          ; Usage Fault Handler
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     SVC_Size                 ; SVCall Handler
        DCD     DebugMon_Size          ; Debug Monitor Handler
        DCD     0                           ; Reserved
        DCD     PendSV_Size                ; PendSV Handler
        DCD     SysTick_Size              ; SysTick Handler

         ; External Interrupts
        DCD     WWDG_IRQSize           ; Window Watchdog
        DCD     PVD_IRQSize            ; PVD through EXTI Line detect
        DCD     TAMPER_IRQSize         ; Tamper
        DCD     RTC_IRQSize            ; RTC
        DCD     FLASH_IRQSize          ; Flash
        DCD     RCC_IRQSize            ; RCC
        DCD     EXTI0_IRQSize          ; EXTI Line 0
        DCD     EXTI1_IRQSize          ; EXTI Line 1
        DCD     EXTI2_IRQSize          ; EXTI Line 2
        DCD     EXTI3_IRQSize          ; EXTI Line 3
        DCD     EXTI4_IRQSize          ; EXTI Line 4
        DCD     DMA1_Channel1_IRQSize  ; DMA1 Channel 1
        DCD     DMA1_Channel2_IRQSize  ; DMA1 Channel 2
        DCD     DMA1_Channel3_IRQSize  ; DMA1 Channel 3
        DCD     DMA1_Channel4_IRQSize  ; DMA1 Channel 4
        DCD     DMA1_Channel5_IRQSize  ; DMA1 Channel 5
        DCD     DMA1_Channel6_IRQSize  ; DMA1 Channel 6
        DCD     DMA1_Channel7_IRQSize  ; DMA1 Channel 7
        DCD     ADC1_2_IRQSize         ; ADC1 & ADC2
        DCD     USB_HP_CAN1_TX_IRQSize  ; USB High Priority or CAN1 TX
        DCD     USB_LP_CAN1_RX0_IRQSize ; USB Low  Priority or CAN1 RX0
        DCD     CAN1_RX1_IRQSize       ; CAN1 RX1
        DCD     CAN1_SCE_IRQSize       ; CAN1 SCE
        DCD     EXTI9_5_IRQSize        ; EXTI Line 9..5
        DCD     TIM1_BRK_IRQSize       ; TIM1 Break
        DCD     TIM1_UP_IRQSize        ; TIM1 Update
        DCD     TIM1_TRG_COM_IRQSize   ; TIM1 Trigger and Commutation
        DCD     TIM1_CC_IRQSize        ; TIM1 Capture Compare
        DCD     TIM2_IRQSize          ; TIM2
        DCD     TIM3_IRQSize           ; TIM3
        DCD     TIM4_IRQSize           ; TIM4
        DCD     I2C1_EV_IRQSize        ; I2C1 Event
        DCD     I2C1_ER_IRQSize        ; I2C1 Error
        DCD     I2C2_EV_IRQSize        ; I2C2 Event
        DCD     I2C2_ER_IRQSize        ; I2C2 Error
        DCD     SPI1_IRQSize           ; SPI1
        DCD     SPI2_IRQSize           ; SPI2
        DCD     USART1_IRQSize         ; USART1
        DCD     USART2_IRQSize        ; USART2
        DCD     USART3_IRQSize         ; USART3
        DCD     EXTI15_10_IRQSize      ; EXTI Line 15..10
        DCD     RTCAlarm_IRQSize      ; RTC Alarm through EXTI Line
        DCD     USBWakeUp_IRQSize      ; USB Wakeup from suspend
        DCD     TIM8_BRK_IRQSize       ; TIM8 Break
        DCD     TIM8_UP_IRQSize        ; TIM8 Update
        DCD     TIM8_TRG_COM_IRQSize   ; TIM8 Trigger and Commutation
        DCD     TIM8_CC_IRQSize        ; TIM8 Capture Compare
        DCD     ADC3_4_IRQSize         ; ADC3 & ADC4
        DCD     XFMC_IRQSize          ; XFMC
        DCD     SDIO_IRQSize           ; SDIO
        DCD     TIM5_IRQSize           ; TIM5
        DCD     SPI3_IRQSize           ; SPI3
        DCD     UART4_IRQSize          ; UART4
        DCD     UART5_IRQSize             ; UART5
        DCD     TIM6_IRQSize           ; TIM6
        DCD     TIM7_IRQSize           ; TIM7
        DCD     DMA2_Channel1_IRQSize  ; DMA2 Channel1
        DCD     DMA2_Channel2_IRQSize  ; DMA2 Channel2
        DCD     DMA2_Channel3_IRQSize  ; DMA2 Channel3
        DCD     DMA2_Channel4_IRQSize  ; DMA2 Channel4
        DCD     DMA2_Channel5_IRQSize  ; DMA2 Channel5
        DCD     ETH_IRQSize           ; Ethernet global interrupt
        DCD     ETH_WKUP_IRQSize      ; Ethernet Wakeup through EXTI line interrupt
        DCD     CAN2_TX_IRQSize        ; CAN2 TX
        DCD     CAN2_RX0_IRQSize       ; CAN2 RX0
        DCD     CAN2_RX1_IRQSize       ; CAN2 RX1
        DCD     CAN2_SCE_IRQSize       ; CAN2 SCE
        DCD     QSPI_IRQSize           ; QSPI
        DCD     DMA2_Channel6_IRQSize  ; DMA2 Channel6
        DCD     DMA2_Channel7_IRQSize  ; DMA2 Channel7
        DCD     I2C3_EV_IRQSize        ; I2C3 event
        DCD     I2C3_ER_IRQSize        ; I2C3 error
        DCD     I2C4_EV_IRQSize        ; I2C4 event
        DCD     I2C4_ER_IRQSize        ; I2C4 error
        DCD     UART6_IRQSize          ; UART6
        DCD     UART7_IRQSize          ; UART7
        DCD     DMA1_Channel8_IRQSize  ; DMA1 Channel8
        DCD     DMA2_Channel8_IRQSize  ; DMA2 Channel8
        DCD     DVP_IRQSize            ; DVP
        DCD     SAC_IRQSize            ; SAC
        DCD     MMU_IRQSize            ; MMU
        DCD     TSC_IRQSize           ; TSC
        DCD     COMP_1_2_3_IRQSize     ; COMP1 & COMP2 & COMP3
        DCD     COMP_4_5_6_IRQSize    ; COMP4 & COMP5 & COMP6
        DCD     COMP7_IRQSize          ; COMP7
        DCD     RSRAM_IRQSize          ; R-SRAM parity error interrupt
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
    SECTION ApplicationMark:CODE:NOROOT(2)
    DATA
sAppOkMark:
    DCD     0x88888889
	
    SECTION .approm:CODE:REORDER:NOROOT(2)        
        THUMB


               
sDoIarcmain:
    CPSID   I
	LDR     R0, =__cmain        ; fix iar __cmain function address								  
    BX      R0                  ; __cmain will jump to main()
        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
        B HardFault_Handler

        PUBWEAK MemManage_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
MemManage_Handler
        B MemManage_Handler

        PUBWEAK BusFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BusFault_Handler
        B BusFault_Handler

        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UsageFault_Handler
        B UsageFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B vPortSVCHandler					  

        PUBWEAK DebugMon_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DebugMon_Handler
        B DebugMon_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B xPortPendSVHandler					  
        
        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B xPortSysTickHandler					  

        PUBWEAK WWDG_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
WWDG_IRQHandler
        B WWDG_IRQHandler

        PUBWEAK PVD_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PVD_IRQHandler
        B PVD_IRQHandler

        PUBWEAK TAMPER_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TAMPER_IRQHandler
        B TAMPER_IRQHandler

        PUBWEAK RTC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RTC_IRQHandler
        B RTC_IRQHandler

        PUBWEAK FLASH_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
FLASH_IRQHandler
        B FLASH_IRQHandler

        PUBWEAK RCC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RCC_IRQHandler
        B RCC_IRQHandler

        PUBWEAK EXTI0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI0_IRQHandler
        B EXTI0_IRQHandler

        PUBWEAK EXTI1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI1_IRQHandler
        B EXTI1_IRQHandler

        PUBWEAK EXTI2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI2_IRQHandler
        B EXTI2_IRQHandler

        PUBWEAK EXTI3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI3_IRQHandler
        B EXTI3_IRQHandler

        PUBWEAK EXTI4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI4_IRQHandler
        B EXTI4_IRQHandler

        PUBWEAK DMA1_Channel1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA1_Channel1_IRQHandler
        B DMA1_Channel1_IRQHandler

        PUBWEAK DMA1_Channel2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA1_Channel2_IRQHandler
        B DMA1_Channel2_IRQHandler

        PUBWEAK DMA1_Channel3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA1_Channel3_IRQHandler
        B DMA1_Channel3_IRQHandler

        PUBWEAK DMA1_Channel4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA1_Channel4_IRQHandler
        B DMA1_Channel4_IRQHandler

        PUBWEAK DMA1_Channel5_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA1_Channel5_IRQHandler
        B DMA1_Channel5_IRQHandler

        PUBWEAK DMA1_Channel6_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA1_Channel6_IRQHandler
        B DMA1_Channel6_IRQHandler

        PUBWEAK DMA1_Channel7_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA1_Channel7_IRQHandler
        B DMA1_Channel7_IRQHandler

        PUBWEAK ADC1_2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
ADC1_2_IRQHandler
        B ADC1_2_IRQHandler

        PUBWEAK USB_HP_CAN1_TX_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
USB_HP_CAN1_TX_IRQHandler
        B USB_HP_CAN1_TX_IRQHandler

        PUBWEAK USB_LP_CAN1_RX0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
USB_LP_CAN1_RX0_IRQHandler
        B USB_LP_CAN1_RX0_IRQHandler

        PUBWEAK CAN1_RX1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
CAN1_RX1_IRQHandler
        B CAN1_RX1_IRQHandler

        PUBWEAK CAN1_SCE_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
CAN1_SCE_IRQHandler
        B CAN1_SCE_IRQHandler

        PUBWEAK EXTI9_5_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI9_5_IRQHandler
        B EXTI9_5_IRQHandler

        PUBWEAK TIM1_BRK_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIM1_BRK_IRQHandler
        B TIM1_BRK_IRQHandler

        PUBWEAK TIM1_UP_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIM1_UP_IRQHandler
        B TIM1_UP_IRQHandler

        PUBWEAK TIM1_TRG_COM_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIM1_TRG_COM_IRQHandler
        B TIM1_TRG_COM_IRQHandler

        PUBWEAK TIM1_CC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIM1_CC_IRQHandler
        B TIM1_CC_IRQHandler

        PUBWEAK TIM2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIM2_IRQHandler
        B TIM2_IRQHandler

        PUBWEAK TIM3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIM3_IRQHandler
        B TIM3_IRQHandler

        PUBWEAK TIM4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIM4_IRQHandler
        B TIM4_IRQHandler

        PUBWEAK I2C1_EV_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C1_EV_IRQHandler
        B I2C1_EV_IRQHandler

        PUBWEAK I2C1_ER_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C1_ER_IRQHandler
        B I2C1_ER_IRQHandler

        PUBWEAK I2C2_EV_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C2_EV_IRQHandler
        B I2C2_EV_IRQHandler

        PUBWEAK I2C2_ER_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C2_ER_IRQHandler
        B I2C2_ER_IRQHandler

        PUBWEAK SPI1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPI1_IRQHandler
        B SPI1_IRQHandler

        PUBWEAK SPI2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPI2_IRQHandler
        B SPI2_IRQHandler

        PUBWEAK USART1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
USART1_IRQHandler
        B USART1_IRQHandler

        PUBWEAK USART2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
USART2_IRQHandler
        B USART2_IRQHandler

        PUBWEAK USART3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
USART3_IRQHandler
        B USART3_IRQHandler

        PUBWEAK EXTI15_10_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI15_10_IRQHandler
        B EXTI15_10_IRQHandler

        PUBWEAK RTCAlarm_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RTCAlarm_IRQHandler
        B RTCAlarm_IRQHandler

        PUBWEAK USBWakeUp_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
USBWakeUp_IRQHandler
        B USBWakeUp_IRQHandler

        PUBWEAK TIM8_BRK_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIM8_BRK_IRQHandler
        B TIM8_BRK_IRQHandler

        PUBWEAK TIM8_UP_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIM8_UP_IRQHandler
        B TIM8_UP_IRQHandler

        PUBWEAK TIM8_TRG_COM_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIM8_TRG_COM_IRQHandler
        B TIM8_TRG_COM_IRQHandler

        PUBWEAK TIM8_CC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIM8_CC_IRQHandler
        B TIM8_CC_IRQHandler

        PUBWEAK ADC3_4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
ADC3_4_IRQHandler
        B ADC3_4_IRQHandler

        PUBWEAK XFMC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
XFMC_IRQHandler
        B XFMC_IRQHandler

        PUBWEAK SDIO_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SDIO_IRQHandler
        B SDIO_IRQHandler

        PUBWEAK TIM5_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIM5_IRQHandler
        B TIM5_IRQHandler

        PUBWEAK SPI3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPI3_IRQHandler
        B SPI3_IRQHandler

        PUBWEAK UART4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART4_IRQHandler
        B UART4_IRQHandler

        PUBWEAK UART5_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART5_IRQHandler
        B UART5_IRQHandler

        PUBWEAK TIM6_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIM6_IRQHandler
        B TIM6_IRQHandler

        PUBWEAK TIM7_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIM7_IRQHandler
        B TIM7_IRQHandler

        PUBWEAK DMA2_Channel1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA2_Channel1_IRQHandler
        B DMA2_Channel1_IRQHandler

        PUBWEAK DMA2_Channel2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA2_Channel2_IRQHandler
        B DMA2_Channel2_IRQHandler

        PUBWEAK DMA2_Channel3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA2_Channel3_IRQHandler
        B DMA2_Channel3_IRQHandler

        PUBWEAK DMA2_Channel4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA2_Channel4_IRQHandler
        B DMA2_Channel4_IRQHandler

        PUBWEAK DMA2_Channel5_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA2_Channel5_IRQHandler
        B DMA2_Channel5_IRQHandler

        PUBWEAK ETH_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
ETH_IRQHandler
        B ETH_IRQHandler

        PUBWEAK ETH_WKUP_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
ETH_WKUP_IRQHandler
        B ETH_WKUP_IRQHandler

        PUBWEAK CAN2_TX_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
CAN2_TX_IRQHandler
        B CAN2_TX_IRQHandler

        PUBWEAK CAN2_RX0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
CAN2_RX0_IRQHandler
        B CAN2_RX0_IRQHandler

        PUBWEAK CAN2_RX1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
CAN2_RX1_IRQHandler
        B CAN2_RX1_IRQHandler

        PUBWEAK CAN2_SCE_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
CAN2_SCE_IRQHandler
        B CAN2_SCE_IRQHandler

        PUBWEAK QSPI_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
QSPI_IRQHandler
        B QSPI_IRQHandler

        PUBWEAK DMA2_Channel6_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA2_Channel6_IRQHandler
        B DMA2_Channel6_IRQHandler

        PUBWEAK DMA2_Channel7_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA2_Channel7_IRQHandler
        B DMA2_Channel7_IRQHandler

        PUBWEAK I2C3_EV_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C3_EV_IRQHandler
        B I2C3_EV_IRQHandler

        PUBWEAK I2C3_ER_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C3_ER_IRQHandler
        B I2C3_ER_IRQHandler

        PUBWEAK I2C4_EV_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C4_EV_IRQHandler
        B I2C4_EV_IRQHandler

        PUBWEAK I2C4_ER_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C4_ER_IRQHandler
        B I2C4_ER_IRQHandler

        PUBWEAK UART6_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART6_IRQHandler
        B UART6_IRQHandler

        PUBWEAK UART7_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART7_IRQHandler
        B UART7_IRQHandler

        PUBWEAK DMA1_Channel8_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA1_Channel8_IRQHandler
        B DMA1_Channel8_IRQHandler

        PUBWEAK DMA2_Channel8_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA2_Channel8_IRQHandler
        B DMA2_Channel8_IRQHandler

        PUBWEAK DVP_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DVP_IRQHandler
        B DVP_IRQHandler

        PUBWEAK SAC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SAC_IRQHandler
        B SAC_IRQHandler

        PUBWEAK MMU_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
MMU_IRQHandler
        B MMU_IRQHandler

        PUBWEAK TSC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TSC_IRQHandler
        B TSC_IRQHandler

        PUBWEAK COMP_1_2_3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
COMP_1_2_3_IRQHandler
        B COMP_1_2_3_IRQHandler

        PUBWEAK COMP_4_5_6_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
COMP_4_5_6_IRQHandler
        B COMP_4_5_6_IRQHandler

        PUBWEAK COMP7_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
COMP7_IRQHandler
        B COMP7_IRQHandler

        PUBWEAK RSRAM_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RSRAM_IRQHandler
        B RSRAM_IRQHandler
        

        END

