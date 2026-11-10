#include "common.h"

/* Declarations from smt3-decomp src/smt3/basic/dds3SceneBasic.c */
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
/* End of declarations from src/smt3/basic/dds3SceneBasic.c */

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_0011A628);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_0011A6E8);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", scenemainprocuder);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_0011A878);

void dds3VisibleSceneBasic(dds3Basic_t *basic, s8 visible)
{
    if ((dds3SceneBasic_t *)basic->workdata != NULL) {
        ((dds3SceneBasic_t *)basic->workdata)->visible = visible;
    }
}

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_0011A928);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_0011A9D0);

dds3Basic_t *dds3SetActiveCameraSceneBasic(dds3Basic_t *pSceneBasic, dds3Basic_t *pCameraBasic)
{
    dds3SceneBasic_t *work;
    dds3Basic_t *previous;

    work = pSceneBasic->workdata;
    previous = dds3GetActiveCameraSceneBasic(pSceneBasic);
    work->activecamera = pCameraBasic;
    return previous;
}

dds3Basic_t *dds3GetActiveCameraSceneBasic(dds3Basic_t *pBasic)
{
    return ((dds3SceneBasic_t *)pBasic->workdata)->activecamera;
}

dds3Basic_t *dds3SetActiveUnitSceneBasic(dds3Basic_t *pSceneBasic, dds3Basic_t *pUnitBasic)
{
    dds3SceneBasic_t *work;
    dds3Basic_t *previous;

    work = pSceneBasic->workdata;
    previous = dds3GetActiveUnitSceneBasic(pSceneBasic);
    work->activeunit = pUnitBasic;
    return previous;
}

dds3Basic_t *dds3GetActiveUnitSceneBasic(dds3Basic_t *pSceneBasic)
{
    return ((dds3SceneBasic_t *)pSceneBasic->workdata)->activeunit;
}

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_0011AAF0);

dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *pBasic, u32 serialID, s32 type)
{
    dds3Basic_t *basic;

    if (((dds3SceneBasic_t *)pBasic->workdata)->basictable[type].linkcount == 0) {
        return NULL;
    }
    basic = ((dds3SceneBasic_t *)pBasic->workdata)->basictable[type].firstbasic;
    do {
        if (serialID == basic->serialID) {
            return basic;
        }
        basic = basic->next;
    } while (basic != NULL);
    return NULL;
}

dds3BasicArrayHandle_t *dds3SceneBasicGetList(dds3Basic_t *pBasic, s32 type)
{
    dds3SceneBasic_t *work = pBasic->workdata;
    dds3BasicArrayHandle_t *handle;
    dds3Basic_t *basic;

    if (work->basictable[type].linkcount == 0) {
        return NULL;
    }
    handle = dds3GetBasicArrayHandle(0);
    basic = work->basictable[type].firstbasic;
    do {
        dds3AddBasicArray(handle, 1);
        dds3SetBasicArray(handle, basic);
        basic = basic->next;
    } while (basic != NULL);
    return handle;
}

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_0011ACB0);

void dds3SceneBasicMakeScene_FromMemory(dds3Basic_t *pBasic, u32 field, u32 area,
                                       void *pF1, void *pF2,
                                       void *pTB, u32 pBF)
{
    dds3SceneBasic_t *work;
    u32 handle;

    work = pBasic->workdata;
    handle = fldLoadFile_FromMemory(field, area, pF1, pF2, pTB, pBF);
    work->hFileHandle = handle;
}

void dds3SceneBasicFreeScene(dds3Basic_t *pBasic)
{
    dds3SceneBasic_t *work;

    work = pBasic->workdata;
    fldFreeFile();
    work->hFileHandle = 0;
}

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_0011AD40);

/* A null handle preserves the old script; replacement releases it first. */
s32 dds3SceneBasicLoadScriptFile_FromMem(dds3Basic_t *scene, sdfMemHandle_t *mem)
{
    s32 notAttached = 0;
    void *address;
    dds3SceneBasic_t *work = scene->workdata;

    if (mem == NULL) {
        return notAttached;
    }
    if (work->hScrFileHandle != NULL) {
        dds3SceneBasicReleaseScriptFile(scene);
    }
    address = sdfMemLock(mem);
    if (address != NULL) {
        work->hScrFileHandle = mem;
        work->pScrFileData = address;
        return 1;
    }
    return notAttached;
}

void dds3SceneBasicReleaseScriptFile(dds3Basic_t *scene)
{
    dds3SceneBasic_t *work = scene->workdata;

    if (work->hScrFileHandle != NULL) {
        sdfMemFreeDirect(work->hScrFileHandle);
    }
    work->hScrFileHandle = NULL;
    work->pScrFileData = NULL;
}

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_0011AE60);

void dds3SceneBasicEndScript(dds3Basic_t *scene, const char *procname)
{
    dds3ProcessID_t *process;

    if (procname != NULL) {
        process = dds3SearchProcessName(procname);
        if (process == NULL) {
            return;
        }
        dds3KillProcess(process, 0);
    }
}

s32 dds3SceneBasicExistBasicFromType(dds3Basic_t *scene, s32 type, dds3Basic_t *target)
{
    dds3Basic_t *basic;

    if (scene == NULL || target == NULL) {
        return 0;
    }
    basic = ((dds3SceneBasic_t *)scene->workdata)->basictable[type].firstbasic;
    while (basic != NULL) {
        if (basic == target) {
            return 1;
        }
        basic = basic->next;
    }
    return 0;
}

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_0011AF88);

void dds3SetMapNo(dds3Basic_t *scene, u32 mapno)
{
    if (scene != NULL) {
        ((dds3SceneBasic_t *)scene->workdata)->mapno = mapno;
    }
}

s32 dds3GetMapNo(dds3Basic_t *scene)
{
    if (scene == NULL) {
        return -1;
    }
    return ((dds3SceneBasic_t *)scene->workdata)->mapno;
}

dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *scene, s32 type, const char *name)
{
    dds3Basic_t *basic;
    u8 *candidate;
    s32 i;

    if (scene == NULL || name == NULL) {
        return NULL;
    }
    basic = ((dds3SceneBasic_t *)scene->workdata)->basictable[type].firstbasic;
    while (basic != NULL) {
        candidate = (u8 *)basic->name;
        if (candidate == NULL) {
            basic = basic->next;
            continue;
        }
        if (candidate[0] == '\0') {
            basic = basic->next;
            continue;
        }
        i = 0;
        if (((const u8 *)name)[0] == candidate[0]) {
            do {
                if (((const u8 *)name)[i] == '\0') {
                    return basic;
                }
                i++;
            } while (((const u8 *)name)[i] == ((u8 *)basic->name)[i]);
        }
        basic = basic->next;
    }
    return NULL;
}

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_0011B0E0);
