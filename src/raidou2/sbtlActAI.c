#include "common.h"
#include "sBtlUnit.h"
#include "sbtlWork.h"
#include "sbtlSub.h"
#include "sBtlAI.h"
#include "sbtlActAI.h"
#include "sBtlPath.h"
#include "sBtlCalc.h"
#include "sBtlSkill.h"
#include "sdfMemory.h"
#include "sdk_libc.h"
#include "ps2asm.h"
#include "sBtlDraw.h"
#include "scrTraceCode.h"
#include "sBtlEncount.h"
#include "code_r1_0027CBE8.h"
#include "sBtlSM.h"
#include "sBtlPlayer.h"
#include "sdfQuat.h"
#include "sbtlSub_0027ED08.h"
#include "sdfVu0.h"

INCLUDE_ASM(const s32, "sbtlActAI", datLeaf81);

INCLUDE_ASM(const s32, "sbtlActAI", datCallStopEffectStop);

INCLUDE_ASM(const s32, "sbtlActAI", datCallSupportRunWarpProcess);

INCLUDE_ASM(const s32, "sbtlActAI", datCallGetIntPara7);

INCLUDE_ASM(const s32, "sbtlActAI", datCommand_GET_DEVIL_ID);

INCLUDE_ASM(const s32, "sbtlActAI", datCallGetIntPara8);

INCLUDE_ASM(const s32, "sbtlActAI", datCallGetIntPara9);

INCLUDE_ASM(const s32, "sbtlActAI", nbCallSetIntReturnValue3);

INCLUDE_ASM(const s32, "sbtlActAI", nbCommand_BTL_EVENT);

INCLUDE_ASM(const s32, "sbtlActAI", nbCallSetIntReturnValue4);

INCLUDE_ASM(const s32, "sbtlActAI", nbCallGetIntPara10);

INCLUDE_ASM(const s32, "sbtlActAI", nbCallGetIntPara11);

INCLUDE_ASM(const s32, "sbtlActAI", at3decCallSetIntReturnValue);

INCLUDE_ASM(const s32, "sbtlActAI", at3decCallGetIntPara);

INCLUDE_ASM(const s32, "sbtlActAI", at3decCallGetIntPara2);

INCLUDE_ASM(const s32, "sbtlActAI", at3decReadBits);

INCLUDE_ASM(const s32, "sbtlActAI", at3decCallGetIntPara3);

INCLUDE_ASM(const s32, "sbtlActAI", at3decCallGetIntPara4);

INCLUDE_ASM(const s32, "sbtlActAI", nbCallGetIntPara12);

INCLUDE_ASM(const s32, "sbtlActAI", nbCallSetIntReturnValue5);

INCLUDE_ASM(const s32, "sbtlActAI", nbCommand_BTL_CAM_BOUGAI);

INCLUDE_ASM(const s32, "sbtlActAI", nbFunc28);

INCLUDE_ASM(const s32, "sbtlActAI", nbCallGetIntPara13);

INCLUDE_ASM(const s32, "sbtlActAI", nbCallGetIntPara14);

INCLUDE_ASM(const s32, "sbtlActAI", nbCallEncGetPointNums);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara2);

INCLUDE_ASM(const s32, "sbtlActAI", nbCallGetIntPara15);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallSupportGetNakamaData);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallSetIntReturnValue12);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara37);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara38);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara39);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlGetBadStatus3);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara3);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara4);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara40);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallSetIntReturnValue13);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara41);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallSetIntReturnValue14);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara42);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallSetIntReturnValue15);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara43);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallSetIntReturnValue16);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara44);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallSetIntReturnValue17);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara45);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara46);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara47);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara48);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara49);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara50);

INCLUDE_ASM(const s32, "sbtlActAI", nbCallSetIntReturnValue6);

INCLUDE_ASM(const s32, "sbtlActAI", nbCallGetIntPara16);

INCLUDE_ASM(const s32, "sbtlActAI", RunReactProcess);

INCLUDE_ASM(const s32, "sbtlActAI", nbCallGetIntPara17);

INCLUDE_ASM(const s32, "sbtlActAI", nbCallSetIntReturnValue7);

INCLUDE_ASM(const s32, "sbtlActAI", nbCallGetIntPara18);

INCLUDE_ASM(const s32, "sbtlActAI", nbCallGetIntPara19);

INCLUDE_ASM(const s32, "sbtlActAI", nbFunc29);

INCLUDE_ASM(const s32, "sbtlActAI", nbCallGetIntPara20);

INCLUDE_ASM(const s32, "sbtlActAI", PosReSet);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara5);

INCLUDE_ASM(const s32, "sbtlActAI", Support_SetPosInit);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlGetPointData);

INCLUDE_ASM(const s32, "sbtlActAI", Support_SetPos);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara6);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara7);

INCLUDE_ASM(const s32, "sbtlActAI", nbCallGetIntPara24);

INCLUDE_ASM(const s32, "sbtlActAI", nbCallGetIntPara25);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlBtlCommandAI);

INCLUDE_ASM(const s32, "sbtlActAI", nbCallGetIntPara26);

INCLUDE_ASM(const s32, "sbtlActAI", sdfCdImageLoadStart);

INCLUDE_ASM(const s32, "sbtlActAI", sdfCallBtlUseTableDataProc);

INCLUDE_ASM(const s32, "sbtlActAI", sdfCallSetIntReturnValue);

INCLUDE_ASM(const s32, "sbtlActAI", sdfFunc3);

INCLUDE_ASM(const s32, "sbtlActAI", sdfCallGetIntPara);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAISETMOVE);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara8);

INCLUDE_ASM(const s32, "sbtlActAI", sdfCallGetIntPara3);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlGetDevilDataSBtlSub2);

INCLUDE_ASM(const s32, "sbtlActAI", BS14_CHOUSIN_TargetSet);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetFloatPara2);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetFloatPara3);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallSetIntReturnValue);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf18);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf19);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara51);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAIPRINTINT);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallEncGetPointNums5);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlGetNearPosHD);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallEncGetPointNums3);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlBossCERBERModelNodeFadeOutSub);

INCLUDE_ASM(const s32, "sbtlActAI", btlBoss_CERBER_ModelNodeFadeOut);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlModelNodeFadeout);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlFunc3);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlIncNodeColor);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlIncNodeColorFromName);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlWkScrData);

s32 sbtlFunc11(void) {
    sbtlLeaf113();
    return 1;
}

s32 sbtlFunc12(void) {
    sbtlEnableSceneGroupAdvancementSub();
    return 1;
}

s32 CheckLoadData_sbtlScript(void) {
    s32 i;
    struct SBTLUNIT_WORK *unit;

    if (wk_sbtl->mode == 9 || wk_sbtl->mode == 16) {
        return 1;
    }
    for (i = 0; i < wk_sbtl->unitList->number;) {
        unit = wk_sbtl->unitList->pUnitL[i++].pUnit;
        if (unit == NULL) {
            continue;
        }
        if ((unit->mode == 1 || unit->mode == 2) && sbtlSearchReturn_Unit(unit) == 0) {
            return 1;
        }
    }
    return 0;
}

s32 evtIsFadeCompleteAndMessageWindowIdle(void) {
    if (CheckLoadData_sbtlScript() != 0) {
        return 0;
    }
    wk_sbtl->flg2 |= 0x200;
    return 1;
}

s32 sbtlRefSbtl39(void) {
    wk_sbtl->flg2 &= ~0x200;
    return 1;
}

s32 fldEnableSceneGroupAdvancement(void) {
    sbtlEnableSceneGroupAdvancementSub();
    wk_sbtl->flg2 |= 0xE0;
    return 1;
}

s32 sbtlCallGetIntPara52(void) {
    s32 para = scrGetIntPara(0);

    sbtlLeaf113();
    wk_sbtl->flg2 &= ~0x80;
    if (para != 1) {
        wk_sbtl->flg2 &= ~0x40;
        wk_sbtl->flg2 &= ~0x20;
    }
    return 1;
}

s32 sbtlKudaHuuinCheck(void) {
    scrSetIntReturnValue(sbtlCheckPlayerDead() != 0);
    return 1;
}

s32 sbtlCallGetIntPara53(void) {
    if (scrGetIntPara(0) == 1) {
        wk_sbtl->flg2 &= ~0x80;
    } else {
        wk_sbtl->flg2 |= 0x80;
    }
    return 1;
}

s32 sbtlCallGetIntPara54(void) {
    if (scrGetIntPara(0) == 1) {
        switch (wk_sbtl->field678) {
        case 0:
            if (!CheckLoadData_sbtlScript()) {
                wk_sbtl->flg2 |= 0x200;
                wk_sbtl->field678++;
            }
            break;
        case 1:
            if (sbtlLoadEffectData_ID(SKLID_BS14_APPEAR_C)) {
                sbtlLoadEffectData_ID(SKLID_BS14_DIVE_C);
                wk_sbtl->field678++;
            }
            break;
        case 2:
            if (sbtlLoadEffectData_ID(SKLID_BS14_DIVE_C)) {
                wk_sbtl->field678++;
                wk_sbtl->flg2 &= ~0x200;
            }
            break;
        }
        if (wk_sbtl->field678 == 3) {
            scrSetIntReturnValue(1);
            return 1;
        }
        scrSetIntReturnValue(0);
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlSetChkskl);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara9);

