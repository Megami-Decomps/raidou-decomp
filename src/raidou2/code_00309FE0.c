#include "common.h"
#include "sdf_resource.h"
#include "sBtlCmndMenuNew.h"
#include "sdfMemory.h"
#include "sdk_libc.h"
#include "dds3KernelCore.h"
#include "sbtlWork.h"

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallBtlGetSkilltbl11);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallMemset12);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallMemset13);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlChangeWorldtoScreenUnity4);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlTargetNoCursor);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallBtlGetSkilltbl12);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlRefSbtl20);

INCLUDE_ASM(const s32, "code_00309FE0", SetLightChg);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlRefSbtl21);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlSetCmbBfrPos);

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

INCLUDE_ASM(const s32, "code_00309FE0", cmpCallGetProcessData2);

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

INCLUDE_ASM(const s32, "code_00309FE0", cmpCallGetNowSummonerRank);

INCLUDE_ASM(const s32, "code_00309FE0", cmpCallBtlSearchUnitID);

INCLUDE_ASM(const s32, "code_00309FE0", cmpCallGetProcessData6);

INCLUDE_ASM(const s32, "code_00309FE0", cmpCallGetProcessData7);

INCLUDE_ASM(const s32, "code_00309FE0", cmpCallGetProcessData8);

INCLUDE_ASM(const s32, "code_00309FE0", cmpCallBtlSearchUnitID2);

INCLUDE_ASM(const s32, "code_00309FE0", cmpTALKRECOVERY);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData3);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData4);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData5);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData6);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlLeaf38);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData7);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData8);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallBtlCheckLoopMotion);

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

INCLUDE_ASM(const s32, "code_00309FE0", sbtlRefDDS3GBWK4);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlRefDDS3GBWK5);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlGetDevilHuumaItem);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlRefDDS3GBWK6);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallBtlSearchUnitID15);

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

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData13);

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData14);

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData15);

INCLUDE_ASM(const s32, "code_00309FE0", nbCallGetProcessData16);

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

INCLUDE_ASM(const s32, "code_00309FE0", slCallGetProcessData7);

INCLUDE_ASM(const s32, "code_00309FE0", slCallGetProcessData8);

INCLUDE_ASM(const s32, "code_00309FE0", slCallGetProcessData9);

INCLUDE_ASM(const s32, "code_00309FE0", slCallGetProcessData10);

INCLUDE_ASM(const s32, "code_00309FE0", slCallBtlSearchUnitID);

INCLUDE_ASM(const s32, "code_00309FE0", slRefSbtl);

INCLUDE_ASM(const s32, "code_00309FE0", slCallBitCheck2);

INCLUDE_ASM(const s32, "code_00309FE0", trmCallBtlSearchUnitID);

INCLUDE_ASM(const s32, "code_00309FE0", trmCallGetProcessData);

INCLUDE_ASM(const s32, "code_00309FE0", trmBtlAnalyzeMeisiSub);

INCLUDE_ASM(const s32, "code_00309FE0", trmLeaf);

INCLUDE_ASM(const s32, "code_00309FE0", trmRefDDS3GBWK);

INCLUDE_ASM(const s32, "code_00309FE0", trmCallBtlSearchUnitID2);

INCLUDE_ASM(const s32, "code_00309FE0", trmSetProcessTerm);

INCLUDE_ASM(const s32, "code_00309FE0", trmCallBtlSearchUnitID3);

INCLUDE_ASM(const s32, "code_00309FE0", trmCallBtlSearchUnitID4);

INCLUDE_ASM(const s32, "code_00309FE0", trmCallBtlClearHitC);

INCLUDE_ASM(const s32, "code_00309FE0", trmCallBtlCheckReturnUnit);

INCLUDE_ASM(const s32, "code_00309FE0", ChangeTarget_BSHelper_sbtlAI);

INCLUDE_ASM(const s32, "code_00309FE0", trmCallBtlCreateUnitList);

INCLUDE_ASM(const s32, "code_00309FE0", SetBSHelper);

INCLUDE_ASM(const s32, "code_00309FE0", trmCallBtlSearchUnitID5);

INCLUDE_ASM(const s32, "code_00309FE0", trmCallBtlSetMode);

