#include "common.h"
#include "sdf_resource.h"
#include "sBtlCmndMenuNew.h"
#include "sdfMemory.h"
#include "sdk_libc.h"
#include "dds3KernelCore.h"
#include "sbtlWork.h"
#include "itf.h"
#include "sBtlItem.h"
#include "fclMisc.h"
#include "sBtlUnit.h"
#include "newdata.h"
#include "EventBit.h"
#include "sBtlSkill.h"
#include "sbtlDMG.h"
#include "sbtlActAI.h"
#include "code_r1_0027CBE8.h"
#include "code_0027A1E8.h"
#include "model.h"
#include "motion.h"
#include "sBtlAI.h"
#include "ps2asm.h"
#include "sBtlSM.h"
#include "sBtlEff.h"
#include "code_002F5CF0.h"

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallBtlGetSkilltbl11);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallMemset12);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallMemset13);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlChangeWorldtoScreenUnity4);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlTargetNoCursor);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallBtlGetSkilltbl12);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlRefSbtl20);

INCLUDE_ASM(const s32, "code_00309FE0", SetLightChg);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlRefSbtl21);

void sbtlSetCmbBfrPos(void) {
    s32 i;
    struct SBTLUNIT_WORK *pUnit;

    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        pUnit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (pUnit != NULL) {
            pUnit->bOldDispOff = (pUnit->vstatus & 0x1000) != 0;
            SDF_VU0_LOADVECTOR(&pUnit->pos);
        SDF_VU0_STOREVECTOR(&pUnit->vPosBfrCmb);
        }
    }
}

INCLUDE_ASM(const s32, "code_00309FE0", sbtlSetPosCmbBfrPos);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallBtlGetPChara3);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallBtlGetNakama4);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlTIMEUP);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlTIMEUP2);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallBtlGetSkilltbl13);

INCLUDE_RODATA(const s32, "code_00309FE0", sbtlRefSbtl20Table);

INCLUDE_RODATA(const s32, "code_00309FE0", sbtlRefSbtl20Table2);

INCLUDE_RODATA(const s32, "code_00309FE0", SetLightChgTable);

INCLUDE_RODATA(const s32, "code_00309FE0", sbtlStrTimeupH);

INCLUDE_RODATA(const s32, "code_00309FE0", sbtlCallBtlGetSkilltbl14Table);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallBtlGetSkilltbl14);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallMemset14);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlLeaf35);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallBtlGetPChara4);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallBtlGetPChara5);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlRefSbtl22);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlSelectTargetCursorSub);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlLeaf36);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlPlayerSetTateHit);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlPlayerSetTateHitEnemy);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlRefSbtl23);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlSetDrawChar);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlPlayerSetEnemyPos);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallBtlGetPChara6);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlDeleteDeBuffAll);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlLeaf37);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData2);

INCLUDE_ASM(const s32, "code_00309FE0", cmpCallBtlGetNakama);

INCLUDE_ASM(const s32, "code_00309FE0", cmpGetModelScale);

s32 cmpCallGetProcessData2(s32 index) {
    sbtlKaiwa_t *wk = dds3GetProcessData(inputpanel_id2);

    return wk->data->fieldD2C[index];
}

INCLUDE_RODATA(const s32, "code_00309FE0", sbtlStrBattleDvlkaiwaDvlkaiwa);

INCLUDE_RODATA(const s32, "code_00309FE0", sbtlStrBattleDvlkaiwa);

INCLUDE_RODATA(const s32, "code_00309FE0", sbtlStrBattleDvlkaiwa2);

INCLUDE_RODATA(const s32, "code_00309FE0", sbtlStrBattleDvlkaiwa3);

INCLUDE_RODATA(const s32, "code_00309FE0", sbtlStrBattleDvlkaiwa4);

INCLUDE_RODATA(const s32, "code_00309FE0", sbtlStrBattleDvlkaiwa5);

INCLUDE_RODATA(const s32, "code_00309FE0", sbtlStrBattleDvlkaiwa6);

INCLUDE_RODATA(const s32, "code_00309FE0", sbtlStrBattleDvlkaiwa7);

INCLUDE_RODATA(const s32, "code_00309FE0", sbtlStrBattleDvlkaiwa8);

INCLUDE_RODATA(const s32, "code_00309FE0", sbtlStrBattleDvlkaiwa9);

INCLUDE_RODATA(const s32, "code_00309FE0", sbtlStrBattleDvlkaiwa10);

INCLUDE_RODATA(const s32, "code_00309FE0", sbtlStrBattleDvlkaiwa11);

INCLUDE_RODATA(const s32, "code_00309FE0", sbtlStrBattleDvlkaiwa12);

INCLUDE_RODATA(const s32, "code_00309FE0", sbtlStrBattleDvlkaiwa13);

INCLUDE_RODATA(const s32, "code_00309FE0", sbtlStrBattleDvlkaiwa14);

INCLUDE_RODATA(const s32, "code_00309FE0", sbtlStrBattleDvlkaiwa15);

INCLUDE_RODATA(const s32, "code_00309FE0", sbtlStrBattleDvlkaiwa16);

INCLUDE_RODATA(const s32, "code_00309FE0", sbtlStrBattleDvlkaiwa17);

