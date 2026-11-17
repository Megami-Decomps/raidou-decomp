#include "common.h"
#include "sBtlUnit.h"
#include "sbtlSub.h"
#include "sbtlActAI.h"
#include "sBtlAI.h"
#include "sbtlWork.h"
#include "sBtlEff.h"
#include "sBtlSkill.h"
#include "sBtlSM.h"
#include "sBtlPath.h"
#include "sdfMemory.h"
#include "sdk_libc.h"
#include "ps2asm.h"
#include "sBtlCalc.h"
#include "sbtlDMG.h"
#include "sbtlSub_0027ED08.h"
#include "sdfVu0.h"
#include "code_r1_0027CBE8.h"
#include "mdlManager.h"

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

/* NON_MATCHING: 35 of 43 words match.  */
#ifdef NON_MATCHING
SBTLUNIT_MODEL *sbtlCreate(void) {
    struct sdfMemHandle_t *handle = sdfMemAllocDirect(sizeof(SBTLUNIT_MODEL));
    SBTLUNIT_MODEL *model = sdfMemLock(handle);

    memset(model, 0, sizeof(SBTLUNIT_MODEL));
    model->handle = handle;
    model->flags |= 0x24;
    model->field10C = -1;
    model->field11C = 60.0f;
    model->field120 = 100.0f;
    model->field124 = 200.0f;
    model->field128 = 300.0f;
    model->mode = 0;
    model->field104 = -1;
    model->field108 = -1;
    model->dataflg = 0;
    model->data_p[1] = 0;
    return model;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlCreate);
#endif

INCLUDE_ASM(const s32, "sbtlActAI", sbtlDelete);

s32 sbtlProc(struct SBTLUNIT_WORK *wk) {
    if ((wk->field414 & 0x80) || (wk->vstatus & 0x4000)) {
        wk->model->mode = 0;
        return 0;
    }
    if (wk->flg & 6) {
        if (!(wk_sbtl->flg & 0x10000)) {
            return 0;
        }
        sbtlBtlProcSub(wk);
        return 1;
    }
    sbtlCallBtlCheckEquation(wk);
    return 1;
}

AITBLDATA_WORK *sbtlGetAITBL(s32 devilId) {
    return &ai_tbldata[devilId];
}

s32 sbtlGetAIPatternData(s32 id, s32 p1, s32 p2) {
    return ai_tbldata[id].pat_tbl[p1][p2];
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetSkillData_AI);

/* NON_MATCHING: 22 of 36 words match.  */
#ifdef NON_MATCHING
s32 sbtlCheckEquation(struct SBTLUNIT_WORK *wk) {
    SBTLAI_WORK *aiwk = wk->aiwk;
    SBTLUNIT_MODEL *model;

    if (aiwk->scr_actcheckflg == 1) {
        return 0;
    }
    model = wk->model;
    if (model->mode == 9 || model->mode == 10) {
        return 0;
    }
    if (aiwk->stat != 1) {
        return 0;
    }
    if (model->stat != 2) {
        return 0;
    }
    if (model->dataflg) {
        sbtlUseTableDataProc(wk);
    } else {
        sbtlCheckEquationProc(wk);
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlCheckEquation);
#endif

INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetChkskl);

INCLUDE_ASM(const s32, "sbtlActAI", SetChkskl);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf65);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetUseskl);

/* NON_MATCHING: 5 of 15 words match.  */
#ifdef NON_MATCHING
void SetUseskl(struct SBTLUNIT_WORK *wk, SKLID sklid) {
    wk->model->useskl[sklid / 32] |= 1 << sklid;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", SetUseskl);
#endif

INCLUDE_ASM(const s32, "sbtlActAI", sbtlResetUseskl);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCheckMeshiyose);

