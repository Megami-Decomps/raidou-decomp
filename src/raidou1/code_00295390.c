#include "common.h"
#include "dds3Basic.h"
#include "dds3ObjectBase.h"
#include "ps2asm.h"
#include "sdfQuat.h"
#include "sbtlSub.h"
#include "sdk_libc.h"
#include "sdfMemory.h"
#include "sbtlWork.h"
#include "sBtlUnit.h"
#include "EventBit.h"
#include "evt.h"
#include "sBtlVisual.h"
#include "sBtlEncount.h"
#include "dds3Randomize.h"
#include "code_00295390.h"
#include "motion.h"
#include "AW_XAtkPad.h"
#include "fclMisc.h"
#include "sBtlPlayer.h"
#include "code_r1_0027CBE8.h"
#include "scr.h"
#include "dds3KernelCore.h"
#include "sbtlActAI.h"
#include "fld.h"
#include "sBtlDvltk.h"
#include "sBtlAI.h"
#include "code_002929C8.h"
#include "code_00195698.h"
#include "sBtlSM.h"
#include "sBtlScene.h"

/* NON_MATCHING: 114 of 122 words match.  */
#ifdef NON_MATCHING
void sbtlCheckBattleWin(void) {
    s32 win = 0;
    struct SBTLUNIT_WORK *wk;
    s32 i;

    if (sbtlCheckTutorialBattle()) {
        if (!evtBitCheck(1934)) {
            if ((evtBitCheck(1933) || evtBitCheck(248)) && wk_sbtl->stockNum > 0) {
                if (wk_sbtl->field344 > 0) {
                    wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->stockUnitID[wk_sbtl->summonIndex[0]]);
                    if (wk == NULL || !CheckLiveTarget(wk)) {
                        win = 1;
                    }
                } else {
                    win = 1;
                }
            }
            if (!win && !evtBitCheck(1933) && !evtBitCheck(248) && evtBitCheck(1932)) {
                for (i = 11; i >= 0; i--) {
                    if (wk_sbtl->huumaDevilID[i] != -1) {
                        break;
                    }
                }
                if (i < 0) {
                    win = 2;
                }
            }
        }
        if (!win) {
            win = 3;
        }
    }
    switch (win) {
    case 1:
        evtBitOff(2096);
        evtBitOff(2097);
        evtBitOff(2098);
        evtBitOn(2099);
        break;
    case 2:
        evtBitOff(2096);
        evtBitOff(2097);
        evtBitOn(2098);
        evtBitOff(2099);
        break;
    case 3:
        evtBitOn(2096);
        evtBitOff(2097);
        evtBitOff(2098);
        evtBitOff(2099);
        break;
    }
}
#else
INCLUDE_ASM(const s32, "code_00295390", sbtlCheckBattleWin);
#endif

void sbtlCheckBattleLose(void) {
    if (sbtlCheckTutorialBattle()) {
        evtBitOff(2096);
        evtBitOn(2097);
        evtBitOff(2098);
        evtBitOff(2099);
    }
}

s32 sbtlCheckTutorialBattle(void) {
    return evtBitCheck(2100) != 0;
}

s32 sbtlEvent_CheckRain(void) {
    s32 ret = 1;

    if (wk_sbtl->stageNo == 26) {
        switch (evtCallGetProcessData29()) {
        case 0:
            evtStartRain(1);
            ret = 0;
            break;
        case 1:
            ret = 0;
            break;
        case 2:
            ret = 1;
            break;
        }
    }
    return ret;
}

s32 sbtlEvent_LoadEventScript(void) {
    if (wk_sbtl->flg & 0x10000000) {
        return 1;
    }
    switch (wk_sbtl->evtloadst) {
    case 0:
        wk_sbtl->evtloadFile = fileLoad("/battle/event/sb_btlevent.bf");
        wk_sbtl->evtloadst++;
        break;
    case 1:
        if (wk_sbtl->evtloadpData == NULL && pacFileCheckEnd(wk_sbtl->evtloadFile)) {
            wk_sbtl->evtloadHandle = fileGetMemHandle(wk_sbtl->evtloadFile);
            wk_sbtl->evtloadpData = sdfMemLock(wk_sbtl->evtloadHandle);
            fileHandleFree(wk_sbtl->evtloadFile);
            wk_sbtl->evtloadFile = NULL;
            wk_sbtl->flg |= 0x10000000;
            return 1;
        }
        break;
    default:
        return 0;
    }
    return 0;
}

