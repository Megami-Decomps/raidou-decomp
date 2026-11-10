#include "shared/sdf/sdfMotion/decls.h"

void sdfMotionSeek(sdfMotionCtrlr_t *aCtrlr, f32 aFrame) {
    s32 i;
    s32 count;
    f32 elapsed;
    f32 duration;
    f32 weight;
    sdfMotionNode_t *binding;

    aCtrlr->stat = SDF_MOTION_STAT_SEEK;
    duration = aCtrlr->blendLength;
    elapsed = aFrame - aCtrlr->blendPreLength;
    aCtrlr->frame = aFrame;
    if (elapsed < duration) {
        if (elapsed < 0.0f) {
            weight = 0.0f;
        } else {
            weight = elapsed / duration;
        }
        if (aFrame < 0.0f) {
            aFrame = 0.0f;
        }
        for (i = 0; i < aCtrlr->nodeList->count; i++) {
            binding = aCtrlr->nodeList->items[i];
            binding->vtbl->blendSeek(binding, aFrame, weight);
        }
    } else {
        count = aCtrlr->nodeList->count;
        for (i = 0; i < count; i++) {
            binding = aCtrlr->nodeList->items[i];
            binding->vtbl->seek(binding, aFrame);
        }
    }
}
