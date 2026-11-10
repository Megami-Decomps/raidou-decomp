#include "shared/effect/effPCPMisc/decls.h"

void effPCPDistortionBlurExpDraw(effPCPDistortionBlurExp_tag *hData) {
    effBlurScatterWork_t *resource;
    f32 projected[4] __attribute__((aligned(16)));
    s32 frame = hData->frame;
    s32 duration = SDF_FRAMES(hData->life);
    s32 fadeIn;
    s32 fadeOut;
    f32 opacity;
    f32 displacement;
    u32 color;

    if (duration < frame) {
        return;
    }
    fadeIn = SDF_FRAMES(hData->fade_in);
    fadeOut = SDF_FRAMES(hData->fade_out);
    displacement = (f32)(hData->area_e - hData->area_s) *
                   (f32)frame / (f32)duration;
    displacement = (s32)displacement;
    resource = hData->resource.scatter;
    resource->params.area =
        (f32)hData->area_s + displacement;
    if (hData->type == 0) {
        SDF_VU0_LOADVECTOR(hData->position);
        effScreenOffset();
        SDF_VU0_STOREVECTOR(projected);
        resource = hData->resource.scatter;
        resource->params.center[0] = (s32)projected[0] - 2048;
        resource->params.center[1] = (u32)((s32)projected[1] - 2048) << 1;
    } else {
        resource->params.center[0] = 0;
        resource->params.center[1] = 0;
    }
    if (frame < fadeIn && fadeIn != 0) {
        opacity = (f32)frame / (f32)fadeIn;
    } else if (duration - frame <= fadeOut && fadeOut != 0) {
        opacity = (f32)(duration - frame) / (f32)fadeOut;
    } else {
        opacity = 1.0f;
    }
    color = hData->abgr;
    color = effMagaCylinderCalcColor(
        effColorBlend(color & 0xFFFFFF, color, opacity), hData->abgr_base);
    resource = hData->resource.scatter;
    resource->params.abgr = color;
    dds3EffectDistortionBlurDraw(resource);
    hData->frame++;
}
