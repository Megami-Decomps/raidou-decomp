#include "common.h"
#include "dds3Basic.h"
#include "dds3obj.h"
#include "dds3WorldBasic.h"
#include "dds3SceneBasic.h"
#include "dds3CameraObjectBasic.h"
#include "dds3ObjectBase.h"
#include "sbtlWork.h"
#include "sBtlUnit.h"
#include "sBtlEff.h"
#include "sBtlSound.h"
#include "sBtlScript.h"
#include "sbtlDMG.h"
#include "sBtlCalc.h"
#include "sBtlSM.h"
#include "sBtlItem.h"
#include "sdatUnit.h"
#include "sdfMemory.h"
#include "sdfChip.h"
#include "mdlManager.h"
#include "code_r1_0021CE60.h"
#include "sdfDelayedResrc.h"
#include "libsdf.h"
#include "fileManager.h"
#include "sdfFile.h"
#include "dds3Randomize.h"
#include "sdfSound.h"
#include "sbtlSub.h"
#include "sbtlSub_0027ED08.h"
#include "sdfQuat.h"
#include "sBtlSkill.h"
#include "sBtlAI.h"
#include "sBtlDvltk.h"
#include "sbtlActAI.h"
#include "evtUnitManager.h"
#include "dds3UnitObjectBasic.h"
#include "sdk_libc.h"
#include "fpu.h"
#include "ps2asm.h"
#include "dds3KernelCore.h"
#include "dds3KernelDraw.h"
#include "dds3KernelMain.h"
#include "fld.h"
#include "sBtlPlayer.h"
#include "dds3PadManager.h"
#include "scrScriptProcess.h"
#include "itfMesManager.h"
#include "sBtlTalk.h"
#include "sdfVu0.h"
#include "newdata.h"
#include "code_r1_0027CBE8.h"
#include "sBtlPanel.h"
#include "AW_XAtkPad.h"
#include "EventBit.h"
#include "fldProcess.h"
#include "sdfMath.h"
#include "sbtlTubeSupport.h"
#include "sdfSystem.h"
#include "sBtlEncount.h"
#include "sBtlVisual.h"

