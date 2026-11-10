#include "shared/effect/effPCPScatter/decls.h"

void effPCPWwSpherePolyDraw(effPCPWwSpherePoly_tag *hData) {
    s32 loop;
    u32 i;
    u32 count = hData->param.number;
    dds3WhirlwindPolyList_tag *draw = hData->hPoly;
    effPCPWwSpherePolyPart_tag *particle = hData->pPart;
    s32 duration = SDF_FRAMES(hData->param.life);
    s32 fadeIn;
    s32 fadeRange;
    s32 color;
    loop = hData->param.loop;
    fadeIn = SDF_FRAMES(hData->param.fade_in);
    fadeRange = SDF_FRAMES(hData->param.fade_out);
    color = effMagaCylinderCalcColor(hData->abgr, hData->param.abgr);

    for (i = 0; i < count; i++) {
        s32 age = particle->frame;
        if (duration < age) {
            draw->colors[i] = 0;
        } else {
            if (age == 0) {
                effPCPWwSpherePolyPartInit(hData, i);
            } else if (age > 0) {
                s32 remaining = duration - age;
                f32 factor;
                if (age < fadeIn && fadeIn != 0) {
                    factor = (f32)age / (f32)fadeIn;
                } else if (fadeRange >= remaining && fadeRange != 0) {
                    factor = (f32)remaining / (f32)fadeRange;
                } else {
                    factor = 1.0f;
                }
                draw->colors[i] = effColorBlend(color & EFF_SCATTER_RGB_MASK, color, factor);
                effPCPWwSpherePolyPartMove(hData, i);
            }
            if (loop != 0 && age >= duration) {
                particle->frame = 0;
            } else {
                particle->frame++;
            }
        }
        particle++;
    }
    draw->scale = hData->scale;
    SDF_VU0_COPYVECTOR(draw->origin, hData->param.pos);
    dds3WhirlwindPolyListSetRotateMatrix(draw, hData);
    dds3WhirlwindPolyListDraw(draw);
}
