#include "common.h"
#include "sdfTex.h"
#include "sdfSound.h"
#include "effBlur_Filter.h"
#include "fldEnc.h"
#include "dds3KernelDraw.h"
#include "fldDraw.h"
#include "sdfPrim.h"
#include "sdfPacketBuff.h"
#include "sdf_packet_list.h"
#include "kernel_draw.h"
#include "dds3KernelMain.h"
#include "sdfDraw.h"
#include "fileManager.h"
#include "sdk_libc.h"
#include "sdf2D.h"
#include "sdfMemory.h"

void encLoadTexture(void)
{
    char path[32];
    fileRequest_t *pac;
    sdfPacDecodeResrc_t *resrc;

    sprintf(path, encStrMapMap39Pac00BLb);
    pac = pacLoad(path);
    pacSync(pac);
    for (resrc = pac->pac.resrcHead; resrc != NULL; resrc = resrc->next) {
        if (((sdfPacStdBlockHeader_t *)resrc->header)->kind == 1) {
            fldData42 = sdfTexSet(sdfMemGetPtr(resrc->resrc));
            sdfMemFreeDelayedDirect(resrc->resrc);
        } else {
            sdfMemFreeDirect(resrc->resrc);
        }
    }
    pacFileHandleFree_(pac);
    fldCallSetOt5Data = 512.0f;
    fldCallSetOt5Data2 = 0.0f;
}

void fldFreeRunMapData(void)
{
    if (fldData42 != NULL) {
        sdfTexFreeDelayed(fldData42);
        fldData42 = NULL;
        fldCallSetOt5Data = 0.0f;
        fldCallSetOt5Data2 = 0.0f;
    }
}

void fldCallSetOt5(void)
{
    if (fldData42 == NULL) {
        return;
    }
    if (fldCallSetOt5Data + 256.0f + 128.0f + 128.0f + 256.0f + 200.0f < 0.0f) {
        return;
    }
    eglSetOt(101);
    eglLinkAlpha(0);
    eglLinkTest(1, 0, 128, 1, 0, 0, 1, 1);
    eglCallDLinkTexPacket2(fldCallSetOt5Data + 0.0f, 0.0f, 968.0f, 448.0f, 0.0f, 0.0f, 256.0f, 256.0f,
                           0x80808080, 0x80808080, 0x80808080, 0x80808080, fldData42);
    fldCallSetOt5Data -= fldCallSetOt5Data2 + 4.0f;
    fldCallSetOt5Data2 += 2.5f;
}

void encDrawBokasi0(void)
{
    sdfDrawTag_t *list = sdfPrimAllocDrawTag(NULL);
    sdfGrRefPacketHeader_t *reference = EE_ADDR_AS(sdfGrRefPacketHeader_t *, sdfGrAllocPacketBuff(0x20));
    u64 *texturePacket;
    u64 *blendPacket;
    sdfGrPacketHeader_t *sprite;
    fldSpriteVertex_t *vertex;
    sdfGrOt_t *surface;

    sdfGrLinkDispFrameTex(list, EE_ADDR(&dds3DrawEnv[dds3GetFrameID()].stdEnv), reference);
    texturePacket = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(0x40));
    texturePacket[0] = 3;
    texturePacket[1] = 0x5000000310000000ULL;
    texturePacket[2] = 0x1000000000008002ULL;
    texturePacket[3] = 0xE;
    texturePacket[4] = 0x8000000080ULL;
    texturePacket[5] = 0x3B;
    texturePacket[6] = 0;
    texturePacket[7] = 0x3F;
    sdfDrawTagLink(list, EE_ADDR(texturePacket));
    blendPacket = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(0x40));
    blendPacket[0] = 3;
    blendPacket[1] = 0x5000000310000000ULL;
    blendPacket[2] = 0x1000000000008002ULL;
    blendPacket[3] = 0xE;
    blendPacket[4] = 0x33001;
    blendPacket[5] = 0x47;
    blendPacket[6] = 0x2000000064ULL;
    blendPacket[7] = 0x42;
    sdfDrawTagLink(list, EE_ADDR(blendPacket));
    sprite = sdf2DAllocSprite(1);
    vertex = sdf2DGetPrimPtr(sprite);
    vertex->r = 0x8A;
    vertex->g = 0x8A;
    vertex->b = 0x8A;
    vertex->a = 0x30;
    vertex->corner[0].u = 0xC8;
    vertex->corner[0].v = 0x64;
    vertex->corner[1].u = 0x1F38;
    vertex->corner[1].v = 0xD9B;
    vertex->corner[0].x = 0x7000;
    vertex->corner[0].y = 0x7900;
    vertex->corner[1].x = 0x9000;
    vertex->corner[1].y = 0x8700;
    vertex->corner[0].mask = 0;
    vertex->corner[0].flag = 0;
    vertex->corner[1].mask = 0;
    vertex->corner[1].flag = 0;
    sdfDrawTagLink(list, EE_ADDR(sprite));
    surface = &sdfGrOtTable[90];
    surface->regFunc(surface, list);
}

