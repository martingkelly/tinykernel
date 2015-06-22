#include <stdlib.h>
#include <string.h>

#include "tk/common.h"
#include "tk/ddf.h"
#include "tk/tests.h"
#include "tk/thread.h"
#include "tk/utility.h"

#include "tk/drivers/test.h"

#define MAX_TEST_THREADS (3)

#define ASSERT(x) do { if (!(x)) { return -1; } } while (0)

struct ThreadInfo {
    struct TKThread * thread;
    TKThreadPriority priority;
    struct TKThreadQueue * queue;
};

static struct TKThread threads[MAX_TEST_THREADS];
static struct TKThreadQueue freeQueue;
static struct TKThreadQueue sleepQueue;
static struct TKThreadQueue runQueue;

/**
 * Sets up the thread queue with some number of threads.
 *
 * @param sleepCount the number of threads in the sleep queue
 * @param sleepPriorities the priorities of the threads in the sleep queue
 * @param runCount the number of threads in the run queue
 * @param runPriorities the priorities of the threads in the run queue
 */
static void InitializeThreadQueues(struct ThreadInfo * threadInfo, int count) {
    int i;
    struct ThreadInfo * info;

    freeQueue.head = NULL;
    sleepQueue.head = NULL;
    runQueue.head = NULL;

    for (i = 0; i < count; i++) {
        info = &threadInfo[i];
        info->thread->priority = info->priority;
        TKAddThread(info->queue, info->thread);
    }
    for (i = count; i < MAX_TEST_THREADS; i++) {
        TKAddThread(&freeQueue, &threads[i]);
    }

    return;
}

static int ScheduleEmpty(void) {
    struct TKThread * thread;

    InitializeThreadQueues(NULL, 0);

    thread = _TKSchedule(&runQueue, &sleepQueue, 0);
    ASSERT(thread == NULL);

    return 0;
}

static int ScheduleOne(void) {
    struct TKThread * thread;
    struct ThreadInfo info[1] = {
                                 { &threads[0], TK_PRIORITY_NORMAL, &runQueue }
                                };
    InitializeThreadQueues(info, ARRAYLEN(info));

    thread = _TKSchedule(&runQueue, &sleepQueue, 0);
    ASSERT(thread == &threads[0]);

    return 0;
}

static int ScheduleAllBlocked(void) {
    int i;
    struct TKThread * thread;
    struct ThreadInfo info[] = {
                                { &threads[0], TK_PRIORITY_NORMAL, &sleepQueue },
                                { &threads[1], TK_PRIORITY_NORMAL, &sleepQueue },
                                { &threads[2], TK_PRIORITY_NORMAL, &sleepQueue },
                               };
    for (i = 0; i < MAX_TEST_THREADS; i++) {
        threads[i].sleepTarget = 1000;
    }
    InitializeThreadQueues(info, ARRAYLEN(info));

    thread = _TKSchedule(&runQueue, &sleepQueue, 0);
    ASSERT(thread == NULL);

    return 0;
}

static int ScheduleAllSamePriority(void) {
    struct TKThread * thread;
    struct ThreadInfo info[] = {
                                { &threads[0], TK_PRIORITY_NORMAL, &runQueue },
                                { &threads[1], TK_PRIORITY_NORMAL, &runQueue },
                                { &threads[2], TK_PRIORITY_NORMAL, &runQueue },
                               };
    InitializeThreadQueues(info, ARRAYLEN(info));

    thread = _TKSchedule(&runQueue, &sleepQueue, 0);
    ASSERT(thread == &threads[0]);

    return 0;
}

static int ScheduleHighReady(void) {
    struct TKThread * thread;
    struct ThreadInfo info[] = {
                                { &threads[0], TK_PRIORITY_NORMAL, &runQueue },
                                { &threads[1], TK_PRIORITY_NORMAL + 1, &runQueue },
                                { &threads[2], TK_PRIORITY_NORMAL, &runQueue },
                               };
    InitializeThreadQueues(info, ARRAYLEN(info));

    thread = _TKSchedule(&runQueue, &sleepQueue, 0);
    ASSERT(thread == &threads[1]);

    return 0;
}

