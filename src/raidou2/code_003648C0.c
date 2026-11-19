#include "common.h"
#include "sdfMemory.h"
#include "cmpFile.h"
#include "dat.h"
#include "cmpModel.h"
#include "dds3KernelCore.h"
#include "code_00300578.h"
#include "dds3KernelDraw.h"
#include "dds3KernelDraw_0010A750.h"
#include "code_r1_002CAFD8.h"
#include "code_003648C0.h"
#include "cmpMain.h"
#include "code_0038AD80.h"
#include "dds3PadManager.h"
#include "fldProcess.h"
#include "sdfSound.h"
#include "sdk_libc.h"
#include "mapData.h"
#include "code_r1_002C8188.h"
#include "itf.h"
#include "cmpKuda.h"
#include "cmpStatus.h"
#include "newdata.h"
#include "mdlManager.h"
#include "cmpItem.h"
#include "cmpBook.h"
#include "EventBit.h"
#include "effManager.h"
#include "sdfChip.h"
#include "sBtlSkill.h"
#include "fclMisc.h"
#include "sdfDelayedResrc.h"

INCLUDE_ASM(const s32, "code_003648C0", titleFunc4);

INCLUDE_ASM(const s32, "code_003648C0", titleFunc5);

INCLUDE_ASM(const s32, "code_003648C0", titleFunc6);

INCLUDE_ASM(const s32, "code_003648C0", titleSeffViewDraw);

INCLUDE_ASM(const s32, "code_003648C0", titleCallMemGetPtr);

INCLUDE_ASM(const s32, "code_003648C0", titleCallMemGetPtr2);

INCLUDE_ASM(const s32, "code_003648C0", titleSeffViewProcess);

INCLUDE_ASM(const s32, "code_003648C0", titleTestRelease);

INCLUDE_ASM(const s32, "code_003648C0", titleTestDeleteWorld);

INCLUDE_ASM(const s32, "code_003648C0", titleSeffViewArea);

INCLUDE_ASM(const s32, "code_003648C0", titleFunc7);

INCLUDE_ASM(const s32, "code_003648C0", titleLeaf);

INCLUDE_ASM(const s32, "code_003648C0", titleCallMatNew);

INCLUDE_ASM(const s32, "code_003648C0", titleCallMatFreeDelayed);

INCLUDE_ASM(const s32, "code_003648C0", titleCallGrAllocPacketBuff);

INCLUDE_ASM(const s32, "code_003648C0", titleCallSetOt);

INCLUDE_ASM(const s32, "code_003648C0", sbtlCallInversMatrix);

INCLUDE_ASM(const s32, "code_003648C0", sbtlCallInversMatrix2);

INCLUDE_ASM(const s32, "code_003648C0", sbtlCallInversMatrix3);

INCLUDE_ASM(const s32, "code_003648C0", sbtlCallInversMatrix4);

INCLUDE_ASM(const s32, "code_003648C0", sbtlCallMemset22);

INCLUDE_ASM(const s32, "code_003648C0", sbtlCallMemset23);

INCLUDE_ASM(const s32, "code_003648C0", sbtlSetDevilID_EnemyRot);

INCLUDE_ASM(const s32, "code_003648C0", sbtlCallMemset24);

INCLUDE_ASM(const s32, "code_003648C0", sbtlCallLinkTest);

INCLUDE_ASM(const s32, "code_003648C0", sbtlCallFptodp2);

INCLUDE_ASM(const s32, "code_003648C0", sbtlSeffActCam);

INCLUDE_ASM(const s32, "code_003648C0", sbtlCallBtlSetDevilIDEnemyRot);

INCLUDE_ASM(const s32, "code_003648C0", sbtlCallMemset25);

INCLUDE_ASM(const s32, "code_003648C0", sbtlLeaf84);

INCLUDE_ASM(const s32, "code_003648C0", sbtlCallGrAllocPacketBuff2);

INCLUDE_ASM(const s32, "code_003648C0", sbtlCallGrAllocPacketBuff3);

INCLUDE_ASM(const s32, "code_003648C0", sbtlCallDrawTagLink);

INCLUDE_ASM(const s32, "code_003648C0", sbtlLeaf85);

INCLUDE_ASM(const s32, "code_003648C0", sbtlLeaf86);

INCLUDE_ASM(const s32, "code_003648C0", sbtlCallGetRandA);

INCLUDE_ASM(const s32, "code_003648C0", sbtlCallGrAllocPacketBuff4);

INCLUDE_ASM(const s32, "code_003648C0", sbtlCallDebugDrawWireModel);

INCLUDE_ASM(const s32, "code_003648C0", sbtlCallGrAllocPacketBuff5);

INCLUDE_ASM(const s32, "code_003648C0", sbtlCallMemset26);

INCLUDE_ASM(const s32, "code_003648C0", sbtlFunc14);

INCLUDE_ASM(const s32, "code_003648C0", nbFunc33);

INCLUDE_ASM(const s32, "code_003648C0", nbFunc34);

INCLUDE_ASM(const s32, "code_003648C0", nbFunc35);

INCLUDE_ASM(const s32, "code_003648C0", nbFunc36);

INCLUDE_ASM(const s32, "code_003648C0", nbFunc37);

INCLUDE_ASM(const s32, "code_003648C0", nbFunc38);

INCLUDE_ASM(const s32, "code_003648C0", nbFunc39);

INCLUDE_ASM(const s32, "code_003648C0", nbLeaf64);

INCLUDE_ASM(const s32, "code_003648C0", nbLeaf65);

INCLUDE_ASM(const s32, "code_003648C0", nbRefCameraTM);

INCLUDE_ASM(const s32, "code_003648C0", nbCallMemset3);

INCLUDE_ASM(const s32, "code_003648C0", nbFunc40);

INCLUDE_ASM(const s32, "code_003648C0", nbFunc41);

INCLUDE_ASM(const s32, "code_003648C0", nbFunc42);

INCLUDE_ASM(const s32, "code_003648C0", nbFunc43);

INCLUDE_ASM(const s32, "code_003648C0", nbCallMultMatrixRevNoArg);

INCLUDE_ASM(const s32, "code_003648C0", nbCallIncResultCounter);

INCLUDE_ASM(const s32, "code_003648C0", nbIncResultCounter);

INCLUDE_ASM(const s32, "code_003648C0", nbLeaf66);

INCLUDE_ASM(const s32, "code_003648C0", nbLeaf67);

INCLUDE_ASM(const s32, "code_003648C0", nbCallLinkTest);

INCLUDE_ASM(const s32, "code_003648C0", nbCallMemcpy);

INCLUDE_ASM(const s32, "code_003648C0", nbFunc44);

INCLUDE_ASM(const s32, "code_003648C0", nbFunc45);

INCLUDE_ASM(const s32, "code_003648C0", nbFunc46);

INCLUDE_ASM(const s32, "code_003648C0", nbFunc47);

INCLUDE_ASM(const s32, "code_003648C0", nbCallMultMatrixRev);

INCLUDE_ASM(const s32, "code_003648C0", nbCallGetRandFloatA);

INCLUDE_ASM(const s32, "code_003648C0", nbLeaf68);

INCLUDE_ASM(const s32, "code_003648C0", nbCallToMatrix);

INCLUDE_ASM(const s32, "code_003648C0", nbCallGetRandFloatA2);

INCLUDE_ASM(const s32, "code_003648C0", nbCallGetRandFloatA3);

INCLUDE_ASM(const s32, "code_003648C0", nbCallToMatrix2);

INCLUDE_ASM(const s32, "code_003648C0", nbCallToMatrix3);

INCLUDE_ASM(const s32, "code_003648C0", nbLeaf69);

INCLUDE_ASM(const s32, "code_003648C0", nbFunc48);

INCLUDE_ASM(const s32, "code_003648C0", nbLeaf70);

INCLUDE_ASM(const s32, "code_003648C0", nbLeaf71);

INCLUDE_ASM(const s32, "code_003648C0", nbCallMemAllocDirect2);

INCLUDE_ASM(const s32, "code_003648C0", nbLeaf72);

INCLUDE_ASM(const s32, "code_003648C0", nbFileOpen);

INCLUDE_ASM(const s32, "code_003648C0", nbCallDevLoadFileEx);

INCLUDE_ASM(const s32, "code_003648C0", nbCallChipAllocDirect2);

