#include "common.h"
#include "dds3KernelCore.h"
#include "dds3Basic.h"
#include "dds3ObjectBase.h"
#include "sdk_libc.h"
#include "sbtlWork.h"
#include "sBtlUnit.h"
#include "sBtlCalc.h"
#include "ps2asm.h"
#include "basic.h"
#include "dds3CameraObjectBasic.h"
#include "sBtlSkill.h"
#include "dat.h"
#include "sdatUnit.h"
#include "sbtlDMG.h"
#include "EventBit.h"
#include "newdata.h"
#include "dds3WorldBasic.h"
#include "dds3SceneBasic.h"
#include "dds3obj.h"
#include "sBtlItem.h"
#include "sBtlPlayer.h"
#include "sBtlEXP.h"

/* body: matched in raidou @ 61f09dc: src/raidou1/code_002929C8.c:124, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
s32 sbtlLeaf125(void) {
    return 1;
}

void sbtlSbtlCameraproc(void) {
    sdfVector4_t target = { 0.0f };
    sdfVector4_t pos = { -507.7f, -435.3f, -973.36f, 0.0f };
    sdfVector4_t rot = { 0.16f, -0.22f, -0.04f, 0.89f };
    dds3ProcessID_t *pid;
    struct dds3Basic_t *cm;
    SBTLCAMERA_WORK *cam;

    wk_sbtl->camera.cm = NULL;
    wk_sbtl->camerabk.cm = NULL;
    pid = dds3StartProcess("sbtl_cameraproc", 0x40A, 1, 1, sbtlLeaf103, sbtlLeaf87, NULL);
    target.x += main_data.val20;
    target.z += main_data.val24;
    pos.x += main_data.val20;
    pos.z += main_data.val24;
    cm = dds3AddCameraObjectBasicTypeFree(dds3AllocObjectId(), &pos, &rot);
    dds3SetUnitObjectId(cm, "sbtl_scam");
    dds3GetRadiusPoseSet(cm, 10.0f);
    dds3ObjectBaseCreateMover(cm);
    wk_sbtl->camera.cm = cm;
    wk_sbtl->camera.fovy = 0.6981317f;
    dds3CameraObjectBasicSetFovy(cm, 0.6981317f);
    sbtlCallCameraObjectBasicGetFovy(&wk_sbtl->camera, cm);
    cam = &wk_sbtl->camera;
    SDF_VU0_LOADVECTOR(&target);
    SDF_VU0_STOREVECTOR_NOMEM(&cam->targetpos);
    SDF_VU0_LOADVECTOR(&cam->targetpos);
    SDF_VU0_LOADVECTOR_B(&cam->camerapos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(cam->distance);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&cam->direction);
    dds3LinkProcess(wk_sbtl->process, pid);
}

s32 sbtlCallSetGameCamera0(void) {
    struct dds3Basic_t *cm;

    wk_sbtl->flg |= 0x400000;
    pbSetGameCamera0();
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

    wk_sbtl->flg &= ~0x400000;
    pbLeaf19();
    if (dds3GetActiveCameraSceneBasic(dds3GetActiveScene()) == NULL) {
        return 0;
    }
    dds3SetActiveCameraSceneBasic(dds3GetActiveScene(), wk_sbtl->camerabk.cm);
    work = wk_sbtl->camerabk.cm->workdata;
    dds3CameraObjectBasicSetFovy(wk_sbtl->camerabk.cm, wk_sbtl->camerabk.fovy);
    work->flag |= 1;
    return 1;
}

s32 sbtlRefSbtl65(void) {
    if (wk_sbtl->flg & 0x400000) {
        return 1;
    }
    return 0;
}

s32 sbtlCallSetPosPoseSetDds3Basic(SBTLCAMERA_WORK *wk) {
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
    dds3SetRotPoseSet(wk->cm, &sbtlCallSetPosPoseSetDds3BasicData2);
    SDF_VU0_LOADVECTOR(&sbtlCallSetPosPoseSetDds3BasicData);
    SDF_VU0_STOREVECTOR(&work->vertical);
    dds3CameraObjectBasicSetFovy(wk->cm, wk->fovy);
    work->flag |= 1;
    return 1;
}

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

s32 sbtlLeaf103(dds3ProcessID_t *id) {
    return 0;
}

s32 sbtlLeaf87(dds3ProcessID_t *id)
{
#ifdef FIX_BUGS
    return 0;
#endif
}

s32 sbtlCallBitCheck5(void) {
    if (evtBitCheck(161) && !evtBitCheck(163)) {
        return 1;
    }
    return 0;
}

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

/* NON_MATCHING: 68 of 72 words match.  */
#ifdef NON_MATCHING
void sbtlSetWeakBit(s32 devilid) {
    u32 pair = 0;

    DDS3_GBWK->anlyzBit[devilid / 32] |= 1 << devilid;
    switch (devilid) {
    case 96:
        pair = 235;
        break;
    case 104:
        pair = 234;
        break;
    case 126:
        pair = 214;
        break;
    case 136:
        pair = 236;
        break;
    case 214:
        pair = 136;
        break;
    case 235:
        pair = 96;
        break;
    case 234:
        pair = 104;
        break;
    case 236:
        pair = 126;
        break;
    }
    if (pair != 0) {
        DDS3_GBWK->anlyzBit[pair / 32] |= 1 << pair;
    }
}
#else
INCLUDE_ASM(const s32, "sbtl", sbtlSetWeakBit);
#endif

