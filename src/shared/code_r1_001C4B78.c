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
#include "evtPolygonMovie.h"
#include "evtStage.h"
#include "dds3WorldBasic.h"
#include "dds3SceneBasic.h"
#include "evtUnitManager.h"
#include "sdfTex.h"
#include "dds3CameraObjectBasic.h"
#include "dds3KernelDraw.h"
#include "evtSound.h"
#include "code_r1_001CA8F8.h"

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
typedef struct evtSerializedChild_t {
    u16 groupType;
    u16 start;
    u16 span;
    u16 value;
    u8 opaque08[4];
    s32 body[8];
} evtSerializedChild_t;

typedef char evtSerializedChild_size_check[
    (sizeof(evtSerializedChild_t) == 0x2C) ? 1 : -1];

extern void evtWriteSerializedBytes(s32 output, void *data, s32 size);

struct sndMixer_t;

typedef struct {
    s16 enabled;
    s8 fields[8]; /* Per-row signed lookup values at +0x2. */
} EvtTblEntry; /* 0xA bytes */

typedef s32 (*EvtMenuHeaderFn)();
typedef void (*EvtMenuRowFn)();

typedef struct evtMotionBits_t {
    s32 group : 8;
    s32 motion : 8;
    s32 loop : 8;
    s32 hokan : 8;
} evtMotionBits_t;

typedef union EvtMotionValue {
    s32 word;
    evtMotionBits_t bits;
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

typedef struct evtSelectionCache_t {
    u8 pad00[0x24];
    s32 selectedEntry; /* 0x24 */
} evtSelectionCache_t;

typedef char EvtViewKey_size_must_be_0x38[(sizeof(evtFrameInfo_t) == 0x38) ? 1 : -1];

typedef char evtCameraColorPayload_size_must_be_0x40[
    (sizeof(evtCameraColorPayload_t) == 0x40) ? 1 : -1];

s32 evtBasicListMenu(s32 x, s32 y, evtViewerInfo_t *ctx);
s32 evtCallGetPIDfromUID6(s32 x, s32 y, evtViewerInfo_t *ctx);
s32 evtCallMenuDisp(s32 x, s32 y, evtViewerInfo_t *ctx);
s32 evtCallMenuDisp2(s32 x, s32 y, evtViewerInfo_t *ctx);
s32 evtCallMenuDisp3(s32 x, s32 y, evtViewerInfo_t *ctx);
s32 evtCallMenuDisp4(s32 x, s32 y, evtViewerInfo_t *ctx);
s32 evtCallMenuDisp6(s32 x, s32 y, evtViewerInfo_t *ctx);
s32 evtCallMenuDisp7(s32 x, s32 y, evtViewerInfo_t *ctx);
s32 evtFileName2Menu(s32 x, s32 y, evtViewerInfo_t *ctx);
s32 evtFileNameMenu(s32 x, s32 y, evtViewerInfo_t *ctx);
s32 evtFloatChangeMenu(s32 x, s32 y, evtViewerInfo_t *ctx);
s32 evtFrameChangeMenu(s32 x, s32 y, evtViewerInfo_t *ctx);
s32 evtFunc4(s32 x, s32 y, evtViewerInfo_t *ctx);
s32 evtFunc5(s32 x, s32 y, evtViewerInfo_t *ctx);
s32 evtIntChangeMenu(s32 x, s32 y, evtViewerInfo_t *ctx);
s32 evtMainMenu(s32 x, s32 y, evtViewerInfo_t *ctx);
s32 evtOjbectListMenu(s32 x, s32 y, evtViewerInfo_t *ctx);
s32 evtSelectMenu(s32 x, s32 y, evtViewerInfo_t *ctx);
s32 evtUpdateEffectViewerGroup(s32 x, s32 y, evtViewerInfo_t *ctx);
s32 evtUpdateGroupListDialog(s32 x, s32 y, evtViewerInfo_t *ctx);
s32 evtUpdateGroupPropertyDialog(s32 x, s32 y, evtViewerInfo_t *ctx);
s32 evtViewerDrawGroupWindow(s32 x, s32 y, evtViewerInfo_t *ctx);
s32 mnuDrawInfoWindowB(s32 x, s32 y, evtViewerInfo_t *ctx);

s32 (*evtDispatchActionByIndexData[24])(s32 x, s32 y, evtViewerInfo_t *ctx) = {
    NULL,
    evtMainMenu, evtSelectMenu, evtFileNameMenu,
    evtOjbectListMenu, evtCallMenuDisp, evtFrameChangeMenu,
    evtIntChangeMenu, evtFloatChangeMenu, evtFileName2Menu,
    evtBasicListMenu, evtUpdateEffectViewerGroup, mnuDrawInfoWindowB,
    evtCallMenuDisp2, evtCallMenuDisp3, evtUpdateGroupPropertyDialog,
    evtCallMenuDisp4, evtUpdateGroupListDialog, evtFunc5,
    evtCallMenuDisp6, evtCallMenuDisp7, evtViewerDrawGroupWindow,
    evtCallGetPIDfromUID6, evtFunc4,
};
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
typedef struct evtViewerFileHeader_t {
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
} evtViewerFileHeader_t;

typedef char evtViewerFileHeader_size_check[sizeof(evtViewerFileHeader_t) == 0x20 ? 1 : -1];

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
typedef struct evtViewerPackageRecord_t {
    s32 nameIndex;
    s32 reserved04;
    s32 packageId;
    s32 partId;
    u32 offset;
    s32 length;
    s32 loadOrder;
    s32 reserved1C;
} evtViewerPackageRecord_t;

/* PM1 auxiliary and texture section entries each occupy 0x10 bytes. */
typedef struct evtViewerResourceRecord_t {
    s32 nameIndex;
    u32 offset;
    s32 kind;
    s32 reserved0C;
} evtViewerResourceRecord_t;

/* The section-21 wire record is also the group's link subobject at +0x18. */
typedef char evtViewerPackageRecord_size[(sizeof(evtViewerPackageRecord_t) == 0x20) ? 1 : -1];

typedef char evtViewerResourceRecord_size[(sizeof(evtViewerResourceRecord_t) == 0x10) ? 1 : -1];

typedef struct { u8 bytes[0x18]; } EvtRowPayload18;

typedef struct { u8 bytes[0x20]; } EvtRowPayload20;

typedef struct { u8 bytes[0x24]; } EvtRowPayload24;

typedef struct { u8 bytes[0x28]; } EvtRowPayload28;

typedef struct { u8 bytes[0x2C]; } EvtRowPayload2C;

typedef struct { u8 bytes[0x30]; } EvtRowPayload30;

typedef struct { u8 bytes[0x40]; } EvtRowPayload40;


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
    evtViewerObject_t *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0xA) {
            evtFrameInfo_t *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.groupTypeAIndex = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeElevenChildren(evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    evtViewerObject_t *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0xB) {
            evtFrameInfo_t *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->index = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeThirteenChildren(evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    evtViewerObject_t *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0xD) {
            evtFrameInfo_t *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.typeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeFourteenChildren(evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    evtViewerObject_t *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0xE) {
            evtFrameInfo_t *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.typeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeFifteenChildren(evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    evtViewerObject_t *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0xF) {
            evtFrameInfo_t *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.typeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeTwentyThreeChildren(evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    evtViewerObject_t *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0x17) {
            evtFrameInfo_t *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.typeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeTwentySevenChildren(evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    evtViewerObject_t *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0x1B) {
            evtFrameInfo_t *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.typeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeSixteenChildren(evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    evtViewerObject_t *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0x10) {
            evtFrameInfo_t *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.typeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeSeventeenChildren(evtViewerInfo_t *runtime) {
    s32 index = 0;
    evtViewerObject_t *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0x11) {
            evtFrameInfo_t *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.typeIndex = index++;
            }
        }
    }
    return index;
}

s32 evtIndexGroupTypeTwentyFiveChildren(evtViewerInfo_t *runtime) {
    s32 index = 0;
    evtViewerObject_t *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0x19) {
            evtFrameInfo_t *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.typeIndex = index++;
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
    evtViewerObject_t *group;

    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        evtFrameInfo_t *child = group->children;

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
            evtSerializedChild_t record;
            s32 i;
            u16 value;

            value = (u16)child->nameIndex;
            record.groupType = group->type;
            record.start = child->frame;
            record.span = child->index;
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
    evtViewerObject_t *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0xB) {
            evtFrameInfo_t *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x20);
            }
        }
    }
}

void evtEmitGroupTypeThirteenPayloads(s32 output, evtViewerInfo_t *runtime) {
    evtViewerObject_t *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0xD) {
            evtFrameInfo_t *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x28);
            }
        }
    }
}

