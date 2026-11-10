/* Declarations of smt3-decomp src/smt3/basic/dds3SceneBasic.c, for its bodies in this directory. */
#ifndef SHARED_BASIC_DDS3SCENEBASIC_H
#define SHARED_BASIC_DDS3SCENEBASIC_H

#include "common.h"

#include "sdf_resource.h"

#include "scr.h"

#include "basic.h"

#include "kernel.h"

extern dds3Basic_t *dds3GetControlScene(void);

extern dds3Basic_t *dds3AllocBasic(u32 type);

extern void dds3DeleteBasic(dds3Basic_t *basic);

extern void drawSceneBasicList(void);

extern dds3BasicArrayHandle_t *dds3GetBasicArrayHandle(s32 bufcnt);

extern s32 dds3AddBasicArray(dds3BasicArrayHandle_t *hndl, s32 bufcnt);

extern s32 dds3SetBasicArray(dds3BasicArrayHandle_t *hndl, dds3Basic_t *data);

extern u32 fldLoadFile(const char *name);

extern u32 fldLoadFile_FromMemory(u32 field, u32 area, void *pF1,
                                  void *pF2, void *pTB, u32 pBF);

extern void fldFreeFile(void);

extern void *sdfMemLock(sdfMemHandle_t *aHandle);

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern dds3ProcessID_t *dds3SearchProcessName(const char *namestr);

extern void dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);

dds3Basic_t *dds3GetActiveCameraSceneBasic(dds3Basic_t *pBasic);

dds3Basic_t *dds3GetActiveUnitSceneBasic(dds3Basic_t *pSceneBasic);

void dds3SceneBasicReleaseScriptFile(dds3Basic_t *scene);

dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *scene, s32 type, const char *name);

void dds3DeleteSceneBasicData(dds3Basic_t *basic);

void dds3SceneBasicFreeScene(dds3Basic_t *pBasic);

extern void *sdfChipAllocDirect(u32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

extern sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

extern s32 scrSearchProcedureName(void *scrdata, char *procname);

#include "sdk_libc.h"

s32 sceneconstructor(dds3Basic_t *basic);

void scenedestructor(dds3Basic_t *basic);

/* Preserve the next link across callbacks that can delete their own basic. */
u32 scenemainprocuder(dds3Basic_t *scene);

u32 scenedrawprocuder(dds3Basic_t *scene);

void dds3VisibleSceneBasic(dds3Basic_t *basic, s8 visible);

dds3Basic_t *dds3AddSceneBasicData(s32 type);

void dds3DeleteSceneBasicData(dds3Basic_t *basic);

dds3Basic_t *dds3SetActiveCameraSceneBasic(dds3Basic_t *pSceneBasic, dds3Basic_t *pCameraBasic);

dds3Basic_t *dds3GetActiveCameraSceneBasic(dds3Basic_t *pBasic);

dds3Basic_t *dds3SetActiveUnitSceneBasic(dds3Basic_t *pSceneBasic, dds3Basic_t *pUnitBasic);

dds3Basic_t *dds3GetActiveUnitSceneBasic(dds3Basic_t *pSceneBasic);

dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *pBasic, u32 serialID, s32 type);

dds3BasicArrayHandle_t *dds3SceneBasicGetList(dds3Basic_t *pBasic, s32 type);

void dds3SceneBasicMakeScene(dds3Basic_t *scene, const char *name);

void dds3SceneBasicMakeScene_FromMemory(dds3Basic_t *pBasic, u32 field, u32 area,
                                       void *pF1, void *pF2,
                                       void *pTB, u32 pBF);

void dds3SceneBasicFreeScene(dds3Basic_t *pBasic);

s32 dds3SceneBasicLoadScriptFile(dds3Basic_t *scene, char *name);

/* A null handle preserves the old script; replacement releases it first. */
s32 dds3SceneBasicLoadScriptFile_FromMem(dds3Basic_t *scene, sdfMemHandle_t *mem);

void dds3SceneBasicReleaseScriptFile(dds3Basic_t *scene);

saddr_t dds3SceneBasicStartScript(dds3Basic_t *scene, char *procname);

void dds3SceneBasicEndScript(dds3Basic_t *scene, const char *procname);

s32 dds3SceneBasicExistBasicFromType(dds3Basic_t *scene, s32 type, dds3Basic_t *target);

s32 dds3SceneBasicExistBasic(dds3Basic_t *scene, dds3Basic_t *target);

void dds3SetMapNo(dds3Basic_t *scene, u32 mapno);

s32 dds3GetMapNo(dds3Basic_t *scene);

dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *scene, s32 type, const char *name);

dds3Basic_t *dds3SceneBasicSearchName(dds3Basic_t *scene, const char *name);

#endif
