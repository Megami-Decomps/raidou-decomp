#include "shared/effect/effPCPMisc/decls.h"

void effPCPMultiFilterExpDraw(effPCPMultiBlurExp2_tag *hData) {
    sdfVector4_t projected;
    s32 frame = hData->frame;
    s32 duration = SDF_FRAMES(hData->duration);
    dds3Effect_PartialMultiFilter_t *rect = hData->resource.rectangle;
    s32 fadeIn;
    s32 fadeOut;
    f32 opacity;
    u32 color;

    if (duration >= frame) {
        fadeIn = SDF_FRAMES(hData->fadeIn);
        fadeOut = SDF_FRAMES(hData->fadeOut);
        if (hData->flags == 0) {
            SDF_VU0_LOADVECTOR(hData->position);
            effScreenOffset();
            SDF_VU0_STOREVECTOR(&projected);
            rect->param.center[0] = (s32)projected.x - 2048;
            rect->param.center[1] = ((s32)projected.y - 2048) << 1;
        } else {
            rect->param.center[0] = 0;
            rect->param.center[1] = 0;
        }
        rect->param.area = InterpolateCompactExtent(
            hData->startExtent, hData->endExtent, frame, duration);
        if (frame < fadeIn && fadeIn != 0) {
            opacity = (f32)frame / (f32)fadeIn;
        } else if (duration - frame <= fadeOut && fadeOut != 0) {
            opacity = (f32)(duration - frame) / (f32)fadeOut;
        } else {
            opacity = 1.0f;
        }
        color = hData->color;
        *(u32 *)rect->param.Filter.color = effMagaCylinderCalcColor(
            effColorBlend(color & 0xFFFFFF, color, opacity), hData->baseColor);
        dds3EffectPartialMultiFilterDraw(rect);
        hData->frame++;
    }
}
