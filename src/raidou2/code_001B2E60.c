#include "common.h"

/* Declarations from dds-decomp src/dds1/model/mdlEffect.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "fileManager.h"

#include "model.h"

#include "sdfDevCons.h"

#include "sdk_libc.h"

#include "dds/bill_object_api.h"

#include "dds/dds3_admin_control_api.h"

#include "dds/dds3_task_add_child.h"

#include "dds/dds3_task_create.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/eff_node.h"

#include "dds/eff_node_descriptor.h"

#include "dds/file_request_api.h"

#include "dds/fld_integer_editor_api.h"

#include "dds/kwln.h"

#include "dds/mdl.h"

#include "dds/mdl_blur_api.h"

#include "dds/mdl_motion_api.h"

#include "dds/sdf_bordered_rectangle_packet.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_chunk.h"

#include "dds/sdf_draw.h"

#include "dds/sdf_file_existence_api.h"

#include "dds/sdf_gs_header.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_line_pair_packet_api.h"

#include "dds/sdf_motion.h"

#include "dds/sdf_motion_loop.h"

#include "dds/sdf_packet_builders.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_projection.h"

#include "dds/sdf_request.h"

#include "dds/sdf_sif_command.h"

#include "dds/sdf_texture_draw_packet.h"

#include "dds/sdf_texture_registry.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/sdk_libc.h"

/* Multiply the quaternions in vf10 and vf11, writing the product to vf10. */
void Mult(void);

/* Convert the quaternion in vf10 into the primary matrix bank vf28-vf31.
 * Providers: DDS1 game/code_002E7C20.c and DDS2 game/code_00340AC8.c. */
void ToMatrix(void);

/* Set bit 1 in the active administration task's flags; that task must exist. */
void dds3AdminiExitSequence(void);

struct billHandle_t *dds3BillboardCopy(const struct billHandle_t *source);

/* The selected kind interprets the opaque payload word. */
struct billHandle_t *dds3BillboardCreate(s32 kind, u32 data);

void dds3BillboardDelete(struct billHandle_t *obj);

void dds3BillboardSetRGBA(struct billHandle_t *billboard, u32 parameter);

void dds3BillboardSetScale(struct billHandle_t *billboard, f32 x, f32 y);

void dds3DebugIntEdit(void *valueAddress, FldIntegerStorageType storageType, s64 min, s64 max,
                      s64 step, s64 bigStep);

/* The pad-state bytes are borrowed read-only input; valueAddress is updated. */
void dds3DebugIntEditEx(void *valueAddress, FldIntegerStorageType storageType, s64 min, s64 max,
                     s64 step, s64 bigStep, const s8 *padState);

EffNode *dds3EffectCopy(const EffNode *source);

void dds3EffectDelete(EffNode *node);

struct EffNode *dds3EffectLoad(EffNodeDescriptor *descriptor);

void dds3EffectSetScale(EffNode *node, f32 factor);

struct sdfDrawTag_t *dds3FontReqAllocDrawTag(void);

u32 dds3GetProcessData(const dds3ProcessID_t *task);

dds3TaskKillRequestResult_t dds3KillProcess(dds3ProcessID_t *task, s32 delayTicks);

dds3TaskKillRequestResult_t dds3KillProcessName(const char *name, s32 delayTicks);

/*
 * Append child to parent's child list, detaching it from any previous parent.
 * This hierarchy controls teardown: parent finalization calls
 * dds3KillProcess(child, 0) for linked children before its destroy callback.
 * It does not change scheduler queue membership.
 */
void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *child);

u64 *dds3PrimFrameRect(s32 x, s32 y, s32 depth, s32 width, s32 height, u32 colorA, u32 colorB);

extern SdfProjectionRecord dds3ScreenParam;

/* Return a borrowed scheduler-owned handle from the starting, active, or
 * destroy-pending queue. The handle remains valid through its destroy callback
 * and is freed when finalization completes; return NULL when no name matches. */
dds3ProcessID_t *dds3SearchProcessName(const char *name);

/*
 * Allocate a task and place it in scheduler queues. Returns NULL on allocation
 * failure. On success, the scheduler owns the task handle and frees it after
 * its destroy callback; callers may borrow the handle only until finalization.
 */
dds3ProcessID_t *dds3StartProcess(const char *name, u32 priority, s32 startDelay,
                         s32 destroyDelay, dds3ProcessFunc_t update,
                         dds3ProcessShutdownFunc_t destroy, u32 userValue);

/* Return the address of the loaded resource payload as a byte pointer. */
u8 *fileGetLoadPtr(const struct FileRequest *request);

/* Return the signed byte count recorded by the file transfer request. */
s32 fileGetLoadSize(const struct FileRequest *request);

/* Return the payload allocation descriptor. For normal file loads, freeing the
 * request wrapper does not free this allocation; its resource owner must keep
 * or release it separately. */
struct sdfMemHandle_t *fileGetMemHandle(const struct FileRequest *request);

/* Free a completed request wrapper; return one while it remains pending. */
FileRequestCleanupResult fileHandleFree(struct FileRequest *request);

/* fileSave also copies requestName into request-owned storage. It separately
 * borrows sourceData for the asynchronous write, which must stay readable and
 * unchanged until CLOSED. */
struct FileRequest *fileSave(const char *requestName, const void *sourceData, s32 sourceByteCount);

void fileSync(struct FileRequest *request);

/* Model-context lifetime, per-frame update, and broadcast accessors. */
/* Destroy one context. If it leaves its resource-owning group empty, group
 * teardown also releases that group's registered assets. */
void mdlFree(dds3ModelHandle_t *ctx);

u16 mdlGetMajorNum(const dds3ModelHandle_t *ctx);

u16 mdlGetMinorNum(const dds3ModelHandle_t *ctx);

/* Context-level motion APIs; per-sdfMotionCtrlr_t lifecycle methods are in sdf_motion.h. */
MdlMotionIndexResult mdlGetMotionNum(const dds3ModelHandle_t *ctx, MdlMotionSearchId searchId);

/* The referenced halfword is promoted to a word-sized SDK result. */
s32 mdlGetNumOfCut(const dds3ModelHandle_t *ctx, s32 searchId);

/* VU helpers transfer vectors through vf10 and basis rows through vf28-vf30. */
void mdlGetPosition(dds3ModelHandle_t *ctx);

void mdlGetRotate(dds3ModelHandle_t *ctx);

MdlMotionPresence mdlHasMotionEx(const dds3ModelHandle_t *ctx, MdlMotionSearchId searchId);

void mdlLink(dds3ModelHandle_t *ctx, struct sdfGrOt_t **surfaces);

/* Allocate a distinct caller-owned context linked to the registered group.
 * Contexts share the group node and assets; release each context with mdlFree. */
dds3ModelHandle_t *mdlLoad(u32 group, u32 id);

void mdlLoopMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex);

void mdlSetBlur(dds3ModelHandle_t *ctx, MdlBlurSpawnInterval spawnInterval,
                MdlBlurSnapshotCapacity snapshotCapacity,
                u32 endColor, u32 startColor);

void mdlSetPosition(dds3ModelHandle_t *ctx);

void mdlSetRotate(dds3ModelHandle_t *ctx);

void mdlSmoothLoopMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex, f32 blendLeadFrames,
                          f32 blendDurationFrames);

void mdlSmoothSingleMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex, f32 blendLeadFrames,
                       f32 blendDurationFrames);

s32 memcmp(const void *a, const void *b, u32 size);

void *memset(void *dst, s32 value, u32 size);

SdfTextureDrawPacket *sdf2DLinkTexPacket(sdfDrawTag_t *packetList, sdfTexHandle_t *texture, s32 context);

void *sdfChipAllocZeroDirect(s32 size);

void sdfChipFreeDirect(void *memory);

void *sdfDevPrintfEx(SifCommand *packet, const char *format, ...);

void sdfDevSetPrintCB(SifCommand *packet, s32 source, s32 end, s32 argument, s32 index);

struct SdfPacket *sdfDrawRotatedModel(const f32 vertices[][4], const u32 *colors,
                                                   s32 vertexCount, u32 primitiveFlags);

void sdfDrawTagLink(struct sdfDrawTag_t *list, u32 packetAddress);

SdfFileExistenceResult sdfFileCheckExist(const char *path);

/* DMA/VIF transfer header and packed A+D GIF tag, excluding register payload. */
void sdfGrSetRegsPacketHeader(SdfGsPacketHeader *header, s32 payloadQwords);

/* Operations on the complete buffered-request owner, distinct from draw packets. */
void sdfItemArrayFree(DevRequest *request);

DevRequest *sdfItemArrayNew(s32 count, s32 stride, s32 growStep);

void sdfMemFreeDirect(struct sdfMemHandle_t *allocation);