s32 sbtlEvent_DeleteEventScript(void) {
    s32 ret = 1;

    if (wk_sbtl->flg & 0x10000000) {
        if (wk_sbtl->evtloadHandle != NULL) {
            sdfMemFreeDirect(wk_sbtl->evtloadHandle);
            wk_sbtl->evtloadHandle = NULL;
            wk_sbtl->evtloadst = 0;
        }
    } else {
        ret = wk_sbtl->evtloadst == 0;
    }
    return ret;
}

s32 sbtlEvent_SetCharacterProc(s32 negoid) {
    u8 *mes[3] = { sbtlEvent_SetCharacterProcData, sbtlEvent_SetCharacterProcData2, sbtlEvent_SetCharacterProcData3 };
    s32 mesno0[20] = { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 3, 0, 0, 0, 4, 0, 0, 5, 0 };
    s32 mesno1[20] = { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 3, 0, 0, 0, 4, 0, 0, 5, 0 };
    s32 mesno2[20] = { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 3, 0, 0, 0, 4, 0, 0, 5, 0 };
    s32 *mesno[3] = { mesno0, mesno1, mesno2 };

    wk_sbtl->nego_chrid = negoid;
    return sbtlEvent_NegoProc(mes[wk_sbtl->field2D8], mesno[wk_sbtl->field2D8][wk_sbtl->stockDevilSkill[wk_sbtl->summonIndex[0]]]) != 0;
}

u8 *sbtlGetEventMes(s16 eventno) {
    u8 *mes[8] = {
        sbtlGetEventMesData4, sbtlGetEventMesData,  sbtlGetEventMesData2, sbtlGetEventMesData2,
        sbtlGetEventMesData3, sbtlGetEventMesData3, sbtlGetEventMesData4, sbtlGetEventMesData4,
    };

    return mes[eventno];
}

s32 sbtlEvent_NegoProc(u8 *adr, s32 no) {
    switch (wk_sbtl->nego_gstat) {
    case 0:
        fclInitMessage(adr);
        wk_sbtl->flg |= 0x80;
        wk_sbtl->nego_gstat++;
        mphStartLoadID(wk_sbtl->nego_chrid);
        return 0;
    case 1:
        if (!sbtlRefSbtl28()) {
            return 0;
        }
        if (wk_sbtl->nego_chrid >= 4) {
            mphStartDrawBtlLeft();
        } else {
            mphStartDrawBtlRight();
        }
        wk_sbtl->nego_gstat++;
        return 0;
    case 2:
        wk_sbtl->nego_alpha += 16;
        wk_sbtl->nego_alpha = (wk_sbtl->nego_alpha <= 0) ? 0 : (wk_sbtl->nego_alpha < 129) ? wk_sbtl->nego_alpha : 128;
        if (wk_sbtl->nego_alpha >= 128) {
            fclStartMessage(no);
            fclChangeMessageWindowAlpha(1);
            wk_sbtl->nego_gstat++;
        }
        return 0;
    case 3:
        if (fclChkMessage() == 1) {
            fclUpdateMessage();
        } else {
            wk_sbtl->nego_gstat++;
        }
        return 0;
    case 4:
        wk_sbtl->nego_alpha -= 16;
        wk_sbtl->nego_alpha = (wk_sbtl->nego_alpha <= 0) ? 0 : (wk_sbtl->nego_alpha < 129) ? wk_sbtl->nego_alpha : 128;
        if (wk_sbtl->nego_alpha <= 0) {
            wk_sbtl->nego_gstat++;
        }
        return 0;
    case 5:
        fclDestroyMessage();
        fldConsumeSceneInputFlags();
        wk_sbtl->nego_gstat = 0;
        return 1;
    }
    return 1;
}

s32 sbtlMessageProc(u8 *adr, s32 no, struct SBTLUNIT_WORK *wk, s32 flg) {
    switch (wk_sbtl->nego_gstat) {
    case 0:
        fclInitMessage(adr);
        wk_sbtl->nego_gstat++;
        return 0;
    case 1:
        if (!flg) {
            fclSetMessageVar(0, sbtlData94[wk->unit.raceId]);
            fclSetMessageVar(1, g_itemNo[wk->unit.devilId]);
        } else {
            fclSetMessageVar(0, g_itemNo[sbtlGetDevilData_sbtlSub(wk->unit.devilId)->huumaMesNo]);
        }
        wk_sbtl->nego_gstat++;
        return 0;
    case 2:
        fclStartMessage(no);
        fclChangeMessageWindowAlpha(1);
        wk_sbtl->nego_gstat++;
        return 0;
    case 3:
        if (fclChkMessage() == 1) {
            fclUpdateMessage();
        } else {
            wk_sbtl->nego_gstat++;
        }
        return 0;
    case 4:
        wk_sbtl->nego_gstat++;
        return 0;
    case 5:
        fclDestroyMessage();
        wk_sbtl->nego_gstat = 0;
        return 1;
    default:
        return 1;
    }
}

