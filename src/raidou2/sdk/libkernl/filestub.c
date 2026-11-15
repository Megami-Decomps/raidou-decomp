#include "common.h"
#include "newlib_stat.h"
#include "sdk_libgcc.h"
#include "ps2asm.h"
#include "sif_rpc.h"
#include "sdk_libc.h"
extern s32 DIntr(void);
extern s32 EIntr(void);

s32 write(s32 descriptor, const void *buffer, s32 length);
s32 read(s32 descriptor, void *buffer, s32 length);
s32 open(const char *path, s32 flags, s32 mode);
s32 close(s32 descriptor);
s32 stat(const char *path, struct stat *result);

typedef struct threadMgrQueue_t {
    u32 readIndex;
    u32 writeIndex;
    u8 commands[512][2];
} threadMgrQueue_t;

extern s32 WaitSema(s32 sema);

typedef struct rpcSemaParam {
    s32 count;
    s32 maxCount;
    s32 initCount;
    s32 waitThreads;
    u32 attr;
    u32 option;
} rpcSemaParam;

extern s32 CreateSema(rpcSemaParam *param);

typedef struct kernelThreadParam_t {
    s32 status;
    void (*entry)(void *);
    void *stack;
    s32 stackSize;
    void *gp;
    s32 priority;
    u8 opaque[0x18];
} kernelThreadParam_t;

extern s32 DeleteSema(s32 sema);

typedef struct ttyQueue_t {
    u32 capacity;
    /* The TTY callback produces bytes while sceTtyRead polls occupancy. */
    volatile s32 count;
    u8 *read;
    u8 *write;
    u8 data[256];
} ttyQueue_t;

/* Native TTY context prefix, through its receive queue. */
typedef struct ttyState_t {
    s32 handle;
    s32 sendLength;
    s32 receiveLength;
    s32 sendBusy;
    u8 *sendBuffer;
    u8 *receiveBuffer;
    ttyQueue_t *queue;
} ttyState_t;

#define TTY_CALLBACK_WORD(state, member) (*(volatile s32 *)&(state)->member)

/* The transport header is eight bytes; TTY adds a four-byte reserved word.
 * Both native backing buffers span 320 bytes. */
typedef struct ttyPacket_t {
    u16 length;
    u16 reserved;
    u16 protocol;
    u8 source;
    u8 destination;
    u32 ttyReserved;
    u8 payload[308];
} ttyPacket_t;

/* In ee-gcc 2.9, next_arg names the first incoming stack argument. These
 * one-fixed-argument wrappers save the seven remaining argument GPRs in
 * contiguous eight-byte slots immediately before it. The formatter walks
 * that register-save area and then continues into the stack arguments. */
typedef char *kernelVaList_t;

typedef struct {
    void (*handler)(void *packet, void *argument);
    void *argument;
} sifCommandHandler_t;

/* Consumed command-context prefix; later state remains opaque. */
typedef struct {
    u8 prefix[8];
    void *remoteAddress;
    sifCommandHandler_t *systemHandlers;
    s32 systemCount;
    sifCommandHandler_t *handlers;
    s32 handlerCount;
    u32 *softwareRegisters;
} sifCommandContext_t;

typedef struct {
    u8 header[16];
    u32 index;
    u32 value;
} sifRegisterPacket_t;

typedef struct {
    u8 header[16];
    void *address;
} sifAddressPacket_t;

/* The sign selects the table. Scaling in 32 bits drops that selector and
 * gives the native eight-byte entry offset. Once tested, command is reused
 * for the selected table's actual address; its old value is no longer live.
 */
void sceSifAddCmdHandler(s32 command, void (*handler)(void *, void *), void *argument);

/* DMA bus addresses and transfer controls occupy four 32-bit words. */
typedef struct sifDmaTransfer_t {
    u32 source;
    u32 destination;
    s32 size;
    u32 attributes;
} sifDmaTransfer_t;

SIZE_CHECK(sifDmaTransfer_size_must_be_16, sifDmaTransfer_t, 16);

#include "sdk_libkernl.h"

extern void sceSifWriteBackDCache(void *address, s32 size);

typedef struct sifCommandHeader_t {
    u32 packetSize : 8;
    u32 dataSize : 24;
    u32 destination;
    u32 command;
    u32 option;
} sifCommandHeader_t;

SIZE_CHECK(sifCommandHeader_size_must_be_16, sifCommandHeader_t, 16);

typedef struct rpcPacket {
    u32 w[16];
} rpcPacket;

typedef struct rpcQueue {
    u32 seq;
    rpcPacket *base;
    s32 count;
    u32 unk_C; /* Cleared by sceSifInitRpc. */
    u32 unk_10; /* Cleared by sceSifInitRpc. */
    rpcPacket *forwardBase;
    s32 forwardCount;
    rpcPacket *indexedBase;
    s32 indexedCount;
    s32 forwardIndex;
    sifRpcDataQueue_t *queues;
} rpcQueue;

