#include "common.h"
#include "dds3Basic.h"
#include "dds3obj.h"
#include "dds3WorldBasic.h"
#include "dds3SceneBasic.h"
#include "dds3CameraObjectBasic.h"
#include "dds3ObjectBase.h"
#include "sbtlWork.h"
#include "sBtlUnit.h"
#include "sBtlEff.h"
#include "sBtlSound.h"
#include "sBtlScript.h"
#include "sbtlDMG.h"
#include "sBtlCalc.h"
#include "sBtlSM.h"
#include "sBtlItem.h"
#include "sdatUnit.h"
#include "sdfMemory.h"
#include "sdfChip.h"
#include "mdlManager.h"
#include "code_r1_0021CE60.h"
#include "sdfDelayedResrc.h"
#include "libsdf.h"
#include "fileManager.h"
#include "sdfFile.h"
#include "dds3Randomize.h"
#include "sdfSound.h"
#include "sbtlSub.h"
#include "sbtlSub_0027ED08.h"
#include "sdfQuat.h"
#include "sdfModel.h"
#include "sBtlSkill.h"
#include "sBtlAI.h"
#include "sBtlDvltk.h"
#include "sbtlActAI.h"
#include "evtUnitManager.h"
#include "dds3UnitObjectBasic.h"
#include "sdk_libc.h"
#include "fpu.h"
#include "ps2asm.h"
#include "dds3KernelCore.h"
#include "dds3KernelDraw.h"
#include "dds3KernelMain.h"
#include "fld.h"
#include "sBtlPlayer.h"
#include "dds3PadManager.h"
#include "scrScriptProcess.h"
#include "itfMesManager.h"
#include "sBtlTalk.h"
#include "sdfVu0.h"
#include "newdata.h"
#include "code_r1_0027CBE8.h"
#include "sBtlPanel.h"
#include "AW_XAtkPad.h"
#include "EventBit.h"
#include "fldProcess.h"
#include "sdfMath.h"
#include "sbtlTubeSupport.h"
#include "sdfSystem.h"
#include "sBtlEncount.h"
#include "sBtlVisual.h"
#include "sBtlDraw.h"
#include "itf.h"
#include "code_r1_001CC1D0.h"
#include "evtMoon.h"
#include "frFont.h"
#include "sBtlEXP.h"
#include "sBtlMenu.h"
#include "datCalc.h"
#include "sbtlDebugStr.h"
#include "sbattle.h"
#include "code_002A38D8.h"
#include "sdfDevCons.h"
#include "sdfDraw.h"
#include "sdfPacketBuff.h"
#include "kernel_draw.h"
#include "sdf_Decomp.h"
#include "billboard.h"

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

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlSearchUnitID32);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlSearchUnitID33);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlSearchUnitID30);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlGetExpHosei);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlGetDevilDataSBtlSub3);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallMemAllocDirect8);

INCLUDE_ASM(const s32, "code_002A38D8", DecVice);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlSetVicePara);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlResetVicePara);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlSearchUnitID27);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlSetRirituNakamaHD);

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
    if (wk_sbtl->flg & 0x40) {
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

/* body: matched in raidou @ 61f09dc: src/raidou1/code_002929C8.c:124, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
s32 sbtlLeaf124(void) {
    return 1;
}

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

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDeleteEXPList);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlGetDamageSub2);

struct SBTLSKILLP_TBL *sbtlGetSkilltbl(SKLID skillid)
{
    return &sbtlCallBtlDrawRedGgData[skillid];
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
    } else if (sklid == 465) {
        switch (wk->aiwk->scrStep3) {
        case 0:
            sbtlCallBtlGetMotionVisualData(wk, 17, 0, 0, 0);
            break;
        case 1:
            sbtlCallBtlGetMotionVisualData(wk, 16, 0, 0, 0);
            break;
        case -1:
            sbtlCallBtlGetMotionVisualData(wk, 15, 0, 0, 0);
            break;
        }
    } else if (skl->type & 0x1000) {
        if (vis->flg & 0x10) {
            sbtlSetMotion_sbtlUnit(wk, vis->motionno, 0, 0, 0);
            if (wk_sbtl->field344 > 0) {
                sbtlSetMotion_sbtlUnit(sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[wk_sbtl->summonIndex[0]]),
                                       vis->motionno2, 0, 0, 0);
            }
        } else {
            sbtlSetMotion_sbtlUnit(wk, vis->motionno, 0, 0, 0);
            sbtlSetMotion_sbtlUnit(sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID), vis->motionno2, 0, 0, 0);
        }
    } else {
        sbtlSetMotion_sbtlUnit(wk, vis->motionno, 0, 0, 0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlGetSklVisualTbl3);

s32 sbtlCallBtlGetSklVisualTbl2(struct SBTLUNIT_WORK *wk, SKLID sklid) {
    SBTLSKLVISUAL_TBL *tbl = sbtlGetSklVisualTbl(sklid);

    sbtlCallBtlGetSkilltbl8(wk, 0x400);
    if (CheckBadStatus_sbtlSkill(wk, sklid)) {
        return 0;
    }
    if (tbl->SABno != 0 && sklid != 149) {
        sbtlCallBtlSearchEffectData(wk, sklid, 1, 0, wk->field414, -1, 0);
    }
    return 1;
}

/* NON_MATCHING: 25 of 51 words match. Retail keeps ok in s2 across the call and tests 164..166 with two slti; cc1 folds ours into one range test. */
#ifdef NON_MATCHING
s32 CheckBadStatus_sbtlSkill(struct SBTLUNIT_WORK *wk, SKLID sklid) {
    s32 ok = 0;
    struct SBTLSKILLP_TBL *tbl = sbtlGetSkilltbl(sklid);

    switch (tbl->type & 0xFFF) {
    case 1:
    case 3:
    case 4:
    case 5:
    case 7:
        break;
    default:
        return 0;
    }
    if (sbtlGetBadStatus(&wk->unit) == BST_MOUDOKU) {
        if (sklid < 144) {
            ok = 1;
        } else if (sklid < 150) {
            ok = 0;
        } else if (sklid >= 167 || sklid < 164) {
            ok = 1;
        } else {
            ok = 0;
        }
        if (tbl->type & 0x1000) {
            ok = 0;
        }
    }
    return ok;
}
#else
INCLUDE_ASM(const s32, "code_002A38D8", CheckBadStatus_sbtlSkill);
#endif

s32 sbtlSetHatData(struct SBTLUNIT_WORK *wk, SKLID sklid)
{
    SBTLSKLVISUAL_TBL *tbl = sbtlGetSklVisualTbl(sklid);

    if (tbl->hatEffect != 0) {
        sbtlCallBtlSearchEffectData(wk, tbl->hatEffect, 1, 0, wk->field414, -1, 0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_002A38D8", sbtlSetSkillData);

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

INCLUDE_ASM(const s32, "code_002A38D8", GetMessagePrio);

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

INCLUDE_ASM(const s32, "code_002A38D8", sbtlAddMessage);

SBTLSM_LISTWORK *sbtlCreateSMList(void)
{
    SBTLSM_LISTWORK *lwk = sdfChipAllocZeroDirect(sizeof(SBTLSM_LISTWORK));

    lwk->num = 0;
    lwk->dwk = NULL;
    return lwk;
}

SBTLSM_WORK *sbtlCreateSM(SBTLSM_LISTWORK *lwk, struct SBTLUNIT_WORK *wk, SBTLSM mode, SKLID sklid)
{
    sdfMemHandle_t *handle = sdfMemAllocDirect(sizeof(SBTLSM_WORK));
    SBTLSM_WORK *dwk = sdfMemLock(handle);

    memset(dwk, 0, sizeof(SBTLSM_WORK));
    dwk->prev = NULL;
    dwk->next = NULL;
    dwk->handle = handle;
    if (lwk->dwk == NULL) {
        lwk->dwk = dwk;
        lwk->num = 1;
    } else {
        sbtlAddMessage(lwk, dwk, mode);
    }
    dwk->mode = mode;
    dwk->listID = wk->field1A0;
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

/* NON_MATCHING: 97 of 109 words match. Retail does not tail-call frSetVariableRequest in the default (devil name) case. */
#ifdef NON_MATCHING
void SetVar_sbtlSM(SBTLSM_WORK *dwk)
{
    switch (dwk->mode) {
    case 10:
    case 11:
    case 45:
    case 46:
    case 47:
    case 62:
    case 66:
    case 70:
        sprintf(dwk->fontbuff, "%s", sbtlData95[dwk->sklid]);
        frSetVariableRequest(0, EE_ADDR_S(dwk->fontbuff));
        break;
    case 51:
    case 52:
        if (dwk->raceid >= 8) {
            sprintf(dwk->fontbuff, "%s", sbtlData94[dwk->raceid + 24]);
        } else {
            sprintf(dwk->fontbuff, "%s", sbtlData94[dwk->raceid]);
        }
        frSetVariableRequest(0, EE_ADDR_S(dwk->fontbuff));
        sprintf(dwk->fontbuff2, "%s", g_itemNo[dwk->devilid]);
        frSetVariableRequest(1, EE_ADDR_S(dwk->fontbuff2));
        break;
    default:
        if (!(dwk->side & 0x80)) {
            sprintf(dwk->fontbuff, "%s", g_itemNo[dwk->devilid]);
            frSetVariableRequest(0, EE_ADDR_S(dwk->fontbuff));
        }
        break;
    }
}
#else
INCLUDE_ASM(const s32, "code_002A38D8", SetVar_sbtlSM);
#endif

s32 sbtlCallDeleteMessage(SBTLSM_LISTWORK *lwk)
{
    SBTLSM_WORK *dwk;

    if (lwk == NULL) {
        return 0;
    }
    dwk = lwk->dwk;
    if (dwk == NULL || lwk->num == 0) {
        return 0;
    }
    if ((wk_sbtl->flg & 0x8000000) && dwk->mode != 71) {
        return 0;
    }
    if (wk_sbtl->flg2 & 0xA) {
        return 0;
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
        dwk->counter++;
        dwk->counter = dwk->counter > 0 ? (dwk->counter < dwk->cmpcounter ? dwk->counter : dwk->cmpcounter) : 0;
        if (dwk->counter >= dwk->cmpcounter) {
            DeleteMessage(lwk, dwk);
        }
        break;
    }
    return 1;
}

/* NON_MATCHING: 57 of 61 words match. The num guard takes beql where retail has beq; 1 word (that beql) once SetVar_sbtlSM is C in the unit. */
#ifdef NON_MATCHING
s32 sbtlCallSetVarSBtlSM(SBTLSM_LISTWORK *lwk)
{
    SBTLSM_WORK *dwk;

    if (wk_sbtl->panel != NULL && wk_sbtl->panel->hPanelSpr3 != NULL && lwk != NULL) {
        dwk = lwk->dwk;
        if (dwk != NULL && lwk->num != 0) {
            if (!dds3BgSetScrollSub()) {
                if ((wk_sbtl->flg & 0x8000000) && dwk->mode != 71 && dwk->mode != 11 && dwk->sklid != 240 &&
                    dwk->devilid < 80) {
                    return 0;
                }
                if (wk_sbtl->flg2 & 0xA) {
                    return 0;
                }
            }
            switch (dwk->gstat) {
            case SBTLSM_ST_INIT:
            case SBTLSM_ST_PROC:
                SetVar_sbtlSM(dwk);
                sbtlFunc20(256, 68, dwk);
                break;
            }
            return 1;
        }
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallSetVarSBtlSM);
#endif

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

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDeleteSMList);

s32 sbtlFunc20(s32 a_x, s32 a_y, SBTLSM_WORK *dwk)
{
    u32 color = dwk->alpha | 0xA09DC300;

    sbtlCallMessageNumSearch4(a_x + dwk->ofs_x, a_y + dwk->ofs_y, 0, color, dwk->mode);
    return 1;
}

s32 sbtlCallMessageNumSearch4(s32 a_x, s32 a_y, s32 a_z, u32 a_color, SBTLSM a_mode)
{
    sprHandle_t *hSpr = wk_sbtl->panel->hPanelSpr3;
    u8 *msg = MessageNumSearch(a_mode, sbtlCallMessageNumSearch4Data, 0);
    frFontHandle_t *frq;
    s32 width;
    s32 half;
    s32 x;

    if (msg == NULL) {
        return 0;
    }
    frq = frCallMessageRequestEx2(0, 0, a_z, 0, 0, 0, 0, msg, NULL, -1);
    width = frCallGetStrWidth3(0, frq);
    half = width >> 4;
    x = 256 - (width >> 5);
    hSpr->descriptions[11].rect[3] = half;
    itfCallSetOt2(x - 24, a_y - 7, 0, 255, hSpr, 11, 103);
    itfCallSetOt2(x + half, a_y - 6, 0, 255, hSpr, 15, 103);
    frFontMoveChainTo(x * 16, a_y * 8, frq);
    frSetFontColor(frq, 0xA09DC380);
    frDrawFontEx(frq, 1);
    frDestroyFontRequestAfter(frq);
    return 1;
}

/* body: matched in smt3 @ 9401f79eb: src/smt3/kernel/dds3SequenceList.c:914, match, same function 1.00 (words 18/18, calls 1.00, shape 1.00) */
void sbtlBtlDebugInitSbattleSub(const char *format, ...)
{
}

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_CheckMainPanel);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlLeaf111);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlSbtlDebug_CheckFreeCameraFlg);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrGreenblueEht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrRTestEht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrTestEht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr001CEht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr001BEht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr001AEht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr001Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr001AaEht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrSword06Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrSword05Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrSword04Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrSword03Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrSword02Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrSword01Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrSword01AEht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrToushaEht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrMakizoeEht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr0F9AEht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrKaitenEht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrAgiball02Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrAgiball01Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrAgiball00Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPbBugEht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrRndRotHitEht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrMaphit00Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrJCou06Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrJCou05Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrJCou04Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrJCou03Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrJCou02Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrJCou01Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrHuheuh00Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrMudoEht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrB12Hit012Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrB12Hit011Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrB12Hit010Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrBlock00Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrIBreathEht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrFBreathEht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrZanball02Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrZanball01Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrZanball00Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrBlood00Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrJRange03Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrJRang015Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrJRange02Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrJRang003Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrBuhmain01Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrBuhball01Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrLaser00Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr1Fb02Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPet3Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPet2Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPet1Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPet0Eht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrTestPassPb);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPathRotRightPb);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrAgInEpl);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrAgiTobEpl);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPlayerAll);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPlayer3);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPlayer2);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPlayer1);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrMahoujin);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrSeDebug);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrSeDouji);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrSeDistance);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrBststDraw);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrDamageS);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrCmndPanel);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrFaceIcon);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrCombination);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrNoResult);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrEnemyStop);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrInterface);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrCameraLock);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrCameraMove);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrModelmoveEditor);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrModelhitEditor);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrEffhitEditor);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrAllDead);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrEffectEditor);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrDebugMenuKari);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrInterfaceOff);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrBattleMode);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrEncountNo);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrEncountNo2);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrParaReset);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPositionReset);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrEquipSordno);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrEquipGunNo);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrMashingun);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrDanganNumChk);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrAiPanel);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrUnitAreaEnmy);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrUnitAreaFrid);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrUnitAreaPriv);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrUnitAreaHit);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrHitDrawMode);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr11);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr12);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrAi);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr13);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr14);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr15);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr16);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr17);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr18);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrUnitChange);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrMotionChange);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr26Reserve);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr25Reserve);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr24Reserve);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr23Reserve);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr22Reserve);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr21Reserve);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr20Reserve);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr19Reserve);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr18Reserve);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr17Reserve);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr16Reserve);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr152);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr112);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr102);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr09);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr08);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr06);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr05);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr04);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr03Reserve);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr02Loop);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr01Loop);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr00Loop);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr182);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr172);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr162);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr153);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr142);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr132);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr122);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr113);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr03);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr02);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr01);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr39);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr38);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr37);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr36);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr35);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr34);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr33);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr32);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr31);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr30);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr29);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr28);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr27);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr25);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr24);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr23);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr22);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr21);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr20);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr19);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr183);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr154);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr14Reserve);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr133);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr114);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr10Reserve);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr092);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr082);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr07);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr06Reserve);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr052);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr042);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr032);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr022);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr012);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrCylinder);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrECenter);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrECylinder);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrDefMvspd);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrDefRSpd);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrDefMSpd);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrCancelFrm);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrHokanFrm);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrHitRadian);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrMotionSpd);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrEndMotion);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrStopFrm);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrActionFrm);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrMotionPara);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrEffcySave);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrVJI);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr2601Reserve);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr25019Reserve);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr24018Reserve);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr23017Reserve);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr22016Reserve);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr21015Reserve);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr20014Reserve);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr19013Reserve);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr18012Reserve);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr17011Reserve);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr16010Reserve);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr1500);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr1400A);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr1300);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr1200S);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr1100K);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr1000);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr09009);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr08008X);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr07007);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr06006AN);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr05005W);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr04004W);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr03003Reserve);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr02002XLoop);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr01001Loop);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr00000Loop);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr012C);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr011C);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr010AE);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr00);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr00EA);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr00E);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr002);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr004);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr009);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr0042);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr003U);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr002X);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr001W);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr000);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrOJEffectMotionIVLZ);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrVA);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr26);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrAO);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrGFN);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrSV);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr40);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrXP);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrTCYV);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr027K);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr026K);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr025K);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr024K);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr023K);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr022RVU);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr021);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr020);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr01F);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr01EZ);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr01D3);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr01C2);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr01B1);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr01A);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr019C);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr018C);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr017C);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr016);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr015RBAY);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr014RAPX);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr013);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr0122);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr011A);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr010);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr00FAC);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr00DI);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr00C);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr00BK);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr0092);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr008);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr007);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr006);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr005W);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr004W);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr003XX);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr002XX);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr001);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrModelchange);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPosReset);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrSave);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrNew);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrSaveMht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrLoadEht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrSaveEht);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPageList);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPage16);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPage15);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPage14);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPage13);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPage12);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPage11);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPage10);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPage09);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPage08);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPage07);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPage06);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPage05);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPage04);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPage03);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPage02);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPage01);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrNewPage);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrDelete);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrCopy);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrEdit);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrDeleteOk);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrExitOk);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrCylinder2);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrMtionno);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPoseture);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrGravity);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrBehaviorType);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPathPb);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrBezier);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrAttachEpl);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrEplMenu);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrIxTH);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrCombibH);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrCombiaH);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrTpartyH);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrIpartyH);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrWorldH);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrTargetH);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrInvorkH);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrAllH);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrIxT);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrCombib);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrCombia);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrTparty);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrIparty);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrWorld);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrTarget);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrInvork);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrAll);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrHittype);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrHomingpara);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrKasokudo);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrHitbkPara);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrHitbkLife);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrKakudoRl);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrDirFlg);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrStTime);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrHelperno);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPosition);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrStartPos);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrTargetpos);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrInterpo1);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrInterpo2);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrSbtldebugProc);

