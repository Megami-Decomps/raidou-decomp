#include "common.h"
#include "sbtlWork.h"
#include "sdk_libc.h"
#include "ps2asm.h"
#include "dds3KernelCore.h"
#include "newdata.h"
#include "sBtlUnit.h"
#include "sdfMemory.h"

INCLUDE_ASM(const s32, "code_00261DF0", sbtlRaidouKatana);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlSbattle);

INCLUDE_ASM(const s32, "code_00261DF0", Exit_sbtl);

INCLUDE_ASM(const s32, "code_00261DF0", StopBattle);

INCLUDE_ASM(const s32, "code_00261DF0", IsSceanStop);

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

INCLUDE_ASM(const s32, "code_00261DF0", sbtlRefDDS3GBWK2);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlSetData_Unit_Stock);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlGetData_Stock_Unit);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlLeaf53);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlBtlCheckSummonSub);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlCheckStockPara);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlCallBtlGetItemtbl);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlSetMapFade);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlMapFadeProc);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlSetMapFade_H);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlMapFadeProc_H);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlRefSbtl);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlSetHitStop);

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

INCLUDE_ASM(const s32, "code_00261DF0", sbtlCallBtlSearchUnitID);

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

INCLUDE_ASM(const s32, "code_00261DF0", sbtlGetPChara);

INCLUDE_ASM(const s32, "code_00261DF0", GetMeireiNo);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlInitRedGg);

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

INCLUDE_ASM(const s32, "code_00261DF0", slCallEffectLoadEPL);

INCLUDE_ASM(const s32, "code_00261DF0", pbCallEffectDelete);

INCLUDE_ASM(const s32, "code_00261DF0", pbCallMemset);

INCLUDE_ASM(const s32, "code_00261DF0", pbCallEffectReset);

void pbSetGameCamera0(void)
{
    SDF_VU0_COPYVECTOR(&pbWork3, &aViewDown);
    SDF_VU0_COPYVECTOR(&pbWork2, &aViewEye);
    SDF_VU0_COPYVECTOR(&pbWork, &aViewTarget);
}

INCLUDE_ASM(const s32, "code_00261DF0", pbLeaf19);

INCLUDE_ASM(const s32, "code_00261DF0", SetMapLight);

INCLUDE_ASM(const s32, "code_00261DF0", Init_sbtlScene);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlSetRndEncBg);

INCLUDE_RODATA(const s32, "code_00261DF0", slStrCampEffTesEpl);

INCLUDE_RODATA(const s32, "code_00261DF0", slCallEffectLoadEPLTable);

INCLUDE_RODATA(const s32, "code_00261DF0", pbCallMemsetTable);

INCLUDE_RODATA(const s32, "code_00261DF0", pbCallMemsetTable2);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlSceneLoad);
