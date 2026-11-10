#include "shared/effect/effPCPMisc/decls.h"

void effPCPRadiateThunderDraw(effPCPRadiateThunder_tag *work)
{
    f32 point[3];
    f32 origin[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    effPartHandle_t **followers = NULL;
    u32 repeat;
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    u32 count;
    u32 color;
    f32 angularDamping;
    f32 startRadialBlend;
    f32 endRadialBlend;
    effPCPRadiateThunderPart_tag *entry;
    f32 length;
    u32 i;
    s32 frame;

    count = work->param.number;
    repeat = work->param.repeat;
    duration = SDF_FRAMES(work->param.life);
    fadeIn = SDF_FRAMES(work->param.fadeIn);
    fadeOut = SDF_FRAMES(work->param.fadeOut);
    angularDamping = SDF_FRAME_DECAY(work->param.angularDamping);
    startRadialBlend = work->param.startRadialBlend;
    endRadialBlend = work->param.endRadialBlend;
    entry = work->pPart;
    color = work->abgr;
    length = work->height;
    SDF_VU0_COPYVECTOR(origin, work->param.origin);
    if (duration == 0) {
        return;
    }
    for (i = 0; i < count; i++, entry++) {
        frame = entry->frame;
        if (frame <= duration) {
            if (work->phAttach != NULL) {
                followers = work->phAttach + i;
            }
            if (frame == 0) {
                effPCPRadiateThunderPartInit(work, i);
                frame = entry->frame;
            }
            if (frame >= 0) {
                s32 elapsedFrames = entry->frame;
                f32 angle = entry->radian;
                f32 radius = entry->radius;
                f32 cosine;
                f32 sine;
                f32 blend;
                f32 radialHeight;
                f32 fade;
                u32 fadedColor;
                effPCP_ThunderPolyParam_t *fragment;

                cosine = sdfCos(angle);
                sine = sdfSin(angle);
                point[0] = origin[0] + cosine * radius;
                point[1] = origin[1];
                point[2] = origin[2] + sine * radius;
                entry->radius += SDF_FRAME_STEP(entry->radius_s);
                entry->radian += SDF_FRAME_STEP(entry->radian_s);
                entry->radian_s *= angularDamping;
                fragment = effPCP_ThunderPolyGetParam(entry->pThunderPoly);
                blend = startRadialBlend +
                        (endRadialBlend - startRadialBlend) * ((f32)frame / (f32)duration);
                radialHeight = 1.0f - ffabsf(blend);
                direction[0] = cosine * blend;
                direction[1] = radialHeight;
                direction[2] = sine * blend;
                SDF_VU0_LOADVECTOR(direction);
                SDF_VU0_NORMALIZE();
                SDF_VU0_STOREVECTOR(direction);
                fragment->start[0] = point[0] + direction[0] * length;
                fragment->start[1] = point[1] + direction[1] * length;
                fragment->start[2] = point[2] + direction[2] * length;
                fragment->end[0] = point[0];
                fragment->end[1] = point[1];
                fragment->end[2] = point[2];
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
                if (work->phAttach != NULL) {
                    u32 followerIndex;

                    for (followerIndex = 0;
                         followerIndex < ARRAY_COUNT(work->param.activeGroups);
                         followerIndex++) {
                        if (*followers != 0) {
                            dds3EffectPartSetPosition(*followers, fragment->end);
                            dds3EffectPartSetColor(*followers, fadedColor);
                            dds3EffectPartDraw(*followers);
                        }
                        followers += count;
                    }
                }
                frame = entry->frame;
            }
            if (repeat != 0 && frame >= duration) {
                entry->frame = 0;
            } else {
                entry->frame = frame + 1;
            }
        }
    }
}