s32 sbtlCheckItemMember_Dead(s32 index) {
    struct SBTLUNIT_WORK *wk;
    BST bst;

    if (wk_sbtl->field344 > 0 && wk_sbtl->summonIndex[0] == index) {
        wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[index]);
        if (wk != NULL && !CheckLiveTarget(wk)) {
            sbtlResetBadStatus(wk, wk->unit.badStatus);
            wk->unit = *sbtlGetStockUnitData(index);
            bst = sbtlGetBadStatus(&wk->unit);
            if (bst != BST_BOYCOTT) {
                sbtlSetBadStatus_No(wk, SKLID_NONE, bst, 1, -1);
            }
        }
    }
    return 1;
}

s32 sbtlCheckItemMember(s32 index) {
    struct SBTLUNIT_WORK *wk;
    BST bst;

    if (wk_sbtl->field344 > 0 && wk_sbtl->summonIndex[0] == index) {
        wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[index]);
        if (wk != NULL) {
            if (wk_sbtl->field358[index] != 2) {
                sbtlResetBadStatus(wk, wk->unit.badStatus);
                wk->unit = *sbtlGetStockUnitData(index);
                bst = sbtlGetBadStatus(&wk->unit);
                if (bst != BST_BOYCOTT && !(wk->mode == 1 || wk->mode == 2)) {
                    sbtlSetBadStatus_No(wk, SKLID_NONE, bst, 1, -1);
                }
            } else {
                if (!CheckLiveTarget(wk)) {
                    sbtlResetVicePara(wk_sbtl->expList);
                    wk->unit.vice = wk_sbtl->expList->field10;
                    sbtlBtlSetEndDataSub(wk, index);
                }
            }
        }
    }
    return 1;
}

