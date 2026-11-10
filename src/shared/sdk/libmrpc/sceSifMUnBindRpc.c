#include "shared/sdk/libmrpc/decls.h"

s32 sceSifMUnBindRpc(rpcClient *client, s32 mode)
{
    sdfSemaParam_t param;
    rpcPacket *packet;
    s32 sema;
    s32 result;

    if (mode != 0) {
        scePrintf(sceSifMModeErrorString);
        return -1;
    }
    WaitSema(client->completionSema);
    packet = _sceRpcGetPacket(&sceSifMRpcQueue);
    result = -1;
    if (packet != NULL) {
        client->sequence = packet->unbind.sequence;
        client->packet = packet;
        packet->unbind.serverId = client->serverId;
        packet->unbind.self = packet;
        packet->unbind.client = client;
        memcpy(&packet->unbind.server, &client->server, sizeof(packet->unbind.server));
        param.maxCount = 1;
        param.initCount = 0;
        param.option = 0;
        sema = CreateSema(&param);
        client->sema = sema;
        if (sema < 0) {
            _sceRpcFreePacket(packet);
            return -3;
        }
        if (sceSifSendCmd(0x8000001D, packet, 0x40, NULL, NULL, 0) == 0) {
            _sceRpcFreePacket(packet);
            DeleteSema(sema);
            return -2;
        }
        WaitSema(sema);
        DeleteSema(sema);
        DeleteSema(client->completionSema);
        result = client->result;
    }
    return result;
}
