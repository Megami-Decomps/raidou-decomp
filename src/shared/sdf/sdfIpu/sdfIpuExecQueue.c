#include "shared/sdf/sdfIpu/decls.h"

/* Remove and advance the first list item matching the IPU cleanup test. */
void sdfIpuExecQueue(s32 critical) {
    sdfIpuDecodeCb_t *node;
    s32 interruptsEnabled = 0;
    if (sdfCurrIpuDecodeCb != NULL) {
        return;
    }
    if (critical == 0) {
        interruptsEnabled = DIntr();
    }
    node = sdfIpuQueueHead;
    while (node != 0) {
        if (node->activeBuffCount + node->imageTrans < 2 &&
            (node->active != 1 || node->filledSlots != 0)) {
            sdfIpuDetachQueue(node, 1);
            sdfIpuDecode(node);
            break;
        }
        node = node->streamNext;
    }
    if (critical == 0 && interruptsEnabled != 0) {
        EIntr();
    }
}
