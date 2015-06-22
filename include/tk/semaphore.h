#ifndef __TK_SEMAPHORE_H__
#define __TK_SEMAPHORE_H__

#include "tk/critical_section.h"
#include "tk/status.h"
#include "tk/thread.h"

struct TKSemaphore {
    struct TKCriticalSection cs;
    struct TKThreadQueue waitQueue;
    uint32_t count;
};

/**
 * Initialize a semaphore.
 *
 * @param sem a semaphore pointer
 * @param count the semaphore max count
 */
TKStatus TKCreateSemaphore(struct TKSemaphore * sem, uint32_t count);

/**
 * Up operation on a semaphore.
 *
 * @param sem a semaphore pointer
 */
TKStatus _TKUpSemaphore(struct TKSemaphore * sem,
                        struct TKThreadQueue * runQueue,
                        struct TKThread * thread);
TKStatus TKUpSemaphore(struct TKSemaphore * sem);

/**
 * Down operation on a semaphore.
 *
 * @param sem a semaphore pointer
 */
TKStatus _TKDownSemaphore(struct TKSemaphore * sem, struct TKThread * thread);
TKStatus TKDownSemaphore(struct TKSemaphore * sem);

#endif
