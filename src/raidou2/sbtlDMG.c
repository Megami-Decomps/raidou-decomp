#include "common.h"
#include "sbtlWork.h"
#include "sBtlSM.h"
#include "sdfChip.h"
#include "code_r1_0021CE60.h"
#include "sdfQuat.h"
#include "sdk_libc.h"
#include "dds3SceneBasic.h"
#include "sBtlUnit.h"
#include "sBtlCalc.h"
#include "sbtlDMG.h"
#include "sbtlSub.h"
#include "sBtlSkill.h"
#include "sBtlAI.h"
#include "sbtlActAI.h"
#include "sBtlDvltk.h"
#include "code_r1_0010DF68.h"
#include "sBtlEff.h"
#include "newdata.h"
#include "dds3ObjectBase.h"
#include "ps2asm.h"
#include "dds3KernelDraw.h"
#include "sBtlPlayer.h"
#include "EventBit.h"
#include "sdfModel.h"
#include "sBtlCmndMenuNew.h"
#include "event.h"
#include "code_002F5CF0.h"
#include "sBtlSound.h"
#include "code_0027A1E8.h"
#include "sdfMath.h"

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCalcInOutPara);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlGetPlayerLuck);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlGetPlayerLuckStep);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlGetCorrectionParamLuck);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlPurificationPlayerLuck);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetPlayerLuck);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlITMUNKUIUpdown);

/* NON_MATCHING: 37 of 45 words match.  */
#ifdef NON_MATCHING
void datSetKillDevil(struct SBTLUNIT_WORK *wk) {
    switch (wk->unit.devilId) {
    case 388:
        evtBitOn(2412);
        evtBitOn(2404);
        break;
    case 389:
        evtBitOn(2413);
        evtBitOn(2405);
        break;
    case 390:
        evtBitOn(2414);
        evtBitOn(2406);
        break;
    case 391:
        evtBitOn(2415);
        evtBitOn(2407);
        break;
    }
}
#else
INCLUDE_ASM(const s32, "sbtlDMG", datSetKillDevil);
#endif

s32 sbtlRefSbtl15(s32 x, s32 y) {
    if (wk_sbtl->status3 & 2) {
        x = x * 3 / 2;
    }
    return x;
}

void sbtlSetAddMag_Setsuma(struct SBTLUNIT_WORK *a_pUnitTgt, s32 a_addMag) {
    sdfVector4_t pos;
    s32 mag;

    if (a_pUnitTgt && a_addMag > 0) {
        mag = sbtlRefSbtl15(a_addMag, a_addMag);
        sbtlGetUnitHelperPos(a_pUnitTgt, SBTLUNIT_MARK__1SHOT);
        SDF_VU0_STOREVECTOR_MEMOPS(pos);
        if (!sdfModelGetMarkPos_sdfModel(a_pUnitTgt->hEveUnit->owner->inner, 1)) {
            sdfModelGetMarkPos_sdfModel(a_pUnitTgt->hEveUnit->owner->inner, 0);
        }
        SDF_VU0_STOREVECTOR_MEMOPS(pos);
        Add_AW_MagWait(&pos, mag);
    }
}

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCallBitCheck2);

void sbtlCallBtlChkHaveMemSkl(struct SBTLUNIT_WORK *acter, struct SBTLUNIT_WORK *target, s32 mag) {
    s32 add;

    if (acter && target && mag > 0 && (acter->field414 & 1) && (target->field414 & 2)) {
        add = mag;
        if (sbtlChkHaveMemSkl(&acter->unit, AUTO_MAGADDWEAKATK)) {
            add += acter->unit.autost_cnt;
        }
        if (sbtlChkHaveMemSkl(&acter->unit, AUTO_MAGKYUSYUUP)) {
            add += add * acter->unit.autost_cnt2 / 100;
        }
        sbtlSetAddMag_Setsuma(target, add);
    }
}

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

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCreateAnim_Unit);

INCLUDE_ASM(const s32, "sbtlDMG", CreateAnim);

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

s32 sbtlAnimListClr(void) {
    SBTLDMG_ANIMWORK **unit;
    SBTLDMG_ANIMWORK *wk;
    s32 i;

    unit = wk_sbtl->animList->unit;
    for (i = 0; i < SBTLDMG_ANIM_MAX; i++) {
        wk = *unit++;
        if (wk) {
            wk->st = 3;
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "sbtlDMG", sbtlAnimListProc_sbtlDMG);

s32 sbtlDrawAnimList(void) {
    SBTLDMG_ANIMWORK *anim;
    s32 i;

    if (!wk_sbtl || !wk_sbtl->animList) {
        return 0;
    }
    if (wk_sbtl->flg2 & 0x20) {
        return 0;
    }
    for (i = 0; i < SBTLDMG_ANIM_MAX; i++) {
        anim = wk_sbtl->animList->unit[i];
        if (anim == NULL || anim->st == 0 || anim->st < 0 || anim->st >= 3) {
            continue;
        }
        if (anim->dmg_anm_add > 0) {
            anim->dmg_anm += anim->dmg_anm_add;
        }
        if (anim->dmg <= anim->dmg_anm) {
            anim->dmg_anm = anim->dmg;
            anim->dmg_anm_add = 0;
        }
        switch (anim->side) {
        case 0:
            sbtlDamagePanelR(&anim->pos, anim->dmg_anm, anim->rgba, anim->rgba2);
            break;
        case 1:
            sbtlDamagePanelN(&anim->pos, anim->dmg_anm, anim->rgba, anim->rgba2, anim->minor);
            break;
        default:
            sbtlDamagePanelE(&anim->pos, anim->dmg_anm, anim->rgba, anim->rgba2);
            break;
        }
    }
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

INCLUDE_ASM(const s32, "sbtlDMG", sbtlLoadAttackHitData_Decode);

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
    sbtlDeleteAttackHitData(&sbtlWork2);
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

INCLUDE_RODATA(const s32, "sbtlDMG", CreateAnimTable);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlLoadAttackHitData);

s32 sbtlDeleteAttackHitData(struct SBTLUNIT_WORK *wk)
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

void sbtlDltHit_by_UnitSkl(SBTLDMG_HITLISTWORK *lwk, s32 unitID, SKLID sklID) {
    SBTLDMG_HITWORK *wk;

    wk = lwk->wk;
    if (wk && lwk->num) {
        do {
            if (wk->acterID == unitID && wk->skillid == sklID) {
                wk->st = 2;
            }
            wk = wk->next;
        } while (wk);
    }
}

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCallMemset6);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCreateHitC_None);

