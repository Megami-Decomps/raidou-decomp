#include "common.h"

/* Declarations from dds-decomp src/dds1/game/code_00122030.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "dds3obj.h"

#include "field.h"

#include "kernel.h"

#include "newdata.h"

#include "sdf_resource.h"

#include "sdk_libc.h"

#include "dds/btl.h"

#include "dds/dds3_basic_array_cursor.h"

#include "dds/dds3_fade.h"

#include "dds/dds3_object_flags.h"

#include "dds/dds3_object_payload.h"

#include "dds/dds3_slot1_handler_api.h"

#include "dds/dds3_task_create.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/dds3_task_priority_query.h"

#include "dds/dds3_world_counter.h"

#include "dds/dds3_world_primary.h"

#include "dds/dds3_world_secondary.h"

#include "dds/dds3_world_selection.h"

#include "dds/dds3_world_value.h"

#include "dds/dds3obj.h"

#include "dds/eff_blur.h"

#include "dds/eff_inner_float_api.h"

#include "dds/eff_random.h"

#include "dds/evt_solar.h"

#include "dds/evt_world.h"

#include "dds/evt_world_object_api.h"

#include "dds/evt_world_source_transform.h"

#include "dds/fld_scene_ready_api.h"

#include "dds/kwln.h"

#include "dds/mdl.h"

#include "dds/mdl_flag_api.h"

#include "dds/mdl_motion_api.h"

#include "dds/mnu_camp_state_ack_api.h"

#include "dds/scr.h"

#include "dds/sdf_delayed_resource_check_api.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_message.h"

#include "dds/sdf_model_scalars.h"

#include "dds/sdf_trig.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/sdk_libc.h"

#include "dds/snd_sequence_volume_pan.h"

/* Multiply the quaternions in vf10 and vf11, writing the product to vf10. */
void Mult(void);

/* Consume CLEANED_UP back to IDLE, then report whether camp remains active. */
MnuCampStateAcknowledgementResult campCheckExitState(void);

dds3Basic_t *dds3AddPointBasic(u32 key, EvtWorldSourceTransformPrefix *source, const char *name);

/* Advance the wrapping 16-bit world object sequence carried in a u32. */
u32 dds3AllocObjectId(void);

/* Factories accept the serialized parameter prefix used by effect callbacks. */
EffBlurScatterWork *dds3EffectDistortionBlurCreate(dds3Effect_DistortionBlurParam_t *params);

void dds3EffectDistortionBlurDelete(EffBlurScatterWork *work);

/* Return nonzero while either foreground fade direction is enabled. */
s32 dds3FadeCheck(void);

void dds3FadeIn(s32 red, s32 green, s32 blue, s32 duration);

void dds3FadeIn2(s32 duration);

/* Foreground alpha transitions, stepped by dds3FadeUpdate. */
void dds3FadeOut(s32 red, s32 green, s32 blue, s32 duration);

void dds3FadeOut2(s32 duration);

void dds3FreeBasicArrayHandle(WorldIndexNode *node);

/* Return the borrowed primary world node, or NULL when no world is active. */
struct dds3Basic_t *dds3GetActiveScene(void);

dds3Basic_t *dds3GetActiveUnitSceneBasic(dds3Basic_t *world);

dds3Basic_t *dds3GetBasicArray(const dds_dds3BasicArrayHandle_t *object);

u16 dds3GetBasicArrayCount(const dds_dds3BasicArrayHandle_t *object);

/* Return the borrowed secondary world node, or NULL when no world is active. */
struct dds3Basic_t *dds3GetControlScene(void);

void dds3GetPosPoseSet(dds3Basic_t *object);

/* Set the radius stored by the world node's inner transform.
 * Providers: DDS1 game/code_0010EEF0.c and DDS2 game/code_0010F118.c. */
void dds3GetRadiusPoseSet(dds3Basic_t *node, f32 value);

/* Advance caller-owned state, returning its next 32-bit value. NULL selects
 * the shared effect RNG state. */
u32 dds3GetRandA(EffRandState *state);

void dds3GetRotPoseSet(dds3Basic_t *object);

dds3TaskKillRequestResult_t dds3KillProcessName(const char *name, s32 delayTicks);

u32 dds3MatterObjectBasicGetAlpMode(const struct dds3Basic_t *object);

/* Object data word 8 stores a kind-dependent 32-bit payload. */
void dds3MatterObjectBasicSetAlpMode(struct dds3Basic_t *object, u32 value);

void *dds3ObjectBaseGetModelHandle(dds3Basic_t *object);

void dds3ObjectBaseOffFlag(struct dds3Basic_t *object, u32 mask);

/* Set or clear requested bits in the object's flag word. */
void dds3ObjectBaseOnFlag(struct dds3Basic_t *object, u32 mask);

/* Set the callback on an object's slot-1 handler; returns whether it exists. */
u8 dds3ObjectBaseSetPrimaryControl(struct dds3Basic_t *object,
                               Dds3MoverUpdate update);

WorldIndexNode *dds3SceneBasicGetList(dds3Basic_t *object, s32 kind);

dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *world, s32 slotIndex, const char *name);

/* Return a borrowed scheduler-owned handle from the starting, active, or
 * destroy-pending queue. The handle remains valid through its destroy callback
 * and is freed when finalization completes; return NULL when no name matches. */
