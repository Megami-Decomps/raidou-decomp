#include "common.h"
#include "sbtlWork.h"
#include "sBtlSM.h"
#include "sdfChip.h"
#include "sdk_libc.h"
#include "code_002929C8.h"
#include "code_00291C78.h"
#include "sBtlSkill.h"
#include "code_r1_0021CE60.h"
#include "nbDebugProcess.h"
#include "sBtlCmndMenuNew.h"
#include "code_r1_002C8188.h"
#include "code_r1_002C7ED0.h"
#include "sdfMemory.h"
#include "sBtlDraw.h"
#include "code_0027A1E8.h"
#include "dds3KernelDraw.h"
#include "evtMoon.h"

s32 OnDataLoadProc(dds3ProcessID_t *id) {
    s32 i;

    if (!sbtlEvent_CheckRain()) {
        return 0;
    }
    if (!sbtlEvent_LoadEventScript()) {
        return 0;
    }
    for (i = 0; i < 46; i++) {
        if (sbtl_effonfilecheck[i] == 2) {
            btlLoadSbPackData[i].unkC = Create_effPartsList(btlLoadSbPackData[i].data);
        }
    }
    wk_sbtl->flg |= 4;
    return -1;
}

void sbtlLeaf75(dds3ProcessID_t *id) {
}

/* NON_MATCHING: 26 of 32 words match.  */
#ifdef NON_MATCHING
s32 sbtlFunc6(s32 id, void *pData) {
    id -= 4;
    if (id < 46) {
        if (id < 12) {
            if (sbtlCallBtlSearchEffectData2(sbtlFunc6Table[id], 12, pData)) {
                return 1;
            }
        } else {
            if (!sbtlCallBtlSearchEffectData2(sbtlFunc6Table[id], 12, pData)) {
                return 0;
            }
            return 1;
        }
    } else {
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlFunc6);
#endif

s32 sbtlLoadEffectData_Decode(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_WORK *unit;
    s32 i;
    s32 match;
    s32 exist;

    exist = wk != NULL;
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        unit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (unit == NULL) {
            continue;
        }
        if ((u16)(unit->mode - 1) < 2) {
            continue;
        }
        match = 0;
        if (exist != 1 || wk->field1A0 == unit->field1A0) {
            match = 1;
        }
        if (match != 1) {
            continue;
        }
        if (unit->field414 & 0x80) {
            if (!evtCommand_PMV_LOAD_END_WAIT()) {
                return 0;
            }
        } else if (unit->field414 & 2) {
            if (!sbtlLoadEffectData(unit)) {
                return 0;
            }
        } else if (!sbtlCallBtlSearchDelWorkSide(unit)) {
            return 0;
        }
    }
    return 1;
}

s32 sbtlInitLoadPlayerOnMemData(void) {
    switch (sbtlDat()->kind) {
        case 1:
            sbtlData83->pacHandle = pacLoad(sbtlStrBattleSbPack3SLb);
            break;
        case 2:
            sbtlData83->pacHandle = pacLoad(sbtlStrBattleSbPack3LLb);
            break;
        case 3:
            sbtlData83->pacHandle = pacLoad(sbtlStrBattleSbPack3ALb);
            break;
    }
    sbtlData83->setflg = 0;
    sbtlData83->dataCnt = 0;
    pacSync(sbtlData83->pacHandle);
    return 1;
}

s32 sbtlLoadEndCheck_PlayerOnMemData(void) {
    if (sbtlData83->setflg == 0) {
        return pacCheckEnd(sbtlData83->pacHandle) != 0;
    }
    return 1;
}

s32 sbtlLoadPlayerOnMemData(void) {
    sdfPacDecodeResrc_t *resrc;

    for (resrc = sbtlData83->pacHandle->pac.resrcHead; resrc != NULL; resrc = resrc->next) {
        if (sbtlData83->dataCnt == 15) {
            while (!sbtlLoadAttackHitData_pData(0, resrc->data)) {
            }
        } else {
            while (!sbtlBtlLoadPlayerOnMemDataSub(sbtlData83->dataCnt, resrc->data)) {
            }
        }
        sdfMemFreeDirect(resrc->resrc);
        sbtlData83->dataCnt++;
    }
    pacFileHandleFree_(sbtlData83->pacHandle);
    sbtlData83->setflg = 1;
    return 1;
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlBtlLoadPlayerOnMemDataSub);

s32 evtCommand_PMV_LOAD_END_WAIT(void) {
    return sbtlLoadEffectData_IDMask(sbtlCallBtlDat3(), EFFPAC_ST_DECODE | EFFPAC_ST_ONMEM) != 0;
}

