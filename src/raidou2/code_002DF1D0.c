#include "common.h"
#include "sdfChip.h"
#include "sBtlSkill.h"
#include "sdk_libc.h"
#include "sBtlEXP.h"
#include "sbtlWork.h"
#include "sBtlSM.h"
#include "sBtlCalc.h"
#include "sBtlDraw.h"
#include "sBtlUnit.h"
#include "sBtlAI.h"

s32 sbtlCheckMsgMemType(void)
{
    return 1;
}

SBTLEXP_LISTWORK *InitTarSelProcessData(void)
{
    SBTLEXP_LISTWORK *lwk = sdfChipAllocZeroDirect(sizeof(SBTLEXP_LISTWORK));

    memset(lwk, 0, sizeof(SBTLEXP_LISTWORK));
    lwk->ewk = NULL;
    lwk->riritu = 1.0f;
    lwk->field10 = 100;
    lwk->counter = 180;
    lwk->alpha = 128;
    lwk->o_alpha = 128;
    lwk->ralpha = 0;
    lwk->o_counter = 0;
    return lwk;
}

void sbtlInitDropWork(void)
{
    memset(&wk_sbtl->drop, 0, sizeof(wk_sbtl->drop));
}

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlGetAutoSkillPoint);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlGetChuuseiPoint);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlGetSpecialExp);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlGetSpecialMoney);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlGetExp_ChkMemSkl);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlGetChuusei_ChkMemSkl);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCheckDropItem);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCheckGetExpUnit);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCheckDrop);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlBtlCheckDropItemSub);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCheckDrop_Normal);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallMemAllocDirect8);

INCLUDE_ASM(const s32, "code_002DF1D0", DecVice);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlSetVicePara);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlResetVicePara);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallBtlSearchUnitID27);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlSetRirituNakamaHD);

s32 sbtlRefSbtl60(SBTLEXP_LISTWORK *lwk) {
    SBTLEXP_WORK *ewk;

    if (lwk == NULL) {
        return 0;
    }
    ewk = lwk->ewk;
    lwk->counter++;
    lwk->counter = (lwk->counter <= 0) ? 0 : ((lwk->counter > 180) ? 180 : lwk->counter);
    if (lwk->counter >= 180) {
        lwk->ganpon = 0.0f;
        lwk->riritu = 1.0f;
    }
    if (lwk->ralpha > 0) {
        if (lwk->ralpha == 128) {
            lwk->rcounter++;
            lwk->rcounter = (lwk->rcounter <= 0) ? 0 : ((lwk->rcounter > 60) ? 60 : lwk->rcounter);
            if (lwk->rcounter >= 60) {
                lwk->ralpha--;
                lwk->alpha = 255;
                lwk->ganpon *= lwk->riritu;
            }
        } else {
            lwk->ralpha -= 16;
            lwk->ralpha = (lwk->ralpha <= 0) ? 0 : ((lwk->ralpha > 128) ? 128 : lwk->ralpha);
        }
    }
    if (lwk->alpha > 128) {
        lwk->alpha -= 8;
        lwk->alpha = (lwk->alpha <= 128) ? 128 : ((lwk->alpha > 255) ? 255 : lwk->alpha);
    }
    if (ewk == NULL || lwk->num == 0) {
        return 0;
    }
    if (IsSceanStop()) {
        return 0;
    }
    lwk->za_cnt = 0;
    do {
        if (ewk->status == SBTLEXP_ST_PROC) {
            ewk->alpha -= 4;
            ewk->alpha = (ewk->alpha <= 0) ? 0 : ((ewk->alpha > 128) ? 128 : ewk->alpha);
            ewk->y -= 2;
            if (ewk->alpha <= 0) {
                ewk->status = SBTLEXP_ST_END;
            }
        }
        ewk = ewk->next;
    } while (ewk != NULL);
    return 1;
}

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlLeaf124);

