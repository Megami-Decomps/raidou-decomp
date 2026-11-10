#include "shared/sdf/sdfIpu/decls.h"

/* Move a node to the queue tail, using one interrupt guard for unlink plus append. */
void sdfIpuAttachQueue(sdfIpuDecodeCb_t *cb, s32 critical) {
    s32 interruptsEnabled = 0;
    if (critical == 0) {
        interruptsEnabled = DIntr();
    }
    if (cb->queueLinked != 0) {
        sdfIpuDetachQueue(cb, 1);
    }
    cb->queueLinked = 1;
    if (sdfIpuQueueTail == 0) {
        sdfIpuQueueHead = cb;
    } else {
        sdfIpuQueueTail->streamNext = cb;
    }
    cb->prev = sdfIpuQueueTail;
    cb->streamNext = 0;
    sdfIpuQueueTail = cb;
    if (critical == 0 && interruptsEnabled != 0) {
        EIntr();
    }
}
