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
#include "sBtlDraw.h"
#include "scrTraceCode.h"
#include "sBtlCalc.h"
#include "sbtlDMG.h"
#include "sbtlSub_0027ED08.h"
#include "sdfVu0.h"
#include "code_r1_0027CBE8.h"
#include "mdlManager.h"
#include "sBtlScript.h"
#include "sBtlEncount.h"
#include "sBtlVisual.h"
#include "sdfQuat.h"
#include "sdfMath.h"
#include "dds3Basic.h"
#include "fldProcess.h"
#include "sBtlPlayer.h"

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

    if (wk_sbtl->mode == 8 || wk_sbtl->mode == 16) {
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

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallGetIntPara54);

/* NON_MATCHING: 38 of 43 words match.  */
#ifdef NON_MATCHING
SBTLUNIT_MODEL *sbtlCreate(void) {
    struct sdfMemHandle_t *handle = sdfMemAllocDirect(sizeof(SBTLUNIT_MODEL));
    SBTLUNIT_MODEL *model = sdfMemLock(handle);

    memset(model, 0, sizeof(SBTLUNIT_MODEL));
    model->handle = handle;
    model->flags |= 0x24;
    model->field11C = 60.0f;
    model->field120 = 100.0f;
    model->field124 = 200.0f;
    model->field128 = 300.0f;
    model->mode = 0;
    model->field104 = -1;
    model->field108 = -1;
    model->field10C = -1;
    model->dataflg = 0;
    model->data_p[1] = 0;
    return model;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlCreate);
#endif

s32 sbtlDelete(SBTLUNIT_MODEL *model)
{
    if (model != NULL) {
        sdfMemFreeDirect(model->handle);
    }
    return 1;
}

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

s32 sbtlSetSkillData_AI(struct SBTLUNIT_WORK *wk, SKLID skillid) {
    wk->skillid = skillid;
    sbtlSetAIMode(wk);
    wk->model->stat = 1;
    sbtlCheckTargetEquation(wk);
    wk->aiwk->stat = 2;
    wk->model->mode = 14;
    return 1;
}

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
    if (aiwk->stat == 1 && model->stat == 2) {
        if (model->dataflg) {
            sbtlUseTableDataProc(wk);
        } else {
            sbtlCheckEquationProc(wk);
        }
        return 1;
    }
    return 0;
}

s32 sbtlSetChkskl(struct SBTLUNIT_WORK *wk, s32 sklid) {
    return (wk->model->chkskl[sklid / 32] & (1 << sklid)) != 0;
}

