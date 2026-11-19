#include "common.h"
#include "code_0015D490.h"
#include "code_r1_0010DF68.h"
#include "dds3ObjectBase.h"
#include "dds3Randomize.h"
#include "fld.h"
#include "fldProcess.h"
#include "mapData.h"
#include "motion.h"
#include "sdfSound.h"
#include "dat.h"
#include "sdfModel.h"
#include "mdlManager.h"
#include "ps2asm.h"
#include "effManager.h"
#include "dds3KernelCore.h"
#include "sdfTex.h"
#include "code_r1_002C8188.h"

INCLUDE_ASM(const s32, "code_00169A40", mapLeaf4);

/* NON_MATCHING: 95 of 102 words match.  */
#ifdef NON_MATCHING
void mapBadEffDrawTask_fld(void) {
    sdfVector4_t pos = { 0.0f, 0.0f, 0.0f, 1.0f };
    mdlObj_t *summonMdl;
    mdlObj_t *playerMdl;
    dds3Basic_t *player;
    dds3Basic_t *summon;
    s32 bad;
    s32 id;

    if (mapData121 != 0) {
        return;
    }
    if (fldGlobalBase.nakamaMode == 0) {
        playerMdl = playerModel;
        summonMdl = summonDvModel;
        player = playerBasic;
    } else {
        playerMdl = raidoModel;
        summonMdl = summonDvModel;
        player = raidoBasic;
    }
    summon = summonDvBasic;
    if (eMapBadEff_R != NULL && mapData118 == 0) {
        bad = sdatCalc_GetBadStatus(&DDS3_GBWK->playerData.unit);
        if (bad == 6 && !dds3ObjectBaseCheckFlag(player, 1)) {
            sdfModelGetMarkPos_sdfModel(playerMdl->inner, 2);
            SDF_VU0_STOREVECTOR(&pos);
            dds3EffectSetPosition(eMapBadEff_R, &pos);
            dds3EffectDraw_dds3Effect(eMapBadEff_R);
        }
    }
    if (eMapBadEff_N != NULL && mapData120 == 0) {
        id = datGetStockDevilIndex(DDS3_GBWK->mapSummonID);
        if (id != -1) {
            bad = sdatCalc_GetBadStatus(&DDS3_GBWK->playerData.stock[id]);
            if (bad == 6 && !dds3ObjectBaseCheckFlag(summon, 1) && (mdlGetColor(summonMdl) & 0xFF000000)) {
                sdfModelGetMarkPos_sdfModel(summonMdl->inner, 2);
                SDF_VU0_STOREVECTOR(&pos);
                dds3EffectSetPosition(eMapBadEff_N, &pos);
                dds3EffectDraw_dds3Effect(eMapBadEff_N);
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_00169A40", mapBadEffDrawTask_fld);
#endif

INCLUDE_ASM(const s32, "code_00169A40", mapEffMskMaruDraw);

INCLUDE_ASM(const s32, "code_00169A40", mapEffMskFlishDraw);

INCLUDE_ASM(const s32, "code_00169A40", mapCallSin2);

INCLUDE_ASM(const s32, "code_00169A40", mapCallSin3);

INCLUDE_ASM(const s32, "code_00169A40", mapCallSin4);

INCLUDE_ASM(const s32, "code_00169A40", mapCallSin5);

INCLUDE_ASM(const s32, "code_00169A40", mapCallSin6);

INCLUDE_ASM(const s32, "code_00169A40", mapCallSin7);

INCLUDE_ASM(const s32, "code_00169A40", mapCallSin8);

INCLUDE_ASM(const s32, "code_00169A40", mapCallSin9);

INCLUDE_ASM(const s32, "code_00169A40", mapCallSin10);

INCLUDE_ASM(const s32, "code_00169A40", fldCallSin4);

INCLUDE_ASM(const s32, "code_00169A40", mapCallSin);

INCLUDE_ASM(const s32, "code_00169A40", mapCallSetOt2);

void fldCallGetRandA3(void) {
    s32 i;
    s32 j;

    for (i = 0; i < 32; i++) {
        for (j = 0; j < 32; j++) {
            fldWork7[i][j] = ((f32)((dds3GetRandA(NULL) >> 4) & 0xFF) - 128.0f) / 10000.0f;
            fldCallSetOtWork[i][j] = ((f32)((dds3GetRandA(NULL) >> 4) & 0xFF) - 128.0f) / 10000.0f;
        }
    }
}

INCLUDE_ASM(const s32, "code_00169A40", fldLeaf23);

INCLUDE_ASM(const s32, "code_00169A40", fldCallSin5);

INCLUDE_ASM(const s32, "code_00169A40", fldCallSin6);

INCLUDE_ASM(const s32, "code_00169A40", fldCallSin7);

INCLUDE_ASM(const s32, "code_00169A40", fldCallGetRandA);

INCLUDE_ASM(const s32, "code_00169A40", fldCallGetRandA2);

INCLUDE_ASM(const s32, "code_00169A40", fldCallSin);

INCLUDE_ASM(const s32, "code_00169A40", fldLeaf3);

INCLUDE_ASM(const s32, "code_00169A40", fldLeaf4);

INCLUDE_ASM(const s32, "code_00169A40", fldCallDrwZLoad);

void fldCallGetRandFloatA(mefMistParticle_t *particle) {
    mefMistSystem_t *mist = pMapMoya;
    s32 i;

    particle->fExist = 1;
    switch (mist->scroll) {
    case 0:
        particle->pos[0] = dds3GetRandFloatA(NULL) * 672.0f - 160.0f;
        particle->pos[1] = dds3GetRandFloatA(NULL) * 608.0f - 160.0f;
        break;
    case 1:
        particle->pos[0] = dds3GetRandFloatA(NULL) * 1184.0f - 160.0f;
        particle->pos[1] = dds3GetRandFloatA(NULL) * 608.0f - 160.0f;
        break;
    case 2:
        particle->pos[0] = dds3GetRandFloatA(NULL) * 672.0f - 160.0f;
        particle->pos[1] = dds3GetRandFloatA(NULL) * 1056.0f - 160.0f;
        break;
    case 3:
        particle->pos[0] = dds3GetRandFloatA(NULL) * 1184.0f - 160.0f;
        particle->pos[1] = dds3GetRandFloatA(NULL) * 1056.0f - 160.0f;
        break;
    }
    particle->velocity[0] = (dds3GetRandFloatA(NULL) - 0.5f) * 0.5f;
    particle->velocity[1] = (dds3GetRandFloatA(NULL) - 0.5f) * 0.5f;
    particle->lifeCnt = 0;
    particle->deathCnt = dds3GetRandIntA(NULL, 988) + 512;
    for (i = 0; i < 4; i++) {
        particle->rgba[i][3] = dds3GetRandIntA(NULL, 255);
        particle->rgba[i][2] = dds3GetRandIntA(NULL, 255);
        particle->rgba[i][1] = dds3GetRandIntA(NULL, 255);
        particle->rgba[i][0] = (particle->rgba[i][3] + particle->rgba[i][2] + particle->rgba[i][1]) / 3;
    }
    mist->num++;
}

s32 fldIsMapTitleActive(void) {
    return dds3SearchProcessName(fldTitleTaskName) != 0;
}

INCLUDE_ASM(const s32, "code_00169A40", fldCallLinkSprite);

INCLUDE_ASM(const s32, "code_00169A40", mapCallLinkSprite);

void mapTitleSkip(void) {
    if (fldCallSetOt4Data2 == 2) {
        fldCallSetOt4Data = 0;
        fldCallSetOt4Data2 = 3;
    }
}

INCLUDE_ASM(const s32, "code_00169A40", mapProcSequenceSub9);

INCLUDE_ASM(const s32, "code_00169A40", fldCallSetOt4);

void fldReleaseTitleTextures(dds3ProcessID_t *task) {
    if (mapCallLinkSpriteData != NULL) {
        sdfTexFreeDelayed(mapCallLinkSpriteData);
        mapCallLinkSpriteData = NULL;
    }
    if (fldData30 != NULL) {
        sdfTexFreeDelayed(fldData30);
        fldData30 = NULL;
    }
    fldCallSetOt4Data = 0;
    fldCallSetOt4Data3 = 0;
    fldCallSetOt4Data2 = 99;
    fldData31 = 0;
    fldCallSetOt4Data4 = 0;
}

void mapTitleStart(struct sdfMat_t *file) {
    fldReleaseTitleTextures(file);
}

INCLUDE_ASM(const s32, "code_00169A40", mapMnameTmx);

void fldDestroyTitleTask(void) {
    if (fldIsMapTitleActive() != 0) {
        dds3KillProcessName(fldTitleTaskName, 1);
    }
}

void fldProcSequenceSub6(s32 mode) {
    fldData31 = mode;
}

void fldSe_Init(void) {
    s32 i;

    for (i = 0; i < 128; i++) {
        mapSoundKankyoSeSetWork[i].active = 0;
        mapSoundKankyoSeSetWork[i].se = 0;
        mapSoundKankyoSeSetWork[i].vol = 0;
        mapSoundKankyoSeSetWork[i].update = 0;
        mapSoundKankyoSeSetWork[i].state = 0;
    }
    mapData131 = 0;
    mapSoundKankyoTaskSubData = 100;
}

INCLUDE_ASM(const s32, "code_00169A40", fldCallSoundKankyoSeSet);

INCLUDE_ASM(const s32, "code_00169A40", mapBgTexLoad);

INCLUDE_ASM(const s32, "code_00169A40", mapSoundKankyoTaskSub);

INCLUDE_ASM(const s32, "code_00169A40", mapSoundKankyoSeSet);

INCLUDE_ASM(const s32, "code_00169A40", mapCallBitCheck6);

void mapCallPlaySEVolLoop(void) {
    s32 i;

    for (i = 0; i < 128; i++) {
        if (mapSoundKankyoSeSetWork[i].active != 0) {
            p2sdPlaySE_Vol_Loop(mapSoundKankyoSeSetWork[i].se);
        }
    }
    fldSe_Init();
}

void mapProcSequenceSub10(void) {
    s32 i;

    for (i = 0; i < 8; i++) {
        mapFileLoadSub8Work[i].active = 0;
        mapFileLoadSub8Work[i].se = 0;
        mapFileLoadSub8Work[i].vol = 0;
        mapFileLoadSub8Work[i].update = 0;
        mapFileLoadSub8Work[i].state = 0;
    }
}

INCLUDE_ASM(const s32, "code_00169A40", mapFileLoadSub8);

void mapCallPlaySEVolLoop2(void) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (mapFileLoadSub8Work[i].active != 0) {
            p2sdPlaySE_Vol_Loop(mapFileLoadSub8Work[i].se);
        }
    }
    mapProcSequenceSub10();
}

INCLUDE_ASM(const s32, "code_00169A40", mapSoundMoveSeVol);

void mapSoundMoveSeTask(void) {
    s32 i;
    s32 vol;

    for (i = 0; i < 8; i++) {
        if (mapFileLoadSub8Work[i].active != 0 && mapFileLoadSub8Work[i].update != 0) {
            mapFileLoadSub8Work[i].update = 0;
            if (mapFileLoadSub8Work[i].vol > 0) {
                vol = mapFileLoadSub8Work[i].vol * mapSoundKankyoTaskSubData / 100;
                if (mapFileLoadSub8Work[i].state != 0) {
                    p2sdVolume2(mapFileLoadSub8Work[i].se, vol);
                } else {
                    p2sdPlaySE_Vol(mapFileLoadSub8Work[i].se, vol);
                    mapFileLoadSub8Work[i].state = 1;
                }
            } else {
                p2sdPlaySE_Vol_Loop(mapFileLoadSub8Work[i].se);
                mapFileLoadSub8Work[i].state = 0;
            }
        }
    }
}

void mapSoundMoveSeFade0(void) {
    s32 i;
    s32 vol;

    for (i = 0; i < 8; i++) {
        if (mapFileLoadSub8Work[i].active != 0) {
            if (mapFileLoadSub8Work[i].vol > 0) {
                vol = mapFileLoadSub8Work[i].vol * mapSoundKankyoTaskSubData / 100;
                if (mapFileLoadSub8Work[i].state != 0) {
                    p2sdVolume2(mapFileLoadSub8Work[i].se, vol);
                } else {
                    p2sdPlaySE_Vol(mapFileLoadSub8Work[i].se, vol);
                    mapFileLoadSub8Work[i].state = 1;
                }
            } else {
                p2sdPlaySE_Vol_Loop(mapFileLoadSub8Work[i].se);
                mapFileLoadSub8Work[i].state = 0;
            }
        }
    }
}

INCLUDE_ASM(const s32, "code_00169A40", mapLeaf23);

INCLUDE_ASM(const s32, "code_00169A40", mapLeaf24);

INCLUDE_ASM(const s32, "code_00169A40", mapCallBitCheck7);

INCLUDE_ASM(const s32, "code_00169A40", p2sdProcSequenceSub);

s32 p2sdSoundSmgLoadChkSub(s32 map) {
    return p2sdSoundSmgLoadChkSubData[map];
}

void p2sdPlaySE_Vol(s32 se, s32 vol) {
    p2sdPlaySE(p2sdData2 + se, vol, 63);
}

INCLUDE_ASM(const s32, "code_00169A40", p2sdVolume2);

void p2sdPlaySE_Vol_Loop(s32 se)
{
    p2sdStopSe(p2sdData2 + se);
}

void mapSoundSmgLoadChk(s32 map) {
    s32 addr = p2sdSoundSmgLoadChkSub(map);

    fldBgmSmgAddr = addr;
    p2sdData2 = addr;
    fldBgmLoadStep = 0;
}

INCLUDE_ASM(const s32, "code_00169A40", mapCallLoadCheckMidi);

INCLUDE_ASM(const s32, "code_00169A40", mapNakamaCashLoadEndCheckSub);

INCLUDE_ASM(const s32, "code_00169A40", mapSoundNakamaSmgLoadhk);

INCLUDE_ASM(const s32, "code_00169A40", mapNakamaChikaraTaskSub2);

INCLUDE_ASM(const s32, "code_00169A40", sndNakamaJoinTaskSub);

void datInitBossPress(void) {
    s32 i;

    for (i = 0; i < 16; i++) {
        datWork7[i].timer = -1;
        datWork7[i].vol = 127;
    }
}

INCLUDE_ASM(const s32, "code_00169A40", datNakamaMotionSub);

INCLUDE_ASM(const s32, "code_00169A40", mapSeNakamaDelayTask);

void mapSeWallHit(s32 hit)
{
    if (mapSeWallHitData == 1 && hit == 0) {
        mapSeWallHitData = 0;
    } else if (mapSeWallHitData == 0 && hit == 1) {
        /* The loop keeps ee-gcc from making the call a sibling call, as in Raidou 1. */
        do {
            mapSeWallHitData = 1;
            p2sdPlaySE(0x10024, 127, 63);
        } while (0);
    }
}

void mapTakaraOpen(void) {
    takara_openanm_objid = -1;
}

INCLUDE_ASM(const s32, "code_00169A40", mapNowAnmRe);

INCLUDE_ASM(const s32, "code_00169A40", mapRefDDS3GBWK6);

void mapTakaraTask(void) {
    mdlObj_t *model;

    if (takara_openanm_objid != -1) {
        datDebugPrintf(mapStrTakaraOpenanmObjid, takara_openanm_objid);
        model = dds3ObjectBaseGetModelHandle(mapObjGetBasic(takara_openanm_objid));
        if (model->first->stat == SDF_MOTION_STAT_NOCUT) {
            mapObjMotionCng(takara_openanm_objid, 2, 0);
            takara_openanm_objid = -1;
        }
    }
}

INCLUDE_ASM(const s32, "code_00169A40", mapCallPartyDamageMap);

INCLUDE_ASM(const s32, "code_00169A40", mapCallPlaySE);

struct mapUnloadRequestSubWork_t *mapUnloadRequestSub(void) {
    return &mapUnloadRequestSubWork;
}

INCLUDE_ASM(const s32, "code_00169A40", mapAll2);

void mapProcShutDownSub(void)
{
    struct mapUnloadRequestSubWork_t *work = mapUnloadRequestSub();

    if (work->fieldC4 != NULL) {
        effReleaseTextureHandlesAndResetSlots(work->fieldC4);
        effDestroyResourceSlotSet(work->fieldC4);
        work->fieldC4 = NULL;
    }
}

INCLUDE_ASM(const s32, "code_00169A40", mapLeaf5);

INCLUDE_ASM(const s32, "code_00169A40", mapCallBtlGetRndEncCnt);

INCLUDE_ASM(const s32, "code_00169A40", mapCallSetOt3);

INCLUDE_ASM(const s32, "code_00169A40", mapIsFldKeyFreeSub2);

INCLUDE_ASM(const s32, "code_00169A40", mapIfDrawActionWin2);

INCLUDE_ASM(const s32, "code_00169A40", mapCallSetOt4);

INCLUDE_ASM(const s32, "code_00169A40", mapFunc4);

INCLUDE_ASM(const s32, "code_00169A40", mapLeaf6);

INCLUDE_ASM(const s32, "code_00169A40", mapLeaf7);

INCLUDE_ASM(const s32, "code_00169A40", mapCallPreCalcStrWidth);

void mapProcSequenceSub11(void) {
    mapData133 = 0;
}

INCLUDE_RODATA(const s32, "code_00169A40", mapStrTakaraOpenanmObjid);

