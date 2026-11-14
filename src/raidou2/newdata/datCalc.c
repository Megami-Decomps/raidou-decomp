#include "common.h"

/* Declarations from smt3-decomp src/smt3/newdata/datCalc.c */
#include "common.h"

#include "sdk_libgcc.h"

#include "datCalc.h"

#include "ps2asm.h"

extern s32 evtBitCheck(s32 no);

typedef struct datTakaraSagasiItem_t {
    u16 item;
    u16 par;
} datTakaraSagasiItem_t;

typedef struct datTakaraSagasiTable_t {
    u32 flag;
    datTakaraSagasiItem_t itemtbl[8];
} datTakaraSagasiTable_t;

/* FLD_SAGASI_AKUMA reads four inline 0x24-byte groups and their weights. */
typedef struct datTakaraSagasi_t {
    u32 flag;
    datTakaraSagasiTable_t tbl[4];
} datTakaraSagasi_t;

extern datTakaraSagasi_t datSagasiSkillTable;

extern void datDebugPrintf(const char *format, ...);

extern s32 GetButuriAttack(s32 nskill, datUnitWork_t *s, datUnitWork_t *d, s32 waza);

extern u32 datGetAisyo(datUnitWork_t *work, s32 attr);

extern s32 func_0022E168(u16 devilId);

typedef struct
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 skill;
    /* 0x04 */ u8 price[4];
} datItem_t;

extern datItem_t datItemTable[];

extern s32 datGetItemSlotIndex(s32 id);

extern void evtBitOff(s32 no);

extern datUnitWork_t datPlayerUnitTemplate;

extern s32 memset(void *a, s32 b, s32 c);

#include "sdk_libc.h"

extern s32 datCopyDevilWorkEntry(datUnitWork_t *work, s32 level);

extern s32 fclEncycRegist(datUnitWork_t *work, s32 overwrite);

extern s32 datBossRashGetRec(s32 eid, s32 type);

extern datSkill_t attr[];

extern u32 dds3GetRandIntA(void *state, u32 limit);

extern s32 datCountRegisteredHearts(void);

extern void datAddHeartsEntry(s32 id, datUnitWork_t *work);

extern void evtBitOn(s32 no);

extern void cmbAddHeartsParam(s32 devilId, datUnitWork_t *work);

extern void rstLoadHeartsSkill(s8 HeartsID, datUnitWork_t *pStock);

extern u32 datAisyoTable[248][16];

extern void cmbSubHeartsParam(s32 devilId, datUnitWork_t *work);

extern void rstStoreHeartsSkill(s8 HeartsID, datUnitWork_t *pStock);

extern void datSetAutoKaifukuSkill(datUnitWork_t *work, s32 skill);

extern s32 datAddDevil(s32 id, s32 sw);

extern f32 D_0054E478;

extern s32 cmbGetHeartsParam(s8 devilId, s8 index);

extern void cmpSetMaxHPMP(datUnitWork_t *work);

extern s32 datGetPlayerLNDLevel(datUnitWork_t *work);

extern datDevilFormat_t datDevilTable[];

s32 datGetBaseMaxHp(datUnitWork_t *work);

s32 datGetBaseMaxMp(datUnitWork_t *work);

s32 datGetNormalAtkPow(datUnitWork_t *work);

s32 datGetMagicAtkPow(datUnitWork_t *work);

s32 datGetNormalHitPow(datUnitWork_t *work);

s32 datGetMagicHitPow(datUnitWork_t *work);

s32 datGetDefPow(datUnitWork_t *work);

s32 datGetSakePow(datUnitWork_t *work);

s32 GetButuriAttack(s32 nskill, datUnitWork_t *s, datUnitWork_t *d, s32 waza);

s32 GetButuriKaifuku(s32 nskill, datUnitWork_t *s, datUnitWork_t *d, s32 waza);

s32 GetMagicAttack(s32 nskill, datUnitWork_t *s, datUnitWork_t *d, s32 waza);

s32 GetMagicKaifuku(s32 nskill, datUnitWork_t *s, datUnitWork_t *d, s32 waza);

s32 GetMaxHpWazaPoint(s32 nskill, datUnitWork_t *s, datUnitWork_t *d, s32 waza);

f32 GetAisyoRitu(s32 nskill, datUnitWork_t *s, datUnitWork_t *d);

struct sdfRandCB_t;

extern f32 dds3GetRandFloatA(struct sdfRandCB_t *state);

s32 datGetSkillKouka(s32 nskill, s32 kind, datUnitWork_t *s, datUnitWork_t *d);

extern s32 datGetSkillKouka(s32 nskill, s32 kind, datUnitWork_t *s, datUnitWork_t *d);

extern s32 datGetSkillBadKouka(s32 nskill, datUnitWork_t *s, datUnitWork_t *d);

extern void datResetBadStat(datUnitWork_t *work, s32 badstat);

extern void datSetBadStat(datUnitWork_t *work, u32 badstat);

extern s32 datAddMp(datUnitWork_t *work, s32 amount);

s32 datExecSkill(s32 nskill, datUnitWork_t *s, datUnitWork_t *d);

void datOverwriteBadStat(datUnitWork_t *work, u32 badstat);

