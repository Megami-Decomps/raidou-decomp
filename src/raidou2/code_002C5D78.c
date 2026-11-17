#include "common.h"
#include "sbtlWork.h"
#include "sdfMemory.h"
#include "sdk_libc.h"
#include "EventBit.h"
#include "sBtlVisual.h"
#include "sBtlItem.h"
#include "code_r1_0021CE60.h"
#include "sdfFile.h"
#include "sdfSystem.h"

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCheckLoadFlg);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallMemFreeDirect2);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallCheckEnd);

s32 sbtlLeaf77(struct SBTL_PANEL *panel) {
    if (panel == NULL) {
        return 0;
    }
    if (!(panel->flg & 1)) {
        return 0;
    }
    if (!(panel->flg & 2)) {
        return 0;
    }
    switch (panel->mode) {
    case 1:
        break;
    case 0:
        panel->alpha += 16;
        panel->alpha = (panel->alpha <= 0) ? 0 : (panel->alpha > 128) ? 128 : panel->alpha;
        if (panel->alpha >= 128) {
            panel->mode = 1;
        }
        break;
    case 2:
        panel->alpha -= 16;
        panel->alpha = (panel->alpha <= 0) ? 0 : (panel->alpha > 128) ? 128 : panel->alpha;
        if (panel->alpha <= 0) {
            panel->mode = 3;
        }
        break;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_002C5D78", sbtlLeaf78);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlLeaf79);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlLeaf80);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlBtlDrawTextSub);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlLeaf81);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlDrawSprite_RGBA2);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlRefSbtl50);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallBtlSearchUnitID12);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallRefSbtl51);

INCLUDE_RODATA(const s32, "code_002C5D78", sbtlRefSbtl50Table);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlSummonMenuProc);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlResetSummonPanel);

INCLUDE_ASM(const s32, "code_002C5D78", SummonMenuFunc_Open00);

INCLUDE_ASM(const s32, "code_002C5D78", SummonMenuFunc_00);

INCLUDE_ASM(const s32, "code_002C5D78", SummonMenuFunc_10);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlGetMenuStatus);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlResetMenuPara);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCancelMenuPara);

INCLUDE_RODATA(const s32, "code_002C5D78", SummonMenuFunc_10Table);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlSetMenuPara);

INCLUDE_ASM(const s32, "code_002C5D78", SummonMenuFunc_50);

INCLUDE_ASM(const s32, "code_002C5D78", SummonMenuFunc_60);

INCLUDE_ASM(const s32, "code_002C5D78", SummonMenuFunc_20);

INCLUDE_ASM(const s32, "code_002C5D78", SummonMenuFunc_30);

INCLUDE_ASM(const s32, "code_002C5D78", SummonMenuFunc_40);

INCLUDE_ASM(const s32, "code_002C5D78", SummonMenuFunc_70);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlResetColorFlag);

INCLUDE_ASM(const s32, "code_002C5D78", CheckSummonTarget);

INCLUDE_ASM(const s32, "code_002C5D78", effAllocateModelBindings);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallMemAllocDirect6);

INCLUDE_ASM(const s32, "code_002C5D78", Proc_sbtlEXP);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallMemFreeDirect6);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlBattleEnd);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallBtlSearchUnitID26);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlLeaf82);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCheckTutorialEnd);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallBtlCheckTutorialBattle);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCheckBattleWin);

INCLUDE_ASM(const s32, "code_002C5D78", campFreeStatusModel);

s32 sbtlCheckTutorialBattle(void) {
    return evtBitCheck(2100) != 0;
}

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallStartRain);

INCLUDE_RODATA(const s32, "code_002C5D78", sbtlCallBtlSearchUnitID26Table);

INCLUDE_RODATA(const s32, "code_002C5D78", sbtlCallBtlSearchUnitID26Table2);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlBattleEventSb);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlFreeUIScene);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlLeaf32);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlGetEventMes);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlEventProcTbl00Sub);

INCLUDE_RODATA(const s32, "code_002C5D78", sbtlGetEventMesTable);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlMessageProc);

INCLUDE_ASM(const s32, "code_002C5D78", EventProc);

INCLUDE_ASM(const s32, "code_002C5D78", EventProcTbl_00);

INCLUDE_ASM(const s32, "code_002C5D78", EventProcTbl_01);

INCLUDE_ASM(const s32, "code_002C5D78", EventProcTbl_02);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallBtlSearchUnitID13);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallBtlSearchUnitID14);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallMemset10);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlLoadHuuma_MesData);

