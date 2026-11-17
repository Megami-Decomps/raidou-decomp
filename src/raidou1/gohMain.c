#include "common.h"
#include "kernel.h"
#include "dds3KernelCore.h"
#include "code_r1_001CC1D0.h"
#include "gohMain.h"
#include "fclMisc.h"
#include "gohDraw.h"
#include "jmsMain.h"
#include "newdata.h"
#include "itf.h"

INCLUDE_ASM(const s32, "gohMain", gohCallGetProcessData5);

INCLUDE_ASM(const s32, "gohMain", gohCallGetProcessData6);

dds3ProcessFunc_t gohCallGetProcessData24(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "gohMain", gohCallEndMessageEx);

INCLUDE_ASM(const s32, "gohMain", gohCallCallDrawBack2);

INCLUDE_ASM(const s32, "gohMain", gohCallGetProcessData7);

INCLUDE_ASM(const s32, "gohMain", gohCallGetProcessData8);

dds3ProcessFunc_t gohSwordTreeUpdateProcess(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "gohMain", gohCallSeqJumpAndFree);

s32 gohStatInit(dds3ProcessID_t *PID)
{
    ((gohStatWork_t *)dds3GetProcessData(PID))->unk20330 = 0;
    return 1;
}

INCLUDE_ASM(const s32, "gohMain", gohCmbSearchiCalcProcess);

INCLUDE_ASM(const s32, "gohMain", fldFastTravelSetup);

INCLUDE_ASM(const s32, "gohMain", gohCallGetProcessData9);

