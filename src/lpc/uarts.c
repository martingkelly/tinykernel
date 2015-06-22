#include "lpc/lpc2378.h"
#include "lpc/uarts.h"

void PUTC(uint8_t c) {
    while(!(VOLATILE32(U0LSR) & ULSR_THRE)) {
        continue;
    }
    VOLATILE32(U0THR) = c;
}

uint8_t GETC(void) {
    while(!(VOLATILE32(U0LSR) & ULSR_RDR)) {
        continue;
    }
    return VOLATILE32(U0RBR);
}