void evtEmitGroupTypeFourteenPayloads(s32 output, evtViewerInfo_t *runtime) {
    evtViewerObject_t *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0xE) {
            evtFrameInfo_t *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x2C);
            }
        }
    }
}

void evtEmitGroupTypeFifteenPayloads(s32 output, evtViewerInfo_t *runtime) {
    evtViewerObject_t *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0xF) {
            evtFrameInfo_t *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x2C);
            }
        }
    }
}

void evtEmitGroupTypeTwentyThreePayloads(s32 output, evtViewerInfo_t *runtime) {
    evtViewerObject_t *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0x17) {
            evtFrameInfo_t *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x2C);
            }
        }
    }
}

void evtEmitGroupTypeTwentySevenPayloads(s32 output, evtViewerInfo_t *runtime) {
    evtViewerObject_t *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0x1B) {
            evtFrameInfo_t *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x28);
            }
        }
    }
}

void evtEmitGroupTypeSixteenPayloads(s32 output, evtViewerInfo_t *runtime) {
    evtViewerObject_t *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0x10) {
            evtFrameInfo_t *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x18);
            }
        }
    }
}

void evtEmitGroupTypeSeventeenPayloads(s32 output, evtViewerInfo_t *runtime) {
    evtViewerObject_t *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0x11) {
            evtFrameInfo_t *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x24);
            }
        }
    }
}

void evtWriteGroupMetadata(s32 output, evtViewerInfo_t *runtime) {
    evtViewerObject_t *group;
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
    evtViewerObject_t *group;
    for (group = runtime->firstobjinfo; group != NULL; group = group->next) {
        if (group->type == 0x19) {
            evtFrameInfo_t *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x40);
            }
        }
    }
}

INCLUDE_ASM(const s32, "code_r1_001C4B78", evtPfsEventPmvtool);

u16 evtSetGameData2and3Sub(evtPolygonMovieWork_t *wdata, s32 index) {
    if (wdata->header2->kind == 4) {
        return wdata->frameTable[index].ObjectType;
    }
    return wdata->frameTable2[index].ObjectType;
}

s16 evtSetGameData2and3Sub2(evtPolygonMovieWork_t *wdata, s32 index) {
    if (wdata->header2->kind == 4) {
        return wdata->frameTable[index].NameIndex;
    }
    return wdata->frameTable2[index].NameIndex;
}

u16 evtSetGameData2and3Sub3(evtPolygonMovieWork_t *wdata, s32 index) {
    if (wdata->header2->kind == 4) {
        return wdata->frameTable[index].Frame;
    }
    return wdata->frameTable2[index].Frame;
}

u16 evtSetGameData2and3Sub4(evtPolygonMovieWork_t *wdata, s32 index) {
    if (wdata->header2->kind == 4) {
        return wdata->frameTable[index].Length;
    }
    return wdata->frameTable2[index].Length;
}

pmdFrameData_t *evtSetGameData2and3Sub5(evtPolygonMovieWork_t *wdata, s32 index) {
    if (wdata->header2->kind == 4) {
        return (pmdFrameData_t *)&wdata->frameTable[index].data;
    }
    return &wdata->frameTable2[index].data;
}

void evtNotFoundLinkslight(evtPolygonMovieWork_t *wdata, evtViewerInfo_t *pInfo, evtGroupMetadata_t *link) {
    s32 i;
    evtViewerObject_t *obj;

    if (link->option != 3) {
        link->extra2 = 0;
    } else {
        for (i = 0; i < wdata->objectCount; i++) {
            for (obj = pInfo->firstobjinfo; obj != NULL; obj = obj->next) {
                if (strcmp(pInfo->nametable[obj->entryHeader], wdata->nameTable[link->extra2]) == 0) {
                    link->extra2 = obj->entryHeader;
                    return;
                }
            }
        }
        printf("not found linkslight obj index %s\n", wdata->nameTable[link->extra2]);
        link->option = 0;
        link->extra2 = 0;
    }
}

