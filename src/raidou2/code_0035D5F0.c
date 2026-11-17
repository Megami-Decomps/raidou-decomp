#include "common.h"
#include "effManager.h"
#include "mapData.h"
#include "mjn.h"
#include "lmap.h"
#include "mselGlobal.h"
#include "sdfMemory.h"
#include "fldNme.h"

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

void mselCallMInitSceneMjn(void) {
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

INCLUDE_ASM(const s32, "code_0035D5F0", fldNmeProcess);

INCLUDE_ASM(const s32, "code_0035D5F0", fldCallMemGetPtr);

INCLUDE_ASM(const s32, "code_0035D5F0", fldCallMemGetPtr2);

INCLUDE_ASM(const s32, "code_0035D5F0", fldLeaf28);

INCLUDE_ASM(const s32, "code_0035D5F0", fldCallMemGetPtr5);

s32 fldCallMemGetPtr6(dds3ProcessID_t *id, s32 *code) {
    fldNmeWork_t *work = sdfMemGetPtr(id->workdata);
    s32 page;
    s32 index;
    u32 count;
    s32 ret;

    if (work->unkC >= 3) {
        if (work->unkC == 3) {
            titleFunc4(titleFunc6(work->unk38), &page, &index);
            index += work->unk34;
            count = fclLeaf23(page);
            if (index >= count) {
                return 0;
            }
            *code = fclLeaf24(page, index);
            return 1;
        }
    } else {
        ret = titleFunc(work->unkC, work->unk34, work->unk38);
        if (ret != 0) {
            index = nmeLeaf2(work->unkC, work->unk34, work->unk38);
            *code = index;
            return 1;
        }
        return ret;
    }
    return 0;
}
