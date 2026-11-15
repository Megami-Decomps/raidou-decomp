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

#include "sdk_libkernl.h"

typedef struct sifCommandHeader_t {
    u32 packetSize : 8;
    u32 dataSize : 24;
    u32 destination;
    u32 command;
    u32 option;
} sifCommandHeader_t;

SIZE_CHECK(sifCommandHeader_size_must_be_16, sifCommandHeader_t, 16);

/* Request pool: common 32-byte header followed by its command payload. */
typedef struct rpcRequestHeader {
    u32 commandHeader[4];
    u32 flags;
    void *packetAddress;
    u32 sequence;
    void *clientAddress;
} rpcRequestHeader;

SIZE_CHECK(rpcRequestHeader_size, rpcRequestHeader, 32);

SIZE_CHECK(rpcRequestHeader_pointer_size, void *, 4);

SIZE_CHECK(rpcRequestHeader_word_size, u32, 4);

typedef struct rpcPacket {
    rpcRequestHeader header;
    union {
        u32 words[8];
        struct {
            s32 function;
            s32 sendBytes;
            void *receive;
            s32 receiveBytes;
            u32 responseMode;
            void *server;
            u32 tail[2];
        } call;
    } payload;
} rpcPacket;

SIZE_CHECK(rpcPacket_payload_size, rpcPacket, 64);

/* Response pools retain a common nine-word header and a bind payload. */
typedef struct rpcResponsePacket {
    u32 header[9];
    union {
        u32 words[3];
        struct {
            sifRpcServerData_t *server;
            void *buffer;
            void *callbackBuffer;
        } bind;
    } payload;
    u32 tail[4];
} rpcResponsePacket;

SIZE_CHECK(rpcResponsePacket_size, rpcResponsePacket, 0x40);

typedef struct rpcQueue {
    u32 seq;
    rpcPacket *base;
    s32 count;
    u32 unk_C; /* Cleared by sceSifInitRpc. */
    u32 unk_10; /* Cleared by sceSifInitRpc. */
    rpcResponsePacket *forwardBase;
    s32 forwardCount;
    rpcResponsePacket *indexedBase;
    s32 indexedCount;
    s32 forwardIndex;
    sifRpcDataQueue_t *queues;
} rpcQueue;

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

/* Incoming ISR scratch command, not an overlay on the outgoing packet pool. */
typedef struct rpcBindRequest {
    u32 commandHeader[4];
    u32 flags;
    u32 packetAddress;
    u32 sequence;
    u32 client;
    s32 serviceId;
} rpcBindRequest;

SIZE_CHECK(rpcBindRequest_size, rpcBindRequest, 0x24);

s32 sceSifBindRpc(rpcClient *client, u32 serverId, s32 mode);


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


extern u8 kernelBuiltinVersion[4] __attribute__((section(".data")));

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

extern lfRequest_t lfRequest;
extern rpcClient lfRpcClient;
extern s32 _lf_bind(void);
extern s32 _lf_version(void);
extern s32 lfBindStatus;
extern u8 lfDetectedVersion[0x18];
extern u8 kernelBuiltinVersion[4];
extern void *lfFallbackVersion;
s32 _lf_bind(void);
s32 _lf_version(void);
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

