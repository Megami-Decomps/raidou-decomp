#include "common.h"
#include "fld.h"
#include "fldProcess.h"
#include "mapData.h"
#include "ctn.h"
#include "neko.h"
#include "lmap.h"
#include "dds3WorldBasic.h"
#include "dds3SceneBasic.h"
#include "dds3ObjectBase.h"
#include "code_r1_00128378.h"
#include "evtUnitManager.h"
#include "sdfSound.h"
#include "dds3KernelDraw_0010A750.h"
#include "dds3UnitObjectBasic.h"
#include "dds3Randomize.h"
#include "fldFileResolver.h"
#include "fldDraw.h"
#include "EventBit.h"
#include "evtStage.h"
#include "dds3KernelMain.h"
#include "dds3KernelDraw.h"
#include "mapLbWork.h"
#include "fldDbg.h"
#include "sdfAspect.h"

INCLUDE_ASM(const s32, "code_00175908", mapDrinkClearMsg);

INCLUDE_ASM(const s32, "code_00175908", mapMoudokuClearMsg);

INCLUDE_ASM(const s32, "code_00175908", mapRiberaMaClearMsg);

INCLUDE_ASM(const s32, "code_00175908", mapRiberaMaClear);

INCLUDE_ASM(const s32, "code_00175908", mapRiberaMaStart);

INCLUDE_ASM(const s32, "code_00175908", mapRefDDS3GBWK10);

INCLUDE_ASM(const s32, "code_00175908", mapEsutoMaClearMsg);

INCLUDE_ASM(const s32, "code_00175908", mapEsutoMaClear);

INCLUDE_ASM(const s32, "code_00175908", mapEsutoMaStart);

INCLUDE_ASM(const s32, "code_00175908", mapRefDDS3GBWK11);

INCLUDE_ASM(const s32, "code_00175908", mapRiberaMa2ClearMsg);

INCLUDE_ASM(const s32, "code_00175908", mapRiberaMa2Clear);

INCLUDE_ASM(const s32, "code_00175908", mapRiberaMa2Start);

INCLUDE_ASM(const s32, "code_00175908", mapRefDDS3GBWK12);

INCLUDE_ASM(const s32, "code_00175908", mapEsutoMa2ClearMsg);

INCLUDE_ASM(const s32, "code_00175908", mapEsutoMa2Clear);

INCLUDE_ASM(const s32, "code_00175908", mapEsutoMa2Start);

INCLUDE_ASM(const s32, "code_00175908", mapRefDDS3GBWK13);

INCLUDE_ASM(const s32, "code_00175908", mapRefDDS3GBWK14);

INCLUDE_RODATA(const s32, "code_00175908", mapStrPlNlRnd10);

INCLUDE_ASM(const s32, "code_00175908", mapPlNlRnd);

INCLUDE_ASM(const s32, "code_00175908", mapCallBitCheck4);

INCLUDE_RODATA(const s32, "code_00175908", mapStrIdx);

INCLUDE_ASM(const s32, "code_00175908", mapPlNlRnd2);

INCLUDE_ASM(const s32, "code_00175908", mapCallBitCheck5);

INCLUDE_ASM(const s32, "code_00175908", mapGetSkilTakumiMapCheck);

INCLUDE_ASM(const s32, "code_00175908", mapCtnMdlLoadPreSetSub);

INCLUDE_ASM(const s32, "code_00175908", mapBCheckPanelModeSub);

INCLUDE_ASM(const s32, "code_00175908", mapBCheckPanelModeSub2);

INCLUDE_ASM(const s32, "code_00175908", mapCtnColorFadeTask);

INCLUDE_ASM(const s32, "code_00175908", mapCtnModelMake);

INCLUDE_ASM(const s32, "code_00175908", sbtlCheckModelDataLoadEnd);

INCLUDE_ASM(const s32, "code_00175908", mapCtnMdlLoad_PreSet);

INCLUDE_ASM(const s32, "code_00175908", mapCallBtlCheckModelDataLoadEnd);

INCLUDE_ASM(const s32, "code_00175908", mapCtnMdlLoad);

INCLUDE_ASM(const s32, "code_00175908", mapCtnMdlTypeRand);

INCLUDE_ASM(const s32, "code_00175908", mapCtnAndMdlLinkOn);

