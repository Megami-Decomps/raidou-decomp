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
#include "sBtlVisual.h"
#include "sBtlCalc.h"
#include "sBtlSM.h"
#include "sBtlItem.h"
#include "sdatUnit.h"
#include "sdfMemory.h"
#include "sdfChip.h"
#include "mdlManager.h"
#include "sbtlplayertbl.h"
#include "sdfDifficulty.h"
#include "sdfModel.h"
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
#include "dds3KernelDraw.h"

SBTLDMG_LISTWORK *sbtlAnimListProc(void) {
    SBTLDMG_LISTWORK *lwk;
    sdfMemHandle_t *handle;
    s32 i;

    lwk = sdfChipAllocZeroDirect(sizeof(SBTLDMG_LISTWORK));
    lwk->num = 0;
    handle = sdfMemAllocDirect(SBTLDMG_ANIM_MAX * sizeof(SBTLDMG_ANIMWORK *));
    lwk->unit = sdfMemLock(handle);
    for (i = SBTLDMG_ANIM_MAX - 1; i >= 0; i--) {
        lwk->unit[i] = NULL;
    }
    lwk->handle = handle;
    return lwk;
}

s32 sbtlCallBtlSearchUnitID14(struct SBTLUNIT_WORK *wk, s32 damage, s16 time, s32 mode)
{
    s32 found = 0;
    sdfVector4_t pos;
    SBTLVISUAL_DATA data;
    struct SBTLUNIT_WORK *helper;

    if (wk == NULL) {
        return 0;
    }
    if (damage == 0) {
        return 0;
    }
    if (wk->flg & 0x1000) {
        if (mode == 1) {
            helper = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->hitsliphelper);
        } else if (mode == 2) {
            helper = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->tdanhelper);
        } else {
            helper = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->field494);
        }
        if (helper != NULL) {
            SDF_VU0_LOADVECTOR(&helper->pos);
            SDF_VU0_STOREVECTOR(&pos);
            found = 1;
        }
    }
    if (!found) {
        sbtlGetCylinderData(wk, &data);
        SDF_VU0_LOADVECTOR(&data);
        SDF_VU0_STOREVECTOR(&pos);
    }
    if (wk->unit.now.maxhp <= 0) {
        return 0;
    }
    if (mode >= 0) {
        if (mode >= 3) {
            if (mode < 5) {
                sbtlCallMemAllocDirect(damage, &pos, mode, time);
            }
        } else if (wk->unit.now.hp * 100 / wk->unit.now.maxhp <= 25) {
            sbtlCallMemAllocDirect(damage, &pos, 2, time);
        } else if (wk->unit.now.hp * 100 / wk->unit.now.maxhp <= 50) {
            sbtlCallMemAllocDirect(damage, &pos, 1, time);
        } else {
            sbtlCallMemAllocDirect(damage, &pos, 0, time);
        }
    }
    return 1;
}

s32 sbtlCallMemAllocDirect(s32 dmg, const sdfVector4_t *pos, s32 type, s16 cmptime)
{
    u32 rgba[5] = { 0xA09DC380, 0xF2D22764, 0xEB501464, 0x46DCFA64, 0x96FA3264 };
    struct sdfMemHandle_t *handle;
    SBTLDMG_ANIMWORK *wk;
    s32 id;
    SBTLDMG_LISTWORK *lwk;

    handle = sdfMemAllocDirect(sizeof(SBTLDMG_ANIMWORK));
    wk = sdfMemLock(handle);
    wk->handle = handle;
    wk->cmptime = cmptime;
    wk->dmg = dmg;
    wk->st = 0;
    wk->time = 0;
    wk->counter = 0;
    SDF_VU0_LOADVECTOR(pos);
    SDF_VU0_STOREVECTOR(wk);
    lwk = wk_sbtl->animList;
    wk->rgba = rgba[type];
    wk->dmg_anm = 0;
    id = sbtlReleaseAnim_ID(lwk, wk);
    if (id != 255) {
        wk->ID = id;
    } else {
        sdfMemFreeDirect(wk->handle);
    }
    return 1;
}

s32 sbtlRegistAnimList(SBTLDMG_LISTWORK *lwk, s32 id)
{
    if (lwk->unit[id] != NULL) {
        sdfMemFreeDirect(lwk->unit[id]->handle);
        lwk->unit[id] = NULL;
    }
    return 1;
}

s32 sbtlReleaseAnim_ID(SBTLDMG_LISTWORK *lwk, SBTLDMG_ANIMWORK *wk)
{
    SBTLDMG_ANIMWORK **unit = lwk->unit;
    s32 i;

    for (i = 0; i < SBTLDMG_ANIM_MAX; i++, unit++) {
        if (*unit == NULL) {
            *unit = wk;
            lwk->num++;
            return i;
        }
    }
    return 255;
}

s32 sbtlDeleteAnimList(SBTLDMG_LISTWORK *lwk)
{
    s32 i;

    for (i = 0; i < SBTLDMG_ANIM_MAX; i++) {
        if (lwk->unit[i] != NULL) {
            sbtlRegistAnimList(lwk, lwk->unit[i]->ID);
        }
    }
    if (lwk->handle != NULL) {
        sdfMemFreeDirect(lwk->handle);
    }
    sdfChipFreeDirect(lwk);
    return 1;
}

s32 sbtlCallSin2(void) {
    SBTLDMG_ANIMWORK *wk;
    s32 i;
    s32 alpha;

    if (wk_sbtl == NULL || wk_sbtl->animList == NULL) {
        return 0;
    }
    if (wk_sbtl->flg & 0x8000000) {
        return 0;
    }
    if (wk_sbtl->flg2 & 0x20) {
        return 0;
    }
    for (i = 0; i < SBTLDMG_ANIM_MAX; i++) {
        wk = wk_sbtl->animList->unit[i];
        if (wk == NULL) {
            continue;
        }
        wk->time++;
        wk->time = (wk->time <= 0) ? 0 : (wk->time > 60) ? 60 : wk->time;
        switch (wk->st) {
        case 0:
            if (wk->time >= wk->cmptime) {
                wk->st = 1;
                wk->counter = 90;
            }
            break;
        case 1:
            wk->counter = (wk->counter + 2) % 360;
            wk->pos.y -= (sdfSin((f32)((wk->counter + 90) % 360) / 180.0f * 3.14159f) + 1.0f) * 0.5f * 4.9f + 0.1f;
            if (wk->counter >= 180) {
                wk->counter = 0;
                wk->st = 2;
            }
            break;
        case 2:
            alpha = wk->rgba & 0xFF;
            alpha -= 16;
            if (alpha > 0) {
                wk->rgba = (wk->rgba & ~0xFF) | alpha;
            } else {
                wk->st = 3;
            }
            break;
        default:
            sbtlRegistAnimList(wk_sbtl->animList, wk->ID);
            break;
        }
    }
    return 1;
}

s32 sbtlRefSbtl23(void) {
    SBTLDMG_ANIMWORK *wk;
    s32 i;

    if (wk_sbtl == NULL || wk_sbtl->animList == NULL) {
        return 0;
    }
    if (wk_sbtl->flg2 & 0x20) {
        return 0;
    }
    for (i = 0; i < SBTLDMG_ANIM_MAX; i++) {
        wk = wk_sbtl->animList->unit[i];
        if (wk == NULL) {
            continue;
        }
        switch (wk->st) {
        case 0:
            break;
        case 1:
        case 2:
            sbtlCallSprintf8(&wk->pos, wk->dmg, wk->rgba);
            break;
        }
    }
    return 1;
}

/* body: matched in raidou @ 61f09dc: src/raidou1/code_002929C8.c:124, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
s32 sbtlLeaf6(void) {
    return 1;
}

SBTLDMG_HITLISTWORK *sbtlCreateHitList(void) {
    SBTLDMG_HITLISTWORK *lwk;

    lwk = sdfChipAllocZeroDirect(sizeof(SBTLDMG_HITLISTWORK));
    lwk->num = 0;
    lwk->wk = NULL;
    lwk->serialID = 1;
    return lwk;
}

/* NON_MATCHING: 63 of 75 words match. a1 and a2 swap (match flag and wk_sbtl). Retail has a dead field414 & 2 test, reproduced by the else-if branch with the same call. */
#ifdef NON_MATCHING
s32 sbtlLoadAttackHitData_Decode(struct SBTLUNIT_WORK *wk) {
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
            if (wk_sbtl->battleMode == 0 || wk_sbtl->battleMode == 2) {
                if (!sbtlBtlLoadAttackHitDataDecodeSub(unit, 0)) {
                    return 0;
                }
            } else if (!sbtlLoadAttackHitData(unit, unit->minor, NULL)) {
                return 0;
            }
        } else if (unit->field414 & 2) {
            if (!sbtlLoadAttackHitData(unit, unit->unit.devilId, NULL)) {
                return 0;
            }
        } else if (!sbtlLoadAttackHitData(unit, unit->unit.devilId, NULL)) {
            return 0;
        }
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlDMG", sbtlLoadAttackHitData_Decode);
#endif

s32 sbtlSearchAttackHitData_Copy(struct SBTLUNIT_WORK *wk) {
    sdfMemHandle_t *handle;
    SBTL_HIT_PACDATA *pwk;
    s32 index;
    s32 i;

    index = sbtlSearchPacData(wk->major, wk->minor, wk->unit.devilId);
    if (index >= 0) {
    wk->hitwk.num = sbtlData84->pac[index].hit.num;
    for (i = 0; i < wk->hitwk.num; i++) {
        handle = sdfMemAllocDirect(sizeof(SBTL_HIT_PACDATA));
        pwk = sdfMemLock(handle);
        wk->hitwk.pwk[i] = pwk;
        pwk->handle = handle;
        pwk->entry = sbtlData84->pac[index].hit.pwk[i]->entry;
        wk->hitwk.pwk[i]->handle08 = NULL;
        wk->hitwk.pwk[i]->handle10 = NULL;
    }
    wk->hitwk.status |= 1;
    return 1;
    }
    return 0;
}

s32 sbtlLoadAttackHitData_pData(s32 index, void *pData) {
    while (sbtlLoadAttackHitData(&sbtlWork2, 0, pData) == 0) {
    }
    return 1;
}

s32 sbtlInitLoadAttackHitData_pData(void) {
    memset(&sbtlWork2, 0, sizeof(sbtlWork2));
    return 1;
}

s32 sbtlForceDelete_AttackHitData(void)
{
    sbtlCreateHitC_New(&sbtlWork2);
    return 1;
}

s32 sbtlBtlLoadAttackHitDataDecodeSub(struct SBTLUNIT_WORK *wk, s32 id) {
    if (wk->flg & 0x800) {
        return 1;
    }
    if (wk->hitwk.status & 1) {
        return 1;
    }
    memcpy(&wk->hitwk, &sbtlWork2.hitwk, sizeof(wk->hitwk));
    wk->hitwk.status |= 2;
    return 1;
}

