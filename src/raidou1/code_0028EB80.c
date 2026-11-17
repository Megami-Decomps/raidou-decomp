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
#include "sBtlVisual.h"

INCLUDE_ASM(const s32, "code_0028EB80", sbtlLoadEffectData_Nakama);

/* NON_MATCHING: 3 of 68 words match. Register allocation and the skip-rule branch layout differ; same calls. */
#ifdef NON_MATCHING
s32 sbtlCallBtlGetSkilltbl10(struct SBTLUNIT_WORK *wk)
{
    s32 *skill = &wk->haveskill[2];
    s32 i;
    s32 fail = 0;
    s32 no;

    for (i = 0; i < 9; i++, skill++) {
        if (*skill == 0) {
            continue;
        }
        if ((sbtlGetSkilltbl(*skill)->type & 0x2000) && *skill != 240) {
            continue;
        }
        if (sbtlGetExeSkill(*skill) == -1) {
            continue;
        }
        no = sbtlSearchEffectData(*skill);
        if (no == -1) {
            no = sbtlBtlLoadEffectDataIDMaskSub(*skill);
            if (no == -1) {
                continue;
            }
        }
        if (!sbtlCallMemset18(&sbtlData83->effpac[no], *skill, 0, NULL)) {
            fail = 1;
            break;
        }
    }
    return fail ^ 1;
}
#else
INCLUDE_ASM(const s32, "code_0028EB80", sbtlCallBtlGetSkilltbl10);
#endif