/* NON_MATCHING: 97 of 203 words match.  */
#ifdef NON_MATCHING
s32 sbtlCheckEquationProc(struct SBTLUNIT_WORK *wk) {
    s32 chk[3];
    AITBLDATA_WORK *tbl;
    u32 code;
    s32 i;
    s32 n;
    s16 rnd;
    s16 sum;
    u32 sklid;
    u32 target;

    if (!(sbtlData100[wk_sbtl->stageNo].flags & 4) && (wk->field414 & 2) && wk->model->nonempflg) {
        tbl = sbtlGetAITBL(0);
    } else {
        tbl = sbtlGetAITBL(wk->unit.devilId);
    }
    chk[0] = -1;
    chk[1] = -1;
    chk[2] = -1;
    wk->model->ai_target[0] = -1;
    wk->model->ai_target[1] = -1;
    wk->model->ai_target[2] = -1;
    for (i = 0; i < 3; i++) {
        code = tbl->act[wk->model->datamode].code[i];
        n = (code & 0xFFF00000) >> 20;
        if (n != 0) {
            chk[i] = CheckFuncTbl[n](wk, code & 0xFFFFF, i);
                }
    }
    if (chk[0] == 1 && chk[1] == 1 && chk[2] == 1) {
        n = 0;
    } else if (chk[0] == 1 && chk[1] == 1) {
        n = 1;
    } else if (chk[0] == 1 && chk[2] == 1) {
        n = 2;
    } else if (chk[1] == 1 && chk[2] == 1) {
        n = 3;
    } else if (chk[0] == 1) {
        n = 4;
    } else if (chk[1] == 1) {
        n = 5;
    } else if (chk[2] == 1) {
        n = 6;
    } else {
        n = 7;
    }
    n = tbl->act[wk->model->datamode].jmptbl[n];
    if (n < 8) {
        i = 0;
        rnd = GetPercentage();
        sum = tbl->acttbl[n][0].per;
        while (sum < rnd || sum <= 0) {
            i++;
            if (i >= 4) {
                break;
            }
            sum += tbl->acttbl[n][i].per;
        }
        sklid = tbl->acttbl[n][i].sklid;
        target = tbl->acttbl[n][i].targetid;
        n = sklid >> 16;
        sbtlCallBtlSetNormalAction(wk, n, sklid & 0xFFFF, target >> 16, target & 0xFFFF);
        if (sbtlBtlCheckEquationProcSub(n)) {
            wk->model->dataflg = 0;
        }
    } else if (n < 16) {
        wk->model->data_p[0] = n - 8;
        sbtlUseTableDataProc(wk);
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlCheckEquationProc);
#endif

s32 sbtlBtlCheckEquationProcSub(s16 code) {
    return code != 0x16;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlGetBadStatus6);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCheckCharmTarget);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf66);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCheckCharmTarget);

/* NON_MATCHING: 3 of 56 words match.  */
#ifdef NON_MATCHING
s32 sbtlCallBtlCreateSideList2(struct SBTLUNIT_WORK *wk, s32 r, s32 idx)
{
    struct SBTLUNIT_LISTWORK *list;
    struct SBTLUNIT_WORK *unit;
    s32 i;
    s32 hit;
    f32 radius = r;

    list = sbtlCreateSideList(wk->field414, 1);
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
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCreateSideList2);
#endif

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCheckCharmTarget2);

s32 sbtlCallBtlSearchUnitID21(struct SBTLUNIT_WORK *wk, s32 num, s32 idx) {
    struct SBTLUNIT_WORK *target;
    struct SBTLUNIT_WORK *unit;
    SBTLUNIT_LISTWORK *list;
    s32 i;
    s32 n;
    s32 ret;

    i = 0;
    n = 0;
    target = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    list = sbtlCreateSideList_sbtlUnit(target->field414, 1);
    if (list->count > 0) {
        do {
            unit = list->pUnitL[i++].pUnit;
            if (sbtlCheckCrossCircle(&target->pos, 250.0f, &unit->pos, unit->model->field11C) && unit->aiwk->mode) {
                n++;
                wk->model->ai_target[idx] = unit->field1A0;
            }
        } while (i < list->count);
    }
    if (n < num) {
        ret = FALSE;
    } else {
        ret = TRUE;
    }
    sbtlDeleteUnitList(list);
    return ret;
}

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

/* NON_MATCHING: 13 of 92 words match.  */
#ifdef NON_MATCHING
s32 sbtlUseTableDataProc(struct SBTLUNIT_WORK *wk) {
    SBTLUNIT_MODEL *model;
    s32 devilid;
    s32 pat;
    s32 code;
    s32 arg;
    s32 codet;
    s32 arg2;
    s32 ret;

    model = wk->model;
    if (!(sbtlData100[wk_sbtl->stageNo].flags & 4) && (wk->field414 & 2)) {
        if (!model->nonempflg) {
            devilid = wk->unit.devilId;
        } else {
            devilid = 0;
        }
    } else {
        devilid = wk->unit.devilId;
    }
    arg2 = 0;
    codet = 0;
    do {
        pat = sbtlGetAIPatternData(devilid, wk->model->data_p[0], wk->model->data_p[1]);
        code = (pat & 0xFFFF0000) >> 16;
        arg = pat & 0xFFFF;
        if (sbtlBtlUseTableDataProcSub(code)) {
            pat = sbtlGetAIPatternData(devilid, wk->model->data_p[0], wk->model->data_p[1] + 1);
            codet = (pat & 0xFFFF0000) >> 16;
            arg2 = pat & 0xFFFF;
        }
        ret = sbtlCallBtlSetNormalAction(wk, code, arg, codet, arg2);
        switch (ret) {
        case 2:
        case 3:
            wk->model->data_p[1] += 1;
            break;
        case 4:
            wk->model->data_p[1] += 2;
            break;
        }
    } while (ret == 0);
    return ret == 3 || ret == 4;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlUseTableDataProc);
