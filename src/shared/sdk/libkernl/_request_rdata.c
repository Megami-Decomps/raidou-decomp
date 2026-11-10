#include "shared/sdk/libkernl/decls.h"

void _request_rdata(rpcPacket *request, rpcQueue *queue)
{
    rpcPacket *packet = _sceRpcGetFPacket(queue);

    u32 packetAddress = request->w[5];
    u32 client = request->w[7];

    packet->w[5] = packetAddress;
    packet->w[7] = client;
    packet->w[8] = 0x8000000C;
    isceSifSendCmd(0x80000008, packet, 0x40, EE_ADDR_AS(void *, request->w[8]),
                   EE_ADDR_AS(void *, request->w[9]), request->w[10]);
}
