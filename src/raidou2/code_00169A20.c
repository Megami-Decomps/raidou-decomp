#include "common.h"

/* Declarations from dds-decomp src/dds1/game/code_001DEFE0.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "effBattle.h"

#include "fileManager.h"

#include "newdata.h"

#include "sdk_libc.h"

#include "dds/btl.h"

#include "dds/btl_effect_position.h"

#include "dds/btl_sound.h"

#include "dds/btl_stage_task_cleanup.h"

#include "dds/dds3_fade.h"

#include "dds/dds3_fade_timing.h"

#include "dds/dds3_world_counter.h"

#include "dds/eff_random.h"

#include "dds/eff_random_mod.h"

#include "dds/evt_unit.h"

#include "dds/evt_unit_color_transition.h"

#include "dds/file_pac_completion_api.h"

#include "dds/file_request_api.h"

#include "dds/fld_area_path.h"

#include "dds/mdl_flag_api.h"

#include "dds/mdl_motion_api.h"

#include "dds/mnu_title_sound.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_delayed_resource_check_api.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_map_position_api.h"

#include "dds/sdf_trig.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/sdk_libc.h"

#include "dds/sdk_libm.h"

#include "dds/snd_sequence_volume_pan.h"

/* Convert three Euler angle components to the VU0 quaternion output.
 * Providers: DDS1 game/code_002E7C20.c and DDS2 game/code_00340AC8.c. */
void FromEuler(f32 x, f32 y, f32 z);

/* Multiply the quaternions in vf10 and vf11, writing the product to vf10. */
void Mult(void);

/* Convert the quaternion in vf10 into the primary matrix bank vf28-vf31.
 * Providers: DDS1 game/code_002E7C20.c and DDS2 game/code_00340AC8.c. */
void ToMatrix(void);

f32 acosf(f32 x);

f32 atanf(f32 x);

f32 cosf(f32 x);

/* Advance the wrapping 16-bit world object sequence carried in a u32. */
u32 dds3AllocObjectId(void);

void dds3FadeIn2(s32 duration);

/* Foreground alpha transitions, stepped by dds3FadeUpdate. */
void dds3FadeOut(s32 red, s32 green, s32 blue, s32 duration);

/* Advance caller-owned state, returning its next 32-bit value. NULL selects
 * the shared effect RNG state. */
u32 dds3GetRandA(EffRandState *state);

/* Return the next caller-owned or shared state value reduced modulo `modulus`. */
u32 dds3GetRandIntA(EffRandState *state, u32 modulus);

/* Disable configured timing while retaining the stored parameters. */
void dds3StopQuake(void);

/* Read the updater-owned frame counter through a borrowed handle view. */
u32 effBTLCutGetFrame(const effBTLCutHandle_t *effect);

void effBTLCutSetDrawEndFrame(effBTLCutHandle_t *effect, u32 endFrame);

s32 effBTLGetDrawEndFrame(const SoundMixer *mixer, EffBattleVoiceKind kind);

/* Frame queries borrow a mixer and only inspect its bank metadata. */
s32 effBTLGetMotionChangeFrame(const SoundMixer *mixer, EffBattleVoiceKind kind);

/* Leaves the unit's transformed muzzle position in vf10. */
void effBTLModelGetSphereCenter(struct BtlUnit *unit);

/* Return whether the selected flag bit is set, as EvtBitTestResult. */
EvtBitTestResult evtBitCheck(s32 flag);

/* Initialize an event unit's color transition for the requested duration. */
void evtSetUnitLightAndAmbient(evtUnitHandle_t *unit, s32 duration, u32 firstColor, u32 secondColor);

void evtSetUnitLightDir(evtUnitHandle_t *unit, EvtUnitDirectionTransitionFrames durationFrames);

void evtUnitLightOn(evtUnitHandle_t *unit);

/* Return the signed byte count recorded by the file transfer request. */
s32 fileGetLoadSize(const struct FileRequest *request);

/* Return the payload allocation descriptor. For normal file loads, freeing the
 * request wrapper does not free this allocation; its resource owner must keep
 * or release it separately. */
struct sdfMemHandle_t *fileGetMemHandle(const struct FileRequest *request);

/* Free a completed request wrapper; return one while it remains pending. */
FileRequestCleanupResult fileHandleFree(struct FileRequest *request);

/* Write the area directory path into buffer. The final argument is ignored. */
void fldMakeFilePath(char *buffer, s32 area, s32 unused);

/* Context-level motion APIs; per-sdfMotionCtrlr_t lifecycle methods are in sdf_motion.h. */
MdlMotionIndexResult mdlGetMotionNum(const dds3ModelHandle_t *ctx, MdlMotionSearchId searchId);

extern void *memcpy(void *, const void *, unsigned int);

void *memset(void *dst, s32 value, u32 size);

/* The voice-file RPC sets this busy state on open and clears it on close.
 * It is separate from the PCM callback-command busy latch. */
MnuTitleSoundBusyState p2sdGetPcmState(void);

/* Apply volume and pan to a sequence track through the command queue. */
void p2sdPlaySE(s32 trackId, s32 volume, s32 pan);

FilePacRequestCompletionResult pacCheckEnd(const struct FileRequest *request);

s32 pacFileCheckEnd(const struct FileRequest *request);

/* These title-specific wrappers forward an opaque stage-task owner and its
 * handled/not-handled result. */
s32 pacFileHandleFree_(void *request);

struct FileRequest *pacLoad(const char *requestName);

void pacSync(struct FileRequest *request);

void *sdfChipAllocZeroDirect(s32 size);

void sdfChipFreeDirect(void *memory);

f32 sdfCos(f32 angle);

SdfDelayedResourceCheckResult sdfDelayedResrcCheck(void);

/* Queue release of the sdfAllocAuto base in its source heap; interior pointers
 * are invalid. */
s32 sdfFreeAutoDelayed(void *data);

/* Queue the two heap owners separately: the general heap stores descriptors,
 * while the chip heap stores the address of the cell to release. */
void sdfMemFreeDelayedDirect(struct sdfMemHandle_t *allocation);

void sdfMemFreeDirect(struct sdfMemHandle_t *allocation);

/* Return the descriptor's payload base as a byte address, not the descriptor. */
u8 *sdfMemGetPtr(const struct sdfMemHandle_t *block);

s32 sdfMemGetSize(const struct sdfMemHandle_t *block);

void *sdfMemLock(struct sdfMemHandle_t *allocation);

/* On success, write the requested MPOS record's position or matrix to VU state. */
SdfMapPositionLookupResult sdfModelGetMarkPos(
    struct dds_sdfModelRootNode_t *model, s32 id);

f32 sdfSin(f32 angle);

f32 sinf(f32 x);

char *strcpy(char *dst, const char *src);

u32 strlen(const char *text);

f32 tanf(f32 x);

#include "dds/sdk_ps2.h"

#include "dds/snd_packed_track_load_status.h"

#include "dds/btl_vec3.h"

#include "dds/btl_motion_transform.h"

#include "dds/btl_unit_motion_duration_api.h"

#include "dds/btl_camera_pose_api.h"

#include "dds/btl_camera_snapshots.h"

#include "dds/btl_camera_cursor.h"

#include "dds/btl_camera_path_preset.h"

#include "dds/btl_pair_camera_preset.h"

#include "dds/btl_camera_parameter_record.h"

#include "dds/btl_camera_mode_index.h"

#include "dds/common.h"

#include "dds/snd_battle_sound_file_request_status.h"

#include "dds/sdf_map_position_api.h"

#include "dds/btl_action_entry_is_empty_api.h"

#include "dds/btl_horizontal_aim_api.h"

#include "dds/btl_linked_actor_eligibility.h"

#include "dds/btl_target_list_builder.h"

#include "dds/btl_unit_model_progress_api.h"

#include "dds/btl_quarter_health_result.h"

#include "dds/sdf_delayed_resource_check_api.h"

#include "dds/btl_camera_quaternion_pair.h"

#include "dds/btl_aim_horizontal_direction_clamped_api.h"

