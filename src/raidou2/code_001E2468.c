#include "common.h"
#include "sdfMemory.h"
#include "sdfTex.h"
#include "sdk_libc.h"

sdfTexHandle_t *evtCallMemAllocDirect(void) {
    sdfMemHandle_t *mem;
    TMX_HEADER_t *tmx;
    sdfTexHandle_t *tex;
    s32 width = 32;
    s32 height = 16;
    s32 size = 0x180;

    mem = sdfMemAllocDirect(size);
    tmx = sdfMemLock(mem);
    memset(tmx, 0, size);
    tmx->fileType = 2;
    tmx->compression = 0;
    tmx->fileSize = size;
    tmx->formatTag[0] = 'T';
    tmx->formatTag[1] = 'M';
    tmx->formatTag[2] = 'X';
    tmx->formatTag[3] = '0';
    tmx->realSize = 0;
    tmx->numOfCLUT = 1;
    tmx->CLUTFormat = 0;
    tmx->width = width;
    tmx->height = height;
    tmx->pixelFormat = 20;
    tmx->numOfMipmap = 0;
    tmx->mipmapParam = 0;
    tmx->flags = 0;
    tex = sdfTexSet(tmx);
    sdfMemFreeDirect(mem);
    return tex;
}

INCLUDE_ASM(const s32, "code_001E2468", evtLeaf23);

INCLUDE_ASM(const s32, "code_001E2468", evtCallMemAllocDirect2);

INCLUDE_ASM(const s32, "code_001E2468", evtCallSetRotateMatrixAxis);

INCLUDE_ASM(const s32, "code_001E2468", evtCallTexSetFilter);

INCLUDE_ASM(const s32, "code_001E2468", evtCallMemset10);

INCLUDE_ASM(const s32, "code_001E2468", evtCallMemLock);

INCLUDE_ASM(const s32, "code_001E2468", evtCallGetProcessData20);

INCLUDE_ASM(const s32, "code_001E2468", evtCallGetProcessData21);

INCLUDE_ASM(const s32, "code_001E2468", evtCallGetProcessData22);

INCLUDE_ASM(const s32, "code_001E2468", evtCallMemset11);

INCLUDE_ASM(const s32, "code_001E2468", evtScreenBreakCreate);

INCLUDE_ASM(const s32, "code_001E2468", scrTestEntryFlag);
