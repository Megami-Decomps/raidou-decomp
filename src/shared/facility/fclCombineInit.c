#include "common.h"
#include "fclCombineInit.h"
#include "EventBit.h"
#include "newdata.h"
#include "fld.h"
#include "fldProcess.h"
#include "dds3ObjectBase.h"
#include "dds3SceneBasic.h"
#include "dds3WorldBasic.h"
#include "sbtlWork.h"
#include "code_0015D490.h"
#include "dat.h"

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "facility/fclCombineInit", mapNakamaPosRotTable);

INCLUDE_RODATA(const s32, "facility/fclCombineInit", mapNakamaSummonTaskTable);
#else
INCLUDE_RODATA(const s32, "facility/fclCombineInit", fclModelDevilLbTable);
#endif

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapNakamaSummonTask);

#ifdef VERSION_RAIDOU2
INCLUDE_RODATA(const s32, "facility/fclCombineInit", mapStrSmndvUnit);

INCLUDE_RODATA(const s32, "facility/fclCombineInit", jtbl_004AD370);
#endif

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapNakamaReturnTask);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapNakamaBranchTask);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapNakamaJoinTask);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapNakamaSearchTask_fld);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapNakamaSpotTask_fld);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapNakamaGankTask_fld);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapNakamaKoukTask);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapNakamaHakaTask);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapNakamaReiTask);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapNakamaChikaraTask);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapNakamaDokusinTask);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapNakamaIroTask);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapNakamaHikouTask);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapNakamaSinnTask);

#ifdef VERSION_RAIDOU2
INCLUDE_ASM(const s32, "facility/fclCombineInit", fclGitaiStartLabel);

INCLUDE_ASM(const s32, "facility/fclCombineInit", fclGitaiEndLabel);
#endif

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapNakamaSpotTask);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapNakamaGankTask);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapNakamaTakumiTask);

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "facility/fclCombineInit", mapRefDDS3GBWK7);
#else
INCLUDE_ASM(const s32, "facility/fclCombineInit", fclTakumiMotGdc);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "facility/fclCombineInit", mapRefDDS3GBWK8);
#else
INCLUDE_ASM(const s32, "facility/fclCombineInit", fclTakumiMotMagic);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "facility/fclCombineInit", mapRefDDS3GBWK9);
#else
INCLUDE_ASM(const s32, "facility/fclCombineInit", fclTakumiMotBstwait);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "facility/fclCombineInit", mapLeaf3);
#else
INCLUDE_ASM(const s32, "facility/fclCombineInit", fclTakumiMotWait);

#endif

void fclCombineInitWork(void) {
    sndNakamaJoinTaskSub(14, 127);
}

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapLeaf25);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapNakamaTakumi_FLUSH);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapNakamaTakumi_CheckEffStart);

