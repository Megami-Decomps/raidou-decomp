#include "common.h"
#include "newdata.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "dds3KernelCore.h"
#include "sbtlWork.h"
#include "sdfChip.h"
#include "sdatUnit.h"
#include "sBtlEncount.h"
#include "sBtlCalc.h"
#include "sBtlUnit.h"
#include "sdk_libc.h"
#include "sBtlSkill.h"
#include "sBtlPlayer.h"
#include "sBtlSM.h"
#include "sbtlSub.h"
#include "sdfDifficulty.h"

u8 sbtlCheckCost_SkillID(struct SBTLUNIT_WORK *wk, SKLID sklid, s32 arg)
{
    struct SBTLSKILLP_TBL *skill;
    struct SBTLUNIT_WORK *target;
    s32 i;

    skill = sbtlGetSkilltbl(sklid);
    for (i = 0; i < 6; i++) {
        if (!((skill->s_type >> i) & 1)) {
            continue;
        }
        switch (i) {
        case 5:
            if (wk->field414 & 2) {
                return 1;
            }
            if (wk->field414 & 0x80) {
                if (wk_sbtl->field344 > 0) {
                    target = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[wk_sbtl->summonIndex[0]]);
                } else {
                    return 0;
                }
            } else {
                target = wk;
            }
            if (target == NULL) {
                return 0;
            }
            if (target->unit.vice < skill->para) {
                return 0;
            }
            if (arg == 1) {
                sbtlDecVice(target, skill->para);
            }
            return 1;
        case 0:
            if (wk_sbtl->mag < skill->para) {
                return 0;
            }
            if (arg == 1) {
                sbtlRefSbtl64(skill->para);
            }
            return 1;
        case 1:
            if (wk->unit.now.hp < skill->para) {
                return 0;
            }
            if (arg == 1) {
                sbtlDecHP(wk, skill->para);
            }
            return 1;
        case 4:
            if (wk->unit.now.mp < skill->para) {
                return 0;
            }
            if (arg == 1) {
                sbtlDecMP(wk, skill->para);
            }
            return 1;
        case 2:
            if (sbtlCallBtlGetPlayerData3() < skill->para) {
                return 0;
            }
            if (arg == 1) {
                sbtlCallBtlGetPlayerData6(skill->para);
            }
            return 1;
        case 3:
            break;
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlDecVice);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlRefSbtl64);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlDecHP);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlDecMP);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlRefDDS3GBWK22);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlRefSbtl63);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlCallBtlGetPlayerData7);

void sbtlCallBtlGetPlayerData6(s32 num)
{
    SBTL_PLAYER_DATA *player = sbtlGetPlayerData();
    s32 infinite = FALSE;
    s32 bullet = wk_sbtl->equipBullet;

    if (bullet == 80) {
        infinite = player->item[111] > 0;
    }
    if (!infinite) {
        wk_sbtl->hgunNum -= num;
        player->item[bullet] -= num;
    }
}

s32 sbtlCallBtlGetPlayerData3(void)
{
    return sbtlGetPlayerData()->item[wk_sbtl->equipBullet];
}

SBTL_AISYO_TBL *sbtlGetAisyotbl(s32 id)
{
    return &aisyo_tbldata[id];
}

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlCheckAttrFlg);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlCheckAisyo_Old);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlSEquationno03Sub);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", WeakBstDvlTk);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", LimitRange);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlCallLimitRange);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlCheckKuishibari);

s32 sbtlRefSbtl68(struct SBTLUNIT_WORK *wk)
{
    if (wk_sbtl->flg & 0x2000000) {
        return 1;
    }
    if (wk_sbtl->flg & 0x40) {
        return 1;
    }
    if (wk->unit.devilId == 263 || wk->unit.devilId == 302) {
        if (wk->status == 7) {
            return 1;
        }
    }
    return 0;
}