INCLUDE_ASM(const s32, "code_003648C0", nbCallMemFreeDirect2);

INCLUDE_ASM(const s32, "code_003648C0", nbCallTexFreeDelayed2);

INCLUDE_ASM(const s32, "code_003648C0", nbFunc49);

INCLUDE_ASM(const s32, "code_003648C0", nbFunc50);

INCLUDE_ASM(const s32, "code_003648C0", nbFunc51);

INCLUDE_ASM(const s32, "code_003648C0", nbCallTexFreeDelayed3);

INCLUDE_ASM(const s32, "code_003648C0", nbCallMemFreeDirect3);

INCLUDE_ASM(const s32, "code_003648C0", nbLeaf73);

INCLUDE_ASM(const s32, "code_003648C0", nbLeaf74);

INCLUDE_ASM(const s32, "code_003648C0", nbSeffresDumpResource);

INCLUDE_ASM(const s32, "code_003648C0", nbCallLinkTest2);

INCLUDE_ASM(const s32, "code_003648C0", nbCallMemGetPtr);

INCLUDE_ASM(const s32, "code_003648C0", nbCallTexFreeDelayed4);

INCLUDE_ASM(const s32, "code_003648C0", nbLeaf75);

INCLUDE_ASM(const s32, "code_003648C0", sysCallMemGetPtr);

INCLUDE_ASM(const s32, "code_003648C0", sysFunc29);

INCLUDE_ASM(const s32, "code_003648C0", sysLeaf47);

INCLUDE_ASM(const s32, "code_003648C0", sysCallMemGetPtr2);

INCLUDE_ASM(const s32, "code_003648C0", sysFunc30);

INCLUDE_ASM(const s32, "code_003648C0", sysLeaf48);

INCLUDE_ASM(const s32, "code_003648C0", sysLeaf49);

INCLUDE_ASM(const s32, "code_003648C0", sysFunc31);

INCLUDE_ASM(const s32, "code_003648C0", sysLeaf50);

INCLUDE_ASM(const s32, "code_003648C0", sysLeaf51);

INCLUDE_ASM(const s32, "code_003648C0", sysCallChipAllocDirect);

INCLUDE_ASM(const s32, "code_003648C0", sysFunc32);

INCLUDE_ASM(const s32, "code_003648C0", sysCallGrAllocPacketBuff);

INCLUDE_ASM(const s32, "code_003648C0", sysCallGrAllocPacketBuff2);

INCLUDE_ASM(const s32, "code_003648C0", sysCallBillBoardMakeN);

INCLUDE_ASM(const s32, "code_003648C0", sysCallCos);

INCLUDE_ASM(const s32, "code_003648C0", sysCallMemAllocDirect);

INCLUDE_ASM(const s32, "code_003648C0", sysLeaf52);

INCLUDE_ASM(const s32, "code_003648C0", sysLeaf53);

INCLUDE_ASM(const s32, "code_003648C0", sysLeaf54);

INCLUDE_ASM(const s32, "code_003648C0", sysCallMemFreeDirect2);

INCLUDE_ASM(const s32, "code_003648C0", sysBILLNUM);

INCLUDE_ASM(const s32, "code_003648C0", sysCallTexAlloc);

INCLUDE_ASM(const s32, "code_003648C0", sysLeaf55);

INCLUDE_ASM(const s32, "code_003648C0", sysLeaf56);

INCLUDE_ASM(const s32, "code_003648C0", sysCallImageDataSize);

INCLUDE_ASM(const s32, "code_003648C0", sysSeffEditDraw);

INCLUDE_ASM(const s32, "code_003648C0", sysCallMemGetPtr3);

INCLUDE_ASM(const s32, "code_003648C0", sysTYPETEXTURE);

INCLUDE_ASM(const s32, "code_003648C0", frCallMemGetPtr);

INCLUDE_ASM(const s32, "code_003648C0", frSeffEditProcess);

INCLUDE_ASM(const s32, "code_003648C0", frFunc);

INCLUDE_ASM(const s32, "code_003648C0", frCallMemGetPtr2);

INCLUDE_ASM(const s32, "code_003648C0", frFunc2);

INCLUDE_ASM(const s32, "code_003648C0", frCallMemGetPtr3);

INCLUDE_ASM(const s32, "code_003648C0", frLeaf9);

INCLUDE_ASM(const s32, "code_003648C0", frCallMemGetPtr4);

INCLUDE_ASM(const s32, "code_003648C0", frCallMemGetPtr5);

INCLUDE_ASM(const s32, "code_003648C0", frCallMemGetPtr6);

INCLUDE_ASM(const s32, "code_003648C0", frLeaf10);

INCLUDE_ASM(const s32, "code_003648C0", frCallMemAllocDirect);

INCLUDE_ASM(const s32, "code_003648C0", frLeaf11);

INCLUDE_ASM(const s32, "code_003648C0", frFunc3);

INCLUDE_ASM(const s32, "code_003648C0", frFunc4);

INCLUDE_ASM(const s32, "code_003648C0", frFunc5);

INCLUDE_ASM(const s32, "code_003648C0", frFunc6);

INCLUDE_ASM(const s32, "code_003648C0", frLeaf12);

INCLUDE_ASM(const s32, "code_003648C0", frFunc7);

INCLUDE_ASM(const s32, "code_003648C0", frLeaf13);

INCLUDE_ASM(const s32, "code_003648C0", frLeaf14);

INCLUDE_ASM(const s32, "code_003648C0", frFunc8);

INCLUDE_ASM(const s32, "code_003648C0", frCallMemAllocDirect2);

INCLUDE_ASM(const s32, "code_003648C0", frLeaf15);

s32 evtFunc16(void) {
    evtStatus13_t *status;

    if (evtWork6[CMP_SPR_COMMON] == NULL) {
        return 0;
    }
    evtWork[0] = effCreateStatusBatch(13);
    status = (evtStatus13_t *)evtWork[0]->records->status;
    status->unk0 = 299;
    status->unk4 = 80;
    status->unk8 = 2;
    effConfigureIndexedSlotResource(evtWork6[CMP_SPR_COMMON], 142, evtWork[0], 0, 4);
    return 1;
}

INCLUDE_ASM(const s32, "code_003648C0", evtFunc17);

s32 evtCreateLinkSub(CMP_SPR a_no) {
    s32 mot;

    if (a_no >= CMP_SPR_NUM) {
        return 0;
    }
    mot = evtData9[a_no];
    if (mot < 0) {
        return 0;
    }
    return cmpLinkMotFunc[mot]();
}