/* NON_MATCHING: 165 of 249 words match. Register allocation of the stream pointer and handles and the shared return blocks differ; same calls and stores. */
#ifdef NON_MATCHING
s32 sbtlLoadAttackHitData(struct SBTLUNIT_WORK *wk, s32 id, void *ppData) {
    char filename[256];
    s32 ret;
    void *data;
    u8 *p;
    s32 loaded;
    sdfMemHandle_t *handle;
    sdfMemHandle_t *dataHandle;
    SBTL_HIT_PACDATA *pwk;
    s32 size;
    s32 i;

    ret = 1;
    data = NULL;
    loaded = 0;
    handle = NULL;
    if (wk->flg & 0x800) {
        return ret;
    }
    if (wk->hitwk.status & 1) {
        return ret;
    }
    memset(filename, 0, sizeof(filename));
    if (ppData != NULL && wk->hitwk.gstat == 0) {
        wk->hitwk.gstat++;
    }
    switch (wk->hitwk.gstat) {
    case 0:
        if (wk->major == 0) {
            sprintf(filename, "/battle/mdlhit/P%03X.MHT", wk->minor);
        } else {
            sprintf(filename, "/battle/mdlhit/%03X.MHT", wk->minor);
        }
        if (!sdfFileCheckExist(filename)) {
            return 1;
        }
        wk->hitwk.hFile = fileLoad(filename);
        wk->hitwk.gstat++;
        return 0;
    case 1:
        if (ppData != NULL) {
            data = ppData;
            wk->hitwk.hFile = NULL;
            handle = NULL;
            loaded = 1;
        } else if (pacFileCheckEnd(wk->hitwk.hFile)) {
            loaded = 1;
            handle = fileGetMemHandle(wk->hitwk.hFile);
            data = sdfMemLock(handle);
            fileHandleFree(wk->hitwk.hFile);
            wk->hitwk.hFile = NULL;
        }
        if (loaded != 1) {
            return 0;
        }
        p = data;
        wk->hitwk.num = *(s32 *)p;
        p += sizeof(s32);
        for (i = 0; i < wk->hitwk.num; i++) {
            dataHandle = sdfMemAllocDirect(sizeof(SBTL_HIT_PACDATA));
            pwk = sdfMemLock(dataHandle);
            pwk->handle = dataHandle;
            wk->hitwk.pwk[i] = pwk;
            pwk->entry = *(SBTLSKILLHIT_TBL *)p;
            p += sizeof(SBTLSKILLHIT_TBL);
            if (wk->hitwk.pwk[i]->entry.flg & 0x1000) {
                wk->hitwk.pwk[i]->BpSize = *(s32 *)p;
                p += sizeof(s32);
                dataHandle = sdfMemAllocDirect(wk->hitwk.pwk[i]->BpSize);
                wk->hitwk.pwk[i]->BpData = sdfMemLock(dataHandle);
                memcpy(wk->hitwk.pwk[i]->BpData, p, wk->hitwk.pwk[i]->BpSize);
                wk->hitwk.pwk[i]->handle08 = dataHandle;
                p += wk->hitwk.pwk[i]->BpSize;
            } else {
                wk->hitwk.pwk[i]->handle08 = NULL;
            }
            if (wk->hitwk.pwk[i]->entry.flg & 0x2000) {
                size = *(s32 *)p;
                p += sizeof(s32);
                dataHandle = sdfMemAllocDirect(size);
                wk->hitwk.pwk[i]->hPartsList = NULL;
                wk->hitwk.pwk[i]->EpData = sdfMemLock(dataHandle);
                memcpy(wk->hitwk.pwk[i]->EpData, p, size);
                p += size;
                wk->hitwk.pwk[i]->handle10 = dataHandle;
                wk->hitwk.pwk[i]->time = *(SBTLEFF_EFFTIME *)p;
                p += sizeof(SBTLEFF_EFFTIME);
            } else {
                wk->hitwk.pwk[i]->handle10 = NULL;
            }
        }
        wk->hitwk.status |= 1;
        if (handle != NULL) {
            sdfMemFreeDirect(handle);
        }
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "sbtlDMG", sbtlLoadAttackHitData);
#endif

s32 sbtlCreateHitC_New(struct SBTLUNIT_WORK *wk)
{
    s32 i;
    u16 status = wk->hitwk.status;
    s32 ret = 1;

    if (!(status & 1)) {
        return ret;
    }
    if (status & 2) {
        return ret;
    }
        for (i = 0; i < wk->hitwk.num; i++) {
            if (wk->hitwk.pwk[i]->handle08 != NULL) {
                sdfMemFreeDirect(wk->hitwk.pwk[i]->handle08);
                wk->hitwk.pwk[i]->handle08 = NULL;
            }
            if (wk->hitwk.pwk[i]->handle10 != NULL) {
                sdfMemFreeDirect(wk->hitwk.pwk[i]->handle10);
                wk->hitwk.pwk[i]->handle10 = NULL;
            }
            if (wk->hitwk.pwk[i]->handle != NULL) {
                sdfMemFreeDirect(wk->hitwk.pwk[i]->handle);
                wk->hitwk.pwk[i]->handle = NULL;
            }
        }
    wk->hitwk.status = 0;
    return 1;
}

/* NON_MATCHING: 251 of 545 words match. Same size; the first 251 words match, then the scheduling and register order of the field copies differ. */
#ifdef NON_MATCHING
s32 sbtlCreateHitC_New_sbtlDMG(struct SBTLDMG_HITLISTWORK *lwk, struct SBTLUNIT_WORK *wk, struct SBTLEFF_WORK *ewk, s32 acterID, SKLID skillid, struct SBTLSKILLP_TBL *tbl, struct SBTLSKILLHIT_TBL *hittbl, struct SBTL_HIT_PACDATA *pwk, u16 acterside, s32 serialID2) {
    sdfVector4_t rott;
    sdfVector4_t ofs;
    sdfVector4_t scl = { 1.0f, 1.0f, 1.0f, 1.0f };
    struct sdfMemHandle_t *handle;
    struct SBTLDMG_HITWORK *hwk;
    struct SBTLDMG_HITWORK *last;
    struct SBTLUNIT_WORK *unit;
    u32 serialID;

    memset(&ofs, 0, sizeof(ofs));
    ofs.w = 1.0f;
    handle = sdfMemAllocDirect(sizeof(SBTLDMG_HITWORK));
    hwk = sdfMemLock(handle);
    memset(hwk, 0, sizeof(SBTLDMG_HITWORK));
    hwk->handle = handle;
    hwk->acterID = wk->field1A0;
    hwk->actID = acterID;
    hwk->skillid = skillid;
    hwk->starttime = hittbl->starttime;
    hwk->life = hittbl->lifetime;
    hwk->targetflg = hittbl->targetflg;
    hwk->shatei = hittbl->shatei;
    hwk->fKaraburi = 1;
    hwk->dir = hittbl->dir;
    hwk->targetID = wk->model->field104;
    hwk->actioncnt = tbl->count;
    if (serialID2 == -1) {
        if (wk != NULL) {
            hwk->awk.aside = acterside;
            hwk->awk.adevilid = wk->unit.devilId;
            hwk->awk.aListID = wk->field1A0;
            hwk->awk.alevel = wk->unit.level;
            hwk->awk.aatk = wk->unit.now.str;
            hwk->awk.atec = wk->unit.now.tec;
            hwk->awk.ahp = wk->unit.now.hp;
            hwk->awk.amaxhp = wk->unit.now.maxhp;
        }
    } else if (wk_sbtl->huka2hwk != NULL) {
        hwk->awk = wk_sbtl->huka2hwk->awk;
    }
    hwk->hitlwk = sbtlCreateUnitList(24);
    if (hwk->actioncnt > 0) {
        hwk->actionf = hwk->life / hwk->actioncnt;
    } else {
        hwk->actionf = 0;
    }
    hwk->start = hittbl->start;
    hwk->end = hittbl->end;
    hwk->hittbl = hittbl;
    hwk->x = hittbl->x;
    hwk->y = hittbl->y;
    hwk->hitbk_cnt = tbl->hitbk_cnt;
    hwk->z = hittbl->z;
    hwk->hitbk_rate = tbl->hitbk_rate;
    if (ewk != NULL) {
        hwk->workID = ewk->workID;
        hwk->BEDtype = ewk->BEDtype;
    } else {
        hwk->workID = 0;
        hwk->BEDtype = 1;
    }
    hwk->lwk = NULL;
    hwk->prev = NULL;
    hwk->next = NULL;
    hwk->hModel = NULL;
    hwk->pBasic = NULL;
    hwk->hEveUnit = NULL;
    hwk->hPartsList = NULL;
    unit = NULL;
    if (skillid == 285) {
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, acterID);
    }
    if (unit == NULL) {
        unit = wk;
    }
    SDF_VU0_LOADVECTOR(&unit->rot);
    SDF_VU0_STOREVECTOR(&hwk->rot);
    hwk->acterpos[0] = unit->pos.x;
    hwk->acterpos[1] = unit->pos.y;
    hwk->acterpos[2] = unit->pos.z;
    if (lwk->wk == NULL) {
        lwk->wk = hwk;
        lwk->num = 1;
    } else {
        for (last = lwk->wk; last->next != NULL; last = last->next) {
        }
        last->next = hwk;
        hwk->prev = last;
        lwk->num++;
    }
    hwk->rad = hittbl->rad;
    hwk->type = tbl->type;
    hwk->rad_area = hittbl->rad_area;
    hwk->skillflg = tbl->flg;
    hwk->area = tbl->area;
    hwk->area_r = hittbl->area_r;
    hwk->attr_flg = tbl->attr;
    hwk->atype = hittbl->area_type;
    hwk->area_r2 = hittbl->area_r2;
    if (ewk != NULL) {
        hwk->eListID = ewk->eListID;
        if (ewk->flg & 0x100) {
            hwk->flg |= 0x40;
        }
    } else {
        hwk->eListID = -1;
    }
    hwk->acterside = acterside;
    hwk->side = sbtlGetSide(wk, tbl->area, hwk->acterside, &hwk->tchangeflg);
    hwk->sokudo = hittbl->sokudo;
    hwk->spd = hittbl->sokudo;
    hwk->kasokudo = hittbl->kasokudo;
    hwk->flg |= hittbl->flg;
    hwk->hittype = hittbl->type;
    hwk->cy = hittbl->cy;
    sbtlSetHitPos(wk, hwk, ewk, &hittbl->start, &hwk->pos, &hwk->rot, &rott, tbl->type);
    ofs.x += hwk->x;
    ofs.y += hwk->y;
    ofs.z += hwk->z;
    if (hwk->flg & 0x1000) {
        SDF_VU0_LOADVECTOR(&hwk->rot);
    } else {
        SDF_VU0_LOADVECTOR(&rott);
    }
    ToMatrix();
    SDF_VU0_LOADVECTOR(&ofs);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(&ofs);
    hwk->pos.x += ofs.x;
    hwk->pos.y += ofs.y;
    hwk->pos.z += ofs.z;
    if (hittbl->flg & 0x400) {
        hwk->homingpara = hittbl->homingpara;
    }
    if (hittbl->flg & 0x100) {
        if (hwk->flg & 0x1000) {
            while (mdlSearchResrc(7, wk_sbtl->hitMdlNo) != NULL) {
                wk_sbtl->hitMdlNo++;
            }
            mdlPackDecode(7, wk_sbtl->hitMdlNo, 0x101, pwk->BpData, pwk->BpSize);
            serialID = UnitRegist(7, wk_sbtl->hitMdlNo);
            hwk->pBasic = dds3SearchSceneBasic(dds3GetActiveScene(), serialID, 5);
            hwk->hEveUnit = ((dds3UnitObjectBasicWorkData_t *)hwk->pBasic->workdata)->hUnit;
            hwk->hModel = hwk->hEveUnit->owner;
            hwk->hEveUnit->flags |= 0x200000;
            SDF_VU0_LOADVECTOR(&scl);
            SDF_VU0_STOREVECTOR(&hwk->pBasic->poseset->scl);
            SDF_VU0_LOADVECTOR(&hwk->rot);
            SDF_VU0_STOREVECTOR(&hwk->pBasic->poseset->rot);
            SDF_VU0_LOADVECTOR(&hwk->pos);
            SDF_VU0_STOREVECTOR(&hwk->pBasic->poseset->pos);
            hwk->hModel->flags |= 1;
            hwk->hModel->first->speed = 1.0f;
            hwk->flg |= 0x1000;
            SDF_VU0_LOADVECTOR(&hwk->pos);
            SDF_VU0_STOREVECTOR(&hwk->iwk.apos);
            hwk->iwk.addtime = 1.0f / hwk->life;
            wk_sbtl->hitMdlNo++;
        } else {
            sbtlSetHitEndPos(wk, hwk, ewk, hittbl, &hwk->start, &hittbl->end, &hwk->endpos, tbl->type);
            hwk->iwk.apos0_per = hittbl->apos1.per;
            hwk->targetID = wk->model->field104;
            hwk->iwk.apos1_per = hittbl->apos2.per;
            SDF_VU0_LOADVECTOR(&wk->rot);
            SDF_VU0_STOREVECTOR(&hwk->rot);
            SDF_VU0_LOADVECTOR(&hwk->pos);
            SDF_VU0_STOREVECTOR(&hwk->iwk.apos);
            sbtlSetApos12(&hwk->pos, &hwk->endpos, &hwk->rot, hittbl, &hwk->iwk.apos0, &hwk->iwk.apos1);
            hwk->spd = hwk->sokudo;
            sbtlGetQuat(&hwk->pos, &hwk->endpos);
            SDF_VU0_STOREVECTOR(&hwk->rot);
            hwk->iwk.addtime = 0.0f;
        }
    }
    if (hwk->flg & 0x2000) {
        hwk->hPartsList = Create_effPartsList(pwk->EpData);
        effPartsRotateSet(hwk->hPartsList, (const f32 *)&hwk->rot);
        effPartsPositionSet(hwk->hPartsList, (const f32 *)&hwk->pos);
        hwk->EPL_counter = 0.0f;
        hwk->EPL_st_time = pwk->time.st_time;
        hwk->EPL_life = pwk->time.life;
        hwk->EPL_fout_time = pwk->time.fout_time;
        hwk->EPL_alpha = 128.0f;
        hwk->EPL_fin_time = pwk->time.fin_time;
        if (hwk->EPL_fin_time == hwk->EPL_counter) {
            hwk->EPL_alpha = 128.0f;
            hwk->EPL_in_alpha = 128.0f;
        } else {
            hwk->EPL_in_alpha = 128.0f / hwk->EPL_fin_time;
        }
        if (hwk->EPL_fout_time == 0.0f) {
            hwk->EPL_out_alpha = 128.0f;
        } else {
            hwk->EPL_out_alpha = 128.0f / hwk->EPL_fout_time;
        }
    }
    hwk->serialID = lwk->serialID;
    if (serialID2 == -1) {
        hwk->serialID2 = lwk->serialID2;
    } else {
        hwk->serialID2 = serialID2;
    }
    hwk->st = 1;
    lwk->serialID++;
    return hwk->serialID;
}
#else
INCLUDE_ASM(const s32, "sbtlDMG", sbtlCreateHitC_New_sbtlDMG);
#endif

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCreateHitC_None);

void sbtlAddSerialID2(void) {
    wk_sbtl->hitList->serialID2++;
    if (wk_sbtl->hitList->serialID2 >= 0x7FFF) {
        wk_sbtl->hitList->serialID2 = 0;
    }
}

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetApos12);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlCreateHitC_New_sbtlDMGTable);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlSetApos12Table);

s32 sbtlResetHitPos_XYZ(struct SBTLUNIT_WORK *wk, struct SBTLDMG_HITWORK *twk, struct SBTLEFF_WORK *ewk, struct SBTLSKILLHIT_POSWORK *pwk, sdfVector4_t *pos, sdfVector4_t *rot, u16 tbltype) {
    sdfVector4_t scale = { 1.0f, 1.0f, 1.0f, 1.0f };

    if (twk->skillid == 450) {
        return 1;
    }
    sbtlSetHitPos_XYZ(wk, twk, ewk, pwk, pos, rot, tbltype);
    if (twk->flg & 0x100) {
        if (twk->flg & 0x1000) {
            SDF_VU0_LOADVECTOR(&scale);
            SDF_VU0_STOREVECTOR(&twk->pBasic->poseset->scl);
            SDF_VU0_LOADVECTOR(&twk->rot);
            SDF_VU0_STOREVECTOR(&twk->pBasic->poseset->rot);
            SDF_VU0_LOADVECTOR(&twk->pos);
            SDF_VU0_STOREVECTOR(&twk->pBasic->poseset->pos);
            SDF_VU0_LOADVECTOR(&twk->pos);
            SDF_VU0_STOREVECTOR(&twk->iwk.apos);
            twk->iwk.addtime = 1.0f / twk->life;
            wk_sbtl->hitMdlNo++;
        } else {
            twk->targetID = wk->model->field104;
            SDF_VU0_LOADVECTOR(&wk->rot);
            SDF_VU0_STOREVECTOR(&twk->rot);
            SDF_VU0_LOADVECTOR(&twk->pos);
            SDF_VU0_STOREVECTOR(&twk->iwk.apos);
            sbtlSetApos12(&twk->pos, &twk->endpos, &twk->rot, twk->hittbl, &twk->iwk.apos0, &twk->iwk.apos1);
            twk->spd = twk->sokudo;
            twk->iwk.addtime = 0.0f;
        }
    }
    return 1;
}

