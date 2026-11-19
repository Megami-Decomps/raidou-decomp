#include "common.h"
#include "effBattle.h"

INCLUDE_ASM(const s32, "code_001FC6B8", effLeaf20);

INCLUDE_ASM(const s32, "code_001FC6B8", effBTLSkillCheckRun);

INCLUDE_ASM(const s32, "code_001FC6B8", effBTLFieldColorSetFlags);

INCLUDE_ASM(const s32, "code_001FC6B8", effBTLFieldColorClearFlags);

void effBTLFieldColorResetFlags(void)
{
    effFieldColorFlags = 0;
}
