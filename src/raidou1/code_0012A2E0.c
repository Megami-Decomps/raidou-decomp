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

/* NON_MATCHING: 510 of 511 words match.  */
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
    s32 status;
    s32 stock;

    fldGlobalBase.wordDC = 0;
    SDF_VU0_LOADVECTOR(&axis);
    FromRotation(3.1415926f);
    SDF_VU0_LOADVECTOR_B(&rot);
    Mult();
    SDF_VU0_STOREVECTOR(&rot);
    if (playerBasic == NULL) {
        playerBasic = dds3AddUnitObjectBasic(dds3AllocObjectId(), &pos, &rot);
        dds3SetUnitObjectId(playerBasic, "PLAYER_UNIT");
        dds3SetActiveUnitSceneBasic(dds3GetControlScene(), playerBasic);
        if (evtBitCheck(161) && !evtBitCheck(163)) {
            if (fldGlobalBase.amSBlock == 3 && fldGlobalBase.nameU == 2) {
                fldGlobalBase.wordE0 = 3;
                minor = 7;
            } else {
                fldGlobalBase.wordE0 = 2;
                minor = 6;
            }
        } else if (fldGlobalBase.amSBlock == 3 && fldGlobalBase.nameU == 2) {
            fldGlobalBase.wordE0 = 1;
            minor = 8;
        } else {
            minor = 0;
            fldGlobalBase.wordE0 = 0;
        }
        fldJDataLoad();
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
    status = sdatCalc_GetBadStatus(&DDS3_GBWK->playerData.unit);
    switch (status) {
    case 6:
        mapBadEffLoad(0, 6);
        break;
    case 11:
        mapBadEffLoad(0, 11);
        break;
    }
    aiboBasic = NULL;
    if (evtBitCheck(42) && !evtBitCheck(202)) {
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
        visual = sbtlGetVisualData(1, DDS3_GBWK->mapSummonID);
        scl.x = visual->scale;
        scl.y = visual->scale;
        scl.z = visual->scale;
        dds3SetPosPoseSet(summonDvBasic, &pos);
        dds3SetRotPoseSet(summonDvBasic, &rot);
        dds3SetSclPoseSet(summonDvBasic, &scl);
        summonDvModel = dds3ObjectBaseGetModelHandle(summonDvBasic);
        if (visual->field2C & 4) {
            dds3GetRadiusPoseSet(summonDvBasic, 50.0f);
        } else if (visual->field2C & 8) {
            dds3GetRadiusPoseSet(summonDvBasic, 100.0f);
        } else if (visual->field2C & 0x10) {
            dds3GetRadiusPoseSet(summonDvBasic, 150.0f);
        } else if (visual->field2C & 0x20) {
            dds3GetRadiusPoseSet(summonDvBasic, 200.0f);
        } else {
            dds3GetRadiusPoseSet(summonDvBasic, 100.0f);
        }
        motion = sbtlGetMotionVisualData(1, DDS3_GBWK->mapSummonID, 0);
        if (motion->flags & 1) {
            speed = motion->field10;
        } else {
            speed = sbtlGetVisualData(1, fldGlobalBase.word16C)->field0C;
        }
        summonDvModel->first->speed = speed;
        mdlLoopMotion(summonDvModel, 0, 0);
        dds3ObjectBaseOnFlag(summonDvBasic, 0x400);
        dds3ObjectBaseOnFlag(summonDvBasic, 0x40000);
        dds3ObjectBaseOnFlag(summonDvBasic, 0x800);
        mapNakamaCashLoadEndCheckSub(DDS3_GBWK->mapSummonID);
        while (!sndCallLoadCheckMidi()) {
        }
        stock = datCallGetStockDevilID(DDS3_GBWK->mapSummonID);
        if (stock != -1) {
            status = sdatCalc_GetBadStatus(&DDS3_GBWK->playerData.stock[stock]);
            switch (status) {
            case 6:
                mapBadEffLoad(1, 6);
                break;
            case 11:
                mapBadEffLoad(1, 11);
                break;
            }
        }
    }
    if (fldGlobalBase.nakamaMode != 0) {
        fldGlobalBase.nakamaMode = 1;
        fldGlobalBase.word16C = DDS3_GBWK->mapSummonID;
        fldGlobalBase.word170 = 5;
        fldGlobalBase.word174 = 5;
        evtBitOn(2993);
        fldPlayerToNakama(0);
        mdlSetMotionSpeed(5);
    }
    dds3AddPointBasic(dds3AllocObjectId(), &ppos01Point, "ppos01");
}
#else
INCLUDE_RODATA(const s32, "code_0012A2E0", fldSetUnitTable);

INCLUDE_RODATA(const s32, "code_0012A2E0", fldSetUnitTable2);

INCLUDE_ASM(const s32, "code_0012A2E0", fldSetUnit);
#endif

INCLUDE_ASM(const s32, "code_0012A2E0", fldPlayerToNakama);

INCLUDE_ASM(const s32, "code_0012A2E0", fldCallObjectBaseOnFlag);

INCLUDE_ASM(const s32, "code_0012A2E0", fldCallDeleteSceneBasicData);

INCLUDE_ASM(const s32, "code_0012A2E0", fldCallGetRotPoseSet);

INCLUDE_RODATA(const s32, "code_0012A2E0", fldPlayerToNakamaTable);

