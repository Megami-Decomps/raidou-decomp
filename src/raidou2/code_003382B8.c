#include "common.h"
#include "lmap.h"
#include "fileManager.h"
#include "dds3KernelDraw_0010A750.h"
#include "dds3KernelCore.h"
#include "ctn.h"
#include "code_002D7950.h"
#include "dds3WorldBasic.h"
#include "basic.h"
#include "dds3AreaBasic.h"
#include "dds3Basic.h"
#include "sdfMemory.h"
#include "sdk_libc.h"

INCLUDE_ASM(const s32, "code_003382B8", nbRefWorkMjn3);

INCLUDE_ASM(const s32, "code_003382B8", nbFunc30);

INCLUDE_ASM(const s32, "code_003382B8", nbRefWorkMjn4);

INCLUDE_ASM(const s32, "code_003382B8", nbRefWorkMjn5);

INCLUDE_ASM(const s32, "code_003382B8", nbCheckAgariSub);

INCLUDE_ASM(const s32, "code_003382B8", nbRefWorkMjn6);

INCLUDE_ASM(const s32, "code_003382B8", AnalyzeTehaiNormal);

INCLUDE_ASM(const s32, "code_003382B8", nbRefWorkMjn7);

INCLUDE_ASM(const s32, "code_003382B8", nbFunc31);

INCLUDE_ASM(const s32, "code_003382B8", nbFunc32);

INCLUDE_ASM(const s32, "code_003382B8", nbChooseAgariSub);

INCLUDE_ASM(const s32, "code_003382B8", nbRefWorkMjn8);

INCLUDE_ASM(const s32, "code_003382B8", nbRefWorkMjn9);

INCLUDE_ASM(const s32, "code_003382B8", CheckRon_Reach);

INCLUDE_ASM(const s32, "code_003382B8", CalcFu);

INCLUDE_ASM(const s32, "code_003382B8", CalcTensu);

INCLUDE_ASM(const s32, "code_003382B8", CalcAgari);

INCLUDE_ASM(const s32, "code_003382B8", ChooseAgari);

INCLUDE_ASM(const s32, "code_003382B8", nbRefWorkMjn10);

INCLUDE_ASM(const s32, "code_003382B8", nbRefWorkMjn11);

INCLUDE_ASM(const s32, "code_003382B8", nbCalcFuSub);

INCLUDE_ASM(const s32, "code_003382B8", GetHaiColor);

INCLUDE_ASM(const s32, "code_003382B8", GetMentsuType);

INCLUDE_ASM(const s32, "code_003382B8", CheckPinfu);

INCLUDE_ASM(const s32, "code_003382B8", CheckTanyao);

INCLUDE_ASM(const s32, "code_003382B8", nbRefWorkMjn12);

INCLUDE_ASM(const s32, "code_003382B8", nbRefWorkMjn13);

INCLUDE_ASM(const s32, "code_003382B8", nbRefWorkMjn14);

INCLUDE_ASM(const s32, "code_003382B8", nbRefWorkMjn15);

INCLUDE_ASM(const s32, "code_003382B8", nbRefWorkMjn16);

INCLUDE_ASM(const s32, "code_003382B8", nbRefWorkMjn17);

INCLUDE_ASM(const s32, "code_003382B8", nbRefWorkMjn18);

INCLUDE_ASM(const s32, "code_003382B8", nbRefWorkMjn19);

INCLUDE_ASM(const s32, "code_003382B8", CheckChankan);

INCLUDE_ASM(const s32, "code_003382B8", nbRefWorkMjn20);

INCLUDE_ASM(const s32, "code_003382B8", CheckChanta);

INCLUDE_ASM(const s32, "code_003382B8", nbRefWorkMjn21);

INCLUDE_ASM(const s32, "code_003382B8", CheckIttsu);

INCLUDE_ASM(const s32, "code_003382B8", CheckSanshokujun);

INCLUDE_ASM(const s32, "code_003382B8", CheckSanshokupon);

INCLUDE_ASM(const s32, "code_003382B8", CheckToitoi);

