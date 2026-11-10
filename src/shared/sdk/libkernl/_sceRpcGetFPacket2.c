#include "shared/sdk/libkernl/decls.h"

rpcPacket *_sceRpcGetFPacket2(rpcQueue *queue, s32 index)
{
    if (index < 0 || index >= queue->indexedCount) {
        return _sceRpcGetFPacket(queue);
    }
    return &queue->indexedBase[index];
}
