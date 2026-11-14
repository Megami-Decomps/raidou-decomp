#include "common.h"

/* Declarations from dds-decomp src/dds1/game/code_001D0BA0.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "dds3obj.h"

#include "effBattle.h"

#include "event.h"

#include "model.h"

#include "newdata.h"

#include "sdf_model_packet_callback.h"

#include "sdk_libc.h"

#include "dds/btl.h"

#include "dds/btl_effect_position.h"

#include "dds/btl_sound.h"

#include "dds/dds3_object_flags.h"

#include "dds/dds3_world_counter.h"

#include "dds/dds3_world_primary.h"

#include "dds/dds3obj.h"

#include "dds/eff_random.h"

#include "dds/eff_random_mod.h"

#include "dds/eff_transform.h"

#include "dds/evt_unit.h"

#include "dds/evt_unit_color_transition.h"

#include "dds/evt_unit_motion_api.h"

#include "dds/file_request_api.h"

#include "dds/mdl.h"

#include "dds/mdl_asset_request.h"

#include "dds/mdl_flag_api.h"

#include "dds/mdl_motion_api.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_dev_slot_api.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_map_position_api.h"

#include "dds/sdf_model.h"

#include "dds/sdf_motion.h"

#include "dds/sdf_packet_alloc.h"

#include "dds/sdf_packet_builders.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/sdk_libc.h"

#include "dds/snd_sequence_volume_pan.h"

/* Sparse world-node identities recovered from the paired operation tables and
 * concrete creators. Unlisted values stay unnamed; COUNT remains the full
 * directory extent rather than an enum-completeness claim. */
enum {
    EFF_WORLD_KIND_WORLD_INFO = 0,
    EFF_WORLD_KIND_EVENT_TABLE = 1,
    EFF_WORLD_KIND_WORLD_INDEX_OWNER = 2,
    EFF_WORLD_KIND_SLOT_RING = 3,
    EFF_WORLD_KIND_CAMERA = 4,
    EFF_WORLD_KIND_FOLLOW_MODEL = 5,
    EFF_WORLD_KIND_EFFECT_TRANSFORM = 6,
    EFF_WORLD_KIND_EFFECT_OBJECT = 7,
    EFF_WORLD_KIND_RESOURCE_OWNER = 8,
    EFF_WORLD_KIND_LIGHT = 9,
    EFF_WORLD_KIND_SCRIPT_OBJECT = 10,
    EFF_WORLD_KIND_ACTION_B = 11,
    EFF_WORLD_KIND_ACTION_D = 13,
    EFF_WORLD_KIND_ACTION_10 = 16,
    EFF_WORLD_KIND_TRANSFORM_SOURCE = 17,
    EFF_WORLD_KIND_COUNT = 18
};

/* Multiply the quaternions in vf10 and vf11, writing the product to vf10. */
void Mult(void);

enum {
    OBJECT_TRANSFORM_FLAG_UPDATE_PENDING = 1,
    OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID = 2,
    OBJECT_TRANSFORM_FLAG_USE_SMOOTHED_POSITION = 8
};

enum SdfMotionStateValue {
    SDF_MOTION_STATE_UNINITIALIZED = 0,
    SDF_MOTION_STATE_INITIALIZED = 1,
    SDF_MOTION_STATE_SAMPLING = 4,
    SDF_MOTION_STATE_TERMINAL = 5,
    SDF_MOTION_STATE_SUSPENDED = 6,
};

/* Convert the quaternion in vf10 into the primary matrix bank vf28-vf31.
 * Providers: DDS1 game/code_002E7C20.c and DDS2 game/code_00340AC8.c. */
void ToMatrix(void);

/* Advance the wrapping 16-bit world object sequence carried in a u32. */
u32 dds3AllocObjectId(void);

void dds3DeleteSceneBasicData(dds3Basic_t *node);

dds3Basic_t *dds3GetActiveCameraSceneBasic(dds3Basic_t *world);

/* Return the borrowed primary world node, or NULL when no world is active. */
struct dds3Basic_t *dds3GetActiveScene(void);

void dds3GetPosPoseSet(dds3Basic_t *object);

/* Returns 24 random bits scaled to [0, 1); NULL selects the shared state. */
f32 dds3GetRandFloatA(EffRandState *state);

/* Return the next caller-owned or shared state value reduced modulo `modulus`. */
u32 dds3GetRandIntA(EffRandState *state, u32 modulus);

void dds3GetRotPoseSet(dds3Basic_t *object);

void dds3ObjectBaseOffFlag(struct dds3Basic_t *object, u32 mask);

/* Set or clear requested bits in the object's flag word. */
void dds3ObjectBaseOnFlag(struct dds3Basic_t *object, u32 mask);

/* Forward a callback to the object's slot-1 handler when one is present; returns 1 if it did. */
u8 dds3ObjectBaseSetPadControl(dds3Basic_t *object,
                                   s32 (*update)(struct dds3PoseSet_t *, struct dds3Basic_t *));

dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *world, u32 key, s32 kind);

void effBTLCutSetDrawEndFrame(effBTLCutHandle_t *effect, u32 endFrame);

/* Leaves the unit's transformed muzzle position in vf10. */
void effBTLModelGetSphereCenter(struct BtlUnit *unit);

/* Return whether the selected flag bit is set, as EvtBitTestResult. */
EvtBitTestResult evtBitCheck(s32 flag);

void evtSetUnitColorA(evtUnitHandle_t *unit, s32 duration, u32 color);

void evtSetUnitColorRGB(evtUnitHandle_t *unit, s32 duration, u32 color);

/* Initialize an event unit's color transition for the requested duration. */
void evtSetUnitLightAndAmbient(evtUnitHandle_t *unit, s32 duration, u32 firstColor, u32 secondColor);

void evtSetUnitLightDir(evtUnitHandle_t *unit, EvtUnitDirectionTransitionFrames durationFrames);

void evtSetUnitMotion(evtUnitHandle_t *unit, s32 motionId, s32 startFrame,
                      s32 endFrame, EvtUnitMotionMode mode);

void evtSetWaitMotion(evtUnitHandle_t *unit, s32 value);

void evtUnitLightOn(evtUnitHandle_t *unit);

/* Return the payload allocation descriptor. For normal file loads, freeing the
 * request wrapper does not free this allocation; its resource owner must keep
 * or release it separately. */
struct sdfMemHandle_t *fileGetMemHandle(const struct FileRequest *request);

/* Free a completed request wrapper; return one while it remains pending. */
FileRequestCleanupResult fileHandleFree(struct FileRequest *request);

struct FileRequest *fileLoadUseTail(const char *requestName);

u32 mdlGetColor(const dds3ModelHandle_t *ctx);

u16 mdlGetMajorNum(const dds3ModelHandle_t *ctx);

u16 mdlGetMinorNum(const dds3ModelHandle_t *ctx);

/* Context-level motion APIs; per-sdfMotionCtrlr_t lifecycle methods are in sdf_motion.h. */
MdlMotionIndexResult mdlGetMotionNum(const dds3ModelHandle_t *ctx, MdlMotionSearchId searchId);

/* VU helpers transfer vectors through vf10 and basis rows through vf28-vf30. */
void mdlGetPosition(dds3ModelHandle_t *ctx);

void mdlGetRotate(dds3ModelHandle_t *ctx);

void mdlLink(dds3ModelHandle_t *ctx, struct sdfGrOt_t **surfaces);

void mdlLoopMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex);

