#ifndef SBTLENCOUNT_H
#define SBTLENCOUNT_H

#include "common.h"
#include "sdf_types.h"

struct SBTLUNIT_WORK;

typedef struct datRndEncRitu_t {
    u16 encountid;
    u16 ritu;
    s16 field04; /* HD has s8 map, s8 floor here; Raidou reads one halfword into g_rndenc1_map */
    s16 type;
} datRndEncRitu_t;

typedef struct datRndEncTbl_t {
    s16 mapNo[2]; /* HD has u8 map, u8 floor, u16 flag0; Raidou reads two halfwords (pbCallBtlGetRndEncJoken) */
    u16 flag1;
    u16 flag0j;
    u16 flag1j;
    u16 pad0A;
    s32 normalritu;
    datRndEncRitu_t ritu[8];
} datRndEncTbl_t;

/* One encounter generator; HD packs zan_cnt with type_d_sommon_flg, as raidou2 does; raidou1 reads it as a word. */
typedef struct SB_GENERATOR_WORK {
    s32 use;
#ifdef VERSION_RAIDOU2
    s16 zan_cnt;
    s16 type_d_sommon_flg;
#else
    s32 zan_cnt;
#endif
    s32 wait;
    struct SB_ENCOUNT1 *penc;
} SB_GENERATOR_WORK;

extern SB_GENERATOR_WORK sb_gen_work[13];
extern s32 sb_gen_d_flag;
extern datRndEncTbl_t (*sbtlData101)[3];
extern s32 pbCallBtlGetRndEncJokenData;
extern s32 datRndEncBase;
extern s32 encmax;
/* The encounter sbtlRndEncCalc picked: its datRndEncRitu_t field04 and type. */
extern s32 g_rndenc1_map;
extern s32 g_rndenc1_floor;

struct SBTLUNIT_WORK *SetEncountData_Sub_sbtlEncount(s32 devilId, s32 point, s32 arg, s32 mode);
struct SBTLUNIT_WORK *sbtlAddAWXAtkPadSub(s32 devilId, s32 point, s32 arg);
struct SBTLUNIT_WORK *sbtlSetEncountData(s32 devil, s32 posid, s32 groupID);
struct SBTLUNIT_WORK *sbtlAddAWXAtkPadSub2(s32 devilId, s32 point, s32 arg);
s32 sbtlCallBtlClearGeneratorWork(s32 stageNo);
s32 sbtlCheckGeneratorGroupID(s32 group);
s32 sbtlCheckGenerator_D(void);
s32 sbtlGetRndEncJoken(s32 id);
s32 sbtlGetRndEncMax(s32 id);
s32 sbtlRndEncCalc(s32 id, f32 length);
void pbCallBtlGetRndEncJoken(s32 id);
s32 sbtlGetLevelAverage(s32 encountId);

struct SBTL_AISYO_TBL *sbtlGetAisyotbl(s32 id);
s32 sbtlGetPointData(s32 pointID, sdfVector4_t *pos, sdfVector4_t *rot);
void sbtlSetactcounter(struct SBTLUNIT_WORK *wk);

#endif
