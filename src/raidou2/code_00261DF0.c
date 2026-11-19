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

INCLUDE_ASM(const s32, "code_00261DF0", sbtlRaidouKatana);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlSbattle);

INCLUDE_ASM(const s32, "code_00261DF0", Exit_sbtl);

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

INCLUDE_ASM(const s32, "code_00261DF0", sbtlCheckSummon);

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

INCLUDE_ASM(const s32, "code_00261DF0", LoadOnMemoryData_sbtl);

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

INCLUDE_ASM(const s32, "code_00261DF0", sbtlSetData_Unit_Stock);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlGetData_Stock_Unit);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlLeaf53);

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

INCLUDE_ASM(const s32, "code_00261DF0", sbtlProcHitStop);

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

INCLUDE_ASM(const s32, "code_00261DF0", sbtlPacData_Free);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlCallMemFreeDirect);

INCLUDE_ASM(const s32, "code_00261DF0", Init_sbtl);

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

INCLUDE_ASM(const s32, "code_00261DF0", sbtlCheckStartBattle);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlFunc);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlSbattle4);

INCLUDE_ASM(const s32, "code_00261DF0", DrawProc2_sbtl);

s32 Proc_sbtl(void)
{
    if (wk_sbtl == NULL) {
        return 0;
    }
    return dds3SearchProcessName(sbtlData85) != NULL;
}

INCLUDE_ASM(const s32, "code_00261DF0", sbtlProcSequenceSub);

INCLUDE_ASM(const s32, "code_00261DF0", CheckEnd);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlGetNakama);

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

INCLUDE_ASM(const s32, "code_00261DF0", sbtlAnalyzeLoyalty);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlSetCameraLockOnTgtUpdate);

INCLUDE_ASM(const s32, "code_00261DF0", GetPartyLevelDfference);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlSENSEIDMGCHK);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlCallBitCheck);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlChkBossEventStart);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlBattleTmx);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlCallTexSetFromHead);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlCallTexFreeDelayed);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlCallUnlock2);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlRefSbtl2);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlROULETTECHKPer);

INCLUDE_ASM(const s32, "code_00261DF0", DeleteAll_MenuItemUse);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlCallGetRand);

INCLUDE_ASM(const s32, "code_00261DF0", slLeaf);

INCLUDE_ASM(const s32, "code_00261DF0", slCallSetOt);

INCLUDE_ASM(const s32, "code_00261DF0", slCallSetOt2);

INCLUDE_ASM(const s32, "code_00261DF0", slCallSetOt3);

INCLUDE_ASM(const s32, "code_00261DF0", slCallSetOt4);

INCLUDE_ASM(const s32, "code_00261DF0", slCallPlaySE);

INCLUDE_ASM(const s32, "code_00261DF0", slCallBitCheck);

INCLUDE_ASM(const s32, "code_00261DF0", slQueryYesNoDraw_slMain);

INCLUDE_ASM(const s32, "code_00261DF0", slCallSin);

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
    tbl = &sbtlData101[rnd_enc_id][sbtlCallBitCheck3(rnd_enc_id)];
    sbtlSceneLoadData4 = tbl->map;
    sbtlSceneLoadData5 = tbl->floor;
    sbtlSceneLoadData6 = 1;
}

INCLUDE_RODATA(const s32, "code_00261DF0", slStrCampEffTesEpl);

INCLUDE_RODATA(const s32, "code_00261DF0", slCallEffectLoadEPLTable);

INCLUDE_RODATA(const s32, "code_00261DF0", pbCallMemsetTable);

INCLUDE_RODATA(const s32, "code_00261DF0", pbCallMemsetTable2);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlSceneLoad);
