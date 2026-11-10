#include "shared/sdk/libpad/decls.h"

int scePadInit2(int mode)
{
    int i;

    for (i = 0; i < 4; i++) {
        padPortState[0][i].open = 0;
        padPortState[0][i].f18 = 0;
        padPortState[0][i].f14 = 0;
        padPortState[1][i].open = 0;
        padPortState[1][i].f18 = 0;
        padPortState[1][i].f14 = 0;
    }
    padRpcPacket.cmd = 0x10;
    padRpcPacket.a.w[1] = 0;
    if (sceSifCallRpc(padRpcClient, 1, 0, &padRpcPacket, 0x80, &padRpcPacket, 0x80, 0, 0) < 0) {
        return 0;
    }
    return padRpcPacket.a.w[0];
}
