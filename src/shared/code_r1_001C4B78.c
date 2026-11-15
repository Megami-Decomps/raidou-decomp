#include "common.h"
#include "raidou_alloc.h"
#include "mdlManager.h"
#include "dds3DebugStuff.h"

#include "newdata.h"
#include "sdfDevCons.h"
#include "sdk_libc.h"
#include "sdk_libkernl.h"
#include "include_asm.h"
#include "basic.h"
#include "evt_world.h"
#include "sdf_resource.h"
#include "billboard.h"
#include "sdf_types.h"
#include "draw.h"
#include "libsdf.h"
#include "sdf_movie_types.h"
#include "field.h"
#include "kernel.h"
#include "draw3d.h"
#include "ps2asm.h"
#include "code_r1_001C4B78.h"
#include "evtEventViewerMenu.h"
#include "evtEventViewer.h"

/* The primary object returned by dds3GetActiveScene is distinct from the active
 * world's WorldInfo payload. Event lookup, lip-sync and viewer code share this chain:
 * object +0x18 -> table +0x08 -> 0x0C-byte slot entries, each with head +0x04.
 * Node payloads depend on the slot. */
enum {
    EVT_WORLD_SLOT_MOVIE = 3, /* viewer movie-time lists: byte offset 0x28 */
    EVT_WORLD_SLOT_CAMERA = 4, /* linked cameras; active nodes supply the view transform */
    EVT_WORLD_SLOT_UNIT = 5   /* unit/group/lip-sync lists: byte offset 0x40 */
};

/* Borrow a controller owned by ctx. The pointer is read-only and expires when
 * that controller is removed or the model context is freed. */

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */

/* One child row emitted by the event-viewer track serializer. */
typedef struct EvtSerializedChild {
    u16 groupType;
    u16 start;
    u16 span;
    u16 value;
    u8 opaque08[4];
    s32 body[8];
} EvtSerializedChild;

typedef char EvtSerializedChild_size_check[
    (sizeof(EvtSerializedChild) == 0x2C) ? 1 : -1];

extern void evtWriteSerializedBytes(s32 output, void *data, s32 size);

struct SoundMixer;

typedef struct {
    s16 enabled;
    s8 fields[8]; /* Per-row signed lookup values at +0x2. */
} EvtTblEntry; /* 0xA bytes */

typedef s32 (*EvtMenuHeaderFn)();
typedef void (*EvtMenuRowFn)();

typedef struct EvtMotionBits {
    s32 group : 8;
    s32 motion : 8;
    s32 loop : 8;
    s32 hokan : 8;
} EvtMotionBits;

typedef union EvtMotionValue {
    s32 word;
    EvtMotionBits bits;
} EvtMotionValue;

/* Select and draw one light-color bank at the current panel offset. */
static inline void evtDrawLightRgbComponents(sdfDrawTag_t *list, evtViewerInfo_t *runtime,
                                             s32 selected, s32 y, s32 firstComponent) {
    if ((u32)selected >= 3U || runtime->lightEditorEditMode != 1) {
        selected = -1;
    }
    dds3PrimDebugTextBox(list,
                     (s32)(0x8740u + ((u32)runtime->lightPanelOffset << 4)),
                     y, selected, &runtime->commandMatrix[firstComponent]);
}

/* Convert the panel offset into the fixed-point X origin used by captions. */
static inline s32 evtLightCaptionX(s32 panelOffset) {
    return (s32)(0x8740u + ((u32)panelOffset << 4));
}

typedef struct EvtSelectionCache {
    u8 pad00[0x24];
    s32 selectedEntry; /* 0x24 */
} EvtSelectionCache;

typedef char EvtViewKey_size_must_be_0x38[(sizeof(EvtRuntimeChild) == 0x38) ? 1 : -1];

typedef char EvtCameraColorPayload_size_must_be_0x40[
    (sizeof(EvtCameraColorPayload) == 0x40) ? 1 : -1];