INCLUDE_ASM(const s32, "code_002A38D8", evtBEnterStateA);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_Init);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_Init_sbtl);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_Draw);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlGetPlayerData5);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallDrawMiscFontAlpha);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_DrawHPGauge);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlLeaf92);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlCheckCrossCircle);

/* body: matched in raidou @ 61f09dc: src/raidou1/code_002929C8.c:124, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
s32 sbtlLeaf89(void) {
    return 1;
}

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlLeaf92Table);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallSprintf4);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCmndCntST);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallSprintf5);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlSearchUnitID36);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_DrawTCursor);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlLeaf109);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallSprintf6);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlFunc26);

INCLUDE_ASM(const s32, "code_002A38D8", ModelSelectDraw);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlParaDraw);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlBSTDraw);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlDebug_DrawTCursorTable);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlParaDrawTable);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlFRAME);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlUnitChngDraw);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDrawUnitChangeList);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlLeaf114);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallDestroyFontRequestAfter2);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlFunc25);

s32 sbtlLeaf102(dds3ProcessID_t *id)
{
    return 1;
}

INCLUDE_ASM(const s32, "code_002A38D8", sbtlVisualDraw);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallChipAllocZeroDirect2);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallMemAllocDirect9);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDeleteFileList);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlVisualDrawTable);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlVisualDrawTable2);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrFrame4141);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrFrame24141);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlDebugDrawNormalListCol);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrBattleMdlhit);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrHiteffclr);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrMSttime);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrHelperloc);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlBattleMdlhitTable);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlBattleMdlhit);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrBattleEffhit);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlBattleEffhitTable);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlBattleEffhit);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlEditDraw_MOVE);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlEditDraw);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_HitTypeMoveDraw);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlLoadDraw);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlPBLoadDraw);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlDebugDrawNormalListCol2);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlDebugDrawNormalListCol3);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlDebugDrawNormalListCol4);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlEPLLoadDraw);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlDebugDrawNormalListCol5);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlDebugDrawNormalListCol6);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlDebugDrawNormalListCol7);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlDebugDrawNormalListCol8);

INCLUDE_ASM(const s32, "code_002A38D8", Calc_AW_GunTarget);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlFunc27);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlFunc22);

s32 sbtlEffHitEditorDraw_Functbl_Cylinder(SBDEBUG_CSRWORK *wk, SBTLSKILLHIT_TBL *hwk, SBTLSKILLHIT_POSWORK *para,
                                          SBTLSKILLIDOU_WORK *iwk, struct SBDEBUG_PATHDATA *path) {
    char *str = sbtlData109;

    fldLmapSubmitScaledSpritePacket(0, 320, 0, 244, 52, 0x60000000, 0x40804040, 103);
    sbtlDebug_DrawNormalList_Col(sblist_efhitmenu_edit_para00, 4, wk->point - 1, -1, 14, 42);
    sbtlDrawFloat_Num(182, 40, hwk->area_r, wk->point, 1);
    sbtlDrawFloat_Num(182, 52, hwk->cy.height, wk->point, 2);
    sbtlDrawFloat_Num(182, 64, hwk->rad, wk->point, 3);
    sbtlDrawInt_Num(182, 76, hwk->lifetime, wk->point, 4);
    fldLmapSubmitScaledSpritePacket(0, 768, 0, 244, 16, 0x60000000, 0x40804040, 103);
    sbtlDebug_DrawNormalList_Col(sblist_efhitmenu_edit_para01, 1, wk->point - 5, -1, 14, 98);
    sbtlDrawString(160, 96, str, wk->point, 5);
    fldLmapSubmitScaledSpritePacket(0, 896, 0, 244, 76, 0x60000000, 0x40804040, 103);
    sbtlDebug_DrawNormalList_Col(sblist_efhitmenu_edit_para02, 6, wk->point - 6, -1, 14, 114);
    sbtlDrawFloat_Num(182, 112, hwk->dir, wk->point, 6);
    sbtlDrawFloat_Num(182, 124, hwk->shatei, wk->point, 7);
    sbtlDrawFloat_Num(182, 136, hwk->sokudo, wk->point, 8);
    sbtlDrawFloat_Num(182, 148, hwk->kasokudo, wk->point, 9);
    sbtlDrawFloat_Num(182, 172, hwk->homingpara, wk->point, 11);
    fldLmapSubmitScaledSpritePacket(0, 1728, 0, 244, 64, 0x60000000, 0x40804040, 103);

    switch (sbtlData62.ef_stendp) {
    case 0:
        if (hwk->flg & 0x100) {
            sbtlDebug_DrawNormalList_Col(sblist_effhitemenu03, 5, wk->point - 12, 0, 14, 218);
            sbtlDrawString(160, 216, str, wk->point, 12);
        } else {
            sbtlDebug_DrawNormalList_Col(sblist_effhitemenu03_0, 5, wk->point - 12, 0, 14, 218);
        }
        sbtlDrawString(144, 228, sblist_efhitmenu_edit_para06_00[para->iti], wk->point, 13);
        sbtlDrawInt_Num(182, 240, para->iti_no, wk->point, 14);
        sbtlDrawInt_Num(182, 252, para->takasa, wk->point, 15);
        sbtlDrawInt_Num(182, 264, para->muki, wk->point, 16);
        break;
    case 1:
        sbtlDebug_DrawNormalList_Col(sblist_effhitemenu03_a1, 5, wk->point - 12, 0, 14, 218);
        sbtlDrawString(160, 216, str, wk->point, 12);
        sbtlDrawFloat_Num(182, 228, iwk->per, wk->point, 13);
        sbtlDrawFloat_Num(182, 240, iwk->w, wk->point, 14);
        sbtlDrawFloat_Num(182, 252, iwk->h, wk->point, 15);
        break;
    case 2:
        sbtlDebug_DrawNormalList_Col(sblist_effhitemenu03_a2, 5, wk->point - 12, 0, 14, 218);
        sbtlDrawString(160, 216, str, wk->point, 12);
        sbtlDrawFloat_Num(182, 228, iwk->per, wk->point, 13);
        sbtlDrawFloat_Num(182, 240, iwk->w, wk->point, 14);
        sbtlDrawFloat_Num(182, 252, iwk->h, wk->point, 15);
        break;
    case 3:
        sbtlDebug_DrawNormalList_Col(sblist_effhitemenu04, 5, wk->point - 12, 0, 14, 218);
        sbtlDrawString(160, 216, str, wk->point, 12);
        sbtlDrawString(144, 228, sblist_efhitmenu_edit_para06_00[para->iti], wk->point, 13);
        sbtlDrawInt_Num(182, 240, para->iti_no, wk->point, 14);
        sbtlDrawInt_Num(182, 252, para->takasa, wk->point, 15);
        sbtlDrawInt_Num(182, 264, para->muki, wk->point, 16);
        break;
    }
    sbtlDebug_EPLParaDraw(wk, hwk, para, iwk, 17);
    sbtlDebug_MoveTypeDraw(wk, hwk, para, iwk, path, 22);
    return 1;
}

s32 sbtlEffHitEditorDraw_Functbl_Sphere(SBDEBUG_CSRWORK *wk, SBTLSKILLHIT_TBL *hwk, SBTLSKILLHIT_POSWORK *para,
                                          SBTLSKILLIDOU_WORK *iwk, struct SBDEBUG_PATHDATA *path) {
    char *str = sbtlData109;

    fldLmapSubmitScaledSpritePacket(0, 320, 0, 244, 52, 0x60000000, 0x40804040, 103);
    sbtlDebug_DrawNormalList_Col(sblist_efhitmenu_edit_para00_0, 4, wk->point - 1, -1, 14, 42);
    sbtlDrawFloat_Num(182, 40, hwk->area_r, wk->point, 1);
    sbtlDrawFloat_Num(182, 52, hwk->cy.height, wk->point, 2);
    sbtlDrawFloat_Num(182, 64, hwk->rad, wk->point, 3);
    sbtlDrawInt_Num(182, 76, hwk->lifetime, wk->point, 4);
    fldLmapSubmitScaledSpritePacket(0, 768, 0, 244, 16, 0x60000000, 0x40804040, 103);
    sbtlDebug_DrawNormalList_Col(sblist_efhitmenu_edit_para01, 1, wk->point - 5, -1, 14, 98);
    sbtlDrawString(160, 96, str, wk->point, 5);
    fldLmapSubmitScaledSpritePacket(0, 896, 0, 244, 88, 0x60000000, 0x40804040, 103);
    sbtlDebug_DrawNormalList_Col(sblist_efhitmenu_edit_para02_0, 7, wk->point - 6, -1, 14, 114);
    sbtlDrawFloat_Num(182, 112, hwk->dir, wk->point, 6);
    sbtlDrawFloat_Num(182, 124, hwk->cy.xrad, wk->point, 7);
    sbtlDrawFloat_Num(182, 136, hwk->shatei, wk->point, 8);
    sbtlDrawFloat_Num(182, 148, hwk->sokudo, wk->point, 9);
    sbtlDrawFloat_Num(182, 160, hwk->kasokudo, wk->point, 10);
    sbtlDrawFloat_Num(182, 184, hwk->homingpara, wk->point, 12);
    fldLmapSubmitScaledSpritePacket(0, 1728, 0, 244, 64, 0x60000000, 0x40804040, 103);

    switch (sbtlData62.ef_stendp) {
    case 0:
        if (hwk->flg & 0x100) {
            sbtlDebug_DrawNormalList_Col(sblist_effhitemenu03, 5, wk->point - 13, 0, 14, 218);
            sbtlDrawString(160, 216, str, wk->point, 13);
        } else {
            sbtlDebug_DrawNormalList_Col(sblist_effhitemenu03_0, 5, wk->point - 13, 0, 14, 218);
        }
        sbtlDrawString(144, 228, sblist_efhitmenu_edit_para06_00[para->iti], wk->point, 14);
        sbtlDrawInt_Num(182, 240, para->iti_no, wk->point, 15);
        sbtlDrawInt_Num(182, 252, para->takasa, wk->point, 16);
        sbtlDrawInt_Num(182, 264, para->muki, wk->point, 17);
        break;
    case 1:
        sbtlDebug_DrawNormalList_Col(sblist_effhitemenu03_a1, 5, wk->point - 13, 0, 14, 218);
        sbtlDrawString(160, 216, str, wk->point, 13);
        sbtlDrawFloat_Num(182, 228, iwk->per, wk->point, 14);
        sbtlDrawFloat_Num(182, 240, iwk->w, wk->point, 15);
        sbtlDrawFloat_Num(182, 252, iwk->h, wk->point, 16);
        break;
    case 2:
        sbtlDebug_DrawNormalList_Col(sblist_effhitemenu03_a2, 5, wk->point - 13, 0, 14, 218);
        sbtlDrawString(160, 216, str, wk->point, 13);
        sbtlDrawFloat_Num(182, 228, iwk->per, wk->point, 14);
        sbtlDrawFloat_Num(182, 240, iwk->w, wk->point, 15);
        sbtlDrawFloat_Num(182, 252, iwk->h, wk->point, 16);
        break;
    case 3:
        sbtlDebug_DrawNormalList_Col(sblist_effhitemenu04, 5, wk->point - 13, 0, 14, 218);
        sbtlDrawString(160, 216, str, wk->point, 13);
        sbtlDrawString(144, 228, sblist_efhitmenu_edit_para06_00[para->iti], wk->point, 14);
        sbtlDrawInt_Num(182, 240, para->iti_no, wk->point, 15);
        sbtlDrawInt_Num(182, 252, para->takasa, wk->point, 16);
        sbtlDrawInt_Num(182, 264, para->muki, wk->point, 17);
        break;
    }
    sbtlDebug_EPLParaDraw(wk, hwk, para, iwk, 18);
    sbtlDebug_MoveTypeDraw(wk, hwk, para, iwk, path, 23);
    return 1;
}

s32 sbtlEffHitEditorDraw_Functbl_Ring(SBDEBUG_CSRWORK *wk, SBTLSKILLHIT_TBL *hwk, SBTLSKILLHIT_POSWORK *para,
                                          SBTLSKILLIDOU_WORK *iwk, struct SBDEBUG_PATHDATA *path) {
    char *str = sbtlData109;

    fldLmapSubmitScaledSpritePacket(0, 320, 0, 244, 64, 0x60000000, 0x40804040, 103);
    sbtlDebug_DrawNormalList_Col(sblist_efhitmenu_edit_para00_1, 5, wk->point - 1, -1, 14, 42);
    sbtlDrawFloat_Num(182, 40, hwk->area_r, wk->point, 1);
    sbtlDrawFloat_Num(182, 52, hwk->area_r2, wk->point, 2);
    sbtlDrawFloat_Num(182, 64, hwk->cy.height, wk->point, 3);
    sbtlDrawFloat_Num(182, 76, hwk->rad, wk->point, 4);
    sbtlDrawInt_Num(182, 88, hwk->lifetime, wk->point, 5);
    fldLmapSubmitScaledSpritePacket(0, 832, 0, 244, 16, 0x60000000, 0x40804040, 103);
    sbtlDebug_DrawNormalList_Col(sblist_efhitmenu_edit_para01, 1, wk->point - 6, -1, 14, 106);
    sbtlDrawString(160, 106, str, wk->point, 6);
    fldLmapSubmitScaledSpritePacket(0, 960, 0, 244, 88, 0x60000000, 0x40804040, 103);
    sbtlDebug_DrawNormalList_Col(sblist_efhitmenu_edit_para02_0, 7, wk->point - 7, -1, 14, 122);
    sbtlDrawFloat_Num(182, 120, hwk->dir, wk->point, 7);
    sbtlDrawFloat_Num(182, 132, hwk->cy.xrad, wk->point, 8);
    sbtlDrawFloat_Num(182, 144, hwk->shatei, wk->point, 9);
    sbtlDrawFloat_Num(182, 156, hwk->sokudo, wk->point, 10);
    sbtlDrawFloat_Num(182, 168, hwk->kasokudo, wk->point, 11);
    sbtlDrawFloat_Num(182, 192, hwk->homingpara, wk->point, 13);
    fldLmapSubmitScaledSpritePacket(0, 1728, 0, 244, 64, 0x60000000, 0x40804040, 103);

    switch (sbtlData62.ef_stendp) {
    case 0:
        if (hwk->flg & 0x100) {
            sbtlDebug_DrawNormalList_Col(sblist_effhitemenu03, 5, wk->point - 14, 0, 14, 218);
            sbtlDrawString(160, 216, str, wk->point, 14);
        } else {
            sbtlDebug_DrawNormalList_Col(sblist_effhitemenu03_0, 5, wk->point - 14, 0, 14, 218);
        }
        sbtlDrawString(144, 228, sblist_efhitmenu_edit_para06_00[para->iti], wk->point, 15);
        sbtlDrawInt_Num(182, 240, para->iti_no, wk->point, 16);
        sbtlDrawInt_Num(182, 252, para->takasa, wk->point, 17);
        sbtlDrawInt_Num(182, 264, para->muki, wk->point, 18);
        break;
    case 1:
        sbtlDebug_DrawNormalList_Col(sblist_effhitemenu03_a1, 5, wk->point - 14, 0, 14, 218);
        sbtlDrawString(160, 216, str, wk->point, 14);
        sbtlDrawFloat_Num(182, 228, iwk->per, wk->point, 15);
        sbtlDrawFloat_Num(182, 240, iwk->w, wk->point, 16);
        sbtlDrawFloat_Num(182, 252, iwk->h, wk->point, 17);
        break;
    case 2:
        sbtlDebug_DrawNormalList_Col(sblist_effhitemenu03_a2, 5, wk->point - 14, 0, 14, 218);
        sbtlDrawString(160, 216, str, wk->point, 14);
        sbtlDrawFloat_Num(182, 228, iwk->per, wk->point, 15);
        sbtlDrawFloat_Num(182, 240, iwk->w, wk->point, 16);
        sbtlDrawFloat_Num(182, 252, iwk->h, wk->point, 17);
        break;
    case 3:
        sbtlDebug_DrawNormalList_Col(sblist_effhitemenu04, 5, wk->point - 14, 0, 14, 218);
        sbtlDrawString(160, 216, str, wk->point, 14);
        sbtlDrawString(144, 228, sblist_efhitmenu_edit_para06_00[para->iti], wk->point, 15);
        sbtlDrawInt_Num(182, 240, para->iti_no, wk->point, 16);
        sbtlDrawInt_Num(182, 252, para->takasa, wk->point, 17);
        sbtlDrawInt_Num(182, 264, para->muki, wk->point, 18);
        break;
    }
    sbtlDebug_EPLParaDraw(wk, hwk, para, iwk, 19);
    sbtlDebug_MoveTypeDraw(wk, hwk, para, iwk, path, 24);
    return 1;
}

s32 sbtlMDLHitEditorDraw_Functbl_Sphere(struct SBDEBUG_CSRWORK *wk, SBTLSKILLHIT_TBL *hwk, SBTLSKILLHIT_POSWORK *para,
                                        SBTLSKILLIDOU_WORK *iwk, struct SBDEBUG_PATHDATA *path) {
    char *str = sbtlData109;

    fldLmapSubmitScaledSpritePacket(0, 320, 0, 244, 52, 0x60000000, 0x40804040, 103);
    sbtlDebug_DrawNormalList_Col(sblist_mdlhitmenu_edit_para00, 4, wk->point - 1, -1, 14, 42);
    sbtlDrawFloat_Num(182, 40, hwk->area_r, wk->point, 1);
    sbtlDrawFloat_Num(182, 52, hwk->cy.height, wk->point, 2);
    sbtlDrawFloat_Num(182, 64, hwk->rad, wk->point, 3);
    sbtlDrawInt_Num(182, 76, hwk->lifetime, wk->point, 4);
    fldLmapSubmitScaledSpritePacket(0, 768, 0, 244, 16, 0x60000000, 0x40804040, 103);
    sbtlDebug_DrawNormalList_Col(sblist_efhitmenu_edit_para01, 1, wk->point - 5, -1, 14, 98);
    sbtlDrawString(160, 98, str, wk->point, 5);
    fldLmapSubmitScaledSpritePacket(0, 896, 0, 244, 28, 0x60000000, 0x40804040, 103);
    sbtlDebug_DrawNormalList_Col(sblist_mdlhitmenu_edit_para02, 2, wk->point - 6, -1, 14, 114);
    sbtlDrawFloat_Num(182, 112, hwk->dir, wk->point, 6);
    sbtlDrawFloat_Num(182, 124, hwk->cy.xrad, wk->point, 7);
    fldLmapSubmitScaledSpritePacket(0, 1120, 0, 244, 64, 0x60000000, 0x40804040, 103);
    sbtlDebug_DrawNormalList_Col(sblist_effhitemenu03_0, 5, wk->point - 8, 0, 14, 142);
    sbtlDrawString(160, 144, str, wk->point, 8);
    sbtlDrawString(144, 156, sblist_efhitmenu_edit_para06_00[para->iti], wk->point, 9);
    sbtlDrawInt_Num(182, 168, para->iti_no, wk->point, 10);
    sbtlDrawInt_Num(182, 180, para->takasa, wk->point, 11);
    sbtlDrawInt_Num(182, 192, para->muki, wk->point, 12);
    sbtlMDLHitEditor_DrawMotion(wk, hwk, 13);
    return 1;
}

INCLUDE_ASM(const s32, "code_002A38D8", sbtlMDLHitEditor_DrawMotion);

s32 sbtlMDLHitEditorDraw_Functbl_Cylinder(struct SBDEBUG_CSRWORK *wk, SBTLSKILLHIT_TBL *hwk, SBTLSKILLHIT_POSWORK *para,
                                        SBTLSKILLIDOU_WORK *iwk, struct SBDEBUG_PATHDATA *path) {
    char *str = sbtlData109;

    fldLmapSubmitScaledSpritePacket(0, 320, 0, 244, 52, 0x60000000, 0x40804040, 103);
    sbtlDebug_DrawNormalList_Col(sblist_mdlhitmenu_edit_para00_0, 4, wk->point - 1, -1, 14, 42);
    sbtlDrawFloat_Num(182, 40, hwk->area_r, wk->point, 1);
    sbtlDrawFloat_Num(182, 52, hwk->cy.height, wk->point, 2);
    sbtlDrawFloat_Num(182, 64, hwk->rad, wk->point, 3);
    sbtlDrawInt_Num(182, 76, hwk->lifetime, wk->point, 4);
    fldLmapSubmitScaledSpritePacket(0, 768, 0, 244, 16, 0x60000000, 0x40804040, 103);
    sbtlDebug_DrawNormalList_Col(sblist_efhitmenu_edit_para01, 1, wk->point - 5, -1, 14, 98);
    sbtlDrawString(160, 98, str, wk->point, 5);
    fldLmapSubmitScaledSpritePacket(0, 896, 0, 244, 28, 0x60000000, 0x40804040, 103);
    sbtlDebug_DrawNormalList_Col(sblist_mdlhitmenu_edit_para02, 2, wk->point - 6, -1, 14, 114);
    sbtlDrawFloat_Num(182, 112, hwk->dir, wk->point, 6);
    sbtlDrawFloat_Num(182, 124, hwk->cy.xrad, wk->point, 7);
    fldLmapSubmitScaledSpritePacket(0, 1120, 0, 244, 64, 0x60000000, 0x40804040, 103);
    sbtlDebug_DrawNormalList_Col(sblist_effhitemenu03_0, 5, wk->point - 8, 0, 14, 142);
    sbtlDrawString(160, 144, str, wk->point, 8);
    sbtlDrawString(144, 156, sblist_efhitmenu_edit_para06_00[para->iti], wk->point, 9);
    sbtlDrawInt_Num(182, 168, para->iti_no, wk->point, 10);
    sbtlDrawInt_Num(182, 180, para->takasa, wk->point, 11);
    sbtlDrawInt_Num(182, 192, para->muki, wk->point, 12);
    sbtlMDLHitEditor_DrawMotion(wk, hwk, 13);
    return 1;
}

s32 sbtlMDLHitEditorDraw_Functbl_Ring(struct SBDEBUG_CSRWORK *wk, SBTLSKILLHIT_TBL *hwk, SBTLSKILLHIT_POSWORK *para,
                                      SBTLSKILLIDOU_WORK *iwk, struct SBDEBUG_PATHDATA *path) {
    char *str = sbtlData109;

    fldLmapSubmitScaledSpritePacket(0, 320, 0, 244, 64, 0x60000000, 0x40804040, 103);
    sbtlDebug_DrawNormalList_Col(sblist_mdlhitmenu_edit_para00_1, 5, wk->point - 1, -1, 14, 42);
    sbtlDrawFloat_Num(182, 40, hwk->area_r, wk->point, 1);
    sbtlDrawFloat_Num(182, 52, hwk->area_r2, wk->point, 2);
    sbtlDrawFloat_Num(182, 64, hwk->cy.height, wk->point, 3);
    sbtlDrawFloat_Num(182, 76, hwk->rad, wk->point, 4);
    sbtlDrawInt_Num(182, 88, hwk->lifetime, wk->point, 5);
    fldLmapSubmitScaledSpritePacket(0, 832, 0, 244, 16, 0x60000000, 0x40804040, 103);
    sbtlDebug_DrawNormalList_Col(sblist_efhitmenu_edit_para01, 1, wk->point - 6, -1, 14, 106);
    sbtlDrawString(160, 106, str, wk->point, 6);
    fldLmapSubmitScaledSpritePacket(0, 960, 0, 244, 28, 0x60000000, 0x40804040, 103);
    sbtlDebug_DrawNormalList_Col(sblist_mdlhitmenu_edit_para02, 2, wk->point - 7, -1, 14, 122);
    sbtlDrawFloat_Num(182, 122, hwk->dir, wk->point, 7);
    sbtlDrawFloat_Num(182, 134, hwk->cy.xrad, wk->point, 8);
    fldLmapSubmitScaledSpritePacket(0, 1184, 0, 244, 64, 0x60000000, 0x40804040, 103);
    sbtlDebug_DrawNormalList_Col(sblist_effhitemenu03_0, 5, wk->point - 9, 0, 14, 150);
    sbtlDrawString(160, 150, str, wk->point, 9);
    sbtlDrawString(144, 162, sblist_efhitmenu_edit_para06_00[para->iti], wk->point, 10);
    sbtlDrawInt_Num(182, 174, para->iti_no, wk->point, 11);
    sbtlDrawInt_Num(182, 186, para->takasa, wk->point, 12);
    sbtlDrawInt_Num(182, 198, para->muki, wk->point, 13);
    sbtlMDLHitEditor_DrawMotion(wk, hwk, 14);
    return 1;
}

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_EPLParaDraw);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_MoveTypeDraw);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlEPLDraw);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_EffHitPathDraw);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlNEFMODEFuncEditSub);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDrawPoseture);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_DrawClrcle_Line);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_EffHitIdou);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_EffHitDraw);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_MDLHitDraw);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallDrwZLoad);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallDrwZLoad2);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_DrawHitCircle);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDrawString);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDrawFloat_Num);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDrawInt_Num);

s32 sbtlLeaf100(dds3ProcessID_t *id)
{
    return 1;
}

INCLUDE_ASM(const s32, "code_002A38D8", sbtlSkillChangeProc);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlFunc15);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlRefSbtl67);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlRefSbtl56);

s32 sbtlLeaf119(dds3ProcessID_t *id)
{
    return 1;
}

INCLUDE_ASM(const s32, "code_002A38D8", sbtlGetUnit);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallSbtlDebugCheckFreeCameraFlg);

INCLUDE_ASM(const s32, "code_002A38D8", SceneDel_Calc);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_Proc);

INCLUDE_ASM(const s32, "code_002A38D8", MainProc);

INCLUDE_ASM(const s32, "code_002A38D8", ModelSelectProc);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlDebugSetStatusflg);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallGetUnit);

INCLUDE_ASM(const s32, "code_002A38D8", MotionChngProc);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlParaProc);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrBattleHitpath);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrBattleBed);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlSetUnitPara);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlBSTProc);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlUnitChngProc);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_SetVisualData);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlLeaf105);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlVisualProc);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlLeaf98);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlLeaf122);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlLeaf104);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlNEFMODE_Func_Open);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlMDLMODE_Func_Open);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_MOVEMODE_Func_Open);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_LoadMotionMoveData);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_LoadMHTData);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCheckOpenMenu);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlResetMotionMovePos);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlBtlDebugLoadMHTDataSub);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlNEFMODE_Func_Save);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlNEFMODE_Func_Load);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallMemset27);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlResetMotionMovePosTable);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlBattleHitpath);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrFilename);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlPacPBFile);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlBattleBed);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallFileCheckEnd);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlSetPathMotion);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlResetEPLData);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCreateUnit2);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlSetPathMotionTable);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlSetPreViewModelTable);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlSetPreViewModel);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDeletePrevModel);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlNEFMODE_Func_PageList);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlMOVEMODE_Func_PageList);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlMDLMODE_Func_PageList);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlAddPrev);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDecPrev);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlClearEffHitData);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlLeaf108);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlLeaf95);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlSizeofData);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlFunc28);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallRestartBattle);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlFunc17);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlEPLDataDelete);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlPATHDataDelete);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCopyData);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_ChangeModel);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlSetModelData);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlMOVEMODE_Func_Edit);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlSetMotionMoveData_Buffer);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlMDLMODE_Func_Edit);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlNEFMODE_Func_Edit);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlSetEffHitPara_S);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlSetEffHitPara_sbtl);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_SetMDLMOVEPara);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlGetMotionNum);

#define SBTL_LIMIT_F(v, min, max) (((v) <= (min)) ? (min) : (((max) <= (v)) ? (max) : (v)))
#define SBTL_LIMIT_I(v, min, max) (((v) <= (min)) ? (min) : (((v) > (max)) ? (max) : (v)))

s32 sbtlSetMDLHitPara(SBTLSKILLHIT_TBL *hwk, SBDEBUG_CSRWORK *wk, s32 flg)
{
    f32 fadd = 0.0f;
    s32 add = 0;
    s32 num;
    SBTLSKILLHIT_POSWORK *start;
    /* BUG: copied from the effect hit editor without its ef_stendp switch, so
     * the move fields (ef_stendp 1 and 2) are written through NULL. */
    SBTLSKILLIDOU_WORK *idou = NULL;

    if (flg == 0) {
        fadd = 0.1f;
        add = 1;
    } else if (flg == 1) {
        fadd = -0.1f;
        add = -1;
    } else if (flg == 2) {
        fadd = 0.5f;
        add = 5;
    } else if (flg == 3) {
        fadd = -0.5f;
        add = -5;
    } else if (flg == 4) {
        fadd = 5.0f;
        add = 10;
    } else if (flg == 5) {
        fadd = -5.0f;
        add = -10;
    }

    start = &hwk->start;
