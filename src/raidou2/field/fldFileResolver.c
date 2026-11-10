#include "common.h"

/* Declarations from dds-decomp src/dds1/field/fldFileResolver.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "dds/sdk_ps2.h"

#include "dds/fld_item_box_object_api.h"

#include "dds/sdf_line_pair_packet_api.h"

#include "dds/sdf_gs_header.h"

#include "dds/sdf_pac_reloc.h"

#include "dds/fld_script_resource.h"

#include "dds/sdf_packet_header.h"

#include "dds/eff.h"

#include "dds/fld_player_scene_state.h"

#include "dds/fld_marker_packet.h"

#include "dds/sdf_gs_blend.h"

#include "dds/common.h"

#include "dds/sdf_map_position_api.h"

#include "dds/pty_status_query_api.h"

#include "dds/file_config_option_api.h"

#include "dds/fld_camera_model_motion_request.h"

#include "dds/fld_elv_camera_scene_api.h"

#include "dds/fld_placement_batch_mode.h"

#include "dds/dds3_object_flags.h"

#include "dds/sdf_vu0.h"

#include "dds/fld_stop_entry.h"

#include "dds/sdf_resource_list_release_api.h"

#include "dds/dds3_background_fade_api.h"

#include "dds/fld_movement_hold_state_api.h"

#include "dds/fld_movement_gate_result.h"

#include "dds/fld_scene_ready_api.h"

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

#include "dds/dds3_draw_buffer.h"

#include "dds/dds3_fade.h"

#include "dds/mnu_title_stream.h"

#include "dds/fld_bill_config.h"

#include "dds/sdf_trig.h"

#include "dds/fld_area_path.h"

#include "dds/fld_npc_motion.h"

#include "dds/fld_npc_palette.h"

#include "dds/btl_stage_task_cleanup.h"

#include "dds/mdl_motion_api.h"

#include "dds/sdf_motion.h"

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

#include "dds/sdf_heap_api.h"

#include "dds/dds3obj.h"

#include "dds/dds3_object_control_slot.h"

#include "dds/evt_world_object_api.h"

#include "dds/dds3_world_value.h"

#include "dds/sdf_texture_file.h"

#include "dds/sdf_texture_primary_buffer_api.h"

#include "dds/evt_action_object.h"

#include "dds/sdf_texture_offset_list.h"

#include "dds/evt_script_model.h"

#include "dds/sdf_model.h"

#include "dds/mdl_flag_api.h"

#include "dds/dds3_world_counter.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/dds3_world_secondary.h"

#include "dds/eff_light.h"

#include "dds/fld_area_work.h"

#include "dds/fld_spawn_collision.h"

#include "dds/evt_world.h"

#include "dds/evt_world_source_transform.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_column_packet_api.h"

#include "dds/sdf_packet_append.h"

#include "dds/sdf_packet_builders.h"

#include "dds/fr_font.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_dev_state.h"

#include "dds/fld_waypoint.h"

#include "dds/fld_inf.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/file_request_api.h"

#include "dds/file_pac.h"

#include "dds/fld_packed_resource_kind.h"

#include "dds/fld_title_banner_menu.h"

#include "dds/field_stage.h"

#include "dds/sdf_pac_work.h"

#include "dds/sdf_primitive_packet_api.h"

#include "dds/evt_unit.h"

#include "dds/evt_unit_color_transition.h"

#include "dds/fpu.h"

#include "dds/pcp_vu0.h"

#include "dds/sdf.h"

#include "dds/sdf_sif_command.h"

#include "dds/mdl.h"

#include "dds/scr.h"

#include "dds/kwln.h"

#include "dds/dds3_task_create.h"

#include "dds/dds3_task_runner.h"

#include "dds/dat_state.h"

#include "dds/dds3_world_primary.h"

#include "dds/sdf_packet_alloc.h"

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

typedef char SdfGsTextureAlphaFlushPacket_payload_offsets_must_match[
    (((u32)&((SdfGsTextureAlphaFlushPacket *)0)->textureAlpha == 0x20) &&
     ((u32)&((SdfGsTextureAlphaFlushPacket *)0)->textureFlush == 0x30)) ? 1 : -1];

/* Declared before first use (were implicit int). */
extern void btlResetAsyncState(void);

extern void fldDrawCameraColorScrollingGradient(void);

extern FldFileResource *D_003BD7B8;

extern u32 D_003BD7BC;

struct dds3Basic_t;

extern struct dds3Basic_t *dds3AddMatterObjectBasic(s32 key, f32 *offset, f32 *rotation);

extern char D_0039FD50[];

struct dds3Basic_t *fldMakeDmyMatter(void);

dds3Basic_t *fldGetMatter_From_ModelRangeID(u32 resourceId);

dds3Basic_t *fldGetModelRange_From_MatterName(const char *resourceEntryName);

struct EffNodeDescriptor;

typedef char FldSpawnBatchSize[(sizeof(FldSpawnBatch) == 12) ? 1 : -1];

extern FldFileResource *D_003BD7B0;

extern u32 D_003BD7B4;

extern u32 D_003C91D0[];

extern struct sdfMemHandle_t *D_003C91E0[];

extern struct EffNodeDescriptor *D_003C91F0[];

extern char D_003BAC40[];

extern dds3Basic_t *effObjCreateResourceKindZero(const char *, void *, s32);

extern dds3Basic_t *dds3AddP2AEffectObjectBasic(const char *, void *, s32);

extern dds3Basic_t *dds3AddTMXEffectObjectBasic3(dds3Basic_t *, void *, s32);

extern dds3Basic_t *dds3AddP2AEffectObjectBasic3(dds3Basic_t *, void *, s32);

extern dds3Basic_t *effObjSpawnDescriptorBoundEffect(struct EffNodeDescriptor *, void *, void *);

extern dds3Basic_t *dds3AddESPEffectObjectBasic(const char *, void *, void *, f32);

extern void dds3EffectObjectBasicStatOn(dds3Basic_t *, u32);

extern void dds3EffectObjectBasicStatOff(dds3Basic_t *, u32);

extern void dds3EffectObjectBasicSetFldEx(dds3Basic_t *, BillConfig *);

extern dds3Basic_t *dds3AddLightObjectBasic(s32);

extern dds3Basic_t *dds3AddPathBasic(s32 key, Dds3PathCurveTable *work, const char *name);

extern dds3Basic_t *dds3AddModelRangeBasicNew(s32, void *, const f32 *, const char *);

extern void dds3GetRadiusPoseSet(dds3Basic_t *, f32);

extern void dds3SetLightObjectResoruce(dds3Basic_t *, WorldTransformSetup *);

extern void fldMakePlane(const FldCollisionDescriptor *geometry, const f32 *translation,
                         u32 id, s32 sceneFlag,
                         FldMapSlotAuxiliaryFlagSelector auxiliaryFlagSelector);

extern s32 fldCommand_GetHojiID(const char *);

extern void dds3SetPathBasicSpline(dds3Basic_t *path, u32 value);

void fldMakeResouceBasicFL1(FldSpawnBatch *batch, u32 batchCount);

extern FldInfTable D_00332E30;

/* Camera-setting table capacity. */
enum {
    FIELD_CAMERA_SETTING_COUNT = 8
};

typedef struct SdfDrawPacket SdfDrawPacket;

typedef struct DmaPacketHeader DmaPacketHeader;

extern void fldPlayFieldSeVolumePan(s32);

extern void dds3FadeColorSet(s32 red, s32 green, s32 blue);

extern void fldReleaseActorTasksById(s32);

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

typedef char FldActorRow_secondaryMotion_offset_must_be_0x10[
    ((u32)&((FldActorRow *)0)->secondaryMotion == 0x10) ? 1 : -1];

