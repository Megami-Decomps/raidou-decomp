#include "common.h"
#include "include_asm.h"
#include "code_0015D490.h"
#include "code_r1_0010DF68.h"
#include "code_r1_002C8188.h"
#include "dds3KernelCore.h"
#include "dds3ObjectBase.h"
#include "dds3Randomize.h"
#include "EventBit.h"
#include "fld.h"
#include "fldProcess.h"
#include "mapData.h"
#include "model.h"
#include "motion.h"
#include "sdfSound.h"
#include "code_r1_002CA3F0.h"
#include "dds3KernelDraw.h"
#include "sdfMath.h"
#include "code_r1_001CC1D0.h"
#include "code_r1_002CAFD8.h"
#include "frFont.h"
#include "dat.h"
#include "effManager.h"
#include "field.h"
#include "mdlManager.h"
#include "newdata.h"
#include "ps2asm.h"
#include "sdfModel.h"

INCLUDE_ASM(const s32, "code_0015D490", mapLeaf);

void mapCallMemset2(void) {
    sdfVector4_t pos = { 0.0f, 0.0f, 0.0f, 1.0f };
    mdlObj_t *playerMdl;
    mdlObj_t *summonMdl;
    dds3Basic_t *player;
    dds3Basic_t *summon;
    s32 bad;
    s32 id;

    if (mapData121 != 0) {
        return;
    }
    if (fldGlobalBase.summonStop == 0) {
        playerMdl = playerModel;
        summonMdl = summonDvModel;
        player = playerBasic;
    } else {
        playerMdl = raidoModel;
        summonMdl = summonDvModel;
        player = raidoBasic;
    }
    summon = summonDvBasic;
    if (mapData117 != NULL && mapData118 == 0) {
        bad = sdatCalc_GetBadStatus(&DDS3_GBWK->playerData.unit);
        if ((bad == 6 || bad == 11) && !dds3ObjectBaseCheckFlag(player, 1)) {
            sdfModelGetMarkPos_sdfModel(playerMdl->inner, 2);
            SDF_VU0_STOREVECTOR(&pos);
            dds3EffectSetPosition(mapData117, &pos);
            dds3EffectDraw_dds3Effect(mapData117);
        }
    }
    if (mapCallMemset2Data != NULL && mapData120 == 0) {
        id = datCallGetStockDevilID(DDS3_GBWK->mapSummonID);
        if (id != -1) {
            bad = sdatCalc_GetBadStatus(&DDS3_GBWK->playerData.stock[id]);
            if ((bad == 6 || bad == 11) && !dds3ObjectBaseCheckFlag(summon, 1) && (mdlGetColor(summonMdl) & 0xFF000000)) {
                sdfModelGetMarkPos_sdfModel(summonMdl->inner, 2);
                SDF_VU0_STOREVECTOR(&pos);
                dds3EffectSetPosition(mapCallMemset2Data, &pos);
                dds3EffectDraw_dds3Effect(mapCallMemset2Data);
            }
        }
    }
}

INCLUDE_ASM(const s32, "code_0015D490", mapEffMskMaruDraw);

INCLUDE_ASM(const s32, "code_0015D490", mapEffMskFlishDraw);

INCLUDE_ASM(const s32, "code_0015D490", mapCallSin2);

INCLUDE_ASM(const s32, "code_0015D490", mapCallSin3);

INCLUDE_ASM(const s32, "code_0015D490", mapCallSin4);

INCLUDE_ASM(const s32, "code_0015D490", mapCallSin5);

INCLUDE_ASM(const s32, "code_0015D490", mapCallSin6);

INCLUDE_ASM(const s32, "code_0015D490", mapCallSin7);

INCLUDE_ASM(const s32, "code_0015D490", mapCallSin8);

INCLUDE_ASM(const s32, "code_0015D490", mapCallSin9);

INCLUDE_ASM(const s32, "code_0015D490", mapCallSin10);

INCLUDE_ASM(const s32, "code_0015D490", fldCallSin4);

INCLUDE_ASM(const s32, "code_0015D490", fldCallSetOt);

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

INCLUDE_ASM(const s32, "code_0015D490", fldLeaf23);

INCLUDE_ASM(const s32, "code_0015D490", fldCallSin5);

INCLUDE_ASM(const s32, "code_0015D490", fldCallSin6);

INCLUDE_ASM(const s32, "code_0015D490", fldCallSin7);

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