#ifdef VERSION_RAIDOU1
void mapNakamaSkilIs_SetBit(s32 id)
{
    s32 i;

    for (i = 0; i < 32; i++) {
        evtBitOff(i + 2272);
    }
    if (id <= 0) {
        return;
    }
    if (id >= 81) {
        return;
    }
    switch (mapData44[id]) {
    case 8:
        evtBitOn(2280);
        break;
    case 9:
        evtBitOn(2281);
        break;
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        break;
    case 16:
        evtBitOn(2288);
        break;
    case 17:
        evtBitOn(2289);
        break;
    case 18:
        evtBitOn(2290);
        break;
    case 19:
        evtBitOn(2291);
        break;
    case 20:
        evtBitOn(2292);
        break;
    }
    switch (sbtlGetDevilData_sbtlSub(DDS3_GBWK->mapSummonID)->field0E) {
    case 0:
        break;
    case 1:
        evtBitOn(2272);
        break;
    case 2:
        evtBitOn(2273);
        break;
    case 3:
        evtBitOn(2274);
        break;
    case 4:
        evtBitOn(2275);
        break;
    case 5:
        evtBitOn(2276);
        break;
    case 6:
        evtBitOn(2277);
        break;
    case 7:
        evtBitOn(2278);
        break;
    }
}
#else
void mapNakamaSkilIs_SetBit(s32 id)
{
    s32 i;
    s32 index;
    sdatUnitWork_t *unit;

    for (i = 0; i < 32; i++) {
        evtBitOff(i + 2272);
    }
    if (id <= 0) {
        return;
    }
    if (id >= 209) {
        return;
    }
    switch (sbtlIfDrawActionSub(id)->mskskill) {
    case 8:
        evtBitOn(2280);
        break;
    case 9:
        evtBitOn(2281);
        break;
    case 10:
        evtBitOn(2282);
        break;
    case 11:
        evtBitOn(2283);
        break;
    case 12:
        evtBitOn(2284);
        break;
    case 13:
        evtBitOn(2285);
        break;
    case 14:
    case 15:
        break;
    case 16:
        evtBitOn(2288);
        break;
    case 17:
        evtBitOn(2289);
        break;
    case 18:
        evtBitOn(2290);
        break;
    case 19:
        evtBitOn(2291);
        break;
    case 20:
        evtBitOn(2292);
        break;
    case 21:
        evtBitOn(2293);
        break;
    case 22:
        evtBitOn(2294);
        break;
    case 23:
        evtBitOn(2295);
        break;
    case 24:
        evtBitOn(2296);
        break;
    case 25:
        evtBitOn(2297);
        break;
    case 26:
        evtBitOn(2298);
        break;
    }
    switch (sbtlIfDrawActionSub(DDS3_GBWK->mapSummonID)->kuda_id) {
    case 0:
        break;
    case 1:
        evtBitOn(2272);
        break;
    case 2:
        evtBitOn(2273);
        break;
    case 3:
        evtBitOn(2274);
        break;
    case 4:
        evtBitOn(2275);
        break;
    case 5:
        evtBitOn(2276);
        break;
    case 6:
        evtBitOn(2277);
        break;
    case 7:
        evtBitOn(2278);
        evtBitOn(2298);
        break;
    }
    index = datGetStockDevilIndex(DDS3_GBWK->mapSummonID);
    if (index != -1) {
        unit = &DDS3_GBWK->playerData.stock[index];
        if (datOmoideSkillHaveSkill(unit, SKLID_KANE_HIROU)) {
            evtBitOn(2289);
        }
        if (datOmoideSkillHaveSkill(unit, SKLID_ITEM_HIROU)) {
            evtBitOn(2290);
        }
    }
}
#endif

void mapNakamaStockOnCheck(void)
{
    s32 i;

    for (i = 0; i < 32; i++) {
        evtBitOff(0x8C0 + i);
    }
}

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "facility/fclCombineInit", mapNakamaSkil_SetBit);
#else
INCLUDE_ASM(const s32, "facility/fclCombineInit", fclCallBitOff);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "facility/fclCombineInit", mapIgnite3);
#else
INCLUDE_ASM(const s32, "facility/fclCombineInit", fclCallNakamaStockOnCheck);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "facility/fclCombineInit", mskGetNo);
#else
INCLUDE_ASM(const s32, "facility/fclCombineInit", mskGet);
#endif

#ifdef VERSION_RAIDOU1
/* NON_MATCHING: 16 of 61 words match.  */
#ifdef NON_MATCHING
void mapNakamaStockOnCheck_fld(void)
{
    s32 found = FALSE;
    s32 i;

    if (DDS3_GBWK->mapSummonID <= 0) {
        return;
    }
    for (i = 0; i < DDS3_GBWK->playerData.partyNum; i++) {
        if (DDS3_GBWK->mapSummonID == DDS3_GBWK->playerData.stock[i].devilId) {
            if (DDS3_GBWK->playerData.stock[i].now.hp > 0) {
                found = TRUE;
                break;
            }
        }
    }
    if (!found) {
        mapNakamaSkilIs_SetBit(0);
        mapNakamaStockOnCheck();
        DDS3_GBWK->mapSummonID = 0;
        evtBitOff(0xBB1);
        mapRiberaMa2ClearMsg();
        mapEsutoMa2ClearMsg();
        if (summonDvBasic != NULL) {
            mapBadEffFree(1);
            dds3ObjectBaseOnFlag(summonDvBasic, 1);
            dds3DeleteSceneBasicData(summonDvBasic);
            summonDvBasic = NULL;
            summonDvModel = NULL;
            dds3SetActiveUnitSceneBasic(dds3GetControlScene(), playerBasic);
        }
    }
}
#else
INCLUDE_ASM(const s32, "facility/fclCombineInit", mapNakamaStockOnCheck_fld);
#endif
#else
INCLUDE_ASM(const s32, "facility/fclCombineInit", fclFunc);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapNakamaStockOnCheck_fld);
#endif

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapEffDvReturnLoad_LB);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapEffDvReturnLoad_LB_EndCheck);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapEffDvReturnStart);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapCallEffectGetState2);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapEffDvReturnEnd);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapEffDvSummonLoad_LB);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapEffDvSummonLoad_LB_EndCheck);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapEffDvSummonStart);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapCallEffectGetState3);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapEffDvSummonEnd);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapLeaf26);

