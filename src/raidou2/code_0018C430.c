#include "common.h"
#include "scrTraceCode.h"
#include "fldProcess.h"
#include "evtUnitManager.h"
#include "fld.h"
#include "field.h"
#include "mapData.h"
#include "EventBit.h"
#include "fldDraw.h"
#include "code_00163C50.h"
#include "dds3KernelDraw.h"
#include "code_r1_0010DF68.h"

INCLUDE_ASM(const s32, "code_0018C430", fldGetMAP_CUT_OVERSET_Cam);

INCLUDE_ASM(const s32, "code_0018C430", fldCallGetIntPara);

INCLUDE_ASM(const s32, "code_0018C430", nbMAPSETTAKARA);

INCLUDE_ASM(const s32, "code_0018C430", nbCallSetIntReturnValue);

INCLUDE_ASM(const s32, "code_0018C430", fldCallGetIntPara2);

INCLUDE_ASM(const s32, "code_0018C430", fldCallGetIntPara3);

INCLUDE_ASM(const s32, "code_0018C430", fldCallGetIntPara4);

INCLUDE_ASM(const s32, "code_0018C430", fldCallSetIntReturnValue2);

INCLUDE_ASM(const s32, "code_0018C430", fldCallSetIntReturnValue3);

INCLUDE_ASM(const s32, "code_0018C430", fldCallWarpFloorRefresh);

INCLUDE_ASM(const s32, "code_0018C430", nbCallGetIntPara3);

INCLUDE_ASM(const s32, "code_0018C430", nbMapBossFirst);

INCLUDE_ASM(const s32, "code_0018C430", FLD_NEKO);

s32 nbCallGetIntPara4(void)
{
    fldBgZoomTest(scrGetIntPara(0));
    return 1;
}

s32 nbCallGetIntPara5(void)
{
    s32 frames;
    s32 alpha;

    frames = scrGetIntPara(0);
    alpha = scrGetIntPara(1);
    fldBgZoomTest2(frames, alpha, scrGetIntPara(2));
    return 1;
}

s32 nbErrNPCMODEL(void)
{
    s32 id = scrGetIntPara(0);
    dds3Basic_t *basic;

    if (id == 0) {
        scrSetIntReturnValue(nbCallGetIntPara2Data[0]);
    } else {
        basic = mapObjGetBasic(id);
        if (basic == NULL) {
            datDEBUGCONSOLE("Err : NPC MODEL ID:%04x N", id);
            basic = playerBasic;
        }
        scrSetIntReturnValue(basic->serialID);
    }
    return 1;
}

s32 nbCallNpcTblRefreshWait(void) {
    mdlCallMemFreeDirect();
    if (mapNpcTblRefresh_Wait() == -1) {
        mapNpcTblRefresh();
    }
    if (mapNpcTblRefresh_Wait() == 1) {
        return 0;
    }
    mapMapObjectRefresh();
    mapObjOnOff();
    return 1;
}

INCLUDE_ASM(const s32, "code_0018C430", nbCallGetIntPara6);

s32 nbCallGetIntPara7(void) {
    scrSetIntReturnValue(mapObjPointGetSerializeID(scrGetIntPara(0)));
    return 1;
}

INCLUDE_ASM(const s32, "code_0018C430", nbCallSetIntReturnValue2);

INCLUDE_ASM(const s32, "code_0018C430", nbCallGetIntPara);

s32 nbCommand_AI_ACT_SKILL(void) {
    evtGetUnitHandle(scrGetIntPara(0))->unk206 = 1;
    return 1;
}

INCLUDE_ASM(const s32, "code_0018C430", nbCallGetIntPara9);

INCLUDE_ASM(const s32, "code_0018C430", evtCommand_TEX_SWAP);

INCLUDE_ASM(const s32, "code_0018C430", evtBCheckPanelMode);

