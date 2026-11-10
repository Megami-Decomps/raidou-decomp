#include "common.h"

#include "shared/sdk/libmrpc/sceSifMInitRpc.c"

INCLUDE_ASM(const s32, "sdk/libmrpc/msifrpc", func_0037C410);

#include "shared/sdk/libmrpc/_sceRpcGetPacket.c"

#include "shared/sdk/libmrpc/_sceRpcFreePacket.c"

#include "shared/sdk/libmrpc/_request_end.c"

#include "shared/sdk/libmrpc/sceSifMBindRpc.c"

#include "shared/sdk/libmrpc/sceSifMBindRpcParam.c"

#include "shared/sdk/libmrpc/sceSifMUnBindRpc.c"

INCLUDE_RODATA(const s32, "sdk/libmrpc/msifrpc", D_004660E8);

INCLUDE_ASM(const s32, "sdk/libmrpc/msifrpc", sceSifMCallRpc);

