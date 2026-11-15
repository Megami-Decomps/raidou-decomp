#include "common.h"
#include "sdf_resource.h"
#include "basic.h"
#include "dds3obj.h"
#include "field.h"
#include "fileManager.h"

enum fldPackedResourceKind
{
    FLD_PACKED_RESOURCE_INFO_TABLE = 1,
    FLD_PACKED_RESOURCE_NPC_PALETTE = 2,
    FLD_PACKED_RESOURCE_SKY_BUFFER = 3,
    FLD_PACKED_RESOURCE_ACTOR_WAYPOINT_TABLE = 4,
    FLD_PACKED_RESOURCE_RETAINED_COPY = 5,
    FLD_PACKED_RESOURCE_SCENE_RECORD_CHUNK = 6
};

typedef struct objectTransform_t objectTransform_t;

typedef s32 (*dds3MoverUpdate_t)(struct objectTransform_t *, dds3Basic_t *);

typedef struct worldIndexNode_t worldIndexNode_t;

typedef struct motion_t motion_t;

typedef struct fldMoverWork_t fldMoverWork_t;

typedef struct fldTransferChunk_t fldTransferChunk_t;

typedef struct fldNpcPalette_t fldNpcPalette_t;

typedef struct fldSkyBuffer_t fldSkyBuffer_t;

/* Kind 3 allocates 0x10 bytes in dds3AllocateClearedObjectWork. */
struct fldMoverWork_t {
    dds3Basic_t *target; /* 0x00: object whose transform is updated. */
    dds3PathBasicControl_t *path; /* 0x04: retained curve work. */
    dds3MoverUpdate_t update; /* 0x08: used when no curve work is retained. */
    dds3Basic_t *sourceObject; /* 0x0C: kind-16 curve source. */
};

/* Packed-resource transfer descriptor; the first word's role is unknown. */
struct fldTransferChunk_t {
    u32 unk00; /* 0x00: opaque descriptor word */
    s32 offset; /* 0x04: signed byte offset from the resource base */
    u32 size; /* 0x08: byte length of the transferred payload */
};

#include "sdk_libc.h"

typedef struct fldModelData_t {
    u8 unk00[16]; /* 0x00 */
    u8 modelDef[16]; /* 0x10: the model definition dds3AddModelBasic2 takes */
    void *playBook; /* 0x20: motion play book */
} fldModelData_t;

typedef struct fldResourceAreaRef_t {
    u32 word00; /* 0x00 */
    const u8 *name; /* 0x04 */
} fldResourceAreaRef_t;

void sdfAddressResolverReset(u32 *aDataBody, u32 aOffset, const u8 *aAddrInfoPtr, s32 aAddrInfoLeng);
extern void *memcpy(void *a, const void *b, size_t c);
extern u32 fldNextTbSize, fldNextF1Size, fldNextF1Size, fldNextF2Size;
extern u32 fldNextTbData, fldNextF1Data, fldNextF1Data, fldNextF2Data;

typedef struct fldFileAreaDataFL1_t {
    u32 fileID;
    u32 SkyTblIdx;
    char *filename0;
    char *filename1;
    char *filename2;
    char *filename3;
} fldFileAreaDataFL1_t;

typedef struct fldFileInfoHeaderFL1_t {
    u32 nResourceTypeTable;
    void *pResourceTypeTable;
    fldFileAreaDataFL1_t *pAreaData;
} fldFileInfoHeaderFL1_t;

extern addr_t fldNextTbHandle, fldFileBufF1, fldFileBuf2;
extern u8 *FildMaskObj, *mat_black, *mat_white, *mat_line;

typedef struct fldConstruntAreaFL2_t {
    u32 flag; /* 0x00 */
    const char *areaName; /* 0x04 */
    u32 areaID; /* 0x08 */
} fldConstruntAreaFL2_t;

typedef struct fldFileAreaDataFL2_t {
    u32 word00;
    s32 label; /* 0x04: the script entry recorded for a field retry. */
} fldFileAreaDataFL2_t;

typedef struct fldFileInfoHeaderFL2_t {
    u32 nResourceTypeTable; /* 0x00 */
    void *pResourceTypeTable; /* 0x04 */
    fldFileAreaDataFL2_t *pAreaData; /* 0x08 */
    u32 nArea; /* 0x0C */
    fldConstruntAreaFL2_t *pAreaTable; /* 0x10 */
} fldFileInfoHeaderFL2_t;