INCLUDE_RODATA(const s32, "code_00309FE0", sbtlStrBattleDvlkaiwa18);

INCLUDE_RODATA(const s32, "code_00309FE0", cmpStrBribeThem);

INCLUDE_RODATA(const s32, "code_00309FE0", cmpStrKeepQuiet);

INCLUDE_RODATA(const s32, "code_00309FE0", cmpStrDoNothing);

INCLUDE_RODATA(const s32, "code_00309FE0", cmpStrUseBothDemons);

INCLUDE_ASM(const s32, "code_00309FE0", DeadTextLogGet);

INCLUDE_ASM(const s32, "code_00309FE0", cmpCallGetProcessData3);

INCLUDE_ASM(const s32, "code_00309FE0", cmpCallGetProcessData4);

INCLUDE_ASM(const s32, "code_00309FE0", cmpCallGetProcessData5);

INCLUDE_ASM(const s32, "code_00309FE0", cmpCallBtlCreateTargetList);

s32 cmpCallGetNowSummonerRank(s32 flag) {
    s32 rank = cmpGetNowSummonerRank();
    struct SBTLUNIT_WORK *wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->huumaTargetID);
    s32 per;
    s32 rnd;

    if (GetRand(100) < 30) {
        return 1;
    }
    per = rank * 4 - wk->unit.level + 40;
    if (per > 85) {
        per = 85;
    }
    if (flag == 0) {
        rnd = GetRand(100);
        if (rnd < per) {
            return 1;
        }
    } else {
        rnd = GetRand(100);
        if (per < rnd) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_00309FE0", cmpCallBtlSearchUnitID);

INCLUDE_ASM(const s32, "code_00309FE0", cmpCallGetProcessData6);

INCLUDE_ASM(const s32, "code_00309FE0", cmpCallGetProcessData7);

INCLUDE_ASM(const s32, "code_00309FE0", cmpCallGetProcessData8);

s32 cmpCallBtlSearchUnitID2(void) {
    struct SBTLUNIT_WORK *wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->huumaTargetID);

    if (wk->unit.devilId == 359 && !evtBitCheck(8104)) {
        wk_sbtl->field42A8 = 198;
        return 1;
    }
    if (wk->unit.devilId == 360 && !evtBitCheck(8104)) {
        wk_sbtl->field42A8 = 199;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_00309FE0", cmpTALKRECOVERY);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData3);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData4);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData5);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData6);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlLeaf38);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData7);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData8);

s32 sbtlCallBtlCheckLoopMotion(struct SBTLUNIT_WORK *wk) {
    if (wk->hModel == NULL) {
        return 1;
    }
    if (sbtlCheckLoopMotion(wk, wk->motionID)) {
        return 1;
    }
    return wk->hModel->first->stat == 5;
}

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData9);

INCLUDE_ASM(const s32, "code_00309FE0", Process_ConfigUnity);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData10);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData11);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData12);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlDeadDevilTalkSet);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData13);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData14);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlTALKNETARecovery);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData15);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData16);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData17);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlTALKAinoTe);

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData);

INCLUDE_ASM(const s32, "code_00309FE0", nbTALKNETARecovery);

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData2);

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData3);

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData4);

INCLUDE_ASM(const s32, "code_00309FE0", nbTALKSettokuReovery);

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData5);

INCLUDE_ASM(const s32, "code_00309FE0", nbCommand_AI_SET_CRITICAL);

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData6);

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData7);

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData8);

INCLUDE_RODATA(const s32, "code_00309FE0", nbCallGetProcessData8Table);

INCLUDE_ASM(const s32, "code_00309FE0", fldTALKYoukyuuRecovery);

INCLUDE_ASM(const s32, "code_00309FE0", fldTALKYoukyuuRecovery2);

INCLUDE_ASM(const s32, "code_00309FE0", fldTALKBOUNUSPer);

INCLUDE_ASM(const s32, "code_00309FE0", fldCommand_FLD_KANKYO);

INCLUDE_ASM(const s32, "code_00309FE0", fldCallGetRand);

INCLUDE_ASM(const s32, "code_00309FE0", rcvCallGetProcessData);

INCLUDE_ASM(const s32, "code_00309FE0", rcvAppAllRecover);

INCLUDE_ASM(const s32, "code_00309FE0", rcvSeikouEsc);

INCLUDE_ASM(const s32, "code_00309FE0", rcvCallGetProcessData2);

INCLUDE_ASM(const s32, "code_00309FE0", rcvCallGetProcessData3);

INCLUDE_ASM(const s32, "code_00309FE0", rcvCallGetProcessData4);

INCLUDE_ASM(const s32, "code_00309FE0", rcvCallGetProcessData5);

INCLUDE_ASM(const s32, "code_00309FE0", GuardInit);

INCLUDE_ASM(const s32, "code_00309FE0", rcvCallGetStockDevilNums);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData18);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData19);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData20);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData21);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData22);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData23);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlJumpProc_Eve);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData24);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData25);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData26);