dds3ProcessID_t *dds3SearchProcessName(const char *name);

/* Returns a borrowed task from the scheduler queues, or NULL when none has this priority. */
dds3ProcessID_t *dds3SearchProcessPriority(u32 priority);

/* Setting either selection does not transfer ownership. */
void dds3SetActiveScene(struct dds3Basic_t *primaryObject);

/* Compatible redeclarations keep the broad object header unchanged. */
Dds3BasicArrayCursorResult dds3SetFirstBasicArray(dds_dds3BasicArrayHandle_t *object);

Dds3BasicArrayCursorResult dds3SetNextBasicArray(dds_dds3BasicArrayHandle_t *object);

void dds3SetSclPoseSet(dds3Basic_t *object, u128 *vector);

/* Store objectName directly as the node's lookup key; no copy is made.
 * A NULL node is ignored and a NULL name clears the key. Keep non-NULL names
 * alive while the node can be searched. */
void dds3SetUnitObjectId(struct dds3Basic_t *node, const char *objectName);

/*
 * Allocate a task and place it in scheduler queues. Returns NULL on allocation
 * failure. On success, the scheduler owns the task handle and frees it after
 * its destroy callback; callers may borrow the handle only until finalization.
 */
dds3ProcessID_t *dds3StartProcess(const char *name, u32 priority, s32 startDelay,
                         s32 destroyDelay, dds3ProcessFunc_t update,
                         dds3ProcessShutdownFunc_t destroy, u32 userValue);

/* Preserve the signed-byte drawing gate; only a missing payload is ignored. */
void dds3VisibleSceneBasic(dds3Basic_t *object, s8 enabled);

/* Return whether the selected flag bit is set, as EvtBitTestResult. */
EvtBitTestResult evtBitCheck(s32 flag);

/* Set and clear one flag using the providers' signed-index adjustment. */
void evtBitOn(s32 flag);

EvtSolarCyclePhase evtGetAgeOfMoon16(void);

/* Create and select the world object for an area/room pair; returns 1 after setup. */
s32 evtLoadArea(s32 area, s32 room);

FldSceneReadyState fldAutoMapChkMode(void);

void mdlLoopMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex);

void mdlSetMotionSpeedEx(dds3ModelHandle_t *ctx, MdlMotionSearchId searchId, f32 frameStep);

extern void *memcpy(void *, const void *, unsigned int);

void *memset(void *dst, s32 value, u32 size);

/* Apply volume and pan to a sequence track through the command queue. */
void p2sdPlaySE(s32 trackId, s32 volume, s32 pan);

scrProcessWork_s *scrSearchScriptName(char *scriptname);

/* Angles are in radians; cosine applies a pi/2 shift to the sine input. */
f32 sdfAtan2(f32 y, f32 x);

SdfDelayedResourceCheckResult sdfDelayedResrcCheck(void);

struct sdfMemHandle_t *sdfMemAllocDirect(s32 requestedBytes);

/* Queue the two heap owners separately: the general heap stores descriptors,
 * while the chip heap stores the address of the cell to release. */
void sdfMemFreeDelayedDirect(struct sdfMemHandle_t *allocation);

/* Return the descriptor's payload base as a byte address, not the descriptor. */
u8 *sdfMemGetPtr(const struct sdfMemHandle_t *block);

/* A model provider's draw surfaces: a row of four pool entries in a surface
 * group table. The second is a key-sorted pool (SdfPool), whose release
 * callbacks the store installs. */
void sdfModelOtInit(SdfModelDrawSurfaceSet surfaces, struct sdfGrOt_t *first,
                                           struct sdfGrOt_t *pool, struct sdfGrOt_t *third,
                                           struct sdfGrOt_t *fourth);

void sdfModelSetFaceDivParm(struct dds_sdfModelRootNode_t *model, f32 first, f32 second);

char *strcpy(char *dst, const char *src);

#include "dds/sdk_ps2.h"

#include "dds/scr.h"

#include "dds/mdl_motion_api.h"

#include "dds/fld_area_work.h"

#include "dds/fld_archive_load_state.h"

#include "dds/fld_runtime_reset_api.h"

#include "dds/fld_area_encounter_table.h"

#include "dds/sdf_packet_list.h"

#include "dds/ee_mmi.h"

#include "dds/kwln.h"

#include "dds/dds3_task_runner.h"

#include "dds/dds3_task_priority_query.h"

#include "dds/dds3_admin_control_api.h"

#include "dds/dds3_task_create.h"

#include "dds/dds3_fade.h"

#include "dds/pcp_vu0.h"

#include "dds/common.h"

#include "dds/fld_camera_link_mode.h"

#include "dds/pty_status_query_api.h"

#include "dds/pty_field_hp_attrition_api.h"

#include "dds/sdf_delayed_resource_check_api.h"

#include "dds/dds3_object_flags.h"

#include "dds/sdf_vu0.h"

#include "dds/eff_inner_float_api.h"

#include "dds/sdf_trig.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/fld_scene_status_api.h"

#include "dds/fld_scene_ready_api.h"

#include "dds/fld_area_resource_ready_api.h"

#include "dds/fld_location_coordinate_flags.h"

#include "dds/fld_movement_hold_state_api.h"

#include "dds/fld_scene_command_state_api.h"

