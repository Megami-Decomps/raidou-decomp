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

s32 OnDataLoadProc(dds3ProcessID_t *id) {
    s32 i;

    if (!sbtlCallStartRain()) {
        return 0;
    }
    if (!sbtlBattleEventSb()) {
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

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlFunc6);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlLoadEffectData_Decode);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlInitLoadPlayerOnMemData);

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

INCLUDE_ASM(const s32, "sbtlEff_00289E50", evtCommand_PMV_LOAD_END_WAIT);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlDeleteOnData);

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

INCLUDE_RODATA(const s32, "sbtlEff_00289E50", sbtlBtlLoadPlayerOnMemDataSubTable2);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlGetDmgEffScale);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlSetDmgEffShinad);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCreateEff);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallMemset9);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlBtlCheckEffectDataSub);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlBtlSetEffectDataSub);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlBtlCheckDeleteEffectSub);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlBtlListProcSub);

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

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCheckEffectData_SeOnly);

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

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlDeleteEffectData_SkillIDType);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlDeleteEffectData_SkillIDTypeUnit);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlEff_DeleteEffectData_EffID);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlGetStartTime_Damage);

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

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlGetEffectStat_ID);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceDelete_ID);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceDelete_ID_Acter);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceDelete_ID_Summon);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceDelete_ID_SummonFR);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceDelete_ID_Return);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlFadeOut_BadStatusEffect);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlSetDecAlpha_SerialID);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceDelete_SerialID);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceDelete_SkillFlag);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceDelete_SkillFlag_Acter);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceDelete_SkillID_Acter);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceDeleteAll);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceDeleteAll_TimeStopSkill);

INCLUDE_RODATA(const s32, "sbtlEff_00289E50", sbtlLeaf30Table);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCheckDeleteEffect);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCheckSABData);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallBtlCheckSABData);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceReset_Tatari);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceResetDecAlpha);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlReleaseEff);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlDeleteEffList);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlGetNumOfEffect_SAB);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlSearchEffect_SAB);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlSearchEffect_SAB_Acter);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlSearchEffect_ID);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlSearchEffect_WorkID);

s32 sbtlInitEffectWork(void)
{
    memset(sbtlData83->effpac, 0, sizeof(SBTLEFF_PACLIST));
    return 1;
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCheckLoadEnd_EffectWork);

SBTLEFF_DELLISTWORK *sbtlCreateDelList(void)
{
    SBTLEFF_DELLISTWORK *lwk = sdfChipAllocZeroDirect(sizeof(SBTLEFF_DELLISTWORK));

    lwk->num = 0;
    lwk->dwk = NULL;
    return lwk;
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCheckDelWork_Request);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlSearchDelWork_Side);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCreateDelWork);

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
                sbtlCallBtlSearchEffectData(dwk->sklid, 1);
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

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlDeleteDelList);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlDeleteHaveSkill_EffectWork);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallBtlSearchEffectData);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceDelete_EffectWork);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlBtlForceDeleteEffectWorkIDSub2);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlSearchEffectData);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlBtlLoadEffectDataIDMaskSub);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlLoadEffectData_IDMask);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallBtlSearchEffectData2);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlLoadEffectData_ID);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlGetExeSkill);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallBtlSearchDelWorkSide);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlLoadEffectData);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", CheckLoadEffectData);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlSetSummonEffect_Nakama);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlSetReturnEffect_Nakama);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", LoadOnMemoryData);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCheckYukaDamageEffect);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallIsSceanStop2);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallPartsCreateFinish);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", LoadOnMemoryData_BED);

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

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlOutHitEff);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlAddHitEff);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlDltHitEff);

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

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallMemFreeDirect4);

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

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlProcSBtlSTPSub);

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

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlDeleteSTPList);

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

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlRefSbtl46);

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

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallMemFreeDirect5);

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

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlMainDraw);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlMainDraw2);