extern void sceSifAddCmdHandler(s32 command, void (*handler)(void *, void *), void *argument);

/* Shared transaction fields used by RPC completion. */
typedef struct rpcTransactionPrefix {
    void *packet;
    u32 sequence;
    s32 sema;
} rpcTransactionPrefix;

SIZE_CHECK(rpcTransactionPrefix_size_must_be_12, rpcTransactionPrefix, 12);

typedef struct rpcClient {
    rpcTransactionPrefix transaction;
    u32 unk_C;
    u32 serverId;
    void *buffer;
    void *cbuffer;
    void (*endFunction)(void *);
    void *endParam;
    void *server;
} rpcClient;

SIZE_CHECK(rpcClient_size_must_be_40, rpcClient, 40);

/* Observed completion-response prefix, not the complete packet allocation. */
typedef struct rpcResponse {
    u32 commandHeader[4];
    u32 flags;
    void *packetAddress;
    u32 sequence;
    rpcTransactionPrefix *client;
    u32 command;
    void *server;
    void *buffer;
    void *cbuffer;
} rpcResponse;

SIZE_CHECK(rpcResponse_prefix_size_must_be_48, rpcResponse, 48);

s32 sceSifBindRpc(rpcClient *client, u32 serverId, s32 mode);

typedef struct rpcClientPrefix {
    void *packet;
    u32 sequence;
} rpcClientPrefix;

extern s32 fsIobSema __attribute__((section(".data")));
void gcc2_compiled__13(void);

typedef struct {
    u32 word0;
    s32 active;
    u8 opaque[8];
} fsRequestEntry_t;

extern fsRequestEntry_t fsIobTable[32];
extern s32 fsIobSema;
extern void gcc2_compiled__13(void);
extern s32 SignalSema(s32 sema);
fsRequestEntry_t *new_iob(void);
fsRequestEntry_t *get_iob(u32 index);

typedef struct fsStat_t {
    u32 mode;
    u32 attributes;
    u32 size;
    u8 creationTime[8];
    u8 accessTime[8];
    u8 modificationTime[8];
    u32 highSize;
    u32 privateData[6];
} fsStat_t;

typedef struct fsCompletion_t {
    s32 sema;
    s32 command;
    void *result;
    u32 resultSize;
} fsCompletion_t;

typedef struct fsReadFragments_t {
    s32 headSize;
    s32 tailSize;
    u8 *headDestination;
    u8 *tailDestination;
    u8 head[0x40];
    u8 tail[0x40];
} fsReadFragments_t;

typedef struct fsDirectoryEntry_t {
    fsStat_t stat;
    char name[0x100];
    u32 privateData;
} fsDirectoryEntry_t;

typedef struct fsOutputCompletion_t {
    void *destination;
    u32 size;
} fsOutputCompletion_t;

extern u8 fsCompletionBuffer[];
extern s32 fsCommandSema __attribute__((section(".data")));
void _sceFsSemInit(void);
s32 _sceFsWaitS(s32 command);
void _sceFsSigSema(void);
typedef void (*fsPowerOffCallback_t)(void *argument);

typedef struct fsPowerOffState_t {
    fsPowerOffCallback_t callback;
    void *argument;
} fsPowerOffState_t;

extern fsPowerOffState_t fsPowerOffState;
extern s32 fsInitialized;
extern s32 sceFsInit(void);
fsPowerOffCallback_t scePowerOffHandler(fsPowerOffCallback_t callback, void *argument);
void _sceFs_Poff_Intr(void *packet, fsPowerOffState_t *state);

typedef struct { u8 bytes[4]; } fsVersionTag_t;

typedef struct { void *receiveBuffer; } fsRegistrationPacket_t;

extern u8 fsCmdHandlerContext[];
extern rpcClient fsRpcClient;
extern u8 fsVersionRpcBuffer[];
extern fsRegistrationPacket_t fsRegistration;
extern void sceSifInitRpc(s32 mode);
extern void _sceFs_Rcv_Intr(void *packet, void *argument);

extern s32 sceSifCallRpc(void *client, s32 function, s32 mode, void *send,
                         s32 sendBytes, void *receive, s32 receiveBytes,
                         void *endFunction, void *endParameter);

extern u8 fsDetectedVersion[0x18];
extern s32 fsInitialized __attribute__((section(".data")));
s32 sceFsInit(void);
extern u8 kernelBuiltinVersion[4] __attribute__((section(".data")));
extern const void *fsFallbackVersion __attribute__((section(".data")));
s32 _fs_version(void);
s32 sceFsReset(void);