#include "dds/fld_camp_check_state_api.h"

#include "dds/fld_scene_transition_gate_api.h"

#include "dds/mnu_camp_state_ack_api.h"

#include "dds/fld_next_scene_request_api.h"

#include "dds/sdf_message.h"

#include "dds/mnu_title_stream.h"

#include "dds/eff_random.h"

#include "dds/snd_sequence_volume_pan.h"

#include "dds/evt_world_object_api.h"

#include "dds/sdf_model_scalars.h"

#include "dds/dds3_path.h"

#include "dds/dds3_slot1_handler_api.h"

#include "dds/sdf_dev_state.h"

#include "dds/sdf_heap_api.h"

#include "dds/field_stage.h"

#include "dds/eff_blur.h"

#include "dds/fpu.h"

#include "dds/fld.h"

#include "dds/fld_scene_sequence_api.h"

#include "dds/evt_world.h"

#include "dds/evt_solar.h"

#include "dds/evt_random_status_clear_api.h"

#include "dds/evt_world_source_transform.h"

#include "dds/dds3obj.h"

#include "dds/dds3_basic_array_cursor.h"

#include "dds/dds3_world_value.h"

#include "dds/dds3_world_selection.h"

#include "dds/mdl.h"

#include "dds/dat_state.h"

#include "dds/dat_new_cycle_state.h"

#include "dds/dat_world_field_flags.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/mdl_flag_api.h"

#include "dds/dds3_world_counter.h"

#include "dds/fld_resource_wait_api.h"

#include "dds/fld_sequence_controller.h"

#include "dds/file_selection_api.h"

#include "dds/file_slot_flags_change_api.h"

#include "dds/fld_player_scene_state.h"

#include "dds/fld_spark_controller_state.h"

#include "dds/btl.h"

/* Declared before first use (were implicit int). */
extern void evtFreeStage(void);

extern f32 fabsf(f32);

extern void fldPauseBgm(void);

extern void fldUpdateCameraTarget(void);

extern void fldSubmitSelectedActorWaypointTransition(void);

extern dds3Basic_t *dds3AddCameraObjectBasicTypeLookAt(s32 key, u128 *targetPosition, u128 *worldEye, u128 *localUp);

extern void fldSkySetTableAnime(u32);

extern u32 D_003BABE0;

extern s32 fldNext_fld;

extern FldSequenceController fldAutoMapSeqFlags;

extern s32 D_003BABF0;

extern dds3Basic_t *D_003BABD0;

#include "dds/dds3_object_payload.h"

extern u32 fldDeferredCommandParameter;

extern s32 gFldElvFloorNow;

extern s32 fldElvFloorFrom;

extern s32 fldElvFloorNum;

extern s32 fldElvFloorTo;

extern s32 fldElvFloorDisp;

extern s32 fldElvFloorTarget;

extern s32 fldElvState;

extern s32 D_0032E4DC[];

extern sdfGrOt_t D_00324F88[];

extern sdfGrOt_t *D_dds1_00325788[13][4];

extern u8 D_00324530[];

extern char D_0039FCA0[];

extern char D_0039FCB0[];

extern char D_0039FCC8[];

extern void fldDispatchSceneAction(u32 action);

extern dds3Basic_t *fldPlayerObject;

extern struct sdfMemHandle_t *fldPlayerModelResource;

extern u32 D_0043EA44[];

extern struct sdfMemHandle_t *D_003BAB58;

extern u32 D_0032E3D0[];

extern u32 D_003BABE8;

extern dds3ModelHandle_t *fldPlayerModelContext;

extern void fldLoadNextFile_SetId_BtlRet(u32 area, u32 floor);

extern void fldFreeNextFile(void);

extern void func_0013F100(s32, u32);

extern u32 D_0032E4EC[];

extern s32 D_003BABEC;

void fldProcNextJump(void);

extern FieldPlayerSceneWork fldSceneParam;

extern void dds3WorkClear(void);

extern char D_0039FBC0[];

struct dds3Basic_t;

extern u8 D_003BAB3C;

extern u8 D_0032C9A0[];

void fldLightMaClear(void);

void fldRifutoMaClear(void);

void fldRiberaMaClear(void);

void fldEsutoMaClear(void);

void fldEncountClearFlag(u32 mask);

extern u32 D_003BAB64;

extern u32 D_003BAB60;

extern u32 D_003BAB5C;

extern void mdlPackDecode(s32, s32, s32, const u8 *, s32);

void fldLoadPlayerModel(void);

u8 fldEncountTestFlag(u32 mask);

#include "dds/fld_encounter_request.h"

extern FieldEncounterRequest D_003BAC08;

extern void fldResetEventSceneState(void);

extern void fldEveHit_End(void);

extern void fldAutoMapSeqSetFlag(u32);

extern void evtKillFieldScript(void);

extern char D_003BAAE0[];

extern char D_003BAAE8[];

extern char D_003BAAF0[];

extern char D_003BAAF8[];

extern char D_003BAB00[];

extern u32 fldRunDanteGunFlag[];

extern u32 fldRunDanteGunFlag[];

/* Search location records 1..639; a miss returns the reserved first record, * not NULL, so callers can read the fallback record's fields. */ const s16 *fldFindLocationCoordinateRecord(s32 area, s32 locationIndex);