void sbtlAddSerialID2(void) {
    wk_sbtl->hitList->serialID2++;
    if (wk_sbtl->hitList->serialID2 >= 0x7FFF) {
        wk_sbtl->hitList->serialID2 = 0;
    }
}

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetApos12);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlCallMemset6Table);

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

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlSetHitPosTable);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetHitPos);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlGetHit_OnlyEndPosTable2);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlGetHit_OnlyEndPos);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlSetHitEndPosTable);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetHitEndPos);

s32 sbtlCallMemAllocDirect2(struct SBTLDMG_HITLISTWORK *lwk, struct SBTLUNIT_WORK *wk, struct SBTLEFF_WORK *ewk, s32 actID, s32 starttime, s32 life, SKLID skillid, struct SBTLSKILL_TBL *tbl)
{
    struct sdfMemHandle_t *handle;
    SBTLDMG_HITWORK *twk;
    SBTLDMG_HITWORK *hwk;
    struct SBTLUNIT_WORK *unit;

    handle = sdfMemAllocDirect(sizeof(SBTLDMG_HITWORK));
    twk = sdfMemLock(handle);
    memset(twk, 0, sizeof(SBTLDMG_HITWORK));
    twk->handle = handle;
    twk->hitbk_cnt = tbl->hitbk_cnt;
    twk->acterID = wk->field1A0;
    twk->hitbk_rate = tbl->hitbk_rate;
    twk->starttime = starttime;
    twk->actID = actID;
    twk->skillid = skillid;
    twk->targetflg = tbl->targetflg;
    twk->hittype = 0;
    twk->hitlwk = NULL;
    if (ewk != NULL) {
        twk->workID = ewk->workID;
    } else {
        twk->workID = 0;
    }
    twk->lwk = NULL;
    twk->life = life;
    twk->prev = NULL;
    twk->next = NULL;
    twk->hModel = NULL;
    twk->hEveUnit = NULL;
    twk->pBasic = NULL;
    twk->hPartsList = NULL;
    if (skillid == 285) {
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, actID);
        if (unit == NULL) {
            unit = wk;
        }
    } else {
        unit = wk;
    }
    SDF_VU0_LOADVECTOR(&unit->rot);
    SDF_VU0_STOREVECTOR(&twk->rot);
    hwk = lwk->wk;
    if (hwk == NULL) {
        lwk->wk = twk;
        lwk->num = 1;
    } else {
        while (hwk->next != NULL) {
            hwk = hwk->next;
        }
        hwk->next = twk;
        twk->prev = hwk;
        lwk->num++;
    }
    twk->type = tbl->type;
    twk->skillflg = tbl->flg;
    twk->area = tbl->area;
    twk->rad = tbl->rad;
    twk->atype = tbl->area_type;
    twk->rad_area = tbl->rad_area;
    twk->attr_flg = tbl->attr_flg;
    twk->acterside = unit->field414;
    if (skillid == 149) {
        sbtlCHitFunc_ATTACK(wk, ewk, twk, tbl);
    }
    twk->st = 1;
    twk->serialID = lwk->serialID;
    lwk->serialID++;
    return lwk->num - 1;
}

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

INCLUDE_ASM(const s32, "sbtlDMG", sbtlUnsetKaraburi);

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

INCLUDE_ASM(const s32, "sbtlDMG", sbtlHitCProc_Idou);

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

INCLUDE_ASM(const s32, "sbtlDMG", sbtlHitCProc);

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
    if (target->field4A2 > 0 || (wk->field6C4 & 2) || acter->status == 7 || acter->status == 9) {
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
    if ((target->flg & 0x100) && !(ptbl->flg2 & 0x20000)) {
        sbtlCallBtlGetStartTimeDamage(target, 0);
        sbtlSetHitback(target, &hwk, 0x1000);
        return 1;
    }
    hp = target->unit.now.hp - damage;
    target->unit.now.hp = hp;
    target->unit.now.hp = hp <= 0 ? 0 : (hp < target->unit.now.maxhp ? hp : target->unit.now.maxhp);
    sbtlCallBtlCreateAnimUnit2(target, damage);
    sbtlSetHitback(target, &hwk, 0);
    sbtlCreateAnim_Unit(target, acter, damage, 0, 0);
    sbtlCheckYukaDamageEffect(target, hwk.skillid, hwk.type, 0, 0);
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

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCallMemset7);

