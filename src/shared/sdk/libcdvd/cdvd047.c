#include "common.h"
#include "sdk_libcdvd.h"
#include "ps2asm.h"

extern s32 cdvdStreamActive __attribute__((section(".data")));
typedef void (*cdvdCallback_t)(s32 reason);
extern u8 cdvdDefaultStreamMode[] __attribute__((section(".data")));
extern s32 cdvdDebugLevel __attribute__((section(".data")));

typedef struct cdvdRpcClient_t {
    u8 transactionAndCallbacks[0x24];
    void *server;
} cdvdRpcClient_t;

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

extern int scePrintf(const char *fmt, ...);
extern s32 sceCdStream(u32 lsn, u32 sectors, void *buf, s32 cmd, void *arg);

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

s32 sceCdStInit(u32 bufmax, u32 bankmax, void *buf);
s32 sceCdStStart(u32 lsn, void *mode);
s32 sceCdStSeekF(u32 lsn);
s32 sceCdStSeek(u32 lsn);
s32 sceCdStStop(void);
s32 sceCdStPause(void);
s32 sceCdStResume(void);
s32 sceCdStStat(void);

typedef struct cdvdStreamRequest_t {
    u32 lsn;
    u32 sectors;
    void *buffer;
    s32 command;
    u8 mode[3];
    u8 opaque13;
} cdvdStreamRequest_t;

s32 sceCdStream(u32 lsn, u32 sectors, void *buf, s32 cmd, void *arg);

s32 sceCdStInit(u32 bufmax, u32 bankmax, void *buf)
{
    cdvdStreamActive = 0;
    return sceCdStream(bufmax, bankmax, buf, 5, cdvdDefaultStreamMode);
}

s32 sceCdStStart(u32 lsn, void *mode)
{
    cdvdStreamActive = 1;
    return sceCdStream(lsn, 0, 0, 1, mode);
}

s32 sceCdStSeekF(u32 lsn)
{
    return sceCdStream(lsn, 0, 0, 9, cdvdDefaultStreamMode);
}

s32 sceCdStSeek(u32 lsn)
{
    return sceCdStream(lsn, 0, 0, 4, cdvdDefaultStreamMode);
}

s32 sceCdStStop(void)
{
    cdvdStreamActive = 0;
    return sceCdStream(0, 0, 0, 3, cdvdDefaultStreamMode);
}

INCLUDE_ASM(const s32, "sdk/libcdvd/cdvd047", sceCdStRead);

s32 sceCdStPause(void)
{
    cdvdStreamActive = 0;
    if (cdvdDebugLevel > 0) {
        scePrintf("sceCdStPause call\n");
    }
    return sceCdStream(0, 0, 0, 7, cdvdDefaultStreamMode);
}

s32 sceCdStResume(void)
{
    cdvdStreamActive = 1;
    if (cdvdDebugLevel > 0) {
        scePrintf("sceCdStResume call\n");
    }
    return sceCdStream(0, 0, 0, 8, cdvdDefaultStreamMode);
}

s32 sceCdStStat(void)
{
    if (cdvdDebugLevel > 0) {
        scePrintf("sceCdStStat call\n");
    }
    return sceCdStream(0, 0, 0, 6, cdvdDefaultStreamMode);
}
