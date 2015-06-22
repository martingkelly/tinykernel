#ifndef __TK_UTILITY_H__
#define __TK_UTILITY_H__

#include "lpc/lpc2378.h"

extern uint32_t TKDisableInterrupts(void);
extern void TKEnableInterrupts(uint32_t cpsr);

/**
 * Initialize the data needed for the TK print wrappers.
 */
void TKInitPrintData(void);

/**
 * Print a hex word over UART.
 * @param x the hex word
 */
void TKPrintHex(uint32_t x);
void TKRawPrintHex(uint32_t x);

/**
 * Print a decimal (up to 64-bit) over UART.
 * @param x the decimal
 */
void TKPrintDecimal(uint64_t x);
void TKRawPrintDecimal(uint64_t x);

/**
 * Print a NULL-terminated string over UART.
 * @param p the string to print
 */
void TKPrintString(const char * p);

/**
 * Print a NULL-terminated string directly over UART without using the TK serial
 * driver. This should be used when TK is not yet initialized.
 * @param p the string to print
 */
void TKRawPrintString(const char * p);

/**
 * Display an error message and halt the world
 */
void TKFatal(const char * msg);

#endif
