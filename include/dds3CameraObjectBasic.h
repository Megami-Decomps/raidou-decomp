#ifndef DDS3CAMERAOBJECTBASIC_H
#define DDS3CAMERAOBJECTBASIC_H

#include "common.h"
#include "basic.h"
#include "sdf_types.h"

dds3Basic_t *dds3AddCameraObjectBasicTypeFree(u32 serialID, const sdfVector4_t *pos, const sdfVector4_t *rot);
dds3ObjectBaseControlWorkData_t *dds3GetCameraObjectBaseControlWorkData(dds3Basic_t *basic);

s32 Constructor(dds3Basic_t *pBasic);
void Destructor(dds3Basic_t *pBasic);
s32 Mainprocuder(dds3Basic_t *pBasic);
dds3Basic_t *dds3AddCameraObjectBasic(u32 serialID, f32 fovy, const sdfVector4_t *position, const sdfVector4_t *eye, const sdfVector4_t *up, s32 mode);
dds3Basic_t *dds3AddCameraObjectBasicTypeLookAt(u32 serialID, const sdfVector4_t *eyePos, const sdfVector4_t *targetPos, const sdfVector4_t *up);
void dds3CameraObjectBasicSetFovy(dds3Basic_t *pBasic, f32 fovy);
f32 dds3CameraObjectBasicGetFovy(dds3Basic_t *pBasic);
void dds3CameraObjectBasicAlterMode(dds3Basic_t *pBasic, u32 mode);
u32 dds3CameraObjectBasicGetMode(dds3Basic_t *pBasic);
void dds3CameraObjectBasicSetTgtCam(dds3Basic_t *pBasic, const sdfVector4_t *tgtpos, const sdfVector4_t *campos);

#endif
