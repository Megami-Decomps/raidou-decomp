#ifndef SBTLCALC_H
#define SBTLCALC_H

#include "common.h"
#include "enums.h"

struct SBTLCALC_DMGWORK;
struct SBTLSKILLP_TBL;
struct SBTLUNIT_WORK;
struct sdatUnitWork_t;

void CalcAisyo(struct SBTLUNIT_WORK *wk);
void sbtlCheckBadStatus_Weak(struct SBTLUNIT_WORK *wk, SKLID sklid, struct SBTLSKILLP_TBL *ptbl, s32 acterID);
void sbtlCheckBattlePara(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckCost(struct SBTLUNIT_WORK *wk, s32 flg);
BST sbtlGetBadStatus(struct sdatUnitWork_t *unit);
s32 sbtlGetDamage_CHUUSEI(struct sdatUnitWork_t *unit, struct SBTLCALC_DMGWORK *wk, struct SBTLSKILLP_TBL *ptbl);
s32 sbtlGetDamage_P_LUCK(struct sdatUnitWork_t *unit, struct SBTLCALC_DMGWORK *wk, struct SBTLSKILLP_TBL *ptbl, s32 itemid);
s32 sbtlGetDamage_P_STR(struct sdatUnitWork_t *unit, struct SBTLCALC_DMGWORK *wk, struct SBTLSKILLP_TBL *ptbl, s32 itemid);
s32 sbtlGetDamage_P_TEC(struct sdatUnitWork_t *unit, struct SBTLCALC_DMGWORK *wk, struct SBTLSKILLP_TBL *ptbl, s32 itemid);
s32 sbtlGetDamage_P_VIT(struct sdatUnitWork_t *unit, struct SBTLCALC_DMGWORK *wk, struct SBTLSKILLP_TBL *ptbl, s32 itemid);
void sbtlKudaHuuinCheck(void);
s32 sbtlRefSbtl63(SKLID sklid);
s32 sbtlResetBadStatus(struct SBTLUNIT_WORK *wk, BSTF flg);
s32 sbtlResetBadStatus_GB(struct sdatUnitWork_t *unit, BSTF flg);
s32 sbtlSetBadStatus_No(struct SBTLUNIT_WORK *wk, SKLID sklid, BST badstno, s32 flg, s32 acterID);
s32 sbtlSetItem(s32 index);

#endif
