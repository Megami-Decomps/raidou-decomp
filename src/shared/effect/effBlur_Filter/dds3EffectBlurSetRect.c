#include "shared/effect/effBlur_Filter/decls.h"

void dds3EffectBlurSetRect(dds3Effect_PartialMultiBlurParam_t *work) {
    s32 x = work->Blur.center[0] + 0x100;
    s32 y = work->Blur.center[1] + 0xE0;
    s32 extent = work->area;

    work->Blur.xy[0][0] = x - extent;
    work->Blur.xy[0][1] = y - extent;
    work->Blur.xy[1][0] = x + extent;
    work->Blur.xy[1][1] = y + extent;
    dds3EffectBlur(&work->Blur);
}