s32 sbtlSetHitPos_XYZ(struct SBTLUNIT_WORK *wk, struct SBTLDMG_HITWORK *twk, struct SBTLEFF_WORK *ewk, struct SBTLSKILLHIT_POSWORK *pwk, sdfVector4_t *pos, sdfVector4_t *rot, u16 tbltype) {
    sdfVector4_t ofs;
    sdfVector4_t hitpos;
    sdfVector4_t hitrot;
    sdfVector4_t rott;
    sdfVector4_t *prot;
    sdfVector4_t *prott;

    memset(&ofs, 0, sizeof(ofs));
    ofs.w = 1.0f;
    SDF_VU0_LOADVECTOR(pos);
    SDF_VU0_STOREVECTOR(&hitpos);
    SDF_VU0_LOADVECTOR(rot);
    SDF_VU0_STOREVECTOR_NOMEM(&hitrot);
    prot = &hitrot;
    prott = &rott;
    sbtlSetHitPos(wk, twk, ewk, pwk, &hitpos, prot, prott, tbltype);
    ofs.x += twk->x;
    ofs.y += twk->y;
    ofs.z += twk->z;
    if (twk->flg & 0x1000) {
        SDF_VU0_LOADVECTOR(prot);
    } else {
        SDF_VU0_LOADVECTOR(prott);
    }
    ToMatrix();
    SDF_VU0_LOADVECTOR(&ofs);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(&ofs);
    hitpos.x += ofs.x;
    hitpos.y += ofs.y;
    hitpos.z += ofs.z;
    SDF_VU0_LOADVECTOR(&hitpos);
    SDF_VU0_STOREVECTOR(pos);
    SDF_VU0_LOADVECTOR(prot);
    SDF_VU0_STOREVECTOR(rot);
    return 1;
}

/* NON_MATCHING: 8 of 301 words match. Retail copies the angle table (sbtlSetHitPosTable) after the three memsets, this copies it at the declaration; registers differ from there. */
#ifdef NON_MATCHING
s32 sbtlSetHitPos(struct SBTLUNIT_WORK *wk, struct SBTLDMG_HITWORK *twk, struct SBTLEFF_WORK *ewk, struct SBTLSKILLHIT_POSWORK *pwk, sdfVector4_t *pos, sdfVector4_t *rot, sdfVector4_t *rott, u16 tbltype) {
    sdfVector4_t hpos;
    sdfVector4_t hrot;
    SBTLVISUAL_DATA vd;
    sdfVector4_t dir;
    sdfMatrix4_t mat;
    f32 deg[9] = { 0.0f, 45.0f, 90.0f, 135.0f, 180.0f, -135.0f, -90.0f, -45.0f, 0.0f };
    struct SBTLUNIT_WORK *unit;
    struct SBTLUNIT_WORK *found;
    s32 stock;
    s32 search;
    s32 set;

    set = 0;
    unit = wk;
    memset(&hpos, 0, sizeof(hpos));
    hpos.w = 1.0f;
    memset(&hrot, 0, sizeof(hrot));
    hrot.w = 1.0f;
    memset(&dir, 0, sizeof(dir));
    dir.z = -1.0f;
    stock = wk_sbtl->stackID[0];
    if (stock == wk_sbtl->summonIndex[0]) {
        stock = wk_sbtl->stackID[1];
    }
    switch (pwk->iti & 0xF) {
    case 0:
    case 1:
    case 4:
    case 5:
    case 8:
        unit = wk;
        set = 1;
        break;
    case 2:
        set = 1;
        search = 0;
        if (ewk != NULL) {
            if (ewk->BEDtype == 2 || ewk->BEDtype == 5) {
                unit = wk;
            } else {
                search = 1;
            }
        } else {
            search = 1;
        }
        if (search == 1) {
            unit = wk;
            found = sbtlSearchUnit_ID(wk_sbtl->unitList, twk->targetID);
            if (found != NULL) {
                unit = found;
            }
        }
        break;
    case 3:
        set = 0;
        unit = wk;
        SDF_VU0_LOADVECTOR(&ewk->pos);
        SDF_VU0_STOREVECTOR(&hpos);
        break;
    case 6:
        set = 1;
        if (tbltype & 0x1000) {
            unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
        } else if (wk_sbtl->field344 > 0) {
            unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[wk_sbtl->summonIndex[0]]);
        } else {
            unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
        }
        if (unit == NULL || !sbtlCheckDamageTarget(unit)) {
            unit = wk;
        }
        break;
    case 7:
        set = 1;
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[stock]);
        if (unit == NULL || !sbtlCheckDamageTarget(unit)) {
            unit = wk;
        }
        break;
    }
    if (set == 1) {
        if (pwk->iti & 0xF0) {
            sbtlGetUnitHelperPos(unit, pwk->iti_no);
            SDF_VU0_STOREVECTOR(&hpos);
            if (twk->flg & 0x8000) {
                sbtlGetUnitHelperMatrix(unit, pwk->iti_no);
                SDF_VU0_STOREMATRIX(&mat);
                sbtlGetQuatFromMatrix(&mat);
                SDF_VU0_STOREVECTOR(&hrot);
            } else {
                SDF_VU0_LOADVECTOR(&unit->rot);
                SDF_VU0_STOREVECTOR(&hrot);
            }
        } else if ((pwk->iti & 0xF) != 3) {
            SDF_VU0_LOADVECTOR(&unit->pos);
            SDF_VU0_STOREVECTOR(&hpos);
            SDF_VU0_LOADVECTOR(&unit->rot);
            SDF_VU0_STOREVECTOR(&hrot);
        }
    }
    sbtlGetCylinderData(unit, &vd);
    if (!(pwk->iti & 0xF0) && set == 1) {
        switch (pwk->takasa) {
        case 0:
            SDF_VU0_LOADVECTOR(&vd.center);
            SDF_VU0_STOREVECTOR(&hpos);
            hpos.y += vd.e_height * 0.5f;
            break;
        case 1:
            SDF_VU0_LOADVECTOR(&vd.center);
            SDF_VU0_STOREVECTOR(&hpos);
            break;
        case 2:
            SDF_VU0_LOADVECTOR(&vd.center);
            SDF_VU0_STOREVECTOR(&hpos);
            hpos.y -= vd.e_height * 0.5f;
            break;
        case 3:
            SDF_VU0_LOADVECTOR(&vd.center);
            SDF_VU0_STOREVECTOR(&hpos);
            hpos.y -= vd.e_height * 0.8f;
            break;
        case 5:
            SDF_VU0_LOADVECTOR(&vd.center);
            SDF_VU0_STOREVECTOR(&hpos);
            hpos.y = sbtlFree(hpos.x, hpos.z);
            break;
        }
    }
    sbtlGetRotateDeg(&hrot, twk->dir);
    SDF_VU0_STOREVECTOR(&hrot);
    if (pwk->muki != 8) {
        sbtlGetRotateDeg(&hrot, deg[pwk->muki]);
        ToMatrix();
        SDF_VU0_LOADVECTOR(&dir);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_SCALEVECTOR(vd.width * wk->visual[0]);
        SDF_VU0_LOADVECTOR_B(&hpos);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(&hpos);
    }
    if (pos != NULL) {
        SDF_VU0_LOADVECTOR(&hpos);
        SDF_VU0_STOREVECTOR(pos);
    }
    if (rot != NULL) {
        SDF_VU0_LOADVECTOR(&hrot);
        SDF_VU0_STOREVECTOR(rot);
    }
    if (rott != NULL) {
        SDF_VU0_LOADVECTOR(&unit->rot);
        SDF_VU0_STOREVECTOR(rott);
    }
    return 1;
}
#else
INCLUDE_RODATA(const s32, "sbtlDMG", sbtlSetHitPosTable);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetHitPos);
#endif

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlGetHit_OnlyEndPosTable);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlGetHit_OnlyEndPos);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlSetHitEndPosTable);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetHitEndPos);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCallMemAllocDirect2);

SBTLDMG_HITWORK *sbtlSearchHit_SerialID(SBTLDMG_HITLISTWORK *lwk, s32 serialID)
{
    SBTLDMG_HITWORK *wk = lwk->wk;

    if (wk == NULL || lwk->num == 0) {
        return NULL;
    }
    do {
        if (wk->serialID == serialID) {
            return wk;
        }
        wk = wk->next;
    } while (wk != NULL);
    return NULL;
}

s32 sbtlGetKaraburiNum(struct SBTLDMG_HITLISTWORK *lwk, struct SBTLDMG_HITWORK *hwk)
{
    struct SBTLDMG_HITWORK *wk = lwk->wk;
    s32 num = 0;
    SKLID skillid = hwk->skillid;
    s32 acterID = hwk->acterID;

    if (wk == NULL || lwk->num == 0) {
        return 0;
    }
    do {
        if (wk->skillid == skillid && wk->st == 1 && wk->fKaraburi == 1 && wk->acterID == acterID) {
            num++;
        }
        wk = wk->next;
    } while (wk != NULL);
    return num;
}

void UnsetKaraburiSub(struct SBTLDMG_HITLISTWORK *lwk, struct SBTLDMG_HITWORK *hwk)
{
    struct SBTLDMG_HITWORK *wk = lwk->wk;
    SKLID skillid = hwk->skillid;
    s32 acterID = hwk->acterID;

    if (wk == NULL || lwk->num == 0) {
        return;
    }
    do {
        if (wk->skillid == skillid && wk->acterID == acterID) {
            wk->fKaraburi = 0;
        }
        wk = wk->next;
    } while (wk != NULL);
}

/* NON_MATCHING: 32 of 71 words match. Block order: retail puts the sub call and the devilId switch out of line. */
#ifdef NON_MATCHING
void sbtlUnsetKaraburi(struct SBTLDMG_HITLISTWORK *lwk, struct SBTLDMG_HITWORK *hwk)
{
    SKLID skillid = hwk->skillid;
    struct SBTLUNIT_WORK *acter = sbtlSearchUnit_ID(wk_sbtl->unitList, hwk->acterID);

    if (acter == NULL) {
        UnsetKaraburiSub(lwk, hwk);
        return;
    }
    if (skillid >= SKLID_OFS_BUTURI) {
        if (skillid >= SKLID_ATK) {
            if (skillid == SKLID_ATK) {
                switch (acter->unit.devilId) {
                case 39:
                case 60:
                case 119:
                case 140:
                case 166:
                case 229:
                    hwk->fKaraburi = 0;
                    break;
                }
            }
        } else {
            UnsetKaraburiSub(lwk, hwk);
        }
    }
}
#else
INCLUDE_ASM(const s32, "sbtlDMG", sbtlUnsetKaraburi);
#endif

s32 sbtlClearHitC(struct SBTLDMG_HITLISTWORK *lwk, struct SBTLUNIT_WORK *wk)
{
    s32 i;
    struct SBTLDMG_HITWORK *hwk = lwk->wk;

    for (i = 0; i < lwk->num; i++, hwk = hwk->next) {
        if (wk->field1A0 == hwk->acterID && (hwk->flg & 1)) {
            hwk->st = 2;
            sbtlForceDelete_SerialID(wk_sbtl->effList, hwk->serialID);
        }
    }
    return 1;
}

void sbtlResetIdouPara(struct SBTLDMG_HITWORK *hwk)
{
    sdfVector4_t dir;

    memset(&dir, 0, sizeof(dir));
    dir.z = -1.0f;
    if (hwk->flg & 0x40000) {
        hwk->iwk.time = 0.0f;
        hwk->counter = hwk->starttime + 2;
        SDF_VU0_LOADVECTOR(&hwk->iwk.apos);
        SDF_VU0_STOREVECTOR(&hwk->pos);
    } else if (hwk->flg & 0x100) {
        hwk->counter = hwk->starttime + 2;
        SDF_VU0_LOADVECTOR(&hwk->rot);
        ToMatrix();
        SDF_VU0_LOADVECTOR(&dir);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_SCALEVECTOR(hwk->shatei);
        SDF_VU0_LOADVECTOR_B(&hwk->pos);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(&hwk->endpos);
        SDF_VU0_LOADVECTOR(&hwk->pos);
        SDF_VU0_STOREVECTOR(&hwk->iwk.apos);
        hwk->iwk.time = 0.0f;
        sbtlSetApos12(&hwk->pos, &hwk->endpos, &hwk->rot, hwk->hittbl, &hwk->iwk.apos0, &hwk->iwk.apos1);
    } else if (hwk->flg & 0x800) {
        hwk->counter = hwk->starttime;
    }
}