#ifdef FIX_BUGS
    /* The effect hit editor (sbtlSetEffHitPara_sbtl) edits apos1 for ef_stendp 1, apos2 for 2. */
    idou = (evtStageTestState.ef_stendp == 1) ? &hwk->apos1 : &hwk->apos2;
#endif

    if (evtStageTestState.ef_mode == 3) {
        switch (evtStageTestState.nef_poseture) {
        case 0:
            hwk->x += fadd;
            if (hwk->x <= -1000.0f) {
                hwk->x = -1000.0f;
            }
            if (1000.0f <= hwk->x) {
                hwk->x = 1000.0f;
            }
            break;
        case 1:
            hwk->y += fadd;
            if (hwk->y <= -1000.0f) {
                hwk->y = -1000.0f;
            }
            if (1000.0f <= hwk->y) {
                hwk->y = 1000.0f;
            }
            break;
        case 2:
            hwk->z += fadd;
            if (hwk->z <= -1000.0f) {
                hwk->z = -1000.0f;
            }
            if (1000.0f <= hwk->z) {
                hwk->z = 1000.0f;
            }
            break;
        }
        return 1;
    }

    switch (hwk->type) {
    case 0:
        switch (wk->point) {
        case 0:
            evtStageTestState.ef_mode = 1;
            break;
        case 1:
            hwk->area_r += fadd;
            hwk->area_r = SBTL_LIMIT_F(hwk->area_r, 0.0f, 4000.0f);
            break;
        case 2:
            hwk->cy.height += fadd;
            hwk->cy.height = SBTL_LIMIT_F(hwk->cy.height, 0.0f, 4000.0f);
            break;
        case 3:
            hwk->rad += fadd;
            if (1.0f <= hwk->rad) {
                hwk->rad = 1.0f;
            }
            if (hwk->rad <= -1.0f) {
                hwk->rad = -1.0f;
            }
            break;
        case 4:
            hwk->lifetime += add;
            hwk->lifetime = SBTL_LIMIT_I(hwk->lifetime, 0, 300);
            break;
        case 5:
            evtStageTestState.ef_mode = 3;
            break;
        case 6:
            hwk->dir += fadd;
            if (360.0f <= hwk->dir) {
                hwk->dir = 0.0f;
            }
            if (hwk->dir < 0.0f) {
                hwk->dir = 360.0f;
            }
            break;
        case 7:
            hwk->cy.xrad += fadd;
            if (360.0f <= hwk->cy.xrad) {
                hwk->cy.xrad = 0.0f;
            }
            if (hwk->cy.xrad < 0.0f) {
                hwk->cy.xrad = 360.0f;
            }
            break;
        case 8:
            break;
        case 9:
            if (start->iti == 1) {
                start->iti = 10;
                hwk->flg |= 0x8000;
            } else {
                start->iti = 1;
                hwk->flg &= ~0x8000;
            }
            break;
        case 10:
            if (evtStageTestState.ef_stendp == 0 || evtStageTestState.ef_stendp == 3) {
                start->iti_no += add;
                start->iti_no = SBTL_LIMIT_I(start->iti_no, 0, 999);
            } else {
                idou->w += fadd;
                if (1000.0f <= idou->w) {
                    idou->w = 1000.0f;
                }
                if (idou->w <= -1000.0f) {
                    idou->w = -1000.0f;
                }
            }
            break;
        case 11:
            if (evtStageTestState.ef_stendp == 0 || evtStageTestState.ef_stendp == 3) {
                start->takasa += add;
                start->takasa = SBTL_LIMIT_I(start->takasa, 0, 5);
            } else {
                idou->h += fadd;
                if (1000.0f <= idou->h) {
                    idou->h = 1000.0f;
                }
                if (idou->h <= -1000.0f) {
                    idou->h = -1000.0f;
                }
            }
            break;
        case 12:
            if (evtStageTestState.ef_stendp == 0 || evtStageTestState.ef_stendp == 3) {
                start->muki += add;
                start->muki = SBTL_LIMIT_I(start->muki, 0, 8);
            }
            break;
        case 13:
            hwk->motionno += add;
            num = sbtlGetMotionNum(evtStageTestState.acterID);
            if (hwk->motionno >= num) {
                hwk->motionno = 0;
            }
            if (hwk->motionno < 0) {
                hwk->motionno = num - 1;
            }
            break;
        }
        break;
    case 1:
        switch (wk->point) {
        case 0:
            evtStageTestState.ef_mode = 1;
            break;
        case 1:
            hwk->area_r += fadd;
            hwk->area_r = SBTL_LIMIT_F(hwk->area_r, 0.0f, 4000.0f);
            hwk->cy.area_r += fadd;
            hwk->cy.area_r = SBTL_LIMIT_F(hwk->cy.area_r, 0.0f, 4000.0f);
            break;
        case 2:
            hwk->cy.height += fadd;
            hwk->cy.height = SBTL_LIMIT_F(hwk->cy.height, 0.0f, 4000.0f);
            break;
        case 3:
            hwk->rad += fadd;
            if (1.0f <= hwk->rad) {
                hwk->rad = 1.0f;
            }
            if (hwk->rad <= -1.0f) {
                hwk->rad = -1.0f;
            }
            break;
        case 4:
            hwk->lifetime += add;
            hwk->lifetime = SBTL_LIMIT_I(hwk->lifetime, 0, 300);
            break;
        case 5:
            evtStageTestState.ef_mode = 3;
            break;
        case 6:
            hwk->dir += fadd;
            if (360.0f <= hwk->dir) {
                hwk->dir = 0.0f;
            }
            if (hwk->dir < 0.0f) {
                hwk->dir = 360.0f;
            }
            break;
        case 7:
            hwk->cy.xrad += fadd;
            if (360.0f <= hwk->cy.xrad) {
                hwk->cy.xrad = 0.0f;
            }
            if (hwk->cy.xrad < 0.0f) {
                hwk->cy.xrad = 360.0f;
            }
            break;
        case 8:
            break;
        case 9:
            if (start->iti == 1) {
                start->iti = 10;
                hwk->flg |= 0x8000;
            } else {
                start->iti = 1;
                hwk->flg &= ~0x8000;
            }
            break;
        case 10:
            if (evtStageTestState.ef_stendp == 0 || evtStageTestState.ef_stendp == 3) {
                start->iti_no += add;
                start->iti_no = SBTL_LIMIT_I(start->iti_no, 0, 999);
            } else {
                idou->w += fadd;
                if (1000.0f <= idou->w) {
                    idou->w = 1000.0f;
                }
                if (idou->w <= -1000.0f) {
                    idou->w = -1000.0f;
                }
            }
            break;
        case 11:
            if (evtStageTestState.ef_stendp == 0 || evtStageTestState.ef_stendp == 3) {
                start->takasa += add;
                start->takasa = SBTL_LIMIT_I(start->takasa, 0, 5);
            } else {
                idou->h += fadd;
                if (1000.0f <= idou->h) {
                    idou->h = 1000.0f;
                }
                if (idou->h <= -1000.0f) {
                    idou->h = -1000.0f;
                }
            }
            break;
        case 12:
            if (evtStageTestState.ef_stendp == 0 || evtStageTestState.ef_stendp == 3) {
                start->muki += add;
                start->muki = SBTL_LIMIT_I(start->muki, 0, 8);
            }
            break;
        case 13:
            hwk->motionno += add;
            num = sbtlGetMotionNum(evtStageTestState.acterID);
            if (hwk->motionno >= num) {
                hwk->motionno = 0;
            }
            if (hwk->motionno < 0) {
                hwk->motionno = num - 1;
            }
            break;
        }
        break;
    case 2:
        switch (wk->point) {
        case 0:
            evtStageTestState.ef_mode = 1;
            break;
        case 1:
            hwk->area_r += fadd;
            hwk->area_r = SBTL_LIMIT_F(hwk->area_r, 0.0f, 4000.0f);
            hwk->cy.area_r += fadd;
            hwk->cy.area_r = SBTL_LIMIT_F(hwk->cy.area_r, 0.0f, 4000.0f);
            break;
        case 2:
            hwk->area_r2 += fadd;
            hwk->area_r2 = SBTL_LIMIT_F(hwk->area_r2, 0.0f, 4000.0f);
            break;
        case 3:
            hwk->cy.height += fadd;
            hwk->cy.height = SBTL_LIMIT_F(hwk->cy.height, 0.0f, 4000.0f);
            break;
        case 4:
            hwk->rad += fadd;
            if (1.0f <= hwk->rad) {
                hwk->rad = 1.0f;
            }
            if (hwk->rad <= -1.0f) {
                hwk->rad = -1.0f;
            }
            break;
        case 5:
            hwk->lifetime += add;
            hwk->lifetime = SBTL_LIMIT_I(hwk->lifetime, 0, 300);
            break;
        case 6:
            evtStageTestState.ef_mode = 3;
            break;
        case 7:
            hwk->dir += fadd;
            if (360.0f <= hwk->dir) {
                hwk->dir = 0.0f;
            }
            if (hwk->dir < 0.0f) {
                hwk->dir = 360.0f;
            }
            break;
        case 8:
            hwk->cy.xrad += fadd;
            if (360.0f <= hwk->cy.xrad) {
                hwk->cy.xrad = 0.0f;
            }
            if (hwk->cy.xrad < 0.0f) {
                hwk->cy.xrad = 360.0f;
            }
            break;
        case 9:
            break;
        case 10:
            if (start->iti == 1) {
                start->iti = 10;
                hwk->flg |= 0x8000;
            } else {
                start->iti = 1;
                hwk->flg &= ~0x8000;
            }
            break;
        case 11:
            if (evtStageTestState.ef_stendp == 0 || evtStageTestState.ef_stendp == 3) {
                start->iti_no += add;
                start->iti_no = SBTL_LIMIT_I(start->iti_no, 0, 999);
            } else {
                idou->w += fadd;
                if (1000.0f <= idou->w) {
                    idou->w = 1000.0f;
                }
                if (idou->w <= -1000.0f) {
                    idou->w = -1000.0f;
                }
            }
            break;
        case 12:
            if (evtStageTestState.ef_stendp == 0 || evtStageTestState.ef_stendp == 3) {
                start->takasa += add;
                start->takasa = SBTL_LIMIT_I(start->takasa, 0, 5);
            } else {
                idou->h += fadd;
                if (1000.0f <= idou->h) {
                    idou->h = 1000.0f;
                }
                if (idou->h <= -1000.0f) {
                    idou->h = -1000.0f;
                }
            }
            break;
        case 13:
            if (evtStageTestState.ef_stendp == 0 || evtStageTestState.ef_stendp == 3) {
                start->muki += add;
                start->muki = SBTL_LIMIT_I(start->muki, 0, 8);
            }
            break;
        case 14:
            hwk->motionno += add;
            num = sbtlGetMotionNum(evtStageTestState.acterID);
            if (hwk->motionno >= num) {
                hwk->motionno = 0;
            }
            if (hwk->motionno < 0) {
                hwk->motionno = num - 1;
            }
            break;
        }
        break;

    }
    return 1;
}

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallSprintf7);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallDclose);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPfs0BattleEffhit);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPfs0BattleHitpath);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPfs0BattleBed);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPfs0BattleMdlhit);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlCallSprintf7Table);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlLenDir);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallMemcpy);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlMotionMoveData);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlFilename);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlSetEffHitParaTable);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlSetEffHitPara);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlBtlDebugMDLHitDrawSub);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlBtlDebugMDLHitDrawSub2);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlBtlDebugMDLHitDrawSub2Table);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlSetVisualPara);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlEffectCylinderData);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrCylinderHeight);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr31Ff31Ff31Ff31Ff31Ff);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStr31Ff31Ff31Ff31Ff31Ff2);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrPfs0ModelBattleEfftxt03X);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrSmodelBattleEfftxt03XTxt);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrSbtlmodelSizeM);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrSbtlmodelSizeS);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrSbtlmodelSizeL);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrSbtlmodelSizeLl);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlMotionDataSaveTable);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrMotionFlgNone);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrMotionFlgSpeedOn);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrMotionFlgNcancel);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrMotionFlgSpeedOnMotion);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlMotionDataSaveTable2);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrSbtlsdwFlgNone);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrSbtlsdwFlgCircleSbtlsdw);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrSbtlsdwFlgCircleSbtlsdw2);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrSbtlsdwFlgCircleSbtlsdw3);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrSbtlsdwFlgCircleSbtlsdw4);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrSbtlsdwFlgHelper);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlMotionDataSaveTable3);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlMotionDataSave);

