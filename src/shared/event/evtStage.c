#include "common.h"
#include "scr.h"
#include "event.h"
#include "kernel.h"
#include "draw3d.h"
#include "libsdf.h"
#include "ps2asm.h"
#include "dds3SceneBasic.h"
#include "dds3AreaBasic.h"
#include "dds3Basic.h"
#include "dds3PathBasic.h"
#include "evtStage.h"
#include "dds3WorldBasic.h"
#include "dds3ObjectBase.h"
#include "dds3MoverBasic.h"
#include "dds3CameraObjectBasic.h"
#include "effManager.h"
#include "dds3KernelCore.h"
#include "sdfQuat.h"
#include "dds3KernelDraw.h"
#include "kernel_draw.h"
#include "sdfDelayedResrc.h"
#include "dds3UnitObjectBasic.h"
#include "mdlManager.h"
#include "evtUnitManager.h"
#include "itf.h"

/* declarations needed in evtStage.c (Raidou 1 retail signatures) */
extern char evtAreaPathFormat[];
extern sdf3DLight_t evtStageLights[3];
extern sdf3DFogParam_t evtStageFog;
extern sdfVector4_t evtStageAmbient;
/* Build the field world object and load its stage resource. */

u32 dds3AllocObjectId(void);



#include "basic.h"
#include "dds3obj.h"


enum {
    EVT_WORLD_SLOT_MOVIE = 3,
    EVT_WORLD_SLOT_UNIT = 5
};

struct BfFlw0Header;

#include "evt_world.h"

extern void evtSetTestCamera(void);

#include "sdk_libc.h"

extern char evtStageScriptPath[];
s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);
dds3ProcessID_t *dds3SearchProcessNumber(u32 type, s32 n);

enum evtProcessState {
    EVT_PROCESS_STATE_DELAYED_START = 1,
    EVT_PROCESS_STATE_DESTROY_PENDING = 3
};

#ifdef VERSION_RAIDOU1
extern const char evtKillFieldScriptFormat[0x28];
#else
extern const char evtKillFieldScriptFormat[];
#endif
void dds3PathBasicPauseOn(dds3PathBasicControl_t *value);
void dds3PathBasicPauseOff(dds3PathBasicControl_t *pControl);
dds3PathBasicControl_t *dds3MoverBasicGetPathControl(struct dds3Basic_t *pMoverBasic);
extern void dds3PathBasicLoopOn(struct dds3PathBasicControl_t *pControl);
extern void dds3PathBasicLoopOff(struct dds3PathBasicControl_t *pControl);

void dds3EffectSetLoop(effHandle_t *hEffect, u8 loop);

typedef struct evtNodeInner_t {
    u8 pad00[0x08];
    s32 kind;
    void *node;
} evtNodeInner_t;

void evtSetTestCamera(void);
void evtFreeStage(void);
void evtFreeAllUnit(void);
void evtKillFieldScript(void);
void evtPathStop(dds3Basic_t *basic);
void evtPathStart(dds3Basic_t *basic);
void evtPathRate(dds3Basic_t *basic, f32 rate);
void evtPathDir(dds3Basic_t *basic, u32 dir);
s32 evtPathLinkAndMake(dds3Basic_t *path, dds3Basic_t *obj);
s32 evtCheckPathStat(dds3Basic_t *obj);
void evtPathLoop(dds3Basic_t *basic, s32 sw);
dds3Basic_t *evtAddCamera(const sdfVector4_t *eye, const sdfVector4_t *rot);
void evtEffectLoopControl(dds3Basic_t *eff, s32 sw);
s32 evtStageAnimeControl(s32 type, s32 mode);
void evtSetTestCamera(void);

