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

struct tagCMP_STATUS_WORK;
void CallProcEnd_cmpStatus(struct tagCMP_STATUS_WORK *wk);
void datCalcLuckParam(void);
s32 dds3GlobalWorkMainProc(dds3ProcessID_t *id);
void UserInit(void);
s32 datAddDevil(void);
s32 datMoonAddMoveSub(void);
s32 datGetPlayerExp(s32 level);
#ifdef VERSION_RAIDOU2
void datGetPlayerLevelUpUnit(struct SBTL_PLAYER_DATA *pDst, struct SBTL_PLAYER_DATA *pSrc);
#else
struct SBTL_PLAYER_DATA datGetPlayerLevelUpUnit(struct SBTL_PLAYER_DATA *player);
#endif
s32 datSetPlayerUnitSword(struct SBTL_PLAYER_DATA *player, s32 sword);
s32 datGetDevilLevelUpNumsSub(s32 devilId, s32 level);
#ifdef VERSION_RAIDOU2
void datGetDevilLevelUpUnit(struct sdatUnitWork_t *pDst, struct sdatUnitWork_t *pSrc);
#else
struct sdatUnitWork_t datGetDevilLevelUpUnit(struct sdatUnitWork_t *unit);
#endif
void datCallGetPlayerMaxHP(struct sdatUnitWork_t *unit, s32 swordLevel);
void datGlobalWorkDestroy(void);
void dds3GlobalWorkInit(void);
s32 sdatCalc_GetBadStatus(struct sdatUnitWork_t *unit);
s32 datGetTotalCyuusei(struct sdatUnitWork_t *pUnit);
void datAddTotalCyuusei(struct sdatUnitWork_t *pUnit, s32 Val);
s32 datCallCalcGetItemNum2(s32 id, s32 num);
s32 datGetItemNum(s32 id);
void datAddStock(s32 nums);
s32 datCreateDevilUnitExSub(s32 devilId);
void datLeaf(s32 arg0, s32 arg1, s32 arg2);

/* One gift a devil can bring at a level: three per devil id, indexed devilId * 3 (HD sdatLvUpDevilGift_t). */
typedef struct sdatLvUpDevilGift_t {
    u16 ItemID;
    u8 Rate;
    u16 Level;
    u8 NumMax;
} sdatLvUpDevilGift_t;

extern sdatLvUpDevilGift_t *sdatLvUpDevilGiftTbl;
extern u32 sdatLvUpDevilGiftSize;
typedef struct sdatLvUpDevilParamUp_t {
    u8 Param[4];
} sdatLvUpDevilParamUp_t;

extern sdatLvUpDevilParamUp_t *sdatLvUpDevilParamUp;
extern u32 sdatLvUpDevilParamUpSize;
extern u32 sdatLvUpPlayerParamUp;
extern u32 sdatLvUpPlayerParamUpSize;
/* One summoner rank: the loyalty total it needs, the bit set once it is reached, and a bit it requires (0 for none). */
typedef struct sdatLvUpPlayerRankUp_t {
    s32 Cyuusei;
    u32 GetFlag;
    u32 ChkFlag;
} sdatLvUpPlayerRankUp_t;

extern sdatLvUpPlayerRankUp_t *sdatLvUpPlayerRankUp;
extern u32 sdatLvUpPlayerRankUpSize;

/* gohDevilParam.dat and gohSameDevils.dat buffers and sizes, filled by cmpLoadModelTables. */
extern u32 datCallCheckStrongSwordAttackData;
extern u32 cmpLoadModelTablesData;
extern u32 datData6;
extern u32 datCallGetStockDevilIDData;
#ifdef VERSION_RAIDOU2
/* Sword table entry (0x4C bytes); datData20 points at the loaded table. */
typedef struct {
    s32 field0;
    s32 kind; /* row of sbtlData140 */
    u8 pad8[0x44];
} datSword_t;

extern datSword_t *datData20;
extern const char cmpStrGohmadenGohDevilParamDat[];
extern const char cmpStrGohmadenGohSameDevilsDat[];
#endif

void datLoadLvUpTables(void);
void datInitPlayer(void);
void datLoadGohmadenTable(void);
#ifdef VERSION_RAIDOU2
void datRefDDS3GBWK19(void);
s32 datCheckEventItem(s32 id);
#endif
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
#ifdef VERSION_RAIDOU2
s32 datGetStockDevilIndex(u32 id);
s16 datRefDDS3GBWK20(s16 id);
s32 datOmoideSkillHaveSkill(struct sdatUnitWork_t *pWork, SKLID skill);
s32 datRefDDS3GBWK12(s32 sword);
void datCalcNowParam(struct sdatUnitWork_t *unit, s32 sword);
#endif
s32 datCheckSameDevil(u32 id);
s32 datAllReleaseScriptSub(void);

s32 datCheckJidouSkillPrioSub(SKLID skill);
s32 CheckJidouSkillPrio(struct sdatUnitWork_t *unit, SKLID skill);
s32 datCheckSkillEnable(struct sdatUnitWork_t *unit, SKLID skill);

#endif
