#include "common.h"
#include "newdata.h"
#include "sBtlDvltk.h"
#include "sbtlWork.h"
#include "fldProcess.h"
#include "sdfChip.h"
#include "sBtlCalc.h"
#include "sdk_libc.h"
#include "sdfMemory.h"
#include "sbattle.h"
#include "fld.h"

INCLUDE_ASM(const s32, "sbtl", sbtlLeaf125);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlRunSklProcSub);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl65);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl64);

INCLUDE_ASM(const s32, "sbtl", sbtlLeaf123);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl68);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl62);

INCLUDE_ASM(const s32, "sbtl", sbtlCallFptodp3);

INCLUDE_ASM(const s32, "sbtl", sbtlSbtlCameraproc);

INCLUDE_ASM(const s32, "sbtl", sbtlCallGetActiveScene4);

INCLUDE_ASM(const s32, "sbtl", sbtlCallGetActiveScene3);

s32 sbtlCallBgSetScrollReset(void)
{
    sbtlLeaf118();
    sbtlCallSetPara();
    mapBgSetScrollReset();
    return 1;
}

INCLUDE_ASM(const s32, "sbtl", sbtlSetBossMode);

INCLUDE_ASM(const s32, "sbtl", sbtlInitSBtlSub3);

INCLUDE_ASM(const s32, "sbtl", sbtlLeaf110);

INCLUDE_ASM(const s32, "sbtl", sbtlGetIdling);

INCLUDE_ASM(const s32, "sbtl", Slerp_sbtlCamera);

INCLUDE_ASM(const s32, "sbtl", sbtlIsMoveEnd);

INCLUDE_ASM(const s32, "sbtl", sbtlSetMoveEnd);

INCLUDE_ASM(const s32, "sbtl", GetNowPosAS);

INCLUDE_ASM(const s32, "sbtl", GetNowPosS);

INCLUDE_ASM(const s32, "sbtl", PosCtrlSystem);

INCLUDE_ASM(const s32, "sbtl", CameraMoveSystem);

INCLUDE_ASM(const s32, "sbtl", SetEnemyCPos);

INCLUDE_ASM(const s32, "sbtl", MenuStealth);

INCLUDE_ASM(const s32, "sbtl", sbtlMenuOpen);

INCLUDE_ASM(const s32, "sbtl", sbtlMenuClose);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl67);

INCLUDE_ASM(const s32, "sbtl", SetPara);

INCLUDE_ASM(const s32, "sbtl", sbtlCallCameraObjectBasicGetFovy);

INCLUDE_ASM(const s32, "sbtl", sbtlCallSetPosPoseSetDds3Basic2);

s32 sbtlLeaf103(dds3ProcessID_t *id) {
    return 0;
}

INCLUDE_ASM(const s32, "sbtl", sbtlLeaf87);

s32 sbtlLeaf118(void)
{
    return 1;
}

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlSearchUnitID35);

INCLUDE_ASM(const s32, "sbtl", sbtlCallSetPara);

INCLUDE_ASM(const s32, "sbtl", sbtlNoHokan);

INCLUDE_ASM(const s32, "sbtl", sbtlSetMode);

INCLUDE_ASM(const s32, "sbtl", sbtlGetMode);

INCLUDE_ASM(const s32, "sbtl", sbtlCallMemset31);

INCLUDE_ASM(const s32, "sbtl", sbtlLeaf121);

INCLUDE_ASM(const s32, "sbtl", sbtlCallMemset30);

INCLUDE_ASM(const s32, "sbtl", sbtlCallMemset33);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckDamageTargetUseItem);

INCLUDE_ASM(const s32, "sbtl", sbtlGetCutFlg);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl56);

INCLUDE_ASM(const s32, "sbtl", sbtlBgnPosSlp);

INCLUDE_ASM(const s32, "sbtl", ClrAction);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl55);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl61);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl59);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl57);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl69);

INCLUDE_ASM(const s32, "sbtl", sbtlLeaf93);

