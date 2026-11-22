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
#include "sBtlCmndMenuNew.h"
#include "sBtlDraw.h"
#include "sbtlplayertbl.h"
#include "model.h"
#include "sBtlAI.h"
#include "dds3ObjectBase.h"
#include "code_r1_00104370.h"
#include "sBtlPlayer.h"
#include "dds3KernelMain.h"
#include "frFont.h"
#include "dds3KernelDraw.h"
#include "code_r1_0010DF68.h"
#include "dds3KernelCore.h"
#include "sdfMath.h"
#include "sBtlMenu.h"
#include "sBtlSM.h"

INCLUDE_ASM(const s32, "sbtl", sbtlLeaf125);

S_CameraMoveDt *sbtlBtlRunSklProcSub(s32 idx) {
    return &sbtlBtlRunSklProcSubData[idx];
}

void sbtlRefSbtl65(S_CameraMoveDt *dt) {
    SBTLCAMERA_WORK *camera = &wk_sbtl->camera;

    camera->m_PosCtrlCam.m_pPosMove = dt->m_pPosMoveCam;
    camera->m_PosCtrlTgt.m_pPosMove = dt->m_pPosMoveTgt;
    camera->m_PosCtrlCam.m_cnt = 0;
    camera->m_PosCtrlCam.m_stp = 0;
    camera->m_PosCtrlCam.m_run = 1;
    camera->m_PosCtrlTgt.m_cnt = 0;
    camera->m_PosCtrlTgt.m_stp = 0;
    camera->m_PosCtrlTgt.m_run = 1;
}

f32 GetHighest(SBTLUNIT_SIDE side)
{
    f32 height;
    f32 value;
    s32 i;
    struct SBTLUNIT_WORK *wk;

    height = 0.0f;
    for (i = 0; i < 48; i++) {
        wk = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (wk != NULL && (wk->field414 & side)) {
            value = sbtlGetMotionVisualData(wk->major, wk->minor)->field28;
            if (height < value) {
                height = value;
            }
        }
    }
    return height;
}

INCLUDE_ASM(const s32, "sbtl", sbtlLeaf123);

/* vu0 routine: distance from pos1 to pos0 less its part along the camera's level direction. */
f32 GetDisNoCamDep(sdfVector4_t *pos0, sdfVector4_t *pos1)
{
    sdfVector4_t dir;
    f32 dis;
    f32 len;
    f32 dot;

    SDF_VU0_LOADVECTOR(&wk_sbtl->camera.direction);
    SDF_VU0_SETVECTOR_Y(0.0f);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&dir);
    SDF_VU0_LOADVECTOR(pos0);
    SDF_VU0_LOADVECTOR_B(pos1);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(len);
    dis = len;
    SDF_VU0_LOADVECTOR_B(&dir);
    SDF_VU0_INNERPRODUCT(dot);
    if (dot > 0.0f) {
        dis = len - dot;
    }
    return dis;
}

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl62);

INCLUDE_ASM(const s32, "sbtl", sbtlCallFptodp3);

void sbtlSbtlCameraproc(void)
{
    sdfVector4_t target = { 0.0f };
    sdfVector4_t pos = { -507.7f, -435.3f, -973.36f, 0.0f };
    sdfVector4_t up = { 0.0f, 1.0f, 0.0f, 1.0f };
    sdfVector4_t eye = { 0.0f, 0.0f, 0.0f, 1.0f };
    sdfVector4_t at = { 0.0f, 0.0f, 10.0f, 1.0f };
    dds3ProcessID_t *pid;
    struct dds3Basic_t *cm;
    SBTLCAMERA_WORK *cam;

    wk_sbtl->camera.cm = NULL;
    wk_sbtl->camerabk.cm = NULL;
    pid = dds3StartProcess("sbtl_cameraproc", 0x40A, 1, 1, ProcCamera, ShutdownCamera, NULL);
    target.x += main_data.val20;
    target.z += main_data.val24;
    pos.x += main_data.val20;
    pos.z += main_data.val24;
    cm = dds3AddCameraObjectBasicTypeLookAt(dds3AllocObjectId(), &eye, &at, &up);
    dds3SetUnitObjectId(cm, "sbtl_scam");
    dds3CameraObjectBasicGetCameraTM(cm, &at);
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
    sbtlSetBossMode(0);
}

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

void sbtlSetBossMode(s32 mode) {
    sbtlGetIdlingData2 = mode;
    sbtlGetIdlingData3 = 0;
}

/* NON_MATCHING: 15 of 17 words match.  */
#ifdef NON_MATCHING
void sbtlSetStaticInit(void) {
    sbtlData171 = 1.0f;
    sbtlData172 = 100.0f;
    sbtlGetIdlingData2 = 0;
    sbtlGetIdlingData3 = 0;
    SetParaData = 0;
    sbtlData169 = 0;
    sbtlData170 = 0;
    sbtlGetIdlingData[0] = 0.0f;
    sbtlGetIdlingData[1] = 0.0f;
    sbtlGetIdlingData[2] = 0.0f;
}
#else
INCLUDE_ASM(const s32, "sbtl", sbtlSetStaticInit);
#endif

void sbtlLeaf110(f32 x, f32 y, f32 z) {
    sbtlData169 = 1;
    sbtlData171 = 1.0f;
    sbtlData170 = 0x45;
    sbtlLeaf110Work[0] = x;
    sbtlLeaf110Work[1] = y;
    sbtlLeaf110Work[2] = z;
}

INCLUDE_ASM(const s32, "sbtl", sbtlGetIdling);

/* vu0 routine: move the camera position and target towards vPos and vTgt by f. */
void Slerp_sbtlCamera(SBTLCAMERA_WORK *wk, sdfVector4_t *vPos, sdfVector4_t *vTgt, f32 f)
{
    sdfVector4_t tmp;

    SDF_VU0_LOADVECTOR(&wk->camerapos);
    SDF_VU0_SCALEVECTOR(1.0f - f);
    SDF_VU0_STOREVECTOR(&tmp);
    SDF_VU0_LOADVECTOR(vPos);
    SDF_VU0_SCALEVECTOR(f);
    SDF_VU0_LOADVECTOR_B(&tmp);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(&wk->camerapos);
    SDF_VU0_LOADVECTOR(&wk->targetpos);
    SDF_VU0_SCALEVECTOR(1.0f - f);
    SDF_VU0_STOREVECTOR(&tmp);
    SDF_VU0_LOADVECTOR(vTgt);
    SDF_VU0_SCALEVECTOR(f);
    SDF_VU0_LOADVECTOR_B(&tmp);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(&wk->targetpos);
}

INCLUDE_ASM(const s32, "sbtl", sbtlIsMoveEnd);

void sbtlSetMoveEnd(void) {
    wk_sbtl->camera.m_PosCtrlCam.m_run = 0;
}

void GetNowPosAS(sdfVector4_t *vPosRes, const sdfVector4_t *vPosBgn, const sdfVector4_t *vPosEnd, f32 fTime, f32 x) {
    f32 x2;
    f32 x3;
    f32 time2;
    f32 time3;
    f32 t;

    if (x == fTime) {
        SDF_VU0_LOADVECTOR(vPosEnd);
        SDF_VU0_STOREVECTOR(vPosRes);
        return;
    }
    x2 = x * x;
    time2 = fTime * fTime;
    x3 = x2 * x;
    time3 = time2 * fTime;
    SDF_VU0_LOADVECTOR(vPosEnd);
    SDF_VU0_LOADVECTOR_B(vPosBgn);
    SDF_VU0_SUBVECTOR();
    t = x3 * -2.0f / time3 + x2 * 3.0f / time2;
    SDF_VU0_SCALEVECTOR(t);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(vPosRes);
}

INCLUDE_ASM(const s32, "sbtl", GetNowPosS);

INCLUDE_ASM(const s32, "sbtl", PosCtrlSystem);

f32 CameraMoveSystem(SBTLCAMERA_WORK *wk) {
    f32 t = PosCtrlSystem(&wk->m_PosCtrlCam, &wk->camerapos);

    PosCtrlSystem(&wk->m_PosCtrlTgt, &wk->targetpos);
    return t;
}

