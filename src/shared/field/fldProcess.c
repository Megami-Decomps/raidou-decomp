#include "common.h"
#include "basic.h"
#include "kernel.h"
#include "libsdf.h"
#include "ps2asm.h"
#include "newdata.h"
#include "field.h"
#include "effect.h"
#include "sdfFrame.h"
#include "fldProcess.h"
#include "dds3PadManager.h"
#include "fld.h"
#include "fldFileResolver.h"
#include "dds3obj.h"
#include "dds3AdminiProcess.h"
#include "dds3WorldBasic.h"
#include "dds3SceneBasic.h"
#include "sdfQuat.h"
#include "code_r1_00128378.h"
#include "sdk_libc.h"
#include "cmpMain.h"
#include "cmpMap.h"
#include "code_r1_0010DF68.h"
#include "dds3Randomize.h"
#include "dds3ObjectBase.h"
#include "itf.h"
#include "EventBit.h"
#include "evtMoon.h"
#include "slMain.h"
#include "sdfSound.h"
#include "dat.h"
#include "sdfDelayedResrc.h"
#include "sndAt3.h"
#include "mapData.h"
#include "dds3KernelCore.h"



extern char dds3DebugLabelB[], dds3DebugLabelG[], dds3DebugLabelR[], dds3DebugFmtInt3[], dds3DebugFmtFloat3[];

typedef struct {
    u32 flags;
    s32 stage;
} fldSequenceController_t;

extern const char fldProcDrawName[];
extern void fldAutoMapSeqSetFlag(u32 flag);
extern effBlurScatterWork_t *hDistortionBlur;
extern void fldEncountClearFlag(u32 flag);
extern s32 fldEncountTestFlag(u32 flag);
extern s32 fldAutoMapSeqTestFlag(u32 flag);
extern void fldAutoMapSeqClearFlag(u32 flag);
extern void fldEncountSetFlag(u32 flag);

typedef struct scrProcessWork_s scrProcessWork_s;

typedef struct dds3FontReq_t dds3FontReq_t;

/* destroy runs when the request leaves the list, draw once per frame while it
 * is still on it. */
typedef struct
{
    void (*destroy)(dds3FontReq_t *req);
    void (*draw)(dds3FontReq_t *req);
} dds3FontReqFunc_t;

struct dds3FontReq_t
{
    dds3FontReq_t *prev;
    dds3FontReq_t *next;
    s32 group;
    dds3FontReqFunc_t *func;
};

extern fldSequenceController_t fldAutoMapSeqFlags;
extern s8 slNewGameBit2208, slNewGameBit2209, slNewGameBit2210, slNewGameBit2211;
extern s8 slNewGameBit2212, slNewGameBit2213, slNewGameBit2323, slNewGameBit162;
extern s32 gFldStart, fldSkyEnable, gfldPlayerHasiCnt;
extern void dds3EffectBlur(const dds3Effect_Blur_t *source);

#define DRAW_FIELD_BLUR(color, mode, angle, zoom) { \
    blur.abgr = (color); \
    blur.blend = (mode); \
    blur.rotate = (angle); \
    blur.scale = (zoom); \
    blur.center[0] = 0; \
    blur.center[1] = 0; \
    blur.xy[0][0] = 0; \
    blur.xy[0][1] = 0; \
    blur.xy[1][0] = 0x200; \
    blur.xy[1][1] = 0x1C0; \
    dds3EffectBlur(&blur); \
}

void fldAutoMapSeqSetFlag(u32 flag);
void fldAutoMapSeqClearFlag(u32 flag);
s32 fldAutoMapSeqTestFlag(u32 flag);
void fldEncountSetFlag(u32 flag);
void fldEncountClearFlag(u32 flag);
s32 fldEncountTestFlag(u32 flag);
extern const char fldProcDrawName[16];
extern s32 gFldStart;