s32 sbtlDeleteOnData(s32 flg) {
    s32 i;

    for (i = 0; i < SBTL_PACK_SLOT_COUNT; i++) {
        if (btlLoadSbPackData[i].unkC) {
            effPartsDelete(btlLoadSbPackData[i].unkC);
            btlLoadSbPackData[i].unkC = NULL;
        }
        if (flg == 1 && btlLoadSbPackData[i].handle) {
            sdfMemFreeDirect(btlLoadSbPackData[i].handle);
            btlLoadSbPackData[i].handle = NULL;
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallColorSet4);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallColorSet5);

SBTLEFF_LISTWORK *sbtlCreateEffList(void)
{
    SBTLEFF_LISTWORK *lwk = sdfChipAllocZeroDirect(sizeof(SBTLEFF_LISTWORK));

    lwk->num = 0;
    lwk->wk = NULL;
    return lwk;
}

SBTLEFF_WORK *sbtlCallMemAllocDirect3(SBTLEFF_LISTWORK *lwk, struct SBTLUNIT_WORK *wk, s32 id, s32 no)
{
    sdfMemHandle_t *handle;
    SBTLEFF_WORK *ewk;
    SBTLEFF_WORK *last;
    EFFPDATA_WORK *data;

    handle = sdfMemAllocDirect(sizeof(SBTLEFF_WORK));
    ewk = sdfMemLock(handle);
    memset(ewk, 0, sizeof(SBTLEFF_WORK));
    data = sbtlBtlSetEffectDataSub(id, no);
    ewk->handle = handle;
    ewk->effectid = data->eplno;
    ewk->scale = 1.0f;
    ewk->st = SBTLEFF_ST_INIT;
    ewk->starttime = 0;
    ewk->life = 0;
    ewk->dwk.hData = NULL;
    ewk->dwk.hFile = NULL;
    ewk->dwk.pData = NULL;
    ewk->dwk.hPartsList = NULL;
    if (wk != NULL && (wk->flg & SBTLEFF_FLG_NONSTOP)) {
        ewk->flg |= SBTLEFF_FLG_NONSTOP;
    }
    ewk->prev = NULL;
    ewk->next = NULL;
    last = lwk->wk;
    if (last == NULL) {
        lwk->wk = ewk;
        lwk->num = 1;
    } else {
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = ewk;
        ewk->prev = last;
        lwk->num++;
    }
    lwk->serial++;
    ewk->eListID = lwk->serial;
    ewk->serialID2 = lwk->serialID2;
    return ewk;
}

INCLUDE_RODATA(const s32, "sbtlEff_00289E50", sbtlStrBattleSbPack3LLb);

INCLUDE_RODATA(const s32, "sbtlEff_00289E50", sbtlStrBattleSbPack3ALb);

INCLUDE_RODATA(const s32, "sbtlEff_00289E50", sbtlBtlLoadPlayerOnMemDataSubTable2);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlGetDmgEffScale);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlSetDmgEffShinad);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCreateEff);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallMemset9);

/* NON_MATCHING: 5 of 14 words match.  */
#ifdef NON_MATCHING
s32 sbtlBtlCheckEffectDataSub(s32 id) {
    SBTLEFF_PACLIST *pac = &sbtlData83->effpac[id];

    if (pac->status & EFFPAC_ST_LOADEND) {
        return pac->num;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlBtlCheckEffectDataSub);
#endif

EFFPDATA_WORK *sbtlBtlSetEffectDataSub(s32 id, s32 no)
{
    if (sbtlData83->effpac[id].status & EFFPAC_ST_LOADEND) {
        return &sbtlData83->effpac[id].pwk[no]->wk;
    }
    return NULL;
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlBtlCheckDeleteEffectSub);

s32 sbtlBtlListProcSub(SBTLEFF_WORK *wk)
{
    if (wk->BEDsklid != 0 && (sbtlData83->effpac[wk->workID].status & EFFPAC_ST_LOADEND)) {
        return 1;
    }
    return 0;
}

s32 CheckEffectDataSingle(s32 type, s32 cmp)
{
    if (cmp < 2) {
        if (type <= cmp) {
            return 1;
        }
    } else if (type == cmp) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCheckEffectData);

s32 sbtlCheckEffectData_SeOnly(struct SBTLUNIT_WORK *wk, SKLID sklid, u8 type) {
    struct SBTLSKILLP_TBL *skl;
    s32 flg = 0;

    if (sbtlSearchEffectData(sklid) == -1) {
        return 0;
    }
    skl = sbtlGetSkilltbl(sklid);
    if (skl) {
        if (type == 1 && (skl->flg2 & 0x100000)) {
            flg = 1;
        }
        if (type == 2 && (skl->flg2 & 0x200000)) {
            flg = 1;
        }
    }
    if (!flg) {
        sbtlBstse(&wk->pos, sklid, type);
    }
    return 1;
}

/* body: matched in raidou @ 780f3f57: src/raidou1/code_0028A320.c:405, found, same function 1.00 (words 25/25, calls 1.00, shape 1.00) */
s32 sbtlCallBtlGetSkilltbl16(struct SBTLUNIT_WORK *tgtwk, SKLID sklid)
{
    u16 area = sbtlGetSkilltbl(sklid)->area;

    if (!(area & 0x10)) {
        return 1;
    }
    if ((tgtwk->flg & 0x8800) != 0x8800) {
        return 1;
    }
    if (!(area & 0x4000)) {
        return 1;
    }
    if (tgtwk->field498 == 0) {
        return 0;
    }
    return 2;
}

s32 CheckEffectData_Core(struct SBTLUNIT_LISTWORK *lwk, struct SBTLUNIT_WORK *wk, SKLID sklid)
{
    struct SBTLUNIT_WORK *unit;
    s32 i;

    if (sbtlGetSkilltbl(sklid)->area & 0x4000) {
        if ((wk->flg & 0x8800) == 0x8800 || ((wk->flg & 0x1000) && wk->unit.devilId == 626)) {
            for (i = 0; i < wk_sbtl->unitList->number;) {
                unit = wk_sbtl->unitList->pUnitL[i++].pUnit;
                if (unit != NULL && (unit->flg & 0xC800) == 0x8800 && unit->field498 != 0) {
                    sbtlEntryUnitList(lwk, unit);
                    return 1;
                }
            }
        }
    }
    sbtlEntryUnitList(lwk, wk);
    return 1;
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", SetDamageProc);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", CheckEffectData_Core_sbtlEff);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallCheckEffectDataSingle);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", GetDataScale);

INCLUDE_RODATA(const s32, "sbtlEff_00289E50", SetEffectPosTable);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", SetEffectPos);