s32 CalcItem(sdatUnitWork_t *unit, s32 itemid) {
    SBTLCALC_DMGWORK wk;
    struct SBTLSKILLP_TBL *ptbl;
    SKLID sklid;
    s32 damage;

    damage = 0;
    sklid = sbtlGetItemtbl(itemid)->sklid;
    if (sklid != SKLID_NONE) {
        ptbl = sbtlGetSkilltbl(sklid);
        if ((ptbl->flg & 0x80) && (ptbl->attr & 0x400)) {
            damage = sbtlLeaf97(unit);
        }
        if (ptbl->flg & 0x1000) {
            wk.n0 = ptbl->n0;
            damage = sbtlGetDamage_I_REC_K(unit, &wk, ptbl, 0);
        }
        if (ptbl->flg & 0x2000) {
            wk.n1 = ptbl->n1;
            damage = sbtlGetDamage_I_REC_P(unit, &wk, ptbl, 0);
        }
        if (ptbl->flg & 0x4000) {
            wk.n0 = ptbl->n0;
            damage = sbtlGetDamage_I_REC_K(unit, &wk, ptbl, 1);
        }
        if (ptbl->flg & 0x8000) {
            wk.n1 = ptbl->n1;
            damage = sbtlGetDamage_I_REC_P(unit, &wk, ptbl, 1);
        }
        if (ptbl->flg & 0x10000) {
            damage = sbtlGetDamage_I_REC_PARA(unit, &wk, ptbl);
        }
        if (ptbl->attrtime > 0 && (ptbl->flg & 0x1E0000)) {
            sbtlBtlCheckItemCountSub(unit);
        }
        if (ptbl->flg & 0x20000) {
            damage = sbtlGetDamage_P_STR(unit, &wk, ptbl, itemid);
        }
        if (ptbl->flg & 0x40000) {
            damage = sbtlGetDamage_P_VIT(unit, &wk, ptbl, itemid);
        }
        if (ptbl->flg & 0x80000) {
            damage = sbtlGetDamage_P_TEC(unit, &wk, ptbl, itemid);
        }
        if (ptbl->flg & 0x100000) {
            damage = sbtlGetDamage_P_LUCK(unit, &wk, ptbl, itemid);
        }
        if (ptbl->flg & 0x40) {
            damage = sbtlGetDamage_CHUUSEI(unit, &wk, ptbl);
        }
        if (ptbl->flg & 0x80) {
            damage = sbtlGetDamage_ITEMFLG(unit, &wk, ptbl, itemid);
        }
        if (ptbl->badst & 0x10000) {
            if (GetPercentage() < 50) {
                if (sbtlCheckBadStatus_Number(unit->badStatus, BST_STONE)
                    && sbtlCheckBadStatus_Hit(unit->aisyo.word, unit->now.luck, unit, unit->now.luck, sklid, BST_STONE,
                                              sbtlGetBadStatus_Attr(BST_STONE), 1)) {
                    unit->badst_cnt = 0;
                    unit->badStatus = (unit->badStatus & ~BSTF_MOUDOKU) | BSTF_STONE;
                }
            } else {
                if (sbtlCheckBadStatus_Number(unit->badStatus, BST_MOUDOKU)
                    && sbtlCheckBadStatus_Hit(unit->aisyo.word, unit->now.luck, unit, unit->now.luck, sklid, BST_MOUDOKU,
                                              sbtlGetBadStatus_Attr(BST_MOUDOKU), 1)) {
                    unit->badst_cnt = 0;
                    unit->badStatus = (unit->badStatus & ~BSTF_STONE) | BSTF_MOUDOKU;
                }
            }
        }
    }
    return damage;
}

