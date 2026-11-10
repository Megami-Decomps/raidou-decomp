#include "common.h"

/* Declarations from dds-decomp src/dds1/game/code_00117438.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "basic.h"

#include "datCalc.h"

#include "newdata.h"

#include "sdk_libc.h"

#include "dds/btl.h"

#include "dds/dds3_path.h"

#include "dds/dds3_task_create.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/dds3obj.h"

#include "dds/eff_random_mod.h"

#include "dds/eff_transform.h"

#include "dds/mdl_flag_api.h"

#include "dds/sdf_dev_state.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdk_libc.h"

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

dds3Basic_t *dds3AddSceneBasicData(s32 kind);

/* Return the next caller-owned or shared state value reduced modulo `modulus`. */
u32 dds3GetRandIntA(EffRandState *state, u32 modulus);

dds3TaskKillRequestResult_t dds3KillProcessName(const char *name, s32 delayTicks);

void dds3PathEvalSpline(u32 *segment, f32 *weight, const Dds3PathKeyframes *keys, f32 frame);

/*
 * Allocate a task and place it in scheduler queues. Returns NULL on allocation
 * failure. On success, the scheduler owns the task handle and frees it after
 * its destroy callback; callers may borrow the handle only until finalization.
 */
dds3ProcessID_t *dds3StartProcess(const char *name, u32 priority, s32 startDelay,
                         s32 destroyDelay, dds3ProcessFunc_t update,
                         dds3ProcessShutdownFunc_t destroy, u32 userValue);

/* Return whether the selected flag bit is set, as EvtBitTestResult. */
EvtBitTestResult evtBitCheck(s32 flag);

void evtBitOff(s32 flag);

/* Set and clear one flag using the providers' signed-index adjustment. */
void evtBitOn(s32 flag);

extern void *memcpy(void *, const void *, unsigned int);

void *memset(void *dst, s32 value, u32 size);

/* Allocate from the chip heap through 0x400 bytes; larger requests use the
 * general heap and return its locked payload address. */
void *sdfAllocAuto(s32 size);

void sdfFileClose(sdfFileCmdHandle_t *state);

u32 sdfFileLength(sdfFileCmdHandle_t *state);

/* Stable state APIs. Callback factories keep caller-specific callback types. */
sdfFileCmdHandle_t *sdfFileOpen(const char *path);

void sdfFileRead(sdfFileCmdHandle_t *state, void *buffer, s32 byteCount);

/* Release the sdfAllocAuto base in its source heap; interior pointers are
 * invalid. */
s32 sdfFreeAuto(void *data);

struct sdfMemHandle_t *sdfMemAllocDirect(s32 requestedBytes);

struct sdfMemHandle_t *sdfMemAllocFromTailDirect(s32 requestedBytes);

void sdfMemFreeDirect(struct sdfMemHandle_t *allocation);

/* Return the descriptor's payload base as a byte address, not the descriptor. */
u8 *sdfMemGetPtr(const struct sdfMemHandle_t *block);

void *sdfMemLock(struct sdfMemHandle_t *allocation);

void sdfMemUnlock(struct sdfMemHandle_t *allocation);

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

extern u32 sdfRollActionHit(s32 channel, dds_datUnitWork_t *sourceRecord, dds_datUnitWork_t *packed);

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

s32 sdfDispatchUnitScriptDefault5(u32 unitIndex, dds_datUnitWork_t *sourceRecord,
    dds_datUnitWork_t *targetRecord, u8 mode);

/* Linearly interpolated curve sample at `time`; 0 when no curve is active. */
f32 dds3PathBasicControlGetFovy(dds3PathBasicControl_t *user);

#define SDF_FLOAT_COUNTER_FROZEN_FLAG 0x00000008

#define SDF_FLOAT_COUNTER_WRAP_FLAG 0x00000020