typedef char FldActorRow_size_must_be_0x5C[sizeof(FldActorRow) == 0x5C ? 1 : -1];

extern FldActorRow fldActorSlots[];

extern s32 D_0032E530[];

extern FldNpcMotion D_00336A60[];

extern char *D_003BAE44;

extern char *D_003BAE48;

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

extern void fldSpawnActionObjects(const FldActionSpawn *, u32);

extern s32 D_0032E400[];

extern void fldClearCameraObjectHighlightFlag(void);

extern void fldSetCameraObjectHighlightFlag(void);

extern s32 fldApplyPlayerVerticalStep(void);

extern void fldUpdateModeOneCameraPose(void);

extern void fldDispatchCameraModelFrameEvents(s32 motion, s32 selector);

extern s32 fldUpdateFieldPlayerMovement(void);

extern void fldUpdateCameraModelMotion(s32 currentMotionIndex, s32 requestedMotionIndex, f32 blendFrames);

extern void dds3CameraObjectBasicSetFovy(dds3Basic_t *, f32);

extern void fldToggleWorldNodeState(s64);

extern void fldProbeCameraSupportSurface(void);

extern void fldRestoreCameraModelColor(void);

extern void fldUpdateCameraFollowPlayer(void);

extern void fldCamUpdateFollow(void);

extern void fldUpdateCameraFollowLinkedObject(void);

extern void fldCamElvTask(void);

extern u8 fldEncountTestFlag(u32);

extern f32 fldPointDistance(f32 ax, f32 ay, f32 az, f32 bx, f32 by, f32 bz);

extern void fldClearCameraModelColor(void);

extern void fldSetCameraObjectTransitionFlag(void);

extern void fldClearCameraObjectTransitionFlags(void);

extern FldCamRow fldCameraFollowRows[];

extern f32 D_00330630[];

extern s32 D_003BAD24;

extern s32 D_003BAD28;

extern f32 D_00330650[];

extern f32 D_00330660[];

extern void btlActivateRuntime(s32 mode);

extern void *sdf2DMakeStdPacket(void *, s32);

extern s32 sdf2DGetPrimPacketSize(s32 registerCount, s32 loopCount);

extern u8 *sdf2DGetPrimPtr(SdfDrawPacket *packet);

extern s32 D_0032E3C0[];

extern s8 D_0032C9A0[];

extern void fldReleaseSkyResources(void);

extern void fldCreatePlayerObject(void);

extern const char *fldSelectCurrentActorOnNextFloor(void);

extern void fldSpawnPlacementObjectsFromBatches(FldSpawnBatch *spawnBatches, u32 batchCount,
                                                FldPlacementBatchMode mode);

extern s32 D_003BAE68;

extern FldWallMeshRecord *fldItemBoxObjBuf;

extern collision_s *fldMatterTable;

extern struct sdfMemHandle_t *fldAuxRecordResource;

extern struct sdfMemHandle_t *fldValueRecordResource;

extern u32 gFilterAlpha;

extern u32 gFilterAnime;

extern f32 gFilterAnimeCntF;

extern s32 gFilterAnimeOfsA;

extern SdfFlagListWork *fldCameraColorEffect;

extern u32 fldCameraColorEnabled;

extern sdfTexHandle_t *fldRainTextureReference;

extern struct sdfMemHandle_t *fldRainTextureResource;

extern dds3ModelHandle_t *fldPlayerModelContext;

extern dds3Basic_t *fldPlayerObject;

extern void dds3UnitObjectBasicEL_LinkFree(dds3Basic_t *spawnedObject);

extern u8 D_003BAB3C;

extern FldCameraModelMotionRequest D_003BAD34;

extern u32 D_003BAD38;

extern u32 D_003BAD3C;

extern u32 D_003BAD40;

extern FldElvCameraSceneMode D_003BAD1C;

extern u32 D_003BACF8;

extern s32 fldDrwZBuf;

extern u32 rsetFrameMaskOtOffset;

extern FldFileAddrResetMode gFldFileResetMode;

extern u32 fldPendingArea;

extern u32 fldPendingFloor;

extern u32 D_003BAD98;

extern u32 D_003BAD9C;

extern f32 D_003BADA0;

extern u32 D_003BADA4;

extern u32 D_003BADC8;

extern u32 D_003BADD8;

extern u32 D_003BAE28;

extern s32 gFldInf_HitIdx;

extern u32 fldSelectedActorEntryIndex;

extern u32 D_0032E428[];

extern u32 D_0032E538[];

extern u32 D_0032E544[];

extern u32 fldRunDanteGunFlag[];

extern s32 D_003BAE38;

extern u32 D_0032E59C[];

extern u32 D_0032E5A8[];

extern sdfGrOt_t sdfGrOtTable[];

extern u8 sdfViewMatrix[];

extern u32 D_003BACD4;

extern u32 D_003BACEC;

extern sdfTexHandle_t *fldMarkerTexture;

extern u32 D_003BD7C0;

extern u8 D_0032F260[];

extern sdfMat_t *sdfMatNew(void);

extern void fldClrCollision(void);

extern u8 sdfProjectionMatrix[];

extern u8 D_00324660[];

extern char D_003C9200[];

extern s16 D_003C9518[];

extern void fldFormatAreaResourceName(char *out);

extern void fldApplySkyLightSet(u32 skyLightSetIndex, s32 durationMode);

extern f32 D_003BAD20;

extern char fldEncounterTaskName[];

extern f32 D_003C9230[];

extern f32 D_003C9220[];

extern s32 fldEncProc(dds3ProcessID_t *task);

extern void fldResetEncounterAsyncState(dds3ProcessID_t *task);

extern s32 btlUpdateRuntimeFadeState(void);

extern void btlClearRuntimeState(void);

extern void dds3CameraObjectBasicGetTgtCam(dds3Basic_t *camera, f32 *worldEyeOut,
                                                      f32 *targetPositionOut);

extern f32 sdfAtan2(f32 y, f32 x);

extern FilePacRequest *pFldNextFileDataF2;

extern u8 D_003BACE0[];

extern u8 D_003BACE8[];

extern u32 D_003BACFC;

extern u32 D_00330738[];

extern u32 D_003306B0[];

extern f32 D_003306C0[];

extern f32 D_00330670[];

extern s32 fldUpdateTitleBannerTask(dds3ProcessID_t *task);

extern s8 btlGetRuntimeResult(void);

extern s32 fldValueRecordCount;

extern s32 D_003BADF8;

extern s32 D_003BADEC;

extern FldFileIoPriorityState D_003BA734;

extern struct sdfMemHandle_t *fldFieldPackData;

extern struct FileRequest *gFldFileF1;

extern u8 D_003BAC90[];

extern s32 D_0032E4C8[];

extern s32 D_003BAE30;

extern s32 gFldEveRngIdx_N;

extern s32 fldEveRngCount;

extern s16 D_003C9510[];

extern u32 D_003BAE4C;

extern s32 D_003BAE50;

extern void fldDestroyTitleTask(void);

extern void fldReleaseObjectSlots(void);

extern void fldReleaseTitleSlots(void);

extern void fldReleaseResourceHandles();

extern u8 fldTitleMiniIsActive(void);

extern void fldResetZoneRecordsAndActorSlots(void);

extern void fldSe_Init(void);

extern void fldReleaseMenuSlotsAfterWait(void);

extern void mnuReleaseResourceEntries(void);

extern void fldDrwZFree(void);

extern fldWaypointBlock_t fldActorWaypointRows;

extern FldFileResource *fldRoomResources[];

extern void fldDrawPos(const f32 position[3]);

extern FldIndexPair D_0032EF18[];

extern FldIndexPair D_0032EFE0[];

extern s32 D_003BAE54;

extern s32 D_003BAE5C;

