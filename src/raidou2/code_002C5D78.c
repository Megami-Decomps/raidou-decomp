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
#include "code_002929C8.h"
#include "evt.h"
#include "sBtlEncount.h"
#include "newdata.h"
#include "sdfChip.h"

s32 sbtlCheckLoadFlg(void) {
    u32 load;

    if (wk_sbtl == NULL || wk_sbtl->panel == NULL) {
        return 0;
    }
    load = wk_sbtl->panel->flg & 2;
    return load != 0;
}

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

s32 sbtlLeaf78(void)
{
    return 1;
}

INCLUDE_ASM(const s32, "code_002C5D78", sbtlLeaf79);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlLeaf80);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlBtlDrawTextSub);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlLeaf81);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlDrawSprite_RGBA2);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlRefSbtl50);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallBtlSearchUnitID12);

u32 sbtlCallRefSbtl51(void) {
    sbtlSummonMenuProc(sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID));
    return 1;
}

INCLUDE_RODATA(const s32, "code_002C5D78", sbtlRefSbtl50Table);

void sbtlSummonMenuProc(struct SBTLUNIT_WORK *wk) {
    switch (wk_sbtl->field436) {
    case 1:
        sbtlRefSbtl51Data[wk_sbtl->field440](wk, &wk_sbtl->mode);
        break;
    case 2:
        sbtlRefSbtl51Data2[wk_sbtl->field440](wk, &wk_sbtl->mode);
        break;
    case 3:
        sbtlRefSbtl51Data3[wk_sbtl->field440](wk, &wk_sbtl->mode);
        break;
    case 4:
        sbtlRefSbtl51Data4[wk_sbtl->field440](wk, &wk_sbtl->mode);
        break;
    case 5:
        sbtlRefSbtl51Data5[wk_sbtl->field440](wk, &wk_sbtl->mode);
        break;
    case 6:
        sbtlRefSbtl51Data6[wk_sbtl->field440](wk, &wk_sbtl->mode);
        break;
    case 7:
        sbtlRefSbtl51Data7[wk_sbtl->field440](wk, &wk_sbtl->mode);
        break;
    }
}

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

struct SBTL_MODELBIND_LIST *effAllocateModelBindings(void) {
    struct SBTL_MODELBIND_LIST *list = sdfChipAllocZeroDirect(sizeof(SBTL_MODELBIND_LIST));

    list->num = 0;
    list->top = NULL;
    return list;
}

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallMemAllocDirect6);

INCLUDE_ASM(const s32, "code_002C5D78", Proc_sbtlEXP);

s32 sbtlCallMemFreeDirect6(struct SBTL_MODELBIND_LIST *stp, u8 flg) {
    struct SBTL_MODELBIND *node;
    struct SBTL_MODELBIND *next;


    if (stp == NULL) {
        return 0;
    }
    node = stp->top;
    if (node == NULL || stp->num == 0) {
        return 0;
    }
    do {
        if (flg) {
            node->status = 2;
        }
        if (node->status == 2) {
            if (node->prev != NULL) {
                node->prev->next = node->next;
            }
            if (node->next != NULL) {
                node->next->prev = node->prev;
            }
            if (node->prev == NULL) {
                stp->top = node->next;
                if (stp->top != NULL) {
                    stp->top->prev = NULL;
                }
            }
            next = node->next;
            sdfMemFreeDirect(node->handle);
            node = next;
            stp->num--;
        } else {
            node = node->next;
        }
    } while (node != NULL);
    return 1;
}

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

s32 sbtlCallStartRain(void) {
    s32 ret = 1;

    if (wk_sbtl->stageNo == 26) {
        switch (evtCallGetProcessData29()) {
        case 0:
            evtStartRain(1);
            ret = 0;
            break;
        case 1:
            ret = 0;
            break;
        case 2:
            ret = 1;
            break;
        }
    }
    return ret;
}

INCLUDE_RODATA(const s32, "code_002C5D78", sbtlCallBtlSearchUnitID26Table);

INCLUDE_RODATA(const s32, "code_002C5D78", sbtlCallBtlSearchUnitID26Table2);

s32 sbtlBattleEventSb(void) {
    if (wk_sbtl->flg & 0x10000000) {
        return 1;
    }
    switch (wk_sbtl->evtloadst) {
    case 0:
        wk_sbtl->evtloadFile = fileLoad("/battle/event/sb_btlevent.bf");
        wk_sbtl->evtloadst++;
        break;
    case 1:
        if (wk_sbtl->evtloadpData == NULL && pacFileCheckEnd(wk_sbtl->evtloadFile)) {
            wk_sbtl->evtloadHandle = fileGetMemHandle(wk_sbtl->evtloadFile);
            wk_sbtl->evtloadpData = sdfMemLock(wk_sbtl->evtloadHandle);
            fileHandleFree(wk_sbtl->evtloadFile);
            wk_sbtl->evtloadFile = NULL;
            wk_sbtl->flg |= 0x10000000;
            return 1;
        }
        break;
    default:
        return 0;
    }
    return 0;
}

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

struct SBTLUNIT_WORK *sbtlSetEncountData(s32 devil, s32 posid, s32 groupID)
{
    return SetEncountData_Sub_sbtlEncount(devil, posid, groupID, 0);
}

struct SBTLUNIT_WORK *sbtlAddAWXAtkPadSub(s32 devilId, s32 point, s32 arg)
{
    return SetEncountData_Sub_sbtlEncount(devilId, point, arg, 1);
}

INCLUDE_ASM(const s32, "code_002C5D78", sbtlAddAWXAtkPadSub2);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlSetEncountData4);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlLeaf34);

void sbtlSetactcounter(struct SBTLUNIT_WORK *wk)
{
    f32 rate[5] = { 0.25f, 0.5f, 1.0f, 1.0f, 2.0f };

    if (wk->field414 & 1) {
        wk->actMax = wk->actMax * rate[wk->unit.seikaku];
    }
}

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

s32 sbtlGetRndEncCnt(void)
{
    return DDS3_GBWK->encountcnt;
}

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallGetRandIntA);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlRefDDS3GBWK3);

void sbtlResetRndEncCount(void)
{
    DDS3_GBWK->encountkyori = 0.0f;
    DDS3_GBWK->encountcnt = 0;
    encmax = 0;
}

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

SBTLITEM_LISTWORK *sbtlCreateItemList(void)
{
    SBTLITEM_LISTWORK *lwk = sdfChipAllocZeroDirect(sizeof(SBTLITEM_LISTWORK));

    lwk->handle = NULL;
    lwk->num = 0;
    return lwk;
}

INCLUDE_ASM(const s32, "code_002C5D78", sbtlEntryItemList);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlEntryItemList_Type);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlReleaseItemList);