/* NON_MATCHING: 304 of 340 words match.  */
#ifdef NON_MATCHING
s32 evtSetGameData1(evtPolygonMovieWork_t *wdata, evtViewerInfo_t *pInfo) {
    char buf[64];
    evtWorldTable_t *world;
    evtViewerObject_t *obj;
    dds3Basic_t *basic;
    dds3Basic_t *target;
    s32 major;
    s32 minor;
    s32 serial;
    s32 priority;
    s32 i;
    s32 type;

    if (wdata->stageTable != NULL) {
        major = wdata->stageTable->MajorNum;
        minor = wdata->stageTable->MinorNum;
        evtLoadArea_FromMemory(major, minor, wdata->stageF1, wdata->stageF2, wdata->stageTB, 0);
        obj = evtAddViewerObject(0, pInfo);
        obj->entryHeader = evtAddNameTable(wdata->nameTable1[wdata->stageTable->NameIndex], pInfo);
        obj->resourceIds.resourceGroup = major;
        obj->resourceIds.resourceId = minor;
    }
    world = dds3GetControlScene()->workdata;
    for (basic = world->slots[BASIC_TYPE_CAMERAOBJECT].head; basic != NULL; basic = basic->next) {
        if (basic->name != NULL) {
            obj = evtAddViewerObject(2, pInfo);
            obj->info = basic;
            obj->entryHeader = evtAddNameTable(basic->name, pInfo);
        }
    }
    for (priority = 2; priority >= 0; priority--) {
        for (i = 0; i < wdata->unitCount; i++) {
            if (wdata->unitTable[i].Reserve1 == priority) {
                major = wdata->unitTable[i].MajorNum;
                minor = wdata->unitTable[i].MinorNum;
                mdlPackDecode(major, minor, 0x103, (u8 *)wdata->header1 + wdata->unitTable[i].DataOffset, wdata->unitTable[i].DataSize);
                serial = evtSetRegistUnit_evt(major, minor);
                obj = evtAddViewerObject(1, pInfo);
                obj->info = dds3SearchSceneBasic(dds3GetControlScene(), serial, BASIC_TYPE_UNITOBJECT);
                obj->entryHeader = evtAddNameTable(wdata->nameTable1[wdata->unitTable[i].NameIndex], pInfo);
                obj->info->name = pInfo->nametable[obj->entryHeader];
                obj->resourceIds.resourceGroup = major;
                obj->resourceIds.resourceId = minor;
                if (evtGetUnitHandle(serial)->owner->first != NULL) {
                    mdlSmoothSingleMotion(evtGetUnitHandle(serial)->owner, 0, 0, 0.0f, 0.0f);
                    evtGetUnitHandle(serial)->owner->first->speed = 0.5f;
                }
                SDF_VU0_COPYVECTOR_F32(obj->savedPosition, &obj->info->poseset->pos);
                SDF_VU0_COPYVECTOR_F32(obj->savedRotation, &obj->info->poseset->rot);
            }
        }
    }
    for (i = 0; i < wdata->effectCount; i++) {
        switch (wdata->effectTable[i].Type) {
            case 0:
                type = 3;
                break;
            case 1:
                type = 18;
                break;
            case 2:
                type = 20;
                break;
            case 3:
                type = 21;
                break;
            case 4:
                type = 26;
                break;
            default:
                continue;
        }
        obj = evtAddViewerObject(type, pInfo);
        obj->entryHeader = evtAddNameTable(wdata->nameTable1[wdata->effectTable[i].NameIndex], pInfo);
        obj->resourceData = (u8 *)wdata->header1 + wdata->effectTable[i].DataOffset;
    }
    for (i = 0; i < wdata->tmxCount; i++) {
        obj = evtAddViewerObject(24, pInfo);
        obj->entryHeader = evtAddNameTable(wdata->nameTable1[wdata->tmxTable[i].NameIndex], pInfo);
        obj->texture = sdfTexSet((TMX_HEADER_t *)((u8 *)wdata->header1 + wdata->tmxTable[i].DataOffset));
    }
    for (basic = world->slots[BASIC_TYPE_LIGHTOBJECT].head; basic != NULL; basic = basic->next) {
        if (basic->name != NULL) {
            obj = evtAddViewerObject(9, pInfo);
            obj->info = basic;
            obj->entryHeader = evtAddNameTable(basic->name, pInfo);
        }
    }
    pInfo->activecamera = NULL;
    for (basic = world->slots[BASIC_TYPE_PATH].head; basic != NULL; basic = basic->next) {
        if (basic->name == NULL) {
            continue;
        }
        for (target = world->slots[BASIC_TYPE_CAMERAOBJECT].head; target != NULL; target = target->next) {
            if (target->name != NULL) {
                sprintf(buf, "%s_MOTION", target->name);
                if (strcmp(basic->name, buf) == 0) {
                    evtPathLinkAndMake(basic, target);
                }
            }
        }
        for (target = world->slots[BASIC_TYPE_LIGHTOBJECT].head; target != NULL; target = target->next) {
            if (target->name != NULL) {
                sprintf(buf, "%s_MOTION", target->name);
                if (strcmp(basic->name, buf) == 0) {
                    evtPathLinkAndMake(basic, target);
                }
            }
        }
        for (target = world->slots[BASIC_TYPE_EFFECTOBJECT].head; target != NULL; target = target->next) {
            if (target->name != NULL) {
                sprintf(buf, "%s_MOTION", target->name);
                if (strcmp(basic->name, buf) == 0) {
                    evtPathLinkAndMake(basic, target);
                }
            }
        }
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_001C4B78", evtSetGameData1);
#endif

INCLUDE_ASM(const s32, "code_r1_001C4B78", evtSetGameData2and3Sub6);

#ifdef VERSION_RAIDOU1
s32 evtSetGameData2and3(evtPolygonMovieWork_t *wdata, evtViewerInfo_t *pInfo, s32 mode) {
    pmdCutInfo_t *cut;
    evtViewerObject_t *group;
    evtFrameInfo_t *key;
    pmdSceneLightTable1_t *light1;
    s32 loadBgm;
    s32 count;
    s32 index;
    s32 entryIndex;
    s32 i;
    s32 j;

    cut = wdata->cutInfo;
    loadBgm = FALSE;
    if (cut != NULL) {
        pInfo->totalframe = cut->TotalFrame;
        pInfo->firstframe = cut->FirstFrame;
        pInfo->lastframe.word = cut->LastFrame;
        pInfo->headerMetadata = cut->reserve;
        pInfo->nowframe = pInfo->firstframe < 0 ? 0 : pInfo->firstframe;
        if (pInfo->totalframe - 1 < pInfo->nowframe) {
            pInfo->nowframe = pInfo->totalframe - 1;
        }
    }
    for (i = 0; i < wdata->frameCount; i++) {
        if (evtSetGameData2and3Sub(wdata, i) != PMD_OBJTYPE_UNIT) {
            continue;
        }
        for (group = pInfo->firstobjinfo; group != NULL; group = group->next) {
            if (strcmp(pInfo->nametable[group->entryHeader], wdata->nameTable[evtSetGameData2and3Sub2(wdata, i)]) != 0) {
                continue;
            }
            key = evtAddFrameInfo_evtViewer(group, evtSetGameData2and3Sub3(wdata, i), pInfo);
            key->p08.b[0] = evtSetGameData2and3Sub5(wdata, i)->ub[0];
            key->p0C.i = evtSetGameData2and3Sub5(wdata, i)->w[1];
            key->nameIndex = evtAddNameTable(wdata->nameTable[evtSetGameData2and3Sub2(wdata, i)], pInfo);
            if (key->p08.sb[0] == 7) {
                index = evtSetGameData2and3Sub5(wdata, i)->h[2];
                printf("oldtable index=%d string=%s\n", index, wdata->nameTable[evtSetGameData2and3Sub5(wdata, i)->h[2]]);
                if (evtSetGameData2and3Sub5(wdata, i)->h[2] != -1) {
                    if (dds3SceneBasicSearchName(dds3GetActiveScene(), wdata->nameTable[evtSetGameData2and3Sub5(wdata, i)->h[2]]) != NULL) {
                        key->p0C.sh[0] = evtAddNameTable(wdata->nameTable[evtSetGameData2and3Sub5(wdata, i)->h[2]], pInfo);
                    } else {
                        key->p0C.sh[0] = -1;
                        printf("name not found \n");
                    }
                }
            }
            key->p10.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[4];
            key->p14.f = evtSetGameData2and3Sub5(wdata, i)->f[3];
            key->p18.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[8];
            key->p1C.i = evtSetGameData2and3Sub5(wdata, i)->w[5];
        }
    }
    for (i = 0; i < wdata->frameCount; i++) {
        if (evtSetGameData2and3Sub(wdata, i) != PMD_OBJTYPE_CAMERA) {
            continue;
        }
        for (group = pInfo->firstobjinfo; group != NULL; group = group->next) {
            if (strcmp(pInfo->nametable[group->entryHeader], wdata->nameTable[evtSetGameData2and3Sub2(wdata, i)]) != 0) {
                continue;
            }
            key = evtAddFrameInfo_evtViewer(group, evtSetGameData2and3Sub3(wdata, i), pInfo);
            key->p08.f = evtSetGameData2and3Sub5(wdata, i)->f[0];
            key->p0C.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[2];
            key->p0C.h[1] = evtSetGameData2and3Sub5(wdata, i)->uh[3];
            key->nameIndex = evtAddNameTable(wdata->nameTable[evtSetGameData2and3Sub2(wdata, i)], pInfo);
            dds3CameraObjectBasicSetFovy(group->info, key->p08.f);
        }
    }
    for (i = 0; i < wdata->frameCount; i++) {
        switch (evtSetGameData2and3Sub(wdata, i)) {
            case PMD_OBJTYPE_EFFECT:
            case PMD_OBJTYPE_BED:
            case PMD_OBJTYPE_MG1:
            case PMD_OBJTYPE_MG2:
            case PMD_OBJTYPE_EPL:
                for (group = pInfo->firstobjinfo; group != NULL; group = group->next) {
                    if (strcmp(pInfo->nametable[group->entryHeader], wdata->nameTable[evtSetGameData2and3Sub2(wdata, i)]) != 0) {
                        continue;
                    }
                    key = evtAddFrameInfo_evtViewer(group, evtSetGameData2and3Sub3(wdata, i), pInfo);
                    switch (evtSetGameData2and3Sub(wdata, i)) {
                        case PMD_OBJTYPE_EFFECT:
                        case PMD_OBJTYPE_EPL:
                            key->p08.h[1] = evtSetGameData2and3Sub5(wdata, i)->uh[1];
                            key->p0C.b[1] = evtSetGameData2and3Sub5(wdata, i)->ub[5];
                            if (evtSetGameData2and3Sub5(wdata, i)->h[3] < 0) {
                                key->p0C.sh[1] = -1;
                            } else {
                                key->p0C.sh[1] = evtAddNameTable(wdata->nameTable[evtSetGameData2and3Sub5(wdata, i)->h[3]], pInfo);
                            }
                            key->p0C.b[0] = evtSetGameData2and3Sub5(wdata, i)->ub[4];
                            key->p0C.b[1] = evtSetGameData2and3Sub5(wdata, i)->ub[8];
                            if (wdata->header2->kind < 4) {
                                key->index = 0;
                            } else {
                                key->index = evtSetGameData2and3Sub4(wdata, i);
                            }
                            key->p10.b[0] = evtSetGameData2and3Sub5(wdata, i)->ub[8];
                            if (wdata->header2->kind < 9) {
                                key->index = 0;
                            }
                            break;
                        case PMD_OBJTYPE_MG1:
                        case PMD_OBJTYPE_MG2:
                            key->p08.h[1] = evtSetGameData2and3Sub5(wdata, i)->uh[1];
                            key->index = evtSetGameData2and3Sub4(wdata, i);
                            for (j = 0; j < 4; j++) {
                                if (evtSetGameData2and3Sub5(wdata, i)->link.nameIndex[j] < 0) {
                                    key->p0C.sb[j] = -1;
                                } else {
                                    key->p0C.sb[j] = evtAddNameTable(wdata->nameTable[evtSetGameData2and3Sub5(wdata, i)->link.nameIndex[j]], pInfo);
                                }
                            }
                            key->p08.b[1] = evtSetGameData2and3Sub5(wdata, i)->ub[1];
                            key->p08.b[2] = evtSetGameData2and3Sub5(wdata, i)->ub[8];
                            key->p10.b[0] = evtSetGameData2and3Sub5(wdata, i)->ub[8];
                            if (wdata->header2->kind < 9) {
                                key->index = 0;
                            }
                            break;
                        case PMD_OBJTYPE_BED:
                            key->p08.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[0];
                            key->p0C.b[0] = evtSetGameData2and3Sub5(wdata, i)->ub[4];
                            if (evtSetGameData2and3Sub5(wdata, i)->h[1] < 0) {
                                key->p08.sh[1] = -1;
                            } else {
                                key->p08.sh[1] = evtAddNameTable(wdata->nameTable[evtSetGameData2and3Sub5(wdata, i)->h[1]], pInfo);
                            }
                            break;
                    }
                }
                break;
        }
    }
    for (i = 0; i < wdata->frameCount; i++) {
        if (evtSetGameData2and3Sub(wdata, i) != PMD_OBJTYPE_LIGHT) {
            continue;
        }
        for (group = pInfo->firstobjinfo; group != NULL; group = group->next) {
            if (strcmp(pInfo->nametable[group->entryHeader], wdata->nameTable[evtSetGameData2and3Sub2(wdata, i)]) != 0) {
                continue;
            }
            key = evtAddFrameInfo_evtViewer(group, evtSetGameData2and3Sub3(wdata, i), pInfo);
            key->index = evtSetGameData2and3Sub4(wdata, i);
            key->p08.h[1] = evtSetGameData2and3Sub5(wdata, i)->uh[1];
            if (evtSetGameData2and3Sub5(wdata, i)->h[0] < 0) {
                key->p08.sh[0] = -1;
            } else {
                key->p08.sh[0] = evtAddNameTable(wdata->nameTable[evtSetGameData2and3Sub5(wdata, i)->h[0]], pInfo);
            }
        }
    }
    for (j = 0; j < 7; j++) {
        group = evtAddViewerObject(PMD_OBJTYPE_SLIGHT, pInfo);
        group->setterId = j;
        for (i = 0; i < wdata->frameCount; i++) {
            if (evtSetGameData2and3Sub(wdata, i) != PMD_OBJTYPE_SLIGHT) {
                continue;
            }
            if (evtSetGameData2and3Sub5(wdata, i)->h[4] != j) {
                continue;
            }
            key = evtAddFrameInfo_evtViewer(group, evtSetGameData2and3Sub3(wdata, i), pInfo);
            key->p08.f = evtSetGameData2and3Sub5(wdata, i)->f[0];
            key->p0C.f = evtSetGameData2and3Sub5(wdata, i)->f[1];
            key->p10.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[4];
            key->p14.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[6];
            key->p14.h[1] = evtSetGameData2and3Sub5(wdata, i)->uh[7];
            key->p18.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[8];
            key->p18.h[1] = evtSetGameData2and3Sub5(wdata, i)->uh[9];
            key->p1C.b[0] = evtSetGameData2and3Sub5(wdata, i)->ub[20];
            key->p1C.b[1] = evtSetGameData2and3Sub5(wdata, i)->ub[21];
            key->p1C.b[2] = evtSetGameData2and3Sub5(wdata, i)->ub[22];
            if (wdata->header2->kind < 7) {
                key->index = evtSetGameData2and3Sub5(wdata, i)->uh[5];
            } else {
                key->index = evtSetGameData2and3Sub4(wdata, i);
            }
            key->nameIndex = evtSetGameData2and3Sub2(wdata, i);
            if (wdata->header2->kind < 11) {
                light1 = (pmdSceneLightTable1_t *)wdata->slightTable;
                if (wdata->header2->kind < 7) {
                    light1 += evtSetGameData2and3Sub4(wdata, i);
                } else {
                    light1 += evtSetGameData2and3Sub5(wdata, i)->h[5];
                }
                evtSetGameData2and3Sub6(key->payload, light1);
            } else {
                memcpy(key->payload, &wdata->slightTable[evtSetGameData2and3Sub5(wdata, i)->h[5]], sizeof(pmdSceneLightTable_t));
            }
            key->p14.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[6];
        }
    }
    group = evtAddViewerObject(PMD_OBJTYPE_SFOG, pInfo);
    for (i = 0; i < wdata->frameCount; i++) {
        if (evtSetGameData2and3Sub(wdata, i) == PMD_OBJTYPE_SFOG) {
            key = evtAddFrameInfo_evtViewer(group, evtSetGameData2and3Sub3(wdata, i), pInfo);
            key->p08.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[0];
            memcpy(key->payload, &wdata->sfogTable[evtSetGameData2and3Sub4(wdata, i)], sizeof(pmdSceneFogTable_t));
        }
    }
    group = evtAddViewerObject(PMD_OBJTYPE_SKY, pInfo);
    for (i = 0; i < wdata->frameCount; i++) {
        if (evtSetGameData2and3Sub(wdata, i) == PMD_OBJTYPE_SKY) {
            key = evtAddFrameInfo_evtViewer(group, evtSetGameData2and3Sub3(wdata, i), pInfo);
            key->index = evtSetGameData2and3Sub4(wdata, i);
            key->p08.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[0];
            key->p08.h[1] = evtSetGameData2and3Sub5(wdata, i)->uh[1];
            key->p0C.i = evtSetGameData2and3Sub5(wdata, i)->w[1];
            if (wdata->header2->kind == 1) {
                key->p08.sh[1] = 0;
            }
            if (wdata->header2->kind == 2) {
                key->p08.sh[1] -= 128;
                if (key->p08.sh[1] < -255) {
                    key->p08.sh[1] = -255;
                }
                if (key->p08.sh[1] > 255) {
                    key->p08.sh[1] = 255;
                }
            }
            key->p14.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[6];
        }
    }
    group = evtAddViewerObject(PMD_OBJTYPE_FB, pInfo);
    for (i = 0; i < wdata->frameCount; i++) {
        if (evtSetGameData2and3Sub(wdata, i) == PMD_OBJTYPE_FB) {
            key = evtAddFrameInfo_evtViewer(group, evtSetGameData2and3Sub3(wdata, i), pInfo);
            key->p08.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[0];
        }
    }
    group = evtAddViewerObject(PMD_OBJTYPE_BLUR2, pInfo);
    for (i = 0; i < wdata->frameCount; i++) {
        if (evtSetGameData2and3Sub(wdata, i) == PMD_OBJTYPE_BLUR2) {
            key = evtAddFrameInfo_evtViewer(group, evtSetGameData2and3Sub3(wdata, i), pInfo);
            key->p08.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[0];
            memcpy(key->payload, &wdata->blur2Table[evtSetGameData2and3Sub5(wdata, i)->h[1]], sizeof(dds3Effect_Blur_t));
        }
    }
    group = evtAddViewerObject(PMD_OBJTYPE_MBLUR, pInfo);
    for (i = 0; i < wdata->frameCount; i++) {
        if (evtSetGameData2and3Sub(wdata, i) == PMD_OBJTYPE_MBLUR) {
            key = evtAddFrameInfo_evtViewer(group, evtSetGameData2and3Sub3(wdata, i), pInfo);
            key->p08.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[0];
            memcpy(key->payload, &wdata->mblurTable[evtSetGameData2and3Sub5(wdata, i)->h[1]], sizeof(dds3Effect_PartialMultiBlurParam_t));
            key->p10.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[4];
            if (evtSetGameData2and3Sub5(wdata, i)->h[4] != 0 && evtSetGameData2and3Sub5(wdata, i)->h[4] != 1) {
                entryIndex = mnuCampFindMatchingEntryIndex(wdata, pInfo, evtSetGameData2and3Sub5(wdata, i)->h[4] - 2);
                if (entryIndex != -1) {
                    key->p10.h[0] = entryIndex + 2;
                } else {
                    key->p10.h[0] = 0;
                }
            }
        }
    }
    group = evtAddViewerObject(PMD_OBJTYPE_DBLUR, pInfo);
    for (i = 0; i < wdata->frameCount; i++) {
        if (evtSetGameData2and3Sub(wdata, i) == PMD_OBJTYPE_DBLUR) {
            key = evtAddFrameInfo_evtViewer(group, evtSetGameData2and3Sub3(wdata, i), pInfo);
            key->p08.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[0];
            memcpy(key->payload, &wdata->dblurTable[evtSetGameData2and3Sub5(wdata, i)->h[1]], sizeof(dds3Effect_DistortionBlurParam_t));
            key->p10.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[4];
            if (evtSetGameData2and3Sub5(wdata, i)->h[4] != 0 && evtSetGameData2and3Sub5(wdata, i)->h[4] != 1) {
                entryIndex = mnuCampFindMatchingEntryIndex(wdata, pInfo, evtSetGameData2and3Sub5(wdata, i)->h[4] - 2);
                if (entryIndex != -1) {
                    key->p10.h[0] = entryIndex + 2;
                } else {
                    key->p10.h[0] = 0;
                }
            }
        }
    }
    group = evtAddViewerObject(PMD_OBJTYPE_RBLUR, pInfo);
    for (i = 0; i < wdata->frameCount; i++) {
        if (evtSetGameData2and3Sub(wdata, i) == PMD_OBJTYPE_RBLUR) {
            key = evtAddFrameInfo_evtViewer(group, evtSetGameData2and3Sub3(wdata, i), pInfo);
            key->p08.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[0];
            memcpy(key->payload, &wdata->rblurTable[evtSetGameData2and3Sub5(wdata, i)->h[1]], sizeof(dds3Effect_RippleBlurParam_t));
            key->p10.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[4];
            if (evtSetGameData2and3Sub5(wdata, i)->h[4] != 0 && evtSetGameData2and3Sub5(wdata, i)->h[4] != 1) {
                entryIndex = mnuCampFindMatchingEntryIndex(wdata, pInfo, evtSetGameData2and3Sub5(wdata, i)->h[4] - 2);
                if (entryIndex != -1) {
                    key->p10.h[0] = entryIndex + 2;
                } else {
                    key->p10.h[0] = 0;
                }
            }
        }
    }
    group = evtAddViewerObject(PMD_OBJTYPE_HBLUR, pInfo);
    for (i = 0; i < wdata->frameCount; i++) {
        if (evtSetGameData2and3Sub(wdata, i) == PMD_OBJTYPE_HBLUR) {
            key = evtAddFrameInfo_evtViewer(group, evtSetGameData2and3Sub3(wdata, i), pInfo);
            key->p08.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[0];
            memcpy(key->payload, &wdata->hblurTable[evtSetGameData2and3Sub5(wdata, i)->h[1]], sizeof(pmdHeikouBlurTable_t));
        }
    }
    group = evtAddViewerObject(PMD_OBJTYPE_FILTER, pInfo);
    for (i = 0; i < wdata->frameCount; i++) {
        if (evtSetGameData2and3Sub(wdata, i) == PMD_OBJTYPE_FILTER) {
            key = evtAddFrameInfo_evtViewer(group, evtSetGameData2and3Sub3(wdata, i), pInfo);
            key->p08.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[0];
            memcpy(key->payload, &wdata->filterTable[evtSetGameData2and3Sub5(wdata, i)->h[1]], sizeof(dds3Effect_Filter_t));
            key->p14.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[6];
        }
    }
    group = evtAddViewerObject(PMD_OBJTYPE_MFILTER, pInfo);
    for (i = 0; i < wdata->frameCount; i++) {
        if (evtSetGameData2and3Sub(wdata, i) == PMD_OBJTYPE_MFILTER) {
            key = evtAddFrameInfo_evtViewer(group, evtSetGameData2and3Sub3(wdata, i), pInfo);
            key->p08.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[0];
            memcpy(key->payload, &wdata->mfilterTable[evtSetGameData2and3Sub5(wdata, i)->h[1]], sizeof(dds3Effect_PartialMultiFilterParam_t));
            key->p10.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[4];
            if (evtSetGameData2and3Sub5(wdata, i)->h[4] != 0 && evtSetGameData2and3Sub5(wdata, i)->h[4] != 1) {
                entryIndex = mnuCampFindMatchingEntryIndex(wdata, pInfo, evtSetGameData2and3Sub5(wdata, i)->h[4] - 2);
                if (entryIndex != -1) {
                    key->p10.h[0] = entryIndex + 2;
                } else {
                    key->p10.h[0] = 0;
                }
            }
            key->p14.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[6];
        }
    }
    if (wdata->MesHandle != -1 && itfGetMessageNo(wdata->MesHandle) != 0) {
        group = evtAddViewerObject(PMD_OBJTYPE_MESSAGE, pInfo);
        for (i = 0; i < wdata->frameCount; i++) {
            if (evtSetGameData2and3Sub(wdata, i) == PMD_OBJTYPE_MESSAGE) {
                key = evtAddFrameInfo_evtViewer(group, evtSetGameData2and3Sub3(wdata, i), pInfo);
                key->index = evtSetGameData2and3Sub4(wdata, i);
                key->p08.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[0];
                key->p08.h[1] = evtSetGameData2and3Sub5(wdata, i)->uh[1];
                key->p0C.b[0] = evtSetGameData2and3Sub5(wdata, i)->ub[4];
            }
        }
    }
    group = evtAddViewerObject(PMD_OBJTYPE_SE, pInfo);
    if (wdata->header2->kind < 6) {
        count = wdata->frameCount;
    } else {
        count = wdata->soundFrameCount;
    }
    for (i = 0; i < count; i++) {
        if (wdata->header2->kind < 6) {
            if (evtSetGameData2and3Sub(wdata, i) == PMD_OBJTYPE_SE) {
                key = evtAddFrameInfo_evtViewer(group, evtSetGameData2and3Sub3(wdata, i), pInfo);
                loadBgm = TRUE;
                key->p08.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[0];
                key->p08.h[1] = evtSetGameData2and3Sub5(wdata, i)->uh[1];
            }
        } else {
            if (wdata->soundFrameTable[i].ObjectType == PMD_OBJTYPE_SE) {
                key = evtAddFrameInfo_evtViewer(group, wdata->soundFrameTable[i].Frame, pInfo);
                key->p08.h[0] = wdata->soundFrameTable[i].data.uh[0];
                key->p08.h[1] = wdata->soundFrameTable[i].data.uh[1];
                key->p0C.b[0] = wdata->soundFrameTable[i].data.ub[4];
                if (key->p0C.sb[0] == 0) {
                    loadBgm = TRUE;
                }
            }
        }
    }
    group = evtAddViewerObject(PMD_OBJTYPE_BGM, pInfo);
    if (wdata->header2->kind < 6) {
        count = wdata->frameCount;
    } else {
        count = wdata->soundFrameCount;
    }
    for (i = 0; i < count; i++) {
        if (wdata->header2->kind < 6) {
            if (evtSetGameData2and3Sub(wdata, i) == PMD_OBJTYPE_BGM) {
                key = evtAddFrameInfo_evtViewer(group, evtSetGameData2and3Sub3(wdata, i), pInfo);
                loadBgm = TRUE;
                key->p08.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[0];
                key->p08.h[1] = evtSetGameData2and3Sub5(wdata, i)->uh[1];
                key->p0C.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[2];
            }
        } else {
            if (wdata->soundFrameTable[i].ObjectType == PMD_OBJTYPE_BGM) {
                key = evtAddFrameInfo_evtViewer(group, wdata->soundFrameTable[i].Frame, pInfo);
                key->p08.h[0] = wdata->soundFrameTable[i].data.uh[0];
                key->p08.h[1] = wdata->soundFrameTable[i].data.uh[1];
                key->p0C.h[0] = wdata->soundFrameTable[i].data.uh[2];
                key->p0C.b[2] = wdata->soundFrameTable[i].data.ub[6];
                if (key->p0C.sb[2] == 0) {
                    loadBgm = TRUE;
                }
            }
        }
    }
    group = evtAddViewerObject(PMD_OBJTYPE_FADE, pInfo);
    for (i = 0; i < wdata->frameCount; i++) {
        if (evtSetGameData2and3Sub(wdata, i) == PMD_OBJTYPE_FADE) {
            key = evtAddFrameInfo_evtViewer(group, evtSetGameData2and3Sub3(wdata, i), pInfo);
            key->index = evtSetGameData2and3Sub4(wdata, i);
            key->p08.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[0];
            key->p0C.b[0] = evtSetGameData2and3Sub5(wdata, i)->ub[4];
            key->p0C.b[1] = evtSetGameData2and3Sub5(wdata, i)->ub[5];
            key->p0C.b[2] = evtSetGameData2and3Sub5(wdata, i)->ub[6];
            key->p0C.b[3] = evtSetGameData2and3Sub5(wdata, i)->ub[7];
        }
    }
    group = evtAddViewerObject(PMD_OBJTYPE_QUAKE, pInfo);
    for (i = 0; i < wdata->frameCount; i++) {
        if (evtSetGameData2and3Sub(wdata, i) == PMD_OBJTYPE_QUAKE) {
            key = evtAddFrameInfo_evtViewer(group, evtSetGameData2and3Sub3(wdata, i), pInfo);
            key->index = evtSetGameData2and3Sub4(wdata, i);
            key->p08.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[0];
        }
    }
    group = evtAddViewerObject(PMD_OBJTYPE_BLUR, pInfo);
    for (i = 0; i < wdata->frameCount; i++) {
        if (evtSetGameData2and3Sub(wdata, i) == PMD_OBJTYPE_BLUR) {
            key = evtAddFrameInfo_evtViewer(group, evtSetGameData2and3Sub3(wdata, i), pInfo);
            key->p08.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[0];
            key->p08.h[1] = evtSetGameData2and3Sub5(wdata, i)->uh[1];
            key->p0C.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[2];
        }
    }
    for (i = 0; i < wdata->frameCount; i++) {
        if (evtSetGameData2and3Sub(wdata, i) != PMD_OBJTYPE_TMX) {
            continue;
        }
        for (group = pInfo->firstobjinfo; group != NULL; group = group->next) {
            if (strcmp(pInfo->nametable[group->entryHeader], wdata->nameTable[evtSetGameData2and3Sub2(wdata, i)]) != 0) {
                continue;
            }
            key = evtAddFrameInfo_evtViewer(group, evtSetGameData2and3Sub3(wdata, i), pInfo);
            key->p08.b[0] = evtSetGameData2and3Sub5(wdata, i)->ub[0];
            key->p08.b[1] = evtSetGameData2and3Sub5(wdata, i)->ub[1];
            key->p0C.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[2];
            key->p0C.h[1] = evtSetGameData2and3Sub5(wdata, i)->uh[3];
            key->p10.b[0] = evtSetGameData2and3Sub5(wdata, i)->ub[8];
            key->p10.b[1] = evtSetGameData2and3Sub5(wdata, i)->ub[9];
            key->p10.b[2] = evtSetGameData2and3Sub5(wdata, i)->ub[10];
            key->p10.b[3] = evtSetGameData2and3Sub5(wdata, i)->ub[11];
            key->p14.f = evtSetGameData2and3Sub5(wdata, i)->f[3];
            key->p18.f = evtSetGameData2and3Sub5(wdata, i)->f[4];
            key->p1C.b[0] = evtSetGameData2and3Sub5(wdata, i)->ub[20];
        }
    }
    group = evtAddViewerObject(PMD_OBJTYPE_PADACT, pInfo);
    for (i = 0; i < wdata->frameCount; i++) {
        if (evtSetGameData2and3Sub(wdata, i) == PMD_OBJTYPE_PADACT) {
            key = evtAddFrameInfo_evtViewer(group, evtSetGameData2and3Sub3(wdata, i), pInfo);
            key->index = evtSetGameData2and3Sub4(wdata, i);
            key->p08.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[0];
            key->p08.h[1] = evtSetGameData2and3Sub5(wdata, i)->uh[1];
        }
    }
    group = evtAddViewerObject(PMD_OBJTYPE_MOVIE, pInfo);
    for (i = 0; i < wdata->frameCount; i++) {
        if (evtSetGameData2and3Sub(wdata, i) == PMD_OBJTYPE_MOVIE) {
            key = evtAddFrameInfo_evtViewer(group, evtSetGameData2and3Sub3(wdata, i), pInfo);
            key->index = evtSetGameData2and3Sub4(wdata, i);
            key->p08.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[0];
            key->p0C.b[0] = evtSetGameData2and3Sub5(wdata, i)->ub[4];
        }
    }
    group = evtAddViewerObject(PMD_OBJTYPE_BISTA, pInfo);
    for (i = 0; i < wdata->frameCount; i++) {
        if (evtSetGameData2and3Sub(wdata, i) == PMD_OBJTYPE_BISTA) {
            key = evtAddFrameInfo_evtViewer(group, evtSetGameData2and3Sub3(wdata, i), pInfo);
            key->index = evtSetGameData2and3Sub4(wdata, i);
            key->p08.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[0];
        }
    }
    group = evtAddViewerObject(PMD_OBJTYPE_CUTIN, pInfo);
    for (i = 0; i < wdata->frameCount; i++) {
        if (evtSetGameData2and3Sub(wdata, i) == PMD_OBJTYPE_CUTIN) {
            key = evtAddFrameInfo_evtViewer(group, evtSetGameData2and3Sub3(wdata, i), pInfo);
            key->p08.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[0];
            key->p08.h[1] = evtSetGameData2and3Sub5(wdata, i)->uh[1];
            key->p0C.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[2];
            key->p0C.h[1] = evtSetGameData2and3Sub5(wdata, i)->uh[3];
            key->p10.b[0] = evtSetGameData2and3Sub5(wdata, i)->ub[8];
            key->p10.b[1] = evtSetGameData2and3Sub5(wdata, i)->ub[9];
        }
    }
    group = evtAddViewerObject(PMD_OBJTYPE_SCRBREAK, pInfo);
    for (i = 0; i < wdata->frameCount; i++) {
        if (evtSetGameData2and3Sub(wdata, i) == PMD_OBJTYPE_SCRBREAK) {
            key = evtAddFrameInfo_evtViewer(group, evtSetGameData2and3Sub3(wdata, i), pInfo);
            key->p08.b[0] = evtSetGameData2and3Sub5(wdata, i)->ub[0];
        }
    }
    group = evtAddViewerObject(PMD_OBJTYPE_CRSFADE, pInfo);
    for (i = 0; i < wdata->frameCount; i++) {
        if (evtSetGameData2and3Sub(wdata, i) == PMD_OBJTYPE_CRSFADE) {
            key = evtAddFrameInfo_evtViewer(group, evtSetGameData2and3Sub3(wdata, i), pInfo);
            key->index = evtSetGameData2and3Sub4(wdata, i);
        }
    }
    group = evtAddViewerObject(PMD_OBJTYPE_PLTMX, pInfo);
    for (i = 0; i < wdata->frameCount; i++) {
        if (evtSetGameData2and3Sub(wdata, i) == PMD_OBJTYPE_PLTMX) {
            key = evtAddFrameInfo_evtViewer(group, evtSetGameData2and3Sub3(wdata, i), pInfo);
            key->p08.b[0] = evtSetGameData2and3Sub5(wdata, i)->ub[0];
            key->p08.b[1] = evtSetGameData2and3Sub5(wdata, i)->ub[1];
            key->p08.h[1] = evtSetGameData2and3Sub5(wdata, i)->uh[1];
            key->p0C.h[0] = evtSetGameData2and3Sub5(wdata, i)->uh[2];
            key->p0C.h[1] = evtSetGameData2and3Sub5(wdata, i)->uh[3];
            key->p10.b[0] = evtSetGameData2and3Sub5(wdata, i)->ub[8];
            key->p10.b[1] = evtSetGameData2and3Sub5(wdata, i)->ub[9];
            key->p10.b[2] = evtSetGameData2and3Sub5(wdata, i)->ub[10];
            key->p10.b[3] = evtSetGameData2and3Sub5(wdata, i)->ub[11];
            key->p14.f = evtSetGameData2and3Sub5(wdata, i)->f[3];
            key->p18.f = evtSetGameData2and3Sub5(wdata, i)->f[4];
            key->p1C.b[0] = evtSetGameData2and3Sub5(wdata, i)->ub[20];
        }
    }
    if (wdata->objectTable != NULL) {
        for (i = 0; i < wdata->objectCount; i++) {
            if (wdata->objectTable[i].entry != -1) {
                for (group = pInfo->firstobjinfo; group != NULL; group = group->next) {
                    if (strcmp(pInfo->nametable[group->entryHeader], wdata->nameTable[wdata->objectTable[i].entry]) == 0) {
                        group->metadata.type = wdata->objectTable[i].type;
                        group->metadata.entry = wdata->objectTable[i].entry;
                        if (group->type == PMD_OBJTYPE_CAMERA) {
                            group->metadata.value = 0;
                        } else {
                            group->metadata.value = wdata->objectTable[i].value;
                        }
                        group->metadata.option = wdata->objectTable[i].option;
                        group->metadata.extra2 = wdata->objectTable[i].extra2;
                    }
                }
            } else {
                for (group = pInfo->firstobjinfo; group != NULL; group = group->next) {
                    if (group->type == wdata->objectTable[i].type && group->setterId == wdata->objectTable[i].flag) {
                        group->metadata.type = wdata->objectTable[i].type;
                        group->metadata.flag = wdata->objectTable[i].flag;
                        group->metadata.entry = wdata->objectTable[i].entry;
                        group->metadata.value = wdata->objectTable[i].value;
                        group->metadata.option = wdata->objectTable[i].option;
                        group->metadata.extra2 = wdata->objectTable[i].extra2;
                        evtNotFoundLinkslight(wdata, pInfo, &group->metadata);
                        break;
                    }
                }
            }
        }
    }
    for (i = 0; i < 10; i++) {
        DDS3_GBWK->integers[200 + i] = -1;
    }
    if (loadBgm == TRUE) {
        evtLoadEventBgm(wdata->stageTable->MajorNum);
        while (!evtCheckEventBgmLoaded(wdata->stageTable->MajorNum)) {
        }
        printf("smg file load :eventViewer\n");
    }
    if (!evtMainMenuItemSub(pInfo)) {
        dds3ChangeBistaMode(1);
        dds3BistaOut(0);
    } else {
        dds3BistaIn(0);
    }
    evtViewerDispatchFlagMode(pInfo);
    evtRunFrame(pInfo->nowframe, pInfo);
    evtViewerDispatchFlagMode(pInfo);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_001C4B78", evtSetGameData2and3);
#endif

INCLUDE_ASM(const s32, "code_r1_001C4B78", evtCallEventFadeOutAllBgm);
INCLUDE_SDATA(const s32, "code_r1_001C4B78", evtNpcSadaPBData);

INCLUDE_SDATA(const s32, "code_r1_001C4B78", evtNpcSadaPBData2);

INCLUDE_SDATA(const s32, "code_r1_001C4B78", evtNpcSadaPBData3);

INCLUDE_SDATA(const s32, "code_r1_001C4B78", evtNpcSadaPBData4);

INCLUDE_SDATA(const s32, "code_r1_001C4B78", evtMsgSaveOpenError);

INCLUDE_SDATA(const s32, "code_r1_001C4B78", evtPfsDevice);