void fldLoadFieldTables(void);

void InitOoyTest(void);

void KillOoyTest(void);

/* Return to-minus-from after integer-degree reduction and one wrap adjustment. */
f32 fldAngleDifference(f32 fromAngle, f32 toAngle);

f32 fldSnapAngleToCompassPoint(f32 angle);

/* Snap an angle in degrees to the nearest of the eight compass directions. */
f32 fldPlayerCalc360_8(f32 angle);

f32 fldPlayerCalc360_4(f32 angle);

/* Direction in degrees from two X/Z positions, preserving the field's 3.14 PI. */
f32 fldCalcAngleBetween(f32 fromX, f32 fromZ, f32 toX, f32 toZ);

extern f32 fldCalcAngleBetween(f32 fromX, f32 fromZ, f32 toX, f32 toZ);

f32 GetSphericalPosMove(f32 fromX, f32 fromZ, f32 toX, f32 toZ);

/* Interpolate the support triangle along its two projected X/Z edges. */
f32 fldCamCalcSlopeOffset(f32 x, f32 z, f32 *xs, f32 *ys, f32 *zs);

f32 fldPointDistance(f32 ax, f32 ay, f32 az, f32 bx, f32 by, f32 bz);

/* For kind-6 objects in mode 4, set mode 3 or clear it to 0.
 * status covers the count, payload mode word and cursor result. */
void fldToggleWorldNodeState(s64 clearMode);

extern u32 fldRunDanteGunFlag[];

extern void sdfResetGameRuntime(s32);

extern void fileCacheSlotFlagsFromState(void);

extern void fileRestoreSlotFlagsToState(void);

void fldSetDeferredFieldCommand(u32, u32);

extern void dds3AdminiCallSequence(s32, const void *, u32, s32);

void fldPrepareDeferredSceneTransition(void);

extern char D_003BAB68[];

void fldSetSceneParam2(fldSceneParam_t *record, s32 stage, s32 kind, const char *name);

void fldMainLoadStart(void);

extern u64 D_003BAB70[], D_003BAB78[], D_003BAB80[], D_003BAB88[], D_003BAB90[];

extern u64 D_003BAB98[], D_003BABA0[], D_003BABA8[], D_003BABB0[];

extern u8 D_0032F240[];

#include "dds/sdf_debug_api.h"

extern void fldDrawInit(void);

extern void fldResetPlayerSceneTransformState(void);

extern void fldSe_Init(void);

extern void sdfRandomize(void *, s32);

extern void fldParseMixLb(void);

extern void fldLoadBattleSkyAndFilter(void);

extern void fldResetSceneResourceTables(void);

void fldInitializeDisplayAndTables(void);

void fldResetPlayerSceneTransformState(void);

/* Fill a sequence packet for stage/kind/name, then clear the temporary override.
 * Unspecified packet bytes deliberately retain their previous contents. */
void fldSetSceneParam2(fldSceneParam_t *record, s32 stage, s32 kind, const char *name);

/* Fill the alternate-mode sequence packet for stage/kind/name.
 * Reuse a running sequence in the same area; otherwise request a work clear. */
void fldSetSceneParam3(fldSceneParam_t *record, s32 stage, s32 kind, const char *name);

/* Fill a field sequence packet, including its narrowed code, link and detail name.
 * A running sequence in the same area requests reuse rather than a work clear. */
void fldSetSceneParam4(fldSceneParam_t *record, s32 stage, s32 kind, const char *name,
                    s32 code, FldCameraLinkMode link, const char *subname);

/* Fill a sequence packet with a narrowed code and note instead of a detail/link.
 * A running sequence in the same area requests reuse rather than a work clear. */
void fldSetSceneParam5(fldSceneParam_t *record, s32 stage, s32 kind, const char *name,
                    s32 code, const char *subname);

void fldSetSceneParamLoad(fldSceneParam_t *record, s32 stage, s32 kind, const char *name,
                    s32 code, FldCameraLinkMode link, const char *subname);

/* Converts the field work's primary and secondary state words into a
 * compact scene status, giving the primary state precedence. */
FldSceneStatus fldGetRestoreMode(void);

extern FieldAreaEncounterTable *D_003BAA38;

extern void fldInitializeActorLinkedSequence(s32 mode, s32 index, fldSceneParam_t *sequence);

extern f32 *dds3GetTransformSourceData(dds3Basic_t *node);

extern void fldStartMiniTitleForUnlock(s32 area);

extern void FromRotation(f32 angle);

extern void fldPreparePlayerAndCameraScene(u128 *transform);

extern void fldPreparePlayerSceneCameraTarget(void);

extern void fldCreateSecondaryWorldCamera(void);

extern void fldUpdateCameraViewFromPlayer(void);

extern void fldUpdateFieldCameraViewState(void);

extern void func_0012CED0(void);

extern void fldActivateCameraColorSetting(s32 enabled);

extern void mnuInitializeResourceEntries(void);

extern void fldLoadFieldEffectTextureSlots(void);

void fldRebuildFieldScene(void);

void fldUnloadPlayerModel(void);

void fldLoadPlayerModel(void);

void fldUnloadPlayerModel(void);

typedef struct FieldVec4 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} FieldVec4;

extern FieldVec4 D_0039FC40;