extern s32 (*evtDispatchActionByIndexData[])(s32, s32, evtViewerInfo_t *);
s32 evtDispatchActionByIndex(s32 actionIndex, s32 x, s32 y, evtViewerInfo_t *runtime);
void evtWriteSerializedBytes(s32 output, void *data, s32 size);
s32 evtAssignRuntimeChildSequenceAndCount(evtViewerInfo_t *runtime);
s32 evtIndexGroupTypeElevenChildren(evtViewerInfo_t *runtime);
s32 evtIndexGroupTypeThirteenChildren(evtViewerInfo_t *runtime);
s32 evtIndexGroupTypeFourteenChildren(evtViewerInfo_t *runtime);
s32 evtIndexGroupTypeFifteenChildren(evtViewerInfo_t *runtime);
s32 evtIndexGroupTypeTwentyThreeChildren(evtViewerInfo_t *runtime);
s32 evtIndexGroupTypeTwentySevenChildren(evtViewerInfo_t *runtime);
s32 evtIndexGroupTypeSixteenChildren(evtViewerInfo_t *runtime);
s32 evtIndexGroupTypeSeventeenChildren(evtViewerInfo_t *runtime);
s32 evtIndexGroupTypeTwentyFiveChildren(evtViewerInfo_t *runtime);

/* Fixed prefix shared by PMD directory streams. */
typedef struct EvtPmdFilePrefix {
    u8 reserved00;
    u8 reserved01;
    u16 reserved02;
    s32 byteLength;
    char tag[4];
    s32 reserved0C;
    s32 count;
    s32 kind;
    s32 reserved18;
    s32 reserved1C;
} EvtPmdFilePrefix;

typedef char EvtPmdFilePrefix_size_check[sizeof(EvtPmdFilePrefix) == 0x20 ? 1 : -1];

void evtViewerWriteChildRecords(s32 output, EvtViewerTrackFileFormat format, evtViewerInfo_t *runtime);
void evtWriteFixedSizeEntries(s32 output, evtViewerInfo_t *table);
void evtEmitGroupTypeElevenPayloads(s32 output, evtViewerInfo_t *runtime);
void evtEmitGroupTypeThirteenPayloads(s32 output, evtViewerInfo_t *runtime);
void evtEmitGroupTypeFourteenPayloads(s32 output, evtViewerInfo_t *runtime);
void evtEmitGroupTypeFifteenPayloads(s32 output, evtViewerInfo_t *runtime);
void evtEmitGroupTypeTwentyThreePayloads(s32 output, evtViewerInfo_t *runtime);
void evtEmitGroupTypeTwentySevenPayloads(s32 output, evtViewerInfo_t *runtime);
void evtEmitGroupTypeSixteenPayloads(s32 output, evtViewerInfo_t *runtime);
void evtEmitGroupTypeSeventeenPayloads(s32 output, evtViewerInfo_t *runtime);
void evtWriteGroupMetadata(s32 output, evtViewerInfo_t *runtime);
void evtEmitGroupTypeTwentyFivePayloads(s32 output, evtViewerInfo_t *runtime);

/* PM1 section 2 and section 3 use the same 0x20-byte package record. */
typedef struct EvtViewerPackageRecord {
    s32 nameIndex;
    s32 reserved04;
    s32 packageId;
    s32 partId;
    u32 offset;
    s32 length;
    s32 loadOrder;
    s32 reserved1C;
} EvtViewerPackageRecord;

/* PM1 auxiliary and texture section entries each occupy 0x10 bytes. */
typedef struct EvtViewerResourceRecord {
    s32 nameIndex;
    u32 offset;
    s32 kind;
    s32 reserved0C;
} EvtViewerResourceRecord;

/* The section-21 wire record is also the group's link subobject at +0x18. */
typedef char EvtViewerPackageRecord_size[(sizeof(EvtViewerPackageRecord) == 0x20) ? 1 : -1];

