#ifndef SBTLCALC_H
#define SBTLCALC_H

#include "common.h"
#include "enums.h"

struct SBTLCALC_DMGWORK;
struct SBTLDMG_HITWORK;
struct SBTLSKILLP_TBL;
struct SBTLUNIT_WORK;
struct sdatUnitWork_t;
struct SBTL_AISYO_TBL;

s32 CalcAisyo(struct SBTLUNIT_WORK *wk);
s32 sbtlCallBtlGetSkilltbl19(struct SBTLUNIT_WORK *wk, SKLID id);
SKLID sbtlGetAutoSkill_ID(struct sdatUnitWork_t *unit, s32 id);
s32 sbtlCheckBadStatus(struct SBTLUNIT_WORK *wk);
void sbtlCheckBadStatus_Weak(struct SBTLUNIT_WORK *wk, SKLID sklid, struct SBTLSKILLP_TBL *ptbl, s32 acterID);
void sbtlCheckBattlePara(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckCost(struct SBTLUNIT_WORK *wk, s32 flg);
s32 sbtlGetMagCost(struct sdatUnitWork_t *player, struct sdatUnitWork_t *unit, s32 devilid);
s32 sbtlCheckMagCost_GB(struct sdatUnitWork_t *player, struct sdatUnitWork_t *unit, s32 devilid, s8 flg);
s32 sbtlCheckMagCost(struct sdatUnitWork_t *unit, s32 devilid, s8 flg);
BST sbtlGetBadStatus(struct sdatUnitWork_t *unit);
/* HP (isMp 0) or MP gain to add to unit, cut so it stops at the maximum. */
s32 LimitRange(struct sdatUnitWork_t *unit, s32 value, s32 isMp);
s32 sbtlCallLimitRange(struct SBTLUNIT_WORK *wk, s32 value);
s32 sbtlCheckMoon(u16 moon);
s32 sbtlResetAutoSkill(struct sdatUnitWork_t *unit);
s32 sbtlSetAutoSkill_GBID(struct sdatUnitWork_t *unit, s32 id);
s32 sbtlCheckAutoSkill_ActerDamage_ID(SKLID sklid);
s32 sbtlCheckAutoSkill_TargetDamage_ID(SKLID sklid);
s32 sbtlCheckAutoSkill_ActerDamage(s32 autono);
s32 sbtlCheckAutoSkill_TargetDamage(s32 autono);
s32 sbtlCheckAutoSkill_Action(struct SBTLUNIT_WORK *wk, struct SBTLDMG_HITWORK *hwk, s32 attr);
s32 sbtlCheckKuishibari(struct SBTLUNIT_WORK *wk, s32 damage);
s32 sbtlGetAutoSkillPara(SKLID id, s16 *badst, s16 *power);
s32 sbtlBtlCheckBadStatusWeakSub(SKLID skill);
s32 sbtlCallBtlGetSkilltbl17(s32 *resist, s32 luck, struct sdatUnitWork_t *unit, s32 tgtLuck, SKLID skill, BST bst, s32 idx);
s32 CalcBtlMagicStatus(struct SBTLUNIT_WORK *wk);
s32 sbtlCallSetClutColor(struct SBTLUNIT_WORK *wk, BST bst);
s32 sbtlGetDamage_CHUUSEI(struct sdatUnitWork_t *unit, struct SBTLCALC_DMGWORK *wk, struct SBTLSKILLP_TBL *ptbl);
s32 sbtlGetDamage_P_LUCK(struct sdatUnitWork_t *unit, struct SBTLCALC_DMGWORK *wk, struct SBTLSKILLP_TBL *ptbl, s32 itemid);
s32 sbtlGetDamage_P_STR(struct sdatUnitWork_t *unit, struct SBTLCALC_DMGWORK *wk, struct SBTLSKILLP_TBL *ptbl, s32 itemid);
s32 sbtlGetDamage_P_TEC(struct sdatUnitWork_t *unit, struct SBTLCALC_DMGWORK *wk, struct SBTLSKILLP_TBL *ptbl, s32 itemid);
s32 sbtlGetDamage_P_VIT(struct sdatUnitWork_t *unit, struct SBTLCALC_DMGWORK *wk, struct SBTLSKILLP_TBL *ptbl, s32 itemid);
s32 sbtlKudaHuuinCheck(void);
s32 sbtlRefSbtl63(SKLID sklid);
s32 sbtlCheckAisyo_Old(u32 attr);
u32 sbtlSEquationno03Sub(struct SBTL_AISYO_TBL *aisyo, u32 attr);
s32 sbtlResetBadStatus(struct SBTLUNIT_WORK *wk, BSTF flg);
void sbtlResetItemFlg(struct sdatUnitWork_t *unit);
s32 sbtlCallBtlGetSkilltbl24(struct sdatUnitWork_t *unit, s32 mode, s32 value, SKLID skillid, s8 attr, s16 percent);
s32 sbtlResetBadStatus_GB(struct sdatUnitWork_t *unit, BSTF flg);
s32 sbtlSetBadStatus_No(struct SBTLUNIT_WORK *wk, SKLID sklid, BST badstno, s32 flg, s32 acterID);
s32 sbtlSetItem(struct SBTLUNIT_WORK *wk);
s32 sbtlDeleteHitHelper(struct SBTLUNIT_WORK *wk);

#endif