/* NON_MATCHING: 436 of 441 words match.  */
#ifdef NON_MATCHING
s32 CalcBtlMagicStatus(struct SBTLUNIT_WORK *wk)
{
    sdatUnitWork_t *unit;
    struct SBTLUNIT_WORK *target;
    struct SBTLUNIT_WORK *eff;
    struct SBTLSKILLP_TBL *tbl;
    SKLID sklid;
    f32 heal;
    s32 value;
    s32 hp;

    unit = &wk->unit;
    if (sbtlCallBtlGetSkilltbl21(sbtlGetAutoSkill_ID(unit, 0), &wk->unit.autost_cnt, &wk->unit.autost_cnt2) == 2
        || sbtlCallBtlGetSkilltbl21(sbtlGetAutoSkill_ID(unit, 1), &wk->unit.autost_cnt, &wk->unit.autost_cnt2) == 2) {
        if (++wk->autoreccnt >= wk->unit.autost_cnt2) {
            wk->autoreccnt = 0;
            heal = wk->unit.now.maxhp * 0.03f;
            if (heal <= 1.0f) {
                heal = 1.0f;
            }
            sbtlCallLimitRange(wk, heal);
            target = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
            if (target != NULL && sbtlCheckDamageTarget(target) && !(wk->field414 & 2) && target->status != 34) {
                if (sbtlCheckCrossCircle_Rot(&wk->pos, &wk->rot, wk->model->field11C + 300.0f, &target->pos, target->model->field11C, -1.0f)) {
                    heal = target->unit.now.maxhp * 0.03f;
                    if (heal <= 1.0f) {
                        heal = 1.0f;
                    }
                    sbtlCallLimitRange(target, heal);
                }
            }
        }
    }

    if (wk->field360 > 0 && --wk->field360 <= 0) {
        wk->field360 = 0;
        wk->field35C = 0;
        if (wk->field358 & 0x1000000) {
            sklid = SKLID_OTAKEBI;
        } else if (wk->field358 & 0x2000000) {
            sklid = SKLID_INTERIB;
        } else {
            sklid = SKLID_MASSURU;
        }
        if (wk->field414 & 1) {
            sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_H2_UP_END_P, sklid);
        } else {
            sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_H2_UP_END_E, sklid);
        }
        wk->field358 = 0;
    }

    if (wk->field362 > 0) {
        if (--wk->field362 <= 0) {
            wk->field362 = 0;
            wk->rejyenecounter = 0;
            wk->rejyenecmpcnt = 0;
            wk->rejyenepara = 0;
        } else if (++wk->rejyenecounter >= wk->rejyenecmpcnt) {
            wk->rejyenecounter = 0;
            value = LimitRange(unit, wk->rejyenepara, 0);
            hp = wk->unit.now.hp + value;
            wk->unit.now.hp = hp;
            wk->unit.now.hp = hp <= 0 ? 0 : (hp < wk->unit.now.maxhp ? hp : wk->unit.now.maxhp);
            if (value > 0) {
                sbtlCallBtlSearchUnitID14(wk, value, 0, 3);
                if (!(wk->flg & 0x1000) || (eff = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->rejyenehelper)) == NULL) {
                    eff = wk;
                }
                sbtlCallMemset17(wk_sbtl->effList, eff, EFFID_AUTOREC, 0, &eff->pos.x, 0x80808080, 0);
            }
        }
    }

    if (wk->field384 > 0 && --wk->field384 <= 0) {
        wk->field384 = 0;
        wk->field37C = 0;
        wk->field380 = 0;
        sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_COMBI_EFFEND, wk->barriersklid);
        sbtlCheckBattlePara(wk);
    }

    if (!sbtlRefSbtl68(wk) && wk->field370 > 0) {
        if (wk->mv_spd.z == wk->max_mvspd.z) {
            wk->mv_spd.z *= 0.5f;
        }
        if (wk->mv_spdbk.z == wk->max_mvspd.z) {
            wk->mv_spdbk.z *= 0.5f;
        }
        if (--wk->field370 <= 0) {
            wk->field370 = 0;
            wk->slipcounter = 0;
            wk->slipcmpcnt = 0;
            wk->slippara = 0;
            wk->slip_sklid = 0;
            sbtlForceDelete_ID_Acter(wk_sbtl->effList, wk, EFFID_RENKEIOR);
        } else if (++wk->slipcounter >= wk->slipcmpcnt) {
            wk->slipcounter = 0;
            hp = wk->unit.now.hp - sbtlCheckKuishibari(wk, wk->slippara);
            wk->unit.now.hp = hp;
            wk->unit.now.hp = hp <= 0 ? 0 : (hp < wk->unit.now.maxhp ? hp : wk->unit.now.maxhp);
            tbl = sbtlGetSkilltbl(wk->slip_sklid);
            if (!(wk->flg & 0x1000) || (eff = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->hitsliphelper)) == NULL) {
                eff = wk;
            }
            if (tbl->attr & 4) {
                sbtlCallMemset17(wk_sbtl->effList, eff, EFFID_SLIPKAEN, 0, &eff->pos.x, 0x80808080, 0);
            }
            if (tbl->attr & 8) {
                sbtlCallMemset17(wk_sbtl->effList, eff, EFFID_SLIPICE, 0, &eff->pos.x, 0x80808080, 0);
            }
            if (tbl->attr & 0x10) {
                sbtlCallMemset17(wk_sbtl->effList, eff, EFFID_SLIPTHUNDER, 0, &eff->pos.x, 0x80808080, 0);
            }
            if (tbl->attr & 0x20) {
                sbtlCallMemset17(wk_sbtl->effList, eff, EFFID_SLIPZAN, 0, &eff->pos.x, 0x80808080, 0);
            }
            sbtlCallBtlSearchUnitID14(wk, wk->slippara, 0, 1);
            if (wk->flg & 0x40000000) {
                sbtlSetSpecialDamage_DamageOnly(wk);
                return 0;
            }
            if (wk->unit.now.hp == 0) {
                sbtlSetDeadDamage(wk, 0);
                sbtlForceDelete_ID_Acter(wk_sbtl->effList, wk, EFFID_RENKEIOR);
                return 0;
            }
        }
    }

    if (!sbtlRefSbtl68(wk) && wk->tdancnt > 0) {
        if (--wk->tdancnt <= 0) {
            wk->tdanhitcnt = 0;
            wk->tdancnt = 0;
            wk->tdancounter = 0;
            wk->tdancmpcnt = 0;
            wk->tdanpara = 0;
        } else if (++wk->tdancounter >= wk->tdancmpcnt) {
            wk->tdancounter = 0;
            hp = wk->unit.now.hp - sbtlCheckKuishibari(wk, wk->tdanpara);
            wk->unit.now.hp = hp;
            wk->unit.now.hp = hp <= 0 ? 0 : (hp < wk->unit.now.maxhp ? hp : wk->unit.now.maxhp);
            sbtlCallBtlSearchUnitID14(wk, wk->tdanpara, 0, 2);
            if (wk->flg & 0x40000000) {
                sbtlSetSpecialDamage_DamageOnly(wk);
                return 0;
            }
            if (wk->unit.now.hp == 0) {
                sbtlSetDeadDamage(wk, 0);
            }
        }
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "sbtl_0029BFB8", CalcBtlMagicStatus);
#endif

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlCallEventBattleStopChack);