SdfChunkHeader *sdfModelGetExtraChunk(const struct dds_sdfModelRootNode_t *model, s32 tag);

u32 sdfModelGetMarkPosCount(const struct dds_sdfModelRootNode_t *model);

void sdfModelMakeMarkPosMatrix(struct dds_sdfModelRootNode_t *model, SdfMapPositionRecord *record);

SdfMapPositionRecord *sdfModelSearchMarkPosData(struct dds_sdfModelRootNode_t *model, s32 recordId);

sdfModelNode_t *sdfModelSearchNode(dds_sdfModelRootNode_t *model, s32 nodeIdOrIndex);

void sdfMotionChange(struct sdfMotionCtrlr_t *motion, s32 motionIndex, SdfMotionLoopMode loopMode);

void sdfMotionSeek(struct sdfMotionCtrlr_t *motion, f32 frame);

void sdfPktSetCmd(SifCommand *packet, s32 index);

void sdfPrimLinkF3(
    sdfDrawTag_t *list, s32 color, s32 primitive, s32 x0, s32 y0, s32 x1,
    s32 y1, s32 x2, s32 y2, s32 depth, s32 (*allocatePacket)(s32));

void sdfPrimLinkLine(
    sdfDrawTag_t *list, s32 color, s32 primitive, s32 left, s32 top, s32 right,
    s32 bottom, s32 depth, s32 (*allocatePacket)(s32));

void sdfPrimLinkSprite(
    sdfDrawTag_t *list, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0,
    s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 depth,
    s32 (*allocatePacket)(s32));

struct sdfTexHandle_t *sdfTexSearch(s32 resourceKey);

u32 strlen(const char *text);

#include "dds/sdk_ps2.h"

#include "dds/sdf_line_pair_packet_api.h"

#include "dds/dds3_path.h"

#include "dds/dds3_admin_control_api.h"

#include "dds/sdf_request.h"

#include "dds/kwln.h"

#include "dds/dds3_task_create.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_packet_builders.h"

#include "dds/sdf_bordered_rectangle_packet.h"

#include "dds/common.h"

#include "dds/sdf_file_existence_api.h"

#include "dds/dds3_object_flags.h"

#include "dds/sdf_vu0.h"

#include "dds/mdl_option_label.h"

#include "dds/eff_inner_float_api.h"

#include "dds/mdl_light_editor_state.h"

#include "dds/dds3_task_add_child.h"

#include "dds/mdl_motion_api.h"

#include "dds/sdf_motion_loop.h"

#include "dds/sdf_texture_draw_packet.h"

#include "dds/file_request_api.h"

#include "dds/bill_object_api.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf.h"

#include "dds/sdf_texture_registry.h"

#include "dds/sdf_projection.h"

#include "dds/mdl.h"

#include "dds/mdl_blur_api.h"

#include "dds/mdl_part_info_api.h"

#include "dds/mdl_p2a_resource_api.h"

#include "dds/mdl_part_entry.h"

#include "dds/mdl_resource_entries.h"

#include "dds/mdl_track_poly_record.h"

#include "dds/mdl_mark_params.h"

#include "dds/fld_integer_editor_api.h"

#include "dds/mdl_resource_table.h"

#include "dds/mdl_grouped_parts_api.h"

#include "dds/mdl_flag_api.h"

#include "dds/mdl_world_object_source_vector_api.h"

#include "dds/dds3_world_counter.h"

#include "dds/evt_unit_object_key.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/pcp_vu0.h"

#include "dds/ee_mmi.h"

#include "dds/dat_state.h"

#include "dds/sdf_draw.h"

#include "dds/sdf_chunk.h"

#include "dds/sdf_pac_state.h"

#include "dds/eff.h"

#include "dds/eff_node_descriptor.h"

#include "dds/eff_node.h"

#include "dds/sdf_sif_command.h"

#include "dds/eff_transform.h"

#include "dds/evt_world_source_transform.h"

#include "dds/evt_script_event_id.h"

#include "dds/file.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/mdl_object_stream.h"

#include "dds/dds3obj.h"

#include "dds/sdf_gs_header.h"

#include "dds/mdl_option_state.h"

#include "dds/dds3_world_secondary.h"

#define MDL_VIEWER_RESOURCE_SLOTS 12

#define MDL_RECORD_LIST_HEADER_BYTES 8

#define MDL_RECORD_END_KIND 0xFFFF

#define MDL_MARK_COLOR_COUNT 4

#define MDL_COLOR_CHANNEL_COUNT 4

#define MDL_COLOR_BYTE_MASK 0xFF

#define MDL_COLOR_BYTE_BITS 8

#define MDL_MARK_LAST_FIELD 0x16

#define MDL_MARK_COLOR_FIRST_FIELD 7

#define MDL_PAD_REPEAT_FLAG 2

#define MDL_PART_SLOT_BYTES 0x10

#define MDL_PART_VALUE_SIZE_THRESHOLD 0x11

#define MDL_MAP_POSITION_TAG 0x534F504D

typedef enum MdlViewerRecordKind {
    MDL_VIEWER_RECORD_KIND_BILLBOARD_PART = 1,
    MDL_VIEWER_RECORD_KIND_EFFECT_PART = 2,
    MDL_VIEWER_RECORD_KIND_TRACK_POLY = 3,
    MDL_VIEWER_RECORD_KIND_STREAM = 4,
    MDL_VIEWER_RECORD_KIND_OBJECT_PART = 5
} MdlViewerRecordKind;

/* Viewer-wide state for the model viewer task (DDS1 game/code_00218B48 and
 * DDS2 game/code_00233660 share this layout field for field). Fields that are
 * only written by a defaults initialiser and never read in either game are
 * left as unkNN on purpose: there is no read to earn a role from. */
typedef struct MdlViewState {
    dds3ProcessID_t *unk00;
    dds3ProcessID_t *viewerTask;
    s8 exitCleanupStarted; /* 0x08: one-shot guard for viewer task/resource teardown. */
    s8 selectionHighlightPhase;
    s8 taskPhase; /* 0x0A: one-based index into D_00367A40 */
    s8 unk0B;
    s8 unitStepMode;  /* 0x0C: toggled by the step button (D_003D24510[0x23]) */
    s8 unitStepSign;  /* 0x0D: +1/-1, derived from the input keys at 0x24/0x25 */
    s8 unk0E;
    s8 assetDisplayMode; /* 0x0F: toggles asset labels and cursor paging. */
    s8 yawStepMode; /* 0x10: one yaw step per left/right button press */
    u8 pad11[3];
    s16 selectedTaskIndex;
    s16 resourceCount;
    s16 resourceGroup; /* 0x18: group passed to the resource loader */
    s16 resourceId;    /* 0x1A: ID passed to the resource loader */
    s16 selectedResourceGroup; /* 0x1C: resource group under the selection cursor. */
    s16 selectedResourceId; /* 0x1E: resource ID under the selection cursor. */
    s16 selectionCursor; /* 0x20: selects one of five controls in the asset panel. */
    s16 activeEntryId;    /* 0x22: assigned from selectedEntryId when adding */
    s16 selectedEntryId;  /* 0x24: key passed to the mdlAddEntry* family;
                           signed in DDS1, unsigned in DDS2. */
    s16 selectedNodeId;   /* 0x26: checked by mdlHasMotionEx before adding */
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 entryHeight;      /* 0x2E: low bound offered to mdlAddEntry* */
    s16 entryWidth;       /* 0x30: high bound, limited to entryHeight when drawing */
    u8 pad32[2];
    s16 labelIndexA;      /* 0x34: indexes D_00367B10 */
    s16 labelIndexB;      /* 0x36: indexes D_00367B18 */
    s16 scrollPage;  /* 0x38: page stepped by mdlUpdateViewerCursorWithPageStep */
    s16 nodeCursor;  /* 0x3A: selection within the loaded node count */
    s16 settingsCursor; /* 0x3C: selected row among the ten viewer settings. */
    s16 settingsPage;
    s16 editorMode; /* 0x40: 0 selects records, 1 edits a mark record */
    s16 selectedRecordIndex; /* 0x42: index across the shared and active-entry record lists */
    s16 effectListScroll; /* 0x44: first visible effect row */
    s16 markFieldCursor; /* 0x46: selected row in the mark parameter editor */
    s16 workspaceSetupPhase; /* 0x48: capture workspace setup advances through phases 0, 1, 2. */
    s16 captureScale; /* 0x4A: model capture scale setting */
    s16 captureDivX;  /* 0x4C: model capture horizontal division setting */
    s16 captureDivY;  /* 0x4E: model capture vertical division setting */
    s16 unk50;
    s16 unk52;
    f32 viewerScale;      /* 0x54: viewer zoom, in hundredths */
    u8 pad58[8];
    u128 unk60[3]; /* 0x60: quadword slots; their alignment makes the state quadword aligned */
    dds3ModelHandle_t *resources[MDL_VIEWER_RESOURCE_SLOTS]; /* 0x90 */
    sdfDrawTag_t *packetList; /* 0xC0: drawing packet destination */
} MdlViewState;