/* FL1 and FL2 use the same 0x0C-byte resource-type table entries. */
typedef struct fldSpawnBatch_t {
    u32 kind;
    u32 count;
    fldResHead_t *entries;
} fldSpawnBatch_t;

typedef struct fldBillConfigView_t {
    u32 flags;
    u32 kind;
    u32 resourceIndex;
    f32 width;
} fldBillConfigView_t;

typedef struct fldLightDataView_t {
    u8 pad00[0x64];
    u32 flags;
} fldLightDataView_t;

typedef struct fldCollisionDescriptor_t fldCollisionDescriptor_t;

typedef struct fldSpawnCollisionView_t {
    u32 kind;
    u32 word04;
    fldCollisionDescriptor_t *header;
} fldSpawnCollisionView_t;

enum {
    FLD_SPAWN_BATCH_KIND_SCRIPT_RESOURCE = 2,
    FLD_SPAWN_BATCH_KIND_COLLISION = 3,
    FLD_SPAWN_BATCH_KIND_CAMERA = 4,
    FLD_SPAWN_BATCH_KIND_EVENT = 6,
    FLD_SPAWN_BATCH_KIND_UNSUPPORTED = 7,
    FLD_SPAWN_BATCH_KIND_PATH = 9,
    FLD_SPAWN_BATCH_KIND_POINT = 10,
    FLD_SPAWN_BATCH_KIND_BILLBOARD_EFFECT = 11,
    FLD_SPAWN_BATCH_KIND_LIGHT = 12
};

typedef struct fldResourceEffect_t fldResourceEffect_t;

typedef struct fldResourceLight_t {
    u32 resv, Flag, AnimeType;
    f32 InsideR, OutsideR, Soften, Bias;
    f32 DiffuseR, DiffuseG, DiffuseB;
    f32 AmbientR, AmbientG, AmbientB;
} fldResourceLight_t;

/* Resolved FL2 event and placement payloads, selected by the batch kind. */
typedef struct fldSpawnEventDataFL2_t {
    u32 kind;
    void *data;
} fldSpawnEventDataFL2_t;

typedef struct fldSpawnSoundDataFL2_t {
    f32 radius;
    s32 soundId;
} fldSpawnSoundDataFL2_t;

typedef struct fldEventDataHeader_t fldEventDataHeader_t;

extern u8 *FildMaskObj;
extern u8 *mat_black;
extern u8 *mat_white;
extern addr_t fldNextTbHandle;
extern addr_t fldFileBufF1;
extern u32 fldNextTbData;
extern u32 fldNextF1Data;
extern u32 fldNextTbSize;
extern u32 fldNextF1Size;

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#define SMT3_DEFINES_FLD_PACKED_RESOURCE_ACTOR_WAYPOINT_TABLE 1

#define SMT3_DEFINES_FLD_PACKED_RESOURCE_INFO_TABLE 1

#define SMT3_DEFINES_FLD_PACKED_RESOURCE_NPC_PALETTE 1

#define SMT3_DEFINES_FLD_PACKED_RESOURCE_RETAINED_COPY 1

#define SMT3_DEFINES_FLD_PACKED_RESOURCE_SCENE_RECORD_CHUNK 1

#define SMT3_DEFINES_FLD_PACKED_RESOURCE_SKY_BUFFER 1

#define SMT3_DEFINES_FLD_SPAWN_BATCH_KIND_BILLBOARD_EFFECT 1

#define SMT3_DEFINES_FLD_SPAWN_BATCH_KIND_CAMERA 1

#define SMT3_DEFINES_FLD_SPAWN_BATCH_KIND_COLLISION 1

#define SMT3_DEFINES_FLD_SPAWN_BATCH_KIND_EVENT 1

#define SMT3_DEFINES_FLD_SPAWN_BATCH_KIND_LIGHT 1

#define SMT3_DEFINES_FLD_SPAWN_BATCH_KIND_PATH 1

#define SMT3_DEFINES_FLD_SPAWN_BATCH_KIND_POINT 1

#define SMT3_DEFINES_FLD_SPAWN_BATCH_KIND_SCRIPT_RESOURCE 1

#define SMT3_DEFINES_FLD_SPAWN_BATCH_KIND_UNSUPPORTED 1

