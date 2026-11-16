#include "common.h"
#include "effEvent.h"

INCLUDE_ASM(const s32, "code_001E5A58", effCallMemAllocDirect);

INCLUDE_ASM(const s32, "code_001E5A58", effLeaf24);

INCLUDE_ASM(const s32, "code_001E5A58", effLeaf25);

INCLUDE_ASM(const s32, "code_001E5A58", effPCPBezierEval);

INCLUDE_ASM(const s32, "code_001E5A58", effLeaf26);

INCLUDE_ASM(const s32, "code_001E5A58", effLeaf27);

INCLUDE_ASM(const s32, "code_001E5A58", effLeaf28);

INCLUDE_ASM(const s32, "code_001E5A58", effBlurUpdateSub);

void Eve_BlurOFF(void)
{
    Eve_BlurShow = 0;
}