/* NON_MATCHING: 2 of 12 words match.  */
#ifdef NON_MATCHING
s32 sbtlRefDDS3GBWK4(s32 id) {
    s32 index = id >= 209 ? id - 209 : id;

    if (id < 433) {
        return DDS3_GBWK->devilWork[index].field6;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_00309FE0", sbtlRefDDS3GBWK4);
#endif

/* NON_MATCHING: 3 of 12 words match.  */
#ifdef NON_MATCHING
void sbtlRefDDS3GBWK5(s32 id, s32 value) {
    s32 index = id >= 209 ? id - 209 : id;

    if (id < 433) {
        DDS3_GBWK->devilWork[index].field6 = value;
    }
}
#else
INCLUDE_ASM(const s32, "code_00309FE0", sbtlRefDDS3GBWK5);
#endif

/* NON_MATCHING: 1 of 12 words match.  */
#ifdef NON_MATCHING
u16 sbtlGetDevilHuumaItem(s32 id) {
    s32 index = id >= 209 ? id - 207 : id;

    if (id < 433) {
        return DDS3_GBWK->devilWork[index].huumaItem;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_00309FE0", sbtlGetDevilHuumaItem);
#endif

/* NON_MATCHING: 3 of 12 words match.  */
#ifdef NON_MATCHING
void sbtlRefDDS3GBWK6(s32 id, s32 value) {
    s32 index = id >= 209 ? id - 207 : id;

    if (id < 433) {
        DDS3_GBWK->devilWork[index].huumaItem = value;
    }
}
#else
INCLUDE_ASM(const s32, "code_00309FE0", sbtlRefDDS3GBWK6);
#endif

/* NON_MATCHING: 20 of 29 words match.  */
#ifdef NON_MATCHING
void sbtlCallBtlSearchUnitID15(s32 add) {
    struct SBTLUNIT_WORK *wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->huumaTargetID);
    SBTL_DEVIL_DATA *data = sbtlIfDrawActionSub(wk->unit.devilId);
    s16 *value;
    s32 sum;

    if (wk != NULL) {
        value = &DDS3_GBWK->devilWork[data->field82].field2;
        sum = *value + add;
        if (sum > 50) {
            sum = 50;
        }
        *value = sum;
    }
}
#else
INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallBtlSearchUnitID15);
#endif

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallBtlSearchUnitID16);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallBtlSearchUnitID17);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData27);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData28);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData29);

INCLUDE_ASM(const s32, "code_00309FE0", AW_XAtkAdd);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData30);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData31);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData32);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData33);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData34);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData35);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData36);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData37);

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData9);

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData10);

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData11);

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData12);

INCLUDE_ASM(const s32, "code_00309FE0", nbLeaf8);

s32 nbCallGetProcessData13(void) {
    sbtlKaiwa_t *wk = dds3GetProcessData(inputpanel_id2);

    if (slCallGetProcessData7(0) != 0) {
        wk->field1A = wk->field1C;
    }
    return 1;
}

s32 nbCallGetProcessData14(void) {
    sbtlKaiwa_t *wk = dds3GetProcessData(inputpanel_id2);

    if (slCallGetProcessData7(1) == 0) {
        return 1;
    }
    wk->field16 = evtGetCapturedWindowPanelValue();
    if (wk->field46 != 0) {
        wk->field1A = wk->field1C;
        return 1;
    }
    wk->field46 = 0;
    wk->field1A = wk->field1C;
    mapMenuStart();
    return 1;
}

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData15);

s32 nbCallGetProcessData16(void) {
    sbtlKaiwa_t *wk = dds3GetProcessData(inputpanel_id2);

    if (slCallGetProcessData8(0) != 0) {
        wk->field1A = wk->field1C;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData17);

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData18);

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData19);

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData20);

INCLUDE_ASM(const s32, "code_00309FE0", nbLeaf9);

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData21);

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData22);

INCLUDE_ASM(const s32, "code_00309FE0", nbCommand_AI_ACT_ATTACK);

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData23);

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData24);

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData25);

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData26);

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData27);

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetRand);

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData28);

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData29);

INCLUDE_ASM(const s32, "code_00309FE0", slCallGetProcessData);

INCLUDE_ASM(const s32, "code_00309FE0", slCallGetProcessData2);

INCLUDE_ASM(const s32, "code_00309FE0", slCallGetProcessData3);

INCLUDE_ASM(const s32, "code_00309FE0", slCallGetProcessData4);

INCLUDE_ASM(const s32, "code_00309FE0", slRefDDS3GBWK);

INCLUDE_ASM(const s32, "code_00309FE0", slCallGetRand);

INCLUDE_ASM(const s32, "code_00309FE0", slCallGetProcessData5);

INCLUDE_ASM(const s32, "code_00309FE0", slCallGetProcessData6);

INCLUDE_ASM(const s32, "code_00309FE0", slChoiceMsgDraw);

