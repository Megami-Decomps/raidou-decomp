#include "common.h"
#include "raidou_alloc.h"

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "newdata.h"
#include "sdfDevCons.h"
#include "sdk_libc.h"
#include "sdk_libkernl.h"
#include "dds/btl.h"
#include "dds/dds3_task_create.h"
#include "dds/dds3_task_lifecycle.h"
#include "dds/dds3_task_update_count.h"
#include "dds/dds3_world_primary.h"
#include "dds/dds3_world_secondary.h"
#include "dds/dds3obj.h"
#include "dds/eff_event.h"
#include "dds/eff_event_setup_readiness.h"
#include "dds/evt_picture.h"
#include "dds/evt_polygon_movie_load_api.h"
#include "dds/evt_unit.h"
#include "dds/evt_viewer.h"
#include "dds/evt_world.h"
#include "dds/itf_mes_panel_mode.h"
#include "dds/itf_mes_window.h"
#include "dds/dds3_process.h"
#include "dds/mdl.h"
#include "dds/mdl_motion_controller_lookup_api.h"
#include "dds/sdf.h"
#include "dds/sdf_delayed_resource_check_api.h"
#include "dds/sdf_file_existence_api.h"
#include "dds/sdf_heap_api.h"
#include "dds/sdf_packet_list.h"
#include "dds/sdf_texture_file.h"
#include "dds/sdf_vu_matrix.h"
#include "dds/sdk_libc.h"
#include "dds/sdk_libkernl.h"
#include "dds/snd_sequence_volume_pan.h"
#include "dds/snd_track_playback_api.h"

/* The primary object returned by dds3GetActiveScene is distinct from the active
 * world's WorldInfo payload. Event lookup, lip-sync and viewer code share this chain:
 * object +0x18 -> table +0x08 -> 0x0C-byte slot entries, each with head +0x04.
 * DDS1 and DDS2 use the same observed layout. Node payloads depend on the slot. */
enum {
    EVT_WORLD_SLOT_MOVIE = 3, /* viewer movie-time lists: byte offset 0x28 */
    EVT_WORLD_SLOT_CAMERA = 4, /* linked cameras; active nodes supply the view transform */
    EVT_WORLD_SLOT_UNIT = 5   /* unit/group/lip-sync lists: byte offset 0x40 */
};

/* Borrow a controller owned by ctx. The pointer is read-only and expires when
 * that controller is removed or the model context is freed. */
const struct sdfMotionCtrlr_t *mdlGetMotionCtrlr(
    const dds3ModelHandle_t *ctx, MdlMotionSearchId searchId);

extern void *memcpy(void *, const void *, unsigned int);

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */

#include "dds/sdk_ps2.h"
#include "dds/snd_packed_track_load_status.h"
#include "dds/common.h"
#include "dds/evt_viewer_track_file_format.h"
#include "dds/sdf_vu0.h"
#include "dds/dds3_background_fade_api.h"
#include "dds/event.h"
#include "dds/evt_menu_table_entry_availability.h"
#include "dds/mnu_camp_option_index_api.h"
#include "dds/evt_reload_viewer_api.h"
#include "dds/evt_viewer_rows.h"
#include "dds/evt_float_panel_row.h"
#include "dds/evt_pad.h"
#include "dds/evt_serialized_child.h"
#include "dds/evt_event_bgm_readiness.h"
#include "dds/evt_sound_variation.h"
#include "dds/evt_sky_alpha_animation_status.h"
#include "dds/eff_object.h"
#include "dds/pcp_vu0.h"
#include "dds/ee_mmi.h"
#include "dds/sdf_sif_command.h"
#include "dds/dds3_task_runner.h"
#include "dds/evt_task.h"
#include "dds/evt_unit_object_key.h"
#include "dds/evt_unit_vector_slot.h"
#include "dds/evt_event_pack.h"
#include "dds/evt_polygon_movie.h"
#include "dds/evt_effect_scale_object.h"
#include "dds/fld.h"

extern void evtWriteSerializedBytes(s32 output, void *data, s32 size);

struct SoundMixer;

#include "dds/sdf_debug_api.h"

typedef struct {
    s16 enabled;
    s8 fields[8]; /* Per-row signed lookup values at +0x2. */
} EvtTblEntry; /* 0xA bytes */

typedef s32 (*EvtMenuHeaderFn)();
typedef void (*EvtMenuRowFn)();

#include "dds/dds3_world_selection.h"

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

