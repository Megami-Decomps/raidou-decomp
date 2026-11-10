#include "shared/effect/effPCPScatter/decls.h"

void effPCPWhirlwind3PolyDraw(effPCPWhirlwind3Poly_tag *hData) {
    s32 loop;
    s32 duration = SDF_FRAMES(hData->param.life);
    dds3WhirlwindPolyList_tag *draw = hData->hPoly;
    effPCPWhirlwindPolyPart_tag *particle = hData->pPart;
    u32 count = hData->param.number;
    s32 fadeIn;
    s32 fadeRange;
    u32 delayModulus;
    s32 instanceAge;
    s32 remaining;
    f32 instanceFade;
    f32 particleFade;
    f32 particleAgeFloat;
    s32 tintColor;
    s32 startColor;
    s32 endColor;
    s32 color;
    u32 i;

    instanceAge = hData->frame_all;
    loop = hData->param.loop;
    fadeIn = SDF_FRAMES(hData->param.fade_in);
    fadeRange = SDF_FRAMES(hData->param.fade_out);
    delayModulus = hData->param.timing;
    startColor = hData->param.abgr_s;
    endColor = hData->param.abgr_e;
    tintColor = hData->abgr;
    if (duration < instanceAge) {
        return;
    }
    remaining = duration - instanceAge;
    if (fadeRange >= remaining && fadeRange != 0) {
        instanceFade = (f32)remaining / (f32)fadeRange;
    } else {
        instanceFade = 1.0f;
    }
    for (i = 0; i < count; i++) {
        s32 particleAge = particle->frame;

        if (duration < particleAge) {
            draw->colors[i] = 0;
        } else {
            if (particleAge == 0) {
                effPCPWhirlwind3PolyPartInit(hData, i);
            } else if (particleAge > 0) {
                particleAgeFloat = (f32)particleAge;
                color = effColorBlend(startColor, endColor, particleAgeFloat / (f32)duration);
                if (particleAge < fadeIn && fadeIn != 0) {
                    particleFade = particleAgeFloat / (f32)fadeIn;
                } else {
                    particleFade = 1.0f;
                }
                color = effMagaCylinderCalcColor(tintColor, color);
                draw->colors[i] = effColorBlend(color & EFF_SCATTER_RGB_MASK, color, particleFade * instanceFade);
                effPCPWhirlwind3PolyPartMove(hData, i);
            }
            if (loop != 0 && !(instanceAge < duration)) {
                particle->frame = -SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % delayModulus);
            } else {
                particle->frame++;
            }
        }
        particle++;
    }
    if (loop != 0 && instanceAge >= duration) {
        hData->frame_all = 0;
    } else {
        hData->frame_all++;
    }
    draw->scale = hData->scale;
    SDF_VU0_COPYVECTOR(draw->origin, hData->param.pos);
    dds3WhirlwindPolyListSetRotateMatrix(draw, hData);
    dds3WhirlwindPolyListDraw(draw);
}
