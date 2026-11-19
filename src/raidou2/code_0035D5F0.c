#include "common.h"
#include "effManager.h"
#include "mapData.h"
#include "mjn.h"
#include "lmap.h"
#include "mselGlobal.h"
#include "sdfMemory.h"
#include "fldNme.h"
#include "dds3KernelCore.h"
#include "dds3KernelMain.h"
#include "dds3PadManager.h"
#include "sdfSound.h"

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

extern const char fldStrFontNameEntFnt[];
extern const char fldStrNmeProcess[];
extern const char fldStrNmeDrawProcess[];

dds3ProcessID_t *fldNmeProcess(void) {
    sdfMemHandle_t *mem = sdfMemAllocDirect(sizeof(fldNmeWork_t));
    dds3ProcessID_t *process = dds3StartProcess(fldStrNmeProcess, 0x3FF, 0, 0, fldCallMemGetPtr, (dds3ProcessFunc_t)fldCallMemGetPtr5, mem);
    fldNmeWork_t *work = sdfMemGetPtr(mem);

    work->unk0 = 0;
    work->drawProcess = dds3StartProcess(fldStrNmeDrawProcess, 0x2B07, 0, 0, fldCallMemGetPtr2, NULL, mem);
    work->unk34 = 0;
    work->unk38 = 0;
    work->unk3C = 0;
    work->unk14 = 0;
    work->unk40 = 0;
    work->unk30 = 0;
    work->unk10 = 0;
    work->unkC = 0;
    work->unk44 = titleStartSub(0);
    work->unk48 = titleStartSub2(work->unkC);
    work->font = fileLoad(fldStrFontNameEntFnt);
    work->unk8 = 0;
    dds3CallEventSub(0);
    return process;
}

INCLUDE_ASM(const s32, "code_0035D5F0", fldCallMemGetPtr);

s32 fldCallMemGetPtr2(dds3ProcessID_t *id) {
    fldNmeWork_t *work = sdfMemGetPtr(id->workdata);

    switch (work->unk8) {
    case 1:
        fclCallSetSelect(&work->draw, 0);
        fclLeaf15(&work->draw);
        fldCallMemGetPtr9(id);
        fclCallSetFontColorNo(&work->draw, work->unkC, work->unk30, work->unk10, work->unk34, work->unk38);
        fclFunc6(&work->draw, work->unk10, work->unk3C);
        fldCallMemGetPtr10(id);
        break;
    case 2:
        fclCallSetSelect(&work->draw, 0);
        fclLeaf15(&work->draw);
        fldCallMemGetPtr9(id);
        fclFunc6(&work->draw, work->unk10, work->unk3C);
        fldCallMemGetPtr11(id);
        break;
    case 3:
        fclCallSetSelect(&work->draw, 0);
        fclLeaf15(&work->draw);
        fldCallMemGetPtr9(id);
        fclFunc6(&work->draw, work->unk10, work->unk3C);
        fldCallMemGetPtr4(id);
        break;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0035D5F0", Finish_nmeMain);

void fldCallMemGetPtr5(dds3ProcessID_t *id) {
    fldNmeWork_t *work = sdfMemGetPtr(id->workdata);
    u32 i;

    for (i = 0; i < 2; i++) {
        fclFunc10(&work->list[i]);
    }
    fclFunc4(&work->draw);
    if (work->drawProcess != NULL) {
        dds3KillProcess(work->drawProcess, 0);
    }
    frCallMemFreeDirect(4);
    dds3CallEventSub(1);
    sdfMemFreeDelayedDirect(id->workdata);
}

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