INCLUDE_ASM(const s32, "code_002A38D8", mdlGetMotionLength);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlSousaProc);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_UnitPad);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlSetSword);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlOpti);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlSearchUnitID31);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlFunc21);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlMainProcSub);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_DevilNameListDraw);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_UnitPanelCursor);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlRefSbtl52);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_SetEncountPara);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_ResetStockPara);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_CheckLoadEncountData);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_Shutdown);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlLeaf96);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlLeaf88);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlFunc16);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_DrawNormalList_Col);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_SetStatusflg);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_DrawONOFFList_Motion);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_DrawNormalList);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlDebug_DrawONOFFList_MotionTable);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlDebug_DrawNormalListTable);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlCallBtlCheckBit2Table);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlCheckBit2);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlLeaf117);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallVsprintf);

void sbtlCallGrAllocPacketBuff8(s32 x, s32 y, s32 attr, const char *format)
{
    sdfDrawTag_t *list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfGrOt_t *otp;

    sdfDrawTagInit(list);
    sdfDrawTagLink(list, (u32)sdfDevPrintf((x << 4) + 0x7000, (y << 3) + 0x7900, 0xFF0000, attr, format));
    otp = &sdfGrOtTable[112];
    otp->regFunc(otp, list);
}

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallMemset29);

INCLUDE_ASM(const s32, "code_002A38D8", evtStageTestRequestModelAsset);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallGetRunTimeCount);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallVsprintf2);