/* NON_MATCHING: 28 of 61 words match.  */
#ifdef NON_MATCHING
s32 slCallGetProcessData7(s32 arg0) {
    sbtlKaiwa_t *kwk = dds3GetProcessData(inputpanel_id2);
    struct SBTLUNIT_WORK *unit;

    wk_sbtl->flg2 |= 0x80;
    unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->huumaTargetID);
    sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    switch (wk_sbtl->eventgstat) {
        case 0:
            if (slCallGetProcessData6(kwk->msg[0].pData, wk_sbtl->field42A8, unit, 1, wk_sbtl->field42AE, arg0) != 0) {
                fclEndMessage();
                wk_sbtl->eventgstat++;
            }
            break;
        case 1:
            wk_sbtl->eventgstat = 0;
            wk_sbtl->flg &= ~0x400;
            return 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_00309FE0", slCallGetProcessData7);
#endif

/* NON_MATCHING: 28 of 61 words match.  */
#ifdef NON_MATCHING
s32 slCallGetProcessData8(s32 arg0) {
    sbtlKaiwa_t *kwk = dds3GetProcessData(inputpanel_id2);
    struct SBTLUNIT_WORK *unit;

    wk_sbtl->flg2 |= 0x80;
    unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->huumaTargetID);
    sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    switch (wk_sbtl->eventgstat) {
        case 0:
            if (slCallGetProcessData6(kwk->msg[1].pData, wk_sbtl->field42A8, unit, 2, wk_sbtl->field42AE, arg0) != 0) {
                fclEndMessageEx(0);
                wk_sbtl->eventgstat++;
            }
            break;
        case 1:
            wk_sbtl->eventgstat = 0;
            wk_sbtl->flg &= ~0x400;
            return 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_00309FE0", slCallGetProcessData8);
#endif

INCLUDE_ASM(const s32, "code_00309FE0", slCallGetProcessData9);

INCLUDE_ASM(const s32, "code_00309FE0", slCallGetProcessData10);

s32 slCallBtlSearchUnitID(void) {
    struct SBTLUNIT_WORK *wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->huumaTargetID);
    SBTL_DEVIL_DATA *data = sbtlIfDrawActionSub(wk->unit.devilId);
    sdatUnitWork_t *stock;
    s32 i;

    for (i = 0; i < wk_sbtl->field64C; i++) {
        stock = sbtlRefDDS3GBWK2(i);
        if (stock != NULL && stock->devilId == data->field82) {
            return 0;
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_00309FE0", slRefSbtl);

INCLUDE_ASM(const s32, "code_00309FE0", slCallBitCheck2);

s32 trmCallBtlSearchUnitID(void) {
    struct SBTLUNIT_WORK *wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    struct SBTLUNIT_WORK *partner;
    SBTL_STAGE_DATA *stage;

    if (wk_sbtl->field42C0 == 4) {
        return 1;
    }
    stage = &sbtlData100[wk_sbtl->stageNo];
    if (wk_sbtl->battleMode != 0 && wk_sbtl->battleMode != 2) {
        return 0;
    }
    if (stage->flags & 6) {
        return 0;
    }
    partner = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->huumaTargetID);
    if (partner != NULL && wk_sbtl->field42C0 != 3 && partner->field6E9) {
        sbtlCreateSM(wk_sbtl->sysmsgList, wk, SBTLSM_MODE_HUUMAMISS_NORMAL, SKLID_NONE);
        return 0;
    }
    return slCallBitCheck2(partner->unit.devilId, 0) == 0;
}

s32 trmCallGetProcessData(void) {
    sbtlKaiwa_t *wk = dds3GetProcessData(inputpanel_id2);
    struct SBTLUNIT_WORK *unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->huumaTargetID);
    SBTL_DEVIL_DATA *data = sbtlIfDrawActionSub(unit->unit.devilId);
    s32 index = data->field82;

    if (DDS3_GBWK->devilWork[index].field0 != 0 && !wk_sbtl->field42DD) {
        if (DDS3_GBWK->devilWork[index].field0 == 1) {
            wk->field98 = 1;
        }
        if (DDS3_GBWK->devilWork[index].field0 == 2) {
            wk->field98 = 2;
        }
        if (DDS3_GBWK->devilWork[index].field0 == 4) {
            wk->field98 = 4;
        }
        if (DDS3_GBWK->devilWork[index].field0 == 3) {
            wk->field98 = 3;
        }
        DDS3_GBWK->devilWork[index].field0 = 0;
        return 0;
    }
    wk->field98 = 0;
    return 1;
}

s32 trmBtlAnalyzeMeisiSub(s16 id) {
    return datRefDDS3GBWK20(id);
}

INCLUDE_ASM(const s32, "code_00309FE0", trmLeaf);

INCLUDE_ASM(const s32, "code_00309FE0", trmRefDDS3GBWK);

INCLUDE_ASM(const s32, "code_00309FE0", trmCallBtlSearchUnitID2);

/* NON_MATCHING: 4 of 13 words match.  */
#ifdef NON_MATCHING
s32 trmSetProcessTerm(void) {
    struct SBTLUNIT_WORK *unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->huumaTargetID);

    if (unit != NULL) {
        unit->field6E9 = 1;
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_00309FE0", trmSetProcessTerm);
#endif

INCLUDE_ASM(const s32, "code_00309FE0", trmCallBtlSearchUnitID3);

INCLUDE_ASM(const s32, "code_00309FE0", trmCallBtlSearchUnitID4);

void trmCallBtlClearHitC(struct SBTLUNIT_WORK *wk) {
    if (wk->mode < 5 || (wk->flg & 0x800)) {
        return;
    }
    sbtlClearHitC(wk_sbtl->hitList, wk);
    sbtlForceReset(wk_sbtl->effList, wk);
    sbtlResetAAI(wk);
    wk->model->dataflg = 0;
    sbtlResetAction_Flag(wk);
    if (wk->field1A0 == wk_sbtl->targetUnitID) {
        sbtlResetAction(wk);
    } else {
        sbtlSetAIWkStat(wk);
        wk->aiwk->mode = 0;
        ResetAction(wk, 0);
    }
}

INCLUDE_ASM(const s32, "code_00309FE0", trmCallBtlCheckReturnUnit);

INCLUDE_ASM(const s32, "code_00309FE0", ChangeTarget_BSHelper_sbtlAI);

struct SBTLUNIT_LISTWORK *trmCallBtlCreateUnitList(u8 group) {
    struct SBTLUNIT_LISTWORK *list = sbtlCreateUnitList(48);
    struct SBTLUNIT_WORK *pUnit;
    s32 i;

    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        pUnit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (pUnit != NULL && (pUnit->field414 & 0x3F) == group && !(pUnit->flg & 0x1000) && sbtlCheckDamageTarget(pUnit)) {
            sbtlEntryUnitList(list, pUnit);
        }
    }
    return list;
}

INCLUDE_ASM(const s32, "code_00309FE0", SetBSHelper);

INCLUDE_ASM(const s32, "code_00309FE0", trmCallBtlSearchUnitID5);

INCLUDE_ASM(const s32, "code_00309FE0", trmCallBtlSetMode);

INCLUDE_ASM(const s32, "code_00309FE0", trmCallBtlSetMode2);

INCLUDE_ASM(const s32, "code_00309FE0", trmCallBtlSetMode3);

INCLUDE_ASM(const s32, "code_00309FE0", evtCallBtlSearchUnitID);

/* NON_MATCHING: 35 of 41 words match.  */
#ifdef NON_MATCHING
void evtCallMemset3(s32 index, s32 value) {
    s32 flags[4];

    memset(flags, 0, sizeof(flags));
    if (index != 0) {
        flags[index - 1] = value;
    } else {
        flags[2] = value;
        flags[1] = value;
        flags[0] = value;
    }
    if (flags[1]) {
        wk_sbtl->field42CB = 1;
    } else {
        wk_sbtl->field42CB = 0;
    }
    if (flags[1]) {
        wk_sbtl->field42CC = 1;
    } else {
        wk_sbtl->field42CC = 0;
    }
}
#else
INCLUDE_ASM(const s32, "code_00309FE0", evtCallMemset3);
#endif

INCLUDE_ASM(const s32, "code_00309FE0", evtCallMemset4);

INCLUDE_ASM(const s32, "code_00309FE0", WajutsuPosSet);

INCLUDE_ASM(const s32, "code_00309FE0", evtCallMemset5);

INCLUDE_ASM(const s32, "code_00309FE0", evtCallGetProcessData);

INCLUDE_ASM(const s32, "code_00309FE0", evtCallGetProcessData2);

void evtCallBtlSearchUnitID2(void) {
    struct SBTLUNIT_WORK *wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    f32 dz = wk_sbtl->camera.camerapos.z - wk->pos.z;

    if (dz < 0.0f) {
        dz = -dz;
    }
    if (dz < 60.0f) {
        wk->vstatus |= 0x1000;
    } else {
        wk->vstatus &= ~0x1000;
    }
}

void evtCallGetProcessData3(void) {
    sbtlKaiwa_t *kwk = dds3GetProcessData(inputpanel_id2);
    struct SBTLUNIT_WORK *wk = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->huumaTargetID);

    if (wk != NULL && kwk->fieldAB && sbtlCheckMotionNo(wk, SBTLUNIT_MOTION_KIKAN) && sbtlCallBtlCheckLoopMotion(wk)) {
        wk->vstatus |= 0x2000000;
        sbtlSetMotion_sbtlUnit(wk, 0, 6, 0, 2);
        kwk->fieldAB = 0;
    }
}

INCLUDE_ASM(const s32, "code_00309FE0", evtKAIWANoEnemy);

void evtMoonShutdown(dds3ProcessID_t *id) {
    sbtlKaiwa_t *wk = dds3GetProcessData(id);

    if (dds3ExistProcess(wk->drawPID) != 0) {
        dds3KillProcess(wk->drawPID, 0);
        wk->drawPID = NULL;
    }
    if (wk->msg[0].hMem != NULL) {
        sdfMemFreeDirect(wk->msg[0].hMem);
    }
    if (wk->msg[1].hMem != NULL) {
        sdfMemFreeDirect(wk->msg[1].hMem);
    }
    if (wk->msg[2].hMem != NULL) {
        sdfMemFreeDirect(wk->msg[2].hMem);
    }
    if (sbtlWork12[2] != NULL) {
        sdfMemFreeDirect(sbtlWork12[2]);
        sbtlWork12[2] = NULL;
    }
    sdfMemFreeDirect(wk->hMem);
    inputpanel_id2 = NULL;
}

s32 evtCallGetProcessData4(dds3ProcessID_t *pid) {
    sbtlKaiwa_t *wk = dds3GetProcessData(pid);
    s32 id;
    s32 no;

    if (wk->fieldAA) {
        id = itfMenuGetSelectID(wk->menuPID);
        id = cmpCallGetProcessData10Work[id];
        if (id == 100) {
            no = 1;
        } else if (id == 900) {
            no = 2;
        } else {
            no = id + 3;
        }
        sbtlHelp(5, no, 0xFF);
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_00309FE0", cmpPartyAnimFunc);

INCLUDE_ASM(const s32, "code_00309FE0", cmpCallGetProcessData9);

INCLUDE_ASM(const s32, "code_00309FE0", cmpCallGetProcessData10);

INCLUDE_ASM(const s32, "code_00309FE0", cmpPartyListMake_cmpMisc);

void cmpSbtlkaiwaMain(void) {
    sdfMemHandle_t *hMem = sdfMemAllocDirect(sizeof(sbtlKaiwa_t));
    sbtlKaiwa_t *wk = sdfMemLock(hMem);

    memset(wk, 0, sizeof(sbtlKaiwa_t));
    wk->hMem = hMem;
    inputpanel_id2 = dds3StartProcess("sbtlkaiwa_main", 0x3FE, 0, 0, evtKAIWANoEnemy, (dds3ProcessFunc_t)evtMoonShutdown, wk);
    wk->drawPID = dds3StartProcess("sbtlkaiwa_draw", 0x2B15, 0, 0, evtCallGetProcessData4, NULL, wk);
    wk->menuPID = itfMenuProcessStart();
    cmpPartyListMake_cmpMisc(wk->menuPID);
    sbtlReleaseItemList(wk_sbtl->itemList);
    sbtlEntryItemList(wk_sbtl->itemList);
    evtKAIWANoEnemyData2 = 0;
}

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallExistProcess);

s32 sbtlProcessCheck(void) {
    if (inputpanel_id2 == NULL) {
        return 0;
    }
    if (dds3GetProcessStat(inputpanel_id2) == 3 || dds3GetProcessStat(inputpanel_id2) == 0) {
        inputpanel_id2 = NULL;
        return 0;
    }
    return 1;
}

s32 sbtlCallMemFreeDirect3(void) {
    if (sbtlWork12[0] != NULL) {
        sdfMemFreeDirect(sbtlWork12[0]);
    }
    sbtlWork12[0] = sdfDevLoadFileEx(sbtlData136[20], &slWork2[0], &sbtlWork14[0]);
    if (sbtlWork12[1] != NULL) {
        sdfMemFreeDirect(sbtlWork12[1]);
    }
    sbtlWork12[1] = sdfDevLoadFileEx(sbtlData136[19], &slWork2[1], &sbtlWork14[1]);
    return 1;
}

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData38);

s32 btlLoadKaiwaData(void) {
    sdfDevLoadFileEx("/battle/dvlkaiwa/sbtl_kaiwa_data.dat", &sbtlData179, &btlLoadKaiwaDataData);
    return sbtlCallMemFreeDirect3();
}

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallPrimAllocDrawTag);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlLeaf39);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlLeaf40);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlChkEvent);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallMemset15);