extern void dds3PrimDebugTextBox(void *, s32, s32, s32, f32 *);

/* Select and draw one light-color bank at the current panel offset. */
static inline void evtDrawLightRgbComponents(sdfDrawTag_t *list, dds_evtViewerInfo_t *runtime,
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

extern s32 (*evtDispatchActionByIndexData[])(s32, s32, dds_evtViewerInfo_t *);
s32 evtDispatchActionByIndex(s32 actionIndex, s32 x, s32 y, dds_evtViewerInfo_t *runtime);
void evtWriteSerializedBytes(s32 output, void *data, s32 size);
s32 evtAssignRuntimeChildSequenceAndCount(dds_evtViewerInfo_t *runtime);
s32 evtIndexGroupTypeElevenChildren(dds_evtViewerInfo_t *runtime);
s32 evtIndexGroupTypeThirteenChildren(dds_evtViewerInfo_t *runtime);
s32 evtIndexGroupTypeFourteenChildren(dds_evtViewerInfo_t *runtime);
s32 evtIndexGroupTypeFifteenChildren(dds_evtViewerInfo_t *runtime);
s32 evtIndexGroupTypeTwentyThreeChildren(dds_evtViewerInfo_t *runtime);
s32 evtIndexGroupTypeTwentySevenChildren(dds_evtViewerInfo_t *runtime);
s32 evtIndexGroupTypeSixteenChildren(dds_evtViewerInfo_t *runtime);
s32 evtIndexGroupTypeSeventeenChildren(dds_evtViewerInfo_t *runtime);
s32 evtIndexGroupTypeTwentyFiveChildren(dds_evtViewerInfo_t *runtime);

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

void evtViewerWriteChildRecords(s32 output, EvtViewerTrackFileFormat format, dds_evtViewerInfo_t *runtime);
void evtWriteFixedSizeEntries(s32 output, dds_evtViewerInfo_t *table);
void evtEmitGroupTypeElevenPayloads(s32 output, dds_evtViewerInfo_t *runtime);
void evtEmitGroupTypeThirteenPayloads(s32 output, dds_evtViewerInfo_t *runtime);
void evtEmitGroupTypeFourteenPayloads(s32 output, dds_evtViewerInfo_t *runtime);
void evtEmitGroupTypeFifteenPayloads(s32 output, dds_evtViewerInfo_t *runtime);
void evtEmitGroupTypeTwentyThreePayloads(s32 output, dds_evtViewerInfo_t *runtime);
void evtEmitGroupTypeTwentySevenPayloads(s32 output, dds_evtViewerInfo_t *runtime);
void evtEmitGroupTypeSixteenPayloads(s32 output, dds_evtViewerInfo_t *runtime);
void evtEmitGroupTypeSeventeenPayloads(s32 output, dds_evtViewerInfo_t *runtime);
void evtWriteGroupMetadata(s32 output, dds_evtViewerInfo_t *runtime);
void evtEmitGroupTypeTwentyFivePayloads(s32 output, dds_evtViewerInfo_t *runtime);

#include "dds/dat_state.h"
#include "dds/sdf_texture_offset_list.h"

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

INCLUDE_ASM(const s32, "code_001C4B78", evtNpcSadaPB);

INCLUDE_ASM(const s32, "code_001C4B78", evtUpdateEffectViewerGroup);

s32 evtDispatchActionByIndex(s32 actionIndex, s32 x, s32 y, dds_evtViewerInfo_t *runtime) {
    s32 mode = runtime->actionMode;
    if (mode == 11 && actionIndex != mode) {
        return 0;
    }
    return evtDispatchActionByIndexData[actionIndex](x, y, runtime);
}

void evtWriteSerializedBytes(s32 output, void *data, s32 size) {
    sceWrite(output, data, size);
}

s32 evtAssignRuntimeChildSequenceAndCount(dds_evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xA) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.groupTypeAIndex = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeElevenChildren(dds_evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xB) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->duration = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeThirteenChildren(dds_evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xD) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.groupTypeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeFourteenChildren(dds_evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xE) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.groupTypeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeFifteenChildren(dds_evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xF) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.groupTypeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeTwentyThreeChildren(dds_evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x17) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.groupTypeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeTwentySevenChildren(dds_evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x1B) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.groupTypeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeSixteenChildren(dds_evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x10) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.groupTypeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeSeventeenChildren(dds_evtViewerInfo_t *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x11) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.groupTypeIndex = index++;
            }
        }
    }
    return index;
}