/* NON_MATCHING: 10 of 18 words match.  */
#ifdef NON_MATCHING
u8 evtFunc18(s32 a_no) {
    effMappedResource_t **res = &evtWork[a_no];
    u8 ret = 0;

    if (*res != NULL) {
        effDestroyPackedBatch(*res);
        ret = 1;
        *res = NULL;
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "code_003648C0", evtFunc18);
#endif

s32 evtFunc19(CMP_SPR a_no)
{
    s32 mot;

    if (a_no >= CMP_SPR_NUM) {
        return 0;
    }
    mot = evtData9[a_no];
    if (mot < 0) {
        return 0;
    }
    return evtFunc18(mot);
}

/* NON_MATCHING: 7 of 29 words match.  */
#ifdef NON_MATCHING
s32 evtCallGetSpriteHnd(CMP_SPR a_no, s32 a_index) {
    sprHandle_t *hSpr;
    s32 ret = 0;

    if ((u32)a_no < CMP_SPR_NUM && a_index >= 0) {
        hSpr = cmpGetSpriteHnd(a_no);
        hSpr->workEntries[a_index].states[0].flags &= ~1;
        effInitializeSlotWork(hSpr, a_index);
        return 1;
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "code_003648C0", evtCallGetSpriteHnd);
#endif

s32 evtFunc20(void) {
    evtCallGetSpriteHnd(CMP_SPR_COMMON, 142);
    return 1;
}

INCLUDE_ASM(const s32, "code_003648C0", evtStartTerminalMenuMessage);

INCLUDE_ASM(const s32, "code_003648C0", cmpResetMotion);

INCLUDE_ASM(const s32, "code_003648C0", cmpFreeSpriteMem);

INCLUDE_ASM(const s32, "code_003648C0", cmpLoadSpriteMem);

s32 cmpFreeSpriteHnd(CMP_SPR a_no) {
    struct sprHandle_t **set = &evtWork6[a_no];
    s32 *count;

    if (a_no >= CMP_SPR_NUM) {
        return 0;
    }
    if (*set == NULL) {
        cmpWork8[a_no] = 0;
        return 0;
    }
    count = &cmpWork8[a_no];
    if (--*count > 0) {
        return 0;
    }
    effDestroyResourceSlotSet(*set);
    *count = 0;
    *set = NULL;
    evtFunc19(a_no);
    return 1;
}

s32 cmpSetupCmpItemSub(CMP_SPR a_no) {
    struct sprHandle_t **spr = &evtWork6[a_no];
    struct sprHandle_t *hSpr;

    if ((u32)a_no < CMP_SPR_NUM) {
        if (*spr == NULL || cmpWork8[a_no] == 0) {
            hSpr = effCreateResourceSlotSetFromAllocation(cmpWork3[a_no], 0);
            cmpWork8[a_no] = 1;
            *spr = hSpr;
            evtCreateLinkSub(a_no);
            return 1;
        }
        cmpWork8[a_no]++;
        return 0;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_003648C0", cmpLoadSpriteToMem);

INCLUDE_ASM(const s32, "code_003648C0", cmpFreeSpriteToMem);

void cmpFreeSpriteToGS(void) {
    s32 i;

    for (i = 0; i < 5; i++) {
        cmpFreeSpriteHnd(i);
    }
}

struct sdfMemHandle_t *cmpLoadSpriteHnd(CMP_SPR a_no)
{
    struct sdfMemHandle_t *handle = NULL;

    if (a_no < CMP_SPR_NUM) {
        handle = cmpWork3[a_no];
    }
    return handle;
}

struct sprHandle_t *cmpGetSpriteHnd(CMP_SPR a_no)
{
    struct sprHandle_t *hSpr = NULL;

    if (a_no < CMP_SPR_NUM) {
        hSpr = evtWork6[a_no];
    }
    return hSpr;
}

INCLUDE_ASM(const s32, "code_003648C0", cmpFirstInitSub);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallDevLoadFileEx);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGrAllocPacketBuff3);

/* NON_MATCHING: 3 of 22 words match.  */
#ifdef NON_MATCHING
s32 cmpChkDevilStockExist(s32 a_stockID) {
    sdatUnitWork_t *unit;
    s32 ret = 0;

    if ((u32)a_stockID < CMP_STOCK_NUM) {
        unit = &DDS3_GBWK->playerData.stock[a_stockID];
        if (unit->flag & 1) {
            ret = unit->devilId != 0;
        }
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "code_003648C0", cmpChkDevilStockExist);
#endif

INCLUDE_ASM(const s32, "code_003648C0", cmpDelDevilStock);

s32 cmpGetDevilStockDevilID(s32 a_stockID) {
    if (cmpChkDevilStockExist(a_stockID)) {
        return DDS3_GBWK->playerData.stock[a_stockID].devilId;
    }
    return -1;
}

/* NON_MATCHING: 3 of 20 words match.  */
#ifdef NON_MATCHING
BST cmpGetBstNo(BSTF a_bst) {
    BST ret = BST_BOYCOTT;

    if (a_bst & BSTF_KANSEN) {
        ret = BST_KANSEN;
    } else if (a_bst & BSTF_BOYCOTT) {
        ret = BST_UNK19;
    } else {
        if (a_bst & BSTF_STONE) {
            ret = BST_STONE;
        }
        if (a_bst & BSTF_MOUDOKU) {
            ret = BST_MOUDOKU;
        }
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "code_003648C0", cmpGetBstNo);
#endif

void cmpUpdateBulletNum(void) {
    s32 bulletno = DDS3_GBWK->playerData.bulletno;

    if (bulletno > 0) {
        DDS3_GBWK->playerData.hgunnum = DDS3_GBWK->playerData.item[bulletno];
    } else {
        DDS3_GBWK->playerData.hgunnum = 0;
    }
}

INCLUDE_ASM(const s32, "code_003648C0", cmpPartyDrawFunc);

dds3ProcessID_t *cmpStockListProcessStart(void)
{
    dds3ProcessID_t *PID = itfMenuProcessStart();

    cmpStockListMake(PID);
    return PID;
}

INCLUDE_ASM(const s32, "code_003648C0", cmpPartyList);

INCLUDE_ASM(const s32, "code_003648C0", cmpStockListMake);

void datDlcNoneCheck(dds3ProcessID_t *PID, struct sprHandle_t *hSpr) {
    if (!itfMenuLinkSprite(PID, hSpr)) {
        printf(datStrErrorItfMenuLinkSprite);
    }
    itfMenuSetSpriteData(PID, ITFMENU_IMG_DAT_PANEL, 219, 27, 293, 25, 14, 2, NULL, datDlcNoneCheckData);
    itfMenuSetSpriteData(PID, ITFMENU_IMG_DAT_SELECT, 219, 27, 293, 25, 14, 2, NULL, datDlcNoneCheckData2);
    itfGeneralMenuCreatePIDSub(PID, 3);
    itfGeneralMenuCreatePIDSub2(PID, 3);
}

INCLUDE_ASM(const s32, "code_003648C0", datLeaf82);

INCLUDE_ASM(const s32, "code_003648C0", datLeaf83);

INCLUDE_ASM(const s32, "code_003648C0", datLeaf84);

INCLUDE_ASM(const s32, "code_003648C0", datCallGetRandIntA2);

INCLUDE_ASM(const s32, "code_003648C0", datCallGetRandIntA3);

INCLUDE_ASM(const s32, "code_003648C0", datCallGetRandIntA4);

INCLUDE_ASM(const s32, "code_003648C0", datFunc26);

INCLUDE_ASM(const s32, "code_003648C0", cmpLeaf42);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetRandIntA);

INCLUDE_ASM(const s32, "code_003648C0", cmpFunc5);

INCLUDE_ASM(const s32, "code_003648C0", cmpLeaf43);

INCLUDE_ASM(const s32, "code_003648C0", cmpFunc6);

INCLUDE_ASM(const s32, "code_003648C0", cmpCampTestMomiji);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallTexFreeDelayed);

INCLUDE_ASM(const s32, "code_003648C0", cmpSetDraw);

INCLUDE_ASM(const s32, "code_003648C0", cmpGetDraw);

INCLUDE_ASM(const s32, "code_003648C0", cmpSetType);

INCLUDE_ASM(const s32, "code_003648C0", cmpSetId);

INCLUDE_ASM(const s32, "code_003648C0", cmpLeaf44);

INCLUDE_ASM(const s32, "code_003648C0", cmpLeaf45);

INCLUDE_ASM(const s32, "code_003648C0", fldLeaf34);

INCLUDE_ASM(const s32, "code_003648C0", fldCallButtonLineHelpTextSet);

INCLUDE_ASM(const s32, "code_003648C0", fldCamUpdateDefault);

INCLUDE_ASM(const s32, "code_003648C0", fldLeaf5);

INCLUDE_ASM(const s32, "code_003648C0", fldCallMemAllocDirect);

INCLUDE_ASM(const s32, "code_003648C0", fldShopSetumeiSpr);

INCLUDE_ASM(const s32, "code_003648C0", cmpProcessShpMainSub);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallMemFreeDelayedDirect);

void cmpLoadModelTables(void)
{
    sdfDevLoadFileEx(cmpStrGohmadenGohDevilParamDat, &datCallCheckStrongSwordAttackData, &cmpLoadModelTablesData);
    sdfDevLoadFileEx(cmpStrGohmadenGohSameDevilsDat, &datData6, &datCallGetStockDevilIDData);
}

s32 cmpGetFlagScreenShot(void) {
    return cmpDrawStatusModel_cmpModelData != NULL;
}

INCLUDE_ASM(const s32, "code_003648C0", cmpSetupCmpModelSub);

void cmpInitCmpModelSub(void) {
    cmpInitCore2();
}

INCLUDE_ASM(const s32, "code_003648C0", cmpSetLight);

INCLUDE_ASM(const s32, "code_003648C0", cmpInitCore2);

INCLUDE_ASM(const s32, "code_003648C0", cmpSetLightUnit);

/* NON_MATCHING: 4 of 28 words match.  */
#ifdef NON_MATCHING
void cmpInitDstEff2(s32 a_type) {
    effHandle_t **eff = &cmpWork2[a_type];

    if ((u32)a_type < 2 && *eff != NULL) {
        fclInitDstEff2Sub((struct plistHandle_t *)(*eff)->instance);
        dds3EffectSetLoop_dds3Effect(*eff, 1);
        dds3EffectSetScale(*eff, 1.0f);
    }
}
#else
INCLUDE_ASM(const s32, "code_003648C0", cmpInitDstEff2);
#endif

void cmpInitDstEff(void) {
    u32 data[4];

    cmpWork[0] = sdfDevLoadFileEx(cmpStrCampBad01Epl, data, NULL);
    cmpWork2[0] = dds3EffectLoadEPL((void *)data[0]);
    cmpInitDstEff2(0);
    cmpWork[1] = sdfDevLoadFileEx(cmpStrCampBad04Epl, data, NULL);
    cmpWork2[1] = dds3EffectLoadEPL((void *)data[0]);
    cmpInitDstEff2(1);
}

INCLUDE_ASM(const s32, "code_003648C0", cmpCreateBstEff);

void cmpDeleteBstEff(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        if (cmpWork2[i] != NULL) {
            dds3EffectDelete(cmpWork2[i]);
            cmpWork2[i] = NULL;
        }
        if (cmpWork[i] != NULL) {
            sdfMemFreeDelayedDirect(cmpWork[i]);
            cmpWork[i] = NULL;
        }
    }
    g_hEffBst = NULL;
    g_bstID = 21;
    cmpDeleteBstEffData = 0;
    cmpDeleteBstEffData2 = 0;
}