/* A nonblocking request returns QUEUED when it starts work and
 * ALREADY_PENDING when work for the key already exists. Otherwise the result
 * is the ready dds3ModelResrc_t address. */
MdlAssetRequestResult mdlPrepare(
    s32 group, s32 id, MdlAssetRequestMode requestMode);

void mdlSetColor(dds3ModelHandle_t *ctx, u32 value);

void mdlSetEffectColor(dds3ModelHandle_t *ctx, u32 value);

void mdlSetPosition(dds3ModelHandle_t *ctx);

void mdlSetRotate(dds3ModelHandle_t *ctx);

void mdlSetScale(dds3ModelHandle_t *ctx);

/* sdfMotionCtrlr_t-record selection and lookup. */
void mdlSingleMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex);

extern void *memcpy(void *, const void *, unsigned int);

void *memset(void *dst, s32 value, u32 size);

/* Apply volume and pan to a sequence track through the command queue. */
void p2sdPlaySE(s32 trackId, s32 volume, s32 pan);

s32 pacFileCheckEnd(const struct FileRequest *request);

void *sdfChipAllocZeroDirect(s32 size);

void sdfChipFreeDirect(void *memory);

void sdfDrawTagInit(struct sdfDrawTag_t *list);

void sdfDrawTagLink(struct sdfDrawTag_t *list, u32 packetAddress);

/* Release the sdfAllocAuto base in its source heap; interior pointers are
 * invalid. */
s32 sdfFreeAuto(void *data);

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 size);

void sdfLinkLoadImage(
    sdfDrawTag_t *list, sdfGsMemSlice_t *source, s32 destinationX,
    s32 destinationY, s32 width, s32 height, s32 sourceAddress,
    s32 (*allocatePacket)(s32));

void sdfLinkStoreImage(
    sdfDrawTag_t *list, sdfGsMemSlice_t *source, s32 sourceX, s32 sourceY,
    s32 width, s32 height, s32 resourceAddress, s32 arg7, s32 arg8,
    s32 (*allocatePacket)(s32));

struct sdfMemHandle_t *sdfMemAllocDirect(s32 requestedBytes);

/* Queue the two heap owners separately: the general heap stores descriptors,
 * while the chip heap stores the address of the cell to release. */
void sdfMemFreeDelayedDirect(struct sdfMemHandle_t *allocation);

void sdfMemFreeDirect(struct sdfMemHandle_t *allocation);

void *sdfMemLock(struct sdfMemHandle_t *allocation);

/* Refresh the current per-node transforms from the model's selected frame. */
void sdfModelEvalTm(dds_sdfModelRootNode_t *model);

/* On success, write the requested MPOS record's position or matrix to VU state. */
SdfMapPositionLookupResult sdfModelGetMarkPos(
    struct dds_sdfModelRootNode_t *model, s32 id);

SdfMapPositionLookupResult sdfModelGetMarkPosMatrix(
    struct dds_sdfModelRootNode_t *model, s32 id);

dds_sdfModelRootNode_t *sdfModelMake(DevRequest *resourceList, sdfModelDef_t *listRef);

void sdfModelRootFree(struct dds_sdfModelRootNode_t *slot,
                      SdfModelRootCleanupPolicy rootCleanupPolicy,
                      SdfModelMaterialResourceReleasePolicy materialResourcePolicy);

void sdfMotionSeek(struct sdfMotionCtrlr_t *motion, f32 frame);

struct sdfDrawTag_t *sdfPrimAllocDrawTag(s32 (*allocator)(s32));

#ifndef DDS_ENUM_EvtUnitMotionState
#define DDS_ENUM_EvtUnitMotionState
typedef int EvtUnitMotionState;
#endif

#include "dds/btl_motion_selection_result.h"

#include "dds/sdk_ps2.h"

#include "dds/dds3_path.h"

#include "dds/sdf_model_packet_callback.h"

#include "dds/btl_motion_transform.h"

#include "dds/btl_unit_motion_duration_api.h"

#include "dds/btl_status_motion_override.h"

#include "dds/btl_reusable_model_api.h"

#include "dds/btl_camera_pose_step.h"

#include "dds/btl_actor_transform_parameters.h"

#include "dds/btl_select_transform_parameters_api.h"

#include "dds/common.h"

#include "dds/sdf_map_position_api.h"

#include "dds/btl_unit_defeat_by_delta_result.h"

#include "dds/btl_action_entry_is_empty_api.h"

#include "dds/btl_action_state_api.h"

#include "dds/btl_selected_entry_api.h"

#include "dds/btl_unit_model_progress_api.h"

#include "dds/btl_quarter_health_result.h"

#include "dds/btl_horizontal_aim_api.h"

#include "dds/btl_actor_command_selector_value_api.h"

#include "dds/dds3_camera_update_flags.h"

#include "dds/file_config_option_api.h"

#include "dds/btl_command_use_error.h"

#include "dds/btl_command_cost_api.h"

#include "dds/btl_action_reaction_motion.h"

#include "dds/btl_action_press_result.h"

#include "dds/btl_fear_infliction_roll.h"

#include "dds/btl_command_hit_result.h"

#include "dds/btl_command_status_api.h"

#include "dds/btl_hunt_mp_recovery_api.h"

#include "dds/btl_action_state_selection.h"

#include "dds/btl_scene_counter_mode.h"

#include "dds/btl_command_result_kind_api.h"

#include "dds/btl_command_target_policy.h"

#include "dds/btl_command_target_eligibility.h"

#include "dds/btl_actor_roll_eligibility.h"

#include "dds/btl_actor_mode_acceptance.h"

#include "dds/dds3_object_flags.h"

#include "dds/sdf_vu0.h"

#include "dds/btl_aim_horizontal_direction_clamped_api.h"

#include "dds/btl_set_actor_effect_parameter_api.h"

#include "dds/btl_raise_party_record_status_value.h"

#include "dds/btl_actor_resource_bank_api.h"

#include "dds/btl_unit_fade_task_args.h"

#include "dds/btl_unit_color_fade_args.h"

#include "dds/btl_special_ability.h"

#include "dds/eff_random_mod.h"

#include "dds/snd_sequence_volume_pan.h"

#include "dds/evt_world.h"

#include "dds/snd_pad.h"

#include "dds/eff_random.h"

#include "dds/btl_stage_task_cleanup.h"

#include "dds/mdl_motion_api.h"

#include "dds/sdf_motion.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_packet_builders.h"

#include "dds/btl_effect_position.h"

#include "dds/btl_unit_geometry_api.h"

#include "dds/btl_actor_entry_code_api.h"

#include "dds/eff_offset_if_owner_flag_clear.h"

#include "dds/sdf_chip.h"

#include "dds/snd_slot.h"

#include "dds/kwln.h"

#include "dds/btl_task_state.h"

#include "dds/btl_task_poll_result.h"

#include "dds/btl_task_condition.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_model.h"

#include "dds/sdf_model_release_api.h"

#include "dds/btl.h"

#include "dds/btl_index_list_search_api.h"

#include "dds/btl_skill_name_index.h"

#include "dds/btl_model_state_ownership.h"

#include "dds/btl_deferred_unit_status_args.h"

#include "dds/btl_position_effect_task_args.h"

#include "dds/btl_position_lerp_task_args.h"

#include "dds/btl_actor_work.h"

#include "dds/btl_find_actor_for_owner_api.h"

#include "dds/sdf_texture_offset_list.h"