void datSetBadStat(datUnitWork_t *work, u32 badstat);

void datResetBadStat(datUnitWork_t *work, s32 badstat);

extern s32 datCheckSyojiSkill(datUnitWork_t *work, s32 skill);

u32 datGetMaxHp(datUnitWork_t *work);

u32 datGetMaxMp(datUnitWork_t *work);

s32 datAddHp(datUnitWork_t *work, s32 hp);

s32 datAddMp(datUnitWork_t *work, s32 amount);

s32 datGetBaseParam(datUnitWork_t *work, s32 paratype);

s32 datGetParam(datUnitWork_t *work, s32 paratype);

s32 datCheckSyojiSkill(datUnitWork_t *work, s32 skill);

u32 datGetAisyo(datUnitWork_t *work, s32 attr);

u32 datGetAisyoValue(datUnitWork_t *work, s32 attr);

u32 datGetAisyoFlag(datUnitWork_t *work, s32 attr);

s32 datGetBadStatusAttr(s32 bad);

s32 datCheckDying(datUnitWork_t *work);

void datAddMaka(s32 addmaka);

void datAddMakaPar(s32 addmaka);

s32 datCheckMaka(s32 maka);

extern s32 datAddHearts(s32 id);

void datAddItem(s32 id, s32 num);

s32 datCheckItem(s32 id, s32 num);

s32 datCheckItemFull(s32 id);

s32 datGetRace(datUnitWork_t *work);

s32 datCheckTalk(datUnitWork_t *work);

s32 datSearchDevilStock(s32 id);

s32 datGetPlayerLevel(void);

s32 datCheckPlayerDead(void);

void datAddPlayerParam(s32 id, s32 add);

s32 datGetNakamaParam(s32 did, s32 id);

s32 datGetPlayerLND(void);

/* NON_MATCHING: 28 of 48 words differ (first at +0x44). */
void datDeadNakama(s32 did);

/* Partial view of the persistent party and stock records. */
typedef struct {
    u8 unknown00[0xA50];
    /* 0xA50 */ s32 maxstock;
    /* 0xA54 */ datUnitWork_t unitwork[16];
    /* 0x1914 */ s8 stocklist[16];
    /* 0x1924 */ s32 stockcnt;
    /* 0x1928 */ u8 unknown1928[0xA0];
} datPartyWork_t;

/* The same persistent-work pointer, through its recovered party/stock view. */

void datPartyDamage(s32 mode);

void datPartyDamageFld(s32 mode);

/* NON_MATCHING: 54 of 68 words differ (first at +0x4). */
void datPartyDamagePipe(s32 big, s32 sml);

void datPartyKaifuku(void);

s32 datGet3Zokusei(datUnitWork_t *work);

s32 datCheckStockFull(void);

void datSetDevilWork(s32 id, datUnitWork_t *work);

s32 datAddDevil(s32 id, s32 sw);

s32 datAddUniqueDevil(s32 id, s32 sw, s32 uid);

s32 datSearchUniqueDevilStock(s32 uid);

s32 datGetUniqueDevilId(s32 uid);

void datAddExp(datUnitWork_t *w, s32 exp);

s32 datAddHearts(s32 id);

void datRemoveHearts(void);

void datEquipHearts(s32 HeartsID);

void datFieldSkillKaifuku(void);

s32 datCheckSkillInParty(s32 skillid);

s32 datCheckSkillTakaraSagasi(void);

void datClearCommMemory(datUnitWork_t *work);

void datAllClearCommMemory(void);

s32 datGetNakamaNum(void);

void datAddKotowariPoint(s32 type, s32 point);

s32 datGetKotowariLank(s32 lank);

s32 datGetSkillCost(datUnitWork_t *w, s32 nskill);

/* NON_MATCHING: 7 of 75 words differ (first at +0xE4). */
/* NON_MATCHING. Behaviour checked against retail: 1 PCSX2 call vector give the same return and memory,
 * seq41, covering only the paths those calls took.
 * (sdfport 73e010f, tools/funcvec, tests/funcvec.sh). */
void datInitPlayer(void);

void datInitBossPress(void);

void datDelDevil(u16 id);

s32 datGetItemSkillNo(s32 itemid);

u16 datGetLevelForDraw(datUnitWork_t *work);

void datCopyDevilWork(s32 id, datUnitWork_t *w);

s32 datBossRashGetRec(s32 eid, s32 type);

void datBossRashUpdRec(s32 eid, s32 press);

s32 datBossRashChk(s32 eid);
/* End of declarations from src/smt3/newdata/datCalc.c */

/* Declarations from dds-decomp src/dds1/game/code_00117438.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#define SMT3_DEFINES_datItem_t 1

#define SMT3_DEFINES_datPartyWork_t 1

#define SMT3_DEFINES_datTakaraSagasiItem_t 1

#define SMT3_DEFINES_datTakaraSagasiTable_t 1

#define SMT3_DEFINES_datTakaraSagasi_t 1

#include "basic.h"

#include "dds/btl.h"

#include "dds/dds3_path.h"

#include "dds/dds3_task_create.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/dds3obj.h"

#include "dds/eff_transform.h"

#include "dds/sdf_dev_state.h"

#include "dds/sdf_heap_api.h"

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

extern void *memcpy(void *, const void *, unsigned int);

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

extern scrHeader_t *D_r1_0046B604;

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

/* These unused-result legacy mutator calls had no prototype in scope. */
void sdfRaisePackedChannelValue(dds_datUnitWork_t *item, u32 minimumValue);

