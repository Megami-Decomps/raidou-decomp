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
#include "EventBit.h"
#include "frFont.h"
#include "dds3KernelDraw.h"
#include "code_r1_002CAFD8.h"
#include "code_r1_002CA3F0.h"
#include "sdfMath.h"

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

void fldCallLinkSprite(u32 color) {
    s32 alpha = color >> 24;

    if (fldData30 != NULL) {
        color = (color & 0xFFFFFF) | ((alpha * 110 / 128) << 24);
        eglLinkSprite(128, 96, 256, 256, 0, 0, 256, 256, color, color, color, color, fldData30);
    }
}

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

void fldCallSoundKankyoSeSet(s32 on) {
    if (on == 0) {
        mapSoundKankyoTaskSubData = 100;
        mapData131 = 0;
        mapSoundKankyoSeSet(fldGlobalBase.amSBlock, fldGlobalBase.nameU, fldGlobalBase.cutIdx);
        mapSoundMoveSeFade0();
    } else {
        mapData131 = 1;
    }
}

void mapBgTexLoad(s32 on) {
    if (on == 0) {
        mapData131 = 0;
        mapSoundKankyoTaskSubData = 0;
        mapSoundKankyoSeSet(fldGlobalBase.amSBlock, fldGlobalBase.nameU, fldGlobalBase.cutIdx);
        mapSoundMoveSeFade0();
    } else {
        mapData131 = -1;
    }
}

INCLUDE_ASM(const s32, "code_00169A40", mapSoundKankyoTaskSub);

