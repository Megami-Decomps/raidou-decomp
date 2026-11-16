#include "common.h"
#include "effect.h"

INCLUDE_ASM(const s32, "code_001F9908", effComputeBlurRectBounds);

f32 effBlurGetBlurValid(void)
{
    return EFF_BLURVAL;
}

INCLUDE_ASM(const s32, "code_001F9908", effBlurSetBlurValid);

INCLUDE_ASM(const s32, "code_001F9908", dds3PCPHama01PartInitSub);

INCLUDE_ASM(const s32, "code_001F9908", dds3EffectPartGetType);
