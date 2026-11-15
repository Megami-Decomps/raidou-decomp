#include "common.h"
#include "sdk_libcdvd.h"
#include "ps2asm.h"

typedef void (*cdvdCallback_t)(s32 reason);
extern s32 cdvdDebugLevel __attribute__((section(".data")));

typedef struct cdvdRpcClient_t {
    u8 transactionAndCallbacks[0x24];
    void *server;
} cdvdRpcClient_t;

extern cdvdRpcClient_t cdvdScmdClient __attribute__((section(".data")));
extern s32 cdvdScmdLockSema __attribute__((section(".data")));
extern s32 cdvdScmdReply[4] __attribute__((section(".data")));

typedef struct sdfSemaParam_t {
    s32 count;
    s32 maxCount;
    s32 initCount;
    s32 waitThreads;
    u32 attr;
    u32 option;
} sdfSemaParam_t;

/* Same native CreateThread descriptor as the matched SDF constructor. */
typedef struct {
    s32 status;
    void (*entry)(void);
    void *stack;
    s32 stackSize;
    void *gp;
    s32 priority;
    u8 unk18[0x18];
} sdfKernelThreadParam_t;

#include "sdk_libkernl.h"

/* Consumed transfer view: payloads begin at 0x10 and 0x50. The trailing
 * array leaves the second payload extent to the producer; no total size is known. */
typedef struct {
    s32 firstLength;
    s32 secondLength;
    u8 *firstDestination;
    u8 *secondDestination;
    u8 firstData[0x40];
    u8 secondData[0];
} cdvdReadReplyPrefix_t;

typedef void (*cdvdPowerCallback_t)(void *argument);

#define UNCACHED(a) ((s32 *)(EE_ADDR(a) | 0x20000000))

extern s32 SignalSema(s32 sema);

extern s32 sceSifCallRpc(void *cd, u32 fno, u32 mode, void *send, s32 ssize,
                         void *recv, s32 rsize, void *endfunc, void *endpara);

s32 sceCdStatus(void);

typedef struct sdfCdClock_t {
    u8 status;
    u8 second;
    u8 minute;
    u8 hour;
    u8 pad04;
    u8 day;
    u8 month;
    u8 year;
} sdfCdClock_t;

typedef struct cdvdStreamRequest_t {
    u32 lsn;
    u32 sectors;
    void *buffer;
    s32 command;
    u8 mode[3];
    u8 opaque13;
} cdvdStreamRequest_t;

s32 sceCdStatus(void)
{
    s32 ret;

    if (!_sceCd_scmd_prechk(2)) {
        return -1;
    }
    if (sceSifCallRpc(&cdvdScmdClient, 12, 0, 0, 0, cdvdScmdReply, 4, 0, 0) < 0) {
        SignalSema(cdvdScmdLockSema);
        return -1;
    }
    ret = UNCACHED(cdvdScmdReply)[0];
    SignalSema(cdvdScmdLockSema);
    if (cdvdDebugLevel >= 2) {
        scePrintf("status called\n");
    }
    return ret;
}
