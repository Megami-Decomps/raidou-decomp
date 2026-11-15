#include "common.h"
#include "scr.h"
#include "event.h"
#include "kernel.h"
#include "draw3d.h"
#include "libsdf.h"
#include "ps2asm.h"
#include "dds3SceneBasic.h"
#include "dds3Basic.h"

u32 dds3AllocObjectId(void);
void dds3ObjectBaseCreateMover(void *pObjectBasic);

extern void dds3ObjectBaseSetPadControl(dds3Basic_t *object,
                                     s32 (*update)(struct dds3PoseSet_t *, struct dds3Basic_t *));

extern dds3Basic_t *dds3AddCameraObjectBasicTypeFree(s32 serialID, f32 *pos, f32 *rot);
struct dds3Basic_t *dds3GetControlScene(void);

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

enum evtProcessState {
    EVT_PROCESS_STATE_DELAYED_START = 1,
    EVT_PROCESS_STATE_DESTROY_PENDING = 3
};

typedef struct effNode_t effNode_t;

typedef struct evtNodeInner_t {
    u8 pad00[0x08];
    s32 kind;
    void *node;
} evtNodeInner_t;

void evtSetTestCamera(void);

void evtSetTestCamera(void) {
    f32 position[4] = { 0.0f, -100.0f, -600.0f, 0.0f };
    sdfVector4_t rotation;
    dds3Basic_t *object;

    memset(&rotation, 0, 0x10);
    rotation.w = 1.0f;
    object = dds3AddCameraObjectBasicTypeFree(dds3AllocObjectId(), position, &rotation.x);
    dds3GetRadiusPoseSet((dds3Basic_t *)object, 10.0f);
    dds3ObjectBaseCreateMover(object);
    dds3SetActiveCameraSceneBasic(dds3GetControlScene(), (dds3Basic_t *)object);
    dds3ObjectBaseSetPadControl(object, 0);
}

INCLUDE_ASM(const s32, "event/evtStage_001B88F8", itfCriLipSyncManagerOff);

INCLUDE_ASM(const s32, "event/evtStage_001B88F8", evtCallMemset);

INCLUDE_ASM(const s32, "event/evtStage_001B88F8", evtLoadUnit);
