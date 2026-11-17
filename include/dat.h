#ifndef DAT_H
#define DAT_H

#include "common.h"
#include "kernel.h"

/* One 0x20C-byte encounter adjustment record. Sound selection shares the
 * header with condition/variant data; four complete weighted groups follow. */
typedef struct SBTLADJUSTMENT_ENTRY {
    u16 battleMode;
    u16 weight;
    s8 value;
    u8 serialEncounterChanceAdjustment; /* 0x05: input percentage before world-flag scaling and clamp. */
} SBTLADJUSTMENT_ENTRY;

typedef struct SBTLADJUSTMENT_GROUP {
    s32 interval;
    SBTLADJUSTMENT_ENTRY entries[20];
} SBTLADJUSTMENT_GROUP;

typedef struct SBTLADJUSTMENT_RECORD {
    u16 encounterParamA; /* 0x00: copied to SBTL_STATE.encounterParamA by 001A11F0. */
    u16 encounterParamB; /* 0x02 */
    u16 streamSelection; /* 0x04: default stream selector used by 001F3278. */
    u8 pad06[2];
    u32 conditions[3]; /* Packed kind and solar-phase-mask/model-flag condition. */
    u8 variantCodes[8];
    SBTLADJUSTMENT_GROUP groups[4];
} SBTLADJUSTMENT_RECORD;

typedef char SBTLADJUSTMENT_ENTRYSizeCheck[sizeof(SBTLADJUSTMENT_ENTRY) == 6 ? 1 : -1];

typedef char SBTLADJUSTMENT_GROUPSizeCheck[sizeof(SBTLADJUSTMENT_GROUP) == 0x7C ? 1 : -1];

typedef char SBTLADJUSTMENT_RECORDSizeCheck[sizeof(SBTLADJUSTMENT_RECORD) == 0x20C ? 1 : -1];

extern SBTLADJUSTMENT_RECORD *datBattleAdjustmentRecords;

#include "enums.h"

struct sdatUnitWork_t;
struct SBTL_PLAYER_DATA;

void CallProcEnd_cmpStatus(void);
s32 dds3GlobalWorkMainProc(dds3ProcessID_t *id);
void UserInit(void);
s32 datAddDevil(void);
s32 datMoonAddMoveSub(void);
s32 datGetPlayerExp(s32 level);
struct SBTL_PLAYER_DATA datGetPlayerLevelUpUnit(struct SBTL_PLAYER_DATA *player);
s32 datGetDevilLevelUpNumsSub(s32 devilId, s32 level);
struct sdatUnitWork_t datGetDevilLevelUpUnit(struct sdatUnitWork_t *unit);
void datCallGetPlayerMaxHP(struct sdatUnitWork_t *unit, s32 swordLevel);
void datGlobalWorkDestroy(void);
void dds3GlobalWorkInit(void);
s32 sdatCalc_GetBadStatus(struct sdatUnitWork_t *unit);

extern u32 sdatLvUpDevilGiftTbl;
extern u32 sdatLvUpDevilGiftSize;
extern u32 sdatLvUpDevilParamUp;
extern u32 sdatLvUpDevilParamUpSize;
extern u32 sdatLvUpPlayerParamUp;
extern u32 sdatLvUpPlayerParamUpSize;
extern u32 sdatLvUpPlayerRankUp;
extern u32 sdatLvUpPlayerRankUpSize;

void datLoadLvUpTables(void);
void cmpLoadModelTables(void);
void datCallMemset(void);

void datCallBitCheck(void);
s32 datRefDDS3GBWK3(s32 value, s32 mode);
void NewGame2Pop(void);
void dds3GlobalWorkUserInit2(s32 mode);
void datGlobalWorkDestroySub(void);

s32 datGetStockDevilID(s32 index);
s32 datGetStockDevilNums(void);
void datCreateDevilUnitEx(s32 devilId, s32 skillMode, struct sdatUnitWork_t *unit);
s32 datCallCreateDevilUnitEx(u32 itemId, s32 nums);
s32 datCallGetStockDevilID2(u32 id);
s32 datCheckSameDevil(u32 id);
s32 datAllReleaseScriptSub(void);

s32 datCheckJidouSkillPrioSub(SKLID skill);
s32 CheckJidouSkillPrio(struct sdatUnitWork_t *unit, SKLID skill);
s32 datCheckSkillEnable(struct sdatUnitWork_t *unit, SKLID skill);

#endif