s32 evtIndexGroupTypeTwentyFiveChildren(dds_evtViewerInfo_t *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x19) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.groupTypeIndex = index++;
            }
        }
    }
    return index;
}

INCLUDE_ASM(const s32, "code_001C4B78", evtViewerWriteTrackDirectory);

INCLUDE_ASM(const s32, "code_001C4B78", evtWriteRuntimeHeaderValues);

/* Filter groups by file format and serialize each child body. Type-8 spans use the
 * next start or terminal range halfword, unless their body marker disables them. */
void evtViewerWriteChildRecords(s32 output, EvtViewerTrackFileFormat format, dds_evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;

    for (group = runtime->groups; group != NULL; group = group->next) {
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
                        record.span = runtime->frameRange.f.end - child->frame;
                    }
                }
            }
            evtWriteSerializedBytes(output, &record, sizeof(record));
        }
    }
}

/* Emit entryTotal consecutive thirty-two-byte names; nonpositive totals emit nothing. */
void evtWriteFixedSizeEntries(s32 output, dds_evtViewerInfo_t *table) {
    void *entry;
    s32 index;

    index = 0;
    if (0 < table->entryTotal) {
        entry = table->entryName;
        do {
            evtWriteSerializedBytes(output, entry, 0x20);
            index = index + 1;
            entry = (void *)((s32)entry + 0x20);
        } while (index < table->entryTotal);
    }
}

INCLUDE_ASM(const s32, "code_001C4B78", evtWriteGroupHeader);

INCLUDE_ASM(const s32, "code_001C4B78", evtFunc6);

void evtEmitGroupTypeElevenPayloads(s32 output, dds_evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xB) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x20);
            }
        }
    }
}

void evtEmitGroupTypeThirteenPayloads(s32 output, dds_evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xD) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x28);
            }
        }
    }
}

void evtEmitGroupTypeFourteenPayloads(s32 output, dds_evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xE) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x2C);
            }
        }
    }
}

void evtEmitGroupTypeFifteenPayloads(s32 output, dds_evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xF) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x2C);
            }
        }
    }
}

void evtEmitGroupTypeTwentyThreePayloads(s32 output, dds_evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x17) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x2C);
            }
        }
    }
}

void evtEmitGroupTypeTwentySevenPayloads(s32 output, dds_evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x1B) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x28);
            }
        }
    }
}

void evtEmitGroupTypeSixteenPayloads(s32 output, dds_evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x10) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x18);
            }
        }
    }
}

void evtEmitGroupTypeSeventeenPayloads(s32 output, dds_evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x11) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x24);
            }
        }
    }
}

void evtWriteGroupMetadata(s32 output, dds_evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
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

void evtEmitGroupTypeTwentyFivePayloads(s32 output, dds_evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x19) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x40);
            }
        }
    }
}

INCLUDE_ASM(const s32, "code_001C4B78", evtPfsEventPmvtool);

INCLUDE_ASM(const s32, "code_001C4B78", evtSetGameData2and3Sub);

INCLUDE_ASM(const s32, "code_001C4B78", evtSetGameData2and3Sub2);

INCLUDE_ASM(const s32, "code_001C4B78", evtSetGameData2and3Sub3);

INCLUDE_ASM(const s32, "code_001C4B78", evtSetGameData2and3Sub4);

INCLUDE_ASM(const s32, "code_001C4B78", evtSetGameData2and3Sub5);

INCLUDE_ASM(const s32, "code_001C4B78", evtNotFoundLinkslight);

INCLUDE_ASM(const s32, "code_001C4B78", evtSetGameData1);

INCLUDE_ASM(const s32, "code_001C4B78", evtSetGameData2and3Sub6);

INCLUDE_ASM(const s32, "code_001C4B78", evtSetGameData2and3);

INCLUDE_ASM(const s32, "code_001C4B78", evtCallEventFadeOutAllBgm);
INCLUDE_SDATA(const s32, "code_001C4B78", evtNpcSadaPBData);

INCLUDE_SDATA(const s32, "code_001C4B78", evtNpcSadaPBData2);

INCLUDE_SDATA(const s32, "code_001C4B78", evtNpcSadaPBData3);

INCLUDE_SDATA(const s32, "code_001C4B78", evtNpcSadaPBData4);

INCLUDE_SDATA(const s32, "code_001C4B78", evtMsgSaveOpenError);

INCLUDE_SDATA(const s32, "code_001C4B78", evtPfsDevice);

