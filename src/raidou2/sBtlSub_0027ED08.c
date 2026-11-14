#include "common.h"

/* Declarations from dds-decomp src/dds1/game/code_001F6110.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "kernel_draw.h"

#include "scr.h"

#include "sdfDevCons.h"

#include "sdf_resource.h"

#include "sdk_libc.h"

#include "dds/bill_object_api.h"

#include "dds/dds3_task_add_child.h"

#include "dds/dds3_world_primary.h"

#include "dds/dds3obj.h"

#include "dds/evt_unit.h"

#include "dds/kwln.h"

#include "dds/scr.h"

#include "dds/scr_arg_read.h"

#include "dds/scr_current_process_user_data_api.h"

#include "dds/scr_integer_parameter_api.h"

#include "dds/scr_integer_return_value_api.h"

#include "dds/sdf.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_column_packet_api.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_packet_alloc.h"

#include "dds/sdf_packet_builders.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_texture_draw_packet.h"

#include "dds/sdf_texture_file.h"

#include "dds/sdf_trig.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/sdf_vu_transform_api.h"

#include "dds/sdk_libc.h"

#include "dds/sdk_libkernl.h"

#include "dds/sdk_libm.h"

/* Convert three Euler angle components to the VU0 quaternion output.
 * Providers: DDS1 game/code_002E7C20.c and DDS2 game/code_00340AC8.c. */
void FromEuler(f32 x, f32 y, f32 z);

/* Load matrix into the VU0 scratch bank and postmultiply the primary bank. */
void MultMatrixRev(const struct Matrix4 *matrix);

/* Rigid-invert the VU matrix in vf28-vf31 in place; translation becomes
 * -(R^T * t). The rotation input is assumed to contain no scale. */
void ReverseMatrix(void);

/* Convert the quaternion in vf10 into the primary matrix bank vf28-vf31.
 * Providers: DDS1 game/code_002E7C20.c and DDS2 game/code_00340AC8.c. */
void ToMatrix(void);

f32 atan2f(f32 y, f32 x);

struct sdfTexHandle_t *dds3BillboardGetGeneralTex(s32 index);

dds3Basic_t *dds3GetActiveCameraSceneBasic(dds3Basic_t *world);

/* Return the borrowed primary world node, or NULL when no world is active. */
struct dds3Basic_t *dds3GetActiveScene(void);

void dds3GetPosPoseSet(dds3Basic_t *object);

u32 dds3GetProcessData(const dds3ProcessID_t *task);

/*
 * Append child to parent's child list, detaching it from any previous parent.
 * This hierarchy controls teardown: parent finalization calls
 * dds3KillProcess(child, 0) for linked children before its destroy callback.
 * It does not change scheduler queue membership.
 */
void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *child);

void evtSetUnitLightDir(evtUnitHandle_t *unit, EvtUnitDirectionTransitionFrames durationFrames);

void evtUnitLightOn(evtUnitHandle_t *unit);

s32 sceDclose(s32 directory);

s32 sceDopen(const char *path);

s32 sceDread(s32 directory, void *entry);

/* Returns the opaque caller-owned context attached to the ambient current
 * script process. This is a borrowed pointer; the caller retains ownership. */
void *scrGetData(void);

/* Read the current script argument as a float, converting supported numeric tags. */
f32 scrGetFloatPara(ScrCommandParameterIndex parameterIndex);

/* Read a command parameter as a signed VM integer, converting floats and
 * dereferencing global integer or float references. */
ScrVmInteger scrGetIntPara(ScrCommandParameterIndex parameterIndex);

/* Declared void for the callers that ignore its result: ee-gcc 2.96
 * allocates registers around the call as retail did only then. The
 * definition (scrTraceCode.c) returns the context it set the value in, for
 * the script commands that pass it on as their result; their units declare
 * it so. */
void scrSetIntReturnValue(ScrVmInteger retVal);

void scrSetUserData(dds3ProcessID_t *id, void *p);

/* Allocate a variable-length sprite column packet with this draw header. */
SdfDrawPacket *sdf2DAllocSprite(s32 loopCount);

SdfTextureDrawPacket *sdf2DLinkTexPacket(sdfDrawTag_t *packetList, sdfTexHandle_t *texture, s32 context);

void *sdfChipAllocDirect(s32 size);

void sdfChipFreeDirect(void *memory);

f32 sdfCos(f32 angle);

void sdfDrawTagInit(struct sdfDrawTag_t *list);

void sdfDrawTagLink(struct sdfDrawTag_t *list, u32 packetAddress);

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 size);

void sdfMemFreeDirect(struct sdfMemHandle_t *allocation);

void sdfPrimLinkGt4(
    sdfDrawTag_t *list, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0,
    s32 color0, s32 x1, s32 y1, s32 u1, s32 v1, s32 color1, s32 x2, s32 y2,
    s32 u2, s32 v2, s32 color2, s32 x3, s32 y3, s32 u3, s32 v3, s32 color3,
    s32 depth, s32 (*allocatePacket)(s32));

f32 sdfSin(f32 angle);

void sdfTexFreeDelayed(sdfTexHandle_t *texture);

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */
/* For a new texture, copy payload bytes into queue-owned storage before return. */
struct sdfTexHandle_t *sdfTexSet(const SdfTextureFileHeader *header);

char *strcpy(char *dst, const char *src);

u32 strlen(const char *text);

#include "dds/sdk_ps2.h"

#include "dds/btl_command.h"

#include "dds/btl_ui.h"

#include "dds/btl_motion_transform.h"

#include "dds/dds3_world_primary.h"

#include "dds/scr_integer_return_value_api.h"

#include "dds/btl_effect_position.h"

#include "dds/btl_unit_geometry_api.h"

#include "dds/dds3_effect_sprite.h"

#include "dds/common.h"

#include "dds/scr_integer_parameter_api.h"

#include "dds/btl_horizontal_aim_api.h"

#include "dds/btl_camera_mode_index.h"

#include "dds/sdf_vu0.h"

#include "dds/btl_aim_horizontal_direction_clamped_api.h"

#include "dds/btl_actor_mode_acceptance.h"

#include "dds/btl_boss_debug_api.h"

#include "dds/btl_vec3.h"

#include "dds/btl_projected_screen_point.h"

