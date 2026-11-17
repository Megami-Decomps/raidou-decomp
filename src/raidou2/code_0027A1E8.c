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

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCheckKajaAnyEx);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCheckKajaAny);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallBtlGetSkilltbl);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCheckEnableUseKaja);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlBtlCheckEnableUseSkillSub);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCheckEnableUseSkill);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlResetBtlMagicStatus);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlGetBothSide);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlUnit_CheckSummon_LoadExecOk_sub);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCheckSummon_LoadExecOk);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlGetNakamaUnit);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCancelSystemSub);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallBtlGetPChara2);

INCLUDE_ASM(const s32, "code_0027A1E8", SetFirstAtk_Core);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlSetVibe);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlGetPlayerID);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlRefSbtl10);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallObjectBaseSetPadControl);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlResetAction);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlRefSbtl36);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlActiveAction);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlResetATKEffect);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlResetCombine);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlLeaf63);

s32 sbtlNormalProcNakamaSub(struct SBTLUNIT_WORK *wk)
{
    return wk->status < 3;
}

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlSummonKey);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlSetNSTOP);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlResetNSTOP);

INCLUDE_RODATA(const s32, "code_0027A1E8", sbtlSetVibeTable);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallGetActiveScene2);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCheckMove_BadStatus);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlGetRandomTarget);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlSetSummonFunc);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallBtlGetNakama2);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallHirukoYukaGoIdx2B);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlResetColor);

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

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlEscapeCheck);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlRefSbtl11);

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

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlSetBit);

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

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_00);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallBtlGetSkilltbl3);

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_02);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallBtlGetSkilltbl4);

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_04);

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_05);

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_06);

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_07);

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_08);

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_09);

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_0a);

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_0b);

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_0c);

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_0d);

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_0e);

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_0f);

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_10);

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_11);

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_12);

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_13);

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_14);

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_15);

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_16);

INCLUDE_ASM(const s32, "code_0027A1E8", SEquationno_17);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlAI_SetSeikaku_Action);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCheckArea);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCheckAreaCount);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCheckArea_Atk);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCheckArea_Masic);

INCLUDE_ASM(const s32, "code_0027A1E8", SetOrderTarget);

INCLUDE_ASM(const s32, "code_0027A1E8", SetNextTarget);

INCLUDE_ASM(const s32, "code_0027A1E8", SetNextTarget_sbtlEquat);

INCLUDE_ASM(const s32, "code_0027A1E8", CheckEnemyTarget);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlGetTargetUnit);

INCLUDE_ASM(const s32, "code_0027A1E8", CheckBSTTarget);

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

INCLUDE_ASM(const s32, "code_0027A1E8", IsEnemyZenmetsu);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlSetByct);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlProc_YoyakuEndChk);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlProc_YoyakuEnd);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCallMemset5);

INCLUDE_ASM(const s32, "code_0027A1E8", CheckActionEquation);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlSetAIMode);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCheckActionEquationSub);

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

INCLUDE_ASM(const s32, "code_0027A1E8", Init_sbtlScript);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCheckSetHITNONE);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlEncountInit);

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlBattleSummonEffectSub);

INCLUDE_ASM(const s32, "code_0027A1E8", Delete_sbtlScript);

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

INCLUDE_ASM(const s32, "code_0027A1E8", sbtlCheckEnshutuSummonUnit);

INCLUDE_RODATA(const s32, "code_0027A1E8", sbtlCheckSetHITNONETable);