static int ScheduleHighBlockedLowReady(void) {
    struct TKThread * thread;
    struct ThreadInfo info[] = {
                                { &threads[0], TK_PRIORITY_HIGHEST, &sleepQueue },
                                { &threads[1], TK_PRIORITY_LOWEST, &runQueue },
                               };
    threads[0].sleepTarget = 1000;
    InitializeThreadQueues(info, ARRAYLEN(info));

    thread = _TKSchedule(&runQueue, &sleepQueue, 0);
    ASSERT(thread == &threads[1]);

    return 0;
}

static int ScheduleThreadWakesUp(void) {
    struct TKThread * thread;
    struct ThreadInfo info[] = {
                                { &threads[0], TK_PRIORITY_NORMAL, &sleepQueue },
                               };
    threads[0].sleepTarget = 1000;
    InitializeThreadQueues(info, ARRAYLEN(info));

    thread = _TKSchedule(&runQueue, &sleepQueue, 1000);
    ASSERT(thread == &threads[0]);

    return 0;
}

static int ScheduleThreadAlmostWakesUp(void) {
    struct TKThread * thread;
    struct ThreadInfo info[] = {
                                { &threads[0], TK_PRIORITY_HIGHEST, &sleepQueue },
                                { &threads[1], TK_PRIORITY_LOWEST, &runQueue },
                               };
    threads[0].sleepTarget = 1000;
    InitializeThreadQueues(info, ARRAYLEN(info));

    thread = _TKSchedule(&runQueue, &sleepQueue, 999);
    ASSERT(thread == &threads[1]);

    return 0;
}

static int ScheduleHighWakesUpLowReady(void) {
    struct TKThread * thread;
    struct ThreadInfo info[] = {
                                { &threads[0], TK_PRIORITY_HIGHEST, &sleepQueue },
                                { &threads[1], TK_PRIORITY_LOWEST, &runQueue },
                               };
    threads[0].sleepTarget = 1000;
    InitializeThreadQueues(info, ARRAYLEN(info));

    thread = _TKSchedule(&runQueue, &sleepQueue, 1000);
    ASSERT(thread == &threads[0]);

    return 0;
}

static int CreateThreadNullName(void) {
    TKThreadStatus status;
    InitializeThreadQueues(NULL, 0);

    status = _TKCreateThread(&freeQueue,
                             &runQueue,
                             NULL,
                             TK_PRIORITY_NORMAL,
                             NULL,
                             NULL);
    ASSERT(status == TK_NULL);

    return 0;
}

static int CreateThreadNameTooLong(void) {
    TKThreadStatus status;
    InitializeThreadQueues(NULL, 0);

    status = _TKCreateThread(&freeQueue,
                             &runQueue,
                             "abcdefghijkl",
                             TK_PRIORITY_NORMAL,
                             NULL,
                             NULL);
    ASSERT(status == TK_NAME_TOO_LONG);
    return 0;
}

static int CreateThreadNullEntryPoint(void) {
    TKThreadStatus status;
    InitializeThreadQueues(NULL, 0);

    status = _TKCreateThread(&freeQueue,
                             &runQueue,
                             "abc",
                             TK_PRIORITY_NORMAL,
                             NULL,
                             NULL);
    ASSERT(status == TK_NULL);
    return 0;
}

static int CreateThreadNoFreeSlots(void) {
    TKThreadStatus status;
    struct ThreadInfo info[] = {
                                  { &threads[0], TK_PRIORITY_NORMAL, &runQueue },
                                  { &threads[1], TK_PRIORITY_NORMAL, &runQueue },
                                  { &threads[2], TK_PRIORITY_NORMAL, &runQueue },
                                 };
    InitializeThreadQueues(info, ARRAYLEN(info));

    status = _TKCreateThread(&freeQueue,
                             &runQueue,
                             "abc",
                             TK_PRIORITY_NORMAL,
                             (void *) 1,
                             NULL);
    ASSERT(status == TK_FULL);
    return 0;
}

static int CreateThreadValidateNormalCase(void) {
    int i;
    TKThreadStatus status;
    InitializeThreadQueues(NULL, 0);

    for (i = 0; i < MAX_TEST_THREADS; i++) {
        status = _TKCreateThread(&freeQueue,
                                 &runQueue,
                                 "abc",
                                 TK_PRIORITY_NORMAL,
                                 (void *) 1,
                                 NULL);
        ASSERT(status == TK_OK);
    }

    return 0;
}

