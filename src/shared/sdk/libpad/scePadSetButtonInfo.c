#include "shared/sdk/libpad/decls.h"

int scePadSetButtonInfo(int port, int slot, u32 buttonInfo)
{
    padRpcPacket.a.w[0] = buttonInfo;
    padRpcPacket.cmd = 0xA;
    padRpcPacket.port = port;
    padRpcPacket.slot = slot;
    if (sceSifCallRpc(padRpcClient, 1, 0, &padRpcPacket, 0x80, &padRpcPacket, 0x80, 0, 0) < 0) {
        return 0;
    }
    if (padRpcPacket.a.w[1] == 1) {
        scePadSetReqState(port, slot, 2);
    }
    return padRpcPacket.a.w[1];
}