s32 CalcItem_Battle_sbtlCalc(struct SBTLUNIT_WORK *wk, sdatUnitWork_t *unit, s32 itemid) {
    SBTLCALC_DMGWORK dwk;
    struct SBTLSKILLP_TBL *ptbl;
    SKLID sklid;
    s32 damage;

    damage = 0;
    sklid = sbtlGetItemtbl(itemid)->sklid;
    if (sklid != SKLID_NONE) {
        ptbl = sbtlGetSkilltbl(sklid);
        if ((ptbl->flg & 0x80) && (ptbl->attr & 0x400)) {
            damage = sbtlLeaf97(unit);
        }
        if (ptbl->flg & 0x1000) {
            dwk.n0 = ptbl->n0;
            damage = sbtlGetDamage_I_REC_K(unit, &dwk, ptbl, 0);
            sbtlCreateAnim_Unit(wk, damage, 0, 3);
        }
        if (ptbl->flg & 0x2000) {
            dwk.n1 = ptbl->n1;
            damage = sbtlGetDamage_I_REC_P(unit, &dwk, ptbl, 0);
            sbtlCreateAnim_Unit(wk, damage, 0, 3);
        }
        if (ptbl->flg & 0x4000) {
            dwk.n0 = ptbl->n0;
            damage = sbtlGetDamage_I_REC_K(unit, &dwk, ptbl, 1);
            sbtlCreateAnim_Unit(wk, damage, 12, 4);
        }
        if (ptbl->flg & 0x8000) {
            dwk.n1 = ptbl->n1;
            damage = sbtlGetDamage_I_REC_P(unit, &dwk, ptbl, 1);
            sbtlCreateAnim_Unit(wk, damage, 12, 4);
        }
        if (ptbl->flg & 0x10000) {
            damage = sbtlGetDamage_I_REC_PARA(unit, &dwk, ptbl);
        }
        if (ptbl->attrtime > 0 && (ptbl->flg & 0x1E0000)) {
            sbtlBtlCheckItemCountSub(unit);
        }
        if (ptbl->flg & 0x20000) {
            damage = sbtlGetDamage_P_STR(unit, &dwk, ptbl, itemid);
        }
        if (ptbl->flg & 0x40000) {
            damage = sbtlGetDamage_P_VIT(unit, &dwk, ptbl, itemid);
        }
        if (ptbl->flg & 0x80000) {
            damage = sbtlGetDamage_P_TEC(unit, &dwk, ptbl, itemid);
        }
        if (ptbl->flg & 0x100000) {
            damage = sbtlGetDamage_P_LUCK(unit, &dwk, ptbl, itemid);
        }
        if (ptbl->flg & 0x40) {
            damage = sbtlGetDamage_CHUUSEI(unit, &dwk, ptbl);
        }
        if (ptbl->flg & 0x80) {
            damage = sbtlGetDamage_ITEMFLG(unit, &dwk, ptbl, itemid);
        }
    }
    return damage;
}

void sbtlResetItemFlg(sdatUnitWork_t *unit) {
    unit->itemstatus &= ~0x410;
}

s32 sbtlCheckDrink(void) {
    s32 ret = 0;
    SBTL_PLAYER_DATA *pd = sbtlGetPlayerData();
    sdatUnitWork_t *unit = &pd->unit;

    if (unit->itemst_cnt2 != 0) {
        if (sbtlCheckMoon(0)) {
            if (!(unit->itemstatus & 0x40000000)) {
                if (!(unit->itemstatus & 0x80000000)) {
                    unit->itemstatus |= 0x40000000;
                    unit->itemst_cnt--;
                } else {
                    unit->itemstatus = (unit->itemstatus & 0x7FFFFFFF) | 0x40000000;
                    unit->itemst_cnt--;
                }
            }
        } else {
            unit->itemstatus &= ~0x40000000;
        }
        if (unit->itemst_cnt <= 0) {
            ret = unit->itemst_cnt2;
            sbtlCheckItemCount(unit);
            datCallGetPlayerMaxHP(unit, pd->val146);
        }
    }
    return ret;
}

s32 sbtlCheckItemCount(sdatUnitWork_t *unit) {
    unit->itemst_cnt2 = 0;
    unit->itemst_cnt = 0;
    unit->itemstatus &= 0x3FFFFCF0;
    sbtlBtlCheckItemCountSub(unit);
    return 1;
}