void evtSetTestCamera(void) {
    sdfVector4_t position = { 0.0f, -100.0f, -600.0f, 0.0f };
    sdfVector4_t rotation;
    dds3Basic_t *object;

    memset(&rotation, 0, 0x10);
    rotation.w = 1.0f;
    object = dds3AddCameraObjectBasicTypeFree(dds3AllocObjectId(), &position, &rotation);
    dds3GetRadiusPoseSet((dds3Basic_t *)object, 10.0f);
    dds3ObjectBaseCreateMover(object);
    dds3SetActiveCameraSceneBasic(dds3GetControlScene(), (dds3Basic_t *)object);
    dds3ObjectBaseSetPadControl(object, 0);
}

#ifdef VERSION_RAIDOU1
s32 evtSetRegistUnit_evt(s32 major, s32 minor) {
    sdfVector4_t position;
    sdfVector4_t rotation;
    s32 counter;
    dds3Basic_t *object;
    mdlObj_t *model;

    memset(&position, 0, 0x10);
    position.w = 1.0f;
    memset(&rotation, 0, 0x10);
    rotation.w = 1.0f;
    counter = dds3AllocObjectId();
    object = dds3AddUnitObjectBasic(counter, &position, &rotation);
    dds3ObjectBaseOnFlag(object, 0x20);
    dds3GetRadiusPoseSet(object, 10.0f);
    dds3ObjectBaseMakeUnit(object, major, minor);
    model = dds3ObjectBaseGetModelHandle(object);
    if (model->first != NULL) {
        mdlLoopMotion(dds3ObjectBaseGetModelHandle(object), 0, 0);
    }
    dds3ObjectBaseSetPadControl(object, 0);
    return counter;
}
#else
INCLUDE_ASM(const s32, "event/evtStage", itfCriLipSyncManagerOff);

INCLUDE_ASM(const s32, "event/evtStage", evtSetRegistUnit_evt);
#endif

#ifdef VERSION_RAIDOU1
s32 evtLoadUnit(s32 major, s32 minor) {
    sdfVector4_t position;
    sdfVector4_t rotation;
    s32 counter;
    dds3Basic_t *object;

    memset(&position, 0, 0x10);
    position.w = 1.0f;
    memset(&rotation, 0, 0x10);
    rotation.w = 1.0f;
    counter = dds3AllocObjectId();
    object = dds3AddUnitObjectBasic(counter, &position, &rotation);
    dds3ObjectBaseOnFlag(object, 0x20);
    dds3GetRadiusPoseSet(object, 10.0f);
    dds3ObjectBaseMakeUnit(object, major, minor);
    mdlLoopMotion(dds3ObjectBaseGetModelHandle(object), 0, 0);
    mdlSetMotionSpeedEx(dds3ObjectBaseGetModelHandle(object), 0, 0.5f);
    mdlSetMotionSpeedEx(dds3ObjectBaseGetModelHandle(object), 1, 0.5f);
    mdlSetMotionSpeedEx(dds3ObjectBaseGetModelHandle(object), 2, 0.5f);
    evtSetWaitMotionSpeed(evtGetUnitHandle2(object), 0.5f);
    dds3ObjectBaseSetControlBasic(object, dds3SearchSceneBasic(dds3GetControlScene(), 0x10000, 2));
    dds3ObjectBaseRegistArea(object);
    dds3ObjectBaseSetPadControl(object, 0);
    dds3ObjectBaseOnFlag(object, 0x400);
    return counter;
}
#else
s32 evtLoadUnit(s32 major, s32 minor) {
    sdfVector4_t position;
    sdfVector4_t rotation;
    s32 counter;
    dds3Basic_t *object;

    memset(&position, 0, 0x10);
    position.w = 1.0f;
    memset(&rotation, 0, 0x10);
    rotation.w = 1.0f;
    counter = dds3AllocObjectId();
    object = dds3AddUnitObjectBasic(counter, &position, &rotation);
    dds3ObjectBaseOnFlag(object, 0x20);
    dds3ObjectBaseMakeUnit(object, major, minor);
    mdlLoopMotion(dds3ObjectBaseGetModelHandle(object), 0, 0);
    mdlSetMotionSpeedEx(dds3ObjectBaseGetModelHandle(object), 0, 0.5f);
    mdlSetMotionSpeedEx(dds3ObjectBaseGetModelHandle(object), 1, 0.5f);
    mdlSetMotionSpeedEx(dds3ObjectBaseGetModelHandle(object), 2, 0.5f);
    evtSetWaitMotionSpeed(evtGetUnitHandle2(object), 0.5f);
    dds3GetRadiusPoseSet(object, itfCriLipSyncManagerOff(major, minor));
    dds3ObjectBaseSetControlBasic(object, dds3SearchSceneBasic(dds3GetControlScene(), 0x10000, 2));
    dds3ObjectBaseRegistArea(object);
    dds3ObjectBaseSetPadControl(object, 0);
    dds3ObjectBaseOnFlag(object, 0x400);
    return counter;
}
#endif
#ifdef VERSION_RAIDOU2

