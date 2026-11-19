#include "common.h"
#include "sbtlWork.h"
#include "sBtlSM.h"
#include "sBtlAI.h"
#include "sBtlUnit.h"
#include "sdk_libc.h"
#include "scrScriptProcess.h"
#include "sBtlCalc.h"
#include "dds3PadManager.h"
#include "ps2asm.h"
#include "sbtlActAI.h"
#include "itfMesManager.h"
#include "sbtlSub.h"
#include "sBtlSkill.h"
#include "code_0027A1E8.h"
#include "AW_XAtkPad.h"
#include "sBtlPlayer.h"

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCheckKajaAnyEx);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCheckKajaAny);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallBtlGetSkilltbl);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCheckEnableUseKaja);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlBtlCheckEnableUseSkillSub);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCheckEnableUseSkill);

void sbtlResetBtlMagicStatus(struct SBTLUNIT_WORK *wk) {
    s32 i;
    s32 j;

    if (wk == NULL) {
        return;
    }
    if (wk->barrier.size != 0) {
        wk->barrier.size = 0;
        wk->barrier.field0 = 0;
        wk->barrier.field4 = 0;
        wk->barrier.fieldC = 0;
        sbtlCheckBattlePara(wk);
    }
    for (i = 0; i < 3; i++) {
        for (j = 1; j >= 0; j--) {
            wk->kaja[i][j].m_cnt = 0;
        }
    }
    wk->karnCnt = 0;
}

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlGetBothSide);

/* NON_MATCHING: 45 of 46 words match.  */
#ifdef NON_MATCHING
s32 sbtlUnit_CheckSummon_LoadExecOk_sub(void) {
    s32 i;
    struct SBTLUNIT_WORK *pUnit;

    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        pUnit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (pUnit != NULL && (pUnit->field414 & 1) && !(pUnit->field414 & 0x80) && sbtlSearchReturn_Unit(pUnit)) {
            pUnit->flg |= 0x10;
            return 0;
        }
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_0027A1E8", sbtlUnit_CheckSummon_LoadExecOk_sub);
#endif

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCheckSummon_LoadExecOk);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlGetNakamaUnit);

s32 sbtlCancelSystemSub(s32 id) {
    s32 i;

    for (i = 0; sbtlCancelSystemSubData[i] != 0; i++) {
        if (sbtlCancelSystemSubData[i] == id) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallBtlGetPChara2);

INCLUDE_ASM(const s32, "code_0027A1E8", SetFirstAtk_Core);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlSetVibe);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlGetPlayerID);

struct SBTLUNIT_WORK *sbtlRefSbtl10(s32 ID)
{
    return sbtlSearchUnit_ID(wk_sbtl->unitList, ID);
}

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallObjectBaseSetPadControl);

s32 sbtlResetAction(struct SBTLUNIT_WORK *wk)
{
    wk->vstatus &= 0xF67DFFFF;
    wk->flg &= 0xFFFFBEEE;
    wk->flg2 &= ~0x20;
    wk->flg2 &= ~0x400;
    wk->model->flags &= ~0x4200;
    wk_sbtl->mode = 1;
    wk_sbtl->gstat[0] = 0;
    wk_sbtl->gstat[1] = 0;
    wk_sbtl->field7C8 = 0;
    wk_sbtl->field7D8 = 0;
    wk_sbtl->field7D0 = 0;
    wk_sbtl->walk_counter = 0;
    wk_sbtl->field7DC = 0;
    wk_sbtl->field50A = 0;
    wk_sbtl->field618 = 0;
    wk_sbtl->field620 = 0;
    wk_sbtl->raido_magwaza_costnot = 0;
    wk->field4A0 = 0;
    sbtlRestartMotion(wk);
    wk->vstatus |= 0x80;
    wk->field474 = 0;
    sbtlRefSbtl11(wk);
    return 1;
}

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlRefSbtl36);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlActiveAction);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlResetATKEffect);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlResetCombine);

s32 sbtlLeaf63(struct SBTLUNIT_WORK *wk) {
    u32 status = wk->status;

    if (status >= 7 && (status < 11 || status == 34)) {
        return 0;
    }
    return 1;
}