typedef struct MdlPadState {
    u8 pad00;
    s8 confirm;
    u8 pad02;
    s8 cancel;
    s8 stepDownA;  /* 0x04 */
    s8 stepUpA;    /* 0x05 */
    s8 stepDownB;  /* 0x06 */
    s8 stepUpB;    /* 0x07 */
} MdlPadState;

typedef struct MdlCtrlState {
    MdlPadState *pad;
    u8 unk04;
    u8 blinkTick;
    s16 selection;
    sdfDrawTag_t *packetList;
} MdlCtrlState;

extern MdlViewState mdlViewerState;

static inline s8 mdlGetViewerDisplayMode(MdlViewState *state) {
    return state->assetDisplayMode;
}

extern MdlCtrlState mdlDebugScaleWork;

extern s8 D_003D7A60[];

typedef struct MdlViewerTaskDef {
    const char *name;
    dds3ProcessFunc_t update;
    s32 data;
} MdlViewerTaskDef;

extern MdlViewerTaskDef D_00367A40[];

typedef struct MdlSystemTask {
    const char *name;
    s32 arg;
} MdlSystemTask;

extern MdlSystemTask D_00367AE0[3];

extern char D_003ABF78[];

extern char D_003ABF88[];

extern char D_003BBC80[];

extern char D_003ABC98[];

extern char D_003ABCB0[];

extern char D_003ABCC8[];

extern char D_003ABCD8[];

extern char D_003BBC28[];

extern s32 mdlViewer(dds3ProcessID_t *);

extern s32 mdlViewerEnd(dds3ProcessID_t *);

void dds3PrimDebugTextBoxRGB(void *, s32, s32, s32, u32, s32);

s32 mdlUpdateViewerCursor(s16 *, s32);

void mdlUpdateViewerSelectedModelFromPad(void);

void mdlDrawViewerSelectionLabel(void);

void sdfIpuSetOutputTex(sdfIpuDecodeCb_t *, const SdfStreamParams *, u8 *, s32, sdfTexHandle_t *);

void sdfIpuFree(sdfIpuDecodeCb_t *);

void mdlCleanupViewerTasksAndResources(void);

extern u32 D_003BA8EC;

extern void dds3DebugGraphSetEnabled(s8 mode);

extern s32 dds3DebugColorEditEx(u32 *color, s32 channel, s8 *pad);

extern s8 dds3PadButtonState[64];

extern u8 sdfHddValid;

s32 mdlPibGetChunkItemCount(MdlRecord *listHeader);

/* Read the model record's payload word without advancing its relative link. */
u32 mdlPibGetVersion(MdlRecord *record);

/* The part-list count is the low half of the packed parameter word. */
u16 mdlPibGetChunkCount(MdlRecord *record);

/* Follow the relative links in a resource's part-info table to find a list key. */
MdlRecord *mdlPibGetChunk(dds3ModelHandle_t *resource, MdlPartInfoListKey partInfoListKey);

/* The first eight bytes belong to the enclosing list, not its first record. */
MdlRecord *mdlPibGetFirstChunkItem(MdlRecord *listHeader);

/* A terminal kind returns NULL; the link is a byte offset, not a pointer. */
MdlRecord *mdlPibGetNextChunkItem(MdlRecord *currentRecord);

/* Count relative-offset records until the 0xffff sentinel. */
s32 mdlPibGetChunkItemCount(MdlRecord *listHeader);

s32 mdlPibCheckItemType(MdlRecord *record, s32 kind);

u16 mdlPibGetEffectCount(const MdlRecord *record);

/* Read the counted group record's number of following entity-slot packets. */
u16 mdlPibGetMotionCount(const MdlRecord *record);

extern char D_003ABA80[], D_003ABA90[], D_003ABAA0[], D_003ABAB0[], D_003ABAC0[], D_003ABAD0[];

extern char D_003BBB80[], D_003BBB88[];

void mdlPibDrawEffectInfo(sdfDrawTag_t *packetList, s32 x, s32 y, s32 depth, s32 textStyle, MdlRecord *record);

extern char *D_003679B8[];

extern u8 D_003679B0[];

extern u16 D_00367980[];

extern char D_003BBBB0[];

extern char D_003BBBB8[];

/* Draw the scalar marker fields, then four colors with four byte channels each; negative selection hides the cursor. */
void mdlPibDrawTrackInfo(sdfDrawTag_t *packetList, s32 x, s32 y, s32 depth, EffMarkParams *params, s32 selectedField);

/* Edit the marker parameters with the pad: rows 0-6 are the scalar fields, 7-22 the color bytes. */
void mdlPibEditTrackInfo(EffMarkParams *params, s16 *fieldCursor);

#define MDL_PART_BILLBOARD 0

#define MDL_PART_EFFECT 1

#define MDL_PART_OBJECT 3

void mdlDeleteEffectResrc_IPU(MdlObj *obj);

void mdlStartEffectResrc_IPU(MdlObj *obj, sdfTexHandle_t *resource, const SdfStreamParams *params);

/* Prepend a cleared, typed resource item to the owner's list. */
dds3ModelEffectCtrlr_t *mdlCreateEffectCtrlr(
    dds3ModelHandle_t *owner, MdlResourceKind resourceKind, MdlMotionSlotIndex motionSlotIndex);

void *mdlEffectDup_P2A(MdlPartEntry *entry);

void *mdlEffectDup_D3P(MdlPartEntry *entry);

/* Resolve a native fixed-size slot when its table exists and index is below the upper bound; no lower-bound check. */
MdlPartEntry *mdlGetEffectResrc(dds3ModelHandle_t *resource, s32 slotIndex);

/* Bind each consecutive record ID to a newly created part when the chunk contains it. */
void mdlCreateEffect_P2A_D3P(
    dds3ModelHandle_t *owner, const MdlRecord *partRecord, MdlMotionSlotIndex motionSlotIndex,
    MdlResourceKind resourceKind, void *(*createPart)(MdlPartEntry *));

/* Convert the effect record to native track parameters and retain its work pointer. */
void mdlCreateEffect_TRACK(dds3ModelHandle_t *owner,
                           const MdlTrackPolyRecordView *record,
                           MdlMotionSlotIndex motionSlotIndex);

void mdlCreateEffect_BLUR(dds3ModelHandle_t *owner, const MdlRecord *record);

/* Claim an unused object only after its data resource resolves, retaining record flags and deferred-init parameters. */
void mdlCreateEffect_IPU(dds3ModelHandle_t *owner, const MdlRecord *entryRecord,
                         MdlMotionSlotIndex motionSlotIndex);

/* Initialize once the owner's motion reaches the stored threshold; retain native float-to-signed-to-unsigned conversion. */
void mdlDrawEffect_IPU(dds3ModelEffectCtrlr_t *item);

/* Dispatch the five record kinds, retaining the game's native return behavior. */
void mdlCreateEffect(dds3ModelHandle_t *owner, const MdlRecord *record,
                      MdlMotionSlotIndex motionSlotIndex);

/* Copy values from the group-owned record list; context resources retain no record pointers. */
void mdlGenEffect(dds3ModelHandle_t *owner, MdlPartInfoListKey partInfoListKey,
                  MdlMotionSlotIndex motionSlotIndex);

/* Release billboard, effect, or tracked-poly handles when applicable, then always free the list item. */
void mdlFreeEffect(dds3ModelEffectCtrlr_t *item);

/* Release all items for this slot without losing the link across consecutive matches. */
void mdlStopEffect(dds3ModelHandle_t *owner, MdlMotionSlotIndex motionSlotIndex);

extern u8 aViewEye[];

/* vu0 routine: positionOut = p + normalize(p - aViewEye) * scale, p = transformed node position. */
void mdlEffect_P2A_D3P_GetPos(dds_sdfModelRootNode_t *model, dds3ModelEffectCtrlr_t *anchorRecord, f32 *positionOut);

extern void dds3BillboardDraw(billHandle_t *billboard);

/* Update anchored billboard/effect positions, tracked polygons, or deferred object initialization by type. */
void mdlLinkEffect(dds3ModelHandle_t *owner, dds3ModelEffectCtrlr_t *anchorRecord);

/* Set a billboard/effect frame; other resource-item kinds have no frame dispatch. */
void mdlSetEffectColorSub(dds3ModelHandle_t *owner, dds3ModelEffectCtrlr_t *item, s32 frame);

/* Apply amount to billboard/effect scale; other resource-item kinds are ignored. */
void mdlSetEffectScaleSub(dds3ModelHandle_t *owner, dds3ModelEffectCtrlr_t *item, float amount);

