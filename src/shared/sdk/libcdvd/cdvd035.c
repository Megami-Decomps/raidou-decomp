#include "common.h"

/* Declarations from smt3-decomp src/smt3/sdk/libcdvd.c */
#include "sdk_libcdvd.h"

#include "common.h"

#include "ps2asm.h"

extern s32 cdvdStreamActive __attribute__((section(".data")));

typedef void (*cdvdCallback_t)(s32 reason);

extern cdvdCallback_t cdvdCallback __attribute__((section(".data")));

extern u8 cdvdDefaultStreamMode[] __attribute__((section(".data")));

extern s32 cdvdDebugLevel __attribute__((section(".data")));

typedef struct cdvdRpcClient_t {
    u8 transactionAndCallbacks[0x24];
    void *server;
} cdvdRpcClient_t;

extern cdvdRpcClient_t cdvdScmdClient __attribute__((section(".data")));

extern s32 cdvdNcmdState __attribute__((section(".data")));

extern s32 cdvdScmdLockSema __attribute__((section(".data")));

extern cdvdRpcClient_t cdvdNcmdClient __attribute__((section(".data")));

extern s32 cdvdMmodeArg __attribute__((section(".data")));

extern s32 cdvdScmdReply[4] __attribute__((section(".data")));

typedef struct sdfSemaParam_t {
    s32 count;
    s32 maxCount;
    s32 initCount;
    s32 waitThreads;
    u32 attr;
    u32 option;
} sdfSemaParam_t;

extern s32 CreateSema(sdfSemaParam_t *param);

extern s32 iSignalSema(s32 sema);

extern s32 WaitSema(s32 sema);

extern s32 DeleteSema(s32 sema);

extern s32 SetAlarm(u16 ticks, void (*callback)(s32, s32, s32), void *argument);

extern void gcc2_compiled__6(s32 cause, s32 address, s32 argument);

extern s32 cdvdNcmdSema __attribute__((section(".data")));

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

extern s32 cdvdCallbackThreadId __attribute__((section(".data")));

extern s32 cdvdOwnerThreadId __attribute__((section(".data")));

extern u8 cdvdThreadStatus[0x30] __attribute__((section(".data")));

extern sdfKernelThreadParam_t cdvdCallbackThreadParam __attribute__((section(".data")));

extern s32 _gp;

extern s32 GetThreadId(void);

extern s32 ReferThreadStatus(s32 threadId, void *status);

extern s32 CreateThread(sdfKernelThreadParam_t *param);

#include "sdk_libkernl.h"

extern s32 ChangeThreadPriority(s32 threadId, s32 priority);

void _Cdvd_cbLoop(void);

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

extern s32 cdvdCallbackReason __attribute__((section(".data")));

void _sceCd_cd_callback(s32 *reason);

typedef void (*cdvdPowerCallback_t)(void *argument);

extern cdvdPowerCallback_t cdvdPowerCallback __attribute__((section(".data")));

extern void *cdvdPowerArgument __attribute__((section(".data")));

extern s32 cdvdPowerOffActive __attribute__((section(".data")));

extern s32 cdvdScmdState[6] __attribute__((section(".data")));

extern void sceSifAddCmdHandler(s32 id, void (*handler)(void *, void *), void *argument);

void _sceCd_Poff_Intr(void *packet, void *argument);

s32 PowerOffCB(void);

#define UNCACHED(a) ((s32 *)(EE_ADDR(a) | 0x20000000))

extern s32 DIntr(void);

extern s32 SignalSema(s32 sema);

extern void sceSifWriteBackDCache(void *addr, s32 size);

extern s32 sceSifCallRpc(void *cd, u32 fno, u32 mode, void *send, s32 ssize,
                         void *recv, s32 rsize, void *endfunc, void *endpara);

extern s32 _sceCd_scmd_prechk(s32 cmd);

extern int scePrintf(const char *fmt, ...);

extern s32 sceSifCheckStatRpc(void *cd);

extern void sceCdDelayThread(u16 ticks);

extern s32 EIntr(void);

extern s32 sceCdSync(s32 mode);

extern s32 sceCdStream(u32 lsn, u32 sectors, void *buf, s32 cmd, void *arg);

extern s32 sceCdLayerSearchFile(addr_t file, const char *name, s32 layer);

void gcc2_compiled__6(s32 cause, s32 address, s32 argument);

void sceCdDelayThread(u16 ticks);

cdvdCallback_t sceCdCallback(cdvdCallback_t func);

s32 sceCdInitEeCB(s32 priority, void *stack, s32 stackSize);

void _sceCd_cd_read_intr(void *argument);

extern s32 cdvdScmdSema __attribute__((section(".data")));

extern void sceSifRemoveCmdHandler(s32 command);

void cmd_sem_init(void);

void cdvd_exit(void);

cdvdPowerCallback_t sceCdPOffCallback(cdvdPowerCallback_t callback, void *argument);

void _sceCd_Poff_Intr(void *packet, void *argument);

s32 PowerOffCB(void);

s32 sceCdSearchFile(sceCdlFILE *file, const char *name);

extern void cmd_sem_init(void);

extern s32 PollSema(s32 sema);

extern void sceSifInitRpc(s32 mode);

extern s32 sceSifBindRpc(void *client, u32 service, s32 mode);

extern s32 cdvdNcmdLastCommand __attribute__((section(".data")));

extern s32 cdvdNcmdInit;

s32 _sceCd_ncmd_prechk(s32 command);

extern s32 _sceCd_ncmd_prechk(s32 cmd);

extern s32 cdvdNcmdReply __attribute__((section(".data")));

s32 sceCdNcmdDiskReady(void);

s32 sceCdSync(s32 mode);

s32 sceCdSyncS(s32 mode);

extern void cmd_sem_init(void);

extern s32 PollSema(s32 sema);

extern void sceSifInitRpc(s32 mode);

extern s32 sceSifBindRpc(void *client, u32 service, s32 mode);

extern s32 cdvdScmdLastCommand __attribute__((section(".data")));

extern s32 cdvdScmdInit;

s32 _sceCd_scmd_prechk(s32 command);

s32 sceCdMmode(s32 media);

s32 sceCdGetDiskType(void);

s32 sceCdStatus(void);

s32 sceCdPowerOff(s32 *result);

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

extern const char cdvdMsgClockRead1[32];

extern const char cdvdMsgClockRead2[32];

s32 sceCdReadClock(sdfCdClock_t *clock);

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

extern cdvdStreamRequest_t cdvdStreamRequest __attribute__((section(".data")));

extern s32 _sceCd_ncmd_prechk(s32 command);

s32 sceCdStream(u32 lsn, u32 sectors, void *buf, s32 cmd, void *arg);
/* End of declarations from src/smt3/sdk/libcdvd.c */

s32 sceCdPowerOff(s32 *result)
{
    s32 ret;

    if (!_sceCd_scmd_prechk(33)) {
        return 0;
    }
    if (sceSifCallRpc(&cdvdScmdClient, 33, 0, 0, 0, cdvdScmdReply, 8, 0, 0) < 0) {
        SignalSema(cdvdScmdLockSema);
        return 0;
    }
    *result = UNCACHED(cdvdScmdReply + 1)[0];
    ret = UNCACHED(cdvdScmdReply)[0];
    SignalSema(cdvdScmdLockSema);
    return ret;
}
