#include "common.h"
#include "effect.h"
#include "effPart.h"
#include "effBlur_Filter.h"

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_00187DB0.c:109, match, same function 1.00 (words 29/29, calls 1.00, shape 1.00) */
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

INCLUDE_ASM(const s32, "code_001F9908", effBlurSetBlurValid);

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPart.c:139, rework, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void *dds3PCPHama01PartInitSub(effPartHandle_t *hEffPart)
{
    return hEffPart->pHandle;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPart.c:144, rework, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
u16 dds3EffectPartGetType(effPartHandle_t *hEffPart)
{
    return hEffPart->type;
}
