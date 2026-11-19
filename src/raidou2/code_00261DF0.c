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

INCLUDE_ASM(const s32, "code_00261DF0", CheckEnd);

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

INCLUDE_ASM(const s32, "code_00261DF0", sbtlSetCameraLockOnTgtUpdate);

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

INCLUDE_ASM(const s32, "code_00261DF0", sbtlROULETTECHKPer);

INCLUDE_ASM(const s32, "code_00261DF0", DeleteAll_MenuItemUse);

INCLUDE_ASM(const s32, "code_00261DF0", sbtlCallGetRand);

INCLUDE_ASM(const s32, "code_00261DF0", slLeaf);

INCLUDE_ASM(const s32, "code_00261DF0", slCallSetOt);

INCLUDE_ASM(const s32, "code_00261DF0", slCallSetOt2);

INCLUDE_ASM(const s32, "code_00261DF0", slCallSetOt3);

INCLUDE_ASM(const s32, "code_00261DF0", slCallSetOt4);

INCLUDE_ASM(const s32, "code_00261DF0", slCallPlaySE);

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
