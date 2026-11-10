#include "shared/sdk/libkernl/decls.h"

void sceSifRegisterRpc(sifRpcServerData_t *server, u32 serviceId,
                        sifRpcFunc_t handler, void *buffer,
                        sifRpcFunc_t callback, void *callbackBuffer,
                        sifRpcDataQueue_t *queue)
{
    sifRpcServerData_t *current;
    sifRpcServerData_t *head;

    DIntr();
    server->next = NULL;
    server->link = NULL;
    server->serviceId = serviceId;
    server->handler = handler;
    server->buffer = buffer;
    server->callback = callback;
    server->callbackBuffer = callbackBuffer;
    server->queue = queue;
    head = queue->link;
    if (head == NULL) {
        queue->link = server;
    } else {
        current = head;
        while (current->link != NULL) {
            current = current->link;
        }
        current->link = server;
    }
    EIntr();
}
