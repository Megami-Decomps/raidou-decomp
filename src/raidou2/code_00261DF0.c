#include "common.h"
#include "sbtlWork.h"
#include "sdk_libc.h"
#include "ps2asm.h"
#include "dds3KernelCore.h"
#include "newdata.h"
#include "sBtlUnit.h"
#include "sdfMemory.h"
#include "effManager.h"
#include "fclMisc.h"
#include "evtUnitManager.h"
#include "sBtlEncount.h"
#include "sBtlScene.h"
#include "sBtlCalc.h"
#include "code_002F5CF0.h"
#include "sBtlAI.h"
#include "itf.h"
#include "EventBit.h"
#include "code_r1_0010DF68.h"
#include "code_r1_0027CBE8.h"
#include "sBtlCmndMenuNew.h"
#include "dds3KernelDraw.h"
#include "dds3KernelDraw_0010A750.h"
#include "sdfSound.h"
#include "sbtlDMG.h"
#include "sBtlEXP.h"
#include "sBtlSM.h"
#include "sBtlEff.h"
#include "sBtlItem.h"
#include "sBtlSound.h"
#include "sBtlDraw.h"
#include "dat.h"
#include "sbtlActAI.h"
#include "sBtlMenu.h"
#include "rstInit.h"
#include "sdfSystem.h"
#include "code_001C32D8.h"
#include "sBtlScript.h"
#include "dds3KernelMain.h"
#include "sbattle.h"
#include "code_0027A1E8.h"
#include "sdfMath.h"
#include "dds3Randomize.h"
#include "sBtlPlayer.h"
#include "BsFlow.h"
#include "dds3AdminiProcess.h"
#include "fld.h"
#include "fldProcess.h"
#include "sndAt3.h"

INCLUDE_ASM(const s32, "code_00261DF0", sbtlRaidouKatana);

s32 sbtlSbattle(SBTL_START_DATA *start, s8 flag)
{
    dds3ProcessID_t *process;

    sbtlSbattleData = dds3GetKernelCount();
    sbtlBGMLOADSTARTData = sdfGetRunTimeCountBase();
    main_data = *start;
    if (flag == 0) {
        ReloadTableData();
        rstSetFlag(0x20000);
    }
    sbtlForceDelete_EffectWork(2);
    sbtlForceDelete_AttackHitData();
    sbtlInitEffectWork();
    sbtlInitLoadAttackHitData_pData();
    Init_sbtl(start->val30, start->val28);
    process = dds3StartProcess(sbtlData85, 1015, 0, 0, sbtlSbattle3, sbtlFunc, NULL);
    wk_sbtl->process = process;
    dds3LinkProcess(process, dds3StartProcess("sbattle_draw", 11020, 0, 0, sbtlSbattle4, NULL, NULL));
    dds3LinkProcess(process, dds3StartProcess("sbattle_draw2", 11021, 0, 0, DrawProc2_sbtl, NULL, NULL));
    sbtlSceneLoad();
    Init_sbtlUnit(flag);
    sbtlCreateAIScript();
    sbtlInitDropWork();
    RestartBattle();
    sbtlStopVibe();
    evtExitSBtlSub(0);
    return 1;
}

