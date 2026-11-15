#include "common.h"
#include "newlib_stat.h"
#include "sdk_libgcc.h"
#include "ps2asm.h"
#include "sif_rpc.h"
#include "sdk_libc.h"

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

typedef struct rpcSemaParam {
    s32 count;
    s32 maxCount;
    s32 initCount;
    s32 waitThreads;
    u32 attr;
    u32 option;
} rpcSemaParam;

typedef struct kernelThreadParam_t {
    s32 status;
    void (*entry)(void *);
    void *stack;
    s32 stackSize;
    void *gp;
    s32 priority;
    u8 opaque[0x18];
} kernelThreadParam_t;

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

extern void FlushCache(s32 mode);

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

/* DMA bus addresses and transfer controls occupy four 32-bit words. */
typedef struct sifDmaTransfer_t {
    u32 source;
    u32 destination;
    s32 size;
    u32 attributes;
} sifDmaTransfer_t;

SIZE_CHECK(sifDmaTransfer_size_must_be_16, sifDmaTransfer_t, 16);

#include "sdk_libkernl.h"

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

typedef struct rpcClientPrefix {
    void *packet;
    u32 sequence;
} rpcClientPrefix;

typedef struct {
    u32 word0;
    s32 active;
    u8 opaque[8];
} fsRequestEntry_t;

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

typedef void (*fsPowerOffCallback_t)(void *argument);

typedef struct fsPowerOffState_t {
    fsPowerOffCallback_t callback;
    void *argument;
} fsPowerOffState_t;

typedef struct { u8 bytes[4]; } fsVersionTag_t;

typedef struct { void *receiveBuffer; } fsRegistrationPacket_t;

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

typedef union fsIoctlStatus_t {
    s32 word;
    s64 doubleword;
} fsIoctlStatus_t;

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

extern void InitTLBFunctions(void);

typedef struct execArgumentBlock_t {
    char *arguments[16];
    char strings[1];
} execArgumentBlock_t;

extern void *execArgumentBlock;
extern kernelEntryPair_t tlbEntryPairs[8];
extern const u8 TLBFunctionsImage[0x330];
extern s32 Copy_2(void *destination, const void *source, u32 size);
extern void *GetEntryAddress_1(u32 index);
extern void setup_0(u32 index, void *entry);
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

/* Handler entries stay opaque: callers supply code addresses, and this unit
 * does not establish a specific handler function signature. */
extern void *tlbHandler;
extern void *debugHandlers[16];
extern void _kTLBException();
extern void _kDebugException();
extern void *SetVTLBRefillHandler(s32 index, void *handler);
extern void *SetVCommonHandler(s32 index, void *handler);
void *SetTLBHandler(void *handler);
void *SetDebugHandler(s32 index, void *handler);
s32 kCopy_3(u32 *dst, const u32 *src, u32 count);
void InitTLBFunctions(void);
extern s32 _SetTLBEntry(u32 index, u32 pageMask, u32 entryHi, u32 entryLo0, u32 entryLo1);
s32 iPutTLBEntry(u32 index, u32 pageMask, u32 entryHi, u32 entryLo0, u32 entryLo1);

void *SetTLBHandler(void *handler)
{
    tlbHandler = handler;
    SetVTLBRefillHandler(1, _kTLBException);
    SetVTLBRefillHandler(2, _kTLBException);
    SetVTLBRefillHandler(3, _kTLBException);
    return handler;
}

void *SetDebugHandler(s32 index, void *handler)
{
    void *previous;

    if (((u32)index - 1) >= 13) {
        return (void *)-1;
    }

    previous = debugHandlers[index];
    debugHandlers[index] = handler;
    if (((u32)index - 1) < 3) {
        SetVTLBRefillHandler(index, _kDebugException);
    } else {
        SetVCommonHandler(index, _kDebugException);
    }
    return previous;
}

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", Copy_2);

s32 kCopy_3(u32 *dst, const u32 *src, u32 count)
{
    u32 i;

    count >>= 2;
    for (i = 0; i < count; i++) {
        dst[i] = src[i];
    }
    return 0;
}

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", GetEntryAddress_1);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", setup_0);

void InitTLBFunctions(void)
{
    u32 index;
    u32 i;
    void *entry;

    setup_0(tlbEntryPairs[0].index, tlbEntryPairs[0].entry);
    Copy_2((void *)0x80075000, TLBFunctionsImage, 0x330);
    FlushCache(0);
    FlushCache(2);
    setup_0(tlbEntryPairs[1].index, tlbEntryPairs[1].entry);
    setup_0(tlbEntryPairs[2].index, tlbEntryPairs[2].entry);
    for (i = 3; i < 8; i++) {
        index = tlbEntryPairs[i].index;
        entry = GetEntryAddress_1(index);
        setup_0(tlbEntryPairs[i].index, entry);
    }
    execArgumentBlock = GetEntryAddress_1(3);
}

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", tlbfuncLeaf);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", tlbfuncLeaf2);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", _SetTLBEntry);

/* Existing retail symbol is retained; this is the checked indexed setter,
 * not the separate interrupt-context PutTLBEntry syscall stub. */
s32 iPutTLBEntry(u32 index, u32 pageMask, u32 entryHi, u32 entryLo0, u32 entryLo1)
{
    if (index - 13U >= 35U) {
        return -1;
    }
    return _SetTLBEntry(index, pageMask, entryHi, entryLo0, entryLo1);
}

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", tlbfuncLeaf3);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", tlbfuncLeaf4);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", tlbfuncLeaf5);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", tlbfuncLeaf6);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", tlbfuncLeaf7);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", tlbfuncLeaf8);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", InitTLB);

INCLUDE_ASM(const s32, "sdk/libkernl/tlbfunc", InitTLB32MB);