s32 sbtlSetHit(SBTLDMG_HITWORK *wk, struct SBTLUNIT_WORK *wklock, s32 *hitflg) {
    s32 flg;

    flg = 0;
    if (wk->skillid >= SKLID_NOUSE_SORD1 && (wk->skillid < SKLID_ATK || wk->skillid == SKLID_MAWASIKERI)) {
        flg = 1;
    }
    if (!wklock->field78C || !flg) {
        sbtlSEISHINCNTR(wklock, wk, 0);
        *hitflg = 1;
    } else {
        *hitflg = 0;
    }
    if (!flg) {
        if (wk->flg & 0x200) {
            sbtlForceDelete_SerialID(wk_sbtl->effList, wk->serialID);
        }
        if (wk->flg & 2) {
            wk->st = 2;
            return 0;
        }
        if (wk->area & 1) {
            return 0;
        }
    }
    if (flg == 1) {
        if (!wklock->field78C) {
            wklock->field78C = 4;
            return 1;
        }
        return 0;
    }
    return 1;
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
    if (hwk == NULL || lwk->num == 0) {
        return 0;
    }
    do {
        if (!IsSceanStop() || (hwk->flg & 0x80)) {
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
    u16 muteki_time = sbtlGetSkilltbl(skillid)->muteki_time;

    wk->field4A2 = muteki_time;
    if (flg & 8 && (s16)muteki_time <= 0) {
        wk->field4A2 = 1;
    }
    return 1;
}

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCheckHitArea_sbtlDMG);

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

INCLUDE_ASM(const s32, "sbtlDMG", sbtlGetSide);

s32 CheckDamageTarget(struct SBTLUNIT_WORK *wk, struct SBTLDMG_HITWORK *hwk, SKLID skillid) {
    if (wk->L2_muteki) {
        return 0;
    }
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
    if (sbtlCallGetProcessStat()) {
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

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCheckDamageTarget_Skilltbl);

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

s32 sbtlSetDeadDamageEx(struct SBTLUNIT_WORK *wklock, struct SBTLUNIT_WORK *wk, SBTLDMG_HITWORK *hwk, s32 damage, u32 attr, s8 *a_pbDamage, f32 *a_pRate) {
    s32 ret;
    s32 bDamage;
    f32 rate;

    ret = 0;
    if (wklock) {
        ret = CheckLiveTarget(wklock);
        if (ret) {
            ret = sbtlIsDeadBgn(wklock);
            if (ret) {
                rate = 0.0f;
                sbtlResetBtlMagicStatus(wklock);
                if (wklock->field414 & 1) {
                    if (!(wklock->field414 & 0x80)) {
                        bDamage = sbtlSetDeadDamageProc(wk, wklock, hwk, damage, attr);
                        sbtlSetRirituNakamaHD(wk_sbtl->expList, wklock, hwk->skillid, hwk->acterID, 10);
                    } else {
                        bDamage = sbtlSetDeadDamageProc(wk, wklock, hwk, damage, attr);
                        rate = wklock->m_rate;
                    }
                } else {
                    if (attr & 0x4000) {
                        return 0;
                    }
                    bDamage = sbtlSetDeadDamageProc(wk, wklock, hwk, damage, attr);
                    rate = wklock->m_rate;
                    sbtlSEVolCalc(0x10021);
                }
                if (a_pbDamage) {
                    *a_pbDamage = bDamage;
                }
                if (a_pRate) {
                    *a_pRate = rate;
                }
                return 1;
            }
        }
    }
    return ret;
}

INCLUDE_ASM(const s32, "sbtlDMG", sbtlRefSbtl16);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetKudaDamage);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCallBtlCreateAnimUnit);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSEISHINCNTR);

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

s32 sbtlIsAura(struct SBTLUNIT_WORK *wk, s32 kindFlg) {
    if (wk->field4AC && (wk->aurakind & kindFlg)) {
        return wk->aurapara;
    }
    return -1;
}

INCLUDE_ASM(const s32, "sbtlDMG", sbtlUpdateDamageKaja);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlGetCalcDamageEx);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrEienboudoAtk20);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrEienboudoDmg15);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlGetCalcDamage);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetReflectDamage);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlMAGWAZAMagNot);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetDamage);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetDamageProc_sbtlDMG);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlGetHitUnit_Muki);

