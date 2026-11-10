#include "shared/sdk/libpad/decls.h"

int scePadSetMainMode(int port, int slot, int offs, int lock)
{
    padRpcPacket.a.w[0] = offs;
    padRpcPacket.a.w[1] = lock;
    padRpcPacket.cmd = 6;
    padRpcPacket.port = port;
    padRpcPacket.slot = slot;
    if (sceSifCallRpc(padRpcClient, 1, 0, &padRpcPacket, 0x80, &padRpcPacket, 0x80, 0, 0) < 0) {
        return 0;
    }
    if (padRpcPacket.a.w[2] == 1) {
        scePadSetReqState(port, slot, 2);
    }
    return padRpcPacket.a.w[2];
}