u64 *mdlBuildViewerRectanglePacket(s32 x, s32 y, s32 depth, s32 width, s32 height);

void mdlAppendViewerRectToDrawList(s32 x, s32 y, s32 depth, s32 width, s32 height, s32 unused);

extern s8 dds3PadBitTable[];

/* Return 1 after a cursor update: fresh presses wrap, while repeated presses stop at the ends. */
s32 mdlUpdateViewerCursor(s16 *cursor, s32 entryCount);

/* Apply single-entry or optional page steps using the native wrap/clamp and input-precedence rules. */
s32 mdlUpdateViewerCursorWithPageStep(s16 *cursor, s32 entryCount, s32 pageStep);

void mdlLoadViewerResourceAndResetCursors(void);

typedef struct {
    f32 x, y, z, w;
} __attribute__((aligned(16))) MdlEyeVec;

extern MdlEyeVec D_003D7B20;

extern MdlEyeVec D_003D7B30;

extern u128 D_003D7B40;

extern MdlEyeVec D_00367A10;

extern MdlEyeVec D_00367A20;

extern u128 D_00367A30;

extern s16 D_003D7A84[];

void mdlResetViewerBasisVectors(void);

/* Rotate the viewer resource list in place without moving its allocation. */
void mdlRotateViewResourcesRight(void);

/* Move the first resource to the end of the active list without changing its allocation. */
void mdlRotateViewList(void);

extern const char *D_003679D0[];

extern char D_003BBC20[];

/* Draw the MODE header and current or saved viewer mode label. */
void mdlDrawViewerModeLabel(void);

/* Draw the phase-zero option list in two columns, with cursor/blink styling. */
void mdlDrawViewerOptionLabels(void);

void mdlDrawViewerModelAndMotionSummary(void);

void mdlAddViewEntryFlagged(void);

void mdlAddPlainViewerEntryForSelectedNode(void);

extern u32 mdlGetNumOfMajor(void);

extern u32 mdlGetNumOfMinor(s32 tableIndex);

extern void mdlMakeHddFileList(void);

extern void mdlFreeViewResources(void);

void mdlHandleViewerAssetSelection(void);

extern char D_003BBC30[];

extern char D_003BBC38[];

extern char D_003BBC40[];

extern char D_003BBC48[];

extern char D_003BBC50[];

extern char D_003BBC58[];

extern char D_003BBC60[];

extern char D_003BBC68[];

extern void mdlGetFilebase(s32 selectionListIndex, s32 selectionIndex, char *destination, s32 capacity);

/* Draw the pending resource details and the current selection in each model node. */
void mdlDrawViewerAssetSelection(void);

u32 mdlRunViewerAssetSelectionTask(void);

extern void FromRotation_B(f32 angle);

extern f32 sdfViewMatrix[];

/* vu0 routine: move the viewer camera or orbit it about the look-at point. */
void mdlUpdateViewerCameraFromPad(void);

extern const char *D_00367B10[];

void mdlDrawViewerIndexedLabelOverlay(void);

u32 mdlRunViewerIndexedLabelTask(void);

extern f32 sdfUnitXVector[4];

extern f32 sdf3DUpVector[4];

extern f32 D_003983A0[4];

/* vu0 routine: edit the viewer transform with translation or quaternion steps. */
void mdlUpdateViewerSelectedModelFromPad(void);

extern const char *D_00367B18[];

void mdlDrawViewerSelectionLabel(void);

s32 mdlRunViewerSelectionLabelTask(void);

/* Change the viewer scale in hundredths, with larger steps at larger values. */
void mdlAdjustViewerScale(void);

/* Draw the motion progress bar: timeline frame, playhead marker and "[time/length]" label, then the zoom value. */
void mdlDrawViewerMotionTimeline(void);

u32 mdlUpdateViewerScaleTask(void);

extern s8 D_00398629[];

extern u8 D_003BD888;

extern s8 sdf3DScreenAdjust;

extern f32 sdf3DScreenAdjustScaleX;

extern f32 sdf3DScreenAdjustScaleY;

extern f32 sdf3DScreenAdjustOffsetX;

extern f32 sdf3DScreenAdjustOffsetY;

/* Final workspace publication: see docs/idioms.md, "Publishing viewer workspace". */
void mdlUpdateViewerWorkspaceSetup(void);

/* Run workspace setup and its separate hook; keep the task result zero. */
u32 mdlUpdateViewerWorkspaceSetupTask(void);

void mdlApplyViewerResourceMenuAction(void);

extern const char *D_00367B20[];

/* Draw resource actions with independent eligibility and selection styles. */
void mdlDrawViewerResourceMenu(void);

u32 mdlRunViewerResourceMenuTask(void);

/* Loaded-resource count chain, identical to the DDS2 model viewer. */
typedef struct MdlCountNode {
    u8 pad00[4];
    s16 count;
    u8 pad06[6];
    sdfTexHandle_t **textures;
} MdlCountNode;

void mdlHandleViewerNodeCursorInput(void);

extern char D_003ABDA8[];

extern char D_003BBC88[];

void mdlDrawViewerTexturePreview(void);

u32 mdlUpdateViewerNodeCursorTask(void);

s32 mdlIsDebugTimeGraph(void);

extern void mdlSaveViewerPresentationConfig(void);

/* Debug menu: page 0 selects an action, pages 1-4 edit the color channels and value steps. */
void mdlUpdateViewerSettingsInput(void);

extern const s32 D_00367B38[];

extern const char *D_00367B60[];

extern const char *D_00367B70[];

extern char D_003BBC90[];

extern char D_003BBC98[];

extern char D_003BBCA0[];

extern char D_003BBCA8[];

void mdlDrawViewerSettings(void);

u32 mdlRunViewerSettingsTask(void);

/* Sum context-wide and selected-entry list counts, retaining both lookups. */
s32 mdlCountActiveRecords(void);

/* Count the context-wide list before the selected-entry list for cursor lookup. */
MdlRecord *mdlGetSelectedRecord(void);

extern MdlRecord *mdlGetSelectedRecord(void);

void mdlUpdateViewerMarkEditorInput(void);

extern const char D_003ABEB0[];

extern const char D_003BBCB0[];

void mdlDrawViewerEffectRecordList(void);

u32 mdlRunViewerEffectEditorTask(void);

s32 mdlRequestViewerExitOnce(void);

extern sdfGrOt_t D_00325048;

extern f32 D_00367B80[][4];

extern u32 D_00367BE0[];

/* Submit the intermediate packet with an identity basis in phases 2 and 3. */
void mdlSubmitViewerIntermediateDrawPacket(void);

extern f32 D_00367C00[][4];

extern u32 D_00367C40[];

extern s32 D_003BBCB8;

extern void sdfDumpHexLine(s32 packet, s32 arg);

/* Submit the selected resource in phase 3; keep the native bitwise input/once-flag test. */
void mdlSubmitViewerResourceDrawPacket(void);

void mdlViewerTaskDestroy(void);

void mdlRestartViewerPhaseTask(void);

extern s8 D_00398636[];

extern s8 D_00324532[];

extern u8 aViewTarget[];

extern u8 aViewDown[];

extern u32 sdfGetLodChunkValue(dds_sdfModelRootNode_t *);

extern s32 sdfGrAllocPacketBuff(s32 size);

s32 mdlViewer(dds3ProcessID_t *task);

extern f32 D_00367C50[][4];

extern u32 D_00367CB0[];

/* Walk native 64-byte map-position records and submit their visualization in one packet list. */
void mdlDrawMapPositionRecords(dds3ModelHandle_t *resource);

extern sdfGrOt_t D_003255A8;

extern sdfGrOt_t *kwlnSceneSurfaceSets[13][4];

s32 mdlViewerEnd(dds3ProcessID_t *task);

extern char D_00367AF8[];

extern char D_003BBCC0[];

extern char D_003BBCC8[];

extern char D_003BBCD0[];

extern char D_003BBCD8[];

extern KwlnDrawVectorParams kwlnDrawVector;

void mdlLoadViewerPresentationConfig(void);

extern const char D_003ABF18[0x60] __attribute__((aligned(8)));

void mdlSaveViewerPresentationConfig(void);

extern s32 fclEncycFreeEntries(const char *);

extern MdlEyeVec D_00367CD0;

extern MdlEyeVec D_003246C0;

/* Load viewer config/resources, initialize view state and recreate the viewer tasks. */
void mdlInitializeViewerTasksAndResources(void);

/* Clear each occupied resource slot before destroying its context; retain the native scan form. */
void mdlFreeViewResources(void);

extern void fclEncycAllocEntry(const char *name, s32, s32, s32);

void mdlCleanupViewerTasksAndResources(void);

extern MdlLightEditorState D_00367D30;

extern MdlOptionLabel D_00367CE0[4];