/* NON_MATCHING: 35 of 42 words match.  */
#ifdef NON_MATCHING
s32 sbtlCheckHaveskill_Atk(sdatUnitWork_t *unit) {
    struct SBTLSKILLP_TBL *tbl;
    s32 type;
    s32 i;

    for (i = 0; i < 10; i++) {
        if (unit->haveskill[i] != 0 && unit->haveskill[i] != SKLID_OFS_PROGRAM) {
            tbl = sbtlGetSkilltbl(unit->haveskill[i]);
            type = tbl->type & 0xFFF;
            if (!(tbl->type & 0xF000)) {
                if (tbl->attr & 1) {
                    return 1;
                }
                if (type < 3 && type != 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "sbtl", sbtlCheckHaveskill_Atk);
#endif

s32 sbtlCheckHaveskill_Magic(sdatUnitWork_t *unit) {
    s32 type;
    s32 i;

    for (i = 0; i < 10; i++) {
        if (unit->haveskill[i] != 0 && unit->haveskill[i] != SKLID_OFS_PROGRAM && unit->haveskill[i] != SKLID_ATK) {
            type = sbtlGetSkilltbl(unit->haveskill[i])->type;
            if (!(type & 0xF000) && ((type & 0xFFF) == 3 || (type & 0xFFF) == 5)) {
                return 1;
            }
        }
    }
    return 0;
}

s32 sbtlCheckHaveskill_Rec(sdatUnitWork_t *unit) {
    struct SBTLSKILLP_TBL *tbl;
    s32 i;

    for (i = 0; i < 10; i++) {
        if (unit->haveskill[i] != 0 && unit->haveskill[i] != SKLID_OFS_PROGRAM && unit->haveskill[i] != SKLID_ATK) {
            tbl = sbtlGetSkilltbl(unit->haveskill[i]);
            if (!(tbl->type & 0xF000) && (tbl->flg & 0x24)) {
                return 1;
            }
        }
    }
    return 0;
}

/* body: matched in raidou @ 9c5e6e0: src/raidou2/sbtl.c:202, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
s32 sbtlCheckAutoSkill(sdatUnitWork_t *unit, s32 flg) {
    return 0;
}

s32 sbtlCheckAutoSkill_Enemy(struct SBTLUNIT_WORK *wk) {
    s32 i;

    for (i = 0; i < 9; i++) {
        wk->unit.haveskill[i] = wk->haveskill[i];
    }
    CalcAisyo(wk);
    return 1;
}

s32 CalcAisyo(struct SBTLUNIT_WORK *wk) {
    sdatUnitWork_t *unit = &wk->unit;
    SKLID id;

    id = sbtlGetAutoSkill_ID(unit, 0);
    if (id != 0) {
        sbtlSetAutoSkill_ID(wk, id);
    }
    id = sbtlGetAutoSkill_ID(unit, 1);
    if (id != 0) {
        sbtlSetAutoSkill_ID(wk, id);
    }
    return 1;
}

s32 sbtlSetAutoSkill_GB(sdatUnitWork_t *unit) {
    s32 id;

    sbtlResetAutoSkill(unit);
    id = sbtlGetAutoSkillPara(sbtlGetAutoSkill_ID(unit, 0), &unit->autost_cnt, &unit->autost_cnt2);
    if (id != -1) {
        sbtlSetAutoSkill_GBID(unit, id);
    }
    id = sbtlGetAutoSkillPara(sbtlGetAutoSkill_ID(unit, 1), &unit->autost_cnt, &unit->autost_cnt2);
    if (id != -1) {
        sbtlSetAutoSkill_GBID(unit, id);
    }
    return 1;
}

/* NON_MATCHING: 32 of 48 words match.  */
#ifdef NON_MATCHING
s32 sbtlSetAutoSkill_GBID(sdatUnitWork_t *unit, s32 id) {
    if (id != -1) {
        switch (id) {
            case 10:
                unit->pbtl.maxhp = unit->pbtl.maxhp + unit->backup.maxhp * (unit->autost_cnt2 / 100.0f);
                break;
            case 11:
                unit->pbtl.maxmp = unit->pbtl.maxmp + unit->backup.maxmp * (unit->autost_cnt2 / 100.0f);
                break;
        }
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtl", sbtlSetAutoSkill_GBID);
#endif

s32 sbtlSetAutoSkill_ID(struct SBTLUNIT_WORK *wk, SKLID sklid) {
    if ((sbtlGetSkilltbl(sklid)->attr & 0xFFFFF000) == 0x10000) {
        if (!((wk->field414 & 1) && !(wk->field414 & 0x80) && sbtlCheckBit(&wk_sbtl->kuishibBit, wk_sbtl->summonIndex[0]))) {
            wk->flg |= 0x20000000;
        }
    }
    return 1;
}

s32 sbtlResetAutoSkill(sdatUnitWork_t *unit) {
    unit->koufunst = 0;
    unit->autost_cnt = 0;
    unit->autost_cnt2 = 0;
    unit->pbtl.maxhp = 0;
    unit->pbtl.maxmp = 0;
    return 1;
}

s32 sbtlGetAutoSkillPara(SKLID id, s16 *badst, s16 *power) {
    struct SBTLSKILLP_TBL *tbl;
    u32 attr;
    s32 i;

    if (id == SKLID_NONE) {
        return -1;
    }
    tbl = sbtlGetSkilltbl(id);
    attr = tbl->attr & 0xFFFFF000;
    *badst = tbl->badst & 0xFFF;
    *power = tbl->n1;
    if (tbl->attr & 0x4000) {
        *badst = 0;
    }
    if (attr & 0x24000000) {
        *badst = sbtlCheckAisyo_Old(*badst);
    }
    for (i = 0; i < 18; i++) {
        if (attr & (0x1000 << i)) {
            return i;
        }
    }
    return -1;
}

SKLID sbtlGetAutoSkill_ID(sdatUnitWork_t *unit, s32 id) {
    s32 n;
    s32 i;

    for (i = 0, n = 0; i < 10; i++) {
        if (!datCheckSkillEnable(unit, unit->haveskill[i]) && unit->devilId < 80) {
            continue;
        }
        if (unit->haveskill[i] == SKLID_NONE || unit->haveskill[i] == SKLID_OFS_PROGRAM ||
            unit->haveskill[i] == SKLID_ATK) {
            continue;
        }
        if (sbtlGetSkilltbl(unit->haveskill[i])->type & 0x2000) {
            if (n == id) {
                return unit->haveskill[i];
            }
            n++;
        }
    }
    return SKLID_NONE;
}


s32 sbtlLeaf123(sdatUnitWork_t *unit) {
    s32 i;

    for (i = 0; i < 18; i++) {
        if (unit->koufunst & (0x1000 << i)) {
            return i;
        }
    }
    return -1;
}

s32 sbtlCheckAutoSkill_ActerDamage_ID(SKLID sklid) {
    u32 attr = sbtlGetSkilltbl(sklid)->attr & 0xFFFFF000;
    s32 result = 0;

    switch (attr) {
        case 0x1000000:
        case 0x2000000:
        case 0x4000000:
        case 0x8000000:
            result = 1;
    }
    return result;
}

s32 sbtlCheckAutoSkill_TargetDamage_ID(SKLID sklid) {
    u32 attr = sbtlGetSkilltbl(sklid)->attr & 0xFFFFF000;
    s32 result = 0;

    if (attr == 0x8000 || attr == 0x20000000) {
        result = 1;
    }
    return result;
}

s32 sbtlCheckAutoSkill_ActerDamage(s32 autono) {
    s32 result = 0;

    if (autono < 16) {
        if (autono < 12) {
            result = 0;
        } else {
            result = 1;
        }
    }
    return result;
}

s32 sbtlCheckAutoSkill_TargetDamage(s32 type)
{
    s32 result = 0;

    if (type == 3 || type == 17) {
        result = 1;
    }
    return result;
}

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

/* NON_MATCHING: 156 of 196 words match.  */
#ifdef NON_MATCHING
s32 CalcAutoSkill(sdatUnitWork_t *unit, s32 mode, s32 value, SKLID skillid, s16 attr, s16 percent) {
    s32 ret = 0;
    struct SBTLSKILLP_TBL *tbl = sbtlGetSkilltbl(skillid);
    s32 type;
    s32 hp;

    switch (mode) {
    case 6:
        wk_sbtl->drop.gold = (s32)((percent / 100.0f) * wk_sbtl->drop.gold);
        break;
    case 8:
        wk_sbtl->drop.nakama_exp = (s32)((percent / 100.0f) * wk_sbtl->drop.nakama_exp);
        break;
    case 9:
        wk_sbtl->drop.chuusei = (s32)((percent / 100.0f) * wk_sbtl->drop.chuusei);
        break;
    case 1:
        unit->now.hp += (s32)((percent / 100.0f) * unit->now.maxhp);
        hp = unit->now.hp;
        unit->now.hp = (hp <= 0) ? 0 : (hp < unit->now.maxhp) ? hp : unit->now.maxhp;
        break;
    case 12:
        if ((s16)GetPercentage() < 50) {
            type = tbl->type & 0xFFF;
            if (type == 1 || type == 12 || type == 13) {
                ret = (s32)((percent / 100.0f) * value);
            }
        }
        break;
    case 13:
        if ((s16)GetPercentage() < 50) {
            type = tbl->type & 0xFFF;
            if ((type == 3 || type == 15) && (tbl->flg & 1) && !(tbl->flg & 0x200000)) {
                ret = (s32)((percent / 100.0f) * value);
            }
        }
        break;
    case 14:
        if ((tbl->attr & (1 << attr) & 0xFFF) && !(tbl->flg & 0x200000)) {
            ret = (s32)((percent / 100.0f) * value);
        }
        break;
    case 17:
        if (tbl->attr & (1 << attr) & 0xFFF) {
            ret = (s32)((percent / 100.0f) * value);
        }
        break;
    case 15:
        if ((tbl->flg & 0x10004) == 4 && !(tbl->s_type & 8)) {
            ret = (s32)((percent / 100.0f) * value);
        }
        break;
    case 3:
        ret = (s32)((percent / 100.0f) * value);
        break;
    case 2:
    case 4:
    case 5:
    case 7:
    case 10:
    case 11:
    case 16:
        break;
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "sbtl", CalcAutoSkill);
#endif

s32 sbtlGetDropMoney(sdatUnitWork_t *unit, s32 gold) {
    if ((unit->itemstatus & 0x100) != 0) {
        gold += gold / 5;
    }
    return gold;
}

s32 sbtlCheckMoon(u16 moon) {
    u16 tbl[9] = {0xBC0, 0xBC1, 0xBC2, 0xBC3, 0xBC4, 0xBC5, 0xBC6, 0xBC7, 0xBC8};

    return evtBitCheck(tbl[moon]) != 0;
}

s32 sbtlGetMagCost(sdatUnitWork_t *player, sdatUnitWork_t *unit, s32 devilid) {
    SBTL_DEVIL_DATA *dv = sbtlGetDevilData_sbtlSub(devilid);
    f32 cost = dv->field10;
    s32 ret;

    cost = (cost - player->now.tec * 1.5f) + 7.0f;

    if (unit->chuusei >= 50) {
        if (unit->chuusei < 71) {
            cost *= 0.7999999523162842f;
        } else if (unit->chuusei < 91) {
            cost *= 0.69999999f;
        } else if (unit->chuusei < 100) {
            cost *= 0.59999997f;
        } else {
            cost *= 0.5f;
        }
    }
    if (evtBitCheck(2961)) {
        cost *= 3.0f;
    }
    ret = cost;
    if (ret < 2) {
        ret = 1;
    }
    if (ret >= 999) {
        return 999;
    }
    return ret;
}

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
    sbtlGetMagCost(player, unit, devilid);
    return 0;
}

s32 sbtlCheckMagCost(sdatUnitWork_t *unit, s32 devilid, s8 flg) {
    struct SBTLUNIT_WORK *player;
    s32 cost;

    if (wk_sbtl->battleMode == 3) {
        return 1;
    }
    player = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    cost = 0;
    if (wk_sbtl->battleMode != 3) {
        cost = sbtlGetMagCost(&player->unit, unit, devilid);
    }
    if (wk_sbtl->mag < cost) {
        return 0;
    }
    if (!flg) {
        return 1;
    }
    sbtlRefSbtl64(cost);
    return 1;
}