INCLUDE_ASM(const s32, "code_00309FE0", ResetBadStatusAll);

INCLUDE_ASM(const s32, "code_00309FE0", CameraPosSet);

INCLUDE_ASM(const s32, "code_00309FE0", PosSet);

INCLUDE_ASM(const s32, "code_00309FE0", PosReSet_sbtlBossEvent);

INCLUDE_ASM(const s32, "code_00309FE0", CameraSet);

INCLUDE_ASM(const s32, "code_00309FE0", ReStartBattle);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlEffect);

INCLUDE_ASM(const s32, "code_00309FE0", BattleRestart_Effect);

INCLUDE_ASM(const s32, "code_00309FE0", BattleMag_Effect);

INCLUDE_ASM(const s32, "code_00309FE0", BattleDead_Effect);

INCLUDE_ASM(const s32, "code_00309FE0", BattleTime_Effect);

INCLUDE_ASM(const s32, "code_00309FE0", BattleSummon_Effect);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlBossEventModelMove);

INCLUDE_ASM(const s32, "code_00309FE0", BattleLose_Effect);

INCLUDE_ASM(const s32, "code_00309FE0", BattleRaidoDying_Effect);

INCLUDE_ASM(const s32, "code_00309FE0", BattleDying_Effect);

INCLUDE_ASM(const s32, "code_00309FE0", BattleDyingSinado1_Effect);