INCLUDE_ASM(const s32, "code_003648C0", cmpDrawBstEff);

/* NON_MATCHING: 6 of 27 words match.  */
#ifdef NON_MATCHING
void Init_cmpModel(void) {
    cmpInitCmpModelSub();
    cmpCampEFFTES();
    cmpInitDstEff();
    cmpData18 = NULL;
    cmpData18 = sdfChipAllocZeroDirect(0xE0);
    cmpData11 = 7;
    cmpSetModelMotionData = -1;
    cmpData17 = 30.0f;
    cmpData19 = -1;
    cmpData10 = 0;
    cmpData12 = 0;
    cmpData13 = 0;
    cmpData16 = 0;
}
#else
INCLUDE_ASM(const s32, "code_003648C0", Init_cmpModel);
#endif

void End_cmpModel(void) {
    cmpWaitLoadModel();
    cmpFreeResource();
    cmpDeleteBstEff();
    cmpDeleteEffect();
    if (cmpData18 != NULL) {
        sdfChipFreeDirect(cmpData18);
        cmpData18 = NULL;
    }
}

s32 cmpFreeResource(void) {
    if (cmpGetFlagScreenShot()) {
        mdlFree(cmpDrawStatusModel_cmpModelData);
        cmpDrawStatusModel_cmpModelData = NULL;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_003648C0", cmpCallPrepare);

INCLUDE_ASM(const s32, "code_003648C0", Setup);

INCLUDE_ASM(const s32, "code_003648C0", Setup_cmpModel);

/* NON_MATCHING: 32 of 48 words match.  */
#ifdef NON_MATCHING
s32 cmpLoadStatusModel(void) {
    if (cmpData10 == 1) {
        if (sdfDelayedResrcCheck()) {
            return 1;
        }
        cmpCallPrepare(cmpData11, cmpData12, cmpData13);
        cmpData10 = 2;
    }
    if (cmpData10 == 0) {
        return 0;
    }
    if (cmpCallPrepare(cmpData11, cmpData12, cmpData13) == NULL) {
        return 1;
    }
    if (cmpCallPrepare(cmpData11, cmpData12, cmpData13) == (dds3ModelResrc_t *)-1) {
        return 1;
    }
    cmpFreeResource();
    cmpDrawStatusModel_cmpModelData = mdlLoad(cmpData11, cmpData12);
    cmpSetModelMotion(cmpDrawStatusModel_cmpModelData);
    cmpCreateBstEff();
    cmpData10 = 0;
    return 2;
}
#else
INCLUDE_ASM(const s32, "code_003648C0", cmpLoadStatusModel);
#endif

s32 cmpWaitLoadModel(void) {
    if (cmpLoadStatusModel() == 1) {
        cmpCallPrepare(cmpData11, cmpData12, 1);
        if (cmpLoadStatusModel() != 1) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_003648C0", cmpSetModelMotion);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetFlagScreenShot2);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetFlagScreenShot3);

INCLUDE_ASM(const s32, "code_003648C0", cmpSetSmoothLoopMotion);

INCLUDE_ASM(const s32, "code_003648C0", cmpCheckHandle);

INCLUDE_ASM(const s32, "code_003648C0", cmpSeqGoodbySub);

INCLUDE_ASM(const s32, "code_003648C0", cmpGetMotionNum);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetFlagScreenShot4);

INCLUDE_ASM(const s32, "code_003648C0", cmpGetMotionSpeed);

INCLUDE_ASM(const s32, "code_003648C0", cmpSetModelScale);

INCLUDE_ASM(const s32, "code_003648C0", cmpSetModelPosition);

INCLUDE_ASM(const s32, "code_003648C0", cmpSetModelRotate);

INCLUDE_ASM(const s32, "code_003648C0", cmpSetStatusModelParam);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetFlagScreenShot5);

INCLUDE_ASM(const s32, "code_003648C0", cmpLeaf46);

INCLUDE_ASM(const s32, "code_003648C0", cmpLeaf47);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetFlagScreenShot6);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetFlagScreenShot7);

INCLUDE_ASM(const s32, "code_003648C0", cmpAddPos);

INCLUDE_ASM(const s32, "code_003648C0", cmpSetPos_GoodBy);

INCLUDE_ASM(const s32, "code_003648C0", cmpAdjustPosY_Gattai);

INCLUDE_ASM(const s32, "code_003648C0", cmpAddRot);

INCLUDE_ASM(const s32, "code_003648C0", cmpSetRot_GoodBy);

INCLUDE_ASM(const s32, "code_003648C0", cmpAddScl);

INCLUDE_ASM(const s32, "code_003648C0", itfMesMngDebugShutdown);

INCLUDE_ASM(const s32, "code_003648C0", cmpClearDat);

INCLUDE_ASM(const s32, "code_003648C0", cmpDrawStatusModel);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallLoadStatusModel);

INCLUDE_ASM(const s32, "code_003648C0", cmpGetViewerLimit);

INCLUDE_ASM(const s32, "code_003648C0", cmpSetViewerLimit);

INCLUDE_ASM(const s32, "code_003648C0", cmpGetPosLimit);

INCLUDE_ASM(const s32, "code_003648C0", cmpSetPosLimit);

INCLUDE_ASM(const s32, "code_003648C0", cmpSetLightDef);

INCLUDE_ASM(const s32, "code_003648C0", ModelLoadBuffEnd);

INCLUDE_ASM(const s32, "code_003648C0", cmpCampEFFTES);

void cmpDeleteEffect(void) {
    if (fldTitleMiniTex5 != NULL) {
        dds3EffectDelete(fldTitleMiniTex5);
        fldTitleMiniTex5 = NULL;
    }
    if (fldTitleMiniWork5 != NULL) {
        sdfMemFreeDirect(fldTitleMiniWork5);
        fldTitleMiniWork5 = NULL;
        cmpCampEFFTESData = 0;
    }
}

INCLUDE_ASM(const s32, "code_003648C0", cmpEffectSetRGBA);

INCLUDE_ASM(const s32, "code_003648C0", cmpDrawEffect);

INCLUDE_ASM(const s32, "code_003648C0", cmpLeaf4);

INCLUDE_ASM(const s32, "code_003648C0", cmpSeqGoodbySub2);

INCLUDE_ASM(const s32, "code_003648C0", cmpDrawPartsOmoideSkillSub2);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallExistProcess);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallExistProcess2);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallExistProcess3);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallExistProcess4);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallExistProcess5);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallExistProcess6);

INCLUDE_ASM(const s32, "code_003648C0", CmpStatusSetOfficialSkl);

INCLUDE_ASM(const s32, "code_003648C0", cmpLearnSkl);