#include "dds/btl_state.h"

#include "dds/btl_model_record.h"

#include "dds/btl_task_args.h"

#include "dds/btl_sound.h"

#include "dds/btl_selected_unit_effect_task_args.h"

#include "dds/btl_approach_task_args.h"

#include "dds/btl_unit_base_light_task_args.h"

#include "dds/btl_effect_node.h"

#include "dds/eff_field_color.h"

#include "dds/dds3obj.h"

#include "dds/pcp_vu0.h"

#include "dds/ee_mmi.h"

#include "dds/fpu.h"

#include "dds/dat_state.h"

#include "dds/evt_unit.h"

#include "dds/evt_unit_motion_api.h"

#include "dds/evt_unit_color_transition.h"

#include "dds/mdl.h"

#include "dds/mdl_asset_request.h"

#include "dds/btl_model_and_sound_readiness_api.h"

#include "dds/btl_action.h"

#include "dds/btl_unit_tasks.h"

#include "dds/sdf.h"

#include "dds/file.h"

#include "dds/dat_command.h"

#include "dds/btl_command_id.h"

#include "dds/btl_command_effect.h"

#include "dds/file_request_api.h"

#include "dds/dat_affinity.h"

#include "dds/mdl_flag_api.h"

#include "dds/dds3_world_counter.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/pty_adjust_item_quantity_api.h"

/* SYSEFF metadata and runtime registrations share these indices. */
enum { BTL_SELECTED_UNIT_EFFECT_SOUND_SLOT = 0x26 };

extern BtlUnit *btlSelectUnitAtExtremeX(BtlUnit *, BtlIndexList *);

void btlPrepareSavedCommandTargets(BtlTask *, BattleIndexWork *);

extern s32 btlIsUnitInActiveList(BtlUnit *unit);

extern void btlBossDebugPrintf(const char *format, ...);

extern u64 btlStartTask(BtlRuntimeTask *);

extern s32 btlHasSingleLinkedResource(BtlLinkedCommand *);

extern void btlResetCameraMotion(BtlLinkedCommand *);

extern s32 btlCountTasksForOwner(u64);

extern s32 btlUpdateUnitPositionInterpolationTask(BtlPosLerpTaskArgs *);

typedef struct BtlRotationTaskArgs BtlRotationTaskArgs;

extern s32 btlStepUnitRotationNlerp(BtlRotationTaskArgs *);

extern u64 btlAdvanceRuntimeSequenceCounter(void);

extern BtlRuntimeTask *btlAllocateIndexedUnitEffectTask(BtlUnit *, s32, s32, f32);

extern SndPad dds3PadButtonState;

extern u32 dds3DrawFlag;

extern void btlDestroyUnit(BtlUnit *);

extern void btlInheritActorModelState(BtlUnit *, BtlUnit *);

extern BtlRuntimeTask *btlDeferredTaskTail;

extern BtlRuntimeTask *btlDeferredTaskHead;

extern DatEnemyRecord *datEnemyRecords;

extern void btlApplyScaledUnitEffectParameter(BtlUnit *, s32, s32, f32);

extern BtlActionAnimationRecord *datActionAnimationRecords;

extern EffRandState effSharedRandomState[];

extern void btlResetDeferredTaskQueue(void);

s32 btlAdjustUnitHp(dds_datUnitWork_t *object, s32 delta);

s32 btlAdjustUnitMp(dds_datUnitWork_t *object, s32 delta);

u16 btlRefreshUnitMaximumHpAndClampCurrentHp(dds_datUnitWork_t *object);

u16 btlRefreshUnitMaximumMpAndClampCurrentMp(dds_datUnitWork_t *object);

extern EffRandState effSharedRandomState[];

extern SndPad dds3PadButtonState;

extern void btlInitializeWorldCamera(void);

/* No selected entry is represented by -1. */ void btlClearActorSelectedEntryIndex(BtlUnit *actor);

void btlClearAllActorEntrySlots(BtlUnit *unit);

s32 btlGetActorBedAssetIdFromIndex(s32 itemIndex);

s32 btlGetEntryFlagsUnlessDisabled(dds_datUnitWork_t *entry);

struct evtUnitHandle_t;

extern void evtSetWaitMotionSpeed(struct evtUnitHandle_t *, f32);

extern s32 btlTestActorStatusPredicate(BtlUnit *);

extern void btlApplyUnitModelScaledValue(BtlUnit *);

extern void btlApplyUnitMotionSelection(BtlUnit *, u32, s32, f32);

extern void btlRefreshUnitMotionSelection(BtlUnit *unit);

extern void btlRefreshUnitEffectMotionAndEntry(BtlUnit *unit);

extern void evtSetWaitHokanFrame(struct evtUnitHandle_t *, s32, s32);

extern s32 btlGetSlotRateKind(BtlUnit *, s32);

extern BtlRuntimeTask *btlCreateStiffenDamageShakeTask(BtlUnit *, f32);

s32 btlGetLoggedIndexedCommandItem(s32 index);

void btlSetTrackedTaskDisplayMode(s32 mode);

void btlClearPartyRecordStatusBits(dds_datUnitWork_t *record, s32 mask);

void btlApplyMaskedActorEntryAdjustments(BtlUnit *unit, u32 flags, s16 delta);

typedef struct BtlRotationTaskArgs {
    s128 from;
    s128 to;
    f32 rate;
    f32 t;
    s32 count;
    BtlUnit *unit;
} BtlRotationTaskArgs;

typedef char BtlRotationTaskArgsSizeCheck[
    sizeof(BtlRotationTaskArgs) == 0x30 ? 1 : -1];

extern s32 btlCountTasksByKind(u16 kind);

BtlRuntimeTask *btlCreateActorParameterDeltaTask(BtlUnit *owner, BtlOperandEntry *spec);

void btlResolveActionTargetResults(BtlTask *task, BattleIndexWork *work);

extern BtlRuntimeTask *btlCreateModelChangeTask(BtlUnit *unit, s32 resourceKind, s32 resourceId, s32 delay, s32 duration, u8 transitionMode);

extern s32 btlCountTasksByKind(u16 kind);

extern BtlRuntimeTask *btlCreateGunLoadPollTask(BtlUnit *);

extern BtlRuntimeTask *btlCreateUnitDefeatFadeInTask(BtlUnit *, s32);

extern void btlResetUnitLinks(BtlUnit *);

extern void btlReleaseUnitResources(BtlUnit *);

extern s32 btlCountTasksForOwner(u64);

extern s32 btlCountTasksForOwner(u64);

extern BtlRuntimeTask *btlCreateSelectedEffectUpdateTask(BtlUnit *);

extern BtlRuntimeTask *btlCreateUnitExitFadeTask(BtlUnit *, BtlUnitExitFadeMode);

extern void btlBossDebugPrintfN(s32, s32, s32, s32, ...);

BtlTask *btlFindUnitByActor(BtlUnit *target);

extern void scrSetGlobalSeenBit(u32 flagId);

void btlDispatchCommandViaHookOrDefault(BtlTask *command, BattleIndexWork *argument);

s32 btlIsSupportedCommandKind(const s32 *phase);

BtlCommandId btlResolveActionOperand(BtlUnit *unit, BattleIndexWork *work);

u32 btlClassifyActionOperand(BtlUnit *actor, BattleIndexWork *work);

BtlActionReactionMotionIndex btlClassifyActionResult(
    BtlUnit *actor, u32 actionKind, BtlActionPressResult press,
    u32 elementMask, s32 hitCount, u8 groupInactive, s32 commandIndex);