/* NON_MATCHING: 37 of 651 words match. The draft is 20 words shorter: retail calls sbtlResetHitPos_XYZ twice, once with the effect and once with NULL, where this passes the pointer; scheduling differs throughout. */
#ifdef NON_MATCHING
s32 sbtlHitCProc_Idou(struct SBTLDMG_HITLISTWORK *lwk) {
    sdfVector4_t mpos;
    sdfVector4_t a;
    sdfVector4_t tmp;
    sdfVector4_t dirv;
    sdfVector4_t p1;
    sdfVector4_t p2;
    sdfVector4_t epos;
    struct SBTLDMG_HITWORK *hwk;
    struct SBTLEFF_WORK *ewk;
    struct SBTLUNIT_WORK *unit;
    struct SBTLUNIT_WORK *target;
    s32 d;
    f32 kas;
    f32 len;

    memset(&a, 0, sizeof(a));
    a.w = 1.0f;
    if (wk_sbtl->hitList == NULL) {
        return 0;
    }
    hwk = lwk->wk;
    if (hwk == NULL || lwk->num == 0) {
        return 0;
    }
    do {
        if ((wk_sbtl->flg & 0x40) && !(hwk->flg & 0x80)) {
            if (hwk->flg & 0x1000) {
                hwk->hModel->first->speed = 0.0f;
            }
            goto next;
        }
        if (!(hwk->flg & 0x900)) {
            goto next;
        }
        if (hwk->st != 1) {
            goto next;
        }
        if (hwk->pathflg == 2) {
            hwk->pathflg = 1;
        }
        if (hwk->eListID != -1 && (hwk->flg & 0x40)) {
            ewk = sbtlGetEffectWork_ID(wk_sbtl->effList, hwk->eListID);
            if (ewk == NULL) {
                goto next;
            }
            if ((hwk->flg & 0x1000) && hwk->skillid == 444) {
                d = 2;
            } else {
                d = 1;
            }
            if (ewk->st == 2 && ewk->counter + d < ewk->starttime) {
                goto next;
            }
            if (ewk->st < 2 || ewk->st > 4) {
                goto next;
            }
            if (ewk->st == 2 && ewk->starttime == ewk->counter + d) {
                if (hwk->flg & 0x1000) {
                    mdlSmoothSingleMotion(hwk->hModel, 0, 0, 0.0f, 0.0f);
                    hwk->pathflg = 2;
                }
                unit = sbtlSearchUnit_ID(wk_sbtl->unitList, hwk->acterID);
                if (unit != NULL && sbtlCheckDamageTarget(unit)) {
                    sbtlResetHitPos_XYZ(unit, hwk, ewk, &hwk->start, &hwk->pos, &hwk->rot, hwk->type);
                }
            }
        }
        if (hwk->flg & 0x60000) {
            if (hwk->starttime + hwk->life == hwk->counter && hwk->skillid != 443) {
                sbtlResetIdouPara(hwk);
            }
        } else if (hwk->counter >= hwk->starttime + hwk->life) {
            goto next;
        }
        if (hwk->flg & 0x10000) {
            if (hwk->counter < hwk->starttime) {
                goto next;
            }
            if (hwk->starttime == hwk->counter || (hwk->starttime < hwk->counter && hwk->counter == 1)) {
                if (hwk->flg & 0x1000) {
                    mdlSmoothSingleMotion(hwk->hModel, 0, 0, 0.0f, 0.0f);
                    hwk->pathflg = 2;
                }
                unit = sbtlSearchUnit_ID(wk_sbtl->unitList, hwk->acterID);
                if (unit != NULL && sbtlCheckDamageTarget(unit)) {
                    ewk = sbtlGetEffectWork_ID(wk_sbtl->effList, hwk->eListID);
                    sbtlResetHitPos_XYZ(unit, hwk, ewk, &hwk->start, &hwk->pos, &hwk->rot, hwk->type);
                }
            }
        }
        if (hwk->skillid == 443) {
            SDF_VU0_LOADVECTOR(&hwk->pos);
            SDF_VU0_STOREVECTOR(&hwk->oldpos);
            /* Retail looks the target up and drops the result. */
            if (sbtlSearchUnit_ID(wk_sbtl->unitList, hwk->targetID) == NULL) {
                sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
            }
            unit = sbtlSearchUnit_ID(wk_sbtl->unitList, hwk->acterID);
            ewk = sbtlGetEffectWork_ID(wk_sbtl->effList, hwk->eListID);
            sbtlGetHit_OnlyEndPos(unit, hwk, ewk, hwk->hittbl, &hwk->start, &hwk->hittbl->end, &epos, hwk->type);
            SDF_VU0_LOADVECTOR(&epos);
            SDF_VU0_LOADVECTOR_B(&hwk->pos);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_NORMALIZE();
            SDF_VU0_SCALEVECTOR(hwk->sokudo);
            SDF_VU0_LOADVECTOR_B(&hwk->pos);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(&hwk->pos);
            sbtlGetQuat(&hwk->pos, &hwk->oldpos);
            SDF_VU0_STOREVECTOR(&hwk->rot);
            goto next;
        }
        if ((hwk->flg & 0x1000) && !(hwk->flg & 0x10040) && hwk->starttime == hwk->counter) {
            mdlSmoothSingleMotion(hwk->hModel, 0, 0, 0.0f, 0.0f);
            hwk->pathflg = 2;
        }
        if ((hwk->flg & 0x800) || (hwk->starttime == hwk->counter && hwk->pathflg == 0)) {
            unit = sbtlSearchUnit_ID(wk_sbtl->unitList, hwk->acterID);
            if (unit != NULL && unit->hModel != NULL) {
                ewk = sbtlGetEffectWork_ID(wk_sbtl->effList, hwk->eListID);
                sbtlResetHitPos_XYZ(unit, hwk, ewk, &hwk->start, &hwk->pos, &hwk->rot, hwk->type);
            }
        } else if (hwk->flg & 0x1000) {
            hwk->iwk.time += hwk->iwk.addtime;
            if (hwk->iwk.time >= 1.0f) {
                hwk->iwk.time = 1.0f;
            }
            if (hwk->iwk.time < 1.0f && hwk->hModel->first->stat != 5) {
                SDF_VU0_LOADVECTOR(&hwk->pos);
                SDF_VU0_STOREVECTOR(&tmp);
                SDF_VU0_STOREVECTOR(&hwk->oldpos);
                hwk->hModel->first->speed = 1.0f;
                sdfModelEvalTm(hwk->hModel->inner);
                sdfModelGetMarkPos_sdfModel(hwk->hModel->inner, 0);
                SDF_VU0_STOREVECTOR(&mpos);
                SDF_VU0_STOREVECTOR(&hwk->pos);
            }
        } else {
            kas = hwk->kasokudo;
            if (kas < 0.1f && kas > -0.1f) {
                hwk->spd = hwk->sokudo;
            } else if (kas > -0.1f) {
                hwk->spd += kas;
            } else {
                hwk->spd += kas;
                if (hwk->spd < 0.0f) {
                    hwk->spd = 0.0f;
                }
            }
            if (hwk->spd > 0.0f) {
                hwk->iwk.addtime = 1.0f / (hwk->shatei / hwk->spd);
            } else {
                hwk->iwk.addtime = 0.0f;
            }
            hwk->iwk.time += hwk->iwk.addtime;
            if (hwk->iwk.time >= 1.0f) {
                hwk->iwk.time = 1.0f;
            }
            if (hwk->iwk.time < 1.0f) {
                SDF_VU0_LOADVECTOR(&hwk->pos);
                SDF_VU0_STOREVECTOR(&tmp);
                SDF_VU0_STOREVECTOR(&a);
                SDF_VU0_STOREVECTOR(&hwk->oldpos);
                target = sbtlSearchUnit_ID(wk_sbtl->unitList, hwk->targetID);
                if ((hwk->flg & 0x400) && target != NULL) {
                    sbtlSetApos12(&hwk->pos, &hwk->endpos, &hwk->rot, hwk->hittbl, &hwk->iwk.apos0, &hwk->iwk.apos1);
                    SDF_VU0_LOADVECTOR(&target->pos);
                    SDF_VU0_LOADVECTOR_B(&hwk->iwk.apos0);
                    SDF_VU0_SUBVECTOR();
                    SDF_VU0_GETLENGTH(len);
                    SDF_VU0_NORMALIZE();
                    SDF_VU0_SCALEVECTOR(len * hwk->iwk.apos0_per * hwk->homingpara);
                    SDF_VU0_LOADVECTOR_B(&hwk->iwk.apos0);
                    SDF_VU0_ADDVECTOR();
                    SDF_VU0_STOREVECTOR(&p1);
                    SDF_VU0_LOADVECTOR(&target->pos);
                    SDF_VU0_LOADVECTOR_B(&hwk->iwk.apos1);
                    SDF_VU0_SUBVECTOR();
                    SDF_VU0_GETLENGTH(len);
                    SDF_VU0_NORMALIZE();
                    SDF_VU0_SCALEVECTOR(len * hwk->iwk.apos1_per * hwk->homingpara);
                    SDF_VU0_LOADVECTOR_B(&hwk->iwk.apos1);
                    SDF_VU0_ADDVECTOR();
                    SDF_VU0_STOREVECTOR(&p2);
                    unit = sbtlSearchUnit_ID(wk_sbtl->unitList, hwk->acterID);
                    ewk = sbtlGetEffectWork_ID(wk_sbtl->effList, hwk->eListID);
                    if (unit != NULL && ewk != NULL) {
                        sbtlHitCProc_IdouWork = hwk->pos.x;
                        sbtlGetHit_OnlyEndPos(unit, hwk, ewk, hwk->hittbl, &hwk->start, &hwk->hittbl->end, &epos, hwk->type);
                    } else {
                        SDF_VU0_LOADVECTOR(&target->pos);
                        SDF_VU0_STOREVECTOR(&epos);
                    }
                    SDF_VU0_LOADVECTOR(&epos);
                    SDF_VU0_LOADVECTOR_B(&hwk->endpos);
                    SDF_VU0_SUBVECTOR();
                    SDF_VU0_GETLENGTH(len);
                    SDF_VU0_NORMALIZE();
                    SDF_VU0_SCALEVECTOR(len * hwk->homingpara);
                    SDF_VU0_LOADVECTOR_B(&hwk->endpos);
                    SDF_VU0_ADDVECTOR();
                    SDF_VU0_STOREVECTOR(&epos);
                    a.x = sbtlGetBezier(hwk->iwk.apos.x, p1.x, p2.x, epos.x, hwk->iwk.time);
                    a.y = sbtlGetBezier(hwk->iwk.apos.y, p1.y, p2.y, epos.y, hwk->iwk.time);
                    a.z = sbtlGetBezier(hwk->iwk.apos.z, p1.z, p2.z, epos.z, hwk->iwk.time);
                } else {
                    a.x = sbtlGetBezier(hwk->iwk.apos.x, hwk->iwk.apos0.x, hwk->iwk.apos1.x, hwk->endpos.x, hwk->iwk.time);
                    a.y = sbtlGetBezier(hwk->iwk.apos.y, hwk->iwk.apos0.y, hwk->iwk.apos1.y, hwk->endpos.y, hwk->iwk.time);
                    a.z = sbtlGetBezier(hwk->iwk.apos.z, hwk->iwk.apos0.z, hwk->iwk.apos1.z, hwk->endpos.z, hwk->iwk.time);
                }
                SDF_VU0_LOADVECTOR(&a);
                SDF_VU0_STOREVECTOR(&hwk->pos);
                SDF_VU0_STOREVECTOR(&hwk->pos);
                SDF_VU0_LOADVECTOR_B(&hwk->oldpos);
                SDF_VU0_SUBVECTOR();
                SDF_VU0_GETLENGTH(len);
                SDF_VU0_NORMALIZE();
                SDF_VU0_STOREVECTOR(&dirv);
                if (hwk->spd <= len) {
                    SDF_VU0_LOADVECTOR(&dirv);
                    SDF_VU0_SCALEVECTOR(hwk->spd);
                    SDF_VU0_LOADVECTOR_B(&hwk->oldpos);
                    SDF_VU0_ADDVECTOR();
                    SDF_VU0_STOREVECTOR(&hwk->pos);
                }
            }
        }
        if (hwk->flg & 0x100) {
            sbtlGetQuat(&hwk->pos, &hwk->oldpos);
            SDF_VU0_STOREVECTOR(&hwk->rot);
        }
        if (hwk->flg & 0x1000) {
            SDF_VU0_LOADVECTOR(&hwk->pBasic->poseset->rot);
            SDF_VU0_STOREVECTOR(&hwk->rot);
        }
    next:
        hwk = hwk->next;
    } while (hwk != NULL);
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtlDMG", sbtlHitCProc_Idou);
#endif

struct SBTLDMG_HITWORK *sbtlCheckHitArea_Single(struct SBTLDMG_HITLISTWORK *lwk)
{
    struct SBTLDMG_HITWORK *hwk;
    struct SBTLUNIT_WORK *target;
    f32 save;

    if (wk_sbtl->hitList == NULL) {
        return NULL;
    }
    hwk = lwk->wk;
    if (hwk == NULL || lwk->num == 0) {
        return NULL;
    }
    do {
        if (hwk->st == 1 && hwk->starttime <= hwk->counter) {
            if (hwk->counter >= hwk->starttime + hwk->life) {
                hwk->st = 2;
            } else {
                target = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
                save = target->model->field11C;
                target->model->field11C = 200.0f;
                if (sbtlCheckHitArea_sbtlDMG(hwk, target)) {
                    target->model->field11C = save;
                    return hwk;
                }
                target->model->field11C = save;
            }
        }
        hwk = hwk->next;
    } while (hwk != NULL);
    return NULL;
}

s32 sbtlHitCProc(struct SBTLDMG_HITLISTWORK *lwk) {
    struct SBTLDMG_HITWORK *hwk;
    struct SBTLEFF_WORK *ewk;
    struct SBTLUNIT_WORK *unit;
    s32 hitflg;
    s32 ret;
    s32 i;
    s32 counter;

    if (wk_sbtl->hitList == NULL) {
        return 0;
    }
    hwk = lwk->wk;
    if (hwk == NULL) {
        return 0;
    }
    if (lwk->num == 0) {
        return 0;
    }
    do {
        if ((wk_sbtl->flg & 0x40) && !(hwk->flg & 0x80)) {
            goto next;
        }
        hwk->counter++;
        if (hwk->counter > 0) {
            counter = hwk->counter;
            if (counter > 0x7FFF) {
                counter = 0x7FFF;
            }
        } else {
            counter = 0;
        }
        hwk->counter = counter;
        if (hwk->st != 1) {
            goto next;
        }
        hitflg = 0;
        if (hwk->eListID != -1) {
            if (sbtlGetEffectStat_ID(wk_sbtl->effList, hwk->eListID) == -1) {
                if (!(hwk->flg & 0x200)) {
                    hwk->st = 2;
                    goto next;
                }
            } else if (hwk->flg & 0x40) {
                ewk = sbtlGetEffectWork_ID(wk_sbtl->effList, hwk->eListID);
                if (ewk != NULL && ewk->st < 3) {
                    hwk->counter = 0;
                    goto next;
                }
            }
        }
        if ((hwk->flg & 0x1000) && hwk->pathflg != 1) {
            goto next;
        }
        if (hwk->starttime > hwk->counter) {
            goto next;
        }
        if (hwk->counter >= hwk->starttime + hwk->life && !(hwk->flg & 0x60000)) {
            hwk->st = 2;
            goto next;
        }
        if (hwk->skillid == 443 && hwk->counter >= hwk->starttime + 900) {
            hwk->st = 2;
            goto next;
        }
        if ((hwk->skillid == 465 && hwk->counter >= hwk->starttime + 600) ||
            (hwk->skillid == 72 && hwk->counter >= hwk->starttime + 900)) {
            hwk->st = 2;
            sbtlSetDecAlpha_SerialID(wk_sbtl->effList, hwk->serialID, 4);
            sbtlForceDelete_SerialID(wk_sbtl->effList, hwk->serialID);
            goto next;
        }
        if (hwk->lwk == NULL) {
            sbtlCheckHitArea(hwk, hwk->skillid);
        }
        ret = 1;
        if (hwk->area & 1) {
            unit = sbtlCheckGround(hwk);
            if (unit != NULL) {
                ret = sbtlCallBtlForceDeleteSerialID(hwk, unit, &hitflg);
                sbtlUnsetKaraburi(lwk, hwk);
            }
            if (ret && (hwk->flg & 0x4000)) {
                ret = sbtlCheckGround_sbtlDMG(hwk, &hitflg);
            }
            if (!ret) {
                hwk->st = 2;
            }
        } else {
            for (i = 0; i < hwk->lwk->count;) {
                unit = hwk->lwk->pUnitL[i++].pUnit;
                if (unit != NULL && sbtlCheckHitArea_sbtlDMG(hwk, unit)) {
                    ret = sbtlCallBtlForceDeleteSerialID(hwk, unit, &hitflg);
                    sbtlUnsetKaraburi(lwk, hwk);
                }
                if (ret != 1) {
                    break;
                }
            }
            if (hwk->flg & 0x4000) {
                sbtlCheckGround_sbtlDMG(hwk, &hitflg);
            }
        }
        if (hwk->lwk != NULL) {
            sbtlDeleteUnitList(hwk->lwk);
            hwk->lwk = NULL;
        }
    next:
        hwk = hwk->next;
    } while (hwk != NULL);
    return 1;
}

s32 sbtlCheckActDamage(struct SBTLUNIT_WORK *wk, struct SBTLUNIT_WORK *wklock) {
    SBTLDMG_HITWORK hwk;
    struct SBTLUNIT_WORK *target;
    struct SBTLUNIT_WORK *acter;
    struct SBTLSKILLP_TBL *ptbl;
    s32 damage;
    s32 hp;

    if ((wk->field414 & 0x80) && (wklock->field414 & 2)) {
        target = wk;
        acter = wklock;
    } else if ((wklock->field414 & 0x80) && (wk->field414 & 2)) {
        target = wklock;
        acter = wk;
    } else {
        return 0;
    }
    if (!(acter->flg & 0x200)) {
        return 0;
    }
    if (target->field4A2 > 0 || acter->status == 7 || acter->status == 9) {
        return 0;
    }
    ptbl = sbtlGetSkilltbl(286);
    target->field4A2 = ptbl->muteki_time;
    SDF_VU0_LOADVECTOR(&acter->pos);
    SDF_VU0_STOREVECTOR(&hwk.pos);
    hwk.flg = 0;
    hwk.hitbk_cnt = ptbl->hitbk_cnt;
    damage = ptbl->max_damage;
    hwk.hitbk_rate = ptbl->hitbk_rate;
    if (target->flg & 0x100) {
        sbtlSetMukouDamage(target, 0);
        sbtlCallMemset16(target, &hwk, 0x1000);
        return 1;
    }
    hp = target->unit.now.hp - damage;
    target->unit.now.hp = hp;
    target->unit.now.hp = hp <= 0 ? 0 : (hp < target->unit.now.maxhp ? hp : target->unit.now.maxhp);
    sbtlCallBtlSetMotionSBtlUnit4(target, damage);
    sbtlCallMemset16(target, &hwk, 0);
    sbtlCallBtlSearchUnitID14(target, damage, 0, 0);
    sbtlCallBtlGetSklVisualTbl(target, hwk.skillid, hwk.type, 0, 0);
    return 1;
}

struct SBTLUNIT_WORK *sbtlCheckGround(struct SBTLDMG_HITWORK *hwk) {
    struct SBTLUNIT_WORK *unit;
    struct SBTLUNIT_WORK *near;
    s32 i;
    f32 min;
    f32 len;

    min = 9999.0f;
    near = NULL;
    for (i = 0; i < hwk->lwk->count; i++) {
        unit = hwk->lwk->pUnitL[i].pUnit;
        if (unit == NULL) {
            continue;
        }
        if (!sbtlCheckHitArea_sbtlDMG(hwk, unit)) {
            continue;
        }
        SDF_VU0_LOADVECTOR(&hwk->pos);
        SDF_VU0_LOADVECTOR_B(&unit->pos);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_GETLENGTH(len);
        if (len < min) {
            near = unit;
            min = len;
        }
    }
    return near;
}

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCheckGround_sbtlDMG);