#define SMT3_DEFINES_dds3MoverUpdate_t 1

#define SMT3_DEFINES_fldBillConfigView_t 1

#define SMT3_DEFINES_fldCollisionDescriptor_t 1

#define SMT3_DEFINES_fldConstruntAreaFL2_t 1

#define SMT3_DEFINES_fldEventDataHeader_t 1

#define SMT3_DEFINES_fldFileAreaDataFL1_t 1

#define SMT3_DEFINES_fldFileAreaDataFL2_t 1

#define SMT3_DEFINES_fldFileInfoHeaderFL1_t 1

#define SMT3_DEFINES_fldFileInfoHeaderFL2_t 1

#define SMT3_DEFINES_fldLightDataView_t 1

#define SMT3_DEFINES_fldModelData_t 1

#define SMT3_DEFINES_fldMoverWork_t 1

#define SMT3_DEFINES_fldNpcPalette_t 1

#define SMT3_DEFINES_fldPackedResourceKind 1

#define SMT3_DEFINES_fldResourceAreaRef_t 1

#define SMT3_DEFINES_fldResourceEffect_t 1

#define SMT3_DEFINES_fldResourceLight_t 1

#define SMT3_DEFINES_fldSkyBuffer_t 1

#define SMT3_DEFINES_fldSpawnBatch_t 1

#define SMT3_DEFINES_fldSpawnCollisionView_t 1

#define SMT3_DEFINES_fldSpawnEventDataFL2_t 1

#define SMT3_DEFINES_fldSpawnSoundDataFL2_t 1

#define SMT3_DEFINES_fldTransferChunk_t 1

#define SMT3_DEFINES_motion_t 1

#define SMT3_DEFINES_objectTransform_t 1

#define SMT3_DEFINES_worldIndexNode_t 1

#include "kernel_draw.h"
#include "sdf_packet_list.h"
#include "dds/dds3_draw_buffer.h"
#include "dds/dds3_fade.h"
#include "dds/dds3_object_flags.h"
#include "dds/dds3_task_create.h"
#include "dds/dds3_world_primary.h"
#include "dds/dds3obj.h"
#include "dds/eff_transform.h"
#include "dds/evt_unit.h"
#include "dds/evt_unit_color_transition.h"
#include "dds/evt_world.h"
#include "dds/fld_scene_ready_api.h"
#include "dds/fr_font.h"
#include "dds/mdl.h"
#include "dds/mdl_motion_api.h"
#include "dds/sdf.h"
#include "dds/sdf_column_packet_api.h"
#include "dds/sdf_dev_state.h"
#include "dds/sdf_gs_blend.h"
#include "dds/sdf_gs_packet.h"
#include "dds/sdf_heap_api.h"
#include "dds/sdf_line_pair_packet_api.h"
#include "dds/sdf_map_position_api.h"
#include "dds/sdf_motion.h"
#include "dds/sdf_packet_alloc.h"
#include "dds/sdf_packet_append.h"
#include "dds/sdf_packet_builders.h"
#include "dds/sdf_packet_header.h"
#include "dds/sdf_packet_list.h"
#include "dds/sdf_primitive_packet_api.h"
#include "dds/sdf_sif_command.h"
#include "dds/sdf_texture_draw_packet.h"
#include "dds/sdf_texture_file.h"
#include "dds/sdf_texture_primary_buffer_api.h"
#include "dds/sdf_trig.h"
#include "dds/sdf_vu_matrix.h"

enum {
    OBJECT_TRANSFORM_FLAG_UPDATE_PENDING = 1,
    OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID = 2,
    OBJECT_TRANSFORM_FLAG_USE_SMOOTHED_POSITION = 8
};

