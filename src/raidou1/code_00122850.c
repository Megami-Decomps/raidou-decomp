#include "common.h"
#include "newdata.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "dds3KernelCore.h"
#include "model.h"
#include "dds3ObjectBase.h"

INCLUDE_ASM(const s32, "code_00122850", nbRefDDS3GBWK11);

INCLUDE_ASM(const s32, "code_00122850", nbLeaf54);

/* body: matched in dds @ 18d1fa88f: src/dds1/newdata/datCalc.c:2059, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void btlPreemptiveChanceHook(void) {
}

/* body: matched in dds @ 18d1fa88f: src/dds1/newdata/datCalc.c:2063, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void btlSerialBattleChanceHook(s32 chance) {
}

/* body: matched in dds @ 18d1fa88f: src/dds1/newdata/datCalc.c:2067, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void btlSurpriseChanceHook(s32 chance) {
}
