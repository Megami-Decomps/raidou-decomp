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
#include "code_r1_0027CBE8.h"
#include "sBtlVisual.h"
#include "sBtlSound.h"
#include "sbtlSub.h"
#include "effStat.h"
#include "sBtlScript.h"
#include "sBtlItem.h"
#include "sBtlCalc.h"

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

void OnDataLoadShutdown(dds3ProcessID_t *id) {
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

f32 sbtlGetDmgEffScale(struct SBTLUNIT_WORK *wk)
{
    f32 scale = 1.0f;

    switch (wk->unit.devilId) {
    case 0x1D5:
    case 0x1D6:
    case 0x1D7:
    case 0x1DB:
    case 0x1DC:
    case 0x1DD:
        scale = 2.0f;
        break;
    case 0x1B8:
    case 0x1CE:
        scale = 5.0f;
        break;
    }
    return scale;
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlSetDmgEffShinad);

SBTLEFF_WORK *sbtlCreateEff(SBTLEFF_LISTWORK *lwk, struct SBTLUNIT_WORK *wk, EFFID effectid, s32 starttime, const f32 *defpos,
                            u32 rgba, s8 flg)
{
    sdfVector4_t a;
    sdfVector4_t b;
    sdfVector4_t defrot;
    s32 start;
    s32 life;
    sdfMemHandle_t *handle;
    SBTLEFF_WORK *ewk;
    SBTLEFF_WORK *last;

    memset(&defrot, 0, sizeof(defrot));
    defrot.w = 1.0f;
    handle = sdfMemAllocDirect(sizeof(SBTLEFF_WORK));
    ewk = sdfMemLock(handle);
    memset(ewk, 0, sizeof(SBTLEFF_WORK));
    ewk->handle = handle;
    ewk->starttime = starttime;
    ewk->effectid = effectid;
    Draw_sbtlEff(effectid, &start, &life);
    ewk->scale = 1.0f;
    ewk->life = life;
    ewk->rgba = rgba;
    ewk->workID = 0;
    ewk->st = SBTLEFF_ST_INIT;
    ewk->flg |= sbtlCallMemset17Data[ewk->effectid];
    ewk->dwk.hData = NULL;
    ewk->dwk.hFile = NULL;
    ewk->dwk.pData = NULL;
    ewk->dwk.hPartsList = NULL;
    if (wk->flg & 0x10) {
        ewk->flg |= 0x10;
    }
    if (flg == 1) {
        ewk->flg |= SBTLEFF_FLG_RGBASET;
    }
    ewk->prev = NULL;
    ewk->next = NULL;
    if (wk != NULL) {
        ewk->acterID = wk->field1A0;
        ewk->targetID = wk->model->field104;
        SDF_VU0_LOADVECTOR(&wk->pos);
        SDF_VU0_STOREVECTOR(&ewk->pos);
    } else {
        ewk->acterID = 0;
        ewk->targetID = 0;
        SDF_VU0_LOADVECTOR(defpos);
        SDF_VU0_STOREVECTOR(&ewk->pos);
    }
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
    switch (ewk->effectid) {
    case 8:
    case 9:
    case 12:
    case 15:
    case 20:
    case 21:
    case 26:
    case 27:
    case 28:
    case 29:
    case 33:
        sbtlGetUnitHelperPos(wk, 0);
        SDF_VU0_STOREVECTOR(&ewk->pos);
        if (ewk->pos.y >= -1.0) {
            ewk->pos.y -= 50.0f;
        }
        SDF_VU0_LOADVECTOR(&wk_sbtl->camera.camerapos);
        SDF_VU0_LOADVECTOR_B(&ewk->pos);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_NORMALIZE();
        SDF_VU0_SCALEVECTOR(sbtlGetMotionVisualData(wk->major, wk->minor)->field24);
        SDF_VU0_LOADVECTOR_B(&ewk->pos);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(&ewk->pos);
        ewk->scale = sbtlGetDmgEffScale(wk);
        if (wk->unit.devilId >= 455 && wk->unit.devilId <= 456) {
            sbtlSetDmgEffShinad(ewk, wk, 0);
        }
        break;
    case 34:
        SDF_VU0_LOADVECTOR(&wk->pos);
        SDF_VU0_STOREVECTOR(&ewk->pos);
        ewk->scale = sbtlGetDmgEffScale(wk);
        break;
    case 56:
        sbtlGetUnitHelperPos(wk, 0);
        SDF_VU0_STOREVECTOR(&ewk->pos);
        break;
    case 1:
    case 2:
    case 6:
    case 10:
    case 11:
    case 13:
    case 50:
    case 55:
        sbtlGetUnitHelperPos(wk, 0);
        SDF_VU0_STOREVECTOR(&ewk->pos);
        SDF_VU0_STOREVECTOR(&a);
        ewk->pos.y = 0.0f;
        SDF_VU0_LOADVECTOR(defpos);
        SDF_VU0_STOREVECTOR_NOMEM(&b);
        a.y = 0.0f;
        b.y = 0.0f;
        sbtlGetQuat(&b, &a);
        SDF_VU0_STOREVECTOR(&ewk->rot);
        ewk->pos.y = sbtlFree(ewk->pos.x, ewk->pos.z);
        ewk->scale = sbtlGetDmgEffScale(wk);
        if (wk->unit.devilId >= 455 && wk->unit.devilId <= 456) {
            sbtlSetDmgEffShinad(ewk, wk, 1);
        }
        break;
    case 16:
    case 17:
    case 19:
        SDF_VU0_LOADVECTOR(&wk->pos);
        SDF_VU0_STOREVECTOR(&a);
        sbtlGetPosforDistance(wk, 10.0f);
        SDF_VU0_STOREVECTOR_NOMEM(&b);
        a.y = 0.0f;
        b.y = 0.0f;
        sbtlGetQuat(&b, &a);
        SDF_VU0_STOREVECTOR_NOMEM(&ewk->rot);
        a.x = ewk->pos.y;
        a.y = ewk->pos.x;
        a.z = ewk->pos.z;
        a.w = ewk->pos.w;
        b.x = defpos[4];
        b.y = defpos[0];
        b.z = defpos[8];
        b.w = defpos[12];
        sbtlGetQuat(&a, &b);
        SDF_VU0_STOREVECTOR(&a);
        ewk->rot.x = 0.5f;
        sbtlGetUnitHelperPos(wk, 0);
        SDF_VU0_STOREVECTOR(&a);
        ewk->pos.y = a.y;
        ewk->scale = sbtlGetDmgEffScale(wk);
        break;
    case 3:
    case 4:
    case 40:
    case 41:
    case 42:
    case 43:
    case 45:
    case 46:
    case 47:
    case 48:
        SDF_VU0_LOADVECTOR(&wk->pos);
        SDF_VU0_STOREVECTOR(&ewk->pos);
        ewk->scale = GetDataScale(wk, 0);
        SDF_VU0_LOADVECTOR(&defrot);
        SDF_VU0_STOREVECTOR(&ewk->rot);
        ewk->pos.y = sbtlFree(ewk->pos.x, ewk->pos.z);
        break;
    }
    ewk->eListID = ++lwk->serial;
    switch (ewk->effectid) {
    case 1:
    case 9:
    case 10:
    case 11:
    case 13:
    case 15:
    case 16:
    case 17:
    case 19:
    case 33:
    case 50:
        sbtlAddHitEff(ewk);
        break;
    default:
        ewk->pHitEff = NULL;
        break;
    }
    return ewk;
}

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

SBTLEFF_PACDATAWORK *sbtlGetEffectPacData(s32 id, s32 no)
{
    if (sbtlData83->effpac[id].status & EFFPAC_ST_LOADEND) {
        return sbtlData83->effpac[id].pwk[no];
    }
    return NULL;
}

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

s32 sbtlCheckEffectData(struct SBTLUNIT_WORK *wk, SKLID skillid, u8 flg, s8 type, u16 side, s32 id, s32 param)
{
    struct SBTLSKILLP_TBL *tbl;
    EFFPDATA_WORK *data;
    s32 index;
    s32 i;
    s32 num;
    s32 mute;
    u32 bit;

    mute = 0;
    index = sbtlSearchEffectData(skillid);
    if (index == -1) {
        return 0;
    }
    tbl = sbtlGetSkilltbl(skillid);
    if (tbl != NULL) {
        if (flg == 1) {
            bit = tbl->flg2 & 0x100000;
            mute = bit != 0;
        }
        if (flg == 2 && (tbl->flg2 & 0x200000)) {
            mute = 1;
        }
    }
    if (!mute) {
        sbtlBstse(&wk->pos, skillid, flg);
    }
    num = sbtlBtlCheckEffectDataSub(index);
    for (i = 0; i < num; i++) {
        data = sbtlBtlSetEffectDataSub(index, i);
        if (!CheckEffectDataSingle(data->type, flg)) {
            continue;
        }
        if (type == 1) {
            return 1;
        }
        switch (data->iti & 0xF) {
        case 0:
            CheckEffectData_Core_sbtlEff(wk, skillid, flg, index, i, side, id, 1, param, i);
            break;
        case 4:
            CheckEffectData_Core_sbtlEff(wk, skillid, flg, index, i, side, id, 2, param, i);
            break;
        case 5:
            CheckEffectData_Core_sbtlEff(wk, skillid, flg, index, i, side, id, 3, param, i);
            break;
        case 8:
            CheckEffectData_Core_sbtlEff(wk, skillid, flg, index, i, side, id, 4, param, i);
            break;
        default:
            CheckEffectData_Core_sbtlEff(wk, skillid, flg, index, i, side, id, 0, param, i);
            break;
        }
    }
    if (num > 0) {
        sbtlGetEffectData_Num();
        sbtlAddSerialID2();
    }
    return 1;
}

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

s32 SetDamageProc(struct SBTLUNIT_WORK *wklock, struct SBTLUNIT_WORK *wk, SKLID sklid, const sdfVector4_t *ppos) {
    struct SBTLDMG_HITWORK hwk;
    struct SBTLSKILLP_TBL *tbl;
    s32 ret = 0;

    if (wklock) {
        ret = CheckLiveTarget(wklock);
        if (ret) {
            ret = sbtlCallBtlGetSkilltbl16(wklock, sklid);
            if (ret) {
                tbl = sbtlGetSkilltbl(sklid);
                if (tbl == NULL) {
                    return 0;
                }
                memset(&hwk, 0, sizeof(hwk));
                sbtlCreateHitC_None(wk_sbtl->hitList, wklock, wk, &hwk, sklid, tbl, ppos, -1);
                sbtlAddSerialID2();
                sbtlSEISHINCNTR(wklock, &hwk, 0);
                sbtlDeleteUnitList(hwk.hitlwk);
                return 1;
            }
        }
    }
    return ret;
}

/* NON_MATCHING: 768 of 833 words match.  */
#ifdef NON_MATCHING
s32 CheckEffectData_Core_sbtlEff(struct SBTLUNIT_WORK *wk, SKLID sklid, u8 type, s32 index, s32 id, u16 acterside, s32 serialID2,
                                s16 tflg, s32 sttime, s16 count)
{
    SBTLDMG_HITWORK hwk;
    struct SBTLUNIT_LISTWORK *lwk;
    struct SBTLUNIT_WORK *unit;
    struct SBTLSKILLP_TBL *skltbl;
    struct SBTLSKILLP_TBL *tbl;
    SBTLEFF_WORK *ewk;
    const sdfVector4_t *pos = NULL;
    sdatUnitWork_t *stock;
    BST badst;
    SBTLUNIT_SIDE side;
    s32 nakama;
    s32 i;
    s32 j;

    skltbl = sbtlGetSkilltbl(sklid);
    lwk = sbtlCreateUnitList(48);
    switch (tflg) {
    case 0:
        CheckEffectData_Core(lwk, wk, sklid);
        break;
    case 1:
        for (i = 0; i < wk_sbtl->unitList->number; i++) {
            unit = wk_sbtl->unitList->pUnitL[i].pUnit;
            if (unit != NULL && sbtlCheckLiveTarget_NotHitDmg(unit)) {
                sbtlEntryUnitList(lwk, unit);
            }
        }
        break;
    case 2:
        side = sbtlGetBothSide(wk, acterside);
        for (i = 0; i < wk_sbtl->unitList->number; i++) {
            unit = wk_sbtl->unitList->pUnitL[i].pUnit;
            if (unit != NULL && sbtlCheckLiveTarget_NotHitDmg(unit) && (unit->field414 & 0x3F & side)) {
                sbtlEntryUnitList(lwk, unit);
            }
        }
        break;
    case 3:
        side = sbtlGetBothSide(wk, acterside);
        for (i = 0; i < wk_sbtl->unitList->number; i++) {
            unit = wk_sbtl->unitList->pUnitL[i].pUnit;
            if (unit != NULL && sbtlCheckLiveTarget_NotHitDmg(unit) && !(unit->field414 & 0x3F & side)) {
                sbtlEntryUnitList(lwk, unit);
            }
        }
        break;
    case 4:
        sbtlEntryUnitList(lwk, wk);
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->model->field104);
        if (unit != NULL && sbtlCheckLiveTarget_NotHitDmg(unit)) {
            sbtlEntryUnitList(lwk, unit);
        }
        break;
    case 5:
        break;
    case 6:
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->model->field104);
        if (unit != NULL && sbtlCheckLiveTarget_NotHitDmg(unit)) {
            sbtlEntryUnitList(lwk, unit);
        }
        break;
    }

    for (i = 0; i < lwk->count; i++) {
        unit = lwk->pUnitL[i].pUnit;
        if (sbtlCheckEffectCancel(wk, sklid, sbtlBtlSetEffectDataSub(index, id))) {
            continue;
        }
        if (!sbtlCallBtlGetSkilltbl16(unit, sklid)) {
            continue;
        }
        ewk = sbtlCallMemAllocDirect3(wk_sbtl->effList, unit, index, id);
        pos = &ewk->pos;
        if (skltbl->flg2 & 0x10) {
            if (wk_sbtl->sword_eff_cmb != NULL) {
                ewk->rgba = swordEffClrTbl[wk_sbtl->tateClr];
            } else if (wk_sbtl->sword_eff_step != 0) {
                ewk->rgba = swordEffClrTbl[wk_sbtl->sword_eff_kind];
            } else {
            switch (sklid) {
            case 144:
            case 145:
            case 146:
            case 147:
            case 148:
            case 167:
            case 674:
                ewk->rgba = 0x80808080;
                break;
            default:
                ewk->rgba = 0x800A800A;
                break;
            }
            }
        } else {
            ewk->rgba = 0x80808080;
        }
        if (unit->vstatus & 0x4000) {
            ewk->flg |= SBTLEFF_FLG_NONSTOP;
        }
        if (sbtlCallBtlGetSkilltbl16(unit, sklid) == 2) {
            sbtlSetEffectData(ewk, unit, sklid, index, type, id, acterside, serialID2, sttime);
        } else {
            sbtlSetEffectData(ewk, unit, sklid, index, type, id, acterside, serialID2, sttime + i * 12);
        }
    }

    tbl = sbtlGetSkilltbl(sklid);
    if ((wk_sbtl->flg & 0x2000000) || type >= 2 || lwk->count <= 0 || !(tbl->area & 0x10) || count != 0) {
        goto end;
    }
    if (tbl->area & 0x4000) {
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
        if (unit == NULL || unit->status == 34) {
            goto end;
        }
        if (tflg == 0 && (tbl->area & 2)) {
            sbtlDeleteUnitList(lwk);
            lwk = sbtlCreateUnitList(48);
            for (i = 0; i < wk_sbtl->unitList->number; i++) {
                unit = wk_sbtl->unitList->pUnitL[i].pUnit;
                if (unit != NULL && CheckLiveTarget(unit) && (unit->field414 & 0x3F & acterside)) {
                    sbtlEntryUnitList(lwk, unit);
                }
            }
            for (i = 0; i < lwk->count; i++) {
                unit = lwk->pUnitL[i].pUnit;
                if (unit != NULL && CheckLiveTarget(unit) && sbtlCallBtlGetSkilltbl16(unit, sklid)) {
                    if ((unit->field414 & 1) && !(unit->unit.flag & 2)) {
                        nakama = 0;
                        for (j = 0; j < 2; j++) {
                            if (unit == sbtlGetNakama(j)) {
                                nakama = 1;
                                break;
                            }
                        }
                        if (!nakama) {
                            continue;
                        }
                    }
                    memset(&hwk, 0, sizeof(hwk));
                    sbtlCreateHitC_None(wk_sbtl->hitList, unit, wk, &hwk, sklid, tbl, pos, -1);
                    sbtlAddSerialID2();
                    sbtlSEISHINCNTR(unit, &hwk, 0);
                    sbtlDeleteUnitList(hwk.hitlwk);
                }
            }
        } else if (tflg != 0) {
            for (i = 0; i < lwk->count; i++) {
                unit = lwk->pUnitL[i].pUnit;
                if (unit != NULL && CheckLiveTarget(unit) && sbtlCallBtlGetSkilltbl16(unit, sklid)) {
                    if ((unit->field414 & 1) && !(unit->unit.flag & 2)) {
                        nakama = 0;
                        for (j = 0; j < 2; j++) {
                            if (unit == sbtlGetNakama(j)) {
                                nakama = 1;
                                break;
                            }
                        }
                        if (!nakama) {
                            continue;
                        }
                    }
                    memset(&hwk, 0, sizeof(hwk));
                    sbtlCreateHitC_None(wk_sbtl->hitList, unit, wk, &hwk, sklid, tbl, pos, -1);
                    sbtlAddSerialID2();
                    sbtlSEISHINCNTR(unit, &hwk, 0);
                    sbtlDeleteUnitList(hwk.hitlwk);
                }
            }
        } else if (wk->model->field104 >= 0) {
            if (wk->stockID >= 0 && !(wk->field414 & 2)) {
                if (!(tbl->type & 8)) {
                    /* A stocked demon's skill on its own side reaches the player's stock. */
                    stock = sbtlRefDDS3GBWK2(wk->stockID);
                    if ((tbl->area & 0x100) || sbtlCheckLive(stock)) {
                        sbtlGetDamage_GB(&wk->unit, stock, sklid);
                    }
                }
            } else {
                unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->model->field104);
                if (unit != NULL && CheckLiveTarget(unit)) {
                    memset(&hwk, 0, sizeof(hwk));
                    sbtlCreateHitC_None(wk_sbtl->hitList, unit, wk, &hwk, sklid, tbl, pos, -1);
                    sbtlAddSerialID2();
                    sbtlSEISHINCNTR(unit, &hwk, 0);
                    sbtlDeleteUnitList(hwk.hitlwk);
                }
            }
        }

        if (tbl->type & 8) {
            if (wk_sbtl->battleMode == 3) {
                goto end;
            }
            if (sbtlGetItemtbl(wk_sbtl->useitemID)->side & 4) {
                for (i = 0; i < wk_sbtl->field64C; i++) {
                    stock = sbtlRefDDS3GBWK2(i);
                    if (sbtlCheckLive(stock)) {
                        CalcItem(stock, wk_sbtl->useitemID);
                        sbtlCheckItemMember(i);
                    }
                }
                goto end;
            }
            if (wk_sbtl->useitem_stock == -1) {
                goto end;
            }
            stock = sbtlRefDDS3GBWK2(wk_sbtl->useitem_stock);
            if (!(tbl->area & 0x100)) {
                if (sbtlCheckLive(stock)) {
                    CalcItem(stock, wk_sbtl->useitemID);
                    sbtlCheckItemMember(wk_sbtl->useitem_stock);
                }
            } else {
                sbtlCheckItemMember(wk_sbtl->useitem_stock);
                CalcItem(stock, wk_sbtl->useitemID);
                sbtlCheckItemMember_Dead(wk_sbtl->useitem_stock);
            }
            wk_sbtl->useitem_stock = -1;
        } else if (((tbl->area & 2) || (tbl->area & 4)) && (wk->field414 & 1) && wk_sbtl->battleMode != 3) {
            badst = sbtlGetBadStatus(&wk->unit);
            if (badst == BST_BOUDO || badst == BST_MAHUU) {
                goto end;
            }
            for (i = 0; i < wk_sbtl->field64C; i++) {
                if (wk_sbtl->member_num > 0 && wk_sbtl->summonIndex[0] == i) {
                    continue;
                }
                stock = sbtlRefDDS3GBWK2(i);
                if (!(tbl->area & 0x100)) {
                    if (sbtlCheckLive(stock)) {
                        sbtlGetDamage_GB(&wk->unit, stock, sklid);
                    }
                } else {
                    sbtlGetDamage_GB(&wk->unit, stock, sklid);
                }
            }
        }
    } else if (tflg == 0 && (tbl->area & 2)) {
        sbtlDeleteUnitList(lwk);
        lwk = sbtlCreateUnitList(48);
        for (i = 0; i < wk_sbtl->unitList->number; i++) {
            unit = wk_sbtl->unitList->pUnitL[i].pUnit;
            if (unit != NULL && CheckLiveTarget(unit) && !(unit->field414 & 0x3F & acterside)) {
                sbtlEntryUnitList(lwk, unit);
            }
        }
        for (i = 0; i < lwk->count; i++) {
            SetDamageProc(lwk->pUnitL[i].pUnit, wk, sklid, pos);
        }
    } else if (tflg == 0) {
        if (wk->model->field104 != -1) {
            SetDamageProc(sbtlSearchUnit_ID(wk_sbtl->unitList, wk->model->field104), wk, sklid, pos);
        }
    } else {
        for (i = 0; i < lwk->count; i++) {
            SetDamageProc(lwk->pUnitL[i].pUnit, wk, sklid, pos);
        }
    }
