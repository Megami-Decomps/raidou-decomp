#include "shared/sdf/sdfChip/decls.h"

/* Hand out the next never-used chip; the last one switches the page to the
 * free-list handler. */
sdfChip_t *sdfChipAllocPageGrow(sdfChipCB_tag *aCB, sdfChipPageInfo_tag *aPageInfo)
{
    s16 chipsPerPage = aCB->chipsPerPage;
    s32 allocCount = (u16)aPageInfo->allocCount + 1;
    s32 growLeft = aPageInfo->growLeft;
    sdfChip_t *chip;

    aPageInfo->allocCount = allocCount;
    chip = (sdfChip_t *)((u8 *)aPageInfo->page + (chipsPerPage - growLeft) * aCB->chipSize);
    aPageInfo->growLeft = growLeft - 1;
    if (aPageInfo->growLeft == 0) {
        aPageInfo->alloc = (void *)sdfChipAllocGetFree;
        if ((s16)allocCount == chipsPerPage) {
            sdfChipAllocNextPage(aCB);
        }
    }
    return chip;
}
