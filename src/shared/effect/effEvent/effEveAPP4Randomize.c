#include "shared/effect/effEvent/decls.h"

/* Jitter each slot and choose its initial unsigned-modulo step.
 * Stored record time is one ahead of the Bezier cursor parameter's sampled step. */
void effEveAPP4Randomize(effEmit_t *hEffect) {
    effAPPoint4_tag *channelWork = hEffect->primitive;
    u32 stepModulus = channelWork->param.steps;
    u32 recordCount = channelWork->param.count;
    effAPPoint4Part_tag *recordCursor = channelWork->pPart;
    u32 recordIndex = 0;
    effCubicBezierSlot_t *bezierSlot;
    s32 randomStep;
    f32 cursorStep;

    if (recordCount == 0) {
        return;
    }
    do {
        effAPPoint4PartInit(channelWork, recordIndex);
        recordCursor->frame = SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % stepModulus);
        bezierSlot = dds3Bezier4PGetPart(channelWork->pBezier4P, recordIndex);
        recordIndex++;
        randomStep = recordCursor->frame;
        cursorStep = bezierSlot->step;
        recordCursor->frame = randomStep + 1;
        recordCursor++;
        bezierSlot->t = cursorStep * (f32)randomStep;
    } while (recordIndex < recordCount);
}
