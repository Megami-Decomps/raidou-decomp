#include "common.h"
#include "dds3ObjectBase.h"

#include "basic.h"
#include "dds3obj.h"
#include "field.h"
#include "include_asm.h"
#include "kernel_draw.h"
#include "sdf_packet_list.h"
#include "sdf_resource.h"
#include "sdk_libc.h"
#include "billboard.h"
#include "sdf_types.h"
#include "sdf_lighting.h"
#include "libsdf.h"
#include "model.h"
#include "motion.h"
#include "draw.h"
#include "sdf_movie_types.h"
#include "ps2asm.h"
#include "fpu.h"
#include "kernel.h"
#include "draw3d.h"
#include "scr.h"
#include "fldFileResolver_00132080.h"
#include "effPCPMisc_00224D10.h"
#include "fldFileResolver.h"
#include "datCalc.h"
#include "newdata.h"
#include "fld.h"
#include "fldProcess.h"
#include "mapData.h"
#include "mapLbWork.h"
#include "mdlManager.h"
#include "fldDbg.h"
#include "sdfQuat.h"
#include "sdfMath.h"
#include "sdk_libm.h"
#include "sdfMemory.h"
#include "dds3KernelCore.h"
#include "sdf3D.h"
#include "EventBit.h"
#include "sdfTex.h"
#include "effManager.h"
#include "dds3KernelDraw.h"

INCLUDE_ASM(const s32, "code_00157CD0", fldLeaf22);

INCLUDE_ASM(const s32, "code_00157CD0", fldMapMapMap);

void fldHirukoYukaGo_Idx2_B(s32 id) {
    mapSKYBGEDITData5 = id;
}

void fldHirukoYukaClear(void) {
    s32 i;

    fldHirukoYukaMovetaskWork.uniq = -1;
    fldHirukoYukaMovetaskWork.word04 = 0;
    fldHirukoYukaMovetaskWork.vec0C[0] = 0.0f;
    fldHirukoYukaMovetaskWork.vec0C[1] = 0.0f;
    fldHirukoYukaMovetaskWork.vec0C[2] = 0.0f;
    fldHirukoYukaMovetaskWork.vec18[0] = 0.0f;
    fldHirukoYukaMovetaskWork.vec18[1] = 0.0f;
    fldHirukoYukaMovetaskWork.vec18[2] = 0.0f;
    fldHirukoYukaMovetaskWork.vec24[0] = 0.0f;
    fldHirukoYukaMovetaskWork.vec24[1] = 0.0f;
    fldHirukoYukaMovetaskWork.vec24[2] = 0.0f;
    fldHirukoYukaMovetaskWork.vec30[0] = 0.0f;
    fldHirukoYukaMovetaskWork.vec30[1] = 0.0f;
    fldHirukoYukaMovetaskWork.vec30[2] = 0.0f;
    for (i = 0; i < 26; i++) {
        DDS3_GBWK->pointTable[i].word = 0;
    }
    DDS3_GBWK->pointTable[0].word = 257;
    DDS3_GBWK->pointTable[1].word = 259;
    DDS3_GBWK->pointTable[2].word = 263;
    DDS3_GBWK->pointTable[3].word = 289;
    DDS3_GBWK->pointTable[4].word = 291;
    DDS3_GBWK->pointTable[5].word = 295;
    DDS3_GBWK->pointTable[6].word = 299;
    DDS3_GBWK->pointTable[7].word = 337;
    DDS3_GBWK->pointTable[8].word = 339;
    DDS3_GBWK->pointTable[9].word = 344;
    DDS3_GBWK->pointTable[10].word = 345;
    DDS3_GBWK->pointTable[11].word = 350;
    DDS3_GBWK->pointTable[12].word = 351;
    DDS3_GBWK->pointTable[13].word = 356;
    DDS3_GBWK->pointTable[14].word = 265;
    DDS3_GBWK->pointTable[15].word = 301;
    DDS3_GBWK->pointTable[16].word = 302;
    DDS3_GBWK->pointTable[17].word = 303;
    DDS3_GBWK->pointTable[18].word = 304;
    DDS3_GBWK->pointTable[19].word = 359;
    DDS3_GBWK->pointTable[20].word = 360;
    DDS3_GBWK->pointTable[21].word = 361;
    DDS3_GBWK->pointTable[22].word = 362;
    DDS3_GBWK->pointTable[23].word = 400;
    evtBitOn(1691);
    evtBitOff(1692);
    evtBitOn(1693);
    evtBitOff(1694);
    evtBitOff(1695);
    evtBitOff(1696);
    evtBitOff(1697);
    evtBitOff(1698);
    evtBitOff(1699);
    evtBitOff(1700);
    evtBitOff(1701);
    evtBitOff(1702);
}