extern u16 D_00367D10[16];

extern s8 D_00324520[16];

extern sdf3DLight_t *D_00324770[3];

extern f32 kwlnDefaultColorVector[4];

extern char D_003BBCF8[];

extern char D_003BBD00[];

extern sdfGrOt_t D_00325708;

extern void dds3DebugFloatEditEx(f32 *value, u8 *padState, f32 min, f32 max, f32 step, f32 bigStep);

extern void dds3PrimDebugTextBox(void *packetList, s32 x, s32 y, s32 selected, f32 *values);

/* Direction-preview packet builder consumes the vector loaded in vf10. */
extern void *dds3PrimLine3D(s32, s32, s32, s32);

extern void FromRotation(f32 angle);

/* vu0 routine: rotate the light editor vector by two degrees about pad-selected camera axes. */
void mdlSetDebugMatrix(MdlLightEditorState *context, f32 *position);

/* Update/draw the task-selected light and ambient editor; return zero. */
s32 mdlDebugPrintEffectScale(dds3ProcessID_t *task);

void mdlDebugTextBox(s32 first, s32 second, s32 color, s32 variant);

typedef struct MdlValueEdit {
    s32 y;
    f32 *target;   /* 0x04 */
    s32 min;       /* 0x08 */
    s32 max;       /* 0x0C */
    const char *label;  /* 0x10 */
    const char *format; /* 0x14 */
} MdlValueEdit;

extern MdlValueEdit D_00367D58[];

/* Step the number being edited in table slot `index`: coarse steps (A) and fine steps (B),
   held-button repeat, wrapping from one end of [min, max] to the other on a fresh press. */
void mdlGetDebugEntry(s32 valueEditIndex);

extern MdlOptionLabel D_00367D48;

extern char D_003BBD38[];

extern s8 D_00398638[16];

/* Edit the draw-vector target: four numeric rows and three packed-colour channels. */
s32 mdlDebugPrintEffectScale2(dds3ProcessID_t *task);

extern MdlOptionState D_003D7B60;

extern MdlOptionLabel D_00367DB8[4];

extern u16 D_00367DE8[4] __attribute__((section(".data")));

extern f32 sdfFaceDivSoften;

extern f32 sdfFaceDivBias;

/* The selector occupies byte zero of its aligned small-data allocation. */
extern u8 corePktSoftenBias __attribute__((section(".sdata")));

extern const char D_003BBD58[];

extern const char D_003BBD60[];

extern const char D_003BBD68[];

extern const char D_003BBD70[];

extern const char D_003BBD78[];

/* Update/draw the FACE-DIV soften, bias and divide controls; return zero. */
s32 soften_bias_Process(dds3ProcessID_t *task);
/* End of declarations from src/dds1/model/mdlEffect.c */

u64 *mdlBuildViewerRectanglePacket(s32 x, s32 y, s32 depth, s32 width, s32 height) {
    return dds3PrimFrameRect(x, y, depth, width, height, 0x30000000, 0x60404040);
}

void mdlAppendViewerRectToDrawList(s32 x, s32 y, s32 depth, s32 width, s32 height, s32 unused) {
    sdfDrawTag_t *packet;

    packet = mdlViewerState.packetList;
    sdfDrawTagLink(packet, (u32)(mdlBuildViewerRectanglePacket(x, y, depth, width, height)));
}

/* Return 1 after a cursor update: fresh presses wrap, while repeated presses stop at the ends. */
s32 mdlUpdateViewerCursor(s16 *cursor, s32 entryCount) {
    s32 lastIndex = entryCount - 1;
    s32 selectedIndex = *cursor;
    s32 changed = 0;
    if (dds3PadBitTable[5] < 0) {
        if (selectedIndex < lastIndex) {
            selectedIndex += 1;
            changed = 1;
        } else {
            selectedIndex = 0;
            changed = 1;
        }
    } else if (((u8)dds3PadBitTable[5] & MDL_PAD_REPEAT_FLAG) != 0) {
        if (selectedIndex < lastIndex) {
            selectedIndex += 1;
            changed = 1;
        }
    } else if (dds3PadBitTable[4] < 0) {
        if (selectedIndex > 0) {
            selectedIndex -= 1;
            changed = 1;
        } else {
            selectedIndex = lastIndex;
            changed = 1;
        }
    } else if ((((u8)dds3PadBitTable[4] & MDL_PAD_REPEAT_FLAG) != 0) && (selectedIndex > 0)) {
        selectedIndex -= 1;
        changed = 1;
    }
    if (changed != 0) {
        *cursor = selectedIndex;
        mdlViewerState.selectionHighlightPhase = 0;
        return 1;
    }
    return 0;
}

/* Apply single-entry or optional page steps using the native wrap/clamp and input-precedence rules. */
s32 mdlUpdateViewerCursorWithPageStep(s16 *cursor, s32 entryCount, s32 pageStep) {
    s32 lastIndex = entryCount - 1;
    s32 selectedIndex = *cursor;
    s32 changed = 0;
    if (dds3PadBitTable[7] < 0) {
        if (selectedIndex < lastIndex) {
            selectedIndex += 1;
        } else {
            selectedIndex = 0;
        }
        changed = 1;
    } else if (((u8)dds3PadBitTable[7] & MDL_PAD_REPEAT_FLAG) != 0) {
        if (selectedIndex < lastIndex) {
            selectedIndex += 1;
            changed = 1;
        }
    } else if (dds3PadBitTable[6] < 0) {
        if (selectedIndex > 0) {
            selectedIndex -= 1;
        } else {
            selectedIndex = lastIndex;
        }
        changed = 1;
    } else if (((u8)dds3PadBitTable[6] & MDL_PAD_REPEAT_FLAG) != 0) {
        if (selectedIndex > 0) {
            selectedIndex -= 1;
            changed = 1;
        }
    } else if (pageStep != 0) {
        if (dds3PadBitTable[11] < 0) {
            if (selectedIndex < lastIndex) {
                selectedIndex += pageStep;
                if (selectedIndex > lastIndex) {
                    selectedIndex = lastIndex;
                }
            } else {
                selectedIndex = 0;
            }
            changed = 1;
        } else if (((u8)dds3PadBitTable[11] & MDL_PAD_REPEAT_FLAG) != 0) {
            if (selectedIndex < lastIndex) {
                selectedIndex += pageStep;
                if (selectedIndex > lastIndex) {
                    selectedIndex = lastIndex;
                }
                changed = 1;
            }
        } else if (dds3PadBitTable[10] < 0) {
            if (selectedIndex > 0) {
                selectedIndex -= pageStep;
                if (selectedIndex < 0) {
                    selectedIndex = 0;
                }
            } else {
                selectedIndex = lastIndex;
            }
            changed = 1;
        } else if ((((u8)dds3PadBitTable[10] & MDL_PAD_REPEAT_FLAG) != 0) && (selectedIndex > 0)) {
            selectedIndex -= pageStep;
            if (selectedIndex < 0) {
                selectedIndex = 0;
            }
            changed = 1;
        }
    }
    if (changed != 0) {
        *cursor = selectedIndex;
        mdlViewerState.selectionHighlightPhase = 0;
        return 1;
    }
    return 0;
}

void mdlLoadViewerResourceAndResetCursors(void) {
    dds3ModelHandle_t *resource = mdlLoad(mdlViewerState.resourceGroup, mdlViewerState.resourceId);

    mdlViewerState.resources[0] = resource;
    mdlViewerState.activeEntryId = 0;
    mdlViewerState.selectedEntryId = 0;
    mdlViewerState.selectedNodeId = 0;
    mdlViewerState.unk28 = 0;
    mdlViewerState.unk2A = 0;
    mdlViewerState.unk2C = 0;
    if (resource->first != NULL) {
        mdlLoopMotion(resource, 0, 0);
    }
    mdlViewerState.nodeCursor = 0;
}

void mdlResetViewerBasisVectors(void) {
    SDF_VU0_COPYVECTOR(&D_003D7B20, &D_00367A10);
    SDF_VU0_COPYVECTOR(&D_003D7B30, &D_00367A20);
    SDF_VU0_COPYVECTOR(&D_003D7B40, &D_00367A30);
    D_003D7A84[0] = 0;
}

/* Rotate the viewer resource list in place without moving its allocation. */
void mdlRotateViewResourcesRight(void) {
    s32 resourceIndex = mdlViewerState.resourceCount - 1;
    dds3ModelHandle_t *lastResource = mdlViewerState.resources[resourceIndex];

    if (resourceIndex > 0) {
        do {
            mdlViewerState.resources[resourceIndex] = mdlViewerState.resources[resourceIndex - 1];
            resourceIndex -= 1;
        } while (resourceIndex > 0);
    }
    mdlViewerState.resources[0] = lastResource;
}

