#include "common.h"
#include "sdfVu0.h"

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "kernel_draw.h"
#include "scr.h"
#include "sdfDevCons.h"
#include "sdf_resource.h"
#include "sdk_libc.h"

/* Load matrix into the VU0 scratch bank and postmultiply the primary bank. */

/* Rigid-invert the VU matrix in vf28-vf31 in place; translation becomes
 * -(R^T * t). The rotation input is assumed to contain no scale. */

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */

#include "dds/btl_command.h"
#include "dds/btl_projected_screen_point.h"
#include "dds/btl_resource_browser.h"
#include "dds/ee_mmi.h"
#include "dds/pcp_vu0.h"
#include "dds/fpu.h"

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

struct BtlRuntimeTask;

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

extern f32 sbtlGetVerticalLength(const f32 *, const f32 *, const f32 *);

typedef union BtlVec4 {
    f32 f[4];
    u128 q;
} BtlVec4;

struct BtlRuntimeTask;

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

extern f32 aViewEye[];
extern f32 sdfViewMatrix[];
extern f32 sdfProjectionMatrix[];
extern f32 dds3BillboardGetDirectionRotateData2[];

/* Select the smallest X for reference flag 0x200, otherwise the largest.
 * An empty indexed list returns NULL but still loads the native scratch vector. */

void sbtlGetVerticalVector(const f32 *vertexA, const f32 *vertexB, const f32 *vertexC);
f32 sbtlGetVerticalLength(const f32 *vertexA, const f32 *vertexB, const f32 *vertexC);

struct RefObj;

typedef struct BtnUv {
    s32 u;
    s32 v;
} BtnUv;

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

/* Select targets for the current command's action; return VM success. */
u32 btlScriptChooseActionTargets(void);

/* Parameter 0 supplies the group element-check input; return VM success. */
u32 btlScriptPickElementCheckedTarget(void);

/* Select the current command's minimum nonzero-HP candidate; return VM success. */
u32 btlScriptPickLowestHpTarget(void);

/* Select the current command's minimum HP-percentage candidate; return VM success. */
u32 btlScriptPickLowestHpRatioTarget(void);

/* Parameter 0 feeds lowest-HP element-block selection; return VM success. */
u32 btlScriptPickLowestHpBlockTarget(void);

/* Pick the current command's highest-MP target and return VM completion. */
u32 btlScriptPickHighestMpTarget(void);

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

/* Run the current script command's unlinked-party target selection; return script success. */
u32 btlScriptPickUnlinkedPartyTarget(void);

/* Run the current script command's linked-party target selection; return script success. */
u32 btlScriptPickLinkedPartyTarget(void);

/* Append this command's own actor after target scan; return VM success. */
u32 btlScriptAppendActorToTargets(void);

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
u32 btlCommandReportBossEffectOption(void);
u32 btlCmdTestEffectActor(void);
u32 btlCommandReportSpecialEnemyLink(void);

/* Report an eligible matching unit at or below the scripted HP percentage.
   Preserve the native signed products and their unsigned comparison. */
u32 btlCmdCheckHpPercent(void);
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
u32 btlCmdRunWeightedAiSelection(void);
u32 btlScriptSetBattleCommand(void);
u32 btlResetCommandContextAndSetStateFlag(void);
u32 btlScriptClearSceneTransition(void);
u32 btlScriptBeginSceneTransition(void);
u32 btlScriptFlagAllUnitsDefeatCandidates(void);
u32 btlScriptFlagPlayerDefeatCandidates(void);
u32 btlScriptFlagEnemyDefeatCandidates(void);
u32 btlCmdCameraMove(void);
u32 btlScriptQueueActorCommandSound(void);
u32 btlScriptQueueUnboundCommandSound(void);
s32 btlScriptSetCameraBlendStart(void);
s32 btlScriptSetCameraBlendEnd(void);
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
u32 btlCmdClearSpecialEnemyFlags(void);

#define EDITOR_KEY_COLS 13

#define EDITOR_KEY_ROWS 3

#define EDITOR_KEY_CELLS (EDITOR_KEY_COLS * EDITOR_KEY_ROWS)

#define EDITOR_CHARACTER_COUNT (EDITOR_KEY_CELLS - 1)

#define EDITOR_BLANK_CELL (EDITOR_KEY_CELLS - 1)

#define EDITOR_BACKSPACE_CELL EDITOR_KEY_CELLS

#define EDITOR_CONFIRM_CELL (EDITOR_KEY_CELLS + 1)

