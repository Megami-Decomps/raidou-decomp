#include "common.h"
#include "newdata.h"
#include "sBtlDvltk.h"
#include "sbtlWork.h"
#include "fldProcess.h"
#include "sdfChip.h"
#include "sBtlCalc.h"
#include "sdatUnit.h"
#include "dds3WorldBasic.h"
#include "dds3SceneBasic.h"
#include "dds3obj.h"
#include "dds3Basic.h"
#include "sBtlItem.h"
#include "sdk_libc.h"
#include "sdfMemory.h"
#include "sbattle.h"
#include "fld.h"
#include "sBtlUnit.h"
#include "ps2asm.h"
#include "dds3CameraObjectBasic.h"
#include "sBtlSkill.h"
#include "EventBit.h"

INCLUDE_ASM(const s32, "sbtl", sbtlLeaf125);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlRunSklProcSub);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl65);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl64);

INCLUDE_ASM(const s32, "sbtl", sbtlLeaf123);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl68);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl62);

INCLUDE_ASM(const s32, "sbtl", sbtlCallFptodp3);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrWs);

INCLUDE_RODATA(const s32, "sbtl", sbtlSbtlCameraprocTable2);

INCLUDE_RODATA(const s32, "sbtl", sbtlSbtlCameraprocTable3);

INCLUDE_ASM(const s32, "sbtl", sbtlSbtlCameraproc);

s32 sbtlCallSetGameCamera0(void) {
    struct dds3Basic_t *cm;

    wk_sbtl->flg |= 0x80000000;
    cm = dds3GetActiveCameraSceneBasic(dds3GetActiveScene());
    if (cm == NULL) {
        return 0;
    }
    sbtlCallCameraObjectBasicGetFovy(&wk_sbtl->camerabk, cm);
    sbtlCallSetPosPoseSetDds3Basic2(&wk_sbtl->camera);
    return 1;
}

s32 sbtlCallGetActiveScene4(void) {
    dds3CameraObjectBasicWorkData_t *work;

    wk_sbtl->flg &= ~0x80000000;
    if (dds3GetActiveCameraSceneBasic(dds3GetActiveScene()) == NULL) {
        return 0;
    }
    dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), wk_sbtl->camerabk.cm);
    work = wk_sbtl->camerabk.cm->workdata;
    dds3CameraObjectBasicSetFovy(wk_sbtl->camerabk.cm, wk_sbtl->camerabk.fovy);
    work->flag |= 1;
    return 1;
}

s32 sbtlCallBgSetScrollReset(void)
{
    sbtlLeaf118();
    sbtlCallSetPara();
    mapBgSetScrollReset();
    return 1;
}

INCLUDE_ASM(const s32, "sbtl", sbtlSetBossMode);

INCLUDE_ASM(const s32, "sbtl", sbtlInitSBtlSub3);

INCLUDE_ASM(const s32, "sbtl", sbtlLeaf110);

INCLUDE_ASM(const s32, "sbtl", sbtlGetIdling);

INCLUDE_ASM(const s32, "sbtl", Slerp_sbtlCamera);

INCLUDE_ASM(const s32, "sbtl", sbtlIsMoveEnd);

INCLUDE_ASM(const s32, "sbtl", sbtlSetMoveEnd);

INCLUDE_ASM(const s32, "sbtl", GetNowPosAS);

INCLUDE_ASM(const s32, "sbtl", GetNowPosS);

INCLUDE_ASM(const s32, "sbtl", PosCtrlSystem);

INCLUDE_ASM(const s32, "sbtl", CameraMoveSystem);

INCLUDE_ASM(const s32, "sbtl", SetEnemyCPos);

INCLUDE_ASM(const s32, "sbtl", MenuStealth);

INCLUDE_ASM(const s32, "sbtl", sbtlMenuOpen);

INCLUDE_ASM(const s32, "sbtl", sbtlMenuClose);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl67);

INCLUDE_ASM(const s32, "sbtl", SetPara);

