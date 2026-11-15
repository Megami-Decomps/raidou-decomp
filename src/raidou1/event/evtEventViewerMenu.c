#include "common.h"

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "newdata.h"
#include "sdfDevCons.h"
#include "sdk_libc.h"
#include "sdk_libkernl.h"
#include "dds/btl.h"
#include "dds/dds3_process.h"
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

/* Convert the quaternion in vf10 into the primary matrix bank vf28-vf31.
 * Providers: DDS1 game/code_002E7C20.c and DDS2 game/code_00340AC8.c. */
void ToMatrix(void);
struct sdfDrawTag_t *dds3FontReqAllocDrawTag(void);
u32 dds3GetProcessData(const dds3ProcessID_t *task);
EffEventSetupReadiness evtViewerCheckDirty(void);

/* Borrow a controller owned by ctx. The pointer is read-only and expires when
 * that controller is removed or the model context is freed. */
const struct sdfMotionCtrlr_t *mdlGetMotionCtrlr(
    const dds3ModelHandle_t *ctx, MdlMotionSearchId searchId);

extern void *memcpy(void *, const void *, unsigned int);
void sdfDrawTagLink(struct sdfDrawTag_t *list, u32 packetAddress);

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

struct SoundMixer;

#include "dds/sdf_debug_api.h"

typedef struct {
    s16 enabled;
    s8 fields[8]; /* Per-row signed lookup values at +0x2. */
} EvtTblEntry; /* 0xA bytes */

extern EvtTblEntry evtData6[];
typedef s32 (*EvtMenuHeaderFn)();
typedef void (*EvtMenuRowFn)();

/* Draw a framed debug menu: the optional header returns how many rows it used, the row callback fills the rest,
 * and blinking scroll markers appear above/below when entries precede `first` or follow the last drawn one. */
void evtDrawMenuFrame(sdfDrawTag_t *list, s32 x, s32 y, s32 width, s32 rows, s32 first, s32 total, void *callbackContext,
                   EvtMenuHeaderFn header, EvtMenuRowFn row);

extern sdfGrOt_t sdfDebugGrOt;
extern EvtPad dds3PadButtonState;
extern char evtFmtStr[];

extern s32 dds3SelectMenu(s32 padSet, s32 secondaryCount, s32 primaryCount, s32 secondaryVisible,
                                 s32 primaryVisible, s32 *secondaryScroll, s32 *primaryScroll,
                                 s32 *secondaryCursor, s32 *primaryCursor);

#include "dds/dds3_world_selection.h"

void evtViewerDrawPendingNodeRow(sdfDrawTag_t *list, s32 x, s32 y, s32 index, dds_evtViewerInfo_t *ctx);
extern void evtViewerDrawPendingNodeRow();
s32 mnuDrawInfoWindowB(s32 x, s32 y, dds_evtViewerInfo_t *work);
s32 mnuDrawCutFlagLabel(sdfDrawTag_t *target, s32 x, s32 y);
EvtMenuTableEntryAvailability evtIsMenuTableEntryEnabled(s32 *index);
s32 mnuDrawMotionChangeLabel(sdfDrawTag_t *target, s32 x, s32 y);

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

extern char evtDrawGroupListRowData2[];
extern char evtDrawGroupListRowData[];
extern char evtStrUnitAll[];
void evtDrawGroupListRow(sdfDrawTag_t *list, s32 x, s32 y, s32 index, dds_evtViewerInfo_t *ctx);
s32 evtUpdateGroupListDialog(s32 x, s32 y, dds_evtViewerInfo_t *ctx);
extern f32 sdfViewMatrix[];
extern void FromRotation(f32 angle);
void evtRotateViewerPositionFromPad(f32 *position);
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

extern char evtViewerDrawGroupRowData[];
void evtViewerDrawGroupRow(sdfDrawTag_t *list, s32 x, s32 y, s32 index, dds_evtViewerInfo_t *ctx);
s32 evtViewerDrawGroupWindow(s32 x, s32 y, dds_evtViewerInfo_t *ctx);

typedef struct EvtSelectionCache {
    u8 pad00[0x24];
    s32 selectedEntry; /* 0x24 */
} EvtSelectionCache;

dds3TaskUpdateResult_t evtPollRuntimeControlReady(dds3ProcessID_t *task);

