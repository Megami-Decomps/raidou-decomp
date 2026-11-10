#include "shared/sdk/libpad/decls.h"

int scePadSetVrefParam(int port, int slot, const vrefParam *param)
{
    padRpcPacket.port = port;
    padRpcPacket.slot = slot;
    padRpcPacket.cmd = 0xB;
    *(vrefParam *)padRpcPacket.a.b = *param;
    if (sceSifCallRpc(padRpcClient, 1, 0, &padRpcPacket, 0x80, &padRpcPacket, 0x80, 0, 0) < 0) {
        return 0;
    }
    if (padRpcPacket.a.w[4] == 1) {
        scePadSetReqState(port, slot, 2);
    }
    return padRpcPacket.a.w[4];
}