s32 sbtlReleaseEXP_List(SBTLEXP_LISTWORK *lwk, u8 flg)
{
    SBTLEXP_WORK *dwk;
    SBTLEXP_WORK *next;

    if (lwk == NULL) {
        return 0;
    }
    dwk = lwk->ewk;
    if (dwk == NULL || lwk->num == 0) {
        return 0;
    }
    do {
        if (flg) {
            dwk->status = SBTLEXP_ST_END;
        }
        if (dwk->status == SBTLEXP_ST_END) {
            if (dwk->prev != NULL) {
                dwk->prev->next = dwk->next;
            }
            if (dwk->next != NULL) {
                dwk->next->prev = dwk->prev;
            }
            if (dwk->prev == NULL) {
                lwk->ewk = dwk->next;
                if (lwk->ewk != NULL) {
                    lwk->ewk->prev = NULL;
                }
            }
            next = dwk->next;
            sdfMemFreeDirect(dwk->handle);
            dwk = next;
            lwk->num--;
        } else {
            dwk = dwk->next;
        }
    } while (dwk != NULL);
    return 1;
}

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDeleteEXPList);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallBtlGetSkilltbl23);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlGetDamageSub2);

struct SBTLSKILLP_TBL *sbtlGetSkilltbl(SKLID skillid)
{
    struct SBTLSKILLP_TBL *tbl = NULL;

    if (sbtlCallBtlDrawRedGgData != NULL && (u32)skillid < 0x500) {
        tbl = &sbtlCallBtlDrawRedGgData[skillid];
    }
    return tbl;
}

SBTLSKLVISUAL_TBL *sbtlGetSklVisualTbl(SKLID sklid)
{
    return &sbtlGetSklVisualTblData[sklid];
}

s32 sbtlSetSkill_MotionData(struct SBTLUNIT_WORK *wk, SKLID sklid) {
    struct SBTLSKILLP_TBL *skl = sbtlGetSkilltbl(sklid);
    SBTLSKLVISUAL_TBL *vis = sbtlGetSklVisualTbl(sklid);

    if (wk_sbtl->battleMode == 3 && (skl->type & 0xFFF) == 8) {
        sbtlSetMotion_sbtlUnit(wk, 4, 0, 0, 0);
    } else if (sklid == SKLID_BS14_CSHO_R) {
        switch (wk->aiwk->scrStep3) {
        case 0:
            sbtlSetMotion_ID(wk, 17, 0, 0, 0);
            break;
        case 1:
            sbtlSetMotion_ID(wk, 16, 0, 0, 0);
            break;
        case -1:
            sbtlSetMotion_ID(wk, 15, 0, 0, 0);
            break;
        }
    } else if (skl->type & 0x1000) {
        if (vis->flg & 0x10) {
            sbtlSetMotion_sbtlUnit(wk, vis->motionno, 0, 0, 0);
            if (wk_sbtl->member_num > 0) {
                sbtlSetMotion_sbtlUnit(wk_sbtl->pNakama0, vis->motionno2, 0, 0, 0);
            }
        } else {
            sbtlSetMotion_sbtlUnit(wk, vis->motionno, 0, 0, 0);
            sbtlSetMotion_sbtlUnit(sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID), vis->motionno2, 0, 0, 0);
        }
    } else {
        sbtlSetMotion_sbtlUnit(wk, vis->motionno, 0, 0, 0);
        if (sklid == SKLID_FUKOSYUUJIRAIYA) {
            sbtlSetMotion_sbtlUnit(sbtlGetAnotherEnemy(wk), vis->motionno, 0, 0, 0);
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallBtlGetSklVisualTbl3);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlSetSkill_EffectData);

INCLUDE_ASM(const s32, "code_002DF1D0", CheckBadStatus_sbtlSkill);