/* NON_MATCHING: 5 of 15 words match.  */
#ifdef NON_MATCHING
void SetChkskl(struct SBTLUNIT_WORK *wk, SKLID sklid)
{
    wk->model->chkskl[sklid / 32] |= 1 << sklid;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", SetChkskl);
#endif

void sbtlLeaf65(struct SBTLUNIT_WORK *wk, SKLID sklid)
{
    wk->model->chkskl[sklid / 32] &= ~(1 << sklid);
}

s32 sbtlSetUseskl(struct SBTLUNIT_WORK *wk, s32 sklid) {
    return (wk->model->useskl[sklid / 32] & (1 << sklid)) != 0;
}

/* NON_MATCHING: 5 of 15 words match.  */
#ifdef NON_MATCHING
void SetUseskl(struct SBTLUNIT_WORK *wk, SKLID sklid) {
    wk->model->useskl[sklid / 32] |= 1 << sklid;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", SetUseskl);
#endif

void sbtlResetUseskl(struct SBTLUNIT_WORK *wk, SKLID sklid)
{
    wk->model->useskl[sklid / 32] &= ~(1 << sklid);
}

s32 sbtlCheckMeshiyose(struct SBTLUNIT_WORK *wk) {
    s32 ret = 0;

    if (sbtlCheckGenerator_D() != 0) {
        ret = wk_sbtl->field424 < 6;
    }
    return ret;
}

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
        sbtlSetAIData(wk, n, sklid & 0xFFFF, target >> 16, target & 0xFFFF);
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

/* NON_MATCHING: 10 of 18 words match.  */
#ifdef NON_MATCHING
s32 sbtlCallBtlGetBadStatus6(struct SBTLUNIT_WORK *wk, u8 flg) {
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

/* body: matched in raidou @ 61f09dc: src/raidou1/code_002929C8.c:124, near, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
s32 sbtlLeaf66(void) {
    return -1;
}

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

s32 sbtlCallBtlCheckCharmTarget3(struct SBTLUNIT_WORK *wk, s32 per, s32 idx) {
    struct SBTLUNIT_WORK *unit = sbtlCheckCharmTarget(wk);

    if (unit->unit.now.hp * 100 < unit->unit.now.maxhp * per) {
        wk->model->ai_target[idx] = wk->field1A0;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCreateSideList3);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCreateSideListSBtlUnit6);

s32 sbtlLeaf67(struct SBTLUNIT_WORK *wk, s32 cnt, s32 idx) {
    if (wk->aiwk->syscnt_stop >= cnt) {
        wk->model->ai_target[idx] = wk->field1A0;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlSetChkskl2);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlSetChkskl3);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlSetUseskl);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlSetUseskl2);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallCheckLiveTarget);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallCheckLiveTarget2);

s32 sbtlGetBgmID(s32 encountid, u16 bit) {
    return sbtlCheckEvtBit_Table(bit) != 0;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCreateSideList4);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCreateSideListSBtlUnit);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCreateSideList);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCreateSideListSBtlUnit2);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCreateSideListSBtlUnit7);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCreateSideList5);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCreateSideListSBtlUnit8);

s32 sbtlGetUseTable_Target(struct SBTLUNIT_WORK *wk) {
    return sbtlCheckMeshiyose(wk) != 0;
}

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
        ret = sbtlSetAIData(wk, code, arg, codet, arg2);
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

s32 sbtlSetAIData(struct SBTLUNIT_WORK *wk, s32 code, s32 arg, s32 codet, s32 arg2)
{
    struct SBTLUNIT_WORK *unit;
    s32 ret = 0;
    s32 i;
    s32 sel;
    f32 len;

    switch (code) {
    case 0:
        ret = 1;
        break;
    case 1:
        wk->model->mode = 1;
        sbtlSetNormalAction(wk, 1, 0, arg, 0);
        ret = 2;
        wk->model->stat = 2;
        wk->aiwk->stat = 2;
        wk->model->dataflg = 1;
        break;
    case 21:
        ret = 2;
        wk->model->mode = 1;
        sbtlSetNormalAction(wk, wk->model->mode, 0, GetRand((u16)(arg + 1)), 0);
        wk->model->stat = 2;
        wk->aiwk->stat = 2;
        wk->model->dataflg = 1;
        break;
    case 20:
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, sbtlCheckEnemyTarget(wk, sbtlGetUseTable_Target_sbtlActAI(wk, codet, arg2)));
        if (unit != NULL) {
            wk->model->mode = 6;
            wk->model->field104 = unit->field1A0;
            wk->model->field100 = unit->field1A0;
            sbtlSetNormalAction(wk, 6, 0, arg, 0);
            wk->model->stat = 2;
            wk->aiwk->stat = 2;
        }
        ret = 4;
        wk->model->dataflg = 1;
        break;
    case 2:
        wk->model->mode = 2;
        sbtlSetNormalAction(wk, 2, 0, arg, 0);
        ret = 2;
        wk->model->stat = 2;
        wk->aiwk->stat = 2;
        wk->model->dataflg = 1;
        break;
    case 23:
        wk->model->mode = 3;
        sbtlSetNormalAction(wk, 3, 0, arg, 0);
        ret = 2;
        wk->model->stat = 2;
        wk->aiwk->stat = 2;
        wk->model->dataflg = 1;
        break;
    case 13:
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, sbtlCheckEnemyTarget(wk, sbtlGetUseTable_Target_sbtlActAI(wk, codet, arg2)));
        if (unit != NULL) {
            struct SBTLUNIT_MODEL *model;

            wk->model->flags = (wk->model->flags | 0x14) & ~0x20B;
            SDF_VU0_LOADVECTOR(&wk->max_mvspd);
            SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
            wk->model->mode = 2;
            SDF_VU0_LOADVECTOR(&unit->pos);
            model = wk->model;
            SDF_VU0_STOREVECTOR(model);
            SDF_VU0_LOADVECTOR_B(&wk->pos);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_GETLENGTH(len);
            len = len / wk->mv_spdbk.z;
            if (len <= (f32)arg) {
                len = arg;
            }
            model->cmpframe = len;
            model->frame = 0;
            sbtlSetMotion_sbtlUnit(wk, 5, 0, 0, 0);
            sbtlSetAction(wk, 2, 0);
            wk->model->stat = 2;
            wk->aiwk->stat = 2;
        }
        ret = 4;
        wk->model->dataflg = 1;
        break;
    case 17:
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, sbtlCheckEnemyTarget(wk, sbtlGetUseTable_Target_sbtlActAI(wk, codet, arg2)));
        if (unit != NULL) {
            wk->model->flags = (wk->model->flags | 0x14) & ~0x20B;
            SDF_VU0_LOADVECTOR(&wk->max_mvspd);
            SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
            wk->model->mode = 2;
            SDF_VU0_LOADVECTOR(&unit->pos);
            SDF_VU0_STOREVECTOR(wk->model);
            SDF_VU0_LOADVECTOR_B(&wk->pos);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_GETLENGTH(len);
            SDF_VU0_NORMALIZE();
            SDF_VU0_SCALEVECTOR_BITS(len * 2.5f);
            SDF_VU0_LOADVECTOR_B(&wk->pos);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(wk->model);
            len = len / wk->mv_spdbk.z * 2.5f;
            wk->model->frame = 0;
            wk->model->cmpframe = len;
            sbtlSetMotion_sbtlUnit(wk, 5, 4, 0, 0);
            sbtlSetAction(wk, 2, 0);
            wk->model->stat = 2;
            wk->aiwk->stat = 2;
        }
        ret = 4;
        wk->model->dataflg = 1;
        break;
    case 15:
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, sbtlGetUseTable_Target_sbtlActAI(wk, codet, arg2));
        if (unit != NULL) {
            wk->model->flags = (wk->model->flags | 0x14) & ~0x20B;
            SDF_VU0_LOADVECTOR(&wk->max_mvspd);
            SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
            wk->model->mode = 2;
            len = wk->mv_spdbk.z * (f32)arg;
            sbtlGetEscapePoint(wk, unit, &wk->model->vec00, len);
            wk->model->cmpframe = arg;
            wk->model->frame = 0;
            sbtlSetMotion_sbtlUnit(wk, 5, 4, 0, 0);
            sbtlSetAction(wk, 2, 0);
            wk->model->stat = 2;
            wk->aiwk->stat = 2;
        }
        ret = 4;
        wk->model->dataflg = 1;
        break;
    case 16:
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, sbtlGetUseTable_Target_sbtlActAI(wk, codet, arg2));
        if (unit != NULL) {
            wk->model->flags = (wk->model->flags | 0x14) & ~0x20B;
            SDF_VU0_LOADVECTOR(&wk->max_mvspd);
            SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
            wk->model->mode = 2;
            len = wk->mv_spdbk.z * (f32)arg;
            GetSenkaiPointSub(wk, unit, &wk->model->vec00, len);
            wk->model->cmpframe = arg;
            wk->model->frame = 0;
            sbtlSetMotion_sbtlUnit(wk, 5, 4, 0, 0);
            sbtlSetAction(wk, 2, 0);
            wk->model->stat = 2;
            wk->aiwk->stat = 2;
        }
        ret = 4;
        wk->model->dataflg = 1;
        break;
    case 14:
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, sbtlCheckEnemyTarget(wk, sbtlGetUseTable_Target_sbtlActAI(wk, codet, arg2)));
        if (unit != NULL) {
            struct SBTLUNIT_MODEL *model;

            wk->model->flags = (wk->model->flags | 0x14) & ~0x20B;
            SDF_VU0_LOADVECTOR(&wk->max_mvspd);
            SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
            wk->model->mode = 2;
            SDF_VU0_LOADVECTOR(&unit->pos);
            SDF_VU0_STOREVECTOR(wk->model);
            SDF_VU0_LOADVECTOR_B(&wk->pos);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_GETLENGTH(len);
            SDF_VU0_NORMALIZE();
            SDF_VU0_SCALEVECTOR_BITS(len * 0.5f);
            SDF_VU0_LOADVECTOR_B(&wk->pos);
            SDF_VU0_ADDVECTOR();
            model = wk->model;
            SDF_VU0_STOREVECTOR(model);
            len = len * 0.5f / wk->mv_spdbk.z;
            if (len <= (f32)arg) {
                len = arg;
            }
            model->cmpframe = len;
            model->frame = 0;
            sbtlSetMotion_sbtlUnit(wk, 5, 4, 0, 0);
            sbtlSetAction(wk, 2, 0);
            wk->model->stat = 2;
            wk->aiwk->stat = 2;
        }
        ret = 4;
        wk->model->dataflg = 1;
        break;
    case 7:
        wk->model->data_p[1] = arg;
        break;
    case 22:
        wk->model->data_p[0] = arg - 8;
        wk->model->data_p[1] = 0;
        sbtlUseTableDataProc(wk);
        wk->model->dataflg = 1;
        ret = 1;
        break;
    case 12:
        sel = arg;
        for (i = 0; i < 12; i++) {
            if (wk->haveskill[i] == SKLID_ATK || wk->haveskill[i] == SKLID_AVOID || wk->haveskill[i] == 0) {
                continue;
            }
            if (GetPercentage() < 50) {
                sel = wk->haveskill[i];
                break;
            }
        }
        if (i == 10) {
            sel = SKLID_ATK;
        }
        wk->skillid = sel;
        ret = 4;
        wk->model->field104 = sbtlGetUseTable_Target_sbtlActAI(wk, codet, arg2);
        wk->model->field104 = sbtlCheckEnemyTarget(wk, wk->model->field104);
        sbtlSetAIMode(wk);
        wk->model->stat = 1;
        wk->aiwk->stat = 2;
        wk->model->mode = 14;
        wk->model->dataflg = 1;
        break;
    case 9:
        wk->skillid = arg;
        ret = 4;
        wk->model->field104 = sbtlGetUseTable_Target_sbtlActAI(wk, codet, arg2);
        wk->model->field104 = sbtlCheckEnemyTarget(wk, wk->model->field104);
        sbtlSetAIMode(wk);
        wk->model->stat = 1;
        wk->aiwk->stat = 2;
        wk->model->mode = 14;
        wk->model->dataflg = 1;
        break;
    case 10:
        ret = 1;
        wk->model->dataflg = 0;
        wk->model->data_p[0] = 0;
        wk->model->data_p[1] = 0;
        break;
    case 11:
        wk->model->data_p[0] = arg;
        wk->model->data_p[1] = 0;
        break;
    case 19:
        sbtlResetAIPattern(wk, arg);
        ret = 1;
        break;
    case 18:
        wk->model->mode = 12;
        sbtlSetNormalAction(wk, 12, 0, arg, 0);
        ret = 2;
        wk->model->stat = 2;
        wk->aiwk->stat = 2;
        wk->model->dataflg = 1;
        break;
    case 8:
        break;
    default:
        ret = 2;
        break;
    }
    return ret;
}

/* NON_MATCHING: 57 of 403 words match.  */
#ifdef NON_MATCHING
s32 sbtlGetUseTable_Target_sbtlActAI(struct SBTLUNIT_WORK *wk, s32 type, s32 index)
{
    s32 ids[32];
    s16 rates[32];
    struct SBTLUNIT_LISTWORK *list;
    struct SBTLUNIT_WORK *unit;
    struct SBTLUNIT_LISTWORK *ulist;
    s32 id = -1;
    s32 best = 101;
    s32 tid;
    s32 i;
    s32 n;
    s32 sel;
    s16 rate;
    f32 len;
    f32 val;
    f32 ratio;

    switch (type) {
    case 2:
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
        if (unit != NULL && CheckLiveTarget(unit)) {
            return unit->field1A0;
        }
        if (wk_sbtl->field344 <= 0) {
            return -1;
        }
        ulist = wk_sbtl->unitList;
        tid = wk_sbtl->stockUnitID[wk_sbtl->summonIndex[0]];
        goto search;
    case 1:
        if (wk_sbtl->field344 > 0) {
            ulist = wk_sbtl->unitList;
            tid = wk_sbtl->stockUnitID[wk_sbtl->summonIndex[0]];
        } else {
            tid = wk_sbtl->targetUnitID;
            ulist = wk_sbtl->unitList;
        }
        goto search;
    case 3:
        n = 0;
        for (i = 0; i < wk_sbtl->unitList->number; i++) {
            unit = wk_sbtl->unitList->pUnitL[i].pUnit;
            if (unit != NULL && sbtlCheckDamageTarget(unit) && (wk->field414 & 7) == (unit->field414 & 7)) {
                ids[n] = unit->field1A0;
                rates[n] = GetPercentage();
                n++;
            }
        }
        if (n <= 0) {
            return -1;
        }
        for (i = 0; i < n; i++) {
            if (rates[i] <= best) {
                id = ids[i];
                best = rates[i];
            }
        }
        return id;
    case 4:
        n = 0;
        for (i = 0; i < wk_sbtl->unitList->number; i++) {
            unit = wk_sbtl->unitList->pUnitL[i].pUnit;
            if (unit != NULL && sbtlCheckDamageTarget(unit) && (wk->field414 & 7) == (unit->field414 & 7) &&
                wk->field1A0 != unit->field1A0) {
                ids[n] = unit->field1A0;
                rates[n] = GetPercentage();
                n++;
            }
        }
        if (n <= 0) {
            return wk->field1A0;
        }
        for (i = 0; i < n; i++) {
            if (rates[i] <= best) {
                id = ids[i];
                best = rates[i];
            }
        }
        return id;
    case 8:
        list = sbtlCreateSideList(wk->field414, 1);
        if (list->count <= 0) {
            goto fail;
        }
        rate = 49;
        if (list->count == 1) {
            unit = list->pUnitL[0].pUnit;
        } else {
            for (i = 0; i < list->count; i++) {
                unit = list->pUnitL[i].pUnit;
                if (GetPercentage() <= rate) {
                    goto found;
                }
                rate += 50;
            }
            goto fail;
        }
        goto found;
    case 9:
        val = 2000.0f;
        list = sbtlCreateSideList(wk->field414, 1);
        sel = 0;
        for (i = 0; i < list->count; i++) {
            unit = list->pUnitL[i].pUnit;
            SDF_VU0_LOADVECTOR(&wk->pos);
            SDF_VU0_LOADVECTOR_B(&unit->pos);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_GETLENGTH(len);
            if (len <= val) {
                val = len;
                sel = i;
            }
        }
        if (sel >= list->count) {
            goto fail;
        }
        unit = list->pUnitL[sel].pUnit;
        goto found;
    case 7:
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->model->field10C);
        if (unit != NULL && CheckLiveTarget(unit) && !(unit->field414 & wk->field414)) {
            return wk->model->field10C;
        }
        tid = wk_sbtl->targetUnitID;
        ulist = wk_sbtl->unitList;
    search:
        unit = sbtlSearchUnit_ID(ulist, tid);
        if (unit != NULL && CheckLiveTarget(unit)) {
            return unit->field1A0;
        }
        return -1;
    case 10:
        val = 0.0f;
        list = sbtlCreateSideList(wk->field414, 1);
        sel = 0;
        for (i = 0; i < list->count; i++) {
            unit = list->pUnitL[i].pUnit;
            ratio = (f32)unit->unit.now.hp * 100.0f / (f32)unit->unit.now.maxhp;
            if (val < ratio) {
                val = ratio;
                sel = i;
            }
        }
        if (sel >= list->count) {
            goto fail;
        }
        unit = list->pUnitL[sel].pUnit;
        goto found;
    case 11:
        val = 100.0f;
        list = sbtlCreateSideList(wk->field414, 1);
        sel = 0;
        for (i = 0; i < list->count; i++) {
            unit = list->pUnitL[i].pUnit;
            ratio = (f32)unit->unit.now.hp * 100.0f / (f32)unit->unit.now.maxhp;
            if (ratio <= val) {
                val = ratio;
                sel = i;
            }
        }
        if (sel >= list->count) {
            goto fail;
        }
        unit = list->pUnitL[sel].pUnit;
        goto found;
    case 5:
        return wk->model->ai_target[index];
    case 6:
        return wk->field1A0;
    default:
        return -1;
    }
fail:
    sbtlDeleteUnitList(list);
    return -1;
found:
    sbtlDeleteUnitList(list);
    return unit->field1A0;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlGetUseTable_Target_sbtlActAI);
#endif

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

/* NON_MATCHING: 6 of 18 words match.  */
#ifdef NON_MATCHING
void sbtlRefSbtl21(struct SBTLUNIT_WORK *wk) {
    struct SBTLAI_WORK *aiwk = wk->aiwk;

    if (aiwk->status & 1) {
        aiwk->status &= ~1;
        aiwk->status &= ~8;
        aiwk->meirei_nobk = 7;
        aiwk->status &= ~0x10;
        wk_sbtl->panel->meirei_no = 7;
    }
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlRefSbtl21);
#endif

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

/* body: matched in raidou @ 61f09dc: src/raidou1/code_002929C8.c:124, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
s32 sbtlLeaf68(void) {
    return 1;
}

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

INCLUDE_ASM(const s32, "sbtlActAI", sbtlEAIProc_Ctact);

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

s32 sbtlEAIProc_Escape(struct SBTLUNIT_WORK *wk) {
    return 1;
}

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

s32 sbtlSetSearchEnemyMode(struct SBTLUNIT_WORK *wk, s32 targetid) {
    wk->model->field118 = 1;
    sbtlSetGroupAction(wk, targetid, 12, 0);
    sbtlResetAIPattern(wk, 0);
    return 1;
}

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

s32 sbtlMVSupport(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_WORK *wklock;
    s32 lockID;
    s32 count;
    s32 i;
    s32 r;
    f32 bez;
    f32 len;
    f32 len2;
    f32 ground;
    f32 y;

    count = 0;
    lockID = sbtlSetNoMP(wk);
    wklock = sbtlSearchUnit_ID(wk_sbtl->unitList, lockID);
    if ((!(wk_sbtl->flg & 0x40) || (wk->flg & 0x10)) && SetUnitHitApproach(wk)) {
        if (wk->model->flags & 0x10) {
            sbtlGetRotate(wk, &wk->model->vec00);
            UnitMV(wk, &wk->model->vec00);
        }
        if ((wk->model->flags & 1) && lockID != wk->field1A0 && wklock != NULL && sbtlCheckExist(wklock)) {
            if (!sbtlCheckCrossCircle(&wk->pos, wk->model->field120, &wklock->pos, wklock->model->field11C)) {
                sbtlGetRotate(wk, &wklock->pos);
                UnitMV(wk, &wklock->pos);
            }
        }
        if ((wk->model->flags & 0x200) && lockID != wk->field1A0 && wklock != NULL && sbtlCheckExist(wklock)) {
            sbtlGetRotate(wk, &wklock->pos);
            UnitMV(wk, &wklock->pos);
        }
        if ((wk->model->flags & 8) && lockID != wk->field1A0 && wklock != NULL && sbtlCheckExist(wklock)) {
            wk->field424 = wklock->field424;
            SDF_VU0_LOADVECTOR(&wklock->rot);
            SDF_VU0_STOREVECTOR(&wk->rot);
            wk->angleY = wklock->angleY;
        }
        if ((wk->model->flags & 2) && lockID != wk->field1A0 && wklock != NULL && sbtlCheckExist(wklock)) {
            if (sbtlCheckCrossCircle(&wk->pos, wk->model->field124, &wklock->pos, wklock->model->field120)) {
                wk->field424 = wklock->field424;
                wk->angleY = wklock->angleY;
                SDF_VU0_LOADVECTOR(&wklock->mv_spd);
                SDF_VU0_STOREVECTOR(&wk->mv_spd);
            }
        }
        if ((wk->vstatus & 0x800) && !(wk->flg & 0x800)) {
            if (wk->flg & 0x10000000) {
                bez = 0.0f;
            } else {
                switch (wk->visualKind) {
                case 0:
                    bez = sbtlGetBezier(wk->model->hitbk_rate, wk->model->hitbk_rate * 0.8f,
                                        wk->model->hitbk_rate * 0.6f, 0.0f, wk->model->hitbk_cnt);
                    break;
                case 1:
                    bez = sbtlGetBezier(wk->model->hitbk_rate, wk->model->hitbk_rate * 0.6f,
                                        wk->model->hitbk_rate * 0.3f, 0.0f, wk->model->hitbk_cnt);
                    break;
                case 2:
                    bez = sbtlGetBezier(wk->model->hitbk_rate, wk->model->hitbk_rate * 0.3f,
                                        wk->model->hitbk_rate * 0.1f, 0.0f, wk->model->hitbk_cnt);
                    break;
                default:
                    bez = sbtlGetBezier(wk->model->hitbk_rate, wk->model->hitbk_rate * 0.1f,
                                        wk->model->hitbk_rate * 0.1f, 0.0f, wk->model->hitbk_cnt);
                    break;
                }
            }
            if (bez > 40.0f) {
                bez = 40.0f;
            }
            if (bez < -40.0f) {
                bez = -40.0f;
            }
            SDF_VU0_LOADVECTOR(&wk->model->hitbk_dir);
            SDF_VU0_SCALEVECTOR_BITS(bez);
            SDF_VU0_LOADVECTOR_B(&wk->pos);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR_F32(&wk->pos);
            for (i = 0; i < wk_sbtl->unitList->number; i++) {
                wklock = wk_sbtl->unitList->pUnitL[i].pUnit;
                if (wklock == NULL || wklock->field1A0 == wk->field1A0) {
                    continue;
                }
                if (sbtlBtlMVSupportSub2(wk, wklock) || !sbtlCheckExist2_sbtlUnit(wklock)) {
                    continue;
                }
                if ((wk->flg & 1) || !sbtlBtlMVSupportSub(wk, wklock)) {
                    continue;
                }
                if (!sbtlCheckHITCheck_Char(wk) || !sbtlCheckHITCheck_Char(wklock) || (wklock->flg & 1)) {
                    continue;
                }
                if (!sbtlCheckCrossCircle_Rot(&wk->pos, &wk->rot, wk->model->field11C, &wklock->pos,
                                              wklock->model->field11C, -1.0f)) {
                    continue;
                }
                sbtlSetPullPos(wk, wk->model->field11C, wklock, wklock->model->field11C,
                               sbtlFree(wklock->pos.x, wklock->pos.z));
                SDF_VU0_STOREVECTOR(&wk->pos);
                if (++count >= 2) {
                    SDF_VU0_LOADVECTOR(&wk->nextpos);
                    SDF_VU0_STOREVECTOR(&wk->pos);
                    break;
                }
            }
            wk->model->hitbk_cnt += wk->model->hitbk_cmpcnt;
            if (wk->model->hitbk_cnt >= 1.0f) {
                wk->vstatus &= ~0x800;
            }
        }
    }
    SDF_VU0_LOADVECTOR(&wk->mv_spd);
    SDF_VU0_NORMALIZE();
    SDF_VU0_GETLENGTH(len);
    if (PosOut(wk)) {
        wk->field4A0++;
        wk->field4A0 = (wk->field4A0 <= 0) ? 0 : (wk->field4A0 > 20) ? 20 : wk->field4A0;
    }
    if (wk->model->flags & 4) {
        for (i = 0; i < wk_sbtl->unitList->number; i++) {
            wklock = wk_sbtl->unitList->pUnitL[i].pUnit;
            if (wklock == NULL || wklock->field1A0 == wk->field1A0) {
                continue;
            }
            if (sbtlBtlMVSupportSub2(wk, wklock) || !sbtlCheckExist2_sbtlUnit(wklock)) {
                continue;
            }
            if ((wk->flg & 1) || !sbtlBtlMVSupportSub(wk, wklock)) {
                continue;
            }
            if (!sbtlCheckHITCheck_Char(wk) || !sbtlCheckHITCheck_Char(wklock) || (wklock->flg & 1)) {
                continue;
            }
            if ((wk->vstatus & 0x200) && !(wklock->vstatus & 0x200)) {
                continue;
            }
            if (wk->flg & 0x80000) {
                SDF_VU0_LOADVECTOR(&wk->posSaved);
                SDF_VU0_STOREVECTOR(&wk->pos);
            }
            SDF_VU0_LOADVECTOR(&wklock->mv_spd);
            SDF_VU0_NORMALIZE();
            SDF_VU0_GETLENGTH(len2);
            if (!sbtlCheckCrossCircle_Rot(&wk->pos, &wk->rot, wk->model->field11C, &wklock->pos,
                                          wklock->model->field11C, -1.0f)) {
                continue;
            }
            if (sbtlCheckActDamage(wk, wklock)) {
                continue;
            }
            if (wk->model->flags & 0x20) {
                r = sbtlSetPushPos(wk, wk->model->field11C, wklock, wklock->model->field11C,
                                   sbtlFree(wklock->pos.x, wklock->pos.z));
                SDF_VU0_STOREVECTOR(&wklock->pos);
                if (!r) {
                    sbtlPrivAreaCheck(wklock, 1);
                }
            } else if (len == 0.0f && (wk->model->flags & 0x40) && !wk->field415) {
                r = sbtlSetPushPos(wk, wk->model->field11C, wklock, wklock->model->field11C,
                                   sbtlFree(wklock->pos.x, wklock->pos.z));
                SDF_VU0_STOREVECTOR(&wklock->pos);
                if (!r) {
                    sbtlPrivAreaCheck(wklock, 1);
                }
            } else if (!PosOut(wk)) {
                if (len > 0.0f) {
                    if (!(wklock->flg & 0x80)) {
                        wklock->mv_spd.z = 0.0f;
                        SDF_VU0_LOADVECTOR(&wklock->nextpos);
                        SDF_VU0_STOREVECTOR(&wklock->pos);
                    }
                    sbtlSetPullPos(wk, wk->model->field11C, wklock, wklock->model->field11C,
                                   sbtlFree(wklock->pos.x, wklock->pos.z));
                    SDF_VU0_STOREVECTOR(&wk->pos);
                } else {
                    sbtlSetPullPos(wk, wk->model->field11C, wklock, wklock->model->field11C,
                                   sbtlFree(wklock->pos.x, wklock->pos.z));
                    SDF_VU0_STOREVECTOR(&wk->pos);
                }
            } else if (sbtlCheckCrossCircle_Rot(&wk->pos, &wk->rot, wk->model->field11C, &wklock->pos,
                                                wklock->model->field11C, 0.35f) &&
                       wk->field4A0 < 21) {
                if (!(wk->flg & 0x80)) {
                    wk->mv_spd.z = 0.0f;
                    SDF_VU0_LOADVECTOR(&wk->nextpos);
                    SDF_VU0_STOREVECTOR(&wk->pos);
                }
            } else {
                sbtlSetPullPos(wk, wk->model->field11C, wklock, wklock->model->field11C,
                               sbtlFree(wklock->pos.x, wklock->pos.z));
                SDF_VU0_STOREVECTOR(&wk->pos);
            }
        }
        if (!(wk->vstatus & 0x800000) && wk->status != 35) {
            sbtlFree(wk->pos.x, wk->pos.z);
            if (wk->pos.y < sbtlFree(wk->pos.x, wk->pos.z) && (wk->vstatus & 0x400)) {
                wk->field474 = 0;
                wk->vstatus = (wk->vstatus | 0x80) & ~0x400;
            }
        }
    }
    wk->field4CC = 0;
    ground = sbtlFree(wk->pos.x, wk->pos.z);
    if (!(wk_sbtl->flg & 0x40)) {
        if ((wk->flg & 0x20) || (wk->vstatus & 0x80)) {
            wk->field474++;
            wk->field474 = (wk->field474 <= 0) ? 0 : (wk->field474 > 0x7FFF) ? 0x7FFF : wk->field474;
            y = wk->pos.y + (f32)wk->field474 * 0.1633f * (f32)wk->field474 * 0.5f;
            if (ground <= y) {
                wk->field474 = 0;
                y = ground;
                wk->vstatus &= ~0x80;
            }
            wk->pos.y = y;
        } else if (!(wk->flg & 0x200000) && !sbtlBtlMVSupportSub2(wk, wk) && !(wk->vstatus & 0x800000)) {
            wk->pos.y = sbtlFree(wk->pos.x, wk->pos.z);
        }
    }
    if (wk->flg & 0x80000) {
        SDF_VU0_LOADVECTOR(&wk->posSaved);
        SDF_VU0_STOREVECTOR(&wk->pos);
    }
    return 1;
}

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

/* body: matched in raidou @ 61f09dc: src/raidou1/sbtlEff_00289E50.c:75, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void sbtlLeaf69(dds3ProcessID_t *id) {
}

/* body: matched in raidou @ 61f09dc: src/raidou1/sbtlEff_00289E50.c:75, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void sbtlLeaf70(dds3ProcessID_t *id) {
}

/* body: matched in raidou @ 61f09dc: src/raidou1/sbtlEff_00289E50.c:75, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void sbtlLeaf71(dds3ProcessID_t *id) {
}

/* body: matched in raidou @ 61f09dc: src/raidou1/sbtlEff_00289E50.c:75, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void sbtlLeaf72(dds3ProcessID_t *id) {
}

void sbtlSetNoMP_sbtlActAI(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_MODEL *model = wk->model;

    if (model->nonempflg == 0 && (wk->field414 & 2)) {
        model->nonempflg = 1;
        sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_NOTMP, wk->skillid);
        sbtlResetAIPattern(wk, 0);
    }
}

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
        sbtlSetAction(wk, 1, 0);
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
        sbtlSetAction(wk, 2, 0);
        break;
    case SBTL_ACTAIMODE_GUARD:
        wk->model->cmpframe = time;
        wk->model->mode = 3;
        wk->model->frame = 0;
        SDF_VU0_LOADVECTOR(&zero);
        SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
        wk->model->flags = (wk->model->flags | 4) & ~0x21B;
        sbtlSetMotion_sbtlUnit(wk, 29, 0, 0, 0);
        sbtlSetAction(wk, 2, 0);
        break;
    case SBTL_ACTAIMODE_CTACT:
        wk->model->flags = (wk->model->flags | 5) & ~0x21A;
        SDF_VU0_LOADVECTOR(&wk->max_mvspd);
        SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
        wk->model->cmpframe = 1200;
        wk->model->mode = 4;
        wk->model->frame = 0;
        sbtlSetMotion_sbtlUnit(wk, 5, 4, 0, 0);
        sbtlSetAction(wk, 2, 0);
        break;
    case SBTL_ACTAIMODE_SEARCH:
        wk->model->flags = (wk->model->flags | 4) & ~0x21B;
        SDF_VU0_LOADVECTOR(&zero);
        SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
        wk->model->cmpframe = 180;
        wk->model->mode = 5;
        wk->model->frame = 0;
        sbtlSetMotion_sbtlUnit(wk, 0, 4, 0, 0);
        sbtlSetAction(wk, 1, 0);
        break;
    case SBTL_ACTAIMODE_CHASE:
        wk->model->flags = (wk->model->flags | 0x204) & ~0x1B;
        SDF_VU0_LOADVECTOR(&wk->max_mvspd);
        SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
        wk->model->cmpframe = time;
        wk->model->mode = 6;
        wk->model->frame = 0;
        sbtlSetMotion_sbtlUnit(wk, 5, 4, 0, 0);
        sbtlSetAction(wk, 2, 0);
        break;
    case SBTL_ACTAIMODE_BSTWAIT:
        wk->model->flags = (wk->model->flags | 4) & ~0x21B;
        SDF_VU0_LOADVECTOR(&zero);
        SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
        wk->model->cmpframe = time;
        wk->model->mode = 1;
        wk->model->frame = 0;
        sbtlSetMotion_sbtlUnit(wk, 2, 4, 0, 0);
        sbtlSetAction(wk, 1, 0);
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

INCLUDE_ASM(const s32, "sbtlActAI", sbtlGetEscapePoint);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlCreateSideListSBtlUnit3);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlGetMovePos_MapHit);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlGetMovePos_MapHit_2);

s32 sbtlCheckMapHit(const sdfVector4_t *pos0, const sdfVector4_t *ppos, f32 dis) {
    return sbtlCheckMapHit_Cross(pos0, ppos, dis);
}

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

s32 sbtlAAIInit_Free(struct SBTLUNIT_WORK *wk) {
    return 1;
}

s32 sbtlCallBtlGetWarpUnitID(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_MODEL *model;
    struct SBTLUNIT_WORK *unit;

    wk->model->mode = 2;
    wk->model->flags = (wk->model->flags | 5) & ~0x21A;
    wk->model->field100 = sbtlGetPlayerID();
    SDF_VU0_LOADVECTOR(&wk->max_mvspd);
    SDF_VU0_STOREVECTOR(&wk->mv_spdbk);
    model = wk->model;
    model->mode = 4;
    model->frame = 0;
    model->cmpframe = 0;
    unit = sbtlGetWarpUnit_ID(wk);
    sbtlSetWalk(wk, wk->model->field120 + unit->model->field11C);
    wk->model->stat = 2;
    return 1;
}

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

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_N_GardeT);

/* NON_MATCHING: 4 of 41 words match.  */
#ifdef NON_MATCHING
s32 sbtlAAIInit_N_Renkei_sbtlActAI(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_MODEL *model = wk->model;
    sdfVector4_t *rot = &wk->rot;
    struct SBTLUNIT_WORK *unit;
    f32 deg;

    model->mode = 2;
    model->flags &= ~ACTAIFLGMASK;
    sbtlSetMotion_sbtlUnit(wk, 30, 0, 0, 1);
    unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    deg = sbtlGetChorikiDeg(&wk->pos, rot, &unit->pos, 1.0f);
    SDF_VU0_STOREVECTOR(rot);
    sbtlAddUnitDeg(wk, deg);
    wk->model->stat = 2;
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_N_Renkei_sbtlActAI);
#endif

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_N_Renkei);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_N_Comb_R);