s32 sbtlCallBtlForceDeleteSerialID(struct SBTLDMG_HITWORK *hwk, struct SBTLUNIT_WORK *wk, s32 *hitflg)
{
    sbtlCallBtlCheckALLSkill(wk, hwk, 0);
    *hitflg = 1;
    if (hwk->flg & 0x200) {
        sbtlForceDelete_SerialID(wk_sbtl->effList, hwk->serialID);
    }
    if (hwk->flg & 2) {
        hwk->st = 2;
        return 0;
    }
    return (hwk->area & 1) ^ 1;
}

s32 sbtlCallColorSet2(u32 color)
{
    struct SBTLDMG_HITLISTWORK *lwk = wk_sbtl->hitList;
    struct SBTLDMG_HITWORK *wk;

    if (lwk == NULL) {
        return 0;
    }
    wk = lwk->wk;
    if (wk == NULL || lwk->num == 0) {
        return 0;
    }
    do {
        if ((wk->flg & 0x2000) && wk->st == 1) {
            wk->abgr = wk->hPartsList->pose.abgr;
            ColorSet(wk->hPartsList, ((s32)wk->EPL_alpha << 24) | (color & 0xFFFFFF));
        }
        wk = wk->next;
    } while (wk != NULL);
    return 1;
}

s32 sbtlCallColorSet3(void)
{
    SBTLDMG_HITLISTWORK *lwk = wk_sbtl->hitList;
    struct SBTLDMG_HITWORK *wk;

    if (lwk == NULL) {
        return 0;
    }
    wk = lwk->wk;
    if (wk == NULL || lwk->num == 0) {
        return 0;
    }
    do {
        if ((wk->flg & 0x2000) && wk->st == 1) {
            ColorSet(wk->hPartsList, wk->abgr);
        }
        wk = wk->next;
    } while (wk != NULL);
    return 1;
}

s32 sbtlDrawAttachEPL(struct SBTLDMG_HITLISTWORK *lwk) {
    struct SBTLDMG_HITWORK *hwk;
    f32 cnt;
    f32 t;

    if (wk_sbtl->hitList == NULL) {
        return 0;
    }
    hwk = lwk->wk;
    if (hwk == NULL) {
        return 0;
    }
    if (lwk->num == 0) {
        return 0;
    }
    do {
        if (!(wk_sbtl->flg & 0x40) || (hwk->flg & 0x80)) {
            if (hwk->flg & 0x2000) {
                hwk->EPL_counter += 1.0f;
                cnt = hwk->EPL_counter <= 0.0f ? 0.0f : (hwk->EPL_counter >= 32767.0f ? 32767.0f : hwk->EPL_counter);
                hwk->EPL_counter = cnt;
                if (hwk->st == 1) {
                    if (!(hwk->EPL_st_time <= cnt)) {
                        hwk = hwk->next;
                        continue;
                    }
                    if (!(cnt <= hwk->EPL_st_time + hwk->EPL_life) && hwk->EPL_life != 0.0f) {
                        hwk = hwk->next;
                        continue;
                    }
                    t = cnt - hwk->EPL_st_time;
                    if (t >= 0.0f && t <= hwk->EPL_fin_time) {
                        hwk->EPL_alpha += hwk->EPL_in_alpha;
                        if (hwk->EPL_alpha >= 80.0f) {
                            hwk->EPL_alpha = 80.0f;
                        }
                    }
                    if (hwk->EPL_life != 0.0f && hwk->EPL_life - hwk->EPL_fout_time <= hwk->EPL_counter) {
                        hwk->EPL_alpha -= hwk->EPL_out_alpha;
                        if (hwk->EPL_alpha <= 0.0f) {
                            hwk->EPL_alpha = 0.0f;
                        }
                    }
                    ColorSet(hwk->hPartsList, ((s32)hwk->EPL_alpha << 24) | 0x808080);
                    effPartsRotateSet(hwk->hPartsList, (const f32 *)&hwk->rot);
                    effPartsPositionSet(hwk->hPartsList, (const f32 *)&hwk->pos);
                    effPartsDraw(hwk->hPartsList);
                }
            }
        }
        hwk = hwk->next;
    } while (hwk != NULL);
    return 1;
}

s32 sbtlReleaseHitC_List(struct SBTLDMG_HITLISTWORK *lwk, u8 flg) {
    struct SBTLDMG_HITWORK *hwk;
    struct SBTLDMG_HITWORK *next;

    if (wk_sbtl->hitList == NULL) {
        return 0;
    }
    hwk = lwk->wk;
    if (hwk == NULL || lwk->num == 0) {
        return 0;
    }
    do {
        if (flg) {
            hwk->st = 2;
        }
        if (hwk->st == 2) {
            if (hwk->prev != NULL) {
                hwk->prev->next = hwk->next;
            }
            if (hwk->next != NULL) {
                hwk->next->prev = hwk->prev;
            }
            if (hwk->prev == NULL) {
                lwk->wk = hwk->next;
                if (lwk->wk != NULL) {
                    lwk->wk->prev = NULL;
                }
            }
            next = hwk->next;
            if (hwk->lwk != NULL) {
                sbtlDeleteUnitList(hwk->lwk);
                hwk->lwk = NULL;
            }
            if (hwk->pBasic != NULL) {
                dds3DeleteSceneBasicData(hwk->pBasic);
                hwk->pBasic = NULL;
                hwk->hEveUnit = NULL;
            }
            if (hwk->hPartsList != NULL) {
                effPartsDelete(hwk->hPartsList);
                hwk->hPartsList = NULL;
            }
            if (hwk->hitlwk != NULL) {
                sbtlDeleteUnitList(hwk->hitlwk);
                hwk->hitlwk = NULL;
            }
            sdfMemFreeDirect(hwk->handle);
            hwk = next;
            lwk->num--;
        } else {
            hwk = hwk->next;
        }
    } while (hwk != NULL);
    return 1;
}

s32 sbtlDeleteHitList(struct SBTLDMG_HITLISTWORK *wk)
{
    sbtlReleaseHitC_List(wk, 1);
    sdfChipFreeDirect(wk);
    return 1;
}

struct SBTLUNIT_LISTWORK *sbtlCreateTargetList_Normal(struct SBTLUNIT_WORK *wk, SKLID skillid)
{
    struct SBTLUNIT_LISTWORK *lwk;
    struct SBTLSKILLP_TBL *ptbl;
    struct SBTLUNIT_WORK *unit;
    SBTLUNIT_SIDE side;
    s32 i;

    i = 0;
    ptbl = sbtlGetSkilltbl(skillid);
    side = sbtlGetSide(wk, ptbl->area, wk->field414, NULL);
    lwk = sbtlCreateUnitList(48);
    for (; i < wk_sbtl->unitList->number; i++) {
        unit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (unit == NULL) {
            continue;
        }
        if (!(side & unit->field414 & 0x3F)) {
            continue;
        }
        if (!sbtlCheckDamageTarget_Skilltbl(wk, unit, ptbl, skillid, 0)) {
            continue;
        }
        if (ptbl->area & 0x1000) {
            if (unit->field1A0 != wk->field1A0 && (!(unit->flg & 0x800) || wk->field1A0 != unit->field492)) {
                sbtlEntryUnitList(lwk, unit);
            }
        } else if (!(unit->vstatus & 0x10000)) {
            sbtlEntryUnitList(lwk, unit);
        }
    }
    return lwk;
}

s32 sbtlCheckTargetList_csr(struct SBTLUNIT_WORK *wk, SKLID skillid)
{
    struct SBTLUNIT_LISTWORK *lwk;
    struct SBTLSKILLP_TBL *ptbl;
    struct SBTLUNIT_WORK *unit;
    SBTLUNIT_SIDE side;
    s32 i;

    i = 0;
    ptbl = sbtlGetSkilltbl(skillid);
    side = sbtlGetSide(wk, ptbl->area, wk->field414, NULL);
    lwk = sbtlCreateUnitList(48);
    for (; i < wk_sbtl->unitList->number; i++) {
        unit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (unit == NULL) {
            continue;
        }
        if (!(side & unit->field414 & 0x3F)) {
            continue;
        }
        if (!sbtlCheckDamageTarget_Skilltbl(wk, unit, ptbl, skillid, 1)) {
            continue;
        }
        if (ptbl->area & 0x1000) {
            if (unit->field1A0 != wk->field1A0 && (!(unit->flg & 0x800) || wk->field1A0 != unit->field492)) {
                sbtlEntryUnitList(lwk, unit);
            }
        } else if (!(unit->vstatus & 0x10000)) {
            sbtlEntryUnitList(lwk, unit);
        }
    }
    if (lwk->count == 0) {
        sbtlDeleteUnitList(lwk);
        return 0;
    }
    sbtlDeleteUnitList(lwk);
    return 1;
}

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCreateTargetList_csr);

s32 sbtlCheckHitArea(struct SBTLDMG_HITWORK *wk, SKLID skillid) {
    struct SBTLUNIT_WORK *unit;
    BST bst;
    s32 i;

    wk->lwk = sbtlCreateUnitList(48);
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        unit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (unit == NULL) {
            continue;
        }
        if (!(unit->field414 & SBTLUNIT_SIDE_MASK & wk->side)) {
            continue;
        }
        if (!CheckDamageTarget(unit, wk, skillid)) {
            continue;
        }
        bst = sbtlGetBadStatus(&unit->unit);
        if ((wk->area & 0x1000) || bst == BST_BOUDO || bst == BST_MAHUU || wk->tchangeflg == 1) {
            if (wk->acterID == unit->field1A0) {
                continue;
            }
            if (wk->actID == unit->field1A0) {
                continue;
            }
            if ((unit->flg & 0x800) && wk->actID == unit->field492) {
                continue;
            }
            sbtlEntryUnitList(wk->lwk, unit);
        } else if (!(unit->vstatus & 0x10000)) {
            sbtlEntryUnitList(wk->lwk, unit);
        }
    }
    return 1;
}

s32 sbtlCallBtlGetSkilltbl7(struct SBTLUNIT_WORK *wk, SKLID skillid, u32 flg)
{
    struct SBTLSKILLP_TBL *ptbl = sbtlGetSkilltbl(skillid);

    if (wk->status != 9) {
        if (!(wk->flg & 0x100) || (ptbl->attr & 0x100)) {
            if (flg & 8) {
                if (ptbl->muteki_time > 0 && wk->field4A2 <= 0) {
                    wk->field4A2 = ptbl->muteki_time;
                }
            }
            return 1;
        }
    }
    return 0;
}

