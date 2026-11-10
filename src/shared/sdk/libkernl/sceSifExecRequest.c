#include "shared/sdk/libkernl/decls.h"

void sceSifExecRequest(sifRpcServerData_t *server)
{
    sifDmaTransfer_t transfers[2];
    rpcPacket *packet;
    void *reply;
    s32 replySize = 0;
    s32 count;
    s32 status;
    s32 delay;

    reply = server->handler(server->rpcNumber, server->buffer, server->size);
    if (reply != NULL) {
        replySize = server->receiveSize;
    }
    if (server->size > 0) {
        sceSifWriteBackDCache(server->buffer, server->size);
    }
    if (replySize > 0) {
        sceSifWriteBackDCache(reply, replySize);
    }
    DIntr();
    if (server->requestId & 4) {
        packet = _sceRpcGetFPacket2(&sifRpcQueue, (u32)server->requestId >> 16);
    } else {
        packet = _sceRpcGetFPacket(&sifRpcQueue);
    }
    EIntr();
    packet->w[8] = 0x8000000A;
    packet->w[7] = EE_ADDR(server->client);
    if (server->mode != 0) {
        while (sceSifSendCmd(0x80000008, packet, 0x40, reply, server->receiveBuffer, replySize) == 0) {
        }
        return;
    }
    packet->w[6] = 0;
    count = 0;
    packet->w[4] = 0;
    if (replySize > 0) {
        transfers[0].source = EE_ADDR(reply);
        transfers[0].destination = EE_ADDR(server->receiveBuffer);
        transfers[0].size = replySize;
        transfers[0].attributes = 0;
        count++;
    }
    transfers[count].source = EE_ADDR(packet);
    transfers[count].destination = EE_ADDR(server->packetAddress);
    transfers[count].size = 0x40;
    transfers[count].attributes = 0;
    count++;
    do {
        status = sceSifSetDma(transfers, count);
        if (status == 0) {
            for (delay = 0x100000; delay != -1; delay--) {
            }
        }
    } while (status == 0);
}