INCLUDE_ASM(const s32, "code_003382B8", CheckSananko);

INCLUDE_ASM(const s32, "code_003382B8", CheckSankantsu);

INCLUDE_ASM(const s32, "code_003382B8", sdfRefWorkMjn);

INCLUDE_ASM(const s32, "code_003382B8", CheckHonitsu);

INCLUDE_ASM(const s32, "code_003382B8", CheckJunchan);

INCLUDE_ASM(const s32, "code_003382B8", sdfRefWorkMjn2);

INCLUDE_ASM(const s32, "code_003382B8", sdfCalcYakuSub);

INCLUDE_ASM(const s32, "code_003382B8", CheckTsuiso);

INCLUDE_ASM(const s32, "code_003382B8", sdfRefWorkMjn3);

INCLUDE_ASM(const s32, "code_003382B8", sdfRefWorkMjn4);

INCLUDE_ASM(const s32, "code_003382B8", sdfRefWorkMjn5);

INCLUDE_ASM(const s32, "code_003382B8", sdfRefWorkMjn6);

INCLUDE_ASM(const s32, "code_003382B8", sdfRefWorkMjn7);

INCLUDE_ASM(const s32, "code_003382B8", sdfRefWorkMjn8);

INCLUDE_ASM(const s32, "code_003382B8", sdfRefWorkMjn9);

INCLUDE_ASM(const s32, "code_003382B8", sdfRefWorkMjn10);

INCLUDE_ASM(const s32, "code_003382B8", sdfCalcYakuSub2);

INCLUDE_ASM(const s32, "code_003382B8", sdfCalcYakuSub3);

INCLUDE_ASM(const s32, "code_003382B8", sdfCalcYakuSub4);

INCLUDE_ASM(const s32, "code_003382B8", sdfRefWorkMjn11);

INCLUDE_ASM(const s32, "code_003382B8", CalcYaku);

INCLUDE_ASM(const s32, "code_003382B8", CalcYakuChitoi);

INCLUDE_ASM(const s32, "code_003382B8", CalcYakuKokushi);

INCLUDE_ASM(const s32, "code_003382B8", SetBestAnalysis);

INCLUDE_ASM(const s32, "code_003382B8", mnuRebuildScrollLists);

INCLUDE_ASM(const s32, "code_003382B8", CalcHaiValueTanki);

INCLUDE_ASM(const s32, "code_003382B8", CalcHaiValue);

INCLUDE_ASM(const s32, "code_003382B8", CalcHaiValueChitoi);

INCLUDE_ASM(const s32, "code_003382B8", AIChooseSutehai);

INCLUDE_ASM(const s32, "code_003382B8", UpdateTensu);

INCLUDE_ASM(const s32, "code_003382B8", Shukyoku);

INCLUDE_ASM(const s32, "code_003382B8", CheckYakuAri);

INCLUDE_ASM(const s32, "code_003382B8", ApplyAgari);

INCLUDE_ASM(const s32, "code_003382B8", CheckFuriten);

INCLUDE_ASM(const s32, "code_003382B8", ApplyChonbo);

INCLUDE_ASM(const s32, "code_003382B8", sdfCallCounterDrawGlyphAtGridCell);

INCLUDE_ASM(const s32, "code_003382B8", sdfCounterDrawGlyphAtGridCell);

INCLUDE_ASM(const s32, "code_003382B8", sdfCallDrawFontEx2);

INCLUDE_ASM(const s32, "code_003382B8", sdfCallGetSmallFontFRQ2);

INCLUDE_ASM(const s32, "code_003382B8", sdfCallSprintf);

INCLUDE_ASM(const s32, "code_003382B8", sdfCallSprintf2);

INCLUDE_ASM(const s32, "code_003382B8", sdfCallSprintf3);

INCLUDE_ASM(const s32, "code_003382B8", InitLight);

INCLUDE_ASM(const s32, "code_003382B8", InitCamera);

INCLUDE_ASM(const s32, "code_003382B8", SetCamera);

