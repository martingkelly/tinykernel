#include <stdlib.h>
#include <string.h>

#include "tk/common.h"
#include "tk/data.h"
#include "tk/thread.h"
#include "tk/timing.h"
#include "tk/thread.h"
#include "tk/utility.h"

extern void TKFirstContextSwitch(int * stackPointer);
extern void TKContextSwitchYield(void);

static struct TKInstrumentData scheduleInstrumentData;

static void TKIdleThreadEntry(void * p) {
    for (;;);
}

void TKInitThreadData(void) {
    TKInitInstrumentData("scheduling", &scheduleInstrumentData);

    /* Setup the idle thread, just to simplify the code when there's no runnable
     * threads.
     */
    TKCreateThread("TKIdle", TK_PRIORITY_LOWEST, TKIdleThreadEntry, NULL);
}

void TKPrintSchedulingMetrics(void) {
    TKPrintInstrumentationData(&scheduleInstrumentData);
}

void TKAddThread(struct TKThreadQueue * queue,
                 struct TKThread * thread) {
    struct TKThread * prev;

    thread->queue = queue;

    /* Special-case queue of length 0. */
    if (queue->head == NULL) {
        thread->prev = thread;
        thread->next = thread;
        queue->head = thread;
        return;
    }

    /* Since the scheduler will pick threads based on properties of the threads
     * themselves (rather than the order of the threads in the queue), we can
     * add anywhere in the queue. That said, we would like the property that
     * threads are run in the order that they are created, so we will insert
     * right before the head.
     */
    prev = queue->head->prev;
    thread->prev = prev;
    thread->next = queue->head;
    prev->next = thread;
    queue->head->prev = thread;
}

int TKRemoveThread(struct TKThread * thread) {
    struct TKThread * prev;
    struct TKThreadQueue * queue;
    struct TKThread * next;

    /* Special-case of the thread not being in a queue. */
    queue = thread->queue;
    if (queue == NULL) {
        return -1;
    }

    prev = thread->prev;
    next = thread->next;
    thread->queue = NULL;
    thread->prev = NULL;
    thread->next = NULL;

    /* Special-case of single item queue. */
    if (prev == thread) {
        queue->head = NULL;
        return 0;
    }

    /* Normal case of two or more items. */
    prev->next = next;
    next->prev = prev;
    if (queue->head == thread) {
        queue->head = prev;
    }

   return 0;
}

struct TKThread * TKPopThread(struct TKThreadQueue * queue) {
    int result;
    struct TKThread * thread;

    if (queue->head == NULL) {
        return NULL;
    }

    thread = queue->head;
    result = TKRemoveThread(thread);
    if (result != 0) {
        return NULL;
    }

    return thread;
}

struct TKThread * TKPickThread(struct TKThreadQueue * queue) {
    struct TKThread * bestThread;
    TKThreadPriority highestPriority;
    struct TKThread * thread;

    if (queue->head == NULL) {
        return NULL;
    }

    /* Find the highest priority ready thread. */
    highestPriority = TK_PRIORITY_LOWEST - 1;
    bestThread = NULL;
    thread = queue->head;
    do {
        if (thread->priority > highestPriority) {
            highestPriority = thread->priority;
            bestThread = thread;
        }

        thread = thread->next;
    } while (thread != queue->head);

    return bestThread;
}

void TKSchedule(void) {
    CurrentThread = _TKSchedule(&RunQueue, &SleepQueue, TickCount);
}

struct TKThread * _TKSchedule(struct TKThreadQueue * runQueue,
                              struct TKThreadQueue * sleepQueue,
                              TKTickCount tickCount) {
    struct TKThread * next;
    struct TKThread * thread;

    /* Check for any sleeping threads that are ready to wake up. */
    if (sleepQueue->head != NULL) {
        thread = sleepQueue->head;
        do {
            next = thread->next;
            if (tickCount >= thread->sleepTarget) {
                TKRemoveThread(thread);
                TKAddThread(runQueue, thread);
                if (sleepQueue->head == NULL) {
                    break;
                }
            }

            thread = next;
        } while (thread != sleepQueue->head);
    }

    thread = TKPickThread(runQueue);

    /* Move head forward by one so that identical priority processes get
     * round-robin treatment.
     */
    runQueue->head = runQueue->head->next;

    return thread;
}

void TKSwitchThread(void * stackPointer) {
    CurrentThread->stackPointer = stackPointer;
    TKSchedule();
}

