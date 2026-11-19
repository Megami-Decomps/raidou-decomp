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

INCLUDE_ASM(const s32, "sbtl", sbtlSetWeakBit);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckItemMember_Dead);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlSearchUnitID35);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlGetItemtbl3);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlGetItemtbl2);

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

INCLUDE_ASM(const s32, "sbtl", sbtlCheckHaveskill_Magic);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckHaveskill_Rec);

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

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlGetSkilltbl24);

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
    sbtlGetMagCost(player, unit, devilid);
    return 0;
}

INCLUDE_ASM(const s32, "sbtl", sbtlCheckMagCost);

