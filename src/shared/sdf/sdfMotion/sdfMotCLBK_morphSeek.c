#include "shared/sdf/sdfMotion/decls.h"

void sdfMotCLBK_morphSeek(sdfMotNodeMorph_t *aThis, f32 aFrame) {
    sdfMotGetKeyIf_t sample;
    sdfModelMorphParm_t *out;
    s32 firstId;
    s32 secondId;
    f32 firstValue;
    f32 secondValue;
    f32 weight;
    f32 inverse;

    sdfMotGetKey(&aThis->nodeBase, aFrame, &sample);
    firstId = ((sdfModelMorphParm_t *)sample.key0)->targetId;
    secondId = ((sdfModelMorphParm_t *)sample.key1)->targetId;
    firstValue = ((sdfModelMorphParm_t *)sample.key0)->blend;
    secondValue = ((sdfModelMorphParm_t *)sample.key1)->blend;
    weight = sample.rate;
    inverse = 1.0f - weight;
    out = aThis->morphNode->target;
    if (firstId == secondId) {
        out[0].targetId = firstId;
        out[0].blend = firstValue * inverse + secondValue * weight;
        out[1].targetId = secondId;
        out[1].blend = 0;
    } else {
        out[0].targetId = firstId;
        out[0].blend = firstValue * inverse;
        out[1].targetId = secondId;
        out[1].blend = secondValue * weight;
    }
}