s32 sbtlAAIInit_N_Return(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_MODEL *model = wk->model;

    model->mode = 2;
    model->stat = 2;
    model->flags &= ~ACTAIFLGMASK;
    return 1;
}

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

/* NON_MATCHING: 39 of 41 words match.  */
#ifdef NON_MATCHING
s32 sbtlAAIInit_BAttack(struct SBTLUNIT_WORK *wk) {
    sdfVector4_t zero;
    struct SBTLUNIT_MODEL *model;

    memset(&zero, 0, sizeof(zero));
    wk->model->flags = (wk->model->flags | 0x205) & ~0x1A;
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

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIInit_Skl_S);

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

void sbtlSetWalk(struct SBTLUNIT_WORK *wk, f32 length) {
    if (sbtlSetMotion(wk, SBTLUNIT_MOTION_WALK) && !(wk->field414 & 2)) {
        sbtlSetMotion_sbtlUnit(wk, SBTLUNIT_MOTION_WALK, 4, 0, 0);
        sbtlSetAction(wk, 2, 0);
    }
}

s32 sbtlBtlAAIProcNKaihiSub(struct SBTLUNIT_WORK *wk) {
    return 0;
}

s32 sbtlAAIProc_Free(struct SBTLUNIT_WORK *wk) {
    return 1;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlResetAAI);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_N_Wait);

