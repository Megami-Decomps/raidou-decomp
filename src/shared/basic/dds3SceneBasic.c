#include "common.h"
#include "sdf_resource.h"
#include "scr.h"
#include "basic.h"
#include "kernel.h"
#include "sdfMemory.h"
#include "dds3SceneBasic.h"
#include "dds3Basic.h"
#include "dds3WorldBasic.h"
#include "dds3KernelCore.h"
#include "sdfChip.h"
#include "fldFileResolver_00132080.h"
#include "scrScriptProcess.h"


extern u32 fldLoadFile_FromMemory(u32 field, u32 area, saddr_t pF1, saddr_t pF2,
                                  const struct sdfTextureOffsetListHeader_t *pTB, s32 pBF);

extern void fldFreeFile(void);
dds3Basic_t *dds3GetActiveCameraSceneBasic(dds3Basic_t *pBasic);
dds3Basic_t *dds3GetActiveUnitSceneBasic(dds3Basic_t *pSceneBasic);
void dds3SceneBasicReleaseScriptFile(dds3Basic_t *scene);
dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *scene, s32 type, const char *name);
void dds3SceneBasicFreeScene(dds3Basic_t *pBasic);

#include "sdk_libc.h"

void dds3VisibleSceneBasic(dds3Basic_t *basic, s8 visible);
dds3Basic_t *dds3SetActiveCameraSceneBasic(dds3Basic_t *pSceneBasic, dds3Basic_t *pCameraBasic);
dds3Basic_t *dds3SetActiveUnitSceneBasic(dds3Basic_t *pSceneBasic, dds3Basic_t *pUnitBasic);
dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *pBasic, u32 serialID, s32 type);
dds3BasicArrayHandle_t *dds3SceneBasicGetList(dds3Basic_t *pBasic, s32 type);

void dds3SceneBasicEndScript(dds3Basic_t *scene, const char *procname);
s32 dds3SceneBasicExistBasicFromType(dds3Basic_t *scene, s32 type, dds3Basic_t *target);
s32 dds3GetMapNo(dds3Basic_t *scene);

