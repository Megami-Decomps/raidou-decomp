#include "common.h"
#include "mapData.h"
#include "sdfMemory.h"
#include "fldProcess.h"
#include "EventBit.h"
#include "fld.h"
#include "effBlur_Filter.h"

INCLUDE_ASM(const s32, "code_00163C50", fldLeaf22);

INCLUDE_ASM(const s32, "code_00163C50", fldMapMapMap);

INCLUDE_ASM(const s32, "code_00163C50", fldHirukoYukaGo_Idx2_B);

INCLUDE_ASM(const s32, "code_00163C50", mapSpotPointGet2D);

INCLUDE_ASM(const s32, "code_00163C50", mapSpotPointFlagCheck);

INCLUDE_ASM(const s32, "code_00163C50", mapSpotPointFlagOn);

INCLUDE_ASM(const s32, "code_00163C50", mapSpotPointDrawStart);

INCLUDE_ASM(const s32, "code_00163C50", mapSpotPointDrawStart2);

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

INCLUDE_ASM(const s32, "code_00163C50", mapSIZE);

INCLUDE_ASM(const s32, "code_00163C50", mapNakamaHajiki1_PosSet);

INCLUDE_ASM(const s32, "code_00163C50", mapEveHitMsk_Find);

INCLUDE_ASM(const s32, "code_00163C50", mapEveHitMsk_Set);

INCLUDE_ASM(const s32, "code_00163C50", mapEveHitMsk_SetPosCam);

INCLUDE_ASM(const s32, "code_00163C50", mapScreenReset);

INCLUDE_ASM(const s32, "code_00163C50", mapScreenCutReset);

INCLUDE_ASM(const s32, "code_00163C50", mapScreenChkIn);

INCLUDE_ASM(const s32, "code_00163C50", mapFunc9);

INCLUDE_ASM(const s32, "code_00163C50", mapLeaf2);

INCLUDE_ASM(const s32, "code_00163C50", mapRefDDS3GBWK4);

INCLUDE_ASM(const s32, "code_00163C50", mapRefDDS3GBWK5);

INCLUDE_ASM(const s32, "code_00163C50", mapFunc2);

INCLUDE_ASM(const s32, "code_00163C50", mapCallObjPos);

INCLUDE_ASM(const s32, "code_00163C50", mapFunc3);

INCLUDE_ASM(const s32, "code_00163C50", fldCallObjVisible);

INCLUDE_ASM(const s32, "code_00163C50", fldRefDDS3GBWK);

INCLUDE_ASM(const s32, "code_00163C50", fldCallAiboFoceSet3);

INCLUDE_ASM(const s32, "code_00163C50", fldEveHitTaskFldSub);

INCLUDE_ASM(const s32, "code_00163C50", fldFileLoadSub);

INCLUDE_ASM(const s32, "code_00163C50", fldCallObjPointGet);

INCLUDE_ASM(const s32, "code_00163C50", fldCallEncountTestFlag);

INCLUDE_ASM(const s32, "code_00163C50", fldClearObjectEntryHandles);

INCLUDE_ASM(const s32, "code_00163C50", fldCallSetPlayerTargetData);

INCLUDE_ASM(const s32, "code_00163C50", fldCallBitCheck2);

INCLUDE_ASM(const s32, "code_00163C50", GetOverrideMapObj);

INCLUDE_ASM(const s32, "code_00163C50", fldLeaf);

INCLUDE_ASM(const s32, "code_00163C50", fldCallMemset2);

INCLUDE_ASM(const s32, "code_00163C50", nbModelFieldApo);

INCLUDE_ASM(const s32, "code_00163C50", nbLeaf);

INCLUDE_ASM(const s32, "code_00163C50", nbCallEffectDistortionBlurSetParam);

INCLUDE_ASM(const s32, "code_00163C50", nbCallObjDel);

INCLUDE_ASM(const s32, "code_00163C50", nbFreeDeadMoonTex);

INCLUDE_ASM(const s32, "code_00163C50", nbCallEffectDistortionBlurCreate);

void nbObjTaskSub(void) {
    if (DeadMoonTmx3 != NULL) {
        effBlurStepScatterSlotsAndDraw(DeadMoonTmx3);
    }
}

INCLUDE_ASM(const s32, "code_00163C50", nbFunc2);

INCLUDE_ASM(const s32, "code_00163C50", mefLeaf);

INCLUDE_ASM(const s32, "code_00163C50", mefLeaf2);

/* NON_MATCHING: 85 of 88 words match.  */
#ifdef NON_MATCHING
mefMistSystem_t *mefMistFastCreate(void) {
    sdfMemHandle_t *hMem;
    mefMistSystem_t *mist;
    s32 i;

    hMem = sdfMemAllocDirect(sizeof(mefMistSystem_t));
    mist = sdfMemLock(hMem);
    mist->hMem = hMem;
    pMapMoya = mist;
    mist->ot = 80;
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
    mist->texHandle = NULL;
    for (i = 511; i >= 0; i--) {
        mist->particle[i].fExist = 0;
    }
    for (i = 999; i >= 0; i--) {
        mefMistUpdate();
    }
    return mist;
}
#else
INCLUDE_ASM(const s32, "code_00163C50", mefMistFastCreate);
#endif

INCLUDE_ASM(const s32, "code_00163C50", mapLoadMistTexture);

s32 mefMistTexOnDataCheck(void) {
    if (pMapMoya == NULL) {
        return 0;
    }
    return pMapMoya->texHandle != NULL;
}