s32 sbtlSetHatData(struct SBTLUNIT_WORK *wk, SKLID sklid)
{
    SBTLSKLVISUAL_TBL *tbl = sbtlGetSklVisualTbl(sklid);

    if (tbl->hatEffect != 0) {
        sbtlCheckEffectData(wk, tbl->hatEffect, 1, 0, wk->field414, -1, 0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlSetSkillData);

s32 sbtlCheckMHTData(struct SBTLUNIT_WORK *wk, SKLID sklid) {
    SBTLSKLVISUAL_TBL *tbl = sbtlGetSklVisualTbl(sklid);

    sbtlGetBadStatus(&wk->unit);
    if (tbl->flg & 4) {
        if (!CheckBadStatus_sbtlSkill(wk, sklid)) {
            sbtlSetMotionHit(wk, wk->next_motionno, sklid);
        } else {
            sbtlSetMotion_sbtlUnit(wk, 0, 0, 0, 0);
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlRefSbtl58);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlKettouProc);

INCLUDE_ASM(const s32, "code_002DF1D0", GetMessagePrio);

void DeleteMessage(SBTLSM_LISTWORK *lwk, SBTLSM_WORK *dwk)
{
    SBTLSM_WORK *prev;
    SBTLSM_WORK *next;

    if (dwk != NULL && lwk->num != 0) {
        prev = dwk->prev;
        next = dwk->next;
        sdfMemFreeDirect(dwk->handle);
        if (prev != NULL) {
            prev->next = next;
        }
        if (next != NULL) {
            next->prev = prev;
        }
        if (--lwk->num == 0) {
            lwk->dwk = NULL;
        } else if (prev == NULL) {
            lwk->dwk = next;
        }
    }
}

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlAddMessage);

SBTLSM_LISTWORK *sbtlCreateSMList(void)
{
    SBTLSM_LISTWORK *lwk = sdfChipAllocZeroDirect(sizeof(SBTLSM_LISTWORK));

    lwk->num = 0;
    lwk->dwk = NULL;
    return lwk;
}

SBTLSM_WORK *sbtlCreateSM(SBTLSM_LISTWORK *lwk, struct SBTLUNIT_WORK *wk, SBTLSM mode, SKLID sklid)
{
    sdfMemHandle_t *handle;
    SBTLSM_WORK *dwk;

    if (lwk->bLock) {
        return NULL;
    }
    handle = sdfMemAllocDirect(sizeof(SBTLSM_WORK));
    dwk = sdfMemLock(handle);
    memset(dwk, 0, sizeof(SBTLSM_WORK));
    dwk->handle = handle;
    dwk->prev = NULL;
    dwk->next = NULL;
    if (lwk->dwk == NULL) {
        lwk->dwk = dwk;
        lwk->num = 1;
    } else {
        sbtlAddMessage(lwk, dwk, mode);
    }
    if (wk != NULL) {
        dwk->listID = wk->field1A0;
    }
    dwk->mode = mode;
    if (wk != NULL) {
        dwk->raceid = wk->unit.raceId;
        dwk->devilid = wk->unit.devilId;
        if (sklid) {
            dwk->sklid = sklid;
        } else {
            dwk->sklid = wk->skillid;
        }
        dwk->side = wk->field414;
    }
    dwk->ofs_x = 0;
    dwk->ofs_y = 0;
    dwk->gstat = SBTLSM_ST_INIT;
    dwk->cmpcounter = 120;
    return dwk;
}

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallSprintf8);

s32 Proc(SBTLSM_LISTWORK *lwk)
{
    SBTLSM_WORK *dwk;

    if (lwk == NULL) {
        return 0;
    }
    dwk = lwk->dwk;
    if (dwk == NULL || lwk->num == 0) {
        return 0;
    }
    if (wk_sbtl->field42CA == 0) {
        if ((wk_sbtl->flg & 0x8000000) && dwk->mode != 71) {
            return 0;
        }
        if (wk_sbtl->flg2 & 0xA) {
            return 0;
        }
    }
    switch (dwk->gstat) {
    case SBTLSM_ST_INIT:
        dwk->alpha += 12;
        dwk->alpha = dwk->alpha > 0 ? (dwk->alpha < 129 ? dwk->alpha : 128) : 0;
        if (dwk->alpha >= 128) {
            dwk->gstat = SBTLSM_ST_PROC;
        }
        break;
    case SBTLSM_ST_PROC:
        if (lwk->bLock == 0) {
            dwk->counter++;
        }
        dwk->counter = dwk->counter > 0 ? (dwk->counter < dwk->cmpcounter ? dwk->counter : dwk->cmpcounter) : 0;
        if (dwk->counter >= dwk->cmpcounter) {
            DeleteMessage(lwk, dwk);
        }
        break;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_002DF1D0", Draw_sbtlSM);

s32 sbtlReleaseSM_List(SBTLSM_LISTWORK *lwk, u8 flg)
{
    SBTLSM_WORK *dwk;
    SBTLSM_WORK *next;

    if (lwk == NULL) {
        return 0;
    }
    dwk = lwk->dwk;
    if (dwk == NULL || lwk->num == 0) {
        return 0;
    }
    do {
        if (flg) {
            dwk->gstat = SBTLSM_ST_END;
        }
        if (dwk->gstat == SBTLSM_ST_END) {
            if (dwk->prev != NULL) {
                dwk->prev->next = dwk->next;
            }
            if (dwk->next != NULL) {
                dwk->next->prev = dwk->prev;
            }
            if (dwk->prev == NULL) {
                lwk->dwk = dwk->next;
                if (lwk->dwk != NULL) {
                    lwk->dwk->prev = NULL;
                }
            }
            next = dwk->next;
            sdfMemFreeDirect(dwk->handle);
            dwk = next;
            lwk->num--;
        } else {
            dwk = dwk->next;
        }
    } while (dwk != NULL);
    return 1;
}

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDeleteSMList);

s32 sbtlFunc20(s32 a_x, s32 a_y, SBTLSM_WORK *dwk)
{
    u32 color = dwk->alpha | 0xA09DC300;

    sbtlCallMessageNumSearch(a_x + dwk->ofs_x, a_y + dwk->ofs_y, 0, color, dwk->mode);
    return 1;
}

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallMessageNumSearch);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallMessageNumSearch2);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlLeaf120);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallMessageNumSearch4);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlLeaf94);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlBtlDebugInitSbattleSub);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDebug_CheckMainPanel);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDebug_CheckEffectEditor);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlSbtlDebug_CheckFreeCameraFlg);

