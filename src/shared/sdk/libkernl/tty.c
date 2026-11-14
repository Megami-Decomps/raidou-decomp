#include "common.h"

/* Declarations from smt3-decomp src/smt3/sdk/libkernl.c */
#include "common.h"

#include "newlib_stat.h"

#include "sdk_libgcc.h"

#include "ps2asm.h"

#include "sif_rpc.h"

#include "ps2asm.h"

extern s32 ttyInitDone __attribute__((section(".data")));

void sceResetttyinit(void);

extern s32 DIntr(void);

extern s32 EIntr(void);

void VSync(void);

extern void SetVSyncFlag(volatile u32 *flag, volatile u64 *result);

u64 VSync2(void);

extern s32 ttyInitDone __attribute__((section(".data")));

extern s32 sceTtyInit(void);

extern s32 sceTtyRead(char *buffer, s32 length);

extern s32 sceTtyWrite(const char *buffer, s32 length);

s32 write(s32 descriptor, const void *buffer, s32 length);

s32 read(s32 descriptor, void *buffer, s32 length);

extern s32 *__errno_8(void);

s32 open(const char *path, s32 flags, s32 mode);

s32 close(s32 descriptor);

/* Native reads no input registers; parameter types remain unrecovered. */
s32 ioctl();

long lseek(s32 descriptor, s32 offset, s32 whence);

s32 isatty(s32 descriptor);

s32 fstat(s32 fd, struct stat *st);

s32 libc_2DC670(void);

extern void Exit(s32 status);

s32 kill(s32 pid, s32 signal);

s32 stat(const char *path, struct stat *result);

s32 unlink(const char *path);

extern void _sceSDC(void *start, void *end);

void iSyncDCache(void *start, void *end);

extern void _sceIDC(void *start, void *end);

void iInvalidDCache(void *start, void *end);

typedef struct threadMgrQueue_t {
    u32 readIndex;
    u32 writeIndex;
    u8 commands[512][2];
} threadMgrQueue_t;

extern s32 threadMgrSema;

extern s32 WaitSema(s32 sema);

extern s32 WakeupThread(s32 threadId);

extern s32 RotateThreadReadyQueue(s32 priority);

extern s32 SuspendThread(s32 threadId);

extern void kprintf(const char *format, ...);

extern const char kernlInternalErrorMsg[];

void topThread(void *argument);

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

extern s32 topThreadId __attribute__((section(".data")));

extern s32 threadMgrSema;

extern u8 topThreadStack[0x400];

extern u8 _gp[];

extern threadMgrQueue_t threadCommandQueue;

extern void topThread(void *queue);

extern s32 CreateThread(kernelThreadParam_t *param);

extern s32 DeleteSema(s32 sema);

extern s32 GetThreadId(void);

extern s32 ChangeThreadPriority(s32 threadId, s32 priority);

s32 InitThread(void);

extern s32 iSignalSema(s32 sema);

s32 iRotateThreadReadyQueue(s32 priority);

typedef struct ttyQueue_t {
    u32 capacity;
    /* The TTY callback produces bytes while sceTtyRead polls occupancy. */
    volatile s32 count;
    u8 *read;
    u8 *write;
    u8 data[256];
} ttyQueue_t;

extern ttyQueue_t ttyQueueStorage;

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

extern void kprintf(const char *format, ...);

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

s32 kputchar(s32 character);

extern s32 deci2LineIndex __attribute__((section(".data")));

extern char deci2LineBuffer[128];

extern s32 kputs(const char *text);

void deci2Putchar(s32 character);

extern s32 kputchar(s32 character);

void serialPutchar(s32 character);

/* ftoi consumes the integer bits directly, not a numeric double conversion. */
extern s32 ftoi(u64 bits);

extern void (*printfPutchar)(s32 character) __attribute__((section(".data")));

void printfloat(f64 value);

/* In ee-gcc 2.9, next_arg names the first incoming stack argument. These
 * one-fixed-argument wrappers save the seven remaining argument GPRs in
 * contiguous eight-byte slots immediately before it. The formatter walks
 * that register-save area and then continues into the stack arguments. */
typedef char *kernelVaList_t;

extern void _printf(const char *format, kernelVaList_t arguments);

extern void (*printfPutchar)(s32 character) __attribute__((section(".data")));

void kprintf(const char *format, ...);

