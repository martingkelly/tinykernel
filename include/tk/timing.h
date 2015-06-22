#ifndef __TIMING_H__
#define __TIMING_H__

#include "lpc/lpc2378.h"

/* Note: At 2 Hz, this value overflows after 90 days. Since this is for a class
 * rather than a production system, I think it's ok. But if not, I would either
 * need to carefully design the system to handle the overflow or to use a
 * different timer system, such as:
 * - Two timer variables chained together (when one overflows, the other
 *   increments, and the tick is a combination of the two)
 * - A 64-bit timing.counter
 */
typedef volatile uint32_t TKTickCount;

struct TKInstrumentData {
    const char * name;
    uint32_t start;
    uint32_t last;
    uint32_t count;
    uint64_t sum;
    uint32_t min;
    uint32_t max;
};

/**
 * Initialize the data for instrumenting.
 * @param name the name of the data being instrumented
 * @param data the data for keeping track of instrumentation.
 */
void TKInitInstrumentData(const char * name, struct TKInstrumentData * data);

/**
 * Start instrumenting.
 * @param data the data for keeping track of instrumentation.
 */
void TKStartInstrumenting(struct TKInstrumentData * data);

/**
 * Stop instrumenting.
 * @param data the data for keeping track of instrumentation.
 */
void TKStopInstrumenting(struct TKInstrumentData * data);

/**
 * Print instrumentation data recorded so far.
 * @param data the data for keeping track of instrumentation.
 */
void TKPrintInstrumentationData(struct TKInstrumentData * data);

/*
 * Initialize the timer.
 *
 * @param hz the timer frequency
 */
void TKInitTimer(uint32_t hz);

/*
 * Start the timer.
 */
void TKStartTimer(void);

#endif