INCLUDE_ASM(const s32, "code_003648C0", cmpGetSklCsrMax_R);

INCLUDE_ASM(const s32, "code_003648C0", cmpSetSklCsrMax);

INCLUDE_ASM(const s32, "code_003648C0", cmpGetSklCsrMax);

INCLUDE_ASM(const s32, "code_003648C0", cmpLeaf5);

INCLUDE_ASM(const s32, "code_003648C0", cmpGetCsrSelSkl);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallExistProcess7);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallExistProcess8);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallExistProcess9);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallExistProcess10);

INCLUDE_ASM(const s32, "code_003648C0", cmpAddEffScl);

INCLUDE_ASM(const s32, "code_003648C0", cmpGetMotionParam);

INCLUDE_ASM(const s32, "code_003648C0", cmpSetModelLight);

INCLUDE_ASM(const s32, "code_003648C0", cmpUpdatemodel);

INCLUDE_ASM(const s32, "code_003648C0", cmpViewerModel);

INCLUDE_ASM(const s32, "code_003648C0", cmpDrawParamSub);

INCLUDE_ASM(const s32, "code_003648C0", cmpCalcRestParamPly);

INCLUDE_ASM(const s32, "code_003648C0", cmpCalcRestParamDvl);

INCLUDE_ASM(const s32, "code_003648C0", cmpCalcRestParam);

INCLUDE_ASM(const s32, "code_003648C0", cmpCalcParam);

INCLUDE_ASM(const s32, "code_003648C0", cmpResetParam);

INCLUDE_ASM(const s32, "code_003648C0", cmpCtrlSetParam);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallCreateLink);

INCLUDE_ASM(const s32, "code_003648C0", cmpLvUpSeqFadeInStepUpdate);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallWaitLoadModel);

INCLUDE_ASM(const s32, "code_003648C0", cmpSequence);

INCLUDE_ASM(const s32, "code_003648C0", cmpMemSetSeqStatusStepUpdate);

INCLUDE_ASM(const s32, "code_003648C0", cmpSeqGoodby);

INCLUDE_ASM(const s32, "code_003648C0", cmpSTATUSSTEP);

INCLUDE_ASM(const s32, "code_003648C0", cmpSeqRankUp);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallEndCmpModel);

void CallProcEnd_cmpStatus(void) {
}

s32 Process_cmpStatus(void)
{
    return 0;
}

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetProcessData11);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallDrawFont2);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallDrawFont3);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallDrawFont4);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallSetFontSpace);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetSpriteHnd3);

INCLUDE_ASM(const s32, "code_003648C0", cmpDrawSprList);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetSpriteHnd4);

INCLUDE_ASM(const s32, "code_003648C0", cmpDrawParam);

INCLUDE_ASM(const s32, "code_003648C0", cmpDrawParamCursor);

INCLUDE_ASM(const s32, "code_003648C0", cmpDrawParameter);

s32 cmpGetNowSummonerRank(void) {
    s32 i = 0;
    s32 rank;

    for (rank = 29;; rank--) {
        s32 bit = cmpGetSummonerRank2SprNoData[i++].bit;

        if (bit < 0 || evtBitCheck(bit)) {
            break;
        }
    }
    return rank;
}

/* NON_MATCHING: 1 of 32 words match.  */
#ifdef NON_MATCHING
s32 cmpGetSummonerRank2SprNo(void) {
    s32 i;
    s32 sprNo = 29;

    for (i = 0; cmpGetSummonerRank2SprNoData[i].bit >= 0; i++) {
        if (evtBitCheck(cmpGetSummonerRank2SprNoData[i].bit)) {
            sprNo = cmpGetSummonerRank2SprNoData[i].sprNo + 29;
            break;
        }
    }
    return sprNo;
}
#else
INCLUDE_ASM(const s32, "code_003648C0", cmpGetSummonerRank2SprNo);
#endif

INCLUDE_ASM(const s32, "code_003648C0", cmpSummnerRankObjectInit);

void cmpSummnerRankMngInit(cmpSummnerRankMng_t *psSelf) {
    cmpSummnerRankInit(&psSelf->sSummnerRank);
    cmpSummnerRankMngSetState(psSelf, cmpSummnerRankStateWait);
}

void cmpSummnerRankMngSetState(cmpSummnerRankMng_t *psSelf, cmpSummnerRankFunc_t cbFunc) {
    psSelf->cbState = cbFunc;
    psSelf->sSummnerRank.iCounter = 0;
}

INCLUDE_ASM(const s32, "code_003648C0", cmpSummnerRankMngExecute);

void cmpSummnerRankInit(cmpSummnerRank_t *psSelf) {
    s32 sprNo = cmpGetSummonerRank2SprNo();

    memset(psSelf, 0, sizeof(cmpSummnerRank_t));
    psSelf->bSummnerRankStart = 0;
    psSelf->bSummnerRankWait = 1;
    cmpSummnerRankObjectInit(&psSelf->sObjBG);
    cmpSummnerRankObjectInit(&psSelf->sObjBGSRank);
    cmpSummnerRankObjectInit(&psSelf->sObjBGRaido);
    cmpSummnerRankObjectInit(&psSelf->sObjCyusei);
    cmpSummnerRankObjectInit(&psSelf->sObjSRank);
    cmpSummnerRankObjectInit(&psSelf->sObjRaido);
    cmpSummnerRankObjectInit(&psSelf->sObjBlur);
    psSelf->sObjBGSRank.bEnabled = 1;
    psSelf->sObjBGSRank.iRankSprNo = sprNo;
    psSelf->sObjBGRaido.bEnabled = 1;
    psSelf->sObjSRank.iRankSprNo = sprNo;
}

INCLUDE_ASM(const s32, "code_003648C0", cmpSummnerRankStateWait);

INCLUDE_ASM(const s32, "code_003648C0", cmpSummnerRankStateBGHalf);

INCLUDE_ASM(const s32, "code_003648C0", cmpSummnerRankStateCyuuseiMeimetsu);

INCLUDE_ASM(const s32, "code_003648C0", cmpSummnerRankStateSummnerRankOldFadein);

INCLUDE_ASM(const s32, "code_003648C0", cmpSummnerRankStateSummnerRankOldBig);

INCLUDE_ASM(const s32, "code_003648C0", cmpSummnerRankStateSummnerRankWait);

INCLUDE_ASM(const s32, "code_003648C0", cmpSummnerRankStateSummnerRankFadeout);

INCLUDE_ASM(const s32, "code_003648C0", cmpSummnerRankStateSummnerRankNewFadein);

INCLUDE_ASM(const s32, "code_003648C0", cmpSummnerRankStateSummnerRankNew);

INCLUDE_ASM(const s32, "code_003648C0", cmpSummnerRankStateSummnerRankNewSmall);

INCLUDE_ASM(const s32, "code_003648C0", cmpSummnerRankStateSummnerRankNewQuake);

INCLUDE_ASM(const s32, "code_003648C0", cmpSummnerRankStateSummnerRankRaidoPika);

INCLUDE_ASM(const s32, "code_003648C0", cmpLeaf6);

INCLUDE_ASM(const s32, "code_003648C0", cmpSummnerRankStateBGFukki);

INCLUDE_ASM(const s32, "code_003648C0", cmpSummnerRankDrawCyusei);

INCLUDE_ASM(const s32, "code_003648C0", cmpSummnerRankDrawSummnerRank);

INCLUDE_ASM(const s32, "code_003648C0", cmpSummnerRankDrawRaido);

INCLUDE_ASM(const s32, "code_003648C0", cmpSummnerRankDrawSummnerRankBlur);

INCLUDE_ASM(const s32, "code_003648C0", cmpSummnerRankDraw);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallDrawSprList);

INCLUDE_ASM(const s32, "code_003648C0", cmpDrawPartsExp);

INCLUDE_ASM(const s32, "code_003648C0", cmpGetSwordName);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetSwordName);

INCLUDE_ASM(const s32, "code_003648C0", cmpRefDDS3GBWK);

INCLUDE_ASM(const s32, "code_003648C0", cmpGetSwordNameEx);

INCLUDE_ASM(const s32, "code_003648C0", GetValueColor);

INCLUDE_ASM(const s32, "code_003648C0", cmpDrawEquip);

