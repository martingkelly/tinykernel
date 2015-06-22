#ifndef __UARTS_H__
#define __UARTS_H__

#include "lpc/lpc2378.h"

/*
 * UART0
 */ 

#define U0RBR          0xE000C000
#define U0THR          0xE000C000
#define U0DLL          0xE000C000
#define U0DLM          0xE000C004
#define U0IER          0xE000C004
#define U0IIR          0xE000C008
#define U0FCR          0xE000C008
#define U0LCR          0xE000C00C
#define U0LSR          0xE000C014
#define U0SCR          0xE000C01C
#define U0ACR          0xE000C020
#define U0ICR          0xE000C024
#define U0FDR          0xE000C028
#define U0TER          0xE000C030


/* http://gcc.gnu.org/onlinedocs/gcc/Type-Attributes.html*/
#define ATTR_PACKED           __attribute__ ((__packed__))

typedef volatile struct __attribute__ ((__packed__,aligned(4))) _LPC23XX_UART {
    union ATTR_PACKED {         /* Offset 0x00 */
        uint8_t RBR;            /* RO */
        uint8_t THR;            /* WO */
        uint8_t DLL;            /* RW if DLAB = 1 */
    };
    uint8_t   pad0[3];
    union ATTR_PACKED {         /* Offset 0x04 */
        struct ATTR_PACKED {
            uint8_t DLM;
            uint8_t pad1[3];
        };
        uint32_t IER;
    };
    union ATTR_PACKED {         /* Offset 0x08 */
        uint32_t IIR;
        struct ATTR_PACKED {
            uint8_t FCR;
            uint8_t pad2[3];
        };
    };
    uint8_t   LCR;              /* Offset 0x0C */
    uint8_t   pad3[3];
    uint8_t   pad4[4];
    uint8_t   LSR;              /* Offset 0x14 */
    uint8_t   pad5[3];
    uint8_t   MSR;              /* Offset 0x18 */
    uint8_t   pad6[3];
    uint8_t   SCR;              /* Offset 0x1C */
    uint8_t   pad7[3];
    uint32_t  ACR;              /* Offset 0x20 */
    uint8_t   ICR;              /* Offset 0x24 */
    uint8_t   pad8[3];
    uint8_t   FDR;              /* Offset 0x28 */
    uint8_t   pad9[3];
    uint32_t  pad10;            /* Offset 0x2C */
    uint8_t   TER;              /* Offset 0x30 */
    uint8_t   pad11[3];
}  LPC23XX_UART, *P_LPC23XX_UART;

#define PCONP (0xE01FC0C4)
#define PCUART0 (1UL<<3)

#define P_UART0_REGS ((LPC23XX_UART *)0xE000C000)

#define U0_TX_PINSEL_REG   PINSEL0
#define U0_TX_PINSEL       (1UL<<4)              
#define U0_TX_PINMASK      (3UL<<4)              
#define U0_RX_PINSEL_REG   PINSEL0
#define U0_RX_PINSEL       (1UL<<6)              
#define U0_RX_PINMASK      (3UL<<6)              

#define ULCR_STOPBIT_SHIFT (2UL)
#define ULCR_PARITY_ODD    (0UL)
#define ULCR_PARITY_EVEN   (1UL)
#define ULCR_PARITY_ENABLE (1UL<<3)

#define ULSR_RDR           (1<<0)
#define ULSR_PE            (1<<2)
#define ULSR_FE            (1<<3)
#define ULSR_BI            (1<<4)
#define ULSR_THRE          (1<<5)
#define ULSR_TEMT          (1<<6)
#define ULSR_RXFE          (1<<7)

#define UIER_THRE          (1<<1)
#define UTER_TXEN          (1<<7)
#define UART_8N1           (uint8_t)(3<<0)        
#define UART_FIFO_OFF      (0x00)

#define UFCR_FIFO_ENABLE   (1<<0)
#define UFCR_RXFIFO_RESET  (1<<1)
#define UFCR_TXFIFO_RESET  (1<<2)
#define UFCR_RX_TRIGGER_1  (0<<6)
#define UFCR_RX_TRIGGER_4  (1<<6)
#define UFCR_RX_TRIGGER_8  (2<<6)
#define UFCR_RX_TRIGGER_14 (3<<6)

/**
 * Write a character over serial via UART.
 * @param c the character to write
 */
void PUTC(uint8_t c);

/**
 * Read a character over serial via UART. Note that this routine will block
 * forever until a character is available.
 * @return the character read
 */
uint8_t GETC(void);

#endif /* __UARTS_H__ */