/* NON_MATCHING: 85 of 156 words match. The hittype tests and the return 0/1 tails are ordered differently (retail tests 1, below 2, 0, then 2). */
#ifdef NON_MATCHING
s32 sbtlCheckHitArea_sbtlDMG(struct SBTLDMG_HITWORK *hwk, struct SBTLUNIT_WORK *wk) {
    sdfVector4_t center;
    sdfVector4_t rot;
    sdfVector4_t pos1;
    sdfVector4_t pos2;
    sdfVector4_t pos3;
    sdfVector4_t pos4;
    sdfVector4_t upos;
    SBTLVISUAL_DATA vd;

    if (wk_sbtl->flg & 0x2000000) {
        return 0;
    }
    if (sbtlCheckPlayerDead()) {
        return 0;
    }
    if (wk->flg & 0x800) {
        SDF_VU0_LOADVECTOR(&wk->pos);
        SDF_VU0_STOREVECTOR(&upos);
    } else {
        sbtlGetCylinderData(wk, &vd);
        SDF_VU0_LOADVECTOR(&vd.center);
        SDF_VU0_STOREVECTOR(&upos);
    }
    switch (hwk->hittype) {
    case 1:
        if (sbtlCheckCrossCylinder_Circle(&hwk->pos, &hwk->rot, &hwk->cy, &upos, hwk->cy.area_r + wk->model->field11C)) {
            return 1;
        }
        return 0;
    case 2:
        if (!sbtlCheckCrossCylinder_Circle(&hwk->pos, &hwk->rot, &hwk->cy, &upos, hwk->cy.area_r + wk->model->field11C)) {
            return 0;
        }
        if (sbtlCheckCrossCylinder_Circle(&hwk->pos, &hwk->rot, &hwk->cy, &upos, hwk->area_r2)) {
            return 0;
        }
        return 1;
    case 0:
        if (hwk->flg & 8) {
            sbtlGetUnitHelperPos(wk, SBTLUNIT_MARK__1SHOT);
            SDF_VU0_STOREVECTOR(&center);
        } else {
            SDF_VU0_LOADVECTOR(&vd.center);
            SDF_VU0_STOREVECTOR(&center);
        }
        SDF_VU0_LOADVECTOR(&hwk->rot);
        SDF_VU0_STOREVECTOR(&rot);
        if (hwk->cy.height <= 0.0f) {
            hwk->cy.height = hwk->area_r;
        }
        SDF_VU0_LOADVECTOR(&hwk->pos);
        SDF_VU0_STOREVECTOR(&pos1);
        SDF_VU0_STOREVECTOR(&pos3);
        SDF_VU0_LOADVECTOR(&center);
        SDF_VU0_STOREVECTOR(&pos2);
        SDF_VU0_STOREVECTOR(&pos4);
        if (wk->flg & 0x800) {
            SDF_VU0_LOADVECTOR(&upos);
            SDF_VU0_STOREVECTOR(&pos2);
            SDF_VU0_STOREVECTOR(&pos4);
        }
        pos1.y = 0.0f;
        pos2.y = 0.0f;
        pos3.z = 0.0f;
        pos4.z = 0.0f;
        if (!sbtlCheckCrossCircle_Rot(&pos1, &rot, hwk->area_r, &pos2, wk->model->field11C, hwk->rad)) {
            return 0;
        }
        if (sbtlCheckCrossCircle_Rot(&pos3, &rot, hwk->cy.height, &pos4, wk->model->field11C, hwk->rad)) {
            return 1;
        }
        return 0;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "sbtlDMG", sbtlCheckHitArea_sbtlDMG);
#endif

s32 sbtlCHitFunc_ATTACK(struct SBTLUNIT_WORK *wk, struct SBTLEFF_WORK *ewk, struct SBTLDMG_HITWORK *twk, struct SBTLSKILL_TBL *tbl)
{
    f32 area_r = tbl->area_r;

    sbtlGetPosforDistance(wk, tbl->dis);
    SDF_VU0_STOREVECTOR(&twk->pos);
    twk->area_r = area_r;
    twk->side = sbtlGetSide(wk, tbl->area, twk->acterside, &twk->tchangeflg);
    twk->flg |= 1;
    return 1;
}

/* NON_MATCHING: 16 of 69 words match. Retail tests the field414 bits with andi and sltu where ee-gcc folds them to shifts, and recomputes acterside & 0xFF; argument registers differ from there. */
#ifdef NON_MATCHING
SBTLUNIT_SIDE sbtlGetSide(struct SBTLUNIT_WORK *wk, u16 area, u16 acterside, s8 *flg) {
    BST bst;
    s32 chk;
    u8 side;

    bst = sbtlGetBadStatus(&wk->unit);
    if (wk_sbtl->battleMode == 3) {
        chk = (wk->field414 & 2) != 0;
    } else {
        chk = (wk->field414 & 0x80) == 0;
    }
    side = acterside;
    if (chk == 1 && (u32)(bst - BST_BOUDO) < 2) {
        if (flg != NULL) {
            *flg = chk;
        }
        side = SBTLUNIT_SIDE_PLAYER;
        if (acterside & SBTLUNIT_SIDE_PLAYER) {
            side = SBTLUNIT_SIDE_ENEMY;
            if (wk->aiwk->bstside & 1) {
                *flg = 0;
                side = acterside;
            }
        }
    }
    if (area & 0x8000) {
        side = (side & SBTLUNIT_SIDE_PLAYER) ? SBTLUNIT_SIDE_BOBJ | SBTLUNIT_SIDE_ENEMY : SBTLUNIT_SIDE_PLAYER;
    } else if (area & 0x4000) {
        side = (side & SBTLUNIT_SIDE_PLAYER) ? SBTLUNIT_SIDE_PLAYER : SBTLUNIT_SIDE_ENEMY;
    } else if (area & 0xC000) {
        side = SBTLUNIT_SIDE_BOBJ | SBTLUNIT_SIDE_ENEMY | SBTLUNIT_SIDE_PLAYER;
    }
    return side;
}
#else
INCLUDE_ASM(const s32, "sbtlDMG", sbtlGetSide);
#endif

s32 CheckDamageTarget(struct SBTLUNIT_WORK *wk, struct SBTLDMG_HITWORK *hwk, SKLID skillid) {
    if ((hwk->area & 0x1000) && hwk != NULL && hwk->acterID == wk->field1A0) {
        return 0;
    }
    if (wk_sbtl->flg & 0x1000000) {
        return 0;
    }
    if (wk_sbtl->flg & 0x2000000) {
        return 0;
    }
    if (sbtlCheckPlayerDead()) {
        return 0;
    }
    if ((wk->field414 & 1) && (wk->field414 & 0x80) && wk->status == 8) {
        return 0;
    }
    switch (hwk->type & 0xFFF) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 9:
    case 12:
    case 13:
    case 14:
    case 15:
        if (sbtlCheckDamageTarget(wk) != 0 && sbtlCheckExist(wk) != 0 && !(wk->flg & 0x4001000)) {
            return 1;
        }
        return 0;
    case 0:
    case 6:
    case 7:
    case 8:
    case 10:
    case 11:
        return 0;
    }
    return 0;
}

s32 sbtlCheckPlayerDead(void) {
    struct SBTLUNIT_WORK *wk;

    wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    if (wk != NULL && (wk->status == 34 || wk->status == 10 || wk->status == 40)) {
        return 1;
    }
    return 0;
}

/* NON_MATCHING: 58 of 92 words match. gcc merges or folds the flg == 0 path's checks; retail keeps them as branches. */
#ifdef NON_MATCHING
s32 sbtlCheckDamageTarget_Skilltbl(struct SBTLUNIT_WORK *acter, struct SBTLUNIT_WORK *wk, struct SBTLSKILLP_TBL *ptbl, SKLID skillid, s32 flg)
{
    s32 ok;

    if ((ptbl->area & 0x1000) && acter->field1A0 == wk->field1A0) {
        return 0;
    }
    if (wk_sbtl->flg & 0x1000000) {
        return 0;
    }
    if (wk_sbtl->flg & 0x2000000) {
        return 0;
    }
    if (sbtlCheckPlayerDead()) {
        return 0;
    }
    if ((wk->field414 & 1) && (wk->field414 & 0x80) && wk->status == 8) {
        return 0;
    }
    switch (ptbl->type & 0xFFF) {
    case 0:
    case 6:
    case 7:
    case 8:
    case 10:
    case 11:
        return 0;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 12:
    case 13:
    case 14:
    case 15:
        if (!flg) {
            if (!sbtlCheckDamageTarget(wk)) {
                return 0;
            }
            if (sbtlCheckExist(wk)) {
                if (!(wk->flg & 0x4001000)) {
                    return 1;
                }
            }
            return 0;
        }
        ok = CheckLiveTarget(wk);
        break;
    case 9:
        ok = sbtlCheckDamageTarget(wk);
        break;
    default:
        return 0;
    }
    if (ok && sbtlCheckExist(wk) && !(wk->flg & 0x4001000)) {
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "sbtlDMG", sbtlCheckDamageTarget_Skilltbl);
#endif

s32 sbtlSearchDMG_Unit(struct SBTLDMG_HITWORK *hwk, struct SBTLUNIT_WORK *wk)
{
    SBTLUNIT_LISTWORK *hitlwk = hwk->hitlwk;
    s32 i;

    if (hitlwk == NULL) {
        return 0;
    }
    for (i = 0; i < 24; i++) {
        if (hitlwk->pUnitL[i].pUnit != NULL && hitlwk->pUnitL[i].pUnit->field1A0 == wk->field1A0) {
            return 0;
        }
    }
    return 1;
}

s32 sbtlSetEntryUnitList_Hit(struct SBTLDMG_HITWORK *hwk, struct SBTLUNIT_WORK *wklock)
{
    struct SBTLDMG_HITLISTWORK *lwk = wk_sbtl->hitList;
    struct SBTLDMG_HITWORK *wk;

    if (lwk == NULL) {
        return 0;
    }
    wk = lwk->wk;
    if (wk == NULL || lwk->num == 0) {
        return 0;
    }
    do {
        if (wk->st == 1 && hwk->serialID != wk->serialID && hwk->serialID2 == wk->serialID2) {
            sbtlEntryUnitList(wk->hitlwk, wklock);
        }
        wk = wk->next;
    } while (wk != NULL);
    return 1;
}

struct SBTLUNIT_WORK *sbtlCheckALLSkill(struct SBTLUNIT_WORK *wk, s32 mode)
{
    struct SBTLUNIT_WORK *target;
    s32 id;

    if (mode == 0x40 && (wk->flg & 0x8800) == 0x8800 && (id = ChangeTarget_BSHelper()) != -1) {
        target = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->field492);
        target->field494 = id;
    } else {
        target = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->field492);
        target->field494 = wk->field1A0;
    }
    return target;
}

/* NON_MATCHING: 12 of 54 words match. gcc folds the acterID compares into a set; retail branches. */
#ifdef NON_MATCHING
s32 sbtlDamageProc(struct SBTLDMG_HITWORK *hwk, struct SBTLUNIT_WORK *wk)
{
    struct SBTLUNIT_WORK *owner;
    s32 id;

    if (!(wk->flg & 0x1800)) {
        if (wk->field1A0 != hwk->acterID) {
            return 0;
        }
        return 1;
    }
    if ((wk->flg & 0x8800) == 0x8800) {
        owner = sbtlSearchUnit_ID(wk_sbtl->unitList, hwk->acterID);
        if (owner != NULL && (owner->flg & 0x8800) == 0x8800) {
            return 1;
        }
        id = wk->field492;
    } else {
        if (wk->flg & 0x1000) {
            owner = sbtlSearchUnit_ID(wk_sbtl->unitList, hwk->acterID);
            if (owner != NULL && (owner->flg & 0x8800) == 0x8800) {
                return 1;
            }
        }
        id = wk->field1A0;
    }
    if (id == hwk->acterID) {
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "sbtlDMG", sbtlDamageProc);
#endif

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCallBtlCheckALLSkill);

s32 sbtlSetDamageProc(struct SBTLDMG_HITWORK *hwk, u32 attr)
{
    s32 ret = 0;

    if (attr & 0x4000) {
        switch (hwk->skillid) {
        case 144:
        case 145:
        case 146:
        case 147:
        case 148:
        case 149:
        case 164:
        case 165:
        case 166:
            ret = 1;
            break;
        }
    }
    return (hwk->area & 0x400) ? 1 : ret;
}

#ifdef NON_MATCHING
/* 569 of 721 words differ; 144 bytes short. Retail retests flg & 1 on each
 * path and its register allocation differs from the start. Behavior is the
 * same. */
/* An attacker or target with field414 bit 0 that is not a stock demon (bit
 * 0x80) outside battle mode 3; in mode 3 bit 0 alone. */
static s32 IsViceUnit(struct SBTLUNIT_WORK *wk) {
    if (wk_sbtl->battleMode == 3) {
        return wk->field414 & 1;
    }
    return (wk->field414 & 1) && !(wk->field414 & 0x80);
}

/* The damage skillid from unit acterID does to wk, after status, buff and
 * vice modifiers, capped to the skill's maximum and 999, or -1 for a miss.
 * Applies wk's counter auto skill to the attacker on the way. */