s32 sdfApplyCommandResults(s32 channel, dds_datUnitWork_t *sourceRecord, dds_datUnitWork_t *item);

void sdfSetPackedValuePreservingFlag(dds_datUnitWork_t *item, u16 channelValue);

/* Raise the packed value to `minimumValue` (never lower it); reaching 0x40, 0x80 or 0x400 re-randomizes the entry. */
void sdfRaisePackedChannelValue(dds_datUnitWork_t *item, u32 minimumValue);
/* End of declarations from src/dds1/game/code_00117438.c */

/* Declarations from dds-decomp src/dds1/newdata/datCalc.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#define SMT3_DEFINES_EFF_WORLD_KIND_ACTION_10 1

#define SMT3_DEFINES_EFF_WORLD_KIND_ACTION_B 1

#define SMT3_DEFINES_EFF_WORLD_KIND_ACTION_D 1

#define SMT3_DEFINES_EFF_WORLD_KIND_CAMERA 1

#define SMT3_DEFINES_EFF_WORLD_KIND_COUNT 1

#define SMT3_DEFINES_EFF_WORLD_KIND_EFFECT_OBJECT 1

#define SMT3_DEFINES_EFF_WORLD_KIND_EFFECT_TRANSFORM 1

#define SMT3_DEFINES_EFF_WORLD_KIND_EVENT_TABLE 1

#define SMT3_DEFINES_EFF_WORLD_KIND_FOLLOW_MODEL 1

#define SMT3_DEFINES_EFF_WORLD_KIND_LIGHT 1

#define SMT3_DEFINES_EFF_WORLD_KIND_RESOURCE_OWNER 1

#define SMT3_DEFINES_EFF_WORLD_KIND_SCRIPT_OBJECT 1

#define SMT3_DEFINES_EFF_WORLD_KIND_SLOT_RING 1

#define SMT3_DEFINES_EFF_WORLD_KIND_TRANSFORM_SOURCE 1

#define SMT3_DEFINES_EFF_WORLD_KIND_WORLD_INDEX_OWNER 1

#define SMT3_DEFINES_EFF_WORLD_KIND_WORLD_INFO 1

#include "dds/scr.h"

#include "dds/scr_integer_parameter_api.h"

#include "dds/scr_procedure_start_api.h"

scrProcessWork_s *scrCreateTaskWithDefaultOption(void *data);

/* Read a command parameter as a signed VM integer, converting floats and
 * dereferencing global integer or float references. */
ScrVmInteger scrGetIntPara(ScrCommandParameterIndex parameterIndex);

void scrPrintProcedureEnd(scrProcessWork_s *pw);

s32 scrRunScript(scrProcessWork_s *h);

ScrProcedureStartResult scrStartProcessProcedure(scrProcessWork_s *process,
    ScrProcedureIndex procedureIndex);

#include "dds/sdk_ps2.h"

#include "dds/common.h"

#include "dds/pty_experience_threshold_type.h"

#include "dds/scr_integer_parameter_api.h"

#include "dds/scr.h"

#include "dds/scr_procedure_start_api.h"

#include "dds/evt_script_context.h"

#include "dds/evt_solar.h"

#include "dds/evt_item_value_threshold_api.h"

#include "dds/evt_random_status_clear_api.h"

#include "dds/pty_roster_value_record.h"

#include "dds/pty_adjust_item_quantity_api.h"

#include "dds/dat_state.h"

#include "dds/dat_battle_adjustment_condition.h"

#include "dds/dat_currency_requirement_api.h"

#include "dds/dat_calculated_stat_value_api.h"

#include "dds/dat_calculate_command_base_value_api.h"

#include "dds/dat_unit_dying_api.h"

#include "dds/btl_scene_party_level_threshold_api.h"

#include "dds/btl_actor_attribute_api.h"

#include "dds/dat_affinity_selector.h"

#include "dds/dat_affinity_value_api.h"

#include "dds/dat_effective_affinity_api.h"

#include "dds/btl_conditional_encounter_roll_api.h"

#include "dds/dat_world_field_flags.h"

#include "dds/btl_affinity_row.h"

#include "dds/btl_resolve_unit_value_api.h"

#include "dds/evt_randomize_entry_api.h"

#include "dds/mdl_flag_api.h"

#include "dds/pty_active_roster_index_api.h"

#include "dds/pty_roster_api.h"

#include "dds/pty_party_unit_presence_api.h"

#include "dds/pty_frontline_promotion_api.h"

#include "dds/pty_status_query_api.h"

#include "dds/pty_event_penalty_api.h"

#include "dds/pty_field_hp_attrition_api.h"

#include "dds/pty_read_roster_stat_api.h"

#include "dds/dat_unit_has_skill_api.h"

#include "dds/pty_skill_recovery_api.h"

#include "dds/pty_item_quantity_update_view.h"

