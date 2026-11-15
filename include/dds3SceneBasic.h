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

#include "kernel.h"

s32 dds3SceneBasicExistBasic(dds3Basic_t *scene, dds3Basic_t *target);
void dds3SceneBasicMakeScene_FromMemory(dds3Basic_t *pBasic, u32 field, u32 area, void *pF1, void *pF2, void *pTB, u32 pBF);
u32 dds3SceneBasicSearchSerialID_Used(u32 serialID);
s32 dds3SceneBasicLoadScriptFile_FromMem(dds3Basic_t *scene, sdfMemHandle_t *mem);
dds3ProcessID_t *dds3SceneBasicStartScript(dds3Basic_t *scene, char *procname);

#endif