INCLUDE_ASM(const s32, "sbtl", sbtlRefDDS3GBWK21);

INCLUDE_ASM(const s32, "sbtl", CalcItem_Battle);

INCLUDE_ASM(const s32, "sbtl", sbtlSetWeakBit);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlGetNakama5);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckItemMember);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlSetBadStatusRandSub);

INCLUDE_ASM(const s32, "sbtl", sbtlCallGetPercentage);

INCLUDE_ASM(const s32, "sbtl", sbtlSetBadStatusRand);

INCLUDE_ASM(const s32, "sbtl", CalcItem);

INCLUDE_ASM(const s32, "sbtl", CalcItem_Battle_sbtlCalc);

INCLUDE_ASM(const s32, "sbtl", sbtlResetItemFlg);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckDrink);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckItemCount);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckHaveskill_Atk);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckHaveskill_Magic);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckHaveskill_Rec);

s32 sbtlCheckAutoSkill(sdatUnitWork_t *unit, s32 flg) {
    return 0;
}

INCLUDE_ASM(const s32, "sbtl", sbtlCheckAutoSkill_Enemy);

INCLUDE_ASM(const s32, "sbtl", CalcAisyo);

INCLUDE_ASM(const s32, "sbtl", sbtlCallClearUnitSkills);

INCLUDE_ASM(const s32, "sbtl", sbtlSetAutoSkill_GBID);

INCLUDE_ASM(const s32, "sbtl", sbtlSetAutoSkill_ID);

INCLUDE_ASM(const s32, "sbtl", cmbClearUnitSkills);

INCLUDE_ASM(const s32, "sbtl", sbtlGetAutoSkillPara);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlGetSkilltbl19);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckAutoSkill_ActerDamage_ID);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckAutoSkill_TargetDamage_ID);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckAutoSkill_ActerDamage);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckAutoSkill_TargetDamage);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckAutoSkill_Action);

INCLUDE_ASM(const s32, "sbtl", CalcAutoSkill);

INCLUDE_ASM(const s32, "sbtl", sbtlGetDropMoney);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckEvtBit_Table);

INCLUDE_ASM(const s32, "sbtl", sbtlGetSkillCost);

INCLUDE_ASM(const s32, "sbtl", sbtlGetMagCost);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckUseSkill);

INCLUDE_ASM(const s32, "sbtl", sbtlLeaf90);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckMagCost);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckCost2);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckCost);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckCost_SkillID);

INCLUDE_ASM(const s32, "sbtl", sbtlDecVice);

INCLUDE_ASM(const s32, "sbtl", sbtlDecMag);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl71);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl54);

INCLUDE_ASM(const s32, "sbtl", sbtlDecHP);

INCLUDE_ASM(const s32, "sbtl", sbtlDecMP);

INCLUDE_ASM(const s32, "sbtl", sbtlRefDDS3GBWK22);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl63);

INCLUDE_ASM(const s32, "sbtl", sbtlSetGunNum);

INCLUDE_ASM(const s32, "sbtl", sbtlRefSbtl66);

s32 sbtlGetGun_Num(void)
{
    return wk_sbtl->hgunNum;
}

SBTL_AISYO_TBL *sbtlGetAisyotbl(s32 id)
{
    return &aisyo_tbldata[id];
}

INCLUDE_ASM(const s32, "sbtl", sbtlCheckAttrFlg);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckAisyo_Old);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlCheckAisyoExSub);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckAisyoEx);

INCLUDE_ASM(const s32, "sbtl", WeakBstDvlTk);

INCLUDE_ASM(const s32, "sbtl", LimitRange);

INCLUDE_ASM(const s32, "sbtl", sbtlAutoRec);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlGetAutoSkillPara);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckSpecialDamage_Stop);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlChkHaveMemSkl2);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckBadStatus_Return);

INCLUDE_ASM(const s32, "sbtl", sbtlGetBadStatus);

