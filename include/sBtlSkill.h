#ifndef SBTLSKILL_H
#define SBTLSKILL_H

#include "common.h"
#include "enums.h"

struct SBTLSKILLP_TBL;
struct SBTLSKLVISUAL_TBL;
struct SBTLUNIT_WORK;

s32 sbtlCallBtlCreateSideList4(struct SBTLUNIT_WORK *wk, SKLID sklid, s32 r_kaja);
s32 sbtlCallBtlGetSkilltbl22(struct SBTLUNIT_WORK *wk, SKLID sklid, s32 arg);
s32 CheckBadStatus_sbtlSkill(struct SBTLUNIT_WORK *wk, SKLID sklid);
s32 sbtlCallBtlGetSklVisualTbl2(struct SBTLUNIT_WORK *wk, SKLID sklid);
s32 sbtlCheckMHTData(struct SBTLUNIT_WORK *wk, SKLID sklid);
struct SBTLSKILLP_TBL *sbtlGetSkilltbl(SKLID skillid);
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
};
SIZE_CHECK(sbtlSkillTblSizeCheck, struct SBTLSKILLP_TBL, 0x34);

extern struct SBTLSKILLP_TBL *sbtlCallBtlDrawRedGgData;
extern SBTLSKLVISUAL_TBL *sbtlGetSklVisualTblData;

#endif
