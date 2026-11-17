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
    wk_sbtl->field070 = 0;
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

INCLUDE_ASM(const s32, "sbtl", sbtlCallSetGameCamera0);

INCLUDE_ASM(const s32, "sbtl", sbtlCallGetActiveScene4);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl65);

INCLUDE_ASM(const s32, "sbtl", sbtlCallSetPosPoseSetDds3Basic);

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

INCLUDE_ASM(const s32, "sbtl", sbtlCallSetPosPoseSetDds3Basic2);

s32 sbtlLeaf103(dds3ProcessID_t *id) {
    return 0;
}

s32 sbtlLeaf87(dds3ProcessID_t *id)
{
}

INCLUDE_ASM(const s32, "sbtl", sbtlCallBitCheck5);

INCLUDE_ASM(const s32, "sbtl", sbtlRefDDS3GBWK21);

INCLUDE_ASM(const s32, "sbtl", CalcItem_Battle);

INCLUDE_ASM(const s32, "sbtl", sbtlRefDDS3GBWK20);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckItemMember_Dead);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlSearchUnitID35);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlGetItemtbl3);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlGetItemtbl2);

INCLUDE_ASM(const s32, "sbtl", sbtlResetItemFlg);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckDrink);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckItemCount);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckHaveskill_Atk);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckHaveskill_Magic);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckHaveskill_Rec);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckAutoSkill);

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
        sbtlCallBtlGetSkilltbl19(wk, id);
    }
    id = sbtlGetAutoSkill_ID(unit, 1);
    if (id != 0) {
        sbtlCallBtlGetSkilltbl19(wk, id);
    }
    return 1;
}

INCLUDE_ASM(const s32, "sbtl", sbtlSetAutoSkill_GB);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlSetAutoSkillGBSub);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlGetSkilltbl19);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlSetAutoSkillGBSub2);

s32 sbtlCallBtlGetSkilltbl21(SKLID id, s16 *badst, s16 *power) {
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


INCLUDE_ASM(const s32, "sbtl", sbtlLeaf123);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlGetSkilltbl23);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlGetSkilltbl18);

INCLUDE_ASM(const s32, "sbtl", sbtlLeaf110);

s32 sbtlGetDamageSub(s32 type)
{
    s32 result = 0;

    if (type == 3 || type == 17) {
        result = 1;
    }
    return result;
}

INCLUDE_ASM(const s32, "sbtl", sbtlCheckAutoSkill_Action);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlGetSkilltbl24);

INCLUDE_ASM(const s32, "sbtl", sbtlGetDropMoney);

INCLUDE_ASM(const s32, "sbtl", LimitRange_2);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlGetDevilDataSBtlSub5);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckUseSkill);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckCost);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlSearchUnitID34);

INCLUDE_RODATA(const s32, "sbtl", LimitRange_2Table);