int DriverNullOp(void) {
    uint8_t buf[1];
    int ret;
    TKPowerState state;
    TKStatus status;

    TKInitDrivers();
    
    status = TKDriverClose(NULL);
    ASSERT(status == TK_NULL);

    status = TK_OK;
    ret = TKDriverRead(NULL, &status, buf, sizeof(buf));
    ASSERT(ret == -1);
    ASSERT(status == TK_NULL);

    status = TK_OK;
    ret = TKDriverWrite(NULL, &status, buf, sizeof(buf));
    ASSERT(ret == -1);
    ASSERT(status == TK_NULL);

    status = TKDriverIoctl(NULL,
                           TK_TEST_IOCTL_IN_OUT,
                           buf,
                           sizeof(buf),
                           buf,
                           sizeof(buf));
    ASSERT(status == TK_NULL);

    status = TKDriverPowerUp(NULL);
    ASSERT(status == TK_NULL);

    status = TKDriverPowerDown(NULL);
    ASSERT(status == TK_NULL);

    status = TKDriverPowerState(NULL, &state);
    ASSERT(status == TK_NULL);

    status = TKDriverPowerState(NULL, &state);
    ASSERT(status == TK_NULL);

    return 0;
} 
int DriverClosedOp(void) {
    uint8_t buf[1];
    TKDriverHandle handle;
    int ret;
    TKPowerState state;
    TKStatus status;

    TKInitDrivers();

    handle = TKDriverOpen(TK_TEST_MAJOR, TK_TEST_MINOR);
    ASSERT(handle != NULL);

    status = TKDriverClose(handle);
    ASSERT(status == TK_OK);

    status = TKDriverClose(handle);
    ASSERT(status == TK_CLOSED);

    status = TK_OK;
    ret = TKDriverRead(handle, &status, buf, sizeof(buf));
    ASSERT(ret == -1);
    ASSERT(status == TK_CLOSED);

    status = TK_OK;
    ret = TKDriverWrite(handle, &status, buf, sizeof(buf));
    ASSERT(ret == -1);
    ASSERT(status == TK_CLOSED);

    status = TKDriverIoctl(handle,
                           TK_TEST_IOCTL_IN_OUT,
                           buf,
                           sizeof(buf),
                           buf,
                           sizeof(buf));
    ASSERT(status == TK_CLOSED);

    status = TKDriverPowerUp(handle);
    ASSERT(status == TK_CLOSED);

    status = TKDriverPowerDown(handle);
    ASSERT(status == TK_CLOSED);

    status = TKDriverPowerState(handle, &state);
    ASSERT(status == TK_CLOSED);

    return 0;
}

int DriverPoweredDownOps(void) {
    uint8_t buf[1];
    TKDriverHandle handle;
    TKStatus status;
    int ret;

    TKInitDrivers();
    handle = TKDriverOpen(TK_TEST_MAJOR, TK_TEST_MINOR);

    status = TKDriverPowerDown(handle);
    ASSERT(status == TK_OK);

    ret = TKDriverRead(handle, &status, buf, sizeof(buf));
    ASSERT(ret == -1);
    ASSERT(status == TK_NO_POWER);

    ret = TKDriverWrite(handle, &status, buf, sizeof(buf));
    ASSERT(ret == -1);
    ASSERT(status == TK_NO_POWER);

    return 0;
}

int DriverNullStatusRead(void) {
    uint8_t buf[1];
    TKDriverHandle handle;
    int ret;

    TKInitDrivers();
    handle = TKDriverOpen(TK_TEST_MAJOR, TK_TEST_MINOR);

    ret = TKDriverRead(handle, NULL, buf, sizeof(buf));
    ASSERT(ret == -1);

    return 0;
}

int DriverNullStatusWrite(void) {
    uint8_t buf[1];
    TKDriverHandle handle;
    int ret;

    TKInitDrivers();
    handle = TKDriverOpen(TK_TEST_MAJOR, TK_TEST_MINOR);

    ret = TKDriverWrite(handle, NULL, buf, sizeof(buf));
    ASSERT(ret == -1);

    return 0;
}