extern s32 D_003BAE60;

extern u32 D_003C91D0[];

extern struct EffNodeDescriptor *D_003C91F0[];

extern struct sdfMemHandle_t *D_003C91E0[];

extern char D_003BAC40[];

extern void fldMakeResouceBasicFL1(FldSpawnBatch *, u32);

extern FldFileResource *D_003BD7B8;

extern u32 D_003BD7BC;

extern s32 fldRoomEventPlacementCount;

extern s16 D_0032E4B4[];

extern s32 fldCommand_GetHojiID(const char *);

extern void fldMakePlane(const FldCollisionDescriptor *geometry, const f32 *translation,
                         u32 id, s32 sceneFlag,
                         FldMapSlotAuxiliaryFlagSelector auxiliaryFlagSelector);

extern dds3Basic_t *dds3AddModelRangeBasicNew(s32, void *, const f32 *, const char *);

extern void dds3SetPathBasicSpline(dds3Basic_t *path, u32 value);

extern dds3Basic_t *dds3AddEventBasic(s32 key, const char *resourcePath, const char *name);

extern dds3Basic_t *dds3AddPathBasic(s32 key, Dds3PathCurveTable *work, const char *name);

#include "dds/fld_event_display_resource_api.h"

extern void func_00148FF0(s32, u32, f32 *, f32, f32, f32);

extern void func_001480D0(u32, f32 *, f32, f32, f32, s32, const char *, s32);

extern void func_001486D0(u32, f32 *, f32, f32, f32);

extern s32 fldSetSparkVectors(s32 index, const u128 *position, const u128 *rotation);

extern void fldAddSe3D(FldSpatialSoundId soundId, f32 radius, f32 sourceX, f32 sourceY, f32 sourceZ);

extern dds3Basic_t *dds3AddCameraObjectBasicTypeFree(s32, u128 *, u128 *);

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

void fldSpawnActionObjects(const FldActionSpawn *spawnList, u32 spawnCount);

extern FldFileResource *D_003BD7B0;

extern u32 D_003BD7B4;

extern FldFileResource *D_003BD7B8;

extern u32 D_003BD7BC;

extern struct DevRequest *fldRoomSoundResources;

extern dds3Basic_t *dds3AddMatterObjectBasic(s32 key, f32 *offset, f32 *rotation);

extern void dds3MatterObjectBasicSetOt(dds3Basic_t *, s32);

extern s32 fldCommand_GetHojiID(const char *);

extern void dds3MatterObjectBasicSetHojiID(dds3Basic_t *, u32);

extern void dds3ObjectBaseMakeMatter(dds3Basic_t *);

extern s32 dds3ObjectBaseRegistArea(dds3Basic_t *);

void fldCreateResourceScriptObjects(void);

extern u8 fldTestMapSlotValueFlag(s32 mapId, u32 slotIndex, s32 bit);

extern u8 fldTestRoomModeFlag(s32 mapId, u32 slotIndex, s32 bit);

extern void dds3ObjectBaseCreateMover(dds3Basic_t *);

extern void dds3MoverBasicMakePath(dds3Basic_t *object);

extern void evtPathRate(dds3Basic_t *object, f32 multiplier);

extern void dds3MoverBasicUpdate(dds3Basic_t *);

extern void dds3MatterObjectBasicSetAlpMode(dds3Basic_t *, u32);

extern char D_0039FE08[], D_0039FE18[], D_0039FE28[];

/* Restore the room's named objects and enable their retained path controllers. */
void fldMakeObjectBasic3(void);

void fldSetFileResolverIdx(FldLightSetIndex initialLightSetIndex);

void fldLoadSceneRequestFiles(const FldLoadRequest *request);

/* Handle a field request, creating the player only in non-special scene states. */
void fldProcessFieldRequest(const FldSceneRequest *request);

void fldProcessFieldRequestAlternate(const FldSceneRequest *request);

u8 *fldGetInfoHeader(const FldTransferChunk *chunk);

void sdfAddressResolverReset(u32 *table, u32 base, const u8 *data, s32 size);

/* Relocate the words described by this packed-resource transfer chunk. */
void fldAddressResolver(u8 *buffer, const FldTransferChunk *chunk);

void fldRelocateTransferChunkWords(u8 *buffer, const FldTransferChunk *chunk);

void fldLoadNextFile_SetId_BtlRet(u32 area, u32 floor);

extern char D_0039FE38[];

extern void fldFreeNextFile(void);

FldBattleReturnAreaLoadResult fldTryStartPendingBattleReturnAreaLoad(void);

extern void sdfFileCmdThreadSetLoPrio(void);

extern s32 D_003BAC80;

s32 fldRequestAreaResource(s32 area, s32 room);

void fldFreeNextFile(void);

u32 fldPollAreaResourceLoad(void);

FldNextFileLoadState fldGetNextFileLoadState(void);

FldAreaResourceReadyState fldIsAreaResourceReady(void);

FldAreaResourceRequestReuseResult fldCanReuseAreaResourceRequest(s32 area, s32 room);

extern const u8 *fldCachedRoomResourceData, *fldCachedRoomF1ResourceData, *fldCachedRoomF2ResourceData,
    *fldCachedRoomKF2ResourceData;

extern u32 fldCachedRoomResourceSize, fldCachedRoomF1ResourceSize, fldCachedRoomF2ResourceSize, fldCachedRoomKF2ResourceSize;

struct sdfMemHandle_t *fldCopyCachedRoomResourceIfLocationMatches(const SdfTextureOffsetListHeader **destination, s32 area, s32 room);

struct sdfMemHandle_t *fldCopyCachedRoomF1ResourceIfLocationMatches(u8 **destination, s32 area, s32 room);

struct sdfMemHandle_t *fldCopyCachedRoomF2ResourceIfLocationMatches(u8 **destination, s32 area, s32 room);

struct sdfMemHandle_t *fldCopyCachedRoomKF2ResourceIfLocationMatches(u8 **destination, s32 area, s32 room);

void fldFormatAreaResourceName(char *out);

FldAreaResourceNameChangeResult fldHasAreaResourceNameChanged(void);

extern void fldGetDoorEntry(const fldWaypointBlock_t *source);

extern void fldInfLoadFromMem(const FldInfTable *source);

extern void fldAdoptSceneRecordChunk(u8 *chunk, struct sdfMemHandle_t *resourceHandle);

void fldLoadFieldPack(void);

void fldFreePac0(void);

extern s32 fldRoomEventPlacementCount;

extern FldFileAddrResetMode gFldFileResetMode;

extern s32 gFldInf_WpExit;

extern FldTransferChunk *fldRoomF1Chunk;

extern u8 *fldRoomF1Data;

extern FldTransferChunk *fldRoomF2Chunk;

extern u8 *fldRoomF2Data;

extern FldTransferChunk *fldRoomKF2Chunk;

extern u8 *fldRoomKF2Data;

extern FilePacRequest *D_003BAC38;

extern struct sdfMemHandle_t *D_003BAC4C;

extern struct sdfMemHandle_t *fldCachedRoomResourceAllocation;

extern struct sdfMemHandle_t *fldCachedRoomF1ResourceAllocation;

extern struct sdfMemHandle_t *fldCachedRoomF2ResourceAllocation;

extern struct sdfMemHandle_t *fldCachedRoomKF2ResourceAllocation;

extern struct sdfMemHandle_t *fldFieldPackData;

extern void fldSetSceneLocation(s32 area, s32 floor, s32 stage);

extern void fldLoadPlayerModel(void);

extern void fldInitSparkTable(void);

extern void fldInitializeMenuResources(void);

extern void fldUpdateSparkSlots(void);

extern void fldMakeObjectBasic3(void);

extern void fldInitializeActorMotionSlots(void);