end:
    sbtlDeleteUnitList(lwk);
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlEff_00289E50", CheckEffectData_Core_sbtlEff);
#endif

void sbtlSetEffectData(SBTLEFF_WORK *ewk, struct SBTLUNIT_WORK *wk, SKLID sklid, s32 workID, u8 type, s32 no,
                                   u16 acterside, s32 serialID2, s32 sttime)
{
    EFFPDATA_WORK *data;
    SBTLEFF_PACDATAWORK *pac;
    struct SBTLSKILLP_TBL *tbl;
    struct SBTL_HIT_PACDATA **hit;
    s32 hitmode;
    s32 i;

    ewk->BEDtype = type;
    ewk->BEDno = no;
    ewk->BEDsklid = sklid;
    ewk->BEDacterside = wk->field414;
    ewk->workID = workID;
    ewk->acterID = wk->field1A0;
    ewk->targetID = wk->model->field104;
    data = sbtlBtlSetEffectDataSub(workID, no);
    pac = sbtlGetEffectPacData(ewk->workID, ewk->BEDno);
    if (!CheckEffectDataSingle(data->type, type)) {
        return;
    }
    ewk->alpha = 128;
    ewk->life = data->life;
    ewk->starttime = data->starttime + sttime;
    if (data->fadeout_time == 0.0f) {
        ewk->alpha_cnt = 128;
    } else {
        ewk->alpha_cnt = 128.0f / data->fadeout_time;
    }
    if (data->scale == 0.0f) {
        ewk->scale = 1.0f;
    } else {
        ewk->scale = GetDataScale(wk, sklid);
    }
    ewk->flg |= data->flg;
    if (!SetEffectPos(ewk, wk)) {
        ewk->flg |= SBTLEFF_FLG_FORCERESET;
    }
    hitmode = 0;
    if (pac != NULL) {
        hitmode = pac->flg == 1;
        if (pac->flg == 0) {
            hitmode = 2;
        }
    }
    tbl = sbtlGetSkilltbl(sklid);
    if ((tbl->type & 0xFFF) == 2 || (tbl->type & 0xFFF) == 14) {
        if (tbl->area & 0x10) {
            hitmode = 2;
        }
    }
    if ((tbl->flg2 & 0x1000) && wk_sbtl->field4264) {
        return;
    }
    if (hitmode == 1) {
        if (pac->hitnum > 0) {
            hit = pac->hit;
            i = pac->hitnum;
            do {
                ewk->hitserialID = sbtlCallMemset6(wk_sbtl->hitList, wk, ewk, wk->field1A0, sklid, tbl, &(*hit)->entry,
                                                   *hit, acterside, serialID2);
                hit++;
            } while (--i != 0);
        }
    }
}

