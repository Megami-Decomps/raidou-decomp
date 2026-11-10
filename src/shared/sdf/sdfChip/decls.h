/* Declarations of smt3-decomp src/smt3/sdf/sdfChip.c, for its bodies in this directory. */
#ifndef SHARED_SDF_SDFCHIP_H
#define SHARED_SDF_SDFCHIP_H

#include "common.h"

#include "libsdf.h"

#include "ps2asm.h"

/* A free chip carries its free-list link at byte 8. */
typedef struct sdfChip_t
{
    u8 pad00[8];
    void *nextFree;
} sdfChip_t;

#define SDF_CHIP_CLASS_COUNT 7

#define SDF_CHIP_PAGE_SHIFT 12

#define SDF_CHIP_PAGE_BYTES (1 << SDF_CHIP_PAGE_SHIFT)

extern void *malloc(size_t size);

extern s32 DIntr(void);

extern void EIntr(void);

extern void *sdfZeroMemQuad(void *ptr, s32 quadCount);

extern void sdfDelayedResrcInitCB(sdfDelayedResrcCB_t *aCB, sdfDelayedResrcFreeFunc aFreeFunc);

extern void sdfDelayedResrcEntry(sdfDelayedResrcCB_t *aCB, void *aItem);

extern sdfDelayedResrcCB_t dds3CleanupList;

extern sdfChipCB_tag sdfChipCB[SDF_CHIP_CLASS_COUNT];

extern sdfChipPageInfo_tag *sdfChipPageInfo;

extern sdfChipPageInfo_tag *sdfChipPageFreeLink;

extern u8 *sdfChipPageTop;

extern u8 *sdfChipPageEnd;

extern s32 sdfChipPageCount;

void sdfChipAllocNextPage(sdfChipCB_tag *aCB);

sdfChip_t *sdfChipAllocGetFree(sdfChipCB_tag *aCB, sdfChipPageInfo_tag *aPageInfo);

sdfChip_t *sdfChipAllocPageGrow(sdfChipCB_tag *aCB, sdfChipPageInfo_tag *aPageInfo);

void *sdfChipAllocDirect(s32 aSize);

void sdfChipFreeDirect(void *aPtr);

/* Promote the next page on the partial list to current; an empty list clears it. */
void sdfChipAllocNextPage(sdfChipCB_tag *aCB);

/* Pop a recycled chip and advance when its page becomes full. */
sdfChip_t *sdfChipAllocGetFree(sdfChipCB_tag *aCB, sdfChipPageInfo_tag *aPageInfo);

/* Hand out the next never-used chip; the last one switches the page to the
 * free-list handler. */
sdfChip_t *sdfChipAllocPageGrow(sdfChipCB_tag *aCB, sdfChipPageInfo_tag *aPageInfo);

sdfChip_t *sdfChipAllocNewPage(sdfChipCB_tag *aCB);

/* Allocate from the size class that covers the request; classes are powers of
 * two from 16 bytes up, and each page hands out from its own chip list. */
void *sdfChipAllocDirect(s32 aSize);

void *sdfChipAllocZeroDirect(s32 aSize);

void sdfChipFreeDirect(void *aPtr);

void sdfChipFreeDelayedDirect(void *aPtr);

s32 sdfChipCheck(s32 aPtr);

/* Allocate the chip heap plus its page records, then build the free page list
 * and the size classes. */
void sdfChipInit(u32 aSize);

void sdfChipGetInfo(sdfChipInfo_t *aInfo);

#endif