INCLUDE_ASM(const s32, "code_002C5D78", EventProcTbl_07);

INCLUDE_ASM(const s32, "code_002C5D78", EventProcTbl_06);

INCLUDE_RODATA(const s32, "code_002C5D78", EventProcTbl_00Table);

INCLUDE_ASM(const s32, "code_002C5D78", EventProcTbl_08);

INCLUDE_ASM(const s32, "code_002C5D78", EventProcTbl_09);

INCLUDE_ASM(const s32, "code_002C5D78", EventProcTbl_0A);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlLeaf33);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlRefSbtl17);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlRefSbtl18);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlDrawShadow);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallSetOt2);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlGetMotionMoveData);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallBtlGetMotionMoveData);

V_POSITION_WORK *sbtlGetEffCylinderData(MODELTYPE major, s32 id) {
    if (major == 0) {
        return &sbtleff_cylindertbl_p[id];
    }
    return &sbtleff_cylindertbl[id];
}

INCLUDE_ASM(const s32, "code_002C5D78", sbtlGetMotionVisualData);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlGetVisualData);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlGetMotionVisualData_sbtlVisual);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallBtlGetMotionVisualDataSBtlVi2);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlGetActionTime);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlGetRotate_Jump);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlGetCylinderData_ID);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallMemset11);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCalcAnim_DmgNumPos);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallBtlCheckDamageTarget8);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlGetPointData);

INCLUDE_ASM(const s32, "code_002C5D78", SetEncountData_Sub);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlSetEncountPacData);

INCLUDE_ASM(const s32, "code_002C5D78", SetEncountData_Sub_sbtlEncount);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlSetEncountData);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlAddAWXAtkPadSub);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlAddAWXAtkPadSub2);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlSetEncountData4);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlLeaf34);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlSetactcounter);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlInitSBtlSub);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlSetGeneratorData);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlGeneratorD_On);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCheckGeneratorGroupID);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCheckGeneratorGroupID2);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCheckGenerator_ND);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCheckGenerator_D);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCheckGeneratorAll);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlSetGeneCam);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlGeneratorProc);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallBitCheck3);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlGetRndEncType);

INCLUDE_ASM(const s32, "code_002C5D78", campUpdateEffectStep);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallBitCheck4);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlGetRndEncCnt);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallGetRandIntA);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlRefDDS3GBWK3);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlResetRndEncCount);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlRndEncTopPriorityMap);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallGetRandIntA2);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlGetDevilNum);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallBtlGetDevilNum);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallGetRandIntA3);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlSetNextPathData);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlLeaf83);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlBtlSetNextPathDataSub);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlBtlSetNextPathDataSub2);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlInitEffect);

s32 sbtlCallFreeAuto(void **dst, s32 size, s32 *src)
{
    s32 raw = 0;
    s32 len = *src;
    void *old;
    void *buf;
    u32 n;

    if (size != 0 && len != size) {
        len = size;
        raw = 1;
    }
    old = *dst;
    if (old != NULL) {
        sdfFreeAuto(old);
    }
    buf = sdfAllocAuto(len);
    *dst = buf;
    if (!raw) {
        memcpy(buf, src + 1, len);
    } else {
        memcpy(buf, src, len);
    }
    n = (u32)(len + 4) >> 4;
    if ((len + 4) & 0xF) {
        return (n + 1) << 4;
    }
    return n << 4;
}

void sbtlCallFileOpen(sbtlFileEntry_t *entry)
{
    sdfFileCmdHandle_t *file = sdfFileOpen(entry->path);
    s32 size = sdfFileLength(file);
    sdfMemHandle_t *handle = sdfMemAllocFromTailDirect(size);
    u8 *buf = sdfMemLock(handle);
    s32 i;

    sdfFileRead(file, buf, size);
    sdfFileClose(file);
    for (i = 0; entry->slot[i].dst != NULL; i++) {
        buf += sbtlCallFreeAuto(entry->slot[i].dst, entry->slot[i].arg, (s32 *)buf);
    }
    sdfMemFreeDirect(handle);
}

SBTLITEM_TBL *sbtlGetItemtbl(s32 id)
{
    return &tbldata[id];
}

INCLUDE_ASM(const s32, "code_002C5D78", sbtlRefSbtl19);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCreateItemList);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlEntryItemList);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlEntryItemList_Type);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlReleaseItemList);

INCLUDE_RODATA(const s32, "code_002C5D78", sbtlSetactcounterTable);

