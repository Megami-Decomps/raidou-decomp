#include "shared/effect/effPCPMisc/decls.h"

void effPCPRadiateBEDDraw(effPCPRadiateBED_tag *hData) {
    effEveBEDParams_t place;
    f32 base[4] __attribute__((aligned(16)));
    u32 count = hData->param.count;
    s32 duration = SDF_FRAMES(hData->param.duration);
    s32 fadeIn = SDF_FRAMES(hData->param.fadeIn);
    s32 fadeOut = SDF_FRAMES(hData->param.fadeOut);
    f32 angleDecay = SDF_FRAME_DECAY(hData->param.angleDecay);
    effPCPRadiateBEDPart_tag *entry = hData->pPart;
    u32 color = hData->color;
    u32 i;
    s32 frame;

    SDF_VU0_COPYVECTOR(base, hData->param.position);
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
                effPCPRadiateBEDPartInit(hData, i);
                frame = entry->frame;
            }
            if (frame >= 0) {
                f32 angle = entry->radian;
                f32 radius = entry->radius;
                f32 cosine;
                f32 sine;
                f32 t;

                cosine = sdfCos(angle);
                sine = sdfSin(angle);
                place.center[0] = base[0] + cosine * radius;
                place.center[1] = base[1];
                place.center[2] = base[2] + sine * radius;
                entry->radius += SDF_FRAME_STEP(entry->radius_s);
                entry->radian += SDF_FRAME_STEP(entry->radian_s);
                entry->radian_s *= angleDecay;
                if (frame < fadeIn && fadeIn != 0) {
                    t = (f32)frame / (f32)fadeIn;
                } else if (duration - frame <= fadeOut && fadeOut != 0) {
                    t = (f32)(duration - frame) / (f32)fadeOut;
                } else {
                    t = 1.0f;
                }
                place.abgr = effColorBlend(color & 0xFFFFFF, color, t);
                effEveBEDCutSetParams(entry->hBEDCut, &place);
                effEveBEDCutDraw(entry->hBEDCut);
                frame = entry->frame;
            }
            entry->frame = frame + 1;
        }
    }
}
