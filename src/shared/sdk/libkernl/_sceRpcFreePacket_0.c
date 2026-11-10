#include "shared/sdk/libkernl/decls.h"

void _sceRpcFreePacket_0(rpcPacket *packet)
{
    u32 flags = packet->w[4];

    packet->w[6] = 0;
    packet->w[4] = flags & ~1;
}