int DriverNullPowerState(void) {
    TKDriverHandle handle;
    TKStatus status;

    TKInitDrivers();
    handle = TKDriverOpen(TK_TEST_MAJOR, TK_TEST_MINOR);

    status = TKDriverPowerState(handle, NULL);
    ASSERT(status == TK_NULL);

    return 0;
}

int DriverDoubleOpen(void) {
    TKDriverHandle handle;

    TKInitDrivers();

    handle = TKDriverOpen(TK_TEST_MAJOR, TK_TEST_MINOR);
    ASSERT(handle != NULL);

    handle = TKDriverOpen(TK_TEST_MAJOR, TK_TEST_MINOR);
    ASSERT(handle == NULL);

    return 0;
}

int DriverIoctlBadCode(void) {
    uint8_t buf[1];
    TKDriverHandle handle;
    TKStatus status;

    TKInitDrivers();
    handle = TKDriverOpen(TK_TEST_MAJOR, TK_TEST_MINOR);

    status = TKDriverIoctl(handle,
                           0xFFFFFFFF,
                           buf,
                           sizeof(buf),
                           buf,
                           sizeof(buf));
    ASSERT(status == TK_IOCTL_BAD_CODE);

    return 0;
}

int DriverIoctlBadBufferSizes(void) {
    uint8_t buf[4];
    TKDriverHandle handle;
    TKStatus status;

    TKInitDrivers();
    handle = TKDriverOpen(TK_TEST_MAJOR, TK_TEST_MINOR);

    status = TKDriverIoctl(handle,
                           TK_IOCTL_IN,
                           buf,
                           1,
                           NULL,
                           0);
    ASSERT(status == TK_IOCTL_IN_BUF_BAD_SIZE);

    status = TKDriverIoctl(handle,
                           TK_IOCTL_OUT,
                           NULL,
                           0,
                           buf,
                           1);
    ASSERT(status == TK_IOCTL_OUT_BUF_BAD_SIZE);

    status = TKDriverIoctl(handle,
                           TK_IOCTL_IN_OUT,
                           buf,
                           4,
                           buf,
                           1);
    ASSERT(status == TK_IOCTL_OUT_BUF_BAD_SIZE);

    return 0;
}

int DriverIoctlNullBuffers(void) {
    uint8_t buf[4];
    TKDriverHandle handle;
    TKStatus status;

    TKInitDrivers();
    handle = TKDriverOpen(TK_TEST_MAJOR, TK_TEST_MINOR);

    status = TKDriverIoctl(handle,
                           TK_IOCTL_IN,
                           NULL,
                           0,
                           NULL,
                           0);
    ASSERT(status == TK_IOCTL_IN_BUF_NULL);

    status = TKDriverIoctl(handle,
                           TK_IOCTL_OUT,
                           NULL,
                           0,
                           NULL,
                           0);
    ASSERT(status == TK_IOCTL_OUT_BUF_NULL);

    status = TKDriverIoctl(handle,
                           TK_IOCTL_IN_OUT,
                           buf,
                           4,
                           NULL,
                           0);
    ASSERT(status == TK_IOCTL_OUT_BUF_NULL);

    return 0;
}

int DriverDoublePowerUp(void) {
    TKDriverHandle handle;
    TKStatus status;

    TKInitDrivers();
    handle = TKDriverOpen(TK_TEST_MAJOR, TK_TEST_MINOR);

    status = TKDriverPowerDown(handle);
    ASSERT(status == TK_OK);

    status = TKDriverPowerUp(handle);
    ASSERT(status == TK_OK);

    status = TKDriverPowerUp(handle);
    ASSERT(status == TK_ALREADY_POWERED_ON);

    return 0;
}

int DriverDoublePowerDown(void) {
    TKDriverHandle handle;
    TKStatus status;

    TKInitDrivers();
    handle = TKDriverOpen(TK_TEST_MAJOR, TK_TEST_MINOR);

    status = TKDriverPowerDown(handle);
    ASSERT(status == TK_OK);

    status = TKDriverPowerDown(handle);
    ASSERT(status == TK_ALREADY_POWERED_OFF);

    return 0;
}