f32 GetDataScale(struct SBTLUNIT_WORK *wk, SKLID sklid) {
    struct SBTLSKILLP_TBL *tbl;
    SBTLVISUAL_DATA base;
    SBTLVISUAL_DATA visual;
    f32 w;
    f32 h;
    f32 max;

    tbl = sbtlGetSkilltbl(sklid);
    if (wk->unit.devilId == 628 && (tbl->flg & 0x100)) {
        return 4.0f;
    }
    max = 1.5f;
    sbtlGetCylinderData_ID(0, 2, &base);
    sbtlGetCylinderData(wk, &visual);
    h = visual.height / base.height;
    w = visual.width / base.width;
    if (w <= h) {
        if (h >= max) {
            h = max;
        }
        w = h;
    } else {
        if (w >= max) {
            w = max;
        }
    }
    if (w <= 0.5f) {
        w = 0.5f;
    }
    return w;
}

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

s32 sbtlListProc(SBTLEFF_LISTWORK *lwk) {
    SBTLEFF_WORK *ewk;
    struct SBTLSKLVISUAL_TBL *vtbl;
    struct SBTLUNIT_WORK *unit;
    s32 stop;

    stop = 0;
    if (IsSceanStop()) {
        stop = wk_sbtl->effNoStop == 0;
    }
    if (wk_sbtl->effList == NULL) {
        return 0;
    }
    ewk = lwk->wk;
    if (lwk != NULL) {
        effPadVibeFadeUpdate();
    }
    if (ewk == NULL || lwk->num == 0) {
        return 0;
    }
    do {
        if (stop && sbtlBtlListProcSub(ewk) && !(wk_sbtl->scrFlag & 0x80000)) {
            goto next;
        }
        if (ewk->bTateBg) {
            EffFunc_TateBg(ewk);
        } else if (sbtlCallIsSceanStop2(ewk)) {
            ewk->counter++;
            ewk->counter = (ewk->counter <= 0) ? 0 : (ewk->counter > 0x7FFF) ? 0x7FFF : ewk->counter;
            if (sbtlBtlListProcSub(ewk)) {
                EffFunc_Hanyou_BED(ewk);
            } else {
                sbtlListProcData[ewk->effectid](ewk);
            }
        } else if (sbtlBtlListProcSub(ewk)) {
            if ((sbtlGetSkilltbl(ewk->BEDsklid)->flg & 0x100) || ewk->st != SBTLEFF_ST_PROC) {
                EffFunc_Hanyou_BED(ewk);
            }
        } else if (ewk->st != SBTLEFF_ST_PROC) {
            if (sbtlBtlListProcSub(ewk)) {
                EffFunc_Hanyou_BED(ewk);
            } else {
                sbtlListProcData[ewk->effectid](ewk);
            }
        }
        if (ewk->st == SBTLEFF_ST_PROC && ewk->BEDsklid != 0) {
            vtbl = sbtlGetSklVisualTbl(ewk->BEDsklid);
            if (vtbl != NULL && (vtbl->flg & 0x100)) {
                unit = sbtlSearchUnit_ID(wk_sbtl->unitList, ewk->acterID);
                if (unit != NULL) {
                    SDF_VU0_LOADVECTOR(&unit->pos);
                    SDF_VU0_STOREVECTOR(&ewk->pos);
                    effPartsPositionSet(ewk->dwk.hPartsList, (const f32 *)&ewk->pos);
                    SDF_VU0_LOADVECTOR(&unit->rot);
                    SDF_VU0_STOREVECTOR_NOMEM(&ewk->rot);
                    effPartsRotateSet(ewk->dwk.hPartsList, (const f32 *)&ewk->rot);
                }
            }
        }
    next:
        ewk = ewk->next;
    } while (ewk != NULL);
    return 1;
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", effClassifyResourceMask);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallBtlGetSkilltbl10);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlLeaf30);

s32 sbtlCallBtlGetCutFlg(void)
{
    SBTLEFF_LISTWORK *lwk;
    SBTLEFF_WORK *ewk;
    struct SBTLUNIT_WORK *target;
    struct SBTLUNIT_WORK *acter;
    struct SBTLSKILLP_TBL *tbl;
    E_CAMMOD_CUT cut;
    s32 stop;
    s32 flag;

    stop = 0;
    cut = sbtlGetCutFlg();
    if (IsSceanStop()) {
        flag = wk_sbtl->scrFlag & 0x80000;
        stop = flag == 0;
    }
    lwk = wk_sbtl->effList;
    if (lwk == NULL) {
        return 0;
    }
    if (wk_sbtl->unitList == NULL) {
        return 0;
    }
    ewk = lwk->wk;
    if (ewk == NULL || lwk->num == 0) {
        return 0;
    }
    eglSetOt(40);
    eglLinkTest(1, 0, 128, 1, 0, 0, 1, 2);
    eglLinkAlpha(0);
    if (stop) {
        effCallStopStat();
    }
    do {
        if ((cut & E_CAMMOD_CUT_ExcRenkeiTameEff) && !sbtlLeaf30(ewk->BEDsklid)) {
        } else if (ewk->st >= SBTLEFF_ST_EXIT) {
        } else if (ewk->st >= SBTLEFF_ST_PROC) {
            sbtlEffSetActer(ewk->acterID);
            if (sbtlSearchUnit_ID(wk_sbtl->unitList, ewk->acterID) != NULL) {
                target = sbtlSearchUnit_ID(wk_sbtl->unitList, ewk->targetID);
                if (target != NULL) {
                    if (ewk->BEDtype == SBTLEFF_BEDTYPE_HUKA || ewk->BEDtype == SBTLEFF_BEDTYPE_HUKA2) {
                        sbtlEffSetTarget(ewk->acterID);
                    } else {
                        sbtlEffSetTarget(target->field1A0);
                    }
                }
            }
            if (ewk->dwk.hPartsList != NULL) {
                if (sbtlBtlListProcSub(ewk)) {
                    sbtlCallSetOtData = 0;
                } else if (ewk->effectid == 50 || ewk->effectid == 1 || ewk->effectid == 2 || ewk->effectid == 10
                           || ewk->effectid == 11 || ewk->effectid == 13 || ewk->effectid == 6) {
                    sbtlCallSetOtData = 1;
                } else {
                    sbtlCallSetOtData = 0;
                }
                if (!sbtlBtlListProcSub(ewk) || !(sbtlGetSkilltbl(ewk->BEDsklid)->flg & 0x100)
                    || (acter = sbtlSearchUnit_ID(wk_sbtl->unitList, ewk->acterID)) == NULL
                    || (acter->mode >= 5 && !(acter->vstatus & 0x1000))) {
                    if (wk_sbtl->bEffNoDraw != 0) {
                        tbl = sbtlGetSkilltbl(ewk->BEDsklid);
                        if ((tbl->flg2 & 0x80) || (tbl->type & 0x1000)) {
                            effPartsDraw(ewk->dwk.hPartsList);
                        }
                    } else {
                        effPartsDraw(ewk->dwk.hPartsList);
                    }
                }
            }
        }
        ewk = ewk->next;
    } while (ewk != NULL);
    if (stop) {
        effStopEffectStop();
    }
    return 1;
}

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

/* NON_MATCHING: 107 of 112 words match.  */
#ifdef NON_MATCHING
void sbtlForceDeleteAll(s32 flag)
{
    SBTLEFF_LISTWORK *lwk = wk_sbtl->effList;
    SBTLEFF_WORK *wk = lwk->wk;
    struct SBTLSKILLP_TBL *tbl;
    s32 i;
    u32 flg;

    if (lwk->num == 0 || wk == NULL || (u32)flag > 3) {
        return;
    }
    do {
        if (flag == 1 && wk->BEDsklid == SKLID_MAG_HOUSHUTU_1) {
            goto next;
        }
        tbl = sbtlGetSkilltbl(wk->BEDsklid);
        for (i = 0; i < 15; i++) {
            if (wk->effectid == sbtlTable2[i]) {
                goto next;
            }
        }
        if (flag != 2 && wk->effectid < 79 && wk->effectid >= 73) {
            goto next;
        }
        if (flag == 1 || flag == 3) {
            if (wk->BEDtype == 2) {
                wk->st = SBTLEFF_ST_EXIT;
                wk->alpha = 0;
            }
            if ((tbl->flg & 0x100) && wk->BEDsklid != SKLID_BST_MOUDOKU) {
                wk->st = SBTLEFF_ST_EXIT;
                wk->alpha = 0;
            }
            sbtlCheckDeleteEffect(wk);
        }
        if (wk->BEDtype != 2 && wk->BEDtype != 5 && !(tbl->flg & 0x100) && sbtlCheckDeleteEffect(wk)) {
            flg = wk->flg;
            if ((u32)(wk->st - SBTLEFF_ST_INIT) < 2) {
                flg |= SBTLEFF_FLG_SESTOP;
                flg |= SBTLEFF_FLG_FORCERESET;
            } else {
                wk->st = SBTLEFF_ST_EXIT;
                flg |= SBTLEFF_FLG_SESTOP;
                wk->alpha = 0;
            }
            wk->flg = flg;
        }
    next:
        wk = wk->next;
    } while (wk != NULL);
}
#else
INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceDeleteAll);
#endif

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

