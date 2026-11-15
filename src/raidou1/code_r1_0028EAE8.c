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

/* NON_MATCHING: 22 of 38 words match. Sd ra and the copy of the buffer argument are swapped at +0x14. */
#ifdef NON_MATCHING
s32 sbtlSobedSAB(s32 id, char *buf)
{
    if ((u32)(id - 288) < 21 || id == 283 || id == 284 || id == 287) {
        sprintf(buf, "%s", sbtlSobedSABData[id]);
    } else {
        sprintf(buf, "/sobed/%03X.SAB", sbtlGetExeSkill());
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0028EAE8", sbtlSobedSAB);
#endif

INCLUDE_SDATA(const s32, "code_r1_0028EAE8", sbtlSobedSABData2);