s32 sbtlRefSbtl29(void) {
    if (wk_sbtl->flg & 0x400) {
        return sbtlRefSbtl29Data[wk_sbtl->eventno]();
    }
    return 0;
}

s32 EventProcTbl_00(void) {
    s16 no[8] = { 2, 3, 3, 3, 3, 0, 1, 3 };

    if (!sbtlEvent_NegoProc(sbtlGetEventMes(wk_sbtl->eventno), no[wk_sbtl->eventno])) {
        return 0;
    }
    fclEndMessage();
    wk_sbtl->flg &= ~0x4C0;
    sbtlActiveAction();
    return 1;
}

s32 EventProcTbl_01(void) {
    s32 i;

    switch (wk_sbtl->eventgstat) {
    case 0:
        if (sbtlEvent_NegoProc(sbtlGetEventMes(wk_sbtl->eventno), 10)) {
            fclEndMessage();
            wk_sbtl->eventgstat++;
        }
        return 0;
    case 1:
        if (sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->eventListID) != NULL) {
            for (i = 0; i < 12; i++) {
                if (sbtlCheckBit(&wk_sbtl->summonBit, i) && wk_sbtl->stockUnitID[i] == wk_sbtl->eventListID) {
                    break;
                }
            }
            if (i >= 12) {
                wk_sbtl->flg &= ~0x4C0;
            } else {
                if (wk_sbtl->field358[i] != 2) {
                    return 0;
                }
                sbtlCallBtlSearchUnitID4(i, 0);
                sbtlResetBit(&wk_sbtl->summonBit, i);
                wk_sbtl->flg &= ~0x4C0;
                sbtlActiveAction();
                wk_sbtl->eventgstat = 0;
                break;
            }
        } else {
            wk_sbtl->flg &= ~0x4C0;
        }
        sbtlActiveAction();
        wk_sbtl->eventgstat = 0;
        return 1;
    case 2:
        break;
    }
    return 0;
}

s32 EventProcTbl_02(void) {
    s32 ret = 0;

    wk_sbtl->flg2 |= 0x80;
    sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->eventListID);
    wk_sbtl->eventcounter++;
    wk_sbtl->eventcounter = (wk_sbtl->eventcounter <= 0) ? 0 : (wk_sbtl->eventcounter < 10001) ? wk_sbtl->eventcounter : 10000;
    switch (wk_sbtl->eventgstat) {
    case 0:
        sbtlDefaultZoom2(15);
        wk_sbtl->eventcounter = 0;
        wk_sbtl->eventgstat++;
        break;
    case 1:
        if (wk_sbtl->eventcounter < 15) {
            break;
        }
        wk_sbtl->eventgstat = 0;
        ret = 1;
        RestartBattle();
        wk_sbtl->flg &= ~0x400;
        wk_sbtl->flg2 &= ~0x80;
        sbtlResetNSTOP();
        break;
    }
    return ret;
}

/* NON_MATCHING: 90 of 91 words match.  */
#ifdef NON_MATCHING
s32 EventProcTbl_03(void) {
    struct SBTLUNIT_WORK *wk;

    wk_sbtl->flg2 |= 0x80;
    wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->huumaTargetID);
    sbtlGetDevilData_sbtlSub(wk->unit.devilId);
    switch (wk_sbtl->eventgstat) {
    case 0:
        sbtlSetMapFade_H(10, 0x80202020, 1);
        wk_sbtl->eventgstat++;
        return 0;
    case 1:
        if (!(wk_sbtl->startFlg & 0x10)) {
            if (sbtlMessageProc(wk->msgData, wk_sbtl->field4CE, wk, 1)) {
                fclEndMessage();
                wk_sbtl->eventgstat++;
            }
        }
        break;
    case 2:
        sbtlSetMapFade_H(10, 0x80808080, 0);
        wk_sbtl->eventgstat++;
        return 0;
    case 3:
        if (wk_sbtl->startFlg & 0x10) {
            break;
        }
        wk_sbtl->eventgstat = 0;
        wk_sbtl->huumaTargetID = -1;
        RestartBattle();
        wk_sbtl->flg &= ~0x400;
        wk_sbtl->flg2 &= ~0x80;
        sbtlResetNSTOP();
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_00295390", EventProcTbl_03);
#endif

s32 EventProcTbl_04(void) {
    struct SBTLUNIT_WORK *wk;

    wk_sbtl->flg2 |= 0x80;
    wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->huumaTargetID);
    sbtlGetDevilData_sbtlSub(wk->unit.devilId);
    switch (wk_sbtl->eventgstat) {
    case 0:
        wk_sbtl->field4CE = 38;
        wk_sbtl->eventgstat++;
        return 0;
    case 1:
        if (sbtlMessageProc(wk->msgData, wk_sbtl->field4CE, wk, 1)) {
            fclEndMessage();
            wk_sbtl->eventgstat++;
        }
        return 0;
    case 2:
        wk_sbtl->eventgstat = 0;
        wk_sbtl->flg &= ~0x400;
        wk_sbtl->flg2 &= ~0x80;
        sbtlResetNSTOP();
        return 1;
    }
    return 0;
}