/* NON_MATCHING: 276 of 2882 words match. Switch cases reconstructed from the pseudocode and checked against the asm, so tail-merged blocks differ. ProcSequence is Raidou 1 only, the field layout differs in Raidou 2. */
#ifdef VERSION_RAIDOU1
#ifdef NON_MATCHING
s32 ProcSequence(dds3ProcessID_t *id)
{
    sdfVector4_t rotation;
    sdfVector4_t axis = { 0.0f, 1.0f, 0.0f, 1.0f };
    f32 pos[4];
    f32 entryPos[2];
    f32 entryPos2[2];
    char scriptName[32];
    s32 result;
    s32 bgmId;
    s32 titleStart;
    s32 cutReq;
    s32 badStatus;
    s32 mafuResult;
    s32 hirukoResult;
    s32 titleResult;
    s32 nextStage;
    s32 adminData;

    dds3GetRandIntA(0, 32);
    if (dds3AdminiGetNextSequence() != -1) {
        return 0;
    }
    if (encLoad()) {
        return 0;
    }
    if (!fldEncountTestFlag(0x40)) {
        fldPlayerPosRot();
    }
    bgmId = p2sdCallBitCheck(fldGlobalBase.amSBlock, fldGlobalBase.nameU);
    mapEffEplAllCalcTask();
    fldProcSequenceSub3();
    mapSpotPointDrawEndChk();
    switch (fldAutoMapSeqFlags.stage) {
    case 0:
        datDebugPrintf("fld_process_init:0 SEVOL:%d\n", sndProcSequenceSub4());
        fldGbInit_Field();
        fldGlobalBase.amSBlock = gfldSceneParam.loadField;
        fldGlobalBase.nameU = gfldSceneParam.loadFloor;
        ProcSequenceData5 = -1;
        ProcSequenceData4 = 0;
        gMapRndEncLoad_Mode = 0;
        fldCallGetRandA3();
        datInitBossPress();
        mapWaitCbScreenFadeBSub(0);
        if (fldGlobalBase.word2E8) {
            datDebugPrintf("EISEI TAIITU devil:[%02x]\n", fldGlobalBase.word2EC);
            fldGlobalBase.summonStop = 1;
            DDS3_GBWK->mapSummonID = fldGlobalBase.word2EC;
            fldGlobalBase.word2E8 = 0;
            fldGlobalBase.word2EC = 0;
        }
        if (fldGlobalBase.word2C0 && pbProcSequenceSub() != 1) {
            mapProcSequenceSub4();
        }
        if (fldGlobalBase.amSBlock == 31) {
            nbProcSequenceSub(46);
            fldDrwZAlloc();
        }
        if (fldGlobalBase.amSBlock == 1 || fldGlobalBase.amSBlock == 21) {
            nbSetEncountMode(1);
        }
        dds3BackColor = 0x80000000;
        mapTakaraOpen();
        titleStart = 0;
        mapProcSequenceSub8();
        mapProcSequenceSub10();
        fldGlobalBase.word314 = 0;
        mapIfMenuInit();
        mapMenuProcessStart();
        if (fldGlobalBase.word1E0) {
            titleStart = 1;
            mapTitleStart_fld(fldGlobalBase.word1E0, fldGlobalBase.word1E4, 0, 0);
            fldGlobalBase.word1E0 = 0;
            fldGlobalBase.word1E4 = 0;
        }
        if (fldPlayerALoad_OnDataCheck()) {
            if (titleStart) {
                fldAutoMapSeqFlags.stage = 1;
            } else {
                fldAutoMapSeqFlags.stage = 2;
            }
        } else {
            fldPlayerALoadStart();
            fldAutoMapSeqFlags.stage = 1;
        }
        ProcSequenceData6 = 0;
        break;
    case 1:
        if (fldPlayerALoadEndCheck()) {
            datDebugPrintf("LB LOAD TIME:%d\n", ProcSequenceData6);
            if (ProcSequenceData6 >= 16) {
                fldAutoMapSeqFlags.stage = 2;
            }
        }
        ProcSequenceData6++;
        break;
    case 2:
        if (fldGlobalBase.word304) {
            sndSeFadeOut();
        }
        fldReadyScene();
        dds3SetActiveScene(dds3GetControlScene());
        datDebugPrintf("fld_process_init:1 SEVOL:%d\n", sndProcSequenceSub4());
        if (fldGlobalBase.amSBlock == 6) {
            evtInitEventViewer();
        }
        if (fldGlobalBase.amSBlock == 14) {
            elvMenuProcessStart();
        }
        DecisionFeedIn();
        dds3VisibleSceneBasic(dds3GetActiveScene(), 1);
        dds3ProcSequenceSub(1);
        fldGlobalBase.ifDrawOn = 1;
        mapBgDrawOnOff(1);
        evtEnableMoonDraw();
        if (fldGlobalBase.amSBlock != 31) {
            fldHirukoYukaClear();
        }
        dds3FadeOut2(0);
        fldBgZoomTest(4);
        fldEncountClearFlag(0x40);
        datDebugPrintf("fld_process_init:2 SEVOL:%d\n", sndProcSequenceSub4());
        fldGlobalBase.wordF8 = 0;
        if (gfldSceneParam.loadPoint == -1) {
            fldGlobalBase.focusY = mapGetY(fldGlobalBase.focusX, fldGlobalBase.focusZ);
        } else {
            datDebugPrintf("param->loadPoint=%04x\n", gfldSceneParam.loadPoint);
            mapObjPointGet(gfldSceneParam.loadPoint, &pos[0], &pos[1], &pos[2], &pos[3]);
            fldGlobalBase.focusX = pos[0];
            fldGlobalBase.focusZ = pos[2];
            fldGlobalBase.focusY = mapGetY(fldGlobalBase.focusX, fldGlobalBase.focusZ);
            fldGlobalBase.cutIdx = mapGetCut_FXY(fldGlobalBase.focusX, fldGlobalBase.focusZ);
            CutSet(fldGlobalBase.cutIdx);
            fldGlobalBase.floatC0 = pos[3];
            fldGlobalBase.floatC4 = pos[3];
            VU0_LOAD_VF(vf10, &axis);
            FromRotation(fldGlobalBase.floatC0 * 3.14f / 180.0f);
            VU0_STORE_VF(vf10, &rotation);
            dds3SetRotPoseSet(playerBasic, &rotation);
        }
        fldGlobalBase.word1D0 = 0;
        if (fldGlobalBase.rndEncMode) {
            datDebugPrintf("returnToMap RndEnc\n");
            fldGlobalBase.rndEncMode = 0;
            fldGlobalBase.focusX = fldGlobalBase.rndEncX;
            fldGlobalBase.focusY = fldGlobalBase.rndEncY;
            fldGlobalBase.focusZ = fldGlobalBase.rndEncZ;
            fldPlayerForceSetRot_NoHokan(fldGlobalBase.floatC4);
            fldGlobalBase.word1D0 = 1;
        } else {
            mapHamidasiCheck();
            if (!fldGlobalBase.summonStop) {
                mapAiboFoceSet3(fldGlobalBase.focusX, fldGlobalBase.focusY, fldGlobalBase.focusZ);
                mapAiboWakeUp(1);
                mapSummonDvFoceSet2(fldGlobalBase.focusX, fldGlobalBase.focusY, fldGlobalBase.focusZ);
                mapNakamaWakeUp(1);
            }
        }
        fldPlayerForceSetPos_NoHokan(fldGlobalBase.focusX, fldGlobalBase.focusY, fldGlobalBase.focusZ);
        datDebugPrintf("fld_process_init:3 SEVOL:%d\n", sndProcSequenceSub4());
        cutReq = fldGlobalBase.word218;
        if (cutReq == -1) {
            mapCutSet_From_Pos(fldGlobalBase.focusX, fldGlobalBase.focusY, fldGlobalBase.focusZ);
        } else {
            if (fldGlobalBase.word21C != -1) {
                fldGlobalBase.word1DC = fldGlobalBase.word21C;
            }
            fldGlobalBase.word21C = -1;
            fldGlobalBase.cutIdx = cutReq;
            fldGlobalBase.word218 = -1;
            mapCatChkMain(cutReq);
        }
        if (fldGlobalBase.summonStop) {
            mdlSetMotionSpeed(5);
        } else {
            mdlSetMotionSpeed(0);
        }
        mdlChkMotionEnd();
        mapNakamaSkilIs_SetBit(DDS3_GBWK->mapSummonID);
        fldGlobalBase.nameD = fldGetFldBgNameList();
        datDebugPrintf("fld_process_init:4 SEVOL:%d\n", sndProcSequenceSub4());
        fldGlobalBase.prevY = 0;
        fldGlobalBase.float20C = fldGlobalBase.focusX;
        fldGlobalBase.float210 = fldGlobalBase.focusY;
        fldGlobalBase.float214 = fldGlobalBase.focusZ;
        fldGlobalBase.word204 = 0;
        fldGlobalBase.float208 = (f32)dds3GetRandIntA(0, 2000) + 2500.0f;
        if (playerBasic) {
            mapBgSetScroll(playerBasic->poseset->pos2.x, playerBasic->poseset->pos2.y - 128.0f, playerBasic->poseset->pos2.z, 0);
        }
        mapCutSetCam(cameraBasic, 0);
        if (!fldGlobalBase.summonStop) {
            fldGlobalBase.wordF8 = mapCallSetPlayerTargetData3(fldGlobalBase.focusX, fldGlobalBase.focusZ);
            mapUnitSetLight(playerBasic, fldGlobalBase.wordF8, 0);
            if (aiboBasic) {
                mapUnitSetLight(aiboBasic, mapCallSetPlayerTargetData3(aiboBasic->poseset->pos.x, aiboBasic->poseset->pos.z), 0);
            }
            if (summonDvBasic) {
                mapUnitSetLight(summonDvBasic, mapCallSetPlayerTargetData3(summonDvBasic->poseset->pos.x, summonDvBasic->poseset->pos.z), 0);
            }
        } else {
            fldGlobalBase.wordF8 = mapCallSetPlayerTargetData3(fldGlobalBase.focusX, fldGlobalBase.focusZ);
            mapUnitSetLight(playerBasic, fldGlobalBase.wordF8, 0);
            if (fldGlobalBase.amSBlock == fldGlobalBase.word190) {
                if (fldGlobalBase.nameU == fldGlobalBase.word194) {
                    if (raidoBasic) {
                        mapUnitSetLight(raidoBasic, mapCallSetPlayerTargetData3(raidoBasic->poseset->pos.x, raidoBasic->poseset->pos.z), 0);
                    }
                } else if (gSubFloorID == fldGlobalBase.word194 && raidoBasic) {
                    mapUnitSetLight(raidoBasic, mapGetFloorLight(gSubFloorID, raidoBasic->poseset->pos.x, raidoBasic->poseset->pos.z), 0);
                }
            }
            if (fldGlobalBase.amSBlock == fldGlobalBase.word1A8) {
                if (fldGlobalBase.nameU == fldGlobalBase.word1AC) {
                    if (aiboBasic) {
                        mapUnitSetLight(aiboBasic, mapCallSetPlayerTargetData3(aiboBasic->poseset->pos.x, aiboBasic->poseset->pos.z), 0);
                    }
                } else if (gSubFloorID == fldGlobalBase.word1AC && aiboBasic) {
                    mapUnitSetLight(aiboBasic, mapGetFloorLight(gSubFloorID, aiboBasic->poseset->pos.x, aiboBasic->poseset->pos.z), 0);
                }
            }
        }
        sndProcSequenceSub();
        mapCallBitCheck();
        if (fldGlobalBase.wordD8 > 0) {
            datDebugPrintf("SEQ_TOBISE_LOAD\n");
            fldAutoMapSeqFlags.stage = 3;
            mnuQueueTitleSoundStreamEntry(55);
            break;
        }
        goto start_script;
    case 3:
        if (sndSePlay() != 2) {
            break;
        }
    start_script:
        if (fldGlobalBase.initScript) {
            dds3SceneBasicStartScript(dds3GetControlScene(), fldGlobalBase.initScript);
            datDebugPrintf("SEQ_INITALSCRIPTWAIT\n");
            fldAutoMapSeqFlags.stage = 4;
        } else {
            datDebugPrintf("SEQ_BGMLOAD\n");
            fldAutoMapSeqFlags.stage = 5;
        }
        break;
    case 4:
        if (scrSearchScriptName(fldGlobalBase.initScript)) {
            break;
        }
        if (sndProcSequenceSub2()) {
            fldAutoMapSeqFlags.stage = 7;
            datDebugPrintf("SEQ_SMGLOAD\n");
        } else {
            fldAutoMapSeqFlags.stage = 5;
            datDebugPrintf("SEQ_BGMLOAD SEVOL:%d\n", sndProcSequenceSub4());
        }
        break;
    case 5:
        if (fldGlobalBase.word1D4 == 1 && fldGlobalBase.word1CC != bgmId) {
            fldGlobalBase.word1D4 = 0;
        }
        datDebugPrintf("DelayPlay:%d SEVOL:%d\n", fldGlobalBase.word310, sndProcSequenceSub4());
        if (!fldGlobalBase.word1D4) {
            sndBgmLoadId_XF(bgmId);
            fldGlobalBase.word1CC = bgmId;
        }
        fldAutoMapSeqFlags.stage = 6;
        datDebugPrintf("SEQ_BGMLOAD_WAIT SEVOL:%d\n", sndProcSequenceSub4());
        break;
    case 6:
        if (fldGlobalBase.word1D4) {
            fldGlobalBase.word1D4 = 0;
            fldAutoMapSeqFlags.stage++;
            datDebugPrintf("SEQ_SMGLOAD\n");
        } else if (sndBgmStatus_XF() == 2) {
            if (!fldGlobalBase.word310) {
                if (fldGlobalBase.word1D0) {
                    sndBgmContinue();
                } else {
                    sndBgmPlay();
                }
            }
            fldAutoMapSeqFlags.stage++;
            datDebugPrintf("SEQ_SMGLOAD SEVOL:%d\n", sndProcSequenceSub4());
        }
        break;
    case 7:
        mapSoundSmgLoadChk(fldGlobalBase.amSBlock);
        fldAutoMapSeqFlags.stage++;
        datDebugPrintf("SEQ_SMGLOAD_WAIT SEVOL:%d\n", sndProcSequenceSub4());
        break;
    case 8:
        if (!mapSoundSmgLoadChk_fld()) {
            break;
        }
        fldGlobalBase.word248 = 1;
        if (!fldGlobalBase.word310) {
            mapSoundKankyoSeSet(fldGlobalBase.amSBlock, fldGlobalBase.nameU, fldGlobalBase.cutIdx);
        }
        fldAutoMapSeqFlags.stage++;
        datDebugPrintf("SEQ_STARTSCR_WAIT SEVOL:%d\n", sndProcSequenceSub4());
        break;
    case 9:
        datDebugPrintf("SEQ_PRIVATE_PROCSTART SEVOL:%d\n", sndProcSequenceSub4());
        fldAutoMapSeqFlags.stage++;
        break;
    case 0xA:
        if (fldGlobalBase.wordD8 > 0) {
            rstDrawEvoBgFade(27, 0.5f);
            fldAutoMapSeqFlags.stage = 11;
            datDebugPrintf("SEQ_TOBIORI\n");
        } else if (fldGlobalBase.word310) {
            fldAutoMapSeqFlags.stage = 12;
            datDebugPrintf("SEQ_BGM_DPLAY\n");
        } else if (!fldGlobalBase.summonStop) {
            fldAutoMapSeqFlags.stage = 14;
            datDebugPrintf("SEQ_NORMAL\n");
        } else {
            fldAutoMapSeqFlags.stage = 15;
            datDebugPrintf("SEQ_NORMAL_NAKAMA\n");
        }
        mapTitleSkip();
        if (fldGlobalBase.bgZoomSet) {
            mapScreenReset();
            fldGlobalBase.bgZoomSet = 0;
            fldBgZoomTest(5);
            mapScreenReset();
            if (playerBasic) {
                mapBgSetScroll(playerBasic->poseset->pos.x, playerBasic->poseset->pos.y - 128.0f, playerBasic->poseset->pos.z, 0);
            }
            fldBgZoomTest(2);
        }
        if (fldGlobalBase.summonStop) {
            mdlSetMotionSpeed(5);
        } else {
            mdlSetMotionSpeed(1);
        }
        if (!fldGlobalBase.wordD8 && !dds3SearchProcessPriority(1002)) {
            fldRegistUnitPad();
        }
        if (gfldSceneParam.FoceScript[0]) {
            datDebugPrintf("FoceScript\n");
            sprintf(scriptName, "%s", gfldSceneParam.FoceScript);
            fldPlayerMotReset3();
            dds3SceneBasicStartScript(dds3GetActiveScene(), scriptName);
            if (!strcmp(scriptName, "m13continue_label")) {
                dds3FadeIn2(20);
            }
            if (!strcmp(scriptName, "continue_label")) {
                goto fade_in;
            }
            goto script_done;
        }
        if (fldGlobalBase.word2C0) {
            if (pbProcSequenceSub() == 1) {
                datDebugPrintf("WallEsc\n");
            } else {
                datDebugPrintf("WallScript\n");
                fldPlayerMotReset3();
                dds3SceneBasicStartScript(dds3GetActiveScene(), "mwall_dead_label");
            }
        } else {
            if (!fldGlobalBase.word80) {
                if (fldGlobalBase.startScript) {
                    datDebugPrintf("StartScript\n");
                    dds3SceneBasicStartScript(dds3GetControlScene(), fldGlobalBase.startScript);
                    if (fldGlobalBase.word2D4) {
                        goto script_done;
                    }
                    goto fade_in;
                }
                datDebugPrintf("NoScript SEVOL:%d\n", sndProcSequenceSub4());
                dds3FadeIn2(20);
            script_done:
                gfldSceneParam.FoceScript[0] = 0;
                fldGlobalBase.word2C0 = 0;
                fldGlobalBase.startScript = 0;
                mapProcSequenceSub12(1);
                break;
            }
            datDebugPrintf("BtlMapRetScript\n");
            fldPlayerMotReset3();
            dds3SceneBasicStartScript(dds3GetActiveScene(), "jimusyo_return_label");
        }
    fade_in:
        dds3FadeIn2(20);
        goto script_done;
    case 0xB:
        if (fldGlobalBase.wordD8 == 95) {
            sndCallWaitSema();
        }
        if (fldGlobalBase.wordD8 <= 0) {
            if (fldGlobalBase.word310) {
                fldAutoMapSeqFlags.stage = 12;
                datDebugPrintf("SEQ_BGM_DPLAY\n");
            } else if (fldGlobalBase.summonStop) {
                fldAutoMapSeqFlags.stage = 15;
                datDebugPrintf("SEQ_NORMAL_NAKAMA\n");
            } else {
                fldAutoMapSeqFlags.stage = 14;
                datDebugPrintf("SEQ_NORMAL\n");
            }
        } else {
            fldGlobalBase.wordD8--;
            if (fldGlobalBase.wordD8 <= 0) {
                fldGlobalBase.wordD8 = 0;
                fldRegistUnitPad();
            }
        }
        goto proc_msk_b;
    case 0xC:
        fldAutoMapSeqFlags.stage = 13;
        datDebugPrintf("SEQ_BGM_DPLAY2\n");
        goto proc_msk_b;
    case 0xD:
        fldGlobalBase.word310 = 0;
        if (!fldGlobalBase.word314) {
            datDebugPrintf("kankyo se start\n");
            mapSoundKankyoSeSet(fldGlobalBase.amSBlock, fldGlobalBase.nameU, fldGlobalBase.cutIdx);
        }
        if (sndProcSequenceSub2()) {
            datDebugPrintf("map bgm skip!!\n");
        } else {
            datDebugPrintf("bgm script reqest NO map bgm Start!!\n");
            if (fldGlobalBase.word1D0) {
                sndBgmContinue();
            } else {
                sndBgmPlay();
            }
        }
        if (fldGlobalBase.summonStop) {
            fldAutoMapSeqFlags.stage = 15;
            datDebugPrintf("SEQ_NORMAL_NAKAMA\n");
        } else {
            fldAutoMapSeqFlags.stage = 14;
            datDebugPrintf("SEQ_NORMAL\n");
        }
    proc_msk_b:
        ProcMskSub_B();
        break;
    case 0xE:
        if (evtBitCheck(2101)) {
            goto e_encounter_key;
        }
        if ((fldGlobalBase.amSBlock != 3 || fldGlobalBase.nameU != 2) && mapBadMafuHajiki()) {
            goto bad_mafu;
        }
        if (!fldGlobalBase.word334 && fldEncountTestFlag(0x40) && (fldGlobalBase.amSBlock != 3 || fldGlobalBase.nameU != 2) && dds3PadButtonState[0x20] < 0 && !fldGlobalBase.noInpPlCnt) {
            goto menu_key;
        }
    e_encounter_key:
        if (fldEncountTestFlag(0x40) && fldGlobalBase.word240 != 2 && (fldGlobalBase.amSBlock != 3 || fldGlobalBase.nameU != 2) && dds3PadButtonState[0x22] < 0 && !fldGlobalBase.noInpPlCnt) {
            p2sdPlaySE(17, 127, 63);
            fldAutoMapSeqFlags.stage = 19;
            fldReleaseUnitPad();
            fldPlayerMotReset();
            dds3FadeOut(0, 0, 0, 8);
        }
        fldPlayerCalc();
        mapAiboTask();
        mapNakamaTask();
        fldGlobalBase.wordF8 = mapCallSetPlayerTargetData3(fldGlobalBase.focusX, fldGlobalBase.focusZ);
        mapUnitSetLight(playerBasic, fldGlobalBase.wordF8, 10);
        mapCalcFXY(fldGlobalBase.wordF8);
        fldGlobalBase.encountState = mapProcSequenceSub5(fldGlobalBase.focusX, fldGlobalBase.focusZ);
        if (fldGlobalBase.word35C) {
            mapCallSetPlayerTargetData4();
        }
        mapProcSequenceSub11();
        result = fldSaveLabel();
        if (result == -1) {
            return result;
        }
        MskL2Task();
        fldSetSnowActive();
        mapObjTask();
        mapTakaraTask();
        mapWallObjTask(fldGlobalBase.focusX, fldGlobalBase.focusY, fldGlobalBase.focusZ);
        fldGlobalBase.word78 = mapCallSetPlayerTargetData6(fldGlobalBase.focusX, fldGlobalBase.focusZ);
        mapProcSequenceSub(fldGlobalBase.amSBlock, fldGlobalBase.nameU);
        mapAM_MaskOpen_ID(fldGlobalBase.amSBlock, fldGlobalBase.nameU, fldGlobalBase.word78);
        goto map_tail;
    case 0xF:
        fldGlobalBase.word308 = fldGlobalBase.amSBlock;
        fldGlobalBase.word30C = fldGlobalBase.nameU;
        if (evtBitCheck(2101)) {
            goto f_encounter_key;
        }
        if ((fldGlobalBase.amSBlock != 3 || fldGlobalBase.nameU != 2) && mapBadMafuHajiki()) {
        bad_mafu:
            fldReleaseUnitPad();
            fldAutoMapSeqFlags.stage = 39;
            fldPlayerMotReset();
            ProcMskSub_B();
            return 0;
        }
        if (!fldGlobalBase.word334 && fldEncountTestFlag(0x40) && fldGlobalBase.amSBlock != 12 && (fldGlobalBase.amSBlock != 3 || fldGlobalBase.nameU != 2) && dds3PadButtonState[0x20] < 0 && !fldGlobalBase.noInpPlCnt) {
        menu_key:
            fldAutoMapSeqFlags.stage = 21;
            fldReleaseUnitPad();
            fldPlayerMotReset();
            dds3FadeOut(0, 0, 0, 8);
            fldProcSequenceSub6(1);
            p2sdPlaySE(17, 127, 63);
            ProcMskSub_B();
            return 0;
        }
    f_encounter_key:
        if (fldEncountTestFlag(0x40) && fldGlobalBase.amSBlock != 12 && (fldGlobalBase.amSBlock != 3 || fldGlobalBase.nameU != 2) && fldGlobalBase.word240 != 2 && dds3PadButtonState[0x22] < 0 && !fldGlobalBase.noInpPlCnt) {
            p2sdPlaySE(17, 127, 63);
            fldAutoMapSeqFlags.stage = 19;
            fldReleaseUnitPad();
            fldPlayerMotReset();
            dds3FadeOut(0, 0, 0, 8);
        }
        fldPlayerCalc();
        fldGlobalBase.wordF8 = mapCallSetPlayerTargetData3(fldGlobalBase.focusX, fldGlobalBase.focusZ);
        mapUnitSetLight(playerBasic, fldGlobalBase.wordF8, 10);
        fldGlobalBase.encountState = mapProcSequenceSub5(fldGlobalBase.focusX, fldGlobalBase.focusZ);
        if (fldGlobalBase.word35C) {
            mapCallSetPlayerTargetData4();
        }
        mapProcSequenceSub11();
        result = fldSaveLabel();
        if (result == -1) {
            return result;
        }
        MskL2Task();
        fldSetSnowActive();
        if (raidoBasic && fldGlobalBase.word190 == fldGlobalBase.amSBlock) {
            if (fldGlobalBase.word194 == fldGlobalBase.nameU) {
                mapUnitSetLight(raidoBasic, mapGetFloorLight(fldGlobalBase.word194, fldGlobalBase.float184, fldGlobalBase.float18C), 0);
                if (mapBigClipChkF(fldGlobalBase.cutIdx, fldGlobalBase.float184, fldGlobalBase.float188, fldGlobalBase.float18C)) {
                    dds3ObjectBaseOffFlag(raidoBasic, 1);
                } else {
                    dds3ObjectBaseOnFlag(raidoBasic, 1);
                }
            } else if (fldGlobalBase.word194 == gSubFloorID) {
                mapUnitSetLight(raidoBasic, mapGetFloorLight(fldGlobalBase.word194, fldGlobalBase.float184, fldGlobalBase.float18C), 0);
                if (fldGlobalBase.amSBlock != 8) {
                    if (mapBigClipChkF(fldGlobalBase.cutIdx, fldGlobalBase.float184, fldGlobalBase.float188, fldGlobalBase.float18C)) {
                        dds3ObjectBaseOffFlag(raidoBasic, 1);
                    } else {
                        dds3ObjectBaseOnFlag(raidoBasic, 1);
                    }
                }
            }
        }
        if (aiboBasic && fldGlobalBase.word1A8 == fldGlobalBase.amSBlock) {
            if (fldGlobalBase.word1AC == fldGlobalBase.nameU) {
                mapUnitSetLight(aiboBasic, mapGetFloorLight(fldGlobalBase.word1AC, aiboBasic->poseset->pos.x, aiboBasic->poseset->pos.z), 0);
                if (mapBigClipChkF(fldGlobalBase.cutIdx, aiboBasic->poseset->pos.x, aiboBasic->poseset->pos.y, aiboBasic->poseset->pos.z)) {
                    dds3ObjectBaseOffFlag(aiboBasic, 1);
                } else {
                    dds3ObjectBaseOnFlag(aiboBasic, 1);
                }
            } else if (fldGlobalBase.word1AC == gSubFloorID) {
                mapUnitSetLight(aiboBasic, mapGetFloorLight(fldGlobalBase.word1AC, aiboBasic->poseset->pos.x, aiboBasic->poseset->pos.z), 0);
                if (fldGlobalBase.amSBlock != 8) {
                    if (mapBigClipChkF(fldGlobalBase.cutIdx, aiboBasic->poseset->pos.x, aiboBasic->poseset->pos.y, aiboBasic->poseset->pos.z)) {
                        dds3ObjectBaseOffFlag(aiboBasic, 1);
                    } else {
                        dds3ObjectBaseOnFlag(aiboBasic, 1);
                    }
                }
            }
        }
        mapObjTask();
        mapTakaraTask();
        mapWallObjTask(fldGlobalBase.focusX, fldGlobalBase.focusY, fldGlobalBase.focusZ);
        fldGlobalBase.word78 = mapCallSetPlayerTargetData6(fldGlobalBase.focusX, fldGlobalBase.focusZ);
        mapProcSequenceSub(fldGlobalBase.amSBlock, fldGlobalBase.nameU);
        mapAM_MaskOpen_ID(fldGlobalBase.amSBlock, fldGlobalBase.nameU, fldGlobalBase.word78);
    map_tail:
        if (fldGlobalBase.word344) {
            mapCtnGet();
        } else {
            mapCtnViewOff();
        }
        if (!fldEncountTestFlag(0x40)) {
            break;
        }
        if (ProcRandomEncountCheck_fld()) {
            fldGlobalBase.word300 = 12;
            fldAutoMapSeqFlags.stage = 35;
            fldBgZoomTest(8);
            fldReleaseUnitPad();
        }
        break;
    case 0x10:
    case 0x12:
        return -1;
    case 0x11:
        break;
    case 0x13:
        if (!dds3FadeCheck()) {
            fldAutoMapSeqFlags.stage = 20;
            fldDestroyTitleTask();
            dds3VisibleSceneBasic(dds3GetActiveScene(), 0);
            dds3ProcSequenceSub(0);
            evtDisableMoonDraw();
            fldGlobalBase.ifDrawOn = 0;
            mapBgDrawOnOff(0);
            mapWaitCbScreenFadeBSub(1);
            if (fldGlobalBase.summonStop) {
                fldProcSequenceSub5(fldGlobalBase.float184, fldGlobalBase.float18C, fldGlobalBase.word194, &entryPos[0], &entryPos[1]);
                fldProcSequenceSub4(fldGlobalBase.focusX, fldGlobalBase.focusZ, &entryPos2[0], &entryPos2[1]);
                fldCallFindSceneEntryData(entryPos[0], entryPos[1], entryPos2[0], entryPos2[1], fldGlobalBase.amSBlock, fldGlobalBase.nameU, 1);
            } else {
                fldProcSequenceSub4(fldGlobalBase.focusX, fldGlobalBase.focusZ, &entryPos[0], &entryPos[1]);
                fldCallFindSceneEntryData(entryPos[0], entryPos[1], 0.0f, 0.0f, fldGlobalBase.amSBlock, fldGlobalBase.nameU, 0);
            }
            if (!fldGlobalBase.word240) {
                ProcessStart_cmpMap();
            }
            dds3FadeIn2(8);
        }
        goto proc_msk_b;
    case 0x14:
        if (fldGlobalBase.word240) {
            if (fldGlobalBase.word240 != 1) {
                break;
            }
            mapCallSetOt8();
            if (dds3PadButtonState[0x21] >= 0) {
                break;
            }
            fldAutoMapSeqFlags.stage = fldGlobalBase.summonStop ? 15 : 14;
            dds3VisibleSceneBasic(dds3GetActiveScene(), 1);
            fldGlobalBase.ifDrawOn = 1;
            mapBgDrawOnOff(1);
            dds3ProcSequenceSub(1);
        } else {
            if (!ProcessCheck_cmpMap()) {
                break;
            }
            fldAutoMapSeqFlags.stage = fldGlobalBase.summonStop ? 15 : 14;
            dds3VisibleSceneBasic(dds3GetActiveScene(), 1);
            fldGlobalBase.ifDrawOn = 1;
            mapBgDrawOnOff(1);
            mapWaitCbScreenFadeBSub(0);
            dds3ProcSequenceSub(1);
        }
        goto moon_pad;
    case 0x15:
        if (!dds3FadeCheck()) {
            if (fldGlobalBase.summonStop) {
                fldProcSequenceSub5(fldGlobalBase.float184, fldGlobalBase.float18C, fldGlobalBase.word194, &entryPos[0], &entryPos[1]);
                fldProcSequenceSub4(fldGlobalBase.focusX, fldGlobalBase.focusZ, &entryPos2[0], &entryPos2[1]);
                fldCallFindSceneEntryData(entryPos[0], entryPos[1], entryPos2[0], entryPos2[1], fldGlobalBase.amSBlock, fldGlobalBase.nameU, 1);
            } else {
                fldProcSequenceSub4(fldGlobalBase.focusX, fldGlobalBase.focusZ, &entryPos[0], &entryPos[1]);
                fldCallFindSceneEntryData(entryPos[0], entryPos[1], 0.0f, 0.0f, fldGlobalBase.amSBlock, fldGlobalBase.nameU, 0);
            }
            fldAutoMapSeqFlags.stage = 22;
            fldDestroyTitleTask();
            dds3RefCameraTM();
            dds3VisibleSceneBasic(dds3GetActiveScene(), 0);
            dds3ProcSequenceSub(0);
            evtDisableMoonDraw();
            fldGlobalBase.ifDrawOn = 0;
            mapBgDrawOnOff(0);
            mapBgSetScrollReset();
            mapWaitCbScreenFadeBSub(1);
            cmpProcessStartEx_cmpMain(0);
            dds3FadeIn2(8);
        }
        goto proc_msk_b;
    case 0x16:
        if (cmpProcessCheck()) {
            break;
        }
        fldAutoMapSeqFlags.stage = 23;
        break;
    case 0x17:
        mapNakamaStockOnCheck_fld();
        if (cmpGetEndMode()) {
            fmProcTrigSequence_slMain(1);
            fldAutoMapSeqFlags.stage = 24;
            break;
        }
        badStatus = sdatCalc_GetBadStatus(&DDS3_GBWK->playerData.unit);
        if (badStatus == 6) {
            mapBadEffLoad(0, 6);
        } else if (badStatus == 11) {
            mapBadEffLoad(0, 11);
        }
        if (datCallGetStockDevilID(DDS3_GBWK->mapSummonID) != -1) {
            badStatus = sdatCalc_GetBadStatus(&DDS3_GBWK->playerData.stock[0]);
            if (badStatus == 6) {
                mapBadEffLoad(1, 6);
            } else if (badStatus == 11) {
                mapBadEffLoad(1, 11);
            }
        }
        dds3RefCameraTM2();
        fldAutoMapSeqFlags.stage = fldGlobalBase.summonStop ? 15 : 14;
        dds3VisibleSceneBasic(dds3GetActiveScene(), 1);
        dds3ProcSequenceSub(1);
        fldGlobalBase.ifDrawOn = 1;
        mapBgDrawOnOff(1);
        mapWaitCbScreenFadeBSub(0);
    moon_pad:
        evtEnableMoonDraw();
        fldRegistUnitPad();
        dds3FadeIn2(8);
        break;
    case 0x18:
        if (fmProcessChk_slMain()) {
            break;
        }
        if (sdfDelayedResrcCheck()) {
            break;
        }
        if (!slRetuenFromLoadSub()) {
            ProcSequenceData4 = 1;
            dds3VisibleSceneBasic(dds3GetActiveScene(), 0);
            dds3AdminiCallSequence(8, NULL, 0, 0);
            return -1;
        }
        dds3VisibleSceneBasic(dds3GetActiveScene(), 0);
        dds3ProcSequenceSub(0);
        evtDisableMoonDraw();
        fldGlobalBase.ifDrawOn = 0;
        mapBgDrawOnOff(0);
        mapBgSetScrollReset();
        cmpProcessStartEx_cmpMain(1);
        mapWaitCbScreenFadeBSub(1);
        dds3FadeIn2(8);
        fldAutoMapSeqFlags.stage = 22;
        break;
    case 0x19:
        fldAutoMapSeqFlags.stage = 26;
        mnuQueueTitleSoundStreamEntry(73);
        break;
    case 0x1A:
        if (sndSePlay() != 2) {
            break;
        }
        dds3FadeOut(0, 0, 0, 20);
        sndCallWaitSema();
        fldAutoMapSeqFlags.stage = 27;
        break;
    case 0x1B:
        if (dds3FadeCheck()) {
            break;
        }
        mapIdouGo_Idx(0);
        return -1;
    case 0x1C:
        mafuResult = mapBadMafuHajiki();
        if (!mapNakamaBranchTask() && !mafuResult) {
            fldAutoMapSeqFlags.stage = 15;
            fldGlobalBase.word320 = 0;
            fldRegistUnitPad();
            ProcMskSub_B();
            return 0;
        }
        fldPlayerCalc();
        goto proc_msk_b;
    case 0x1D:
        if (!mapNakamaJoinTask()) {
            fldAutoMapSeqFlags.stage = 14;
            fldRegistUnitPad();
            if (aiboBasic) {
                dds3ObjectBaseOffFlag(aiboBasic, 1);
            }
        }
        goto proc_msk_b;
    case 0x1E:
        if (fldHirukoYukaMovetask() == 1) {
            hirukoResult = fldHirukoYukaGo_Idx2_B_fld(ProcSequenceData2);
            if (hirukoResult == 2) {
                ProcSequenceData3 = hirukoResult;
                fldAutoMapSeqFlags.stage = 31;
            } else {
                ProcSequenceData3 = 2;
                fldAutoMapSeqFlags.stage = 32;
            }
        } else if (fldGlobalBase.word33C) {
            mapCallSetPlayerTargetData4();
        }
        goto proc_msk_b;
    case 0x1F:
        if (--ProcSequenceData3 < 0) {
            mapSoundKankyoSeSet(fldGlobalBase.amSBlock, fldGlobalBase.nameU, fldGlobalBase.cutIdx);
            fldRegistUnitPad();
            fldAutoMapSeqFlags.stage = fldGlobalBase.summonStop ? 15 : 14;
        }
        if (fldGlobalBase.word33C) {
            mapCallSetPlayerTargetData4();
        }
        goto proc_msk_b;
    case 0x20:
        if (--ProcSequenceData3 >= 0) {
            break;
        }
        titleResult = mapCallTitleStartFld(ProcSequenceData2);
        switch (titleResult) {
        case 60:
            fldAutoMapSeqFlags.stage = 66;
            ProcSequenceData8 = 24;
            goto case_end;
        case 61:
            fldAutoMapSeqFlags.stage = 67;
            ProcSequenceData8 = 20;
            goto case_end;
        case 63:
            fldAutoMapSeqFlags.stage = 68;
            ProcSequenceData8 = 20;
            mapProcSequenceSub9(1);
            goto case_end;
        }
        if ((u32)(titleResult - 2) >= 2) {
            return -1;
        }
        mapHamidasiCheck();
        fldPlayerForceSetPos_NoHokan(fldGlobalBase.focusX, fldGlobalBase.focusY, fldGlobalBase.focusZ);
        fldPlayerMotReset2();
        if (playerBasic) {
            mapBgSetScroll(playerBasic->poseset->pos.x, playerBasic->poseset->pos.y - 128.0f, playerBasic->poseset->pos.z, 0);
        }
        if (fldGlobalBase.bgZoomSet) {
            fldGlobalBase.bgZoomSet = 0;
            fldBgZoomTest(4);
            mapScreenReset();
            if (playerBasic) {
                mapBgSetScroll(playerBasic->poseset->pos.x, playerBasic->poseset->pos.y - 128.0f, playerBasic->poseset->pos.z, 0);
            }
            fldBgZoomTest(5);
            if (playerBasic) {
                mapBgSetScroll(playerBasic->poseset->pos.x, playerBasic->poseset->pos.y - 128.0f, playerBasic->poseset->pos.z, 0);
            }
            fldBgZoomTest(2);
        }
        if (mapNpcTblRefresh_Wait() == -1) {
            mapNpcTblRefresh();
        }
        while (mapNpcTblRefresh_Wait() == 1) {
        }
        mapMapObjectRefresh();
        if (!fldGlobalBase.summonStop) {
            mapAiboFoceSet3(fldGlobalBase.focusX, fldGlobalBase.focusY, fldGlobalBase.focusZ);
            mapAiboWakeUp(1);
            mapSummonDvFoceSet2(fldGlobalBase.focusX, fldGlobalBase.focusY, fldGlobalBase.focusZ);
            mapNakamaWakeUp(1);
        }
        if (titleResult == 2) {
            fldAutoMapSeqFlags.stage = 33;
            ProcSequenceData3 = 4;
        } else {
            ProcSequenceData3 = 2;
            fldAutoMapSeqFlags.stage = 34;
        }
    case_end:
        break;
    case 0x21:
        if (--ProcSequenceData3 < 0) {
            mapSoundKankyoSeSet(fldGlobalBase.amSBlock, fldGlobalBase.nameU, fldGlobalBase.cutIdx);
            dds3FadeIn2(15);
            fldRegistUnitPad();
            fldAutoMapSeqFlags.stage = fldGlobalBase.summonStop ? 15 : 14;
        }
        break;
    case 0x22:
        if (--ProcSequenceData3 >= 0) {
            break;
        }
        sndProcSequenceSub();
        mapCallBitCheck();
        if (!fldGlobalBase.initScript) {
            fldAutoMapSeqFlags.stage = 5;
            break;
        }
        dds3SceneBasicStartScript(dds3GetControlScene(), fldGlobalBase.initScript);
        fldAutoMapSeqFlags.stage = 4;
        break;
    case 0x23:
        if (fldGlobalBase.word300 == 12) {
            gMapRndEncLoad_Mode = 1;
            ProcSequenceData5 = sndProcSequenceSub3();
            mdlMotionPause(playerModel);
            if (!fldGlobalBase.summonStop) {
                if (summonDvModel) {
                    mdlMotionPause(summonDvModel);
                }
                if (aiboModel) {
                    mdlMotionPause(aiboModel);
                }
            }
            ProcSequenceData7 = 0;
            sndBgmLoadId_XF(sbtlProcSequenceSub(fldGlobalBase.rndEncNo));
        }
        if (fldGlobalBase.word300 == 10) {
            p2sdPlaySE(15, 127, 63);
        }
        if (fldGlobalBase.word300 < 9) {
            encDrawBokasi0();
            encDrawBokasi0();
        }
        if (fldGlobalBase.word300 > 0) {
            fldGlobalBase.word300--;
        }
        if (sndBgmStatus_XF() == 2 && fldGlobalBase.word300 <= 0) {
            dds3VisibleSceneBasic(dds3GetActiveScene(), 0);
            encStart();
            fldGlobalBase.word218 = fldGlobalBase.cutIdx;
            fldGlobalBase.word21C = fldGlobalBase.word1DC;
            fldGlobalBase.word1D8 = 0;
            dds3AdminiCallSequence(19, &ProcSequenceData, 52, 0);
            return -1;
        }
        ProcSequenceData7++;
        mapObjTask();
        break;
    case 0x24:
        if (fldGlobalBase.word2BC == 1) {
            gMapRndEncLoad_Mode = 1;
            ProcSequenceData5 = sndProcSequenceSub3();
            sndBgmLoadId_XF(sbtlProcSequenceSub(fldGlobalBase.rndEncNo));
        }
        if (fldGlobalBase.word2BC == 20) {
            fldBgZoomTest(8);
        }
        if (fldGlobalBase.word2BC == 24) {
            p2sdPlaySE(15, 127, 63);
        }
        if (fldGlobalBase.word2BC >= 28) {
            encDrawBokasi0();
            encDrawBokasi0();
        }
        if (fldGlobalBase.word2BC < 40) {
            fldGlobalBase.word2BC++;
        }
        if (sndBgmStatus_XF() != 2 || fldGlobalBase.word2BC < 40) {
            mapWallObjTask(fldGlobalBase.focusX, fldGlobalBase.focusY, fldGlobalBase.focusZ);
            break;
        }
        fldGlobalBase.word2C0 = 1;
        dds3VisibleSceneBasic(dds3GetActiveScene(), 0);
        encStart();
        fldGlobalBase.word218 = fldGlobalBase.cutIdx;
        fldGlobalBase.word21C = fldGlobalBase.word1DC;
        fldGlobalBase.word1D8 = 0;
        fldGlobalBase.word2BC = 0;
        dds3AdminiCallSequence(19, &ProcSequenceData, 52, 0);
        return -1;
    case 0x25:
        if (fldGlobalBase.word2A8 < 19) {
            if (!fldGlobalBase.mskIndex && fldGlobalBase.word35C) {
                mapIgnite(fldGlobalBase.word294, fldGlobalBase.word298, fldGlobalBase.word29C, fldGlobalBase.word2A0, fldGlobalBase.word2A4);
            } else {
                mapIfDrawActionMaster(fldGlobalBase.word294, fldGlobalBase.word298, fldGlobalBase.word29C, fldGlobalBase.word2A0, fldGlobalBase.word2A4);
            }
            fldGlobalBase.word2A8++;
        } else {
            fldPlayerMotReset3();
            fldNakamaMotReset3();
            fldAiboMotReset();
            fldAutoMapSeqFlags.stage = 38;
        }
        goto proc_msk_b;
    case 0x26:
        if (!sndSeStatus()) {
            goto proc_msk_b;
        }
        if (fldGlobalBase.summonStop && (fldGlobalBase.word148 & 0xF000) == 0x1000) {
            dds3SceneBasicStartScript(dds3GetActiveScene(), "bunri_hajiki_label");
        } else if (fldGlobalBase.summonStop && fldGlobalBase.word148 == 0x2000) {
            dds3SceneBasicStartScript(dds3GetActiveScene(), "bunri_hajiki3_label");
        } else {
            mdlSetMotionSpeed(2);
            dds3SceneBasicStartScript(dds3GetActiveScene(), fldGlobalBase.scriptName);
        }
        fldAutoMapSeqFlags.stage = fldGlobalBase.summonStop ? 15 : 14;
        goto proc_msk_b;
    case 0x27:
        mafuResult = mapBadMafuHajiki();
        if (mafuResult) {
            switch (mafuResult) {
            case 1:
                goto proc_msk_b;
            case 2:
                mapCallSetMotionSpeed();
                nextStage = 40;
                break;
            case 3:
                fldPlayerMotion_Single();
                nextStage = 41;
                break;
            case 4:
            case 6:
                nextStage = fldGlobalBase.summonStop ? 15 : 14;
                break;
            case 5:
                fldReleaseUnitPad();
                mapNakamaReturnStart();
                nextStage = 28;
                break;
            default:
                goto proc_msk_b;
            }
        } else {
            fldRegistUnitPad();
            nextStage = fldGlobalBase.summonStop ? 15 : 14;
        }
        fldAutoMapSeqFlags.stage = nextStage;
        goto proc_msk_b;
    case 0x28:
        mafuResult = mapBadMafuHajiki();
        if (mapNakamaSummonTask() || mafuResult) {
            goto proc_msk_b;
        }
        if (fldGlobalBase.word33C) {
            mapCallSetPlayerTargetData4();
        }
        fldRegistUnitPad();
        fldAutoMapSeqFlags.stage = 14;
        goto proc_msk_b;
    case 0x29:
        mafuResult = mapBadMafuHajiki();
        if (mapNakamaReturnTask() || mafuResult) {
            goto proc_msk_b;
        }
        if (fldGlobalBase.word33C) {
            mapCallSetPlayerTargetData4();
        }
        fldRegistUnitPad();
        fldAutoMapSeqFlags.stage = 14;
        goto proc_msk_b;
    case 0x2A:
        mapBadMafuHajiki();
        if (mapNakamaSearchTask_fld()) {
            goto proc_msk_b;
        }
        fldAutoMapSeqFlags.stage = fldGlobalBase.summonStop ? 15 : 14;
        if (fldGlobalBase.word33C) {
            mapCallSetPlayerTargetData4();
        }
        fldRegistUnitPad();
        goto proc_msk_b;
    case 0x2B:
        mapBadMafuHajiki();
        if (mapNakamaSpotTask_fld()) {
            goto proc_msk_b;
        }
        fldAutoMapSeqFlags.stage = fldGlobalBase.summonStop ? 15 : 14;
        fldRegistUnitPad();
        goto proc_msk_b;
    case 0x2C:
        if (mapNakamaGankTask_fld()) {
            goto proc_msk_b;
        }
        fldAutoMapSeqFlags.stage = fldGlobalBase.summonStop ? 15 : 14;
        if (fldGlobalBase.word33C) {
            mapCallSetPlayerTargetData4();
        }
        fldRegistUnitPad();
        goto proc_msk_b;
    case 0x2D:
        if (mapNakamaKoukTask()) {
            goto proc_msk_b;
        }
        fldAutoMapSeqFlags.stage = fldGlobalBase.summonStop ? 15 : 14;
        if (fldGlobalBase.word33C) {
            mapCallSetPlayerTargetData4();
        }
        fldRegistUnitPad();
        goto proc_msk_b;
    case 0x2E:
        ProcMskSub_A();
        if (!mapNakamaHakaTask()) {
            fldAutoMapSeqFlags.stage = fldGlobalBase.summonStop ? 15 : 14;
        }
        goto proc_msk_b;
    case 0x2F:
        ProcMskSub_A();
        ProcRandomEncountCheck();
        if (fldData14 == 100) {
            mapCallNakamaWakeUp4();
            fldAutoMapSeqFlags.stage = 46;
        }
        goto proc_msk_b;
    case 0x30:
        ProcMskSub_A();
        if (!mapNakamaReiTask()) {
            fldAutoMapSeqFlags.stage = fldGlobalBase.summonStop ? 15 : 14;
        }
        goto proc_msk_b;
    case 0x31:
        ProcMskSub_A();
        ProcRandomEncountCheck();
        if (fldData14 == 100) {
            mapNakamaJoinStart();
            fldAutoMapSeqFlags.stage = 48;
        }
        goto proc_msk_b;
    case 0x32:
        ProcMskSub_A();
        if (!mapNakamaChikaraTask()) {
            fldAutoMapSeqFlags.stage = fldGlobalBase.summonStop ? 15 : 14;
        }
        goto proc_msk_b;
    case 0x33:
        ProcMskSub_A();
        ProcRandomEncountCheck();
        if (fldData14 == 100) {
            mapCallNakamaWakeUp5();
            fldAutoMapSeqFlags.stage = 50;
        }
        goto proc_msk_b;
    case 0x34:
        ProcMskSub_A();
        if (!mapNakamaDokusinTask()) {
            fldAutoMapSeqFlags.stage = fldGlobalBase.summonStop ? 15 : 14;
        }
        goto proc_msk_b;
    case 0x35:
        ProcMskSub_A();
        ProcRandomEncountCheck();
        if (fldData14 == 100) {
            mapCallNakamaWakeUp6();
            fldAutoMapSeqFlags.stage = 52;
        }
        goto proc_msk_b;
    case 0x36:
        ProcMskSub_A();
        if (!mapNakamaIroTask()) {
            fldAutoMapSeqFlags.stage = fldGlobalBase.summonStop ? 15 : 14;
        }
        goto proc_msk_b;
    case 0x37:
        ProcMskSub_A();
        ProcRandomEncountCheck();
        if (fldData14 == 100) {
            fldCallNakamaWakeUp();
            fldAutoMapSeqFlags.stage = 54;
        }
        goto proc_msk_b;
    case 0x38:
        ProcMskSub_A();
        if (!mapNakamaHikouTask()) {
            fldAutoMapSeqFlags.stage = fldGlobalBase.summonStop ? 15 : 14;
        }
        goto proc_msk_b;
    case 0x39:
        ProcMskSub_A();
        ProcRandomEncountCheck();
        if (fldData14 == 100) {
            fldCallNakamaWakeUp2();
            fldAutoMapSeqFlags.stage = 56;
        }
        goto proc_msk_b;
    case 0x3A:
        ProcMskSub_A();
        if (!mapNakamaSinnTask()) {
            fldAutoMapSeqFlags.stage = fldGlobalBase.summonStop ? 15 : 14;
        }
        goto proc_msk_b;
    case 0x3B:
        ProcMskSub_A();
        ProcRandomEncountCheck();
        if (fldData14 == 100) {
            fldFastTravelStart();
            fldAutoMapSeqFlags.stage = 58;
        }
        goto proc_msk_b;
    case 0x3C:
        if (mapNakamaSpotTask()) {
            goto proc_msk_b;
        }
        fldAutoMapSeqFlags.stage = fldGlobalBase.summonStop ? 15 : 14;
        if (fldGlobalBase.word33C) {
            mapCallSetPlayerTargetData4();
        }
        fldRegistUnitPad();
        goto proc_msk_b;
    case 0x3D:
        if (mapNakamaGankTask()) {
            goto proc_msk_b;
        }
        fldAutoMapSeqFlags.stage = fldGlobalBase.summonStop ? 15 : 14;
        if (fldGlobalBase.word33C) {
            mapCallSetPlayerTargetData4();
        }
        fldRegistUnitPad();
        goto proc_msk_b;
    case 0x3E:
        if (mapNakamaTakumiTask()) {
            goto proc_msk_b;
        }
        fldAutoMapSeqFlags.stage = fldGlobalBase.summonStop ? 15 : 14;
        if (fldGlobalBase.word33C) {
            mapCallSetPlayerTargetData4();
        }
        fldRegistUnitPad();
        goto proc_msk_b;
    case 0x3F:
        if (!scrSearchScriptName("bunri_hajiki_label")) {
            fldReleaseUnitPad();
            dds3FadeOut(0, 0, 0, 30);
            fldAutoMapSeqFlags.stage = 64;
        }
        goto proc_msk_b;
    case 0x40:
        if (!dds3FadeCheck()) {
            mapNakamaHajiki1_PosSet();
            fldAutoMapSeqFlags.stage = 65;
            dds3FadeIn(0, 0, 0, 30);
            fldRegistUnitPad();
        }
        goto proc_msk_b;
    case 0x41:
        if (dds3FadeCheck()) {
            goto proc_msk_b;
        }
        fldAutoMapSeqFlags.stage = fldGlobalBase.summonStop ? 15 : 14;
        fldRegistUnitPad();
        goto proc_msk_b;
    case 0x42:
        if (ProcSequenceData8 > 0) {
            ProcSequenceData8--;
        }
        if (ProcSequenceData8) {
            break;
        }
        adminData = 0;
        dds3AdminiCallSequence(11, &adminData, 4, 0);
        scrAllReleaseScript();
        fldBgTexClear();
        return -1;
    case 0x43:
        if (ProcSequenceData8 > 0) {
            ProcSequenceData8--;
        }
        if (ProcSequenceData8) {
            break;
        }
        adminData = 1;
        dds3AdminiCallSequence(11, &adminData, 4, 0);
        scrAllReleaseScript();
        fldBgTexClear();
        return -1;
    case 0x44:
        if (ProcSequenceData8 > 0) {
            ProcSequenceData8--;
        }
        if (ProcSequenceData8) {
            break;
        }
        mapProcSequenceSub9(0);
        dds3AdminiCallSequence(17, NULL, 0, 0);
        scrAllReleaseScript();
        fldBgTexClear();
        return -1;
    default:
        break;
    }
    if (fldAutoMapSeqFlags.stage < 0 || (fldAutoMapSeqFlags.stage >= 3 && (fldAutoMapSeqFlags.stage >= 25 || fldAutoMapSeqFlags.stage < 20))) {
        if (!effProcSequenceSub()) {
            if (playerBasic) {
                mapBgSetScroll(playerBasic->poseset->pos.x, playerBasic->poseset->pos.y - 128.0f, playerBasic->poseset->pos.z, 0);
            }
        }
        if (cameraBasic) {
            mapCutSetCam(cameraBasic, 0);
        }
    }
    if (fldGlobalBase.word32C > 0) {
        if (fldGlobalBase.word32C < 3) {
            mapProcSequenceSub6();
            mapCallSetOt7();
        } else if (fldGlobalBase.word32C == 3) {
            mapProcSequenceSub7();
        }
    }
    return 0;
}
#else
INCLUDE_RODATA(const s32, "field/fldProcess", ProcSequenceTable);

