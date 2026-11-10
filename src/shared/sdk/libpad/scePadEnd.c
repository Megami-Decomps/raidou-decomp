#include "shared/sdk/libpad/decls.h"

int scePadEnd(void)
{
    padRpcPacket.cmd = 0xF;
    if (sceSifCallRpc(padRpcClient, 1, 0, &padRpcPacket, 0x80, &padRpcPacket, 0x80, 0, 0) < 0) {
        return 0;
    }
    if (padRpcPacket.a.w[0] == 1) {
        padInitialized = 0;
    }
    return padRpcPacket.a.w[0];
}
