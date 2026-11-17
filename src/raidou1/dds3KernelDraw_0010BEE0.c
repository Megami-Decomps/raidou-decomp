#include "common.h"
#include "dds3KernelDraw.h"
#include "dds3KernelDraw_0010A750.h"
#include "sdfMemory.h"
#include "sdfFile.h"
#include "code_r1_0021CE60.h"
#include "sdfMath.h"

INCLUDE_ASM(const s32, "dds3KernelDraw_0010BEE0", dds3CallSetOt3);

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

/* NON_MATCHING: 12 of 15 words match.  */
#ifdef NON_MATCHING
void dds3FilterClear(void)
{
    gFilterAlpha = 0x80;
    dds3Data34 = 1;
    gFilterFilterAlpOfs = 0;
    gFilterMode = 1;
    gFilterAnime = 0;
    gFilterAnimeCntF = 0;
    gFilterAnimeOfsA = 0;
    gFilterNowIdx = 0;
    gFilterBlendIdx = 0;
    gFilterBlend = 0;
    gFilterBlendSpd = 0;
    gFilterCursor = 0;
}
#else
INCLUDE_ASM(const s32, "dds3KernelDraw_0010BEE0", dds3FilterClear);
#endif

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

void dds3FilterSetAlpha(s32 alp)
{
    gFilterAlpha = alp;
}

s32 dds3FilterGetAlpha(void)
{
    return gFilterAlpha;
}

void dds3FilterSetAlphaOfs(s32 alp)
{
    gFilterFilterAlpOfs = alp;
}

s32 dds3FilterGetAlphaOfs(void)
{
    return gFilterFilterAlpOfs;
}

void dds3ProcSequenceSub(s32 mode)
{
    dds3Data34 = mode;
}

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

