#include "shared/effect/effBlur_Filter/decls.h"

/* Reset a slot for a new burst: zero phase and angle, then copy the colour, the two spare fields and the centre from the work parameters. */
void dds3EffectRipBlurPartInit(effBlurScaleWork_t *pParam, effBlurScaleSlot_t *pPart) {
    dds3Effect_Blur_t *quad = &pPart->quad;

    pPart->phase = 0.0f;
    pPart->angle = 0.0f;
    quad->abgr = pParam->params.abgr;
    quad->blend = pParam->params.blend;
    quad->rotate = pParam->params.rotate;
    quad->center[0] = pParam->params.center[0];
    quad->center[1] = pParam->params.center[1];
}