extern char D_0039FC50[];

extern void FromRotation(f32);

extern s32 D_0032E4C8[];

extern void dds3RebuildObjectModelContext(dds3Basic_t *object, s32 resourceGroup, s32 resourceId);

extern void fldApplySceneLightSet(s32 lightSetIndex, s32 durationMode);

extern s32 fldUpdateCameraObjectPose(void);

extern dds3Basic_t *dds3AddUnitObjectBasic(s32, u128 *, u128 *);

extern void fldApplySkyLightSet(u32 skyLightSetIndex, s32 durationMode);

extern void dds3DeleteUnitObjectBasic(dds3Basic_t *);

/* Toggle the selected player model and rebuild its saved scene pose.
 * Port of the qualified DDS2 Sable/Garnet/Tidal reconstruction. The four
 * legacy SDK stores are scoped to the verified VU/callee-read pipeline. */
void fldTogglePlayerModel(void);

void fldPrepareResourceBuffer(void);

void fldReleaseResources(void);

void fldReleasePlayerSceneResources(void);

void fldReleaseCampSceneTasks(void);

void fldCleanupFieldScene(void);

dds3Basic_t **fldGetPlayerSceneStateAddress(void);

dds3Basic_t *fldGetPlayerSceneState(void);

s32 fldPlayerCameraTargetNoPathUpdate(dds3PoseSet_t *relativeTransform, dds3Basic_t *target);

extern void fldRefreshAreaStateFromPosition(f32 *position);

void fldEncountSetFlag(u32 mask);

/* Raise the scene-state minima, clear pending slots and aim ten units above
 * the fetched player position. Existing larger mode/state values are retained. */
void fldPreparePlayerSceneCameraTarget(void);

void fldReleaseUnitPad(void);

extern s32 D_0032F1DC[];

extern char D_0039FC50[];

extern s32 D_003BAB50;

/* Initialize the homogeneous vectors and create/register the player if absent. */
void fldCreatePlayerObject(void);

extern FieldVec4 D_0039FC60;

extern FieldVec4 D_0039FC70;

extern f32 D_0032F170[12];

extern char D_003BABB8[];

extern void FromRotation(f32);

extern void fldResetCameraModelHandles(void);

void fldEnterSceneCommand(void);

extern void fldSkySetTblLightMaDZ(s32);

/* Install the two supplied homogeneous transform vectors and prepare field
 * model, scene commands and the borrowed action transform. */
void fldPreparePlayerAndCameraScene(u128 *transform);

extern FieldVec4 D_0039FC80;

extern FieldVec4 D_0039FC90;

extern char D_003BABC0[];

extern void dds3CameraObjectBasicSetWorldEye(struct dds3Basic_t *camera, const u128 *worldEye);

/* Create the secondary camera at target origin with the stored eye/up vectors. */
void fldCreateSecondaryWorldCamera(void);

s32 fldPlayerCameraTargetNoPathUpdate(dds3PoseSet_t *relativeTransform, dds3Basic_t *target);

s32 fldLightMaIdxCalc(void);

void fldLightMaClearMsg(void);

void fldLightMaClear(void);

extern void fldPlayFieldSeVolumePan(s32);

void fldEnterSceneCommand(void);

FldSceneCommandState fldGetSceneCommandState(void);

/* Latch the area/floor command while enabled and flagged; stop it when either
 * gate clears. Disable passes transition parameter 0; flag removal passes 20. */
void fldUpdateSceneCommand(void);

/* Area-state consumption markers are separate from shared game-state flags. */
enum {
    FIELD_TRANSITION_CONSUMED_FLAG = 2,
    FIELD_SECONDARY_SCENE_CONSUMED_FLAG = 4,
    FIELD_PRIMARY_SCENE_CONSUMED_FLAG = 8
};

/* Clear a set transition flag and mark its consumption in area state. */
void fldRifutoMaClearMsg(void);

/* Clear the transition work flag and its area-state consumption marker. */
void fldRifutoMaClear(void);

/* Set the transition work flag and reset its area-state consumption marker. */
void fldRifutoMaStart(void);

/* Return whether the transition work flag is set. */
u8 fldRifutoMaChk(void);

/* Clear a set secondary-scene flag and mark its consumption in area state. */
void fldRiberaMaClearMsg(void);

/* Clear the secondary-scene work flag and its consumption marker. */
void fldRiberaMaClear(void);

/* Select secondary rather than primary, resetting secondary's consume marker. */
void fldRiberaMaStart(void);

/* Return whether the secondary-scene work flag is set. */
u8 fldRiberaMaChk(void);

/* Clear a set primary-scene flag and mark its consumption in area state. */
void fldEsutoMaClearMsg(void);

/* Clear the primary-scene work flag and its area-state consumption marker. */
void fldEsutoMaClear(void);

/* Select primary rather than secondary, resetting primary's consume marker. */
void fldEsutoMaStart(void);

/* Return whether the primary-scene work flag is set. */
u8 fldEsutoMaChk(void);

void fldConfigureRenderCleanupMessage(void);

#include "dds/fld_encounter_roll_entry.h"

extern FldEncEntry *fldEncounterRollTable;

enum {
    FIELD_ENCOUNTER_ROLL_ENTRY_COUNT = 16
};

#define FIELD_ENCOUNTER_PERCENT_RANGE 100U

