#ifndef __TK_TEST_DRIVER_H__
#define __TK_TEST_DRIVER_H__

#define TK_TEST_MAJOR (0)
#define TK_TEST_MINOR (0)

#define TK_TEST_IOCTL_NONE (0)
#define TK_TEST_IOCTL_IN (1)
#define TK_TEST_IOCTL_OUT (2)
#define TK_TEST_IOCTL_IN_OUT (3)

struct TKDriver TKTestDriver;

void TKTestDriverInit(void);

#endif
