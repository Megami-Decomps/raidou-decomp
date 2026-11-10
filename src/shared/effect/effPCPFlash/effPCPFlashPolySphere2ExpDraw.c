#include "shared/effect/effPCPFlash/decls.h"

void effPCPFlashPolySphere2ExpDraw(effPCPFlashPolySphere2Exp_tag *hData) {
    s128 axis;
    s32 restart;
    s32 tintColor;
    s32 index;
    s32 active;
    s32 count;
    s32 lifetime;
    s32 fadeIn;
    s32 fadeOut;
    f32 startA;
    f32 startB;
    f32 decay;
    effPCPFlashPolyRingExpPart_tag *part;
    dds3FlashPolyList_t *handle;

    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&axis);
    count = hData->particleCount;
    part = hData->parts;
    lifetime = SDF_FRAMES(hData->lifetime);
    startA = hData->initialRadius;
    startB = hData->initialRadialSpeed;
    decay = hData->radialDamping;
    fadeIn = SDF_FRAMES(hData->fadeInTime);
    fadeOut = SDF_FRAMES(hData->fadeOutTime);
    restart = hData->restartRandomly;
    tintColor = hData->abgr;
    active = 0;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->frame;

        if (lifetime < age) {
            effPCPFlashPolySphere2ExpPartSetVC(hData, index, 0);
        } else {
            if (age == 0) {
                effPCPFlashPolySphere2ExpPartSetVertex(hData, index, &axis);
                effPCPFlashPolySphere2ExpPartSetVC(hData, index, 0);
                part->accumulator = startA;
                part->stepSpeed = startB;
                part->color = EFF_NEUTRAL_COLOR;
            } else if (age > 0) {
                f32 speed = part->stepSpeed;
                s32 remain;
                f32 blend;

                part->stepSpeed = speed * SDF_FRAME_DECAY(decay);
                part->accumulator = part->accumulator + SDF_FRAME_STEP(speed);
                effPCPFlashPolySphere2ExpPartSetVertex(hData, index, &axis);
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
                active++;
                effPCPFlashPolySphere2ExpPartSetVC(hData, index, effMagaCylinderCalcColor(effColorBlend(0, part->color, blend), tintColor));
            }
            if (age == lifetime && restart != 0) {
                part->frame = 0;
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
    if (active != 0) {
        dds3FlashPolyList2Draw(handle);
    }
}