/* Return the first successful stage/flag-gated roll, or FLD_ENCOUNTER_TABLE_INDEX_NO_SELECTION.
 * Each eligible row rolls independently; chance retains its unsigned cast. */
FldEncounterTableIndex fldRollEncounter(void);

extern f32 D_0032F250[4];

#include "dds/dds3_world_primary.h"

extern void fldUpdateSmoothedCameraPose(void);

extern u8 fldAutoMapSeqTestFlag(u32);

extern void evtAdvanceSolarClockAndExpireControls(f32 travelDistance);

extern void evtUpdateFlaggedStats(void);

#include "dds/btl_conditional_encounter_roll_api.h"

extern void dds3WorkClear(void);

extern char D_0039FCA0[], D_0039FCB0[];

/* Advance movement-driven solar/attrition state and queue field encounters.
 * These two legacy SDK stores target only the aligned previous-position
 * quadword. Its old value and later observations are actual VU loads, never
 * cached C scalar reads; the pinned compiler trace verifies this pipeline. */
s32 fldUpdateTravelAndEncounters(void);

/* Queue the current area and next floor, reset the scene lifecycle, and return -1.
 * The pending pair is consumed later with a 200-entry offset on the area value. */
s32 fldAdvanceToNextScene(void);

u8 fldHasKiretaLabelProcess(void);

u8 fldProcKiretaChk(void);

u8 fldProcHirakenaiChk(void);

FldCampCheckState fldProcCampChk(void);

extern void fldUpdateSmoothedCameraPose(void);

extern void fldSetCameraNodeModeWithTen(void);

extern void mnuCreateCampTasks(void);

extern void evtDisableMoonDraw(void);

extern void fldApplySkyLightSetToPlayerVU(void);

extern void fldAutoMapSeqClearFlag(u32);

extern u8 fldAutoMapSeqTestFlag(u32);

extern s16 D_0032E4D8[];

s32 fldUpdateCampMenuTransition(void);

FldSceneTransitionGateResult fldProcAmChk(void);

struct dds_dds3ObjectBaseControlWorkData_t;

extern void func_00145B18(void);

extern void fldMarkStageExplorationCellsAroundPosition(s32, s32, f32, f32, f32);

/* Gate next-floor input on camp/readiness flags, then start the native scene
 * transition or its delayed fade path. All return paths retain zero. */
s32 fldUpdateNextFloorTransition(void);

extern void func_0014C468(void);

/* Dispatch one pending field command, preferring the temporary override
 * over the scene-work buffer and its saved fallback. */
s32 fldDispatchPendingSceneResource(void);

extern void evtEnableMoonCalc(void);

extern s32 fileMenuTaskExists(void);

extern void fldCheckSceneReady(void);

extern void fldInitInputPanel(void);

extern void fldInitAddressProcess(void);

extern void fldFinishDeferredExit(void);

extern void fldFlushQueuedEffectPositions(void);

extern FldArchivePendingState fldGetArchiveLoadPending(void);

extern void fldLoadSceneModelsAndCamera(void);

extern void fldPrepareSceneBgmArchive(void);

extern void fldSetSnowActive(void);

extern s32 fldEveHit_ChkEveHitAfter(void);

extern void fldStartDeferredFieldExit(void);

extern void fldStartSceneBgm(void);

extern void fldStartSceneBgmAlternate(void);

extern void fldPlayFieldBgm(void);

extern s32 fldUpdateCameraFollow(void);

extern s32 fldUpdateCameraFrame(void);

extern void fldUpdateObjectSlotActivation(void);

extern void fldSkyCalcFilter(void);

extern void fldInitializeAreaWork(void);

extern void fldRebuildFieldScene(void);

extern s32 fldUpdateTravelAndEncounters(void);

extern void fldUpdateSelectedValueTransition(void);

extern void fldUpdateRoomTriggers(dds3Basic_t *);

extern void func_001415F8(void);

extern void fldUpdateSparkMessageSequence(void);

extern FldSparkDialogPhase fldGetSparkDialogPhase(void);

extern void fldUpdateSparkEncounterScene(void);

extern void fldNoopWithMode(s32 mode);

extern u32 D_003BABE8;

extern u32 D_003BA8EC;

extern u32 D_003BAD58;

extern FieldEncounterRequest D_003BAC08;

extern EffBlurScatterWork *D_003BABE4;

dds3TaskUpdateResult_t fldProcSequence(dds3ProcessID_t *task);

#include "dds/fld_encounter_runtime_api.h"

extern void fldDbgPrint2D(s32, s32, const char *);

extern u8 fldIsEventPhaseAtLeastTwo(void);

extern void func_00149B68(void);

extern void fldDrawAutomapWhenReady(void);

extern void fldElvDrawFloorNum(void);

extern void fldTickWeatherEffectNodes(void);

extern s32 D_0032E590[];

extern s32 D_0032E598[];

extern s32 D_003BABFC;

dds3TaskUpdateResult_t fldProcDraw(dds3ProcessID_t *task);

/* Lifecycle flags and the separate scene-control flags have independent
 * set, clear and test operations. */
void fldAutoMapSeqSetFlag(u32 mask);

void fldAutoMapSeqClearFlag(u32 mask);

u8 fldAutoMapSeqTestFlag(u32 mask);