#define EDITOR_DELETE_CODE (EDITOR_KEY_CELLS + 3)

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
    SDF_VU0_LOADVECTOR_B(dds3BillboardGetDirectionRotateData2);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(&position);
    outPoint->x = (s32)position.f[0] - 0x700;
    outPoint->y = (s32)position.f[1] * 2 - 0xF20;
    return BTL_PROJECTED_SCREEN_POINT_WRITTEN;
}

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlCallFptodp);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlCallGrAllocPacketBuff);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlGetLargePara);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlParaAdd);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlParaAdd_16);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlParaAdd_8);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlParaDec);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlParaDec_16);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlParaDec_8);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlGetBezier);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlGetMeireiFont);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlCallBtlGetMeireiFont);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlCallDrawFontEx2);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlCallGetSmallNumFontFRQ);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlLeaf29);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlCallBtlGetWajutsuNameFont);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlLeaf73);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlCallGetSmallFontFRQ23);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sdatDevilName);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlCallDevilName);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlCallDestroyFontRequestAfter);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", slKanjiDrawN);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlGetSkillNameFont);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlDrawSkillName);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlLeaf74);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlCallGetSmallFontFRQ24);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlGetAisyoHelpFont);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlDrawAisyoName);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlGetWajutsuNameFont);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlGetPlayerRankNameFont);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlGetBSTNameFont);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlDrawBSTName);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlGetSkillMenu);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlIfDrawActionSub);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlCheckVerticalArea);

/* vu0 routine: vf10.xyz = normalize(e x (e x f)),
 * e = vertexB - vertexA, f = vertexC - vertexA.
 * The double cross lies in the triangle plane, despite the legacy normal name. */
void sbtlGetVerticalVector(const f32 *vertexA, const f32 *vertexB, const f32 *vertexC) {
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
f32 sbtlGetVerticalLength(const f32 *vertexA, const f32 *vertexB, const f32 *vertexC) {
    f32 direction[4];
    f32 projection;
    sbtlGetVerticalVector(vertexA, vertexB, vertexC);
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
f32 sbtlGetVerticalLength_Point(const f32 *vertexA, const f32 *vertexB, const f32 *vertexC) {
    f32 direction[4];
    f32 projection;
    sbtlGetVerticalVector(vertexA, vertexB, vertexC);
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

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlInitOutInPara);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlCalcOutInPara);

INCLUDE_ASM(const s32, "sbtlSub_0027ED08", sbtlInitInOutPara);

INCLUDE_RODATA(const s32, "sbtlSub_0027ED08", sbtlCallDestroyFontRequestAfterTable);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlGetAisyoHelpFontData);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", nbData24);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", slCallGetProcessData6Data);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlData165);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", cmpCallGetProcessData12Data);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", cmpData9);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlData166);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlLeaf73DataEntry2);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlGetMeireiFontDataEntry);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlGetMeireiFontDataEntry2);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlGetMeireiFontDataEntry3);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlGetMeireiFontDataEntry4);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlGetMeireiFontDataEntry5);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlGetMeireiFontDataEntry6);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlGetMeireiFontDataEntry7);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlGetMeireiFontDataEntry8);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlCallDestroyFontRequestAfterDataEntry);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlCallDestroyFontRequestAfterDataEntry2);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlCallDestroyFontRequestAfterDataEntry3);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlCallDestroyFontRequestAfterDataEntry4);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlCallDestroyFontRequestAfterDataEntry5);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlCallDestroyFontRequestAfterDataEntry6);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlCallDestroyFontRequestAfterDataEntry7);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlCallDestroyFontRequestAfterDataEntry8);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlCallDestroyFontRequestAfterDataEntry9);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlCallDestroyFontRequestAfterDataEntry10);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlCallDestroyFontRequestAfterDataEntry11);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlCallDestroyFontRequestAfterDataEntry12);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlCallDestroyFontRequestAfterDataEntry13);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlGetBSTNameFontDataEntry20);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlGetBSTNameFontDataEntry21);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlGetBSTNameFontDataEntry2);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlGetBSTNameFontDataEntry3);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlGetBSTNameFontDataEntry4);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlGetBSTNameFontDataEntry5);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlGetBSTNameFontDataEntry22);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlGetBSTNameFontDataEntry7);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlGetBSTNameFontDataEntry8);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlGetBSTNameFontDataEntry9);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlGetBSTNameFontDataEntry10);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlGetBSTNameFontDataEntry23);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlGetBSTNameFontDataEntry12);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlGetBSTNameFontDataEntry13);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlGetBSTNameFontDataEntry14);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlGetBSTNameFontDataEntry15);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlGetBSTNameFontDataEntry16);

INCLUDE_SDATA(const s32, "sbtlSub_0027ED08", sbtlGetBSTNameFontDataEntry17);

