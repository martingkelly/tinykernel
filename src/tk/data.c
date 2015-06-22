#include <stddef.h>

#include "tk/common.h"
#include "tk/critical_section.h"
#include "tk/data.h"
#include "tk/thread.h"
#include "tk/utility.h"

static struct TKThread threads[16];

void TKInitKernelData(void) {
    size_t i;

    FreeQueue.head = NULL;
    RunQueue.head = NULL;
    SleepQueue.head = NULL;
    for (i = 0; i < ARRAYLEN(threads); i++) {
        TKAddThread(&FreeQueue, &threads[i]);
    }

    CurrentThread = NULL;
    TickHz = 500;

    TKInitTimer(TickHz);
}