s32 sbtlCallCameraObjectBasicGetFovy(SBTLCAMERA_WORK *wk, dds3Basic_t *cm) {
    dds3CameraObjectBasicWorkData_t *work;
    dds3PoseSet_t *poseset;
    f32 distance;

    wk->cm = cm;
    work = cm->workdata;
    poseset = cm->poseset;
    SDF_VU0_LOADVECTOR(&work->targetPos);
    SDF_VU0_STOREVECTOR(&wk->targetpos);
    SDF_VU0_LOADVECTOR(&work->eyeVector);
    SDF_VU0_STOREVECTOR(&wk->direction);
    SDF_VU0_LOADVECTOR(&poseset->pos);
    SDF_VU0_STOREVECTOR(&wk->camerapos);
    SDF_VU0_LOADVECTOR(&wk->targetpos);
    SDF_VU0_LOADVECTOR_B(&wk->camerapos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(distance);
    wk->distance = distance;
    wk->fovy = dds3CameraObjectBasicGetFovy(cm);
    return 1;
}

s32 sbtlCallSetPosPoseSetDds3Basic2(SBTLCAMERA_WORK *wk) {
    dds3CameraObjectBasicWorkData_t *work = wk->cm->workdata;
    sdfVector4_t pos;

    SDF_VU0_LOADVECTOR(&wk->direction);
    SDF_VU0_STOREVECTOR(&work->eyeVector);
    SDF_VU0_SCALEVECTOR(-wk->distance);
    SDF_VU0_LOADVECTOR_B(&wk->targetpos);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_B(&work->targetPos);
    SDF_VU0_STOREVECTOR(&pos);
    dds3SetPosPoseSet(wk->cm, &pos);
    dds3SetRotPoseSet(wk->cm, &sbtlCallSetPosPoseSetDds3Basic2Data2);
    SDF_VU0_LOADVECTOR(&sbtlCallSetPosPoseSetDds3Basic2Data);
    SDF_VU0_STOREVECTOR(&work->vertical);
    dds3CameraObjectBasicSetFovy(wk->cm, wk->fovy);
    work = wk->cm->workdata;
    work->flag |= 1;
    dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), wk->cm);
    return 1;
}

s32 ProcCamera(dds3ProcessID_t *id) {
    return 0;
}

INCLUDE_ASM(const s32, "sbtl", ShutdownCamera);

s32 sbtlLeaf118(void)
{
    return 1;
}

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlSearchUnitID35);

INCLUDE_ASM(const s32, "sbtl", sbtlCallSetPara);

INCLUDE_ASM(const s32, "sbtl", sbtlNoHokan);

INCLUDE_ASM(const s32, "sbtl", sbtlSetMode);

INCLUDE_ASM(const s32, "sbtl", sbtlGetMode);

INCLUDE_ASM(const s32, "sbtl", sbtlCallMemset31);

INCLUDE_ASM(const s32, "sbtl", sbtlLeaf121);

INCLUDE_ASM(const s32, "sbtl", sbtlCallMemset30);

INCLUDE_ASM(const s32, "sbtl", sbtlCallMemset33);

INCLUDE_RODATA(const s32, "sbtl", SetParaTable);

INCLUDE_RODATA(const s32, "sbtl", SetParaTable2);

INCLUDE_RODATA(const s32, "sbtl", SetParaTable3);

INCLUDE_RODATA(const s32, "sbtl", jtbl_004C6860);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckDamageTargetUseItem);

INCLUDE_ASM(const s32, "sbtl", sbtlGetCutFlg);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl56);

INCLUDE_ASM(const s32, "sbtl", sbtlBgnPosSlp);

INCLUDE_ASM(const s32, "sbtl", ClrAction);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl55);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl61);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl59);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl57);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl69);

INCLUDE_ASM(const s32, "sbtl", sbtlLeaf93);

s32 sbtlCheckItemCost_GBWK(s32 itemid, s32 flg) {
    SBTL_PLAYER_DATA *pd = &DDS3_GBWK->playerData;

    if (pd->item[itemid] <= 0) {
        return 0;
    }
    if (!flg) {
        return 1;
    }
    sbtlDecItem_GBWK(itemid);
    return 1;
}

s32 sbtlCheckItemCost(s32 itemid, s32 flg) {
    s32 i;

    for (i = 0; i < wk_sbtl->itemList->num; i++) {
        if (wk_sbtl->itemList->data[i].itemid == itemid) {
            if (wk_sbtl->itemList->data[i].num <= 0) {
                return 0;
            }
            if (!flg) {
                return 1;
            }
            sbtlDecItem(i, itemid);
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "sbtl", sbtlSetWeakBit);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlGetNakama5);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckItemMember);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlSetBadStatusRandSub);

