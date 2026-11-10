#include "shared/sdk/libpad/decls.h"

int scePadGetSlotMax(int port)
{
    padRpcPacket.port = port;
    padRpcPacket.cmd = 0xD;
    if (sceSifCallRpc(padRpcClient, 1, 0, &padRpcPacket, 0x80, &padRpcPacket, 0x80, 0, 0) < 0) {
        return 0;
    }
    return padRpcPacket.a.w[0];
}
