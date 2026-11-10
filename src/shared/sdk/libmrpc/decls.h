/* Declarations of smt3-decomp src/smt3/sdk/libmrpc.c, for its bodies in this directory. */
#ifndef SHARED_SDK_LIBMRPC_H
#define SHARED_SDK_LIBMRPC_H

#include "common.h"

#include "sdk_libmrpc.h"

extern void *memcpy(void *, const void *, size_t);

typedef struct sceSifMClientData rpcClient;

/* The unbind request prefix after its 16-byte SIF command header. */
typedef struct rpcUnbindPacket {
    u32 commandHeader[4];
    u32 flags;
    void *self;
    u32 sequence;
    rpcClient *client;
    u32 serverId;
    void *server;
} rpcUnbindPacket;

/* The allocator and command views occupy the same 64-byte packet storage. */
typedef union rpcPacket {
    u32 w[16];
    rpcUnbindPacket unbind;
} rpcPacket;

typedef char rpcPacketSize[(sizeof(rpcPacket) == 64) ? 1 : -1];

typedef char rpcPacketAlignment[(__alignof__(rpcPacket) == __alignof__(u32)) ? 1 : -1];

/* Observed prefix of the 0x40-byte M-RPC queue allocation. The remaining
 * bytes have no established fields and are not accessed through this view. */
typedef struct rpcQueue {
    u32 seq;
    rpcPacket *base;
    s32 count;
    u32 unk_C; /* Cleared during initialization. */
    u32 unk_10; /* Cleared during initialization. */
} rpcQueue;

extern s32 sceSifMRpcInitialized __attribute__((section(".data")));

extern rpcQueue sceSifMRpcQueue __attribute__((section(".data")));

extern s32 DIntr(void);

extern s32 EIntr(void);

extern rpcPacket sceSifMRpcPackets[32];

extern void _request_end(void *packet, void *queue);

extern void sceSifAddCmdHandler(s32 command, void (*handler)(void *, void *), void *argument);

extern u32 sceSifGetSreg(s32 index);

extern s32 sceSifSendCmd(s32 command, void *packet, s32 size, void *source, void *destination, s32 length);

void sceSifMInitRpc(s32 mode);

void sceSifMExitRpc(void);

rpcPacket *_sceRpcGetPacket(rpcQueue *queue);

void _sceRpcFreePacket(rpcPacket *p);

/* The M-RPC client extends the shared client prefix. Packet addresses are
 * opaque, as in SifRpcClientData_t in include/irx/iop.h. */
struct sceSifMClientData {
    void *packet;
    u32 sequence;
    s32 sema;
    u32 unk_C;
    u32 serverId;
    void *buffer;
    void *cbuffer;
    void (*endFunction)(void *);
    void *endParam;
    void *server;
    s32 completionSema;
    s32 result;
    u32 parameters[3];
};

/* Observed completion-response prefix; fields after this prefix are unknown. */
typedef struct rpcResponse {
    u32 commandHeader[4];
    u32 flags;
    void *self;
    u32 sequence;
    rpcClient *client;
    u32 command;
    union {
        void *server;
        s32 result;
    } value;
    void *buffer;
    void *cbuffer;
} rpcResponse;

extern s32 iSignalSema(s32 sema);

void _request_end(void *packet, void *queue);

typedef struct sdfSemaParam_t {
    s32 count;
    s32 maxCount;
    s32 initCount;
    s32 waitThreads;
    u32 attr;
    u32 option;
} sdfSemaParam_t;

extern s32 CreateSema(sdfSemaParam_t *param);

extern s32 WaitSema(s32 sema);

extern s32 DeleteSema(s32 sema);

extern s32 scePrintf(const char *format, ...);

extern s32 sceSifSendCmd(s32 command, void *packet, s32 size, void *source, void *destination, s32 length);

extern char sceSifMModeErrorString[];

s32 sceSifMBindRpcParam(rpcClient *client, u32 serverId, s32 mode, u32 arg0, u32 arg1, u32 arg2);

s32 sceSifMBindRpc(rpcClient *client, u32 serverId, s32 mode);

s32 sceSifMBindRpcParam(rpcClient *client, u32 serverId, s32 mode, u32 arg0, u32 arg1, u32 arg2);

/* The EE command transfers the four-byte server-pointer representation. */
typedef char rpcUnbindServerPointerSize[(sizeof(((rpcClient *)0)->server) == 4) ? 1 : -1];

typedef char rpcUnbindRequestPointerSize[(sizeof(((rpcUnbindPacket *)0)->server) == 4) ? 1 : -1];

s32 sceSifMUnBindRpc(rpcClient *client, s32 mode);

extern void sceSifWriteBackDCache(void *address, s32 size);

s32 sceSifMCallRpc(rpcClient *client, u32 function, u32 mode,
                   void *send, s32 sendSize, void *receive, s32 receiveSize,
                   void (*endFunction)(void *), void *endParam);

#endif