/* Register selectors carried by GIF A+D writes in the two GS contexts. */
enum {
    SDF_GS_PRIM = 0x00,
    SDF_GS_RGBAQ = 0x01,
    SDF_GS_XYZ2 = 0x05,
    SDF_GS_TEX0_1 = 0x06,
    SDF_GS_TEX0_2 = 0x07,
    SDF_GS_CLAMP_1 = 0x08,
    SDF_GS_CLAMP_2 = 0x09,
    SDF_GIF_REGISTER_AD = 0x0E,
    SDF_GS_TEX1_1 = 0x14,
    SDF_GS_TEX1_2 = 0x15,
    SDF_GS_XYOFFSET_1 = 0x18,
    SDF_GS_XYOFFSET_2 = 0x19,
    SDF_GS_PRMODECONT = 0x1A,
    SDF_GS_TEXA = 0x3B,
    SDF_GS_TEXFLUSH = 0x3F,
    SDF_GS_SCISSOR_1 = 0x40,
    SDF_GS_SCISSOR_2 = 0x41,
    SDF_GS_ALPHA_1 = 0x42,
    SDF_GS_ALPHA_2 = 0x43,
    SDF_GS_DTHE = 0x45,
    SDF_GS_COLCLAMP = 0x46,
    SDF_GS_TEST_1 = 0x47,
    SDF_GS_TEST_2 = 0x48,
    SDF_GS_FRAME_1 = 0x4C,
    SDF_GS_FRAME_2 = 0x4D,
    SDF_GS_ZBUF_1 = 0x4E,
    SDF_GS_ZBUF_2 = 0x4F,
    SDF_GS_BITBLTBUF = 0x50,
    SDF_GS_TRXPOS = 0x51,
    SDF_GS_TRXREG = 0x52,
    SDF_GS_TRXDIR = 0x53
};

enum SdfMotionStateValue {
    SDF_MOTION_STATE_UNINITIALIZED = 0,
    SDF_MOTION_STATE_INITIALIZED = 1,
    SDF_MOTION_STATE_SAMPLING = 4,
    SDF_MOTION_STATE_TERMINAL = 5,
    SDF_MOTION_STATE_SUSPENDED = 6,
};

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */

#include "dds/sdk_ps2.h"
#include "dds/fld_item_box_object_api.h"
#include "dds/sdf_gs_header.h"
#include "dds/sdf_pac_reloc.h"
#include "dds/fld_script_resource.h"
#include "dds/eff.h"
#include "dds/fld_player_scene_state.h"
#include "dds/fld_marker_packet.h"
#include "dds/common.h"
#include "dds/pty_status_query_api.h"
#include "dds/file_config_option_api.h"
#include "dds/fld_camera_model_motion_request.h"
#include "dds/fld_elv_camera_scene_api.h"
#include "dds/fld_placement_batch_mode.h"
#include "dds/sdf_vu0.h"
#include "dds/fld_stop_entry.h"
#include "dds/sdf_resource_list_release_api.h"
#include "dds/dds3_background_fade_api.h"
#include "dds/fld_movement_hold_state_api.h"
#include "dds/fld_movement_gate_result.h"
#include "dds/fld_area_resource_ready_api.h"
#include "dds/fld_area_resource_name_change_api.h"
#include "dds/fld_area_resource_request_reuse_api.h"
#include "dds/fld_file_io_priority_state.h"
#include "dds/fld_location_coordinate_flags.h"
#include "dds/fld_scene_record_api.h"
#include "dds/fld_scene_object_key_result.h"
#include "dds/fld_light_set.h"
#include "dds/fld_sky_buffer.h"
#include "dds/fld_sky_gradient_record.h"
#include "dds/fld_quad_state.h"
#include "dds/dds3_frame_draw_buffer.h"
#include "dds/mnu_title_stream.h"
#include "dds/fld_bill_config.h"
#include "dds/fld_area_path.h"
#include "dds/fld_npc_motion.h"
#include "dds/fld_npc_palette.h"
#include "dds/btl_stage_task_cleanup.h"
#include "dds/fld_resource_resolver.h"
#include "dds/fld_clear_menu_entries_api.h"
#include "dds/fld_play_pending_sounds_api.h"
#include "dds/fld_release_texture_slots_api.h"
#include "dds/fld_transfer_chunk.h"
#include "dds/fld_camera_follow_row.h"
#include "dds/fld_named_camera_table.h"
#include "dds/fld.h"
#include "dds/field.h"
#include "dds/fld_encounter_runtime_api.h"
#include "dds/fld_collision_geometry.h"
#include "dds/fld_collision_query_api.h"
#include "dds/fld_scene_request.h"
#include "dds/fld_room_resource_api.h"
#include "dds/fld_area_room_resource_load_api.h"
#include "dds/fld_file_addr_reset_mode.h"
#include "dds/fld_spawn_event.h"
#include "dds/dds3_object_control_slot.h"
#include "dds/evt_world_object_api.h"
#include "dds/dds3_world_value.h"
#include "dds/evt_action_object.h"
#include "dds/sdf_texture_offset_list.h"
#include "dds/evt_script_model.h"
#include "dds/sdf_model.h"
#include "dds/mdl_flag_api.h"
#include "dds/dds3_world_counter.h"
#include "dds/dds3_world_secondary.h"
#include "dds/eff_light.h"
#include "dds/fld_area_work.h"
#include "dds/fld_spawn_collision.h"
#include "dds/evt_world_source_transform.h"
#include "dds/sdf_chip.h"
#include "dds/fld_waypoint.h"
#include "dds/fld_inf.h"
#include "dds/dds3_task_lifecycle.h"
#include "dds/file_request_api.h"
#include "dds/file_pac.h"
#include "dds/fld_packed_resource_kind.h"
#include "dds/fld_title_banner_menu.h"
#include "dds/field_stage.h"
#include "dds/sdf_pac_work.h"
#include "dds/fpu.h"
#include "dds/pcp_vu0.h"
#include "dds/scr.h"
#include "dds/dds3_process.h"
#include "dds/dds3_task_runner.h"
#include "dds/dat_state.h"
#include "dds/fld_object_slot_api.h"
#include "dds/fld_placement_data.h"
#include "dds/fld_resource_name.h"
#include "dds/fld_file_resource_link_view.h"
#include "dds/eff_object.h"
#include "dds/dds3_path.h"
#include "dds/fld_label_cache_api.h"
#include "dds/fld_sprite_vertex.h"
#include "dds/fld_room_state.h"
#include "dds/fld_trigger_shape.h"
#include "dds/fld_action_row.h"
#include "dds/btl.h"
#include "dds/itf.h"
#include "dds/fld_index_pair.h"