s32 sbtlSetHitback(struct SBTLUNIT_WORK *wk, struct SBTLDMG_HITWORK *hwk, u32 attr)
{
    sdfVector4_t tpos;
    sdfVector4_t pos;
    sdfVector4_t *ppos = &pos;
    struct SBTLUNIT_WORK *acter;
    u16 type;
    struct SBTLUNIT_MODEL *model;
    f32 rate;

    memset(ppos, 0, sizeof(pos));
    pos.w = 1.0f;
    if (!(wk->field414 & 7)) {
        return 0;
    }
    if ((wk->field414 & 0x80) && (wk->flg2 & 0x400)) {
        return 0;
    }
    if (attr & 0x3000) {
        return 0;
    }
    if (wk->field6C4 & 0x40) {
        return 0;
    }
    if (sbtlGetBadStatus(&wk->unit) == BST_SLEEP) {
        return 0;
    }
    model = wk->model;
    model->hitbk_cnt = 0.0f;
    wk->vstatus |= 0x800;
    model->hitbk_cmpcnt = 1.0f / hwk->hitbk_cnt;
    rate = hwk->hitbk_rate;
    model->hitbk_rate = rate;
    if (attr & 0x400000) {
        model->hitbk_rate = rate * 0.5f;
    }
    type = hwk->type & 0xFFF;
    if ((type == 2 || type == 14) && (hwk->area & 0x10)) {
        acter = sbtlSearchUnit_ID(wk_sbtl->unitList, hwk->acterID);
        if (acter != NULL && sbtlCheckDamageTarget(acter)) {
            SDF_VU0_LOADVECTOR(&acter->pos);
            SDF_VU0_STOREVECTOR(ppos);
        } else {
            pos.x = hwk->pos.x;
            pos.y = hwk->pos.y;
            pos.z = hwk->pos.z;
        }
        SDF_VU0_LOADVECTOR(ppos);
        SDF_VU0_SETVECTOR_Y(0.0f);
        SDF_VU0_STOREVECTOR(&tpos);
        SDF_VU0_LOADVECTOR(&wk->pos);
        SDF_VU0_SETVECTOR_Y(0.0f);
        SDF_VU0_LOADVECTOR_B(&tpos);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR(&wk->model->hitbk_dir);
        return 1;
    }
    if (hwk->skillid == 287) {
        pos.x = hwk->pos.x;
        pos.y = hwk->pos.y;
        pos.z = hwk->pos.z;
    } else {
        pos.x = hwk->acterpos[0];
        pos.y = hwk->acterpos[1];
        pos.z = hwk->acterpos[2];
    }
    SDF_VU0_LOADVECTOR(ppos);
    SDF_VU0_SETVECTOR_Y(0.0f);
    SDF_VU0_STOREVECTOR(&tpos);
    SDF_VU0_LOADVECTOR(&wk->pos);
    SDF_VU0_SETVECTOR_Y(0.0f);
    SDF_VU0_LOADVECTOR_B(&tpos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&model->hitbk_dir);
    return 1;
}

s32 sbtlSetStoneDeadDamageProc(struct SBTLUNIT_WORK *wk, struct SBTLUNIT_WORK *wklock, struct SBTLDMG_HITWORK *hwk, s32 damage, u32 attr)
{
    s32 time;

    sbtlSetStoneDeadDamage(wk, damage);
    time = sbtlGetStartTime_Damage();
    if (hwk->acterID == wk_sbtl->targetUnitID && wk_sbtl->mode != 4 && wk_sbtl->mode != 6) {
        sbtlSetHitStop(sbtlBtlSetDamageProcSBtlDMGSub(hwk->skillid), 3);
    }
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
    sbtlSetWeakBit(wk->unit.devilId);
    if (wk->status != 9 && (wk->field414 & 2) && (attr & 0x4000)) {
        wk->flg2 |= 0x10;
        wk->aiwk->scrflag |= 0x400000;
        sbtlSetWeakBit(wk->unit.devilId);
        sbtlCreateSTP_Unit(wk_sbtl->stp, wk, SBTLSTP_ID_WEAK, ptbl->attrtime);
    }
}

s32 sbtlSetDeadDamageProc(struct SBTLUNIT_WORK *wk, struct SBTLUNIT_WORK *wklock, struct SBTLDMG_HITWORK *hwk, s32 damage, u32 attr)
{
    sdfVector4_t head;
    struct SBTLSKILLP_TBL *ptbl;
    struct SBTLUNIT_WORK *acter;
    s32 ret;
    s32 bst;
    s32 time;
    s32 type;

    ptbl = sbtlGetSkilltbl(hwk->skillid);
    acter = sbtlSearchUnit_ID(wk_sbtl->unitList, hwk->acterID);
    if ((attr & 0x4000) || wk->status == 9) {
        if (wk->status != 9) {
            sbtlGetUnitHelperPos(wk, SBTLUNIT_MARK_HEAD);
            SDF_VU0_STOREVECTOR(&head);
        } else {
            sbtlCreateDvltk_Priority(wk_sbtl->dvltkList, wk, SBTLDVLTK_MODE_S_SUCCESS2);
        }
    }
    sbtlSetDeadWeak(wk, ptbl, attr);
    bst = sbtlCallCheckBadStatus(acter, wk, hwk->skillid, 1);
    if (bst >= 0 && (bst < 4 || bst == 5)) {
        if (wk->status != 9 && (wk->field414 & 2) && !(wk->flg2 & 0x10)) {
            wk->flg2 |= 0x10;
            wk->aiwk->scrflag |= 0x400000;
            sbtlSetWeakBit(wk->unit.devilId);
            sbtlCreateSTP_Unit(wk_sbtl->stp, wk, SKLID_OFS_KAEN, ptbl->attrtime);
        }
    }
    ret = sbtlSetDeadDamage(wk, damage);
    time = sbtlGetStartTime_Damage();
    if (hwk->acterID == wk_sbtl->targetUnitID) {
        sbtlSetHitStop(time + 9, 20);
        if (!sbtlResetJump()) {
            dds3SetQuake(30, 40);
        }
        sbtlLeaf110(wk->pos.x, wk->pos.y, wk->pos.z);
    }
    if (hwk->workID == 288) {
        hwk->skillid = SKLID_OFS_PROGRAM;
    }
    if (!(wklock->field414 & 7)) {
        return ret;
    }
    if (hwk->BEDtype == 2) {
        wk_sbtl->huka2hwk = hwk;
        sbtlCheckEffectData(wklock, hwk->skillid, 5, 0, hwk->acterside, hwk->serialID2, time);
    } else {
        sbtlCheckYukaDamageEffect(wklock, hwk->skillid, hwk->type, time + 1, 1);
        type = hwk->type & 0xFFF;
        switch (type) {
        case 1 ... 5:
        case 9 ... 15:
            wk_sbtl->huka2hwk = hwk;
            sbtlCheckEffectData(wklock, hwk->skillid, 2, 0, hwk->acterside, hwk->serialID2, time);
            break;
        }
    }
    return ret;
}

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetInvalidDamage);

