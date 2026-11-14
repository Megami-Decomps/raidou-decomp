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

extern u8 D_003BB818[];

extern u8 sdfHddValid;

extern s32 btlBuiltinResourceNameIndex;

extern s8 D_003A54A8[];

extern s8 D_003A54C8[];

extern s8 D_003A54F0[];

extern s8 D_003A5518[];

extern s8 D_003A5538[];

extern s8 D_003A5558[];

extern u32 D_003BB6C8;

extern u8 D_003BB820[];

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

void func_001FB158(void);

void func_001FB170(void);

void func_001FB190(void);

void func_001FB1A0(void);

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

void btlBossDebugPrintfN(s32 x, s32 y, s32 color, s32 format, ...) {
}

void btlDebugDrawText(void) {
}

void btlDebugDrawRectangle(void) {
}

void btlDebugDrawTwoColorRectangle(void) {
}

void btlDebugDrawMappingColorSpan(void) {
}

void btlDebugDrawLine2D(void) {
}

void func_r1_0017BC40(void) {
}

void btlDebugDrawPoint2DNoOp(void) {
}

void btlDebugDrawPoint(void) {
}

void func_r1_0017BC58(void) {
}

void btlDebugDrawRadiusGuide(void) {
}

void btlDebugDrawOrientedExtentGuide(void) {
}

void btlDebugDrawScaledPositionGuide(void) {
}

void func_r1_0017BC78(void) {
}

void btlDebugDrawSegment(void) {
}

void btlDebugMenuBeforeRecordNoOp(void) {
}

void btlDebugMenuAfterRecordNoOp(void) {
}

INCLUDE_ASM(const s32, "code_0017BBD8", func_r1_0017BC98);

void btlDebugMenuRecordNoOp(void) {
}

INCLUDE_ASM(const s32, "code_0017BBD8", btlFindEntryByCommand);

INCLUDE_ASM(const s32, "code_0017BBD8", btlCountNonPartOpcodes);

s32 btlOpenPfsDebugDirectory(const char *directoryName) {
    char pathBuffer[BTL_DIRECTORY_PATH_BYTES];

    /* Debug mode opens a formatted pfs0 path; built-in mode resets name iteration. */

    if (sdfHddValid != 0) {
        sprintf(pathBuffer, "pfs0:/%s", directoryName);
        return sceDopen(pathBuffer);
    }
    btlBuiltinResourceNameIndex = 0;
    return 0;
}

/* Close the scanned directory only in debug filesystem mode; ignore the native result. */
void btlClosePfsDebugDirectory(s32 directoryHandle) {
    if (sdfHddValid == 0) {
        return;
    }
    sceDclose(directoryHandle);
}

INCLUDE_ASM(const s32, "code_0017BBD8", btlReadBattleResourceDirectoryEntry);