s32 sbtlLeaf91(void)
{
    return printf(sbtlMotionDataSaveData4, sbtlData80);
}

INCLUDE_ASM(const s32, "code_002A38D8", sbtlPfsBattleEfftime);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlGetUnitHelperPos2);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallSetOt5);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallMemset34);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallMemset32);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallMemset35);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallMemset28);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlFunc18);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_DrawC);

INCLUDE_ASM(const s32, "code_002A38D8", _3DCylinderDraw);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallSetOt4);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallMemAllocDirect7);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDebug_DeleteHitList);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlDrawHitList);

/* NON_MATCHING: 139 of 165 words match.  */
#ifdef NON_MATCHING
void sbtlDraw3DSphere(const sdfVector4_t *center, u32 color, const sdfMatrix4_t *rotation, f32 radius)
{
    sdfVector4_t point;
    sdfMatrix4_t matrix;
    sdfVector4_t vertices[22];
    u32 colors[22];
    sdfDrawTag_t *list;
    sdfVector4_t *out;
    f32 latitude;
    f32 longitude;
    f32 ringRadius;
    f32 ringY;
    u32 pass;
    u32 ring = 0;
    u32 segment;

    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    do {
        colors[ring++] = color;
    } while (ring < 22);

    if (rotation == NULL) {
        SDF_VU0_STOREIDENTITY(&matrix);
    } else {
        SDF_VU0_LOADMATRIX(rotation);
        SDF_VU0_STOREMATRIX(&matrix);
    }
    for (pass = 0; pass < 2; pass++) {
        latitude = 0.0f;
        for (ring = 0; ring < 9; ring++) {
            latitude += 0.31415925f;
            out = vertices;
            longitude = 0.0f;
            ringY = radius * sdfCos(latitude);
            ringRadius = radius * sdfSin(latitude);
            SDF_VU0_LOADMATRIX(&matrix);
            SDF_VU0_LOAD_ROW3(center);
            point.x = ringRadius;
            point.y = ringY;
            point.z = 0.0f;
            SDF_VU0_LOADVECTOR(&point);
            SDF_VU0_APPLYMATRIX_XYZ1();

            for (segment = 0; segment < 11; segment++) {
                SDF_VU0_STOREVECTOR(out);
                out++;
                longitude += 0.5711986f;
                point.x = sdfCos(longitude) * ringRadius;
                point.y = ringY;
                point.z = sdfSin(longitude) * ringRadius;
                SDF_VU0_LOADVECTOR(&point);
                SDF_VU0_APPLYMATRIX_XYZ1();
                SDF_VU0_STOREVECTOR(out);
                out++;
            }
            sdfDrawTagLink(list, (addr_t)effDebugDrawWireModel(vertices, colors, 22, 0x80));
        }
        SetRotateMatrixX(1.5707963f);
        SDF_VU0_LOADMATRIX_B(&matrix);
        MultMatrixNoArg();
        SDF_VU0_STOREMATRIX_NOMEM(&matrix);
    }
    dds3MultiBillboardOt.regFunc(&dds3MultiBillboardOt, list);
}
#else
INCLUDE_ASM(const s32, "code_002A38D8", sbtlDraw3DSphere);
#endif

