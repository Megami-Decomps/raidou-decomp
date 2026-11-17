#include "common.h"
#include "newdata.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "dds3KernelCore.h"
#include "kernel_draw.h"
#include "dds3KernelDraw.h"
#include "sdf2D.h"
#include "sdfPrim.h"
#include "sdfPacketBuff.h"
#include "sdfDraw.h"
#include "dds3KernelMain.h"
#include "code_r1_0010DF68.h"
#include "code_0010F2C0.h"
#include "scrTraceCode.h"
#include "scrCommand.h"
#include "datCalc.h"

/* body: matched in raidou @ 88f0d54: src/shared/script/scrCommonCommand.c:352, found, same function 0.92 (words 18/18, calls 0.75, shape 1.00) */
s32 gohScrCmd_PresentItem(void)
{
    if (datCheckMoney(scrGetIntPara(0)) != 0)
    {
        scrSetIntReturnValue(1);
    }
    else
    {
        scrSetIntReturnValue(0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_001159C0", gohCallSetIntReturnValue);

INCLUDE_ASM(const s32, "code_001159C0", gohCallPartyRecovery);

INCLUDE_ASM(const s32, "code_001159C0", TakumiGetTarget);

INCLUDE_ASM(const s32, "code_001159C0", TakumiKoukaA);

INCLUDE_ASM(const s32, "code_001159C0", TakumiKoukaB);

INCLUDE_ASM(const s32, "code_001159C0", TakumiKoukaC);

INCLUDE_ASM(const s32, "code_001159C0", TakumiKoukaD);

INCLUDE_ASM(const s32, "code_001159C0", TakumiKoukaE);

INCLUDE_ASM(const s32, "code_001159C0", TakumiKoukaF);

INCLUDE_RODATA(const s32, "code_001159C0", TakumiKoukaDTable);

INCLUDE_RODATA(const s32, "code_001159C0", TakumiKoukaGTable);

INCLUDE_ASM(const s32, "code_001159C0", TakumiKoukaG);

INCLUDE_ASM(const s32, "code_001159C0", ExecTakumi);

INCLUDE_ASM(const s32, "code_001159C0", prmCallGetIntPara5);

INCLUDE_RODATA(const s32, "code_001159C0", ExecTakumiTable2);

INCLUDE_ASM(const s32, "code_001159C0", ExecTeisatuItem);

INCLUDE_ASM(const s32, "code_001159C0", prmCallGetIntPara6);

INCLUDE_ASM(const s32, "code_001159C0", prmCallGetIntPara7);

INCLUDE_ASM(const s32, "code_001159C0", prm_BOSS_PRESS_REC);
