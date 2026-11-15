#ifndef DDS3SCENEBASIC_H
#define DDS3SCENEBASIC_H

#include "common.h"
#include "basic.h"

dds3Basic_t *dds3AddSceneBasicData(s32 type);
void dds3DeleteSceneBasicData(dds3Basic_t *basic);
dds3Basic_t *dds3GetActiveUnitSceneBasic(dds3Basic_t *pSceneBasic);
s32 dds3GetMapNo(dds3Basic_t *scene);
dds3BasicArrayHandle_t *dds3SceneBasicGetList(dds3Basic_t *pBasic, s32 type);
dds3Basic_t *dds3SceneBasicSearchName(dds3Basic_t *scene, const char *name);
dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *scene, s32 type, const char *name);
dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *pBasic, u32 serialID, s32 type);
dds3Basic_t *dds3SetActiveCameraSceneBasic(dds3Basic_t *pSceneBasic, dds3Basic_t *pCameraBasic);

#endif
