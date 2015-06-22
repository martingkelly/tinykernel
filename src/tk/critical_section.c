#include <stdlib.h>

#include "tk/critical_section.h"
#include "tk/data.h"

uint32_t TKTrySet(volatile uint32_t * p, uint32_t value ) {
	uint32_t result;

	__asm__ __volatile__ (
		"swp %[result], %[value], [%[p]]"
		:[result] "=&r" (result)
		:[p] "r" (p), [value] "r" (value)
		: "cc", "memory"
	);

	return result;
}

TKStatus TKCreateCriticalSection(struct TKCriticalSection * cs) {
	if (cs == NULL) {
		return TK_UNEXPECTED;
	}

	cs->lock = 0;
	cs->thread = 0;
	cs->count = 0;

	return TK_OK;
}

TKStatus _TKEnterCriticalSection(struct TKCriticalSection * cs,
                                 struct TKThread * thread) {
	uint32_t result;

	if (cs == NULL) {
		return TK_UNEXPECTED;
	}

	if (thread == NULL) {
		return TK_UNEXPECTED;
	}

	/* Check if we currently hold the lock.
	 * If so, just increment the counter and exit.
	 */
	if (cs->thread == thread) {
		cs->count++;
		return TK_OK;
	}

	/* We don't hold the lock; get it. */
	while (1) {
		result = TKTrySet(&cs->lock, 1);
        if (result != 0) {
            TKYieldThread();
        }
        else {
            break;
        }
	}

	/* Got the lock */
	cs->thread = thread;
	cs->count = 1;

	return TK_OK;
}

TKStatus TKEnterCriticalSection(struct TKCriticalSection * cs) {
    return _TKEnterCriticalSection(cs, CurrentThread);
}

TKStatus _TKLeaveCriticalSection(struct TKCriticalSection * cs,
                                 struct TKThread * thread) {
	uint32_t result;

	if (cs == NULL) {
		return TK_UNEXPECTED;
	}

	if (thread == NULL) {
		return TK_UNEXPECTED;
	}

	/* Check if we currently hold the lock. If not, exit */
	if (cs->thread != thread) {
		return TK_UNEXPECTED;
	}

	/* We hold the lock; decrement the counter */
	cs->count--;

	/* If the counter is now 0, release the lock */
	if (cs->count == 0) {
		cs->thread = 0;
		result = TKTrySet(&cs->lock, 0);
		/* Since we hold the lock, the release should always work */
		if (result != 1)
		{
			return TK_UNEXPECTED;
		}
	}

	return TK_OK;
}

TKStatus TKLeaveCriticalSection(struct TKCriticalSection * cs) {
    return _TKLeaveCriticalSection(cs, CurrentThread);
}

TKStatus _TKQueryEnterCriticalSection(struct TKCriticalSection * cs,
                                      struct TKThread * thread) {
	uint32_t result;

	if (cs == NULL) {
		return TK_UNEXPECTED;
	}

	if (thread == NULL) {
		return TK_UNEXPECTED;
	}

	/* Check if we currently hold the lock.
	 * If so, just increment the counter and exit.
	 */
	if (cs->thread == thread) {
		cs->count++;
		return TK_OK;
	}

	/* We don't hold the lock; try to get it */
	result = TKTrySet(&cs->lock, 1);
	if (result != 0) {
		return TK_BUSY;
	}

	/* We got the lock */
	cs->thread = thread;
	cs->count = 1;

	return TK_OK;
}

TKStatus TKQueryEnterCriticalSection(struct TKCriticalSection * cs) {
    return _TKQueryEnterCriticalSection(cs, CurrentThread);
}