/* NON_MATCHING: 28 of 41 words match.  */
#ifdef NON_MATCHING
s32 sbtlCallBtlGetStartTimeDamage(struct SBTLUNIT_WORK *wk, s32 attr) {
    s32 time;
    s32 ret;
    sdfVector4_t *pos;

    time = sbtlGetStartTime_Damage();
    pos = &wk->pos;
    if (attr == 0x1000) {
        ret = 0;
        if (!wk->field7AC) {
            sbtlCreateEff(wk_sbtl->effList, wk, EFFID_MUKOU, time, pos, 0x80808080, 0);
            wk->field77C++;
            ret = 1;
        }
    } else {
        sbtlCreateEff(wk_sbtl->effList, wk, EFFID_GUARD, time, pos, 0x80808080, 0);
        ret = 0;
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "sbtlDMG", sbtlCallBtlGetStartTimeDamage);
#endif

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetAbsorbDamage);

void sbtlGuardHit(struct SBTLUNIT_WORK *wk, s32 damage)
{
    if (wk_sbtl->battleMode != 3 && wk->field1A0 == wk_sbtl->targetUnitID && !(wk->flg2 & 0x20)) {
        sbtlSetMotion_sbtlUnit(wk, 0, 0, 0, 0);
        sbtlSetAction(wk, 1, 0);
    }
    sbtlCreateAnim_Unit(wk, NULL, damage, 0, 0);
    if ((wk->field414 & 0x80) || ((wk->field414 & 1) && wk_sbtl->battleMode == 3)) {
        sbtlSetVibe(SBTLPLAYER_VTYPE_ONLYD);
    }
    if ((wk->field414 & 0x80) || ((wk->field414 & 1) && wk_sbtl->battleMode == 3)) {
        if (!sbtlResetJump()) {
            dds3SetQuake(20, 40);
        }
    }
}

void sbtlCallBtlCreateAnimUnit2(struct SBTLUNIT_WORK *wk, s32 damage) {
    sbtlCreateAnim_Unit(wk, NULL, damage, 0, 0);
    if (sbtlGetBadStatus(&wk->unit) != BST_SLEEP && !wk->field710 && !wk->field718) {
        sbtlSetMotion_sbtlUnit(wk, 1, 20, 0, 0);
        sbtlSetAction(wk, SBTLACTION_ST_DAMAGE, 0);
    }
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

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCallMemset8);

/* NON_MATCHING: 38 of 268 words match. gcc swaps the wk and time registers and folds the devilId 455..456 test into one unsigned compare. */
#ifdef NON_MATCHING
void sbtlSetDyingDamage(struct SBTLUNIT_WORK *wk, SKLID sklid, s32 damage, u32 attr, s32 acterID) {
    struct SBTLSKILLP_TBL *skl;
    s32 time;
    s32 base;
    SKLID id;
    s16 para[2];

    skl = sbtlGetSkilltbl(sklid);
    time = skl->attrtime;
    if (wk->field414 & 2) {
        time = time * 3 >> 1;
        id = sbtlChkHaveMemSklParty(30);
        if (id) {
            sbtlGetAutoSkillPara(id, &para[0], &para[1]);
            base = time;
            time = (f32)time * ((f32)para[1] / 100.0f);
            datDebugPrintf("J_KOUCYOKU[ACT] %d * [H]%d/100 -> %d\n", base, para[1], time);
        }
    }
    if (wk->field414 & 2) {
        if (wk->status != 9) {
            if (wk->unit.devilId >= 457 || wk->unit.devilId < 455) {
                sbtlSetMotion_sbtlUnit(wk, 1, 0, 0, 0);
            }
            sbtlSetAction(wk, 9, time);
            if (attr & 0x4000) {
                wk->flg2 |= 0x10;
                wk->aiwk->scrflag |= 0x400000;
                sbtlCreateSTP_Unit(wk_sbtl->stp, wk, SKLID_OFS_KAEN, time);
                sbtlSetRirituNakamaHD(wk_sbtl->expList, wk, sklid, acterID, 1);
                sbtlSetRirituNakamaHD(wk_sbtl->expList, wk, sklid, acterID, 7);
            }
        } else {
            sbtlSetRirituNakamaHD(wk_sbtl->expList, wk, sklid, acterID, 5);
            sbtlSetRirituNakamaHD(wk_sbtl->expList, wk, sklid, acterID, 0);
        }
    } else if (!(wk->field414 & 0x80)) {
        if (wk->status != 9) {
            sbtlSetMotion_sbtlUnit(wk, 1, 0, 0, 0);
            sbtlSetAction(wk, 9, time);
            if (attr & 0x4000) {
                wk->flg2 |= 0x10;
                sbtlCreateSTP_Unit(wk_sbtl->stp, wk, SKLID_OFS_KAEN, time);
                sbtlSetRirituNakamaHD(wk_sbtl->expList, wk, sklid, acterID, 1);
                sbtlSetRirituNakamaHD(wk_sbtl->expList, wk, sklid, acterID, 7);
            }
        } else {
            sbtlSetRirituNakamaHD(wk_sbtl->expList, wk, sklid, acterID, 5);
            sbtlSetRirituNakamaHD(wk_sbtl->expList, wk, sklid, acterID, 0);
        }
        if ((wk->field414 & 1) && wk_sbtl->battleMode == 3) {
            sbtlSetVibe(SBTLPLAYER_VTYPE_DOWN);
            if (!sbtlResetJump()) {
                dds3SetQuake(25, 65);
            }
        }
    } else {
        if (time) {
            sbtlSetMotion_sbtlUnit(wk, 1, 0, 0, 0);
            if (wk->status != 9) {
                sbtlSetAction(wk, 9, time);
            }
            sbtlSetVibe(SBTLPLAYER_VTYPE_DOWN);
            if (!sbtlResetJump()) {
                dds3SetQuake(25, 65);
            }
            sbtlSetRirituNakamaHD(wk_sbtl->expList, wk, sklid, acterID, 5);
            sbtlSetRirituNakamaHD(wk_sbtl->expList, wk, sklid, acterID, 0);
        }
    }
    sbtlCreateAnim_Unit(wk, NULL, damage, 0, 0);
    sbtlClearHitC(wk_sbtl->hitList, wk);
    sbtlForceReset(wk_sbtl->effList, wk);
    wk->model->flags &= ~0x20;
    wk->flg |= 0x20;
    wk->field474 = 0;
    wk->vstatus |= 0x80;
}
#else
INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetDyingDamage);
#endif