#endif

s32 sbtlBtlUseTableDataProcSub(s32 type) {
    switch (type) {
    case 9:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 20:
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlSetNormalAction);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlGetUseTable_Target_sbtlActAI);

/* NON_MATCHING: 66 of 69 words match.  */
#ifdef NON_MATCHING
s32 sbtlCheckEnemyTarget(struct SBTLUNIT_WORK *wk, s32 id) {
    struct SBTLUNIT_WORK *target = sbtlSearchUnit_ID(wk_sbtl->unitList, id);
    BST bst;
    s32 ret;

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
    ret = target != NULL ? id : -1;
    if (wk->flg & 0x100000) {
        if (target != NULL) {
            return id;
        }
        return wk_sbtl->targetUnitID;
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlCheckEnemyTarget);
#endif

INCLUDE_ASM(const s32, "sbtlActAI", sbtlRefSbtl21);

s32 sbtlResetAIPattern(struct SBTLUNIT_WORK *wk, u32 flg) {
    SBTLUNIT_MODEL *model = wk->model;

    model->dataflg = 0;
    model->datamode = flg;
    wk->model->data_p[0] = 0;
    wk->model->data_p[1] = 0;
    return 1;
}

s32 sbtlCallBtlCheckEquation(struct SBTLUNIT_WORK *wk) {
    SBTLUNIT_MODEL *model;

    sbtlCheckEquation(wk);
    model = wk->model;
    if (model->mode < 14) {
        switch (model->stat) {
        case 1:
            if (AAIInit_Func[model->mode](wk)) {
                wk->model->stat = 2;
            }
            break;
        case 2:
            model->frame++;
            model->frame = (model->frame <= 0) ? 0 : (model->frame > 0x7FFF) ? 0x7FFF : model->frame;
            SDF_VU0_LOADVECTOR(&wk->mv_spdbk);
            SDF_VU0_STOREVECTOR(&wk->mv_spd);
            AAIProc_Func[model->mode](wk);
            break;
        }
    } else if (wk->aiwk->stat == 1 && model->stat == 2) {
        model->mode = 12;
        if (!(model->flags & 0x1000)) {
            model->stat = 1;
        }
    } else {
        sbtlBtlProcSub(wk);
    }
    return 1;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf68);

/* NON_MATCHING: 29 of 40 words match.  */
#ifdef NON_MATCHING
s32 sbtlEAIInit_None(struct SBTLUNIT_WORK *wk) {
    s16 stat = 1;
    SBTLUNIT_MODEL *model;

    if (sbtlCheckScriptData(wk->unit.devilId)) {
        wk->aiwk->stat = stat;
        model = wk->model;
        model->mode = 12;
        model->stat = stat;
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
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlEAIInit_None);
#endif

s32 sbtlEAIInit_Summon(struct SBTLUNIT_WORK *wk) {
    sdfVector4_t zero;
    struct SBTLUNIT_WORK *unit;
    s32 i;
    s32 n;
    SBTL_STAGE_DATA *stage;

    memset(&zero, 0, sizeof(zero));
    wk->model->flags = (wk->model->flags | 0x24) & ~0x21B;
    SDF_VU0_LOADVECTOR(&zero);
    SDF_VU0_STOREVECTOR(&wk->mv_spd);
    SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
    wk->model->cmpframe = 60;
    wk->model->frame = 0;
    if (wk->vstatus & 0x4000000) {
        wk->vstatus &= ~0x4000000;
    } else {
        n = 0;
        for (i = 0; i < wk_sbtl->unitList->number; i++) {
            unit = wk_sbtl->unitList->pUnitL[i].pUnit;
            if (unit != NULL && (unit->field414 & 2) && unit->field1A0 != wk->field1A0 &&
                unit->unit.raceId == wk->unit.raceId && unit->unit.devilId == wk->unit.devilId &&
                (unit->mode == 1 || unit->mode == 2 || unit->mode == 4)) {
                n++;
            }
        }
        stage = &sbtlData100[wk_sbtl->stageNo];
        if (!(stage->flags & 4)) {
            if (n > 0) {
                sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_ENC_ENEMIES, SKLID_NONE);
            } else {
                sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_ENC_ENEMY, SKLID_NONE);
            }
            for (i = 0; i < wk_sbtl->unitList->number; i++) {
                unit = wk_sbtl->unitList->pUnitL[i].pUnit;
                if (unit != NULL && (unit->field414 & 2) && unit->field1A0 != wk->field1A0 &&
                    unit->unit.raceId == wk->unit.raceId && unit->unit.devilId == wk->unit.devilId) {
                    unit->vstatus |= 0x4000000;
                }
            }
        }
    }
    return 1;
}

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