/* Test entry emptiness after command-category and operand-group-kind exclusions. */
BtlActionEntryEmptyResult btlActionEntryIsEmpty(BtlCommandId commandId, BtlOperandGroup *group,
                          BtlOperandEntry *entry);

/* Return whether any grouped operand has flag bit 0; the selector is unused. */
s32 btlHasAnyOperandLowFlag(s32 unused, BattleIndexWork *state);

extern void btlBuildCombinedActorUnit(BtlUnit *, BtlUnit *, BtlUnit *, BtlUnit *);

extern s32 btlShouldDistributeRandomTargetHits(BtlUnit *, BtlCommandId);

extern void btlDistributeRandomTargetHits(
    BtlUnit *, BtlIndexList *, BtlOperandGroup *, BtlCommandId);

extern void btlSortActorMuzzleDirections(BtlUnit *, BtlIndexList *, s32);

extern u32 btlEncodeActorIndexAsSelectionMask(u32);

extern s32 btlSelectedEntryHitsElement(BtlUnit *, BtlUnit *, s32);

extern BtlCommandHitResult btlResolveCommandHitResult(BtlUnit *, BtlUnit *, s32);

extern u8 btlSelectCommandHitCount(BtlUnit *, BtlCommandId);

extern s32 btlSelectActorAction(BtlUnit *actor);

extern s32 btlCalculateHuntEpReward(BtlUnit *acquirer, BtlUnit *enemy);

/* Native hunt interface passes actor and target; HP recovery is zero in this title. */
extern u32 btlGetHuntHpRecoveryAmount(BtlUnit *actor, BtlUnit *target);

/* Native caller supplies actor,target; concrete provider consumes only actor. */
extern u32 btlGetHuntPenaltyFlags(BtlUnit *unit, BtlUnit *enemy);

/* The damage/restore result is signed; keep this cross-unit declaration aligned with its provider. */
extern s32 btlCalculateHpAttackOrRestoreAmount(
    BtlUnit *, BtlUnit *, BtlCommandId, u32, BtlActionPressResult, s32, u32);

extern s32 btlCalculateMpAttackOrRestoreAmount(
    BtlUnit *, BtlUnit *, BtlCommandId, u32, BtlActionPressResult, s32, u32);

extern f32 btlGetActionCategoryMultiplier(BtlUnit *, s32, s32);

extern f32 btlGetActionCategoryGateAsFloat(s32, s32, s32);

/* Native resolver hook passes kind and press; both are unused by this title's no-op. */
extern void btlFindSoundTaskByWorkValue(s32 kind, BtlActionPressResult press);

extern BtlFearInflictionRollResult btlRollAllFearChance(s32, BtlUnit *, u32, s32, u8);

extern s32 btlRollStoneDeathChance(BtlUnit *, s32);

extern s32 btlCalculateEnemyExperienceReward(BtlUnit *, BtlUnit *);

extern s32 btlGetEnemyMoney(BtlUnit *, BtlUnit *);

extern u8 btlGetActorDisplayByteWithDefault(BtlUnit *object, s32 index);

extern s32 btlRollPassiveAbilityAction(BtlUnit *);

extern void fldAppendSceneGroupHandle(BtlTask *);

extern BtlCommandEffectId btlGetCommandEffectId(BtlTask *, BtlCommandId);

extern s32 btlCompareSkippedAndActiveTargetCounts(BtlIndexList *, BtlOperandGroup *);

extern u32 btlAdjustPointsForCombatFlags(u32, u32, u32, s32);

void btlResolveActionTargetResults(BtlTask *action, BattleIndexWork *work);

/* Reset work status and group headers, preserving the retained payload and allocation. */
void btlResetIndexWork(BattleIndexWork *work);

/* Allocate the index list and retained groups, then initialize their headers. */
void btlInitBattleIndexWork(BattleIndexWork *object);

/* Release each owned buffer once. The cached group address is deliberately not cleared. */
void btlReleaseObjectBuffers(BattleIndexWork *object);

extern void btlHighlightActorStatPanel(BtlUnit *, s8);

/* Apply the copied operand, then refresh the actor's status and defeat state. */
s32 btlApplyActorParameterDelta(BtlOperandTaskArgs *arguments);

extern BtlRuntimeTask *btlAllocTask(s32);

BtlRuntimeTask *btlCreateActorParameterDeltaTask(BtlUnit *owner, BtlOperandEntry *spec);

u32 btlApplyDeferredActorStats(BtlOperandTaskArgs *arguments);

BtlRuntimeTask *btlCreateDeferredActorStatsTask(BtlUnit *owner, BtlOperandEntry *spec);

u32 btlApplyDeferredUnitStatus(BtlDeferredUnitStatusArgs *arguments);

extern BtlRuntimeTask *btlAllocTask(s32);

extern u32 btlApplyDeferredUnitStatus(BtlDeferredUnitStatusArgs *);

BtlRuntimeTask *btlCreateDeferredUnitStatusTask(BtlUnit *owner, u32 value);

BtlTaskPollResult btlApplyCategoryStatDamage(BtlCategoryStatDamageArgs *args);

extern BtlTaskPollResult btlApplyCategoryStatDamage(BtlCategoryStatDamageArgs *);

BtlRuntimeTask *btlCreateCategoryStatDamageTask(BtlUnit *owner, s32 category, s32 amount);

/* Apply the operand's masked entry changes; complete immediately. */
s32 btlApplyMaskedActorEntryUpdate(BtlOperandTaskArgs *args);

/* Copy the operand into an owner-bound masked-entry-update task. */
BtlRuntimeTask *btlCreateMaskedActorEntryUpdateTask(BtlUnit *owner, BtlOperandEntry *spec);

u32 btlApplyQueuedActorEntrySelection(BtlOperandTaskArgs *taskArgs);

/* Queue the operand's actor entry selection in an owner-bound task. */
BtlRuntimeTask *btlCreateQueuedActorEntrySelectionTask(BtlUnit *owner, BtlOperandEntry *spec);

u32 btlClearQueuedActorEntrySelection(BtlUnit **taskArgs);

/* Create an owner-bound task to clear the actor's queued entry selection. */
BtlRuntimeTask *btlCreateClearActorEntrySelectionTask(BtlUnit *owner);

extern dds_datUnitWork_t *btlGetIndexedPartyEntryRecord(s32);

/* Award hunt experience; the sharing ability also rewards eligible party records. */
u32 btlAwardHuntExperience(BtlHuntExpArgs *args);

extern u32 btlAwardHuntExperience(BtlHuntExpArgs *);

BtlRuntimeTask *btlCreateActorSoundOptionTask(BtlUnit *actor, s32 option);

/* Consume one item when its table flag 4 is set, then signal task completion. */
typedef struct {
    BtlUnit *actor;
    s32 itemId;
} BtlCommandItemWork;

u32 btlConsumeFlaggedCommandItem(BtlCommandItemWork *work);

/* Queue an actor/item argument pair for conditional inventory consumption. */
BtlRuntimeTask *btlCreateFlaggedCommandItemConsumeTask(BtlUnit *actor, s32 itemId);

/* Add one to the argument's 16-bit item ID quantity, then complete the task. */
u32 btlIncrementCommandItemQuantity(BtlRuntimeTaskArgs *taskArgs);

