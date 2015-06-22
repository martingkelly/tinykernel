/*
 * MODULE:
 *
 *  pins.h
 * 
 * DESCRIPTION:
 *
 *  Provides the PIN configuration for LPC2378 part
 *  LPC2378 pin configured for UART0 TX and RX.
 *  
 * PLATFORM:
 *
 *  Olimex LPC-2378-STK
 *
 * ORIGINATOR:
 *
 *  Andrew N. Sloss
 *
 * HISTORY:
 *
 * 2007-10-01 DMI cleaned up mcuPins initializer with explicit brackets
 * 
 *  - started creating the header file
 *
 * NOTES:
 *
 *  n/a
 *
 */
 
#ifndef PINS_LPC2378
#define PINS_LPC2378

/*
 * MACROS
 */

#define PRIMARY            0x0              /* 0b00 primary function */
#define FIRST              0x1              /* 0b01 first  alternate function */
#define SECOND             0x2              /* 0b10 second alternate function */
#define THIRD              0x3              /* 0b11 third  alternate function */

/*
 * TYPES
 */ 

typedef struct
{
    uint8_t position; 
    uint8_t multiplex;
} pinStr;

typedef struct 
{
    uint32_t pinSelect;
    pinStr pin[16];
} pinConfiguratorStr;

/*
 * STRUCTURES
 */