INCLUDE_ASM(const s32, "sbtlActAI", nbCommand_AI_GET_FUREA);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara10);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara11);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara12);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara13);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara14);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallClrAction);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallSetIntReturnValue2);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara15);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara16);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlFunc4);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallSetIntReturnValue3);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara17);

INCLUDE_RODATA(const s32, "sbtlActAI", sbtlWkScrDataTable);

INCLUDE_RODATA(const s32, "sbtlActAI", sbtlStrBtMap16Pillar02);

INCLUDE_RODATA(const s32, "sbtlActAI", sbtlStrBtMap16Pillar03);

INCLUDE_RODATA(const s32, "sbtlActAI", sbtlStrBtMap16Pillar01);

INCLUDE_RODATA(const s32, "sbtlActAI", sbtlWkScrDataTable2);

INCLUDE_RODATA(const s32, "sbtlActAI", jtbl_004C0330);

INCLUDE_RODATA(const s32, "sbtlActAI", jtbl_004C0490);

INCLUDE_RODATA(const s32, "sbtlActAI", jtbl_004C04B0);

INCLUDE_RODATA(const s32, "sbtlActAI", jtbl_004C04D0);

INCLUDE_RODATA(const s32, "sbtlActAI", jtbl_004C04F0);

INCLUDE_RODATA(const s32, "sbtlActAI", jtbl_004C0530);

INCLUDE_RODATA(const s32, "sbtlActAI", jtbl_004C0560);

INCLUDE_RODATA(const s32, "sbtlActAI", jtbl_004C0580);

INCLUDE_RODATA(const s32, "sbtlActAI", jtbl_004C05C0);

INCLUDE_RODATA(const s32, "sbtlActAI", jtbl_004C05E0);

INCLUDE_RODATA(const s32, "sbtlActAI", jtbl_004C0610);

INCLUDE_RODATA(const s32, "sbtlActAI", jtbl_004C0630);

INCLUDE_RODATA(const s32, "sbtlActAI", jtbl_004C0650);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlWKNULL);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara18);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlBgnWaitALL);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara19);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlEndWaitALL);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallSetIntReturnValue4);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara20);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara21);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallSetIntReturnValue5);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlFunc5);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara22);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf20);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf21);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallSetIntReturnValue6);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf22);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetAddMag_Setsuma_D);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara23);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara24);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara25);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlIsMoveEnd);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallSetIntReturnValue7);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara26);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara27);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara28);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallSetIntReturnValue8);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara29);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara30);

void sbtlACTAIFLG_Clr(struct SBTLUNIT_WORK *wk)
{
    wk->model->flags &= ~0x201;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCreate);

s32 sbtlDelete(SBTLUNIT_MODEL *model)
{
    if (model != NULL) {
        sdfMemFreeDirect(model->handle);
    }
    return 1;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlAAIProc);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlGetAITBL);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf23);

s32 sbtlSetSkillData_AI(struct SBTLUNIT_WORK *wk, SKLID skillid) {
    wk->skillid = skillid;
    sbtlSetAIMode(wk);
    wk->model->stat = 1;
    sbtlCheckTargetEquation(wk);
    wk->aiwk->stat = 2;
    wk->model->mode = 14;
    return 1;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCheckEquationProc);

s32 sbtlSetChkskl(struct SBTLUNIT_WORK *wk, s32 sklid) {
    return (wk->model->chkskl[sklid / 32] & (1 << sklid)) != 0;
}

INCLUDE_ASM(const s32, "sbtlActAI", SetChkskl);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf65);

s32 sbtlSetUseskl(struct SBTLUNIT_WORK *wk, s32 sklid) {
    return (wk->model->useskl[sklid / 32] & (1 << sklid)) != 0;
}

INCLUDE_ASM(const s32, "sbtlActAI", SetUseskl);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlResetUseskl);

s32 sbtlCheckMeshiyose(struct SBTLUNIT_WORK *wk) {
    s32 ret = 0;

    if (sbtlCheckGenerator_D() != 0) {
        ret = wk_sbtl->field424 < 6;
    }
    return ret;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCheckEquationProc);

s32 sbtlBtlCheckEquationProcSub(s16 code) {
    return code != 0x16;
}

/* NON_MATCHING: 10 of 18 words match.  */
#ifdef NON_MATCHING
u8 sbtlCallBtlGetBadStatus6(struct SBTLUNIT_WORK *wk, u8 flg) {
    s32 ret = flg;

    if (sbtlGetBadStatus(&wk->unit) == BST_MAHUU) {
        ret = 1;
        if (!(flg & 2)) {
            ret = 2;
        }
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlGetBadStatus6);
#endif

/* NON_MATCHING: 4 of 21 words match.  */
#ifdef NON_MATCHING
struct SBTLUNIT_WORK *sbtlCheckCharmTarget(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_WORK *target;

    if (sbtlGetBadStatus(&wk->unit) == BST_MAHUU) {
        target = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
        if (target != NULL) {
            wk = target;
        }
    }
    return wk;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlCheckCharmTarget);
#endif

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf66);

s32 sbtlCallBtlCheckCharmTarget(struct SBTLUNIT_WORK *wk, s32 per, s32 idx) {
    struct SBTLUNIT_WORK *unit = sbtlCheckCharmTarget(wk);

    if (per * 10 >= unit->unit.now.hp * 1000 / unit->unit.now.maxhp) {
        wk->model->ai_target[idx] = wk->field1A0;
        return 1;
    }
    return 0;
}

s32 sbtlCallBtlCreateSideList2(struct SBTLUNIT_WORK *wk, s32 r, s32 idx)
{
    struct SBTLUNIT_LISTWORK *list;
    struct SBTLUNIT_WORK *unit;
    s32 i;
    s32 hit;
    f32 radius;

    list = sbtlCreateSideList(wk->field414, 1);
    radius = r;
    for (i = 0; i < list->count; i++) {
        unit = list->pUnitL[i].pUnit;
        if (sbtlCheckCrossCircle(&wk->pos, radius, &unit->pos, unit->model->field11C)) {
            wk->model->ai_target[idx] = unit->field1A0;
            break;
        }
    }
    hit = i < list->count;
    sbtlDeleteUnitList(list);
    return hit;
}

