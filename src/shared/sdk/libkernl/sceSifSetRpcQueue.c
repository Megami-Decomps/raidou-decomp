#include "shared/sdk/libkernl/decls.h"

void sceSifSetRpcQueue(sifRpcDataQueue_t *queue, s32 threadId)
{
    sifRpcDataQueue_t *current;
    sifRpcDataQueue_t *head;

    DIntr();
    queue->threadId = threadId;
    queue->active = 0;
    queue->link = NULL;
    queue->start = NULL;
    queue->end = NULL;
    queue->next = NULL;
    head = sifRpcQueue.queues;
    if (head == NULL) {
        sifRpcQueue.queues = queue;
    } else {
        current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = queue;
    }
    EIntr();
}