/* Move the first resource to the end of the active list without changing its allocation. */
void mdlRotateViewList(void) {
    s32 resourceIndex;
    s32 resourceCount = mdlViewerState.resourceCount;
    dds3ModelHandle_t *firstResource = mdlViewerState.resources[0];

    for (resourceIndex = 0; resourceIndex < resourceCount - 1; resourceIndex++) {
        mdlViewerState.resources[resourceIndex] = mdlViewerState.resources[resourceIndex + 1];
    }
    mdlViewerState.resources[resourceIndex] = firstResource;
}

/* Draw the MODE header and current or saved viewer mode label. */
void mdlDrawViewerModeLabel(void) {
    SifCommand packet;
    s32 phase;

    mdlAppendViewerRectToDrawList(0x7150, 0x7948, 0xFF0080, 0xBA0, 0x90, 0);
    sdfDevSetPrintCB(&packet, 0x7180, 0x7960, 0xFF0080, 0);
    sdfDrawTagLink(mdlViewerState.packetList, (u32)(sdfDevPrintfEx(&packet, D_003BBC20)));
    phase = mdlViewerState.taskPhase;
    if (phase == 0) {
        phase = mdlViewerState.unk0B - 1;
    } else {
        phase = phase - 1;
    }
    sdfDrawTagLink(mdlViewerState.packetList, (u32)(sdfDevPrintfEx(&packet, D_003679D0[phase])));
}

/* Draw the phase-zero option list in two columns, with cursor/blink styling. */
void mdlDrawViewerOptionLabels(void) {
    const char **label;
    u32 i;

    if (mdlViewerState.taskPhase != 0) {
        return;
    }

    mdlAppendViewerRectToDrawList(0x7690, 0x7A08, 0xFF0080, 0xC60, 0x2D0, 0);
    label = D_003679D0;
    i = 0;
    do {
        s32 style = (i == mdlViewerState.selectedTaskIndex && mdlViewerState.selectionHighlightPhase < 0x14) ? 4 : 0;
        s32 xOffset;
        u32 row;
        u32 packet;

        if (i >= 7) {
            xOffset = 0x600;
            row = i - 7;
        } else {
            xOffset = 0;
            row = i;
        }
        packet = (u32)sdfDevPrintf(0x76C0 + xOffset,
                                                   0x7A20 + row * 0x60,
                                                   0xFF0080, style, *label);
        i++;
        sdfDrawTagLink(mdlViewerState.packetList, packet);
        label++;
    } while (i != 13);
}

void mdlDrawViewerModelAndMotionSummary(void) {
    SifCommand packet;
    const char *format;
    s32 displayMode;
    s32 nodeCount;
    void *formatted;

    mdlAppendViewerRectToDrawList(0x7E10, 0x8608, 0xFF0080, 0xEA0, 0x90, 0);
    sdfDevSetPrintCB(&packet, 0x7E40, 0x8620, 0xFF0080, 0);

    if ((displayMode = mdlGetViewerDisplayMode(&mdlViewerState)) == 0) {
        format = D_003ABC98;
    } else {
        format = D_003ABCB0;
    }
    sdfDrawTagLink(mdlViewerState.packetList, (u32)(sdfDevPrintfEx(&packet, format, mdlViewerState.resourceGroup, mdlViewerState.resourceId)));

    nodeCount = mdlGetNumOfCut(mdlViewerState.resources[0], 0);
    if (nodeCount == 0) {
        formatted = sdfDevPrintfEx(&packet, D_003BBC28);
    } else {
        if (mdlViewerState.assetDisplayMode == 0) {
            format = D_003ABCC8;
        } else {
            format = D_003ABCD8;
        }
        formatted = sdfDevPrintfEx(&packet, format,
                                       mdlGetMotionNum(mdlViewerState.resources[0], 0), nodeCount - 1);
    }
    sdfDrawTagLink(mdlViewerState.packetList, (u32)(formatted));
}

void mdlAddViewEntryFlagged(void) {
    f32 low;
    f32 high;
    f32 first;

    if (mdlHasMotionEx(mdlViewerState.resources[0], mdlViewerState.selectedNodeId) != MDL_MOTION_ABSENT) {
        low = mdlViewerState.entryHeight;
        high = mdlViewerState.entryWidth;
        first = high;
        if (low < high) {
            first = low;
        }
        mdlViewerState.activeEntryId = mdlViewerState.selectedEntryId;
        mdlSmoothLoopMotion(mdlViewerState.resources[0], mdlViewerState.selectedNodeId, mdlViewerState.selectedEntryId, first, low);
    }
}

void mdlAddPlainViewerEntryForSelectedNode(void) {
    f32 low;
    f32 high;
    f32 first;

    if (mdlHasMotionEx(mdlViewerState.resources[0], mdlViewerState.selectedNodeId) != MDL_MOTION_ABSENT) {
        low = mdlViewerState.entryHeight;
        high = mdlViewerState.entryWidth;
        first = high;
        if (low < high) {
            first = low;
        }
        mdlViewerState.activeEntryId = mdlViewerState.selectedEntryId;
        mdlSmoothSingleMotion(mdlViewerState.resources[0], mdlViewerState.selectedNodeId, mdlViewerState.selectedEntryId, first, low);
    }
}

INCLUDE_RODATA(const s32, "code_001B2E60", D_004B4050);

INCLUDE_RODATA(const s32, "code_001B2E60", D_004B4068);

INCLUDE_RODATA(const s32, "code_001B2E60", D_004B4078);

INCLUDE_ASM(const s32, "code_001B2E60", mdlHandleViewerAssetSelection);

INCLUDE_ASM(const s32, "code_001B2E60", mdlDrawViewerAssetSelection);

u32 mdlRunViewerAssetSelectionTask(void) {
    mdlHandleViewerAssetSelection();
    mdlDrawViewerAssetSelection();
    return 0;
}

INCLUDE_ASM(const s32, "code_001B2E60", func_001B3D68);

void mdlDrawViewerIndexedLabelOverlay(void) {
    mdlAppendViewerRectToDrawList(0x8A10, 0x7948, 0xFF007F, 0x4E0, 0x90, 0);
    sdfDrawTagLink(mdlViewerState.packetList, (u32)(sdfDevPrintf(0x8A40, 0x7960, 0xFF0080, 0, D_00367B10[mdlViewerState.labelIndexA])));
}

u32 mdlRunViewerIndexedLabelTask(void) {
    mdlUpdateViewerCameraFromPad();
    mdlDrawViewerIndexedLabelOverlay();
    return 0;
}

INCLUDE_ASM(const s32, "code_001B2E60", mdlUpdateViewerSelectedModelFromPad);

void mdlDrawViewerSelectionLabel(void) {
    mdlAppendViewerRectToDrawList(0x8A10, 0x7948, 0xFF007F, 0x4E0, 0x90, 0);
    sdfDrawTagLink(mdlViewerState.packetList, (u32)(sdfDevPrintf(0x8A40, 0x7960, 0xFF0080, 0, D_00367B18[mdlViewerState.labelIndexB])));
}

