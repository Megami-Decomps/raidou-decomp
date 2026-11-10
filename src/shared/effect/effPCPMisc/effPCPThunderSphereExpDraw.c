#include "shared/effect/effPCPMisc/decls.h"

void effPCPThunderSphereExpDraw(effPCPThunderSphereExp_tag *hData) {
    s32 frame;
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    f32 base;
    f32 extent;
    f32 scale;
    f32 opacity;
    u32 color;

    effPCPThunderSpherePolyGetParam(hData->pThunderSpherePoly);
    frame = hData->frame;
    duration = SDF_FRAMES(hData->life);
    if (duration < frame) {
        return;
    }
    base = (f32)hData->radius_s;
    extent = (f32)hData->radius_e;
    extent -= base;
    extent = base + extent * ((f32)frame / (f32)duration);
    scale = extent / base;
    fadeIn = SDF_FRAMES(hData->fade_in);
    fadeOut = SDF_FRAMES(hData->fade_out);
    if (frame < fadeIn && fadeIn != 0) {
        opacity = (f32)frame / (f32)fadeIn;
    } else if ((s32)((u32)duration - (u32)frame) <= fadeOut && fadeOut != 0) {
        opacity = (f32)(s32)((u32)duration - (u32)frame) / (f32)fadeOut;
    } else {
        opacity = 1.0f;
    }
    color = hData->abgr;
    effPCPThunderSpherePolySetColor(hData->pThunderSpherePoly, effColorBlend(color & 0xFFFFFF, color, opacity));
    effPCPThunderSpherePolySetPosition(hData->pThunderSpherePoly, hData->pos);
    effPCPThunderSpherePolySetScale(hData->pThunderSpherePoly, hData->scale * scale);
    effPCPThunderSpherePolyDraw(hData->pThunderSpherePoly);
    hData->frame = (s32)((u32)hData->frame + 1);
}