INCLUDE_ASM(const s32, "code_003648C0", cmpLeaf7);

INCLUDE_ASM(const s32, "code_003648C0", cmpLeaf48);

INCLUDE_ASM(const s32, "code_003648C0", sbtlCallSprintf);

INCLUDE_ASM(const s32, "code_003648C0", sbtlFunc7);

s32 sbtlChkRepeat(SKLID a_sklId) {
    struct SBTLSKILLP_TBL *tbl = sbtlGetSkilltbl(a_sklId);

    if (tbl == NULL) {
        return -1;
    }
    if (tbl->type & 0x1000) {
        return 66;
    }
    if (tbl->type & 0x2000) {
        return 65;
    }
    if (sbtlCallBtlGetSkilltbl15(a_sklId)) {
        return 64;
    }
    return 67;
}

INCLUDE_ASM(const s32, "code_003648C0", sbtlDrawSkillNameExSub);

INCLUDE_ASM(const s32, "code_003648C0", cmpDrawSkillNameEx);

INCLUDE_ASM(const s32, "code_003648C0", drawMiscHelp);

INCLUDE_ASM(const s32, "code_003648C0", cmpDrawPartsOmoideSkillSub);

INCLUDE_ASM(const s32, "code_003648C0", cmpDrawPartsOmoideSkill);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallDrawSprList2);

INCLUDE_ASM(const s32, "code_003648C0", DrawHelp);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallMoverGetPosition);

INCLUDE_ASM(const s32, "code_003648C0", DrawProcess_cmpStatus);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetProcessData12);

INCLUDE_ASM(const s32, "code_003648C0", Start_cmpStatus);

INCLUDE_ASM(const s32, "code_003648C0", End);

s32 cmpChkEnd(void) {
    cmpStatusWork_t *wk = dds3GetProcessData(g_mainPID);

    if (wk->step2 == 5) {
        return wk->step == 7;
    }
    return wk->step == 9;
}

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetProcessData13);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetProcessData14);

INCLUDE_ASM(const s32, "code_003648C0", cmpStartRankUp);

INCLUDE_ASM(const s32, "code_003648C0", cmpChkWait2);

INCLUDE_ASM(const s32, "code_003648C0", cmpChkEnd2);

INCLUDE_ASM(const s32, "code_003648C0", cmpStartLearnSkillMessage);

INCLUDE_ASM(const s32, "code_003648C0", cmpSetRecover);

INCLUDE_ASM(const s32, "code_003648C0", cmpSetFlagDraw);

s32 cmpChkDraw(void) {
    cmpStatusWork_t *wk;

    if (g_mainPID != NULL && dds3ExistProcess(g_mainPID)) {
        wk = dds3GetProcessData(g_mainPID);
        if (wk->flags & 0x100) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetProcessData35);

void cmpSetFlagCamp(void) {
    cmpStatusWork_t *wk = dds3GetProcessData(g_mainPID);

    wk->flags = (wk->flags & ~0x1000) | 0x2000;
}

INCLUDE_ASM(const s32, "code_003648C0", StatusGoSword);

INCLUDE_ASM(const s32, "code_003648C0", cmpGetFlagCallDebug);

INCLUDE_ASM(const s32, "code_003648C0", Init_cmpStatus);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetProcessData15);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallInitCmpModel);

void Shutdown_cmpStatus(dds3ProcessID_t *id)
{
    cmpStatusWork_t *wk = dds3GetProcessData(id);

    cmpCallEndCmpModel(wk);
    if (dds3ExistProcess(wk->drawPID)) {
        dds3KillProcess(wk->drawPID, 0);
        wk->drawPID = NULL;
    }
    sdfMemFreeDirect(wk->hMem);
    g_mainPID = NULL;
}

dds3ProcessID_t *ProcessStart_cmpStatus(void)
{
    struct sdfMemHandle_t *hMem = sdfMemAllocDirect(sizeof(cmpStatusWork_t));
    cmpStatusWork_t *wk = sdfMemGetPtr(hMem);

    memset(wk, 0, sizeof(cmpStatusWork_t));
    wk->hMem = hMem;
    g_mainPID = dds3StartProcess(ProcessStart_cmpStatusStrCampStatusMain, 0x3EE, 0, 0, cmpCallGetProcessData11, (dds3ProcessFunc_t)Shutdown_cmpStatus, wk);
    wk->drawPID = dds3StartProcess(ProcessStart_cmpStatusStrCampStatusDraw, 0x2B03, 0, 0, cmpCallGetProcessData12, NULL, wk);
    cmpCallInitCmpModel(wk);
    return g_mainPID;
}

void ProcessEnd_cmpStatus(void) {
    cmpStatusWork_t *wk;

    if (g_mainPID != NULL && dds3ExistProcess(g_mainPID)) {
        wk = dds3GetProcessData(g_mainPID);
        if (!(wk->flags & 1)) {
            wk->flags |= 1;
            wk->step2 = 3;
            wk->count = 0;
            wk->step = 0;
        }
    }
}

s32 ProcessCheck_cmpStatus(void) {
    if (g_mainPID == NULL) {
        return 0;
    }
    if (dds3GetProcessStat(g_mainPID) == 3 || dds3GetProcessStat(g_mainPID) == 0) {
        g_mainPID = NULL;
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_003648C0", cmpLeaf8);

INCLUDE_ASM(const s32, "code_003648C0", cmpLeaf9);

INCLUDE_ASM(const s32, "code_003648C0", cmpSetModeSq);

INCLUDE_ASM(const s32, "code_003648C0", cmpDecideSuteSkl);

INCLUDE_ASM(const s32, "code_003648C0", cmpSlctSuteSkl);

INCLUDE_ASM(const s32, "code_003648C0", cmpSuteKakuninMsg);

INCLUDE_ASM(const s32, "code_003648C0", cmpOmoideModoshi);

INCLUDE_ASM(const s32, "code_003648C0", cmpMemSklCurPrev);

INCLUDE_ASM(const s32, "code_003648C0", cmpMemSklCurPrevLoop);

INCLUDE_ASM(const s32, "code_003648C0", cmpMemSklCurNext);

INCLUDE_ASM(const s32, "code_003648C0", cmpMemSklCurNextLoop);

INCLUDE_ASM(const s32, "code_003648C0", cmpMemSklCurPrevPage);

INCLUDE_ASM(const s32, "code_003648C0", cmpMemSklCurPrevPageLoop);

INCLUDE_ASM(const s32, "code_003648C0", cmpMemSklCurNextPage);

INCLUDE_ASM(const s32, "code_003648C0", cmpMemSklCurNextPageLoop);

INCLUDE_ASM(const s32, "code_003648C0", cmpMemSklCurSelPage);

INCLUDE_ASM(const s32, "code_003648C0", cmpSetBtlSkl);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetProcessData16);

INCLUDE_ASM(const s32, "code_003648C0", cmpMemSet);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetProcessData17);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetProcessData18);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetProcessData19);

INCLUDE_ASM(const s32, "code_003648C0", cmpInitModeSq);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetProcessData20);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetProcessData21);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetProcessData22);

INCLUDE_ASM(const s32, "code_003648C0", cmpStartGoodby);

INCLUDE_ASM(const s32, "code_003648C0", cmpUDHelpNon);

INCLUDE_ASM(const s32, "code_003648C0", cmpNoCallVoiceSet);

INCLUDE_ASM(const s32, "code_003648C0", cmpCheckStepGoodbyMessage);

INCLUDE_ASM(const s32, "code_003648C0", cmpSetEvent);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetProcessData23);

INCLUDE_ASM(const s32, "code_003648C0", cmpStartSklSet);

INCLUDE_ASM(const s32, "code_003648C0", cmpStartDvlReport);

INCLUDE_ASM(const s32, "code_003648C0", mtrUnitSelectInit);

INCLUDE_ASM(const s32, "code_003648C0", mtrCallMultiBlurOff);

INCLUDE_ASM(const s32, "code_003648C0", cmpEquipSwordNoHelp);

INCLUDE_ASM(const s32, "code_003648C0", cmpLeaf10);

INCLUDE_ASM(const s32, "code_003648C0", cmpLeaf11);

INCLUDE_ASM(const s32, "code_003648C0", cmpLeaf12);

INCLUDE_ASM(const s32, "code_003648C0", cmpLeaf13);

