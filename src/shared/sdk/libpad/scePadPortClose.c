#include "shared/sdk/libpad/decls.h"

int scePadPortClose(int port, int slot)
{
    if (padPortState[port][slot].open == 0) {
        return 0;
    }
    padRpcPacket.cmd = 0xE;
    padRpcPacket.port = port;
    padRpcPacket.slot = slot;
    padRpcPacket.a.w[1] = 1;
    if (sceSifCallRpc(padRpcClient, 1, 0, &padRpcPacket, 0x80, &padRpcPacket, 0x80, 0, 0) < 0) {
        return 0;
    }
    padPortState[port][slot].open = 0;
    return padRpcPacket.a.w[0];
}
