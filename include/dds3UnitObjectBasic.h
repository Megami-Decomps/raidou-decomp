#ifndef DDS3_UNIT_OBJECT_BASIC_H
#define DDS3_UNIT_OBJECT_BASIC_H

#include "common.h"
#include "basic.h"

/* Kind-5 constructors allocate and clear the complete 0x40-byte payload. */
typedef struct effObjectData_t {
    dds3ObjectBaseControlWorkData_t *handle;
    s32 word04; /* 0x04: room-mode flag selector; nonpositive bypasses room gating. */
    struct evtUnitHandle_t *transitionWork; /* 0x08: object transition work */
    dds3ObjectBaseControlWorkData_t *modelHolder; /* 0x0C: owned base retains the model resource. */
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
} effObjectData_t;

typedef char effObjectData_size_must_be_0x40[(sizeof(effObjectData_t) == 0x40) ? 1 : -1];

#include "sdf_types.h"

dds3Basic_t *dds3AddUnitObjectBasic(u32 serialID, const sdfVector4_t *aPos, const sdfVector4_t *aRot);
void dds3UnitObjectBasicSetOt(dds3Basic_t *aPtr, s32 idx);
struct evtUnit_t *dds3GetUnitHandleObjectBasic(dds3Basic_t *b);
void dds3UnitObjectBasicSetKubiTerget(dds3Basic_t *pBasic, const sdfVector4_t *aTgt);
void dds3UnitObjectBasicSetKubiModori(dds3Basic_t *pBasic);
void dds3UnitObjectBasicKubiOn(dds3Basic_t *pBasic, s32 posid);
void dds3UnitObjectBasicKubiOff(dds3Basic_t *pBasic);

#endif