typedef char EvtViewKey_size_must_be_0x38[(sizeof(EvtRuntimeChild) == 0x38) ? 1 : -1];

typedef char EvtCameraColorPayload_size_must_be_0x40[
    (sizeof(EvtCameraColorPayload) == 0x40) ? 1 : -1];

extern s32 (*evtDispatchActionByIndexData[])(s32, s32, dds_evtViewerInfo_t *);

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

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtMenuDisp);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFloatChangeHead);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrValueChange);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFloatChangeItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFloatChangeMenu);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtIntChangeHead);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrRrEnter);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrRdCancel);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", jtbl_00450010);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrLRValue);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtIntChangeItemTable);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtEventViewerMenuStr10);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtIntChangeItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtIntChangeMenu);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFrameChangeHead);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFrameChangeItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFrameChangeMenu);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtMainMenuItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtMainMenu);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSelectHead);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSelectItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSelectMenu);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFileNameHead);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFileNameItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFileNameMenu);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrBlackOut2);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrWhiteOut3);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtMainMenuItemTable2);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFileName2Head);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFileName2Item);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFileName2Menu);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtObjectListSub);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtObjectListItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtOjbectListMenu);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFrameListHead);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrWhiteOut22);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrWhiteIn22);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrBlackOutB2);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrBlackInB2);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrWhiteOutB2);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrWhiteInB2);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrBlackIn2);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrWhiteIn3);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrMaskOff2);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrVshadowOn2);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrMshadowOn2);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrShadowOff2);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrDispOff2);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtNORMAL);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFrameListItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCallMenuDisp);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtBasicListItem);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtBasicListMenu);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtEventPosItem);

/* Draw the pending-node selector for group types 20/21; other types return -1. */
s32 mnuDrawInfoWindowB(s32 x, s32 y, dds_evtViewerInfo_t *work) {
    sdfDrawTag_t *packets = dds3FontReqAllocDrawTag();
    s32 rows;
    switch (work->frameGroup->type) {
    case 20:
        rows = 2;
        break;
    case 21:
        rows = 4;
        break;
    default:
        return -1;
    }
    evtDrawMenuFrame(packets, x, y, 0x1C, rows, 0, rows, work, 0, evtViewerDrawPendingNodeRow);
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, packets);
    if (work->actionMode != 0xC) {
        return 0;
    }
    return dds3SelectMenu(0, 1, rows, 1, rows, 0, 0, 0, &work->inputB);
}

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtMESSAGEMENUMESMAX);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrBranchRes0);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrBranchRes1);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrBranchRes2);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrBranchRes3);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrBranchRes4);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrBranchRes5);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrBranchRes6);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrBranchRes7);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrBranchRes8);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrBranchRes9);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtNONEMESDATATable);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtNONEMESDATA);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCallMenuDisp2);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrUDSelect2);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", jtbl_004508C0);

s32 mnuDrawCutFlagLabel(sdfDrawTag_t *target, s32 x, s32 y) {
    sdfDrawTagLink(target, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, "CUTFLAG MENU"));
    return 2;
}

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtCMPVALUETable);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCMPVALUE);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCallMenuDisp3);

EvtMenuTableEntryAvailability evtIsMenuTableEntryEnabled(s32 *index) {
    return evtData6[*index].enabled != EVT_MENU_TABLE_ENTRY_DISABLED;
}

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtLeaf10);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtSOUTAI);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrSlightType);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrUnitAll);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtUpdateGroupPropertyDialog);

s32 mnuDrawMotionChangeLabel(sdfDrawTag_t *target, s32 x, s32 y) {
    sdfDrawTagLink(target, (u32)sdfDevPrintf(x, y, 0xFEFFFF, 0, "MOTION CHANGE MENU"));
    return 2;
}

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtGROUPMAX);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCallMenuDisp4);

void evtDrawGroupListRow(sdfDrawTag_t *list, s32 x, s32 y, s32 index, dds_evtViewerInfo_t *ctx) {
    EvtRuntimeGroup *group;
    s32 color;
    s32 n;

    color = 4;
    if (ctx->groupFirst + ctx->groupCursor != index) {
        color = 0;
    }
    if (index == 0) {
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, evtDrawGroupListRowData2));
    } else if (index == 1) {
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, evtDrawGroupListRowData));
        return;
    } else if (index == 2) {
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, evtStrUnitAll));
        return;
    }
    n = 3;
    for (group = ctx->groups; group != NULL; group = group->next) {
        if (group->type == 1) {
            if (n == index) {
                sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, evtFmtStr, ((char *)group->info->name)));
                return;
            }
            n++;
        }
    }
}