void fldEncountSetFlag(u32 mask);

void fldEncountClearFlag(u32 mask);

u8 fldEncountTestFlag(u32 mask);

extern s32 D_003BABF4;

extern char D_0039FD30[], D_0039FD40[];

extern dds3TaskUpdateResult_t fldProcDraw(dds3ProcessID_t *task);

/* Start the field scene tasks; a zero mode copies the caller's sequence record into the scene work. */
void fldProcTrigSequence(const fldSceneParam_t *record, u32 mode);

extern s32 D_003BABF4;

extern EffBlurScatterWork *D_003BABE4;

extern char D_0039FD30[], D_0039FD40[];

extern void dds3FadeResetBackground(void);

extern void fldFreeInputPanel(void), evtDisableMoonDraw(void), fldFreeAddressProcess(void);

extern void mnuDestroyCampTasks(void), scrAllReleaseScript(void);

extern void fldReleaseMenuSlotsAfterWait(void);

void fldReleaseCampSceneTasks(void);

FldResourceWaitResult fldIsFieldResourceWaitFinished(void);

extern void fldSelectActorFromSceneIndexTables(s32 mode, s32 index);

void fldApplyTerminalActorSelection(s32 mode, s32 index);

void fldProc_SisetuReturnPoint(void);

void fldProc_CallSisetuReturn(void);

void fldSetDeferredFieldCommand(u32 command, u32 parameter);

void fldProcNextJump(void);

void fldSetPendingSceneAction(u32 argument);

void fldRunPendingSceneAction(void);

/* Consume the pending pair; write outputs only when a request is consumed. */
FldNextSceneRequestResult fldConsumeNextSceneRequest(FldNextSceneRequestMapCode *outMapCode,
                                                      FldNextSceneRequestEntryIndex *outEntryIndex);
/* End of declarations from src/dds1/game/code_00122030.c */

f32 fldPointDistance(f32 ax, f32 ay, f32 az, f32 bx, f32 by, f32 bz) {
    f32 dx = ax - bx;
    f32 dy = ay - by;
    f32 dz = az - bz;
    return fsqrtf(dx * dx + dy * dy + dz * dz);
}

INCLUDE_ASM(const s32, "code_0012C998", func_r1_001283A8);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00128418);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00128528);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00128580);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00128618);

/* For kind-6 objects in mode 4, set mode 3 or clear it to 0.
 * status covers the count, payload mode word and cursor result. */
void fldToggleWorldNodeState(s64 clearMode) {
    WorldIndexNode *valueChain;
    s32 status;
    dds3Basic_t *worldObject;

    valueChain = dds3SceneBasicGetList(dds3GetControlScene(), 6);
    status = dds3GetBasicArrayCount((dds_dds3BasicArrayHandle_t *)valueChain);
    if (status == 0) {
        return;
    }
    dds3SetFirstBasicArray((dds_dds3BasicArrayHandle_t *)valueChain);
    do {
        worldObject = dds3GetBasicArray((dds_dds3BasicArrayHandle_t *)valueChain);
        status = dds3MatterObjectBasicGetAlpMode(worldObject);
        if (status == 4) {
            if (clearMode == 0) {
                dds3MatterObjectBasicSetAlpMode(worldObject, 3);
            }
            else {
                dds3MatterObjectBasicSetAlpMode(worldObject, 0);
            }
        }
        status = dds3SetNextBasicArray((dds_dds3BasicArrayHandle_t *)valueChain);
    } while (status != DDS3_BASIC_ARRAY_CURSOR_EXHAUSTED);
    dds3FreeBasicArrayHandle(valueChain);
}

INCLUDE_ASM(const s32, "code_0012C998", func_0012CD48);

INCLUDE_ASM(const s32, "code_0012C998", func_0012CD70);

INCLUDE_ASM(const s32, "code_0012C998", func_0012CF80);

INCLUDE_ASM(const s32, "code_0012C998", func_0012CFF0);

INCLUDE_ASM(const s32, "code_0012C998", func_0012D068);

INCLUDE_ASM(const s32, "code_0012C998", func_0012D0E0);

INCLUDE_ASM(const s32, "code_0012C998", mapRndEncParamSet);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00128CE0);

INCLUDE_ASM(const s32, "code_0012C998", mapSetPlayersVisible);

INCLUDE_ASM(const s32, "code_0012C998", func_0012D5C8);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_001290D0);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_001290F8);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00129138);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00129150);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00129190);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_001291A8);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_001291E8);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00129228);

INCLUDE_ASM(const s32, "code_0012C998", mapRaidouBackUpPosReset);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_001293A0);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00129460);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00129510);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00129530);

INCLUDE_ASM(const s32, "code_0012C998", func_0012DB00);

INCLUDE_ASM(const s32, "code_0012C998", func_0012DB58);

INCLUDE_ASM(const s32, "code_0012C998", func_0012DB80);

INCLUDE_ASM(const s32, "code_0012C998", func_0012DBC0);

INCLUDE_ASM(const s32, "code_0012C998", func_0012DBE8);

INCLUDE_ASM(const s32, "code_0012C998", func_0012DC28);

INCLUDE_ASM(const s32, "code_0012C998", func_0012DC50);

INCLUDE_ASM(const s32, "code_0012C998", func_0012DC90);

