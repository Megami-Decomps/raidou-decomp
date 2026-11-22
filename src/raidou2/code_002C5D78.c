#include "common.h"
#include "sbtlWork.h"
#include "sdfMemory.h"
#include "sdk_libc.h"
#include "EventBit.h"
#include "sBtlVisual.h"
#include "sBtlItem.h"
#include "code_r1_0021CE60.h"
#include "sdfFile.h"
#include "sdfSystem.h"
#include "code_002929C8.h"
#include "evt.h"
#include "sBtlEncount.h"
#include "newdata.h"
#include "sdfChip.h"
#include "sBtlPath.h"
#include "sBtlAI.h"
#include "sBtlUnit.h"
#include "code_00295390.h"
#include "fclMisc.h"
#include "sBtlPlayer.h"
#include "scr.h"
#include "dds3KernelCore.h"
#include "sbtlActAI.h"
#include "sBtlDvltk.h"
#include "AW_XAtkPad.h"
#include "code_r1_0027CBE8.h"
#include "sBtlScene.h"
#include "sdfQuat.h"
#include "sbtlSub.h"
#include "fld.h"
#include "code_r1_001CC1D0.h"
#include "code_r1_002CA3F0.h"
#include "model.h"
#include "motion.h"
#include "sBtlCmndMenuNew.h"
#include "dds3Randomize.h"

s32 sbtlCheckLoadFlg(void) {
    u32 load;

    if (wk_sbtl == NULL || wk_sbtl->panel == NULL) {
        return 0;
    }
    load = wk_sbtl->panel->flg & 2;
    return load != 0;
}

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallMemFreeDirect2);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallCheckEnd);

s32 sbtlLeaf77(struct SBTL_PANEL *panel) {
    if (panel == NULL) {
        return 0;
    }
    if (!(panel->flg & 1)) {
        return 0;
    }
    if (!(panel->flg & 2)) {
        return 0;
    }
    switch (panel->mode) {
    case 1:
        break;
    case 0:
        panel->alpha += 16;
        panel->alpha = (panel->alpha <= 0) ? 0 : (panel->alpha > 128) ? 128 : panel->alpha;
        if (panel->alpha >= 128) {
            panel->mode = 1;
        }
        break;
    case 2:
        panel->alpha -= 16;
        panel->alpha = (panel->alpha <= 0) ? 0 : (panel->alpha > 128) ? 128 : panel->alpha;
        if (panel->alpha <= 0) {
            panel->mode = 3;
        }
        break;
    }
    return 1;
}

s32 sbtlLeaf78(void)
{
    return 1;
}

INCLUDE_ASM(const s32, "code_002C5D78", sbtlLeaf79);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlLeaf80);

void sbtlBtlDrawTextSub(s32 x, s32 y, u32 alpha, struct sprHandle_t *handle, s32 sprno)
{
    u32 rgba[4];

    rgba[0] = (handle->workEntries[sprno].savedColors[0] & ~0xFF) | alpha;
    rgba[1] = (handle->workEntries[sprno].savedColors[1] & ~0xFF) | alpha;
    rgba[2] = (handle->workEntries[sprno].savedColors[2] & ~0xFF) | alpha;
    rgba[3] = (handle->workEntries[sprno].savedColors[3] & ~0xFF) | alpha;
    effDrawSpriteWithCornerColors(x * 16, y * 8, 0, rgba, 0, handle, sprno, DDS3_OT_2D_HIGH | DDS3_OT_BACKEX_NORMAL);
}

INCLUDE_ASM(const s32, "code_002C5D78", sbtlLeaf81);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlDrawSprite_RGBA2);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlRefSbtl50);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallBtlSearchUnitID12);

u32 sbtlCallRefSbtl51(void) {
    sbtlSummonMenuProc(sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID));
    return 1;
}

INCLUDE_RODATA(const s32, "code_002C5D78", sbtlRefSbtl50Table);

