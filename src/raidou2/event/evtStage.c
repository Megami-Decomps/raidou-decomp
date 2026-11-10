#include "common.h"

/* Declarations from smt3-decomp src/smt3/event/evtStage.c */
#include "common.h"

#include "scr.h"

#include "event.h"

#include "kernel.h"

#include "draw3d.h"

#include "libsdf.h"

#include "ps2asm.h"

extern u64 sdfDelayedResrcCheck(void);

void dds3GetRadiusPoseSet(dds3Basic_t *node, f32 value);

u32 dds3AllocObjectId(void);

void dds3ObjectBaseCreateMover(void *pObjectBasic);

void dds3ObjectBaseSetPadControl(dds3Basic_t *object,
                                     s32 (*update)(struct dds3PoseSet_t *, struct dds3Basic_t *));

dds3Basic_t *dds3SetActiveCameraSceneBasic(dds3Basic_t *pSceneBasic, dds3Basic_t *pCameraBasic);

extern dds3Basic_t *dds3AddCameraObjectBasicTypeFree(s32 serialID, f32 *pos, f32 *rot);

struct dds3Basic_t *dds3GetControlScene(void);

extern void *memset(void *a, s32 b, size_t c);

void mdlLoopMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber);

void *dds3ObjectBaseGetModelHandle(dds3Basic_t *pObjectBasic);

void *dds3ObjectBaseSetControlBasic(dds3Basic_t *pObjectBasic, dds3Basic_t *pItemBasic);

dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *pBasic, u32 serialID, s32 type);

extern void dds3ObjectBaseMakeUnit(dds3Basic_t *pObjectBasic, s32, s32);

extern dds3Basic_t *dds3AddUnitObjectBasic(s32 serialID, void *aPos, void *aRot);

extern void dds3ObjectBaseOnFlag(void *object, s32 flags);

extern void dds3ObjectBaseRegistArea(void *pObjectBasic);

void evtSetUnitPos(u32 unitID, u32 posID);

void Mult(void);

#include "basic.h"

#include "dds3obj.h"

void dds3GetPosPoseSet(dds3Basic_t *pBasic);

extern void FromRotation(f32 aAngle);

void dds3DeleteScene(struct dds3Basic_t *scene);

void dds3DeleteSceneBasicData(dds3Basic_t *basic);

enum {
    EVT_WORLD_SLOT_MOVIE = 3,
    EVT_WORLD_SLOT_UNIT = 5
};

struct BfFlw0Header;

#include "evt_world.h"

void fldMakeFilePath(char *pathstr, s32 field, s32 area);

void dds3SceneBasicMakeScene(dds3Basic_t *object, addr_t resourceId);

void dds3SetActiveScene(struct dds3Basic_t *scene);

void dds3SetControlScene(struct dds3Basic_t *scene);

void dds3SetMapNo(dds3Basic_t *scene, u32 mapno);

s32 evtLoadArea(s32 fld, s32 area);

extern dds3Basic_t *dds3AddScene(void);

extern void evtSetTestCamera(void);

extern void dds3SetScreenFovy(f32 fovy);

extern void dds3SetLight(void *light, s32 no);

extern void dds3SetFog(void *fog);

extern void dds3SetAmbient(u128 *ambient);

/* Three parallel lights, 0x40 each, which is what the IDB types here and what
 * the +0x40 steps below were spelling as byte arithmetic. */
extern sdf3DLight_t evtStageLights[3];

extern sdf3DFogParam_t evtStageFog;

extern u128 evtStageAmbient;

#include "sdk_libc.h"

extern void dds3SceneBasicMakeScene_FromMemory(dds3Basic_t *pBasic, u32 field, u32 area,
                                                       u32 pF1, u32 pF2,
                                                       const sdfTextureOffsetListHeader_t *pTB, u32 pBF);

extern char evtStageScriptPath[];

s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);

s32 dds3GetProcessCount(u32 type);

dds3ProcessID_t *dds3SearchProcessNumber(u32 type, s32 n);

enum evtProcessState {
    EVT_PROCESS_STATE_DELAYED_START = 1,
    EVT_PROCESS_STATE_DESTROY_PENDING = 3
};

extern const char evtKillFieldScriptFormat[0x28];