typedef struct fsCloseRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    u32 descriptor;
    s32 slot;
} fsCloseRequest_t;

typedef struct fsPathRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    char name[0x400];
} fsPathRequest_t;

typedef struct fsMkdirRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    s32 mode;
    char name[0x400];
} fsMkdirRequest_t;

typedef struct fsGetstatRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    void *stat;
    u8 name[0x400];
} fsGetstatRequest_t;

typedef struct fsSyncRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    u32 descriptor; /* Shared word, left untouched by sync. */
    s32 mode;
    char name[0x400];
} fsSyncRequest_t;

typedef struct fsRenameRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    char oldName[0x400];
    char newName[0x400];
} fsRenameRequest_t;

typedef struct fsChstatRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    u32 mask;
    fsStat_t stat;
    u8 name[0x400];
} fsChstatRequest_t;

typedef struct fsReadlinkRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    u32 count;
    char *buffer;
    char name[0x400];
} fsReadlinkRequest_t;

typedef struct fsFormatRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    char device[0x400];
    char blockDevice[0x400];
    u8 args[0x400];
    s32 argSize;
} fsFormatRequest_t;

typedef struct fsMountRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    char device[0x400];
    char blockDevice[0x400];
    u8 args[0x400];
    s32 mode;
    s32 argSize;
} fsMountRequest_t;

typedef struct fsDreadRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    u32 descriptor;
    void *entry;
    u8 opaque14[0xC];
} fsDreadRequest_t;

typedef struct fsDevctlRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    char name[0x400];
    u8 args[0x400];
    s32 command;
    u32 argSize;
    void *output;
    u32 outputSize;
} fsDevctlRequest_t;

typedef struct fsIoctl2Request_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    u32 descriptor;
    s32 command;
    u8 args[0x400];
    void *output;
    u32 outputSize;
    u32 argSize;
} fsIoctl2Request_t;

typedef struct fsOpenRequest_t {
    s32 sema;
    s32 *result;
    s32 resultSize;
    u32 flags;
    s32 mode;
    char name[0x400];
    s32 slot;
} fsOpenRequest_t;

typedef union fsRequest_t {
    fsOpenRequest_t open;
    fsCloseRequest_t close;
    fsPathRequest_t path;
    fsMkdirRequest_t mkdir;
    fsGetstatRequest_t getstat;
    fsSyncRequest_t sync;
    fsRenameRequest_t rename;
    fsChstatRequest_t chstat;
    fsReadlinkRequest_t readlink;
    fsFormatRequest_t format;
    fsDevctlRequest_t devctl;
    fsIoctl2Request_t ioctl2;
    fsMountRequest_t mount;
    fsDreadRequest_t dread;
} fsRequest_t;

extern fsRequest_t fsRequest;
s32 sceClose(s32 descriptor);

typedef union fsIoctlStatus_t {
    s32 word;
    s64 doubleword;
} fsIoctlStatus_t;

s32 sceIoctl2(s32 descriptor, s32 command, const void *args, u32 argSize,
              void *output, u32 outputSize);

s32 _sceCallCode(const char *path, s32 command);
s32 sceRemove(const char *path);
s32 sceMkdir(const char *path, s32 mode);
s32 sceRmdir(const char *path);
s32 sceFormat(const char *device, const char *blockDevice, const void *args, s32 argSize);
s32 sceAddDrv(void *driver);
s32 sceDelDrv(const char *path);
s32 sceDopen(const char *path);
s32 sceDclose(s32 descriptor);
s32 sceDread(s32 descriptor, void *directoryEntry);
s32 sceGetstat(const char *path, void *stat);
s32 sceChstat(const char *path, const fsStat_t *stat, u32 mask);
s32 sceRename(const char *oldPath, const char *newPath);
s32 sceChdir(const char *path);
s32 sceSync(const char *path, s32 mode);
s32 sceMount(const char *device, const char *blockDevice, s32 mode, const void *args, s32 argSize);
s32 sceUmount(const char *path);


s32 sceSymlink(const char *oldPath, const char *newPath);
s32 sceReadlink(const char *path, char *buffer, u32 count);

typedef struct heapLoadRequest_t {
    u32 address;
    char name[0xFC];
} heapLoadRequest_t;

/* The load-file RPC sends this complete 0x200-byte shared request region. */
typedef struct lfRequest_t {
    s32 value;
    s32 moduleResult; /* Argument size on buffer-load/stop requests; result on replies. */
    char name[0xFC];
    u8 arguments[0xFC];
} lfRequest_t;

extern u8 kernelBuiltinVersion[4];

typedef struct sifResetPacket_t {
    u64 packetSize : 8;
    u64 dataSize : 24;
    void *destination;
    u32 command;
    u32 option;
    s32 argumentSize;
    s32 mode;
    char arguments[80];
} sifResetPacket_t;

