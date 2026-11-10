#include "shared/sdf/sdfChip/decls.h"

sdfChip_t *sdfChipAllocNewPage(sdfChipCB_tag *aCB)
{
    sdfChipPageInfo_tag *page = sdfChipPageFreeLink;

    sdfChipPageFreeLink = page->next;
    page->allocCount = 0;
    page->alloc = (void *)sdfChipAllocPageGrow;
    aCB->currentPage = page;
    page->owner = aCB;
    page->growLeft = aCB->chipsPerPage;
    return sdfChipAllocPageGrow(aCB, page);
}
