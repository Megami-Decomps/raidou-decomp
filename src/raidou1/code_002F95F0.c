#include "common.h"
#include "nmeMain.h"
#include "Quest.h"
#include "effManager.h"
#include "mapData.h"
#include "mjn.h"
#include "lmap.h"
#include "mselGlobal.h"
#include "sdfMemory.h"

void mselCallEffectDelete(void) {
    if (fldTitleMiniTex3 != NULL) {
        dds3EffectDelete(fldTitleMiniTex3);
        fldTitleMiniTex3 = NULL;
    }
    if (fldTitleMiniWork4 != NULL) {
        sdfMemFreeDelayedDirect(fldTitleMiniWork4);
        fldTitleMiniWork4 = NULL;
    }
    mselNameNEFLASHData = 0;
}

void mselLeaf6(void)
{
    if (fldTitleMiniTex3 != NULL) {
        dds3EffectDraw_dds3Effect(fldTitleMiniTex3);
    }
}

void mselCallMInitSceneMjn(void)
{
    M_InitScene_mjn();
    mselNmeCamera();
    mselNameNEFLASH();
}

void fldFunc19(void) {
    mselCallEffectDelete();
    mselCallGetControlScene();
}

void Start_nmeMain(void) {
    mselLeaf6();
}
