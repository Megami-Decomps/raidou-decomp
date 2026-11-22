#ifndef SBTLCALC_H
#define SBTLCALC_H

#include "common.h"
#include "enums.h"

struct SBTLDMG_HITWORK;
struct SBTLSKILLP_TBL;
struct SBTLSKL_KAJA_WORK;
struct SBTLUNIT_WORK;
struct sdatUnitWork_t;
struct SBTL_AISYO_TBL;

/* A bad status's model colors: the clut color, then the ratio and model color for sbtlLeaf60. */
struct TEXCOLOR_WORK {
    s32 cr;
    s32 cg;
    s32 cb;
    f32 ratio;
    s32 mr;
    s32 mg;
    s32 mb;
    s8 flg;
};

extern struct TEXCOLOR_WORK *GlobalPID;


/* Attacker and target figures for one damage calculation, on the caller's stack. */
typedef struct SBTLCALC_DMGWORK {
    /* 0x00 */ s16 alevel;
    /* 0x04 */ s32 aatk;
    /* 0x08 */ s32 atec;
    /* 0x0C */ f32 ahp;
    /* 0x10 */ f32 amaxhp;
    /* 0x14 */ s32 gun_power;
    /* 0x18 */ s32 sword_power;
    /* 0x1C */ s16 tlevel;
    /* 0x20 */ s32 tdef;
    /* 0x24 */ s32 ttec;
    /* 0x28 */ s32 tvit;
    /* 0x2C */ s32 tluck;
    /* 0x30 */ f32 thp;
    /* 0x34 */ f32 tmaxhp;
    /* 0x38 */ s16 n0;
    /* 0x3C */ f32 n1;
    /* 0x40 */ f32 damage_per;
    /* 0x44 */ u8 side;
    /* 0x48 */ struct SBTL_AISYO_TBL *aisyo_tbl;
} SBTLCALC_DMGWORK;

SIZE_CHECK(SBTLCALC_DMGWORKSizeCheck, SBTLCALC_DMGWORK, 0x4C);