INCLUDE_ASM(const s32, "code_003382B8", DrawOya3D);

INCLUDE_ASM(const s32, "code_003382B8", DrawHai3DExCalc);

INCLUDE_ASM(const s32, "code_003382B8", pipeLeaf);

INCLUDE_ASM(const s32, "code_003382B8", DrawHai3DIndex);

INCLUDE_ASM(const s32, "code_003382B8", pipeRefWorkMjn);

INCLUDE_ASM(const s32, "code_003382B8", pipeDrawSection);

INCLUDE_ASM(const s32, "code_003382B8", DrawTable);

INCLUDE_ASM(const s32, "code_003382B8", pipeCallDrawHai3DExCalc);

INCLUDE_ASM(const s32, "code_003382B8", pipeCallDrawHai3DExCalc2);

INCLUDE_ASM(const s32, "code_003382B8", pipeRefWorkMjn2);

INCLUDE_ASM(const s32, "code_003382B8", pipeCallDrawHai3DExCalc3);

INCLUDE_ASM(const s32, "code_003382B8", pipeCallLink);

INCLUDE_ASM(const s32, "code_003382B8", InitTenbouModel);

INCLUDE_ASM(const s32, "code_003382B8", DrawInfoUnit);

INCLUDE_ASM(const s32, "code_003382B8", InitOyaModel);

INCLUDE_ASM(const s32, "code_003382B8", nbCallLink);

INCLUDE_ASM(const s32, "code_003382B8", nbRefWorkMjn22);

INCLUDE_ASM(const s32, "code_003382B8", nbRefWorkMjn23);

INCLUDE_ASM(const s32, "code_003382B8", nbCallMemset2);

INCLUDE_ASM(const s32, "code_003382B8", nbRefWorkMjn24);

INCLUDE_ASM(const s32, "code_003382B8", nbRefWorkMjn25);

INCLUDE_ASM(const s32, "code_003382B8", nbPanelPressGaugeReset);

INCLUDE_ASM(const s32, "code_003382B8", Draw3D);

INCLUDE_ASM(const s32, "code_003382B8", nbLeaf10);

INCLUDE_ASM(const s32, "code_003382B8", StartCutin);

INCLUDE_ASM(const s32, "code_003382B8", MenuInit);

INCLUDE_ASM(const s32, "code_003382B8", nbMenuDrawFuncSub);

INCLUDE_ASM(const s32, "code_003382B8", nbMenuInit);

INCLUDE_ASM(const s32, "code_003382B8", nbCallSetOt3);

INCLUDE_ASM(const s32, "code_003382B8", nbMahjonMenuProcess);

INCLUDE_ASM(const s32, "code_003382B8", MenuLoop);

INCLUDE_ASM(const s32, "code_003382B8", MenuStartMahjon);

INCLUDE_ASM(const s32, "code_003382B8", MenuPadFunc);

INCLUDE_ASM(const s32, "code_003382B8", MenuDrawFunc);

INCLUDE_ASM(const s32, "code_003382B8", BridgeLoop);

INCLUDE_ASM(const s32, "code_003382B8", BridgeDestroy);

INCLUDE_ASM(const s32, "code_003382B8", ctnRefWorkCtn);

INCLUDE_ASM(const s32, "code_003382B8", ctnSimulateSetFlagsSub);

INCLUDE_ASM(const s32, "code_003382B8", ctnMoveFollowerSub);

INCLUDE_ASM(const s32, "code_003382B8", InitWork);

INCLUDE_ASM(const s32, "code_003382B8", ctnMapMapMAP);

INCLUDE_ASM(const s32, "code_003382B8", ctnLoadDataIDFloor);

INCLUDE_ASM(const s32, "code_003382B8", InitFromFile);

INCLUDE_ASM(const s32, "code_003382B8", LoadDataSub);

INCLUDE_ASM(const s32, "code_003382B8", ctnGetGrid);

INCLUDE_ASM(const s32, "code_003382B8", ctnMemFree);