void sbtlGetEffectData_Num(void)
{
    SBTLEFF_LISTWORK *lwk = wk_sbtl->effList;

    if (lwk != NULL) {
        lwk->serialID2++;
        if (lwk->serialID2 > 0xFFFE) {
            lwk->serialID2 = 0;
        }
    }
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlGetEffectSerialID2_Now);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlDeleteEffectData_SerialID2);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlSetDecAlpha_SerialID2);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlDeleteEffectData_SkillID);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceDelete_SPID);

/* NON_MATCHING: 20 of 37 words match.  */
#ifdef NON_MATCHING
s32 sbtlDeleteEffectData_SkillIDType(SBTLEFF_LISTWORK *lwk, SKLID skillid, u8 type) {
    SBTLEFF_WORK *wk = lwk->wk;
    if (!skillid) {
        return 1;
    }
    if (!lwk->num || !wk) {
        return 0;
    }
    do {
        if (wk->BEDsklid == skillid && wk->BEDtype == type) {
            if ((u32)(wk->st - 1) >= 2) {
                wk->st = 5;
                wk->alpha = 0;
                wk->flg |= 0x80000000;
            }
            wk->flg |= 8;
        }
        wk = wk->next;
    } while (wk);
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlDeleteEffectData_SkillIDType);
#endif

s32 sbtlDeleteEffectData_SkillIDTypeUnit(SBTLEFF_LISTWORK *lwk, SKLID skillid, u8 type, s32 unitID) {
    SBTLEFF_WORK *wk = lwk->wk;

    if (!skillid) {
        return 1;
    }
    if (!lwk->num || !wk) {
        return 0;
    }
    do {
        if (wk->BEDsklid == skillid && wk->BEDtype == type && wk->acterID == unitID) {
            if ((u32)(wk->st - 1) < 2) {
                wk->flg |= 8;
            } else {
                wk->st = 5;
                wk->alpha = 0;
            }
        }
        wk = wk->next;
    } while (wk);
    return 1;
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlEff_DeleteEffectData_EffID);

s32 sbtlGetStartTime_Damage(void) {
    SBTL_WORK *wk = wk_sbtl;

    if (++wk->effcnt >= 8) {
        wk->effcnt = 0;
    }
    return wk_sbtl->effcnt;
}

void Draw_sbtlEff(EFFID id, s32 *starttime, s32 *life)
{
    s32 table[58][2] = {
        { 0, 0 }, { 0, 162 }, { 0, 162 }, { 0, 120 }, { 0, 240 }, { 0, 120 },
        { 0, 160 }, { 0, 120 }, { 0, 40 }, { 0, 104 }, { 0, 162 }, { 0, 162 },
        { 0, 300 }, { 0, 162 }, { 0, 120 }, { 0, 50 }, { 0, 60 }, { 0, 60 },
        { 0, 120 }, { 0, 100 }, { 0, 40 }, { 0, 100 }, { 0, 60 }, { 0, 30 },
        { 0, 120 }, { 0, 120 }, { 0, 60 }, { 0, 64 }, { 0, 50 }, { 0, 50 },
        { 0, 90 }, { 0, 144 }, { 0, 120 }, { 0, 50 }, { 0, 60 }, { 0, 60 },
        { 0, 30 }, { 4, 44 }, { 0, 120 }, { 0, 160 }, { 0, 160 }, { 0, 120 },
        { 0, 160 }, { 0, 120 }, { 0, 120 }, { 0, 120 }, { 0, 120 }, { 0, 120 },
        { 0, 120 }, { 0, 160 }, { 0, 162 }, { 0, 120 }, { 0, 160 }, { 0, 160 },
        { 0, 160 }, { 0, 360 }, { 0, 160 }, { 0, 6 }
    };

    *starttime = table[id][0];
    *life = table[id][1];
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlListProc);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", effClassifyResourceMask);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallBtlGetSkilltbl10);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlLeaf30);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallBtlGetCutFlg);

SBTLEFF_WORK *sbtlGetEffectWork_ID(SBTLEFF_LISTWORK *lwk, s32 eListID)
{
    SBTLEFF_WORK *wk = lwk->wk;

    if (lwk->num == 0 || wk == NULL) {
        return NULL;
    }
    do {
        if (wk->eListID == eListID) {
            return wk;
        }
        wk = wk->next;
    } while (wk != NULL);
    return NULL;
}

s32 sbtlGetEffectStat_ID(SBTLEFF_LISTWORK *lwk, s32 eListID) {
    SBTLEFF_WORK *wk = lwk->wk;

    if (!lwk->num || !wk) {
        return -1;
    }
    for (;;) {
        if (wk->eListID == eListID) {
            return wk->st;
        }
        wk = wk->next;
        if (!wk) {
            break;
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceDelete_ID);

/* NON_MATCHING: 52 of 57 words match.  */
#ifdef NON_MATCHING
s32 sbtlForceDelete_ID_Acter(SBTLEFF_LISTWORK *lwk, struct SBTLUNIT_WORK *wk, EFFID effectid) {
    SBTLEFF_WORK *next;
    u32 flg;

    if (!wk_sbtl->effList) {
        return 0;
    }
    next = lwk->wk;
    if (!lwk->num || !next) {
        return 0;
    }
    do {
        if (next->effectid == effectid && sbtlCheckDeleteEffect(next) && wk->field1A0 == next->acterID) {
            flg = next->flg;
            if ((u32)(next->st - 1) < 2) {
                flg |= 0x80000000;
                flg |= 8;
            } else {
                next->st = 4;
                flg |= 0x80000000;
            }
            next->flg = flg;
        }
        next = next->next;
    } while (next);
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceDelete_ID_Acter);
#endif

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceDelete_ID_Summon);

/* NON_MATCHING: 19 of 39 words match.  */
#ifdef NON_MATCHING
s32 sbtlForceDelete_ID_SummonFR(SBTLEFF_LISTWORK *lwk, struct SBTLUNIT_WORK *wk) {
    SBTLEFF_WORK *next;
    u32 flg;

    if (!wk_sbtl->effList) {
        return 0;
    }
    next = lwk->wk;
    if (!lwk->num || !next) {
        return 0;
    }
    do {
        if (next->effectid == 40 && wk->field1A0 == next->acterID) {
            flg = next->flg | 0x80000000;
            if ((u32)(next->st - 1) < 2) {
                flg |= 8;
            } else {
                next->st = 4;
            }
            next->flg = flg;
        }
        next = next->next;
    } while (next);
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceDelete_ID_SummonFR);
#endif

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceDelete_ID_Return);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlFadeOut_BadStatusEffect);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlSetDecAlpha_SerialID);

