#include "common.h"
#include "effect.h"

INCLUDE_ASM(const s32, "code_00210C00", effComputeBlurRectBounds);

f32 effBlurGetBlurValid(void)
{
    return EFF_BLURVAL;
}

void effBlurSetBlurValid(f32 z) {
    EFF_BLURVAL = z;
}

INCLUDE_ASM(const s32, "code_00210C00", dds3PCPHama01PartInitSub);

INCLUDE_ASM(const s32, "code_00210C00", dds3EffectPartGetType);
