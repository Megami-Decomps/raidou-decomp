#include "shared/sdk/libmrpc/decls.h"

s32 sceSifMBindRpcParam(rpcClient *client, u32 serverId, s32 mode, u32 arg0, u32 arg1, u32 arg2)
{
    sdfSemaParam_t param;
    rpcPacket *packet;
    s32 sema;

    if (mode != 0) {
        scePrintf(sceSifMModeErrorString);
        return -1;
    }
    param.option = 0;
    param.initCount = 1;
    param.maxCount = 1;
    client->completionSema = CreateSema(&param);
    if (client->completionSema < 0) {
        return -3;
    }
    WaitSema(client->completionSema);
    client->serverId = serverId;
    client->server = NULL;
    client->parameters[0] = arg0;
    client->parameters[1] = arg1;
    client->parameters[2] = arg2;
    packet = _sceRpcGetPacket(&sceSifMRpcQueue);
    if (packet == NULL) {
        DeleteSema(client->completionSema);
        return -1;
    }
    client->sequence = packet->w[6];
    client->packet = packet;
    packet->w[8] = serverId;
    packet->w[9] = arg0;
    packet->w[10] = arg1;
    packet->w[11] = arg2;
    packet->w[5] = EE_ADDR(packet);
    packet->w[7] = EE_ADDR(client);
    param.maxCount = 1;
    param.initCount = 0;
    param.option = 0;
    sema = CreateSema(&param);
    client->sema = sema;
    if (sema < 0) {
        _sceRpcFreePacket(packet);
        DeleteSema(client->completionSema);
        return -3;
    }
    if (sceSifSendCmd(0x80000019, packet, 0x40, NULL, NULL, 0) == 0) {
        _sceRpcFreePacket(packet);
        DeleteSema(sema);
        DeleteSema(client->completionSema);
        return -2;
    }
    WaitSema(sema);
    DeleteSema(sema);
    return 0;
}
