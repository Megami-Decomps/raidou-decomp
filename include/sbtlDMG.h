#ifndef SBTLDMG_H
#define SBTLDMG_H

#include "common.h"
#include "enums.h"
#include "sdf_types.h"

struct SBTLDMG_ANIMWORK;
struct SBTLDMG_HITLISTWORK;
struct SBTLDMG_HITWORK;
struct SBTLEFF_WORK;
struct SBTLSKILLHIT_POSWORK;
struct SBTLSKILLHIT_TBL;
struct SBTLSKILLP_TBL;
struct SBTLSKILL_TBL;
struct SBTLUNIT_LISTWORK;
struct SBTLUNIT_WORK;

void UnsetKaraburiSub(struct SBTLDMG_HITLISTWORK *lwk, struct SBTLDMG_HITWORK *hwk);
void sbtlAddSerialID2(void);
void sbtlAnalyzeAisyo(struct SBTLUNIT_WORK *wk, s32 mode, sdfVector4_t *pos);
s32 sbtlAnimListProc(void);
s32 sbtlCHitFunc_ATTACK(struct SBTLUNIT_WORK *wk, struct SBTLEFF_WORK *ewk, struct SBTLDMG_HITWORK *twk, struct SBTLSKILL_TBL *tbl);
s32 sbtlCalcVibe(void);
s32 sbtlCheckALLSkill(struct SBTLDMG_HITWORK *hwk, struct SBTLUNIT_WORK *wklock);
s32 sbtlCheckDamageTarget_Skilltbl(struct SBTLUNIT_WORK *acter, struct SBTLUNIT_WORK *wk, struct SBTLSKILLP_TBL *ptbl, SKLID skillid, s32 flg);
s32 sbtlCheckHitArea(struct SBTLDMG_HITWORK *wk, struct SBTLUNIT_WORK *wklock);
s32 sbtlCheckPlayerDead(void);
s32 sbtlCheckTargetList_csr(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 sbtlClearHitC(struct SBTLDMG_HITLISTWORK *lwk, struct SBTLUNIT_WORK *wk);
s32 sbtlCreateHitC_None(struct SBTLDMG_HITLISTWORK *lwk, struct SBTLUNIT_WORK *wk, struct SBTLUNIT_WORK *acter, struct SBTLDMG_HITWORK *twk, SKLID skillid, struct SBTLSKILLP_TBL *tbl, const sdfVector4_t *pos, s32 serialID2);
struct SBTLDMG_HITLISTWORK *sbtlCreateHitList(void);
struct SBTLUNIT_LISTWORK *sbtlCreateTargetList_Normal(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 sbtlCreateTargetList_csr(struct SBTLUNIT_WORK *wk, SKLID skillid);
void sbtlDeleteAnimList(s32 devilid);
s32 sbtlDeleteHitList(struct SBTLDMG_HITLISTWORK *wk);
s32 sbtlDrawAttachEPL(struct SBTLDMG_HITLISTWORK *lwk);
s32 sbtlGetHit_OnlyEndPos(struct SBTLUNIT_WORK *wk, struct SBTLDMG_HITWORK *twk, struct SBTLEFF_WORK *ewk, struct SBTLSKILLHIT_TBL *hittbl, struct SBTLSKILLHIT_POSWORK *pwk_st, struct SBTLSKILLHIT_POSWORK *pwk, sdfVector4_t *pos, u16 tbltype);
s32 sbtlGetKaraburiNum(struct SBTLDMG_HITLISTWORK *lwk, struct SBTLDMG_HITWORK *hwk);
SBTLUNIT_SIDE sbtlGetSide(struct SBTLUNIT_WORK *wk, u16 area, SBTLUNIT_SIDE acterside, u8 *flg);
s32 sbtlHitCProc(struct SBTLDMG_HITLISTWORK *lwk);
s32 sbtlHitCProc_Idou(struct SBTLDMG_HITLISTWORK *lwk);
s32 sbtlInitLoadAttackHitData_pData(void);
s32 sbtlLoadAttackHitData(struct SBTLUNIT_WORK *wk, s32 id, void *ppData);
s32 sbtlSearchAttackHitData_Copy(struct SBTLUNIT_WORK *wk);
s32 sbtlLoadAttackHitData_Decode(struct SBTLUNIT_WORK *wk);
s32 sbtlLoadAttackHitData_pData(s32 index, void *pData);
void sbtlRegistAnimList(struct SBTLDMG_ANIMWORK *wk, u32 alp);
s32 sbtlReleaseHitC_List(struct SBTLDMG_HITLISTWORK *lwk, u8 flg);
s32 sbtlResetHitPos_XYZ(struct SBTLUNIT_WORK *wk, struct SBTLDMG_HITWORK *twk, struct SBTLEFF_WORK *ewk, struct SBTLSKILLHIT_POSWORK *pwk, sdfVector4_t *pos, sdfVector4_t *rot, u16 tbltype);
void sbtlResetIdouPara(struct SBTLDMG_HITWORK *hwk);
s32 sbtlSearchDMG_Unit(struct SBTLDMG_HITWORK *hwk, struct SBTLUNIT_WORK *wk);
struct SBTLDMG_HITWORK *sbtlSearchHit_SerialID(struct SBTLDMG_HITLISTWORK *lwk, s32 serialID);
s32 sbtlSetApos12(const sdfVector4_t *startpos, const sdfVector4_t *endpos, const sdfVector4_t *rot, struct SBTLSKILLHIT_TBL *hittbl, sdfVector4_t *apos1, sdfVector4_t *apos2);
s32 sbtlSetEntryUnitList_Hit(struct SBTLDMG_HITWORK *hwk, struct SBTLUNIT_WORK *wklock);
s32 sbtlSetHitEndPos(struct SBTLUNIT_WORK *wk, struct SBTLDMG_HITWORK *twk, struct SBTLEFF_WORK *ewk, struct SBTLSKILLHIT_TBL *hittbl, struct SBTLSKILLHIT_POSWORK *pwk_st, struct SBTLSKILLHIT_POSWORK *pwk, sdfVector4_t *pos, u16 tbltype);
s32 sbtlSetHitPos(struct SBTLUNIT_WORK *wk, struct SBTLDMG_HITWORK *twk, struct SBTLEFF_WORK *ewk, struct SBTLSKILLHIT_POSWORK *pwk, sdfVector4_t *pos, sdfVector4_t *rot, sdfVector4_t *rott, u16 tbltype);
s32 sbtlSetHitPos_XYZ(struct SBTLUNIT_WORK *wk, struct SBTLDMG_HITWORK *twk, struct SBTLEFF_WORK *ewk, struct SBTLSKILLHIT_POSWORK *pwk, sdfVector4_t *pos, sdfVector4_t *rot, u16 tbltype);
void sbtlUnsetKaraburi(struct SBTLDMG_HITLISTWORK *lwk, struct SBTLDMG_HITWORK *hwk);

#endif