s32 sbtlCallBtlCheckCharmTarget2(struct SBTLUNIT_WORK *wk, s32 per, s32 idx) {
    struct SBTLUNIT_WORK *unit = sbtlCheckCharmTarget(wk);

    if (unit->unit.now.hp * 1000 / unit->unit.now.maxhp >= per * 10) {
        wk->model->ai_target[idx] = wk->field1A0;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlSearchUnitID21);

s32 sbtlCallBtlCreateSideListSBtlUnit5(struct SBTLUNIT_WORK *wk, s32 arg, s32 idx) {
    struct SBTLUNIT_LISTWORK *list;
    struct SBTLUNIT_WORK *unit;
    s32 i;
    s32 hit;

    list = sbtlCreateSideList_sbtlUnit(sbtlCallBtlGetBadStatus6(wk, wk->field414), 0);
    for (i = 0; i < list->count; i++) {
        unit = list->pUnitL[i].pUnit;
        if (unit->unit.now.hp * 1000 / unit->unit.now.maxhp <= arg * 10) {
            wk->model->ai_target[idx] = unit->field1A0;
            break;
        }
    }
    hit = i < list->count;
    sbtlDeleteUnitList(list);
    return hit;
}

s32 sbtlCallBtlCheckCharmTarget3(struct SBTLUNIT_WORK *wk, s32 per, s32 idx) {
    struct SBTLUNIT_WORK *unit = sbtlCheckCharmTarget(wk);

    if (unit->unit.now.hp * 100 < unit->unit.now.maxhp * per) {
        wk->model->ai_target[idx] = wk->field1A0;
        return 1;
    }
    return 0;
}

s32 sbtlCallBtlCreateSideList3(struct SBTLUNIT_WORK *wk, s32 r, s32 idx)
{
    struct SBTLUNIT_LISTWORK *list;
    struct SBTLUNIT_WORK *unit;
    s32 i;
    s32 hit;
    f32 radius;

    list = sbtlCreateSideList(wk->field414, 1);
    radius = r;
    for (i = 0; i < list->count; i++) {
        unit = list->pUnitL[i].pUnit;
        if (!sbtlCheckCrossCircle(&wk->pos, radius, &unit->pos, unit->model->field11C)) {
            wk->model->ai_target[idx] = unit->field1A0;
            break;
        }
    }
    hit = i < list->count;
    sbtlDeleteUnitList(list);
    return hit;
}

s32 sbtlCallBtlCreateSideListSBtlUnit6(struct SBTLUNIT_WORK *wk, s32 arg, s32 idx) {
    struct SBTLUNIT_LISTWORK *list;
    struct SBTLUNIT_WORK *unit;
    s32 i;
    s32 hit;

    list = sbtlCreateSideList_sbtlUnit(sbtlCallBtlGetBadStatus6(wk, wk->field414), 0);
    for (i = 0; i < list->count; i++) {
        unit = list->pUnitL[i].pUnit;
        if (unit->unit.badStatus & arg) {
            wk->model->ai_target[idx] = unit->field1A0;
            break;
        }
    }
    hit = i < list->count;
    sbtlDeleteUnitList(list);
    return hit;
}

s32 sbtlLeaf67(struct SBTLUNIT_WORK *wk, s32 cnt, s32 idx) {
    if (wk->aiwk->syscnt_stop >= cnt) {
        wk->model->ai_target[idx] = wk->field1A0;
        return 1;
    }
    return 0;
}

s32 sbtlCallBtlSetChkskl2(struct SBTLUNIT_WORK *wk, s32 arg, s32 idx) {
    s32 ret = 0;
    s32 i;

    switch (arg & 0xF000) {
    case 0:
        ret = sbtlSetChkskl(wk, arg & 0xFFF) != 0;
        break;
    case 0x8000:
        for (i = SKLID_NONE; i < SBTL_AI_SKILL_MAX; i++) {
            if ((sbtlGetSkilltbl(i)->attr & arg) && sbtlSetChkskl(wk, i)) {
                ret = 1;
                break;
            }
        }
        break;
    case 0x4000:
        for (i = SKLID_NONE; i < SBTL_AI_SKILL_MAX; i++) {
            if ((sbtlGetSkilltbl(i)->flg & arg) && sbtlSetChkskl(wk, i)) {
                ret = 1;
                break;
            }
        }
        break;
    }
    return ret;
}

s32 sbtlCallBtlSetChkskl3(struct SBTLUNIT_WORK *wk, s32 arg, s32 idx) {
    s32 ret = 0;
    s32 i;

    switch (arg & 0xF000) {
    case 0:
        i = arg & 0xFFF;
        if (sbtlSetChkskl(wk, i)) {
            sbtlLeaf65(wk, i);
            ret = 1;
        }
        break;
    case 0x8000:
        for (i = SKLID_NONE; i < SBTL_AI_SKILL_MAX; i++) {
            if ((sbtlGetSkilltbl(i)->attr & arg) && sbtlSetChkskl(wk, i)) {
                sbtlLeaf65(wk, i);
                ret = 1;
                break;
            }
        }
        break;
    case 0x4000:
        for (i = SKLID_NONE; i < SBTL_AI_SKILL_MAX; i++) {
            if ((sbtlGetSkilltbl(i)->flg & arg) && sbtlSetChkskl(wk, i)) {
                sbtlLeaf65(wk, i);
                ret = 1;
                break;
            }
        }
        break;
    }
    return ret;
}

s32 sbtlCallBtlSetUseskl(struct SBTLUNIT_WORK *wk, s32 arg, s32 idx) {
    s32 ret = 0;
    s32 i;

    switch (arg & 0xF000) {
    case 0:
        ret = sbtlSetUseskl(wk, arg & 0xFFF) != 0;
        break;
    case 0x8000:
        for (i = SKLID_NONE; i < SBTL_AI_SKILL_MAX; i++) {
            if ((sbtlGetSkilltbl(i)->attr & arg) && sbtlSetUseskl(wk, i)) {
                ret = 1;
                break;
            }
        }
        break;
    case 0x4000:
        for (i = SKLID_NONE; i < SBTL_AI_SKILL_MAX; i++) {
            if ((sbtlGetSkilltbl(i)->flg & arg) && sbtlSetUseskl(wk, i)) {
                ret = 1;
                break;
            }
        }
        break;
    }
    return ret;
}

s32 sbtlCallBtlSetUseskl2(struct SBTLUNIT_WORK *wk, s32 arg, s32 idx) {
    s32 ret = 0;
    s32 i;

    switch (arg & 0xF000) {
    case 0:
        i = arg & 0xFFF;
        if (sbtlSetUseskl(wk, i)) {
            sbtlResetUseskl(wk, i);
            ret = 1;
        }
        break;
    case 0x8000:
        for (i = SKLID_NONE; i < SBTL_AI_SKILL_MAX; i++) {
            if ((sbtlGetSkilltbl(i)->attr & arg) && sbtlSetUseskl(wk, i)) {
                sbtlResetUseskl(wk, i);
                ret = 1;
                break;
            }
        }
        break;
    case 0x4000:
        for (i = SKLID_NONE; i < SBTL_AI_SKILL_MAX; i++) {
            if ((sbtlGetSkilltbl(i)->flg & arg) && sbtlSetUseskl(wk, i)) {
                sbtlResetUseskl(wk, i);
                ret = 1;
                break;
            }
        }
        break;
    }
    return ret;
}

s32 sbtlCallCheckLiveTarget(struct SBTLUNIT_WORK *wk, s32 arg, s32 idx) {
    struct SBTLUNIT_WORK *unit;
    s32 i;

    for (i = 0; i < wk_sbtl->unitList->number;) {
        unit = wk_sbtl->unitList->pUnitL[i++].pUnit;
        if (unit != NULL && CheckLiveTarget(unit) && (unit->field414 & wk->field414 & 0x3F) &&
            unit->GroupID == wk->GroupID && sbtlCallBtlSetUseskl(unit, arg, idx) == 1) {
            return 1;
        }
    }
    return 0;
}

s32 sbtlCallCheckLiveTarget2(struct SBTLUNIT_WORK *wk, s32 arg, s32 idx) {
    struct SBTLUNIT_WORK *unit;
    s32 i;

    for (i = 0; i < wk_sbtl->unitList->number;) {
        unit = wk_sbtl->unitList->pUnitL[i++].pUnit;
        if (unit != NULL && CheckLiveTarget(unit) && (unit->field414 & wk->field414 & 0x3F) &&
            unit->GroupID == wk->GroupID && sbtlCallBtlSetUseskl2(unit, arg, idx) == 1) {
            return 1;
        }
    }
    return 0;
}

s32 sbtlGetBgmID(s32 encountid, u16 bit) {
    return sbtlCheckMoon(bit) != 0;
}

s32 sbtlCallBtlCreateSideList4(struct SBTLUNIT_WORK *wk, s32 arg, s32 idx) {
    struct SBTLUNIT_LISTWORK *list;
    struct SBTLUNIT_WORK *unit;
    s32 i;
    s32 hit;

    list = sbtlCreateSideList(sbtlCallBtlGetBadStatus6(wk, wk->field414), 1);
    for (i = 0; i < list->count; i++) {
        unit = list->pUnitL[i].pUnit;
        if (unit->unit.badStatus & arg) {
            wk->model->ai_target[idx] = unit->field1A0;
            break;
        }
    }
    hit = i < list->count;
    sbtlDeleteUnitList(list);
    return hit;
}

s32 sbtlCallBtlCreateSideListSBtlUnit(struct SBTLUNIT_WORK *wk, s32 arg, s32 idx) {
    struct SBTLUNIT_LISTWORK *list;
    struct SBTLUNIT_WORK *unit;
    s32 i;
    s32 hit;
    s32 count;

    list = sbtlCreateSideList_sbtlUnit(sbtlCallBtlGetBadStatus6(wk, wk->field414), 0);
    for (i = 0; i < list->count; i++) {
        unit = list->pUnitL[i].pUnit;
        count = 0;
        if (sbtlCheckKajaAnyEx(unit, &count) && count >= arg) {
            wk->model->ai_target[idx] = unit->field1A0;
            break;
        }
    }
    hit = i < list->count;
    sbtlDeleteUnitList(list);
    return hit;
}

s32 sbtlCallBtlCreateSideList(struct SBTLUNIT_WORK *wk, s32 arg, s32 idx) {
    struct SBTLUNIT_LISTWORK *list;
    struct SBTLUNIT_WORK *unit;
    s32 i;
    s32 hit;
    s32 count;

    list = sbtlCreateSideList(sbtlCallBtlGetBadStatus6(wk, wk->field414), 1);
    for (i = 0; i < list->count; i++) {
        unit = list->pUnitL[i].pUnit;
        count = 0;
        if (sbtlCheckKajaAnyEx(unit, &count) && count >= arg) {
            wk->model->ai_target[idx] = unit->field1A0;
            break;
        }
    }
    hit = i < list->count;
    sbtlDeleteUnitList(list);
    return hit;
}

s32 sbtlCallBtlCreateSideListSBtlUnit2(struct SBTLUNIT_WORK *wk, s32 arg, s32 idx) {
    struct SBTLUNIT_LISTWORK *list;
    struct SBTLUNIT_WORK *unit;
    s32 i;
    s32 hit;

    list = sbtlCreateSideList_sbtlUnit(sbtlCallBtlGetBadStatus6(wk, wk->field414), 0);
    for (i = 0; i < list->count; i++) {
        unit = list->pUnitL[i].pUnit;
        if (sbtlCheckEnableUseKaja(unit, arg) > 0) {
            wk->model->ai_target[idx] = unit->field1A0;
            break;
        }
    }
    hit = i < list->count;
    sbtlDeleteUnitList(list);
    return hit;
}

s32 sbtlCallBtlCreateSideListSBtlUnit7(struct SBTLUNIT_WORK *wk, s32 arg, s32 idx) {
    struct SBTLUNIT_LISTWORK *list;
    struct SBTLUNIT_WORK *unit;
    s32 i;
    s32 hit;

    list = sbtlCreateSideList_sbtlUnit(sbtlCallBtlGetBadStatus6(wk, wk->field414), 0);
    for (i = 0; i < list->count; i++) {
        unit = list->pUnitL[i].pUnit;
        if (unit->field4AC) {
            wk->model->ai_target[idx] = unit->field1A0;
            break;
        }
    }
    hit = i < list->count;
    sbtlDeleteUnitList(list);
    return hit;
}

s32 sbtlCallBtlCreateSideList5(struct SBTLUNIT_WORK *wk, s32 arg, s32 idx) {
    struct SBTLUNIT_LISTWORK *list;
    struct SBTLUNIT_WORK *unit;
    s32 i;
    s32 hit;

    list = sbtlCreateSideList(sbtlCallBtlGetBadStatus6(wk, wk->field414), 1);
    for (i = 0; i < list->count; i++) {
        unit = list->pUnitL[i].pUnit;
        if (unit->field4AC) {
            wk->model->ai_target[idx] = unit->field1A0;
            break;
        }
    }
    hit = i < list->count;
    sbtlDeleteUnitList(list);
    return hit;
}

s32 sbtlCallBtlCreateSideListSBtlUnit8(struct SBTLUNIT_WORK *wk, s32 arg, s32 idx) {
    struct SBTLUNIT_LISTWORK *list;
    struct SBTLUNIT_WORK *unit;
    s32 i;
    s32 hit;

    list = sbtlCreateSideList_sbtlUnit(sbtlCallBtlGetBadStatus6(wk, wk->field414), 0);
    for (i = 0; i < list->count; i++) {
        unit = list->pUnitL[i].pUnit;
        if (!unit->field4AC) {
            wk->model->ai_target[idx] = unit->field1A0;
            break;
        }
    }
    hit = i < list->count;
    sbtlDeleteUnitList(list);
    return hit;
}

s32 sbtlGetUseTable_Target(struct SBTLUNIT_WORK *wk) {
    return sbtlCheckMeshiyose(wk) != 0;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf24);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlSearchUnitID5);

