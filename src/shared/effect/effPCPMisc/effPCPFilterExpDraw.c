#include "shared/effect/effPCPMisc/decls.h"

void effPCPFilterExpDraw(effPCPFilterExp_tag *hData) {
    if (hData->frame <= SDF_FRAMES(hData->param.life)) {
        s32 fadeIn = SDF_FRAMES(hData->param.fade_in);
        s32 fadeOut = SDF_FRAMES(hData->param.fade_out);
        s32 remaining;
        f32 alpha;

        hData->param.Filter.bounds.left = 0;
        hData->param.Filter.bounds.top = 0;
        hData->param.Filter.bounds.right = 512;
        hData->param.Filter.bounds.bottom = 448;
        if (hData->frame < fadeIn && fadeIn != 0) {
            alpha = (f32)hData->frame / (f32)fadeIn;
        } else {
            remaining = SDF_FRAMES(hData->param.life) - hData->frame;
            if (remaining <= fadeOut && fadeOut != 0) {
                alpha = (f32)remaining / (f32)fadeOut;
            } else {
                alpha = 1.0f;
            }
        }
        hData->param.Filter.rgba = effMagaCylinderCalcColor(
            effColorBlend(hData->abgr & 0xFFFFFF, hData->abgr, alpha), hData->abgr_base);
        dds3EffectFilter(&hData->param.Filter);
        hData->frame++;
    }
}