/* NON_MATCHING: 80 of 86 words match.  */
#ifdef NON_MATCHING
/* vu0 routine: leaves the mean enemy position, at the tallest enemy's height, in vf10 for the caller. */
void SetEnemyCPos(void)
{
    sdfVector4_t pos;
    struct SBTLUNIT_WORK *wk;
    f32 height;
    f32 value;
    s32 boss;
    s32 i;
    s32 n;

    boss = FALSE;
    if (sbtlGetIdlingData2 < 3) {
        boss = sbtlGetIdlingData2 > 0;
    }
    pos.w = 1.0f;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    height = 70.0f;
    n = 1;
    for (i = 0; i < 48; i++) {
        wk = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (wk != NULL && (wk->field414 & 2) && !boss) {
            pos.x += wk->pos.x;
            pos.z += wk->pos.z;
            value = sbtlGetMotionVisualData(wk->major, wk->minor)->field28;
            if (height < value) {
                height = value;
            }
            n++;
        }
    }
    if (n > 0) {
        SDF_VU0_LOADVECTOR(&pos);
        SDF_VU0_SCALEVECTOR(1.0f / n);
        SDF_VU0_STOREVECTOR(&pos);
    } else {
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.z = 0.0f;
        pos.w = 1.0f;
    }
    pos.y = height;
    SDF_VU0_LOADVECTOR(&pos);
}
#else
INCLUDE_ASM(const s32, "sbtl", SetEnemyCPos);
#endif

void MenuStealth(s32 mode)
{
    struct SBTLUNIT_WORK *pchara;
    struct SBTLUNIT_WORK *nakama0;
    struct SBTLUNIT_WORK *nakama1;

    pchara = sbtlGetPChara();
    nakama0 = sbtlGetNakama(0);
    nakama1 = sbtlGetNakama(1);
    if (mode == 0) {
        dds3Data48 = 0;
        dds3ObjectBaseOffFlag(pchara->pBasic, 0x400000);
        if (nakama0 != NULL) {
            dds3ObjectBaseOffFlag(nakama0->pBasic, 0x400000);
        }
        if (nakama1 != NULL) {
            dds3ObjectBaseOffFlag(nakama1->pBasic, 0x400000);
        }
    } else {
        dds3Data48 = 1;
        dds3ObjectBaseOnFlag(pchara->pBasic, 0x400000);
        if (nakama0 != NULL) {
            dds3ObjectBaseOnFlag(nakama0->pBasic, 0x400000);
        }
        if (nakama1 != NULL) {
            dds3ObjectBaseOnFlag(nakama1->pBasic, 0x400000);
        }
    }
}

void sbtlMenuOpen(void) {
    if (wk_sbtl->flg >= 0) {
        sbtlSetMode(E_CAMERA_MODE_Menu2);
    } else {
        sbtlSetMode(E_CAMERA_MODE_Menu);
    }
}

void sbtlMenuClose(void) {
    if (!wk_sbtl->fieldCA2) {
        sbtlSetMode(E_CAMERA_MODE_Normal);
    }
    MenuStealth(0);
}

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

s32 ShutdownCamera(dds3ProcessID_t *id)
{
#ifdef FIX_BUGS
    return 0;
#endif
}

s32 sbtlLeaf118(void)
{
    return 1;
}

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlSearchUnitID35);

s32 sbtlCallSetPara(void) {
    SetPara(&wk_sbtl->camera);
    return 1;
}

INCLUDE_ASM(const s32, "sbtl", sbtlNoHokan);

s32 sbtlSetMode(E_CAMERA_MODE no)
{
    SBTLCAMERA_WORK *camera;
    s8 mode;

    mode = no;
    if (mode == E_CAMERA_MODE_Normal && !wk_sbtl->bBfrBtlEnd && !wk_sbtl->bosseventflg) {
        mode = 0;
        wk_sbtl->camCnt = 200;
    } else {
        wk_sbtl->camCnt = 0;
    }
    wk_sbtl->camCnt2 = 0;
    camera = &wk_sbtl->camera;
    if (camera != NULL) {
        camera->mode = mode;
        wk_sbtl->camTgtConsF = 0.0f;
        wk_sbtl->camTgtConsI = 0;
        camera->fieldAE = 1;
        if (mode == 13) {
            SDF_VU0_LOADVECTOR(&camera->camerapos);
            SDF_VU0_STOREVECTOR(&camera->m_PosCtrlCam.vLast);
            SDF_VU0_LOADVECTOR(&camera->targetpos);
            SDF_VU0_STOREVECTOR(&camera->m_PosCtrlTgt.vLast);
        }
        SDF_VU0_LOADVECTOR(&camera->camerapos);
        SDF_VU0_STOREVECTOR(&camera->camerapos_bk);
        SDF_VU0_LOADVECTOR(&camera->direction);
        SDF_VU0_STOREVECTOR(&camera->direction_bk);
        SDF_VU0_LOADVECTOR(&camera->targetpos);
        SDF_VU0_STOREVECTOR(&camera->targetpos_bk);
    }
    return 1;
}

E_CAMERA_MODE sbtlGetMode(void) {
    return wk_sbtl->camera.mode;
}

INCLUDE_ASM(const s32, "sbtl", sbtlCallMemset31);

INCLUDE_ASM(const s32, "sbtl", sbtlLeaf121);

INCLUDE_ASM(const s32, "sbtl", sbtlCallMemset30);

INCLUDE_ASM(const s32, "sbtl", sbtlCallMemset33);

INCLUDE_RODATA(const s32, "sbtl", SetParaTable);

INCLUDE_RODATA(const s32, "sbtl", SetParaTable2);

INCLUDE_RODATA(const s32, "sbtl", SetParaTable3);

INCLUDE_RODATA(const s32, "sbtl", jtbl_004C6860);

void sbtlCheckDamageTargetUseItem(s32 num) {
    SBTL_WORK *sbtl = wk_sbtl;

    switch (sbtl->stageNo) {
    case 600:
    case 601:
    case 602:
    case 604:
        return;
    case 605:
    case 606:
    case 607:
        if (sbtlGetIdlingData2 != 3) {
            return;
        }
    case 603:
        break;
    }
    sbtl->fieldC64 = 100;
    if (sbtl->camTgtConsI < 20) {
        sbtl->camTgtConsI = 20;
        return;
    }
    sbtl->camTgtConsI += num;
    if (sbtl->camTgtConsI >= 31) {
        sbtl->camTgtConsI = 30;
    }
}

E_CAMMOD_CUT sbtlGetCutFlg(void) {
    switch (wk_sbtl->camera.mode) {
    case E_CAMERA_MODE_TAME1:
    case E_CAMERA_MODE_TAME2:
        return E_CAMMOD_CUT_DmgNum | E_CAMMOD_CUT_Stp | E_CAMMOD_CUT_NakamaIcon | E_CAMMOD_CUT_ExcRenkeiTameEff;
    }
    return E_CAMMOD_CUT_Non;
}

void sbtlRefSbtl56(const sdfVector4_t *pos) {
    SDF_VU0_LOADVECTOR(pos);
    SDF_VU0_STOREVECTOR(&wk_sbtl->camera.camerapos);
}

void sbtlBgnPosSlp(const sdfVector4_t *pos) {
    SDF_VU0_LOADVECTOR(pos);
    SDF_VU0_STOREVECTOR(&wk_sbtl->field110);
}

INCLUDE_ASM(const s32, "sbtl", ClrAction);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl55);

void sbtlRefSbtl61(const sdfVector4_t *pos) {
    SDF_VU0_LOADVECTOR(pos);
    SDF_VU0_STOREVECTOR(&wk_sbtl->camera.targetpos);
}

void sbtlRefSbtl59(const sdfVector4_t *pos) {
    SDF_VU0_LOADVECTOR(pos);
    SDF_VU0_STOREVECTOR(&wk_sbtl->field120);
}

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

/* NON_MATCHING: 65 of 73 words match.  */
#ifdef NON_MATCHING
void sbtlSetWeakBit(s32 devilid)
{
    u32 pair = 0;

    DDS3_GBWK->anlyzBit[devilid / 32] |= 1 << devilid;
    switch (devilid) {
    case 236:
        pair = 617;
        break;
    case 256:
        pair = 616;
        break;
    case 302:
        pair = 596;
        break;
    case 327:
        pair = 618;
        break;
    case 596:
        pair = 327;
        break;
    case 616:
        pair = 256;
        break;
    case 617:
        pair = 236;
        break;
    case 618:
        pair = 302;
        break;
    }
    if (pair != 0) {
        DDS3_GBWK->anlyzBit[pair / 32] |= 1 << pair;
    }
}
#else
INCLUDE_ASM(const s32, "sbtl", sbtlSetWeakBit);
#endif

