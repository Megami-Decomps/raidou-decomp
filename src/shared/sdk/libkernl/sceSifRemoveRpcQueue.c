#include "shared/sdk/libkernl/decls.h"

sifRpcDataQueue_t *sceSifRemoveRpcQueue(sifRpcDataQueue_t *queue)
{
    sifRpcDataQueue_t *current;

    DIntr();
    current = sifRpcQueue.queues;
    if (current == queue) {
        sifRpcQueue.queues = current->next;
    } else {
        while (current != NULL) {
            if (current->next == queue) {
                current->next = queue->next;
                break;
            }
            current = current->next;
        }
    }
    EIntr();
    return current;
}