/* NON_MATCHING: 13 of 26 words match.  */
#ifdef NON_MATCHING
/* body: matched in raidou @ 61f09dc: src/raidou1/code_r1_0024F220.c:1671, near, same function 0.94 (words 26/23, calls 1.00, shape 0.94) */
s32 sbtlGetMotionNumSub(struct SBTLUNIT_WORK *wk)
{
    s32 status = wk->status;

    if ((u32)(status - 10) >= 2 && status != 14 && wk->model->mode != 9 && wk->mode >= 5 && status != 40) {
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_002A38D8", sbtlGetMotionNumSub);
#endif

INCLUDE_ASM(const s32, "code_002A38D8", sdfSetLocalMapCounterMarkerState);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallGrAllocPacketBuff7);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallGrAllocPacketBuff6);

/* body: matched in raidou @ 61f09dc: src/raidou1/sbtl.c:86, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
s32 sbtlLeaf106(dds3ProcessID_t *id) {
    return 0;
}

/* body: matched in raidou @ 61f09dc: src/raidou1/sbtl.c:86, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
s32 sbtlLeaf116(dds3ProcessID_t *id) {
    return 0;
}

INCLUDE_ASM(const s32, "code_002A38D8", sbtlBtlDebugProcSub);

void sbtlCallMoonFadeIn(void)
{
    if (!(sbtlData119 & 4)) {
        if (sbtlData119 & 2) {
            if (!(sbtlData119 & 1)) {
                evtMoonFadeIn(12);
            }
            sbtlData119 &= ~2;
        }
    }
}

INCLUDE_ASM(const s32, "code_002A38D8", sbtlFuncKatana);

void sbtlSetMenuImage(dds3ProcessID_t *PID) {
    itfMenuLinkSprite(PID, wk_sbtl->panel->hPanelSpr3);
    itfMenuSetSpriteData(PID, ITFMENU_IMG_DAT_TITLE, 0, 0, 0, 0, 23, 24, NULL, sbtlSetMenuImageData3);
    itfMenuSetSpriteData(PID, ITFMENU_IMG_DAT_PANEL, 322, 71, 128, 23, 17, 4, NULL, sbtlSetMenuImageData);
    itfMenuSetSpriteData(PID, ITFMENU_IMG_DAT_SELECT, 322, 71, 128, 23, 17, 4, NULL, sbtlSetMenuImageData2);
    itfMenuSetSpriteData(PID, ITFMENU_IMG_DAT_SCROLL, 0, 0, 3, 163, 490, 84, NULL, sbtlSetMenuImageData4);
    itfMenuSetImageColor(PID, ITFMENU_IMG_DAT_SCROLL, 200, 180, 80, 128);
}

dds3ProcessID_t *sbtlFuncDecideItem(u8 num) {
    s32 i = 0;
    dds3ProcessID_t *pid = itfMenuProcessStart();

    for (; i < num; i++) {
        itfMenuSetRootMenu(pid, i, sbtlFuncDecideItemData, NULL);
    }
    itfGeneralMenuCreatePIDSub(pid, 3);
    itfGeneralMenuCreatePIDSub2(pid, 3);
    itfMenuSetFlag(pid, ITFMENU_GF_EXIST_TITLE, 0);
    itfMenuSetFlag(pid, ITFMENU_GF_CANCEL_RELEASE, 0);
    sbtlSetMenuImage(pid);
    itfMenuSetOt(pid);
    return pid;
}

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallGetProcessData54);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallGetProcessData44);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallGetProcessData48);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlSearchUnitID28);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallGetProcessData42);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallMenuDecFilter);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlFuncAnalyze);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallGetProcessData52);

INCLUDE_ASM(const s32, "code_002A38D8", Add_AW_XAtkHand);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallGetProcessData47);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlGetSkilltbl20);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallGetProcessData45);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallGetProcessData55);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallGetProcessData49);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallGetProcessData57);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallGetProcessData58);

INCLUDE_ASM(const s32, "code_002A38D8", cmpSetSkillListHd);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallGetProcessData41);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlRefTbldata);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallGetProcessData51);

INCLUDE_ASM(const s32, "code_002A38D8", DrawFuncSkl);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlGetPlayerUnit);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlFuncItem);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlFuncDecideAnalyze);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallGetProcessData50);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallGetProcessData46);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallMenuSetFont);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallMenuSetFlagMenu);

void sbtlCallMenuSetRootMenu(sBtlMenu_t *menu) {
    itfMenuSetRootMenu(menu->menuPID, 1, sbtlCallMenuSetRootMenuData[1], NULL);
    itfMenuSetRootMenu(menu->menuPID, 2, sbtlCallMenuSetRootMenuData[2], sbtlCallGetProcessData55);
    itfMenuSetRootMenu(menu->menuPID, 3, sbtlCallMenuSetRootMenuData[3], sbtlCallGetProcessData49);
    itfMenuSetRootMenu(menu->menuPID, 4, sbtlCallMenuSetRootMenuData[4], cmpSetSkillListHd);
    itfMenuSetRootMenu(menu->menuPID, 5, sbtlCallMenuSetRootMenuData[5], sbtlFuncItem);
    if (evtBitCheck(2093)) {
        itfMenuSetRootMenu(menu->menuPID, 6, sbtlCallMenuSetRootMenuData[6], sbtlCallGetProcessData50);
    }
    itfMenuSetRootMenu(menu->menuPID, 7, sbtlCallMenuSetRootMenuData[7], sbtlCallGetProcessData46);
    itfMenuSetMenu(menu->menuPID, 8, 1, sbtlCallMenuSetRootMenuData[9], sbtlCallGetProcessData47);
    itfMenuSetMenu(menu->menuPID, 9, 1, sbtlCallMenuSetRootMenuData[10], sbtlCallGetProcessData47);
    itfMenuSetMenu(menu->menuPID, 10, 1, sbtlCallMenuSetRootMenuData[11], sbtlCallGetProcessData47);
    itfMenuSetMenu(menu->menuPID, 11, 1, sbtlCallMenuSetRootMenuData[12], sbtlCallGetProcessData47);
    itfMenuSetMenu(menu->menuPID, 12, 1, sbtlCallMenuSetRootMenuData[13], sbtlCallGetProcessData47);
    itfMenuSetMenu(menu->menuPID, 13, 1, sbtlCallMenuSetRootMenuData[14], sbtlCallGetProcessData47);
    itfGeneralMenuCreatePIDSub(menu->menuPID, 3);
    itfGeneralMenuCreatePIDSub2(menu->menuPID, 3);
    itfMenuSetFlag(menu->menuPID, ITFMENU_GF_EXIST_TITLE, 0);
    itfMenuSetFlag(menu->menuPID, ITFMENU_GF_CANCEL_RELEASE, 0);
    itfMenuSetOt(menu->menuPID);
}

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallMenuProcessEnd);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallMenuProcessCheck);

INCLUDE_ASM(const s32, "code_002A38D8", effPCPAnalyzeBEDDelete);

s32 cmpSelectUpdate(sBtlMenu_t *menu) {
    s32 update = 0;

    if (dds3PadButtonState[0x2A] >= 0) {
        goto end;
    }
    if (menu->step == 2) {
        if (dds3ExistProcess(menu->menuPID) && itfMenuGetCtrlType(menu->menuPID) == ITFMENU_CTRL_TYPE_WAIT) {
            update = 1;
            cmpSklTypeSet(menu->menuPID);
        }
    } else if (menu->step == 3) {
        if (dds3ExistProcess(menu->listPID) && itfMenuGetCtrlType(menu->listPID) == ITFMENU_CTRL_TYPE_WAIT) {
            update = 1;
            cmpSklTypeSet(menu->listPID);
        }
    } else if (menu->step == 4) {
        if (dds3ExistProcess(menu->charPID) && itfMenuGetCtrlType(menu->charPID) == ITFMENU_CTRL_TYPE_WAIT) {
            update = 1;
            cmpSklTypeSet(menu->charPID);
        }
    } else if (menu->step == 5) {
        if (dds3ExistProcess(menu->listPID) && itfMenuGetCtrlType(menu->listPID) == ITFMENU_CTRL_TYPE_WAIT) {
            update = 1;
            itfMenuSetFlag(menu->listPID, ITFMENU_GF_FOCUS, 1);
            cmpSklTypeSet(menu->listPID);
        }
    }
    if (update) {
        menu->flag |= 0x20;
        return 1;
    }
end:
    return 0;
}

/* NON_MATCHING: 132 of 233 words match. Retail fills the return branch delay slots with v0 = 0 and orders the case 3/4 and 6/7 blocks differently. */
#ifdef NON_MATCHING
s32 sbtlCallGetProcessData56(dds3ProcessID_t *id) {
    sBtlMenu_t *menu = dds3GetProcessData(id);

    if (cmpSelectUpdate(menu)) {
        menu->flag |= 0x20;
        goto end;
    }
    switch (menu->step) {
        case 0:
            if (!sbtlCheckLoadFlg()) {
                return 0;
            }
            sbtlCallMenuSetRootMenu(menu);
            sbtlSetMenuImage(menu->menuPID);
            menu->listPID = sbtlFuncDecideItem(0x20);
            menu->charPID = sbtlFuncDecideItem(0x10);
            menu->step = 1;
            if (sbtlCheckTutorialBattle()) {
                sbtlData119 |= 4;
            }
            break;
        case 1:
            sbtlCallMoonFadeIn();
            if (menu->flag & 0x10) {
                menu->flag = (menu->flag & 0xCF) | 2;
                sbtlCallMenuSetFlagMenu(menu);
                itfCallGetProcessData6(menu->menuPID);
                itfMenuStart(menu->menuPID);
                menu->step = 2;
            } else {
                menu->flag &= ~2;
            }
            break;
        case 2:
            if (itfMenuGetWaitType(menu->menuPID) != ITFMENU_WAIT_TYPE_CANCEL) {
                return 0;
            }
            effPCPAnalyzeBEDDelete(menu);
            return 0;
        case 3:
            if (itfMenuGetWaitType(menu->listPID) != ITFMENU_WAIT_TYPE_CANCEL) {
                return 0;
            }
            wk_sbtl->flg &= ~0x80000;
            if (menu->flag & 0x20) {
                effPCPAnalyzeBEDDelete(menu);
            } else {
                sbtlCallMenuSetFlagMenu(menu);
                itfMenuStart(menu->menuPID);
                sbtlCallMoonFadeIn();
                menu->step = 2;
            }
            sbtlCallSetColor();
            return 0;
        case 4:
            if (itfMenuGetWaitType(menu->charPID) != ITFMENU_WAIT_TYPE_CANCEL) {
                return 0;
            }
            wk_sbtl->flg &= ~0x80000;
            if (menu->flag & 0x20) {
                effPCPAnalyzeBEDDelete(menu);
            } else {
                sbtlCallMenuSetFlagMenu(menu);
                itfMenuStart(menu->listPID);
                sbtlCallMoonFadeIn();
                menu->step = 3;
            }
            sbtlCallSetColor();
            return 0;
        case 5:
            if (menu->flag & 0x20) {
                wk_sbtl->flg &= ~0x80000;
                effPCPAnalyzeBEDDelete(menu);
                return 0;
            }
            if (menu->count == 0) {
                if (dds3PadButtonState[0x21] < 0) {
                    menu->count = 1;
                }
                menu->cntAnalyze++;
                if (menu->cntAnalyze > 10) {
                    menu->cntAnalyze = 10;
                }
                break;
            }
            if (wk_sbtl->mag < 10) {
                itfMenuSetFlagMenu(menu->listPID, -1, ITFMENU_CB_GF_ENABLE_DECIDE, 0);
            }
            itfMenuSetFlag(menu->listPID, ITFMENU_GF_FOCUS, 1);
            itfMenuSetFlagMenu(menu->listPID, -1, ITFMENU_CB_GF_SELECT, 0);
            menu->step = 3;
            menu->count = 0;
            menu->cntAnalyze = 0;
            sbtlCallMoonFadeIn();
            return 0;
        case 6:
            if (menu->count == 0) {
                sbtlCallMenuProcessEnd(menu);
            }
            if (sbtlCallMenuProcessCheck(menu)) {
                menu->count++;
                break;
            }
            menu->count = 0;
            menu->step = 7;
            break;
        case 7:
            if (menu->count == 0) {
                menu->flag &= ~2;
                menu->count++;
            } else if (menu->count < 2) {
                menu->count++;
            } else {
                menu->count = 0;
                menu->step = 8;
            }
            break;
        case 8:
            return -1;
    }
end:
    return 0;
}
#else
INCLUDE_RODATA(const s32, "code_002A38D8", sdfSetLocalMapCounterMarkerStateTable);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrILeaveItToYou);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrDonWasteMp);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrHealSupportMe);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrUseMagicAttacks);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrUsePhysicalAttacks);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrListenToMyOrders);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrCancelEscape);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrAnalyze);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrCommand);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrCommandMenu);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlCallGetProcessData47Table);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallGetProcessData56);
#endif

