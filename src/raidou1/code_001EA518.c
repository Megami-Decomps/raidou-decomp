#include "common.h"
#include "parManager.h"

INCLUDE_ASM(const s32, "code_001EA518", dds3Leaf36);

void dds3ParticleManagerInit(void)
{
    sdfRandomizeByTime(&ParRand);
}

INCLUDE_ASM(const s32, "code_001EA518", dds3ParticleManagerExit);