/* NON_MATCHING: 135 of 549 words match. Retail re-reads pac->pwk[hitidx[j]]->hit[k] at each use and spills a register; the draft caches it. 36 words short. */
#ifdef NON_MATCHING
s32 sbtlCallMemset18(SBTLEFF_PACLIST *pac, SKLID skillid, u16 flg, void *pData)
{
    s32 size[SBTLEFF_PACDATA_MAX] = { 0 };
    s32 hitidx[SBTLEFF_PACDATA_MAX];
    char filename[256];
    s32 ready;
    s32 hitcount;
    s32 hitnum;
    s32 *p;
    u8 *src;
    s32 i;
    s32 j;
    s32 k;
    SBTLEFF_PACDATAWORK *pwk;
    SBTL_HIT_PACDATA *hit;
    sdfMemHandle_t *handle;
    s16 cnt;

    if (pac->status & EFFPAC_ST_LOADEND) {
        return 1;
    }
    switch (pac->gstat) {
    case 0:
        if (pData == NULL) {
            sbtlSobedSAB(skillid, filename);
            pac->file = fileLoad(filename);
        }
        pac->skillID = skillid;
        pac->gstat++;
        wk_sbtl->flg |= 0x20000;
        return 0;
    case 1:
        break;
    case 2:
        if (sbtlLoadSkillSE(pac)) {
            pac->gstat = 5;
            pac->status |= EFFPAC_ST_LOADEND;
            return 1;
        }
        return 0;
    case 5:
        pac->status |= EFFPAC_ST_LOADEND;
        return 0;
    default:
        return 0;
    }

    ready = 0;
    if (pData != NULL) {
        ready = 1;
    } else if (pac->pData == NULL) {
        ready = pacFileCheckEnd(pac->file) != 0;
    }
    if (ready != 1) {
        return 0;
    }
    if (pData != NULL) {
        pac->pData = pData;
        pac->handle = NULL;
    } else {
        pac->handle = fileGetMemHandle(pac->file);
        pac->pData = sdfMemLock(pac->handle);
        fileHandleFree(pac->file);
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
        pwk = pac->pwk[i];
        if (pwk->wk.eplno == i) {
            handle = sdfMemAllocDirect(size[i]);
            pwk->pData = sdfMemLock(handle);
            pwk->flg = 0;
            pwk->handle = handle;
            memcpy(pwk->pData, src, size[i]);
        } else {
            handle = sdfMemAllocDirect(size[pwk->wk.eplno]);
            pwk->pData = sdfMemLock(handle);
            pwk->flg = 0;
            pwk->handle = handle;
            memcpy(pwk->pData, pac->pwk[pwk->wk.eplno]->pData, size[pwk->wk.eplno]);
        }
        pac->pwk[i]->hitnum = 0;
        if (flg & EFFPAC_ST_DECODE) {
            pac->pwk[i]->hPartsList = Create_effPartsList(pac->pwk[i]->pData);
            pac->status |= EFFPAC_ST_DECON;
        }
        src += size[i];
    }
    for (i = 0; i < pac->num; i++) {
        if (flg & EFFPAC_ST_DECODE) {
            sdfMemFreeDirect(pac->pwk[i]->handle);
            pac->pwk[i]->pData = NULL;
            pac->pwk[i]->handle = NULL;
        }
    }

    /* Hit records: per listed effect, a count then each SBTLSKILLHIT_TBL with its optional data. */
    p = (s32 *)src;
    for (j = 0; j < hitcount; j++) {
        hitnum = *p++;
        pac->pwk[hitidx[j]]->hitnum = hitnum;
        for (k = 0; k < hitnum; k++) {
            handle = sdfMemAllocDirect(sizeof(SBTL_HIT_PACDATA));
            pac->pwk[hitidx[j]]->flg = 1;
            hit = sdfMemLock(handle);
            hit->handle = handle;
            pac->pwk[hitidx[j]]->hit[k] = hit;
            hit->entry = *(SBTLSKILLHIT_TBL *)p;
            p += sizeof(SBTLSKILLHIT_TBL) / sizeof(s32);
            hit = pac->pwk[hitidx[j]]->hit[k];
            if (hit->entry.flg & 0x1000) {
                hit->BpSize = *p;
                handle = sdfMemAllocDirect(hit->BpSize);
                hit->BpData = sdfMemLock(handle);
                memcpy(hit->BpData, p + 1, hit->BpSize);
                hit->handle08 = handle;
                p = (s32 *)((u8 *)p + hit->BpSize + sizeof(s32));
            } else {
                hit->handle08 = NULL;
            }
            if (pac->pwk[hitidx[j]]->hit[k]->entry.flg & 0x2000) {
                s32 sz = *p;

                src = (u8 *)(p + 1);
                p = (s32 *)((u8 *)p + sz + sizeof(s32) + sizeof(SBTLEFF_EFFTIME));
                handle = sdfMemAllocDirect(sz);
                hit = pac->pwk[hitidx[j]]->hit[k];
                hit->EpData = sdfMemLock(handle);
                hit->hPartsList = NULL;
                memcpy(hit->EpData, src, sz);
                hit->handle10 = handle;
                hit->time = *(SBTLEFF_EFFTIME *)(src + sz);
                if (sz == 0) {
                    hit->entry.flg &= ~0x2000;
                }
            } else {
                pac->pwk[hitidx[j]]->hit[k]->handle10 = NULL;
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
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_0028EB80", sbtlCallMemset18);
#endif

INCLUDE_ASM(const s32, "code_0028EB80", sbtlRefSbtl24);

INCLUDE_ASM(const s32, "code_0028EB80", sbtlRefSbtl25);

INCLUDE_ASM(const s32, "code_0028EB80", LoadOnMemoryData);

INCLUDE_ASM(const s32, "code_0028EB80", sbtlCallBtlGetSklVisualTbl);

s32 sbtlRefSbtl26(SBTLEFF_WORK *wk)
{
    if (wk_sbtl->flg & 0x8000000) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_0028EB80", LoadOnMemoryData_sbtlEff);

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
            effPartsRotateSet(wk->dwk.hPartsList, &wk->rot.x);
            effPartsScaleSet(wk->dwk.hPartsList, wk->scale);
            if (wk->flg & SBTLEFF_FLG_LOCKHIT) {
                hit = sbtlSearchHit_SerialID(wk_sbtl->hitList, wk->hitserialID);
                if (hit != NULL) {
                    SDF_VU0_LOADVECTOR(&hit->pos);
                    SDF_VU0_STOREVECTOR(&wk->pos);
                    effPartsPositionSet(wk->dwk.hPartsList, &wk->pos.x);
                }
            } else {
                effPartsPositionSet(wk->dwk.hPartsList, &wk->pos.x);
            }
        }
        wk->st = SBTLEFF_ST_PROC;
        wk->cmpcounter = wk->life;
        wk->counter = 0;
        return;
    }
    wk->st = SBTLEFF_ST_EXIT;
}

INCLUDE_ASM(const s32, "code_0028EB80", EffFunc_Slip_G);

INCLUDE_ASM(const s32, "code_0028EB80", EffFunc_Hanyou);

/* NON_MATCHING: 61 of 157 words match. flg is cached in a0 where retail uses v1, and &wk->rot is computed once; 2 words short. */
#ifdef NON_MATCHING
s32 EffFunc_Hanyou_BED(SBTLEFF_WORK *wk)
{
    sdfMatrix4_t mat;
    struct SBTLDMG_HITWORK *hit;
    struct SBTLUNIT_WORK *unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->acterID);

    switch (wk->st) {
    case SBTLEFF_ST_INIT:
        if (wk_sbtl->flg & 4) {
            wk->st = SBTLEFF_ST_LOAD;
        }
        break;
    case SBTLEFF_ST_LOAD:
        LoadOnMemoryData_BED(wk, NULL, 0);
        break;
    case SBTLEFF_ST_PROC:
        if (wk->cmpcounter != 0 && wk->counter >= wk->cmpcounter) {
            wk->counter = 0;
            wk->st = SBTLEFF_ST_END;
            break;
        }
        ColorSet(wk->dwk.hPartsList, (wk->alpha << 24) | 0x808080);
        if ((wk->flg & SBTLEFF_FLG_LOCKHLP) && unit != NULL) {
            sbtlGetUnitHelperMatrix(unit, wk->helperno);
            SDF_VU0_STOREMATRIX(&mat);
            sbtlGetQuatFromMatrix(&mat);
            SDF_VU0_STOREVECTOR(&wk->rot);
            effPartsRotateSet(wk->dwk.hPartsList, &wk->rot.x);
        }
        if (wk->flg & SBTLEFF_FLG_LOCKHIT) {
            hit = sbtlSearchHit_SerialID(wk_sbtl->hitList, wk->hitserialID);
            if (hit == NULL) {
                break;
            }
            SDF_VU0_LOADVECTOR(&hit->rot);
            SDF_VU0_STOREVECTOR(&wk->rot);
            effPartsRotateSet(wk->dwk.hPartsList, &wk->rot.x);
            SDF_VU0_LOADVECTOR(&hit->pos);
            SDF_VU0_STOREVECTOR(&wk->pos);
            effPartsPositionSet(wk->dwk.hPartsList, &wk->pos.x);
        }
        if ((wk->flg & (SBTLEFF_FLG_LOCKHLP | SBTLEFF_FLG_LOCKHIT)) == SBTLEFF_FLG_LOCKHLP && unit != NULL) {
            sbtlGetUnitHelperPos(unit, wk->helperno);
            SDF_VU0_STOREVECTOR(&wk->pos);
            effPartsPositionSet(wk->dwk.hPartsList, &wk->pos.x);
        }
        if ((wk->flg & SBTLEFF_FLG_LOCKHPOS) && unit != NULL && !(wk->flg & SBTLEFF_FLG_LOCKHIT)) {
            SetEffectPos(wk, unit);
            effPartsRotateSet(wk->dwk.hPartsList, &wk->rot.x);
            effPartsPositionSet(wk->dwk.hPartsList, &wk->pos.x);
        }
        break;
    case SBTLEFF_ST_END:
        wk->alpha -= wk->alpha_cnt;
        if (wk->alpha <= 0) {
            wk->alpha = 0;
            wk->st = SBTLEFF_ST_EXIT;
        }
        if (wk->st != SBTLEFF_ST_EXIT) {
            ColorSet(wk->dwk.hPartsList, (wk->alpha << 24) | 0x808080);
        }
        break;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_0028EB80", EffFunc_Hanyou_BED);
#endif

INCLUDE_ASM(const s32, "code_0028EB80", EffFunc_NMLDAMAGE);

INCLUDE_ASM(const s32, "code_0028EB80", EffFunc_DEADDAMAGE);

INCLUDE_ASM(const s32, "code_0028EB80", sbtlCallLoadOnMemoryDataSBtlEff);

INCLUDE_ASM(const s32, "code_0028EB80", effPartsColorSet);

INCLUDE_ASM(const s32, "code_0028EB80", EffFunc_SUMMON);

INCLUDE_ASM(const s32, "code_0028EB80", EffFunc_RETURN);

INCLUDE_ASM(const s32, "code_0028EB80", EffFunc_SUMMON_E);

INCLUDE_ASM(const s32, "code_0028EB80", sbtlCallBtlSearchUnitID22);

INCLUDE_ASM(const s32, "code_0028EB80", sbtlCallBtlSearchUnitID23);

/* NON_MATCHING: 34 of 138 words match. Register allocation and the state branch layout differ; same calls and stores. */
#ifdef NON_MATCHING
s32 sbtlCallBtlSearchUnitID24(SBTLEFF_WORK *wk)
{
    struct SBTLUNIT_WORK *unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->acterID);
    s32 fadergb;

    switch (wk->st) {
    case SBTLEFF_ST_INIT:
        if (unit == NULL || unit->mode != 4 || !(wk_sbtl->flg & 4)) {
            break;
        }
        wk->st = SBTLEFF_ST_LOAD;
    case SBTLEFF_ST_LOAD:
        if (wk->flg & SBTLEFF_FLG_FORCERESET) {
            wk->st = SBTLEFF_ST_EXIT;
            break;
        }
        wk->st = SBTLEFF_ST_PROC;
        wk->cmpcounter = wk->life;
        wk->dwk.hPartsList = NULL;
        wk->counter = 0;
        break;
    case SBTLEFF_ST_PROC:
        if (wk->counter >= wk->cmpcounter || unit == NULL) {
            wk->counter = 0;
            wk->st = SBTLEFF_ST_END;
            break;
        }
        if (wk->counter != 2 || unit->status == 10 || unit->changeflg) {
            break;
        }
        unit->mode = 5;
        if (unit->color == 0) {
            unit->color = unit->dstcolor ? unit->dstcolor & 0xFFFFFF : 0x808080;
        }
        switch (unit->unit.devilId) {
        case 258:
        case 259:
            fadergb = 12;
            goto fade;
        case 290:
        case 297:
            fadergb = 8;
        fade:
            unit->color = 0;
            unit->fadergb = fadergb;
            unit->vstatus |= 0x200000;
            break;
        }
        unit->model->mode = 10;
        if (unit->aiwk->scrflag & 0x800000) {
            sbtlSetMotion_sbtlUnit(unit, 9, 0, 0, 1);
        } else {
            sbtlSetMotion_sbtlUnit(unit, 0, 0, 0, 1);
        }
        break;
    case SBTLEFF_ST_END:
        wk->st = SBTLEFF_ST_EXIT;
        break;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_0028EB80", sbtlCallBtlSearchUnitID24);
#endif

INCLUDE_ASM(const s32, "code_0028EB80", sbtlCallBtlSearchUnitID25);

INCLUDE_ASM(const s32, "code_0028EB80", sbtlLeaf9);

INCLUDE_ASM(const s32, "code_0028EB80", sbtlLeaf10);

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

INCLUDE_ASM(const s32, "code_0028EB80", sbtlCallMemFreeDirect4);

INCLUDE_ASM(const s32, "code_0028EB80", sbtlSetDevilSkillID);

INCLUDE_ASM(const s32, "code_0028EB80", sbtlRefSbtl43);

INCLUDE_ASM(const s32, "code_0028EB80", sbtlLeaf76);

INCLUDE_ASM(const s32, "code_0028EB80", sbtlRefSbtl44);

SBTL_STP *evtAllocateContext(void)
{
    SBTL_STP *stp = sdfChipAllocZeroDirect(sizeof(SBTL_STP));

    stp->enabled = 0;
    stp->head = NULL;
    return stp;
}

INCLUDE_ASM(const s32, "code_0028EB80", sbtlCreateSTP_Unit);

INCLUDE_ASM(const s32, "code_0028EB80", CreateSTP);

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
        } else {
            if (node->id == 1 && !(unit->flg & 0x800)) {
                sbtlGetCylinderData(unit, &data);
                SDF_VU0_LOADVECTOR(&data.center);
                SDF_VU0_STOREVECTOR(&node->pos);
            }
            if (wk_sbtl->mode == 14 && wk_sbtl->huumaTargetID != -1 &&
                unit->field1A0 == sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->huumaTargetID)->field1A0 && node->id == 1 &&
                node->mode < 2) {
                node->mode = 2;
            }
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