void sbtlSetStoneDeadDamage(struct SBTLUNIT_WORK *wk, s32 damage)
{
    s32 mag;

    sbtlDeath(wk);
    sbtlSetWeakBit(wk->unit.devilId);
    sbtlGetStartTime_Damage();
    if (wk->field414 & 0x80) {
        sbtlSetMotion_sbtlUnit(wk, 13, 0, 0, 0);
        sbtlSetAction(wk, 0x22, 0);
        if (wk_sbtl->battleMode == 3) {
            sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_DEAD_N, SKLID_NONE);
        } else if (sbtlCheckTutorialBattle()) {
            sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_T_DEAD_P, SKLID_NONE);
        } else {
            sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_DEAD_P, SKLID_NONE);
        }
        sbtlSetVibe(SBTLPLAYER_VTYPE_STONEDEAD);
        if (!sbtlResetJump()) {
            dds3SetQuake(25, 65);
        }
    } else {
        sbtlSetMotion_sbtlUnit(wk, 13, 0, 0, 0);
        sbtlSetAction(wk, 0xA, 0);
        if ((wk->field414 & 1) && wk_sbtl->battleMode == 3) {
            sbtlSetVibe(SBTLPLAYER_VTYPE_STONEDEAD);
            if (!sbtlResetJump()) {
                dds3SetQuake(25, 65);
            }
        }
    }
    wk->unit.now.hp = 0;
    wk->action_flg = 0;
    sbtlSetFade(wk, SBTLUNIT_VST_FADEOUT, 0, 0, 128);
    wk->color = 0;
    sbtlCreateEff(wk_sbtl->effList, wk, EFFID_STONEDEAD, 0, &wk->pos.x, 0x80808080, 0);
    if ((wk->field414 & 1) && !(wk->field414 & 0x80)) {
        sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_DEAD_N, SKLID_NONE);
    }
    sbtlClearHitC(wk_sbtl->hitList, wk);
    sbtlForceReset(wk_sbtl->effList, wk);
    wk->flg |= 0x21;
    wk->vstatus |= 0x80;
    wk->flg2 |= 0x100;
    wk->field474 = 0;
    wk->action_flg = 0;
    sbtlCheckBadStatus(wk);
    sbtlSetBadStatus_No(wk, SKLID_NONE, BST_KANSEN, 0, -1);
    dds3ObjectBaseOffFlag(wk->pBasic, 1024);
    sbtlCreateDvltk_Priority(wk_sbtl->dvltkList, wk, SBTLDVLTK_MODE_DEAD);
    sbtlForceDelete_ID_Acter(wk_sbtl->effList, wk, EFFID_RENKEIOR);
    if (wk->field414 & 2) {
        sbtlRefDDS3GBWK6(wk->unit.devilId, sbtlGetDevilHuumaItem(wk->unit.devilId) + 1);
        gohFunc6(wk->unit.devilId);
        gohKarteRegistDevilBtl(&wk->unit, &DDS3_GBWK->unk7110, 1, 0);
        datSetKillDevil(wk);
        if ((wk->field414 & 2) && !(wk->flg & 0x100000) && sbtlGetPChara()) {
            mag = wk->unit.level >> 2;
            if (mag == 0) {
                mag = 1;
            }
            sbtlSetAddMag_Setsuma(wk, mag);
        }
    }
}