s32 CalcAisyo(struct SBTLUNIT_WORK *wk);
struct TEXCOLOR_WORK *sbtlRefGlobalPID(BST badstno);
s32 sbtlBtlSetBadStatusNoSub2(struct SBTLUNIT_WORK *wk, BST badstno);
s32 sbtlSetAutoSkill_ID(struct SBTLUNIT_WORK *wk, SKLID id);
SKLID sbtlGetAutoSkill_ID(struct sdatUnitWork_t *unit, s32 id);
s32 sbtlCheckBadStatus(struct SBTLUNIT_WORK *wk);
void sbtlCheckBadStatus_Weak(struct SBTLUNIT_WORK *wk, SKLID sklid, struct SBTLSKILLP_TBL *ptbl, s32 acterID);
void sbtlCheckBattlePara(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckCost(struct SBTLUNIT_WORK *wk, s32 flg);
#ifdef VERSION_RAIDOU2
u8 sbtlCheckCost2(struct SBTLUNIT_WORK *wk, SKLID sklid, s32 flg);
#endif
s32 sbtlGetMagCost(struct sdatUnitWork_t *player, struct sdatUnitWork_t *unit, s32 devilid);
s32 sbtlCheckMagCost_GB(struct sdatUnitWork_t *player, struct sdatUnitWork_t *unit, s32 devilid, s8 flg);
s32 sbtlCheckMagCost(struct sdatUnitWork_t *unit, s32 devilid, s8 flg);
BST sbtlGetBadStatus(struct sdatUnitWork_t *unit);
/* HP (isMp 0) or MP gain to add to unit, cut so it stops at the maximum. */
s32 LimitRange(struct sdatUnitWork_t *unit, s32 value, s32 isMp);
s32 sbtlCallLimitRange(struct SBTLUNIT_WORK *wk, s32 value);
s32 sbtlCheckMoon(u16 moon);
s32 sbtlResetAutoSkill(struct sdatUnitWork_t *unit);
#ifdef VERSION_RAIDOU2
s32 sbtlSetAutoSkill_GBID(struct sdatUnitWork_t *unit, s32 id, SKLID sklid);
#else
s32 sbtlSetAutoSkill_GBID(struct sdatUnitWork_t *unit, s32 id);
#endif
s32 sbtlCheckAutoSkill_ActerDamage_ID(SKLID sklid);
s32 sbtlCheckAutoSkill_TargetDamage_ID(SKLID sklid);
s32 sbtlCheckAutoSkill_ActerDamage(s32 autono);
s32 sbtlCheckAutoSkill_TargetDamage(s32 autono);
s32 sbtlCheckAutoSkill_Action(struct SBTLUNIT_WORK *wk, struct SBTLDMG_HITWORK *hwk, s32 attr);
s32 sbtlCheckKuishibari(struct SBTLUNIT_WORK *wk, s32 damage);
s32 sbtlGetAutoSkillPara(SKLID id, s16 *badst, s16 *power);
#ifdef VERSION_RAIDOU2
u32 sbtlCheckAisyoEx(struct SBTLUNIT_WORK *wk, u32 attr, s32 *hanshaType);
s32 sbtlGetDamage_MRecover(SBTLCALC_DMGWORK *dmg, struct SBTLSKILLP_TBL *tbl);
s32 sbtlCheckRegistKaja(struct SBTLSKL_KAJA_WORK *kaja, struct SBTLSKILLP_TBL *skill);
s32 sbtlGetSkillCost(struct sdatUnitWork_t *unit, SKLID id);
#endif
s32 sbtlBtlCheckBadStatusWeakSub(SKLID skill);
u32 sbtlCheckBadStatus_Number(u32 badstatus, BST bstno);
s32 sbtlGetBadStatus_Attr(BST bstno);
s32 sbtlCheckBadStatus_Hit(s32 *resist, s32 luck, struct sdatUnitWork_t *unit, s32 tgtLuck, SKLID skill, BST bst, s32 idx, u8 side);
s32 CalcBtlMagicStatus(struct SBTLUNIT_WORK *wk);
s32 sbtlCallSetClutColor(struct SBTLUNIT_WORK *wk, BST bst);
s32 sbtlGetDamage_I_REC_K(struct sdatUnitWork_t *unit, struct SBTLCALC_DMGWORK *wk, struct SBTLSKILLP_TBL *ptbl, s8 flg);
s32 sbtlGetDamage_I_REC_P(struct sdatUnitWork_t *unit, struct SBTLCALC_DMGWORK *wk, struct SBTLSKILLP_TBL *ptbl, s8 flg);
s32 sbtlGetDamage_I_REC_PARA(struct sdatUnitWork_t *unit, struct SBTLCALC_DMGWORK *wk, struct SBTLSKILLP_TBL *ptbl);
s32 sbtlGetDamage_ITEMFLG(struct sdatUnitWork_t *unit, struct SBTLCALC_DMGWORK *wk, struct SBTLSKILLP_TBL *ptbl, s32 itemid);
s32 sbtlLeaf97(struct sdatUnitWork_t *unit);
s32 sbtlGetDamage_CHUUSEI(struct sdatUnitWork_t *unit, struct SBTLCALC_DMGWORK *wk, struct SBTLSKILLP_TBL *ptbl);
s32 sbtlGetDamage_P_LUCK(struct sdatUnitWork_t *unit, struct SBTLCALC_DMGWORK *wk, struct SBTLSKILLP_TBL *ptbl, s32 itemid);
s32 sbtlGetDamage_P_STR(struct sdatUnitWork_t *unit, struct SBTLCALC_DMGWORK *wk, struct SBTLSKILLP_TBL *ptbl, s32 itemid);
s32 sbtlGetDamage_P_TEC(struct sdatUnitWork_t *unit, struct SBTLCALC_DMGWORK *wk, struct SBTLSKILLP_TBL *ptbl, s32 itemid);
s32 sbtlGetDamage_P_VIT(struct sdatUnitWork_t *unit, struct SBTLCALC_DMGWORK *wk, struct SBTLSKILLP_TBL *ptbl, s32 itemid);
s32 sbtlKudaHuuinCheck(void);
void sbtlDecItem(s32 index, s32 itemid);
s32 sbtlCheckAisyo_Old(u32 attr);
u32 sbtlSEquationno03Sub(struct SBTL_AISYO_TBL *aisyo, u32 attr);
s32 sbtlResetBadStatus(struct SBTLUNIT_WORK *wk, BSTF flg);
void sbtlResetItemFlg(struct sdatUnitWork_t *unit);
s32 CalcAutoSkill(struct sdatUnitWork_t *unit, s32 mode, s32 value, SKLID skillid, s16 attr, s16 percent);
s32 sbtlResetBadStatus_GB(struct sdatUnitWork_t *unit, BSTF flg);
s32 sbtlSetBadStatus_No(struct SBTLUNIT_WORK *wk, SKLID sklid, BST badstno, s32 flg, s32 acterID);
s32 sbtlSetItem(struct SBTLUNIT_WORK *wk);
s32 sbtlDeleteHitHelper(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckItemCost_GBWK(s32 itemid, s32 flg);
void sbtlDecItem_GBWK(s32 itemid);
s32 sbtlCheckItemCount(struct sdatUnitWork_t *unit);
void sbtlBtlCheckItemCountSub(struct sdatUnitWork_t *unit);
s32 sbtlLeaf123(struct sdatUnitWork_t *unit);

#endif