SIZE_CHECK(sifResetPacket_size_must_be_0x68, sifResetPacket_t, 0x68);

typedef struct {
    u32 index;
    void *entry;
} kernelEntryPair_t;

typedef struct execArgumentBlock_t {
    char *arguments[16];
    char strings[1];
} execArgumentBlock_t;

typedef void (*deci2Handler_t)(s32 event, s32 parameter, void *argument);

typedef struct deci2Registration_t {
    u32 protocol;
    void *argument;
    deci2Handler_t handler;
    void *storage;
} deci2Registration_t;

typedef struct deci2Transfer_t {
    s32 handle;
    void *buffer;
    u32 length;
} deci2Transfer_t;

fsRequestEntry_t *new_iob(void)
{
    fsRequestEntry_t *entry;

    gcc2_compiled__13();
    WaitSema(fsIobSema);
    for (entry = fsIobTable; entry < &fsIobTable[32]; entry++) {
        if (entry->active == 0) {
            entry->active = 0x10000000;
            SignalSema(fsIobSema);
            return entry;
        }
    }
    SignalSema(fsIobSema);
    return NULL;
}

fsRequestEntry_t *get_iob(u32 index)
{
    fsRequestEntry_t *entry;

    gcc2_compiled__13();
    WaitSema(fsIobSema);
    if (index >= 32) {
        SignalSema(fsIobSema);
        return NULL;
    }
    entry = &fsIobTable[index];
    SignalSema(fsIobSema);
    return entry;
}

INCLUDE_ASM(const s32, "sdk/libkernl/filestub", _sceFs_Rcv_Intr);

void _sceFsSemInit(void)
{
    rpcSemaParam param;
    if (fsCommandSema == -1) {
        /* Only these descriptor fields are initialized by the native helper. */
        param.initCount = 1;
        param.maxCount = 1;
        param.option = 0;
        fsCommandSema = CreateSema(&param);
    }
}

s32 _sceFsWaitS(s32 command)
{
    _sceFsSemInit();
    WaitSema(fsCommandSema);
    return 0;
}

/* Internal notification: callers discard the SignalSema status. */
void _sceFsSigSema(void)
{
    SignalSema(fsCommandSema);
}

fsPowerOffCallback_t scePowerOffHandler(fsPowerOffCallback_t callback, void *argument)
{
    fsPowerOffState_t *state = &fsPowerOffState;
    fsPowerOffCallback_t previous;
    s32 interrupts;

    _sceFsWaitS(0x1B);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    interrupts = DIntr();
    previous = fsPowerOffState.callback;
    state->argument = argument;
    fsPowerOffState.callback = callback;
    if (interrupts != 0) {
        EIntr();
    }
    _sceFsSigSema();
    return previous;
}

void _sceFs_Poff_Intr(void *packet, fsPowerOffState_t *state)
{
    if (state->callback != NULL) {
        state->callback(state->argument);
    }
    EE_SYNC_EI();
}

