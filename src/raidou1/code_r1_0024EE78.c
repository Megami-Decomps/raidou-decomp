#include "common.h"
#include "evtStage.h"
#include "mapData.h"
#include "sndAt3.h"
#include "dds3Basic.h"
#include "dds3WorldBasic.h"
#include "dds3SceneBasic.h"
#include "dds3CameraObjectBasic.h"
#include "dds3ObjectBase.h"
#include "sbtlWork.h"
#include "sdk_libc.h"
#include "ps2asm.h"
#include "dds3KernelCore.h"
#include "fclMisc.h"
#include "rstInit.h"
#include "sBtlUnit.h"
#include "fld.h"
#include "sdfSound.h"
#include "dds3KernelDraw.h"
#include "sBtlScene.h"
#include "evtMoon.h"
#include "sdfQuat.h"
#include "sdfDelayedResrc.h"

/* NON_MATCHING: 199 of 233 words match. Unit pointer lands in s1 not s0 and the following code shifts; globals and structure otherwise follow IDA. */
#ifdef NON_MATCHING
s32 sbtlCallMemset4(s32 fld, s32 area)
{
    struct SBTLUNIT_WORK *unit;
    sdfVector4_t identity;
    f32 x;
    f32 y;
    f32 z;
    f32 r;
    SBTL_STAGE_DATA *stage;
    f32 pivot;

    memset(&identity, 0, sizeof(identity));
    sbtlData86 = 0;
    identity.w = 1.0f;
    sbtlCallMemset4Data4 = 0;
    sbtlCallMemset4Data5 = 0.0f;
    sbtlCallMemset4Data6 = 0.0f;
    sbtlCallMemset4Data7 = 0.0f;
    sbtlData90 = 0;
    sbtlData91 = 0;
    sdfBtlReLoadMapSBtlSceneSub();
    dds3BistaClear();
    BasicSceneFree();
    mapBtlReLoadMapSBtlSceneSub2();
    evtLoadArea(fld, area);
    if (sbtlCheckTutorialBattle() == 0) {
        evtEnableMoonDraw();
    }
    SetTestCamera();
    sbtlSbtlCameraproc();
    sbtlResetRndEncCount();
    mapMapSetEnc(wk_sbtl->stageNo);
    stage = &sbtlData100[wk_sbtl->stageNo];
    if (stage->form != 0) {
        mapMapSetForm(stage->form);
    }
    mapCutSetCam(dds3GetActiveCameraSceneBasic(dds3GetActiveScene()), 0);
    unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    if (main_data.mode == 1) {
        if (unit == NULL) {
            goto light;
        }
        mapEncPointGet(sbtlData100[wk_sbtl->stageNo].encPoint, &x, &y, &z, &r);
        unit->pos.x = x;
        unit->pos.y = y;
        unit->pos.z = z;
        FromEuler(0.0f, r * 0.017453293f, 0.0f);
        VU0_STORE_VF(vf10, &unit->rot);
        VU0_LOAD_VF(vf10, &identity);
        VU0_STORE_VF(vf10, &unit->rotBase);
        unit->angleY = r;
        if (sbtlCallBtlGetPlayerData() != 0) {
            wk_sbtl->startFlg |= 0x20;
            sbtlCallMemset4Data2.y = y;
            sbtlCallMemset4Data2.z = z;
            sbtlCallMemset4Data2.x = x;
            if (r > -45.0f && r < 45.0f) {
                sbtlCallMemset4Data2.x = x - 200.0f;
            } else if (r < -45.0f && r > -135.0f) {
                sbtlCallMemset4Data2.z = sbtlCallMemset4Data2.z - 200.0f;
            } else if (r > 45.0f && r < 135.0f) {
                sbtlCallMemset4Data2.z = sbtlCallMemset4Data2.z + 200.0f;
            } else {
                sbtlCallMemset4Data2.x = sbtlCallMemset4Data2.x + 200.0f;
            }
            FromEuler(0.0f, r * 0.017453293f, 0.0f);
            VU0_STORE_VF(vf10, &sbtlCallMemset4Data3);
            sbtlData92 = r;
        }
    }
    if (unit != NULL) {
        pivot = unit->pos.y;
        sbtlCallMemset4Data4 = 0;
        sbtlCallMemset4Data5 = unit->pos.x;
        sbtlCallMemset4Data6 = pivot;
        sbtlCallMemset4Data7 = unit->pos.z;
        mapCutSet_From_Pos(unit->pos.x, pivot - 100.0f, unit->pos.z);
    }
light:
    mapCutSetLight();
    sbtlRefSbtl31();
    sbtlSetScrollID(sbtlGetScrollID());
    if (sndBgmStatus_XF() == 2) {
        sndBgmPlay();
    } else {
        sndBgmLoadId_XF(sbtlData100[wk_sbtl->stageNo].bgmId);
    }
    while (sdfDelayedResrcCheck() != 0) {
    }
    pbSetGameCamera0();
    fldHirukoYukaGo_Idx2_B(0x80808080);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0024EE78", sbtlCallMemset4);
#endif

INCLUDE_SDATA(const s32, "code_r1_0024EE78", sbtlData92);