s32 sbtlAAIProc_N_Sabato(struct SBTLUNIT_WORK *wk) {
    wk->actCounter++;
    wk->actCounter = (wk->actCounter <= 0) ? 0 : (wk->actCounter < wk->actMax) ? wk->actCounter : wk->actMax;
    return 1;
}

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_N_Renkei);

INCLUDE_ASM(const s32, "sbtlActAI", sbtlAAIProc_N_Comb_E);

s32 sbtlCallBtlSearchUnitID13(struct SBTLUNIT_WORK *wk) {
    if (sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID)->model->flags & 0x4000) {
        return 0;
    }
    wk->flg &= ~0x4010;
    sbtlResetAAI(wk);
    if (wk_sbtl->flg & 0x40000) {
        wk_sbtl->flg &= ~0x40000;
    }
    wk->aiwk->mode = 0;
    sbtlSetMotion_sbtlUnit(wk, 0, 4, 0, 0);
    return 1;
}

s32 sbtlAAIProc_N_Return(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_WORK *unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);

    if (unit != NULL && sbtlCheckDamageTarget(unit) != 0) {
        sbtlSetReturn(unit);
    }
    return 1;
}

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
                sbtlSetUseSkill_sbtlActAI(wk, wk->skillid, 0);
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

s32 Proc_sbtlAction(struct SBTLUNIT_WORK *wk) {
    sbtlResetAAI(wk);
    return 1;
}

