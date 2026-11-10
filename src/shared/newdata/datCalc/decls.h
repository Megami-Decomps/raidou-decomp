/* Declarations of smt3-decomp src/smt3/newdata/datCalc.c, for its bodies in this directory. */
#ifndef SHARED_NEWDATA_DATCALC_H
#define SHARED_NEWDATA_DATCALC_H

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
extern datPartyWork_t *datPartyWork FAKEMATCH_ASM_NAME("DDS3_GBWK");

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

#endif