pinConfiguratorStr mcuPins[] =
{
    /* PORT0 */
    {PINSEL0, { { 0, PRIMARY}, /* P0.0  */
                { 1, PRIMARY}, /* P0.1  */
                { 2, FIRST  }, /* P0.2  */      /* TXD0 - UART0 */
                { 3, FIRST  }, /* P0.3  */      /* RXD0 - UART0 */
                { 4, PRIMARY}, /* P0.4  */
                { 5, PRIMARY}, /* P0.5  */
                { 6, PRIMARY}, /* P0.6  */
                { 7, PRIMARY}, /* P0.7  */
                { 8, PRIMARY}, /* P0.8  */
                { 9, PRIMARY}, /* P0.9  */
                {10, PRIMARY}, /* P0.10 */
                {11, PRIMARY}, /* P0.11 */
                {12, PRIMARY}, /* P0.12 */
                {13, PRIMARY}, /* P0.13 */
                {14, PRIMARY}, /* P0.14 */
                {15, PRIMARY}  /* P0.15 */ } },
    {PINSEL1, { { 0, PRIMARY}, /* P0.16 */
                { 1, PRIMARY}, /* P0.17 */
                { 2, PRIMARY}, /* P0.18 */
                { 3, PRIMARY}, /* P0.19 */
                { 4, PRIMARY}, /* P0.20 */
                { 5, PRIMARY}, /* P0.21 */
                { 6, PRIMARY}, /* P0.22 */
                { 7, PRIMARY}, /* P0.23 */
                { 8, PRIMARY}, /* P0.24 */
                { 9, PRIMARY}, /* P0.25 */
                {10, PRIMARY}, /* P0.26 */
                {11, PRIMARY}, /* P0.27 */
                {12, PRIMARY}, /* P0.28 */
                {13, PRIMARY}, /* P0.29 */
                {14, PRIMARY}, /* P0.30 */
                {15, PRIMARY}  /* P0.31 */ } },

    /* PORT1 */
    {PINSEL2, { { 0, PRIMARY}, /* P1.0  */
                { 1, PRIMARY}, /* P1.1  */
                { 2, PRIMARY}, /* P1.2  */
                { 3, PRIMARY}, /* P1.3  */
                { 4, PRIMARY}, /* P1.4  */
                { 5, PRIMARY}, /* P1.5  */
                { 6, PRIMARY}, /* P1.6  */
                { 7, PRIMARY}, /* P1.7  */
                { 8, PRIMARY}, /* P1.8  */
                { 9, PRIMARY}, /* P1.9  */
                {10, PRIMARY}, /* P1.10 */
                {11, PRIMARY}, /* P1.11 */
                {12, PRIMARY}, /* P1.12 */
                {13, PRIMARY}, /* P1.13 */
                {14, PRIMARY}, /* P1.14 */
                {15, PRIMARY}  /* P1.15 */ } },
    {PINSEL3, { { 0, PRIMARY}, /* P1.16 */
                { 1, PRIMARY}, /* P1.17 */
                { 2, PRIMARY}, /* P1.18 */
                { 3, PRIMARY}, /* P1.19 */
                { 4, PRIMARY}, /* P1.20 */
                { 5, PRIMARY}, /* P1.21 */
                { 6, PRIMARY}, /* P1.22 */
                { 7, PRIMARY}, /* P1.23 */
                { 8, PRIMARY}, /* P1.24 */
                { 9, PRIMARY}, /* P1.25 */
                {10, PRIMARY}, /* P1.26 */
                {11, PRIMARY}, /* P1.27 */
                {12, PRIMARY}, /* P1.28 */
                {13, PRIMARY}, /* P1.29 */
                {14, PRIMARY}, /* P1.30 */
                {15, PRIMARY}  /* P1.31 */ } },

    /* PORT2 */
    {PINSEL4, { { 0, PRIMARY}, /* P2.0  */
                { 1, PRIMARY}, /* P2.1  */
                { 2, PRIMARY}, /* P2.2  */
                { 3, PRIMARY}, /* P2.3  */
                { 4, PRIMARY}, /* P2.4  */
                { 5, PRIMARY}, /* P2.5  */
                { 6, PRIMARY}, /* P2.6  */
                { 7, PRIMARY}, /* P2.7  */
                { 8, PRIMARY}, /* P2.8  */
                { 9, PRIMARY}, /* P2.9  */
                {10, PRIMARY}, /* P2.10 */
                {11, PRIMARY}, /* P2.11 */
                {12, PRIMARY}, /* P2.12 */
                {13, PRIMARY}, /* P2.13 */
                {14, PRIMARY}, /* P2.14 */
                {15, PRIMARY}  /* P2.15 */ } },
    {PINSEL5, { { 0, PRIMARY}, /* P2.16 */
                { 1, PRIMARY}, /* P2.17 */
                { 2, PRIMARY}, /* P2.18 */
                { 3, PRIMARY}, /* P2.19 */
                { 4, PRIMARY}, /* P2.20 */
                { 5, PRIMARY}, /* P2.21 */
                { 6, PRIMARY}, /* P2.22 */
                { 7, PRIMARY}, /* P2.23 */
                { 8, PRIMARY}, /* P2.24 */
                { 9, PRIMARY}, /* P2.25 */
                {10, PRIMARY}, /* P2.26 */
                {11, PRIMARY}, /* P2.27 */
                {12, PRIMARY}, /* P2.28 */
                {13, PRIMARY}, /* P2.29 */
                {14, PRIMARY}, /* P2.30 */
                {15, PRIMARY}  /* P2.31 */ } },
 
    /* PORT3 */
    {PINSEL6, { { 0, PRIMARY}, /* P3.0  */
                { 1, PRIMARY}, /* P3.1  */
                { 2, PRIMARY}, /* P3.2  */
                { 3, PRIMARY}, /* P3.3  */
                { 4, PRIMARY}, /* P3.4  */
                { 5, PRIMARY}, /* P3.5  */
                { 6, PRIMARY}, /* P3.6  */
                { 7, PRIMARY}, /* P3.7  */
                { 8, PRIMARY}, /* P3.8  */
                { 9, PRIMARY}, /* P3.9  */
                {10, PRIMARY}, /* P3.10 */
                {11, PRIMARY}, /* P3.11 */
                {12, PRIMARY}, /* P3.12 */
                {13, PRIMARY}, /* P3.13 */
                {14, PRIMARY}, /* P3.14 */
                {15, PRIMARY}  /* P3.15 */ } },
    {PINSEL7, { { 0, PRIMARY}, /* P3.16 */
                { 1, PRIMARY}, /* P3.17 */
                { 2, PRIMARY}, /* P3.18 */
                { 3, PRIMARY}, /* P3.19 */
                { 4, PRIMARY}, /* P3.20 */
                { 5, PRIMARY}, /* P3.21 */
                { 6, PRIMARY}, /* P3.22 */
                { 7, PRIMARY}, /* P3.23 */
                { 8, PRIMARY}, /* P3.24 */
                { 9, PRIMARY}, /* P3.25 */
                {10, PRIMARY}, /* P3.26 */
                {11, PRIMARY}, /* P3.27 */
                {12, PRIMARY}, /* P3.28 */
                {13, PRIMARY}, /* P3.29 */
                {14, PRIMARY}, /* P3.30 */
                {15, PRIMARY}  /* P3.31 */ } },

    /* PORT4 */
    {PINSEL8, { { 0, PRIMARY}, /* P4.0  */
                { 1, PRIMARY}, /* P4.1  */
                { 2, PRIMARY}, /* P4.2  */
                { 3, PRIMARY}, /* P4.3  */
                { 4, PRIMARY}, /* P4.4  */
                { 5, PRIMARY}, /* P4.5  */
                { 6, PRIMARY}, /* P4.6  */
                { 7, PRIMARY}, /* P4.7  */
                { 8, PRIMARY}, /* P4.8  */
                { 9, PRIMARY}, /* P4.9  */
                {10, PRIMARY}, /* P4.10 */
                {11, PRIMARY}, /* P4.11 */
                {12, PRIMARY}, /* P4.12 */
                {13, PRIMARY}, /* P4.13 */
                {14, PRIMARY}, /* P4.14 */
                {15, PRIMARY}  /* P4.15 */ } },
    {PINSEL9, { { 0, PRIMARY}, /* P4.16 */
                { 1, PRIMARY}, /* P4.17 */
                { 2, PRIMARY}, /* P4.18 */
                { 3, PRIMARY}, /* P4.19 */
                { 4, PRIMARY}, /* P4.20 */
                { 5, PRIMARY}, /* P4.21 */
                { 6, PRIMARY}, /* P4.22 */
                { 7, PRIMARY}, /* P4.23 */
                { 8, PRIMARY}, /* P4.24 */
                { 9, PRIMARY}, /* P4.25 */
                {10, PRIMARY}, /* P4.26 */
                {11, PRIMARY}, /* P4.27 */
                {12, PRIMARY}, /* P4.28 */
                {13, PRIMARY}, /* P4.29 */
                {14, PRIMARY}, /* P4.30 */
                {15, PRIMARY}  /* P4.31 */ } },

    {0,       { { 0, 0},
                { 0, 0},
                { 0, 0},
                { 0, 0},
                { 0, 0},
                { 0, 0},
                { 0, 0},
                { 0, 0},
                { 0, 0},
                { 0, 0},
                { 0, 0},
                { 0, 0},
                { 0, 0},
                { 0, 0},
                { 0, 0},
                { 0, 0} } }
};

#endif /* PINS_LPC2378 */
