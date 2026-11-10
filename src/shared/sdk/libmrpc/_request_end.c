#include "shared/sdk/libmrpc/decls.h"

void _request_end(void *packet, void *queue)
{
    rpcResponse *response = packet;
    rpcClient *client;

    switch (response->command) {
    case 0x8000001A:
        client = response->client;
        if (client->endFunction != NULL) {
            client->endFunction(client->endParam);
        }
        iSignalSema(client->completionSema);
        break;
    case 0x80000019:
        client = response->client;
        client->server = response->value.server;
        client->buffer = response->buffer;
        client->cbuffer = response->cbuffer;
        iSignalSema(client->completionSema);
        break;
    case 0x8000001D:
        client = response->client;
        client->result = response->value.result;
        iSignalSema(client->completionSema);
        break;
    case 0x8000001C:
        break;
    }
    client = response->client;
    if (client->sema >= 0) {
        iSignalSema(client->sema);
    }
    _sceRpcFreePacket(client->packet);
    client->packet = NULL;
}