dds3ProcessFunc_t gohCallGetProcessData25(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t gohCallGetProcessData26(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "gohMain", gohLeaf25);

INCLUDE_ASM(const s32, "gohMain", gohCallGetProcessData10);

INCLUDE_ASM(const s32, "gohMain", gohLeaf26);

INCLUDE_ASM(const s32, "gohMain", gohCallGetProcessData11);

dds3ProcessFunc_t gohCallGetProcessData27(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t gohCallGetProcessData28(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    fclUpdateMessage();
    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "gohMain", gohCyuuseiStartDirection);

INCLUDE_ASM(const s32, "gohMain", gohCallStrongSwordFlag);

INCLUDE_ASM(const s32, "gohMain", gohCallGetProcessData12);

INCLUDE_ASM(const s32, "gohMain", gohLeaf27);

INCLUDE_ASM(const s32, "gohMain", gohCallGetProcessData13);

dds3ProcessFunc_t gohCyuuseiDrawProcess(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t gohAccidentCalcProcess(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    fclUpdateMessage();
    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "gohMain", TestProcess2);

INCLUDE_ASM(const s32, "gohMain", gohCallCallDrawBack3);

INCLUDE_ASM(const s32, "gohMain", scrLogStart);

INCLUDE_ASM(const s32, "gohMain", gohRecoverCalcProcess);

INCLUDE_ASM(const s32, "gohMain", gohRecoverDrawProcess);

dds3ProcessFunc_t gohRecoverUpdateProcess(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "gohMain", gohTalkInit);

INCLUDE_ASM(const s32, "gohMain", gohTalkDestroy);

INCLUDE_ASM(const s32, "gohMain", gohTalkCalcProcess);

dds3ProcessFunc_t gohCallGetProcessData29(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    gohModel(work);
    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t gohCallGetProcessData30(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "gohMain", gohMiscInit);

INCLUDE_ASM(const s32, "gohMain", gohLeaf28);

s32 gohCallGetProcessData31(dds3ProcessID_t *PID)
{
    jmsSeqWork_t *work = dds3GetProcessData(PID);
    s32 result = menuRunPanel(&work->panel, &work->seqUnit, 0, PID);

    if (result == 0) {
        if (ScrControl(PID) == 0) {
            jmsSeqJumpAndFree(&work->seqUnit, &gohData5);
        }
        return 0;
    }
    return result;
}

dds3ProcessFunc_t gohCallGetProcessData32(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    gohModel(work);
    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t gohCallGetProcessData33(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "gohMain", gohCallGetProcessData34);

INCLUDE_ASM(const s32, "gohMain", gohLeaf29);

INCLUDE_ASM(const s32, "gohMain", gohSinanCalcProcess);

dds3ProcessFunc_t gohCallGetProcessData35(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    gohModel(work);
    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t gohCallGetProcessData36(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    fclUpdateMessage();
    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "gohMain", gohCallGetProcessData37);

INCLUDE_ASM(const s32, "gohMain", gohLeaf30);

INCLUDE_ASM(const s32, "gohMain", gohSearchiErrorCalcProcess);

INCLUDE_ASM(const s32, "gohMain", evtRefreshDispatchStart);

dds3ProcessFunc_t gohCallGetProcessData38(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    fclUpdateMessage();
    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "gohMain", EndProcess_gohMain);

INCLUDE_ASM(const s32, "gohMain", gohStrongSwordFlag);

INCLUDE_ASM(const s32, "gohMain", gohTablCheckEventDevil);

INCLUDE_ASM(const s32, "gohMain", gohSortedSkillPrioHigherCallback);

INCLUDE_ASM(const s32, "gohMain", gohSortedSkillPrioLowerCallback);

INCLUDE_ASM(const s32, "gohMain", gohSortedDevilListCallback);

INCLUDE_ASM(const s32, "gohMain", gohCreateSortedDevilList);

INCLUDE_ASM(const s32, "gohMain", gohFunc16);

INCLUDE_ASM(const s32, "gohMain", gohFunc17);

INCLUDE_ASM(const s32, "gohMain", sdfBuildFrameRegisterReference);

INCLUDE_ASM(const s32, "gohMain", gohCallGetAgeOfMoon16);

INCLUDE_ASM(const s32, "gohMain", gohCallGetStockDevilID);

INCLUDE_ASM(const s32, "gohMain", gohCallMemset);

INCLUDE_ASM(const s32, "gohMain", gohCallMemset2);

INCLUDE_ASM(const s32, "gohMain", gohCallBitCheck2);

INCLUDE_ASM(const s32, "gohMain", gohLeaf);

INCLUDE_ASM(const s32, "gohMain", gohCallGetRandA);

INCLUDE_ASM(const s32, "gohMain", gohLeaf2);

INCLUDE_ASM(const s32, "gohMain", gohLeaf3);

INCLUDE_ASM(const s32, "gohMain", gohLeaf4);

INCLUDE_ASM(const s32, "gohMain", gohCallTablCheckEventDevil);

INCLUDE_ASM(const s32, "gohMain", gohCallGetAgeOfMoon162);

INCLUDE_ASM(const s32, "gohMain", gohLeaf5);

INCLUDE_ASM(const s32, "gohMain", gohLeaf6);

INCLUDE_ASM(const s32, "gohMain", gohLeaf7);

INCLUDE_ASM(const s32, "gohMain", effPCPDANTE_Damage_Delete);

void gohLeaf8(gohData_t *pData)
{
    s32 i = 9;
    u32 *p = &pData->unk90[9];

    do {
        i--;
        *p = 0;
        p--;
    } while (i >= 0);
}

INCLUDE_ASM(const s32, "gohMain", gohLeaf31);

INCLUDE_ASM(const s32, "gohMain", gohFunc3);

INCLUDE_ASM(const s32, "gohMain", gohFunc4);

INCLUDE_ASM(const s32, "gohMain", gohFunc5);

INCLUDE_ASM(const s32, "gohMain", gohLeaf9);

INCLUDE_ASM(const s32, "gohMain", gohFunc6);

INCLUDE_ASM(const s32, "gohMain", gohCallPCPDANTEDamageDelete);

INCLUDE_ASM(const s32, "gohMain", gohCallCheckSkillEnable);

INCLUDE_ASM(const s32, "gohMain", gohCallCheckSkillEnable2);

INCLUDE_ASM(const s32, "gohMain", gohFunc7);

INCLUDE_ASM(const s32, "gohMain", gohLeaf10);

INCLUDE_ASM(const s32, "gohMain", gohCallCreateDevilUnitEx);

INCLUDE_ASM(const s32, "gohMain", gohLeaf11);

INCLUDE_ASM(const s32, "gohMain", gohCallGetRandA2);

INCLUDE_ASM(const s32, "gohMain", gohCallGetRandA3);

INCLUDE_ASM(const s32, "gohMain", gohLogStartSub);

INCLUDE_ASM(const s32, "gohMain", gohCallMemset3);

INCLUDE_ASM(const s32, "gohMain", gohCallCreateDevilUnit);

INCLUDE_ASM(const s32, "gohMain", gohCallCheckSameDevil);

INCLUDE_ASM(const s32, "gohMain", gohFunc8);

INCLUDE_ASM(const s32, "gohMain", gohRefDDS3GBWK5);

INCLUDE_ASM(const s32, "gohMain", gohFunc9);

INCLUDE_ASM(const s32, "gohMain", gohCallCheckSameDevil2);

INCLUDE_ASM(const s32, "gohMain", gohCommandDEVILOUTAFTERSub);

INCLUDE_ASM(const s32, "gohMain", gohCallBitCheck3);

INCLUDE_ASM(const s32, "gohMain", cmpReturnParamDH);

INCLUDE_ASM(const s32, "gohMain", gohKarteRestoreAll);

INCLUDE_ASM(const s32, "gohMain", gohCreateRecoverUnit);

INCLUDE_ASM(const s32, "gohMain", gohSelDevilNameSub);

INCLUDE_ASM(const s32, "gohMain", gohCaption);

INCLUDE_ASM(const s32, "gohMain", gohMagCost);

INCLUDE_ASM(const s32, "gohMain", gohSelDevilName);

/* body: matched in raidou @ 88f0d54: src/raidou1/code_0018CE10.c:206, near, same function 1.00 (words 39/39, calls 1.00, shape 1.00) */
void gohCalcWidth(struct sprHandle_t *a_hSpr, s32 a_sprNo, s32 a_num, s32 a_max)
{
    f32 width;
    f32 w;

    if (a_max <= 0) {
        return;
    }
    width = a_hSpr->workEntries[a_sprNo].sourceWidth;
    w = width * a_num / a_max;
    a_hSpr->workEntries[a_sprNo].geometry.bounds[2] = itfCalcSpriteWidthSub(w, 4);
    a_hSpr->workEntries[a_sprNo].parameters[2] = w - width;
}

INCLUDE_ASM(const s32, "gohMain", gohCheckEnableCombine);

INCLUDE_ASM(const s32, "gohMain", gohDevilParamSimple);

INCLUDE_ASM(const s32, "gohMain", gohResultIcon);

INCLUDE_ASM(const s32, "gohMain", gohCallGetSpriteHandle);

INCLUDE_ASM(const s32, "gohMain", gohCallGetSpriteHandle2);

INCLUDE_ASM(const s32, "gohMain", gohCallGetSpriteHandle3);

INCLUDE_ASM(const s32, "gohMain", gohCallGetSpriteHandle4);

INCLUDE_ASM(const s32, "gohMain", gohCombineListFrameNormalA);

INCLUDE_ASM(const s32, "gohMain", gohCallGetSpriteHandle5);

INCLUDE_ASM(const s32, "gohMain", gohCallGetSpriteHandle6);

INCLUDE_ASM(const s32, "gohMain", gohCallSetOt);

INCLUDE_ASM(const s32, "gohMain", gohCallGetSpriteHandle7);

INCLUDE_ASM(const s32, "gohMain", gohCallGetSpriteHandle8);

INCLUDE_ASM(const s32, "gohMain", gohCallGetSpriteHandle9);

INCLUDE_ASM(const s32, "gohMain", gohCallGetSpriteHandle10);

INCLUDE_ASM(const s32, "gohMain", gohCallGetSpriteHandle11);

INCLUDE_ASM(const s32, "gohMain", gohCallGetSpriteHandle12);

INCLUDE_ASM(const s32, "gohMain", gohCallGetSpriteHandle13);

INCLUDE_ASM(const s32, "gohMain", gohModel);

s32 gohRefDDS3GBWK6(void *pDevil)
{
    return gohCommandDEVILOUTAFTERSub(pDevil, &DDS3_GBWK->unk48F0, 1);
}

INCLUDE_ASM(const s32, "gohMain", gohRefDDS3GBWK7);

INCLUDE_ASM(const s32, "gohMain", gohCallSetDrawBack);

INCLUDE_ASM(const s32, "gohMain", gohCallSetDrawBack2);

INCLUDE_ASM(const s32, "gohMain", gohCmbSelInit);

INCLUDE_ASM(const s32, "gohMain", gohCallGetProcessData39);

INCLUDE_ASM(const s32, "gohMain", gohCmbSelDestroy);

INCLUDE_ASM(const s32, "gohMain", gohCmbSelCalcProcess);

dds3ProcessFunc_t gohCallGetProcessData40(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    gohModel(work);
    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t gohCallGetProcessData41(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "gohMain", gohCallEndMessage);

INCLUDE_ASM(const s32, "gohMain", gohLeaf32);

INCLUDE_ASM(const s32, "gohMain", gohCallGetProcessData42);

INCLUDE_ASM(const s32, "gohMain", gohCallGetProcessData43);

dds3ProcessFunc_t gohCallGetProcessData44(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    gohCallChkCommonInput(work->unkCC);
    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_RODATA(const s32, "gohMain", gohLeafTable);