#include "dds/btl_set_actor_effect_parameter_api.h"

#include "dds/btl_action_camera_preset.h"

#include "dds/btl_actor_resource_bank_api.h"

#include "dds/btl_formation_spacing_cell.h"

#include "dds/btl_rain_effect_api.h"

#include "dds/btl_blend_color_api.h"

#include "dds/dds3_fade.h"

#include "dds/mnu_title_stream.h"

#include "dds/mnu_title_sound.h"

#include "dds/mnu_title_sound_buffer_status.h"

#include "dds/eff_random_mod.h"

#include "dds/btl_tint_transition.h"

#include "dds/btl_task_poll_result.h"

#include "dds/eff_random.h"

#include "dds/btl_camera_timed_instruction.h"

#include "dds/snd_sequence_volume_pan.h"

#include "dds/sdf_trig.h"

#include "dds/fld_area_path.h"

#include "dds/btl_scene_light_params.h"

#include "dds/btl_scene_lighting_task_args.h"

#include "dds/btl_stage_task_cleanup.h"

#include "dds/mdl_motion_api.h"

#include "dds/sdf_motion.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_packet_builders.h"

#include "dds/btl_effect_position.h"

#include "dds/btl_unit_geometry_api.h"

#include "dds/btl_effect_table.h"

#include "dds/sdf_chip.h"

#include "dds/snd_slot.h"

#include "dds/snd_track_load_activity.h"

#include "dds/active_sound_node.h"

#include "dds/kwln.h"

#include "dds/btl_task_state.h"

#include "dds/btl_task_condition.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_model.h"

#include "dds/btl.h"

#include "dds/btl_actor_work.h"

#include "dds/sdf_texture_offset_list.h"

#include "dds/btl_state.h"

#include "dds/btl_model_record.h"

#include "dds/btl_task_args.h"

#include "dds/btl_custom_fade_task_args.h"

#include "dds/btl_field_load_task_args.h"

#include "dds/file_pac.h"

#include "dds/btl_floor_load_task_args.h"

#include "dds/btl_floor_resource_readiness_api.h"

#include "dds/btl_sound.h"

#include "dds/btl_sound_resource_use_result.h"

#include "dds/btl_referenced_battle_effect_task.h"

#include "dds/btl_sound_release_task.h"

#include "dds/actor_effect_task.h"

#include "dds/eff_field_color.h"

#include "dds/dds3obj.h"

#include "dds/pcp_vu0.h"

#include "dds/ee_mmi.h"

#include "dds/fpu.h"

#include "dds/dat_state.h"

#include "dds/evt_unit.h"

#include "dds/evt_unit_color_transition.h"

#include "dds/mdl.h"

#include "dds/mdl_asset_request.h"

#include "dds/btl_action.h"

#include "dds/btl_unit_tasks.h"

#include "dds/sdf.h"

#include "dds/file.h"

#include "dds/dat_command.h"

#include "dds/file_request_api.h"

#include "dds/mdl_flag_api.h"

#include "dds/dds3_world_counter.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/dds3_fade_timing.h"

#include "dds/btl_camera_setup_policy.h"

/* Declared before first use (were implicit int). */
extern s32 btlGetSelectedCommandEffectTint(BtlUnit *unit);

extern void effBTLFieldColorResetFlags(void);

extern void effSweepFloorModelList(void);

extern void effTickSlotVolumeFade(void);

extern void evtFreeStage(void);

extern void evtFreeAllUnit(void);

extern void fileResetRenderFlags(void);

extern void fldSkyDrawFilter(void);

extern u32 fldSkyGetAlpha(void);

extern void fldSkySetAlpha(u32 drawState);

extern void fldUpdateSelectedValueTransition(void);

extern void fldSkyCalcFilter(void);

extern void mdlClearListedObjectFlag(void);

extern void mdlMarkAndProcessObjectNodes(void);

extern void mdlSetListedObjectFlag(void);

extern void mnuAdvanceTitleStateUnderSemaphore(void);

extern void mnuTitleStreamUpdateAndLogBgm(void);

extern s32 p2sdLoadPcmFile(const char *name, u8 channel);

extern void p2sdLoadSMGFile(s32 packed);

extern s32 sndIsCommandBusySigned(void);

extern void p2sdStopAllBgm(void);

extern void sdfSndQueueNoPayloadCommand210(void);

extern f32 D_00359EE0[4];

extern void effObjSetOpacityPassEnabled(u32 enabled);

extern void btlBossDebugPrintf(const char *format, ...);

extern u64 btlStartTask(BtlRuntimeTask *);

extern BtlCameraCursorView btlCameraCursor;

extern const BtlCameraTimedInstruction *D_0035D9F0[];

extern const BtlCameraTimedInstruction *D_0035DA08[];

extern void btlApplyTimedCameraInstruction(BtlLinkedCommand *, BtlCamState *, const BtlCameraTimedInstruction *);

extern s32 btlUpdateCameraCursorPose(BtlLinkedCommand *, BtlCamState *);

extern void btlAdvancePlayerCursorAnimation(BtlLinkedCommand *, BtlCamState *);

extern void btlAdvanceSkillCameraCursor(BtlLinkedCommand *, BtlCamState *);

extern void btlInitializeRandomCameraCursor(BtlLinkedCommand *, BtlCamState *);

extern void btlAdvanceCommandCursor(BtlLinkedCommand *, BtlCamState *);

extern void btlAdvanceTargetCursorAnimation(BtlLinkedCommand *, BtlCamState *);

extern void btlInitLinkedUnitActionCursor(BtlTask *);

extern s32 btlHasSingleLinkedResource(BtlLinkedCommand *);

extern BattleSoundArchiveEntry D_0035F748[BTL_SOUND_ENTRY_COUNT];

/* SYSEFF metadata and runtime registrations share these indices. */
enum { BTL_COMMAND_UNIT_EFFECT_SOUND_SLOT = 0x2C };

extern char D_003A5158[];

extern SndPackedTrackLoadStatus p2sdLoadCheckMidi(u32);

extern s64 btlForceSceneLightingRestore(SceneLightRestoreArgs *);

extern s32 btlRestoreSceneTransformLighting(SceneLightRestoreArgs *);

extern s64 btlForceReleaseTintTransition(const BtlTintReleaseArgs *args);

extern s32 sndPlaySkillSeTask(SkillSeTaskArgs *);

extern u32 sndFinishEarringPlayback(void);

extern BtlRuntimeTask *btlCreateMoveOtherUnitsTask(BtlUnit *, u32);

extern SoundResourceNode *sndAllocResourceNode(void);

extern u32 dds3DrawFlag;

extern u32 D_003BB694;

extern u32 btlTintTransitionHoldCount;

extern u8 D_0035F5D0[];

extern char D_003BB6B0[];

extern struct dds3Basic_t *dds3AddLightObjectBasic(s32);

extern void fldCamUpdateDefault(BtlLinkedCommand *, BtlCamState *);