#include "dds/pty_accumulate_stat_gains_api.h"

#include "dds/eff_random_mod.h"

#define DAT_EXTERNAL_SKILL_TABLE 0x20

#define DAT_INLINE_SKILL_COUNT 0x18

#define DAT_TABLE_SKILL_COUNT 8

#define DAT_SKILL_HP_BONUS_SMALL 0x200

#define DAT_SKILL_HP_BONUS_MEDIUM 0x201

#define DAT_SKILL_HP_BONUS_LARGE 0x202

#define DAT_SKILL_MP_BONUS_SMALL 0x203

#define DAT_SKILL_MP_BONUS_MEDIUM 0x204

#define DAT_SKILL_MP_BONUS_LARGE 0x205

#define DAT_PERCENT_SCALE 100

#define DAT_BONUS_SMALL_PERCENT 10

#define DAT_BONUS_MEDIUM_PERCENT 20

#define DAT_BONUS_LARGE_PERCENT 30

#define DAT_BOOSTED_RESOURCE_LIMIT 1000

#define DAT_BOOSTED_RESOURCE_MAX 999

#define DAT_STATUS_VALUE_MASK 0x7FFF

#define DAT_STATUS_STAT_OVERRIDE 0x1000

#define DAT_PROFILE_STAT_LIMIT 100

#define DAT_PROFILE_STAT_MAX 99

#define DAT_CALC_HIGH_MASK 0xFFFF0000

#define DAT_CALC_SKIP_STATUS_MASK 0x70000

#define DAT_QUARTER_PERCENT 25

#define DAT_CURRENCY_MAX 0x98967F

typedef struct UiObject {
    u8 unk_00[0x110];
    u32 flags;
    u8 unk_114[0x10];
    u16 index;
    u16 currentValue;
    u16 maximumValue;
    u8 unk_12A[4];
    u16 statusFlags;
} UiObject;

/* Add each owned HP bonus separately against the original maximum, truncating
 * each unsigned percentage independently. Only inline-skill units cap at 999. */
u32 datComputeSkillBoostedMaxHp(dds_datUnitWork_t *unit);

/* MP bonuses have the same independent truncation and conditional cap as HP. */
u32 datComputeSkillBoostedMaxMp(dds_datUnitWork_t *unit);

extern s32 ptyGetCurrentProfileId(const dds_datUnitWork_t *unit);

extern u32 prfGetIndexedProfileByte(u16 profileId, s32 statIndex);

/* Add the signed per-unit stat byte to its profile adjustment and clamp
 * the existing signed result to 1..99. Stat index is not checked here. */
DatCalculatedStatValue datGetClampedProfileAdjustedStat(
    const dds_datUnitWork_t *unit, DatBaseStatIndex statIndex);

/* Only an exact masked status of 0x1000 forces one; combinations of low
 * status bits do not take this path. Otherwise use the profile-adjusted stat. */
DatCalculatedStatValue datGetStatWithStatusOverride(
    const dds_datUnitWork_t *unit, DatBaseStatIndex statIndex);

/* Search 24 inline skill IDs or eight IDs in the selected external record.
 * Return on the first match; no record-index or pointer validation here. */
DatUnitHasSkillResult datUnitHasSkill(
    const dds_datUnitWork_t *unit, DatSkillId skillId);

/* Unless a skip-status bit is set, adjust only the low 16 bits by exact masked
 * status: 8/0x100 floor at 200, 2 at 150, 1 at 300, and 4 forces one.
 * High bits are retained; combined or unlisted statuses leave the value alone. */
s32 datRaiseCalculatedValueFloor(const dds_datUnitWork_t *unit, s32 packedValue);

extern const BtlAffinityRow *datAffinityTablePrimary;

extern const BtlAffinityRow *datAffinityTableAlternate;

extern const BtlAffinityRow *datExternalAffinityTable;

DatEffectiveAffinityResult datGetEffectiveAffinity(
    const dds_datUnitWork_t *unit, DatAffinitySelector selector);

/* Map one exact channel flag to an affinity-table column. Flag one and
 * unknown/combined flags have no column; several flags share a destination. */
DatAffinitySelector datFlagToElementIndex(s32 channelFlag);

/* Add delta using the existing s32 addition, then clamp the currency balance
 * to 0..9,999,999. No overflow validation is performed before the clamp. */
void datAddCurrencyClamped(s32 delta);

/* Compare the signed balance against the requested amount; no debit or
 * validation is performed, so negative requests keep their original semantics. */
DatCurrencyRequirementResult datHasEnoughCurrency(s32 requiredAmount);

#include "dds/common.h"

#include "dds/scr.h"

#include "dds/evt_run_context_api.h"

#include "dds/scr_work_task_api.h"

#include "dds/dat_state.h"

#include "dds/sdf.h"

#include "dds/btl_action.h"

#include "dds/dat_command.h"

#include "dds/dat_command_record_index.h"

enum {
    PTY_ACTIVE_ROSTER_COUNT = 5,
    PTY_RECOVERY_DELTA = 9999,
    PTY_MAX_LEVEL = 99,
    PTY_LEVEL_LIMIT = 100
};

