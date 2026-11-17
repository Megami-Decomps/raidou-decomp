#include "common.h"
#include "raidou_alloc.h"
#include "basic.h"
#include "dds3obj.h"
#include "ps2asm.h"
#include "sdfVu0.h"
#include "sdfQuat.h"
#include "dds3SceneBasic.h"
#include "dds3Basic.h"
#include "dds3CameraObjectBasic.h"
#include "dds3ObjectBase.h"
#include "dds3KernelDraw.h"
#include "sdfChip.h"
#include "dds3WorldBasic.h"

extern const sdfVector4Unaligned_t dds3CameraObjectBasicInitialUp;
extern const sdfVector4Unaligned_t dds3CameraObjectBasicInitialEye;
void MakeCamera(dds3Basic_t *pBasic);
dds3ObjectBaseControlWorkData_t *dds3GetCameraObjectBaseControlWorkData(dds3Basic_t *basic);
dds3Basic_t *dds3AllocCameraObjectBasic(u32 serialID);

dds3Basic_t *dds3AddCameraObjectBasicTypeFree(u32 serialID, const sdfVector4_t *pos,
                                           const sdfVector4_t *rot);

dds3Basic_t *dds3AddCameraObjectBasicTypeLookAt(u32 serialID, const sdfVector4_t *position,
                                             const sdfVector4_t *target, const sdfVector4_t *up);

dds3Basic_t *dds3AddCameraObjectBasic(u32 serialID, f32 fovy, const sdfVector4_t *position,
                                     const sdfVector4_t *eye, const sdfVector4_t *up, s32 mode);

void dds3DeleteCameraObjectBasic(dds3Basic_t *pBasic);
void dds3CameraObjectBasicGetCameraTM(dds3Basic_t *basic, const sdfVector4_t *target);
void dds3CameraObjectBasicGetTargetPos(dds3Basic_t *pBasic);
void dds3CameraObjectBasicSetFovy(dds3Basic_t *pBasic, f32 fovy);
f32 dds3CameraObjectBasicGetFovy(dds3Basic_t *pBasic);
void dds3CameraObjectBasicGetTgtCam(dds3Basic_t *pBasic, sdfVector4_t *tgtpos, sdfVector4_t *campos);

enum {
    OBJECT_TRANSFORM_FLAG_UPDATE_PENDING = 1,
    OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID = 2,
    OBJECT_TRANSFORM_FLAG_USE_SMOOTHED_POSITION = 8
};

#include "include_asm.h"
#include "dds3UnitObjectBasic.h"

/* Camera eye-position selectors are stored and passed as signed words. */
typedef s32 CameraEyePositionMode;

/* Kind-4 world-node data: cached look-at matrix and eye/up vectors (0x90). */
typedef struct camCameraData_t {
    u32 matrix[4][4]; /* Raw VU matrix words; the SDK also exposes word [0][3]. */
    u128 localEyeOffset;
    u128 localUp;
    u128 worldEye;
    u128 worldUp;
    dds3ObjectBaseControlWorkData_t *handle;
    CameraEyePositionMode eyeIsRelative;
    u32 fovUpdatePending; /* Bit 0 requests a field-of-view update. */
    f32 fieldOfView;      /* Radians. */
} camCameraData_t;

typedef char camCameraData_size_must_be_0x90[(sizeof(camCameraData_t) == 0x90) ? 1 : -1];

typedef struct {
    f32 components[4];
} CameraVector;

void dds3LoadObjectMatrixPointerIntoVu(dds3Basic_t *camera);

/* body: matched in smt3 @ 9401f79eb: src/smt3/basic/dds3CameraObjectBasic.c:34, match, same function 1.00 (words 23/23, calls 1.00, shape 1.00) */
s32 Constructor(dds3Basic_t *basic)
{
    dds3CameraObjectBasicWorkData_t *work;
    dds3ObjectBaseControlWorkData_t *control;

    dds3InitPoseSet(basic);
    work = sdfChipAllocDirect(sizeof(dds3CameraObjectBasicWorkData_t));
    basic->workdata = work;
    control = dds3ObjectBaseInitControlWorkData(basic);
    work->pObjectBaseControlWorkData = control;
    dds3ObjectBaseOnFlag(basic, 0x62);
    work->flag = 0;
    return 1;
}