/* NON_MATCHING: 174 of 3008 words match.  */
#ifdef NON_MATCHING
s32 Proc(struct SBTLUNIT_WORK *wk) {
    sdfVector4_t zero;
    sdfVector4_t dir;
    sdfVector4_t posid;
    sdfVector4_t pos;
    sdfVector4_t rot;
    sdfVector4_t oldpos;
    sdfVector4_t oldrot;
    SBTLVISUAL_DATA data;
    struct SBTLUNIT_WORK *unit;
    struct SBTLUNIT_MODEL *model;
    struct SBTLSKILLP_TBL *tbl;
    f32 oldangleY;
    f32 angle;
    f32 len;
    s32 frame;
    s32 devil;
    s32 i;
    s32 ret;
    BST bst;

    memset(&zero, 0, sizeof(zero));
    if (!(wk->field414 & 0x80) && !(wk->flg & 0x800000) && !(wk->vstatus & 0x4000)) {
        SDF_VU0_LOADVECTOR(&zero);
        SDF_VU0_STOREVECTOR(&wk->mv_spd);
    }
    if ((wk_sbtl->battleMode != 3) ? (wk->field414 & 0x80) : (wk->field414 & 1)) {
        SDF_VU0_LOADVECTOR(&wk->pos);
        SDF_VU0_STOREVECTOR(&oldpos);
    } else {
        SDF_VU0_LOADVECTOR(&wk->pos);
        SDF_VU0_STOREVECTOR(&oldpos);
        SDF_VU0_STOREVECTOR(&wk->nextpos);
    }
    if ((wk_sbtl->flg & 0x40) && !(wk->flg & 0x810)) {
        wk->field424 = 0.0f;
        sbtlMVSupport(wk);
        sbtlCallMemset5(wk);
        return 0;
    }
    if (wk->field4A2 > 0) {
        wk->field4A2--;
        if (wk->field4A2 <= 0) {
            wk->field4A2 = 0;
            wk->damage_hitcnt = 0;
        }
    }
    wk->damage_actionf--;
    if (wk->damage_actionf <= 0) {
        wk->damage_actionf = 0;
    }
    if (wk->field4A2 > 0) {
        sbtlCallSin(wk);
    }
    wk->framecnt++;
    wk->framecnt = (wk->framecnt <= 0) ? 0 : (wk->framecnt > 0x7FFF) ? 0x7FFF : wk->framecnt;
    wk->aiwk->syscnt_stop++;
    wk->aiwk->syscnt_stop = (wk->aiwk->syscnt_stop <= 0)  ? 0
                            : (wk->aiwk->syscnt_stop > 99999) ? 99999
                                                              : wk->aiwk->syscnt_stop;
    SDF_VU0_LOADVECTOR(&wk->rot);
    SDF_VU0_STOREVECTOR(&oldrot);
    oldangleY = wk->angleY;
    if ((wk->field414 & 1) && !(wk->field414 & 0x80) && wk->aiwk->mode != 17) {
        wk->flg &= ~0x100;
    }
    sbtlCheckMotionMove(wk);
    if ((wk->field414 & 0x82) == 2) {
        sbtlProcScript(wk);
    }
    if (wk != NULL && CheckLiveTarget(wk) && !(wk_sbtl->flg & 8) && !(wk->model->flags & 0x4000) &&
        ((wk->flg & 0x1000) || !(wk->flg & 0x5000)) && !(wk_sbtl->flg & 0x1000000) && !(wk_sbtl->flg2 & 2) &&
        wk->status != 42 && !sbtlRefSbtl22(wk)) {
        CalcBadStatus(wk);
        if (wk->unit.now.hp > 0) {
            CalcBtlMagicStatus(wk);
        }
    }
    switch (wk->status) {
    case 19:
        sbtlProc(wk);
        sbtlMVSupport(wk);
        CheckMapHit(wk, &oldpos);
        sbtlCallMemset5(wk);
        if (!sbtlCallBtlSearchEffectSABActer(wk)) {
            return 1;
        }
        sbtlSetAction(wk, 0, 0);
        return 1;
    case 0:
    case 1:
    case 2:
    case 42:
        sbtlCallMemset12(wk);
        sbtlProc(wk);
        sbtlMVSupport(wk);
        CheckMapHit(wk, &oldpos);
        sbtlCallMemset5(wk);
        return 1;
    case 35:
        wk->m_rate = 0.0f;
        sbtlMVSupport(wk);
        CheckMapHit(wk, &oldpos);
        sbtlCallMemset5(wk);
        return 1;
    case 33:
        sbtlMVSupport(wk);
        CheckMapHit(wk, &oldpos);
        sbtlCallMemset5(wk);
        if (wk->unit.badStatus & 0x80) {
            return 1;
        }
        sbtlRestartMotion(wk);
        sbtlSetAction(wk, 2, 0);
        wk->aiwk->stat = 1;
        wk->flg &= ~0x20;
        return 1;
    case 3:
        if (wk->framecnt >= (s32)sbtlCallBtlGetMotionVisualData4(wk, 0) && !wk->action_flg) {
            wk->action_flg = 1;
            sbtlSetUseSkill(wk, wk->skillid);
            if (sbtlSearchEffectData(wk->skillid) != -1) {
                sbtlCallBtlGetSklVisualTbl2(wk, wk->skillid);
            } else {
                sbtlForceDelete_SkillFlag_Acter(wk, 0x400);
            }
        }
        sbtlCallMemset12(wk);
        sbtlProc(wk);
        sbtlMVSupport(wk);
        CheckMapHit(wk, &oldpos);
        sbtlCallMemset5(wk);
        if (!sbtlCallBtlSearchEffectSABActer(wk)) {
            return 1;
        }
        wk->status = 2;
        sbtlBS_MUNAKA_Init_Imad_Restart(wk);
        sbtlEndAction(wk);
        return 1;
    case 21:
        sbtlCallMemset12(wk);
        sbtlProc(wk);
        sbtlMVSupport(wk);
        CheckMapHit(wk, &oldpos);
        sbtlCallMemset5(wk);
        if (!sbtlCallBtlSearchEffectSABActer(wk)) {
            return 1;
        }
        wk->status = 2;
        sbtlBS_MUNAKA_Init_Imad_Restart(wk);
        return 1;
    case 37:
        if (wk->framecnt >= (s32)sbtlCallBtlGetMotionVisualData4(wk, 0) && !wk->action_flg) {
            wk->action_flg = 1;
            sbtlSetUseSkill(wk, wk->skillid);
            if ((wk->vstatus & 0x4000) && wk->skillid == SKLID_OFS_PROGRAM) {
                sbtlCallBtlGetSklVisualTbl2(wk, SKLID_OFS_PROGRAM);
            } else if (sbtlSearchEffectData(wk->skillid) != -1) {
                sbtlCallBtlGetSklVisualTbl2(wk, wk->skillid);
            } else {
                sbtlForceDelete_SkillFlag_Acter(wk, 0x400);
            }
        }
        sbtlMVSupport(wk);
        CheckMapHit(wk, &oldpos);
        sbtlCallMemset5(wk);
        if (!sbtlCallBtlSearchEffectSABActer(wk) && wk->action_flg != 1) {
            return 1;
        }
        sbtlSetAction(wk, 2, 0);
        return 1;
    case 6:
        if (wk->framecnt >= (s32)sbtlCallBtlGetMotionVisualData4(wk, 0) && !wk->action_flg) {
            wk->action_flg = 1;
            sbtlSetUseSkill(wk, wk->skillid);
            if (sbtlSearchEffectData(wk->skillid) != -1) {
                sbtlCallBtlGetSklVisualTbl2(wk, wk->skillid);
            } else {
                sbtlForceDelete_SkillFlag_Acter(wk, 0x400);
            }
        }
        goto end_proc;
    case 47:
        if (wk->framecnt >= (s32)sbtlCallBtlGetMotionVisualData4(wk, 0) && !wk->action_flg) {
            wk->action_flg = 1;
            sbtlSetUseSkill(wk, wk->skillid);
            if (sbtlSearchEffectData(wk->skillid) != -1) {
                sbtlCallBtlGetSklVisualTbl2(wk, wk->skillid);
            } else {
                sbtlForceDelete_SkillFlag_Acter(wk, 0x400);
            }
        }
        sbtlCallMemset12(wk);
        sbtlProc(wk);
        sbtlMVSupport(wk);
        CheckMapHit(wk, &oldpos);
        sbtlCallMemset5(wk);
        if (!sbtlCallBtlSearchEffectSABActer(wk)) {
            return 1;
        }
        sbtlBS_MUNAKA_Init_Imad_Restart(wk);
        sbtlSetAction(wk, 2, 0);
        return 1;
    case 38:
        if (wk->framecnt >= (s32)sbtlCallBtlGetMotionVisualData4(wk, 0) && !wk->action_flg) {
            wk->action_flg = 1;
            if (sbtlSearchEffectData(SKLID_BS14_GIGATO_P) != -1) {
                sbtlCallBtlGetSklVisualTbl2(wk, SKLID_BS14_GIGATO_P);
            } else {
                sbtlForceDelete_SkillFlag_Acter(wk, 0x400);
            }
        }
        if (wk->action_flg == 1 && sbtlCheckMotionEnd(wk)) {
            wk->action_flg = 2;
            sbtlCallBtlGetMotionVisualData(wk, 21, 0, 0, 0);
            wk->framecnt = 0;
            wk->framecntB = 0;
        }
        if (wk->action_flg == 2 && wk->framecnt >= wk->framecntB) {
            wk->action_flg = 3;
            sbtlCallBtlGetMotionVisualData(wk, 22, 0, 0, 0);
            wk->framecnt = 0;
        }
        if (wk->framecnt >= (s32)sbtlCallBtlGetMotionVisualData4(wk, 0) && wk->action_flg == 3) {
            wk->action_flg = 4;
            sbtlSetUseSkill(wk, SKLID_BS14_GIGATO);
            if (sbtlSearchEffectData(SKLID_BS14_GIGATO) != -1) {
                sbtlCallBtlGetSklVisualTbl2(wk, SKLID_BS14_GIGATO);
            } else {
                sbtlForceDelete_SkillFlag_Acter(wk, 0x400);
            }
        }
        sbtlCallMemset12(wk);
        sbtlProc(wk);
        sbtlMVSupport(wk);
        CheckMapHit(wk, &oldpos);
        sbtlCallMemset5(wk);
        if (wk->action_flg != 4) {
            return 1;
        }
        goto end_check;
    case 31:
        if (wk->framecnt >= (s32)sbtlCallBtlGetMotionVisualData4(wk, 0) && !wk->action_flg) {
            wk->action_flg = 1;
            sbtlSetUseSkill(wk, wk->skillid);
            if (sbtlSearchEffectData(wk->skillid) != -1) {
                sbtlCallBtlGetSklVisualTbl2(wk, wk->skillid);
            } else {
                sbtlForceDelete_SkillFlag_Acter(wk, 0x400);
            }
        }
        sbtlMVSupport(wk);
        CheckMapHit(wk, &oldpos);
        sbtlCallMemset5(wk);
        if (!sbtlCallBtlSearchEffectSABActer(wk)) {
            return 1;
        }
        sbtlBS_MUNAKA_Init_Imad_Restart(wk);
        sbtlSetUseSkill_sbtlActAI(wk, SKLID_NONE, 0);
        sbtlEndAction(wk);
        return 1;
    case 39:
        if (wk->unit.devilId == 269 || wk->unit.devilId == 304) {
            return 1;
        }
        unit = wk_sbtl->scrUnitList[2];
        if (wk->framecnt >= (s32)sbtlCallBtlGetMotionVisualData4(wk, 0) && !wk->action_flg) {
            wk->action_flg = 1;
            sbtlSetUseSkill(wk, wk->skillid);
            if (sbtlSearchEffectData(wk->skillid) != -1) {
                sbtlCallBtlGetSklVisualTbl2(wk, wk->skillid);
            } else {
                sbtlForceDelete_SkillFlag_Acter(wk, 0x400);
            }
        }
        if (wk->action_flg == 1 && sbtlCheckMotionEnd(wk)) {
            wk->action_flg = 2;
            sbtlCallBtlGetMotionVisualData(wk, 22, 0, 0, 0);
            sbtlCallBtlGetMotionVisualData(unit, 22, 0, 0, 0);
            wk->framecnt = 0;
            wk_sbtl->scrCnt[1] = 0;
            wk_sbtl->scrCnt[0] = 0;
        }
        if (wk->action_flg < 2) {
            return 1;
        }
        SDF_VU0_LOADVECTOR(&sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID)->pos);
        model = wk->model;
        SDF_VU0_LOADVECTOR_B(&model->scrPos);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_STOREVECTOR(&dir);
        switch (wk->aiwk->scrStep2) {
        case 0:
            frame = wk_sbtl->scrCnt[0];
            if (frame < 120) {
                SDF_VU0_LOADVECTOR(&dir);
                SDF_VU0_NORMALIZE();
                SDF_VU0_SCALEVECTOR_BITS(-5.0f);
                SDF_VU0_LOADVECTOR_B(&model->scrPos);
                SDF_VU0_ADDVECTOR();
                SDF_VU0_STOREVECTOR(&wk->model->scrPos);
            } else if (frame == 120) {
                SDF_VU0_LOADVECTOR(&dir);
                SDF_VU0_NORMALIZE();
                SDF_VU0_STOREVECTOR(&model->scrPos2);
            } else if (frame < 150) {
                SDF_VU0_LOADVECTOR(&model->scrPos2);
                SDF_VU0_SCALEVECTOR_BITS((f32)(wk_sbtl->scrCnt[0] - 120) * 1.5f);
                SDF_VU0_LOADVECTOR_B(&wk->model->scrPos);
                SDF_VU0_ADDVECTOR();
                SDF_VU0_STOREVECTOR(&wk->model->scrPos);
            } else if (frame < 220) {
                SDF_VU0_LOADVECTOR(&model->scrPos2);
                SDF_VU0_SCALEVECTOR_BITS(45.0f);
                SDF_VU0_LOADVECTOR_B(&wk->model->scrPos);
                SDF_VU0_ADDVECTOR();
                SDF_VU0_STOREVECTOR(&wk->model->scrPos);
            } else {
                wk->aiwk->scrStep2 = 1;
                wk_sbtl->scrCnt[0] = 0;
            }
            break;
        case 1:
            SDF_VU0_LOADVECTOR(&dir);
            SDF_VU0_SCALEVECTOR_BITS(0.0005f);
            SDF_VU0_LOADVECTOR_B(&model->scrPos2);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(&wk->model->scrPos2);
            model = wk->model;
            model->scrPos2.y = 0.0f;
            SDF_VU0_LOADVECTOR(&model->scrPos2);
            SDF_VU0_GETLENGTH(len);
            if (len > 16.0f) {
                SDF_VU0_NORMALIZE();
                SDF_VU0_SCALEVECTOR_BITS(16.0f);
                SDF_VU0_STOREVECTOR(&model->scrPos2);
            }
            SDF_VU0_LOADVECTOR_B(&model->scrPos);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(&wk->model->scrPos);
            break;
        case 2:
            SDF_VU0_LOADVECTOR(&dir);
            SDF_VU0_SCALEVECTOR_BITS(0.003f);
            SDF_VU0_LOADVECTOR_B(&model->scrPos2);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(&wk->model->scrPos2);
            model = wk->model;
            model->scrPos2.y = 0.0f;
            SDF_VU0_LOADVECTOR(&model->scrPos2);
            SDF_VU0_GETLENGTH(len);
            if (len > 5.0f) {
                SDF_VU0_NORMALIZE();
                SDF_VU0_SCALEVECTOR_BITS(5.0f);
                SDF_VU0_STOREVECTOR(&model->scrPos2);
            }
            SDF_VU0_LOADVECTOR_B(&model->scrPos);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(&wk->model->scrPos);
            break;
        case 3:
            SDF_VU0_LOADVECTOR(&model->vec30);
            SDF_VU0_LOADVECTOR_B(&wk->model->vec00);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_SCALEVECTOR_BITS((f32)(wk_sbtl->scrCnt[1] - wk_sbtl->scrCmpCnt) / 30.0f);
            SDF_VU0_LOADVECTOR_B(&wk->model->vec00);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(&wk->model->scrPos);
            if (wk_sbtl->scrCnt[1] >= wk_sbtl->scrCmpCnt + 30) {
                wk->status = 0;
                unit->status = 0;
                sbtlProcSub(wk_sbtl->effList, 441, 1);
                sbtlEndAction(wk);
                wk->aiwk->scrStep = 7;
            }
            break;
        }
        SDF_VU0_LOADVECTOR(&wk->model->scrPos);
        SDF_VU0_STOREVECTOR(&wk->pos);
        SDF_VU0_STOREVECTOR(&unit->pos);
        wk->pos.x += 140.0f;
        unit->pos.x -= 140.0f;
        dds3SetPosPoseSet_dds3Basic(wk->pBasic, &wk->pos);
        dds3SetPosPoseSet_dds3Basic(unit->pBasic, &unit->pos);
        return 1;
    case 43:
        frame = 8;
        if (wk->framecnt <= frame) {
            SDF_VU0_LOADVECTOR(&wk->model->scrRot);
            SDF_VU0_LOADVECTOR_B(&wk->model->scrRot2);
            Slerp((f32)wk->framecnt / (f32)frame);
            SDF_VU0_STOREVECTOR(&wk->rot);
            sbtlSetRotateDeg(wk);
        }
        if (!wk->action_flg) {
            wk->action_flg = 1;
            wk->flg |= 0x4001;
            sbtlSetUseSkill(wk, wk->skillid);
            if (sbtlSearchEffectData(wk->skillid) != -1) {
                sbtlCallBtlGetSklVisualTbl2(wk, wk->skillid);
            } else {
                sbtlForceDelete_SkillFlag_Acter(wk, 0x400);
            }
        }
        if (wk->framecnt >= 8 && wk->framecnt <= 24) {
            SDF_VU0_LOADVECTOR(&wk->model->scrPos2);
            SDF_VU0_LOADVECTOR_B(&wk->model->scrPos);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_SCALEVECTOR_BITS((f32)(wk->framecnt - frame) / (f32)(24 - frame));
            SDF_VU0_LOADVECTOR_B(&wk->model->scrPos);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(&wk->pos);
        }
        if (wk->action_flg == 1 && wk->framecnt >= 24) {
            wk->action_flg = 2;
        }
        sbtlCallMemset12(wk);
        sbtlProc(wk);
        sbtlMVSupport(wk);
        CheckMapHit(wk, &oldpos);
        sbtlCallMemset5(wk);
        if (!sbtlCallBtlSearchEffectSABActer(wk)) {
            return 1;
        }
        if (wk->framecnt < 24) {
            return 1;
        }
        wk->flg &= ~0x4001;
        goto end_action;
    case 45:
        if (wk->framecnt >= (s32)sbtlCallBtlGetMotionVisualData4(wk, 0) && !wk->action_flg) {
            wk->action_flg = 1;
            sbtlSetUseSkill(wk, wk->skillid);
            if (sbtlSearchEffectData(wk->skillid) != -1) {
                sbtlCallBtlGetSklVisualTbl2(wk, wk->skillid);
            } else {
                sbtlForceDelete_SkillFlag_Acter(wk, 0x400);
            }
        }
        sbtlCallMemset12(wk);
        sbtlProc(wk);
        sbtlMVSupport(wk);
        CheckMapHit(wk, &oldpos);
        sbtlCallMemset5(wk);
        if (!sbtlCallBtlSearchEffectSABActer(wk) && (wk->skillid != SKLID_BS15_BAROFF || wk->framecnt < 101)) {
            return 1;
        }
        goto end_action;
    case 12:
        if (wk->framecnt >= (s32)sbtlCallBtlGetMotionVisualData4(wk, 0) && !wk->action_flg) {
            wk->action_flg = 1;
            sbtlSetUseSkill(wk, wk->skillid);
            if (sbtlSearchEffectData(wk->skillid) != -1) {
                sbtlCallBtlGetSklVisualTbl2(wk, wk->skillid);
                if ((wk->field414 & 0x80) && wk_sbtl->battleMode != 3) {
                    switch (wk_sbtl->mode) {
                    case 3:
                        if (wk->skillid == SKLID_NOUSE_SORD3) {
                            sbtlSetVibe(SBTLPLAYER_VTYPE_SWORD);
                        }
                        if (wk->skillid == SKLID_OFS_BUTURI) {
                            sbtlSetVibe(SBTLPLAYER_VTYPE_TUKI);
                        }
                        break;
                    case 4:
                        sbtlSetVibe(SBTLPLAYER_VTYPE_GUN);
                        break;
                    case 5:
                        sbtlSetVibe(SBTLPLAYER_VTYPE_KAITEN);
                        break;
                    }
                }
            } else {
                sbtlForceDelete_SkillFlag_Acter(wk, 0x400);
            }
        }
        sbtlMVSupport(wk);
        CheckMapHit(wk, &oldpos);
        sbtlCallMemset5(wk);
        return 1;
    case 15:
    case 16:
    case 25:
    case 26:
        sbtlMVSupport(wk);
        CheckMapHit(wk, &oldpos);
        sbtlCallMemset5(wk);
        if (!sbtlCheckMotionEnd(wk)) {
            return 1;
        }
        sbtlSetUseSkill_sbtlActAI(wk, SKLID_NONE, 0);
        if (wk_sbtl->mode == 6) {
            sbtlResetAction(wk);
            wk_sbtl->mode2 = 1;
            wk->flg |= 1;
            wk->vstatus &= ~0x80;
            wk_sbtl->mode = 6;
            sbtlSetMotion_sbtlUnit(wk, (sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->scrWarp[0])->flg & 0x200000) ? 25 : 24,
                                   0, 0, 1);
            return 1;
        }
        sbtlResetAction(wk);
        return 1;
    case 7:
        if (wk->vstatus & 4) {
            mdlFreeBlur(wk->hModel);
            wk->vstatus &= ~4;
        }
        sbtlMVSupport(wk);
        CheckMapHit(wk, &oldpos);
        sbtlCallMemset5(wk);
        if (sbtlCallBtlSearchEffectSABActer(wk)) {
            if ((wk->unit.devilId == 263 || wk->unit.devilId == 302) && wk->aiwk->scrStep == 3) {
                wk->color = 0;
            }
            ResetAction(wk, 0);
            sbtlResetAAI(wk);
            if (wk->field414 & 1) {
                wk->aiwk->mode = 0;
                wk->actCounter = wk->actMax;
            }
            wk->damage_hitcnt = 0;
            sbtlSetAction(wk, 2, 0);
            wk->flg &= ~0x20;
            if (wk->unit.badStatus & 1) {
                wk->unit.badStatus &= ~1;
            }
        }
        return 1;
    case 23:
        sbtlMVSupport(wk);
        CheckMapHit(wk, &oldpos);
        sbtlCallMemset5(wk);
        if (!sbtlCheckMotionEnd(wk)) {
            return 1;
        }
        if (wk->framecnt < wk->framecntB) {
            return 1;
        }
        sbtlSetAction(wk, 7, 0);
        wk->field424 = 0.0f;
        return 1;
    case 8:
        if ((wk_sbtl->battleMode == 3) ? (wk->field414 & 1) : (wk->field414 & 0x80)) {
            wk->field4A2 = 20;
        }
        wk->model->flags |= 0x20;
        sbtlMVSupport(wk);
        CheckMapHit(wk, &oldpos);
        sbtlCallMemset5(wk);
        if (wk->vstatus & 0x800) {
            return 1;
        }
        if (!sbtlCallBtlSearchEffectSABActer(wk)) {
            return 1;
        }
        ResetAction(wk, 0);
        sbtlResetAAI(wk);
        sbtlBS_MUNAKA_Init_Imad_Restart(wk);
        sbtlSetAction(wk, 2, 0);
        if (wk->field414 & 1) {
            wk->aiwk->mode = 0;
            wk->actCounter = wk->actMax;
        }
        sbtlEndAction(wk);
        wk->model->flags &= ~0x20;
        wk->flg &= ~0x20;
        if ((wk_sbtl->battleMode == 3) ? (wk->field414 & 1) : (wk->field414 & 0x80)) {
            wk->field4A2 = 60;
        }
        return 1;
    case 41:
        if (wk->framecnt >= (s32)sbtlCallBtlGetMotionVisualData4(wk, 0) && !wk->action_flg) {
            wk->action_flg = 1;
            sbtlSetUseSkill(wk, wk->skillid);
            if (wk->unit.devilId == 290) {
                sbtlSetEncountData(166, 2, 1);
                sbtlSetEncountData(166, 3, 1);
            } else if (wk->unit.devilId == 297) {
                sbtlSetEncountData(298, 2, 1);
                sbtlSetEncountData(298, 3, 1);
            } else if (sbtlCheckMeshiyose(wk)) {
                sbtlGeneratorD_On();
            } else {
                sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_SUMMONMIS, wk->skillid);
            }
        }
        sbtlCallMemset12(wk);
        sbtlProc(wk);
        sbtlMVSupport(wk);
        CheckMapHit(wk, &oldpos);
        sbtlCallMemset5(wk);
        if (!sbtlCallBtlGetMotionVisualData2(wk, wk->motionID)) {
            if (!sbtlCheckMotionEnd(wk)) {
                return 1;
            }
        } else if (wk->action_flg != 1) {
            return 1;
        }
        sbtlSetAction(wk, 2, 0);
        sbtlEndAction(wk);
        if (wk->skillid == SKLID_C_RESUM) {
            wk->flg &= ~0x4000;
        }
        return 1;
    case 40:
        frame = (s32)sbtlCallBtlGetMotionVisualData4(wk, 0);
        if (wk->framecnt >= frame && !wk->action_flg) {
            wk->action_flg = 1;
            if (wk->skillid == SKLID_BS14_CHANGE) {
                sbtlForceResetDecAlpha(wk_sbtl->effList, wk, 4);
            }
            if (wk->skillid == SKLID_BS14_DEAD) {
                sbtlSetMapFade(30, 0x80191919, 0);
            }
            sbtlSetUseSkill(wk, wk->skillid);
            wk_sbtl->enshutuCnt[3] = 0;
            wk_sbtl->scrCnt[3] = 0;
            wk->cmpframecnt2 = frame;
            if (sbtlSearchEffectData(wk->skillid) != -1) {
                sbtlCallBtlGetSklVisualTbl2(wk, wk->skillid);
            } else {
                sbtlForceDelete_SkillFlag_Acter(wk, 0x400);
            }
        } else {
            wk_sbtl->scrCnt[3] = 0;
        }
        if (wk->framecnt == wk->cmpframecnt2 + wk->framecntC) {
            if (wk->effect_flg & 1) {
                dds3FadeOut(0, 0, 0, wk->cmpframecnt);
                if (!wk_sbtl->battleMode || wk_sbtl->battleMode == 2) {
                    unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
                    if (unit->status != 42 && wk->unit.devilId != 259) {
                        sbtlSetMotion_sbtlUnit(unit, 12, 0, 0, 0);
                        sbtlSetAction(unit, 42, 0);
                    }
                }
            } else if (wk->effect_flg & 2) {
                dds3FadeOut(255, 255, 255, wk->cmpframecnt);
                if (!wk_sbtl->battleMode || wk_sbtl->battleMode == 2) {
                    unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
                    if (unit->status != 42 && wk->unit.devilId != 259) {
                        sbtlSetMotion_sbtlUnit(unit, 12, 0, 0, 0);
                        sbtlSetAction(unit, 42, 0);
                    }
                }
            }
            if (wk->skillid == SKLID_E_TETTAI) {
                sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_TOUBOU_E, SKLID_E_TETTAI);
                wk_sbtl->flg2 |= 1;
            }
        }
        if ((wk->field414 & 1) && wk->skillid == SKLID_OFS_AUTO) {
            sbtlMVSupport(wk);
            CheckMapHit(wk, &oldpos);
            sbtlCallMemset5(wk);
        } else {
            sbtlCallMemset12(wk);
            sbtlProc(wk);
            sbtlMVSupport(wk);
            CheckMapHit(wk, &oldpos);
            sbtlCallMemset5(wk);
        }
        if (wk->skillid == SKLID_BS7_DEADWAIT) {
            if (!sbtlCheckMotionEnd(wk)) {
                return 1;
            }
            if (wk->action_flg == 1) {
                wk->action_flg = 2;
                sbtlCallBtlGetMotionVisualData(wk, 21, 0, 0, 0);
            }
            return 1;
        }
        if (wk->skillid == SKLID_BS14_DEAD) {
            if (!sbtlCheckMotionEnd(wk)) {
                return 1;
            }
            if (wk->action_flg == 1) {
                wk->action_flg = 2;
                sbtlCallBtlGetMotionVisualData(wk, 17, 0, 0, 0);
            }
            return 1;
        }
        if (wk->framecnt < wk->cmpframecnt2 + wk->framecntC + wk->cmpframecnt) {
            return 1;
        }
        tbl = sbtlGetSkilltbl(wk->skillid);
        if ((tbl->type & 0x2FFF) == 0x2009) {
            if (sbtlSearchEffect_SAB(wk_sbtl->effList, wk->skillid, 1)) {
                return 1;
            }
            if (sbtlSearchEffect_SAB(wk_sbtl->effList, wk->skillid, 2)) {
                return 1;
            }
            if (sbtlSearchEffect_SAB(wk_sbtl->effList, wk->skillid, 5)) {
                return 1;
            }
        }
        if (wk->skillid == SKLID_OFS_AUTO && (wk->field414 & 1) && !(wk->field414 & 0x80)) {
            sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_DEAD_N, SKLID_NONE);
        }
        devil = 0;
        switch (wk->unit.devilId) {
        case 259:
            devil = 260;
            break;
        case 285:
            devil = 288;
            break;
        case 286:
            devil = 285;
            break;
        case 289:
            devil = 286;
            break;
        case 306:
            devil = 307;
            break;
        case 307:
            devil = 308;
            break;
        case 308:
            devil = 309;
            break;
        }
        if (devil == 260) {
            unit = NULL;
            for (i = 0; i < wk_sbtl->unitList->number; i++) {
                if (wk_sbtl->unitList->pUnitL[i].pUnit != NULL &&
                    wk_sbtl->unitList->pUnitL[i].pUnit->unit.devilId == 260) {
                    unit = wk_sbtl->unitList->pUnitL[i].pUnit;
                    break;
                }
            }
            pos.y = 600.0f;
            pos.z = 2000.0f;
            pos.x = 0.0f;
            sbtlCallMemset17(wk_sbtl->effList, unit, EFFID_SUMMON_ED, 0, &pos.x, 0x80808080, 0);
            wk_sbtl->scrFlag |= 0x20000;
        } else if (devil != 0) {
            SDF_VU0_LOADVECTOR(&wk->pos);
            SDF_VU0_LOADVECTOR_B(&aViewEye);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_STOREVECTOR(&dir);
            FromEuler(0.0f, sdfAtan2(dir.x, dir.z), 0.0f);
            SDF_VU0_STOREVECTOR(&posid);
            SetEncountData_Sub(&wk->pos.x, &posid.x);
            sbtlSetEncountData4(devil, 100, 0)->unit.now.hp = wk->unit.now.hp;
        }
        sbtlEndAction(wk);
        if (wk->effect_flg & 4) {
            if (wk->field414 & 1) {
                sbtlDeletePlayer(wk);
                sbtlSetMotion_sbtlUnit(wk, 13, 0, 0, 0);
                return 1;
            }
            if ((tbl->type & 0x2FFF) != 0x2009) {
                wk->color = 0;
                wk->mode = 9;
                return 1;
            }
            sbtlSetAction(wk, 10, 0);
            return 1;
        }
        if (wk->effect_flg & 8) {
            for (i = 0; i < wk_sbtl->unitList->number; i++) {
                unit = wk_sbtl->unitList->pUnitL[i].pUnit;
                if (unit != NULL && (unit->field414 & 2)) {
                    unit->mode = 9;
                }
            }
        }
        return 1;
    case 44:
        if (wk->framecnt >= (s32)sbtlCallBtlGetMotionVisualData4(wk, 0) && !wk->action_flg) {
            wk->action_flg = 1;
            sbtlSetUseSkill(wk, wk->skillid);
            if (sbtlSearchEffectData(wk->skillid) != -1) {
                sbtlCallBtlGetSklVisualTbl2(wk, wk->skillid);
            } else {
                sbtlForceDelete_SkillFlag_Acter(wk, 0x400);
            }
        }
        sbtlCallMemset12(wk);
        sbtlProc(wk);
        sbtlMVSupport(wk);
        CheckMapHit(wk, &oldpos);
        sbtlCallMemset5(wk);
        if (!sbtlCallBtlSearchEffectSABActer(wk)) {
            return 1;
        }
        switch (wk->skillid) {
        case SKLID_BS14_DEAD_R:
        case SKLID_BS14_DEAD_R2:
            ret = sbtlModelNodeFadeout(wk, "r arm root");
            wk_sbtl->scrUnitList[1]->mode = 9;
            break;
        case SKLID_BS14_DEAD_L:
        case SKLID_BS14_DEAD_L2:
            ret = sbtlModelNodeFadeout(wk, "l arm root");
            wk_sbtl->scrUnitList[2]->mode = 9;
            break;
        default:
            ret = -1;
            break;
        }
        if (ret == -1 || ret == 0) {
            return 1;
        }
        goto end_action;
    case 9:
        if (wk->framecnt >= (s32)sbtlCallBtlGetMotionVisualData4(wk, 1) && !wk->action_flg) {
            wk->m_rate = 0.0f;
        }
        sbtlMVSupport(wk);
        CheckMapHit(wk, &oldpos);
        sbtlCallMemset5(wk);
        if (wk->framecnt < wk->framecntB) {
            return 1;
        }
        if (wk_sbtl->flg & 0x40) {
            return 1;
        }
        if (wk->action_flg == 1) {
            if (!sbtlCallBtlSearchEffectSABActer(wk)) {
                return 1;
            }
            ResetAction(wk, 0);
            sbtlSetAction(wk, 2, 0);
            wk->damage_hitcnt = 0;
            if (wk->field414 & 1) {
                sbtlResetAAI(wk);
                wk->aiwk->mode = 0;
                wk->actCounter = wk->actMax;
            }
        } else if (wk->unit.now.hp <= 0 && !(wk->field414 & 0x80)) {
            sbtlRestartMotion(wk);
            if (!(wk->field414 & 0x80)) {
                sbtlSetDeadDamage(wk, 0);
            }
            wk->action_flg = 1;
        } else {
            sbtlRestartMotion(wk);
            wk->aiwk->stat = 1;
            wk->flg &= ~0x20;
            wk->flg2 &= ~0x10;
            wk->field4A2 = 30;
            wk->action_flg = 1;
        }
        return 1;
    case 10:
        if (wk->aiwk->scrflag & 0x20000) {
            sbtlProc(wk);
            if (wk->aiwk->scrflag & 0x40000) {
                return 1;
            }
            sbtlStartDeadEvent(wk);
            wk->action_flg = 1;
            return 1;
        }
        if (wk->flg & 0x40) {
            if (wk->action_flg != 1 && wk->framecnt >= 80) {
                wk->action_flg = 1;
                wk->m_rate = 0.0f;
                sbtlSetFade(wk, SBTLUNIT_VST_FADEOUT, 20, 0, 8);
            }
        } else if (wk->action_flg != 1 && wk->framecnt >= (s32)sbtlCallBtlGetMotionVisualData4(wk, 1)) {
            wk->action_flg = 1;
            wk->m_rate = 0.0f;
            if (wk->field414 & 2) {
                sbtlSetFade(wk, SBTLUNIT_VST_FADEOUT, 20, 0, 8);
            }
        }
        sbtlMVSupport(wk);
        CheckMapHit(wk, &oldpos);
        sbtlCallMemset5(wk);
        if (wk->action_flg != 1) {
            return 1;
        }
        if (wk->field414 & 1) {
            sbtlDeletePlayer(wk);
            return 1;
        }
        if (!sbtlCheckMotionEnd(wk) && (wk->vstatus & 2)) {
            return 1;
        }
        wk->mode = 9;
        return 1;
    case 34:
        if (wk_sbtl->battleMode == 3) {
            wk->effect_flg = 1;
            wk->framecntC = 10;
            wk->cmpframecnt = 90;
            wk->cmpframecnt2 = 0;
        } else {
            wk->effect_flg = 1;
            wk->cmpframecnt2 = 120;
            wk->framecntC = 135;
            wk->cmpframecnt = 90;
        }
        if (wk->framecnt >= (s32)sbtlCallBtlGetMotionVisualData4(wk, 1)) {
            wk->m_rate = 0.0f;
            wk->action_flg = 1;
        }
        sbtlMVSupport(wk);
        CheckMapHit(wk, &oldpos);
        sbtlCallMemset5(wk);
        if (wk->framecnt != wk->cmpframecnt2 + wk->framecntC) {
            return 1;
        }
        if (wk->effect_flg & 1) {
            dds3FadeOut(0, 0, 0, wk->cmpframecnt);
            return 1;
        }
        if (wk->effect_flg & 2) {
            dds3FadeOut(255, 255, 255, wk->cmpframecnt);
        }
        return 1;
    case 18:
        wk->mode = 9;
        return 1;
    case 14:
        sbtlResetSummon_E(wk);
        return 1;
    case 20:
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->field492);
        if (unit != NULL) {
            sbtlGetHelperPos(wk, unit, wk->unit.devilId);
        }
        sbtlMVSupport(wk);
        return 1;
    case 22:
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->field492);
        if (unit != NULL) {
            sbtlGetUnitHelperPos(unit, wk->field490);
            SDF_VU0_STOREVECTOR(&wk->pos);
        }
        sbtlMVSupport(wk);
        if (wk->framecnt < wk->framecntB) {
            return 1;
        }
        wk->unit.now.hp = wk->unit.now.maxhp;
        sbtlSetAction(wk, 20, 0);
        wk->unit.badStatus &= ~1;
        return 1;
    case 46:
        sbtlGetCylinderData(wk, &data);
        if (wk->framecnt >= 121) {
            wk->pos.y += data.height / 150.0f;
            dds3SetPosPoseSet_dds3Basic(wk->pBasic, &wk->pos);
        }
        if (wk->framecnt >= 270 && !wk->action_flg) {
            wk->action_flg = 1;
            sbtlCallBtlSearchUnitID4(wk_sbtl->summonIndex[0], 1);
            sbtlResetBit(&wk_sbtl->summonBit, wk_sbtl->summonIndex[0]);
            sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_HUUIN_N, SKLID_NONE);
        }
        return 1;
    case 29:
        if (wk->vstatus & 2) {
            wk->status = 30;
        }
        sbtlMVSupport(wk);
        CheckMapHit(wk, &oldpos);
        sbtlCallMemset5(wk);
        return 1;
    case 48:
        if (wk->framecnt >= (s32)sbtlCallBtlGetMotionVisualData4(wk, 0) && !wk->action_flg) {
            wk->action_flg = 1;
            bst = sbtlGetBadStatus(&wk->unit);
            if (bst != BST_BOYCOTT && bst != BST_KANSEN) {
                sbtlResetBadStatus(wk, 1 << bst);
                sbtlRestartMotion(wk);
                wk->flg &= ~0x20;
                wk->flg2 &= ~0x10;
            }
            sbtlSetBadStatus_No(wk, wk->skillid, BST_CHARM, 0, -1);
            sbtlEndAction(wk);
        }
        goto end_proc;
    case 49:
        sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
        if (wk->framecnt >= (s32)sbtlCallBtlGetMotionVisualData4(wk, 0) && !wk->action_flg) {
            wk->action_flg = 1;
            sbtlSetMapFade(15, 0x80404040, 0);
            if (sbtlSearchEffectData(wk->skillid) != -1) {
                sbtlCallBtlGetSklVisualTbl2(wk, wk->skillid);
                sbtlSetUseSkill(wk, SKLID_BS14_CHOUSIN_HIT);
            } else {
                sbtlForceDelete_SkillFlag_Acter(wk, 0x400);
            }
        }
        if (wk->framecnt >= 225 && wk->action_flg == 1) {
            wk->action_flg = 2;
            if (sbtlSearchEffectData(SKLID_BS14_CHOUSIN_HIT) != -1) {
                wk->aiwk->scrStep3 = 2;
                sbtlGetPointData(sbtlGetPointLast(wk, SKLID_BS14_CHOUSIN_HIT), &pos, &rot);
                SDF_VU0_LOADVECTOR(&wk->pos);
                SDF_VU0_LOADVECTOR_B(&pos);
                SDF_VU0_SUBVECTOR();
                SDF_VU0_STOREVECTOR(&dir);
                angle = sdfAtan2(dir.x, dir.z);
                FromEuler(0.0f, angle, 0.0f);
                SDF_VU0_STOREVECTOR(&wk->rot);
                wk->angleY = angle * 57.295777f;
                sbtlCallBtlGetSklVisualTbl2(wk, SKLID_BS14_CHOUSIN_HIT);
                SDF_VU0_LOADVECTOR(&oldrot);
                SDF_VU0_STOREVECTOR(&wk->rot);
                wk->angleY = oldangleY;
            }
        }
        if (wk->framecnt >= 245 && wk->action_flg == 2) {
            wk->action_flg = 3;
            if (sbtlSearchEffectData(SKLID_BS14_CHOUSIN_HIT) != -1) {
                wk->aiwk->scrStep3 = 1;
                sbtlGetPointData(sbtlGetPointLast(wk, SKLID_BS14_CHOUSIN_HIT), &pos, &rot);
                SDF_VU0_LOADVECTOR(&wk->pos);
                SDF_VU0_LOADVECTOR_B(&pos);
                SDF_VU0_SUBVECTOR();
                SDF_VU0_STOREVECTOR(&dir);
                angle = sdfAtan2(dir.x, dir.z);
                FromEuler(0.0f, angle, 0.0f);
                SDF_VU0_STOREVECTOR(&wk->rot);
                wk->angleY = angle * 57.295777f;
                sbtlCallBtlGetSklVisualTbl2(wk, SKLID_BS14_CHOUSIN_HIT);
                SDF_VU0_LOADVECTOR(&oldrot);
                SDF_VU0_STOREVECTOR(&wk->rot);
                wk->angleY = oldangleY;
            }
        }
        if (wk->framecnt >= 265 && wk->action_flg == 3) {
            wk->action_flg = 4;
            if (sbtlSearchEffectData(SKLID_BS14_CHOUSIN_HIT) != -1) {
                wk->aiwk->scrStep3 = 3;
                sbtlGetPointData(sbtlGetPointLast(wk, SKLID_BS14_CHOUSIN_HIT), &pos, &rot);
                SDF_VU0_LOADVECTOR(&wk->pos);
                SDF_VU0_LOADVECTOR_B(&pos);
                SDF_VU0_SUBVECTOR();
                SDF_VU0_STOREVECTOR(&dir);
                angle = sdfAtan2(dir.x, dir.z);
                FromEuler(0.0f, angle, 0.0f);
                SDF_VU0_STOREVECTOR(&wk->rot);
                wk->angleY = angle * 57.295777f;
                sbtlCallBtlGetSklVisualTbl2(wk, SKLID_BS14_CHOUSIN_HIT);
                SDF_VU0_LOADVECTOR(&oldrot);
                SDF_VU0_STOREVECTOR(&wk->rot);
                wk->angleY = oldangleY;
            }
        }
        if (wk->framecnt >= 285 && wk->action_flg == 4) {
            wk->action_flg = 5;
            if (sbtlSearchEffectData(SKLID_BS14_CHOUSIN_HIT) != -1) {
                wk->aiwk->scrStep3 = 0;
                sbtlGetPointData(sbtlGetPointLast(wk, SKLID_BS14_CHOUSIN_HIT), &pos, &rot);
                SDF_VU0_LOADVECTOR(&wk->pos);
                SDF_VU0_LOADVECTOR_B(&pos);
                SDF_VU0_SUBVECTOR();
                SDF_VU0_STOREVECTOR(&dir);
                angle = sdfAtan2(dir.x, dir.z);
                FromEuler(0.0f, angle, 0.0f);
                SDF_VU0_STOREVECTOR(&wk->rot);
                wk->angleY = angle * 57.295777f;
                sbtlCallBtlGetSklVisualTbl2(wk, SKLID_BS14_CHOUSIN_HIT);
                SDF_VU0_LOADVECTOR(&oldrot);
                SDF_VU0_STOREVECTOR(&wk->rot);
                wk->angleY = oldangleY;
            }
        }
        if (wk->framecnt >= 305 && wk->action_flg == 5) {
            wk->action_flg = 6;
            if (sbtlSearchEffectData(SKLID_BS14_CHOUSIN_HIT) != -1) {
                wk->aiwk->scrStep3 = 4;
                sbtlGetPointData(sbtlGetPointLast(wk, SKLID_BS14_CHOUSIN_HIT), &pos, &rot);
                SDF_VU0_LOADVECTOR(&wk->pos);
                SDF_VU0_LOADVECTOR_B(&pos);
                SDF_VU0_SUBVECTOR();
                SDF_VU0_STOREVECTOR(&dir);
                angle = sdfAtan2(dir.x, dir.z);
                FromEuler(0.0f, angle, 0.0f);
                SDF_VU0_STOREVECTOR(&wk->rot);
                wk->angleY = angle * 57.295777f;
                sbtlCallBtlGetSklVisualTbl2(wk, SKLID_BS14_CHOUSIN_HIT);
                SDF_VU0_LOADVECTOR(&oldrot);
                SDF_VU0_STOREVECTOR(&wk->rot);
                wk->angleY = oldangleY;
            }
        }
        if (wk->framecnt >= 340 && wk->action_flg == 6) {
            wk->action_flg = 7;
            sbtlSetMapFade(30, 0x80808080, 1);
        }
        goto end_proc;
    case 50:
        sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
        frame = (s32)sbtlCallBtlGetMotionVisualData4(wk, 0);
        if (wk->framecnt >= frame && !wk->action_flg) {
            wk->action_flg = 1;
            wk->aiwk->scrStep3 = 0;
            if (sbtlSearchEffectData(wk->skillid) != -1) {
                sbtlCallBtlGetSklVisualTbl2(wk, wk->skillid);
            } else {
                sbtlForceDelete_SkillFlag_Acter(wk, 0x400);
            }
        }
        if (wk->framecnt >= frame + 30 && wk->action_flg == 1) {
            wk->action_flg = 2;
            wk->aiwk->scrStep3 = 1;
            if (sbtlSearchEffectData(wk->skillid) != -1) {
                sbtlCallBtlGetSklVisualTbl2(wk, wk->skillid);
            } else {
                sbtlForceDelete_SkillFlag_Acter(wk, 0x400);
            }
        }
        goto end_proc;
    case 51:
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
        frame = (s32)sbtlCallBtlGetMotionVisualData4(wk, 0);
        if (!wk->action_flg) {
            wk->action_flg = 1;
            switch (wk->skillid) {
            case SKLID_BS14_PLASMA_R:
            case SKLID_BS14_PLASMA_L2:
                sbtlGetPointData(10, &wk->model->scrPos, &rot);
                wk->model->scrPos.z += 50.0f;
                SDF_VU0_LOADVECTOR(&wk->model->scrPos);
                SDF_VU0_STOREVECTOR(&wk->model->scrPos2);
                wk->model->scrPos2.x = unit->pos.x + 700.0f;
                sbtlGetPointData(17, &pos, &rot);
                if (pos.x - 300.0f < wk->model->scrPos2.x) {
                    wk->model->scrPos2.x = pos.x - 300.0f;
                }
                break;
            case SKLID_BS14_PLASMA_L:
            case SKLID_BS14_PLASMA_R2:
                sbtlGetPointData(17, &wk->model->scrPos, &rot);
                wk->model->scrPos.z += 50.0f;
                SDF_VU0_LOADVECTOR(&wk->model->scrPos);
                SDF_VU0_STOREVECTOR(&wk->model->scrPos2);
                wk->model->scrPos2.x = unit->pos.x - 700.0f;
                sbtlGetPointData(10, &pos, &rot);
                if (wk->model->scrPos2.x < pos.x + 300.0f) {
                    wk->model->scrPos2.x = pos.x + 300.0f;
                }
                break;
            }
        }
        SDF_VU0_LOADVECTOR(&wk->model->scrPos2);
        SDF_VU0_LOADVECTOR_B(&wk->model->scrPos);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_SCALEVECTOR_BITS((f32)(wk->framecnt - frame - 120) / 150.0f);
        SDF_VU0_LOADVECTOR_B(&wk->model->scrPos);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(&wk->model->scrPos3);
        if (wk->framecnt >= frame && wk->action_flg == 1) {
            wk->action_flg = 2;
            sbtlSetUseSkill(wk, wk->skillid);
            if (sbtlSearchEffectData(wk->skillid) != -1) {
                sbtlCallBtlGetSklVisualTbl2(wk, wk->skillid);
            } else {
                sbtlForceDelete_SkillFlag_Acter(wk, 0x400);
            }
        }
    end_proc:
        sbtlCallMemset12(wk);
        sbtlProc(wk);
        sbtlMVSupport(wk);
        CheckMapHit(wk, &oldpos);
        sbtlCallMemset5(wk);
    end_check:
        if (!sbtlCallBtlSearchEffectSABActer(wk)) {
            return 1;
        }
    end_action:
        sbtlBS_MUNAKA_Init_Imad_Restart(wk);
        sbtlSetAction(wk, 2, 0);
        sbtlEndAction(wk);
        return 1;
    case 52:
        if (wk->framecnt >= (s32)sbtlCallBtlGetMotionVisualData4(wk, 0) && !wk->action_flg) {
            wk->action_flg = 1;
            sbtlSetUseSkill(wk, wk->skillid);
            if (sbtlSearchEffectData(wk->skillid) != -1) {
                sbtlCallBtlGetSklVisualTbl2(wk, wk->skillid);
            } else {
                sbtlForceDelete_SkillFlag_Acter(wk, 0x400);
            }
        }
        sbtlCallMemset12(wk);
        sbtlProc(wk);
        sbtlMVSupport(wk);
        CheckMapHit(wk, &oldpos);
        sbtlCallMemset5(wk);
        break;
    }
    return 1;
}
#else
INCLUDE_RODATA(const s32, "sbtlActAI", sbtlGetMovePos_MapHit_2Table);