/* NON_MATCHING: 36 of 112 words match.  */
#ifdef NON_MATCHING
s32 CheckAutoSkill(struct SBTLUNIT_WORK *wk) {
    s32 para;
    s32 i;
    SKLID id;
    s16 pct;
    s32 idx;

    para = 0;
    i = 0;
    id = SKLID_NONE;
    while (i < 6) {
        id = sbtlGetAutoSkill_ID(&wk->unit, i++);
        para = sbtlGetAutoSkillPara(id, &wk->unit.autost_cnt, &wk->unit.autost_cnt2);
        if (para == 1) {
            break;
        }
    }
    if (para != 1) {
        return 1;
    }
    pct = GetPercentage();
    if (wk_sbtl->field4354) {
        return 1;
    }
    if ((wk_sbtl->status3 & 0x4000) || pct >= 80) {
        return 1;
    }
    wk_sbtl->field4354 = 30;
    if (!(wk->flg2 & 0x800)) {
        return 1;
    }
    wk->flg2 &= ~0x800;
    if ((wk->field414 & 1) && !(wk->field414 & 0x80)) {
        idx = wk_sbtl->summonIndex[0];
        if (wk->field1A0 != wk_sbtl->stockUnitID[idx]) {
            idx = wk_sbtl->summonIndex[1];
        }
        sbtlSetBit(&wk_sbtl->deadbombBit, idx);
    }
    if (wk->field414 & 2) {
        sbtlSetUseSkill_sbtlActAI(wk, id, 0);
        wk->model->stat = 1;
        wk->aiwk->stat = 2;
        wk->model->mode = 14;
        return 0;
    }
    sbtlResetBadStatus(wk, wk->unit.badStatus);
    ResetAction(wk, 0);
    sbtlSetSkillData_AI(wk, id);
    sbtlSetUseSkill_sbtlActAI(wk, id, 0);
    return 0;
}
#else
INCLUDE_ASM(const s32, "sbtlDMG", CheckAutoSkill);
#endif

INCLUDE_ASM(const s32, "sbtlDMG", sbtlIsEnemyLastOne);

s32 sbtlSetDeadDamage(struct SBTLUNIT_WORK *wk, s32 damage) {
    s32 ret = 1;
    s32 addmag = 0;
    s32 found4;
    s32 found6;
    s32 mag;

    sbtlSetWeakBit(wk->unit.devilId);
    sbtlDeath(wk);
    sbtlGetStartTime_Damage();
    if (wk->field414 & 0x80) {
        sbtlSetMotion_sbtlUnit(wk, 13, 0, 0, 0);
        sbtlSetAction(wk, 34, 0);
        if (wk_sbtl->battleMode == 3) {
            sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_DEAD_N, SKLID_NONE);
        } else if (sbtlCheckTutorialBattle()) {
            sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_T_DEAD_P, SKLID_NONE);
        } else {
            sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_DEAD_P, SKLID_NONE);
        }
        sbtlSetVibe(SBTLPLAYER_VTYPE_DEAD);
        if (!sbtlResetJump()) {
            dds3SetQuake(25, 65);
        }
    } else if (wk->field414 & 1) {
        sbtlSetMotion_sbtlUnit(wk, 13, 0, 0, 0);
        sbtlSetAction(wk, 10, 0);
        ret = CheckAutoSkill(wk);
        if ((wk->field414 & 1) && wk_sbtl->battleMode == 3) {
            sbtlSetVibe(SBTLPLAYER_VTYPE_DEAD);
            if (!sbtlResetJump()) {
                dds3SetQuake(25, 65);
            }
        }
    } else {
        found4 = sbtlCheck(wk, 4, 0);
        found6 = sbtlCheck(wk, 6, 0);
        if (found4 >= 0) {
            wk->field6C0 = 1;
        } else if (found6 >= 0) {
            wk->field6C0 = 2;
        } else if (wk->aiwk->scrflag & 0x20000) {
            sbtlSetAction(wk, 10, 0);
            wk->action_flg = 0;
            wk->framecnt = 0;
            wk->field5A4 = 0;
        } else {
            sbtlSetMotion_sbtlUnit(wk, 13, 4, 0, 0);
            wk->action_flg = 0;
            sbtlSetAction(wk, 10, 0);
            ret = CheckAutoSkill(wk);
            if (wk->field414 & 2) {
                if (wk->flg & 0x100000) {
                    addmag = sbtlCheckDeadEventMag(wk);
                } else {
                    addmag = 1;
                }
            }
        }
    }
    if ((wk->field414 & 1) && !(wk->field414 & 0x80) && wk->status != 40 && wk->skillid != SKLID_OFS_AUTO) {
        sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_DEAD_N, SKLID_NONE);
    }
    if (wk->flg & 0x20000) {
        wk->GroupID = -1;
    }
    sbtlCreateAnim_Unit(wk, NULL, damage, 0, 0);
    sbtlClearHitC(wk_sbtl->hitList, wk);
    sbtlForceReset(wk_sbtl->effList, wk);
    if (!(wk->aiwk->scrflag & 0x20000) && wk->status != 40 && !(wk->flg & 0x100000)) {
        wk->flg |= 1;
    }
    wk->flg |= 0x20;
    wk->field474 = 0;
    wk->vstatus |= 0x80;
    wk->action_flg = 0;
    sbtlCheckBadStatus(wk);
    sbtlSetBadStatus_No(wk, SKLID_NONE, BST_KANSEN, 0, -1);
    wk->unit.now.hp = 0;
    dds3ObjectBaseOffFlag(wk->pBasic, 1024);
    sbtlCreateDvltk_Priority(wk_sbtl->dvltkList, wk, SBTLDVLTK_MODE_DEAD);
    sbtlForceDelete_ID_Acter(wk_sbtl->effList, wk, EFFID_RENKEIOR);
    if (wk->field414 & 2) {
        sbtlRefDDS3GBWK6(wk->unit.devilId, sbtlGetDevilHuumaItem(wk->unit.devilId) + 1);
        gohFunc6(wk->unit.devilId);
        gohKarteRegistDevilBtl(&wk->unit, &DDS3_GBWK->unk7110, 1, 0);
        datSetKillDevil(wk);
    }
    if (addmag && sbtlGetPChara()) {
        mag = wk->unit.level / 5;
        if (mag <= 0) {
            mag = 1;
        }
        sbtlSetAddMag_Setsuma(wk, mag);
    }
    return ret;
}