s32 sbtlEAIProc_Movef(struct SBTLUNIT_WORK *wk) {
    SBTLUNIT_MODEL *model = wk->model;

    if (model->cmpframe <= model->frame ||
        ((model->flags & 0x10) && sbtlCheckCrossCircle(&wk->pos, model->field120, &model->vec00, 2.0f))) {
        sbtlResetAAI(wk);
        sbtlSetAIWkStat(wk);
    }
    return 1;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlEAIProc_BtlWait);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlEAIProc_Guard);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlEAIProc_Ctact);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlEAIProc_Search);

/* NON_MATCHING: 40 of 70 words match.  */
#ifdef NON_MATCHING
s32 sbtlEAIProc_Chase(struct SBTLUNIT_WORK *wk) {
    SBTLUNIT_MODEL *model = wk->model;
    struct SBTLUNIT_WORK *target;
    sdfVector4_t pos0;
    sdfVector4_t pos1;

    if (model->cmpframe > model->frame) {
        target = sbtlSearchUnit_ID(wk_sbtl->unitList, model->field104);
        if (target == NULL || !CheckLiveTarget(target)) {
            goto lost;
        }
        if (!sbtlCheckCrossCircle_Rot(&wk->pos, &wk->rot, wk->model->field120, &target->pos, target->model->field11C,
                                      0.7f)) {
            return 1;
        }
        SDF_VU0_LOADVECTOR(&wk->pos);
        SDF_VU0_STOREVECTOR(&pos0);
        SDF_VU0_LOADVECTOR(&target->pos);
        SDF_VU0_STOREVECTOR(&pos1);
        pos0.y = 0.0f;
        pos1.y = 0.0f;
        if (!sbtlCheckVerticalArea(&pos0, &wk->rot, &pos1, 80.0f)) {
            return 1;
        }
    }
    sbtlResetAAI(wk);
    sbtlSetAIWkStat(wk);
    return 1;
lost:
    sbtlResetAAI(wk);
    sbtlSetAIWkStat(wk);
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlEAIProc_Chase);
#endif

INCLUDE_ASM(const s32, "sbtlActAI", sbtlEAIProc_Escape);

/* NON_MATCHING: 3 of 72 words match.  */
#ifdef NON_MATCHING
s32 sbtlEAIProc_Summonw(struct SBTLUNIT_WORK *wk) {
    s32 i;
    struct SBTLUNIT_WORK *unit;
    SBTLUNIT_MODEL *model;

    if (!(wk_sbtl->flg & 0x10000)) {
        return 0;
    }
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        unit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (unit != NULL && (unit->field414 & 2) && (unit->mode == 1 || unit->mode == 2)) {
            return 0;
        }
    }
    if (wk->motionno != 0 && !sbtlCheckMotionEnd(wk)) {
        return 1;
    }
    if (wk->vstatus & 1) {
        return 1;
    }
    if (wk->mode == 1 || wk->mode == 2) {
        return 1;
    }
    model = wk->model;
    wk->flg2 |= 0x80;
    model->stat = 1;
    model->mode = 0;
    model->flags = (model->flags | 0x40) & ~0x20;
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlEAIProc_Summonw);
#endif

INCLUDE_ASM(const s32, "sbtlActAI", sbtlEAIProc_BtlAI);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetSearchEnemyMode);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetGroupAction);

/* NON_MATCHING: 15 of 32 words match.  */
#ifdef NON_MATCHING
s32 sbtlSetNoMP(struct SBTLUNIT_WORK *wk) {
    SBTLUNIT_MODEL *model = wk->model;

    if (model->mode < 12 && (wk_sbtl->battleMode == 3 || !(wk->field414 & 0x80))) {
        return model->field100;
    }
    return model->field104;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetNoMP);
#endif

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

/* NON_MATCHING: 6 of 19 words match.  */
#ifdef NON_MATCHING
s32 sbtlBtlMVSupportSub(struct SBTLUNIT_WORK *wk, struct SBTLUNIT_WORK *wklock) {
    s32 side = wk->field414 & 7;
    s32 sidelock;
    s32 ret;

    if (side == 0 || (sidelock = wklock->field414 & 7) == 0) {
        return 1;
    }
    if (wk->field414 & 2) {
        return 1;
    }
    ret = side != sidelock;
    if (wklock->field414 & 2) {
        return 1;
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlBtlMVSupportSub);
#endif

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
    case 292:
    case 294:
    case 295:
    case 296:
        ret = 0;
        break;
    }
    return ret;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlPrivAreaCheck);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCheckSearchEnemy);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf69);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf70);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf71);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlLeaf72);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetNoMP_sbtlActAI);