/* body: matched in raidou @ 9ab2342: src/shared/code_r1_0019D3A8.c:827, found, same function 1.00 (words 9/9, calls 1.00, shape 1.00) */
s32 fldIsMapTitleActive(void) {
    return dds3SearchProcessName(fldTitleTaskName) != 0;
}

INCLUDE_ASM(const s32, "code_0015D490", fldCallLinkSprite);

INCLUDE_ASM(const s32, "code_0015D490", mapCallLinkSprite);

void mapTitleSkip(void) {
    if (fldCallSetOt4Data2 == 2) {
        fldCallSetOt4Data = 0;
        fldCallSetOt4Data2 = 3;
    }
}

INCLUDE_ASM(const s32, "code_0015D490", mapProcSequenceSub9);

INCLUDE_ASM(const s32, "code_0015D490", fldCallSetOt4);

INCLUDE_ASM(const s32, "code_0015D490", fldReleaseTitleTextures);

INCLUDE_ASM(const s32, "code_0015D490", mapTitleStart);

INCLUDE_ASM(const s32, "code_0015D490", mapTitleStart_fld);

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_001411F0.c:4799, found, same function 0.67 (words 15/15, calls 0.00, shape 1.00) */
void fldDestroyTitleTask(void) {
    if (fldIsMapTitleActive() != 0) {
        dds3KillProcessName(fldTitleTaskName, 1);
    }
}

INCLUDE_ASM(const s32, "code_0015D490", fldProcSequenceSub6);

void fldSe_Init(void) {
    s32 i;

    for (i = 0; i < 64; i++) {
        mapSoundKankyoSeSetWork[i].active = 0;
        mapSoundKankyoSeSetWork[i].se = 0;
        mapSoundKankyoSeSetWork[i].vol = 0;
        mapSoundKankyoSeSetWork[i].update = 0;
        mapSoundKankyoSeSetWork[i].state = 0;
    }
    mapData131 = 0;
    mapSoundKankyoTaskSubData = 100;
}

INCLUDE_ASM(const s32, "code_0015D490", fldCallSoundKankyoSeSet);

INCLUDE_ASM(const s32, "code_0015D490", mapBgTexLoad);

/* NON_MATCHING: 10 of 23 words match.  */
#ifdef NON_MATCHING
void mapSoundKankyoTaskSub(void) {
    s32 vol;

    switch (mapData131) {
    case -1:
        vol = mapSoundKankyoTaskSubData - 2;
        if (vol < 0) {
            vol = 0;
        }
        mapSoundKankyoTaskSubData = vol;
        break;
    case 1:
        vol = mapSoundKankyoTaskSubData + 2;
        if (vol > 100) {
            vol = 100;
        }
        mapSoundKankyoTaskSubData = vol;
        mapData131 = vol != 100;
        break;
    }
}
#else
INCLUDE_ASM(const s32, "code_0015D490", mapSoundKankyoTaskSub);
#endif

void mapSoundKankyoSeSet(s32 map, s32 floor, s32 cut) {
    s32 i;
    s32 j;
    s32 idx;
    s32 se;
    s32 vol;

    for (i = 0; i < 64; i++) {
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
                p2sdVolume2(se, vol * mapSoundKankyoTaskSubData / 100);
                continue;
            }
            p2sdPlaySE_Vol_Loop(mapSoundKankyoSeSetWork[i].se);
            mapSoundKankyoSeSetWork[i].active = 0;
            continue;
        }
        if (idx == -1 || se == 0 || vol == 0) {
            continue;
        }
        if (evtBitCheck(184) || evtBitCheck(206)) {
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
            if (map == 4) {
                if (mapData4[i].se == 50) {
                    continue;
                }
                if (mapData4[i].se == 51) {
                    continue;
                }
                if (mapData4[i].se == 52) {
                    continue;
                }
                if (mapData4[i].se == 53) {
                    continue;
                }
                if (mapData4[i].se == 54) {
                    continue;
                }
            }
            if (map == 5) {
                if (mapData4[i].se == 48) {
                    continue;
                }
                if (mapData4[i].se == 49) {
                    continue;
                }
            }
        }
        if ((evtBitCheck(65) && !evtBitCheck(96)) || evtBitCheck(184)) {
            if (map == 3) {
                if (mapData4[i].se == 49) {
                    continue;
                }
                if (mapData4[i].se == 51) {
                    continue;
                }
                if (mapData4[i].se == 52) {
                    continue;
                }
            }
        }
        if (evtBitCheck(184) && map == 3 && floor == 0 && mapData4[i].se == 50) {
            continue;
        }
        if ((evtBitCheck(131) && !evtBitCheck(145)) || evtBitCheck(184)) {
            if (map == 7) {
                if (mapData4[i].se == 48) {
                    continue;
                }
                if (mapData4[i].se == 49) {
                    continue;
                }
                if (mapData4[i].se == 50) {
                    continue;
                }
            }
        }
        mapSoundKankyoSeSetWork[i].active = 1;
        mapSoundKankyoSeSetWork[i].se = mapData4[i].se;
        mapSoundKankyoSeSetWork[i].vol = vol;
        mapSoundKankyoSeSetWork[i].update = 0;
        mapSoundKankyoSeSetWork[i].state = mapData4[i].waittime;
        if (mapSoundKankyoSeSetWork[i].state == 0) {
            p2sdPlaySE_Vol(se, vol * mapSoundKankyoTaskSubData / 100);
        }
    }
}