s32 GetDamage(struct SBTLUNIT_WORK *wk, SKLID skillid, s32 acterID, struct SBTLDMG_HITWORK *hwk, s32 attr) {
    struct SBTLSKILLP_TBL *tbl;
    struct SBTLUNIT_WORK *acter;
    struct SBTLUNIT_WORK *unit;
    s32 damage;
    s32 roll;
    s32 type;
    s32 counter;
    s32 counterDamage;
    s32 hp;
    s32 charmHit;
    s32 guarded;
    BST bst;

    damage = 0;
    acter = NULL;
    tbl = sbtlGetSkilltbl(skillid);
    charmHit = 0;
    guarded = 0;
    if (tbl->type & 0xFFF) {
        acter = sbtlSearchUnit_ID(wk_sbtl->unitList, acterID);
        if (acter != NULL) {
            damage = sbtlCallBtlGetPlayerData4(acter, wk, hwk, skillid, attr);
        }
    }

    /* A confused target dodges or takes double. */
    if ((wk->unit.badStatus & BSTF_PANIC) && (tbl->attr & 1) && (tbl->flg & 1)) {
        roll = (s16)GetPercentage();
        if (wk->field414 & 2) {
            damage = roll < (s16)((f32)(10 - wk->unit.now.luck) * 0.7 + 25.0) ? -1 : damage * 2;
        } else {
            damage = roll < (s16)(((f32)(10 - wk->unit.now.luck) * 0.7 + 25.0) * 0.5) ? -1 : damage * 2;
        }
    }

    if (acter != NULL && sbtlGetBadStatus(&acter->unit) == BST_SLEEP && (tbl->type & 0xFFF)) {
        type = tbl->type & 0xFFF;
        if (type < 3 || (type < 15 && type >= 12)) {
            if ((tbl->flg & 1) && damage != -1 && !(tbl->flg & 0x200000)) {
                damage = (f32)damage * 1.5f;
            }
        }
    }
    if (wk != NULL && sbtlGetBadStatus(&wk->unit) == BST_SLEEP) {
        if ((tbl->flg & 1) && damage != -1 && !(tbl->flg & 0x200000)) {
            damage *= 2;
        }
    }

    if ((wk->unit.badStatus & BSTF_CHARM) && damage != -1) {
        if (tbl->attr & 1) {
            damage = (f32)damage * 1.5f;
            charmHit = 1;
            sbtlCreateSTP_Unit(wk_sbtl->stp, wk, SKLID_AGIDAIN, 0);
            sbtlSetRirituNakamaHD(wk_sbtl->expList, wk, skillid, acterID, 2);
            wk->field406 = charmHit;
        } else if (!(tbl->flg & 1)) {
            goto vice;
        }
        /* A hit may wake a charmed target. */
        roll = (s16)GetPercentage();
        if (wk->field414 & 2) {
            if (roll < (s16)((80.0 - (f32)(13 - wk->unit.now.luck) * 0.7) * 0.80999999999999995)) {
                sbtlResetBadStatus(wk, BSTF_CHARM);
            }
        } else {
            if (roll < (s16)(80.0 - (f32)(13 - wk->unit.now.luck) * 0.7)) {
                sbtlResetBadStatus(wk, BSTF_CHARM);
            }
        }
    } else if (wk->status == 9 && (wk->flg2 & 0x10) && (tbl->attr & 1)) {
        if (!(tbl->flg & 1)) {
            goto vice;
        }
        if (damage != -1) {
            damage = (damage * 3) >> 1;
            sbtlCreateSTP_Unit(wk_sbtl->stp, wk, SKLID_AGIDAIN, 0);
            charmHit = 1;
            sbtlSetRirituNakamaHD(wk_sbtl->expList, wk, skillid, acterID, 2);
            wk->field406 = charmHit;
            if (wk->unit.devilId == 0x123) {
                damage *= 4;
            } else if (wk->unit.devilId == 0x91) {
                damage *= 7;
            } else if (wk->unit.devilId == 0x104) {
                damage *= 6;
            }
        }
    }

    if ((tbl->flg & 1) && damage != -1) {
        type = tbl->type & 0xFFF;
        if (acter != NULL) {
            switch (type) {
            case 1:
            case 2:
            case 12:
            case 13:
            case 14:
                if (acter->field358 & 0x1000000) {
                    damage += (s32)((f32)damage * (acter->field35C / 100.0f));
                }
                if (acter->field34C & 0x1000000) {
                    damage += (s32)((f32)damage * (acter->field350 / 100.0f));
                }
                break;
            case 3:
            case 5:
            case 15:
                if ((acter->field34C & 0x2000000) && !(tbl->flg & 0x200000)) {
                    damage += (s32)((f32)damage * (acter->field350 / 100.0f));
                }
                break;
            }
        }
        if (wk->field34C & 0x4000000) {
            damage -= (s32)((f32)damage * (wk->field350 / 100.0f));
        }
        switch (type) {
        case 1:
        case 2:
        case 12:
        case 13:
        case 14:
            if (wk->field358 & 0x4000000) {
                damage -= (s32)((f32)damage * (wk->field35C / 100.0f));
            }
            break;
        case 3:
        case 5:
        case 15:
            if (wk->field358 & 0x2000000) {
                damage -= (s32)((f32)damage * (wk->field35C / 100.0f));
            }
            break;
        }
    }

vice:
    unit = sbtlSearchUnit_ID(wk_sbtl->unitList, acterID);
    if (unit != NULL && CheckLiveTarget(unit) && IsViceUnit(unit) && sbtlGetViceLevel(unit->unit.vice) >= 2) {
        if ((tbl->flg & 1) && damage != -1) {
            damage = (f32)damage * 1.19999993f;
        }
    }
    if (IsViceUnit(wk) && sbtlGetViceLevel(wk->unit.vice) >= 2) {
        if ((tbl->flg & 1) && damage != -1) {
            damage = (f32)damage * 0.79999996f;
        }
    }
    if ((tbl->flg & 1) && damage != -1 && attr != 0x2000 && (wk->flg & 0x100) && !(hwk->attr_flg & 0x300)) {
        guarded = 1;
        damage = (f32)damage * 0.099999995f;
    }

    if (damage < 0) {
        damage = -1;
    } else if (damage >= tbl->max_damage) {
        damage = tbl->max_damage;
    }
    if (tbl->flg & 1) {
        if (damage == 0) {
            damage = 1;
        }
        if (damage >= 999) {
            damage = 999;
        }
        damage = SDF_DIFFICULTY_DAMAGE(wk, damage);

        /* wk's counter auto skill hits the attacker back. */
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, acterID);
        if (unit != NULL && sbtlCheckDamageTarget(unit) && wk->unit.now.hp - damage >= 2 && !sbtlRefSbtl68(unit)) {
            counter = sbtlCallBtlGetSkilltbl21(sbtlGetAutoSkill_ID(&wk->unit, 0), &wk->unit.autost_cnt, &wk->unit.autost_cnt2);
            if (counter != 3) {
                counter = sbtlCallBtlGetSkilltbl21(sbtlGetAutoSkill_ID(&wk->unit, 1), &wk->unit.autost_cnt, &wk->unit.autost_cnt2);
            }
            if (sbtlGetDamageSub(counter) && counter == 3) {
                bst = sbtlGetBadStatus(&wk->unit);
                if ((bst == BST_STONE || bst == BST_MOUDOKU || bst == BST_SLEEP || bst == BST_RAND0 || bst == BST_BOYCOTT) &&
                    (s16)GetPercentage() < 80 && sbtlCheckAutoSkill_Action(wk, (struct SBTLUNIT_WORK *)hwk, attr) && !charmHit) {
                    counterDamage = sbtlCallBtlGetSkilltbl24(&unit->unit, counter, damage, skillid, wk->unit.autost_cnt, wk->unit.autost_cnt2);
                    if (counterDamage <= 0) {
                        counterDamage = 1;
                    }
                    if (sbtlGetDamageSub2(wk, 0xF3)) {
                        sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_SAKAURAMI, 0xF3);
                    } else {
                        sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_GYAKUGIRE, 0xF4);
                    }
                    counterDamage = sbtlCheckKuishibari(unit, counterDamage);
                    hp = unit->unit.now.hp - counterDamage;
                    unit->unit.now.hp = hp;
                    if (hp <= 0) {
                        unit->unit.now.hp = 0;
                    } else if (hp >= unit->unit.now.maxhp) {
                        unit->unit.now.hp = unit->unit.now.maxhp;
                    } else {
                        unit->unit.now.hp = hp;
                    }
                    sbtlCallBtlGetSklVisualTbl(unit, 0x95, 1, 0, 0);
                    sbtlCallBtlSearchUnitID14(unit, counterDamage, 0, 0);
                    if (unit->flg & 0x40000000) {
                        sbtlSetSpecialDamage_DamageOnly(unit);
                    } else if (unit->unit.now.hp <= 0) {
                        sbtlSetDeadDamage(unit, 0);
                    }
                }
            }
        }
        if ((tbl->flg & 1) && attr != 0x2000) {
            damage = sbtlCheckKuishibari(wk, damage);
        }
    }

    if (!(tbl->flg & 4) && !guarded && attr != 0x2000) {
        if (charmHit) {
            sbtlPlayDamageSE(&wk->pos, 3);
        } else if (!((wk->unit.devilId == 0x107 || wk->unit.devilId == 0x12E) && wk->status == 7)) {
            sbtlPlayDamageSE(&wk->pos, 2);
        }
    }
    return damage;
}
#else
INCLUDE_RODATA(const s32, "sbtlDMG", GetDamageTable);

INCLUDE_RODATA(const s32, "sbtlDMG", GetDamageTable2);

INCLUDE_RODATA(const s32, "sbtlDMG", GetDamageTable3);

INCLUDE_RODATA(const s32, "sbtlDMG", GetDamageTable4);

INCLUDE_RODATA(const s32, "sbtlDMG", GetDamageStrFfffff);

INCLUDE_ASM(const s32, "sbtlDMG", GetDamage);
#endif

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetDamage);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCallBtlSearchUnitID15);

s32 sbtlCallMemset15(struct SBTLUNIT_WORK *wk, struct SBTLDMG_HITWORK *hwk)
{
    struct SBTLUNIT_WORK *acter;
    sdfVector4_t pos;
    s32 type;
    f32 deg;

    memset(&pos, 0, sizeof(pos));
    pos.w = 1.0f;
    type = hwk->type & 0xFFF;
    if ((type == 2 || type == 14) && (hwk->area & 8)) {
        acter = sbtlSearchUnit_ID(wk_sbtl->unitList, hwk->acterID);
        if (acter != NULL && sbtlCheckDamageTarget(acter)) {
            SDF_VU0_LOADVECTOR(&acter->pos);
            SDF_VU0_STOREVECTOR(&pos);
        } else {
            pos.x = hwk->pos.x;
            pos.y = hwk->pos.y;
            pos.z = hwk->pos.z;
        }
        deg = sbtlGetChorikiDeg(&wk->pos, &wk->rot, &pos, 1.0f);
        SDF_VU0_STOREVECTOR(&wk->rot);
    } else {
        if (hwk->skillid == 287) {
            pos.x = hwk->pos.x;
            pos.y = hwk->pos.y;
            pos.z = hwk->pos.z;
        } else {
            pos.x = hwk->acterpos[0];
            pos.y = hwk->acterpos[1];
            pos.z = hwk->acterpos[2];
        }
        deg = sbtlGetChorikiDeg(&wk->pos, &wk->rot, &pos, 1.0f);
        SDF_VU0_STOREVECTOR(&wk->rot);
    }
    sbtlAddUnitDeg(wk, deg);
    return 1;
}

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCallMemset16);

s32 sbtlSetStoneDeadDamageProc(struct SBTLUNIT_WORK *wk, struct SBTLUNIT_WORK *wklock, struct SBTLDMG_HITWORK *hwk, s32 damage, u32 attr)
{
    sdfVector4_t head;
    s32 time;

    if ((attr & 0x4000) || wk->status == 9) {
        if (wk->status != 9) {
            if (!(wk->flg & 0x2000)) {
                sbtlCallMemset15(wk, hwk);
            }
            sbtlGetUnitHelperPos(wk, SBTLUNIT_MARK_HEAD);
            SDF_VU0_STOREVECTOR(&head);
        }
    }
    sbtlSetStoneDeadDamage(wk, damage);
    time = sbtlGetStartTime_Damage();
    if (hwk->workID == 288) {
        hwk->skillid = hwk->workID;
    }
    if (!(wklock->field414 & 7)) {
        return 1;
    }
    if (hwk->BEDtype == 2) {
        wk_sbtl->huka2hwk = hwk;
        sbtlCheckEffectData(wklock, hwk->skillid, 5, 0, hwk->acterside, hwk->serialID2, time);
        return 1;
    }
    switch (hwk->type & 0xFFF) {
    case 1:
    case 2:
    case 4:
    case 12:
    case 13:
    case 14:
        wk_sbtl->huka2hwk = hwk;
        sbtlCheckEffectData(wklock, hwk->skillid, 2, 0, hwk->acterside, hwk->serialID2, time);
        break;
    }
    return 1;
}

void sbtlSetDeadWeak(struct SBTLUNIT_WORK *wk, struct SBTLSKILLP_TBL *ptbl, u32 attr)
{
    if (wk->status != 9 && (wk->field414 & 2) && (attr & 0x4000)) {
        wk->flg2 |= 0x10;
        wk->aiwk->scrflag |= 0x400000;
        sbtlSetWeakBit(wk->unit.devilId);
        sbtlCreateSTP_Unit(wk_sbtl->stp, wk, SBTLSTP_ID_WEAK, ptbl->attrtime);
    }
}

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetDeadDamageProc);

void sbtlSetMukouDamage(struct SBTLUNIT_WORK *wk, s32 flg)
{
    s32 starttime = sbtlGetStartTime_Damage();

    if (flg == 0x1000) {
        sbtlCallMemset17(wk_sbtl->effList, wk, EFFID_MUKOU, starttime, &wk->pos.x, 0x80808080, 0);
    } else {
        sbtlCallMemset17(wk_sbtl->effList, wk, EFFID_GUARD, starttime, &wk->pos.x, 0x80808080, 0);
    }
}

void sbtlCallGetDamage(struct SBTLUNIT_WORK *wk, struct SBTLDMG_HITWORK *hwk)
{
    s32 damage;
    s32 starttime;

    if (!(hwk->skillflg & 1)) {
        return;
    }
    if (hwk->BEDtype == 2) {
        damage = GetDamage(wk, hwk->skillid, hwk->awk.aListID, hwk, 0x2000);
    } else {
        damage = GetDamage(wk, hwk->skillid, hwk->acterID, hwk, 0x2000);
    }
    wk->unit.now.hp += damage;
    wk->unit.now.hp = wk->unit.now.hp <= 0                  ? 0
                      : wk->unit.now.hp < wk->unit.now.maxhp ? wk->unit.now.hp
                                                            : wk->unit.now.maxhp;
    sbtlCallBtlSearchUnitID14(wk, damage, 0, 3);
    starttime = sbtlGetStartTime_Damage();
    sbtlCallMemset17(wk_sbtl->effList, wk, EFFID_KYUSHU, starttime, &wk->pos.x, 0x80808080, 0);
}

void sbtlCallBtlSetMotionSBtlUnit3(struct SBTLUNIT_WORK *wk, s32 damage)
{
    if (wk_sbtl->battleMode != 3 && wk->field1A0 == wk_sbtl->targetUnitID && !(wk->flg2 & 0x20)) {
        sbtlSetMotion_sbtlUnit(wk, 0, 0, 0, 0);
        sbtlSetAction(wk, 1, 0);
    }
    sbtlCallBtlSearchUnitID14(wk, damage, 0, 0);
    if ((wk->field414 & 0x80) || ((wk->field414 & 1) && wk_sbtl->battleMode == 3)) {
        sbtlSetVibe(SBTLPLAYER_VTYPE_ONLYD);
    }
    if ((wk->field414 & 0x80) || ((wk->field414 & 1) && wk_sbtl->battleMode == 3)) {
        if (!sbtlResetJump()) {
            dds3SetQuake(20, 40);
        }
    }
}