extern DevRequest *sdfDevLoadTexList(const char *name);

extern char D_003BAC98[];

extern char D_003BACA0[];

extern char D_003BACA8[];

extern FieldPlayerSceneWork fldSceneParam;

extern void fldDrwZAlloc(void);

extern void fldCreateResourceScriptObjects(void);

extern void fldLoadFieldPack(void);

extern void fldFreePac0(void);

extern const char D_0039FF18[16];

const struct sdfMemHandle_t *fldLoadAreaRoomResources(const char *resourceName);

extern s32 fldRoomEventPlacementCount;

extern FldTransferChunk *fldRoomF1Chunk;

extern u8 *fldRoomF1Data;

extern FldTransferChunk *fldRoomF2Chunk;

extern u8 *fldRoomF2Data;

extern FldTransferChunk *fldRoomKF2Chunk;

extern u8 *fldRoomKF2Data;

extern FilePacRequest *D_003BAC38;

extern struct sdfMemHandle_t *D_003BAC4C;

extern struct sdfMemHandle_t *fldCachedRoomResourceAllocation;

extern struct sdfMemHandle_t *fldCachedRoomF1ResourceAllocation;

extern struct sdfMemHandle_t *fldCachedRoomF2ResourceAllocation;

extern struct sdfMemHandle_t *fldCachedRoomKF2ResourceAllocation;

void fldReleaseFieldResources(void);

/* All 25 code words match. Check_unit rodata range extends beyond selector strings into the separately-owned next string. */
void fldMakeFilePath(char *path, s32 field, s32 unused);

void fldDbgPrint2D(s32 x, s32 y, const char *format);

extern u8 fldTestRoomSceneFlag(s32 mapId, u32 slotIndex, s32 bit);

extern char D_003BACB0[];

extern char D_003BACB8[];

extern char D_003BACC0[];

extern char D_0039FF88[];

/* Debug view of the area's map-slot values (GMC) and room scene flags (HIT):
 * one row per floor 1..31 and one column per slot 0..15, "." for clear. Each
 * draw recomputes the row offset: GCSE PRE merges the `floor * 12 - 0x4C`
 * terms into one reaching register but leaves every `<< 3` per call, as retail does. */
void fldHojiDebugView(void);

extern u8 fldTestRoomObjectModeFlag(s32 mapId, u32 slotIndex, s32 bit);

extern char D_003BACB0[];

extern char D_003BACB8[];

extern char D_003BACC0[];

void fldHojiDebugView2(void);

/* Enable or skip relocation of remaining transfer chunks during field teardown. */
void fldFileAddrResetMode(FldFileAddrResetMode mode);

void fldDrawInit(void);

sdfGrOt_t *fldDrawGetOt(void);

void DrwSprite2D(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh, s32 color, sdfTexHandle_t *texture);

void DrwSprite2DC(s32 x, s32 y, s32 width, s32 height,
                   s32 u, s32 v, s32 textureWidth, s32 textureHeight,
                   u32 color0, u32 color1, u32 color2, u32 color3, sdfTexHandle_t *texture);

void DrwSprite2DF(f32 x, f32 y, f32 w, f32 h, s32 u, s32 v, s32 uw, s32 vh, s32 color, sdfTexHandle_t *texture);

extern u8 D_00324650[];

void fldPoint3To2(f32 *dstX, f32 *dstY, f32 x, f32 y, f32 z);

extern u8 D_003249B0[];

extern u8 D_003249F0[];

extern u8 D_00324A00[];

extern u8 D_00329790[];

void fldPoint3To2H(f32 *dstX, f32 *dstY, f32 x, f32 y, f32 z);

/* vu0 routine: prepare the shared projection and secondary matrix bank. */
void fldPrepareProjectionMatrix(void);

void fldProjectPointWithPreparedMatrix(f32 *dstX, f32 *dstY, f32 x, f32 y, f32 z);

void rsetOtOfs(u32 displayRow);

void rsetTexa(s32 lower, s32 bits, u64 upper);

void rsetTest(s32 bit0, s32 bit1, s32 bit4, s32 bit12, s32 bit14, s32 bit15, s32 unused, s32 bit17);

/* Write the selected blend equation to GS ALPHA_1. */
void rsetAlpha(s32 mode);

void aDrawLineStripPrim(s32 x0, s32 y0, s32 x1, s32 y1, u32 gsWord0, u32 gsWord1, u32 gsWord2);

void aDrawLineDirect(u32 x0, u32 y0, u32 x1, u32 y1, u32 gsWord0, u32 gsWord1, u32 gsWord2);

void aDrawBoxF(s32 x, s32 y, s32 w, s32 h, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3);

void aDrawBoxFZ(s32 x, s32 y, s32 w, s32 h, u32 vertexTag, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3);

void aDrawBoxF_Direct(s32 x0, s32 y0, s32 x1, s32 y1, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3);

void aDrawTriG(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2);

void fldSubmitGsGradientQuad(s32 x, s32 y, s32 w, s32 h, u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2, u32 r3, u32 g3, u32 b3, u32 a3);

void aDrawBoxGF(u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2, u32 r3, u32 g3, u32 b3, u32 a3, f32 x, f32 y, f32 w, f32 h);

void fldSubmitTaggedGsRectangle(s32 x, s32 y, s32 w, s32 h, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3, u32 vertexTag);

extern void sdf3DMakeDirectRefPacket(DmaPacketHeader *, u32, s32);

extern void sdfBillBoardMakeTMPacket(u8 *, const f32 *);

extern void sdfBillBoardMake1(u8 *, const f32 *, u32, const BillTextureQuad *, const f32 *);

void drawSprite3D(sdfTexHandle_t *texture, FldMarkerPacket *modelData);

extern dds3FrameDrawBuffer_t dds3FrameDrawPacketRecords[];

extern void sdfGrSetFrameMaskRGB(const u8 *, SdfDmaReferenceChainPacket *, s32);

extern void sdfGrSetFrameMaskReset(const u8 *, SdfDmaReferenceChainPacket *, s32);

void fldSubmitPrimaryFramePacket(void);

void fldSubmitAlternateFramePacket(void);

extern f32 D_0032F4E0[][4];

extern u32 D_0032F500[];

void ooyTestLine3D(u32 first, u32 second, f32 x, f32 y, f32 z, f32 u, f32 v, f32 w);

void fldDrawPoly3(s32 color0, s32 color1, s32 color2,
                         f32 x0, f32 y0, f32 z0,
                         f32 x1, f32 y1, f32 z1,
                         f32 x2, f32 y2, f32 z2);

/* Submit the field overlay's fixed register state and textured sprite payload. */
void fldSubmitTexturedOverlaySprite(void);

/* Submit the field overlay's fixed register state and textured sprite payload. */
void fldSubmitOverlaySpriteWithRenderState(s32 mode);

/* Submit the field overlay's fixed register state and textured sprite payload. */
void fldSubmitOverlayStateAndSprite(s32 alpha);

void fldDrawPos(const f32 position[3]);

void fldDrawMarkerQuadColored(const f32 position[3], s32 color);

extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);

void fldDraw2DBar(s32 x, s32 y, s32 width, s32 height);

void fldDrwZAlloc(void);

void fldDrwZFree(void);

extern sdfGrFrameSet_t sdfGrFrameSet;

void fldDrwZSave(void);

void fldDrwZLoad(void);

void fldKanjiDraw();

void LF(FldQuadState *quad);

void initDrawTag(FldQuadState *quad);

extern sdfGrOt_t D_00325708;

extern sdfGrOt_t sdfDebugGrOt;

extern void InversMatrix(void);

void fldDbgPrint3D(f32 x, f32 y, f32 z, s32 drawValue);

void fldDbgPrint2D(s32 x, s32 y, const char *format);