BST sbtlGetBadStatus(sdatUnitWork_t *unit)
{
    s32 i;

    if (unit->badStatus != 0) {
        for (i = BST_KANASHIB; i < BST_RAND0; i++) {
            if ((unit->badStatus >> i) & 1) {
                return i;
            }
        }
    }
    return BST_BOYCOTT;
}

/* NON_MATCHING: 32 of 236 words match.  */
#ifdef NON_MATCHING
s32 CalcBadStatus(struct SBTLUNIT_WORK *wk)
{
    s32 i;
    s32 dmg;
    s32 hp;
    s16 cnt;
    f32 rate;
    sdatUnitWork_t *unit;

    if (wk->unit.badStatus == 0) {
        return 1;
    }
    for (i = 0; i < 18; i++) {
        if (((wk->unit.badStatus >> i) & 1) == 0) {
            continue;
        }
        if (i != 13 && wk->unit.badst_cnt != 0) {
            wk->unit.badst_cnt--;
            if (wk->unit.badst_cnt <= 0) {
                wk->unit.badst_cnt = 0;
                sbtlResetBadStatus(wk, 1 << i);
                return 0;
            }
        }
        switch (i) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 14:
        case 15:
        case 16:
        case 17:
            break;
        case 6:
            cnt = ++wk->unit.badst_cnt2;
            if (cnt <= 0) {
                cnt = 0;
            } else if (cnt > 180) {
                cnt = 180;
            }
            wk->unit.badst_cnt2 = cnt;
            if (cnt < 180) {
                break;
            }
            hp = wk->unit.now.hp;
            dmg = wk->unit.now.maxhp * 0.03f;
            wk->unit.badst_cnt2 = 0;
            if (hp < 2) {
                break;
            }
            if (hp - dmg > 0) {
                wk->unit.now.hp = hp - dmg;
            } else {
                dmg = hp - 1;
                wk->unit.now.hp = hp - dmg;
            }
            if ((wk->field414 & 1) && (wk->field414 & 0x80)) {
                sbtlSetVibe(SBTLPLAYER_VTYPE_NORMALD);
            }
            if (wk->unit.now.hp <= 0) {
                wk->unit.now.hp = 1;
            }
            sbtlCallBtlSearchUnitID14(wk, dmg, 0, 0);
            break;
        case 7:
            if (sbtlRefSbtl68(wk)) {
                break;
            }
            cnt = ++wk->unit.badst_cnt2;
            if (cnt <= 0) {
                cnt = 0;
            } else if (cnt > 180) {
                cnt = 180;
            }
            wk->unit.badst_cnt2 = cnt;
            if (cnt < 180) {
                break;
            }
            wk->unit.badst_cnt2 = 0;
            if (wk->unit.now.hp <= 0) {
                break;
            }
            if (sbtlCheckDamageTarget(wk) == 0) {
                break;
            }
            unit = &wk->unit;
            rate = wk->unit.now.maxhp * 0.01;
            dmg = rate;
            dmg = LimitRange(unit, (dmg > 0) ? dmg : 1, 0);
            hp = wk->unit.now.hp += dmg;
            if (hp <= 0) {
                wk->unit.now.hp = 0;
            } else if (hp >= wk->unit.now.maxhp) {
                wk->unit.now.hp = wk->unit.now.maxhp;
            } else {
                wk->unit.now.hp = hp;
            }
            sbtlCallBtlSearchUnitID14(wk, dmg, 0, 3);
            rate = wk->unit.now.maxmp * 0.01;
            dmg = rate;
            dmg = LimitRange(unit, (dmg > 0) ? dmg : 1, 1);
            hp = wk->unit.now.mp += dmg;
            if (hp <= 0) {
                wk->unit.now.mp = 0;
            } else if (hp >= wk->unit.now.maxmp) {
                wk->unit.now.mp = wk->unit.now.maxmp;
            } else {
                wk->unit.now.mp = hp;
            }
            sbtlCallBtlSearchUnitID14(wk, dmg, 12, 4);
            break;
        case 13:
            cnt = ++wk->unit.badst_cnt2;
            if (cnt <= 0) {
                cnt = 0;
            } else if (cnt > 30) {
                cnt = 30;
            }
            wk->unit.badst_cnt2 = cnt;
            if (cnt >= 10) {
                sbtlRefSbtl64(1);
            }
            break;
        }
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtl_0029BFB8", CalcBadStatus);
#endif

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlResetBadStatus_GB);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlResetBadStatus);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlBtlCheckBadStatusWeakNumberSub);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlCheckBadStatus);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlCallCheckBadStatus);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlResetUnit);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlLeaf107);

