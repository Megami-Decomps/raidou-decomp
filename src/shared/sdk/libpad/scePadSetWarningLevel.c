#include "shared/sdk/libpad/decls.h"

int scePadSetWarningLevel(int level)
{
    padRpcPacket.port = level;
    padRpcPacket.cmd = 0x14;
    if (sceSifCallRpc(padRpcClient, 1, 0, &padRpcPacket, 0x80, &padRpcPacket, 0x80, 0, 0) < 0) {
        return 0;
    }
    return padRpcPacket.slot;
}