INCLUDE_ASM(const s32, "field/fldProcess", ProcSequence);
#endif
#else
INCLUDE_ASM(const s32, "field/fldProcess", ProcSequence);
#endif

INCLUDE_ASM(const s32, "field/fldProcess", IsFldKeyFree);

void fldAutoMapSeqSetFlag(u32 flag)
{
    fldAutoMapSeqFlags.flags |= flag;
}

void fldAutoMapSeqClearFlag(u32 flag)
{
    fldAutoMapSeqFlags.flags &= ~flag;
}

s32 fldAutoMapSeqTestFlag(u32 flag)
{
    return (fldAutoMapSeqFlags.flags & flag) != 0;
}

void fldEncountSetFlag(u32 flag)
{
    fldEncountFlags |= flag;
}

void fldEncountClearFlag(u32 flag)
{
    fldEncountFlags &= ~flag;
}

s32 fldEncountTestFlag(u32 flag)
{
    return (fldEncountFlags & flag) != 0;
}

INCLUDE_ASM(const s32, "field/fldProcess", fldProcTrigSequence);

INCLUDE_ASM(const s32, "field/fldProcess", fldProcShutDown);

INCLUDE_ASM(const s32, "field/fldProcess", fldCallDelayedResrcCheck);

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "field/fldProcess", ProcSequenceStrBunriHajikiLabel);
#else
INCLUDE_RODATA(const s32, "field/fldProcess", ProcSequenceStrBunriHajikiLabel2);
#endif