/* NON_MATCHING: 209 of 213 words match.  */
#ifdef NON_MATCHING
s32 sbtlSetNormalAction(struct SBTLUNIT_WORK *wk, u16 type, s8 flg, s32 time, s32 arg4) {
    sdfVector4_t zero;
    SBTLUNIT_MODEL *model;

    memset(&zero, 0, sizeof(zero));
    switch (type) {
    case SBTL_ACTAIMODE_WAIT:
        wk->model->flags = (wk->model->flags | 4) & ~0x21B;
        SDF_VU0_LOADVECTOR(&zero);
        SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
        wk->model->cmpframe = time;
        wk->model->mode = 1;
        wk->model->frame = 0;
        sbtlSetMotion_sbtlUnit(wk, 0, 0, 0, 0);
        sbtlCallBtlEndAction(wk, 1, 0);
        break;
    case SBTL_ACTAIMODE_MOVEF:
        if (!sbtlGetMovePos_MapHit(wk, wk, &wk->model->vec00, wk->max_mvspd.z * time, (s32)GetRand(360), 0)) {
            SDF_VU0_LOADVECTOR(&wk->pos);
            model = wk->model;
            SDF_VU0_STOREVECTOR(&model->vec00);
        } else {
            model = wk->model;
        }
        model->cmpframe = time;
        model->mode = 2;
        model->frame = 0;
        SDF_VU0_LOADVECTOR(&wk->max_mvspd);
        SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
        wk->model->flags = (wk->model->flags | 0x14) & ~0x20B;
        sbtlSetMotion_sbtlUnit(wk, 5, 0, 0, 0);
        sbtlCallBtlEndAction(wk, 2, 0);
        break;
    case SBTL_ACTAIMODE_GUARD:
        wk->model->cmpframe = time;
        wk->model->mode = 3;
        wk->model->frame = 0;
        SDF_VU0_LOADVECTOR(&zero);
        SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
        wk->model->flags = (wk->model->flags | 4) & ~0x21B;
        sbtlSetMotion_sbtlUnit(wk, 29, 0, 0, 0);
        sbtlCallBtlEndAction(wk, 2, 0);
        break;
    case SBTL_ACTAIMODE_CTACT:
        wk->model->flags = (wk->model->flags | 5) & ~0x21A;
        SDF_VU0_LOADVECTOR(&wk->max_mvspd);
        SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
        wk->model->cmpframe = 1200;
        wk->model->mode = 4;
        wk->model->frame = 0;
        sbtlSetMotion_sbtlUnit(wk, 5, 4, 0, 0);
        sbtlCallBtlEndAction(wk, 2, 0);
        break;
    case SBTL_ACTAIMODE_SEARCH:
        wk->model->flags = (wk->model->flags | 4) & ~0x21B;
        SDF_VU0_LOADVECTOR(&zero);
        SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
        wk->model->cmpframe = 180;
        wk->model->mode = 5;
        wk->model->frame = 0;
        sbtlSetMotion_sbtlUnit(wk, 0, 4, 0, 0);
        sbtlCallBtlEndAction(wk, 1, 0);
        break;
    case SBTL_ACTAIMODE_CHASE:
        wk->model->flags = (wk->model->flags | 0x204) & ~0x1B;
        SDF_VU0_LOADVECTOR(&wk->max_mvspd);
        SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
        wk->model->cmpframe = time;
        wk->model->mode = 6;
        wk->model->frame = 0;
        sbtlSetMotion_sbtlUnit(wk, 5, 4, 0, 0);
        sbtlCallBtlEndAction(wk, 2, 0);
        break;
    case SBTL_ACTAIMODE_BSTWAIT:
        wk->model->flags = (wk->model->flags | 4) & ~0x21B;
        SDF_VU0_LOADVECTOR(&zero);
        SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
        wk->model->cmpframe = time;
        wk->model->mode = 1;
        wk->model->frame = 0;
        sbtlSetMotion_sbtlUnit(wk, 2, 4, 0, 0);
        sbtlCallBtlEndAction(wk, 1, 0);
        break;
    }
    return 1;
}
#else
INCLUDE_RODATA(const s32, "sbtlActAI", sbtlSetGroupActionTable);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlSetNormalAction);
#endif

