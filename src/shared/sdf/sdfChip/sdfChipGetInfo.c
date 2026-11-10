#include "shared/sdf/sdfChip/decls.h"

void sdfChipGetInfo(sdfChipInfo_t *aInfo)
{
    sdfChipPageInfo_tag *page;
    s32 pagesLeft;
    s32 index;
    s32 fragmentedPageCount;
    s32 freePageCount;
    s32 freeSize;
    s32 freeChips;

    pagesLeft = sdfChipPageCount;
    aInfo->pageCount = pagesLeft;
    aInfo->total = pagesLeft << SDF_CHIP_PAGE_SHIFT;
    for (index = 0; index != SDF_CHIP_CLASS_COUNT; index++) {
        aInfo->severalAllocCount[index] = 0;
    }
    page = sdfChipPageInfo;
    freePageCount = 0;
    fragmentedPageCount = 0;
    freeSize = 0;
    /* This post-tested walk assumes a nonzero page count. */
    do {
        if (page->owner == NULL) {
            freePageCount++;
            freeSize += SDF_CHIP_PAGE_BYTES;
        } else {
            freeChips = page->owner->chipsPerPage - page->allocCount;
            if (freeChips != 0) {
                fragmentedPageCount++;
                freeSize += freeChips * page->owner->chipSize;
            }
            aInfo->severalAllocCount[page->owner - sdfChipCB] += page->allocCount;
        }
        page++;
    } while (--pagesLeft != 0);
    aInfo->freeSize = freeSize;
    aInfo->freePageCount = freePageCount;
    aInfo->fragmentedPageCount = fragmentedPageCount;
}
