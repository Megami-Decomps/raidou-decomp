#include "shared/effect/effPCPFlash/decls.h"

void effPCPFlashPolyRingDraw(effPCPFlashPolyRing_tag *hData) {
    s32 index;
    s32 lifetime;
    s32 count;
    s32 half;
    s32 ramp;
    f32 maxScale;
    s32 restart;
    s32 tintColor;
    u32 range;
    effPCPFlashPolyRingExp2Part_tag *part;
    dds3FlashPolyList_t *handle;

    lifetime = SDF_FRAMES(hData->lifetime);
    count = hData->particleCount;
    part = hData->parts;
    half = lifetime >> 1;
    ramp = SDF_FRAMES(hData->scaleRampTime);
    maxScale = hData->maxScale;
    restart = hData->restartRandomly;
    range = hData->randomRange;
    tintColor = hData->abgr;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->frame;
        s32 color;
        f32 blend;

        if (part->frame == 0) {
            part->length_max = maxScale;
            if (ramp == 0) {
                part->length = maxScale;
            } else {
                part->length = 0.0f;
            }
            effPCPFlashPolyRingPartSetVertex(hData, index);
            effPCPFlashPolyRingPartSetVC(hData, index, 0);
            part->abgr = EFF_NEUTRAL_COLOR;
        } else {
            if (part->frame >= lifetime) {
                if (restart != 0) {
                    part->frame = ~SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % range);
                }
                color = 0;
                effPCPFlashPolyRingPartSetVC(hData, index, color);
            } else if (part->frame > 0) {
                if (ramp == 0) {
                    part->length = maxScale;
                } else {
                    part->length = (maxScale * (f32)part->frame) / (f32)ramp;
                    if (maxScale < part->length) {
                        part->length = maxScale;
                    }
                }
                effPCPFlashPolyRingPartRotate(hData, index);
                effPCPFlashPolyRingPartSetVertex(hData, index);
                if (part->frame < half) {
                    blend = (f32)age / (f32)half;
                } else {
                    blend = (f32)(lifetime - age) / (f32)half;
                }
                color = effMagaCylinderCalcColor(effColorBlend(0, part->abgr, blend), tintColor);
                effPCPFlashPolyRingPartSetVC(hData, index, color);
            }
        }
        part->frame = part->frame + 1;
    }
    handle = hData->hPoly;
    handle->pos[0] = hData->origin[0];
    hData->frame = hData->frame + 1;
    handle->pos[1] = hData->origin[1];
    handle->pos[2] = hData->origin[2];
    handle->scale = hData->scale;
    dds3FlashPolyListDraw(handle);
}