INCLUDE_ASM(const s32, "sbtl", sbtlCheckItemMember_Dead);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckItemMember);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlSetBadStatusRandSub);

s32 sbtlCallGetPercentage(s32 id)
{
    s32 tbl[5];
    s32 percentage;
    s32 num;
    s32 max;
    s32 i;

    memcpy(tbl, sbtlCallGetPercentageTable, sizeof(tbl));
    if (id == 16) {
        num = 5;
        max = 100;
        percentage = GetPercentage();
        for (i = 0; i < num; i++) {
            if (percentage < max / num * (i + 1)) {
                return tbl[i];
            }
        }
    }
    return id;
}

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

s32 CalcAisyo(struct SBTLUNIT_WORK *wk)
{
    sdatUnitWork_t *unit = &wk->unit;
    SKLID id;
    s32 i;

    for (i = 0; i < 6; i++) {
        id = sbtlGetAutoSkill_ID(unit, i);
        if (id != 0) {
            sbtlSetAutoSkill_ID(wk, id);
        }
    }
    return 1;
}

s32 sbtlSetAutoSkill_GB(sdatUnitWork_t *unit)
{
    s32 i;
    SKLID sklid;
    s32 id;

    sbtlResetAutoSkill(unit);
    for (i = 0; i < 6; i++) {
        sklid = sbtlGetAutoSkill_ID(unit, i);
        id = sbtlGetAutoSkillPara(sklid, &unit->autost_cnt, &unit->autost_cnt2);
        if (id >= 0) {
            sbtlSetAutoSkill_GBID(unit, id, sklid);
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "sbtl", sbtlSetAutoSkill_GBID);

/* NON_MATCHING: 20 of 68 words match.  */
#ifdef NON_MATCHING
s32 sbtlSetAutoSkill_ID(struct SBTLUNIT_WORK *wk, SKLID sklid)
{
    u32 attr;
    s32 idx;

    attr = sbtlGetSkilltbl(sklid)->attr & 0xFFFFF000;
    if (attr != 0x1000) {
        if (attr == 0x5000) {
            if ((wk->field414 & 1) && !(wk->field414 & 0x80)) {
                idx = wk_sbtl->summonIndex[0];
                if (wk->field1A0 != wk_sbtl->stockUnitID[idx]) {
                    idx = wk_sbtl->summonIndex[1];
                }
                if (sbtlCheckBit(&wk_sbtl->kuishibBit, idx)) {
                    return 1;
                }
            }
            wk->flg |= 0x20000000;
        }
    } else {
        if ((wk->field414 & 1) && !(wk->field414 & 0x80)) {
            idx = wk_sbtl->summonIndex[0];
            if (wk->field1A0 != wk_sbtl->stockUnitID[idx]) {
                idx = wk_sbtl->summonIndex[1];
            }
            if (sbtlCheckBit(&wk_sbtl->deadbombBit, idx)) {
                return 1;
            }
        }
        wk->flg2 |= 0x800;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtl", sbtlSetAutoSkill_ID);
#endif

s32 sbtlResetAutoSkill(sdatUnitWork_t *unit) {
    memset(&unit->pbtl, 0, sizeof(unit->pbtl));
    unit->autost_cnt = 0;
    unit->autost_cnt2 = 0;
    return 1;
}

s32 sbtlGetAutoSkillPara(SKLID id, s16 *badst, s16 *power)
{
    struct SBTLSKILLP_TBL *tbl;
    u32 attr;

    if (id == SKLID_NONE) {
        return -1;
    }
    tbl = sbtlGetSkilltbl(id);
    attr = tbl->attr & 0xFFFFF000;
    *badst = tbl->badst & 0xFFF;
    *power = tbl->n1;
    if (attr == 0x3000) {
        *badst = 0;
    }
    if (attr == 0xF000 || attr == 0x12000 || attr == 0x56000 || attr == 0x57000) {
        *badst = sbtlCheckAisyo_Old(*badst);
    }
    if (attr == 0x2B000 || attr == 0x5C000) {
        *badst = tbl->n0;
    }
    return attr >> 12;
}

SKLID sbtlGetAutoSkill_ID(sdatUnitWork_t *unit, s32 id)
{
    s32 i;
    s32 n;
    SKLID sklid;
    SKLID result;
    s32 cnt;

    result = SKLID_NONE;
    cnt = unit->iOmoideSkillCnt;
    if (unit->devilId >= 208) {
        for (i = 0, n = 0; i < 17; i++) {
            sklid = unit->haveskill[i];
            if (sklid == SKLID_NONE || sklid == SKLID_OFS_PROGRAM || sklid == SKLID_ATK) {
                continue;
            }
            if (sbtlGetSkilltbl(sklid)->type & 0x2000) {
                if (n == id) {
                    return unit->haveskill[i];
                }
                n++;
            }
        }
        return SKLID_NONE;
    }
    if (id >= 0 && id < cnt) {
        result = unit->aOmoideSkill[id];
    }
    return result;
}

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

s32 sbtlCheckAutoSkill_TargetDamage(s32 autono) {
    s32 result = 0;

    switch (autono) {
    case AUTO_EDAMAGE:
    case AUTO_Z_DMGDOWN:
    case AUTO_BS_DOWN:
    case AUTO_JS_DOWN:
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

s32 sbtlGetSkillCost(sdatUnitWork_t *unit, SKLID skillid)
{
    struct SBTLSKILLP_TBL *tbl;
    s32 cost;

    if (unit == NULL) {
        return 0;
    }
    tbl = sbtlGetSkilltbl(skillid);
    if (tbl == NULL) {
        return 0;
    }
    cost = tbl->para;
    if (sbtlChkHaveMemSkl(unit, AUTO_COSTRATE)) {
        cost = cost * unit->autost_cnt2 / 100;
    }
    if (cost == 0) {
        cost = cost < tbl->para;
    }
    return cost;
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
    return 1;
}

s32 sbtlCheckMagCost(sdatUnitWork_t *unit, s32 devilid, s8 flg) {
    return 1;
}

u8 sbtlCheckCost2(struct SBTLUNIT_WORK *wk, SKLID sklid, s32 flg)
{
    struct SBTLSKILLP_TBL *cmnd;
    struct SBTLSKILLP_TBL *tbl;

    if (wk->field414 & 2) {
        return 1;
    }
    if (wk->aiwk->cmndFrm != 0) {
        cmnd = sbtlGetSkilltbl(wk->aiwk->cmndSkl);
        if ((cmnd->flg2 & 1) && wk->skillid == cmnd->badst) {
            return 1;
        }
    }
    if (wk->flg & 0x100000) {
        return 1;
    }
    tbl = sbtlGetSkilltbl(wk->skillid);
    if (tbl->type & 0x1000) {
        return 1;
    }
    if (tbl->flg2 & 2) {
        sklid = tbl->badst;
    }
    return sbtlCheckCost_SkillID(wk, sklid, flg);
}

s32 sbtlCheckCost(struct SBTLUNIT_WORK *wk, s32 flg) {
    return sbtlCheckCost2(wk, wk->skillid, flg);
}

INCLUDE_ASM(const s32, "sbtl", sbtlCheckCost_SkillID);

INCLUDE_ASM(const s32, "sbtl", sbtlDecVice);

void sbtlDecMag(s32 mag) {
    SBTL_WORK *sbtl = wk_sbtl;
    s32 shift = 0;

    if (sbtl->status3 & 0x2000000) {
        shift = 1;
    }
    sbtl->mag -= mag << shift;
    sbtl->mag = (sbtl->mag <= 0) ? 0 : (sbtl->mag >= sbtl->magMax) ? sbtl->magMax : sbtl->mag;
}

s32 sbtlRefSbtl71(s32 num) {
    SBTL_WORK *sbtl = wk_sbtl;
    s32 max = sbtl->magMax;
    s32 shift = 0;
    s32 dec;
    s32 short_of_mag;

    if (sbtl->status3 & 0x2000000) {
        shift = 1;
    }
    dec = max * (num << shift) / 100;
    if (dec <= 0) {
        dec = 1;
    }
    sbtl->mag -= dec;
    short_of_mag = 0;
    if (sbtl->mag < 0) {
        short_of_mag = 1;
    }
    sbtl->mag = (sbtl->mag <= 0) ? 0 : (sbtl->mag >= max) ? max : sbtl->mag;
    return short_of_mag;
}

void sbtlRefSbtl54(s32 num) {
    SBTL_WORK *sbtl = wk_sbtl;

    sbtl->mag += num;
    sbtl->mag = (sbtl->mag <= 0) ? 0 : (sbtl->mag >= sbtl->magMax) ? sbtl->magMax : sbtl->mag;
}

INCLUDE_ASM(const s32, "sbtl", sbtlDecHP);

INCLUDE_ASM(const s32, "sbtl", sbtlDecMP);

INCLUDE_ASM(const s32, "sbtl", sbtlDecItem_GBWK);

INCLUDE_ASM(const s32, "sbtl", sbtlDecItem);

void sbtlSetGunNum(s32 num) {
    sbtlGetPlayerData();
    wk_sbtl->hgunNum = num;
}

void sbtlRefSbtl66(s32 num) {
    wk_sbtl->hgunNum -= num;
    if (wk_sbtl->hgunNum < 0) {
        wk_sbtl->hgunNum = 0;
    }
}

s32 sbtlGetGun_Num(void)
{
    return wk_sbtl->hgunNum;
}

SBTL_AISYO_TBL *sbtlGetAisyotbl(s32 id)
{
    return &aisyo_tbldata[id];
}

INCLUDE_ASM(const s32, "sbtl", sbtlCheckAttrFlg);

s32 sbtlCheckAisyo_Old(u32 attr) {
    s32 i;

    for (i = 0; i < 16; i++) {
        if ((attr >> i) & 1) {
            return i;
        }
    }
    return -1;
}

u32 sbtlBtlCheckAisyoExSub(SBTL_AISYO_TBL *aisyo, u32 attr)
{
    u32 result;
    s32 i;
    s32 para;

    result = 0;
    for (i = 0; i < 16; i++) {
        if ((attr >> i) & 1) {
            para = aisyo->word[i];
            if (para & 0x10000) {
                return 0x10000;
            }
            if (para & 0x2000) {
                return 0x2000;
            }
            if (para & 0x1000) {
                return 0x1000;
            }
            if (para & 0x80000) {
                return 0x80000;
            }
            if (para & 0x4000) {
                result |= 0x4000;
            }
            if (para & 0x8000) {
                result |= 0x8000;
            }
            if (para & 0x20000) {
                result |= 0x20000;
            }
            if (para & 0x40000) {
                result |= 0x40000;
            }
        }
    }
    return result;
}

u32 sbtlCheckAisyoEx(struct SBTLUNIT_WORK *wk, u32 attr, s32 *hanshaType)
{
    u32 aisyo;

    aisyo = sbtlBtlCheckAisyoExSub(&wk->unit.aisyo, attr);
    if (hanshaType != NULL) {
        *hanshaType = 0;
    }
    if (wk->karnCnt != 0 && (attr & 0x401) != 0 && !(wk->flg2 & 0x10)) {
        if (hanshaType != NULL) {
            *hanshaType = 2;
        }
        return 0x1000;
    }
    if (wk->barrier.size != 0) {
        if (attr & 0x400) {
            attr |= 1;
        }
        if (wk->barrier.fieldC == 0x305 && hanshaType != NULL) {
            *hanshaType = 1;
        }
        if (wk->barrier.fieldC == 0x306) {
            aisyo = 0x1000;
            if (hanshaType != NULL) {
                *hanshaType = 2;
            }
        }
        if (wk->barrier.field0 & attr) {
            if (hanshaType != NULL) {
                *hanshaType = 3;
            }
            switch (wk->barrier.field4) {
            case 0x2000:
                aisyo = 0x2000;
                break;
            case 0x1000:
                if (aisyo != 0x2000) {
                    aisyo = 0x1000;
                }
                break;
            case 0x80000:
                if (aisyo != 0x2000 && aisyo != 0x1000) {
                    aisyo = 0x80000;
                }
                break;
            }
        }
    }
    return aisyo;
}

INCLUDE_ASM(const s32, "sbtl", WeakBstDvlTk);

s32 LimitRange(sdatUnitWork_t *unit, s32 value, s32 isMp) {
    s32 max;
    s32 cur;

    if (!isMp) {
        cur = unit->now.hp;
        max = unit->now.maxhp;
    } else {
        cur = unit->now.mp;
        max = unit->now.maxmp;
    }
    cur += value;
    if (cur > max) {
        return value - (cur - max);
    }
    return value;
}

/* NON_MATCHING: 67 of 72 words match.  */
#ifdef NON_MATCHING
s32 sbtlAutoRec(struct SBTLUNIT_WORK *wk, s32 para)
{
    s32 value;

    if (wk->unit.now.hp < wk->unit.now.maxhp) {
        value = para;
        if (wk_sbtl->battleMode == 2 && !(wk->field414 & 2)) {
            value = para / 2;
        }
        value = LimitRange(&wk->unit, value <= 0 ? 1 : value, 0);
        if ((wk->field414 & 1) && (wk_sbtl->status3 & 0x8000000)) {
            value /= 2;
        }
        wk->unit.now.hp += value;
        wk->unit.now.hp = (wk->unit.now.hp <= 0) ? 0 : (wk->unit.now.hp >= wk->unit.now.maxhp) ? wk->unit.now.maxhp : wk->unit.now.hp;
        sbtlCreateAnim_Unit(wk, NULL, value, 0, 3);
        sbtlCreateEff(wk_sbtl->effList, wk, EFFID_AUTOREC, 0, &wk->pos, 0x80808080, 0);
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtl", sbtlAutoRec);
#endif

/* NON_MATCHING: 22 of 175 words match.  */
#ifdef NON_MATCHING
s32 sbtlCheckKuishibari(struct SBTLUNIT_WORK *wk, s32 para)
{
    SKLID sklid;
    SKLID id;
    s32 per;
    s32 i;
    s32 idx;
    BST bst;

    sklid = SKLID_NONE;
    per = 0;
    if (wk->field1A0 == wk_sbtl->targetUnitID) {
        if ((wk_sbtl->scrFlag & 0x20) && wk->unit.now.hp - para <= 0) {
            para = wk->unit.now.hp - 1;
            wk_sbtl->scrFlag |= 0x10;
        }
    }
    if (!(wk->flg & 0x20000000)) {
        return para;
    }
    for (i = 0; i < 6; i++) {
        id = sbtlGetAutoSkill_ID(&wk->unit, i);
        if (sbtlGetAutoSkillPara(id, &wk->unit.autost_cnt, &wk->unit.autost_cnt2) == 5) {
            if (per < wk->unit.autost_cnt2) {
                per = wk->unit.autost_cnt2;
                sklid = id;
            } else if (sklid == SKLID_NONE) {
                sklid = id;
            }
        }
    }
    if (sklid == SKLID_NONE) {
        if (wk->unit.now.hp - para <= 0) {
            para = wk->unit.now.hp - 1;
        }
    } else {
    if (wk->unit.now.hp - para > 0) {
        return para;
    }
    bst = sbtlGetBadStatus(&wk->unit);
    if (bst != BST_STONE && bst != BST_BOUDO && bst != BST_MAHUU && bst != BST_MOUDOKU && bst != BST_SLEEP
        && bst != BST_BOYCOTT && bst != BST_RAND0) {
        return para;
    }
    if (wk_sbtl->sysmsgList->bLock) {
        wk_sbtl->sysmsgList->bLock = 0;
    }
    if ((wk->field414 & 0x80) || (wk->field414 & 2)) {
        sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_BOSS, sklid);
    } else {
        sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_BOSS, sklid);
        idx = wk_sbtl->summonIndex[0];
        if (wk->field1A0 != wk_sbtl->stockUnitID[idx]) {
            idx = wk_sbtl->summonIndex[1];
        }
        sbtlSetBit(&wk_sbtl->kuishibBit, idx);
    }
    wk->flg &= ~0x20000000;
    wk->flg2 |= 0x40;
    para = wk->unit.now.hp - 1;
    wk->field4A2 = 180;
    if (per != 0) {
        wk->unit.now.hp = wk->unit.now.maxhp * per / 100;
        sbtlCreateAnim_Unit(wk, NULL, wk->unit.now.hp - 1, 12, 3);
        sbtlCreateAnim_Unit(wk, NULL, para, 0, 0);
        para = 0;
    }
    }
    return para;
}
#else
INCLUDE_ASM(const s32, "sbtl", sbtlCheckKuishibari);
#endif

s32 sbtlCheckSpecialDamage_Stop(struct SBTLUNIT_WORK *wk) {
    if (wk_sbtl->flg & 0x2000000) {
        return 1;
    }
    if (IsSceanStop()) {
        return 1;
    }
    switch (wk->unit.devilId) {
    case 631:
    case 672:
        if (wk->status == 7) {
            return 1;
        }
    }
    return 0;
}

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

/* NON_MATCHING: 48 of 51 words match.  */
#ifdef NON_MATCHING
s32 sbtlResetBadStatus(struct SBTLUNIT_WORK *wk, BSTF flg)
{
    if (!(wk->flg & 0x800) && flg != 0) {
        wk->unit.badst_cnt = 0;
        wk->unit.badStatus &= ~flg;
        if (wk->hModel != NULL) {
            mdlSetClutColor(wk->hModel, 0x80808080, 0.0f);
        }
        sbtlBtlFadeOutBadStatusEffectSub(wk);
        sbtlBtlFadeOutBadStatusEffectSub2(wk);
        if (wk->unit.devilId != 455 && wk->unit.devilId != 456) {
            CalcAisyo_sbtlCalc(wk);
        }
        if (wk->bst_effserialID2 != -1) {
            sbtlDeleteEffectData_SerialID2(wk_sbtl->effList, wk->bst_effserialID2);
            wk->bst_effserialID2 = -1;
        }
        if (wk->hModel != NULL) {
            wk->hModel->flags &= ~2;
        }
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "sbtl", sbtlResetBadStatus);
#endif

INCLUDE_RODATA(const s32, "sbtl", CalcBadStatusTable3);

INCLUDE_RODATA(const s32, "sbtl", CalcBadStatusTable4);

s32 sbtlGetBadStatus_Attr(BST id)
{
    s32 table[21] = { 2, 3, 4, 5, 0, 6, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 1, 7, 7, 7 };

    return table[id];
}

/* NON_MATCHING: 7 of 20 words match.  */
#ifdef NON_MATCHING
s32 sbtlCheckBadStatus(struct SBTLUNIT_WORK *wk) {
    s32 bst = sbtlGetBadStatus(&wk->unit);

    if (bst != BST_RAND0 && bst != BST_BOYCOTT) {
        return sbtlResetBadStatus(wk, 1 << bst);
    }
    return bst;
}
#else
INCLUDE_ASM(const s32, "sbtl", sbtlCheckBadStatus);
#endif

INCLUDE_ASM(const s32, "sbtl", sbtlCallCheckBadStatus);

INCLUDE_ASM(const s32, "sbtl", sbtlResetUnit);

/* NON_MATCHING: 78 of 92 words match.  */
#ifdef NON_MATCHING
u32 sbtlCheckBadStatus_Number(u32 badstatus, BST bstno)
{
    u32 tbl[18];
    u32 ret;
    s32 i;
    s32 j;

    ret = 1;
    memcpy(tbl, sbtlLeaf107Table, sizeof(tbl));
    for (i = 0; i < 17; i++) {
        if (badstatus & tbl[i]) {
            break;
        }
    }
    if (i < 16) {
        for (j = 0; j < 17; j++) {
            if (tbl[j] == 1 << bstno) {
                break;
            }
        }
        if (j >= 16) {
            ret = 1;
        } else {
            ret = i < j;
        }
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "sbtl", sbtlCheckBadStatus_Number);
#endif

INCLUDE_RODATA(const s32, "sbtl", sbtlLeaf107Table);

INCLUDE_ASM(const s32, "sbtl", sbtlCallSetClutColor);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckBadStatus_Weak);

void sbtlBtlSetBadStatusNoSub(struct SBTLUNIT_WORK *wk, BST badstno)
{
    if (wk->major == 0) {
        return;
    }
    if (badstno != 1 && badstno != 8) {
        return;
    }
    switch (wk->unit.devilId) {
    case 1:
    case 2:
    case 5:
    case 6:
    case 7:
    case 9:
    case 25:
    case 51:
    case 52:
    case 71:
    case 116:
    case 208:
    case 209:
    case 212:
    case 213:
    case 214:
    case 216:
    case 232:
    case 258:
    case 259:
    case 278:
    case 323:
    case 544:
    case 556:
    case 597:
    case 598:
    case 602:
    case 612:
    case 613:
    case 653:
    case 654:
    case 655:
    case 656:
    case 676:
    case 677:
    case 678:
    case 679:
        wk->hModel->flags |= 2;
        break;
    }
}

INCLUDE_ASM(const s32, "sbtl", sbtlSetBadStatus_No);

struct TEXCOLOR_WORK *sbtlRefGlobalPID(BST badstno)
{
    return &bstcolor_tbldata[badstno];
}

s32 sbtlCheckBadStatus_Color(struct SBTLUNIT_WORK *wk, BST badstno)
{
    return sbtlRefGlobalPID(badstno)->flg != 0;
}

INCLUDE_ASM(const s32, "sbtl", sbtlCallSetClutColor2);

void sbtlSetBadStatusAction(struct SBTLUNIT_WORK *wk, u32 action, s32 cnt) {
    sbtlSetAction(wk, action, cnt);
    sbtlClearHitC(wk_sbtl->hitList, wk);
    sbtlForceReset(wk_sbtl->effList, wk);
    wk->model->flags &= ~0x20;
    wk->field474 = 0;
    wk->flg |= 0x20;
    wk->vstatus |= 0x80;
    sbtlForceDelete_SkillFlag_Acter(wk, 0x400);
}

/* NON_MATCHING: 130 of 134 words match.  */
#ifdef NON_MATCHING
void SetBadStatusCounter(struct SBTLUNIT_WORK *wk, struct SBTLSKILLP_TBL *ptbl, BST badstno)
{
    f32 per;
    s32 attrtime;
    s32 time;
    SKLID sklid;
    s16 badst;
    s16 power;

    per = 1.0f;
    attrtime = ptbl->attrtime;
    switch (badstno) {
    case BST_KANASHIB:
    case BST_HYOUKETU:
    case BST_KANDEN:
    case BST_APPAKU:
    case BST_KOUFUN:
        if (wk->field414 & 2) {
            attrtime = attrtime * 3 >> 1;
            sklid = sbtlChkHaveMemSklParty(30);
            if (sklid != SKLID_NONE) {
                sbtlGetAutoSkillPara(sklid, &badst, &power);
                per = power / 100.0f;
            }
        }
        time = attrtime;
        attrtime = time * per;
        datDebugPrintf("J_KOUCYOKU[BST] %d * [H]%d/100 -> %d\n", time, power, attrtime);
        break;
    case BST_MOUDOKU:
        attrtime = (wk->field414 & 0x80) ? 900 : 1800;
        break;
    case BST_KAEN:
        attrtime = (wk->field414 & 2) ? 180 : 120;
        break;
    case BST_DEAD:
        attrtime = 180;
        sbtlPlaySE(0x10025);
        break;
    }
    wk->unit.badst_cnt = attrtime;
    wk->unit.badStatus |= 1 << badstno;
    wk->unit.badst_cnt2 = 0;
    if (wk->flg & 8) {
        wk->noWeakStfCnt = attrtime;
    } else {
        wk->noWeakStfCnt = 0;
        sbtlSetBadStatusAction(wk, 9, attrtime);
    }
    if (wk->field414 & 0x80) {
        if (badstno == BST_KAEN) {
            sbtlSetMotion_sbtlUnit(wk, 2, 0, 0, 0);
        } else if (badstno == BST_MAHUU) {
            sbtlSetMotion_sbtlUnit(wk, 2, 0, 0, 0);
        } else {
            sbtlSetMotion_sbtlUnit(wk, 1, 0, 0, 0);
        }
    } else {
        switch (wk->unit.devilId) {
        case 455:
        case 456:
            break;
        default:
            sbtlSetMotion_sbtlUnit(wk, 1, 0, 0, 0);
            break;
        }
    }
}
#else
INCLUDE_ASM(const s32, "sbtl", SetBadStatusCounter);
#endif

s32 sbtlBtlCheckBadStatusWeakSub(SKLID skill) {
    s32 result = 0;

    switch (skill) {
    case 427:
    case 432:
    case 443:
    case 450:
    case 903:
    case 1131:
        result = 1;
    }
    return result;
}

INCLUDE_ASM(const s32, "sbtl", sbtlCheckBadStatus_WeakNumber);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckBadStatus_Hit);

INCLUDE_ASM(const s32, "sbtl", sbtlGetDamage_GB);

void sbtlKudaDmg(s8 kuda, s32 *pDmg)
{
    s32 tbl[8];
    s32 i;
    s32 j;
    struct SBTLUNIT_WORK *pUnit;
    SKLID sklid;
    s32 per;
    s32 dmg;

    memcpy(tbl, sbtlKudaDmgTable, sizeof(tbl));
    if (kuda >= 8 || kuda == 0) {
        return;
    }
    kuda = tbl[kuda];
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        pUnit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (pUnit == NULL) {
            continue;
        }
        pUnit->unit.autost_cnt = 0;
        pUnit->unit.autost_cnt2 = 0;
        for (j = 0; j < 6; j++) {
            sklid = pUnit->unit.aOmoideSkill[j];
            if (sbtlGetAutoSkillPara(sklid, &pUnit->unit.autost_cnt, &pUnit->unit.autost_cnt2) == 19) {
                per = autotbldata[sbtlGetSkilltbl(sklid)->weapontype].aisyo.word[kuda];
                if (per != 0) {
                    dmg = *pDmg;
                    *pDmg = dmg * per / 100;
                    datDebugPrintf(sbtlStrZKeisaNorKeisanShimuA100, dmg, per, *pDmg);
                }
            }
        }
    }
}

INCLUDE_RODATA(const s32, "sbtl", sbtlKudaDmgTable);

INCLUDE_RODATA(const s32, "sbtl", sbtlStrZKeisaNorKeisanShimuA100);

void sbtlFullMoonDmg(s32 *pDmg, struct SBTLUNIT_WORK *pAtkr)
{
    s32 i;
    s32 dmg;

    if (pAtkr == NULL) {
        return;
    }
    if (sbtlCheckMoon(8)) {
        for (i = 0; i < 6; i++) {
            switch (sbtlGetAutoSkillPara(pAtkr->unit.aOmoideSkill[i], &pAtkr->unit.autost_cnt, &pAtkr->unit.autost_cnt2)) {
            case 40:
            case 42:
            case 75:
                dmg = *pDmg;
                *pDmg = dmg * pAtkr->unit.autost_cnt2 / 100;
                datDebugPrintf("MANGETUUPorMANGETUorM_RBM_DMGUP %d * [H]%d/100 -> %d\n", dmg, pAtkr->unit.autost_cnt2, *pDmg);
                break;
            }
        }
    } else if (sbtlCheckMoon(0)) {
        for (i = 0; i < 6; i++) {
            if (sbtlGetAutoSkillPara(pAtkr->unit.aOmoideSkill[i], &pAtkr->unit.autost_cnt, &pAtkr->unit.autost_cnt2) == 75) {
                dmg = *pDmg;
                *pDmg = dmg * pAtkr->unit.autost_cnt2 / 100;
                datDebugPrintf("M_RBM_DMGUP %d * [H]%d/100 -> %d\n", dmg, pAtkr->unit.autost_cnt2, *pDmg);
            }
        }
    }
}

/* NON_MATCHING: 132 of 139 words match.  */
#ifdef NON_MATCHING
void sbtlZokuseiDmg(s32 *pDmg, struct SBTLUNIT_WORK *pTgt, s32 attr)
{
    s32 i;
    SKLID sklid;
    s32 per;
    s32 dmg;
    SBTLAUTO_TBL *autotbl;

    if (pTgt == NULL) {
        return;
    }
    for (i = 0; i < 6; i++) {
        sklid = pTgt->unit.aOmoideSkill[i];
        switch (sbtlGetAutoSkillPara(sklid, &pTgt->unit.autost_cnt, &pTgt->unit.autost_cnt2)) {
        case 41:
        case 42:
            if (sbtlCheckMoon(0)) {
                autotbl = &autotbldata[sbtlGetSkilltbl(sklid)->weapontype];
                per = autotbl->aisyo.word[sbtlCheckAisyo_Old(attr)];
                per &= 0xFFF;
                if (per != 0) {
                    dmg = *pDmg;
                    *pDmg = dmg * per / 100;
                    datDebugPrintf("SHINGETDOWNorMANGETU %d * [A]%d/100 -> %d\n", dmg, per, *pDmg);
                }
            }
            break;
        case 51:
        case 52:
            per = autotbldata[sbtlGetSkilltbl(sklid)->weapontype].aisyo.word[attr];
            per &= 0xFFF;
            if (per != 0) {
                dmg = *pDmg;
                *pDmg = dmg * per / 100;
                datDebugPrintf(sbtlStrZKeisaNorKeisanShimuA100, dmg, per, *pDmg);
            }
            break;
        case 74:
            if (pTgt->flg & 0x100) {
                per = autotbldata[sbtlGetSkilltbl(sklid)->weapontype].aisyo.word[attr];
                per &= 0xFFF;
                if (per != 0) {
                    dmg = *pDmg;
                    *pDmg = dmg * per / 100;
                    datDebugPrintf("G_Z_KEISAN %d * [A]%d/100 -> %d\n", dmg, per, *pDmg);
                }
            }
            break;
        }
    }
}
#else
INCLUDE_ASM(const s32, "sbtl", sbtlZokuseiDmg);
#endif

INCLUDE_ASM(const s32, "sbtl", sbtlGetDamage_sbtlCalc);

void sbtlResetBattlePara(struct SBTLUNIT_WORK *wk, s32 index)
{
    sbtlCheckStockPara(&wk->unit);
    wk->barrier.size = 0;
    CalcAisyo_sbtlCalc(wk);
    wk->unit.now.str = wk->unit.now.str <= 0 ? 0 : wk->unit.now.str > 40 ? 40 : wk->unit.now.str;
    wk->unit.now.vit = wk->unit.now.vit <= 0 ? 0 : wk->unit.now.vit > 40 ? 40 : wk->unit.now.vit;
    wk->unit.now.tec = wk->unit.now.tec <= 0 ? 0 : wk->unit.now.tec > 40 ? 40 : wk->unit.now.tec;
    wk->unit.now.luck = wk->unit.now.luck <= 0 ? 0 : wk->unit.now.luck > 40 ? 40 : wk->unit.now.luck;
    if (index != -1) {
        sbtlSetData_Unit_Stock(wk, index);
    }
}

/* NON_MATCHING: 98 of 116 words match.  */
#ifdef NON_MATCHING
void sbtlGetPlayerTec_ParaUp(sdatUnitWork_t *unit, sdatParaWork_t *para)
{
    s32 tectbl[13];
    s32 paratbl[13][4];
    s32 i;
    s32 idx;
    s32 tec;

    memcpy(tectbl, sbtlBtlGetDamageGBSubTable, sizeof(tectbl));
    memcpy(paratbl, sbtlGetPlayerTec_ParaUpTable, sizeof(paratbl));
    tec = unit->now.tec;
    for (i = 0; i < 13; i++) {
        if (tectbl[i] >= tec) {
            break;
        }
    }
    idx = i < 13 ? i : 0;
    para->str = paratbl[idx][0];
    para->vit = paratbl[idx][1];
    para->tec = paratbl[idx][2];
    para->luck = paratbl[idx][3];
}
#else
INCLUDE_ASM(const s32, "sbtl", sbtlGetPlayerTec_ParaUp);
#endif

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

/* NON_MATCHING: 106 of 132 words match.  */
#ifdef NON_MATCHING
void CalcAisyo_sbtlCalc(struct SBTLUNIT_WORK *wk)
{
    s32 i;
    s32 j;
    s32 per;
    struct SBTLSKILLP_TBL *skl;

    wk->unit.aisyo = aisyo_tbldata[gohData7[wk->unit.devilId].aisyoId];
    for (i = 0; i < 6; i++) {
        skl = sbtlGetSkilltbl(sbtlGetAutoSkill_ID(&wk->unit, i));
        if (skl == NULL) {
            continue;
        }
        j = 0;
        if (skl->attr & 0x33032) {
            for (; j < 16; j++) {
                per = autotbldata[skl->weapontype].aisyo.word[j];
                if (per != 0) {
                    if (skl->attr & 0x32) {
                        wk->unit.aisyo.word[j] = wk->unit.aisyo.word[j] * (per & 0xFFF) / 100;
                    }
                    wk->unit.aisyo.word[j] |= per & 0xFFFFF000;
                }
            }
        }
    }
    for (i = 0; i < 16; i++) {
        if (wk->unit.aisyo.word[i] & 0x800000) {
            wk->unit.aisyo.word[i] &= ~0x4000 & ~0x800000;
        }
    }
}
#else
INCLUDE_ASM(const s32, "sbtl", CalcAisyo_sbtlCalc);
#endif

INCLUDE_ASM(const s32, "sbtl", sbtlBtlGetDamageSBtlCalcSub);

/* NON_MATCHING: 26 of 69 words match.  */
#ifdef NON_MATCHING
s32 sbtlGetDamage_ATK(SBTLCALC_DMGWORK *wk, struct SBTLSKILLP_TBL *ptbl)
{
    f32 base;
    f32 damage;
    s32 idx;

    base = (wk->sword_power * 0.5f * (wk->aatk + wk->alevel) * 0.033333f + wk->sword_power * 0.333333f) * wk->n0 * 0.01f;
    if (base * (100.0f - (wk->tlevel + wk->tdef) * 0.5f) * 0.01f == 0.0f) {
        damage = 1.0f;
    } else {
        damage = base * (100.0f - (wk->tlevel + wk->tdef) * 0.5f) * 0.01f;
    }
    idx = sbtlCheckAisyo_Old(ptbl->attr);
    if (idx != -1) {
        damage *= (wk->aisyo_tbl->word[idx] & 0xFFF) / 100.0f;
    }
    return damage;
}
#else
INCLUDE_ASM(const s32, "sbtl", sbtlGetDamage_ATK);
#endif

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlCheckAisyoOld3);

s32 sbtlGetDamage_GUN(SBTLCALC_DMGWORK *wk, struct SBTLSKILLP_TBL *ptbl)
{
    f32 damage_per;
    f32 damage;
    s32 idx;

    damage_per = wk->damage_per;
    damage = (wk->alevel + wk->gun_power) * wk->n0;
    damage /= wk->tlevel / 10.0f + 16.5f + wk->tdef / 10.0f;
    damage /= damage_per;
    idx = sbtlCheckAisyo_Old(ptbl->attr);
    if (idx != -1) {
        damage *= (wk->aisyo_tbl->word[idx] & 0xFFF) / 100.0f;
    }
    return damage;
}

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlCheckAisyoOld5);

s32 sbtlGetDamage_MAGIC(SBTLCALC_DMGWORK *wk, struct SBTLSKILLP_TBL *ptbl)
{
    f32 damage_per;
    f32 damage;
    s32 idx;

    damage_per = wk->damage_per;
    damage = wk->n0 * (wk->alevel + wk->atec * 3 + 20.0f);
    damage /= wk->ttec + 15;
    damage *= damage_per;
    idx = sbtlCheckAisyo_Old(ptbl->attr);
    if (idx != -1) {
        damage *= (wk->aisyo_tbl->word[idx] & 0xFFF) / 100.0f;
    }
    return damage;
}

INCLUDE_ASM(const s32, "sbtl", sbtlBtlGetDamageSBtlCalcSub2);

INCLUDE_ASM(const s32, "sbtl", sbtlGetDamage_The_W);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlCheckAisyoOld7);

s32 sbtlGetDamage_MRecover(SBTLCALC_DMGWORK *wk, struct SBTLSKILLP_TBL *tbl)
{
    return (wk->tmaxhp * (wk->n0 / 100.0f) + (wk->alevel * 0.5f + wk->atec * 1.5f + wk->tluck * 1.5f) + 20.0f) *
           wk->damage_per;
}

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlCheckAisyoOld2);

s32 sbtlGetDamage_Tadan(struct SBTLUNIT_WORK *wk, struct SBTLSKILLP_TBL *ptbl, SKLID skillid, s32 damage)
{
    f32 fdamage;

    if ((wk->flg & 0x100) && !(ptbl->flg2 & 0x20000)) {
        return damage;
    }
    fdamage = damage;
    if (wk->tdan_sklid == 0) {
        wk->tdan_sklid = skillid;
        wk->tdancounter = ptbl->para2;
        wk->tdancnt = 0;
        wk->tdancmpcnt = ptbl->para2 / ptbl->count;
        wk->tdanpara = fdamage * (ptbl->n1 / 100.0f);
        if (wk->tdanpara < 2) {
            wk->tdanpara = 1;
        }
    }
    return damage;
}

INCLUDE_ASM(const s32, "sbtl", sbtlBtlGetDamageSBtlCalcSub4);

s32 sbtlCheckRegistKaja(SBTLSKL_KAJA_WORK *kaja, struct SBTLSKILLP_TBL *skill) {
    s32 i;
    s32 result;
    f32 n1;

    for (i = 0; i < 2; i++) {
        if (kaja[i].m_cnt == 0) {
            return i;
        }
    }
    n1 = skill->n1;
    result = -1;
    for (i = 0; i < 2; i++) {
        if (kaja[i].m_cnt != 0 && kaja[i].m_fPara < n1) {
            n1 = kaja[i].m_fPara;
            result = i;
        }
    }
    return result;
}

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

s32 sbtlGetViceLevel(s32 vice) {
    if (vice >= 300) {
        return 3;
    }
    if (vice >= 238) {
        return 2;
    }
    if (vice < 166) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "sbtl", sbtlRecalcAisyoMember);

s32 sbtlBtlLoadHuumaMesDataSub(s32 huumaMesNo) {
    switch (huumaMesNo) {
    case 208:
        return 15;
    case 209:
        return 16;
    }
    return gohData7[huumaMesNo].field80;
}

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

struct SBTLDVLTK_WORK *sbtlCreateDvltk_Priority(struct SBTLDVLTK_LISTWORK *lwk, struct SBTLUNIT_WORK *wk, SBTLDVLTK_MODE mode)
{
    struct SBTLUNIT_WORK *target;
    s32 type;
    u8 side;

    if (wk_sbtl->status4 & 0x100) {
        return NULL;
    }
    target = wk;
    type = mode;
    if (target == NULL) {
        return NULL;
    }
    side = target->field414;
    if (!(side & 1)) {
        return NULL;
    }
    switch (type) {
    case 9:
    case 10:
    case 11:
    case 12:
        if ((side & 0x80) && wk_sbtl->member_num > 0) {
            target = sbtlGetNakama(0);
            if (target == NULL || !CheckLiveTarget(target)) {
                target = wk;
            }
        }
        break;
    case 3:
    case 4:
        if ((u32)(sbtlGetBadStatus(&target->unit) - 9) < 2) {
            type = 13;
        }
        break;
    }
    if ((target->status == 10 || target->status == 40) && mode != 15) {
        return NULL;
    }
    return sbtlCreateDvltk(lwk, target, type);
}

/* NON_MATCHING: 95 of 127 words match.  */
#ifdef NON_MATCHING
struct SBTLDVLTK_WORK *sbtlCreateDvltk(struct SBTLDVLTK_LISTWORK *lwk, struct SBTLUNIT_WORK *wk, SBTLDVLTK_MODE mode)
{
    SBTLDVLTK_WORK *dwk;
    sdfMemHandle_t *handle;
    u8 side;
    s32 i;

    if (wk == NULL) {
        return NULL;
    }
    side = wk->field414;
    if (!(side & 1)) {
        return NULL;
    }
    if (side & 0x80) {
        return NULL;
    }
    if (lwk->dwk == NULL) {
        handle = sdfMemAllocDirect(sizeof(SBTLDVLTK_WORK));
        dwk = sdfMemLock(handle);
        memset(dwk, 0, sizeof(SBTLDVLTK_WORK));
        dwk->handle = handle;
        dwk->prev = NULL;
        dwk->next = NULL;
        lwk->num = 1;
        lwk->dwk = dwk;
    } else {
        dwk = lwk->dwk;
        if (sbtlData104 == NULL) {
            if (dwk->mode == mode) {
                if (dwk->status == 1) {
                    dwk->counter = 0;
                }
                return dwk;
            }
            if (!sbtlCheckPriority_Battle(dwk->mode, mode)) {
                return dwk;
            }
            handle = dwk->handle;
            memset(dwk, 0, sizeof(SBTLDVLTK_WORK));
            dwk->handle = handle;
            dwk->prev = NULL;
            dwk->next = NULL;
        } else if (dwk->counter == 0 && (s32)mode < (s32)dwk->mode) {
            return dwk;
        }
    }
    dwk->listID = wk->field1A0;
    dwk->mode = mode;
    dwk->devilid = wk->unit.devilId;
    dwk->cmpcounter = 120;
    dwk->seikaku = wk->unit.seikaku;
    dwk->fieldDC = 1;
    dwk->sincount = 0;
    dwk->status = 0;
    dwk->wk[3].st = 1;
    dwk->wk[4].st = 1;
    dwk->fieldD8 = 0;
    dwk->ofs_x = 514;
    dwk->ofs_y = 322;
    for (i = 0; i < 7; i++) {
        dwk->wk[i].x = sbtlCallMemAllocDirect10Data[i - 7].x;
        dwk->wk[i].y = sbtlCallMemAllocDirect10Data[i - 7].y;
        dwk->wk[i].rgb = sbtlCallMemAllocDirect10Data[i - 7].rgb;
        dwk->wk[i].alpha = sbtlCallMemAllocDirect10Data[i - 7].alpha;
    }
    sbtlPlaySE(0x10023);
    return dwk;
}
#else
INCLUDE_ASM(const s32, "sbtl", sbtlCreateDvltk);
#endif

s32 Proc_sbtlDvltk(struct SBTLDVLTK_LISTWORK *lwk)
{
    SBTLDVLTK_WORK *dwk;

    if (lwk == NULL) {
        return 0;
    }
    dwk = lwk->dwk;
    if (dwk == NULL || lwk->num == 0) {
        return 0;
    }
    if (wk_sbtl->bosseventflg) {
        return 0;
    }
    if (sbtlData104 == NULL && !wk_sbtl->field42CA) {
        if (wk_sbtl->flg & 0x8000000) {
            return 0;
        }
        if (wk_sbtl->flg2 & 0x12) {
            return 0;
        }
    }
    while (dwk != NULL) {
        switch (dwk->status) {
        case SBTLDVLTK_ST_INIT:
            dwk->sincount = (dwk->sincount + 12) % 360;
            dwk->alpha += 5;
            dwk->alpha = (dwk->alpha <= 0) ? 0 : (dwk->alpha > 128) ? 128 : dwk->alpha;
            if (dwk->sincount >= 0 && dwk->sincount < 180) {
                dwk->ofs_x = (sdfSin((f32)((dwk->sincount + 90) % 360) / 180.0f * 3.14159f) + 1.0f) * 0.5f * 128.0f + 386.0f;
            } else {
                dwk->ofs_x = 386;
                dwk->status = SBTLDVLTK_ST_PROC;
                if (dwk->mode == 15) {
                    StopBattle();
                    wk_sbtl->eventno = 6;
                    wk_sbtl->eventcounter = 0;
                    wk_sbtl->flg |= 0x400;
                    wk_sbtl->eventgstat = 0;
                    sbtlSetNSTOP();
                }
            }
            break;
        case SBTLDVLTK_ST_PROC:
            dwk->alpha += 5;
            dwk->alpha = (dwk->alpha <= 0) ? 0 : (dwk->alpha > 128) ? 128 : dwk->alpha;
            dwk->counter++;
            dwk->counter = (dwk->counter <= 0) ? 0 : (dwk->counter < dwk->cmpcounter) ? dwk->counter : dwk->cmpcounter;
            if (dwk->counter >= dwk->cmpcounter) {
                dwk->status = SBTLDVLTK_ST_END;
            }
            break;
        }
        dwk = dwk->next;
    }
    return 1;
}

INCLUDE_ASM(const s32, "sbtl", sbtlSearchDvltk);

s32 Draw_sbtlDvltk(struct SBTLDVLTK_LISTWORK *lwk)
{
    SBTLDVLTK_WORK *dwk;
    s32 last = FALSE;

    if (sbtlGetSpriteHandle() == NULL) {
        return 0;
    }
    if (lwk == NULL) {
        return 0;
    }
    dwk = lwk->dwk;
    if (dwk == NULL || lwk->num == 0) {
        return 0;
    }
    if (wk_sbtl->bosseventflg) {
        return 0;
    }
    if (sbtlData104 == NULL && !dds3BgSetScrollSub() && !wk_sbtl->field42CA) {
        if (wk_sbtl->flg2 & 0x12) {
            return 0;
        }
        if (wk_sbtl->flg2 & 0x80) {
            return 0;
        }
    }
    while (dwk != NULL) {
        if (dwk->mode == 0) {
            last = TRUE;
        }
        if (last == TRUE || dwk->next == NULL) {
            if (dwk->status < SBTLDVLTK_ST_END) {
                sbtlDrawText(dwk);
            }
        }
        dwk = dwk->next;
        if (last == TRUE) {
            return 1;
        }
    }
    return 1;
}

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

s32 sbtlDeleteDvltkList(SBTLDVLTK_LISTWORK *lwk) {
    sbtlReleaseDvltk_List(lwk, 1);
    sdfChipFreeDirect(lwk);
    return 1;
}

s32 sbtlDrawTextFont(s32 x, s32 y, s32 z, u32 rgba, struct SBTLUNIT_WORK *wk, SBTLDVLTK_MODE mode)
{
    u8 *msg;
    frFontHandle_t *frq;

    sbtlData106 = 0;
    if (wk->msgData != NULL) {
        msg = MessageNumSearch(mode, wk->msgData, 0);
        if (msg != NULL) {
            eglSetOt(105);
            eglLinkTest(1, 5, 1, 1, 0, 0, 1, 1);
            eglLinkAlpha(0);
            frq = frCallMessageRequestEx22(0, 0, z, 0, 0, 0, 0, msg, NULL, -1, 22);
            sbtlData106 = frCallGetStrWidth3(0, frq) >> 4;
            frFontMoveChainTo(x * 16, y * 8, frq);
            frSetFontColor(frq, rgba);
            frDrawFontOt(frq, 1, 105);
            frDestroyFontRequestAfter(frq);
            return 1;
        }
    }
    return 0;
}

s32 sbtlCheckPriority_Battle(s32 a, s32 b)
{
    s32 tbl[32];

    memcpy(tbl, sbtlLeaf99Table, sizeof(tbl));
    if (a == 22 && b == 18) {
        return 1;
    }
    if (tbl[a] > tbl[b]) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "sbtl", sbtlDrawText);

/* NON_MATCHING: 27 of 31 words match.  */
#ifdef NON_MATCHING
void sbtlAnimWaku(SBTLDVLTK_WORK *dwk)
{
    s32 i;

    for (i = 0; i < 7; i++) {
        switch (dwk->wk[i].st) {
        case 0:
            dwk->wk[i].alpha--;
            if (dwk->wk[i].alpha < 2) {
                dwk->wk[i].alpha = 1;
                dwk->wk[i].st = 1;
            }
            break;
        case 1:
            dwk->wk[i].alpha++;
            if (dwk->wk[i].alpha >= 128) {
                dwk->wk[i].alpha = 128;
                dwk->wk[i].st = 0;
            }
            break;
        }
    }
}
#else
INCLUDE_ASM(const s32, "sbtl", sbtlAnimWaku);
#endif

s32 sbtlBtlDrawTextSub2(void) {
    return 1;
}

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