INCLUDE_RODATA(const s32, "field/fldProcess", fldStrBunriHajiki3Label);

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "field/fldProcess", jtbl_00447820);
#else
INCLUDE_RODATA(const s32, "field/fldProcess", EveHitTask_fldStrGitaiHajikiLabel);

INCLUDE_RODATA(const s32, "field/fldProcess", ProcSequenceStrGitaiHajiki2Label);

INCLUDE_RODATA(const s32, "field/fldProcess", jtbl_004AA5B0);

INCLUDE_RODATA(const s32, "field/fldProcess", jtbl_004AA6F0);
#endif

INCLUDE_RODATA(const s32, "field/fldProcess", fldStrFldProcSequence);

INCLUDE_RODATA(const s32, "field/fldProcess", fldProcDrawName);

INCLUDE_ASM(const s32, "field/fldProcess", fldTest2);

void fldExitEventSub(void) {
}

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "field/fldProcess", mapReturnFromShop);
#else
INCLUDE_ASM(const s32, "field/fldProcess", fldCallBtlResetRndEncCount);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "field/fldProcess", fldCallAdminiCallSequence);
#else
INCLUDE_ASM(const s32, "field/fldProcess", mapReturnFromRndEncBattle);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "field/fldProcess", mselStartGame);
#else
INCLUDE_ASM(const s32, "field/fldProcess", fldCallStrlen);