void fldDbgPrint2DC(s32 x, s32 y, s32 packetField, s32 drawValue);

void fldDbgPrintNum2D(s32 x, s32 y, s32 drawValue);

void fldDbgPrintNum2DH(s32 x, s32 y, s32 drawValue);

void fldDbgPrintStr2D(s32 x, s32 y, s32 drawValue);

void fldDbgPrintStr2DC(s32 x, s32 y, s32 packetField, s32 drawValue);

void fldDbgPrintStr2DCF(s32 packetField, s32 drawValue, f32 x, f32 y);

void fldDrawMaruKage(u32 fade, f32 x, f32 y, f32 z, f32 radius);

void fldSubmitAreaOverlayQuads(s32 alpha, s32 offset);

FldEncounterRuntimeResult fldGetEncounterRuntimeResult(void);

void fldSetEncounterPendingValue(u32 pendingValue);

dds3TaskUpdateResult_t fldEncProc(dds3ProcessID_t *task);

extern void p2sdPlaySE(s32 sequence, s32 volume, s32 pan);

void fldRequestEncounterWithFade(FldEncounterRuntimeMode encounterMode,
                                 FldBattleSceneRecordIndex recordIndex);

s32 fldSetEncounterMode(FldEncounterRuntimeMode encounterMode);

void fldResetEncounterAsyncState(dds3ProcessID_t *task);

void fldCreateEncounterTask(void);

extern f32 fldLookAtNearPoint[];

extern f32 fldCamTarget[];

extern f32 D_0032E41C[];

void fldUpdateLookAtSegmentDistance(void);

extern f32 fldCamCalcSlopeOffset(f32 x, f32 z, f32 *xs, f32 *ys, f32 *zs);

/* Probe the support surface under the camera's focus and its mirrored heading. */
void fldProbeCameraSupportSurface(void);

extern f32 EulerAngleAxisY(void);

extern f32 D_003BAD10, D_003BAD14, D_003BAD18;

typedef union FldLookAtVector {
    u128 q;
    f32 f[4];
} FldLookAtVector;

void fldUpdateFieldCameraViewState(void);

/* Follow the player with the field camera and pull the far point back to the collision hit. */
void func_0012CED0(void);

void fldUpdateCameraProjectionEndpoints(void);

void fldCamUpdateFollow(void);

extern s32 D_003BAB50;

extern s32 D_003BAD30;

extern void dds3CameraObjectBasicSetWorldEye(dds3Basic_t *camera, const u128 *worldEye);

void fldUpdateCameraFollowPlayer(void);

extern void dds3CameraObjectBasicSetWorldEye(dds3Basic_t *camera, const u128 *worldEye);

extern f32 GetSphericalPosMove(f32 fromX, f32 fromZ, f32 toX, f32 toZ);

extern f32 EulerAngleAxisY(void);

extern void dds3GetTMPoseSet(dds3Basic_t *object);

void fldUpdateCameraViewFromPlayer(void);

/* Suppress the world object's current entry when it is already selected. */
dds3Basic_t *fldCamNowEveCamera(void);

void fldSetElvCameraScene(FldElvCameraSceneMode mode);

extern void dds3OnPoseSetFlag(dds3PoseSet_t *, u32);

void fldCamElvTask(void);

void fldClearCameraMoveMode(void);

typedef struct FldModelMatrices {
    u8 pad00[0xC];
    u8 **rows; /* 0x0C: slot-indexed matrix blocks */
} FldModelMatrices;

void fldUpdateCameraProximity(void);

s32 fldUpdateCameraFollow(void);

void fldResetCameraModelHandles(void);

void fldReleaseCameraModel(u32 enabled);

void fldSetCameraModelMotionRequest(FldCameraModelMotionRequest request,
                                    u32 frameCountdown);

void fldUpdateCameraModelMotion(s32 currentMotionIndex, s32 requestedMotionIndex, f32 blendFrames);

struct EffRandState;

extern u32 dds3GetRandA(struct EffRandState *state);

extern void fldSetSequenceVolume(s32 category, s32 volume);

extern void fldQueuePrimaryEffectPosition(f32 x, f32 y, f32 z);

extern void fldQueueSecondaryEffectPosition(f32 x, f32 y, f32 z);

extern s32 D_003BAD44;

/* Dispatch camera-model frame crossings to sequence sounds and queued effects. */
void fldDispatchCameraModelFrameEvents(s32 motion, s32 selector);

extern s32 D_003BAB50;

extern void fldHitChkUnitElv(f32 *);

void fldUpdateCameraTarget(void);

extern const PcpVectorCopyF32 D_003A0048;

extern f32 fldAngleDifference(f32 fromAngle, f32 toAngle);

extern void FromRotation(f32 angle);

extern void fldRefreshAreaStateFromPosition(f32 *);

/* Follow the selected mode-1 heading while preserving field position history.
 * The four legacy SDK stores are qualified only for this pinned emitted body;
 * each vector consumer and conditional join requires a fresh audit on changes. */
void fldUpdateModeOneCameraPose(void);

/* Signed verticalStepDirection: adjust player Y by -2/+2 and save previous XYZ. */
/* Publish the vertical step to field state, then rebuild the object vector.
 * Each branch-local SDK snapshot dominates all of its scalar reads; there is
 * no earlier scratch read or loop-carried value. The finite qualification
 * and memory-contract limitation are documented in docs/idioms.md. */
s32 fldApplyPlayerVerticalStep(void);

extern void mnuTitleStreamUpdateAndLogBgm(void);

extern void dds3PadVibrationSet(u32 motor, u8 level, s32 duration);

extern void fldEncountSetFlag(u32 mask);

/* Advance the field transition while the camera model's motion drives completion. */
FldMovementGateResult fldUpdateFieldTransition(void);

FldMovementGateResult fldUpdateAreaDamageReaction(void);

extern s8 dds3PadButtonState[2][2][16];

extern u8 D_003BA870[2][2][2];

extern s32 D_003BAD2C;

extern s32 D_003BAD30;

extern s32 D_003BAD48;

extern s32 D_003BAD4C;

extern f32 D_003BAD50;

extern s32 D_003BAD54;

extern f32 D_0032F170[12];

extern f32 fldSnapAngleToCompassPoint(f32);

extern f32 fldPlayerCalc360_8(f32);

extern f32 fldPlayerCalc360_4(f32);

extern u8 fldIsNearSpark(f32 x, f32 y, f32 z);

extern void fldHitChkUnitDamage(f32 *);

extern void fldRestoreCameraModelColor(void);

extern void fldClearCameraModelColor(void);

extern void fldDisableCameraObjectFlag(void);

extern void fldSetCameraObjectTransitionFlag(void);

extern void fldSetCameraObjectHighlightFlag(void);

s32 fldUpdateFieldPlayerMovement(void);

/* Field camera model: node id at +0x12, render-data pointer at +0x18. */
s32 fldUpdateCameraFrame(void);

FldMovementHoldState fldGetMovementHoldState(void);

void fldSetCameraNodeModeWithZero(void);

void fldEnableCameraObjectFlag(void);

void fldDisableCameraObjectFlag(void);

void fldClearCameraObjectHighlightFlag(void);

void fldSetCameraObjectHighlightFlag(void);

void fldClearCameraModelColor(void);

void fldRestoreCameraModelColor(void);

void fldSetCameraObjectTransitionFlag(void);

void fldClearCameraObjectTransitionFlags(void);

extern void dds3OffPoseSetFlag(dds3PoseSet_t *, u32);

/* State-first XYZ publication; SDK snapshot audit: docs/idioms.md. */
s32 fldUpdateCameraObjectPose(void);

extern void fldUpdateCameraModelMotion(s32 currentMotionIndex, s32 requestedMotionIndex, f32 blendFrames);