/* NON_MATCHING: 30 of 137 words match.  */
#ifdef NON_MATCHING
s32 sbtlDeleteHaveSkill_EffectWork(struct SBTLUNIT_WORK *wk) {
    struct SBTLUNIT_WORK *unit;
    s32 sklid[4];
    s32 skl;
    s32 i;
    s32 j;
    s32 k;
    s32 l;
    s32 found;

    for (i = 0; i < 19; i++) {
        if (sbtlGetSkilltbl(wk->haveskill[i])->flg2 & 0x1000) {
            continue;
        }
        if (wk->haveskill[i] == SKLID_ATK || wk->haveskill[i] == 0 || wk->haveskill[i] == SKLID_OFS_PROGRAM) {
            continue;
        }
        sklid[0] = sbtlCallBtlGetSkilltbl23(wk->haveskill[i], &sklid[1]);
        for (j = 0; j < 2 && sklid[j] != 0; j++) {
            found = 0;
            for (k = 0; k < wk_sbtl->unitList->number; k++) {
                unit = wk_sbtl->unitList->pUnitL[k].pUnit;
                if (unit != NULL && unit->field1A0 != wk->field1A0) {
                    for (l = 0; l < 19; l++) {
                        skl = unit->haveskill[l];
                        if (sbtlGetSkilltbl(skl)->flg2 & 1) {
                            skl = sbtlCallBtlGetSkilltbl23(skl, NULL);
                        }
                        if (skl == sklid[j]) {
                            found = 1;
                            goto check;
                        }
                    }
                }
            }
        check:
            if (!found) {
                if (wk->field414 & 2) {
                    sbtlCreateDelWork(wk_sbtl->effdelList, sklid[j], wk->field414, wk->GroupID);
                } else {
                    sbtlCreateDelWork(wk_sbtl->effdelList, sklid[j], wk->field414, -1);
                }
            }
        }
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlDeleteHaveSkill_EffectWork);
#endif

/* NON_MATCHING: 124 of 175 words match.  */
#ifdef NON_MATCHING
s32 sbtlForceDelete_EffectWork_ID(SKLID id, s8 flg)
{
    SBTLEFF_PACLIST *pac;
    s32 no;
    s32 i;
    s32 j;

    no = sbtlSearchEffectData(id);
    if (no == -1) {
        return 1;
    }
    pac = &sbtlData83->effpac[no];
    if (pac->status & 0x20) {
        return 1;
    }
    if ((pac->status & 4) && flg != 1) {
        return 1;
    }
    if (pac->status & 1) {
        sbtlBtlForceDeleteEffectWorkIDSub(pac->SMGID);
        pac->SMGID = 0;
        for (i = 0; i < pac->num; i++) {
            if (pac->pwk[i]->hPartsList != NULL) {
                effPartsDelete(pac->pwk[i]->hPartsList);
                pac->pwk[i]->hPartsList = NULL;
            }
            if (pac->pwk[i]->flg) {
                for (j = 0; j < pac->pwk[i]->hitnum; j++) {
                    if (pac->pwk[i]->hit[j]->handle != NULL) {
                        if (pac->pwk[i]->hit[j]->handle08 != NULL) {
                            sdfMemFreeDirect(pac->pwk[i]->hit[j]->handle08);
                            pac->pwk[i]->hit[j]->BpData = NULL;
                            pac->pwk[i]->hit[j]->handle08 = NULL;
                        }
                        if (pac->pwk[i]->hit[j]->handle10 != NULL) {
                            if (pac->pwk[i]->hit[j]->hPartsList != NULL) {
                                effPartsDelete(pac->pwk[i]->hit[j]->hPartsList);
                                pac->pwk[i]->hit[j]->hPartsList = NULL;
                            }
                            sdfMemFreeDirect(pac->pwk[i]->hit[j]->handle10);
                            pac->pwk[i]->hit[j]->EpData = NULL;
                            pac->pwk[i]->hit[j]->handle10 = NULL;
                        }
                        sdfMemFreeDirect(pac->pwk[i]->hit[j]->handle);
                        pac->pwk[i]->hit[j]->handle = NULL;
                    }
                }
            }
            if (pac->pwk[i]->handle != NULL) {
                sdfMemFreeDirect(pac->pwk[i]->handle);
                pac->pwk[i]->handle = NULL;
            }
            sdfMemFreeDirect(pac->pwk[i]->selfHandle);
            pac->pwk[i]->selfHandle = NULL;
            pac->pwk[i]->pData = NULL;
        }
        if (sbtlData83->effpac[no].handle != NULL) {
            sdfMemFreeDirect(sbtlData83->effpac[no].handle);
        }
        sbtlBtlForceDeleteEffectWorkIDSub2(no);
        sbtlData83->effpaccnt--;
        sbtlData83->effpaccnt = sbtlData83->effpaccnt <= 0                    ? 0
                                : sbtlData83->effpaccnt >= SBTLEFF_PACLIST_MAX ? SBTLEFF_PACLIST_MAX - 1
                                                                               : sbtlData83->effpaccnt;
    } else if (pac->gstat != 0) {
        return 0;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceDelete_EffectWork_ID);
#endif

/* NON_MATCHING: 89 of 187 words match.  */
#ifdef NON_MATCHING
s32 sbtlForceDelete_EffectWork(s8 flg)
{
    SBTLEFF_PACLIST *pac;
    s32 i;
    s32 j;
    s32 k;

    for (i = 0; i < SBTLEFF_PACLIST_MAX; i++) {
        if (flg != 2) {
            if (sbtlData83->effpac[i].status & 0x20) {
                continue;
            }
            if ((sbtlData83->effpac[i].status & 4) && flg != 1) {
                continue;
            }
        }
        pac = &sbtlData83->effpac[i];
        if (pac->status & 1) {
            sbtlBtlForceDeleteEffectWorkIDSub(pac->SMGID);
            pac->SMGID = 0;
            for (j = 0; j < pac->num; j++) {
                if (pac->pwk[j]->hPartsList != NULL) {
                    effPartsDelete(pac->pwk[j]->hPartsList);
                    pac->pwk[j]->hPartsList = NULL;
                }
                if (pac->pwk[j]->flg) {
                    for (k = 0; k < pac->pwk[j]->hitnum; k++) {
                        if (pac->pwk[j]->hit[k]->handle != NULL) {
                            if (pac->pwk[j]->hit[k]->handle08 != NULL) {
                                sdfMemFreeDirect(pac->pwk[j]->hit[k]->handle08);
                                pac->pwk[j]->hit[k]->BpData = NULL;
                                pac->pwk[j]->hit[k]->handle08 = NULL;
                            }
                            if (pac->pwk[j]->hit[k]->handle10 != NULL) {
                                if (pac->pwk[j]->hit[k]->hPartsList != NULL) {
                                    effPartsDelete(pac->pwk[j]->hit[k]->hPartsList);
                                    pac->pwk[j]->hit[k]->hPartsList = NULL;
                                }
                                sdfMemFreeDirect(pac->pwk[j]->hit[k]->handle10);
                                pac->pwk[j]->hit[k]->EpData = NULL;
                                pac->pwk[j]->hit[k]->handle10 = NULL;
                            }
                            sdfMemFreeDirect(pac->pwk[j]->hit[k]->handle);
                            pac->pwk[j]->hit[k]->handle = NULL;
                        }
                    }
                }
                if (pac->pwk[j]->handle != NULL) {
                    sdfMemFreeDirect(pac->pwk[j]->handle);
                    pac->pwk[j]->handle = NULL;
                }
                sdfMemFreeDirect(pac->pwk[j]->selfHandle);
                pac->pwk[j]->selfHandle = NULL;
                pac->pwk[j]->pData = NULL;
            }
            if (sbtlData83->effpac[i].handle != NULL) {
                sdfMemFreeDirect(sbtlData83->effpac[i].handle);
            }
            sbtlBtlForceDeleteEffectWorkIDSub2(i);
            sbtlData83->effpaccnt--;
            sbtlData83->effpaccnt = sbtlData83->effpaccnt <= 0                    ? 0
                                    : sbtlData83->effpaccnt >= SBTLEFF_PACLIST_MAX ? SBTLEFF_PACLIST_MAX - 1
                                                                                   : sbtlData83->effpaccnt;
        } else if (pac->gstat != 0) {
            return 0;
        }
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlForceDelete_EffectWork);
#endif

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

/* NON_MATCHING: 11 of 92 words match.  */
#ifdef NON_MATCHING
s32 sbtlLoadEffectData(struct SBTLUNIT_WORK *wk) {
    struct SBTLSKILLP_TBL *tbl;
    s32 sklid[2];
    s32 *p;
    s32 id;
    s32 i;
    s32 *have;

    have = &wk->haveskill[2];
    for (i = 0; i < 16; i++, have++) {
        sklid[0] = *have;
        if (sklid[0] == 0) {
            continue;
        }
        tbl = sbtlGetSkilltbl(sklid[0]);
        if (tbl->flg2 & 0x1000) {
            continue;
        }
        if ((tbl->type & 0x2000) && sklid[0] != SKLID_OFS_AUTO) {
            continue;
        }
        sklid[0] = sbtlCallBtlGetSkilltbl23(sklid[0], &sklid[1]);
        if (sklid[0] == 0) {
            continue;
        }
        p = sklid;
        do {
            if (sbtlGetExeSkill(*p) != -1) {
                id = sbtlSearchEffectData(*p);
                if (id != -1) {
                    if (!CheckLoadEffectData(&sbtlData83->effpac[id], *p, 0, NULL)) {
                        return 0;
                    }
                } else {
                    id = sbtlBtlLoadEffectDataIDMaskSub(*p);
                    if (id != -1 && !CheckLoadEffectData(&sbtlData83->effpac[id], *p, 0, NULL)) {
                        return 0;
                    }
                }
            }
            p++;
        } while (p < &sklid[2] && *p != 0);
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlLoadEffectData);
#endif

/* NON_MATCHING: 150 of 553 words match.  */
#ifdef NON_MATCHING
s32 CheckLoadEffectData(SBTLEFF_PACLIST *pac, SKLID skillid, u16 pacflg, void *ppData)
{
    s32 size[SBTLEFF_PACDATA_MAX] = { 0 };
    s32 hitidx[SBTLEFF_PACDATA_MAX];
    char filename[128];
    s32 ready;
    s32 hitcount;
    s32 hitnum;
    s32 *p;
    u8 *src;
    s32 i;
    s32 j;
    s32 k;
    s32 sz;
    SBTLEFF_PACDATAWORK *pwk;
    sdfMemHandle_t *handle;
    void *data;
    s16 cnt;

    if (pac->status & EFFPAC_ST_LOADEND) {
        return 1;
    }
    if (ppData != NULL && pac->gstat == 0) {
        pac->skillID = skillid;
        pac->gstat = 1;
        wk_sbtl->flg |= 0x20000;
    }
    switch (pac->gstat) {
    case 0:
        if (ppData == NULL) {
            sprintf(filename, "/sobed_lb/%03X.LB", skillid);
            pac->pacHandle = pacLoad(filename);
        }
        pac->skillID = skillid;
        pac->gstat++;
        wk_sbtl->flg |= 0x20000;
        return 0;
    case 1:
        break;
    case 2:
        goto load_se;
    case 5:
        pac->status |= EFFPAC_ST_LOADEND;
        return 0;
    default:
        return 0;
    }

    ready = 0;
    if (ppData != NULL) {
        ready = 1;
    } else if (pac->pData == NULL) {
        ready = pacCheckEnd(pac->pacHandle) != 0;
    }
    if (!ready) {
        return 0;
    }
    if (ppData != NULL) {
        pac->pData = ppData;
        pac->handle = NULL;
    } else {
        pac->handle = pac->pacHandle->pac.resrcHead->resrc;
        pac->pData = sdfMemLock(pac->handle);
        pacFileHandleFree_(pac->pacHandle);
        pac->pacHandle = NULL;
    }
    pac->file = NULL;

    p = pac->pData;
    pac->num = *p++;
    pac->gstat++;
    hitcount = *p++;
    for (i = 0; i < hitcount; i++) {
        hitidx[i] = *p++;
    }
    for (i = 0; i < pac->num; i++) {
        size[i] = *p++;
    }
    for (i = 0; i < pac->num; i++) {
        handle = sdfMemAllocDirect(sizeof(SBTLEFF_PACDATAWORK));
        pwk = sdfMemLock(handle);
        memset(pwk, 0, sizeof(SBTLEFF_PACDATAWORK));
        pwk->selfHandle = handle;
        pac->pwk[i] = pwk;
    }
    for (i = 0; i < pac->num; i++) {
        pac->pwk[i]->wk = *(EFFPDATA_WORK *)p;
        p += sizeof(EFFPDATA_WORK) / sizeof(s32);
    }

    /* Effects that share an EPL with an earlier entry copy its data. */
    src = (u8 *)p;
    for (i = 0; i < pac->num; i++) {
        if (pac->pwk[i]->wk.eplno == i) {
            handle = sdfMemAllocDirect(size[i]);
            data = sdfMemLock(handle);
            pac->pwk[i]->flg = 0;
            pac->pwk[i]->handle = handle;
            pac->pwk[i]->pData = data;
            memcpy(data, src, size[i]);
        } else {
            handle = sdfMemAllocDirect(size[pac->pwk[i]->wk.eplno]);
            data = sdfMemLock(handle);
            pwk = pac->pwk[i];
            pwk->pData = data;
            pwk->flg = 0;
            pwk->handle = handle;
            memcpy(data, pac->pwk[pwk->wk.eplno]->pData, size[pwk->wk.eplno]);
        }
        pac->pwk[i]->hitnum = 0;
        if (pacflg & EFFPAC_ST_DECODE) {
            pac->pwk[i]->hPartsList = Create_effPartsList(pac->pwk[i]->pData);
            pac->status |= EFFPAC_ST_DECON;
        }
        src += size[i];
    }
    for (i = 0; i < pac->num; i++) {
        if (pacflg & EFFPAC_ST_DECODE) {
            sdfMemFreeDirect(pac->pwk[i]->handle);
            pac->pwk[i]->pData = NULL;
            pac->pwk[i]->handle = NULL;
        }
    }

    /* Hit records: per listed effect, a count then each SBTLSKILLHIT_TBL with its optional data. */
    p = (s32 *)src;
    if (hitcount != 0) {
        for (j = 0; j < hitcount; j++) {
            hitnum = *p++;
            pac->pwk[hitidx[j]]->hitnum = hitnum;
            for (k = 0; k < hitnum; k++) {
                SBTL_HIT_PACDATA *hit;

                handle = sdfMemAllocDirect(sizeof(SBTL_HIT_PACDATA));
                pac->pwk[hitidx[j]]->flg = 1;
                hit = sdfMemLock(handle);
                hit->handle = handle;
                pac->pwk[hitidx[j]]->hit[k] = hit;
                hit->entry = *(SBTLSKILLHIT_TBL *)p;
                p += sizeof(SBTLSKILLHIT_TBL) / sizeof(s32);
                if (pac->pwk[hitidx[j]]->hit[k]->entry.flg & 0x1000) {
                    pac->pwk[hitidx[j]]->hit[k]->BpSize = *p;
                    handle = sdfMemAllocDirect(pac->pwk[hitidx[j]]->hit[k]->BpSize);
                    data = sdfMemLock(handle);
                    pac->pwk[hitidx[j]]->hit[k]->BpData = data;
                    memcpy(data, p + 1, pac->pwk[hitidx[j]]->hit[k]->BpSize);
                    pac->pwk[hitidx[j]]->hit[k]->handle08 = handle;
                    p = (s32 *)((u8 *)p + pac->pwk[hitidx[j]]->hit[k]->BpSize + sizeof(s32));
                } else {
                    pac->pwk[hitidx[j]]->hit[k]->handle08 = NULL;
                }
                if (pac->pwk[hitidx[j]]->hit[k]->entry.flg & 0x2000) {
                    sz = *p;
                    src = (u8 *)(p + 1);
                    p = (s32 *)(src + sz + sizeof(SBTLEFF_EFFTIME));
                    handle = sdfMemAllocDirect(sz);
                    data = sdfMemLock(handle);
                    pac->pwk[hitidx[j]]->hit[k]->hPartsList = NULL;
                    pac->pwk[hitidx[j]]->hit[k]->EpData = data;
                    memcpy(data, src, sz);
                    pac->pwk[hitidx[j]]->hit[k]->handle10 = handle;
                    pac->pwk[hitidx[j]]->hit[k]->time = *(SBTLEFF_EFFTIME *)(src + sz);
                    if (sz == 0) {
                        pac->pwk[hitidx[j]]->hit[k]->entry.flg &= ~0x2000;
                    }
                } else {
                    pac->pwk[hitidx[j]]->hit[k]->handle10 = NULL;
                }
            }
        }
    }

    if (pac->handle != NULL) {
        sdfMemFreeDirect(pac->handle);
    }
    pac->handle = NULL;
    pac->pData = NULL;
    cnt = sbtlData83->effpaccnt + 1;
    sbtlData83->effpaccnt = cnt;
    if (cnt <= 0) {
        cnt = 0;
    } else if (cnt >= SBTLEFF_PACLIST_MAX) {
        cnt = SBTLEFF_PACLIST_MAX - 1;
    }
    sbtlData83->effpaccnt = cnt;
    pac->gstat = 2;
    wk_sbtl->flg &= ~0x20000;
load_se:
    if (sbtlLoadSkillSE(pac)) {
        pac->gstat = 5;
        pac->status |= EFFPAC_ST_LOADEND;
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "sbtlEff_00289E50", CheckLoadEffectData);
#endif

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

s32 sbtlCheckYukaDamageEffect(struct SBTLUNIT_WORK *wk, SKLID sklid, u16 htype, s32 time, s8 type)
{
    EFFID dmgEff[5] = { 9, 0x21, 15, 15, 15 };
    EFFID yukaEff[5] = { 0x32, 1, 2, 2, 2 };
    EFFID dmgEffE[5] = { 0x13, 0x10, 0x11, 0x11, 0x11 };
    EFFID yukaEffE[5] = { 10, 11, 13, 13, 13 };
    SBTLEFFDMG_TBL *tbl;
    u32 rgba;
    s8 flg;
    s32 r;
    s32 g;
    s32 b;

    flg = 0;
    if (sbtlGetSklVisualTbl(sklid)->flg & 0x80) {
        flg = 1;
    } else {
        switch (htype & 0xFFF) {
        case 1:
        case 4:
        case 12:
        case 13:
        case 14:
            flg = 1;
            break;
        }
    }
    if (flg) {
        tbl = LoadOnMemoryData(wk->major, wk->minor);
        if (tbl->flg & 1) {
            rgba = 0x80808080;
            flg = 0;
            if (tbl->flg & 4) {
                flg = 1;
                r = tbl->dr;
                g = tbl->dg;
                b = tbl->db;
                rgba = (b << 16) | r | (g << 8) | 0x80000000;
            }
            if (type == 0) {
                sbtlCreateEff(wk_sbtl->effList, wk, dmgEff[tbl->d_eplno], time, &wk->pos, rgba, flg);
            } else {
                sbtlCreateEff(wk_sbtl->effList, wk, dmgEffE[tbl->d_eplno], time, &wk->pos, rgba, flg);
            }
        }
        if (tbl->flg & 2) {
            rgba = 0x80808080;
            flg = 0;
            if (tbl->flg & 8) {
                flg = 1;
                r = tbl->yr;
                g = tbl->yg;
                b = tbl->yb;
                rgba = (b << 16) | r | (g << 8) | 0x80000000;
            }
            if (type == 0) {
                sbtlCreateEff(wk_sbtl->effList, wk, yukaEff[tbl->y_eplno], time, &wk->pos, rgba, flg);
            } else {
                sbtlCreateEff(wk_sbtl->effList, wk, yukaEffE[tbl->y_eplno], time, &wk->pos, rgba, flg);
            }
        }
    }
    return 1;
}

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

void LoadOnMemoryData_sbtlEff(SBTLEFF_WORK *wk, struct SBTLUNIT_WORK *wklock, u16 no)
{
    SBTLDMG_HITWORK *hit;
    s32 onmem;
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
            if (wk->bTateBg) {
                sbtlCallPartsCreateFinish2(wk);
            } else {
                onmem = LoadOnMemoryData_sbtlEffData2[wk->effectid];
                if (sbtl_effonfilecheck[onmem] == 2) {
                    wk->dwk.hPartsList = effPartsCreateFinish(btlLoadSbPackData[onmem].unkC);
                } else {
                    wk->dwk.hPartsList = Create_effPartsList(btlLoadSbPackData[onmem].data);
                }
            }
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
            effPartsRotateSet(wk->dwk.hPartsList, (const f32 *)&wk->rot);
            effPartsScaleSet(wk->dwk.hPartsList, wk->scale);
            flg = wk->flg;
        }
        if (flg & SBTLEFF_FLG_RGBASET) {
            ColorSet(wk->dwk.hPartsList, wk->rgba);
        }
        wk->st = SBTLEFF_ST_PROC;
        wk->cmpcounter = wk->life;
        wk->counter = 0;
        return;
    }
    wk->st = SBTLEFF_ST_EXIT;
}

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

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallBtlSearchUnitID9);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallBtlSearchUnitID10);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallBtlSearchUnitID11);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", EffFunc_Hanyou_BED);