INCLUDE_ASM(const s32, "sbtl", CalcBadStatus);

INCLUDE_ASM(const s32, "sbtl", sbtlResetBadStatus_GB);

INCLUDE_ASM(const s32, "sbtl", sbtlResetBadStatus);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlCheckBadStatusWeakNumberSub);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckBadStatus);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckBadStatus_sbtlCalc);

INCLUDE_ASM(const s32, "sbtl", sbtlResetUnit);

INCLUDE_ASM(const s32, "sbtl", sbtlLeaf107);

INCLUDE_ASM(const s32, "sbtl", sbtlCallSetClutColor);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckBadStatus_Weak);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlSetBadStatusNoSub);

INCLUDE_ASM(const s32, "sbtl", sbtlSetBadStatus_No);

INCLUDE_ASM(const s32, "sbtl", sbtlRefGlobalPID);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlSetBadStatusNoSub2);

INCLUDE_ASM(const s32, "sbtl", sbtlCallSetClutColor2);

INCLUDE_ASM(const s32, "sbtl", sbtlSetBadStatusAction);

INCLUDE_ASM(const s32, "sbtl", SetBadStatusCounter);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlCheckBadStatusWeakSub);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckBadStatus_WeakNumber);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckBadStatus_Hit);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlGetSkilltbl21);

INCLUDE_ASM(const s32, "sbtl", sbtlKudaDmg);

INCLUDE_ASM(const s32, "sbtl", sbtlFullMoonDmg);

INCLUDE_ASM(const s32, "sbtl", sbtlZokuseiDmg);

INCLUDE_ASM(const s32, "sbtl", sbtlGetDamage_sbtlCalc);

INCLUDE_ASM(const s32, "sbtl", sbtlResetBattlePara);

INCLUDE_ASM(const s32, "sbtl", sbtlGetPlayerTec_ParaUp);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckBattlePara);

INCLUDE_ASM(const s32, "sbtl", CalcAisyo_sbtlCalc);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlGetDamageSBtlCalcSub);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlCheckAisyoOld);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlCheckAisyoOld3);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlCheckAisyoOld4);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlCheckAisyoOld5);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlCheckAisyoOld6);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlGetDamageSBtlCalcSub2);

INCLUDE_ASM(const s32, "sbtl", sbtlGetDamage_The_W);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlCheckAisyoOld7);

INCLUDE_ASM(const s32, "sbtl", sbtlGetDamage_MRecover);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlCheckAisyoOld2);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlGetDamageSBtlCalcSub3);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlGetDamageSBtlCalcSub4);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckRegistKaja);

INCLUDE_ASM(const s32, "sbtl", sbtlCheckRegistKajaMaxNewAI);

INCLUDE_ASM(const s32, "sbtl", sbtlGetDamage_ParaUP);

INCLUDE_ASM(const s32, "sbtl", sbtlLeaf115);

INCLUDE_ASM(const s32, "sbtl", sbtlITEMRECUP);

INCLUDE_ASM(const s32, "sbtl", sbtlITEMRECUP2);

INCLUDE_ASM(const s32, "sbtl", sbtlplayertblPlayerSkill_GetMag3);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlCheckItemCountSub);

INCLUDE_ASM(const s32, "sbtl", sbtlGetDamage_P_STR);

INCLUDE_ASM(const s32, "sbtl", sbtlGetDamage_P_VIT);

INCLUDE_ASM(const s32, "sbtl", sbtlGetDamage_P_TEC);

INCLUDE_ASM(const s32, "sbtl", sbtlGetDamage_P_LUCK);

INCLUDE_ASM(const s32, "sbtl", sbtlGetDamage_CHUUSEI);

INCLUDE_ASM(const s32, "sbtl", sbtlGetDamage_ITEMFLG);

INCLUDE_ASM(const s32, "sbtl", sbtlLeaf97);

INCLUDE_ASM(const s32, "sbtl", sbtlGetViceLevel);