typedef struct SdfGsTextureAlphaFlushPacket {
    SdfGsPacketHeader header;
    SdfGsRegisterWrite textureAlpha;
    SdfGsRegisterWrite textureFlush;
} SdfGsTextureAlphaFlushPacket;

typedef char SdfGsTextureAlphaFlushPacket_size_must_be_0x40[
    (sizeof(SdfGsTextureAlphaFlushPacket) == 0x40) ? 1 : -1];

struct dds3Basic_t;

struct EffNodeDescriptor;

typedef char FldSpawnBatchSize[(sizeof(FldSpawnBatch) == 12) ? 1 : -1];

/* Camera-setting table capacity. */
enum {
    FIELD_CAMERA_SETTING_COUNT = 8
};

typedef struct SdfDrawPacket SdfDrawPacket;

typedef struct DmaPacketHeader DmaPacketHeader;

/* Per-actor slot in the field's actor table; reset by fldResetActorSlots and
   scanned by fldReleaseActorTasksById. 0x5C is the stride retail uses. Three transition keys, signed frame
   counters and two float triples with per-frame increments (same layout as DDS2). */
typedef struct {
    s32 kind;
    u32 actorId;
    u32 unk08;
    u32 firstKey;
    u32 secondaryMotion;
    u32 secondKey;
    u32 transitionKey;
    s32 firstFrame;
    s32 secondFrame;
    s32 transitionFrame;
    s32 frameCount;
    f32 firstValues[3];
    f32 firstStep[3];
    f32 secondValues[3];
    f32 secondStep[3];
} FldActorRow; /* 0x5C bytes */

typedef char FldActorRow_size_must_be_0x5C[sizeof(FldActorRow) == 0x5C ? 1 : -1];

static inline s32 fldTestBits(u32 flags, u32 mask) {
    return (flags & mask) != 0;
}

/* vu0 routine: normalize vf10 and return its original XYZ length. */
static inline f32 fldNormalizeProbeVector(void) {
    f32 length;

    SDF_VU0_GETLENGTH(length);
    SDF_VU0_NORMALIZE();
    return length;
}

extern void fldSpawnPlacementObjectsFromBatches(FldSpawnBatch *spawnBatches, u32 batchCount,
                                                FldPlacementBatchMode mode);

#include "dds/fld_event_display_resource_api.h"