#include "dds/btl_skill_max_api.h"

#include "dds/btl_blend_color_api.h"

#include "dds/btl_effect_active_api.h"

#include "dds/dds3_task_add_child.h"

#include "dds/btl_command_affinity_api.h"

#include "dds/btl_packed_action_api.h"

#include "dds/sdf_trig.h"

#include "dds/dds3obj.h"

#include "dds/evt_unit.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_packet_builders.h"

#include "dds/sdf_column_packet_api.h"

#include "dds/btl_scene_fade.h"

#include "dds/btl_resource_browser.h"

#include "dds/eff_ref_obj.h"

#include "dds/sdf_heap_api.h"

#include "dds/bill_object_api.h"

#include "dds/btl_state.h"

#include "dds/ee_mmi.h"

#include "dds/pcp_vu0.h"

#include "dds/sdf_vu_transform_api.h"

#include "dds/kwln.h"

#include "dds/scr.h"

#include "dds/scr_current_process_user_data_api.h"

#include "dds/scr.h"

#include "dds/scr_arg_read.h"

#include "dds/sdf.h"

#include "dds/btl_resource_name.h"

#include "dds/dat_command.h"

#include "dds/sce_io.h"

#include "dds/fpu.h"

#include "dds/sdf_texture_file.h"

#include "dds/sdf_vu_matrix.h"

enum {
    BTL_RESOURCE_DESCRIPTOR_BYTES = 0x48,
    BTL_RESOURCE_NAME_RECORD_BYTES = 0x38,
    BTL_PACKED_SCREEN_X_BIAS = 0x7000,
    BTL_PACKED_SCREEN_Y_BIAS = 0x7900,
    BTL_HP_PERCENT_SCALE = 100,
    BTL_DIRECTORY_PATH_BYTES = 0x70,
    BTL_BUILTIN_NAME_COUNT = 8,
    BTL_DIRECTORY_FLAG_CLEAR = 0x1000,
    BTL_RESOURCE_ENTRY_BYTES = sizeof(BtlResourceEntry),
    /* High-bit selectors are game-specific callback-table positions. */
    BTL_PACKED_ENEMY_COUNT_LIMIT = 0x1400000,
    BTL_PACKED_ENEMY_QUEUED_QUERY = 0x0F800000,
    BTL_PACKED_PARTY_QUEUED_QUERY = 0x0FC00000,
    BTL_PACKED_PARTY_ELEMENT_BLOCK = 0x0F400000,
    BTL_PACKED_PARTY_EMPTY_MP = 0x10000000
};

extern void btlClearUnitDefeatCandidate(BtlUnit *unit);

extern f32 btlMeasureIndexListBounds(BtlIndexList *list, f32 *maxTop, f32 *minTop);

extern void btlFlagUnitDefeatCandidate(BtlUnit *unit);

extern s32 D_00360348[];

extern s32 D_0035FFE0[];

extern s32 D_003BB6B8;

extern s32 D_003BD854;

extern void btlPickCommandElementCheckedTarget(BtlTask *context, s32 action);

extern void btlPickCommandTargetExcludingActor(BtlTask *context, s32);

extern void btlPickCommandLowestHpBlockTarget(BtlTask *context, s32);

extern void btlSelectCommandTargetsByMode(BtlTask *context, s32);

extern void btlPickCommandTargetWithoutStatusMask(BtlTask *context, s32);

extern void btlPickCommandElementBlockTarget(BtlTask *context, s32);

extern void btlRunWeightedAiAction(BtlTask *, s32);

extern s8 btlHistoryCounter;

extern s8 D_003A5440[];

extern s8 D_003A5460[];

extern s8 D_003A5488[];

extern u8 D_r1_0046C168[];

extern u8 sdfHddValid;

extern s32 btlBuiltinResourceNameIndex;

extern s8 D_003A54A8[];

extern s8 D_003A54C8[];

extern s8 D_003A54F0[];

extern s8 D_003A5518[];

extern s8 D_003A5538[];

extern s8 D_003A5558[];

extern u32 D_003BB6C8;

extern u8 D_r1_0046C170[];

struct BtlRuntimeTask;

extern struct BtlRuntimeTask *btlCreateCommandSoundUpdateTask(void);

typedef struct BtlActor {
    u8 pad00[0x18];
    BtlUnit *unit;
} BtlActor;

typedef struct BtlList {
    u8 pad00[0x20];
    s32 count;
} BtlList;

typedef struct BtlCmdCtx {
    u8 pad00[0xC];
    u32 flags; /* 0x0C: command-context flags */
    u8 pad10[8];
    BtlUnit *unit;
    u8 pad1C[4];
    s32 commandMode;
    s32 commandValue;
    u8 pad28[4];
    BtlUnit *actionProbeFirst;
    BtlUnit *actionProbeSecond;
    u8 pad34[4];
    u32 parameter38; /* 0x38 */
    u8 pad3C[0x50];
    u32 parameter8C; /* 0x8C */
} BtlCmdCtx;

extern f32 btlUnitGetTopY(BtlUnit *);

extern s32 btlReadCurrentUnitHp(dds_datUnitWork_t *);

extern void btlGetUnitWorldPos(BtlUnit *, f32 *);

extern f32 sBtlGetVerticalLength(const f32 *, const f32 *, const f32 *);

typedef union BtlVec4 {
    f32 f[4];
    u128 q;
} BtlVec4;

extern struct BtlRuntimeTask *btlCreateSecondaryCommandSoundTask(void);

struct BtlRuntimeTask;

extern struct BtlRuntimeTask *btlCreateCommandSoundTask(BtlTask *, BtlCameraModeIndex cameraMode);

extern BattleTrackedTaskWork *btlTrackedTaskHandles;

extern u32 btlGetEventEffectValue(void);

extern u32 btlGetEffectValue(void);

extern u32 btlGetSelectedBossEffectId(void);

extern u32 btlGetSpecialModeEffectValue(void);

extern u32 btlButtonIconTexture;

extern void btlPickCommandTargetWithStatusMask(BtlTask *context, s32);

extern struct BtlRuntimeTask *btlAllocTask(s32);

extern void btlCommandRecenterParty(void);