typedef char EvtViewerResourceRecord_size[(sizeof(EvtViewerResourceRecord) == 0x10) ? 1 : -1];

typedef struct { u8 bytes[0x18]; } EvtRowPayload18;

typedef struct { u8 bytes[0x20]; } EvtRowPayload20;

typedef struct { u8 bytes[0x24]; } EvtRowPayload24;

typedef struct { u8 bytes[0x28]; } EvtRowPayload28;

typedef struct { u8 bytes[0x2C]; } EvtRowPayload2C;

typedef struct { u8 bytes[0x30]; } EvtRowPayload30;

typedef struct { u8 bytes[0x40]; } EvtRowPayload40;

EvtPackLoadState *evtGetPackProcessData(EvtPackEventId eventId);

INCLUDE_ASM(const s32, "code_r1_001C4B78", evtNpcSadaPB);

INCLUDE_ASM(const s32, "code_r1_001C4B78", evtUpdateEffectViewerGroup);

s32 evtDispatchActionByIndex(s32 actionIndex, s32 x, s32 y, evtViewerInfo_t *runtime) {
    s32 mode = runtime->nowmenu;
    if (mode == 11 && actionIndex != mode) {
        return 0;
    }
    return evtDispatchActionByIndexData[actionIndex](x, y, runtime);
}

void evtWriteSerializedBytes(s32 output, void *data, s32 size) {
    sceWrite(output, data, size);
}

s32 evtAssignRuntimeChildSequenceAndCount(evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0xA) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.groupTypeAIndex = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeElevenChildren(evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0xB) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->duration = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeThirteenChildren(evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0xD) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.groupTypeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeFourteenChildren(evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0xE) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.groupTypeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeFifteenChildren(evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0xF) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.groupTypeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeTwentyThreeChildren(evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0x17) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.groupTypeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeTwentySevenChildren(evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0x1B) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.groupTypeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeSixteenChildren(evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0x10) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.groupTypeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeSeventeenChildren(evtViewerInfo_t *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0x11) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.groupTypeIndex = index++;
            }
        }
    }
    return index;
}

s32 evtIndexGroupTypeTwentyFiveChildren(evtViewerInfo_t *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0x19) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.groupTypeIndex = index++;
            }
        }
    }
    return index;
}

INCLUDE_ASM(const s32, "code_r1_001C4B78", evtViewerWriteTrackDirectory);

INCLUDE_ASM(const s32, "code_r1_001C4B78", evtWriteRuntimeHeaderValues);

/* Filter groups by file format and serialize each child body. Type-8 spans use the
 * next start or terminal range halfword, unless their body marker disables them. */
void evtViewerWriteChildRecords(s32 output, EvtViewerTrackFileFormat format, evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;

    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        EvtRuntimeChild *child = group->children;

        if (format == EVT_VIEWER_TRACK_FILE_FORMAT_PM2) {
            if (group->type == 5 || group->type == 0x13) {
                continue;
            }
        } else if (format == EVT_VIEWER_TRACK_FILE_FORMAT_PM3) {
            if (group->type != 5 && group->type != 0x13) {
                continue;
            }
        }

        for (; child != NULL; child = child->next) {
            EvtSerializedChild record;
            s32 i;
            u16 value;

            value = (u16)child->serializedValue;
            record.groupType = group->type;
            record.start = child->frame;
            record.span = child->duration;
            record.value = value;
            for (i = 0; i < 8; i++) {
                record.body[i] = child->body.words[i];
            }
            if (group->type == 8) {
                if (child->body.words[0] == 0) {
                    record.span = 0;
                } else {
                    if (child->next != NULL) {
                        record.span = child->next->frame - child->frame;
                    } else {
                        record.span = runtime->lastframe.f.end - child->frame;
                    }
                }
            }
            evtWriteSerializedBytes(output, &record, sizeof(record));
        }
    }
}