s32 sbtlNormalProcNakamaSub(struct SBTLUNIT_WORK *wk)
{
    return wk->status < 3;
}

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlSummonKey);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlSetNSTOP);

void sbtlResetNSTOP(void) {
    s32 i;
    struct SBTLUNIT_WORK *unit;

    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        unit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (unit != NULL && unit->hModel != NULL) {
            unit->vstatus &= ~0x8000000;
        }
    }
}

INCLUDE_RODATA(const s32, "code_0027A1E8", sbtlSetVibeTable);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallGetActiveScene2);

/* NON_MATCHING: 15 of 42 words match.  */
#ifdef NON_MATCHING
s32 sbtlCheckMove_BadStatus(struct SBTLUNIT_WORK *wk) {
    if (wk_sbtl->flg2 & 0x20000000) {
        return 0;
    }
    if (wk_sbtl->mode == 9) {
        return 1;
    }
    if (wk->status == 34 || wk->status == 42 || wk->status == 10) {
        return 0;
    }
    switch (sbtlGetBadStatus(&wk->unit)) {
        case BST_STONE:
        case BST_BOUDO:
        case BST_MOUDOKU:
        case BST_SLEEP:
        case BST_DEAD:
        case BST_RAND0:
        case BST_BOYCOTT:
            return 1;
        case BST_CHARM:
        case BST_PANIC:
        case BST_MAHUU:
        case BST_KIZETU:
        case BST_GUN:
        case BST_KANSEN:
        case BST_HUUIN:
        case BST_UNK18:
        case BST_UNK19:
            return 0;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCheckMove_BadStatus);
#endif

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlGetRandomTarget);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlSetSummonFunc);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlSetReturn);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallHirukoYukaGoIdx2B);

s32 sbtlResetColor(u32 color) {
    cmpSetColor(color);
    sbtlResetColorSub(color);
    return 1;
}

INCLUDE_ASM(const s32, "code_0027A1E8", SetSummonMenu);

/* body: matched in raidou @ 780f3f57: src/raidou1/code_r1_00259A88.c:1481, found, same function 0.92 (words 159/157, calls 0.80, shape 0.96) */
f32 slCallMemset(struct SBTLUNIT_WORK *wk, sdfVector4_t *dir)
{
    sdfVector4_t key;
    sdfVector4_t norm;
    f32 length;
    BST bst;

    memset(&key, 0, sizeof(key));
    key.w = 1.0f;
    bst = sbtlGetBadStatus(&wk->unit);
    if (!IsSceanStop()) {
        if (((wk->field414 & 0x80) || (wk_sbtl->battleMode == 3 && (wk->field414 & 1))) && bst == BST_BOUDO) {
            key.x = -(GetAnlogKeyData[0] - 128.0f);
            key.z = GetAnlogKeyData[1] - 128.0f;
        } else {
            key.x = GetAnlogKeyData[0] - 128.0f;
            key.z = -(GetAnlogKeyData[1] - 128.0f);
        }
    }
    SDF_VU0_LOADVECTOR(&key);
    SDF_VU0_GETLENGTH(length);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&norm);
    if (length < 48.0f) {
        key.x = 0.0f;
        key.z = 0.0f;
        if (dds3PadButtonState[5]) {
            key.x = 128.0f;
        }
        if (dds3PadButtonState[4]) {
            key.x = -128.0f;
        }
        if (dds3PadButtonState[6]) {
            key.z = 128.0f;
        }
        if (dds3PadButtonState[7]) {
            key.z = -128.0f;
        }
        if (((wk->field414 & 0x80) || (wk_sbtl->battleMode == 3 && (wk->field414 & 1))) && bst == BST_BOUDO) {
            key.x = -key.x;
            key.z = -key.z;
        }
        SDF_VU0_LOADVECTOR(&key);
        SDF_VU0_GETLENGTH(length);
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR(&norm);
    }
    if (dir != NULL) {
        SDF_VU0_LOADVECTOR(&norm);
        SDF_VU0_STOREVECTOR(dir);
    }
    return length;
}

