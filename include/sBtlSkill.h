#ifndef SBTLSKILL_H
#define SBTLSKILL_H

#include "common.h"
#include "enums.h"

struct SBTLSKILLP_TBL;
struct SBTLSKLVISUAL_TBL;
struct SBTLUNIT_WORK;

s32 sbtlCallBtlCreateSideList4(struct SBTLUNIT_WORK *wk, s32 arg, s32 idx);
u8 sbtlCheckCost_SkillID(struct SBTLUNIT_WORK *wk, SKLID sklid, s32 arg);
s32 CheckBadStatus_sbtlSkill(struct SBTLUNIT_WORK *wk, SKLID sklid);
s32 sbtlCallBtlGetSklVisualTbl2(struct SBTLUNIT_WORK *wk, SKLID sklid);
s32 sbtlCheckMHTData(struct SBTLUNIT_WORK *wk, SKLID sklid);
s32 sbtlCheckUseSkill(struct SBTLUNIT_WORK *wk, SKLID sklid);
struct SBTLSKILLP_TBL *sbtlGetSkilltbl(SKLID skillid);
void sbtlDecVice(struct SBTLUNIT_WORK *wk, s32 vice);
#ifdef VERSION_RAIDOU1 /* Raidou 2's sbtlRefSbtl64 is other code: the highest unit of the given sides */
void sbtlDecMag(s32 mag);
#else
f32 sbtlRefSbtl64(SBTLUNIT_SIDE side);
#endif
void sbtlDecHP(struct SBTLUNIT_WORK *wk, s32 hp);
void sbtlDecMP(struct SBTLUNIT_WORK *wk, s32 mp);
struct SBTLSKLVISUAL_TBL *sbtlGetSklVisualTbl(SKLID sklid);
s32 sbtlSetSkillData(struct SBTLUNIT_WORK *wk, SKLID sklid);
s32 sbtlSetHatData(struct SBTLUNIT_WORK *wk, SKLID sklid);
s32 sbtlSetSkill_MotionData(struct SBTLUNIT_WORK *wk, SKLID sklid);

/* Skill visual table entry (16 bytes in R1). Partial layout. */
typedef struct SBTLSKLVISUAL_TBL {
    s32 motionno;
    s32 motionno2;
    s16 hatEffect;
    s16 SABno;
    u32 flg;
} SBTLSKLVISUAL_TBL;

/* Skill parameter table entry (0x34 bytes in R1). */
struct SBTLSKILLP_TBL {
    u16 s_type;
    s16 para;
    u32 attr;
    s16 attrtime;
    u16 transfer;
    u16 type;
    u16 area;
    u8 useflg;
    s8 count;
    u32 flg;
    s16 n0;
    f32 n1;
    s16 max_damage;
    s16 muteki_time;
    u32 badst;
    s16 percentage;
    s16 para2;
    u16 equipID;
    s16 hitbk_cnt;
    f32 hitbk_rate;
#ifdef VERSION_RAIDOU2
    u32 flg2;
    s32 weapontype; /* HD name; index into sbtlZokuseiDmgData */
    u8 pad3C[0x4];
#endif
};
#ifdef VERSION_RAIDOU1
SIZE_CHECK(sbtlSkillpTblSizeCheck, struct SBTLSKILLP_TBL, 0x34);
#else
SIZE_CHECK(sbtlSkillpTblSizeCheck, struct SBTLSKILLP_TBL, 0x40);
#endif

extern struct SBTLSKILLP_TBL *sbtlCallBtlDrawRedGgData;
extern SBTLSKLVISUAL_TBL *sbtlGetSklVisualTblData;

#endif