void sbtlSummonMenuProc(struct SBTLUNIT_WORK *wk) {
    switch (wk_sbtl->field436) {
    case 1:
        sbtlRefSbtl51Data[wk_sbtl->field440](wk, &wk_sbtl->mode);
        break;
    case 2:
        sbtlRefSbtl51Data2[wk_sbtl->field440](wk, &wk_sbtl->mode);
        break;
    case 3:
        sbtlRefSbtl51Data3[wk_sbtl->field440](wk, &wk_sbtl->mode);
        break;
    case 4:
        sbtlRefSbtl51Data4[wk_sbtl->field440](wk, &wk_sbtl->mode);
        break;
    case 5:
        sbtlRefSbtl51Data5[wk_sbtl->field440](wk, &wk_sbtl->mode);
        break;
    case 6:
        sbtlRefSbtl51Data6[wk_sbtl->field440](wk, &wk_sbtl->mode);
        break;
    case 7:
        sbtlRefSbtl51Data7[wk_sbtl->field440](wk, &wk_sbtl->mode);
        break;
    }
}

INCLUDE_ASM(const s32, "code_002C5D78", sbtlResetSummonPanel);

INCLUDE_ASM(const s32, "code_002C5D78", SummonMenuFunc_Open00);

INCLUDE_ASM(const s32, "code_002C5D78", SummonMenuFunc_00);

INCLUDE_ASM(const s32, "code_002C5D78", SummonMenuFunc_10);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlGetMenuStatus);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlResetMenuPara);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCancelMenuPara);

INCLUDE_RODATA(const s32, "code_002C5D78", SummonMenuFunc_10Table);

/* NON_MATCHING: 55 of 63 words match.  */
#ifdef NON_MATCHING
s32 sbtlSetMenuPara(u16 mode, s32 stock_id, SKLID skillid, s32 itemid, s32 targetid, s32 smnid) {
    wk_sbtl->panel->setdata[0].flg = 1;
    wk_sbtl->panel->setdata[0].select_no = mode;
    switch (mode) {
    case SBTLPNL_MENU_S_MEIREI:
        wk_sbtl->panel->setdata[smnid].skillid = skillid;
        wk_sbtl->panel->setdata[smnid].target_p = stock_id;
        wk_sbtl->panel->setdata[smnid].targetid = targetid;
        wk_sbtl->panel->setdata[smnid].meirei_no = mode;
        wk_sbtl->ctrlSmnIdx = smnid;
        break;
    case SBTLPNL_MENU_S_Y_ATK:
    case SBTLPNL_MENU_S_Y_REC:
    case SBTLPNL_MENU_S_Y_HOJ:
    case SBTLPNL_MENU_S_Y_NMP:
    case SBTLPNL_MENU_S_DEF:
        wk_sbtl->ctrlSmnIdx = smnid;
        wk_sbtl->panel->setdata[smnid].meirei_no = mode;
        wk_sbtl->panel->setdata[0].target_p = stock_id;
        break;
    case SBTLPNL_MENU_SUMMON:
    case SBTLPNL_MENU_RETURN:
        wk_sbtl->ctrlSmnIdx = smnid;
        wk_sbtl->panel->setdata[0].target_p = stock_id;
        break;
    case SBTLPNL_MENU_ITEM:
        wk_sbtl->panel->setdata[0].targetid = targetid;
        wk_sbtl->panel->setdata[0].target_p = stock_id;
        wk_sbtl->panel->setdata[0].itemid = itemid;
        break;
    case SBTLPNL_MENU_BULLET:
        wk_sbtl->panel->setdata[0].itemid = itemid;
        break;
    case SBTLPNL_MENU_NONE:
    case SBTLPNL_MENU_ESCAPE:
    case SBTLPNL_MENU_ORDER:
        break;
    }
    wk_sbtl->field436 = 0;
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_002C5D78", sbtlSetMenuPara);
#endif

INCLUDE_ASM(const s32, "code_002C5D78", SummonMenuFunc_50);

INCLUDE_ASM(const s32, "code_002C5D78", SummonMenuFunc_60);

INCLUDE_ASM(const s32, "code_002C5D78", SummonMenuFunc_20);

INCLUDE_ASM(const s32, "code_002C5D78", SummonMenuFunc_30);

INCLUDE_ASM(const s32, "code_002C5D78", SummonMenuFunc_40);

INCLUDE_ASM(const s32, "code_002C5D78", SummonMenuFunc_70);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlResetColorFlag);

INCLUDE_ASM(const s32, "code_002C5D78", CheckSummonTarget);

struct SBTL_MODELBIND_LIST *effAllocateModelBindings(void) {
    struct SBTL_MODELBIND_LIST *list = sdfChipAllocZeroDirect(sizeof(SBTL_MODELBIND_LIST));

    list->num = 0;
    list->top = NULL;
    return list;
}

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallMemAllocDirect6);