INCLUDE_ASM(const s32, "sbtlActAI", AICHK_HPP);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf25);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf26);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlRefSbtl14);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallEffectFldTypeIsSpecial);

INCLUDE_RODATA(const s32, "sbtlActAI", sbtlCallGetIntPara23Table);

INCLUDE_ASM(const s32, "sbtlActAI", dds3EffectFldTypeIsSpecial);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetAIData);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlGetUseTable_Target_sbtlActAI);

s32 sbtlCheckEnemyTarget(struct SBTLUNIT_WORK *wk, s32 id) {
    struct SBTLUNIT_WORK *target = sbtlSearchUnit_ID(wk_sbtl->unitList, id);
    BST bst;
    s32 ret = -1;

    if (target != NULL && (target->field414 & 1) && (target->field414 & 0x80) && (target->flg2 & 0x100)) {
        return -1;
    }
    bst = sbtlGetBadStatus(&wk->unit);
    if (bst == BST_BOUDO || bst == BST_MAHUU) {
        if (sbtlGetSkilltbl(wk->skillid)->area & 0x4000) {
            return sbtlGetUseTable_Target_sbtlActAI(wk, 8, 0);
        }
        return sbtlGetUseTable_Target_sbtlActAI(wk, 4, 0);
    }
    if (wk->flg & 0x100000) {
        if (target == NULL) {
            return wk_sbtl->targetUnitID;
        }
        return id;
    }
    if (target != NULL) {
        ret = id;
    }
    return ret;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlGetSmnNo);

s32 sbtlResetAIPattern(struct SBTLUNIT_WORK *wk, u32 flg) {
    SBTLUNIT_MODEL *model = wk->model;

    model->dataflg = 0;
    model->datamode = flg;
    wk->model->data_p[0] = 0;
    wk->model->data_p[1] = 0;
    return 1;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlEnemyAI);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf68);

s32 sbtlEAIInit_None(struct SBTLUNIT_WORK *wk) {
    s16 stat = 1;
    SBTLUNIT_MODEL *model;

    if (sbtlCheckScriptData(wk->unit.devilId)) {
        wk->aiwk->stat = stat;
        wk->model->mode = 12;
        wk->model->stat = stat;
        return 0;
    }
    model = wk->model;
    if (model->flags & 0x400) {
        sbtlSetNextPathData(wk);
        return 0;
    }
    model->mode = SBTL_ACTAIMODE_WAIT;
    sbtlSetNormalAction(wk, SBTL_ACTAIMODE_WAIT, 0, -1, 0);
    return 1;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlEAIInit_Summon);

/* NON_MATCHING: 26 of 28 words match.  */
#ifdef NON_MATCHING
s32 sbtlEAIInit_BtlInit(struct SBTLUNIT_WORK *wk) {
    sdfVector4_t zero;
    struct SBTLUNIT_MODEL *model;

    memset(&zero, 0, sizeof(zero));
    wk->model->flags = (wk->model->flags | 4) & ~0x21B;
    SDF_VU0_LOADVECTOR(&zero);
    SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
    model = wk->model;
    model->cmpframe = 60;
    model->mode = 13;
    model->frame = 0;
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlEAIInit_BtlInit);
#endif

s32 sbtlEAIInit_BtlWait(struct SBTLUNIT_WORK *wk) {
    sdfVector4_t zero;
    struct SBTLUNIT_MODEL *model;

    memset(&zero, 0, sizeof(zero));
    wk->model->flags = (wk->model->flags | 5) & ~0x21A;
    SDF_VU0_LOADVECTOR(&zero);
    SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
    model = wk->model;
    model->stat = 2;
    model->mode = 11;
    model->cmpframe = 60;
    model->frame = 0;
    return 1;
}

s32 sbtlEAIProc_Dummy(struct SBTLUNIT_WORK *wk) {
    wk->model->stat = 2;
    return 1;
}

s32 sbtlEAIProc_Wait(struct SBTLUNIT_WORK *wk) {
    if (wk->model->cmpframe <= wk->model->frame) {
        sbtlResetAAI(wk);
        sbtlSetAIWkStat(wk);
    }
    return 1;
}

s32 sbtlEAIProc_Movef(struct SBTLUNIT_WORK *wk) {
    SBTLUNIT_MODEL *model = wk->model;

    if (model->cmpframe <= model->frame ||
        ((model->flags & 0x10) && sbtlCheckCrossCircle(&wk->pos, model->field120, &model->vec00, 2.0f))) {
        sbtlResetAAI(wk);
        sbtlSetAIWkStat(wk);
    }
    return 1;
}

s32 sbtlEAIProc_BtlWait(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_MODEL *model = wk->model;
    struct SBTLUNIT_MODEL *next;

    if (model->cmpframe <= model->frame) {
        model->flags &= ~1;
        sbtlResetAAI(wk);
        next = wk->model;
        next->stat = 1;
        next->mode = 0;
        sbtlSetMotion_sbtlUnit(wk, 0, 4, 0, 0);
    }
    return 1;
}

s32 sbtlEAIProc_Guard(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_MODEL *model = wk->model;

    wk->flg |= 0x100;
    if (model->cmpframe <= model->frame) {
        sbtlResetAAI(wk);
        sbtlSetAIWkStat(wk);
        wk->flg &= ~0x100;
    }
    return 1;
}

/* NON_MATCHING: 14 of 58 words match.  */
#ifdef NON_MATCHING
s32 sbtlEAIProc_Ctact(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_MODEL *model = wk->model;
    struct SBTLUNIT_WORK *unit;
    s32 ret;

    if (model->cmpframe <= model->frame) {
        model->mode = 0;
        model->stat = 1;
        return 1;
    }
    unit = sbtlSearchUnit_ID(wk_sbtl->unitList, sbtlSetNoMP(wk));
    if (unit == NULL) {
        goto lost;
    }
    if (!sbtlCheckExist(unit)) {
        goto lost;
    }
    ret = 1;
    if (sbtlCheckCrossCircle(&wk->pos, wk->model->field120, &unit->pos, unit->model->field120)) {
        model = wk->model;
        if (model->field118) {
            sbtlSetSearchEnemyMode(unit, model->field104);
            model = wk->model;
        }
        model->mode = 0;
        model->stat = 1;
        return 1;
    }
    return ret;

lost:
    wk->model->stat = 1;
    wk->model->mode = 0;
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlEAIProc_Ctact);
#endif

s32 sbtlEAIProc_Search(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_MODEL *model = wk->model;
    s32 targetid[4];

    if (model->cmpframe <= model->frame) {
        model->mode = 0;
        model->stat = 1;
        return 1;
    }
    if (sbtlCheckSearchEnemy(wk, targetid, 0) != 0) {
        sbtlSetSearchEnemyMode(wk, targetid[0]);
        wk->model->stat = 1;
    }
    return 1;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlEAIProc_Chase);

s32 sbtlEAIProc_Escape(struct SBTLUNIT_WORK *wk) {
    return 1;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlEAIProc_Summonw);

s32 sbtlEAIProc_BtlAI(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_WORK *pUnit;
    s32 i = 0;
    s32 actMax = wk->actMax;

    while (i < wk_sbtl->unitList->number) {
        pUnit = wk_sbtl->unitList->pUnitL[i++].pUnit;
        if (pUnit != NULL && sbtlCheckDamageTarget(pUnit) && (pUnit->field414 & 1) &&
            sbtlCheckCrossCircle(&wk->pos, wk->model->field130, &pUnit->pos, pUnit->model->field11C)) {
            actMax = wk->actMax >> 4;
            break;
        }
    }
    if (wk->actCounter >= actMax) {
        wk->actCounter = 0;
        if (!sbtlCheckScriptData(wk->unit.devilId)) {
            wk->aiwk->stat = 1;
        }
    } else {
        wk->actCounter++;
        wk->actCounter = (wk->actCounter <= 0) ? 0 : (wk->actCounter < actMax) ? wk->actCounter : actMax;
    }
    return 1;
}

s32 sbtlSetSearchEnemyMode(struct SBTLUNIT_WORK *wk, s32 targetid) {
    wk->model->field118 = 1;
    sbtlSetGroupAction(wk, targetid, 12, 0);
    sbtlResetAIPattern(wk, 0);
    return 1;
}

