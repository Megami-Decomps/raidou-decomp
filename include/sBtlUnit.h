#ifndef SBTLUNIT_H
#define SBTLUNIT_H

#include "common.h"
#include "enums.h"
#include "sdf_types.h"

struct SBTLUNIT_LISTWORK;
struct SBTLUNIT_WORK;

s32 CheckLiveTarget(struct SBTLUNIT_WORK *wk);
s32 CreateSTP(struct SBTLUNIT_WORK *wk, s32 id, s32 bframe, s32 aframe, s8 flg);
s32 Draw_sbtlUnit(void);
s32 Exit_sbtlUnit(void);
s32 Init_sbtlUnit(s8 flg);
s32 ResetAction(struct SBTLUNIT_WORK *wk, s8 flg);
s32 UnitMV(struct SBTLUNIT_WORK *wk);
void sbtlCallBtlCheckDamageTarget(struct SBTLUNIT_WORK *wk);
s32 sbtlChangeModelColor(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckExist(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckExist2(struct SBTLUNIT_WORK *wk);
void sbtlCheckGuardMotion(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckModelDataLoadEnd(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckMotionEnd(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckMotionNo(struct SBTLUNIT_WORK *wk, SBTLUNIT_MOTION no);
s32 sbtlCheckTateSkillSelectTarget(struct SBTLUNIT_WORK *wk);
s32 sbtlClearUnitList(struct SBTLUNIT_LISTWORK *wk);
struct SBTLUNIT_LISTWORK *sbtlCreateSideList(SBTLUNIT_SIDE side, s32 flg);
struct SBTLUNIT_LISTWORK *sbtlCreateTargetList(SBTLUNIT_SIDE side);
struct SBTLUNIT_LISTWORK *sbtlCreateTargetList_AI(SBTLUNIT_SIDE side);
struct SBTLUNIT_LISTWORK *sbtlCreateTargetList_Group(SBTLUNIT_SIDE side, s32 GroupID);
struct SBTLUNIT_LISTWORK *sbtlCreateTargetList_PForward(SBTLUNIT_SIDE side);
struct SBTLUNIT_LISTWORK *sbtlCreateUnitList(s32 number);
s32 sbtlDeleteUnit(struct SBTLUNIT_WORK *wk);
s32 sbtlEntryUnitList(struct SBTLUNIT_LISTWORK *lwk, struct SBTLUNIT_WORK *wk);
s32 sbtlGetLight_SetPos(struct SBTLUNIT_WORK *wk, s8 lightno);
s32 sbtlGetNumUnit_GroupID(struct SBTLUNIT_LISTWORK *lwk, s32 serialID);
s32 sbtlGetNumUnit_GroupID2(struct SBTLUNIT_LISTWORK *lwk, s32 serialID, s32 flg);
s32 sbtlRefSbtl33(struct SBTLUNIT_WORK *wk);
s32 sbtlReleaseUnitList_ID(struct SBTLUNIT_LISTWORK *lwk, s32 id);
s32 sbtlResetSummon_E(struct SBTLUNIT_WORK *wk);
struct SBTLUNIT_WORK *sbtlSearchUnit_GroupID(struct SBTLUNIT_LISTWORK *lwk, s32 serialID);
struct SBTLUNIT_WORK *sbtlSearchUnit_ID(struct SBTLUNIT_LISTWORK *lwk, s32 ID);
s32 sbtlSearchUnit_Nakama(struct SBTLUNIT_WORK *wk);
s32 sbtlSetAIMode(struct SBTLUNIT_WORK *wk);
void sbtlSetEndBadStatus(void);
s32 sbtlSetEndData(void);
s32 sbtlSetFade(struct SBTLUNIT_WORK *wk, SBTLUNIT_VST flg, s32 start, s32 count, s32 para);
s32 sbtlSetLight_SetPos(struct SBTLUNIT_WORK *wk, s8 lightno);
s32 sbtlSetUnitLight(struct SBTLUNIT_WORK *wk, s32, u32 diffuse, u32 ambient, s32 flg, s32 flg2);
s32 sbtlSetUnitLight_Dir(struct SBTLUNIT_WORK *wk, const sdfVector4_t *dir, s32 flg);

#endif