s32 Proc_sbtlEXP(struct SBTL_MODELBIND_LIST *lwk) {
    SBTL_MODELBIND *wk;

    if (lwk == NULL) {
        return 0;
    }
    wk = lwk->top;
    if (wk == NULL || lwk->num == 0) {
        return 0;
    }
    if (IsSceanStop()) {
        return 0;
    }
    while (wk != NULL) {
        if (wk->status == 1) {
            if (wk->flg & 1) {
                wk_sbtl->field9D8 += wk->para;
                wk_sbtl->field9D8 = wk_sbtl->field9D8 > 0 ? (wk_sbtl->field9D8 < 1000000 ? wk_sbtl->field9D8 : 999999) : 0;
                sbtlCallBtlSearchUnitID26(wk);
            }
            if (wk->flg & 2) {
                wk_sbtl->field9D8 -= wk->para;
                wk_sbtl->field9D8 = wk_sbtl->field9D8 > 0 ? (wk_sbtl->field9D8 < 1000000 ? wk_sbtl->field9D8 : 999999) : 0;
                sbtlCallBtlSearchUnitID26(wk);
            }
            wk->status = 2;
        }
        wk = wk->next;
    }
    return 1;
}

s32 sbtlCallMemFreeDirect6(struct SBTL_MODELBIND_LIST *stp, u8 flg) {
    struct SBTL_MODELBIND *node;
    struct SBTL_MODELBIND *next;


    if (stp == NULL) {
        return 0;
    }
    node = stp->top;
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
                stp->top = node->next;
                if (stp->top != NULL) {
                    stp->top->prev = NULL;
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

s32 sbtlBattleEnd(SBTL_MODELBIND_LIST *lwk)
{
    sbtlCallMemFreeDirect6(lwk, 1);
    sdfChipFreeDirect(lwk);
    return 1;
}

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallBtlSearchUnitID26);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlLeaf82);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCheckTutorialEnd);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallBtlCheckTutorialBattle);

/* NON_MATCHING: 74 of 82 words match.  */
#ifdef NON_MATCHING
void sbtlCheckBattleWin(void) {
    s32 win = 0;
    struct SBTLUNIT_WORK *wk;

    if (sbtlCheckTutorialBattle()) {
        if (evtBitCheck(248) && wk_sbtl->field64C > 0) {
            if (wk_sbtl->member_num > 0) {
                wk = sbtlGetNakama(0);
                if (wk == NULL || !CheckLiveTarget(wk)) {
                    win = 1;
                }
            } else {
                win = 1;
            }
        }
        if (!win && !evtBitCheck(248)) {
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
INCLUDE_ASM(const s32, "code_002C5D78", sbtlCheckBattleWin);
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

INCLUDE_RODATA(const s32, "code_002C5D78", sbtlCallBtlSearchUnitID26Table);

INCLUDE_RODATA(const s32, "code_002C5D78", sbtlCallBtlSearchUnitID26Table2);

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

INCLUDE_ASM(const s32, "code_002C5D78", sbtlLeaf32);

u8 *sbtlGetEventMes(s16 eventno) {
    u8 *mes[8] = {
        sbtlGetEventMesData4, sbtlGetEventMesData,  sbtlGetEventMesData2, sbtlGetEventMesData2,
        sbtlGetEventMesData3, sbtlGetEventMesData3, sbtlGetEventMesData4, sbtlGetEventMesData4,
    };

    return mes[eventno];
}

INCLUDE_ASM(const s32, "code_002C5D78", sbtlEvent_NegoProc);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlMessageProc);

s32 EventProc(void)
{
    s32 stop = 0;

    if (wk_sbtl->flg2 & 2) {
        stop = 1;
    }
    if (sbtlGetStat() == 2) {
        stop |= 2;
    }
    if (sbtlProcessCheck() && wk_sbtl->eventno != 6) {
        stop |= 4;
    }
    if (sbtlCallGetProcessStat()) {
        stop |= 8;
    }
    if (stop == 0) {
        if (wk_sbtl->flg & 0x400) {
            return EventProcData[wk_sbtl->eventno]();
        }
        return 0;
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

INCLUDE_ASM(const s32, "code_002C5D78", EventProcTbl_01);

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

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallBtlSearchUnitID13);

INCLUDE_ASM(const s32, "code_002C5D78", EventProcTbl_04);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallMemset10);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlLoadHuuma_MesData);

s32 EventProcTbl_07(void) {
    struct SBTLUNIT_WORK *wk;

    wk_sbtl->flg2 |= 0x80;
    wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->huumaTargetID);
    sbtlIfDrawActionSub(wk->unit.devilId);
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

INCLUDE_ASM(const s32, "code_002C5D78", EventProcTbl_06);

INCLUDE_ASM(const s32, "code_002C5D78", EventProcTbl_08);

INCLUDE_ASM(const s32, "code_002C5D78", EventProcTbl_09);

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

INCLUDE_ASM(const s32, "code_002C5D78", sbtlLeaf33);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlRefSbtl17);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlRefSbtl18);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlDrawShadow);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallSetOt2);

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
    if (IsSceanStop() && !(wk->flg & 0x10)) {
        return 0.0f;
    }
    if (wk->status == 9) {
        return 0.0f;
    }
    if (wk->field6CC != 0) {
        return 0.0f;
    }
    for (i = 0; i < 8; i++) {
        if (mv->para[i].motionno == wk->next_motionno && mv->para[i].startframe <= frame && frame <= mv->para[i].endframe) {
            return mv->para[i].sokudo;
        }
    }
    if (wk->major == 0 && wk->minor == 5 && wk->next_motionno == 33 && frame >= 24.0f && frame <= 39.0f) {
        return 30.0f;
    }
    return 0.0f;
}