/* NON_MATCHING: 88 of 145 words match.  */
#ifdef NON_MATCHING
s32 GetSenkaiPointSub(struct SBTLUNIT_WORK *wk, struct SBTLUNIT_WORK *wklock, sdfVector4_t *pos, s32 area) {
    sdfVector4_t dir;
    sdfVector4_t point;
    sdfMatrix4_t mtx;
    sdfVector4_t mypos;
    sdfVector4_t lockpos;
    f32 len;
    s32 i;

    len = area;
    memset(&point, 0, sizeof(point));
    point.w = 1.0f;
    SDF_VU0_LOADVECTOR(&wk->pos);
    SDF_VU0_SETVECTOR_Y(0.0f);
    SDF_VU0_STOREVECTOR(&mypos);
    SDF_VU0_LOADVECTOR(&wklock->pos);
    SDF_VU0_SETVECTOR_Y(0.0f);
    SDF_VU0_STOREVECTOR(&lockpos);
    if (GetPercentage() < 50) {
        SetRotateMatrixY(1.5707963f);
        SDF_VU0_STOREMATRIX(&mtx);
    } else {
        SetRotateMatrixY(-1.5707963f);
        SDF_VU0_STOREMATRIX(&mtx);
    }
    SDF_VU0_LOADVECTOR(&mypos);
    SDF_VU0_LOADVECTOR_B(&lockpos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&dir);
    SDF_VU0_LOADMATRIX(&mtx);
    SDF_VU0_LOADVECTOR(&dir);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(&dir);
    SDF_VU0_SCALEVECTOR(wk->model->field11C + len);
    SDF_VU0_LOADVECTOR_B(&wk->pos);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_MEMOPS(point);
    i = 0;
    while (i < 2 && (sbtlCheckArea(wk, wk->model->field11C, &point) || !sbtlCheckMapHit(&wk->pos, &point, wk->model->field11C + len))) {
        SetRotateMatrixY(3.1415926f);
        SDF_VU0_LOADVECTOR(&dir);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(&dir);
        SDF_VU0_SCALEVECTOR(wk->model->field11C + len);
        SDF_VU0_LOADVECTOR_B(&wk->pos);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR_MEMOPS(point);
        i++;
    }
    if (i < 2) {
        SDF_VU0_LOADVECTOR(&point);
        SDF_VU0_STOREVECTOR(pos);
    } else {
        SDF_VU0_LOADVECTOR(&wk->pos);
        SDF_VU0_STOREVECTOR(pos);
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", GetSenkaiPointSub);
#endif

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallMemset13);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCreateSideListSBtlUnit3);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlGetMovePos_MapHit);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlGetMovePos_MapHit_2);

/* NON_MATCHING: 2 of 5 words match.  */
#ifdef NON_MATCHING
s32 sbtlCheckMapHit(const sdfVector4_t *pos0, const sdfVector4_t *ppos, f32 dis) {
    s32 hit = sbtlCheckMapHit_Cross(pos0, ppos, dis);

    return hit;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlCheckMapHit);
#endif

s32 sbtlBtlProcSub(struct SBTLUNIT_WORK *wk) {
    switch (wk->model->stat) {
    case 1:
        EAIInit_Func[wk->aiwk->mode](wk);
        break;
    case 2:
        EAIProc_Func[wk->aiwk->mode](wk);
        break;
    }
    return 1;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_Free);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlGetWarpUnitID);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlBtlAAIInitSklSSub);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_N_Escape);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_N_Kaihi);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_N_Wait);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_N_Sabato);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_N_GardeT);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_N_Renkei_sbtlActAI);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_N_Renkei);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_N_Comb_R);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_N_Return);

s32 sbtlAAIInit_Attack(struct SBTLUNIT_WORK *wk) {
    SBTLUNIT_MODEL *model;
    struct SBTLUNIT_WORK *target;
    f32 len;
    f32 length;

    if (sbtlGetSkilltbl(wk->skillid)->area & 0x10) {
        model = wk->model;
        model->flags = (model->flags | 4) & ~0x21B;
    } else {
        model = wk->model;
        model->flags = (model->flags | 0x205) & ~0x1A;
    }
    SDF_VU0_LOADVECTOR(&wk->max_mvspd);
    SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
    model->mode = 14;
    model->frame = 0;
    model->field140 = 0;
    target = sbtlSearchUnit_ID(wk_sbtl->unitList, model->field104);
    if (target != NULL) {
        SDF_VU0_LOADVECTOR(&target->pos);
        SDF_VU0_LOADVECTOR_B(&wk->pos);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_GETLENGTH(len);
        wk->model->cmpframe = len / wk->mv_spdbk.z;
        length = wk->model->field120 + target->model->field11C;
    } else {
        wk->model->cmpframe = 180;
        length = wk->model->field120;
    }
    sbtlSetWalk(wk, length);
    wk->model->stat = 2;
    return 1;
}

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

void sbtlResetAAI(struct SBTLUNIT_WORK *wk) {
    sdfVector4_t spd;

    memset(&spd, 0, sizeof(spd));
    spd.w = 1.0f;
    SDF_VU0_LOADVECTOR(&spd);
    SDF_VU0_STOREVECTOR(&wk->mv_spd);
    SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
    wk->aiwk->stat = 1;
    sbtlCallBtlGetSkilltbl8(wk, 0x400);
    if ((wk->field414 & 1) && (wk->flg & 0x40000)) {
        wk_sbtl->flg |= 0x4000;
    }
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlGetWarpUnit_ID);

