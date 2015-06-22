#ifndef __COMMON_H__
#define __COMMON_H__

#define ARM_MODE_ARM 0x00000000
#define ARM_SVC_MODE_ARM (0x00000013L + ARM_MODE_ARM)

#define ARRAYLEN(x) (sizeof(x) / sizeof(x[0]))

#endif