INCLUDE_ASM(const s32, "code_003382B8", ctnFindAheadListSub);

INCLUDE_ASM(const s32, "code_003382B8", ctnSimulateSetFlagsSub2);

INCLUDE_ASM(const s32, "code_003382B8", KillCitizenFriend);

INCLUDE_ASM(const s32, "code_003382B8", KillCitizen);

INCLUDE_ASM(const s32, "code_003382B8", FindAheadList);

INCLUDE_ASM(const s32, "code_003382B8", FindAhead);

INCLUDE_ASM(const s32, "code_003382B8", FindHeadOn);

INCLUDE_ASM(const s32, "code_003382B8", SetHeadOn);

INCLUDE_ASM(const s32, "code_003382B8", SimulateSetFlags);

INCLUDE_ASM(const s32, "code_003382B8", ctnRefWorkCtn2);

INCLUDE_ASM(const s32, "code_003382B8", FreeGridBeside);

INCLUDE_ASM(const s32, "code_003382B8", ctnUpdateTargetVelocitySub);

INCLUDE_ASM(const s32, "code_003382B8", MoveFollower);

INCLUDE_ASM(const s32, "code_003382B8", ctnSetPrefSub);

INCLUDE_ASM(const s32, "code_003382B8", ctnUpdateTargetVelocitySub2);

INCLUDE_ASM(const s32, "code_003382B8", SetPref);

INCLUDE_ASM(const s32, "code_003382B8", ctnFunc);

INCLUDE_ASM(const s32, "code_003382B8", ctnFindAheadListSub2);

INCLUDE_ASM(const s32, "code_003382B8", ctnRefWorkCtn3);

INCLUDE_ASM(const s32, "code_003382B8", ctnRefWorkCtn4);

INCLUDE_ASM(const s32, "code_003382B8", ctnFunc2);

INCLUDE_ASM(const s32, "code_003382B8", UpdateTargetVelocity);

INCLUDE_ASM(const s32, "code_003382B8", ctnRefWorkCtn5);

INCLUDE_ASM(const s32, "code_003382B8", ctnRefWorkCtn6);

INCLUDE_ASM(const s32, "code_003382B8", ctnKillCitizenSub);

INCLUDE_ASM(const s32, "code_003382B8", EnterBlock);

INCLUDE_ASM(const s32, "code_003382B8", UpdateCitizenVelocity);

INCLUDE_ASM(const s32, "code_003382B8", UpdateCitizenPos);

INCLUDE_ASM(const s32, "code_003382B8", UpdateCitizenRotate);

INCLUDE_ASM(const s32, "code_003382B8", GetClassVelocity);

INCLUDE_ASM(const s32, "code_003382B8", AddCitizen);

INCLUDE_ASM(const s32, "code_003382B8", ShootCitizen);

INCLUDE_ASM(const s32, "code_003382B8", ctnRefWorkCtn7);

INCLUDE_ASM(const s32, "code_003382B8", SetWallInfo);

INCLUDE_ASM(const s32, "code_003382B8", ctnLeaf2);

INCLUDE_ASM(const s32, "code_003382B8", ctnRefWorkCtn8);

INCLUDE_ASM(const s32, "code_003382B8", ctnFunc3);

INCLUDE_ASM(const s32, "code_003382B8", ctnFunc4);

INCLUDE_ASM(const s32, "code_003382B8", IsValidNewMobSystem);

INCLUDE_ASM(const s32, "code_003382B8", CheckExit);

INCLUDE_ASM(const s32, "code_003382B8", ctnUpdate);

INCLUDE_ASM(const s32, "code_003382B8", ctnRefWorkCtn9);

INCLUDE_ASM(const s32, "code_003382B8", ctnMoveFollowerSub2);

INCLUDE_ASM(const s32, "code_003382B8", ctnCallFptodp);

INCLUDE_ASM(const s32, "code_003382B8", ctnLeaf3);

INCLUDE_ASM(const s32, "code_003382B8", stfCallUpdate);

INCLUDE_ASM(const s32, "code_003382B8", stfCallMemFree);

