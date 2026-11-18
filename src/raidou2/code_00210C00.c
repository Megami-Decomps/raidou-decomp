#include "common.h"
#include "effect.h"
#include "effBlur_Filter.h"

void effComputeBlurRectBounds(dds3Effect_PartialMultiFilter_t *work) {
    f32 scaledExtent = (f32)work->param.area * 1.4f;
    s32 x = work->param.center[0] + 0x1000;
    s32 y = (work->param.center[1] + 0xE00) >> 1;
    s32 extent = (s32)scaledExtent;
    s32 right = x + extent;
    s32 bottom;

    x -= extent;
    extent >>= 1;
    work->param.Filter.bounds.left = x;
    bottom = y + extent;
    y -= extent;
    work->param.Filter.bounds.right = right;
    work->param.Filter.bounds.top = y;
    work->param.Filter.bounds.bottom = bottom;
    effResourceQuadDraw(&work->param.Filter, work->hAlphaTex, 1);
}

f32 effBlurGetBlurValid(void)
{
    return EFF_BLURVAL;
}

void effBlurSetBlurValid(f32 z) {
    EFF_BLURVAL = z;
}

INCLUDE_ASM(const s32, "code_00210C00", dds3PCPHama01PartInitSub);

INCLUDE_ASM(const s32, "code_00210C00", dds3EffectPartGetType);
