#include "shared/sdk/libkernl/decls.h"

sifRpcServerData_t *sceSifGetNextRequest(sifRpcDataQueue_t *queue)
{
    sifRpcServerData_t *request;

    DIntr();
    request = queue->start;
    if (request == NULL) {
        queue->active = 0;
    } else {
        queue->active = 1;
        queue->start = request->next;
    }
    EIntr();
    return request;
}