s32 EventProcTbl_05(void) {
    s32 no[2] = { 0, 0 };
    struct SBTLUNIT_WORK *wk;
    s32 mesno;

    wk_sbtl->flg2 |= 0x80;
    wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->eventListID);
    mesno = (wk != NULL) ? no[0] : no[1];
    switch (wk_sbtl->eventgstat) {
    case 0:
        if (sbtlMessageProc(sbtlGetEventMes(wk_sbtl->eventno), mesno, wk, 0)) {
            fclEndMessage();
            wk_sbtl->eventgstat++;
        }
        return 0;
    case 1:
        wk_sbtl->eventgstat = 0;
        RestartBattle();
        wk_sbtl->flg &= ~0x400;
        wk_sbtl->flg2 &= ~0x80;
        sbtlResetNSTOP();
        return 1;
    case 2:
        break;
    }
    return 0;
}

s32 sbtlLoadHuuma_MesData(void) {
    struct SBTLUNIT_WORK *wk;
    s16 mesno;

    if (sbtlCheckMsgMemType()) {
        return 1;
    }
    wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->huumaTargetID);
    mesno = sbtlGetDevilData_sbtlSub(wk->unit.devilId)->huumaMesNo;
    if (wk->flg2 & 1) {
        return 1;
    }
    switch (wk->msgState) {
    case 0:
        wk->msgFile = fileLoad(sbtlData17[sbtlBtlLoadHuumaMesDataSub(mesno)]);
        wk->msgState++;
        break;
    case 1:
        if (wk->msgData != NULL) {
            break;
        }
        if (!pacFileCheckEnd(wk->msgFile)) {
            break;
        }
        wk->msgHandle = fileGetMemHandle(wk->msgFile);
        wk->msgData = sdfMemLock(wk->msgHandle);
        fileHandleFree(wk->msgFile);
        wk->msgFile = NULL;
        wk->msgState++;
        wk->flg2 |= 1;
        return 1;
    }
    return 0;
}

s32 EventProcTbl_07(void) {
    struct SBTLUNIT_WORK *wk;

    wk_sbtl->flg2 |= 0x80;
    wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->huumaTargetID);
    sbtlGetDevilData_sbtlSub(wk->unit.devilId);
    switch (wk_sbtl->eventgstat) {
    case 0:
        wk_sbtl->field4CE = GetIntervalRand(35, 37);
        wk_sbtl->eventgstat++;
        return 0;
    case 1:
        if (sbtlMessageProc(wk->msgData, wk_sbtl->field4CE, wk, 1)) {
            fclEndMessage();
            wk_sbtl->eventgstat++;
        }
        return 0;
    case 2:
        wk_sbtl->flg2 &= ~0x80;
        sbtlResetNSTOP();
        wk_sbtl->eventgstat = 0;
        RestartBattle();
        wk_sbtl->flg &= ~0x400;
        return 1;
    }
    return 0;
}

