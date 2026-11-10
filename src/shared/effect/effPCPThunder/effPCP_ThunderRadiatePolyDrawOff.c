#include "shared/effect/effPCPThunder/decls.h"

void effPCP_ThunderRadiatePolyDrawOff(effPCP_ThunderRadiatePoly_t *work, s32 index) {
    effPCP_ThunderRadiatePolyPart_t *cell = work->pPart + index;
    f32 centeredRandom;

    centeredRandom = dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT;
    cell->directionX = centeredRandom + centeredRandom;
    centeredRandom = dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT;
    cell->directionY = centeredRandom + centeredRandom;
    centeredRandom = dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT;
    cell->directionZ = centeredRandom + centeredRandom;
    cell->delayFrames = dds3GetRandA((effRandState_t *)dds3EffectBase) % work->head.startDelayRange;
    cell->activeFrames = dds3GetRandA((effRandState_t *)dds3EffectBase) % work->head.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
}
