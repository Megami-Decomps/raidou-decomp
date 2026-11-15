#include "common.h"
#include "sdk_libcdvd.h"
#include "ps2asm.h"
#include "sif_rpc.h"
extern s32 DIntr(void);
extern s32 EIntr(void);

typedef void (*cdvdCallback_t)(s32 reason);
extern cdvdCallback_t cdvdCallback __attribute__((section(".data")));
extern s32 cdvdDebugLevel __attribute__((section(".data")));

typedef struct cdvdRpcClient_t {
    u8 transactionAndCallbacks[0x24];
    void *server;
} cdvdRpcClient_t;

extern cdvdRpcClient_t cdvdScmdClient __attribute__((section(".data")));
extern s32 cdvdNcmdState __attribute__((section(".data")));
extern s32 cdvdScmdLockSema __attribute__((section(".data")));
extern cdvdRpcClient_t cdvdNcmdClient __attribute__((section(".data")));

typedef struct sdfSemaParam_t {
    s32 count;
    s32 maxCount;
    s32 initCount;
    s32 waitThreads;
    u32 attr;
    u32 option;
} sdfSemaParam_t;

extern s32 CreateSema(sdfSemaParam_t *param);
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

extern s32 cdvdCallbackReason __attribute__((section(".data")));
void _sceCd_cd_callback(s32 *reason);
typedef void (*cdvdPowerCallback_t)(void *argument);
extern cdvdPowerCallback_t cdvdPowerCallback __attribute__((section(".data")));
extern void *cdvdPowerArgument __attribute__((section(".data")));
extern s32 cdvdPowerOffActive __attribute__((section(".data")));
extern s32 cdvdScmdState[6] __attribute__((section(".data")));
void _sceCd_Poff_Intr(void *packet, void *argument);
s32 PowerOffCB(void);

#define UNCACHED(a) ((s32 *)(EE_ADDR(a) | 0x20000000))



extern s32 _sceCd_scmd_prechk(s32 cmd);
extern void sceCdDelayThread(u16 ticks);
extern s32 sceCdSync(s32 mode);
extern s32 sceCdLayerSearchFile(addr_t file, const char *name, s32 layer);
void gcc2_compiled__6(s32 cause, s32 address, s32 argument);
void sceCdDelayThread(u16 ticks);
cdvdCallback_t sceCdCallback(cdvdCallback_t func);
void _sceCd_cd_read_intr(void *argument);
extern s32 cdvdScmdSema __attribute__((section(".data")));
void cmd_sem_init(void);
void cdvd_exit(void);
cdvdPowerCallback_t sceCdPOffCallback(cdvdPowerCallback_t callback, void *argument);
extern void cmd_sem_init(void);
extern s32 sceSifBindRpc(void *client, u32 service, s32 mode);
extern s32 cdvdNcmdLastCommand __attribute__((section(".data")));
extern s32 cdvdNcmdInit;
s32 _sceCd_ncmd_prechk(s32 command);
extern s32 _sceCd_ncmd_prechk(s32 cmd);
extern s32 cdvdNcmdReply __attribute__((section(".data")));
s32 sceCdNcmdDiskReady(void);
s32 sceCdSync(s32 mode);
s32 sceCdSyncS(s32 mode);
extern s32 cdvdScmdLastCommand __attribute__((section(".data")));
extern s32 cdvdScmdInit;
s32 _sceCd_scmd_prechk(s32 command);

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

extern s32 _sceCd_ncmd_prechk(s32 command);

void sceCdDelayThread(u16 ticks)
{
    sdfSemaParam_t param;
    s32 sema;

    param.maxCount = 1;
    param.initCount = 0;
    param.option = 0;
    sema = CreateSema(&param);
    SetAlarm(ticks, gcc2_compiled__6, EE_ADDR_AS(void *, sema));
    WaitSema(sema);
    DeleteSema(sema);
}

cdvdCallback_t sceCdCallback(cdvdCallback_t func)
{
    s32 intr;
    cdvdCallback_t old;

    if (sceCdSync(1) != 0) {
        return 0;
    }
    intr = DIntr();
    old = cdvdCallback;
    cdvdCallback = func;
    if (intr) {
        EIntr();
    }
    return old;
}

INCLUDE_ASM(const s32, "sdk/libcdvd/cdvd000", _sceCd_cd_callback);

INCLUDE_ASM(const s32, "sdk/libcdvd/cdvd000", _Cdvd_cbLoop);

