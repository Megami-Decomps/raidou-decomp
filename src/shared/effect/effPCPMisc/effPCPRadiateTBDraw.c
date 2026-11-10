#include "shared/effect/effPCPMisc/decls.h"

void effPCPRadiateTBDraw(effPCPRadiateTB_tag *hData) {
    effEveBEDParams_t place QWORD_ALIGNED;
    f32 origin[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    f32 tiltAxis[4] __attribute__((aligned(16)));
    f32 viewAxis[4] __attribute__((aligned(16)));
    u32 repeat;
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    s32 delaySpread;
    u32 count;
    u32 color;
    effPCPRadiateTBPart_tag *entry;
    u32 i;
    s32 frame;

    count = hData->param.count;
    repeat = hData->param.repeat;
    duration = SDF_FRAMES(hData->param.duration);
    fadeIn = SDF_FRAMES(hData->param.fadeIn);
    fadeOut = SDF_FRAMES(hData->param.fadeOut);
    delaySpread = hData->param.delaySpread;
    entry = hData->pPart;
    color = hData->abgr;
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
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(viewAxis);
    for (i = 0; i < count; i++, entry++) {
        frame = entry->frame;
        if (frame <= duration) {
            if (frame == 0) {
                effPCPRadiateTBPartInit(hData, i);
                frame = entry->frame;
            }
            if (frame >= 0) {
                s32 elapsedFrames = frame;
                effPCP_ThunderPolyParam_t *fragment;
                f32 radius;
                f32 fade;
                u32 fadedColor;

                fragment = effPCP_ThunderPolyGetParam(entry->pThunderPoly);
                SetRotateMatrixAxis(entry->radian, (const struct sdfVector3_t *)viewAxis);
                SDF_VU0_LOADVECTOR(effPCPRadiateTBParam);
                SDF_VU0_LOADVECTOR_B(viewAxis);
                SDF_VU0_OUTERPRODUCT();
                SDF_VU0_NORMALIZE();
                SDF_VU0_APPLYMATRIX_XYZ();
                SDF_VU0_STOREVECTOR(direction);
                SDF_VU0_LOADVECTOR_B(effPCPRadiateTBParam);
                SDF_VU0_OUTERPRODUCT();
                SDF_VU0_NORMALIZE();
                /* This SDK-produced axis is consumed only by the external builder. */
                SDF_VU0_STOREVECTOR_NOMEM(tiltAxis);
                radius = entry->radius;
                SetRotateMatrixAxis(entry->rotate, (const struct sdfVector3_t *)tiltAxis);
                SDF_VU0_LOADVECTOR(direction);
                SDF_VU0_APPLYMATRIX_XYZ();
                SDF_VU0_STOREVECTOR(direction);
                fragment->start[0] = origin[0] + direction[0] * radius;
                fragment->start[1] = origin[1] + direction[1] * radius;
                fragment->start[2] = origin[2] + direction[2] * radius;
                fragment->end[0] = origin[0];
                fragment->end[1] = origin[1];
                fragment->end[2] = origin[2];
                entry->rotate += SDF_FRAME_STEP(entry->rotate_s);
                if (elapsedFrames < fadeIn && fadeIn != 0) {
                    fade = (f32)elapsedFrames / (f32)fadeIn;
                } else {
                    s32 remainingFrames = duration - frame;
                    if (remainingFrames <= fadeOut && fadeOut != 0) {
                        fade = (f32)remainingFrames / (f32)fadeOut;
                    } else {
                        fade = 1.0f;
                    }
                }
                fadedColor = effColorBlend(color & 0xFFFFFF, color, fade);
                effPCP_ThunderPolySetColor(entry->pThunderPoly, fadedColor);
                effPCP_ThunderPolyDraw(entry->pThunderPoly);
                place.abgr = fadedColor;
                SDF_VU0_COPYVECTOR(place.center, fragment->end);
                effEveBEDCutSetParams(entry->eventA, &place);
                effEveBEDCutDraw(entry->eventA);
                SDF_VU0_COPYVECTOR(place.center, fragment->start);
                effEveBEDCutSetParams(entry->eventB, &place);
                effEveBEDCutDraw(entry->eventB);
                frame = entry->frame;
            }
            if (repeat != 0 && frame >= duration) {
                entry->frame = -SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % delaySpread);
            } else {
                entry->frame = frame + 1;
            }
        }
    }
}
