#include <stddef.h>

#include "tk/common.h"
#include "tk/ddf.h"

#include "tk/drivers/serial.h"
#include "tk/drivers/test.h"

static struct TKDriverEntry DriverTable[2];

void TKInitDrivers(void) {
    
    int i;

    /* Why don't we statically initialize this at the top of the file? Because
     * of the strange bug with compiler-initialized structs not getting
     * initialized if they're global. I'd really like to solve this one when I
     * get more time.
     */
    DriverTable[0].driver = &TKTestDriver;
    DriverTable[1].driver = &TKSerialDriver;
    TKSerialDriver.ops.init = TKSerialDriverInit;
    TKTestDriver.ops.init = TKTestDriverInit;

    for (i = 0; i < ARRAYLEN(DriverTable); i++) {
        DriverTable[i].driver->ops.init();
        DriverTable[i].used = false;
        TKCreateSemaphore(&DriverTable[i].sem, 1);
    }
}

TKDriverHandle TKDriverOpen(uint32_t major, uint32_t minor) {
    struct TKDriverEntry * entry;
    int i;

    for (i = 0; i < ARRAYLEN(DriverTable); i++) {
        entry = &DriverTable[i];
        if (entry->driver->major != major) {
            continue;
        }
        if (entry->driver->minor != minor) {
            continue;
        }
        break;
    }
    if (i == ARRAYLEN(DriverTable)) {
        return NULL;
    }

    if (entry->used) {
        return NULL;
    }

    entry->driver->ops.open();
    entry->used = true;

    return entry;
}

TKStatus TKDriverClose(TKDriverHandle handle) {
    int i;
    struct TKDriverEntry * entry;

    if (handle == NULL) {
        return TK_NULL;
    }

    if (!handle->used) {
        return TK_CLOSED;
    }

    for (i = 0; i < ARRAYLEN(DriverTable); i++) {
        entry = &DriverTable[i];
        if (entry->driver == handle->driver) {
            break;
        }
    }
    if (i == ARRAYLEN(DriverTable)) {
        /* Driver not found in table; bad handle. */
        return TK_UNEXPECTED;
    }

    TKDownSemaphore(&handle->sem);
    handle->driver->ops.close();
    TKUpSemaphore(&handle->sem);

    entry->used = false;

    return TK_OK;
}

int TKDriverRead(TKDriverHandle handle,
                 TKStatus * status,
                 uint8_t * buffer,
                 uint32_t size) {
    int ret;

    if (status == NULL) {
        return -1;
    }
    if (handle == NULL || buffer == NULL) {
        *status = TK_NULL;
        return -1;
    }

    if (!handle->used) {
        *status = TK_CLOSED;
        return -1;
    }

    if (handle->driver->powerstate == TK_POWER_OFF) {
        *status = TK_NO_POWER;
        return -1;
    }

    TKDownSemaphore(&handle->sem);
    ret = handle->driver->ops.read(status, buffer, size);
    TKUpSemaphore(&handle->sem);

    return ret;
}

int TKDriverWrite(TKDriverHandle handle,
                  TKStatus * status,
                  const uint8_t * buffer,
                  uint32_t size) {
    int ret;

    if (status == NULL) {
        return -1;
    }
    if (handle == NULL || buffer == NULL) {
        *status = TK_NULL;
        return -1;
    }

    if (!handle->used) {
        *status = TK_CLOSED;
        return -1;
    }

    if (handle->driver->powerstate == TK_POWER_OFF) {
        *status = TK_NO_POWER;
        return -1;
    }

    TKDownSemaphore(&handle->sem);
    ret = handle->driver->ops.write(status, buffer, size);
    TKUpSemaphore(&handle->sem);

    return ret;
}