/* NON_MATCHING: 151 of 206 words match. Retail fills the field414 & 2 branch delay slot with the wk_sbtl load; gcc schedules the skillid copy there. */
#ifdef NON_MATCHING
void sbtlSetSpecialDamage(struct SBTLUNIT_WORK *wk, struct SBTLDMG_HITWORK *hwk, SKLID skillid, s32 damage) {
    s32 sttime;
    s32 type;

    if (wk->unit.devilId != 631 && wk->unit.devilId != 672) {
        return;
    }
    if (wk->status == 7) {
        return;
    }
    if ((wk->field414 & 2) && (wk_sbtl->status3 & 0x4000)) {
        wk = sbtlSearchUnit_ID(wk_sbtl->unitList, hwk->acterID);
        if (wk == NULL) {
            return;
        }
    }
    if ((wk->field414 & 2) && (wk_sbtl->status3 & 0x4000000)) {
        damage /= 2;
    }
    sbtlSetDamage(wk, skillid, damage);
    sttime = sbtlGetStartTime_Damage();
    sbtlCreateAnim_Unit(wk, NULL, damage, 0, 0);
    sbtlSetHitback(wk, hwk, 0);
    if (hwk->BEDtype == 2) {
        wk_sbtl->huka2hwk = hwk;
        sbtlCheckEffectData(wk, hwk->skillid, 5, 0, hwk->acterside, hwk->serialID2, sttime);
    } else {
        sbtlEffSetTarget(wk->field1A0);
        if (wk->field406 == 1) {
            wk->field406 = 0;
            sbtlCheckYukaDamageEffect(wk, hwk->skillid, hwk->type, sttime + 1, 1);
        } else {
            sbtlCheckYukaDamageEffect(wk, hwk->skillid, hwk->type, sttime + 1, 0);
        }
        type = hwk->type & 0xFFF;
        switch (type) {
        case 1 ... 5:
        case 9 ... 15:
            wk_sbtl->huka2hwk = hwk;
            sbtlCheckEffectData(wk, hwk->skillid, 2, 0, hwk->acterside, hwk->serialID2, sttime);
            break;
        }
    }
    if (!sbtlIsDeadBgn(wk)) {
        return;
    }
    sbtlClearHitC(wk_sbtl->hitList, wk);
    sbtlForceReset(wk_sbtl->effList, wk);
    sbtlResetAAI(wk);
    wk->model->dataflg = 0;
    sbtlSetAIWkStat(wk);
    wk->aiwk->mode = 0;
    ResetAction(wk, 0);
    sbtlSetAction(wk, 7, 0);
    if (wk->aiwk->scrStep == 0) {
        sbtlSetMotion_sbtlUnit(wk, 1, 4, 0, 0);
        wk->motion_setno = 10;
        wk->unit.now.hp = wk->unit.now.maxhp;
        wk->aiwk->scrStep = 1;
    } else if (wk->aiwk->scrStep == 1) {
        sbtlSetMotion_sbtlUnit(wk, 1, 4, 0, 0);
        wk->motion_setno = 11;
        wk->unit.now.hp = wk->unit.now.maxhp;
        wk->aiwk->scrStep = 2;
    } else {
        sbtlSetMotion_sbtlUnit(wk, 13, 4, 0, 0);
        wk->flg |= 0x4000;
        wk->aiwk->scrStep = 3;
    }
    wk->model->flags &= ~0x20;
    wk->field474 = 0;
    wk->flg |= 0x20;
    wk->vstatus |= 0x80;
}
#else
INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetSpecialDamage);
#endif

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetSpecialDamage_DamageOnly);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetDamage_L_EnemyAll);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCallBtlCreateSideListSBtlUnit4);

/* NON_MATCHING: 27 of 56 words match.  */
#ifdef NON_MATCHING
s32 sbtlBtlSetDamageProcSBtlDMGSub(SKLID skillid) {
    s32 stop;

    stop = 4;
    switch (skillid) {
    case SKLID_TUKI:
    case SKLID_NOUSE_SORD3:
    case SKLID_NOUSE_MAGTUKI:
    case SKLID_NOUSE_SHORTPIKE:
    case SKLID_NOUSE_MIZOOCHITUKI:
        stop = 5;
        break;
    case SKLID_NOUSE_SORD2:
        stop = 6;
        break;
    case SKLID_NOUSE_KAITEN:
        stop = 1;
        break;
    case SKLID_NOUSE_IAITATE:
        stop = 2;
        break;
    case SKLID_MAWASIKERI:
        stop = 9;
        break;
    case SKLID_NOUSE_DORIRUPIKE:
    case SKLID_NOUSE_RAINPIKE:
    case SKLID_NOUSE_MEGATONWARI:
        break;
    }
    return stop + 5;
}
#else
INCLUDE_ASM(const s32, "sbtlDMG", sbtlBtlSetDamageProcSBtlDMGSub);
#endif

INCLUDE_ASM(const s32, "sbtlDMG", EffFunc_TateBg);