void sbtlSetWalk(struct SBTLUNIT_WORK *wk, f32 length) {
    if (sbtlSetMotion(wk, SBTLUNIT_MOTION_WALK) && !(wk->field414 & 2)) {
        sbtlSetMotion_sbtlUnit(wk, SBTLUNIT_MOTION_WALK, 4, 0, 0);
        sbtlCallBtlEndAction(wk, 2, 0);
    }
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlBtlAAIProcNKaihiSub);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_Free);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlResetAAI);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_N_Wait);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_N_Sabato);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_N_Renkei);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_N_Comb_E);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlSearchUnitID13);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_N_Return);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_N_Escape);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_N_Kaihi);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_Attack);

/* NON_MATCHING: 133 of 258 words match.  */
#ifdef NON_MATCHING
s32 sbtlCallMemset14(struct SBTLUNIT_WORK *wk) {
    sdfVector4_t zero;
    sdfVector4_t pos0;
    sdfVector4_t pos1;
    SBTLUNIT_MODEL *model;
    struct SBTLSKILLP_TBL *skl;
    struct SBTLUNIT_WORK *target;
    SBTLAI_WORK *aiwk;
    s32 hit;
    s32 early;
    f32 r;
    f32 deg;

    memset(&zero, 0, sizeof(zero));
    model = wk->model;
    model->frame++;
    model->frame = (model->frame <= 0) ? 0 : (model->frame > 0x7FFF) ? 0x7FFF : model->frame;
    if ((wk->field414 & 1) && model->cmpframe != 0 && model->cmpframe <= model->frame) {
        sbtlResetAAI(wk);
        return 1;
    }
    SDF_VU0_LOADVECTOR(&wk->mv_spdbk);
    SDF_VU0_STOREVECTOR(&wk->mv_spd);
    skl = sbtlGetSkilltbl(wk->skillid);
    switch (wk->model->mode) {
    case 14:
        hit = 0;
        target = sbtlSearchUnit_ID(wk_sbtl->unitList, sbtlSetNoMP(wk));
        if (target == NULL) {
            hit = 1;
        } else if (skl->area & 0x10) {
            hit = 1;
        } else {
            if (wk->model->cmpframe != 0) {
                early = wk->model->frame - 1 < wk->model->cmpframe;
                hit = early == 0;
            }
            if (skl->type & 0x1000) {
                deg = sbtlGetChorikiDeg(&wk->pos, &wk->rot, &target->pos, 1.0f);
                SDF_VU0_STOREVECTOR(&wk->rot);
                sbtlAddUnitDeg(wk, deg);
                skl = sbtlGetSkilltbl(wk->skillid);
                if (skl->type & 0x1000) {
                    hit = 1;
                }
            }
            if ((wk->flg & 0x2000000) || (wk->field414 & 2)) {
                SDF_VU0_LOADVECTOR(&zero);
                SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
                SDF_VU0_LOADVECTOR(&wk->pos);
                SDF_VU0_STOREVECTOR(&pos0);
                SDF_VU0_LOADVECTOR(&target->pos);
                SDF_VU0_STOREVECTOR(&pos1);
                pos0.y = 0.0f;
                pos1.y = 0.0f;
                if (sbtlCheckVerticalArea(&pos0, &wk->rot, &pos1, 80.0f)) {
                    hit = 1;
                }
            } else {
                SDF_VU0_LOADVECTOR(&wk->pos);
                SDF_VU0_STOREVECTOR(&pos0);
                SDF_VU0_LOADVECTOR(&target->pos);
                SDF_VU0_STOREVECTOR(&pos1);
                pos0.y = 0.0f;
                pos1.y = 0.0f;
                if (skl->area & 0x20) {
                    r = wk->model->field120;
                } else {
                    r = 300.0f;
                }
                if (sbtlCheckCrossCircle_Rot(&pos0, &wk->rot, r, &pos1, target->model->field11C, 0.7f)
                    && sbtlCheckVerticalArea(&pos0, &wk->rot, &pos1, 80.0f)) {
                    hit = 1;
                }
            }
        }
        if (hit == 1) {
            SDF_VU0_LOADVECTOR(&zero);
            SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
            if (sdfRemoveTaskItem(wk, 1)) {
                wk->model->flags = (wk->model->flags | 4) & ~0x21B;
                wk->model->cmpframe = 0;
                wk->model->frame = 0;
                wk->model->mode = 16;
                sbtlCallBtlGetNumOfEffectSAB(wk, wk->skillid, 0);
            } else {
                sbtlSetNoMP_sbtlActAI(wk);
                sbtlResetAAI(wk);
                sbtlRefSbtl21(wk);
            }
        }
        break;
    case 15:
        if (sbtlCheckStatus_Move(wk)) {
            sbtlResetAAI(wk);
            aiwk = wk->aiwk;
            aiwk->actcnt++;
            aiwk->actcnt = (aiwk->actcnt <= 0) ? 0 : (aiwk->actcnt > 0x7FFF) ? 0x7FFF : aiwk->actcnt;
        }
        break;
    case 16:
        if (wk->hModel->first->frame >= 0.0f) {
            wk->model->cmpframe = 0;
            wk->model->frame = 0;
            wk->model->mode = 15;
            sbtlCheckMHTData(wk, wk->skillid);
        }
        break;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallMemset14);
#endif

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlResetAAI2);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_TurnMV);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_Skl_S);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_Skl_SMV);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_Skl_SRT);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_SUMMON);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_N_GuardH);