/* Queue an owner-bound task that increments this item's inventory quantity. */
BtlRuntimeTask *btlCreateItemQuantityIncrementTask(BtlUnit *actor, u16 itemId);

u32 btlAddEpFromPacket(BtlRewardPacket *packet);

extern u32 btlAddEpFromPacket(BtlRewardPacket *);

BtlRuntimeTask *btlScheduleEpPacketTask(BtlUnit *actor, s32 amount);

u32 btlAddMoneyFromPacket(BtlRewardPacket *packet);

extern u32 btlAddMoneyFromPacket(BtlRewardPacket *);

BtlRuntimeTask *btlScheduleMoneyPacketTask(BtlUnit *actor, s32 amount);

u32 btlRefreshEligibleActors(void);

BtlRuntimeTask *btlCreateRefreshEligibleActorsTask(void);

extern char D_003A3A40[];

extern char D_003A3A50[];

void btlUpdateAutoMusic(void);

void btlPrepareSavedCommandTargets(BtlTask *task, BattleIndexWork *work);

/* Resolver hook receives kind and press; both are unused by this title's no-op. */
void btlFindSoundTaskByWorkValue(s32 kind, BtlActionPressResult press);

/* Return the oldest task matching this handle; unstarted tasks may have handle 0. */
BtlRuntimeTask *btlFindTaskByHandle(u64 handle);

/* Return the oldest task with this owner, or zero. */
BtlRuntimeTask *btlFindTaskByOwner(u64 owner);

/* Return the oldest registered task of this kind, or zero. */
BtlRuntimeTask *btlFindTaskByKind(u16 kind);

/* Count all registrations, including tasks awaiting startup or release. */
s32 btlCountRegisteredTasks(void);

/* Count registrations with this full-width owner key. */
s32 btlCountTasksForOwner(u64 owner);

/* Count registrations of the requested task kind. */
s32 btlCountTasksByKind(u16 kind);

/* Walk newest first and request release for bulk-release-eligible tasks. */
void btlFlagTasksForUpdate(void);

u8 btlEvalTaskCondition(BtlTaskCondition *condition, s32 currentCount);

/* Append a cleared task; positive size exposes argument bytes after the header. */
BtlRuntimeTask *btlAllocTask(s32 size);

/* Return the argument address recorded by allocation (zero for no arguments). */
void *btlGetTaskArguments(BtlRuntimeTask *task);

/* Invoke the finish hook before unlinking, then release the task block. */
void btlFreeTask(BtlRuntimeTask *task);

/* Install a fresh handle/reset phase counters, invoke startup, then reread handle. */
u64 btlStartTask(BtlRuntimeTask *taskObject);

void btlResetDeferredTaskQueue(void);

/* Advance a started task through wait/delay/update/release.
 * Fallthrough is intentional: zero delays permit all phases in one poll. */
void btlRunTask(BtlRuntimeTask *task);

/* Run ordinary registrations now; queue deferred registrations for the later pass. */
void btlSweepFinishedTasks(void);

/* Run deferred tasks, saving the next link before callbacks may free the task. */
void btlRunDeferredTasks(void);

/* Release newest first; cache the previous registration before its block is freed. */
void btlClearTaskLists(void);

/* Process callback for the zero-argument immediate-completion task. */
u32 btlCompleteTaskImmediately(void);

BtlRuntimeTask *btlCreateImmediateCompletionTask(void);

void btlDumpTaskQueue(void);

extern u128 btlDefaultUnitBodyOffset;

void btlInitUnitFxDefaults(BtlUnit *fx);

extern u128 btlDefaultUnitPosition;

extern u128 btlDefaultUnitRotation;

void btlInitFxLights(BtlUnit *fx);

void btlInitializeEffectVectorsFromSourceRecords(
    BtlUnit *fx, s32 resourceKind, s32 resourceIndex);

BtlReusableModelAvailability btlHasMatchingModel(s32 effectId, BtlModelId modelId);

/* Inherit the source unit's sound owner, transparency model, world node and
 * event work, and mark the destination as an inheriting unit. */
void btlInheritActorModelState(BtlUnit *dst, BtlUnit *src);

/* Kind-5 world-node payload attaches the event unit at +8. */
typedef struct EventUnitData {
    u8 pad00[8];
    evtUnitHandle_t *unit;
} EventUnitData;

extern s32 mdlSpawnCameraSlotViewerObject(s32, s32);

#include "dds/dds3_world_primary.h"

extern void mdlSetEffectScale(dds3ModelHandle_t *, f32);

extern struct SoundSlotOwner *sndAcquireSlotOwner(s32 category, s32 id);

extern void btlResetUnitModelProgress(BtlUnit *);

extern void btlSetUnitPosition(BtlUnit *object, const f32 *position);

extern void btlSetUnitRotation(BtlUnit *object, const f32 rotation[4]);

/* vu0 routine: initialize the actor world transform with the SDK unit vector. */
void btlBindActorModel(BtlUnit *unit, u32 kind, u32 id);

extern const char D_003A3AD0[];

void btlReleaseActorModelResources(BtlUnit *object);

void btlRequestModelAssetByMode(u32 unused, u32 effect, u32 model);

extern void btlReleaseFoundModelEntry(BtlModelResourceKind modelKind, BtlModelId modelId);

void btlReleaseModelAssetByMode(u32 unused, u32 effect, u32 model);

u8 btlCheckModelAssetByMode(u8 *object, u32 effect, u32 model);

void btlFlagUnitDefeatCandidate(BtlUnit *unit);

void btlClearUnitDefeatCandidate(BtlUnit *object);

s32 btlIsUnitInfoFlagOneEligible(BtlUnit *unit);

void btlApplyUnitMotionSelection(BtlUnit *unit, u32 index, s32 mode, f32 rate);

void btlRefreshUnitMotionSelection(BtlUnit *unit);

BtlActorModeAcceptanceResult btlIsActorModeAcceptedByBattleHook(BtlUnit *object);

extern void btlApplyUnitMotionSelection(BtlUnit *, u32, s32, f32);

void btlApplyScaledUnitEffectParameter(BtlUnit *unit, s32 index, s32 argument, f32 scale);

s32 btlGetSlotRateKind(BtlUnit *unit, s32 index);

void btlUpdateUnitEffects(void);

void btlApplyUnitModelScaledValue(BtlUnit *object);

void btlResetUnitModelProgress(BtlUnit *object);

BtlUnitMotionDurationFrames btlGetUnitMotionDurationFrames(const BtlUnit *unit, s32 motionIndex);

/* Return the unit model's current frame, or zero while its model flag is clear. */
f32 btlGetUnitModelCurrentFrame(const BtlUnit *unit);

/* The caller's frame argument is forwarded unchanged to the sampler. */
void btlAdvanceUnitModelFrame(BtlUnit *unit, f32 frame);

BtlUnitModelFrameCount btlGetUnitModelFrameCount(const BtlUnit *unit);

void btlSeekUnitModelFrameZero(BtlUnit *unit);

void btlSeekRandomModelFrame(BtlUnit *object);

u32 btlIsUnitModelStateFive(BtlUnit *object);

void btlSetUnitPosition(BtlUnit *object, const f32 *position);

/* Copy the stored current-position vector, without the runtime world offset. */
void btlCopyUnitCurrentPosition(BtlUnit *unit, f32 *destination);

void btlGetUnitWorldPos(BtlUnit *unit, f32 *worldPosition);

extern void btlRefreshUnitFxVectors(BtlUnit *);

BtlActorEffectPositionResult btlSetActorEffectParameter();

