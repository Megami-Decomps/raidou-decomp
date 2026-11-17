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

/* body: matched in raidou @ 61f09dc: src/raidou1/sbtlEff_00289E50.c:75, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void sbtlLeaf45(dds3ProcessID_t *id) {
}

void sbtlDebugTimeGrph(s8 enabled)
{
    if (enabled == 1) {
        sbtlDebugTimeGrphWork = dds3StartProcess(sbtlStrDebugTimeGrph, 10000, 1, 1, evtDrawResourceCountDebugTask,
                                                 (dds3ProcessFunc_t)sbtlLeaf45, NULL);
    } else if (enabled == 0) {
        dds3KillProcess(sbtlDebugTimeGrphWork, 0);
    }
}

INCLUDE_ASM(const s32, "code_0010F2C0", sbtlFunc8);

INCLUDE_ASM(const s32, "code_0010F2C0", sbtlLeaf46);

INCLUDE_ASM(const s32, "code_0010F2C0", sbtlBtlDrawParamSub);

INCLUDE_ASM(const s32, "code_0010F2C0", sbtlCallGetPlayerExp);

INCLUDE_ASM(const s32, "code_0010F2C0", sbtlRefDDS3GBWK7);

INCLUDE_ASM(const s32, "code_0010F2C0", sbtlRefDDS3GBWK8);

INCLUDE_ASM(const s32, "code_0010F2C0", sbtlRefDDS3GBWK9);

INCLUDE_ASM(const s32, "code_0010F2C0", sbtlRefDDS3GBWK10);

INCLUDE_ASM(const s32, "code_0010F2C0", sbtlRefDDS3GBWK11);

INCLUDE_ASM(const s32, "code_0010F2C0", sbtlRefDDS3GBWK12);

INCLUDE_ASM(const s32, "code_0010F2C0", sbtlRefDDS3GBWK13);

INCLUDE_ASM(const s32, "code_0010F2C0", sbtlDrawParam);

INCLUDE_ASM(const s32, "code_0010F2C0", sbtlFunc9);

INCLUDE_ASM(const s32, "code_0010F2C0", sbtlRefDDS3GBWK14);

INCLUDE_ASM(const s32, "code_0010F2C0", sbtlLEVEL);

INCLUDE_ASM(const s32, "code_0010F2C0", sbtlCallAddItem);

INCLUDE_ASM(const s32, "code_0010F2C0", sbtlCallGetSmallFontFRQ2);

INCLUDE_ASM(const s32, "code_0010F2C0", sbtlCallCheckItem);

INCLUDE_ASM(const s32, "code_0010F2C0", sbtlCallGetStockDevilNums);

INCLUDE_ASM(const s32, "code_0010F2C0", sbtlCallGetSmallFontFRQ22);

INCLUDE_ASM(const s32, "code_0010F2C0", sbtlLeaf47);

INCLUDE_ASM(const s32, "code_0010F2C0", sbtlRefDDS3GBWK15);

INCLUDE_ASM(const s32, "code_0010F2C0", sbtlCallGetDevilLevelUpUnit);

INCLUDE_ASM(const s32, "code_0010F2C0", sbtlRefDDS3GBWK);

INCLUDE_ASM(const s32, "code_0010F2C0", sbtlRefDDS3GBWK16);

INCLUDE_ASM(const s32, "code_0010F2C0", sbtlRefDDS3GBWK17);

INCLUDE_ASM(const s32, "code_0010F2C0", sbtlRefDDS3GBWK18);

INCLUDE_ASM(const s32, "code_0010F2C0", sbtlRefDDS3GBWK19);

INCLUDE_ASM(const s32, "code_0010F2C0", nbRefDDS3GBWK4);

INCLUDE_ASM(const s32, "code_0010F2C0", nbRefDDS3GBWK5);

INCLUDE_ASM(const s32, "code_0010F2C0", nbRefDDS3GBWK6);

INCLUDE_ASM(const s32, "code_0010F2C0", nbRefDDS3GBWK7);

INCLUDE_ASM(const s32, "code_0010F2C0", nbRefDDS3GBWK8);

INCLUDE_ASM(const s32, "code_0010F2C0", nbCallGetSmallFontFRQ2);

INCLUDE_ASM(const s32, "code_0010F2C0", nbRefDDS3GBWK);

INCLUDE_ASM(const s32, "code_0010F2C0", nbLEVEL);

INCLUDE_ASM(const s32, "code_0010F2C0", nbRefDDS3GBWK9);

INCLUDE_ASM(const s32, "code_0010F2C0", nbTOTAL);

INCLUDE_ASM(const s32, "code_0010F2C0", nbCallSetAgeOfMoon);

INCLUDE_ASM(const s32, "code_0010F2C0", GohInitSkillChange);

INCLUDE_ASM(const s32, "code_0010F2C0", nbRefDDS3GBWK10);

INCLUDE_ASM(const s32, "code_0010F2C0", nbNUMS2);

INCLUDE_ASM(const s32, "code_0010F2C0", nbCallMemAllocDirect);

INCLUDE_ASM(const s32, "code_0010F2C0", nbCallGetProcessData31);

INCLUDE_ASM(const s32, "code_0010F2C0", nbGetEventForm);

INCLUDE_RODATA(const s32, "code_0010F2C0", sbtlStrDebugTimeGrph);

INCLUDE_RODATA(const s32, "code_0010F2C0", sbtlLEVELTable);

INCLUDE_RODATA(const s32, "code_0010F2C0", sbtlCallCheckItemTable);

INCLUDE_RODATA(const s32, "code_0010F2C0", sbtlRefDDS3GBWK15Table);

INCLUDE_RODATA(const s32, "code_0010F2C0", nbLEVELTable);

INCLUDE_RODATA(const s32, "code_0010F2C0", nbTOTALTable);

INCLUDE_RODATA(const s32, "code_0010F2C0", GohInitSkillChangeTable);

INCLUDE_RODATA(const s32, "code_0010F2C0", nbNUMS2Table);

INCLUDE_ASM(const s32, "code_0010F2C0", nbUnitEdit);

INCLUDE_ASM(const s32, "code_0010F2C0", nbCallKillProcess);