typedef struct BtlControlObject {
    u8 enabled; /* 0x00 */
    u8 pad01[0x0F];
    u8 state; /* 0x10 */
    u8 pad11[0x0F];
    u16 type; /* 0x20 */
    u8 pad22[0x26];
    s32 status; /* 0x48 */
    void (*update)(void); /* 0x4C */
} BtlControlObject;

struct BtlRuntimeTask *btlCreateControlObject(void);

extern f32 aViewEye[];

extern f32 sdfViewMatrix[];

extern f32 sdfProjectionMatrix[];

extern f32 D_r1_003AD4E0[];

/* vu0 routine: forward-cone projection from vf10 to integer screen XY */
BtlProjectedScreenPointResult btlProjectForwardPositionToScreen(BtlProjectedScreenPoint *outPoint);

/* vu0 routine: project the vf10 position into four screen-coordinate words with four fractional bits.
   Return zero outside the distance/forward-cone checks, without writing the output. */
s32 nbClipEffectNum(s32 screenPosition[4]);

void effBTLModelGetSphereCenter(BtlUnit *unit);

void btlUnitGetBodyPosVU(const BtlUnit *unit);

void btlUnitGetEffectPosVU(BtlUnit *unit);

/* Return the larger unscaled extent multiplied by the unit scale. */
f32 btlUnitGetLargestScaledExtent(BtlUnit *unit);

/* Upper Y bound: scaled half-height minus the sampled position's Y. */
f32 btlUnitGetTopY(BtlUnit *unit);

/* Lower Y bound uses the same negated position Y and scaled half-height. */
f32 btlUnitGetBottomY(BtlUnit *unit);

f32 btlMeasureMaskedUnitBounds(s32 mask, f32 *outMaxTop, f32 *outMinTop);

f32 btlMeasureEffectTargetGroupBounds(s32 mask, f32 *outMaxTop, f32 *outMinTop);

/* Maximum top bound among active units matching any mask bit; zero if none. */
f32 btlGetMaxUnitTop(u32 mask);

/* Maximum scaled reach among active units matching any mask bit; zero if none. */
f32 btlGetMaxUnitReach(u32 mask);

/* Despite the legacy Y name, this selects a Z edge from position[2].
 * Mask bit 0x200 selects the maximum plus reach; otherwise use the minimum
 * minus reach. No matching active unit leaves the zero result. */
f32 btlGetExtremeUnitY(u32 mask);

f32 btlMeasureIndexListBounds(BtlIndexList *list, f32 *maxTop, f32 *minTop);

/* Compare VU-computed distances from the target's muzzle to eligible units. */
BtlUnit *btlFindNearestUnit(u32 mask, BtlUnit *target);

/* Return the eligible unit farthest from point, using the VU distance.
 * The zero baseline leaves NULL when no candidate has a positive distance. */
BtlUnit *btlFindFarthestUnit(u32 mask, f32 *point);

/* Select the smallest X for reference flag 0x200, otherwise the largest.
 * An empty indexed list returns NULL but still loads the native scratch vector. */
/* vu0 routine: returns the selected muzzle position in vf10. */
BtlUnit *btlSelectUnitAtExtremeX(BtlUnit *reference, BtlIndexList *list);

void btlFlagAllUnitsDefeatCandidate(void);

void btlClearAllUnitDefeatCandidates(void);

void btlFlagMatchingUnitsDefeatCandidate(s32 mask);

void btlClearMatchingUnitDefeatCandidates(s32 mask);

/* Mark every unit in the indexed list as a defeat candidate. */
void btlFlagActorUnitsDefeatCandidate(BtlIndexList *list);

/* Clear the defeat-candidate state of every unit in the indexed list. */
void btlClearActorUnitDefeatCandidates(BtlIndexList *list);

extern void btlSetUnitPosition(BtlUnit *object, const f32 *position);

extern void btlSetUnitRotation(BtlUnit *object, const f32 rotation[4]);

extern void btlRefreshUnitMotionSelection(BtlUnit *unit);

extern void btlApplyUnitMotionSelection(BtlUnit *, u32, s32, f32);

/* Refresh each unit's transform/effect state, then invoke the runtime callback. */
void btlUpdateUnitActors(void);

void btlRefreshUnitEffects(void);

/* Count matching units with bit 0 set, excluding every unit with bit 0x20. */
s32 btlCountActiveUnitsWithFlags(s32 mask);

extern u128 btlHorizontalAimIdentityQuaternion;

/* Aim along the horizontal origin-to-target delta; zero delta loads the default vf10. */
BtlHorizontalAimResult btlAimHorizontalDirectionVU(const f32 *origin, const f32 *targetPosition);

s32 btlAimHorizontalDirectionClampedVU(const f32 *origin, const f32 *targetPosition, f32 angle);

/* vu0 routine: vf10.xyz = normalize(e x (e x f)),
 * e = vertexB - vertexA, f = vertexC - vertexA.
 * The double cross lies in the triangle plane, despite the legacy normal name. */
void sBtlGetVerticalVector(const f32 *vertexA, const f32 *vertexB, const f32 *vertexC);

/* Dot the normalized double-cross direction with vertexA - vertexC. */
f32 sBtlGetVerticalLength(const f32 *vertexA, const f32 *vertexB, const f32 *vertexC);

/* vu0 routine: vf10 = vertexC + normalize(point - vertexC) * projection,
 * where projection is the double-cross direction dotted with vertexA - vertexC. */
void btlPointOffPlaneVU(f32 *vertexA, f32 *vertexB, f32 *vertexC, f32 *point);

/* vu0 routine: vf10 = vertexC + direction * dot(direction, vertexA - vertexC).
 * Direction is the normalized in-plane double cross, not a triangle normal. */
f32 btlProjectOnPlaneVU(const f32 *vertexA, const f32 *vertexB, const f32 *vertexC);

/* Inclusive box test; either corner order is accepted independently per axis. */
s32 btlPointInBox(BtlVec3 *cornerA, BtlVec3 *cornerB, BtlVec3 *point);

/* vu0 routine: interpolate packed RGBA bytes using a 1/128 channel scale.
 * Factors at or above one return colorB directly; negative factors are not clamped. */
u32 btlBlendColor(u32 colorA, u32 colorB, f32 blendFactor);

