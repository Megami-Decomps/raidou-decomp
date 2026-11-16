#ifndef EVT_EVENT_VIEWER_MENU_H
#define EVT_EVENT_VIEWER_MENU_H

#include "common.h"
#include "sdf_resource.h"
#include "evtEventPack.h"

/* Camera-color timeline keys interpolate two banks of three values. */
typedef struct evtBlendKey_t {
    s32 w[4];
    s32 x;
    u32 flagWord;
    u8 pad18[8];
    s32 y[3];
    s32 z[3];
} evtBlendKey_t;

typedef char evtBlendKeySizeCheck[(sizeof(evtBlendKey_t) == 0x38) ? 1 : -1];

/* Allocated and serialized type-0x19 payload; the final bytes remain opaque. */
typedef struct evtCameraColorPayload_t {
    evtBlendKey_t parameters;
    u8 unknown38[8];
} evtCameraColorPayload_t;

typedef char evtCameraColorPayload_size[(sizeof(evtCameraColorPayload_t) == 0x40) ? 1 : -1];

/* Payload categories used by the fixed-stride BE resource table. */
typedef enum EvtPackKind {
    EVT_PACK_KIND_INVALID = -1,
    EVT_PACK_KIND_ENTRY_POINT = 0,
    EVT_PACK_KIND_AREA_ROOM_IDS = 1,
    EVT_PACK_KIND_AREA_RESOURCE_PRIMARY = 2,
    EVT_PACK_KIND_AREA_RESOURCE_SECONDARY = 3,
    EVT_PACK_KIND_TEXTURE_OFFSETS = 4,
    EVT_PACK_KIND_MODEL = 5,
    EVT_PACK_KIND_MOTION_CUES = 6
} EvtPackKind;

/* Key used to locate a payload by the BE record's secondaryResourceId. */
typedef s32 EvtPackSecondaryResourceId;

/* FIELD_BE selector matched to each required resource row's key field. */
typedef s32 EvtPackFieldResourceId;

/* MODEL_BE key used to select a motion-cue record. */
typedef s32 EvtPackMotionResourceId;

/* Entry resource keys are interpreted according to evtPackEntry_t.kind. */
typedef union EvtPackResourceId {
    EvtPackFieldResourceId field; /* resource and texture-offset entries */
    EvtPackMotionResourceId motion; /* motion-cue entries */
} EvtPackResourceId;

/* Numeric event key used to name and find the camp's event-pack task. */
typedef s32 EvtPackEventId;

typedef enum EvtPackLoadPhase {
    EVT_PACK_LOAD_PHASE_INVALID = -1,
    EVT_PACK_LOAD_NOT_STARTED = 0,
    EVT_PACK_LOAD_REQUEST_PENDING = 1,
    EVT_PACK_LOAD_COMPLETE = 2
} EvtPackLoadPhase;

typedef char evtPackEntry_size_must_be_0x20[(sizeof(evtPackEntry_t) == 0x20) ? 1 : -1];

typedef char evtPackHeader_size_must_be_0x20[(sizeof(evtPackHeader_t) == 0x20) ? 1 : -1];

typedef char EvtPackLoadPhase_must_be_4_bytes[(sizeof(EvtPackLoadPhase) == 4) ? 1 : -1];

#define DDS3_TASK_UPDATE_RETAIN_CALLBACK 0

#endif