enum {
    EVT_DEFAULT_STAT_VALUE = 100,
    EVT_ROSTER_DETAIL_KIND = 5,
    EVT_DEFAULT_PARTY_RECORD_FLAGS_MASK = 4,
    EVT_ALTERNATE_PARTY_RECORD_FLAGS_MASK = 0x20,
    EVT_CONTEXT_BYTES = 0x18
};

enum {
    PTY_ITEM_MAX_QUANTITY = 99,
    PTY_ITEM_QUANTITY_LIMIT = 100,
    PTY_ITEM_SINGLE_MAX = 1,
    PTY_ITEM_SINGLE_LIMIT = 2
};

enum {
    BTL_SCENE_RECORD_BYTES = 0x28,
    BTL_SCENE_ENEMY_SLOT_COUNT = 11,
    BTL_ENEMY_RECORD_BYTES = 0x4C,
    BTL_ENEMY_LEVEL_OFFSET = 5
};

extern u8 ptyAffinityPermutationOrders[];

extern PtyAffinityMatchResult
ptyUnitMatchesAffinityRequirement(
    const dds_datUnitWork_t *unit, BtlCommandId commandId, u8 requirementIndex);

extern EventRosterStat *datRosterDetails;

extern s32 btlConditionalEncounterCachedInterval;

extern dds_datUnitWork_t *D_003BAA04;

extern PtyRosterValueRecord ptyRosterMenuValues[];

extern PtyItemQuantityUpdateView ptyItemQuantityUpdates[];

extern scrProcessWork_s *evtWorkScriptTask;

extern EvtScriptContext evtWorkScriptContext;

extern struct scrProcessWork_s *scrSetIntReturnValue(s32 retVal);

extern struct scrProcessWork_s *scrSetFloatReturnValue(f32 value);

extern dds_datUnitWork_t *dds3FindEntry(DatUnitId unitId);

extern void ptyMergeStockSkills(dds_datUnitWork_t *unit);

extern u8 btlIsRuntimeAllocated(void);

extern f32 btlGetEncounterAdjustmentScale(void);

extern u32 btlGetEscapeAttemptCount(void);

/* Flag-range items set their model flag regardless of quantityDelta.
 * Other items add the delta to their byte quantity and clamp it. */
void ptyAdjustItemQuantity(PtyItemId itemId, PtyItemQuantityDelta quantityDelta);

/* Flag-range items test their model flag and ignore minimumQuantity.
 * Other items require at least the requested byte quantity. */
EvtItemValueThresholdResult evtCheckValueThreshold(PtyItemId itemId, s32 minimumQuantity);

/* Flag-range items have one slot; every quantity range caps at 99. */
u8 ptyIsInventoryItemAtCapacity(PtyItemId itemId);

u8 evtGetFlaggedRosterValue(dds_datUnitWork_t *entry);

PtyActiveRosterIndex dds3FindEntryIndex(DatUnitId unitId);

/* Read one indexed signed base-stat value from the first active roster record. */
s8 ptyReadSignedRosterStatByte(DatBaseStatIndex statIndex);

/* Event penalties affect living roster slots, then optionally raise a status channel. */
void ptyApplyEventPenalty(PtyEventPenaltyMode penaltyMode);

/* Apply field HP attrition without allowing a living roster entry to reach zero. */
void ptyApplyFieldHpAttrition(PtyFieldHpAttritionMode mode);

/* Recover HP/MP in each occupied roster slot and preserve only status bit 15. */
void ptyRecoverAllUnits(void);

/* Check occupied, nonzero-HP roster members for a status bit. */
PtyPartyStatusMatchResult ptyAnyLivingUnitHasStatus(
    u32 statusMask, PtyPartyStatusScope scope);

/* Apply the owned skill's positive HP/MP recovery rate; unsupported skills do nothing. */
void ptyApplySkillRecovery(dds_datUnitWork_t *unit, DatSkillId skillId);

/* Apply the two selected recovery skills to occupied frontline entries. */
void evtUpdateFlaggedStats(void);

/* Return whether an occupied frontline entry owns the requested skill. */
s32 evtHasMatchingFlaggedEntry(DatSkillId skillId);

/* Resolve the command's unmodified integer value for this entry.
 * Mode one scales max HP and floors the result at one unless the entry is
 * enemy-flagged; mode two returns a fixed value subject to the enemy flag gate.
 * commandId selects the record; cost holds the resolved result. */
DatCommandBaseCost datCalculateCommandBaseValue(
    dds_datUnitWork_t *battler, BtlCommandId commandId);

/* Return the record's command index, zero-extended: its callers read a word. */
DatCommandRecordIndex evtGetIndexedEventRecordId(s32 tableIndex);

/* Find the first occupied party record for this unit ID, or return NULL. */
dds_datUnitWork_t *dds3FindEntry(DatUnitId unitId);

/* Promote the selected roster entry into the current frontline boundary. */
PtyFrontlinePromotionResult ptyRebalanceFrontline(DatUnitId unitId);

PtyPartyUnitPresenceResult ptyIsPartyUnitPresent(DatUnitId unitId);

/* Return the highest occupied-slot level, or zero when no slot is occupied. */
s32 dds3EntryMax(void);

/* Ceiling average of occupied-slot levels; native code assumes a nonempty roster. */
s32 ptyGetAverageLevel(void);