/* Build a scan-order list of built-in names and enabled resource-file categories. */
BtlResourceEntryList *btlScanDirectory(const char *path, s32 flags) {
    SceDirent reader;
    BtlResourceEntryList *list;
    BtlResourceEntry *entry;
    BtlResourceEntry *tail;
    s32 directoryHandle;
    s32 generalCount;
    s32 index;
    s32 length;
    s32 category;

    directoryHandle = btlOpenPfsDebugDirectory(path);
    list = sdfChipAllocDirect(sizeof(BtlResourceEntryList));
    if (path != NULL) {
        list->pathPrefix = sdfChipAllocDirect(strlen(path) + 1);
        strcpy(list->pathPrefix, path);
    } else {
        list->pathPrefix = NULL;
    }
    list->head = NULL;
    tail = NULL;
    list->count = 0;
    if (path == NULL) {
        return list;
    }
    if (flags & 8) {
        index = 0;
        generalCount = dds3BillboardGeneralNumber();
        for (; index < generalCount; index++) {
            entry = sdfChipAllocDirect(BTL_RESOURCE_ENTRY_BYTES);
            entry->category = 8;
            entry->value = index;
            entry->id = 0;
            sprintf(entry->name, "GENERAL %d", index);
            if (tail != NULL) {
                tail->next = entry;
            } else {
                list->head = entry;
            }
            entry->prev = tail;
            entry->next = NULL;
            tail = entry;
            list->count++;
        }
    }
    while ((length = btlReadBattleResourceDirectoryEntry(directoryHandle, &reader)) > 0) {
        if ((reader.stat.mode & 0xF000) == 0x1000) {
            continue;
        }
        if (length < 4) {
            continue;
        }
        if (length >= 0x2F) {
            continue;
        }
        if ((flags & 1)
            && (reader.name[length - 3] == 't' || reader.name[length - 3] == 'T')
            && (reader.name[length - 2] == 'm' || reader.name[length - 2] == 'M')
            && (reader.name[length - 1] == 'x' || reader.name[length - 1] == 'X')) {
            category = 1;
        } else if ((flags & 2)
            && (reader.name[length - 3] == 'p' || reader.name[length - 3] == 'P')
            && reader.name[length - 2] == '2'
            && (reader.name[length - 1] == 'a' || reader.name[length - 1] == 'A')) {
            category = 2;
        } else if ((flags & 4)
            && (reader.name[length - 2] == 'p' || reader.name[length - 2] == 'P')
            && (reader.name[length - 1] == 'b' || reader.name[length - 1] == 'B')) {
            category = 4;
        } else if ((flags & 0x20)
            && (reader.name[length - 2] == 'e' || reader.name[length - 2] == 'E')
            && (reader.name[length - 1] == 'p' || reader.name[length - 1] == 'P')) {
            category = 0x20;
        } else if ((flags & 0x10)
            && (reader.name[length - 3] == 'e' || reader.name[length - 3] == 'E')
            && (reader.name[length - 2] == 'p' || reader.name[length - 2] == 'P')
            && (reader.name[length - 1] == 'l' || reader.name[length - 1] == 'L')) {
            category = 0x10;
        } else if ((flags & 0x40)
            && (reader.name[length - 3] == 't' || reader.name[length - 3] == 'T')
            && (reader.name[length - 2] == 'l' || reader.name[length - 2] == 'L')
            && (reader.name[length - 1] == 'p' || reader.name[length - 1] == 'P')) {
            category = 0x40;
        } else if ((flags & 0x80)
            && (reader.name[length - 2] == 'f' || reader.name[length - 2] == 'F')
            && reader.name[length - 1] == '2') {
            category = 0x80;
        } else {
            continue;
        }
        if (category != 0) {
            entry = sdfChipAllocDirect(BTL_RESOURCE_ENTRY_BYTES);
            entry->category = category;
            entry->value = 0;
            entry->id = 0;
            strcpy(entry->name, reader.name);
            if (tail != NULL) {
                tail->next = entry;
            } else {
                list->head = entry;
            }
            entry->next = NULL;
            entry->prev = tail;
            list->count++;
            tail = entry;
        }
    }
    btlClosePfsDebugDirectory(directoryHandle);
    return list;
}

/* Release all scanned entries, the owned path prefix, and the list header. */
void btlDestroyEntryList(BtlResourceEntryList *list) {
    BtlResourceEntry *entry;

    if (list->head != NULL) {
        entry = list->head;
        do {
            BtlResourceEntry *next = entry->next;
            sdfChipFreeDirect(entry);
            entry = next;
        } while (entry != NULL);
    }
    sdfChipFreeDirect(list->pathPrefix);
    sdfChipFreeDirect(list);
}

/* Append a named browser entry without changing the current scan order. */
void btlAppendEntry(BtlResourceEntryList *list, const char *name, s32 category, s32 value, s32 id) {
    BtlResourceEntry *entry = sdfChipAllocDirect(BTL_RESOURCE_ENTRY_BYTES);
    BtlResourceEntry *tail;
    entry->category = category;
    entry->value = value;
    entry->id = id;
    strcpy(entry->name, name);
    if (list->head == NULL) {
        list->head = entry;
        entry->prev = NULL;
        entry->next = NULL;
    } else {
        tail = list->head;
        while (tail->next != NULL) {
            tail = tail->next;
        }
        tail->next = entry;
        entry->prev = tail;
        entry->next = NULL;
    }
    list->count++;
}

