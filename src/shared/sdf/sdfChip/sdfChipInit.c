#include "shared/sdf/sdfChip/decls.h"

/* Allocate the chip heap plus its page records, then build the free page list
 * and the size classes. */
void sdfChipInit(u32 aSize)
{
    u32 recordBytes;
    s32 align;
    s32 remaining;
    void *allocation;
    sdfChipPageInfo_tag *record;
    sdfChipPageInfo_tag *next;
    u8 *recordEnd;
    u8 *heapTop;
    u8 *page;
    s32 pageCount;
    sdfChipCB_tag *cb;
    s32 chipSize;
    s32 chipsPerPage;

    allocation = malloc(aSize);
    sdfChipPageInfo = allocation;
    recordBytes = (aSize / (SDF_CHIP_PAGE_BYTES + sizeof(sdfChipPageInfo_tag))) *
                  sizeof(sdfChipPageInfo_tag);
    recordEnd = (u8 *)allocation + recordBytes;
    record = allocation;
    sdfChipPageFreeLink = record;
    align = -EE_ADDR_S(recordEnd) & 0x3F;
    remaining = (aSize - recordBytes) - align;
    heapTop = recordEnd + align;
    sdfChipPageTop = heapTop;
    pageCount = remaining / SDF_CHIP_PAGE_BYTES;
    sdfChipPageCount = pageCount;
    sdfChipPageEnd = heapTop + pageCount * SDF_CHIP_PAGE_BYTES;
    page = heapTop;
    do {
        next = record + 1;
        pageCount--;
        record->page = page;
        page += SDF_CHIP_PAGE_BYTES;
        record->owner = NULL;
        record->next = next;
        record = next;
    } while (pageCount != 0);
    next[-1].next = NULL;

    cb = sdfChipCB;
    chipSize = 0x10;
    chipsPerPage = 0x100;
    pageCount = SDF_CHIP_CLASS_COUNT;
    do {
        pageCount--;
        cb->chipSize = chipSize;
        cb->chipsPerPage = chipsPerPage;
        chipsPerPage >>= 1;
        cb->currentPage = NULL;
        chipSize <<= 1;
        cb->freeLink = NULL;
        cb++;
    } while (pageCount != 0);
    sdfDelayedResrcInitCB(&dds3CleanupList, (sdfDelayedResrcFreeFunc)sdfChipFreeDirect);
}
