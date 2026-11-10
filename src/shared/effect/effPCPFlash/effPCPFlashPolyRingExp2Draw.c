#include "shared/effect/effPCPFlash/decls.h"

void effPCPFlashPolyRingExp2Draw(effPCPFlashPolyRingExp2_tag *hData) {
    s32 restart;
    s32 tintColor;
    s32 count;
    s32 index;
    s32 lifetime;
    s32 fadeIn;
    s32 fadeOut;
    s32 ramp;
    f32 maxScale;
    u32 range;
    effPCPFlashPolyRingExp2Part_tag *part;
    dds3FlashPolyList_t *handle;

    count = hData->particleCount;
    part = hData->pPart;
    lifetime = SDF_FRAMES(hData->lifetime);
    fadeIn = SDF_FRAMES(hData->fadeInTime);
    fadeOut = SDF_FRAMES(hData->fadeOutTime);
    ramp = SDF_FRAMES(hData->scaleRampTime);
    maxScale = hData->maxScale;
    restart = hData->restartRandomly;
    range = hData->randomRange;
    tintColor = hData->abgr;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->frame;

        if (lifetime < age) {
            effPCPFlashPolyRingExp2PartSetVC(hData, index, 0);
        } else {
            if (age == 0) {
                part->length_max = maxScale;
                if (ramp == 0) {
                    part->length = maxScale;
                } else {
                    part->length = 0.0f;
                }
                effPCPFlashPolyRingExp2PartSetVertex(hData, index);
                effPCPFlashPolyRingExp2PartSetVC(hData, index, 0);
                part->abgr = EFF_NEUTRAL_COLOR;
            } else if (age > 0) {
                s32 remain;
                f32 blend;

                if (ramp == 0) {
                    part->length = maxScale;
                } else {
                    part->length = (maxScale * (f32)age) / (f32)ramp;
                    if (maxScale < part->length) {
                        part->length = maxScale;
                    }
                }
                effPCPFlashPolyRingExp2PartRotate(hData, index);
                effPCPFlashPolyRingExp2PartSetVertex(hData, index);
                if (age < fadeIn && fadeIn != 0) {
                    blend = (f32)age / (f32)fadeIn;
                } else {
                    remain = lifetime - age;
                    if (fadeOut >= remain && fadeOut != 0) {
                        blend = (f32)remain / (f32)fadeOut;
                    } else {
                        blend = 1.0f;
                    }
                }
                effPCPFlashPolyRingExp2PartSetVC(hData, index, effMagaCylinderCalcColor(effColorBlend(0, part->abgr, blend), tintColor));
            }
            if (age == lifetime && restart != 0) {
                part->frame = ~SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % range);
                effPCPFlashPolyRingExp2PartSetVC(hData, index, 0);
            } else {
                part->frame = part->frame + 1;
            }
        }
    }
    handle = hData->hPoly;
    handle->pos[0] = hData->origin[0];
    hData->frame = hData->frame + 1;
    handle->pos[1] = hData->origin[1];
    handle->pos[2] = hData->origin[2];
    handle->scale = hData->scale;
    dds3FlashPolyListDraw(handle);
}