INCLUDE_ASM(const s32, "code_0017BBD8", btlCreateResourceDescriptor);

INCLUDE_ASM(const s32, "code_0017BBD8", Draw_effLensFlare);

/* Release an owned texture handle and the descriptor, but not its entry list. */
void btlDestroyResourceDescriptor(BtlResourceDescriptor *descriptor) {
    sdfTexHandle_t *texture = descriptor->texture;
    if (texture != NULL && descriptor->textureCategory == BTL_RESOURCE_ENTRY_CATEGORY_TMX) {
        sdfTexFreeDelayed(texture);
    }
    sdfChipFreeDirect(descriptor);
}

void btlSetResourceBrowserPosition(BtlResourceDescriptor *descriptor, s32 x, s32 y) {
    descriptor->originX = x;
    descriptor->originY = y;
}

/* Read the browser descriptor's zero-based selected entry index. */
u32 btlGetResourceBrowserSelectedIndex(
    const BtlResourceDescriptor *descriptor) {
    return descriptor->selectedIndex;
}

u32 btlGetResourceBrowserSelectionStatus(const BtlResourceDescriptor *descriptor) {
    return descriptor->selectionStatus;
}

/* Format prefix + selected name; return its resource category, not its id. */
s32 btlFormatSelectedResourceName(BtlResourceDescriptor *descriptor, char *output) {
    char *pathPrefix = descriptor->entryList->pathPrefix;
    if (pathPrefix != NULL) {
        sprintf(output, D_003BB818, pathPrefix, descriptor->selectedEntry->name);
    } else {
        sprintf(output, D_003BB820, descriptor->selectedEntry->name);
    }
    return descriptor->selectedEntry->category;
}

/* Strip from the first '.' onward; return the selected entry's category. */
s32 btlTrimResourceName(BtlResourceDescriptor *descriptor, char *output) {
    u32 nameLength;
    u32 characterIndex;
    sprintf(output, D_003BB820, descriptor->selectedEntry->name);
    nameLength = strlen(output);
    for (characterIndex = 0; characterIndex < nameLength && output[characterIndex] != '.'; characterIndex++) {
    }
    if (nameLength != characterIndex) {
        output[characterIndex] = 0;
    }
    return descriptor->selectedEntry->category;
}

/* Return the selected entry's category-specific metadata word unchanged. */
u32 btlGetResourcePathVariant(const BtlResourceDescriptor *resource) {
    const BtlResourceEntry *entry = resource->selectedEntry;
    return entry->value;
}

/* Replace an owned old texture, acquire the named resource's texture,
 * then release the temporary allocation returned by the resource reader. */
void btlLoadAndReplaceResourceHandle(BtlResourceDescriptor *descriptor, const char *resourceName) {
    u32 loadedResource;
    sdfTexHandle_t *texture = descriptor->texture;
    struct sdfMemHandle_t *allocation;
    if (texture != NULL && descriptor->textureCategory == BTL_RESOURCE_ENTRY_CATEGORY_TMX) {
        sdfTexFreeDelayed(texture);
        descriptor->texture = 0;
    }
    allocation = sdfDevLoadFileEx(resourceName, &loadedResource, 0);
    btlReplaceResourceHandle(descriptor, (void *)loadedResource);
    sdfMemFreeDirect(allocation);
}

/* Acquire a texture from the supplied resource, releasing an owned old handle. */
void btlReplaceResourceHandle(BtlResourceDescriptor *descriptor, void *textureResource) {
    sdfTexHandle_t *texture = descriptor->texture;
    if (texture != NULL && descriptor->textureCategory == BTL_RESOURCE_ENTRY_CATEGORY_TMX) {
        sdfTexFreeDelayed(texture);
        descriptor->texture = 0;
    }
    descriptor->texture = sdfTexSet((SdfTextureFileHeader *)(textureResource));
}