INCLUDE_ASM(const s32, "code_003382B8", stfLeaf);

INCLUDE_ASM(const s32, "code_003382B8", stfCtnTestProcess);

INCLUDE_ASM(const s32, "code_003382B8", stfInit);

INCLUDE_ASM(const s32, "code_003382B8", stfLeaf2);

INCLUDE_ASM(const s32, "code_003382B8", stfInterval);

INCLUDE_ASM(const s32, "code_003382B8", stfMapMapNeko);

INCLUDE_ASM(const s32, "code_003382B8", nekoCallMemAllocDirect);

INCLUDE_ASM(const s32, "code_003382B8", nekoCallDevLoadFileEx);

INCLUDE_ASM(const s32, "code_003382B8", nekoCallMemFreeDelayedDirect);

INCLUDE_ASM(const s32, "code_003382B8", nekoUpdate);

INCLUDE_ASM(const s32, "code_003382B8", nekoCallSin);

INCLUDE_ASM(const s32, "code_003382B8", ChooseNextTarget);

INCLUDE_ASM(const s32, "code_003382B8", nekoUpdateSub);

INCLUDE_ASM(const s32, "code_003382B8", UpdateRotation);

INCLUDE_ASM(const s32, "code_003382B8", nekoRefWorkNeko);

INCLUDE_ASM(const s32, "code_003382B8", mapRefWorkNeko);

INCLUDE_ASM(const s32, "code_003382B8", mapRefWorkNeko2);

INCLUDE_ASM(const s32, "code_003382B8", mapRefWorkNeko3);

INCLUDE_ASM(const s32, "code_003382B8", UpdateTargetVelocity_neko);

INCLUDE_ASM(const s32, "code_003382B8", mapUpdateSub);

INCLUDE_ASM(const s32, "code_003382B8", getListProcessSub);

INCLUDE_ASM(const s32, "code_003382B8", mapPrepareSelectedMenuEntrySub);

INCLUDE_ASM(const s32, "code_003382B8", mapMenuCreateMenuPID);

INCLUDE_ASM(const s32, "code_003382B8", evtPrepareSelectedMenuEntry);

INCLUDE_ASM(const s32, "code_003382B8", evtCallGetProcessData5);

INCLUDE_ASM(const s32, "code_003382B8", evtCallBitCheck);

INCLUDE_ASM(const s32, "code_003382B8", sbtlBtlFuncKatanaSBtlMenuSub);

INCLUDE_ASM(const s32, "code_003382B8", sbtlFuncKatana_sbtlMenu);

INCLUDE_ASM(const s32, "code_003382B8", sbtlCallMenuProcessStart);

INCLUDE_ASM(const s32, "code_003382B8", sbtlListProcessSub);

INCLUDE_ASM(const s32, "code_003382B8", ListCallProcEnd);

INCLUDE_ASM(const s32, "code_003382B8", ListCheckProcEnd);

INCLUDE_ASM(const s32, "code_003382B8", ListProcess);

INCLUDE_ASM(const s32, "code_003382B8", ListDrawProcess);

INCLUDE_ASM(const s32, "code_003382B8", mapCallGetProcessData3);

INCLUDE_ASM(const s32, "code_003382B8", ListLinkSprite);

INCLUDE_ASM(const s32, "code_003382B8", mapCallGetProcessData4);

INCLUDE_ASM(const s32, "code_003382B8", ListStart);

INCLUDE_ASM(const s32, "code_003382B8", mapCallGetProcessData5);

INCLUDE_ASM(const s32, "code_003382B8", ListSetFlagFocus);

INCLUDE_ASM(const s32, "code_003382B8", objCallGetProcessData);

INCLUDE_ASM(const s32, "code_003382B8", objCallGetProcessData2);

INCLUDE_ASM(const s32, "code_003382B8", objListProcessSub);

INCLUDE_ASM(const s32, "code_003382B8", getFunc);

INCLUDE_ASM(const s32, "code_003382B8", getListProcessStartSub);

