#include "shared/sdk/libmrpc/decls.h"

s32 sceSifMBindRpc(rpcClient *client, u32 serverId, s32 mode)
{
    return sceSifMBindRpcParam(client, serverId, mode, 0x800, 0x2000, 0x20);
}
