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
#include "newdata.h"
#include "dat.h"
#include "datCalc.h"
#include "sbattle.h"
#include "sBtlEncount.h"
#include "sbtlWork.h"
#include "sBtlContinue.h"
#include "evtMoon.h"
#include "EventBit.h"
#include "evtStage.h"
#include "sdfSound.h"
#include "dds3KernelDraw.h"
#include "dds3PadManager.h"
#include "dds3WorldBasic.h"
#include "dds3SceneBasic.h"
#include "code_r1_0010DF68.h"
#include "sndAt3.h"

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

/* body: written from the asm when landing; the found body was a different function */
void sbtlRefMapRndEncLoadMode(void) {
    if (gMapRndEncLoad_Mode != 0) {
        sndBgmLoadId_XF(ProcSequenceData5);
    }
}

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

/* Per-frame field walk check: moon damage, devil searches, poison steps,
 * then the random encounter roll. Returns 1 when a battle starts. */
s32 ProcRandomEncountCheck_fld(void) {
    fldSequenceController_t *seq = &fldAutoMapSeqFlags;
    dds3PoseSet_t *poseset;
    f32 distance;
    s32 oldMoon;
    s32 newMoon;
    s32 found;
    s32 nameList;
    s32 battle;

    if (fldGlobalBase.noInpPlCnt > 0) {
        fldGlobalBase.noInpPlCnt--;
    }
    if (fldGlobalBase.noEncountCnt > 0) {
        fldGlobalBase.noEncountCnt--;
    }
    if (fldEncountTestFlag(0x40) == 0) {
        return 0;
    }
    if (fldAutoMapSeqTestFlag(1) == 1) {
        return 0;
    }
    if (dds3GetActiveScene() == NULL) {
        return 0;
    }
    if (playerBasic == NULL) {
        return 0;
    }

    dds3GetPosPoseSet(playerBasic);
    SDF_VU0_LOADVECTOR_B(&ProcRandomEncountCheck_fldData2);
    SDF_VU0_STOREVECTOR(&ProcRandomEncountCheck_fldData2);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(distance);
    if (distance < 0.2f) {
        distance = 0.0f;
    }
    fldGlobalBase.lastMoveLeng = distance;
    oldMoon = evtGetAgeOfMoon16();
    if (fldGlobalBase.noEncountCnt > 0) {
        fldGlobalBase.lastMoveLeng = 0.0f;
        return 0;
    }
    evtMoonAddMove(distance);
    if (fldGlobalBase.word5C & 0xFC) {
        fldReleaseUnitPad();
        fldPlayerMotReset();
        dds3SceneBasicStartScript(dds3GetActiveScene(), (char *)fldKiretaLabel);
        fldPlayerPosRot();
        return 0;
    }

    newMoon = evtGetAgeOfMoon16();
    if (oldMoon != newMoon && evtBitCheck(161) && !evtBitCheck(163) && fldGlobalBase.nakamaMode == 0) {
        if (newMoon == 8) {
            datCallCalcGetBadStatus(3);
            p2sdPlaySE(0x10002, 127, 63);
            dds3SetQuake(30, 40);
            dds3PadAct(0, 48, 20);
            dds3PadAct(1, 48, 30);
            mapProcRandomEncountCheckFldSub(3);
        } else {
            datCallCalcGetBadStatus(2);
            p2sdPlaySE(0x10002, 127, 63);
            dds3SetQuake(30, 40);
            dds3PadAct(0, 48, 20);
            dds3PadAct(1, 48, 30);
            mapProcRandomEncountCheckFldSub(2);
        }
    }

    if (ProcRandomEncountCheck_fldData3 != evtGetAgeOfMoon16() && DDS3_GBWK->takaraCnt == 0) {
        found = mapPlNlRnd();
        if (found != -1) {
            fldReleaseUnitPad();
            fldPlayerMotReset();
            evtMoonTakaraCntReset();
            fldGlobalBase.word2F0 = found;
            ProcRandomEncountCheck_fldData3 = evtGetAgeOfMoon16();
            ProcRandomEncountCheck_fldData4 = evtGetAgeOfMoon16();
            ProcRandomEncountCheck_fldData5 = evtGetAgeOfMoon16();
            datDebugPrintf(ProcRandomEncountCheck_fldStrTakaraSagasiDvlItem, DDS3_GBWK->mapSummonID, found);
            mapCallNakamaWakeUp7();
            seq->stage = 61;
            return 0;
        }
    }
    ProcRandomEncountCheck_fldData3 = evtGetAgeOfMoon16();

    if (ProcRandomEncountCheck_fldData4 != evtGetAgeOfMoon16() && DDS3_GBWK->zeniCnt == 0) {
        found = mapPlNlRnd2();
        if (found != -1) {
            fldReleaseUnitPad();
            fldPlayerMotReset();
            evtMoonZeniCntReset();
            fldGlobalBase.word2F4 = found;
            ProcRandomEncountCheck_fldData4 = evtGetAgeOfMoon16();
            ProcRandomEncountCheck_fldData5 = evtGetAgeOfMoon16();
            datDebugPrintf(ProcRandomEncountCheck_fldStrZeniSagasiDvlYen, DDS3_GBWK->mapSummonID, found);
            fldCallNakamaWakeUp3();
            seq->stage = 60;
            return 0;
        }
    }
    ProcRandomEncountCheck_fldData4 = evtGetAgeOfMoon16();

    if (!evtBitCheck(864)) {
        if (ProcRandomEncountCheck_fldData5 != evtGetAgeOfMoon16() && DDS3_GBWK->takumiCnt == 0
            && mapGetSkilTakumiMapCheck() == 1) {
            fldReleaseUnitPad();
            fldPlayerMotReset();
            evtMoonTakumiCntReset();
            ProcRandomEncountCheck_fldData5 = evtGetAgeOfMoon16();
            mapNakamaSearchTask();
            seq->stage = 62;
            return 0;
        }
        ProcRandomEncountCheck_fldData5 = evtGetAgeOfMoon16();
    }

    if (fldGlobalBase.nakamaMode == 0) {
        if (datCallCalcGetBadStatus2(6)) {
            fldGlobalBase.floatE4 += distance;
            if (fldGlobalBase.floatE4 > 500.0f) {
                fldGlobalBase.floatE4 = 0.0f;
                datCallCalcGetBadStatus(1);
                p2sdPlaySE(0x10002, 127, 63);
                dds3SetQuake(30, 40);
                dds3PadAct(0, 32, 10);
                dds3PadAct(1, 32, 20);
                mapProcRandomEncountCheckFldSub(1);
            }
        }
    } else {
        s32 stock = datCallGetStockDevilID(DDS3_GBWK->mapSummonID);

        if (stock != -1 && sdatCalc_GetBadStatus(&DDS3_GBWK->playerData.stock[stock]) == 6) {
            fldGlobalBase.floatE4 += distance;
            if (fldGlobalBase.floatE4 > 500.0f) {
                fldGlobalBase.floatE4 = 0.0f;
                datCallCalcGetBadStatus(4);
                p2sdPlaySE(0x10002, 127, 63);
                dds3SetQuake(30, 40);
                dds3PadAct(0, 32, 10);
                dds3PadAct(1, 32, 20);
                mapProcRandomEncountCheckFldSub(1);
            }
        }
    }

    if (distance < 9998.0f && fldDbg.encountOn == 0 && fldDbg.encountBtnOn == 0) {
        return 0;
    }
    nameList = fldGetFldBgNameList(fldGlobalBase.focusX, fldGlobalBase.focusZ);
    fldGlobalBase.nameD = nameList;
    battle = sbtlRndEncCalc(nameList, distance * 2.0f);
    if (battle != 0) {
        if (fldDbg.encountOn == 1) {
            fldAutoMapSeqSetFlag(1);
            fldAutoMapSeqSetFlag(2);
            fldReleaseUnitPad();
            evtKillFieldScript();
            fldGlobalBase.float20C = fldGlobalBase.focusX;
            fldGlobalBase.float210 = fldGlobalBase.focusY;
            fldGlobalBase.float214 = fldGlobalBase.focusZ;
            fldGlobalBase.rndEncX = fldGlobalBase.focusX;
            fldGlobalBase.rndEncY = fldGlobalBase.focusY;
            fldGlobalBase.rndEncZ = fldGlobalBase.focusZ;
            poseset = playerBasic->poseset;
            ProcSequenceData.pos.x = poseset->pos.x;
            fldGlobalBase.rndEncFieldID = fldGlobalBase.amSBlock;
            ProcSequenceData.pos.y = poseset->pos.y;
            fldGlobalBase.rndEncFloorID = fldGlobalBase.nameU;
            fldGlobalBase.rndEncMode = 1;
            ProcSequenceData.pos.z = poseset->pos.z;
            ProcSequenceData.type = 1;
            fldGlobalBase.rndEncNo = battle;
            ProcSequenceData.pos.w = poseset->pos.w;
            ProcSequenceData.rot.x = poseset->rot.x;
            ProcSequenceData.rot.y = poseset->rot.y;
            ProcSequenceData.rot.z = poseset->rot.z;
            ProcSequenceData.rot.w = poseset->rot.w;
            ProcSequenceData.offset_x = 0.0f;
            ProcSequenceData.encountno = battle;
            ProcSequenceData.offset_z = 0.0f;
            if (fldGlobalBase.nakamaMode == 0) {
                ProcSequenceData.stock_id = -1;
                ProcSequenceData.mode = 0;
                if (fldGlobalBase.wordE0 == 2 || fldGlobalBase.wordE0 == 3) {
                    ProcSequenceData.mode = 2;
                }
            } else {
                ProcSequenceData.mode = 3;
                ProcSequenceData.stock_id = datCallGetStockDevilID(DDS3_GBWK->mapSummonID);
            }
            pbCallBtlGetRndEncJoken(nameList);
            fldGlobalBase.word24C++;
            return 1;
        }
        sbtlResetRndEncCount();
        p2sdPlaySE(15, 127, 63);
        dds3FadeIn(128, 128, 128, 6);
        fldGlobalBase.word24C++;
    }
    return 0;
}

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

INCLUDE_RODATA(const s32, "code_0012B6A0", fldKiretaLabel);

INCLUDE_RODATA(const s32, "code_0012B6A0", ProcRandomEncountCheck_fldStrTakaraSagasiDvlItem);

INCLUDE_RODATA(const s32, "code_0012B6A0", ProcRandomEncountCheck_fldStrZeniSagasiDvlYen);

INCLUDE_RODATA(const s32, "code_0012B6A0", mapIdouGo_IdxTable);