typedef struct {
    u32 index;
    void (*entry)(void);
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

typedef struct {
    u32 pageMask;
    u32 entryHi;
    u32 entryLo0;
    u32 entryLo1;
} kernelTlbEntry_t;

/* InitTLB32MB loads the kernel and default entries, stores how many it wrote in
 * loadedCount, then loads the extended ones. */
typedef struct {
    s32 kernelCount;
    s32 defaultCount;
    s32 extendedCount;
    s32 loadedCount;
    kernelTlbEntry_t *kernelEntries;
    kernelTlbEntry_t *defaultEntries;
    kernelTlbEntry_t *extendedEntries;
} kernelTlbTable_t;

s32 _lf_bind(void)
{
    s32 delay;

    if (lfBindStatus < 0) {
        for (;;) {
            if (sceSifBindRpc(&lfRpcClient, 0x80000006, 0) < 0) {
                return -1;
            }
            if (lfRpcClient.server != NULL) {
                lfBindStatus = 0;
                if (sceSifCallRpc(&lfRpcClient, 0xFF, 0, NULL, 0,
                                  &lfRequest, 4, NULL, NULL) < 0) {
                    return -65537;
                }
                memcpy(lfDetectedVersion, &lfRequest, 4);
                return 0;
            }
            for (delay = 0x100000; delay != -1; delay--) {
            }
        }
    }
    return 0;
}

s32 _lf_version(void)
{
    const void *builtin = kernelBuiltinVersion;
    const void *detected = lfDetectedVersion;
    s32 mismatch = 0;

    if (memcmp(detected, builtin, 4) != 0) {
        if (memcmp(detected, lfFallbackVersion, 4) != 0) {
            mismatch = memcmp(builtin, lfFallbackVersion, 4) != 0;
        }
    }
    return mismatch;
}

s32 sceSifLoadFileReset(void)
{
    lfBindStatus = -1;
    memset(lfDetectedVersion, 0, 4);
    return 0;
}

s32 _sceSifLoadModuleBuffer(void *buffer, s32 argumentSize, const void *arguments, s32 *moduleResult)
{
    s32 result;

    if (_lf_bind() < 0) {
        return -65536;
    }
    if (_lf_version() != 0) {
        return -65540;
    }
    lfRequest.value = EE_ADDR_S(buffer);
    if (arguments != NULL) {
        if (argumentSize > 0xFC) {
            memcpy(lfRequest.arguments, arguments, 0xFC);
            lfRequest.moduleResult = 0xFC;
        } else {
            memcpy(lfRequest.arguments, arguments, argumentSize);
            lfRequest.moduleResult = argumentSize;
        }
    } else {
        lfRequest.moduleResult = 0;
    }
    if (sceSifCallRpc(&lfRpcClient, 6, 0, &lfRequest, 0x200,
                      &lfRequest, 8, NULL, NULL) < 0) {
        return -65537;
    }
    result = lfRequest.value;
    *moduleResult = lfRequest.moduleResult;
    return result;
}

s32 sceSifStopModule(s32 moduleId, s32 argumentSize, const void *arguments, s32 *moduleResult)
{
    s32 result;

    if (_lf_bind() < 0) {
        return -65536;
    }
    if (_lf_version() != 0) {
        return -65540;
    }
    lfRequest.value = moduleId;
    if (arguments != NULL) {
        if (argumentSize > 0xFC) {
            memcpy(lfRequest.arguments, arguments, 0xFC);
            lfRequest.moduleResult = 0xFC;
        } else {
            memcpy(lfRequest.arguments, arguments, argumentSize);
            lfRequest.moduleResult = argumentSize;
        }
    } else {
        lfRequest.moduleResult = 0;
    }
    if (sceSifCallRpc(&lfRpcClient, 7, 0, &lfRequest, 0x200,
                      &lfRequest, 8, NULL, NULL) < 0) {
        return -65537;
    }
    result = lfRequest.value;
    *moduleResult = lfRequest.moduleResult;
    return result;
}

s32 sceSifUnloadModule(s32 moduleId)
{
    if (_lf_bind() < 0) {
        return -65536;
    }
    if (_lf_version() != 0) {
        return -65540;
    }
    lfRequest.value = moduleId;
    if (sceSifCallRpc(&lfRpcClient, 8, 0, &lfRequest, 4,
                      &lfRequest, 4, NULL, NULL) < 0) {
        return -65537;
    }
    return lfRequest.value;
}

s32 sceSifSearchModuleByName(const char *name)
{
    if (_lf_bind() < 0) {
        return -65536;
    }
    if (_lf_version() != 0) {
        return -65540;
    }
    strncpy(lfRequest.name, name, 0xFC);
    lfRequest.name[0xFB] = 0;
    if (sceSifCallRpc(&lfRpcClient, 9, 0, &lfRequest, 512,
                      &lfRequest, 4, NULL, NULL) < 0) {
        return -65537;
    }
    return lfRequest.value;
}

s32 sceSifSearchModuleByAddress(void *address)
{
    if (_lf_bind() < 0) {
        return -65536;
    }
    if (_lf_version() != 0) {
        return -65540;
    }
    lfRequest.value = EE_ADDR_S(address);
    if (sceSifCallRpc(&lfRpcClient, 10, 0, &lfRequest, 4,
                      &lfRequest, 4, NULL, NULL) < 0) {
        return -65537;
    }
    return lfRequest.value;
}

s32 sceSifLoadModuleBuffer(void *buffer, s32 argumentSize, const void *arguments)
{
    s32 moduleResult;

    return _sceSifLoadModuleBuffer(buffer, argumentSize, arguments, &moduleResult);
}

s32 sceSifLoadStartModuleBuffer(void *buffer, s32 argumentSize, const void *arguments,
                                s32 *moduleResult)
{
    return _sceSifLoadModuleBuffer(buffer, argumentSize, arguments, moduleResult);
}

s32 _sceSifLoadModule(const char *path, s32 argumentSize, const void *arguments,
                      s32 *moduleResult, s32 command)
{
    s32 result;

    if (_lf_bind() < 0) {
        return -65536;
    }
    if (_lf_version() != 0) {
        return -65540;
    }
    strncpy(lfRequest.name, path, 0xFC);
    lfRequest.name[0xFB] = 0;
    if (arguments != NULL) {
        if (argumentSize > 0xFC) {
            memcpy(lfRequest.arguments, arguments, 0xFC);
            lfRequest.value = 0xFC;
        } else {
            memcpy(lfRequest.arguments, arguments, argumentSize);
            lfRequest.value = argumentSize;
        }
    } else {
        lfRequest.arguments[0] = 0;
        lfRequest.value = 0;
    }
    if (sceSifCallRpc(&lfRpcClient, command, 0, &lfRequest, 0x200,
                      &lfRequest, 8, NULL, NULL) < 0) {
        return -65537;
    }
    result = lfRequest.value;
    *moduleResult = lfRequest.moduleResult;
    return result;
}

s32 sceSifLoadModule(const char *path, s32 argumentSize, const void *arguments)
{
    s32 moduleResult;

    return _sceSifLoadModule(path, argumentSize, arguments, &moduleResult, 0);
}

s32 sceSifLoadStartModule(const char *path, s32 argumentSize, const void *arguments,
                          s32 *moduleResult)
{
    return _sceSifLoadModule(path, argumentSize, arguments, moduleResult, 0);
}

s32 _sceSifLoadElfPart(const char *path, const char *section, u32 *result, s32 command)
{
    if (_lf_bind() < 0) {
        return -65536;
    }
    if (_lf_version() != 0) {
        return -65540;
    }
    strncpy(lfRequest.name, path, 0xFC);
    lfRequest.name[0xFB] = 0;
    strncpy((char *)lfRequest.arguments, section, 0xFC);
    lfRequest.arguments[0xFB] = 0;
    if (sceSifCallRpc(&lfRpcClient, command, 0, &lfRequest, 0x200,
                      &lfRequest, 16, NULL, NULL) < 0) {
        return -65537;
    }
    if (lfRequest.value == 0) {
        return -65539;
    }
    result[0] = lfRequest.value;
    result[1] = lfRequest.moduleResult;
    return 0;
}

s32 sceSifLoadElfPart(const char *path, const char *section, u32 *result)
{
    return _sceSifLoadElfPart(path, section, result, 1);
}

s32 sceSifLoadElf(const char *path, u32 *result)
{
    return _sceSifLoadElfPart(path, "all", result, 1);
}

s32 sceSifGetIopAddr(void *address, void *output, u32 kind)
{
    if (_lf_bind() < 0) {
        return -65536;
    }
    if (kind >= 3) {
        return -65538;
    }
    lfRequest.value = EE_ADDR_S(address);
    lfRequest.moduleResult = kind;
    if (sceSifCallRpc(&lfRpcClient, 3, 0, &lfRequest, 32,
                      &lfRequest, 32, NULL, NULL) < 0) {
        return -65537;
    }
    if (kind == 0) {
        *(u8 *)output = *(u8 *)&lfRequest.value;
    } else if (kind == 1) {
        *(u16 *)output = *(u16 *)&lfRequest.value;
    } else if (kind == 2) {
        *(u32 *)output = lfRequest.value;
    } else {
        return -65538;
    }
    return 0;
}

s32 sceSifSetIopAddr(void *address, const void *input, u32 kind)
{
    if (_lf_bind() < 0) {
        return -65536;
    }
    lfRequest.value = EE_ADDR_S(address);
    lfRequest.moduleResult = kind;
    if (kind == 0) {
        *(u8 *)lfRequest.name = *(const u8 *)input;
    } else if (kind == 1) {
        *(u16 *)lfRequest.name = *(const u16 *)input;
    } else if (kind == 2) {
        *(u32 *)lfRequest.name = *(const u32 *)input;
    } else {
        return -65538;
    }
    {
        s32 status = sceSifCallRpc(&lfRpcClient, 2, 0, &lfRequest, 32,
                               &lfRequest, 16, NULL, NULL);
        /* Keep the selected post-RPC return value in its ABI register.
         * This is a target-specific matching aid, not an original-source claim. */
        FAKEMATCH_ASM_REG(s32, result, "v0") = status < 0 ? -65537 : 0;
        return result;
    }
}
