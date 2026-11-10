#include "shared/sdk/libpad/decls.h"

int scePadGetPortMax(void)
{
    padRpcPacket.cmd = 0xC;
    if (sceSifCallRpc(padRpcClient, 1, 0, &padRpcPacket, 0x80, &padRpcPacket, 0x80, 0, 0) < 0) {
        return 0;
    }
    return padRpcPacket.a.w[0];
}