INCLUDE_ASM(const s32, "sbtl", sbtlCallGetPercentage);

INCLUDE_ASM(const s32, "sbtl", sbtlSetBadStatusRand);

INCLUDE_ASM(const s32, "sbtl", CalcItem);

INCLUDE_ASM(const s32, "sbtl", CalcItem_Battle_sbtlCalc);

void sbtlResetItemFlg(sdatUnitWork_t *unit) {
    unit->itemstatus &= ~0x410;
}

INCLUDE_ASM(const s32, "sbtl", sbtlCheckDrink);

s32 sbtlCheckItemCount(sdatUnitWork_t *unit) {
    unit->itemst_cnt2 = 0;
    unit->itemst_cnt = 0;
    unit->itemstatus &= 0x3FFFFCF0;
    sbtlBtlCheckItemCountSub(unit);
    return 1;
}

INCLUDE_ASM(const s32, "sbtl", sbtlCheckHaveskill_Atk);

s32 sbtlCheckHaveskill_Magic(sdatUnitWork_t *unit) {
    s32 type;
    s32 i;

    for (i = 0; i < 17; i++) {
        if (unit->haveskill[i] != 0 && unit->haveskill[i] != SKLID_OFS_PROGRAM && unit->haveskill[i] != SKLID_ATK) {
            type = sbtlGetSkilltbl(unit->haveskill[i])->type;
            if (!(type & 0xF000) && ((type & 0xFFF) == 3 || (type & 0xFFF) == 5)) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "sbtl", sbtlCheckHaveskill_Rec);

s32 sbtlCheckAutoSkill(sdatUnitWork_t *unit, s32 flg) {
    return 0;
}

s32 sbtlCheckAutoSkill_Enemy(struct SBTLUNIT_WORK *wk) {
    s32 i;

    for (i = 0; i < 16; i++) {
        wk->unit.haveskill[i] = wk->haveskill[i];
    }
    CalcAisyo(wk);
    return 1;
}

INCLUDE_ASM(const s32, "sbtl", CalcAisyo);

INCLUDE_ASM(const s32, "sbtl", sbtlCallClearUnitSkills);

INCLUDE_ASM(const s32, "sbtl", sbtlSetAutoSkill_GBID);

INCLUDE_ASM(const s32, "sbtl", sbtlSetAutoSkill_ID);

INCLUDE_ASM(const s32, "sbtl", cmbClearUnitSkills);

INCLUDE_ASM(const s32, "sbtl", sbtlGetAutoSkillPara);

INCLUDE_ASM(const s32, "sbtl", sbtlGetAutoSkill_ID);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckAutoSkill_ActerDamage_ID);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckAutoSkill_TargetDamage_ID);

s32 sbtlCheckAutoSkill_ActerDamage(s32 autono) {
    s32 result = 0;

    switch (autono) {
    case 13:
    case 14:
    case 15:
    case 16:
    case 54:
        result = 1;
        break;
    }
    return result;
}

INCLUDE_ASM(const s32, "sbtl", sbtlCheckAutoSkill_TargetDamage);

s32 sbtlCheckAutoSkill_Action(struct SBTLUNIT_WORK *wk, SBTLDMG_HITWORK *hwk, s32 attr) {
    BST bst;

    if (attr != 0 && !(attr & 0x8000)) {
        return 0;
    }
    if (hwk->attr_flg & 0x100) {
        return 0;
    }
    bst = sbtlGetBadStatus(&wk->unit);
    if (bst == BST_STONE || bst == BST_BOUDO || bst == BST_MAHUU || bst == BST_MOUDOKU || bst == BST_SLEEP
        || bst == BST_BOYCOTT || bst == BST_RAND0) {
        return 1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "sbtl", sbtlCallGetPercentageTable);

INCLUDE_ASM(const s32, "sbtl", CalcAutoSkill);

s32 sbtlGetDropMoney(sdatUnitWork_t *unit, s32 gold) {
    if ((unit->itemstatus & 0x100) != 0) {
        gold += gold / 5;
    }
    return gold;
}

INCLUDE_RODATA(const s32, "sbtl", sbtlStrBDmgupH100);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrMDmgupH100);

