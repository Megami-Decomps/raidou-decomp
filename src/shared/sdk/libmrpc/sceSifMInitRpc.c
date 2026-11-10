#include "shared/sdk/libmrpc/decls.h"

void sceSifMInitRpc(s32 mode)
{
    rpcPacket *packets;
    rpcPacket *packet;
    rpcQueue *queue;

    if (sceSifMRpcInitialized != 0) {
        return;
    }
    sceSifMRpcInitialized = 1;
    DIntr();
    packets = sceSifMRpcPackets;
    queue = &sceSifMRpcQueue;
    queue->base = EE_ADDR_AS(rpcPacket *, (EE_ADDR(packets) | 0x20000000));
    queue->count = 32;
    queue->seq = 1;
    queue->unk_C = 0;
    queue->unk_10 = 0;
    sceSifAddCmdHandler(0x80000018, _request_end, queue);
    EIntr();
    packet = packets + 1;
    packet->w[4] = 1;
    packet->w[5] = 1;
    sceSifSendCmd(0x80000001, packet, 24, NULL, NULL, 0);
    while (sceSifGetSreg(1) == 0) {
    }
}