TKStatus TKDriverIoctl(TKDriverHandle handle,
                       uint32_t code,
                       const void * inBuf,
                       uint32_t inSize,
                       void * outBuf,
                       uint32_t outSize) {
    struct TKIoctlInfo * ioctlInfo;
    TKStatus status;

    if (handle == NULL) {
        return TK_NULL;
    }

    if (!handle->used) {
        return TK_CLOSED;
    }

    if (handle->driver->powerstate == TK_POWER_OFF) {
        return TK_NO_POWER;
    }

    /* Validate ioctl code. */
    ioctlInfo = handle->driver->ops.ioctlInfo(code);
    if (ioctlInfo == NULL) {
        return TK_IOCTL_BAD_CODE;
    }

    /* Validate ioctl parameters. */
    switch (ioctlInfo->type) {
    case TK_IOCTL_NONE:
        break;
    case TK_IOCTL_IN:
        if (inBuf == NULL) {
            return TK_IOCTL_IN_BUF_NULL;
        }
        if (inSize != ioctlInfo->inSize) {
            return TK_IOCTL_IN_BUF_BAD_SIZE;
        }
        break;
    case TK_IOCTL_OUT:
        if (outBuf == NULL) {
            return TK_IOCTL_OUT_BUF_NULL;
        }
        if (outSize != ioctlInfo->outSize) {
            return TK_IOCTL_OUT_BUF_BAD_SIZE;
        }
        break;
    case TK_IOCTL_IN_OUT:
        if (inBuf == NULL) {
            return TK_IOCTL_IN_BUF_NULL;
        }
        if (inSize != ioctlInfo->inSize) {
            return TK_IOCTL_IN_BUF_BAD_SIZE;
        }
        if (outBuf == NULL) {
            return TK_IOCTL_OUT_BUF_NULL;
        }
        if (outSize != ioctlInfo->outSize) {
            return TK_IOCTL_OUT_BUF_BAD_SIZE;
        }
        break;
    }

    TKDownSemaphore(&handle->sem);
    status = ioctlInfo->op(inBuf, outBuf);
    TKUpSemaphore(&handle->sem);

    return status;
}

TKStatus TKDriverPowerUp(TKDriverHandle handle) {
    TKStatus status;

    if (handle == NULL) {
        return TK_NULL;
    }

    if (!handle->used) {
        return TK_CLOSED;
    }

    if (handle->driver->powerstate == TK_POWER_ON) {
        return TK_ALREADY_POWERED_ON;
    }

    if (handle->driver->ops.powerUp == NULL) {
        return TK_POWER_STATES_UNSUPPORTED;
    }

    TKDownSemaphore(&handle->sem);
    status = handle->driver->ops.powerUp();
    TKUpSemaphore(&handle->sem);

    if (status == TK_OK) {
        handle->driver->powerstate = TK_POWER_ON;
    }

    return status;
}

TKStatus TKDriverPowerDown(TKDriverHandle handle) {
    TKStatus status;

    if (handle == NULL) {
        return TK_NULL;
    }

    if (!handle->used) {
        return TK_CLOSED;
    }

    if (handle->driver->powerstate == TK_POWER_OFF) {
        return TK_ALREADY_POWERED_OFF;
    }

    if (handle->driver->ops.powerDown == NULL) {
        return TK_POWER_STATES_UNSUPPORTED;
    }

    TKDownSemaphore(&handle->sem);
    status = handle->driver->ops.powerDown();
    TKUpSemaphore(&handle->sem);

    if (status == TK_OK) {
        handle->driver->powerstate = TK_POWER_OFF;
    }

    return status;
}

TKStatus TKDriverPowerState(TKDriverHandle handle, TKPowerState * state) {
    if (handle == NULL || state == NULL) {
        return TK_NULL;
    }

    if (!handle->used) {
        return TK_CLOSED;
    }

    if (handle->driver->ops.powerUp == NULL ||
        handle->driver->ops.powerDown == NULL) {
        return TK_POWER_STATES_UNSUPPORTED;
    }

    *state = handle->driver->powerstate;

    return TK_OK;
}
