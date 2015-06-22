#ifndef __DDF_H__
#define __DDF_H__

#include <stdbool.h>

#include "lpc/lpc2378.h"

#include "tk/semaphore.h"
#include "tk/status.h"

#define TK_MAX_DRIVER_NAME_LENGTH (11)

typedef enum TKPowerState {
    TK_POWER_ON,
    TK_POWER_OFF
} TKPowerState;

/* An enum specifying what type of buffers an ioctl expects. */
enum TKIoctlType {
    TK_IOCTL_NONE,
    TK_IOCTL_IN,
    TK_IOCTL_OUT,
    TK_IOCTL_IN_OUT,
};

struct TKIoctlInfo {
    enum TKIoctlType type;
    uint32_t inSize;
    uint32_t outSize;
    TKStatus (*op)(const void * inBuf, void * outBuf);
};

struct TKDriverOps {
    void (*init)(void);
    void (*open)(void);
    void (*close)(void);
    int (*read)(TKStatus * status, uint8_t * buffer, uint32_t size);
    int (*write)(TKStatus * status, const uint8_t * buffer, uint32_t size);
    struct TKIoctlInfo * (*ioctlInfo)(uint32_t code);
    TKStatus (*powerUp)(void);
    TKStatus (*powerDown)(void);
};

struct TKDriver {
    char name[TK_MAX_DRIVER_NAME_LENGTH + 1];
    uint32_t major;
    uint32_t minor;
    TKPowerState powerstate;
    struct TKDriverOps ops;
};

struct TKDriverEntry {
    struct TKDriver * driver;
    struct TKSemaphore sem;
    bool used;
};

typedef struct TKDriverEntry * TKDriverHandle;

/**
 * Initialize the TK driver subsystem.
 * @param table a driver table
 */
void TKInitDrivers(void);

/**
 * Open a handle to a given driver.
 * @param major the major number of the driver
 * @param minor the minor number of the driver
 * @return TKDriverHandle
 *   NULL if the open failed
 *   a valid handle otherwise
 */
TKDriverHandle TKDriverOpen(uint32_t major, uint32_t minor);

/**
 * Close a handle to a given driver.
 * @param handle a driver handle
 * @return TKStatus
 * TK_OK if the operation succeeded
 * an error code otherwise
 */
TKStatus TKDriverClose(TKDriverHandle handle);

/**
 * Read from a device.
 * @param handle a driver handle
 * @param status a pointer to a TKStatus, which will receive the read status
 * @param buffer a buffer to read into
 * @param size the size of the buffer, in bytes
 * @return int
 * the number of bytes read, if successful
 * -1 otherwise
 */
int TKDriverRead(TKDriverHandle handle,
                 TKStatus * status,
                 uint8_t * buffer,
                 uint32_t size);

/**
 * Write to a device.
 * @param handle a driver handle
 * @param status a pointer to a TKStatus, which will receive the read status
 * @param buffer a buffer containing data to be written
 * @param size the size of the buffer, in bytes
 * @return int
 * the number of bytes read, if successful
 * -1 otherwise
 */
int TKDriverWrite(TKDriverHandle handle,
                  TKStatus * status,
                  const uint8_t * buffer,
                  uint32_t size);

/**
 * Issue an ioctl (special control code).
 * @param handle a driver handle
 * @param code the ioctl code; this is driver-specific
 * @param inBuf a constant buffer for ioctl input; could be NULL
 * @param inSize the size of inBuf, in bytes
 * @param outBuf a buffer to receive ioctl output; could be NULL
 * @param outSize the size of outBuf, in bytes
 * @return TKStatus
 * TK_OK if the operation succeeded
 * an error code otherwise
 */
TKStatus TKDriverIoctl(TKDriverHandle handle,
                       uint32_t code,
                       const void * inBuf,
                       uint32_t inSize,
                       void * outBuf,
                       uint32_t outSize);

/**
 * Power up a device.
 * @param handle a driver handle
 * @return TKStatus
 * TK_OK if the operation succeeded
 * an error code otherwise
 */
TKStatus TKDriverPowerUp(TKDriverHandle handle);

/**
 * Power down a device.
 * @param handle a driver handle
 * @return TKStatus
 * TK_OK if the operation succeeded
 * an error code otherwise
 */
TKStatus TKDriverPowerDown(TKDriverHandle handle);

/**
 * Get the current power state of a device.
 * @param handle a driver handle
 * @param state a power to a TKPowerState
 * @return TKStatus
 * TK_OK if the operation is supported
 * an error code otherwise
 */
TKStatus TKDriverPowerState(TKDriverHandle handle, TKPowerState * state);

#endif