INCLUDE_ASM(const s32, "field/fldProcess", mapReturnFromGohmaden);

INCLUDE_ASM(const s32, "field/fldProcess", fldCallBitCheck);
#endif

INCLUDE_ASM(const s32, "field/fldProcess", fldLeaf7);

INCLUDE_ASM(const s32, "field/fldProcess", autoNakamaToRaidou);

INCLUDE_ASM(const s32, "field/fldProcess", mapCallMapNext);

#ifdef VERSION_RAIDOU1
void mapCallMap(s32 map, s32 flor, s32 point, char *label)
{
    sdfVector4_t rotation;
    sdfVector4_t axis = { 0.0f, 1.0f, 0.0f, 1.0f };
    f32 pos[4];

    if (map == 0) {
        map = mapCallLastMap;
        flor = mapCallLastFloor;
        point = mapCallLastPoint;
        label = mapCallLastLabel;
    } else if (playerBasic == NULL || fldGlobalBase.amSBlock != map || fldGlobalBase.nameU != flor) {
        goto reload;
    } else {
        fldBgZoomTest(4);
        mapObjPointGet(point, &pos[0], &pos[1], &pos[2], &pos[3]);
        fldGlobalBase.focusX = pos[0];
        fldGlobalBase.focusZ = pos[2];
        fldGlobalBase.focusY = mapGetY(fldGlobalBase.focusX, fldGlobalBase.focusZ);
        mapHamidasiCheck();
        mapCutSet_From_Pos(fldGlobalBase.focusX, fldGlobalBase.focusY, fldGlobalBase.focusZ);
        fldPlayerForceSetPos_NoHokan(fldGlobalBase.focusX, fldGlobalBase.focusY, fldGlobalBase.focusZ);
        if (fldGlobalBase.summonStop == 0) {
            mapAiboFoceSet3(fldGlobalBase.focusX, fldGlobalBase.focusY, fldGlobalBase.focusZ);
            mapAiboWakeUp(1);
            mapSummonDvFoceSet2(fldGlobalBase.focusX, fldGlobalBase.focusY, fldGlobalBase.focusZ);
            mapNakamaWakeUp(1);
        }
        fldGlobalBase.floatC0 = pos[3];
        fldGlobalBase.floatC4 = pos[3];
        VU0_LOAD_VF(vf10, &axis);
        FromRotation(fldGlobalBase.floatC0 * 3.14f / 180.0f);
        VU0_STORE_VF(vf10, &rotation);
        dds3SetRotPoseSet(playerBasic, &rotation);
        if (mapNpcTblRefresh_Wait() == -1) {
            mapNpcTblRefresh();
        }
        while (mapNpcTblRefresh_Wait() == 1) {
        }
        mapMapObjectRefresh();
        fldRegistUnitPad();
        if (label != NULL) {
            fldPlayerMotReset();
            dds3SceneBasicStartScript(dds3GetActiveScene(), label);
        }
        return;
    }
reload:
    fldGlobalBase.mapCallMode = 1;
    fldGlobalBase.word1D8 = 0;
    mapSetSceneParam(&mapCallScriptBuf, map, flor, point);
    if (label != NULL) {
        sprintf(mapCallScriptBuf.FoceScript, mapCallMapFmt, label);
    }
    dds3AdminiCallSequence(7, &mapCallScriptBuf, sizeof(mapCallScriptBuf), 0);
    scrAllReleaseScript();
}
#else
INCLUDE_ASM(const s32, "field/fldProcess", mapCallMap);
#endif