void encCallPrimAllocDrawTag(void)
{
    sdfDrawTag_t *list = sdfPrimAllocDrawTag(NULL);
    sdfGrRefPacketHeader_t *reference = EE_ADDR_AS(sdfGrRefPacketHeader_t *, sdfGrAllocPacketBuff(0x20));
    u64 *texturePacket;
    u64 *blendPacket;
    sdfGrPacketHeader_t *sprite;
    fldSpriteVertex_t *vertex;
    sdfGrOt_t *surface;

    sdfGrLinkDispFrameTex(list, EE_ADDR(&dds3DrawEnv[dds3GetFrameID()].stdEnv), reference);
    texturePacket = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(0x40));
    texturePacket[0] = 3;
    texturePacket[1] = 0x5000000310000000ULL;
    texturePacket[2] = 0x1000000000008002ULL;
    texturePacket[3] = 0xE;
    texturePacket[4] = 0x8000000080ULL;
    texturePacket[5] = 0x3B;
    texturePacket[6] = 0;
    texturePacket[7] = 0x3F;
    sdfDrawTagLink(list, EE_ADDR(texturePacket));
    blendPacket = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(0x40));
    blendPacket[0] = 3;
    blendPacket[1] = 0x5000000310000000ULL;
    blendPacket[2] = 0x1000000000008002ULL;
    blendPacket[3] = 0xE;
    blendPacket[4] = 0x33001;
    blendPacket[5] = 0x47;
    blendPacket[6] = 0x8000000064ULL;
    blendPacket[7] = 0x42;
    sdfDrawTagLink(list, EE_ADDR(blendPacket));
    sprite = sdf2DAllocSprite(1);
    vertex = sdf2DGetPrimPtr(sprite);
    vertex->r = 0x81;
    vertex->g = 0x81;
    vertex->b = 0x81;
    vertex->a = 0x80;
    vertex->corner[0].u = 0;
    vertex->corner[0].v = 0;
    vertex->corner[1].u = 0x2000;
    vertex->corner[1].v = 0xDFF;
    vertex->corner[0].x = 0x6FF8;
    vertex->corner[0].y = 0x78FB;
    vertex->corner[1].x = 0x8FF8;
    vertex->corner[1].y = 0x86FB;
    vertex->corner[0].mask = 0;
    vertex->corner[0].flag = 0;
    vertex->corner[1].mask = 0;
    vertex->corner[1].flag = 0;
    sdfDrawTagLink(list, EE_ADDR(sprite));
    surface = &sdfGrOtTable[rsetFrameMaskOtOffset];
    surface->regFunc(surface, list);
}

INCLUDE_ASM(const s32, "code_00186680", encCallPrimAllocDrawTag2);

s32 encLoad(void) {
    return gencOn;
}

void encMode(s32 mode)
{
    gencMode = mode;
    if (mode == 1) {
        p2sdPlaySE(14, 127, 63);
    }
}

s32 encDrawProc(dds3ProcessID_t *aProcId)
{
    if (gencOn == 0) {
        return 0;
    }
    if (gencMode == 0) {
        rsetOtOfs(90);
        rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
        rsetAlpha(0);
        encCallPrimAllocDrawTag();
        aDrawBoxF(0, 0xDF, 0x200, 2, 0, 0, 0, 0x80);
        rsetAlpha(0);
        encDrawState = encDrawState + 1;
    }
    if (gencMode == 1) {
        if (gencStageInnAlp > 0) {
            rsetOtOfs(101);
            rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
            rsetAlpha(0);
            encCallPrimAllocDrawTag2(0x80);
            aDrawBoxF(0, 0xDF, 0x200, 2, 0, 0, 0, 0x80);
            gencStageInnAlp = gencStageInnAlp - 1;
        } else {
            encStop();
            return 0;
        }
        fldCallSetOt5();
    }
    return 0;
}

void encStop(void)
{
    gencOn = 0;
    if (hPartialMultiBlur != NULL) {
        effStopSub(hPartialMultiBlur);
        hPartialMultiBlur = NULL;
    }
    fldFreeRunMapData();
}

INCLUDE_RODATA(const s32, "code_00186680", encStrMapMap39Pac00BLb);

