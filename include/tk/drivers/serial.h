#ifndef __TK_SERIAL_DRIVER_H__
#define __TK_SERIAL_DRIVER_H__

#define TK_SERIAL_MAJOR (1)
#define TK_SERIAL_MINOR (0)

#define TK_SERIAL_IOCTL_BAUD (0)

struct TKDriver TKSerialDriver;

void TKSerialDriverInit(void);

/* Note that these enums have carefully selected values in order to correspond
 * with the right bits to set in the UART0 LCR.
 */
enum TKSerialDataBits {
    TK_SERIAL_DATA_BITS_5 = 0,
    TK_SERIAL_DATA_BITS_6 = 1,
    TK_SERIAL_DATA_BITS_7 = 2,
    TK_SERIAL_DATA_BITS_8 = 3,
};

enum TKSerialStopBits {
    TK_SERIAL_STOP_BITS_1 = 0,
    TK_SERIAL_STOP_BITS_2 = 1
};

enum TKSerialBaudParity {
    TK_SERIAL_BAUD_PARITY_NONE,
    TK_SERIAL_BAUD_PARITY_ODD,
    TK_SERIAL_BAUD_PARITY_EVEN
};

struct TKSerialBaudInfo {
    uint32_t rate;
    uint8_t dataBits;
    uint8_t stopBits;
    enum TKSerialBaudParity parity;
};

#endif
