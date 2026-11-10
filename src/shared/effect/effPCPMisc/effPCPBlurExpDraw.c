#include "shared/effect/effPCPMisc/decls.h"

void effPCPBlurExpDraw(effPCPBlurExp_tag *hData) {
    if (hData->frame <= SDF_FRAMES(hData->param.duration)) {
        s32 fadeIn = SDF_FRAMES(hData->param.fadeIn);
        s32 fadeOut = SDF_FRAMES(hData->param.fadeOut);
        s32 remaining;
        f32 alpha;

        hData->param.Blur.center[0] = 0;
        hData->param.Blur.center[1] = 0;
        hData->param.Blur.xy[0][0] = 0;
        hData->param.Blur.xy[0][1] = 0;
        hData->param.Blur.xy[1][0] = 512;
        hData->param.Blur.xy[1][1] = 448;
        if (hData->frame < fadeIn && fadeIn != 0) {
            alpha = (f32)hData->frame / (f32)fadeIn;
        } else {
            remaining = SDF_FRAMES(hData->param.duration) - hData->frame;
            if (remaining <= fadeOut && fadeOut != 0) {
                alpha = (f32)remaining / (f32)fadeOut;
            } else {
                alpha = 1.0f;
            }
        }
        hData->param.Blur.abgr = effMagaCylinderCalcColor(
            effColorBlend(hData->abgr & 0xFFFFFF, hData->abgr, alpha), hData->abgr_base);
        dds3EffectBlur(&hData->param.Blur);
        hData->frame++;
    }
}
