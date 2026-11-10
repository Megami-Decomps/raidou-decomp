#include "shared/sdf/sdfFile/decls.h"

/* Queue a request on the global list and on its worker's list; wake the worker when its list was empty. */
void sdfFileCmdAllocHandle(sdfFileCmdHandle_t *state) {
    sdfFileDeviceCB_t *worker;
    s32 interrupts;
    s32 wake = 0;
    sdfFileCmdHandle_t *last;
    interrupts = DIntr(state);
    worker = &sdfFileDeviceCB[state->device];
    if (worker->thId < 0) {
        sdfFileCmdStartThread(state->device);
    }
    state->prev = sdfFileCmdFreeList;
    if (sdfFileCmdFreeList != NULL) {
        sdfFileCmdFreeList->next = state;
    } else {
        sdfFileCmdActiveHead = state;
    }
    last = worker->last;
    sdfFileCmdFreeList = state;
    state->devicePrev = last;
    if (last != NULL) {
        last->deviceNext = state;
    } else {
        worker->first = state;
        wake = 1;
    }
    worker->last = state;
    sdfFileCmdFreeCount += 1;
    if (interrupts != 0) {
        EIntr();
    }
    if (wake != 0) {
        SignalSema(worker->semaId);
    }
}