INCLUDE_ASM(const s32, "code_00309FE0", trmCallBtlSetMode2);

INCLUDE_ASM(const s32, "code_00309FE0", trmCallBtlSetMode3);

INCLUDE_ASM(const s32, "code_00309FE0", evtCallBtlSearchUnitID);

INCLUDE_ASM(const s32, "code_00309FE0", evtCallMemset3);

INCLUDE_ASM(const s32, "code_00309FE0", evtCallMemset4);

INCLUDE_ASM(const s32, "code_00309FE0", WajutsuPosSet);

INCLUDE_ASM(const s32, "code_00309FE0", evtCallMemset5);

INCLUDE_ASM(const s32, "code_00309FE0", evtCallGetProcessData);

INCLUDE_ASM(const s32, "code_00309FE0", evtCallGetProcessData2);

INCLUDE_ASM(const s32, "code_00309FE0", evtCallBtlSearchUnitID2);

INCLUDE_ASM(const s32, "code_00309FE0", evtCallGetProcessData3);

INCLUDE_ASM(const s32, "code_00309FE0", evtKAIWANoEnemy);

INCLUDE_ASM(const s32, "code_00309FE0", evtMoonShutdown);

INCLUDE_ASM(const s32, "code_00309FE0", evtCallGetProcessData4);

INCLUDE_ASM(const s32, "code_00309FE0", cmpPartyAnimFunc);

INCLUDE_ASM(const s32, "code_00309FE0", cmpCallGetProcessData9);

INCLUDE_ASM(const s32, "code_00309FE0", cmpCallGetProcessData10);

INCLUDE_ASM(const s32, "code_00309FE0", cmpPartyListMake_cmpMisc);

INCLUDE_ASM(const s32, "code_00309FE0", cmpSbtlkaiwaMain);

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

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallMemFreeDirect3);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallGetProcessData38);

INCLUDE_ASM(const s32, "code_00309FE0", btlLoadKaiwaData);

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

INCLUDE_ASM(const s32, "code_00309FE0", Start_sbtlTime);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCheckEnemyAllDead);

INCLUDE_ASM(const s32, "code_00309FE0", Result_sbtlTime);

INCLUDE_ASM(const s32, "code_00309FE0", End_sbtlTime);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlRefSbtl25);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlRefSbtl26);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlRefSbtl27);

void sbtlLeaf42(void) {
}

INCLUDE_ASM(const s32, "code_00309FE0", sbtlFirstSummonTextSet);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlLeaf43);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallBtlSearchUnitID18);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallSetOt);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlRefSbtl28);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlRefSbtl29);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlRefSbtl30);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlBtlFirstSummonTextSetSub);

INCLUDE_ASM(const s32, "code_00309FE0", Add_AW_MagWait);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlDat);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallBtlDat);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallBtlDat2);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallBtlDat3);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallBtlDat4);

INCLUDE_ASM(const s32, "code_00309FE0", GetProcessData);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlEnd);

INCLUDE_ASM(const s32, "code_00309FE0", ResetBtlParam);

INCLUDE_ASM(const s32, "code_00309FE0", SkillChkUseUnit);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlCallBtlGetSkilltbl15);

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

INCLUDE_ASM(const s32, "code_00309FE0", Shutdown_sbtlCmndMenuNew);

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
    wk->unk12 = 0;
}

INCLUDE_ASM(const s32, "code_00309FE0", Delete_sbtlCmndMenuNew);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlIsStart);

INCLUDE_ASM(const s32, "code_00309FE0", Start_Local);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlStart);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlStartBattle1st);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlLeaf44);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlGetStat);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlChkDecide);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlFuncDecideTgtSub);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlProcSBtlCmndMenuNewSub);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlDrawRepeatIconExSub);

INCLUDE_ASM(const s32, "code_00309FE0", DrawRepeatIconEx);

INCLUDE_ASM(const s32, "code_00309FE0", sbtlGetActionUnit);

INCLUDE_ASM(const s32, "code_00309FE0", GetTargetUnit_sbtlCmndMenuNew);

INCLUDE_RODATA(const s32, "code_00309FE0", sbtlStrCmndMenu);

INCLUDE_RODATA(const s32, "code_00309FE0", sbtlStrCmndMenuDraw);