INCLUDE_ASM(const s32, "code_00157CD0", fldCallHirukoYukaOn);

INCLUDE_ASM(const s32, "code_00157CD0", fldCallHirukoYukaOn2);

INCLUDE_ASM(const s32, "code_00157CD0", fldHirukoYukaOn);

INCLUDE_ASM(const s32, "code_00157CD0", fldCallObjVisible);

INCLUDE_ASM(const s32, "code_00157CD0", fldHirukoYukaMoveStart);

INCLUDE_ASM(const s32, "code_00157CD0", HirukoYukaMoveStartUpDown);

INCLUDE_ASM(const s32, "code_00157CD0", fldHirukoYukaMovetask);

INCLUDE_ASM(const s32, "code_00157CD0", fldHirukoYukaGo_Idx);

INCLUDE_ASM(const s32, "code_00157CD0", fldHirukoYukaGo_Idx2_B_fld);

INCLUDE_ASM(const s32, "code_00157CD0", HirukoYukaPlayerOnChkSub);

INCLUDE_ASM(const s32, "code_00157CD0", fldHirukoYukaPlayerOnChk);

INCLUDE_ASM(const s32, "code_00157CD0", mapSpotPointGet2D);

INCLUDE_ASM(const s32, "code_00157CD0", mapSpotPointFlagCheck);

INCLUDE_ASM(const s32, "code_00157CD0", mapSpotPointFlagOn);

INCLUDE_ASM(const s32, "code_00157CD0", mapSpotPointDrawStart);

INCLUDE_ASM(const s32, "code_00157CD0", mapSpotPointDrawStart2);

void mapSpotPointDrawEndChk(void) {
    s32 i;
    s32 idx;
    fldGenbaTbl_t *genba;

    for (i = 0; i < 32; i++) {
        if (mapData31[i].use != 0) {
            idx = mapData31[i].uniq & 0xFF;
            genba = &fldGenbaTbl[idx];
            if (evtBitCheck(genba->evtBit) && !mapSpotPointFlagCheck(idx)) {
                mapObjVisible(mapData31[i].uniq, 0);
            }
        }
    }
}

INCLUDE_ASM(const s32, "code_00157CD0", mapSIZE);

INCLUDE_ASM(const s32, "code_00157CD0", mapNakamaHajiki1_PosSet);

void mapEveHitMsk_Find(s32 evehitid) {
    MskEveHitTbl_t *tbl;
    s32 i;

    fldGlobalBase.word154 = -1;
    tbl = mapMskEveHitTbl;
    for (i = 0; i < 16; i++, tbl++) {
        if (tbl->_map == fldGlobalBase.amSBlock && tbl->_floor == fldGlobalBase.nameU && tbl->_EveHitId == evehitid) {
            fldGlobalBase.word154 = i;
            return;
        }
    }
}

INCLUDE_ASM(const s32, "code_00157CD0", mapEveHitMsk_Set);

INCLUDE_ASM(const s32, "code_00157CD0", mapEveHitMsk_SetPosCam);

INCLUDE_ASM(const s32, "code_00157CD0", mapScreenReset);

INCLUDE_ASM(const s32, "code_00157CD0", mapScreenCutReset);

INCLUDE_ASM(const s32, "code_00157CD0", mapScreenChkIn);

INCLUDE_ASM(const s32, "code_00157CD0", mapFunc9);

mefMistSystem_t *mefMistFastCreate(void) {
    sdfMemHandle_t *hMem;
    mefMistSystem_t *mist;
    s32 i;

    hMem = sdfMemAllocDirect(sizeof(mefMistSystem_t));
    mist = sdfMemLock(hMem);
    mist->hMem = hMem;
    pMapMoya = mist;
    mist->ot = 79;
    mist->saturation = 128;
    mist->brightness = 90;
    mist->fRun = 1;
    mist->alpha = 80;
    mist->num = 0;
    mist->cnt = 0;
    mist->frequency = 16;
    mist->alphaMax = 16;
    mist->fDebugMenu = 0;
    mist->fDebugTimeGraph = 0;
    mist->debugMenuIndex = 0;
    mist->scroll = 0;
    mist->mode = 0;
    for (i = 511; i >= 0; i--) {
        mist->particle[i].fExist = 0;
    }
    for (i = 999; i >= 0; i--) {
        mefMistUpdate();
    }
    return mist;
}