s32 sbtlForceDelete_SerialID(SBTLEFF_LISTWORK *lwk, s32 serialID)
{
    SBTLEFF_WORK *wk = lwk->wk;

    if (lwk->num == 0 || wk == NULL) {
        return 0;
    }
    do {
        if (wk->hitserialID == serialID && sbtlCheckDeleteEffect(wk)) {
            if ((u32)(wk->st - SBTLEFF_ST_INIT) < 2) {
                wk->flg |= SBTLEFF_FLG_SESTOP;
                wk->flg |= SBTLEFF_FLG_FORCERESET;
            } else {
                wk->st = SBTLEFF_ST_END;
                wk->flg |= SBTLEFF_FLG_SESTOP;
            }
        }
        wk = wk->next;
    } while (wk != NULL);
    return 1;
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceDelete_SkillFlag);

s32 sbtlForceDelete_SkillFlag_Acter(struct SBTLUNIT_WORK *wk, u32 flg)
{
    SBTLEFF_LISTWORK *lwk;
    SBTLEFF_WORK *ewk;
    s32 ret = 0;

    if (wk_sbtl != NULL && (lwk = wk_sbtl->effList) != NULL) {
        if (wk->field6F4 != 0) {
            return 0;
        }
        ewk = lwk->wk;
        if (lwk->num != 0 && ewk != NULL) {
            do {
                if ((sbtlGetSkilltbl(ewk->BEDsklid)->flg & flg) && ewk->acterID == wk->field1A0) {
                    if ((u32)(ewk->st - SBTLEFF_ST_INIT) < 2) {
                        ewk->flg |= SBTLEFF_FLG_FORCERESET;
                    } else {
                        ewk->st = SBTLEFF_ST_EXIT;
                        ewk->alpha = 0;
                    }
                }
                ewk = ewk->next;
            } while (ewk != NULL);
        }
        ret = 1;
    }
    return ret;
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceDelete_SkillID_Acter);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceDeleteAll);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceDeleteAll_TimeStopSkill);

INCLUDE_RODATA(const s32, "sbtlEff_00289E50", sbtlLeaf30Table);

s32 sbtlCheckDeleteEffect(SBTLEFF_WORK *wk) {
    s32 ret = 1;

    if ((u16)((sbtlGetSkilltbl(wk->BEDsklid)->type & 0xFFF) - 9) < 2) {
        return 0;
    }
    if (sbtlBtlListProcSub(wk)) {
        return 1;
    }
    switch (wk->effectid) {
        case 3:
        case 4:
        case 5:
        case 7:
        case 14:
        case 18:
        case 40:
        case 42:
        case 44:
        case 51:
            ret = 0;
            break;
    }
    return ret;
}

s32 sbtlCheckSABData(SKLID sklid) {
    switch (sklid) {
    case 152:
    case 418:
    case 419:
    case 420:
    case 425:
    case 427:
    case 433:
    case 435:
    case 436:
    case 437:
    case 439:
    case 442:
    case 449:
    case 451:
    case 468:
    case 472:
    case 477:
    case 1120:
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceReset);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceReset_Tatari);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceResetDecAlpha);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlReleaseEff);

s32 sbtlDeleteEffList(SBTLEFF_LISTWORK *lwk) {
    s32 ret = sbtlReleaseEff(lwk, 1);

    if (ret) {
        sdfChipFreeDirect(lwk);
        return 1;
    }
    return ret;
}

s32 sbtlGetNumOfEffect_SAB(SBTLEFF_LISTWORK *lwk, s32 BEDtype, u8 side)
{
    s32 num = 0;
    SBTLEFF_WORK *wk = lwk->wk;

    if (lwk->num == 0 || wk == NULL) {
        return 0;
    }
    do {
        if (CheckEffectDataSingle(wk->BEDtype, BEDtype) && (wk->BEDacterside & side)) {
            num++;
        }
        wk = wk->next;
    } while (wk != NULL);
    return num;
}

/* NON_MATCHING: 8 of 29 words match.  */
#ifdef NON_MATCHING
s32 sbtlSearchEffect_SAB(SBTLEFF_LISTWORK *lwk, SKLID BEDsklid, s32 BEDtype) {
    SBTLEFF_WORK *wk = lwk->wk;

    if (lwk->num) {
        while (wk) {
            if (wk->BEDsklid == BEDsklid && CheckEffectDataSingle(wk->BEDtype, BEDtype)) {
                return 1;
            }
            wk = wk->next;
        }
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlSearchEffect_SAB);
#endif

s32 sbtlSearchEffect_SAB_Acter(SBTLEFF_LISTWORK *lwk, struct SBTLUNIT_WORK *wk, SKLID BEDsklid, s32 BEDtype) {
    SBTLEFF_WORK *ewk = lwk->wk;
    struct SBTLUNIT_WORK *acter;

    if (!lwk->num) {
        return 0;
    }
    while (ewk) {
        if (ewk->BEDsklid == BEDsklid && sbtlBtlListProcSub(ewk) && CheckEffectDataSingle(ewk->BEDtype, BEDtype)) {
            acter = sbtlSearchUnit_ID(wk_sbtl->unitList, ewk->acterID);
            if (acter && wk->field1A0 == acter->field1A0) {
                return 1;
            }
        }
        ewk = ewk->next;
    }
    return 0;
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlSearchEffect_ID);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlSearchEffect_WorkID);

