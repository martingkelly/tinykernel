#include <stddef.h>

#include "tk/data.h"
#include "tk/semaphore.h"
#include "tk/thread.h"
#include "tk/utility.h"

TKStatus TKCreateSemaphore(struct TKSemaphore * sem, uint32_t count) {
    if (sem == NULL) {
        return TK_UNEXPECTED;
    }

    TKCreateCriticalSection(&sem->cs);
    sem->waitQueue.head = NULL;
    sem->count = count;

    return TK_OK;
}

TKStatus _TKUpSemaphore(struct TKSemaphore * sem,
                        struct TKThreadQueue * runQueue,
                        struct TKThread * thread) {
    uint32_t cpsr;
    struct TKThread * waiter;

    if (sem == NULL) {
        return TK_UNEXPECTED;
    }

    TKEnterCriticalSection(&sem->cs);
    if (sem->count == 0 && sem->waitQueue.head != NULL) {
        waiter = TKPopThread(&sem->waitQueue);
        cpsr = TKDisableInterrupts();
        TKAddThread(runQueue, waiter);
        TKLeaveCriticalSection(&sem->cs);
        TKEnableInterrupts(cpsr);
        return TK_OK;
    }
    else {
        sem->count++;
    }
    TKLeaveCriticalSection(&sem->cs);

    return TK_OK;
}

TKStatus TKUpSemaphore(struct TKSemaphore * sem) {
    return _TKUpSemaphore(sem, &RunQueue, CurrentThread);
}

TKStatus _TKDownSemaphore(struct TKSemaphore * sem, struct TKThread * thread) {
    uint32_t cpsr;

    if (sem == NULL) {
        return TK_UNEXPECTED;
    }

    TKEnterCriticalSection(&sem->cs);
    if (sem->count == 0) {
        cpsr = TKDisableInterrupts();
        TKRemoveThread(thread);
        TKLeaveCriticalSection(&sem->cs);
        TKAddThread(&sem->waitQueue, thread);
        TKEnableInterrupts(cpsr);
        _TKYieldThread(thread);
        return TK_OK;
    }
    else {
        sem->count--;
    }
    TKLeaveCriticalSection(&sem->cs);

    return TK_OK;
}

TKStatus TKDownSemaphore(struct TKSemaphore * sem) {
    return _TKDownSemaphore(sem, CurrentThread);
}