/* Serialized transforms leave the position W component zero. */
static inline void fldCopySpawnTransform(f32 *position, f32 *rotation, const f32 *source) {
    if (source != NULL) {
        position[0] = source[0];
        position[1] = source[1];
        position[2] = source[2];
        position[3] = 0.0f;
        rotation[0] = source[4];
        rotation[1] = source[5];
        rotation[2] = source[6];
        rotation[3] = source[7];
    } else {
        position[0] = 0.0f;
        position[1] = 0.0f;
        position[2] = 0.0f;
        position[3] = 0.0f;
        rotation[0] = 0.0f;
        rotation[1] = 0.0f;
        rotation[2] = 0.0f;
        rotation[3] = 0.0f;
    }
}

void fldSpawnPlacementObjectsFromBatches(FldSpawnBatch *spawnBatch, u32 batchCount, FldPlacementBatchMode mode);
extern char D_0039FE08[], D_0039FE18[], D_0039FE28[];
void fldProcessFieldRequestAlternate(const FldSceneRequest *request);

extern const u8 *fldCachedRoomResourceData, *fldCachedRoomF1ResourceData, *fldCachedRoomF2ResourceData,
    *fldCachedRoomKF2ResourceData;

extern u32 fldCachedRoomResourceSize, fldCachedRoomF1ResourceSize, fldCachedRoomF2ResourceSize, fldCachedRoomKF2ResourceSize;
extern f32 D_003BAD10, D_003BAD14, D_003BAD18;

typedef union FldLookAtVector {
    u128 q;
    f32 f[4];
} FldLookAtVector;

typedef struct FldModelMatrices {
    u8 pad00[0xC];
    u8 **rows; /* 0x0C: slot-indexed matrix blocks */
} FldModelMatrices;

struct EffRandState;

/* Signed verticalStepDirection: adjust player Y by -2/+2 and save previous XYZ. */

extern s32 D_003BAD88, D_003BAD8C;
extern f32 D_003BAD90, D_003BAD94;

enum {
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_1_OFFSET_32 = 1,
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_2_OFFSET_32 = 2,
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_05_OFFSET_32 = 3,
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_1_OFFSET_48 = 4,
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_2_OFFSET_48 = 5,
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_05_OFFSET_48 = 6,
};

extern s32 D_003BADB8, D_003BADBC;
extern f32 D_003BADC0, D_003BADC4;

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldMakeBlockBasicFL2);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", MakeObjectBasic2);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldFileResolverFL1);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldFileResolverFL2);

void fldProcessFieldRequestAlternate(const FldSceneRequest *request) {
    fldSpawnPlacementObjectsFromBatches(request->spawnBatches, request->spawnBatchCount,
                                        FLD_PLACEMENT_BATCH_MODE_APPEND);
}

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldGetInfoHeader);

void sdfAddressResolverReset(u32 *aDataBody, u32 aOffset, const u8 *aAddrInfoPtr, s32 aAddrInfoLeng) {
    const u8 *p = aAddrInfoPtr;
    s32 code;
    s32 i;

    while (p - aAddrInfoPtr < aAddrInfoLeng) {
        code = *p++;
        if ((code & 1) == 0) {
            code >>= 1;
        } else if ((code & 2) == 0) {
            code = (code | (*p++ << 8)) >> 2;
        } else if ((code & 4) == 0) {
            code = (code | (p[0] << 8) | (p[1] << 16)) >> 3;
            p += 2;
        } else {
            code = (code >> 3) + 2;
            for (i = 0; i < code; i++) {
                aDataBody++;
                *aDataBody -= aOffset;
            }
            continue;
        }
        aDataBody += code;
        *aDataBody -= aOffset;
    }
}

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldAddressResolverReset);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldAddressResolver);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldLeaf9);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldLeaf10);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldLeaf11);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldLeaf12);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldLeaf13);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldLeaf14);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldLeaf15);

void fldLeaf16(void) {
}

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldLoadFile);

INCLUDE_ASM(const s32, "field/fldFileResolver_00132080", fldLoadFile_FromMemory);
INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", fldData15);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", fldCallTexListFreeData);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", fldCallTexListFreeData2);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", fldCallTexListFreeData3);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", fldCallTexListFreeData4);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", fldCallTexListFreeData5);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", fldCallTexListFreeData6);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", fldData16);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", fldCallTexListFreeData7);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", fldCallTexListFreeData8);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", fldData17);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", fldCallTexListFreeData9);

INCLUDE_SDATA(const s32, "field/fldFileResolver_00132080", fldCallTexListFreeData10);

