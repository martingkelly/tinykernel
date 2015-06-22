#ifndef __TK_THREAD_H__
#define __TK_THREAD_H__

#include "lpc/lpc2378.h"

#include "tk/critical_section.h"
#include "tk/status.h"
#include "tk/timing.h"

#define TK_MAX_THREAD_NAME_LENGTH (11)
#define TK_PRIORITY_LOWEST (1)
#define TK_PRIORITY_NORMAL (127)
#define TK_PRIORITY_HIGHEST (254)
#define TK_STACK_SIZE (256)

typedef uint8_t TKThreadPriority;
typedef uint32_t TKThreadStatus;
typedef void TKThreadEntryType(void * p);
typedef TKThreadEntryType * TKThreadEntry;

struct TKThreadQueue {
    struct TKThread * head;
};

struct TKThread {
    /* It is important that the stack pointer comes first because the context
     * switching code will use the global current thread pointer as a stack
     * pointer. In this way, the C code can set current thread (and see all its
     * attributes) and the context switching assembly can use the same value.
     */
    void * stackPointer;

    TKTickCount sleepTarget;
	TKThreadPriority priority;
    int stack[TK_STACK_SIZE];
	char name[TK_MAX_THREAD_NAME_LENGTH + 1];
    struct TKThreadQueue * queue;
    struct TKThread * prev;
    struct TKThread * next;
};

/**
 * Initialize the global data used for threads.
 *
 */
void TKInitThreadData(void);

/**
 * Print metrics about scheduling performance.
 */
void TKPrintSchedulingMetrics(void);

/**
 * Add a thread to a thread queue.
 *
 * @param queue a thread queue pointer
 * @param thread a thread pointer
 */
void TKAddThread(struct TKThreadQueue * queue,
                 struct TKThread * thread);


/**
 * Pop a thread from a thread queue.
 *
 * @param queue a thread queue pointer
 * @return struct TKThread *
 * @return NULL if the queue was empty
 */
struct TKThread * TKPopThread(struct TKThreadQueue * queue);

/**
 * Remove a thread from the queue on which it resides.
 *
 * @param a thread pointer to remove
 * @return int
 *   0 if the thread was removed
 *   -1 if the thread was not part of a queue
 */
int TKRemoveThread(struct TKThread * thread);

/**
 * Enter the scheduler to pick a task and switch to it. This routine is called
 * from the timer tick ISR.
 *
 */
void TKSchedule(void);
struct TKThread * _TKSchedule(struct TKThreadQueue * runQueue,
                              struct TKThreadQueue * sleepQueue,
                              TKTickCount tickCount);

/**
 * Make a thread scheduling decision
 *
 * @param queue a thread queue pointer
 * @return TKThread the next thread to be run
 */
struct TKThread * TKPickThread(struct TKThreadQueue * queue);

/**
 * Create a thread (add it to the thread queue)
 *
 * @param name a null-terminated string description of the thread
 * @param priority the thread priority
 * @param entry the thread entry point
 * @param data thread data
 * @return TK_OK if successful
 * @return TK_NULL if the passed-in entry point is NULL
 * @return TK_NAME_TOO_LONG if the thread name is longer than
 *                          TK_MAX_THREAD_NAME_LENGTH
 * @return TK_QUEUE_FULL if there's no free queue space left
 */
TKStatus TKCreateThread(const char * name,
		                TKThreadPriority priority,
		                TKThreadEntry entry,
		                void * data);
TKStatus _TKCreateThread(struct TKThreadQueue * freeQueue,
                         struct TKThreadQueue * usedQueue,
		                 const char * name,
		                 TKThreadPriority priority,
		                 TKThreadEntry entry,
		                 void * data);

/*
 * Yield so that another thread can be scheduled.
 */
void TKYieldThread(void);
void _TKYieldThread(struct TKThread * thead);

/*
 * Suspend a thread for some set time.
 * @param seconds number of seconds to suspend
 */
void TKThreadSleep(uint32_t seconds);
void _TKThreadSleep(struct TKThreadQueue * runQueue,
                    struct TKThreadQueue * sleepQueue,
                    struct TKThread * thread,
                    uint32_t seconds);

#endif