INCLUDE_ASM(const s32, "field/fldProcess", fldHardSelect1Sub);

INCLUDE_ASM(const s32, "field/fldProcess", EveHitTask);

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "field/fldProcess", fldSaveLabel);
#else
INCLUDE_RODATA(const s32, "field/fldProcess", mapCallMapTable2);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "field/fldProcess", MskL2Task);
#else
INCLUDE_ASM(const s32, "field/fldProcess", EveHitTask_fld);
INCLUDE_SDATA(const s32, "field/fldProcess", ProcSequenceData11);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "field/fldProcess", ProcSequenceData6);
#else
INCLUDE_SDATA(const s32, "field/fldProcess", ProcSequenceData12);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "field/fldProcess", ProcSequenceData7);
#else
INCLUDE_SDATA(const s32, "field/fldProcess", ProcSequenceData13);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "field/fldProcess", ProcSequenceData8);
#else
INCLUDE_SDATA(const s32, "field/fldProcess", ProcSequenceData14);
#endif

INCLUDE_SDATA(const s32, "field/fldProcess", mapCallMapFmt);

INCLUDE_SDATA(const s32, "field/fldProcess", fldProcShutDownData);

INCLUDE_SDATA(const s32, "field/fldProcess", mapCallLastMap);

INCLUDE_SDATA(const s32, "field/fldProcess", mapCallLastFloor);

INCLUDE_SDATA(const s32, "field/fldProcess", mapCallLastPoint);

INCLUDE_SDATA(const s32, "field/fldProcess", mapCallLastLabel);

INCLUDE_SDATA(const s32, "field/fldProcess", fldAutoMapSeqFlags);

INCLUDE_SDATA(const s32, "field/fldProcess", nbCallGetIntPara8Data);

#ifdef VERSION_RAIDOU1
u32 fldEncountFlags = 0;

INCLUDE_RODATA(const s32, "field/fldProcess", fldStrBadMafuHajikiLabel);

INCLUDE_RODATA(const s32, "field/fldProcess", fldStrMwallDevilonlyLabel);

INCLUDE_RODATA(const s32, "field/fldProcess", fldStrMap13WarpLabel);

INCLUDE_RODATA(const s32, "field/fldProcess", fldStrRunwarpLabel);

INCLUDE_RODATA(const s32, "field/fldProcess", fldStrHasigoLabel);

INCLUDE_RODATA(const s32, "field/fldProcess", fldStrDexitwarpLabel);

INCLUDE_RODATA(const s32, "field/fldProcess", fldStrElvLabel);

INCLUDE_RODATA(const s32, "field/fldProcess", jtbl_00447B30);

#endif
