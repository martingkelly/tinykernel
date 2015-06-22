#ifndef __CRITICAL_SECTION_H__
#define __CRITICAL_SECTION_H__

#include "lpc/lpc2378.h"

#include "tk/status.h"

/* Note that this API is not exactly the same as the one described
 * in the assignment description. The reasons for the changes are:
 *
 * - AFAICT, the CreateCriticalSection API doesn't quite work as described
 *   because there's no way to provide the backing storage for it. I think
 *   it needs to either:
 *     (a) Take in a pointer to a CriticalSection
 *     (b) Return a newly allocated CriticalSection, and implement allocation
 *         logic similar to malloc
 *   I opted for option A, as it's simpler, less error-prone, and works just
 *   as well. That meant I needed to make the functions take pointer paremeters
 *   and that the DeleteCriticalSection function was a no-ops.
 */

/** Container for critical section internal data */
struct TKCriticalSection {
	volatile uint32_t lock;
	volatile struct TKThread * thread;
	uint32_t count;
};

/**
* Setup a critical section for later use
*
* @param cs Critical section container
* @return 0 Critical section is ready for use
* @return non-zero An error code
*/
TKStatus TKCreateCriticalSection(struct TKCriticalSection * cs);

/**
* Take the critical section for this thread. If it is not available then
* block until it is.
* This may be entered multiple times within the same thread.
* The critical section will not be released until the
* corresponding number of LeavecriticalSection() calls have
* been made within the same thread.
*
* @param cs Critical section container
* @param thread A thread pointer
* @return 0 Critical section was taken
* @return non-zero An error code
*/
TKStatus _TKEnterCriticalSection(struct TKCriticalSection * cs,
                            struct TKThread * thread);
TKStatus TKEnterCriticalSection(struct TKCriticalSection * cs);

/**
* Release the critical section for this thread.
*
* @param cs Critical section container
* @param thread A thread pointer
* @return 0 Critical section was released
* @return non-zero An error code
*/
TKStatus _TKLeaveCriticalSection(struct TKCriticalSection * cs,
                            struct TKThread * thread);
TKStatus TKLeaveCriticalSection(struct TKCriticalSection * cs);

/**
* Take the critical section for this thread if it is available.
* The call will return immediately if critical section is unavailable
* with the error code BUSY
*
* @param cs Critical section container
* @param thread A thread pointer
* @return 0 Critical section was taken
* @return BUSY Critical section is held by another thread
* @return non-zero An error code
*/
TKStatus _TKQueryEnterCriticalSection(struct TKCriticalSection * cs,
                                 struct TKThread * thread);
TKStatus TKQueryEnterCriticalSection(struct TKCriticalSection * cs);

#endif