V_POSITION_WORK *sbtlGetEffCylinderData(MODELTYPE major, s32 id) {
    if (major == 0) {
        return &sbtleff_cylindertbl_p[id];
    }
    return &sbtleff_cylindertbl[id];
}

SBTL_MOTION_VISUAL *sbtlGetMotionVisualData(s32 a, s32 id) {
    if (a == 0) {
        return &sbtlData99[id].head;
    }
    if (id < 416) {
        return &sbtlData167[id].head;
    }
    return &sbtlData168[id - 416].head;
}

SBTL_MOTION_VISUAL *sbtlGetVisualData(s32 a, s32 id) {
    if (a == 0) {
        return &sbtlData99[id].head;
    }
    return &sbtlData168[id].head;
}

SBTL_UNIT_SUB *sbtlGetMotionVisualData_sbtlVisual(s32 a, s32 id, s32 b) {
    if (a == 0) {
        return &sbtlData99[id].mwk[b];
    }
    if (id < 416) {
        return &sbtlData167[id].mwk[b];
    }
    return &sbtlData168[id - 416].mwk[b];
}

s32 sbtlCallBtlGetMotionVisualDataSBtlVi2(struct SBTLUNIT_WORK *wk)
{
    f32 frame;

    if (sbtlGetMotionVisualData_sbtlVisual(wk->major, wk->minor, wk->next_motionno)->flags & 2) {
        return 0;
    }
    if (sbtlCheckLoopMotion(wk, wk->motionID) || wk->motionID == 1 || wk->motionID == 14) {
        return 1;
    }
    if (wk->hModel != NULL) {
        frame = wk->hModel->first->frame;
    } else {
        frame = 0.0f;
    }
    if (frame >= 10.0f) {
        return 0;
    }
    return 1;
}