INCLUDE_ASM(const s32, "code_003382B8", ListProcessStart);

INCLUDE_ASM(const s32, "code_003382B8", objDel);

INCLUDE_ASM(const s32, "code_003382B8", objLmapListMain);

INCLUDE_ASM(const s32, "code_003382B8", ListProcessEnd);

INCLUDE_ASM(const s32, "code_003382B8", getCallGetProcessStat);

INCLUDE_ASM(const s32, "code_003382B8", objBtlFuncKatanaSBtlMenuSub);

INCLUDE_ASM(const s32, "code_003382B8", objUpdateMessageSub);

INCLUDE_ASM(const s32, "code_003382B8", objFunc);

INCLUDE_ASM(const s32, "code_003382B8", SoundSmgLoad);

INCLUDE_ASM(const s32, "code_003382B8", SoundSmgLoadChk);

INCLUDE_ASM(const s32, "code_003382B8", objEffectOutSub);

INCLUDE_ASM(const s32, "code_003382B8", objEffectOutSub2);

INCLUDE_ASM(const s32, "code_003382B8", objEffectOutSub3);

INCLUDE_ASM(const s32, "code_003382B8", EffectOut);

s32 Start_lmap(s8 aFieldNo) {
    if (DDS3_GBWK2 == NULL) {
        return 0;
    }
    DDS3_GBWK2->flags |= 0x10;
    if (aFieldNo != 0) {
        DDS3_GBWK2->flags |= 0x20;
    } else {
        DDS3_GBWK2->flags &= ~0x20;
    }
    return 1;
}

