; ---------------------------------------------------------------------------------------
;  @file:    BootloaderAsmFuncs.asm
;  @purpose: asm functions for bootloader
;  @version: 1.0
;  @date:    2018-6-21
;  @build:   
; ---------------------------------------------------------------------------------------        


; ---------------------------------------------------------------------------------------
;  RAM area
; ---------------------------------------------------------------------------------------
cdwRAM_START_ADDR               EQU     0x20000000
cdwRAM_BOOT_REVERSE_ADDR        EQU     0x20000200
cdwRAM_END_ADDR                 EQU     0x20023FFF
cdwDEFAULT_SP                   EQU     cdwRAM_END_ADDR-3


        
        EXTERN  __vector_table
        EXTERN  sResetEntry
        EXTERN  BootSystemInit


        SECTION .resetrom:CODE:NOROOT(2)
        PUBLIC  cdwDEFAULT_SP         
        PUBLIC  cdwRAM_BOOT_REVERSE_ADDR
        PUBLIC  cdwRAM_END_ADDR
 
        PUBLIC  sResetSP
        PUBLIC  sClearUserRam
        PUBLIC  sClearReverseRam
        PUBLIC  Reset_Handler
        PUBLIC  sAsmTest


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB


        SECTION .text:CODE:REORDER:NOROOT(2)
;Reset_Handler
;        LDR     R0, = sBspSystemAndRccInit
;        BLX     R0
Reset_Handler
        CPSID   I               ; Mask interrupts//readonly section  .iar.init_table,
        LDR     R0,=0           ; Initialize the GPRs
        LDR     R1,=0
        LDR     R2,=0
        LDR     R3,=0
        LDR     R4,=0
        LDR     R5,=0
        LDR     R6,=0
        LDR     R7,=0
        
        LDR     R0, =cdwDEFAULT_SP ;initial VTOR 
        LDR     R1, =__vector_table
        STR     R1, [R0]
        LDR     R2, [R1]
        
        MSR     MSP, R2         ;initial MSP to CSTACK
        
        LDR     R0, = BootSystemInit
        BLX     R0
        LDR     R0, = sResetEntry
        BX      R0       
;ResetEntry                      ;jump to bootloader reset entry
;        LDR     R0, = SystemInit
;        BLX     R0
;        LDR     R0, = sResetEntry
;        BX      R0              ;run reset entry jump controler

sResetSP
        LDR     R0, =cdwDEFAULT_SP ;initial VTOR 
        LDR     R1, =__vector_table
        STR     R1, [R0]
        LDR     R2, [R1]
         
        MSR     MSP, R2         ;initial MSP to CSTACK

        BX      LR              ;return

sClearUserRam
        LDR     R0, =cdwRAM_BOOT_REVERSE_ADDR
        LDR     R1, =cdwRAM_END_ADDR
        LDR     R2, =0
        CLR_RAM_LOOP:
        STR     R2, [R0]
        ADDS    R0,R0,#+4
        CMP     R0, R1
        BLO     CLR_RAM_LOOP
        BX      LR              ;return
sClearReverseRam
        LDR     R0, =cdwRAM_START_ADDR
        LDR     R1, =cdwRAM_BOOT_REVERSE_ADDR
        LDR     R2, =0
        CLR_RAM_LOOP2:
        STR     R2, [R0]
        ADDS    R0,R0,#+4
        CMP     R0, R1
        BLO     CLR_RAM_LOOP2
        BX      LR              ;return
        

sAsmTest
        MOVW    R0, #0x819A
        MOVT    R0, #0x1FFF
        LDR.W   R0, [R0]
        BX      LR
        END
        