void mapSoundKankyoTask(s32 map, s32 floor, s32 cut) {
    s32 i;
    s32 j;
    s32 idx;
    s32 se;
    s32 vol;

    mapSoundKankyoTaskSub();
    for (i = 0; i < 64; i++) {
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
        if (mapSoundKankyoSeSetWork[i].active == 0) {
            continue;
        }
        if (evtBitCheck(184)) {
            if (map == 1) {
                if (se == 51) {
                    continue;
                }
                if (se == 52) {
                    continue;
                }
                if (se == 53) {
                    continue;
                }
            }
            if (map == 2) {
                if (se == 51) {
                    continue;
                }
                if (se == 52) {
                    continue;
                }
            }
            if (map == 4) {
                if (se == 50) {
                    continue;
                }
                if (se == 51) {
                    continue;
                }
                if (se == 52) {
                    continue;
                }
                if (se == 53) {
                    continue;
                }
                if (se == 54) {
                    continue;
                }
            }
            if (map == 5) {
                if (se == 48) {
                    continue;
                }
                if (se == 49) {
                    continue;
                }
            }
        }
        if ((evtBitCheck(65) && !evtBitCheck(96)) || evtBitCheck(184)) {
            if (map == 3) {
                if (se == 49) {
                    continue;
                }
                if (se == 51) {
                    continue;
                }
                if (se == 52) {
                    continue;
                }
            }
        }
        if ((evtBitCheck(131) && !evtBitCheck(145)) || evtBitCheck(184)) {
            if (map == 7) {
                if (se == 48) {
                    continue;
                }
                if (se == 49) {
                    continue;
                }
                if (se == 50) {
                    continue;
                }
            }
        }
        if (idx == -1 || se == 0 || vol == 0) {
            continue;
        }
        if (mapSoundKankyoSeSetWork[i].state > 0) {
            if (--mapSoundKankyoSeSetWork[i].state > 0) {
                continue;
            }
            mapSoundKankyoSeSetWork[i].state = mapData4[i].waittime;
            p2sdPlaySE_Vol(se, vol * mapSoundKankyoTaskSubData / 100);
        } else if (mapSoundKankyoSeSetWork[i].state == 0 && mapData131 != 0) {
            p2sdVolume2(se, vol * mapSoundKankyoTaskSubData / 100);
        }
    }
}

void mapCallPlaySEVolLoop(void) {
    s32 i;

    for (i = 0; i < 64; i++) {
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

INCLUDE_ASM(const s32, "code_0015D490", mapFileLoadSub8);

void mapCallPlaySEVolLoop2(void) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (mapFileLoadSub8Work[i].active != 0) {
            p2sdPlaySE_Vol_Loop(mapFileLoadSub8Work[i].se);
        }
    }
    mapProcSequenceSub10();
}

INCLUDE_ASM(const s32, "code_0015D490", mapSoundMoveSeVol);

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

INCLUDE_ASM(const s32, "code_0015D490", mapSoundMoveSeFade0);

INCLUDE_ASM(const s32, "code_0015D490", mapLeaf23);

INCLUDE_ASM(const s32, "code_0015D490", mapLeaf24);

s32 p2sdCallBitCheck(s32 map, s32 floor) {
    s32 bgm;

    if (map >= 35) {
        return 8;
    }
    if (map == 13 && floor >= 16) {
        bgm = p2sdCallBitCheckData[floor];
    } else {
        bgm = p2sdData[map][floor];
    }
    if (evtBitCheck(184) && !evtBitCheck(208) && bgm == 8) {
        bgm = 15;
    }
    return bgm;
}