/* Emit entryTotal consecutive thirty-two-byte names; nonpositive totals emit nothing. */
void evtWriteFixedSizeEntries(s32 output, evtViewerInfo_t *table) {
    void *entry;
    s32 index;

    index = 0;
    if (0 < table->nametablecnt) {
        entry = table->nametable;
        do {
            evtWriteSerializedBytes(output, entry, 0x20);
            index = index + 1;
            entry = (void *)((s32)entry + 0x20);
        } while (index < table->nametablecnt);
    }
}

INCLUDE_ASM(const s32, "code_r1_001C4B78", evtWriteGroupHeader);

INCLUDE_ASM(const s32, "code_r1_001C4B78", evtFunc6);

void evtEmitGroupTypeElevenPayloads(s32 output, evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0xB) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x20);
            }
        }
    }
}

void evtEmitGroupTypeThirteenPayloads(s32 output, evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0xD) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x28);
            }
        }
    }
}

void evtEmitGroupTypeFourteenPayloads(s32 output, evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0xE) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x2C);
            }
        }
    }
}

void evtEmitGroupTypeFifteenPayloads(s32 output, evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0xF) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x2C);
            }
        }
    }
}

void evtEmitGroupTypeTwentyThreePayloads(s32 output, evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0x17) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x2C);
            }
        }
    }
}

void evtEmitGroupTypeTwentySevenPayloads(s32 output, evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0x1B) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x28);
            }
        }
    }
}

void evtEmitGroupTypeSixteenPayloads(s32 output, evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0x10) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x18);
            }
        }
    }
}

void evtEmitGroupTypeSeventeenPayloads(s32 output, evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0x11) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x24);
            }
        }
    }
}

void evtWriteGroupMetadata(s32 output, evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        u8 header[8];
        header[0] = *(u8 *)group;
        header[1] = group->metadataFlag;
        *(u16 *)&header[2] = (u16)group->entryHeader;
        *(u16 *)&header[4] = group->metadata.value;
        header[6] = group->metadata.option;
        header[7] = group->metadata.extra2;
        evtWriteSerializedBytes(output, header, sizeof(header));
    }
}

void evtEmitGroupTypeTwentyFivePayloads(s32 output, evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0x19) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x40);
            }
        }
    }
}

INCLUDE_ASM(const s32, "code_r1_001C4B78", evtPfsEventPmvtool);

INCLUDE_ASM(const s32, "code_r1_001C4B78", evtSetGameData2and3Sub);

INCLUDE_ASM(const s32, "code_r1_001C4B78", evtSetGameData2and3Sub2);

INCLUDE_ASM(const s32, "code_r1_001C4B78", evtSetGameData2and3Sub3);

INCLUDE_ASM(const s32, "code_r1_001C4B78", evtSetGameData2and3Sub4);

INCLUDE_ASM(const s32, "code_r1_001C4B78", evtSetGameData2and3Sub5);

INCLUDE_ASM(const s32, "code_r1_001C4B78", evtNotFoundLinkslight);

INCLUDE_ASM(const s32, "code_r1_001C4B78", evtSetGameData1);

INCLUDE_ASM(const s32, "code_r1_001C4B78", evtSetGameData2and3Sub6);

INCLUDE_ASM(const s32, "code_r1_001C4B78", evtSetGameData2and3);

INCLUDE_ASM(const s32, "code_r1_001C4B78", evtCallEventFadeOutAllBgm);
INCLUDE_SDATA(const s32, "code_r1_001C4B78", evtNpcSadaPBData);

INCLUDE_SDATA(const s32, "code_r1_001C4B78", evtNpcSadaPBData2);

INCLUDE_SDATA(const s32, "code_r1_001C4B78", evtNpcSadaPBData3);

INCLUDE_SDATA(const s32, "code_r1_001C4B78", evtNpcSadaPBData4);

INCLUDE_SDATA(const s32, "code_r1_001C4B78", evtMsgSaveOpenError);

INCLUDE_SDATA(const s32, "code_r1_001C4B78", evtPfsDevice);