s32 EffFunc_NMLDAMAGE(SBTLEFF_WORK *wk)
{
    struct SBTLUNIT_WORK *unit;

    switch (wk->st) {
    case SBTLEFF_ST_INIT:
        if (wk_sbtl->flg & 4) {
            wk->st = SBTLEFF_ST_LOAD;
        }
        break;
    case SBTLEFF_ST_LOAD:
        LoadOnMemoryData_sbtlEff(wk, NULL, 0);
        if (wk->st != SBTLEFF_ST_PROC) {
            break;
        }
        switch (wk->effectid) {
        case 9:
        case 15:
        case 33:
            unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->acterID);
            if (unit != NULL && (unit->field414 & 1)) {
                sbtlGetUnitHelperPos(unit, 0);
                SDF_VU0_STOREVECTOR(&wk->pos);
                if (wk->pos.y >= -1.0) {
                    wk->pos.y -= 50.0f;
                }
                SDF_VU0_LOADVECTOR(&aViewEye);
                SDF_VU0_LOADVECTOR_B(&wk->pos);
                SDF_VU0_SUBVECTOR();
                SDF_VU0_NORMALIZE();
                SDF_VU0_SCALEVECTOR(15.0f);
                SDF_VU0_LOADVECTOR_B(&wk->pos);
                SDF_VU0_ADDVECTOR();
                SDF_VU0_STOREVECTOR(&wk->pos);
                effPartsPositionSet(wk->dwk.hPartsList, (const f32 *)&wk->pos);
            }
            break;
        }
        break;
    case SBTLEFF_ST_PROC:
        if (wk->counter >= wk->cmpcounter) {
            wk->counter = 0;
            wk->st = SBTLEFF_ST_END;
        } else {
            effPartsScaleSet(wk->dwk.hPartsList, wk->scale);
        }
        break;
    case SBTLEFF_ST_END:
        wk->st = SBTLEFF_ST_EXIT;
        break;
    }
    return 1;
}

