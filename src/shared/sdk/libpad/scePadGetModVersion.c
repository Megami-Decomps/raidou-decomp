#include "shared/sdk/libpad/decls.h"

int scePadGetModVersion(void)
{
    padRpcPacket.cmd = 0x12;
    if (sceSifCallRpc(padRpcClient, 1, 0, &padRpcPacket, 0x80, &padRpcPacket, 0x80, 0, 0) < 0) {
        return 0;
    }
    return padRpcPacket.a.w[0];
}