/* Step the counter by one; return zero when a non-looping endpoint is reached. */
s32 dds3PathBasicControlStep(SdfFloatCounter *counter);

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
s32 ptyComputeMaxHp(dds_datUnitWork_t *unit);

/* Compute the matching MP value from the base table or the MP script. */
s32 ptyComputeMaxMp(dds_datUnitWork_t *unit);

/* Sdf dispatch: pick the script id from the unit's runtime flag byte and run
 * it against the given entry index. 0x40/0x80 select the alternate scripts. */
s32 sdfDispatchUnitScriptDefault5(u32 unitIndex, dds_datUnitWork_t *sourceRecord,
    dds_datUnitWork_t *targetRecord, u8 mode);

s32 sdfDispatchCmd(u32 unitIndex, dds_datUnitWork_t *sourceRecord,
    dds_datUnitWork_t *targetRecord, u32 mode);

s32 sdfDispatchUnitScriptDefault9(u32 unitIndex, dds_datUnitWork_t *sourceRecord,
    dds_datUnitWork_t *targetRecord, u8 mode);

s32 evtRunCommandContext10(u32 unitIndex, dds_datUnitWork_t *sourceRecord,
    dds_datUnitWork_t *targetRecord, u8 mode);

s32 evtRunCommandContext7(u32 unitIndex, dds_datUnitWork_t *sourceRecord,
    dds_datUnitWork_t *targetRecord, u8 mode);

s32 sdfDispatchSubCmd(u32 unitIndex, dds_datUnitWork_t *sourceRecord,
    dds_datUnitWork_t *targetRecord, u32 mode);

s32 sdfCalculateCommandVitalDelta(s32 channel, dds_datUnitWork_t *sourceRecord, dds_datUnitWork_t *item, u32 mode, u8 vital);

extern char D_0039F980[];

/* Rolls whether the action hits: returns the surviving channel mask, or 0 on a miss. */
u32 sdfRollActionHit(s32 channel, dds_datUnitWork_t *sourceRecord, dds_datUnitWork_t *packed);

u32 sdfQueryChannelValue(s32 channel, dds_datUnitWork_t *sourceRecord, dds_datUnitWork_t *item);

u32 sdfQueryChannelBits(s32 channel, dds_datUnitWork_t *sourceRecord, dds_datUnitWork_t *item);

extern s32 sdfCalculateCommandVitalDelta(s32 channel, dds_datUnitWork_t *sourceRecord, dds_datUnitWork_t *item, u32 mode, u8 vital);

extern void datResetBadStat(dds_datUnitWork_t *item, s32 mask);

/* These unused-result legacy mutator calls had no prototype in scope. */
void sdfRaisePackedChannelValue(dds_datUnitWork_t *item, u32 minimumValue);

extern s32 datAddMp(dds_datUnitWork_t *unit, s32 delta);

s32 sdfApplyCommandResults(s32 channel, dds_datUnitWork_t *sourceRecord, dds_datUnitWork_t *item);

void sdfSetPackedValuePreservingFlag(dds_datUnitWork_t *item, u16 channelValue);

/* Raise the packed value to `minimumValue` (never lower it); reaching 0x40, 0x80 or 0x400 re-randomizes the entry. */
void sdfRaisePackedChannelValue(dds_datUnitWork_t *item, u32 minimumValue);
/* End of declarations from src/dds1/game/code_00117438.c */

/* Load the world node's borrowed data vector into VF10. */
void evtLoadValueVectorIntoVu(dds3Basic_t *node) {
    void *vec = dds3GetWorldNodeData(node);

    SDF_VU0_LOADVECTOR_BARRIER(vec);

}

/* Same, from the second quadword in the borrowed vector. */
void evtLoadValueSecondaryVectorIntoVu(dds3Basic_t *node) {
    void *vec = (void *)((u8 *)dds3GetWorldNodeData(node) + 0x10);

    SDF_VU0_LOADVECTOR_BARRIER(vec);

}

