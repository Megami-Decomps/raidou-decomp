#ifndef SDK_LIBMRPC_H
#define SDK_LIBMRPC_H

#include "common.h"

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
typedef struct sceSifMClientData sceSifMClientData;
typedef void (*sceSifMEndFunc)(void *argument);

s32 sceSifMCallRpc(sceSifMClientData *client, u32 function, u32 mode,
                   void *send, s32 sendSize, void *receive, s32 receiveSize,
                   sceSifMEndFunc end, void *endArgument);

s32 sceSifMBindRpc(sceSifMClientData *client, u32 serverId, s32 mode);
s32 sceSifMBindRpcParam(sceSifMClientData *client, u32 serverId, s32 mode, u32 arg0, u32 arg1, u32 arg2);

#endif
