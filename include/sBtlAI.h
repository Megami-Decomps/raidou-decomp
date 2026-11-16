#ifndef SBTLAI_H
#define SBTLAI_H

#include "common.h"
#include "enums.h"

struct SBTLAI_WORK;
struct SBTLUNIT_WORK;

s32 CheckAIScript(struct SBTLUNIT_WORK *wk);
s32 Proc_sbtlAI(struct SBTLUNIT_WORK *wk);
s32 SEquationno_00(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_05(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_07(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_08(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_09(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_0a(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_0b(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_0c(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_0d(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_0e(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_10(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_11(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_12(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_14(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_15(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_16(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 SEquationno_17(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 TEquationno_0c(struct SBTLUNIT_WORK *wk);
s32 TEquationno_0d(struct SBTLUNIT_WORK *wk);
s32 TEquationno_0e(struct SBTLUNIT_WORK *wk);
s32 sbtlAI_SetSeikaku_Action(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckSkillEquation(struct SBTLUNIT_WORK *wk, s16 adr, SKLID skillid);
s32 sbtlCheckTargetEquation(struct SBTLUNIT_WORK *wk);
s32 sbtlGetTargetUnit(struct SBTLUNIT_WORK *wk);
struct SBTLAI_WORK *sbtlLeaf64(void);
struct SBTLAI_WORK *Create_sbtlAI(void);
void sbtlRefSbtl17(struct SBTLUNIT_WORK *wk);
s32 sbtlProcScript(struct SBTLUNIT_WORK *wk);

#endif