s32 sbtlInitEffectWork(void)
{
    memset(sbtlData83->effpac, 0, sizeof(SBTLEFF_PACLIST));
    return 1;
}

s32 sbtlCheckLoadEnd_EffectWork(void) {
    s32 i;

    for (i = 0; i < SBTLEFF_PACLIST_MAX; i++) {
        if (sbtlData83->effpac[i].gstat > 0 && !(sbtlData83->effpac[i].status & EFFPAC_ST_ONMEM) &&
            !(sbtlData83->effpac[i].status & EFFPAC_ST_LOADEND)) {
            CheckLoadEffectData(&sbtlData83->effpac[i], sbtlData83->effpac[i].skillID, 0, NULL);
            return 0;
        }
    }
    return 1;
}

SBTLEFF_DELLISTWORK *sbtlCreateDelList(void)
{
    SBTLEFF_DELLISTWORK *lwk = sdfChipAllocZeroDirect(sizeof(SBTLEFF_DELLISTWORK));

    lwk->num = 0;
    lwk->dwk = NULL;
    return lwk;
}

s32 sbtlCheckDelWork_Request(SBTLEFF_DELLISTWORK *lwk, s32 GroupID) {
    SBTLEFF_DELWORK *dwk;

    if (lwk) {
        dwk = lwk->dwk;
        if (!dwk || !lwk->num) {
            return 0;
        }
        if (dwk->gstat == SBTLEFF_DELST_PROC && dwk->GroupID != -1 && GroupID == dwk->GroupID) {
            return 1;
        }
    }
    return 0;
}

s32 sbtlSearchDelWork_Side(SBTLEFF_DELLISTWORK *lwk, u8 side) {
    SBTLEFF_DELWORK *dwk;

    if (lwk) {
        dwk = lwk->dwk;
        if (!dwk || !lwk->num) {
            return 0;
        }
        do {
            if (dwk->gstat == SBTLEFF_DELST_PROC && (dwk->side & side)) {
                return 1;
            }
            dwk = dwk->next;
        } while (dwk);
        return 0;
    }
    return 0;
}

s32 sbtlCreateDelWork(SBTLEFF_DELLISTWORK *lwk, SKLID skillid, u8 side, s32 GroupID) {
    sdfMemHandle_t *handle;
    SBTLEFF_DELWORK *dwk;
    SBTLEFF_DELWORK *last;

    handle = sdfMemAllocDirect(sizeof(SBTLEFF_DELWORK));
    dwk = sdfMemLock(handle);
    memset(dwk, 0, sizeof(SBTLEFF_DELWORK));
    dwk->handle = handle;
    dwk->prev = NULL;
    dwk->next = NULL;
    last = lwk->dwk;
    if (!last) {
        lwk->dwk = dwk;
        lwk->num = 1;
    } else {
        while (last->next) {
            last = last->next;
        }
        last->next = dwk;
        dwk->prev = last;
        lwk->num++;
    }
    dwk->GroupID = GroupID;
    dwk->sklid = skillid;
    dwk->side = side;
    dwk->gstat = SBTLEFF_DELST_PROC;
    return 1;
}

s32 sbtlDelProc(SBTLEFF_DELLISTWORK *lwk)
{
    SBTLEFF_DELWORK *dwk;

    if (lwk == NULL) {
        return 0;
    }
    dwk = lwk->dwk;
    if (dwk == NULL || lwk->num == 0) {
        return 0;
    }
    while (dwk != NULL) {
        switch (dwk->gstat) {
        case SBTLEFF_DELST_PROC:
            if (!sbtlSearchEffect_SAB(wk_sbtl->effList, dwk->sklid, SBTLEFF_BEDTYPE_KOUKA)
                && !sbtlSearchEffect_SAB(wk_sbtl->effList, dwk->sklid, SBTLEFF_BEDTYPE_HUKA)
                && !sbtlSearchEffect_SAB(wk_sbtl->effList, dwk->sklid, SBTLEFF_BEDTYPE_HUKA2)) {
                sbtlForceDelete_EffectWork_ID(dwk->sklid, 1);
                dwk->gstat = SBTLEFF_DELST_EXIT;
            }
            break;
        case SBTLEFF_DELST_EXIT:
            dwk->gstat = SBTLEFF_DELST_END;
            break;
        }
        dwk = dwk->next;
    }
    return 1;
}

s32 sbtlReleaseDelList(SBTLEFF_DELLISTWORK *lwk, u8 flg)
{
    SBTLEFF_DELWORK *dwk;
    SBTLEFF_DELWORK *next;

    if (lwk == NULL) {
        return 0;
    }
    dwk = lwk->dwk;
    if (dwk == NULL || lwk->num == 0) {
        return 0;
    }
    do {
        if (flg) {
            dwk->gstat = SBTLEFF_DELST_END;
        }
        if (dwk->gstat == SBTLEFF_DELST_END) {
            if (dwk->prev != NULL) {
                dwk->prev->next = dwk->next;
            }
            if (dwk->next != NULL) {
                dwk->next->prev = dwk->prev;
            }
            if (dwk->prev == NULL) {
                lwk->dwk = dwk->next;
                if (lwk->dwk != NULL) {
                    lwk->dwk->prev = NULL;
                }
            }
            next = dwk->next;
            sdfMemFreeDirect(dwk->handle);
            dwk = next;
            lwk->num--;
        } else {
            dwk = dwk->next;
        }
    } while (dwk != NULL);
    return 1;
}

