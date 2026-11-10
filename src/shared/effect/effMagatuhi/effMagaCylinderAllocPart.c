#include "shared/effect/effMagatuhi/decls.h"

effMagatuhiOwner_t *effMagaCylinderAllocPart(const u32 *source) {
    effMagatuhiOwner_t *effect;

    effect = sdfChipAllocDirect(sizeof(*effect));
    memcpy(&effect->params, source, sizeof(effect->params));
    effect->valueWork = effMagaCylinderAllocTex(effect->params.count, effect->params.historyCount, effect->params.historyDistanceScale, effect->params.drawBucket, effect->params.historyAngleStep);
    effMagaCylinderBlendColor(effect->valueWork, effect->params.colorA, effect->params.colorB);
    return effect;
}
