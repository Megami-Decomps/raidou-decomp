#include "common.h"
#include "newlib_stat.h"
#include "sdk_libgcc.h"
#include "ps2asm.h"
#include "sif_rpc.h"

extern s32 DIntr(void);
extern s32 EIntr(void);
extern s32 sceTtyInit(void);
extern s32 sceTtyRead(char *buffer, s32 length);
extern s32 sceTtyWrite(const char *buffer, s32 length);
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

extern void kprintf(const char *format, ...);

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

ttyQueue_t *gcc2_compiled__10(u32 capacity);
void QueuePeekWriteDone(ttyQueue_t *queue);
void QueuePeekReadDone(ttyQueue_t *queue);

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

extern ttyState_t ttyState;
extern s32 sceDeci2ExRecv(s32 handle, void *buffer, u16 length);
extern s32 sceDeci2ExSend(s32 handle, void *buffer, u16 length);
extern const char ttyMsgPacketTooLarge[];
extern const char ttyMsgReceiveError[];
extern const char ttyMsgSendError[];
extern const char ttyMsgWlenError[];

#define TTY_CALLBACK_WORD(state, member) (*(volatile s32 *)&(state)->member)

void sceTtyHandler(s32 event, s32 byteCount, void *argument);

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

extern ttyPacket_t ttySendPacket;
extern ttyPacket_t ttyReceivePacket;
extern s32 sceDeci2ReqSend(s32 handle, s8 destination);
extern s32 sceDeci2Poll(s32 handle);
s32 sceTtyWrite(const char *buffer, s32 length);
s32 sceTtyRead(char *buffer, s32 length);
extern void FlushCache(s32 mode);

extern s32 sceDeci2Open(u16 protocol, void *argument,
                       void (*handler)(s32, s32, void *));

s32 sceTtyInit(void);

/* In ee-gcc 2.9, next_arg names the first incoming stack argument. These
 * one-fixed-argument wrappers save the seven remaining argument GPRs in
 * contiguous eight-byte slots immediately before it. The formatter walks
 * that register-save area and then continues into the stack arguments. */
typedef char *kernelVaList_t;

void kprintf(const char *format, ...);

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

extern void *memcpy(void *destination, const void *source, size_t size);
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

s32 sceDeci2Open(u16 protocol, void *argument, deci2Handler_t handler);
s32 sceDeci2ReqSend(s32 handle, s8 destination);
s32 sceDeci2Poll(s32 handle);

typedef struct deci2Transfer_t {
    s32 handle;
    void *buffer;
    u32 length;
} deci2Transfer_t;

s32 sceDeci2ExRecv(s32 handle, void *buffer, u16 length);
s32 sceDeci2ExSend(s32 handle, void *buffer, u16 length);

void QueuePeekWriteDone(ttyQueue_t *queue)
{
    queue->count++;
    queue->write++;
    if (queue->write == queue->data + queue->capacity) {
        queue->write = queue->data;
    }
}

void QueuePeekReadDone(ttyQueue_t *queue)
{
    queue->count--;
    queue->read++;
    if (queue->read == queue->data + queue->capacity) {
        queue->read = queue->data;
    }
}

void sceTtyHandler(s32 event, s32 byteCount, void *argument)
{
    /* Qualified EE 2.9 global_alloc trace: this shared receive/traversal count
     * has 17 refs over 44 live instructions, ahead of the context's 25/78.
     * It takes r16 and the context takes r17; splitting the count reverses them.
     * Ordinary, dumped and hooked assembly agree; both RTL dump sets agree. */
    ttyState_t *state = argument;
    u8 *packet;

    switch (event) {
    case 1:
    case 2:
        if (byteCount != 0) {
            if ((u32)TTY_CALLBACK_WORD(state, receiveLength) + (u32)byteCount > 320) {
                kprintf(ttyMsgPacketTooLarge);
            }
            {
                s32 receiveOffset = TTY_CALLBACK_WORD(state, receiveLength);
                byteCount = sceDeci2ExRecv(state->handle,
                                          state->receiveBuffer + receiveOffset,
                                          (u16)byteCount);
            }
            if (byteCount < 0) {
                kprintf(ttyMsgReceiveError);
            }
            TTY_CALLBACK_WORD(state, receiveLength) += byteCount;
        } else {
            packet = state->receiveBuffer;
            for (byteCount = 12; byteCount < *(u16 *)packet; byteCount++) {
                *state->queue->write = state->receiveBuffer[byteCount];
                QueuePeekWriteDone(state->queue);
            }
            TTY_CALLBACK_WORD(state, receiveLength) = 0;
        }
        return;
    case 3: {
        s32 sent = sceDeci2ExSend(state->handle, state->sendBuffer,
                                (u16)TTY_CALLBACK_WORD(state, sendLength));
        if (sent < 0) {
            kprintf(ttyMsgSendError, sent);
        } else {
            state->sendBuffer += sent;
            TTY_CALLBACK_WORD(state, sendLength) -= sent;
            return;
        }
        break;
    }
    case 4:
        if (TTY_CALLBACK_WORD(state, sendLength) != 0) {
            kprintf(ttyMsgWlenError, TTY_CALLBACK_WORD(state, sendLength));
        }
        break;
    default:
        return;
    }
    TTY_CALLBACK_WORD(state, sendBusy) = 0;
}

