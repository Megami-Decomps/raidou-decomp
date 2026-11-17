#include "common.h"
#include "effEvent.h"

INCLUDE_ASM(const s32, "code_001FCD50", effCallMemAllocDirect);

INCLUDE_ASM(const s32, "code_001FCD50", effLeaf24);

INCLUDE_ASM(const s32, "code_001FCD50", effLeaf25);

INCLUDE_ASM(const s32, "code_001FCD50", effPCPBezierEval);

INCLUDE_ASM(const s32, "code_001FCD50", effLeaf26);

INCLUDE_ASM(const s32, "code_001FCD50", effLeaf27);

INCLUDE_ASM(const s32, "code_001FCD50", effLeaf28);

void effBlurUpdateSub(void) {
    Eve_BlurShow = 1;
}

void Eve_BlurOFF(void)
{
    Eve_BlurShow = 0;
}