/* NON_MATCHING: 29 of 192 words match.  */
#ifdef NON_MATCHING
s32 sbtlSetGroupAction(struct SBTLUNIT_WORK *wk, s32 targetid, u32 mode, s8 flg) {
    u16 tbl[11] = {
        SBTL_ACTAIMODE_WAIT,  SBTL_ACTAIMODE_WAIT,  SBTL_ACTAIMODE_MOVEF,  SBTL_ACTAIMODE_GUARD,
        SBTL_ACTAIMODE_CTACT, SBTL_ACTAIMODE_SEARCH, SBTL_ACTAIMODE_CHASE, SBTL_ACTAIMODE_ESCAPE,
        SBTL_ACTAIMODE_RESMNR, SBTL_ACTAIMODE_SUMMON, SBTL_ACTAIMODE_SUMMONW,
    };
    struct SBTLUNIT_WORK *unit;
    struct SBTLUNIT_MODEL *model;
    s32 i;
    s32 set;

    if (wk->field414 & 1) {
        return 1;
    }
    if (!(wk->field414 & 7)) {
        return 1;
    }
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        unit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (unit == NULL || unit->field1A0 == wk->field1A0 || !sbtlCheckExist(unit)) {
            continue;
        }
        model = unit->model;
        set = 0;
        if ((model->flags & 0x100) && wk->model->field0FC == model->field0FC) {
            set = ((wk->field414 ^ unit->field414) & 7) == 0;
        }
        if (flg == 1 && ((wk->field414 ^ unit->field414) & 7) == 0) {
            set = 1;
        }
        if (wk->field1A0 == model->field0FC) {
            set = 1;
        }
        if (sbtlCheckScriptData(unit->unit.devilId)) {
            set = 0;
        }
        if (unit->flg & 0x800) {
            set = 0;
        }
        if (!(unit->field414 & 7)) {
            set = 0;
        }
        if (set == 1) {
            if (mode < 12) {
                sbtlSetNormalAction(unit, tbl[mode], 0, -1, 0);
            } else if (unit->model->mode >= 12 && unit->model->mode <= 14) {
                unit->model->field104 = targetid;
            } else {
                model = unit->model;
                model->stat = 1;
                model->field104 = targetid;
                model->mode = mode;
                model->flags = (model->flags | 0x44) & ~0x23B;
            }
        }
    }
    if (sbtlCheckScriptData(wk->unit.devilId)) {
        return 0;
    }
    if (wk->flg & 0x800) {
        return 0;
    }
    model = wk->model;
    if (model->mode < 12 && mode < 12) {
        sbtlSetNormalAction(wk, tbl[mode], 0, -1, 0);
    } else if (model->mode >= 12 && model->mode <= 14) {
        model->field104 = targetid;
    } else {
        model->field104 = targetid;
        model->mode = mode;
        model->stat = 1;
        model->flags = (model->flags | 0x44) & ~0x23B;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetGroupAction);
#endif

INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetNoMP);

s32 SetUnitHitApproach(struct SBTLUNIT_WORK *wk) {
    u32 hit;

    if (wk_sbtl->battleMode == 3) {
        return 1;
    }
    hit = wk->vstatus & 0x4000;
    return hit == 0;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlMVSupport);

s32 PosOut(struct SBTLUNIT_WORK *wk) {
    s32 ret = 0;
    u32 flg;

    if (wk_sbtl->battleMode == 3) {
        if (wk->field414 & 1) {
            ret = 1;
        }
    } else {
        flg = wk->field414 & 0x80;
        ret = flg != 0;
    }
    return ret;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlBtlMVSupportSub);

s32 sbtlBtlMVSupportSub2(struct SBTLUNIT_WORK *wk, struct SBTLUNIT_WORK *wklock) {
    if ((wk->flg & 0x800) || (wklock->flg & 0x800)) {
        return 1;
    }
    if ((wk->flg & 0x1000) || (wklock->flg & 0x1000)) {
        return 1;
    }
    return 0;
}

s32 sbtlCheckHITCheck_Char(struct SBTLUNIT_WORK *wk) {
    s32 ret = 1;

    switch (wk->unit.devilId) {
    case 659:
    case 664:
    case 665:
    case 666:
        ret = 0;
        break;
    }
    return ret;
}

s32 sbtlPrivAreaCheck(struct SBTLUNIT_WORK *wk, s32 flg) {
    sdfVector4_t pos;
    struct SBTLUNIT_WORK *pUnit;
    s32 ret;
    f32 deg;
    s32 i;

    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        pUnit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (pUnit == NULL || pUnit->field1A0 == wk->field1A0) {
            continue;
        }
        if (!sbtlBtlMVSupportSub(wk, pUnit) || !sbtlCheckHITCheck_Char(wk) || !sbtlCheckHITCheck_Char(pUnit)) {
            continue;
        }
        if (!sbtlCheckExist2_sbtlUnit(pUnit) || sbtlBtlMVSupportSub2(wk, pUnit)) {
            continue;
        }
        if ((wk->flg & 1) || (pUnit->flg & 1) || (wk->vstatus & 0x200) || (pUnit->vstatus & 0x200) || flg != 1) {
            continue;
        }
        if (sbtlCheckCrossCircle(&wk->pos, wk->model->field11C, &pUnit->pos, pUnit->model->field11C)) {
            ret = sbtlSetPushPos(wk, wk->model->field11C, pUnit, pUnit->model->field11C,
                                 sbtlFree(pUnit->pos.x, pUnit->pos.z));
            /* sbtlSetPushPos leaves the pushed position in vf10 */
            SDF_VU0_STOREVECTOR(&pUnit->pos);
            pUnit->mv_spd.z = 0.0f;
            if (!ret) {
                deg = sbtlGetChorikiDeg(&wk->pos, &wk->rot, &pUnit->pos, 1.0f);
                if (!sbtlGetMovePos_MapHit_2(wk, pUnit, &pos, wk->model->field11C + pUnit->model->field11C + 2.0f, deg,
                                             1) &&
                    !sbtlGetMovePos_MapHit_2(wk, pUnit, &pos,
                                             wk->model->field11C + pUnit->model->field11C + pUnit->model->field11C, deg,
                                             1)) {
                    SDF_VU0_LOADVECTOR(&pUnit->pos);
                    SDF_VU0_STOREVECTOR(&pos);
                }
                SDF_VU0_LOADVECTOR(&pos);
                SDF_VU0_STOREVECTOR(&pUnit->pos);
            }
        }
    }
    return 1;
}

s32 sbtlCheckSearchEnemy(struct SBTLUNIT_WORK *wk, s32 *targetid, s8 mode) {
    struct SBTLUNIT_WORK *unit;
    s32 i;

    if (mode != 0 && GetPercentage() < 91) {
        return 0;
    }
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        unit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (unit == NULL || wk->field1A0 == unit->field1A0 || !sbtlCheckExist(unit)) {
            continue;
        }
        if ((wk->field414 & 7) != (unit->field414 & 7) && (unit->field414 & 7) &&
            sbtlCheckCrossCircle(&wk->pos, wk->model->field128, &unit->pos, unit->model->field11C)) {
            *targetid = unit->field1A0;
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf69);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf70);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf71);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf72);

void sbtlSetNoMP_sbtlActAI(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_MODEL *model = wk->model;

    if (model->nonempflg == 0 && (wk->field414 & 2)) {
        model->nonempflg = 1;
        sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_NOTMP, wk->skillid);
        sbtlResetAIPattern(wk, 0);
    }
}

INCLUDE_RODATA(const s32, "sbtlActAI", sbtlSetGroupActionTable);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetNormalAction);

INCLUDE_ASM(const s32, "sbtlActAI", GetSenkaiPointSub);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlGetFukinPoint);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlGetEscapePoint);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlGetWarpPoint);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlGetNWarpPoint);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCheckCrossUnit_HIT);