void _sceCd_cd_read_intr(void *argument)
{
    cdvdReadReplyPrefix_t *reply = EE_ADDR_AS(cdvdReadReplyPrefix_t *, (EE_ADDR(argument) | 0x20000000));
    u8 *destination;
    s32 i;

    if (reply->firstLength > 0) {
        destination = reply->firstDestination;
        for (i = 0; i < reply->firstLength; i++) {
            destination[i] = reply->firstData[i];
        }
    }
    if (reply->secondLength > 0) {
        destination = reply->secondDestination;
        for (i = 0; i < reply->secondLength; i++) {
            destination[i] = reply->secondData[i];
        }
    }
    _sceCd_cd_callback(&cdvdCallbackReason);
}

void cmd_sem_init(void)
{
    /* Retain the native partial descriptor initialization. The opaque kernel
     * service's treatment of the untouched fields is not inferred here. */
    sdfSemaParam_t param;

    if (cdvdNcmdSema == -1 || cdvdScmdLockSema == -1) {
        param.option = 0;
        param.initCount = 1;
        param.maxCount = 1;
        {
            s32 sema = CreateSema(&param);
            /* Target-specific aid on the actual second-call argument: prepare
             * it before publishing the first result. Its lifetime begins after
             * the first call and ends at the second, with no intervening call. */
            FAKEMATCH_ASM_REG(sdfSemaParam_t *, nextParam, "a0") = &param;
            FAKEMATCH_ASM_LIVE(nextParam);
            cdvdNcmdSema = sema;
            cdvdScmdLockSema = CreateSema(nextParam);
        }
        param.initCount = 0;
        cdvdScmdSema = CreateSema(&param);
        /* Keep the real published semaphore in storage before the following
         * clear-address setup. This empty read-only operand emits no access
         * and is not a memory barrier or an original-source claim. */
        FAKEMATCH_ASM_READ_MEM(cdvdScmdSema);
        cdvdNcmdState = 0;
    }
}

void cdvd_exit(void)
{
    s32 intr;

    if (cdvdCallbackThreadId != 0) {
        /* The one-pass block preserves the SDK compiler's native ordering:
         * publish the stop reason before loading the semaphore to signal. */
        do {
            cdvdCallbackReason = -1;
        } while (0);
        SignalSema(cdvdScmdSema);
    }
    DeleteSema(cdvdNcmdSema);
    DeleteSema(cdvdScmdLockSema);
    DeleteSema(cdvdScmdSema);
    intr = DIntr();
    sceSifRemoveCmdHandler(0x80000012);
    if (intr) {
        EIntr();
    }
}

cdvdPowerCallback_t sceCdPOffCallback(cdvdPowerCallback_t callback, void *argument)
{
    cdvdPowerCallback_t old;
    s32 intr;

    if (cdvdScmdState[0] < 0) {
        PowerOffCB();
    }
    intr = DIntr();
    old = cdvdPowerCallback;
    cdvdPowerArgument = argument;
    cdvdPowerCallback = callback;
    if (intr) {
        EIntr();
    }
    return old;
}

void _sceCd_Poff_Intr(void *packet, void *argument)
{
    if (cdvdPowerCallback != NULL && cdvdPowerOffActive == 0) {
        cdvdPowerCallback(cdvdPowerArgument);
    }
}

s32 PowerOffCB(void)
{
    s32 intr;

    cdvdPowerOffActive = 1;
    intr = DIntr();
    sceSifAddCmdHandler(0x80000012, _sceCd_Poff_Intr, NULL);
    if (intr) {
        EIntr();
    }
    cdvdPowerOffActive = 0;
    cdvdScmdState[0] = 1;
    return 1;
}

INCLUDE_ASM(const s32, "sdk/libcdvd/cdvd000", sceCdLayerSearchFile);

s32 sceCdSearchFile(sceCdlFILE *file, const char *name)
{
    return sceCdLayerSearchFile(EE_ADDR(file), (char *)name, 0);
}

