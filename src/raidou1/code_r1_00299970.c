#include "common.h"
#include "sBtlEff.h"
#include "sBtlUnit.h"
#include "sBtlSkill.h"
#include "sbattle.h"
#include "sbtlWork.h"
#include "EventBit.h"
#include "fileManager.h"
#include "sdk_libc.h"
#include "newdata.h"
#include "sBtlEncount.h"
#include "sBtlCalc.h"
#include "sbtlSub.h"
#include "fld.h"
#include "ps2asm.h"
#include "code_r1_001CC1D0.h"

void sbtlFunc2(void)
{
    sbtlFileEntry_t *entry = sbtlFunc2Data;
    u32 i;

    for (i = 0; i < 10; i++) {
        sbtlCallFileOpen(entry++);
    }
}