void Destructor(dds3Basic_t *pBasic)
{
    dds3CameraObjectBasicWorkData_t *work;

    dds3FreePoseSet(pBasic);
    work = pBasic->workdata;
    dds3ObjectBaseFreeControlWorkData(work->pObjectBaseControlWorkData);
    sdfChipFreeDirect(work);
}

s32 Mainprocuder(dds3Basic_t *basic)
{
    dds3PoseSet_t *pose;
    dds3CameraObjectBasicWorkData_t *work;

    pose = basic->poseset;
    work = basic->workdata;
    if (dds3CheckPoseSetFlag(pose, POSESET_FLAG_UPDATEPOSE) == 1) {
        dds3OffPoseSetFlag(pose, POSESET_FLAG_UPDATEPOSE);
        MakeCamera(basic);
        dds3SetRadiusPoseSet(pose);
    }
    if (dds3GetActiveCameraSceneBasic(dds3GetControlScene()) == basic) {
        SDF_VU0_COPYVECTOR(&aViewEye, &pose->pos);
        SDF_VU0_COPYVECTOR(&aViewTarget, &work->targetPos);
        SDF_VU0_COPYVECTOR(&aViewDown, &work->upVector);
        if (work->flag & 1) {
            dds3SetScreenFovy(work->fovy);
            work->flag &= ~1;
        }
    }
    return 1;
}

u32 dds3Leaf27(void) {
    return 1;
}

/* Rotate the local vectors and cache their world-space look-at matrix. */
void MakeCamera(dds3Basic_t *pBasic)
{
    dds3CameraObjectBasicWorkData_t *work = pBasic->workdata;
    dds3PoseSet_t *pose = pBasic->poseset;

    SDF_VU0_LOADVECTOR(&pose->rot);
    ToMatrix();
    SDF_VU0_LOADVECTOR(&work->vertical);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_STOREVECTOR(&work->upVector);
    if (work->mode == 1) {
        SDF_VU0_LOADVECTOR(&work->eyeVector);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_LOADVECTOR_B(&pose->pos);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR(&work->targetPos);
    }
    LookAtMatrix(&work->targetPos, &pose->pos, &work->upVector);
    SDF_VU0_STOREMATRIX(&work->cameraTM);
}

dds3ObjectBaseControlWorkData_t *dds3GetCameraObjectBaseControlWorkData(dds3Basic_t *basic)
{
    dds3CameraObjectBasicWorkData_t *work = basic->workdata;

    return work->pObjectBaseControlWorkData;
}

dds3Basic_t *dds3AllocCameraObjectBasic(u32 serialID)
{
    dds3Basic_t *basic = dds3AddSceneBasicData(BASIC_TYPE_CAMERAOBJECT);

    basic->serialID = serialID;
    dds3ObjectBaseCreateMover(basic);
    return basic;
}

dds3Basic_t *dds3AddCameraObjectBasicTypeFree(u32 serialID, const sdfVector4_t *pos,
                                           const sdfVector4_t *rot)
{
    sdfVector4_t initialUp;
    sdfVector4_t initialEye;
    dds3Basic_t *basic;
    dds3CameraObjectBasicWorkData_t *work;

    initialUp = dds3CameraObjectBasicInitialUp;
    initialEye = dds3CameraObjectBasicInitialEye;
    basic = dds3AllocCameraObjectBasic(serialID);
    work = basic->workdata;
    work->mode = 1;
    work->fovy = 0.6283185f;
    dds3SetRotPoseSet(basic, rot);
    dds3SetPosPoseSet(basic, pos);
    dds3SetRadiusPoseSet(basic->poseset);
    SDF_VU0_COPYVECTOR(&work->vertical, &initialUp);
    SDF_VU0_COPYVECTOR(&work->eyeVector, &initialEye);
    MakeCamera(basic);
    return basic;
}

