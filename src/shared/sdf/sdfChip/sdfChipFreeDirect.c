#include "shared/sdf/sdfChip/decls.h"

void sdfChipFreeDirect(void *aPtr)
{
    sdfChipPageInfo_tag *page;
    sdfChipCB_tag *cb;
    sdfChip_t *chip = aPtr;
    sdfChipPageInfo_tag **link;
    s32 intrEnabled;
    s16 allocCount;
    saddr_t heapByteOffset;

    if (aPtr == NULL) {
        return;
    }
    heapByteOffset = EE_ADDR_S(aPtr) - EE_ADDR_S(sdfChipPageTop);
    if (heapByteOffset < 0) {
        heapByteOffset += SDF_CHIP_PAGE_BYTES - 1;
    }
    page = (sdfChipPageInfo_tag *)((u8 *)sdfChipPageInfo +
                                   (heapByteOffset >> SDF_CHIP_PAGE_SHIFT) *
                                       sizeof(sdfChipPageInfo_tag));
    cb = page->owner;
    intrEnabled = DIntr();
    allocCount = page->allocCount;
    page->allocCount = allocCount - 1;
    if (page->allocCount == 0) {
        link = &cb->freeLink;
        if (cb->currentPage == page) {
            sdfChipAllocNextPage(cb);
        } else {
            while (*link != page) {
                link = &(*link)->next;
            }
            *link = page->next;
        }
        page->owner = NULL;
        page->next = sdfChipPageFreeLink;
        sdfChipPageFreeLink = page;
    } else {
        if (allocCount == cb->chipsPerPage) {
            page->next = cb->freeLink;
            cb->freeLink = page;
        }
        chip->nextFree = page->freeLink;
        page->freeLink = chip;
    }
    if (intrEnabled != 0) {
        EIntr();
    }
}