INCLUDE_ASM(const s32, "code_002A38D8", sbtlBtlDrawProcessSub);

s32 sbtlDrawProcess(dds3ProcessID_t *PID)
{
    sBtlMenu_t *menu = dds3GetProcessData(PID);

    sbtlBtlDrawProcessSub(menu);
    if (dds3ExistProcess(menu->drawPID)) {
        dds3KillProcess(menu->drawPID, 0);
        menu->drawPID = NULL;
    }
    sdfMemFreeDirect(menu->hMem);
    ProcessCheckData = NULL;
}

s32 sbtlCallGetProcessData53(dds3ProcessID_t *id) {
    sBtlMenu_t *menu = dds3GetProcessData(id);
    s32 sel;
    struct SBTLUNIT_WORK *wk;
    u8 alpha;

    if (!(menu->flag & 2)) {
        return 0;
    }
    if (menu->step == 2) {
        sel = itfMenuGetSelectID(menu->menuPID);
        if (sel < 8) {
            if (sel == 7 && sbtlRefSbtl13() == 1) {
                sel = 8;
            }
            sbtlHelpCantUse(3, sel, GetSelectRootID(menu->menuPID, 2));
        }
    }
    if (menu->step == 5) {
        wk = wk_sbtl->list2CC->pUnitL[itfMenuGetSelectID(menu->listPID)].pUnit;
        alpha = 255 * menu->cntAnalyze / 10;
        if (wk->flg & 0x800) {
            sbtlCallBtlAnalyzeBack3(sbtlSearchUnit_ID(wk_sbtl->unitList, wk->field492), alpha);
        } else {
            sbtlCallBtlAnalyzeBack3(wk, alpha);
        }
    }
    return 0;
}

/* NON_MATCHING: 11 of 13 words match.  */
#ifdef NON_MATCHING
/* body: matched in raidou @ 61f09dc: src/raidou1/code_0018CE10.c:286, near, same function 1.00 (words 13/13, calls 1.00, shape 1.00) */
s32 sbtlCallMemFreeDelayedDirect(void)
{
    if (itfData5 == NULL) {
        return 0;
    }
    sdfMemFreeDelayedDirect(itfData5);
    itfData5 = NULL;
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallMemFreeDelayedDirect);
#endif

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCampSubSpr);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlIsMenuStartScript);

INCLUDE_ASM(const s32, "code_002A38D8", ProcessStart_sbtlMenu);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallGetProcessData43);

void sbtlSbtlmenuMain(void) {
    struct sdfMemHandle_t *hMem = sdfMemAllocDirect(sizeof(sBtlMenu_t));
    sBtlMenu_t *menu = sdfMemLock(hMem);

    memset(menu, 0, sizeof(sBtlMenu_t));
    menu->hMem = hMem;
    ProcessCheckData = dds3StartProcess("sbtlmenu_main", 1022, 0, 0, sbtlCallGetProcessData56, sbtlDrawProcess, menu);
    menu->drawPID = dds3StartProcess("sbtlmenu_draw", 11029, 0, 0, sbtlCallGetProcessData53, NULL, menu);
    menu->menuPID = itfMenuProcessStart();
    itfCallGetProcessData7(menu->menuPID, 2, -1);
    itfCallGetProcessData7(menu->menuPID, 1, -1);
    sbtlData119 = 0;
}

/* NON_MATCHING: 15 of 26 words match.  */
#ifdef NON_MATCHING
/* body: matched in raidou @ 61f09dc: src/raidou1/code_00195698.c:164, adapt, same function 0.91 (words 26/25, calls 0.80, shape 0.98) */
void sbtlCallExistProcess2(void) {
    tagMAP_MENU_WORK *wk;

    if (ProcessCheckData != NULL && dds3ExistProcess(ProcessCheckData)) {
        wk = dds3GetProcessData(ProcessCheckData);
        if (!(wk->flag & 1)) {
            wk->flag |= 1;
            wk->step = 2;
            wk->count = 0;
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallExistProcess2);
#endif

s32 ProcessCheck(void) {
    if (ProcessCheckData == NULL) {
        return 0;
    }
    if (dds3GetProcessStat(ProcessCheckData) == 3 || dds3GetProcessStat(ProcessCheckData) == 0) {
        ProcessCheckData = NULL;
        return 0;
    }
    return 1;
}

s64 sbtlCallBtlCheckTutorialBattle3(void)
{
    return sbtlCheckTutorialBattle();
}

char *sbtlLeaf112(u8 n)
{
    char *name = NULL;

    if (n < 3) {
        name = sbtlData81[n];
    }
    return name;
}

char *sbtlGetPlayerName(void)
{
    if (sbtlCallBtlCheckTutorialBattle3()) {
        return sbtlLeaf112(2);
    }
    return sbtlLeaf112(1);
}

INCLUDE_ASM(const s32, "code_002A38D8", sbtlLeaf113);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlEnableSceneGroupAdvancementSub);

void Init_sbtlDraw(void)
{
    sbtlCallMoverInit();
    sbtlData121 = 1;
    sbtlRefSbtl57Data = 0;
    sbtlRefSbtl61Data = -1;
    sbtlRefSbtl59Data = -1;
    sbtlCallBtlSearchUnitID37Data2 = -1;
    sbtlCallBtlSpriteData = -1;
    sbtlCallBtlSpriteData2 = 0;
    sbtlEscapeGaugeData = -1;
    sbtlCallCheckDying2Data = -1;
    sbtlCallCheckDying3Data = -1;
    sbtlCallCheckDying2Data2 = -1;
    sbtlCallCheckDying3Data2 = -1;
    sbtlCallCheckDying2Data3 = -1;
    sbtlCallCheckDying3Data3 = -1;
    sbtlCallCheckDying3Data4 = -1;
    sbtlCallEventBattleStopChack2Data2 = -1;
    Init_sbtlDrawData = -1;
    sbtlData122 = -1;
    Init_sbtlDrawData2 = -1;
    sbtlData123 = -1;
}

INCLUDE_ASM(const s32, "code_002A38D8", sbtlInitDevil);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlLeaf121);

void sbtlSpriteEx(s32 a_x, s32 a_y, u8 a_alpha, s32 a_sprNo, DDS3_OT a_ot)
{
    itfCallSetOt2(a_x, a_y, 0, a_alpha, wk_sbtl->panel->hPanelSpr3, a_sprNo, a_ot);
}

void sbtlSprite(s32 a_x, s32 a_y, u8 a_alpha, s32 a_sprNo)
{
    sbtlSpriteEx(a_x, a_y, a_alpha, a_sprNo, 103);
}

void sbtlRefSbtl55(s32 a_x, s32 a_y, u32 a_alphaRate, DDS3_OT a_ot, itfSprList_t *a_list)
{
    itfDrawMiscSpriteList(a_x, a_y, 0, a_alphaRate, wk_sbtl->panel->hPanelSpr3, 103, a_list);
}

void sbtlLeaf93(s32 a_x, s32 a_y, u32 a_alphaRate, itfSprList_t *a_list)
{
    sbtlRefSbtl55(a_x, a_y, a_alphaRate, 103, a_list);
}

void sbtlNum(s32 a_x, s32 a_y, u32 a_color, u8 a_alpha, s32 a_num, DDS3_OT a_ot) {
    s32 i = 0;
    s32 figure = itfGetFigure(a_num);
    s32 sprNo;
    s32 j;

    for (; i < figure; i++) {
        sprNo = itfGetFigureNum(a_num, figure - i);
        if (sprNo >= 0) {
            sprNo += 70;
            for (j = 0; j < 4; j++) {
                wk_sbtl->panel->hPanelSpr3->workEntries[sprNo].geometry.cornerColors[j] = a_color;
            }
            sbtlSpriteEx(a_x, a_y, a_alpha, sprNo, a_ot);
            a_x += 9;
        }
    }
}

s32 sbtlCallGetSmallFontFRQ4(s32 x, s32 y, u8 bRight, u32 color, u8 alpha, s32 ot, void *str) {
    frFontHandle_t *frq;
    s32 width;

    if (str == NULL) {
        return -1;
    }
    frq = frwGetSmallFontFRQ2(x << 4, y << 3, 0, (color & ~0xFF) + (u8)((color & 0xFF) * alpha / 255), str, 0);
    frSetFontSpace(frq, 0);
    width = frq->advance;
    if (bRight) {
        frSetDrawPosition(frq, (x - width) << 4, y << 3);
    }
    frDrawFontOt(frq, 1, ot);
    frDestroyFontRequestAfter(frq);
    return width;
}

INCLUDE_ASM(const s32, "code_002A38D8", sbtlNumFont);

void sbtlDrawRedGg(s32 a_x, s32 a_y, u8 a_alphaRate, s32 a_sprNo, DDS3_OT a_ot)
{
    itfIconDrawSprite(a_x, a_y, 0, a_alphaRate, a_sprNo, a_ot);
}

void sbtlGetIdling_HD(s32 a_x, s32 a_y, u8 a_alphaRate, s32 a_otNo, struct sdatUnitWork_t *unit) {
    if (unit != NULL) {
        itfIconDrawBadStatus(a_x, a_y, 0, a_alphaRate, sbtlGetBadStatus(unit), a_otNo);
    }
}

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrSummoner);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlStrKuzunoha);

void sbtlCallBtlCheckAisyoOld8(s32 a_x, s32 a_y, u8 a_alphaRate, DDS3_OT a_ot, struct SBTLUNIT_WORK *wk) {
    s32 type;
    s32 sprNo;
    u32 aisyo;

    if (wk == NULL) {
        return;
    }
    if (wk->field384 > 0) {
        type = sbtlCheckAisyo_Old(wk->field37C);
        if (type < 0) {
            return;
        }
        aisyo = sbtlSEquationno03Sub(&wk->unit.aisyo, wk->field37C);
        sprNo = -1;
        if (aisyo == 0x2000) {
            switch (type) {
                case 2:
                    sprNo = 28;
                    break;
                case 3:
                    sprNo = 29;
                    break;
                case 4:
                    sprNo = 30;
                    break;
                case 5:
                    sprNo = 52;
                    break;
                case 0:
                    sprNo = 31;
                    break;
            }
        } else if (aisyo == 0x1000) {
            switch (type) {
                case 2:
                    sprNo = 21;
                    break;
                case 3:
                    sprNo = 22;
                    break;
                case 4:
                    sprNo = 23;
                    break;
                case 5:
                    sprNo = 24;
                    break;
                case 0:
                    sprNo = 25;
                    break;
                case 6:
                    sprNo = 26;
                    break;
                case 7:
                    sprNo = 27;
                    break;
            }
        }
        if (sprNo >= 0) {
            sbtlDrawRedGg(a_x, a_y, a_alphaRate, sprNo, a_ot);
            return;
        }
    } else if (wk->field362 > 0) {
        sbtlDrawRedGg(a_x, a_y, a_alphaRate, 17, a_ot);
        return;
    } else if (wk->field360 > 0) {
        if (wk->field358 & 0x1000000) {
            sprNo = 18;
        } else if (wk->field358 & 0x2000000) {
            sprNo = 20;
        } else {
            sprNo = 19;
        }
        if (sprNo >= 0) {
            sbtlDrawRedGg(a_x, a_y, a_alphaRate, sprNo, a_ot);
            return;
        }
    }
}

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlGetViceLevel);