INCLUDE_ASM(const s32, "code_0027A1E8", UseItemProc_sbtlPlayer);

INCLUDE_ASM(const s32, "code_0027A1E8", KaihiInit_Nakama);

INCLUDE_ASM(const s32, "code_0027A1E8", slCallBtlGetPChara);

INCLUDE_ASM(const s32, "code_0027A1E8", slCallSetFirstAtkGun);

INCLUDE_ASM(const s32, "code_0027A1E8", slCallSetFirstAtkCore);

s32 sbtlEscapeCheck(void) {
    SBTL_WORK *wk = wk_sbtl;
    SBTL_STAGE_DATA *stage = &sbtlData100[wk->stageNo];

    if (stage->flags & 0x10) {
        return 2;
    }
    if (wk->flg & 0x400000) {
        return 1;
    }
    return 0;
}

void sbtlRefSbtl11(struct SBTLUNIT_WORK *wk)
{
    wk_sbtl->field7FC = 0;
    wk_sbtl->field800 = 0;
    wk_sbtl->field80C = 0;
    wk_sbtl->field810 = 0;
    wk->vstatus &= ~0x1000000;
    wk_sbtl->fieldEC0 = 0;
}

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallBtlCreateEff);

INCLUDE_ASM(const s32, "code_0027A1E8", NormalProc);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallBtlResetAction);

INCLUDE_ASM(const s32, "code_0027A1E8", EscapeGauge);

INCLUDE_ASM(const s32, "code_0027A1E8", CheckAutoSkill_Escape);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlDrawEscapeGauge);

INCLUDE_ASM(const s32, "code_0027A1E8", SetColor);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallBtlResetJump);

INCLUDE_RODATA(const s32, "code_0027A1E8", CancelSystemTable);

INCLUDE_ASM(const s32, "code_0027A1E8", CancelSystem);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallBtlGetSkilltbl2);

s32 sbtlCheckBit(u32 *bits, s32 bit)
{
    return (*bits >> bit) & 1;
}

void sbtlSetBit(u32 *bits, s32 no) {
    *bits |= 1 << no;
}

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlResetBit);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallBtlGetMotionVisualDataSBtlVi);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallBtlSearchUnitID4);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlGetNearTarget_Random);

INCLUDE_ASM(const s32, "code_0027A1E8", ShugouKaijyoSub);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallBtlGetNakama3);

INCLUDE_ASM(const s32, "code_0027A1E8", Check_Stls);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlReset_SupportAction_Shugou);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlShugouKaijyo);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlNakamaWarpReset);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallMemset4);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlLeaf7);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallBtlGetBadStatus2);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlRefSbtl37);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlRefSbtl12);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallBtlResetAction2);

INCLUDE_ASM(const s32, "code_0027A1E8", KaihiInit_Sub);

void sbtlKaihiInit(struct SBTLUNIT_WORK *wk)
{
    KaihiInit_Sub(wk, 0);
}

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlKaihiInitNakamaSub2);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallGetScreenDir2);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlPlayerRJigenKiriProc);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCheckSkillEquation);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCheckTargetEquation);

s32 SEquationno_00(struct SBTLUNIT_WORK *wk, s16 skillid) {
    struct SBTLSKILLP_TBL *tbl = sbtlGetSkilltbl(skillid);
    struct SBTLUNIT_WORK *unit;
    s32 i;

    if (tbl->area & 0x4000) {
        for (i = 0; i < wk_sbtl->field404->count; i++) {
            unit = wk_sbtl->field404->pUnitL[i].pUnit;
            if (unit != NULL && sbtlCheckDamageTarget(unit)) {
                return 1;
            }
        }
    } else {
        for (i = 0; i < wk_sbtl->field408->count; i++) {
            unit = wk_sbtl->field408->pUnitL[i].pUnit;
            if (unit != NULL && sbtlCheckDamageTarget(unit)) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallBtlGetSkilltbl3);

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_02);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallBtlGetSkilltbl4);

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_04);