int DriverPowerStatesNotSupported(void) {
    TKDriverHandle handle;
    TKPowerState state;
    TKStatus status;

    TKInitDrivers();
    handle = TKDriverOpen(TK_TEST_MAJOR, TK_TEST_MINOR);

    /* Turn off power state support by setting the handlers to NULL. */
    handle->driver->ops.powerUp = NULL;
    handle->driver->ops.powerDown = NULL;

    status = TKDriverPowerDown(handle);
    ASSERT(status == TK_POWER_STATES_UNSUPPORTED);

    status = TKDriverPowerDown(handle);
    ASSERT(status == TK_POWER_STATES_UNSUPPORTED);

    status = TKDriverPowerState(handle, &state);
    ASSERT(status == TK_POWER_STATES_UNSUPPORTED);

    return 0;

}

int DriverReadWriteVerify(void) {
    uint8_t buf[4];
    int i;
    int ret;
    TKDriverHandle handle;
    TKStatus status;

    TKInitDrivers();
    handle = TKDriverOpen(TK_TEST_MAJOR, TK_TEST_MINOR);

    for (i = 0; i < ARRAYLEN(buf); i++) {
        buf[i] = i;
    }
    ret = TKDriverWrite(handle, &status, buf, sizeof(buf));
    ASSERT(ret == sizeof(buf));
    ASSERT(status == TK_OK);

    memset(buf, 0, sizeof(buf));
    ret = TKDriverRead(handle, &status, buf, sizeof(buf));
    ASSERT(ret == sizeof(buf));
    ASSERT(status == TK_OK);

    for (i = 0; i < ARRAYLEN(buf); i++) {
        ASSERT(buf[i] == i);
    }

    return 0;
}

int DriverCloseVerify(void) {
    TKDriverHandle handle;
    TKStatus status;

    TKInitDrivers();
    handle = TKDriverOpen(TK_TEST_MAJOR, TK_TEST_MINOR);
    ASSERT(handle->used == true);

    status = TKDriverClose(handle);
    ASSERT(status == TK_OK);
    ASSERT(handle->used == false);

    return 0;
}

int DriverPowerStateVerify(void) {
    TKDriverHandle handle;
    TKPowerState state;
    TKStatus status;

    TKInitDrivers();
    handle = TKDriverOpen(TK_TEST_MAJOR, TK_TEST_MINOR);

    status = TKDriverPowerState(handle, &state);
    ASSERT(status == TK_OK);
    ASSERT(state == TK_POWER_ON);

    status = TKDriverPowerDown(handle);
    ASSERT(status == TK_OK);

    status = TKDriverPowerState(handle, &state);
    ASSERT(status == TK_OK);
    ASSERT(state == TK_POWER_OFF);

    status = TKDriverPowerUp(handle);
    ASSERT(status == TK_OK);

    status = TKDriverPowerState(handle, &state);
    ASSERT(status == TK_OK);
    ASSERT(state == TK_POWER_ON);

    return 0;
}

int DriverIoctlNoBuf(void) {
    TKDriverHandle handle;
    TKStatus status;

    TKInitDrivers();
    handle = TKDriverOpen(TK_TEST_MAJOR, TK_TEST_MINOR);

    status = TKDriverIoctl(handle,
                           TK_IOCTL_NONE,
                           NULL,
                           0,
                           NULL,
                           0);
    ASSERT(status == TK_OK);

    return 0;
}

int DriverIoctlInBuf(void) {
    uint8_t buf[4];
    TKDriverHandle handle;
    TKStatus status;

    TKInitDrivers();
    handle = TKDriverOpen(TK_TEST_MAJOR, TK_TEST_MINOR);

    status = TKDriverIoctl(handle,
                           TK_IOCTL_IN,
                           buf,
                           sizeof(buf),
                           NULL,
                           0);
    ASSERT(status == TK_OK);

    return 0;
}

int DriverIoctlOutBuf(void) {
    uint8_t buf[4];
    TKDriverHandle handle;
    TKStatus status;

    TKInitDrivers();
    handle = TKDriverOpen(TK_TEST_MAJOR, TK_TEST_MINOR);

    status = TKDriverIoctl(handle,
                           TK_IOCTL_OUT,
                           NULL,
                           0,
                           buf,
                           sizeof(buf));
    ASSERT(status == TK_OK);

    return 0;
}

