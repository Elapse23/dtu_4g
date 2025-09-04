; ---------------------------------------------------------------------------------------
;  @file:    InterruptJmpTable_Keil.s
;  @purpose: Interrupt jump table for bootloader (Keil ARM Assembler syntax)
;  @version: 1.0
;  @date:    2025-09-04
;  @build:   Converted from IAR to Keil syntax
; ---------------------------------------------------------------------------------------        

        PRESERVE8
        THUMB

        AREA    |.text|, CODE, READONLY

        ; Import external interrupt handlers
        IMPORT  NMI_Handler
        IMPORT  HardFault_Handler      
        IMPORT  MemManage_Handler  
        IMPORT  BusFault_Handler 
        IMPORT  UsageFault_Handler
        IMPORT  SVC_Handler
        IMPORT  DebugMon_Handler
        IMPORT  PendSV_Handler
        IMPORT  SysTick_Handler

        ; Export interrupt jump functions
        EXPORT  NMI_Size
        EXPORT  HardFault_Size
        EXPORT  MemManage_Size
        EXPORT  BusFault_Size
        EXPORT  UsageFault_Size
        EXPORT  SVC_Size
        EXPORT  DebugMon_Size
        EXPORT  PendSV_Size
        EXPORT  SysTick_Size

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Core interrupt handlers jump table
;;
NMI_Size        PROC
        LDR     R0, =NMI_Handler   
        BX      R0              
        ENDP
     
HardFault_Size  PROC
        LDR     R0, =HardFault_Handler 
        BX      R0     
        ENDP
      
MemManage_Size  PROC
        LDR     R0, =MemManage_Handler 
        BX      R0     
        ENDP
    
BusFault_Size   PROC
        LDR     R0, =BusFault_Handler 
        BX      R0     
        ENDP

UsageFault_Size PROC
        LDR     R0, =UsageFault_Handler 
        BX      R0     
        ENDP

SVC_Size        PROC
        LDR     R0, =SVC_Handler 
        BX      R0     
        ENDP

DebugMon_Size   PROC
        LDR     R0, =DebugMon_Handler 
        BX      R0     
        ENDP

PendSV_Size     PROC
        LDR     R0, =PendSV_Handler 
        BX      R0     
        ENDP

SysTick_Size    PROC
        LDR     R0, =SysTick_Handler 
        BX      R0     
        ENDP

        END
