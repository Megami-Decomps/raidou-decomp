#include "shared/effect/effEvent/decls.h"

/* Step active records, update their packed color/position callbacks, and optionally recycle.
 * The slot-zero lookup precedes the empty-count check; retain the time snapshot and later stored increments. */
void effAPPoint4Draw(effAPPoint4_tag *hData) {
    effAPPoint4Part_tag *recordCursor = hData->pPart;
    effArrHdr_t *bezierSlots = hData->pBezier4P;
    u32 recordCount = hData->param.count;
    u32 recordIndex = 0;
    u8 cycleEnabled = hData->param.enabled;
    s32 activeSteps = SDF_FRAMES(hData->param.steps);
    s32 delayModulus = hData->param.spread;
    s32 fadeInSteps = SDF_FRAMES(hData->param.fade_in);
    s32 fadeOutSteps = SDF_FRAMES(hData->param.fade_out);
    f32 position[3] QWORD_ALIGNED;

    (void)dds3Bezier4PGetPart(bezierSlots, 0);
    if (recordCount == 0) {
        return;
    }
    do {
        s32 recordTime = recordCursor->frame;

        if (recordTime == 0) {
            effAPPoint4PartInit(hData, recordIndex);
        }
        if (recordTime > 0 && recordTime <= activeSteps) {
            f32 opacity;
            s32 fadePhase;
            u32 alpha;
            u32 color;

            dds3Bezier4PPartRun(bezierSlots, recordIndex, position);
            fadePhase = fadeInSteps > recordTime;
            if (fadePhase) {
                opacity = (f32)recordTime / (f32)fadeInSteps;
            } else {
                fadePhase = activeSteps - recordTime;
                if (fadePhase <= fadeOutSteps) {
                    opacity = (f32)fadePhase / (f32)fadeOutSteps;
                } else {
                    opacity = 1.0f;
                }
            }
            alpha = (u32)(opacity * EFF_CHANNEL_ALPHA_SCALE);
            color = (alpha << EFF_CHANNEL_ALPHA_SHIFT) | EFF_CHANNEL_RGB_COLOR;
            dds3EffectPartSetColor(recordCursor->hPart, color);
            dds3EffectPartSetPosition(recordCursor->hPart, position);
            dds3EffectPartDraw(recordCursor->hPart);
        }
        if (recordTime < activeSteps) {
            recordCursor->frame++;
        } else if (cycleEnabled != 0) {
            recordCursor->frame = -SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % delayModulus);
        } else {
            recordCursor->frame++;
        }
        recordCursor++;
        recordIndex++;
    } while (recordIndex < recordCount);
}