int DriverIoctlInOutBuf(void) {
    uint8_t buf[4];
    TKDriverHandle handle;
    TKStatus status;

    TKInitDrivers();
    handle = TKDriverOpen(TK_TEST_MAJOR, TK_TEST_MINOR);

    status = TKDriverIoctl(handle,
                           TK_IOCTL_IN_OUT,
                           buf,
                           sizeof(buf),
                           buf,
                           sizeof(buf));
    ASSERT(status == TK_OK);

    return 0;
}

/**
 * Runs all the unit tests.
 */
void RunTests(void) {
    int i;
    int passCount;
    int result;
    int testCount;

    /* I'd like to make this global rather than a stack variable, but there's
     * some build issue such that compiler-initialized structs are not getting
     * initialized if they are global. I haven't figured it out yet or had the
     * time to get to the bottom of it.
     */
    struct Test Tests[] = {
        { ScheduleEmpty, "schedule with no threads" },
        { ScheduleOne, "schedule with one thread" },
        { ScheduleAllBlocked, "schedule with all threads blocked" },
        { ScheduleAllSamePriority, "schedule with all threads same priority" },
        { ScheduleHighReady, "schedule with a high priority ready thread" },
        { ScheduleHighBlockedLowReady, "schedule with high/blocked low/ready threads" },
        { ScheduleThreadWakesUp, "schedule with a thread waking up" },
        { ScheduleThreadAlmostWakesUp, "schedule with a thread close to waking" },
        { ScheduleHighWakesUpLowReady, "schedule with a high priority thread waking up while a low priority thread is ready another thread is ready" },
        { CreateThreadNullName, "create thread with NULL name" },
        { CreateThreadNameTooLong, "create a thread with a name that's too long" },
        { CreateThreadNullEntryPoint, "create a thread with a NULL entry point" },
        { CreateThreadNoFreeSlots, "create a thread when there's no free slots" },
        { CreateThreadValidateNormalCase, "create a thread and validate correct TCB entry" },
        { DriverNullOp, "do operations on NULL driver handle" },
        { DriverClosedOp, "do operations on closed handle" },
        { DriverPoweredDownOps, "do operations on a powered down driver" },
        { DriverNullStatusRead, "do read with a NULL status pointer" },
        { DriverNullStatusWrite, "do write with a NULL status pointer" },
        { DriverNullPowerState, "power state with NULL state pointer" },
        { DriverDoubleOpen, "open an already opened handle" },
        { DriverIoctlBadCode, "ioctl with unknown code" },
        { DriverIoctlBadBufferSizes, "ioctl with required buffers that are too small" },
        { DriverIoctlNullBuffers, "ioctl with required buffers that are NULL" },
        { DriverDoublePowerUp, "power up twice" },
        { DriverDoublePowerDown, "power down twice" },
        { DriverPowerStatesNotSupported, "try power operations when not supported" },
        { DriverReadWriteVerify, "write, read, and verify" },
        { DriverCloseVerify, "close a handle and verify" },
        { DriverPowerStateVerify, "verify power state transitions" },
        { DriverIoctlNoBuf, "ioctl with no buffers used" },
        { DriverIoctlInBuf, "ioctl with an in buffer" },
        { DriverIoctlOutBuf, "ioctl with an out buffer" },
        { DriverIoctlInOutBuf, "ioctl with an in and an out buffer" }
    };

    passCount = 0;
    testCount = ARRAYLEN(Tests);
    for (i = 0; i < testCount; i++) {
        TKRawPrintString(Tests[i].description);
        TKRawPrintString("... ");
        result = Tests[i].run();
        if (result == 0) {
            TKRawPrintString("ok");
            passCount++;
        }
        else {
            TKRawPrintString("fail!");
        }
        TKRawPrintString("\n");
    }

    if (passCount == testCount) {
        TKRawPrintString("All tests pass!");
    }
    else {
        TKRawPrintDecimal(passCount);
        TKRawPrintString("/");
        TKRawPrintDecimal(testCount);
        TKRawPrintString(" tests passed.");
        for (;;);
    }
    TKRawPrintString("\n");
}
