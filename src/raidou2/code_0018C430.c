#include "common.h"
#include "scrTraceCode.h"
#include "fldProcess.h"
#include "evtUnitManager.h"
#include "fld.h"
#include "field.h"
#include "mapData.h"

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

INCLUDE_ASM(const s32, "code_0018C430", nbErrNPCMODEL);

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

INCLUDE_ASM(const s32, "code_0018C430", prmLeaf);

INCLUDE_ASM(const s32, "code_0018C430", prmLeaf2);

INCLUDE_ASM(const s32, "code_0018C430", prmLeaf3);

INCLUDE_ASM(const s32, "code_0018C430", prmCallGetIntPara3);

INCLUDE_ASM(const s32, "code_0018C430", prmCallGetIntPara4);

INCLUDE_ASM(const s32, "code_0018C430", nbCallClearObjectEntryHandles);

INCLUDE_ASM(const s32, "code_0018C430", nbCallGetIntPara2);

INCLUDE_ASM(const s32, "code_0018C430", nbErrNPCMODEL2);

INCLUDE_ASM(const s32, "code_0018C430", nbLeaf2);

INCLUDE_ASM(const s32, "code_0018C430", nbLeaf3);

INCLUDE_ASM(const s32, "code_0018C430", nbLeaf4);

INCLUDE_ASM(const s32, "code_0018C430", nbCameraInit);

INCLUDE_ASM(const s32, "code_0018C430", nbFunc3);

INCLUDE_ASM(const s32, "code_0018C430", nbCallFreeEventPack);

INCLUDE_ASM(const s32, "code_0018C430", nbRefDDS3GBWK2);

INCLUDE_ASM(const s32, "code_0018C430", nbRefDDS3GBWK3);

INCLUDE_ASM(const s32, "code_0018C430", nbCallSetOt);

INCLUDE_ASM(const s32, "code_0018C430", nbCallGetRandIntA);

INCLUDE_ASM(const s32, "code_0018C430", nbCallGetRandIntA2);

INCLUDE_ASM(const s32, "code_0018C430", nbFunc4);

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

