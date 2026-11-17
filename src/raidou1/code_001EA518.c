#include "common.h"
#include "parManager.h"

INCLUDE_ASM(const s32, "code_001EA518", dds3Leaf36);

void dds3ParticleManagerInit(void)
{
    sdfRandomizeByTime(&ParRand);
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/parManager.c:810, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void dds3ParticleManagerExit(void) {
}