/* NON_MATCHING: 17 of 34 words match.  */
#ifdef NON_MATCHING
s32 EventProcTbl_06(void) {
    wk_sbtl->eventcounter++;
    if (wk_sbtl->eventcounter <= 0) {
        wk_sbtl->eventcounter = 0;
    } else if (wk_sbtl->eventcounter > 10000) {
        wk_sbtl->eventcounter = 10000;
    }
    if (wk_sbtl->eventcounter < 90) {
        return 0;
    }
    wk_sbtl->eventcounter = 0;
    wk_sbtl->eventgstat = 0;
    RestartBattle();
    wk_sbtl->flg &= ~0x400;
    sbtlResetNSTOP();
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_00295390", EventProcTbl_06);
#endif

s32 EventProcTbl_08(void) {
    struct SBTLUNIT_WORK *wk;

    wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->eventListID);
    switch (wk_sbtl->eventgstat) {
    case 0:
        wk_sbtl->eventgstat++;
        return 0;
    case 1:
        sbtlCallBtlGetMotionVisualData4(wk, 1);
        if (wk->framecnt >= (s32)sbtlCallBtlGetMotionVisualData4(wk, 1)) {
            wk->m_rate = 0.0f;
            wk->action_flg = 1;
            wk_sbtl->eventgstat++;
            sbtlCheckEffectData(wk, SKLID_E_DEADIM, 1, 0, wk->field414, -1, 0);
            wk->framecnt = 0;
        }
        return 0;
    case 2:
        wk->m_rate = 0.0f;
        if (wk->framecnt >= 186) {
            wk_sbtl->eventgstat++;
        }
        return 0;
    case 3:
        wk->m_rate = 0.0f;
        sbtlSetFade(wk, SBTLUNIT_VST_FADEOUT, 0, 0, 3);
        wk_sbtl->eventgstat++;
        return 0;
    case 4:
        wk->m_rate = 0.0f;
        if (!(wk->vstatus & 2)) {
            wk_sbtl->eventgstat++;
        }
        return 0;
    case 5:
        wk->m_rate = 0.5f;
        wk_sbtl->eventgstat++;
        return 0;
    case 6:
        wk_sbtl->eventgstat = 0;
        wk_sbtl->flg &= ~0x400;
        return 1;
    }
    return 0;
}

/* NON_MATCHING: 27 of 52 words match.  */
#ifdef NON_MATCHING
s32 EventProcTbl_09(void) {
    struct SBTLUNIT_WORK *wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->eventListID);

    switch (wk_sbtl->eventgstat) {
    case 0:
        if (!(wk_sbtl->flg & 0x10000000)) {
            return 1;
        }
        wk_sbtl->eventprocID = scrStartScript2(1014, wk_sbtl->evtloadpData, wk_sbtl->evtload_evtno);
        wk_sbtl->eventgstat++;
        sbtlSbattle3();
        return 0;
    case 1:
        if (dds3SearchProcessPriority(1014)) {
            return 0;
        }
        wk_sbtl->eventgstat = 0;
        wk_sbtl->flg &= ~0x400;
        RestartBattle();
        sbtlResetAAI(wk);
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_00295390", EventProcTbl_09);
#endif

s32 EventProcTbl_0A(void) {
    sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->eventListID);
    switch (wk_sbtl->eventgstat) {
    case 0:
        wk_sbtl->eventprocID = scrStartScript2(1014, ai_scr, wk_sbtl->evtload_evtno);
        wk_sbtl->eventgstat++;
        return 0;
    case 1:
        if (dds3SearchProcessPriority(1014)) {
            return 0;
        }
        wk_sbtl->eventgstat = 0;
        wk_sbtl->flg &= ~0x400;
        return 1;
    }
    return 0;
}