s32 sbtlGetMovePos_MapHit(struct SBTLUNIT_WORK *wk, struct SBTLUNIT_WORK *wklock, sdfVector4_t *pos, f32 dis, f32 deg,
                          s32 flg) {
    sdfVector4_t ppos;
    sdfVector4_t dir;
    sdfMatrix4_t mtx;
    s32 i;

    memset(&dir, 0, sizeof(dir));
    dir.z = -1.0f;
    sbtlGetRotateDeg(&wk->rot, deg);
    ToMatrix();
    SDF_VU0_STOREMATRIX(&mtx);
    for (i = 0; i < 12; i++) {
        SDF_VU0_LOADMATRIX(&mtx);
        SDF_VU0_LOADVECTOR(&dir);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_SCALEVECTOR_V0(dis);
        SDF_VU0_LOADVECTOR_B(&wk->pos);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(&ppos);
        if (sbtlCheckMapHit(&wk->pos, &ppos, dis) && (flg != 1 || sbtlCheckCrossUnit_HIT(wklock, &ppos))) {
            break;
        }
        sbtlGetRotateDeg(&wk->rot, deg + (i + 1) * 30);
        ToMatrix();
        SDF_VU0_STOREMATRIX(&mtx);
    }
    if (i < 12) {
        SDF_VU0_LOADVECTOR(&ppos);
        SDF_VU0_STOREVECTOR(pos);
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlGetMovePos_MapHit_Nakama);

/* NON_MATCHING: 148 of 152 words match.  */
#ifdef NON_MATCHING
s32 sbtlGetMovePos_MapHit_2(struct SBTLUNIT_WORK *wk, struct SBTLUNIT_WORK *wklock, sdfVector4_t *pos, f32 dis, f32 deg,
                            s32 flg) {
    sdfVector4_t ppos;
    sdfVector4_t dir = {0.0f, 0.0f, -1.0f, 0.0f};
    sdfMatrix4_t mtx;
    f32 degtbl[24] = {
        15.0f,  -15.0f,  30.0f,  -30.0f,  45.0f,  -45.0f,  60.0f,  -60.0f,  75.0f,  -75.0f,  90.0f,  -90.0f,
        105.0f, -105.0f, 120.0f, -120.0f, 135.0f, -135.0f, 150.0f, -150.0f, 165.0f, -165.0f, 180.0f, 180.0f,
    };
    s32 i;

    sbtlGetRotateDeg(&wk->rot, deg);
    ToMatrix();
    SDF_VU0_STOREMATRIX(&mtx);
    for (i = 0; i < 24; i++) {
        SDF_VU0_LOADMATRIX(&mtx);
        SDF_VU0_LOADVECTOR(&dir);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_SCALEVECTOR_V0(dis);
        SDF_VU0_LOADVECTOR_B(&wk->pos);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(&ppos);
        if (sbtlCheckMapHit(&wk->pos, &ppos, dis) && (flg != 1 || sbtlCheckCrossUnit_HIT(wklock, &ppos))) {
            break;
        }
        sbtlGetRotateDeg(&wk->rot, deg + degtbl[i]);
        ToMatrix();
        SDF_VU0_STOREMATRIX(&mtx);
    }
    if (i < 24) {
        SDF_VU0_LOADVECTOR(&ppos);
        SDF_VU0_STOREVECTOR(pos);
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlGetMovePos_MapHit_2);
#endif

s32 sbtlCheckMapHit(const sdfVector4_t *pos0, const sdfVector4_t *ppos, f32 dis) {
    return sbtlCheckMapHit_Cross(pos0, ppos, dis);
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc);

s32 sbtlAAIInit_None(struct SBTLUNIT_WORK *wk) {
    return 1;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_Free);

void sbtlBtlAAIInitSklSSub(struct SBTLUNIT_WORK *wk) {
    wk->model->stat = 2;
    wk->model->mode = 14;
    wk->model->frame = 0;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_N_Escape);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_N_Kaihi);

s32 sbtlAAIInit_N_Wait(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_MODEL *model;
    f32 max;

    wk->model->mode = 2;
    wk->model->flags = (wk->model->flags | 4) & ~0x21B;
    sbtlSetMotion_sbtlUnit(wk, 0, 0, 0, 0);
    model = wk->model;
    model->stat = 2;
    max = wk->actMax;
    model->frame = 0;
    wk->model->cmpframe = GetIntervalRand(max * 0.8f, max * 1.2f);
    return 1;
}

s32 sbtlAAIInit_N_Sabato(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_MODEL *model = wk->model;

    model->mode = 2;
    model->flags &= ~ACTAIFLGMASK;
    sbtlSetMotion_sbtlUnit(wk, 0, 0, 0, 0);
    wk->model->stat = 2;
    return 1;
}

s32 sbtlAAIInit_N_GardeT(struct SBTLUNIT_WORK *wk) {
    sdfVector4_t vec;
    struct SBTLUNIT_MODEL *model;
    struct SBTLUNIT_WORK *unit;
    struct SBTLDMG_HITWORK *hit;

    wk->model->field100 = -1;
    wk->model->mode = 2;
    wk->model->flags = (wk->model->flags | 0x14) & ~0x20B;
    SDF_VU0_LOADVECTOR(&wk->max_mvspd);
    SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
    wk->model->mode = 14;
    wk->model->frame = 0;
    wk->model->cmpframe = 0;
    unit = sbtlGetWarpUnit_ID(wk);
    sbtlSetWalk(wk, wk->model->field120 + unit->model->field11C);
    model = wk->model;
    model->stat = 2;
    hit = sbtlSearchHit_SerialID(wk_sbtl->hitList, model->h_serialID);
    if (hit != NULL) {
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
        SDF_VU0_LOADVECTOR(&hit->pos);
        SDF_VU0_STOREVECTOR(&vec);
        vec.y = 0.0f;
        SDF_VU0_LOADVECTOR(&vec);
        SDF_VU0_LOADVECTOR_B(&unit->pos);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_NORMALIZE();
        SDF_VU0_SCALEVECTOR(unit->model->field120 + wk->model->field120);
        SDF_VU0_LOADVECTOR_B(&unit->pos);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(&wk->model->vec00);
    }
    return 1;
}

/* NON_MATCHING: 6 of 42 words match.  */
#ifdef NON_MATCHING
s32 sbtlAAIInit_N_Renkei(struct SBTLUNIT_WORK *wk) {
    sdfVector4_t *rot = &wk->rot;
    struct SBTLUNIT_WORK *unit;
    struct SBTLUNIT_MODEL *model;
    f32 deg;

    wk->model->mode = 2;
    wk->model->flags &= ~ACTAIFLGMASK;
    sbtlSetMotion_sbtlUnit(wk, 29, 4, 0, 1);
    unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    deg = sbtlGetChorikiDeg(&wk->pos, rot, &unit->pos, 1.0f);
    SDF_VU0_STOREVECTOR(rot);
    sbtlAddUnitDeg(wk, deg);
    model = wk->model;
    model->stat = 2;
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_N_Renkei);
#endif

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlSearchUnitID7);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlSearchUnitID8);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_N_Return);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_Attack);

/* NON_MATCHING: 39 of 41 words match.  */
#ifdef NON_MATCHING
s32 sbtlAAIInit_BAttack(struct SBTLUNIT_WORK *wk) {
    sdfVector4_t zero;
    struct SBTLUNIT_MODEL *model;

    memset(&zero, 0, sizeof(zero));
    wk->model->flags = (wk->model->flags | 4) & ~0x21B;
    SDF_VU0_LOADVECTOR(&zero);
    SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
    model = wk->model;
    model->cmpframe = 180;
    model->mode = 14;
    model->frame = 0;
    sbtlSetMotion_sbtlUnit(wk, 18, 4, 0, 0);
    sbtlSetAction(wk, 2, 0);
    wk->model->stat = 2;
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_BAttack);
#endif

s32 sbtlAAIInit_Skl_S(struct SBTLUNIT_WORK *wk) {
    sdfVector4_t vec;
    struct SBTLUNIT_MODEL *model;
    SKLID skillid;

    memset(&vec, 0, sizeof(vec));
    skillid = wk->skillid;
    vec.w = 1.0f;
    if ((sbtlGetSkilltbl(skillid)->area & 0x40) && (wk->field414 & 2)) {
        model = wk->model;
        model->flags = (model->flags | 4) & ~0x21B;
    } else {
        wk->model->flags = (wk->model->flags | 0x8004) & ~0x21B;
        sbtlSetAction(wk, 2, 0);
        model = wk->model;
        if (model->target_frame == 0 && model->mahaskl_waitframe == 0) {
            sbtlSetMotion_sbtlUnit(wk, 5, 4, 0, 0);
            model = wk->model;
        }
    }
    SDF_VU0_LOADVECTOR(&vec);
    SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
    model->cmpframe = 240;
    sbtlBtlAAIInitSklSSub(wk);
    return 1;
}

/* NON_MATCHING: 22 of 35 words match.  */
#ifdef NON_MATCHING
s32 sbtlAAIInit_SUMMON(struct SBTLUNIT_WORK *wk) {
    sdfVector4_t zero;
    struct SBTLUNIT_MODEL *model;

    memset(&zero, 0, sizeof(zero));
    model = wk->model;
    model->mode = 2;
    model->flags = (model->flags | 4) & ~0x21B;
    model->flags |= 0x160;
    model->flags &= ~0x80;
    wk->model->field100 = sbtlGetPlayerID();
    SDF_VU0_LOADVECTOR(&zero);
    SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
    model = wk->model;
    model->stat = 2;
    model->frame = 0;
    model->cmpframe = 0;
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_SUMMON);
#endif

/* NON_MATCHING: 39 of 43 words match.  */
#ifdef NON_MATCHING
s32 sbtlAAIInit_N_GuardH(struct SBTLUNIT_WORK *wk) {
    sdfVector4_t vec;
    struct SBTLUNIT_MODEL *model;

    memset(&vec, 0, sizeof(vec));
    model = wk->model;
    vec.w = 1.0f;
    model->mode = 3;
    model->cmpframe = 90;
    model->frame = 0;
    SDF_VU0_LOADVECTOR(&vec);
    SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
    model->flags = (model->flags | 4) & ~0x21B;
    sbtlSetMotion_sbtlUnit(wk, 29, 0, 0, 0);
    sbtlSetAction(wk, 2, 0);
    wk->model->stat = 2;
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_N_GuardH);
#endif

s32 sbtlAAIInit_SAttack(struct SBTLUNIT_WORK *wk) {
    sdfVector4_t vec;
    struct SBTLUNIT_MODEL *model;

    memset(&vec, 0, sizeof(vec));
    vec.z = 32.0f;
    wk->model->flags = (wk->model->flags | 0x205) & ~0x1A;
    SDF_VU0_LOADVECTOR(&vec);
    SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
    model = wk->model;
    model->mode = 14;
    model->frame = 0;
    model->cmpframe = 120;
    sbtlSetMotion_sbtlUnit(wk, 5, 4, 0, 0);
    sbtlSetAction(wk, 2, 0);
    wk->model->stat = 2;
    return 1;
}

