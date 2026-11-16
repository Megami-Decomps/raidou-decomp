#ifndef SBTLENCOUNT_H
#define SBTLENCOUNT_H

#include "common.h"
#include "sdf_types.h"

struct SBTLUNIT_WORK;

typedef struct datRndEncRitu_t {
    u16 encountid;
    u16 ritu;
    s16 field04; /* HD has s8 map, s8 floor here; Raidou reads one halfword into sbtlData102 */
    s16 type;
} datRndEncRitu_t;

typedef struct datRndEncTbl_t {
    u8 map;
    u8 floor;
    u16 flag0;
    u16 flag1;
    u16 flag0j;
    u16 flag1j;
    u16 pad0A;
    s32 normalritu;
    datRndEncRitu_t ritu[8];
} datRndEncTbl_t;

/* One encounter generator; HD packs zan_cnt with type_d_sommon_flg, the PS2 reads it as a word. */
typedef struct SB_GENERATOR_WORK {
    s32 use;
    s32 zan_cnt;
    s32 wait;
    struct SB_ENCOUNT1 *penc;
} SB_GENERATOR_WORK;

extern SB_GENERATOR_WORK sb_gen_work[13];
extern s32 sb_gen_d_flag;
extern datRndEncTbl_t (*sbtlData101)[3];
extern s32 datRndEncBase;
extern s32 encmax;
/* The encounter sbtlRndEncCalc picked: its datRndEncRitu_t field04 and type. */
extern s32 sbtlData102;
extern s32 sbtlData103;

struct SBTLUNIT_WORK *sbtlCallMemset21(s32 devilId, s32 point, s32 arg, s32 mode);
struct SBTLUNIT_WORK *sbtlAddAWXAtkPadSub(s32 devilId, s32 point, s32 arg);
struct SBTLUNIT_WORK *sbtlSetEncountData(s32 devil, s32 posid, s32 groupID);
s32 sbtlCheckGeneratorGroupID(s32 group);
s32 sbtlGetRndEncJoken(s32 id);
s32 sbtlGetRndEncMax(s32 id);
s32 sbtlRndEncCalc(s32 id, f32 length);
s32 sbtlGetLevelAverage(s32 encountId);

struct SBTL_AISYO_TBL *sbtlGetAisyotbl(s32 id);
s32 sbtlGetPointData(s32 pointID, sdfVector4_t *pos, sdfVector4_t *rot);
void sbtlSetactcounter(struct SBTLUNIT_WORK *wk);

#endif