s32 SEquationno_05(struct SBTLUNIT_WORK *wk, s16 skillid) {
    u32 flg = sbtlGetSkilltbl(skillid)->flg;
    struct SBTLUNIT_WORK *unit;
    s32 i;

    if ((flg & 4) && (flg & 0x81F000)) {
        for (i = 0; i < wk_sbtl->field404->count; i++) {
            unit = wk_sbtl->field404->pUnitL[i].pUnit;
            if (unit != NULL && sbtlCheckDamageTarget(unit)) {
                if (unit->unit.now.maxhp != 0 && 100 * unit->unit.now.hp / unit->unit.now.maxhp < 40) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_06);

s32 SEquationno_07(struct SBTLUNIT_WORK *wk, s16 skillid) {
    s32 type = sbtlGetSkilltbl(skillid)->type & 0x2000;

    return type != 0;
}

s32 SEquationno_08(struct SBTLUNIT_WORK *wk, s16 skillid) {
    u32 flg = sbtlGetSkilltbl(skillid)->flg;
    struct SBTLUNIT_WORK *unit;
    s32 i;

    if ((flg & 4) && (flg & 0x81F000)) {
        for (i = 0; i < wk_sbtl->field404->count; i++) {
            unit = wk_sbtl->field404->pUnitL[i].pUnit;
            if (unit != NULL && sbtlCheckDamageTarget(unit)) {
                if (unit->unit.now.maxhp != 0 && 100 * unit->unit.now.hp / unit->unit.now.maxhp < 70) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

s32 SEquationno_09(struct SBTLUNIT_WORK *wk, s16 skillid) {
    struct SBTLSKILLP_TBL *tbl = sbtlGetSkilltbl(skillid);
    struct SBTLUNIT_WORK *unit;
    s32 i;

    if ((tbl->attr & 1) && (tbl->flg & 1)) {
        for (i = 0; i < wk_sbtl->field408->count; i++) {
            unit = wk_sbtl->field408->pUnitL[i].pUnit;
            if (unit != NULL && sbtlCheckDamageTarget(unit) && (unit->flg2 & 0x10)) {
                return 1;
            }
        }
    }
    return 0;
}

s32 SEquationno_0a(struct SBTLUNIT_WORK *wk, s16 skillid) {
    struct SBTLSKILLP_TBL *tbl = sbtlGetSkilltbl(skillid);
    struct SBTLUNIT_WORK *unit;
    s32 i;

    if (tbl->flg & 1) {
        for (i = 0; i < wk_sbtl->field408->count; i++) {
            unit = wk_sbtl->field408->pUnitL[i].pUnit;
            if (unit != NULL && sbtlCheckDamageTarget(unit) && sbtlCheckAisyoEx(unit, tbl->attr, NULL) == 0x4000) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_0b);

s32 SEquationno_0c(struct SBTLUNIT_WORK *wk, s16 skillid) {
    s32 result = 0;
    s32 type = sbtlGetSkilltbl(skillid)->type & 0xFFF;

    if (type == 3 || type == 5) {
        result = wk->unit.now.str > wk->unit.now.tec;
    }
    return result;
}

s32 SEquationno_0d(struct SBTLUNIT_WORK *wk, s16 skillid) {
    struct SBTLSKILLP_TBL *tbl = sbtlGetSkilltbl(skillid);
    s32 i;

    for (i = 0; i < 6; i++) {
        if (SetNextTarget(wk, tbl, sbtlGetAutoSkillPara(sbtlGetAutoSkill_ID(&wk->unit, i), &wk->unit.autost_cnt, &wk->unit.autost_cnt2))) {
            return 1;
        }
    }
    if (sbtlIsAura(wk, 0x1000000) && (tbl->type & 0xFFF) == 1) {
        return 1;
    }
    return 0;
}

s32 SEquationno_0e(struct SBTLUNIT_WORK *wk, s16 skillid) {
    s32 type = sbtlGetSkilltbl(skillid)->type & 0x1000;

    return type != 0;
}

s32 SEquationno_0f(struct SBTLUNIT_WORK *wk, s16 skillid) {
    struct SBTLSKILLP_TBL *tbl = sbtlGetSkilltbl(skillid);

    if (wk_sbtl->panel->setdata[GetMeireiNo(wk)].meirei_no != 6) {
        return 0;
    }
    if (tbl->s_type && tbl->para != 0) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_10);

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_11);

s32 SEquationno_12(struct SBTLUNIT_WORK *wk, s16 skillid) {
    return sbtlCheckCost_SkillID(wk, skillid, 0);
}

s32 SEquationno_13(struct SBTLUNIT_WORK *wk, s16 skillid) {
    struct SBTLSKILLP_TBL *tbl = sbtlGetSkilltbl(skillid);
    s32 i;

    for (i = 0; i < 6; i++) {
        if (((tbl->s_type >> i) & 1) && tbl->para > 0 && wk_sbtl->panel->setdata[GetMeireiNo(wk)].meirei_no == 6) {
            return 1;
        }
    }
    return 0;
}

s32 SEquationno_14(struct SBTLUNIT_WORK *wk, s16 skillid) {
    struct SBTLSKILLP_TBL *tbl = sbtlGetSkilltbl(skillid);
    struct SBTLUNIT_WORK *unit;
    u32 badst;
    s32 i;

    if ((tbl->flg & 0x10004) == 0x10004) {
        badst = tbl->badst;
        for (i = 0; i < wk_sbtl->field404->count; i++) {
            unit = wk_sbtl->field404->pUnitL[i].pUnit;
            if (unit != NULL && sbtlCheckDamageTarget(unit)) {
                if ((unit->unit.badStatus & badst) != 0 && (badst & 0x8000) == 0) {
                    return 1;
                }
            }
        }
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_15);

s32 SEquationno_16(struct SBTLUNIT_WORK *wk, s16 skillid) {
    u32 flg = sbtlGetSkilltbl(skillid)->flg;
    struct SBTLUNIT_WORK *unit;
    s32 i;

    if ((flg & 4) && (flg & 0x81F000)) {
        for (i = 0; i < wk_sbtl->field404->count; i++) {
            unit = wk_sbtl->field404->pUnitL[i].pUnit;
            if (unit != NULL && sbtlCheckDamageTarget(unit)) {
                if (unit->unit.now.maxhp != 0 && 100 * unit->unit.now.hp / unit->unit.now.maxhp < 20) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_17);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlAI_SetSeikaku_Action);

s32 sbtlCheckArea(struct SBTLUNIT_WORK *wk, f32 r, const sdfVector4_t *pos)
{
    s32 i;
    struct SBTLUNIT_WORK *unit;

    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        unit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (unit != NULL && sbtlCheckDamageTarget(unit) && (unit->field414 & 7) && (unit->field414 & 7) != (wk->field414 & 7)) {
            if (sbtlCheckCrossCircle(pos, r, &unit->pos, unit->model->field11C)) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCheckAreaCount);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCheckArea_Atk);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCheckArea_Masic);

INCLUDE_ASM(const s32, "code_0027A1E8", SetOrderTarget);

s32 SetNextTarget(struct SBTLUNIT_WORK *wk, struct SBTLSKILLP_TBL *tbl, s32 no) {
    switch (no) {
    case 13:
        if ((tbl->type & 0xFFF) != 1) {
            break;
        }
        return 1;
    case 14:
        if ((tbl->type & 0xFFF) != 3) {
            break;
        }
        return 1;
    case 15:
        if (!(tbl->attr & (1 << wk->unit.autost_cnt) & 0xFFF)) {
            break;
        }
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0027A1E8", SetNextTarget_sbtlEquat);

INCLUDE_ASM(const s32, "code_0027A1E8", CheckEnemyTarget);

s32 sbtlGetTargetUnit(struct SBTLUNIT_WORK *wk) {
    struct SBTLSKILLP_TBL *tbl = sbtlGetSkilltbl(wk->skillid);
    u32 flg;

    if (CheckBSTTarget(wk, tbl)) {
        return 1;
    }
    flg = tbl->flg;
    if (flg & 1) {
        return TEquationno_01(wk);
    }
    if (flg & 4) {
        if (flg & 0x10000) {
            return TEquationno_0d(wk);
        }
        return TEquationno_0c(wk);
    }
    if (flg & 0x200) {
        return TEquationno_0e(wk);
    }
    return TEquationno_01(wk);
}

s32 CheckBSTTarget(struct SBTLUNIT_WORK *wk, struct SBTLSKILLP_TBL *tbl) {
    BST bst = sbtlGetBadStatus(&wk->unit);

    if (!(tbl->area & 0x4000) && (bst == BST_BOUDO || bst == BST_MAHUU)) {
        if (wk->aiwk->mode == 18) {
            wk->model->field104 = wk->field1A0;
        } else {
            if (wk->aiwk->bstside & 1) {
                return 0;
            }
            wk->model->field104 = wk_sbtl->targetUnitID;
        }
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0027A1E8", TEquationno_01);

INCLUDE_ASM(const s32, "code_0027A1E8", TEquationno_0c);

INCLUDE_ASM(const s32, "code_0027A1E8", TEquationno_0d);

INCLUDE_ASM(const s32, "code_0027A1E8", TEquationno_0e);

s32 sbtlCheckScriptData(s32 devilId)
{
    return sbtlGetAITBL(devilId)->scr_no != 0;
}

s8 CheckAIScript(struct SBTLUNIT_WORK *wk)
{
    AITBLDATA_WORK *aitbl = sbtlGetAITBL(wk->unit.devilId);

    if (aitbl->scr_no == 0) {
        return 0;
    }
    if (wk_sbtl->hAIScript == NULL) {
        return 0;
    }
    if (wk->model->mode == 9 || wk->model->mode == 10) {
        return 0;
    }
    if (wk_sbtl->hAIScript->MesHandle >= 0) {
        itfMesMngSetVar_itf(wk_sbtl->hAIScript->MesHandle, 0, wk->unit.devilId, 0);
    }
    wk->aiwk->scr_actcheckflg = 0;
    scrStartProcessProcedure(wk_sbtl->hAIScript, aitbl->scr_no);
    wk_sbtl->scrUnit = wk;
    scrRunScript(wk_sbtl->hAIScript);
    return 1;
}

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlLeaf64);

SBTLAI_WORK *Create_sbtlAI(void)
{
    sdfMemHandle_t *handle;
    SBTLAI_WORK *aiwk;

    handle = sdfMemAllocDirect(sizeof(SBTLAI_WORK));
    aiwk = sdfMemLock(handle);
    memset(aiwk, 0, sizeof(SBTLAI_WORK));
    aiwk->handle = handle;
    aiwk->mode = 0;
    aiwk->stat = 1;
    aiwk->field4C = 0;
    return aiwk;
}

s32 Proc_sbtlAI(SBTLAI_WORK *aiwk)
{
    if (aiwk != NULL) {
        sdfMemFreeDirect(aiwk->handle);
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_0027A1E8", IsKaja);

s32 IsEnemyZenmetsu(void) {
    s32 i;
    struct SBTLUNIT_WORK *pUnit;

    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        pUnit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (pUnit != NULL && (pUnit->field414 & 2) && CheckLiveTarget(pUnit)) {
            return 0;
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlSetByct);

void sbtlProc_YoyakuEndChk(struct SBTLUNIT_WORK *wk) {
    struct SBTLSKILLP_TBL *ptbl = sbtlGetSkilltbl(wk->aiwk->rsvSkl);

    if (!sbtlCheckCost(wk, 0)) {
        wk->aiwk->rsvTgt = -1;
        wk->aiwk->rsvSkl = 0;
        return;
    }
    if (ptbl->flg2 & 1) {
        if (wk->aiwk->usedSkl == sbtlCallBtlGetSkilltbl23(wk->aiwk->rsvSkl, NULL)) {
            wk->aiwk->rsvTgt = -1;
            wk->aiwk->rsvSkl = 0;
        }
    } else if (wk->aiwk->usedSkl == wk->aiwk->rsvSkl) {
        wk->aiwk->rsvSkl = 0;
        wk->aiwk->rsvTgt = -1;
    }
}

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlProc_YoyakuEnd);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallMemset5);

INCLUDE_ASM(const s32, "code_0027A1E8", CheckActionEquation);

s32 sbtlSetAIMode(struct SBTLUNIT_WORK *wk)
{
    struct SBTLSKILLP_TBL *skill = sbtlGetSkilltbl(wk->skillid);

    if (wk->skillid == 288 || wk->skillid == 0) {
        wk->aiwk->mode = 0;
        return 1;
    }
    switch (skill->type & 0xFFF) {
    case 1:
    case 4:
    case 12:
    case 13:
        wk->aiwk->mode = 1;
        wk->aiwk->scrflag |= 0x10000;
        break;
    case 2:
    case 3:
    case 5:
    case 9:
    case 10:
    case 11:
    case 14:
    case 15:
    case 16:
        wk->aiwk->mode = 8;
        wk->aiwk->scrflag |= 0x10000;
        break;
    case 6:
    case 7:
    case 8:
    default:
        wk->aiwk->mode = 0;
        break;
    }
    return 1;
}

s32 sbtlCheckActionEquationSub(void)
{
    return 0;
}

s32 sbtlProcScript(struct SBTLUNIT_WORK *wk)
{
    if (wk == NULL || (wk->status == 10 && !(wk->aiwk->scrflag & 0x120000)) || wk->status == 11
        || wk->status == 14 || wk->model->mode == 9 || wk->status == 30 || wk->mode < 5) {
        return 0;
    }
    return CheckAIScript(wk);
}

s32 sbtlCreateAIScript(void)
{
    wk_sbtl->hAIScript = scrRegistScript2(ai_scr);
    return 1;
}

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallReleaseScript);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallRegistScript2);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallReleaseScript2);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlRefSbtl13);

/* NON_MATCHING: 41 of 48 words match.  */
#ifdef NON_MATCHING
void Init_sbtlScript(struct SBTLUNIT_WORK *wk) {
    s32 i;

    wk->aiwk->scrTargetCode = 0x20000;
    wk->aiwk->scrSkillID = -1;
    wk->aiwk->scrflag = 0;
    wk->aiwk->act_p = 0;
    wk->aiwk->scrStep = 0;
    wk->aiwk->scrStep2 = 0;
    wk->aiwk->scrStep3 = 0;
    wk->aiwk->scrListID = -1;
    for (i = 0; i < 5; i++) {
        wk->aiwk->scrMagNum[i] = 0;
        wk->aiwk->scrMagHp[i] = 0;
        wk->aiwk->scrMagFlg[i] = 0;
    }
    if (!(wk_sbtl->scrFlag & 0x40000) && (wk->field414 & 1) && !(wk->field414 & 0x80) && wk_sbtl->battleMode != 3) {
        wk->vstatus |= 0x2000000;
    }
}
#else
INCLUDE_ASM(const s32, "code_0027A1E8", Init_sbtlScript);
#endif

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCheckSetHITNONE);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlEncountInit);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlDeath);

void Delete_sbtlScript(struct SBTLUNIT_WORK *wk) {
    s32 id = wk->aiwk->scrListID;

    if (id != -1) {
        wk_sbtl->scrUnitList[id] = NULL;
        wk_sbtl->scrUnitNum--;
    }
}

void sbtlBattleStart(SBTL_STAGE_DATA *stage)
{
    if (stage->flags & 0x20) {
        sbtlSupport_BattleStop(1, 1, 0);
    }
}

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlSetAttr);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCheckHuttobi);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlBtlCheckBadStatusSBtlCalcSub);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCheckSkillCancel);

s32 sbtlCheckEnshutuSummonUnit(struct SBTLUNIT_WORK *wk) {
    if (wk == NULL || !(wk_sbtl->scrFlag & 0x40000)) {
        return 0;
    }
    if ((wk->field414 & 1) && !(wk->field414 & 0x80) && wk_sbtl->battleMode != 3) {
        return 1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "code_0027A1E8", sbtlCheckSetHITNONETable);