s32 getLeaf3(s8 flag) {
    if (DDS3_GBWK2 == NULL) {
        return 0;
    }
    if (flag != 0) {
        DDS3_GBWK2->flags |= 0x80;
    } else {
        DDS3_GBWK2->flags &= ~0x80;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_003382B8", objLmapLMPNPC);

INCLUDE_ASM(const s32, "code_003382B8", objCallBitCheck);

INCLUDE_ASM(const s32, "code_003382B8", objLmpMainProc);

INCLUDE_ASM(const s32, "code_003382B8", objCallBitCheck2);

INCLUDE_ASM(const s32, "code_003382B8", ndFunc);

INCLUDE_ASM(const s32, "code_003382B8", ndLeaf);

INCLUDE_ASM(const s32, "code_003382B8", ndLeaf2);

INCLUDE_ASM(const s32, "code_003382B8", LoadFile);

INCLUDE_ASM(const s32, "code_003382B8", ndLmapLmapTmx);

INCLUDE_ASM(const s32, "code_003382B8", ndLmapDaimapSpr);

INCLUDE_ASM(const s32, "code_003382B8", ResetLightChg);

INCLUDE_ASM(const s32, "code_003382B8", Release_lmap);

INCLUDE_ASM(const s32, "code_003382B8", Shutdown_lmap);

INCLUDE_ASM(const s32, "code_003382B8", getCallTexFreeDelayed);

INCLUDE_ASM(const s32, "code_003382B8", ndCallSetOt);

INCLUDE_ASM(const s32, "code_003382B8", M_InitScene_lmap);

INCLUDE_ASM(const s32, "code_003382B8", getCallGetControlScene);

INCLUDE_ASM(const s32, "code_003382B8", M_InitCamera);

INCLUDE_ASM(const s32, "code_003382B8", ndCallMemset);

INCLUDE_ASM(const s32, "code_003382B8", ndCallMemset2);

INCLUDE_ASM(const s32, "code_003382B8", ndCallMemset3);

INCLUDE_ASM(const s32, "code_003382B8", ndCallMemset4);

INCLUDE_ASM(const s32, "code_003382B8", ndWorkActive);

INCLUDE_ASM(const s32, "code_003382B8", ndLeaf3);

INCLUDE_ASM(const s32, "code_003382B8", ndCallCameraObjectBasicSetTgtCam);

INCLUDE_ASM(const s32, "code_003382B8", M_MoveMap);

INCLUDE_ASM(const s32, "code_003382B8", sbtlCallFadeIn2);

INCLUDE_ASM(const s32, "code_003382B8", sbtlSpiritWazaAdd);

INCLUDE_ASM(const s32, "code_003382B8", sbtlCtrlActionWindowLmapSub);

INCLUDE_ASM(const s32, "code_003382B8", sbtlCallBtlSpiritWazaAdd);

INCLUDE_ASM(const s32, "code_003382B8", sbtlCallChrGetExLineType);

INCLUDE_ASM(const s32, "code_003382B8", CtrlActionWindow);

INCLUDE_ASM(const s32, "code_003382B8", sbtlLMPSYSEXIT);

INCLUDE_ASM(const s32, "code_003382B8", sbtlCallMemset16);

INCLUDE_ASM(const s32, "code_003382B8", datCallGetActiveScene);

INCLUDE_ASM(const s32, "code_003382B8", CtrlActionWindow_lmap);

INCLUDE_ASM(const s32, "code_003382B8", SetNpcCB);

INCLUDE_ASM(const s32, "code_003382B8", datCallBitCheck2);

INCLUDE_ASM(const s32, "code_003382B8", M_SetLockPointNpcCB);

INCLUDE_ASM(const s32, "code_003382B8", M_UpdateKaya);

INCLUDE_ASM(const s32, "code_003382B8", datAllClearCommMemory);

INCLUDE_ASM(const s32, "code_003382B8", datCallSetOt);

INCLUDE_ASM(const s32, "code_003382B8", itfFloatTo32s);

INCLUDE_ASM(const s32, "code_003382B8", itfCallPreCalcStrWidth);

INCLUDE_ASM(const s32, "code_003382B8", itfCallPreCalcStrWidth2);

INCLUDE_ASM(const s32, "code_003382B8", itfCallMemset);

INCLUDE_ASM(const s32, "code_003382B8", itfLeaf3);

INCLUDE_ASM(const s32, "code_003382B8", itfLeaf4);

INCLUDE_ASM(const s32, "code_003382B8", itfLeaf5);

INCLUDE_ASM(const s32, "code_003382B8", dmLeaf);

INCLUDE_ASM(const s32, "code_003382B8", dmLeaf2);

INCLUDE_ASM(const s32, "code_003382B8", NpcDelete);

INCLUDE_ASM(const s32, "code_003382B8", dmListDrawProcessSub);

INCLUDE_ASM(const s32, "code_003382B8", dmLeaf3);

INCLUDE_ASM(const s32, "code_003382B8", dmLMAP_POINT_FLAG);

INCLUDE_ASM(const s32, "code_003382B8", dmLeaf4);

INCLUDE_ASM(const s32, "code_003382B8", dmChrUpdateModelSub);

INCLUDE_ASM(const s32, "code_003382B8", dmIfHajikiCheckSub);

INCLUDE_ASM(const s32, "code_003382B8", mapFunc7);

INCLUDE_ASM(const s32, "code_003382B8", mapFunc8);

INCLUDE_ASM(const s32, "code_003382B8", mapIfHajikiCheck);

INCLUDE_ASM(const s32, "code_003382B8", mapLeaf8);

INCLUDE_ASM(const s32, "code_003382B8", MapDat_GetRouteDat);

INCLUDE_ASM(const s32, "code_003382B8", mapCtrlActionWindowSub);

INCLUDE_ASM(const s32, "code_003382B8", MapDat_GetPointID);

INCLUDE_ASM(const s32, "code_003382B8", dmCallLMAPPOINTNO);

INCLUDE_ASM(const s32, "code_003382B8", MapDat_GetPointXY);

INCLUDE_ASM(const s32, "code_003382B8", mapLeaf9);

INCLUDE_ASM(const s32, "code_003382B8", dmCallBitCheck);

INCLUDE_ASM(const s32, "code_003382B8", Util_CalcLength);

INCLUDE_ASM(const s32, "code_003382B8", dmChrUpdatePadTrainSub);

INCLUDE_ASM(const s32, "code_003382B8", campLeaf);

INCLUDE_ASM(const s32, "code_003382B8", campLeaf2);

INCLUDE_ASM(const s32, "code_003382B8", campLeaf3);

INCLUDE_ASM(const s32, "code_003382B8", campCallBitCheck);

INCLUDE_ASM(const s32, "code_003382B8", campCallDeleteUnitObjectBasic);

INCLUDE_ASM(const s32, "code_003382B8", campCallMemset);

INCLUDE_ASM(const s32, "code_003382B8", campDhIconIsReady);

INCLUDE_ASM(const s32, "code_003382B8", campChrUpdatePadTrainSub);

INCLUDE_ASM(const s32, "code_003382B8", campChrUpdatePadTrainSub2);

INCLUDE_ASM(const s32, "code_003382B8", MdlSetOfsLength);

INCLUDE_ASM(const s32, "code_003382B8", campLeaf4);

INCLUDE_ASM(const s32, "code_003382B8", MdlSetTrain);

INCLUDE_ASM(const s32, "code_003382B8", MdlUpdate);

INCLUDE_ASM(const s32, "code_003382B8", campCallMdlSetOfsLength);

INCLUDE_ASM(const s32, "code_003382B8", campCallMdlSetOfsLength2);

INCLUDE_ASM(const s32, "code_003382B8", MdlUpdate_lmap);

INCLUDE_ASM(const s32, "code_003382B8", campCallDhIconIsReady);

INCLUDE_ASM(const s32, "code_003382B8", campChrUpdatePadTrainSub3);

INCLUDE_ASM(const s32, "code_003382B8", campChrUpdateModelSub);

INCLUDE_ASM(const s32, "code_003382B8", ChrSetModel);

INCLUDE_ASM(const s32, "code_003382B8", Chr_UpdateModel);

INCLUDE_ASM(const s32, "code_003382B8", checkCallMemset);

INCLUDE_ASM(const s32, "code_003382B8", checkCallChrSetModel);

INCLUDE_ASM(const s32, "code_003382B8", Chr_SetPlayer);

INCLUDE_ASM(const s32, "code_003382B8", Chr_SetPlayerRoute);

INCLUDE_ASM(const s32, "code_003382B8", checkCallChrSetModel2);

INCLUDE_ASM(const s32, "code_003382B8", checkCallLMAPPOINTFLAG);

INCLUDE_ASM(const s32, "code_003382B8", checkCallLMAPPOINTFLAG2);

INCLUDE_ASM(const s32, "code_003382B8", checkCallLMAPPOINTFLAG3);

INCLUDE_ASM(const s32, "code_003382B8", checkCallMemset2);

INCLUDE_ASM(const s32, "code_003382B8", ChrCalcLineVector);

INCLUDE_ASM(const s32, "code_003382B8", ChrCalcDegree);

INCLUDE_ASM(const s32, "code_003382B8", Chr_CalcPosition);

INCLUDE_ASM(const s32, "code_003382B8", checkLeaf);

INCLUDE_ASM(const s32, "code_003382B8", check_point_line);

INCLUDE_ASM(const s32, "code_003382B8", check_cross_line2);

INCLUDE_ASM(const s32, "code_003382B8", checkCallMemset3);

INCLUDE_ASM(const s32, "code_003382B8", checkCallChrCalcPosition);

INCLUDE_ASM(const s32, "code_003382B8", Chr_UpdatePadRandom);

INCLUDE_ASM(const s32, "code_003382B8", ChrRouteCalcPadParam);

INCLUDE_ASM(const s32, "code_003382B8", _Chr_UpdatePadRoute);

INCLUDE_ASM(const s32, "code_003382B8", Chr_UpdatePadRoute);

INCLUDE_ASM(const s32, "code_003382B8", Chr_UpdatePadTrain);

INCLUDE_ASM(const s32, "code_003382B8", _Chr_GetExLineDat);

INCLUDE_ASM(const s32, "code_003382B8", Chr_GetExLineDat);