/* NON_MATCHING: 78 of 126 words match. Retail keeps the unit pointer in t1; ours copies it to a0 at entry, which moves max. */
#ifdef NON_MATCHING
void sbtlCallCheckDying(u16 mode, s32 x, s32 y, u8 alpha, DDS3_OT ot, sdatUnitWork_t *unit, s8 bUnknown) {
    s32 num;
    s32 max;
    f32 rate;
    u32 color;
    char str[16];
    u8 type = mode;

    if (unit == NULL) {
        return;
    }
    if (type >= 4) {
        return;
    }
    if (type < 2) {
        num = unit->now.hp;
        max = unit->now.maxhp;
    } else {
        num = unit->now.mp;
        max = unit->now.maxmp;
    }
    if (type == 0) {
        rate = 0.0f;
        if (max > 0) {
            rate = num * 100 / max;
        }
        color = 0x754B3D80;
        if (!datCheckDying(unit)) {
            color = 0xB4A06480;
            if (!(rate < 50.0f)) {
                color = 0xA09DC380;
            }
        }
    } else if (type != 2) {
        num = max;
        color = 0xA09DC380;
    } else {
        color = 0xA09DC380;
    }
    if (mode & 0x100) {
        sbtlNum(x + (3 - itfGetFigure(num)) * 9, y, color, alpha, num, ot);
    } else if (bUnknown) {
        sprintf(str, "???");
        sbtlCallGetSmallFontFRQ4(x, y - 4, 0, 0xA09DC380, alpha, ot, str);
    } else {
        sprintf(str, "%3d", num);
        sbtlNumFont(x, y, color, alpha, ot, str);
    }
}
#else
INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallCheckDying);
#endif

INCLUDE_ASM(const s32, "code_002A38D8", sbtlMagnetiteData);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlLeaf90);

/* NON_MATCHING: 116 of 131 words match. Only s0 and s1 swap (y and alpha). */
#ifdef NON_MATCHING
s32 sbtlCallCheckDying2(struct SBTLUNIT_WORK *wk, s32 x, s32 y) {
    s32 alpha;
    s32 blink;
    s32 hp;

    if (sbtlCallCheckDying2Data3 < 0) {
        sbtlCallCheckDying2Data3 = wk->unit.now.hp;
    }
    if (datCheckDying(&wk->unit)) {
        blink = sbtlCallCheckDying2Data;
        if (blink < 0) {
            sbtlCallCheckDying2Data = blink = 0;
        }
    } else {
        sbtlCallCheckDying2Data = blink = -1;
    }
    hp = wk->unit.now.hp;
    if (hp < sbtlCallCheckDying2Data3) {
        sbtlCallCheckDying2Data2 = 0;
    }
    if (blink < 0 && sbtlCallCheckDying2Data2 < 0) {
        sbtlCallCheckDying2Data3 = hp;
        return 0;
    }
    alpha = 0;
    if (blink >= 0) {
        if (blink < 20) {
            alpha = blink * 110 / 20 + 50;
        } else {
            alpha = 160 - (blink * 110 - 2200) / 20;
        }
    } else if (sbtlCallCheckDying2Data2 >= 0) {
        if (sbtlCallCheckDying2Data2 < 2) {
            alpha = sbtlCallCheckDying2Data2 * 105 / 2 + 75;
        } else {
            alpha = 180 - (sbtlCallCheckDying2Data2 * 105 - 210) / 10;
        }
    }
    if (sbtlCallCheckDying2Data2 >= 0) {
        y += (s32)sbtlLeaf90(sbtlCallCheckDying2Data2);
    }
    sbtlSprite(x, y, 0xFF, 0);
    sbtlSprite(x, y - 12, alpha, 91);
    blink = sbtlCallCheckDying2Data + 1;
    if (sbtlCallCheckDying2Data >= 0) {
        if (blink >= 40) {
            blink = 0;
        }
        sbtlCallCheckDying2Data = blink;
    }
    if (sbtlCallCheckDying2Data2 >= 0) {
        sbtlCallCheckDying2Data2++;
        if (sbtlCallCheckDying2Data2 >= 12) {
            sbtlCallCheckDying2Data2 = -1;
        }
    }
    sbtlCallCheckDying2Data3 = wk->unit.now.hp;
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallCheckDying2);
#endif

/* NON_MATCHING: 96 of 166 words match. Register allocation differs: retail keeps y and num in one s-register. */
#ifdef NON_MATCHING
void sbtlCallEventBattleStopChack2(void) {
    struct sprHandle_t *hSpr = wk_sbtl->panel->hPanelSpr3;
    struct SBTLUNIT_WORK *wk;
    s32 y;
    s32 num;
    u32 color;
    s32 sy;

    if (wk_sbtl->battleMode == 3) {
        return;
    }
    y = (EventBattleStopChack() == 2) ? 364 : 300;
    wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    if (wk == NULL) {
        return;
    }
    if (sbtlCallCheckDying2(wk, 157, y) != 1) {
        sbtlSprite(157, y, 255, 0);
    }
    sy = y + 11;
    sbtlLeaf93(157, y, 255, sbtlCallEventBattleStopChack2Data);
    sbtlCallGetSmallFontFRQ4(16, sy, 0, 0xA09DC380, 255, 103, sbtlGetPlayerName());
    sy = y + 39;
    itfCalcSpriteWidth(hSpr, 37, wk->unit.now.hp, wk->unit.now.maxhp);
    sbtlSprite(55, sy, 255, 37);
    sy = y + 10;
    sbtlCallCheckDying(256, 80, y + 34, 255, 103, &wk->unit, 0);
    sbtlGetIdling_HD(149, sy, 255, 103, &wk->unit);
    sy = y + 51;
    sbtlCallBtlCheckAisyoOld8(128, sy, 255, 103, wk);
    itfIconDrawBullet(19, y + 40, 0, 255, wk_sbtl->equipBullet, 103);
    num = sbtlCallBtlGetPlayerData3();
    color = 0xA09DC380;
    if (num <= 0) {
        color = 0x754B3D80;
    }
    sbtlNum((2 - itfGetFigure(num)) * 9 + 52, sy, color, 255, num, 103);
    num = wk_sbtl->mag;
    color = 0x754B3D80;
    if (num > 0) {
        color = 0xA09DC380;
    }
    sbtlNum((4 - itfGetFigure(num)) * 9 + 108, sy, color, 255, num, 103);
    if (sbtlCallEventBattleStopChack2Data2 < 0) {
        sbtlCallEventBattleStopChack2Data2 = wk_sbtl->mag;
        return;
    }
    if (wk_sbtl->mag < sbtlCallEventBattleStopChack2Data2) {
        sbtlCallGetRandIntA6();
    }
    sbtlCallEventBattleStopChack2Data2 = wk_sbtl->mag;
}
#else
INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallEventBattleStopChack2);
#endif

INCLUDE_ASM(const s32, "code_002A38D8", sbtlFunc23);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlDrawRedGg);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallCheckDying3);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallCheckSkillEnable);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlRefSbtl61);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlRefSbtl59);

/* NON_MATCHING: 225 of 230 words match. Retail uses bnel for the battleMode 3 check where cc1 uses bne; one argument scheduled differently. */
#ifdef NON_MATCHING
void sbtlCallBtlSearchUnitID37(void) {
    struct sprHandle_t *hSpr = wk_sbtl->panel->hPanelSpr3;
    struct SBTLUNIT_WORK *wk = NULL;
    s32 i;
    s32 bCursor;
    s32 bEnable;
    s32 y;
    s32 dying;
    s32 sy;
    sdfVector4_t pos;

    for (i = 0; i < SDAT_STOCK_NUM; i++) {
        if (sbtlCheckBit(&wk_sbtl->summonBit, i) && wk_sbtl->field358[i] == 2) {
            wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[i]);
            break;
        }
    }
    if (wk == NULL) {
        return;
    }
    bCursor = 0;
    if (sbtlData121 && wk_sbtl->battleMode != 3 && CheckLiveTarget(wk)) {
        if (wk->aiwk->mode != 4 || (wk->color & 0xFF000000) > 0x3FFFFFFF) {
            bCursor = 1;
        }
    }
    if (bCursor) {
        sdfModelEvalTm(wk->hEveUnit->owner->inner);
        if (!sdfModelGetMarkPos_sdfModel(wk->hEveUnit->owner->inner, 1)) {
            sdfModelGetMarkPos_sdfModel(wk->hEveUnit->owner->inner, 0);
        }
        VU0_STORE_VF($vf10, &pos);
        sbtlNakamaCursor(&pos);
    }
    if (sbtlCallBtlSearchUnitID37Data2 < 0) {
        sbtlCallBtlSearchUnitID37Data2 = sbtlCallBtlSearchUnitID27();
    }
    bEnable = sbtlCallCheckSkillEnable(wk);
    if (wk_sbtl->battleMode == 3) {
        y = 308;
        bEnable = 0;
    } else {
        y = 364;
    }
    dying = sbtlCallCheckDying3(wk, 157, y);
    if (dying != 1) {
        if (wk_sbtl->battleMode == 3) {
            sbtlSprite(157, 372, 255, 2);
        }
        sbtlSprite(157, y, 255, 1);
        if (bEnable == 1) {
            sbtlRefSbtl61(wk, 157, y);
        }
    }
    if (wk_sbtl->battleMode == 3) {
        sbtlCallBtlDrawRedGg(wk);
    }
    sy = y + 39;
    sbtlLeaf93(157, y, 255, sbtlCallBtlSearchUnitID37Data);
    sbtlCallGetSmallFontFRQ4(16, y + 12, 0, 0xA09DC380, 255, 103, g_itemNo[wk->unit.devilId]);
    itfCalcSpriteWidth(hSpr, 37, wk->unit.now.hp, wk->unit.now.maxhp);
    sbtlSprite(55, sy, 255, 37);
    sy = y + 56;
    sbtlCallCheckDying(256, 80, y + 34, 255, 103, &wk->unit, 0);
    itfCalcSpriteWidth(hSpr, 41, wk->unit.now.mp, wk->unit.now.maxmp);
    sbtlSprite(55, sy, 255, 41);
    sy = y + 11;
    sbtlCallCheckDying(258, 117, y + 51, 255, 103, &wk->unit, 0);
    sbtlGetIdling_HD(149, sy, 255, 103, &wk->unit);
    sbtlCallBtlCheckAisyoOld8(128, sy, 255, 103, wk);
    sbtlCallBtlGetViceLevel(24, y + 40, 255, 103, &wk->unit, bEnable == 1);
    if (dying >= 0) {
        sbtlRefSbtl59(wk, 157, y);
    }
    sbtlCallBtlSearchUnitID37Data2 = sbtlCallBtlSearchUnitID27();
}
#else
INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlSearchUnitID37);
#endif

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlSpriteEx);

void sbtlCallBtlGetPlayerHitTarget(void)
{
    struct SBTLUNIT_WORK *wk;

    if (wk_sbtl->mode != 14) {
        wk = sbtlGetPlayer_HitTarget();
        if (wk != NULL && !(wk->flg & 0x80000000)) {
            sbtlCallBtlSpriteEx(wk);
        }
    }
}

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallPreCalcStrWidth);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlAnalyzeBackEx);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlAnalyzeBack);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlAnalyzeLvEx);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlAnalyzeLv);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallCalcSpriteWidth);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlRefSbtl53);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlAnalyzeHpEx);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlAnalyzeHp);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallCalcSpriteWidth2);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlSkillData);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlAnalyzeAisyo);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlAnalyzeBack);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlAnalyzeBack2);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlAnalyzeBack3);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlAnalyzeBackEx);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlNakamaCursor);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlRefSbtl57);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlBtlFuncAnalyzeSub);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlSearchUnitID29);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallBtlSprite);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallSetFontColor);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlHelpCantUse);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlEscapeGauge);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlResetEscapeGauge);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallSprintf8);

void sbtlCallMoverInit(void)
{
    s32 i;

    for (i = 0; i < 128; i++) {
        itfMoverInit(&sbtlWork7[i]);
    }
}

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallGetRandIntA6);

INCLUDE_ASM(const s32, "code_002A38D8", sbtlCallSetOt3);

INCLUDE_RODATA(const s32, "code_002A38D8", sbtlEscapeGaugeTable);

