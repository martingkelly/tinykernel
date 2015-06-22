#ifndef __TK_DATA_H__
#define __TK_DATA_H__

#include "tk/timing.h"
#include "tk/thread.h"

struct TKThreadQueue FreeQueue;
struct TKThreadQueue RunQueue;
struct TKThreadQueue SleepQueue;

struct TKThread * CurrentThread;
uint32_t TickHz;
volatile TKTickCount TickCount;

/**
 * Initialize the global kernel data.
 *
 */
void TKInitKernelData(void);

#endif