INCLUDE_ASM(const s32, "code_003648C0", cmpMemSklCurPrevSub);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetProcessData24);

INCLUDE_ASM(const s32, "code_003648C0", cmpCurGetSelectMemSklID);

INCLUDE_ASM(const s32, "code_003648C0", cmpLeaf14);

s32 cmpCallMenuGetSelectID(void) {
    s32 id = itfMenuGetSelectID(cmpPartyDrawFuncSub());
    s32 ret = -1;

    if (id == 1) {
        ret = DDS3_GBWK->unk4242;
    } else if (id == 2) {
        ret = DDS3_GBWK->unk4244;
    } else if (id >= 4) {
        ret = id + cmpGetStockPID()->page * 8 - 4;
    }
    return ret;
}

INCLUDE_ASM(const s32, "code_003648C0", cmpRefDDS3GBWK2);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallCombineDraw);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallCombineDraw2);

INCLUDE_ASM(const s32, "code_003648C0", cmpRefDDS3GBWK3);

INCLUDE_ASM(const s32, "code_003648C0", cmpMenuFilterUnit);

INCLUDE_ASM(const s32, "code_003648C0", cmpMenuFilter);

s32 cmpCallGetStockPID(void)
{
    return cmpMenuFilter(cmpGetStockPID());
}

void cmpSetFilterEx(sdatUnitWork_t *psUse, ecmpKudaSelectMode eSelMode, ecmpKudaCMU iSelectAttrId,
                    ecmpKudaCMU iDecideAttrId) {
    struct tagCMP_WORK *wk = cmpGetStockPID();

    wk->psUse = psUse;
    wk->eSelMode = eSelMode;
    wk->iAttrId = iSelectAttrId;
    wk->iSelectAttrId = iSelectAttrId;
    wk->iDecideAttrId = iDecideAttrId;
}

void cmpSetFilter(sdatUnitWork_t *psUse, ecmpKudaSelectMode eSelMode, ecmpKudaCMU iAttrId) {
    cmpSetFilterEx(psUse, eSelMode, iAttrId, iAttrId);
}

void cmpSetFilterBadStatus(BSTF iBadStatus) {
    cmpGetStockPID()->iBadStatus = iBadStatus;
}

void cmpClearFilter(void) {
    cmpSetFilter(NULL, ecmpKudaSelectMode_ecmpKSM_Target, ecmpKudaCMU_None);
    cmpSetFilterBadStatus(BSTF_NONE);
}

INCLUDE_ASM(const s32, "code_003648C0", cmpChkEnableUseSkill);

INCLUDE_ASM(const s32, "code_003648C0", cmpChkSkillSelect);

INCLUDE_ASM(const s32, "code_003648C0", cmpUpdateEnableUseSkillUnit);

INCLUDE_ASM(const s32, "code_003648C0", CallFuncSkillSet);

INCLUDE_ASM(const s32, "code_003648C0", cmpNakamaView);

INCLUDE_ASM(const s32, "code_003648C0", CallFuncSkill2);

INCLUDE_ASM(const s32, "code_003648C0", CallFuncSkill);

INCLUDE_ASM(const s32, "code_003648C0", cmpSetSavePanel);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallFuncOraganize);

INCLUDE_ASM(const s32, "code_003648C0", cmpCharSelectSetDefault);

INCLUDE_ASM(const s32, "code_003648C0", itfGeneralMenuReturnFunc);

INCLUDE_ASM(const s32, "code_003648C0", cmpSetupDvlSkl);

INCLUDE_ASM(const s32, "code_003648C0", DrawFuncSkl_sbtlCmndMenuNew);

INCLUDE_ASM(const s32, "code_003648C0", mapCallGetStockPID);

INCLUDE_ASM(const s32, "code_003648C0", mapMenuCreateMenuPID_itf);

void mapSetupCmpKudaSub(void) {
    sdatUnitWork_t *unit = NULL;

    if (cmpData41 >= 0) {
        unit = cmpRefDDS3GBWK2(cmpData41);
    }
    cmpSetFilterEx(unit, ecmpKudaSelectMode_ecmpKSM_Target, ecmpKudaCMU_Devil, ecmpKudaCMU_Devil);
}

INCLUDE_ASM(const s32, "code_003648C0", nmeDrawBox);

INCLUDE_ASM(const s32, "code_003648C0", nmeCallSetFilter);

INCLUDE_ASM(const s32, "code_003648C0", nmeSetupCmpKudaSub);

INCLUDE_ASM(const s32, "code_003648C0", nmeSetupCmpKudaSub2);

INCLUDE_ASM(const s32, "code_003648C0", Setup_cmpKuda);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetStockDevilNums);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetStockPID3);

INCLUDE_ASM(const s32, "code_003648C0", cmpUpdateFontCharPID);

INCLUDE_ASM(const s32, "code_003648C0", cmpChangeChkTabPage);

INCLUDE_ASM(const s32, "code_003648C0", cmpChangeTabPage);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetStockDevilNums2);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetStockDevilNums3);

INCLUDE_ASM(const s32, "code_003648C0", cmpUpdateSkillHD_cmpKuda);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetCsrSelSkl);

INCLUDE_ASM(const s32, "code_003648C0", cmpRefDDS3GBWK4);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetStockPID4);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallMenuGetFlag);

INCLUDE_ASM(const s32, "code_003648C0", cmpPartSetParticle);

INCLUDE_ASM(const s32, "code_003648C0", cmpDrawParticle);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetStockPID5);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetStockPID6);

INCLUDE_ASM(const s32, "code_003648C0", cmpUpdateSkillListHd);

INCLUDE_ASM(const s32, "code_003648C0", cmpChkSkillSelectHd);

void CmpOnMenu(u32 a_menu)
{
    CmpOnMenuData[a_menu >> 16]();
}

INCLUDE_ASM(const s32, "code_003648C0", cmpChangeChkTabPageSub);

INCLUDE_ASM(const s32, "code_003648C0", cmpChangeChkTabPageSub2);

INCLUDE_ASM(const s32, "code_003648C0", cmpSequenceSub);

INCLUDE_ASM(const s32, "code_003648C0", cmpStartBirthDevilStatusSub);

INCLUDE_ASM(const s32, "code_003648C0", cmpChkEnableUseUnit);

INCLUDE_ASM(const s32, "code_003648C0", cmpChkEnableUseChar);

INCLUDE_ASM(const s32, "code_003648C0", cmpGetNoneItemText);

INCLUDE_ASM(const s32, "code_003648C0", cmpChkUse);

INCLUDE_ASM(const s32, "code_003648C0", cmpUpdateUseItemEnebleDecide);

INCLUDE_ASM(const s32, "code_003648C0", cmpUpdateEnableUseCharUnitPC);

INCLUDE_ASM(const s32, "code_003648C0", cmpUpdateEnableUseCharUnit);

INCLUDE_ASM(const s32, "code_003648C0", cmpCheckUseCharEnableSelect);

INCLUDE_ASM(const s32, "code_003648C0", cmpUpdateSkillHD);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallFuncUse);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallFuncBullet);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallMenuGetSelectID2);

INCLUDE_ASM(const s32, "code_003648C0", cmpUpdateSword);

INCLUDE_ASM(const s32, "code_003648C0", cmpGetValueColor);

INCLUDE_ASM(const s32, "code_003648C0", cmpDrawSwordParam);

INCLUDE_ASM(const s32, "code_003648C0", DrawFunc);

INCLUDE_ASM(const s32, "code_003648C0", MakeList);

INCLUDE_ASM(const s32, "code_003648C0", Setup_cmpItem);

void FreeSpriteHnd(void) {
    if (cmpPanelEffList3 != NULL) {
        cmpFreeSpriteHnd(CMP_SPR_STATUS);
        cmpPanelEffList3 = NULL;
    }
}

INCLUDE_ASM(const s32, "code_003648C0", sbtlSetItemScript);

INCLUDE_ASM(const s32, "code_003648C0", awdCallMenuGetWaitType);

INCLUDE_ASM(const s32, "code_003648C0", cmpEndMesChk);

INCLUDE_ASM(const s32, "code_003648C0", cmpDebugLoadFlagOff);

INCLUDE_ASM(const s32, "code_003648C0", cmpUpdateSword_cmpItem);

