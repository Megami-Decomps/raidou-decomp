#include "shared/sdf/sdfChip/decls.h"

/* Pop a recycled chip and advance when its page becomes full. */
sdfChip_t *sdfChipAllocGetFree(sdfChipCB_tag *aCB, sdfChipPageInfo_tag *aPageInfo)
{
    sdfChip_t *chip = aPageInfo->freeLink;

    aPageInfo->allocCount++;
    aPageInfo->freeLink = chip->nextFree;
    if (aPageInfo->allocCount == aCB->chipsPerPage) {
        sdfChipAllocNextPage(aCB);
    }
    return chip;
}