s32 evtUpdateGroupListDialog(s32 x, s32 y, dds_evtViewerInfo_t *ctx) {
    EvtRuntimeGroup *group;
    s32 count = 0;
    sdfDrawTag_t *packets;
    s32 shown = 15;

    for (group = ctx->groups; group != NULL; group = group->next) {
        if (group->type == 1) {
            count++;
        }
    }
    count += 3;
    packets = dds3FontReqAllocDrawTag();
    evtDrawMenuFrame(packets, x, y, 20, 15, ctx->groupFirst, count, ctx, NULL, evtDrawGroupListRow);
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, packets);
    if (ctx->actionMode != 17) {
        return 0;
    }
    if (count < shown) {
        shown = count;
    }
    return dds3SelectMenu(0, 1, count, 1, shown, 0, &ctx->groupFirst, 0, &ctx->groupCursor);
}

void evtLeaf11(void) {
}

/* vu0 routine: apply the selected camera-axis rotations to a position. */
void evtRotateViewerPositionFromPad(f32 *position)
{
    if (dds3PadButtonState.incOne != 0) {
        SDF_VU0_LOADVECTOR_MTXCOL(sdfViewMatrix + 1);
        FromRotation(2.0f * 3.14159265f / 180.0f);
        ToMatrix();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_APPLYMATRIX_XYZ1();
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR(position);
    } else if (dds3PadButtonState.decOne != 0) {
        SDF_VU0_LOADVECTOR_MTXCOL(sdfViewMatrix + 1);
        FromRotation(-2.0f * 3.14159265f / 180.0f);
        ToMatrix();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_APPLYMATRIX_XYZ1();
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR(position);
    }
    if (dds3PadButtonState.incTen != 0) {
        SDF_VU0_LOADVECTOR_MTXCOL(sdfViewMatrix);
        FromRotation(-2.0f * 3.14159265f / 180.0f);
        ToMatrix();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_APPLYMATRIX_XYZ1();
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR(position);
    } else if (dds3PadButtonState.decTen != 0) {
        SDF_VU0_LOADVECTOR_MTXCOL(sdfViewMatrix);
        FromRotation(2.0f * 3.14159265f / 180.0f);
        ToMatrix();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_APPLYMATRIX_XYZ1();
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR(position);
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCOPYCOLOR);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCallMenuDisp5);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtFunc5);

void evtLeaf12(void) {
}

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCallMenuDisp6);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCallDevPrintf);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCallMenuDisp7);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtLeaf13);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtCallMenuDisp5Table);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrFaceDiv);

void evtViewerDrawGroupRow(sdfDrawTag_t *list, s32 x, s32 y, s32 index, dds_evtViewerInfo_t *ctx) {
    EvtRuntimeGroup *group;
    s32 color;
    s32 n;

    color = 4;
    if (ctx->groupFirst + ctx->groupCursor != index) {
        color = 0;
    }
    if (index == 0) {
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, evtViewerDrawGroupRowData));
        return;
    }
    if (index == 1) {
        sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, "DEFFAULT"));
        return;
    }
    n = 2;
    for (group = ctx->groups; group != NULL; group = group->next) {
        if (group->type == 0x18) {
            if (n == index) {
                sdfDrawTagLink(list, (u32)sdfDevPrintf(x, y, 0xFEFFFF, color, evtFmtStr, ctx->entryName[group->entryHeader]));
                return;
            }
            n++;
        }
    }
}