INCLUDE_ASM(const s32, "code_00309FE0", BattleStartApo_Effect);

INCLUDE_ASM(const s32, "code_00309FE0", GetTable);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCheck);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlRefSbtl24);

INCLUDE_ASM(const s32, "code_00309FE0", Process_sbtlBossEvent);

INCLUDE_ASM(const s32, "code_00309FE0", Shutdown);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlProcessStart);

s32 sbtlCallGetProcessStat(void) {
    if (sbtlCallGetProcessStatData == NULL) {
        return 0;
    }
    if (dds3GetProcessStat(sbtlCallGetProcessStatData) == 3 || dds3GetProcessStat(sbtlCallGetProcessStatData) == 0) {
        sbtlCallGetProcessStatData = NULL;
        return 0;
    }
    return 1;
}

void btlLoadBossEventTable(void) {
    sdfDevLoadFileEx("/battle/sb_bossevtbl.dat", &sbtlCheckData, &btlLoadBossEventTableData);
}

INCLUDE_ASM(const s32, "code_00309FE0", sbtlLeaf41);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCheckDeadEventMag);

/* NON_MATCHING: 9 of 37 words match.  */
#ifdef NON_MATCHING
s32 Start_sbtlTime(s32 arg0, s32 arg1) {
    SBTL_WORK *wk = wk_sbtl;
    s32 ret = 0;
    u32 i;
    sbtlTimeTbl_t *tbl;

    if (wk_sbtl->fieldE8C == 0) {
        i = 0;
        tbl = Start_sbtlTimeData;
        do {
            if (arg0 == tbl->field0 && arg1 == tbl->field4 && wk_sbtl->timeState == 0) {
                ret = 1;
                wk->timeMode = tbl->mode;
                wk->timeLimit = tbl->timeLimit;
                wk->field42CE = tbl->fieldD;
                wk_sbtl->timeState = 1;
                sbtlData141 = 0;
                break;
            }
            i++;
            tbl++;
        } while (i < 29);
    }
    Result_sbtlTimeData = 0;
    Result_sbtlTimeData2 = 0;
    return ret;
}
#else
INCLUDE_ASM(const s32, "code_00309FE0", Start_sbtlTime);
#endif

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCheckEnemyAllDead);

