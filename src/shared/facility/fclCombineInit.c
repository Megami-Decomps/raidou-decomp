#include "common.h"

#ifdef VERSION_RAIDOU2
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

INCLUDE_ASM(const s32, "facility/fclCombineInit", fclCombineInitWork);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapLeaf25);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapNakamaTakumi_FLUSH);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapNakamaTakumi_CheckEffStart);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapNakamaSkilIs_SetBit);

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapNakamaStockOnCheck);

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

INCLUDE_ASM(const s32, "facility/fclCombineInit", fclFunc);
#endif

INCLUDE_ASM(const s32, "facility/fclCombineInit", mapNakamaStockOnCheck_fld);

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
