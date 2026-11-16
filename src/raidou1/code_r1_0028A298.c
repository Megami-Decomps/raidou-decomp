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

/* NON_MATCHING: 6 of 34 words match. Retail keeps a dead always-true test of a constant 1 around the call and schedules the table copy differently. */
#ifdef NON_MATCHING
s32 sbtlBtlLoadPlayerOnMemDataSub(s32 index, void *pData)
{
    s32 table[8] = { 145, 146, 147, 148, 144, 283, 284, 287 };
    s32 ret = 1;

    if (ret) {
        ret = sbtlCallBtlSearchEffectData2(table[index], 40, pData);
        if (ret) {
            ret = 1;
        }
        return ret;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0028A298", sbtlBtlLoadPlayerOnMemDataSub);
#endif

INCLUDE_RODATA(const s32, "code_r1_0028A298", sbtlBtlLoadPlayerOnMemDataSubTable);