INCLUDE_ASM(const s32, "code_0012C998", func_0012DCC8);

INCLUDE_ASM(const s32, "code_0012C998", func_0012DD00);

INCLUDE_RODATA(const s32, "code_0012C998", D_r1_00446B50);

INCLUDE_RODATA(const s32, "code_0012C998", D_r1_00446B60);

INCLUDE_RODATA(const s32, "code_0012C998", D_r1_00446B70);

INCLUDE_RODATA(const s32, "code_0012C998", D_r1_00446B80);

INCLUDE_ASM(const s32, "code_0012C998", func_0012DDE0);

INCLUDE_ASM(const s32, "code_0012C998", func_0012DE48);

INCLUDE_ASM(const s32, "code_0012C998", func_0012DE88);

INCLUDE_ASM(const s32, "code_0012C998", func_0012DED0);

INCLUDE_ASM(const s32, "code_0012C998", func_0012DEF0);

INCLUDE_ASM(const s32, "code_0012C998", func_0012DF00);

INCLUDE_ASM(const s32, "code_0012C998", func_0012DF20);

INCLUDE_ASM(const s32, "code_0012C998", func_0012DF38);

INCLUDE_ASM(const s32, "code_0012C998", func_0012DF68);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00129608);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00129618);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00129680);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00129778);

INCLUDE_ASM(const s32, "code_0012C998", func_0012E1C0);

INCLUDE_ASM(const s32, "code_0012C998", func_0012E3A0);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00129B20);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00129B58);

INCLUDE_ASM(const s32, "code_0012C998", fldPlayerALoadStart);

INCLUDE_ASM(const s32, "code_0012C998", fldPlayerALoadEndCheck);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00129C40);

INCLUDE_ASM(const s32, "code_0012C998", func_0012E590);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00129D08);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00129D38);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00129D70);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00129DA8);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00129E28);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00129E70);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00129EA0);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00129EE8);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00129F98);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00129FA8);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_00129FE0);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_0012A018);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_0012A098);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_0012A0D8);

INCLUDE_ASM(const s32, "code_0012C998", func_0012EAC0);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_0012A158);

INCLUDE_ASM(const s32, "code_0012C998", func_r1_0012A1A0);

INCLUDE_ASM(const s32, "code_0012C998", func_0012EB38);

INCLUDE_ASM(const s32, "code_0012C998", func_0012EB58);

INCLUDE_ASM(const s32, "code_0012C998", func_0012EB78);

INCLUDE_ASM(const s32, "code_0012C998", fldCleanupFieldScene);

INCLUDE_ASM(const s32, "code_0012C998", fldRegistUnitPad);

void fldReleaseUnitPad(void) {
    if (fldPlayerObject != 0) {
        fldEncountClearFlag(0x40);
        dds3ObjectBaseSetPrimaryControl(fldPlayerObject, 0);
    }
    D_0043EA44[0] = 4;
}
INCLUDE_SDATA(const s32, "code_0012C998", D_r1_0046B5E8);

INCLUDE_SDATA(const s32, "code_0012C998", raidoBasic);

INCLUDE_SDATA(const s32, "code_0012C998", raidoModel);

INCLUDE_SDATA(const s32, "code_0012C998", aiboBasic);

INCLUDE_SDATA(const s32, "code_0012C998", aiboModel);

INCLUDE_SDATA(const s32, "code_0012C998", summonDvBasic);

INCLUDE_SDATA(const s32, "code_0012C998", summonDvModel);

INCLUDE_SDATA(const s32, "code_0012C998", cameraBasic);

INCLUDE_SDATA(const s32, "code_0012C998", D_r1_0046B608);

INCLUDE_SDATA(const s32, "code_0012C998", D_004DE770);

INCLUDE_SDATA(const s32, "code_0012C998", D_004DE778);

INCLUDE_SDATA(const s32, "code_0012C998", D_r1_0046B690);

INCLUDE_SDATA(const s32, "code_0012C998", D_r1_0046B614);

INCLUDE_SDATA(const s32, "code_0012C998", D_r1_0046B618);

INCLUDE_SDATA(const s32, "code_0012C998", D_r1_0046B61C);

INCLUDE_SDATA(const s32, "code_0012C998", D_r1_0046B620);

INCLUDE_SDATA(const s32, "code_0012C998", D_r1_0046B624);

INCLUDE_SDATA(const s32, "code_0012C998", D_r1_0046B628);

INCLUDE_SDATA(const s32, "code_0012C998", D_r1_0046B62C);

INCLUDE_SDATA(const s32, "code_0012C998", D_r1_0046B630);

INCLUDE_SDATA(const s32, "code_0012C998", D_r1_0046B634);

INCLUDE_SDATA(const s32, "code_0012C998", D_r1_0046B638);

INCLUDE_SDATA(const s32, "code_0012C998", D_r1_0046B63C);

INCLUDE_SDATA(const s32, "code_0012C998", D_r1_0046B640);

INCLUDE_SDATA(const s32, "code_0012C998", D_r1_0046B644);

INCLUDE_SDATA(const s32, "code_0012C998", D_r1_0046B648);

INCLUDE_SDATA(const s32, "code_0012C998", D_r1_0046B64C);

INCLUDE_SDATA(const s32, "code_0012C998", D_r1_0046B650);

