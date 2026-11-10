#include "shared/sdf/sdfDraw/decls.h"

void sdfGrReqDraw(sdfDrawTag_t *aTag, sdfPreDrawChain_t *aPreDraw) {
    u32 start = sdfGetRunTimeCountBase();
    s32 interrupts;
    s32 index;
    sdfDrawReqRec_t *slot;

    SDF_FLAG_WAIT(&sdfReqDrawDelayCount, sdfReqDrawDelayCount != 0);
    sdfReqDrawDelaySleepCount = sdfGetRunTimeCount(start);
    interrupts = DIntr();
    index = sdfDrawReqQueueIndex + 1;
    if (index == 3) {
        index = 0;
    }
    slot = &sdfGrDrawState[index];
    slot->drawTag = NULL;
    slot->preDraw = NULL;
    slot->drawCount = 0;
    if (aTag != NULL && aTag->tail != 0) {
        slot->drawTag = aTag;
        if (aPreDraw != NULL) {
            slot->preDraw = aPreDraw->tagHead->head;
            if (slot->preDraw != NULL) {
                aPreDraw->tail->tail->next = NULL;
            }
        }
        slot->queuedBufferIndex = sdfFrameId;
    }
    sdfDrawReqQueueIndex = index;
    sdfDrawImageTrans = 1;
    sdfDrawFrameCount++;
    if (interrupts != 0) {
        EIntr();
    }
}
