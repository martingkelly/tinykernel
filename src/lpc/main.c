#include <string.h>

#include "lpc/lpc2378.h"
#include "lpc/init.h"
#include "lpc/threads.h"

#include "tk/critical_section.h"
#include "tk/data.h"
#include "tk/init.h"
#include "tk/tests.h"
#include "tk/thread.h"
#include "tk/utility.h"

struct ThreadData data;

/**
 * main: this function is the first to be called.
 * @return N/A main does not return and instead infinite loops
 */
int main(void) {
    TKStatus status;

    initHardware();
    TKRawPrintString("Hardware initialized!\n");

    RunTests();

    TKInit();
    TKPrintString("Tiny Kernel initialized!\n");

    memset(&data, 0, sizeof(data));
    TKCreateSemaphore(&data.sem, 1);
    status = TKCreateThread("producer",
                            TK_PRIORITY_NORMAL,
                            producer,
                            &data);
    if (status != TK_OK) {
        TKPrintString("Error creating producer thread\n");
    }

    TKCreateThread("consumer",
                   TK_PRIORITY_NORMAL,
                   consumer,
                   &data);
    if (status != TK_OK) {
        TKPrintString("Error creating consumer thread\n");
    }

    TKCreateThread("monitor",
                   TK_PRIORITY_NORMAL,
                   monitor,
                   &data);
    if (status != TK_OK) {
        TKPrintString("Error creating monitor thread\n");
    }

    TKCreateThread("idle",
                   TK_PRIORITY_NORMAL,
                   idle,
                   &data);
    if (status != TK_OK) {
        TKPrintString("Error creating idle thread\n");
    }

    TKStart();

    return 0;
}