INCLUDE_ASM(const s32, "code_00309FE0", Result_sbtlTime);

s32 End_sbtlTime(void) {
    wk_sbtl->timeState = 0;
    wk_sbtl->field42CE = 0;
    return 0;
}

void sbtlRefSbtl25(sbtlSummonText_t *text) {
    sbtlSummonText_t *head;

    text->prev = NULL;
    head = wk_sbtl->textList;
    text->next = head;
    if (head != NULL) {
        head->prev = text;
    }
    wk_sbtl->textList = text;
}

INCLUDE_ASM(const s32, "code_00309FE0", sbtlRefSbtl26);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlRefSbtl27);

void sbtlLeaf42(void) {
}

INCLUDE_ASM(const s32, "code_00309FE0", sbtlFirstSummonTextSet);

/* NON_MATCHING: 20 of 24 words match.  */
#ifdef NON_MATCHING
void sbtlLeaf43(sbtlSummonText_t *text) {
    if (text->type == 1) {
        text->sprNo = text->count + 16;
        text->field58 = 13;
        text->field44 = 80;
        text->field48 = -10;
        text->field3C = 1.5f;
        text->hSpr = sbtlData135.hSpr;
        text->field60 = 0;
        text->field40 = 0;
        text->field64 = 0.0f;
    }
}
#else
INCLUDE_ASM(const s32, "code_00309FE0", sbtlLeaf43);
#endif

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallBtlSearchUnitID18);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallSetOt);

void sbtlRefSbtl28(void) {
    if (wk_sbtl != NULL) {
        sbtlRefSbtl27(sbtlCallSetOt);
    }
}

INCLUDE_ASM(const s32, "code_00309FE0", sbtlRefSbtl29);

/* NON_MATCHING: 29 of 36 words match.  */
#ifdef NON_MATCHING
sbtlSummonText_t *sbtlRefSbtl30(void) {
    SBTL_WORK *wk = wk_sbtl;
    sbtlSummonText_t *text;
    sbtlSummonText_t *end;

    end = &wk->textPool[100];
    for (text = wk->textCursor; text < end; text++) {
        if (text->type == 0) {
            return text;
        }
    }
    for (text = wk->textPool; text < wk->textCursor; text++) {
        if (text->type == 0) {
            return text;
        }
    }
    return NULL;
}
#else
INCLUDE_ASM(const s32, "code_00309FE0", sbtlRefSbtl30);
#endif

s32 sbtlBtlFirstSummonTextSetSub(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    sbtlSummonText_t *text = sbtlRefSbtl30();

    if (text == NULL) {
        return 0;
    }
    sbtlRefSbtl29(text, arg0, arg1, arg2, arg3);
    text->field74 = arg4;
    return 1;
}

void Add_AW_MagWait(s32 type, s32 count) {
    s32 i;

    if (!sbtlBtlFirstSummonTextSetSub(type, 0, 0, 0, count)) {
        return;
    }
    if (count > 10) {
        count = 10;
    }
    for (i = 0; i < count; i++) {
        if (!sbtlBtlFirstSummonTextSetSub(type, 0, 0, 0, 0)) {
            return;
        }
    }
}

datSword_t *sbtlDat(void) {
    return &datData20[wk_sbtl->fieldE84];
}

s32 sbtlCallBtlDat(void) {
    return sbtlData140[sbtlDat()->kind].field0;
}

s32 sbtlCallBtlDat2(void) {
    return sbtlData140[sbtlDat()->kind].field4;
}

s32 sbtlCallBtlDat3(void) {
    return sbtlData140[sbtlDat()->kind].field8;
}

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallBtlDat4);

sbtlCmndMenuNew_t *GetProcessData(void) {
    if (dds3ExistProcess(sbtlData181) == 0) {
        return NULL;
    }
    return dds3GetProcessData(sbtlData181);
}

