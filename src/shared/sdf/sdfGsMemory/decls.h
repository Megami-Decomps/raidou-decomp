/* Declarations of smt3-decomp src/smt3/sdf/sdfGsMemory.c, for its bodies in this directory. */
#ifndef SHARED_SDF_SDFGSMEMORY_H
#define SHARED_SDF_SDFGSMEMORY_H

#include "common.h"

#include "libsdf.h"

#include "ps2asm.h"

extern void sdfDelayedResrcEntry(sdfDelayedResrcCB_t *aCB, void *aItem);

/* GS pixel storage modes, the values of sdfGsMemSlice_t.pixelFormat. */
enum sdf_psm
{
    SDF_PSMCT32 = 0x00,
    SDF_PSMCT24 = 0x01,
    SDF_PSMCT16 = 0x02,
    SDF_PSMCT16S = 0x0A,
    SDF_PSMT8 = 0x13,
    SDF_PSMT4 = 0x14,
    SDF_PSMT8H = 0x1B,
    SDF_PSMT4HL = 0x24,
    SDF_PSMT4HH = 0x2C,
    SDF_PSMZ32 = 0x30,
    SDF_PSMZ24 = 0x31,
    SDF_PSMZ16 = 0x32,
    SDF_PSMZ16S = 0x3A
};

extern s32 DIntr(void);

extern void EIntr(void);

extern void *sdfChipAllocDirect(u32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

extern void sdfDelayedResrcInitCB(sdfDelayedResrcCB_t *aCB, sdfDelayedResrcFreeFunc aFreeFunc);

extern sdfGsMemSlice_t *gsMemHandleLinkHead;

extern sdfGsMemSlice_t *gsMemHandleLinkTail;

extern sdfDelayedResrcCB_t gsMemDelayedResrcCB;

extern void (*sdfGsErrorHandler)(s32 size, s32 type);

/* sdfImageSetPacket_tag.freeMode: which heap owns the uploaded pixels. */
enum SdfImageSetMem
{
    SDF_IMAGESET_MEM_IGNORE = 0,
    SDF_IMAGESET_MEM_HANDLE = 1,
    SDF_IMAGESET_MEM_CHIP = 2
};

s32 sdfGsMemGetBitCount(u32 aFormat);

sdfGsMemSlice_t *sdfGsMemAllocFromHead(s32 aSize, s32 aType);

sdfGsMemSlice_t *sdfGsMemAllocFromTail(s32 aSize, s32 aType);

s32 sdfGsMemCheckMerge(sdfGsMemSlice_t *aHandle);

void sdfGsMemFree(sdfGsMemSlice_t *aHandle);

s32 sdfGsMemGetBitCount(u32 aFormat);

s32 sdfGetTexelBitCount(u32 aFormat);

s32 sdfGsMemCheckHandle(sdfGsMemSlice_t *aHandle);

sdfGsMemSlice_t *sdfGsMemAllocFromHead(s32 aSize, s32 aType);

sdfGsMemSlice_t *sdfGsMemAllocFromTail(s32 aSize, s32 aType);

sdfGsMemSlice_t *sdfGsMemAllocRect(s32 aWidth, s32 aHeight, s32 aFormat, s32 aType, s32 aFromHead);

s32 sdfGsMemCheckMerge(sdfGsMemSlice_t *aHandle);

void sdfGsMemCheckLinkConsistence(void);

void sdfGsMemFree(sdfGsMemSlice_t *aHandle);

void sdfGsMemFreeDelayed(sdfGsMemSlice_t *aHandle);

void sdfGsMemInit(void);

sdfGsMemSlice_t *sdfGsAllocFrameBuff(s32 aW, s32 aH, s32 aPixelFormat);

sdfGsMemSlice_t *sdfGsMemGetLinkHead(void);

sdfGsMemSlice_t *sdfGsMemGetLinkTail(void);

#endif
