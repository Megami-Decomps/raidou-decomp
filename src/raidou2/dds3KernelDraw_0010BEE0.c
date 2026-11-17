#include "common.h"
#include "dds3KernelDraw.h"
#include "sdfMemory.h"
#include "sdfFile.h"
#include "code_r1_0021CE60.h"
#include "sdfMath.h"

INCLUDE_ASM(const s32, "dds3KernelDraw_0010BEE0", dds3CallSetOt3);

INCLUDE_ASM(const s32, "dds3KernelDraw_0010BEE0", dds3Func);

INCLUDE_ASM(const s32, "dds3KernelDraw_0010BEE0", dds3FirstInitSub);

INCLUDE_ASM(const s32, "dds3KernelDraw_0010BEE0", dds3DrawAddMagSub);

INCLUDE_ASM(const s32, "dds3KernelDraw_0010BEE0", nbResultCountEntries);

INCLUDE_ASM(const s32, "dds3KernelDraw_0010BEE0", dds3CallSetOt);

void fldSkyDefaultLoad(void)
{
    sdfFileCmdHandle_t *file;

    if (gFilterBuff == NULL) {
        gFilterBuff = sdfMemLock(sdfMemAllocDirect(0x12400));
    }
    file = sdfFileOpen(fldStrMapAllFilterFld);
    sdfFileRead(file, gFilterBuff, 0x12400);
    sdfFileClose(file);
}

INCLUDE_ASM(const s32, "dds3KernelDraw_0010BEE0", dds3FilterClear);

void dds3FilterSet(s32 idx, s32 mode, s32 hokan)
{
    if (hokan == 0) {
        gFilterBlend = 1.0f;
        gFilterBlendIdx = idx;
        gFilterBlendSpd = 1.0f;
    } else {
        gFilterBlend = 0.0f;
        gFilterBlendIdx = gFilterNowIdx;
        gFilterBlendSpd = (f32)hokan;
    }
    gFilterNowIdx = idx;
    gFilterMode = mode;
    dds3Data34 = 1;
}

void fldSetFilterAnime(s32 anime)
{
    gFilterAnime = anime;
    gFilterAnimeCntF = 0;
    gFilterAnimeOfsA = 0;
}

INCLUDE_ASM(const s32, "dds3KernelDraw_0010BEE0", dds3FilterSetAlpha);

INCLUDE_ASM(const s32, "dds3KernelDraw_0010BEE0", dds3FilterGetAlpha);

INCLUDE_ASM(const s32, "dds3KernelDraw_0010BEE0", dds3FilterSetAlphaOfs);

INCLUDE_ASM(const s32, "dds3KernelDraw_0010BEE0", dds3FilterGetAlphaOfs);

void dds3ProcSequenceSub(u32 ot)
{
    dds3Data34 = ot;
}

INCLUDE_RODATA(const s32, "dds3KernelDraw_0010BEE0", fldStrMapAllFilterFld);

void dds3FilterCalc(void)
{
    if (dds3Data34 == 0) {
        return;
    }
    gFilterAnimeOfsA = 0;
    switch (gFilterAnime) {
    case 1:
        gFilterAnimeCntF += 0.05f;
        gFilterAnimeOfsA = sdfSin(gFilterAnimeCntF) * 32.0f;
        break;
    case 2:
        gFilterAnimeCntF += 0.1f;
        gFilterAnimeOfsA = sdfSin(gFilterAnimeCntF) * 32.0f;
        break;
    case 3:
        gFilterAnimeCntF += 0.025f;
        gFilterAnimeOfsA = sdfSin(gFilterAnimeCntF) * 32.0f;
        break;
    case 4:
        gFilterAnimeCntF += 0.05f;
        gFilterAnimeOfsA = sdfSin(gFilterAnimeCntF) * 48.0f;
        break;
    case 5:
        gFilterAnimeCntF += 0.1f;
        gFilterAnimeOfsA = sdfSin(gFilterAnimeCntF) * 48.0f;
        break;
    case 6:
        gFilterAnimeCntF += 0.025f;
        gFilterAnimeOfsA = sdfSin(gFilterAnimeCntF) * 48.0f;
        break;
    case 7:
        gFilterAnimeCntF += 0.05f;
        gFilterAnimeOfsA = sdfSin(gFilterAnimeCntF) * 12.0f;
        break;
    case 8:
        gFilterAnimeCntF += 0.1f;
        gFilterAnimeOfsA = sdfSin(gFilterAnimeCntF) * 12.0f;
        break;
    case 9:
        gFilterAnimeCntF += 0.025f;
        gFilterAnimeOfsA = sdfSin(gFilterAnimeCntF) * 12.0f;
        break;
    }
}

INCLUDE_ASM(const s32, "dds3KernelDraw_0010BEE0", dds3FilterDraw);

INCLUDE_ASM(const s32, "dds3KernelDraw_0010BEE0", dds3FilterEdit);