void scePrintf(const char *format, ...);

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

extern sifCommandContext_t sifCommandContext;

extern u32 sifSregs[];

typedef struct {
    u8 header[16];
    u32 index;
    u32 value;
} sifRegisterPacket_t;

typedef struct {
    u8 header[16];
    void *address;
} sifAddressPacket_t;

void gcc2_compiled__12(void *packet, void *argument);

void _change_addr(void *packet, void *argument);

u32 sceSifGetSreg(s32 index);

u32 sceSifSetSreg(s32 index, u32 value);

sifCommandContext_t *sceSifGetDataTable(void);

extern s32 DisableDmac(s32 channel);

extern s32 RemoveDmacHandler(s32 channel, s32 handlerId);

extern s32 sifDmacHandlerId;

extern s32 D_r1_00442CE0;

void sceSifExitCmd(void);

sifCommandHandler_t *sceSifSetCmdBuffer(sifCommandHandler_t *buffer, s32 count);

sifCommandHandler_t *sceSifSetSysCmdBuffer(sifCommandHandler_t *buffer, s32 count);

/* The sign selects the table. Scaling in 32 bits drops that selector and
 * gives the native eight-byte entry offset. Once tested, command is reused
 * for the selected table's actual address; its old value is no longer live.
 */
void sceSifAddCmdHandler(s32 command, void (*handler)(void *, void *), void *argument);

void sceSifRemoveCmdHandler(s32 command);

/* DMA bus addresses and transfer controls occupy four 32-bit words. */
typedef struct sifDmaTransfer_t {
    u32 source;
    u32 destination;
    s32 size;
    u32 attributes;
} sifDmaTransfer_t;

SIZE_CHECK(sifDmaTransfer_size_must_be_16, sifDmaTransfer_t, 16);

#include "sdk_libkernl.h"

extern s32 isceSifSetDma(sifDmaTransfer_t *transfers, s32 count);

extern void sceSifWriteBackDCache(void *address, s32 size);

typedef struct sifCommandHeader_t {
    u32 packetSize : 8;
    u32 dataSize : 24;
    u32 destination;
    u32 command;
    u32 option;
} sifCommandHeader_t;

SIZE_CHECK(sifCommandHeader_size_must_be_16, sifCommandHeader_t, 16);

s32 _sceSifSendCmd(s32 command, s32 interruptMode, void *packet, s32 packetBytes,
                    void *source, void *destination, s32 dataBytes);

s32 sceSifSendCmd(s32 command, void *packet, s32 packetBytes,
                  void *source, void *destination, s32 dataBytes);

s32 isceSifSendCmd(s32 command, void *packet, s32 packetBytes,
                   void *source, void *destination, s32 dataBytes);

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

extern rpcQueue sifRpcQueue;

extern rpcPacket rpcPackets[32];

extern rpcPacket rpcForwardPackets[32];

extern rpcPacket rpcIndexedPackets[32];

extern s32 sifRpcInitialized;

extern void sceSifInitCmd(void);

extern u32 sceSifGetReg(u32 registerId);

extern s32 sceSifSetReg(u32 registerId, u32 value);

extern void sceSifAddCmdHandler(s32 command, void (*handler)(void *, void *), void *argument);

extern void _request_end_0(void *packet, void *queue);

extern void _request_bind(void *packet, void *queue);

extern void _request_call(void *packet, void *queue);

extern void _request_rdata(rpcPacket *packet, rpcQueue *queue);

extern s32 sifRpcInitialized;

void sceSifExitRpc(void);

extern s32 DIntr(void);

extern s32 EIntr(void);

rpcPacket *_sceRpcGetPacket_0(rpcQueue *queue);

void _sceRpcFreePacket_0(rpcPacket *packet);

rpcPacket *_sceRpcGetFPacket(rpcQueue *queue);

rpcPacket *_sceRpcGetFPacket2(rpcQueue *queue, s32 index);

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

void _request_end_0(void *packet, void *queue);

void _request_rdata(rpcPacket *request, rpcQueue *queue);

extern s32 WaitSema(s32 sema);

extern s32 DeleteSema(s32 sema);

s32 sceSifGetOtherData(rpcTransactionPrefix *client, void *source, void *destination, s32 size, s32 mode);

sifRpcServerData_t *_search_svdata(s32 serviceId, rpcQueue *queue);