s32 sbtlAAIInit_TurnMV(struct SBTLUNIT_WORK *wk) {
    sdfVector4_t zero;
    struct SBTLUNIT_MODEL *model;
    f32 len;

    memset(&zero, 0, sizeof(zero));
    wk->model->flags = (wk->model->flags | 0x14) & ~0x20B;
    SDF_VU0_LOADVECTOR(&zero);
    SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
    SDF_VU0_LOADVECTOR(&wk->model->vec00);
    SDF_VU0_LOADVECTOR_B(&wk->pos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(len);
    model = wk->model;
    model->mode = 14;
    model->frame = 0;
    model->cmpframe = len / wk->max_mvspd.z;
    sbtlSetMotion_sbtlUnit(wk, 18, 4, 0, 0);
    sbtlSetAction(wk, 2, 0);
    wk->model->stat = 2;
    return 1;
}

void sbtlSetAIWkStat(struct SBTLUNIT_WORK *wk) {
    SBTLUNIT_MODEL *model;

    wk->aiwk->stat = 1;
    model = wk->model;
    model->stat = 1;
    model->mode = 0;
}

void sbtlResetAAI(struct SBTLUNIT_WORK *wk) {
    sdfVector4_t spd;

    memset(&spd, 0, sizeof(spd));
    spd.w = 1.0f;
    SDF_VU0_LOADVECTOR(&spd);
    SDF_VU0_STOREVECTOR(&wk->mv_spd);
    SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
    wk->aiwk->stat = 1;
    sbtlForceDelete_SkillFlag_Acter(wk, 0x400);
    if ((wk->field414 & 1) && (wk->flg & 0x40000)) {
        wk_sbtl->flg |= 0x4000;
    }
}

struct SBTLUNIT_WORK *sbtlGetWarpUnit_ID(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_WORK *unit = sbtlSearchUnit_ID(wk_sbtl->unitList, sbtlSetNoMP(wk));

    if (unit == NULL || sbtlCheckExist(unit) == 0) {
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    }
    return unit;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetWalk);

s32 sbtlBtlAAIProcNKaihiSub(struct SBTLUNIT_WORK *wk) {
    return 0;
}

s32 sbtlAAIProc_None(struct SBTLUNIT_WORK *wk) {
    return 1;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_Free);

s32 sbtlAAIProc_N_Wait(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_MODEL *model;

    if ((wk->flg & 0x840000) == 0x840000) {
        return 0;
    }
    model = wk->model;
    model->frame++;
    model->frame = (model->frame <= 0) ? 0 : (model->frame > 0x7FFF) ? 0x7FFF : model->frame;
    if (model->cmpframe != 0 && model->frame >= model->cmpframe) {
        sbtlResetAAI(wk);
        sbtlSetMotion_sbtlUnit(wk, 0, 4, 0, 0);
        wk->actCounter = wk->actMax;
        return 1;
    }
    return 1;
}

s32 sbtlAAIProc_N_Sabato(struct SBTLUNIT_WORK *wk) {
    wk->actCounter++;
    wk->actCounter = (wk->actCounter <= 0) ? 0 : (wk->actCounter < wk->actMax) ? wk->actCounter : wk->actMax;
    return 1;
}

s32 sbtlAAIProc_N_Renkei(struct SBTLUNIT_WORK *wk) {
    wk->actCounter++;
    wk->actCounter = (wk->actCounter <= 0) ? 0 : (wk->actCounter < wk->actMax) ? wk->actCounter : wk->actMax;
    if (sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID)->status < 3) {
        wk->flg &= ~0x4010;
        sbtlResetAAI(wk);
        sbtlSetMotion_sbtlUnit(wk, 0, 4, 0, 0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_N_Comb_E);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_N_Comb_R);

s32 sbtlAAIProc_N_Return(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_WORK *unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);

    if (unit != NULL && sbtlCheckDamageTarget(unit) != 0) {
        sbtlSetReturn(unit);
    }
    return 1;
}