void fldSetCameraNodeModeWithTen(void);

void fldSetCameraNodeModeWithZero(void);

void fldAddCameraModelEntry(s32 motionIndex);

void fldAddCameraModelPair(s32 firstMotionIndex, s32 secondMotionIndex);

void fldClearCameraFacingPointRequest(void);

void fldClearCameraHeadingRequest(void);

/* Camera facing requests share the field-work block. Both request states
 * advance from 1 to 2 when their new angle is installed. */
void fldQueueCameraXYOverride(f32 targetX, f32 targetZ);

void fldQueueCameraHeadingFromVector(f32 x, f32 unusedY, f32 z);

void fldApplyCameraFacingPoint(void);

void fldApplyPendingCameraHeading(void);

extern const PcpVectorCopyF32 D_003A00A8;

/* Advance a pending heading, retain position history, and export the player's
 * smoothed position and rotation. Vertical movement bypasses position damping. */
void fldUpdateSmoothedCameraPose(void);

extern SdfFlagListParams fldCameraColorParameters[];

extern FldCameraSetting *fldCameraSettings;

extern FldCameraSetting D_003306D0;

extern FldLightSet *D_003BAD60;

extern FldSkyGradientRecord *D_003BAD74;

extern s32 D_003BAD88, D_003BAD8C;

extern f32 D_003BAD90, D_003BAD94;

void fldLoadBattleSkyAndFilter(void);

extern char D_003A0100[];

extern u32 fldRainTextureData;

void fldLoadSkyResource(s32 area);

void fldReleaseSkyResources(void);

void fldUploadSkyBuffer(const FldSkyBuffer *src);

enum {
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_1_OFFSET_32 = 1,
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_2_OFFSET_32 = 2,
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_05_OFFSET_32 = 3,
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_1_OFFSET_48 = 4,
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_2_OFFSET_48 = 5,
    FLD_SKY_FILTER_MODE_PHASE_STEP_0_05_OFFSET_48 = 6,
};

void fldSkyCalcFilter(void);

/* Draw the current filter grid, blending the previous grid during a fade. */
void fldSkyDrawFilter(void);

extern s32 D_003BAD58;

void fldSetFadeTarget(s32 area, s32 targetMode, s32 duration);

void fldSetFilterAnime(u32 mode);

void fldSkySetAlpha(u32 drawState);

u32 fldSkyGetAlpha(void);

void fldSkySetAlphaOfs(u32 alpha);

u32 fldSkyGetAlphaOfs(void);

void fldSkySetTableAnime(u32 transitionMode);

extern char D_003A0138[];

/* Alternate the selected value at the requested rate, and apply live lighting edits. */
void fldUpdateSelectedValueTransition(void);

/* VU0 direction setters consume vf10, as in the DDS2 lighting routine. */
void fldSetPlayerLighting(s32 duration, f32 redA, f32 greenA, f32 blueA,
                   f32 redB, f32 greenB, f32 blueB,
                   f32 x, f32 y, f32 z);

extern void dds3SetFogColor(s32, f32 *);

extern void dds3SetLightColor(s32 blendFrames, s32 lightIndex, const f32 *color);

extern void dds3SetAmbientColor(s32, f32 *);

extern void dds3SetLightDir(s32 blendFrames, s32 lightIndex, const f32 *direction);

extern void dds3SetFogPara(s32 blendFrames, f32 x, f32 y, f32 z, f32 w);

extern void fldSetFilterAnime(u32);

extern u32 D_003BADA8;

extern void fldSetPlayerLighting(s32 duration, f32 redA, f32 greenA, f32 blueA,
                          f32 redB, f32 greenB, f32 blueB, f32 x, f32 y, f32 z);

/* Start fading to the sky light set at index. A zero scene command also applies the set to the
 * display state and player lighting using the (clamped) transition duration. */
void fldApplySkyLightSet(u32 skyLightSetIndex, s32 durationMode);

extern s32 D_0032E3C0[];

extern s16 D_0032E4D8[];

extern s32 D_003BAD58;

void fldApplyPendingSceneValueWithSpeed(s32 speed);

/* The resolver's selected-light transition cache is stored in these existing
 * D1 sdata words; the light record itself is the complete 0xE0-byte owner. */
extern u32 D_003BADA8;

extern u32 D_003BADAC;

extern f32 D_003BADB0;

extern s32 D_003BADB4;

extern s32 D_003BADB8, D_003BADBC;

extern f32 D_003BADC0, D_003BADC4;

extern u32 D_003BADCC;

extern f32 D_003BADD0;

extern u32 D_003BADD4;

extern void dds3SetFogColor(s32, f32 *);

extern void dds3SetLightColor(s32 blendFrames, s32 lightIndex, const f32 *color);

extern void dds3SetAmbientColor(s32, f32 *);

extern void dds3SetLightDir(s32 blendFrames, s32 lightIndex, const f32 *direction);

extern void dds3SetFogPara(s32 blendFrames, f32 x, f32 y, f32 z, f32 w);

void fldTransitionSkyLightSet(s32 lightSetIndex, s32 mode);

void fldSkySetTblLightMaDZ(s32 enabled);

/*BEGIN fldApplySceneLightSet*/
extern void dds3SetFogColor(s32, f32 *);

extern void dds3SetLightColor(s32 blendFrames, s32 lightIndex, const f32 *color);

extern void dds3SetAmbientColor(s32, f32 *);

extern void dds3SetLightDir(s32 blendFrames, s32 lightIndex, const f32 *direction);

extern void dds3SetFogPara(s32 blendFrames, f32 x, f32 y, f32 z, f32 w);

extern void fldSetFilterAnime(u32);

extern u32 D_003BADDC;

extern void fldSetPlayerLighting(s32 duration, f32 redA, f32 greenA, f32 blueA,
                          f32 redB, f32 greenB, f32 blueB, f32 x, f32 y, f32 z);

void fldApplySceneLightSet(s32 lightSetIndex, s32 durationMode);

/* Select the scene light set and reset transient light overrides. */
void fldSetFileResolverIdx(FldLightSetIndex initialLightSetIndex);

void fldInitializeDisplayPointerTable(void);

extern void dds3SetFogColor(s32, f32 *);

extern void dds3SetLightColor(s32 blendFrames, s32 lightIndex, const f32 *color);

extern void dds3SetAmbientColor(s32, f32 *);

extern void dds3SetLightDir(s32 blendFrames, s32 lightIndex, const f32 *direction);

extern void dds3SetFogPara(s32 blendFrames, f32 x, f32 y, f32 z, f32 w);

extern void fldSetFilterAnime(u32);

void fldApplySkyLightSetToPlayerVU(void);

void fldSkyBattle(void);

void fldApplyLightSetIndex(s32 index);

extern FldCameraSetting fldAppliedCameraSettings[];

void fldActivateCameraColorSetting(s32 enable);

extern s32 fldCameraColorFade;

s32 fldComposeFadeColor(s32 fade, s32 color, s32 alpha);

extern char D_003A0100[];

extern u32 fldRainTextureData;

void fldInitializeCameraColorResource(void);

void fldDrawCameraColorLayerPass(void);

void fldReleaseCameraColorEffect(void);

void fldCopyCameraSetting(FldCameraSetting *destination);

void fldUpdateCameraColorEffect(const FldCameraSetting *setting);

/* Keep both the resource handles and retained addresses: callers use the
 * retained storage, whereas the handles are needed at release time. */
void fldAllocPlaneTable(void);

/* Release both retained resources and clear the usable buffer addresses. */
void fldFreeMatterTable(void);

/* Dot product of XYZ only; any fourth vector component is ignored. */
float fldVecDot(const float *left, const float *right);

