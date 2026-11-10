/* Declarations of smt3-decomp src/smt3/basic/dds3CameraObjectBasic.c, for its bodies in this directory. */
#ifndef SHARED_BASIC_DDS3CAMERAOBJECTBASIC_H
#define SHARED_BASIC_DDS3CAMERAOBJECTBASIC_H

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

extern sdfVector4Unaligned_t dds3CameraObjectBasicInitialUp;

extern sdfVector4Unaligned_t dds3CameraObjectBasicInitialEye;

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

#endif
