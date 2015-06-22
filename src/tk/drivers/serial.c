/* Serial driver. This driver communicates with the serial port, sets baud rate,
 * etc. */

#include <stddef.h>
#include <string.h>

#include "lpc/init.h"
#include "lpc/lpc2378.h"
#include "lpc/uarts.h"

#include "tk/ddf.h"
#include "tk/status.h"
#include "tk/utility.h"

#include "tk/drivers/serial.h"

static void Open(void) {
    return;
}

static void Close(void) {
    return;
}

static int Read(TKStatus * status, uint8_t * buffer, uint32_t size) {
    uint32_t i;

    for (i = 0; i < size; i++) {
        buffer[i] = GETC();
    }

    *status = TK_OK;
    return size;
}

static int Write(TKStatus * status, const uint8_t * buffer, uint32_t size) {
    uint32_t i;

    for (i = 0; i < size; i++) {
        if (buffer[i] == '\n') {
            PUTC('\n');
            PUTC('\r');
        }
        else {
            PUTC(buffer[i]);
        }
    }

    *status = TK_OK;
    return size;
}

static TKStatus PowerUp(void) {
    VOLATILE32(PCONP) |= PCUART0;
    return TK_OK;
}

static TKStatus PowerDown(void) {
    VOLATILE32(PCONP) &= ~PCUART0;
    return TK_OK;
}

/* Ioctl */
static struct TKIoctlInfo IoctlBaud;

static struct TKIoctlInfo * IoctlInfo(uint32_t code) {
    switch (code) {
        case TK_SERIAL_IOCTL_BAUD:
            return &IoctlBaud;
        default:
            return NULL;
    }
}

static TKStatus IoctlBaudOp(const void * inBuf, void * outBuf) {
    struct TKSerialBaudInfo * baud;
    uint32_t cclk;
    int8_t lcr;
    uint8_t pclkDiv;
    uint8_t pclkSel;
    uint32_t uartDivisorLatch;
    uint8_t udlRoundBit;

    baud = (struct TKSerialBaudInfo *) inBuf;

    /* Stop any transmissions */
    P_UART0_REGS->TER = 0;

    /* Calculate and set baud rate */
	cclk = SCBParams.PLL_Fcco/SCBParams.CCLK_Div;
    pclkSel = GET_PCLK_SEL( P_SCB_REGS->PCLKSEL0, PCLK_UART0 );
    pclkDiv = ( pclkSel == 0 ? 4 : \
    		    pclkSel == 1 ? 1 : \
    		    pclkSel == 2 ? 2 : \
    		    pclkSel == 3 ? 8 : \
    		    0 ); /* this evaluation should never happen */
    if (pclkDiv == 0) {
        TKFatal("pclkDiv reached an impossible value; internal error");
    }

    uartDivisorLatch = ( 2 * ( (cclk/pclkDiv) / ( (baud->rate) * 16) ) );
    udlRoundBit = ( (uartDivisorLatch & 0x1) == 0 ? 0 : 1 );
    uartDivisorLatch /= 2;
    P_UART0_REGS->LCR = ULCR_DLAB_ENABLE;
    P_UART0_REGS->DLL = (uint8_t) uartDivisorLatch + udlRoundBit;
    P_UART0_REGS->DLM = (uint8_t)(uartDivisorLatch >> 8);

    /* Set mode */
    lcr = 0;
    lcr &= ~ULCR_DLAB_ENABLE;
    lcr |= baud->dataBits;
    lcr |= baud->stopBits << ULCR_STOPBIT_SHIFT;
    switch (baud->parity) {
    case TK_SERIAL_BAUD_PARITY_NONE:
        break;
    case TK_SERIAL_BAUD_PARITY_ODD:
        lcr |= ULCR_PARITY_ENABLE | ULCR_PARITY_ODD;
        break;
    case TK_SERIAL_BAUD_PARITY_EVEN:
        lcr |= ULCR_PARITY_ENABLE | ULCR_PARITY_EVEN;
        break;
    }
    
    P_UART0_REGS->LCR = lcr;

    /* Resume transmissions */
    P_UART0_REGS->TER = UTER_TXEN;

    return TK_OK;
}

void TKSerialDriverInit(void) {
    strcpy(TKSerialDriver.name, "serial");
    TKSerialDriver.major = TK_SERIAL_MAJOR;
    TKSerialDriver.minor = TK_SERIAL_MINOR;
    TKSerialDriver.powerstate = TK_POWER_ON;

    TKSerialDriver.ops.init = TKSerialDriverInit;
    TKSerialDriver.ops.open = Open;
    TKSerialDriver.ops.close = Close;
    TKSerialDriver.ops.read = Read;
    TKSerialDriver.ops.write = Write;
    TKSerialDriver.ops.ioctlInfo = IoctlInfo;
    TKSerialDriver.ops.powerUp = PowerUp;
    TKSerialDriver.ops.powerDown = PowerDown;

    IoctlBaud.type = TK_IOCTL_IN;
    IoctlBaud.inSize = sizeof(struct TKSerialBaudInfo);
    IoctlBaud.outSize = 0;
    IoctlBaud.op = IoctlBaudOp;
}
