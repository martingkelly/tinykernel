#include <stddef.h>

#include "lpc/threads.h"

#include "tk/semaphore.h"
#include "tk/utility.h"

void producer(void * p) {
	struct ThreadData * data;

	data = (struct ThreadData *) p;

    for (;;) {
        TKDownSemaphore(&data->sem);
        if (data->inc == 1) {
            TKUpSemaphore(&data->sem);
            TKYieldThread();
            continue;
        }
        data->inc++;
        TKPrintString("Producer incremented, now at ");
        TKPrintDecimal(data->inc);
        TKPrintString("\n");
        TKUpSemaphore(&data->sem);
    }
}

void consumer(void * p) {
	struct ThreadData * data;

	data = (struct ThreadData *) p;

    for(;;) {
        TKDownSemaphore(&data->sem);
        if (data->inc == 0) {
            TKUpSemaphore(&data->sem);
            TKYieldThread();
            continue;
        }
        data->inc--;
        TKPrintString("Consumer decremented, now at ");
        TKPrintDecimal(data->inc);
        TKPrintString("\n");
        TKUpSemaphore(&data->sem);
    }
}

void monitor(void * p) {
	struct ThreadData * data;

	data = (struct ThreadData *) p;

    for (;;) {
        TKDownSemaphore(&data->sem);
        TKPrintString("Monitor running, no errors\n");
        TKPrintSchedulingMetrics();
        if (data->inc > 1) {
            TKPrintString("Monitor caught error, inc is at ");
            TKPrintDecimal(data->inc);
            TKPrintString("\n");
            TKFatal(NULL);
        }
        TKUpSemaphore(&data->sem);
        TKYieldThread();
    }
}

void idle(void * p) {
    for (;;) {
        TKPrintString("Idle thread sleeping for 10 seconds...\n");
        TKThreadSleep(10);
        TKPrintString("Idle thread waking up\n");
    }
}