INCLUDE_ASM(const s32, "code_00121708", func_00121750);

INCLUDE_ASM(const s32, "code_00121708", func_001217D8);

INCLUDE_ASM(const s32, "code_00121708", func_00121828);

INCLUDE_ASM(const s32, "code_00121708", func_00121850);

void func_001218C0(void) {
}

INCLUDE_ASM(const s32, "code_00121708", func_001218C8);

INCLUDE_ASM(const s32, "code_00121708", func_00121C50);

INCLUDE_ASM(const s32, "code_00121708", func_00121EE8);

INCLUDE_ASM(const s32, "code_00121708", func_00121F20);

INCLUDE_ASM(const s32, "code_00121708", func_00121F40);

void func_00122078(void) {
}

void func_00122080(void) {
}

INCLUDE_ASM(const s32, "code_00121708", func_00122088);

INCLUDE_ASM(const s32, "code_00121708", func_00122090);

INCLUDE_ASM(const s32, "code_00121708", func_00122120);

INCLUDE_ASM(const s32, "code_00121708", func_00122218);

INCLUDE_ASM(const s32, "code_00121708", func_00122238);

INCLUDE_ASM(const s32, "code_00121708", func_001222F0);

INCLUDE_ASM(const s32, "code_00121708", func_001223A8);

INCLUDE_ASM(const s32, "code_00121708", func_00122428);

INCLUDE_ASM(const s32, "code_00121708", func_001225C0);

INCLUDE_ASM(const s32, "code_00121708", func_00122600);

INCLUDE_ASM(const s32, "code_00121708", func_00122778);

INCLUDE_ASM(const s32, "code_00121708", func_001227D8);

INCLUDE_ASM(const s32, "code_00121708", func_00122848);

INCLUDE_ASM(const s32, "code_00121708", func_001228B8);

INCLUDE_ASM(const s32, "code_00121708", func_001228F0);

INCLUDE_ASM(const s32, "code_00121708", func_00122918);

INCLUDE_ASM(const s32, "code_00121708", func_00122940);

INCLUDE_ASM(const s32, "code_00121708", func_00122988);

INCLUDE_ASM(const s32, "code_00121708", func_00122A20);

INCLUDE_ASM(const s32, "code_00121708", func_00122A98);

INCLUDE_ASM(const s32, "code_00121708", func_00122B00);

INCLUDE_ASM(const s32, "code_00121708", func_00122B80);

INCLUDE_ASM(const s32, "code_00121708", func_00122BC0);

INCLUDE_ASM(const s32, "code_00121708", func_00122C78);

INCLUDE_ASM(const s32, "code_00121708", func_00122DA0);

INCLUDE_ASM(const s32, "code_00121708", func_00122DE0);

INCLUDE_ASM(const s32, "code_00121708", func_00122EC8);

INCLUDE_ASM(const s32, "code_00121708", func_00122F18);

INCLUDE_ASM(const s32, "code_00121708", func_00123110);

INCLUDE_ASM(const s32, "code_00121708", func_00123130);

INCLUDE_ASM(const s32, "code_00121708", func_00123168);

INCLUDE_ASM(const s32, "code_00121708", func_001232B0);

INCLUDE_ASM(const s32, "code_00121708", func_00123368);

INCLUDE_ASM(const s32, "code_00121708", func_00123380);

INCLUDE_ASM(const s32, "code_00121708", func_001234E0);

void func_00123548(void) {
}

INCLUDE_ASM(const s32, "code_00121708", func_00123550);

INCLUDE_ASM(const s32, "code_00121708", func_00123590);

INCLUDE_ASM(const s32, "code_00121708", func_00123608);

INCLUDE_ASM(const s32, "code_00121708", func_00123698);

INCLUDE_ASM(const s32, "code_00121708", func_001236D0);

INCLUDE_ASM(const s32, "code_00121708", func_001238E8);

