#ifndef SBTLSKILL_H
#define SBTLSKILL_H

#include "common.h"
#include "enums.h"

struct SBTLSKILLP_TBL;
struct SBTLSKLVISUAL_TBL;
struct SBTLUNIT_WORK;

s32 sbtlCallBtlCreateSideList4(struct SBTLUNIT_WORK *wk, SKLID sklid, s32 r_kaja);
s32 sbtlCheckMHTData(struct SBTLUNIT_WORK *wk, SKLID sklid);
struct SBTLSKILLP_TBL *sbtlGetSkilltbl(SKLID skillid);
struct SBTLSKLVISUAL_TBL *sbtlGetSklVisualTbl(SKLID sklid);
s32 sbtlSetSkillData(struct SBTLUNIT_WORK *wk, SKLID sklid);
s32 sbtlSetSkill_MotionData(struct SBTLUNIT_WORK *wk, SKLID sklid);

#endif
