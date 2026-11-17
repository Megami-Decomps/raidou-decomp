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

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCalcInOutPara);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlGetPlayerLuck);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlGetPlayerLuckStep);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlGetCorrectionParamLuck);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlPurificationPlayerLuck);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetPlayerLuck);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlITMUNKUIUpdown);

INCLUDE_ASM(const s32, "sbtlDMG", datSetKillDevil);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlRefSbtl15);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetAddMag_Setsuma);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCallBitCheck2);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCallBtlChkHaveMemSkl);

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

INCLUDE_ASM(const s32, "sbtlDMG", sbtlRegistAnimList);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlReleaseAnim_ID);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlDeleteAnimList);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlAnimListClr);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlAnimListProc_sbtlDMG);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlDrawAnimList);

SBTLDMG_HITLISTWORK *sbtlCreateHitList(void) {
    SBTLDMG_HITLISTWORK *lwk;

    lwk = sdfChipAllocZeroDirect(sizeof(SBTLDMG_HITLISTWORK));
    lwk->num = 0;
    lwk->wk = NULL;
    lwk->serialID = 1;
    return lwk;
}

INCLUDE_ASM(const s32, "sbtlDMG", sbtlLoadAttackHitData_Decode);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSearchAttackHitData_Copy);

s32 sbtlLoadAttackHitData_pData(s32 index, void *pData) {
    while (sbtlLoadAttackHitData(&sbtlWork2, 0, pData) == 0) {
    }
    return 1;
}

INCLUDE_ASM(const s32, "sbtlDMG", sbtlInitLoadAttackHitData_pData);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlForceDelete_AttackHitData);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlBtlLoadAttackHitDataDecodeSub);

INCLUDE_RODATA(const s32, "sbtlDMG", CreateAnimTable);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlLoadAttackHitData);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCreateHitC_New);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlDltHit_by_UnitSkl);

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

INCLUDE_ASM(const s32, "sbtlDMG", sbtlGetKaraburiNum);

INCLUDE_ASM(const s32, "sbtlDMG", UnsetKaraburiSub);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlUnsetKaraburi);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlClearHitC);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlResetIdouPara);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlHitCProc_Idou);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCheckHitArea_Single);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlHitCProc);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCheckActDamage);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCheckGround);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCallMemset7);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetHit);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCallColorSet2);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCallColorSet3);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlDrawAttachEPL);

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

INCLUDE_ASM(const s32, "sbtlDMG", sbtlDeleteHitList);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCreateTargetList_Normal);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCheckTargetList_csr);

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

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCallBtlGetSkilltbl7);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCallBtlCheckPlayerDead);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCHitFunc_ATTACK);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlGetSide);

INCLUDE_ASM(const s32, "sbtlDMG", CheckDamageTarget);

s32 sbtlCheckPlayerDead(void) {
    struct SBTLUNIT_WORK *wk;

    wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    if (wk != NULL && (wk->status == 34 || wk->status == 10 || wk->status == 40)) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCheckDamageTarget_Skilltbl);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSearchDMG_Unit);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetEntryUnitList_Hit);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCheckALLSkill);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlDamageProc);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetDeadDamageEx);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlRefSbtl16);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetKudaDamage);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCallBtlCreateAnimUnit);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSEISHINCNTR);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetDamageProc);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlIsAura);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlUpdateDamageKaja);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlGetCalcDamageEx);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrEienboudoAtk20);

INCLUDE_RODATA(const s32, "sbtlDMG", sbtlStrEienboudoDmg15);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlGetCalcDamage);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetReflectDamage);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlMAGWAZAMagNot);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCallBtlGetSkilltbl8);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetDamageProc_sbtlDMG);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlGetHitUnit_Muki);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetHitback);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetStoneDeadDamageProc);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCallBtlSetWeakBit);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCallBtlGetSkilltbl9);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetInvalidDamage);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCallBtlGetStartTimeDamage);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetAbsorbDamage);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlGuardHit);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCallBtlCreateAnimUnit2);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCallMemset8);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlKOUCYOKUACT);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCallBtlSetWeakBit2);

INCLUDE_ASM(const s32, "sbtlDMG", CheckAutoSkill);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlIsEnemyLastOne);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCallBtlSetWeakBit3);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetSpecialDamage);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetSpecialDamage_DamageOnly);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlSetDamage_L_EnemyAll);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlCallBtlCreateSideListSBtlUnit4);

INCLUDE_ASM(const s32, "sbtlDMG", sbtlBtlSetDamageProcSBtlDMGSub);

INCLUDE_ASM(const s32, "sbtlDMG", EffFunc_TateBg);
