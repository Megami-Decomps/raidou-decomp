#include "common.h"
#include "raidou_alloc.h"

/* Declarations from smt3-decomp src/smt3/basic/dds3CameraObjectBasic.c */
#include "common.h"

#include "basic.h"

#include "dds3obj.h"

#include "ps2asm.h"

extern void *sdfChipAllocDirect(u32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

extern s32 dds3InitPoseSet(dds3Basic_t *basic);

extern void dds3FreePoseSet(dds3Basic_t *basic);

extern void dds3ObjectBaseOnFlag(dds3Basic_t *basic, s32 flag);

extern s32 dds3CheckPoseSetFlag(dds3PoseSet_t *pPoseSet, u32 flag);

extern void dds3OffPoseSetFlag(dds3PoseSet_t *pPoseSet, u32 flag);

extern void dds3SetRadiusPoseSet(dds3PoseSet_t *pose);

extern dds3Basic_t *dds3GetControlScene(void);

extern dds3Basic_t *dds3GetActiveCameraSceneBasic(dds3Basic_t *pBasic);

extern void dds3SetScreenFovy(f32 fovy);

extern void ToMatrix(void);

extern void LookAtMatrix(const sdfVector4_t *aTarget, const sdfVector4_t *aEye,
                          const sdfVector4_t *aDown);

extern sdfVector4_t aViewEye;

extern sdfVector4_t aViewTarget;

extern sdfVector4_t aViewDown;

extern const sdfVector4Unaligned_t dds3CameraObjectBasicInitialUp;

extern const sdfVector4Unaligned_t dds3CameraObjectBasicInitialEye;

extern dds3Basic_t *dds3AddSceneBasicData(s32 type);

extern void dds3DeleteSceneBasicData(dds3Basic_t *basic);

extern void dds3ObjectBaseCreateMover(dds3Basic_t *pObjectBasic);

void MakeCamera(dds3Basic_t *pBasic);

s32 func_00112370(dds3Basic_t *basic);

void func_001123D0(dds3Basic_t *basic);

/* Publish only the selected scene camera; consume its pending FOV update. */
s32 func_00112410(dds3Basic_t *basic);

s32 func_001124F8(void);

/* Rotate the local vectors and cache their world-space look-at matrix. */
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

/* vu0 routine: return the camera matrix in vf28-vf31. */
void func_00112888(dds3Basic_t *basic);

/* The recovered name is misleading: retail stores a target, not a matrix. */
void dds3CameraObjectBasicGetCameraTM(dds3Basic_t *basic, const sdfVector4_t *target);

/* vu0 routine: return the cached world-space target in vf10. */
void dds3CameraObjectBasicGetTargetPos(dds3Basic_t *pBasic);

void dds3CameraObjectBasicSetFovy(dds3Basic_t *pBasic, f32 fovy);

void func_001128F0(dds3Basic_t *basic, f32 fovy);

f32 dds3CameraObjectBasicGetFovy(dds3Basic_t *pBasic);

/* vu0 routine: refresh the rotated vectors and copy target/eye to the caller. */
void dds3CameraObjectBasicGetTgtCam(dds3Basic_t *pBasic, sdfVector4_t *tgtpos, sdfVector4_t *campos);

u32 dds3CameraObjectBasicGetMode(dds3Basic_t *pBasic);

void func_00112A18(dds3Basic_t *basic, u32 mode);

extern const sdfVector4Unaligned_t dds3CameraObjectBasicInitialUp;

extern const sdfVector4Unaligned_t dds3CameraObjectBasicInitialEye;
/* End of declarations from src/smt3/basic/dds3CameraObjectBasic.c */

/* Declarations from dds-decomp src/dds1/basic/dds3CameraObjectBasic.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "basic.h"

#include "dds/eff_transform.h"

enum {
    OBJECT_TRANSFORM_FLAG_UPDATE_PENDING = 1,
    OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID = 2,
    OBJECT_TRANSFORM_FLAG_USE_SMOOTHED_POSITION = 8
};

#include "dds/sdf_projection.h"

#include "dds/common.h"

#include "dds/dds3_camera_update_flags.h"

#include "dds/sdf_vu0.h"

#include "dds/sdf_chip.h"

#include "dds/dds3obj.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/pcp_vu0.h"

typedef struct {
    f32 components[4];
} CameraVector;

extern CameraVector D_0039F6F8;

extern CameraVector D_0039F708;

#include "dds/dds3_world_secondary.h"

/* Release the camera's inner node, base handle, and owned data block. */
void dds3DestroyCameraData(dds3Basic_t *camera);

/* Rebuild dirty vectors and publish only the active world's camera; return 1.
 * A pending field-of-view update is consumed only while this camera is active. */
s32 dds3UpdateCameraObject(dds3Basic_t *camera);

/* The camera draw callback has no geometry to submit. */
s32 dds3DrawCameraObject(dds3Basic_t *camera);

/* Load the owned look-at matrix into vf28-vf31. */
void dds3LoadObjectMatrixPointerIntoVu(dds3Basic_t *camera);

/* Copy the supplied vector to the camera's world-space eye position. */
void dds3CameraObjectBasicSetWorldEye(dds3Basic_t *camera, const u128 *worldEye);

/* Load the camera's world-space eye vector into vf10. */
void dds3CameraObjectBasicLoadWorldEyeIntoVu(dds3Basic_t *camera);
/* End of declarations from src/dds1/basic/dds3CameraObjectBasic.c */

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_r1_00119E98);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_r1_00119EF8);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_r1_00119F38);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_r1_0011A020);

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
    VU0_LOAD_MATRIX(((CameraData *)camera->workdata)->matrix);
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