INCLUDE_ASM(const s32, "code_00157CD0", mapLoadMistTexture);

s32 mefMistTexOnDataCheck(void) {
    if (pMapMoya == NULL) {
        return 0;
    }
    return pMapMoya->texHandle != NULL;
}

INCLUDE_ASM(const s32, "code_00157CD0", mapLeaf22);

INCLUDE_ASM(const s32, "code_00157CD0", pipeFileLoadSub);

INCLUDE_ASM(const s32, "code_00157CD0", pipeFileLoadSub2);

void mefMistUpdate(void) {
    mefMistSystem_t *mist = pMapMoya;

    if (mist->fRun != 0) {
        mefMistEmitParticle();
        mefMistUpdateParticle();
        mist->cnt++;
    }
}

INCLUDE_ASM(const s32, "code_00157CD0", pipeRefZoom);

INCLUDE_ASM(const s32, "code_00157CD0", mefMistDraw);

INCLUDE_ASM(const s32, "code_00157CD0", mefMistTexDestroy);

void mefMistEmitParticle(void) {
    mefMistSystem_t *mist = pMapMoya;
    mefMistParticle_t *p;
    s32 n = 1;
    s32 i;

    if (mist->num == 512) {
        return;
    }
    if (mist->cnt % mist->frequency != 0) {
        return;
    }
    p = mist->particle;
    for (i = 0; i < 512; i++, p++) {
        if (p->fExist == 0) {
            n--;
            fldCallGetRandFloatA(p);
            if (n == 0) {
                return;
            }
        }
    }
}

void mefMistUpdateParticle(void) {
    mefMistSystem_t *mist = pMapMoya;
    mefMistParticle_t *p = mist->particle;
    s32 i;

    for (i = 511; i >= 0; i--, p++) {
        if (p->fExist != 0) {
            p->pos[0] += p->velocity[0];
            p->pos[1] += p->velocity[1];
            p->lifeCnt++;
            if (p->lifeCnt >= p->deathCnt) {
                p->fExist = 0;
                mist->num--;
            }
        }
    }
}

void mapEffEplInit(void) {
    s32 i;

    for (i = 0; i < 33; i++) {
        mapEffEplStartWork[i].pos.x = 0.0f;
        mapEffEplStartWork[i].pos.y = 0.0f;
        mapEffEplStartWork[i].pos.z = 0.0f;
        mapEffEplStartWork[i].pos.w = 1.0f;
        mapEffEplStartWork[i].hMem = NULL;
        mapEffEplStartWork[i].pMem = NULL;
        mapEffEplStartWork[i].eHdl = NULL;
        mapEffEplStartWork[i].view = 0;
        mapEffEplStartWork[i].yukay = 0.0f;
        mapEffEplStartWork[i].time = -1;
    }
}

INCLUDE_ASM(const s32, "code_00157CD0", fldMapAllBUFU);

INCLUDE_ASM(const s32, "code_00157CD0", mapEffEplLoad_LB_GENBA);

INCLUDE_ASM(const s32, "code_00157CD0", mapEffEplLoad_LB_GENBA_EndCheck_fld);

INCLUDE_ASM(const s32, "code_00157CD0", mapEffEplLoad_LB_INEN_fld);

INCLUDE_ASM(const s32, "code_00157CD0", mapEffEplLoad_LB_INEN_EndCheck);

INCLUDE_ASM(const s32, "code_00157CD0", mapEffEplLoad_LB_TAKANE);

INCLUDE_ASM(const s32, "code_00157CD0", mapEffEplLoad_LB_REI_EndCheck);

INCLUDE_ASM(const s32, "code_00157CD0", mapEffEplLoad_LB_HAKA);

INCLUDE_ASM(const s32, "code_00157CD0", mapEffEplLoad_LB_CHIKARA_EndCheck);

INCLUDE_ASM(const s32, "code_00157CD0", mapEffEplLoad_LB_REI);

