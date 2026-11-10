#include "shared/sdk/libkernl/decls.h"

s32 sceSifBindRpc(rpcClient *client, u32 serverId, s32 mode)
{
    rpcSemaParam param;
    rpcPacket *packet;

    client->serverId = 0;
    client->server = NULL;
    packet = _sceRpcGetPacket_0(&sifRpcQueue);
    if (packet == NULL) {
        return -1;
    }
    client->transaction.packet = packet;
    client->transaction.sequence = packet->w[6];
    packet->w[8] = serverId;
    packet->w[5] = EE_ADDR(packet);
    packet->w[7] = EE_ADDR(client);
    if ((mode & 1) == 0) {
        param.maxCount = 1;
        param.initCount = 0;
        client->transaction.sema = CreateSema(&param);
        if (client->transaction.sema < 0) {
            _sceRpcFreePacket_0(packet);
            return -3;
        }
        if (sceSifSendCmd(0x80000009, packet, 0x40, NULL, NULL, 0) == 0) {
            _sceRpcFreePacket_0(packet);
            DeleteSema(client->transaction.sema);
            return -2;
        }
        WaitSema(client->transaction.sema);
        DeleteSema(client->transaction.sema);
        return 0;
    }
    client->transaction.sema = -1;
    if (sceSifSendCmd(0x80000009, packet, 0x40, NULL, NULL, 0) == 0) {
        _sceRpcFreePacket_0(packet);
        return -2;
    }
    return 0;
}
