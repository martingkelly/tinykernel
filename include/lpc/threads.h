#ifndef __THREADS_H__
#define __THREADS_H__

#include "tk/critical_section.h"
#include "tk/semaphore.h"
#include "tk/thread.h"

struct ThreadData {
    struct TKSemaphore sem;
	uint8_t inc;
	uint32_t t1;
	uint32_t t2;
};

/**
 * Producer thread
 */
TKThreadEntryType producer;

/**
 * Consumer thread
 */
TKThreadEntryType consumer;

/**
 * Monitor thread
 */
TKThreadEntryType monitor;

/**
 * Idle thread
 */
TKThreadEntryType idle;

#endif
