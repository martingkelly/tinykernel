/* Test driver. This driver does minimal operations to prove out the DDF. */

#include <stddef.h>
#include <string.h>

#include "lpc/lpc2378.h"

#include "tk/ddf.h"
#include "tk/status.h"

#include "tk/drivers/test.h"

static uint8_t buf[4];

static void Open(void) {
    return;
}

static void Close(void) {
    return;
}

static int Read(TKStatus * status, uint8_t * buffer, uint32_t size) {
    memcpy(buffer, buf, size);

    *status = TK_OK;
    return size;
}

static int Write(TKStatus * status, const uint8_t * buffer, uint32_t size) {
    memcpy(buf, buffer, size);

    *status = TK_OK;
    return size;
}

static TKStatus PowerUp(void) {
    return TK_OK;
}

static TKStatus PowerDown(void) {
    return TK_OK;
}

/* Ioctl */
static struct TKIoctlInfo IoctlNone;
static struct TKIoctlInfo IoctlIn;
static struct TKIoctlInfo IoctlOut;
static struct TKIoctlInfo IoctlInOut;

static struct TKIoctlInfo * IoctlInfo(uint32_t code) {
    switch (code) {
        case TK_TEST_IOCTL_NONE:
            return &IoctlNone;
        case TK_TEST_IOCTL_IN:
            return &IoctlIn;
        case TK_TEST_IOCTL_OUT:
            return &IoctlOut;
        case TK_TEST_IOCTL_IN_OUT:
            return &IoctlInOut;
        default:
            return NULL;
    }
}

static TKStatus IoctlNoneOp(const void * inBuf, void * outBuf) {
    return TK_OK;
}

static TKStatus IoctlInOp(const void * inBuf, void * outBuf) {
    return TK_OK;
}

static TKStatus IoctlOutOp(const void * inBuf, void * outBuf) {
    return TK_OK;
}

static TKStatus IoctlInOutOp(const void * inBuf, void * outBuf) {
    return TK_OK;
}

void TKTestDriverInit(void) {
    strcpy(TKTestDriver.name, "test");
    TKTestDriver.major = TK_TEST_MAJOR;
    TKTestDriver.minor = TK_TEST_MINOR;
    TKTestDriver.powerstate = TK_POWER_ON;

    TKTestDriver.ops.init = TKTestDriverInit;
    TKTestDriver.ops.open = Open;
    TKTestDriver.ops.close = Close;
    TKTestDriver.ops.read = Read;
    TKTestDriver.ops.write = Write;
    TKTestDriver.ops.ioctlInfo = IoctlInfo;
    TKTestDriver.ops.powerUp = PowerUp;
    TKTestDriver.ops.powerDown = PowerDown;

    IoctlNone.type = TK_IOCTL_NONE;
    IoctlNone.inSize = 0;
    IoctlNone.outSize = 0;
    IoctlNone.op = IoctlNoneOp;

    IoctlIn.type = TK_IOCTL_IN;
    IoctlIn.inSize = 4;
    IoctlIn.outSize = 0;
    IoctlIn.op = IoctlInOp;

    IoctlOut.type = TK_IOCTL_OUT;
    IoctlOut.inSize = 0;
    IoctlOut.outSize = 4;
    IoctlOut.op = IoctlOutOp;

    IoctlInOut.type = TK_IOCTL_IN_OUT;
    IoctlInOut.inSize = 4;
    IoctlInOut.outSize = 4;
    IoctlInOut.op = IoctlInOutOp;
}
