#include "common.h"
#include "effBattle.h"

INCLUDE_ASM(const s32, "code_001FC6B8", effLeaf20);

INCLUDE_ASM(const s32, "code_001FC6B8", effBTLSkillCheckRun);

INCLUDE_ASM(const s32, "code_001FC6B8", effLeaf21);

INCLUDE_ASM(const s32, "code_001FC6B8", effLeaf22);

void effBTLFieldColorResetFlags(void)
{
    effFieldColorFlags = 0;
}