/* vu0 routine: interpolate RGBA float vectors, multiply channels by 128,
 * then truncate and pack. Unlike the packed-color routine, neither endpoint
 * is returned early and the blend factor is not clamped. */
u32 btlBlendColorVec(f32 *colorA, f32 *colorB, f32 blendFactor);

/* Store the exponential span and clear its progress. */
void btlScalarRangeSetStartClearEnd(BtlExponentialRange *state, f32 start);

/* Advance progress by (1 - progress) / span and return at most one.
   Only the return is capped: stored progress is raw. A nonpositive span is unchanged. */
f32 btlScalarRangeStepExponential(BtlExponentialRange *state);

/* Initialize the quadratic accumulator; a zero span leaves inverseSpan unchanged. */
void btlScalarRangeInitQuadratic(BtlScalarRange *state, f32 start);

/* Integrate the quadratic accumulator, reversing acceleration after the midpoint.
   Completion returns one without updating state; intermediate values are not capped. */
f32 btlScalarRangeStepQuadratic(BtlScalarRange *state, f32 timeStep);

extern s32 btlIndexedIconUvOrigins[16];

struct RefObj;

extern sdfTexHandle_t *sdfGetOrCreateTextureForReference(struct sdfGrOt_t *, struct RefObj *);

#include "dds/sdf_packet_alloc.h"

/* Draw an indexed 64-pixel glyph with independent corner colors. */
void btlDrawButtonIconFixed64(sdfGrOt_t *surface, s32 x, s32 y,
                   s32 topLeftColor, s32 topRightColor,
                   s32 bottomLeftColor, s32 bottomRightColor,
                   s32 buttonIndex);

typedef struct BtnUv {
    s32 u;
    s32 v;
} BtnUv;

extern BtnUv D_0035FA30[];

extern BtnUv D_0035FA38[];

extern BtnUv D_0035FA40[];

extern BtnUv D_0035FA48[];

extern BtnUv D_0035FA50[];

extern BtnUv D_0035FA58[];

extern BtnUv D_0035FA60[];

extern BtnUv D_0035FA68[];

extern BtnUv D_0035FA70[];

extern BtnUv D_0035FA78[];

extern BtnUv D_0035FA80[];

extern BtnUv D_0035FA88[];

extern BtnUv D_0035FA90[];

extern BtnUv D_0035FA98[];

extern BtnUv D_0035FAA0[];

/* Draw a button glyph using its UV pair; screen coordinates are GS fixed-point. */
void btlDrawButtonIcon(sdfGrOt_t *surface, s32 x, s32 y, s32 topLeftColor, s32 topRightColor, s32 bottomLeftColor, s32 bottomRightColor, s32 button);

void btlOpenButtonIconResource(void);

/* Cache and later release the loaded battle resource in battle state. */
void btlRetainButtonTexture(void);

void btlReleaseButtonTexture(void);

/* Script command handlers set the command state at +0x20 and its
 * argument at +0x24; their numeric opcodes are still unidentified. */
u32 btlScriptSelectEmptyCommand(void);

/* Select the fixed mode-6/value-0xC2 command and continue the script VM. */
u32 btlScriptSelectFixedCommand(void);

u32 btlScriptSelectModeDValueOne(void);

u32 btlScriptSelectModeAValueOne(void);

/* Resolve a command-table entry through its probe, or store the direct command id. */
u32 btlScriptSelectActionEntry(void);

u32 btlScriptSetCommandValueAndParameter(void);

u32 btlCmdSetContextFlagOne(void);

u32 btlScriptSetActorUnitParameter(void);

u32 btlScriptSetBattleWorkParameter(void);

extern void btlChooseCommandActionTargets(BtlTask *context);

/* Select targets for the current command's action; return VM success. */
u32 btlScriptChooseActionTargets(void);

/* Parameter 0 supplies the group element-check input; return VM success. */
u32 btlScriptPickElementCheckedTarget(void);

extern void btlPickCommandLowestHpTarget(BtlTask *context);

/* Select the current command's minimum nonzero-HP candidate; return VM success. */
u32 btlScriptPickLowestHpTarget(void);

extern void btlPickCommandLowestHpRatioTarget(BtlTask *context);

/* Select the current command's minimum HP-percentage candidate; return VM success. */
u32 btlScriptPickLowestHpRatioTarget(void);

/* Parameter 0 feeds lowest-HP element-block selection; return VM success. */
u32 btlScriptPickLowestHpBlockTarget(void);

extern void btlPickCommandHighestMpTarget(BtlTask *context);

/* Pick the current command's highest-MP target and return VM completion. */
u32 btlScriptPickHighestMpTarget(void);

extern void btlPickCommandLowestLevelTarget(BtlTask *context);

/* Run the current command's minimum-value target selector; return VM success. */
u32 btlScriptPickLowestLevelTarget(void);

/* Parameter 0 is a status-presence mask for command target selection; return VM success. */
u32 btlScriptPickTargetWithStatusMask(void);

/* Parameter 0 is a status-absence mask for command target selection; return VM success. */
u32 btlScriptPickTargetWithoutStatusMask(void);

/* Apply script parameter 0 as the command target-selection mode; return VM success. */
u32 btlScriptCmdSelectTargetsByMode(void);

/* Separate builtin entry with the same action-target selection body; return VM success. */
u32 btlScriptChooseActionTargetsDuplicate(void);

extern void btlPickUnlinkedCommandTarget(BtlTask *context, u32 input);

/* Run the current script command's unlinked-party target selection; return script success. */
u32 btlScriptPickUnlinkedPartyTarget(void);

extern void btlPickLinkedCommandTarget(BtlTask *context, u32 input);

/* Run the current script command's linked-party target selection; return script success. */
u32 btlScriptPickLinkedPartyTarget(void);

extern void btlAppendActorToCommandTargets(BtlTask *context, s32 input);

/* Append this command's own actor after target scan; return VM success. */
u32 btlScriptAppendActorToTargets(void);

extern void btlAddEffectActorTargetToCommand(BtlTask *context);

/* Append the global effect actor to this command's targets; return VM success. */
u32 btlScriptAddEffectActorTarget(void);

/* Parameter 0 feeds element-block target selection; return VM success. */
u32 btlScriptPickElementBlockTarget(void);

