#include <stddef.h>

#include "lpc/uarts.h"

#include "tk/common.h"
#include "tk/ddf.h"
#include "tk/utility.h"

#include "tk/drivers/serial.h"

static TKDriverHandle handle;

void TKInitPrintData(void) {
    TKStatus status;

    handle = TKDriverOpen(TK_SERIAL_MAJOR, TK_SERIAL_MINOR);
    if (handle == NULL) {
        TKFatal("Serial driver open failed!");
    }

    struct TKSerialBaudInfo baud = {
        .rate = 38400,
        .dataBits = TK_SERIAL_DATA_BITS_8,
        .stopBits = TK_SERIAL_STOP_BITS_1,
        .parity = TK_SERIAL_BAUD_PARITY_NONE
    };

    status = TKDriverIoctl(handle,
                           TK_SERIAL_IOCTL_BAUD,
                           &baud,
                           sizeof(baud),
                           NULL,
                           0);
    if (status != TK_OK) {
        TKFatal("Failed to set baud rate in serial driver!");
    }
}

void _TKPrintHex(uint32_t x, void (*print)(const char *)) {
    char buffer[11];
    uint32_t i;
    uint32_t u32Mask  = 0xF0000000;
    uint8_t u32Shift = 32;
    uint32_t u32Char;

    i = 0;
    do {
        u32Shift -= 4;
        u32Char = (x & u32Mask) >> u32Shift;
        u32Mask >>= 4;
        if (u32Char >= 0xA) {
            buffer[i] = 'A' + (u32Char - 10);
        }
        else {
            buffer[i] = '0' + u32Char;
        }
        i++;
    } while (u32Shift > 0);
    buffer[i] = '\0';

    print(buffer);
}

void TKPrintHex(uint32_t x) {
    _TKPrintHex(x, TKPrintString);
}

void TKRawPrintHex(uint32_t x) {
    _TKPrintHex(x, TKRawPrintString);
}

void _TKPrintDecimal(uint64_t x, void (*print)(const char *)) {
    char buffer[21];
    char *p = &buffer[sizeof(buffer) - 1];

    *p = '\0';
    do {
        p--;
        *p = (x % 10) + '0';
        x /= 10;
    } while (x > 0);

    print(p);
}

void TKPrintDecimal(uint64_t x) {
    return _TKPrintDecimal(x, TKPrintString);
}

void TKRawPrintDecimal(uint64_t x) {
    return _TKPrintDecimal(x, TKRawPrintString);
}

void TKPrintString(const char * s) {
    uint8_t buffer[128];
    int bytesRead;
    uint32_t i;
    const char * p;
    TKStatus status;

    if (s == NULL) {
        return;
    }

    p = s;
    while (true) {
        for (i = 0; i < ARRAYLEN(buffer) && *p != '\0'; i++, p++) {
            buffer[i] = *p;
        }

        do {
            bytesRead = TKDriverWrite(handle, &status, buffer, i);
            if (status != TK_OK) {
                TKFatal("Serial driver write failed!");
            }
            i -= bytesRead;
        } while (i != 0);

        if (*p == '\0') {
            break;
        }
    }
}

void TKRawPrintString(const char * s) {
    const char *p;

    if (s == NULL) {
        return;
    }

    for (p = s; *p != '\0'; p++) {
        if (*p == '\n') {
            PUTC('\n');
            PUTC('\r');
        }
        else {
            PUTC(*p);
        }
    }
}

void TKFatal(const char * msg) {
    TKDisableInterrupts();
    if (msg != NULL) {
        TKRawPrintString(msg);
        TKRawPrintString("\n");
    }
    for (;;);
}