INCLUDE_ASM(const s32, "code_003648C0", cmpUpdateJewel);

INCLUDE_ASM(const s32, "code_003648C0", cmpUpdateMaterial);

INCLUDE_ASM(const s32, "code_003648C0", Update_cmpItem);

void cmpLoadScript(void) {
    cmpFreeScriptData = sdfDevLoadFileEx(cmpStrCampScriptCmpUseScrBf, &effScalyTextureHandle2, NULL);
}

INCLUDE_ASM(const s32, "code_003648C0", cmpFreeScript);

void cmpStart(void) {
}

void End_cmpItem(void) {
    FreeSpriteHnd();
    if (hWorkArea3 != NULL) {
        dds3KillProcess(hWorkArea3, 0);
        hWorkArea3 = NULL;
    }
}

INCLUDE_ASM(const s32, "code_003648C0", cmpChkMessageDraw);

s32 cmpGetItemIDFromSortNo(s32 a_sortNo) {
    s32 id = -1;

    if ((u32)a_sortNo < 443) {
        id = cmpData30[a_sortNo];
    }
    return id;
}

INCLUDE_ASM(const s32, "code_003648C0", fldCommand_FLD_WARP_RUN_START);

INCLUDE_ASM(const s32, "code_003648C0", cmpDrawFunc);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetCharPID);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallMakeCateChk);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallMenuGetWaitType);

/* NON_MATCHING: 5 of 16 words match.  */
#ifdef NON_MATCHING
s32 cmpUpdateKeyWord(void) {
    s32 ret = 0;

    if (cmpData46 == 0) {
        if (itfMenuGetWaitType(cmpCallGetCharPIDData3) == ITFMENU_WAIT_TYPE_CANCEL) {
            ret = 1;
        } else {
            ret = 0;
        }
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "code_003648C0", cmpUpdateKeyWord);
#endif

INCLUDE_ASM(const s32, "code_003648C0", cmpCallMenuGetWaitType3);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallMenuGetWaitType4);

INCLUDE_ASM(const s32, "code_003648C0", cmpLeaf15);

INCLUDE_ASM(const s32, "code_003648C0", cmpLeaf16);

INCLUDE_ASM(const s32, "code_003648C0", cmpRefDDS3GBWK5);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallGetItemNum);

INCLUDE_ASM(const s32, "code_003648C0", cmpRefDDS3GBWK6);

INCLUDE_ASM(const s32, "code_003648C0", cmpFunc);

INCLUDE_ASM(const s32, "code_003648C0", cmpCallSetIntReturnValue);

s32 cmpCallBitCheck(s32 a_no) {
    if ((u32)a_no < 64 && evtBitCheck(cmpCallBitCheckData[a_no])) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_003648C0", cmpReadFlgChk);

INCLUDE_ASM(const s32, "code_003648C0", cmpReadFlgOn);

INCLUDE_ASM(const s32, "code_003648C0", cmpMakeCateChk);

INCLUDE_ASM(const s32, "code_003648C0", cmpMakekeywordctlist);

INCLUDE_ASM(const s32, "code_003648C0", cmpMakekeywordlist);

INCLUDE_ASM(const s32, "code_003648C0", cmpMenuDecideFunc2);

INCLUDE_ASM(const s32, "code_003648C0", cmpMenuDecideFunc);

INCLUDE_ASM(const s32, "code_003648C0", cmpListDrawFunc);

INCLUDE_ASM(const s32, "code_003648C0", cmpMenuDrawFunc);

INCLUDE_ASM(const s32, "code_003648C0", cmpMenuSetMenuImage2);

INCLUDE_ASM(const s32, "code_003648C0", cmpMenuSetMenuImage);

INCLUDE_ASM(const s32, "code_003648C0", cmpMenuCreateListPID);

INCLUDE_ASM(const s32, "code_003648C0", cmpMenuCreateMenuPID);

INCLUDE_ASM(const s32, "code_003648C0", cmpMenuCallProcEnd);

INCLUDE_ASM(const s32, "code_003648C0", cmpMenuCheckProcEnd);

INCLUDE_ASM(const s32, "code_003648C0", cmpMenuProcess);

INCLUDE_ASM(const s32, "code_003648C0", cmpMenuCheckWait);

INCLUDE_ASM(const s32, "code_003648C0", cmpMenuStart);

INCLUDE_ASM(const s32, "code_003648C0", cmpMenuGetSelectID);

INCLUDE_ASM(const s32, "code_003648C0", cmpMenuRelease);

INCLUDE_ASM(const s32, "code_003648C0", cmpMenuShutdown);

INCLUDE_ASM(const s32, "code_003648C0", cmpProcStart);

INCLUDE_ASM(const s32, "code_003648C0", cmpMenuProcessStart);

INCLUDE_ASM(const s32, "code_003648C0", cmpMenuProcessEnd);

INCLUDE_ASM(const s32, "code_003648C0", cmpMenuProcessCheck);

INCLUDE_ASM(const s32, "code_003648C0", Update_cmpKeyword);

INCLUDE_ASM(const s32, "code_003648C0", Process_cmpMap);

void cmpCallSetOt2(s32 x, s32 y, s32 frame) {
    u32 color[4];
    s32 px;

    eglSetOt(44);
    eglLinkTest(0, 0, 0, 0, 0, 0, 1, 2);
    eglLinkAlpha(0);
    color[0] = 0x80182328;
    color[1] = 0x80282A37;
    color[2] = 0x80282A37;
    color[3] = 0x80182328;
    if (frame < 70) {
        px = x + (s32)((f32)frame * 108.0f / 70.0f);
        eglLinkBoxZ(px - 108, y, 108, 173, 0, color[1], color[0], color[3], color[2]);
        eglLinkBoxZ(px, y, 108, 173, 0, color[0], color[1], color[2], color[3]);
        eglLinkBoxZ(px + 108, y, 108, 173, 0, color[1], color[0], color[3], color[2]);
    } else {
        px = x + (s32)((f32)(frame - 70) * 108.0f / 70.0f);
        eglLinkBoxZ(px - 108, y, 108, 173, 0, color[0], color[1], color[2], color[3]);
        eglLinkBoxZ(px, y, 108, 173, 0, color[1], color[0], color[3], color[2]);
        eglLinkBoxZ(px + 108, y, 108, 173, 0, color[0], color[1], color[2], color[3]);
    }
}

void cmpDrawBack(tagCMP_MAP_WORK *wk) {
    if (cmpGetSpriteHnd(CMP_SPR_MAP) == NULL) {
        return;
    }
    eglSetOt(44);
    eglLinkTest(1, 2, 128, 0, 0, 0, 1, 1);
    eglLinkAlpha(0);
    eglLinkBoxZ(0, 0, 512, 58, 4095, 0, 0, 0, 0);
    eglLinkBoxZ(0, 405, 512, 43, 4095, 0, 0, 0, 0);
    eglLinkBoxZ(0, 58, 22, 347, 4095, 0, 0, 0, 0);
    cmpCallSetOt2(-39, 4, cmpDrawBackData);
    cmpCallSetOt2(311, -53, cmpDrawBackData);
    cmpCallSetOt2(222, 284, cmpDrawBackData);
    cmpDrawBackData++;
    if (cmpDrawBackData >= 141) {
        cmpDrawBackData = 0;
    }
    eglSetOt(44);
    eglLinkTest(1, 2, 128, 0, 0, 0, 1, 1);
    eglLinkAlpha(0);
    itfDrawGridWithResolvedSlot(-624, 32, 4095, 96, cmpGetSpriteHnd(CMP_SPR_MAP), 0, 44);
    itfDrawGridWithResolvedSlot(4976, -424, 4095, 96, cmpGetSpriteHnd(CMP_SPR_MAP), 32, 44);
    itfDrawGridWithResolvedSlot(3552, 2272, 4095, 96, cmpGetSpriteHnd(CMP_SPR_MAP), 0, 44);
    eglSetOt(44);
    eglLinkTest(1, 0, 128, 1, 0, 0, 1, 2);
    eglLinkAlpha(0);
    itfDrawGridWithResolvedSlot(320, 448, 0, 96, cmpGetSpriteHnd(CMP_SPR_MAP), 1, 44);
}

INCLUDE_RODATA(const s32, "code_003648C0", cmpStrCampScriptCmpUseScrBf);