INCLUDE_RODATA(const s32, "sbtl_0029BFB8", CalcBadStatusTable);

INCLUDE_RODATA(const s32, "sbtl_0029BFB8", CalcBadStatusTable2);

INCLUDE_RODATA(const s32, "sbtl_0029BFB8", sbtlBtlCheckBadStatusWeakNumberSubTable);

INCLUDE_RODATA(const s32, "sbtl_0029BFB8", sbtlLeaf107Table);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlCallSetClutColor);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlCheckBadStatus_Weak);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlBtlSetBadStatusNoSub);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlSetBadStatus_No);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlRefGlobalPID);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlBtlSetBadStatusNoSub2);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlCallSetClutColor2);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlSetBadStatusAction);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlCallBtlSetBadStatusAction);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlBtlCheckBadStatusWeakSub);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlCheckBadStatus_WeakNumber);

#ifdef NON_MATCHING
/* 89 of 119 words differ: retail branches past an inline return 0 after
 * sbtlBtlCheckBadStatusWeakSub where this reuses its zero, and orders the
 * float products differently. Behavior is the same. */
/* Whether the status bst from skill lands on a unit with resistance table
 * resist: the skill's percentage, by the luck ratio and the resistance, against
 * a roll. luck is the attacker's, tgtLuck the target's. */
