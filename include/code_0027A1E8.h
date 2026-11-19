#ifndef CODE_0027A1E8_H
#define CODE_0027A1E8_H

#include "common.h"

struct SBTLUNIT_WORK;

/* Zero-terminated ids that sbtlCancelSystemSub accepts. */
extern s32 sbtlCancelSystemSubData[];

s32 sbtlCancelSystemSub(s32 id);
s32 sbtlEscapeCheck(void);
SKLID sbtlCallBtlGetSkilltbl23(SKLID skillid, s32 *base);
void Delete_sbtlScript(struct SBTLUNIT_WORK *wk);
void sbtlResetBtlMagicStatus(struct SBTLUNIT_WORK *wk);
s32 IsEnemyZenmetsu(void);
s32 sbtlCheckMove_BadStatus(struct SBTLUNIT_WORK *wk);
void sbtlProc_YoyakuEndChk(struct SBTLUNIT_WORK *wk);
s32 sbtlUnit_CheckSummon_LoadExecOk_sub(void);
void Init_sbtlScript(struct SBTLUNIT_WORK *wk);

s32 sbtlCallBtlGetNakama3(void);
s32 ShugouKaijyoSub(struct SBTLUNIT_WORK *wk);
s32 sbtlDrawEscapeGauge(void);
void sbtlResetATKEffect(void);
s32 sbtlCheckSetHITNONE(s32 Group, u32 DevilID);
void sbtlCallBtlResetJump(struct SBTLUNIT_WORK *wk, s32 frame);
void sbtlShugouKaijyo(void);
#endif