#ifdef VERSION_RAIDOU2
INCLUDE_ASM(const s32, "facility/fclCombineInit", fclLeaf);

INCLUDE_ASM(const s32, "facility/fclCombineInit", fclCallSmoothSingleMotion);

INCLUDE_ASM(const s32, "facility/fclCombineInit", fclLeaf2);

INCLUDE_ASM(const s32, "facility/fclCombineInit", fclCallGetRandIntA);

INCLUDE_ASM(const s32, "facility/fclCombineInit", fclCallBgZoomTest);

INCLUDE_ASM(const s32, "facility/fclCombineInit", fclAIUpdateSub);

INCLUDE_RODATA(const s32, "facility/fclCombineInit", AI_UpdateTable);

INCLUDE_RODATA(const s32, "facility/fclCombineInit", AI_UpdateTable2);

INCLUDE_ASM(const s32, "facility/fclCombineInit", AI_Update);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclModelDevilLbData3);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclModelDevilLbData4);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclModelDevilLbData5);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclModelDevilLbData6);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclCallSummonDvTaskData);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclCallSummonDvTaskData2);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclCallSummonDvTaskData3);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclBunriStartLabelData2);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclBunriStartLabelData3);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclBunriStartLabelData4);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclMskLabelData);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclMskLabelData2);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclMskLabelData3);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclMskLabelData4);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclMskLabelData5);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclMskLabel2Data3);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclMskLabel2Data4);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclInnenLabelData);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclInnenLabelData2);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclInnenLabelData3);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclInnenLabelData4);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclInnenLabelData5);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclInnenLabelData6);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclInnenLabelData7);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclTakaneLabelData);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclTakaneLabelData2);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclTakaneLabelData3);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclTakaneLabelData4);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclTakaneLabelData5);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclTakaneLabelData6);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclTakaneLabelData7);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclGitaiStartLabelData2);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclGitaiStartLabelData3);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclGitaiStartLabelData4);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclGitaiStartLabelData5);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclGitaiStartLabelData6);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclGitaiStartLabelData7);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclGitaiStartLabelData8);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", mapNakamaSpotTaskData3);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", mapNakamaSpotTaskData4);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", mapNakamaSpotTaskData5);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", mapNakamaSpotTaskData6);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", mapNakamaSpotTaskData7);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", mapNakamaSpotTaskData8);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", mapNakamaGankTaskData3);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", mapNakamaGankTaskData4);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", mapNakamaGankTaskData5);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", mapNakamaGankTaskData6);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", mapNakamaGankTaskData7);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", mapNakamaGankTaskData8);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclTakumiLabelData2);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclTakumiLabelData3);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclTakumiLabelData4);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclTakumiLabelData5);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclTakumiLabelData6);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclTakumiLabelData7);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclData5);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", AI_UpdateData);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", AI_UpdateData2);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", AI_UpdateData3);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", AI_UpdateData4);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", AI_UpdateData5);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", AI_UpdateData6);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", AI_UpdateData7);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", AI_UpdateData8);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", AI_UpdateData9);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", AI_UpdateData10);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", fclCallBgZoomTestData3);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", AI_UpdateData11);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", AI_UpdateData12);

INCLUDE_SDATA(const s32, "facility/fclCombineInit", nbData23);

#endif