INCLUDE_ASM(const s32, "code_00157CD0", mapEffEplLoad_LB_DOKUSIN_EndCheck);

INCLUDE_ASM(const s32, "code_00157CD0", mapEffEplLoad_LB_CHIKARA);

INCLUDE_ASM(const s32, "code_00157CD0", mapEffEplLoad_LB_IRO_EndCheck);

INCLUDE_ASM(const s32, "code_00157CD0", mapEffEplLoad_LB_DOKUSIN);

INCLUDE_ASM(const s32, "code_00157CD0", mapEffEplLoad_LB_ZENI_EndCheck);

INCLUDE_ASM(const s32, "code_00157CD0", mapEffEplLoad_LB_IRO);

INCLUDE_ASM(const s32, "code_00157CD0", mapEffEplLoad_LB_IRO_EndCheck_fld);

INCLUDE_ASM(const s32, "code_00157CD0", mapEffEplLoad_LB_ZENI);

INCLUDE_ASM(const s32, "code_00157CD0", mapEffEplLoad_LB_GENBA_EndCheck);

INCLUDE_ASM(const s32, "code_00157CD0", mapEffEplLoad_LB_INEN);

INCLUDE_ASM(const s32, "code_00157CD0", mapEffEplLoad_LB_TAKUMI_EndCheck);

INCLUDE_ASM(const s32, "code_00157CD0", mapEffEplLoad_LB_TAKUMI);

INCLUDE_ASM(const s32, "code_00157CD0", mapCallCheckEnd);

INCLUDE_ASM(const s32, "code_00157CD0", mapEffEplStart);

void mapEffEplEnd(s32 id) {
    if (mapEffEplStartWork[id].eHdl != NULL) {
        dds3EffectDelete(mapEffEplStartWork[id].eHdl);
        mapEffEplStartWork[id].eHdl = NULL;
    }
    if (mapEffEplStartWork[id].hMem != NULL) {
        sdfMemFreeDirect(mapEffEplStartWork[id].hMem);
        mapEffEplStartWork[id].hMem = NULL;
        mapEffEplStartWork[id].pMem = NULL;
    }
    mapEffEplStartWork[id].view = 0;
    mapEffEplStartWork[id].startwait = 0;
}

void mapEffEplAllEnd(void) {
    s32 i;

    for (i = 0; i < 33; i++) {
        mapEffEplEnd(i);
    }
}

void mapEffEplAllCalcTask(void) {
    s32 i;

    for (i = 0; i < 33; i++) {
        if (mapEffEplStartWork[i].eHdl == NULL) {
            continue;
        }
        if (mapEffEplStartWork[i].view != 0) {
            if (mapEffEplStartWork[i].time == -1) {
                continue;
            }
            if (dds3EffectGetState(mapEffEplStartWork[i].eHdl) == DDS3EFFECT_STATE_END) {
                mapEffEplStartWork[i].time = -1;
                mapEffEplEnd(i);
            } else if (--mapEffEplStartWork[i].time <= 0) {
                mapEffEplStartWork[i].time = -1;
                mapEffEplEnd(i);
            }
        } else if (mapEffEplStartWork[i].startwait > 0) {
            if (--mapEffEplStartWork[i].startwait == 0) {
                mapEffEplStartWork[i].view = 1;
            }
        }
    }
}

void mapEffEplAllDrawTask(void) {
    s32 i;
    f32 x;
    f32 y;
    f32 z;

    for (i = 0; i < 33; i++) {
        if (mapEffEplStartWork[i].eHdl != NULL && mapEffEplStartWork[i].view != 0) {
            dds3EffectDraw_dds3Effect(mapEffEplStartWork[i].eHdl);
            x = mapEffEplStartWork[i].pos.x;
            y = mapEffEplStartWork[i].yukay;
            z = mapEffEplStartWork[i].pos.z;
            if (fldGlobalBase.amSBlock != 31) {
                dds3CallSetOt3(x, y, z, 200.0f);
            }
        }
    }
}

INCLUDE_ASM(const s32, "code_00157CD0", mapLoadGenbaEpl);

void workRelease(void) {
    if (hWorkArea2 != NULL) {
        sdfMemFreeDirect(hWorkArea2);
        hWorkArea2 = NULL;
        sdfDiscRequestSemaphore2 = 0;
    }
}

INCLUDE_ASM(const s32, "code_00157CD0", mapGenbaPosEffCreate);
