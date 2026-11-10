#include "shared/sdk/libkernl/decls.h"

rpcPacket *_sceRpcGetFPacket(rpcQueue *queue)
{
    s32 index = queue->forwardIndex % queue->forwardCount;
    rpcPacket *packet = &queue->forwardBase[index];

    queue->forwardIndex = index + 1;
    return packet;
}
