#include "common.h"
#include "effBattle.h"

u32 effLeaf20(s32 index) {
    return effLeaf20Data[index][0];
}

INCLUDE_ASM(const s32, "code_001E53C0", effBTLSkillCheckRun);

INCLUDE_ASM(const s32, "code_001E53C0", effBTLFieldColorSetFlags);

INCLUDE_ASM(const s32, "code_001E53C0", effBTLFieldColorClearFlags);

void effBTLFieldColorResetFlags(void) {
    effFieldColorFlags = 0;
}
