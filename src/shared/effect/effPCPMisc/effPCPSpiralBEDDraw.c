#include "shared/effect/effPCPMisc/decls.h"

void effPCPSpiralBEDDraw(effPCPSpiralBED_tag *hData) {
    effEveBEDParams_t place;
    f32 origin[4] __attribute__((aligned(16)));
    u32 count = hData->param.count;
    s32 duration = SDF_FRAMES(hData->param.duration);
    s32 fadeIn = SDF_FRAMES(hData->param.fadeIn);
    s32 fadeOut = SDF_FRAMES(hData->param.fadeOut);
    f32 angularDamping = SDF_FRAME_DECAY(hData->param.angularDamping);
    f32 heightDamping = SDF_FRAME_DECAY(hData->param.heightDamping);
    effPCPSpiralBEDPart_tag *entry = hData->pPart;
    u32 color = hData->color;
    u32 i;
    s32 frame;

    SDF_VU0_COPYVECTOR(origin, hData->param.origin);
    if (duration == 0) {
        return;
    }
    place.rot[0] = 0;
    place.rot[1] = 0;
    place.rot[2] = 0;
    place.rot[3] = 1.0f;
    place.r = 100.0f;
    place.h = 100.0f;
    place.scale = 1.0f;
    for (i = 0; i < count; i++, entry++) {
        frame = entry->frame;
        if (frame <= duration) {
            if (frame == 0) {
                effPCPSpiralBEDPartInit(hData, i);
                effEveBEDCutSetScale(entry->hBEDCut, hData->scale);
                frame = entry->frame;
            }
            if (frame >= 0) {
                f32 angle = entry->rotate;
                f32 radius = entry->radius;
                f32 cosine;
                f32 sine;
                f32 fade;

                cosine = sdfCos(angle);
                sine = sdfSin(angle);
                place.center[0] = origin[0] + cosine * radius;
                place.center[1] = origin[1] + entry->up;
                place.center[2] = origin[2] + sine * radius;
                entry->up += SDF_FRAME_STEP(entry->up_s);
                entry->radius += SDF_FRAME_STEP(entry->radius_s);
                entry->rotate += SDF_FRAME_STEP(entry->rotate_s);
                entry->up_s *= heightDamping;
                entry->rotate_s *= angularDamping;
                if (frame < fadeIn && fadeIn != 0) {
                    fade = (f32)frame / (f32)fadeIn;
                } else if (duration - frame <= fadeOut && fadeOut != 0) {
                    fade = (f32)(duration - frame) / (f32)fadeOut;
                } else {
                    fade = 1.0f;
                }
                place.abgr = effColorBlend(color & 0xFFFFFF, color, fade);
                effEveBEDCutSetParams(entry->hBEDCut, &place);
                effEveBEDCutDraw(entry->hBEDCut);
                frame = entry->frame;
            }
            entry->frame = frame + 1;
        }
    }
}