f32 sbtlGetActionTime(struct SBTLUNIT_WORK *wk, s32 type) {
    f32 frame = 0.0f;
    SBTL_MOTION_VISUAL *visual;
    SBTL_UNIT_SUB *sub;
    f32 speed;

    if (wk->major == 0) {
        visual = sbtlGetVisualData(0, wk->minor);
        sub = &visual->mwk[wk->next_motionno];
        switch (type) {
        case 0:
            frame = sub->e_startrate + wk->beforeframe;
            break;
        case 1:
            frame = sub->m_stoprate + wk->beforeframe;
            break;
        }
    } else {
        visual = sbtlGetMotionVisualData(wk->major, wk->minor);
        sub = &visual->mwk[wk->next_motionno];
        switch (type) {
        case 0:
            frame = sub->e_startrate + wk->beforeframe;
            break;
        case 1:
            frame = sub->m_stoprate + wk->beforeframe;
            break;
        }
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

/* NON_MATCHING: 35 of 48 words match.  */
#ifdef NON_MATCHING
f32 sbtlGetSETime(struct SBTLUNIT_WORK *wk, s32 motionID) {
    SBTL_MOTION_VISUAL *visual;
    SBTL_UNIT_SUB *sub;
    f32 frame;
    f32 speed;

    if (wk->major == 0) {
        visual = sbtlGetVisualData(0, wk->minor);
    } else {
        visual = sbtlGetMotionVisualData(wk->major, wk->minor);
    }
    sub = &visual->mwk[motionID];
    frame = (f32)sub->se_rate + wk->beforeframe;
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
#else
INCLUDE_ASM(const s32, "code_002C5D78", sbtlGetSETime);
#endif

s32 sbtlGetCylinderData_ID(MODELTYPE major, s32 minor, struct SBTLVISUAL_DATA *data) {
    sdfVector4_t ofs;
    sdfVector4_t rot;
    SBTL_MOTION_VISUAL *visual;
    V_POSITION_WORK *eff;

    memset(&ofs, 0, sizeof(ofs));
    ofs.w = 1.0f;
    memset(&rot, 0, sizeof(rot));
    rot.w = 1.0f;
    visual = sbtlGetMotionVisualData(major, minor);
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

INCLUDE_ASM(const s32, "code_002C5D78", sbtlGetCylinderData);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCalcAnim_DmgNumPos);

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

/* NON_MATCHING: 58 of 61 words match.  */
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
INCLUDE_ASM(const s32, "code_002C5D78", sbtlGetPointData);
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

INCLUDE_ASM(const s32, "code_002C5D78", sbtlSetEncountPacData);

INCLUDE_ASM(const s32, "code_002C5D78", SetEncountData_Sub_sbtlEncount);

struct SBTLUNIT_WORK *sbtlSetEncountData(s32 devil, s32 posid, s32 groupID)
{
    return SetEncountData_Sub_sbtlEncount(devil, posid, groupID, 0);
}

struct SBTLUNIT_WORK *sbtlAddAWXAtkPadSub(s32 devilId, s32 point, s32 arg)
{
    return SetEncountData_Sub_sbtlEncount(devilId, point, arg, 1);
}

INCLUDE_ASM(const s32, "code_002C5D78", sbtlAddAWXAtkPadSub2);

struct SBTLUNIT_WORK *sbtlSetEncountData4(s32 devil, s32 point, u32 addst) {
    struct SBTLUNIT_WORK *wk = SetEncountData_Sub_sbtlEncount(devil, point, addst, 1);

    wk->aiwk->scrflag |= 0x800000;
    return wk;
}

INCLUDE_ASM(const s32, "code_002C5D78", sbtlLeaf34);

void sbtlSetactcounter(struct SBTLUNIT_WORK *wk)
{
    f32 rate[5] = { 0.25f, 0.5f, 1.0f, 1.0f, 2.0f };

    if (wk->field414 & 1) {
        wk->actMax = wk->actMax * rate[wk->unit.seikaku];
    }
}

s32 sbtlClearGeneratorWork(void)
{
    s32 i;

    for (i = 0; i < 13; i++) {
        sb_gen_work[i].use = 0;
        sb_gen_work[i].zan_cnt = 0;
        sb_gen_work[i].type_d_sommon_flg = 0;
        sb_gen_work[i].wait = 0;
        sb_gen_work[i].penc = NULL;
    }
    sb_gen_d_flag = 0;
    return 1;
}

s32 sbtlSetGeneratorData(s32 encountno) {
    SB_ENCOUNT1 *enc;
    s32 i;
    s32 j;

    sbtlClearGeneratorWork();
    for (i = 0; i < 13; i++) {
        enc = &sbtlData100[encountno].enc1[i];
        if (enc->type == 2 || enc->type == 3 || enc->type == 4 || enc->type == 5 || enc->type == 8 || enc->type == 9) {
            for (j = 0; j < 13; j++) {
                if (sb_gen_work[j].use == 0) {
                    sb_gen_work[j].use = 1;
                    sb_gen_work[j].zan_cnt = enc->zan_num;
                    sb_gen_work[j].type_d_sommon_flg = 0;
                    if (sbtlCallBitCheckData == 524 && enc->type == 5) {
                        sb_gen_work[j].wait = j;
                    } else {
                        sb_gen_work[j].wait = GetIntervalRand(0, 30);
                    }
                    if (enc->type == 8) {
                        sb_gen_work[j].wait = GetIntervalRand(0, 30) + 100;
                    }
                    sb_gen_work[j].penc = enc;
                    break;
                }
            }
        }
    }
    return 1;
}

void sbtlGeneratorD_On(void) {
    sb_gen_d_flag = 1;
}

s32 sbtlCheckGeneratorGroupID(s32 group)
{
    s32 i;

    for (i = 0; i < 13; i++) {
        if (sb_gen_work[i].use != 0 && sb_gen_work[i].penc->groupID == group && sb_gen_work[i].zan_cnt > 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCheckGeneratorGroupID2);

s32 sbtlCheckGenerator_ND(void) {
    SB_GENERATOR_WORK *gen = sb_gen_work;
    s32 i;

    for (i = 0; i < 13; i++, gen++) {
        if (gen->use && (gen->penc->type == 2 || gen->penc->type == 3 || gen->penc->type == 4) && !(gen->penc->devil == 523 && gen->penc->groupID == 20)
            && sbtlCheckGeneratorGroupID(gen->penc->groupID)) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCheckGenerator_D);

s32 sbtlCheckGeneratorAll(void)
{
    s32 i;

    for (i = 0; i < 13; i++) {
        if (sb_gen_work[i].use == 0) {
            continue;
        }
        if (sb_gen_work[i].penc->devil == 523 && sb_gen_work[i].penc->groupID == 20) {
            continue;
        }
        if (sb_gen_work[i].zan_cnt > 0) {
            return 1;
        }
    }
    return 0;
}

void sbtlSetGeneCam(s16 posID)
{
    sdfVector4_t rot;

    if (wk_sbtl->addEnemyCamCnt != 0) {
        return;
    }
    if (sbtlGetMode() != E_CAMERA_MODE_Normal) {
        return;
    }
    if (wk_sbtl->stageNo != 543 && wk_sbtl->stageNo != 535 && wk_sbtl->stageNo != 547 && wk_sbtl->stageNo != 551
        && wk_sbtl->stageNo != 601 && wk_sbtl->stageNo != 557) {
        wk_sbtl->addEnemyCamCnt = 120;
    }
    sbtlGetPointData(posID, &wk_sbtl->vAddEnemy, &rot);
}

INCLUDE_ASM(const s32, "code_002C5D78", sbtlGeneratorProc);

/* NON_MATCHING: 48 of 58 words match.  */
#ifdef NON_MATCHING
s32 sbtlGetRndEncJoken(s32 id) {
    datRndEncTbl_t *tbl;
    s32 i = 0;

    do {
        tbl = &sbtlData101[id][i];
        if (tbl->flag0 == 0 || (tbl->flag0j == 0 ? !evtBitCheck(tbl->flag0) : evtBitCheck(tbl->flag0))) {
            if (tbl->flag1 == 0) {
                return i;
            }
            if (tbl->flag1j != 0 ? evtBitCheck(tbl->flag1) : !evtBitCheck(tbl->flag1)) {
                return i;
            }
        }
        i++;
    } while (i < 5);
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_002C5D78", sbtlGetRndEncJoken);
#endif

INCLUDE_ASM(const s32, "code_002C5D78", sbtlGetRndEncType);

INCLUDE_ASM(const s32, "code_002C5D78", campUpdateEffectStep);

s32 sbtlGetRndEncMax(s32 id) {
    datRndEncTbl_t *tbl;
    s32 total;
    s32 i;
    s32 base;

    if (id == 0) {
        return 0;
    }
    if (evtBitCheck(2999)) {
        return 0;
    }
    tbl = &sbtlData101[id][sbtlGetRndEncJoken(id)];
    total = 0;
    for (i = 0; i < 8; i++) {
        total += tbl->ritu[i].ritu;
    }
    if (total == 0) {
        return 0;
    }
    if (encmax > 0) {
        return encmax;
    }
    base = datRndEncBase - tbl->normalritu * 48 / 100;
    if (base < 50) {
        base = 50;
    }
    encmax = dds3GetRandIntA(NULL, base * 2 - 96) + 48;
    return encmax;
}

s32 sbtlGetRndEncCnt(void)
{
    return DDS3_GBWK->encountcnt;
}

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallGetRandIntA);

s32 sbtlRefDDS3GBWK3(s32 id) {
    SB_ENCOUNT1 *enc;
    s32 total;
    s32 cnt;
    s32 devil;
    s32 i;

    if (id == 0) {
        return 0;
    }
    total = 0;
    cnt = 0;
    for (i = 0; i < 13; i++) {
        enc = &sbtlData100[id].enc1[i];
        devil = 0;
        if (enc->type == 1 || enc->type == 6) {
            devil = enc->devil;
        }
        if (devil != 0) {
            cnt++;
            total += sbtlIfDrawActionSub(devil)->field04;
        }
    }
    if (cnt == 0) {
        return 0;
    }
    if (total / cnt + DDS3_GBWK->playerData.unit.level / 4 < DDS3_GBWK->playerData.unit.level) {
        return 0;
    }
    return 1;
}

void sbtlResetRndEncCount(void)
{
    DDS3_GBWK->encountkyori = 0.0f;
    DDS3_GBWK->encountcnt = 0;
    encmax = 0;
}

void sbtlRndEncTopPriorityMap(s32 *map, s32 *floor) {
    *map = g_rndenc1_map;
    *floor = g_rndenc1_floor;
}

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallGetRandIntA2);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlGetDevilNum);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallBtlGetDevilNum);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlCallGetRandIntA3);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlSetNextPathData);

s16 sbtlGetPathDataCount(s16 id) {
    return tbldatacnt[id];
}

SBTLENMY_ACTDATA *sbtlGetPathACTData(s16 id, s16 no) {
    return &data[id][no];
}

SBTLENMY_PATHDATA *sbtlGetPathData(s16 id) {
    return &pathdata[id];
}

void sbtlInitEffect(void) {
    sbtlFileEntry_t *entry = sbtlInitEffectData;
    u32 i;

    for (i = 0; i < 11; i++) {
        sbtlCallFileOpen(entry++);
    }
    for (i = 0; i < 0x400; i++) {
        if (sbtlData100[i].mapNo[0] == 37) {
            sbtlData100[i].mapNo[0] = 40;
        }
        if (sbtlData100[i].mapNo[0] == 38) {
            sbtlData100[i].mapNo[0] = 41;
        }
        if (sbtlData100[i].mapNo[0] == 39) {
            sbtlData100[i].mapNo[0] = 42;
        }
        if (sbtlData100[i].mapNo[0] == 41) {
            sbtlData100[i].mapNo[0] = 43;
        }
    }
}

s32 sbtlCallFreeAuto(void **dst, s32 size, s32 *src)
{
    s32 raw = 0;
    s32 len = *src;
    void *old;
    void *buf;
    u32 n;

    if (size != 0 && len != size) {
        len = size;
        raw = 1;
    }
    old = *dst;
    if (old != NULL) {
        sdfFreeAuto(old);
    }
    buf = sdfAllocAuto(len);
    *dst = buf;
    if (!raw) {
        memcpy(buf, src + 1, len);
    } else {
        memcpy(buf, src, len);
    }
    n = (u32)(len + 4) >> 4;
    if ((len + 4) & 0xF) {
        return (n + 1) << 4;
    }
    return n << 4;
}

void sbtlCallFileOpen(sbtlFileEntry_t *entry)
{
    sdfFileCmdHandle_t *file = sdfFileOpen(entry->path);
    s32 size = sdfFileLength(file);
    sdfMemHandle_t *handle = sdfMemAllocFromTailDirect(size);
    u8 *buf = sdfMemLock(handle);
    s32 i;

    sdfFileRead(file, buf, size);
    sdfFileClose(file);
    for (i = 0; entry->slot[i].dst != NULL; i++) {
        buf += sbtlCallFreeAuto(entry->slot[i].dst, entry->slot[i].arg, (s32 *)buf);
    }
    sdfMemFreeDirect(handle);
}

SBTLITEM_TBL *sbtlGetItemtbl(s32 id)
{
    return &tbldata[id];
}

INCLUDE_ASM(const s32, "code_002C5D78", sbtlRefSbtl19);

SBTLITEM_LISTWORK *sbtlCreateItemList(void)
{
    SBTLITEM_LISTWORK *lwk = sdfChipAllocZeroDirect(sizeof(SBTLITEM_LISTWORK));

    lwk->handle = NULL;
    lwk->num = 0;
    return lwk;
}

INCLUDE_ASM(const s32, "code_002C5D78", sbtlEntryItemList);

INCLUDE_ASM(const s32, "code_002C5D78", sbtlEntryItemList_Type);

s32 sbtlReleaseItemList(SBTLITEM_LISTWORK *lwk)
{
    if (lwk->handle != NULL) {
        sdfMemFreeDirect(lwk->handle);
        lwk->handle = NULL;
    }
    lwk->num = 0;
    return 1;
}