s32 sceTtyWrite(const char *buffer, s32 length)
{
    s32 consumed = 0;
    s32 outputLength = 0;
    /* Reuse the live transport status for the request and completion tests. */
    s32 status;
    ttyPacket_t *packet;
    u8 *output;

    if ((*(volatile ttyState_t *)&ttyState).sendBusy != 0) {
        return -1;
    }
    DIntr();
    packet = (ttyPacket_t *)(EE_ADDR(&ttySendPacket) | 0x20000000);
    ttyState.sendBusy = 1;
    ttyState.sendBuffer = (u8 *)packet;
    output = packet->payload;
    /* Native decrement wraps even for negative lengths; only zero stops it. */
    while ((length = (s32)((u32)length - 1)) != -1) {
        if (*buffer == '\n') {
            *output++ = '\r';
            if (++outputLength >= 256) {
                break;
            }
        }
        *output++ = *buffer++;
        outputLength++;
        consumed++;
        if (outputLength >= 256) {
            break;
        }
    }
    {
        volatile ttyState_t *state = &ttyState;

        state->sendLength = outputLength + 12;
        packet->length = state->sendLength;
        status = sceDeci2ReqSend((*(volatile ttyState_t *)&ttyState).handle,
                                 (s8)packet->destination);
        if (status < 0) {
            state->sendBusy = 0;
            EIntr();
            return -1;
        }
        while ((status = state->sendBusy) != 0) {
            sceDeci2Poll((*(volatile ttyState_t *)&ttyState).handle);
        }
        EIntr();
        return consumed;
    }
}

s32 sceTtyRead(char *buffer, s32 length)
{
    s32 count;

    for (count = 0; count < length; count++) {
        while (ttyState.queue->count == 0) {
        }
        buffer[count] = *ttyState.queue->read;
        QueuePeekReadDone(ttyState.queue);
        if (buffer[count] == 10 || buffer[count] == 13) {
            return count + 1;
        }
    }
    return count;
}

s32 sceTtyInit(void)
{
    ttyPacket_t *packet;

    /* Registration exposes this context to the asynchronous TTY callback.
     * Publish and observe its shared words with native-width accesses. */
    FlushCache(0);
    (*(volatile s32 *)&ttyState.handle) = sceDeci2Open(0x210, &ttyState, sceTtyHandler);
    if ((*(volatile s32 *)&ttyState.handle) < 0) {
        return 0;
    }
    (*(volatile s32 *)&ttyState.sendBusy) = 0;
    (*(volatile s32 *)&ttyState.sendLength) = 0;
    (*(volatile s32 *)&ttyState.receiveLength) = 0;
    ttyState.receiveBuffer = (u8 *)(EE_ADDR(&ttyReceivePacket) | 0x20000000);
    packet = (ttyPacket_t *)(EE_ADDR(&ttySendPacket) | 0x20000000);
    ttyState.sendBuffer = (u8 *)packet;
    packet->protocol = 0x210;
    packet->source = 'E';
    packet->destination = 'H';
    packet->reserved = 0;
    packet->ttyReserved = 0;
    ttyState.queue = gcc2_compiled__10(0x100);
    return 1;
}

const char ttyMsgPacketTooLarge[40] = "TTY: packet size larger than expect\n";

const char ttyMsgReceiveError[24] = "TTY: receive error";

const char ttyMsgSendError[24] = "TTY: send err %d\n";

const char ttyMsgWlenError[24] = "TTY: err ti->wlen=%08x\n";