/* NON_MATCHING: 83 of 187 words match.  */
#ifdef NON_MATCHING
s32 sbtlAAIProc_N_Escape(struct SBTLUNIT_WORK *wk) {
    sdfVector4_t mypos;
    sdfVector4_t front;
    sdfVector4_t rot;
    sdfVector4_t zero;
    sdfVector4_t back;
    sdfVector4_t dir;
    struct SBTLUNIT_MODEL *model;

    memset(&zero, 0, sizeof(zero));
    memset(&back, 0, sizeof(back));
    memset(&dir, 0, sizeof(dir));
    dir.z = -1.0f;
    if ((wk->flg & 0x840000) == 0x840000) {
        return 0;
    }
    SDF_VU0_LOADVECTOR(&wk->mv_spdbk);
    SDF_VU0_STOREVECTOR(&wk->mv_spd);
    model = wk->model;
    model->frame++;
    model->frame = (model->frame <= 0) ? 0 : (model->frame > 0x7FFF) ? 0x7FFF : model->frame;
    if (model->cmpframe != 0 && model->frame >= model->cmpframe) {
        sbtlResetAAI(wk);
        sbtlSetMotion_sbtlUnit(wk, 0, 0, 0, 0);
        wk->model->flags &= ~0x10;
        wk->actCounter = wk->actMax;
        return 1;
    }
    switch (model->mode) {
    case 14:
        if (sbtlCheckCrossCircle(&wk->pos, model->field11C, &model->vec00, 30.0f)) {
            wk->model->mode = 2;
            sbtlGetRotateDeg(&wk->rot, 180.0f);
            SDF_VU0_STOREVECTOR(&rot);
            back.z = -wk->model->field11C;
            SDF_VU0_LOADVECTOR_B(&back);
            SDF_VU0_LOADVECTOR(&rot);
            ToMatrix();
            SDF_VU0_APPLYMATRIX_B();
            SDF_VU0_LOADVECTOR(&wk->pos);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(&wk->model->vec00);
        }
        break;
    case 2:
        SDF_VU0_LOADVECTOR(&zero);
        SDF_VU0_STOREVECTOR(&wk->mv_spd);
        SDF_VU0_LOADVECTOR(&wk->pos);
        SDF_VU0_STOREVECTOR(&mypos);
        SDF_VU0_LOADVECTOR(&wk->rot);
        ToMatrix();
        SDF_VU0_LOADVECTOR(&dir);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_NORMALIZE();
        SDF_VU0_SCALEVECTOR(wk->model->field11C);
        SDF_VU0_LOADVECTOR_B(&mypos);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(&front);
        mypos.y = 0.0f;
        front.y = 0.0f;
        if (sbtlGetVerticalLength(&mypos.x, &front.x, &wk->model->vec00.x) <= wk->model->field11C * 0.29999999f &&
            sbtlCheckInner(&mypos, &front, &wk->model->vec00, 0.0f)) {
            sbtlResetAAI(wk);
            sbtlSetMotion_sbtlUnit(wk, 0, 0, 0, 0);
            wk->model->flags &= ~0x10;
            wk->actCounter = wk->actMax;
        }
        break;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_N_Escape);
#endif

/* NON_MATCHING: 209 of 250 words match.  */
#ifdef NON_MATCHING
s32 sbtlAAIProc_N_Kaihi(struct SBTLUNIT_WORK *wk) {
    sdfVector4_t mypos;
    sdfVector4_t front;
    sdfVector4_t rot;
    sdfVector4_t zero;
    sdfVector4_t back;
    sdfVector4_t dir;
    struct SBTLUNIT_MODEL *model;
    struct SBTLAI_WORK *aiwk;
    s32 hit;

    memset(&zero, 0, sizeof(zero));
    memset(&back, 0, sizeof(back));
    memset(&dir, 0, sizeof(dir));
    dir.z = -1.0f;
    wk->actCounter++;
    wk->actCounter = (wk->actCounter <= 0) ? 0 : (wk->actCounter < wk->actMax) ? wk->actCounter : wk->actMax;
    if ((wk->flg & 0x840000) == 0x840000) {
        return 0;
    }
    SDF_VU0_LOADVECTOR(&wk->mv_spdbk);
    SDF_VU0_STOREVECTOR(&wk->mv_spd);
    model = wk->model;
    model->frame++;
    model->frame = (model->frame <= 0) ? 0 : (model->frame > 0x7FFF) ? 0x7FFF : model->frame;
    if ((model->cmpframe != 0 && model->frame >= model->cmpframe) || wk->actCounter >= wk->actMax) {
        aiwk = wk->aiwk;
        if (aiwk->backmode != 0) {
            aiwk->stat = 2;
            aiwk->mode = aiwk->backmode;
            aiwk->backmode = 0;
            model->stat = 1;
            return 1;
        }
        sbtlResetAAI(wk);
        wk->aiwk->actcnt = (wk->aiwk->actcnt <= 0) ? 0 : (wk->aiwk->actcnt > 0x7FFF) ? 0x7FFF : wk->aiwk->actcnt;
        sbtlSetMotion_sbtlUnit(wk, 0, 4, 0, 0);
        wk->model->flags &= ~0x10;
        return 1;
    }
    switch (model->mode) {
    case 14:
        hit = 0;
        if (sbtlCheckCrossCircle(&wk->pos, model->field11C, &model->vec00, 30.0f) || sbtlBtlAAIProcNKaihiSub(wk)) {
            hit = 1;
        }
        if (hit == 1) {
            wk->model->mode = 2;
            sbtlGetRotateDeg(&wk->rot, 180.0f);
            SDF_VU0_STOREVECTOR(&rot);
            back.z = -wk->model->field11C;
            SDF_VU0_LOADVECTOR_B(&back);
            SDF_VU0_LOADVECTOR(&rot);
            ToMatrix();
            SDF_VU0_APPLYMATRIX_B();
            SDF_VU0_LOADVECTOR(&wk->pos);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(&wk->model->vec00);
        }
        break;
    case 2:
        SDF_VU0_LOADVECTOR(&zero);
        SDF_VU0_STOREVECTOR(&wk->mv_spd);
        SDF_VU0_LOADVECTOR(&wk->pos);
        SDF_VU0_STOREVECTOR(&mypos);
        SDF_VU0_LOADVECTOR(&wk->rot);
        ToMatrix();
        SDF_VU0_LOADVECTOR(&dir);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_NORMALIZE();
        SDF_VU0_SCALEVECTOR_V0(wk->model->field11C);
        SDF_VU0_LOADVECTOR_B(&mypos);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(&front);
        mypos.y = 0.0f;
        front.y = 0.0f;
        if (sbtlGetVerticalLength(&mypos.x, &front.x, &wk->model->vec00.x) <= wk->model->field11C * 0.29999999f &&
            sbtlCheckInner(&mypos, &front, &wk->model->vec00, 0.0f)) {
            aiwk = wk->aiwk;
            if (aiwk->backmode != 0) {
                aiwk->stat = 2;
                aiwk->mode = aiwk->backmode;
                aiwk->backmode = 0;
                wk->model->stat = 1;
                return 1;
            } else {
                sbtlResetAAI(wk);
                wk->aiwk->mode = 12;
                sbtlSetMotion_sbtlUnit(wk, 0, 4, 0, 0);
                wk->model->flags &= ~0x10;
            }
        }
        break;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_N_Kaihi);
#endif

s32 sbtlAAIProc_N_GardeT(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_MODEL *model;

    if ((wk->flg & 0x840000) == 0x840000) {
        return 0;
    }
    SDF_VU0_LOADVECTOR(&wk->mv_spdbk);
    SDF_VU0_STOREVECTOR(&wk->mv_spd);
    model = wk->model;
    model->frame++;
    model->frame = (model->frame <= 0) ? 0 : (model->frame > 0x7FFF) ? 0x7FFF : model->frame;
    if (model->cmpframe != 0 && model->frame >= model->cmpframe) {
        sbtlResetAAI(wk);
        wk->aiwk->actcnt = (wk->aiwk->actcnt <= 0) ? 0 : (wk->aiwk->actcnt > 0x7FFF) ? 0x7FFF : wk->aiwk->actcnt;
        sbtlSetMotion_sbtlUnit(wk, 0, 4, 0, 0);
        return 1;
    }
    if (model->mode == 14 && sbtlCheckCrossCircle(&wk->pos, model->field11C, &model->vec00, 10.0f)) {
        sbtlResetAAI(wk);
        wk->aiwk->actcnt = (wk->aiwk->actcnt <= 0) ? 0 : (wk->aiwk->actcnt > 0x7FFF) ? 0x7FFF : wk->aiwk->actcnt;
        sbtlSetMotion_sbtlUnit(wk, 0, 4, 0, 0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_Attack);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_BAttack);

s32 sbtlAAIProc_TurnMV(struct SBTLUNIT_WORK *wk) {
    sdfVector4_t zero;
    struct SBTLUNIT_MODEL *model;
    f32 x;
    f32 z;

    memset(&zero, 0, sizeof(zero));
    model = wk->model;
    model->frame++;
    model->frame = (model->frame <= 0) ? 0 : (model->frame > 0x7FFF) ? 0x7FFF : model->frame;
    SDF_VU0_LOADVECTOR(&wk->mv_spdbk);
    SDF_VU0_STOREVECTOR(&wk->mv_spd);
    switch (model->mode) {
    case 14:
        if (sbtlSetMotion(wk, SBTLUNIT_MOTION_WALK)) {
            wk->model->frame = 0;
            if (sbtlCheckVerticalArea(&wk->pos, &wk->rot, &wk->model->vec00, 200.0f)) {
                SDF_VU0_LOADVECTOR(&wk->max_mvspd);
                SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
                sbtlSetMotion_sbtlUnit(wk, 5, 4, 0, 0);
                wk->model->frame = 0;
                wk->model->flags = (wk->model->flags | 0x14) & ~0x20B;
            }
        } else if (sbtlCheckCrossCircle(&wk->pos, wk->model->field11C, &wk->model->vec00, 2.0f)) {
            wk->model->vec00.x = x = main_data.val20 + 0.0f;
            wk->model->vec00.z = z = main_data.val24 + 400.0f;
            wk->model->vec00.y = sbtlFree(x, z) + 0.0f;
            SDF_VU0_LOADVECTOR(&zero);
            SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
            wk->model->cmpframe = 10800;
            wk->model->frame = 0;
            wk->model->mode = 15;
            sbtlSetMotion_sbtlUnit(wk, 18, 4, 0, 0);
        }
        break;
    case 15:
        if (sbtlCheckVerticalArea(&wk->pos, &wk->rot, &model->vec00, 200.0f)) {
            sbtlResetAAI(wk);
            wk->aiwk->actcnt++;
            wk->aiwk->actcnt = (wk->aiwk->actcnt <= 0) ? 0 : (wk->aiwk->actcnt > 0x7FFF) ? 0x7FFF : wk->aiwk->actcnt;
            wk->skillid = SKLID_NONE;
        }
        break;
    }
    return 1;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_Skl_S);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_Skl_SMV);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_Skl_SRT);

s32 sbtlAAIProc_SUMMON(struct SBTLUNIT_WORK *wk) {
    sdfVector4_t zero;
    struct SBTLUNIT_MODEL *model;

    memset(&zero, 0, sizeof(zero));
    model = wk->model;
    model->frame++;
    model->frame = (model->frame <= 0) ? 0 : (model->frame > 0x7FFF) ? 0x7FFF : model->frame;
    SDF_VU0_LOADVECTOR(&zero);
    SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
    if ((model->cmpframe != 0 && model->cmpframe <= model->frame) ||
        (wk->mode == 8 && !(wk->vstatus & 1) && sbtlCheckNextAction(wk))) {
        if (sbtlLoadEffectData_Decode(wk)) {
            sbtlSetAction(wk, 1, 0);
            wk->aiwk->stat = 1;
            wk->model->flags = (wk->model->flags | 0x40) & ~0x20;
        }
    }
    return 1;
}

s32 sbtlAAIProc_N_GuardH(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_MODEL *model = wk->model;

    model->frame++;
    model->frame = (model->frame <= 0) ? 0 : (model->frame < 4001) ? model->frame : 4000;
    wk->flg |= 0x100;
    if (model->cmpframe <= model->frame) {
        sbtlResetAAI(wk);
        wk->flg &= ~0x100;
    }
    return 1;
}

s32 sbtlAAIProc_SAttack(struct SBTLUNIT_WORK *wk) {
    sbtlResetAAI(wk);
    return 1;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlMawariChk);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlInitAIdataDLC);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlGotoAtkPos);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlNakamaGaolChk_Sub);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlNakamaGoalChk);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlNakamaGoalChk2);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlBtlSetUseSkillSBtlActionSub);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlBtlSetUseSkillSBtlActionSub2);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlGetAnotherEnemy);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLockOnFree);

INCLUDE_RODATA(const s32, "sbtlActAI", sbtlGetMovePos_MapHit_2Table);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlSKINBodyLeft);

INCLUDE_ASM(const s32, "sbtlActAI", SklCameraEnd);

INCLUDE_ASM(const s32, "sbtlActAI", FrcSklEnd);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetUseSkill);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCheckNextAction);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetMotionHit);

INCLUDE_ASM(const s32, "sbtlActAI", CheckSkillMessage);

INCLUDE_RODATA(const s32, "sbtlActAI", effBTLCutRotateB);

INCLUDE_RODATA(const s32, "sbtlActAI", sbtlSetUseSkill_sbtlActionTable);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetUseSkill_sbtlAction);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetHangekiFlag_sbtlAction);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetAction);

void sbtlBS_MUNAKA_Init_Imad_Restart(struct SBTLUNIT_WORK *wk) {
    SBTLUNIT_MODEL *model = wk->model;
    struct SBTLUNIT_WORK *target;

    if ((model->flags & 0x4000) && (wk->field414 & 1)) {
        model->flags &= ~0x4000;
        wk->flg &= ~0x4010;
        target = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
        if (target != NULL) {
            sbtlResetAction(target);
        }
    }
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCheckStatus_Move);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlDeleteEnemy);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCheckBit);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlBtlSetUseSkillSBtlActionSub3);

INCLUDE_ASM(const s32, "sbtlActAI", StartSklProc);

INCLUDE_ASM(const s32, "sbtlActAI", RunBlur);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlRunSklProc);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf28);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlEisyoWaitTask);

INCLUDE_ASM(const s32, "sbtlActAI", EisyoEndCheck);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAttackwaitWaitTask);

INCLUDE_ASM(const s32, "sbtlActAI", AttackwaitEndCheck);

INCLUDE_ASM(const s32, "sbtlActAI", RaidoAttackCheck);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlZakoRouletteSet);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlZakoWarpReset);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlBossWarpReset);
