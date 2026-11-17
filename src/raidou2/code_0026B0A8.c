#include "common.h"
#include "sBtlUnit.h"
#include "sBtlSM.h"
#include "sdfChip.h"

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlReLoadMap);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlModelFieldLb);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlModelFieldLb2);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallMemGetPtr);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallMemGetPtr2);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLoadMapFromMemorySub);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLoadMapFromMemorySub2);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallSync);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlModelFieldLb3);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallMemGetPtr3);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallPackDecode);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallSync2);

INCLUDE_ASM(const s32, "code_0026B0A8", LoadMap_FromMemory);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallMemset2);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlSetCutScroll);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlDefaultCutScroll);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLeaf);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallBtlSearchUnitID3);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLeaf55);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallBtlSearchUnitID19);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLeaf56);

INCLUDE_ASM(const s32, "code_0026B0A8", cmpLeaf2);

INCLUDE_ASM(const s32, "code_0026B0A8", cmpProcSequenceSub);

INCLUDE_ASM(const s32, "code_0026B0A8", cmpSetColor);

INCLUDE_ASM(const s32, "code_0026B0A8", cmpRefSbtl);

INCLUDE_ASM(const s32, "code_0026B0A8", cmpRefSbtl2);

INCLUDE_ASM(const s32, "code_0026B0A8", cmpLeaf3);

void sbtlSummonDvTaskSub(SBTLSOUND_FOOTWORK *fwk)
{
    fwk->prevSMGID = 0;
    fwk->prevFrame = 100000.0f;
    fwk->played = 0;
}

INCLUDE_ASM(const s32, "code_0026B0A8", cmpCallMemFreeDirect);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlDevilFootSEProcSub);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlBtlLoadSkillSESub);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlBtlForceDeleteEffectWorkIDSub);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLeaf57);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlPlaySE);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlSEVolCalc);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlSEVolAngleCalc_sbtlSound);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlSEVolAngleCalc);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlPlay3DSE_BaseVol);

INCLUDE_ASM(const s32, "code_0026B0A8", DevilFootSEProc);

INCLUDE_ASM(const s32, "code_0026B0A8", NakamaMotion3DSE);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlPlayViceSE);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlRefSbtl6);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlPlayMotionSENowSub);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlGetBossSE);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlPlayDamageSE);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallGetRandIntA4);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallGetRandIntA5);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlPlaySpecialSE);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLeaf58);

INCLUDE_ASM(const s32, "code_0026B0A8", Proc_sbtlSound);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlGetDevilSESub);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlGetViceSE);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLoadDevilSE);

INCLUDE_ASM(const s32, "code_0026B0A8", GetR1CombSklSmg);

INCLUDE_ASM(const s32, "code_0026B0A8", GetSkillSE);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLoadSkillSE);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlBstse);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlBtlSetRirituNakamaHDSub);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallLoad);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallEffectShutdown);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlSoundatBossAt);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlBtlModelDataLoadSBtlUnitSub);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlPlayDeadVoice);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlRefSbtl7);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallEffectShutdown3);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallGetSkillSE);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlFunc10);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlENMY);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLeaf2);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLeaf3);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallBtlSEVolAngleCalc);

INCLUDE_ASM(const s32, "code_0026B0A8", Init_sbtlUnit);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlSetEndBadStatus);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlSetEndData);

INCLUDE_ASM(const s32, "code_0026B0A8", Exit_sbtlUnit);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallBtlGetPlayerData2);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlRefSbtl32);

INCLUDE_ASM(const s32, "code_0026B0A8", Draw_sbtlUnit);

struct SBTLUNIT_LISTWORK *sbtlCreateUnitList(s32 number)
{
    struct SBTLUNIT_LISTWORK *lwk;
    sdfMemHandle_t *handle;
    SBTLUNIT_LISTWK *list;
    s32 i;

    lwk = sdfChipAllocZeroDirect(sizeof(struct SBTLUNIT_LISTWORK));
    lwk->number = number;
    lwk->count = 0;
    handle = sdfMemAllocDirect((number + 1) * sizeof(SBTLUNIT_LISTWK));
    list = sdfMemLock(handle);
    lwk->pUnitL = list;
    for (i = 0; i <= number; i++) {
        list[i].ID = 0;
        list[i].pUnit = NULL;
    }
    lwk->handle = handle;
    return lwk;
}

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlSearchUnit_ID);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlSearchUnit_GroupID);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlSearchUnit_Nakama);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlGetNumUnit_GroupID);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlGetNumUnit_GroupID2);

