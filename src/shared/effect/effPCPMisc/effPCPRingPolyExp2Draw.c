#include "shared/effect/effPCPMisc/decls.h"

void effPCPRingPolyExp2Draw(effPCPRingPolyExp2_tag *hData) {
    s32 frame = hData->frame;
    s32 duration = SDF_FRAMES(hData->head.duration);
    s32 fadeIn = SDF_FRAMES(hData->head.fadeIn);
    s32 fadeOut = SDF_FRAMES(hData->head.fadeOut);
    f32 t;
    u32 color;
    dds3RingPolyExpList_tag *beam;

    if (duration < frame) {
        return;
    }
    hData->radius += SDF_FRAME_STEP(hData->radius_s);
    effPCPRingPolyExp2SetVertex(hData, hData->radius);
    hData->radius_s *= SDF_FRAME_DECAY(hData->head.radiusDecay);
    t = 1.0f;
    if (frame < fadeIn && fadeIn != 0) {
        t = (f32)frame / (f32)fadeIn;
    } else if (duration - frame <= fadeOut && fadeOut != 0) {
        t = (f32)(duration - frame) / (f32)fadeOut;
    }
    color = effColorBlend(hData->abgr & 0xFFFFFF, hData->abgr, t);
    beam = hData->hPoly;
    beam->color = color;
    SDF_VU0_COPYVECTOR(&beam->pos, hData);
    dds3RingPolyExpListDraw(beam);
    hData->frame++;
}