void evtLoadBustupData(void) {
    sdfDevLoadFileEx("/event/eveBustup.dat", &evtGetDefaultPosBustUpWork, &evtLoadBustupDataWork);
}
#endif

INCLUDE_ASM(const s32, "event/evtStage", evtLeaf3);

INCLUDE_ASM(const s32, "event/evtStage", evtGetDefaultPosBustUp);

INCLUDE_ASM(const s32, "event/evtStage", evtLoadUnitBustUp);

INCLUDE_ASM(const s32, "event/evtStage", evtSetLightBustup);

INCLUDE_ASM(const s32, "event/evtStage", evtGetDrawBustupNums);
#ifdef VERSION_RAIDOU2

INCLUDE_ASM(const s32, "event/evtStage", evtCallGetActiveScene);
#endif

void evtSetUnitPos(u32 unitID, u32 posID) {
    sdfVector4_t quaternion = { 0.0f, 1.0f, 0.0f, 1.0f };
    dds3Basic_t *target;
    dds3Basic_t *source;
    evtWorldSourceTransformPrefix_t *sourceTransform;

    target = dds3SearchSceneBasic(dds3GetControlScene(), unitID, 5);
    if (target != NULL) {
        source = dds3SearchSceneBasic(dds3GetControlScene(), posID, 0x10);
        if (source != NULL) {
            sourceTransform = source->workdata;
            dds3SetPosPoseSet_dds3Basic(target, &sourceTransform->position);
            SDF_VU0_LOADVECTOR(&quaternion);
            FromRotation(3.14159265f);
            SDF_VU0_LOADVECTOR_B(sourceTransform->rotation);
            Mult();
            SDF_VU0_STOREVECTOR(&quaternion);
            dds3SetRotPoseSet(target, &quaternion);
            dds3GetPosPoseSet(target);
            SDF_VU0_STOREVECTOR(&target->poseset->pos2);
        }
    }
}

void evtFreeStage(void)
{
    dds3Basic_t *node;

    node = (dds3Basic_t *)dds3GetControlScene();
    if (node != 0) {
        dds3DeleteScene(node);
    }
}

/* Detach every unit node from the secondary object's per-kind list. */
void evtFreeAllUnit(void) {
    dds3Basic_t *secondaryObject = (dds3Basic_t *)dds3GetControlScene();
    evtWorldTable_t *worldTable;

    if (secondaryObject != NULL) {
        worldTable = ((evtWorldTable_t *)secondaryObject->workdata);
        while (worldTable->slots[EVT_WORLD_SLOT_UNIT].head != NULL) {
            dds3DeleteSceneBasicData(worldTable->slots[EVT_WORLD_SLOT_UNIT].head);
        }
    }
}