/* NON_MATCHING: 669 of 672 words match.  */
#ifdef NON_MATCHING
void dds3FilterDraw(void)
{
    s32 column, row;
    s32 current = gFilterNowIdx;
    s32 previous;
    s32 color0, color1, color2, color3;
    s32 r0, g0, b0, a0;
    s32 r1, g1, b1, a1;
    s32 r2, g2, b2, a2;
    s32 r3, g3, b3, a3;
    f32 currentWeight, previousWeight;
    s32 oldR0, oldG0, oldB0, oldA0;
    s32 oldR1, oldG1, oldB1, oldA1;
    s32 oldR2, oldG2, oldB2, oldA2;
    s32 oldR3, oldG3, oldB3, oldA3;

    if (current == 0 || dds3Data34 == 0) {
        return;
    }
    currentWeight = gFilterBlend;
    if (currentWeight < 1.0f) {
        currentWeight += 1.0f / gFilterBlendSpd;
        if (currentWeight > 1.0f) {
            currentWeight = 1.0f;
            gFilterBlend = currentWeight;
            gFilterBlendIdx = current;
        } else {
            gFilterBlend = currentWeight;
        }
    }
    previous = gFilterBlendIdx;
    previousWeight = 1.0f - currentWeight;

    eglSetOt(0x4F);
    eglLinkTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    switch (gFilterMode) {
    case 0:
        eglLinkAlpha(0);
        break;
    case 1:
        eglLinkAlpha(1);
        break;
    case 2:
        eglLinkAlpha(2);
        break;
    }

    for (row = 0; row < 7; row++) {
        for (column = 0; column < 8; column++) {
            color0 = gFilterBuff[current].colors[row][column];
            color1 = gFilterBuff[current].colors[row][column + 1];
            color2 = gFilterBuff[current].colors[row + 1][column + 1];
            color3 = gFilterBuff[current].colors[row + 1][column];
            r0 = color0 & 0xFF;
            g0 = (color0 >> 8) & 0xFF;
            b0 = (color0 >> 16) & 0xFF;
            a0 = (u32)color0 >> 24;
            r1 = color1 & 0xFF;
            g1 = (color1 >> 8) & 0xFF;
            b1 = (color1 >> 16) & 0xFF;
            a1 = (u32)color1 >> 24;
            r2 = color2 & 0xFF;
            g2 = (color2 >> 8) & 0xFF;
            b2 = (color2 >> 16) & 0xFF;
            a2 = (u32)color2 >> 24;
            r3 = color3 & 0xFF;
            g3 = (color3 >> 8) & 0xFF;
            b3 = (color3 >> 16) & 0xFF;
            a3 = (u32)color3 >> 24;
            a0 += gFilterAnimeOfsA;
            a1 += gFilterAnimeOfsA;
            a2 += gFilterAnimeOfsA;
            a3 += gFilterAnimeOfsA;
            a0 += gFilterFilterAlpOfs;
            a1 += gFilterFilterAlpOfs;
            a2 += gFilterFilterAlpOfs;
            a3 += gFilterFilterAlpOfs;
            if (a0 < 0) a0 = 0;
            if (a0 > 255) a0 = 255;
            if (a1 < 0) a1 = 0;
            if (a1 > 255) a1 = 255;
            if (a2 < 0) a2 = 0;
            if (a2 > 255) a2 = 255;

            if (gFilterBlend < 1.0f) {
                s32 oldColor0 = gFilterBuff[previous].colors[row][column];
                s32 oldColor1 = gFilterBuff[previous].colors[row][column + 1];
                s32 oldColor2 = gFilterBuff[previous].colors[row + 1][column + 1];
                s32 oldColor3 = gFilterBuff[previous].colors[row + 1][column];
                oldR0 = oldColor0 & 0xFF;
                oldG0 = (oldColor0 >> 8) & 0xFF;
                oldB0 = (oldColor0 >> 16) & 0xFF;
                oldA0 = (u32)oldColor0 >> 24;
                oldR1 = oldColor1 & 0xFF;
                oldG1 = (oldColor1 >> 8) & 0xFF;
                oldB1 = (oldColor1 >> 16) & 0xFF;
                oldA1 = (u32)oldColor1 >> 24;
                oldR2 = oldColor2 & 0xFF;
                oldG2 = (oldColor2 >> 8) & 0xFF;
                oldB2 = (oldColor2 >> 16) & 0xFF;
                oldA2 = (u32)oldColor2 >> 24;
                oldR3 = oldColor3 & 0xFF;
                oldG3 = (oldColor3 >> 8) & 0xFF;
                oldB3 = (oldColor3 >> 16) & 0xFF;
                oldA3 = (u32)oldColor3 >> 24;
                oldA0 += gFilterAnimeOfsA;
                oldA1 += gFilterAnimeOfsA;
                oldA2 += gFilterAnimeOfsA;
                oldA3 += gFilterAnimeOfsA;
                oldA0 += gFilterFilterAlpOfs;
                oldA1 += gFilterFilterAlpOfs;
                oldA2 += gFilterFilterAlpOfs;
                oldA3 += gFilterFilterAlpOfs;
                if (oldA0 < 0) oldA0 = 0;
                if (oldA0 > 255) oldA0 = 255;
                if (oldA1 < 0) oldA1 = 0;
                if (oldA1 > 255) oldA1 = 255;
                if (oldA2 < 0) oldA2 = 0;
                if (oldA2 > 255) oldA2 = 255;

                r0 = (s32)(r0 * currentWeight + oldR0 * previousWeight);
                g0 = (s32)(g0 * currentWeight + oldG0 * previousWeight);
                b0 = (s32)(b0 * currentWeight + oldB0 * previousWeight);
                a0 = (s32)(a0 * currentWeight + oldA0 * previousWeight);
                r1 = (s32)(r1 * currentWeight + oldR1 * previousWeight);
                g1 = (s32)(g1 * currentWeight + oldG1 * previousWeight);
                b1 = (s32)(b1 * currentWeight + oldB1 * previousWeight);
                a1 = (s32)(a1 * currentWeight + oldA1 * previousWeight);
                r2 = (s32)(r2 * currentWeight + oldR2 * previousWeight);
                g2 = (s32)(g2 * currentWeight + oldG2 * previousWeight);
                b2 = (s32)(b2 * currentWeight + oldB2 * previousWeight);
                a2 = (s32)(a2 * currentWeight + oldA2 * previousWeight);
                r3 = (s32)(r3 * currentWeight + oldR3 * previousWeight);
                g3 = (s32)(g3 * currentWeight + oldG3 * previousWeight);
                b3 = (s32)(b3 * currentWeight + oldB3 * previousWeight);
                a3 = (s32)(a3 * currentWeight + oldA3 * previousWeight);
            }

            if (gFilterAlpha != 0x80) {
                r0 = r0 * gFilterAlpha / 128;
                g0 = g0 * gFilterAlpha / 128;
                b0 = b0 * gFilterAlpha / 128;
                a0 = a0 * gFilterAlpha / 128;
                r1 = r1 * gFilterAlpha / 128;
                g1 = g1 * gFilterAlpha / 128;
                b1 = b1 * gFilterAlpha / 128;
                a1 = a1 * gFilterAlpha / 128;
                r2 = r2 * gFilterAlpha / 128;
                g2 = g2 * gFilterAlpha / 128;
                b2 = b2 * gFilterAlpha / 128;
                a2 = a2 * gFilterAlpha / 128;
                r3 = r3 * gFilterAlpha / 128;
                g3 = g3 * gFilterAlpha / 128;
                b3 = b3 * gFilterAlpha / 128;
                a3 = a3 * gFilterAlpha / 128;
            }
            evtSubmitDefaultDepthGradientRect(column * 64, row * 64, 64, 64,
                                              r0 | (g0 << 8) | (b0 << 16) | (a0 << 24),
                                              r1 | (g1 << 8) | (b1 << 16) | (a1 << 24),
                                              r3 | (g3 << 8) | (b3 << 16) | (a3 << 24),
                                              r2 | (g2 << 8) | (b2 << 16) | (a2 << 24));
        }
    }
    eglLinkTest(1, 5, 0x80, 1, 0, 0, 1, 2);
    eglLinkAlpha(0);
}
#else
INCLUDE_ASM(const s32, "dds3KernelDraw_0010BEE0", dds3FilterDraw);
#endif

INCLUDE_ASM(const s32, "dds3KernelDraw_0010BEE0", dds3FilterEdit);