s32 sbtlCallBtlGetMotionVisualData3(struct SBTLUNIT_WORK *wk) {
    SBTL_MOTION_VISUAL *visual;

    visual = sbtlGetVisualData(wk->major, wk->minor);
    if (visual == NULL) {
        return 0;
    }
    if ((visual->field2C & 2) && !(wk->flg & 0x1000000)) {
        dds3ObjectBaseOnFlag(wk->pBasic, 0x400);
        dds3ObjectBaseOnFlag(wk->pBasic, 0x20000);
        if (visual->field2C & 4) {
            dds3GetRadiusPoseSet(wk->pBasic, 50.0f);
        } else if (visual->field2C & 8) {
            dds3GetRadiusPoseSet(wk->pBasic, 100.0f);
        } else if (visual->field2C & 0x10) {
            dds3GetRadiusPoseSet(wk->pBasic, 150.0f);
        } else if (visual->field2C & 0x20) {
            dds3GetRadiusPoseSet(wk->pBasic, 200.0f);
        } else {
            dds3GetRadiusPoseSet(wk->pBasic, 100.0f);
        }
    } else {
        dds3ObjectBaseOffFlag(wk->pBasic, 0x400);
    }
    if (visual->field2C & 1) {
        sbtlCallSetOt2(wk, sbtlCallBtlGetMotionVisualData3Data2[visual->field2E], sbtlCallBtlGetMotionVisualData3Data[visual->field2E]);
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_00295390", sbtlCallSetOt2);

MOTIONMOVE_WORK *sbtlGetMotionMoveData(MODELTYPE major, s32 minor) {
    if (major == 0) {
        return &sbtlmotionmove_tbldata_player[minor];
    }
    return &sbtlmotionmv_tbldata[minor];
}

f32 sbtlGetMotionMove(struct SBTLUNIT_WORK *wk, f32 frame) {
    MOTIONMOVE_WORK *mv;
    s32 i;

    mv = sbtlGetMotionMoveData(wk->major, wk->minor);
    if (mv == NULL) {
        return 0.0f;
    }
    if ((wk_sbtl->flg & 0x40) && !(wk->flg & 0x10)) {
        return 0.0f;
    }
    if (wk->status == 9) {
        return 0.0f;
    }
    for (i = 0; i < 10; i++) {
        if (mv->para[i].motionno == wk->next_motionno && mv->para[i].startframe <= frame && frame <= mv->para[i].endframe) {
            return mv->para[i].sokudo;
        }
    }
    return 0.0f;
}

V_POSITION_WORK *sbtlGetEffCylinderData(MODELTYPE major, s32 id) {
    if (major == 0) {
        return &sbtleff_cylindertbl_p[id];
    }
    return &sbtleff_cylindertbl[id];
}

SBTL_MOTION_VISUAL *sbtlGetVisualData(s32 a, s32 id) {
    if (a == 0) {
        return &sbtlData99[id];
    }
    return &sbtlData98[id];
}

SBTL_UNIT_SUB *sbtlGetMotionVisualData(s32 a, s32 id, s32 b) {
    if (a == 0) {
        return &sbtlData99[id].mwk[b];
    }
    return &sbtlData98[id].mwk[b];
}

/* NON_MATCHING: 30 of 35 words match.  */
#ifdef NON_MATCHING
s32 sbtlCheckMotionTiming(struct SBTLUNIT_WORK *wk) {
    s32 ret;

    if (sbtlGetMotionVisualData(wk->major, wk->minor, wk->next_motionno)->flags & 2) {
        return 0;
    }
    ret = sbtlCallBtlGetMotionVisualData2(wk, wk->motionID);
    if (ret || wk->motionID == 1 || wk->motionID == 14 || !(wk->hModel->first->frame >= 10.0f)) {
        return 1;
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "code_00295390", sbtlCheckMotionTiming);
#endif

f32 sbtlCallBtlGetMotionVisualData4(struct SBTLUNIT_WORK *wk, s32 type) {
    f32 frame = 0.0f;
    SBTL_MOTION_VISUAL *visual = sbtlGetVisualData(wk->major, wk->minor);
    SBTL_UNIT_SUB *sub = &visual->mwk[wk->next_motionno];
    f32 speed;

    switch (type) {
    case 0:
        frame = sub->e_startrate + wk->beforeframe;
        break;
    case 1:
        frame = sub->m_stoprate + wk->beforeframe;
        break;
    }
    if (sub->flags & 1) {
        speed = sub->field10;
    } else {
        speed = visual->field0C;
    }
    if (speed <= 0.0f) {
        return 0.0f;
    }
    return frame / speed;
}

f32 sbtlGetSETime(struct SBTLUNIT_WORK *wk, s32 motionID) {
    f32 frame = 0.0f;
    SBTL_MOTION_VISUAL *visual;
    SBTL_UNIT_SUB *sub;
    f32 speed;

    if (wk->field1A0 == wk_sbtl->targetUnitID && wk_sbtl->mode == 1) {
        visual = sbtlGetVisualData(1, 321);
        if (motionID >= 27) {
            return frame;
        }
    } else {
        visual = sbtlGetVisualData(wk->major, wk->minor);
    }
    sub = &visual->mwk[motionID];
    frame = sub->se_rate + wk->beforeframe;
    if (sub->flags & 1) {
        speed = sub->field10;
    } else {
        speed = visual->field0C;
    }
    if (speed <= 0.0f) {
        return 0.0f;
    }
    return frame / speed;
}

s32 sbtlGetCylinderData_ID(MODELTYPE major, s32 minor, struct SBTLVISUAL_DATA *data) {
    sdfVector4_t ofs;
    sdfVector4_t rot;
    SBTL_MOTION_VISUAL *visual;
    V_POSITION_WORK *eff;

    memset(&ofs, 0, sizeof(ofs));
    ofs.w = 1.0f;
    memset(&rot, 0, sizeof(rot));
    rot.w = 1.0f;
    visual = sbtlGetVisualData(major, minor);
    eff = sbtlGetEffCylinderData(major, minor);
    ofs.x = visual->field18;
    ofs.y = visual->field1C;
    ofs.z = visual->field20;
    sbtlGetRotateDeg(&rot, 180.0f);
    ToMatrix();
    SDF_VU0_LOADVECTOR(&ofs);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(&ofs);
    data->center.x = ofs.x;
    data->center.y = ofs.y;
    data->center.z = ofs.z;
    data->center.w = ofs.w;
    data->hit_area_r = visual->field10;
    data->width = visual->field24;
    data->height = visual->field28;
    data->e_width = eff->cylinder;
    data->e_height = eff->height;
    return 1;
}

s32 sbtlGetCylinderData(struct SBTLUNIT_WORK *wk, SBTLVISUAL_DATA *data) {
    sdfVector4_t ofs;
    SBTL_MOTION_VISUAL *visual;
    V_POSITION_WORK *eff;

    memset(&ofs, 0, sizeof(ofs));
    ofs.w = 1.0f;
    visual = sbtlGetVisualData(wk->major, wk->minor);
    eff = sbtlGetEffCylinderData(wk->major, wk->minor);
    ofs.x = visual->field18;
    ofs.y = visual->field1C;
    ofs.z = visual->field20;
    sbtlGetRotateDeg(&wk->rot, 180.0f);
    ToMatrix();
    SDF_VU0_LOADVECTOR(&ofs);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(&ofs);
    data->center.x = wk->pos.x + ofs.x;
    data->center.y = wk->pos.y + ofs.y;
    data->center.z = wk->pos.z + ofs.z;
    data->center.w = wk->pos.w;
    data->hit_area_r = visual->field10;
    data->width = visual->field24;
    data->height = visual->field28;
    data->e_width = eff->cylinder;
    data->e_height = eff->height;
    return 1;
}

/* NON_MATCHING: 153 of 161 words match.  */
#ifdef NON_MATCHING
s32 sbtlCalcAnim_DmgNumPos(SBTLVISUAL_DATA *data, struct SBTLUNIT_WORK *wk) {
    sdfVector4_t rot;
    SBTLVISUAL_DATA vis;
    struct SBTLUNIT_WORK *unit;
    s32 i;
    s32 found;
    f32 minx, maxx, miny, maxy, minz, maxz;
    f32 y;
    f32 sx, sy, sz;

    memset(&rot, 0, sizeof(rot));
    rot.w = 1.0f;
    found = 0;
    maxz = miny = maxx = minz = minx = maxy = 0.0f;
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        unit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (unit == NULL || !sbtlCheckDamageTarget(unit)) {
            continue;
        }
        if (unit->field1A0 != wk->field1A0 && unit->field1A0 != wk->model->field104) {
            continue;
        }
        sbtlGetCylinderData(unit, &vis);
        if (!found) {
            found = 1;
            minx = unit->pos.x;
            minz = unit->pos.z;
            maxz = minz;
            maxx = minx;
            maxy = vis.center.y + vis.height * 0.5f;
            miny = maxy;
            continue;
        }
        if (unit->pos.x < minx) {
            minx = unit->pos.x;
        }
        if (maxx < unit->pos.x) {
            maxx = unit->pos.x;
        }
        y = vis.center.y + vis.height * 0.5f;
        if (maxy < y) {
            maxy = y;
        }
        if (y < miny) {
            miny = y;
        }
        if (unit->pos.z < minz) {
            minz = unit->pos.z;
        }
        if (maxz < unit->pos.z) {
            maxz = unit->pos.z;
        }
    }
    sx = minx + maxx;
    sy = maxy + miny;
    sz = minz + maxz;
    data->center.w = 1.0f;
    data->height = sy;
    data->center.x = sx * 0.5f;
    data->center.y = sy * 0.5f;
    data->center.z = sz * 0.5f;
    if (sz < sx) {
        data->width = sx;
    } else {
        data->width = sz;
    }
    unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk->model->field104);
    if (unit != NULL && sbtlCheckDamageTarget(unit)) {
        sbtlGetQuat(&wk->pos, &unit->pos);
        SDF_VU0_STOREVECTOR(&rot);
    }
    SDF_VU0_LOADVECTOR(&rot);
    SDF_VU0_STOREVECTOR(&data->rot);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_00295390", sbtlCalcAnim_DmgNumPos);
#endif

s32 sbtlCallBtlCheckDamageTarget8(SBTLVISUAL_DATA *data) {
    SBTLVISUAL_DATA vis;
    struct SBTLUNIT_WORK *wk;
    s32 i;
    s32 found;
    f32 minx, maxx, miny, maxy, minz, maxz;
    f32 y;
    f32 sx, sy, sz;

    found = 0;
    maxz = miny = maxx = minz = minx = maxy = 0.0f;
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        wk = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (wk == NULL || !sbtlCheckDamageTarget(wk)) {
            continue;
        }
        sbtlGetCylinderData(wk, &vis);
        if (!found) {
            found = 1;
            minx = wk->pos.x;
            minz = wk->pos.z;
            maxz = minz;
            maxx = minx;
            maxy = vis.center.y + vis.height * 0.5f;
            miny = maxy;
            continue;
        }
        if (wk->pos.x < minx) {
            minx = wk->pos.x;
        }
        if (maxx < wk->pos.x) {
            maxx = wk->pos.x;
        }
        y = vis.center.y + vis.height * 0.5f;
        if (maxy < y) {
            maxy = y;
        }
        if (y < miny) {
            miny = y;
        }
        if (wk->pos.z < minz) {
            minz = wk->pos.z;
        }
        if (maxz < wk->pos.z) {
            maxz = wk->pos.z;
        }
    }
    sx = minx + maxx;
    sy = maxy + miny;
    sz = minz + maxz;
    data->center.w = 1.0f;
    data->height = sy;
    data->center.x = sx * 0.5f;
    data->center.y = sy * 0.5f;
    data->center.z = sz * 0.5f;
    if (sz < sx) {
        data->width = sx;
    } else {
        data->width = sz;
    }
    return 1;
}

/* NON_MATCHING: 57 of 61 words match.  */
#ifdef NON_MATCHING
s32 sbtlGetPointData(s32 pointID, sdfVector4_t *pos, sdfVector4_t *rot) {
    f32 x, y, z, r;

    if (pointID == 100) {
        pos->x = sb_enc_ex_pos.x;
        pos->y = sb_enc_ex_pos.y;
        pos->z = sb_enc_ex_pos.z;
        pos->w = sb_enc_ex_pos.w;
        rot->x = sb_enc_ex_rot.x;
        rot->y = sb_enc_ex_rot.y;
        rot->z = sb_enc_ex_rot.z;
        rot->w = sb_enc_ex_rot.w;
        return 1;
    } else {
        mapEncPointGet(pointID, &x, &y, &z, &r);
        pos->x = x;
        pos->y = y;
        pos->z = z;
        pos->w = 1.0f;
        FromEuler(0.0f, r * 0.017453293f, 0.0f);
        SDF_VU0_STOREVECTOR(rot);
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_00295390", sbtlGetPointData);
#endif

void sbtlSetExPos(sdfVector4_t *pos, sdfVector4_t *rot) {
    sb_enc_ex_pos.x = pos->x;
    sb_enc_ex_pos.y = pos->y;
    sb_enc_ex_pos.z = pos->z;
    sb_enc_ex_pos.w = pos->w;
    sb_enc_ex_rot.x = rot->x;
    sb_enc_ex_rot.y = rot->y;
    sb_enc_ex_rot.z = rot->z;
    sb_enc_ex_rot.w = rot->w;
}

/* NON_MATCHING: 106 of 175 words match.  */
#ifdef NON_MATCHING
s32 sbtlCallBitCheck(s32 n) {
    s32 list[16];
    s32 pick[4];
    SB_ENCOUNT1 *enc;
    s32 i;
    s32 j;
    s32 k;
    s32 cnt;
    s32 num;
    s32 r;
    s32 c;

    sbtlCallBitCheckData = n;
    if (!(sbtlData100[n].flags & 6) && evtBitCheck(2993)) {
        for (i = 15; i >= 0; i--) {
            list[i] = -1;
        }
        cnt = 0;
        for (i = 0; i < 13; i++) {
            if (sbtlData100[n].enc1[i].type == 1 || sbtlData100[n].enc1[i].type == 6) {
                list[cnt++] = i;
            }
        }
        if (cnt == 1) {
            num = 1;
            pick[0] = list[0];
        } else {
            num = dds3GetRandIntA(NULL, 2) + 1;
            j = 0;
            for (k = 0; k < num; k++) {
                r = dds3GetRandIntA(NULL, num - k);
                c = 0;
                for (i = 0; i < cnt; i++) {
                    if (list[i] != -1) {
                        if (c++ == r) {
                            pick[j++] = list[i];
                            list[i] = -1;
                        }
                    }
                }
            }
        }
        for (k = 0; k < num; k++) {
            enc = &sbtlData100[n].enc1[pick[k]];
            if (enc->type == 1) {
                sbtlSetEncountData(enc->devil, enc->pos, enc->groupID);
            } else if (enc->type == 6) {
                sbtlAddAWXAtkPadSub(enc->devil, enc->pos, enc->groupID);
            }
        }
        return 1;
    }
    for (i = 0; i < 13; i++) {
        enc = &sbtlData100[n].enc1[i];
        if (enc->type == 1) {
            sbtlSetEncountData(enc->devil, enc->pos, enc->groupID);
        } else if (enc->type == 6) {
            sbtlAddAWXAtkPadSub(enc->devil, enc->pos, enc->groupID);
        } else if (enc->type == 7) {
            sbtlAddAWXAtkPadSub2(enc->devil, enc->pos, enc->groupID);
        }
    }
    sbtlCallBtlClearGeneratorWork(n);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_00295390", sbtlCallBitCheck);
#endif
