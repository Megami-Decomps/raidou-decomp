#include "shared/sdf/sdfFileIO/decls.h"

/* Move a completed request off both active lists and into the bounded cache. */
void sdfFileCmdDequeue(void *aCmd)
{
    sdfFileCmdHandle_t *state = aCmd;
    sdfFileDeviceCB_t *worker;
    sdfFileCmdHandle_t *previous;
    sdfFileCmdHandle_t *next;
    s32 interrupts;

    interrupts = DIntr();
    previous = state->prev;
    next = state->next;
    if (previous == NULL) {
        sdfFileCmdActiveHead = next;
    } else {
        previous->next = next;
    }
    if (next == NULL) {
        sdfFileCmdFreeList = previous;
    } else {
        next->prev = previous;
    }
    worker = &sdfFileDeviceCB[state->device];
    previous = state->devicePrev;
    next = state->deviceNext;
    if (previous == NULL) {
        worker->first = next;
    } else {
        previous->deviceNext = next;
    }
    if (next == NULL) {
        worker->last = previous;
    } else {
        next->devicePrev = previous;
    }
    state->prev = sdfFileCmdCacheTail;
    if (sdfFileCmdCacheTail == NULL) {
        sdfFileCmdCacheHead = state;
    } else {
        sdfFileCmdCacheTail->next = state;
    }
    sdfFileCmdCacheTail = state;
    state->next = NULL;
    sdfFileCmdCount++;
    sdfFileCmdFreeCount--;
    if (interrupts != 0) {
        EIntr();
    }
    if (sdfFileCmdCount >= 9) {
        sdfFileCmdUnlink(sdfFileCmdCacheHead);
    }
    if (worker->first != NULL) {
        SignalSema(worker->semaId);
    }
}
