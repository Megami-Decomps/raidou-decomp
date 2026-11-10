#include "shared/effect/effPCPMisc/decls.h"

void effPCPRingPolyExpDraw(effPCPRingPolyExp_tag *hData) {
    s32 vertexFrames = SDF_FRAMES(hData->param.life_a);
    s32 endFrame = vertexFrames;
    s32 frame = hData->frame;
    s32 fadeIn = SDF_FRAMES(hData->param.fade_in);
    s32 fadeOut = SDF_FRAMES(hData->param.fade_out);
    f32 t;
    u32 color;
    dds3RingPolyExpList_tag *beam;

    if (endFrame < SDF_FRAMES(hData->param.life_r)) {
        endFrame = SDF_FRAMES(hData->param.life_r);
    }
    endFrame += SDF_FRAMES(hData->param.wait);
    if (frame > endFrame) return;
    if (frame < vertexFrames) {
        u32 groups;
        t = (f32)frame / (f32)vertexFrames;
        groups = (u32)((f32)(hData->point_max >> 2) * t);
        hData->hPoly->vertexCount = groups << 2;
    } else {
        hData->hPoly->vertexCount = hData->point_max;
    }
    if (frame < SDF_FRAMES(hData->param.life_r)) {
        t = (f32)hData->frame / (f32)SDF_FRAMES(hData->param.life_r);
        effPCPRingPolyExpSetVertex(
            (hData->param.radius_e - hData->param.radius_s) *
                t + hData->param.radius_s,
            hData);
    } else {
        effPCPRingPolyExpSetVertex(hData->param.radius_e, hData);
    }
    if (frame < fadeIn && fadeIn != 0) {
        t = (f32)frame / (f32)fadeIn;
    } else if (endFrame - frame <= fadeOut && fadeOut != 0) {
        t = (f32)(endFrame - frame) / (f32)fadeOut;
    } else {
        t = 1.0f;
    }
    color = effColorBlend(hData->abgr & 0xFFFFFF, hData->abgr, t);
    beam = hData->hPoly;
    beam->color = color;
    SDF_VU0_COPYVECTOR(&beam->pos, hData->param.pos);
    dds3RingPolyExpListDraw(beam);
    hData->frame++;
}