INCLUDE_ASM(const s32, "sbtlActAI", Proc);
#endif

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

s32 sbtlSetUseSkill_sbtlActAI(struct SBTLUNIT_WORK *wk, SKLID skillid, s32 after_frame)
{
    SBTL_STAGE_DATA *stage;
    struct SBTLSKILLP_TBL *skl;
    struct SBTLUNIT_WORK *unit;
    s32 n0;
    s32 n1;
    s32 n2;
    s32 n5;
    s32 i;

    stage = &sbtlData100[wk_sbtl->stageNo];
    if (!(stage->flags & 4) && (wk->field414 & 2)) {
        n0 = sbtlGetNumOfEffect_SAB(wk_sbtl->effList, 0, 2);
        if (n0 >= 6) {
            return 1;
        }
        n1 = sbtlGetNumOfEffect_SAB(wk_sbtl->effList, 1, 2);
        if (n1 >= 6) {
            return 1;
        }
        n2 = sbtlGetNumOfEffect_SAB(wk_sbtl->effList, 2, 2);
        if (n2 >= 4) {
            return 1;
        }
        n5 = sbtlGetNumOfEffect_SAB(wk_sbtl->effList, 5, 2);
        if (n5 >= 4) {
            return 1;
        }
        if (n0 + n1 + n2 + n5 >= 8) {
            return 1;
        }
    }
    if ((stage->flags & 4) && sbtlCheckSkillCancel(wk, skillid)) {
        return 1;
    }
    wk->skillid = skillid;
    SetUseskl(wk, skillid);
    sbtlSetSkill_MotionData(wk, wk->skillid);
    sbtlSetHatData(wk, wk->skillid);
    skl = sbtlGetSkilltbl(skillid);
    if (skl->flg & 0x10) {
        SetAction_ProgramSkill(wk, skillid);
        wk->action_count = 0;
        return 1;
    }
    if (skillid == SKLID_C_RESUM) {
        wk->flg |= 0x4000;
        sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_SUMMONOK, wk->skillid);
    }
    switch (skl->type & 0xFFF) {
    case 0:
        sbtlSetAction(wk, 1, 0);
        break;
    case 1:
    case 12:
    case 13:
        sbtlSetAction(wk, 3, 0);
        break;
    case 3:
    case 15:
        sbtlSetAction(wk, 6, 90);
        break;
    case 8:
        sbtlSetAction(wk, 37, 0);
        break;
    case 2:
    case 5:
    case 14:
        sbtlSetAction(wk, 31, 90);
        break;
    case 10:
        sbtlSetAction(wk, 41, 0);
        break;
    case 11:
        sbtlSetAction(wk, 43, 0);
        break;
    case 16:
        sbtlSetAction(wk, 45, 0);
        break;
    case 9:
        sbtlSetAction(wk, 40, 0);
        switch (skillid) {
        case SKLID_BS0_ESCAPE:
            wk->effect_flg = 10;
            wk_sbtl->flg |= 0x40000000;
            wk->framecntC = 35;
            wk->cmpframecnt = 45;
            wk->flg2 |= 8;
            break;
        case SKLID_E_DEADIM2:
            wk->effect_flg = 4;
            wk->cmpframecnt = 1000;
            wk->framecntC = 0;
            break;
        case SKLID_E_DEADIM:
            wk->effect_flg = 10;
            wk_sbtl->flg |= 0x40000000;
            wk->framecntC = 430;
            wk->cmpframecnt = 60;
            break;
        case SKLID_BS2_DEAD:
            wk->effect_flg = 4;
            wk->framecntC = 150;
            wk->cmpframecnt = 0;
            break;
        case SKLID_BS3_DEAD:
            wk->effect_flg = 6;
            wk_sbtl->flg |= 0x40000000;
            wk->framecntC = 220;
            wk->cmpframecnt = 60;
            break;
        case SKLID_BS4_ESCAPE:
            wk->effect_flg = 6;
            wk_sbtl->flg |= 0x40000000;
            wk->framecntC = 240;
            wk->cmpframecnt = 60;
            wk->flg2 |= 8;
            break;
        case SKLID_BS4_DEAD:
            wk->effect_flg = 6;
            wk_sbtl->flg |= 0x40000000;
            wk->framecntC = 300;
            wk->cmpframecnt = 60;
            break;
        case SKLID_BS6_DEAD0:
            wk->effect_flg = 4;
            wk->framecntC = 105;
            wk->cmpframecnt = 0;
            break;
        case SKLID_BS6_DEAD1:
            wk->effect_flg = 9;
            wk_sbtl->flg |= 0x40000000;
            wk->framecntC = 330;
            wk->cmpframecnt = 60;
            break;
        case SKLID_BS7_DEADWAIT:
            wk->effect_flg = 0;
            wk->framecntC = 0;
            wk->cmpframecnt = 0;
            break;
        case SKLID_BS7_DEAD:
            wk->effect_flg = 10;
            wk_sbtl->flg |= 0x40000000;
            wk->framecntC = 630;
            wk->cmpframecnt = 60;
            break;
        case SKLID_BS8_DEAD_N:
            wk->effect_flg = 4;
            wk->framecntC = 400;
            wk->cmpframecnt = 0;
            break;
        case SKLID_BS8_DEAD_A:
            wk->effect_flg = 4;
            wk->framecntC = 400;
            wk->cmpframecnt = 0;
            break;
        case SKLID_BS8_DEAD:
            wk->effect_flg = 6;
            wk_sbtl->flg |= 0x40000000;
            wk->framecntC = 450;
            wk->cmpframecnt = 60;
            break;
        case SKLID_BS9_CHANGE1:
            wk->effect_flg = 4;
            wk->cmpframecnt = 270;
            wk->framecntC = 0;
            break;
        case SKLID_BS9_CHANGE2:
            wk->effect_flg = 4;
            wk->cmpframecnt = 270;
            wk->framecntC = 0;
            break;
        case SKLID_BS9_CHANGE3:
            wk->effect_flg = 4;
            wk->cmpframecnt = 270;
            wk->framecntC = 0;
            break;
        case SKLID_BS9_DEAD:
            wk->effect_flg = 4;
            wk->cmpframecnt = 260;
            wk->framecntC = 0;
            break;
        case SKLID_BS10_DEAD:
            wk->effect_flg = 6;
            wk_sbtl->flg |= 0x40000000;
            wk->framecntC = 370;
            wk->cmpframecnt = 60;
            sbtlForceResetDecAlpha(wk_sbtl->effList, wk, 4);
            break;
        case SKLID_BS12_DEAD:
            wk->effect_flg = 6;
            wk_sbtl->flg |= 0x40000000;
            wk->framecntC = 420;
            wk->cmpframecnt = 60;
            sbtlForceResetDecAlpha(wk_sbtl->effList, wk, 4);
            break;
        case SKLID_BS14_DEAD_S:
            wk->effect_flg = 10;
            wk_sbtl->flg |= 0x40000000;
            wk->framecntC = 300;
            wk->cmpframecnt = 60;
            break;
        case SKLID_BS14_CHANGE:
            wk->effect_flg = 6;
            wk->framecntC = 144;
            wk->cmpframecnt = 143;
            break;
        case SKLID_BS14_DEAD:
            wk->effect_flg = 0;
            wk_sbtl->flg |= 0x40000000;
            wk->cmpframecnt = 60;
            wk->framecntC = 0;
            break;
        case SKLID_BS15_DEAD:
            wk->effect_flg = 10;
            wk_sbtl->flg |= 0x40000000;
            wk->framecntC = 270;
            wk->cmpframecnt = 60;
            break;
        case SKLID_OFS_AUTO:
            wk->effect_flg = 4;
            wk->framecntC = 0;
            wk->cmpframecnt = 0;
            sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_BOSS, SKLID_NONE);
            break;
        case SKLID_E_TETTAI:
            wk->effect_flg = 4;
            wk->framecntC = 120;
            wk->flg2 |= 8;
            wk->cmpframecnt = 0;
            break;
        default:
            wk->effect_flg = 0;
            break;
        }
        if (wk->effect_flg & 4) {
            sbtlFadeOut_BadStatusEffect(wk, 4);
        }
        if (wk->effect_flg & 8) {
            for (i = 0; i < wk_sbtl->unitList->number; i++) {
                unit = wk_sbtl->unitList->pUnitL[i].pUnit;
                if (unit != NULL && (unit->field414 & 2)) {
                    sbtlFadeOut_BadStatusEffect(unit, 4);
                }
            }
        }
        break;
    case 4:
    case 6:
    case 7:
        break;
    }
    wk->action_count = 0;
    return 1;
}

s32 sbtlSetAction(struct SBTLUNIT_WORK *wk, u32 action, s32 flg) {
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
            sbtlResetAction(target);
        }
    }
}

s32 sbtlCheckStatus_Move(struct SBTLUNIT_WORK *wk) {
    return wk->status < 3;
}

/* NON_MATCHING: 32 of 35 words match.  */
#ifdef NON_MATCHING
s32 sbtlCallBtlDeleteHitHelper(struct SBTLUNIT_WORK *wk) {
    sbtlRefSbtl19(wk);
    if (wk->flg & 0x1000) {
        sbtlDeleteHitHelper(wk);
    }
    if (!(wk->flg2 & 6) && sbtlGetBadStatus(&wk->unit) == BST_KANSEN) {
        sbtlSetItem(wk);
    }
    sbtlCallBtlCreateDelWork(wk);
    sbtlResetBadStatus(wk, wk->unit.badStatus);
    sbtlForceDelete_SkillFlag_Acter(wk, 0x400);
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlActAI", sbtlCallBtlDeleteHitHelper);
#endif

INCLUDE_ASM(const s32, "sbtlActAI", sbtlDeletePlayer);

INCLUDE_ASM(const s32, "sbtlActAI", SetAction_ProgramSkill);