extern PtyExperienceThreshold ptyComputeTotalExp(const dds_datUnitWork_t *, s32);

extern void ptyRecomputeMaxHpMp(dds_datUnitWork_t *);

extern void ptyRebuildProfileSkills(s32, dds_datUnitWork_t *);

extern void evtCopyRosterTableValue(dds_datUnitWork_t *);

extern s32 D_0032ACA8[128];

/* Restore or initialise the first free roster slot and publish its model flags. */
PtyAddUnitResult ptyAddUnit(DatUnitId unitId);

extern s32 D_0032ACA8[128];

PtyRemoveUnitResult ptyRemoveUnit(DatUnitId unitId);

u32 ptyReturnZero(void);

u32 ptyReturnOneForCommand193(void);

u32 ptyReturnOne(void);

extern DatSkillList *D_003BAA58;

/* Test whether a party record satisfies one participant requirement of an affinity. */
PtyAffinityMatchResult ptyUnitMatchesAffinityRequirement(
    const dds_datUnitWork_t *unit, BtlCommandId commandId, u8 requirementIndex);

/* Try the six stored orders; success requires exactly the non-sentinel requirement count. */
PtyAffinityMatchResult ptyMatchAffinityPermutation(
    const dds_datUnitWork_t *const *actors, BtlCommandId commandId);

void evtCopyRosterTableValue(dds_datUnitWork_t *entry);

extern u16 ptyRosterDefaultSkillIds[16][4];

extern void scrClearAllSecondaryScriptFlags(dds_datUnitWork_t *unit);

extern s32 scrSetFlag(dds_datUnitWork_t *unit, u16 skill);

extern void scrSetSecondaryScriptFlag(dds_datUnitWork_t *unit, u16 skill);

void ptyMergeStockSkills(dds_datUnitWork_t *unit);

void evtRandomizeEntryValue(dds_datUnitWork_t *entry);

/* Clear a subset of per-unit status flags on occupied qualifying entries,
 * gated by the event RNG; report whether any flags were cleared. */
EvtRandomStatusClearResult evtClearRandomStatusFlags(void);

/* Step the script with these context values; clear the written flag, not the stored result. */
ScrVmInteger evtRunContext(ScrProcedureIndex procedureIndex, s32 firstArgument, s32 secondArgument, s32 commandIndex, u16 options);

scrProcessWork_s *scrGetWorkTaskHandle(void);

s32 evtPushFirstRosterLevel(void);

s32 evtPushSecondRosterLevel(void);

s32 evtPushFirstRosterCurrentHp(void);

s32 evtPushSecondRosterCurrentHp(void);

s32 evtPushFirstRosterMaximumHp(void);

s32 evtPushSecondRosterMaximumHp(void);

/* Push the first entry's selected stat; selectors -1, 16 and 17 use the default. */
s32 evtPushFirstRosterSelectedStat(void);

/* Push the second entry's selected stat; selectors -1, 16 and 17 use the default. */
s32 evtPushSecondRosterSelectedStat(void);

/* Push the first entry's option-selected stat, retaining the same default selectors. */
s32 evtPushFirstRosterOptionStat(void);

/* Push the second entry's option-selected stat, retaining the same default selectors. */
s32 evtPushSecondRosterOptionStat(void);

/* Push the first entry's profile-adjusted stat, including its native status override. */
s32 evtPushFirstRosterStatEligibility(void);

/* Push the second entry's profile-adjusted stat, including its native status override. */
s32 evtPushSecondRosterStatEligibility(void);

/* Kind 5 selects the first entry's roster detail instead of the command-table stat. */
s32 evtPushSelectedStatOrRosterLowValue(void);

/* Kind 5 scales the command stat by the first entry's roster multiplier, then truncates. */
s32 evtPushSelectedScaledStat(void);

s32 evtPushSelectedStatGrade(void);

/* Options 1 and 2 select alternate command halfwords; other options push zero. */
s32 evtSelectScriptStatValue(void);

/* Select the alternate stat through the first entry's table value, not the context selector. */
s32 evtPushEntryIndexedStatOption(void);

/* Kind 5 selects the first entry's high roster detail instead of the command total. */
s32 evtPushSelectedTotalOrRosterHighValue(void);

s32 evtPushSelectedStatMaximum(void);

/* Store the script argument as the context result and mark it written. */
s32 evtStoreScriptParameterResult(void);

s32 evtPushScriptContextResult(void);

s32 evtPushEntryFlagBitInverted(void);

/* For positive ranges, return 1.0 plus a percentage jitter; the upper endpoint is excluded. */
s32 evtRollRandomScale(void);

/* Without a battle runtime, push zero without reading the group-choice argument. */
s32 scrGetBattleAverageCurrentValueForGroup(void);

/* Without a battle runtime, push zero without reading the group-choice argument. */
s32 scrGetBattleAverageMaximumValueForGroup(void);

/* Without a battle runtime, push zero without reading the group-choice argument. */
s32 scrGetBattleAverageActorStatForGroup(void);

/* Push the selected actor-group average for the requested attribute. */
s32 evtPushAvailableChoiceRatio(void);