/* Prefer a non-actor target, retaining native self fallback; return VM success. */
u32 btlScriptPickTargetExcludingActor(void);

/* Script command selection: low/high flag words gate the stored choices. */
typedef struct BtlCommandContext {
    u8 pad00[0x18];
    BtlUnit *actor; /* 0x18: the command actor's unit. */
    u8 pad1C[0x74];
    u32 selectionFlagsA; /* 0x90 */
    u32 selectionFlagsB; /* 0x94 */
    s32 choicesA[26]; /* 0x98..0xFC */
    s32 choicesB[17]; /* 0x100..0x140 */
} BtlCommandContext;

u32 btlNbScriptCheckActorFlag(void);

/* Remember the choice when the packed MP-percentage condition succeeds. */
u32 btlCmdStoreActorMpCheckChoice(void);

u32 btlCmdStoreBossHealthCheckChoice(void);

/* Remember the count limit only when the eligible enemy count is at most that limit.
   Failure clears the selection flag but leaves the previous stored choice intact. */
u32 btlCmdStoreEnemyCountCheckChoice(void);

u32 btlCmdStorePartyCountCheckChoice(void);

u32 btlCmdStoreActorActionMaskChoice(void);

u32 btlCmdStoreEnemyActionMaskChoice(void);

u32 btlCmdStorePartyActionMaskChoice(void);

u32 btlCmdRememberAllLowerActionBits(void);

u32 btlCmdRememberLowerUnitMode(void);

u32 btlCmdRememberOtherUpperUnitMode(void);

u32 btlCmdRememberLowerEntryCondition(void);

u32 btlCmdRememberUpperEntryCondition(void);

/* Remember the actor entry-code-10 gate result; failure clears only its selection flag. */
u32 btlCmdRememberActorActionGate(void);

u32 btlCmdReturnReadyWithoutTurnsCheck(void);

u32 btlCmdRememberCounterLimit(void);

u32 btlCmdStoreCounterReachedLimitChoice(void);

u32 btlCmdReturnLowHpActionReadiness(void);

u32 btlCmdReturnActorEligibility(void);

/* Report whether active-enemy and cumulative-spawn limits permit another enemy. */
u32 btlCmdReportEnemySpawnCapacity(void);

u32 btlCmdRememberLowerElementAccess(void);

u32 btlCmdRememberUpperElementAccess(void);

u32 btlCmdReturnPartyElementQueryCheck(void);

u32 btlCmdReturnEnemyElementQueryCheck(void);

u32 btlCmdRememberLowerElementExclusion(void);

u32 btlCmdRememberUpperElementExclusion(void);

u32 btlCmdRememberUpperActionAvailability(void);

/* Remember the actor entry-code condition choice only when its query succeeds. */
u32 btlCmdRememberActorEntryCondition(void);

u32 btlCmdStoreDefaultUnitActionChoice(void);

u32 btlCmdStoreAlternateUnitActionChoice(void);

u32 btlCmdStorePartyEntryCheckChoice(void);

u32 btlCmdStoreInversePartyEntryChoice(void);

u32 btlCmdRememberUpperEntryCodeMatch(void);

u32 btlCmdRememberUpperEntryCodeInverse(void);

u32 btlCmdRememberLowerEntryCodeMismatch(void);

u32 btlCmdRememberUpperEntryCodeMismatch(void);

u32 btlCmdReturnPartyActionPresence(void);

u32 btlCmdReturnEnemyActionPresence(void);

/* Remember a choice matching an eligible enemy's selected-action element. */
u32 btlCmdRememberUpperSelectedElement(void);

/* Remember the choice when an eligible party unit's queued action matches its category query. */
u32 btlCmdRememberPartyQueuedActionCategory(void);

u32 btlCmdRememberLowerMissingStatus(void);

u32 btlCmdReturnPartyMissingFlagCheck(void);

u32 btlCmdReturnPartyHeldFlagCheck(void);

/* Remember actor record bit 0x2000 when status bit 0x200 is clear. */
u32 btlCmdRememberNonLowerStatFlag(void);

u32 btlCmdRememberContextSecondaryFlag(void);

extern s32 btlHasBossEffectOption(void);

u32 btlCommandReportBossEffectOption(void);

extern s32 btlHasEffectActor(void);

u32 btlCmdTestEffectActor(void);

extern s32 btlIsSpecialEnemyEffectLinkSatisfied(void);

u32 btlCommandReportSpecialEnemyLink(void);

/* Report an eligible matching unit at or below the scripted HP percentage.
   Preserve the native signed products and their unsigned comparison. */
u32 btlCmdCheckHpPercent(void);

extern s32 btlHasFirstSpecialEnemySpecies(void);

u32 btlScriptReturnFirstSpecialEnemySpecies(void);

/* Remember the choice when an enemy's queued query matches; failure clears only its flag. */
u32 btlCmdRememberUpperQueuedQuery(void);

/* Remember the choice when a party unit's queued query matches; failure clears only its flag. */
u32 btlCmdRememberLowerQueuedQuery(void);

/* Remember the choice on a successful party-element block check; failure clears only its flag. */
u32 btlCmdRememberLowerElementBlock(void);

/* Report whether an eligible party unit has zero current MP; the handler tests unit +0x12A. */
u32 btlCmdReportPartyEmptyMp(void);

u32 btlScriptReturnSceneSlotCount(void);

/* Publish the battle turn count to the script integer result; return VM success. */
u32 btlScriptReturnTurnCount(void);

/* Actor +0x134 is partyRecord.level, not an action timer; return VM success. */
u32 btlScriptReturnActorLevel(void);

u32 btlCommandReportSpecialModeValue(void);

u32 btlScriptReturnSelectedBossEffectId(void);

u32 btlScriptReturnEffectActive(void);

u32 btlCmdReturnBattlePhase(void);

u32 btlScriptReturnEffectValue(void);

u32 btlScriptReturnContextSignedByte(void);

u32 btlScriptReturnGlobalDebugValue(void);

u32 btlCommandReportEventEffectValue(void);

extern s32 btlRunRandomWeightedAiTableAction(BtlTask *task);

u32 btlCmdRunWeightedAiSelection(void);

u32 btlScriptSetBattleCommand(void);

u32 btlResetCommandContextAndSetStateFlag(void);