void effBTLModelGetMarkPosition2(BtlUnit *unit, s32 mode);

/* vu0 routine: preserve the actor's primary position and rotation quaternion while
 * evaluating the requested model record; return the sampled vector in vf10. */
s32 btlSampleActorModelPositionVU(BtlUnit *unit, s32 value);

s8 btlSetActorAlternateEffectParameter();

void effBTLModelGetMarkMatrix2(BtlUnit *unit, s32 mode);

s32 btlIsUnitAtStoredPosition(BtlUnit *unit);

extern u8 D_003A3B70[];

void btlSetUnitRotation(BtlUnit *object, const f32 rotation[4]);

void btlCopyUnitRotationQuaternion(BtlUnit *unit, f32 dst[4]);

void btlSetUnitColor(BtlUnit *unit, u32 color, s32 mode);

void btlBlendUnitColor(BtlUnit *unit, u32 color, s32 mode);

void btlReleaseUnitModelColorResource(BtlUnit *unit, s32 value, f32 scalar);

void btlRefreshUnitFxVectors(BtlUnit *unit);

void btlUnitFaceTarget(BtlUnit *object, BtlUnit *target);

void btlUnitFaceTargetScaled(BtlUnit *object, BtlUnit *target, f32 scale);

void btlCopyUnitStats(BtlUnit *unit, dds_datUnitWork_t *source);

extern void mdlLinkEffect(dds3ModelHandle_t *, dds3ModelEffectCtrlr_t *);

#include "dds/sdf_packet_alloc.h"

extern void sdfModelCopyStat(dds_sdfModelRootNode_t *, const dds_sdfModelRootNode_t *);

extern u64 btlTransparencySurfaceTestValues[4];

/* Draw the model into four surfaces in three GS TEST passes, then update its anchors. */
void btlDrawUnitTransparencyPasses(BtlUnit *unit, dds3ModelHandle_t *model, dds_sdfModelRootNode_t *overlay, sdfGrOt_t **surfaces, u32 color);

void btlCreateUnitTransparency(BtlUnit *unit);

extern struct sdfGrOt_t *D_dds1_00325788[13][4];

extern sdfGrOt_t *btlUnitTransparencySurfaces[];

void btlUpdateUnitTransparency(BtlUnit *unit);

extern sdfGrFrameSet_t sdfGrFrameSet;

extern sdfGrOt_t *btlMirrorSourceTransparencySurfaces[];

extern sdfGrOt_t *btlMirrorTargetTransparencySurfaces[];

/* Draw the unit's transparency model into its mirror's packet buffer, then draw the mirror from it. Either
 * model is created (and the update ends) on the first frame it is missing. */
/* Emit the mirror model's transparency packets using its overlay color. */
void btlDrawUnitMirrorTransparency(BtlUnit *unit);

extern char D_003A3BA8[];

extern char D_003BB5F8[];

s32 btlFormatUnitBedName(BtlUnit *actor, char *filename);

extern u8 D_0037E110[];

extern const char btlUnitWarpFormat[16];

void btlWarpUnitToMotionReach(BtlUnit *unit, BtlUnit *target, s32 index);

void btlRefreshUnitEffectMotionAndEntry(BtlUnit *unit);

u32 btlApplyIndexedUnitEffectTask(BtlRuntimeTaskArgs *taskArgs);

BtlRuntimeTask *btlAllocateIndexedUnitEffectTask(BtlUnit *owner, s32 index, s32 value, f32 scale);

u32 btlApplyScaledUnitModelTask(BtlUnit **taskArgs);

BtlRuntimeTask *btlCreateScaledUnitModelTask(BtlUnit *owner);

BtlTaskPollResult btlPollThresholdTask(BtlThresholdTaskArgs *arguments);

BtlRuntimeTask *btlScheduleThresholdTask(BtlUnit *actor, s32 option);

s32 btlApproachTargetTask(BtlApproachTaskArgs *args);

extern s32 btlApproachTargetTask(BtlApproachTaskArgs *);

BtlRuntimeTask *btlAllocateApproachTargetTask(BtlUnit *unit, BtlUnit *target, f32 scale);

s32 btlUpdateUnitPositionInterpolationTask(BtlPosLerpTaskArgs *args);

BtlRuntimeTask *btlCreateUnitPositionLerpTowardTargetTask(BtlUnit *unit, const f32 target[4], f32 scale);

extern void Lerp(f32);

s32 btlStepUnitRotationNlerp(BtlRotationTaskArgs *args);

BtlRuntimeTask *btlCreateUnitRotationInterpolationTask(BtlUnit *unit, const f32 targetOrientation[4], f32 interpolationRate);

extern char D_003A3BC8[];

extern char D_003A3BE8[];

void btlRequestModelOrReuse(BtlRuntimeTaskArgs *arguments);

extern char D_003A3C08[];

extern u8 btlCheckModelAssetByMode(u8 *, u32, u32);

BtlTaskPollResult btlPollModelLoadCompletion(BtlRuntimeTaskArgs *arguments);

BtlRuntimeTask *btlCreateModelLoadPollTask(BtlUnit *owner, u32 effectId, u32 modelId, s8 preserveDefeatCandidate);

u32 btlReleaseUnitModelTask(BtlUnit **taskArgs);

BtlRuntimeTask *btlScheduleRefreshTask(BtlUnit *owner);

void btlBeginModelChange(BtlModelChangeArgs *arguments);

/* Complete model loading, cross-fade the retained actor, and release it. */
/* Advance model replacement and mirror crossfade; return one after cleanup. */
u32 btlUpdateModelChangeTask(BtlModelChangeArgs *args);

extern u32 btlUpdateModelChangeTask(BtlModelChangeArgs *);

BtlRuntimeTask *btlCreateModelChangeTask(BtlUnit *unit, s32 resourceKind, s32 resourceId, s32 delay, s32 duration, u8 transitionMode);

void btlApplyLinkedUnitStatusWhenActorActive(BtlUnitLightAndAmbientTaskArgs *taskArgs);

u32 btlApplyUnitFxWhenLoaded(BtlUnitLightAndAmbientTaskArgs *taskArgs);

u8 *btlCreateUnitTask0F(BtlUnit *unit, s32 firstColor, s32 secondColor, s32 duration);

typedef struct BtlUnitLightDirectionTaskArgs {
    f32 direction[4];
    s32 directionMode;
    BtlUnit *unit;
} BtlUnitLightDirectionTaskArgs;

typedef char BtlUnitLightDirectionTaskArgs_size_check[
    sizeof(BtlUnitLightDirectionTaskArgs) == 0x18 ? 1 : -1];

void btlPrepareUnitStatusFxOnStart(BtlUnitLightDirectionTaskArgs *task);

s32 btlApplyUnitVectorFxWhenLoaded(BtlUnitLightDirectionTaskArgs *task);

BtlRuntimeTask *btlCreateUnitTask10(BtlUnit *unit, f32 *direction, s32 directionMode);

u32 btlUnitFadeInTask(BtlUnitColorFadeArgs *args);

extern u32 btlUnitFadeInTask(BtlUnitColorFadeArgs *);

BtlRuntimeTask *btlCreateUnitFadeInTask(BtlUnit *owner, u32 value, u32 variant);

u32 btlUnitFadeOutTask(BtlUnitColorFadeArgs *args);

extern u32 btlUnitFadeOutTask(BtlUnitColorFadeArgs *);