s32 sceSifBindRpc(rpcClient *client, u32 serverId, s32 mode);

typedef struct rpcClientPrefix {
    void *packet;
    u32 sequence;
} rpcClientPrefix;

void sceSifSetRpcQueue(sifRpcDataQueue_t *queue, s32 threadId);

void sceSifRegisterRpc(sifRpcServerData_t *server, u32 serviceId,
                        sifRpcFunc_t handler, void *buffer,
                        sifRpcFunc_t callback, void *callbackBuffer,
                        sifRpcDataQueue_t *queue);

sifRpcServerData_t *sceSifRemoveRpc(sifRpcServerData_t *server, sifRpcDataQueue_t *queue);

sifRpcDataQueue_t *sceSifRemoveRpcQueue(sifRpcDataQueue_t *queue);

sifRpcServerData_t *sceSifGetNextRequest(sifRpcDataQueue_t *queue);

void sceSifExecRequest(sifRpcServerData_t *server);

extern void sceSifExecRequest(sifRpcServerData_t *request);

extern s32 SleepThread(void);

void sceSifRpcLoop(sifRpcDataQueue_t *queue);

extern s32 fsIobSema __attribute__((section(".data")));

extern s32 fsIoctlSema __attribute__((section(".data")));

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

extern u8 fsReadResultBuffer[];

extern s32 fsRequestSemas[32];

extern void *memcpy(void *destination, const void *source, size_t size);

extern s32 iSignalSema(s32 sema);

extern s32 fsCommandSema __attribute__((section(".data")));

void _sceFsSemInit(void);

s32 _sceFsWaitS(s32 command);

extern s32 SignalSema(s32 sema);

/* Internal notification: callers discard the SignalSema status. */
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

extern u8 fsCompletionBuffer[];

extern u8 fsVersionRpcBuffer[];

extern fsRegistrationPacket_t fsRegistration;

extern void sceSifInitRpc(s32 mode);

extern void sceSifAddCmdHandler(s32 command, void (*handler)(void *, void *), void *argument);

extern void _sceFs_Rcv_Intr(void *packet, void *argument);

extern s32 sceSifCallRpc(void *client, s32 function, s32 mode, void *send,
                         s32 sendBytes, void *receive, s32 receiveBytes,
                         void *endFunction, void *endParameter);

extern u8 fsDetectedVersion[0x18];

extern s32 fsInitialized __attribute__((section(".data")));

extern s32 SignalSema(s32 sema);

s32 sceFsInit(void);

extern u8 fsDetectedVersion[0x18];

extern u8 kernelBuiltinVersion[4] __attribute__((section(".data")));

extern const void *fsFallbackVersion __attribute__((section(".data")));

extern s32 memcmp(const void *left, const void *right, size_t count);

s32 _fs_version(void);

extern s32 fsInitialized __attribute__((section(".data")));

extern void *memset(void *destination, s32 value, size_t count);

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

extern fsIoctlStatus_t fsIoctlStatus;

extern void *fsIoctlArgument;

extern s32 fsRequestSemas[32];

extern void *memcpy(void *destination, const void *source, size_t size);

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

s32 sceDevctl(const char *device, s32 command, const void *args, u32 argSize,
              void *output, u32 outputSize);

s32 sceSymlink(const char *oldPath, const char *newPath);

s32 sceReadlink(const char *path, char *buffer, u32 count);

extern s32 iopHeapStatus;

/* The BSS map bounds this opaque RPC client to 0x40 bytes. */
extern u8 iopHeapRpcClient[0x40];

extern s32 iopHeapResult;

/* Shared heap request storage is 0x40 bytes in the BSS map. */
extern addr_t iopHeapArgs[16];

extern s32 sceSifCallRpc(void *client, s32 function, s32 mode, void *send,
                         s32 sendBytes, void *receive, s32 receiveBytes,
                         void *endFunction, void *endParameter);

s32 sceSifInitIopHeap(void);

s32 sceSifAllocIopHeap(s32 size);

void *sceSifAllocSysMemory(s32 mode, s32 size, void *address);

s32 sceSifFreeSysMemory(void *address);

s32 sceSifFreeIopHeap(void *address);

typedef struct heapLoadRequest_t {
    u32 address;
    char name[0xFC];
} heapLoadRequest_t;