s32 sbtlDeleteDelList(SBTLEFF_DELLISTWORK *lwk) {
    sbtlReleaseDelList(lwk, 1);
    sdfChipFreeDirect(lwk);
    return 1;
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlDeleteHaveSkill_EffectWork);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceDelete_EffectWork_ID);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceDelete_EffectWork);

/* NON_MATCHING: 4 of 18 words match.  */
#ifdef NON_MATCHING
void sbtlBtlForceDeleteEffectWorkIDSub2(s32 id) {
    sbtlData83->effpac[id].handle = NULL;
    sbtlData83->effpac[id].skillID = -1;
    sbtlData83->effpac[id].pData = NULL;
    sbtlData83->effpac[id].SMGID = 0;
    sbtlData83->effpac[id].status = 0;
    sbtlData83->effpac[id].gstat = 0;
}
#else
INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlBtlForceDeleteEffectWorkIDSub2);
#endif

s32 sbtlSearchEffectData(SKLID skillid)
{
    s32 i;

    for (i = 0; i < SBTLEFF_PACLIST_MAX; i++) {
        if (sbtlData83->effpac[i].skillID == skillid) {
            return i;
        }
    }
    return -1;
}

s32 sbtlBtlLoadEffectDataIDMaskSub(SKLID skillid)
{
    s32 i;

    for (i = 0; i < SBTLEFF_PACLIST_MAX; i++) {
        if ((u16)(sbtlData83->effpac[i].skillID + 1) < 2) {
            return i;
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlLoadEffectData_IDMask);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallBtlSearchEffectData2);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlLoadEffectData_ID);

s32 sbtlGetExeSkill(SKLID sklid)
{
    s32 no = sbtlGetSklVisualTbl(sklid)->SABno;

    if (no == 0) {
        no = -1;
    }
    return no;
}

/* NON_MATCHING: 39 of 75 words match.  */
#ifdef NON_MATCHING
s32 sbtlCallBtlSearchDelWorkSide(struct SBTLUNIT_WORK *wk) {
    struct SBTLSKILLP_TBL *tbl;
    s32 sklid[4];
    s32 i;
    s32 k;

    if (sbtlSearchDelWork_Side(wk_sbtl->effdelList, wk->field414)) {
        return 0;
    }
    for (i = 0; i < 16; i++) {
        sklid[0] = sbtlCallBtlGetSkilltbl23(wk->haveskill[i + 2], &sklid[1]);
        if (sklid[0] != 0) {
            k = 0;
            do {
                tbl = sbtlGetSkilltbl(sklid[k]);
                if (!(tbl->flg2 & 0x1000) && (!(tbl->type & 0x2000) || sklid[k] == SKLID_OFS_AUTO)
                    && sbtlGetExeSkill(sklid[k]) != -1 && !sbtlLoadEffectData_IDMask(sklid[k], EFFPAC_ST_DECODE)) {
                    return 0;
                }
                k++;
            } while (k < 2 && sklid[k] != 0);
        }
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallBtlSearchDelWorkSide);
#endif

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlLoadEffectData);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", CheckLoadEffectData);

/* NON_MATCHING: 43 of 53 words match.  */
#ifdef NON_MATCHING
s32 sbtlSetSummonEffect_Nakama(struct SBTLUNIT_WORK *wk, s8 flg) {
    if (wk_sbtl->status4 & 0x200) {
        flg = 1;
    }
    switch (flg) {
    case 0:
        sbtlCreateEff(wk_sbtl->effList, wk, EFFID_SUMMON_FR, 0, &wk->pos, 0x80808080, 0);
        break;
    case 1:
        sbtlCreateEff(wk_sbtl->effList, wk, EFFID_SUMMON_ND, 0, &wk->pos, 0x80808080, 0);
        break;
    case 2:
        sbtlCreateEff(wk_sbtl->effList, wk, EFFID_SWTIN_FR, 0, &wk->pos, 0x80808080, 0);
        break;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlSetSummonEffect_Nakama);
#endif

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlSetReturnEffect_Nakama);

SBTLEFFDMG_TBL *LoadOnMemoryData(s32 major, s32 minor) {
    if (!major) {
        return &dmg_tbldata_p[minor];
    }
    return &dmg_tbldata[minor];
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCheckYukaDamageEffect);

/* NON_MATCHING: 5 of 19 words match.  */
#ifdef NON_MATCHING
s32 sbtlCallIsSceanStop2(SBTLEFF_WORK *wk) {
    s32 ret = 1;

    if (wk_sbtl->flg & 0x8000000) {
        if (wk->flg & 0x10) {
            return ret;
        }
        ret = IsSceanStop() == 0;
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallIsSceanStop2);
#endif

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallPartsCreateFinish);