BtlRuntimeTask *btlCreateUnitFadeOutTask(BtlUnit *owner, u32 value, u32 variant);

u32 btlStepUnitDefeatFadeIn(BtlUnitFadeTaskArgs *taskArgs);

BtlRuntimeTask *btlCreateUnitDefeatFadeInTask(BtlUnit *owner, s32 duration);

u32 btlStepUnitDefeatFadeOut(BtlUnitFadeTaskArgs *taskArgs);

/* Create the owner-bound defeat fade-out task with a zero elapsed count. */
BtlRuntimeTask *btlCreateUnitDefeatFadeOutTask(BtlUnit *unit, s32 duration);

/* Arguments hold unit, mode and elapsed ticks; return one after the exit fade. */
u32 btlStepUnitExitFade(BtlUnitFadeTaskArgs *taskArgs);

/* Queue the selected exit-fade mode for this owner, starting at tick zero. */
BtlRuntimeTask *btlCreateUnitExitFadeTask(BtlUnit *owner, BtlUnitExitFadeMode mode);

extern void effBattleUpdate(effBTLCutHandle_t *);

/* Start from the selected-unit SYSEFF source, then update through its duration. */
BtlSelectedUnitEffectTaskResult btlUpdateSelectedUnitEffect(BtlSelectedUnitEffectTaskArgs *arguments);

void btlFinishSelectedUnitEffect(BtlSelectedUnitEffectTaskArgs *arguments);

BtlRuntimeTask *btlCreateSelectedEffectUpdateTask(BtlUnit *unit);

extern void btlUpdateUnitActors(void);

u32 btlUpdateCommandSoundTask(void);

BtlRuntimeTask *btlCreateCommandSoundUpdateTask(void);

extern void btlRefreshUnitEffects(void);

u32 btlUpdateCommandSoundTaskSecondary(void);

BtlRuntimeTask *btlCreateSecondaryCommandSoundTask(void);

/* Complete the empty task on its first process call; no sound operation is performed. */
u32 btlFinishNoOpTask(void);

/* Allocate the zero-argument, kind-0x20 task with an unconditional completion callback. */
BtlRuntimeTask *btlCreateNoOpTask(void);

extern f32 *D_00324770[];

extern u8 D_dds1_00324780[];

u32 btlUnitBaseLightTask(BtlUnitBaseLightArgs *work);

BtlRuntimeTask *btlCreateUnitBaseLightTask(BtlUnit *owner);

extern char D_003A3CA0[];

typedef struct BtlDamageShakeArgs {
    BtlUnit *unit;
    f32 amplitude;
    s32 tick;
} BtlDamageShakeArgs;

u32 btlStiffenDamageShakeStep(BtlDamageShakeArgs *task);

extern u32 btlStiffenDamageShakeStep(BtlDamageShakeArgs *);

BtlRuntimeTask *btlCreateStiffenDamageShakeTask(BtlUnit *owner, f32 value);

/* Request the position-effect motion or advance its fallback position curve. */
u32 btlAdvanceActorPositionEffectTask(BtlPositionEffectArgs *task);

extern u32 btlAdvanceActorPositionEffectTask(BtlPositionEffectArgs *);

/* Queue the actor's position effect with its tick counter reset. */
BtlRuntimeTask *btlCreateActorPositionEffectTask(BtlUnit *unit);

/* Apply the queued actor's stored orientation and complete immediately. */
u32 btlApplyQueuedActorRotation(BtlRuntimeTaskArgs *taskArgs);

BtlRuntimeTask *btlScheduleActorUpdate(BtlUnit *unit);

u32 btlRefreshUnitFxVectorTask(BtlUnit **taskArgs);

BtlRuntimeTask *btlCreateUnitFxVectorRefreshTask(BtlUnit *owner);

typedef struct GunLoadArgs {
    BtlUnit *unit;
    struct FileRequest *request;
} GunLoadArgs;

void btlStartGunFinishLoad(GunLoadArgs *taskArgs);

BtlTaskPollResult btlPollGunLoad(GunLoadArgs *loadArgs);

extern void btlStartGunFinishLoad(GunLoadArgs *);

extern BtlTaskPollResult btlPollGunLoad(GunLoadArgs *);

BtlRuntimeTask *btlCreateGunLoadPollTask(BtlUnit *owner);

u32 btlUpdateUnitEffectsTask(void);

BtlRuntimeTask *btlCreateUpdateUnitEffectsTask(void);

extern void btlCreateUnitTransparency(BtlUnit *);

typedef struct BtlActorTransparencyArgs {
    BtlUnit *unit;
} BtlActorTransparencyArgs;

s32 btlCreateActorTransparency(BtlActorTransparencyArgs *arguments);

BtlRuntimeTask *btlCreateActorTransparencyTask(BtlUnit *actor);

/* Advance the queued model-motion blend in two stages; return one when done. */
BtlTaskPollResult btlAdvanceActorModelBlendTask(BtlActorModelBlendArgs *args);

BtlRuntimeTask *btlCreateActorModelBlendTask(BtlUnit *actor, BtlUnit *target, s32 index, s32 value, f32 scale);

/* Update motion completion, alpha transitions, and the selected-unit color pulse. */
void btlUpdateActiveActorModelState(void);

extern void btlDrawUnitStatusShadowDisc(BtlUnit *unit);

void btlUpdateActorModelColorAndLinks(void);

void btlResetUnitLinks(BtlUnit *actor);

BtlUnit *btlCreateUnit(void);

void btlReleaseUnitResources(BtlUnit *unit);

extern char D_003A3D38[];

void btlDestroyUnit(BtlUnit *unit);

void btlDestroyAllUnits(void);

void btlRemoveActorsWithFlags(u32 mask);

BtlUnit *btlFindActorForOwner(BtlUnitIdentity identity);

s32 btlIsUnitInActorList(const BtlUnit *candidate);

BtlUnit *btlFindUnitByModeClear(s32 unitId);

BtlUnit *btlFindUnitByModeFlagged(s32 unitId);

/* Allocate a native header followed by capacity pointer entries. */
BtlIndexList *btlAllocateIndexList(s32 capacity);

void btlFreeIndexList(BtlIndexList *list);

/* The caller keeps the live count within the allocated capacity. */
void btlAppendIndexListEntry(BtlIndexList *list, BtlUnit *entry);

/* Clear the live count without changing storage or its existing entries. */
void btlClearIndexList(BtlIndexList *list);

/* Return the number of live entries, not the allocated capacity. */
u32 btlGetIndexListCount(BtlIndexList *list);

/* The caller supplies an in-range index. */
BtlUnit *btlGetIndexListEntry(BtlIndexList *list, s32 index);

void btlCopyIndexList(BtlIndexList *destination, BtlIndexList *source);

/* Swap two entries without changing the live count. */
void btlSwapIndexListEntries(BtlIndexList *list, s32 firstIndex, s32 secondIndex);

BtlIndexListEntryIndex btlFindListIndex(BtlIndexList *list, BtlUnit *entry);

extern void mdlSetEffectScale(dds3ModelHandle_t *, f32);

void btlApplyUnitEffectScale(BtlUnit *unit);

#include "dds/btl_camera_mode_definition.h"

extern BtlCameraModeDefinition btlCameraModeDefinitions[];

extern void mdlPropagateObjectFlag(void);

extern void mdlSetListedObjectFlag(void);

void btlSetCameraCommandMode(BtlCameraModeIndex cameraMode, BtlTask *link, BtlUnit *valueA,
                             BtlUnit *valueB, BtlActionCode actionCode);