INCLUDE_ASM(const s32, "code_00175908", mapCtnGetSub);

INCLUDE_ASM(const s32, "code_00175908", mapCtnAndMdlLinkOff);

INCLUDE_ASM(const s32, "code_00175908", mapCtnAndMdlLinkOff2);

INCLUDE_ASM(const s32, "code_00175908", mapCtnAndMdlLinkGet);

INCLUDE_ASM(const s32, "code_00175908", mapCtnSyncFast);

INCLUDE_ASM(const s32, "code_00175908", mapCtnLoad);

INCLUDE_ASM(const s32, "code_00175908", mapCallLoadDataIDFloor);

INCLUDE_ASM(const s32, "code_00175908", mapCallCtnMdlLoad);

INCLUDE_ASM(const s32, "code_00175908", mapCtnSync);

void mapCtnFree(void) {
    if (mapCtnLoadFlag) {
        mapData149 = 0;
        ctnMemFree();
        if (mapCtnFreeData != 0.0f) {
            nekoCallMemFreeDelayedDirect();
            mapCtnFreeData = 0.0f;
        }
    }
    mapCtnLoadFlag = 0;
}

INCLUDE_ASM(const s32, "code_00175908", mapCtnCamInChk);

INCLUDE_ASM(const s32, "code_00175908", mapNakamaSummonTaskSub);