s32 EffFunc_DEADDAMAGE(SBTLEFF_WORK *wk)
{
    struct SBTLUNIT_WORK *unit;

    switch (wk->st) {
    case SBTLEFF_ST_INIT:
        if (wk_sbtl->flg & 4) {
            wk->st = SBTLEFF_ST_LOAD;
        }
        return 1;
    case SBTLEFF_ST_LOAD:
        LoadOnMemoryData_sbtlEff(wk, NULL, 0);
        if (wk->st == SBTLEFF_ST_PROC && (wk->flg & SBTLEFF_FLG_RGBASET)) {
            ColorSet(wk->dwk.hPartsList, wk->rgba);
        }
        return 1;
    case SBTLEFF_ST_PROC:
        switch (wk->effectid) {
        case 1:
        case 2:
        case 6:
        case 10:
        case 11:
        case 13:
        case 50:
            if (wk->counter >= wk->cmpcounter) {
                wk->counter = 0;
                wk->st = SBTLEFF_ST_END;
            }
            break;
        default:
            unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->acterID);
            if (wk->counter >= wk->cmpcounter || unit == NULL) {
                wk->counter = 0;
                wk->st = SBTLEFF_ST_END;
                break;
            }
            switch (wk->effectid) {
            case 16:
            case 17:
            case 19:
                effPartsRotateSet(wk->dwk.hPartsList, (const f32 *)&wk->rot);
                break;
            }
            sbtlGetUnitHelperPos(unit, 0);
            SDF_VU0_STOREVECTOR(&wk->pos);
            if (wk->pos.y >= -1.0) {
                wk->pos.y -= 50.0f;
            }
            effPartsPositionSet(wk->dwk.hPartsList, (const f32 *)&wk->pos);
            effPartsScaleSet(wk->dwk.hPartsList, wk->scale);
            return 1;
        }
        break;
    case SBTLEFF_ST_END:
        wk->st = SBTLEFF_ST_EXIT;
        break;
    }
    return 1;
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCallLoadOnMemoryDataSBtlEff);

