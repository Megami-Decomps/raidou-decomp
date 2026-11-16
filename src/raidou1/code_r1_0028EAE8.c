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

s32 sbtlSobedSAB(s32 id, char *buf)
{
    if ((u32)(id - 288) < 21 || id == 283 || id == 284 || id == 287) {
        sprintf(buf, "%s", sbtlSobedSABData[id]);
    } else {
        sprintf(buf, "/sobed/%03X.SAB", sbtlGetExeSkill(id));
    }
    return 1;
}