extern void btlBeginScenePanelFadeOut(void);

u32 btlScriptClearSceneTransition(void);

extern void btlBeginScenePanelFadeIn(void);

u32 btlScriptBeginSceneTransition(void);

u32 btlScriptFlagAllUnitsDefeatCandidates(void);

u32 btlScriptFlagPlayerDefeatCandidates(void);

u32 btlScriptFlagEnemyDefeatCandidates(void);

extern struct BtlRuntimeTask *btlCreateFloatTask28(BtlTask *actor, f32 x, f32 y, f32 z,
                                                   f32 directionX, f32 directionY, f32 directionZ,
                                                   f32 directionW, f32 fovDegrees);

extern struct BtlRuntimeTask *btlScheduleContextReset(void);

extern u64 btlStartTask(struct BtlRuntimeTask *taskObject);

u32 btlCmdCameraMove(void);

u32 btlScriptQueueActorCommandSound(void);

u32 btlScriptQueueUnboundCommandSound(void);

extern f32 D_003D74E0[];

extern f32 D_003D7500[];

s32 btlScriptSetCameraBlendStart(void);

s32 btlScriptSetCameraBlendEnd(void);

extern struct BtlRuntimeTask *btlCreateFloatTask29(BtlTask *, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

u32 btlCmdCameraMoveBlend(void);

u32 btlAiCheckBa(void);

u32 btlAiGetAllKoudo(void);

/* Complete zero-operand builtin 342; retain its unused command-work lookup. */
u32 btlScriptBuiltin342NoOp(void);

u32 btlAiGetFurea(void);

u32 btlCmdReportUnsupportedCameraOrigin(void);

u32 btlCmdReportUnsupportedCameraTwoShot(void);

u32 btlCmdReportUnsupportedCameraBougai(void);

u32 btlCmdReportUnsupportedEvent(void);

u32 btlCmdReportUnsupportedTaikyo(void);

u32 btlCmdReportUnsupportedAllTaikyo(void);

extern void btlClearSpecialEnemyEntryFlags(void);

u32 btlCmdClearSpecialEnemyFlags(void);

extern u8 *D_003BAAA8;

extern void itfMesSetTextSlotFromValue(s32, s32, s32, s32);

void btlBindActorSlot(BtlTask *task, s32 taskArg);

void func_001FADA8(void);

void func_001FADB0(void);

void func_001FADB8(void);

/* EFFECT EDIT menu callback requests the editor through the shared debug flags. */
u32 btlRequestEffectEditor(void);

/* Return the supplied value unchanged. */
u32 btlReturnInputValue(u32 value);

void func_001FADE0(void);

void func_001FADE8(void);

/* Empty callback registered by the POINT LIGHT debug menu. */
void btlPointLightMenuNoOp(void);

extern void evtUnitLightOff(s32 unitAddress, s32 duration);

void btlReleaseActiveUnitEffectsUnlessPaused(void);

extern s8 dds3PadBitTable[32];

extern f32 D_003BB768;

extern f32 D_003BB76C;

extern u8 D_003BB770;

extern f32 btlDebugCameraPositions[2][4];

extern f32 btlDebugCameraRotations[2][4];

/* Battle debug camera: one button saves the world camera's position and
 * rotation into one of two slots (alternating), the other toggles a camera
 * command built from both saved slots with a 40-degree field of view. The
 * debug-menu descriptors mirror the command's active bit. */
void btlUpdateDebugCameraFromPad(void);

/* Clear bit 0x80 in both battle debug-menu descriptors. */
void btlResetBattleDebugMenuFlags(void);

void func_001FB080(void);

/* Empty hook between active actor updates and the finished-task sweep. */
void btlUpdateActiveFrameNoOp(void);

/* Empty hook between event playback and battle-field presentation. */
void btlUpdateFieldPresentationNoOp(void);

/* Empty hook at the end of battle-runtime initialization. */
void btlInitializeRuntimeNoOp(void);

void func_001FB0A0(void);

/* Retail discards debug output but retains the format-string/varargs ABI. */
void btlBossDebugPrintf(const char *format, ...);

void btlBossDebugPrintfN(s32 x, s32 y, s32 color, s32 format, ...);

/* Retail discards positioned debug text; name-editor callers retain this ABI. */
void btlDebugDrawText(s32 x, s32 y, s32 style, const char *text);

/* Retail discards debug rectangles; name-editor callers pass position and size. */
void btlDebugDrawRectangle(s32 x, s32 y, s32 width, s32 height);

/* Retail discards menu-frame rectangles; callers supply geometry and two colors. */
void btlDebugDrawTwoColorRectangle(s32 x, s32 y, s32 width, s32 height, u32 color0, u32 color1);

/* Retail no-op for the colored span between adjacent mapping-plot knots. */
void btlDebugDrawMappingColorSpan(s32 x, s32 y, s32 width, s32 height, u32 color0, u32 color1);

/* Retail no-op: native plot guides supply two-dimensional endpoints and color. */
void btlDebugDrawLine2D(s32 x0, s32 y0, s32 x1, s32 y1, u32 color);

void func_001FB158(void);

/* Retail ignores the scalar-curve knot's pixel coordinates and packed color. */
void btlDebugDrawPoint2DNoOp(s32 x, s32 y, u32 color);

/* Retail stub for the endpoint marker submitted by camera diagnostics. */
void btlDebugDrawPoint(f32 *position);

void func_001FB170(void);

/* Retail stub for a centered radius guide; preserve the mixed-argument order. */
void btlDebugDrawRadiusGuide(f32 *position, f32 radius, u32 color, s32 flags);

/* Retail diagnostic stub: two extents, a start point, color and matrix. */
void btlDebugDrawOrientedExtentGuide(f32 *start, f32 firstExtent, f32 secondExtent, u32 color, f32 (*matrix)[4]);

/* Retail stub for the position, color, mode and scale submitted by the actor editor. */
void btlDebugDrawScaledPositionGuide(const f32 *position, u32 color, s32 mode, f32 scale);

void func_001FB190(void);

/* Retail stub for a colored segment between two diagnostic guide endpoints. */
void btlDebugDrawSegment(f32 *from, f32 *to, u32 color);

void func_001FB1A0(void);

/* Empty stage immediately before the per-context debug-menu entry. */
void btlDebugMenuBeforeRecordNoOp(void);

/* Empty stage immediately after the per-context debug-menu entry. */
void btlDebugMenuAfterRecordNoOp(void);

void func_001FB1B8(void);

void func_001FB1C0(void);

void func_001FB1C8(void);

void func_001FB1D0(void);

void func_001FB1D8(void);

/* DDS1's file-queue editor consumes v0 after this call (002B609C).
 * The DDS2 counterpart returns its argument; this retail debug stub's
 * value-returning body was compiled out, leaving jr ra; nop. */
u32 btlDebugMenuSetupNoOp(void *context);

void func_001FB1E8(void);

/* Empty per-context stage; its sequence position does not imply recording. */
void btlDebugMenuRecordNoOp(void *context);

/* Return the matching entry, or the one-past-end address when none matches. */
u8 *btlFindEntryByCommand(EffQueuedFileObject *list, s32 command);

/* Count entries other than the two part opcodes, 0x14 and 0x15. */
s32 btlCountNonPartOpcodes(EffQueuedFileObject *list);

s32 btlOpenPfsDebugDirectory(const char *directoryName);

/* Close the scanned directory only in debug filesystem mode; ignore the native result. */
void btlClosePfsDebugDirectory(s32 directoryHandle);

extern char *btlBuiltinResourceNames[];

/* Debug mode delegates to the native reader. Built-in mode returns each name's length,
   clears the directory-type bit, and returns zero when the eight-name table is exhausted. */
s32 btlReadBattleResourceDirectoryEntry(s32 directoryHandle, SceDirent *reader);

extern u32 dds3BillboardGeneralNumber(void);

/* Build a scan-order list of built-in names and enabled resource-file categories. */
BtlResourceEntryList *btlScanDirectory(const char *path, s32 flags);

/* Release all scanned entries, the owned path prefix, and the list header. */
void btlDestroyEntryList(BtlResourceEntryList *list);

/* Append a named browser entry without changing the current scan order. */
void btlAppendEntry(BtlResourceEntryList *list, const char *name, s32 category, s32 value, s32 id);

/* Initialize a browser descriptor at the first entry of the borrowed list.
 * The native constructor leaves textureCategory untouched. */
BtlResourceDescriptor *btlCreateResourceDescriptor(BtlResourceEntryList *list);

extern s8 dds3PadBitTable[0x20];

extern s8 D_r1_0043EED9[];

extern u64 *dds3PrimFrameRect(s32, s32, s32, s32, s32, u32, u32);

s32 btlFormatSelectedResourceName(BtlResourceDescriptor *, char *);

void btlLoadAndReplaceResourceHandle(BtlResourceDescriptor *, const char *);

void btlReplaceResourceHandle(BtlResourceDescriptor *, void *);

/* Update list selection and submit the visible browser rows to its surface. */
s32 Draw_effLensFlare(BtlResourceDescriptor *descriptor);

/* Release an owned texture handle and the descriptor, but not its entry list. */
void btlDestroyResourceDescriptor(BtlResourceDescriptor *descriptor);

void btlSetResourceBrowserPosition(BtlResourceDescriptor *descriptor, s32 x, s32 y);

/* Read the browser descriptor's zero-based selected entry index. */
u32 btlGetResourceBrowserSelectedIndex(
    const BtlResourceDescriptor *descriptor);

u32 btlGetResourceBrowserSelectionStatus(const BtlResourceDescriptor *descriptor);

/* Format prefix + selected name; return its resource category, not its id. */
s32 btlFormatSelectedResourceName(BtlResourceDescriptor *descriptor, char *output);

/* Strip from the first '.' onward; return the selected entry's category. */
s32 btlTrimResourceName(BtlResourceDescriptor *descriptor, char *output);

/* Return the selected entry's category-specific metadata word unchanged. */
u32 btlGetResourcePathVariant(const BtlResourceDescriptor *resource);

void btlReplaceResourceHandle(BtlResourceDescriptor *, void *);

/* Replace an owned old texture, acquire the named resource's texture,
 * then release the temporary allocation returned by the resource reader. */
void btlLoadAndReplaceResourceHandle(BtlResourceDescriptor *descriptor, const char *resourceName);

/* Acquire a texture from the supplied resource, releasing an owned old handle. */
void btlReplaceResourceHandle(BtlResourceDescriptor *descriptor, void *textureResource);

extern u8 *sdf2DGetPrimPtr(SdfDrawPacket *packet);

/* Queue the descriptor's preview texture as a quad at its screen position. */
void btlDrawResourcePreview(BtlResourceDescriptor *descriptor);

/* Allocate the native name record and initialize its header and extension. */
struct BtlResourceNameRecord *btlCreateResourceNameRecord(const char *extension);

/* Release the chip allocation used for an editable resource-name record. */
void btlDestroyResourceNameRecord(struct BtlResourceNameRecord *record);

extern u8 btlResourceNameCharacters[];

extern char D_003BB828[8];

extern char D_003BB830[];

extern char D_003BB838[];

#define EDITOR_KEY_COLS 13

#define EDITOR_KEY_ROWS 3

#define EDITOR_KEY_CELLS (EDITOR_KEY_COLS * EDITOR_KEY_ROWS)

#define EDITOR_CHARACTER_COUNT (EDITOR_KEY_CELLS - 1)

#define EDITOR_BLANK_CELL (EDITOR_KEY_CELLS - 1)

#define EDITOR_BACKSPACE_CELL EDITOR_KEY_CELLS

#define EDITOR_CONFIRM_CELL (EDITOR_KEY_CELLS + 1)

#define EDITOR_DELETE_CODE (EDITOR_KEY_CELLS + 3)

void btlUpdateResourceNameEditor(struct BtlResourceNameRecord *record);

void btlSetResourceNamePosition(struct BtlResourceNameRecord *record, s32 x, s32 y);

u32 btlGetResourceNameSelectionStatus(const struct BtlResourceNameRecord *record);

/* Copy the resource name at +0x21 and store its length. */
void btlResourceRecordSetName(struct BtlResourceNameRecord *record, const char *name);

void btlFormatResourceNameWithExtension(const struct BtlResourceNameRecord *record, char *output);

void btlFormatResourceNameWithoutExtension(const struct BtlResourceNameRecord *record, char *output);

void btlSetResourceNameLengthLimit(struct BtlResourceNameRecord *record, u32 maximumNameLength);
/* End of declarations from src/dds1/game/code_001F6110.c */

/* vu0 routine: forward-cone projection from vf10 to integer screen XY */
BtlProjectedScreenPointResult btlProjectForwardPositionToScreen(BtlProjectedScreenPoint *outPoint) {
    f32 distance;
    f32 facing;
    f32 *projection;
    BtlVec4 position;

    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(distance);
    if (!(distance > 16.0)) {
        return BTL_PROJECTED_SCREEN_POINT_REJECTED;
    }
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    VU0_LOAD_MATRIX(sdfViewMatrix);
    SDF_VU0_COPYMATRIX_A_TO_B();
    ReverseMatrix();
    SDF_VU0_GET_ROW2();
    SDF_VU0_NORMALIZE();
    SDF_VU0_INNERPRODUCT(facing);
    if (facing <= 0.5f) {
        return BTL_PROJECTED_SCREEN_POINT_REJECTED;
    }
    SDF_VU0_COPYVECTOR_C_TO_A();
    VU0_MOVE_VF(vf28, vf24);
    VU0_MOVE_VF(vf29, vf25);
    VU0_MOVE_VF(vf30, vf26);
    VU0_MOVE_VF(vf31, vf27);
    projection = sdfProjectionMatrix;
    MultMatrixRev((const struct Matrix4 *)projection);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    projection += 16;
    SDF_VU0_LOADVECTOR_B(projection);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(D_r1_003AD4E0);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(&position);
    outPoint->x = (s32)position.f[0] - 0x700;
    outPoint->y = (s32)position.f[1] * 2 - 0xF20;
    return BTL_PROJECTED_SCREEN_POINT_WRITTEN;
}

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_002AB7B8);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_r1_0027EEB0);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_r1_0027F060);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_r1_0027F0B0);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_r1_0027F0D0);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_r1_0027F108);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_r1_0027F140);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_r1_0027F158);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_r1_0027F188);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", sBtlGetBezier);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_r1_0027F218);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_r1_0027F230);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_r1_0027F2B0);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_r1_0027F2F8);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_002ABDE8);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_r1_0027F358);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_r1_0027F3D8);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_r1_0027F3F0);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", sdatDevilName);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_r1_0027F490);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_r1_0027F510);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_r1_0027F748);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_r1_0027F798);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_r1_0027F7B8);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_r1_0027F838);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_r1_0027F858);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_r1_0027F8D8);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_r1_0027F8F0);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_r1_0027F340);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_002AC438);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", sBtlGetBSTNameFont);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_r1_0027F988);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_r1_0027FA08);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_002AC610);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", sBtlCheckVerticalArea);

