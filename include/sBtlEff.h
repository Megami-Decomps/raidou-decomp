#ifndef SBTLEFF_H
#define SBTLEFF_H

#include "common.h"
#include "enums.h"

struct SBTLEFF_DELLISTWORK;
struct SBTLEFF_LISTWORK;
struct SBTLEFF_WORK;
struct SBTLUNIT_WORK;

s32 EffFunc_DEADDAMAGE(struct SBTLEFF_WORK *wk);
s32 EffFunc_Hanyou(struct SBTLEFF_WORK *wk);
s32 EffFunc_Hanyou_BED(struct SBTLEFF_WORK *wk);
s32 EffFunc_NMLDAMAGE(struct SBTLEFF_WORK *wk);
s32 EffFunc_Slip_G(struct SBTLEFF_WORK *wk);
f32 GetDataScale(struct SBTLUNIT_WORK *wk, SKLID sklid);
void LoadOnMemoryData_BED(struct SBTLEFF_WORK *wk, struct SBTLUNIT_WORK *wklock, u16 no);
s32 SetEffectPos(struct SBTLEFF_WORK *ewk, struct SBTLUNIT_WORK *wk);
struct SBTLEFF_LISTWORK *sbtlCallColorSet5(void);
s32 sbtlCheckDelWork_Request(struct SBTLEFF_DELLISTWORK *lwk, s32 GroupID);
s32 sbtlCheckDeleteEffect(struct SBTLEFF_WORK *wk);
struct SBTLEFF_DELLISTWORK *sbtlCreateDelList(void);
s32 sbtlCreateDelWork(struct SBTLEFF_DELLISTWORK *lwk, SKLID skillid, SBTLUNIT_SIDE side, s32 GroupID);
s32 sbtlDelProc(struct SBTLEFF_DELLISTWORK *lwk);
s32 sbtlDeleteDelList(struct SBTLUNIT_WORK *wk);
s32 sbtlDeleteEffList(struct SBTLEFF_LISTWORK *lwk);
s32 sbtlDeleteEffectData_SerialID2(struct SBTLEFF_LISTWORK *lwk, struct SBTLUNIT_WORK *wk);
s32 sbtlDeleteEffectData_SkillID(struct SBTLEFF_LISTWORK *lwk, SKLID skillid);
s32 sbtlDeleteOnData(s32 flg);
s32 sbtlFadeOut_BadStatusEffect(struct SBTLUNIT_WORK *wk, s32 alpha);
s32 sbtlForceDelete_EffectWork(s8 flg);
s32 sbtlForceDelete_ID(struct SBTLEFF_LISTWORK *lwk, s32 eListID);
s32 sbtlForceDelete_ID_Acter(struct SBTLEFF_LISTWORK *lwk, struct SBTLUNIT_WORK *wk, EFFID effectid);
s32 sbtlForceDelete_ID_SummonFR(struct SBTLEFF_LISTWORK *lwk, struct SBTLUNIT_WORK *wk);
s32 sbtlForceDelete_SPID(struct SBTLEFF_LISTWORK *lwk, struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 sbtlForceDelete_SerialID(struct SBTLEFF_LISTWORK *lwk, s32 serialID);
s32 sbtlForceDelete_SkillFlag(u32 flg);
s32 sbtlForceResetDecAlpha(struct SBTLEFF_LISTWORK *lwk, struct SBTLUNIT_WORK *wk, s32 alpha);
s32 sbtlGetEffectSerialID2_Now(void);
s32 sbtlGetEffectStat_ID(struct SBTLEFF_LISTWORK *lwk, s32 eListID);
struct SBTLEFF_WORK *sbtlGetEffectWork_ID(struct SBTLEFF_LISTWORK *lwk, s32 eListID);
s32 sbtlInitEffectWork(void);
s32 sbtlLoadEffectData_ID(SKLID id);
s32 sbtlLoadEffectData_IDMask(SKLID id, EFFPAC_ST flg);
s32 sbtlLoadEndCheck_PlayerOnMemData(void);
s32 sbtlLoadPlayerOnMemData(void);
s32 sbtlReleaseEff(struct SBTLEFF_LISTWORK *lwk, s32 flg);
s32 sbtlResetAIPattern(struct SBTLUNIT_WORK *wk, u32 flg);
s32 sbtlSearchDelWork_Side(struct SBTLEFF_DELLISTWORK *lwk, SBTLUNIT_SIDE side);
s32 sbtlSearchEffectData(SKLID skillid);
s32 sbtlSearchEffect_ID(struct SBTLEFF_LISTWORK *lwk, EFFID effectid);
s32 sbtlSearchEffect_SAB_Acter(struct SBTLEFF_LISTWORK *lwk, struct SBTLUNIT_WORK *wk, SKLID BEDsklid, s32 BEDtype);
s32 sbtlSearchEffect_WorkID(struct SBTLEFF_LISTWORK *lwk, s32 workid);
s32 sbtlSetDecAlpha_SerialID(struct SBTLEFF_LISTWORK *lwk, s32 serialID, s32 alpha);
s32 sbtlSetDecAlpha_SerialID2(struct SBTLEFF_LISTWORK *lwk, s32 serialID2, s32 alpha);

#endif
