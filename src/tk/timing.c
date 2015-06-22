#include <stdint.h>
#include <string.h>

#include "lpc/lpc2378.h"
#include "lpc/bsp.h"

#include "tk/data.h"
#include "tk/utility.h"

/* Timer Control Register (TCR) bits */
#define TCR_ENABLE_BIT BIT(0)
#define TCR_RESET_BIT BIT(1)

/* Match Control Register (MCR) bits */
#define MCR_MR0_INTERRUPT_BIT BIT(0)
#define MCR_MR0_RESET_BIT BIT(1)

#define US_PER_S (1000000UL)

void TKInitTimer(uint32_t hz) {
    int frequency;

    /* Reset and then disable the timer counter. */
    WRITEREG32(T0TCR, TCR_RESET_BIT);
    WRITEREG32(T0TCR, 0);

    /* Set the desired timer frequency. */
    frequency = BSP_CPU_PclkFreq(PCLKINDX_TIMER0);
    WRITEREG32(T0MR0, frequency / hz);

    /* Set to timer mode so the timer counter increments on every PCLK edge. */
    WRITEREG32(T0TCR, 0);
    WRITEREG32(T0PR, 0);
    WRITEREG32(T0PC, 0);

    /* When the timer fires, generate an interrupt and reset the counter. */
    WRITEREG16(T0MCR, MCR_MR0_INTERRUPT_BIT | MCR_MR0_RESET_BIT);

    /* Don't use the capture functionality. */
    WRITEREG32(T0CCR, 0);

    /* Don't use the external match functionality. */
    WRITEREG32(T0EMR, 0);

    WRITEREG32(VICINTENABLE,  (1 << VIC_TIMER0));
}

void TKStartTimer(void) {
    TickCount = 0;
    WRITEREG32(T0TCR, TCR_ENABLE_BIT);
}

void TKInitInstrumentData(const char * name, struct TKInstrumentData * data) {
    data->name = name;
    data->start = READREG32(T0TC);
    data->last = 0;
    data->count = 0;
    data->sum = 0;
    data->min = UINT32_MAX;
    data->max = 0;
}

void TKStartInstrumenting(struct TKInstrumentData * data) {
    data->last = READREG32(T0TC);
}

void TKStopInstrumenting(struct TKInstrumentData * data) {
    uint32_t current;
    uint32_t delta;

    current = READREG32(T0TC);

    /* Compute delta. Since the timer counter is reset after every interrupt, we
     * should not have to deal with overflow here.
     */
    delta = current - data->last;

    if (delta < data->min) {
        data->min = delta;
    }
    if (delta > data->max) {
        data->max = delta;
    }

    data->sum += delta;
    data->count++;
}
uint32_t Normalize(uint64_t n, uint32_t hz, uint32_t unit_per_s) {
    return unit_per_s * n / hz;
}

void TKPrintInstrumentationData(struct TKInstrumentData * data) {
    uint32_t average;
    uint32_t cpsr;
    uint32_t frequency;
    uint32_t max;
    uint32_t min;
    TKTickCount tickCount;
    struct TKInstrumentData tmp;
    uint64_t sum;
    uint64_t total;

    /* Disable interrupts so we can safely get metrics atomically. */
    cpsr = TKDisableInterrupts();
    memcpy(&tmp, data, sizeof(tmp));
    tickCount = TickCount;
    TKEnableInterrupts(cpsr);

    /* Compute average. */
    if (tmp.count != 0) {
        average = tmp.sum / tmp.count;
    }
    else {
        average = 0;
    }

    /* Normalize all metrics. */
    frequency = BSP_CPU_PclkFreq(PCLKINDX_TIMER0);
    average = Normalize(average, frequency, US_PER_S);
    min = Normalize(tmp.min, frequency, US_PER_S);
    max = Normalize(tmp.max, frequency, US_PER_S);
    sum = Normalize(tmp.sum, frequency, US_PER_S);

    /* Compute total time in us since we first started instrumenting. */
    total = 1000000ULL * tickCount / TickHz;

    TKPrintString("Time spent ");
    TKPrintString(tmp.name);
    TKPrintString(":\n");
    TKPrintString("Average: ");
    TKPrintDecimal(average);
    TKPrintString(" us\n");
    TKPrintString("Min: ");
    TKPrintDecimal(min);
    TKPrintString(" us\n");
    TKPrintString("Max: ");
    TKPrintDecimal(max);
    TKPrintString(" us\n");
    TKPrintString("Ratio of scheduling to total: ");
    TKPrintDecimal(sum);
    TKPrintString("/");
    TKPrintDecimal(total);
    TKPrintString("\n");
}
