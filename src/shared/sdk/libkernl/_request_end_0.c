#include "shared/sdk/libkernl/decls.h"

void _request_end_0(void *packet, void *queue)
{
    rpcResponse *response = packet;
    rpcClient *client;
    rpcTransactionPrefix *transaction;

    switch (response->command) {
    case 0x8000000A:
        client = (rpcClient *)response->client;
        if (client->endFunction != NULL) {
            client->endFunction(client->endParam);
        }
        break;
    case 0x80000009:
        client = (rpcClient *)response->client;
        client->server = response->server;
        client->buffer = response->buffer;
        client->cbuffer = response->cbuffer;
        break;
    case 0x8000000C:
        break;
    }
    transaction = response->client;
    if (transaction->sema >= 0) {
        iSignalSema(transaction->sema);
    }
    _sceRpcFreePacket_0(transaction->packet);
    transaction->packet = NULL;
}
