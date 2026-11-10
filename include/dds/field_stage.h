#ifndef DDS_FIELD_STAGE_H

#define DDS_FIELD_STAGE_H
#include "common.h"

#include "fld_sequence_scene_mode.h"

/* Stage-map entries are keyed by area/stage and store spatial layout in sixteen bytes.
 * The index and pointer lookups intentionally have different scan limits. */
typedef struct FieldStageCoordinate {
    s16 area;
    s16 stage;
    f32 originX;
    f32 originZ;
    u8 cols;
    u8 rows;
    s16 cellSize;
} FieldStageCoordinate;

typedef char FieldStageCoordinate_size_must_be_0x10[(sizeof(FieldStageCoordinate) == 0x10) ? 1 : -1];

/* Contiguous player-scene work: saved transform, status words and deferred resource.
 * The data also exports a label at +0x3C for separate object-slot consumers. */
typedef struct FieldPlayerSceneWork {
    u128 position;
    u128 rotation;
    u8 pad20[0x14];
    u32 primaryState;
    u32 sequenceWord38; /* copied from the sequence packet */
    u32 sequenceWord3C; /* copied from the sequence packet */
    char entryName[0x10]; /* 0x40: kind-0x11 transform-source name */
    s32 area; /* 0x50: sequence destination area */
    s32 room; /* 0x54: one-based destination room */
    u32 retainCachedAreaResource; /* 0x58: preserves the cached room resource for this transition. */
    FldSequenceSceneMode secondaryState;
    u16 sequenceCode; /* 0x60: copied from fldSceneParam_t.code */
    s16 sequenceMode; /* 0x62: copied by the sequence initializer */
    s32 cameraLinkMode; /* 0x64: sequence packet camera focus mode */
    u8 pad68[8];
    char cameraName[0x10]; /* 0x70: kind-4 camera-node name */
    char resourceName[0x10]; /* 0x80: sequence note; nonzero source is "elvstart_label". */
    s32 sequenceOption; /* 0x90: read by DDS2 fldRebuildFieldScene, separately copied from packet options. */
    u8 pad94[0xC];
} FieldPlayerSceneWork;

typedef char FieldPlayerSceneWork_size_must_be_0xA0[(sizeof(FieldPlayerSceneWork) == 0xA0) ? 1 : -1];

FieldStageCoordinate *fldFindStageCoordinateRecord(s32 area, s32 stage);

#endif /* DDS_FIELD_STAGE_H */
