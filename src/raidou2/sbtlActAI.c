#include "common.h"
#include "sBtlUnit.h"
#include "sbtlWork.h"
#include "sbtlSub.h"
#include "sBtlAI.h"
#include "sbtlActAI.h"
#include "sBtlPath.h"

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

INCLUDE_ASM(const s32, "sbtlActAI", sbtlFunc11);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlFunc12);

INCLUDE_ASM(const s32, "sbtlActAI", CheckLoadData_sbtlScript);

INCLUDE_ASM(const s32, "sbtlActAI", evtIsFadeCompleteAndMessageWindowIdle);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlRefSbtl39);

INCLUDE_ASM(const s32, "sbtlActAI", fldEnableSceneGroupAdvancement);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara52);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlKudaHuuinCheck);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara53);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara54);

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

INCLUDE_ASM(const s32, "sbtlActAI", sbtlACTAIFLG_Clr);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCreate);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlDelete);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlAAIProc);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlBtlCheckScriptDataSub);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf23);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetSkillData_AI);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCheckEquationProc);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetChkskl);

INCLUDE_ASM(const s32, "sbtlActAI", SetChkskl);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf65);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetUseskl);

INCLUDE_ASM(const s32, "sbtlActAI", SetUseskl);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlResetUseskl);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCheckMeshiyose);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCheckEquationProc);

s32 sbtlBtlCheckEquationProcSub(s16 code) {
    return code != 0x16;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlGetBadStatus6);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCheckCharmTarget);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf66);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCheckCharmTarget);

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

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCheckCharmTarget2);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlSearchUnitID21);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCreateSideListSBtlUnit5);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCheckCharmTarget3);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCreateSideList3);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCreateSideListSBtlUnit6);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf67);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlSetChkskl2);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlSetChkskl3);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlSetUseskl);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlSetUseskl2);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallCheckLiveTarget);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallCheckLiveTarget2);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlGetBgmID);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCreateSideList4);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCreateSideListSBtlUnit);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCreateSideList);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCreateSideListSBtlUnit2);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCreateSideListSBtlUnit7);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCreateSideList5);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCreateSideListSBtlUnit8);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlGetUseTable_Target);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf24);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlSearchUnitID5);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCreateSideListSBtlUnit3);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf25);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf26);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlRefSbtl14);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallEffectFldTypeIsSpecial);

INCLUDE_RODATA(const s32, "sbtlActAI", sbtlCallGetIntPara23Table);

INCLUDE_ASM(const s32, "sbtlActAI", dds3EffectFldTypeIsSpecial);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetAIData);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlSearchUnitID6);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCheckEnemyTarget);

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

INCLUDE_ASM(const s32, "sbtlActAI", sbtlEAIInit_BtlInit);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlEAIInit_BtlWait);

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

INCLUDE_ASM(const s32, "sbtlActAI", sbtlEAIProc_Movef);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlEAIProc_BtlWait);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlEAIProc_Guard);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlEAIProc_Ctact);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlEAIProc_Search);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlEAIProc_Chase);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlEAIProc_Escape);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlEAIProc_Summonw);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlEAIProc_BtlAI);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetSearchEnemyMode);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetGroupAction);

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

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCheckHITCheck_Char);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlPrivAreaCheck);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCheckSearchEnemy);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf69);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf70);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf71);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf72);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetNoMP_sbtlActAI);

INCLUDE_RODATA(const s32, "sbtlActAI", sbtlSetGroupActionTable);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetNormalAction);

INCLUDE_ASM(const s32, "sbtlActAI", GetSenkaiPointSub);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlGetFukinPoint);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlGetEscapePoint);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlGetWarpPoint);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlGetNWarpPoint);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCheckCrossUnit_HIT);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlGetMovePos_MapHit);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlGetMovePos_MapHit_Nakama);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlGetMovePos_MapHit_2);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCheckMapHit);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_Free);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_Free_sbtlActAI);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlBtlAAIInitSklSSub);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_N_Escape);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_N_Kaihi);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_N_Wait);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_N_Sabato);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_N_GardeT);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_N_Renkei);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlSearchUnitID7);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlSearchUnitID8);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf27);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlGetSkilltbl5);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_BAttack);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_Skl_S);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_SUMMON);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_N_GuardH);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_SAttack);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_TurnMV);

void sbtlSetAIWkStat(struct SBTLUNIT_WORK *wk) {
    SBTLUNIT_MODEL *model;

    wk->aiwk->stat = 1;
    model = wk->model;
    model->stat = 1;
    model->mode = 0;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlResetAAI);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlGetWarpUnit_ID);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlSetMotion);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlBtlAAIProcNKaihiSub);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_Free);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_Free_sbtlActAI);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_N_Wait);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_N_Sabato);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_N_Renkei);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlSetMapFadeH);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_N_Comb_R);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_N_Return);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_N_Escape);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_N_Kaihi);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_Attack);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_Attack_sbtlActAI);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_BAttack);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_TurnMV);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_Skl_S);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_Skl_SMV);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_Skl_SRT);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_SUMMON);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_N_GuardH);

INCLUDE_ASM(const s32, "sbtlActAI", Proc_sbtlAction);

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

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlGetSkilltbl6);

INCLUDE_ASM(const s32, "sbtlActAI", CheckSkillMessage);

INCLUDE_RODATA(const s32, "sbtlActAI", effBTLCutRotateB);

INCLUDE_RODATA(const s32, "sbtlActAI", sbtlSetUseSkill_sbtlActionTable);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetUseSkill_sbtlAction);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetHangekiFlag_sbtlAction);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetAction);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlBS_MUNAKA_Init_Imad_Restart);

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