s32 sbtlCallBtlGetSkilltbl17(s32 *resist, s32 luck, struct sdatUnitWork_t *unit, s32 tgtLuck, SKLID skill, BST bst, s32 idx) {
    struct SBTLSKILLP_TBL *tbl;
    f32 chance;
    f32 hundred;

    if (resist[idx] & 0x23000) {
        return 0;
    }
    if (bst < BST_STONE) {
        if (resist[idx] & 0x4000) {
            return 1;
        }
        if (!sbtlBtlCheckBadStatusWeakSub(skill)) {
            return 0;
        }
    }
    if (bst == BST_GUN) {
        return 1;
    }
    hundred = 100.0f;
    tbl = sbtlGetSkilltbl(skill);
    chance = (f32)tbl->percentage / hundred * (f32)(luck * 2 + 100) / (f32)(tgtLuck * 2 + 100) * (f32)(resist[idx] & 0xFFF);
    if (unit != NULL) {
        if (sbtlCallBtlGetSkilltbl21(sbtlGetAutoSkill_ID(unit, 0), &unit->autost_cnt, &unit->autost_cnt2) == 16) {
            chance *= (f32)unit->autost_cnt2 / hundred;
        }
        if (sbtlCallBtlGetSkilltbl21(sbtlGetAutoSkill_ID(unit, 1), &unit->autost_cnt, &unit->autost_cnt2) == 16) {
            chance *= (f32)unit->autost_cnt2 / hundred;
        }
    }
    chance = SDF_DIFFICULTY_STATUS_CHANCE(chance, bst);
    return (s16)GetPercentage() < (s16)chance;
}
#else
INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlCallBtlGetSkilltbl17);
#endif

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlGetDamage_GB);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlCallBtlGetPlayerData4);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlBtlSetEndDataSub);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlGetPlayerTec_ParaUp);

void sbtlCheckBattlePara(struct SBTLUNIT_WORK *wk)
{
    s16 max = 40;
    sdatUnitWork_t *unit;
    sdatParaWork_t para;

    CalcAisyo(wk);
    sbtlCheckStockPara(&wk->unit);
    wk->unit.now.str += wk->kajyapara.str;
    wk->unit.now.vit += wk->kajyapara.vit;
    wk->unit.now.tec += wk->kajyapara.tec;
    wk->unit.now.luck += wk->kajyapara.luck;
    if (!(wk->field414 & 0x80) && (wk->field414 & 1)) {
        unit = &sbtlGetPlayerData()->unit;
        if (unit->flag & 1) {
            memset(&para, 0, sizeof(para));
            sbtlGetPlayerTec_ParaUp(unit, &para);
            wk->unit.now.str += para.str;
            wk->unit.now.vit += para.vit;
            wk->unit.now.tec += para.tec;
            wk->unit.now.luck += para.luck;
        }
    }
    CalcAisyo_sbtlCalc(wk);
    if (wk->field414 & 2) {
        max = 99;
    }
    wk->unit.now.str = wk->unit.now.str > 0 ? (wk->unit.now.str < max ? wk->unit.now.str : max) : 0;
    wk->unit.now.vit = wk->unit.now.vit > 0 ? (wk->unit.now.vit < max ? wk->unit.now.vit : max) : 0;
    wk->unit.now.tec = wk->unit.now.tec > 0 ? (wk->unit.now.tec < max ? wk->unit.now.tec : max) : 0;
    wk->unit.now.luck = wk->unit.now.luck > 0 ? (wk->unit.now.luck < max ? wk->unit.now.luck : max) : 0;
}

