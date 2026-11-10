#include "shared/effect/effPCPFlash/decls.h"

void effPCPFlashPolySphere3Draw(effPCPFlashPolySphere3_tag *hData) {
    s128 axis;
    s32 index;
    s32 count;
    s32 lifetime;
    f32 maxThickness;
    s32 fadeIn;
    s32 fadeOut;
    f32 startA;
    f32 startB;
    f32 decay;
    s32 restart;
    u32 range;
    s32 tintColor;
    effPCPFlashPolySphere3Part_tag *part;
    dds3FlashPolyList_t *handle;

    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
        SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(&axis);
    count = hData->particleCount;
    part = hData->parts;
    lifetime = SDF_FRAMES(hData->lifetime);
    maxThickness = hData->maxThickness;
    fadeIn = SDF_FRAMES(hData->fadeInTime);
    fadeOut = SDF_FRAMES(hData->fadeOutTime);
    startA = hData->initialRadius;
    startB = hData->initialRadialSpeed;
    decay = hData->radialDamping;
    restart = hData->restartRandomly;
    range = hData->randomRange;
    tintColor = hData->abgr;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->frame;

        if (lifetime < age) {
            effPCPFlashPolySphere3PartSetVC(hData, index, 0);
        } else {
            if (age == 0) {
                effPCPFlashPolySphere3PartInit(hData, index, &axis);
                effPCPFlashPolySphere3PartSetVertex(hData, index, &axis);
                effPCPFlashPolySphere3PartSetVC(hData, index, 0);
                part->radius = maxThickness;
                part->move = startA;
                part->move_s = startB;
                part->abgr = EFF_NEUTRAL_COLOR;
            } else if (age > 0) {
                f32 speed = part->move_s;
                s32 remain;
                f32 blend;

                part->move_s = speed * SDF_FRAME_DECAY(decay);
                part->move = part->move + SDF_FRAME_STEP(speed);
                effPCPFlashPolySphere3PartRotate(hData, index, &axis);
                effPCPFlashPolySphere3PartSetVertex(hData, index, &axis);
                age = part->frame;
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
                effPCPFlashPolySphere3PartSetVC(hData, index, effMagaCylinderCalcColor(effColorBlend(0, part->abgr, blend), tintColor));
            }
            if (age == lifetime && restart != 0) {
                part->frame = ~SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % range);
                effPCPFlashPolySphere3PartSetVC(hData, index, 0);
            } else {
                part->frame = part->frame + 1;
            }
        }
    }
    handle = hData->hPoly;
    handle->pos[0] = hData->origin[0];
    hData->updateCount = hData->updateCount + 1;
    handle->pos[1] = hData->origin[1];
    handle->pos[2] = hData->origin[2];
    handle->scale = hData->scale;
    dds3FlashPolyList3Draw(handle);
}
