#include "common.h"
#include "basic.h"
#include "dds3Basic.h"
#include "dds3obj.h"
#include "dds3ObjectBase.h"
#include "dds3UnitObjectBasic.h"
#include "dds3SceneBasic.h"
#include "dds3WorldBasic.h"
#include "dds3PointBasic.h"
#include "dds3KernelMain.h"
#include "field.h"
#include "fld.h"
#include "fldProcess.h"
#include "fldFileResolver.h"
#include "code_r1_00128378.h"
#include "code_0015D490.h"
#include "EventBit.h"
#include "mdlManager.h"
#include "model.h"
#include "newdata.h"
#include "dat.h"
#include "sbtlWork.h"
#include "sdfQuat.h"
#include "sdfSystem.h"
#include "sdk_libc.h"
#include "ps2asm.h"
#include "fclCombineInit.h"

INCLUDE_RODATA(const s32, "code_0012ECC0", fldSetUnitTable);

INCLUDE_RODATA(const s32, "code_0012ECC0", fldSetUnitTable2);

INCLUDE_RODATA(const s32, "code_0012ECC0", fldStrPlayerUnit);

/* NON_MATCHING: 504 of 505 words match.  */
#ifdef NON_MATCHING
void fldSetUnit(void)
{
    sdfVector4_t rot = { 0.0f, 0.0f, 0.0f, 1.0f };
    sdfVector4_t axis = { 0.0f, 1.0f, 0.0f, 1.0f };
    sdfVector4_t scl = { 1.0f, 1.0f, 1.0f, 1.0f };
    sdfVector4_t pos = { 0.0f, 0.0f, 0.0f, 1.0f };
    dds3PoseSet_t *poseset;
    SBTL_MOTION_VISUAL *visual;
    SBTL_UNIT_SUB *motion;
    f32 speed;
    s32 minor;
    s32 stock;

    fldGlobalBase.wordDC = 0;
    SDF_VU0_LOADVECTOR(&axis);
    FromRotation(3.1415926f);
    SDF_VU0_LOADVECTOR_B(&rot);
    Mult();
    SDF_VU0_STOREVECTOR(&rot);
    if (playerBasic == NULL) {
        minor = 0;
        playerBasic = dds3AddUnitObjectBasic(dds3AllocObjectId(), &pos, &rot);
        dds3SetUnitObjectId(playerBasic, "PLAYER_UNIT");
        dds3SetActiveUnitSceneBasic(dds3GetControlScene(), playerBasic);
        if (tipsProcessCheck()) {
            fldGlobalBase.wordE0 = 1;
            minor = 8;
        } else {
            fldGlobalBase.wordE0 = 0;
        }
        fldCallPlayerALoad();
        dds3ObjectBaseMakeUnit(playerBasic, MODELTYPE_FIELD, minor);
    } else {
        poseset = playerBasic->poseset;
        SDF_VU0_COPYVECTOR_NOMEM(&poseset->pos, &pos);
        SDF_VU0_COPYVECTOR_NOMEM(&poseset->pos2, &pos);
        SDF_VU0_COPYVECTOR_NOMEM(&poseset->rot, &rot);
    }
    dds3SetPosPoseSet(playerBasic, &pos);
    dds3SetRotPoseSet(playerBasic, &rot);
    dds3SetSclPoseSet(playerBasic, &scl);
    playerModel = dds3ObjectBaseGetModelHandle(playerBasic);
    dds3GetRadiusPoseSet(playerBasic, 50.0f);
    if (fldGlobalBase.nakamaMode == 0) {
        fldSetUnitSub3(0, 0.5f);
    } else {
        fldSetUnitSub3(19, 0.5f);
    }
    dds3ObjectBaseOnFlag(playerBasic, 0x400);
    dds3ObjectBaseOnFlag(playerBasic, 0x80000);
    dds3ObjectBaseOnFlag(playerBasic, 0x800);
    if (sdatCalc_GetBadStatus(&DDS3_GBWK->playerData.unit) == 6) {
        mapCampBADEPL(0, 6);
    }
    aiboBasic = NULL;
    if (!(fldGlobalBase.amSBlock == 1 && (fldGlobalBase.nameU == 1 || fldGlobalBase.nameU == 3 || fldGlobalBase.nameU == 4))
        && !(fldGlobalBase.amSBlock == 19 && fldGlobalBase.nameU == 1)
        && !(fldGlobalBase.amSBlock == 33 && fldGlobalBase.nameU == 15)
        && !(fldGlobalBase.amSBlock == 36 && fldGlobalBase.nameU == 3 && !evtBitCheck(6022) && evtBitCheck(1229))) {
        SDF_VU0_LOADVECTOR(&axis);
        FromRotation(3.1415926f);
        SDF_VU0_LOADVECTOR_B(&rot);
        Mult();
        SDF_VU0_STOREVECTOR(&rot);
        aiboBasic = dds3AddUnitObjectBasic(dds3AllocObjectId(), &pos, &rot);
        dds3SetUnitObjectId(aiboBasic, "AIBO_UNIT");
        dds3SetActiveUnitSceneBasic(dds3GetControlScene(), aiboBasic);
        if (mdlSearchResrc(MODELTYPE_EVENT, 110) == NULL) {
            if (fldSetUnitSub2()) {
                fldCallPackDecode();
            } else {
                fldAiboCashLoad_Block();
            }
        } else if (fldSetUnitSub2()) {
            fldCallMemFreeDelayedDirect3();
        }
        dds3ObjectBaseMakeUnit(aiboBasic, MODELTYPE_EVENT, 110);
        dds3SetPosPoseSet(aiboBasic, &pos);
        dds3SetRotPoseSet(aiboBasic, &rot);
        dds3SetSclPoseSet(aiboBasic, &scl);
        aiboModel = dds3ObjectBaseGetModelHandle(aiboBasic);
        dds3GetRadiusPoseSet(aiboBasic, 20.0f);
        mdlLoopMotion(aiboModel, 0, 2);
        dds3ObjectBaseOnFlag(aiboBasic, 0x400);
        dds3ObjectBaseOnFlag(aiboBasic, 0x40000);
        fldGlobalBase.aiboRunStop = 0;
    }
    mapNakamaStockOnCheck_fld();
    summonDvBasic = NULL;
    if (DDS3_GBWK->mapSummonID > 0 && fldGlobalBase.nakamaTaskOff == 0) {
        SDF_VU0_LOADVECTOR(&axis);
        FromRotation(3.1415926f);
        SDF_VU0_LOADVECTOR_B(&rot);
        Mult();
        SDF_VU0_STOREVECTOR(&rot);
        summonDvBasic = dds3AddUnitObjectBasic(dds3AllocObjectId(), &pos, &rot);
        dds3SetUnitObjectId(summonDvBasic, "SMNDV_UNIT");
        dds3SetActiveUnitSceneBasic(dds3GetControlScene(), summonDvBasic);
        sdfGetRunTimeCountBase();
        dds3GetKernelCount();
        if (mdlSearchResrc(MODELTYPE_DEVIL, DDS3_GBWK->mapSummonID) == NULL) {
            if (fldSetUnitSub()) {
                nbResultFreeTextures();
            } else {
                fldNakamaCashLoad_Block();
            }
        } else if (fldSetUnitSub()) {
            fldCallMemFreeDelayedDirect2();
        }
        dds3ObjectBaseMakeUnit(summonDvBasic, MODELTYPE_DEVIL, DDS3_GBWK->mapSummonID);
        visual = sbtlGetMotionVisualData(1, DDS3_GBWK->mapSummonID);
        scl.x = visual->scale;
        scl.y = visual->scale;
        scl.z = visual->scale;
        dds3SetPosPoseSet(summonDvBasic, &pos);
        dds3SetRotPoseSet(summonDvBasic, &rot);
        dds3SetSclPoseSet(summonDvBasic, &scl);
        summonDvModel = dds3ObjectBaseGetModelHandle(summonDvBasic);
        evtBitOn(2996);
        if (visual->field00 & 4) {
            dds3GetRadiusPoseSet(summonDvBasic, 50.0f);
        } else if (visual->field00 & 8) {
            dds3GetRadiusPoseSet(summonDvBasic, 100.0f);
        } else if (visual->field00 & 0x10) {
            dds3GetRadiusPoseSet(summonDvBasic, 150.0f);
        } else if (visual->field00 & 0x20) {
            dds3GetRadiusPoseSet(summonDvBasic, 200.0f);
        } else {
            dds3GetRadiusPoseSet(summonDvBasic, 100.0f);
        }
        motion = sbtlGetMotionVisualData_sbtlVisual(1, DDS3_GBWK->mapSummonID, 0);
        if (motion->flags & 1) {
            speed = motion->field10;
        } else {
            speed = sbtlGetMotionVisualData(1, fldGlobalBase.word198)->field0C;
        }
        summonDvModel->first->speed = speed;
        mdlLoopMotion(summonDvModel, 0, 0);
        dds3ObjectBaseOnFlag(summonDvBasic, 0x400);
        dds3ObjectBaseOnFlag(summonDvBasic, 0x40000);
        dds3ObjectBaseOnFlag(summonDvBasic, 0x800);
        mapNakamaCashLoadEndCheckSub(DDS3_GBWK->mapSummonID);
        while (!mapSoundNakamaSmgLoadhk()) {
        }
        stock = datGetStockDevilIndex(DDS3_GBWK->mapSummonID);
        if (stock != -1 && sdatCalc_GetBadStatus(&DDS3_GBWK->playerData.stock[stock]) == 6) {
            mapCampBADEPL(1, 6);
        }
    }
    if (fldGlobalBase.nakamaMode != 0) {
        fldGlobalBase.nakamaMode = 1;
        fldGlobalBase.word198 = DDS3_GBWK->mapSummonID;
        fldGlobalBase.word19C = 5;
        fldGlobalBase.word1A0 = 5;
        evtBitOn(2993);
        fldPlayerToNakama(0);
        mdlSetMotionSpeed(5);
    }
    if (fldGlobalBase.word204 != 0) {
        fldCallObjectBaseOnFlag2(0);
    }
    if (fldGlobalBase.word1A8 != 0) {
        fldGITAIUNIT(0);
    }
    fclCallBitOff();
    nbData21 = dds3AllocObjectId();
    dds3AddPointBasic(nbData21, &ppos01Point, "ppos01");
}
#else
INCLUDE_ASM(const s32, "code_0012ECC0", fldSetUnit);
#endif

INCLUDE_ASM(const s32, "code_0012ECC0", fldPlayerToNakama);

INCLUDE_ASM(const s32, "code_0012ECC0", fldCallObjectBaseOnFlag);

INCLUDE_ASM(const s32, "code_0012ECC0", fldBgmStop);

INCLUDE_ASM(const s32, "code_0012ECC0", fldNakamaToSummon);

INCLUDE_ASM(const s32, "code_0012ECC0", fldCallObjectBaseOnFlag2);

INCLUDE_ASM(const s32, "code_0012ECC0", fldCallGetRotPoseSet);

INCLUDE_RODATA(const s32, "code_0012ECC0", fldPlayerToNakamaTable);

INCLUDE_RODATA(const s32, "code_0012ECC0", fldCallObjectBaseOnFlag2Table);

INCLUDE_RODATA(const s32, "code_0012ECC0", fldGITAIUNITTable);

INCLUDE_ASM(const s32, "code_0012ECC0", fldGITAIUNIT);

INCLUDE_ASM(const s32, "code_0012ECC0", fldPLAYERUNIT);

INCLUDE_RODATA(const s32, "code_0012ECC0", fldPLAYERUNITTable);

