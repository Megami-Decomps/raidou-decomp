#include "shared/sdk/libkernl/decls.h"

s32 sceSifGetOtherData(rpcTransactionPrefix *client, void *source, void *destination, s32 size, s32 mode)
{
    rpcSemaParam param;
    rpcPacket *packet;

    packet = _sceRpcGetPacket_0(&sifRpcQueue);
    if (packet == NULL) {
        return -1;
    }
    client->packet = packet;
    client->sequence = packet->w[6];
    packet->w[8] = EE_ADDR(source);
    packet->w[9] = EE_ADDR(destination);
    packet->w[10] = size;
    packet->w[5] = EE_ADDR(packet);
    packet->w[7] = EE_ADDR(client);
    if ((mode & 1) == 0) {
        param.maxCount = 1;
        param.initCount = 0;
        client->sema = CreateSema(&param);
        if (client->sema < 0) {
            _sceRpcFreePacket_0(packet);
            return -3;
        }
        if (sceSifSendCmd(0x8000000C, packet, 0x40, NULL, NULL, 0) == 0) {
            _sceRpcFreePacket_0(packet);
            DeleteSema(client->sema);
            return -2;
        }
        WaitSema(client->sema);
        DeleteSema(client->sema);
        return 0;
    }
    client->sema = -1;
    if (sceSifSendCmd(0x8000000C, packet, 0x40, NULL, NULL, 0) == 0) {
        _sceRpcFreePacket_0(packet);
        return -2;
    }
    return 0;
}
