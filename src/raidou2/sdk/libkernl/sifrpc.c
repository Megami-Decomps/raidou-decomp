#include "common.h"

INCLUDE_ASM(const s32, "sdk/libkernl/sifrpc", sceSifInitRpc);

#include "shared/sdk/libkernl/sceSifExitRpc.c"

#include "shared/sdk/libkernl/_sceRpcGetPacket_0.c"

#include "shared/sdk/libkernl/_sceRpcFreePacket_0.c"

#include "shared/sdk/libkernl/_sceRpcGetFPacket.c"

#include "shared/sdk/libkernl/_sceRpcGetFPacket2.c"

#include "shared/sdk/libkernl/_request_end_0.c"

#include "shared/sdk/libkernl/_request_rdata.c"

#include "shared/sdk/libkernl/sceSifGetOtherData.c"

#include "shared/sdk/libkernl/_search_svdata.c"

INCLUDE_ASM(const s32, "sdk/libkernl/sifrpc", _request_bind);

#include "shared/sdk/libkernl/sceSifBindRpc.c"

INCLUDE_ASM(const s32, "sdk/libkernl/sifrpc", _request_call);

INCLUDE_ASM(const s32, "sdk/libkernl/sifrpc", sceSifCallRpc);

INCLUDE_ASM(const s32, "sdk/libkernl/sifrpc", sceSifCheckStatRpc);

#include "shared/sdk/libkernl/sceSifSetRpcQueue.c"

#include "shared/sdk/libkernl/sceSifRegisterRpc.c"

#include "shared/sdk/libkernl/sceSifRemoveRpc.c"

#include "shared/sdk/libkernl/sceSifRemoveRpcQueue.c"

#include "shared/sdk/libkernl/sceSifGetNextRequest.c"

#include "shared/sdk/libkernl/sceSifExecRequest.c"

#include "shared/sdk/libkernl/sceSifRpcLoop.c"