dds3Basic_t *dds3AddCameraObjectBasicTypeLookAt(u32 serialID, const sdfVector4_t *position,
                                             const sdfVector4_t *target, const sdfVector4_t *up)
{
    dds3Basic_t *basic = dds3AllocCameraObjectBasic(serialID);
    dds3CameraObjectBasicWorkData_t *work = basic->workdata;

    work->fovy = 0.6283185f;
    work->mode = 0;
    dds3SetPosPoseSet(basic, position);
    dds3SetRadiusPoseSet(basic->poseset);
    SDF_VU0_COPYVECTOR(&work->vertical, up);
    SDF_VU0_COPYVECTOR(&work->targetPos, target);
    MakeCamera(basic);
    return basic;
}

dds3Basic_t *dds3AddCameraObjectBasic(u32 serialID, f32 fovy, const sdfVector4_t *position,
                                     const sdfVector4_t *eye, const sdfVector4_t *up, s32 mode)
{
    dds3Basic_t *basic = dds3AllocCameraObjectBasic(serialID);
    dds3CameraObjectBasicWorkData_t *work = basic->workdata;

    work->fovy = fovy;
    work->mode = mode;
    dds3SetPosPoseSet(basic, position);
    dds3SetRadiusPoseSet(basic->poseset);
    SDF_VU0_COPYVECTOR(&work->vertical, up);
    SDF_VU0_COPYVECTOR(&work->eyeVector, eye);
    if (mode == 0) {
        SDF_VU0_COPYVECTOR(&work->targetPos, eye);
    }
    MakeCamera(basic);
    return basic;
}

void dds3DeleteCameraObjectBasic(dds3Basic_t *pBasic)
{
    dds3DeleteSceneBasicData(pBasic);
}

/* Load the owned look-at matrix into vf28-vf31. */
void dds3LoadObjectMatrixPointerIntoVu(dds3Basic_t *camera) {
    VU0_LOAD_MATRIX(((camCameraData_t *)camera->workdata)->matrix);
}

/* The recovered name is misleading: retail stores a target, not a matrix. */
void dds3CameraObjectBasicGetCameraTM(dds3Basic_t *basic, const sdfVector4_t *target)
{
    SDF_VU0_COPYVECTOR(&((dds3CameraObjectBasicWorkData_t *)basic->workdata)->targetPos, target);
}

/* vu0 routine: return the cached world-space target in vf10. */
void dds3CameraObjectBasicGetTargetPos(dds3Basic_t *pBasic)
{
    sdfVector4_t *target = &((dds3CameraObjectBasicWorkData_t *)pBasic->workdata)->targetPos;

    SDF_VU0_LOADVECTOR_BARRIER(target);
}

void dds3CameraObjectBasicSetFovy(dds3Basic_t *pBasic, f32 fovy)
{
    dds3CameraObjectBasicWorkData_t *work = pBasic->workdata;

    work->fovy = fovy;
    work->flag |= 1;
}

f32 dds3CameraObjectBasicGetFovy(dds3Basic_t *pBasic)
{
    dds3CameraObjectBasicWorkData_t *work = pBasic->workdata;

    return work->fovy;
}

/* vu0 routine: refresh the rotated vectors and copy target/eye to the caller. */
void dds3CameraObjectBasicGetTgtCam(dds3Basic_t *pBasic, sdfVector4_t *tgtpos, sdfVector4_t *campos)
{
    dds3CameraObjectBasicWorkData_t *work = pBasic->workdata;
    dds3PoseSet_t *pose = pBasic->poseset;

    SDF_VU0_LOADVECTOR(&pose->rot);
    ToMatrix();
    SDF_VU0_LOADVECTOR(&work->vertical);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_STOREVECTOR(&work->upVector);
    if (work->mode == 1) {
        SDF_VU0_LOADVECTOR(&work->eyeVector);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_LOADVECTOR_B(&pose->pos);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR(&work->targetPos);
    }
    tgtpos->x = work->targetPos.x;
    tgtpos->y = work->targetPos.y;
    tgtpos->z = work->targetPos.z;
    tgtpos->w = work->targetPos.w;
    campos->x = pose->pos.x;
    campos->y = pose->pos.y;
    campos->z = pose->pos.z;
    campos->w = pose->pos.w;
}

const sdfVector4Unaligned_t dds3CameraObjectBasicInitialUp = {0.0f, 1.0f, 0.0f, 1.0f};

const sdfVector4Unaligned_t dds3CameraObjectBasicInitialEye = {0.0f, 0.0f, 1.0f, 1.0f};
