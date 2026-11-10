#include "shared/sdk/libmrpc/decls.h"

void _sceRpcFreePacket(rpcPacket *p)
{
    u32 flags = p->w[4];

    p->w[6] = 0;
    p->w[4] = flags & ~1;
}