/* Return the Euclidean XYZ length without normalizing the input vector. */
f32 MATH_VectorLength(const f32 *vector);

/* Build a normalized plane from three vertices spaced four floats apart.
 * Snap near-zero normal components before deriving D from the third vertex.
 * Degenerate triangles retain the native divide-by-zero behavior. */
void fldMakeEveHitPlanes(const f32 *vertices, f32 *normalX, f32 *normalY, f32 *normalZ, f32 *planeConstant);

/* Return the first matching record's world node; NULL also denotes a missing ID. */
dds3Basic_t *fldFindWorldNodeForRecordId(s32 recordId);

/* Update every matching ID, including duplicates; leave other records intact. */
void fldBindWorldNodeToRecordId(s32 recordId, dds3Basic_t *worldNode);

void fldClrCollision(void);

void fldMakePlane(const FldCollisionDescriptor *geometry, const f32 *translation,
                   u32 id, s32 sceneFlag,
                   FldMapSlotAuxiliaryFlagSelector auxiliaryFlagSelector);

s32 HitPositionInside2(f32 *out, FldCollisionAxis axis, s32 count, f32 margin, const f32 *pos,
                        const collision_s *zone);

extern f32 fldResolvedPosition[3];

extern u8 fldTestRoomSceneFlag(s32 mapId, u32 slotIndex, s32 bit);

extern u8 fldTestMapSlotAuxiliaryFlag(
    s32 mapId, u32 slotIndex, FldMapSlotAuxiliaryFlagSelector bit);

void fldHitChkUnitElvOnly(const f32 *input);

/* Indexed fields retain index-first addresses; see docs/idioms.md,
 * "Trace-justified indexed field addresses: relative zone motion". */
void fldHitChkUnitElv(f32 *position);

/* Non-owning native alias of fldGlobalBase.terrainAttritionKind (FldAreaWork + 0x11C). */
extern FldTerrainAttritionKind D_0032E4CC[];

/* Classify flagged zone faces after checking the current movement. */
void fldHitChkUnitDamage(f32 *position);

void func_00137E90(void);

void func_00137E98(void);

void func_00137EA0(void);

/* Project away the selected axis and reject points outside expanded bounds.
 * Return the last negative plane's margin-adjusted distance, not a minimum;
 * no negative plane leaves margin unchanged. Reject results below 0.001. */
f32 HitPositionInside3(FldCollisionAxis axis, s32 planeCount, f32 margin, const f32 *position,
                       const collision_s *zone);

FldValueRecordIndex fldHitChkCamera(const f32 start[3], const f32 end[3], f32 hitPoint[3]);

FldValueRecordIndex fldHitChkEnemy(const f32 start[3], const f32 end[3], f32 hitPoint[3]);

/* Add the four vertical faces of an axis-aligned field collision box. */
void fldAddItemBoxObject(f32 x, f32 y, f32 z, f32 extent, f32 height,
                         FldMapSlotAuxiliaryFlagSelector auxiliaryFlagSelector);

void evtMessageExtraActionNoOp(void);
/* End of declarations from src/dds1/field/fldFileResolver.c */

/* Declarations from dds-decomp src/dds1/game/code_00117438.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "dds/sdk_ps2.h"

#include "dds/evt_run_context_api.h"

#include "dds/common.h"

#include "dds/scr.h"

#include "dds/sdf_vu0.h"

#include "dds/sdf_channel.h"

#include "dds/scr_work_task_api.h"

#include "dds/dat_save_words.h"

#include "dds/sdf_float_counter.h"

#include "dds/eff_random_mod.h"

#include "dds/dds3_task_create.h"

#include "dds/dds3obj.h"

#include "dds/sdf_heap_api.h"

#include "dds/mnu_flag_snapshot.h"

#include "dds/dds3_path.h"

#include "dds/eff_transform.h"

#include "dds/evt_world_source_transform.h"

#include "dds/pcp_vu0.h"

#include "dds/btl_action.h"

#include "dds/dat_state.h"

#include "dds/dat_affinity_selector.h"

#include "dds/dat_effective_affinity_api.h"

#include "dds/dat_world_field_flags.h"

#include "dds/dat_unit_has_skill_api.h"

#include "dds/evt_randomize_entry_api.h"

#include "dds/dat_command.h"

#include "dds/sdf.h"

#include "dds/sdf_dev_state.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/fld_runtime_reset_api.h"

#include "dds/mdl_flag_api.h"

extern void scrDestroyWorkTask(void);

extern void dds3WorkInit(const scrHeader_t *header);

extern scrHeader_t *D_003BAAAC;

#define SDF_PACKED_STATUS_BIT 0x8000

#define SDF_PACKED_CHANNEL_MASK 0x7fff

#define SDF_UNIT_ENEMY 0x20

extern DatEnemyRecord *datEnemyRecords;

extern u32 sdfRollActionHit(s32 channel, datUnitWork_t *sourceRecord, datUnitWork_t *packed);

extern char sdfRuntimeTaskName[];

void sdfRuntimeTaskTeardownHook(void);

s32 dds3GlobalWorkMainProc(dds3ProcessID_t *task);

void evtResetWorldAndProfileRuntime(void);

extern void dds3GlobalWorkClearFlags(void);

extern void evtInitGlobalWork(void);

extern void ptyClearProfileRecords(void);

extern void ptyRebuildAllProfiles(void);

extern void evtUpdateFlaggedEntries(void);

extern void dds3ForEachEntry(void);

extern void sdfSaveResetSnapshot(void);

extern void sdfRestoreResetSnapshot(void);

s32 sdfDispatchUnitScriptDefault5(u32 unitIndex, datUnitWork_t *sourceRecord,
    datUnitWork_t *targetRecord, u8 mode);

/* Linearly interpolated curve sample at `time`; 0 when no curve is active. */
f32 dds3PathBasicControlGetFovy(dds3PathBasicControl_t *user);

#define SDF_FLOAT_COUNTER_FROZEN_FLAG 0x00000008

#define SDF_FLOAT_COUNTER_WRAP_FLAG 0x00000020

/* Step the counter by one; return zero when a non-looping endpoint is reached. */
s32 dds3PathBasicControlStep(SdfFloatCounter *counter);

void dds3PathBasicControlAlterMode(SdfFloatCounter *counter, u32 direction);

u32 dds3PathBasicControlGetMode(const SdfFloatCounter *counter);

void dds3PathBasicControlSetRatio(SdfFloatCounter *value, float multiplier);

float dds3PathBasicControlGetRatio(const SdfFloatCounter *value);

void dds3PathBasicPauseOn(SdfFloatCounter *value);

void dds3PathBasicPauseOff(SdfFloatCounter *value);

void dds3PathBasicLoopOn(SdfFloatCounter *value);

void dds3PathBasicLoopOff(SdfFloatCounter *value);

dds3Basic_t *dds3AddPointBasic(u32 key, EvtWorldSourceTransformPrefix *source, const char *name);

f32 *dds3GetTransformSourceData(dds3Basic_t *node);

void *dds3GetWorldNodeData(dds3Basic_t *node);

/* Load the world node's borrowed data vector into VF10. */
void evtLoadValueVectorIntoVu(dds3Basic_t *node);

/* Same, from the second quadword in the borrowed vector. */
void evtLoadValueSecondaryVectorIntoVu(dds3Basic_t *node);

/* Allocate the 0x33600 game-state block, retain its scene allocation, zero it
 * and register the "GBWK" tick task that owns it. */
void dds3GlobalWorkInit(void);

/* Tear down the "GBWK" task hierarchy, clear the backing scene allocation
 * owned by the game state, and release the global state handle. */
void dds3GlobalWorkDestroy(void);

s32 dds3GlobalWorkMainProc(dds3ProcessID_t *task);

