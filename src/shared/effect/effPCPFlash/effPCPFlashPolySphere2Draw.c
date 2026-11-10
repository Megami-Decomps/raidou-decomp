#include "shared/effect/effPCPFlash/decls.h"

void effPCPFlashPolySphere2Draw(effPCPFlashPolySphere2_tag *hData) {
    sdfVector4_t axis;
    s32 particleIndex;
    s32 lifetime;
    s32 particleCount;
    s32 half;
    s32 ramp;
    f32 maxScale;
    s32 restart;
    s32 tintColor;
    effPCPFlashPolySphere2Part_tag *part;
    dds3FlashPolyList_t *handle;

    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&axis);
    lifetime = SDF_FRAMES(hData->lifetime);
    particleCount = hData->particleCount;
    part = hData->pPart;
    half = lifetime >> 1;
    ramp = SDF_FRAMES(hData->scaleRampTime);
    maxScale = hData->maxScale;
    restart = hData->restartRandomly;
    tintColor = hData->abgr;
    for (particleIndex = 0; particleIndex < particleCount; particleIndex++, part++) {
        s32 age = part->frame;
        s32 color;
        f32 blend;

        if (part->frame == 0) {
            effPCPFlashPolySphere2PartSetVertex(hData, particleIndex, &axis);
            effPCPFlashPolySphere2PartSetVC(hData, particleIndex, 0);
            if (ramp == 0) {
                part->radius = maxScale;
            } else {
                part->radius = 0.0f;
            }
            part->abgr = EFF_NEUTRAL_COLOR;
        } else {
            if (part->frame >= lifetime) {
                if (restart != 0) {
                    part->frame = 0;
                }
                color = 0;
                effPCPFlashPolySphere2PartSetVC(hData, particleIndex, color);
            } else if (part->frame > 0) {
                if (ramp == 0) {
                    part->radius = maxScale;
                } else {
                    part->radius = (maxScale * (f32)part->frame) / (f32)ramp;
                    if (maxScale < part->radius) {
                        part->radius = maxScale;
                    }
                }
                effPCPFlashPolySphere2PartSetVertex(hData, particleIndex, &axis);
                if (part->frame < half) {
                    blend = (f32)age / (f32)half;
                } else {
                    blend = (f32)(lifetime - age) / (f32)half;
                }
                color = effMagaCylinderCalcColor(effColorBlend(0, part->abgr, blend), tintColor);
                effPCPFlashPolySphere2PartSetVC(hData, particleIndex, color);
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
    dds3FlashPolyList2Draw(handle);
}
