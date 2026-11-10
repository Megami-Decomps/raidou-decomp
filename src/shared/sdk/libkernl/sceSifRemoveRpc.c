#include "shared/sdk/libkernl/decls.h"

sifRpcServerData_t *sceSifRemoveRpc(sifRpcServerData_t *server, sifRpcDataQueue_t *queue)
{
    sifRpcServerData_t *current;

    DIntr();
    current = queue->link;
    if (current == server) {
        queue->link = current->link;
    } else {
        while (current != NULL) {
            if (current->link == server) {
                current->link = server->link;
                break;
            }
            current = current->link;
        }
    }
    EIntr();
    return current;
}
