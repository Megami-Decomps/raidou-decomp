#include "shared/sdf/sdfIpu/decls.h"

/* Consume a pending buffer when present, but always advance processed; loop only on exact equality. */
void sdfIpuFinishActiveFrame(sdfIpuDecodeCb_t *cb) {
    s32 interruptsEnabled = DIntr();
    s32 pendingBuffers = cb->activeBuffCount;
    s32 remainingBuffers = pendingBuffers - 1;
    if (pendingBuffers > 0) {
        cb->activeBuffCount = remainingBuffers;
        cb->getBuffId ^= 1;
        cb->imageTrans++;
    }
    cb->frame++;
    if (cb->frame == cb->frameCount && cb->loopMode != 0) {
        cb->frame = 0;
    }
    if (interruptsEnabled != 0) {
        EIntr();
    }
    sdfIpuExecQueue(0);
}