extern void btlPrepareUnitPoseWithTiltRotation(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern void fldCamSetChase(BtlLinkedCommand *, BtlCamState *);

extern void btlApplyScaledUnitEffectParameter(BtlUnit *, s32, s32, f32);

extern u8 *datActionAnimationRecords;

extern s32 btlArrangeFormationSlots(s32 filter);

extern EffRandState effSharedRandomState[];

extern void sndResetTransition(void);

extern void btlResetMotionSoundFrameCounters(void);

extern void btlResetFieldColorAndSweepFlags(void);

extern void btlRefreshSoundEntries(void);

extern EffRandState effSharedRandomState[];

extern void sndSetStationedSeVolume(u32 sequence);

extern void btlResetTitleStreamOnBattleFlag(void);

extern void btlAdvanceWorldCounterAndSpawnActionObject(void);

extern s32 btlRepositionPartyAroundBattleCenter(void);

s32 btlGetEntryFlagsUnlessDisabled(dds_datUnitWork_t *entry);

struct evtUnitHandle_t;

extern void btlRefreshUnitEffectMotionAndEntry(BtlUnit *unit);

s32 btlGetLoggedIndexedCommandItem(s32 index);

extern s32 btlConstrainCameraEndpointHeight(BtlLinkedCommand *, BtlCamState *, s8, s8);

typedef struct BattleVoiceEntry {
    u8 format;
    u8 pad01[3];
    char fileName[0xC];
} BattleVoiceEntry;

#define CURSOR (&btlCameraCursor)

extern s32 btlRepositionPartyAroundBattleCenter(void);

extern BtlRuntimeTask *btlAllocTask(s32);

extern BtlRuntimeTask *btlAllocTask(s32);

extern struct SoundSlotOwner *sndAcquireSlotOwner(s32, s32);

extern void btlSetUnitPosition(BtlUnit *object, const f32 *position);

extern void btlSetUnitRotation(BtlUnit *object, const f32 rotation[4]);

extern void mdlGetPosition(dds3ModelHandle_t *);

extern u8 D_0037E110[];

extern void effBattleUpdate(effBTLCutHandle_t *);

extern f32 *D_00324770[];

extern u8 kwlnDefaultColorVector[];

extern void btlClearRuntimeFlag2000(void);

extern u8 D_0037E110[];

extern f32 btlUnitGetTopY(BtlUnit *);

/* With a qualifying unit, the span query leaves its last muzzle point in VF10. */
extern f32 btlMeasureMaskedUnitBounds(s32, f32 *, f32 *);

extern void btlPrepareActionCameraPoseWithActorClearance(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern void btlUpdateTiltedActionCameraPose(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern void btlFrameLeftmostUnitCameraPair(BtlCamState *, BtlCamState *, BtlCamState *);

extern void btlFlagUserAndTargetDefeat(BtlLinkedCommand *, BtlLinkedCommand *);

extern void btlInitTargetCursorAndFacing(BtlLinkedCommand *, BtlCamState *);

extern void btlInitializeCameraCursorForActorMode(BtlLinkedCommand *, BtlCamState *);

extern void btlFrameCommandTargetGroups(BtlLinkedCommand *);

extern void btlSetupActionCameraPair(BtlLinkedCommand *);

extern void btlBuildApproachCamera(BtlLinkedCommand *, BtlCamState *);

extern void btlUpdateActionTargetCameraPose(BtlLinkedCommand *);

extern void btlAdvanceCursorForUnmarkedUnit(BtlLinkedCommand *, BtlCamState *);

extern void btlRefreshActionPoseBlendSnapshot();

extern void btlAimEffectPoseAtUnit();

extern void btlBeginActionEndCameraPullback();

extern void btlUpdateApproachCameraPose(BtlLinkedCommand *, BtlCamState *);

extern void btlBuildHeightClampedApproachCamera(BtlLinkedCommand *, BtlCamState *);

s32 btlAdjustCameraDirectionForDefaultPlane(BtlCamState *state);

/* Adjust the pose direction using the supplied horizontal height plane. */ s32 btlAdjustCameraDirectionForPlane(BtlCamState *state, f32 height);

/* Allocate a native header followed by capacity pointer entries. */ BtlIndexList *btlAllocateIndexList(s32 capacity);

/* The caller keeps the live count within the allocated capacity. */ void btlAppendIndexListEntry(BtlIndexList *list, BtlUnit *entry);

void btlApplyCombinedActorFlags(BtlLinkedCommand *command);

void btlBlendUnitColor(BtlUnit *unit, u32 color, s32 mode);

extern void btlClearAllUnitDefeatCandidates(void);

void btlClearAllUnitDefeatCandidatesTask(void);

void btlClearUnitDefeatCandidate(BtlUnit *object);

void btlCopyUnitRotationQuaternion(BtlUnit *unit, f32 dst[4]);

/* Return the oldest task matching this handle; unstarted tasks may have handle 0. */ BtlRuntimeTask *btlFindTaskByHandle(u64 handle);

void btlFlagLinkedGroupDefeatCandidatesTask(BtlLinkedCommand *command);

void btlFlagUnitDefeatCandidate(BtlUnit *unit);

void btlFreeIndexList(BtlIndexList *list);

BtlCameraModeIndex btlGetCameraCommandMode(void);

/* Return the argument address recorded by allocation (zero for no arguments). */ void *btlGetTaskArguments(struct BtlRuntimeTask *task);

/* Find a type-two linked group whose associated unit is not disabled. */ s32 btlHasEligibleLinkedEntryTypeTwo(BtlLinkedCommand *actor);

s32 btlHasFirstLinkedCategoryFlag1000(BtlLinkedCommand *node);

/* Find an unmarked linked kind-two slot whose unit is not disabled. */ s32 btlHasIdleLinkedSlotKindTwo(BtlLinkedCommand *actor);

#include "dds/btl_marked_entry10_api.h"

void btlInterpolateVectorStep(BtlCamState *camera);

void btlReleaseUnitModelColorResource(BtlUnit *unit, s32 value, f32 scalar);

void effBTLModelGetMarkPosition2(BtlUnit *unit, s32 mode);

void btlSetUnitColor(BtlUnit *unit, u32 color, s32 mode);

void btlUnitFaceTarget(BtlUnit *object, BtlUnit *target);

void btlUnitFaceTargetScaled(BtlUnit *object, BtlUnit *target, f32 scale);

void btlCopyUnitCurrentPosition(BtlUnit *unit, f32 *destination);

/* vu0 routine: preserve the actor's primary position and rotation quaternion while * evaluating the requested model record; return the sampled vector in vf10. */ s32 btlSampleActorModelPositionVU(BtlUnit *unit, s32 value);

extern void btlApplyMarkedUnitActionCamera(BtlLinkedCommand *, BtlCamState *, BtlUnit *, s32);

extern void btlSetupCameraAimDualUnit(BtlLinkedCommand *, BtlCamState *, s32, f32, f32);

extern s32 btlRunTimedCameraSetupHook(BtlLinkedCommand *action);

extern s32 btlRunTimedCameraFinishHook(BtlLinkedCommand *action);

extern f32 fabsf(f32);

extern f32 btlUnitGetBottomY(BtlUnit *);

extern void btlFlagAllUnitsDefeatCandidate(void);

extern void SetRotateMatrixY(f32);

void btlBuildBattleUnitFramingCameraPose(BtlLinkedCommand *action, BtlCamState *out, f32 tiltDegrees);

void fldCamUpdateDefault(BtlLinkedCommand *command, BtlCamState *pose);

extern void SetRotateMatrixY(f32 angle);

void btlPrepareUnitPoseWithTiltRotation();

extern void btlFlagMatchingUnitsDefeatCandidate(s32);

void btlConfigureBattleCameraAction(BtlLinkedCommand *action, BtlCamState *from, BtlCamState *to);

void btlPrepareRandomizedActionCameraPose(BtlLinkedCommand *action, BtlCamState *from,
                                           BtlCamState *to);

extern void btlBuildActorPairFramingCameraPose(BtlLinkedCommand *, BtlCamState *, u8);

extern void btlBuildTargetGroupFramingCameraPose(BtlLinkedCommand *, BtlCamState *);

/* Build the from pose, then aim the to pose at the unit's muzzle using the pose row
 * for the side of the from view the muzzle lies on. */
void btlBuildSideSelectedActorCameraBlend(BtlLinkedCommand *action, BtlCamState *to, BtlCamState *from);

/* Build action camera endpoints with a FOV-dependent minimum distance. */
void btlBuildDistanceClampedActionCameraBlend(BtlLinkedCommand *action, BtlCamState *to, BtlCamState *from);

extern f32 btlGetActorEffectScale(BtlTask *);

extern void btlFlagMatchingUnitsDefeatCandidate(s32);

void btlSetupCameraPoseAimUnit(BtlLinkedCommand *action, BtlCamState *from, BtlCamState *to);

extern const BtlActionCameraPreset D_003A4000[8];

/* Frame the first listed target from an action-filtered pair preset: both
 * cameras start at its muzzle, lowered by a quarter of its height, at a
 * distance that fits the larger of its reach (at least 135, wider view) and
 * half its height. */
void btlBuildRandomTargetCameraStates(BtlLinkedCommand *action, BtlCamState *from, BtlCamState *to);

extern const BtlActionCameraPreset D_003A4190[8];

extern u8 D_003A4180[];

/* Frame all selected actor groups using an action-filtered camera preset. */
void btlBuildFilteredGroupCameraPair(BtlLinkedCommand *action, BtlCamState *from, BtlCamState *to);

extern void btlSetupCameraAimDualUnit(BtlLinkedCommand *, BtlCamState *, s32, f32, f32);

extern void btlFlagMatchingUnitsDefeatCandidate(s32);

void btlBuildGroupSizeActionCameraPair(BtlLinkedCommand *command, BtlCamState *front, BtlCamState *back);

void btlSetupCameraAimDualUnit(BtlLinkedCommand *command, BtlCamState *pose, s32 modeBits,
f32 angle, f32 blend);

extern void SetRotateMatrixY(f32);

void btlBuildActorPairFramingCameraPose(BtlLinkedCommand *action, BtlCamState *pose, u8 mode);

extern void btlFlagAllUnitsDefeatCandidate(void);

extern s32 btlPointInBox(BtlVec3 *, BtlVec3 *, BtlVec3 *);

extern void btlBuildActorPairFramingCameraPose(BtlLinkedCommand *, BtlCamState *, u8);

void btlBuildTargetGroupFramingCameraPose(BtlLinkedCommand *command, BtlCamState *pose);

/* vu0 routine: fit the action camera around one target and its reach. */
void btlFrameSingleTargetActionCamera(BtlLinkedCommand *action, BtlCamState *out);

void btlActionAimUserAtTargets(BtlLinkedCommand *command, BtlCamState *pose, BtlCamState *out);

s32 btlBuildMaskedGroupFramingCameraPose(BtlLinkedCommand *command, BtlCamState *camera, s32 mask,
                   f32 *direction, f32 distance, f32 xOffset);

extern s32 btlBuildMaskedGroupFramingCameraPose(BtlLinkedCommand *, BtlCamState *, s32, f32 *, f32, f32);

extern f32 D_003A4310[4];

/* Camera framing preset: from and to quaternions, their distance multipliers,
 * the camera parameter copied to the action, and padding. */
typedef struct CameraFramePose {
    f32 fromQuat[4];
    f32 toQuat[4];
    f32 fromDistance;
    f32 toDistance;
    f32 parameter;
    f32 pad2C;
} CameraFramePose;

extern CameraFramePose D_003A4320[4];

/* Frame the from and to poses around the action's targets from a random preset row
 * (rows 2-3 for animations with flag 0x200); targets in group 0x400 mirror the presets. */
void btlBuildRandomizedActionCameraPosePair(BtlLinkedCommand *action, BtlCamState *from, BtlCamState *to);

void btlFlagUserAndTargetDefeat(BtlLinkedCommand *command, BtlLinkedCommand *unused);

#define BTL_APPROACH_DIST_START 0.55f

#define BTL_APPROACH_DIST_END 0.6f

#define BTL_APPROACH_PITCH_START 0.6108652f /* 35 degrees */

#define BTL_APPROACH_PITCH_END 0.6108652f

/* vu0 routine: */
void btlBuildApproachCamera(BtlLinkedCommand *action, BtlCamState *out);

void btlSetupActionCameraPair(BtlLinkedCommand *command);

/* vu0 routine: update the action camera's saved target pose. */
void btlUpdateActionTargetCameraPose(BtlLinkedCommand *action);

extern u8 D_003A43E0[];

extern u8 D_003A43F0[];

void btlFrameCommandTargetGroups(BtlLinkedCommand *action);

extern BtlUnit *btlFindFarthestUnit(u32, f32 *);

void btlBuildActorGroupProjectionCameraPose(BtlLinkedCommand *source, BtlCamState *out,
                   BtlUnit *actor, s32 side);

extern BtlUnit *btlFindFarthestUnit(u32, f32 *);

extern BtlUnit *btlFindNearestUnit(u32, BtlUnit *);

extern void btlFlagAllUnitsDefeatCandidate(void);

/* Frame the camera for a two-actor command: place the eye so the actor, the farthest
   and the nearest actors fit the view, then store the resulting distance. */
void btlBuildShoulderActionCamera(BtlLinkedCommand *action, BtlCamState *pose, BtlUnit *unit, s32 mode);

extern void btlBuildActorGroupProjectionCameraPose(BtlLinkedCommand *, BtlCamState *, BtlUnit *, s32);

extern void btlBuildShoulderActionCamera(BtlLinkedCommand *, BtlCamState *, BtlUnit *, s32);

/* Apply the marked actor's action camera, honoring the runtime camera-arrangement hook. */
void btlApplyMarkedUnitActionCamera(BtlLinkedCommand *action, BtlCamState *pose,
                   BtlUnit *target, s32 mode);

extern f32 btlMeasureIndexListBounds(BtlIndexList *, f32 *, f32 *);

/* Use the defeat-camera hook or frame the selected actor set with the native extent rules. */
void btlBuildActorSetCameraPose(BtlLinkedCommand *action, BtlCamState *pose,
                   BtlUnit *startActor, BtlIndexList *indices, s8 mode);

/* vu0 routine: measure camera clearance from the actor's adjusted muzzle position. */
void btlPrepareActionCameraPoseWithActorClearance(BtlLinkedCommand *command, BtlCamState *pose, BtlCamState *out);

/* Forward the linked action and both poses to the tilt-rotation preparation. */
void btlUpdateTiltedActionCameraPose(BtlLinkedCommand *command, BtlCamState *pose, BtlCamState *out);

/* vu0 routine: frame the leftmost marked unit in two camera poses. */
void btlFrameLeftmostUnitCameraPair(BtlCamState *source, BtlCamState *from,
                   BtlCamState *to);

/* vu0 routine: frame the two unit groups using their bounding extents. */
void btlBuildGroupFramingCameraPose(BtlCamState *source, BtlCamState *out);

void fldCamSetChase(BtlLinkedCommand *command, BtlCamState *pose);

extern const BtlCameraQuaternionPair D_003A4408[3];

/* vu0 routine: frame the marked kind-11 unit that is earliest in its
 * animation; the back camera sits between the front camera and the group. */
void btlFrameEarliestAnimatedUnitCameraPair(BtlLinkedCommand *action, BtlCamState *from, BtlCamState *to);

/* vu0 routine: follow the tracked unit until its animation tail, then frame both groups. */
void btlStepUnitHeightFramedCamera(BtlLinkedCommand *action);

/* The pair preset row with an integer frame count in place of the motion parameter. */
typedef struct BattlePairCameraFramePreset {
    f32 fromQuaternion[4];
    f32 toQuaternion[4];
    f32 fromDistanceScale;
    f32 toDistanceScale;
    f32 fromHeightScale;
    s32 frames;
} BattlePairCameraFramePreset;

void btlBuildRandomDefeatCandidateCameraPair(BtlLinkedCommand *action, BtlCamState *from, BtlCamState *to);

/* cameraKind 7 participant preparation; the camera argument is unused. */
void btlPrepareCameraMode7Participants(BtlLinkedCommand *action, BtlCamState *camera);

/* vu0 routine: interpolate pull-back and pitch for actor/target framing. */
void btlBuildHeightClampedApproachCamera(BtlLinkedCommand *action, BtlCamState *out);

/* cameraKind 6 uses the same participant preparation through this separate entry. */
void btlPrepareCameraMode6Participants(BtlLinkedCommand *action, BtlCamState *camera);

/* Update the supplied pose with height-clamped actor/target approach framing. */
void btlUpdateApproachCameraPose(BtlLinkedCommand *action, BtlCamState *out);

extern void btlBuildFilteredGroupCameraPair(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

void btlChooseActionPoseBlendFromActorCount(BtlLinkedCommand *action, BtlCamState *from, BtlCamState *to);

void func_001E57F8(void);

extern s16 D_003BB670[];

extern s16 D_003BB678[];

extern s16 D_003BB680[];

extern u32 btlNextScaledRandom(u32);

extern void btlBuildPresetCameraPose(BtlLinkedCommand *, BtlCamState *, s16, s16);

extern void btlInitializeCameraCursorFromPreset(BtlLinkedCommand *, BtlCamState *, s32, s32);

/* Four retail flag/boolean sites: 1E5CE4/1E5CF0/1E5D3C/1E5D48; matched DDS2 predicate. */
static inline s32 btlHasFlag(u32 flags, u32 mask) {
    return (flags & mask) != 0;
}

void btlInitSkillCommandCursor(BtlLinkedCommand *command, BtlCamState *pose);

extern const BtlCameraTimedInstruction *D_0035DA28[];

extern const BtlCameraTimedInstruction *D_0035DAD0[];

void btlAdvancePlayerCursorAnimation(BtlLinkedCommand *action, BtlCamState *state);

/* Reset global cursor frame; flag0x400 actors select a Linkage skill preset. */
void btlSelectLinkageSkillCameraPreset(BtlLinkedCommand *action);

extern const BtlCameraTimedInstruction *D_0035DA40[];

/* Evaluate the skill camera script, apply its pose, then advance the cursor. */
void btlAdvanceSkillCameraCursor(BtlLinkedCommand *action, BtlCamState *state);

extern u32 btlNextScaledRandom(u32);

extern void btlBuildPresetCameraPose(BtlLinkedCommand *, BtlCamState *, s16, s16);

extern void btlInitializeCameraCursorFromPreset(BtlLinkedCommand *, BtlCamState *, s32, s32);

extern s16 D_0035D810[];

extern s16 D_0035D7F0[];

typedef struct BtlCursorChoices {
    u16 values[3][3][8];
} BtlCursorChoices;

extern const BtlCursorChoices D_003A4668;

/* Choose a camera preset by marked-party count, actor category and random draw. */
void btlInitializeRandomCameraCursor(BtlLinkedCommand *action, BtlCamState *state);

void btlAdvanceCommandCursor(BtlLinkedCommand *action, BtlCamState *state);

BtlUnit *btlFindLinkedActorById(BtlLinkedCommand *action, s32 id);

extern BtlCameraPathPreset D_00359EF0[18];

extern BtlCameraPathPreset D_0035A5B0[2];

extern BtlCameraPathPreset D_0035A670[2];

extern void btlUnitGetPosVU(BtlUnit *, u8);

/* vu0 routine: initialize the selected battle-camera path preset. */
void btlInitializeCameraCursorFromPreset(BtlLinkedCommand *action, BtlCamState *camera,
                   s32 bank, s32 presetIndex);

/* Camera clearance for a unit: mode 1 is the scaled camera radius plus any
 * negative horizontal offset; mode 0 is the larger of the scaled reach plus the
 * offset and half the scaled height plus any negative vertical offset. */
f32 btlGetUnitCameraClearance(BtlUnit *unit, u8 mode, f32 horizontalOffset, f32 verticalOffset);

void btlUnitGetPosVU(BtlUnit *unit, u8 mode);

extern BtlCameraSetupPolicy D_0035A730[8];

extern BtlCameraSetupPolicy D_0035AEB0[31];

extern BtlCameraSetupPolicy D_0035AAF0[7];

extern BtlCameraSetupPolicy D_0035ADF0[1];

extern BtlCameraSetupPolicy D_0035AD90[1];

extern BtlCameraSetupPolicy D_0035AA30[2];

extern BtlCameraSetupPolicy D_0035AE50[1];

extern f32 btlGetUnitCameraClearance(BtlUnit *, u8, f32, f32);

extern BtlUnit *btlFindActiveActorById(s32 id);

extern s32 btlCountUnitsByFlags(u32);

extern f32 btlMeasureBattleSideBoundsWithSeed(s32, f32 *, f32 *, const f32 *, const f32 *, const f32 *, f32);

extern f32 btlMeasureFilteredActorBounds(s32, BtlUnit *, BtlUnit *, s8, f32 *, f32 *, const f32 *, const f32 *);

extern f32 btlMeasureSideFilteredActorBounds(s32, BtlUnit *, BtlUnit *, s8, f32 *, f32 *, const f32 *, const f32 *, s8);

extern f32 btlBuildProjectedActorPairBoundsVU(s32, BtlUnit *, BtlUnit *, s8, s8, f32 *, f32 *, const f32 *, const f32 *);

extern f32 btlCameraTerminalPoint[4];

extern f32 btlCameraTerminalDirection[4];

extern f32 btlCameraTerminalEye[4];

extern f32 btlCameraTerminalDistance;

void btlBuildPresetCameraPose(BtlLinkedCommand *action, BtlCamState *out, s16 bank, s16 index);

extern void btlApplyScriptCameraTrack(BtlLinkedCommand *, BtlCamState *,
                         const BtlCameraTimedInstruction *, s32 *,
                         const BtlCameraParameterRecord *);

extern void btlApplyRadialLinkageFormation(BtlLinkedCommand *);

extern BtlCameraParameterSnapshot D_0035F230;

extern BtlCameraParameterSnapshot D_0035F330;

extern BtlCameraStoredPose D_0035F430;

extern BtlCameraStoredPose D_0035F560;

extern BtlCameraParameterSnapshot D_0035F460;

extern BtlCameraParameterRecord D_0035E000[];

extern BtlCameraParameterRecord D_0035E080[];

extern BtlCameraParameterRecord D_0035DD00[];

extern BtlCameraParameterRecord D_0035E100[];

extern BtlCameraParameterRecord D_0035E480[];

extern BtlCameraParameterRecord D_0035E780[];

extern BtlCameraParameterRecord D_0035E800[];

extern BtlCameraParameterRecord D_0035DB00[];

extern BtlCameraParameterRecord D_0035EB00[];

extern BtlCameraParameterRecord D_0035F080[];

extern s16 D_0035D800[];

typedef char CameraCursorExtent[(sizeof(BtlCameraCursorView) == 0x130) ? 1 : -1];

void btlApplyTimedCameraInstruction(BtlLinkedCommand *action, BtlCamState *pose,
                   const BtlCameraTimedInstruction *instruction);

/* vu0 routine: constrain a camera pose endpoint to the enabled height planes. */
s32 btlConstrainCameraEndpointHeight(BtlLinkedCommand *action, BtlCamState *pose, s8 bypassUpper, s8 bypassLower);

s32 btlUpdateCameraCursorPose(BtlLinkedCommand *action, BtlCamState *pose);

extern void btlScaleCameraTrackValues(BtlLinkedCommand *, BtlCamState *,
                         const BtlCameraTimedInstruction *, const s32 *,
                         const BtlCameraParameterRecord *, f32 *);

extern void SetRotateMatrixX(f32);

extern f32 btlCameraTerminalPoint[4];

extern f32 btlCameraTerminalDirection[4];

extern f32 btlCameraTerminalEye[4];

extern f32 btlCameraTerminalDistance;

void btlApplyScriptCameraTrack(BtlLinkedCommand *command, BtlCamState *pose,
                  const BtlCameraTimedInstruction *instruction, s32 *currentIndex,
                  const BtlCameraParameterRecord *records);

BtlUnit *btlFindActiveActorById(s32 id);

f32 btlMeasureBattleSideBoundsWithSeed(s32 sides, f32 *top, f32 *bottom, const f32 *from, const f32 *to, const f32 *extra, f32 extraRadius);

#include "dds/sdf_vu_matrix.h"

f32 btlMeasureFilteredActorBounds(s32 mask, BtlUnit *included, BtlUnit *excluded, s8 mode,
                  f32 *outMaxHeight, f32 *outMinHeight, const f32 *from, const f32 *to);

f32 btlMeasureSideFilteredActorBounds(s32 mask, BtlUnit *included, BtlUnit *excluded, s8 mode,
                  f32 *outMaxHeight, f32 *outMinHeight, const f32 *from, const f32 *to, s8 keepSide);

f32 btlBuildProjectedActorPairBoundsVU(s32 mask, BtlUnit *first, BtlUnit *second, s8 firstMode, s8 secondMode,
                  f32 *maxY, f32 *minY, const f32 *lineStart, const f32 *lineEnd);

s32 btlCountUnitsByFlags(u32 mask);

void btlInitializeCameraCursorForActorMode(BtlLinkedCommand *action, BtlCamState *state);

extern const BtlCameraTimedInstruction *D_0035DAE0[];

void btlAdvanceCursorForUnmarkedUnit(BtlLinkedCommand *action, BtlCamState *state);

void btlClearCommandCursorAndRunAction(BtlLinkedCommand *actor);

extern const BtlCameraTimedInstruction *D_0035DAF0[];

extern const BtlCameraTimedInstruction *D_0035DAF8[];

void btlAdvanceCommandCursorOrAction(BtlLinkedCommand *action, BtlCamState *state);

void btlInitCommandCursorForCategory(BtlLinkedCommand *action, BtlCamState *state);

/* Advance timed camera instructions, or use the action's approach camera. */
void btlAdvanceTimedCameraOrApproach(BtlLinkedCommand *action, BtlCamState *state);

void btlInitCommandCursorForFirstActor(BtlLinkedCommand *action, BtlCamState *state);

void btlAdvanceTargetCursorAnimation(BtlLinkedCommand *action, BtlCamState *state);

extern void btlRefreshUnitEffectMotionAndEntry(BtlUnit *unit);

extern void btlBuildPresetCameraPose(BtlLinkedCommand *, BtlCamState *, s16, s16);

extern void btlInitializeCameraCursorFromPreset(BtlLinkedCommand *, BtlCamState *, s32, s32);

void btlInitLinkedUnitActionCursor(BtlTask *task);

/* Initialize the target cursor and orient the linked unit toward its target. */
void btlInitTargetCursorAndFacing(BtlLinkedCommand *action, BtlCamState *state);

extern void btlBuildPresetCameraPose(BtlLinkedCommand *, BtlCamState *, s16, s16);

extern void btlInitializeCameraCursorFromPreset(BtlLinkedCommand *, BtlCamState *, s32, s32);

extern s32 btlConstrainCameraEndpointHeight(BtlLinkedCommand *, BtlCamState *, s8, s8);

void btlInitCursorAndApplyAction(BtlLinkedCommand *actor, BtlCamState *target);

void btlAdvanceWorldCounterAndSpawnActionObject(void);

BtlFloorResourceReadiness btlAreFrontAndSideFloorResourcesReady(void);

void btlReleaseWorkBuffers(void);

void btlWaitForPendingWorkAndReleaseBuffers(void);

extern char D_003A4AD8[];

extern char D_003A4AF0[];

extern char D_003A4B08[];

void btlFreeFieldBlocks(void);

extern f32 *D_00324770[];

extern u8 kwlnDefaultColorVector[];

extern void fldSkyBattle(void);

void btlInitTintTransitionResource(u32 resource, u16 soundId);

void btlInitializeSceneLightingAndTint(void);

void btlBuildActionLightParameters(s32 skillIndex, BtlSceneLightParams *light);

s32 btlGetActionDefaultOrOverride(s32 skillIndex);

u32 btlCameraVectorHasNaN(void);

typedef struct TintTransitionState {
    u32 sourceAlpha;
    u32 targetAlpha;
    u16 framesRemaining;
    u16 totalFrames;
} TintTransitionState;

extern TintTransitionState D_0035F5C0;

void btlInitTintTransitionResource(u32 resource, u16 soundId);

void btlInitTintTransitionDefault(u16 soundId);

void btlStepTintTransition(void);

void btlQueueTintTransition(u32 color, u16 frames);

void btlQueueTintTransitionToZero(u16 frames);

void btlStepBlendColor(void);

struct dds3Effect_Filter_t;

extern void dds3EffectFilter(struct dds3Effect_Filter_t *source);

void effBTLFieldFilterDraw(void);

void sndResetTransition(void);

void btlClearTintAndEnableCamera(void);

extern f32 *D_00324770[];

void btlUpdateTintAndWorldLight(void);

void btlTickFieldSwayAndTint(void);

extern WorldTransformSetup D_003D74A0;

extern void dds3SetLightObjectResoruce(dds3Basic_t *, WorldTransformSetup *);

extern void dds3LinkSceneLight(dds3Basic_t *object, s32 blendFrames);

/* Stage light/ambient RGB on the scene node and start its frames-long blend. */
void btlBeginSceneLightColorTransition(f32 *lightColor, f32 *ambientColor, s32 frames);

extern SdfFlagListParams D_0035F5F8;

void btlCreateRainEffect(u32 majorId, u32 minorId);

void btlDispatchLinkedEffectWhenBattleGatesClear(void);

extern char D_003A4B40[];

void btlStopRainSoundTransition(void);

extern char D_003A4B98[];

extern char D_003A4BB8[];

extern char D_003A4BC8[];

extern char D_003A4BE8[];

extern char D_003A4BF8[];

extern char D_003A4C10[];

extern s32 evtLoadArea_FromMemory(s32 area, s32 room, u8 *resource0Data, u8 *resource1Data,
                                            const SdfTextureOffsetListHeader *textureOffsets, s32 flags);

extern char D_003A4B50[];

extern char D_003A4B60[];

extern char D_003A4B78[];

BtlTaskPollResult btlPollFieldArchiveLoad(BtlFieldLoadArgs *args);

BtlRuntimeTask *fldCreateSceneTileTask(s32 stage, s32 variant);

BtlTaskPollResult btlPollFloorLoadTask(BtlFloorLoadArgs *args);

BtlRuntimeTask *btlCreateFloorLoadTask(s32 stage, s32 variant);

extern u32 btlBlendColorVec(f32 *, f32 *, f32);

/* Apply the scene blend and per-unit light colors, then retain a lighting hold. */
u32 btlApplySceneLightingTransition(SceneLightTransitionArgs *args);

extern u32 btlApplySceneLightingTransition(SceneLightTransitionArgs *);

BtlRuntimeTask *btlCreateEffectTaskWithSourceParams(const void *sourceParams, u32 value);

s32 btlRestoreSceneTransformLighting(SceneLightRestoreArgs *args);

/* Create the deferred lighting-restore task with its native four-byte argument. */
BtlRuntimeTask *btlCreateSceneLightingRestoreTask(u32 value);

/* Collapse lighting holds to one before the normal restore callback. */
s64 btlForceSceneLightingRestore(SceneLightRestoreArgs *arguments);

BtlRuntimeTask *btlCreateSoundUpdateTask(u32 value);

s32 btlQueueTintTransitionWhenEnabled(const BtlTintAcquireArgs *args);

BtlRuntimeTask *sndCreateAcquireTask(u32 color, u32 frames);

s32 sndTickFadeCounter(const BtlTintReleaseArgs *args);

BtlRuntimeTask *sndCreateReleaseTask(u32 frames);

/* Collapse tint holds to one before the existing tint-release callback. */
s64 btlForceReleaseTintTransition(const BtlTintReleaseArgs *args);

BtlRuntimeTask *btlCreateSoundReleaseTask(u32 frames);

void btlExtendTaskFrameLimit(SoundResourceNode *effect, s32 frames);

u32 sndGetResourceStatus(SoundResourceNode *sound);

s32 sndLookupResourceType(BtlUnit *actor, s32 resourceIndex);

void btlSetCommandEffectLinkFlags(BtlTask *task, BattleIndexWork *work);

void sndCreateSystemEffect(SoundResourceNode *effect);

void sndDeleteSystemEffect(SoundResourceNode *effect);

void sndAddEffectReferences(SoundEffectReferenceArgs *args);

extern void effBTLCutSetCutEndFrame(effBTLCutHandle_t *, s32);

/* Advance the effect until its battle or duration ends. */
BtlTaskPollResult btlUpdateReferencedBattleEffect(SoundEffectReferenceArgs *args);

void sndReleaseEffectReferences(SoundEffectReferenceArgs *args);

/* Create a deferred effect task whose start/finish callbacks balance resource and actor references. */
BtlRuntimeTask *btlCreateReferencedBattleEffectTask(SoundResourceNode *borrowedEffect,
                                                     BtlUnit *sourceOwner,
                                                     BtlUnit *targetOwner, EffBattleVoiceKind kind);

BtlRuntimeTask *sndCreateEffectWithTargets(SoundResourceNode *effect, BtlUnit *sourceOwner,
                                         s32 source, s32 target, BtlUnit *owner, EffBattleVoiceKind kind);

void sndStartEffectTask(ActorEffectTaskArgs *args);

s32 sndUpdateActorEffectTask(ActorEffectTaskArgs *args);

void sndFinishActorEffectTask(ActorEffectTaskArgs *args);

BtlRuntimeTask *sndCreateActorEffectTask(SoundResourceNode *source, BtlUnit *owner, u32 duration);

void sndIncrementEffectActiveCount(TimedUnitEffectArgs *args);

s32 sndWaitEffectFramesAndApplyUnitParameter(TimedUnitEffectArgs *args);

BtlRuntimeTask *sndCreateTimedUnitEffectTask(SoundResourceNode *effect, BtlUnit *actor, EffBattleVoiceKind kind,
                                          s32 channel, u32 volume);

void sndBeginEffectLoad(EffectLoadArgs *args);

extern char D_003A4C88[];

BtlTaskPollResult sndPollEffectLoad(EffectLoadArgs *args);

BtlRuntimeTask *sndCreateEffectLoadTask(SoundResourceNode *effect, const char *filename);

extern u32 btlWaitUnitListIdle(void);

u32 btlWaitUnitListIdle(void);

BtlRuntimeTask *btlCreateWaitUnitListIdleTask(u32 sound);

u32 sndApplyToActiveActors(u32 *soundId);

BtlRuntimeTask *btlCreateApplyToActiveActorsTask(u32 sound);

u32 btlCancelTimedFadeTask(void);

BtlRuntimeTask *btlCreateFadeStateResetTask(void);

void sndAddSourceReferences(SoundEffectSourceArgs *args);

/* Fade in the effect, hold it while the dependency task exists, then fade out. */
s32 sndUpdateEffectSourceFade(SoundEffectSourceArgs *args);

extern s32 sndUpdateEffectSourceFade(SoundEffectSourceArgs *);

void sndFinishEffectSourceTask(SoundEffectSourceArgs *args);

BtlRuntimeTask *sndCreateEffectSourceTask(SoundResourceNode *effect, BtlUnit *owner,
                                          u64 waitForTaskHandle);

u32 btlTaskStartFadeIn(u32 *taskArgs);

BtlRuntimeTask *btlCreateFadeInTask(u32 sound);

u32 btlTaskStartCustomFadeIn(BtlCustomFadeTaskArgs *taskArgs);

BtlRuntimeTask *sndCreateCustomTask(u32 packedColor, u32 duration);

u32 btlTaskSetBattleFlag40000(void);

BtlRuntimeTask *sndCreateSetBattleFlagTask(void);

u32 btlTaskClearBattleFlag40000(void);

BtlRuntimeTask *sndCreateClearBattleFlagTask(void);

extern const char D_003BB6A0[];

extern const char D_003A4CA8[16] __attribute__((aligned(8)));

extern const char D_003A4CB8[32] __attribute__((aligned(8)));

s32 btlFormatActionEventFilename(s32 skillIndex, char *output);

s32 sndSetEffectNodeParameter(SoundResourceNode *effect, EffBattleVoiceKind kind);

s32 sndGetEffectNodeParameter(SoundResourceNode *effect, EffBattleVoiceKind kind);

BtlSoundResourceUseResult sndIsResourceNodeReferencedOrActive(SoundResourceNode *effect);

s32 sndHasActiveActor(void);

/* Allocate a cleared resource node and prepend it to the runtime's resource list. */
SoundResourceNode *sndAllocResourceNode(void);

SoundResourceNode *sndCreateResourceNode(SoundMixer *sourceMixer);

/* Release owned voices, unlink the resource node, and free its allocation. */
void sndFreeResourceNode(SoundResourceNode *node);

/* Advance resource countdowns and battle tint, then tick slot-volume fades. */
void btlUpdateFadeColor(void);

void btlSweepFloorModelLists(void);

void btlResetFieldColorAndSweepFlags(void);

void sndClearResourceNodes(void);

void btlClearSoundAndModelResources(void);

/* Destroy all resource nodes; preserve each next link before freeing its owner. */
void sndClearResourceNodes(void);

s32 btlButtonMaskToIndex(u32 mask);

SoundResourceLink *sndAllocResourceLink(BtlUnit *owner);

void sndFreeResourceLink(SoundResourceLink *node);

void btlMarkTaskReady(SoundResourceLink *link);

SoundLink *sndAllocLink(BtlUnit *owner);

void sndFreeLink(SoundLink *node);

/* Blend the linked command effect through the SDK's packed-color vectors. */
void btlUpdateUnitCommandEffect(SoundLink *link);

/* Clear the fade gate and report completion; the frame updater may overwrite it. */
u32 btlDisableBattleFade(void);

BtlRuntimeTask *sndCreateClearStateTask(void);

/* Set the fade gate and report completion; the frame updater may overwrite it. */
u32 btlEnableBattleFade(void);

BtlRuntimeTask *sndCreateSetStateTask(void);

extern char D_003A5008[];

extern char D_003A5020[];

/* Consume archive records only for enabled SYSEFF rows; clear unavailable entries. */
void sndLoadSysEffLb(void);

void btlCreateIndexedSoundResourceNode(s32 slotIndex, SoundMixer *source);

/* Register available SYSEFF handles; unavailable slots are left untouched. */
void btlRefreshSoundEntries(void);

/* Free and clear slots selected by the archive table, without discarding metadata. */
void sndFreeBattleSoundEntries(void);

/* Register a borrowed archive source in its SYSEFF slot, without cloning it. */
void btlCreateIndexedSoundResourceNode(s32 slotIndex, SoundMixer *source);

#include "dds/btl_sound_handle_node.h"

SoundHandleNode *sndCreateSystemEffectHandle(struct dds3ModelHandle_t *actor, s32 index);

void btlUpdateJobPositionFromModel(SoundHandleNode *node);

void sndDestroyFileQueueWrapper(SoundHandleNode *node);

void func_r1_001DBF38(void);

void sndSetStationedSeVolume(u32 sequence);

void sndSetStationedSeHighVolume(u32 sequence);

extern s32 btlSceneStreamCycleEntries[];

extern u32 D_003BB6A8;

void sndStartBattleSceneStream(s32 adjustmentIndex, s32 sceneIndex);

u8 sndIsStreamStatusTwoOrThree(void);

void btlResetTitleStreamOnBattleFlag(void);

void btlAdvanceTitleState(void);

void btlAdvanceTitleStateWithAudioCleanup(void);

void btlAdvanceTitleStateTask(void);

void btlAdvanceTitleStateWithAudioCleanupTask(void);

s32 sndLoadAndPlayStationedSe(u32 soundId);

s32 sndPlayStationedSe(u32 *sound);

BtlRuntimeTask *sndCreateStationedSeTask(u32 soundId);

/* Return whether the independent active-node list contains a node with flag 8. */
s32 sndHasFlaggedActiveNode(void);

s32 sndPlaySkillSeTask(SkillSeTaskArgs *args);

BtlRuntimeTask *sndCreateSkillSeTask(const ActiveSoundNode *node, u16 variant);

void sndStartFileLoad(SoundFileTaskArgs *request);

extern char D_003A50D8[];

extern char D_003A50F0[];

extern char D_003A5110[];

extern char D_003A5138[];

extern void p2sdSetPcmBuffer(const void *, s32);

s32 sndPollMotSeFileAndSpu(SoundFileTaskArgs *request);

BtlRuntimeTask *sndCreateFileLoadTask(ActiveSoundNode *node, u32 skillIndex, const char *filename);

s32 sndLoadDataFile(const SoundDataFileArgs *data);

BtlRuntimeTask *sndCreateDataFileLoadTask(BtlUnit *unit);

s32 sndIsCommandBusySigned(void);

s32 sndHasResourceFlagsOneOrEight(ActiveSoundNode *node);

void sndFormatResourceNameFromIndex(s32 skillIndex, char *output);

void sndFormatResourceNameFromUnitMode(const BtlUnit *unit, char *output);

s32 sndMapResourceType(BtlUnit *actor, s32 resourceIndex);

/* Allocate a cleared active-sound node and prepend it to its independent list. */
ActiveSoundNode *sndAllocListNode(void);

/* Unlink and free an active-sound node without changing the resource-node list. */
void sndFreeListNode(ActiveSoundNode *node);

/* Clear active-sound nodes, saving next before each allocation is released. */
void sndClearList(void);

typedef struct SoundTaskArgs {
    BtlUnit *actor;
    s32 option;
    u32 slot;
    s32 waitFrames;
} SoundTaskArgs;

extern const SoundSlotTableEntry *btlSelectSideIndexedActorParameterTable(s32 category, s32 actorId);

/* Return the category/id/slot's packed motion-SE key, or zero if unavailable. */
u32 sndBuildMotSeResourceKey(const SoundSlotOwner *sound, u32 slot);

extern char D_003A5178[];

extern char D_003A5188[];

extern char D_003A5198[];

extern char D_003A51A8[];

/* Queue available motion-SE files into fileRequests, including slot 11's stream. */
void sndLoadMotSeFiles(SoundSlotOwner *sound);

/* Find the shared category/id owner, returning null if absent. */
SoundSlotOwner *sndFindListNodeForChannel(s32 category, s32 id);

extern char D_003A51D0[];

/* Retain or register an owner; model flag 0xC0F suppresses initial file queuing. */
SoundSlotOwner *sndAcquireSlotOwner(s32 category, s32 id);

/* The last reference cleans queued files and resource handles, then unlinks/frees. */
void sndReleaseSlotOwner(SoundSlotOwner *node);

/* Release once per owner; preserve the next link before a final release may free. */
void sndReleaseAllSlotOwners(void);

void btlStartMoveOtherUnitsTask(BtlUnit *unit, s32 soundId);

/* Flag 8 denotes packed-track loading, distinct from queued motion-SE files. */
SndTrackLoadActivity sndHasActiveFileLoad(void);

void btlQueueUnitSoundSlotFileLoad(SoundTaskArgs *args);

extern void mnuResetSoundBufferLocked(void);

extern void mnuReleaseSoundBufferLocked(void);

extern void mnuPrintTitleDebugBanner(void);

#include "dds/sdf_debug_api.h"

u32 sndPollMotionSePlayback(SoundTaskArgs *args);

BtlRuntimeTask *btlCreateMoveOtherUnitsTask(BtlUnit *unit, u32 soundId);

#include "dds/btl_sound_load_frame_counter.h"

/* Disable the motion- and skill-SE load-frame counters with their all-ones value. */
void btlResetMotionSoundFrameCounters(void);

void sndLoadBattleBank(void);

u8 sndIsBattleBankLoaded(void);

void btlPollMotionSoundResources(void);

/* Return whether a not-yet-file-ready owner still has an outstanding request. */
SndBattleSoundFileRequestStatus sndHasOccupiedNodeSlots(void);

u32 sndFinishEarringPlayback(void);

BtlRuntimeTask *sndCreateEarringTask(void);

extern BattleVoiceEntry D_00377650[];

extern char D_003A5328[];

extern char D_003A5340[];

extern char D_003A5358[];

BtlTaskPollResult sndPollAtrac3SELoadTask(Atrac3LoadTaskArgs *args);

BtlRuntimeTask *sndCreateAtracEffectLoadTask(u32 entryIndex);

extern char D_003A5370[];

void sndStartDeadAtracLoad(BtlDeadLoadArgs *args);

extern char D_003A5390[];

extern char D_003A53B0[];

extern char D_003A53D0[];

SndEarringPlaybackTaskResult sndUpdateEarringDeadPlayback(BtlDeadLoadArgs *args);

void sndFinishEarringPlaybackTask(BtlDeadLoadArgs *args);

BtlRuntimeTask *sndCreateEarringPlaybackTask(BtlUnit *unit);

u32 btlPlayStationedSe1C(void);

BtlRuntimeTask *btlCreateStationedSe1CTask(void);

u32 btlAdvanceTitleStateAfterSound(void);

BtlRuntimeTask *btlCreateAdvanceTitleStateTask(void);

extern f32 D_0035F9B0[4];

/* Orient party actors and expand their formation when the marked count grows. */
s32 btlExpandPartyFormationAroundCenter(f32 *center);

extern f32 D_0035F9C0[4];

extern f32 D_0035F9D0[4];

s32 btlArrangeFormationSlots(s32 filter);

extern s32 btlExpandPartyFormationAroundCenter(f32 *);

s32 btlRepositionPartyAroundBattleCenter(void);

s64 btlArrangeOpponentFormationSlots(void);

/* Place the three actor slots around the common battle center supplied in vf10. */
void btlPlaceTripleFormationAroundCenter(BtlTask *link, BtlUnit *first, BtlUnit *second);

extern void btlFlagAllUnitsDefeatCandidate(void);

extern void btlClearMatchingUnitDefeatCandidates(s32);

extern u8 D_0037E100[];

void btlPlaceTripleFormationAroundTarget(BtlTask *link, BtlUnit *first, BtlUnit *second);

/* vu0 routine: Place three indexed actors around the middle actor's facing and muzzle. */
void btlPlaceTripleFormationAroundMiddleActor(BtlTask *link, BtlUnit *first, BtlUnit *second);

void btlOrientFrontAndBackUnitsTowardTargets(BtlTask *link, BtlUnit *a, BtlUnit *b);

/* vu0 routine: measure the center actor's displacement from its sole target. */
void btlAlignTripleFormationWithTarget(BtlTask *link, BtlUnit *first, BtlUnit *second);

/* Enemy formation row kind 7 deliberately leaves the supplied participants unchanged. */
void btlEnemyMode7FormationNoOp(BtlTask *link, BtlUnit *first, BtlUnit *second);

enum {
    BATTLE_FORMATION_ACTION_FIRST = 0x1AB,
    BATTLE_FORMATION_ACTION_COUNT = 0x55
};

typedef struct BattleFormationActionTable {
    BtlEffectActionRecord rows[BATTLE_FORMATION_ACTION_COUNT];
} BattleFormationActionTable;

typedef char BattleFormationActionTableSizeCheck[(sizeof(BattleFormationActionTable) == 0x2A8) ? 1 : -1];

extern BattleFormationActionTable *D_003BAA64;

void btlApplyRadialLinkageFormation(BtlLinkedCommand *command);

typedef struct BattleFormationActionArgs {
    BtlTask *link;
    BtlUnit *first;
    BtlUnit *second;
    u32 actionId;
    u32 unk10;
} BattleFormationActionArgs;

typedef char BattleFormationActionArgs_size[(sizeof(BattleFormationActionArgs) == 0x14) ? 1 : -1];

u32 btlApplyLinkageFormation(BattleFormationActionArgs *args);

extern u32 btlApplyLinkageFormation(BattleFormationActionArgs *);

/* Package the action, two companion addresses, skill and flags for formation. */
BtlRuntimeTask *btlCreateLinkageFormationTask(BtlTask *owner, BtlUnit *firstActor,
        BtlUnit *secondActor, u32 skillId, u32 flags);
/* End of declarations from src/dds1/game/code_001DEFE0.c */

void btlClearTintAndEnableCamera(void) {
    btlQueueTintTransitionToZero(0);
    effObjSetOpacityPassEnabled(1);
}
