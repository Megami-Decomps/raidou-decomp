#include "shared/sdk/libpad/decls.h"

int scePadSetActAlign(int port, int slot, const u8 *align)
{
    padRpc *rpc = &padRpcPacket;
    int i;

    padRpcPacket.cmd = 8;
    rpc->port = port;
    rpc->slot = slot;
    for (i = 0; i < 6; i++) {
        rpc->a.b[i] = align[i];
    }
    if (sceSifCallRpc(padRpcClient, 1, 0, rpc, 0x80, rpc, 0x80, 0, 0) < 0) {
        return 0;
    }
    if (rpc->a.w[2] == 1) {
        scePadSetReqState(port, slot, 2);
    }
    return rpc->a.w[2];
}
