#include "shared/sdk/libkernl/decls.h"

sifRpcServerData_t *_search_svdata(s32 serviceId, rpcQueue *queue)
{
    sifRpcDataQueue_t *current;
    sifRpcServerData_t *server;

    for (current = queue->queues; current != NULL; current = current->next) {
        for (server = current->link; server != NULL; server = server->link) {
            if (server->serviceId == serviceId) {
                return server;
            }
        }
    }
    return NULL;
}