s32 p2sdSoundSmgLoadChkSub(s32 map) {
    return p2sdSoundSmgLoadChkSubData[map];
}

void p2sdPlaySE_Vol(s32 se, s32 vol) {
    p2sdPlaySE(p2sdData2 + se, vol, 63);
}

INCLUDE_ASM(const s32, "code_0015D490", p2sdVolume2);

INCLUDE_ASM(const s32, "code_0015D490", p2sdPlaySE_Vol_Loop);

void mapSoundSmgLoadChk(s32 map) {
    s32 addr = p2sdSoundSmgLoadChkSub(map);

    fldBgmSmgAddr = addr;
    p2sdData2 = addr;
    fldBgmLoadStep = 0;
}

/* body: matched in smt3 @ 9401f79eb: src/smt3/field/fldSe.c:734, found, same function 1.00 (words 32/32, calls 1.00, shape 1.00) */
s32 mapSoundSmgLoadChk_fld(void) {
    switch (fldBgmLoadStep) {
    case 0:
        if (p2sdLoadCheckMidi(fldBgmSmgAddr) == 0) {
            p2sdLoadSMGFile(fldBgmSmgAddr);
            fldBgmLoadStep = fldBgmLoadStep + 1;
        } else {
            fldBgmLoadStep = fldBgmLoadStep + 2;
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

INCLUDE_ASM(const s32, "code_0015D490", mapNakamaCashLoadEndCheckSub);

INCLUDE_ASM(const s32, "code_0015D490", sndCallLoadCheckMidi);

INCLUDE_ASM(const s32, "code_0015D490", sndSeLoadId);

INCLUDE_ASM(const s32, "code_0015D490", sndNakamaJoinTaskSub);

void datInitBossPress(void) {
    s32 i;

    for (i = 0; i < 16; i++) {
        datWork7[i].timer = -1;
        datWork7[i].vol = 127;
    }
}

INCLUDE_ASM(const s32, "code_0015D490", datNakamaMotionSub);

/* NON_MATCHING: 19 of 29 words match.  */
#ifdef NON_MATCHING
void mapSeNakamaDelayTask(void) {
    mapSeDelay_t *delay = datWork7;
    s32 i;

    for (i = 0; i < 16; i++, delay++) {
        if (delay->timer >= 0) {
            delay->timer--;
            if (delay->timer < 0) {
                p2sdPlaySE(mapData129 + i, delay->vol, 63);
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_0015D490", mapSeNakamaDelayTask);
#endif

/* NON_MATCHING: 14 of 22 words match.  */
#ifdef NON_MATCHING
void mapSeWallHit(s32 hit) {
    if (mapSeWallHitData == 1 && hit == 0) {
        mapSeWallHitData = 0;
    } else if (mapSeWallHitData == 0 && hit == 1) {
        mapSeWallHitData = 1;
        p2sdPlaySE_Vol(33, 127);
    }
}
#else
INCLUDE_ASM(const s32, "code_0015D490", mapSeWallHit);
#endif

void mapTakaraOpen(void) {
    takara_openanm_objid = -1;
}

INCLUDE_ASM(const s32, "code_0015D490", mapTakaraOpen_fld);

INCLUDE_ASM(const s32, "code_0015D490", mapRefDDS3GBWK5);

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

void DecisionFeedIn(void) {
    fldDrwZBuf2 = effLoadIndexedResource(DecisionFeedInStrMapAll, DecisionFeedInStrMapMeiSpr, 0);
}

void mapProcShutDownSub(void) {
    if (fldDrwZBuf2 != NULL) {
        effReleaseTextureHandlesAndResetSlots(fldDrwZBuf2);
        effDestroyResourceSlotSet(fldDrwZBuf2);
        fldDrwZBuf2 = NULL;
    }
}

/* NON_MATCHING: 152 of 207 words match.  */
#ifdef NON_MATCHING
void mapIfDrawES(void) {
    u32 palette[4];
    s32 pct;
    s32 cnt;
    s32 max;
    s32 area;
    s32 level;
    s32 r;
    s32 b;
    s32 g;
    f32 t;
    u32 color;

    if (fldDrwZBuf2 == NULL) {
        return;
    }
    pct = 0;
    eglSetOt(103);
    eglLinkAlpha(1);
    eglLinkTest(1, 0, 128, 3, 0, 0, 1, 1);
    area = fldGlobalBase.nameD;
    cnt = sbtlGetRndEncCnt();
    max = nbGetEncountMax(area);
    if (max < cnt) {
        cnt = max;
    }
    if (max > 0) {
        pct = cnt * 100 / max;
        level = 0;
        if (pct > 50) {
            level = 1;
        }
        if (pct > 90) {
            level = 2;
        }
    } else {
        level = 4;
    }
    r = 128;
    eglLinkAlpha(0);
    itfDrawGridWithResolvedSlot(6864, 2736, 0, 32, fldDrwZBuf2, 3, 103);
    b = 128;
    g = 128;
    if (level == 4) {
        r = 0;
        g = 50;
        b = 170;
        mapIfDrawESData2 += 0.02f;
    }
    if (level == 0) {
        t = pct * 0.02f;
        if (t > 1.0f) {
            t = 1.0f;
        }
        mapIfDrawESData2 += 0.03f;
        r = (1.0f - t) * 190.0f + t * 200.0f;
        b = (1.0f - t) * 60.0f + t * 20.0f;
        g = (1.0f - t) * 160.0f + t * 90.0f;
    }
    if (level == 1) {
        t = (pct - 50) * 0.025f;
        if (t > 1.0f) {
            t = 1.0f;
        }
        mapIfDrawESData2 += 0.05f;
        r = (1.0f - t) * 200.0f + t * 160.0f;
        b = (1.0f - t) * 20.0f + t * 40.0f;
        g = (1.0f - t) * 90.0f + t * 35.0f;
    }
    if (level == 2) {
        r = 160;
        g = 35;
        b = 40;
        mapIfDrawESData2 += 0.07f;
    }
    color = ((s32)(sdfSin(mapIfDrawESData2) * 25.0f) + 65) | (r << 24) | (g << 16 | b << 8);
    palette[0] = color;
    palette[1] = color;
    palette[2] = color;
    palette[3] = color;
    effDrawSpriteWithCornerColors(7184, 2816, 0, palette, 32, fldDrwZBuf2, 4, 103);
    eglLinkAlpha(0);
}
#else
INCLUDE_ASM(const s32, "code_0015D490", mapIfDrawES);
#endif

INCLUDE_ASM(const s32, "code_0015D490", mapIfDrawActionWin2);

/* NON_MATCHING: 89 of 167 words match.  */
#ifdef NON_MATCHING
void mapCallEnableMode(s32 map, s32 floor, s32 cut) {
    frFontHandle_t *font;
    u32 width;
    u32 color;
    s32 i;
    s32 right = 496;

    if (fldGlobalBase.previousFloor != -1) {
        frEnableMode(1);
        font = frSetFontString(mapCallEnableModeData2[fldGlobalBase.previousFloor], 0, 0, 0, NULL);
        frDisableMode(1);
        frSetFontSpace(font, 1);
        width = frGetStrWidth2(font);
        frSetDrawPosition(font, (right - width) * 16, 3232);
        frSetFontColor(font, 0xF0DC6480);
        fldDrwZBuf2->descriptions[2].rect[3] = width + 39;
        itfDrawGridWithResolvedSlot((right - width - 23) * 16, 3184, 0, 32, fldDrwZBuf2, 2, 103);
        frDrawFontEx2(font, 0, 103);
        frDestroyFontRequestAfter(font);
    } else {
        for (i = 0; i < 160; i++) {
            if (mapData[i].map == map && mapData[i].floor == floor && mapData[i].cut == cut) {
                color = 0xA09DC380;
                if (mapData[i].visited != 0) {
                    color = 0xF0DC6480;
                }
                frEnableMode(1);
                font = frSetFontString(mapData[i].name, 0, 0, 0, NULL);
                frDisableMode(1);
                frSetFontSpace(font, -1);
                width = frGetStrWidth2(font);
                frSetDrawPosition(font, (right - width) * 16, 3232);
                frSetFontColor(font, color);
                fldDrwZBuf2->descriptions[2].rect[3] = width + 39;
                itfDrawGridWithResolvedSlot((right - width - 23) * 16, 3184, 0, 32, fldDrwZBuf2, 2, 103);
                frDrawFontEx2(font, 0, 103);
                frDestroyFontRequestAfter(font);
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_0015D490", mapCallEnableMode);
#endif

INCLUDE_ASM(const s32, "code_0015D490", mapIfDrawMskSub);
