/****************************************************************************
*  Copyright (c) 2006 by Michael Fischer. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*  
*  1. Redistributions of source code must retain the above copyright 
*     notice, this list of conditions and the following disclaimer.
*  2. Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the 
*     documentation and/or other materials provided with the distribution.
*  3. Neither the name of the author nor the names of its contributors may 
*     be used to endorse or promote products derived from this software 
*     without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
*  THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
*  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
*  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
*  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF 
*  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
*  SUCH DAMAGE.
*
****************************************************************************
*
*  History:
*
*  31.03.06  mifi   First Version
*                   This version based on an example from Ethernut and
*                   "ARM Cross Development with Eclipse" from James P. Lynch
****************************************************************************/

/*
 * Some defines for the program status registers
 */

ARM_MODE_USER  = 0x10      /* Normal User Mode                             */
ARM_MODE_FIQ   = 0x11      /* FIQ Fast Interrupts Mode                     */
ARM_MODE_IRQ   = 0x12      /* IRQ Standard Interrupts Mode                 */
ARM_MODE_SVC   = 0x13      /* Supervisor Interrupts Mode                   */
ARM_MODE_ABORT = 0x17      /* Abort Processing memory Faults Mode          */
ARM_MODE_UNDEF = 0x1B      /* Undefined Instructions Mode                  */
ARM_MODE_SYS   = 0x1F      /* System Running in Priviledged Operating Mode */
ARM_MODE_MASK  = 0x1F
   
I_BIT          = 0x80      /* disable IRQ when I bit is set */
F_BIT          = 0x40      /* disable FIQ when F bit is set */
   
/*
 * Imports and exports
 */

.extern main
.extern TKContextSwitchISR
.global _startup           /* the linker wants this symbol   */

/*
 * Startup symbol to satisfy the linker.
 */

.code 32
.section .vectors,"ax"

_startup:
        
/****************************************************************************/
/*               Vector table and reset entry                               */
/****************************************************************************/
_vectors:
    ldr pc, ResetAddr           /* Reset                 */
    ldr pc, UndefAddr           /* Undefined instruction */
    ldr pc, SWIAddr             /* Software interrupt    */
    ldr pc, PAbortAddr          /* Prefetch abort        */
    ldr pc, DAbortAddr          /* Data abort            */
    ldr pc, ReservedAddr        /* Reserved              */
    ldr pc, TKContextSwitchAddr /* IRQ interrupt         */
    ldr pc, FIQAddr             /* FIQ interrupt         */

/*
 * Literal pool for the addresses
 */
ResetAddr:           .word ResetHandler
UndefAddr:           .word UndefHandler
SWIAddr:             .word SWIHandler
PAbortAddr:          .word PAbortHandler
DAbortAddr:          .word DAbortHandler
ReservedAddr:        .word 0
TKContextSwitchAddr: .word TKContextSwitchISR
FIQAddr:             .word FIQHandler

.ltorg

.code 32
.section .init, "ax"
   
/****************************************************************************/
/*                           Reset handler                                  */
/****************************************************************************/
ResetHandler:
/*
 * Wait until the oscillator is stable.
 */   
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
   
/*
 * Setup the stack for each mode.
 */
    msr   CPSR_c, #ARM_MODE_UNDEF | I_BIT | F_BIT   /* Undefined Instruction Mode */
    ldr   sp, =__stack_und_end
   
    msr   CPSR_c, #ARM_MODE_ABORT | I_BIT | F_BIT   /* Abort Mode */
    ldr   sp, =__stack_abt_end
   
    msr   CPSR_c, #ARM_MODE_FIQ | I_BIT | F_BIT     /* FIQ Mode */
    ldr   sp, =__stack_fiq_end
   
    msr   CPSR_c, #ARM_MODE_IRQ | I_BIT | F_BIT     /* IRQ Mode */
    ldr   sp, =__stack_irq_end
   
    msr   CPSR_c, #ARM_MODE_SVC | I_BIT | F_BIT     /* Supervisor Mode */
    ldr   sp, =__stack_svc_end

/* 
 * Copy .data section from Flash to RAM.
 */
    ldr     r1, =_etext
    ldr     r2, =__data_start
    ldr     r3, =__data_end
copyloop:   
    cmp     r2, r3
    ldrlo   r0, [r1], #4
    strlo   r0, [r2], #4
    blo     copyloop

/*
 * Zero out the .bss section
 */
    ldr   r1, =__bss_start
    ldr   r2, =__bss_end
    ldr   r3, =0
bss_clear_loop:
    cmp   r1, r2
    strne r3, [r1], #+4
    bne   bss_clear_loop
   
/*
 * Turn on interrupts and jump to main.
 */
    mrs   r0, cpsr
    bic   r0, r0, #I_BIT | F_BIT   /*   Enable FIQ and IRQ interrupt */
    /*bic   r0, r0, #I_BIT        Enable IRQ interrupt only */
    msr   cpsr, r0
   
    ldr   r2, =main            /* Jump to main with no arguments */
    mov   lr, pc
    bx    r2
                       
ExitFunction:
    b ExitFunction   
   

/****************************************************************************/
/*                         Default interrupt handlers                       */
/****************************************************************************/

UndefHandler:
    b UndefHandler
   
SWIHandler:
    b SWIHandler

PAbortHandler:
    b PAbortHandler

DAbortHandler:
    b DAbortHandler

IRQHandler:
    b IRQHandler
   
FIQHandler:
    b FIQHandler

   
.weak ExitFunction
.weak UndefHandler, PAbortHandler, DAbortHandler
.weak IRQHandler, FIQHandler

.ltorg

/*** EOF ***/   
   