s32 evtCallGetIntPara3(void) {
    s32 se = scrGetIntPara(0);
    s32 on = scrGetIntPara(1);

    if (se == 0) {
        fldGlobalBase.word358 = 1;
        if (on == 0) {
            mapBgTexLoad(0);
        } else {
            mapBgTexLoad(1);
        }
    } else {
        if (on == 0) {
            fldCallSoundKankyoSeSet(0);
        } else {
            fldCallSoundKankyoSeSet(1);
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_0018C430", prm_MAKA_PLUS);

INCLUDE_ASM(const s32, "code_0018C430", prmFunc);

extern s32 prmLeafData[];

s32 prmLeaf(void)
{
    prmLeafData[0] = 1;
    return 1;
}

INCLUDE_ASM(const s32, "code_0018C430", prmLeaf2);

INCLUDE_ASM(const s32, "code_0018C430", prmLeaf3);

INCLUDE_ASM(const s32, "code_0018C430", prmCallGetIntPara3);

INCLUDE_ASM(const s32, "code_0018C430", prmCallGetIntPara4);

INCLUDE_ASM(const s32, "code_0018C430", nbCallClearObjectEntryHandles);

INCLUDE_ASM(const s32, "code_0018C430", nbCallGetIntPara2);

INCLUDE_ASM(const s32, "code_0018C430", nbErrNPCMODEL2);

s32 nbLeaf2(s32 a, s32 b, s32 c)
{
    s32 i;
    nbData10_t *p = nbData10;

    for (i = 0; i < 64; i++, p++) {
        if (a == p->key[0] && b == p->key[1] && (p->key[2] == 0 || c == p->key[2])) {
            return p->value;
        }
    }
    return 0;
}

s32 nbLeaf3(void)
{
    return nbData14[0];
}

void nbLeaf4(s32 flag)
{
    nbData14[0] = flag;
    if (flag) {
        evtBitOn(2102);
    } else {
        evtBitOff(2102);
    }
}

void nbCameraInit(void)
{
    mapCameraInitSub(&nbCallSetOtWork.eff[0]);
    mapCameraInitSub(&nbCallSetOtWork.eff[1]);
    mapCameraInitSub(&nbCallSetOtWork.eff[2]);
    nbCallSetOtWork.effOn[0] = 0;
    nbCallSetOtWork.effOn[1] = 0;
    nbCallSetOtWork.effOn[2] = 0;
}

INCLUDE_ASM(const s32, "code_0018C430", nbFunc3);

void nbCallFreeEventPack(void)
{
    nbFreeEventPack(&nbCallSetOtWork.eff[0]);
    nbFreeEventPack(&nbCallSetOtWork.eff[1]);
    nbFreeEventPack(&nbCallSetOtWork.eff[2]);
    nbCallSetOtWork.effOn[0] = 0;
    nbCallSetOtWork.effOn[1] = 0;
    nbCallSetOtWork.effOn[2] = 0;
}

void nbRefDDS3GBWK2(void)
{
    if (!DDS3_GBWK->unk48F8) {
        if (nbCallSetOtWork.effOn[1]) {
            fldGetSelectedUnitStat(&nbCallSetOtWork.eff[1], fldGlobalBase.focusX, fldGlobalBase.focusY, fldGlobalBase.focusZ);
        }
    } else if (nbCallSetOtWork.effOn[2]) {
        fldGetSelectedUnitStat(&nbCallSetOtWork.eff[2], fldGlobalBase.focusX, fldGlobalBase.focusY, fldGlobalBase.focusZ);
    }
}

void nbRefDDS3GBWK3(void)
{
    if (nbCallSetOtWork.effOn[0]) {
        mapCallEffectGetState(&nbCallSetOtWork.eff[0]);
    }
    if (!DDS3_GBWK->unk48F8) {
        if (nbCallSetOtWork.effOn[1]) {
            mapCallEffectGetState(&nbCallSetOtWork.eff[1]);
        }
    } else if (nbCallSetOtWork.effOn[2]) {
        mapCallEffectGetState(&nbCallSetOtWork.eff[2]);
    }
}

void nbCallSetOt(void)
{
    eglSetOt(0x2C);
    if (nbCallSetOtWork.effOn[0]) {
        fldCallEffectDrawDds3Effect(&nbCallSetOtWork.eff[0]);
    }
    if (!DDS3_GBWK->unk48F8) {
        if (nbCallSetOtWork.effOn[1]) {
            fldCallEffectDrawDds3Effect(&nbCallSetOtWork.eff[1]);
        }
    } else if (nbCallSetOtWork.effOn[2]) {
        fldCallEffectDrawDds3Effect(&nbCallSetOtWork.eff[2]);
    }
    if (nbCallSetOtWork.effOn[0]) {
        fldDrawBokasi();
    }
}

INCLUDE_ASM(const s32, "code_0018C430", nbCallGetRandIntA);

INCLUDE_ASM(const s32, "code_0018C430", nbCallGetRandIntA2);

s32 nbFunc4(s32 mode)
{
    switch (mode) {
    case 0:
        return nbLeaf3() == 2;
    case 1:
        return nbLeaf3() == 1;
    case 2:
        return nbLeaf3() == 3;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0018C430", nbLeaf5);

INCLUDE_ASM(const s32, "code_0018C430", nbMapSpriteAnime);

INCLUDE_ASM(const s32, "code_0018C430", nbLeaf6);

INCLUDE_RODATA(const s32, "code_0018C430", nbFunc3Table);

INCLUDE_RODATA(const s32, "code_0018C430", nbStrJgkYagiEpl);

INCLUDE_RODATA(const s32, "code_0018C430", nbStrJigkYtexEpl);

INCLUDE_RODATA(const s32, "code_0018C430", nbStrJigkItexEpl);

INCLUDE_RODATA(const s32, "code_0018C430", nbStrZigokuIcEpl);

INCLUDE_RODATA(const s32, "code_0018C430", nbStrZigokuHiEpl);

INCLUDE_RODATA(const s32, "code_0018C430", nbStrJigkFtexEpl);

INCLUDE_ASM(const s32, "code_0018C430", nbJimusyoIraiTmx);

INCLUDE_ASM(const s32, "code_0018C430", nbFunc5);

INCLUDE_ASM(const s32, "code_0018C430", nbFunc6);

INCLUDE_ASM(const s32, "code_0018C430", nbLeaf7);

INCLUDE_ASM(const s32, "code_0018C430", nbCallSetOt2);

INCLUDE_ASM(const s32, "code_0018C430", nbCallTexFreeDelayed);

INCLUDE_ASM(const s32, "code_0018C430", nbMapSpriteAnime2);

INCLUDE_ASM(const s32, "code_0018C430", EndPipe);

INCLUDE_RODATA(const s32, "code_0018C430", nbFunc6Table);

INCLUDE_RODATA(const s32, "code_0018C430", nbFunc6Table2);