void TKInstrumentedSwitchThread(void * stackPointer) {
    TKStartInstrumenting(&scheduleInstrumentData);
    TKSwitchThread(stackPointer);
    TKStopInstrumenting(&scheduleInstrumentData);
}

void TKIncrementTick(void) {
    TickCount++;
}

int * TKInitStack(int * stack, TKThreadEntry entryPoint, void * data) {
    /* Calculate the top of the stack. */
    int * p = (int *) (stack + TK_STACK_SIZE - 1);

    *(p) = (int) entryPoint; /* PC */
    *(--p) = 0x0e0e0e0e; /* R14 - LR */
    *(--p) = 0x0c0c0c0c; /* R12 */
    *(--p) = 0x0b0b0b0b; /* R11 */
    *(--p) = 0x0a0a0a0a; /* R10 */
    *(--p) = 0x09090909; /* R9 */
    *(--p) = 0x08080808; /* R8 */
    *(--p) = 0x07070707; /* R7 */
    *(--p) = 0x06060606; /* R6 */
    *(--p) = 0x05050505; /* R5 */
    *(--p) = 0x04040404; /* R4 */
    *(--p) = 0x03030303; /* R3 */
    *(--p) = 0x02020202; /* R2 */
    *(--p) = 0x01010101; /* R1 */
    *(--p) = (int) data; /* R0 */
    *(--p) = ARM_SVC_MODE_ARM; /* CPSR */

    return p;
}

TKStatus _TKCreateThread(struct TKThreadQueue * freeQueue,
                         struct TKThreadQueue * runQueue,
                         const char * name,
                         TKThreadPriority priority,
                         TKThreadEntry entryPoint,
                         void * data) {
    uint32_t cpsr;
    int i;
    struct TKThread * thread;

    /* Validate thread name. */
    if (name == NULL) {
        return TK_NULL;
    }

    for (i = 0; i < TK_MAX_THREAD_NAME_LENGTH + 1; i++) {
        if (name[i] == '\0') {
            break;
        }
    }
    if (i == TK_MAX_THREAD_NAME_LENGTH + 1) {
        return TK_NAME_TOO_LONG;
    }

    /* Validate thread entry point. */
    if (entryPoint == NULL) {
        return TK_NULL;
    }

    /* Get a free thread. */
    thread = TKPopThread(freeQueue);
    if (thread == NULL) {
        return TK_FULL;
    }

    /* Everything is ok. Initialize the thread. */
    strcpy(thread->name, name);
    thread->sleepTarget = 0;
    thread->priority = priority;
    thread->stackPointer = TKInitStack(thread->stack, entryPoint, data);
    thread->queue = NULL;
    thread->prev = NULL;
    thread->next = NULL;

    cpsr = TKDisableInterrupts();
    TKAddThread(runQueue, thread);
    TKEnableInterrupts(cpsr);

    return TK_OK;
}

TKStatus TKCreateThread(const char * name,
                        TKThreadPriority priority,
                        TKThreadEntry entry,
                        void * data) {
    TKStatus status;

    status = _TKCreateThread(&FreeQueue,
                             &RunQueue,
                             name,
                             priority,
                             entry,
                             data);

    return status;
}

void _TKYieldThread(struct TKThread * thread) {
    uint32_t cpsr;

    cpsr = TKDisableInterrupts();
    TKContextSwitchYield();
    TKEnableInterrupts(cpsr);
}

void TKYieldThread(void) {
    _TKYieldThread(CurrentThread);
}

void _TKThreadSleep(struct TKThreadQueue * runQueue,
                    struct TKThreadQueue * sleepQueue,
                    struct TKThread * thread,
                    uint32_t seconds) {
    uint32_t cpsr;
    TKTickCount duration;
    int result;

    duration = TickHz * seconds;
    thread->sleepTarget = TickCount + duration;

    /* Both the run and sleep queues are touched by timer interrupt routines, so
     * disable interrupts.
     */
    cpsr = TKDisableInterrupts();
    result = TKRemoveThread(thread);
    if (result != 0) {
        TKFatal("Thread is not in run queue!");
    }
    TKAddThread(sleepQueue, thread);
    TKEnableInterrupts(cpsr);

    _TKYieldThread(thread);
}

void TKThreadSleep(uint32_t seconds) {
    _TKThreadSleep(&RunQueue, &SleepQueue, CurrentThread, seconds);
}
