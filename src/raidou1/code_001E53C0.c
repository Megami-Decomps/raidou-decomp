#include "common.h"
#include "effBattle.h"

INCLUDE_ASM(const s32, "code_001E53C0", effLeaf20);

INCLUDE_ASM(const s32, "code_001E53C0", effBTLSkillCheckRun);

INCLUDE_ASM(const s32, "code_001E53C0", effLeaf21);

INCLUDE_ASM(const s32, "code_001E53C0", effLeaf22);

void effBTLFieldColorResetFlags(void) {
    effFieldColorFlags = 0;
}