/* Queue the descriptor's preview texture as a quad at its screen position. */
void btlDrawResourcePreview(BtlResourceDescriptor *descriptor) {
    sdfDrawTag_t *list;
    void *sprite;
    dds3EffectSpriteVertex_t *vertex;
    sdfTexHandle_t *texture;
    sdfGrOt_t *surface;

    if (descriptor->texture != 0) {
        list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(list);
        sprite = sdf2DAllocSprite(1);
        vertex = (dds3EffectSpriteVertex_t *)sdf2DGetPrimPtr(sprite);
        vertex->a = 0x80;
        vertex->b = 0x80;
        vertex->g = 0x80;
        vertex->r = 0x80;
        texture = descriptor->texture;
        vertex->corner[1].u = texture->width << 4;
        vertex->corner[1].v = texture->height << 4;
        vertex->corner[0].u = 0;
        vertex->corner[0].v = 0;
        vertex->corner[1].flag = 0;
        vertex->corner[0].flag = 0;
        vertex->corner[0].x = (descriptor->originX << 4) + 0x7000;
        vertex->corner[0].y = ((descriptor->originY + 0xEA) << 3) + 0x7900;
        vertex->corner[0].mask = 0xFF0000;
        vertex->corner[1].x = vertex->corner[0].x + 0x800;
        vertex->corner[1].y = vertex->corner[0].y + 0x400;
        vertex->corner[1].mask = 0xFF0000;
        sdf2DLinkTexPacket(list, texture, SDF_GS_CONTEXT_1);
        sdfDrawTagLink(list, (u32)sprite);
        surface = &sdfGrOtTable[descriptor->drawSurfaceIndex];
        surface->regFunc(surface, list);
    }
}

/* Allocate the native name record and initialize its header and extension. */
struct BtlResourceNameRecord *btlCreateResourceNameRecord(const char *extension) {
    struct BtlResourceNameRecord *record;

    record = (struct BtlResourceNameRecord *)sdfChipAllocDirect(BTL_RESOURCE_NAME_RECORD_BYTES);
    record->maximumNameLength = 9;
    record->x = 8;
    record->y = 8;
    record->selectionStatus = BTL_RESOURCE_SELECTION_PENDING;
    record->nameLength = 0;
    record->selection = 0;
    record->overwritePromptActive = 0;
    strcpy(record->extension, extension);
    return record;
}

/* Release the chip allocation used for an editable resource-name record. */
void btlDestroyResourceNameRecord(struct BtlResourceNameRecord *record) {
    sdfChipFreeDirect(record);
}

INCLUDE_ASM(const s32, "code_0017BBD8", btlUpdateResourceNameEditor);

void btlSetResourceNamePosition(struct BtlResourceNameRecord *record, s32 x, s32 y) {
    record->x = x;
    record->y = y;
}

u32 btlGetResourceNameSelectionStatus(const struct BtlResourceNameRecord *record) {
    return record->selectionStatus;
}

/* Copy the resource name at +0x21 and store its length. */
void btlResourceRecordSetName(struct BtlResourceNameRecord *record, const char *name) {
    strcpy(record->resourceName, name);
    record->nameLength = strlen(name);
}

void btlFormatResourceNameWithExtension(const struct BtlResourceNameRecord *record, char *output) {
    sprintf(output, D_003BB818, record->resourceName, record->extension);
}

void btlFormatResourceNameWithoutExtension(const struct BtlResourceNameRecord *record, char *output) {
    sprintf(output, D_003BB820, record->resourceName);
}

void btlSetResourceNameLengthLimit(struct BtlResourceNameRecord *record, u32 maximumNameLength) {
    record->maximumNameLength = maximumNameLength;
}

INCLUDE_ASM(const s32, "code_0017BBD8", func_r1_0017D288);
INCLUDE_SDATA(const s32, "code_0017BBD8", D_r1_0046C168);

INCLUDE_SDATA(const s32, "code_0017BBD8", D_r1_0046C170);

INCLUDE_SDATA(const s32, "code_0017BBD8", D_r1_0046C178);

INCLUDE_SDATA(const s32, "code_0017BBD8", D_r1_0046C180);

INCLUDE_SDATA(const s32, "code_0017BBD8", D_r1_0046C188);

INCLUDE_SDATA(const s32, "code_0017BBD8", D_r1_0046C190);

INCLUDE_SDATA(const s32, "code_0017BBD8", D_r1_0046C198);