INCLUDE_ASM(const s32, "sbtlActAI", Proc_sbtlAction);

INCLUDE_RODATA(const s32, "sbtlActAI", sbtlGetMovePos_MapHit_2Table);

INCLUDE_ASM(const s32, "sbtlActAI", Proc);

s32 sbtlCallBtlSearchEffectSABActer(struct SBTLUNIT_WORK *wk) {
    if (sbtlCallBtlGetMotionVisualData2(wk, wk->next_motionID)) {
        return 1;
    }
    if (sbtlGetMotionVisualData(wk->major, wk->minor, wk->motionno)->field18 == 1 &&
        (wk->skillid != SKLID_NONE || !(wk->field414 & 2))) {
        if (sbtlSearchEffect_SAB_Acter(wk_sbtl->effList, wk, wk->skillid, 1)) {
            return 0;
        }
    }
    return sbtlCheckMotionEnd(wk) != 0;
}

/* NON_MATCHING: 3 of 14 words match.  */
#ifdef NON_MATCHING
s32 sbtlRefSbtl22(struct SBTLUNIT_WORK *wk) {
    return wk_sbtl->mode == 14 && wk_sbtl->huumaTargetID != -1 && wk_sbtl->huumaTargetID == wk->field1A0;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlRefSbtl22);
#endif

s32 sbtlSetMotionHit(struct SBTLUNIT_WORK *wk, s32 motionno, SKLID skillid) {
    s32 i;
    s32 hit = 0;
    struct SBTLSKILLP_TBL *tbl;
    SBTL_HIT_PACDATA *pdata;

    for (i = 0; i < wk->hitwk.num; i++) {
        if (wk->hitwk.pwk[i]->entry.motionno == motionno) {
            tbl = sbtlGetSkilltbl(skillid);
            pdata = wk->hitwk.pwk[i];
            sbtlCreateHitC_New_sbtlDMG(wk_sbtl->hitList, wk, NULL, wk->field1A0, wk->skillid, tbl, &pdata->entry, pdata,
                                       wk->field414, -1);
            hit = 1;
        }
    }
    if (hit == 1) {
        sbtlAddSerialID2();
    }
    return 1;
}

void sbtlSetUseSkill(struct SBTLUNIT_WORK *wk, SKLID skillid) {
    if (sbtlGetSkilltbl(skillid)->flg & 0x40000000) {
        sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_BOSS, SKLID_NONE);
    }
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlGetNumOfEffectSAB);

s32 sbtlCallBtlEndAction(struct SBTLUNIT_WORK *wk, u32 action, s32 flg) {
    switch (action) {
    case 7:
    case 8:
    case 9:
    case 23:
    case 33:
        sbtlEndAction(wk);
        break;
    }
    if (sbtlGetBadStatus(&wk->unit) == BST_CHARM && action != 33 && action != 10 && action != 34 && action != 40) {
        return 0;
    }
    wk->status = action;
    wk->framecntB = flg;
    wk->framecnt = 0;
    wk->action_flg = 0;
    wk->cmpframecnt2 = 0xFFFFFFF;
    return 1;
}

void sbtlBS_MUNAKA_Init_Imad_Restart(struct SBTLUNIT_WORK *wk) {
    SBTLUNIT_MODEL *model = wk->model;
    struct SBTLUNIT_WORK *target;

    if ((model->flags & 0x4000) && (wk->field414 & 1)) {
        model->flags &= ~0x4000;
        wk->flg &= ~0x4010;
        target = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
        if (target != NULL) {
            sbtlCallFalseHuuma(target);
        }
    }
}

s32 sbtlCheckStatus_Move(struct SBTLUNIT_WORK *wk) {
    return wk->status < 3;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlDeleteHitHelper);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlDeletePlayer);

INCLUDE_ASM(const s32, "sbtlActAI", SetAction_ProgramSkill);