/* body: matched in smt3 @ 9401f79e: src/smt3/basic/dds3SceneBasic.c:36, near, same function 0.93 (words 48/49, calls 0.89, shape 0.93) */
s32 sceneconstructor(dds3Basic_t *basic)
{
    dds3SceneBasic_t *work = sdfChipAllocDirect(sizeof(dds3SceneBasic_t));
    dds3SceneBasicTable_t *list;
    s32 i;

    if (work == NULL) {
        return 0;
    }
    work->mapno = -1;
    work->visible = 1;
    work->mem_hndl = NULL;
    work->basictable = NULL;
    work->activecamera = NULL;
    work->activeunit = NULL;
    work->hFileHandle = 0;
    work->hScrFileHandle = NULL;
    work->pScrFileData = NULL;
    work->mem_hndl = sdfMemAllocDirect(BASIC_TYPE_MAX * sizeof(dds3SceneBasicTable_t));
    if (work->mem_hndl == NULL) {
        sdfChipFreeDirect(work);
        return 0;
    }
    list = sdfMemLock(work->mem_hndl);
    work->basictable = list;
    for (i = 0; i < BASIC_TYPE_MAX; i++) {
        list->linkcount = 0;
        list->firstbasic = NULL;
        list->lastbasic = NULL;
        list++;
    }
    basic->workdata = work;
    return 1;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/basic/dds3SceneBasic.c:72, near, same function 0.91 (words 53/49, calls 0.89, shape 0.90) */
void scenedestructor(dds3Basic_t *basic)
{
    dds3SceneBasic_t *work = basic->workdata;
    s32 type;

    if (work != NULL) {
        for (type = 0; type < BASIC_TYPE_MAX; type++) {
            while (work->basictable[type].firstbasic != NULL) {
                dds3DeleteSceneBasicData(work->basictable[type].firstbasic);
            }
        }
        if (work->mem_hndl != NULL) {
            sdfMemFreeDirect(work->mem_hndl);
        }
        dds3SceneBasicFreeScene(basic);
        dds3SceneBasicReleaseScriptFile(basic);
        sdfChipFreeDirect(work);
    }
}

/* body: matched in smt3 @ 9401f79e: src/smt3/basic/dds3SceneBasic.c:94, near, same function 1.00 (words 45/45, calls 1.00, shape 1.00) */
u32 scenemainprocuder(dds3Basic_t *scene)
{
    dds3SceneBasic_t *work = scene->workdata;
    dds3Basic_t *basic;
    dds3Basic_t *next;
    s32 type;

    if (work == NULL) {
        return 0;
    }
    for (type = 0; type < BASIC_TYPE_MAX; type++) {
        if (work->basictable[type].linkcount != 0) {
            basic = work->basictable[type].firstbasic;
            if (basic->basicfunction != NULL && basic->basicfunction->mainprocuder != NULL) {
                do {
                    next = basic->next;
                    ((void (*)(dds3Basic_t *))basic->basicfunction->mainprocuder)(basic);
                    basic = next;
                } while (next != NULL);
            }
        }
    }
    return 1;
}

u32 scenedrawprocuder(dds3Basic_t *scene)
{
    dds3SceneBasic_t *work = scene->workdata;
    dds3Basic_t *basic;
    dds3Basic_t *next;

    if (work == NULL) {
        return 0;
    }
    if (work->visible == 0) {
        return 1;
    }
    if (work->basictable[BASIC_TYPE_AREA].linkcount == 0) {
        return 1;
    }
    basic = work->basictable[BASIC_TYPE_AREA].firstbasic;
    if (basic->basicfunction == NULL) {
        return 1;
    }
    if (basic->basicfunction->drawprocuder == NULL) {
        return 1;
    }
    do {
        next = basic->next;
        ((void (*)(dds3Basic_t *))basic->basicfunction->drawprocuder)(basic);
        basic = next;
    } while (next != NULL);
    return 1;
}

void dds3VisibleSceneBasic(dds3Basic_t *basic, s8 visible)
{
    if ((dds3SceneBasic_t *)basic->workdata != NULL) {
        ((dds3SceneBasic_t *)basic->workdata)->visible = visible;
    }
}

dds3Basic_t *dds3AddSceneBasicData(s32 type)
{
    dds3Basic_t *scene;
    dds3Basic_t *basic;
    dds3SceneBasicTable_t *list;

    if ((u32)(type - 2) >= 15) {
        return NULL;
    }
    scene = dds3GetControlScene();
    if (scene == NULL) {
        return NULL;
    }
    list = &((dds3SceneBasic_t *)scene->workdata)->basictable[type];
    basic = dds3AllocBasic(type);
    if (basic == NULL) {
        return NULL;
    }
    basic->workpointer = scene;
    if (list->lastbasic == NULL) {
        list->firstbasic = basic;
        list->lastbasic = basic;
    } else {
        list->lastbasic->next = basic;
        basic->prev = list->lastbasic;
        list->lastbasic = basic;
    }
    list->linkcount++;
    return basic;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/basic/dds3SceneBasic.c:188, near, same function 1.00 (words 35/35, calls 1.00, shape 1.00) */
void dds3DeleteSceneBasicData(dds3Basic_t *basic)
{
    dds3SceneBasicTable_t *list;

    if (basic != NULL && (u32)((basic->type >> 24) - 2) < BASIC_TYPE_MAX - 2) {
        list = &((dds3SceneBasic_t *)basic->workpointer->workdata)->basictable[basic->type >> 24];
        if (list->firstbasic == basic) {
            list->firstbasic = basic->next;
        }
        if (list->lastbasic == basic) {
            list->lastbasic = basic->prev;
        }
        dds3DeleteBasic(basic);
        list->linkcount--;
    }
}

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

/* NON_MATCHING: 15 of 48 words match.  */
#ifdef NON_MATCHING
u32 dds3SceneBasicSearchSerialID_Used(u32 serialID)
{
    dds3Basic_t *active = dds3GetActiveScene();
    dds3Basic_t *control = dds3GetControlScene();
    s32 type;

    for (;;) {
        for (type = BASIC_TYPE_SCENE; type < BASIC_TYPE_MAX; type++) {
            if (active != NULL && dds3SearchSceneBasic(active, serialID, type) != NULL) {
                break;
            }
            if (active != control && control != NULL && dds3SearchSceneBasic(control, serialID, type) != NULL) {
                break;
            }
        }
        if (type >= BASIC_TYPE_MAX) {
            return serialID;
        }
        serialID = (serialID + 1) & 0xFFF;
    }
}
#else
INCLUDE_ASM(const s32, "basic/dds3SceneBasic", dds3SceneBasicSearchSerialID_Used);
#endif

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

void dds3SceneBasicMakeScene(dds3Basic_t *scene)
{
    dds3SceneBasic_t *work;

    work = scene->workdata;
    work->hFileHandle = fldLoadFile();
}

void dds3SceneBasicMakeScene_FromMemory(dds3Basic_t *pBasic, u32 field, u32 area, saddr_t pF1, saddr_t pF2,
                                        const struct sdfTextureOffsetListHeader_t *pTB, s32 pBF)
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

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", dds3SceneBasicLoadScriptFile);

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

dds3ProcessID_t *dds3SceneBasicStartScript(dds3Basic_t *scene, char *procname)
{
    void *script = ((dds3SceneBasic_t *)scene->workdata)->pScrFileData;
    s32 procedure;

    if (script == NULL) {
        return NULL;
    }
    if (procname == NULL) {
        return NULL;
    }
    procedure = scrSearchProcedureName(script, procname);
    if (procedure < 0) {
        return NULL;
    }
    if (dds3SearchProcessName(procname) != NULL) {
        return NULL;
    }
    /* The loop keeps ee-gcc from making this a sibling call (calls.c tries none inside a loop), as in SMT3's copy of this function. */
    do {
        return scrStartScript2(1002, script, procedure);
    } while (0);
}

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

/* body: matched in smt3 @ 9401f79e: src/smt3/basic/dds3SceneBasic.c:422, match, same function 1.00 (words 30/30, calls 1.00, shape 1.00) */
s32 dds3SceneBasicExistBasic(dds3Basic_t *scene, dds3Basic_t *target)
{
    dds3SceneBasicTable_t *lists;
    s32 type;

    if (scene == NULL || target == NULL) {
        return 0;
    }
    lists = ((dds3SceneBasic_t *)scene->workdata)->basictable;
    for (type = 0; type < BASIC_TYPE_MAX; type++) {
        dds3Basic_t *basic = lists[type].firstbasic;

        if (basic == NULL) {
            continue;
        }
        do {
            if (basic == target) {
                return 1;
            }
            basic = basic->next;
        } while (basic != NULL);
    }
    return 0;
}

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

/* body: matched in smt3 @ 9401f79e: src/smt3/basic/dds3SceneBasic.c:496, match, same function 1.00 (words 27/27, calls 1.00, shape 1.00) */
dds3Basic_t *dds3SceneBasicSearchName(dds3Basic_t *scene, const char *name)
{
    dds3Basic_t *basic;
    s32 type;

    if (scene == NULL || name == NULL) {
        return NULL;
    }
    for (type = 0; type < BASIC_TYPE_MAX; type++) {
        basic = dds3SceneBasicSearchNameAndType(scene, type, name);
        if (basic != NULL) {
            return basic;
        }
    }
    return NULL;
}