/* NON_MATCHING: 11 of 32 words match. Register allocation and branch layout differ; same calls and stores. */
#ifdef NON_MATCHING
s32 sbtlCallBtlLoadEffectDataIDMask(void)
{
    SBTL_WORK *wk = wk_sbtl;
    SKLID id;

    if (wk->battleMode == 1) {
        if (!sbtlLoadEffectData_IDMask(SKLID_HADAKA1, EFFPAC_ST_DECODE | EFFPAC_ST_ONMEM)) {
            return 0;
        }
        if (!sbtlLoadEffectData_IDMask(SKLID_HADAKA2, EFFPAC_ST_DECODE | EFFPAC_ST_ONMEM)) {
            return 0;
        }
        id = SKLID_HADAKA3;
    } else {
        s32 bullet = wk->equipBullet;

        if (bullet == 0) {
            return 1;
        }
        id = sbtlGetItemtbl(bullet)->sklid;
    }
    if (sbtlLoadEffectData_IDMask(id, EFFPAC_ST_DECODE | EFFPAC_ST_ONMEM)) {
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_0028A320", sbtlCallBtlLoadEffectDataIDMask);
#endif

INCLUDE_ASM(const s32, "code_0028A320", sbtlDeleteOnData);

INCLUDE_ASM(const s32, "code_0028A320", sbtlCallColorSet4);

INCLUDE_ASM(const s32, "code_0028A320", sbtlCallColorSet5);

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
    data = sbtlGetDmgEffScale(id, no);
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

/* NON_MATCHING: 230 of 251 words match. Register allocation in the defpos cases differs; same calls and stores. */
#ifdef NON_MATCHING
SBTLEFF_WORK *sbtlCallMemset17(SBTLEFF_LISTWORK *lwk, struct SBTLUNIT_WORK *wk, EFFID effectid, s32 starttime, const f32 *defpos,
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
        break;
    case 34:
        SDF_VU0_LOADVECTOR(&wk->pos);
        SDF_VU0_STOREVECTOR(&ewk->pos);
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
        SDF_VU0_STOREVECTOR(&b);
        a.y = 0.0f;
        b.y = 0.0f;
        sbtlGetQuat(&b, &a);
        SDF_VU0_STOREVECTOR(&ewk->rot);
        ewk->pos.y = sbtlFree(ewk->pos.x, ewk->pos.z);
        break;
    case 16:
    case 17:
    case 19:
        SDF_VU0_LOADVECTOR(&wk->pos);
        SDF_VU0_STOREVECTOR(&a);
        sbtlGetPosforDistance(wk, 10.0f);
        SDF_VU0_STOREVECTOR(&b);
        a.y = 0.0f;
        b.y = 0.0f;
        sbtlGetQuat(&b, &a);
        SDF_VU0_STOREVECTOR(&ewk->rot);
        /* retail reads defpos with a 16-byte stride, where HD swizzles y, x, z, w */
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
    return ewk;
}
#else
INCLUDE_ASM(const s32, "code_0028A320", sbtlCallMemset17);
#endif

s32 sbtlLeaf7(s32 id)
{
    if (sbtlData83->effpac[id].status & EFFPAC_ST_LOADEND) {
        return sbtlData83->effpac[id].num;
    }
    return 0;
}

EFFPDATA_WORK *sbtlGetDmgEffScale(s32 id, s32 no)
{
    if (sbtlData83->effpac[id].status & EFFPAC_ST_LOADEND) {
        return &sbtlData83->effpac[id].pwk[no]->wk;
    }
    return NULL;
}

SBTLEFF_PACDATAWORK *sbtlBtlSetEffectDataSub(s32 id, s32 no)
{
    if (sbtlData83->effpac[id].status & EFFPAC_ST_LOADEND) {
        return sbtlData83->effpac[id].pwk[no];
    }
    return NULL;
}

s32 sbtlBtlCheckDeleteEffectSub(SBTLEFF_WORK *wk)
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

s32 sbtlCallBtlSearchEffectData(struct SBTLUNIT_WORK *wk, SKLID skillid, u8 flg, s8 type, u16 side, s32 id, s32 param)
{
    EFFPDATA_WORK *data;
    s32 index;
    s32 i;
    s32 num;

    index = sbtlSearchEffectData(skillid);
    if (index == -1) {
        return 0;
    }
    sbtlPlaySkillSE(&wk->pos, skillid, flg);
    num = sbtlLeaf7(index);
    for (i = 0; i < num; i++) {
        data = sbtlGetDmgEffScale(index, i);
        if (!CheckEffectDataSingle(data->type, flg)) {
            continue;
        }
        if (type == 1) {
            return 1;
        }
        switch (data->iti & 0xF) {
        case 0:
            sbtlCallBtlCreateUnitList3(wk, skillid, flg, index, i, side, id, 1, param, i);
            break;
        case 4:
            sbtlCallBtlCreateUnitList3(wk, skillid, flg, index, i, side, id, 2, param, i);
            break;
        case 5:
            sbtlCallBtlCreateUnitList3(wk, skillid, flg, index, i, side, id, 3, param, i);
            break;
        case 8:
            sbtlCallBtlCreateUnitList3(wk, skillid, flg, index, i, side, id, 4, param, i);
            break;
        default:
            sbtlCallBtlCreateUnitList3(wk, skillid, flg, index, i, side, id, 0, param, i);
            break;
        }
    }
    if (num > 0) {
        sbtlGetEffectData_Num();
        sbtlAddSerialID2();
    }
    return 1;
}

s32 sbtlCallBtlGetSkilltbl16(struct SBTLUNIT_WORK *tgtwk, SKLID sklid)
{
    u16 area = sbtlGetSkilltbl(sklid)->area;

    if (!(area & 0x8)) {
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
        if ((wk->flg & 0x8800) == 0x8800 || ((wk->flg & 0x1000) && wk->unit.devilId == 258)) {
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

/* NON_MATCHING: 579 of 670 words match. Register allocation and block placement differ, and 2 of the 5 jump table entries; 8 words short. */
#ifdef NON_MATCHING
s32 sbtlCallBtlCreateUnitList3(struct SBTLUNIT_WORK *wk, SKLID sklid, u8 type, s32 index, s32 id, u16 acterside, s32 serialID2,
                                s16 tflg, s32 sttime, s16 count)
{
    SBTLDMG_HITWORK hwk;
    struct SBTLUNIT_LISTWORK *lwk;
    struct SBTLUNIT_WORK *unit;
    struct SBTLSKILLP_TBL *tbl;
    SBTLEFF_WORK *ewk;
    const sdfVector4_t *pos = NULL;
    sdatUnitWork_t *stock;
    BST badst;
    u8 side;
    s32 i;

    lwk = sbtlCreateUnitList(48);
    badst = BST_BOYCOTT;
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
        if (wk != NULL) {
            badst = sbtlGetBadStatus(&wk->unit);
        }
        /* A confused (BOUDO, MAHUU) attacker hits one fixed side. */
        for (i = 0; i < wk_sbtl->unitList->number; i++) {
            side = acterside;
            if (badst == BST_BOUDO || badst == BST_MAHUU) {
                side = (acterside & 2) ? 1 : 2;
            }
            unit = wk_sbtl->unitList->pUnitL[i].pUnit;
            if (unit != NULL && sbtlCheckLiveTarget_NotHitDmg(unit) && (unit->field414 & 0x3F & side)) {
                sbtlEntryUnitList(lwk, unit);
            }
        }
        break;
    case 3:
        for (i = 0; i < wk_sbtl->unitList->number; i++) {
            unit = wk_sbtl->unitList->pUnitL[i].pUnit;
            if (unit != NULL && sbtlCheckLiveTarget_NotHitDmg(unit) && !(unit->field414 & 0x3F & acterside)) {
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
    }

    for (i = 0; i < lwk->count; i++) {
        unit = lwk->pUnitL[i].pUnit;
        if (sbtlCheckEffectCancel(wk, sklid, sbtlGetDmgEffScale(index, id))) {
            continue;
        }
        if (!sbtlCallBtlGetSkilltbl16(unit, sklid)) {
            continue;
        }
        ewk = sbtlCallMemAllocDirect3(wk_sbtl->effList, unit, index, id);
        pos = &ewk->pos;
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
    if ((wk_sbtl->flg & 0x2000000) || type >= 2 || lwk->count <= 0 || !(tbl->area & 8) || count != 0) {
        goto end;
    }
    if (tbl->area & 0x4000) {
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
        if (unit == NULL || unit->status == 34) {
            goto end;
        }
        if (tflg != 0) {
            for (i = 0; i < lwk->count; i++) {
                unit = lwk->pUnitL[i].pUnit;
                if (unit != NULL && CheckLiveTarget(unit) && sbtlCallBtlGetSkilltbl16(unit, sklid)) {
                    memset(&hwk, 0, sizeof(hwk));
                    sbtlCreateHitC_None(wk_sbtl->hitList, unit, wk, &hwk, sklid, tbl, pos, -1);
                    sbtlAddSerialID2();
                    sbtlCallBtlCheckALLSkill(unit, &hwk, 0);
                    sbtlDeleteUnitList(hwk.hitlwk);
                }
            }
        } else if (tbl->area & 2) {
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
                    memset(&hwk, 0, sizeof(hwk));
                    sbtlCreateHitC_None(wk_sbtl->hitList, unit, wk, &hwk, sklid, tbl, pos, -1);
                    sbtlAddSerialID2();
                    sbtlCallBtlCheckALLSkill(unit, &hwk, 0);
                    sbtlDeleteUnitList(hwk.hitlwk);
                }
            }
        } else if (wk->model->field104 != -1) {
            if (!(wk->aiwk->status & 0x10) || wk->stockID == -1 || (wk->field414 & 2)) {
                unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->model->field104);
                if (unit != NULL && CheckLiveTarget(unit)) {
                    memset(&hwk, 0, sizeof(hwk));
                    sbtlCreateHitC_None(wk_sbtl->hitList, unit, wk, &hwk, sklid, tbl, pos, -1);
                    sbtlAddSerialID2();
                    sbtlCallBtlCheckALLSkill(unit, &hwk, 0);
                    sbtlDeleteUnitList(hwk.hitlwk);
                }
            } else if (!(tbl->type & 8) && wk_sbtl->battleMode != 3) {
                /* A stocked demon's skill on its own side reaches the player's stock. */
                stock = sbtlGetStockUnitData(wk->stockID);
                if ((tbl->area & 0x100) || sbtlLeaf53(stock)) {
                    sbtlGetDamage_GB(&wk->unit, stock, sklid);
                }
            }
        }

        if (tbl->type & 8) {
            if (wk_sbtl->battleMode == 3) {
                goto end;
            }
            if (sbtlGetItemtbl(wk_sbtl->useitemID)->side & 4) {
                for (i = 0; i < wk_sbtl->stockNum; i++) {
                    stock = sbtlGetStockUnitData(i);
                    if (sbtlLeaf53(stock)) {
                        sbtlCallBtlGetItemtbl3(stock, wk_sbtl->useitemID);
                        sbtlCallBtlSearchUnitID35(i);
                    }
                }
                goto end;
            }
            if (wk_sbtl->useitem_stock == -1) {
                goto end;
            }
            stock = sbtlGetStockUnitData(wk_sbtl->useitem_stock);
            if (tbl->area & 0x100) {
                sbtlCallBtlSearchUnitID35(wk_sbtl->useitem_stock);
                sbtlCallBtlGetItemtbl3(stock, wk_sbtl->useitemID);
                sbtlCheckItemMember_Dead(wk_sbtl->useitem_stock);
            } else if (sbtlLeaf53(stock)) {
                sbtlCallBtlGetItemtbl3(stock, wk_sbtl->useitemID);
                sbtlCallBtlSearchUnitID35(wk_sbtl->useitem_stock);
            }
            wk_sbtl->useitem_stock = -1;
        } else if ((tbl->area & 6) && (wk->field414 & 1) && wk_sbtl->battleMode != 3) {
            badst = sbtlGetBadStatus(&wk->unit);
            if (badst == BST_BOUDO || badst == BST_MAHUU) {
                goto end;
            }
            for (i = 0; i < wk_sbtl->stockNum; i++) {
                if (wk_sbtl->field344 > 0 && wk_sbtl->summonIndex[0] == i) {
                    continue;
                }
                stock = sbtlGetStockUnitData(i);
                if ((tbl->area & 0x100) || sbtlLeaf53(stock)) {
                    sbtlGetDamage_GB(&wk->unit, stock, sklid);
                }
            }
        }
    } else {
        if (wk->model->field104 != -1) {
            unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->model->field104);
            if (unit != NULL && sbtlCheckDamageTarget(unit)) {
                memset(&hwk, 0, sizeof(hwk));
                sbtlCreateHitC_None(wk_sbtl->hitList, unit, wk, &hwk, sklid, tbl, pos, -1);
                sbtlAddSerialID2();
                sbtlCallBtlCheckALLSkill(unit, &hwk, 0);
                sbtlDeleteUnitList(hwk.hitlwk);
            }
        }
    }
end:
    sbtlDeleteUnitList(lwk);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_0028A320", sbtlCallBtlCreateUnitList3);
#endif

void sbtlSetEffectData(SBTLEFF_WORK *ewk, struct SBTLUNIT_WORK *wk, SKLID sklid, s32 workID, u8 type, s32 no, u16 acterside,
                       s32 serialID2, s32 sttime)
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
    data = sbtlGetDmgEffScale(workID, no);
    pac = sbtlBtlSetEffectDataSub(ewk->workID, ewk->BEDno);
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
        if (tbl->area & 8) {
            hitmode = 2;
        }
    }
    if (hitmode == 1) {
        if (pac->hitnum > 0) {
            hit = pac->hit;
            i = pac->hitnum;
            do {
                ewk->hitserialID = sbtlCreateHitC_New_sbtlDMG(wk_sbtl->hitList, wk, ewk, wk->field1A0, sklid, tbl, &(*hit)->entry,
                                                              *hit, acterside, serialID2);
                hit++;
            } while (--i != 0);
        }
    }
}

INCLUDE_ASM(const s32, "code_0028A320", GetDataScale);

/* NON_MATCHING: 85 of 323 words match. Same size, structure aligned; the s-register roles differ. */
#ifdef NON_MATCHING
s32 SetEffectPos(SBTLEFF_WORK *ewk, struct SBTLUNIT_WORK *wk)
{
    SBTLVISUAL_DATA vd;
    sdfMatrix4_t mat;
    sdfVector4_t dir = { 0.0f, 0.0f, -1.0f, 0.0f };
    sdfVector4_t pos = { 0.0f, 0.0f, 0.0f, 0.0f };
    sdfVector4_t rot = { 0.0f, 0.0f, 0.0f, 1.0f };
    f32 effdeg[9] = { 0.0f, 45.0f, 90.0f, 135.0f, 180.0f, -135.0f, -90.0f, -45.0f, 0.0f };
    EFFPDATA_WORK *data;
    struct SBTLUNIT_WORK *unit;
    struct SBTLSKILLP_TBL *tbl;
    SBTL_WORK *work;
    s32 stack;
    s32 fromunit;

    fromunit = 0;
    unit = wk;
    data = sbtlGetDmgEffScale(ewk->workID, ewk->BEDno);
    work = wk_sbtl;
    stack = work->stackID[0];
    if (stack == work->summonIndex[0]) {
        stack = work->stackID[1];
    }
    if (!CheckEffectDataSingle(data->type, ewk->BEDtype)) {
        return 1;
    }
    switch (data->iti & 0xF) {
    case 0:
    case 1:
    case 4:
    case 5:
    case 8:
        unit = wk;
        fromunit = 1;
        break;
    case 2:
        fromunit = 1;
        if (data->type == 2) {
            unit = wk;
            break;
        }
        if (data->type == 5) {
            unit = wk;
            break;
        }
        unit = sbtlSearchUnit_ID(work->unitList, ewk->targetID);
        if (unit != NULL) {
            break;
        }
        tbl = sbtlGetSkilltbl(ewk->BEDsklid);
        unit = wk;
        if (!(tbl->area & 0x4000)) {
            break;
        }
        if (data->type != 1) {
            break;
        }
        if ((tbl->type & 0xFFF) != 8) {
            return 0;
        }
        break;
    case 3:
        unit = wk;
        fromunit = 0;
        if (ewk->BEDsklid != 416) {
            sbtlGetPointData(sbtlGetPointLast(unit, ewk->BEDsklid), &pos, &rot);
        } else {
            sbtlGetPointData(7, &pos, &rot);
        }
        break;
    case 6:
        fromunit = 1;
        if (sbtlGetSkilltbl(ewk->BEDsklid)->type & 0x1000) {
            unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
        } else if (wk_sbtl->field344 > 0) {
            unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[wk_sbtl->summonIndex[0]]);
        } else {
            unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
        }
        goto check;
    case 7:
        fromunit = 1;
        unit = sbtlSearchUnit_ID(work->unitList, work->stockUnitID[stack]);
    check:
        if (unit == NULL || !sbtlCheckDamageTarget(unit)) {
            unit = wk;
            fromunit = 1;
        }
        break;
    }
    if (fromunit == 1) {
        if (data->iti & 0xF0) {
            sbtlGetUnitHelperPos(unit, data->iti_no);
            SDF_VU0_STOREVECTOR(&pos);
            if (!(ewk->flg & SBTLEFF_FLG_LOCKHIT)) {
                ewk->flg |= SBTLEFF_FLG_LOCKHPOS;
            }
            if (data->iti & 0x40) {
                ewk->helperno = data->iti_no;
                ewk->flg |= SBTLEFF_FLG_LOCKHLP;
                sbtlGetUnitHelperMatrix(unit, data->iti_no);
                SDF_VU0_STOREMATRIX(&mat);
                sbtlGetQuatFromMatrix(&mat);
                SDF_VU0_STOREVECTOR(&rot);
            } else {
                SDF_VU0_LOADVECTOR(&unit->rot);
                SDF_VU0_STOREVECTOR(&rot);
            }
        } else {
            SDF_VU0_LOADVECTOR(&unit->pos);
            SDF_VU0_STOREVECTOR(&pos);
            SDF_VU0_LOADVECTOR(&unit->rot);
            SDF_VU0_STOREVECTOR(&rot);
        }
    }
    sbtlGetCylinderData(unit, &vd);
    if (!(data->iti & 0xF0) && fromunit == 1) {
        switch (data->takasa) {
        case 0:
            SDF_VU0_LOADVECTOR(&vd.center);
            SDF_VU0_STOREVECTOR(&pos);
            pos.y += vd.e_height * 0.5f;
            break;
        case 1:
            SDF_VU0_LOADVECTOR(&vd.center);
            SDF_VU0_STOREVECTOR(&pos);
            break;
        case 2:
            SDF_VU0_LOADVECTOR(&vd.center);
            SDF_VU0_STOREVECTOR(&pos);
            pos.y -= vd.e_height * 0.5f;
            break;
        case 3:
            SDF_VU0_LOADVECTOR(&vd.center);
            SDF_VU0_STOREVECTOR(&pos);
            pos.y -= vd.e_height * 0.8f;
            break;
        case 5:
            SDF_VU0_LOADVECTOR(&vd.center);
            SDF_VU0_STOREVECTOR(&pos);
            pos.y = sbtlFree(pos.x, pos.z);
            break;
        }
    }
    SDF_VU0_LOADVECTOR(&rot);
    SDF_VU0_STOREVECTOR(&ewk->rot);
    if (data->muki == 8) {
        SDF_VU0_LOADVECTOR(&pos);
        SDF_VU0_STOREVECTOR(&ewk->pos);
    } else {
        sbtlGetRotateDeg(&rot, effdeg[data->muki]);
        ToMatrix();
        SDF_VU0_LOADVECTOR(&dir);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_SCALEVECTOR(vd.width * ewk->scale);
        SDF_VU0_LOADVECTOR_B(&pos);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(&ewk->pos);
    }
    return 1;
}
#else
INCLUDE_RODATA(const s32, "code_0028A320", SetEffectPosTable);

INCLUDE_ASM(const s32, "code_0028A320", SetEffectPos);
#endif

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

INCLUDE_ASM(const s32, "code_0028A320", sbtlGetEffectSerialID2_Now);

s32 sbtlDeleteEffectData_SerialID2(SBTLEFF_LISTWORK *lwk, s32 serialID)
{
    SBTLEFF_WORK *wk = lwk->wk;

    if (lwk->num == 0 || wk == NULL) {
        return 0;
    }
    do {
        if (wk->serialID2 == serialID && sbtlCheckDeleteEffect(wk)) {
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

INCLUDE_ASM(const s32, "code_0028A320", sbtlSetDecAlpha_SerialID2);

s32 sbtlDeleteEffectData_SkillID(SBTLEFF_LISTWORK *lwk, SKLID skillid)
{
    SBTLEFF_WORK *wk = lwk->wk;

    if (lwk->num == 0 || wk == NULL) {
        return 0;
    }
    do {
        if (wk->BEDsklid == skillid && sbtlCheckDeleteEffect(wk)) {
            if ((u32)(wk->st - SBTLEFF_ST_INIT) < 2) {
                wk->flg |= SBTLEFF_FLG_SESTOP;
                wk->flg |= SBTLEFF_FLG_FORCERESET;
            } else {
                wk->st = SBTLEFF_ST_EXIT;
                wk->alpha = 0;
                wk->flg |= SBTLEFF_FLG_SESTOP;
            }
        }
        wk = wk->next;
    } while (wk != NULL);
    return 1;
}

INCLUDE_ASM(const s32, "code_0028A320", sbtlForceDelete_SPID);

INCLUDE_ASM(const s32, "code_0028A320", sbtlProcSub);

INCLUDE_ASM(const s32, "code_0028A320", sbtlLeaf8);

INCLUDE_ASM(const s32, "code_0028A320", sbtlGetStartTime_Damage);

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

/* NON_MATCHING: 26 of 92 words match. The counter clamp registers and the scheduling of the table address differ. */
#ifdef NON_MATCHING
s32 sbtlCallPadVibeFadeUpdate(SBTLEFF_LISTWORK *lwk)
{
    SBTLEFF_WORK *ewk;
    s32 counter;

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
        if (sbtlRefSbtl26(ewk)) {
            counter = ++ewk->counter;
            if (counter <= 0) {
                ewk->counter = 0;
            } else if (counter > 0x7FFF) {
                ewk->counter = 0x7FFF;
            } else {
                ewk->counter = counter;
            }
            if (sbtlBtlCheckDeleteEffectSub(ewk)) {
                EffFunc_Hanyou_BED(ewk);
            } else {
                sbtlCallPadVibeFadeUpdateData[ewk->effectid](ewk);
            }
        } else if (sbtlBtlCheckDeleteEffectSub(ewk)) {
            if ((sbtlGetSkilltbl(ewk->BEDsklid)->flg & 0x100) || ewk->st != SBTLEFF_ST_PROC) {
                EffFunc_Hanyou_BED(ewk);
            }
        } else if (ewk->st != SBTLEFF_ST_PROC) {
            if (sbtlBtlCheckDeleteEffectSub(ewk)) {
                EffFunc_Hanyou_BED(ewk);
            } else {
                sbtlCallPadVibeFadeUpdateData[ewk->effectid](ewk);
            }
        }
        ewk = ewk->next;
    } while (ewk != NULL);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_0028A320", sbtlCallPadVibeFadeUpdate);
#endif

s32 sbtlCallSetOt(void)
{
    SBTL_WORK *wk = wk_sbtl;
    SBTLEFF_LISTWORK *lwk = wk->effList;
    SBTLEFF_WORK *ewk;
    struct SBTLUNIT_WORK *target;
    struct SBTLUNIT_WORK *acter;

    if (lwk == NULL) {
        return 0;
    }
    if (wk->unitList == NULL) {
        return 0;
    }
    ewk = lwk->wk;
    if (ewk == NULL || lwk->num == 0) {
        return 0;
    }
    eglSetOt(40);
    eglLinkTest(1, 0, 128, 1, 0, 0, 1, 2);
    eglLinkAlpha(0);
    do {
        if (ewk->st >= SBTLEFF_ST_EXIT) {
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
                if (sbtlBtlCheckDeleteEffectSub(ewk)) {
                    sbtlCallSetOtData = 0;
                } else if (ewk->effectid == 50 || ewk->effectid == 1 || ewk->effectid == 2 || ewk->effectid == 10
                           || ewk->effectid == 11 || ewk->effectid == 13 || ewk->effectid == 6) {
                    sbtlCallSetOtData = 1;
                } else {
                    sbtlCallSetOtData = 0;
                }
                if (!sbtlBtlCheckDeleteEffectSub(ewk) || !(sbtlGetSkilltbl(ewk->BEDsklid)->flg & 0x100)
                    || (acter = sbtlSearchUnit_ID(wk_sbtl->unitList, ewk->acterID)) == NULL || acter->mode >= 5) {
                    effPartsDraw(ewk->dwk.hPartsList);
                }
            }
        }
        ewk = ewk->next;
    } while (ewk != NULL);
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

INCLUDE_ASM(const s32, "code_0028A320", sbtlGetEffectStat_ID);

s32 sbtlForceDelete_ID(SBTLEFF_LISTWORK *lwk, s32 eListID)
{
    SBTLEFF_WORK *wk = lwk->wk;

    if (lwk->num == 0 || wk == NULL) {
        return 0;
    }
    do {
        if (wk->eListID == eListID && sbtlCheckDeleteEffect(wk)) {
            if ((u32)(wk->st - SBTLEFF_ST_INIT) < 2) {
                wk->flg |= SBTLEFF_FLG_SESTOP;
                wk->flg |= SBTLEFF_FLG_FORCERESET;
            } else {
                wk->st = SBTLEFF_ST_EXIT;
                wk->alpha = 0;
                wk->flg |= SBTLEFF_FLG_SESTOP;
            }
        }
        wk = wk->next;
    } while (wk != NULL);
    return 1;
}

INCLUDE_ASM(const s32, "code_0028A320", sbtlForceDelete_ID_Acter);

INCLUDE_ASM(const s32, "code_0028A320", sbtlForceDelete_ID_SummonFR);

INCLUDE_ASM(const s32, "code_0028A320", sbtlFadeOut_BadStatusEffect);

INCLUDE_ASM(const s32, "code_0028A320", sbtlSetDecAlpha_SerialID);

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

INCLUDE_ASM(const s32, "code_0028A320", sbtlForceDelete_SkillFlag);

s32 sbtlCallBtlGetSkilltbl8(struct SBTLUNIT_WORK *wk, u32 flg)
{
    SBTLEFF_LISTWORK *lwk;
    SBTLEFF_WORK *ewk;
    s32 ret = 0;

    if (wk_sbtl != NULL && (lwk = wk_sbtl->effList) != NULL) {
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

INCLUDE_ASM(const s32, "code_0028A320", sbtlCallBtlGetSkilltbl9);

INCLUDE_ASM(const s32, "code_0028A320", sbtlCheckDeleteEffect);

INCLUDE_ASM(const s32, "code_0028A320", sbtlBtlForceResetSub);

INCLUDE_ASM(const s32, "code_0028A320", sbtlForceReset);

s32 sbtlForceResetDecAlpha(SBTLEFF_LISTWORK *lwk, struct SBTLUNIT_WORK *wk, s32 alpha)
{
    SBTLEFF_WORK *ewk = lwk->wk;

    if (lwk->num == 0 || ewk == NULL) {
        return 0;
    }
    do {
        if (ewk->acterID == wk->field1A0 && sbtlCheckDeleteEffect(ewk)) {
            if ((u32)(ewk->st - SBTLEFF_ST_INIT) < 2) {
                ewk->flg |= SBTLEFF_FLG_SESTOP;
                ewk->flg |= SBTLEFF_FLG_FORCERESET;
            } else {
                ewk->st = SBTLEFF_ST_END;
                ewk->alpha_cnt = alpha;
                ewk->flg |= SBTLEFF_FLG_SESTOP;
            }
        }
        ewk = ewk->next;
    } while (ewk != NULL);
    return 1;
}

/* NON_MATCHING: 50 of 128 words match. s6/s7 swap and the reload order in the unlink; 3 words long. */
#ifdef NON_MATCHING
s32 sbtlReleaseEff(SBTLEFF_LISTWORK *lwk, u8 flg)
{
    SBTLEFF_WORK *ewk;
    SBTLEFF_WORK *next;
    s32 st;
    s32 found;
    SKLID BEDsklid;
    s32 BEDtype;
    s32 sestop;

    if (wk_sbtl->effList == NULL) {
        return 1;
    }
    ewk = lwk->wk;
    found = 0;
    if (ewk == NULL || lwk->num == 0) {
        return 1;
    }
    do {
        st = ewk->st;
        if (flg) {
            if (st != SBTLEFF_ST_INIT && st != SBTLEFF_ST_LOAD) {
                ewk->st = SBTLEFF_ST_EXIT;
                st = SBTLEFF_ST_EXIT;
            } else {
                found = 1;
                ewk->flg |= SBTLEFF_FLG_FORCERESET;
            }
        }
        switch (st) {
        case SBTLEFF_ST_INIT:
        case SBTLEFF_ST_LOAD:
            if (wk_sbtl->flg & 8) {
                ewk->st = SBTLEFF_ST_EXIT;
            }
            ewk = ewk->next;
            break;
        case SBTLEFF_ST_EXIT:
            BEDsklid = ewk->BEDsklid;
            BEDtype = ewk->BEDtype;
            sestop = ewk->flg < 0;
            if (ewk->prev != NULL) {
                ewk->prev->next = ewk->next;
            }
            if (ewk->next != NULL) {
                ewk->next->prev = ewk->prev;
            }
            if (ewk->prev == NULL) {
                lwk->wk = ewk->next;
                if (ewk->next != NULL) {
                    ewk->next->prev = NULL;
                }
            }
            next = ewk->next;
            if (ewk->dwk.hPartsList != NULL) {
                effPartsDelete(ewk->dwk.hPartsList);
                ewk->dwk.hPartsList = NULL;
            }
            if (!(ewk->flg & SBTLEFF_FLG_ONMEMORY) && ewk->dwk.hData != NULL) {
                sdfMemFreeDirect(ewk->dwk.hData);
                ewk->dwk.pData = NULL;
                ewk->dwk.hData = NULL;
                ewk->dwk.hFile = NULL;
            }
            sdfMemFreeDirect(ewk->handle);
            ewk = next;
            lwk->num--;
            if (sestop && !sbtlSearchEffect_SAB(wk_sbtl->effList, BEDsklid, BEDtype)) {
                sbtlStopSkillSE(BEDsklid, BEDtype);
            }
            break;
        default:
            ewk = ewk->next;
            break;
        }
    } while (ewk != NULL);
    if (flg == 1 && found == 1) {
        return 0;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_0028A320", sbtlReleaseEff);
#endif

INCLUDE_ASM(const s32, "code_0028A320", sbtlDeleteEffList);

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

INCLUDE_ASM(const s32, "code_0028A320", sbtlSearchEffect_SAB);

INCLUDE_ASM(const s32, "code_0028A320", sbtlSearchEffect_SAB_Acter);

s32 sbtlSearchEffect_ID(SBTLEFF_LISTWORK *lwk, EFFID effectid)
{
    SBTLEFF_WORK *wk = lwk->wk;

    if (lwk->num == 0 || wk == NULL) {
        return 0;
    }
    do {
        if (wk->effectid == effectid) {
            return 1;
        }
        wk = wk->next;
    } while (wk != NULL);
    return 0;
}

s32 sbtlSearchEffect_WorkID(SBTLEFF_LISTWORK *lwk, s32 workid)
{
    SBTLEFF_WORK *wk = lwk->wk;

    if (lwk->num == 0 || wk == NULL) {
        return 0;
    }
    do {
        if (wk->workID == workid) {
            return 1;
        }
        wk = wk->next;
    } while (wk != NULL);
    return 0;
}

s32 sbtlInitEffectWork(void)
{
    memset(sbtlData83->effpac, 0, sizeof(SBTLEFF_PACLIST));
    return 1;
}

INCLUDE_ASM(const s32, "code_0028A320", CheckActionRun);

SBTLEFF_DELLISTWORK *sbtlCreateDelList(void)
{
    SBTLEFF_DELLISTWORK *lwk = sdfChipAllocZeroDirect(sizeof(SBTLEFF_DELLISTWORK));

    lwk->num = 0;
    lwk->dwk = NULL;
    return lwk;
}

INCLUDE_ASM(const s32, "code_0028A320", sbtlCheckDelWork_Request);

INCLUDE_ASM(const s32, "code_0028A320", sbtlSearchDelWork_Side);

INCLUDE_ASM(const s32, "code_0028A320", sbtlCreateDelWork);

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

INCLUDE_ASM(const s32, "code_0028A320", sbtlDeleteDelList);

INCLUDE_ASM(const s32, "code_0028A320", sbtlCallBtlCreateDelWork);

INCLUDE_ASM(const s32, "code_0028A320", sbtlForceDelete_EffectWork_ID);

INCLUDE_ASM(const s32, "code_0028A320", sbtlForceDelete_EffectWork);

INCLUDE_ASM(const s32, "code_0028A320", sbtlBtlForceDeleteEffectWorkIDSub2);

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

/* NON_MATCHING: 8 of 62 words match. Register allocation and the placement of the not-found return differ; same calls and stores. */
#ifdef NON_MATCHING
s32 sbtlLoadEffectData_IDMask(SKLID id, EFFPAC_ST flg)
{
    s32 i = sbtlSearchEffectData(id);

    if (i == -1) {
        i = sbtlBtlLoadEffectDataIDMaskSub(id);
        if (i == -1) {
            return 0;
        }
    }
    if (!sbtlCallMemset18(&sbtlData83->effpac[i], id, flg, NULL)) {
        return 0;
    }
    if (!(sbtlData83->effpac[i].status & EFFPAC_ST_DECON) && (flg & EFFPAC_ST_DECODE)) {
        sbtlData83->effpac[i].status |= flg & ~EFFPAC_ST_DECODE;
    } else {
        sbtlData83->effpac[i].status |= flg;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_0028A320", sbtlLoadEffectData_IDMask);
#endif

/* NON_MATCHING: 5 of 65 words match. Register allocation and the status update branch layout differ; same calls and stores. */
#ifdef NON_MATCHING
s32 sbtlCallBtlSearchEffectData2(s32 id, u16 flg, void *pData)
{
    s32 i = sbtlSearchEffectData(id);

    if (i == -1) {
        i = sbtlBtlLoadEffectDataIDMaskSub(id);
        if (i == -1) {
            return 0;
        }
    }
    if (!sbtlCallMemset18(&sbtlData83->effpac[i], id, flg, pData)) {
        return 0;
    }
    if (!(sbtlData83->effpac[i].status & EFFPAC_ST_DECON) && (flg & EFFPAC_ST_DECODE)) {
        sbtlData83->effpac[i].status |= flg & ~EFFPAC_ST_DECODE;
    } else {
        sbtlData83->effpac[i].status |= flg;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_0028A320", sbtlCallBtlSearchEffectData2);
#endif

INCLUDE_ASM(const s32, "code_0028A320", sbtlLoadEffectData_ID);

s32 sbtlGetExeSkill(SKLID sklid)
{
    s32 no = sbtlGetSklVisualTbl(sklid)->SABno;

    if (no == 0) {
        no = -1;
    }
    return no;
}