/* vu0 routine: vf10.xyz = normalize(e x (e x f)),
 * e = vertexB - vertexA, f = vertexC - vertexA.
 * The double cross lies in the triangle plane, despite the legacy normal name. */
void sBtlGetVerticalVector(const f32 *vertexA, const f32 *vertexB, const f32 *vertexC) {
    SDF_VU0_LOADVECTOR(vertexB);
    SDF_VU0_LOADVECTOR_B(vertexA);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR_B(vertexC);
    SDF_VU0_LOADVECTOR_BARRIER(vertexA);
    SDF_VU0_SUBVECTOR_B_A();
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
}

/* Dot the normalized double-cross direction with vertexA - vertexC. */
f32 sBtlGetVerticalLength(const f32 *vertexA, const f32 *vertexB, const f32 *vertexC) {
    f32 direction[4];
    f32 projection;
    sBtlGetVerticalVector(vertexA, vertexB, vertexC);
    VU0_STORE_VF($vf10, direction);
    VU0_LOAD_VF($vf10, vertexA);
    VU0_LOAD_VF($vf11, vertexC);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_B();
    VU0_LOAD_VF($vf10, direction);
        SDF_VU0_INNERPRODUCT(projection);
    return projection;
}

/* vu0 routine: vf10 = vertexC + direction * dot(direction, vertexA - vertexC).
 * Direction is the normalized in-plane double cross, not a triangle normal. */