extern heapLoadRequest_t iopHeapLoadRequest;

s32 sceSifLoadIopHeap(const char *path, u32 address);

u32 sceSifQueryMemSize(void);

u32 sceSifQueryMaxFreeMemSize(void);

u32 sceSifQueryTotalFreeMemSize(void);

/* The IOP block key and query results travel as 32-bit words. */
u32 sceSifQueryBlockTopAddress(u32 block);

u32 sceSifQueryBlockSize(u32 block);

/* The load-file RPC sends this complete 0x200-byte shared request region. */
typedef struct lfRequest_t {
    s32 value;
    s32 moduleResult; /* Argument size on buffer-load/stop requests; result on replies. */
    char name[0xFC];
    u8 arguments[0xFC];
} lfRequest_t;

extern lfRequest_t lfRequest;

extern rpcClient lfRpcClient;

extern s32 _lf_bind(void);

extern s32 _lf_version(void);

extern char *strncpy(char *destination, const char *source, size_t count);

extern s32 lfBindStatus;

extern u8 lfDetectedVersion[0x18];

extern u8 kernelBuiltinVersion[4];

extern void *lfFallbackVersion;

extern void *memcpy(void *destination, const void *source, size_t size);

extern s32 memcmp(const void *a, const void *b, size_t size);

s32 _lf_bind(void);

s32 _lf_version(void);

extern void *memset(void *destination, s32 value, size_t size);

s32 sceSifLoadFileReset(void);

s32 _sceSifLoadModuleBuffer(void *buffer, s32 argumentSize, const void *arguments, s32 *moduleResult);

s32 sceSifStopModule(s32 moduleId, s32 argumentSize, const void *arguments, s32 *moduleResult);

s32 sceSifUnloadModule(s32 moduleId);

s32 sceSifSearchModuleByName(const char *name);

s32 sceSifSearchModuleByAddress(void *address);

s32 sceSifLoadModuleBuffer(void *buffer, s32 argumentSize, const void *arguments);

s32 sceSifLoadStartModuleBuffer(void *buffer, s32 argumentSize, const void *arguments,
                                s32 *moduleResult);

s32 _sceSifLoadModule(const char *path, s32 argumentSize, const void *arguments,
                      s32 *moduleResult, s32 command);

extern s32 _sceSifLoadModule(const char *path, s32 argumentSize, const void *arguments,
                              s32 *moduleResult, s32 command);

s32 sceSifLoadModule(const char *path, s32 argumentSize, const void *arguments);

s32 sceSifLoadStartModule(const char *path, s32 argumentSize, const void *arguments,
                          s32 *moduleResult);

s32 _sceSifLoadElfPart(const char *path, const char *section, u32 *result, s32 command);

s32 sceSifLoadElfPart(const char *path, const char *section, u32 *result);

extern const char D_0054BCA8[8];

extern const char D_0054BCB0[8];

s32 sceSifLoadElf(const char *path, u32 *result);

s32 sceSifGetIopAddr(void *address, void *output, u32 kind);

s32 sceSifSetIopAddr(void *address, const void *input, u32 kind);

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

extern sifResetPacket_t sifResetPacket;

extern void sceSifStopDma(void);

extern u32 sceSifGetReg(u32 registerId);

extern s32 sceSifSetReg(u32 registerId, u32 value);

extern void sceSifInitRpc(s32 mode);

extern void sceSifExitRpc(void);

s32 sceSifResetIop(const char *arguments, s32 mode);

extern void sceResetttyinit(void);

s32 sceSifIsAliveIop(void);

s32 sceSifSyncIop(void);

extern s32 mallocSema;

extern s32 frameObjectSema;

void supplement_crt0(void);

s32 kCopy(u32 *dst, const u32 *src, u32 count);

u32 *kFindAddress(u32 *start, const u32 *end, u32 instruction);

extern u32 *systemCallTable __attribute__((section(".data")));

extern s32 Copy(void *dst, const void *src, u32 count);

u32 GetSystemCallEntry(s32 index);

typedef struct {
    u32 index;
    void *entry;
} kernelEntryPair_t;

extern kernelEntryPair_t systemCallEntryPairs[2];

extern void setup(u32 index, void *entry);

extern u32 *FindAddress(u32 *start, const u32 *end, u32 instruction);

void GetSystemCallTableEntry(void);

