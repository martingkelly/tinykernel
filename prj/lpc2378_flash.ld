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
*  18.12.06  mifi   First Version
*  2007-10-01 dmi   Converted for Flash and LPC2378 use
****************************************************************************/


/*ENTRY(ResetHandler)*/
ENTRY(_startup)
SEARCH_DIR(.)

/*
 * Define stack size here
 */
FIQ_STACK_SIZE = 0x0100;
IRQ_STACK_SIZE = 0x0100;
ABT_STACK_SIZE = 0x0100;
UND_STACK_SIZE = 0x0100;
SVC_STACK_SIZE = 0x0400;

/*
 * This file, lpc2378_flash.ld, locate the program in the internal
 * flash of the LPC2378. For more information about the memory of the LPC2378
 * take a look in the user.manual.lpc2364.lpc2366.lpc2368.lpc2378
 * Reference is made to the user manual from 6 July 2007 Rev. 1.05
 *
 * Take a look at Chapter 2, Section 1. Memory maps
 */
 
MEMORY
{
    flash           : ORIGIN = 0,          LENGTH = 0x7E000 /* 516096 FLASH ROM */
    flash_isp       : ORIGIN = 0x0007E000, LENGTH = 0x2000  /* 8192 boot block */

    ram_isp_lo(A)   : ORIGIN = 0x40000040, LENGTH = 0x1C0   /* 448 bytes for variables 
                                                               used by ISP bootloader
                                                               and RealMonitor */
    ram             : ORIGIN = 0x40000200, LENGTH = 0x7CE0  /* 31968 free RAM area */
    ram_isp_stack   : ORIGIN = 0x40007EE0, LENGTH = 0x100   /* 256 bytes used by ISP */
    ram_isp_hi(A)   : ORIGIN = 0x40007FE0, LENGTH = 0x20    /* 32 bytes used by ISP */
}

/* define the bootloader entry address.  This is the first free address in RAM */

/* boot_entry = 0x40000200; */

/* define a global symbol _stack_end a.k.a. first stack entry. stack grows down  */

/*_stack_end = 0x40007EDC;  */              /* 0x40007FFF-0x40007EDC=0x123=291d */


/*
 * Do not change the next code
 */
SECTIONS
{
    . = 0;                                  /* set location counter to address zero  */

    startup : 
    {
        *(.vectors);
        . = ALIGN(4);
        *(.init);
        . = ALIGN(4);
    } > flash                               /* the startup code goes into FLASH */

    .text :
    {
        *(.text);
        . = ALIGN(4);
        *(.rodata);
        . = ALIGN(4);
        *(.rodata*);
        . = ALIGN(4);
        *(.glue_7t);
        . = ALIGN(4);
        *(.glue_7);
        . = ALIGN(4);
        etext = .;
        _etext = .;
    } > flash

    .data :
    {
        PROVIDE (__data_start = .);
        *(.data)
        . = ALIGN(4);
        edata = .;
        _edata = .;
        PROVIDE (__data_end = .);
    } > ram AT > flash

    .eh_frame :
    {
        *(.eh_frame)
        . = ALIGN(4);
    } > flash

    .bss :
    {
        PROVIDE (__bss_start = .);
        *(.bss)
        *(COMMON)
        . = ALIGN(4);
        PROVIDE (__bss_end = .);

        . = ALIGN(256);

        PROVIDE (__stack_start = .);

        PROVIDE (__stack_fiq_start = .);
        . += FIQ_STACK_SIZE;
        . = ALIGN(4);
        PROVIDE (__stack_fiq_end = .);

        PROVIDE (__stack_irq_start = .);
        . += IRQ_STACK_SIZE;
        . = ALIGN(4);
        PROVIDE (__stack_irq_end = .);

        PROVIDE (__stack_abt_start = .);
        . += ABT_STACK_SIZE;
        . = ALIGN(4);
        PROVIDE (__stack_abt_end = .);

        PROVIDE (__stack_und_start = .);
        . += UND_STACK_SIZE;
        . = ALIGN(4);
        PROVIDE (__stack_und_end = .);

        PROVIDE (__stack_svc_start = .);
        . += SVC_STACK_SIZE;
        . = ALIGN(4);
        PROVIDE (__stack_svc_end = .);
        PROVIDE (__stack_end = .);
        PROVIDE (__heap_start = .);   
    } > ram
}

_end = .;                                   /* define a global symbol marking the end of application RAM */
PROVIDE ( end = . );

/*** EOF ***/