/* NON_MATCHING: 223 of 379 words match.  */
#ifdef NON_MATCHING
void mapSoundKankyoSeSet(s32 map, s32 floor, s32 cut) {
    s32 playSe[8];
    s32 playIdx[8];
    s32 playVol[8];
    s32 stopSe[8];
    s32 stopIdx[8];
    s32 volSe[8];
    s32 volIdx[8];
    s32 volVol[8];
    s32 playNum;
    s32 stopNum;
    s32 volNum;
    s32 i;
    s32 j;
    s32 idx;
    s32 se;
    s32 vol;
    s32 found;

    playNum = 0;
    volNum = 0;
    stopNum = 0;
    for (i = 0; i < 8; i++) {
        playSe[i] = 0;
        playIdx[i] = 0;
        playVol[i] = 0;
        stopSe[i] = 0;
        stopIdx[i] = 0;
        volSe[i] = 0;
        volIdx[i] = 0;
        volVol[i] = 0;
    }
    for (i = 0; i < 128; i++) {
        idx = -1;
        vol = 0;
        se = 0;
        if (mapData4[i].map == map) {
            for (j = 0; j < 32; j++) {
                if (mapData4[i].cutvol[j].cut == cut) {
                    idx = j;
                    vol = mapData4[i].cutvol[j].vol;
                    se = mapData4[i].se;
                    break;
                }
            }
        }
        if (mapSoundKankyoSeSetWork[i].active != 0) {
            if (idx != -1 && se != 0 && vol != 0) {
                volSe[volNum] = se;
                volIdx[volNum] = i;
                volVol[volNum] = vol;
                volNum++;
                continue;
            }
            stopSe[stopNum] = mapSoundKankyoSeSetWork[i].se;
            stopIdx[stopNum] = i;
            stopNum++;
            continue;
        }
        if (idx == -1 || se == 0 || vol == 0) {
            continue;
        }
        if (evtBitCheck(6020)) {
            if (map == 1) {
                if (mapData4[i].se == 51) {
                    continue;
                }
                if (mapData4[i].se == 52) {
                    continue;
                }
                if (mapData4[i].se == 53) {
                    continue;
                }
            }
            if (map == 2) {
                if (mapData4[i].se == 51) {
                    continue;
                }
                if (mapData4[i].se == 52) {
                    continue;
                }
            }
            if (map == 3) {
                if (mapData4[i].se == 49) {
                    continue;
                }
                if (mapData4[i].se == 50) {
                    continue;
                }
                if (mapData4[i].se == 51) {
                    continue;
                }
            }
            if (map == 4) {
                if (mapData4[i].se == 52) {
                    continue;
                }
                if (mapData4[i].se == 53) {
                    continue;
                }
            }
            if (map == 7) {
                if (mapData4[i].se == 50) {
                    continue;
                }
                if (mapData4[i].se == 51) {
                    continue;
                }
            }
            if (map == 18) {
                if (mapData4[i].se == 48) {
                    continue;
                }
            }
            if (map == 29) {
                if (mapData4[i].se == 48) {
                    continue;
                }
            }
        }
        playSe[playNum] = se;
        playIdx[playNum] = i;
        playVol[playNum] = vol;
        playNum++;
    }
    for (i = 0; i < playNum; i++) {
        found = 0;
        for (j = 0; j < stopNum; j++) {
            if (stopSe[j] == playSe[i]) {
                found = 1;
                break;
            }
        }
        idx = playIdx[i];
        vol = playVol[i];
        se = playSe[i];
        mapSoundKankyoSeSetWork[idx].active = 1;
        mapSoundKankyoSeSetWork[idx].se = mapData4[idx].se;
        mapSoundKankyoSeSetWork[idx].vol = vol;
        mapSoundKankyoSeSetWork[idx].update = 0;
        mapSoundKankyoSeSetWork[idx].state = mapData4[idx].waittime;
        if (mapSoundKankyoSeSetWork[idx].state == 0) {
            vol = vol * mapSoundKankyoTaskSubData / 100;
            if (found) {
                p2sdVolume2(se, vol);
            } else {
                p2sdPlaySE_Vol(se, vol);
            }
        }
    }
    for (i = 0; i < volNum; i++) {
        p2sdVolume2(volSe[i], volVol[i] * mapSoundKankyoTaskSubData / 100);
    }
    for (i = 0; i < stopNum; i++) {
        found = 0;
        for (j = 0; j < playNum; j++) {
            if (playSe[j] == stopSe[i]) {
                found = 1;
                break;
            }
        }
        for (j = 0; j < volNum; j++) {
            if (volSe[j] == stopSe[i]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            p2sdPlaySE_Vol_Loop(stopSe[i]);
        }
        mapSoundKankyoSeSetWork[stopIdx[i]].active = 0;
    }
}
#else
INCLUDE_ASM(const s32, "code_00169A40", mapSoundKankyoSeSet);
#endif

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

/* NON_MATCHING: 2 of 11 words match.  */
#ifdef NON_MATCHING
s32 p2sdProcSequenceSub(s32 id, u32 idx) {
    s32 result = 8;

    if (id < 48) {
        result = mapCallBitCheck7(id, idx);
    }
    return result;
}
#else
INCLUDE_ASM(const s32, "code_00169A40", p2sdProcSequenceSub);
#endif

s32 p2sdSoundSmgLoadChkSub(s32 map) {
    return p2sdSoundSmgLoadChkSubData[map];
}

void p2sdPlaySE_Vol(s32 se, s32 vol) {
    p2sdPlaySE(p2sdData2 + se, vol, 63);
}

void p2sdVolume2(s32 se, s32 vol) {
    sdfCallSndSendSyncCmd2(p2sdData2 + se, vol);
}

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

s32 mapCallLoadCheckMidi(void) {
    switch (fldBgmLoadStep) {
    case 0:
        if (p2sdLoadCheckMidi(fldBgmSmgAddr) == 0) {
            p2sdLoadSMGFile(fldBgmSmgAddr);
            fldBgmLoadStep = fldBgmLoadStep + 1;
        } else {
            fldBgmLoadStep = -1;
            return 1;
        }
        break;
    case 1:
        if (p2sdLoadCheckMidi(fldBgmSmgAddr) == 1) {
            fldBgmLoadStep = -1;
            return 1;
        }
        break;
    default:
        fldBgmLoadStep = -1;
        return 1;
    }
    return 0;
}

void mapNakamaCashLoadEndCheckSub(s32 id) {
    fldBgmLoadStep = 0;
    mapData129 = fldBgmSmgAddr = (id << 16) + 0x4000000;
}

s32 mapSoundNakamaSmgLoadhk(void) {
    switch (fldBgmLoadStep) {
    case 0:
        if (p2sdLoadCheckMidi(fldBgmSmgAddr) == 0) {
            p2sdLoadSMGFile(fldBgmSmgAddr);
            fldBgmLoadStep = fldBgmLoadStep + 1;
        } else {
            fldBgmLoadStep = -1;
            return 1;
        }
        break;
    case 1:
        if (p2sdLoadCheckMidi(fldBgmSmgAddr) == 1) {
            fldBgmLoadStep = fldBgmLoadStep + 1;
        }
        break;
    default:
        fldBgmLoadStep = -1;
        return 1;
    }
    return 0;
}

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

/* NON_MATCHING: 11 of 26 words match.  */
#ifdef NON_MATCHING
s32 mapRefDDS3GBWK6(s32 id) {
    if (id > 256) {
        id -= 256;
        if ((DDS3_GBWK->unk4900[id >> 3] >> (id & 7)) & 1) {
            return 1;
        }
        return 0;
    }
    if ((DDS3_GBWK->unk4900[(id >> 3) + 16] >> (id & 7)) & 1) {
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_00169A40", mapRefDDS3GBWK6);
#endif

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

/* NON_MATCHING: 20 of 26 words match.  */
#ifdef NON_MATCHING
void mapAll2(void) {
    struct mapUnloadRequestSubWork_t *work = mapUnloadRequestSub();
    struct sprHandle_t *spr = effLoadIndexedResource(mapStrMapAll2, mapStrMapMeiSpr, 0);

    work->fieldD8 = 1;
    work->fieldC4 = spr;
    work->fieldC0 = 4;
    work->fieldCC = 128;
    work->fieldC8 = 0;
    work->fieldD0 = 0;
    work->fieldD4 = 0;
}
#else
INCLUDE_ASM(const s32, "code_00169A40", mapAll2);
#endif

void mapProcShutDownSub(void)
{
    struct mapUnloadRequestSubWork_t *work = mapUnloadRequestSub();

    if (work->fieldC4 != NULL) {
        effReleaseTextureHandlesAndResetSlots(work->fieldC4);
        effDestroyResourceSlotSet(work->fieldC4);
        work->fieldC4 = NULL;
    }
}

void mapLeaf5(u8 *dst, u8 *src0, u8 *src1, f32 rate1, f32 rate0) {
    dst[0] = (u32)(src0[0] * rate0 + src1[0] * rate1);
    dst[1] = (u32)(src0[1] * rate0 + src1[1] * rate1);
    dst[2] = (u32)(src0[2] * rate0 + src1[2] * rate1);
}

INCLUDE_ASM(const s32, "code_00169A40", mapCallBtlGetRndEncCnt);

void mapCallSetOt3(struct mapUnloadRequestSubWork_t *work) {
    u32 rgba[4];
    u8 color[4];
    u32 col;

    eglSetOt(0x68);
    eglLinkAlpha(1);
    eglLinkTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    eglLinkAlpha(0);
    itfDrawGridWithResolvedSlot(6864, 2736, 0, 32, work->fieldC4, 3, 0x68);
    mapCallBtlGetRndEncCnt(work, color);
    col = ((s32)(sdfSin(work->fieldD0) * 25.0f) + 65) | (color[2] << 8) | (color[1] << 16) | (color[0] << 24);
    rgba[0] = col;
    rgba[1] = col;
    rgba[2] = col;
    rgba[3] = col;
    effDrawSpriteWithCornerColors(7184, 2816, 0, rgba, 32, work->fieldC4, 4, 0x68);
    eglLinkAlpha(0);
}

void mapIsFldKeyFreeSub2(void) {
    struct mapUnloadRequestSubWork_t *work = mapUnloadRequestSub();

    if (work->fieldC4 != NULL) {
        mapCallSetOt3(work);
    }
}

void mapIfDrawActionWin2(s32 x, s32 y, u32 color, const char *str) {
    frFontHandle_t *font;

    font = frwGetSmallFontFRQ2(x << 4, y << 3, 0, color, str, NULL);
    frDrawFont(font);
    frDestroyFontRequestAfter(font);
}

INCLUDE_ASM(const s32, "code_00169A40", mapCallSetOt4);

INCLUDE_ASM(const s32, "code_00169A40", mapFunc4);

INCLUDE_ASM(const s32, "code_00169A40", mapLeaf6);

INCLUDE_ASM(const s32, "code_00169A40", mapLeaf7);

INCLUDE_ASM(const s32, "code_00169A40", mapCallPreCalcStrWidth);

void mapProcSequenceSub11(void) {
    mapData133 = 0;
}

INCLUDE_RODATA(const s32, "code_00169A40", mapStrTakaraOpenanmObjid);