INCLUDE_ASM(const s32, "code_00309FE0", sbtlEnd);

INCLUDE_ASM(const s32, "code_00309FE0", ResetBtlParam);

INCLUDE_ASM(const s32, "code_00309FE0", SkillChkUseUnit);

s32 sbtlCallBtlGetSkilltbl15(SKLID skillid) {
    struct SBTLSKILLP_TBL *ptbl = sbtlGetSkilltbl(skillid);

    if (ptbl == NULL) {
        return 0;
    }
    if (ptbl->type & 0x1000) {
        return 0;
    }
    if (ptbl->area & 0x100) {
        return 0;
    }
    if (ptbl->flg & 0x20) {
        return 0;
    }
    if (skillid == 134 || skillid == 1124 || skillid == 1169) {
        return 0;
    }
    if (ptbl->flg & 0x200) {
        return 0;
    }
    return 1;
}

INCLUDE_RODATA(const s32, "code_00309FE0", sbtlCallSetOtTable);

INCLUDE_RODATA(const s32, "code_00309FE0", sbtlCallBtlDat4Table);

INCLUDE_RODATA(const s32, "code_00309FE0", DrawSkillListDataStrChangeDemon);

INCLUDE_RODATA(const s32, "code_00309FE0", DrawSkillListDataStrOneShot);

INCLUDE_RODATA(const s32, "code_00309FE0", DrawSkillListDataStrAutoRepeat);

INCLUDE_ASM(const s32, "code_00309FE0", FuncDecideTgt);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData39);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData40);

INCLUDE_ASM(const s32, "code_00309FE0", SetTgtMenu);

INCLUDE_ASM(const s32, "code_00309FE0", FuncDecideSkl);

INCLUDE_ASM(const s32, "code_00309FE0", DrawSkillList);

INCLUDE_ASM(const s32, "code_00309FE0", ChkUseSkl);

INCLUDE_ASM(const s32, "code_00309FE0", ChkUseSklEx);

INCLUDE_ASM(const s32, "code_00309FE0", GetEnebleUseSkillNum);

INCLUDE_ASM(const s32, "code_00309FE0", MakeSkillList);

INCLUDE_ASM(const s32, "code_00309FE0", SetSklMenu);

INCLUDE_ASM(const s32, "code_00309FE0", GetTabLimit);

INCLUDE_ASM(const s32, "code_00309FE0", Proc_sbtlCmndMenuNew);

void Shutdown_sbtlCmndMenuNew(dds3ProcessID_t *id) {
    sbtlCmndMenuNew_t *wk = dds3GetProcessData(id);

    if (wk->drawPID != NULL) {
        dds3KillProcess(wk->drawPID, 0);
        wk->drawPID = NULL;
    }
}

INCLUDE_ASM(const s32, "code_00309FE0", DrawProc);

void sbtlInit(void) {
    sdfMemHandle_t *hMem;
    sbtlCmndMenuNew_t *wk;

    hMem = sdfMemAllocDirect(sizeof(sbtlCmndMenuNew_t));
    wk = sdfMemLock(hMem);
    memset(wk, 0, sizeof(sbtlCmndMenuNew_t));
    wk->hMem = hMem;
    sbtlData181 = dds3StartProcess(sbtlStrCmndMenu, 0x3FE, 0, 0, Proc_sbtlCmndMenuNew, (dds3ProcessFunc_t)Shutdown_sbtlCmndMenuNew, wk);
    wk->drawPID = dds3StartProcess(sbtlStrCmndMenuDraw, 0x2B15, 0, 0, DrawProc, NULL, wk);
    wk->stat = 0;
}

void Delete_sbtlCmndMenuNew(void) {
    sbtlCmndMenuNew_t *wk;

    if (dds3ExistProcess(sbtlData181) != 0) {
        wk = dds3GetProcessData(sbtlData181);
        wk->stat = 4;
        itfMenuProcessEnd(wk->menuPID);
        itfMenuProcessEnd(wk->menuPID2);
    }
}

INCLUDE_ASM(const s32, "code_00309FE0", sbtlIsStart);

INCLUDE_ASM(const s32, "code_00309FE0", Start_Local);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlStart);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlStartBattle1st);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlLeaf44);

s32 sbtlGetStat(void) {
    sbtlCmndMenuNew_t *wk = GetProcessData();

    if (wk == NULL) {
        return 0;
    }
    switch (wk->stat) {
        case 1:
            return 1;
        case 4:
            return 3;
        default:
            return 2;
    }
}

INCLUDE_ASM(const s32, "code_00309FE0", sbtlChkDecide);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlFuncDecideTgtSub);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlProcSBtlCmndMenuNewSub);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlDrawRepeatIconExSub);

INCLUDE_ASM(const s32, "code_00309FE0", DrawRepeatIconEx);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlGetActionUnit);

INCLUDE_ASM(const s32, "code_00309FE0", GetTargetUnit_sbtlCmndMenuNew);

INCLUDE_RODATA(const s32, "code_00309FE0", sbtlStrCmndMenu);

INCLUDE_RODATA(const s32, "code_00309FE0", sbtlStrCmndMenuDraw);