INCLUDE_ASM(const s32, "sbtl", sbtlRecalcAisyoMember);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlLoadHuumaMesDataSub);

SBTLDVLTK_LISTWORK *sbtlCallChipAllocZeroDirect(void)
{
    SBTLDVLTK_LISTWORK *lwk = sdfChipAllocZeroDirect(sizeof(SBTLDVLTK_LISTWORK));

    lwk->num = 0;
    lwk->dwk = NULL;
    return lwk;
}

INCLUDE_ASM(const s32, "sbtl", sbtlLeaf101);

INCLUDE_ASM(const s32, "sbtl", sbtlCreateWork);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlGetNakama6);

INCLUDE_ASM(const s32, "sbtl", sbtlCreateDvltk);

INCLUDE_ASM(const s32, "sbtl", Proc_sbtlDvltk);

INCLUDE_ASM(const s32, "sbtl", sbtlSearchDvltk);

INCLUDE_ASM(const s32, "sbtl", Draw_sbtlDvltk);

s32 sbtlReleaseDvltk_List(SBTLDVLTK_LISTWORK *stp, u8 flg)
{
    SBTLDVLTK_WORK *node;
    SBTLDVLTK_WORK *next;

    if (stp == NULL) {
        return 0;
    }
    node = stp->dwk;
    if (node == NULL || stp->num == 0) {
        return 0;
    }
    do {
        if (flg) {
            node->status = 2;
        }
        if (node->status == 2) {
            if (node->prev != NULL) {
                node->prev->next = node->next;
            }
            if (node->next != NULL) {
                node->next->prev = node->prev;
            }
            if (node->prev == NULL) {
                stp->dwk = node->next;
                if (stp->dwk != NULL) {
                    stp->dwk->prev = NULL;
                }
            }
            next = node->next;
            sdfMemFreeDirect(node->handle);
            node = next;
            stp->num--;
        } else {
            node = node->next;
        }
    } while (node != NULL);
    return 1;
}

INCLUDE_ASM(const s32, "sbtl", sbtlDeleteDvltkList);

INCLUDE_ASM(const s32, "sbtl", sbtlCallMessageNumSearch3);

INCLUDE_ASM(const s32, "sbtl", sbtlLeaf99);

INCLUDE_ASM(const s32, "sbtl", sbtlCallBtlSearchUnitID34);

INCLUDE_ASM(const s32, "sbtl", sbtlDrawTextFont);

INCLUDE_ASM(const s32, "sbtl", sbtlBtlDrawTextSub2);

struct sprHandle_t *sbtlGetSpriteHandle(void)
{
    if (sbtlData104 == NULL) {
        if (wk_sbtl->panel != NULL) {
            return wk_sbtl->panel->hPanelSpr5;
        }
        return NULL;
    }
    return sbtlData105;
}

INCLUDE_ASM(const s32, "sbtl", sbtlExt_CalcProcess);

INCLUDE_ASM(const s32, "sbtl", sbtlExt_DrawProcess);

INCLUDE_ASM(const s32, "sbtl", sbtlExt_Free);

INCLUDE_ASM(const s32, "sbtl", sbtlExt_ShutDown);

INCLUDE_ASM(const s32, "sbtl", sbtlExt_CreateBuffer);

INCLUDE_ASM(const s32, "sbtl", sbtlExt_FreeBuffer);

INCLUDE_ASM(const s32, "sbtl", sbtlExt_MesReqEx);

INCLUDE_ASM(const s32, "sbtl", sbtlExt_MesReq);

INCLUDE_ASM(const s32, "sbtl", sbtlExt_EndMes);

s32 sbtlExt_ChkMes(void) {
    return sbtlData104 != NULL;
}

INCLUDE_ASM(const s32, "sbtl", sbtlExtCommand_DVLTK_DSP);

INCLUDE_ASM(const s32, "sbtl", sbtlExtCommand_DVLTK_WAIT);