void LoadOnMemoryData_BED(SBTLEFF_WORK *wk, struct SBTLUNIT_WORK *wklock, u16 no)
{
    SBTLDMG_HITWORK *hit;
    u32 flg = wk->flg;

    if (!(flg & SBTLEFF_FLG_LOADEND)) {
        wk->counter = 0;
        wk->flg = flg | SBTLEFF_FLG_LOADEND;
        flg = wk->flg;
    }
    if (!(flg & SBTLEFF_FLG_FORCERESET) && (no == 0 || (wklock != NULL && wklock->status == no))) {
        if (wk->starttime > wk->counter) {
            return;
        }
        if (flg & SBTLEFF_FLG_NONEFF) {
            wk->dwk.hPartsList = NULL;
        } else {
            if (sbtlData83->effpac[wk->workID].status & EFFPAC_ST_DECODE) {
                wk->dwk.hPartsList = effPartsCreateFinish(sbtlData83->effpac[wk->workID].pwk[wk->effectid]->hPartsList);
            } else {
                wk->dwk.hPartsList = Create_effPartsList(sbtlData83->effpac[wk->workID].pwk[wk->effectid]->pData);
            }
            effPartsRotateSet(wk->dwk.hPartsList, (const f32 *)&wk->rot);
            effPartsScaleSet(wk->dwk.hPartsList, wk->scale);
            if (wk->flg & SBTLEFF_FLG_LOCKHIT) {
                hit = sbtlSearchHit_SerialID(wk_sbtl->hitList, wk->hitserialID);
                if (hit != NULL) {
                    SDF_VU0_LOADVECTOR(&hit->pos);
                    SDF_VU0_STOREVECTOR(&wk->pos);
                    effPartsPositionSet(wk->dwk.hPartsList, (const f32 *)&wk->pos);
                }
            } else {
                effPartsPositionSet(wk->dwk.hPartsList, (const f32 *)&wk->pos);
            }
        }
        wk->st = SBTLEFF_ST_PROC;
        wk->cmpcounter = wk->life;
        wk->counter = 0;
        return;
    }
    wk->st = SBTLEFF_ST_EXIT;
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", EffFunc_Slip_G);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", EffFunc_Hanyou);

INCLUDE_RODATA(const s32, "sbtlEff_00289E50", sbtlCallBtlGetSklVisualTblTable3);

INCLUDE_RODATA(const s32, "sbtlEff_00289E50", sbtlCheckYukaDamageEffectTable);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallBtlSearchUnitID9);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallBtlSearchUnitID10);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallBtlSearchUnitID11);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", EffFunc_Hanyou_BED);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", EffFunc_NMLDAMAGE);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", EffFunc_DEADDAMAGE);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallLoadOnMemoryDataSBtlEff);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", effPartsColorSet);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", EffFunc_SUMMON);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", EffFunc_RETURN);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", EffFunc_SUMMON_E);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallBtlSearchUnitID22);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallBtlSearchUnitID23);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallBtlSearchUnitID24);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallBtlSearchUnitID25);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlBtlAddHitEffSub);

void sbtlOutHitEff(SBTL_HITEFF *pHitEff) {
    SBTL_HITEFF *next = pHitEff->next;

    if (next) {
        if (pHitEff->prev) {
            pHitEff->prev->next = next;
            pHitEff->next->prev = pHitEff->prev;
            return;
        }
        wk_sbtl->hitEffList.pHitEffBgn = next;
        pHitEff->next->prev = NULL;
        return;
    }
    next = pHitEff->prev;
    if (next) {
        wk_sbtl->hitEffList.pHitEffEnd = next;
        pHitEff->prev->next = NULL;
        return;
    }
    wk_sbtl->hitEffList.pHitEffBgn = NULL;
    wk_sbtl->hitEffList.pHitEffEnd = NULL;
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlAddHitEff);

void sbtlDltHitEff(SBTLEFF_WORK *pEffWork) {
    SBTL_HITEFF *pHitEff = pEffWork->pHitEff;

    if (pHitEff) {
        sbtlOutHitEff(pHitEff);
        pHitEff->bUse = 0;
        wk_sbtl->hitEffList.cnt--;
        pEffWork->pHitEff = NULL;
    }
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlLeaf31);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallColorSet);

SBTL_FIELD630_WORK *sbtlCallMemAllocDirect4(void)
{
    sdfMemHandle_t *handle = sdfMemAllocDirect(sizeof(SBTL_FIELD630_WORK));
    SBTL_FIELD630_WORK *wk = sdfMemLock(handle);

    memset(wk, 0, sizeof(SBTL_FIELD630_WORK));
    wk->handle = handle;
    wk->num = 0;
    return wk;
}

s32 sbtlRefSbtl42(SBTL_FIELD630_WORK *wk)
{
    if (wk_sbtl->field630 == NULL) {
        return 0;
    }
    return wk->num > 0;
}

s32 sbtlDeleteTalk(SBTL_FIELD630_WORK *wk)
{
    sdfMemFreeDirect(wk->handle);
    return 1;
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlSetDevilSkillID);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlRefSbtl43);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlLeaf76);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlRefSbtl44);

SBTL_STP *evtAllocateContext(void)
{
    SBTL_STP *stp = sdfChipAllocZeroDirect(sizeof(SBTL_STP));

    stp->enabled = 0;
    stp->head = NULL;
    return stp;
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCreateSTP_Unit);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", CreateSTP);

s32 sbtlProcSBtlSTPSub(struct SBTLUNIT_WORK *wk)
{
    s32 status = wk->status;

    if (status != 11 && status != 14 && wk->model->mode != 9 && wk->mode >= 5) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", Proc_sbtlSTP);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", Draw_sbtlSTP);

s32 sbtlReleaseSTP_List(SBTL_STP *stp, u8 flg) {
    SBTL_STP_NODE *node;
    SBTL_STP_NODE *next;

    if (stp == NULL) {
        return 1;
    }
    node = stp->head;
    if (node == NULL || stp->enabled == 0) {
        return 0;
    }
    do {
        if (flg) {
            node->mode = 3;
        }
        if (node->mode == 3) {
            if (node->prev != NULL) {
                node->prev->next = node->next;
            }
            if (node->next != NULL) {
                node->next->prev = node->prev;
            }
            if (node->prev == NULL) {
                stp->head = node->next;
                if (stp->head != NULL) {
                    stp->head->prev = NULL;
                }
            }
            next = node->next;
            sdfMemFreeDirect(node->handle);
            node = next;
            stp->enabled--;
        } else {
            node = node->next;
        }
    } while (node != NULL);
    return 1;
}