INCLUDE_ASM(const s32, "code_00163C50", mapFileLoadSub);

INCLUDE_ASM(const s32, "code_00163C50", mapLeaf22);

INCLUDE_ASM(const s32, "code_00163C50", pipeFileLoadSub);

INCLUDE_ASM(const s32, "code_00163C50", pipeFileLoadSub2);

void mefMistUpdate(void) {
    mefMistSystem_t *mist = pMapMoya;

    if (mist->fRun != 0) {
        mefMistEmitParticle();
        mefMistUpdateParticle();
        mist->cnt++;
    }
}

INCLUDE_ASM(const s32, "code_00163C50", pipeRefZoom);

INCLUDE_ASM(const s32, "code_00163C50", mefMistDraw);

INCLUDE_ASM(const s32, "code_00163C50", mefMistTexDestroy);

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

void mefFunc(void)
{
    s32 i;

    for (i = 0; i < 33; i++) {
        mapCameraInitSub(&mapEffEplStartWork[i]);
    }
}

INCLUDE_ASM(const s32, "code_00163C50", mapCameraInitSub);

INCLUDE_ASM(const s32, "code_00163C50", mapLeaf3);

INCLUDE_ASM(const s32, "code_00163C50", mapAll);

INCLUDE_ASM(const s32, "code_00163C50", mapEffEplLoad_LB_GENBA);

INCLUDE_ASM(const s32, "code_00163C50", mapEffEplLoad_LB_GENBA_EndCheck_fld);

INCLUDE_ASM(const s32, "code_00163C50", mapEffEplLoad_LB_INEN_fld);

INCLUDE_ASM(const s32, "code_00163C50", mapEffEplLoad_LB_INEN_EndCheck);

INCLUDE_ASM(const s32, "code_00163C50", mapEffEplLoad_LB_TAKANE);

INCLUDE_ASM(const s32, "code_00163C50", mapEffEplLoad_LB_REI_EndCheck);

INCLUDE_ASM(const s32, "code_00163C50", mapEffEplLoad_LB_HAKA);

INCLUDE_ASM(const s32, "code_00163C50", mapEffEplLoad_LB_CHIKARA_EndCheck);

INCLUDE_ASM(const s32, "code_00163C50", mapEffEplLoad_LB_REI);

INCLUDE_ASM(const s32, "code_00163C50", mapEffEplLoad_LB_DOKUSIN_EndCheck);

INCLUDE_ASM(const s32, "code_00163C50", mapEffEplLoad_LB_CHIKARA);

INCLUDE_ASM(const s32, "code_00163C50", mapEffEplLoad_LB_IRO_EndCheck);

INCLUDE_ASM(const s32, "code_00163C50", mapEffEplLoad_LB_DOKUSIN);

INCLUDE_ASM(const s32, "code_00163C50", mapEffEplLoad_LB_ZENI_EndCheck);

INCLUDE_ASM(const s32, "code_00163C50", mapEffEplLoad_LB_IRO);

INCLUDE_ASM(const s32, "code_00163C50", mapEffEplLoad_LB_IRO_EndCheck_fld);

INCLUDE_ASM(const s32, "code_00163C50", mapEffEplLoad_LB_ZENI);

INCLUDE_ASM(const s32, "code_00163C50", mapEffEplLoad_LB_GENBA_EndCheck);

INCLUDE_ASM(const s32, "code_00163C50", mapEffEplLoad_LB_INEN);

INCLUDE_ASM(const s32, "code_00163C50", mapEffEplLoad_LB_TAKUMI_EndCheck);

INCLUDE_ASM(const s32, "code_00163C50", mapEffEplLoad_LB_TAKUMI);

INCLUDE_ASM(const s32, "code_00163C50", mapCallCheckEnd);

INCLUDE_ASM(const s32, "code_00163C50", mapNakamaChikaraTaskSub);

INCLUDE_ASM(const s32, "code_00163C50", nbCallEffectReset);

INCLUDE_ASM(const s32, "code_00163C50", mapEffEplEnd);

INCLUDE_ASM(const s32, "code_00163C50", nbFreeEventPack);

void mapEffEplAllEnd(void) {
    s32 i;

    for (i = 0; i < 33; i++) {
        mapEffEplEnd(i);
    }
}

void mapProcSequenceSub2(void)
{
    s32 i;

    for (i = 0; i < 33; i++) {
        mapCallEffectGetState(&mapEffEplStartWork[i]);
    }
}

INCLUDE_ASM(const s32, "code_00163C50", mapCallEffectGetState);

void mapIsFldKeyFreeSub(void)
{
    s32 i;

    for (i = 0; i < 33; i++) {
        fldCallEffectDrawDds3Effect(&mapEffEplStartWork[i]);
    }
}

INCLUDE_ASM(const s32, "code_00163C50", fldCallEffectDrawDds3Effect);

INCLUDE_ASM(const s32, "code_00163C50", fldLeaf2);

INCLUDE_ASM(const s32, "code_00163C50", fldGetSelectedUnitStat);

INCLUDE_ASM(const s32, "code_00163C50", mapLoadGenbaEpl);

void workRelease(void) {
    if (hWorkArea2 != NULL) {
        sdfMemFreeDirect(hWorkArea2);
        hWorkArea2 = NULL;
        sdfDiscRequestSemaphore2 = 0;
    }
}

INCLUDE_ASM(const s32, "code_00163C50", mapGenbaPosEffCreate);