s32 _sceCd_ncmd_prechk(s32 command)
{
    s32 status;
    s32 delay;

    cmd_sem_init();
    {
        s32 sema = cdvdNcmdSema;
        FAKEMATCH_ASM_LIVE(sema);
        status = PollSema(sema);
    }
    if (cdvdNcmdSema != status) {
        if (cdvdDebugLevel > 0) {
            scePrintf("Ncmd fail sema cur_cmd:%d keep_cmd:%d\n", command, cdvdNcmdLastCommand);
        }
        return 0;
    }
    cdvdNcmdLastCommand = command;
    ReferThreadStatus(cdvdOwnerThreadId, cdvdThreadStatus);
    status = sceCdSync(1);
    {
        FAKEMATCH_ASM_REG(s32, rpcMode, "$4") = 0;
        if (status != 0) {
            s32 sema = cdvdNcmdSema;
            FAKEMATCH_ASM_LIVE(sema);
            SignalSema(sema);
            return 0;
        }
        sceSifInitRpc(rpcMode);
    }
    if (cdvdNcmdInit < 0) {
        for (;;) {
            cdvdRpcClient_t *client = &cdvdNcmdClient;

            if (sceSifBindRpc(client, 0x80000595, 0) < 0) {
                if (cdvdDebugLevel > 0) {
                    scePrintf("Libcdvd bind err N CMD\n");
                }
                for (delay = 0x100000; delay != -1; --delay) {
                }
                continue;
            }
            if (client->server != NULL) {
                break;
            }
            for (delay = 0x100000; delay != -1; --delay) {
            }
        }
        cdvdNcmdInit = 0;
    }
    return 1;
}

s32 sceCdNcmdDiskReady(void)
{
    s32 ret;

    if (!_sceCd_ncmd_prechk(2)) {
        return 0;
    }
    if (sceSifCallRpc(&cdvdNcmdClient, 14, 0, 0, 0, &cdvdNcmdReply, 4, 0, 0) < 0) {
        SignalSema(cdvdNcmdSema);
        return 0;
    }
    ret = UNCACHED(&cdvdNcmdReply)[0];
    SignalSema(cdvdNcmdSema);
    return ret;
}

s32 sceCdSync(s32 mode)
{
    if (mode == 0) {
        if (cdvdDebugLevel > 0) {
            scePrintf("N cmd wait\n");
        }
        while (cdvdNcmdState != 0 || sceSifCheckStatRpc(&cdvdNcmdClient) != 0) {
            sceCdDelayThread(60);
        }
        return 0;
    }
    if (cdvdNcmdState != 0 || sceSifCheckStatRpc(&cdvdNcmdClient) != 0) {
        return 1;
    }
    return 0;
}

s32 sceCdSyncS(s32 mode)
{
    if (mode == 0) {
        if (cdvdDebugLevel > 0) {
            scePrintf("S cmd wait\n");
        }
        while (1) {
            if (sceSifCheckStatRpc(&cdvdScmdClient) == 0) {
                break;
            }
            sceCdDelayThread(60);
        }
        return 0;
    }
    return sceSifCheckStatRpc(&cdvdScmdClient);
}

s32 _sceCd_scmd_prechk(s32 command)
{
    s32 status;
    s32 delay;

    cmd_sem_init();
    {
        s32 sema = cdvdScmdLockSema;
        FAKEMATCH_ASM_LIVE(sema);
        status = PollSema(sema);
    }
    if (cdvdScmdLockSema != status) {
        if (cdvdDebugLevel > 0) {
            scePrintf("Scmd fail sema cur_cmd:%d keep_cmd:%d\n", command, cdvdScmdLastCommand);
        }
        return 0;
    }
    cdvdScmdLastCommand = command;
    ReferThreadStatus(cdvdOwnerThreadId, cdvdThreadStatus);
    status = sceCdSyncS(1);
    {
        FAKEMATCH_ASM_REG(s32, rpcMode, "$4") = 0;
        if (status != 0) {
            s32 sema = cdvdScmdLockSema;
            FAKEMATCH_ASM_LIVE(sema);
            SignalSema(sema);
            return 0;
        }
        sceSifInitRpc(rpcMode);
    }
    if (cdvdScmdInit < 0) {
        for (;;) {
            cdvdRpcClient_t *client = &cdvdScmdClient;

            if (sceSifBindRpc(client, 0x80000593, 0) < 0) {
                if (cdvdDebugLevel > 0) {
                    scePrintf("Libcdvd bind err S cmd\n");
                }
                for (delay = 0x100000; delay != -1; --delay) {
                }
                continue;
            }
            if (client->server != NULL) {
                break;
            }
            for (delay = 0x100000; delay != -1; --delay) {
            }
        }
        cdvdScmdInit = 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "sdk/libcdvd/cdvd000", sceCdInit);

INCLUDE_ASM(const s32, "sdk/libcdvd/cdvd000", sceCdDiskReady);