s32 sceFsInit(void)
{
    fsPowerOffState_t *powerOff = &fsPowerOffState;
    s32 previousInterrupts;
    s32 delay;
    fsRequestEntry_t *entry;

    sceSifInitRpc(0);
    powerOff->callback = NULL;
    powerOff->argument = NULL;
    previousInterrupts = DIntr();
    sceSifAddCmdHandler(0x80000011, _sceFs_Rcv_Intr, fsCmdHandlerContext);
    sceSifAddCmdHandler(0x80000013, (void (*)(void *, void *))_sceFs_Poff_Intr, powerOff);
    if (previousInterrupts)
        EIntr();
    for (;;) {
        if (sceSifBindRpc(&fsRpcClient, 0x80000001, 0) < 0)
            return -1;
        if (fsRpcClient.server != NULL)
            break;
        for (delay = 0x100000; delay != -1; --delay) {
        }
    }
    gcc2_compiled__13();
    WaitSema(fsIobSema);
    for (entry = fsIobTable; entry < &fsIobTable[32]; entry++)
        entry->active = 0;
    SignalSema(fsIobSema);
    fsRegistration.receiveBuffer = fsCompletionBuffer;
    if (sceSifCallRpc(&fsRpcClient, 255, 0, &fsRegistration, 4,
                      fsVersionRpcBuffer, 4, NULL, NULL) < 0)
        return -0x10001;
    *(fsVersionTag_t *)fsDetectedVersion =
        *(const fsVersionTag_t *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    fsInitialized = 1;
    return 0;
}

s32 _fs_version(void)
{
    const void *expected = kernelBuiltinVersion;
    const void *version = fsDetectedVersion;
    s32 mismatch = 0;
    if (memcmp(version, expected, 4) != 0) {
        if (memcmp(version, fsFallbackVersion, 4) != 0) {
            mismatch = memcmp(expected, fsFallbackVersion, 4) != 0;
        }
    }
    return mismatch;
}

s32 sceFsReset(void)
{
    fsInitialized = 0;
    memset(fsDetectedVersion, 0, 4);
    return 0;
}

INCLUDE_ASM(const s32, "sdk/libkernl/filestub", sceOpen);

s32 sceClose(s32 descriptor)
{
    rpcSemaParam param;
    s32 result;
    fsCloseRequest_t *request = &fsRequest.close;
    fsRequestEntry_t *entry;
    s32 sema;
    s32 accepted;
    s32 status;
    s32 threshold;

    entry = get_iob(descriptor);
    _sceFsWaitS(1);
    if (fsInitialized == 0) {
        _sceFsSigSema();
        return -1;
    }
    if (entry == NULL || entry->active == 0) {
        _sceFsSigSema();
        return -9;
    }
    request->descriptor = entry->word0;
    request->slot = entry - fsIobTable;
    param.maxCount = 1;
    param.initCount = 0;
    param.option = 0;
    sema = CreateSema(&param);
    fsRequest.close.sema = sema;
    request->result = &result;
    request->resultSize = 4;
    if (sceSifCallRpc(&fsRpcClient, 1, 0, request, 0x14,
                      fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    entry->active = 0;
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    status = result;
    threshold = -1;
    /* Keep the real native signed threshold in a general register for its
     * comparison. No instruction, fixed register or memory effect is added. */
    FAKEMATCH_ASM_LIVE(threshold);
    if (threshold < status) {
        status = 0;
    }
    return status;
}

INCLUDE_ASM(const s32, "sdk/libkernl/filestub", sceLseek);

INCLUDE_ASM(const s32, "sdk/libkernl/filestub", sceRead);

INCLUDE_ASM(const s32, "sdk/libkernl/filestub", sceWrite);

INCLUDE_ASM(const s32, "sdk/libkernl/filestub", sceIoctl);

s32 sceIoctl2(s32 descriptor, s32 command, const void *args, u32 argSize,
              void *output, u32 outputSize)
{
    rpcSemaParam param;
    s32 result;
    fsIoctl2Request_t *request = &fsRequest.ioctl2;
    fsRequestEntry_t *entry;
    s32 sema;
    s32 accepted;
    fsRequest_t *packet;

    entry = get_iob(descriptor);
    _sceFsWaitS(26);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    if (entry == NULL || entry->active == 0) {
        _sceFsSigSema();
        return -9;
    }
    if (argSize > 0x400 || outputSize > 0x400) {
        _sceFsSigSema();
        return -22;
    }
    if (args == NULL) {
        request->argSize = 0;
    } else {
        memcpy(request->args, args, argSize);
    }
    request->descriptor = entry->word0;
    request->command = command;
    request->argSize = argSize;
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    packet = &fsRequest;
    sema = CreateSema(&param);
    request->outputSize = outputSize;
    request->result = &result;
    request->resultSize = 4;
    request->output = output;
    request->sema = sema;
    sceSifWriteBackDCache(packet, 0x420);
    if (sceSifCallRpc(&fsRpcClient, 26, 0, packet,
                      0x420, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 _sceCallCode(const char *path, s32 command)
{
    rpcSemaParam param;
    s32 result;
    fsPathRequest_t *request = &fsRequest.path;
    s32 length;
    s32 sema;
    s32 accepted;

    _sceFsWaitS(command);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    for (length = 0; length < 0x400 &&
         (request->name[length] = path[length]) != 0; length++) {
    }
    if (length == 0x400) {
        request->name[0x3FF] = 0;
        length = 0x3FF;
    }
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    sema = CreateSema(&param);
    request->result = &result;
    request->sema = sema;
    request->resultSize = 4;
    if (sceSifCallRpc(&fsRpcClient, command, 0, &fsRequest,
                      length + 0xD, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceRemove(const char *path)
{
    return _sceCallCode(path, 6);
}

s32 sceMkdir(const char *path, s32 mode)
{
    rpcSemaParam param;
    s32 result;
    fsMkdirRequest_t *request = &fsRequest.mkdir;
    s32 length;
    s32 sema;
    s32 accepted;

    _sceFsWaitS(7);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    for (length = 0; length < 0x400 &&
         (request->name[length] = path[length]) != 0; length++) {
    }
    if (length == 0x400) {
        request->name[0x3FF] = 0;
        length = 0x3FF;
    }
    request->mode = mode;
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    sema = CreateSema(&param);
    request->result = &result;
    request->sema = sema;
    request->resultSize = 4;
    if (sceSifCallRpc(&fsRpcClient, 7, 0, &fsRequest,
                      length + 0x11, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceRmdir(const char *path)
{
    return _sceCallCode(path, 8);
}

s32 sceFormat(const char *device, const char *blockDevice, const void *args, s32 argSize)
{
    rpcSemaParam param;
    s32 result;
    fsFormatRequest_t *request = &fsRequest.format;
    s32 deviceLength;
    s32 blockLength;
    s32 argIndex;
    s32 sema;
    s32 accepted;
    fsRequest_t *packet;

    _sceFsWaitS(14);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    for (deviceLength = 0; deviceLength < 0x400 &&
         (request->device[deviceLength] = device[deviceLength]) != 0; deviceLength++) {
    }
    if (deviceLength == 0x400) {
        request->device[0x3FF] = 0;
    }
    if (blockDevice == NULL) {
        request->blockDevice[0] = 0;
    } else {
        for (blockLength = 0; blockLength < 0x400 &&
             (request->blockDevice[blockLength] = blockDevice[blockLength]) != 0; blockLength++) {
        }
        if (blockLength == 0x400) {
            request->blockDevice[0x3FF] = 0;
        }
    }
    if (argSize > 0x400) {
        _sceFsSigSema();
        return -7;
    }
    for (argIndex = 0; argIndex < argSize; argIndex++) {
        request->args[argIndex] = ((const u8 *)args)[argIndex];
    }
    request->argSize = argSize;
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    packet = &fsRequest;
    sema = CreateSema(&param);
    request->sema = sema;
    request->result = &result;
    request->resultSize = 4;
    sceSifWriteBackDCache(packet, 0xC10);
    if (sceSifCallRpc(&fsRpcClient, 14, 0, packet,
                      0xC10, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceAddDrv(void *driver)
{
    rpcSemaParam param;
    s32 result;
    fsCloseRequest_t *request = &fsRequest.close;
    s32 sema;
    s32 accepted;

    _sceFsWaitS(15);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    request->descriptor = EE_ADDR(driver);
    param.maxCount = 1;
    param.initCount = 0;
    param.option = 0;
    sema = CreateSema(&param);
    fsRequest.close.sema = sema;
    request->result = &result;
    request->resultSize = 4;
    if (sceSifCallRpc(&fsRpcClient, 15, 0, request, 16,
                      fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -1;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -1;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceDelDrv(const char *path)
{
    return _sceCallCode(path, 16);
}

s32 sceDopen(const char *path)
{
    fsRequestEntry_t *entry;
    s32 result;
    s32 sema;

    _sceFsWaitS(9);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    _sceFsSigSema();
    entry = new_iob();
    if (entry == NULL) {
        return -19;
    }
    result = _sceCallCode(path, 9);
    if (result < 0) {
        WaitSema(fsIobSema);
        entry->active = 0;
        sema = fsIobSema;
    } else {
        WaitSema(fsIobSema);
        entry->word0 = result;
        sema = fsIobSema;
        result = entry - fsIobTable;
    }
    SignalSema(sema);
    return result;
}

s32 sceDclose(s32 descriptor)
{
    rpcSemaParam param;
    s32 result;
    fsCloseRequest_t *request = &fsRequest.close;
    fsRequestEntry_t *entry;
    s32 sema;
    s32 accepted;
    s32 status;
    s32 threshold;

    entry = get_iob(descriptor);
    _sceFsWaitS(10);
    if (fsInitialized == 0) {
        _sceFsSigSema();
        return -1;
    }
    if (entry == NULL || entry->active == 0) {
        _sceFsSigSema();
        return -9;
    }
    request->descriptor = entry->word0;
    param.maxCount = 1;
    param.initCount = 0;
    param.option = 0;
    sema = CreateSema(&param);
    fsRequest.close.sema = sema;
    request->result = &result;
    request->resultSize = 4;
    if (sceSifCallRpc(&fsRpcClient, 10, 0, request, 0x14,
                      fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    entry->active = 0;
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    status = result;
    threshold = -1;
    /* Keep the real native signed threshold in a general register for its
     * comparison. No instruction, fixed register or memory effect is added. */
    FAKEMATCH_ASM_LIVE(threshold);
    if (threshold < status) {
        status = 0;
    }
    return status;
}

s32 sceDread(s32 descriptor, void *directoryEntry)
{
    rpcSemaParam param;
    s32 result;
    fsDreadRequest_t *request = &fsRequest.dread;
    fsRequestEntry_t *entry;
    s32 sema;
    s32 accepted;

    entry = get_iob(descriptor);
    _sceFsWaitS(11);
    if (fsInitialized == 0) {
        _sceFsSigSema();
        return -1;
    }
    if (entry == NULL || entry->active == 0) {
        _sceFsSigSema();
        return -9;
    }
    request->entry = directoryEntry;
    request->descriptor = entry->word0;
    param.maxCount = 1;
    param.initCount = 0;
    param.option = 0;
    sema = CreateSema(&param);
    fsRequest.dread.sema = sema;
    request->result = &result;
    request->resultSize = 4;
    if (sceSifCallRpc(&fsRpcClient, 11, 0, request, 0x20,
                      fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        /* The failure path waits before releasing the filesystem lock. */
        WaitSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceGetstat(const char *path, void *stat)
{
    rpcSemaParam param;
    s32 result;
    fsGetstatRequest_t *request = &fsRequest.getstat;
    s32 length;
    s32 sema;
    s32 accepted;

    _sceFsWaitS(12);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    for (length = 0; length < 0x400 &&
         (request->name[length] = path[length]) != 0; length++) {
    }
    if (length == 0x400) {
        request->name[0x3FF] = 0;
        length = 0x3FF;
    }
    request->stat = stat;
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    sema = CreateSema(&param);
    request->result = &result;
    request->sema = sema;
    request->resultSize = 4;
    if (sceSifCallRpc(&fsRpcClient, 12, 0, &fsRequest,
                      length + 0x11, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceChstat(const char *path, const fsStat_t *stat, u32 mask)
{
    rpcSemaParam param;
    s32 result;
    fsChstatRequest_t *request = &fsRequest.chstat;
    s32 length;
    s32 sema;
    s32 accepted;
    fsRequest_t *packet;

    _sceFsWaitS(13);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    for (length = 0; length < 0x400 &&
         (request->name[length] = path[length]) != 0; length++) {
    }
    if (length == 0x400) {
        request->name[0x3FF] = 0;
        length = 0x3FF;
    }
    request->stat = *stat;
    request->mask = mask;
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    packet = &fsRequest;
    sema = CreateSema(&param);
    request->sema = sema;
    request->result = &result;
    request->resultSize = 4;
    sceSifWriteBackDCache(packet, 0x450);
    if (sceSifCallRpc(&fsRpcClient, 13, 0, packet,
                      length + 0x51, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceRename(const char *oldPath, const char *newPath)
{
    rpcSemaParam param;
    s32 result;
    fsRenameRequest_t *request = &fsRequest.rename;
    s32 oldLength;
    s32 newLength;
    s32 sema;
    s32 accepted;
    fsRequest_t *packet;

    _sceFsWaitS(17);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    for (oldLength = 0; oldLength < 0x400 &&
         (request->oldName[oldLength] = oldPath[oldLength]) != 0; oldLength++) {
    }
    if (oldLength == 0x400) {
        request->oldName[0x3FF] = 0;
    }
    for (newLength = 0; newLength < 0x400 &&
         (request->newName[newLength] = newPath[newLength]) != 0; newLength++) {
    }
    if (newLength == 0x400) {
        request->newName[0x3FF] = 0;
    }
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    packet = &fsRequest;
    sema = CreateSema(&param);
    request->sema = sema;
    request->result = &result;
    request->resultSize = 4;
    sceSifWriteBackDCache(packet, 0x80C);
    if (sceSifCallRpc(&fsRpcClient, 17, 0, packet,
                      0x80C, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceChdir(const char *path)
{
    return _sceCallCode(path, 18);
}

s32 sceSync(const char *path, s32 mode)
{
    rpcSemaParam param;
    s32 result;
    fsSyncRequest_t *request = &fsRequest.sync;
    s32 length;
    s32 sema;
    s32 accepted;

    _sceFsWaitS(19);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    for (length = 0; length < 0x400 &&
         (request->name[length] = path[length]) != 0; length++) {
    }
    if (length == 0x400) {
        request->name[0x3FF] = 0;
    }
    request->mode = mode;
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    sema = CreateSema(&param);
    request->result = &result;
    request->sema = sema;
    request->resultSize = 4;
    if (sceSifCallRpc(&fsRpcClient, 19, 0, &fsRequest,
                      0x414, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceMount(const char *device, const char *blockDevice, s32 mode, const void *args, s32 argSize)
{
    rpcSemaParam param;
    s32 result;
    fsMountRequest_t *request = &fsRequest.mount;
    s32 deviceLength;
    s32 blockLength;
    s32 argIndex;
    s32 sema;
    s32 accepted;
    fsRequest_t *packet;

    _sceFsWaitS(20);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    for (deviceLength = 0; deviceLength < 0x400 &&
         (request->device[deviceLength] = device[deviceLength]) != 0; deviceLength++) {
    }
    if (deviceLength == 0x400) {
        request->device[0x3FF] = 0;
    }
    for (blockLength = 0; blockLength < 0x400 &&
         (request->blockDevice[blockLength] = blockDevice[blockLength]) != 0; blockLength++) {
    }
    if (blockLength == 0x400) {
        request->blockDevice[0x3FF] = 0;
    }
    if (argSize > 0x400) {
        _sceFsSigSema();
        return -7;
    }
    for (argIndex = 0; argIndex < argSize; argIndex++) {
        request->args[argIndex] = ((const u8 *)args)[argIndex];
    }
    request->argSize = argSize;
    request->mode = mode;
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    packet = &fsRequest;
    sema = CreateSema(&param);
    request->sema = sema;
    request->result = &result;
    request->resultSize = 4;
    sceSifWriteBackDCache(packet, 0xC14);
    if (sceSifCallRpc(&fsRpcClient, 20, 0, packet,
                      0xC14, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceUmount(const char *path)
{
    return _sceCallCode(path, 21);
}

INCLUDE_ASM(const s32, "sdk/libkernl/filestub", sceLseek64);

s32 sceDevctl(const char *device, s32 command, const void *args, u32 argSize,
              void *output, u32 outputSize)
{
    rpcSemaParam param;
    s32 result;
    fsDevctlRequest_t *request;
    s32 length;
    u32 argIndex;
    s32 sema;
    s32 accepted;
    fsRequest_t *packet;

    _sceFsWaitS(23);
    request = &fsRequest.devctl;
    if (fsInitialized == 0) {
        sceFsInit();
    }
    for (length = 0; length < 0x400 &&
         (request->name[length] = device[length]) != 0; length++) {
    }
    if (length == 0x400) {
        request->name[0x3FF] = 0;
    }
    if (argSize > 0x400 || outputSize > 0x400) {
        _sceFsSigSema();
        return -22;
    }
    for (argIndex = 0; argIndex < argSize; argIndex++) {
        request->args[argIndex] = ((const u8 *)args)[argIndex];
    }
    request->argSize = argSize;
    request->command = command;
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    packet = &fsRequest;
    sema = CreateSema(&param);
    request->outputSize = outputSize;
    request->result = &result;
    request->resultSize = 4;
    request->output = output;
    request->sema = sema;
    sceSifWriteBackDCache(packet, 0x81C);
    if (sceSifCallRpc(&fsRpcClient, 23, 0, packet,
                      0x81C, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceSymlink(const char *oldPath, const char *newPath)
{
    rpcSemaParam param;
    s32 result;
    fsRenameRequest_t *request = &fsRequest.rename;
    s32 oldLength;
    s32 newLength;
    s32 sema;
    s32 accepted;

    _sceFsWaitS(17);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    for (oldLength = 0; oldLength < 0x400 &&
         (request->oldName[oldLength] = oldPath[oldLength]) != 0; oldLength++) {
    }
    if (oldLength == 0x400) {
        request->oldName[0x3FF] = 0;
    }
    for (newLength = 0; newLength < 0x400 &&
         (request->newName[newLength] = newPath[newLength]) != 0; newLength++) {
    }
    if (newLength == 0x400) {
        request->newName[0x3FF] = 0;
    }
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    sema = CreateSema(&param);
    request->result = &result;
    request->sema = sema;
    request->resultSize = 4;
    if (sceSifCallRpc(&fsRpcClient, 24, 0, &fsRequest,
                      0x80C, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

s32 sceReadlink(const char *path, char *buffer, u32 count)
{
    rpcSemaParam param;
    s32 result;
    fsReadlinkRequest_t *request = &fsRequest.readlink;
    s32 length;
    s32 sema;
    s32 accepted;

    _sceFsWaitS(17);
    if (fsInitialized == 0) {
        sceFsInit();
    }
    for (length = 0; length < 0x400 &&
         (request->name[length] = path[length]) != 0; length++) {
    }
    if (length == 0x400) {
        request->name[0x3FF] = 0;
    }
    if (count >= 0x400) {
        count = 0x3FF;
    }
    request->buffer = buffer;
    request->count = count;
    sceSifWriteBackDCache(buffer, count);
    param.initCount = 0;
    param.maxCount = 1;
    param.option = 0;
    sema = CreateSema(&param);
    request->result = &result;
    request->sema = sema;
    request->resultSize = 4;
    /* The fixed transfer also includes the shared buffer's untouched tail. */
    if (sceSifCallRpc(&fsRpcClient, 25, 0, &fsRequest,
                      0x80C, fsVersionRpcBuffer, 4, NULL, NULL) < 0) {
        DeleteSema(sema);
        _sceFsSigSema();
        return -11;
    }
    accepted = *(s32 *)(EE_ADDR(fsVersionRpcBuffer) | 0x20000000);
    _sceFsSigSema();
    if (accepted == 0) {
        DeleteSema(sema);
        return -11;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return result;
}

INCLUDE_RODATA(const s32, "sdk/libkernl/filestub", lfStr);

INCLUDE_RODATA(const s32, "sdk/libkernl/filestub", lfStr2);

