#include "shared/sdf/sdfIpu/decls.h"

/* Remove a queued node without clearing its old links; the caller may own the interrupt guard. */
void sdfIpuDetachQueue(sdfIpuDecodeCb_t *cb, s32 critical) {
    s32 interruptsEnabled = 0;
    sdfIpuDecodeCb_t *previousNode;
    sdfIpuDecodeCb_t *nextNode;
    if (critical == 0) {
        interruptsEnabled = DIntr();
    }
    if (cb->queueLinked != 0) {
        previousNode = cb->prev;
        nextNode = cb->streamNext;
        if (previousNode == 0) {
            sdfIpuQueueHead = nextNode;
        } else {
            previousNode->streamNext = nextNode;
        }
        if (nextNode == 0) {
            sdfIpuQueueTail = previousNode;
        } else {
            nextNode->prev = previousNode;
        }
        cb->queueLinked = 0;
    }
    if (critical == 0 && interruptsEnabled != 0) {
        EIntr();
    }
}