INCLUDE_ASM(const s32, "code_00121708", func_00123938);

INCLUDE_ASM(const s32, "code_00121708", func_00123A10);

INCLUDE_ASM(const s32, "code_00121708", func_00123A88);

INCLUDE_ASM(const s32, "code_00121708", func_00123BA0);

INCLUDE_ASM(const s32, "code_00121708", func_00123F68);

INCLUDE_ASM(const s32, "code_00121708", func_00124180);

INCLUDE_ASM(const s32, "code_00121708", func_001241D0);

INCLUDE_ASM(const s32, "code_00121708", func_001246F0);

INCLUDE_ASM(const s32, "code_00121708", func_00124790);

INCLUDE_ASM(const s32, "code_00121708", func_001247B8);

INCLUDE_ASM(const s32, "code_00121708", func_001247C0);

INCLUDE_ASM(const s32, "code_00121708", func_001247E0);

INCLUDE_ASM(const s32, "code_00121708", func_00124828);

INCLUDE_ASM(const s32, "code_00121708", func_00124830);

INCLUDE_ASM(const s32, "code_00121708", func_00124998);

INCLUDE_ASM(const s32, "code_00121708", func_001249B8);

INCLUDE_ASM(const s32, "code_00121708", func_001249C0);

INCLUDE_ASM(const s32, "code_00121708", func_001249F8);

INCLUDE_ASM(const s32, "code_00121708", func_00124A10);

INCLUDE_ASM(const s32, "code_00121708", func_00124A28);

INCLUDE_ASM(const s32, "code_00121708", func_00124A40);

INCLUDE_ASM(const s32, "code_00121708", func_00124A58);

INCLUDE_ASM(const s32, "code_00121708", func_00124A98);

INCLUDE_ASM(const s32, "code_00121708", func_00124B18);
INCLUDE_SDATA(const s32, "code_00121708", D_004DE5E0);

INCLUDE_SDATA(const s32, "code_00121708", D_004DE5E8);

INCLUDE_SDATA(const s32, "code_00121708", D_004DE5EA);

INCLUDE_SDATA(const s32, "code_00121708", D_004DE5EC);

INCLUDE_SDATA(const s32, "code_00121708", D_004DE5F0);

INCLUDE_SDATA(const s32, "code_00121708", D_004DE5F1);

INCLUDE_SDATA(const s32, "code_00121708", D_004DE5F4);

INCLUDE_SDATA(const s32, "code_00121708", D_004DE5F8);

INCLUDE_SDATA(const s32, "code_00121708", D_004DE5FC);

INCLUDE_SDATA(const s32, "code_00121708", D_004DE600);

INCLUDE_SDATA(const s32, "code_00121708", D_004DE604);

INCLUDE_SDATA(const s32, "code_00121708", D_004DE608);

INCLUDE_SDATA(const s32, "code_00121708", D_004DE618);

INCLUDE_SDATA(const s32, "code_00121708", D_004DE624);

INCLUDE_SDATA(const s32, "code_00121708", D_004DE628);

INCLUDE_SDATA(const s32, "code_00121708", D_004DE63C);

INCLUDE_SDATA(const s32, "code_00121708", D_004DE644);

INCLUDE_SDATA(const s32, "code_00121708", D_004DE670);

INCLUDE_SDATA(const s32, "code_00121708", D_004DE6BC);

INCLUDE_SDATA(const s32, "code_00121708", D_004DE6C0);

INCLUDE_SDATA(const s32, "code_00121708", D_004DE6C4);

INCLUDE_SDATA(const s32, "code_00121708", D_004DE6C8);

INCLUDE_SDATA(const s32, "code_00121708", D_004DE6CC);

INCLUDE_SDATA(const s32, "code_00121708", D_004DE6D0);

INCLUDE_SDATA(const s32, "code_00121708", D_004DE6D4);

INCLUDE_SDATA(const s32, "code_00121708", D_004DE6D8);