INCLUDE_RODATA(const s32, "sbtl", CalcAutoSkillStrBMDmgupH100);

INCLUDE_RODATA(const s32, "sbtl", CalcAutoSkillStrZDmgupH100);

INCLUDE_RODATA(const s32, "sbtl", CalcAutoSkillStrZDmgdowNorBsDowNorJsDown);

INCLUDE_RODATA(const s32, "sbtl", CalcAutoSkillStrRecupH100);

INCLUDE_RODATA(const s32, "sbtl", jtbl_004C6C00);

s32 sbtlCheckMoon(u16 moon) {
    u16 tbl[9] = {0xBC0, 0xBC1, 0xBC2, 0xBC3, 0xBC4, 0xBC5, 0xBC6, 0xBC7, 0xBC8};

    return evtBitCheck(tbl[moon]) != 0;
}

INCLUDE_ASM(const s32, "sbtl", sbtlGetSkillCost);

INCLUDE_ASM(const s32, "sbtl", sbtlGetMagCost);

s32 sbtlCheckUseSkill(struct SBTLUNIT_WORK *wk, SKLID sklid) {
    struct SBTLSKILLP_TBL *tbl = sbtlGetSkilltbl(sklid);
    s32 ret = sbtlCheckCost(wk, 0);

    if (ret != 0) {
        if ((tbl->type & 0xFFF) == 3) {
            if (sbtlGetBadStatus(&wk->unit) == BST_MOUDOKU) {
                return 0;
            }
        }
        return 1;
    }
    return ret;
}

s32 sbtlCheckMagCost_GB(sdatUnitWork_t *player, sdatUnitWork_t *unit, s32 devilid, s8 flg) {
    return 1;
}

INCLUDE_ASM(const s32, "sbtl", sbtlCheckMagCost);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckCost2);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckCost);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckCost_SkillID);

INCLUDE_ASM(const s32, "sbtl", sbtlDecVice);

INCLUDE_ASM(const s32, "sbtl", sbtlDecMag);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl71);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl54);

INCLUDE_ASM(const s32, "sbtl", sbtlDecHP);

INCLUDE_ASM(const s32, "sbtl", sbtlDecMP);

INCLUDE_ASM(const s32, "sbtl", sbtlDecItem_GBWK);

INCLUDE_ASM(const s32, "sbtl", sbtlDecItem);

INCLUDE_ASM(const s32, "sbtl", sbtlSetGunNum);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl66);

s32 sbtlGetGun_Num(void)
{
    return wk_sbtl->hgunNum;
}

SBTL_AISYO_TBL *sbtlGetAisyotbl(s32 id)
{
    return &aisyo_tbldata[id];
}

INCLUDE_ASM(const s32, "sbtl", sbtlCheckAttrFlg);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckAisyo_Old);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlCheckAisyoExSub);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckAisyoEx);

INCLUDE_ASM(const s32, "sbtl", WeakBstDvlTk);

INCLUDE_ASM(const s32, "sbtl", LimitRange);

INCLUDE_ASM(const s32, "sbtl", sbtlAutoRec);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlGetAutoSkillPara);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckSpecialDamage_Stop);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlChkHaveMemSkl2);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckBadStatus_Return);

/* body: matched in raidou @ 780f3f57: src/raidou1/sbtl_0029BFB8.c:324, found, same function 1.00 (words 18/18, calls 1.00, shape 1.00) */
BST sbtlGetBadStatus(sdatUnitWork_t *unit)
{
    s32 i;

    if (unit->badStatus != 0) {
        for (i = BST_KANASHIB; i < BST_RAND0; i++) {
            if ((unit->badStatus >> i) & 1) {
                return i;
            }
        }
    }
    return BST_BOYCOTT;
}

INCLUDE_ASM(const s32, "sbtl", CalcBadStatus);

INCLUDE_ASM(const s32, "sbtl", sbtlResetBadStatus_GB);

INCLUDE_ASM(const s32, "sbtl", sbtlResetBadStatus);

INCLUDE_RODATA(const s32, "sbtl", CalcBadStatusTable3);