s32 sbtlClearUnitList(struct SBTLUNIT_LISTWORK *wk)
{
    s32 i;

    if (wk == NULL) {
        return 0;
    }
    for (i = 0; i < wk->number; i++) {
        wk->pUnitL[i].pUnit = NULL;
    }
    wk->count = 0;
    return 1;
}

s32 sbtlDeleteUnitList(struct SBTLUNIT_LISTWORK *lwk)
{
    sdfMemFreeDirect(lwk->handle);
    sdfChipFreeDirect(lwk);
    return 1;
}

s32 sbtlEntryUnitList(struct SBTLUNIT_LISTWORK *lwk, struct SBTLUNIT_WORK *wk)
{
    s32 i;

    for (i = 0; i < lwk->number; i++) {
        if (lwk->pUnitL[i].pUnit == NULL) {
            break;
        }
    }
    lwk->pUnitL[i].ID = i;
    lwk->pUnitL[i].pUnit = wk;
    lwk->count++;
    return lwk->pUnitL[i].ID;
}

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLeaf4);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallMemAllocDirect);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlDeleteUnit_UnWork);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCheckReturn_Unit);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlSearchReturn_Unit);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlDeleteUnit);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlRefSbtl33);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallSin);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlUseItemProcSBtlPlayerSub);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlUseItemProcSBtlPlayerSub2);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLeaf5);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlResetColorSub);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlBtlFadeOutBadStatusEffectSub);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLeaf59);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLeaf60);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlChangeModelColor);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlResetMutekiColor);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlBtlFadeOutBadStatusEffectSub2);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLeaf61);

s32 sbtlCallBtlCheckMapHitData(f32 posx, f32 posz)
{
    f32 px;
    f32 pz;
    s32 ret;

    ret = sbtlCheckMapHit_Data(posx, posz, &px, &pz);
    return ret != -1 ? ret == 0 : 0;
}

INCLUDE_ASM(const s32, "code_0026B0A8", CheckMapHit);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCheckMapHit_Data);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCheckMapHit_Cross);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlGetRotate);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlGetRotate_Kaihi);

INCLUDE_ASM(const s32, "code_0026B0A8", UnitMV);

INCLUDE_ASM(const s32, "code_0026B0A8", UnitMV_sbtlUnit);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCheckMotionNo);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCheckMotionEnd);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCheckGuardMotion);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlRestartMotion);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlSetMotion);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlSetMotion_ID);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlGetMotionNo);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable2);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable3);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable4);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable5);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable6);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable7);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable8);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable9);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable10);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable11);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable12);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable13);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable14);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable15);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable16);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable17);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable18);

INCLUDE_RODATA(const s32, "code_0026B0A8", sbtlResetAtkSklEffTable19);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlResetAtkSklEff);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallBtlGetPChara);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlplayertblSetPlayerMotionData_Hokan);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlplayertblLeaf);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlSetFade);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCallIsSceanStop);

INCLUDE_ASM(const s32, "code_0026B0A8", RGBFade);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCreateTargetList_MASK);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCreateTargetList_AI);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCreateTargetList);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCreateTargetList_DeadOn);

INCLUDE_ASM(const s32, "code_0026B0A8", ChangeTarget_BSHelper);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCreateTargetList_Talk);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCreateTargetList_Analyze);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCreateTargetList_Group);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCreateTargetList_PForward);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlUpdateMiusinai);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCheckDamageTargetTemp);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlBtlCheckStartBattleSub);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlLeaf6);

INCLUDE_ASM(const s32, "code_0026B0A8", CheckLiveTarget);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCheckLiveTarget_NotHitDmg);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCheckTateSkillSelectTarget);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCheckTalkSelectTarget);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCheckTateSkillSelectTarget_sbtlUnit);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCheckExist);

INCLUDE_ASM(const s32, "code_0026B0A8", sbtlCheckExist2_sbtlUnit);