/* Push the native battle float when a runtime exists, otherwise zero. */
s32 evtPushAvailableFloatValue(void);

/* Push the native battle integer when a runtime exists, otherwise zero. */
s32 evtPushAvailableIntegerValue(void);

s32 evtPushLevelMaxHpGrowth(void);

s32 evtPushLevelMaxMpGrowth(void);

extern u32 datComputeSkillBoostedMaxHp(dds_datUnitWork_t *);

/* Push the coarse HP-percentage table value; only exactly 100 percent uses index zero. */
s32 evtSelectStatGrade(void);

s32 evtPushLevelValueA(void);

s32 evtPushLevelValueB(void);

s32 evtPushLevelValueCCommand18D(void);

s32 evtPushLevelValueCCommand18E(void);

s32 evtScriptSelectRandomValue(void);

s32 evtPushRosterBaseValue(void);

/* Push the finer HP-percentage table value; only exactly 100 percent uses index zero. */
s32 evtSelectFineStatGrade(void);

u8 evtTestSolarPhaseOrModelFlag(DatBattleAdjustmentCondition condition);

u8 btlSelectConditionalEncounterGroup(s32 encounterIndex);

enum {
    BTL_LEVEL_GATE_PARTY_FLAG_ELIGIBLE = 4
};

/* Compare truncated averages: enemy average + party average / 4 >= party average.
 * Requires a nonzero scene, a nonempty enemy list, and party entries carrying
 * occupied, level-gate-eligible and frontline flags. No eligible entries returns zero.
 * count and remaining serve both loops; word holds enemy IDs, then entry flags.
 * partyLevel is accumulated first and divided in place before comparison. */
BtlScenePartyLevelThresholdResult btlCheckScenePartyLevelThreshold(u32 sceneIndex);

/* Reuse an active interval, or seed one for a nonempty encounter group. */
BtlConditionalEncounterGaugeThreshold btlGetConditionalEncounterInterval(s32 encounterIndex);

extern s32 btlSelectedConditionalEncounterIndex;

extern s32 btlSelectedConditionalEncounterResult;

extern void dds3WorkClear(void);

BtlConditionalEncounterRollResult btlAdvanceConditionalEncounterRoll(f32 step, s32 encounterIndex);

void dds3WorkClear(void);

void func_0011CE30(void);

void btlPreemptiveChanceHook(void);

/* The serial-battle caller supplies its clamped chance; retail leaves this hook empty. */
void btlSerialBattleChanceHook(s32 chance);

/* The surprise-probability diagnostic hook receives the final chance. */
void btlSurpriseChanceHook(s32 chance);

s32 evtPushRequestedRosterPresence(void);
/* End of declarations from src/dds1/newdata/datCalc.c */

INCLUDE_ASM(const s32, "newdata/datCalc", datAddMoney);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_001215F8);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00124B70);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00124BC8);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00124C38);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00124D80);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00124DF0);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00124E18);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00124E70);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00121780);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_001217A8);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_001217C0);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00121830);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_001218A8);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_001218C0);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00125048);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00125080);

INCLUDE_ASM(const s32, "newdata/datCalc", func_001250B8);

INCLUDE_ASM(const s32, "newdata/datCalc", func_001253F0);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00121B68);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00125498);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00125568);

INCLUDE_ASM(const s32, "newdata/datCalc", func_001255A8);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00125620);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00125690);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00125730);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00121C10);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00121C18);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00121C20);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00121C28);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00121C30);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00121C38);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00121C40);

/* Raise the packed value to `minimumValue` (never lower it); reaching 0x40, 0x80 or 0x400 re-randomizes the entry. */
void sdfRaisePackedChannelValue(dds_datUnitWork_t *item, u32 minimumValue) {
    s32 channel;

    if ((item->status & SDF_PACKED_CHANNEL_MASK) < minimumValue) {
        item->status = (item->status & SDF_PACKED_STATUS_BIT) | (minimumValue & SDF_PACKED_CHANNEL_MASK);
        channel = item->status & SDF_PACKED_CHANNEL_MASK;
        switch (channel) {
        case 0x40:
        case 0x80:
        case 0x400:
            evtRandomizeEntryValue(item);
            break;
        }
    }
}

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00121CD8);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00121CE0);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00121CE8);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00121D10);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00121D38);

/* Only an exact masked status of 0x1000 forces one; combinations of low
 * status bits do not take this path. Otherwise use the profile-adjusted stat. */
DatCalculatedStatValue datGetStatWithStatusOverride(
    const dds_datUnitWork_t *unit, DatBaseStatIndex statIndex) {
    if ((unit->status & DAT_STATUS_VALUE_MASK) == DAT_STATUS_STAT_OVERRIDE) {
        return 1;
    }
    return datGetClampedProfileAdjustedStat(unit, statIndex);
}

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00121D78);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00121E20);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00121EF8);

u32 datGetAisyoValue(datUnitWork_t *work, s32 attr)
{
    return (u16)datGetAisyo(work, attr);
}

u32 datGetAisyoFlag(datUnitWork_t *work, s32 attr)
{
    return datGetAisyo(work, attr) & 0xFFFF0000;
}

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00121F48);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00122078);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00122098);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_001220D8);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00125C68);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_001220E8);