s32 evtLoadArea(s32 fld, s32 area) {
    dds3Basic_t *worldObject;

    evtFreeStage();
    worldObject = dds3AddScene();
    dds3SetControlScene(worldObject);
    dds3SetActiveScene(worldObject);
    dds3SetMapNo(worldObject, (fld << 16) + area);
    dds3SetActiveScene(NULL);
    dds3AddAreaBasic(dds3AllocObjectId(), evtAreaPathFormat);
    dds3SetActiveScene(dds3GetControlScene());
    dds3SceneBasicMakeScene(worldObject);
    evtSetTestCamera();
    dds3SetScreenFovy(0.75398216f);
    dds3SetLight(&evtStageLights[0], 0);
    dds3SetLight(&evtStageLights[1], 1);
    dds3SetLight(&evtStageLights[2], 2);
    dds3SetFog(&evtStageFog);
    dds3SetAmbient(&evtStageAmbient);
    return 1;
}

s32 evtLoadArea_FromMemory(s32 fld, s32 area, saddr_t pF1, saddr_t pF2,
                           const sdfTextureOffsetListHeader_t *pTB, s32 pBF) {
    dds3Basic_t *worldObject;

    evtFreeStage();
    worldObject = dds3AddScene();
    dds3SetControlScene(worldObject);
    dds3SetActiveScene(worldObject);
    dds3SetMapNo(worldObject, (fld << 16) + area);
    dds3SceneBasicMakeScene_FromMemory(worldObject, fld, area, pF1, pF2, pTB, pBF);
    evtSetTestCamera();
    dds3SetScreenFovy(0.75398216f);
    dds3SetLight(&evtStageLights[0], 0);
    dds3SetLight(&evtStageLights[1], 1);
    dds3SetLight(&evtStageLights[2], 2);
    dds3SetFog(&evtStageFog);
    dds3SetAmbient(&evtStageAmbient);
    while (sdfDelayedResrcCheck() != 0) {
    }
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtStage", evtAreaPathFormat);

#ifdef VERSION_RAIDOU1
void evtStartEvent(s32 no) {
    sprintf(evtStageScriptPath, "/event/e%03d/e%03d/scr/e%03d.bf", no - no % 10, no, no);
    scrStartScriptByName(0x3EB, evtStageScriptPath, 0);
}
#else
INCLUDE_ASM(const s32, "event/evtStage", evtStartEvent);
#endif

void evtKillFieldScript(void)
{
    s32 taskState;
    s32 taskIndex;
    dds3ProcessID_t *task;

    for (taskState = EVT_PROCESS_STATE_DELAYED_START; taskState < EVT_PROCESS_STATE_DESTROY_PENDING; taskState++) {
    restartStateScan:
        for (taskIndex = 0; taskIndex < dds3GetProcessCount(taskState); taskIndex++) {
            task = dds3SearchProcessNumber(taskState, taskIndex);
            if (task->priority == 0x3EA) {
                printf(evtKillFieldScriptFormat, task->name);
                dds3KillProcess(task, 0);
                /* Destruction changes the queue, so restart the current scan. */
                goto restartStateScan;
            }
        }
    }
}

void evtPathStop(dds3Basic_t *basic)
{
    dds3PathBasicControl_t *slotData;

    slotData = dds3ObjectBaseGetPathControl(basic);
    if (slotData != 0) {
        dds3PathBasicPauseOn(slotData);
    }
}

void evtPathStart(dds3Basic_t *basic)
{
    dds3PathBasicControl_t *slotData;

    slotData = dds3ObjectBaseGetPathControl(basic);
    if (slotData != 0) {
        dds3PathBasicPauseOff(slotData);
    }
}

/* Scale the object's retained path time within the clamped multiplier range. */
void evtPathRate(dds3Basic_t *basic, f32 rate) {
    dds3PathBasicControl_t *slotData;

    slotData = dds3ObjectBaseGetPathControl(basic);
    if (slotData != 0) {
        if (rate < 0.0f) {
            rate = 0.0f;
        }
        if (rate > 1.0f) {
            rate = 1.0f;
        }
        dds3PathBasicControlSetRatio(slotData, rate);
    }
}

void evtPathDir(dds3Basic_t *basic, u32 dir) {
    dds3PathBasicControl_t *slotData;

    slotData = dds3ObjectBaseGetPathControl(basic);
    if (slotData != 0) {
        dds3PathBasicControlAlterMode(slotData, dir);
    }
}

/* Attach object to the node referenced by owner's owned handle. */
s32 evtPathLinkAndMake(dds3Basic_t *path, dds3Basic_t *obj) {
    dds3ObjectBaseControlWorkData_t *ref;
    dds3Basic_t *slotNode;

    if (path == NULL) {
        return 0;
    }
    ref = dds3GetBaseControlWorkData(obj);
    if (ref == NULL) {
        return 0;
    }
    slotNode = ref->pBasic[1];
    if (slotNode == NULL) {
        return 0;
    }
    dds3MoverBasicSetPath(slotNode, path);
    dds3MoverBasicMakePath(slotNode);
    return 1;
}

s32 evtCheckPathStat(dds3Basic_t *obj) {
    f32 target = 1.0f;
    dds3Basic_t *slot;
    dds3PathBasicControl_t *handle;
    s32 kind;

    if (obj == NULL) {
        return -1;
    }
    slot = dds3ObjectBaseGetControlBasic(obj, 1);
    if (slot == NULL) {
        return -1;
    }
    handle = dds3MoverBasicGetPathControl(slot);
    if (handle == NULL) {
        return -1;
    }
    kind = dds3PathBasicControlGetMode(handle);
    if (kind != 0) {
        if (kind != 1) {
            return -1;
        }
        target = 0.0f;
    }
    if (dds3PathBasicControlGetRatio(handle) == target) {
        return 0;
    }
    return 1;
}

void evtPathLoop(dds3Basic_t *basic, s32 sw) {
    dds3PathBasicControl_t *data = dds3ObjectBaseGetPathControl(basic);

    if (data == NULL) {
        return;
    }
    if (sw != 0) {
        dds3PathBasicLoopOn(data);
    } else {
        dds3PathBasicLoopOff(data);
    }
}

dds3Basic_t *evtAddCamera(const sdfVector4_t *eye, const sdfVector4_t *rot) {
    dds3Basic_t *obj = dds3AddCameraObjectBasicTypeFree(dds3AllocObjectId(), eye, rot);

    if (obj == 0) {
        return obj;
    }
    dds3GetRadiusPoseSet(obj, 1.0f);
    return obj;
}

void evtEffectLoopControl(dds3Basic_t *eff, s32 sw) {
    evtNodeInner_t *inner = eff->workdata;
    void *node;

    switch (inner->kind) {
    case 1:
    case 7:
    case 8:
        break;
    default:
        return;
    }
    node = inner->node;
    if (sw == 0) {
        dds3EffectSetLoop(node, sw);
    }
}

s32 evtStageAnimeControl(s32 type, s32 mode) {
    dds3Basic_t *world = dds3GetActiveScene();
    dds3Basic_t *node;
    s32 combinedMode;

    if (world == NULL) {
        return 0;
    }
    combinedMode = type * 3 + mode;
    for (node = ((evtWorldTable_t *)world->workdata)->slots[6].head; node != NULL; node = node->next) {
        dds3ObjectBaseMatterAnimeSpeed(node, combinedMode);
    }
    return 1;
}
#ifdef VERSION_RAIDOU2
const char evtKillFieldScriptFormat[0x20] = "kill field script -> [%s]\n";
#endif

#ifdef VERSION_RAIDOU1
const char evtKillFieldScriptFormat[0x28] = "kill field script -> [%s]\n";
#else
INCLUDE_SDATA(const s32, "event/evtStage", evtStartEventData);

INCLUDE_SDATA(const s32, "event/evtStage", evtPendingEventSelection);

#endif