INCLUDE_RODATA(const s32, "sbtl", CalcBadStatusTable4);

s32 sbtlGetBadStatus_Attr(BST id)
{
    s32 table[21] = { 2, 3, 4, 5, 0, 6, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 1, 7, 7, 7 };

    return table[id];
}

INCLUDE_ASM(const s32, "sbtl", sbtlCheckBadStatus);

INCLUDE_ASM(const s32, "sbtl", sbtlCallCheckBadStatus);

INCLUDE_ASM(const s32, "sbtl", sbtlResetUnit);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckBadStatus_Number);

INCLUDE_RODATA(const s32, "sbtl", sbtlLeaf107Table);

INCLUDE_ASM(const s32, "sbtl", sbtlCallSetClutColor);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckBadStatus_Weak);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlSetBadStatusNoSub);

INCLUDE_ASM(const s32, "sbtl", sbtlSetBadStatus_No);

INCLUDE_ASM(const s32, "sbtl", sbtlRefGlobalPID);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlSetBadStatusNoSub2);

INCLUDE_ASM(const s32, "sbtl", sbtlCallSetClutColor2);

INCLUDE_ASM(const s32, "sbtl", sbtlSetBadStatusAction);

INCLUDE_ASM(const s32, "sbtl", SetBadStatusCounter);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlCheckBadStatusWeakSub);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckBadStatus_WeakNumber);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckBadStatus_Hit);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlGetSkilltbl21);

INCLUDE_ASM(const s32, "sbtl", sbtlKudaDmg);

INCLUDE_RODATA(const s32, "sbtl", sbtlKudaDmgTable);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrZKeisaNorKeisanShimuA100);

INCLUDE_ASM(const s32, "sbtl", sbtlFullMoonDmg);

INCLUDE_ASM(const s32, "sbtl", sbtlZokuseiDmg);

INCLUDE_ASM(const s32, "sbtl", sbtlGetDamage_sbtlCalc);

INCLUDE_ASM(const s32, "sbtl", sbtlResetBattlePara);

INCLUDE_ASM(const s32, "sbtl", sbtlGetPlayerTec_ParaUp);

void sbtlCheckBattlePara(struct SBTLUNIT_WORK *wk)
{
    s16 max = 40;
    sdatUnitWork_t *unit;
    sdatParaWork_t para;

    CalcAisyo(wk);
    sbtlCheckStockPara(&wk->unit);
    wk->unit.now.str += wk->kajyapara.str;
    wk->unit.now.vit += wk->kajyapara.vit;
    wk->unit.now.tec += wk->kajyapara.tec;
    wk->unit.now.luck += wk->kajyapara.luck;
    if (!(wk->field414 & 0x80) && (wk->field414 & 1)) {
        unit = &sbtlGetPlayerData()->unit;
        if (unit->flag & 1) {
            memset(&para, 0, sizeof(para));
            sbtlGetPlayerTec_ParaUp(unit, &para);
            wk->unit.now.str += para.str;
            wk->unit.now.vit += para.vit;
            wk->unit.now.tec += para.tec;
            wk->unit.now.luck += para.luck;
        }
    }
    CalcAisyo_sbtlCalc(wk);
    if (wk->field414 & 2) {
        max = 99;
    }
    wk->unit.now.str = wk->unit.now.str > 0 ? (wk->unit.now.str < max ? wk->unit.now.str : max) : 0;
    wk->unit.now.vit = wk->unit.now.vit > 0 ? (wk->unit.now.vit < max ? wk->unit.now.vit : max) : 0;
    wk->unit.now.tec = wk->unit.now.tec > 0 ? (wk->unit.now.tec < max ? wk->unit.now.tec : max) : 0;
    wk->unit.now.luck = wk->unit.now.luck > 0 ? (wk->unit.now.luck < max ? wk->unit.now.luck : max) : 0;
}

INCLUDE_ASM(const s32, "sbtl", CalcAisyo_sbtlCalc);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlGetDamageSBtlCalcSub);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlCheckAisyoOld);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlCheckAisyoOld3);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlCheckAisyoOld4);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlCheckAisyoOld5);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlCheckAisyoOld6);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlGetDamageSBtlCalcSub2);

INCLUDE_ASM(const s32, "sbtl", sbtlGetDamage_The_W);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlCheckAisyoOld7);

INCLUDE_ASM(const s32, "sbtl", sbtlGetDamage_MRecover);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlCheckAisyoOld2);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlGetDamageSBtlCalcSub3);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlGetDamageSBtlCalcSub4);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckRegistKaja);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckRegistKajaMaxNewAI);

INCLUDE_ASM(const s32, "sbtl", sbtlGetDamage_ParaUP);

INCLUDE_ASM(const s32, "sbtl", sbtlLeaf115);

INCLUDE_RODATA(const s32, "sbtl", sbtlBtlGetDamageGBSubTable);

INCLUDE_RODATA(const s32, "sbtl", sbtlGetPlayerTec_ParaUpTable);

INCLUDE_ASM(const s32, "sbtl", sbtlITEMRECUP);

INCLUDE_ASM(const s32, "sbtl", sbtlITEMRECUP2);

INCLUDE_ASM(const s32, "sbtl", sbtlGetDamage_I_REC_PARA);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlCheckItemCountSub);

INCLUDE_ASM(const s32, "sbtl", sbtlGetDamage_P_STR);

INCLUDE_ASM(const s32, "sbtl", sbtlGetDamage_P_VIT);

INCLUDE_ASM(const s32, "sbtl", sbtlGetDamage_P_TEC);

INCLUDE_ASM(const s32, "sbtl", sbtlGetDamage_P_LUCK);

INCLUDE_ASM(const s32, "sbtl", sbtlGetDamage_CHUUSEI);

INCLUDE_ASM(const s32, "sbtl", sbtlGetDamage_ITEMFLG);

INCLUDE_ASM(const s32, "sbtl", sbtlLeaf97);

INCLUDE_ASM(const s32, "sbtl", sbtlGetViceLevel);

INCLUDE_ASM(const s32, "sbtl", sbtlRecalcAisyoMember);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlLoadHuumaMesDataSub);

SBTLDVLTK_LISTWORK *sbtlCallChipAllocZeroDirect(void)
{
    SBTLDVLTK_LISTWORK *lwk = sdfChipAllocZeroDirect(sizeof(SBTLDVLTK_LISTWORK));

    lwk->num = 0;
    lwk->dwk = NULL;
    return lwk;
}

INCLUDE_ASM(const s32, "sbtl", sbtlLeaf101);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrBattleDvltkDvltkmes013);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrBattleDvltkDvltkmes012);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrBattleDvltkDvltkmes011);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrBattleDvltkDvltkmes010);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrBattleDvltkDvltkmes00F);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrBattleDvltkDvltkmes00E);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrBattleDvltkDvltkmes00D);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrBattleDvltkDvltkmes00C);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrBattleDvltkDvltkmes00B);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrBattleDvltkDvltkmes00A);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrBattleDvltkDvltkmes009);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrBattleDvltkDvltkmes008);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrBattleDvltkDvltkmes007);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrBattleDvltkDvltkmes006);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrBattleDvltkDvltkmes005);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrBattleDvltkDvltkmes004);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrBattleDvltkDvltkmes003);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrBattleDvltkDvltkmes002);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrBattleDvltkDvltkmes001);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrBattleDvltkDvltkmes000);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon0CaTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon0C9Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon0C8Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon0C7Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon0C6Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon0C5Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon0C4Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon0C3Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon0C2Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon0C1Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon0C0Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon0BbTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon0BaTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon0B9Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon0B8Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon0B7Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon0B6Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon0B5Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon0B4Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon0AfTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon0AeTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon0AdTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon0AcTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon0A7Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon0A6Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon0A5Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon0A4Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon0A3Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon0A2Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon09ETmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon09DTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon09CTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon09BTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon09ATmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon099Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon098Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon097Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon096Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon095Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon094Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon093Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon092Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon091Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon090Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon08FTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon08ETmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon08DTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon08CTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon08BTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon08ATmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon089Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon088Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon087Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon082Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon081Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon080Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon07FTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon07ETmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon07DTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon07CTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon07BTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon07ATmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon079Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon078Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon077Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon076Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon075Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon074Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon073Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon072Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon071Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon070Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon06BTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon06ATmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon069Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon068Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon067Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon066Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon065Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon064Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon063Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon062Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon061Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon060Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon05FTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon05ETmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon05DTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon05CTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon05BTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon05ATmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon059Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon058Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon057Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon056Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon051Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon050Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon04FTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon04ETmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon04DTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon04CTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon04BTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon04ATmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon049Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon048Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon047Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon046Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon045Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon044Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon043Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon042Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon041Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon03CTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon03BTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon03ATmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon039Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon038Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon037Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon036Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon035Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon034Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon033Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon032Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon031Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon030Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon02FTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon02ETmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon02DTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon02CTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon027Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon026Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon025Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon024Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon023Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon022Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon021Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon020Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon01FTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon01ETmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon01DTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon01CTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon01BTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon01ATmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon019Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon018Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon017Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon016Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon011Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon010Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon00FTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon00ETmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon00DTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon00CTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon00BTmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon00ATmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon009Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon008Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon007Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon006Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon005Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon004Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon003Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon002Tmx);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrIcon001Tmx);