/* NON_MATCHING: 391 of 411 words match.  */
#ifdef NON_MATCHING
s32 Exit_sbtl(s32 restart)
{
    struct SBTLUNIT_WORK *nakama;
    s32 ok;
    s32 i;

    ok = 1;
    if (wk_sbtl == NULL || dds3SearchProcessName(sbtlData85) == NULL) {
        return 1;
    }
    if (wk_sbtl->unitList != NULL) {
        for (i = 0; i < 2; i++) {
            nakama = sbtlGetNakama(i);
            if (nakama == NULL) {
                DDS3_GBWK->stack_p[i] = -1;
            } else {
                DDS3_GBWK->stack_p[i] = datGetStockDevilIndex(nakama->unit.devilId);
            }
        }
    }
    wk_sbtl->flg |= 8;
    if (!(wk_sbtl->flg2 & 0x80000000)) {
        if (!(wk_sbtl->flg & 0x20)) {
            if (!ProcessCheck_rstInit()) {
                rstSetEncountDlcID(wk_sbtl->stageNo);
                if (wk_sbtl->battleMode == 3) {
                    rstSetFlag(0x80000);
                }
                sbtlCheckDrop();
                sbtlCallEffectShutdown3(0);
                if (!restart) {
                    if (wk_sbtl->stageNo == 477) {
                        rstSetFlag(0x10000);
                    }
                    ProcessStart_rstInit();
                    rstResultStart();
                    return 1;
                }
                dds3AdminiCallSequence(12, NULL, 0, 0);
                rstResultStart();
                sndBgmFadeOut();
            } else {
                if (!rstCheckFlag(0x40)) {
                    return 1;
                }
                wk_sbtl->flg2 |= 0x80000000;
            }
        } else if (!rstCheckFlag(0x40000)) {
            rstSetFlag(0x40000);
            sndBgmFadeOut();
            sbtlCallEffectShutdown3(0);
            return 1;
        }
    }
    wk_sbtl->flg2 |= 4;
    sbtlStopVibe();
    sbtlDrawON();
    if (!sbtlCheckUnit_Exec()) {
        return 0;
    }
    if (wk_sbtl->list3E8 != NULL) {
        sbtlDeleteUnitList(wk_sbtl->list3E8);
        wk_sbtl->list3E8 = NULL;
    }
    if (wk_sbtl->tgtList != NULL) {
        sbtlDeleteUnitList(wk_sbtl->tgtList);
        wk_sbtl->tgtList = NULL;
    }
    if (wk_sbtl->list2D4 != NULL) {
        sbtlDeleteUnitList(wk_sbtl->list2D4);
        wk_sbtl->list2D4 = NULL;
    }
    if (wk_sbtl->field404 != NULL) {
        sbtlDeleteUnitList(wk_sbtl->field404);
        wk_sbtl->field404 = NULL;
    }
    if (wk_sbtl->field408 != NULL) {
        sbtlDeleteUnitList(wk_sbtl->field408);
        wk_sbtl->field408 = NULL;
    }
    if (wk_sbtl->field40C != NULL) {
        sbtlDeleteUnitList(wk_sbtl->field40C);
        wk_sbtl->field40C = NULL;
    }
    if (!sbtlCheckLoadEnd_EffectWork()) {
        return 0;
    }
    if (wk_sbtl->effList != NULL) {
        ok = 0;
        if (sbtlDeleteEffList(wk_sbtl->effList)) {
            if (wk_sbtl->animList != NULL) {
                sbtlDeleteAnimList(wk_sbtl->animList);
                wk_sbtl->animList = NULL;
            }
            if (wk_sbtl->hitList != NULL) {
                sbtlDeleteHitList(wk_sbtl->hitList);
                wk_sbtl->hitList = NULL;
            }
            if (wk_sbtl->modelBindings != NULL) {
                sbtlBattleEnd(wk_sbtl->modelBindings);
                wk_sbtl->modelBindings = NULL;
            }
            if (wk_sbtl->dvltkList != NULL) {
                sbtlDeleteDvltkList(wk_sbtl->dvltkList);
                wk_sbtl->dvltkList = NULL;
            }
            if (wk_sbtl->expList != NULL) {
                sbtlDeleteEXPList(wk_sbtl->expList);
                wk_sbtl->expList = NULL;
            }
            if (wk_sbtl->stp != NULL) {
                sbtlDeleteSTPList(wk_sbtl->stp);
                wk_sbtl->stp = NULL;
            }
            if (wk_sbtl->itemList != NULL) {
                sbtlDeleteItemList(wk_sbtl->itemList);
                wk_sbtl->itemList = NULL;
            }
            if (wk_sbtl->bulletList != NULL) {
                sbtlDeleteItemList(wk_sbtl->bulletList);
                wk_sbtl->bulletList = NULL;
            }
            if (wk_sbtl->sysmsgList != NULL) {
                sbtlDeleteSMList(wk_sbtl->sysmsgList);
                wk_sbtl->sysmsgList = NULL;
            }
            if (wk_sbtl->effdelList != NULL) {
                sbtlDeleteDelList(wk_sbtl->effdelList);
                wk_sbtl->effdelList = NULL;
            }
            sbtlProcessEnd();
            Delete_sbtlCmndMenuNew();
            wk_sbtl->effList = NULL;
            return 0;
        }
    }
    if (ProcessCheck() == 1) {
        ok = 0;
    }
    fldReleaseTitleMiniTexture();
    if (!sbtlDeleteOnData(0)) {
        ok = 0;
    }
    if (!sbtlForceDelete_EffectWork(1)) {
        ok = 0;
    }
    if (!sbtlEvent_DeleteEventScript()) {
        ok = 0;
    }
    sbtlForceDelete_EffectWork(2);
    sbtlForceDelete_AttackHitData();
    sbtlInitEffectWork();
    sbtlInitLoadAttackHitData_pData();
    fldHirukoYukaGo_Idx2_B(0x80808080);
    if (!(wk_sbtl->flg & 0x800000)) {
        ok = 0;
        if (wk_sbtl->effList == NULL && Exit_sbtlUnit() && sbtlFree_sbtlScene()) {
            evtCallGetProcessData28();
            fclFilterOFF();
            wk_sbtl->flg |= 0x800000;
            return 0;
        }
    }
    if (!ok) {
        return 0;
    }
    if (!sbtlDeleteMainPanel()) {
        return 0;
    }
    if (wk_sbtl->flg & 0x80) {
        fclDestroyMessage();
        fclEndMessage();
    }
    Exit_sbtlData = wk_sbtl->stageNo;
    Exit_sbtlData2 = wk_sbtl->fieldE84;
    sbtlPacData_Free();
    scrAllReleaseScript();
    if (wk_sbtl->flg & 0x20) {
        Exit2Set(0);
        if (wk_sbtl->battleMode == 3) {
            rstSetFlag(0x80000);
        }
        ProcessStart_rstInit();
    } else if (wk_sbtl->flg & 0x1000000) {
        Exit2Set(2);
    } else {
        Exit2Set(1);
    }
    RestartBattle();
    sbtlLeaf57();
    if (dds3SearchProcessPriority(0x3F7) != NULL) {
        dds3KillProcess(wk_sbtl->process, 1);
    }
    wk_sbtl->field424 = 0;
    wk_sbtl->field430 = 0;
    wk_sbtl->flg &= 0xFB7FFFEC;
    evtExitSBtlSub(0);
    dds3FadeIn2(1);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_00261DF0", Exit_sbtl);
#endif

s32 StopBattle(void) {
    if (dds3SearchProcessName(sbtlData85) == NULL) {
        return 0;
    }
    if (wk_sbtl == NULL) {
        return 0;
    }
    wk_sbtl->flg |= 0x40;
    return 1;
}

s32 IsSceanStop(void) {
    s32 stop;

    if (wk_sbtl == NULL) {
        return 0;
    }
    stop = (wk_sbtl->flg >> 6) & 1;
    return wk_sbtl->hitStop || stop;
}

s32 sbtlCheckSummon(s32 idx)
{
    SBTL_PLAYER_DATA *pd;
    s32 ret = 0;

    pd = sbtlGetPlayerData();
    if ((wk_sbtl->battleMode == 0 || wk_sbtl->battleMode == 2) && wk_sbtl->summonIndex[0] >= 0 && wk_sbtl->field64C > 0) {
        if (sbtlBtlCheckSummonSub(&pd->stock[wk_sbtl->summonIndex[idx]])) {
            ret = sbtlCheckMagCost(&pd->stock[wk_sbtl->summonIndex[idx]], pd->stock[wk_sbtl->summonIndex[idx]].devilId, 0) != 0;
        }
    }
    return ret;
}

INCLUDE_ASM(const s32, "code_00261DF0", CheckStopBattle);

s32 RestartBattle(void)
{
    if (dds3SearchProcessName(sbtlData85) == NULL) {
        return 0;
    }
    if (wk_sbtl == NULL) {
        return 0;
    }
    wk_sbtl->flg &= ~0x40;
    return 1;
}

INCLUDE_ASM(const s32, "code_00261DF0", ReloadTableData);

s32 LoadOnMemoryData_sbtl(void)
{
    sdfMemHandle_t *handle;

    sbtlInitEffect();
    sbtlMap_Init();
    btlLoadSbPack();
    handle = sdfMemAllocDirect(sizeof(SBTL_WORK));
    wk_sbtl = sdfMemLock(handle);
    memset(wk_sbtl, 0, sizeof(SBTL_WORK));
    wk_sbtl->handle = handle;
    wk_sbtl->mode = 1;
    handle = sdfMemAllocDirect(sizeof(SBTL_PACDATA_WORK));
    sbtlData84 = sdfMemLock(handle);
    memset(sbtlData84, 0, sizeof(SBTL_PACDATA_WORK));
    sbtlData84->handle = handle;
    handle = sdfMemAllocDirect(sizeof(SBTL_WORK2));
    sbtlData83 = sdfMemLock(handle);
    memset(sbtlData83, 0, sizeof(SBTL_WORK2));
    sbtlData83->handle = handle;
    sbtlInitEffectWork();
    sbtlInitLoadAttackHitData_pData();
    btlLoadInterfaceSprite();
    return 1;
}

void sbtlMap_Init(void)
{
    memset(btlLoadSbPackData, 0, sizeof(btlLoadSbPackData));
}

INCLUDE_ASM(const s32, "code_00261DF0", sbtlInitPlayerData);

SBTL_PLAYER_DATA *sbtlGetPlayerData(void)
{
    return &DDS3_GBWK->playerData;
}

/* NON_MATCHING: 2 of 22 words match.  */
#ifdef NON_MATCHING
struct sdatUnitWork_t *sbtlRefDDS3GBWK2(u32 index) {
    sdatUnitWork_t *unit;
    sdatUnitWork_t *ret = NULL;

    if (index < 20) {
        unit = &DDS3_GBWK->playerData.stock[index];
        ret = unit;
        if (unit->devilId == 0 || !(unit->flag & 1)) {
            ret = NULL;
        }
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "code_00261DF0", sbtlRefDDS3GBWK2);
#endif

s32 sbtlSetData_Unit_Stock(struct SBTLUNIT_WORK *wk, s32 stock)
{
    sdatUnitWork_t *unit = &DDS3_GBWK->playerData.stock[stock];
    s32 haveskill[17];

    memcpy(haveskill, unit->haveskill, sizeof(haveskill));
    *unit = wk->unit;
    memcpy(unit->haveskill, haveskill, sizeof(haveskill));
    return 1;
}

s32 sbtlGetData_Stock_Unit(struct SBTLUNIT_WORK *wk, s32 stock)
{
    sdatUnitWork_t *unit = &DDS3_GBWK->playerData.stock[stock];
    s32 haveskill[19];
    s32 found;
    s32 i;
    s32 j;
    s32 skill;

    sbtlCheckStockPara(unit);
    wk->unit = *unit;
    wk->aisyo_bkup = wk->unit.aisyo;
    memset(haveskill, 0, sizeof(wk->unit.haveskill));
    haveskill[0] = SKLID_ATK;
    haveskill[1] = SKLID_OFS_PROGRAM;
    for (i = 0; i < 17; i++) {
        skill = wk->unit.haveskill[i];
        if (skill != SKLID_ATK && skill != 0 && skill != SKLID_OFS_PROGRAM) {
            haveskill[i + 2] = skill;
        }
    }
    memcpy(wk->unit.haveskill, haveskill, sizeof(wk->unit.haveskill));
    found = 0;
    for (i = 0; i < 17; i++) {
        if (wk->unit.autoskill[i] != 0) {
            for (j = 0; j < 4; j++) {
                if (wk->unit.battleskill[j] == wk->unit.haveskill[i] && sbtlCallBtlGetSkilltbl15(wk->unit.battleskill[j])) {
                    found = 1;
                    wk->aiwk->RepeatOdrSkl = wk->unit.haveskill[i];
                    break;
                }
            }
            if (found) {
                break;
            }
        }
    }
    if (!found) {
        for (i = 0; i < 17; i++) {
            unit->autoskill[i] = 0;
            wk->unit.autoskill[i] = 0;
        }
        unit->autoskill[0] = 1;
        wk->unit.autoskill[0] = 1;
        wk->aiwk->RepeatOdrSkl = SKLID_ATK;
    }
    wk->aiwk->dfltOdrSkl = SKLID_ATK;
    wk->aiwk->RepeatOdrTgt = -1;
    wk->aiwk->RepeatOdrStk = -1;
    wk->aiwk->dfltOdrTgt = -1;
    wk->aiwk->dfltOdrStk = -1;
    return 1;
}

INCLUDE_ASM(const s32, "code_00261DF0", sbtlCheckLive);

s32 sbtlBtlCheckSummonSub(sdatUnitWork_t *unit) {
    if (unit->flag & 1) {
        return (unit->badStatus & 0x8C000) == 0;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_00261DF0", sbtlCheckStockPara);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlCallBtlGetItemtbl);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlSetMapFade);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlMapFadeProc);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlSetMapFade_H);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlMapFadeProc_H);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlRefSbtl);

/* NON_MATCHING: 10 of 16 words match.  */
#ifdef NON_MATCHING
s32 sbtlSetHitStop(s32 sttime, s32 cmpcnt) {
    if (wk_sbtl->fieldCA2) {
        return 1;
    }
    if (wk_sbtl->flg2 & 0x400) {
        return 1;
    }
    wk_sbtl->flg2 |= 0x400;
    wk_sbtl->hitst_sttime = sttime;
    wk_sbtl->hitst_cmpcnt = cmpcnt;
    wk_sbtl->hitst_stcnt = 0;
    wk_sbtl->hitst_cnt = 0;
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_00261DF0", sbtlSetHitStop);
#endif

/* NON_MATCHING: 70 of 73 words match.  */
#ifdef NON_MATCHING
s32 sbtlProcHitStop(void)
{
    SBTL_WORK *wk;

    if (wk_sbtl == NULL) {
        return 0;
    }
    if (wk_sbtl->flg2 & 0x400) {
        if (wk_sbtl->hitst_stcnt >= wk_sbtl->hitst_sttime) {
            wk_sbtl->hitStop = 1;
        }
        wk_sbtl->hitst_stcnt++;
        wk_sbtl->hitst_stcnt = wk_sbtl->hitst_stcnt <= 0 ? 0 : wk_sbtl->hitst_stcnt < wk_sbtl->hitst_sttime ? wk_sbtl->hitst_stcnt : wk_sbtl->hitst_sttime;
        if (IsSceanStop()) {
            wk = wk_sbtl;
            if (wk->hitst_cnt >= wk->hitst_cmpcnt) {
                wk->hitStop = 0;
                wk->hitst_cnt = 0;
                wk->flg2 &= ~0x400;
                wk->hitst_cmpcnt = 0;
            } else {
                wk->hitst_cnt++;
            }
        }
    }
    if (IsSceanStop()) {
        return 1;
    }
    if (wk_sbtl->fieldC84) {
        if (wk_sbtl->fieldC85) {
            wk_sbtl->fieldC85--;
        } else {
            wk_sbtl->fieldC85 = 1;
            sbtlSetHitStop(0, 1);
            wk_sbtl->fieldC84--;
        }
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_00261DF0", sbtlProcHitStop);
#endif

s32 sbtlSearchPacData(s32 major, s32 minor, s32 devilid)
{
    s32 i;

    for (i = 0; i < SBTL_PACDATA_NUM; i++) {
        if (sbtlData84->pac[i].key.major == major && sbtlData84->pac[i].key.minor == minor
            && sbtlData84->pac[i].key.devilid == devilid) {
            return i;
        }
    }
    for (i = 0; i < SBTL_PACDATA_NUM; i++) {
        if (sbtlData84->pac[i].key.major == 0 && sbtlData84->pac[i].key.minor == 0
            && sbtlData84->pac[i].key.devilid == 0) {
            return i;
        }
    }
    return -2;
}

s32 sbtlSearchPacData2(s32 major, s32 minor, s32 devilid)
{
    s32 i;

    i = sbtlSearchPacData(major, minor, devilid);
    if (i == -1) {
        return 1;
    }
    sbtlData84->pac[i].key.major = major;
    sbtlData84->pac[i].key.minor = minor;
    sbtlData84->pac[i].key.devilid = devilid;
    return 1;
}

/* NON_MATCHING: 124 of 126 words match.  */
#ifdef NON_MATCHING
s32 sbtlCallBtlSearchPacData(s32 major, s32 minor, s32 devilid, SBTL_HIT_PACFILE *file, s32 size)
{
    SBTLSKILLHIT_TBL *entry;
    SBTL_HIT_PACDATA *pwk;
    sdfMemHandle_t *handle;
    s32 i;
    s32 j;

    i = sbtlSearchPacData(major, minor, devilid);
    if (i == -1) {
        return 1;
    }
    entry = file->entry;
    sbtlSearchPacData2(major, minor, devilid);
    sbtlData84->pac[i].hit.num = file->num;
    for (j = 0; j < sbtlData84->pac[i].hit.num; j++) {
        handle = sdfMemAllocDirect(sizeof(SBTL_HIT_PACDATA));
        pwk = sdfMemLock(handle);
        pwk->handle = handle;
        sbtlData84->pac[i].hit.pwk[j] = pwk;
        pwk->entry = *entry;
        sbtlData84->pac[i].hit.pwk[j]->handle08 = NULL;
        sbtlData84->pac[i].hit.pwk[j]->handle10 = NULL;
        entry++;
    }
    sbtlData84->pac[i].hit.status |= 1;
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_00261DF0", sbtlCallBtlSearchPacData);
#endif

s32 sbtlSetPacData_MHT(s32 major, s32 minor, s32 devilid, void *data, s32 size)
{
    sdfMemHandle_t *handle;
    void *buf;
    s32 i;

    i = sbtlSearchPacData(major, minor, devilid);
    if (i == -1) {
        return 1;
    }
    {
        sbtlSearchPacData2(major, minor, devilid);
        handle = sdfMemAllocDirect(size);
        buf = sdfMemLock(handle);
        sbtlData84->pac[i].mht.data = buf;
        sbtlData84->pac[i].mht.handle = handle;
        sbtlData84->pac[i].mht.size = size;
        memcpy(buf, data, size);
    }
    return 1;
}

s32 DrawProc2(s32 major, s32 minor, s32 devilid, void *data, s32 size)
{
    sdfMemHandle_t *handle;
    void *buf;
    s32 i;

    i = sbtlSearchPacData(major, minor, devilid);
    if (i == -1) {
        return 1;
    }
    {
        sbtlSearchPacData2(major, minor, devilid);
        handle = sdfMemAllocDirect(size);
        buf = sdfMemLock(handle);
        sbtlData84->pac[i].file20.data = buf;
        sbtlData84->pac[i].file20.handle = handle;
        sbtlData84->pac[i].file20.size = size;
        memcpy(buf, data, size);
    }
    return 1;
}

void sbtlCallMemset(void)
{
    SBTL_PACDATA *pac;
    s32 i;

    memset(sbtlData84, 0, sizeof(sbtlData84->pac));
    pac = sbtlData84->pac;
    for (i = SBTL_PACDATA_NUM - 1; i >= 0; i--) {
        pac->mht.handle = NULL;
        pac->mht.data = NULL;
        pac->file20.handle = NULL;
        pac->file20.data = NULL;
        pac++;
    }
}

void sbtlPacData_Free(void)
{
    s32 i;
    s32 j;

    for (i = 0; i < SBTL_PACDATA_NUM; i++) {
        if (sbtlData84->pac[i].key.major || sbtlData84->pac[i].key.minor || sbtlData84->pac[i].key.devilid) {
            if (sbtlData84->pac[i].hit.status & 1) {
                for (j = 0; j < sbtlData84->pac[i].hit.num; j++) {
                    if (sbtlData84->pac[i].hit.pwk[j]->handle08) {
                        sdfMemFreeDirect(sbtlData84->pac[i].hit.pwk[j]->handle08);
                        sbtlData84->pac[i].hit.pwk[j]->handle08 = NULL;
                    }
                    if (sbtlData84->pac[i].hit.pwk[j]->handle10) {
                        sdfMemFreeDirect(sbtlData84->pac[i].hit.pwk[j]->handle10);
                        sbtlData84->pac[i].hit.pwk[j]->handle10 = NULL;
                    }
                    if (sbtlData84->pac[i].hit.pwk[j]->handle) {
                        sdfMemFreeDirect(sbtlData84->pac[i].hit.pwk[j]->handle);
                        sbtlData84->pac[i].hit.pwk[j]->handle = NULL;
                    }
                }
                sbtlData84->pac[i].hit.status = 0;
            }
            if (sbtlData84->pac[i].mht.handle) {
                sdfMemFreeDirect(sbtlData84->pac[i].mht.handle);
                sbtlData84->pac[i].mht.handle = NULL;
                sbtlData84->pac[i].mht.data = NULL;
            }
            if (sbtlData84->pac[i].file20.handle) {
                sdfMemFreeDirect(sbtlData84->pac[i].file20.handle);
                sbtlData84->pac[i].file20.handle = NULL;
                sbtlData84->pac[i].file20.data = NULL;
            }
        }
    }
}

void sbtlCallMemFreeDirect(MODELTYPE major, s32 minor, s32 devilid)
{
    s32 i;
    s32 j;

    for (i = 0; i < SBTL_PACDATA_NUM; i++) {
        if ((sbtlData84->pac[i].key.major || sbtlData84->pac[i].key.minor || sbtlData84->pac[i].key.devilid)
            && sbtlData84->pac[i].key.major == major && sbtlData84->pac[i].key.minor == minor
            && sbtlData84->pac[i].key.devilid == devilid) {
            if (sbtlData84->pac[i].hit.status & 1) {
                for (j = 0; j < sbtlData84->pac[i].hit.num; j++) {
                    if (sbtlData84->pac[i].hit.pwk[j]->handle08) {
                        sdfMemFreeDirect(sbtlData84->pac[i].hit.pwk[j]->handle08);
                        sbtlData84->pac[i].hit.pwk[j]->handle08 = NULL;
                    }
                    if (sbtlData84->pac[i].hit.pwk[j]->handle10) {
                        sdfMemFreeDirect(sbtlData84->pac[i].hit.pwk[j]->handle10);
                        sbtlData84->pac[i].hit.pwk[j]->handle10 = NULL;
                    }
                    if (sbtlData84->pac[i].hit.pwk[j]->handle) {
                        sdfMemFreeDirect(sbtlData84->pac[i].hit.pwk[j]->handle);
                        sbtlData84->pac[i].hit.pwk[j]->handle = NULL;
                    }
                }
                sbtlData84->pac[i].hit.status = 0;
            }
            if (sbtlData84->pac[i].mht.handle) {
                sdfMemFreeDirect(sbtlData84->pac[i].mht.handle);
                sbtlData84->pac[i].mht.handle = NULL;
                sbtlData84->pac[i].mht.data = NULL;
                sbtlData84->pac[i].mht.size = 0;
            }
            if (sbtlData84->pac[i].file20.handle) {
                sdfMemFreeDirect(sbtlData84->pac[i].file20.handle);
                sbtlData84->pac[i].file20.handle = NULL;
                sbtlData84->pac[i].file20.data = NULL;
            }
            sbtlData84->pac[i].key.major = 0;
            sbtlData84->pac[i].key.minor = 0;
            sbtlData84->pac[i].key.devilid = 0;
        }
    }
}

/* NON_MATCHING: 158 of 188 words match.  */
#ifdef NON_MATCHING
void Init_sbtl(s32 battleMode, s32 stageNo)
{
    s32 i;

    memset(wk_sbtl, 0, sizeof(SBTL_WORK));
    VRF_DeltaTime = 0;
    pbData = 0;
    wk_sbtl->field4334 = 1;
    wk_sbtl->returnindex[1] = -1;
    wk_sbtl->returnindex[0] = -1;
    wk_sbtl->field430C = 0;
    wk_sbtl->field432C = 0;
    wk_sbtl->field4330 = 0;
    sbtlInitSBtlSub3();
    if (battleMode >= 4) {
        wk_sbtl->battleMode = 0;
    } else if (battleMode < 0) {
        wk_sbtl->battleMode = 0;
    } else {
        wk_sbtl->battleMode = battleMode;
    }
    for (i = 0; i < 16; i++) {
        evtBitOff(i + 0xBE0);
    }
    if (stageNo == 0) {
        wk_sbtl->field4328 = 1;
        wk_sbtl->stageNo = Exit_sbtlData;
    } else {
        wk_sbtl->stageNo = stageNo;
        wk_sbtl->field4328 = 0;
    }
    wk_sbtl->mode = 1;
    wk_sbtl->mf_abgr = 0x80808080;
    wk_sbtl->mfh_abgr = 0x80808080;
    wk_sbtl->flg &= ~8;
    wk_sbtl->flg2 &= ~0x10000000;
    wk_sbtl->animList = sbtlAnimListProc();
    wk_sbtl->hitList = sbtlCreateHitList();
    wk_sbtl->effList = sbtlCreateEffList();
    wk_sbtl->modelBindings = effAllocateModelBindings();
    wk_sbtl->dvltkList = sbtlCallChipAllocZeroDirect();
    wk_sbtl->expList = InitTarSelProcessData();
    wk_sbtl->stp = evtAllocateContext();
    wk_sbtl->tgtList = NULL;
    wk_sbtl->field404 = 0;
    wk_sbtl->field408 = 0;
    wk_sbtl->field40C = 0;
    wk_sbtl->list3E8 = sbtlCreateUnitList(48);
    wk_sbtl->list2D4 = sbtlCreateUnitList(48);
    wk_sbtl->itemList = sbtlCreateItemList();
    wk_sbtl->bulletList = sbtlCreateItemList();
    wk_sbtl->sysmsgList = sbtlCreateSMList();
    wk_sbtl->effdelList = sbtlCreateDelList();
    sbtlEntryItemList(wk_sbtl->itemList);
    sbtlEntryItemList_Type(wk_sbtl->bulletList, 4);
    sbtlCallBtlCheckTutorialBattle();
    sbtlRefSbtl45();
    sbtlClearGeneratorWork();
    sbtlRefSbtl7();
    sbtlAllInit();
    ProcessStart();
    sbtlInit();
    sbtlInitSBtlSub2();
    sbtlCallMemset();
    sbtlEisyoWaitTask();
    sbtlInitAddMag();
    wk_sbtl->textCursor = wk_sbtl->textPool;
    if (wk_sbtl->field4328) {
        wk_sbtl->fieldE84 = Exit_sbtlData2;
        DDS3_GBWK->playerData.sword = wk_sbtl->fieldE84;
    } else if (wk_sbtl->fieldE84 != 0) {
        DDS3_GBWK->playerData.sword = wk_sbtl->fieldE84;
        datRefDDS3GBWK12(DDS3_GBWK->playerData.sword);
        datCalcNowParam(&sbtlGetPChara()->unit, DDS3_GBWK->playerData.sword);
    } else {
        wk_sbtl->fieldE84 = sbtlGetPlayerData()->sword;
    }
    sbtlInitLoadPlayerOnMemData();
    while (!sbtlLoadEndCheck_PlayerOnMemData()) {
    }
    while (!sbtlLoadPlayerOnMemData()) {
    }
    sbtlSetGunNum(6);
    wk_sbtl->timeLimit = 18000;
}
#else
INCLUDE_ASM(const s32, "code_00261DF0", Init_sbtl);
#endif

void SmnCtrlRun(void)
{
    if (wk_sbtl != NULL && wk_sbtl->smnCtrl.bNoOk) {
        if (wk_sbtl->smnCtrl.io) {
            if (wk_sbtl->summonflg[wk_sbtl->smnCtrl.stkId] == 2) {
                wk_sbtl->smnCtrl.bNoOk = 0;
            }
        } else if (sbtlSearchUnit_ID(wk_sbtl->unitList,
                                     wk_sbtl->stockUnitID[wk_sbtl->summonIndex[wk_sbtl->smnCtrl.aprId]]) == NULL) {
            wk_sbtl->smnCtrl.bNoOk = 0;
        }
    }
}

INCLUDE_ASM(const s32, "code_00261DF0", sbtlSbattle3);

/* NON_MATCHING: 52 of 463 words match.  */
#ifdef NON_MATCHING
s32 sbtlCheckStartBattle(void)
{
    SBTL_PLAYER_DATA *player;
    struct SBTLUNIT_WORK *wk;
    struct SBTLUNIT_WORK *pchara;
    SBTL_STAGE_DATA *stage;
    BST bst;
    s32 i;
    s32 flg;
    s32 done;
    u32 count;

    if (wk_sbtl->field424 <= 0) {
        return 0;
    }
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        wk = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (wk != NULL && (wk->field414 & 2) && (u16)(wk->mode - 1) < 2) {
            return 0;
        }
    }
    player = &DDS3_GBWK->playerData;
    if (wk_sbtl->flg < 0) {
        return 1;
    }
    wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    if (wk == NULL) {
        return 0;
    }
    flg = wk_sbtl->flg;
    if (!(flg & 0x8000)) {
        if (wk->mode == 0) {
            wk->mode = 2;
            if (wk_sbtl->battleMode == 3) {
                sbtlSetSummonEffect_Nakama(wk, 0);
            }
            return 0;
        }
        if (wk->mode < 3) {
            return 0;
        }
        wk_sbtl->flg = flg | 0x8000;
        done = 0;
        if (wk_sbtl->battleMode == 1) {
            wk->color = 0;
            wk->mode = 5;
            wk->vstatus |= 0x200000;
            bst = sbtlGetBadStatus(&wk->unit);
            if (bst != BST_BOYCOTT) {
                sbtlSetBadStatus_No(wk, SKLID_NONE, bst, 1, -1);
            }
            done = 2;
            sbtlCheckBattlePara(wk);
        }
        if (done) {
            return 0;
        }
        if (wk_sbtl->battleMode == 3) {
            return 0;
        }
        wk->color = 0;
        wk->mode = 5;
        wk->vstatus |= 0x200000;
        bst = sbtlGetBadStatus(&wk->unit);
        if (bst != BST_BOYCOTT) {
            sbtlSetBadStatus_No(wk, SKLID_NONE, bst, 1, -1);
        }
        sbtlInitRedGg(&wk_sbtl->aRedGuage[0], sbtlGetPChara()->unit.now.hp);
        sbtlCheckBattlePara(wk);
        return 0;
    }
    wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    if (wk == NULL) {
        return 0;
    }
    if (wk->mode < 4) {
        return 0;
    }
    if (wk_sbtl->member_num > 0) {
        for (i = 0; i < 2; i++) {
            if (wk_sbtl->summonIndex[i] >= 0 && sbtlBtlCheckSummonSub(&player->stock[wk_sbtl->summonIndex[i]])) {
                wk = sbtlGetNakama(i);
                if (wk == NULL) {
                    return 0;
                }
                if (!sbtlCheckDamageTarget(wk)) {
                    return 0;
                }
            }
        }
        if (!sbtlLoadEffectData_Decode(NULL)) {
            return 0;
        }
        if (!sbtlLoadAttackHitData_Decode(NULL)) {
            return 0;
        }
        if (wk_sbtl->smnCtrl.bNoOk) {
            return 0;
        }
        if (wk_sbtl->step <= 0) {
            return 0;
        }
        if (wk_sbtl->flg2 & 0x1000) {
            return 0;
        }
        slCallBitCheck();
        if (wk_sbtl->flg2 & 0x2000000) {
            return 0;
        }
        if (!(wk_sbtl->status4 & 4)) {
            return 0;
        }
        stage = &sbtlData100[wk_sbtl->stageNo];
        if (!sbtlChkBossEventStart()) {
            if ((u32)(wk_sbtl->stageNo - 752) < 16 && wk_sbtl->fieldE8C) {
                wk_sbtl->huumaTargetID = 1;
                wk_sbtl->field42C0 = 4;
                wk_sbtl->flg |= 0x80000000;
                cmpSbtlkaiwaMain();
            } else if (stage->flags & 0x36) {
                wk_sbtl->bNoEnemyName = 1;
                wk_sbtl->flg |= 0x80000000;
                IsStart(1);
            } else if (wk_sbtl->fieldE8C) {
                wk_sbtl->field42C0 = wk_sbtl->fieldE8D ? 7 : 4;
                wk_sbtl->huumaTargetID = 1;
                wk_sbtl->flg |= 0x80000000;
                cmpSbtlkaiwaMain();
            } else if ((wk_sbtl->flg2 & 0x400000)
                       || (!GetPartyLevelDfference() && !sbtlSENSEIDMGCHK() && !sbtlCallBitCheck())) {
                wk_sbtl->bNoEnemyName = 1;
                wk_sbtl->flg |= 0x80000000;
                IsStart(1);
            }
        }
        pchara = sbtlGetPChara();
        mapDebugLightSet(mapGetLight(pchara->pos.x, sbtlGetPChara()->pos.z));
        sbtlBattleStart(stage);
        count = dds3GetKernelCount();
        datDebugPrintf("LoadTime:%d %d\n", count, dds3GetKernelCount() - sbtlSbattleData);
        return 0;
    }
    if (!sbtlLoadEffectData_Decode(NULL)) {
        return 0;
    }
    if (!sbtlLoadAttackHitData_Decode(NULL)) {
        return 0;
    }
    wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    if (wk == NULL) {
        return 0;
    }
    if (sbtlCheckDamageTarget(wk)) {
        if (wk_sbtl->smnCtrl.bNoOk) {
            return 0;
        }
        if (wk_sbtl->step <= 0) {
            return 0;
        }
        if (wk_sbtl->flg2 & 0x1000) {
            return 0;
        }
        slCallBitCheck();
        if (wk_sbtl->flg2 & 0x2000000) {
            return 0;
        }
        if (!(wk_sbtl->status4 & 4)) {
            return 0;
        }
        stage = &sbtlData100[wk_sbtl->stageNo];
        if (!sbtlChkBossEventStart()) {
            if ((u32)(wk_sbtl->stageNo - 752) < 16 && wk_sbtl->fieldE8C) {
                wk_sbtl->huumaTargetID = 1;
                wk_sbtl->field42C0 = 4;
                wk_sbtl->flg |= 0x80000000;
                cmpSbtlkaiwaMain();
            } else if (stage->flags & 0x36) {
                wk_sbtl->bNoEnemyName = 1;
                wk_sbtl->flg |= 0x80000000;
                IsStart(1);
            } else if (wk_sbtl->fieldE8C) {
                wk_sbtl->field42C0 = wk_sbtl->fieldE8D ? 7 : 4;
                wk_sbtl->huumaTargetID = 1;
                wk_sbtl->flg |= 0x80000000;
                cmpSbtlkaiwaMain();
            } else if ((wk_sbtl->flg2 & 0x400000)
                       || (!GetPartyLevelDfference() && !sbtlSENSEIDMGCHK() && !sbtlCallBitCheck())) {
                wk_sbtl->bNoEnemyName = 1;
                wk_sbtl->flg |= 0x80000000;
                IsStart(1);
            }
        }
        pchara = sbtlGetPChara();
        mapDebugLightSet(mapGetLight(pchara->pos.x, sbtlGetPChara()->pos.z));
        sbtlBattleStart(stage);
        count = dds3GetKernelCount();
        datDebugPrintf("LoadTime:%d %d\n", count, dds3GetKernelCount() - sbtlSbattleData);
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_00261DF0", sbtlCheckStartBattle);
#endif

INCLUDE_ASM(const s32, "code_00261DF0", sbtlFunc);

s32 sbtlSbattle4(dds3ProcessID_t *id) {
    if (dds3SearchProcessName(sbtlData85) == NULL) {
        return 0;
    }
    if (wk_sbtl == NULL) {
        return 0;
    }
    if (wk_sbtl->flg & 8) {
        if (!(wk_sbtl->flg2 & 4)) {
            sbtlCallBtlGetCutFlg();
            sbtlDrawAttachEPL(wk_sbtl->hitList);
        }
        return 0;
    }
    sbtlCallGetActiveScene();
    Draw_sbtlUnit();
    sbtlMainDraw();
    sbtlDrawEscapeGauge(32, 384);
    Draw_sbtlDvltk(wk_sbtl->dvltkList);
    Draw_sbtlSTP(wk_sbtl->stp);
    EventProc();
    sbtlRefSbtl28();
    return 0;
}

s32 DrawProc2_sbtl(dds3ProcessID_t *id)
{
    if (dds3SearchProcessName(sbtlData85) == NULL) {
        return 0;
    }
    if (wk_sbtl == NULL) {
        return 0;
    }
    if (!(wk_sbtl->flg & 8)) {
        sbtlHitCProc_Idou(wk_sbtl->hitList);
        sbtlListProc(wk_sbtl->effList);
        sbtlReleaseEff(wk_sbtl->effList, 0);
        sbtlAnimListProc_sbtlDMG();
        sbtlHitCProc(wk_sbtl->hitList);
        sbtlReleaseHitC_List(wk_sbtl->hitList, 0);
        sbtlDelProc(wk_sbtl->effdelList);
        sbtlReleaseDelList(wk_sbtl->effdelList, 0);
        sbtlCallBtlGetCutFlg();
        sbtlDrawAttachEPL(wk_sbtl->hitList);
        sbtlDrawAnimList();
        Draw_sbtlSM(wk_sbtl->sysmsgList);
        DrawTime();
    }
    return 0;
}

s32 Proc_sbtl(void)
{
    if (wk_sbtl == NULL) {
        return 0;
    }
    return dds3SearchProcessName(sbtlData85) != NULL;
}

INCLUDE_ASM(const s32, "code_00261DF0", sbtlProcSequenceSub);

/* NON_MATCHING: 132 of 364 words match.  */
#ifdef NON_MATCHING
s32 CheckEnd(void)
{
    struct SBTLUNIT_WORK *wk;
    struct SBTLUNIT_WORK *nakama;
    BST bst;

    if (!sbtlCheckLoadFlg()) {
        return 1;
    }
    wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    if (wk->status == 34 && !(wk_sbtl->flg & 0x1000000) && wk_sbtl->mode != 8 && wk_sbtl->mode != 9 &&
        wk_sbtl->mode != 16) {
        wk_sbtl->flg |= 0x40000000;
        End_sbtlTime();
        if (!(wk->unit.badStatus & 0x8000) && sbtlCheckUnit_Exec()) {
            if (!sbtlCheckTutorialBattle()) {
                wk_sbtl->flg |= 0x20;
            }
            wk->unit.badStatus |= 0x8000;
            wk_sbtl->flg |= 8;
            wk_sbtl->flg2 &= ~0x10000000;
            sbtlInitDropWork();
            sbtlSetEndData();
            sbtlCheckBattleLose();
            return 1;
        }
    }
    if (!(wk_sbtl->field42CA || wk_sbtl->bNoEndCam || wk_sbtl->fieldCA2 || wk->status == 34 ||
          (wk_sbtl->flg & 0x1000000) || wk->status == 40 || wk->status == 10 || wk_sbtl->mode == 8 ||
          wk_sbtl->mode == 9 || wk_sbtl->mode == 16 || wk_sbtl->field424 ||
          (sbtlCheckGeneratorAll() && sbtlCheckGenerator_ND())) ||
        (wk_sbtl->flg2 & 0x10000)) {
        wk_sbtl->fieldC7C = 1;
        End_sbtlTime();
        wk_sbtl->flg |= 0x40000000;
        wk_sbtl->mode = 19;
        wk_sbtl->field430++;
        if (wk_sbtl->field430 <= 0) {
            wk_sbtl->field430 = 0;
        } else if (wk_sbtl->field430 > 0x7FFF) {
            wk_sbtl->field430 = 0x7FFF;
        }
        wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
        if (wk_sbtl->field430 == 1) {
            sbtlShugouKaijyo();
            wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
            bst = sbtlGetBadStatus(&wk->unit);
            if (wk->status != 42) {
                wk->vstatus |= 0x8000000;
                if (bst != BST_STONE) {
                    sbtlResetBadStatus(wk, 0x25FBF);
                }
                wk->next_motion_not_hokan = 1;
                sbtlSetMotion_sbtlUnit(wk, 12, 0, 0, 0);
                sbtlForceDelete_ID_Acter(wk_sbtl->effList, wk, EFFID_RENKEIOR);
                sbtlRefSbtl11(wk);
                sbtlReleaseSTP_List(wk_sbtl->stp, 1);
                if (!wk_sbtl->bNoEndCam) {
                    nakama = sbtlGetPChara();
                    if (nakama != NULL) {
                        nakama->L2_muteki = 1;
                        nakama->pos.x = 0.0f;
                        nakama->pos.y = 0.0f;
                        nakama->pos.z = 0.0f;
                        nakama->rot.x = 0.0f;
                        nakama->rot.y = 0.0f;
                        nakama->rot.z = 0.0f;
                        nakama->rot.w = 1.0f;
                    }
                    nakama = sbtlGetNakama(0);
                    if (nakama != NULL) {
                        sbtlBgnWait(nakama);
                        nakama->pos.x = 197.64499f;
                        nakama->pos.y = 0.0f;
                        nakama->pos.z = 151.62999f;
                        nakama->rot.x = 0.0f;
                        nakama->rot.y = 0.0f;
                        nakama->rot.z = 0.0f;
                        nakama->rot.w = 1.0f;
                        nakama->next_motion_not_hokan = 1;
                        sbtlSetMotion_sbtlUnit(nakama, 0, 0, 0, 0);
                        sbtlGetNakama(0)->flg &= ~6;
                        nakama->L2_muteki = 1;
                        sbtlSetAction(nakama, 1, 0);
                        nakama->skillid = 0;
                        nakama->aiwk->mode = 12;
                        nakama->model->stat = 1;
                    }
                    nakama = sbtlGetNakama(1);
                    if (nakama != NULL) {
                        sbtlBgnWait(nakama);
                        nakama->pos.x = -197.64499f;
                        nakama->pos.y = 0.0f;
                        nakama->pos.z = 151.62999f;
                        nakama->rot.x = 0.0f;
                        nakama->rot.y = 0.0f;
                        nakama->rot.z = 0.0f;
                        nakama->rot.w = 1.0f;
                        nakama->next_motion_not_hokan = 1;
                        sbtlSetMotion_sbtlUnit(nakama, 0, 0, 0, 0);
                        sbtlGetNakama(1)->flg &= ~6;
                        nakama->L2_muteki = 1;
                        sbtlSetAction(nakama, 1, 0);
                        nakama->skillid = 0;
                        nakama->aiwk->mode = 12;
                        nakama->model->stat = 1;
                    }
                    sbtlRefSbtl65(&CheckEndData[dds3GetRandIntA(NULL, 3)]);
                    sbtlSetMode(1);
                    sbtlCallBgSetScrollReset();
                }
            }
            sbtlResetATKEffect();
            sbtlSetAction(wk, 42, 0);
            sbtlSetEndBadStatus();
        }
        if (wk_sbtl->field430 < 180 && (!sbtlCheckNextAction(wk) || wk->field4FA <= 0)) {
            return 1;
        }
        if ((wk_sbtl->flg2 & 1) && wk_sbtl->field430 < 180) {
            return 0;
        }
        if (!sbtlCheckUnit_Exec()) {
            return 1;
        }
        wk_sbtl->flg |= 8;
        wk_sbtl->field430 = 0;
        wk_sbtl->flg2 |= 0x10000000;
        sbtlCheckBattleWin();
        sbtlGetAutoSkillPoint();
        sbtlGetChuuseiPoint();
        sbtlSetEndData();
        return 1;
    }
    wk_sbtl->field430 = 0;
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_00261DF0", CheckEnd);
#endif

struct SBTLUNIT_WORK *sbtlGetNakama(s8 idx) {
    s32 index;
    struct SBTLUNIT_WORK *wk;

    if ((u8)idx >= 2) {
        return NULL;
    }
    index = wk_sbtl->summonIndex[idx];
    if ((u32)index >= 20) {
        return NULL;
    }
    if (index != wk_sbtl->summonindex_menu[idx]) {
        return NULL;
    }
    if (wk_sbtl->summonflg[index] == 3 || wk_sbtl->summonflg[index] == 2 || wk_sbtl->battleMode == 3) {
        wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[index]);
        if (wk != NULL && wk->unit.devilId != 0) {
            return wk;
        }
    }
    return NULL;
}

INCLUDE_ASM(const s32, "code_00261DF0", sbtlCallBtlSearchUnitID2);

struct SBTLUNIT_WORK *sbtlGetPChara(void) {
    return sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
}

s32 GetMeireiNo(struct SBTLUNIT_WORK *wk) {
    s32 i;

    for (i = 0; i < 2; i++) {
        if (wk->field1A0 == wk_sbtl->stockUnitID[wk_sbtl->summonIndex[i]]) {
            return i;
        }
    }
    return -1;
}

void sbtlInitRedGg(RED_GUAGE *pRedGg, s32 hp) {
    pRedGg->bgn = hp;
    pRedGg->cnt = 0;
}

/* NON_MATCHING: 45 of 75 words match.  */
#ifdef NON_MATCHING
void sbtlAnalyzeLoyalty(s32 x, s32 y, RED_GUAGE *pRedGg, s32 hp, s32 maxhp)
{
    s32 sprNo = 19;
    s32 bgn = pRedGg->bgn;
    s32 cnt;

    if (bgn < hp) {
        pRedGg->bgn = hp;
        if (pRedGg->len < hp) {
            pRedGg->cnt = 0;
        }
    } else if (hp < bgn) {
        pRedGg->bgn = hp;
        pRedGg->len = bgn;
        pRedGg->cnt = 400;
    }
    if (pRedGg->cnt >= 256) {
        itfCalcSpriteWidth(sbtlData135.hSpr, sprNo, pRedGg->len, maxhp);
        itfDrawMiscSprite(x, y, 0, sbtlData135.hSpr, sprNo, DDS3_OT_2D_HIGH | DDS3_OT_BACKEX_NORMAL);
        cnt = pRedGg->cnt - 1;
    } else {
        if (pRedGg->cnt < 2) {
            return;
        }
        itfCalcSpriteWidth(sbtlData135.hSpr, sprNo, pRedGg->len, maxhp);
        itfDrawMiscSpriteAlpha(x, y, 0, pRedGg->cnt, sbtlData135.hSpr, sprNo, DDS3_OT_2D_HIGH | DDS3_OT_BACKEX_NORMAL);
        cnt = pRedGg->cnt - 10;
        if (cnt <= 0) {
            cnt = 0;
        }
    }
    pRedGg->cnt = cnt;
}
#else
INCLUDE_ASM(const s32, "code_00261DF0", sbtlAnalyzeLoyalty);
#endif

/* NON_MATCHING: 34 of 294 words match.  */
#ifdef NON_MATCHING
void sbtlSetCameraLockOnTgtUpdate(void)
{
    struct SBTLUNIT_WORK *wk;
    struct SBTLUNIT_WORK *player;
    SBTL_DEVIL_DATA *devil;
    s32 found;
    s32 count;
    s32 i;
    s32 j;
    s32 per;

    if (wk_sbtl == NULL || wk_sbtl->unitList == NULL) {
        return;
    }
    if (wk_sbtl->flg & 0x40) {
        return;
    }
    if (IsSceanStop()) {
        return;
    }
    if (wk_sbtl->field430) {
        return;
    }
    if (wk_sbtl->mode == 12 || wk_sbtl->mode == 21) {
        return;
    }
    if (wk_sbtl->flg & 0x1400000) {
        return;
    }
    if (wk_sbtl->fieldCA2) {
        return;
    }
    if (!(wk_sbtl->flg & 0x80010000)) {
        return;
    }
    if (wk_sbtl->flg & 0x400) {
        return;
    }
    if (wk_sbtl->battleMode == 3) {
        return;
    }
    if (evtBitCheck(2228)) {
        return;
    }
    if (wk_sbtl->flg < 0) {
        return;
    }
    if (sbtlCheckPlayerDead()) {
        return;
    }
    if (sbtlData100[wk_sbtl->stageNo].flags & 0x36) {
        return;
    }
    count = 0;
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        wk = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (wk != NULL && (wk->field414 & 0x3F) == 2 && !(wk->flg & 0x1000) && wk->status != 35 && sbtlCheckDamageTarget(wk)) {
            count++;
        }
    }
    if (count >= 2) {
        return;
    }
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        wk = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (wk == NULL || (wk->field414 & 0x3F) != 2 || (wk->flg & 0x1000) || wk->status == 35) {
            continue;
        }
        if (!sbtlCheckDamageTarget(wk)) {
            continue;
        }
        if (wk->unit.now.hp * 100 / wk->unit.now.maxhp >= 60) {
            continue;
        }
        if (wk->field6E8) {
            continue;
        }
        if (!sbtlCheckMotionNo(wk, 0) && !sbtlCheckMotionNo(wk, 2)) {
            continue;
        }
        if (wk->unit.badStatus) {
            continue;
        }
        wk->field6E8 = 1;
        devil = sbtlIfDrawActionSub(wk->unit.devilId);
        if (gohData7[wk->unit.devilId].field80 == 12 && sbtlRefDDS3GBWK4(devil->field82) < 4) {
            continue;
        }
        found = 0;
        for (j = 0; j < datGetStockDevilNums(); j++) {
            if (wk->unit.devilId - 207 == DDS3_GBWK->playerData.stock[j].devilId) {
                found = 1;
                break;
            }
        }
        if (found) {
            continue;
        }
        per = 10;
        player = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
        if (sbtlGetDevilHuumaItem(devil->field82) >= 3) {
            if (wk_sbtl->status3 & 0x40) {
                per = 20;
            }
            if (sbtlGetDevilHuumaItem(devil->field82) >= 8) {
                per += 5;
            }
            if (wk->unit.level < 10) {
                per += 10;
            }
            if (player->unit.level - wk->unit.level >= 5) {
                per += 5;
            }
        }
        if ((s32)GetRand(100) >= per) {
            continue;
        }
        if (slCallBitCheck2(wk->unit.devilId, 0)) {
            continue;
        }
        if (gohData7[wk->unit.devilId].field80 == 13) {
            continue;
        }
        if (sbtlCheckMoon(8)) {
            continue;
        }
        StopBattle();
        wk_sbtl->field42C0 = 3;
        wk_sbtl->huumaTargetID = wk->field1A0;
        cmpSbtlkaiwaMain();
        return;
    }
}
#else
INCLUDE_ASM(const s32, "code_00261DF0", sbtlSetCameraLockOnTgtUpdate);
#endif

s32 GetPartyLevelDfference(void)
{
    SBTLUNIT_LISTWORK *list;
    struct SBTLUNIT_WORK *pUnit;
    s32 per;
    s32 rnd;
    s32 level;
    s32 i;

    if (evtBitCheck(2228)) {
        return 0;
    }
    if (wk_sbtl->battleMode == 3) {
        return 0;
    }
    if (wk_sbtl->tgtList != NULL) {
        sbtlDeleteUnitList(wk_sbtl->tgtList);
        wk_sbtl->tgtList = NULL;
    }
    list = sbtlCreateTargetList_Talk(SBTLUNIT_SIDE_ENEMY);
    wk_sbtl->tgtList = list;
    if (list->count <= 0) {
        return 0;
    }
    per = 110;
    level = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID)->unit.level;
    if (level < 8) {
        per = 110;
    } else if (level < 11) {
        per = 110;
    } else {
        per = 100;
        if (evtBitCheck(2116)) {
            per = 130;
        }
    }
    rnd = GetRand(1000);
    datDebugPrintf("SENSEI KAIWA CHK per:%d / %d\n", rnd, per);
    if (rnd < per || wk_sbtl->field431C == 2) {
        for (i = 0; i < wk_sbtl->tgtList->count; i++) {
            pUnit = wk_sbtl->tgtList->pUnitL[GetRand((u16)wk_sbtl->tgtList->count)].pUnit;
            if (slCallBitCheck2(pUnit->unit.devilId, 1)) {
                continue;
            }
            if (gohData7[pUnit->unit.devilId].field80 == 13) {
                continue;
            }
            if (gohData7[pUnit->unit.devilId].field80 == 12) {
                continue;
            }
            if (!sbtlCheckMoon(8)) {
                wk_sbtl->field42C0 = 2;
                wk_sbtl->flg |= 0x80000000;
                wk_sbtl->huumaTargetID = pUnit->field1A0;
                cmpSbtlkaiwaMain();
                return 1;
            }
        }
    }
    return 0;
}

s32 sbtlSENSEIDMGCHK(void)
{
    SBTLUNIT_LISTWORK *list;
    struct SBTLUNIT_WORK *pUnit;
    s32 per;
    s32 rnd;
    s32 luck;
    s32 i;
    s32 level;
    s32 index;

    if (wk_sbtl->tgtList != NULL) {
        sbtlDeleteUnitList(wk_sbtl->tgtList);
        wk_sbtl->tgtList = NULL;
    }
    list = sbtlCreateTargetList_Talk(SBTLUNIT_SIDE_ENEMY);
    wk_sbtl->tgtList = list;
    if (list->count <= 0) {
        return 0;
    }
    per = 15;
    if (evtBitCheck(2116)) {
        luck = sbtlGetPlayerLuckStep();
        switch (luck) {
        case PLAYER_LUCK_BEST__:
        case PLAYER_LUCK_BETTER:
        case PLAYER_LUCK_GOOD__:
            per = 18;
            break;
        case PLAYER_LUCK_BAD___:
        case PLAYER_LUCK_WORSE_:
        case PLAYER_LUCK_WORST_:
            per = 38;
            break;
        default:
            per = 25;
            break;
        }
    }
    rnd = GetRand(100);
    datDebugPrintf("SENSEI DMG CHK per:%d / %d\n", rnd, per);
    if (rnd < per || wk_sbtl->field431C == 1) {
        level = 0;
        index = 0;
        for (i = 0; i < wk_sbtl->tgtList->count; i++) {
            pUnit = wk_sbtl->tgtList->pUnitL[i].pUnit;
            if ((pUnit->unit.devilId == 359 || pUnit->unit.devilId == 360) && !evtBitCheck(8104)) {
                continue;
            }
            if (level < pUnit->unit.level) {
                level = pUnit->unit.level;
                index = i;
            }
        }
        pUnit = wk_sbtl->tgtList->pUnitL[index].pUnit;
        if ((pUnit->unit.devilId == 359 || pUnit->unit.devilId == 360) && !evtBitCheck(8104)) {
            return 0;
        }
        wk_sbtl->field42C0 = 5;
        wk_sbtl->flg |= 0x80000000;
        wk_sbtl->huumaTargetID = pUnit->field1A0;
        cmpSbtlkaiwaMain();
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_00261DF0", sbtlCallBitCheck);

s32 sbtlChkBossEventStart(void)
{
    struct SBTLUNIT_WORK *wk;
    s32 i;
    s32 no;

    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        wk = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (wk != NULL && (wk->field414 & 0x3F) == 2) {
            no = sbtlCheck(wk, 0, 0);
            if (no >= 0) {
                wk_sbtl->boss_eventID = wk->field1A0;
                wk_sbtl->flg |= 0x80000000;
                sbtlProcessStart(SBTLBOSSEVENT_START, no);
                wk->aiwk->bWait2 = 1;
                return 1;
            }
        }
    }
    return 0;
}

void sbtlBattleTmx(void)
{
    sbtlBattleTmxData = sdfDevLoadFileEx("/battle/k_01.tmx", &sbtlData142, NULL);
    sbtlBattleTmxData2 = sdfDevLoadFileEx("/battle/k_02.tmx", &sbtlData143, NULL);
    sbtlBattleTmxData3 = sdfDevLoadFileEx("/battle/k_03.tmx", &sbtlData144, NULL);
    sbtlBattleTmxData4 = sdfDevLoadFileEx("/battle/k_04.tmx", &sbtlData145, NULL);
    sbtlBattleTmxData5 = sdfDevLoadFileEx("/battle/h_01.tmx", &sbtlData146, NULL);
    sbtlBattleTmxData6 = sdfDevLoadFileEx("/battle/h_02.tmx", &sbtlData147, NULL);
    sbtlBattleTmxData7 = sdfDevLoadFileEx("/battle/h_03.tmx", &sbtlData148, NULL);
    sbtlBattleTmxData8 = sdfDevLoadFileEx("/battle/h_04.tmx", &sbtlData149, NULL);
    sbtlData150 = sdfDevLoadFileEx("/battle/b_roul.spr", &sbtlBattleTmxData9, NULL);
}

INCLUDE_ASM(const s32, "code_00261DF0", sbtlCallTexSetFromHead);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlCallTexFreeDelayed);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlCallUnlock2);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlRefSbtl2);

/* NON_MATCHING: 264 of 278 words match.  */
#ifdef NON_MATCHING
void sbtlROULETTECHKPer(void)
{
    s32 force = 0;
    s32 row = 0;
    s32 per;
    s32 next;
    s32 item;
    s32 i;

    wk_sbtl->field433C = 0;
    wk_sbtl->field4340 = 0;
    if (wk_sbtl->stageNo == 466) {
        force = 1;
    } else if (sbtlRefSbtl2()) {
        return;
    }
    if (!wk_sbtl->field4328 && evtBitCheck(3000) && !evtBitCheck(2960) && evtBitCheck(2116) && !evtBitCheck(3001)) {
        force = 1;
    }
    per = dds3GetRandIntA(NULL, 100);
    datDebugPrintf("ROULETTE CHK per:%d / %d\n", per, sbtlROULETTECHKPerData + 30);
    if (!force && sbtlROULETTECHKPerData + 30 < per) {
        next = sbtlROULETTECHKPerData + 10;
        if (next > 40) {
            next = 40;
        }
        sbtlROULETTECHKPerData = next;
        return;
    }
    if (!wk_sbtl->field4328 && !force && !evtBitCheck(2116)) {
        return;
    }
    sbtlCallGetRandData6 = GetRand(4);
    slCallPlaySEData = 0;
    slCallPlaySEData2 = 0;
    slCallPlaySEData3 = 0;
    sbtlROULETTECHKPerData = 0;
    if (wk_sbtl->stageNo == 466) {
        sbtlCallGetRandData5 = 99;
        sbtlCallGetRandWork[0] = 141;
        sbtlCallGetRandWork[1] = 141;
        sbtlCallGetRandWork[2] = 141;
        sbtlCallGetRandWork[3] = 141;
    } else {
        switch (sbtlGetPlayerLuckStep()) {
        case 0:
            if (dds3GetRandIntA(NULL, 100) < 80) {
                sbtlCallGetRandData5 = 98;
                row = 0;
            } else {
                sbtlCallGetRandData5 = 99;
                row = 7;
            }
            break;
        case 1:
            if (dds3GetRandIntA(NULL, 100) < 80) {
                sbtlCallGetRandData5 = 98;
                row = 1;
            } else {
                sbtlCallGetRandData5 = 99;
                row = 8;
            }
            break;
        case 2:
        case 3:
        case 4:
            if (dds3GetRandIntA(NULL, 100) < 60) {
                sbtlCallGetRandData5 = 98;
                row = 1;
            } else {
                sbtlCallGetRandData5 = 99;
                row = 8;
            }
            break;
        case 5:
            if (dds3GetRandIntA(NULL, 100) < 20) {
                sbtlCallGetRandData5 = 98;
                row = 1;
            } else {
                sbtlCallGetRandData5 = 99;
                row = 8;
            }
            break;
        case 6:
            if (dds3GetRandIntA(NULL, 100) < 20) {
                sbtlCallGetRandData5 = 98;
                row = 2;
            } else {
                sbtlCallGetRandData5 = 99;
                row = 9;
            }
            break;
        }
        if (sbtlCallGetRandData5 == 98) {
            pbCallEffectReset(1);
        }
        item = sbtlCallGetRandData[row][dds3GetRandIntA(NULL, 8)];
        for (i = 0; i < 4; i++) {
            sbtlCallGetRandWork[i] = sbtlCallUnlock2(item, i, 0);
        }
    }
    switch (sbtlCallGetRandWork[sbtlCallGetRandData6]) {
    case 125:
        wk_sbtl->field4338 = 1;
        break;
    case 126:
        wk_sbtl->field4338 = 2;
        break;
    case 127:
        wk_sbtl->field4338 = 3;
        break;
    case 128:
        wk_sbtl->field4338 = 4;
        break;
    case 129:
        wk_sbtl->field4338 = 5;
        break;
    case 130:
        wk_sbtl->field4338 = 6;
        break;
    case 131:
        wk_sbtl->field4338 = 7;
        break;
    case 132:
        wk_sbtl->field4338 = 8;
        break;
    case 133:
        wk_sbtl->field4338 = 9;
        break;
    case 134:
        wk_sbtl->field4338 = 10;
        break;
    case 135:
        wk_sbtl->field4338 = 11;
        break;
    }
    if (sbtlCallGetRandData5 == 98) {
        sbtlCallTexSetFromHead(0);
    } else {
        sbtlCallTexSetFromHead(1);
    }
    wk_sbtl->flg2 |= 0x401000;
}
#else
INCLUDE_ASM(const s32, "code_00261DF0", sbtlROULETTECHKPer);
#endif

INCLUDE_ASM(const s32, "code_00261DF0", DeleteAll_MenuItemUse);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlCallGetRand);

INCLUDE_ASM(const s32, "code_00261DF0", slLeaf);

INCLUDE_ASM(const s32, "code_00261DF0", slCallSetOt);

INCLUDE_ASM(const s32, "code_00261DF0", slCallSetOt2);

INCLUDE_ASM(const s32, "code_00261DF0", slCallSetOt3);

INCLUDE_ASM(const s32, "code_00261DF0", slCallSetOt4);

/* NON_MATCHING: 828 of 914 words match.  */
#ifdef NON_MATCHING
void slCallPlaySE(void) {
    s32 x;
    s32 y;
    s32 ofs;
    u32 color;
    u32 c;

    if (slCallPlaySEData >= 1 && slCallPlaySEData < 5) {
        slCallSetOt3();
        if (sbtlCallGetRandData5 == 98) {
            slCallSetOt();
        } else {
            slCallSetOt2();
        }
    }
    switch (slCallPlaySEData) {
    case 0:
        slCallPlaySEData2++;
        if (wk_sbtl->flg2 & 0x800) {
            if (sbtlCallGetRandData5 == 98) {
                p2sdPlaySE(16, 127, 63);
            } else {
                p2sdPlaySE(23, 127, 63);
            }
            slCallPlaySEData4 = 1;
            slCallPlaySEData7 = 5;
            slCallPlaySEData9 = -1;
            slCallPlaySEData10 = 0xA09DC380;
            slCallPlaySEData11 = 0x80808080;
            slCallPlaySEData12 = slCallPlaySEData6 = 2;
            slCallPlaySEData = 1;
            slCallPlaySEData3 = 0.0f;
            slCallPlaySEData5 = 0;
            slCallPlaySEData8 = 0;
            slCallPlaySEData2 = 0;
            slLeaf(0);
        }
        break;
    case 1:
        slCallPlaySEData2++;
        slCallPlaySEData3 += 0.2f;
        sdfSin(slCallPlaySEData3);
        if (slCallPlaySEData2 >= 16) {
            slCallPlaySEData2 = 0;
            slCallPlaySEData = 2;
        }
        break;
    case 2:
        slCallPlaySEData2++;
        slCallPlaySEData3 += 0.2f;
        sdfSin(slCallPlaySEData3);
        color = slCallPlaySEData11;
        if (slCallPlaySEData2 == 0) {
            p2sdPlaySE(13, 127, 63);
        }
        if (slCallPlaySEData2 == 4) {
            p2sdPlaySE(13, 127, 63);
        }
        if (slCallPlaySEData2 == 8) {
            p2sdPlaySE(13, 127, 63);
        }
        if (slCallPlaySEData2 == 12) {
            p2sdPlaySE(13, 127, 63);
        }
        ofs = (15 - slCallPlaySEData2) * 32;
        if (ofs < 0) {
            ofs = 0;
        }
        sbtlLeaf120(ofs + 96, 195, 320, sbtlCallGetRandWork[0], color, 255);
        ofs = -((19 - slCallPlaySEData2) * 32);
        if (ofs > 0) {
            ofs = 0;
        }
        sbtlLeaf120(ofs + 96, 221, 320, sbtlCallGetRandWork[1], color, 255);
        ofs = (23 - slCallPlaySEData2) * 32;
        if (ofs < 0) {
            ofs = 0;
        }
        sbtlLeaf120(ofs + 96, 247, 320, sbtlCallGetRandWork[2], color, 255);
        ofs = -((27 - slCallPlaySEData2) * 32);
        if (ofs > 0) {
            ofs = 0;
        }
        sbtlLeaf120(96 - ofs, 273, 320, sbtlCallGetRandWork[3], color, 255);
        if (slCallPlaySEData2 >= 33) {
            slCallPlaySEData = 3;
            slCallPlaySEData2 = 0;
            slLeaf(1);
        }
        break;
    case 3:
        slCallPlaySEData4++;
        if (slCallPlaySEData8 == 0) {
            slCallPlaySEData10 = 0xA09DC380;
            slCallPlaySEData11 = 0x80808080;
        } else {
            slCallPlaySEData10 = 0xA09DC380;
            if ((s32)(slCallPlaySEData11 - 0x04040400u) < 0x40404000) {
                slCallPlaySEData11 = 0x40404080;
            } else {
                slCallPlaySEData11 -= 0x04040400u;
            }
        }
        slCallPlaySEData12--;
        if (slCallPlaySEData8 == 0) {
            if (slCallPlaySEData12 <= 0) {
                if (slCallPlaySEData9 != -1 && slCallPlaySEData5 == slCallPlaySEData9) {
                    slCallPlaySEData6 = 2;
                    slCallPlaySEData7 = 5;
                    slCallPlaySEData9 = -1;
                    slCallPlaySEData8 = 70;
                    if (sbtlCallGetRandData5 == 98) {
                        p2sdPlaySE(26, 127, 63);
                    } else {
                        p2sdPlaySE(7, 127, 63);
                    }
                }
                if (slCallPlaySEData8 == 0) {
                    slCallPlaySEData5 = (slCallPlaySEData5 + 1) % 4;
                    if (sbtlCallGetRandData5 == 98) {
                        p2sdPlaySE(5, 127, 63);
                    } else {
                        p2sdPlaySE(2, 127, 63);
                    }
                    if (slCallPlaySEData5 == 0) {
                        if (slCallPlaySEData7 == 0) {
                            if (slCallPlaySEData6 < 8) {
                                slCallPlaySEData6++;
                            } else {
                                slCallPlaySEData9 = sbtlCallGetRandData6;
                            }
                        } else {
                            slCallPlaySEData7--;
                        }
                    }
                }
                slCallPlaySEData12 = slCallPlaySEData6;
            }
        } else {
            if (sbtlCallGetRandData5 == 98) {
                dds3DrawAddMagSub(105);
                if (slCallPlaySEData8 < 30) {
                    if (slCallPlaySEData8 & 1) {
                        x = (30 - slCallPlaySEData8) * 11 + 96;
                        y = slCallPlaySEData5 * 26 + 195;
                        nbResultCountEntries(x, y - GetRand(26), 0);
                    } else {
                        x = (30 - slCallPlaySEData8) * 11 + 96;
                        y = slCallPlaySEData5 * 26 + 195;
                        nbResultCountEntries(x, y + GetRand(26) + 26, 0);
                    }
                    x = GetRand(320) + 96;
                    y = slCallPlaySEData5 * 26 + 195;
                    nbResultCountEntries(x, y + GetRand(40) - 14, 0);
                }
            }
            if (--slCallPlaySEData8 == 0) {
                slCallPlaySEData = 4;
                slCallPlaySEData2 = 0;
                slLeaf(2);
            }
        }
        slCallPlaySEData3 += 0.2f;
        sdfSin(slCallPlaySEData3);
        slCallSetOt4(slCallPlaySEData5 * 26 + 195);
        c = slCallPlaySEData5 != 0 ? slCallPlaySEData11 : slCallPlaySEData10;
        sbtlLeaf120(96, 195, 320, sbtlCallGetRandWork[0], c, 255);
        c = slCallPlaySEData5 != 1 ? slCallPlaySEData11 : slCallPlaySEData10;
        sbtlLeaf120(96, 221, 320, sbtlCallGetRandWork[1], c, 255);
        c = slCallPlaySEData5 != 2 ? slCallPlaySEData11 : slCallPlaySEData10;
        sbtlLeaf120(96, 247, 320, sbtlCallGetRandWork[2], c, 255);
        c = slCallPlaySEData5 != 3 ? slCallPlaySEData11 : slCallPlaySEData10;
        sbtlLeaf120(96, 273, 320, sbtlCallGetRandWork[3], c, 255);
        break;
    case 4:
        if (sbtlCallGetRandData5 == 98 && slCallPlaySEData2 < 10) {
            x = GetRand(320) + 96;
            y = slCallPlaySEData5 * 26 + 195;
            nbResultCountEntries(x, y + GetRand(40) - 14, 0);
        }
        slCallPlaySEData2++;
        slCallPlaySEData3 += 0.2f;
        sdfSin(slCallPlaySEData3);
        slCallSetOt4(slCallPlaySEData5 * 26 + 195);
        c = slCallPlaySEData5 != 0 ? slCallPlaySEData11 : slCallPlaySEData10;
        sbtlLeaf120(96, 195, 320, sbtlCallGetRandWork[0], (c & ~0xFF) + (128 - slCallPlaySEData2 * 4),
                    255 - slCallPlaySEData2 * 8);
        c = slCallPlaySEData5 != 1 ? slCallPlaySEData11 : slCallPlaySEData10;
        sbtlLeaf120(96, 221, 320, sbtlCallGetRandWork[1], (c & ~0xFF) + (128 - slCallPlaySEData2 * 4),
                    255 - slCallPlaySEData2 * 8);
        c = slCallPlaySEData5 != 2 ? slCallPlaySEData11 : slCallPlaySEData10;
        sbtlLeaf120(96, 247, 320, sbtlCallGetRandWork[2], (c & ~0xFF) + (128 - slCallPlaySEData2 * 4),
                    255 - slCallPlaySEData2 * 8);
        c = slCallPlaySEData5 != 3 ? slCallPlaySEData11 : slCallPlaySEData10;
        sbtlLeaf120(96, 273, 320, sbtlCallGetRandWork[3], (c & ~0xFF) + (128 - slCallPlaySEData2 * 4),
                    255 - slCallPlaySEData2 * 8);
        if (slCallPlaySEData2 >= 30) {
            slCallPlaySEData2 = 0;
            slCallPlaySEData = 5;
        }
        break;
    case 5:
        if (++slCallPlaySEData2 >= 2) {
            wk_sbtl->status3 = wk_sbtl->status3 & ~0x1100 & 0x80009FFF;
            wk_sbtl->field433C = sbtlCallGetRandWork[slCallPlaySEData5];
            wk_sbtl->field4340 = 0;
            switch (sbtlCallGetRandWork[slCallPlaySEData5]) {
            case 0x64:
                wk_sbtl->status3 |= 0x1;
                break;
            case 0x65:
                wk_sbtl->status3 |= 0x2;
                break;
            case 0x66:
                wk_sbtl->status3 |= 0x4;
                break;
            case 0x67:
                wk_sbtl->status3 |= 0x8;
                break;
            case 0x68:
                wk_sbtl->field448 = 1;
                wk_sbtl->status3 |= 0x80;
                break;
            case 0x69:
                wk_sbtl->field448 = 2;
                wk_sbtl->status3 |= 0x80;
                break;
            case 0x6A:
                wk_sbtl->field448 = 3;
                wk_sbtl->status3 |= 0x80;
                break;
            case 0x6B:
                wk_sbtl->field448 = 4;
                wk_sbtl->status3 |= 0x80;
                break;
            case 0x6C:
                wk_sbtl->field448 = 5;
                wk_sbtl->status3 |= 0x80;
                break;
            case 0x6D:
                wk_sbtl->field448 = 6;
                wk_sbtl->status3 |= 0x80;
                break;
            case 0x6E:
                wk_sbtl->field448 = 7;
                wk_sbtl->status3 |= 0x80;
                break;
            case 0x6F:
                wk_sbtl->field448 = 8;
                wk_sbtl->status3 |= 0x80;
                break;
            case 0x70:
                wk_sbtl->field448 = 9;
                wk_sbtl->status3 |= 0x80;
                break;
            case 0x71:
                wk_sbtl->field448 = 10;
                wk_sbtl->status3 |= 0x80;
                break;
            case 0x72:
                wk_sbtl->field448 = 11;
                wk_sbtl->status3 |= 0x80;
                break;
            case 0x73:
                wk_sbtl->status3 |= 0x10;
                break;
            case 0x74:
                wk_sbtl->status3 |= 0x20;
                break;
            case 0x75:
                wk_sbtl->status3 |= 0x40;
                break;
            case 0x76:
                wk_sbtl->status3 |= 0x1000;
                sbtlSetDamage_L_EnemyAll();
                break;
            case 0x7A:
            case 0x8E:
                wk_sbtl->field4340 = 1;
                wk_sbtl->status3 |= 0x10000;
                break;
            case 0x7B:
                wk_sbtl->status3 |= 0x20000;
                sbtlCallBtlCreateSideListSBtlUnit4();
                wk_sbtl->field4340 = 1;
                break;
            case 0x7C:
                wk_sbtl->status3 |= 0x40000;
                break;
            case 0x7D:
                wk_sbtl->field4340 = 1;
                wk_sbtl->field448 = 1;
                wk_sbtl->status3 |= 0x80000;
                break;
            case 0x7E:
                wk_sbtl->field4340 = 1;
                wk_sbtl->field448 = 2;
                wk_sbtl->status3 |= 0x80000;
                break;
            case 0x7F:
                wk_sbtl->field4340 = 1;
                wk_sbtl->field448 = 3;
                wk_sbtl->status3 |= 0x80000;
                break;
            case 0x80:
                wk_sbtl->field4340 = 1;
                wk_sbtl->field448 = 4;
                wk_sbtl->status3 |= 0x80000;
                break;
            case 0x81:
                wk_sbtl->field4340 = 1;
                wk_sbtl->field448 = 5;
                wk_sbtl->status3 |= 0x80000;
                break;
            case 0x82:
                wk_sbtl->field4340 = 1;
                wk_sbtl->field448 = 6;
                wk_sbtl->status3 |= 0x80000;
                break;
            case 0x83:
                wk_sbtl->field4340 = 1;
                wk_sbtl->field448 = 7;
                wk_sbtl->status3 |= 0x80000;
                break;
            case 0x84:
                wk_sbtl->field4340 = 1;
                wk_sbtl->field448 = 8;
                wk_sbtl->status3 |= 0x80000;
                break;
            case 0x85:
                wk_sbtl->field4340 = 1;
                wk_sbtl->field448 = 9;
                wk_sbtl->status3 |= 0x80000;
                break;
            case 0x86:
                wk_sbtl->field4340 = 1;
                wk_sbtl->field448 = 10;
                wk_sbtl->status3 |= 0x80000;
                break;
            case 0x87:
                wk_sbtl->field4340 = 1;
                wk_sbtl->field448 = 11;
                wk_sbtl->status3 |= 0x80000;
                break;
            case 0x88:
                wk_sbtl->field4340 = 1;
                wk_sbtl->status3 |= 0x100000;
                break;
            case 0x89:
                wk_sbtl->field4340 = 1;
                wk_sbtl->status3 |= 0x200000;
                break;
            case 0x8F:
                wk_sbtl->field4340 = 1;
                wk_sbtl->status3 |= 0x400000;
                break;
            case 0x90:
                wk_sbtl->field4340 = 1;
                wk_sbtl->status3 |= 0x800000;
                break;
            case 0x91:
                wk_sbtl->field4340 = 1;
                wk_sbtl->status3 |= 0x1000000;
                break;
            case 0x92:
                wk_sbtl->field4340 = 1;
                wk_sbtl->status3 |= 0x2000000;
                break;
            case 0x94:
                wk_sbtl->field4340 = 1;
                wk_sbtl->status3 |= 0x8000000;
                break;
            case 0x95:
                wk_sbtl->field4340 = 1;
                wk_sbtl->status3 |= 0x10000000;
                break;
            case 0x5A:
                wk_sbtl->field4340 = 1;
                wk_sbtl->status3 |= 0x20000000;
                break;
            case 0x5B:
                wk_sbtl->field4340 = 1;
                wk_sbtl->status3 |= 0x40000000;
                break;
            case 0x5C:
                wk_sbtl->field4340 = 1;
                wk_sbtl->status3 |= 0x2000;
                break;
            case 0x5D:
                wk_sbtl->field4340 = 1;
                wk_sbtl->status3 |= 0x4000;
                break;
            case 0x5E:
            case 0x93:
                wk_sbtl->field4340 = 1;
                wk_sbtl->status3 |= 0x4000000;
                break;
            }
            sbtlCallTexFreeDelayed();
            pbCallEffectReset(0);
            wk_sbtl->flg2 &= ~0x1000;
            slCallPlaySEData = -1;
            slCallPlaySEData2 = 0;
        }
        break;
    }
}
#else
INCLUDE_ASM(const s32, "code_00261DF0", slCallPlaySE);
#endif

/* NON_MATCHING: 88 of 101 words match.  */
#ifdef NON_MATCHING
void slCallBitCheck(void)
{
    s32 msg;

    if (wk_sbtl->flg2 & 0x1000000) {
        return;
    }
    wk_sbtl->flg2 |= 0x1000000;
    slQueryYesNoDraw_slMainData = 0;
    switch (wk_sbtl->stageNo) {
    case 471:
    case 482:
    case 483:
    case 484:
    case 487:
    case 528:
    case 560:
        if (evtBitCheck(3003)) {
            return;
        }
        evtBitOn(3003);
        msg = 167;
        break;
    default:
        if (sbtlData100[wk_sbtl->stageNo].flags & 0x26) {
            return;
        }
        if (wk_sbtl->fieldE8C) {
            return;
        }
        if (evtBitCheck(6010) && !evtBitCheck(3000)) {
            evtBitOn(3000);
            msg = 165;
            break;
        }
        if (!(wk_sbtl->flg2 & 0x400000)) {
            return;
        }
        if (evtBitCheck(2960)) {
            return;
        }
        if (!evtBitCheck(2116)) {
            return;
        }
        if (evtBitCheck(3001)) {
            return;
        }
        evtBitOn(3001);
        msg = 166;
        break;
    }
    slData10 = msg;
    wk_sbtl->flg2 |= 0x4000000;
    wk_sbtl->flg2 |= 0x2000000;
}
#else
INCLUDE_ASM(const s32, "code_00261DF0", slCallBitCheck);
#endif

void slQueryYesNoDraw_slMain(void)
{
    if (!(wk_sbtl->flg2 & 0x2000000)) {
        return;
    }
    switch (slQueryYesNoDraw_slMainData) {
    case 0:
        wk_sbtl->field42BC = 0;
        slQueryYesNoDraw_slMainData3 = 1;
        slQueryYesNoDraw_slMainData4 = 50;
        slQueryYesNoDraw_slMainData++;
        break;
    case 1:
        slQueryYesNoDraw_slMainData4--;
        if (slQueryYesNoDraw_slMainData3 && slQueryYesNoDraw_slMainData4 == 4) {
            p2sdPlaySE(27, 127, 63);
        }
        if (slQueryYesNoDraw_slMainData4 < 0) {
            slQueryYesNoDraw_slMainData++;
        }
        break;
    case 2:
        if (slChoiceMsgDraw(slData10)) {
            slQueryYesNoDraw_slMainData4 = 50;
            slQueryYesNoDraw_slMainData3 = 0;
            slQueryYesNoDraw_slMainData++;
        }
        break;
    case 3:
        if (--slQueryYesNoDraw_slMainData4 < 0) {
            slQueryYesNoDraw_slMainData++;
        }
        break;
    case 4:
        wk_sbtl->flg2 &= ~0x2000000;
        break;
    }
    if (slQueryYesNoDraw_slMainData < 4) {
        if (slQueryYesNoDraw_slMainData3) {
            slQueryYesNoDraw_slMainData2 += 2;
            if (slQueryYesNoDraw_slMainData2 > 80) {
                slQueryYesNoDraw_slMainData2 = 80;
            }
            eglSetOt(0x67);
            eglLinkAlpha(0);
            eglLinkTest(1, 0, 128, 3, 0, 0, 1, 1);
            evtSubmitDefaultDepthGradientRect(0, 0, 512, 448, slQueryYesNoDraw_slMainData2 << 24,
                                              slQueryYesNoDraw_slMainData2 << 24, slQueryYesNoDraw_slMainData2 << 24,
                                              slQueryYesNoDraw_slMainData2 << 24);
            return;
        } else if (slQueryYesNoDraw_slMainData2 > 0) {
            slQueryYesNoDraw_slMainData2 -= 2;
            if (slQueryYesNoDraw_slMainData2 < 0) {
                slQueryYesNoDraw_slMainData2 = 0;
            }
            eglSetOt(0x67);
            eglLinkAlpha(0);
            eglLinkTest(1, 0, 128, 3, 0, 0, 1, 1);
            evtSubmitDefaultDepthGradientRect(0, 0, 512, 448, slQueryYesNoDraw_slMainData2 << 24,
                                              slQueryYesNoDraw_slMainData2 << 24, slQueryYesNoDraw_slMainData2 << 24,
                                              slQueryYesNoDraw_slMainData2 << 24);
            return;
        }
    }
}

/* NON_MATCHING: 19 of 70 words match.  */
#ifdef NON_MATCHING
void slCallSin(s32 alpha)
{
    f32 angle;
    f32 t;
    f32 u;
    s32 hi;
    s32 lo;
    s32 r;
    s32 g;

    if (wk_sbtl->field433C) {
        angle = slCallSinData + 0.08f;
        wk_sbtl->status4 |= 0x2000;
        slCallSinData = angle;
        t = sdfSin(angle) * 0.5f + 0.5f;
        u = 1.0f - t;
        r = 200;
        g = 208;
        hi = t * r + u * 224.0f;
        lo = t * g + u * 232.0f;
        sbtlLeaf94(0, 96, 320, wk_sbtl->field433C, (hi << 24) + alpha / 2 + (lo << 8) + (hi << 16),
                   alpha < 177 ? alpha : 176);
    }
}
#else
INCLUDE_ASM(const s32, "code_00261DF0", slCallSin);
#endif

void slFirstInitSub(void)
{
    sbtlCallSync();
}

void slCampEFFTES(void) {
    fldTitleMiniWork6 = sdfDevLoadFileEx(slStrCampEffTesEpl, &effScalyTextureHandle3, 0);
}

void slCallEffectLoadEPL(void) {
    sdfVector4_t position;

    position = slCallEffectLoadEPLTable;
    fldTitleMiniTex6 = dds3EffectLoadEPL((void *)effScalyTextureHandle3);
    fclInitDstEff2Sub((struct plistHandle_t *)fldTitleMiniTex6->instance);
    dds3EffectReset(fldTitleMiniTex6);
    dds3EffectSetPosition(fldTitleMiniTex6, &position);
    dds3EffectSetLoop_dds3Effect(fldTitleMiniTex6, 1);
    dds3EffectSetScale(fldTitleMiniTex6, 1.5f);
}

void pbCallEffectDelete(void) {
    if (fldTitleMiniTex6 != NULL) {
        dds3EffectDelete(fldTitleMiniTex6);
        fldTitleMiniTex6 = NULL;
    }
    if (fldTitleMiniWork6 != NULL) {
        sdfMemFreeDirect(fldTitleMiniWork6);
        fldTitleMiniWork6 = NULL;
        effScalyTextureHandle3 = 0;
    }
}

INCLUDE_ASM(const s32, "code_00261DF0", pbCallMemset);

INCLUDE_ASM(const s32, "code_00261DF0", pbCallEffectReset);

void pbSetGameCamera0(void)
{
    SDF_VU0_COPYVECTOR(&pbWork3, &aViewDown);
    SDF_VU0_COPYVECTOR(&pbWork2, &aViewEye);
    SDF_VU0_COPYVECTOR(&pbWork, &aViewTarget);
}

INCLUDE_ASM(const s32, "code_00261DF0", pbLeaf19);

void SetMapLight(dds3Basic_t *unit, u32 lrgb, u32 argb) {
    evtUnit_t *h;

    if (unit != NULL) {
        h = evtGetUnitHandle2(unit);
        evtUnitLightOn(h);
        evtSetUnitLightAndAmbient(h, 0, lrgb, argb);
    }
}

INCLUDE_ASM(const s32, "code_00261DF0", Init_sbtlScene);

void sbtlSetRndEncBg(s32 rnd_enc_id) {
    datRndEncTbl_t *tbl;

    sbtlSetRndEncBgData = rnd_enc_id;
    tbl = &sbtlData101[rnd_enc_id][sbtlGetRndEncJoken(rnd_enc_id)];
    sbtlSceneLoadData4 = tbl->map;
    sbtlSceneLoadData5 = tbl->floor;
    sbtlSceneLoadData6 = 1;
}

INCLUDE_RODATA(const s32, "code_00261DF0", slStrCampEffTesEpl);

INCLUDE_RODATA(const s32, "code_00261DF0", slCallEffectLoadEPLTable);

INCLUDE_RODATA(const s32, "code_00261DF0", pbCallMemsetTable);

INCLUDE_RODATA(const s32, "code_00261DF0", pbCallMemsetTable2);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlSceneLoad);