INCLUDE_ASM(const s32, "sbtlEff_00289E50", effPartsColorSet);

s32 EffFunc_SUMMON(SBTLEFF_WORK *wk) {
    struct SBTLUNIT_WORK *unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->acterID);
    s32 i;

    switch (wk->st) {
    case 1:
        if (unit == NULL || unit->mode != 4) {
            break;
        }
        if (wk_sbtl->flg & 4) {
            wk->st = 2;
        }
        break;
    case 2:
        LoadOnMemoryData_sbtlEff(wk, 0, 0);
        break;
    case 3:
        if (wk->counter == 1) {
            sbtlPlaySpecialSE(&unit->pos, 4);
        }
        if (wk->counter == 10) {
            unit->mode = 5;
            if (!unit->color) {
                unit->color = unit->dstcolor ? unit->dstcolor & 0xFFFFFF : 0x808080;
            }
            for (i = 0; i < 20; i++) {
                if (wk_sbtl->summonflg[i] == 1 && wk_sbtl->stockUnitID[i] == unit->field1A0) {
                    wk_sbtl->summonflg[i] = 2;
                }
            }
        }
        if (wk->counter >= wk->cmpcounter) {
            wk->counter = 0;
            wk->st = 4;
        }
        break;
    case 4:
        wk->st = 5;
        break;
    }
    return 1;
}