/* NON_MATCHING: 115 of 1099 words match.  */
#ifdef NON_MATCHING
void mapCtnGet(void) {
    f32 tbl[4] = { 270.0f, 180.0f, 90.0f, 0.0f };
    f32 x2;
    f32 y2;
    s32 attr;
    dds3Basic_t *ppos;
    s32 bustNum;
    s32 num = 256;
    s32 pass;
    s32 i;
    s16 wait;
    s32 j;
    s32 link;
    s32 id;
    s32 linked;
    s32 model;
    s32 over;
    s32 idx;
    s32 uniq;
    u32 color;
    f32 x;
    f32 y;
    f32 z;
    f32 px;
    f32 pz;
    f32 tf;
    s32 ti;
    ctnCitizen_t *c;
    nekoNeko_t *neko;
    s32 *status;

    if (!mapCtnLoadFlag) {
        return;
    }
    if (dds3BgSetScrollSub()) {
        return;
    }
    ppos = dds3SceneBasicSearchNameAndType(dds3GetActiveScene(), 16, "ppos01");
    mapCtnColorFadeTask();
    fldGlobalBase.word2DC = mapCtnGetObjNum();
    bustNum = evtGetDrawBustupNums();
    mapCtnSync();
    eglSetOt(103);
    mapCtnCamInChk(fldGlobalBase.focusX, fldGlobalBase.focusY, fldGlobalBase.focusZ);
    for (i = 0; i < 40; i++) {
        mdl_leng[i] = 0.0f;
        mdl_ctnidx[i] = -1;
    }
    if (evtBitCheck(206)) {
        if (fldGlobalBase.amSBlock == 4) {
            num = 3;
        } else {
            num = fldGlobalBase.amSBlock == 3 ? 3 : 8;
        }
    }

    for (pass = 0; pass < 3; pass++) {
        for (i = 0; i < num; i++) {
            if (!ctn_linkflag2[i] || ctn_linkflag2_type[i] != pass) {
                continue;
            }
            c = &Work_ctn->citizen[i];
            if (!c->fExist) {
                continue;
            }
            px = c->x;
            pz = c->y;
            x = px * 100.0f + ctn_ofs_fx;
            z = pz * -100.0f + ctn_ofs_fz;
            attr = c->attr;
            y = mapGetFloorY(ctn_floor, x, z);
            if (!mapBigClipChkF(fldGlobalBase.cutIdx, x, y, z)) {
                continue;
            }
            fldPoint3To2(&x2, &y2, x, y - 100.0f, z);
            if (!(2048.0f - Zoom * 256.0f - Zoom * 32.0f + SDF_ASPECT_LEFT < x2 &&
                  x2 < Zoom * 256.0f + 2048.0f + Zoom * 32.0f + (SDF_ASPECT_RIGHT - 512) &&
                  2048.0f - Zoom * 112.0f - Zoom * 16.0f < y2 && y2 < Zoom * 112.0f + 2048.0f + Zoom * 16.0f)) {
                continue;
            }
            linked = 0;
            if (bustNum * 6 + fldGlobalBase.word2DC + fldGlobalBase.word2E4 + fldGlobalBase.word2E0 * 3 +
                        fldGlobalBase.word2D8 < 19 &&
                (mapCtnColor & 0xFF000000) && mapCtnCamInChk(x, y - 100.0f, z) && mapCtnAndMdlLinkGet(i) == 0xFFFF) {
                linked = 1;
                if (!mapCtnGetSub(i, mapWork16[i])) {
                    continue;
                }
                fldGlobalBase.word2DC++;
            }
            if (!linked) {
                continue;
            }
            link = mapCtnAndMdlLinkGet(i);
            if (link == 0xFFFF) {
                continue;
            }
            id = link - 20224;
            mapObjPos(id, x, y, z);
            mapObjRotQ(id, &c->rotateQuat);
            mapObjColor(id, mapCtnColor);
            if (!(mapCtnColor & 0xFF000000)) {
                mapObjVisible(id, 0);
            } else {
                mapObjVisible(id, 1);
                mdl_leng[link] = fldPointDistance(mapObjTaskData.x, mapObjTaskData.y, mapObjTaskData.z, x, y, z);
                mdl_ctnidx[link] = i;
            }
            mapUnitSetLight(mapObjGetBasic(id), mapGetFloorLight(ctn_floor, x, z), 0);
            if (attr & 0x20) {
                mapObjMotionCng(id, 0, 1);
            } else {
                mapObjMotionCng(id, 1, 1);
            }
        }
    }

    for (i = 0; i < num; i++) {
        c = &Work_ctn->citizen[i];
        if (!c->fExist) {
            link = mapCtnAndMdlLinkGet(i);
            if (link != 0xFFFF) {
                mapObjVisible(link - 20224, 0);
                mapCtnAndMdlLinkOff(i);
            }
            ctn_linkflag2[i] = 0;
            continue;
        }
        if (ctn_linkflag2[i]) {
            continue;
        }
        px = c->x;
        color = 0x80802020;
        attr = c->attr;
        switch (c->classType) {
        case 3:
        case 5:
        case 7:
            model = mapCtnMdlTypeRand(1);
            break;
        default:
            if (dds3GetRandIntA(NULL, 5) == 0 ||
                (model = mapCtnMdlTypeRand(0), fldGlobalBase.amSBlock == 3 && fldGlobalBase.nameU == 0 && model == 4 &&
                                                   c->friendNum)) {
                model = dds3GetRandIntA(NULL, 5) + 7;
            }
            break;
        }
        pz = c->y;
        x = px * 100.0f + ctn_ofs_fx;
        z = pz * -100.0f + ctn_ofs_fz;
        y = mapGetFloorY(ctn_floor, x, z);
        if (!mapBigClipChkF(fldGlobalBase.cutIdx, x, y, z)) {
            link = mapCtnAndMdlLinkGet(i);
            if (link != 0xFFFF) {
                mapObjVisible(link - 20224, 0);
                mapCtnAndMdlLinkOff2(2, i);
            }
            continue;
        }
        fldPoint3To2(&x2, &y2, x, y - 100.0f, z);
        if (2048.0f - Zoom * 256.0f - Zoom * 32.0f + SDF_ASPECT_LEFT < x2 &&
            x2 < Zoom * 256.0f + 2048.0f + Zoom * 32.0f + (SDF_ASPECT_RIGHT - 512) &&
            2048.0f - Zoom * 112.0f - Zoom * 16.0f < y2 && y2 < Zoom * 112.0f + 2048.0f + Zoom * 16.0f) {
            linked = 0;
            if (bustNum * 6 + fldGlobalBase.word2DC + fldGlobalBase.word2E4 + fldGlobalBase.word2E0 * 3 +
                        fldGlobalBase.word2D8 < 19 &&
                fldGlobalBase.word2E0 == 0 && (mapCtnColor & 0xFF000000) && mapCtnCamInChk(x, y - 100.0f, z) &&
                mapCtnAndMdlLinkGet(i) == 0xFFFF) {
                mapCtnAndMdlLinkOn(i, model);
                linked = 1;
                fldGlobalBase.word2DC++;
            }
            link = mapCtnAndMdlLinkGet(i);
            if (link == 0xFFFF) {
                goto debug;
            }
            id = link - 20224;
            if (linked) {
                mapObjPos(id, x, y, z);
            } else {
                mapObjPosH(id, x, y, z);
            }
            mapObjRotQ(id, &c->rotateQuat);
            mapObjColor(id, mapCtnColor);
            if (!(mapCtnColor & 0xFF000000)) {
                mapObjVisible(id, 0);
            } else {
                mapObjVisible(id, 1);
                mdl_leng[link] = fldPointDistance(mapObjTaskData.x, mapObjTaskData.y, mapObjTaskData.z, x, y, z);
                mdl_ctnidx[link] = i;
            }
            if (linked) {
                mapUnitSetLight(mapObjGetBasic(id), mapGetFloorLight(ctn_floor, x, z), 0);
            } else {
                mapUnitSetLight(mapObjGetBasic(id), mapGetFloorLight(ctn_floor, x, z), 15);
            }
            if (gMapCtnRaidoLook) {
                if (fldCtnGetSub(mapObjGetModelMinID(id)) && !fldGlobalBase.nakamaMode) {
                    if (fldPointDistance(fldGlobalBase.focusX, fldGlobalBase.focusY, fldGlobalBase.focusZ, x, y, z) <
                        1000.0f) {
                        dds3UnitObjectBasicKubiOn(mapObjGetBasic(id), ppos->serialID);
                    } else {
                        dds3UnitObjectBasicKubiOff(mapObjGetBasic(id));
                    }
                }
            } else if (fldEncountTestFlag(0x40)) {
                dds3UnitObjectBasicKubiOff(mapObjGetBasic(id));
            }
            color = 0x802020F0;
            if (attr & 0x20) {
                mapObjMotionCng(id, 0, 1);
            } else {
                mapObjMotionCng(id, 1, 1);
            }
        } else {
            link = mapCtnAndMdlLinkGet(i);
            if (link != 0xFFFF) {
                mapObjVisible(link - 20224, 0);
                mapCtnAndMdlLinkOff2(0, i);
            }
        }
    debug:
        if (fldDbg.infoOn) {
            dds3CallGrAllocPacketBuff5(color, color, color, color, px * 4.0f, pz * 4.0f, 4.0f, 4.0f);
        }
    }

    if (fldDbg.infoOn) {
        dds3CallGrAllocPacketBuff5(0x80F020F0, 0x80F020F0, 0x80F020F0, 0x80F020F0, mapCtnSyncData * 4.0f,
                                   mapCtnSyncData2 * 4.0f, 4.0f, 4.0f);
    }

    if (fldGlobalBase.amSBlock == 1) {
        for (i = 0; i < 5; i++) {
            uniq = i - 20184;
            status = &ctn_neko_status[i];
            neko = &Work_neko->neko[i];
            if (neko->fExist == 1) {
                x = neko->x * 100.0f + ctn_ofs_fx;
                z = neko->y * -100.0f + ctn_ofs_fz;
                y = mapGetY(x, z);
                if (fldDbg.infoOn) {
                    dds3CallGrAllocPacketBuff5(0x8080F0F0, 0x8080F0F0, 0x8080F0F0, 0x8080F0F0, x * 4.0f, y * 4.0f,
                                               4.0f, 4.0f);
                }
                mapObjPos(uniq, x, y, z);
                if (mapBigClipChkF(fldGlobalBase.cutIdx, x, y, z)) {
                    fldPoint3To2(&x2, &y2, x, y - 100.0f, z);
                    if (1760.0f < x2 && x2 < 2336.0f && 1920.0f < y2 && y2 < 2176.0f && *status != 4) {
                        mapObjVisible(uniq, 1);
                        switch (neko->state) {
                        case 0:
                            if (*status == 0) {
                                *status = 1;
                            }
                            if (i < 3) {
                                mapObjRot(uniq, 90.0f);
                            } else {
                                mapObjRot(uniq, 0.0f);
                            }
                            mapObjMotionCng(uniq, 2, 0);
                            mapObjMotionSpeed(uniq, 0.5f);
                            mapUnitSetLight(mapObjGetBasic(uniq), mapGetLight(x, z), 0);
                            break;
                        case 1:
                            if (*status == 1) {
                                *status = 2;
                                if (dds3GetRandIntA(NULL, 2) == 0) {
                                    p2sdPlaySE(0x510036, 127, 63);
                                } else {
                                    p2sdPlaySE(0x510037, 127, 63);
                                }
                            }
                            mapObjRotQ(uniq, &neko->rotateQuat);
                            mapObjMotionCng(uniq, 6, 1);
                            mapObjMotionSpeed(uniq, 0.79999996f);
                            mapUnitSetLight(mapObjGetBasic(uniq), mapGetLight(x, z), 15);
                            break;
                        case 2:
                            if (*status == 2) {
                                *status = 3;
                            }
                            mapObjRotQ(uniq, &neko->rotateQuat);
                            mapUnitSetLight(mapObjGetBasic(uniq), mapGetLight(x, z), 15);
                            break;
                        }
                    } else {
                        mapObjVisible(uniq, 0);
                    }
                } else {
                    if (*status == 4 && neko->state == 0) {
                        *status = 0;
                    }
                    mapObjVisible(uniq, 0);
                }
            } else {
                *status = 4;
                mapObjVisible(uniq, 0);
            }
        }
    }

    over = bustNum * 6 + fldGlobalBase.word2DC + fldGlobalBase.word2E4 + fldGlobalBase.word2E0 * 3 +
           fldGlobalBase.word2D8 - 19;
    if (over > 0) {
        for (wait = 0; wait < 40; wait++) {
        }
        for (i = 0; i < 39; i++) {
            for (j = i + 1; j < 40; j++) {
                if (mdl_ctnidx[j] != -1 && mdl_leng[i] < mdl_leng[j]) {
                    tf = mdl_leng[j];
                    mdl_leng[j] = mdl_leng[i];
                    mdl_leng[i] = tf;
                    ti = mdl_ctnidx[j];
                    mdl_ctnidx[j] = mdl_ctnidx[i];
                    mdl_ctnidx[i] = ti;
                }
            }
        }
        for (i = 0; i < over; i++) {
            idx = mdl_ctnidx[i];
            if (idx == -1 || ctn_linkflag2[idx]) {
                continue;
            }
            c = &Work_ctn->citizen[idx];
            if (!c->fExist) {
                continue;
            }
            mapGetFloorY(ctn_floor, c->x * 100.0f + ctn_ofs_fx, c->y * -100.0f + ctn_ofs_fz);
            if (mapCtnAndMdlLinkGet(idx) != 0xFFFF) {
                link = mapCtnAndMdlLinkGet(idx);
                if (link != 0xFFFF) {
                    mapObjVisible(link - 20224, 0);
                    mapCtnAndMdlLinkOff2(1, idx);
                }
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_00175908", mapCtnGet);
#endif

INCLUDE_ASM(const s32, "code_00175908", mapCtnViewOff);

INCLUDE_ASM(const s32, "code_00175908", mapCtnGetObjNum);

INCLUDE_ASM(const s32, "code_00175908", mapCallSetIntReturnValue);

INCLUDE_ASM(const s32, "code_00175908", mapCallSetIntReturnValue2);

s32 fldCallReleaseUnitPad(void) {
    dds3Basic_t *object;

    fldReleaseUnitPad();
    object = dds3GetActiveUnitSceneBasic(dds3GetActiveScene());
    if (object == NULL) {
        return 1;
    }
    Start_lmap(0);
    return 1;
}

s32 fldCommand_PAD_ENABLE(void) {
    dds3Basic_t *object;

    object = dds3GetActiveUnitSceneBasic(dds3GetActiveScene());
    if (object == NULL) {
        return 1;
    }
    dds3ObjectBaseSetPrimaryControl(object, NULL);
    fldPlayerMotReset();
    fldRegistUnitPad();
    Start_lmap(1);
    mapEffEplAllEnd();
    evtSetUnitStat(evtGetUnitHandle2(playerBasic), 0);
    return 1;
}

INCLUDE_ASM(const s32, "code_00175908", itfSetMesDispPriority);

INCLUDE_ASM(const s32, "code_00175908", itfCallGetControlScene);

INCLUDE_RODATA(const s32, "code_00175908", mapCtnGetTable);