f32 btlProjectOnPlaneVU(const f32 *vertexA, const f32 *vertexB, const f32 *vertexC) {
    f32 direction[4];
    f32 projection;
    sBtlGetVerticalVector(vertexA, vertexB, vertexC);
    SDF_VU0_STOREVECTOR(direction);
    SDF_VU0_LOADVECTOR(vertexA);
    SDF_VU0_LOADVECTOR_B(vertexC);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(direction);
    SDF_VU0_INNERPRODUCT(projection);
    VU0_SCALAR_OP(projection, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_LOADVECTOR_B(vertexC);
    SDF_VU0_ADDVECTOR();
    return projection;
}

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_002AC878);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_002AC888);

INCLUDE_ASM(const s32, "sBtlSub_0027ED08", func_002AC8F0);

INCLUDE_RODATA(const s32, "sBtlSub_0027ED08", D_r1_00458CA8);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_r1_0046DB14);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_004E2440);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_004E2444);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_004E2448);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_004E244C);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_004E2450);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_004E2454);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_r1_0046DB30);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_r1_0046DB38);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_r1_0046DB40);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_r1_0046DB48);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_r1_0046DB50);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_r1_0046DB58);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_r1_0046DB60);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_r1_0046DB68);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_r1_0046DB70);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_r1_0046DB78);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_r1_0046DB80);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_r1_0046DB88);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_r1_0046DB90);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_r1_0046DB98);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_r1_0046DBA0);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_r1_0046DBA8);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_r1_0046DBB0);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_r1_0046DBB8);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_r1_0046DBC0);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_r1_0046DBC8);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_r1_0046DBD0);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_r1_0046DBD8);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_004E2508);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_004E2510);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_004E2518);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_004E2520);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_004E2528);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_004E2530);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_004E2538);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_004E2540);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_004E2548);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_004E2550);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_004E2558);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_004E2560);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_004E2568);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_004E2570);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_004E2578);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_004E2580);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_004E2588);

INCLUDE_SDATA(const s32, "sBtlSub_0027ED08", D_004E2590);

