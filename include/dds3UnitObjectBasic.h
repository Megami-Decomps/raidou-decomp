#ifndef DDS3_UNIT_OBJECT_BASIC_H
#define DDS3_UNIT_OBJECT_BASIC_H

#include "common.h"
#include "basic.h"

#define DDS3_OBJECT_RESOURCE_SLOT_COUNT 24

/* Doubly linked world index node (0x10); DDS1/2 basic/dds3WorldBasic.c. */
typedef struct WorldIndexNode {
    s16 headIndex;
    s16 tailIndex;
    s16 cursorIndex;
    u16 entryCount;
    struct WorldIndexNode *next;
    struct WorldIndexNode *previous;
} WorldIndexNode;

typedef struct dds_dds3ObjectBaseControlWorkData_t {
    u32 flags;
    struct WorldIndexNode *worldIndexNode;
    u32 resourceState;
    void *resourceHandle; /* Owner-held model context/device-motion root;
                           * borrowed views expire on release or rebuild. */
    dds3Basic_t *slots[8];
    void *extData;
    struct sdfModelRootNode_t *devSlot;
    struct sdfMotionCtrlr_t *motion;
    s32 mode;
    f32 weight;
    u32 unk44;
    s32 resourceSlotCount;
    s32 resourceSlots[DDS3_OBJECT_RESOURCE_SLOT_COUNT];
    u32 resourceSlotFamily;
    s32 currentSlot;
} dds_dds3ObjectBaseControlWorkData_t;

typedef char ObjBase_size_must_be_0xB4[(sizeof(dds_dds3ObjectBaseControlWorkData_t) == 0xB4) ? 1 : -1];

typedef char WorldIndexNodeLayoutAssert[
    (sizeof(WorldIndexNode) == 0x10 &&
     (unsigned long)&((WorldIndexNode *)0)->headIndex == 0 &&
     (unsigned long)&((WorldIndexNode *)0)->tailIndex == 2 &&
     (unsigned long)&((WorldIndexNode *)0)->cursorIndex == 4 &&
     (unsigned long)&((WorldIndexNode *)0)->entryCount == 6 &&
     (unsigned long)&((WorldIndexNode *)0)->next == 8 &&
     (unsigned long)&((WorldIndexNode *)0)->previous == 0xC) ? 1 : -1];

/* Kind-5 constructors allocate and clear the complete 0x40-byte payload. */
typedef struct EffectObjectData {
    dds_dds3ObjectBaseControlWorkData_t *handle;
    s32 word04; /* 0x04: room-mode flag selector; nonpositive bypasses room gating. */
    struct evtUnitHandle_t *transitionWork; /* 0x08: object transition work */
    dds_dds3ObjectBaseControlWorkData_t *modelHolder; /* 0x0C: owned base retains the model resource. */
    s32 activeId;
    s32 followParameterIndex; /* 0x14: 16-byte follow-parameter row; -1 selects the base record. */
    u32 word18;
    s32 pendingTargetKey; /* 0x1C: kind-0x11 world-node key used by the follow turn. */
    s32 angleReturnDelayFrames; /* 0x20: holdoff before returning the follow angle to zero. */
    u32 word24;
    f32 angle;
    f32 limitMin2C;
    f32 limitMax30;
    f32 word34; /* 0x34: entry-blend yaw, loaded directly into the float argument. */
    f32 limitMin38;
    f32 limitMax3C;
} EffectObjectData;

typedef char EffectObjectData_size_must_be_0x40[(sizeof(EffectObjectData) == 0x40) ? 1 : -1];

#endif
