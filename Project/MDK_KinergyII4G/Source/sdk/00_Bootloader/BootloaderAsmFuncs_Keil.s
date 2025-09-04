; ---------------------------------------------------------------------------------------
;  @file:    BootloaderAsmFuncs_Keil.s
;  @purpose: asm functions for bootloader (Keil ARM Assembler syntax)
;  @version: 1.0
;  @date:    2025-09-04
;  @build:   Converted from IAR to Keil syntax
; ---------------------------------------------------------------------------------------        

; ---------------------------------------------------------------------------------------
;  RAM area
; ---------------------------------------------------------------------------------------
cdwRAM_START_ADDR               EQU     0x20000000
cdwRAM_BOOT_REVERSE_ADDR        EQU     0x20000200
cdwRAM_END_ADDR                 EQU     0x20023FFF
cdwDEFAULT_SP                   EQU     cdwRAM_END_ADDR-3

        PRESERVE8
        THUMB

        AREA    |.text|, CODE, READONLY

        ; External symbols
        IMPORT  __Vectors             ; 修改为正确的向量表符号
        IMPORT  sResetEntry
        IMPORT  BootSystemInit

        ; Exported symbols
        EXPORT  cdwDEFAULT_SP         
        EXPORT  cdwRAM_BOOT_REVERSE_ADDR
        EXPORT  cdwRAM_END_ADDR
        EXPORT  sResetSP
        EXPORT  sClearUserRam
        EXPORT  sClearReverseRam
        EXPORT  Reset_Handler
        EXPORT  sAsmTest

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Reset Handler
;;
Reset_Handler   PROC
        CPSID   I               ; Mask interrupts
        
        LDR     R0,=0           ; Initialize the GPRs
        LDR     R1,=0
        LDR     R2,=0
        LDR     R3,=0
        LDR     R4,=0
        LDR     R5,=0
        LDR     R6,=0
        LDR     R7,=0
        MOV     R8,R0
        MOV     R9,R0
        MOV     R10,R0
        MOV     R11,R0
        MOV     R12,R0

        ; Initialize stack pointer
        LDR     R0, =cdwDEFAULT_SP
        MOV     SP, R0

        ; Call bootloader reset entry
        BL      sResetEntry
        
        ; Should never reach here
        B       .
        ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Reset Stack Pointer
;;
sResetSP        PROC
        LDR     R0, =cdwDEFAULT_SP
        MOV     SP, R0
        BX      LR
        ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Clear User RAM
;;
sClearUserRam   PROC
        LDR     R0, =cdwRAM_START_ADDR
        LDR     R1, =cdwRAM_BOOT_REVERSE_ADDR
        LDR     R2, =0
clear_user_loop
        CMP     R0, R1
        BGE     clear_user_done
        STR     R2, [R0]
        ADD     R0, R0, #4
        B       clear_user_loop
clear_user_done
        BX      LR
        ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Clear Reverse RAM
;;
sClearReverseRam PROC
        LDR     R0, =cdwRAM_BOOT_REVERSE_ADDR
        LDR     R1, =cdwRAM_END_ADDR
        LDR     R2, =0
clear_reverse_loop
        CMP     R0, R1
        BGE     clear_reverse_done
        STR     R2, [R0]
        ADD     R0, R0, #4
        B       clear_reverse_loop
clear_reverse_done
        BX      LR
        ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Assembly Test Function
;;
sAsmTest        PROC
        LDR     R0, =0x12345678
        BX      LR
        ENDP

        END