void btlNormalizeActionCameraKeyScales(BtlLinkedCommand *action);

void btlInterpolateVectorStep(BtlCamState *camera);

/* Complete native camera flag-0x08 mode without changing command state. */
BtlCameraPoseStepResult btlCompleteCameraFlag08Mode(BtlLinkedCommand *command);

/* Complete native camera flag-0x20 mode without changing command state. */
BtlCameraPoseStepResult btlCompleteCameraFlag20Mode(BtlLinkedCommand *command);

/* Complete native camera flag-0x80 mode without changing command state. */
BtlCameraPoseStepResult btlCompleteCameraFlag80Mode(BtlLinkedCommand *command);

/* Blend position, direction, distance and FOV; both vector W lanes become zero. */
void btlBlendActionCameraState(BtlCamState *dst, BtlCamState *current, BtlCamState *target, f32 blend);

extern void btlBlendActionCameraState(BtlCamState *, BtlCamState *, BtlCamState *, f32);

BtlCameraPoseStepResult btlStepPoseBlendHalf(BtlLinkedCommand *command);

extern void btlBlendActionCameraState(BtlCamState *, BtlCamState *, BtlCamState *, f32);

BtlCameraPoseStepResult btlStepPoseBlend(BtlLinkedCommand *command);

BtlCameraPoseStepResult btlStepPoseBlendFrame(BtlLinkedCommand *command);

BtlCameraPoseStepResult btlStepPoseBlendRatio(BtlLinkedCommand *command);

s32 btlAdjustCameraDirectionForDefaultPlane(BtlCamState *state);

/* Adjust the pose direction using the supplied horizontal height plane. */
s32 btlAdjustCameraDirectionForPlane(BtlCamState *state, f32 height);

u32 btlExecuteCommandSoundTask(BtlCameraModeTaskArgs *arguments);

BtlRuntimeTask *btlCreateCommandSoundTask(BtlTask *actor, BtlCameraModeIndex cameraMode);

BtlRuntimeTask *btlCreateTargetedCommandSoundTask(BtlTask *actor, BtlCameraModeIndex cameraMode, BtlActionCode actionCode);

BtlRuntimeTask *btlCreateCommandSoundWithArguments(BtlTask *actor, BtlUnit *unitA, BtlUnit *unitB, BtlCameraModeIndex cameraMode, BtlActionCode actionCode);

u32 btlInitializeMotionTransformFromTaskArguments(BtlCameraTransformTaskArgs *arguments);

BtlRuntimeTask *btlCreateFloatTask28(BtlTask *actor, f32 x, f32 y, f32 z,
                                     f32 directionX, f32 directionY, f32 directionZ,
                                     f32 directionW, f32 fovDegrees);

extern void btlSetEffectCameraKeys(u8 *, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

s32 btlApplyEffectCameraKeyframes(BtlCameraKeyframesTaskArgs *args);

BtlRuntimeTask *btlCreateFloatTask29(BtlTask *actor, f32 a1, f32 a2, f32 a3, f32 a4, f32 a5, f32 a6, f32 a7, f32 a8, f32 a9, f32 a10, f32 a11, f32 a12, f32 a13, f32 a14, f32 a15, f32 a16);

u32 btlRunCameraMotionResetTask(void);

BtlRuntimeTask *btlScheduleContextReset(void);

extern void dds3CameraObjectBasicSetFovy(dds3Basic_t *, f32);

extern f32 btlCameraLocalUpVector[4], btlCameraOutputRotation[4];

/* Per-frame battle camera update: run the active camera mode's update hook,
 * advance the command's pose blend, then push the pose into the world camera. */
void btlUpdateBattleCameraFrame(void);

extern f32 btlWorldCameraStartOffset[4], btlWorldCameraStartRotation[4];

extern f32 btlCreatedCameraTarget[4], btlCreatedCameraRotation[4];

extern u32 D_003BB660;

extern dds3Basic_t *dds3AddCameraObjectBasicTypeFree(s32, u128 *, u128 *);

extern void dds3GetRadiusPoseSet(dds3Basic_t *, f32);

extern void dds3ObjectBaseCreateMover(dds3Basic_t *);

/* vu0 routine: add the battle origin to the default camera position. */
void btlInitializeWorldCamera(void);

void btlClearPendingSoundList(void);

void btlCopyMotionTransform(BtlCamState *dst, const BtlCamState *src);

void btlSetMotionTransformFieldOfView(BtlCamState *object, f32 fovRadians);

extern void btlClearRuntimeFlag2000(void);

extern u8 D_0037E110[];

void btlInitMotionTransformFromVectors(BtlCamState *object, const f32 *origin, const f32 *direction);

void btlInitMotionTransformFromComponents(BtlCamState *object, f32 x, f32 y, f32 z, f32 vx, f32 vy,
                    f32 vz, f32 vw, f32 fovDegrees);

void btlSetEffectCameraKeys(u8 *fx, f32 x0, f32 y0, f32 z0, f32 vx0, f32 vy0, f32 vz0, f32 vw0,
                            f32 x1, f32 y1, f32 z1, f32 vx1, f32 vy1, f32 vz1, f32 vw1,
                            f32 scale, f32 f154);

BtlCameraModeIndex btlGetCameraCommandMode(void);

f32 btlGetPoseBlendProgress(BtlLinkedCommand *command);

s32 btlIsUnitInActiveList(BtlUnit *unit);

void btlResetActiveUnitList(void);

void btlClearRuntimeFlag2000(void);

void btlSetRuntimeFlag2000(void);

u32 btlIsRuntimeFlag2000Clear(void);

extern f32 dds3CameraObjectBasicGetFovy(const dds3Basic_t *);

extern void dds3SetScreenFovy(f32);

void btlRefreshWorldCameraHandle(void);

extern s32 D_003BB668;

extern s32 D_003BB664;

s32 btlGetWorldObjectDefault(void);

s32 btlIsWorldMotionIdle(void);

s32 btlGetCameraVectorWork(void);

extern void btlFlagAllUnitsDefeatCandidate(void);

void btlFlagAllUnitDefeatCandidatesTask(void);

extern void btlClearAllUnitDefeatCandidates(void);

void btlClearAllUnitDefeatCandidatesTask(void);

extern void btlFlagMatchingUnitsDefeatCandidate(s32 mask);

void btlFlagLinkedGroupDefeatCandidatesTask(BtlLinkedCommand *command);

void btlApplyCombinedActorFlags(BtlLinkedCommand *command);

extern f32 btlCameraMotionResetFrameScales[];

extern char D_003A3DD0[];

void btlResetCameraMotion(BtlLinkedCommand *action);

extern char D_003A3DF0[];

extern char D_003A3E08[];

extern void btlBossDebugPrintfN(s32, s32, s32, s32, ...);

void btlDebugPrintWorldTransform(s32 x, s32 y);

extern f32 btlMeasureIndexListBounds(BtlIndexList *, f32 *, f32 *);

void btlFaceActionParticipantsTowardLinkedTarget(BtlLinkedCommand *action);

void btlAimLinkedUnitAtMuzzle(BtlLinkedCommand *action);
/* End of declarations from src/dds1/game/code_001D0BA0.c */

u32 btlRunCameraMotionResetTask(void) {
    BtlState *work = (BtlState *)btlGetRuntime();

    btlResetCameraMotion(&work->cameraCommand);
    return 1;
}