s32 sbtlDeleteSTPList(SBTL_STP *lwk) {
    if (lwk == NULL) {
        return 1;
    }
    sbtlReleaseSTP_List(lwk, 1);
    sdfChipFreeDirect(lwk);
    return 1;
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", Check_SupportAction);

void sbtlRefSbtl45(void) {
    SBTL_PANEL *panel = sbtlCallMemAllocDirect5();

    panel->resources = NULL;
    panel->hPanelSpr2 = NULL;
    panel->hPanelSpr3 = NULL;
    panel->hPanelSpr4 = NULL;
    panel->hPanelSpr5 = NULL;
    wk_sbtl->panel = panel;
    panel->mds_waku0.x = 314;
    panel->mds_waku0.y = 2;
    panel->mds_waku1.x = 314;
    panel->mds_waku1.y = 2;
    panel->mds_light.x = 297;
    panel->mds_light.y = 0;
    wk_sbtl->field630 = sbtlCallMemAllocDirect4();
}

s32 sbtlDeleteMainPanel(void) {
    s32 ret;

    if (wk_sbtl->flg & 0x800) {
        ret = DeletePanel(wk_sbtl->panel);
        if (ret) {
            wk_sbtl->panel = NULL;
            wk_sbtl->flg &= ~0x800;
        }
        return ret;
    }
    return 1;
}

SBTL_PANEL *sbtlCallMemAllocDirect5(void) {
    sdfMemHandle_t *handle = sdfMemAllocDirect(sizeof(SBTL_PANEL));
    SBTL_PANEL *panel = sdfMemLock(handle);

    memset(panel, 0, sizeof(SBTL_PANEL));
    panel->handle = handle;
    panel->resources = NULL;
    panel->hPanelSpr2 = NULL;
    panel->hPanelSpr3 = NULL;
    panel->hPanelSpr4 = NULL;
    panel->hPanelSpr5 = NULL;
    panel->list = InitDebugProcessData(0);
    panel->alpha = 0;
    panel->mode = 0;
    return panel;
}

/* NON_MATCHING: 30 of 33 words match.  */
#ifdef NON_MATCHING
s32 DeletePanel(SBTL_PANEL *wk) {
    s32 ret = 1;

    if (wk->flg & 1) {
        if (!(wk->flg & 2)) {
            return 0;
        }
        effDestroyResourceSlotSet(wk->resources);
        effDestroyResourceSlotSet(wk->hPanelSpr3);
        effDestroyResourceSlotSet(wk->hPanelSpr4);
        effDestroyResourceSlotSet(wk->hPanelSpr5);
        effDestroyEffectList(wk->list);
        sdfMemFreeDirect(wk->handle);
        sbtlDeleteTalk(wk_sbtl->field630);
        wk_sbtl->field630 = NULL;
        return 1;
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "sbtlEff_00289E50", DeletePanel);
#endif

s32 sbtlRefSbtl47(void) {
    SBTL_PANEL *panel;

    if (wk_sbtl == NULL || (panel = wk_sbtl->panel) == NULL) {
        return 0;
    }
    if (!(panel->flg & 2)) {
        return 0;
    }
    sbtlLeaf77(panel);
    sbtlFunc13();
    sbtlCallCommandFLDSETTAKARAOPEN();
    sbtlRefSbtl42(wk_sbtl->field630);
    return 1;
}

s32 sbtlFunc13(void)
{
    sbtlRefSbtl49();
    return 1;
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", Proc_sbtlPanel);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlResetMidashiAnim);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlRefSbtl48);

s32 sbtlRefSbtl49(void) {
    return wk_sbtl->mode == 2;
}

s32 sbtlCallCommandFLDSETTAKARAOPEN(void)
{
    sbtlCallRefSbtl51();
    return 1;
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCheckSelectAnim);

INCLUDE_RODATA(const s32, "sbtlEff_00289E50", Draw_sbtlSTPTable2);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlSetSelectAnim);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlResetSelectAnim);

/* NON_MATCHING: 8 of 79 words match.  */
#ifdef NON_MATCHING
s32 sbtlMainDraw(void) {
    SBTL_PANEL *panel = wk_sbtl->panel;
    u8 kaiwaflg;

    if (panel) {
        if (!panel->resources) {
            return 0;
        }
        if ((panel->flg & 2) && panel->mode != 3 && !(wk_sbtl->status4 & 0x10)) {
            sbtlEnemyData();
            kaiwaflg = wk_sbtl->field42CA;
            if ((u32)(kaiwaflg - 1) < 2) {
                if (wk_sbtl->field42CB) {
                    sbtlPlayerData();
                }
                if (wk_sbtl->field42CC) {
                    sbtlSummonDevilData();
                }
            } else {
                sbtlPlayerData();
                sbtlSummonDevilData();
            }
            sbtlLeaf78();
            eglSetOt(0x68);
            if (!wk_sbtl->fieldCA2) {
                if (wk_sbtl->field430) {
                    evtMoonFadeOut(1);
                } else if (!wk_sbtl->bBfrBtlEnd) {
                    evtEnableMoonDraw();
                } else if (wk_sbtl->flg2 & 2) {
                    evtEnableMoonDraw();
                }
            }
            return 1;
        }
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlMainDraw);
#endif

/* NON_MATCHING: 10 of 48 words match.  */
#ifdef NON_MATCHING
s32 sbtlMainDraw2(void) {
    SBTL_PANEL *panel = wk_sbtl->panel;
    s32 i;

    if (panel) {
        if (!panel->resources) {
            return 0;
        }
        if ((panel->flg & 2) && panel->mode != 3 && !(wk_sbtl->status4 & 0x10)) {
            for (i = 0; i < 2; i++) {
                if (sbtlGetNakama(i)) {
                    sbtlSummonDevilAshimoto(sbtlGetNakama(i), i);
                }
            }
            return 1;
        }
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlMainDraw2);
#endif