INCLUDE_ASM(const s32, "sbtlEff_00289E50", EffFunc_RETURN);

s32 EffFunc_SUMMON_E(SBTLEFF_WORK *wk) {
    struct SBTLUNIT_WORK *unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->acterID);

    switch (wk->st) {
    case 1:
        if (unit == NULL || unit->mode != 4 || !(wk_sbtl->flg & 4)) {
            break;
        }
        wk->st = 2;
        /* fallthrough */
    case 2:
        LoadOnMemoryData_sbtlEff(wk, 0, 0);
        break;
    case 3:
        if (wk->counter == 1) {
            sbtlPlaySpecialSE(&unit->pos, 4);
        }
        if (wk->counter >= wk->cmpcounter || unit == NULL) {
            wk->counter = 0;
            wk->st = 4;
            break;
        }
        if (wk->counter == 10 && unit->status != 10 && !unit->changeflg) {
            unit->mode = 5;
            if (!unit->color) {
                unit->color = unit->dstcolor ? unit->dstcolor & 0xFFFFFF : 0x808080;
            }
            unit->model->mode = 10;
            sbtlSetMotion_sbtlUnit(unit, 9, 0, 0, 1);
        }
        break;
    case 4:
        wk->st = 5;
        break;
    }
    return 1;
}

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

s32 sbtlSetDevilSkillID(s32 id, s32 flg) {
    SBTL_DEVIL_DATA *dd;
    s32 i;
    s32 n;

    if (wk_sbtl->field630 == NULL) {
        return 0;
    }
    switch (flg) {
        case 0:
            wk_sbtl->field630->sklid[0] = SKLID_ATK;
            dd = sbtlIfDrawActionSub(id);
            for (i = 0, n = 1; i < 4 && n < 4; i++) {
                if (dd->haveskill[i] != 0) {
                    wk_sbtl->field630->sklid[n++] = dd->haveskill[i];
                }
            }
            for (; n < 4; n++) {
                wk_sbtl->field630->sklid[n] = 0;
            }
            break;
        case 1:
            dd = sbtlIfDrawActionSub(id);
            for (i = 1, n = 1; i < 4 && n < 4; i++) {
                wk_sbtl->field630->sklid[n++] = dd->haveskill[i];
            }
            for (; n < 4; n++) {
                wk_sbtl->field630->sklid[n] = 0;
            }
            break;
        case 2:
            wk_sbtl->field630->sklid[0] = SKLID_ATK;
            dd = sbtlIfDrawActionSub(id);
            for (i = 0, n = 1; i < 4 && n < 4; i++) {
                if (dd->haveskill[i] != 0) {
                    wk_sbtl->field630->sklid[n++] = dd->haveskill[i];
                }
            }
            for (; n < 4; n++) {
                wk_sbtl->field630->sklid[n] = 0;
            }
            break;
    }
    return 1;
}

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

/* NON_MATCHING: 59 of 61 words match.  */
#ifdef NON_MATCHING
s32 sbtlCreateSTP_Unit(SBTL_STP *lwk, struct SBTLUNIT_WORK *wk, s16 id, s32 time) {
    struct SBTLUNIT_WORK *helper;
    sdfVector4_t pos;
    SBTLVISUAL_DATA visual;
    s32 found;

    if (wk_sbtl->field4264) {
        return -1;
    }
    if (id == 2) {
        if (wk->eff_wait_kaisin) {
            return -1;
        }
        wk->eff_wait_kaisin = 20;
    }
    found = 0;
    if (wk->flg & 0x1000) {
        helper = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->field494);
        if (helper != NULL) {
            SDF_VU0_LOADVECTOR(&helper->pos);
            SDF_VU0_STOREVECTOR_MEMOPS(pos);
            found = 1;
        }
    }
    if (!found) {
        sbtlGetCylinderData(wk, &visual);
        SDF_VU0_LOADVECTOR(&visual.center);
        SDF_VU0_STOREVECTOR_MEMOPS(pos);
    }
    return CreateSTP(lwk, wk->field1A0, &pos, id, time > 0 ? time : 60);
}
#else
INCLUDE_ASM(const s32, "sbtlEff_00289E50", sbtlCreateSTP_Unit);
#endif

s32 CreateSTP(SBTL_STP *lwk, s32 ListID, const sdfVector4_t *pos, s16 id, s32 time) {
    sdfMemHandle_t *handle;
    SBTL_STP_NODE *stp;
    SBTL_STP_NODE *last;

    if (!lwk) {
        return 0;
    }
    handle = sdfMemAllocDirect(sizeof(SBTL_STP_NODE));
    stp = sdfMemLock(handle);
    memset(stp, 0, sizeof(SBTL_STP_NODE));
    stp->handle = handle;
    stp->prev = NULL;
    stp->next = NULL;
    last = lwk->head;
    if (!last) {
        lwk->head = stp;
        lwk->enabled = 1;
    } else {
        while (last->next) {
            last = last->next;
        }
        last->next = stp;
        stp->prev = last;
        lwk->enabled++;
    }
    stp->id = id;
    stp->ListID = ListID;
    stp->mode = 0;
    SDF_VU0_LOADVECTOR(pos);
    SDF_VU0_STOREVECTOR(&stp->pos);
    stp->pos.y -= (s32)(f32)GetIntervalRand(0, 20);
    stp->spd = GetIntervalRand(10, 40) * 0.01f;
    stp->counterMax = time;
    return lwk->enabled - 1;
}

s32 sbtlProcSBtlSTPSub(struct SBTLUNIT_WORK *wk)
{
    s32 status = wk->status;

    if (status != 11 && status != 14 && wk->model->mode != 9 && wk->mode >= 5) {
        return 1;
    }
    return 0;
}

s32 Proc_sbtlSTP(SBTL_STP *stp)
{
    SBTLVISUAL_DATA data;
    SBTL_STP_NODE *node;
    struct SBTLUNIT_WORK *unit;
    s32 n;

    if (stp == NULL) {
        return 0;
    }
    node = stp->head;
    if (node == NULL || stp->enabled == 0) {
        return 0;
    }
    if (wk_sbtl->flg & 0x8000000) {
        return 0;
    }
    if (wk_sbtl->flg2 & 0x40) {
        return 0;
    }
    do {
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, node->ListID);
        if (unit == NULL || !sbtlProcSBtlSTPSub(unit)) {
            node->mode = 2;
        } else if (node->id == 1 && !(unit->flg & 0x800)) {
            sbtlGetCylinderData(unit, &data);
            SDF_VU0_LOADVECTOR(&data.center);
            SDF_VU0_STOREVECTOR(&node->pos);
        }
        switch (node->mode) {
        case 0:
            node->alpha += 16;
            node->alpha = node->alpha <= 0 ? 0 : node->alpha >= 128 ? 127 : node->alpha;
            n = node->alpha;
            if (n >= 127) {
                node->counter = 0;
                node->mode = 1;
            }
            break;
        case 1:
            if (wk_sbtl->fieldCA2 && unit->status == 9) {
                break;
            }
            if (node->counter < 60) {
                node->y -= node->spd;
            }
            node->counter++;
            node->counter = node->counter <= 0 ? 0 : node->counter > 0x7FFF ? 0x7FFF : node->counter;
            n = node->counter;
            if (n >= node->counterMax) {
                node->mode = 2;
            }
            break;
        case 2:
            node->alpha -= 16;
            node->alpha = node->alpha <= 0 ? 0 : node->alpha > 128 ? 128 : node->alpha;
            n = node->alpha;
            if (n <= 0) {
                node->mode = 3;
            }
            break;
        }
        node = node->next;
    } while (node != NULL);
    return 1;
}

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