INCLUDE_ASM(const s32, "code_002DF1D0", evtBEnterStateA);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDebug_Init);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlSbtldebugProc);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDebug_Draw);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDebug_SetDuummyData);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallDrawMiscFontAlpha);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDebug_DrawHPGauge);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlLeaf92);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallBtlCheckCrossCircle);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlLeaf89);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallSprintf4);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCmndCntST);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallSprintf5);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallBtlSearchUnitID36);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDebug_DrawTCursor);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlLeaf109);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallSprintf6);

INCLUDE_ASM(const s32, "code_002DF1D0", MainDraw);

INCLUDE_ASM(const s32, "code_002DF1D0", ModelSelectDraw);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlParaDraw);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlBSTDraw);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlMotionChngDraw);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlUnitChngDraw);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDrawUnitChangeList);

INCLUDE_ASM(const s32, "code_002DF1D0", sBtlDebug_GetDebugMenuFont);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallDestroyFontRequestAfter2);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlFunc25);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlVisualHitDraw);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlVisualDraw);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallChipAllocZeroDirect2);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallMemAllocDirect9);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDeleteFileList);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlMdlMoveEditorDraw);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlMdlHitEditorDraw);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlEffHitEditorDraw);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlEditDraw_MOVE);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlEditDraw);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDebug_HitTypeMoveDraw);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlLoadDraw);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlPBLoadDraw);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallBtlDebugDrawNormalListCol2);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallBtlDebugDrawNormalListCol3);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallBtlDebugDrawNormalListCol4);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlEPLLoadDraw);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallBtlDebugDrawNormalListCol5);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallBtlDebugDrawNormalListCol6);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallBtlDebugDrawNormalListCol7);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallBtlDebugDrawNormalListCol8);

INCLUDE_ASM(const s32, "code_002DF1D0", Calc_AW_GunTarget);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlFunc27);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlFunc22);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlEffHitEditorDraw_Functbl_Cylinder);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlEffHitEditorDraw_Functbl_Sphere);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlEffHitEditorDraw_Functbl_Ring);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlMDLHitEditorDraw_Functbl_Sphere);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlMDLHitEditor_DrawMotion);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlMDLHitEditorDraw_Functbl_Cylinder);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlMDLHitEditorDraw_Functbl_Ring);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDebug_EPLParaDraw);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDebug_MoveTypeDraw);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlEPLDraw);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDebug_EffHitPathDraw);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlNEFMODEFuncEditSub);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDrawPoseture);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDebug_DrawClrcle_Line);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDebug_EffHitIdou);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDebug_EffHitDraw);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDebug_MDLHitDraw);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallDrwZLoad);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallDrwZLoad2);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDebug_DrawHitCircle);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDrawString);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDrawFloat_Num);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDrawInt_Num);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlSousaDraw);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlSkillChangeDraw);

INCLUDE_ASM(const s32, "code_002DF1D0", ModelMenuDraw);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallBtlDebugDrawONOFFList);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallBtlDebugDrawONOFFList2);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlEffEditorDraw);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlGetUnit);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlTgtLock);

INCLUDE_ASM(const s32, "code_002DF1D0", SceneDel_Calc);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallBtlDebugCheckLoadEncountData);

INCLUDE_ASM(const s32, "code_002DF1D0", MainProc);

INCLUDE_ASM(const s32, "code_002DF1D0", ModelSelectProc);

INCLUDE_ASM(const s32, "code_002DF1D0", ModelMenuProc);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlSkillChangeProc);

INCLUDE_ASM(const s32, "code_002DF1D0", MotionChngProc);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlParaProc);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlSetUnitPara);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlBSTProc);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlUnitChngProc);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDebug_SetVisualData);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlVisualHitProc);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlVisualProc);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlMdlMoveEditorProc);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlMdlHitEditorProc);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlEffHitEditorProc);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlNEFMODE_Func_Open);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlMDLMODE_Func_Open);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDebug_MOVEMODE_Func_Open);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDebug_LoadMotionMoveData);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDebug_LoadMHTData);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCheckOpenMenu);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlResetMotionMovePos);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlBtlDebugLoadMHTDataSub);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlNEFMODE_Func_Save);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlNEFMODE_Func_Load);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallMemset27);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlBattleHitpath);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlPacPBFile);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlBattleBed);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallFileCheckEnd);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlSetPathMotion);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlResetEPLData);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCreateUnit2);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlSetPreViewModel);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDeletePrevModel);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlNEFMODE_Func_PageList);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlMOVEMODE_Func_PageList);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlMDLMODE_Func_PageList);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlAddPrev);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDecPrev);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlClearEffHitData);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlLeaf108);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlLeaf95);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlSizeofData);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlFunc28);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCallRestartBattle);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlFunc17);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlEPLDataDelete);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlPATHDataDelete);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlCopyData);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDebug_ChangeModel);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlSetModelData);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlMOVEMODE_Func_Edit);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlSetMotionMoveData_Buffer);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlMDLMODE_Func_Edit);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlNEFMODE_Func_Edit);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlSetEffHitPara_S);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlSetEffHitPara_sbtl);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlDebug_SetMDLMOVEPara);

INCLUDE_ASM(const s32, "code_002DF1D0", sbtlGetMotionNum);
