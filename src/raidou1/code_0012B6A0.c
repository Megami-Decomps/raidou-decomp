#include "common.h"
#include "fld.h"
#include "fldProcess.h"
#include "field.h"
#include "basic.h"
#include "dds3Basic.h"
#include "mapData.h"
#include "ps2asm.h"
#include "dds3ObjectBase.h"
#include "fldFileResolver.h"
#include "fldDbg.h"

INCLUDE_ASM(const s32, "code_0012B6A0", fldCallElvReset);

INCLUDE_ASM(const s32, "code_0012B6A0", fldCallElvReset2);

INCLUDE_ASM(const s32, "code_0012B6A0", fldBtlDebugDevilNameListDrawSub);

INCLUDE_ASM(const s32, "code_0012B6A0", fldTitle_Shutdown);

INCLUDE_ASM(const s32, "code_0012B6A0", fldCallMotionPauseOff);

INCLUDE_ASM(const s32, "code_0012B6A0", mapRetuenFromSave);

INCLUDE_ASM(const s32, "code_0012B6A0", mapRetuenFromLoad);

INCLUDE_ASM(const s32, "code_0012B6A0", mapRetuenFromNakamDead);

s32 mapExitFieldSub(void) {
    return ProcSequenceData4;
}

INCLUDE_ASM(const s32, "code_0012B6A0", sbtlReLoadFldScene);

INCLUDE_ASM(const s32, "code_0012B6A0", sbtlRefMapRndEncLoadMode);

INCLUDE_ASM(const s32, "code_0012B6A0", mapRndEncReturn_BgmLoadEndCheck);

INCLUDE_ASM(const s32, "code_0012B6A0", ProcMskSub_A);

/* NON_MATCHING: 175 of 183 words match.  */
#ifdef NON_MATCHING
void ProcMskSub_B(void) {
    if (fldGlobalBase.nakamaMode) {
        if (raidoBasic != NULL && fldGlobalBase.word190 == fldGlobalBase.amSBlock) {
            if (fldGlobalBase.word194 == fldGlobalBase.nameU) {
                if (fldProcMskSubBSub2()) {
                    mapUnitSetLight(raidoBasic, mapGetFloorLight(fldGlobalBase.word194, fldGlobalBase.float184, fldGlobalBase.float18C), 0);
                } else {
                    mapUnitSetLight(raidoBasic, mapGetFloorLight(fldGlobalBase.word194, fldGlobalBase.float184, fldGlobalBase.float18C), 10);
                }
                if (!mapBigClipChkF(fldGlobalBase.cutIdx, fldGlobalBase.float184, fldGlobalBase.float188, fldGlobalBase.float18C)) {
                    dds3ObjectBaseOnFlag(raidoBasic, 1);
                } else {
                    dds3ObjectBaseOffFlag(raidoBasic, 1);
                }
            } else if (fldGlobalBase.word194 == gSubFloorID) {
                if (!fldProcMskSubBSub()) {
                    mapUnitSetLight(raidoBasic, mapGetFloorLight(fldGlobalBase.word194, fldGlobalBase.float184, fldGlobalBase.float18C), 0);
                } else {
                    mapUnitSetLight(raidoBasic, mapGetFloorLight(fldGlobalBase.word194, fldGlobalBase.float184, fldGlobalBase.float18C), 10);
                }
                if (fldGlobalBase.amSBlock != 8) {
                    if (!mapBigClipChkF(fldGlobalBase.cutIdx, fldGlobalBase.float184, fldGlobalBase.float188, fldGlobalBase.float18C)) {
                        dds3ObjectBaseOnFlag(raidoBasic, 1);
                    } else {
                        dds3ObjectBaseOffFlag(raidoBasic, 1);
                    }
                }
            }
        }
        if (aiboBasic != NULL && fldGlobalBase.word1A8 == fldGlobalBase.amSBlock) {
            if (fldGlobalBase.word1AC == fldGlobalBase.nameU) {
                mapUnitSetLight(aiboBasic, mapGetFloorLight(fldGlobalBase.word1AC, aiboBasic->poseset->pos.x, aiboBasic->poseset->pos.z), 0);
                if (!mapBigClipChkF(fldGlobalBase.cutIdx, aiboBasic->poseset->pos.x, aiboBasic->poseset->pos.y, aiboBasic->poseset->pos.z)) {
                    dds3ObjectBaseOnFlag(aiboBasic, 1);
                } else {
                    dds3ObjectBaseOffFlag(aiboBasic, 1);
                }
            } else if (fldGlobalBase.word1AC == gSubFloorID) {
                mapUnitSetLight(aiboBasic, mapGetFloorLight(fldGlobalBase.word1AC, aiboBasic->poseset->pos.x, aiboBasic->poseset->pos.z), 0);
                if (fldGlobalBase.amSBlock != 8) {
                    if (!mapBigClipChkF(fldGlobalBase.cutIdx, aiboBasic->poseset->pos.x, aiboBasic->poseset->pos.y, aiboBasic->poseset->pos.z)) {
                        dds3ObjectBaseOnFlag(aiboBasic, 1);
                    } else {
                        dds3ObjectBaseOffFlag(aiboBasic, 1);
                    }
                }
            }
        }
    }
    mapAiboTask();
    mapObjTask();
    mapTakaraTask();
    mapNakamaTask();
    if (fldDbg.ctnOn) {
        mapCtnGet();
    } else {
        mapCtnViewOff();
    }
    mapWallObjTask(fldGlobalBase.focusX, fldGlobalBase.focusY, fldGlobalBase.focusZ);
}
#else
INCLUDE_ASM(const s32, "code_0012B6A0", ProcMskSub_B);
#endif

INCLUDE_ASM(const s32, "code_0012B6A0", ProcRandomEncountCheck);

INCLUDE_ASM(const s32, "code_0012B6A0", ProcRandomEncountCheck_fld);

void CutSet(s32 id) {
    sdfVector4_t pos;
    dds3PoseSet_t *poseset;

    pos.x = fldGlobalBase.focusX;
    pos.y = fldGlobalBase.focusY;
    pos.z = fldGlobalBase.focusZ;
    dds3SetPosPoseSet_dds3Basic(playerBasic, &pos);
    poseset = playerBasic->poseset;
    SDF_VU0_COPYVECTOR(&poseset->pos2, &pos);
    dds3OnPoseSetFlag(poseset, 8);
    mapCutSetSub(id);
    mapCutBgLoad();
    mapCutSetCam(cameraBasic, 0);
}

INCLUDE_ASM(const s32, "code_0012B6A0", mapIdouGo_Idx);

void mapIsFldKeyFreeSub3(void) {
}