s32 mdlRunViewerSelectionLabelTask(void) {
    mdlUpdateViewerSelectedModelFromPad();
    if (D_003D7A60[0] == 0) {
        mdlDrawViewerSelectionLabel();
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_001B2E60", mdlAdjustViewerScale);

INCLUDE_ASM(const s32, "code_001B2E60", mdlDrawViewerMotionTimeline);

u32 mdlUpdateViewerScaleTask(void) {
    mdlAdjustViewerScale();
    mdlDrawViewerMotionTimeline();
    return 0;
}

/* Final workspace publication: see docs/idioms.md, "Publishing viewer workspace". */
void mdlUpdateViewerWorkspaceSetup(void) {
    s32 initialPhase = 1;

    switch (mdlViewerState.workspaceSetupPhase) {
    case 0:
        if (D_00398629[0] < 0) {
            f32 scale = mdlViewerState.captureScale;

            mdlViewerState.yawStepMode = initialPhase;
            mdlViewerState.unk50 = 0;
            mdlViewerState.unk52 = 0;
            mdlViewerState.taskPhase = -1;
            D_003BD888 = mdlViewerState.unitStepMode;
            mdlViewerState.unitStepMode = initialPhase;
            sdf3DScreenAdjust = initialPhase;
            sdf3DScreenAdjustScaleY = sdf3DScreenAdjustScaleX = scale;
            mdlViewerState.workspaceSetupPhase = initialPhase;
        }
        break;
    case 1:
        sdf3DScreenAdjustOffsetX = mdlViewerState.captureDivX * 0x100 -
                   mdlViewerState.unk50 * 0x200 - 0x100;
        sdf3DScreenAdjustOffsetY = mdlViewerState.captureDivY * 0x70 -
                   mdlViewerState.unk52 * 0xE0 - 0x70;
        mdlViewerState.workspaceSetupPhase = 2;
        break;
    case 2:
        /* 0x0021C218..0x0021C228: completed phase retained in retail dispatch. */
        break;
    }
}

void mdlAfterViewerWorkspaceUpdate(void) {
}

/* Run workspace setup and its separate hook; keep the task result zero. */
u32 mdlUpdateViewerWorkspaceSetupTask(void) {
    mdlUpdateViewerWorkspaceSetup();
    mdlAfterViewerWorkspaceUpdate();
    return 0;
}

INCLUDE_ASM(const s32, "code_001B2E60", mdlApplyViewerResourceMenuAction);

/* Draw resource actions with independent eligibility and selection styles. */
void mdlDrawViewerResourceMenu(void) {
    SifCommand packet;
    s32 row;
    s32 enabled;
    s32 style;

    mdlAppendViewerRectToDrawList(0x8590, 0x7948, 0xFF007F, 0x960, 0x210, 0);
    for (row = 0; row != 5; row++) {
        enabled = 0;
        switch (row) {
        case 0:
            if (mdlViewerState.resourceCount < MDL_VIEWER_RESOURCE_SLOTS) {
                enabled = 1;
            }
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            if (mdlViewerState.resourceCount >= 2) {
                enabled = 1;
            }
            break;
        }
        if (mdlViewerState.selectionHighlightPhase < 20 && mdlViewerState.scrollPage == row) {
            style = enabled ? 4 : 12;
        } else {
            style = enabled ? 0 : 8;
        }
        sdfDevSetPrintCB(&packet, 0x85C0, 0x7960 + row * 0x60, 0xFF0080, style);
        sdfDrawTagLink(mdlViewerState.packetList,
            (u32)sdfDevPrintfEx(&packet, D_00367B20[row]));
    }
}

u32 mdlRunViewerResourceMenuTask(void) {
    mdlApplyViewerResourceMenuAction();
    mdlDrawViewerResourceMenu();
    return 0;
}

void mdlHandleViewerNodeCursorInput(void) {
    MdlCountNode *firstNode = mdlViewerState.resources[0]->inner->assetData;

    if (firstNode != 0) {
        s16 nodeCount = firstNode->count;

        if (nodeCount > 0) {
            mdlUpdateViewerCursor(&mdlViewerState.nodeCursor, nodeCount);
        }
    }
}

void mdlDrawViewerTexturePreview(void) {
    s32 index = 0;
    s32 count = 0;
    s32 width, height;
    MdlCountNode *node;
    sdfDrawTag_t *packetList;
    s32 displayWidth, displayHeight;
    s32 selectedNumber = 0;

    mdlAppendViewerRectToDrawList(0x7150, 0x7A08, 0xFF007F, 0x1060, 0x8F0, 0);
    node = mdlViewerState.resources[0]->inner->assetData;
    if (node != NULL) {
        count = node->count;
        index = mdlViewerState.nodeCursor;
        selectedNumber = index + (count > 0);
    }
    packetList = mdlViewerState.packetList;
    sdfDrawTagLink(packetList, (u32)(sdfDevPrintf(0x7180, 0x7A20,
        0xFF0080, 0, D_003ABDA8, selectedNumber, count)));
    if (count > 0) {
        width = node->textures[index]->width;
        height = node->textures[index]->height;
        sdfDrawTagLink(packetList, (u32)(sdfDevPrintf(0x7780, 0x7A20,
            0xFF0080, 0, D_003BBC88, width, height)));
        sdf2DLinkTexPacket(packetList, node->textures[index], SDF_GS_CONTEXT_1);
        displayWidth = width << 4;
        displayHeight = height << 3;
        if (width < height) {
            if (height > 256) {
                displayWidth = ((width << 8) / height) << 4;
                displayHeight = 0x800;
            }
        } else {
            if (width > 256) {
                displayWidth = 0x1000;
                displayHeight = ((height << 8) / width) << 3;
            }
        }
        sdfPrimLinkSprite(packetList, 0x80808080, 0, 0x7180, 0x7AE0,
            0, 0, displayWidth + 0x7180, displayHeight + 0x7AE0,
            width << 4, height << 4, 0xFF0080, 0);
    }
}

u32 mdlUpdateViewerNodeCursorTask(void) {
    mdlHandleViewerNodeCursorInput();
    mdlDrawViewerTexturePreview();
    return 0;
}

INCLUDE_RODATA(const s32, "code_001B2E60", D_004B40F8);

INCLUDE_RODATA(const s32, "code_001B2E60", D_004B4108);

INCLUDE_RODATA(const s32, "code_001B2E60", D_004B4118);

INCLUDE_RODATA(const s32, "code_001B2E60", D_004B4128);

INCLUDE_RODATA(const s32, "code_001B2E60", D_004B4138);

INCLUDE_RODATA(const s32, "code_001B2E60", D_004B4148);

INCLUDE_ASM(const s32, "code_001B2E60", mdlIsDebugTimeGraph);

INCLUDE_ASM(const s32, "code_001B2E60", func_001B5178);

INCLUDE_RODATA(const s32, "code_001B2E60", D_004B41C0);

INCLUDE_RODATA(const s32, "code_001B2E60", D_004B41D8);

INCLUDE_RODATA(const s32, "code_001B2E60", D_004B41F0);

INCLUDE_RODATA(const s32, "code_001B2E60", D_004B4208);

INCLUDE_RODATA(const s32, "code_001B2E60", D_004B4220);

INCLUDE_RODATA(const s32, "code_001B2E60", D_004B4230);

INCLUDE_RODATA(const s32, "code_001B2E60", D_004B4240);

INCLUDE_ASM(const s32, "code_001B2E60", mdlDrawViewerSettings);

u32 mdlRunViewerSettingsTask(void) {
    mdlUpdateViewerSettingsInput();
    mdlDrawViewerSettings();
    return 0;
}

/* Sum context-wide and selected-entry list counts, retaining both lookups. */
s32 mdlCountActiveRecords(void) {
    dds3ModelHandle_t *resource = mdlViewerState.resources[0];
    s32 firstListCount =
        mdlPibGetChunkItemCount(mdlPibGetChunk(resource, MDL_PART_INFO_LIST_CONTEXT_WIDE));
    s32 activeListCount =
        mdlPibGetChunkItemCount(mdlPibGetChunk(resource, mdlViewerState.activeEntryId));

    return firstListCount + activeListCount;
}

/* Count the context-wide list before the selected-entry list for cursor lookup. */
MdlRecord *mdlGetSelectedRecord(void) {
    dds3ModelHandle_t *resource = mdlViewerState.resources[0];
    MdlRecord *firstList =
        mdlPibGetChunk(resource, MDL_PART_INFO_LIST_CONTEXT_WIDE);
    MdlRecord *secondList = mdlPibGetChunk(resource, mdlViewerState.activeEntryId);
    s32 index = mdlViewerState.selectedRecordIndex;
    MdlRecord *record;

    if (firstList != NULL) {
        record = mdlPibGetFirstChunkItem(firstList);
        while (record != NULL) {
            if (index == 0) {
                return record;
            }
            index--;
            record = mdlPibGetNextChunkItem(record);
        }
    }
    if (secondList == NULL) {
        return NULL;
    }
    record = mdlPibGetFirstChunkItem(secondList);
    while (record != NULL) {
        if (index == 0) {
            return record;
        }
        index--;
        record = mdlPibGetNextChunkItem(record);
    }
    return NULL;
}

INCLUDE_ASM(const s32, "code_001B2E60", mdlUpdateViewerMarkEditorInput);

void mdlDrawViewerEffectRecordList(void) {
    MdlViewState *state = &mdlViewerState;
    dds3ModelHandle_t *resource;
    MdlRecord *firstList;
    MdlRecord *secondList;
    MdlRecord *record;
    s32 firstCount;
    s32 secondCount;
    s32 totalCount;
    s32 firstRemaining;
    s32 selected;
    s32 scroll;
    s32 skipCount;
    s32 rowCount;
    s32 markerY;
    s32 descriptionY;
    sdfDrawTag_t *packetList;

    mdlAppendViewerRectToDrawList(0x7150, 0x7A08, 0xFF007F, 0xF60, 0x270, 0);
    resource = state->resources[0];
    packetList = state->packetList;
    firstList = mdlPibGetChunk(resource, MDL_PART_INFO_LIST_CONTEXT_WIDE);
    firstCount = mdlPibGetChunkItemCount(firstList);
    secondList = mdlPibGetChunk(resource, state->activeEntryId);
    secondCount = mdlPibGetChunkItemCount(secondList);
    totalCount = firstCount + secondCount;

    if (totalCount == 0) {
        sdfDrawTagLink(packetList,
                        (u32)sdfDevPrintf(0x7300, 0x7A20,
                                                         0xFF0080, 0,
                                                         D_003ABEB0));
    } else {
        s32 maxScroll = totalCount - 6;

        if (maxScroll < 0) {
            maxScroll = 0;
        }
        scroll = state->effectListScroll;
        if (scroll > maxScroll) {
            scroll = maxScroll;
        }

        firstRemaining = firstCount;
        if (firstCount != 0) {
            record = mdlPibGetFirstChunkItem(firstList);
        } else {
            record = mdlPibGetFirstChunkItem(secondList);
        }

        selected = mdlViewerState.selectedRecordIndex;
        if (selected >= totalCount) {
            selected = totalCount - 1;
            mdlViewerState.selectedRecordIndex = selected;
        }
        if (selected < scroll) {
            scroll = selected;
        } else if (selected >= scroll + 6) {
            scroll = selected - 5;
        }
        mdlViewerState.effectListScroll = scroll;

        skipCount = scroll;
        rowCount = 0;
        if (record != NULL) {
            descriptionY = 0x7A20;
            do {
                s32 selectionMarker = ' ';

                if (skipCount > 0) {
                    skipCount--;
                } else {
                    s32 listMarker = ' ';
                    s32 selectedStyle = 0;

                    if (selected == 0) {
                        if (mdlViewerState.editorMode == 0) {
                            selectionMarker = '>';
                        } else {
                            selectedStyle = 4;
                        }
                    }

                    if (firstRemaining > 0) {
                        listMarker = '*';
                    }

                    markerY = 0x7A20 + rowCount * 0x60;
                    if (selectionMarker != ' ' || listMarker != ' ') {
                        sdfDrawTagLink(packetList,
                                        (u32)sdfDevPrintf(
                                            0x7180, markerY, 0xFF0080, selectedStyle,
                                            D_003BBCB0, selectionMarker, listMarker));
                    }

                    mdlPibDrawEffectInfo(packetList, 0x7300, descriptionY, 0xFF0080,
                                  selectedStyle, record);
                    descriptionY += 0x60;
                    rowCount++;
                }

                if (firstRemaining > 0 && --firstRemaining == 0) {
                    record = secondList != NULL ? mdlPibGetFirstChunkItem(secondList) : NULL;
                } else {
                    record = mdlPibGetNextChunkItem(record);
                }
                selected--;
                if (rowCount >= 6) {
                    break;
                }
            } while (record != NULL);
        }

        if (scroll != 0) {
            sdfPrimLinkF3(packetList, 0x8000A0C0, 0,
                                 0x8040, 0x7A30, 0x8000, 0x7A70,
                                 0x8080, 0x7A70, 0xFF0080, NULL);
        }
        if (record != NULL) {
            sdfPrimLinkF3(packetList, 0x8000A0C0, 0,
                                 0x8000, 0x7C10, 0x8080, 0x7C10,
                                 0x8040, 0x7C50, 0xFF0080, NULL);
        }
    }

    if (mdlViewerState.editorMode == 1) {
        MdlRecord *markRecord = mdlGetSelectedRecord();

        if (markRecord != NULL && mdlPibCheckItemType(markRecord, MDL_VIEWER_RECORD_KIND_TRACK_POLY)) {
            mdlAppendViewerRectToDrawList(0x8590, 0x7A08, 0xFF007F, 0x960, 0xA50, 0);
            mdlPibDrawTrackInfo(packetList, 0x85C0, 0x7A20, 0xFF0080,
                                   (EffMarkParams *)markRecord,
                                   mdlViewerState.markFieldCursor);
        }
    }
}

u32 mdlRunViewerEffectEditorTask(void) {
    mdlUpdateViewerMarkEditorInput();
    mdlDrawViewerEffectRecordList();
    return 0;
}

s32 mdlRequestViewerExitOnce(void) {
    if (mdlViewerState.exitCleanupStarted == 0) {
        mdlViewerState.exitCleanupStarted = 1;
        mdlCleanupViewerTasksAndResources();
        dds3AdminiExitSequence();
    }
    return 0;
}

/* Submit the intermediate packet with an identity basis in phases 2 and 3. */
void mdlSubmitViewerIntermediateDrawPacket(void) {
    sdfDrawTag_t *packetList;

    if (mdlViewerState.taskPhase < 4) {
        if (mdlViewerState.taskPhase >= 2) {
            packetList = dds3FontReqAllocDrawTag();
            SDF_VU0_LOADIDENTITY();
            sdfDrawTagLink(packetList, (u32)((s32)sdfDrawRotatedModel(D_00367B80, D_00367BE0, 6, 0x80)));
            D_00325048.regFunc(&D_00325048, packetList);
        }
    }
}

/* Submit the selected resource in phase 3; keep the native bitwise input/once-flag test. */
void mdlSubmitViewerResourceDrawPacket(void) {
    sdfDrawTag_t *packetList;
    s32 drawPacket;
    dds3ModelHandle_t *resource;

    if (mdlViewerState.taskPhase == 3) {
        packetList = dds3FontReqAllocDrawTag();
        resource = mdlViewerState.resources[0];
    VU0_LOAD_MATRIX(resource->inner->matrix);
        drawPacket = (s32)sdfDrawRotatedModel(D_00367C00, D_00367C40, 4, 0x80);
        if ((dds3PadBitTable[13] < 0) & (D_003BBCB8 == 0)) {
            D_003BBCB8 = 1;
            sdfDumpHexLine(drawPacket, 0x100);
        }
        sdfDrawTagLink(packetList, (u32)(drawPacket));
        D_00325048.regFunc(&D_00325048, packetList);
    }
}

void mdlViewerTaskDestroy(void) {
    if (mdlViewerState.viewerTask != NULL) {
        dds3KillProcess(mdlViewerState.viewerTask, 0);
        mdlViewerState.viewerTask = NULL;
    }
}

INCLUDE_ASM(const s32, "code_001B2E60", mdlRestartViewerPhaseTask);

INCLUDE_ASM(const s32, "code_001B2E60", mdlViewer);

/* Walk native 64-byte map-position records and submit their visualization in one packet list. */
void mdlDrawMapPositionRecords(dds3ModelHandle_t *resource) {
    s32 recordCount = sdfModelGetMarkPosCount(resource->inner);
    s32 recordIndex;
    sdfDrawTag_t *packetList;
    SdfMapPositionRecord *recordCursor;

    if (recordCount > 0) {
        packetList = dds3FontReqAllocDrawTag();
        recordCursor = (SdfMapPositionRecord *)(
            (SdfMapPositionChunkPrefix *)sdfModelGetExtraChunk(resource->inner, MDL_MAP_POSITION_TAG) + 1);
        for (recordIndex = 0; recordIndex != recordCount; recordIndex++) {
            sdfModelMakeMarkPosMatrix(resource->inner, recordCursor);
            recordCursor++;
            sdfDrawTagLink(packetList, (u32)((s32)sdfDrawRotatedModel(D_00367C50, D_00367CB0, 6, 0x80)));
        }
        D_00325048.regFunc(&D_00325048, packetList);
    }
}

INCLUDE_ASM(const s32, "code_001B2E60", func_001B6528);

INCLUDE_RODATA(const s32, "code_001B2E60", D_004B4270);

INCLUDE_ASM(const s32, "code_001B2E60", func_001B6770);

INCLUDE_ASM(const s32, "code_001B2E60", func_001B6CC0);

INCLUDE_ASM(const s32, "code_001B2E60", mdlInitializeViewerTasksAndResources);

/* Clear each occupied resource slot before destroying its context; retain the native scan form. */
void mdlFreeViewResources(void) {
    s32 resourceIndex;
    for (resourceIndex = 0; resourceIndex != MDL_VIEWER_RESOURCE_SLOTS; resourceIndex++) {
        dds3ModelHandle_t *resource = mdlViewerState.resources[resourceIndex];
        if (resource != 0) {
            mdlViewerState.resources[resourceIndex] = 0;
            mdlFree(resource);
        }
    }
}

INCLUDE_ASM(const s32, "code_001B2E60", func_001B7190);

INCLUDE_RODATA(const s32, "code_001B2E60", D_004B43C8);

INCLUDE_RODATA(const s32, "code_001B2E60", D_004B43D8);

INCLUDE_RODATA(const s32, "code_001B2E60", D_004B43E8);

INCLUDE_RODATA(const s32, "code_001B2E60", D_004B43F8);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF698);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF6A0);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF6A8);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF6B0);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF6B8);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF6C0);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF6C8);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF6D0);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF6D8);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF6E0);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF6E8);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF6F0);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF6F8);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF700);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF708);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF710);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF718);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF720);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF728);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF730);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF738);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF73C);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF740);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF748);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF750);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF758);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF760);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF768);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF770);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF778);

INCLUDE_SDATA(const s32, "code_001B2E60", D_004DF780);

