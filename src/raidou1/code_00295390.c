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

INCLUDE_ASM(const s32, "code_00295390", sbtlCheckBattleWin);

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

INCLUDE_ASM(const s32, "code_00295390", sbtlEvent_SetCharacterProc);

INCLUDE_ASM(const s32, "code_00295390", sbtlGetEventMes);

INCLUDE_RODATA(const s32, "code_00295390", sbtlCallMemset20Table);

INCLUDE_RODATA(const s32, "code_00295390", sbtlGetEventMesTable);

INCLUDE_ASM(const s32, "code_00295390", sbtlEvent_NegoProc);

INCLUDE_ASM(const s32, "code_00295390", sbtlMessageProc);

s32 sbtlRefSbtl29(void) {
    if (wk_sbtl->flg & 0x400) {
        return sbtlRefSbtl29Data[wk_sbtl->eventno]();
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_00295390", EventProcTbl_00);

INCLUDE_ASM(const s32, "code_00295390", EventProcTbl_01);

INCLUDE_ASM(const s32, "code_00295390", EventProcTbl_02);

INCLUDE_ASM(const s32, "code_00295390", EventProcTbl_03);

INCLUDE_ASM(const s32, "code_00295390", EventProcTbl_04);

INCLUDE_ASM(const s32, "code_00295390", EventProcTbl_05);

INCLUDE_ASM(const s32, "code_00295390", sbtlLoadHuuma_MesData);

INCLUDE_ASM(const s32, "code_00295390", EventProcTbl_07);

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

INCLUDE_RODATA(const s32, "code_00295390", EventProcTbl_00Table);

INCLUDE_ASM(const s32, "code_00295390", EventProcTbl_08);

INCLUDE_ASM(const s32, "code_00295390", EventProcTbl_09);

INCLUDE_ASM(const s32, "code_00295390", EventProcTbl_0A);

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

INCLUDE_ASM(const s32, "code_00295390", sbtlGetCylinderData_ID);

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

INCLUDE_ASM(const s32, "code_00295390", sbtlCalcAnim_DmgNumPos);

INCLUDE_ASM(const s32, "code_00295390", sbtlCallBtlCheckDamageTarget8);

INCLUDE_ASM(const s32, "code_00295390", sbtlGetPointData);

INCLUDE_ASM(const s32, "code_00295390", sbtlSetExPos);

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