void dds3PathBasicPauseOn(dds3PathBasicControl_t *value);

dds3PathBasicControl_t *dds3ObjectBaseGetPathControl(void *pObjectBasic);

void dds3PathBasicPauseOff(dds3PathBasicControl_t *pControl);

void dds3PathBasicControlSetRatio(dds3PathBasicControl_t *pControl, f32 ratio);

void dds3MoverBasicSetPath(dds3Basic_t *pMoverBasic, dds3Basic_t *pPathBasic);

extern void dds3MoverBasicMakePath(dds3Basic_t *pMoverBasic);

f32 dds3PathBasicControlGetRatio(const dds3PathBasicControl_t *pControl);

void *dds3ObjectBaseGetControlBasic(dds3Basic_t *pObjectBasic, s32 itemType);

dds3PathBasicControl_t *dds3MoverBasicGetPathControl(struct dds3Basic_t *pMoverBasic);

extern void dds3PathBasicLoopOn(struct dds3PathBasicControl_t *pControl);

extern void dds3PathBasicLoopOff(struct dds3PathBasicControl_t *pControl);

typedef struct effNode_t effNode_t;

void dds3EffectSetLoop(effNode_t *hEffect, u8 loop);

typedef struct evtNodeInner_t {
    u8 pad00[0x08];
    s32 kind;
    void *node;
} evtNodeInner_t;

void dds3ObjectBaseMatterAnimeSpeed(struct dds3Basic_t *pObjectBasic, u32 mode);

struct dds3Basic_t *dds3GetActiveScene(void);

void evtSetTestCamera(void);

s32 evtSetRegistUnit(s32 major, s32 minor);

s32 evtLoadUnit(s32 major, s32 minor);

void evtSetUnitPos(u32 unitID, u32 posID);

void evtFreeStage(void);

/* Detach every unit node from the secondary object's per-kind list. */
void evtFreeAllUnit(void);

/* Sized explicitly: retail pads this to 16 bytes and an inline literal would
 * emit 13 plus whatever gcc aligns to, which shortens .rodata and shifts the
 * link. The same padding is why the strings around it stay placeholders. */
extern const char evtAreaPathFormat[0x10];

/* Build the field world object and load its stage resource. */
s32 evtLoadArea(s32 fld, s32 area);

/* Build a field world object from the supplied resource parameters. */
s32 evtLoadArea_FromMemory(s32 fld, s32 area, saddr_t pF1, saddr_t pF2,
                                     const sdfTextureOffsetListHeader_t *pTB, s32 pBF);

void evtStartEvent(s32 no);

extern const char evtKillFieldScriptFormat[0x28];

void evtKillFieldScript(void);

void evtPathStop(dds3Basic_t *basic);

void evtPathStart(dds3Basic_t *basic);

/* Scale the object's retained path time within the clamped multiplier range. */
void evtPathRate(dds3Basic_t *basic, f32 rate);

void evtPathDir(dds3Basic_t *basic, u32 dir);

/* Attach object to the node referenced by owner's owned handle. */
s32 evtPathLinkAndMake(dds3Basic_t *path, dds3Basic_t *obj);

s32 evtCheckPathStat(dds3Basic_t *obj);

void evtPathLoop(dds3Basic_t *basic, s32 sw);

dds3Basic_t *evtAddCamera(f32 *eye, f32 *rot);

void evtEffectLoopControl(dds3Basic_t *eff, s32 sw);

s32 evtStageAnimeControl(s32 type, s32 mode);
/* End of declarations from src/smt3/event/evtStage.c */

INCLUDE_ASM(const s32, "event/evtStage", evtSetUnitPos);

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

INCLUDE_RODATA(const s32, "event/evtStage", D_004B4460);

INCLUDE_ASM(const s32, "event/evtStage", func_001B95C0);

INCLUDE_ASM(const s32, "event/evtStage", func_001B96B8);

INCLUDE_ASM(const s32, "event/evtStage", func_001B97D0);

INCLUDE_ASM(const s32, "event/evtStage", evtKillFieldScript);

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

dds3Basic_t *evtAddCamera(f32 *eye, f32 *rot) {
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
INCLUDE_SDATA(const s32, "event/evtStage", D_004DF838);

INCLUDE_SDATA(const s32, "event/evtStage", D_004DF83C);

