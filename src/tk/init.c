#include <stddef.h>

#include "tk/ddf.h"
#include "tk/init.h"
#include "tk/timing.h"
#include "tk/thread.h"
#include "tk/utility.h"

extern void TKFirstContextSwitch(int * thread);

void TKInit(void) {
    TKInitKernelData();
    TKInitThreadData();
    TKInitDrivers();
    TKInitPrintData();
}

void TKStart(void) {
    TKSchedule();

    /* Disable interrupts so we don't race between a timer interrupt and the
     * first context switch. Interrupts will be reenabled when we context switch
     * into the first task's CPSR.
     */
    TKDisableInterrupts();
    TKStartTimer();
    TKFirstContextSwitch(CurrentThread->stackPointer);
}