extern void supplement_crt0(void);

extern void GetSystemCallTableEntry(void);

extern void InitAlarm(void);

extern s32 InitThread(void);

extern void InitExecPS2(void);

extern void InitTLBFunctions(void);

void _InitSys(void);

s32 kCopy_0(u32 *dst, const u32 *src, u32 count);

extern void SetOsdConfigParam(const u32 *param);

extern void GetOsdConfigParam(u32 *param);

s32 PatchIsNeeded_0(void);

extern void FlushCache(s32 mode);

extern kernelEntryPair_t execEntryPairs[3];

extern const u8 execPs2Payload[0x7A8];

extern s32 PatchIsNeeded_0(void);

extern void gcc2_compiled__15(u32 index, void *entry);

extern s32 Copy_0(void *destination, const void *source, u32 size);

extern void *GetEntryAddress(u32 index);

void InitExecPS2(void);

s32 kCopy_1(u8 *dst, const u8 *src, u32 count);

typedef struct execArgumentBlock_t {
    char *arguments[16];
    char strings[1];
} execArgumentBlock_t;

extern void *execArgumentBlock;

extern kernelEntryPair_t setArgEntryPairs[1];

extern void PatchIsNeeded(u32 index, void *entry);

extern s32 gcc2_compiled__16(void *destination, const void *source, u32 size);

extern u32 strlen(const char *text);

char *SetArg(const char *path, s32 count, const char *const *arguments);

extern void InitTLB(void);

void TerminateLibrary(void);

extern const char execDefaultPath[];

extern s32 _ExecPS2(void *entry, void *gp, s32 count, char **arguments);

extern void libc_2DBAC0(s32 status);

extern void _ExecOSD(s32 count, char **arguments);

s32 ExecPS2(void *entry, void *gp, s32 count, const char *const *arguments);

extern void libc_2DBAE0(const char *path, s32 count, char **arguments);

void LoadExecPS2(const char *path, s32 count, char **arguments);

void Exit(s32 status);

void ExecOSD(s32 count, const char *const *arguments);

s32 kCopy_2(u32 *dst, const u32 *src, u32 count);

extern kernelEntryPair_t alarmEntryPairs[8];

extern kernelEntryPair_t tlbEntryPairs[8];

extern void *execArgumentBlock;

extern const u8 alarmPayload[0x740];

extern const u8 alarmSmallPayload[0x28];

extern const u8 TLBFunctionsImage[0x330];

extern void gcc2_compiled__17(u32 index, void *entry);

extern s32 Copy_1(void *destination, const void *source, u32 size);

extern s32 Copy_2(void *destination, const void *source, u32 size);

extern void *GetEntryAddress_0(u32 index);

extern void *GetEntryAddress_1(u32 index);

extern void setup_0(u32 index, void *entry);

void InitAlarm(void);

extern s32 Deci2Call(s32 command, void *packet);

extern u8 deci2PacketStorage[];

typedef void (*deci2Handler_t)(s32 event, s32 parameter, void *argument);

typedef struct deci2Registration_t {
    u32 protocol;
    void *argument;
    deci2Handler_t handler;
    void *storage;
} deci2Registration_t;

s32 sceDeci2Open(u16 protocol, void *argument, deci2Handler_t handler);

s32 sceDeci2Close(s32 handle);

s32 sceDeci2ReqSend(s32 handle, s8 destination);

s32 sceDeci2Poll(s32 handle);

typedef struct deci2Transfer_t {
    s32 handle;
    void *buffer;
    u32 length;
} deci2Transfer_t;

s32 sceDeci2ExRecv(s32 handle, void *buffer, u16 length);

s32 sceDeci2ExSend(s32 handle, void *buffer, u16 length);

s32 sceDeci2ExReqSend(s32 handle, s8 destination);

s32 sceDeci2ExLock(s32 handle);

s32 sceDeci2ExUnLock(s32 handle);

s32 kputs(const char *text);

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

/* Existing retail symbol is retained; this is the checked indexed setter,
 * not the separate interrupt-context PutTLBEntry syscall stub. */
s32 iPutTLBEntry(u32 index, u32 pageMask, u32 entryHi, u32 entryLo0, u32 entryLo1);

extern const char execDefaultPath[8];
/* End of declarations from src/smt3/sdk/libkernl.c */

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