s32 evtViewerDrawGroupWindow(s32 x, s32 y, dds_evtViewerInfo_t *ctx) {
    EvtRuntimeGroup *group;
    s32 count = 0;
    sdfDrawTag_t *packets;
    s32 shown = 15;

    for (group = ctx->groups; group != NULL; group = group->next) {
        if (group->type == 0x18) {
            count++;
        }
    }
    count += 2;
    packets = dds3FontReqAllocDrawTag();
    evtDrawMenuFrame(packets, x, y, 20, 15, ctx->groupFirst, count, ctx, NULL, evtViewerDrawGroupRow);
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, packets);
    if (ctx->actionMode != 21) {
        return 0;
    }
    if (count < shown) {
        shown = count;
    }
    return dds3SelectMenu(0, 1, count, 1, shown, 0, &ctx->groupFirst, 0, &ctx->groupCursor);
}

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrAlphaYzahyo);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtViewerDrawShadowConfigRow);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCallGetPIDfromUID6);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCallGetProcessData19);

/* Poll solid-rectangle setup until it clears runtime control and finishes. */
dds3TaskUpdateResult_t evtPollRuntimeControlReady(dds3ProcessID_t *task) {
    dds_evtViewerInfo_t *runtime;

    runtime = (dds_evtViewerInfo_t *)dds3GetProcessData(task);
    if (evtViewerCheckDirty() == EFF_EVENT_SETUP_NOT_READY) {
        runtime->controlState = 0;
        return DDS3_TASK_UPDATE_FINISH_CALLBACK;
    }
    return DDS3_TASK_UPDATE_RETAIN_CALLBACK;
}

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtBLEND);

INCLUDE_ASM(const s32, "event/evtEventViewerMenu", evtCallConsPanelTagLinkZ);
INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrX);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrY);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrRight);

INCLUDE_RODATA(const s32, "event/evtEventViewerMenu", evtStrQuick);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtMenuDispData);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtScrollUpMark);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtScrollDownMark);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtFmtIndentInt);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtGROUPMAXData);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtMainMenuItemDataEntry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtMainMenuItemData);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtMainMenuItemTable2Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtMainMenuItemData2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtFmtStr);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtFmtSpaceStr);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtFmtNameExt);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtFmtChar);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtLabelEvent);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtFmtEventNo);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtLabelCut);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtFileName2ItemData);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtFileName2ItemData2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtLabelName);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtFrameListHeadData2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData3Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData3Entry2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData3Entry3);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData3Entry4);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData3Entry5);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData3Entry6);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData3Entry7);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData4Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData4Entry2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData4Entry3);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData4Entry4);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData4Entry5);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData5Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData5Entry2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData6Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData6Entry2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData7Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData7Entry2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData8Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData8Entry2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData9Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData9Entry2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData12);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData10Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData13);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData11Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData14);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData15);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtStrEllipsis);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData16);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtStrOn);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData17);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData18);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData19);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData20);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData21);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData22);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData23);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData24);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData25);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", nbDebugMotionNameOut);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData26);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtFmtFloat1);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData27);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData28);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtFmtName11);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData29);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData30);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData31);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData32);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData33);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData34);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData35);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData36);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNORMALData37);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtFmtPlayerNo);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtFmtIndentStr);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtNONEMESDATATableEntry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtData17);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCMPVALUETableEntry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtSOUTAIDataEntry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtSOUTAIDataEntry2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtDrawGroupListRowData);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtDrawGroupListRowData2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtData18);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtSOUTAIData3);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtSOUTAIData4);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtGROUPMAXData2Entry2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtGROUPMAXData2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtGROUPMAXData3);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtGROUPMAXData4);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtData19);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCOPYCOLORData2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCallMenuDisp5TableEntry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCallMenuDisp5TableEntry2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCallMenuDisp5TableEntry3);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCallMenuDisp5TableEntry4);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCallMenuDisp5TableEntry5);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCallMenuDisp5TableEntry6);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCallMenuDisp5Data2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCallMenuDisp6Data2Entry);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCallMenuDisp6Data2Entry2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCallMenuDisp6Data2Entry3);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCallMenuDisp6Data3);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCallMenuDisp6Data4);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCallMenuDisp6Data5);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtViewerDrawGroupRowData);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtViewerDrawShadowConfigRowDataEntry2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtViewerDrawShadowConfigRowDataEntry3);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtData20);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtData21);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtBLENDData);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtBLENDData2);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtBLENDData3);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtBLENDData4);

INCLUDE_SDATA(const s32, "event/evtEventViewerMenu", evtCallConsPanelTagLinkZData);