void func_r1_001220F8(void) {
}

void func_r1_00122100(void) {
}

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00122108);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00122110);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00122118);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00122130);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00122148);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00122150);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00122158);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00122178);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00122180);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00122188);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00122190);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00122198);

void func_r1_001221A0(void) {
}

void func_r1_001221A8(void) {
}

void func_r1_001221B0(void) {
}

void func_r1_001221B8(void) {
}

void func_r1_001221C0(void) {
}

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_001221C8);

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00122200);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00125DA8);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00125EB0);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00125F68);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00125F88);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00125F98);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00125FF8);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00126018);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00126170);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00126190);

INCLUDE_ASM(const s32, "newdata/datCalc", func_001261D8);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00126228);

INCLUDE_ASM(const s32, "newdata/datCalc", func_001262D0);

INCLUDE_ASM(const s32, "newdata/datCalc", func_001262E0);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00126310);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00126330);

INCLUDE_ASM(const s32, "newdata/datCalc", func_001263D0);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00126430);

u8 evtTestSolarPhaseOrModelFlag(DatBattleAdjustmentCondition condition) {
    u32 conditionType = condition >> 16;
    switch (conditionType) {
    case DAT_BATTLE_ADJUSTMENT_CONDITION_NONE:
        break;
    case DAT_BATTLE_ADJUSTMENT_CONDITION_SOLAR_PHASE_MASK:
        if (condition & (1 << evtGetMirroredSolarPhase()) & 0xFFFF) {
            return 1;
        }
        break;
    case DAT_BATTLE_ADJUSTMENT_CONDITION_MODEL_FLAG:
        if (evtBitCheck(condition & 0xFFFF)) {
            return 1;
        }
        break;
    }
    return 0;
}

u8 btlSelectConditionalEncounterGroup(s32 encounterIndex) {
    s8 enabled[3];
    s32 conditionIndex;
    u8 code;

    for (conditionIndex = 0; conditionIndex < 3; conditionIndex++) {
        enabled[conditionIndex] = evtTestSolarPhaseOrModelFlag(datBattleAdjustmentRecords[encounterIndex].conditions[conditionIndex]);
    }
    code = datBattleAdjustmentRecords[encounterIndex].variantCodes[0];
    if (enabled[0] && enabled[1] && enabled[2]) {
        if (code != 8) {
            return code;
        }
    }
    code = datBattleAdjustmentRecords[encounterIndex].variantCodes[1];
    if (enabled[0] && enabled[1]) {
        if (code != 8) {
            return code;
        }
    }
    code = datBattleAdjustmentRecords[encounterIndex].variantCodes[2];
    if (enabled[0] && enabled[2]) {
        if (code != 8) {
            return code;
        }
    }
    code = datBattleAdjustmentRecords[encounterIndex].variantCodes[3];
    if (enabled[1] && enabled[2]) {
        if (code != 8) {
            return code;
        }
    }
    code = datBattleAdjustmentRecords[encounterIndex].variantCodes[4];
    if (enabled[0]) {
        if (code != 8) {
            return code;
        }
    }
    code = datBattleAdjustmentRecords[encounterIndex].variantCodes[5];
    if (enabled[1]) {
        if (code != 8) {
            return code;
        }
    }
    code = datBattleAdjustmentRecords[encounterIndex].variantCodes[6];
    if (enabled[2]) {
        if (code != 8) {
            return code;
        }
    }
    code = datBattleAdjustmentRecords[encounterIndex].variantCodes[7];
    return code == 8 ? 0 : code;
}

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_00122498);

/* Reuse an active interval, or seed one for a nonempty encounter group. */
BtlConditionalEncounterGaugeThreshold btlGetConditionalEncounterInterval(s32 encounterIndex) {
    s32 variant;
    s32 total;
    s32 i;
    s32 interval;
    if (encounterIndex == 0) {
        return BTL_CONDITIONAL_ENCOUNTER_GAUGE_THRESHOLD_UNAVAILABLE;
    }
    variant = btlSelectConditionalEncounterGroup(encounterIndex);
    total = 0;
    for (i = 0; i < 20; i++) {
        total += datBattleAdjustmentRecords[encounterIndex].groups[variant].entries[i].weight;
    }
    if (total == 0) {
        return BTL_CONDITIONAL_ENCOUNTER_GAUGE_THRESHOLD_UNAVAILABLE;
    }
    if (btlConditionalEncounterCachedInterval > 0) {
        return btlConditionalEncounterCachedInterval;
    }
    interval = datBattleAdjustmentRecords[encounterIndex].groups[variant].interval;
    if (interval <= 0) {
        return BTL_CONDITIONAL_ENCOUNTER_GAUGE_THRESHOLD_UNAVAILABLE;
    }
    btlConditionalEncounterCachedInterval = dds3GetRandIntA(0, interval * 2 - 32) + 16;
    return btlConditionalEncounterCachedInterval;
}

INCLUDE_ASM(const s32, "newdata/datCalc", func_r1_001225D8);

INCLUDE_RODATA(const s32, "newdata/datCalc", D_004A9188);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_r1_0046B584);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_r1_0046B588);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_r1_0046B58C);