void sbtlCallBtlSetMotionSBtlUnit4(struct SBTLUNIT_WORK *wk, s32 damage)
{
    sbtlSetMotion_sbtlUnit(wk, 1, 0, 0, 0);
    sbtlSetAction(wk, 7, 0);
    sbtlCallBtlSearchUnitID14(wk, damage, 0, 0);
    sbtlClearHitC(wk_sbtl->hitList, wk);
    sbtlForceReset(wk_sbtl->effList, wk);
    wk->model->flags &= ~0x20;
    wk->field474 = 0;
    wk->flg |= 0x20;
    wk->vstatus |= 0x80;
    if ((wk->field414 & 0x80) || ((wk->field414 & 1) && wk_sbtl->battleMode == 3)) {
        sbtlSetVibe(SBTLPLAYER_VTYPE_NORMALD);
    }
    if ((wk->field414 & 0x80) || ((wk->field414 & 1) && wk_sbtl->battleMode == 3)) {
        if (!sbtlResetJump()) {
            dds3SetQuake(20, 40);
        }
    }
}

void sbtlSetHuttobiDamage(struct SBTLUNIT_WORK *wk, struct SBTLDMG_HITWORK *hwk, s32 damage, u32 attr)
{
    sdfVector4_t zero;

    memset(&zero, 0, sizeof(zero));
    if (wk->status != 9) {
        if (attr & 0x4000) {
            sbtlSetMotion_sbtlUnit(wk, 14, 0, 0, 0);
            sbtlCreateDvltk_Priority(wk_sbtl->dvltkList, wk, SBTLDVLTK_MODE_DAMAGE);
        } else if (!(attr & 0x40000)) {
            sbtlSetMotion_sbtlUnit(wk, 14, 0, 0, 0);
            sbtlCreateDvltk_Priority(wk_sbtl->dvltkList, wk, SBTLDVLTK_MODE_DAMAGE);
        }
        sbtlSetAction(wk, 8, 0);
    }
    sbtlClearHitC(wk_sbtl->hitList, wk);
    sbtlForceReset(wk_sbtl->effList, wk);
    wk->huttobiID = hwk->acterID;
    sbtlCallBtlSearchUnitID14(wk, damage, 0, 0);
    wk->flg |= 0x20;
    wk->field474 = 0;
    wk->vstatus |= 0x80;
    SDF_VU0_LOADVECTOR(&zero);
    SDF_VU0_STOREVECTOR(&wk->mv_spd);
    wk->counter = 0;
    if ((wk->field414 & 0x80) || ((wk->field414 & 1) && wk_sbtl->battleMode == 3)) {
        sbtlSetVibe(SBTLPLAYER_VTYPE_HUTTOBI);
    }
    if ((wk->field414 & 0x80) || ((wk->field414 & 1) && wk_sbtl->battleMode == 3)) {
        if (!sbtlResetJump()) {
            dds3SetQuake(25, 65);
        }
    }
}

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetDyingDamage);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetStoneDeadDamage);

s32 sbtlCallBtlGetAutoSkillID(struct SBTLUNIT_WORK *wk)
{
    if (!sbtlCallBtlGetSkilltbl21(sbtlGetAutoSkill_ID(&wk->unit, 0), &wk->unit.autost_cnt, &wk->unit.autost_cnt2) ||
        !sbtlCallBtlGetSkilltbl21(sbtlGetAutoSkill_ID(&wk->unit, 1), &wk->unit.autost_cnt, &wk->unit.autost_cnt2)) {
        if ((s16)GetPercentage() < 80) {
            if (wk->field414 & 2) {
                sbtlSetUseSkill_sbtlActAI(wk, SKLID_OFS_AUTO, 0);
                wk->model->stat = 1;
                wk->aiwk->stat = 2;
                wk->model->mode = 14;
            } else {
                sbtlSetSkillData_AI(wk, SKLID_OFS_AUTO);
                sbtlSetUseSkill_sbtlActAI(wk, SKLID_OFS_AUTO, 0);
            }
            return 0;
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetDeadDamage);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCallBtlSetDamage);

/* NON_MATCHING: 15 of 81 words match. Retail sets the first call's arguments before the scrStep test. */
#ifdef NON_MATCHING
void sbtlSetSpecialDamage_DamageOnly(struct SBTLUNIT_WORK *wk)
{
    s32 step;
    s32 hp;
    s16 setno;
    s32 scrStep;

    if (wk->unit.devilId != 263 && wk->unit.devilId != 302) {
        return;
    }
    if (wk->unit.now.hp > 0) {
        return;
    }
    sbtlSetAction(wk, 7, 0);
    scrStep = wk->aiwk->scrStep;
    if (scrStep == 0) {
        sbtlSetMotion_sbtlUnit(wk, 1, 4, 0, 0);
        setno = 10;
        hp = wk->unit.now.maxhp;
        step = 1;
    } else {
        if (scrStep != 1) {
            sbtlSetMotion_sbtlUnit(wk, 13, 4, 0, 0);
            wk->flg |= 0x4000;
            wk->aiwk->scrStep = 3;
            goto clear;
        }
        sbtlSetMotion_sbtlUnit(wk, 1, 4, 0, 0);
        setno = 11;
        hp = wk->unit.now.maxhp;
        step = 2;
    }
    wk->unit.now.hp = hp;
    wk->motion_setno = setno;
    wk->aiwk->scrStep = step;
clear:
    sbtlClearHitC(wk_sbtl->hitList, wk);
    sbtlForceReset(wk_sbtl->effList, wk);
    wk->model->flags &= ~0x20;
    wk->field474 = 0;
    wk->flg |= 0x20;
    wk->vstatus |= 0x80;
}
#else
INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetSpecialDamage_DamageOnly);
#endif

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1EfSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1EeSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1EdSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1EcSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1EbSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1EaSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1E9Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1E8Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1E7Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1E6Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1E5Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1E4Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1E3Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1E2Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1E1Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1E0Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1DfSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1DeSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1DdSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1DcSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1DbSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1DaSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1D9Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1D8Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1D7Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1D6Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1D5Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1D4Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1D3Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1D2Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1D1Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1D0Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1CfSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1CeSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1CdSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1CcSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1CbSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1CaSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1C9Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1C8Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1C7Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1C6Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1C5Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1C4Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1C3Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1C2Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1C1Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1C0Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1BfSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1BeSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1BdSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1BcSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1BbSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1BaSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1B9Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1B8Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1B7Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1B6Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1B5Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1B4Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1B3Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1B2Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1B1Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1B0Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1AfSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1AeSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1AdSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1AcSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1AbSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1AaSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1A9Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1A8Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1A7Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1A6Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1A5Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1A4Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1A3Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1A2Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1A1Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1A0Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed19FSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed19ESab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed19DSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed19CSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed19BSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed19ASab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed199Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed198Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed197Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed196Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed195Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed194Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed193Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed192Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed191Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed190Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed18FSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed18ESab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed18DSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed18CSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed18BSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed18ASab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed189Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed188Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed187Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed186Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed185Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed184Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed183Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed182Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed181Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed180Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed17FSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed17ESab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed17DSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed17CSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed17BSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed17ASab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed179Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed178Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed177Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed176Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed175Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed174Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed173Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed172Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed171Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed170Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed16FSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed16ESab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed16DSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed16CSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed16BSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed16ASab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed169Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed168Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed167Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed166Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed165Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed164Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed163Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed162Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed161Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed160Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed15FSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed15ESab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed15DSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed15CSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed15BSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed15ASab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed159Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed158Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed157Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed156Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed155Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed154Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed153Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed152Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed151Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed150Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed14FSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed14ESab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed14DSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed14CSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed14BSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed14ASab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed149Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed148Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed147Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed146Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed145Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed144Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed143Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed142Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed141Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed140Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed13FSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed13ESab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed13DSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed13CSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed13BSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed13ASab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed139Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed138Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed137Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed136Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed135Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobedOtherSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobedPsySab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobedMudoSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobedDiaSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobedZanSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobedJioSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobedBufuSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobedAgiSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobedAngrySab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobedCloseSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobedCharmSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobedPanicSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobedSleepSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobedPoisonSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobedBindSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobedFaintSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobedWindSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobedShockSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobedFreezeSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobedFireSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobedTestSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobedFuNgSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed11ESab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed11DSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobedFuOkSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobedFuStartSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed11ASab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed119Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed118Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed117Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed116Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed115Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed114Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed113Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed112Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed111Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed110Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed10FSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed10ESab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed10DSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed10CSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed10BSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed10ASab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed109Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed108Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed107Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed106Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed105Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed104Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed103Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed102Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed101Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed100Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0FfSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0FeSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0FdSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0FcSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0FbSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0FaSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0F9Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0F8Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0F7Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0F6Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0F5Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0F4Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0F3Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0F2Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0F1Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0F0Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0EfSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0EeSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0EdSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0EcSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0EbSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0EaSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0E9Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0E8Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0E7Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0E6Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0E5Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0E4Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0E3Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0E2Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0E1Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0E0Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0DfSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0DeSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0DdSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0DcSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0DbSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0DaSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0D9Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0D8Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0D7Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0D6Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0D5Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0D4Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0D3Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0D2Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0D1Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0D0Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0CfSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0CeSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0CdSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0CcSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0CbSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0CaSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0C9Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0C8Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0C7Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0C6Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0C5Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0C4Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0C3Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0C2Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0C1Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0C0Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0BfSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0BeSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0BdSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0BcSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0BbSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0BaSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0B9Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0B8Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0B7Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0B6Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0B5Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0B4Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0B3Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0B2Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0B1Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0B0Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0AfSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0AeSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0AdSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0AcSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0AbSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0AaSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0A9Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0A8Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0A7Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0A6Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0A5Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0A4Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0A3Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0A2Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0A1Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed0A0Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed09FSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed09ESab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed09DSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed09CSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed09BSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed09ASab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed099Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed098Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed097Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed096Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed095Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed094Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed093Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed092Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed091Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed090Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed08FSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed08ESab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed08DSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed08CSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed08BSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed08ASab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed089Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed088Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed087Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed086Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed085Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed084Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed083Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed082Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed081Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed080Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed07FSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed07ESab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed07DSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed07CSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed07BSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed07ASab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed079Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed078Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed077Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed076Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed075Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed074Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed073Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed072Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed071Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed070Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed06FSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed06ESab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed06DSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed06CSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed06BSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed06ASab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed069Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed068Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed067Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed066Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed065Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed064Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed063Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed062Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed061Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed060Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed05FSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed05ESab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed05DSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed05CSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed05BSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed05ASab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed059Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed058Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed057Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed056Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed055Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed054Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed053Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed052Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed051Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed050Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed04FSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed04ESab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed04DSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed04CSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed04BSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed04ASab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed049Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed048Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed047Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed046Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed045Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed044Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed043Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed042Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed041Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed040Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed03FSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed03ESab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed03DSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed03CSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed03BSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed03ASab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed039Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed038Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed037Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed036Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed035Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed034Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed033Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed032Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed031Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed030Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed02FSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed02ESab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed02DSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed02CSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed02BSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed02ASab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed029Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed028Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed027Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed026Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed025Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed024Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed023Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed022Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed021Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed020Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed01FSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed01ESab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed01DSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed01CSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed01BSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed01ASab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed019Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed018Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed017Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed016Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed015Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed014Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed013Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed012Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed011Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed010Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobedMapAttachSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobedMap1110Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobedAttachSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed1110Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed00BSab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed00ASab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed009Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed008Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed007Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed006Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed005Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed004Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed003Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed002Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrSobed001Sab);

INCLUDE_RODATA(const s32, "sbtlDMG", rodatStrBattleOneplConbAuraEpl);

INCLUDE_RODATA(const s32, "sbtlDMG", rodatStrBattleOneplEncountEpl);

INCLUDE_RODATA(const s32, "sbtlDMG", rodatStrBattleOneplDamYukaEpl);

INCLUDE_RODATA(const s32, "sbtlDMG", rodatStrBattleOneplSummonNoEpl);

INCLUDE_RODATA(const s32, "sbtlDMG", rodatStrBattleOneplShoukanEEpl);

INCLUDE_RODATA(const s32, "sbtlDMG", rodatStrBattleOneplMikDam2Epl);

INCLUDE_RODATA(const s32, "sbtlDMG", rodatStrBattleOneplAbsorbEpl);

INCLUDE_RODATA(const s32, "sbtlDMG", rodatStrBattleOneplZanSlipEpl);

INCLUDE_RODATA(const s32, "sbtlDMG", rodatStrBattleOneplJioSlipEpl);

INCLUDE_RODATA(const s32, "sbtlDMG", rodatStrBattleOneplBufSlipEpl);

INCLUDE_RODATA(const s32, "sbtlDMG", rodatStrBattleOneplAgiSlipEpl);

INCLUDE_RODATA(const s32, "sbtlDMG", rodatStrBattleOneplAutorecovEpl);

INCLUDE_RODATA(const s32, "sbtlDMG", rodatStrBattleOneplDead02Epl);

INCLUDE_RODATA(const s32, "sbtlDMG", rodatStrBattleOneplDead01Epl);

INCLUDE_RODATA(const s32, "sbtlDMG", rodatStrBattleOneplDamageR21Epl);

INCLUDE_RODATA(const s32, "sbtlDMG", rodatStrBattleOneplInvalidEpl);

INCLUDE_RODATA(const s32, "sbtlDMG", rodatStrBattleOneplDead00Epl);

INCLUDE_RODATA(const s32, "sbtlDMG", rodatStrBattleOneplGuardEpl);

INCLUDE_RODATA(const s32, "sbtlDMG", rodatStrBattleOneplKaitenEpl);

INCLUDE_RODATA(const s32, "sbtlDMG", rodatStrBattleOneplDeadYuka2Epl);

INCLUDE_RODATA(const s32, "sbtlDMG", rodatStrBattleOneplStonedeadEpl);

INCLUDE_RODATA(const s32, "sbtlDMG", rodatStrBattleOneplDeadYuka1Epl);

INCLUDE_RODATA(const s32, "sbtlDMG", rodatStrBattleOneplDeadYukaEpl);

INCLUDE_RODATA(const s32, "sbtlDMG", rodatStrBattleOneplDamageR2Epl);

INCLUDE_RODATA(const s32, "sbtlDMG", rodatStrBattleOneplShoukanEpl);

INCLUDE_RODATA(const s32, "sbtlDMG", rodatStrBattleOneplDamYuka2Epl);

INCLUDE_RODATA(const s32, "sbtlDMG", rodatStrBattleOneplDamYuka1Epl);