void CalcAisyo_sbtlCalc(struct SBTLUNIT_WORK *wk)
{
    s32 i;

    if (wk->field384 != 0) {
        wk->unit.aisyo = wk->aisyoBase;
        for (i = 0; i < 10; i++) {
            if (wk->field37C & (1 << i)) {
                wk->unit.aisyo.word[i] |= wk->field380;
                if ((wk->field380 & 0x1000) && (wk->unit.aisyo.word[i] & 0x2000)) {
                    wk->unit.aisyo.word[i] &= ~0x2000;
                }
            }
        }
    } else {
        wk->unit.aisyo = wk->aisyoBase;
    }
}

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlCallBtlCheckAisyoOld);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlCallBtlCheckAisyoOld2);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlCallBtlCheckAisyoOld3);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlCallBtlCheckAisyoOld4);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlCallBtlCheckAisyoOld5);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlCallBtlCheckAisyoOld6);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlLeaf118);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlGetDamage_The_W);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlCallBtlCheckAisyoOld7);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlCallFptodp3);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlCallFptodp4);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlGetDamage_Slip);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", GetDamage_Rejyene);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlLeaf115);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlCallBtlCreateSM);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlBtlGetDamageGBSub2);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlGetDamage_I_REC_K);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlGetDamage_I_REC_P);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlplayertblPlayerSkill_GetMag3);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlBtlCheckItemCountSub);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlGetDamage_P_STR);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlGetDamage_P_VIT);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlGetDamage_P_TEC);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlGetDamage_P_LUCK);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlGetDamage_CHUUSEI);

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlGetDamage_ITEMFLG);

/* body: matched in raidou @ 61f09dc: src/raidou1/sbtl.c:86, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00), proto differs */
s32 sbtlLeaf97(dds3ProcessID_t *id) {
    return 0;
}

INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlGetViceLevel);

s32 sbtlBtlLoadHuumaMesDataSub(s32 huumaMesNo)
{
    switch (huumaMesNo) {
    case 80:
        huumaMesNo = 240;
        break;
    case 81:
        huumaMesNo = 241;
        break;
    }
    return gohData7[huumaMesNo].field68;
}

SBTLDVLTK_LISTWORK *sbtlCallChipAllocZeroDirect(void)
{
    SBTLDVLTK_LISTWORK *lwk = sdfChipAllocZeroDirect(sizeof(SBTLDVLTK_LISTWORK));

    lwk->num = 0;
    lwk->dwk = NULL;
    return lwk;
}

/* NON_MATCHING: 42 of 62 words match.  */
#ifdef NON_MATCHING
s32 sbtlRefSbtl62(struct SBTLUNIT_WORK *wk)
{
    struct SBTLUNIT_WORK *unit;
    s32 i;
    s32 hit;
    s32 hasWk = wk != NULL;

    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        unit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (unit != NULL && (u16)(unit->mode - 1) >= 2) {
            hit = FALSE;
            if (hasWk != TRUE || wk->field1A0 == unit->field1A0) {
                hit = TRUE;
            }
            if (hit == TRUE && (unit->field414 & 2) && sbtlCallBtlGetDevilDataSBtlSub4(unit) == 0) {
                return 0;
            }
        }
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "sbtl_0029BFB8", sbtlRefSbtl62);
#endif

INCLUDE_RODATA(const s32, "sbtl_0029BFB8", sbtlBtlGetDamageGBSubTable);

INCLUDE_RODATA(const s32, "sbtl_0029BFB8", sbtlBtlGetDamageGBSubTable2);