extern void btlClearSharedBattleStateWords(void);

extern void ptyInitRuntime(void);

void evtResetWorldAndProfileRuntime(void);

/* Empty GBWK teardown hook, invoked before its backing allocation is released. */
void sdfRuntimeTaskTeardownHook(void);

extern s8 D_003BA9E5;

extern s8 D_003BA9E6;

extern s8 D_003BA9E7;

extern s8 D_003BA9E8;

extern s8 D_003BA9E9;

extern s8 D_003BA9EA;

extern u16 D_003BA9EC;

extern u32 D_003BA9F0;

extern u32 D_003BA9F4;

extern s8 D_003BA9F8;

extern s8 D_003BA9F9;

extern u32 D_003C2E30[3];

extern DatSaveWords D_0032A200;

extern u8 D_003C2E40[41];

extern sdfMemHandle_t *D_003BD790;

extern sdfMemHandle_t *D_003BD794;

extern sdfMemHandle_t *D_003BD798;

extern sdfMemHandle_t *D_003BD79C;

extern u32 D_003BD7A0;

extern void ptySaveActiveUnitsToStock(void);

extern void mnuCollectFlagArray(u8 *flags);

/* Preserve carried flags and progress before the full runtime reset. */
void sdfSaveResetSnapshot(void);

extern void mnuApplyFlagArray(u8 *flags);

extern void dds3ForEachFlagged(void);

void sdfRestoreResetSnapshot(void);

/* A full reset preserves the scene allocation and restores the saved runtime data. */
void sdfResetGameRuntime(s32 fullReset);

void evtInitializeWorkScriptTask(void);

void sdfFirePendingCallback(void);

ScrCurrentWorkTaskResult scrIsCurrentWorkTask(const struct scrProcessWork_s *expectedValue);

#include "dds/sdf_debug_api.h"

extern const char D_0039F930[10];

/* Load a table's file and copy each size-prefixed segment into its own block. */
void sdfReloadChannelSegments(SdfChannel *table);

extern SdfChannel D_0032A6F0[];

void sdfResetChannels(void);

/* Enemy vitals use a 76-byte base table; party vitals come from a script,
 * a unit-specific bonus, and a maximum of 999. */
s32 ptyComputeMaxHp(datUnitWork_t *unit);

/* Compute the matching MP value from the base table or the MP script. */
s32 ptyComputeMaxMp(datUnitWork_t *unit);

/* Sdf dispatch: pick the script id from the unit's runtime flag byte and run
 * it against the given entry index. 0x40/0x80 select the alternate scripts. */
s32 sdfDispatchUnitScriptDefault5(u32 unitIndex, datUnitWork_t *sourceRecord,
    datUnitWork_t *targetRecord, u8 mode);

s32 sdfDispatchCmd(u32 unitIndex, datUnitWork_t *sourceRecord,
    datUnitWork_t *targetRecord, u32 mode);

s32 sdfDispatchUnitScriptDefault9(u32 unitIndex, datUnitWork_t *sourceRecord,
    datUnitWork_t *targetRecord, u8 mode);

s32 evtRunCommandContext10(u32 unitIndex, datUnitWork_t *sourceRecord,
    datUnitWork_t *targetRecord, u8 mode);

s32 evtRunCommandContext7(u32 unitIndex, datUnitWork_t *sourceRecord,
    datUnitWork_t *targetRecord, u8 mode);

s32 sdfDispatchSubCmd(u32 unitIndex, datUnitWork_t *sourceRecord,
    datUnitWork_t *targetRecord, u32 mode);

s32 sdfCalculateCommandVitalDelta(s32 channel, datUnitWork_t *sourceRecord, datUnitWork_t *item, u32 mode, u8 vital);

extern char D_0039F980[];

/* Rolls whether the action hits: returns the surviving channel mask, or 0 on a miss. */
u32 sdfRollActionHit(s32 channel, datUnitWork_t *sourceRecord, datUnitWork_t *packed);

u32 sdfQueryChannelValue(s32 channel, datUnitWork_t *sourceRecord, datUnitWork_t *item);

u32 sdfQueryChannelBits(s32 channel, datUnitWork_t *sourceRecord, datUnitWork_t *item);

extern s32 sdfCalculateCommandVitalDelta(s32 channel, datUnitWork_t *sourceRecord, datUnitWork_t *item, u32 mode, u8 vital);

extern void datResetBadStat(datUnitWork_t *item, s32 mask);

/* These unused-result legacy mutator calls had no prototype in scope. */
void sdfRaisePackedChannelValue(datUnitWork_t *item, u32 minimumValue);

extern s32 datAddHp(datUnitWork_t *unit, s32 delta);

extern s32 datAddMp(datUnitWork_t *unit, s32 delta);

s32 sdfApplyCommandResults(s32 channel, datUnitWork_t *sourceRecord, datUnitWork_t *item);

void sdfSetPackedValuePreservingFlag(datUnitWork_t *item, u16 channelValue);

/* Raise the packed value to `minimumValue` (never lower it); reaching 0x40, 0x80 or 0x400 re-randomizes the entry. */
void sdfRaisePackedChannelValue(datUnitWork_t *item, u32 minimumValue);
/* End of declarations from src/dds1/game/code_00117438.c */

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013CD60);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013CF70);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013D1A8);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013D338);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013D4B0);

void sdfFirePendingCallback(void) {
    if (D_003BAAAC == 0) {
        return;
    }
    scrDestroyWorkTask();
}

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013D560);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013D5B8);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013D620);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013D640);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013D688);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013D6F0);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013D6F8);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013D788);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013D800);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013D8A8);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013D968);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013DAD0);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013DBF8);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013DC38);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013E5E0);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013E6C8);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013E738);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013E7B8);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013E880);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013E8E0);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013E950);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013EA10);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013EB50);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013EB90);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013EBB0);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013EC20);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013F548);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013F598);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013F5E8);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013F630);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013F660);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013F690);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013F6D0);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013F7F0);

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_004AAD30);

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_004AAD40);

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_004AAD50);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_0013F840);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_00140030);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_00140058);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_001400B8);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_001401F0);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_00140658);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_00140678);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_001406A8);

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_004AAD80);

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_004AAD90);

INCLUDE_RODATA(const s32, "field/fldFileResolver", D_004AADA0);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_001406B8);

INCLUDE_ASM(const s32, "field/fldFileResolver", func_001415A0);

void fldEnableCameraObjectFlag(void) {
    dds3ObjectBaseOnFlag(fldPlayerObject, 1);
    fldSetCameraNodeModeWithZero();
}

void fldDisableCameraObjectFlag(void) {
    dds3ObjectBaseOffFlag(fldPlayerObject, 1);
}

INCLUDE_ASM(const s32, "field/fldFileResolver", func_00141650);

void fldRestoreCameraModelColor(void) {
    fldPlayerModelContext->inner->color = 0x80808080;
}

void fldSetCameraObjectTransitionFlag(void) {
    dds3ObjectBaseOnFlag(fldPlayerObject, 0x200);
}

void fldClearCameraObjectTransitionFlags(void) {
    dds3ObjectBaseOffFlag(fldPlayerObject, 0x400);
    dds3ObjectBaseOffFlag(fldPlayerObject, 0x200);
}
INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004DE8BC);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004DE8C0);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004DE8C4);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004DE8C8);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004DE8CC);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004DE8D0);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004DE8D4);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004DE8D8);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004DE8DC);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004DE8E0);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004DE8E4);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004DE8E8);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004DE8EC);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004DE8F0);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004DE8F4);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004DE8F8);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004DE8FC);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004DE900);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004DE904);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004DE908);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004DE90C);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004DE910);

INCLUDE_SDATA(const s32, "field/fldFileResolver", D_004DE914);