INCLUDE_ASM(const s32, "sbtl", sbtlCreateWork);

INCLUDE_ASM(const s32, "sbtl", sbtlCreateDvltk_Priority);

INCLUDE_ASM(const s32, "sbtl", sbtlCreateDvltk);

INCLUDE_ASM(const s32, "sbtl", Proc_sbtlDvltk);

INCLUDE_ASM(const s32, "sbtl", sbtlSearchDvltk);

INCLUDE_ASM(const s32, "sbtl", Draw_sbtlDvltk);

s32 sbtlReleaseDvltk_List(SBTLDVLTK_LISTWORK *stp, u8 flg)
{
    SBTLDVLTK_WORK *node;
    SBTLDVLTK_WORK *next;

    if (stp == NULL) {
        return 0;
    }
    node = stp->dwk;
    if (node == NULL || stp->num == 0) {
        return 0;
    }
    do {
        if (flg) {
            node->status = 2;
        }
        if (node->status == 2) {
            if (node->prev != NULL) {
                node->prev->next = node->next;
            }
            if (node->next != NULL) {
                node->next->prev = node->prev;
            }
            if (node->prev == NULL) {
                stp->dwk = node->next;
                if (stp->dwk != NULL) {
                    stp->dwk->prev = NULL;
                }
            }
            next = node->next;
            sdfMemFreeDirect(node->handle);
            node = next;
            stp->num--;
        } else {
            node = node->next;
        }
    } while (node != NULL);
    return 1;
}

INCLUDE_ASM(const s32, "sbtl", sbtlDeleteDvltkList);

INCLUDE_ASM(const s32, "sbtl", sbtlCallMessageNumSearch3);

INCLUDE_ASM(const s32, "sbtl", sbtlLeaf99);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlSearchUnitID34);

INCLUDE_ASM(const s32, "sbtl", sbtlDrawTextFont);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlDrawTextSub2);

struct sprHandle_t *sbtlGetSpriteHandle(void)
{
    if (sbtlData104 == NULL) {
        if (wk_sbtl->panel != NULL) {
            return wk_sbtl->panel->hPanelSpr5;
        }
        return NULL;
    }
    return sbtlData105;
}

INCLUDE_ASM(const s32, "sbtl", sbtlExt_CalcProcess);

INCLUDE_ASM(const s32, "sbtl", sbtlExt_DrawProcess);

INCLUDE_ASM(const s32, "sbtl", sbtlExt_Free);

INCLUDE_ASM(const s32, "sbtl", sbtlExt_ShutDown);

INCLUDE_RODATA(const s32, "sbtl", sbtlLeaf99Table);

INCLUDE_RODATA(const s32, "sbtl", sbtlDrawTextTable);

INCLUDE_ASM(const s32, "sbtl", sbtlExt_CreateBuffer);

INCLUDE_ASM(const s32, "sbtl", sbtlExt_FreeBuffer);

INCLUDE_ASM(const s32, "sbtl", sbtlExt_MesReqEx);

INCLUDE_ASM(const s32, "sbtl", sbtlExt_MesReq);

INCLUDE_ASM(const s32, "sbtl", sbtlExt_EndMes);

s32 sbtlExt_ChkMes(void) {
    return sbtlData104 != NULL;
}

INCLUDE_ASM(const s32, "sbtl", sbtlExtCommand_DVLTK_DSP);

INCLUDE_ASM(const s32, "sbtl", sbtlExtCommand_DVLTK_WAIT);
